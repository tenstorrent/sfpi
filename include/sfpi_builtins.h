/*
 * SPDX-FileCopyrightText: © 2023-2026 Tenstorrent Inc.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

///////////////////////////////////////////////////////////////////////////////
// SFPI builtins
//////////////////////////////////////////////////////////////////////////////
#pragma once

namespace sfpi {

// Argument reordering here is temporary shim
#define __builtin_rvtt_sfpxicmps(src, imm, mod1) __builtin_rvtt_sfpxicmps(ckernel::instrn_buffer, src, imm, 0, 0, mod1)
#define __builtin_rvtt_sfpxfcmps(src, imm, mod1) __builtin_rvtt_sfpxfcmps(ckernel::instrn_buffer, src, imm, 0, 0, mod1)
#define __builtin_rvtt_sfpxiadd_i(src, imm, mod1) __builtin_rvtt_sfpxiadd_i(ckernel::instrn_buffer, src, imm, 0, 0, mod1)
#define __builtin_rvtt_sfpxloadi(imm, mod0) __builtin_rvtt_sfpxloadi(ckernel::instrn_buffer, -1, imm, 0, 0, mod0)
#define __builtin_rvtt_sfpshft_i(src, imm, mod1) __builtin_rvtt_sfpshft_i(ckernel::instrn_buffer, src, imm, 0, 0, mod1)
#define __builtin_rvtt_sfpload(addr, mod0, mode) __builtin_rvtt_sfpload(ckernel::instrn_buffer, addr, 0, 0, mod0, mode)
#define __builtin_rvtt_sfpstore(src, addr, mod0, mode) __builtin_rvtt_sfpstore(ckernel::instrn_buffer, src, addr, 0, 0, mod0, mode)
#define __builtin_rvtt_sfpsetexp_i(src, imm) __builtin_rvtt_sfpsetexp_i(ckernel::instrn_buffer, src, imm, 0, 0)
#define __builtin_rvtt_sfpsetman_i(src, imm) __builtin_rvtt_sfpsetman_i(ckernel::instrn_buffer, src, imm, 0, 0)
#define __builtin_rvtt_sfpsetsgn_i(src, imm) __builtin_rvtt_sfpsetsgn_i(ckernel::instrn_buffer, src, imm, 0, 0)
#define __builtin_rvtt_sfpdivp2(src, imm, mod1) __builtin_rvtt_sfpdivp2(ckernel::instrn_buffer, src, imm, 0, 0, mod1)
#define __builtin_rvtt_sfpstochrnd_i(src, imm, mode, mod1) __builtin_rvtt_sfpstochrnd_i(ckernel::instrn_buffer, src, imm, 0, 0, mode, mod1)

} // namespace sfpi
