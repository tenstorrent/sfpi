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

template <typename Type, typename std::enable_if_t<std::is_base_of<impl_::vVal, Type>::value>* = nullptr>
sfpi_inline Type reinterpret (impl_::vVal v) {
    return Type (v.get ());
}

template <typename Type, typename std::enable_if_t<std::is_base_of<impl_::vVal, Type>::value>* = nullptr>
sfpi_inline Type as (impl_::vVal v) {
    return Type (v.get ());
}

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

__SFPI_DEPRECATED("Use sfpi::exexp (X, sfpi::ExponentMode::NoDebias)")
sfpi_inline vInt exexp_nodebias(const vFloat v) {
  return __builtin_rvtt_sfpexexp(v.get(), SFPEXEXP_MOD1_NODEBIAS);
}

enum class MantissaMode {
  FractionOnly,
  WithUnitBit,

  ImplicitOne = WithUnitBit,
};

sfpi_inline vMag exman(const vFloat v, MantissaMode mode = MantissaMode::FractionOnly) {
  return vMag (__builtin_rvtt_sfpexman (v.get (),
                                        mode == MantissaMode::FractionOnly ? SFPEXMAN_MOD1_PAD9 :
                                        mode == MantissaMode::WithUnitBit ? SFPEXMAN_MOD1_PAD8 :
                                        ~0 /* bad value, compile error */));
}

template <typename Type,
          typename std::enable_if_t<std::disjunction<std::is_same<vFloat, Type>,
                                                     std::is_base_of<vInt, Type>,
                                                     std::is_base_of<vSMag, Type>>::value>* = nullptr>
sfpi_inline vUInt exsgn (Type v) {
  return as<vUInt> (v) >> 31;
}

__SFPI_DEPRECATED("Use sfpi::exman (X, sfpi::MantissaMode::WithUnitBit)")
sfpi_inline vInt exman8(const vFloat v) {
  return __builtin_rvtt_sfpexman(v.get(), SFPEXMAN_MOD1_PAD8);
}

__SFPI_DEPRECATED("Use sfpi::exman (X[, sfpi::MantissaMode::FractionOnly])")
sfpi_inline vInt exman9(const vFloat v) {
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

sfpi_inline vFloat setman (vFloat v, unsigned man) {
  return __builtin_rvtt_sfpsetman_i (v.get(), man, 0);
}

template <typename Type,
          typename std::enable_if_t<std::disjunction<std::is_base_of<vUInt, Type>,
                                                     std::is_base_of<vSMag, Type>>::value>* = nullptr>
sfpi_inline vFloat setman (vFloat v, Type man) {
  return __builtin_rvtt_sfpsetman_v (v.get (), man.get (), 0);
}

__SFPI_DEPRECATED("Use sfpi:copyman (X, Y)")
sfpi_inline vFloat setman (vFloat v, vFloat man)
{
  return __builtin_rvtt_sfpsetman_v (v.get (), man.get (), 0);
}

sfpi_inline vFloat copyman (vFloat v, vFloat man) {
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

// accept float, unsigned or sign-mag
template <typename TypeA, typename TypeB,
          typename std::enable_if_t<std::disjunction<std::is_base_of<vFloat, TypeA>,
                                                     std::is_base_of<vUInt, TypeA>,
                                                     std::is_base_of<vSMag, TypeA>>::value>* = nullptr,
          typename std::enable_if_t<std::disjunction<std::is_base_of<vFloat, TypeB>,
                                                     std::is_base_of<vUInt, TypeB>,
                                                     std::is_base_of<vSMag, TypeB>>::value>* = nullptr>
sfpi_inline vMag fractional_mul (TypeA a, TypeB b, FractionalHalf half = FractionalHalf::Low) {
  return vMag (__builtin_rvtt_sfpmul24 (a.get (), b.get (),
                                        half == FractionalHalf::Low ? SFPMUL24_MOD1_LOWER
                                        : half == FractionalHalf::High ? SFPMUL24_MOD1_UPPER
                                        : ~0));
}

__SFPI_DEPRECATED("Use sfpi::exexp (X, sfpi::ExponentMode::NoDebias)")
sfpi_inline vInt fractional_mul (vInt a, vInt b, FractionalHalf half = FractionalHalf::Low) {
  return fractional_mul (as<vUInt> (a), as<vUInt> (b), half);
}
#endif

// accept float, sign-mag
template <typename Type,
          typename std::enable_if_t<std::disjunction<std::is_base_of<vFloat, Type>,
                                                     std::is_base_of<vSMag, Type>>::value>* = nullptr>
sfpi_inline Type setsgn (Type v, int sgn) {
  return Type (__builtin_rvtt_sfpsetsgn_i (v.get (), sgn, 0));
}

sfpi_inline vSMag setsgn (vUInt v, int sgn) {
  return vSMag (__builtin_rvtt_sfpsetsgn_i (v.get (), sgn, 0));
}

__SFPI_DEPRECATED("Use sfpi::setsgn on vInt just sets sign bit, do this differently")
sfpi_inline vInt setsgn (vInt v, int sgn) {
  return vInt (__builtin_rvtt_sfpsetsgn_i (v.get (), sgn, 0));
}

template <typename TypeA, typename TypeB,
          typename std::enable_if_t<std::disjunction<std::is_base_of<vFloat, TypeA>,
                                                     std::is_base_of<vSMag, TypeA>>::value>* = nullptr,
          typename std::enable_if_t<std::disjunction<std::is_base_of<vFloat, TypeB>,
                                                     std::is_base_of<vInt, TypeB>,
                                                     std::is_base_of<vSMag, TypeB>>::value>* = nullptr>
sfpi_inline TypeA copysgn (const TypeA v, TypeB sgn) {
  return TypeA (__builtin_rvtt_sfpsetsgn_v (v.get (), sgn.get (), 0));
}

template <typename TypeB,
          typename std::enable_if_t<std::is_base_of<vInt, TypeB>::value>* = nullptr>
sfpi_inline vSMag copysgn (vUInt v, TypeB sgn) {
  return vSMag (__builtin_rvtt_sfpsetsgn_v (v.get (), sgn.get (), 0));
}

template <typename TypeA, typename TypeB,
          typename std::enable_if_t<std::disjunction<std::is_base_of<vFloat, TypeA>,
                                                     std::is_base_of<vSMag, TypeA>>::value>* = nullptr,
          typename std::enable_if_t<std::disjunction<std::is_base_of<vInt, TypeB>,
                                                     std::is_base_of<vUInt, TypeB>,
                                                     std::is_base_of<vSMag, TypeB>>::value>* = nullptr>
// FIXME:We'll rename this to plain setsgn once the old setsgn has gone though
// deprecation and removal
sfpi_inline TypeA setsgn2 (const TypeA v, TypeB sgn) {
  return copysgn (v, as<vSMag>(as<vUInt>(sgn) << 31));
}

template <typename TypeB,
          typename std::enable_if_t<std::disjunction<std::is_base_of<vFloat, TypeB>,
                                                     std::is_base_of<vInt, TypeB>,
                                                     std::is_base_of<vSMag, TypeB>>::value>* = nullptr>
__SFPI_DEPRECATED("Do not copy sign from int")
sfpi_inline vSMag copysgn (vInt v, TypeB sgn) {
  return vSMag (__builtin_rvtt_sfpsetsgn_v (v.get (), sgn.get (), 0));
}

template <typename vTypeA, typename vTypeB,
          typename std::enable_if_t<std::is_base_of<impl_::vVal, vTypeA>::value>* = nullptr,
          typename std::enable_if_t<std::is_base_of<impl_::vVal, vTypeB>::value>* = nullptr>
__SFPI_DEPRECATED("Use sfpi:copysgn (X, Y)")
sfpi_inline vTypeA setsgn(vTypeA v, vTypeB sgn) {
  return copysgn (v, sgn);
}

template <typename vType,
          typename std::enable_if_t<std::is_base_of<impl_::vVal, vType>::value>* = nullptr>
__SFPI_DEPRECATED("Use sfpi:copysgn (X, Y)")
sfpi_inline vType setsgn (vType v, vInt sgn) {
  return copysgn (v, sgn);
}

enum class LZMode {
  All,
  IgnoreSign,
};

template <typename Type,
          typename std::enable_if_t<std::disjunction<
                                      std::is_base_of<vInt, Type>,
                                      std::is_base_of<vUInt, Type>,
                                      std::is_base_of<vSMag, Type>>::value>* = nullptr>
sfpi_inline vMag lz (Type v, LZMode mode = LZMode::All) {
  return vMag (__builtin_rvtt_sfplz (v.get (),
                                     (mode == LZMode::All ? 0 :
                                      mode == LZMode::IgnoreSign ? SFPLZ_MOD1_NOSGN_MASK :
                                      ~0) | SFPLZ_MOD1_CC_NONE));
}

template <typename vType, typename std::enable_if_t<std::is_base_of<impl_::vVal, vType>::value>* = nullptr>
__SFPI_DEPRECATED("Use sfpi::lz (X, sfpi::LXMode::IgnoreSign)")
sfpi_inline vInt lz_nosgn (const vType v) {
  return vInt(__builtin_rvtt_sfplz( v.get (), SFPLZ_MOD1_NOSGN_CC_NONE));
}

sfpi_inline vFloat abs (vFloat v) {
  return __builtin_rvtt_sfpabs (v.get (), SFPABS_MOD1_FLOAT);
}

// Even though mostneg returns unchanged bit pattern, this returns vMag, not vUInt
sfpi_inline vMag abs (vInt v) {
  return vMag (__builtin_rvtt_sfpabs (v.get (), SFPABS_MOD1_INT));
}

sfpi_inline vMag abs (vSMag v) {
  return vMag (setsgn (v, 0).get ());
}

enum class ShiftMode {
  Logical,
#if !__riscv_xtttensixwh
  Arithmetic,
#endif
};

namespace impl_ {
sfpi_inline constexpr unsigned shft_mode (ShiftMode mode) {
  return mode == ShiftMode::Logical ? SFPSHFT_MOD1_LOGICAL :
#if !__riscv_xtttensixwh
      mode == ShiftMode::Arithmetic ? SFPSHFT_MOD1_ARITHMETIC :
#endif
      ~0; // Bad value, compilation error
}
};

sfpi_inline vUInt shft (vUInt v, vInt amt, ShiftMode mode = ShiftMode::Logical) {
  return __builtin_rvtt_sfpshft_v (v.get (), amt.get (), impl_::shft_mode (mode));
}

sfpi_inline vUInt shft (vUInt v, int amt, ShiftMode mode = ShiftMode::Logical) {
  return __builtin_rvtt_sfpshft_i (v.get (), amt, impl_::shft_mode (mode));
}

// WH has no arithmetic shift, force the user to specify logical
sfpi_inline vInt shft(vInt v, vInt amt, ShiftMode mode =
#if !__riscv_xtttensixwh
                      ShiftMode::Arithmetic
#else
                      ShiftMode(~0)
#endif
                      ) {
  return __builtin_rvtt_sfpshft_v (v.get (), amt.get (), impl_::shft_mode (mode));
}

sfpi_inline vInt shft(vInt v, int amt, ShiftMode mode =
#if !__riscv_xtttensixwh
                      ShiftMode::Arithmetic
#else
                      ShiftMode(~0)
#endif
                      ) {
  return __builtin_rvtt_sfpshft_i (v.get (), amt, impl_::shft_mode (mode));
}

// Unfortunately one cannot deprecate individual enumerations, so use a
// class and explicit values for the moment
#if 0
enum
#endif
class RoundMode {
#if 1
 public: enum Values {
#endif
#if __riscv_xtttensixwh || __riscv_xtttensixbh
   NearestAway,
   Nearest = NearestAway,
#else
   NearestEven,
   Nearest = NearestEven,
#endif
   NearestStochastic,
#if !__riscv_xtttensixwh
   Zero,
#endif
#if 1
 };
#if __riscv_xtttensixwh || __riscv_xtttensixbh
 __SFPI_DEPRECATED("Use RoundMode::Nearest or RoundMode::NearestAway")
 static constexpr Values NearestEven = NearestAway;
#endif
 __SFPI_DEPRECATED("Use RoundMode::NearestStochastic")
 static constexpr Values Stochastic = NearestStochastic;

 private: Values v;

 public: constexpr RoundMode (Values v) : v (v) {}
 public: constexpr operator Values () const { return v; }
#endif
};

namespace impl_ {
sfpi_inline constexpr unsigned stochrnd_rnd (RoundMode mode) {
  return mode == RoundMode::Nearest ? SFPSTOCHRND_RND_EVEN :
      mode == RoundMode::NearestStochastic ? SFPSTOCHRND_RND_STOCH :
#if __riscv_xtttensixbh || __riscv_xtttensixqsr
      mode == RoundMode::Zero ? SFPSTOCHRND_RND_ZERO :
#endif
      ~0; // Bad value, compilation error
}

sfpi_inline constexpr unsigned cast_rnd (RoundMode mode) {
  return mode == RoundMode::Nearest ? SFPCAST_MOD1_SM32_TO_FP32_RNE :
      mode == RoundMode::NearestStochastic ? SFPCAST_MOD1_SM32_TO_FP32_RNS :
      ~0; // Bad value, compilation error
}

template <typename ToType, typename FromType>
sfpi_inline constexpr unsigned stochrnd_mod () {
  if constexpr (std::is_same_v<vFloat, FromType>) {
    if constexpr (std::is_same_v<vFloat16a, ToType>)
      return SFPSTOCHRND_MOD1_FP32_TO_FP16A;
    else if constexpr (std::is_same_v<vFloat16b, ToType>)
      return SFPSTOCHRND_MOD1_FP32_TO_FP16B;
    else if constexpr (std::is_same_v<vSMag16, ToType>)
      return SFPSTOCHRND_MOD1_FP32_TO_INT16;
    else if constexpr (std::is_same_v<vUInt16, ToType>)
      return SFPSTOCHRND_MOD1_FP32_TO_UINT16; 
    else
      static_assert (false, "Cannot convert vFloat to target type");
  }
  else
    static_assert (false, "Cannot convert to target type");
}
}

// conversions

// float/float16a/float16b -> float/float16a/float16b/smag32/smag16/uint16
template <typename ToType, typename FromType,
          typename std::enable_if_t<std::is_base_of<impl_::vVal, ToType>::value>* = nullptr,
          typename std::enable_if_t<std::is_base_of<vFloat, FromType>::value>* = nullptr>
sfpi_inline ToType convert (FromType val, RoundMode round [[gnu::unused]] = RoundMode::NearestStochastic)
{
  if constexpr (std::is_same_v<FromType, ToType>)
    return val;

#if __riscv_xtttensixqsr
  else if constexpr (std::is_same_v<vSMag, ToType>)
    {
      // To smag
      unsigned mod1 =
          round == RoundMode::NearestEven ? SFPCAST_MOD1_FP32_TO_SM32_RNE :
          round == RoundMode::NearestStochastic ? SFPCAST_MOD1_FP32_TO_SM32_RNS :
          ~0u;

      return ToType (__builtin_rvtt_sfpcast (val.get (), mod1));
    }
#endif

  else
    {
      // to 16a,16b,int16,uint16
      unsigned mod1 = ~0;
      if constexpr (std::is_same_v<vFloat16a, ToType>)
        mod1 = SFPSTOCHRND_MOD1_FP32_TO_FP16A;
      else if constexpr (std::is_same_v<vFloat16b, ToType>)
        mod1 = SFPSTOCHRND_MOD1_FP32_TO_FP16B;
      else if constexpr (std::is_same_v<vSMag16, ToType>)
        mod1 = SFPSTOCHRND_MOD1_FP32_TO_SMAG16;
      else if constexpr (std::is_same_v<vUInt16, ToType>)
        mod1 = SFPSTOCHRND_MOD1_FP32_TO_UINT16; 
      else if constexpr (std::is_same_v<vSMag8, ToType>)
        mod1 = SFPSTOCHRND_MOD1_FP32_TO_SMAG8;
      else if constexpr (std::is_same_v<vUInt8, ToType>)
        mod1 = SFPSTOCHRND_MOD1_FP32_TO_UINT8; 
      else
        static_assert (false, "Cannot convert vFloat{,16[ab]} to target type");

      return ToType (__builtin_rvtt_sfpstochrnd_i
                     (val.get(), 0, mod1, impl_::stochrnd_rnd (round)));
    }
}

// smag/smag16 -> float/float16a/float16b/int32
template <typename ToType, typename FromType,
          typename std::enable_if_t<std::is_base_of<impl_::vVal, ToType>::value>* = nullptr,
          typename std::enable_if_t<std::is_base_of<vSMag, FromType>::value>* = nullptr>
sfpi_inline ToType convert (FromType val, RoundMode round [[gnu::unused]] = RoundMode::NearestStochastic)
{
  if constexpr (std::is_same_v<FromType, ToType>)
    return val;

  else if constexpr (std::is_base_of_v<vFloat, ToType>)
    {
      // to float, fp16a, fp16b
      unsigned mod1 =
          round == RoundMode::Nearest ? SFPCAST_MOD1_SM32_TO_FP32_RNE :
          round == RoundMode::NearestStochastic ? SFPCAST_MOD1_SM32_TO_FP32_RNS :
          ~0u;

      return convert<ToType> (vFloat (__builtin_rvtt_sfpcast (val.get (), mod1)), round);
    }

  else if constexpr (std::is_same_v<vInt, ToType>)
    return smag_to_int (val);

  else
    static_assert (false, "Cannot convert vSMag to target type");
}

// int -> smag
template <typename ToType, typename FromType,
          typename std::enable_if_t<std::is_base_of<impl_::vVal, ToType>::value>* = nullptr,
          typename std::enable_if_t<std::is_base_of<vInt, FromType>::value>* = nullptr>
sfpi_inline ToType convert (FromType val)
{
  if constexpr (std::is_same_v<FromType, ToType>)
    return val;

  else if constexpr (std::is_same_v<vSMag, ToType>)
    return int_to_smag (val);

  else
    static_assert (false, "Cannot convert vInt to target type");
}

// mag
template <typename ToType, typename FromType,
          typename std::enable_if_t<std::is_base_of<impl_::vVal, ToType>::value>* = nullptr,
          typename std::enable_if_t<std::is_same<vMag, FromType>::value>* = nullptr>
sfpi_inline ToType convert (FromType val, RoundMode round [[gnu::unused]] = RoundMode::NearestStochastic)
{
  if constexpr (std::is_same_v<FromType, ToType>)
    return val;

  else if constexpr (std::is_same_v<vInt, ToType>)
    return as<vInt> (val);

  else
    return convert<ToType> (as<vSMag> (val), round);
}

// uint16
template <typename ToType, typename FromType,
          typename std::enable_if_t<std::is_base_of<impl_::vVal, ToType>::value>* = nullptr,
          typename std::enable_if_t<std::is_same<vUInt16, FromType>::value>* = nullptr>
sfpi_inline ToType convert (FromType val, RoundMode round [[gnu::unused]] = RoundMode::NearestStochastic)
{
  if constexpr (std::is_same_v<FromType, ToType>)
    return val;

  else
    return convert<ToType> (as<vSMag> (val), round);
}

__SFPI_DEPRECATED("Use sfpi:convert<sfpi::vFloat> (X, rounding)")
sfpi_inline vFloat int32_to_float (vInt in, RoundMode rounding = RoundMode::NearestStochastic) {
  return __builtin_rvtt_sfpcast (in.get (), impl_::cast_rnd (rounding));
}
// shim
__SFPI_DEPRECATED("Use sfpi:convert<sfpi::vFloat> (X, rounding)")
sfpi_inline vFloat int32_to_float (vSMag in, RoundMode rounding = RoundMode::NearestStochastic) {
  return __builtin_rvtt_sfpcast (in.get (), impl_::cast_rnd (rounding));
}

__SFPI_DEPRECATED("Use sfpi:convert<sfpi::vFloat16a> (X, rounding)")
sfpi_inline vFloat float_to_fp16a (vFloat in, RoundMode rounding = RoundMode::NearestStochastic) {
  return __builtin_rvtt_sfpstochrnd_i
      (in.get(), 0,
       SFPSTOCHRND_MOD1_FP32_TO_FP16A, impl_::stochrnd_rnd (rounding));
}

__SFPI_DEPRECATED("Use sfpi:convert<sfpi::vFloat16b> (X, rounding)")
sfpi_inline vFloat float_to_fp16b (vFloat in, RoundMode rounding = RoundMode::NearestStochastic) {
  return __builtin_rvtt_sfpstochrnd_i
      (in.get(), 0,
       SFPSTOCHRND_MOD1_FP32_TO_FP16B, impl_::stochrnd_rnd (rounding));
}

__SFPI_DEPRECATED("Use sfpi:convert<sfpi::vUInt16> (X, rounding)")
sfpi_inline vUInt float_to_uint16 (vFloat in, RoundMode rounding = RoundMode::NearestStochastic) 
{
  return __builtin_rvtt_sfpstochrnd_i
      (in.get(), 0,
       SFPSTOCHRND_MOD1_FP32_TO_UINT16, impl_::stochrnd_rnd (rounding));
}

__SFPI_DEPRECATED("Use sfpi:convert<sfpi::vInt16> (X, rounding)")
sfpi_inline vInt float_to_int16 (vFloat in, RoundMode rounding = RoundMode::NearestStochastic) {
  return __builtin_rvtt_sfpstochrnd_i
      (in.get(), 0,
       SFPSTOCHRND_MOD1_FP32_TO_INT16, impl_::stochrnd_rnd (rounding));
}

__SFPI_DEPRECATED("Use sfpi:convert<sfpi::vUInt8> (X, rounding)")
sfpi_inline vUInt float_to_uint8 (vFloat in, RoundMode rounding = RoundMode::NearestStochastic) {
  return __builtin_rvtt_sfpstochrnd_i
      (in.get(), 0,
       SFPSTOCHRND_MOD1_FP32_TO_UINT8, impl_::stochrnd_rnd (rounding));
}

__SFPI_DEPRECATED("Use sfpi:convert<sfpi::vInt8> (X, rounding)")
sfpi_inline vInt float_to_int8 (vFloat in, RoundMode rounding = RoundMode::NearestStochastic) {
  return __builtin_rvtt_sfpstochrnd_i
      (in.get(), 0,
       SFPSTOCHRND_MOD1_FP32_TO_INT8, impl_::stochrnd_rnd (rounding));
}

// These do not appear used anywhere.  We should get to converting to a new
// convert-like API
sfpi_inline vUInt int32_to_uint8 (vInt in, vUInt descale, RoundMode rounding = RoundMode::NearestStochastic) {
  return __builtin_rvtt_sfpstochrnd_v
      (in.get(), descale.get(),
       SFPSTOCHRND_MOD1_INT32_TO_UINT8, impl_::stochrnd_rnd (rounding));
}

sfpi_inline vUInt int32_to_uint8 (vInt in, unsigned descale, RoundMode rounding = RoundMode::NearestStochastic) {
  return __builtin_rvtt_sfpstochrnd_i
      (in.get(), descale,
       SFPSTOCHRND_MOD1_INT32_TO_UINT8, impl_::stochrnd_rnd (rounding));
}

sfpi_inline vInt int32_to_int8 (vInt in, vUInt descale, RoundMode rounding = RoundMode::NearestStochastic) {
  return __builtin_rvtt_sfpstochrnd_v
      (in.get(), descale.get(),
       SFPSTOCHRND_MOD1_INT32_TO_INT8, impl_::stochrnd_rnd (rounding));
}

sfpi_inline vInt int32_to_int8 (vInt in, unsigned descale, RoundMode rounding = RoundMode::NearestStochastic) {
  return __builtin_rvtt_sfpstochrnd_i
      (in.get(), descale,
       SFPSTOCHRND_MOD1_INT32_TO_INT8, impl_::stochrnd_rnd (rounding));
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
sfpi_inline vInt rand () {
  return __builtin_rvtt_sfpreadconfig (SFPCONFIG_SRC_RAND);
}

enum class RecipMode {
  All,
  IfNegative
};

sfpi_inline vFloat approx_recip (vFloat src, RecipMode mode = RecipMode::All) {
#if __riscv_xtttensixbh
  return __builtin_rvtt_sfparecip (src.get (),
                                   mode == RecipMode::All ? SFPARECIP_MOD1_RECIP :
                                   mode == RecipMode::IfNegative ? SFPARECIP_MOD1_COND_RECIP :
                                   ~0);
#elif __riscv_xtttensixqsr
  return __builtin_rvtt_sfpnonlinear (src.get (),
                                      mode == RecipMode::All ? SFPNONLINEAR_MOD1_RECIP :
                                      mode == RecipMode::IfNegative ? SFPNONLINEAR_MOD1_COND_RECIP :
                                      ~0);
#endif
}

template <bool uncond = true>
__SFPI_DEPRECATED("Use sfpi::approx_mode(v, sfpi::RecipMode::{All,IfNegative})")
sfpi_inline vFloat approx_recip (vFloat src) {
return approx_recip (src, uncond ? RecipMode::All : RecipMode::IfNegative);
}

sfpi_inline vFloat approx_exp (vFloat src) {
#if __riscv_xtttensixbh
  return __builtin_rvtt_sfparecip (src.get(), SFPARECIP_MOD1_EXP);
#elif __riscv_xtttensixqsr
  return __builtin_rvtt_sfpnonlinear (src.get (), SFPNONLINEAR_MOD1_EXP);
#endif
}
#endif

#if __riscv_xtttensixqsr
// ReLU(x) = max (x,0).  This is just sfpswap (x, CST0, MAX)
sfpi_inline vFloat rectified_linear_unit (vFloat src) {
  return __builtin_rvtt_sfpnonlinear (src.get (), SFPNONLINEAR_MOD1_RELU);
}
sfpi_inline vFloat approx_sqrt (vFloat src) {
  return __builtin_rvtt_sfpnonlinear (src.get (), SFPNONLINEAR_MOD1_SQRT);
}
sfpi_inline vFloat approx_tanh (vFloat src) {
  return __builtin_rvtt_sfpnonlinear (src.get (), SFPNONLINEAR_MOD1_TANH);
}
#endif

vSMag impl_::int_to_smag (vInt val) {
  vSMag res = as<vSMag> (val);
#if !__riscv_xtttensixwh
  res = vSMag (__builtin_rvtt_sfpcast (res.get (), SFPCAST_MOD1_INT32_TO_SM32));
#else
  v_if (as<vInt> (res) < 0) {
    res = setsgn (res, 0);
    res = as<vSMag> (0 - as<vInt> (res));
  } v_endif;
#endif
  return res;
}

vInt impl_::smag_to_int (vSMag val) {
#if __riscv_xtttensixqsr
  val = vSMag (__builtin_rvtt_sfpcast (val.get (), SFPCAST_MOD1_SM32_TO_INT32));
#else
  v_if (as<vInt>(val) < 0) {
    val = setsgn (val, 0);
    val = as<vSMag>(0 - as<vInt> (val));
  } v_endif;
#endif
  return as<vInt>(val);
}

} // namespace sfpi
