/*
 * SPDX-FileCopyrightText: © 2025 Tenstorrent Inc.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

// Low level tt-insn interface.

#pragma once

#include <cstdint>

#if !__riscv_tt_wormhole && !__riscv_tt_blackhole
#error "A TT architecture must be selected"
#include "stop now, no good will come"
#endif

extern volatile uint32_t __instrn_buffer[];

namespace lltt {
constexpr inline volatile uint32_t *[[gnu::rvtt_reg_ptr]] instrn_buffer = ::__instrn_buffer;

enum ExecBool : bool {NoExec, Exec};

template<ExecBool E = NoExec>
[[gnu::always_inline]] inline void
record(unsigned start, unsigned length) {
  __builtin_rvtt_ttreplay(start, length, bool(E), true);
}

[[gnu::always_inline]] inline void replay(unsigned start, unsigned length) {
  __builtin_rvtt_ttreplay(start, length, false, false);
}

[[gnu::always_inline]] constexpr std::uint32_t replay_insn(unsigned start, unsigned length) {
  return TT_OP_REPLAY(start, length, 0, 0);
}

[[gnu::always_inline]] inline void insn(uint32_t insn) {
  __builtin_rvtt_ttinsn((void *)&instrn_buffer[0], insn);
}

// For use by ckernel_ops.h expansion
#define LLTT_INSN(ENCODING) ::lltt::insn(ENCODING)

} // namespace 
