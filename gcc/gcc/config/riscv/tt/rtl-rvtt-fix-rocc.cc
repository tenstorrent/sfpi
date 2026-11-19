/* Pass to work around GS' memory aribtration bug
   Copyright (C) 2026 Tenstorrent Inc.
   Written by Nathan Sidwell (nsidwell@tenstorrent.com, nathan@acm.org).

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
#include "tree-pass.h"
#include "print-rtl.h"
#include "insn-config.h"
#include "insn-attr.h"
#include "insn-codes.h"
#include "recog.h"
#include "rvtt.h"
#include "tm-preds.h"

/* Quasar ROCC has two errata with addgen insns.

   1) All addrgens that produce a value need a barrier before them.

   2) All addrgen_push insns need a barrier before the next addrgen to the same
   generator (first operand).

   3) The barrier can be a ttrocc.nop insn
   
   4) Control flow into a subordinate function or return from the current
   function should presume a problematic instruction follows (so insert
   barrier).

   5) The barrier should be inserted as late as possible (to minimize the
   possibility of a stall).

*/

/* insert barrier before probe.  */

static void
insert_barrier (rtx_insn *insn, rtx_insn *probe)
{
  auto barrier = emit_insn_before (gen_riscv_ttrocc_nop (), probe);

  if (dump_file)
    {
      fprintf (dump_file, "Inserting barrier ");
      dump_insn_slim (dump_file, barrier);
      fprintf (dump_file, "  before ");
      dump_insn_slim (dump_file, probe);
      if (insn != probe)
	{
	  fprintf (dump_file, "  because of ");
	  dump_insn_slim (dump_file, insn);
	}
    }
}

static void
maybe_insert_barriers (std::vector<basic_block> &visited, function *fn, basic_block bb,
		       rtx arg, rtx_insn *insn, rtx_insn *probe)
{
  if (!probe)
    {
      if (bb->flags & BB_VISITED)
	return;
      bb->flags |= BB_VISITED;
      visited.push_back (bb);
      probe = BB_HEAD (bb);
    }

  for (; probe != NEXT_INSN (BB_END (bb));
       probe = NEXT_INSN (probe))
    {
      if (!NONDEBUG_INSN_P (probe))
	continue;

      if (GET_CODE (probe) == CALL_INSN)
	goto insert;

      if (recog_memoized (probe) < 0)
	continue;

      if (INSN_CODE (probe) == CODE_FOR_riscv_ttrocc_nop)
	return;

      if (recog_memoized (probe) < 0)
	continue;

      if (get_attr_xtt_rocc_addrgen (probe) != XTT_ROCC_ADDRGEN_NO)
	{
	  rtx pat = PATTERN (probe);
	  if (GET_CODE (pat) == SET)
	    pat = SET_SRC (pat);
	  rtx buf = XVECEXP (pat, 0, 0);
	  if (rtx_equal_p (buf, arg))
	    goto insert;
	}
      continue;

    insert:
      insert_barrier (insn, probe);
      return;
    }

  // Walk all the successors
  edge_iterator ei;
  edge e;
  FOR_EACH_EDGE (e, ei, bb->succs)
    if (e->dest == EXIT_BLOCK_PTR_FOR_FN (fn))
      {
	// Insert barrier before control flow out of fn
	// Last insn must be control flow, so before
	insert_barrier (insn, BB_END (bb));
	return;
      }

  // Otherwise walk into each successor
  FOR_EACH_EDGE (e, ei, bb->succs)
    maybe_insert_barriers (visited, fn, e->dest, arg, insn, nullptr);
}

static void
workaround_rocc (function *fn)
{
  std::vector<basic_block> visited;
  std::vector<std::pair<basic_block, rtx_insn *>> pushes; // pushes to post process

  basic_block bb;
  // first handle result-prodcucing addrgen insns and remember pushes.
  FOR_ALL_BB_FN (bb, fn)
    {
      rtx_insn *insn;

      bb->flags &= ~BB_VISITED;
      bool barriered = false;
      FOR_BB_INSNS (bb, insn)
	{
	  if (!NONDEBUG_INSN_P (insn))
	    continue;

	  if (recog_memoized (insn) < 0)
	    continue;

	  if (GET_CODE (insn) == CALL_INSN)
	    barriered = false;

	  if (get_attr_type (insn) != TYPE_TTROCC)
	    continue;

	  if (INSN_CODE (insn) == CODE_FOR_riscv_ttrocc_nop)
	    barriered = true;

	  auto addrgen = get_attr_xtt_rocc_addrgen (insn);
	  switch (addrgen)
	    {
	    case XTT_ROCC_ADDRGEN_NO:
	      break;

	    case XTT_ROCC_ADDRGEN_YES:
	      if (!barriered && GET_CODE (PATTERN (insn)) == SET)
		// Insert barrier before it
		insert_barrier (insn, insn);
	      barriered = false;
	      break;

	    case XTT_ROCC_ADDRGEN_PUSH:
	      pushes.emplace_back (bb, insn);
	      break;
	    }
	}
    }

  // Now handle the pushes -- done as a separate pass because we might have
  // inserted barriers that are helpful.
  for (auto const &pair : pushes)
    {
      rtx arg = XVECEXP (PATTERN (pair.second), 0, 0);
      maybe_insert_barriers (visited, fn, pair.first, arg, pair.second, NEXT_INSN (pair.second));
      for (auto b : visited)
	b->flags &= ~BB_VISITED;
      visited.clear ();
    }
}

namespace {

const pass_data pass_data_rvtt_fix_rocc =
{
  RTL_PASS, /* type */
  "rvtt_fix_rocc", /* name */
  OPTGROUP_NONE, /* optinfo_flags */
  TV_NONE, /* tv_id */
  0, /* properties_required */
  0, /* properties_provided */
  0, /* properties_destroyed */
  0, /* todo_flags_start */
  0, /* todo_flags_finish */
};

class pass_rvtt_fix_rocc : public rtl_opt_pass
{
private:

public:
  pass_rvtt_fix_rocc (gcc::context *ctxt)
    : rtl_opt_pass (pass_data_rvtt_fix_rocc, ctxt)
  {
  }

  virtual bool gate (function *) override
  {
    return riscv_tt_fix_qsr_rocc > 0;
  }
  
  /* opt_pass methods: */
  virtual unsigned execute (function *cfn) override
    {
      workaround_rocc (cfn);

      return 0;
    }
}; // class pass_rvtt_fix_rocc

} // anon namespace

rtl_opt_pass *
make_pass_rvtt_fix_rocc (gcc::context *ctxt)
{
  return new pass_rvtt_fix_rocc (ctxt);
}
