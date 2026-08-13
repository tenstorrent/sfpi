/*
 * SPDX-FileCopyrightText: © 2023-2026 Tenstorrent Inc.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

///////////////////////////////////////////////////////////////////////////////
// sfpi_lib.h: SFPu Interface library free functions
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include <utility>

namespace sfpi {

template <typename Type, typename std::enable_if_t<std::is_base_of<impl_::vVal, Type>::value>* = nullptr>
__SFPI_DEPRECATED("Use sfpi::as<T>")
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

namespace impl_ {

template <bool FP16, unsigned Disc>
class sLut : public impl_::vVal {
public:
  using scalar_t = typename std::conditional<FP16, uint32_t, uint16_t>::type;
  using single_t = typename std::conditional<FP16, uint16_t, uint8_t>::type;

private:
  scalar_t val;

public:
  sfpi_inline sLut (sLut const &) = default;
  sfpi_inline sLut &operator= (sLut const &) = default;

public:
  // For pairs we want the first arg at the low (right) sigde and the second
  // arg at the high (left) side. For Slope&Intercept we want them the otherway
  // round.  Ugh.
  sfpi_inline explicit sLut (single_t a0, single_t a1)
      : val ((a0 << (Disc == 2 ? sizeof (single_t) * 8 : 0))
              | (a1 << (Disc != 2 ? sizeof (single_t) * 8 : 0))) {}
  sfpi_inline explicit sLut (int a0, int a1)
      : sLut (single_t (a0), single_t (a1)) {}
  sfpi_inline sLut (float a0, float a1)
      : sLut (convert (a0), convert (a1)) {}

  template<unsigned From, typename std::enable_if_t<From == 2 && Disc == 3>* = nullptr>
  sfpi_inline sLut (sLut<FP16, From> a0, sLut<FP16, From> a1)
      : sLut (single_t (a0.get () >> sizeof (single_t) * 8),
              single_t (a1.get () >> sizeof (single_t) * 8)) {}
  
  template<unsigned From, typename std::enable_if_t<From == 2 && Disc == 0>* = nullptr>
  sfpi_inline sLut (sLut<FP16, From> a0, sLut<FP16, From> a1)
      : sLut (single_t (a0.get ()), single_t (a1.get ())) {}

public:
  scalar_t get () const { return val; }

private:
  static single_t convert (float v) {
    // FP8 = S:1, E:3, M:4, exp is unbiased, negated
    // FP16 = S:1, E:5, M:10, exp is biased by 16
    constexpr unsigned man_bits = FP16 ? 10 : 4;

    // Extract
    auto bits = impl_::float_as_uint (v);
    unsigned sgn = bits >> 31;
    int exp = (bits >> 23) & 0xff;
    unsigned man = bits & ((1 << 23) - 1);

    // Round to nearest, not handling nearest-even case
    man += (1 << (23 - man_bits - 1));
    if (man >> 23)
      {
        exp++;
        man = 0;
      }

    man >>= 23 - man_bits;
    if constexpr (FP16)
      {
        // Float must be in range +/-[0,16.0f)
        exp -= 128 - 16;
        return (sgn << 15)
            | (exp < 0 ? 0x7c00 // underflow -> 0.0f
               : exp >= 31 ? 0x7bff // overflow -> 16.0f - epsilon
               : (exp << 10) | man);
      }
    else
      {
        // Float must be in range +/-[0,2.0f)
        exp -= 128;

        // Even though representations [0xf0,0xff) are valid non-zero numbers,
        // there is a hole for 0xff, which is treated as zero. Oh well.
        return (sgn << 7)
            | (exp + 7 < 0 ? 0xff // underflow -> 0.0f
               : exp >= 0 ? 0x0f // overflow -> 2.0f - epsilon
               : ((-1 - exp) << 4) | man);
      }
  }  
};

template <bool FP16, unsigned Disc>
class vLut : public impl_::vVal {
  using sLut = impl_::sLut<FP16, Disc>;

public:
  sfpi_inline vLut (vLut const &) = default;
  sfpi_inline vLut &operator= (vLut const &) = default;

public:
  sfpi_inline explicit vLut (sLut v)
      : vVal (__builtin_rvtt_sfpxloadi (v.get (), -int (sizeof (typename sLut::scalar_t)) * 8)) {}
  sfpi_inline explicit vLut (impl_::sfpu_t vec) : vVal (vec) {}
  sfpi_inline explicit vLut (typename sLut::single_t a0, typename sLut::single_t a1)
      : vLut (sLut (a0, a1)) {}
  sfpi_inline vLut (float a0, float a1)
      : vLut (sLut (a0, a1)) {}
};
}

using sLut8si = impl_::sLut<false, 2>;
using sLut16si = impl_::sLut<true, 2>;
using sLut16ss = impl_::sLut<true, 3>;
using sLut16ii = impl_::sLut<true, 0>;
class sLut32si {
private:
  float slope;
  float intercept;

public:
  sfpi_inline sLut32si (float s, float i)
      : slope (s), intercept (i) {}

public:
  vFloat s () const { return vFloat (slope); }
  vFloat i () const { return vFloat (intercept); }
};
using vLut8si = impl_::vLut<false, 2>;
using vLut16si = impl_::vLut<true, 2>;
using vLut16ss = impl_::vLut<true, 3>;
using vLut16ii = impl_::vLut<true, 0>;
class vLut32si {
  using sLut = sLut32si;

private:
  vFloat slope;
  vFloat intercept;

public:
  sfpi_inline vLut32si (sLut v)
      : slope (v.s ()), intercept (v.i ()) {}
  sfpi_inline vLut32si (vFloat s, vFloat i)
      : slope (s), intercept (i) {}
  sfpi_inline vLut32si (float s, float i)
      : vLut32si (vFloat (s), vFloat (i)) {}

public:
  impl_::sfpu_t s_get () const { return slope.get (); }
  impl_::sfpu_t i_get () const { return intercept.get (); }
};

enum class LutSign {
  Retain,
  Update
};

enum class LutMode {
  Fp8x3,       // sfplut
  Fp16x3,      // sfplutfp32 FP16 3Entry
  Fp32x3,      // sfplutfp32 FP32 3Entry
  Fp16x6_HWM3, // sfplutfp32 FP16 6Entry Table1
  Fp16x6_HWM4, // sfplutfp32 FP16 6Entry Table2
};

#if __riscv_xtttensixqsr
sfpi_inline void lut_init (unsigned ix, sLut8si v) {
  __builtin_rvtt_sfpwriteconfig_v (vLut8si (v).get (), 0, ix + CREG_IDX_LUT_SLOPES);
}
sfpi_inline void lut_init (unsigned ix, sLut16si v) {
  __builtin_rvtt_sfpwriteconfig_v (vLut16si (v).get (), 0, ix + CREG_IDX_LUT_SLOPES);
}
sfpi_inline void lut_init (unsigned ix, sLut16ss s, sLut16ii i) {
  __builtin_rvtt_sfpwriteconfig_v (vLut16ss (s).get (), 0, ix + CREG_IDX_LUT_SLOPES);
  __builtin_rvtt_sfpwriteconfig_v (vLut16ii (i).get (), 0, ix + CREG_IDX_LUT_INTERCEPTS);
}
sfpi_inline void lut_init (unsigned ix, sLut16si si0, sLut16si si1) {
  __builtin_rvtt_sfpwriteconfig_v (vLut16ss (sLut16ss (si0, si1)).get (), 0, ix + CREG_IDX_LUT_SLOPES);
  __builtin_rvtt_sfpwriteconfig_v (vLut16ii (sLut16ii (si0, si1)).get (), 0, ix + CREG_IDX_LUT_INTERCEPTS);
}
sfpi_inline void lut_init (unsigned ix, sLut32si si) {
  __builtin_rvtt_sfpwriteconfig_v (si.s ().get (), 0, ix + CREG_IDX_LUT_SLOPES);
  __builtin_rvtt_sfpwriteconfig_v (si.i ().get (), 0, ix + CREG_IDX_LUT_INTERCEPTS);
}

template <LutMode>
struct LutCookie {};

template <LutMode Mode = LutMode::Fp8x3>
sfpi_inline LutCookie<Mode> lut_init (sLut8si si0, sLut8si si1, sLut8si si2) {
  static_assert (Mode == LutMode::Fp8x3, "Unsupported LutMode");
  lut_init (0, si0);
  lut_init (1, si1);
  lut_init (2, si2);
  return LutCookie<LutMode::Fp8x3> ();
}

template <LutMode Mode = LutMode::Fp16x3>
sfpi_inline LutCookie<Mode> lut_init (sLut16si si0, sLut16si si1, sLut16si si2) {
  static_assert (Mode == LutMode::Fp16x3, "Unsupported LutMode");
  lut_init (0, si0);
  lut_init (1, si1);
  lut_init (2, si2);
  return LutCookie<LutMode::Fp16x3> ();
}

template <LutMode Mode = LutMode::Fp32x3>
sfpi_inline LutCookie<Mode> lut_init (sLut32si si0, sLut32si si1, sLut32si si2) {
  static_assert (Mode == LutMode::Fp32x3, "Unsupported LutMode");
  lut_init (0, si0);
  lut_init (1, si1);
  lut_init (2, si2);
  return LutCookie<LutMode::Fp32x3> ();
}

template <LutMode Mode = LutMode::Fp16x6_HWM3>
sfpi_inline LutCookie<Mode> lut_init (sLut16ss s01, sLut16ii i01,
                                      sLut16ss s23, sLut16ii i23,
                                      sLut16ss s45, sLut16ii i45) {
  static_assert (Mode == LutMode::Fp16x6_HWM3 || Mode == LutMode::Fp16x6_HWM4, "Unsupported LutMode");
  lut_init (0, s01, i01);
  lut_init (1, s23, i23);
  lut_init (2, s45, i45);
  return {};
}

template <LutMode Mode = LutMode::Fp16x6_HWM3>
sfpi_inline LutCookie<Mode> lut_init (sLut16si si0, sLut16si si1,
                                      sLut16si si2, sLut16si si3,
                                      sLut16si si4, sLut16si si5) {
  return lut_init<Mode> (sLut16ss (si0, si1), sLut16ii (si0, si1),
                         sLut16ss (si2, si3), sLut16ii (si2, si3),
                         sLut16ss (si4, si5), sLut16ii (si4, si5));
}

template <LutMode Mode>
sfpi_inline vFloat lut (vFloat v, LutSign signedness = LutSign::Retain) {
  unsigned mod =
      signedness == LutSign::Retain ? SFPLUTFP32_MOD0_SGN_RETAIN :
      signedness == LutSign::Update ? SFPLUTFP32_MOD0_SGN_UPDATE :
      ~0;
  if constexpr (Mode == LutMode::Fp8x3)
    return __builtin_rvtt_sfplut (v.get (), mod);
  else
    {
      mod |=
          Mode == LutMode::Fp16x3 ? SFPLUTFP32_MOD0_FP16_3ENTRY_TABLE :
          Mode == LutMode::Fp32x3 ? SFPLUTFP32_MOD0_FP32_3ENTRY_TABLE :
          Mode == LutMode::Fp16x6_HWM3 ? SFPLUTFP32_MOD0_FP16_6ENTRY_TABLE1 :
          Mode == LutMode::Fp16x6_HWM4 ? SFPLUTFP32_MOD0_FP16_6ENTRY_TABLE2 :
          ~0;
      return __builtin_rvtt_sfplutfp32 (v.get (), mod);
    }
}

template <LutMode Mode>
sfpi_inline vFloat lut (vFloat v, LutCookie<Mode>, LutSign signedness = LutSign::Retain) {
  return lut<Mode> (v, signedness);
}
#else

template <LutMode Mode = LutMode::Fp8x3>
sfpi_inline vFloat lut (vFloat v, vLut8si si0, vLut8si si1, vLut8si si2,
                        LutSign signedness = LutSign::Retain) {
  unsigned mod = (Mode == LutMode::Fp8x3 ? 0 : ~0)
      | (signedness == LutSign::Retain ? SFPLUT_MOD0_SGN_RETAIN :
         signedness == LutSign::Update ? SFPLUT_MOD0_SGN_UPDATE :
         ~0);
  return __builtin_rvtt_sfplut (si0.get (), si1.get (), si2.get (), v.get (), mod);
}

__SFPI_DEPRECATED("Pass float or vFloat coefficients")
sfpi_inline vFloat lut (vFloat v, vUInt si0, vUInt si1, vUInt si2) {
  return lut<LutMode::Fp8x3> (v, as<vLut8si> (si0), as<vLut8si> (si1), as<vLut8si> (si2));
}

__SFPI_DEPRECATED("Pass float or vFloat coefficients, pass sfpi::LutSign::Update")
sfpi_inline vFloat lut_sign (vFloat v, vUInt si0, vUInt si1, vUInt si2) {
  return lut<LutMode::Fp8x3> (v, as<vLut8si> (si0), as<vLut8si> (si1), as<vLut8si> (si2), LutSign::Update);
}

template <LutMode Mode = LutMode::Fp16x3>
sfpi_inline vFloat lut (vFloat v, vLut16si si0, vLut16si si1, vLut16si si2,
                        LutSign signedness = LutSign::Retain) {
  unsigned mod = (Mode == LutMode::Fp16x3 ? SFPLUTFP32_MOD0_FP16_3ENTRY_TABLE : ~0)
      | (signedness == LutSign::Retain ? SFPLUTFP32_MOD0_SGN_RETAIN :
         signedness == LutSign::Update ? SFPLUTFP32_MOD0_SGN_UPDATE :
         ~0);
  return __builtin_rvtt_sfplutfp32_3r (si0.get (), si1.get (), si2.get (), v.get (), mod);
}

__SFPI_DEPRECATED("Use sfpi::lut, pass float or vFloat coefficients")
sfpi_inline vFloat lut2 (vFloat v, vUInt si0, vUInt si1, vUInt si2) {
  return lut<LutMode::Fp16x3> (v, as<vLut16si> (si0), as<vLut16si> (si1), as<vLut16si> (si2));
}

__SFPI_DEPRECATED("Use sfpi::lut, pass float or vFloat coefficients, pass sfpi::LutSign::Update")
sfpi_inline vFloat lut2_sign (vFloat v, vUInt si0, vUInt si1, vUInt si2) {
  return lut<LutMode::Fp16x3> (v,
                               as<vLut16si> (si0), as<vLut16si> (si1), as<vLut16si> (si2),
                               LutSign::Update);
}

template <LutMode Mode = LutMode::Fp32x3>
sfpi_inline vFloat lut (vFloat v, vLut32si si0, vLut32si si1, vLut32si si2,
                        LutSign signedness = LutSign::Retain) {
  unsigned mod = (Mode == LutMode::Fp32x3 ? SFPLUTFP32_MOD0_FP32_3ENTRY_TABLE : ~0)
      | (signedness == LutSign::Retain ? SFPLUTFP32_MOD0_SGN_RETAIN :
         signedness == LutSign::Update ? SFPLUTFP32_MOD0_SGN_UPDATE :
         ~0);
  return __builtin_rvtt_sfplutfp32_6r (si0.s_get (), si1.s_get (), si2.s_get (),
                                       si0.i_get (), si1.i_get (), si2.i_get (),
                                       v.get (), mod);
}

__SFPI_DEPRECATED("Use sfpi::lut")
sfpi_inline vFloat lut2 (vFloat v,
                         vFloat s0, vFloat s1, vFloat s2,
                         vFloat i0, vFloat i1, vFloat i2) {
  return lut<LutMode::Fp32x3> (v, vLut32si (s0, i0), vLut32si (s1, i1), vLut32si (s2, i2));
}

__SFPI_DEPRECATED("Use sfpi::lut, pass sfpi::LutSign::Update")
sfpi_inline vFloat lut2_sign (vFloat v,
                              vFloat s0, vFloat s1, vFloat s2,
                              vFloat i0, vFloat i1, vFloat i2) {
  return lut<LutMode::Fp32x3> (v, vLut32si (s0, i0), vLut32si (s1, i1), vLut32si (s2, i2), LutSign::Update);
}

template <LutMode Mode = LutMode::Fp16x6_HWM3>
sfpi_inline vFloat lut (vFloat v,
                        vLut16ss s01, vLut16ii i01,
                        vLut16ss s23, vLut16ii i23,
                        vLut16ss s45, vLut16ii i45,
                        LutSign signedness = LutSign::Retain) {
  unsigned mod =
      (Mode == LutMode::Fp16x6_HWM3 ? SFPLUTFP32_MOD0_FP16_6ENTRY_TABLE1 :
       Mode == LutMode::Fp16x6_HWM4 ? SFPLUTFP32_MOD0_FP16_6ENTRY_TABLE2 :
       ~0)
      | (signedness == LutSign::Retain ? SFPLUTFP32_MOD0_SGN_RETAIN :
         signedness == LutSign::Update ? SFPLUTFP32_MOD0_SGN_UPDATE :
         ~0);
  return __builtin_rvtt_sfplutfp32_6r (s01.get (), s23.get (), s45.get (),
                                       i01.get (), i23.get (), i45.get (),
                                       v.get (), mod);
}

__SFPI_DEPRECATED("Use sfpi::lut, pass vLut16")
sfpi_inline vFloat lut2 (vFloat v,
                         vUInt s01, vUInt s23, vUInt s45,
                         vUInt i01, vUInt i23, vUInt i45, int mode = 1) {
  if (mode == 1)
    return lut<LutMode::Fp16x6_HWM3> (v, as<vLut16ss> (s01), as<vLut16ii> (i01),
                                      as<vLut16ss> (s23), as<vLut16ii> (i23),
                                      as<vLut16ss> (s45), as<vLut16ii> (i45));
  else
    return lut<LutMode::Fp16x6_HWM4> (v, as<vLut16ss> (s01), as<vLut16ii> (i01),
                                      as<vLut16ss> (s23), as<vLut16ii> (i23),
                                      as<vLut16ss> (s45), as<vLut16ii> (i45));
}

__SFPI_DEPRECATED("Use sfpi::lut, pass vLut16, pass sfpi::LutSign::Update")
sfpi_inline vFloat lut2_sign (vFloat v,
                              vUInt s01, vUInt s23, vUInt s45,
                              vUInt i01, vUInt i23, vUInt i45, int mode = 1) {
  if (mode == 1)
    return lut<LutMode::Fp16x6_HWM3> (v, as<vLut16ss> (s01), as<vLut16ii> (i01),
                                      as<vLut16ss> (s23), as<vLut16ii> (i23),
                                      as<vLut16ss> (s45), as<vLut16ii> (i45),
                                      LutSign::Update);
  else
    return lut<LutMode::Fp16x6_HWM4> (v, as<vLut16ss> (s01), as<vLut16ii> (i01),
                                      as<vLut16ss> (s23), as<vLut16ii> (i23),
                                      as<vLut16ss> (s45), as<vLut16ii> (i45),
                                      LutSign::Update);
}
#endif

#if 0
enum
#endif
class ExponentMode {
#if 1
 public: enum Values {
#endif
  Unbiased,
  Biased,
#if 1
  };
 __SFPI_DEPRECATED("Use sfpi::ExponentMode::Unbiased")
 static constexpr Values Debias = Unbiased;
 __SFPI_DEPRECATED("Use sfpi::ExponentMode::Biased")
 static constexpr Values NoDebias = Biased;

 private: Values v;

 public: constexpr ExponentMode (Values v) : v (v) {}
 public: constexpr operator Values () const { return v; }
#endif
};

sfpi_inline vInt exexp (const vFloat v, ExponentMode mode = ExponentMode::Unbiased) {
  return __builtin_rvtt_sfpexexp (v.get (),
                                  mode == ExponentMode::Unbiased ? SFPEXEXP_MOD1_DEBIAS :
                                  mode == ExponentMode::Biased ? SFPEXEXP_MOD1_NODEBIAS :
                                  ~0 /* bad value, compile error */);
}

__SFPI_DEPRECATED("Use sfpi::exexp (X, sfpi::ExponentMode::Biased)")
sfpi_inline vInt exexp_nodebias(const vFloat v) {
  return exexp (v, ExponentMode::Biased);
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

sfpi_inline vFloat setexp (vFloat v, int exp) {
  return __builtin_rvtt_sfpsetexp_i (v.get(), exp, 0);
}

sfpi_inline vFloat setexp (vFloat v, vInt exp) {
  return __builtin_rvtt_sfpsetexp_v (v.get (), exp.get (), 0);
}

sfpi_inline vFloat copyexp (vFloat v, vFloat exp) {
  return __builtin_rvtt_sfpsetexp_v (v.get (), exp.get (), SFPSETEXP_MOD1_CPY);
}

sfpi_inline vFloat addexp (vFloat in, int exp) {
  return __builtin_rvtt_sfpdivp2 (in.get (), exp, SFPSDIVP2_MOD1_ADD);
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

enum class LdexpMode {
  Correct,
  Fast,
};

sfpi_inline vFloat ldexp (vFloat in, int scale, LdexpMode = LdexpMode::Correct) {
  return addexp (in, scale);
}

sfpi_inline vFloat ldexp (vFloat in, vInt scale, LdexpMode mode = LdexpMode::Correct) {
  if (mode == LdexpMode::Fast)
    return setexp (in, exexp (in, ExponentMode::Biased) + scale);
  else
    return in * setexp (vFloat (0), scale + 127);
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

sfpi_inline vBool is_nan (vFloat v) {
  return exexp (v, ExponentMode::Biased) >= 255
      && exman (v) != 0;
}

sfpi_inline vBool is_finite (vFloat v) {
  return exexp (v, ExponentMode::Biased) < 255;
}

sfpi_inline vBool is_normal (vFloat v) {
  auto exp = exexp (v, ExponentMode::Biased);
  return exp < 255 && exp != 0;
}

sfpi_inline vBool is_subnormal (vFloat v) {
  return exexp (v, ExponentMode::Biased) == 0
      && exman (v) != 0;
}

sfpi_inline vBool is_zero (vFloat v) {
  return (as<vUInt>(v) << 1) == 0;
}

sfpi_inline vBool is_inf (vFloat v) {
  return exexp (v, ExponentMode::Biased) >= 255
      && exman (v) == 0;
}

sfpi_inline vBool is_pos (vFloat v) {
  return lz (as<vUInt> (v)) != 0;
}

sfpi_inline vBool is_neg (vFloat v) {
  return lz (as<vUInt> (v)) == 0;
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

__SFPI_DEPRECATED("Use non-2's complement types")
sfpi_inline vInt fractional_mul (vInt a, vInt b, FractionalHalf half = FractionalHalf::Low) {
  return fractional_mul (as<vUInt> (a), as<vUInt> (b), half);
}
#endif

template <typename Type,
          typename std::enable_if_t<std::disjunction<std::is_base_of<vFloat, Type>,
#if __riscv_xtttensixqsr
                                                     std::is_base_of<vInt, Type>,
#endif
                                                     std::is_base_of<vSMag, Type>>::value>* = nullptr>
sfpi_inline std::pair<Type, Type> min_max (Type a, Type b, uint32_t mask = 0) {
  // mask has 0 for min res and 1 for max res
  uint32_t swap =
      // 32-bit mask
      mask == 0xffffffffu ? 0xffffffff :
      mask == 0xff0000ffu ? 0xffffffff:
      mask == 0xff000000u ? 0xffffffff :
      mask == 0x00ff00ffu ? 0xffffffff :
      mask == 0x00ff0000u ? 0xffffffff :
      mask == 0x0000ffffu ? 0xffffffff :
      mask == 0x0000ff00u ? 0xffffffff :
      mask == 0x000000ffu ? 0xffffffff :
      // 4 bit mask
      mask == 0xfu ? 0xf :
      mask == 0x9u ? 0xf :
      mask == 0x8u ? 0xf :
      mask == 0x5u ? 0xf :
      mask == 0x4u ? 0xf :
      mask == 0x3u ? 0xf :
      mask == 0x2u ? 0xf :
      mask == 0x1u ? 0xf :
      0;
  unsigned mod =
      (mask ^ swap) == 0x00000000u ? SFPSWAP_MOD1_VEC_MIN_MAX :
      (mask ^ swap) == 0x00ffff00u ? SFPSWAP_MOD1_SUBVEC_MIN03_MAX12:
      (mask ^ swap) == 0x00ffffffu ? SFPSWAP_MOD1_SUBVEC_MIN3_MAX012 :
      (mask ^ swap) == 0xff00ff00u ? SFPSWAP_MOD1_SUBVEC_MIN02_MAX13 :
      (mask ^ swap) == 0xff00ffffu ? SFPSWAP_MOD1_SUBVEC_MIN2_MAX013 :
      (mask ^ swap) == 0xffff0000u ? SFPSWAP_MOD1_SUBVEC_MIN01_MAX23 :
      (mask ^ swap) == 0xffff00ffu ? SFPSWAP_MOD1_SUBVEC_MIN1_MAX023 :
      (mask ^ swap) == 0xffffff00u ? SFPSWAP_MOD1_SUBVEC_MIN0_MAX123 :
      (mask ^ swap) == 0x0u ? SFPSWAP_MOD1_VEC_MIN_MAX : // Same as first condition
      (mask ^ swap) == 0x6u ? SFPSWAP_MOD1_SUBVEC_MIN03_MAX12:
      (mask ^ swap) == 0x7u ? SFPSWAP_MOD1_SUBVEC_MIN3_MAX012 :
      (mask ^ swap) == 0xau ? SFPSWAP_MOD1_SUBVEC_MIN02_MAX13 :
      (mask ^ swap) == 0xbu ? SFPSWAP_MOD1_SUBVEC_MIN2_MAX013 :
      (mask ^ swap) == 0xcu ? SFPSWAP_MOD1_SUBVEC_MIN01_MAX23 :
      (mask ^ swap) == 0xdu ? SFPSWAP_MOD1_SUBVEC_MIN1_MAX023 :
      (mask ^ swap) == 0xeu ? SFPSWAP_MOD1_SUBVEC_MIN0_MAX123 :
      ~0; // Bad value, compilation error
  auto res = __builtin_rvtt_sfpswap (swap ? b.get () : a.get (),
                                     swap ? a.get () : b.get (), mod
#if __riscv_xtttensixqsr
                                     , std::is_base_of_v<vFloat, Type> ? SFPSWAP_IMM_TYPE_FLOAT :
                                     std::is_base_of_v<vInt, Type> ? SFPSWAP_IMM_TYPE_INT :
                                     std::is_base_of_v<vSMag, Type> ? SFPSWAP_IMM_TYPE_SMAG :
                                     ~0
#endif
                                     );
  auto r0 = Type (__builtin_rvtt_sfpselect2 (res, swap != 0));
  auto r1 = Type (__builtin_rvtt_sfpselect2 (res, swap == 0));
  return std::pair (r0, r1);
}

template <typename Type,
          typename std::enable_if_t<std::disjunction<std::is_base_of<vFloat, Type>,
#if __riscv_xtttensixqsr
                                                     std::is_base_of<vInt, Type>,
#endif
                                                     std::is_base_of<vSMag, Type>>::value>* = nullptr>
sfpi_inline Type min (Type a, Type b) {
  return min_max (a, b).first;
}

sfpi_inline vFloat min (vFloat a, float b) {
  return min (a, vFloat (b));
}

#if __riscv_xtttensixqsr
sfpi_inline vInt min (vInt a, int b) {
  return min (a, vInt (b));
}
#endif

template <typename Type,
          typename std::enable_if_t<std::disjunction<std::is_base_of<vFloat, Type>,
#if __riscv_xtttensixqsr
                                                     std::is_base_of<vInt, Type>,
#endif
                                                     std::is_base_of<vSMag, Type>>::value>* = nullptr>
sfpi_inline Type max (Type a, Type b) {
  return min_max (a, b, 0xf).first;
}

sfpi_inline vFloat max (vFloat a, float b) {
  return max (a, vFloat (b));
}

#if __riscv_xtttensixqsr
sfpi_inline vInt max (vInt a, int b) {
  return max (a, vInt (b));
}
#endif

// Due to hardware limitations, ordering compares of unsigned do not work when
// MSB is one. Sadly the compiler doesn't (yet) compensate
sfpi_inline vUInt min (vUInt x, unsigned c) {
  vUInt cv = c;
  if (reinterpret_cast<int const &> (c) >= 0)
    return ~as<vUInt> (min (as<vSMag> (~x), as<vSMag> (~cv)));

  return as<vUInt> (max (as<vSMag> (x), as<vSMag> (cv)));  
}

sfpi_inline vUInt max (vUInt x, unsigned c) {
  vUInt cv = c;
  if (reinterpret_cast<int const &> (c) >= 0)
    return ~as<vUInt> (max (as<vSMag> (~x), as<vSMag> (~cv)));

  return as<vUInt> (min (as<vSMag> (x), as<vSMag> (cv)));  
}

sfpi_inline vUInt clamp (vUInt x, unsigned lower, unsigned upper) {
  return min (max (x, lower), upper);
}

template <typename Type,
          typename std::enable_if_t<std::disjunction<std::is_base_of<vFloat, Type>,
#if __riscv_xtttensixqsr
                                                     std::is_base_of<vInt, Type>,
#endif
                                                     std::is_base_of<vSMag, Type>>::value>* = nullptr>
sfpi_inline Type clamp (Type val, Type lower, Type upper) {
  return min (max (val, lower), upper);
}

sfpi_inline vFloat clamp (vFloat val, float lower, float upper) {
  return clamp (val, vFloat (lower), vFloat (upper));
}

#if __riscv_xtttensixqsr
sfpi_inline vInt clamp (vInt val, int lower, int upper) {
  return clamp (val, vInt (lower), vInt (upper));
}
#endif

sfpi_inline vFloat symmetric_clamp (vFloat val, float bound) {
  return copysgn (min (abs (val), bound), val);
}

template <typename Type,
          typename std::enable_if_t<std::is_base_of<impl_::vVal, Type>::value>* = nullptr>
sfpi_inline void swap (Type &a, Type &b) {
  auto r = __builtin_rvtt_sfpswap (a.get(), b.get (), SFPSWAP_MOD1_SWAP
#if __riscv_xtttensixqsr
                                   , SFPSWAP_IMM_TYPE_INT
#endif 
                                  );
  a = Type (__builtin_rvtt_sfpselect2 (r, 0));
  b = Type (__builtin_rvtt_sfpselect2 (r, 1));
}

namespace impl_ {
class FloatInt : public std::pair<vFloat, vInt> {
public:
  using pair::pair;

public:
  operator vFloat () const { return first; }
  operator vInt () const { return second; }
};
}

sfpi_inline impl_::FloatInt round (vFloat x) {
  vFloat magic = 0x1.8p23f, f = x + magic;
  return {f - magic, as<vInt> (f) - as<vInt> (magic)};
}


__SFPI_DEPRECATED("Use sfpi::swap")
sfpi_inline void vec_swap (vFloat & a, vFloat &b) {
  swap (a, b);
}
__SFPI_DEPRECATED("Use sfpi::swap")
sfpi_inline void vec_swap (vInt &a, vInt &b) {
  swap (a, b);
}
__SFPI_DEPRECATED("Use sfpi::swap")
sfpi_inline void vec_swap (vUInt &a, vUInt &b) {
  swap (a, b);
}

__SFPI_DEPRECATED("Use sfpi::min_max")
sfpi_inline void vec_min_max (vFloat &a, vFloat &b) {
  auto r = min_max (a, b);
  a = r.first;
  b = r.second;
}
__SFPI_DEPRECATED("Use min_max with vSMag type")
sfpi_inline void vec_min_max (vInt &a, vInt &b) {
  auto r = __builtin_rvtt_sfpswap (a.get (), b.get (), SFPSWAP_MOD1_VEC_MIN_MAX
#if __riscv_xtttensixqsr
                                   , SFPSWAP_IMM_TYPE_SMAG
#endif
                                   );
  a = __builtin_rvtt_sfpselect2 (r, 0);
  b = __builtin_rvtt_sfpselect2 (r, 1);
}

#if __riscv_xtttensixbh || __riscv_xtttensixqsr
sfpi_inline vInt rand () {
  return __builtin_rvtt_sfpreadconfig (SFPCONFIG_SRC_RAND);
}

// ReLU(x) = max (x,0)
sfpi_inline vFloat rectified_linear_unit (vFloat src) {
#if __riscv_xtttensixqsr
  return __builtin_rvtt_sfpnonlinear (src.get (), SFPNONLINEAR_MOD1_RELU);
#else
  return max (src, 0.0f);
#endif
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
__SFPI_DEPRECATED("Use sfpi::approx_recip(v, sfpi::RecipMode::{All,IfNegative})")
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
sfpi_inline vFloat approx_sqrt (vFloat src) {
  return __builtin_rvtt_sfpnonlinear (src.get (), SFPNONLINEAR_MOD1_SQRT);
}
sfpi_inline vFloat approx_tanh (vFloat src) {
  return __builtin_rvtt_sfpnonlinear (src.get (), SFPNONLINEAR_MOD1_TANH);
}
#endif

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
  return ~0;
}
}

// conversions

// float/float16a/float16b -> float/float16a/float16b/int32/smag32/smag16/uint16
template <typename ToType, typename FromType,
          typename std::enable_if_t<std::is_base_of<impl_::vVal, ToType>::value>* = nullptr,
          typename std::enable_if_t<std::is_base_of<vFloat, FromType>::value>* = nullptr>
sfpi_inline ToType convert (FromType val, RoundMode round [[gnu::unused]] = RoundMode::NearestStochastic)
{
  if constexpr (std::is_same_v<FromType, ToType>)
    return val;

#if __riscv_xtttensixqsr
  else if constexpr (std::is_same_v<vSMag, ToType>
                     || std::is_same_v<vInt, ToType>)
    {
      // To smag
      unsigned mod1 =
          round == RoundMode::NearestEven ? SFPCAST_MOD1_FP32_TO_SM32_RNE :
          round == RoundMode::NearestStochastic ? SFPCAST_MOD1_FP32_TO_SM32_RNS :
          ~0u;

      auto tmp = vSMag (__builtin_rvtt_sfpcast (val.get (), mod1));
      if constexpr (std::is_same_v<vInt, ToType>)
        return impl_::smag_to_int (tmp);
      else
        return tmp;
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
    return impl_::smag_to_int (val);

  else {
    static_assert (false, "Cannot convert vSMag to target type");
    return ToType (0);
  }
}

// int -> smag/float
template <typename ToType, typename FromType,
          typename std::enable_if_t<std::is_base_of<impl_::vVal, ToType>::value>* = nullptr,
          typename std::enable_if_t<std::is_base_of<vInt, FromType>::value>* = nullptr>
sfpi_inline ToType convert (FromType val, RoundMode round [[gnu::unused]] = RoundMode::NearestStochastic)
{
  if constexpr (std::is_same_v<FromType, ToType>)
    return val;

  else if constexpr (std::is_same_v<vSMag, ToType>)
    return impl_::int_to_smag (val);

  else if constexpr (std::is_base_of_v<vFloat, ToType>)
    return convert<ToType> (impl_::int_to_smag (val), round);

  else {
    static_assert (false, "Cannot convert vInt to target type");
    return ToType (0);
  }
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

__SFPI_DEPRECATED("This converts a sign-magnitude type, despite its name and argument type. Use sfpi:convert<sfpi::vFloat> (X, rounding), which will convert from both from sign-maginitude and from 2's complement (via sign-magnitude))")
sfpi_inline vFloat int32_to_float (vInt in, RoundMode rounding = RoundMode::NearestStochastic) {
  return __builtin_rvtt_sfpcast (in.get (), impl_::cast_rnd (rounding));
}
// shim
__SFPI_DEPRECATED("This converts a sign-magnitude type, despite its name, use sfpi:convert<sfpi::vFloat> (X, rounding)")
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

// Polynomial evaluator res = c0 + c1.x^1 + c2.x^2 + ...
// Coefficients are float or vFloat
template<typename Coeff0, typename... Coeffs,
         typename std::enable_if<std::disjunction<std::is_base_of<vFloat, Coeff0>,
                                                  std::is_same<float, Coeff0>>::value>* = nullptr>
sfpi_inline vFloat polynomial (vFloat x, Coeff0 c0, Coeffs... cs) {
  return  vFloat (c0) + x * polynomial (x, cs...);
}
template <typename Coeff0,
          typename std::enable_if<std::disjunction<std::is_base_of<vFloat, Coeff0>,
                                                   std::is_same<float, Coeff0>>::value>* = nullptr>
sfpi_inline vFloat polynomial (vFloat, Coeff0 c0) {
  return vFloat (c0);
}
sfpi_inline vFloat polynomial (vFloat) {
  return 0.0f;
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
  // Unfortunately BH's sfpcast implementation cannot be used here :(
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
