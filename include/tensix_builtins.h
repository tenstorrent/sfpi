/*
 * SPDX-FileCopyrightText: © 2023-2026 Tenstorrent Inc.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

///////////////////////////////////////////////////////////////////////////////
// Tensix builtins
//////////////////////////////////////////////////////////////////////////////
#pragma once

#if __riscv_xtttensixwh + __riscv_xtttensixbh + __riscv_xtttensixqsr
#if !__has_builtin(__builtin_rvtt_synth_opcode)
#error "Compiler does not support TENSIX builtins"
#include "stop now, no good will come"
#endif

#else

// We're gonna be faking it
#if defined (ARCH_WORMHOLE)
#define __riscv_xtttensixwh 1
#elif defined (ARCH_BLACKHOLE)
#define __riscv_xtttensixbh 1
#elif defined (ARCH_QUASAR)
#define __riscv_xtttensixqsr 1
#else
#error "No fallback extension selected -- define ARCH_$ARCH"
#include "stop now, no good will come"
#endif

using __xtt_vector = unsigned __attribute__((vector_size (sizeof (unsigned) * 32)));
using __xtt_vector2 = unsigned __attribute__((vector_size (sizeof (unsigned) * 32 * 2)));
using __xtt_vector4 = unsigned __attribute__((vector_size (sizeof (unsigned) * 32 * 4)));

#endif

#include "tensix_builtins.def"

