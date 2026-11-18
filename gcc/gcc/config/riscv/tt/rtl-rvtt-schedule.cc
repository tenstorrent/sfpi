/* Pass to schedule tensix insns (insert nops)
   Copyright (C) 2022-2026 Tenstorrent Inc.
   Originated by Paul Keller (pkeller@tenstorrent.com).
   Rewritten Nathan Sidwell (nsidwell@tenstorrent.com, nathan@acm.org).

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

#define INCLUDE_VECTOR
#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "backend.h"
#include "rtl.h"
#include "tree.h"
#include "tree-pass.h"
#include "print-rtl.h"
#include "insn-config.h"
#include "insn-attr.h"
#include "insn-codes.h"
#include "recog.h"
#include "rvtt.h"
#include "tm-preds.h"

/* Walk the BB graph from PROBE_INSN until we meet a TENSIX insn. Return true
   if REGNO != 0 and the TENSIX insn is dependent.  Return true if REGNO == 0
   and the TENSIX insn is not a NOP. Return false in all other cases. If we
   meet the end of a block, recurse into successor blocks and return the first
   true we get.  Populate VISITED with the BB's we marked. Takes advantage of
   no multi-register values, no return values and no clobbers of TENSIX
   registers. */

static bool
find_next_insn (std::vector<basic_block> &visited, basic_block bb, int regno,
		rtx_insn *probe_insn, bool first = true)
{
  if (bb->flags & BB_VISITED)
    return false;

  if (first)
    {
      if (probe_insn == BB_END (bb))
	goto walk_succs;
      probe_insn = NEXT_INSN (probe_insn);
    }
  else
    {
      // Each block, other than the starting block, should only be
      // walked once -- don't get trapped in a loop of non-TENSIX
      // insns. The starting block should be walked exactly twice, if
      // reachable from itself.
      bb->flags |= BB_VISITED;
      visited.push_back (bb);
      if (!probe_insn)
	goto walk_succs;
    }

  for (; probe_insn != NEXT_INSN (BB_END (bb));
       probe_insn = NEXT_INSN (probe_insn))
    {
      if (GET_CODE (probe_insn) != INSN)
	continue;
      rtx pattern = PATTERN (probe_insn);

      if (GET_CODE (pattern) == USE)
	// The case where this would be a dependency does not arise.
	continue;
      if (GET_CODE (pattern) == CLOBBER)
	continue;

      if (get_attr_type (probe_insn) != TYPE_TENSIX)
	continue;

#if 1  // FIXME: Eventually permit
      if (INSN_CODE (probe_insn) == CODE_FOR_rvtt_ttinsn_int)
	continue;
#endif

      if (!regno)
	{
	  bool is_nop = recog_memoized (probe_insn) == CODE_FOR_rvtt_sfpnop;
	  if (dump_file)
	    {
	      fprintf (dump_file, "Found %snop insn ", is_nop ? "" : "non-");
	      dump_insn_slim (dump_file, probe_insn);
	    }
	  return !is_nop;
	}

      auto reg_used_p = [] (auto self, unsigned regno, rtx rtl) -> bool
      {
	switch (GET_CODE (rtl))
	  {
	  default:
	    // Unknown tensix insn component
	    gcc_unreachable ();
	    
	  case PARALLEL:
	  case UNSPEC:
	  case UNSPEC_VOLATILE:
	    {
	      // All 3 have the vector at position 0
	      auto &vec = XVEC (rtl, 0);
	      for (unsigned ix = GET_NUM_ELEM (vec); ix--;)
		if (self (self, regno, RTVEC_ELT (vec, ix)))
		  return true;
	    }
	    break;

	  case SET:
	    if (self (self, regno, SET_SRC (rtl)))
	      return true;
	    break;

	  case REG:
	    if (regno == REGNO (rtl))
	      return true;
	    break;

	  case CONST_INT:
	  case MEM:
	  case CLOBBER:
	  case USE:
	    break;
	  }
	return false;
      };

      bool is_dependent = reg_used_p (reg_used_p, regno, pattern);

      if (is_dependent)
	if (unsigned mask =
	    TARGET_XTT_TENSIX_BH ? XTT_DYNAMIC_BUG_BH :
	    TARGET_XTT_TENSIX_QSR ? XTT_DYNAMIC_BUG_QSR :
	    0)
	  // BH & QSR has scoreboarding, but with bugs
	  if (!(mask & get_attr_xtt_dynamic_bug (probe_insn)))
	    is_dependent = false;

      if (!is_dependent && !get_attr_length (probe_insn))
	continue;

      if (dump_file)
	{
	  fprintf (dump_file, "Found %sdependent insn ", is_dependent ? "" : "non-");
	  dump_insn_slim (dump_file, probe_insn);
	}
      return is_dependent;
    }

 walk_succs:;
  // Walk all the successors
  edge_iterator ei;
  edge e;
  FOR_EACH_EDGE (e, ei, bb->succs)
    if (find_next_insn (visited, e->dest, regno, BB_HEAD (e->dest), false))
      return true;

  return false;
}

/* INSN is an sfpload insn, this must either be separated by 1 insn from any sfpstore
   that is the same location, or turned into an sfpmov of the stored value.

   1) We presume any store that is not obviously the same location, is a
   different location.

   2) If the store is in the same bb, we replace the load with an sfpassign.

   3) If it is in the only predecessor block we do the same.

   4) Otherwise we insert an sfpnop after the store.

   5) Don't forget to adjust any reg-dead note on the store's value.

*/

static void
handle_qsr_load_latency (std::vector<basic_block> &visited, basic_block bb, rtx_insn *insn,
			 rtx addr, bool first = true)
{
  if (bb->flags & BB_VISITED)
    return;

  rtx_insn *probe;
  if (first)
    {
      if (insn == BB_HEAD (bb))
	goto walk_preds;
      probe = PREV_INSN (insn);
    }
  else
    {
      bb->flags |= BB_VISITED;
      visited.push_back (bb);
      probe = BB_END (bb);
      if (!probe)
	goto walk_preds;
    }

  for (;;)
    {
      if (GET_CODE (probe) == INSN
	  && recog_memoized (probe) >= 0
	  && get_attr_type (probe) == TYPE_TENSIX
	  && get_attr_length (probe))
	{
	  if (INSN_CODE (probe) != CODE_FOR_rvtt_sfpstore_int)
	    // Met not-a-store, there is no hazard
	    return;

	  // There is a hazard if
	  // 1) the store's ADDR_MODE is 7 (non-incrementing)
	  // 2) and the addr is the same constant
	  // We ignore the problem of checking dynamic addresses, that's hard
	  // in non-ssa form.
	  auto store = XVECEXP (PATTERN (probe), 0, 0);
	  if (CONST0_RTX (SImode) != XVECEXP (store, 0, rvtt_synth::IX_mem))
	    // Dynamic
	    return;

	  if (INTVAL (XVECEXP (store, 0, rvtt_synth::IX_src + 1)) != INTVAL (addr))
	    // Addresses are different -- do we need to worry about halves?
	    return;
	  if (INTVAL (XVECEXP (store, 0, rvtt_synth::IX_src + 3)) != SFPLOADSTORE_ADDR_MODE_NOINC)
	    // Store addr_mod is side-effecting
	    return;

	  // We have a hit, insert a nop after the store It's likely that it's
	  // outside the loop, but the load could be inside one.
	  auto inserted = emit_insn_after (gen_rvtt_sfpnop (), probe);

	  if (dump_file)
	    {
	      fprintf (dump_file, "Resolving sfpstore...sfpload hazard by\n"
		       "emitting ");
	      dump_insn_slim (dump_file, inserted);
	      fprintf (dump_file, "after ");
	      dump_insn_slim (dump_file, probe);

	      fprintf (dump_file, "bacause of ");
	      dump_insn_slim (dump_file, insn);
	      fprintf (dump_file, "\n");
	    }

	  return;
	}

      if (probe == BB_HEAD (bb))
	break;
      probe = PREV_INSN (probe);
    }

 walk_preds:;
  // Walk all the predecessors
  edge_iterator ei;
  edge e;
  FOR_EACH_EDGE (e, ei, bb->preds)
    handle_qsr_load_latency (visited, e->src, insn, addr, false);
}

// Perform instruction scheduling. We conditionally insert a nop after
// instructions.  Also handle sfpstore-sfpload latency on QSR

static void
transform (function *fn)
{
  std::vector<basic_block> visited;

  basic_block bb;
  FOR_ALL_BB_FN (bb, fn)
    bb->flags &= ~BB_VISITED;

  FOR_EACH_BB_FN (bb, fn)
    {
      rtx_insn *insn;

      FOR_BB_INSNS (bb, insn)
	{
	  if (GET_CODE (insn) != INSN)
	    continue;

	  if (recog_memoized (insn) < 0)
	    continue;

	  if (get_attr_type (insn) != TYPE_TENSIX)
	    continue;

	  enum xtt_delay delay = get_attr_xtt_delay (insn);
	  if (delay == XTT_DELAY_NONE)
	    {
	      if (INSN_CODE (insn) == CODE_FOR_rvtt_sfpload_lv_int
		  && TARGET_XTT_TENSIX_QSR)
		{
		  // Do we need a nop before this load?  Gimple schedule pass
		  // will have handled when we can propagate the stored value.
		  auto src = SET_SRC (XVECEXP (PATTERN (insn), 0, 0));
		  if (CONST0_RTX (SImode) == XVECEXP (src, 0, rvtt_synth::IX_mem))
		    {
		      auto addr = XVECEXP (src, 0, rvtt_synth::IX_src + 2);
		      handle_qsr_load_latency (visited, bb, insn, addr);
		      for (auto *bb : visited)
			bb->flags &= ~BB_VISITED;
		      visited.clear ();
		    }
		}

	      continue;
	    }

	  visited.reserve (n_basic_blocks_for_fn (fn));
	  bool insert = false;
	  if (delay == XTT_DELAY_STATIC)
	    {
	      insert = find_next_insn (visited, bb, 0, insn);
	      for (auto *bb : visited)
		bb->flags &= ~BB_VISITED;
	      visited.clear ();
	    }
	  else
	    {
	      gcc_assert (delay == XTT_DELAY_DYNAMIC);
	      auto find_next = [] (auto self, std::vector<basic_block> &visited, basic_block bb,
				   rtx_insn *insn, rtx rtl) -> bool
	      {
		switch (GET_CODE (rtl))
		  {
		  default:
		    gcc_unreachable ();

		  case SET:
		    if (REG_P (SET_DEST (rtl)))
		      {
			unsigned regno = REGNO (SET_DEST (rtl));
			if (SFPU_REG_P (regno))
			  {
			    // Writing to a constant reg falls on the floor
			    bool insert = regno < SFPU_REG_FIRST + SFPU_CREG_IDX_LWM
			      && find_next_insn (visited, bb, regno, insn);

			    for (auto *bb : visited)
			      bb->flags &= ~BB_VISITED;
			    visited.clear ();

			    return insert;
			  }
		      }
		    break;

		  case PARALLEL:
		    {
		      auto &vec = XVEC (rtl, 0);
		      for (unsigned ix = GET_NUM_ELEM (vec); ix--;)
			if (self (self, visited, bb, insn, RTVEC_ELT (vec, ix)))
			  return true;
		    }
		    break;

		  case CLOBBER:
		  case SCRATCH:
		    break;
		  }

		return false;
	      };

	      insert = find_next (find_next, visited, bb, insn, PATTERN (insn));
	    }

	  if (insert)
	    emit_insn_after (gen_rvtt_sfpnop (), insn);
	  if (dump_file)
	    {
	      fprintf (dump_file, "%snserting %s nop after ",
		       insert ? "I" : "Not i",
		       delay == XTT_DELAY_STATIC ? "static" : "dynamic");
	      dump_insn_slim (dump_file, insn);
	      fprintf (dump_file, "\n");
	    }
       }
    }
}

namespace {

const pass_data pass_data_rvtt_schedule =
{
  RTL_PASS, /* type */
  "rvtt_schedule", /* name */
  OPTGROUP_NONE, /* optinfo_flags */
  TV_NONE, /* tv_id */
  0, /* properties_required */
  0, /* properties_provided */
  0, /* properties_destroyed */
  0, /* todo_flags_start */
  0, /* todo_flags_finish */
};

class pass_rvtt_schedule : public rtl_opt_pass
{
public:
  pass_rvtt_schedule (gcc::context *ctxt)
    : rtl_opt_pass (pass_data_rvtt_schedule, ctxt)
  {}

  virtual bool gate (function *) override
  {
    return TARGET_XTT_TENSIX;
  }

  virtual unsigned execute (function *fn) override
  {
    transform (fn);
    return 0;
  }
}; // class pass_rvtt_schedule

} // anon namespace

rtl_opt_pass *
make_pass_rvtt_schedule (gcc::context *ctxt)
{
  return new pass_rvtt_schedule (ctxt);
}
