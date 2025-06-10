/*
 * SPDX-FileCopyrightText: © 2025 Tenstorrent Inc.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

// Low level tt-insn interface.

#pragma once

#ifdef TT_OP
#error "ckernel_ops.h already included"
#include "stop now, no good will come"
#endif

#include <cstdint>

#if !__riscv_tt_wormhole && !__riscv_tt_blackhole
#error "A TT architecture must be selected"
#include "stop now, no good will come"
#endif

namespace lltt {

enum ExecBool : bool {NoExec, Exec};

template<ExecBool E = NoExec>
[[gnu::always_inline]] inline void
record(unsigned start, unsigned length) {
  __builtin_rvtt_ttreplay(start, length, bool(E), true);
}

[[gnu::always_inline]] inline void replay(unsigned start, unsigned length) {
  __builtin_rvtt_ttreplay(start, length, false, false);
}

[[gnu::always_inline]] constexpr std::uint32_t replay_insn(unsigned start, unsigned length);

[[gnu::always_inline]] inline void insn(uint32_t insn) {
  __builtin_rvtt_ttinsn(&ckernel::instrn_buffer[0], insn);
}

} // namespace 

// Conversion of ckernel_ops files:
// sed -i '/TT_OP/s/ << \([0-9]\+\)) + (/ << \1) | (/g'
// sed -i 's/TT_OP(\(0x[^,]*\), (\(.*\))$/((\1 << 24) | \2/'
// sed -i '/^  /s/ckernel::instrn_buffer\[0\] = \(.*)\) *$/lltt::insn(\1)/'
// sed -i '/^  /s/INSTRUCTION_WORD(TT_OP_\(.*)\) *)/TT_\1/'
#if __riscv_tt_wormhole

#define TT_OP_ADDDMAREG(OpBisConst, ResultRegIndex, OpBRegIndex, OpARegIndex) \
  ((0x58 << 24) | ((OpBisConst) << 23) | ((ResultRegIndex) << 12) | ((OpBRegIndex) << 6) | ((OpARegIndex) << 0))
#define TT_ADDDMAREG_VALID(OpBisConst, ResultRegIndex, OpBRegIndex, OpARegIndex) \
  (ckernel::is_valid(OpBisConst, 1) && ckernel::is_valid(ResultRegIndex, 11) && ckernel::is_valid(OpBRegIndex, 6) && ckernel::is_valid(OpARegIndex, 6))
#define TT_ADDDMAREG(OpBisConst, ResultRegIndex, OpBRegIndex, OpARegIndex) \
  lltt::insn(TT_OP_ADDDMAREG(OpBisConst, ResultRegIndex, OpBRegIndex, OpARegIndex))
#define TTI_ADDDMAREG(OpBisConst, ResultRegIndex, OpBRegIndex, OpARegIndex) \
  TT_ADDDMAREG(OpBisConst, ResultRegIndex, OpBRegIndex, OpARegIndex)

#define TT_OP_ADDRCRXY(CntSetMask, Ch1_Y, Ch1_X, Ch0_Y, Ch0_X, BitMask) \
  ((0x53 << 24) | ((CntSetMask) << 21) | ((Ch1_Y) << 15) | ((Ch1_X) << 12) | ((Ch0_Y) << 9) | ((Ch0_X) << 6) | ((BitMask) << 0))
#define TT_ADDRCRXY_VALID(CntSetMask, Ch1_Y, Ch1_X, Ch0_Y, Ch0_X, BitMask) \
  (ckernel::is_valid(CntSetMask, 3) && ckernel::is_valid(Ch1_Y, 6) && ckernel::is_valid(Ch1_X, 3) && ckernel::is_valid(Ch0_Y, 3) && ckernel::is_valid(Ch0_X, 3) && ckernel::is_valid(BitMask, 6))
#define TT_ADDRCRXY(CntSetMask, Ch1_Y, Ch1_X, Ch0_Y, Ch0_X, BitMask) \
  lltt::insn(TT_OP_ADDRCRXY(CntSetMask, Ch1_Y, Ch1_X, Ch0_Y, Ch0_X, BitMask))
#define TTI_ADDRCRXY(CntSetMask, Ch1_Y, Ch1_X, Ch0_Y, Ch0_X, BitMask) \
  TT_ADDRCRXY(CntSetMask, Ch1_Y, Ch1_X, Ch0_Y, Ch0_X, BitMask)

#define TT_OP_ADDRCRZW(CntSetMask, Ch1_Y, Ch1_X, Ch0_Y, Ch0_X, BitMask) \
  ((0x56 << 24) | ((CntSetMask) << 21) | ((Ch1_Y) << 15) | ((Ch1_X) << 12) | ((Ch0_Y) << 9) | ((Ch0_X) << 6) | ((BitMask) << 0))
#define TT_ADDRCRZW_VALID(CntSetMask, Ch1_Y, Ch1_X, Ch0_Y, Ch0_X, BitMask) \
  (ckernel::is_valid(CntSetMask, 3) && ckernel::is_valid(Ch1_Y, 6) && ckernel::is_valid(Ch1_X, 3) && ckernel::is_valid(Ch0_Y, 3) && ckernel::is_valid(Ch0_X, 3) && ckernel::is_valid(BitMask, 6))
#define TT_ADDRCRZW(CntSetMask, Ch1_Y, Ch1_X, Ch0_Y, Ch0_X, BitMask) \
  lltt::insn(TT_OP_ADDRCRZW(CntSetMask, Ch1_Y, Ch1_X, Ch0_Y, Ch0_X, BitMask))
#define TTI_ADDRCRZW(CntSetMask, Ch1_Y, Ch1_X, Ch0_Y, Ch0_X, BitMask) \
  TT_ADDRCRZW(CntSetMask, Ch1_Y, Ch1_X, Ch0_Y, Ch0_X, BitMask)

#define TT_OP_APOOL3S1(clear_dvalid, addr_mode, index_en, dst) \
  ((0x25 << 24) | ((clear_dvalid) << 22) | ((addr_mode) << 15) | ((index_en) << 14) | ((dst) << 0))
#define TT_APOOL3S1_VALID(clear_dvalid, addr_mode, index_en, dst) \
  (ckernel::is_valid(clear_dvalid, 2) && ckernel::is_valid(addr_mode, 7) && ckernel::is_valid(index_en, 1) && ckernel::is_valid(dst, 14))
#define TT_APOOL3S1(clear_dvalid, addr_mode, index_en, dst) \
  lltt::insn(TT_OP_APOOL3S1(clear_dvalid, addr_mode, index_en, dst))
#define TTI_APOOL3S1(clear_dvalid, addr_mode, index_en, dst) \
  TT_APOOL3S1(clear_dvalid, addr_mode, index_en, dst)

#define TT_OP_APOOL3S2(clear_dvalid, addr_mode, index_en, dst) \
  ((0x32 << 24) | ((clear_dvalid) << 22) | ((addr_mode) << 15) | ((index_en) << 14) | ((dst) << 0))
#define TT_APOOL3S2_VALID(clear_dvalid, addr_mode, index_en, dst) \
  (ckernel::is_valid(clear_dvalid, 2) && ckernel::is_valid(addr_mode, 7) && ckernel::is_valid(index_en, 1) && ckernel::is_valid(dst, 14))
#define TT_APOOL3S2(clear_dvalid, addr_mode, index_en, dst) \
  lltt::insn(TT_OP_APOOL3S2(clear_dvalid, addr_mode, index_en, dst))
#define TTI_APOOL3S2(clear_dvalid, addr_mode, index_en, dst) \
  TT_APOOL3S2(clear_dvalid, addr_mode, index_en, dst)

#define TT_OP_ATCAS(MemHierSel, SwapVal, CmpVal, Sel32b, DataRegIndex, AddrRegIndex) \
  ((0x64 << 24) | ((MemHierSel) << 23) | ((SwapVal) << 18) | ((CmpVal) << 14) | ((Sel32b) << 12) | ((DataRegIndex) << 6) | ((AddrRegIndex) << 0))
#define TT_ATCAS_VALID(MemHierSel, SwapVal, CmpVal, Sel32b, DataRegIndex, AddrRegIndex) \
  (ckernel::is_valid(MemHierSel, 1) && ckernel::is_valid(SwapVal, 5) && ckernel::is_valid(CmpVal, 4) && ckernel::is_valid(Sel32b, 2) && ckernel::is_valid(DataRegIndex, 6) && ckernel::is_valid(AddrRegIndex, 6))
#define TT_ATCAS(MemHierSel, SwapVal, CmpVal, Sel32b, DataRegIndex, AddrRegIndex) \
  lltt::insn(TT_OP_ATCAS(MemHierSel, SwapVal, CmpVal, Sel32b, DataRegIndex, AddrRegIndex))
#define TTI_ATCAS(MemHierSel, SwapVal, CmpVal, Sel32b, DataRegIndex, AddrRegIndex) \
  TT_ATCAS(MemHierSel, SwapVal, CmpVal, Sel32b, DataRegIndex, AddrRegIndex)

#define TT_OP_ATGETM(mutex_index) \
  ((0xa0 << 24) | ((mutex_index) << 0))
#define TT_ATGETM_VALID(mutex_index) \
  (ckernel::is_valid(mutex_index, 24))
#define TT_ATGETM(mutex_index) \
  lltt::insn(TT_OP_ATGETM(mutex_index))
#define TTI_ATGETM(mutex_index) \
  TT_ATGETM(mutex_index)

#define TT_OP_ATINCGET(MemHierSel, WrapVal, Sel32b, DataRegIndex, AddrRegIndex) \
  ((0x61 << 24) | ((MemHierSel) << 23) | ((WrapVal) << 14) | ((Sel32b) << 12) | ((DataRegIndex) << 6) | ((AddrRegIndex) << 0))
#define TT_ATINCGET_VALID(MemHierSel, WrapVal, Sel32b, DataRegIndex, AddrRegIndex) \
  (ckernel::is_valid(MemHierSel, 1) && ckernel::is_valid(WrapVal, 9) && ckernel::is_valid(Sel32b, 2) && ckernel::is_valid(DataRegIndex, 6) && ckernel::is_valid(AddrRegIndex, 6))
#define TT_ATINCGET(MemHierSel, WrapVal, Sel32b, DataRegIndex, AddrRegIndex) \
  lltt::insn(TT_OP_ATINCGET(MemHierSel, WrapVal, Sel32b, DataRegIndex, AddrRegIndex))
#define TTI_ATINCGET(MemHierSel, WrapVal, Sel32b, DataRegIndex, AddrRegIndex) \
  TT_ATINCGET(MemHierSel, WrapVal, Sel32b, DataRegIndex, AddrRegIndex)

#define TT_OP_ATINCGETPTR(MemHierSel, NoIncr, IncrVal, WrapVal, Sel32b, DataRegIndex, AddrRegIndex) \
  ((0x62 << 24) | ((MemHierSel) << 23) | ((NoIncr) << 22) | ((IncrVal) << 18) | ((WrapVal) << 14) | ((Sel32b) << 12) | ((DataRegIndex) << 6) | ((AddrRegIndex) << 0))
#define TT_ATINCGETPTR_VALID(MemHierSel, NoIncr, IncrVal, WrapVal, Sel32b, DataRegIndex, AddrRegIndex) \
  (ckernel::is_valid(MemHierSel, 1) && ckernel::is_valid(NoIncr, 1) && ckernel::is_valid(IncrVal, 4) && ckernel::is_valid(WrapVal, 4) && ckernel::is_valid(Sel32b, 2) && ckernel::is_valid(DataRegIndex, 6) && ckernel::is_valid(AddrRegIndex, 6))
#define TT_ATINCGETPTR(MemHierSel, NoIncr, IncrVal, WrapVal, Sel32b, DataRegIndex, AddrRegIndex) \
  lltt::insn(TT_OP_ATINCGETPTR(MemHierSel, NoIncr, IncrVal, WrapVal, Sel32b, DataRegIndex, AddrRegIndex))
#define TTI_ATINCGETPTR(MemHierSel, NoIncr, IncrVal, WrapVal, Sel32b, DataRegIndex, AddrRegIndex) \
  TT_ATINCGETPTR(MemHierSel, NoIncr, IncrVal, WrapVal, Sel32b, DataRegIndex, AddrRegIndex)

#define TT_OP_ATRELM(mutex_index) \
  ((0xa1 << 24) | ((mutex_index) << 0))
#define TT_ATRELM_VALID(mutex_index) \
  (ckernel::is_valid(mutex_index, 24))
#define TT_ATRELM(mutex_index) \
  lltt::insn(TT_OP_ATRELM(mutex_index))
#define TTI_ATRELM(mutex_index) \
  TT_ATRELM(mutex_index)

#define TT_OP_ATSWAP(MemHierSel, SwapMask, DataRegIndex, AddrRegIndex) \
  ((0x63 << 24) | ((MemHierSel) << 23) | ((SwapMask) << 14) | ((DataRegIndex) << 6) | ((AddrRegIndex) << 0))
#define TT_ATSWAP_VALID(MemHierSel, SwapMask, DataRegIndex, AddrRegIndex) \
  (ckernel::is_valid(MemHierSel, 1) && ckernel::is_valid(SwapMask, 9) && ckernel::is_valid(DataRegIndex, 8) && ckernel::is_valid(AddrRegIndex, 6))
#define TT_ATSWAP(MemHierSel, SwapMask, DataRegIndex, AddrRegIndex) \
  lltt::insn(TT_OP_ATSWAP(MemHierSel, SwapMask, DataRegIndex, AddrRegIndex))
#define TTI_ATSWAP(MemHierSel, SwapMask, DataRegIndex, AddrRegIndex) \
  TT_ATSWAP(MemHierSel, SwapMask, DataRegIndex, AddrRegIndex)

#define TT_OP_BITWOPDMAREG(OpBisConst, OpSel, ResultRegIndex, OpBRegIndex, OpARegIndex) \
  ((0x5b << 24) | ((OpBisConst) << 23) | ((OpSel) << 18) | ((ResultRegIndex) << 12) | ((OpBRegIndex) << 6) | ((OpARegIndex) << 0))
#define TT_BITWOPDMAREG_VALID(OpBisConst, OpSel, ResultRegIndex, OpBRegIndex, OpARegIndex) \
  (ckernel::is_valid(OpBisConst, 1) && ckernel::is_valid(OpSel, 5) && ckernel::is_valid(ResultRegIndex, 6) && ckernel::is_valid(OpBRegIndex, 6) && ckernel::is_valid(OpARegIndex, 6))
#define TT_BITWOPDMAREG(OpBisConst, OpSel, ResultRegIndex, OpBRegIndex, OpARegIndex) \
  lltt::insn(TT_OP_BITWOPDMAREG(OpBisConst, OpSel, ResultRegIndex, OpBRegIndex, OpARegIndex))
#define TTI_BITWOPDMAREG(OpBisConst, OpSel, ResultRegIndex, OpBRegIndex, OpARegIndex) \
  TT_BITWOPDMAREG(OpBisConst, OpSel, ResultRegIndex, OpBRegIndex, OpARegIndex)

#define TT_OP_CLEARDVALID(cleardvalid, reset) \
  ((0x36 << 24) | ((cleardvalid) << 22) | ((reset) << 0))
#define TT_CLEARDVALID_VALID(cleardvalid, reset) \
  (ckernel::is_valid(cleardvalid, 2) && ckernel::is_valid(reset, 22))
#define TT_CLEARDVALID(cleardvalid, reset) \
  lltt::insn(TT_OP_CLEARDVALID(cleardvalid, reset))
#define TTI_CLEARDVALID(cleardvalid, reset) \
  TT_CLEARDVALID(cleardvalid, reset)

#define TT_OP_CLREXPHIST\
  TT_OP(0x21, 0)
#define TTI_CLREXPHIST\
  INSTRUCTION_WORD(TT_OP_CLREXPHIST)

#define TT_OP_CMPDMAREG(OpBisConst, OpSel, ResultRegIndex, OpBRegIndex, OpARegIndex) \
  ((0x5d << 24) | ((OpBisConst) << 23) | ((OpSel) << 18) | ((ResultRegIndex) << 12) | ((OpBRegIndex) << 6) | ((OpARegIndex) << 0))
#define TT_CMPDMAREG_VALID(OpBisConst, OpSel, ResultRegIndex, OpBRegIndex, OpARegIndex) \
  (ckernel::is_valid(OpBisConst, 1) && ckernel::is_valid(OpSel, 5) && ckernel::is_valid(ResultRegIndex, 6) && ckernel::is_valid(OpBRegIndex, 6) && ckernel::is_valid(OpARegIndex, 6))
#define TT_CMPDMAREG(OpBisConst, OpSel, ResultRegIndex, OpBRegIndex, OpARegIndex) \
  lltt::insn(TT_OP_CMPDMAREG(OpBisConst, OpSel, ResultRegIndex, OpBRegIndex, OpARegIndex))
#define TTI_CMPDMAREG(OpBisConst, OpSel, ResultRegIndex, OpBRegIndex, OpARegIndex) \
  TT_CMPDMAREG(OpBisConst, OpSel, ResultRegIndex, OpBRegIndex, OpARegIndex)

#define TT_OP_CONV3S1(clear_dvalid, rotate_weights, addr_mode, dst) \
  ((0x22 << 24) | ((clear_dvalid) << 22) | ((rotate_weights) << 17) | ((addr_mode) << 15) | ((dst) << 0))
#define TT_CONV3S1_VALID(clear_dvalid, rotate_weights, addr_mode, dst) \
  (ckernel::is_valid(clear_dvalid, 2) && ckernel::is_valid(rotate_weights, 5) && ckernel::is_valid(addr_mode, 2) && ckernel::is_valid(dst, 15))
#define TT_CONV3S1(clear_dvalid, rotate_weights, addr_mode, dst) \
  lltt::insn(TT_OP_CONV3S1(clear_dvalid, rotate_weights, addr_mode, dst))
#define TTI_CONV3S1(clear_dvalid, rotate_weights, addr_mode, dst) \
  TT_CONV3S1(clear_dvalid, rotate_weights, addr_mode, dst)

#define TT_OP_CONV3S2(clear_dvalid, rotate_weights, addr_mode, dst) \
  ((0x23 << 24) | ((clear_dvalid) << 22) | ((rotate_weights) << 17) | ((addr_mode) << 15) | ((dst) << 0))
#define TT_CONV3S2_VALID(clear_dvalid, rotate_weights, addr_mode, dst) \
  (ckernel::is_valid(clear_dvalid, 2) && ckernel::is_valid(rotate_weights, 5) && ckernel::is_valid(addr_mode, 2) && ckernel::is_valid(dst, 15))
#define TT_CONV3S2(clear_dvalid, rotate_weights, addr_mode, dst) \
  lltt::insn(TT_OP_CONV3S2(clear_dvalid, rotate_weights, addr_mode, dst))
#define TTI_CONV3S2(clear_dvalid, rotate_weights, addr_mode, dst) \
  TT_CONV3S2(clear_dvalid, rotate_weights, addr_mode, dst)

#define TT_OP_DMANOP\
  TT_OP(0x60, 0)
#define TTI_DMANOP\
  INSTRUCTION_WORD(TT_OP_DMANOP)

#define TT_OP_DOTPV(clear_dvalid, dest_accum_en, instr_mod19, addr_mode, dst) \
  ((0x29 << 24) | ((clear_dvalid) << 22) | ((dest_accum_en) << 21) | ((instr_mod19) << 19) | ((addr_mode) << 15) | ((dst) << 0))
#define TT_DOTPV_VALID(clear_dvalid, dest_accum_en, instr_mod19, addr_mode, dst) \
  (ckernel::is_valid(clear_dvalid, 2) && ckernel::is_valid(dest_accum_en, 1) && ckernel::is_valid(instr_mod19, 2) && ckernel::is_valid(addr_mode, 4) && ckernel::is_valid(dst, 15))
#define TT_DOTPV(clear_dvalid, dest_accum_en, instr_mod19, addr_mode, dst) \
  lltt::insn(TT_OP_DOTPV(clear_dvalid, dest_accum_en, instr_mod19, addr_mode, dst))
#define TTI_DOTPV(clear_dvalid, dest_accum_en, instr_mod19, addr_mode, dst) \
  TT_DOTPV(clear_dvalid, dest_accum_en, instr_mod19, addr_mode, dst)

#define TT_OP_ELWADD(clear_dvalid, dest_accum_en, instr_mod19, addr_mode, dst) \
  ((0x28 << 24) | ((clear_dvalid) << 22) | ((dest_accum_en) << 21) | ((instr_mod19) << 19) | ((addr_mode) << 15) | ((dst) << 0))
#define TT_ELWADD_VALID(clear_dvalid, dest_accum_en, instr_mod19, addr_mode, dst) \
  (ckernel::is_valid(clear_dvalid, 2) && ckernel::is_valid(dest_accum_en, 1) && ckernel::is_valid(instr_mod19, 2) && ckernel::is_valid(addr_mode, 4) && ckernel::is_valid(dst, 15))
#define TT_ELWADD(clear_dvalid, dest_accum_en, instr_mod19, addr_mode, dst) \
  lltt::insn(TT_OP_ELWADD(clear_dvalid, dest_accum_en, instr_mod19, addr_mode, dst))
#define TTI_ELWADD(clear_dvalid, dest_accum_en, instr_mod19, addr_mode, dst) \
  TT_ELWADD(clear_dvalid, dest_accum_en, instr_mod19, addr_mode, dst)

#define TT_OP_ELWMUL(clear_dvalid, dest_accum_en, instr_mod19, addr_mode, dst) \
  ((0x27 << 24) | ((clear_dvalid) << 22) | ((dest_accum_en) << 21) | ((instr_mod19) << 19) | ((addr_mode) << 15) | ((dst) << 0))
#define TT_ELWMUL_VALID(clear_dvalid, dest_accum_en, instr_mod19, addr_mode, dst) \
  (ckernel::is_valid(clear_dvalid, 2) && ckernel::is_valid(dest_accum_en, 1) && ckernel::is_valid(instr_mod19, 2) && ckernel::is_valid(addr_mode, 4) && ckernel::is_valid(dst, 15))
#define TT_ELWMUL(clear_dvalid, dest_accum_en, instr_mod19, addr_mode, dst) \
  lltt::insn(TT_OP_ELWMUL(clear_dvalid, dest_accum_en, instr_mod19, addr_mode, dst))
#define TTI_ELWMUL(clear_dvalid, dest_accum_en, instr_mod19, addr_mode, dst) \
  TT_ELWMUL(clear_dvalid, dest_accum_en, instr_mod19, addr_mode, dst)

#define TT_OP_ELWSUB(clear_dvalid, dest_accum_en, instr_mod19, addr_mode, dst) \
  ((0x30 << 24) | ((clear_dvalid) << 22) | ((dest_accum_en) << 21) | ((instr_mod19) << 19) | ((addr_mode) << 15) | ((dst) << 0))
#define TT_ELWSUB_VALID(clear_dvalid, dest_accum_en, instr_mod19, addr_mode, dst) \
  (ckernel::is_valid(clear_dvalid, 2) && ckernel::is_valid(dest_accum_en, 1) && ckernel::is_valid(instr_mod19, 2) && ckernel::is_valid(addr_mode, 4) && ckernel::is_valid(dst, 15))
#define TT_ELWSUB(clear_dvalid, dest_accum_en, instr_mod19, addr_mode, dst) \
  lltt::insn(TT_OP_ELWSUB(clear_dvalid, dest_accum_en, instr_mod19, addr_mode, dst))
#define TTI_ELWSUB(clear_dvalid, dest_accum_en, instr_mod19, addr_mode, dst) \
  TT_ELWSUB(clear_dvalid, dest_accum_en, instr_mod19, addr_mode, dst)

#define TT_OP_FLUSHDMA(FlushSpec) \
  ((0x46 << 24) | ((FlushSpec) << 0))
#define TT_FLUSHDMA_VALID(FlushSpec) \
  (ckernel::is_valid(FlushSpec, 24))
#define TT_FLUSHDMA(FlushSpec) \
  lltt::insn(TT_OP_FLUSHDMA(FlushSpec))
#define TTI_FLUSHDMA(FlushSpec) \
  TT_FLUSHDMA(FlushSpec)

#define TT_OP_GAPOOL(clear_dvalid, instr_mod19, addr_mode, max_pool_index_en, dst) \
  ((0x34 << 24) | ((clear_dvalid) << 22) | ((instr_mod19) << 19) | ((addr_mode) << 15) | ((max_pool_index_en) << 14) | ((dst) << 0))
#define TT_GAPOOL_VALID(clear_dvalid, instr_mod19, addr_mode, max_pool_index_en, dst) \
  (ckernel::is_valid(clear_dvalid, 2) && ckernel::is_valid(instr_mod19, 3) && ckernel::is_valid(addr_mode, 4) && ckernel::is_valid(max_pool_index_en, 1) && ckernel::is_valid(dst, 14))
#define TT_GAPOOL(clear_dvalid, instr_mod19, addr_mode, max_pool_index_en, dst) \
  lltt::insn(TT_OP_GAPOOL(clear_dvalid, instr_mod19, addr_mode, max_pool_index_en, dst))
#define TTI_GAPOOL(clear_dvalid, instr_mod19, addr_mode, max_pool_index_en, dst) \
  TT_GAPOOL(clear_dvalid, instr_mod19, addr_mode, max_pool_index_en, dst)

#define TT_OP_GATESRCRST(reset_srcb_gate_control, reset_srca_gate_control) \
  ((0x35 << 24) | ((reset_srcb_gate_control) << 1) | ((reset_srca_gate_control) << 0))
#define TT_GATESRCRST_VALID(reset_srcb_gate_control, reset_srca_gate_control) \
  (ckernel::is_valid(reset srcb gate control, 23) && ckernel::is_valid(reset srca gate control, 1))
#define TT_GATESRCRST(reset_srcb_gate_control, reset_srca_gate_control) \
  lltt::insn(TT_OP_GATESRCRST(reset_srcb_gate_control, reset_srca_gate_control))
#define TTI_GATESRCRST(reset_srcb_gate_control, reset_srca_gate_control) \
  TT_GATESRCRST(reset_srcb_gate_control, reset_srca_gate_control)

#define TT_OP_GMPOOL(clear_dvalid, instr_mod19, addr_mode, max_pool_index_en, dst) \
  ((0x33 << 24) | ((clear_dvalid) << 22) | ((instr_mod19) << 19) | ((addr_mode) << 15) | ((max_pool_index_en) << 14) | ((dst) << 0))
#define TT_GMPOOL_VALID(clear_dvalid, instr_mod19, addr_mode, max_pool_index_en, dst) \
  (ckernel::is_valid(clear_dvalid, 2) && ckernel::is_valid(instr_mod19, 3) && ckernel::is_valid(addr_mode, 4) && ckernel::is_valid(max_pool_index_en, 1) && ckernel::is_valid(dst, 14))
#define TT_GMPOOL(clear_dvalid, instr_mod19, addr_mode, max_pool_index_en, dst) \
  lltt::insn(TT_OP_GMPOOL(clear_dvalid, instr_mod19, addr_mode, max_pool_index_en, dst))
#define TTI_GMPOOL(clear_dvalid, instr_mod19, addr_mode, max_pool_index_en, dst) \
  TT_GMPOOL(clear_dvalid, instr_mod19, addr_mode, max_pool_index_en, dst)

#define TT_OP_INCADCXY(CntSetMask, Ch1_Y, Ch1_X, Ch0_Y, Ch0_X) \
  ((0x52 << 24) | ((CntSetMask) << 21) | ((Ch1_Y) << 15) | ((Ch1_X) << 12) | ((Ch0_Y) << 9) | ((Ch0_X) << 6))
#define TT_INCADCXY_VALID(CntSetMask, Ch1_Y, Ch1_X, Ch0_Y, Ch0_X) \
  (ckernel::is_valid(CntSetMask, 3) && ckernel::is_valid(Ch1_Y, 6) && ckernel::is_valid(Ch1_X, 3) && ckernel::is_valid(Ch0_Y, 3) && ckernel::is_valid(Ch0_X, 3))
#define TT_INCADCXY(CntSetMask, Ch1_Y, Ch1_X, Ch0_Y, Ch0_X) \
  lltt::insn(TT_OP_INCADCXY(CntSetMask, Ch1_Y, Ch1_X, Ch0_Y, Ch0_X))
#define TTI_INCADCXY(CntSetMask, Ch1_Y, Ch1_X, Ch0_Y, Ch0_X) \
  TT_INCADCXY(CntSetMask, Ch1_Y, Ch1_X, Ch0_Y, Ch0_X)

#define TT_OP_INCADCZW(CntSetMask, Ch1_Y, Ch1_X, Ch0_Y, Ch0_X) \
  ((0x55 << 24) | ((CntSetMask) << 21) | ((Ch1_Y) << 15) | ((Ch1_X) << 12) | ((Ch0_Y) << 9) | ((Ch0_X) << 6))
#define TT_INCADCZW_VALID(CntSetMask, Ch1_Y, Ch1_X, Ch0_Y, Ch0_X) \
  (ckernel::is_valid(CntSetMask, 3) && ckernel::is_valid(Ch1_Y, 6) && ckernel::is_valid(Ch1_X, 3) && ckernel::is_valid(Ch0_Y, 3) && ckernel::is_valid(Ch0_X, 3))
#define TT_INCADCZW(CntSetMask, Ch1_Y, Ch1_X, Ch0_Y, Ch0_X) \
  lltt::insn(TT_OP_INCADCZW(CntSetMask, Ch1_Y, Ch1_X, Ch0_Y, Ch0_X))
#define TTI_INCADCZW(CntSetMask, Ch1_Y, Ch1_X, Ch0_Y, Ch0_X) \
  TT_INCADCZW(CntSetMask, Ch1_Y, Ch1_X, Ch0_Y, Ch0_X)

#define TT_OP_INCRWC(rwc_cr, rwc_d, rwc_b, rwc_a) \
  ((0x38 << 24) | ((rwc_cr) << 18) | ((rwc_d) << 14) | ((rwc_b) << 10) | ((rwc_a) << 6))
#define TT_INCRWC_VALID(rwc_cr, rwc_d, rwc_b, rwc_a) \
  (ckernel::is_valid(rwc_cr, 6) && ckernel::is_valid(rwc_d, 4) && ckernel::is_valid(rwc_b, 4) && ckernel::is_valid(rwc_a, 4))
#define TT_INCRWC(rwc_cr, rwc_d, rwc_b, rwc_a) \
  lltt::insn(TT_OP_INCRWC(rwc_cr, rwc_d, rwc_b, rwc_a))
#define TTI_INCRWC(rwc_cr, rwc_d, rwc_b, rwc_a) \
  TT_INCRWC(rwc_cr, rwc_d, rwc_b, rwc_a)

#define TT_OP_LOADIND(SizeSel, OffsetIndex, AutoIncSpec, DataRegIndex, AddrRegIndex) \
  ((0x49 << 24) | ((SizeSel) << 22) | ((OffsetIndex) << 14) | ((AutoIncSpec) << 12) | ((DataRegIndex) << 6) | ((AddrRegIndex) << 0))
#define TT_LOADIND_VALID(SizeSel, OffsetIndex, AutoIncSpec, DataRegIndex, AddrRegIndex) \
  (ckernel::is_valid(SizeSel, 2) && ckernel::is_valid(OffsetIndex, 8) && ckernel::is_valid(AutoIncSpec, 2) && ckernel::is_valid(DataRegIndex, 6) && ckernel::is_valid(AddrRegIndex, 6))
#define TT_LOADIND(SizeSel, OffsetIndex, AutoIncSpec, DataRegIndex, AddrRegIndex) \
  lltt::insn(TT_OP_LOADIND(SizeSel, OffsetIndex, AutoIncSpec, DataRegIndex, AddrRegIndex))
#define TTI_LOADIND(SizeSel, OffsetIndex, AutoIncSpec, DataRegIndex, AddrRegIndex) \
  TT_LOADIND(SizeSel, OffsetIndex, AutoIncSpec, DataRegIndex, AddrRegIndex)

#define TT_OP_LOADREG(TdmaDataRegIndex, RegAddr) \
  ((0x68 << 24) | ((TdmaDataRegIndex) << 18) | ((RegAddr) << 0))
#define TT_LOADREG_VALID(TdmaDataRegIndex, RegAddr) \
  (ckernel::is_valid(TdmaDataRegIndex, 6) && ckernel::is_valid(RegAddr, 18))
#define TT_LOADREG(TdmaDataRegIndex, RegAddr) \
  lltt::insn(TT_OP_LOADREG(TdmaDataRegIndex, RegAddr))
#define TTI_LOADREG(TdmaDataRegIndex, RegAddr) \
  TT_LOADREG(TdmaDataRegIndex, RegAddr)

#define TT_OP_MFCONV3S1(clear_dvalid, rotate_weights, addr_mode, dst) \
  ((0x3a << 24) | ((clear_dvalid) << 22) | ((rotate_weights) << 17) | ((addr_mode) << 15) | ((dst) << 0))
#define TT_MFCONV3S1_VALID(clear_dvalid, rotate_weights, addr_mode, dst) \
  (ckernel::is_valid(clear_dvalid, 2) && ckernel::is_valid(rotate_weights, 5) && ckernel::is_valid(addr_mode, 2) && ckernel::is_valid(dst, 15))
#define TT_MFCONV3S1(clear_dvalid, rotate_weights, addr_mode, dst) \
  lltt::insn(TT_OP_MFCONV3S1(clear_dvalid, rotate_weights, addr_mode, dst))
#define TTI_MFCONV3S1(clear_dvalid, rotate_weights, addr_mode, dst) \
  TT_MFCONV3S1(clear_dvalid, rotate_weights, addr_mode, dst)

#define TT_OP_MOP(mop_type, loop_count, zmask_lo16) \
  ((0x01 << 24) | ((mop_type) << 23) | ((loop_count) << 16) | ((zmask_lo16) << 0))
#define TT_MOP_VALID(mop_type, loop_count, zmask_lo16) \
  (ckernel::is_valid(mop_type, 1) && ckernel::is_valid(loop_count, 7) && ckernel::is_valid(zmask_lo16, 16))
#define TT_MOP(mop_type, loop_count, zmask_lo16) \
  lltt::insn(TT_OP_MOP(mop_type, loop_count, zmask_lo16))
#define TTI_MOP(mop_type, loop_count, zmask_lo16) \
  TT_MOP(mop_type, loop_count, zmask_lo16)

#define TT_OP_MOP_CFG(zmask_hi16) \
  ((0x03 << 24) | ((zmask_hi16) << 0))
#define TT_MOP_CFG_VALID(zmask_hi16) \
  (ckernel::is_valid(zmask_hi16, 24))
#define TT_MOP_CFG(zmask_hi16) \
  lltt::insn(TT_OP_MOP_CFG(zmask_hi16))
#define TTI_MOP_CFG(zmask_hi16) \
  TT_MOP_CFG(zmask_hi16)

#define TT_OP_MOVA2D(dest_32b_lo, src, addr_mode, instr_mod, dst) \
  ((0x12 << 24) | ((dest_32b_lo) << 23) | ((src) << 17) | ((addr_mode) << 15) | ((instr_mod) << 12) | ((dst) << 0))
#define TT_MOVA2D_VALID(dest_32b_lo, src, addr_mode, instr_mod, dst) \
  (ckernel::is_valid(dest_32b_lo, 1) && ckernel::is_valid(src, 6) && ckernel::is_valid(addr_mode, 2) && ckernel::is_valid(instr_mod, 3) && ckernel::is_valid(dst, 12))
#define TT_MOVA2D(dest_32b_lo, src, addr_mode, instr_mod, dst) \
  lltt::insn(TT_OP_MOVA2D(dest_32b_lo, src, addr_mode, instr_mod, dst))
#define TTI_MOVA2D(dest_32b_lo, src, addr_mode, instr_mod, dst) \
  TT_MOVA2D(dest_32b_lo, src, addr_mode, instr_mod, dst)

#define TT_OP_MOVB2A(srca, addr_mode, instr_mod, srcb) \
  ((0x0b << 24) | ((srca) << 17) | ((addr_mode) << 15) | ((instr_mod) << 12) | ((srcb) << 0))
#define TT_MOVB2A_VALID(srca, addr_mode, instr_mod, srcb) \
  (ckernel::is_valid(srca, 7) && ckernel::is_valid(addr_mode, 2) && ckernel::is_valid(instr_mod, 3) && ckernel::is_valid(srcb, 12))
#define TT_MOVB2A(srca, addr_mode, instr_mod, srcb) \
  lltt::insn(TT_OP_MOVB2A(srca, addr_mode, instr_mod, srcb))
#define TTI_MOVB2A(srca, addr_mode, instr_mod, srcb) \
  TT_MOVB2A(srca, addr_mode, instr_mod, srcb)

#define TT_OP_MOVB2D(dest_32b_lo, src, addr_mode, instr_mod, dst) \
  ((0x13 << 24) | ((dest_32b_lo) << 23) | ((src) << 17) | ((addr_mode) << 15) | ((instr_mod) << 12) | ((dst) << 0))
#define TT_MOVB2D_VALID(dest_32b_lo, src, addr_mode, instr_mod, dst) \
  (ckernel::is_valid(dest_32b_lo, 1) && ckernel::is_valid(src, 6) && ckernel::is_valid(addr_mode, 2) && ckernel::is_valid(instr_mod, 3) && ckernel::is_valid(dst, 12))
#define TT_MOVB2D(dest_32b_lo, src, addr_mode, instr_mod, dst) \
  lltt::insn(TT_OP_MOVB2D(dest_32b_lo, src, addr_mode, instr_mod, dst))
#define TTI_MOVB2D(dest_32b_lo, src, addr_mode, instr_mod, dst) \
  TT_MOVB2D(dest_32b_lo, src, addr_mode, instr_mod, dst)

#define TT_OP_MOVD2A(dest_32b_lo, src, addr_mode, instr_mod, dst) \
  ((0x08 << 24) | ((dest_32b_lo) << 23) | ((src) << 17) | ((addr_mode) << 15) | ((instr_mod) << 12) | ((dst) << 0))
#define TT_MOVD2A_VALID(dest_32b_lo, src, addr_mode, instr_mod, dst) \
  (ckernel::is_valid(dest_32b_lo, 1) && ckernel::is_valid(src, 6) && ckernel::is_valid(addr_mode, 2) && ckernel::is_valid(instr_mod, 3) && ckernel::is_valid(dst, 12))
#define TT_MOVD2A(dest_32b_lo, src, addr_mode, instr_mod, dst) \
  lltt::insn(TT_OP_MOVD2A(dest_32b_lo, src, addr_mode, instr_mod, dst))
#define TTI_MOVD2A(dest_32b_lo, src, addr_mode, instr_mod, dst) \
  TT_MOVD2A(dest_32b_lo, src, addr_mode, instr_mod, dst)

#define TT_OP_MOVD2B(dest_32b_lo, src, addr_mode, instr_mod, dst) \
  ((0x0a << 24) | ((dest_32b_lo) << 23) | ((src) << 17) | ((addr_mode) << 15) | ((instr_mod) << 12) | ((dst) << 0))
#define TT_MOVD2B_VALID(dest_32b_lo, src, addr_mode, instr_mod, dst) \
  (ckernel::is_valid(dest_32b_lo, 1) && ckernel::is_valid(src, 6) && ckernel::is_valid(addr_mode, 2) && ckernel::is_valid(instr_mod, 3) && ckernel::is_valid(dst, 12))
#define TT_MOVD2B(dest_32b_lo, src, addr_mode, instr_mod, dst) \
  lltt::insn(TT_OP_MOVD2B(dest_32b_lo, src, addr_mode, instr_mod, dst))
#define TTI_MOVD2B(dest_32b_lo, src, addr_mode, instr_mod, dst) \
  TT_MOVD2B(dest_32b_lo, src, addr_mode, instr_mod, dst)

#define TT_OP_MOVDBGA2D(dest_32b_lo, src, addr_mode, instr_mod, dst) \
  ((0x09 << 24) | ((dest_32b_lo) << 23) | ((src) << 17) | ((addr_mode) << 15) | ((instr_mod) << 12) | ((dst) << 0))
#define TT_MOVDBGA2D_VALID(dest_32b_lo, src, addr_mode, instr_mod, dst) \
  (ckernel::is_valid(dest_32b_lo, 1) && ckernel::is_valid(src, 6) && ckernel::is_valid(addr_mode, 2) && ckernel::is_valid(instr_mod, 3) && ckernel::is_valid(dst, 12))
#define TT_MOVDBGA2D(dest_32b_lo, src, addr_mode, instr_mod, dst) \
  lltt::insn(TT_OP_MOVDBGA2D(dest_32b_lo, src, addr_mode, instr_mod, dst))
#define TTI_MOVDBGA2D(dest_32b_lo, src, addr_mode, instr_mod, dst) \
  TT_MOVDBGA2D(dest_32b_lo, src, addr_mode, instr_mod, dst)

#define TT_OP_MPOOL3S1(clear_dvalid, addr_mode, index_en, dst) \
  ((0x24 << 24) | ((clear_dvalid) << 22) | ((addr_mode) << 15) | ((index_en) << 14) | ((dst) << 0))
#define TT_MPOOL3S1_VALID(clear_dvalid, addr_mode, index_en, dst) \
  (ckernel::is_valid(clear_dvalid, 2) && ckernel::is_valid(addr_mode, 7) && ckernel::is_valid(index_en, 1) && ckernel::is_valid(dst, 14))
#define TT_MPOOL3S1(clear_dvalid, addr_mode, index_en, dst) \
  lltt::insn(TT_OP_MPOOL3S1(clear_dvalid, addr_mode, index_en, dst))
#define TTI_MPOOL3S1(clear_dvalid, addr_mode, index_en, dst) \
  TT_MPOOL3S1(clear_dvalid, addr_mode, index_en, dst)

#define TT_OP_MPOOL3S2(clear_dvalid, addr_mode, index_en, dst) \
  ((0x31 << 24) | ((clear_dvalid) << 22) | ((addr_mode) << 15) | ((index_en) << 14) | ((dst) << 0))
#define TT_MPOOL3S2_VALID(clear_dvalid, addr_mode, index_en, dst) \
  (ckernel::is_valid(clear_dvalid, 2) && ckernel::is_valid(addr_mode, 7) && ckernel::is_valid(index_en, 1) && ckernel::is_valid(dst, 14))
#define TT_MPOOL3S2(clear_dvalid, addr_mode, index_en, dst) \
  lltt::insn(TT_OP_MPOOL3S2(clear_dvalid, addr_mode, index_en, dst))
#define TTI_MPOOL3S2(clear_dvalid, addr_mode, index_en, dst) \
  TT_MPOOL3S2(clear_dvalid, addr_mode, index_en, dst)

#define TT_OP_MULDMAREG(OpBisConst, ResultRegIndex, OpBRegIndex, OpARegIndex) \
  ((0x5a << 24) | ((OpBisConst) << 23) | ((ResultRegIndex) << 12) | ((OpBRegIndex) << 6) | ((OpARegIndex) << 0))
#define TT_MULDMAREG_VALID(OpBisConst, ResultRegIndex, OpBRegIndex, OpARegIndex) \
  (ckernel::is_valid(OpBisConst, 1) && ckernel::is_valid(ResultRegIndex, 11) && ckernel::is_valid(OpBRegIndex, 6) && ckernel::is_valid(OpARegIndex, 6))
#define TT_MULDMAREG(OpBisConst, ResultRegIndex, OpBRegIndex, OpARegIndex) \
  lltt::insn(TT_OP_MULDMAREG(OpBisConst, ResultRegIndex, OpBRegIndex, OpARegIndex))
#define TTI_MULDMAREG(OpBisConst, ResultRegIndex, OpBRegIndex, OpARegIndex) \
  TT_MULDMAREG(OpBisConst, ResultRegIndex, OpBRegIndex, OpARegIndex)

#define TT_OP_MVMUL(clear_dvalid, instr_mod19, addr_mode, dst) \
  ((0x26 << 24) | ((clear_dvalid) << 22) | ((instr_mod19) << 19) | ((addr_mode) << 15) | ((dst) << 0))
#define TT_MVMUL_VALID(clear_dvalid, instr_mod19, addr_mode, dst) \
  (ckernel::is_valid(clear_dvalid, 2) && ckernel::is_valid(instr_mod19, 3) && ckernel::is_valid(addr_mode, 4) && ckernel::is_valid(dst, 15))
#define TT_MVMUL(clear_dvalid, instr_mod19, addr_mode, dst) \
  lltt::insn(TT_OP_MVMUL(clear_dvalid, instr_mod19, addr_mode, dst))
#define TTI_MVMUL(clear_dvalid, instr_mod19, addr_mode, dst) \
  TT_MVMUL(clear_dvalid, instr_mod19, addr_mode, dst)

#define TT_OP_NOP\
  TT_OP(0x02, 0)
#define TTI_NOP\
  INSTRUCTION_WORD(TT_OP_NOP)

#define TT_OP_PACR(AddrMode, ZeroWrite, PackSel, OvrdThreadId, Concat, Flush, Last) \
  ((0x41 << 24) | ((AddrMode) << 15) | ((ZeroWrite) << 12) | ((PackSel) << 8) | ((OvrdThreadId) << 7) | ((Concat) << 4) | ((Flush) << 1) | ((Last) << 0))
#define TT_PACR_VALID(AddrMode, ZeroWrite, PackSel, OvrdThreadId, Concat, Flush, Last) \
  (ckernel::is_valid(AddrMode, 9) && ckernel::is_valid(ZeroWrite, 3) && ckernel::is_valid(PackSel, 4) && ckernel::is_valid(OvrdThreadId, 1) && ckernel::is_valid(Concat, 3) && ckernel::is_valid(Flush, 3) && ckernel::is_valid(Last, 1))
#define TT_PACR(AddrMode, ZeroWrite, PackSel, OvrdThreadId, Concat, Flush, Last) \
  lltt::insn(TT_OP_PACR(AddrMode, ZeroWrite, PackSel, OvrdThreadId, Concat, Flush, Last))
#define TTI_PACR(AddrMode, ZeroWrite, PackSel, OvrdThreadId, Concat, Flush, Last) \
  TT_PACR(AddrMode, ZeroWrite, PackSel, OvrdThreadId, Concat, Flush, Last)

#define TT_OP_PACR_SETREG(Push, AddrSel, WrData, PackSel, StreamId, Flush, Last) \
  ((0x4a << 24) | ((Push) << 23) | ((AddrSel) << 22) | ((WrData) << 12) | ((PackSel) << 8) | ((StreamId) << 2) | ((Flush) << 1) | ((Last) << 0))
#define TT_PACR_SETREG_VALID(Push, AddrSel, WrData, PackSel, StreamId, Flush, Last) \
  (ckernel::is_valid(Push, 1) && ckernel::is_valid(AddrSel, 1) && ckernel::is_valid(WrData, 10) && ckernel::is_valid(PackSel, 4) && ckernel::is_valid(StreamId, 6) && ckernel::is_valid(Flush, 1) && ckernel::is_valid(Last, 1))
#define TT_PACR_SETREG(Push, AddrSel, WrData, PackSel, StreamId, Flush, Last) \
  lltt::insn(TT_OP_PACR_SETREG(Push, AddrSel, WrData, PackSel, StreamId, Flush, Last))
#define TTI_PACR_SETREG(Push, AddrSel, WrData, PackSel, StreamId, Flush, Last) \
  TT_PACR_SETREG(Push, AddrSel, WrData, PackSel, StreamId, Flush, Last)

#define TT_OP_RAREB\
  TT_OP(0x15, 0)
#define TTI_RAREB\
  INSTRUCTION_WORD(TT_OP_RAREB)

#define TT_OP_RDCFG(GprAddress, CfgReg) \
  ((0xb1 << 24) | ((GprAddress) << 16) | ((CfgReg) << 0))
#define TT_RDCFG_VALID(GprAddress, CfgReg) \
  (ckernel::is_valid(GprAddress, 8) && ckernel::is_valid(CfgReg, 16))
#define TT_RDCFG(GprAddress, CfgReg) \
  lltt::insn(TT_OP_RDCFG(GprAddress, CfgReg))
#define TTI_RDCFG(GprAddress, CfgReg) \
  TT_RDCFG(GprAddress, CfgReg)

#define TT_OP_REG2FLOP(SizeSel, TargetSel, ByteOffset, ContextId_2, FlopIndex, RegIndex) \
  ((0x48 << 24) | ((SizeSel) << 22) | ((TargetSel) << 20) | ((ByteOffset) << 18) | ((ContextId_2) << 16) | ((FlopIndex) << 6) | ((RegIndex) << 0))
#define TT_REG2FLOP_VALID(SizeSel, TargetSel, ByteOffset, ContextId_2, FlopIndex, RegIndex) \
  (ckernel::is_valid(SizeSel, 2) && ckernel::is_valid(TargetSel, 2) && ckernel::is_valid(ByteOffset, 2) && ckernel::is_valid(ContextId_2, 2) && ckernel::is_valid(FlopIndex, 10) && ckernel::is_valid(RegIndex, 6))
#define TT_REG2FLOP(SizeSel, TargetSel, ByteOffset, ContextId_2, FlopIndex, RegIndex) \
  lltt::insn(TT_OP_REG2FLOP(SizeSel, TargetSel, ByteOffset, ContextId_2, FlopIndex, RegIndex))
#define TTI_REG2FLOP(SizeSel, TargetSel, ByteOffset, ContextId_2, FlopIndex, RegIndex) \
  TT_REG2FLOP(SizeSel, TargetSel, ByteOffset, ContextId_2, FlopIndex, RegIndex)

#define TT_OP_REPLAY(start_idx, len, execute_while_loading, load_mode) \
  ((0x04 << 24) | ((start_idx) << 14) | ((len) << 4) | ((execute_while_loading) << 1) | ((load_mode) << 0))
#define TT_REPLAY_VALID(start_idx, len, execute_while_loading, load_mode) \
  (ckernel::is_valid(start_idx, 10) && ckernel::is_valid(len, 10) && ckernel::is_valid(execute_while_loading, 3) && ckernel::is_valid(load_mode, 1))
// Do not use directly
#define TT_REPLAY(start_idx, len, execute_while_loading, load_mode)   \
  (!load_mode ? lltt::replay(start_idx, len) : execute_while_loading ? lltt::record<Exec>(start_idx, len) : llt::record(start_idx, len))
#define TTI_REPLAY(start_idx, len, execute_while_loading, load_mode)  \
  TT_REPLAY(start_idx, len, execute_while_loading, load_mode)

#define TT_OP_RMWCIB0(Mask, Data, CfgRegAddr) \
  ((0xb3 << 24) | ((Mask) << 16) | ((Data) << 8) | ((CfgRegAddr) << 0))
#define TT_RMWCIB0_VALID(Mask, Data, CfgRegAddr) \
  (ckernel::is_valid(Mask, 8) && ckernel::is_valid(Data, 8) && ckernel::is_valid(CfgRegAddr, 8))
#define TT_RMWCIB0(Mask, Data, CfgRegAddr) \
  lltt::insn(TT_OP_RMWCIB0(Mask, Data, CfgRegAddr))
#define TTI_RMWCIB0(Mask, Data, CfgRegAddr) \
  TT_RMWCIB0(Mask, Data, CfgRegAddr)

#define TT_OP_RMWCIB1(Mask, Data, CfgRegAddr) \
  ((0xb4 << 24) | ((Mask) << 16) | ((Data) << 8) | ((CfgRegAddr) << 0))
#define TT_RMWCIB1_VALID(Mask, Data, CfgRegAddr) \
  (ckernel::is_valid(Mask, 8) && ckernel::is_valid(Data, 8) && ckernel::is_valid(CfgRegAddr, 8))
#define TT_RMWCIB1(Mask, Data, CfgRegAddr) \
  lltt::insn(TT_OP_RMWCIB1(Mask, Data, CfgRegAddr))
#define TTI_RMWCIB1(Mask, Data, CfgRegAddr) \
  TT_RMWCIB1(Mask, Data, CfgRegAddr)

#define TT_OP_RMWCIB2(Mask, Data, CfgRegAddr) \
  ((0xb5 << 24) | ((Mask) << 16) | ((Data) << 8) | ((CfgRegAddr) << 0))
#define TT_RMWCIB2_VALID(Mask, Data, CfgRegAddr) \
  (ckernel::is_valid(Mask, 8) && ckernel::is_valid(Data, 8) && ckernel::is_valid(CfgRegAddr, 8))
#define TT_RMWCIB2(Mask, Data, CfgRegAddr) \
  lltt::insn(TT_OP_RMWCIB2(Mask, Data, CfgRegAddr))
#define TTI_RMWCIB2(Mask, Data, CfgRegAddr) \
  TT_RMWCIB2(Mask, Data, CfgRegAddr)

#define TT_OP_RMWCIB3(Mask, Data, CfgRegAddr) \
  ((0xb6 << 24) | ((Mask) << 16) | ((Data) << 8) | ((CfgRegAddr) << 0))
#define TT_RMWCIB3_VALID(Mask, Data, CfgRegAddr) \
  (ckernel::is_valid(Mask, 8) && ckernel::is_valid(Data, 8) && ckernel::is_valid(CfgRegAddr, 8))
#define TT_RMWCIB3(Mask, Data, CfgRegAddr) \
  lltt::insn(TT_OP_RMWCIB3(Mask, Data, CfgRegAddr))
#define TTI_RMWCIB3(Mask, Data, CfgRegAddr) \
  TT_RMWCIB3(Mask, Data, CfgRegAddr)

#define TT_OP_RSTDMA\
  TT_OP(0x44, 0)
#define TTI_RSTDMA\
  INSTRUCTION_WORD(TT_OP_RSTDMA)

#define TT_OP_SEMGET(sem_sel) \
  ((0xa5 << 24) | ((sem_sel) << 2))
#define TT_SEMGET_VALID(sem_sel) \
  (ckernel::is_valid(sem_sel, 22))
#define TT_SEMGET(sem_sel) \
  lltt::insn(TT_OP_SEMGET(sem_sel))
#define TTI_SEMGET(sem_sel) \
  TT_SEMGET(sem_sel)

#define TT_OP_SEMINIT(max_value, init_value, sem_sel) \
  ((0xa3 << 24) | ((max_value) << 20) | ((init_value) << 16) | ((sem_sel) << 2))
#define TT_SEMINIT_VALID(max_value, init_value, sem_sel) \
  (ckernel::is_valid(max_value, 4) && ckernel::is_valid(init_value, 4) && ckernel::is_valid(sem_sel, 14))
#define TT_SEMINIT(max_value, init_value, sem_sel) \
  lltt::insn(TT_OP_SEMINIT(max_value, init_value, sem_sel))
#define TTI_SEMINIT(max_value, init_value, sem_sel) \
  TT_SEMINIT(max_value, init_value, sem_sel)

#define TT_OP_SEMPOST(sem_sel) \
  ((0xa4 << 24) | ((sem_sel) << 2))
#define TT_SEMPOST_VALID(sem_sel) \
  (ckernel::is_valid(sem_sel, 22))
#define TT_SEMPOST(sem_sel) \
  lltt::insn(TT_OP_SEMPOST(sem_sel))
#define TTI_SEMPOST(sem_sel) \
  TT_SEMPOST(sem_sel)

#define TT_OP_SEMWAIT(stall_res, sem_sel, wait_sem_cond) \
  ((0xa6 << 24) | ((stall_res) << 15) | ((sem_sel) << 2) | ((wait_sem_cond) << 0))
#define TT_SEMWAIT_VALID(stall_res, sem_sel, wait_sem_cond) \
  (ckernel::is_valid(stall_res, 9) && ckernel::is_valid(sem_sel, 13) && ckernel::is_valid(wait_sem_cond, 2))
#define TT_SEMWAIT(stall_res, sem_sel, wait_sem_cond) \
  lltt::insn(TT_OP_SEMWAIT(stall_res, sem_sel, wait_sem_cond))
#define TTI_SEMWAIT(stall_res, sem_sel, wait_sem_cond) \
  TT_SEMWAIT(stall_res, sem_sel, wait_sem_cond)

#define TT_OP_SETADC(CntSetMask, ChannelIndex, DimensionIndex, Value) \
  ((0x50 << 24) | ((CntSetMask) << 21) | ((ChannelIndex) << 20) | ((DimensionIndex) << 18) | ((Value) << 0))
#define TT_SETADC_VALID(CntSetMask, ChannelIndex, DimensionIndex, Value) \
  (ckernel::is_valid(CntSetMask, 3) && ckernel::is_valid(ChannelIndex, 1) && ckernel::is_valid(DimensionIndex, 2) && ckernel::is_valid(Value, 18))
#define TT_SETADC(CntSetMask, ChannelIndex, DimensionIndex, Value) \
  lltt::insn(TT_OP_SETADC(CntSetMask, ChannelIndex, DimensionIndex, Value))
#define TTI_SETADC(CntSetMask, ChannelIndex, DimensionIndex, Value) \
  TT_SETADC(CntSetMask, ChannelIndex, DimensionIndex, Value)

#define TT_OP_SETADCXX(CntSetMask, x_end2, x_start) \
  ((0x5e << 24) | ((CntSetMask) << 21) | ((x_end2) << 10) | ((x_start) << 0))
#define TT_SETADCXX_VALID(CntSetMask, x_end2, x_start) \
  (ckernel::is_valid(CntSetMask, 3) && ckernel::is_valid(x_end2, 11) && ckernel::is_valid(x_start, 10))
#define TT_SETADCXX(CntSetMask, x_end2, x_start) \
  lltt::insn(TT_OP_SETADCXX(CntSetMask, x_end2, x_start))
#define TTI_SETADCXX(CntSetMask, x_end2, x_start) \
  TT_SETADCXX(CntSetMask, x_end2, x_start)

#define TT_OP_SETADCXY(CntSetMask, Ch1_Y, Ch1_X, Ch0_Y, Ch0_X, BitMask) \
  ((0x51 << 24) | ((CntSetMask) << 21) | ((Ch1_Y) << 15) | ((Ch1_X) << 12) | ((Ch0_Y) << 9) | ((Ch0_X) << 6) | ((BitMask) << 0))
#define TT_SETADCXY_VALID(CntSetMask, Ch1_Y, Ch1_X, Ch0_Y, Ch0_X, BitMask) \
  (ckernel::is_valid(CntSetMask, 3) && ckernel::is_valid(Ch1_Y, 6) && ckernel::is_valid(Ch1_X, 3) && ckernel::is_valid(Ch0_Y, 3) && ckernel::is_valid(Ch0_X, 3) && ckernel::is_valid(BitMask, 6))
#define TT_SETADCXY(CntSetMask, Ch1_Y, Ch1_X, Ch0_Y, Ch0_X, BitMask) \
  lltt::insn(TT_OP_SETADCXY(CntSetMask, Ch1_Y, Ch1_X, Ch0_Y, Ch0_X, BitMask))
#define TTI_SETADCXY(CntSetMask, Ch1_Y, Ch1_X, Ch0_Y, Ch0_X, BitMask) \
  TT_SETADCXY(CntSetMask, Ch1_Y, Ch1_X, Ch0_Y, Ch0_X, BitMask)

#define TT_OP_SETADCZW(CntSetMask, Ch1_Y, Ch1_X, Ch0_Y, Ch0_X, BitMask) \
  ((0x54 << 24) | ((CntSetMask) << 21) | ((Ch1_Y) << 15) | ((Ch1_X) << 12) | ((Ch0_Y) << 9) | ((Ch0_X) << 6) | ((BitMask) << 0))
#define TT_SETADCZW_VALID(CntSetMask, Ch1_Y, Ch1_X, Ch0_Y, Ch0_X, BitMask) \
  (ckernel::is_valid(CntSetMask, 3) && ckernel::is_valid(Ch1_Y, 6) && ckernel::is_valid(Ch1_X, 3) && ckernel::is_valid(Ch0_Y, 3) && ckernel::is_valid(Ch0_X, 3) && ckernel::is_valid(BitMask, 6))
#define TT_SETADCZW(CntSetMask, Ch1_Y, Ch1_X, Ch0_Y, Ch0_X, BitMask) \
  lltt::insn(TT_OP_SETADCZW(CntSetMask, Ch1_Y, Ch1_X, Ch0_Y, Ch0_X, BitMask))
#define TTI_SETADCZW(CntSetMask, Ch1_Y, Ch1_X, Ch0_Y, Ch0_X, BitMask) \
  TT_SETADCZW(CntSetMask, Ch1_Y, Ch1_X, Ch0_Y, Ch0_X, BitMask)

#define TT_OP_SETASHRMH(reg_mask, halo_mask) \
  ((0x1e << 24) | ((reg_mask) << 1) | ((halo_mask) << 0))
#define TT_SETASHRMH_VALID(reg_mask, halo_mask) \
  (ckernel::is_valid(reg_mask, 23) && ckernel::is_valid(halo_mask, 1))
#define TT_SETASHRMH(reg_mask, halo_mask) \
  lltt::insn(TT_OP_SETASHRMH(reg_mask, halo_mask))
#define TTI_SETASHRMH(reg_mask, halo_mask) \
  TT_SETASHRMH(reg_mask, halo_mask)

#define TT_OP_SETASHRMH0(reg_mask, halo_mask) \
  ((0x1a << 24) | ((reg_mask) << 1) | ((halo_mask) << 0))
#define TT_SETASHRMH0_VALID(reg_mask, halo_mask) \
  (ckernel::is_valid(reg_mask, 23) && ckernel::is_valid(halo_mask, 1))
#define TT_SETASHRMH0(reg_mask, halo_mask) \
  lltt::insn(TT_OP_SETASHRMH0(reg_mask, halo_mask))
#define TTI_SETASHRMH0(reg_mask, halo_mask) \
  TT_SETASHRMH0(reg_mask, halo_mask)

#define TT_OP_SETASHRMH1(reg_mask, halo_mask) \
  ((0x1b << 24) | ((reg_mask) << 1) | ((halo_mask) << 0))
#define TT_SETASHRMH1_VALID(reg_mask, halo_mask) \
  (ckernel::is_valid(reg_mask, 23) && ckernel::is_valid(halo_mask, 1))
#define TT_SETASHRMH1(reg_mask, halo_mask) \
  lltt::insn(TT_OP_SETASHRMH1(reg_mask, halo_mask))
#define TTI_SETASHRMH1(reg_mask, halo_mask) \
  TT_SETASHRMH1(reg_mask, halo_mask)

#define TT_OP_SETASHRMV(reg_mask2) \
  ((0x1c << 24) | ((reg_mask2) << 0))
#define TT_SETASHRMV_VALID(reg_mask2) \
  (ckernel::is_valid(reg_mask2, 24))
#define TT_SETASHRMV(reg_mask2) \
  lltt::insn(TT_OP_SETASHRMV(reg_mask2))
#define TTI_SETASHRMV(reg_mask2) \
  TT_SETASHRMV(reg_mask2)

#define TT_OP_SETC16(setc16_reg, setc16_value) \
  ((0xb2 << 24) | ((setc16_reg) << 16) | ((setc16_value) << 0))
#define TT_SETC16_VALID(setc16_reg, setc16_value) \
  (ckernel::is_valid(setc16_reg, 8) && ckernel::is_valid(setc16_value, 16))
#define TT_SETC16(setc16_reg, setc16_value) \
  lltt::insn(TT_OP_SETC16(setc16_reg, setc16_value))
#define TTI_SETC16(setc16_reg, setc16_value) \
  TT_SETC16(setc16_reg, setc16_value)

#define TT_OP_SETDMAREG(Payload_SigSelSize, Payload_SigSel, SetSignalsMode, RegIndex16b) \
  ((0x45 << 24) | ((Payload_SigSelSize) << 22) | ((Payload_SigSel) << 8) | ((SetSignalsMode) << 7) | ((RegIndex16b) << 0))
#define TT_SETDMAREG_VALID(Payload_SigSelSize, Payload_SigSel, SetSignalsMode, RegIndex16b) \
  (ckernel::is_valid(Payload_SigSelSize, 2) && ckernel::is_valid(Payload_SigSel, 14) && ckernel::is_valid(SetSignalsMode, 1) && ckernel::is_valid(RegIndex16b, 7))
#define TT_SETDMAREG(Payload_SigSelSize, Payload_SigSel, SetSignalsMode, RegIndex16b) \
  lltt::insn(TT_OP_SETDMAREG(Payload_SigSelSize, Payload_SigSel, SetSignalsMode, RegIndex16b))
#define TTI_SETDMAREG(Payload_SigSelSize, Payload_SigSel, SetSignalsMode, RegIndex16b) \
  TT_SETDMAREG(Payload_SigSelSize, Payload_SigSel, SetSignalsMode, RegIndex16b)

#define TT_OP_SETDVALID(setvalid) \
  ((0x57 << 24) | ((setvalid) << 0))
#define TT_SETDVALID_VALID(setvalid) \
  (ckernel::is_valid(setvalid, 24))
#define TT_SETDVALID(setvalid) \
  lltt::insn(TT_OP_SETDVALID(setvalid))
#define TTI_SETDVALID(setvalid) \
  TT_SETDVALID(setvalid)

#define TT_OP_SETIBRWC(rwc_cr, rwc_bias, set_inc_ctrl) \
  ((0x39 << 24) | ((rwc_cr) << 18) | ((rwc_bias) << 6) | ((set_inc_ctrl) << 0))
#define TT_SETIBRWC_VALID(rwc_cr, rwc_bias, set_inc_ctrl) \
  (ckernel::is_valid(rwc_cr, 6) && ckernel::is_valid(rwc_bias, 12) && ckernel::is_valid(set_inc_ctrl, 6))
#define TT_SETIBRWC(rwc_cr, rwc_bias, set_inc_ctrl) \
  lltt::insn(TT_OP_SETIBRWC(rwc_cr, rwc_bias, set_inc_ctrl))
#define TTI_SETIBRWC(rwc_cr, rwc_bias, set_inc_ctrl) \
  TT_SETIBRWC(rwc_cr, rwc_bias, set_inc_ctrl)

#define TT_OP_SETPKEDGOF(y_end, y_start, x_end, x_start) \
  ((0x1d << 24) | ((y_end) << 12) | ((y_start) << 8) | ((x_end) << 4) | ((x_start) << 0))
#define TT_SETPKEDGOF_VALID(y_end, y_start, x_end, x_start) \
  (ckernel::is_valid(y_end, 12) && ckernel::is_valid(y_start, 4) && ckernel::is_valid(x_end, 4) && ckernel::is_valid(x_start, 4))
#define TT_SETPKEDGOF(y_end, y_start, x_end, x_start) \
  lltt::insn(TT_OP_SETPKEDGOF(y_end, y_start, x_end, x_start))
#define TTI_SETPKEDGOF(y_end, y_start, x_end, x_start) \
  TT_SETPKEDGOF(y_end, y_start, x_end, x_start)

#define TT_OP_SETRWC(clear_ab_vld, rwc_cr, rwc_d, rwc_b, rwc_a, BitMask) \
  ((0x37 << 24) | ((clear_ab_vld) << 22) | ((rwc_cr) << 18) | ((rwc_d) << 14) | ((rwc_b) << 10) | ((rwc_a) << 6) | ((BitMask) << 0))
#define TT_SETRWC_VALID(clear_ab_vld, rwc_cr, rwc_d, rwc_b, rwc_a, BitMask) \
  (ckernel::is_valid(clear_ab_vld, 2) && ckernel::is_valid(rwc_cr, 4) && ckernel::is_valid(rwc_d, 4) && ckernel::is_valid(rwc_b, 4) && ckernel::is_valid(rwc_a, 4) && ckernel::is_valid(BitMask, 6))
#define TT_SETRWC(clear_ab_vld, rwc_cr, rwc_d, rwc_b, rwc_a, BitMask) \
  lltt::insn(TT_OP_SETRWC(clear_ab_vld, rwc_cr, rwc_d, rwc_b, rwc_a, BitMask))
#define TTI_SETRWC(clear_ab_vld, rwc_cr, rwc_d, rwc_b, rwc_a, BitMask) \
  TT_SETRWC(clear_ab_vld, rwc_cr, rwc_d, rwc_b, rwc_a, BitMask)

#define TT_OP_SFPABS(imm12_math, lreg_c, lreg_dest, instr_mod1) \
  ((0x7d << 24) | ((imm12_math) << 12) | ((lreg_c) << 8) | ((lreg_dest) << 4) | ((instr_mod1) << 0))
#define TT_SFPABS_VALID(imm12_math, lreg_c, lreg_dest, instr_mod1) \
  (ckernel::is_valid(imm12_math, 12) && ckernel::is_valid(lreg_c, 4) && ckernel::is_valid(lreg_dest, 4) && ckernel::is_valid(instr_mod1, 4))
#define TT_SFPABS(imm12_math, lreg_c, lreg_dest, instr_mod1) \
  lltt::insn(TT_OP_SFPABS(imm12_math, lreg_c, lreg_dest, instr_mod1))
#define TTI_SFPABS(imm12_math, lreg_c, lreg_dest, instr_mod1) \
  TT_SFPABS(imm12_math, lreg_c, lreg_dest, instr_mod1)

#define TT_OP_SFPADD(lreg_src_a, lreg_src_b, lreg_src_c, lreg_dest, instr_mod1) \
  ((0x85 << 24) | ((lreg_src_a) << 16) | ((lreg_src_b) << 12) | ((lreg_src_c) << 8) | ((lreg_dest) << 4) | ((instr_mod1) << 0))
#define TT_SFPADD_VALID(lreg_src_a, lreg_src_b, lreg_src_c, lreg_dest, instr_mod1) \
  (ckernel::is_valid(lreg_src_a, 8) && ckernel::is_valid(lreg_src_b, 4) && ckernel::is_valid(lreg_src_c, 4) && ckernel::is_valid(lreg_dest, 4) && ckernel::is_valid(instr_mod1, 4))
#define TT_SFPADD(lreg_src_a, lreg_src_b, lreg_src_c, lreg_dest, instr_mod1) \
  lltt::insn(TT_OP_SFPADD(lreg_src_a, lreg_src_b, lreg_src_c, lreg_dest, instr_mod1))
#define TTI_SFPADD(lreg_src_a, lreg_src_b, lreg_src_c, lreg_dest, instr_mod1) \
  TT_SFPADD(lreg_src_a, lreg_src_b, lreg_src_c, lreg_dest, instr_mod1)

#define TT_OP_SFPADDI(imm16_math, lreg_dest, instr_mod1) \
  ((0x75 << 24) | ((imm16_math) << 8) | ((lreg_dest) << 4) | ((instr_mod1) << 0))
#define TT_SFPADDI_VALID(imm16_math, lreg_dest, instr_mod1) \
  (ckernel::is_valid(imm16_math, 16) && ckernel::is_valid(lreg_dest, 4) && ckernel::is_valid(instr_mod1, 4))
#define TT_SFPADDI(imm16_math, lreg_dest, instr_mod1) \
  lltt::insn(TT_OP_SFPADDI(imm16_math, lreg_dest, instr_mod1))
#define TTI_SFPADDI(imm16_math, lreg_dest, instr_mod1) \
  TT_SFPADDI(imm16_math, lreg_dest, instr_mod1)

#define TT_OP_SFPAND(imm12_math, lreg_c, lreg_dest, instr_mod1) \
  ((0x7e << 24) | ((imm12_math) << 12) | ((lreg_c) << 8) | ((lreg_dest) << 4) | ((instr_mod1) << 0))
#define TT_SFPAND_VALID(imm12_math, lreg_c, lreg_dest, instr_mod1) \
  (ckernel::is_valid(imm12_math, 12) && ckernel::is_valid(lreg_c, 4) && ckernel::is_valid(lreg_dest, 4) && ckernel::is_valid(instr_mod1, 4))
#define TT_SFPAND(imm12_math, lreg_c, lreg_dest, instr_mod1) \
  lltt::insn(TT_OP_SFPAND(imm12_math, lreg_c, lreg_dest, instr_mod1))
#define TTI_SFPAND(imm12_math, lreg_c, lreg_dest, instr_mod1) \
  TT_SFPAND(imm12_math, lreg_c, lreg_dest, instr_mod1)

#define TT_OP_SFPCAST(lreg_src_c, lreg_dest, instr_mod1) \
  ((0x90 << 24) | ((lreg_src_c) << 8) | ((lreg_dest) << 4) | ((instr_mod1) << 0))
#define TT_SFPCAST_VALID(lreg_src_c, lreg_dest, instr_mod1) \
  (ckernel::is_valid(lreg_src_c, 16) && ckernel::is_valid(lreg_dest, 4) && ckernel::is_valid(instr_mod1, 4))
#define TT_SFPCAST(lreg_src_c, lreg_dest, instr_mod1) \
  lltt::insn(TT_OP_SFPCAST(lreg_src_c, lreg_dest, instr_mod1))
#define TTI_SFPCAST(lreg_src_c, lreg_dest, instr_mod1) \
  TT_SFPCAST(lreg_src_c, lreg_dest, instr_mod1)

#define TT_OP_SFPCOMPC(imm12_math, lreg_c, lreg_dest, instr_mod1) \
  ((0x8b << 24) | ((imm12_math) << 12) | ((lreg_c) << 8) | ((lreg_dest) << 4) | ((instr_mod1) << 0))
#define TT_SFPCOMPC_VALID(imm12_math, lreg_c, lreg_dest, instr_mod1) \
  (ckernel::is_valid(imm12_math, 12) && ckernel::is_valid(lreg_c, 4) && ckernel::is_valid(lreg_dest, 4) && ckernel::is_valid(instr_mod1, 4))
#define TT_SFPCOMPC(imm12_math, lreg_c, lreg_dest, instr_mod1) \
  lltt::insn(TT_OP_SFPCOMPC(imm12_math, lreg_c, lreg_dest, instr_mod1))
#define TTI_SFPCOMPC(imm12_math, lreg_c, lreg_dest, instr_mod1) \
  TT_SFPCOMPC(imm12_math, lreg_c, lreg_dest, instr_mod1)

#define TT_OP_SFPCONFIG(imm16_math, config_dest, instr_mod1) \
  ((0x91 << 24) | ((imm16_math) << 8) | ((config_dest) << 4) | ((instr_mod1) << 0))
#define TT_SFPCONFIG_VALID(imm16_math, config_dest, instr_mod1) \
  (ckernel::is_valid(imm16_math, 16) && ckernel::is_valid(config_dest, 4) && ckernel::is_valid(instr_mod1, 4))
#define TT_SFPCONFIG(imm16_math, config_dest, instr_mod1) \
  lltt::insn(TT_OP_SFPCONFIG(imm16_math, config_dest, instr_mod1))
#define TTI_SFPCONFIG(imm16_math, config_dest, instr_mod1) \
  TT_SFPCONFIG(imm16_math, config_dest, instr_mod1)

#define TT_OP_SFPDIVP2(imm12_math, lreg_c, lreg_dest, instr_mod1) \
  ((0x76 << 24) | ((imm12_math) << 12) | ((lreg_c) << 8) | ((lreg_dest) << 4) | ((instr_mod1) << 0))
#define TT_SFPDIVP2_VALID(imm12_math, lreg_c, lreg_dest, instr_mod1) \
  (ckernel::is_valid(imm12_math, 12) && ckernel::is_valid(lreg_c, 4) && ckernel::is_valid(lreg_dest, 4) && ckernel::is_valid(instr_mod1, 4))
#define TT_SFPDIVP2(imm12_math, lreg_c, lreg_dest, instr_mod1) \
  lltt::insn(TT_OP_SFPDIVP2(imm12_math, lreg_c, lreg_dest, instr_mod1))
#define TTI_SFPDIVP2(imm12_math, lreg_c, lreg_dest, instr_mod1) \
  TT_SFPDIVP2(imm12_math, lreg_c, lreg_dest, instr_mod1)

#define TT_OP_SFPENCC(imm12_math, lreg_c, lreg_dest, instr_mod1) \
  ((0x8a << 24) | ((imm12_math) << 12) | ((lreg_c) << 8) | ((lreg_dest) << 4) | ((instr_mod1) << 0))
#define TT_SFPENCC_VALID(imm12_math, lreg_c, lreg_dest, instr_mod1) \
  (ckernel::is_valid(imm12_math, 12) && ckernel::is_valid(lreg_c, 4) && ckernel::is_valid(lreg_dest, 4) && ckernel::is_valid(instr_mod1, 4))
#define TT_SFPENCC(imm12_math, lreg_c, lreg_dest, instr_mod1) \
  lltt::insn(TT_OP_SFPENCC(imm12_math, lreg_c, lreg_dest, instr_mod1))
#define TTI_SFPENCC(imm12_math, lreg_c, lreg_dest, instr_mod1) \
  TT_SFPENCC(imm12_math, lreg_c, lreg_dest, instr_mod1)

#define TT_OP_SFPEXEXP(imm12_math, lreg_c, lreg_dest, instr_mod1) \
  ((0x77 << 24) | ((imm12_math) << 12) | ((lreg_c) << 8) | ((lreg_dest) << 4) | ((instr_mod1) << 0))
#define TT_SFPEXEXP_VALID(imm12_math, lreg_c, lreg_dest, instr_mod1) \
  (ckernel::is_valid(imm12_math, 12) && ckernel::is_valid(lreg_c, 4) && ckernel::is_valid(lreg_dest, 4) && ckernel::is_valid(instr_mod1, 4))
#define TT_SFPEXEXP(imm12_math, lreg_c, lreg_dest, instr_mod1) \
  lltt::insn(TT_OP_SFPEXEXP(imm12_math, lreg_c, lreg_dest, instr_mod1))
#define TTI_SFPEXEXP(imm12_math, lreg_c, lreg_dest, instr_mod1) \
  TT_SFPEXEXP(imm12_math, lreg_c, lreg_dest, instr_mod1)

#define TT_OP_SFPEXMAN(imm12_math, lreg_c, lreg_dest, instr_mod1) \
  ((0x78 << 24) | ((imm12_math) << 12) | ((lreg_c) << 8) | ((lreg_dest) << 4) | ((instr_mod1) << 0))
#define TT_SFPEXMAN_VALID(imm12_math, lreg_c, lreg_dest, instr_mod1) \
  (ckernel::is_valid(imm12_math, 12) && ckernel::is_valid(lreg_c, 4) && ckernel::is_valid(lreg_dest, 4) && ckernel::is_valid(instr_mod1, 4))
#define TT_SFPEXMAN(imm12_math, lreg_c, lreg_dest, instr_mod1) \
  lltt::insn(TT_OP_SFPEXMAN(imm12_math, lreg_c, lreg_dest, instr_mod1))
#define TTI_SFPEXMAN(imm12_math, lreg_c, lreg_dest, instr_mod1) \
  TT_SFPEXMAN(imm12_math, lreg_c, lreg_dest, instr_mod1)

#define TT_OP_SFPIADD(imm12_math, lreg_c, lreg_dest, instr_mod1) \
  ((0x79 << 24) | ((imm12_math) << 12) | ((lreg_c) << 8) | ((lreg_dest) << 4) | ((instr_mod1) << 0))
#define TT_SFPIADD_VALID(imm12_math, lreg_c, lreg_dest, instr_mod1) \
  (ckernel::is_valid(imm12_math, 12) && ckernel::is_valid(lreg_c, 4) && ckernel::is_valid(lreg_dest, 4) && ckernel::is_valid(instr_mod1, 4))
#define TT_SFPIADD(imm12_math, lreg_c, lreg_dest, instr_mod1) \
  lltt::insn(TT_OP_SFPIADD(imm12_math, lreg_c, lreg_dest, instr_mod1))
#define TTI_SFPIADD(imm12_math, lreg_c, lreg_dest, instr_mod1) \
  TT_SFPIADD(imm12_math, lreg_c, lreg_dest, instr_mod1)

#define TT_OP_SFPLOAD(lreg_ind, instr_mod0, sfpu_addr_mode, dest_reg_addr) \
  ((0x70 << 24) | ((lreg_ind) << 20) | ((instr_mod0) << 16) | ((sfpu_addr_mode) << 14) | ((dest_reg_addr) << 0))
#define TT_SFPLOAD_VALID(lreg_ind, instr_mod0, sfpu_addr_mode, dest_reg_addr) \
  (ckernel::is_valid(lreg_ind, 4) && ckernel::is_valid(instr_mod0, 4) && ckernel::is_valid(sfpu_addr_mode, 2) && ckernel::is_valid(dest_reg_addr, 14))
#define TT_SFPLOAD(lreg_ind, instr_mod0, sfpu_addr_mode, dest_reg_addr) \
  lltt::insn(TT_OP_SFPLOAD(lreg_ind, instr_mod0, sfpu_addr_mode, dest_reg_addr))
#define TTI_SFPLOAD(lreg_ind, instr_mod0, sfpu_addr_mode, dest_reg_addr) \
  TT_SFPLOAD(lreg_ind, instr_mod0, sfpu_addr_mode, dest_reg_addr)

#define TT_OP_SFPLOADI(lreg_ind, instr_mod0, imm16) \
  ((0x71 << 24) | ((lreg_ind) << 20) | ((instr_mod0) << 16) | ((imm16) << 0))
#define TT_SFPLOADI_VALID(lreg_ind, instr_mod0, imm16) \
  (ckernel::is_valid(lreg_ind, 4) && ckernel::is_valid(instr_mod0, 4) && ckernel::is_valid(imm16, 16))
#define TT_SFPLOADI(lreg_ind, instr_mod0, imm16) \
  lltt::insn(TT_OP_SFPLOADI(lreg_ind, instr_mod0, imm16))
#define TTI_SFPLOADI(lreg_ind, instr_mod0, imm16) \
  TT_SFPLOADI(lreg_ind, instr_mod0, imm16)

#define TT_OP_SFPLOADMACRO(lreg_ind, instr_mod0, sfpu_addr_mode, dest_reg_addr) \
  ((0x93 << 24) | ((lreg_ind) << 20) | ((instr_mod0) << 16) | ((sfpu_addr_mode) << 14) | ((dest_reg_addr) << 0))
#define TT_SFPLOADMACRO_VALID(lreg_ind, instr_mod0, sfpu_addr_mode, dest_reg_addr) \
  (ckernel::is_valid(lreg_ind, 4) && ckernel::is_valid(instr_mod0, 4) && ckernel::is_valid(sfpu_addr_mode, 2) && ckernel::is_valid(dest_reg_addr, 14))
#define TT_SFPLOADMACRO(lreg_ind, instr_mod0, sfpu_addr_mode, dest_reg_addr) \
  lltt::insn(TT_OP_SFPLOADMACRO(lreg_ind, instr_mod0, sfpu_addr_mode, dest_reg_addr))
#define TTI_SFPLOADMACRO(lreg_ind, instr_mod0, sfpu_addr_mode, dest_reg_addr) \
  TT_SFPLOADMACRO(lreg_ind, instr_mod0, sfpu_addr_mode, dest_reg_addr)

#define TT_OP_SFPLUT(lreg_ind, instr_mod0, dest_reg_addr) \
  ((0x73 << 24) | ((lreg_ind) << 20) | ((instr_mod0) << 16) | ((dest_reg_addr) << 0))
#define TT_SFPLUT_VALID(lreg_ind, instr_mod0, dest_reg_addr) \
  (ckernel::is_valid(lreg_ind, 4) && ckernel::is_valid(instr_mod0, 4) && ckernel::is_valid(dest_reg_addr, 16))
#define TT_SFPLUT(lreg_ind, instr_mod0, dest_reg_addr) \
  lltt::insn(TT_OP_SFPLUT(lreg_ind, instr_mod0, dest_reg_addr))
#define TTI_SFPLUT(lreg_ind, instr_mod0, dest_reg_addr) \
  TT_SFPLUT(lreg_ind, instr_mod0, dest_reg_addr)

#define TT_OP_SFPLUTFP32(lreg_dest, instr_mod1) \
  ((0x95 << 24) | ((lreg_dest) << 4) | ((instr_mod1) << 0))
#define TT_SFPLUTFP32_VALID(lreg_dest, instr_mod1) \
  (ckernel::is_valid(lreg_dest, 20) && ckernel::is_valid(instr_mod1, 4))
#define TT_SFPLUTFP32(lreg_dest, instr_mod1) \
  lltt::insn(TT_OP_SFPLUTFP32(lreg_dest, instr_mod1))
#define TTI_SFPLUTFP32(lreg_dest, instr_mod1) \
  TT_SFPLUTFP32(lreg_dest, instr_mod1)

#define TT_OP_SFPLZ(imm12_math, lreg_c, lreg_dest, instr_mod1) \
  ((0x81 << 24) | ((imm12_math) << 12) | ((lreg_c) << 8) | ((lreg_dest) << 4) | ((instr_mod1) << 0))
#define TT_SFPLZ_VALID(imm12_math, lreg_c, lreg_dest, instr_mod1) \
  (ckernel::is_valid(imm12_math, 12) && ckernel::is_valid(lreg_c, 4) && ckernel::is_valid(lreg_dest, 4) && ckernel::is_valid(instr_mod1, 4))
#define TT_SFPLZ(imm12_math, lreg_c, lreg_dest, instr_mod1) \
  lltt::insn(TT_OP_SFPLZ(imm12_math, lreg_c, lreg_dest, instr_mod1))
#define TTI_SFPLZ(imm12_math, lreg_c, lreg_dest, instr_mod1) \
  TT_SFPLZ(imm12_math, lreg_c, lreg_dest, instr_mod1)

#define TT_OP_SFPMAD(lreg_src_a, lreg_src_b, lreg_src_c, lreg_dest, instr_mod1) \
  ((0x84 << 24) | ((lreg_src_a) << 16) | ((lreg_src_b) << 12) | ((lreg_src_c) << 8) | ((lreg_dest) << 4) | ((instr_mod1) << 0))
#define TT_SFPMAD_VALID(lreg_src_a, lreg_src_b, lreg_src_c, lreg_dest, instr_mod1) \
  (ckernel::is_valid(lreg_src_a, 8) && ckernel::is_valid(lreg_src_b, 4) && ckernel::is_valid(lreg_src_c, 4) && ckernel::is_valid(lreg_dest, 4) && ckernel::is_valid(instr_mod1, 4))
#define TT_SFPMAD(lreg_src_a, lreg_src_b, lreg_src_c, lreg_dest, instr_mod1) \
  lltt::insn(TT_OP_SFPMAD(lreg_src_a, lreg_src_b, lreg_src_c, lreg_dest, instr_mod1))
#define TTI_SFPMAD(lreg_src_a, lreg_src_b, lreg_src_c, lreg_dest, instr_mod1) \
  TT_SFPMAD(lreg_src_a, lreg_src_b, lreg_src_c, lreg_dest, instr_mod1)

#define TT_OP_SFPMOV(imm12_math, lreg_c, lreg_dest, instr_mod1) \
  ((0x7c << 24) | ((imm12_math) << 12) | ((lreg_c) << 8) | ((lreg_dest) << 4) | ((instr_mod1) << 0))
#define TT_SFPMOV_VALID(imm12_math, lreg_c, lreg_dest, instr_mod1) \
  (ckernel::is_valid(imm12_math, 12) && ckernel::is_valid(lreg_c, 4) && ckernel::is_valid(lreg_dest, 4) && ckernel::is_valid(instr_mod1, 4))
#define TT_SFPMOV(imm12_math, lreg_c, lreg_dest, instr_mod1) \
  lltt::insn(TT_OP_SFPMOV(imm12_math, lreg_c, lreg_dest, instr_mod1))
#define TTI_SFPMOV(imm12_math, lreg_c, lreg_dest, instr_mod1) \
  TT_SFPMOV(imm12_math, lreg_c, lreg_dest, instr_mod1)

#define TT_OP_SFPMUL(lreg_src_a, lreg_src_b, lreg_src_c, lreg_dest, instr_mod1) \
  ((0x86 << 24) | ((lreg_src_a) << 16) | ((lreg_src_b) << 12) | ((lreg_src_c) << 8) | ((lreg_dest) << 4) | ((instr_mod1) << 0))
#define TT_SFPMUL_VALID(lreg_src_a, lreg_src_b, lreg_src_c, lreg_dest, instr_mod1) \
  (ckernel::is_valid(lreg_src_a, 8) && ckernel::is_valid(lreg_src_b, 4) && ckernel::is_valid(lreg_src_c, 4) && ckernel::is_valid(lreg_dest, 4) && ckernel::is_valid(instr_mod1, 4))
#define TT_SFPMUL(lreg_src_a, lreg_src_b, lreg_src_c, lreg_dest, instr_mod1) \
  lltt::insn(TT_OP_SFPMUL(lreg_src_a, lreg_src_b, lreg_src_c, lreg_dest, instr_mod1))
#define TTI_SFPMUL(lreg_src_a, lreg_src_b, lreg_src_c, lreg_dest, instr_mod1) \
  TT_SFPMUL(lreg_src_a, lreg_src_b, lreg_src_c, lreg_dest, instr_mod1)

#define TT_OP_SFPMULI(imm16_math, lreg_dest, instr_mod1) \
  ((0x74 << 24) | ((imm16_math) << 8) | ((lreg_dest) << 4) | ((instr_mod1) << 0))
#define TT_SFPMULI_VALID(imm16_math, lreg_dest, instr_mod1) \
  (ckernel::is_valid(imm16_math, 16) && ckernel::is_valid(lreg_dest, 4) && ckernel::is_valid(instr_mod1, 4))
#define TT_SFPMULI(imm16_math, lreg_dest, instr_mod1) \
  lltt::insn(TT_OP_SFPMULI(imm16_math, lreg_dest, instr_mod1))
#define TTI_SFPMULI(imm16_math, lreg_dest, instr_mod1) \
  TT_SFPMULI(imm16_math, lreg_dest, instr_mod1)

#define TT_OP_SFPNOP\
  TT_OP(0x8f, 0)
#define TTI_SFPNOP\
  INSTRUCTION_WORD(TT_OP_SFPNOP)

#define TT_OP_SFPNOT(imm12_math, lreg_c, lreg_dest, instr_mod1) \
  ((0x80 << 24) | ((imm12_math) << 12) | ((lreg_c) << 8) | ((lreg_dest) << 4) | ((instr_mod1) << 0))
#define TT_SFPNOT_VALID(imm12_math, lreg_c, lreg_dest, instr_mod1) \
  (ckernel::is_valid(imm12_math, 12) && ckernel::is_valid(lreg_c, 4) && ckernel::is_valid(lreg_dest, 4) && ckernel::is_valid(instr_mod1, 4))
#define TT_SFPNOT(imm12_math, lreg_c, lreg_dest, instr_mod1) \
  lltt::insn(TT_OP_SFPNOT(imm12_math, lreg_c, lreg_dest, instr_mod1))
#define TTI_SFPNOT(imm12_math, lreg_c, lreg_dest, instr_mod1) \
  TT_SFPNOT(imm12_math, lreg_c, lreg_dest, instr_mod1)

#define TT_OP_SFPOR(imm12_math, lreg_c, lreg_dest, instr_mod1) \
  ((0x7f << 24) | ((imm12_math) << 12) | ((lreg_c) << 8) | ((lreg_dest) << 4) | ((instr_mod1) << 0))
#define TT_SFPOR_VALID(imm12_math, lreg_c, lreg_dest, instr_mod1) \
  (ckernel::is_valid(imm12_math, 12) && ckernel::is_valid(lreg_c, 4) && ckernel::is_valid(lreg_dest, 4) && ckernel::is_valid(instr_mod1, 4))
#define TT_SFPOR(imm12_math, lreg_c, lreg_dest, instr_mod1) \
  lltt::insn(TT_OP_SFPOR(imm12_math, lreg_c, lreg_dest, instr_mod1))
#define TTI_SFPOR(imm12_math, lreg_c, lreg_dest, instr_mod1) \
  TT_SFPOR(imm12_math, lreg_c, lreg_dest, instr_mod1)

#define TT_OP_SFPPOPC(imm12_math, lreg_c, lreg_dest, instr_mod1) \
  ((0x88 << 24) | ((imm12_math) << 12) | ((lreg_c) << 8) | ((lreg_dest) << 4) | ((instr_mod1) << 0))
#define TT_SFPPOPC_VALID(imm12_math, lreg_c, lreg_dest, instr_mod1) \
  (ckernel::is_valid(imm12_math, 12) && ckernel::is_valid(lreg_c, 4) && ckernel::is_valid(lreg_dest, 4) && ckernel::is_valid(instr_mod1, 4))
#define TT_SFPPOPC(imm12_math, lreg_c, lreg_dest, instr_mod1) \
  lltt::insn(TT_OP_SFPPOPC(imm12_math, lreg_c, lreg_dest, instr_mod1))
#define TTI_SFPPOPC(imm12_math, lreg_c, lreg_dest, instr_mod1) \
  TT_SFPPOPC(imm12_math, lreg_c, lreg_dest, instr_mod1)

#define TT_OP_SFPPUSHC(imm12_math, lreg_c, lreg_dest, instr_mod1) \
  ((0x87 << 24) | ((imm12_math) << 12) | ((lreg_c) << 8) | ((lreg_dest) << 4) | ((instr_mod1) << 0))
#define TT_SFPPUSHC_VALID(imm12_math, lreg_c, lreg_dest, instr_mod1) \
  (ckernel::is_valid(imm12_math, 12) && ckernel::is_valid(lreg_c, 4) && ckernel::is_valid(lreg_dest, 4) && ckernel::is_valid(instr_mod1, 4))
#define TT_SFPPUSHC(imm12_math, lreg_c, lreg_dest, instr_mod1) \
  lltt::insn(TT_OP_SFPPUSHC(imm12_math, lreg_c, lreg_dest, instr_mod1))
#define TTI_SFPPUSHC(imm12_math, lreg_c, lreg_dest, instr_mod1) \
  TT_SFPPUSHC(imm12_math, lreg_c, lreg_dest, instr_mod1)

#define TT_OP_SFPSETCC(imm12_math, lreg_c, lreg_dest, instr_mod1) \
  ((0x7b << 24) | ((imm12_math) << 12) | ((lreg_c) << 8) | ((lreg_dest) << 4) | ((instr_mod1) << 0))
#define TT_SFPSETCC_VALID(imm12_math, lreg_c, lreg_dest, instr_mod1) \
  (ckernel::is_valid(imm12_math, 12) && ckernel::is_valid(lreg_c, 4) && ckernel::is_valid(lreg_dest, 4) && ckernel::is_valid(instr_mod1, 4))
#define TT_SFPSETCC(imm12_math, lreg_c, lreg_dest, instr_mod1) \
  lltt::insn(TT_OP_SFPSETCC(imm12_math, lreg_c, lreg_dest, instr_mod1))
#define TTI_SFPSETCC(imm12_math, lreg_c, lreg_dest, instr_mod1) \
  TT_SFPSETCC(imm12_math, lreg_c, lreg_dest, instr_mod1)

#define TT_OP_SFPSETEXP(imm12_math, lreg_c, lreg_dest, instr_mod1) \
  ((0x82 << 24) | ((imm12_math) << 12) | ((lreg_c) << 8) | ((lreg_dest) << 4) | ((instr_mod1) << 0))
#define TT_SFPSETEXP_VALID(imm12_math, lreg_c, lreg_dest, instr_mod1) \
  (ckernel::is_valid(imm12_math, 12) && ckernel::is_valid(lreg_c, 4) && ckernel::is_valid(lreg_dest, 4) && ckernel::is_valid(instr_mod1, 4))
#define TT_SFPSETEXP(imm12_math, lreg_c, lreg_dest, instr_mod1) \
  lltt::insn(TT_OP_SFPSETEXP(imm12_math, lreg_c, lreg_dest, instr_mod1))
#define TTI_SFPSETEXP(imm12_math, lreg_c, lreg_dest, instr_mod1) \
  TT_SFPSETEXP(imm12_math, lreg_c, lreg_dest, instr_mod1)

#define TT_OP_SFPSETMAN(imm12_math, lreg_c, lreg_dest, instr_mod1) \
  ((0x83 << 24) | ((imm12_math) << 12) | ((lreg_c) << 8) | ((lreg_dest) << 4) | ((instr_mod1) << 0))
#define TT_SFPSETMAN_VALID(imm12_math, lreg_c, lreg_dest, instr_mod1) \
  (ckernel::is_valid(imm12_math, 12) && ckernel::is_valid(lreg_c, 4) && ckernel::is_valid(lreg_dest, 4) && ckernel::is_valid(instr_mod1, 4))
#define TT_SFPSETMAN(imm12_math, lreg_c, lreg_dest, instr_mod1) \
  lltt::insn(TT_OP_SFPSETMAN(imm12_math, lreg_c, lreg_dest, instr_mod1))
#define TTI_SFPSETMAN(imm12_math, lreg_c, lreg_dest, instr_mod1) \
  TT_SFPSETMAN(imm12_math, lreg_c, lreg_dest, instr_mod1)

#define TT_OP_SFPSETSGN(imm12_math, lreg_c, lreg_dest, instr_mod1) \
  ((0x89 << 24) | ((imm12_math) << 12) | ((lreg_c) << 8) | ((lreg_dest) << 4) | ((instr_mod1) << 0))
#define TT_SFPSETSGN_VALID(imm12_math, lreg_c, lreg_dest, instr_mod1) \
  (ckernel::is_valid(imm12_math, 12) && ckernel::is_valid(lreg_c, 4) && ckernel::is_valid(lreg_dest, 4) && ckernel::is_valid(instr_mod1, 4))
#define TT_SFPSETSGN(imm12_math, lreg_c, lreg_dest, instr_mod1) \
  lltt::insn(TT_OP_SFPSETSGN(imm12_math, lreg_c, lreg_dest, instr_mod1))
#define TTI_SFPSETSGN(imm12_math, lreg_c, lreg_dest, instr_mod1) \
  TT_SFPSETSGN(imm12_math, lreg_c, lreg_dest, instr_mod1)

#define TT_OP_SFPSHFT(imm12_math, lreg_c, lreg_dest, instr_mod1) \
  ((0x7a << 24) | ((imm12_math) << 12) | ((lreg_c) << 8) | ((lreg_dest) << 4) | ((instr_mod1) << 0))
#define TT_SFPSHFT_VALID(imm12_math, lreg_c, lreg_dest, instr_mod1) \
  (ckernel::is_valid(imm12_math, 12) && ckernel::is_valid(lreg_c, 4) && ckernel::is_valid(lreg_dest, 4) && ckernel::is_valid(instr_mod1, 4))
#define TT_SFPSHFT(imm12_math, lreg_c, lreg_dest, instr_mod1) \
  lltt::insn(TT_OP_SFPSHFT(imm12_math, lreg_c, lreg_dest, instr_mod1))
#define TTI_SFPSHFT(imm12_math, lreg_c, lreg_dest, instr_mod1) \
  TT_SFPSHFT(imm12_math, lreg_c, lreg_dest, instr_mod1)

#define TT_OP_SFPSHFT2(imm12_math, lreg_src_c, lreg_dest, instr_mod1) \
  ((0x94 << 24) | ((imm12_math) << 12) | ((lreg_src_c) << 8) | ((lreg_dest) << 4) | ((instr_mod1) << 0))
#define TT_SFPSHFT2_VALID(imm12_math, lreg_src_c, lreg_dest, instr_mod1) \
  (ckernel::is_valid(imm12_math, 12) && ckernel::is_valid(lreg_src_c, 4) && ckernel::is_valid(lreg_dest, 4) && ckernel::is_valid(instr_mod1, 4))
#define TT_SFPSHFT2(imm12_math, lreg_src_c, lreg_dest, instr_mod1) \
  lltt::insn(TT_OP_SFPSHFT2(imm12_math, lreg_src_c, lreg_dest, instr_mod1))
#define TTI_SFPSHFT2(imm12_math, lreg_src_c, lreg_dest, instr_mod1) \
  TT_SFPSHFT2(imm12_math, lreg_src_c, lreg_dest, instr_mod1)

#define TT_OP_SFPSTORE(lreg_ind, instr_mod0, sfpu_addr_mode, dest_reg_addr) \
  ((0x72 << 24) | ((lreg_ind) << 20) | ((instr_mod0) << 16) | ((sfpu_addr_mode) << 14) | ((dest_reg_addr) << 0))
#define TT_SFPSTORE_VALID(lreg_ind, instr_mod0, sfpu_addr_mode, dest_reg_addr) \
  (ckernel::is_valid(lreg_ind, 4) && ckernel::is_valid(instr_mod0, 4) && ckernel::is_valid(sfpu_addr_mode, 2) && ckernel::is_valid(dest_reg_addr, 14))
#define TT_SFPSTORE(lreg_ind, instr_mod0, sfpu_addr_mode, dest_reg_addr) \
  lltt::insn(TT_OP_SFPSTORE(lreg_ind, instr_mod0, sfpu_addr_mode, dest_reg_addr))
#define TTI_SFPSTORE(lreg_ind, instr_mod0, sfpu_addr_mode, dest_reg_addr) \
  TT_SFPSTORE(lreg_ind, instr_mod0, sfpu_addr_mode, dest_reg_addr)

#define TT_OP_SFPSWAP(imm12_math, lreg_src_c, lreg_dest, instr_mod1) \
  ((0x92 << 24) | ((imm12_math) << 12) | ((lreg_src_c) << 8) | ((lreg_dest) << 4) | ((instr_mod1) << 0))
#define TT_SFPSWAP_VALID(imm12_math, lreg_src_c, lreg_dest, instr_mod1) \
  (ckernel::is_valid(imm12_math, 12) && ckernel::is_valid(lreg_src_c, 4) && ckernel::is_valid(lreg_dest, 4) && ckernel::is_valid(instr_mod1, 4))
#define TT_SFPSWAP(imm12_math, lreg_src_c, lreg_dest, instr_mod1) \
  lltt::insn(TT_OP_SFPSWAP(imm12_math, lreg_src_c, lreg_dest, instr_mod1))
#define TTI_SFPSWAP(imm12_math, lreg_src_c, lreg_dest, instr_mod1) \
  TT_SFPSWAP(imm12_math, lreg_src_c, lreg_dest, instr_mod1)

#define TT_OP_SFPTRANSP(imm12_math, lreg_c, lreg_dest, instr_mod1) \
  ((0x8c << 24) | ((imm12_math) << 12) | ((lreg_c) << 8) | ((lreg_dest) << 4) | ((instr_mod1) << 0))
#define TT_SFPTRANSP_VALID(imm12_math, lreg_c, lreg_dest, instr_mod1) \
  (ckernel::is_valid(imm12_math, 12) && ckernel::is_valid(lreg_c, 4) && ckernel::is_valid(lreg_dest, 4) && ckernel::is_valid(instr_mod1, 4))
#define TT_SFPTRANSP(imm12_math, lreg_c, lreg_dest, instr_mod1) \
  lltt::insn(TT_OP_SFPTRANSP(imm12_math, lreg_c, lreg_dest, instr_mod1))
#define TTI_SFPTRANSP(imm12_math, lreg_c, lreg_dest, instr_mod1) \
  TT_SFPTRANSP(imm12_math, lreg_c, lreg_dest, instr_mod1)

#define TT_OP_SFPXOR(imm12_math, lreg_c, lreg_dest, instr_mod1) \
  ((0x8d << 24) | ((imm12_math) << 12) | ((lreg_c) << 8) | ((lreg_dest) << 4) | ((instr_mod1) << 0))
#define TT_SFPXOR_VALID(imm12_math, lreg_c, lreg_dest, instr_mod1) \
  (ckernel::is_valid(imm12_math, 12) && ckernel::is_valid(lreg_c, 4) && ckernel::is_valid(lreg_dest, 4) && ckernel::is_valid(instr_mod1, 4))
#define TT_SFPXOR(imm12_math, lreg_c, lreg_dest, instr_mod1) \
  lltt::insn(TT_OP_SFPXOR(imm12_math, lreg_c, lreg_dest, instr_mod1))
#define TTI_SFPXOR(imm12_math, lreg_c, lreg_dest, instr_mod1) \
  TT_SFPXOR(imm12_math, lreg_c, lreg_dest, instr_mod1)

#define TT_OP_SFP_STOCH_RND(rnd_mode, imm8_math, lreg_src_b, lreg_src_c, lreg_dest, instr_mod1) \
  ((0x8e << 24) | ((rnd_mode) << 21) | ((imm8_math) << 16) | ((lreg_src_b) << 12) | ((lreg_src_c) << 8) | ((lreg_dest) << 4) | ((instr_mod1) << 0))
#define TT_SFP_STOCH_RND_VALID(rnd_mode, imm8_math, lreg_src_b, lreg_src_c, lreg_dest, instr_mod1) \
  (ckernel::is_valid(rnd_mode, 3) && ckernel::is_valid(imm8_math, 5) && ckernel::is_valid(lreg_src_b, 4) && ckernel::is_valid(lreg_src_c, 4) && ckernel::is_valid(lreg_dest, 4) && ckernel::is_valid(instr_mod1, 4))
#define TT_SFP_STOCH_RND(rnd_mode, imm8_math, lreg_src_b, lreg_src_c, lreg_dest, instr_mod1) \
  lltt::insn(TT_OP_SFP_STOCH_RND(rnd_mode, imm8_math, lreg_src_b, lreg_src_c, lreg_dest, instr_mod1))
#define TTI_SFP_STOCH_RND(rnd_mode, imm8_math, lreg_src_b, lreg_src_c, lreg_dest, instr_mod1) \
  TT_SFP_STOCH_RND(rnd_mode, imm8_math, lreg_src_b, lreg_src_c, lreg_dest, instr_mod1)

#define TT_OP_SHIFTDMAREG(OpBisConst, OpSel, ResultRegIndex, OpBRegIndex, OpARegIndex) \
  ((0x5c << 24) | ((OpBisConst) << 23) | ((OpSel) << 18) | ((ResultRegIndex) << 12) | ((OpBRegIndex) << 6) | ((OpARegIndex) << 0))
#define TT_SHIFTDMAREG_VALID(OpBisConst, OpSel, ResultRegIndex, OpBRegIndex, OpARegIndex) \
  (ckernel::is_valid(OpBisConst, 1) && ckernel::is_valid(OpSel, 5) && ckernel::is_valid(ResultRegIndex, 6) && ckernel::is_valid(OpBRegIndex, 6) && ckernel::is_valid(OpARegIndex, 6))
#define TT_SHIFTDMAREG(OpBisConst, OpSel, ResultRegIndex, OpBRegIndex, OpARegIndex) \
  lltt::insn(TT_OP_SHIFTDMAREG(OpBisConst, OpSel, ResultRegIndex, OpBRegIndex, OpARegIndex))
#define TTI_SHIFTDMAREG(OpBisConst, OpSel, ResultRegIndex, OpBRegIndex, OpARegIndex) \
  TT_SHIFTDMAREG(OpBisConst, OpSel, ResultRegIndex, OpBRegIndex, OpARegIndex)

#define TT_OP_SHIFTXA(log2_amount2, shift_mode) \
  ((0x17 << 24) | ((log2_amount2) << 2) | ((shift_mode) << 0))
#define TT_SHIFTXA_VALID(log2_amount2, shift_mode) \
  (ckernel::is_valid(log2_amount2, 22) && ckernel::is_valid(shift_mode, 2))
#define TT_SHIFTXA(log2_amount2, shift_mode) \
  lltt::insn(TT_OP_SHIFTXA(log2_amount2, shift_mode))
#define TTI_SHIFTXA(log2_amount2, shift_mode) \
  TT_SHIFTXA(log2_amount2, shift_mode)

#define TT_OP_SHIFTXB(addr_mode, rot_shift, shift_row) \
  ((0x18 << 24) | ((addr_mode) << 15) | ((rot_shift) << 10) | ((shift_row) << 0))
#define TT_SHIFTXB_VALID(addr_mode, rot_shift, shift_row) \
  (ckernel::is_valid(addr_mode, 9) && ckernel::is_valid(rot_shift, 5) && ckernel::is_valid(shift_row, 10))
#define TT_SHIFTXB(addr_mode, rot_shift, shift_row) \
  lltt::insn(TT_OP_SHIFTXB(addr_mode, rot_shift, shift_row))
#define TTI_SHIFTXB(addr_mode, rot_shift, shift_row) \
  TT_SHIFTXB(addr_mode, rot_shift, shift_row)

#define TT_OP_STALLWAIT(stall_res, wait_res) \
  ((0xa2 << 24) | ((stall_res) << 15) | ((wait_res) << 0))
#define TT_STALLWAIT_VALID(stall_res, wait_res) \
  (ckernel::is_valid(stall_res, 9) && ckernel::is_valid(wait_res, 15))
#define TT_STALLWAIT(stall_res, wait_res) \
  lltt::insn(TT_OP_STALLWAIT(stall_res, wait_res))
#define TTI_STALLWAIT(stall_res, wait_res) \
  TT_STALLWAIT(stall_res, wait_res)

#define TT_OP_STOREIND(MemHierSel, SizeSel, RegSizeSel, OffsetIndex, AutoIncSpec, DataRegIndex, AddrRegIndex) \
  ((0x66 << 24) | ((MemHierSel) << 23) | ((SizeSel) << 22) | ((RegSizeSel) << 21) | ((OffsetIndex) << 14) | ((AutoIncSpec) << 12) | ((DataRegIndex) << 6) | ((AddrRegIndex) << 0))
#define TT_STOREIND_VALID(MemHierSel, SizeSel, RegSizeSel, OffsetIndex, AutoIncSpec, DataRegIndex, AddrRegIndex) \
  (ckernel::is_valid(MemHierSel, 1) && ckernel::is_valid(SizeSel, 1) && ckernel::is_valid(RegSizeSel, 1) && ckernel::is_valid(OffsetIndex, 7) && ckernel::is_valid(AutoIncSpec, 2) && ckernel::is_valid(DataRegIndex, 6) && ckernel::is_valid(AddrRegIndex, 6))
#define TT_STOREIND(MemHierSel, SizeSel, RegSizeSel, OffsetIndex, AutoIncSpec, DataRegIndex, AddrRegIndex) \
  lltt::insn(TT_OP_STOREIND(MemHierSel, SizeSel, RegSizeSel, OffsetIndex, AutoIncSpec, DataRegIndex, AddrRegIndex))
#define TTI_STOREIND(MemHierSel, SizeSel, RegSizeSel, OffsetIndex, AutoIncSpec, DataRegIndex, AddrRegIndex) \
  TT_STOREIND(MemHierSel, SizeSel, RegSizeSel, OffsetIndex, AutoIncSpec, DataRegIndex, AddrRegIndex)

#define TT_OP_STOREREG(TdmaDataRegIndex, RegAddr) \
  ((0x67 << 24) | ((TdmaDataRegIndex) << 18) | ((RegAddr) << 0))
#define TT_STOREREG_VALID(TdmaDataRegIndex, RegAddr) \
  (ckernel::is_valid(TdmaDataRegIndex, 6) && ckernel::is_valid(RegAddr, 18))
#define TT_STOREREG(TdmaDataRegIndex, RegAddr) \
  lltt::insn(TT_OP_STOREREG(TdmaDataRegIndex, RegAddr))
#define TTI_STOREREG(TdmaDataRegIndex, RegAddr) \
  TT_STOREREG(TdmaDataRegIndex, RegAddr)

#define TT_OP_SUBDMAREG(OpBisConst, ResultRegIndex, OpBRegIndex, OpARegIndex) \
  ((0x59 << 24) | ((OpBisConst) << 23) | ((ResultRegIndex) << 12) | ((OpBRegIndex) << 6) | ((OpARegIndex) << 0))
#define TT_SUBDMAREG_VALID(OpBisConst, ResultRegIndex, OpBRegIndex, OpARegIndex) \
  (ckernel::is_valid(OpBisConst, 1) && ckernel::is_valid(ResultRegIndex, 11) && ckernel::is_valid(OpBRegIndex, 6) && ckernel::is_valid(OpARegIndex, 6))
#define TT_SUBDMAREG(OpBisConst, ResultRegIndex, OpBRegIndex, OpARegIndex) \
  lltt::insn(TT_OP_SUBDMAREG(OpBisConst, ResultRegIndex, OpBRegIndex, OpARegIndex))
#define TTI_SUBDMAREG(OpBisConst, ResultRegIndex, OpBRegIndex, OpARegIndex) \
  TT_SUBDMAREG(OpBisConst, ResultRegIndex, OpBRegIndex, OpARegIndex)

#define TT_OP_TBUFCMD\
  TT_OP(0x4b, 0)
#define TTI_TBUFCMD\
  INSTRUCTION_WORD(TT_OP_TBUFCMD)

#define TT_OP_TRNSPSRCA\
  TT_OP(0x14, 0)
#define TTI_TRNSPSRCA\
  INSTRUCTION_WORD(TT_OP_TRNSPSRCA)

#define TT_OP_TRNSPSRCB\
  TT_OP(0x16, 0)
#define TTI_TRNSPSRCB\
  INSTRUCTION_WORD(TT_OP_TRNSPSRCB)

#define TT_OP_UNPACR(Unpack_block_selection, AddrMode, CfgContextCntInc, CfgContextId, AddrCntContextId, OvrdThreadId, SetDatValid, rareb_en, ZeroWrite2, AutoIncContextID, RowSearch, SearchCacheFlush, Last) \
  ((0x42 << 24) | ((Unpack_block_selection) << 23) | ((AddrMode) << 15) | ((CfgContextCntInc) << 13) | ((CfgContextId) << 10) | ((AddrCntContextId) << 8) | ((OvrdThreadId) << 7) | ((SetDatValid) << 6) | ((rareb_en) << 5) | ((ZeroWrite2) << 4) | ((AutoIncContextID) << 3) | ((RowSearch) << 2) | ((SearchCacheFlush) << 1) | ((Last) << 0))
#define TT_UNPACR_VALID(Unpack_block_selection, AddrMode, CfgContextCntInc, CfgContextId, AddrCntContextId, OvrdThreadId, SetDatValid, rareb_en, ZeroWrite2, AutoIncContextID, RowSearch, SearchCacheFlush, Last) \
  (ckernel::is_valid(Unpack_block_selection, 1) && ckernel::is_valid(AddrMode, 8) && ckernel::is_valid(CfgContextCntInc, 2) && ckernel::is_valid(CfgContextId, 3) && ckernel::is_valid(AddrCntContextId, 2) && ckernel::is_valid(OvrdThreadId, 1) && ckernel::is_valid(SetDatValid, 1) && ckernel::is_valid(rareb_en, 1) && ckernel::is_valid(ZeroWrite2, 1) && ckernel::is_valid(AutoIncContextID, 1) && ckernel::is_valid(RowSearch, 1) && ckernel::is_valid(SearchCacheFlush, 1) && ckernel::is_valid(Last, 1))
#define TT_UNPACR(Unpack_block_selection, AddrMode, CfgContextCntInc, CfgContextId, AddrCntContextId, OvrdThreadId, SetDatValid, rareb_en, ZeroWrite2, AutoIncContextID, RowSearch, SearchCacheFlush, Last) \
  lltt::insn(TT_OP_UNPACR(Unpack_block_selection, AddrMode, CfgContextCntInc, CfgContextId, AddrCntContextId, OvrdThreadId, SetDatValid, rareb_en, ZeroWrite2, AutoIncContextID, RowSearch, SearchCacheFlush, Last))
#define TTI_UNPACR(Unpack_block_selection, AddrMode, CfgContextCntInc, CfgContextId, AddrCntContextId, OvrdThreadId, SetDatValid, rareb_en, ZeroWrite2, AutoIncContextID, RowSearch, SearchCacheFlush, Last) \
  TT_UNPACR(Unpack_block_selection, AddrMode, CfgContextCntInc, CfgContextId, AddrCntContextId, OvrdThreadId, SetDatValid, rareb_en, ZeroWrite2, AutoIncContextID, RowSearch, SearchCacheFlush, Last)

#define TT_OP_UNPACR_NOP(Unpack_block_selection, NoOp) \
  ((0x43 << 24) | ((Unpack_block_selection) << 23) | ((NoOp) << 0))
#define TT_UNPACR_NOP_VALID(Unpack_block_selection, NoOp) \
  (ckernel::is_valid(Unpack_block_selection, 1) && ckernel::is_valid(NoOp, 23))
#define TT_UNPACR_NOP(Unpack_block_selection, NoOp) \
  lltt::insn(TT_OP_UNPACR_NOP(Unpack_block_selection, NoOp))
#define TTI_UNPACR_NOP(Unpack_block_selection, NoOp) \
  TT_UNPACR_NOP(Unpack_block_selection, NoOp)

#define TT_OP_WRCFG(GprAddress, wr128b, CfgReg) \
  ((0xb0 << 24) | ((GprAddress) << 16) | ((wr128b) << 15) | ((CfgReg) << 0))
#define TT_WRCFG_VALID(GprAddress, wr128b, CfgReg) \
  (ckernel::is_valid(GprAddress, 8) && ckernel::is_valid(wr128b, 1) && ckernel::is_valid(CfgReg, 15))
#define TT_WRCFG(GprAddress, wr128b, CfgReg) \
  lltt::insn(TT_OP_WRCFG(GprAddress, wr128b, CfgReg))
#define TTI_WRCFG(GprAddress, wr128b, CfgReg) \
  TT_WRCFG(GprAddress, wr128b, CfgReg)

#define TT_OP_XMOV(Mov_block_selection, Last) \
  ((0x40 << 24) | ((Mov_block_selection) << 23) | ((Last) << 0))
#define TT_XMOV_VALID(Mov_block_selection, Last) \
  (ckernel::is_valid(Mov block selection, 1) && ckernel::is_valid(Last, 23))
#define TT_XMOV(Mov_block_selection, Last) \
  lltt::insn(TT_OP_XMOV(Mov_block_selection, Last))
#define TTI_XMOV(Mov_block_selection, Last) \
  TT_XMOV(Mov_block_selection, Last)

#define TT_OP_ZEROACC(clear_mode, AddrMode, dst) \
  ((0x10 << 24) | ((clear_mode) << 19) | ((AddrMode) << 15) | ((dst) << 0))
#define TT_ZEROACC_VALID(clear_mode, AddrMode, dst) \
  (ckernel::is_valid(clear_mode, 5) && ckernel::is_valid(AddrMode, 4) && ckernel::is_valid(dst, 15))
#define TT_ZEROACC(clear_mode, AddrMode, dst) \
  lltt::insn(TT_OP_ZEROACC(clear_mode, AddrMode, dst))
#define TTI_ZEROACC(clear_mode, AddrMode, dst) \
  TT_ZEROACC(clear_mode, AddrMode, dst)

#define TT_OP_ZEROSRC(zero_val, write_mode, bank_mask, src_mask) \
  ((0x11 << 24) | ((zero_val) << 4) | ((write_mode) << 3) | ((bank_mask) << 2) | ((src_mask) << 0))
#define TT_ZEROSRC_VALID(zero_val, write_mode, bank_mask, src_mask) \
  (ckernel::is_valid(zero_val, 20) && ckernel::is_valid(write_mode, 1) && ckernel::is_valid(bank_mask, 1) && ckernel::is_valid(src_mask, 2))
#define TT_ZEROSRC(zero_val, write_mode, bank_mask, src_mask) \
  lltt::insn(TT_OP_ZEROSRC(zero_val, write_mode, bank_mask, src_mask))
#define TTI_ZEROSRC(zero_val, write_mode, bank_mask, src_mask) \
  TT_ZEROSRC(zero_val, write_mode, bank_mask, src_mask)

#elif __riscv_tt_blackhole

#define TT_OP_ADDDMAREG(OpBisConst, ResultRegIndex, OpBRegIndex, OpARegIndex) \
  ((0x58 << 24) | ((OpBisConst) << 23) | ((ResultRegIndex) << 12) | ((OpBRegIndex) << 6) | ((OpARegIndex) << 0))
#define TT_ADDDMAREG_VALID(OpBisConst, ResultRegIndex, OpBRegIndex, OpARegIndex) \
  (ckernel::is_valid(OpBisConst, 1) && ckernel::is_valid(ResultRegIndex, 11) && ckernel::is_valid(OpBRegIndex, 6) && ckernel::is_valid(OpARegIndex, 6))
#define TT_ADDDMAREG(OpBisConst, ResultRegIndex, OpBRegIndex, OpARegIndex) \
  lltt::insn(TT_OP_ADDDMAREG(OpBisConst, ResultRegIndex, OpBRegIndex, OpARegIndex))
#define TTI_ADDDMAREG(OpBisConst, ResultRegIndex, OpBRegIndex, OpARegIndex) \
  TT_ADDDMAREG(OpBisConst, ResultRegIndex, OpBRegIndex, OpARegIndex)

#define TT_OP_ADDRCRXY(CntSetMask, Ch1_Y, Ch1_X, Ch0_Y, Ch0_X, BitMask) \
  ((0x53 << 24) | ((CntSetMask) << 21) | ((Ch1_Y) << 15) | ((Ch1_X) << 12) | ((Ch0_Y) << 9) | ((Ch0_X) << 6) | ((BitMask) << 0))
#define TT_ADDRCRXY_VALID(CntSetMask, Ch1_Y, Ch1_X, Ch0_Y, Ch0_X, BitMask) \
  (ckernel::is_valid(CntSetMask, 3) && ckernel::is_valid(Ch1_Y, 6) && ckernel::is_valid(Ch1_X, 3) && ckernel::is_valid(Ch0_Y, 3) && ckernel::is_valid(Ch0_X, 3) && ckernel::is_valid(BitMask, 6))
#define TT_ADDRCRXY(CntSetMask, Ch1_Y, Ch1_X, Ch0_Y, Ch0_X, BitMask) \
  lltt::insn(TT_OP_ADDRCRXY(CntSetMask, Ch1_Y, Ch1_X, Ch0_Y, Ch0_X, BitMask))
#define TTI_ADDRCRXY(CntSetMask, Ch1_Y, Ch1_X, Ch0_Y, Ch0_X, BitMask) \
  TT_ADDRCRXY(CntSetMask, Ch1_Y, Ch1_X, Ch0_Y, Ch0_X, BitMask)

#define TT_OP_ADDRCRZW(CntSetMask, Ch1_Y, Ch1_X, Ch0_Y, Ch0_X, BitMask) \
  ((0x56 << 24) | ((CntSetMask) << 21) | ((Ch1_Y) << 15) | ((Ch1_X) << 12) | ((Ch0_Y) << 9) | ((Ch0_X) << 6) | ((BitMask) << 0))
#define TT_ADDRCRZW_VALID(CntSetMask, Ch1_Y, Ch1_X, Ch0_Y, Ch0_X, BitMask) \
  (ckernel::is_valid(CntSetMask, 3) && ckernel::is_valid(Ch1_Y, 6) && ckernel::is_valid(Ch1_X, 3) && ckernel::is_valid(Ch0_Y, 3) && ckernel::is_valid(Ch0_X, 3) && ckernel::is_valid(BitMask, 6))
#define TT_ADDRCRZW(CntSetMask, Ch1_Y, Ch1_X, Ch0_Y, Ch0_X, BitMask) \
  lltt::insn(TT_OP_ADDRCRZW(CntSetMask, Ch1_Y, Ch1_X, Ch0_Y, Ch0_X, BitMask))
#define TTI_ADDRCRZW(CntSetMask, Ch1_Y, Ch1_X, Ch0_Y, Ch0_X, BitMask) \
  TT_ADDRCRZW(CntSetMask, Ch1_Y, Ch1_X, Ch0_Y, Ch0_X, BitMask)

#define TT_OP_APOOL3S1(clear_dvalid, pool_addr_mode, index_en, dst) \
  ((0x25 << 24) | ((clear_dvalid) << 22) | ((pool_addr_mode) << 15) | ((index_en) << 14) | ((dst) << 0))
#define TT_APOOL3S1_VALID(clear_dvalid, pool_addr_mode, index_en, dst) \
  (ckernel::is_valid(clear_dvalid, 2) && ckernel::is_valid(pool_addr_mode, 7) && ckernel::is_valid(index_en, 1) && ckernel::is_valid(dst, 14))
#define TT_APOOL3S1(clear_dvalid, pool_addr_mode, index_en, dst) \
  lltt::insn(TT_OP_APOOL3S1(clear_dvalid, pool_addr_mode, index_en, dst))
#define TTI_APOOL3S1(clear_dvalid, pool_addr_mode, index_en, dst) \
  TT_APOOL3S1(clear_dvalid, pool_addr_mode, index_en, dst)

#define TT_OP_APOOL3S2(clear_dvalid, pool_addr_mode, index_en, dst) \
  ((0x32 << 24) | ((clear_dvalid) << 22) | ((pool_addr_mode) << 15) | ((index_en) << 14) | ((dst) << 0))
#define TT_APOOL3S2_VALID(clear_dvalid, pool_addr_mode, index_en, dst) \
  (ckernel::is_valid(clear_dvalid, 2) && ckernel::is_valid(pool_addr_mode, 7) && ckernel::is_valid(index_en, 1) && ckernel::is_valid(dst, 14))
#define TT_APOOL3S2(clear_dvalid, pool_addr_mode, index_en, dst) \
  lltt::insn(TT_OP_APOOL3S2(clear_dvalid, pool_addr_mode, index_en, dst))
#define TTI_APOOL3S2(clear_dvalid, pool_addr_mode, index_en, dst) \
  TT_APOOL3S2(clear_dvalid, pool_addr_mode, index_en, dst)

#define TT_OP_ATCAS(MemHierSel, SwapVal, CmpVal, Sel32b, DataRegIndex, AddrRegIndex) \
  ((0x64 << 24) | ((MemHierSel) << 23) | ((SwapVal) << 18) | ((CmpVal) << 14) | ((Sel32b) << 12) | ((DataRegIndex) << 6) | ((AddrRegIndex) << 0))
#define TT_ATCAS_VALID(MemHierSel, SwapVal, CmpVal, Sel32b, DataRegIndex, AddrRegIndex) \
  (ckernel::is_valid(MemHierSel, 1) && ckernel::is_valid(SwapVal, 5) && ckernel::is_valid(CmpVal, 4) && ckernel::is_valid(Sel32b, 2) && ckernel::is_valid(DataRegIndex, 6) && ckernel::is_valid(AddrRegIndex, 6))
#define TT_ATCAS(MemHierSel, SwapVal, CmpVal, Sel32b, DataRegIndex, AddrRegIndex) \
  lltt::insn(TT_OP_ATCAS(MemHierSel, SwapVal, CmpVal, Sel32b, DataRegIndex, AddrRegIndex))
#define TTI_ATCAS(MemHierSel, SwapVal, CmpVal, Sel32b, DataRegIndex, AddrRegIndex) \
  TT_ATCAS(MemHierSel, SwapVal, CmpVal, Sel32b, DataRegIndex, AddrRegIndex)

#define TT_OP_ATGETM(mutex_index) \
  ((0xa0 << 24) | ((mutex_index) << 0))
#define TT_ATGETM_VALID(mutex_index) \
  (ckernel::is_valid(mutex_index, 24))
#define TT_ATGETM(mutex_index) \
  lltt::insn(TT_OP_ATGETM(mutex_index))
#define TTI_ATGETM(mutex_index) \
  TT_ATGETM(mutex_index)

#define TT_OP_ATINCGET(MemHierSel, WrapVal, Sel32b, DataRegIndex, AddrRegIndex) \
  ((0x61 << 24) | ((MemHierSel) << 23) | ((WrapVal) << 14) | ((Sel32b) << 12) | ((DataRegIndex) << 6) | ((AddrRegIndex) << 0))
#define TT_ATINCGET_VALID(MemHierSel, WrapVal, Sel32b, DataRegIndex, AddrRegIndex) \
  (ckernel::is_valid(MemHierSel, 1) && ckernel::is_valid(WrapVal, 9) && ckernel::is_valid(Sel32b, 2) && ckernel::is_valid(DataRegIndex, 6) && ckernel::is_valid(AddrRegIndex, 6))
#define TT_ATINCGET(MemHierSel, WrapVal, Sel32b, DataRegIndex, AddrRegIndex) \
  lltt::insn(TT_OP_ATINCGET(MemHierSel, WrapVal, Sel32b, DataRegIndex, AddrRegIndex))
#define TTI_ATINCGET(MemHierSel, WrapVal, Sel32b, DataRegIndex, AddrRegIndex) \
  TT_ATINCGET(MemHierSel, WrapVal, Sel32b, DataRegIndex, AddrRegIndex)

#define TT_OP_ATINCGETPTR(MemHierSel, NoIncr, IncrVal, WrapVal, Sel32b, DataRegIndex, AddrRegIndex) \
  ((0x62 << 24) | ((MemHierSel) << 23) | ((NoIncr) << 22) | ((IncrVal) << 18) | ((WrapVal) << 14) | ((Sel32b) << 12) | ((DataRegIndex) << 6) | ((AddrRegIndex) << 0))
#define TT_ATINCGETPTR_VALID(MemHierSel, NoIncr, IncrVal, WrapVal, Sel32b, DataRegIndex, AddrRegIndex) \
  (ckernel::is_valid(MemHierSel, 1) && ckernel::is_valid(NoIncr, 1) && ckernel::is_valid(IncrVal, 4) && ckernel::is_valid(WrapVal, 4) && ckernel::is_valid(Sel32b, 2) && ckernel::is_valid(DataRegIndex, 6) && ckernel::is_valid(AddrRegIndex, 6))
#define TT_ATINCGETPTR(MemHierSel, NoIncr, IncrVal, WrapVal, Sel32b, DataRegIndex, AddrRegIndex) \
  lltt::insn(TT_OP_ATINCGETPTR(MemHierSel, NoIncr, IncrVal, WrapVal, Sel32b, DataRegIndex, AddrRegIndex))
#define TTI_ATINCGETPTR(MemHierSel, NoIncr, IncrVal, WrapVal, Sel32b, DataRegIndex, AddrRegIndex) \
  TT_ATINCGETPTR(MemHierSel, NoIncr, IncrVal, WrapVal, Sel32b, DataRegIndex, AddrRegIndex)

#define TT_OP_ATRELM(mutex_index) \
  ((0xa1 << 24) | ((mutex_index) << 0))
#define TT_ATRELM_VALID(mutex_index) \
  (ckernel::is_valid(mutex_index, 24))
#define TT_ATRELM(mutex_index) \
  lltt::insn(TT_OP_ATRELM(mutex_index))
#define TTI_ATRELM(mutex_index) \
  TT_ATRELM(mutex_index)

#define TT_OP_ATSWAP(MemHierSel, SwapMask, DataRegIndex, AddrRegIndex) \
  ((0x63 << 24) | ((MemHierSel) << 23) | ((SwapMask) << 14) | ((DataRegIndex) << 6) | ((AddrRegIndex) << 0))
#define TT_ATSWAP_VALID(MemHierSel, SwapMask, DataRegIndex, AddrRegIndex) \
  (ckernel::is_valid(MemHierSel, 1) && ckernel::is_valid(SwapMask, 9) && ckernel::is_valid(DataRegIndex, 8) && ckernel::is_valid(AddrRegIndex, 6))
#define TT_ATSWAP(MemHierSel, SwapMask, DataRegIndex, AddrRegIndex) \
  lltt::insn(TT_OP_ATSWAP(MemHierSel, SwapMask, DataRegIndex, AddrRegIndex))
#define TTI_ATSWAP(MemHierSel, SwapMask, DataRegIndex, AddrRegIndex) \
  TT_ATSWAP(MemHierSel, SwapMask, DataRegIndex, AddrRegIndex)

#define TT_OP_BITWOPDMAREG(OpBisConst, OpSel, ResultRegIndex, OpBRegIndex, OpARegIndex) \
  ((0x5b << 24) | ((OpBisConst) << 23) | ((OpSel) << 18) | ((ResultRegIndex) << 12) | ((OpBRegIndex) << 6) | ((OpARegIndex) << 0))
#define TT_BITWOPDMAREG_VALID(OpBisConst, OpSel, ResultRegIndex, OpBRegIndex, OpARegIndex) \
  (ckernel::is_valid(OpBisConst, 1) && ckernel::is_valid(OpSel, 5) && ckernel::is_valid(ResultRegIndex, 6) && ckernel::is_valid(OpBRegIndex, 6) && ckernel::is_valid(OpARegIndex, 6))
#define TT_BITWOPDMAREG(OpBisConst, OpSel, ResultRegIndex, OpBRegIndex, OpARegIndex) \
  lltt::insn(TT_OP_BITWOPDMAREG(OpBisConst, OpSel, ResultRegIndex, OpBRegIndex, OpARegIndex))
#define TTI_BITWOPDMAREG(OpBisConst, OpSel, ResultRegIndex, OpBRegIndex, OpARegIndex) \
  TT_BITWOPDMAREG(OpBisConst, OpSel, ResultRegIndex, OpBRegIndex, OpARegIndex)

#define TT_OP_CFGSHIFTMASK(disable_mask_on_old_val, operation, mask_width, right_cshift_amt, scratch_sel, CfgReg) \
  ((0xb8 << 24) | ((disable_mask_on_old_val) << 23) | ((operation) << 20) | ((mask_width) << 15) | ((right_cshift_amt) << 10) | ((scratch_sel) << 8) | ((CfgReg) << 0))
#define TT_CFGSHIFTMASK_VALID(disable_mask_on_old_val, operation, mask_width, right_cshift_amt, scratch_sel, CfgReg) \
  (ckernel::is_valid(disable_mask_on_old_val, 1) && ckernel::is_valid(operation, 3) && ckernel::is_valid(mask_width, 5) && ckernel::is_valid(right_cshift_amt, 5) && ckernel::is_valid(scratch_sel, 2) && ckernel::is_valid(CfgReg, 8))
#define TT_CFGSHIFTMASK(disable_mask_on_old_val, operation, mask_width, right_cshift_amt, scratch_sel, CfgReg) \
  lltt::insn(TT_OP_CFGSHIFTMASK(disable_mask_on_old_val, operation, mask_width, right_cshift_amt, scratch_sel, CfgReg))
#define TTI_CFGSHIFTMASK(disable_mask_on_old_val, operation, mask_width, right_cshift_amt, scratch_sel, CfgReg) \
  TT_CFGSHIFTMASK(disable_mask_on_old_val, operation, mask_width, right_cshift_amt, scratch_sel, CfgReg)

#define TT_OP_CLEARDVALID(cleardvalid, reset) \
  ((0x36 << 24) | ((cleardvalid) << 22) | ((reset) << 0))
#define TT_CLEARDVALID_VALID(cleardvalid, reset) \
  (ckernel::is_valid(cleardvalid, 2) && ckernel::is_valid(reset, 22))
#define TT_CLEARDVALID(cleardvalid, reset) \
  lltt::insn(TT_OP_CLEARDVALID(cleardvalid, reset))
#define TTI_CLEARDVALID(cleardvalid, reset) \
  TT_CLEARDVALID(cleardvalid, reset)

#define TT_OP_CLREXPHIST\
  TT_OP(0x21, 0)
#define TTI_CLREXPHIST\
  INSTRUCTION_WORD(TT_OP_CLREXPHIST)

#define TT_OP_CMPDMAREG(OpBisConst, OpSel, ResultRegIndex, OpBRegIndex, OpARegIndex) \
  ((0x5d << 24) | ((OpBisConst) << 23) | ((OpSel) << 18) | ((ResultRegIndex) << 12) | ((OpBRegIndex) << 6) | ((OpARegIndex) << 0))
#define TT_CMPDMAREG_VALID(OpBisConst, OpSel, ResultRegIndex, OpBRegIndex, OpARegIndex) \
  (ckernel::is_valid(OpBisConst, 1) && ckernel::is_valid(OpSel, 5) && ckernel::is_valid(ResultRegIndex, 6) && ckernel::is_valid(OpBRegIndex, 6) && ckernel::is_valid(OpARegIndex, 6))
#define TT_CMPDMAREG(OpBisConst, OpSel, ResultRegIndex, OpBRegIndex, OpARegIndex) \
  lltt::insn(TT_OP_CMPDMAREG(OpBisConst, OpSel, ResultRegIndex, OpBRegIndex, OpARegIndex))
#define TTI_CMPDMAREG(OpBisConst, OpSel, ResultRegIndex, OpBRegIndex, OpARegIndex) \
  TT_CMPDMAREG(OpBisConst, OpSel, ResultRegIndex, OpBRegIndex, OpARegIndex)

#define TT_OP_CONV3S1(clear_dvalid, rotate_weights, addr_mode, dst) \
  ((0x22 << 24) | ((clear_dvalid) << 22) | ((rotate_weights) << 17) | ((addr_mode) << 14) | ((dst) << 0))
#define TT_CONV3S1_VALID(clear_dvalid, rotate_weights, addr_mode, dst) \
  (ckernel::is_valid(clear_dvalid, 2) && ckernel::is_valid(rotate_weights, 5) && ckernel::is_valid(addr_mode, 3) && ckernel::is_valid(dst, 14))
#define TT_CONV3S1(clear_dvalid, rotate_weights, addr_mode, dst) \
  lltt::insn(TT_OP_CONV3S1(clear_dvalid, rotate_weights, addr_mode, dst))
#define TTI_CONV3S1(clear_dvalid, rotate_weights, addr_mode, dst) \
  TT_CONV3S1(clear_dvalid, rotate_weights, addr_mode, dst)

#define TT_OP_CONV3S2(clear_dvalid, rotate_weights, addr_mode, dst) \
  ((0x23 << 24) | ((clear_dvalid) << 22) | ((rotate_weights) << 17) | ((addr_mode) << 14) | ((dst) << 0))
#define TT_CONV3S2_VALID(clear_dvalid, rotate_weights, addr_mode, dst) \
  (ckernel::is_valid(clear_dvalid, 2) && ckernel::is_valid(rotate_weights, 5) && ckernel::is_valid(addr_mode, 3) && ckernel::is_valid(dst, 14))
#define TT_CONV3S2(clear_dvalid, rotate_weights, addr_mode, dst) \
  lltt::insn(TT_OP_CONV3S2(clear_dvalid, rotate_weights, addr_mode, dst))
#define TTI_CONV3S2(clear_dvalid, rotate_weights, addr_mode, dst) \
  TT_CONV3S2(clear_dvalid, rotate_weights, addr_mode, dst)

#define TT_OP_DMANOP\
  TT_OP(0x60, 0)
#define TTI_DMANOP\
  INSTRUCTION_WORD(TT_OP_DMANOP)

#define TT_OP_DOTPV(clear_dvalid, dest_accum_en, instr_mod19, addr_mode, dst) \
  ((0x29 << 24) | ((clear_dvalid) << 22) | ((dest_accum_en) << 21) | ((instr_mod19) << 19) | ((addr_mode) << 14) | ((dst) << 0))
#define TT_DOTPV_VALID(clear_dvalid, dest_accum_en, instr_mod19, addr_mode, dst) \
  (ckernel::is_valid(clear_dvalid, 2) && ckernel::is_valid(dest_accum_en, 1) && ckernel::is_valid(instr_mod19, 2) && ckernel::is_valid(addr_mode, 5) && ckernel::is_valid(dst, 14))
#define TT_DOTPV(clear_dvalid, dest_accum_en, instr_mod19, addr_mode, dst) \
  lltt::insn(TT_OP_DOTPV(clear_dvalid, dest_accum_en, instr_mod19, addr_mode, dst))
#define TTI_DOTPV(clear_dvalid, dest_accum_en, instr_mod19, addr_mode, dst) \
  TT_DOTPV(clear_dvalid, dest_accum_en, instr_mod19, addr_mode, dst)

#define TT_OP_ELWADD(clear_dvalid, dest_accum_en, instr_mod19, addr_mode, dst) \
  ((0x28 << 24) | ((clear_dvalid) << 22) | ((dest_accum_en) << 21) | ((instr_mod19) << 19) | ((addr_mode) << 14) | ((dst) << 0))
#define TT_ELWADD_VALID(clear_dvalid, dest_accum_en, instr_mod19, addr_mode, dst) \
  (ckernel::is_valid(clear_dvalid, 2) && ckernel::is_valid(dest_accum_en, 1) && ckernel::is_valid(instr_mod19, 2) && ckernel::is_valid(addr_mode, 5) && ckernel::is_valid(dst, 14))
#define TT_ELWADD(clear_dvalid, dest_accum_en, instr_mod19, addr_mode, dst) \
  lltt::insn(TT_OP_ELWADD(clear_dvalid, dest_accum_en, instr_mod19, addr_mode, dst))
#define TTI_ELWADD(clear_dvalid, dest_accum_en, instr_mod19, addr_mode, dst) \
  TT_ELWADD(clear_dvalid, dest_accum_en, instr_mod19, addr_mode, dst)

#define TT_OP_ELWMUL(clear_dvalid, dest_accum_en, instr_mod19, addr_mode, dst) \
  ((0x27 << 24) | ((clear_dvalid) << 22) | ((dest_accum_en) << 21) | ((instr_mod19) << 19) | ((addr_mode) << 14) | ((dst) << 0))
#define TT_ELWMUL_VALID(clear_dvalid, dest_accum_en, instr_mod19, addr_mode, dst) \
  (ckernel::is_valid(clear_dvalid, 2) && ckernel::is_valid(dest_accum_en, 1) && ckernel::is_valid(instr_mod19, 2) && ckernel::is_valid(addr_mode, 5) && ckernel::is_valid(dst, 14))
#define TT_ELWMUL(clear_dvalid, dest_accum_en, instr_mod19, addr_mode, dst) \
  lltt::insn(TT_OP_ELWMUL(clear_dvalid, dest_accum_en, instr_mod19, addr_mode, dst))
#define TTI_ELWMUL(clear_dvalid, dest_accum_en, instr_mod19, addr_mode, dst) \
  TT_ELWMUL(clear_dvalid, dest_accum_en, instr_mod19, addr_mode, dst)

#define TT_OP_ELWSUB(clear_dvalid, dest_accum_en, instr_mod19, addr_mode, dst) \
  ((0x30 << 24) | ((clear_dvalid) << 22) | ((dest_accum_en) << 21) | ((instr_mod19) << 19) | ((addr_mode) << 14) | ((dst) << 0))
#define TT_ELWSUB_VALID(clear_dvalid, dest_accum_en, instr_mod19, addr_mode, dst) \
  (ckernel::is_valid(clear_dvalid, 2) && ckernel::is_valid(dest_accum_en, 1) && ckernel::is_valid(instr_mod19, 2) && ckernel::is_valid(addr_mode, 5) && ckernel::is_valid(dst, 14))
#define TT_ELWSUB(clear_dvalid, dest_accum_en, instr_mod19, addr_mode, dst) \
  lltt::insn(TT_OP_ELWSUB(clear_dvalid, dest_accum_en, instr_mod19, addr_mode, dst))
#define TTI_ELWSUB(clear_dvalid, dest_accum_en, instr_mod19, addr_mode, dst) \
  TT_ELWSUB(clear_dvalid, dest_accum_en, instr_mod19, addr_mode, dst)

#define TT_OP_FLUSHDMA(FlushSpec) \
  ((0x46 << 24) | ((FlushSpec) << 0))
#define TT_FLUSHDMA_VALID(FlushSpec) \
  (ckernel::is_valid(FlushSpec, 24))
#define TT_FLUSHDMA(FlushSpec) \
  lltt::insn(TT_OP_FLUSHDMA(FlushSpec))
#define TTI_FLUSHDMA(FlushSpec) \
  TT_FLUSHDMA(FlushSpec)

#define TT_OP_GAPOOL(clear_dvalid, instr_mod19, pool_addr_mode, max_pool_index_en, dst) \
  ((0x34 << 24) | ((clear_dvalid) << 22) | ((instr_mod19) << 19) | ((pool_addr_mode) << 15) | ((max_pool_index_en) << 14) | ((dst) << 0))
#define TT_GAPOOL_VALID(clear_dvalid, instr_mod19, pool_addr_mode, max_pool_index_en, dst) \
  (ckernel::is_valid(clear_dvalid, 2) && ckernel::is_valid(instr_mod19, 3) && ckernel::is_valid(pool_addr_mode, 4) && ckernel::is_valid(max_pool_index_en, 1) && ckernel::is_valid(dst, 14))
#define TT_GAPOOL(clear_dvalid, instr_mod19, pool_addr_mode, max_pool_index_en, dst) \
  lltt::insn(TT_OP_GAPOOL(clear_dvalid, instr_mod19, pool_addr_mode, max_pool_index_en, dst))
#define TTI_GAPOOL(clear_dvalid, instr_mod19, pool_addr_mode, max_pool_index_en, dst) \
  TT_GAPOOL(clear_dvalid, instr_mod19, pool_addr_mode, max_pool_index_en, dst)

#define TT_OP_GATESRCRST(reset_srcb_gate_control, reset_srca_gate_control) \
  ((0x35 << 24) | ((reset_srcb_gate_control) << 1) | ((reset_srca_gate_control) << 0))
#define TT_GATESRCRST_VALID(reset_srcb_gate_control, reset_srca_gate_control) \
  (ckernel::is_valid(reset srcb gate control, 23) && ckernel::is_valid(reset srca gate control, 1))
#define TT_GATESRCRST(reset_srcb_gate_control, reset_srca_gate_control) \
  lltt::insn(TT_OP_GATESRCRST(reset_srcb_gate_control, reset_srca_gate_control))
#define TTI_GATESRCRST(reset_srcb_gate_control, reset_srca_gate_control) \
  TT_GATESRCRST(reset_srcb_gate_control, reset_srca_gate_control)

#define TT_OP_GMPOOL(clear_dvalid, instr_mod19, pool_addr_mode, max_pool_index_en, dst) \
  ((0x33 << 24) | ((clear_dvalid) << 22) | ((instr_mod19) << 19) | ((pool_addr_mode) << 15) | ((max_pool_index_en) << 14) | ((dst) << 0))
#define TT_GMPOOL_VALID(clear_dvalid, instr_mod19, pool_addr_mode, max_pool_index_en, dst) \
  (ckernel::is_valid(clear_dvalid, 2) && ckernel::is_valid(instr_mod19, 3) && ckernel::is_valid(pool_addr_mode, 4) && ckernel::is_valid(max_pool_index_en, 1) && ckernel::is_valid(dst, 14))
#define TT_GMPOOL(clear_dvalid, instr_mod19, pool_addr_mode, max_pool_index_en, dst) \
  lltt::insn(TT_OP_GMPOOL(clear_dvalid, instr_mod19, pool_addr_mode, max_pool_index_en, dst))
#define TTI_GMPOOL(clear_dvalid, instr_mod19, pool_addr_mode, max_pool_index_en, dst) \
  TT_GMPOOL(clear_dvalid, instr_mod19, pool_addr_mode, max_pool_index_en, dst)

#define TT_OP_INCADCXY(CntSetMask, Ch1_Y, Ch1_X, Ch0_Y, Ch0_X) \
  ((0x52 << 24) | ((CntSetMask) << 21) | ((Ch1_Y) << 15) | ((Ch1_X) << 12) | ((Ch0_Y) << 9) | ((Ch0_X) << 6))
#define TT_INCADCXY_VALID(CntSetMask, Ch1_Y, Ch1_X, Ch0_Y, Ch0_X) \
  (ckernel::is_valid(CntSetMask, 3) && ckernel::is_valid(Ch1_Y, 6) && ckernel::is_valid(Ch1_X, 3) && ckernel::is_valid(Ch0_Y, 3) && ckernel::is_valid(Ch0_X, 3))
#define TT_INCADCXY(CntSetMask, Ch1_Y, Ch1_X, Ch0_Y, Ch0_X) \
  lltt::insn(TT_OP_INCADCXY(CntSetMask, Ch1_Y, Ch1_X, Ch0_Y, Ch0_X))
#define TTI_INCADCXY(CntSetMask, Ch1_Y, Ch1_X, Ch0_Y, Ch0_X) \
  TT_INCADCXY(CntSetMask, Ch1_Y, Ch1_X, Ch0_Y, Ch0_X)

#define TT_OP_INCADCZW(CntSetMask, Ch1_Y, Ch1_X, Ch0_Y, Ch0_X) \
  ((0x55 << 24) | ((CntSetMask) << 21) | ((Ch1_Y) << 15) | ((Ch1_X) << 12) | ((Ch0_Y) << 9) | ((Ch0_X) << 6))
#define TT_INCADCZW_VALID(CntSetMask, Ch1_Y, Ch1_X, Ch0_Y, Ch0_X) \
  (ckernel::is_valid(CntSetMask, 3) && ckernel::is_valid(Ch1_Y, 6) && ckernel::is_valid(Ch1_X, 3) && ckernel::is_valid(Ch0_Y, 3) && ckernel::is_valid(Ch0_X, 3))
#define TT_INCADCZW(CntSetMask, Ch1_Y, Ch1_X, Ch0_Y, Ch0_X) \
  lltt::insn(TT_OP_INCADCZW(CntSetMask, Ch1_Y, Ch1_X, Ch0_Y, Ch0_X))
#define TTI_INCADCZW(CntSetMask, Ch1_Y, Ch1_X, Ch0_Y, Ch0_X) \
  TT_INCADCZW(CntSetMask, Ch1_Y, Ch1_X, Ch0_Y, Ch0_X)

#define TT_OP_INCRWC(rwc_cr, rwc_d, rwc_b, rwc_a) \
  ((0x38 << 24) | ((rwc_cr) << 18) | ((rwc_d) << 14) | ((rwc_b) << 10) | ((rwc_a) << 6))
#define TT_INCRWC_VALID(rwc_cr, rwc_d, rwc_b, rwc_a) \
  (ckernel::is_valid(rwc_cr, 6) && ckernel::is_valid(rwc_d, 4) && ckernel::is_valid(rwc_b, 4) && ckernel::is_valid(rwc_a, 4))
#define TT_INCRWC(rwc_cr, rwc_d, rwc_b, rwc_a) \
  lltt::insn(TT_OP_INCRWC(rwc_cr, rwc_d, rwc_b, rwc_a))
#define TTI_INCRWC(rwc_cr, rwc_d, rwc_b, rwc_a) \
  TT_INCRWC(rwc_cr, rwc_d, rwc_b, rwc_a)

#define TT_OP_LOADIND(SizeSel, OffsetIndex, AutoIncSpec, DataRegIndex, AddrRegIndex) \
  ((0x49 << 24) | ((SizeSel) << 22) | ((OffsetIndex) << 14) | ((AutoIncSpec) << 12) | ((DataRegIndex) << 6) | ((AddrRegIndex) << 0))
#define TT_LOADIND_VALID(SizeSel, OffsetIndex, AutoIncSpec, DataRegIndex, AddrRegIndex) \
  (ckernel::is_valid(SizeSel, 2) && ckernel::is_valid(OffsetIndex, 8) && ckernel::is_valid(AutoIncSpec, 2) && ckernel::is_valid(DataRegIndex, 6) && ckernel::is_valid(AddrRegIndex, 6))
#define TT_LOADIND(SizeSel, OffsetIndex, AutoIncSpec, DataRegIndex, AddrRegIndex) \
  lltt::insn(TT_OP_LOADIND(SizeSel, OffsetIndex, AutoIncSpec, DataRegIndex, AddrRegIndex))
#define TTI_LOADIND(SizeSel, OffsetIndex, AutoIncSpec, DataRegIndex, AddrRegIndex) \
  TT_LOADIND(SizeSel, OffsetIndex, AutoIncSpec, DataRegIndex, AddrRegIndex)

#define TT_OP_LOADREG(TdmaDataRegIndex, RegAddr) \
  ((0x68 << 24) | ((TdmaDataRegIndex) << 18) | ((RegAddr) << 0))
#define TT_LOADREG_VALID(TdmaDataRegIndex, RegAddr) \
  (ckernel::is_valid(TdmaDataRegIndex, 6) && ckernel::is_valid(RegAddr, 18))
#define TT_LOADREG(TdmaDataRegIndex, RegAddr) \
  lltt::insn(TT_OP_LOADREG(TdmaDataRegIndex, RegAddr))
#define TTI_LOADREG(TdmaDataRegIndex, RegAddr) \
  TT_LOADREG(TdmaDataRegIndex, RegAddr)

#define TT_OP_MFCONV3S1(clear_dvalid, rotate_weights, addr_mode, dst) \
  ((0x3a << 24) | ((clear_dvalid) << 22) | ((rotate_weights) << 17) | ((addr_mode) << 14) | ((dst) << 0))
#define TT_MFCONV3S1_VALID(clear_dvalid, rotate_weights, addr_mode, dst) \
  (ckernel::is_valid(clear_dvalid, 2) && ckernel::is_valid(rotate_weights, 5) && ckernel::is_valid(addr_mode, 3) && ckernel::is_valid(dst, 14))
#define TT_MFCONV3S1(clear_dvalid, rotate_weights, addr_mode, dst) \
  lltt::insn(TT_OP_MFCONV3S1(clear_dvalid, rotate_weights, addr_mode, dst))
#define TTI_MFCONV3S1(clear_dvalid, rotate_weights, addr_mode, dst) \
  TT_MFCONV3S1(clear_dvalid, rotate_weights, addr_mode, dst)

#define TT_OP_MOP(mop_type, loop_count, zmask_lo16_or_loop_count) \
  ((0x01 << 24) | ((mop_type) << 23) | ((loop_count) << 16) | ((zmask_lo16_or_loop_count) << 0))
#define TT_MOP_VALID(mop_type, loop_count, zmask_lo16_or_loop_count) \
  (ckernel::is_valid(mop_type, 1) && ckernel::is_valid(loop_count, 7) && ckernel::is_valid(zmask_lo16_or_loop_count, 16))
#define TT_MOP(mop_type, loop_count, zmask_lo16_or_loop_count) \
  lltt::insn(TT_OP_MOP(mop_type, loop_count, zmask_lo16_or_loop_count))
#define TTI_MOP(mop_type, loop_count, zmask_lo16_or_loop_count) \
  TT_MOP(mop_type, loop_count, zmask_lo16_or_loop_count)

#define TT_OP_MOP_CFG(zmask_hi16) \
  ((0x03 << 24) | ((zmask_hi16) << 0))
#define TT_MOP_CFG_VALID(zmask_hi16) \
  (ckernel::is_valid(zmask_hi16, 24))
#define TT_MOP_CFG(zmask_hi16) \
  lltt::insn(TT_OP_MOP_CFG(zmask_hi16))
#define TTI_MOP_CFG(zmask_hi16) \
  TT_MOP_CFG(zmask_hi16)

#define TT_OP_MOVA2D(dest_32b_lo, src, addr_mode, instr_mod, dst) \
  ((0x12 << 24) | ((dest_32b_lo) << 23) | ((src) << 17) | ((addr_mode) << 14) | ((instr_mod) << 12) | ((dst) << 0))
#define TT_MOVA2D_VALID(dest_32b_lo, src, addr_mode, instr_mod, dst) \
  (ckernel::is_valid(dest_32b_lo, 1) && ckernel::is_valid(src, 6) && ckernel::is_valid(addr_mode, 3) && ckernel::is_valid(instr_mod, 2) && ckernel::is_valid(dst, 12))
#define TT_MOVA2D(dest_32b_lo, src, addr_mode, instr_mod, dst) \
  lltt::insn(TT_OP_MOVA2D(dest_32b_lo, src, addr_mode, instr_mod, dst))
#define TTI_MOVA2D(dest_32b_lo, src, addr_mode, instr_mod, dst) \
  TT_MOVA2D(dest_32b_lo, src, addr_mode, instr_mod, dst)

#define TT_OP_MOVB2A(srca, addr_mode, instr_mod, srcb) \
  ((0x0b << 24) | ((srca) << 17) | ((addr_mode) << 14) | ((instr_mod) << 12) | ((srcb) << 0))
#define TT_MOVB2A_VALID(srca, addr_mode, instr_mod, srcb) \
  (ckernel::is_valid(srca, 7) && ckernel::is_valid(addr_mode, 3) && ckernel::is_valid(instr_mod, 2) && ckernel::is_valid(srcb, 12))
#define TT_MOVB2A(srca, addr_mode, instr_mod, srcb) \
  lltt::insn(TT_OP_MOVB2A(srca, addr_mode, instr_mod, srcb))
#define TTI_MOVB2A(srca, addr_mode, instr_mod, srcb) \
  TT_MOVB2A(srca, addr_mode, instr_mod, srcb)

#define TT_OP_MOVB2D(dest_32b_lo, src, addr_mode, movb2d_instr_mod, dst) \
  ((0x13 << 24) | ((dest_32b_lo) << 23) | ((src) << 17) | ((addr_mode) << 14) | ((movb2d_instr_mod) << 11) | ((dst) << 0))
#define TT_MOVB2D_VALID(dest_32b_lo, src, addr_mode, movb2d_instr_mod, dst) \
  (ckernel::is_valid(dest_32b_lo, 1) && ckernel::is_valid(src, 6) && ckernel::is_valid(addr_mode, 3) && ckernel::is_valid(movb2d_instr_mod, 3) && ckernel::is_valid(dst, 11))
#define TT_MOVB2D(dest_32b_lo, src, addr_mode, movb2d_instr_mod, dst) \
  lltt::insn(TT_OP_MOVB2D(dest_32b_lo, src, addr_mode, movb2d_instr_mod, dst))
#define TTI_MOVB2D(dest_32b_lo, src, addr_mode, movb2d_instr_mod, dst) \
  TT_MOVB2D(dest_32b_lo, src, addr_mode, movb2d_instr_mod, dst)

#define TT_OP_MOVD2A(dest_32b_lo, src, addr_mode, instr_mod, dst) \
  ((0x08 << 24) | ((dest_32b_lo) << 23) | ((src) << 17) | ((addr_mode) << 14) | ((instr_mod) << 12) | ((dst) << 0))
#define TT_MOVD2A_VALID(dest_32b_lo, src, addr_mode, instr_mod, dst) \
  (ckernel::is_valid(dest_32b_lo, 1) && ckernel::is_valid(src, 6) && ckernel::is_valid(addr_mode, 3) && ckernel::is_valid(instr_mod, 2) && ckernel::is_valid(dst, 12))
#define TT_MOVD2A(dest_32b_lo, src, addr_mode, instr_mod, dst) \
  lltt::insn(TT_OP_MOVD2A(dest_32b_lo, src, addr_mode, instr_mod, dst))
#define TTI_MOVD2A(dest_32b_lo, src, addr_mode, instr_mod, dst) \
  TT_MOVD2A(dest_32b_lo, src, addr_mode, instr_mod, dst)

#define TT_OP_MOVD2B(dest_32b_lo, src, addr_mode, instr_mod, dst) \
  ((0x0a << 24) | ((dest_32b_lo) << 23) | ((src) << 17) | ((addr_mode) << 14) | ((instr_mod) << 12) | ((dst) << 0))
#define TT_MOVD2B_VALID(dest_32b_lo, src, addr_mode, instr_mod, dst) \
  (ckernel::is_valid(dest_32b_lo, 1) && ckernel::is_valid(src, 6) && ckernel::is_valid(addr_mode, 3) && ckernel::is_valid(instr_mod, 2) && ckernel::is_valid(dst, 12))
#define TT_MOVD2B(dest_32b_lo, src, addr_mode, instr_mod, dst) \
  lltt::insn(TT_OP_MOVD2B(dest_32b_lo, src, addr_mode, instr_mod, dst))
#define TTI_MOVD2B(dest_32b_lo, src, addr_mode, instr_mod, dst) \
  TT_MOVD2B(dest_32b_lo, src, addr_mode, instr_mod, dst)

#define TT_OP_MOVDBGA2D(dest_32b_lo, src, addr_mode, instr_mod, dst) \
  ((0x09 << 24) | ((dest_32b_lo) << 23) | ((src) << 17) | ((addr_mode) << 14) | ((instr_mod) << 12) | ((dst) << 0))
#define TT_MOVDBGA2D_VALID(dest_32b_lo, src, addr_mode, instr_mod, dst) \
  (ckernel::is_valid(dest_32b_lo, 1) && ckernel::is_valid(src, 6) && ckernel::is_valid(addr_mode, 3) && ckernel::is_valid(instr_mod, 2) && ckernel::is_valid(dst, 12))
#define TT_MOVDBGA2D(dest_32b_lo, src, addr_mode, instr_mod, dst) \
  lltt::insn(TT_OP_MOVDBGA2D(dest_32b_lo, src, addr_mode, instr_mod, dst))
#define TTI_MOVDBGA2D(dest_32b_lo, src, addr_mode, instr_mod, dst) \
  TT_MOVDBGA2D(dest_32b_lo, src, addr_mode, instr_mod, dst)

#define TT_OP_MOVDBGB2D(dest_32b_lo, src, addr_mode, movb2d_instr_mod, dst) \
  ((0x0c << 24) | ((dest_32b_lo) << 23) | ((src) << 17) | ((addr_mode) << 14) | ((movb2d_instr_mod) << 11) | ((dst) << 0))
#define TT_MOVDBGB2D_VALID(dest_32b_lo, src, addr_mode, movb2d_instr_mod, dst) \
  (ckernel::is_valid(dest_32b_lo, 1) && ckernel::is_valid(src, 6) && ckernel::is_valid(addr_mode, 3) && ckernel::is_valid(movb2d_instr_mod, 3) && ckernel::is_valid(dst, 11))
#define TT_MOVDBGB2D(dest_32b_lo, src, addr_mode, movb2d_instr_mod, dst) \
  lltt::insn(TT_OP_MOVDBGB2D(dest_32b_lo, src, addr_mode, movb2d_instr_mod, dst))
#define TTI_MOVDBGB2D(dest_32b_lo, src, addr_mode, movb2d_instr_mod, dst) \
  TT_MOVDBGB2D(dest_32b_lo, src, addr_mode, movb2d_instr_mod, dst)

#define TT_OP_MPOOL3S1(clear_dvalid, pool_addr_mode, index_en, dst) \
  ((0x24 << 24) | ((clear_dvalid) << 22) | ((pool_addr_mode) << 15) | ((index_en) << 14) | ((dst) << 0))
#define TT_MPOOL3S1_VALID(clear_dvalid, pool_addr_mode, index_en, dst) \
  (ckernel::is_valid(clear_dvalid, 2) && ckernel::is_valid(pool_addr_mode, 7) && ckernel::is_valid(index_en, 1) && ckernel::is_valid(dst, 14))
#define TT_MPOOL3S1(clear_dvalid, pool_addr_mode, index_en, dst) \
  lltt::insn(TT_OP_MPOOL3S1(clear_dvalid, pool_addr_mode, index_en, dst))
#define TTI_MPOOL3S1(clear_dvalid, pool_addr_mode, index_en, dst) \
  TT_MPOOL3S1(clear_dvalid, pool_addr_mode, index_en, dst)

#define TT_OP_MPOOL3S2(clear_dvalid, pool_addr_mode, index_en, dst) \
  ((0x31 << 24) | ((clear_dvalid) << 22) | ((pool_addr_mode) << 15) | ((index_en) << 14) | ((dst) << 0))
#define TT_MPOOL3S2_VALID(clear_dvalid, pool_addr_mode, index_en, dst) \
  (ckernel::is_valid(clear_dvalid, 2) && ckernel::is_valid(pool_addr_mode, 7) && ckernel::is_valid(index_en, 1) && ckernel::is_valid(dst, 14))
#define TT_MPOOL3S2(clear_dvalid, pool_addr_mode, index_en, dst) \
  lltt::insn(TT_OP_MPOOL3S2(clear_dvalid, pool_addr_mode, index_en, dst))
#define TTI_MPOOL3S2(clear_dvalid, pool_addr_mode, index_en, dst) \
  TT_MPOOL3S2(clear_dvalid, pool_addr_mode, index_en, dst)

#define TT_OP_MULDMAREG(OpBisConst, ResultRegIndex, OpBRegIndex, OpARegIndex) \
  ((0x5a << 24) | ((OpBisConst) << 23) | ((ResultRegIndex) << 12) | ((OpBRegIndex) << 6) | ((OpARegIndex) << 0))
#define TT_MULDMAREG_VALID(OpBisConst, ResultRegIndex, OpBRegIndex, OpARegIndex) \
  (ckernel::is_valid(OpBisConst, 1) && ckernel::is_valid(ResultRegIndex, 11) && ckernel::is_valid(OpBRegIndex, 6) && ckernel::is_valid(OpARegIndex, 6))
#define TT_MULDMAREG(OpBisConst, ResultRegIndex, OpBRegIndex, OpARegIndex) \
  lltt::insn(TT_OP_MULDMAREG(OpBisConst, ResultRegIndex, OpBRegIndex, OpARegIndex))
#define TTI_MULDMAREG(OpBisConst, ResultRegIndex, OpBRegIndex, OpARegIndex) \
  TT_MULDMAREG(OpBisConst, ResultRegIndex, OpBRegIndex, OpARegIndex)

#define TT_OP_MVMUL(clear_dvalid, instr_mod19, addr_mode, dst) \
  ((0x26 << 24) | ((clear_dvalid) << 22) | ((instr_mod19) << 19) | ((addr_mode) << 14) | ((dst) << 0))
#define TT_MVMUL_VALID(clear_dvalid, instr_mod19, addr_mode, dst) \
  (ckernel::is_valid(clear_dvalid, 2) && ckernel::is_valid(instr_mod19, 3) && ckernel::is_valid(addr_mode, 5) && ckernel::is_valid(dst, 14))
#define TT_MVMUL(clear_dvalid, instr_mod19, addr_mode, dst) \
  lltt::insn(TT_OP_MVMUL(clear_dvalid, instr_mod19, addr_mode, dst))
#define TTI_MVMUL(clear_dvalid, instr_mod19, addr_mode, dst) \
  TT_MVMUL(clear_dvalid, instr_mod19, addr_mode, dst)

#define TT_OP_NOP\
  TT_OP(0x02, 0)
#define TTI_NOP\
  INSTRUCTION_WORD(TT_OP_NOP)

#define TT_OP_PACR(CfgContext, RowPadZero, DstAccessMode, AddrMode, AddrCntContext, ZeroWrite, ReadIntfSel, OvrdThreadId, Concat, CtxtCtrl, Flush, Last) \
  ((0x41 << 24) | ((CfgContext) << 21) | ((RowPadZero) << 18) | ((DstAccessMode) << 17) | ((AddrMode) << 15) | ((AddrCntContext) << 13) | ((ZeroWrite) << 12) | ((ReadIntfSel) << 8) | ((OvrdThreadId) << 7) | ((Concat) << 4) | ((CtxtCtrl) << 2) | ((Flush) << 1) | ((Last) << 0))
#define TT_PACR_VALID(CfgContext, RowPadZero, DstAccessMode, AddrMode, AddrCntContext, ZeroWrite, ReadIntfSel, OvrdThreadId, Concat, CtxtCtrl, Flush, Last) \
  (ckernel::is_valid(CfgContext, 3) && ckernel::is_valid(RowPadZero, 3) && ckernel::is_valid(DstAccessMode, 1) && ckernel::is_valid(AddrMode, 2) && ckernel::is_valid(AddrCntContext, 2) && ckernel::is_valid(ZeroWrite, 1) && ckernel::is_valid(ReadIntfSel, 4) && ckernel::is_valid(OvrdThreadId, 1) && ckernel::is_valid(Concat, 3) && ckernel::is_valid(CtxtCtrl, 2) && ckernel::is_valid(Flush, 1) && ckernel::is_valid(Last, 1))
#define TT_PACR(CfgContext, RowPadZero, DstAccessMode, AddrMode, AddrCntContext, ZeroWrite, ReadIntfSel, OvrdThreadId, Concat, CtxtCtrl, Flush, Last) \
  lltt::insn(TT_OP_PACR(CfgContext, RowPadZero, DstAccessMode, AddrMode, AddrCntContext, ZeroWrite, ReadIntfSel, OvrdThreadId, Concat, CtxtCtrl, Flush, Last))
#define TTI_PACR(CfgContext, RowPadZero, DstAccessMode, AddrMode, AddrCntContext, ZeroWrite, ReadIntfSel, OvrdThreadId, Concat, CtxtCtrl, Flush, Last) \
  TT_PACR(CfgContext, RowPadZero, DstAccessMode, AddrMode, AddrCntContext, ZeroWrite, ReadIntfSel, OvrdThreadId, Concat, CtxtCtrl, Flush, Last)

#define TT_OP_PACR_SETREG(Push, ModeSel, Unused, DisableStall, AddrSel, StreamId, Flush, Last) \
  ((0x4a << 24) | ((Push) << 23) | ((ModeSel) << 22) | ((Unused) << 12) | ((DisableStall) << 10) | ((AddrSel) << 8) | ((StreamId) << 2) | ((Flush) << 1) | ((Last) << 0))
#define TT_PACR_SETREG_VALID(Push, ModeSel, Unused, DisableStall, AddrSel, StreamId, Flush, Last) \
  (ckernel::is_valid(Push, 1) && ckernel::is_valid(ModeSel, 1) && ckernel::is_valid(Unused, 10) && ckernel::is_valid(DisableStall, 2) && ckernel::is_valid(AddrSel, 2) && ckernel::is_valid(StreamId, 6) && ckernel::is_valid(Flush, 1) && ckernel::is_valid(Last, 1))
#define TT_PACR_SETREG(Push, ModeSel, Unused, DisableStall, AddrSel, StreamId, Flush, Last) \
  lltt::insn(TT_OP_PACR_SETREG(Push, ModeSel, Unused, DisableStall, AddrSel, StreamId, Flush, Last))
#define TTI_PACR_SETREG(Push, ModeSel, Unused, DisableStall, AddrSel, StreamId, Flush, Last) \
  TT_PACR_SETREG(Push, ModeSel, Unused, DisableStall, AddrSel, StreamId, Flush, Last)

#define TT_OP_RAREB\
  TT_OP(0x15, 0)
#define TTI_RAREB\
  INSTRUCTION_WORD(TT_OP_RAREB)

#define TT_OP_RDCFG(GprAddress, CfgReg) \
  ((0xb1 << 24) | ((GprAddress) << 16) | ((CfgReg) << 0))
#define TT_RDCFG_VALID(GprAddress, CfgReg) \
  (ckernel::is_valid(GprAddress, 8) && ckernel::is_valid(CfgReg, 16))
#define TT_RDCFG(GprAddress, CfgReg) \
  lltt::insn(TT_OP_RDCFG(GprAddress, CfgReg))
#define TTI_RDCFG(GprAddress, CfgReg) \
  TT_RDCFG(GprAddress, CfgReg)

#define TT_OP_REG2FLOP(SizeSel, TargetSel, ByteOffset, ContextId_2, FlopIndex, RegIndex) \
  ((0x48 << 24) | ((SizeSel) << 22) | ((TargetSel) << 20) | ((ByteOffset) << 18) | ((ContextId_2) << 16) | ((FlopIndex) << 6) | ((RegIndex) << 0))
#define TT_REG2FLOP_VALID(SizeSel, TargetSel, ByteOffset, ContextId_2, FlopIndex, RegIndex) \
  (ckernel::is_valid(SizeSel, 2) && ckernel::is_valid(TargetSel, 2) && ckernel::is_valid(ByteOffset, 2) && ckernel::is_valid(ContextId_2, 2) && ckernel::is_valid(FlopIndex, 10) && ckernel::is_valid(RegIndex, 6))
#define TT_REG2FLOP(SizeSel, TargetSel, ByteOffset, ContextId_2, FlopIndex, RegIndex) \
  lltt::insn(TT_OP_REG2FLOP(SizeSel, TargetSel, ByteOffset, ContextId_2, FlopIndex, RegIndex))
#define TTI_REG2FLOP(SizeSel, TargetSel, ByteOffset, ContextId_2, FlopIndex, RegIndex) \
  TT_REG2FLOP(SizeSel, TargetSel, ByteOffset, ContextId_2, FlopIndex, RegIndex)

#define TT_OP_REPLAY(start_idx, len, execute_while_loading, load_mode) \
  ((0x04 << 24) | ((start_idx) << 14) | ((len) << 4) | ((execute_while_loading) << 1) | ((load_mode) << 0))
#define TT_REPLAY_VALID(start_idx, len, execute_while_loading, load_mode) \
  (ckernel::is_valid(start_idx, 10) && ckernel::is_valid(len, 10) && ckernel::is_valid(execute_while_loading, 3) && ckernel::is_valid(load_mode, 1))
// Do not use directly
#define TT_REPLAY(start_idx, len, execute_while_loading, load_mode)   \
  (!load_mode ? lltt::replay(start_idx, len) : execute_while_loading ? lltt::record<Exec>(start_idx, len) : llt::record(start_idx, len))
#define TTI_REPLAY(start_idx, len, execute_while_loading, load_mode)  \
  TT_REPLAY(start_idx, len, execute_while_loading, load_mode)

#define TT_OP_RESOURCEDECL(linger_time, resources, op_class) \
  ((0x05 << 24) | ((linger_time) << 13) | ((resources) << 4) | ((op_class) << 0))
#define TT_RESOURCEDECL_VALID(linger_time, resources, op_class) \
  (ckernel::is_valid(linger_time, 11) && ckernel::is_valid(resources, 9) && ckernel::is_valid(op_class, 4))
#define TT_RESOURCEDECL(linger_time, resources, op_class) \
  lltt::insn(TT_OP_RESOURCEDECL(linger_time, resources, op_class))
#define TTI_RESOURCEDECL(linger_time, resources, op_class) \
  TT_RESOURCEDECL(linger_time, resources, op_class)

#define TT_OP_RMWCIB0(Mask, Data, CfgRegAddr) \
  ((0xb3 << 24) | ((Mask) << 16) | ((Data) << 8) | ((CfgRegAddr) << 0))
#define TT_RMWCIB0_VALID(Mask, Data, CfgRegAddr) \
  (ckernel::is_valid(Mask, 8) && ckernel::is_valid(Data, 8) && ckernel::is_valid(CfgRegAddr, 8))
#define TT_RMWCIB0(Mask, Data, CfgRegAddr) \
  lltt::insn(TT_OP_RMWCIB0(Mask, Data, CfgRegAddr))
#define TTI_RMWCIB0(Mask, Data, CfgRegAddr) \
  TT_RMWCIB0(Mask, Data, CfgRegAddr)

#define TT_OP_RMWCIB1(Mask, Data, CfgRegAddr) \
  ((0xb4 << 24) | ((Mask) << 16) | ((Data) << 8) | ((CfgRegAddr) << 0))
#define TT_RMWCIB1_VALID(Mask, Data, CfgRegAddr) \
  (ckernel::is_valid(Mask, 8) && ckernel::is_valid(Data, 8) && ckernel::is_valid(CfgRegAddr, 8))
#define TT_RMWCIB1(Mask, Data, CfgRegAddr) \
  lltt::insn(TT_OP_RMWCIB1(Mask, Data, CfgRegAddr))
#define TTI_RMWCIB1(Mask, Data, CfgRegAddr) \
  TT_RMWCIB1(Mask, Data, CfgRegAddr)

#define TT_OP_RMWCIB2(Mask, Data, CfgRegAddr) \
  ((0xb5 << 24) | ((Mask) << 16) | ((Data) << 8) | ((CfgRegAddr) << 0))
#define TT_RMWCIB2_VALID(Mask, Data, CfgRegAddr) \
  (ckernel::is_valid(Mask, 8) && ckernel::is_valid(Data, 8) && ckernel::is_valid(CfgRegAddr, 8))
#define TT_RMWCIB2(Mask, Data, CfgRegAddr) \
  lltt::insn(TT_OP_RMWCIB2(Mask, Data, CfgRegAddr))
#define TTI_RMWCIB2(Mask, Data, CfgRegAddr) \
  TT_RMWCIB2(Mask, Data, CfgRegAddr)

#define TT_OP_RMWCIB3(Mask, Data, CfgRegAddr) \
  ((0xb6 << 24) | ((Mask) << 16) | ((Data) << 8) | ((CfgRegAddr) << 0))
#define TT_RMWCIB3_VALID(Mask, Data, CfgRegAddr) \
  (ckernel::is_valid(Mask, 8) && ckernel::is_valid(Data, 8) && ckernel::is_valid(CfgRegAddr, 8))
#define TT_RMWCIB3(Mask, Data, CfgRegAddr) \
  lltt::insn(TT_OP_RMWCIB3(Mask, Data, CfgRegAddr))
#define TTI_RMWCIB3(Mask, Data, CfgRegAddr) \
  TT_RMWCIB3(Mask, Data, CfgRegAddr)

#define TT_OP_RSTDMA\
  TT_OP(0x44, 0)
#define TTI_RSTDMA\
  INSTRUCTION_WORD(TT_OP_RSTDMA)

#define TT_OP_SEMGET(sem_sel) \
  ((0xa5 << 24) | ((sem_sel) << 2))
#define TT_SEMGET_VALID(sem_sel) \
  (ckernel::is_valid(sem_sel, 22))
#define TT_SEMGET(sem_sel) \
  lltt::insn(TT_OP_SEMGET(sem_sel))
#define TTI_SEMGET(sem_sel) \
  TT_SEMGET(sem_sel)

#define TT_OP_SEMINIT(max_value, init_value, sem_sel) \
  ((0xa3 << 24) | ((max_value) << 20) | ((init_value) << 16) | ((sem_sel) << 2))
#define TT_SEMINIT_VALID(max_value, init_value, sem_sel) \
  (ckernel::is_valid(max_value, 4) && ckernel::is_valid(init_value, 4) && ckernel::is_valid(sem_sel, 14))
#define TT_SEMINIT(max_value, init_value, sem_sel) \
  lltt::insn(TT_OP_SEMINIT(max_value, init_value, sem_sel))
#define TTI_SEMINIT(max_value, init_value, sem_sel) \
  TT_SEMINIT(max_value, init_value, sem_sel)

#define TT_OP_SEMPOST(sem_sel) \
  ((0xa4 << 24) | ((sem_sel) << 2))
#define TT_SEMPOST_VALID(sem_sel) \
  (ckernel::is_valid(sem_sel, 22))
#define TT_SEMPOST(sem_sel) \
  lltt::insn(TT_OP_SEMPOST(sem_sel))
#define TTI_SEMPOST(sem_sel) \
  TT_SEMPOST(sem_sel)

#define TT_OP_SEMWAIT(stall_res, sem_sel, wait_sem_cond) \
  ((0xa6 << 24) | ((stall_res) << 15) | ((sem_sel) << 2) | ((wait_sem_cond) << 0))
#define TT_SEMWAIT_VALID(stall_res, sem_sel, wait_sem_cond) \
  (ckernel::is_valid(stall_res, 9) && ckernel::is_valid(sem_sel, 13) && ckernel::is_valid(wait_sem_cond, 2))
#define TT_SEMWAIT(stall_res, sem_sel, wait_sem_cond) \
  lltt::insn(TT_OP_SEMWAIT(stall_res, sem_sel, wait_sem_cond))
#define TTI_SEMWAIT(stall_res, sem_sel, wait_sem_cond) \
  TT_SEMWAIT(stall_res, sem_sel, wait_sem_cond)

#define TT_OP_SETADC(CntSetMask, ChannelIndex, DimensionIndex, Value) \
  ((0x50 << 24) | ((CntSetMask) << 21) | ((ChannelIndex) << 20) | ((DimensionIndex) << 18) | ((Value) << 0))
#define TT_SETADC_VALID(CntSetMask, ChannelIndex, DimensionIndex, Value) \
  (ckernel::is_valid(CntSetMask, 3) && ckernel::is_valid(ChannelIndex, 1) && ckernel::is_valid(DimensionIndex, 2) && ckernel::is_valid(Value, 18))
#define TT_SETADC(CntSetMask, ChannelIndex, DimensionIndex, Value) \
  lltt::insn(TT_OP_SETADC(CntSetMask, ChannelIndex, DimensionIndex, Value))
#define TTI_SETADC(CntSetMask, ChannelIndex, DimensionIndex, Value) \
  TT_SETADC(CntSetMask, ChannelIndex, DimensionIndex, Value)

#define TT_OP_SETADCXX(CntSetMask, x_end2, x_start) \
  ((0x5e << 24) | ((CntSetMask) << 21) | ((x_end2) << 10) | ((x_start) << 0))
#define TT_SETADCXX_VALID(CntSetMask, x_end2, x_start) \
  (ckernel::is_valid(CntSetMask, 3) && ckernel::is_valid(x_end2, 11) && ckernel::is_valid(x_start, 10))
#define TT_SETADCXX(CntSetMask, x_end2, x_start) \
  lltt::insn(TT_OP_SETADCXX(CntSetMask, x_end2, x_start))
#define TTI_SETADCXX(CntSetMask, x_end2, x_start) \
  TT_SETADCXX(CntSetMask, x_end2, x_start)

#define TT_OP_SETADCXY(CntSetMask, Ch1_Y, Ch1_X, Ch0_Y, Ch0_X, BitMask) \
  ((0x51 << 24) | ((CntSetMask) << 21) | ((Ch1_Y) << 15) | ((Ch1_X) << 12) | ((Ch0_Y) << 9) | ((Ch0_X) << 6) | ((BitMask) << 0))
#define TT_SETADCXY_VALID(CntSetMask, Ch1_Y, Ch1_X, Ch0_Y, Ch0_X, BitMask) \
  (ckernel::is_valid(CntSetMask, 3) && ckernel::is_valid(Ch1_Y, 6) && ckernel::is_valid(Ch1_X, 3) && ckernel::is_valid(Ch0_Y, 3) && ckernel::is_valid(Ch0_X, 3) && ckernel::is_valid(BitMask, 6))
#define TT_SETADCXY(CntSetMask, Ch1_Y, Ch1_X, Ch0_Y, Ch0_X, BitMask) \
  lltt::insn(TT_OP_SETADCXY(CntSetMask, Ch1_Y, Ch1_X, Ch0_Y, Ch0_X, BitMask))
#define TTI_SETADCXY(CntSetMask, Ch1_Y, Ch1_X, Ch0_Y, Ch0_X, BitMask) \
  TT_SETADCXY(CntSetMask, Ch1_Y, Ch1_X, Ch0_Y, Ch0_X, BitMask)

#define TT_OP_SETADCZW(CntSetMask, Ch1_Y, Ch1_X, Ch0_Y, Ch0_X, BitMask) \
  ((0x54 << 24) | ((CntSetMask) << 21) | ((Ch1_Y) << 15) | ((Ch1_X) << 12) | ((Ch0_Y) << 9) | ((Ch0_X) << 6) | ((BitMask) << 0))
#define TT_SETADCZW_VALID(CntSetMask, Ch1_Y, Ch1_X, Ch0_Y, Ch0_X, BitMask) \
  (ckernel::is_valid(CntSetMask, 3) && ckernel::is_valid(Ch1_Y, 6) && ckernel::is_valid(Ch1_X, 3) && ckernel::is_valid(Ch0_Y, 3) && ckernel::is_valid(Ch0_X, 3) && ckernel::is_valid(BitMask, 6))
#define TT_SETADCZW(CntSetMask, Ch1_Y, Ch1_X, Ch0_Y, Ch0_X, BitMask) \
  lltt::insn(TT_OP_SETADCZW(CntSetMask, Ch1_Y, Ch1_X, Ch0_Y, Ch0_X, BitMask))
#define TTI_SETADCZW(CntSetMask, Ch1_Y, Ch1_X, Ch0_Y, Ch0_X, BitMask) \
  TT_SETADCZW(CntSetMask, Ch1_Y, Ch1_X, Ch0_Y, Ch0_X, BitMask)

#define TT_OP_SETASHRMH(reg_mask, halo_mask) \
  ((0x1e << 24) | ((reg_mask) << 1) | ((halo_mask) << 0))
#define TT_SETASHRMH_VALID(reg_mask, halo_mask) \
  (ckernel::is_valid(reg_mask, 23) && ckernel::is_valid(halo_mask, 1))
#define TT_SETASHRMH(reg_mask, halo_mask) \
  lltt::insn(TT_OP_SETASHRMH(reg_mask, halo_mask))
#define TTI_SETASHRMH(reg_mask, halo_mask) \
  TT_SETASHRMH(reg_mask, halo_mask)

#define TT_OP_SETASHRMH0(reg_mask, halo_mask) \
  ((0x1a << 24) | ((reg_mask) << 1) | ((halo_mask) << 0))
#define TT_SETASHRMH0_VALID(reg_mask, halo_mask) \
  (ckernel::is_valid(reg_mask, 23) && ckernel::is_valid(halo_mask, 1))
#define TT_SETASHRMH0(reg_mask, halo_mask) \
  lltt::insn(TT_OP_SETASHRMH0(reg_mask, halo_mask))
#define TTI_SETASHRMH0(reg_mask, halo_mask) \
  TT_SETASHRMH0(reg_mask, halo_mask)

#define TT_OP_SETASHRMH1(reg_mask, halo_mask) \
  ((0x1b << 24) | ((reg_mask) << 1) | ((halo_mask) << 0))
#define TT_SETASHRMH1_VALID(reg_mask, halo_mask) \
  (ckernel::is_valid(reg_mask, 23) && ckernel::is_valid(halo_mask, 1))
#define TT_SETASHRMH1(reg_mask, halo_mask) \
  lltt::insn(TT_OP_SETASHRMH1(reg_mask, halo_mask))
#define TTI_SETASHRMH1(reg_mask, halo_mask) \
  TT_SETASHRMH1(reg_mask, halo_mask)

#define TT_OP_SETASHRMV(reg_mask2) \
  ((0x1c << 24) | ((reg_mask2) << 0))
#define TT_SETASHRMV_VALID(reg_mask2) \
  (ckernel::is_valid(reg_mask2, 24))
#define TT_SETASHRMV(reg_mask2) \
  lltt::insn(TT_OP_SETASHRMV(reg_mask2))
#define TTI_SETASHRMV(reg_mask2) \
  TT_SETASHRMV(reg_mask2)

#define TT_OP_SETC16(setc16_reg, setc16_value) \
  ((0xb2 << 24) | ((setc16_reg) << 16) | ((setc16_value) << 0))
#define TT_SETC16_VALID(setc16_reg, setc16_value) \
  (ckernel::is_valid(setc16_reg, 8) && ckernel::is_valid(setc16_value, 16))
#define TT_SETC16(setc16_reg, setc16_value) \
  lltt::insn(TT_OP_SETC16(setc16_reg, setc16_value))
#define TTI_SETC16(setc16_reg, setc16_value) \
  TT_SETC16(setc16_reg, setc16_value)

#define TT_OP_SETDMAREG(Payload_SigSelSize, Payload_SigSel, SetSignalsMode, RegIndex16b) \
  ((0x45 << 24) | ((Payload_SigSelSize) << 22) | ((Payload_SigSel) << 8) | ((SetSignalsMode) << 7) | ((RegIndex16b) << 0))
#define TT_SETDMAREG_VALID(Payload_SigSelSize, Payload_SigSel, SetSignalsMode, RegIndex16b) \
  (ckernel::is_valid(Payload_SigSelSize, 2) && ckernel::is_valid(Payload_SigSel, 14) && ckernel::is_valid(SetSignalsMode, 1) && ckernel::is_valid(RegIndex16b, 7))
#define TT_SETDMAREG(Payload_SigSelSize, Payload_SigSel, SetSignalsMode, RegIndex16b) \
  lltt::insn(TT_OP_SETDMAREG(Payload_SigSelSize, Payload_SigSel, SetSignalsMode, RegIndex16b))
#define TTI_SETDMAREG(Payload_SigSelSize, Payload_SigSel, SetSignalsMode, RegIndex16b) \
  TT_SETDMAREG(Payload_SigSelSize, Payload_SigSel, SetSignalsMode, RegIndex16b)

#define TT_OP_SETDVALID(setvalid) \
  ((0x57 << 24) | ((setvalid) << 0))
#define TT_SETDVALID_VALID(setvalid) \
  (ckernel::is_valid(setvalid, 24))
#define TT_SETDVALID(setvalid) \
  lltt::insn(TT_OP_SETDVALID(setvalid))
#define TTI_SETDVALID(setvalid) \
  TT_SETDVALID(setvalid)

#define TT_OP_SETIBRWC(rwc_cr, rwc_bias, set_inc_ctrl) \
  ((0x39 << 24) | ((rwc_cr) << 18) | ((rwc_bias) << 6) | ((set_inc_ctrl) << 0))
#define TT_SETIBRWC_VALID(rwc_cr, rwc_bias, set_inc_ctrl) \
  (ckernel::is_valid(rwc_cr, 6) && ckernel::is_valid(rwc_bias, 12) && ckernel::is_valid(set_inc_ctrl, 6))
#define TT_SETIBRWC(rwc_cr, rwc_bias, set_inc_ctrl) \
  lltt::insn(TT_OP_SETIBRWC(rwc_cr, rwc_bias, set_inc_ctrl))
#define TTI_SETIBRWC(rwc_cr, rwc_bias, set_inc_ctrl) \
  TT_SETIBRWC(rwc_cr, rwc_bias, set_inc_ctrl)

#define TT_OP_SETPKEDGOF(y_end, y_start, x_end, x_start) \
  ((0x1d << 24) | ((y_end) << 12) | ((y_start) << 8) | ((x_end) << 4) | ((x_start) << 0))
#define TT_SETPKEDGOF_VALID(y_end, y_start, x_end, x_start) \
  (ckernel::is_valid(y_end, 12) && ckernel::is_valid(y_start, 4) && ckernel::is_valid(x_end, 4) && ckernel::is_valid(x_start, 4))
#define TT_SETPKEDGOF(y_end, y_start, x_end, x_start) \
  lltt::insn(TT_OP_SETPKEDGOF(y_end, y_start, x_end, x_start))
#define TTI_SETPKEDGOF(y_end, y_start, x_end, x_start) \
  TT_SETPKEDGOF(y_end, y_start, x_end, x_start)

#define TT_OP_SETRWC(clear_ab_vld, rwc_cr, rwc_d, rwc_b, rwc_a, BitMask) \
  ((0x37 << 24) | ((clear_ab_vld) << 22) | ((rwc_cr) << 18) | ((rwc_d) << 14) | ((rwc_b) << 10) | ((rwc_a) << 6) | ((BitMask) << 0))
#define TT_SETRWC_VALID(clear_ab_vld, rwc_cr, rwc_d, rwc_b, rwc_a, BitMask) \
  (ckernel::is_valid(clear_ab_vld, 2) && ckernel::is_valid(rwc_cr, 4) && ckernel::is_valid(rwc_d, 4) && ckernel::is_valid(rwc_b, 4) && ckernel::is_valid(rwc_a, 4) && ckernel::is_valid(BitMask, 6))
#define TT_SETRWC(clear_ab_vld, rwc_cr, rwc_d, rwc_b, rwc_a, BitMask) \
  lltt::insn(TT_OP_SETRWC(clear_ab_vld, rwc_cr, rwc_d, rwc_b, rwc_a, BitMask))
#define TTI_SETRWC(clear_ab_vld, rwc_cr, rwc_d, rwc_b, rwc_a, BitMask) \
  TT_SETRWC(clear_ab_vld, rwc_cr, rwc_d, rwc_b, rwc_a, BitMask)

#define TT_OP_SFPABS(imm12_math, lreg_c, lreg_dest, instr_mod1) \
  ((0x7d << 24) | ((imm12_math) << 12) | ((lreg_c) << 8) | ((lreg_dest) << 4) | ((instr_mod1) << 0))
#define TT_SFPABS_VALID(imm12_math, lreg_c, lreg_dest, instr_mod1) \
  (ckernel::is_valid(imm12_math, 12) && ckernel::is_valid(lreg_c, 4) && ckernel::is_valid(lreg_dest, 4) && ckernel::is_valid(instr_mod1, 4))
#define TT_SFPABS(imm12_math, lreg_c, lreg_dest, instr_mod1) \
  lltt::insn(TT_OP_SFPABS(imm12_math, lreg_c, lreg_dest, instr_mod1))
#define TTI_SFPABS(imm12_math, lreg_c, lreg_dest, instr_mod1) \
  TT_SFPABS(imm12_math, lreg_c, lreg_dest, instr_mod1)

#define TT_OP_SFPADD(lreg_src_a, lreg_src_b, lreg_src_c, lreg_dest, instr_mod1) \
  ((0x85 << 24) | ((lreg_src_a) << 16) | ((lreg_src_b) << 12) | ((lreg_src_c) << 8) | ((lreg_dest) << 4) | ((instr_mod1) << 0))
#define TT_SFPADD_VALID(lreg_src_a, lreg_src_b, lreg_src_c, lreg_dest, instr_mod1) \
  (ckernel::is_valid(lreg_src_a, 8) && ckernel::is_valid(lreg_src_b, 4) && ckernel::is_valid(lreg_src_c, 4) && ckernel::is_valid(lreg_dest, 4) && ckernel::is_valid(instr_mod1, 4))
#define TT_SFPADD(lreg_src_a, lreg_src_b, lreg_src_c, lreg_dest, instr_mod1) \
  lltt::insn(TT_OP_SFPADD(lreg_src_a, lreg_src_b, lreg_src_c, lreg_dest, instr_mod1))
#define TTI_SFPADD(lreg_src_a, lreg_src_b, lreg_src_c, lreg_dest, instr_mod1) \
  TT_SFPADD(lreg_src_a, lreg_src_b, lreg_src_c, lreg_dest, instr_mod1)

#define TT_OP_SFPADDI(imm16_math, lreg_dest, instr_mod1) \
  ((0x75 << 24) | ((imm16_math) << 8) | ((lreg_dest) << 4) | ((instr_mod1) << 0))
#define TT_SFPADDI_VALID(imm16_math, lreg_dest, instr_mod1) \
  (ckernel::is_valid(imm16_math, 16) && ckernel::is_valid(lreg_dest, 4) && ckernel::is_valid(instr_mod1, 4))
#define TT_SFPADDI(imm16_math, lreg_dest, instr_mod1) \
  lltt::insn(TT_OP_SFPADDI(imm16_math, lreg_dest, instr_mod1))
#define TTI_SFPADDI(imm16_math, lreg_dest, instr_mod1) \
  TT_SFPADDI(imm16_math, lreg_dest, instr_mod1)

#define TT_OP_SFPAND(imm12_math, lreg_c, lreg_dest, instr_mod1) \
  ((0x7e << 24) | ((imm12_math) << 12) | ((lreg_c) << 8) | ((lreg_dest) << 4) | ((instr_mod1) << 0))
#define TT_SFPAND_VALID(imm12_math, lreg_c, lreg_dest, instr_mod1) \
  (ckernel::is_valid(imm12_math, 12) && ckernel::is_valid(lreg_c, 4) && ckernel::is_valid(lreg_dest, 4) && ckernel::is_valid(instr_mod1, 4))
#define TT_SFPAND(imm12_math, lreg_c, lreg_dest, instr_mod1) \
  lltt::insn(TT_OP_SFPAND(imm12_math, lreg_c, lreg_dest, instr_mod1))
#define TTI_SFPAND(imm12_math, lreg_c, lreg_dest, instr_mod1) \
  TT_SFPAND(imm12_math, lreg_c, lreg_dest, instr_mod1)

#define TT_OP_SFPARECIP(imm12_math, lreg_c, lreg_dest, instr_mod1) \
  ((0x99 << 24) | ((imm12_math) << 12) | ((lreg_c) << 8) | ((lreg_dest) << 4) | ((instr_mod1) << 0))
#define TT_SFPARECIP_VALID(imm12_math, lreg_c, lreg_dest, instr_mod1) \
  (ckernel::is_valid(imm12_math, 12) && ckernel::is_valid(lreg_c, 4) && ckernel::is_valid(lreg_dest, 4) && ckernel::is_valid(instr_mod1, 4))
#define TT_SFPARECIP(imm12_math, lreg_c, lreg_dest, instr_mod1) \
  lltt::insn(TT_OP_SFPARECIP(imm12_math, lreg_c, lreg_dest, instr_mod1))
#define TTI_SFPARECIP(imm12_math, lreg_c, lreg_dest, instr_mod1) \
  TT_SFPARECIP(imm12_math, lreg_c, lreg_dest, instr_mod1)

#define TT_OP_SFPCAST(lreg_src_c, lreg_dest, instr_mod1) \
  ((0x90 << 24) | ((lreg_src_c) << 8) | ((lreg_dest) << 4) | ((instr_mod1) << 0))
#define TT_SFPCAST_VALID(lreg_src_c, lreg_dest, instr_mod1) \
  (ckernel::is_valid(lreg_src_c, 16) && ckernel::is_valid(lreg_dest, 4) && ckernel::is_valid(instr_mod1, 4))
#define TT_SFPCAST(lreg_src_c, lreg_dest, instr_mod1) \
  lltt::insn(TT_OP_SFPCAST(lreg_src_c, lreg_dest, instr_mod1))
#define TTI_SFPCAST(lreg_src_c, lreg_dest, instr_mod1) \
  TT_SFPCAST(lreg_src_c, lreg_dest, instr_mod1)

#define TT_OP_SFPCOMPC(imm12_math, lreg_c, lreg_dest, instr_mod1) \
  ((0x8b << 24) | ((imm12_math) << 12) | ((lreg_c) << 8) | ((lreg_dest) << 4) | ((instr_mod1) << 0))
#define TT_SFPCOMPC_VALID(imm12_math, lreg_c, lreg_dest, instr_mod1) \
  (ckernel::is_valid(imm12_math, 12) && ckernel::is_valid(lreg_c, 4) && ckernel::is_valid(lreg_dest, 4) && ckernel::is_valid(instr_mod1, 4))
#define TT_SFPCOMPC(imm12_math, lreg_c, lreg_dest, instr_mod1) \
  lltt::insn(TT_OP_SFPCOMPC(imm12_math, lreg_c, lreg_dest, instr_mod1))
#define TTI_SFPCOMPC(imm12_math, lreg_c, lreg_dest, instr_mod1) \
  TT_SFPCOMPC(imm12_math, lreg_c, lreg_dest, instr_mod1)

#define TT_OP_SFPCONFIG(imm16_math, config_dest, instr_mod1) \
  ((0x91 << 24) | ((imm16_math) << 8) | ((config_dest) << 4) | ((instr_mod1) << 0))
#define TT_SFPCONFIG_VALID(imm16_math, config_dest, instr_mod1) \
  (ckernel::is_valid(imm16_math, 16) && ckernel::is_valid(config_dest, 4) && ckernel::is_valid(instr_mod1, 4))
#define TT_SFPCONFIG(imm16_math, config_dest, instr_mod1) \
  lltt::insn(TT_OP_SFPCONFIG(imm16_math, config_dest, instr_mod1))
#define TTI_SFPCONFIG(imm16_math, config_dest, instr_mod1) \
  TT_SFPCONFIG(imm16_math, config_dest, instr_mod1)

#define TT_OP_SFPDIVP2(imm12_math, lreg_c, lreg_dest, instr_mod1) \
  ((0x76 << 24) | ((imm12_math) << 12) | ((lreg_c) << 8) | ((lreg_dest) << 4) | ((instr_mod1) << 0))
#define TT_SFPDIVP2_VALID(imm12_math, lreg_c, lreg_dest, instr_mod1) \
  (ckernel::is_valid(imm12_math, 12) && ckernel::is_valid(lreg_c, 4) && ckernel::is_valid(lreg_dest, 4) && ckernel::is_valid(instr_mod1, 4))
#define TT_SFPDIVP2(imm12_math, lreg_c, lreg_dest, instr_mod1) \
  lltt::insn(TT_OP_SFPDIVP2(imm12_math, lreg_c, lreg_dest, instr_mod1))
#define TTI_SFPDIVP2(imm12_math, lreg_c, lreg_dest, instr_mod1) \
  TT_SFPDIVP2(imm12_math, lreg_c, lreg_dest, instr_mod1)

#define TT_OP_SFPENCC(imm12_math, lreg_c, lreg_dest, instr_mod1) \
  ((0x8a << 24) | ((imm12_math) << 12) | ((lreg_c) << 8) | ((lreg_dest) << 4) | ((instr_mod1) << 0))
#define TT_SFPENCC_VALID(imm12_math, lreg_c, lreg_dest, instr_mod1) \
  (ckernel::is_valid(imm12_math, 12) && ckernel::is_valid(lreg_c, 4) && ckernel::is_valid(lreg_dest, 4) && ckernel::is_valid(instr_mod1, 4))
#define TT_SFPENCC(imm12_math, lreg_c, lreg_dest, instr_mod1) \
  lltt::insn(TT_OP_SFPENCC(imm12_math, lreg_c, lreg_dest, instr_mod1))
#define TTI_SFPENCC(imm12_math, lreg_c, lreg_dest, instr_mod1) \
  TT_SFPENCC(imm12_math, lreg_c, lreg_dest, instr_mod1)

#define TT_OP_SFPEXEXP(imm12_math, lreg_c, lreg_dest, instr_mod1) \
  ((0x77 << 24) | ((imm12_math) << 12) | ((lreg_c) << 8) | ((lreg_dest) << 4) | ((instr_mod1) << 0))
#define TT_SFPEXEXP_VALID(imm12_math, lreg_c, lreg_dest, instr_mod1) \
  (ckernel::is_valid(imm12_math, 12) && ckernel::is_valid(lreg_c, 4) && ckernel::is_valid(lreg_dest, 4) && ckernel::is_valid(instr_mod1, 4))
#define TT_SFPEXEXP(imm12_math, lreg_c, lreg_dest, instr_mod1) \
  lltt::insn(TT_OP_SFPEXEXP(imm12_math, lreg_c, lreg_dest, instr_mod1))
#define TTI_SFPEXEXP(imm12_math, lreg_c, lreg_dest, instr_mod1) \
  TT_SFPEXEXP(imm12_math, lreg_c, lreg_dest, instr_mod1)

#define TT_OP_SFPEXMAN(imm12_math, lreg_c, lreg_dest, instr_mod1) \
  ((0x78 << 24) | ((imm12_math) << 12) | ((lreg_c) << 8) | ((lreg_dest) << 4) | ((instr_mod1) << 0))
#define TT_SFPEXMAN_VALID(imm12_math, lreg_c, lreg_dest, instr_mod1) \
  (ckernel::is_valid(imm12_math, 12) && ckernel::is_valid(lreg_c, 4) && ckernel::is_valid(lreg_dest, 4) && ckernel::is_valid(instr_mod1, 4))
#define TT_SFPEXMAN(imm12_math, lreg_c, lreg_dest, instr_mod1) \
  lltt::insn(TT_OP_SFPEXMAN(imm12_math, lreg_c, lreg_dest, instr_mod1))
#define TTI_SFPEXMAN(imm12_math, lreg_c, lreg_dest, instr_mod1) \
  TT_SFPEXMAN(imm12_math, lreg_c, lreg_dest, instr_mod1)

#define TT_OP_SFPGT(imm12_math, lreg_c, lreg_dest, instr_mod1) \
  ((0x97 << 24) | ((imm12_math) << 12) | ((lreg_c) << 8) | ((lreg_dest) << 4) | ((instr_mod1) << 0))
#define TT_SFPGT_VALID(imm12_math, lreg_c, lreg_dest, instr_mod1) \
  (ckernel::is_valid(imm12_math, 12) && ckernel::is_valid(lreg_c, 4) && ckernel::is_valid(lreg_dest, 4) && ckernel::is_valid(instr_mod1, 4))
#define TT_SFPGT(imm12_math, lreg_c, lreg_dest, instr_mod1) \
  lltt::insn(TT_OP_SFPGT(imm12_math, lreg_c, lreg_dest, instr_mod1))
#define TTI_SFPGT(imm12_math, lreg_c, lreg_dest, instr_mod1) \
  TT_SFPGT(imm12_math, lreg_c, lreg_dest, instr_mod1)

#define TT_OP_SFPIADD(imm12_math, lreg_c, lreg_dest, instr_mod1) \
  ((0x79 << 24) | ((imm12_math) << 12) | ((lreg_c) << 8) | ((lreg_dest) << 4) | ((instr_mod1) << 0))
#define TT_SFPIADD_VALID(imm12_math, lreg_c, lreg_dest, instr_mod1) \
  (ckernel::is_valid(imm12_math, 12) && ckernel::is_valid(lreg_c, 4) && ckernel::is_valid(lreg_dest, 4) && ckernel::is_valid(instr_mod1, 4))
#define TT_SFPIADD(imm12_math, lreg_c, lreg_dest, instr_mod1) \
  lltt::insn(TT_OP_SFPIADD(imm12_math, lreg_c, lreg_dest, instr_mod1))
#define TTI_SFPIADD(imm12_math, lreg_c, lreg_dest, instr_mod1) \
  TT_SFPIADD(imm12_math, lreg_c, lreg_dest, instr_mod1)

#define TT_OP_SFPLE(imm12_math, lreg_c, lreg_dest, instr_mod1) \
  ((0x96 << 24) | ((imm12_math) << 12) | ((lreg_c) << 8) | ((lreg_dest) << 4) | ((instr_mod1) << 0))
#define TT_SFPLE_VALID(imm12_math, lreg_c, lreg_dest, instr_mod1) \
  (ckernel::is_valid(imm12_math, 12) && ckernel::is_valid(lreg_c, 4) && ckernel::is_valid(lreg_dest, 4) && ckernel::is_valid(instr_mod1, 4))
#define TT_SFPLE(imm12_math, lreg_c, lreg_dest, instr_mod1) \
  lltt::insn(TT_OP_SFPLE(imm12_math, lreg_c, lreg_dest, instr_mod1))
#define TTI_SFPLE(imm12_math, lreg_c, lreg_dest, instr_mod1) \
  TT_SFPLE(imm12_math, lreg_c, lreg_dest, instr_mod1)

#define TT_OP_SFPLOAD(lreg_ind, instr_mod0, sfpu_addr_mode, dest_reg_addr) \
  ((0x70 << 24) | ((lreg_ind) << 20) | ((instr_mod0) << 16) | ((sfpu_addr_mode) << 13) | ((dest_reg_addr) << 0))
#define TT_SFPLOAD_VALID(lreg_ind, instr_mod0, sfpu_addr_mode, dest_reg_addr) \
  (ckernel::is_valid(lreg_ind, 4) && ckernel::is_valid(instr_mod0, 4) && ckernel::is_valid(sfpu_addr_mode, 3) && ckernel::is_valid(dest_reg_addr, 13))
#define TT_SFPLOAD(lreg_ind, instr_mod0, sfpu_addr_mode, dest_reg_addr) \
  lltt::insn(TT_OP_SFPLOAD(lreg_ind, instr_mod0, sfpu_addr_mode, dest_reg_addr))
#define TTI_SFPLOAD(lreg_ind, instr_mod0, sfpu_addr_mode, dest_reg_addr) \
  TT_SFPLOAD(lreg_ind, instr_mod0, sfpu_addr_mode, dest_reg_addr)

#define TT_OP_SFPLOADI(lreg_ind, instr_mod0, imm16) \
  ((0x71 << 24) | ((lreg_ind) << 20) | ((instr_mod0) << 16) | ((imm16) << 0))
#define TT_SFPLOADI_VALID(lreg_ind, instr_mod0, imm16) \
  (ckernel::is_valid(lreg_ind, 4) && ckernel::is_valid(instr_mod0, 4) && ckernel::is_valid(imm16, 16))
#define TT_SFPLOADI(lreg_ind, instr_mod0, imm16) \
  lltt::insn(TT_OP_SFPLOADI(lreg_ind, instr_mod0, imm16))
#define TTI_SFPLOADI(lreg_ind, instr_mod0, imm16) \
  TT_SFPLOADI(lreg_ind, instr_mod0, imm16)

#define TT_OP_SFPLOADMACRO(lreg_ind, instr_mod0, sfpu_addr_mode, dest_reg_addr) \
  ((0x93 << 24) | ((lreg_ind) << 20) | ((instr_mod0) << 16) | ((sfpu_addr_mode) << 13) | ((dest_reg_addr) << 0))
#define TT_SFPLOADMACRO_VALID(lreg_ind, instr_mod0, sfpu_addr_mode, dest_reg_addr) \
  (ckernel::is_valid(lreg_ind, 4) && ckernel::is_valid(instr_mod0, 4) && ckernel::is_valid(sfpu_addr_mode, 3) && ckernel::is_valid(dest_reg_addr, 13))
#define TT_SFPLOADMACRO(lreg_ind, instr_mod0, sfpu_addr_mode, dest_reg_addr) \
  lltt::insn(TT_OP_SFPLOADMACRO(lreg_ind, instr_mod0, sfpu_addr_mode, dest_reg_addr))
#define TTI_SFPLOADMACRO(lreg_ind, instr_mod0, sfpu_addr_mode, dest_reg_addr) \
  TT_SFPLOADMACRO(lreg_ind, instr_mod0, sfpu_addr_mode, dest_reg_addr)

#define TT_OP_SFPLUT(lreg_ind, instr_mod0, dest_reg_addr) \
  ((0x73 << 24) | ((lreg_ind) << 20) | ((instr_mod0) << 16) | ((dest_reg_addr) << 0))
#define TT_SFPLUT_VALID(lreg_ind, instr_mod0, dest_reg_addr) \
  (ckernel::is_valid(lreg_ind, 4) && ckernel::is_valid(instr_mod0, 4) && ckernel::is_valid(dest_reg_addr, 16))
#define TT_SFPLUT(lreg_ind, instr_mod0, dest_reg_addr) \
  lltt::insn(TT_OP_SFPLUT(lreg_ind, instr_mod0, dest_reg_addr))
#define TTI_SFPLUT(lreg_ind, instr_mod0, dest_reg_addr) \
  TT_SFPLUT(lreg_ind, instr_mod0, dest_reg_addr)

#define TT_OP_SFPLUTFP32(lreg_dest, instr_mod1) \
  ((0x95 << 24) | ((lreg_dest) << 4) | ((instr_mod1) << 0))
#define TT_SFPLUTFP32_VALID(lreg_dest, instr_mod1) \
  (ckernel::is_valid(lreg_dest, 20) && ckernel::is_valid(instr_mod1, 4))
#define TT_SFPLUTFP32(lreg_dest, instr_mod1) \
  lltt::insn(TT_OP_SFPLUTFP32(lreg_dest, instr_mod1))
#define TTI_SFPLUTFP32(lreg_dest, instr_mod1) \
  TT_SFPLUTFP32(lreg_dest, instr_mod1)

#define TT_OP_SFPLZ(imm12_math, lreg_c, lreg_dest, instr_mod1) \
  ((0x81 << 24) | ((imm12_math) << 12) | ((lreg_c) << 8) | ((lreg_dest) << 4) | ((instr_mod1) << 0))
#define TT_SFPLZ_VALID(imm12_math, lreg_c, lreg_dest, instr_mod1) \
  (ckernel::is_valid(imm12_math, 12) && ckernel::is_valid(lreg_c, 4) && ckernel::is_valid(lreg_dest, 4) && ckernel::is_valid(instr_mod1, 4))
#define TT_SFPLZ(imm12_math, lreg_c, lreg_dest, instr_mod1) \
  lltt::insn(TT_OP_SFPLZ(imm12_math, lreg_c, lreg_dest, instr_mod1))
#define TTI_SFPLZ(imm12_math, lreg_c, lreg_dest, instr_mod1) \
  TT_SFPLZ(imm12_math, lreg_c, lreg_dest, instr_mod1)

#define TT_OP_SFPMAD(lreg_src_a, lreg_src_b, lreg_src_c, lreg_dest, instr_mod1) \
  ((0x84 << 24) | ((lreg_src_a) << 16) | ((lreg_src_b) << 12) | ((lreg_src_c) << 8) | ((lreg_dest) << 4) | ((instr_mod1) << 0))
#define TT_SFPMAD_VALID(lreg_src_a, lreg_src_b, lreg_src_c, lreg_dest, instr_mod1) \
  (ckernel::is_valid(lreg_src_a, 8) && ckernel::is_valid(lreg_src_b, 4) && ckernel::is_valid(lreg_src_c, 4) && ckernel::is_valid(lreg_dest, 4) && ckernel::is_valid(instr_mod1, 4))
#define TT_SFPMAD(lreg_src_a, lreg_src_b, lreg_src_c, lreg_dest, instr_mod1) \
  lltt::insn(TT_OP_SFPMAD(lreg_src_a, lreg_src_b, lreg_src_c, lreg_dest, instr_mod1))
#define TTI_SFPMAD(lreg_src_a, lreg_src_b, lreg_src_c, lreg_dest, instr_mod1) \
  TT_SFPMAD(lreg_src_a, lreg_src_b, lreg_src_c, lreg_dest, instr_mod1)

#define TT_OP_SFPMOV(imm12_math, lreg_c, lreg_dest, instr_mod1) \
  ((0x7c << 24) | ((imm12_math) << 12) | ((lreg_c) << 8) | ((lreg_dest) << 4) | ((instr_mod1) << 0))
#define TT_SFPMOV_VALID(imm12_math, lreg_c, lreg_dest, instr_mod1) \
  (ckernel::is_valid(imm12_math, 12) && ckernel::is_valid(lreg_c, 4) && ckernel::is_valid(lreg_dest, 4) && ckernel::is_valid(instr_mod1, 4))
#define TT_SFPMOV(imm12_math, lreg_c, lreg_dest, instr_mod1) \
  lltt::insn(TT_OP_SFPMOV(imm12_math, lreg_c, lreg_dest, instr_mod1))
#define TTI_SFPMOV(imm12_math, lreg_c, lreg_dest, instr_mod1) \
  TT_SFPMOV(imm12_math, lreg_c, lreg_dest, instr_mod1)

#define TT_OP_SFPMUL(lreg_src_a, lreg_src_b, lreg_src_c, lreg_dest, instr_mod1) \
  ((0x86 << 24) | ((lreg_src_a) << 16) | ((lreg_src_b) << 12) | ((lreg_src_c) << 8) | ((lreg_dest) << 4) | ((instr_mod1) << 0))
#define TT_SFPMUL_VALID(lreg_src_a, lreg_src_b, lreg_src_c, lreg_dest, instr_mod1) \
  (ckernel::is_valid(lreg_src_a, 8) && ckernel::is_valid(lreg_src_b, 4) && ckernel::is_valid(lreg_src_c, 4) && ckernel::is_valid(lreg_dest, 4) && ckernel::is_valid(instr_mod1, 4))
#define TT_SFPMUL(lreg_src_a, lreg_src_b, lreg_src_c, lreg_dest, instr_mod1) \
  lltt::insn(TT_OP_SFPMUL(lreg_src_a, lreg_src_b, lreg_src_c, lreg_dest, instr_mod1))
#define TTI_SFPMUL(lreg_src_a, lreg_src_b, lreg_src_c, lreg_dest, instr_mod1) \
  TT_SFPMUL(lreg_src_a, lreg_src_b, lreg_src_c, lreg_dest, instr_mod1)

#define TT_OP_SFPMUL24(lreg_src_a, lreg_src_b, lreg_src_c, lreg_dest, instr_mod1) \
  ((0x98 << 24) | ((lreg_src_a) << 16) | ((lreg_src_b) << 12) | ((lreg_src_c) << 8) | ((lreg_dest) << 4) | ((instr_mod1) << 0))
#define TT_SFPMUL24_VALID(lreg_src_a, lreg_src_b, lreg_src_c, lreg_dest, instr_mod1) \
  (ckernel::is_valid(lreg_src_a, 8) && ckernel::is_valid(lreg_src_b, 4) && ckernel::is_valid(lreg_src_c, 4) && ckernel::is_valid(lreg_dest, 4) && ckernel::is_valid(instr_mod1, 4))
#define TT_SFPMUL24(lreg_src_a, lreg_src_b, lreg_src_c, lreg_dest, instr_mod1) \
  lltt::insn(TT_OP_SFPMUL24(lreg_src_a, lreg_src_b, lreg_src_c, lreg_dest, instr_mod1))
#define TTI_SFPMUL24(lreg_src_a, lreg_src_b, lreg_src_c, lreg_dest, instr_mod1) \
  TT_SFPMUL24(lreg_src_a, lreg_src_b, lreg_src_c, lreg_dest, instr_mod1)

#define TT_OP_SFPMULI(imm16_math, lreg_dest, instr_mod1) \
  ((0x74 << 24) | ((imm16_math) << 8) | ((lreg_dest) << 4) | ((instr_mod1) << 0))
#define TT_SFPMULI_VALID(imm16_math, lreg_dest, instr_mod1) \
  (ckernel::is_valid(imm16_math, 16) && ckernel::is_valid(lreg_dest, 4) && ckernel::is_valid(instr_mod1, 4))
#define TT_SFPMULI(imm16_math, lreg_dest, instr_mod1) \
  lltt::insn(TT_OP_SFPMULI(imm16_math, lreg_dest, instr_mod1))
#define TTI_SFPMULI(imm16_math, lreg_dest, instr_mod1) \
  TT_SFPMULI(imm16_math, lreg_dest, instr_mod1)

#define TT_OP_SFPNOP\
  TT_OP(0x8f, 0)
#define TTI_SFPNOP\
  INSTRUCTION_WORD(TT_OP_SFPNOP)

#define TT_OP_SFPNOT(imm12_math, lreg_c, lreg_dest, instr_mod1) \
  ((0x80 << 24) | ((imm12_math) << 12) | ((lreg_c) << 8) | ((lreg_dest) << 4) | ((instr_mod1) << 0))
#define TT_SFPNOT_VALID(imm12_math, lreg_c, lreg_dest, instr_mod1) \
  (ckernel::is_valid(imm12_math, 12) && ckernel::is_valid(lreg_c, 4) && ckernel::is_valid(lreg_dest, 4) && ckernel::is_valid(instr_mod1, 4))
#define TT_SFPNOT(imm12_math, lreg_c, lreg_dest, instr_mod1) \
  lltt::insn(TT_OP_SFPNOT(imm12_math, lreg_c, lreg_dest, instr_mod1))
#define TTI_SFPNOT(imm12_math, lreg_c, lreg_dest, instr_mod1) \
  TT_SFPNOT(imm12_math, lreg_c, lreg_dest, instr_mod1)

#define TT_OP_SFPOR(imm12_math, lreg_c, lreg_dest, instr_mod1) \
  ((0x7f << 24) | ((imm12_math) << 12) | ((lreg_c) << 8) | ((lreg_dest) << 4) | ((instr_mod1) << 0))
#define TT_SFPOR_VALID(imm12_math, lreg_c, lreg_dest, instr_mod1) \
  (ckernel::is_valid(imm12_math, 12) && ckernel::is_valid(lreg_c, 4) && ckernel::is_valid(lreg_dest, 4) && ckernel::is_valid(instr_mod1, 4))
#define TT_SFPOR(imm12_math, lreg_c, lreg_dest, instr_mod1) \
  lltt::insn(TT_OP_SFPOR(imm12_math, lreg_c, lreg_dest, instr_mod1))
#define TTI_SFPOR(imm12_math, lreg_c, lreg_dest, instr_mod1) \
  TT_SFPOR(imm12_math, lreg_c, lreg_dest, instr_mod1)

#define TT_OP_SFPPOPC(imm12_math, lreg_c, lreg_dest, instr_mod1) \
  ((0x88 << 24) | ((imm12_math) << 12) | ((lreg_c) << 8) | ((lreg_dest) << 4) | ((instr_mod1) << 0))
#define TT_SFPPOPC_VALID(imm12_math, lreg_c, lreg_dest, instr_mod1) \
  (ckernel::is_valid(imm12_math, 12) && ckernel::is_valid(lreg_c, 4) && ckernel::is_valid(lreg_dest, 4) && ckernel::is_valid(instr_mod1, 4))
#define TT_SFPPOPC(imm12_math, lreg_c, lreg_dest, instr_mod1) \
  lltt::insn(TT_OP_SFPPOPC(imm12_math, lreg_c, lreg_dest, instr_mod1))
#define TTI_SFPPOPC(imm12_math, lreg_c, lreg_dest, instr_mod1) \
  TT_SFPPOPC(imm12_math, lreg_c, lreg_dest, instr_mod1)

#define TT_OP_SFPPUSHC(imm12_math, lreg_c, lreg_dest, instr_mod1) \
  ((0x87 << 24) | ((imm12_math) << 12) | ((lreg_c) << 8) | ((lreg_dest) << 4) | ((instr_mod1) << 0))
#define TT_SFPPUSHC_VALID(imm12_math, lreg_c, lreg_dest, instr_mod1) \
  (ckernel::is_valid(imm12_math, 12) && ckernel::is_valid(lreg_c, 4) && ckernel::is_valid(lreg_dest, 4) && ckernel::is_valid(instr_mod1, 4))
#define TT_SFPPUSHC(imm12_math, lreg_c, lreg_dest, instr_mod1) \
  lltt::insn(TT_OP_SFPPUSHC(imm12_math, lreg_c, lreg_dest, instr_mod1))
#define TTI_SFPPUSHC(imm12_math, lreg_c, lreg_dest, instr_mod1) \
  TT_SFPPUSHC(imm12_math, lreg_c, lreg_dest, instr_mod1)

#define TT_OP_SFPSETCC(imm12_math, lreg_c, lreg_dest, instr_mod1) \
  ((0x7b << 24) | ((imm12_math) << 12) | ((lreg_c) << 8) | ((lreg_dest) << 4) | ((instr_mod1) << 0))
#define TT_SFPSETCC_VALID(imm12_math, lreg_c, lreg_dest, instr_mod1) \
  (ckernel::is_valid(imm12_math, 12) && ckernel::is_valid(lreg_c, 4) && ckernel::is_valid(lreg_dest, 4) && ckernel::is_valid(instr_mod1, 4))
#define TT_SFPSETCC(imm12_math, lreg_c, lreg_dest, instr_mod1) \
  lltt::insn(TT_OP_SFPSETCC(imm12_math, lreg_c, lreg_dest, instr_mod1))
#define TTI_SFPSETCC(imm12_math, lreg_c, lreg_dest, instr_mod1) \
  TT_SFPSETCC(imm12_math, lreg_c, lreg_dest, instr_mod1)

#define TT_OP_SFPSETEXP(imm12_math, lreg_c, lreg_dest, instr_mod1) \
  ((0x82 << 24) | ((imm12_math) << 12) | ((lreg_c) << 8) | ((lreg_dest) << 4) | ((instr_mod1) << 0))
#define TT_SFPSETEXP_VALID(imm12_math, lreg_c, lreg_dest, instr_mod1) \
  (ckernel::is_valid(imm12_math, 12) && ckernel::is_valid(lreg_c, 4) && ckernel::is_valid(lreg_dest, 4) && ckernel::is_valid(instr_mod1, 4))
#define TT_SFPSETEXP(imm12_math, lreg_c, lreg_dest, instr_mod1) \
  lltt::insn(TT_OP_SFPSETEXP(imm12_math, lreg_c, lreg_dest, instr_mod1))
#define TTI_SFPSETEXP(imm12_math, lreg_c, lreg_dest, instr_mod1) \
  TT_SFPSETEXP(imm12_math, lreg_c, lreg_dest, instr_mod1)

#define TT_OP_SFPSETMAN(imm12_math, lreg_c, lreg_dest, instr_mod1) \
  ((0x83 << 24) | ((imm12_math) << 12) | ((lreg_c) << 8) | ((lreg_dest) << 4) | ((instr_mod1) << 0))
#define TT_SFPSETMAN_VALID(imm12_math, lreg_c, lreg_dest, instr_mod1) \
  (ckernel::is_valid(imm12_math, 12) && ckernel::is_valid(lreg_c, 4) && ckernel::is_valid(lreg_dest, 4) && ckernel::is_valid(instr_mod1, 4))
#define TT_SFPSETMAN(imm12_math, lreg_c, lreg_dest, instr_mod1) \
  lltt::insn(TT_OP_SFPSETMAN(imm12_math, lreg_c, lreg_dest, instr_mod1))
#define TTI_SFPSETMAN(imm12_math, lreg_c, lreg_dest, instr_mod1) \
  TT_SFPSETMAN(imm12_math, lreg_c, lreg_dest, instr_mod1)

#define TT_OP_SFPSETSGN(imm12_math, lreg_c, lreg_dest, instr_mod1) \
  ((0x89 << 24) | ((imm12_math) << 12) | ((lreg_c) << 8) | ((lreg_dest) << 4) | ((instr_mod1) << 0))
#define TT_SFPSETSGN_VALID(imm12_math, lreg_c, lreg_dest, instr_mod1) \
  (ckernel::is_valid(imm12_math, 12) && ckernel::is_valid(lreg_c, 4) && ckernel::is_valid(lreg_dest, 4) && ckernel::is_valid(instr_mod1, 4))
#define TT_SFPSETSGN(imm12_math, lreg_c, lreg_dest, instr_mod1) \
  lltt::insn(TT_OP_SFPSETSGN(imm12_math, lreg_c, lreg_dest, instr_mod1))
#define TTI_SFPSETSGN(imm12_math, lreg_c, lreg_dest, instr_mod1) \
  TT_SFPSETSGN(imm12_math, lreg_c, lreg_dest, instr_mod1)

#define TT_OP_SFPSHFT(imm12_math, lreg_c, lreg_dest, instr_mod1) \
  ((0x7a << 24) | ((imm12_math) << 12) | ((lreg_c) << 8) | ((lreg_dest) << 4) | ((instr_mod1) << 0))
#define TT_SFPSHFT_VALID(imm12_math, lreg_c, lreg_dest, instr_mod1) \
  (ckernel::is_valid(imm12_math, 12) && ckernel::is_valid(lreg_c, 4) && ckernel::is_valid(lreg_dest, 4) && ckernel::is_valid(instr_mod1, 4))
#define TT_SFPSHFT(imm12_math, lreg_c, lreg_dest, instr_mod1) \
  lltt::insn(TT_OP_SFPSHFT(imm12_math, lreg_c, lreg_dest, instr_mod1))
#define TTI_SFPSHFT(imm12_math, lreg_c, lreg_dest, instr_mod1) \
  TT_SFPSHFT(imm12_math, lreg_c, lreg_dest, instr_mod1)

#define TT_OP_SFPSHFT2(imm12_math, lreg_src_c, lreg_dest, instr_mod1) \
  ((0x94 << 24) | ((imm12_math) << 12) | ((lreg_src_c) << 8) | ((lreg_dest) << 4) | ((instr_mod1) << 0))
#define TT_SFPSHFT2_VALID(imm12_math, lreg_src_c, lreg_dest, instr_mod1) \
  (ckernel::is_valid(imm12_math, 12) && ckernel::is_valid(lreg_src_c, 4) && ckernel::is_valid(lreg_dest, 4) && ckernel::is_valid(instr_mod1, 4))
#define TT_SFPSHFT2(imm12_math, lreg_src_c, lreg_dest, instr_mod1) \
  lltt::insn(TT_OP_SFPSHFT2(imm12_math, lreg_src_c, lreg_dest, instr_mod1))
#define TTI_SFPSHFT2(imm12_math, lreg_src_c, lreg_dest, instr_mod1) \
  TT_SFPSHFT2(imm12_math, lreg_src_c, lreg_dest, instr_mod1)

#define TT_OP_SFPSTORE(lreg_ind, instr_mod0, sfpu_addr_mode, dest_reg_addr) \
  ((0x72 << 24) | ((lreg_ind) << 20) | ((instr_mod0) << 16) | ((sfpu_addr_mode) << 13) | ((dest_reg_addr) << 0))
#define TT_SFPSTORE_VALID(lreg_ind, instr_mod0, sfpu_addr_mode, dest_reg_addr) \
  (ckernel::is_valid(lreg_ind, 4) && ckernel::is_valid(instr_mod0, 4) && ckernel::is_valid(sfpu_addr_mode, 3) && ckernel::is_valid(dest_reg_addr, 13))
#define TT_SFPSTORE(lreg_ind, instr_mod0, sfpu_addr_mode, dest_reg_addr) \
  lltt::insn(TT_OP_SFPSTORE(lreg_ind, instr_mod0, sfpu_addr_mode, dest_reg_addr))
#define TTI_SFPSTORE(lreg_ind, instr_mod0, sfpu_addr_mode, dest_reg_addr) \
  TT_SFPSTORE(lreg_ind, instr_mod0, sfpu_addr_mode, dest_reg_addr)

#define TT_OP_SFPSWAP(imm12_math, lreg_src_c, lreg_dest, instr_mod1) \
  ((0x92 << 24) | ((imm12_math) << 12) | ((lreg_src_c) << 8) | ((lreg_dest) << 4) | ((instr_mod1) << 0))
#define TT_SFPSWAP_VALID(imm12_math, lreg_src_c, lreg_dest, instr_mod1) \
  (ckernel::is_valid(imm12_math, 12) && ckernel::is_valid(lreg_src_c, 4) && ckernel::is_valid(lreg_dest, 4) && ckernel::is_valid(instr_mod1, 4))
#define TT_SFPSWAP(imm12_math, lreg_src_c, lreg_dest, instr_mod1) \
  lltt::insn(TT_OP_SFPSWAP(imm12_math, lreg_src_c, lreg_dest, instr_mod1))
#define TTI_SFPSWAP(imm12_math, lreg_src_c, lreg_dest, instr_mod1) \
  TT_SFPSWAP(imm12_math, lreg_src_c, lreg_dest, instr_mod1)

#define TT_OP_SFPTRANSP(imm12_math, lreg_c, lreg_dest, instr_mod1) \
  ((0x8c << 24) | ((imm12_math) << 12) | ((lreg_c) << 8) | ((lreg_dest) << 4) | ((instr_mod1) << 0))
#define TT_SFPTRANSP_VALID(imm12_math, lreg_c, lreg_dest, instr_mod1) \
  (ckernel::is_valid(imm12_math, 12) && ckernel::is_valid(lreg_c, 4) && ckernel::is_valid(lreg_dest, 4) && ckernel::is_valid(instr_mod1, 4))
#define TT_SFPTRANSP(imm12_math, lreg_c, lreg_dest, instr_mod1) \
  lltt::insn(TT_OP_SFPTRANSP(imm12_math, lreg_c, lreg_dest, instr_mod1))
#define TTI_SFPTRANSP(imm12_math, lreg_c, lreg_dest, instr_mod1) \
  TT_SFPTRANSP(imm12_math, lreg_c, lreg_dest, instr_mod1)

#define TT_OP_SFPXOR(imm12_math, lreg_c, lreg_dest, instr_mod1) \
  ((0x8d << 24) | ((imm12_math) << 12) | ((lreg_c) << 8) | ((lreg_dest) << 4) | ((instr_mod1) << 0))
#define TT_SFPXOR_VALID(imm12_math, lreg_c, lreg_dest, instr_mod1) \
  (ckernel::is_valid(imm12_math, 12) && ckernel::is_valid(lreg_c, 4) && ckernel::is_valid(lreg_dest, 4) && ckernel::is_valid(instr_mod1, 4))
#define TT_SFPXOR(imm12_math, lreg_c, lreg_dest, instr_mod1) \
  lltt::insn(TT_OP_SFPXOR(imm12_math, lreg_c, lreg_dest, instr_mod1))
#define TTI_SFPXOR(imm12_math, lreg_c, lreg_dest, instr_mod1) \
  TT_SFPXOR(imm12_math, lreg_c, lreg_dest, instr_mod1)

#define TT_OP_SFP_STOCH_RND(rnd_mode, imm8_math, lreg_src_b, lreg_src_c, lreg_dest, instr_mod1) \
  ((0x8e << 24) | ((rnd_mode) << 21) | ((imm8_math) << 16) | ((lreg_src_b) << 12) | ((lreg_src_c) << 8) | ((lreg_dest) << 4) | ((instr_mod1) << 0))
#define TT_SFP_STOCH_RND_VALID(rnd_mode, imm8_math, lreg_src_b, lreg_src_c, lreg_dest, instr_mod1) \
  (ckernel::is_valid(rnd_mode, 3) && ckernel::is_valid(imm8_math, 5) && ckernel::is_valid(lreg_src_b, 4) && ckernel::is_valid(lreg_src_c, 4) && ckernel::is_valid(lreg_dest, 4) && ckernel::is_valid(instr_mod1, 4))
#define TT_SFP_STOCH_RND(rnd_mode, imm8_math, lreg_src_b, lreg_src_c, lreg_dest, instr_mod1) \
  lltt::insn(TT_OP_SFP_STOCH_RND(rnd_mode, imm8_math, lreg_src_b, lreg_src_c, lreg_dest, instr_mod1))
#define TTI_SFP_STOCH_RND(rnd_mode, imm8_math, lreg_src_b, lreg_src_c, lreg_dest, instr_mod1) \
  TT_SFP_STOCH_RND(rnd_mode, imm8_math, lreg_src_b, lreg_src_c, lreg_dest, instr_mod1)

#define TT_OP_SHIFTDMAREG(OpBisConst, OpSel, ResultRegIndex, OpBRegIndex, OpARegIndex) \
  ((0x5c << 24) | ((OpBisConst) << 23) | ((OpSel) << 18) | ((ResultRegIndex) << 12) | ((OpBRegIndex) << 6) | ((OpARegIndex) << 0))
#define TT_SHIFTDMAREG_VALID(OpBisConst, OpSel, ResultRegIndex, OpBRegIndex, OpARegIndex) \
  (ckernel::is_valid(OpBisConst, 1) && ckernel::is_valid(OpSel, 5) && ckernel::is_valid(ResultRegIndex, 6) && ckernel::is_valid(OpBRegIndex, 6) && ckernel::is_valid(OpARegIndex, 6))
#define TT_SHIFTDMAREG(OpBisConst, OpSel, ResultRegIndex, OpBRegIndex, OpARegIndex) \
  lltt::insn(TT_OP_SHIFTDMAREG(OpBisConst, OpSel, ResultRegIndex, OpBRegIndex, OpARegIndex))
#define TTI_SHIFTDMAREG(OpBisConst, OpSel, ResultRegIndex, OpBRegIndex, OpARegIndex) \
  TT_SHIFTDMAREG(OpBisConst, OpSel, ResultRegIndex, OpBRegIndex, OpARegIndex)

#define TT_OP_SHIFTXA(log2_amount2, shift_mode) \
  ((0x17 << 24) | ((log2_amount2) << 2) | ((shift_mode) << 0))
#define TT_SHIFTXA_VALID(log2_amount2, shift_mode) \
  (ckernel::is_valid(log2_amount2, 22) && ckernel::is_valid(shift_mode, 2))
#define TT_SHIFTXA(log2_amount2, shift_mode) \
  lltt::insn(TT_OP_SHIFTXA(log2_amount2, shift_mode))
#define TTI_SHIFTXA(log2_amount2, shift_mode) \
  TT_SHIFTXA(log2_amount2, shift_mode)

#define TT_OP_SHIFTXB(addr_mode, rot_shift, shift_row) \
  ((0x18 << 24) | ((addr_mode) << 14) | ((rot_shift) << 10) | ((shift_row) << 0))
#define TT_SHIFTXB_VALID(addr_mode, rot_shift, shift_row) \
  (ckernel::is_valid(addr_mode, 10) && ckernel::is_valid(rot_shift, 4) && ckernel::is_valid(shift_row, 10))
#define TT_SHIFTXB(addr_mode, rot_shift, shift_row) \
  lltt::insn(TT_OP_SHIFTXB(addr_mode, rot_shift, shift_row))
#define TTI_SHIFTXB(addr_mode, rot_shift, shift_row) \
  TT_SHIFTXB(addr_mode, rot_shift, shift_row)

#define TT_OP_STALLWAIT(stall_res, wait_res) \
  ((0xa2 << 24) | ((stall_res) << 15) | ((wait_res) << 0))
#define TT_STALLWAIT_VALID(stall_res, wait_res) \
  (ckernel::is_valid(stall_res, 9) && ckernel::is_valid(wait_res, 15))
#define TT_STALLWAIT(stall_res, wait_res) \
  lltt::insn(TT_OP_STALLWAIT(stall_res, wait_res))
#define TTI_STALLWAIT(stall_res, wait_res) \
  TT_STALLWAIT(stall_res, wait_res)

#define TT_OP_STOREIND(MemHierSel, SizeSel, RegSizeSel, OffsetIndex, AutoIncSpec, DataRegIndex, AddrRegIndex) \
  ((0x66 << 24) | ((MemHierSel) << 23) | ((SizeSel) << 22) | ((RegSizeSel) << 21) | ((OffsetIndex) << 14) | ((AutoIncSpec) << 12) | ((DataRegIndex) << 6) | ((AddrRegIndex) << 0))
#define TT_STOREIND_VALID(MemHierSel, SizeSel, RegSizeSel, OffsetIndex, AutoIncSpec, DataRegIndex, AddrRegIndex) \
  (ckernel::is_valid(MemHierSel, 1) && ckernel::is_valid(SizeSel, 1) && ckernel::is_valid(RegSizeSel, 1) && ckernel::is_valid(OffsetIndex, 7) && ckernel::is_valid(AutoIncSpec, 2) && ckernel::is_valid(DataRegIndex, 6) && ckernel::is_valid(AddrRegIndex, 6))
#define TT_STOREIND(MemHierSel, SizeSel, RegSizeSel, OffsetIndex, AutoIncSpec, DataRegIndex, AddrRegIndex) \
  lltt::insn(TT_OP_STOREIND(MemHierSel, SizeSel, RegSizeSel, OffsetIndex, AutoIncSpec, DataRegIndex, AddrRegIndex))
#define TTI_STOREIND(MemHierSel, SizeSel, RegSizeSel, OffsetIndex, AutoIncSpec, DataRegIndex, AddrRegIndex) \
  TT_STOREIND(MemHierSel, SizeSel, RegSizeSel, OffsetIndex, AutoIncSpec, DataRegIndex, AddrRegIndex)

#define TT_OP_STOREREG(TdmaDataRegIndex, RegAddr) \
  ((0x67 << 24) | ((TdmaDataRegIndex) << 18) | ((RegAddr) << 0))
#define TT_STOREREG_VALID(TdmaDataRegIndex, RegAddr) \
  (ckernel::is_valid(TdmaDataRegIndex, 6) && ckernel::is_valid(RegAddr, 18))
#define TT_STOREREG(TdmaDataRegIndex, RegAddr) \
  lltt::insn(TT_OP_STOREREG(TdmaDataRegIndex, RegAddr))
#define TTI_STOREREG(TdmaDataRegIndex, RegAddr) \
  TT_STOREREG(TdmaDataRegIndex, RegAddr)

#define TT_OP_STREAMWAIT(stall_res, target_value, target_sel, wait_stream_sel) \
  ((0xa7 << 24) | ((stall_res) << 15) | ((target_value) << 4) | ((target_sel) << 3) | ((wait_stream_sel) << 0))
#define TT_STREAMWAIT_VALID(stall_res, target_value, target_sel, wait_stream_sel) \
  (ckernel::is_valid(stall_res, 9) && ckernel::is_valid(target_value, 11) && ckernel::is_valid(target_sel, 1) && ckernel::is_valid(wait_stream_sel, 3))
#define TT_STREAMWAIT(stall_res, target_value, target_sel, wait_stream_sel) \
  lltt::insn(TT_OP_STREAMWAIT(stall_res, target_value, target_sel, wait_stream_sel))
#define TTI_STREAMWAIT(stall_res, target_value, target_sel, wait_stream_sel) \
  TT_STREAMWAIT(stall_res, target_value, target_sel, wait_stream_sel)

#define TT_OP_STREAMWRCFG(stream_id_sel, StreamRegAddr, CfgReg) \
  ((0xb7 << 24) | ((stream_id_sel) << 21) | ((StreamRegAddr) << 11) | ((CfgReg) << 0))
#define TT_STREAMWRCFG_VALID(stream_id_sel, StreamRegAddr, CfgReg) \
  (ckernel::is_valid(stream_id_sel, 3) && ckernel::is_valid(StreamRegAddr, 10) && ckernel::is_valid(CfgReg, 11))
#define TT_STREAMWRCFG(stream_id_sel, StreamRegAddr, CfgReg) \
  lltt::insn(TT_OP_STREAMWRCFG(stream_id_sel, StreamRegAddr, CfgReg))
#define TTI_STREAMWRCFG(stream_id_sel, StreamRegAddr, CfgReg) \
  TT_STREAMWRCFG(stream_id_sel, StreamRegAddr, CfgReg)

#define TT_OP_SUBDMAREG(OpBisConst, ResultRegIndex, OpBRegIndex, OpARegIndex) \
  ((0x59 << 24) | ((OpBisConst) << 23) | ((ResultRegIndex) << 12) | ((OpBRegIndex) << 6) | ((OpARegIndex) << 0))
#define TT_SUBDMAREG_VALID(OpBisConst, ResultRegIndex, OpBRegIndex, OpARegIndex) \
  (ckernel::is_valid(OpBisConst, 1) && ckernel::is_valid(ResultRegIndex, 11) && ckernel::is_valid(OpBRegIndex, 6) && ckernel::is_valid(OpARegIndex, 6))
#define TT_SUBDMAREG(OpBisConst, ResultRegIndex, OpBRegIndex, OpARegIndex) \
  lltt::insn(TT_OP_SUBDMAREG(OpBisConst, ResultRegIndex, OpBRegIndex, OpARegIndex))
#define TTI_SUBDMAREG(OpBisConst, ResultRegIndex, OpBRegIndex, OpARegIndex) \
  TT_SUBDMAREG(OpBisConst, ResultRegIndex, OpBRegIndex, OpARegIndex)

#define TT_OP_TBUFCMD\
  TT_OP(0x4b, 0)
#define TTI_TBUFCMD\
  INSTRUCTION_WORD(TT_OP_TBUFCMD)

#define TT_OP_TRNSPSRCA\
  TT_OP(0x14, 0)
#define TTI_TRNSPSRCA\
  INSTRUCTION_WORD(TT_OP_TRNSPSRCA)

#define TT_OP_TRNSPSRCB\
  TT_OP(0x16, 0)
#define TTI_TRNSPSRCB\
  INSTRUCTION_WORD(TT_OP_TRNSPSRCB)

#define TT_OP_UNPACR(Unpack_block_selection, AddrMode, CfgContextCntInc, CfgContextId, AddrCntContextId, OvrdThreadId, SetDatValid, srcb_bcast, ZeroWrite2, AutoIncContextID, RowSearch, SearchCacheFlush, Last) \
  ((0x42 << 24) | ((Unpack_block_selection) << 23) | ((AddrMode) << 15) | ((CfgContextCntInc) << 13) | ((CfgContextId) << 10) | ((AddrCntContextId) << 8) | ((OvrdThreadId) << 7) | ((SetDatValid) << 6) | ((srcb_bcast) << 5) | ((ZeroWrite2) << 4) | ((AutoIncContextID) << 3) | ((RowSearch) << 2) | ((SearchCacheFlush) << 1) | ((Last) << 0))
#define TT_UNPACR_VALID(Unpack_block_selection, AddrMode, CfgContextCntInc, CfgContextId, AddrCntContextId, OvrdThreadId, SetDatValid, srcb_bcast, ZeroWrite2, AutoIncContextID, RowSearch, SearchCacheFlush, Last) \
  (ckernel::is_valid(Unpack_block_selection, 1) && ckernel::is_valid(AddrMode, 8) && ckernel::is_valid(CfgContextCntInc, 2) && ckernel::is_valid(CfgContextId, 3) && ckernel::is_valid(AddrCntContextId, 2) && ckernel::is_valid(OvrdThreadId, 1) && ckernel::is_valid(SetDatValid, 1) && ckernel::is_valid(srcb_bcast, 1) && ckernel::is_valid(ZeroWrite2, 1) && ckernel::is_valid(AutoIncContextID, 1) && ckernel::is_valid(RowSearch, 1) && ckernel::is_valid(SearchCacheFlush, 1) && ckernel::is_valid(Last, 1))
#define TT_UNPACR(Unpack_block_selection, AddrMode, CfgContextCntInc, CfgContextId, AddrCntContextId, OvrdThreadId, SetDatValid, srcb_bcast, ZeroWrite2, AutoIncContextID, RowSearch, SearchCacheFlush, Last) \
  lltt::insn(TT_OP_UNPACR(Unpack_block_selection, AddrMode, CfgContextCntInc, CfgContextId, AddrCntContextId, OvrdThreadId, SetDatValid, srcb_bcast, ZeroWrite2, AutoIncContextID, RowSearch, SearchCacheFlush, Last))
#define TTI_UNPACR(Unpack_block_selection, AddrMode, CfgContextCntInc, CfgContextId, AddrCntContextId, OvrdThreadId, SetDatValid, srcb_bcast, ZeroWrite2, AutoIncContextID, RowSearch, SearchCacheFlush, Last) \
  TT_UNPACR(Unpack_block_selection, AddrMode, CfgContextCntInc, CfgContextId, AddrCntContextId, OvrdThreadId, SetDatValid, srcb_bcast, ZeroWrite2, AutoIncContextID, RowSearch, SearchCacheFlush, Last)

#define TT_OP_UNPACR_NOP(Unpacker_Select, Stream_Id, Msg_Clr_Cnt, Set_Dvalid, Clr_to1_fmt_Ctrl, Stall_Clr_Cntrl, Bank_Clr_Ctrl, Src_ClrVal_Ctrl, Unpack_Pop) \
  ((0x43 << 24) | ((Unpacker_Select) << 23) | ((Stream_Id) << 16) | ((Msg_Clr_Cnt) << 12) | ((Set_Dvalid) << 8) | ((Clr_to1_fmt_Ctrl) << 6) | ((Stall_Clr_Cntrl) << 5) | ((Bank_Clr_Ctrl) << 4) | ((Src_ClrVal_Ctrl) << 2) | ((Unpack_Pop) << 0))
#define TT_UNPACR_NOP_VALID(Unpacker_Select, Stream_Id, Msg_Clr_Cnt, Set_Dvalid, Clr_to1_fmt_Ctrl, Stall_Clr_Cntrl, Bank_Clr_Ctrl, Src_ClrVal_Ctrl, Unpack_Pop) \
  (ckernel::is_valid(Unpacker_Select, 1) && ckernel::is_valid(Stream_Id, 7) && ckernel::is_valid(Msg_Clr_Cnt, 4) && ckernel::is_valid(Set_Dvalid, 4) && ckernel::is_valid(Clr_to1_fmt_Ctrl, 2) && ckernel::is_valid(Stall_Clr_Cntrl, 1) && ckernel::is_valid(Bank_Clr_Ctrl, 1) && ckernel::is_valid(Src_ClrVal_Ctrl, 2) && ckernel::is_valid(Unpack_Pop, 2))
#define TT_UNPACR_NOP(Unpacker_Select, Stream_Id, Msg_Clr_Cnt, Set_Dvalid, Clr_to1_fmt_Ctrl, Stall_Clr_Cntrl, Bank_Clr_Ctrl, Src_ClrVal_Ctrl, Unpack_Pop) \
  lltt::insn(TT_OP_UNPACR_NOP(Unpacker_Select, Stream_Id, Msg_Clr_Cnt, Set_Dvalid, Clr_to1_fmt_Ctrl, Stall_Clr_Cntrl, Bank_Clr_Ctrl, Src_ClrVal_Ctrl, Unpack_Pop))
#define TTI_UNPACR_NOP(Unpacker_Select, Stream_Id, Msg_Clr_Cnt, Set_Dvalid, Clr_to1_fmt_Ctrl, Stall_Clr_Cntrl, Bank_Clr_Ctrl, Src_ClrVal_Ctrl, Unpack_Pop) \
  TT_UNPACR_NOP(Unpacker_Select, Stream_Id, Msg_Clr_Cnt, Set_Dvalid, Clr_to1_fmt_Ctrl, Stall_Clr_Cntrl, Bank_Clr_Ctrl, Src_ClrVal_Ctrl, Unpack_Pop)

#define TT_OP_WRCFG(GprAddress, wr128b, CfgReg) \
  ((0xb0 << 24) | ((GprAddress) << 16) | ((wr128b) << 15) | ((CfgReg) << 0))
#define TT_WRCFG_VALID(GprAddress, wr128b, CfgReg) \
  (ckernel::is_valid(GprAddress, 8) && ckernel::is_valid(wr128b, 1) && ckernel::is_valid(CfgReg, 15))
#define TT_WRCFG(GprAddress, wr128b, CfgReg) \
  lltt::insn(TT_OP_WRCFG(GprAddress, wr128b, CfgReg))
#define TTI_WRCFG(GprAddress, wr128b, CfgReg) \
  TT_WRCFG(GprAddress, wr128b, CfgReg)

#define TT_OP_XMOV(Mov_block_selection, Last) \
  ((0x40 << 24) | ((Mov_block_selection) << 23) | ((Last) << 0))
#define TT_XMOV_VALID(Mov_block_selection, Last) \
  (ckernel::is_valid(Mov block selection, 1) && ckernel::is_valid(Last, 23))
#define TT_XMOV(Mov_block_selection, Last) \
  lltt::insn(TT_OP_XMOV(Mov_block_selection, Last))
#define TTI_XMOV(Mov_block_selection, Last) \
  TT_XMOV(Mov_block_selection, Last)

#define TT_OP_ZEROACC(clear_mode, use_32_bit_mode, clear_zero_flags, addr_mode, where) \
  ((0x10 << 24) | ((clear_mode) << 19) | ((use_32_bit_mode) << 18) | ((clear_zero_flags) << 17) | ((addr_mode) << 14) | ((where) << 0))
#define TT_ZEROACC_VALID(clear_mode, use_32_bit_mode, clear_zero_flags, addr_mode, where) \
  (ckernel::is_valid(clear_mode, 5) && ckernel::is_valid(use_32_bit_mode, 1) && ckernel::is_valid(clear_zero_flags, 1) && ckernel::is_valid(addr_mode, 3) && ckernel::is_valid(where, 14))
#define TT_ZEROACC(clear_mode, use_32_bit_mode, clear_zero_flags, addr_mode, where) \
  lltt::insn(TT_OP_ZEROACC(clear_mode, use_32_bit_mode, clear_zero_flags, addr_mode, where))
#define TTI_ZEROACC(clear_mode, use_32_bit_mode, clear_zero_flags, addr_mode, where) \
  TT_ZEROACC(clear_mode, use_32_bit_mode, clear_zero_flags, addr_mode, where)

#define TT_OP_ZEROSRC(zero_val, write_mode, bank_mask, src_mask) \
  ((0x11 << 24) | ((zero_val) << 4) | ((write_mode) << 3) | ((bank_mask) << 2) | ((src_mask) << 0))
#define TT_ZEROSRC_VALID(zero_val, write_mode, bank_mask, src_mask) \
  (ckernel::is_valid(zero_val, 20) && ckernel::is_valid(write_mode, 1) && ckernel::is_valid(bank_mask, 1) && ckernel::is_valid(src_mask, 2))
#define TT_ZEROSRC(zero_val, write_mode, bank_mask, src_mask) \
  lltt::insn(TT_OP_ZEROSRC(zero_val, write_mode, bank_mask, src_mask))
#define TTI_ZEROSRC(zero_val, write_mode, bank_mask, src_mask) \
  TT_ZEROSRC(zero_val, write_mode, bank_mask, src_mask)

#else
#error "Unknown TT arch"
#endif

[[gnu::always_inline]] constexpr std::uint32_t
lltt::replay_insn(unsigned start, unsigned length) {
  return TT_OP_REPLAY(start, length, 0, 0);
}
