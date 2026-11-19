/* Pass to complete handling of the SFPU synth insns
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
#include "target.h"
#include "rtl.h"
#include "tree.h"
#include "tree-pass.h"
#include "print-rtl.h"
#include "insn-config.h"
#include "insn-attr.h"
#include "insn-codes.h"
#include "recog.h"
#include "rvtt.h"
#include <unordered_map>

// This phase of the non-immediate processing works by:
//  - finding all of the load_immediate calls and creating a lookup table
//    based on their id (the value currently being loaded, as set in the
//    nonimm-tag gimple pass)
//  - finding all of the non-immediate insns and matching them via the table
//    to their load immediate
//  - updating the immediate value to match that needed by the insn.  if the
//    value was already set, then confirming the match.  if the old value and
//    new value do not match, the nonimm is flagged to emit the fallback code
static void
transform (function *fn)
{
  struct synth
  {
    rtx_insn_list *ops = nullptr; // synth_opcodes
    rtx_insn_list *uses = nullptr; // synthed insns
  };
  std::vector<synth> synths;

  basic_block bb;

  // Gather
  FOR_EACH_BB_FN (bb, fn)
    {
      rtx_insn *insn;

      FOR_BB_INSNS (bb, insn)
	{
	  if (!NONJUMP_INSN_P (insn))
	    continue;

	  unsigned id = 0;
	  int icode = recog_memoized (insn);
	  if (icode < 0)
	    continue;
	  bool is_opcode = icode == CODE_FOR_rvtt_synth_opcode;
	  if (is_opcode)
	    id = INTVAL (XVECEXP (SET_SRC (PATTERN (insn)), 0, 0));
	  else if (get_attr_type (insn) != TYPE_TENSIX)
	    continue;
	  else if (INSN_CODE (insn) == CODE_FOR_rvtt_ttinsn_int)
	    continue;
	  else
	    {
	      rtx pat = PATTERN (insn);
	      if (GET_CODE (pat) == PARALLEL)
		pat = XVECEXP (pat, 0, 0);
	      if (GET_CODE (pat) == SET)
		pat = SET_SRC (pat);
	      if (GET_CODE (pat) != UNSPEC_VOLATILE)
		// Simple set
		continue;
	      if (!MEM_P (XVECEXP (pat, 0, 0)))
		continue;
	      id = rvtt_synth (INTVAL (XVECEXP (pat, 0, rvtt_synth::IX_encode))).id ();
	    }
	  if (synths.size () <= id)
	    synths.resize (id + 1);

	  auto &elt = synths[id];
	  auto &slot = is_opcode ? elt.ops : elt.uses;
	  slot = alloc_INSN_LIST (insn, slot);
	  if (dump_file)
	    {
	      fprintf (dump_file, "[%u] collecting %s ",
		       id, is_opcode ? "synth" : "use");
	      dump_insn_slim (dump_file, insn);
	    }
	}
    }

  if (synths.empty ())
    // Nothing to do.
    return;

  // For each id in use, find the modal opcode value and use that
  std::unordered_map<unsigned, unsigned> map;
  unsigned id = -1;
  for (auto &synth : synths)
    {
      id++;
      if (!synth.uses)
	{
	  gcc_assert (!synth.ops);
	  continue;
	}
      gcc_assert (synth.ops);

      // Count the use patterns
      for (auto *use = synth.uses; use; use = use->next ())
	{
	  rtx_insn *insn = use->insn ();
	  unsigned opcode = 0;
	  rtx pat = PATTERN (insn);
	  if (GET_CODE (pat) == PARALLEL)
	    pat = XVECEXP (pat, 0, 0);
	  rtx dst = nullptr;
	  if (GET_CODE (pat) == SET)
	    {
	      dst = SET_DEST (pat);
	      pat = SET_SRC (pat);
	      gcc_assert (GET_MODE (dst) == XTT32SImode);
	    }

	  auto enc = rvtt_synth (INTVAL (XVECEXP (pat, 0, rvtt_synth::IX_encode)));

	  if (dst)
	    opcode |= (REGNO (dst) - SFPU_REG_FIRST) << enc.dst_shift ();

	  rtx src = XVECEXP (pat, 0, rvtt_synth::IX_src);
	  gcc_assert (GET_MODE (src) == XTT32SImode);
	  unsigned regno = 0;
	  if (REG_P (src))
	    regno = REGNO (src) - SFPU_REG_FIRST;
	  else
	    {
	      gcc_assert (GET_CODE (src) == UNSPEC);
	      if (XINT (src, 1) == UNSPEC_SFPCSTLREG)
		regno = INTVAL (XVECEXP (src, 0, 0));
	      else
		src = nullptr;
	    }
	  if (src)
	    opcode |= regno << enc.src_shift ();

	  opcode |= INTVAL (XVECEXP (pat, 0, rvtt_synth::IX_opcode));

	  if (dump_file)
	    fprintf (dump_file, "[%u] opcode %08x\n", id, opcode);
	  map[opcode]++;
	}

      // Find the mode
      unsigned count = 0;
      unsigned opcode = 0;
      for (auto &slot : map)
	if (slot.second > count
	    || (slot.second == count && slot.first < opcode))
	  {
	    count = slot.second;
	    opcode = slot.first;
	  }
      map.clear ();

      if (dump_file)
	fprintf (dump_file, "[%u] selecting opcode %08x (%u uses)\n",
		 id, opcode, count);

      // Update all the insns
      for (auto *op = synth.ops; op;)
	{
	  rtx_insn *insn = op->insn ();
	  auto *next = op->next ();
	  free_INSN_LIST_node (op);
	  op = next;

	  rtx unspec = SET_SRC (PATTERN (insn));
	  rtx &op_slot = XVECEXP (unspec, 0, 1);
	  rtx op_rtx = gen_rtx_CONST_INT (SImode, INTVAL (op_slot) + opcode);
	  op_slot = op_rtx;
	  if (rtx note = find_reg_equal_equiv_note (insn))
	    {
	      gcc_checking_assert (GET_CODE (XEXP (note, 0)) == UNSPEC);
	      XEXP (note, 0) = gen_rtx_UNSPEC (GET_MODE (unspec), XVEC (unspec, 0),
					       XINT (unspec, 1));
	    }
	  if (dump_file)
	    {
	      fprintf (dump_file, "[%u] updating synth ", id);
	      dump_insn_slim (dump_file, insn);
	    }
	}
      rtx op_rtx = gen_rtx_CONST_INT (SImode, opcode);
      for (auto *use = synth.uses; use;)
	{
	  rtx_insn *insn = use->insn ();
	  auto *next = use->next ();
	  free_INSN_LIST_node (use);
	  use = next;

	  rtx pat = PATTERN (insn);
	  if (GET_CODE (pat) == PARALLEL)
	    pat = XVECEXP (pat, 0, 0);
	  if (GET_CODE (pat) == SET)
	    pat = SET_SRC (pat);
	  XVECEXP (pat, 0, rvtt_synth::IX_opcode) = op_rtx;
	  if (dump_file)
	    {
	      fprintf (dump_file, "[%u] updating use ", id);
	      dump_insn_slim (dump_file, insn);
	    }
	}
    }
}

namespace {

const pass_data pass_data_rvtt_synth_opcode =
{
  RTL_PASS, /* type */
  "rvtt_synth_opcode", /* name */
  OPTGROUP_NONE, /* optinfo_flags */
  TV_NONE, /* tv_id */
  0, /* properties_required */
  0, /* properties_provided */
  0, /* properties_destroyed */
  0, /* todo_flags_start */
  0, /* todo_flags_finish */
};

class pass_rvtt_synth_opcode : public rtl_opt_pass
{
public:
  pass_rvtt_synth_opcode (gcc::context *ctxt)
    : rtl_opt_pass (pass_data_rvtt_synth_opcode, ctxt)
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
};

}

rtl_opt_pass *
make_pass_rvtt_synth_opcode (gcc::context *ctxt)
{
  return new pass_rvtt_synth_opcode (ctxt);
}
