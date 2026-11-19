/* Passes to deal with immediate operand generation and optimization
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

#define INCLUDE_VECTOR
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
#include "tree-ssa-propagate.h"
#include "gimple-iterator.h"
#include "gimple-pretty-print.h"
#include "tree-ssa.h"
#include "tree-into-ssa.h"
#include "diagnostic-core.h"
#include "rvtt.h"
#include <unordered_set>

static tree
emit_sfploadi (gimple_stmt_iterator &gsi, location_t loc, unsigned mod,
	       tree lv, tree addr, tree val, tree res)
{
  const auto *insnd = rvtt_get_insn_data (rvtt_insn_data::sfploadi) + bool (lv);

  if (!res)
    res = make_ssa_name (TREE_TYPE (TREE_TYPE (insnd->decl ())));

  auto *stmt = gimple_build_call (insnd->decl (), insnd->num_args ());
  gimple_set_location (stmt, loc);
  gimple_call_set_arg (stmt, 0, addr);
  if (lv)
    gimple_call_set_arg (stmt, 1, lv);
  gimple_call_set_arg (stmt, insnd->imm_arg (), val);
  gimple_call_set_arg (stmt, insnd->var_arg (), integer_zero_node);
  gimple_call_set_arg (stmt, insnd->id_arg (), integer_zero_node);
  gimple_call_set_arg (stmt, insnd->mod_arg (), build_int_cst (unsigned_type_node, mod));
  gimple_call_set_lhs (stmt, res);
  gsi_insert_before (&gsi, stmt, GSI_SAME_STMT);

  return res;
}

// For now do the best we can just here.  Later we'll want a combining pass to
// handle the cases we get here, AND those that become constant.  BITS < 0 is
// unsigned, BITS >=0 is position of sign bit.

static tree
emit_loadimm (gimple_stmt_iterator &gsi, location_t loc, int bits,
	      tree addr, tree val, tree res = nullptr)
{
  unsigned mod = SFPLOADI_MOD0_USHORT;
  bool needs_both = false;

  if (bits >= 0)
    {
      if (bits < 16)
	mod = SFPLOADI_MOD0_SHORT;
      else
	needs_both = true;
    }
  else if (bits < -16)
    needs_both = true;

  tree lower = val;
  if (!needs_both)
    ;
  else if (TREE_CODE (lower) != INTEGER_CST)
    {
      lower = make_ssa_name (TREE_TYPE (lower));
      auto *lower_stmt = gimple_build_assign
	(lower, BIT_AND_EXPR, val, build_int_cst (unsigned_type_node, 0xffff));
      gimple_set_location (lower_stmt, loc);
      gsi_insert_before (&gsi, lower_stmt, GSI_SAME_STMT);
    }
  else
    {
      uint32_t ival = TREE_INT_CST_LOW (lower);
      // We want to optimize the simple cases here, so we can detect when one's
      // trying to init the const regs in the immvar simplify pass
      if (!(ival & 0xffff))
	{
	  ival >>= 16;
	  mod = SFPLOADI_MOD0_FLOATB;
	  needs_both = false;
	}
      else if (!(ival >> 16))
	{
	  mod = SFPLOADI_MOD0_USHORT;
	  needs_both = false;
	}
      else if ((ival >> 15) == 0x1ffff)
	{
	  mod = SFPLOADI_MOD0_SHORT;
	  needs_both = false;
	}
      else
	val = build_int_cst (TREE_TYPE (val), ival >> 16);
      lower = build_int_cst (TREE_TYPE (lower), ival & 0xffff);
    }

  tree tmp = emit_sfploadi (gsi, loc, mod, nullptr, addr, lower,
			    needs_both ? nullptr : res);

  if (needs_both)
    {
      tree shift = val;
      if (TREE_CODE (shift) != INTEGER_CST)
	{
	  shift = make_ssa_name (TREE_TYPE (val));
	  auto *shift_stmt = gimple_build_assign
	    (shift, RSHIFT_EXPR, val, build_int_cst (unsigned_type_node, 16));
	  gimple_set_location (shift_stmt, loc);
	  gsi_insert_before (&gsi, shift_stmt, GSI_SAME_STMT);
	}

      res = emit_sfploadi (gsi, loc, SFPLOADI_MOD0_UPPER, tmp, addr, shift, res);
    }
  else
    res = tmp;

  return res;
}

static bool
emit_replacement (gimple_stmt_iterator &gsi, const rvtt_insn_data *insnd, gcall *call,
		  const rvtt_insn_data *new_insnd, tree imm, tree mod)
{
  gcc_assert (new_insnd->mod_info ().is_xmod ()
	      || (1u << TREE_INT_CST_LOW (mod)) & new_insnd->mod_info ().mod ());
  gimple *stmt = gimple_build_call (new_insnd->decl (), new_insnd->num_args ());
  gimple_set_location (stmt, gimple_location (call));
  gcc_checking_assert (new_insnd->num_srcs () == insnd->num_srcs () + 1
		       && insnd->num_imms () == 1
		       && !new_insnd->has_imms ());

  gimple_call_set_arg (stmt, new_insnd->src_arg (insnd->num_srcs ()), imm);
  gimple_call_set_arg (stmt, new_insnd->mod_arg (), mod);
  for (unsigned ix = insnd->num_srcs (); ix--; )
    gimple_call_set_arg (stmt, new_insnd->src_arg (ix),
			 gimple_call_arg (call, insnd->src_arg (ix)));
  for (unsigned ix = new_insnd->mod_arg () + 1, jx = insnd->id_arg () + 1,
	 limit = new_insnd->num_args (); ix != limit; ix++, jx++)
    gimple_call_set_arg (stmt, ix, gimple_call_arg (call, jx));

  gimple_call_set_lhs (stmt, gimple_call_lhs (call));
  gsi_insert_before (&gsi, stmt, GSI_SAME_STMT);

  // Caller will delete CALL
  return true;
}

// Expand immediate arguments, and simplify concat2/select2 chains

static bool
immvar_expand (gimple_stmt_iterator &gsi, const rvtt_insn_data *insnd, gcall *call)
{
  if (!insnd->has_imms ())
    return false;
  tree imm = gimple_call_arg (call, insnd->imm_arg ());
  tree addr = gimple_call_arg (call, 0);

  if (insnd->id == rvtt_insn_data::sfpxloadi)
    {
      int bits = TREE_INT_CST_LOW (gimple_call_arg (call, 4));
      emit_loadimm (gsi, gimple_location (call),
		    bits, addr, imm, gimple_call_lhs (call));
      return true;
    }

  if (!insnd->is_expanded ())
    return false;

  tree mod = insnd->has_mod () ? gimple_call_arg (call, insnd->mod_arg ()) : nullptr;
  bool expand = TREE_CODE (imm) != INTEGER_CST;
  auto info = insnd->ops[0];
  bool is_signed = info.kind () == rvtt_insn_data::op_t::SIGNED;
  if (!expand)
    {
      HOST_WIDE_INT i_imm = TREE_INT_CST_LOW (imm);
      HOST_WIDE_INT bound = (1u << info.bits ()) - 1;
      if (is_signed)
	{
	  bound >>= 1;
	  if (i_imm < ~bound)
	    expand = true;
	}
      else if (i_imm < 0)
	expand = true;
      if (i_imm > bound)
	expand = true;
    }

  if (expand)
    {
      tree tmp = emit_loadimm (gsi, gimple_location (call), is_signed ? -32 : 32, addr, imm, nullptr);
      return emit_replacement (gsi, insnd, call, insnd->get_vector (), tmp, mod);
    }

  return false;
}

static void
update_sums (std::unordered_set<gphi *> &phis, int num_imms, unsigned non_const_mask, tree sum_var)
{
  if (!SSA_VAR_P (sum_var))
    return;

  auto sum_stmt = SSA_NAME_DEF_STMT (sum_var);
  if (auto phi = dyn_cast <gphi *> (sum_stmt))
    {
      if (!phis.insert (phi).second)
	return;

      use_operand_p arg_p;
      ssa_op_iter ix;
      FOR_EACH_PHI_ARG (arg_p, phi, ix, SSA_OP_USE)
	update_sums (phis, num_imms, non_const_mask, USE_FROM_PTR (arg_p));
      return;
    }

  gcc_assert (rvtt_get_insn_data (sum_stmt)->id
	      < rvtt_insn_data::synth_opcode + rvtt_insn_data::imm_arg_hwm
	      && rvtt_get_insn_data (sum_stmt)->num_args () == num_imms + 1);
  for (unsigned ix = num_imms; ix--;)
    if (!((1 << ix) & non_const_mask))
      gimple_call_set_arg (sum_stmt, ix + 1, integer_zero_node);
  update_stmt (sum_stmt);
}

static bool
immvar_gather (std::unordered_set<gphi *> &phis,
	       const rvtt_insn_data *insnd,
	       gcall *call, std::vector<gcall *> *loads)
{
  if (!insnd->has_iptr ())
    return false;

  int num_imms = insnd->num_imms () ? insnd->num_imms () : 1;
  unsigned non_const_mask = 0;
  for (int ix = num_imms; ix--; )
    {
      tree imm = gimple_call_arg (call, insnd->imm_arg (ix));
      if (TREE_CODE (imm) != INTEGER_CST)
	{
	  non_const_mask |= 1 << ix;
	  if (loads && integer_zerop (gimple_call_arg (call, 0)))
	    error_at (gimple_location (call), "%qE immediate operand %d is not known at compile time",
		      gimple_call_fndecl (call), 1 + insnd->imm_arg (ix));
	}
    }

  if (non_const_mask)
    {
      // Not everything is const
      if (non_const_mask != (1u << num_imms) - 1)
	{
	  // But something is const
	  update_sums (phis, num_imms, non_const_mask, 
		       gimple_call_arg (call, insnd->var_arg ()));
	  phis.clear ();
	}
      return false;
    }

  // Everything is const
  bool changed = false;
  if (!integer_zerop (gimple_call_arg (call, 0)))
    {
      // And that wasn't true last time
      if (dump_file)
	{
	  fprintf (dump_file, "Immediates are now known: ");
	  print_gimple_stmt (dump_file, call, 0);
	}
      gimple_call_set_arg (call, 0, null_pointer_node);
      if (insnd->has_imms ())
	{
	  gimple_call_set_arg (call, insnd->var_arg (), integer_zero_node);
	  gimple_call_set_arg (call, insnd->id_arg (), integer_zero_node);
	}
      update_stmt (call);
      changed = true;
    }

  if (loads && insnd->id == rvtt_insn_data::sfploadi)
    // We've not done LV optimizing yet, so we don't have to capture sfploadi_lv
    loads->push_back (call);

  return changed;
}

static bool immvar_gather (function *fn, std::vector<gcall *> *loads)
{
    bool changed = false;
    basic_block bb;
    std::unordered_set<gphi *> phis;

    FOR_EACH_BB_FN (bb, fn)
      for (gimple_stmt_iterator gsi = gsi_start_bb (bb);
	   !gsi_end_p (gsi); gsi_next (&gsi))
	if (auto *insnd = rvtt_get_insn_data (*gsi))
	  if (immvar_gather (phis, insnd, as_a <gcall *> (*gsi), loads))
	    changed = true;

    return changed;
}

static void
replace_loadi (gcall *call, gcall *earlier, int op, tree val,
	       gcall *extra_call = nullptr, const rvtt_insn_data *extra_insnd = nullptr)
{
  const auto *reg_insnd = !val || extra_call ? rvtt_get_insn_data (rvtt_insn_data::sfpreadlreg) : nullptr;
  const auto *other_insnd = !val ? nullptr :
    extra_insnd ? extra_insnd : rvtt_get_insn_data (rvtt_insn_data::sfploadi);
  const auto *new_insnd = reg_insnd ? reg_insnd : other_insnd;

  gimple *new_call = gimple_build_call (new_insnd->decl (), new_insnd->num_args ());
  gimple_set_location (new_call, gimple_location (call));
  gimple_call_set_lhs (new_call,
		       extra_call ? gimple_call_arg (extra_call, 1) : gimple_call_lhs (call));
  if (val)
    {
      auto *imm_call = extra_call ? extra_call : new_call;
      gimple_call_set_arg (imm_call, 0, null_pointer_node);
      gimple_call_set_arg (imm_call, other_insnd->imm_arg (), val);
      gimple_call_set_arg (imm_call, other_insnd->var_arg (), integer_zero_node);
      gimple_call_set_arg (imm_call, other_insnd->id_arg (), integer_zero_node);
    }
  gimple_call_set_arg (new_call, val && !extra_call ? new_insnd->mod_arg () : 0,
		       build_int_cst (unsigned_type_node, op));

  if (dump_file)
    {
      fprintf (dump_file, "\nReplacing\n");
      if (earlier)
	print_gimple_stmt (dump_file, earlier, 2);
      print_gimple_stmt (dump_file, call, 2);
      fprintf (dump_file, "with\n");
      print_gimple_stmt (dump_file, new_call, 2);
      if (extra_call)
	print_gimple_stmt (dump_file, extra_call, 2);
    }

  auto gsi = gsi_for_stmt (call);
  gsi_insert_before (&gsi, new_call, GSI_SAME_STMT);
  if (extra_call)
    gsi_insert_before (&gsi, extra_call, GSI_SAME_STMT);
  unlink_stmt_vdef (call);
  gsi_remove (&gsi, true);
}

// CALL is an sfploadi call, can we simplify it (and the possibly-following
// sfploadi_lv?
// For the two-loadi case, the first one is SFPLOADI_MOD0_USHORT and the second
// is SFPLOADI_MOD0_UPPER.

static bool
immvar_simplify (gcall *call, std::vector<gcall *> seconds)
{
  const auto *insnd = rvtt_get_insn_data (rvtt_insn_data::sfploadi);

  unsigned first_mod = TREE_INT_CST_LOW (gimple_call_arg (call, insnd->mod_arg ()));
  tree first_val = gimple_call_arg (call, insnd->imm_arg ());
  uint32_t first_ival = TREE_INT_CST_LOW (first_val);

  tree res = gimple_call_lhs (call);
  if (!res)
    return false;

  int sfp_use = 0;
  use_operand_p use_p;
  imm_use_iterator iter;
  int second_mod = first_mod == SFPLOADI_MOD0_USHORT ? SFPLOADI_MOD0_UPPER
    : first_mod == SFPLOADI_MOD0_FLOATB ? SFPLOADI_MOD0_LOWER
    : -1;

  seconds.clear ();
  FOR_EACH_IMM_USE_FAST (use_p, iter, res)
    {
      gimple *use_stmt = USE_STMT (use_p);
      if (is_gimple_debug (use_stmt))
	continue;
      auto *use_insnd = rvtt_get_insn_data (use_stmt);
      if (!use_insnd)
	continue;
      auto *use_call = as_a <gcall *> (use_stmt);

      if (second_mod >= 0
	  && use_insnd->id == rvtt_insn_data::sfploadi_lv
	  && integer_zerop (gimple_call_arg (use_call, 0))
	  && unsigned (second_mod)
	  == TREE_INT_CST_LOW (gimple_call_arg (use_call, use_insnd->mod_arg ())))
	{
	  seconds.push_back (use_call);
	  continue;
	}

      sfp_use |= 1;
      if (use_insnd->id == rvtt_insn_data::sfpwriteconfig_v
	  && HAVE_CREG_NEG_1
	  && (TREE_INT_CST_LOW (gimple_call_arg (use_call, use_insnd->mod_arg () + 1))
	      == CREG_IDX_NEG_1))
	// We must not simplify this to a load of the constant register
	// that we're intializing!
	sfp_use |= 2;
    }

  bool changed = false;
  for (auto *second : seconds)
    {
      tree second_val = gimple_call_arg (second, insnd[1].imm_arg ());
      uint32_t second_ival = TREE_INT_CST_LOW (second_val);
      uint32_t full_ival
	= (first_ival << (second_mod == SFPLOADI_MOD0_UPPER ? 0 : 16))
	| (second_ival << (second_mod == SFPLOADI_MOD0_LOWER ? 0 : 16));
      bool neg = full_ival >> 31;
      int op = -1;
      gcall *extra_call = nullptr;
      const rvtt_insn_data *extra_insnd = nullptr;

      if (!full_ival)
	{
	  // 0.0f
	  op = CREG_IDX_0;
	  second_val = nullptr;
	  goto replace;
	}

      if (full_ival << 1 == 0x3f800000 << 1)
	{
	  // +1.0f or -1.0f
	  if (!neg || HAVE_CREG_NEG_1)
	    {
	      op = neg ? CREG_IDX_NEG_1 : CREG_IDX_1;
	      second_val = nullptr;
	      goto replace;
	    }
	}

      if (!(full_ival & 0xffff))
	{
	  // fp16b
	  op = SFPLOADI_MOD0_FLOATB;
	  if (second_mod == SFPLOADI_MOD0_LOWER)
	    second_val = first_val;
	  goto replace;
	}

      if (!(full_ival >> 16))
	{
	  // u16
	  op = SFPLOADI_MOD0_USHORT;
	  if (second_mod == SFPLOADI_MOD0_UPPER)
	    second_val = first_val;
	  goto replace;
	}

      if (!~(full_ival | 0x7fff))
	{
	  // i16
	  op = SFPLOADI_MOD0_SHORT;
	  if (second_mod == SFPLOADI_MOD0_UPPER)
	    second_val = first_val;
	  goto replace;
	}

      {
	// Better as a float16a?
	// FLOATA=SGN:1,EXP:5,MAN:10, bias 15
	// FP32=SGN:1,EXP:8,MAN:23, bias 127
	// Stay away from denorms & nans/infs
	unsigned exp = (full_ival >> 23) & 0xff;
	if ((full_ival & 0x00001fff) == 0
	    && exp > (127 - 15) && exp < (127 - 15) + 31)
	  {
	    // Can be packed to an fp16a
	    uint32_t man = full_ival & 0x7fffff;
	    uint32_t floata
	      = (man >> 13)
	      | ((exp - (127 - 15)) << 10)
	      | (uint32_t (neg) << 15);
	    op = SFPLOADI_MOD0_FLOATA;
	    second_val = build_int_cst (unsigned_type_node, floata);
	    goto replace;
	  }
      }

      {
	// Is it related to one of the known constants?
	static const struct {
	  uint32_t val;
	  unsigned reg;
	} csts[] = {
	  {0x3f800000, CREG_IDX_1}, // +1.0f 0x1.0p0f
	  {0xbf800000, CREG_IDX_NEG_1}, // -1.0f -0x1.0p0f
	  {0x3f56594b, CREG_IDX_0P837300003},// 0.8373 0x0.d6594bp0f
	};
	constexpr unsigned sfpiadd_i_bits = 12;

	unsigned top_zeroes = 0, bot_zeroes = 0;
	if (TARGET_XTT_TENSIX_BH_QSR)
	  {
	    top_zeroes = clz_hwi (neg ? ~full_ival : full_ival);
	    bot_zeroes = ctz_hwi (full_ival);
	  }

	for (auto const &cst : csts) {
	  if (cst.reg == CREG_IDX_NEG_1 && !HAVE_CREG_NEG_1)
	    continue;
	  uint32_t delta = full_ival - cst.val;
	  unsigned imod = 0;
	  if (delta + (1u << (sfpiadd_i_bits - 1)) < (1u << sfpiadd_i_bits))
	    {
	      // known constant + delta
	      second_val = build_int_cst (unsigned_type_node, delta);
	      extra_insnd = rvtt_get_insn_data (rvtt_insn_data::sfpiadd_i);
	      imod = SFPIADD_MOD1_CC_NONE;

	    replace_pair:
	      op = cst.reg;
	      if (!delta)
		second_val = nullptr;
	      else
		{
		  extra_call = gimple_build_call (extra_insnd->decl (), extra_insnd->num_args ());

		  gimple_set_location (extra_call, gimple_location (second));
		  gimple_call_set_arg (extra_call, 1,
				       make_ssa_name (TREE_TYPE (gimple_call_lhs (call))));
		  gimple_call_set_lhs (extra_call, gimple_call_lhs (second));
		  gimple_call_set_arg (extra_call, extra_insnd->mod_arg (),
				       build_int_cst (unsigned_type_node, imod));
		}
	      goto replace;
	    }
	  if (TARGET_XTT_TENSIX_BH_QSR)
	    {
	      // We could look for shift opportunities sfpshft_i
	      unsigned cst_zeroes = clz_hwi (neg ? ~cst.val : cst.val);
	      int shift = top_zeroes - cst_zeroes;
	      if (shift > 0 && (neg ? int32_t (cst.val) >> shift : cst.val >> shift) == full_ival)
		{
		  shift = -shift;
		  if (neg)
		    imod |= SFPSHFT_MOD1_ARITHMETIC;
		  // sfpshft_i
		replace_shft:
		  second_val = build_int_cst (integer_type_node, shift);
		  extra_insnd = rvtt_get_insn_data (rvtt_insn_data::sfpshft_i);
		  goto replace_pair;
		}

	      cst_zeroes = ctz_hwi (cst.val);
	      shift = bot_zeroes - cst_zeroes;
	      if (shift > 0 && cst.val << shift == full_ival)
		goto replace_shft;
	    }
	}
      }

      // Not simplifiable
      sfp_use |= 1;
      continue;

    replace:
      replace_loadi (second, call, op, second_val, extra_call, extra_insnd);
      changed = true;
    }

  if (sfp_use == 1)
    {
      // The first loadi has other uses, can we simplify it itself?
      int op = -1;

      if (!first_ival)
	op = CREG_IDX_0;
      else if (int (first_ival & 0x7fff)
	       == (first_mod == SFPLOADI_MOD0_FLOATB ? 0x3f80
		   : first_mod == SFPLOADI_MOD0_FLOATA ? 0x3c00
		   : ~0))
	{
	  if (!(first_ival & 0x8000) || HAVE_CREG_NEG_1)
	    op = first_ival & 0x8000 ? CREG_IDX_NEG_1 : CREG_IDX_1;
	}

      if (op >= 0)
	{
	  replace_loadi (call, nullptr, op, nullptr);
	  changed = true;
	}
    }

  return changed;
}

// CALL has a SCALAR variant, if its last op is from a LOADI and the value
// being loaded fits in the immediate slot, make it so.
// Also consider commuting sfpiadd_v's operands (and maybe negating)

static gcall *
immload_combine (gimple_stmt_iterator gsi, const rvtt_insn_data *call_insnd,
		 gcall *call, const rvtt_insn_data *scalar_insnd)
{
  gcc_checking_assert (call_insnd->is_live () == scalar_insnd->is_live ());

  auto nlv_call_insnd = call_insnd->get_non_live ();
  int mod = TREE_INT_CST_LOW (gimple_call_arg (call, call_insnd->mod_arg ()));
  auto info = scalar_insnd->ops[0];
  bool is_signed = info.kind () == rvtt_insn_data::op_t::SIGNED;
  int bits = info.bits () - int (is_signed);
  rvtt_arg_info arg_info;
  int keep_arg = call_insnd->src_arg ();
  int32_t cst;

  // For sfpiadd_v try the first operand, do this first
  if (nlv_call_insnd->id == rvtt_insn_data::sfpiadd_v)
    {
      arg_info = gimple_call_arg (call, call_insnd->src_arg ());
      if (arg_info.is_cst ())
	{
	  cst = int32_t (arg_info.get_cst ());
	  if (mod & SFPIADD_MOD1_ARG_2SCOMP_LREG_DST)
	    cst = -cst;
	  auto upper = cst >> bits;
	  if (!(upper && (!is_signed || upper != -1)))
	    {
	      mod &= ~SFPIADD_MOD1_ARG_2SCOMP_LREG_DST;
	      keep_arg++;
	      goto iadd_commute;
	    }
	}
    }

  // Try the last operand
  if (scalar_insnd->mod_info ().mod () & (1u << mod))
    {
      arg_info = gimple_call_arg
	(call, keep_arg + call_insnd->num_srcs () - 1);
      if (!arg_info.is_cst ())
	return nullptr;
      cst = int32_t (arg_info.get_cst ());

      if (scalar_insnd->id == rvtt_insn_data::sfpwriteconfig_i)
	{
	  // Extra constraints for sfpwriteconfig. Some registers will get a
	  // hard-wired number.
	  unsigned reg = TREE_INT_CST_LOW
	    (gimple_call_arg (call, call_insnd->mod_arg () + 1));
	  if (reg >= SFPCONFIG_IMM_FIXED_CREG_LWM
	      && reg <= SFPCONFIG_IMM_FIXED_CREG_HWM)
	    {
	      // Unless we're setting to the fixed constant, fail.  Only check
	      // this particular constant.
	      if (!(reg == SFPCONFIG_IMM_FIXED_CREG_LWM
		    && cst == int32_t (0xbf800000)))
		return nullptr;
	      cst = 0;
	    }
	}
      auto upper = cst >> bits;
      if (upper && (!is_signed || upper != -1))
	return nullptr;
    }
  else
    return nullptr;

 iadd_commute:;
  if (dump_file)
    {
      fprintf (dump_file, "Combining:\n");
      print_gimple_stmt (dump_file, arg_info.get_def (), 2);
      print_gimple_stmt (dump_file, call, 2);
    }

  // Replace the CALL with one of SCALAR
  gimple *new_call = gimple_build_call (scalar_insnd->decl (), scalar_insnd->num_args ());
  gimple_set_location (new_call, gimple_location (call));
  gimple_call_set_lhs (new_call, gimple_call_lhs (call));
  unsigned argno = 0;
  gimple_call_set_arg (new_call, argno++, null_pointer_node);
  if (scalar_insnd->is_live ())
    gimple_call_set_arg (new_call, argno++, gimple_call_arg (call, 0));
  if (scalar_insnd->num_srcs ())
    gimple_call_set_arg (new_call, argno++, gimple_call_arg (call, keep_arg));

  gimple_call_set_arg (new_call, argno++, build_int_cst (is_signed ? integer_type_node : unsigned_type_node,
							 cst));
  gimple_call_set_arg (new_call, argno++, integer_zero_node);
  gimple_call_set_arg (new_call, argno++, integer_zero_node);

  gimple_call_set_arg (new_call, argno++, build_int_cst (unsigned_type_node, mod));

  // Copy remaining args
  for (unsigned limit = scalar_insnd->num_args (); argno != limit; argno++)
    gimple_call_set_arg (new_call, argno, gimple_call_arg (call, argno - 3));

  gsi_insert_before (&gsi, new_call, GSI_SAME_STMT);

  if (dump_file)
    {
      fprintf (dump_file, "to:\n");
      print_gimple_stmt (dump_file, new_call, 2);
    }

  return arg_info.get_def ();
}

namespace {

const pass_data pass_data_rvtt_immvar_expand =
{
  GIMPLE_PASS, /* type */
  "rvtt_immvar_expand", /* name */
  OPTGROUP_NONE, /* optinfo_flags */
  TV_NONE, /* tv_id */
  PROP_ssa, /* properties_required */
  0, /* properties_provided */
  0, /* properties_destroyed */
  0, /* todo_flags_start */
  0, /* todo_flags_finish */
};

class pass_rvtt_immvar_expand : public gimple_opt_pass
{
public:
  pass_rvtt_immvar_expand (gcc::context *ctxt)
    : gimple_opt_pass (pass_data_rvtt_immvar_expand, ctxt)
  {}

  virtual bool gate (function *) override
  {
    return TARGET_XTT_TENSIX;
  }
  virtual unsigned execute (function *fn) override
  {
    bool changed = false;
    basic_block bb;
    FOR_EACH_BB_FN (bb, fn)
      for (gimple_stmt_iterator gsi = gsi_start_bb (bb);
	   !gsi_end_p (gsi);)
	{
	  if (auto *insnd = rvtt_get_insn_data (*gsi))
	    {
	      if (immvar_expand (gsi, insnd, as_a <gcall *> (*gsi)))
		{
		  unlink_stmt_vdef (*gsi);
		  gsi_remove (&gsi, true);
		  changed = true;
		  continue;
		}
	      else if (insnd->id == rvtt_insn_data::sfpselect2)
		{
		  auto *select_call = as_a <gcall *> (*gsi);
		  tree concat_vec = gimple_call_arg (select_call, 0);
		  auto concat_stmt = SSA_NAME_DEF_STMT (concat_vec);
		  auto *concat_insnd = rvtt_get_insn_data (concat_stmt);
		  if (concat_insnd && concat_insnd->id == rvtt_insn_data::sfpconcat2)
		    {
		      // Propagate the appropriate concat's arg to users of the
		      // select_call's result.
		      unsigned argno = TREE_INT_CST_LOW (gimple_call_arg (select_call, 1));
		      auto incoming_vec = gimple_call_arg (concat_stmt, argno);
		      auto select_result = gimple_call_lhs (select_call);

		      if (dump_file)
			{
			  fprintf (dump_file, "Propagating concat/select:\n");
			  print_gimple_stmt (dump_file, concat_stmt, 0);
			  print_gimple_stmt (dump_file, select_call, 0);
			}
		      rvtt_substitute_value (select_result, incoming_vec);

		      // Delete the select
		      unlink_stmt_vdef (*gsi);
		      gsi_remove (&gsi, true);

		      // If there are no remaining uses of the concat result delete
		      // it.
		      if (has_zero_uses (concat_vec))
			{
			  if (dump_file)
			    fprintf (dump_file, "Deleting concat\n");
			  unlink_stmt_vdef (concat_stmt);
			  auto concat_gsi = gsi_for_stmt (concat_stmt);
			  gsi_remove (&concat_gsi, true);
			}
		      changed = true;
		      continue;
		    }
		}
	    }
	  gsi_next (&gsi);
	}

    return changed ? TODO_update_ssa : 0;
  }
};

} // anon namespace

gimple_opt_pass *
make_pass_rvtt_immvar_expand (gcc::context *ctxt)
{
  return new pass_rvtt_immvar_expand (ctxt);
}

namespace {

const pass_data pass_data_rvtt_immload_shorten =
{
  GIMPLE_PASS, /* type */
  "rvtt_immload_shorten", /* name */
  OPTGROUP_NONE, /* optinfo_flags */
  TV_NONE, /* tv_id */
  PROP_ssa, /* properties_required */
  0, /* properties_provided */
  0, /* properties_destroyed */
  0, /* todo_flags_start */
  0, /* todo_flags_finish */
};

class pass_rvtt_immload_shorten : public gimple_opt_pass
{
public:
  pass_rvtt_immload_shorten (gcc::context *ctxt)
    : gimple_opt_pass (pass_data_rvtt_immload_shorten, ctxt)
  {}

  virtual bool gate (function *) override
  {
    return TARGET_XTT_TENSIX;
  }
  virtual unsigned execute (function *fn) override
  {
    // (a) remove var and id from now-constant immvals
    // (b) optimize sfploadi{,_lv} sequences
    std::vector<gcall *> loads;
    std::vector<gcall *> uppers;

    bool changed = immvar_gather (fn, &loads);
    for (auto *call : loads)
      if (immvar_simplify (call, uppers))
	changed = true;

    return changed ? TODO_update_ssa : 0;
  }
};

const pass_data pass_data_rvtt_iptr_anull =
{
  GIMPLE_PASS, /* type */
  "rvtt_iptr_anull", /* name */
  OPTGROUP_NONE, /* optinfo_flags */
  TV_NONE, /* tv_id */
  PROP_ssa, /* properties_required */
  0, /* properties_provided */
  0, /* properties_destroyed */
  0, /* todo_flags_start */
  0, /* todo_flags_finish */
};

class pass_rvtt_iptr_anull : public gimple_opt_pass
{
public:
  pass_rvtt_iptr_anull (gcc::context *ctxt)
    : gimple_opt_pass (pass_data_rvtt_iptr_anull, ctxt)
  {}

  virtual bool gate (function *) override
  {
    return TARGET_XTT_TENSIX;
  }
  virtual unsigned execute (function *fn) override
  {
    bool changed = immvar_gather (fn, nullptr);
    return changed ? TODO_update_ssa : 0;
  }
};

} // anon namespace

gimple_opt_pass *
make_pass_rvtt_immload_shorten (gcc::context *ctxt)
{
  return new pass_rvtt_immload_shorten (ctxt);
}

gimple_opt_pass *
make_pass_rvtt_iptr_anull (gcc::context *ctxt)
{
  return new pass_rvtt_iptr_anull (ctxt);
}

namespace {

const pass_data pass_data_rvtt_immload_combine =
{
  GIMPLE_PASS, /* type */
  "rvtt_immload_combine", /* name */
  OPTGROUP_NONE, /* optinfo_flags */
  TV_NONE, /* tv_id */
  PROP_ssa, /* properties_required */
  0, /* properties_provided */
  0, /* properties_destroyed */
  0, /* todo_flags_start */
  0, /* todo_flags_finish */
};

class pass_rvtt_immload_combine : public gimple_opt_pass
{
public:
  pass_rvtt_immload_combine (gcc::context *ctxt)
    : gimple_opt_pass (pass_data_rvtt_immload_combine, ctxt)
  {}

  virtual bool gate (function *) override
  {
    return TARGET_XTT_TENSIX;
  }
  virtual unsigned execute (function *fn) override
  {
    // See if insns with a scalar variant can use that instead of an input loadi
    bool changed = false;
    basic_block bb;

    FOR_EACH_BB_FN (bb, fn)
      for (gimple_stmt_iterator gsi = gsi_start_bb (bb);
	   !gsi_end_p (gsi);)
	{
	  if (auto *insnd = rvtt_get_insn_data (*gsi))
	    if (auto *scalar = insnd->get_scalar ())
	      if (auto *def_call
		  = immload_combine (gsi, insnd, as_a <gcall *> (*gsi), scalar))
		{
		  unlink_stmt_vdef (*gsi);
		  gsi_remove (&gsi, true);
		  // We run independet of DCE, so remove the defining insn if
		  // it has no other uses.
		  if (has_zero_uses (gimple_call_lhs (def_call)))
		    {
		      if (dump_file)
			{
			  fprintf (dump_file, "deleting:\n");
			  print_gimple_stmt (dump_file, def_call, 2);
			}
		      auto gsi = gsi_for_stmt (def_call);
		      gsi_remove (&gsi, true);
		    }
		  if (dump_file)
		    fprintf (dump_file, "\n");
		  changed = true;
		  continue;
		}

	  gsi_next (&gsi);
	}

    return changed ? TODO_update_ssa : 0;
  }
};

} // anon namespace

gimple_opt_pass *
make_pass_rvtt_immload_combine (gcc::context *ctxt)
{
  return new pass_rvtt_immload_combine (ctxt);
}
