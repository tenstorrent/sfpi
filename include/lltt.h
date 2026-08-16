/*
 * SPDX-FileCopyrightText: © 2025 Tenstorrent Inc.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

// Low level tt-insn interface.

#pragma once

#include "tensix_builtins.h"

#include <cstdint>

#define __builtin_rvtt_ttreplay(START, LENGTH, EXEC, RECORD) \
  __builtin_rvtt_ttreplay(lltt::__instrn_buffer, LENGTH, 0, 0, START, EXEC, RECORD)

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

// Compiler-visible SETRWC boundary.  Keep the architectural fields typed so
// backend ownership analyses can model Dst/RWC effects without decoding an
// opaque instruction word.  Constants are template arguments because the
// instruction has immediate-only fields on every Tensix target.
template<unsigned Clear, unsigned Cr, unsigned D, unsigned B, unsigned A,
         unsigned Set>
[[gnu::always_inline]] inline void setrwc() {
  __builtin_rvtt_ttsetrwc(Clear, Cr, D, B, A, Set);
}

[[gnu::always_inline]] constexpr std::uint32_t
replay_insn(unsigned start, unsigned length) {
  // Perhaps another builtin?
  return (0x04 << 24) | (start << 14) | (length << 4);
}

} // namespace 
