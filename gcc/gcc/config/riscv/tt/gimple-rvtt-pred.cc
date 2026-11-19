/* Pass to expand (lower) boolean SFPU operators
   Copyright (C) 2022-2026 Tenstorrent Inc.
   Originated by Paul Keller (pkeller@tenstorrent.com).
   Rewritten by Nathan Sidwell (nsidwell@tenstorrent.com, nathan@acm.org).

This file is part of GCC.

GCC is free software; you can redistribute it and/or modify it under
the terms of the GNU General Public License as published by the Free
Software Foundation; either version 3, or (at your option) any later
version.

GCC is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or
FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
for more details.

You should have received a copy of the GNU General Public License
along with GCC; see the file COPYING3.  If not see
<http://www.gnu.org/licenses/>.  */

#define INCLUDE_ALGORITHM
#define INCLUDE_VECTOR
#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "backend.h"
#include "rtl.h"
#include "tree.h"
#include "gimple.h"
#include "tree-pass.h"
#include "ssa.h"
#include "gimple-iterator.h"
#include "gimple-pretty-print.h"
#include "tree-into-ssa.h"
#include "diagnostic-core.h"
#include "rvtt.h"
#include <deque>
#include <unordered_map>
#include <unordered_set>

namespace {
// A predicate node marks an block.if/else/end.
// We constrain the graph.  The graph doesn't persist beyond the current
// function's processing.

enum expand_flags {
  EF_none,
  EF_negate = 1 << 0,
  EF_nearby = 1 << 1,
};
inline expand_flags &operator^= (expand_flags &v, expand_flags bit) {
  v = expand_flags (v ^ bit);
  return v;
}
inline expand_flags &operator|= (expand_flags &v, expand_flags bit) {
  v = expand_flags (v | bit);
  return v;
}

struct GTY((chain_next ("%h.next"))) pred_node
{
  pred_node *parent = nullptr; // Node within-which we reside

  pred_node *child = nullptr; // predicates inside this node
  pred_node *first = nullptr; // First node of this sequence

  pred_node *next = nullptr; // next pred, last points to next sequence

  gcall *call = nullptr; // The call marking this point
  gcall *cond = nullptr; // The sfpxcond (if applicable)

  unsigned mod = 0; // The mod flags for this node
  unsigned depth = 0;

 public:
  pred_node (gcall *call, unsigned xmod)
    :call (call), mod (xmod & ((1 << SFPXPRED_MOD1_DEPTH_SHIFT) - 1)),
    depth (xmod >> SFPXPRED_MOD1_DEPTH_SHIFT) {}
  void print (FILE *stream, unsigned indent = 0) const;

 public:
  struct builder
  {
    std::unordered_map<tree, pred_node *> map;
    std::vector<pred_node *> nodes;
    std::vector<std::pair<pred_node *, pred_node *>> sequencing;
    pred_node *current = nullptr;

    void set_node (pred_node *node) {
      current = node;
    }
    pred_node *get_node () const {
      return current;
    }

    void record (pred_node *node, gcall *call, bool begin = false) {
      if (begin)
	sequencing.emplace_back (node, current);
      current = node;
      nodes.push_back (node);
      if (auto lhs = gimple_call_lhs (call))
	map.insert ({lhs, node});
    }

    bool assemble ();
    bool sequence ();
    bool check_sequences () const;
    bool check_control_flows (function *) const;

  private:
    pred_node *find (std::unordered_set<gphi *> &phis, tree var, pred_node *bad) const;
  };
};
}

void
pred_node::print (FILE *stream, unsigned indent) const
{
  static const char spaces[] = "          ";

  fprintf (stream, "%.*s0x%x, %d ", indent * 2, spaces,
	   mod, depth);
  print_gimple_stmt (stream, call, 0);
  if (cond)
    {
      fprintf (stream, "%.*s  cond:", indent * 2, spaces);
      print_gimple_stmt (stream, cond, 0);
    }
  if (child)
    child->print (stream, indent + 1);
  if (next)
    next->print (stream, indent);
}

pred_node *
pred_node::builder::find (std::unordered_set<gphi *> &phis, tree var, pred_node *bad) const
{
  auto def = SSA_NAME_DEF_STMT (var);
  if (auto *phi = dyn_cast <gphi *> (def))
    {
      if (!phis.insert (phi).second)
	return nullptr;

      pred_node *result = nullptr;
      use_operand_p arg_p;
      ssa_op_iter iter;
      FOR_EACH_PHI_ARG (arg_p, phi, iter, SSA_OP_USE)
	{
	  auto r = find (phis, USE_FROM_PTR (arg_p), bad);
	  if (r)
	    {
	      if (!result)
		result = r;
	      else if (result != bad)
		result = bad;
	    }
	}
      return result;
    }

  auto prev = map.find (var);
  if (prev != map.end () && prev->second->first)
    return prev->second;

  return nullptr;
}

// Join sequences togetther

bool
pred_node::builder::assemble ()
{
  std::vector<pred_node *> fragments;
  auto slot = nodes.begin ();
  for (auto node : nodes)
    {
      auto dep = gimple_call_arg (node->call, 1);
      if (SSA_VAR_P (dep))
	{
	  auto prev = map.find (dep);
	  if (prev != map.end ())
	    {
	      if (prev->second->next)
		{
		  error_at (gimple_location (prev->second->call),
			    "multiple %<v_if%> fragments follow");
		  inform (gimple_location (prev->second->next->call),
			  "first follower");
		  inform (gimple_location (node->call),
			  "second follower");
		  return true;
		}
	      prev->second->next = node;
	    }
	  else
	    {
	      auto def = SSA_NAME_DEF_STMT (dep);
	      if (!is_a <gphi *> (def))
		{
		  error_at (gimple_location (node->call),
			    "%<v_if%> depency chain has unexpected source");
		  inform (gimple_location (def), "deoenency source");
		  return true;
		}
	      fragments.push_back (node);
	    }
	}
      else
	*slot++ = node;
    }

  nodes.erase (slot, nodes.end ());

  // thread up the roots
  for (auto node : nodes)
    {
      for (auto probe = node; probe; probe = probe->next)
	probe->first = node;

      if (dump_file && (dump_flags & TDF_DETAILS))
	{
	  fprintf (dump_file, "Sequence %p:\n", (void const *)node);
	  node->print (dump_file);
	  fprintf (dump_file, "\n");
	}
    }

  // attach the fragments
  std::unordered_set<gphi *> phis;
  for (auto frag : fragments)
    {
      auto prev = find (phis, gimple_call_arg (frag->call, 1), frag);
      phis.clear ();
      if (prev == frag || !prev || prev->next)
	{
	  error_at (gimple_location (prev->call),
		    "unexpected %<v_if%> sequence ordering");
	  return true;
	}
      prev->next = frag;
      for (auto probe = frag; probe; probe = probe->next)
	probe->first = prev->first;
    }

  return false;
}

// Verify that we can't already get from NODE to TARGET

static bool
check_unlooped (pred_node const *node, pred_node const *target)
{
  for (; node; node = node->next)
    {
      if (node == target)
	return false;

      if (!check_unlooped (node->child, target))
	return false;
    }
  return true;
}

bool
pred_node::builder::sequence ()
{
  current = nullptr;

  // Iterate in reverse order so we end up with the best approximation of
  // forward order
  for (auto I = sequencing.rbegin (); I != sequencing.rend (); ++I)
    {
      auto [node, prev] = *I;

      if (node->first != node)
	continue;

      pred_node *end = node;
      while (end->next)
	end = end->next;
      if (end->mod != SFPXPRED_MOD1_END)
	{
	  error_at (gimple_location (end->call),
		    "unterminate %<v_if%> sequence");
	  return true;
	}

      auto from = prev ? prev : current;
      gcc_checking_assert (check_unlooped (node, from) && check_unlooped (from, node));
      if (!prev)
	{
	  // Is first
	  end->next = current;
	  current = node;
	}
      else if (prev->mod == SFPXPRED_MOD1_END)
	{
	  // Is following
	  end->next = prev->next;
	  prev->next = node;
	}
      else
	{
	  // Is child
	  end->next = prev->child;
	  prev->child = node;
	}
    }

  return false;
}

bool
pred_node::builder::check_sequences () const
{
  for (auto root : nodes)
    {
      unsigned d = 0;
      for (pred_node const *prev = nullptr, *probe = root;
	   probe; prev = probe, probe = probe->next)
	{
	  if (!prev)
	    {
	      if ((probe->mod & ~SFPXPRED_MOD1_IF) != SFPXPRED_MOD1_PUSH)
		{
		  error_at (gimple_location (probe->call), "incorrectly nesting at %<v_if%>/%<v_block%> start");
		  return true;
		}
	    }
	  else
	    switch (probe->mod)
	      {
	      case SFPXPRED_MOD1_END:
	      case SFPXPRED_MOD1_IF:
		// Allowed anywhere
		break;

	      case SFPXPRED_MOD1_ELSE:
	      case SFPXPRED_MOD1_IF | SFPXPRED_MOD1_ELSE | SFPXPRED_MOD1_PUSH:
		// Only allowed not in a block, cannot follow ELSE
		if ((probe->first->mod & SFPXPRED_MOD1_IF)
		    && (prev->mod != SFPXPRED_MOD1_ELSE))
		  break;
		[[fallthrough]];

	      default:
		error_at (gimple_location (probe->call),
			  "incorrect sequencing of %<v_if%>");
		inform (gimple_location (prev->call),
			"previous %<v_if%> element here");
		return true;
	      }

	  if (bool (gimple_call_lhs (probe->cond ? probe->cond : probe->call))
	      == (probe->mod == SFPXPRED_MOD1_END))
	    {
	      error_at (gimple_location (probe->cond ? probe->cond : probe->call),
			probe->mod == SFPXPRED_MOD1_END
			? "unexpectedly unterminated %<v_endif%>"
			: "unexpectedly terminated %<v_if%> sequence");
	      return true;
	    }

	  if (probe->mod & SFPXPRED_MOD1_PUSH)
	    d++;
	  if (d != probe->depth)
	    {
	      error_at (gimple_location (probe->call),
			"inconsistent %<v_if%> depth information");
	      return true;
	    }
	}
    }
  return false;
}

static bool
check_sese (std::vector<basic_block> &worklist,
	    std::unordered_set<basic_block> &visited,
	    function *fn,
	    pred_node *node)
{
  // start should dominate end and end post-dominate start.  Anything
  // else means there's control flow in or out.  It's more than likely
  // quicker to walk the graph to see if we reach the entry block or the
  // exit block, than actually compute dominance info

  basic_block start = gimple_bb (node->call);
  basic_block end = gimple_bb (node->next->call);
  if (start == end)
    return false;

  visited.clear ();
  visited.insert (end);
  worklist.push_back (start);
  while (!worklist.empty ())
    {
      auto bb = worklist.back ();
      worklist.pop_back ();
      if (visited.insert (bb).second)
	{
	  if (bb == EXIT_BLOCK_PTR_FOR_FN (fn))
	    {
	      error_at (gimple_location (node->call),
			"control flow out of %<v_if%> sequence");
	      inform (gimple_location (node->next->call),
		      "next %<v_if%> element here");
	      return true;
	    }

	  edge e;
	  edge_iterator ei;
	  FOR_EACH_EDGE (e, ei, bb->succs)
	    if (e->dest != end)
	      worklist.push_back (e->dest);
	}
    }

  visited.clear ();
  visited.insert (start);
  worklist.push_back (end);
  while (!worklist.empty ())
    {
      auto bb = worklist.back ();
      worklist.pop_back ();
      if (visited.insert (bb).second)
	{
	  // C++'s constructor paradigm should catch this
	  if (bb == ENTRY_BLOCK_PTR_FOR_FN (fn))
	    {
	      error_at (gimple_location (node->call),
			"control flow in to %<v_if%> sequence");
	      inform (gimple_location (node->next->call),
		      "next %<v_if%> element here");
	      return true;
	    }
	  edge e;
	  edge_iterator ei;
	  FOR_EACH_EDGE (e, ei, bb->preds)
	    if (e->src != start)
	      worklist.push_back (e->src);
	}
    }

  return false;
}

static bool
check_sequences_sese (std::vector<basic_block> &worklist,
		     std::unordered_set<basic_block> &visited,
		     function *fn,
		     pred_node *node)
{
  if (!node)
    return false;

  for (; node->next; node = node->next)
    {
      if (node->mod == SFPXPRED_MOD1_END)
	{
	  gcc_checking_assert (!node->child);
	  continue;
	}

      if (check_sese (worklist, visited, fn, node))
	return true;

      if (check_sequences_sese (worklist, visited, fn, node->child))
	return true;
    }

  return false;
}

bool
pred_node::builder::check_control_flows (function *fn) const
{
  std::vector<basic_block> worklist;
  std::unordered_set<basic_block> visited;

  return check_sequences_sese (worklist, visited, fn, get_node ());
}

static bool v_if_ok;
static GTY(()) pred_node *predicates;

using call_vec_t = std::vector<gcall *>;

static bool expand_cond (call_vec_t &, unsigned &ix,
			 gimple_stmt_iterator *leftmost, gimple_stmt_iterator *rightmost,
			 tree var, gcall *sink, expand_flags flags);

static void
finish_new_insn (gimple_stmt_iterator *gsip, bool insert_before, gimple *new_stmt, gcall *stmt)
{
  gimple_set_location (new_stmt, gimple_location (stmt));
  if (insert_before)
    gsi_insert_before (gsip, new_stmt, GSI_NEW_STMT);
  else
    gsi_insert_after (gsip, new_stmt, GSI_NEW_STMT);
}

static void
emit_pushc (gimple_stmt_iterator *gsip, gcall *stmt, bool insert_before)
{
  const rvtt_insn_data *new_insnd =
    rvtt_get_insn_data(rvtt_insn_data::sfppushc);
  gimple *new_stmt = gimple_build_call(new_insnd->decl (), 1,
				       build_int_cst (unsigned_type_node, SFPPUSHCC_MOD1_PUSH));
  finish_new_insn (gsip, insert_before, new_stmt, stmt);
}

static void
emit_popc (gimple_stmt_iterator *gsip, gcall *stmt, bool insert_before)
{
  const rvtt_insn_data *new_insnd =
    rvtt_get_insn_data(rvtt_insn_data::sfppopc);
  gimple *new_stmt = gimple_build_call(new_insnd->decl (), 1,
				       build_int_cst (unsigned_type_node, SFPPOPCC_MOD1_POP));
  finish_new_insn(gsip, insert_before, new_stmt, stmt);
}

static void
emit_compc (gimple_stmt_iterator *gsip, gcall *stmt, bool emit_before)
{
  const rvtt_insn_data *new_insnd =
    rvtt_get_insn_data(rvtt_insn_data::sfpcompc);
  gimple *new_stmt = gimple_build_call(new_insnd->decl (), 0);
  finish_new_insn(gsip, emit_before, new_stmt, stmt);
}

static tree
emit_loadi(gimple_stmt_iterator *gsip, gcall *stmt, int val, bool emit_before)
{
  const rvtt_insn_data *new_insnd =
    rvtt_get_insn_data(rvtt_insn_data::sfploadi);
  gimple *new_stmt = gimple_build_call(new_insnd->decl (), 5, null_pointer_node,
				       build_int_cst (unsigned_type_node, val),
				       integer_zero_node, integer_zero_node,
				       build_int_cst (unsigned_type_node, SFPLOADI_MOD0_SHORT));
  tree tmp = make_ssa_name (TREE_TYPE (TREE_TYPE (new_insnd->decl ())), new_stmt);
  gimple_call_set_lhs (new_stmt, tmp);

  finish_new_insn(gsip, emit_before, new_stmt, stmt);

  return tmp;
}

static tree
emit_loadi_lv(gimple_stmt_iterator *gsip, gcall *stmt, tree lhs, tree in, int val, bool emit_before)
{
  const rvtt_insn_data *new_insnd =
    rvtt_get_insn_data(rvtt_insn_data::sfploadi_lv);
  gimple *new_stmt = gimple_build_call (new_insnd->decl (), 6, null_pointer_node, in,
					build_int_cst (unsigned_type_node, val),
					integer_zero_node, integer_zero_node,
					build_int_cst (unsigned_type_node, SFPLOADI_MOD0_SHORT));
  if (lhs == NULL_TREE)
    lhs = make_ssa_name (TREE_TYPE (TREE_TYPE (new_insnd->decl ())), new_stmt);
  gimple_call_set_lhs (new_stmt, lhs);

  finish_new_insn(gsip, emit_before, new_stmt, stmt);

  return lhs;
}

static void
emit_setcc (gimple_stmt_iterator *gsip, gcall *stmt, tree in,
	    unsigned mod, unsigned type, bool emit_before)
{
  const rvtt_insn_data *new_insnd =
    rvtt_get_insn_data(rvtt_insn_data::sfpsetcc);
  gimple *new_stmt = gimple_build_call (new_insnd->decl (), new_insnd->num_args ());
  gimple_call_set_arg (new_stmt, new_insnd->src_arg (), in);
  gimple_call_set_arg (new_stmt, new_insnd->mod_arg (),
		       build_int_cst (unsigned_type_node, mod));
  if (TARGET_XTT_TENSIX_QSR)
    gimple_call_set_arg (new_stmt, new_insnd->mod_arg () + 1,
			 build_int_cst (unsigned_type_node, type));
  finish_new_insn(gsip, emit_before, new_stmt, stmt);
}

static unsigned
commute_cmp_args (unsigned op, rvtt_arg_info (&args)[2])
{
  std::swap (args[0], args[1]);
  if ((op & ~(SFPXCMP_MOD1_CC_EQ ^ SFPXCMP_MOD1_CC_NE))
      != SFPXCMP_MOD1_CC_EQ)
    op ^= SFPXCMP_MOD1_CC_LT ^ SFPXCMP_MOD1_CC_GT;
  return op;
}

/* Expand compare using subtract and/or setcc insns.  This is not going to be
   correct in all cases, for instance infinites will compare different, and
   there is no complete ordering of fp types.
   FIXME: integral ordering compares ignore overflow -- that's bug 14598. */

static bool
expand_cmp_using_sub (gimple_stmt_iterator *right, gcall *cmp, rvtt_arg_info (&args)[2], unsigned op, unsigned type)
{
  // Turn GT/GE to LT/LE to avoid extra insn,
  // Turn 0 EQ/NE A into A EQ/NE 0 to avoid subtract
  if (op >= SFPXCMP_MOD1_CC_GT
      || (op >= SFPXCMP_MOD1_CC_EQ && args[0].is_zero ()))
    op = commute_cmp_args (op, args);

  int setcc_op = op;
  if (!args[1].is_zero ())
    {
      const rvtt_insn_data *sub_insnd = nullptr;
      unsigned sub_mod = 0;
      tree neg1 = nullptr;

      if (type == SFPXCMP_MOD1_TYPE_FLOAT && op < SFPXCMP_MOD1_CC_EQ)
	{
	  auto *lreg_insnd = rvtt_get_insn_data(rvtt_insn_data::sfpreadlreg);
	  gcall *lreg_call = gimple_build_call (lreg_insnd->decl (), lreg_insnd->num_args ());
	  auto reg = build_int_cst (unsigned_type_node,
				    TARGET_XTT_TENSIX_WH ? CREG_IDX_NEG_1 : CREG_IDX_1);
	  gimple_call_set_arg (lreg_call, 0, reg);
	  neg1 = make_ssa_name (TREE_TYPE (args[0].get_arg ()));
	  gimple_call_set_lhs (lreg_call, neg1);
	  gimple_set_location (lreg_call, gimple_location (cmp));
	  gsi_insert_after (right, lreg_call, GSI_NEW_STMT);

	  sub_insnd = rvtt_get_insn_data (rvtt_insn_data::sfpmad);
	  sub_mod = TARGET_XTT_TENSIX_WH ? 0 : SFPMAD_MOD1_BH_COMPL_A;
	}
      else
	{
	  static const unsigned char iadd_map[] = {
	    SFPIADD_MOD1_CC_LT0,
	    SFPIADD_MOD1_CC_GTE0,
	    SFPIADD_MOD1_CC_NONE,
	    SFPIADD_MOD1_CC_NONE,
	    0xff,
	    0xff,
	  };

	  sub_mod = iadd_map[op];
	  if (sub_mod != SFPIADD_MOD1_CC_NONE)
	    setcc_op = -1;
	  sub_mod |= SFPIADD_MOD1_ARG_2SCOMP_LREG_DST;
	  sub_insnd = rvtt_get_insn_data (rvtt_insn_data::sfpiadd_v);
	}

      auto *sub_call = gimple_build_call (sub_insnd->decl (), sub_insnd->num_args ());
      if (neg1)
	gimple_call_set_arg (sub_call, sub_insnd->src_arg (), neg1);
      gimple_call_set_arg (sub_call, sub_insnd->src_arg (bool (neg1)), args[1].get_arg ());
      gimple_call_set_arg (sub_call, sub_insnd->src_arg (bool (neg1) + 1), args[0].get_arg ());
      gimple_call_set_arg (sub_call, sub_insnd->mod_arg (),
			   build_int_cst (unsigned_type_node, sub_mod));
      if (setcc_op >= 0)
	{
	  auto tmp = make_ssa_name (TREE_TYPE (args[0].get_arg ()));
	  args[0].set_arg (tmp);
	  gimple_set_lhs (sub_call, tmp);
	}
      gimple_set_location (sub_call, gimple_location (cmp));
      gsi_insert_after (right, sub_call, GSI_NEW_STMT);
    }

  if (setcc_op >= 0)
    {
      static const unsigned char setcc_map[] = {
	SFPSETCC_MOD1_LREG_LT0,
	SFPSETCC_MOD1_LREG_GTE0,
	SFPSETCC_MOD1_LREG_EQ0,
	SFPSETCC_MOD1_LREG_NE0,
	0xff,
	0xff,
      };
      // We always want to use INT type as we're jus concerned about the sign bit
      emit_setcc (right, cmp, args[0].get_arg (), setcc_map[setcc_op],
		  SFPSETCC_IMM_TYPE_INT, false);
    }

  return false;
}

/* Expand compare using gt, le insns. For equality compares emit using sub. */

static bool
expand_cmp_using_gtle (gimple_stmt_iterator *right, gcall *cmp, rvtt_arg_info (&args)[2], unsigned op, unsigned type)
{
  if (op == SFPXCMP_MOD1_CC_EQ || op == SFPXCMP_MOD1_CC_NE)
    // Add a combine pattern to turn an sfpiadd_v/setcc into a pari of sfple's
    return expand_cmp_using_sub (right, cmp, args, op, type);

  if (op < SFPXCMP_MOD1_CC_GT)
    op = commute_cmp_args (op, args);

  // Quasar and later use the int field to specify data type
  // Blackhole treats gtle as smag/float
  static const uint16_t gtle_type_map[] = {
    0xffff, //SFPGTLE_IMM_TYPE_UINT, for 4.1
    SFPGTLE_IMM_TYPE_INT,
    SFPGTLE_IMM_TYPE_SMAG,
    SFPGTLE_IMM_TYPE_FLOAT,
  };

  gcc_checking_assert (op == SFPXCMP_MOD1_CC_GT
		       || op == SFPXCMP_MOD1_CC_LE);

  auto *insnd = rvtt_get_insn_data (op == SFPXCMP_MOD1_CC_GT
				    ? rvtt_insn_data::sfpgt : rvtt_insn_data::sfple);
  auto mod_arg = build_int_cst (unsigned_type_node, SFPGTLE_MOD1_SET_CC);
  auto *call = gimple_build_call (insnd->decl (), insnd->num_args ());
  gimple_call_set_arg (call, insnd->src_arg (0), args[0].get_arg ());
  gimple_call_set_arg (call, insnd->src_arg (1), args[1].get_arg ());
  gimple_call_set_arg (call, insnd->mod_arg (), mod_arg);
  if (TARGET_XTT_TENSIX_QSR)
    {
      auto type_arg = build_int_cst (unsigned_type_node, gtle_type_map[type]);
      gimple_call_set_arg (call, insnd->mod_arg () + 1, type_arg);
    }
  gimple_set_location (call, gimple_location (cmp));
  gsi_insert_after (right, call, GSI_NEW_STMT);
  return false;
}

static bool
verify_cond_call (call_vec_t &conds, unsigned &ix, gcall *call)
{
  if (!conds[0])
    return true; // Already errored

  auto expected = ix < conds.size () ? conds[ix++] : nullptr;
  if (expected == call)
    return false; // OK

  conds[0] = nullptr;
  error_at (gimple_location (call),
	    "unexpected builtin %qD used within predication region",
	    gimple_call_fndecl (call));
  return true;
}

static gcall *
verify_cond_var (tree var, gcall *call)
{
  if (SSA_VAR_P (var))
    if (gcall *call = dyn_cast <gcall *> (SSA_NAME_DEF_STMT (var)))
      return call;

  error_at (gimple_location (call),
	    "operand of %qD call needs to be a variable",
	    gimple_call_fndecl (call));
  return nullptr;
}

static bool
expand_cmp (gimple_stmt_iterator *left, gimple_stmt_iterator *right,
	    gcall *cmp, const rvtt_insn_data *insnd, expand_flags flags)
{
  *left = *right = gsi_for_stmt (cmp);

  unsigned mod = TREE_INT_CST_LOW (gimple_call_arg (cmp, insnd->mod_arg ()));
  unsigned type = (mod >> SFPXCMP_MOD1_TYPE_SHIFT) & SFPXCMP_MOD1_TYPE_MASK;
  unsigned op = mod & SFPXCMP_MOD1_CC_MASK;

  if (flags & EF_negate)
    op ^= SFPXCMP_MOD1_CC_EQ ^ SFPXCMP_MOD1_CC_NE;

  rvtt_arg_info args[2] =
    {{gimple_call_arg (cmp, insnd->src_arg (0)), true},
     {gimple_call_arg (cmp, insnd->src_arg (1)), true}};

  /*
    We have SFPGT & SFPLE, which work on smag, 2's complement and unsigned (arch-depending)
  LT  b > a
  GE  b <= a
  EQ  b <= a && a >= b (avoid clobbering a tmp)
  NE  (a - b) as int is non zero
  GT  a > b
  LE  a <= b

  if one side is zero:
  LT0  0>a
  GE0  0<=a
  EQ0  a==0 or for signed_zeros as for above
  NE0  a!=0 or for signed_zeros as for above
  GT0  a>0
  LE0  a<=0

  If we do not have SFPGT & SFPLE, then we need to use a subtract.

  For float we do:
  LT  a - b is neg
  GE  a - b is non neg
  EQ  a - b is zero
  NE  a - b is non zero
  GT  b - a neg
  LE  b - a non neg

  These ignore signed_zero
  LT0  a is neg
  GE0  a is non neg
  EQ0  a is zero
  NE0  a is non zero
  GT0  a is non neg and a is non zero
  LE0  not (a is non neg and a is non zero)

  For int and uint we do: (this ignores the overflow problem)
  LT  a - b is neg
  GE  a - b is non neg
  EQ  a - b is zero
  NE  a - b is non zero
  GT  b - a neg
  LE  b - a non neg

  LT0  a is neg (false for uint)
  GE0  a is non neg (true for uint)
  EQ0  a is zero
  NE0  a is non zero
  GT0  a is non neg and a is non zero (just is non zero for uint)
  LE0  not (a is non neg and a is non zero) (just is zero for uint)

  FIXME: The following about ordered compares of ints is not yet implemented.

  For int and uint correctly we must have both values within 2^31 of eachother,
  do this by checking if the sign bits match or not.  This would be worth doing
  constant folding using the sign of the constant to handle the result in the
  different-signs case.

  0-0  a - b, neg means a < b
  1-1  a - b, neg means a < b
  1-0  int a is < b, uint a is > b
  0-1  int a is > b, uint a is < b

  t = a ^ b, is neg or non-neg
  t = a - b, is neg or non-neg

  txor = a ^ b, ta_b = a - b

  Int:
  LT (txor is non-neg AND ta_b is neg) OR (txor is neg AND a is neg)
  GE (txor is non-neg AND ta_b is non-neg) OR (txor is neg AND a is non-neg)
  GT handle as b LT a
  LE handle as b GE a

  UInt:
  LT (txor is non-neg AND ta_b is neg) OR (txor is neg AND a is non-neg)
  GE (txor is non-neg AND ta_b is non-neg) OR (txor is neg AND a is neg)
  GT handle as b LT a
  LE handle as b GE a

  We can apply De Morgan's here quite simply, which we do to avoid negations.

  Type   WH    BH   QSR   TRI
  UInt  isub  isub  isub  >,<=
  Int   isub  isub  >,<=  >,<=
  SMag   -    >,<=  >,<=  >,<=
  Float fsub  >,<=  >,<=  >,<=
  */

  // uint<int<smag<float
  bool negated = false;
  if ((TARGET_XTT_TENSIX_BH_QSR && type >= SFPXCMP_MOD1_TYPE_SMAG)
      || (TARGET_XTT_TENSIX_QSR && type == SFPXCMP_MOD1_TYPE_INT
	  && ((op | (SFPXCMP_MOD1_CC_NE ^ SFPXCMP_MOD1_CC_EQ)) == SFPXCMP_MOD1_CC_NE
	      || !(flags & EF_nearby))))
    negated = expand_cmp_using_gtle (right, cmp, args, op, type);
  else if (type != SFPXCMP_MOD1_TYPE_SMAG
	   || ((op | (SFPXCMP_MOD1_CC_NE ^ SFPXCMP_MOD1_CC_EQ)) == SFPXCMP_MOD1_CC_NE))
    negated = expand_cmp_using_sub (right, cmp, args, op, type);
  else
    {
      // Users just love peeking under the covers.
      static const char *const types[] = {"vUInt", "vInt", "vSMag", "vFloat"};
      static const char *const ops[] = {"<", ">=", "==", "!=", ">", "<="};
      error_at (gimple_location (cmp), "comparison %<%s %s %s%> is not supported",
		types[type], ops[op], types[type]);
    }

  return negated;
}

// Handle AND, OR, NOT & NEARBY logical operations
//
// Recursively processes a tree of boolean expressions.	 ORs are converted to
// ANDs by negating the children of the current node.  The negation is toggled
// as the tree is traversed to avoid accumulating redundant negations.
//
// Descending the LHS uses the last PUSHC as the "fence" against which a COMPC
// can be issued, however, descending the RHS would mess up the results from
// the LHS w/o a new fence, hence the PUSHC prior to the RHS.  The POPC would
// destroy the results of the RHS and so those results are saved/restored with
// saved_enables.

static bool
expand_logical (call_vec_t &conds, unsigned &ix,
		gimple_stmt_iterator *leftmost, gimple_stmt_iterator *rightmost,
		gcall *call, const rvtt_insn_data *call_insnd,  expand_flags flags)
{
  unsigned op = TREE_INT_CST_LOW (gimple_call_arg (call, call_insnd->mod_arg ()));
  tree lhs = gimple_call_arg (call, call_insnd->mod_arg () + 1);
  
  if (op == SFPXLOGIC_MOD1_NOT)
    {
      flags ^= EF_negate;
      return expand_cond (conds, ix, leftmost, rightmost, lhs, call, flags);
    }
  if (op == SFPXLOGIC_MOD1_NEARBY)
    {
      flags |= EF_nearby;
      return expand_cond (conds, ix, leftmost, rightmost, lhs, call, flags);
    }

  bool negated = op == (flags & EF_negate ? SFPXLOGIC_MOD1_AND : SFPXLOGIC_MOD1_OR);
  if (negated)
    flags ^= EF_negate;

  // Emit LEFT
  gimple_stmt_iterator lhs_rightmost;
  bool left_negated = expand_cond (conds, ix, leftmost, &lhs_rightmost, lhs, call, flags);

  // Emit RIGHT
  gimple_stmt_iterator rhs_leftmost;
  tree rhs = gimple_call_arg (call, call_insnd->mod_arg () + 2);
  bool right_negated = expand_cond (conds, ix, &rhs_leftmost, rightmost, rhs, call, flags);

  if (right_negated)
    {
      emit_pushc (&rhs_leftmost, call, true);
      tree saved_enables = emit_loadi (&rhs_leftmost, call, 1, true);

      saved_enables = emit_loadi_lv (rightmost, call, NULL_TREE, saved_enables, 0, false);
      emit_popc (rightmost, call, false);
      emit_setcc (rightmost, call, saved_enables, SFPSETCC_MOD1_LREG_EQ0, SFPSETCC_IMM_TYPE_INT, false);
    }

  if (negated)
    emit_compc (rightmost, call, false);

  if (left_negated)
    // Parent needs a fence for this node's left and side (if the parent
    // isn't the root)
    negated = true;

  return negated;
}

static bool
expand_cond (call_vec_t &conds, unsigned &ix,
	     gimple_stmt_iterator *leftmost, gimple_stmt_iterator *rightmost,
	     tree var, gcall *sink, expand_flags flags)
{
  bool negated = false;

  gcall *call = verify_cond_var (var, sink);
  const rvtt_insn_data *insnd = call ? rvtt_get_insn_data (call) : nullptr;
  if (insnd)
    switch (insnd->id)
      {
      default:
	break;

      case rvtt_insn_data::sfpxcmp:
	if (expand_cmp (leftmost, rightmost, call, insnd, flags))
	  {
	    emit_compc (rightmost, call, false);
	    negated = true;
	  }
	break;

      case rvtt_insn_data::sfpxlogic:
	negated = expand_logical (conds, ix, leftmost, rightmost,
				  call, insnd, flags);
	break;
      }

  verify_cond_call (conds, ix, call);

  unlink_stmt_vdef (call);
  gimple_stmt_iterator gsi = gsi_for_stmt (call);
  gsi_remove (&gsi, true);

  return negated;
}

static bool
expand_conditionals (pred_node::builder &builder, call_vec_t &conds, basic_block bb)
{
  bool changed = false;
  pred_node *node = nullptr;

  for (auto gsi = gsi_start_bb (bb); !gsi_end_p (gsi); gsi_next (&gsi))
    {
      auto *insnd = rvtt_get_insn_data (*gsi);
      if (!insnd)
	continue;

      auto *call = as_a <gcall *> (*gsi);
      switch (insnd->id)
	{
	default:
	  if (!conds.empty () && insnd->sets_cc (call))
	    error_at (gimple_location (call),
		      "disallowed cc-setting builtin %qD within predication region",
		      gimple_call_fndecl (call));
	  break;

	case rvtt_insn_data::sfpxpred:
	  {
	    if (dump_file)
	      {
		fprintf (dump_file, "Recording ");
		print_gimple_stmt (dump_file, call, 0);
	       }

	    if (!conds.empty ())
	      {
		conds.clear ();
		error_at (gimple_location (call),
			  "Disallowed nested predication region");
	      }

	    int xmod = TREE_INT_CST_LOW (gimple_call_arg (call, insnd->mod_arg ()));
	    node = new (ggc_alloc<pred_node> ()) pred_node (call, xmod);

	    if (xmod & SFPXPRED_MOD1_IF)
	      conds.push_back (call);
	    else
	      builder.record (node, call);
	  }
	  break;

	case rvtt_insn_data::sfpxlogic:
	case rvtt_insn_data::sfpxcmp:
	case rvtt_insn_data::sfpxcond:
	  if (conds.empty ())
	    error_at (gimple_location (call),
		      "predication builtin %qD outside of predication region",
		      gimple_call_fndecl (call));
	  conds.push_back (call);
	  if (insnd->id != rvtt_insn_data::sfpxcond)
	    break;

	  node->cond = call;
	  builder.record (node, call, true);
	  // FIXME:Verify ssa dep?

	  if (dump_file)
	    {
	      fprintf (dump_file, "Expanding ");
	      print_gimple_stmt (dump_file, call, 0);
	    }

	  tree dep = gimple_call_arg (call, insnd->mod_arg () + 1);
	  gcall *first = verify_cond_var (dep, call);
	  if (!first)
	    break;

	  unsigned ix = 0;
	  verify_cond_call (conds, ix, first);

	  tree cond_var = gimple_call_arg (call, insnd->mod_arg () + 2);
	  gimple_stmt_iterator leftmost, rightmost;
	  expand_cond (conds, ix, &leftmost, &rightmost, cond_var, call, EF_none);

	  verify_cond_call (conds, ix, call);

	  gimple_call_set_arg (call, insnd->mod_arg () + 2, integer_zero_node);
	  update_stmt (call);
	  conds.clear ();
	  changed = true;
	  break;
	}
    }

  if (!conds.empty ())
    {
      error_at (gimple_location (conds.front ()),
		"untermated predication region");
      conds.clear ();
    }

  return changed;
}

// The hardware does not support OR and generates some comparisons (LTE, GE)
// by ANDing others together and issuing a compc.  This requires refactoring
// boolean expressions using De Moragan's laws.	 The root of a tree is anchored
// by an sfpxcondb.  All dependent operations are chained to this by their
// return values.  This pass traverses the tree, more or less deletes it and
// replaces it with one that works w/ the HW.

static unsigned
expand_vif (function *fn)
{
  pred_node::builder builder;
  call_vec_t cond_vec;
  bool changed = false;

  // Walk the blocks in something like graph order.  With the exception of
  // loop back edges every block is walked after its predecessors.
  std::deque<std::pair<basic_block, pred_node *>> worklist;

  basic_block bb;
  FOR_ALL_BB_FN (bb, fn)
    bb->flags &= ~BB_VISITED;

  basic_block entry = ENTRY_BLOCK_PTR_FOR_FN (fn);
  entry->flags |= BB_VISITED;
  worklist.emplace_back (entry, nullptr);

  while (!worklist.empty ())
    {
      auto [bb, node] = worklist.front ();
      worklist.pop_front ();
      builder.set_node (node);
      if (expand_conditionals (builder, cond_vec, bb))
	changed = true;

      edge e;
      edge_iterator ei;
      FOR_EACH_EDGE (e, ei, bb->succs)
	{
	  auto s = e->dest;
	  if (!(s->flags & BB_VISITED))
	    {
	      s->flags |= BB_VISITED;
	      worklist.push_back ({s, builder.get_node ()});
	    }
	}
    }

  v_if_ok = !(false
	      || builder.assemble ()
	      || builder.check_sequences ()
	      || builder.sequence ()
	      || builder.check_control_flows (fn));

  if (auto *root = builder.get_node ())
    if (dump_file)
      {
	fprintf (dump_file, "\nv_if graph:\n");
	root->print (dump_file);
      }

  return changed ? TODO_update_ssa : 0;
}

bool rvtt_preds_valid () {
  return v_if_ok;
}

namespace {

const pass_data pass_data_rvtt_vif =
{
  GIMPLE_PASS, /* type */
  "rvtt_vif", /* name */
  OPTGROUP_NONE, /* optinfo_flags */
  TV_NONE, /* tv_id */
  PROP_ssa, /* properties_required */
  0, /* properties_provided */
  0, /* properties_destroyed */
  0, /* todo_flags_start */
  0, /* todo_flags_finish */
};

class pass_rvtt_vif : public gimple_opt_pass
{
public:
  pass_rvtt_vif (gcc::context *ctxt)
    : gimple_opt_pass (pass_data_rvtt_vif, ctxt)
  {}

  virtual bool gate (function *) override
  {
    return TARGET_XTT_TENSIX;
  }

  virtual unsigned int execute (function *fn) override
  {
    return expand_vif (fn);
  }
}; // class pass_rvtt_vif

} // anon namespace

gimple_opt_pass *
make_pass_rvtt_vif (gcc::context *ctxt)
{
  return new pass_rvtt_vif (ctxt);
}
