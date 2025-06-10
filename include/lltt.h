/*
 * SPDX-FileCopyrightText: © 2025 Tenstorrent Inc.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

// Low level tt-insn interface.

#pragma once

#if __riscv_xtttensixwh || __riscv_xtttensixbh

#include <cstdint>

#define __builtin_rvtt_ttreplay(START, LENGTH, EXEC, RECORD) \
  __builtin_rvtt_ttreplay(lltt::__instrn_buffer, LENGTH, 0, 0, START, EXEC, RECORD)
#define __builtin_rvtt_ttinsn(STATIC, ENCODING) \
  __builtin_rvtt_ttinsn(lltt::__instrn_buffer, STATIC, ENCODING)

extern volatile uint32_t __instrn_buffer[];

namespace lltt {
constexpr inline volatile uint32_t *[[gnu::rvtt_reg_ptr]] __instrn_buffer = ::__instrn_buffer;

enum ExecBool : bool {NoExec, Exec};

template<ExecBool E = NoExec>
[[gnu::always_inline]] inline void
record(unsigned start, unsigned length) {
  __builtin_rvtt_ttreplay(start, length, bool(E), true);
}

[[gnu::always_inline]] inline void replay(unsigned start, unsigned length) {
  __builtin_rvtt_ttreplay(start, length, false, false);
}

[[gnu::always_inline]] constexpr std::uint32_t
replay_insn(unsigned start, unsigned length) {
  // Perhaps another builtin?
  return (0x04 << 24) | (start << 14) | (length << 4);
}

template<bool Static=false>
[[gnu::always_inline]] inline void insn(uint32_t insn) {
  __builtin_rvtt_ttinsn(Static, insn);
}

// For use by ckernel_ops.h expansion
#define LLTT_INSN(STATIC, ENCODING) ::lltt::insn<STATIC>(ENCODING)

} // namespace 

#endif
