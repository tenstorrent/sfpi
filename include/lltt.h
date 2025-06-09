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

[[gnu::always_inline]] constexpr std::uint32_t
replay_insn(unsigned start, unsigned length) {
  // Perhaps another builtin?
  return (0x04 << 24) | (start << 14) | (length << 4);
}

} // namespace 
