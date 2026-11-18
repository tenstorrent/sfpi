/* Pass to generate SFPU synth-opcode and opcode synth sequences for
   currently-non-constant operands.
   Copyright (C) 2026 Tenstorrent Inc.
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
#include "gimple-iterator.h"
#include "gimple-ssa.h"
#include "tree-phinodes.h"
#include "ssa-iterators.h"
#include "value-range.h"
#include "tree-ssa-propagate.h"
#include "stringpool.h"
#include "tree-ssanames.h"
#include "insn-config.h"
#include "insn-attr.h"
#include "insn-codes.h"
#include "regs.h"
#include "memmodel.h"
#include "ira.h"
#include "recog.h"
#include "print-rtl.h"
#include "rvtt.h"

// A combine-like pass that copies cstlreg unspecs into the insns that use
// them. (As described in the related gimple pass, we can't rely on combine,
// late-combine, ira or other passes to do this in all cases.)

// A combine-like pass that
// 1 copies cstlreg unspecs into the insns that use
// them.  (As described in the related gimple pass, we can't rely on combine,
// late-combine, ira or other passes to do this in all cases.)
// 2. copies cleave-together inputs to cleave-apart outputs, thereby handling
// multi-register builtin results.

static void
transform (function *fn)
{
  struct defs_t
  {
    rtx val;
    basic_block bb;
  };
  defs_t *reg_vals = XCNEWVEC (defs_t, max_reg_num ());
  basic_block bb;
  rtx_insn *insn;
  std::vector<unsigned> invalidate;
  struct operand
  {
    rtx *loc;
    unsigned regno;
  };
  std::vector<operand> operands;

  FOR_EACH_BB_FN (bb, fn)
    FOR_BB_INSNS (bb, insn)
    {
      if (GET_CODE (insn) != INSN)
	continue;
      rtx pattern = PATTERN (insn);

      if (GET_CODE (pattern) == USE)
	continue;
      if (GET_CODE (pattern) == CLOBBER)
	continue;

      if (get_attr_type (insn) != TYPE_TENSIX)
	continue;

      if (GET_CODE (pattern) == SET
	  && REG_P (SET_DEST (pattern))
	  && GET_CODE (SET_SRC (pattern)) == UNSPEC)
	{
	  auto src = SET_SRC (pattern);
	  switch (XINT (src, 1))
	    {
	    default:
	      break;

	    case UNSPEC_SFPCLEAVE:
	      {
		rtx slot = XVECEXP (src, 0, 1);
		if (GET_CODE (slot) == CONST_INT)
		  {
		    // select
		    unsigned ix = INTVAL (slot);
		    unsigned regno = REGNO (XVECEXP (src, 0, 0));

		    char const *msg = nullptr;
		    if (reg_vals[regno].bb != bb)
		      msg = "Failed to replace select";
		    else
		      {
			rtx sel = XVECEXP (reg_vals[regno].val, 0, ix);

			bool ok = validate_change (insn, &SET_SRC (pattern), sel, false);
			gcc_assert (ok);
			msg = "Replaced select";
		      }

		    if (dump_file)
		      {
			fprintf (dump_file, "%s %u\n", msg, regno);
			dump_insn_slim (dump_file, insn);
			fprintf (dump_file, "\n");
		      }
		    reg_vals[REGNO (SET_DEST (pattern))].bb = nullptr;
		    continue;
		  }
	      }
	      // FALLTHROUGH

	    case UNSPEC_SFPNOVAL:
	    case UNSPEC_SFPCSTLREG:
	      {
		auto regno = REGNO (SET_DEST (pattern));
		reg_vals[regno].val = src;
		reg_vals[regno].bb = bb;
	      }
	      continue;
	    }
	}

      auto find_operands
	= [&operands, &invalidate, bb, reg_vals, insn](auto &self, rtx *slot) -> void
      {
	switch (GET_CODE (*slot))
	  {
	  default:
	    // Unknown tensix insn component
	    gcc_unreachable ();

	  case PARALLEL:
	  case UNSPEC:
	  case UNSPEC_VOLATILE:
	    {
	      // All 3 have the vector at position 0
	      auto &vec = XVEC (*slot, 0);
	      for (unsigned ix = GET_NUM_ELEM (vec); ix--;)
		self (self, &RTVEC_ELT (vec, ix));
	    }
	    break;

	  case SET:
	    {
	      auto dst = SET_DEST (*slot);
	      if (REG_P (dst))
		invalidate.push_back (REGNO (dst));
	      self (self, &SET_SRC (*slot));
	    }
	    break;

	  case REG:
	    {
	      unsigned regno = REGNO (*slot);
	      if (reg_vals[regno].bb == bb
		  && XINT (reg_vals[regno].val, 1) != UNSPEC_SFPCLEAVE)
		operands.push_back ({slot, regno});
	    }
	    break;

	  case CONST_INT:
	  case MEM:
	  case CLOBBER:
	  case SCRATCH: 
	  case USE:
	    break;
	  }
      };

      find_operands (find_operands, &pattern);

      if (!operands.empty ())
	{
	  // We have to deal with match_dups, where multiple operands must be
	  // changed simultaneously.  In general we could try every combination
	  // of operands reading the same input register, but it is sufficient
	  // just to try changing all such operands simultaneously.
	  std::sort (operands.begin (), operands.end (),
		     [] (auto const &a, auto const &b) { return a.regno < b.regno; });

	  for (auto pos = operands.begin (); pos != operands.end ();)
	    {
	      unsigned regno = pos->regno;
	      rtx val = reg_vals[regno].val;

	      for (; pos != operands.end () && pos->regno == regno; ++pos)
		{
		  if (GET_CODE (val) == UNSPEC)
		    // Do not share unspec RTL
		    val = gen_rtx_UNSPEC (GET_MODE (val), XVEC (val, 0),
					  XINT (val, 1));

		  validate_change (insn, pos->loc, val, true);
		}

	      bool applied = apply_change_group ();
	      if (dump_file)
		{
		  fprintf (dump_file, "%s const register %u with ",
			   applied ? "Replaced" : "Failed to replace", regno);
		  dump_value_slim (dump_file, val, false);
		  fprintf (dump_file, "\n");
		  dump_insn_slim (dump_file, insn);
		  fprintf (dump_file, "\n");
		}
	    }
	}
      for (auto reg : invalidate)
	reg_vals[reg].bb = nullptr;
      invalidate.clear ();
    }
  XDELETEVEC (reg_vals);
}

namespace {

const pass_data pass_data_rvtt_unspec_prop_rtl =
{
  RTL_PASS, /* type */
  "rvtt_unspec_prop", /* name */
  OPTGROUP_NONE, /* optinfo_flags */
  TV_NONE, /* tv_id */
  0, /* properties_required */
  0, /* properties_provided */
  0, /* properties_destroyed */
  0, /* todo_flags_start */
  0, /* todo_flags_finish */
};

class pass_rvtt_unspec_prop_rtl : public rtl_opt_pass
{
public:
  pass_rvtt_unspec_prop_rtl (gcc::context *ctxt)
    : rtl_opt_pass (pass_data_rvtt_unspec_prop_rtl, ctxt)
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

} // anon namespace

rtl_opt_pass *
make_pass_rvtt_unspec_prop_rtl (gcc::context *ctxt)
{
  return new pass_rvtt_unspec_prop_rtl (ctxt);
}
