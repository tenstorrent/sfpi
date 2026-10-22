/*
 * SPDX-FileCopyrightText: © 2025 Tenstorrent Inc.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

// Low level tt-insn interface.

#pragma once

#include "tensix_builtins.h"

#include <cstdint>

extern volatile unsigned long __instrn_buffer[];

namespace lltt {
constexpr inline volatile unsigned long *[[gnu::rvtt_reg_ptr]] __instrn_buffer = ::__instrn_buffer;

constexpr unsigned replay_start_shift_ = 10;

enum ExecBool : bool {NoExec, Exec};

template<ExecBool E = NoExec>
[[gnu::always_inline]] inline void
record(unsigned start, unsigned length
#if __riscv_xtttensixqsr
       , bool last = false, bool mutex = false
#endif
       ) {
  __builtin_rvtt_ttreplay (__instrn_buffer, (start << replay_start_shift_) | length, 0, 0, bool(E), true
#if __riscv_xtttensixqsr
                          , last, mutex
#endif
                          );
}

[[gnu::always_inline]] inline void
replay(unsigned start, unsigned length
#if __riscv_xtttensixqsr
       , bool last = false, bool mutex = false
#endif
       ) {
  __builtin_rvtt_ttreplay (__instrn_buffer, (start << replay_start_shift_) | length, 0, 0, false, false
#if __riscv_xtttensixqsr
                          , last, mutex
#endif
                          );
}

[[gnu::always_inline]] constexpr std::uint32_t
replay_insn(unsigned start, unsigned length
#if __riscv_xtttensixqsr
            , bool last = false, bool mutex = false
#endif
            ) {  
  // Perhaps another builtin?
  return (0x04 << 24) | (((start << replay_start_shift_) | length) << 4)
#if __riscv_xtttensixqsr
      | (last << 3) | (mutex << 2)
#endif
      ;
}

} // namespace 
