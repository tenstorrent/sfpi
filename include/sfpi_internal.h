///////////////////////////////////////////////////////////////////////////////
// SFPI HW constants, compiler abstraction
//////////////////////////////////////////////////////////////////////////////
#pragma once

#ifdef ARCH_GRAYSKULL

namespace sfpi {

// GCC implementation leverages the riscv specific code paths and so the
// builtin name space uses riscv.  Call the builtins through the TT
// specific macros w/ the rvtt namespace

#ifdef COMPILE_FOR_EMULE

#define sfpi_inline inline

#define __builtin_rvtt_sfpassign_lv(v, in) (in)
#define __builtin_rvtt_sfpload(mod0, addr) sfpu_rvtt_sfpload(mod0, addr)
#define __builtin_rvtt_sfpload_lv(dst, mod0, addr) sfpu_rvtt_sfpload(mod0, addr)
#define __builtin_rvtt_sfpassignlr(lr) sfpu_rvtt_sfpassignlr(lr)
#define __builtin_rvtt_sfpkeepalive(x, n)
#define __builtin_rvtt_sfploadi(mod0, imm16) sfpu_rvtt_sfploadi(mod0, imm16)
#define __builtin_rvtt_sfploadi_lv(dst, mod0, imm16) sfpu_rvtt_sfploadi(mod0, imm16)
#define __builtin_rvtt_sfpstore(src, mod0, addr) sfpu_rvtt_sfpstore(src, mod0, addr)
#define __builtin_rvtt_sfpmov(src, mod1) sfpu_rvtt_sfpmov(src, mod1)
#define __builtin_rvtt_sfpnop() sfpu_rvtt_sfpnop()
#define __builtin_rvtt_sfpillegal() sfpu_rvtt_sfpillegal()

#define __builtin_rvtt_sfpencc(imm12, mod1) sfpu_rvtt_sfpencc(imm12, mod1)
#define __builtin_rvtt_sfppushc() sfpu_rvtt_sfppushc()
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

#define __builtin_rvtt_sfpsetman_i(imm12, src) sfpu_rvtt_sfpsetman_i(imm12, src)
#define __builtin_rvtt_sfpsetman_v(dst, src) sfpu_rvtt_sfpsetman_v(dst, src)

#define __builtin_rvtt_sfpabs(src, mod1) sfpu_rvtt_sfpabs(src, mod1)
#define __builtin_rvtt_sfpand(dst, src) sfpu_rvtt_sfpand(dst, src)
#define __builtin_rvtt_sfpor(dst, src) sfpu_rvtt_sfpor(dst, src)
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

#else // COMPILE_FOR_EMULE

#ifdef __clang__
#if !__has_builtin(__builtin_rvtt_sfp_load)
#error TT builtins not found: compile with --target=riscv32-unknown-elf
#include <fails to compile without extensions>
#endif // !__has_builtin(__builtin_riscv_sfpload)

// If you are reading this, note that the clang implementation is deprecated

#define sfpi_inline inline

#elif __GNUC__
#if !__has_builtin(__builtin_riscv_sfpload)
#error TT builtins not found: compile with -march=rv32iy -mabi=ilp32 -msfpu
#include <fails to compile without extensions>
#endif // !__has_builtin(__builtin_riscv_sfpload)

typedef float __rvtt_vec_t __attribute__((vector_size(64*4)));

#define sfpi_inline __attribute__((always_inline)) inline

#define __builtin_rvtt_sfpassign_lv(v, in) __builtin_riscv_sfpassign_lv(v, in)
#define __builtin_rvtt_sfpload(mod0, addr) __builtin_riscv_sfpload((void *)ckernel::instrn_buffer, mod0, addr)
#define __builtin_rvtt_sfpload_lv(dst, mod0, addr) __builtin_riscv_sfpload_lv((void *)ckernel::instrn_buffer, dst, mod0, addr)
#define __builtin_rvtt_sfpassignlr(lr) __builtin_riscv_sfpassignlr(lr)
#define __builtin_rvtt_sfpkeepalive(x, n) __builtin_riscv_sfpkeepalive(x, n)

#define __builtin_rvtt_sfploadi(mod0, imm16) __builtin_riscv_sfploadi((void *)ckernel::instrn_buffer, mod0, imm16)
#define __builtin_rvtt_sfploadi_lv(dst, mod0, imm16) __builtin_riscv_sfploadi_lv((void *)ckernel::instrn_buffer, dst, mod0, imm16)
#define __builtin_rvtt_sfpstore(src, mod0, addr) __builtin_riscv_sfpstore((void *)ckernel::instrn_buffer, src, mod0, addr)
#define __builtin_rvtt_sfpmov(src, mod1) __builtin_riscv_sfpmov(src, mod1)
#define __builtin_rvtt_sfpnop() __builtin_riscv_sfpnop()
#define __builtin_rvtt_sfpillegal() __builtin_riscv_sfpillegal()

#define __builtin_rvtt_sfpencc(imm12, mod1) __builtin_riscv_sfpencc(imm12, mod1)
#define __builtin_rvtt_sfppushc() __builtin_riscv_sfppushc()
#define __builtin_rvtt_sfppopc() __builtin_riscv_sfppopc()
#define __builtin_rvtt_sfpsetcc_v(src, mod1) __builtin_riscv_sfpsetcc_v(src, mod1)
#define __builtin_rvtt_sfpsetcc_i(imm12, mod1) __builtin_riscv_sfpsetcc_i(imm12, mod1)
#define __builtin_rvtt_sfpscmp_ex(v, f, mod1) __builtin_riscv_sfpscmp_ex((void*)ckernel::instrn_buffer, v, f, mod1)
#define __builtin_rvtt_sfpvcmp_ex(v1, v2, mod1) __builtin_riscv_sfpvcmp_ex(v1, v2, mod1)
#define __builtin_rvtt_sfpcompc() __builtin_riscv_sfpcompc()

#define __builtin_rvtt_sfpadd(va, vb, mod1) __builtin_riscv_sfpadd(va, vb, mod1)
#define __builtin_rvtt_sfpmul(va, vb, mod1) __builtin_riscv_sfpmul(va, vb, mod1)
#define __builtin_rvtt_sfpmad(va, vb, vc, mod1) __builtin_riscv_sfpmad(va, vb, vc, mod1)

#define __builtin_rvtt_sfpexexp(src, mod1) __builtin_riscv_sfpexexp(src, mod1)
#define __builtin_rvtt_sfpexman(src, mod1) __builtin_riscv_sfpexman(src, mod1)

#define __builtin_rvtt_sfpsetexp_i(imm12, src) __builtin_riscv_sfpsetexp_i((void *)ckernel::instrn_buffer, imm12, src)
#define __builtin_rvtt_sfpsetexp_v(dst, src) __builtin_riscv_sfpsetexp_v(dst, src)

#define __builtin_rvtt_sfpsetman_i(imm12, src) __builtin_riscv_sfpsetman_i((void *)ckernel::instrn_buffer, imm12, src)
#define __builtin_rvtt_sfpsetman_v(dst, src) __builtin_riscv_sfpsetman_v(dst, src)

#define __builtin_rvtt_sfpabs(src, mod1) __builtin_riscv_sfpabs(src, mod1)
#define __builtin_rvtt_sfpand(dst, src) __builtin_riscv_sfpand(dst, src)
#define __builtin_rvtt_sfpor(dst, src) __builtin_riscv_sfpor(dst, src)
#define __builtin_rvtt_sfpnot(src) __builtin_riscv_sfpnot(src)

#define __builtin_rvtt_sfpmuli(dst, imm12, mod1) __builtin_riscv_sfpmuli((void *)ckernel::instrn_buffer, dst, imm12, mod1)
#define __builtin_rvtt_sfpaddi(dst, imm12, mod1) __builtin_riscv_sfpaddi((void *)ckernel::instrn_buffer, dst, imm12, mod1)

#define __builtin_rvtt_sfpdivp2(imm12, src, mod1) __builtin_riscv_sfpdivp2((void *)ckernel::instrn_buffer, imm12, src, mod1)

#define __builtin_rvtt_sfplz(src, mod1) __builtin_riscv_sfplz(src, mod1)

#define __builtin_rvtt_sfpshft_i(dst, imm12) __builtin_riscv_sfpshft_i((void *)ckernel::instrn_buffer, dst, imm12)
#define __builtin_rvtt_sfpshft_v(dst, src) __builtin_riscv_sfpshft_v(dst, src)

#define __builtin_rvtt_sfpiadd_i(imm12, src, mod1) __builtin_riscv_sfpiadd_i((void *)ckernel::instrn_buffer, src, imm12, mod1)
#define __builtin_rvtt_sfpiadd_i_ex(src, imm12, mod1) __builtin_riscv_sfpiadd_i_ex((void *)ckernel::instrn_buffer, src, imm12, mod1)
#define __builtin_rvtt_sfpiadd_v(dst, src, mod1) __builtin_riscv_sfpiadd_v(dst, src, mod1)
#define __builtin_rvtt_sfpiadd_v_ex(dst, src, mod1) __builtin_riscv_sfpiadd_v_ex(dst, src, mod1)

#define __builtin_rvtt_sfpsetsgn_i(imm12, src) __builtin_riscv_sfpsetsgn_i((void *)ckernel::instrn_buffer, imm12, src)
#define __builtin_rvtt_sfpsetsgn_v(dst, src) __builtin_riscv_sfpsetsgn_v(dst, src)

#define __builtin_rvtt_sfplut(l0, l1, l2, dst, mod1) __builtin_riscv_sfplut(l0, l1, l2, dst, mod1)

#endif // __GNUC__

#endif // COMPILE_FOR_EMULE

constexpr unsigned int SFP_DESTREG_STRIDE = 4;

constexpr unsigned int SFPLOAD_MOD0_REBIAS_EXP = 1;
constexpr unsigned int SFPLOAD_MOD0_NOREBIAS_EXP = 2;

constexpr unsigned int SFPSTORE_MOD0_FLOAT_REBIAS_EXP = 0;
constexpr unsigned int SFPSTORE_MOD0_INT = 2;

constexpr unsigned int SFPMOV_MOD1_COMPSIGN = 1;

constexpr unsigned int SFPMAD_MOD1_OFFSET_NONE = 0;
constexpr unsigned int SFPMAD_MOD1_OFFSET_POSH = 1;
constexpr unsigned int SFPMAD_MOD1_OFFSET_NEGH = 3;

constexpr unsigned int SFPLOADI_MOD0_FLOATB = 0;
constexpr unsigned int SFPLOADI_MOD0_FLOATA = 1;
constexpr unsigned int SFPLOADI_MOD0_USHORT = 2;
constexpr unsigned int SFPLOADI_MOD0_SHORT = 4;

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
constexpr unsigned int SFPCMP_EX_MOD1_CC_LT0 = 1;
constexpr unsigned int SFPCMP_EX_MOD1_CC_EQ0 = 3;
constexpr unsigned int SFPCMP_EX_MOD1_CC_GTE0 = 5;
constexpr unsigned int SFPCMP_EX_MOD1_CC_NE0 = 7;
constexpr unsigned int SFPCMP_EX_MOD1_CC_MASK = 7;

constexpr unsigned int SFPSCMP_EX_MOD1_FMT_A = 8;

constexpr unsigned int SFPIADD_EX_MOD1_IS_SUB = 16;

constexpr unsigned int SFPIADD_I_EX_MOD1_SIGNED = 8;
constexpr unsigned int SFPIADD_I_EX_MOD1_IS_12BITS = 32;

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

constexpr unsigned int SFPLZ_MOD1_CC_NONE = 0;
constexpr unsigned int SFPLZ_MOD1_CC_NE0 = 2;
constexpr unsigned int SFPLZ_MOD1_CC_COMP = 8;
constexpr unsigned int SFPLZ_MOD1_CC_EQ0 = 10;

constexpr unsigned int SFPSDIVP2_MOD1_ADD = 1;

constexpr unsigned int SFPLUT_MOD0_BIAS_MASK = 0x3;
constexpr unsigned int SFPLUT_MOD0_BIAS_NONE = 0x0;
constexpr unsigned int SFPLUT_MOD0_BIAS_POS = 0x1;
constexpr unsigned int SFPLUT_MOD0_BIAS_NEG = 0x3;
constexpr unsigned int SFPLUT_MOD0_SGN_UPDATE = 0;
constexpr unsigned int SFPLUT_MOD0_SGN_RETAIN = 4;

constexpr unsigned int CREG_IDX_0 = 4;
constexpr unsigned int CREG_IDX_0P692871094 = 5;
constexpr unsigned int CREG_IDX_NEG_1P00683594 = 6;
constexpr unsigned int CREG_IDX_1P442382813 = 7;
constexpr unsigned int CREG_IDX_0P836914063 = 8;
constexpr unsigned int CREG_IDX_NEG_0P5 = 9;
constexpr unsigned int CREG_IDX_1 = 10;
constexpr unsigned int CREG_IDX_NEG_1 = 11;
constexpr unsigned int CREG_IDX_0P001953125 = 12;
constexpr unsigned int CREG_IDX_NEG_0P67480469 = 13;
constexpr unsigned int CREG_IDX_NEG_0P34472656 = 14;
constexpr unsigned int CREG_IDX_TILEID = 15;

} // namespace sfpi

#endif // ARCH_GRAYSKULL
