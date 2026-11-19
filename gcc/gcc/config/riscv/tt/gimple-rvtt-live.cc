/* Pass to handle SFPU CC "liveness"
   Copyright (C) 2022-2025 Tenstorrent Inc.
   Originated by Paul Keller (pkeller@tenstorrent.com).

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
#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "backend.h"
#include "target.h"
#include "rtl.h"
#include "tree.h"
#include "gimple.h"
#include "cfghooks.h"
#include "tree-pass.h"
#include "ssa.h"
#include "cgraph.h"
#include "gimple-pretty-print.h"
#include "diagnostic-core.h"
#include "fold-const.h"
#include "trans-mem.h"
#include "stor-layout.h"
#include "print-tree.h"
#include "cfganal.h"
#include "tree-eh.h"
#include "gimple-iterator.h"
#include "gimple-fold.h"
#include "gimplify-me.h"
#include "gimple-walk.h"
#include "tree-cfg.h"
#include "tree-ssa-loop-manip.h"
#include "tree-ssa-loop-niter.h"
#include "tree-into-ssa.h"
#include "tree-dfa.h"
#include "tree-ssa.h"
#include "except.h"
#include "cfgloop.h"
#include "tree-ssa-propagate.h"
#include "value-prof.h"
#include "tree-inline.h"
#include "tree-ssa-live.h"
#include "omp-general.h"
#include "omp-expand.h"
#include "tree-cfgcleanup.h"
#include "gimplify.h"
#include "attribs.h"
#include "selftest.h"
#include "opts.h"
#include "asan.h"
#include "profile.h"
#include <vector>
#include <unordered_map>
#include <tuple>
#include "rvtt.h"

#define DUMP(...) //fprintf(stderr, __VA_ARGS__)

using namespace std;

//////////////////////////////////////////////////////////////////////////////
// block_data: structure used when processing blocks to track:
//   BD_VisitedP: whether or not a block has been visited previously
//   BD_CCLevel: the CC level at entry to the block on the first visit
//   BD_Live: whether all variable uses in the block should be treated as live,
//         this occurs when the block is entered multiple times with different
//         nest levels
//   BD_StackDepth: the CC stack depth at entry, used for sanity checks
typedef vector<tuple<bool, unsigned int, bool, unsigned int>> block_data;

enum {
  BD_VisitedP = 0,
  BD_CCLevel = 1,
  BD_Live = 2,
  BD_StackDepth = 3,
};

//////////////////////////////////////////////////////////////////////////////
// Track "liveness"
// Map statements to:
//   level: The depth of CC setting stmts that were hit prior to this
//          statement
//   generation: The CC "generation" of the statement.  A new generation
//               occurs whenever a CC setting statement is after the LV_CCLevel
//               has decreased (POPC) or for the first generation...
//   force: For the statement to be live (happens when a BB is hit mulitple
//          times w/ different levels
static const int liveness_undefined = 0xFFFF;
struct liveness_data {
  unsigned int level;
  unsigned int generation;
  bool force;

  liveness_data(unsigned int l, unsigned int g, bool f) : level(l), generation(g), force(f) {}
};

typedef unordered_map<gcall *, struct liveness_data> call_liveness;

static long int
get_int_arg(gcall *stmt, unsigned int arg)
{
  tree decl = gimple_call_arg(stmt, arg);
  if (decl)
    {
      gcc_assert(TREE_CODE(decl) == INTEGER_CST);
      return *(decl->int_cst.val);
    }
  return -1;
}

//////////////////////////////////////////////////////////////////////////////
static bool
process_block_stmts(basic_block bb,
		    liveness_data *current,
		    bool *cascading,
		    unsigned int *gen_count,
		    vector<liveness_data> &stack,
		    call_liveness &liveness)
{
  bool found_sfpu = false;

  DUMP("    process_block_stmts CC %d gen %d depth %zu\n",
	 current->level, current->generation, stack.size());

  for (auto gsi = gsi_start_bb (bb); !gsi_end_p (gsi); gsi_next (&gsi))
    {
      auto *insnd = rvtt_get_insn_data (*gsi);
      if (!insnd)
	continue;
      auto *stmt = as_a <gcall *> (*gsi);

      found_sfpu = true;
      if (insnd->id == rvtt_insn_data::sfppushc)
	{
	  bool is_replace = (get_int_arg(stmt, insnd->mod_arg ()) == SFPPUSHCC_MOD1_REPLACE);

	  if (is_replace)
	    stack.pop_back();

	  stack.push_back(*current);
	  if (!*cascading)
	    {
	      (*gen_count)++;
	      *cascading = true;
	    }
	  current->generation = *gen_count;
	  DUMP("      pushed to (l=%d, g=%d)\n", current->level, current->generation);
	}
      else if (insnd->id == rvtt_insn_data::sfppopc)
	{
	  if (stack.size() == 0)
	    {
	      location_t location = gimple_nonartificial_location(stmt);
	      error_at(location, "malformed program, popc without matching pushc");
	    }
	  *current = stack.back();
	  DUMP("      popped to (l=%d, g=%d)\n", current->level, current->generation);
	  *cascading = false;
	  stack.pop_back();
	}
      else
	{
	  gcc_assert(liveness.find(stmt) == liveness.end());
	  liveness.insert(pair<gcall *, liveness_data>(stmt, *current));
	  if (insnd->sets_cc(stmt) &&
	      insnd->id != rvtt_insn_data::sfpencc)
	    {
	      if (stack.size() == 0)
		{
		  location_t location = gimple_nonartificial_location(stmt);
		  error_at(location, "malformed program, %s outside of pushc/popc", insnd->name);
		}
	      current->level++;
	    }
	}
    }

  return found_sfpu;
}

// Process a block
static bool
process_block(function *fn,
	      basic_block bb,
	      block_data& bd,
	      liveness_data current,
	      bool cascading,
	      unsigned int gen_count,
	      vector<liveness_data> stack,
	      call_liveness &liveness)
{
  edge_iterator ei;
  edge e;

  DUMP("    process_block bb %d CC %d gen %d depth %zu\n",
       bb->index, current.level, current.generation, stack.size());

  if (get<BD_VisitedP>(bd[bb->index]))
    {
      gcc_assert(stack.size() == get<BD_StackDepth>(bd[bb->index]));

      // If we hit a BB and the CC level isn't the same as another hit, then
      // everything must be live
      if (!get<BD_Live>(bd[bb->index]) &&
	  current.level > get<BD_CCLevel>(bd[bb->index]))
	{
	  DUMP("     re-visit block w/ higher CC, marking all live\n");

	  // This bb has been visited and the current CC level is greater than
	  // previous, so mark all assignments within for variables defined
	  // without as live
	  get<BD_Live>(bd[bb->index]) = true;

	  for (gimple_stmt_iterator gsi = gsi_start_bb (bb);
	       !gsi_end_p (gsi); gsi_next (&gsi))
	    if (rvtt_get_insn_data (*gsi))
	      {
		auto loc = liveness.find(as_a <gcall *> (*gsi));
		if (loc != liveness.end())
		  loc->second.force = true;
	      }
	}

      return false;
    }

  get<BD_VisitedP>(bd[bb->index]) = true;
  get<BD_CCLevel>(bd[bb->index]) = current.level;
  get<BD_StackDepth>(bd[bb->index]) = stack.size();

  bool found_sfpu = process_block_stmts(bb, &current, &cascading, &gen_count, stack, liveness);

  // When we leave, EDGE_COUNT == 0, stack must be empty
  if (EDGE_COUNT(bb->succs) == 0 && stack.size() != 0)
    {
      error_at(EXPR_LOCATION(fn->decl), "malformed program, pushc without matching popc");
    }

  FOR_EACH_EDGE(e, ei, bb->succs)
    {
      found_sfpu = process_block(fn, e->dest, bd, current, cascading, gen_count, stack, liveness) || found_sfpu;
    }

  return found_sfpu;
}

// Pointer chase back to the root if needed
//
// PHI nodes occur where multiple BB come together
// We are interested in the single defining stmt of an SSA var and what the
// liveness is at that time
// We don't care about all the paths, just one path back to the definition
// However, if we hit a loop, we need to move on to the next path
static liveness_data
get_def_stmt_liveness_1(function *fn, vector<bool> &visited, liveness_data data,
			gimple *def_g, const call_liveness& liveness)
{
  if (def_g == nullptr)
    {
      DUMP("      null gimple, unassigned var\n");
      data.level = liveness_undefined;
      return data;
    }

  if (def_g->code == GIMPLE_PHI)
    {
      DUMP("      process phi\n");

      int n = gimple_phi_num_args (def_g);

      /* If we see non zero constant, we should punt.  The predicate
       * should be one guarding the phi edge.  */
      for (int i = 0; i < n; ++i)
	{
	  tree op = gimple_phi_arg_def (def_g, i);
	  edge e = gimple_phi_arg_edge (dyn_cast<gphi *>(def_g), i);
	  basic_block bb = e->src;
	  if (!visited[bb->index])
	    {
	      visited[bb->index] = true;
	      get_def_stmt_liveness_1(fn, visited, data, SSA_NAME_DEF_STMT(op), liveness);
	    }
	}
    }
  else
    {
      gcall *stmt = dyn_cast<gcall *>(def_g);

      if (stmt == nullptr)
	{
	  DUMP("      null statement, unassigned var\n");
	  data.level = liveness_undefined;
	}
      else
	{
	  const rvtt_insn_data * insnd = rvtt_get_insn_data(stmt);
	  if (insnd)
	    {
	      if (insnd->is_live ())
		{
		  DUMP("      chase assignment\n");
		  // If the defining statement is another _lv insn, chase it through
		  data = get_def_stmt_liveness_1(fn, visited, data,
						 SSA_NAME_DEF_STMT(gimple_call_arg(stmt, insnd->live_arg ())), liveness);
		}
	      else
		{
		  // The chain ends at a non-live insn, use its liveness as the result
		  if (liveness.find(stmt) == liveness.end())
		    {
		      // This is a malformed program
		      DUMP("      confused, malformed program?\n");
		      gcc_assert(0);
		    }
		  else
		    {
		      DUMP("      found base non-live assignment\n");
		      data = liveness.find(stmt)->second;
		    }
		}
	    }
	  else
	    {
	      DUMP("      confused, chased to a non-sfpu instruction\n");
	      gcc_assert(0);
	    }
	}
    }

  return data;
}

// Find the single root of the assignment chain, across BBs
static liveness_data
get_def_stmt_liveness(function *fn, gcall *stmt, const rvtt_insn_data *insnd, const call_liveness& liveness)
{
  liveness_data data(0, 0, false);
  vector<bool> visited;

  visited.resize(n_basic_blocks_for_fn(fn));

  visited[gimple_bb(stmt)->index] = true;

  gimple *def_g = SSA_NAME_DEF_STMT (gimple_call_arg(stmt, insnd->live_arg ()));

  return get_def_stmt_liveness_1(fn, visited, data, def_g, liveness);
}

static void
copy_args_to_live_insn (gimple *new_stmt, unsigned int live_arg, gcall *stmt)
{
  // Copy initial args
  for (unsigned int i = 0; i < live_arg; i++)
    {
      gimple_call_set_arg (new_stmt, i, gimple_call_arg (stmt, i));
    }

  // Shift remaining arguments by 1
  for (unsigned int i = live_arg; i < gimple_call_num_args (stmt); i++)
    {
      gimple_call_set_arg (new_stmt, i + 1, gimple_call_arg (stmt, i));
    }
}

// break_liveness
// Processes the liveness data to find instructions that do not need to be
// live and breaks the liveness connection by either deleting the sfpassign_lv
// (livness assignment) or changing the instruction to the non-live version
static void
break_liveness(function *fn, call_liveness& liveness)
{
  DUMP("  Break liveness\n");
  call_liveness::iterator it;

  // Iterate over all the sfpu instructions
  for (it = liveness.begin(); it != liveness.end(); it++)
    {
      gcall *stmt = it->first;
      const rvtt_insn_data *insnd = rvtt_get_insn_data(stmt);

      if (insnd && insnd->is_live ())
	{
	  // Get the defining statement and it's cc_count for this SSA
	  liveness_data cur_stmt_liveness = it->second;
	  liveness_data def_stmt_liveness = get_def_stmt_liveness(fn, stmt, insnd, liveness);

	  DUMP("    liveness: cur %d def %d curgen %d defgen %d force %d\n",
	       cur_stmt_liveness.level, def_stmt_liveness.level,
	       cur_stmt_liveness.generation, def_stmt_liveness.generation,
	       cur_stmt_liveness.force);

	  if (def_stmt_liveness.level == liveness_undefined)
	    {
	      continue;
	    }

	  if ((cur_stmt_liveness.level <= def_stmt_liveness.level &&
	       cur_stmt_liveness.generation <= def_stmt_liveness.generation) ||
	      cur_stmt_liveness.force)
	    {
	      if (insnd->id == rvtt_insn_data::sfpassign_lv)
		{
		  DUMP("    removing sfpassign\n");
		  tree lhs = gimple_call_lhs (stmt);
		  if (lhs != nullptr)
		    {
		      // For assignments, find all occurences of the LHS,
		      // replace w/ the live SSA

		      tree live_ssa = gimple_call_arg (stmt, 1);
		      gimple * use_stmt;
		      imm_use_iterator iter;
		      FOR_EACH_IMM_USE_STMT (use_stmt, iter, lhs)
			{
			  if (is_gimple_debug (use_stmt))
			    continue;
			  if (use_stmt != stmt)
			    {
			      if (use_stmt->code == GIMPLE_PHI)
				{
				  for (unsigned int i = 0; i < gimple_phi_num_args (use_stmt); i++)
				    {
				      if (gimple_phi_arg_def (use_stmt, i) == lhs)
					{
					  SET_PHI_ARG_DEF(as_a <gphi *> (use_stmt), i, live_ssa);
					  update_stmt (use_stmt);
					  break;
					}
				    }
				}
			      else
				{
				  for (unsigned int i = 0; i < gimple_call_num_args (use_stmt); i++)
				    {
				      if (gimple_call_arg (use_stmt, i) == lhs)
					{
					  gimple_call_set_arg(use_stmt, i, live_ssa);
					  update_stmt (use_stmt);
					}
				    }
				}
			    }
			}
		    }

		  // Remove assign
		  gimple_stmt_iterator gsi = gsi_for_stmt(stmt);

		  // Update SSA for the 2 arguments of the assign
		  gcc_assert(gimple_call_num_args(stmt) == 2);

		  rvtt_prep_stmt_for_deletion(stmt);
		  unlink_stmt_vdef(stmt);
		  gsi_remove(&gsi, true);
		  release_defs(stmt);
		}
	    }
	}
    }
}

// fold_live_assign
// Removes sfpassign_lv instructions by folding:
//    ssa1 = __builtin_riscv_sfp<foo>(...)
//    ssa2 = __builtin_riscv_sfpassign_lv(ssa_live, ssa1)
// into:
//    ssa2 = __builtin_riscv_sfp<foo>_lv(..., ssa_live, ...)
//
// Note: the statement "liveness" database is no longer used and does not need
// to be maintained by the changes below
static void
fold_live_assign (function *fn)
{
  basic_block bb;
  gimple_stmt_iterator gsi;

  DUMP("  Fold live\n");

  FOR_EACH_BB_FN (bb, fn) {
    gsi = gsi_start_bb (bb);
    while (!gsi_end_p (gsi))
      {
	auto *insnd = rvtt_get_insn_data (*gsi);
	if (insnd && insnd->id == rvtt_insn_data::sfpassign_lv)
	  {
	    auto *stmt = as_a <gcall *> (*gsi);
	    tree lhs = gimple_call_lhs (stmt);
	    tree live_arg = gimple_call_arg (stmt, 0);
	    tree assgn_arg = gimple_call_arg (stmt, 1);

	    gimple_stmt_iterator prev_gsi = gsi;
	    gsi_prev_nondebug (&prev_gsi);
	    auto *prev_insnd = !gsi_end_p (prev_gsi)
	      ? rvtt_get_insn_data (*prev_gsi) : nullptr;

	    if (prev_insnd && SSA_NAME_DEF_STMT (assgn_arg) == *prev_gsi)
	      {
		DUMP("    folding %s\n", prev_insnd->name);

		auto *prev_stmt = as_a <gcall *> (*prev_gsi);
		gcall *prev2_stmt = nullptr;
		if (prev_insnd->is_live ())
		  {
		    // The only _lv insns at this point are from the non-imm
		    // path injecting sfploadi_lv for a 32 bit load
		    // Back up one insn and propogate liveness there
		    DUMP("    handling nonimm %s\n", prev_insnd->name);
		    gcc_assert(prev_insnd->id == rvtt_insn_data::sfploadi_lv);
		    prev2_stmt = prev_stmt;
		    gsi_prev_nondebug (&prev_gsi);
		    gcc_assert(rvtt_get_insn_data (*prev_gsi)->id == rvtt_insn_data::sfploadi);
		  }

		const rvtt_insn_data *new_insnd = prev_insnd->get_live ();
		if (new_insnd != nullptr)
		  {
		    // Create _lv version of prev, delete assign stmt, delete prev_stmt
		    DUMP("    building new %s\n", new_insnd->name);
		    gimple *new_stmt = gimple_build_call (new_insnd->decl (),
							  gimple_call_num_args (prev_stmt) + 1);

		    copy_args_to_live_insn (new_stmt, new_insnd->live_arg (), prev_stmt);

		    // Copy the live arg
		    gimple_call_set_arg (new_stmt, new_insnd->live_arg (), live_arg);

		    gimple_set_location (new_stmt, gimple_location (prev_stmt));
		    gsi_insert_before(&prev_gsi, new_stmt, GSI_SAME_STMT);
		    update_stmt(new_stmt);

		    if (prev2_stmt == nullptr)
		      {
			gimple_call_set_lhs (new_stmt, lhs);
			rvtt_prep_stmt_for_deletion(prev_stmt);
			unlink_stmt_vdef(prev_stmt);
			gsi_remove(&prev_gsi, true);
			release_defs(prev_stmt);
		      }
		    else
		      {
			DUMP("    updating both lhs'\n");
			gimple_call_set_lhs (new_stmt, gimple_call_lhs(prev_stmt));
			gimple_call_set_lhs (prev2_stmt, lhs);
			update_stmt(prev2_stmt);
			rvtt_prep_stmt_for_deletion(prev_stmt);
			gsi_remove(&prev_gsi, true);
		      }

		    // Remove candidate
		    gsi_remove(&gsi, true);

		    update_ssa (TODO_update_ssa);
		    continue;
		  }
	      }
	  }

	gsi_next (&gsi);
      }
  }
}

// Liveness BB analysis
// This pass has to occur before SSA form is generated since that breaks the
// association between instructions that should remain live.  However, at this
// point in the pipeline, the following still exist:
//  - lots of already inlined function bodies
//  - POPC loops are not unrolled
static void
transform (function *fn)
{
  block_data bd;
  call_liveness liveness;
  vector<liveness_data> stack;

  DUMP("Liveness pass on: %s\n", function_name(fn));

  if (lookup_attribute ("always_inline", DECL_ATTRIBUTES (fn->decl)) != NULL)
    {
      // Skip the wrapper code, only process instantiated functions
      return;
    }

  stack.reserve(16);
  bd.resize(n_basic_blocks_for_fn(fn));

  if (process_block(fn, ENTRY_BLOCK_PTR_FOR_FN(fn), bd, liveness_data(0, 0, false), true, 0, stack, liveness))
    {
      break_liveness(fn, liveness);
      fold_live_assign(fn);
    }
}

namespace {

const pass_data pass_data_rvtt_live =
{
  GIMPLE_PASS, /* type */
  "rvtt_live", /* name */
  OPTGROUP_NONE, /* optinfo_flags */
  TV_NONE, /* tv_id */
  PROP_ssa, /* properties_required */
  0, /* properties_provided */
  0, /* properties_destroyed */
  0, /* todo_flags_start */
  0, /* todo_flags_finish */
};

class pass_rvtt_live : public gimple_opt_pass
{
public:
  pass_rvtt_live (gcc::context *ctxt)
    : gimple_opt_pass (pass_data_rvtt_live, ctxt)
  {}

  virtual bool gate (function *) override
  {
    return TARGET_XTT_TENSIX && rvtt_preds_valid ();
  }

  virtual unsigned execute (function *fn) override
  {
    transform (fn);
    return 0;
  }
}; // class pass_rvtt_live

} // anon namespace

gimple_opt_pass *
make_pass_rvtt_live (gcc::context *ctxt)
{
  return new pass_rvtt_live (ctxt);
}
