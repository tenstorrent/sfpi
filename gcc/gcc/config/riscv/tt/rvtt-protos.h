/* TT .md file fn prototypes, etc
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

#ifndef GCC_RISCV_SFPU_PROTOS_H
#define GCC_RISCV_SFPU_PROTOS_H

#include "sfpu-ops-wh.h"
#include "sfpu-ops-bh.h"
#include "sfpu-ops-qsr.h"

extern void rvtt_mov_error (const rtx_insn *) ATTRIBUTE_COLD;
extern rtx rvtt_gen_rtx_creg (machine_mode, unsigned sfpu_regno);
extern rtx rvtt_gen_rtx_noval (machine_mode);
extern bool rvtt_merge_lv_src (rtx *lv, rtx *src, rtx *commute = nullptr);

extern void rvtt_substitute_value (tree orig, tree replacement);

// Instruction synthesis
class rvtt_synth
{
 private:
  constexpr static unsigned REG_SHIFT_BITS = 5;
  constexpr static unsigned ID_BITS = 32 - REG_SHIFT_BITS * 2;

  unsigned encode = 0;

 public:
  enum RVTT_SYNTH_OFFSETS {
    IX_mem,     // Memory operand (or zero)
    IX_opcode,  // Opcode (or zero)
    IX_encode,  // Encoded ID & src/dst shifts (or zero)
    IX_insn,    // Instruction (or zero)
    IX_src,     // Src value (or noval)
    IX_lv,      // Live value (if inside SET)
  };

 public:
  rvtt_synth (unsigned HOST_WIDE_INT val)
    : encode (unsigned (val)) {}

  // Extract encode, int is correct here btw
  operator int () const { return encode; }

  // Generate pattern
  static const char *pattern (unsigned is_synthed, const char *tmpl,
			      rtx operands[], bool is_set, int IX_tmp = -1);

  // accessors
  unsigned id () const {
    return encode & ((1u << ID_BITS) - 1u);
  }
  unsigned dst_shift () const {
    return (encode >> ID_BITS)
      & ((1u << REG_SHIFT_BITS) - 1u);
  }
  unsigned src_shift () const {
    return (encode >> (ID_BITS + REG_SHIFT_BITS))
      & ((1u << REG_SHIFT_BITS) - 1u);
  }

  // setters
  auto &dst_shift (unsigned shift) {
    encode |= shift << ID_BITS;
    return *this;
  }
  auto &src_shift (unsigned shift) {
    encode |= shift << (ID_BITS + REG_SHIFT_BITS);
    return *this;
  }
};

class rvtt_arg_info
{
  tree arg;
  gcall *def = nullptr; // cst def stmt
  uint32_t cst = 0; // scalar const

 public:
  rvtt_arg_info () = default;
  rvtt_arg_info (const rvtt_arg_info &) = default;
  rvtt_arg_info (tree arg, bool only_zeroness = false);

public:
  tree get_arg () const { return arg; }
  tree set_arg (tree a) { arg = a; return arg; }

  gcall *get_def () const { return def; }
  bool is_cst () const { return bool (def); }

  uint32_t get_cst () const { return cst; };
  bool is_zero () const { return is_cst () && !get_cst (); }

};

extern bool rvtt_hll_p (rtx pat);
extern bool rvtt_l1_load_p (rtx pat);
extern bool rvtt_reg_load_p (rtx pat);

// Gimple passes
class gimple_opt_pass;
extern gimple_opt_pass *make_pass_rvtt_attrib (gcc::context *ctxt);
extern gimple_opt_pass *make_pass_rvtt_cc (gcc::context *ctxt);
extern gimple_opt_pass *make_pass_rvtt_combine (gcc::context *ctxt);
extern gimple_opt_pass *make_pass_rvtt_check_early (gcc::context *ctxt);
extern gimple_opt_pass *make_pass_rvtt_check_late (gcc::context *ctxt);
extern gimple_opt_pass *make_pass_rvtt_dce (gcc::context *ctxt);
extern gimple_opt_pass *make_pass_rvtt_immload_combine (gcc::context *ctxt);
extern gimple_opt_pass *make_pass_rvtt_immload_shorten (gcc::context *ctxt);
extern gimple_opt_pass *make_pass_rvtt_immvar_expand (gcc::context *ctxt);
extern gimple_opt_pass *make_pass_rvtt_iptr_anull (gcc::context *ctxt);
extern gimple_opt_pass *make_pass_rvtt_noval_elide (gcc::context *ctxt);
extern gimple_opt_pass *make_pass_rvtt_live (gcc::context *ctxt);
extern gimple_opt_pass *make_pass_rvtt_schedule_ssa (gcc::context *ctxt);
extern gimple_opt_pass *make_pass_rvtt_synth_cse (gcc::context *ctxt);
extern gimple_opt_pass *make_pass_rvtt_synth_renumber (gcc::context *ctxt);
extern gimple_opt_pass *make_pass_rvtt_synth_split (gcc::context *ctxt);
extern gimple_opt_pass *make_pass_rvtt_unspec_prop_ssa (gcc::context *ctxt);
extern gimple_opt_pass *make_pass_rvtt_vif (gcc::context *ctxt);

// RTL passes
class rtl_opt_pass;
extern rtl_opt_pass *make_pass_rvtt_fix_ebreak (gcc::context *ctxt);
extern rtl_opt_pass *make_pass_rvtt_fix_raw (gcc::context *ctxt);
extern rtl_opt_pass *make_pass_rvtt_fix_rocc (gcc::context *ctxt);
extern rtl_opt_pass *make_pass_rvtt_hll (gcc::context *ctxt);
extern rtl_opt_pass *make_pass_rvtt_replay (gcc::context *ctxt);
extern rtl_opt_pass *make_pass_rvtt_rmext (gcc::context *ctxt);
extern rtl_opt_pass *make_pass_rvtt_schedule (gcc::context *ctxt);
extern rtl_opt_pass *make_pass_rvtt_synth_opcode (gcc::context *ctxt);
extern rtl_opt_pass *make_pass_rvtt_unspec_prop_rtl (gcc::context *ctxt);

constexpr unsigned int SFPMAD_MOD1_OFFSET_NONE = 0;
// A * B + C
constexpr unsigned int SFPMAD_MOD1_BH_COMPL_A = 1; // negate A operand
constexpr unsigned int SFPMAD_MOD1_BH_COMPL_C = 2; // negate C operand

constexpr unsigned int SFPMOV_MOD1_NONE = 0;
constexpr unsigned int SFPMOV_MOD1_COMPL = 1; // negate
constexpr unsigned int SFPMOV_MOD1_ALL = 2; // copy all lanes
constexpr unsigned int SFPMOV_MOD1_CFG = 8; // read cfg register

constexpr unsigned int SFPLOADI_MOD0_FLOATB = 0;
constexpr unsigned int SFPLOADI_MOD0_FLOATA = 1;
constexpr unsigned int SFPLOADI_MOD0_USHORT = 2;
constexpr unsigned int SFPLOADI_MOD0_SHORT = 4;
constexpr unsigned int SFPLOADI_MOD0_UPPER = 8;
constexpr unsigned int SFPLOADI_MOD0_LOWER = 10;

constexpr unsigned int SFPLOADSTORE_ADDR_MODE_WH_NOINC = 3;
constexpr unsigned int SFPLOADSTORE_ADDR_MODE_NOINC = 7;
constexpr unsigned int SFPLOADSTORE_MOD0_FMT_COPY_MASK = 0x18; // FP32 or FPINT32

constexpr unsigned int SFPEXEXP_MOD1_DEBIAS = 0;
constexpr unsigned int SFPEXEXP_MOD1_NODEBIAS = 1;
constexpr unsigned int SFPEXEXP_MOD1_SET_CC_SGN_EXP = 2;
constexpr unsigned int SFPEXEXP_MOD1_SET_CC_COMP_EXP = 8;
constexpr unsigned int SFPEXEXP_MOD1_SET_CC_SGN_COMP_EXP = 10;

constexpr unsigned int SFPSETMAN_MOD1_LREG = 0;
constexpr unsigned int SFPSETMAN_MOD1_IMM = 1;
constexpr unsigned int SFPSETEXP_MOD1_LREG = 0;
constexpr unsigned int SFPSETEXP_MOD1_IMM = 1;
constexpr unsigned int SFPSETEXP_MOD1_LREG_CPY = 2;
constexpr unsigned int SFPSETSGN_MOD1_LREG = 0;
constexpr unsigned int SFPSETSGN_MOD1_IMM = 1;

constexpr unsigned int SFPSETCC_MOD1_LREG_LT0 = 0;
constexpr unsigned int SFPSETCC_MOD1_IMM_BIT0 = 1;
constexpr unsigned int SFPSETCC_MOD1_LREG_NE0 = 2;
constexpr unsigned int SFPSETCC_MOD1_LREG_GTE0 = 4;
constexpr unsigned int SFPSETCC_MOD1_LREG_EQ0 = 6;
constexpr unsigned int SFPSETCC_MOD1_COMP = 8;

constexpr unsigned int SFPSETCC_IMM_TYPE_INT = 0;
constexpr unsigned int SFPSETCC_IMM_TYPE_FLOAT = 1;
constexpr unsigned int SFPSETCC_IMM_TYPE_SMAG = 1;

// EU: enable unmodified, EC: complement, EI: immediate
// R1: result set, RI: immediate
constexpr unsigned int SFPENCC_IMM12_NEITHER = 0;   // Imm value to clear both enable/result
constexpr unsigned int SFPENCC_IMM12_BOTH = 3;      // Imm value to set both enable/result

constexpr unsigned int SFPENCC_MOD1_EU_R1 = 0;
constexpr unsigned int SFPENCC_MOD1_EC_R1 = 1;
constexpr unsigned int SFPENCC_MOD1_EI_R1 = 2;
constexpr unsigned int SFPENCC_MOD1_EU_RI = 8;
constexpr unsigned int SFPENCC_MOD1_EC_RI = 9;
constexpr unsigned int SFPENCC_MOD1_EI_RI = 10;

constexpr unsigned int SFPPUSHCC_MOD1_PUSH = 0;
constexpr unsigned int SFPPUSHCC_MOD1_REPLACE = 1;

constexpr unsigned int SFPPOPCC_MOD1_POP = 0;

constexpr unsigned int SFPCONFIG_MOD1_ARG_IMM = 1;
constexpr unsigned int SFPCONFIG_MOD1_AND = 2;
constexpr unsigned int SFPCONFIG_MOD1_OR = 4;
constexpr unsigned int SFPCONFIG_MOD1_XOR = 6;
constexpr unsigned int SFPCONFIG_IMM_FIXED_CREG_LWM = 11;
constexpr unsigned int SFPCONFIG_IMM_FIXED_CREG_HWM = 14;

constexpr unsigned int SFPAND_MOD1_USE_VB = 1;

constexpr unsigned int SFPOR_MOD1_USE_VB = 1;

// sfpxor does not have USE_VB option

constexpr unsigned int SFPLZ_MOD1_CC_NONE = 0;
constexpr unsigned int SFPLZ_MOD1_CC_NE0 = 2;
constexpr unsigned int SFPLZ_MOD1_CC_COMP = 8;
constexpr unsigned int SFPLZ_MOD1_CC_EQ0 = 10;
constexpr unsigned int SFPLZ_MOD1_NOSGN_MASK = 4;
constexpr unsigned int SFPLZ_MOD1_NOSGN_CC_NONE = 4;
constexpr unsigned int SFPLZ_MOD1_NOSGN_CC_NE0 = 6;
constexpr unsigned int SFPLZ_MOD1_NOSGN_CC_COMP = 12;
constexpr unsigned int SFPLZ_MOD1_NOSGN_CC_EQ0 = 14;

constexpr unsigned int SFPCAST_MOD1_INT32_TO_FP32_RNE = 0;
constexpr unsigned int SFPCAST_MOD1_INT32_TO_FP32_RNS = 1;
// Added in BlackHole:
constexpr unsigned int SFPCAST_MOD1_SM32_TO_INT32 = 2;
constexpr unsigned int SFPCAST_MOD1_INT32_TO_SM32 = 3;

constexpr unsigned int SFPSTOCHRND_RND_EVEN = 0;
constexpr unsigned int SFPSTOCHRND_RND_STOCH = 1;

constexpr unsigned int SFPSTOCHRND_MOD1_FP32_TO_FP16A = 0;
constexpr unsigned int SFPSTOCHRND_MOD1_FP32_TO_FP16B = 1;
constexpr unsigned int SFPSTOCHRND_MOD1_FP32_TO_UINT8 = 2;
constexpr unsigned int SFPSTOCHRND_MOD1_FP32_TO_INT8 = 3;
constexpr unsigned int SFPSTOCHRND_MOD1_INT32_TO_UINT8 = 4;
constexpr unsigned int SFPSTOCHRND_MOD1_INT32_TO_INT8 = 5;
constexpr unsigned int SFPSTOCHRND_MOD1_FP32_TO_UINT16 = 6;
constexpr unsigned int SFPSTOCHRND_MOD1_FP32_TO_INT16 = 7;
constexpr unsigned int SFPSTOCHRND_MOD1_CONV_MASK = 7;
constexpr unsigned int SFPSTOCHRND_MOD1_IMM8 = 8; // only on INT32 src

constexpr unsigned int SFPXCMP_MOD1_CC_LT = 0;
constexpr unsigned int SFPXCMP_MOD1_CC_GE = 1;
constexpr unsigned int SFPXCMP_MOD1_CC_EQ = 2;
constexpr unsigned int SFPXCMP_MOD1_CC_NE = 3;
constexpr unsigned int SFPXCMP_MOD1_CC_GT = 4;
constexpr unsigned int SFPXCMP_MOD1_CC_LE = 5;
constexpr unsigned int SFPXCMP_MOD1_CC_MASK = 7;

constexpr unsigned int SFPXCMP_MOD1_TYPE_UINT = 0;
constexpr unsigned int SFPXCMP_MOD1_TYPE_INT = 1;
constexpr unsigned int SFPXCMP_MOD1_TYPE_SMAG = 2;
constexpr unsigned int SFPXCMP_MOD1_TYPE_FLOAT = 3;
constexpr unsigned int SFPXCMP_MOD1_TYPE_SHIFT = 4;
constexpr unsigned int SFPXCMP_MOD1_TYPE_MASK = 3;

constexpr unsigned int SFPXSCMP_SRC_ARG_POS = 1;

constexpr unsigned int SFPIADD_MOD1_ARG_LREG_DST = 0;
constexpr unsigned int SFPIADD_MOD1_ARG_IMM = 1;
constexpr unsigned int SFPIADD_MOD1_ARG_2SCOMP_LREG_DST = 2;
constexpr unsigned int SFPIADD_MOD1_CC_LT0 = 0;
constexpr unsigned int SFPIADD_MOD1_CC_NONE = 4;
constexpr unsigned int SFPIADD_MOD1_CC_GTE0 = 8;

constexpr unsigned int SFPXIADD_MOD1_IS_SUB = 8;

constexpr unsigned int SFPXPRED_MOD1_END  = 0;
constexpr unsigned int SFPXPRED_MOD1_IF   = 1;
constexpr unsigned int SFPXPRED_MOD1_ELSE = 2;
constexpr unsigned int SFPXPRED_MOD1_PUSH = 4;
constexpr unsigned int SFPXPRED_MOD1_DEPTH_SHIFT = 4;

constexpr unsigned int SFPXLOGIC_MOD1_AND = 0;
constexpr unsigned int SFPXLOGIC_MOD1_OR = 1;
constexpr unsigned int SFPXLOGIC_MOD1_NOT = 2;
constexpr unsigned int SFPXLOGIC_MOD1_NEARBY = 3;

constexpr unsigned int SFPXCONDI_TREE_ARG_POS = 0;

constexpr unsigned int SFPSHFT_MOD1_SHFT_IMM = 1;
constexpr unsigned int SFPSHFT_MOD1_SHFT_REG = 0;
// Added in BlackHole
constexpr unsigned int SFPSHFT_MOD1_LOGICAL = 0;
constexpr unsigned int SFPSHFT_MOD1_ARITHMETIC = 2;
constexpr unsigned int SFPSHFT_MOD1_SRC_LREG_C = 4;

constexpr unsigned int SFPSHFT2_MOD1_COPY4 = 0;
constexpr unsigned int SFPSHFT2_MOD1_SUBVEC_CHAINED_COPY4 = 1;
constexpr unsigned int SFPSHFT2_MOD1_SUBVEC_SHFLROR1_AND_COPY4 = 2;
constexpr unsigned int SFPSHFT2_MOD1_SUBVEC_SHFLROR1 = 3;
constexpr unsigned int SFPSHFT2_MOD1_SUBVEC_SHFLSHR1 = 4;
constexpr unsigned int SFPSHFT2_MOD1_SHFT_LREG = 5;
constexpr unsigned int SFPSHFT2_MOD1_SHFT_IMM = 6;

constexpr unsigned SFPGTLE_MOD1_SET_CC = 1;
constexpr unsigned SFPGTLE_MOD1_SET_TOS = 2;
constexpr unsigned SFPGTLE_MOD1_OR_TOS = 0;
constexpr unsigned SFPGTLE_MOD1_AND_TOS = 4;
constexpr unsigned SFPGTLE_MOD1_SET_DEST = 8;

constexpr unsigned int SFPGTLE_IMM_TYPE_INT = 0;
constexpr unsigned int SFPGTLE_IMM_TYPE_FLOAT = 1;
constexpr unsigned int SFPGTLE_IMM_TYPE_SMAG = 1;

constexpr unsigned int CREG_IDX_0P837300003 = 8;
constexpr unsigned int CREG_IDX_0 = 9;
constexpr unsigned int CREG_IDX_1 = 10;
constexpr unsigned int CREG_IDX_NEG_1 = 11;
constexpr unsigned int CREG_IDX_TILEID = 15;

#define HAVE_CREG_NEG_1 TARGET_XTT_TENSIX_WH_BH

#endif /* ! GCC_RVTT_PROTOS_H */
