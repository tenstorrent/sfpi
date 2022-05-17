///////////////////////////////////////////////////////////////////////////////
// SFPI HW constants, compiler abstraction
//////////////////////////////////////////////////////////////////////////////
#pragma once

namespace sfpi {

// GCC implementation leverages the riscv specific code paths and so the
// builtin name space uses riscv.  Call the builtins through the TT
// specific macros w/ the rvtt namespace

#ifdef COMPILE_FOR_EMULE

#define sfpi_inline inline

#define __builtin_rvtt_sfpincrwc(cr, d, b, a) sfpu_rvtt_sfpincrwc(cr, d, b, a)
#define __builtin_rvtt_sfpassign_lv(v, in) (in)
#define __builtin_rvtt_sfpload(mod0, mode, addr) sfpu_rvtt_sfpload(mod0, mode, addr)
#define __builtin_rvtt_sfpassignlr(lr) sfpu_rvtt_sfpassignlr(lr)
#define __builtin_rvtt_sfpkeepalive(x, n)
#define __builtin_rvtt_sfploadi_ex(mod0, imm16) sfpu_rvtt_sfploadi(mod0, imm16)
#define __builtin_rvtt_sfpstore(src, mod0, mode, addr) sfpu_rvtt_sfpstore(src, mod0, addr)
#define __builtin_rvtt_sfpmov(src, mod1) sfpu_rvtt_sfpmov(src, mod1)
#define __builtin_rvtt_sfpnop() sfpu_rvtt_sfpnop()
#define __builtin_rvtt_sfpillegal() sfpu_rvtt_sfpillegal()

#define __builtin_rvtt_sfpencc(imm12, mod1) sfpu_rvtt_sfpencc(imm12, mod1)
#define __builtin_rvtt_sfppushc() sfpu_rvtt_sfppushc(0)
#define __builtin_rvtt_sfppopc() sfpu_rvtt_sfppopc()
#define __builtin_rvtt_sfpsetcc_v(src, mod1) sfpu_rvtt_sfpsetcc_v(src, mod1)
#define __builtin_rvtt_sfpsetcc_i(imm12, mod1) sfpu_rvtt_sfpsetcc_i(imm12, mod1)
#define __builtin_rvtt_sfpscmp_ex(v, f, mod1) sfpu_rvtt_sfpscmp_ex(v, f, mod1)
#define __builtin_rvtt_sfpvcmp_ex(v1, v2, mod1) sfpu_rvtt_sfpvcmp_ex(v1, v2, mod1)
#define __builtin_rvtt_sfpcompc() sfpu_rvtt_sfpcompc()

#define __builtin_rvtt_sfpadd(va, vb, mod1) sfpu_rvtt_sfpadd(va, vb, mod1)
#define __builtin_rvtt_sfpmul(va, vb, mod1) sfpu_rvtt_sfpmul(va, vb, mod1)
#define __builtin_rvtt_sfpmad(va, vb, vc, mod1) sfpu_rvtt_sfpmad(va, vb, vc, mod1)

#define __builtin_rvtt_sfpexexp(src, mod1) sfpu_rvtt_sfpexexp(src, mod1)
#define __builtin_rvtt_sfpexman(src, mod1) sfpu_rvtt_sfpexman(src, mod1)

#define __builtin_rvtt_sfpsetexp_i(imm12, src) sfpu_rvtt_sfpsetexp_i(imm12, src)
#define __builtin_rvtt_sfpsetexp_v(dst, src) sfpu_rvtt_sfpsetexp_v(dst, src)

#define __builtin_rvtt_sfpsetman_i(imm12, src, mod) sfpu_rvtt_sfpsetman_i(imm12, src, mod)
#define __builtin_rvtt_sfpsetman_v(dst, src) sfpu_rvtt_sfpsetman_v(dst, src)

#define __builtin_rvtt_sfpabs(src, mod1) sfpu_rvtt_sfpabs(src, mod1)
#define __builtin_rvtt_sfpand(dst, src) sfpu_rvtt_sfpand(dst, src)
#define __builtin_rvtt_sfpor(dst, src) sfpu_rvtt_sfpor(dst, src)
#define __builtin_rvtt_sfpxor(dst, src) sfpu_rvtt_sfpxor(dst, src)
#define __builtin_rvtt_sfpnot(src) sfpu_rvtt_sfpnot(src)

#define __builtin_rvtt_sfpmuli(dst, imm12, mod1) sfpu_rvtt_sfpmuli(dst, imm12, mod1)
#define __builtin_rvtt_sfpaddi(dst, imm12, mod1) sfpu_rvtt_sfpaddi(dst, imm12, mod1)

#define __builtin_rvtt_sfpdivp2(imm12, src, mod1) sfpu_rvtt_sfpdivp2(imm12, src, mod1)

#define __builtin_rvtt_sfplz(src, mod1) sfpu_rvtt_sfplz(src, mod1)

#define __builtin_rvtt_sfpshft_i(dst, imm12) sfpu_rvtt_sfpshft_i(dst, imm12)
#define __builtin_rvtt_sfpshft_v(dst, src) sfpu_rvtt_sfpshft_v(dst, src)

#define __builtin_rvtt_sfpiadd_i(imm12, src, mod1) sfpu_rvtt_sfpiadd_i(imm12, src, mod1)
#define __builtin_rvtt_sfpiadd_i_ex(src, imm12, mod1) sfpu_rvtt_sfpiadd_i_ex(imm12, src, mod1)
#define __builtin_rvtt_sfpiadd_v(dst, src, mod1) sfpu_rvtt_sfpiadd_v(dst, src, mod1)
#define __builtin_rvtt_sfpiadd_v_ex(dst, src, mod1) sfpu_rvtt_sfpiadd_v_ex(dst, src, mod1)

#define __builtin_rvtt_sfpsetsgn_i(imm12, src) sfpu_rvtt_sfpsetsgn_i(imm12, src)
#define __builtin_rvtt_sfpsetsgn_v(dst, src) sfpu_rvtt_sfpsetsgn_v(dst, src)

#define __builtin_rvtt_sfplut(l0, l1, l2, dst, mod0) sfpu_rvtt_sfplut(l0, l1, l2, dst, mod0)
#define __builtin_rvtt_sfplutfp32_3r(l0, l1, l2, l3, mod0) sfpu_rvtt_sfplutfp32_3r(l0, l1, l2, l3, mod0)
#define __builtin_rvtt_sfplutfp32_6r(l0, l1, l2, l4, l5, l6, l3, mod0) sfpu_rvtt_sfplutfp32_6r(l0, l1, l2, l4, l5, l6, l3, mod0)

#define __builtin_rvtt_sfpcast(src, mod1) sfpu_rvtt_sfpcast(src, mod1)
#define __builtin_rvtt_sfpstochrnd_i(mode, imm8, srcc, mod1) sfpu_rvtt_sfpstochrnd_i(mode, imm8, srcc, mod1)
#define __builtin_rvtt_sfpstochrnd_v(mode, srcb, srcc, mod1) sfpu_rvtt_sfpstochrnd_v(mode, srcb, srcc, mod1)

#define __builtin_rvtt_sfptransp(l0, l1, l2, l3) sfpu_rvtt_sfptransp(l0, l1, l2, l3)

#define __builtin_rvtt_sfpshft2_i(dst, imm) sfpu_rvtt_sfpshft2_i(dst, imm)
#define __builtin_rvtt_sfpshft2_v(dst, src) sfpu_rvtt_sfpshft2_v(dst, src)
#define __builtin_rvtt_sfpshft2_g(l0, l1, l2, l3, mod) sfpu_rvtt_sfpshft2_g(l0, l1, l2, l3, mod)
#define __builtin_rvtt_sfpshft2_ge(src, l0, l1, l2, l3) sfpu_rvtt_sfpshft2_ge(src, l0, l1, l2, l3)
#define __builtin_rvtt_sfpshft2_e(src, mod) sfpu_rvtt_sfpshft2_e(src, mod)

#define __builtin_rvtt_sfpswap(dst, src, mod) sfpu_rvtt_sfpswap(dst, src, mod)

#define __builtin_rvtt_sfpconfig_v(l0, config_dest) sfpu_rvtt_sfpconfig_v(l0, config_dest)

#else // COMPILE_FOR_EMULE

#ifdef __clang__
#if !__has_builtin(__builtin_rvtt_sfp_load)
#error TT builtins not found: clang not supported
#include <fails to compile without extensions>
#endif // !__has_builtin(__builtin_rvtt_wh_sfpload)

#define sfpi_inline inline

#elif __GNUC__
#if !__has_builtin(__builtin_rvtt_wh_sfpload)
#error TT builtins not found: compile with -march=rv32iy -mabi=ilp32 -msfpu
#include <fails to compile without extensions>
#endif // !__has_builtin(__builtin_rvtt_wh_sfpload)

typedef float __rvtt_vec_t __attribute__((vector_size(64*4)));

#define sfpi_inline __attribute__((always_inline)) inline

#define __builtin_rvtt_sfpincrwc(cr, d, b, a) __builtin_rvtt_wh_sfpincrwc(cr, d, b, a)
#define __builtin_rvtt_sfpassign_lv(v, in) __builtin_rvtt_wh_sfpassign_lv(v, in)
#define __builtin_rvtt_sfpload(mod0, mode, addr) __builtin_rvtt_wh_sfpload((void *)ckernel::instrn_buffer, mod0, mode, addr)
#define __builtin_rvtt_sfpassignlr(lr) __builtin_rvtt_wh_sfpassignlr(lr)
#define __builtin_rvtt_sfpkeepalive(x, n) __builtin_rvtt_wh_sfpkeepalive(x, n)

#define __builtin_rvtt_sfploadi_ex(mod0, imm16) __builtin_rvtt_wh_sfploadi_ex((void *)ckernel::instrn_buffer, mod0, imm16)
#define __builtin_rvtt_sfpstore(src, mod0, mode, addr) __builtin_rvtt_wh_sfpstore((void *)ckernel::instrn_buffer, src, mod0, mode, addr)
#define __builtin_rvtt_sfpmov(src, mod1) __builtin_rvtt_wh_sfpmov(src, mod1)
#define __builtin_rvtt_sfpnop() __builtin_rvtt_wh_sfpnop()
#define __builtin_rvtt_sfpillegal() __builtin_rvtt_wh_sfpillegal()

#define __builtin_rvtt_sfpencc(imm12, mod1) __builtin_rvtt_wh_sfpencc(imm12, mod1)
#define __builtin_rvtt_sfppushc() __builtin_rvtt_wh_sfppushc(0)
#define __builtin_rvtt_sfppopc() __builtin_rvtt_wh_sfppopc(0)
#define __builtin_rvtt_sfpsetcc_v(src, mod1) __builtin_rvtt_wh_sfpsetcc_v(src, mod1)
#define __builtin_rvtt_sfpsetcc_i(imm12, mod1) __builtin_rvtt_wh_sfpsetcc_i(imm12, mod1)
#define __builtin_rvtt_sfpscmp_ex(v, f, mod1) __builtin_rvtt_wh_sfpscmp_ex((void*)ckernel::instrn_buffer, v, f, mod1)
#define __builtin_rvtt_sfpvcmp_ex(v1, v2, mod1) __builtin_rvtt_wh_sfpvcmp_ex(v1, v2, mod1)
#define __builtin_rvtt_sfpcompc() __builtin_rvtt_wh_sfpcompc()

#define __builtin_rvtt_sfpadd(va, vb, mod1) __builtin_rvtt_wh_sfpadd(va, vb, mod1)
#define __builtin_rvtt_sfpmul(va, vb, mod1) __builtin_rvtt_wh_sfpmul(va, vb, mod1)
#define __builtin_rvtt_sfpmad(va, vb, vc, mod1) __builtin_rvtt_wh_sfpmad(va, vb, vc, mod1)

#define __builtin_rvtt_sfpexexp(src, mod1) __builtin_rvtt_wh_sfpexexp(src, mod1)
#define __builtin_rvtt_sfpexman(src, mod1) __builtin_rvtt_wh_sfpexman(src, mod1)

#define __builtin_rvtt_sfpsetexp_i(imm12, src) __builtin_rvtt_wh_sfpsetexp_i((void *)ckernel::instrn_buffer, imm12, src)
#define __builtin_rvtt_sfpsetexp_v(dst, src) __builtin_rvtt_wh_sfpsetexp_v(dst, src)

#define __builtin_rvtt_sfpsetman_i(imm12, src, mod) __builtin_rvtt_wh_sfpsetman_i((void *)ckernel::instrn_buffer, imm12, src, mod)
#define __builtin_rvtt_sfpsetman_v(dst, src) __builtin_rvtt_wh_sfpsetman_v(dst, src)

#define __builtin_rvtt_sfpabs(src, mod1) __builtin_rvtt_wh_sfpabs(src, mod1)
#define __builtin_rvtt_sfpand(dst, src) __builtin_rvtt_wh_sfpand(dst, src)
#define __builtin_rvtt_sfpor(dst, src) __builtin_rvtt_wh_sfpor(dst, src)
#define __builtin_rvtt_sfpxor(dst, src) __builtin_rvtt_wh_sfpxor(dst, src)
#define __builtin_rvtt_sfpnot(src) __builtin_rvtt_wh_sfpnot(src)

#define __builtin_rvtt_sfpmuli(dst, imm12, mod1) __builtin_rvtt_wh_sfpmuli((void *)ckernel::instrn_buffer, dst, imm12, mod1)
#define __builtin_rvtt_sfpaddi(dst, imm12, mod1) __builtin_rvtt_wh_sfpaddi((void *)ckernel::instrn_buffer, dst, imm12, mod1)

#define __builtin_rvtt_sfpdivp2(imm12, src, mod1) __builtin_rvtt_wh_sfpdivp2((void *)ckernel::instrn_buffer, imm12, src, mod1)

#define __builtin_rvtt_sfplz(src, mod1) __builtin_rvtt_wh_sfplz(src, mod1)

#define __builtin_rvtt_sfpshft_i(dst, imm12) __builtin_rvtt_wh_sfpshft_i((void *)ckernel::instrn_buffer, dst, imm12)
#define __builtin_rvtt_sfpshft_v(dst, src) __builtin_rvtt_wh_sfpshft_v(dst, src)

#define __builtin_rvtt_sfpiadd_i(imm12, src, mod1) __builtin_rvtt_wh_sfpiadd_i((void *)ckernel::instrn_buffer, src, imm12, mod1)
#define __builtin_rvtt_sfpiadd_i_ex(src, imm12, mod1) __builtin_rvtt_wh_sfpiadd_i_ex((void *)ckernel::instrn_buffer, src, imm12, mod1)
#define __builtin_rvtt_sfpiadd_v(dst, src, mod1) __builtin_rvtt_wh_sfpiadd_v(dst, src, mod1)
#define __builtin_rvtt_sfpiadd_v_ex(dst, src, mod1) __builtin_rvtt_wh_sfpiadd_v_ex(dst, src, mod1)

#define __builtin_rvtt_sfpsetsgn_i(imm12, src) __builtin_rvtt_wh_sfpsetsgn_i((void *)ckernel::instrn_buffer, imm12, src)
#define __builtin_rvtt_sfpsetsgn_v(dst, src) __builtin_rvtt_wh_sfpsetsgn_v(dst, src)

#define __builtin_rvtt_sfplut(l0, l1, l2, dst, mod0) __builtin_rvtt_wh_sfplut(l0, l1, l2, dst, mod0)
#define __builtin_rvtt_sfplutfp32_3r(l0, l1, l2, l3, mod0) __builtin_rvtt_wh_sfplutfp32_3r(l0, l1, l2, l3, mod0)
#define __builtin_rvtt_sfplutfp32_6r(l0, l1, l2, l4, l5, l6, l3, mod0) __builtin_rvtt_wh_sfplutfp32_6r(l0, l1, l2, l4, l5, l6, l3, mod0)

#define __builtin_rvtt_sfpcast(src, mod1) __builtin_rvtt_wh_sfpcast(src, mod1)
#define __builtin_rvtt_sfpstochrnd_i(mode, imm8, srcc, mod1) __builtin_rvtt_wh_sfpstochrnd_i((void *)ckernel::instrn_buffer, mode, imm8, srcc, mod1)
#define __builtin_rvtt_sfpstochrnd_v(mode, srcb, srcc, mod1) __builtin_rvtt_wh_sfpstochrnd_v(mode, srcb, srcc, mod1)

#define __builtin_rvtt_sfptransp(l0, l1, l2, l3)                \
    asm("SFPTRANSP %0, %1, %2, %3"                              \
        : "+Q0" (l0), "+Q1" (l1), "+Q2" (l2), "+Q3" (l3)        \
        :)

#define __builtin_rvtt_sfpshft2_i(dst, imm) __builtin_rvtt_wh_sfpshft2_i(dst, imm)
#define __builtin_rvtt_sfpshft2_v(dst, src) __builtin_rvtt_wh_sfpshft2_v(dst, src)
#define __builtin_rvtt_sfpshft2_g(l0, l1, l2, l3, mod)                  \
    asm("SFPSHFT2 0, L0, L0, %[q0], %[q1], %[q2], %[q3], %[modp]"       \
        : [q0] "+Q0" (l0), [q1] "+Q1" (l1), [q2] "+Q2" (l2), [q3] "+Q3" (l3) \
        : [modp] "i" (mod))
#define __builtin_rvtt_sfpshft2_ge(src, l0, l1, l2, l3)                 \
    asm("SFPSHFT2 0, %[lsrc], L0, %[q0], %[q1], %[q2], %[q3], %[mod]"    \
        : [q0] "+Q0" (l0), [q1] "+Q1" (l1), [q2] "+Q2" (l2), [q3] "+Q3" (l3) \
        : [lsrc] "x" (src), [mod] "i" (SFPSHFT2_MOD1_SUBVEC_SHFLROR1_AND_COPY4))
#define __builtin_rvtt_sfpshft2_e(src, mod) __builtin_rvtt_wh_sfpshft2_e(src, mod)

#define __builtin_rvtt_sfpswap(dst, src, mod)       \
    asm("SFPSWAP %[d], %[s], %[m]"                  \
        : [d] "+x" (src), [s] "+x" (dst)            \
        : [m] "i" (mod));                           \
        __builtin_rvtt_sfpnop()

#define __builtin_rvtt_sfpconfig_v(l0, config_dest) __builtin_rvtt_wh_sfpconfig_v(l0, config_dest)

#endif // __GNUC__

#endif // COMPILE_FOR_EMULE

constexpr unsigned int SFP_LREG_COUNT = 8;

// WH size is 1024 rows x 16 cols by 16 bits OR 512 rows by 16 cols by 32 bits
constexpr unsigned int SFP_DESTREG_STRIDE = 2;
constexpr unsigned int SFP_DESTREG_MAX_SIZE = 1024;
constexpr unsigned int SFP_DESTREG_MAX_ADDR = (SFP_DESTREG_MAX_SIZE / SFP_DESTREG_STRIDE);

constexpr unsigned int SFPLOAD_MOD0_FMT_SRCB = 0;
constexpr unsigned int SFPLOAD_MOD0_FMT_FP16A = 1;
constexpr unsigned int SFPLOAD_MOD0_FMT_FP16B = 2;
constexpr unsigned int SFPLOAD_MOD0_FMT_FP32 = 3;
constexpr unsigned int SFPLOAD_MOD0_FMT_INT32_TO_SM = 12;
constexpr unsigned int SFPLOAD_ADDR_MODE_NOINC = 3;

constexpr unsigned int SFPSTORE_MOD0_FMT_SRCB = 0;
constexpr unsigned int SFPSTORE_MOD0_FMT_FP16A = 1;
constexpr unsigned int SFPSTORE_MOD0_FMT_FP16B = 2;
constexpr unsigned int SFPSTORE_MOD0_FMT_FP32 = 3;
constexpr unsigned int SFPSTORE_MOD0_FMT_INT32_TO_SM = 12;
constexpr unsigned int SFPSTORE_ADDR_MODE_NOINC = 3;

constexpr unsigned int SFPMOV_MOD1_COMPSIGN = 1;

constexpr unsigned int SFPMAD_MOD1_OFFSET_NONE = 0;

constexpr unsigned int SFPLOADI_MOD0_FLOATB = 0;
constexpr unsigned int SFPLOADI_MOD0_FLOATA = 1;
constexpr unsigned int SFPLOADI_MOD0_USHORT = 2;
constexpr unsigned int SFPLOADI_MOD0_SHORT = 4;
constexpr unsigned int SFPLOADI_MOD0_UPPER = 8;
constexpr unsigned int SFPLOADI_MOD0_LOWER = 10;
constexpr unsigned int SFPLOADI_EX_MOD0_32BIT_MASK = 16;
constexpr unsigned int SFPLOADI_EX_MOD0_INT32 = 16;
constexpr unsigned int SFPLOADI_EX_MOD0_UINT32 = 17;
constexpr unsigned int SFPLOADI_EX_MOD0_FLOAT = 18;

constexpr unsigned int SFPEXMAN_MOD1_PAD8 = 0;
constexpr unsigned int SFPEXMAN_MOD1_PAD9 = 1;

constexpr unsigned int SFPEXEXP_MOD1_DEBIAS = 0;
constexpr unsigned int SFPEXEXP_MOD1_NODEBIAS = 1;
constexpr unsigned int SFPEXEXP_MOD1_SET_CC_SGN_EXP = 2;
constexpr unsigned int SFPEXEXP_MOD1_SET_CC_COMP_EXP = 8;
constexpr unsigned int SFPEXEXP_MOD1_SET_CC_SGN_COMP_EXP = 10;

constexpr unsigned int SFPABS_MOD1_INT = 0;
constexpr unsigned int SFPABS_MOD1_FLOAT = 1;

constexpr unsigned int SFPIADD_MOD1_ARG_LREG_DST = 0;
constexpr unsigned int SFPIADD_MOD1_ARG_IMM = 1;
constexpr unsigned int SFPIADD_MOD1_ARG_2SCOMP_LREG_DST = 2;
constexpr unsigned int SFPIADD_MOD1_CC_LT0 = 0;
constexpr unsigned int SFPIADD_MOD1_CC_NONE = 4;
constexpr unsigned int SFPIADD_MOD1_CC_GTE0 = 8;

constexpr unsigned int SFPCMP_EX_MOD1_CC_NONE = 0;
constexpr unsigned int SFPCMP_EX_MOD1_CC_LT = 1;
constexpr unsigned int SFPCMP_EX_MOD1_CC_EQ = 2;
constexpr unsigned int SFPCMP_EX_MOD1_CC_GTE = 3;
constexpr unsigned int SFPCMP_EX_MOD1_CC_NE = 4;
constexpr unsigned int SFPCMP_EX_MOD1_CC_LTE = 5;
constexpr unsigned int SFPCMP_EX_MOD1_CC_GT = 6;
constexpr unsigned int SFPCMP_EX_MOD1_CC_MASK = 7;

constexpr unsigned int SFPSCMP_EX_MOD1_FMT_A = 8;
constexpr unsigned int SFPSCMP_EX_MOD1_FMT_B = 16;
constexpr unsigned int SFPSCMP_EX_MOD1_FMT_FLOAT = 32;
constexpr unsigned int SFPSCMP_EX_MOD1_FMT_MASK = 0x38;

constexpr unsigned int SFPIADD_EX_MOD1_IS_SUB = 16;

constexpr unsigned int SFPIADD_I_EX_MOD1_SIGNED = 8;
constexpr unsigned int SFPIADD_I_EX_MOD1_IS_12BITS = 32;

constexpr unsigned int SFPSETCC_MOD1_LREG_LT0 = 0;
constexpr unsigned int SFPSETCC_MOD1_IMM_BIT0 = 1;
constexpr unsigned int SFPSETCC_MOD1_LREG_NE0 = 2;
constexpr unsigned int SFPSETCC_MOD1_LREG_GTE0 = 4;
constexpr unsigned int SFPSETCC_MOD1_LREG_EQ0 = 6;
constexpr unsigned int SFPSETCC_MOD1_COMP = 8;

constexpr unsigned int SFPSETMAN_EX_MOD1_16BITIMM = 2;

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

constexpr unsigned int SFPCAST_MOD1_RND_EVEN = 0;
constexpr unsigned int SFPCAST_MOD1_RND_STOCH = 1;

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

constexpr unsigned int SFPSHFT2_MOD1_COPY4 = 0;
constexpr unsigned int SFPSHFT2_MOD1_SUBVEC_CHAINED_COPY4 = 1;
constexpr unsigned int SFPSHFT2_MOD1_SUBVEC_SHFLROR1_AND_COPY4 = 2;
constexpr unsigned int SFPSHFT2_MOD1_SUBVEC_SHFLROR1 = 3;
constexpr unsigned int SFPSHFT2_MOD1_SUBVEC_SHFLSHR1 = 4;
constexpr unsigned int SFPSHFT2_MOD1_SHFT_IMM = 5;
constexpr unsigned int SFPSHFT2_MOD1_SHFT_LREG = 6;

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
