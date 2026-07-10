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

sfpi::sFloat16b::sFloat16b (float v)
    : val (uint16_t (impl_::float_as_uint (v) >> 16)) {}

//////////////////////////////////////////////////////////////////////////////
// vBool definitions

enum sfpi::vBool::BoolOp : unsigned char {
      Or = SFPXBOOL_MOD1_OR,
     And = SFPXBOOL_MOD1_AND,
     Not = SFPXBOOL_MOD1_NOT,
};

enum sfpi::vBool::CondOp : unsigned char {
      LT = SFPXCMP_MOD1_CC_LT,
      NE = SFPXCMP_MOD1_CC_NE,
     GTE = SFPXCMP_MOD1_CC_GTE,
      EQ = SFPXCMP_MOD1_CC_EQ,
     LTE = SFPXCMP_MOD1_CC_LTE,
      GT = SFPXCMP_MOD1_CC_GT,
};

sfpi::vBool::vBool (BoolOp t, vBool a, vBool b) {
  result = __builtin_rvtt_sfpxbool (t, a.get (), b.get ());
}
sfpi::vBool::vBool (CondOp t, vFloat a, float b) {
  result = __builtin_rvtt_sfpxfcmps (a.get (), impl_::float_as_uint (b), t | SFPXSCMP_MOD1_FMT_FLOAT);
}
sfpi::vBool::vBool (CondOp t, vFloat a, vFloat b) {
  result = __builtin_rvtt_sfpxfcmpv (a.get (), b.get (), t);
}
sfpi::vBool::vBool (CondOp t, vInt a, int32_t b, unsigned mod) {
  result = __builtin_rvtt_sfpxicmps (a.get (), b, mod | t);
}
sfpi::vBool::vBool (CondOp t, vInt a, vInt b, unsigned mod) {
  result = __builtin_rvtt_sfpxicmpv (a.get (), b.get (), mod | t);
}
sfpi::vBool::vBool (CondOp t, vUInt a, uint32_t b, unsigned mod) {
  result = __builtin_rvtt_sfpxicmps (a.get (), b, mod | t);
}
sfpi::vBool::vBool (CondOp t, vUInt a, vUInt b, unsigned mod) {
  result = __builtin_rvtt_sfpxicmpv (a.get (), b.get (), mod | t);
}
sfpi::vBool::vBool (CondOp t, vSMag a, int b, unsigned mod) {
  result = __builtin_rvtt_sfpxicmps
               (a.get (),
                b < 0 ? 0 - (unsigned (b) << 1 >> 1): unsigned (b),
                mod | t);
}
sfpi::vBool::vBool (CondOp t, vSMag a, vSMag b, unsigned mod) {
  result = __builtin_rvtt_sfpxicmpv (a.get (), b.get (), mod | t);
}
sfpi::vBool::vBool (vInt a) {
  result = __builtin_rvtt_sfpxicmps (a.get(), 0, NE);
}
sfpi::vBool::vBool (vUInt a) {
  result = __builtin_rvtt_sfpxicmps (a.get(), 0, NE);
}
sfpi::vBool::vBool (vSMag a) {
  result = __builtin_rvtt_sfpxicmps (a.get(), 0, NE);
}

sfpi::vBool::operator vInt () const {
  return vInt (__builtin_rvtt_sfpxcondi (get ()));
}

sfpi::impl_::CC::CC (CC &&src)
    : dep (src.dep), depth (src.depth) {
  src.dep = 0;
  src.depth = 0;
}

sfpi::impl_::CC::~CC () { pop (); }

auto sfpi::impl_::CC::operator= (CC &&src)-> CC & {
  pop ();
  dep = src.dep, depth = src.depth;
  src.dep = 0, src.depth = 0;
  return *this;
}

auto sfpi::impl_::CC::if_()-> CC & {
  dep = __builtin_rvtt_sfpxvif ();
  return *this;
}
auto sfpi::impl_::CC::else_()-> CC & {
  __builtin_rvtt_sfpcompc ();
  return *this;    
}

auto sfpi::impl_::CC::cond (vBool op)-> void {
  __builtin_rvtt_sfpxcondb (op.get (), dep);
}
auto sfpi::impl_::CC::cond (vInt v)-> void {
  __builtin_rvtt_sfpxcondb (vBool (vBool::NE, v, 0, 0).get (), dep);
}
auto sfpi::impl_::CC::cond (vUInt v)-> void {
  __builtin_rvtt_sfpxcondb (vBool (vBool::NE, v, 0, 0).get (), dep);
}

auto sfpi::impl_::CC::push ()-> CC & {
  depth++;
  __builtin_rvtt_sfppushc (SFPPUSHC_MOD1_PUSH);
  return *this;
}

auto sfpi::impl_::CC::pop ()-> CC & {
  for (unsigned ix = depth; ix--;)
    __builtin_rvtt_sfppopc (SFPPOPC_MOD1_POP);
  return *this;
}

auto sfpi::operator&& (vBool a, vBool b)-> vBool { return vBool (vBool::And, a, b); }
auto sfpi::operator|| (vBool a, vBool b)-> vBool { return vBool (vBool::Or, a, b); }
auto sfpi::operator! (vBool a)-> vBool { return vBool (vBool::Not, a, a); }

//////////////////////////////////////////////////////////////////////////////
template<template<sfpi::DataLayout> typename Derived, sfpi::DataLayout Fmt>
void sfpi::impl_::vReg_<Derived, Fmt>::operator= (vFloat val) const {
  constexpr DataLayout fmt = Fmt != DataLayout::Default ? Fmt : DataLayout::FSrcB;
  static_assert (false
                 || fmt == DataLayout::FSrcB
                 || fmt == DataLayout::F32
                 || fmt == DataLayout::F16a
                 || fmt == DataLayout::F16b
                 , "Fmt value not compatible with storing vFloat");
  write (val.get (),
         fmt == DataLayout::FSrcB ? SFPSTORE_MOD0_FMT_SRCB :
         fmt == DataLayout::F32 ? SFPSTORE_MOD0_FMT_FP32 :
         fmt == DataLayout::F16a ? SFPSTORE_MOD0_FMT_FP16A :
         fmt == DataLayout::F16b ? SFPSTORE_MOD0_FMT_FP16B :
         ~0);
}

template<template<sfpi::DataLayout> typename Derived, sfpi::DataLayout Fmt>
void sfpi::impl_::vReg_<Derived, Fmt>::operator= (vFloat16a val) const {
  constexpr DataLayout fmt = Fmt != DataLayout::Default ? Fmt : DataLayout::F16a;
  static_assert (false
                 || fmt == DataLayout::FSrcB
                 || fmt == DataLayout::F32
                 || fmt == DataLayout::F16a
                 , "Fmt value not compatible with storing vFloat16a");
  write (val.get (),
         fmt == DataLayout::FSrcB ? SFPSTORE_MOD0_FMT_SRCB :
         fmt == DataLayout::F32 ? SFPSTORE_MOD0_FMT_FP32 :
         fmt == DataLayout::F16a ? SFPSTORE_MOD0_FMT_FP16A :
         ~0);
}

template<template<sfpi::DataLayout> typename Derived, sfpi::DataLayout Fmt>
void sfpi::impl_::vReg_<Derived, Fmt>::operator= (vFloat16b val) const {
  constexpr DataLayout fmt = Fmt != DataLayout::Default ? Fmt : DataLayout::F16b;
  static_assert (false
                 || fmt == DataLayout::FSrcB
                 || fmt == DataLayout::F32
                 || fmt == DataLayout::F16b
                 , "Fmt value not compatible with storing vFloat16b");
  write (val.get (),
         fmt == DataLayout::FSrcB ? SFPSTORE_MOD0_FMT_SRCB :
         fmt == DataLayout::F32 ? SFPSTORE_MOD0_FMT_FP32 :
         fmt == DataLayout::F16b ? SFPSTORE_MOD0_FMT_FP16B :
         ~0);
}

template<template<sfpi::DataLayout> typename Derived, sfpi::DataLayout Fmt>
void sfpi::impl_::vReg_<Derived, Fmt>::operator= (float val) const {
  operator= (vFloat (val));
}

template<template<sfpi::DataLayout> typename Derived, sfpi::DataLayout Fmt>
sfpi::impl_::vReg_<Derived, Fmt>::operator vFloat () const {
  constexpr DataLayout fmt = Fmt != DataLayout::Default ? Fmt : DataLayout::FSrcB;
  static_assert (false
                 || fmt == DataLayout::FSrcB
                 || fmt == DataLayout::F32
                 || fmt == DataLayout::F16a
                 || fmt == DataLayout::F16b
                 , "Fmt value not compatible with loading vFloat");
  auto tmp = read (fmt == DataLayout::FSrcB ? SFPLOAD_MOD0_FMT_SRCB :
                   fmt == DataLayout::F32 ? SFPLOAD_MOD0_FMT_FP32 :
                   fmt == DataLayout::F16a ? SFPLOAD_MOD0_FMT_FP16A :
                   fmt == DataLayout::F16b ? SFPLOAD_MOD0_FMT_FP16B :
                   ~0);
  return vFloat (tmp);
}

template<template<sfpi::DataLayout> typename Derived, sfpi::DataLayout Fmt>
sfpi::impl_::vReg_<Derived, Fmt>::operator vFloat16a () const {
  constexpr DataLayout fmt = Fmt != DataLayout::Default ? Fmt : DataLayout::F16a;
  static_assert (false
                 || fmt == DataLayout::FSrcB
                 || fmt == DataLayout::F32
                 || fmt == DataLayout::F16a
                 , "Fmt value not compatible with loading vFloat16a");
  auto tmp = read (fmt == DataLayout::FSrcB ? SFPLOAD_MOD0_FMT_SRCB :
                   fmt == DataLayout::F32 ? SFPLOAD_MOD0_FMT_FP32 :
                   fmt == DataLayout::F16a ? SFPLOAD_MOD0_FMT_FP16A :
                   ~0);
  return vFloat16a (tmp);
}

template<template<sfpi::DataLayout> typename Derived, sfpi::DataLayout Fmt>
sfpi::impl_::vReg_<Derived, Fmt>::operator vFloat16b () const {
  constexpr DataLayout fmt = Fmt != DataLayout::Default ? Fmt : DataLayout::F16b;
  static_assert (false
                 || fmt == DataLayout::FSrcB
                 || fmt == DataLayout::F32
                 || fmt == DataLayout::F16b
                 , "Fmt value not compatible with loading vFloat16b");
  auto tmp = read (fmt == DataLayout::FSrcB ? SFPLOAD_MOD0_FMT_SRCB :
                   fmt == DataLayout::F32 ? SFPLOAD_MOD0_FMT_FP32 :
                   fmt == DataLayout::F16b ? SFPLOAD_MOD0_FMT_FP16B :
                   ~0);
  return vFloat16b (tmp);
}

template<template<sfpi::DataLayout> typename Derived, sfpi::DataLayout Fmt>
void sfpi::impl_::vReg_<Derived, Fmt>::operator= (vInt val) const {
  constexpr DataLayout fmt = Fmt != DataLayout::Default ? Fmt :
#if __riscv_xtttensixwh
                                 // WH defaults to SM for int32
                                 DataLayout::SM32
#else
                                 DataLayout::I32
#endif
                                 ;
  static_assert (false
                 || fmt == DataLayout::I32
                 || fmt == DataLayout::SM32
                 , "Fmt value not compatible with storing vInt");
  auto tmp =
#if !__riscv_xtttensixwh
      fmt == DataLayout::SM32 ? int_to_smag (val).get () :
#endif
      val.get ();

  write (tmp,
         fmt == DataLayout::I32 ? SFPLOAD_MOD0_FMT_INT32 :
         fmt == DataLayout::SM32 ?
#if !__riscv_xtttensixwh
         SFPLOAD_MOD0_FMT_INT32 :
#else
         SFPLOAD_MOD0_FMT_SM32 :
#endif
         ~0);
}

template<template<sfpi::DataLayout> typename Derived, sfpi::DataLayout Fmt>
sfpi::impl_::vReg_<Derived, Fmt>::operator vInt () const {
  constexpr DataLayout fmt = Fmt != DataLayout::Default ? Fmt :
#if __riscv_xtttensixwh
                                 // WH defaults to SM for int32
                                 DataLayout::SM32
#else
                                 DataLayout::I32
#endif
                                 ;
  static_assert (false
                 || fmt == DataLayout::I32
                 || fmt == DataLayout::SM32
                 , "Fmt value not compatible with storing vInt");
  auto tmp = read (fmt == DataLayout::I32 ? SFPLOAD_MOD0_FMT_INT32 :
                   fmt == DataLayout::SM32 ?
#if !__riscv_xtttensixwh
                   SFPLOAD_MOD0_FMT_INT32 :
#else
                   SFPLOAD_MOD0_FMT_SM32 :
#endif
                   ~0);

#if !__riscv_xtttensixwh
  if (fmt == DataLayout::SM32)
    tmp = smag_to_int (vSMag (tmp)).get ();
#endif
  return vInt (tmp);
}

template<template<sfpi::DataLayout> typename Derived, sfpi::DataLayout Fmt>
void sfpi::impl_::vReg_<Derived, Fmt>::operator= (vUInt val) const {
  constexpr DataLayout fmt = Fmt != DataLayout::Default ? Fmt : DataLayout::U32;
  static_assert (false
                 || fmt == DataLayout::U32
                 || fmt == DataLayout::U16
                 || fmt == DataLayout::LO16
                 || fmt == DataLayout::HI16
                 , "Fmt value not compatible with storing vUInt");
  write (val.get (),
         fmt == DataLayout::U32 ? SFPSTORE_MOD0_FMT_INT32 :
         fmt == DataLayout::U16 ? SFPSTORE_MOD0_FMT_UINT16 :
         fmt == DataLayout::LO16 ? SFPSTORE_MOD0_FMT_LO16 :
         fmt == DataLayout::HI16 ? SFPSTORE_MOD0_FMT_HI16 :
         ~0);
}

template<template<sfpi::DataLayout> typename Derived, sfpi::DataLayout Fmt>
void sfpi::impl_::vReg_<Derived, Fmt>::operator= (vMag val) const {
  constexpr DataLayout fmt = Fmt != DataLayout::Default ? Fmt : DataLayout::M32;
  static_assert (false
                 || fmt == DataLayout::M32
                 || fmt == DataLayout::LO16
                 || fmt == DataLayout::HI16
                 , "Fmt value not compatible with storing vMag");
  write (val.get (),
         fmt == DataLayout::M32 ? SFPSTORE_MOD0_FMT_INT32 :
         fmt == DataLayout::LO16 ? SFPSTORE_MOD0_FMT_LO16 :
         fmt == DataLayout::HI16 ? SFPSTORE_MOD0_FMT_HI16 :
         ~0);
}

template<template<sfpi::DataLayout> typename Derived, sfpi::DataLayout Fmt>
void sfpi::impl_::vReg_<Derived, Fmt>::operator= (vUInt16 val) const {
  constexpr DataLayout fmt = Fmt != DataLayout::Default ? Fmt : DataLayout::U16;
  static_assert (false
                 || fmt == DataLayout::U32
                 || fmt == DataLayout::U16
                 || fmt == DataLayout::LO16
                 || fmt == DataLayout::HI16
                 , "Fmt value not compatible with storing vUInt16");
  write (val.get (),
         fmt == DataLayout::U32 ? SFPSTORE_MOD0_FMT_INT32 :
         fmt == DataLayout::U16 ? SFPSTORE_MOD0_FMT_UINT16 :
         fmt == DataLayout::LO16 ? SFPSTORE_MOD0_FMT_LO16 :
         fmt == DataLayout::HI16 ? SFPSTORE_MOD0_FMT_HI16 :
         ~0);
}

template<template<sfpi::DataLayout> typename Derived, sfpi::DataLayout Fmt>
sfpi::impl_::vReg_<Derived, Fmt>::operator vUInt () const {
  constexpr DataLayout fmt = Fmt != DataLayout::Default ? Fmt : DataLayout::U32;
  static_assert (false
                 || fmt == DataLayout::U32
                 || fmt == DataLayout::U16
                 || fmt == DataLayout::LO16
                 || fmt == DataLayout::HI16
                 , "Fmt value not compatible with loading vUInt");
  auto tmp = read (fmt == DataLayout::U32 ? SFPLOAD_MOD0_FMT_INT32 :
                   fmt == DataLayout::U16 ? SFPLOAD_MOD0_FMT_UINT16 :
                   fmt == DataLayout::LO16 ? SFPLOAD_MOD0_FMT_LO16 :
                   fmt == DataLayout::HI16 ? SFPLOAD_MOD0_FMT_HI16 :
                   ~0);
  return vUInt (tmp);
}

template<template<sfpi::DataLayout> typename Derived, sfpi::DataLayout Fmt>
sfpi::impl_::vReg_<Derived, Fmt>::operator vMag () const {
  constexpr DataLayout fmt = Fmt != DataLayout::Default ? Fmt : DataLayout::M32;
  static_assert (false
                 || fmt == DataLayout::M32
                 || fmt == DataLayout::LO16
                 , "Fmt value not compatible with loading vMag");
  auto tmp = read (fmt == DataLayout::M32 ? SFPLOAD_MOD0_FMT_INT32 :
                   fmt == DataLayout::LO16 ? SFPLOAD_MOD0_FMT_LO16 :
                   ~0);
  return vMag (tmp);
}

template<template<sfpi::DataLayout> typename Derived, sfpi::DataLayout Fmt>
sfpi::impl_::vReg_<Derived, Fmt>::operator vUInt16 () const {
  constexpr DataLayout fmt = Fmt != DataLayout::Default ? Fmt : DataLayout::U16;
  static_assert (false
                 || fmt == DataLayout::U16
                 || fmt == DataLayout::LO16
                 , "Fmt value not compatible with loading vUInt16");
  return vUInt16 (read (
                      fmt == DataLayout::U16 ? SFPLOAD_MOD0_FMT_UINT16 :
                      fmt == DataLayout::LO16 ? SFPLOAD_MOD0_FMT_LO16 :
                      ~0));
}

template<template<sfpi::DataLayout> typename Derived, sfpi::DataLayout Fmt>
void sfpi::impl_::vReg_<Derived, Fmt>::operator= (vSMag val) const {
  constexpr DataLayout fmt = Fmt != DataLayout::Default ? Fmt : DataLayout::SM32;
  static_assert (false
                 || fmt == DataLayout::I32
                 || fmt == DataLayout::SM32
                 || fmt == DataLayout::SM16
                 , "Fmt value not compatible with storing vSMag");
  auto tmp = val.get ();
#if !__riscv_xtttensixwh
  if (fmt == DataLayout::I32)
    tmp = impl_::smag_to_int (vSMag (tmp)).get ();
#endif
  write (tmp,
         fmt == DataLayout::I32 ?
#if !__riscv_xtttensixwh
         SFPSTORE_MOD0_FMT_INT32 :
#else
         SFPSTORE_MOD0_FMT_SM32 :
#endif
         fmt == DataLayout::SM32 ? SFPSTORE_MOD0_FMT_INT32 :
         fmt == DataLayout::SM16 ? SFPSTORE_MOD0_FMT_INT16 :
         ~0);
}

template<template<sfpi::DataLayout> typename Derived, sfpi::DataLayout Fmt>
void sfpi::impl_::vReg_<Derived, Fmt>::operator= (vSMag16 val) const {
  constexpr DataLayout fmt = Fmt != DataLayout::Default ? Fmt : DataLayout::SM16;
  static_assert (false
                 || fmt == DataLayout::SM16
                 , "Fmt value not compatible with storing vSMag16");
  write (val.get (),
         fmt == DataLayout::SM16 ? SFPSTORE_MOD0_FMT_INT16 :
         ~0);
}

template<template<sfpi::DataLayout> typename Derived, sfpi::DataLayout Fmt>
sfpi::impl_::vReg_<Derived, Fmt>::operator vSMag () const {
  constexpr DataLayout fmt = Fmt != DataLayout::Default ? Fmt : DataLayout::SM32;
  static_assert (false
                 || fmt == DataLayout::I32
                 || fmt == DataLayout::SM32
                 || fmt == DataLayout::SM16
                 , "Fmt value not compatible with loading vSMag");
  auto val = read (fmt == DataLayout::I32 ?
#if !__riscv_xtttensixwh
                   SFPLOAD_MOD0_FMT_INT32 :
#else
                   SFPLOAD_MOD0_FMT_SM32 :
#endif
                   fmt == DataLayout::SM32 ? SFPLOAD_MOD0_FMT_INT32 :
                   fmt == DataLayout::SM16 ? SFPLOAD_MOD0_FMT_INT16 :
                   ~0);
#if !__riscv_xtttensixwh
  if (fmt == DataLayout::I32)
    val = impl_::int_to_smag (vInt (val)).get ();
#endif
  return vSMag (val);
}

template<template<sfpi::DataLayout> typename Derived, sfpi::DataLayout Fmt>
sfpi::impl_::vReg_<Derived, Fmt>::operator vSMag16 () const {
  constexpr DataLayout fmt = Fmt != DataLayout::Default ? Fmt : DataLayout::SM16;
  static_assert (false
                 || fmt == DataLayout::SM16
                 , "Fmt value not compatible with loading vSMag16");
  auto val = read (fmt == DataLayout::SM16 ? SFPLOAD_MOD0_FMT_INT16 :
                   ~0);
  return vSMag16 (val);
}

template<sfpi::DataLayout Fmt>
auto sfpi::impl_::DstRegFile::vReg<Fmt>::operator= (vReg<Fmt> const &reg) const-> void {
  *this = vFloat (*reg);
}
template<sfpi::DataLayout Fmt>
auto sfpi::impl_::DstRegFile::vReg<Fmt>::operator- () const-> vFloat {
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
  vFloat neg1 = -1.0f;
  *this = __builtin_rvtt_sfpmad (neg1.get (), a.get(), get (), SFPMAD_MOD1_OFFSET_NONE);
#else // __riscv_xtttensixbh || __riscv_xtttensixqsr
  *this = *this + -a;
#endif
  return *this;
}
auto sfpi::vFloat::operator*= (vFloat m)-> vFloat & { return *this = *this * m; }

auto sfpi::vFloat::operator++ (int)-> vFloat { vFloat tmp = *this; *this += 1; return tmp; }
auto sfpi::vFloat::operator++ ()-> vFloat { *this += 1; return *this; }
auto sfpi::vFloat::operator-- (int)-> vFloat { vFloat tmp = *this; *this -= 1; return tmp; }
auto sfpi::vFloat::operator-- ()-> vFloat { *this -= 1; return *this; }

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

// Comparisons
auto sfpi::operator== (vFloat a, vFloat b)-> vBool { return vBool (vBool::EQ, a, b); }
auto sfpi::operator!= (vFloat a, vFloat b)-> vBool { return vBool (vBool::NE, a, b); }
auto sfpi::operator< (vFloat a, vFloat b)-> vBool { return vBool (vBool::LT, a, b); }
auto sfpi::operator> (vFloat a, vFloat b)-> vBool { return vBool (vBool::GT, a, b); }
auto sfpi::operator<= (vFloat a, vFloat b)-> vBool { return vBool (vBool::LTE, a, b); }
auto sfpi::operator>= (vFloat a, vFloat b)-> vBool { return vBool (vBool::GTE, a, b); }

// FIXME: Until we get sfpxloadi optimization into sfpxfcmp, special case these compares
auto sfpi::operator== (vFloat a, float b)-> vBool { return vBool (vBool::EQ, a, b); }
auto sfpi::operator!= (vFloat a, float b)-> vBool { return vBool (vBool::NE, a, b); }
auto sfpi::operator< (vFloat a, float b)-> vBool { return vBool (vBool::LT, a, b); }
auto sfpi::operator> (vFloat a, float b)-> vBool { return vBool (vBool::GT, a, b); }
auto sfpi::operator<= (vFloat a, float b)-> vBool { return vBool (vBool::LTE, a, b); }
auto sfpi::operator>= (vFloat a, float b)-> vBool { return vBool (vBool::GTE, a, b); }

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

sfpi::vInt::operator sfpi::vUInt () const {
  return vUInt (*this);
}

auto sfpi::vInt::operator+= (vInt a)-> vInt & { return *this = *this + a; }
auto sfpi::vInt::operator-= (vInt a)-> vInt & { return *this = *this - a; }
auto sfpi::vInt::operator<<= (unsigned a)-> vInt & { return *this = *this << a; }
auto sfpi::vInt::operator<<= (vUInt a)-> vInt & { return *this = *this << a; }
#if __riscv_xtttensixbh || __riscv_xtttensixqsr
auto sfpi::vInt::operator>>= (unsigned a)-> vInt & { return *this = *this >> a; }
auto sfpi::vInt::operator>>= (vUInt a)-> vInt & { return *this = *this >> a; }
#endif
auto sfpi::vInt::operator&= (vInt a)-> vInt & { return *this = *this & a; }
auto sfpi::vInt::operator|= (vInt a)-> vInt & { return *this = *this | a; }
auto sfpi::vInt::operator^= (vInt a)-> vInt & { return *this = *this ^ a; }

auto sfpi::vInt::operator++ (int)-> vInt { vInt tmp = *this; *this += 1; return tmp; }
auto sfpi::vInt::operator++ ()-> vInt { *this += 1; return *this; }
auto sfpi::vInt::operator-- (int)-> vInt { vInt tmp = *this; *this -= 1; return tmp; }
auto sfpi::vInt::operator-- ()-> vInt { *this -= 1; return *this; }

auto sfpi::operator+ (vInt a)-> vInt { return a; }
auto sfpi::operator+ (vInt a, vInt b)-> vInt { return a.int_add (b, true); }
auto sfpi::operator+ (vInt a, vMag b)-> vInt { return a.int_add (b, true); }
auto sfpi::operator+ (vMag a, vInt b)-> vInt { return a.int_add (b, true); }
auto sfpi::operator+ (vInt a, int32_t b)-> vInt { return a.int_add (b, true); }
auto sfpi::operator- (vInt a)-> vInt { return vInt (0) - a; }
auto sfpi::operator- (vInt a, vInt b)-> vInt { return a.int_sub (b, true); }
auto sfpi::operator- (vInt a, vMag b)-> vInt { return a.int_sub (b, true); }
auto sfpi::operator- (vMag a, vInt b)-> vInt { return a.int_sub (b, true); }
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

auto sfpi::operator== (vInt a, vInt b)-> vBool { return vBool (vBool::EQ, b, a, SFPXIADD_MOD1_SIGNED); }
auto sfpi::operator!= (vInt a, vInt b)-> vBool { return vBool (vBool::NE, b, a, SFPXIADD_MOD1_SIGNED); }
auto sfpi::operator< (vInt a, vInt b)-> vBool { return vBool (vBool::LT, b, a, SFPXIADD_MOD1_SIGNED); }
auto sfpi::operator> (vInt a, vInt b)-> vBool { return vBool (vBool::GT, b, a, SFPXIADD_MOD1_SIGNED); }
auto sfpi::operator<= (vInt a, vInt b)-> vBool { return vBool (vBool::LTE, b, a, SFPXIADD_MOD1_SIGNED); }
auto sfpi::operator>= (vInt a, vInt b)-> vBool { return vBool (vBool::GTE, b, a, SFPXIADD_MOD1_SIGNED); }

// FIXME: Until we get sfpxloadi optimization into sfpxfcmp, special case these compares
auto sfpi::operator== (vInt a, int32_t b)-> vBool { return vBool (vBool::EQ, a, b, SFPXIADD_MOD1_SIGNED); }
auto sfpi::operator!= (vInt a, int32_t b)-> vBool { return vBool (vBool::NE, a, b, SFPXIADD_MOD1_SIGNED); }
auto sfpi::operator< (vInt a, int32_t b)-> vBool { return vBool (vBool::LT, a, b, SFPXIADD_MOD1_SIGNED); }
auto sfpi::operator> (vInt a, int32_t b)-> vBool { return vBool (vBool::GT, a, b, SFPXIADD_MOD1_SIGNED); }
auto sfpi::operator<= (vInt a, int32_t b)-> vBool { return vBool (vBool::LTE, a, b, SFPXIADD_MOD1_SIGNED); }
auto sfpi::operator>= (vInt a, int32_t b)-> vBool { return vBool (vBool::GTE, a, b, SFPXIADD_MOD1_SIGNED); }

//////////////////////////////////////////////////////////////////////////////
// vUInt definitions
sfpi::vUInt::vUInt (impl_::sfpu_t vec) : vVal (vec) {}
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

sfpi::vUInt::operator sfpi::vInt () const {
  return vInt (*this);
}

auto sfpi::vUInt::operator+= (vUInt a)-> vUInt & { return *this = *this + a; }
auto sfpi::vUInt::operator-= (vUInt a)-> vUInt & { return *this = *this - a; }
auto sfpi::vUInt::operator<<= (unsigned a)-> vUInt & { return *this = *this << a; }
auto sfpi::vUInt::operator<<= (vUInt a)-> vUInt & { return *this = *this << a; }
auto sfpi::vUInt::operator>>= (unsigned a)-> vUInt & { return *this = *this >> a; }
auto sfpi::vUInt::operator>>= (vUInt a)-> vUInt & { return *this = *this >> a; }
auto sfpi::vUInt::operator&= (vUInt a)-> vUInt & { return *this = *this & a; }
auto sfpi::vUInt::operator|= (vUInt a)-> vUInt & { return *this = *this | a; }
auto sfpi::vUInt::operator^= (vUInt a)-> vUInt & { return *this = *this ^ a; }

auto sfpi::vUInt::operator++ (int)-> vUInt { vUInt tmp = *this; *this += 1; return tmp; }
auto sfpi::vUInt::operator++ ()-> vUInt { *this += 1; return *this; }
auto sfpi::vUInt::operator-- (int)-> vUInt { vUInt tmp = *this; *this -= 1; return tmp; }
auto sfpi::vUInt::operator-- ()-> vUInt { *this -= 1; return *this; }

auto sfpi::operator+ (vUInt a)-> vUInt { return a; }
auto sfpi::operator+ (vUInt a, vUInt b)-> vUInt { return a.int_add (b, false); }
auto sfpi::operator+ (vUInt a, vMag b)-> vUInt { return a.int_add (b, false); }
auto sfpi::operator+ (vMag a, vUInt b)-> vUInt { return a.int_add (b, false); }
auto sfpi::operator+ (vUInt a, int32_t b)-> vUInt { return a.int_add (b, false); }
auto sfpi::operator- (vUInt a)-> vUInt { return vUInt (0) - a; }
auto sfpi::operator- (vUInt a, vUInt b)-> vUInt { return a.int_sub (b, false); }
auto sfpi::operator- (vUInt a, vMag b)-> vUInt { return a.int_sub (b, false); }
auto sfpi::operator- (vMag a, vUInt b)-> vUInt { return a.int_sub (b, false); }
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

auto sfpi::operator== (vUInt a, vUInt b)-> vBool { return vBool (vBool::EQ, b, a, 0); }
auto sfpi::operator!= (vUInt a, vUInt b)-> vBool { return vBool (vBool::NE, b, a, 0); }
auto sfpi::operator< (vUInt a, vUInt b)-> vBool { return vBool (vBool::LT, b, a, 0); }
auto sfpi::operator> (vUInt a, vUInt b)-> vBool { return vBool (vBool::GT, b, a, 0); }
auto sfpi::operator<= (vUInt a, vUInt b)-> vBool { return vBool (vBool::LTE, b, a, 0); }
auto sfpi::operator>= (vUInt a, vUInt b)-> vBool { return vBool (vBool::GTE, b, a, 0); }

// FIXME: Until we get sfpxloadi optimization into sfpxfcmp, special case these compares
auto sfpi::operator== (vUInt a, uint32_t b)-> vBool { return vBool (vBool::EQ, a, b, 0); }
auto sfpi::operator!= (vUInt a, uint32_t b)-> vBool { return vBool (vBool::NE, a, b, 0); }
auto sfpi::operator< (vUInt a, uint32_t b)-> vBool { return vBool (vBool::LT, a, b, 0); }
auto sfpi::operator> (vUInt a, uint32_t b)-> vBool { return vBool (vBool::GT, a, b, 0); }
auto sfpi::operator<= (vUInt a, uint32_t b)-> vBool { return vBool (vBool::LTE, a, b, 0); }
auto sfpi::operator>= (vUInt a, uint32_t b)-> vBool { return vBool (vBool::GTE, a, b, 0); }


//////////////////////////////////////////////////////////////////////////////
// vMag definitions
auto sfpi::operator>> (vMag a, unsigned b)-> vMag { return vMag (a.int_shift (-b, false)); }
auto sfpi::operator>> (vMag a, int b)-> vMag { return vMag (a.int_shift (-b, false)); }
auto sfpi::operator>> (vMag a, vUInt b)-> vMag { return vMag (a.int_shift (-b, false)); }

auto sfpi::operator& (vMag a, unsigned b)-> vMag { return a & vMag (b); }
auto sfpi::operator& (vMag a, int b)-> vMag { return a & unsigned (b); }
auto sfpi::operator& (vMag a, vMag b)-> vMag { return vMag (a.int_and (b)); }

//////////////////////////////////////////////////////////////////////////////
// vSMag definitions
sfpi::vSMag::vSMag (impl_::sfpu_t vec) : vVal (vec) {}
sfpi::vSMag::vSMag (uint32_t val)
    : vVal (__builtin_rvtt_sfpxloadi (val, 32)) {}

auto sfpi::operator& (vSMag a, unsigned b)-> vUInt { return a.int_and (vUInt (b)); }

auto sfpi::operator== (vSMag a, vSMag b)-> vBool { return vBool (vBool::EQ, a, b, 0); }
auto sfpi::operator== (vSMag a, unsigned b)-> vBool { return vBool (vBool::EQ, a, b, 0); }
auto sfpi::operator== (vSMag a, int b)-> vBool { return vBool (vBool::EQ, a, b, 0); }

auto sfpi::operator!= (vSMag a, vSMag b)-> vBool { return vBool (vBool::NE, a, b, 0); }
auto sfpi::operator!= (vSMag a, unsigned b)-> vBool { return vBool (vBool::NE, a, b, 0); }
auto sfpi::operator!= (vSMag a, int b)-> vBool { return vBool (vBool::NE, a, b, 0); }
