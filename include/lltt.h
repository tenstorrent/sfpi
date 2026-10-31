/*
 * SPDX-FileCopyrightText: © 2025 Tenstorrent Inc.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

// Low level tt-insn interface.

#pragma once

#include "tensix_builtins.h"

#include <cstdint>

#if __riscv_xtttensixqsr
#define __builtin_rvtt_ttreplay(START, LENGTH, EXEC, RECORD, LAST, MUTEX) \
  _Pragma ("GCC warning \"macro is deprecated provide iptr operands explicitly\"") \
  __builtin_rvtt_ttreplay(lltt::__instrn_buffer, LENGTH, 0, 0, START, EXEC, RECORD, LAST, MUTEX)
#else
#define __builtin_rvtt_ttreplay(START, LENGTH, EXEC, RECORD) \
  _Pragma ("GCC warning \"macro is deprecated provide iptr operands explicitly\"") \
  __builtin_rvtt_ttreplay(lltt::__instrn_buffer, LENGTH, 0, 0, START, EXEC, RECORD)
#endif
extern volatile unsigned long __instrn_buffer[];

namespace lltt {
constexpr inline volatile unsigned long *[[gnu::rvtt_reg_ptr]] __instrn_buffer = ::__instrn_buffer;

enum ExecBool : bool {NoExec, Exec};

template<ExecBool E = NoExec>
[[gnu::always_inline]] inline void
record(unsigned start, unsigned length
#if __riscv_xtttensixqsr
       , bool last = false, bool mutex = false
#endif
       ) {
  (__builtin_rvtt_ttreplay) (lltt::__instrn_buffer, length, 0, 0, start, bool(E), true
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
  (__builtin_rvtt_ttreplay) (lltt::__instrn_buffer, length, 0, 0, start, false, false
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
  return (0x04 << 24) | (start << 14) | (length << 4)
#if __riscv_xtttensixqsr
      | (last << 3) | (mutex << 2)
#endif
      ;
}

} // namespace 
