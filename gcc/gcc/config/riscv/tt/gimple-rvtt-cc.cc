/* Pass to optimize SFPU condition codes
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
#include <tuple>
#include "rvtt.h"

#define DUMP(...) //fprintf(stderr, __VA_ARGS__)

using namespace std;

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
// bool in tuple tracks whether or not a COMPC was seen at this stack depth
static void
process_block_stmts(basic_block bb,
		    vector<tuple<bool, bool, gimple_stmt_iterator>> &stack)
{
  constexpr int tuple_prior_removable = 0;
  constexpr int tuple_prior_replace = 1;
  constexpr int tuple_gsi = 2;
  gimple_stmt_iterator gsi, prior_pushc, prior_popc;
  bool prior_removable = false;
  bool prior_is_replace = false;

  // Find all function calls
  gsi = gsi_start_bb (bb);
  while (!gsi_end_p (gsi))
    {
      if (auto *insnd = rvtt_get_insn_data (*gsi))
	{
	  auto *stmt = as_a <gcall *> (*gsi);
	  switch (insnd->id)
	    {
	    case rvtt_insn_data::sfppushc:
	      {
		bool is_replace = (get_int_arg(stmt, insnd->mod_arg ()) == SFPPUSHCC_MOD1_REPLACE);

		prior_removable = false;
		DUMP("PUSHC(%s): stack size %d\n", is_replace ? "replace" : "push", stack.size());

		if (stack.size() == 0)
		  {
		    if (is_replace) {
		      error("malformed program, pushc replace at outer level\n");
		    }

		    DUMP("  removing outermost pushc\n");

		    // Remove outermost pushc
		    gimple *g = gsi_stmt (gsi);
		    unlink_stmt_vdef(g);
		    gsi_remove(&gsi, true);
		    release_defs(g);

		    stack.push_back(make_tuple(false, false, gsi));
		    // Avoid the gsi_next at the end since we removed the inst
		    continue;
		  }
		else
		  {
		    if (is_replace)
		      {
			stack.pop_back();
		      }
		    prior_is_replace = is_replace;
		    stack.push_back(make_tuple(false, is_replace, gsi));
		  }
		break;
	      }

	    case rvtt_insn_data::sfpxcmp:
	      gcc_unreachable ();

	    case rvtt_insn_data::sfpxpred:
	    case rvtt_insn_data::sfpxcond:
	      break;

	    case rvtt_insn_data::sfpcompc:
	      // Set compc to true for current pushc
	      if (stack.size() == 0)
		error("malformed program, sfpcompc outside of pushc/popc - exiting!");

	      prior_removable = false;
	      stack.back() = make_tuple(true, get<tuple_prior_replace>(stack.back()), get<tuple_gsi>(stack.back()));
	      break;

	    case rvtt_insn_data::sfppopc:
	      {
		DUMP("POPC: stack size %d\n", stack.size());

		if (stack.size() == 0)
		  error("malformed program, popc without matching pushc - exiting!");

		// Only remove inner PUSHC/POPC if they fall within a bb
		// since different paths may differ in intervening instructions
		if (prior_removable &&
		    prior_pushc.bb == prior_popc.bb &&
		    prior_popc.bb == gsi.bb)
		  {

		    DUMP("  removing inner PUSHC\n");
		    gimple *g = gsi_stmt (prior_pushc);
		    unlink_stmt_vdef(g);
		    gsi_remove(&prior_pushc, true);
		    release_defs(g);

		    if (!prior_is_replace)
		      {
			DUMP("  removing inner POPC\n");
			gimple *g = gsi_stmt (prior_popc);
			unlink_stmt_vdef(g);
			gsi_remove(&prior_popc, true);
			release_defs(g);
		      }
		  }

		// Not removable if we saw a compc
		prior_removable = !get<tuple_prior_removable>(stack.back()); 
		prior_is_replace = get<tuple_prior_replace>(stack.back());
		prior_pushc = get<tuple_gsi>(stack.back());
		prior_popc = gsi;

		stack.pop_back();
		if (stack.size() == 0)
		  {
		    DUMP("  replacing outermost popc with encc\n");

		    // Replace outermost popc with encc
		    const rvtt_insn_data *new_insnd =
		      rvtt_get_insn_data(rvtt_insn_data::sfpencc);
		    gimple *new_stmt = gimple_build_call
		      (new_insnd->decl (), 2,
		       build_int_cst (unsigned_type_node, SFPENCC_MOD1_EI_RI),
		       build_int_cst (unsigned_type_node, SFPENCC_IMM12_BOTH));

		    gimple_set_vuse (new_stmt, gimple_vuse (stmt));
		    gimple_set_vdef (new_stmt, gimple_vdef (stmt));
		    gimple_set_location (new_stmt, gimple_location (stmt));
		    unlink_stmt_vdef (stmt);
		    gsi_remove (&gsi, true);
		    release_defs (stmt);
		    gsi_insert_before (&gsi, new_stmt, GSI_NEW_STMT);
		    prior_removable = false;
		  }
	      }
	      break;

	    default:
		DUMP("Intervening %s\n", insnd->name);
		// Could be smarter about the non-__builtin_riscv_sfp
		// calls, but bail if anything else comes in to be safe
		// "Other" instructions
		prior_removable = false;
		break;
	    }
	}
      else if (is_a<gcall *> (*gsi))
	{
	  DUMP("Intervening fn call\n");
	  prior_removable = false;
	}

      gsi_next (&gsi);
    }
}

static void
process_block(basic_block bb,
	      vector<bool>& bd,
	      vector<tuple<bool, bool, gimple_stmt_iterator>> stack)
{
  edge_iterator ei;
  edge e;

  // If we hit the same BB multiple times, the stack depth must always be the
  // same.  The liveness pass asserts this.  If this is ever found to not be
  // true, we'll have to bail on optimizing the CC for that BB.

  DUMP("Process block %d\n", bb->index);
  if (!bd[bb->index])
    {
      // Haven't visited this BB before
      process_block_stmts(bb, stack);
      bd[bb->index] = true;

      // When we leave, EDGE_COUNT == 0, stack must be empty
      gcc_assert(EDGE_COUNT(bb->succs) != 0 || stack.size() == 0);

      FOR_EACH_EDGE(e, ei, bb->succs)
	{
	  // When we leave, EDGE_COUNT == 0, stack must be empty
	  process_block(e->dest, bd, stack);
	}
    }
}

// This pass optimizes 2 things:
// 1) The outermost pushc/popc: remove pushc, turn popc into encc
// 2) Tail popc: if 2 popcs occur without intervening instructions and the
//    inner pushc/popc did not have a compc in between, the inner pushc/popc
//    can be removed.  Tricky because you have to track the prior popc.
//    The inner push/pop and the outer pop must all be in the same BB
static void transform (function *fn)
{
  vector<tuple<bool, bool, gimple_stmt_iterator>> stack;
  vector<bool> bd;

  if (lookup_attribute ("always_inline", DECL_ATTRIBUTES (fn->decl)) != NULL)
    {
      // Skip the wrapper code, only process instantiated functions
      return;
    }

  DUMP("CC pass on: %s\n", function_name(fn));

  stack.reserve(16);
  bd.resize(n_basic_blocks_for_fn(fn));

  process_block(ENTRY_BLOCK_PTR_FOR_FN(fn), bd, stack);

  update_ssa (TODO_update_ssa);
}

namespace {

const pass_data pass_data_rvtt_cc =
{
  GIMPLE_PASS, /* type */
  "rvtt_cc", /* name */
  OPTGROUP_NONE, /* optinfo_flags */
  TV_NONE, /* tv_id */
  PROP_ssa, /* properties_required */
  0, /* properties_provided */
  0, /* properties_destroyed */
  0, /* todo_flags_start */
  0, /* todo_flags_finish */
};

class pass_rvtt_cc : public gimple_opt_pass
{
public:
  pass_rvtt_cc (gcc::context *ctxt)
    : gimple_opt_pass (pass_data_rvtt_cc, ctxt)
  {}

  virtual bool gate (function *) override
  {
    return TARGET_XTT_TENSIX && riscv_tt_opt_cc > 0
      && rvtt_preds_valid ();
  }

  virtual unsigned execute (function *fn) override
  {
    transform (fn);
    return 0;
  }
}; // class pass_rvtt_cc

} // anon namespace

gimple_opt_pass *
make_pass_rvtt_cc (gcc::context *ctxt)
{
  return new pass_rvtt_cc (ctxt);
}
