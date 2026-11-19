/* Pass to schedule tensix insns (insert nops)
   Copyright (C) 2022-2026 Tenstorrent Inc.
   Originated by Nathan Sidwell (nsidwell@tenstorrent.com, nathan@acm.org).

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

#define INCLUDE_PAIR
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
#include "tree-ssa-propagate.h"
#include "gimple-iterator.h"
#include "gimple-pretty-print.h"
#include "tree-ssa.h"
#include "tree-into-ssa.h"
#include "rvtt.h"

constexpr unsigned STORE_LOAD_WINDOW = 2;

static std::pair <gcall *, const rvtt_insn_data *>
find_store (gimple_stmt_iterator gsi,
	    rvtt_insn_data::insn_id store_id, unsigned slot_count)
{
  for (bool first = true;;)
    {
      for (; !gsi_end_p (gsi); gsi_prev (&gsi))
	if (auto *insnd = rvtt_get_insn_data (*gsi))
	  switch (insnd->id)
	    {
	    case rvtt_insn_data::sfpstore:
	    case rvtt_insn_data::sfpstoresrcs:
	      if (insnd->id == store_id)
		return {as_a <gcall *> (*gsi), insnd};

	      // Let's not hop over the other kind of store.
	      return {nullptr, nullptr};

	    case rvtt_insn_data::sfpload:
	    case rvtt_insn_data::sfploadsrcs:
	    case rvtt_insn_data::sfpload_lv:
	    case rvtt_insn_data::sfploadsrcs_lv:
	      if (first)
		{
		  // It's the original load, don't stop.
		  first = false;
		  break;
		}
	      // These might mutate load/store state.
	      [[fallthrough]];

	    case rvtt_insn_data::sfpbankdone:
	    case rvtt_insn_data::ttincrwc:
	      // These can mutate load/store state
	      return {nullptr, nullptr};

	    default:
	      if (insnd->is_empty ())
		// Don't count empty isnsn -- not fatal to count them, but they
		// (usually) expand to nothing.
		break;

	      // If the insn setscc, we can't look past it, even if the load is not
	      // merging a live value (consider the end of a v_if block).
	      if (!--slot_count
		  || insnd->sets_cc (as_a <gcall *> (*gsi)))
		return {nullptr, nullptr};
	    }

      // Walk the single predecessor, or fail
      if (!single_pred_p (gsi_bb (gsi)))
	return {nullptr, nullptr};

      gsi = gsi_last_bb (single_pred_edge (gsi_bb (gsi))->src);
    }
}

static bool
maybe_elide_load (gimple_stmt_iterator gsi, rvtt_insn_data::insn_id store_id,
		  const rvtt_insn_data *load_insnd)
{
  // +1 because we must skip over the load
  auto [store_call, store_insnd] = find_store (gsi, store_id, STORE_LOAD_WINDOW + 1);
  if (!store_call)
    return false;

  gcall *load_call = as_a <gcall *> (*gsi);

  // Determine if load_call loads store_call's input
  auto load_addr = gimple_call_arg (load_call, load_insnd->imm_arg ());
  auto store_addr = gimple_call_arg (store_call, store_insnd->imm_arg ());
  if (load_addr != store_addr
      && (TREE_CODE (load_addr) != INTEGER_CST
	  || TREE_CODE (store_addr) != INTEGER_CST
	  || !tree_int_cst_equal (load_addr, store_addr)))
    // Addresses are different
    return false;

  auto load_addr_mode = TREE_INT_CST_LOW (gimple_call_arg (load_call, load_insnd->mod_arg () + 1));
  auto store_addr_mode = TREE_INT_CST_LOW (gimple_call_arg (store_call, store_insnd->mod_arg () + 1));
  if (load_addr_mode != store_addr_mode
      || load_addr_mode != (TARGET_XTT_TENSIX_WH ? SFPLOADSTORE_ADDR_MODE_WH_NOINC
			    : SFPLOADSTORE_ADDR_MODE_NOINC))
    // Addr modes are side-effecting
    return false;

  auto load_addr_mod = TREE_INT_CST_LOW (gimple_call_arg (load_call, load_insnd->mod_arg ()));
  auto store_addr_mod = TREE_INT_CST_LOW (gimple_call_arg (store_call, store_insnd->mod_arg ()));

  if (!((1u << load_addr_mod) & SFPLOADSTORE_MOD0_FMT_COPY_MASK)
      || !((1u << store_addr_mod) & SFPLOADSTORE_MOD0_FMT_COPY_MASK))
    // mod is not tansfering the whole value
    return false;

  if (store_id == rvtt_insn_data::sfpstoresrcs)
    {
      auto load_done = TREE_INT_CST_LOW (gimple_call_arg (load_call, load_insnd->mod_arg () + 2));
      if (load_done)
	// load sets done flag
	return false;
    }

  // Load insn can take the store directly.  We're late in the gimple pipeline,
  // so it doesn't matter we're emitting an sfpassign.  The RTL passes will DTRT.
  auto *assign = rvtt_get_insn_data (rvtt_insn_data::sfpassign);
  bool live = load_insnd->is_live ();
  if (live)
    assign = assign->get_live ();

  gcall *ass_call = gimple_build_call (assign->decl (), assign->num_args ());
  gimple_set_location (ass_call, gimple_location (load_call));
  gimple_call_set_lhs (ass_call, gimple_call_lhs (load_call));
  gimple_call_set_arg (ass_call, int (live), gimple_call_arg (store_call, 1));
  if (live)
    gimple_call_set_arg (ass_call, 0, gimple_call_arg (load_call, 1));

  if (dump_file)
    {
      fprintf (dump_file, "Replacing ");
      print_gimple_stmt (dump_file, load_call, 0);
      fprintf (dump_file, "with ");
      print_gimple_stmt (dump_file, ass_call, 0);
      fprintf (dump_file, "propagating from ");
      print_gimple_stmt (dump_file, store_call, 0);
      fprintf (dump_file, "\n");
    }

  gsi_insert_after (&gsi, ass_call, GSI_SAME_STMT);
  unlink_stmt_vdef (*gsi);
  gsi_remove (&gsi, true);

  return true;
}

// Perform instruction scheduling. We conditionally insert a nop after
// instructions.

static unsigned
transform (function *fn)
{
  std::vector<basic_block> visited;
  basic_block bb;
  bool changed = false;

  FOR_EACH_BB_FN (bb, fn)
    bb->flags &= ~BB_VISITED;

  FOR_EACH_BB_FN (bb, fn)
    for (auto gsi = gsi_start_bb (bb); !gsi_end_p (gsi);
	 gsi_next (&gsi))
      {
	if (auto *insnd = rvtt_get_insn_data (*gsi))
	  switch (insnd->id)
	    {
	    default:
	      break;

	    case rvtt_insn_data::sfpload:
	    case rvtt_insn_data::sfpload_lv:
	      if (maybe_elide_load (gsi, rvtt_insn_data::sfpstore, insnd))
		changed = true;
	      break;

	    case rvtt_insn_data::sfploadsrcs:
	    case rvtt_insn_data::sfploadsrcs_lv:
	      if (maybe_elide_load (gsi, rvtt_insn_data::sfpstoresrcs, insnd))
		changed = true;
	      break;

	    case rvtt_insn_data::sfploadi:
	      // FIXME: loadi sinking
	      break;
	    }
      }

  return changed ? TODO_update_ssa : 0;
}

namespace {

const pass_data pass_data_rvtt_schedule_ssa =
{
  GIMPLE_PASS, /* type */
  "rvtt_schedule", /* name */
  OPTGROUP_NONE, /* optinfo_flags */
  TV_NONE, /* tv_id */
  PROP_ssa, /* properties_required */
  0, /* properties_provided */
  0, /* properties_destroyed */
  0, /* todo_flags_start */
  0, /* todo_flags_finish */
};

class pass_rvtt_schedule_ssa : public gimple_opt_pass
{
public:
  pass_rvtt_schedule_ssa (gcc::context *ctxt)
    : gimple_opt_pass (pass_data_rvtt_schedule_ssa, ctxt)
  {}

  virtual bool gate (function *) override
  {
    return TARGET_XTT_TENSIX;
  }

  virtual unsigned execute (function *fn) override
  {
    return transform (fn);
  }
}; // class pass_rvtt_schedule_ssa

} // anon namespace

gimple_opt_pass *
make_pass_rvtt_schedule_ssa (gcc::context *ctxt)
{
  return new pass_rvtt_schedule_ssa (ctxt);
}
