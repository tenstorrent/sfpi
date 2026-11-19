/* Pass to work around WH/BH ebreak bug.
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
#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "backend.h"
#include "rtl.h"
#include "tree-pass.h"
#include "print-rtl.h"
#include "insn-config.h"
#include "recog.h"

constexpr unsigned NUM_NOPS = 8;

// After an ebreak, insert 8 nops.  It is unclear whether one can emit a loop
// with similar timing charactaristics.

static void
workaround_ebreak (function *cfn)
{
  basic_block bb;
  FOR_EACH_BB_FN (bb, cfn)
    {
      rtx_insn *insn;
      FOR_BB_INSNS (bb, insn)
	{
	  if (GET_CODE (insn) != INSN)
	    continue;
	  if (recog_memoized (insn) >= 0)
	    continue;

	  // Ugh, why is this so hard?
	  rtx body = PATTERN (insn);
	  if (GET_CODE (body) == PARALLEL)
	    body = XVECEXP (body, 0, 0);
	  if (GET_CODE (body) == SET)
	    body = SET_SRC (body);

	  location_t loc = UNKNOWN_LOCATION;
	  char const *tmpl = nullptr;
	  switch (GET_CODE (body))
	    {
	    default:
	      gcc_unreachable ();

	    case USE:
	    case CLOBBER:
	      continue;

	    case ASM_INPUT:
	      tmpl = XSTR (body, 0);
	      loc = ASM_INPUT_SOURCE_LOCATION (body);
	      break;

	    case ASM_OPERANDS:
	      tmpl = ASM_OPERANDS_TEMPLATE (body);
	      loc = ASM_OPERANDS_SOURCE_LOCATION (body);
	      break;
	    }

	  if (0 != strcmp (tmpl, "ebreak"))
	    continue;

	  if (dump_file)
	    {
	      fprintf (dump_file, "Emitting nops after:");
	      dump_insn_slim (dump_file, insn);
	    }

	  for (unsigned ix = NUM_NOPS; ix--;)
	    {
	      body = gen_rtx_ASM_INPUT_loc (VOIDmode, ggc_strdup ("nop"), loc);
	      MEM_VOLATILE_P (body) = true;
	      rtx nop = gen_rtx_PARALLEL (VOIDmode, rtvec_alloc (2));
	      XVECEXP (nop, 0, 0) = body;
	      rtx clobber = gen_rtx_SCRATCH (VOIDmode);
	      clobber = gen_rtx_MEM (BLKmode, clobber);
	      clobber = gen_rtx_CLOBBER (VOIDmode, clobber);
	      XVECEXP (nop, 0, 1) = clobber;
	      emit_insn_after (nop, insn);
	    }
	}
    }
}

namespace {

const pass_data pass_data_rvtt_fix_ebreak =
{
  RTL_PASS, /* type */
  "rvtt_fix_ebreak", /* name */
  OPTGROUP_NONE, /* optinfo_flags */
  TV_NONE, /* tv_id */
  0, /* properties_required */
  0, /* properties_provided */
  0, /* properties_destroyed */
  0, /* todo_flags_start */
  0, /* todo_flags_finish */
};

class pass_rvtt_fix_ebreak : public rtl_opt_pass
{
private:

public:
  pass_rvtt_fix_ebreak (gcc::context *ctxt)
    : rtl_opt_pass (pass_data_rvtt_fix_ebreak, ctxt)
  {
  }

  virtual bool gate (function *) override
  {
    return riscv_tt_fix_whbh_ebreak > 0;
  }
  
  /* opt_pass methods: */
  virtual unsigned execute (function *cfn) override
    {
      workaround_ebreak (cfn);

      return 0;
    }
}; // class pass_rvtt_fix_wh

} // anon namespace

rtl_opt_pass *
make_pass_rvtt_fix_ebreak (gcc::context *ctxt)
{
  return new pass_rvtt_fix_ebreak (ctxt);
}
