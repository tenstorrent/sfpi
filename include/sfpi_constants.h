/*
 * SPDX-FileCopyrightText: © 2023-2026 Tenstorrent Inc.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

///////////////////////////////////////////////////////////////////////////////
// SFPI HW constants, compiler abstraction
//////////////////////////////////////////////////////////////////////////////
#pragma once

namespace sfpi {

constexpr unsigned int SFP_LREG_COUNT = 8;

constexpr unsigned int SFP_DESTREG_STRIDE = 2;
constexpr unsigned int SFP_DESTREG_COUNT = 0x400 / SFP_DESTREG_STRIDE;

#if __riscv_xtttensixqsr
constexpr unsigned int SFP_SRCSREG_STRIDE = 2;
constexpr unsigned int SFP_SRCSREG_COUNT = 0x10 / SFP_SRCSREG_STRIDE;
constexpr unsigned int SFP_SRCSREG_BASE = 0x400;
#endif

constexpr unsigned int SFPLOAD_MOD0_FMT_SRCB = 0;
constexpr unsigned int SFPLOAD_MOD0_FMT_FP16A = 1;
constexpr unsigned int SFPLOAD_MOD0_FMT_FP16B = 2;
constexpr unsigned int SFPLOAD_MOD0_FMT_FP32 = 3;
#if __riscv_xtttensixwh
constexpr unsigned int SFPLOAD_MOD0_FMT_BOB32 = 4; // Bag Of Bits
constexpr unsigned int SFPLOAD_MOD0_FMT_SM32 = 12;
__attribute__((__deprecated__("use SFPLOAD_MOD0_FMT_SM32 instead")))
constexpr unsigned int SFPLOAD_MOD0_FMT_INT32_TO_SM = SFPLOAD_MOD0_FMT_SM32;
constexpr unsigned int SFPLOAD_ADDR_MODE_NOINC = 3;
#elif __riscv_xtttensixbh
constexpr unsigned int SFPLOAD_MOD0_FMT_BOB32 = 12; // Bag Of Bits
constexpr unsigned int SFPLOAD_ADDR_MODE_NOINC = 7;
#endif

constexpr unsigned int SFPSTORE_MOD0_FMT_SRCB = 0;
constexpr unsigned int SFPSTORE_MOD0_FMT_FP16A = 1;
constexpr unsigned int SFPSTORE_MOD0_FMT_FP16B = 2;
constexpr unsigned int SFPSTORE_MOD0_FMT_FP32 = 3;
constexpr unsigned int SFPSTORE_MOD0_FMT_UINT16 = 6;
#if __riscv_xtttensixwh
constexpr unsigned int SFPSTORE_MOD0_FMT_BOB32 = 4; // Bag Of Bits
constexpr unsigned int SFPSTORE_MOD0_FMT_SM32 = 12;
__attribute__((__deprecated__("use SFPSTORE_MOD0_FMT_SM32 instead")))
constexpr unsigned int SFPSTORE_MOD0_FMT_INT32_TO_SM = SFPSTORE_MOD0_FMT_SM32;
constexpr unsigned int SFPSTORE_ADDR_MODE_NOINC = 3;
#elif __riscv_xtttensixbh
constexpr unsigned int SFPSTORE_MOD0_FMT_BOB32 = 4; // Bag Of Bits
constexpr unsigned int SFPSTORE_ADDR_MODE_NOINC = 7;
#endif

constexpr unsigned int SFPMOV_MOD1_COMPSIGN = 1;
#if __riscv_xtttensixbh
constexpr unsigned int SFPMOV_MOD1_CONFIG = 8;
#endif

constexpr unsigned int SFPMAD_MOD1_OFFSET_NONE = 0;
#if __riscv_xtttensixbh
constexpr unsigned int SFPMAD_MOD1_SRCA_LREG7 = 4;
constexpr unsigned int SFPMAD_MOD1_DST_LREG7 = 8;

constexpr unsigned int SFPADD_MOD1_SRCA_LREG7 = 4;
constexpr unsigned int SFPADD_MOD1_DST_LREG7 = 8;

constexpr unsigned int SFPMUL_MOD1_SRCA_LREG7 = 4;
constexpr unsigned int SFPMUL_MOD1_DST_LREG7 = 8;
#endif

constexpr unsigned int SFPLOADI_MOD0_FLOATB = 0;
constexpr unsigned int SFPLOADI_MOD0_FLOATA = 1;
constexpr unsigned int SFPLOADI_MOD0_USHORT = 2;
constexpr unsigned int SFPLOADI_MOD0_SHORT = 4;
constexpr unsigned int SFPLOADI_MOD0_UPPER = 8;
constexpr unsigned int SFPLOADI_MOD0_LOWER = 10;
constexpr unsigned int SFPXLOADI_MOD0_32BIT_MASK = 16;
constexpr unsigned int SFPXLOADI_MOD0_INT32 = 16;
constexpr unsigned int SFPXLOADI_MOD0_UINT32 = 17;
constexpr unsigned int SFPXLOADI_MOD0_FLOAT = 18;

constexpr unsigned int SFPEXMAN_MOD1_PAD8 = 0;
constexpr unsigned int SFPEXMAN_MOD1_PAD9 = 1;

constexpr unsigned int SFPEXEXP_MOD1_DEBIAS = 0;
constexpr unsigned int SFPEXEXP_MOD1_NODEBIAS = 1;
constexpr unsigned int SFPEXEXP_MOD1_SET_CC_SGN_EXP = 2;
constexpr unsigned int SFPEXEXP_MOD1_SET_CC_COMP_EXP = 8;
constexpr unsigned int SFPEXEXP_MOD1_SET_CC_SGN_COMP_EXP = 10;

constexpr unsigned int SFPABS_MOD1_INT = 0;
constexpr unsigned int SFPABS_MOD1_FLOAT = 1;

#if __riscv_xtttensixbh
constexpr unsigned int SFPARECIP_MOD1_RECIP = 0;
constexpr unsigned int SFPARECIP_MOD1_COND_RECIP = 1;
constexpr unsigned int SFPARECIP_MOD1_EXP = 2;
#endif

#if __riscv_xtttensixbh
constexpr unsigned int SFPMUL24_MOD1_NONE = 0;
constexpr unsigned int SFPMUL24_MOD1_LOWER = 0;
constexpr unsigned int SFPMUL24_MOD1_UPPER = 1;
constexpr unsigned int SFPMUL24_MOD1_SRCA_LREG7 = 4;
constexpr unsigned int SFPMUL24_MOD1_DST_LREG7 = 8;
#endif

constexpr unsigned int SFPIADD_MOD1_ARG_LREG_DST = 0;
constexpr unsigned int SFPIADD_MOD1_ARG_IMM = 1;
constexpr unsigned int SFPIADD_MOD1_ARG_2SCOMP_LREG_DST = 2;
constexpr unsigned int SFPIADD_MOD1_CC_LT0 = 0;
constexpr unsigned int SFPIADD_MOD1_CC_NONE = 4;
constexpr unsigned int SFPIADD_MOD1_CC_GTE0 = 8;

constexpr unsigned int SFPXIADD_MOD1_SIGNED = 8;
constexpr unsigned int SFPXIADD_MOD1_IS_SUB = 16;
constexpr unsigned int SFPXIADD_MOD1_12BIT = 32;
constexpr unsigned int SFPXIADD_MOD1_16BIT = 64;

constexpr unsigned int SFPXCMP_MOD1_CC_NONE = 0;
constexpr unsigned int SFPXCMP_MOD1_CC_LT = 1;
constexpr unsigned int SFPXCMP_MOD1_CC_EQ = 2;
constexpr unsigned int SFPXCMP_MOD1_CC_GTE = 3;
constexpr unsigned int SFPXCMP_MOD1_CC_NE = 4;
constexpr unsigned int SFPXCMP_MOD1_CC_LTE = 5;
constexpr unsigned int SFPXCMP_MOD1_CC_GT = 6;
constexpr unsigned int SFPXCMP_MOD1_CC_MASK = 7;

constexpr unsigned int SFPXSCMP_MOD1_FMT_A = 8;
constexpr unsigned int SFPXSCMP_MOD1_FMT_B = 16;
constexpr unsigned int SFPXSCMP_MOD1_FMT_FLOAT = 32;
constexpr unsigned int SFPXSCMP_MOD1_FMT_MASK = 0x38;

constexpr unsigned int SFPXBOOL_MOD1_OR = 1;
constexpr unsigned int SFPXBOOL_MOD1_AND = 2;
constexpr unsigned int SFPXBOOL_MOD1_NOT = 3;

constexpr unsigned int SFPSETCC_MOD1_LREG_LT0 = 0;
constexpr unsigned int SFPSETCC_MOD1_IMM_BIT0 = 1;
constexpr unsigned int SFPSETCC_MOD1_LREG_NE0 = 2;
constexpr unsigned int SFPSETCC_MOD1_LREG_GTE0 = 4;
constexpr unsigned int SFPSETCC_MOD1_LREG_EQ0 = 6;
constexpr unsigned int SFPSETCC_MOD1_COMP = 8;

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

constexpr unsigned int SFPPUSHC_MOD1_PUSH = 0;
constexpr unsigned int SFPPUSHC_MOD1_REPLACE = 1;

constexpr unsigned int SFPPOPC_MOD1_POP = 0;

constexpr unsigned int SFPLZ_MOD1_CC_NONE = 0;
constexpr unsigned int SFPLZ_MOD1_CC_NE0 = 2;
constexpr unsigned int SFPLZ_MOD1_CC_COMP = 8;
constexpr unsigned int SFPLZ_MOD1_CC_EQ0 = 10;
constexpr unsigned int SFPLZ_MOD1_NOSGN_MASK = 4;
constexpr unsigned int SFPLZ_MOD1_NOSGN_CC_NONE = 4;
constexpr unsigned int SFPLZ_MOD1_NOSGN_CC_NE0 = 6;
constexpr unsigned int SFPLZ_MOD1_NOSGN_CC_COMP = 12;
constexpr unsigned int SFPLZ_MOD1_NOSGN_CC_EQ0 = 14;

constexpr unsigned int SFPSDIVP2_MOD1_ADD = 1;

constexpr unsigned int SFPLUT_MOD0_SGN_UPDATE = 0;
constexpr unsigned int SFPLUT_MOD0_SGN_RETAIN = 4;

constexpr unsigned int SFPLUTFP32_MOD0_FP32_3ENTRY_TABLE = 0;
constexpr unsigned int SFPLUTFP32_MOD0_FP16_6ENTRY_TABLE1 = 2;
constexpr unsigned int SFPLUTFP32_MOD0_FP16_6ENTRY_TABLE2 = 3;
constexpr unsigned int SFPLUTFP32_MOD0_FP16_3ENTRY_TABLE = 10;
constexpr unsigned int SFPLUTFP32_MOD0_SGN_UPDATE = 0;
constexpr unsigned int SFPLUTFP32_MOD0_SGN_RETAIN = 4;

constexpr unsigned int SFPCAST_MOD1_INT32_TO_FP32_RNE = 0; // Round Nearest Evne
constexpr unsigned int SFPCAST_MOD1_INT32_TO_FP32_RNS = 1; // Round Nearest Stochastic
#if __riscv_xtttensixbh
// This conversion has a bug, sign-mag -0 converts to mostneg int32, not zero
constexpr unsigned int SFPCAST_MOD1_SM32_TO_INT32 = 2; // Sign-Mag to 2's compl
constexpr unsigned int SFPCAST_MOD1_INT32_TO_SM32 = 3; // 2's compl to Sign-Mag
#endif
// Deprecate these two names (renamed due to features added in Blackhole)
__attribute__((__deprecated__("use SFPCAST_MOD1_INT32_TO_FP32_RNE instead")))
constexpr unsigned int SFPCAST_MOD1_RND_EVEN = SFPCAST_MOD1_INT32_TO_FP32_RNE;
__attribute__((__deprecated__("use SFPCAST_MOD1_INT32_TO_FP32_RNS instead")))
constexpr unsigned int SFPCAST_MOD1_RND_STOCH = SFPCAST_MOD1_INT32_TO_FP32_RNS;

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
constexpr unsigned int SFPSTOCHRND_MOD1_IMM8 = 8;

constexpr unsigned int SFPSHFT_MOD1_LOGICAL = 0;
constexpr unsigned int SFPSHFT_MOD1_SHIFT_IMM = 0;
constexpr unsigned int SFPSHFT_MOD1_SHIFT_LREGC = 1;
#if __riscv_xtttensixbh
constexpr unsigned int SFPSHFT_MOD1_ARITHMETIC = 2;
constexpr unsigned int SFPSHFT_MOD1_SRC_LREGC = 4;
#endif

constexpr unsigned int SFPSHFT2_MOD1_COPY4 = 0;
constexpr unsigned int SFPSHFT2_MOD1_SUBVEC_CHAINED_COPY4 = 1;
constexpr unsigned int SFPSHFT2_MOD1_SUBVEC_SHFLROR1_AND_COPY4 = 2;
constexpr unsigned int SFPSHFT2_MOD1_SUBVEC_SHFLROR1 = 3;
constexpr unsigned int SFPSHFT2_MOD1_SUBVEC_SHFLSHR1 = 4;
constexpr unsigned int SFPSHFT2_MOD1_SHFT_LREG = 5;
constexpr unsigned int SFPSHFT2_MOD1_SHFT_IMM = 6;

constexpr unsigned int SFPSWAP_MOD1_SWAP = 0;
constexpr unsigned int SFPSWAP_MOD1_VEC_MIN_MAX = 1;
constexpr unsigned int SFPSWAP_MOD1_SUBVEC_MIN01_MAX23 = 2;
constexpr unsigned int SFPSWAP_MOD1_SUBVEC_MIN02_MAX13 = 3;
constexpr unsigned int SFPSWAP_MOD1_SUBVEC_MIN03_MAX12 = 4;
constexpr unsigned int SFPSWAP_MOD1_SUBVEC_MIN0_MAX123 = 5;
constexpr unsigned int SFPSWAP_MOD1_SUBVEC_MIN1_MAX023 = 6;
constexpr unsigned int SFPSWAP_MOD1_SUBVEC_MIN2_MAX013 = 7;
constexpr unsigned int SFPSWAP_MOD1_SUBVEC_MIN3_MAX012 = 8;

constexpr unsigned int SFPCONFIG_DEST_MACRO_INST0 = 0;
constexpr unsigned int SFPCONFIG_DEST_MACRO_INST1 = 1;
constexpr unsigned int SFPCONFIG_DEST_MACRO_INST2 = 2;
constexpr unsigned int SFPCONFIG_DEST_MACRO_INST3 = 3;
constexpr unsigned int SFPCONFIG_DEST_MACRO_SEQ0 = 4;
constexpr unsigned int SFPCONFIG_DEST_MACRO_SEQ1 = 5;
constexpr unsigned int SFPCONFIG_DEST_MACRO_SEQ2 = 6;
constexpr unsigned int SFPCONFIG_DEST_MACRO_SEQ3 = 7;
constexpr unsigned int SFPCONFIG_DEST_MACRO_CTRL = 8;
#if __riscv_xtttensixbh
constexpr unsigned int SFPCONFIG_SRC_RAND = 9;
#endif
constexpr unsigned int SFPCONFIG_DEST_LREG11 = 11;
constexpr unsigned int SFPCONFIG_DEST_LREG12 = 12;
constexpr unsigned int SFPCONFIG_DEST_LREG13 = 13;
constexpr unsigned int SFPCONFIG_DEST_LREG14 = 14;
constexpr unsigned int SFPCONFIG_DEST_SFPU_CTRL = 15;

constexpr unsigned int SFPCONFIG_MOD1_SRC_R0_LREG0 = 0;

constexpr unsigned int CREG_IDX_0P837300003 = 8;
constexpr unsigned int CREG_IDX_0 = 9;
constexpr unsigned int CREG_IDX_1 = 10;
constexpr unsigned int CREG_IDX_PRGM0 = 11;
constexpr unsigned int CREG_IDX_PRGM1 = 12;
constexpr unsigned int CREG_IDX_PRGM2 = 13;
constexpr unsigned int CREG_IDX_PRGM3 = 14;
constexpr unsigned int CREG_IDX_NEG_1 = CREG_IDX_PRGM0;
constexpr unsigned int CREG_IDX_TILEID = 15;

} // namespace sfpi
