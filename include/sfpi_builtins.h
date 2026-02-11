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

#define __builtin_rvtt_sfpxicmps(v, i, mod1) __builtin_rvtt_sfpxicmps(ckernel::instrn_buffer, v, i, 0, 0, mod1)
#define __builtin_rvtt_sfpshft_i(dst, imm12, mod1) __builtin_rvtt_sfpshft_i(ckernel::instrn_buffer, dst, imm12, 0, 0, mod1)
#define __builtin_rvtt_sfpload(mod0, mode, addr) __builtin_rvtt_sfpload(ckernel::instrn_buffer, mod0, mode, addr, 0, 0)
#define __builtin_rvtt_sfpxloadi(mod0, imm16) __builtin_rvtt_sfpxloadi(ckernel::instrn_buffer, mod0, imm16, 0, 0)
#define __builtin_rvtt_sfpstore(src, mod0, mode, addr) __builtin_rvtt_sfpstore(ckernel::instrn_buffer, src, mod0, mode, addr, 0, 0)
#define __builtin_rvtt_sfpxfcmps(v, f, mod1) __builtin_rvtt_sfpxfcmps(ckernel::instrn_buffer, v, f, 0, 0, mod1)
#define __builtin_rvtt_sfpsetexp_i(imm12, src) __builtin_rvtt_sfpsetexp_i(ckernel::instrn_buffer, imm12, 0, 0, src)
#define __builtin_rvtt_sfpsetman_i(imm12, src, mod) __builtin_rvtt_sfpsetman_i(ckernel::instrn_buffer, imm12, 0, 0, src, mod)
#define __builtin_rvtt_sfpdivp2(imm12, src, mod1) __builtin_rvtt_sfpdivp2(ckernel::instrn_buffer, imm12, 0, 0, src, mod1)
#define __builtin_rvtt_sfpxiadd_i(src, imm12, mod1) __builtin_rvtt_sfpxiadd_i(ckernel::instrn_buffer, src, imm12, 0, 0, mod1)
#define __builtin_rvtt_sfpsetsgn_i(imm12, src) __builtin_rvtt_sfpsetsgn_i(ckernel::instrn_buffer, imm12, 0, 0, src)
#define __builtin_rvtt_sfpstochrnd_i(mode, imm8, srcc, mod1) __builtin_rvtt_sfpstochrnd_i(ckernel::instrn_buffer, mode, imm8, 0, 0, srcc, mod1)

} // namespace sfpi
