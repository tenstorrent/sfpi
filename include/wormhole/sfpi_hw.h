/*
 * SPDX-FileCopyrightText: © 2023 Tenstorrent Inc.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

///////////////////////////////////////////////////////////////////////////////
// SFPI HW constants, compiler abstraction
//////////////////////////////////////////////////////////////////////////////
#pragma once

namespace sfpi {

using __rvtt_vec_t = ::__xtt_vector;

#define sfpi_inline __attribute__((always_inline)) inline

#define __builtin_rvtt_sfpxicmps(v, i, mod1) __builtin_rvtt_sfpxicmps(ckernel::instrn_buffer, v, i, 0, 0, mod1)
#define __builtin_rvtt_sfpxicmpv(v1, v2, mod1) __builtin_rvtt_sfpxicmpv(v1, v2, mod1)
#define __builtin_rvtt_sfpxvif() __builtin_rvtt_sfpxvif()
#define __builtin_rvtt_sfpxbool(t, a, b) __builtin_rvtt_sfpxbool(t, a, b)
#define __builtin_rvtt_sfpxcondb(s, i) __builtin_rvtt_sfpxcondb(s, i)
#define __builtin_rvtt_sfpxcondi(i) __builtin_rvtt_sfpxcondi(i)

#define __builtin_rvtt_sfpload(mod0, mode, addr) __builtin_rvtt_wh_sfpload(ckernel::instrn_buffer, mod0, mode, addr, 0, 0)

#define __builtin_rvtt_sfpxloadi(mod0, imm16) __builtin_rvtt_wh_sfpxloadi(ckernel::instrn_buffer, mod0, imm16, 0, 0)
#define __builtin_rvtt_sfpstore(src, mod0, mode, addr) __builtin_rvtt_wh_sfpstore(ckernel::instrn_buffer, src, mod0, mode, addr, 0, 0)

#define __builtin_rvtt_sfpxfcmps(v, f, mod1) __builtin_rvtt_wh_sfpxfcmps(ckernel::instrn_buffer, v, f, 0, 0, mod1)
#define __builtin_rvtt_sfpxfcmpv(v1, v2, mod1) __builtin_rvtt_wh_sfpxfcmpv(v1, v2, mod1)

#define __builtin_rvtt_sfpadd(va, vb, mod1) __builtin_rvtt_wh_sfpadd(va, vb, mod1)
#define __builtin_rvtt_sfpmul(va, vb, mod1) __builtin_rvtt_wh_sfpmul(va, vb, mod1)
#define __builtin_rvtt_sfpmad(va, vb, vc, mod1) __builtin_rvtt_wh_sfpmad(va, vb, vc, mod1)

#define __builtin_rvtt_sfpsetexp_i(imm12, src) __builtin_rvtt_wh_sfpsetexp_i(ckernel::instrn_buffer, imm12, 0, 0, src)
#define __builtin_rvtt_sfpsetexp_v(dst, src) __builtin_rvtt_wh_sfpsetexp_v(dst, src)

#define __builtin_rvtt_sfpsetman_i(imm12, src, mod) __builtin_rvtt_wh_sfpsetman_i(ckernel::instrn_buffer, imm12, 0, 0, src, mod)
#define __builtin_rvtt_sfpsetman_v(dst, src) __builtin_rvtt_wh_sfpsetman_v(dst, src)

#define __builtin_rvtt_sfpdivp2(imm12, src, mod1) __builtin_rvtt_wh_sfpdivp2(ckernel::instrn_buffer, imm12, 0, 0, src, mod1)

#define __builtin_rvtt_sfpshft_i(dst, imm12, mod1) __builtin_rvtt_sfpshft_i(ckernel::instrn_buffer, dst, imm12, 0, 0, mod1)
#define __builtin_rvtt_sfpshft_v(dst, src, mod1) __builtin_rvtt_sfpshft_v(dst, src, mod1)

#define __builtin_rvtt_sfpxiadd_i(src, imm12, mod1) __builtin_rvtt_wh_sfpxiadd_i(ckernel::instrn_buffer, src, imm12, 0, 0, mod1)
#define __builtin_rvtt_sfpxiadd_v(dst, src, mod1) __builtin_rvtt_wh_sfpxiadd_v(dst, src, mod1)

#define __builtin_rvtt_sfpsetsgn_i(imm12, src) __builtin_rvtt_wh_sfpsetsgn_i(ckernel::instrn_buffer, imm12, 0, 0, src)
#define __builtin_rvtt_sfpsetsgn_v(dst, src) __builtin_rvtt_wh_sfpsetsgn_v(dst, src)

#define __builtin_rvtt_sfpcast(src, mod1) __builtin_rvtt_wh_sfpcast(src, mod1)
#define __builtin_rvtt_sfpstochrnd_i(mode, imm8, srcc, mod1) __builtin_rvtt_wh_sfpstochrnd_i(ckernel::instrn_buffer, mode, imm8, 0, 0, srcc, mod1)
#define __builtin_rvtt_sfpstochrnd_v(mode, srcb, srcc, mod1) __builtin_rvtt_wh_sfpstochrnd_v(mode, srcb, srcc, mod1)

#define __builtin_rvtt_sfpconfig_v(l0, config_dest) __builtin_rvtt_wh_sfpconfig_v(l0, config_dest)

} // namespace sfpi
