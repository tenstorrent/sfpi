/* Pass to issue diagnostics for SFPU operations
   Copyright (C) 2026 Tenstorrent Inc.
   Originated Nathan Sidwell (nsidwell@tenstorrent.com, nathan@acm.org).

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
#include "tree.h"
#include "stringpool.h"
#include "attribs.h"
#include "gimple.h"
#include "tree-pass.h"
#include "ssa.h"
#include "gimple-iterator.h"
#include "gimple-pretty-print.h"
#include "tree-ssa.h"
#include "tree-into-ssa.h"
#include "diagnostic-core.h"
#include "rvtt.h"

// Is TYPE an sfpu vector type?

static bool
is_sfpu_type (tree type)
{
  return TREE_CODE (type) == VECTOR_TYPE
    && lookup_attribute ("__xtt_vector", TYPE_ATTRIBUTES (type));
}

// We can't pass or return sfpu vector types.  Barf if we are.

static bool
check_function_type (location_t loc, tree type, bool inline_p)
{
  bool bad = false;
  auto error = [](location_t loc, bool ret, bool inline_p) {
    error_at (loc, inline_p
	      ? "cannot %s sfpu type (missing %<sfpi_inline%>?)"
	      : "cannot %s sfpu type",
	      ret ? "return" : "pass");
  };
  
  if (is_sfpu_type (TREE_TYPE (type)))
    {
      bad = true;
      error (loc, true, inline_p);
    }

  for (tree args = TYPE_ARG_TYPES (type); args; args = TREE_CHAIN (args))
    if (is_sfpu_type (TREE_VALUE (args)))
      {
	bad = true;
	error (loc, false, inline_p);
	break;
      }
  return bad;
}


// Check integral arguments passed to CALL, an INSND are within range.

static bool
check_int_args (bool is_early, const rvtt_insn_data *insnd, gcall *call)
{
  bool changed = false;

  for (int ix = 0; auto info = insnd->ops[ix]; ix++)
    {
      if (info.is_runtime ())
	continue;
      if (is_early != info.is_early ())
	continue;

      HOST_WIDE_INT val = 0;
      tree op = gimple_call_arg (call, info.argno ());
      if (TREE_CODE (op) != INTEGER_CST)
	{
	  if (insnd->has_iptr ()
	      && (!insnd->num_imms ()
		  || ix < insnd->num_imms ()))
	    continue;
	  error_at (gimple_nonartificial_location (call),
		    "argument %d is not a constant", info.argno () + 1);
	zap:
	  // If we don't make this correction, we'll likely crash, fail
	  // at RTL expansion or the assembler will barf
	  if (info.is_mod ())
	    {
	      // Compute the smallest valid mod value
	      unsigned mod = __builtin_ffs (info.mod ());
	      val = mod - (mod != 0);
	    }
	  tree zap = build_int_cst (TREE_TYPE (op), val);
	  gimple_call_set_arg (call, info.argno (), zap);
	  update_stmt (call);
	  changed = true;
	  continue;
	}

      val = int32_t (TREE_INT_CST_LOW (op));

      if (!info.is_xmod ())
	{
	  HOST_WIDE_INT upper = 0xf;
	  HOST_WIDE_INT lower = 0;
	  unsigned bias = 0;
	  if (!info.is_mod ())
	    {
	      unsigned bits = info.bits ();
	      if (!bits)
		bits = 32;
	      upper = (1u << (bits - 1)) - 1;
	      if (info.kind () != rvtt_insn_data::op_t::UNSIGNED)
		lower = ~upper;
	      if (info.kind () != rvtt_insn_data::op_t::SIGNED)
		upper = (upper << 1) | 1;
	      bias = info.bias ();
	    }

	  if (val > upper + bias || val < lower + bias)
	    {
	      if (info.is_checked ())
		error_at (gimple_location (call),
			  "argument %d %qE is out of range [%ld, %ld]",
			  info.argno () + 1, op,
			  long (lower + bias), long (upper + bias));

	      if (info.is_mod ())
		goto zap;

	      // Clip imm operands.  Keep nonnimm operands for for the moment,
	      // until we fix sfpxloadi
	      val -= bias;
	      HOST_WIDE_INT sign_bits
		= info.kind () == rvtt_insn_data::op_t::SIGNED && (val & lower)
		? ~upper : 0;
	      val = (val & upper) | sign_bits;
	      val += bias;
	      goto zap;
	    }
	}

      if (info.is_mod () || info.is_xmod ())
	{
	  unsigned mask = info.mod ();
	  if (!((1 << (val & 0xf)) & info.mod ()))
	    {
	      error_at (gimple_location (call),
			"argument %d %qE is invalid mod1 value (mask is 0x%x)",
			info.argno () + 1, op, mask);
	      goto zap;
	    }
	}
    }

  return changed;
}

static bool
is_undef_sfpu (tree val)
{
  return SSA_VAR_P (val)
    && is_sfpu_type (TREE_TYPE (val))
    && ssa_undefined_value_p (val, false);
}

static bool
is_memory (tree arg)
{
  return (TREE_CODE (arg) == MEM_REF
	  || TREE_CODE (arg) == COMPONENT_REF)
    && is_sfpu_type (TREE_TYPE (arg));
}

static tree
const_zero_reg (gimple_stmt_iterator *gsi, tree res = nullptr)
{
  auto *insnd = rvtt_get_insn_data (rvtt_insn_data::sfpreadlreg);
  gcall *new_stmt = gimple_build_call (insnd->decl (), 1);

  if (!res)
    res = make_ssa_name (TREE_TYPE (TREE_TYPE (insnd->decl ())));

  gimple_call_set_arg (new_stmt, 0, build_int_cst (unsigned_type_node, CREG_IDX_0));
  gimple_set_location (new_stmt, gimple_location (**gsi));
  gimple_call_set_lhs (new_stmt, res);

  gsi_insert_before (gsi, new_stmt, GSI_SAME_STMT);

  return res;
}

static bool
check_assign (gimple_stmt_iterator &gsi, bool is_early)
{
  if (!is_early)
    if (auto *lhs = gimple_get_lhs (*gsi))
      if (is_memory (lhs))
	{
	  error_at (gimple_nonartificial_location (*gsi),
		    "cannot write SFPU object to memory");
	  goto remove;
	}

  {
    auto *rhs = gimple_assign_rhs1 (*gsi);
    if (is_early ? is_undef_sfpu (rhs) : is_memory (rhs))
      {
	error_at (gimple_nonartificial_location (*gsi),
		  is_early ? "reading uninitialized"
		  : "cannot read SFPU object from memory");
	if (auto *lhs = gimple_get_lhs (*gsi))
	  const_zero_reg (&gsi, lhs);
	goto remove;
      }
  }

  return false;

 remove:;
  if (dump_file)
    {
      fprintf (dump_file, "Removing erroneous ");
      print_gimple_stmt (dump_file, *gsi, 2);
    }

  unlink_stmt_vdef (*gsi);
  gsi_remove (&gsi, true);

  return true;
}

static unsigned
check_early (function *fn)
{
  bool changed = false;
  basic_block bb;
  FOR_EACH_BB_FN (bb, fn)
    for (gimple_stmt_iterator gsi = gsi_start_bb (bb);
	 !gsi_end_p (gsi);)
      {
	if (auto *insnd = rvtt_get_insn_data (*gsi))
	  {
	    auto *call = as_a <gcall *> (*gsi);
	    changed |= check_int_args (true, insnd, call);

	    if (riscv_tt_fix_qsr_replay > 0 && insnd->id == rvtt_insn_data::ttreplay)
	      {
		// Quasar erratum, cannot exec-while-loading
		tree op = gimple_call_arg (call, insnd->imm_arg () + 4);
		if (!integer_zerop (op))
		  error_at (gimple_nonartificial_location (call),
			    "Quasar replay instruction cannot exec-while-load, "
			    "your program will behave erratically");
	      }

	    for (unsigned argno = 0, limit = gimple_call_num_args (call);
		 argno != limit; argno++)
	      {
		tree arg = gimple_call_arg (call, argno);
		if (is_undef_sfpu (arg))
		  {
		    error_at (gimple_nonartificial_location (call),
			      "argument %d of %qE is not initialized", argno + 1,
			      gimple_call_fndecl (call));
		    gimple_call_set_arg (call, argno,
					 const_zero_reg (&gsi));
		    update_stmt (call);
		    changed = true;
		  }
	      }
	  }
	else if (is_a <gassign *> (*gsi))
	  {
	    if (check_assign (gsi, true))
	      {
		changed = true;
		continue;
	      }
	  }
	else if (auto *call = dyn_cast<gcall *> (*gsi))
	  {
	    if (tree type = gimple_call_fntype (call))
	      if (check_function_type (gimple_nonartificial_location (call),
				       type, true))
		{
		  // Delete call, set lhs to something
		}
	  }
	gsi_next (&gsi);
   }
  return changed ? TODO_update_ssa : 0;
}

static unsigned
check_late (function *fn)
{
  bool changed = false;
  basic_block bb;
  FOR_EACH_BB_FN (bb, fn)
    for (gimple_stmt_iterator gsi = gsi_start_bb (bb);
	 !gsi_end_p (gsi);)
      {
	if (auto *insnd = rvtt_get_insn_data (*gsi))
	  changed |= check_int_args (false, insnd, as_a <gcall *> (*gsi));
	else if (is_a <gassign *> (*gsi))
	  {
	    if (check_assign (gsi, false))
	      {
		changed = true;
		continue;
	      }
	  }
	gsi_next (&gsi);
      }
  return changed ? TODO_update_ssa : 0;
}

namespace {

const pass_data pass_data_rvtt_check_early =
{
  GIMPLE_PASS, /* type */
  "rvtt_check_early", /* name */
  OPTGROUP_NONE, /* optinfo_flags */
  TV_NONE, /* tv_id */
  PROP_ssa, /* properties_required */
  0, /* properties_provided */
  0, /* properties_destroyed */
  0, /* todo_flags_start */
  0, /* todo_flags_finish */
};

class pass_rvtt_check_early : public gimple_opt_pass
{
public:
  pass_rvtt_check_early (gcc::context *ctxt)
    : gimple_opt_pass (pass_data_rvtt_check_early, ctxt)
  {}

  virtual bool gate (function *) override
  {
    return TARGET_XTT_TENSIX;
  }
  virtual unsigned execute (function *fn) override
  {
    check_function_type (DECL_SOURCE_LOCATION (fn->decl), TREE_TYPE (fn->decl), false);
    return check_early (fn);
  }
};

} // anon namespace

gimple_opt_pass *
make_pass_rvtt_check_early (gcc::context *ctxt)
{
  return new pass_rvtt_check_early (ctxt);
}

namespace {

const pass_data pass_data_rvtt_check_late =
{
  GIMPLE_PASS, /* type */
  "rvtt_check_late", /* name */
  OPTGROUP_NONE, /* optinfo_flags */
  TV_NONE, /* tv_id */
  PROP_ssa, /* properties_required */
  0, /* properties_provided */
  0, /* properties_destroyed */
  0, /* todo_flags_start */
  0, /* todo_flags_finish */
};

class pass_rvtt_check_late : public gimple_opt_pass
{
public:
  pass_rvtt_check_late (gcc::context *ctxt)
    : gimple_opt_pass (pass_data_rvtt_check_late, ctxt)
  {}

  virtual bool gate (function *) override
  {
    return TARGET_XTT_TENSIX;
  }
  virtual unsigned execute (function *fn) override
  {
    return check_late (fn);
  }
};

} // anon namespace

gimple_opt_pass *
make_pass_rvtt_check_late (gcc::context *ctxt)
{
  return new pass_rvtt_check_late (ctxt);
}
