/* -*- C++ -*-
 * SPDX-FileCopyrightText: © 2023-2026 Tenstorrent Inc.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

// Postcondition SFPI implementation file.  Not for end use consumption. See sfpi.h

// Use qualified names here to make sure the entity is already declared somewhere

#pragma once

uint32_t sfpi::impl_::float_as_uint (float val) {
  union U {
    float f;
    uint32_t i;

    constexpr U (float inf) : f (inf) {}
    operator uint32_t () const { return i; }
  };

  return U (val);
}

sfpi_inline sfpi::sFloat16b::sFloat16b (float v)
    : val (uint16_t (impl_::float_as_uint (v) >> 16)) {}

//////////////////////////////////////////////////////////////////////////////
// impl_::vCond definitions
sfpi::impl_::vCond::vCond (BoolOp t, vCond a, vCond b) {
  result = __builtin_rvtt_sfpxbool (t, a.get (), b.get ());
}
sfpi::impl_::vCond::vCond (CondOp t, vFloat a, float b) {
  result = __builtin_rvtt_sfpxfcmps (a.get (), impl_::float_as_uint (b), t | SFPXSCMP_MOD1_FMT_FLOAT);
}
sfpi::impl_::vCond::vCond (CondOp t, vFloat a, vFloat b) {
  result = __builtin_rvtt_sfpxfcmpv (a.get (), b.get (), t);
}
sfpi::impl_::vCond::vCond (CondOp t, vInt a, int32_t b, unsigned mod) {
  result = __builtin_rvtt_sfpxicmps (a.get (), b, mod | t);
}
sfpi::impl_::vCond::vCond (CondOp t, vInt a, vInt b, unsigned mod) {
  result = __builtin_rvtt_sfpxicmpv (a.get (), b.get (), mod | t);
}
sfpi::impl_::vCond::vCond (vInt a) {
  result = __builtin_rvtt_sfpxicmps (a.get(), 0, NE);
}

sfpi::impl_::vCond::CC::CC (CC &&src)
    : dep (src.dep), depth (src.depth) {
  src.dep = 0;
  src.depth = 0;
}

sfpi::impl_::vCond::CC::~CC () { pop (); }

auto sfpi::impl_::vCond::CC::operator= (CC &&src)-> CC & {
  pop ();
  dep = src.dep, depth = src.depth;
  src.dep = 0, src.depth = 0;
  return *this;
}

auto sfpi::impl_::vCond::CC::if_()-> CC & {
  dep = __builtin_rvtt_sfpxvif ();
  return *this;
}
auto sfpi::impl_::vCond::CC::else_()-> CC & {
  __builtin_rvtt_sfpcompc ();
  return *this;    
}

auto sfpi::impl_::vCond::CC::cond (vCond op)-> void {
  __builtin_rvtt_sfpxcondb (op.get (), dep);
}
auto sfpi::impl_::vCond::CC::cond (vInt v)-> void {
  __builtin_rvtt_sfpxcondb (vCond (vCond::NE, v, 0, 0).get (), dep);
}
auto sfpi::impl_::vCond::CC::cond (vUInt v)-> void {
  __builtin_rvtt_sfpxcondb (vCond (vCond::NE, v, 0, 0).get (), dep);
}

auto sfpi::impl_::vCond::CC::push ()-> CC & {
  depth++;
  __builtin_rvtt_sfppushc (SFPPUSHC_MOD1_PUSH);
  return *this;
}

auto sfpi::impl_::vCond::CC::pop ()-> CC & {
  for (unsigned ix = depth; ix--;)
    __builtin_rvtt_sfppopc (SFPPOPC_MOD1_POP);
  return *this;
}

auto sfpi::impl_::operator&& (vCond a, vCond b)-> vCond { return vCond (vCond::And, a, b); }
auto sfpi::impl_::operator|| (vCond a, vCond b)-> vCond { return vCond (vCond::Or, a, b); }
auto sfpi::impl_::operator! (vCond a)-> vCond { return vCond (vCond::Not, a, a); }

//////////////////////////////////////////////////////////////////////////////
template<template<int> typename Derived, int Mod>
void sfpi::impl_::vReg_<Derived, Mod>::operator= (vFloat val) const {
  constexpr int mod = Mod >= 0 ? Mod : SFPSTORE_MOD0_FMT_SRCB;
  static_assert (false
                 || mod == SFPSTORE_MOD0_FMT_SRCB
                 || mod == SFPSTORE_MOD0_FMT_FP32
                 || mod == SFPSTORE_MOD0_FMT_FP16A
                 || mod == SFPSTORE_MOD0_FMT_FP16B
                 , "Mod value not compatible with storing vFloat");
  write (val.get (), mod);
}

template<template<int> typename Derived, int Mod>
void sfpi::impl_::vReg_<Derived, Mod>::operator= (vFloat16a val) const {
  constexpr int mod = Mod >= 0 ? Mod : SFPSTORE_MOD0_FMT_SRCB;
  static_assert (false
                 || mod == SFPSTORE_MOD0_FMT_SRCB
                 || mod == SFPSTORE_MOD0_FMT_FP32
                 || mod == SFPSTORE_MOD0_FMT_FP16A
                 , "Mod value not compatible with storing vFloat");
  write (val.get (), mod);
}

template<template<int> typename Derived, int Mod>
void sfpi::impl_::vReg_<Derived, Mod>::operator= (vFloat16b val) const {
  constexpr int mod = Mod >= 0 ? Mod : SFPSTORE_MOD0_FMT_SRCB;
  static_assert (false
                 || mod == SFPSTORE_MOD0_FMT_SRCB
                 || mod == SFPSTORE_MOD0_FMT_FP32
                 || mod == SFPSTORE_MOD0_FMT_FP16B
                 , "Mod value not compatible with storing vFloat");
  write (val.get (), mod);
}

template<template<int> typename Derived, int Mod>
void sfpi::impl_::vReg_<Derived, Mod>::operator= (float val) const {
  operator= (vFloat (val));
}

template<template<int> typename Derived, int Mod>
sfpi::impl_::vReg_<Derived, Mod>::operator vFloat () const {
  constexpr int mod = Mod >= 0 ? Mod : SFPLOAD_MOD0_FMT_SRCB;
  static_assert (false
                 || mod == SFPLOAD_MOD0_FMT_SRCB
                 || mod == SFPLOAD_MOD0_FMT_FP32
                 || mod == SFPLOAD_MOD0_FMT_FP16A
                 || mod == SFPLOAD_MOD0_FMT_FP16B
                 , "Mod value not compatible with loading vFloat");
  return read (mod);
}

template<template<int> typename Derived, int Mod>
sfpi::impl_::vReg_<Derived, Mod>::operator vFloat16a () const {
  constexpr int mod = Mod >= 0 ? Mod : SFPLOAD_MOD0_FMT_FP16A;
  static_assert (false
                 || mod == SFPLOAD_MOD0_FMT_FP16A
                 , "Mod value not compatible with loading vFloat");
  return vFloat16a (read (mod));
  
}

template<template<int> typename Derived, int Mod>
sfpi::impl_::vReg_<Derived, Mod>::operator vFloat16b () const {
  constexpr int mod = Mod >= 0 ? Mod : SFPLOAD_MOD0_FMT_FP16B;
  static_assert (false
                 || mod == SFPLOAD_MOD0_FMT_FP16B
                 , "Mod value not compatible with loading vFloat");
  return vFloat16b (read (mod));
}

template<template<int> typename Derived, int Mod>
void sfpi::impl_::vReg_<Derived, Mod>::operator= (vInt val) const {
  constexpr int mod = Mod >= 0 ? Mod : SFPSTORE_MOD0_FMT_SM32
#if !__riscv_xtttensixwh  // FIXME, endian convert
                      * 0 + SFPSTORE_MOD0_FMT_INT32
#endif
                      ;
  static_assert (false
                 || mod == SFPSTORE_MOD0_FMT_INT32
                 || mod == SFPSTORE_MOD0_FMT_SM32
                 , "Mod value not compatible with storing vInt");
  auto tmp = val.get ();
#if !__riscv_xtttensixwh
  if constexpr (mod == SFPSTORE_MOD0_FMT_SM32) {
#if 0 // FIXME address this later
    tmp = __builtin_rvtt_sfpcast (tmp, SFPCAST_MOD1_INT32_TO_SM32);
#if __riscv_xtttensixbh
    // BH erratum, 2C(MOSTNEG) -> SM(1:0), clamp to SM(MOSTNEG)
    v_if (vInt (tmp) == int32_t (0x8000000)) {
      tmp = vInt (-1).get ();
    } v_endif;
#endif
#endif
  }
#endif

  write (tmp, mod
#if !__riscv_xtttensixwh
         * 0 + SFPSTORE_MOD0_FMT_INT32
#endif
         );
}

template<template<int> typename Derived, int Mod>
sfpi::impl_::vReg_<Derived, Mod>::operator vInt () const {
  constexpr int mod = Mod >= 0 ? Mod : SFPLOAD_MOD0_FMT_SM32
#if !__riscv_xtttensixwh // FIXME endian convert
                      * 0 + SFPLOAD_MOD0_FMT_INT32
#endif
                      ;
  static_assert (false
                 || mod == SFPLOAD_MOD0_FMT_INT32
                 || mod == SFPLOAD_MOD0_FMT_SM32
                 , "Mod value not compatible with storing vInt");
  auto val = read (mod
#if !__riscv_xtttensixwh
                   * 0 + SFPLOAD_MOD0_FMT_INT32
#endif
                   );
#if !__riscv_xtttensixwh
#if 0 // FIXME: Address later
  if constexpr (mod == SFPLOAD_MOD0_FMT_SM32) {
    val = __builtin_rvtt_sfpcast (val.get (), SFPCAST_MOD1_SM32_TO_INT32);
#if __riscv_xtttensixbh
    // BH erratum, SM(1:0) -> 2C(MOSTNEG), not zero
    v_if (vInt (val) == int32_t (0x8000000)) {
      val = vInt (0).get ();
    } v_endif;
#endif
  }
#endif
#endif
  return val;
}

template<template<int> typename Derived, int Mod>
void sfpi::impl_::vReg_<Derived, Mod>::operator= (vUInt val) const {
  constexpr int mod = Mod >= 0 ? Mod : SFPSTORE_MOD0_FMT_INT32;
  static_assert (false
                 || mod == SFPSTORE_MOD0_FMT_INT32
                 || mod == SFPSTORE_MOD0_FMT_UINT16
                 || mod == SFPSTORE_MOD0_FMT_LO16_ONLY
                 || mod == SFPSTORE_MOD0_FMT_HI16_ONLY
                 || mod == SFPSTORE_MOD0_FMT_LO16
                 || mod == SFPSTORE_MOD0_FMT_HI16
                 , "Mod value not compatible with storing vUInt");
  write (val.get (), mod);
}

template<template<int> typename Derived, int Mod>
void sfpi::impl_::vReg_<Derived, Mod>::operator= (vUInt16 val) const {
  constexpr int mod = Mod >= 0 ? Mod : SFPSTORE_MOD0_FMT_UINT16;
  static_assert (false
                 || mod == SFPSTORE_MOD0_FMT_INT32
                 || mod == SFPSTORE_MOD0_FMT_UINT16
                 || mod == SFPSTORE_MOD0_FMT_LO16_ONLY
                 || mod == SFPSTORE_MOD0_FMT_HI16_ONLY
                 || mod == SFPSTORE_MOD0_FMT_LO16
                 || mod == SFPSTORE_MOD0_FMT_HI16
                 , "Mod value not compatible with storing vUInt");
  write (val.get (), mod);
}

template<template<int> typename Derived, int Mod>
sfpi::impl_::vReg_<Derived, Mod>::operator vUInt () const {
  constexpr int mod = Mod >= 0 ? Mod : SFPLOAD_MOD0_FMT_INT32;
  static_assert (false
                 || mod == SFPLOAD_MOD0_FMT_INT32
                 || mod == SFPLOAD_MOD0_FMT_UINT16
                 || mod == SFPLOAD_MOD0_FMT_LO16
                 || mod == SFPLOAD_MOD0_FMT_HI16
                 , "Mod value not compatible with loading vUInt");
  return read (mod);
}

template<template<int> typename Derived, int Mod>
sfpi::impl_::vReg_<Derived, Mod>::operator vUInt16 () const {
  constexpr int mod = Mod >= 0 ? Mod : SFPLOAD_MOD0_FMT_UINT16;
  static_assert (false
                 || mod == SFPLOAD_MOD0_FMT_UINT16
                 || mod == SFPLOAD_MOD0_FMT_LO16
                 , "Mod value not compatible with loading vUInt");
  return vUInt16 (read (mod));
}

template<template<int> typename Derived, int Mod>
void sfpi::impl_::vReg_<Derived, Mod>::operator= (vSMag val) const {
  constexpr int mod = Mod >= 0 ? Mod : SFPSTORE_MOD0_FMT_INT32;
  static_assert (false
                 || mod == SFPSTORE_MOD0_FMT_INT32
                 || mod == SFPSTORE_MOD0_FMT_INT16
                 , "Mod value not compatible with storing vUInt");
  write (val.get (), mod);
}

template<template<int> typename Derived, int Mod>
void sfpi::impl_::vReg_<Derived, Mod>::operator= (vSMag16 val) const {
  constexpr int mod = Mod >= 0 ? Mod : SFPSTORE_MOD0_FMT_INT16;
  static_assert (false
                 || mod == SFPSTORE_MOD0_FMT_INT16
                 , "Mod value not compatible with storing vUInt");
  write (val.get (), mod);
}

template<template<int> typename Derived, int Mod>
sfpi::impl_::vReg_<Derived, Mod>::operator vSMag () const {
  constexpr int mod = Mod >= 0 ? Mod : SFPLOAD_MOD0_FMT_INT32;
  static_assert (false
                 || mod == SFPLOAD_MOD0_FMT_INT32
                 || mod == SFPLOAD_MOD0_FMT_INT16
                 , "Mod value not compatible with loading vUInt");
  return vSMag (read (mod));
}

template<template<int> typename Derived, int Mod>
sfpi::impl_::vReg_<Derived, Mod>::operator vSMag16 () const {
  constexpr int mod = Mod >= 0 ? Mod : SFPLOAD_MOD0_FMT_INT16;
  static_assert (false
                 || mod == SFPLOAD_MOD0_FMT_INT16
                 , "Mod value not compatible with loading vUInt");
  return vSMag16 (read (mod));
}

template<int Mod>
auto sfpi::impl_::DstRegFile::vReg<Mod>::operator= (vReg<Mod> const &reg) const-> void {
  *this = vFloat (*reg);
}
template<int Mod>
auto sfpi::impl_::DstRegFile::vReg<Mod>::operator- () const-> vFloat {
  return -vFloat (*this);
}

//////////////////////////////////////////////////////////////////////////////
// vFloat definitions
sfpi::vFloat::vFloat (impl_::sfpu_t vec) : vVal (vec) {}
sfpi::vFloat::vFloat (sFloat16a val)
    : vVal (__builtin_rvtt_sfploadi (val.get (), SFPLOADI_MOD0_FLOATA)) {}
sfpi::vFloat::vFloat (sFloat16b val)
    : vVal (__builtin_rvtt_sfploadi (val.get (), SFPLOADI_MOD0_FLOATB)) {}
sfpi::vFloat::vFloat (float f)
    : vVal (__builtin_rvtt_sfpxloadi (impl_::float_as_uint (f), -32)) {}

auto sfpi::vFloat::operator+= (vFloat a)-> vFloat & { return *this = *this + a; }
auto sfpi::vFloat::operator-= (vFloat a)-> vFloat & {
#if __riscv_xtttensixwh
  vFloat neg1 = vConstNeg1;
  *this = __builtin_rvtt_sfpmad (neg1.get (), a.get(), get (), SFPMAD_MOD1_OFFSET_NONE);
#else // __riscv_xtttensixbh || __riscv_xtttensixqsr
  *this = *this + -a;
#endif
  return *this;
}
auto sfpi::vFloat::operator*= (vFloat m)-> vFloat & { return *this = *this * m; }

auto sfpi::vFloat::operator++ (int)-> vFloat { *this += 1; return *this; }
auto sfpi::vFloat::operator++ ()-> vFloat { vFloat tmp = *this; *this += 1; return tmp; }
auto sfpi::vFloat::operator-- (int)-> vFloat { *this -= 1; return *this; }
auto sfpi::vFloat::operator-- ()-> vFloat { vFloat tmp = *this; *this -= 1; return tmp; }

auto sfpi::operator+ (vFloat a)-> vFloat { return a; }
auto sfpi::operator+ (vFloat a, vFloat b)-> vFloat { return a.flt_add (b); }
auto sfpi::operator+ (float a, vFloat b)-> vFloat { return b + a; }
auto sfpi::operator- (vFloat a)-> vFloat {
  return __builtin_rvtt_sfpmov (a.get (), SFPMOV_MOD1_COMPSIGN);
}
auto sfpi::operator- (vFloat a, vFloat b)-> vFloat {
  // Do not use sfpmad here, the optimizer will handle that.
  return a.flt_add (-b);
}
auto sfpi::operator- (vFloat a, float b)-> vFloat { return a - vFloat (b); }
auto sfpi::operator- (float a, vFloat b)-> vFloat { return vFloat (a) - b; }
auto sfpi::operator* (vFloat a, vFloat b)-> vFloat { return a.flt_mul (b); }
auto sfpi::operator* (float a, vFloat b)-> vFloat { return b * a; }

//////////////////////////////////////////////////////////////////////////////
// vInt definitions
sfpi::vInt::vInt (impl_::sfpu_t vec) : vVal (vec) {}
sfpi::vInt::vInt (vMag val) : vInt (val.get ()) {}
sfpi::vInt::vInt (vUInt val) : vVal (val.get ()) {}
sfpi::vInt::vInt (vUInt16 val) : vInt (vUInt (val)) {}
sfpi::vInt::vInt (int16_t val)
    : vVal (__builtin_rvtt_sfploadi (val, SFPLOADI_MOD0_SHORT)) {}
sfpi::vInt::vInt (uint16_t val)
    : vVal (__builtin_rvtt_sfploadi (val, SFPLOADI_MOD0_USHORT)) {}
sfpi::vInt::vInt (int32_t val)
    : vVal (__builtin_rvtt_sfpxloadi (val, 31)) {}
sfpi::vInt::vInt (uint32_t val)
    : vVal (__builtin_rvtt_sfpxloadi (val, -32)) {}
sfpi::vInt::vInt (int val) : vInt (int32_t (val)) {};
sfpi::vInt::vInt (unsigned val) : vInt (uint32_t (val)) {}
sfpi::vInt::vInt(const impl_::vCond vc)
    : vVal (__builtin_rvtt_sfpxcondi (vc.get ())) {}

auto sfpi::vInt::operator+= (vInt a)-> vInt & { return *this = *this + a; }
auto sfpi::vInt::operator-= (vInt a)-> vInt & { return *this = *this - a; }
auto sfpi::vInt::operator<<= (unsigned a)-> vInt & { return *this = *this << a; }
auto sfpi::vInt::operator<<= (vInt a)-> vInt & { return *this = *this << a; }
#if __riscv_xtttensixbh || __riscv_xtttensixqsr
auto sfpi::vInt::operator>>= (unsigned a)-> vInt & { return *this = *this >> a; }
auto sfpi::vInt::operator>>= (vInt a)-> vInt & { return *this = *this >> a; }
#endif
auto sfpi::vInt::operator&= (vInt a)-> vInt & { return *this = *this & a; }
auto sfpi::vInt::operator|= (vInt a)-> vInt & { return *this = *this | a; }
auto sfpi::vInt::operator^= (vInt a)-> vInt & { return *this = *this ^ a; }

auto sfpi::vInt::operator++ (int)-> vInt { *this += 1; return *this; }
auto sfpi::vInt::operator++ ()-> vInt { vInt tmp = *this; *this += 1; return tmp; }
auto sfpi::vInt::operator-- (int)-> vInt { *this -= 1; return *this; }
auto sfpi::vInt::operator-- ()-> vInt { vInt tmp = *this; *this -= 1; return tmp; }

auto sfpi::operator+ (vInt a)-> vInt { return a; }
auto sfpi::operator+ (vInt a, vInt b)-> vInt { return a.int_add (b, true); }
auto sfpi::operator+ (vInt a, int32_t b)-> vInt { return a.int_add (b, true); }
auto sfpi::operator- (vInt a)-> vInt { return vInt (0) - a; }
auto sfpi::operator- (vInt a, vInt b)-> vInt { return a.int_sub (b, true); }
auto sfpi::operator- (vInt a, int32_t b)-> vInt { return a.int_sub (b, true); }
auto sfpi::operator<< (vInt vec, unsigned amt)-> vInt { return vec.int_shift (amt, true); }
auto sfpi::operator<< (vInt vec, vUInt amt)-> vInt { return vec.int_shift (amt, true); }
#if __riscv_xtttensixbh || __riscv_xtttensixqsr
auto sfpi::operator>> (vInt vec, unsigned amt)-> vInt { return vec.int_shift (-amt, true); }
auto sfpi::operator>> (vInt vec, vUInt amt)-> vInt { return vec.int_shift (-amt, true); }
#endif
auto sfpi::operator~ (vInt a)-> vInt { return a.int_not (); }
auto sfpi::operator& (vInt a, vInt b)-> vInt { return a.int_and (b); }
auto sfpi::operator& (vInt a, int32_t b)-> vInt { return a & vInt (b); }
auto sfpi::operator& (vInt a, int b)-> vInt { return a & int32_t (b); }
auto sfpi::operator& (vInt a, unsigned b)-> vInt { return a & int32_t (b); }
auto sfpi::operator| (vInt a, vInt b)-> vInt { return a.int_or (b); }
auto sfpi::operator| (vInt a, int32_t b)-> vInt { return a | vInt (b); }
auto sfpi::operator| (vInt a, int b)-> vInt { return a | int32_t (b); }
auto sfpi::operator| (vInt a, unsigned b)-> vInt { return a | int32_t (b); }
auto sfpi::operator^ (vInt a, vInt b)-> vInt { return a.int_xor (b); }
auto sfpi::operator^ (vInt a, int32_t b)-> vInt { return a ^ vInt (b); }
auto sfpi::operator^ (vInt a, int b)-> vInt { return a ^ int32_t (b); }
auto sfpi::operator^ (vInt a, unsigned b)-> vInt { return a ^ int32_t (b); }

//////////////////////////////////////////////////////////////////////////////
// vUInt definitions
sfpi::vUInt::vUInt (impl_::sfpu_t vec) : vVal (vec) {}
sfpi::vUInt::vUInt (vMag val) : vUInt (val.get ()) {}
sfpi::vUInt::vUInt (vInt val) : vVal (val.get ()) {}
sfpi::vUInt::vUInt (int16_t val)
    : vVal (__builtin_rvtt_sfploadi (val, SFPLOADI_MOD0_SHORT)) {}
sfpi::vUInt::vUInt (uint16_t val)
    : vVal (__builtin_rvtt_sfploadi (val, SFPLOADI_MOD0_USHORT)) {}
sfpi::vUInt::vUInt (int32_t val)
    : vVal (__builtin_rvtt_sfpxloadi (val, 31)) {}
sfpi::vUInt::vUInt (uint32_t val)
    : vVal (__builtin_rvtt_sfpxloadi (val, -32)) {}
sfpi::vUInt::vUInt (int val) : vUInt (int32_t (val)) {}
sfpi::vUInt::vUInt (unsigned val) : vUInt (uint32_t (val)) {}

sfpi::vUInt::vUInt(const impl_::vCond vc)
    : vVal (__builtin_rvtt_sfpxcondi (vc.get ())) {}

auto sfpi::vUInt::operator+= (vUInt a)-> vUInt & { return *this = *this + a; }
auto sfpi::vUInt::operator-= (vUInt a)-> vUInt & { return *this = *this - a; }
auto sfpi::vUInt::operator<<= (unsigned a)-> vUInt & { return *this = *this << a; }
auto sfpi::vUInt::operator<<= (vUInt a)-> vUInt & { return *this = *this << a; }
auto sfpi::vUInt::operator>>= (unsigned a)-> vUInt & { return *this = *this >> a; }
auto sfpi::vUInt::operator>>= (vUInt a)-> vUInt & { return *this = *this >> a; }
auto sfpi::vUInt::operator&= (vUInt a)-> vUInt & { return *this = *this & a; }
auto sfpi::vUInt::operator|= (vUInt a)-> vUInt & { return *this = *this | a; }
auto sfpi::vUInt::operator^= (vUInt a)-> vUInt & { return *this = *this ^ a; }

auto sfpi::vUInt::operator++ (int)-> vUInt { *this += 1; return *this; }
auto sfpi::vUInt::operator++ ()-> vUInt { vUInt tmp = *this; *this += 1; return tmp; }
auto sfpi::vUInt::operator-- (int)-> vUInt { *this -= 1; return *this; }
auto sfpi::vUInt::operator-- ()-> vUInt { vUInt tmp = *this; *this -= 1; return tmp; }

auto sfpi::operator+ (vUInt a)-> vUInt { return a; }
auto sfpi::operator+ (vUInt a, vUInt b)-> vUInt { return a.int_add (b, false); }
auto sfpi::operator+ (vUInt a, int32_t b)-> vUInt { return a.int_add (b, false); }
auto sfpi::operator- (vUInt a)-> vUInt { return vUInt (0) - a; }
auto sfpi::operator- (vUInt a, vUInt b)-> vUInt { return a.int_sub (b, false); }
auto sfpi::operator- (vUInt a, vInt b)-> vUInt { return a.int_sub (b, false); }
auto sfpi::operator- (vUInt a, int32_t b)-> vUInt { return a.int_sub (b, false); }
auto sfpi::operator<< (vUInt a, unsigned b)-> vUInt { return a.int_shift (b, false); }
auto sfpi::operator<< (vUInt a, vUInt b)-> vUInt { return a.int_shift (b, false); }
auto sfpi::operator>> (vUInt a, unsigned b)-> vUInt { return a.int_shift (-b, false); }
auto sfpi::operator>> (vUInt a, vUInt b)-> vUInt { return a.int_shift (-b, false); }
auto sfpi::operator~ (vUInt a)-> vUInt { return a.int_not (); }
auto sfpi::operator& (vUInt a, vUInt b)-> vUInt { return a.int_and (b); }
auto sfpi::operator& (vUInt a, vInt b)-> vUInt { return a & vUInt (b); }
auto sfpi::operator& (vInt a, vUInt b)-> vUInt { return vUInt (a) & b; }
auto sfpi::operator& (vUInt a, uint32_t b)-> vUInt { return a & vUInt (b); }
auto sfpi::operator& (vUInt a, unsigned b)-> vUInt { return a & uint32_t (b); }
auto sfpi::operator& (vUInt a, int b)-> vUInt { return a & uint32_t (b); }
auto sfpi::operator| (vUInt a, vUInt b)-> vUInt { return a.int_or (b); }
auto sfpi::operator| (vUInt a, vInt b)-> vUInt { return a | vUInt (b); }
auto sfpi::operator| (vInt a, vUInt b)-> vUInt { return vUInt (a) | b; }
auto sfpi::operator| (vUInt a, uint32_t b)-> vUInt { return a | vUInt (b); }
auto sfpi::operator| (vUInt a, unsigned b)-> vUInt { return a | uint32_t (b); }
auto sfpi::operator| (vUInt a, int b)-> vUInt { return a | uint32_t (b); }
auto sfpi::operator^ (vUInt a, vUInt b)-> vUInt { return a.int_xor (b); }
auto sfpi::operator^ (vUInt a, vInt b)-> vUInt { return a ^ vUInt (b); }
auto sfpi::operator^ (vInt a, vUInt b)-> vUInt { return vUInt (a) ^ b; }
auto sfpi::operator^ (vUInt a, uint32_t b)-> vUInt { return a ^ vUInt (b); }
auto sfpi::operator^ (vUInt a, unsigned b)-> vUInt { return a ^ uint32_t (b); }
auto sfpi::operator^ (vUInt a, int b)-> vUInt { return a ^ uint32_t (b); }

//////////////////////////////////////////////////////////////////////////////
// vSMag definitions
sfpi::vSMag::vSMag (impl_::sfpu_t vec) : vVal (vec) {}
