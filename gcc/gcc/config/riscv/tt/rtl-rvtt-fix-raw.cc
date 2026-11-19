/* Pass to work around GS' memory aribtration bug
   Copyright (C) 2022-2025 Tenstorrent Inc.
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
#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "backend.h"
#include "rtl.h"
#include "tree-pass.h"
#include "cfgbuild.h"
#include "rvtt.h"

#define DUMP(...) //fprintf(stderr, __VA_ARGS__)

static bool
load_mem_p (rtx pat)
{
  if (GET_CODE (pat) != SET)
    return false;

  if (GET_CODE (SET_DEST (pat)) != REG)
    return false;

  rtx src = SET_SRC (pat);
  if (GET_CODE (src) == CALL
      || GET_CODE (src) == ASM_OPERANDS)
    return false;

  return contains_mem_rtx_p (src);
}

static bool
get_mem_reg_and_offset (rtx pat, int *reg, int *offset)
{
  if (GET_CODE (pat) == ZERO_EXTEND
      || GET_CODE (pat) == SIGN_EXTEND)
    pat = XEXP(pat, 0);

  if (GET_CODE (pat) == ASM_OPERANDS)
    return false;

  gcc_assert (MEM_P (pat));

  if (REG_P (XEXP (pat, 0)))
    {
      *reg = REGNO(XEXP(pat, 0));;
      *offset = 0;
    }
  else if (GET_CODE (XEXP(pat, 0)) != PLUS
	   && GET_CODE (XEXP(pat, 0)) != LO_SUM)
    return false;
  else
    {
      gcc_assert (REG_P (XEXP (XEXP (pat, 0), 0)));

      *offset = CONST_INT_P (XEXP (XEXP (pat, 0), 1))
	? INTVAL (XEXP (XEXP (pat, 0), 1))
	: 0;
      *reg = REGNO (XEXP (XEXP (pat, 0), 0));
    }

  return true;
}

static void
emit_load (rtx_insn *insn, bool before, rtx mem)
{
  mem = copy_rtx (mem);
  MEM_VOLATILE_P (mem) = true;
  rtx new_insn = gen_rtx_SET (gen_rtx_REG (SImode, 0), gen_rtx_ZERO_EXTEND (SImode, mem));
  if (before)
    emit_insn_before (new_insn, insn);
  else
    emit_insn_after (new_insn, insn);
}

static bool
clobbers_p (int regno, rtx pat)
{
  switch (GET_CODE (pat))
    {
    default:
      return false;

    case PARALLEL:
      {
	auto &vec = XVEC (pat, 0);
	for (unsigned ix = GET_NUM_ELEM (vec); ix--;)
	  if (clobbers_p (regno, RTVEC_ELT (vec, ix)))
	    return true;
      }
      return false;

    case CLOBBER:
      pat = XEXP (pat, 0);
      break;

    case SET:
      pat = SET_DEST (pat);
      break;
    }

  return refers_to_regno_p (regno, pat);
}

// WH has a read after write hazard bug where loading a word after a byte or
// half store issues the load before the store.  The bug is in the address
// comparator logic and it’s 32bits wide. if addresses match, RAW hazard will
// be detected. So if the shorter store is word-aligned, we have no hazard. (we
// do not take advantage of that) Mem logic is prioritizing loads over stores
// and even though there’s no reorder buffer, 2 loads could get issued before a
// store actually gets out. If there is an intervening store, it is not clear
// whether the hazard is resolved.  As the bug is very sensitive, we anull it
// in all cases by placing a short load as late as possible after the short
// store. That's when we encounter the first control-flow change, write to
// store's ptr register, a load of any size, or the end of the block. (It is
// desirable to sink the load as late as possible.)

static void
workaround_raw (function *cfn)
{
  DUMP("RAW pass on: %s\n", function_name(cfn));

  basic_block bb;
  FOR_EACH_BB_FN (bb, cfn)
    {
      DUMP("Processing BB %d\n", bb->index);
      rtx_insn *insn;
      bool have_store = false;
      int store_ptr_regno = 0;
      rtx store_mem = nullptr;
      FOR_BB_INSNS (bb, insn)
	{
	  if (!NONDEBUG_INSN_P (insn))
	    continue;

	  rtx insn_pat = PATTERN (insn);
	  bool new_store = false;

	  if (GET_CODE (insn_pat) == SET
	      && GET_CODE (SET_DEST (insn_pat)) == MEM)
	    {
	      machine_mode mode = GET_MODE (SET_SRC (insn_pat));
	      if ((mode == HImode || mode == QImode)
		  && !rvtt_reg_store_p (insn_pat))
		new_store = true;
	    }

	  if (!have_store)
	    ;
	  else if (new_store
		   || GET_CODE (insn) == CALL_INSN
		   || load_mem_p (insn_pat)
		   || clobbers_p (store_ptr_regno, insn_pat))
	    {
	      // Emit the war when we hit a load or if the base reg gets modified
	      DUMP("emitting raw war before load\n");
	      emit_load (insn, true, store_mem);
	      have_store = false;
	    }
	  else
	    gcc_assert (insn == BB_END (bb)
			|| !control_flow_insn_p (insn));

	  if (new_store)
	    {
	      // Found a potential RAW issue store
	      int dummy_offset;
	      get_mem_reg_and_offset (SET_DEST (insn_pat), &store_ptr_regno, &dummy_offset);
	      store_mem = SET_DEST (insn_pat);
	      have_store = true;
	      DUMP("raw war pending for [%d]\n", war_ptr_regno);
	    }
	}

      if (have_store)
	{
	  DUMP("emitting raw war at end of bb\n");
	  emit_load (BB_END (bb), control_flow_insn_p (BB_END (bb)), store_mem);
	  have_store = false;
	}
    }
  DUMP("out raw pass\n");
}

namespace {

const pass_data pass_data_rvtt_fix_raw =
{
  RTL_PASS, /* type */
  "rvtt_fix_raw", /* name */
  OPTGROUP_NONE, /* optinfo_flags */
  TV_NONE, /* tv_id */
  0, /* properties_required */
  0, /* properties_provided */
  0, /* properties_destroyed */
  0, /* todo_flags_start */
  0, /* todo_flags_finish */
};

class pass_rvtt_fix_raw : public rtl_opt_pass
{
private:

public:
  pass_rvtt_fix_raw (gcc::context *ctxt)
    : rtl_opt_pass (pass_data_rvtt_fix_raw, ctxt)
  {
  }

  virtual bool gate (function *) override
  {
    return riscv_tt_fix_wh_raw > 0;
  }
  
  /* opt_pass methods: */
  virtual unsigned execute (function *cfn) override
    {
      workaround_raw (cfn);

      return 0;
    }
}; // class pass_rvtt_fix_wh

} // anon namespace

rtl_opt_pass *
make_pass_rvtt_fix_raw (gcc::context *ctxt)
{
  return new pass_rvtt_fix_raw (ctxt);
}
