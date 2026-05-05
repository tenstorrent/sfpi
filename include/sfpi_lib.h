/*
 * SPDX-FileCopyrightText: © 2023-2026 Tenstorrent Inc.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

///////////////////////////////////////////////////////////////////////////////
// sfpi_lib.h: SFPu Interface library free functions
///////////////////////////////////////////////////////////////////////////////

#pragma once

namespace sfpi {

//////////////////////////////////////////////////////////////////////////////
// Functional math library
//////////////////////////////////////////////////////////////////////////////
sfpi_inline vFloat lut(const vFloat v, const vUInt l0, const vUInt l1, const vUInt l2)
{
    return __builtin_rvtt_sfplut(l0.get(), l1.get(), l2.get(), v.get(), SFPLUT_MOD0_SGN_RETAIN);
}

sfpi_inline vFloat lut_sign(const vFloat v, const vUInt l0, const vUInt l1, const vUInt l2)
{
    return __builtin_rvtt_sfplut(l0.get(), l1.get(), l2.get(), v.get(), SFPLUT_MOD0_SGN_UPDATE);
}

sfpi_inline vFloat lut2(const vFloat v, const vUInt l0, const vUInt l1, const vUInt l2)
{
    return __builtin_rvtt_sfplutfp32_3r(l0.get(), l1.get(), l2.get(),
                                        v.get(), SFPLUTFP32_MOD0_FP16_3ENTRY_TABLE | SFPLUTFP32_MOD0_SGN_RETAIN);
}

sfpi_inline vFloat lut2_sign(const vFloat v, const vUInt l0, const vUInt l1, const vUInt l2)
{
    return __builtin_rvtt_sfplutfp32_3r(l0.get(), l1.get(), l2.get(),
                                        v.get(), SFPLUTFP32_MOD0_FP16_3ENTRY_TABLE | SFPLUTFP32_MOD0_SGN_UPDATE);
}

sfpi_inline vFloat lut2(const vFloat v,
                        const vFloat a0, const vFloat a1, const vFloat a2,
                        const vFloat b0, const vFloat b1, const vFloat b2)
{
    return __builtin_rvtt_sfplutfp32_6r(a0.get(), a1.get(), a2.get(),
                                        b0.get(), b1.get(), b2.get(),
                                        v.get(), SFPLUTFP32_MOD0_FP32_3ENTRY_TABLE | SFPLUTFP32_MOD0_SGN_RETAIN);
}

sfpi_inline vFloat lut2_sign(const vFloat v,
                             const vFloat a0, const vFloat a1, const vFloat a2,
                             const vFloat b0, const vFloat b1, const vFloat b2)
{
    return __builtin_rvtt_sfplutfp32_6r(a0.get(), a1.get(), a2.get(),
                                        b0.get(), b1.get(), b2.get(),
                                        v.get(), SFPLUTFP32_MOD0_FP32_3ENTRY_TABLE | SFPLUTFP32_MOD0_SGN_UPDATE);
}

sfpi_inline vFloat lut2(const vFloat v,
                        const vUInt a01, const vUInt a23, const vUInt a45,
                        const vUInt b01, const vUInt b23, const vUInt b45, const int mode = 1)
{
    unsigned int mod = (mode == 1) ? SFPLUTFP32_MOD0_FP16_6ENTRY_TABLE1 : SFPLUTFP32_MOD0_FP16_6ENTRY_TABLE2;
    return __builtin_rvtt_sfplutfp32_6r(a01.get(), a23.get(), a45.get(),
                                        b01.get(), b23.get(), b45.get(),
                                        v.get(), mod | SFPLUTFP32_MOD0_SGN_RETAIN);
}

sfpi_inline vFloat lut2_sign(const vFloat v,
                             const vUInt a01, const vUInt a23, const vUInt a45,
                             const vUInt b01, const vUInt b23, const vUInt b45, const int mode = 1)
{
    unsigned int mod = (mode == 1) ? SFPLUTFP32_MOD0_FP16_6ENTRY_TABLE1 : SFPLUTFP32_MOD0_FP16_6ENTRY_TABLE2;
    return __builtin_rvtt_sfplutfp32_6r(a01.get(), a23.get(), a45.get(),
                                        b01.get(), b23.get(), b45.get(),
                                        v.get(), mod | SFPLUTFP32_MOD0_SGN_UPDATE);
}

enum class ExponentMode {
  Debias,
  NoDebias,
};

sfpi_inline vInt exexp (const vFloat v, ExponentMode mode = ExponentMode::Debias) {
  return __builtin_rvtt_sfpexexp (v.get (),
                                  mode == ExponentMode::Debias ? SFPEXEXP_MOD1_DEBIAS :
                                  mode == ExponentMode::NoDebias ? SFPEXEXP_MOD1_NODEBIAS :
                                  ~0 /* bad value, compile error */);
}

// Deprecate
sfpi_inline vInt exexp_nodebias(const vFloat v)
{
    return __builtin_rvtt_sfpexexp(v.get(), SFPEXEXP_MOD1_NODEBIAS);
}

enum class MantissaMode {
  FractionOnly,
  WithUnitBit,

  ImplicitOne = WithUnitBit,
};

sfpi_inline vInt exman(const vFloat v, MantissaMode mode = MantissaMode::FractionOnly) {
    return __builtin_rvtt_sfpexman (v.get (),
                                    mode == MantissaMode::FractionOnly ? SFPEXMAN_MOD1_PAD9 :
                                    mode == MantissaMode::WithUnitBit ? SFPEXMAN_MOD1_PAD8 :
                                    ~0 /* bad value, compile error */);
}

// Deprecate -> Implicit
sfpi_inline vInt exman8(const vFloat v)
{
    return __builtin_rvtt_sfpexman(v.get(), SFPEXMAN_MOD1_PAD8);
}

// Deprecate -> NoImplicit
sfpi_inline vInt exman9(const vFloat v)
{
    return __builtin_rvtt_sfpexman(v.get(), SFPEXMAN_MOD1_PAD9);
}

sfpi_inline vFloat setexp (const vFloat v, int exp) {
  return __builtin_rvtt_sfpsetexp_i (v.get(), exp, 0);
}

sfpi_inline vFloat setexp (vFloat v, vInt exp) {
  return __builtin_rvtt_sfpsetexp_v (v.get (), exp.get (), 0);
}

sfpi_inline vFloat copyexp (vFloat v, vFloat exp) {
  return __builtin_rvtt_sfpsetexp_v (v.get (), exp.get (), SFPSETEXP_MOD1_CPY);
}

sfpi_inline vFloat setman (vFloat v, int man) {
  return __builtin_rvtt_sfpsetman_i (v.get(), man, 0);
}

sfpi_inline vFloat setman (vFloat v, impl_::vVal man) {
  return __builtin_rvtt_sfpsetman_v (v.get (), man.get (), 0);
}

sfpi_inline vFloat addexp (vFloat in, int exp) {
  return __builtin_rvtt_sfpdivp2 (in.get (), exp, SFPSDIVP2_MOD1_ADD);
}

#if __riscv_xtttensixbh || __riscv_xtttensixqsr
enum class FractionalHalf {
  Low,
  High,
};

sfpi_inline vInt fractional_mul (vInt a, vInt b, FractionalHalf half = FractionalHalf::Low) {
  return __builtin_rvtt_sfpmul24 (a.get (), b.get (),
                                  half == FractionalHalf::Low ? SFPMUL24_MOD1_LOWER
                                  : half == FractionalHalf::High ? SFPMUL24_MOD1_UPPER
                                  : ~0);
}
#endif

// FIXME: These hould be restricted to vFloat and vSMag
template <typename vType, typename std::enable_if_t<std::is_base_of<impl_::vVal, vType>::value>* = nullptr>
sfpi_inline vType setsgn(const vType v, const int32_t sgn)
{
    return __builtin_rvtt_sfpsetsgn_i(v.get(), sgn, 0);
}

template <typename vTypeA, typename vTypeB,
          typename std::enable_if_t<std::is_base_of<impl_::vVal, vTypeA>::value>* = nullptr,
          typename std::enable_if_t<std::is_base_of<impl_::vVal, vTypeB>::value>* = nullptr>
sfpi_inline vTypeA copysgn (const vTypeA v, const vTypeB sgn) {
  return __builtin_rvtt_sfpsetsgn_v (v.get (), sgn.get (), 0);
}

template <typename vType,
          typename std::enable_if_t<std::is_base_of<impl_::vVal, vType>::value>* = nullptr>
sfpi_inline vType copysgn(const vType v, const vInt sgn) {
  return __builtin_rvtt_sfpsetsgn_v (v.get (), sgn.get (), 0);
}

// Old names for compatibility
template <typename vTypeA, typename vTypeB,
          typename std::enable_if_t<std::is_base_of<impl_::vVal, vTypeA>::value>* = nullptr,
          typename std::enable_if_t<std::is_base_of<impl_::vVal, vTypeB>::value>* = nullptr>
sfpi_inline vTypeA setsgn(const vTypeA v, const vTypeB sgn) {
  return copysgn (v, sgn);
}

template <typename vType,
          typename std::enable_if_t<std::is_base_of<impl_::vVal, vType>::value>* = nullptr>
sfpi_inline vType setsgn (const vType v, const vInt sgn) {
  return copysgn (v, sgn);
}

template <typename vType, typename std::enable_if_t<std::is_base_of<impl_::vVal, vType>::value>* = nullptr>
sfpi_inline vInt lz(const vType v)
{
    return vInt(__builtin_rvtt_sfplz(v.get(), SFPLZ_MOD1_CC_NONE));
}

template <typename vType, typename std::enable_if_t<std::is_base_of<impl_::vVal, vType>::value>* = nullptr>
sfpi_inline vInt lz_nosgn(const vType v)
{
    return vInt(__builtin_rvtt_sfplz(v.get(), SFPLZ_MOD1_NOSGN_CC_NONE));
}

sfpi_inline vFloat abs(const vFloat v)
{
    return __builtin_rvtt_sfpabs(v.get(), SFPABS_MOD1_FLOAT);
}

sfpi_inline vInt abs(const vInt v)
{
    return __builtin_rvtt_sfpabs(v.get(), SFPABS_MOD1_INT);
}

sfpi_inline vUInt shft(const vUInt v, const vInt amt)
{
    return __builtin_rvtt_sfpshft_v(v.get(), amt.get(), SFPSHFT_MOD1_LOGICAL);
}

sfpi_inline vUInt shft(const vUInt v, int amt)
{
    return __builtin_rvtt_sfpshft_i(v.get(), amt, SFPSHFT_MOD1_LOGICAL);
}

#if __riscv_xtttensixbh || __riscv_xtttensixqsr
sfpi_inline vInt shft(const vInt v, const vInt amt)
{
    return __builtin_rvtt_sfpshft_v(v.get(), amt.get(), SFPSHFT_MOD1_ARITHMETIC);
}

sfpi_inline vInt shft(const vInt v, int amt)
{
    return __builtin_rvtt_sfpshft_i(v.get(), amt, SFPSHFT_MOD1_ARITHMETIC);
}
#endif

template <typename vType, typename std::enable_if_t<std::is_base_of<impl_::vVal, vType>::value>* = nullptr>
sfpi_inline vType reinterpret(const impl_::vVal v)
{
    return vType(v.get());
}

namespace RoundMode {
// Use a namespace here, so we can at a future time turn it into a scoped enum
enum RoundMode {
  NearestEven,
  Stochastic,
#if __riscv_xtttensixbh || __riscv_xtttensixqsr
  Zero,
#endif
  Even = NearestEven,
  Nearest = NearestEven,
};
}

namespace impl_ {
sfpi_inline constexpr unsigned rounding_to_stochrnd_rnd (int mode) {
  return mode == RoundMode::NearestEven ? SFPSTOCHRND_RND_EVEN :
      mode == RoundMode::Stochastic ? SFPSTOCHRND_RND_STOCH :
#if __riscv_xtttensixbh || __riscv_xtttensixqsr
      mode == RoundMode::Zero ? SFPSTOCHRND_RND_ZERO :
#endif
      ~0; // Bad value, compilation error
}
sfpi_inline constexpr unsigned rounding_to_cast_rnd (int mode) {
  return mode == RoundMode::NearestEven ? SFPCAST_MOD1_INT32_TO_FP32_RNE :
      mode == RoundMode::Stochastic ? SFPCAST_MOD1_INT32_TO_FP32_RNS :
      ~0; // Bad value, compilation error
}
using RoundMode = int;
}

sfpi_inline vFloat int32_to_float (vInt in, impl_::RoundMode rounding = RoundMode::Stochastic) {
  return __builtin_rvtt_sfpcast (in.get (), impl_::rounding_to_cast_rnd (rounding));
}

// FIXME. we should add vFloat16[ab] types to indicate these are in that form.
// And perhaps v{,U}Int16 too?
sfpi_inline vFloat16a float_to_fp16a (vFloat in, impl_::RoundMode rounding = RoundMode::Stochastic) {
  return vFloat16a (__builtin_rvtt_sfpstochrnd_i
                    (in.get(), 0,
                     SFPSTOCHRND_MOD1_FP32_TO_FP16A, impl_::rounding_to_stochrnd_rnd (rounding)));
}

sfpi_inline vFloat16b float_to_fp16b (vFloat in, impl_::RoundMode rounding = RoundMode::Stochastic) {
  return vFloat16b (__builtin_rvtt_sfpstochrnd_i
                    (in.get(), 0,
                     SFPSTOCHRND_MOD1_FP32_TO_FP16B, impl_::rounding_to_stochrnd_rnd (rounding)));
}

sfpi_inline vUInt16 float_to_uint16 (vFloat in, impl_::RoundMode rounding = RoundMode::Stochastic) 
{
  return vUInt16 (__builtin_rvtt_sfpstochrnd_i
                  (in.get(), 0,
                   SFPSTOCHRND_MOD1_FP32_TO_UINT16, impl_::rounding_to_stochrnd_rnd (rounding)));
}

sfpi_inline vInt16 float_to_int16 (vFloat in, impl_::RoundMode rounding = RoundMode::Stochastic) {
  return vInt16 (__builtin_rvtt_sfpstochrnd_i
                 (in.get(), 0,
                  SFPSTOCHRND_MOD1_FP32_TO_INT16, impl_::rounding_to_stochrnd_rnd (rounding)));
}

sfpi_inline vUInt16 float_to_uint8 (vFloat in, impl_::RoundMode rounding = RoundMode::Stochastic) {
  return vUInt16 (__builtin_rvtt_sfpstochrnd_i
                  (in.get(), 0,
                   SFPSTOCHRND_MOD1_FP32_TO_UINT8, impl_::rounding_to_stochrnd_rnd (rounding)));
}

sfpi_inline vInt16 float_to_int8 (vFloat in, impl_::RoundMode rounding = RoundMode::Stochastic) {
  return vInt16 (__builtin_rvtt_sfpstochrnd_i
                 (in.get(), 0,
                  SFPSTOCHRND_MOD1_FP32_TO_INT8, impl_::rounding_to_stochrnd_rnd (rounding)));
}

sfpi_inline vUInt16 int32_to_uint8 (vInt in, vUInt descale, impl_::RoundMode rounding = RoundMode::Stochastic) {
  return vUInt16 (__builtin_rvtt_sfpstochrnd_v
                  (in.get(), descale.get(),
                   SFPSTOCHRND_MOD1_INT32_TO_UINT8, impl_::rounding_to_stochrnd_rnd (rounding)));
}

sfpi_inline vUInt16 int32_to_uint8 (vInt in, unsigned descale, impl_::RoundMode rounding = RoundMode::Stochastic) {
  return vUInt16 (__builtin_rvtt_sfpstochrnd_i
                  (in.get(), descale,
                   SFPSTOCHRND_MOD1_INT32_TO_UINT8, impl_::rounding_to_stochrnd_rnd (rounding)));
}

sfpi_inline vInt16 int32_to_int8 (vInt in, vUInt descale, impl_::RoundMode rounding = RoundMode::Stochastic) {
  return vInt16 (__builtin_rvtt_sfpstochrnd_v
                 (in.get(), descale.get(),
                  SFPSTOCHRND_MOD1_INT32_TO_INT8, impl_::rounding_to_stochrnd_rnd (rounding)));
}

sfpi_inline vInt16 int32_to_int8 (vInt in, unsigned descale, impl_::RoundMode rounding = RoundMode::Stochastic) {
  return vInt16 (__builtin_rvtt_sfpstochrnd_i
                 (in.get(), descale,
                  SFPSTOCHRND_MOD1_INT32_TO_INT8, impl_::rounding_to_stochrnd_rnd (rounding)));
}

sfpi_inline void subvec_transp (vFloat &a, vFloat &b, vFloat &c, vFloat &d) {
  auto r = __builtin_rvtt_sfptransp (a.get (), b.get (), c.get (), d.get ());
  a = __builtin_rvtt_sfpselect4 (r, 0);
  b = __builtin_rvtt_sfpselect4 (r, 1);
  c = __builtin_rvtt_sfpselect4 (r, 2);
  d = __builtin_rvtt_sfpselect4 (r, 3);
}
sfpi_inline void subvec_transp (vInt &a, vInt &b, vInt &c, vInt &d) {
  auto r = __builtin_rvtt_sfptransp (a.get (), b.get (), c.get (), d.get ());
  a = __builtin_rvtt_sfpselect4 (r, 0);
  b = __builtin_rvtt_sfpselect4 (r, 1);
  c = __builtin_rvtt_sfpselect4 (r, 2);
  d = __builtin_rvtt_sfpselect4 (r, 3);
}
sfpi_inline void subvec_transp (vUInt &a, vUInt &b, vUInt &c, vUInt &d) {
  auto r = __builtin_rvtt_sfptransp (a.get (), b.get (), c.get (), d.get ());
  a = __builtin_rvtt_sfpselect4 (r, 0);
  b = __builtin_rvtt_sfpselect4 (r, 1);
  c = __builtin_rvtt_sfpselect4 (r, 2);
  d = __builtin_rvtt_sfpselect4 (r, 3);
}

sfpi_inline impl_::sfpu_t subvec_shflror1(const impl_::vVal& src)
{
    return __builtin_rvtt_sfpshft2_subvec_shfl1(src.get(), SFPSHFT2_MOD1_SUBVEC_SHFLROR1);
}

sfpi_inline impl_::sfpu_t subvec_shflshr1(const impl_::vVal& src)
{
    return __builtin_rvtt_sfpshft2_subvec_shfl1(src.get(), SFPSHFT2_MOD1_SUBVEC_SHFLSHR1);
}

sfpi_inline void vec_swap (vFloat & a, vFloat &b) {
  auto r = __builtin_rvtt_sfpswap (a.get(), b.get (), SFPSWAP_MOD1_SWAP);
  a = __builtin_rvtt_sfpselect2 (r, 0);
  b = __builtin_rvtt_sfpselect2 (r, 1);
}
sfpi_inline void vec_swap (vInt &a, vInt &b) {
  auto r = __builtin_rvtt_sfpswap (a.get(), b.get (), SFPSWAP_MOD1_SWAP);
  a = __builtin_rvtt_sfpselect2 (r, 0);
  b = __builtin_rvtt_sfpselect2 (r, 1);
}
sfpi_inline void vec_swap (vUInt &a, vUInt &b) {
  auto r = __builtin_rvtt_sfpswap (a.get(), b.get (), SFPSWAP_MOD1_SWAP);
  a = __builtin_rvtt_sfpselect2 (r, 0);
  b = __builtin_rvtt_sfpselect2 (r, 1);
}

sfpi_inline void vec_min_max (vFloat &a, vFloat &b) {
  auto r = __builtin_rvtt_sfpswap (a.get (), b.get (), SFPSWAP_MOD1_VEC_MIN_MAX);
  a = __builtin_rvtt_sfpselect2 (r, 0);
  b = __builtin_rvtt_sfpselect2 (r, 1);
}
// FIXME: Use vSMag
sfpi_inline void vec_min_max (vInt &a, vInt &b) {
  auto r = __builtin_rvtt_sfpswap (a.get (), b.get (), SFPSWAP_MOD1_VEC_MIN_MAX);
  a = __builtin_rvtt_sfpselect2 (r, 0);
  b = __builtin_rvtt_sfpselect2 (r, 1);
}
sfpi_inline void vec_max_min (vFloat &a, vFloat &b) {
  auto r = __builtin_rvtt_sfpswap (a.get (), b.get (), SFPSWAP_MOD1_VEC_MAX_MIN);
  a = __builtin_rvtt_sfpselect2 (r, 0);
  b = __builtin_rvtt_sfpselect2 (r, 1);
}

#if __riscv_xtttensixbh || __riscv_xtttensixqsr
sfpi_inline vInt rand() {
  return vInt(__builtin_rvtt_sfpreadconfig(SFPCONFIG_SRC_RAND));
}

template<bool uncond = true>
sfpi_inline vFloat approx_recip(const vFloat &src)
{
  return vFloat(__builtin_rvtt_sfparecip(src.get(), uncond ? SFPARECIP_MOD1_RECIP : SFPARECIP_MOD1_COND_RECIP));
}

sfpi_inline vFloat approx_exp(const vFloat &src)
{
  return vFloat(__builtin_rvtt_sfparecip(src.get(), SFPARECIP_MOD1_EXP));
}
#endif

} // namespace sfpi
