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

//////////////////////////////////////////////////////////////////////////////
// impl_::vCond definitions
sfpi::impl_::vCond::vCond (BoolOp t, vCond a, vCond b) {
  result = __builtin_rvtt_sfpxbool (t, a.get (), b.get ());
}
sfpi::impl_::vCond::vCond (CondOp t, vFloat a, float b) {
  result = __builtin_rvtt_sfpxfcmps (a.get (), impl_::float_as_uint (b), t | SFPXSCMP_MOD1_FMT_FLOAT);
}
sfpi::impl_::vCond::vCond (CondOp t, vFloat a, s2vFloat16 b) {
  result = __builtin_rvtt_sfpxfcmps (a.get (), b.get (),
                                     t | (b.get_format () == SFPLOADI_MOD0_FLOATA
                                          ? SFPXSCMP_MOD1_FMT_A : SFPXSCMP_MOD1_FMT_B));
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
// impl_::vDReg definitions
auto sfpi::impl_::vDReg::operator=(vFloat vec) const-> vFloat {
  __builtin_rvtt_sfpstore(vec.get(), reg, SFPSTORE_MOD0_FMT_SRCB, SFPSTORE_ADDR_MODE_NOINC);
  return vec;
}

auto sfpi::impl_::vDReg::operator= (double d) const-> vFloat {
  vFloat v (static_cast<float> (d));
  *this = v;
  return v;
}

template <typename vecType,
          typename std::enable_if_t<std::is_base_of<sfpi::impl_::vVal, vecType>::value>*>
auto sfpi::impl_::vDReg::operator= (vecType vec) const-> vecType {
  auto mod = SFPSTORE_MOD0_FMT_BOB32;
#if __riscv_xtttensixwh
  if constexpr (std::is_base_of<vInt, vecType>::value)
                   mod = SFPSTORE_MOD0_FMT_SM32;
#endif
  __builtin_rvtt_sfpstore (vec.get (), reg, mod, SFPSTORE_ADDR_MODE_NOINC);
  return vec;
}

auto sfpi::impl_::vDReg::operator=(const impl_::vDReg dreg) const-> void {
  vFloat tmp = dreg;
  __builtin_rvtt_sfpstore (tmp.get(), reg, SFPSTORE_MOD0_FMT_SRCB, SFPSTORE_ADDR_MODE_NOINC);
}

auto sfpi::impl_::vDReg::operator= (s2vFloat16 f) const-> vFloat {
  vFloat v (f);
  *this = v;
  return v;
}

auto sfpi::impl_::vDReg::operator= (const float f) const-> vFloat {
  vFloat v (f);
  *this = v;
  return v;
}
auto sfpi::impl_::vDReg::operator= (const int i) const-> vInt {
  vInt v(i);
  *this = v;
  return v;
}

auto sfpi::impl_::vDReg::operator= (const unsigned int i) const-> vUInt {
  vUInt v(i);
  *this = v;
  return v;
}

auto sfpi::impl_::vDReg::operator- () const-> vFloat {
  vFloat tmp = *this;
  return __builtin_rvtt_sfpmov (tmp.get (), SFPMOV_MOD1_COMPSIGN);
}

auto sfpi::impl_::vDReg::operator+ (const vFloat b) const-> vFloat { return vFloat (*this) + b; }
auto sfpi::impl_::vDReg::operator- (const vFloat b) const-> vFloat { return vFloat (*this) - b; }
auto sfpi::impl_::vDReg::operator* (const vFloat b) const-> vFloat  { return vFloat (*this) * b; }

auto sfpi::impl_::vDReg::operator== (const float x) const-> vCond { return vCond (vCond::EQ, vFloat (*this), x); }
auto sfpi::impl_::vDReg::operator!= (const float x) const-> vCond { return vCond (vCond::NE, vFloat (*this), x); }
auto sfpi::impl_::vDReg::operator< (const float x) const-> vCond { return vCond (vCond::LT, vFloat (*this), x); }
auto sfpi::impl_::vDReg::operator> (const float x) const-> vCond { return vCond (vCond::GT, vFloat (*this), x); }
auto sfpi::impl_::vDReg::operator<= (const float x) const-> vCond { return vCond (vCond::LTE, vFloat (*this), x); }
auto sfpi::impl_::vDReg::operator>= (const float x) const-> vCond { return vCond (vCond::GTE, vFloat (*this), x); }

auto sfpi::impl_::vDReg::operator== (const s2vFloat16 x) const-> vCond {return vCond (vCond::EQ, vFloat (*this), x); }
auto sfpi::impl_::vDReg::operator!= (const s2vFloat16 x) const-> vCond { return vCond (vCond::NE, vFloat (*this), x); }
auto sfpi::impl_::vDReg::operator< (const s2vFloat16 x) const-> vCond { return vCond (vCond::LT, vFloat (*this), x); }
auto sfpi::impl_::vDReg::operator> (const s2vFloat16 x) const-> vCond { return vCond (vCond::GT, vFloat (*this), x); }
auto sfpi::impl_::vDReg::operator<= (const s2vFloat16 x) const-> vCond { return vCond (vCond::LTE, vFloat (*this), x); }
auto sfpi::impl_::vDReg::operator>= (const s2vFloat16 x) const-> vCond { return vCond (vCond::GTE, vFloat (*this), x); }
auto sfpi::impl_::vDReg::operator== (const vFloat x) const-> vCond {return vCond (vCond::EQ, vFloat (*this), x); }
auto sfpi::impl_::vDReg::operator!= (const vFloat x) const-> vCond { return vCond (vCond::NE, vFloat (*this), x); }
auto sfpi::impl_::vDReg::operator< (const vFloat x) const-> vCond { return vCond (vCond::LT, vFloat (*this), x); }
auto sfpi::impl_::vDReg::operator> (const vFloat x) const-> vCond { return vCond (vCond::GT, vFloat (*this), x); }
auto sfpi::impl_::vDReg::operator<= (const vFloat x) const-> vCond { return vCond (vCond::LTE, vFloat (*this), x); }
auto sfpi::impl_::vDReg::operator>= (const vFloat x) const-> vCond { return vCond (vCond::GTE, vFloat (*this), x); }

//////////////////////////////////////////////////////////////////////////////
// vFloat definitions
sfpi::vFloat::vFloat (impl_::sfpu_t vec) : vVal (vec) {}
sfpi::vFloat::vFloat (impl_::vLReg lr) : vVal (__builtin_rvtt_sfpreadlreg (lr.get ())) {}
sfpi::vFloat::vFloat (impl_::vDReg dreg)
    : vVal (__builtin_rvtt_sfpload (dreg.get (), SFPLOAD_MOD0_FMT_SRCB, SFPLOAD_ADDR_MODE_NOINC)) {}
sfpi::vFloat::vFloat (s2vFloat16 val)
    : vVal (__builtin_rvtt_sfploadi (val.get (), val.get_format ())) {}
sfpi::vFloat::vFloat (float f)
    : vVal (__builtin_rvtt_sfpxloadi (impl_::float_as_uint (f), -32)) {}

auto sfpi::vFloat::operator= (vFloat in)-> vFloat & { assign (in.get ()); return *this; }
auto sfpi::vFloat::operator= (impl_::vLReg lr)-> vFloat &  { impl_::vVal::operator= (lr); return *this; }

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
sfpi::vInt::vInt (impl_::vLReg lr)
    : vVal (__builtin_rvtt_sfpreadlreg (lr.get ())) {}
sfpi::vInt::vInt (impl_::vDReg dreg)
    : vVal (__builtin_rvtt_sfpload (dreg.get(),
#if __riscv_xtttensixwh
                                    SFPLOAD_MOD0_FMT_SM32,
#else
                                    SFPLOAD_MOD0_FMT_BOB32,
#endif
                                    SFPLOAD_ADDR_MODE_NOINC)) {
  // FIXME: This should really convert from FPU's sign-magnitude integer representation
}
sfpi::vInt::vInt (vUInt val) : vVal (val.get ()) {}
sfpi::vInt::vInt (int16_t val)
    : vVal (__builtin_rvtt_sfploadi (val, SFPLOADI_MOD0_SHORT)) {}
sfpi::vInt::vInt (uint16_t val)
    : vVal (__builtin_rvtt_sfploadi (val, SFPLOADI_MOD0_USHORT)) {}
sfpi::vInt::vInt (int32_t val)
    : vVal (__builtin_rvtt_sfpxloadi (val, 31)) {}
sfpi::vInt::vInt (uint32_t val)
    : vVal (__builtin_rvtt_sfpxloadi (val, -32)) {}
sfpi::vInt::vInt (int val) : vInt (int32_t (val)) {}
sfpi::vInt::vInt (unsigned val) : vInt (uint32_t (val)) {}
sfpi::vInt::vInt(const impl_::vCond vc)
    : vVal (__builtin_rvtt_sfpxcondi (vc.get ())) {}

auto sfpi::vInt::operator= (vInt in)-> vInt & { assign(in.v); return *this; }
auto sfpi::vInt::operator= (impl_::vLReg lr)-> vInt & { impl_::vVal::operator= (lr); return *this; }

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
sfpi::vUInt::vUInt (impl_::vLReg lr)
    : vVal (__builtin_rvtt_sfpreadlreg (lr.get ())) {}
sfpi::vUInt::vUInt (impl_::vDReg dreg)
    : vVal (__builtin_rvtt_sfpload (dreg.get(),
                                    SFPLOAD_MOD0_FMT_BOB32,
                                    SFPLOAD_ADDR_MODE_NOINC)) {}
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

auto sfpi::vUInt::operator= (vUInt in)-> vUInt & { assign (in.get ()); return *this; }
auto sfpi::vUInt::operator= (impl_::vLReg lr)-> vUInt & { impl_::vVal::operator= (lr); return *this; }

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
