/*
 * SPDX-FileCopyrightText: © 2023-2026 Tenstorrent Inc.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

      /****************************************
       ***    WARNING: Construction Zone    ***
       *** Refactoring for code cleanliness ***
       *** and type rigorousness.           ***
       ****************************************/

///////////////////////////////////////////////////////////////////////////////
// sfpi.h: SFPu Interface
//   This file provides a C++ wrapper around GCC style __builtin functions
//   which map to SFPU instructions.
//
// Note: Op refers to "Operation" while vOperand is spelled out.  Usually.
//
// Local Register (LREG) Operations:
//   class vUInt
//   class vInt
//   class vFloat
//
//   These classes are the workhorse classes of sfpi.  Operators are
//   overloaded to provide functionality such as:
//     vFloat a, b, c, d;
//     vFloat d = a * b + c;
//
//   The classes also support conditional operators for use in conjunction
//   with the vCCCtrl class to enable predicated execution, for example:
//     vFloat a;
//     v_if (a < 5.0F)
//   will load an immediate value of 5.0F into an LReg, perform the subtract
//   and test the result to set the CC.
//
//   Additional operations such as load immediate, extract/set exp, etc are
//   supported through class methods.
//
// Destination Register:
//   class impl_::vDReg
//   constexpr impl_::vDReg::DRegFile dst_reg;
//
//   The Destination Register is modeled by a global variable which is
//   essentially an array of class vDReg.  vDRegs provide much the same
//   functionality as LRegs, eg, the following is legal:
//       dst_reg[0] = dst_reg[1] * dst_reg[2] + dst_reg[3];
//   The above is expanded out to load local registers, perform the operation
//   and store back into the destination register.  Any missing functionality
//   (eg, there is no load immediate) accessed through LRegs.
//
// Constant Local Registers:
//   template<typename T> class __vConst<T>
//   constexpr __vConst<VFloat> vConst0p6928(5);
//
//   The constant value local registers are used in expressions by referencing
//   one of the names above and using them in mathematical operations such as:
//       a = b * c + vConst0p6928;
//
// Predicated Execution:
//   class __vCCCtrl
//   macros v_if(), v_elseif(), v_else, v_endif
//
//   The class __vCCCtrl is used in conjunction with the LReg based classes to
//   enable predicated execution.  By convention the test infastructure indents
//   the code as if executing if/then/else in C++, for example:
//     __vCCCtrl cc;
//     vFloat v = dst_reg[0];
//     cc.cc_v_if().cc_cond(v < 5.0F); {
//         // if side
//     } cc.cc_else(); {
//         // else side
//     }
//     cc.cc_endif();
//   The above chatter is reduced w/ use of macros listed above.  The macros
//   also balance {} to auto-destruct the __vCCCtrl when appropriate.
//
//  Boolean Operators:
//   && and || are handled by building a tree of operations and traversing
//   this at compile time, all with 0 run time overhead.  This requires some
//   compromises.  Ideally, this would be handled by an inline recursive
//   function, which gcc can do, but not with "always_inline".  Instead, the
//   emit() function uses a macro to expand out to 3 levels of recursiion.
//   This is ripe for moving into the compiler.
//
//  Internals:
//   The idea behind this wrapper is that it all collapses down to
//   typically one and sometimes more SFPU instructions at compile time
//   without any runtime overhead.  Many of the classes below are used to
//   track operations/operands as the compiler parses the code and then at a
//   trigger operation (eg, assignment) the operation gets expanded into an
//   instruction.  All of this working relies on compiling w/ optimization
//   enabled (-O) and that the vectors never get written to the stack.
//
//  Liveness:
//    A major complication of the SFPU for the compiler is that assigning a
//    variable doesn't always make the previous assignment obsolete.  For
//    example, assigning x, then executing a v_if and re-assigning x within
//    the predicate keeps x "alive" rather than letting it go dead prior to
//    the second assignment.  The solution for this is a hybrid
//    wrapper/compiler solution; a document exists which describes this in
//    detail.  The main piece for the wrapper is that assignments generate an
//    sfpassign_lv instruction which propogates the value from the previous
//    assignment, thought this can only be done if the variable was previously
//    written (initialized) and is only necessary if the instruction doesn't
//    take the destination as an input (otherwise an unnecessary move may be
//    generated).
//
//  Ugliness:
//    The wrapper should get smaller over time as functionality is moved into
//    the compiler back end (optimizer) and front end (parser, eg, v_endif).
//    Currently the biggest uglinesses are:
//      v_endif
//      assignments return void so cannot be used in, eg conditionals
//        since the compiler doesn't understand predication, using assignments
//        in condtionals can result in evaluation of the assignments out of
//        order w/ regard to the predication CC bits
//      related to the above, add_cc, exexp_cc and lz_cc are exposed to handle
//        instructions within conditionals
//      && and || can only go 3 deep in one conditional
//      conditionals are complex
//
///////////////////////////////////////////////////////////////////////////////
#pragma once

#include <type_traits>

#if !__has_builtin(__builtin_rvtt_synth_opcode)
#error "Compiler does not support TENSIX builtins"
#include "stop now, no good will come"
#endif

#include "sfpi_constants.h"
#include "sfpi_builtins.h"

#define sfpi_inline __attribute__((always_inline)) inline

#include "sfpi_fp16.h"

namespace sfpi {

//////////////////////////////////////////////////////////////////////////////
// Interface
class vFloat;
class vInt;
class vUInt;
enum class LRegs;

namespace impl_ {
using sfpu_t = ::__xtt_vector;
class vVal;
class vReg;
class vLReg;
}

//class __vIntBase;
//class __vConstIntBase;
class __vCond;
class __vCCCtrl;
class __vCCCtrlBase;

//////////////////////////////////////////////////////////////////////////////
namespace impl_ {
sfpi_inline uint32_t float_as_uint (const float val)
{
  union U {
    float f;
    uint32_t i;

    constexpr U(float inf) : f(inf) {}
    operator uint32_t () const { return i; }
  };

  return U(val);
}
}

//////////////////////////////////////////////////////////////////////////////
namespace impl_ {
class vReg { // A register.  Holds the register number
protected:
  int reg;

public:
  constexpr explicit vReg (int r) : reg (r) {}
  constexpr int get () const { return reg; }
};

class vVal { // A value, holds value and intialized flag
protected:
  // FIXME: we want to remove this, with better LV optimization/handling
  bool initialized = false;
  sfpu_t v;

  sfpi_inline void assign (sfpu_t val) {
    v = initialized ? __builtin_rvtt_sfpassign_lv (v, val) : val;
    initialized = true;
  }

public:
  sfpi_inline vVal () {}
  sfpi_inline vVal (sfpu_t v_)
      :initialized (true), v (v_) {
  }

  sfpi_inline sfpu_t get () const { return v; }

  // Associate variable w/ a value pre-loaded into a particular lreg
  sfpi_inline void operator= (vLReg lr);

public:
  sfpi_inline sfpu_t int_and (vVal b) const {
    return __builtin_rvtt_sfpand (get (), b.get ());
  }
  sfpi_inline sfpu_t int_or (vVal b) const {
    return __builtin_rvtt_sfpor (get (), b.get ());
  }
  sfpi_inline sfpu_t int_xor (vVal b) const {
    return __builtin_rvtt_sfpxor (get (), b.get ());
  }
  sfpi_inline sfpu_t int_not () const {
    return __builtin_rvtt_sfpnot (get ());
  }

public:
  sfpi_inline sfpu_t int_add (vVal b, bool is_signed) const {
    return __builtin_rvtt_sfpxiadd_v (get (), b.get (), is_signed ? SFPXIADD_MOD1_SIGNED : 0);
  }
  sfpi_inline sfpu_t int_add (int32_t b, bool is_signed) const {
    return __builtin_rvtt_sfpxiadd_i (get (), b, is_signed ? SFPXIADD_MOD1_SIGNED : 0);
  }
  sfpi_inline sfpu_t int_sub (vVal b, bool is_signed) const {
    // subtracts the first op from the second op, because, why not :)
    return __builtin_rvtt_sfpxiadd_v (b.get (), get (), (is_signed ? SFPXIADD_MOD1_SIGNED : 0) | SFPXIADD_MOD1_IS_SUB);
  }
  sfpi_inline sfpu_t int_sub (int32_t b, bool is_signed) const {
    // subtracts the first op from the second op, because, why not :)
    return __builtin_rvtt_sfpxiadd_i (get (), b, (is_signed ? SFPXIADD_MOD1_SIGNED : 0) | SFPXIADD_MOD1_IS_SUB);
  }
  sfpi_inline sfpu_t int_shift (vVal b, bool is_signed) const {
    return __builtin_rvtt_sfpshft_v (get (), b.get (),
#if __riscv_xtttensixbh
                                     is_signed ? SFPSHFT_MOD1_ARITHMETIC :
#endif
                                     SFPSHFT_MOD1_LOGICAL);
  }
  sfpi_inline sfpu_t int_shift (int b, bool is_signed) const {
    return __builtin_rvtt_sfpshft_i (get (), b,
#if __riscv_xtttensixbh
                                     is_signed ? SFPSHFT_MOD1_ARITHMETIC :
#endif
                                     SFPSHFT_MOD1_LOGICAL);
  }

public:
  sfpi_inline sfpu_t flt_add (vVal b) const {
    return __builtin_rvtt_sfpadd (get (), b.get (), 0);
  }
  sfpi_inline sfpu_t flt_mul (vVal b) const {
    return __builtin_rvtt_sfpmul (get (), b.get (), 0);
  }
};
}

//////////////////////////////////////////////////////////////////////////////
namespace impl_ {
// A constant register
template<typename Type, typename std::enable_if_t<std::is_base_of<impl_::vVal, Type>::value>* = nullptr>
class vConst : public vReg {
public:
  constexpr explicit vConst (int r) : impl_::vReg(r) {}

  sfpi_inline operator Type () const {
    return __builtin_rvtt_sfpreadlreg (get ());
  }

  sfpi_inline void operator= (Type t) const {
    __builtin_rvtt_sfpwriteconfig_v (t.get (), get ());
  }

  template<typename U,
           std::enable_if_t<std::is_constructible<Type, U const &>::value> * = nullptr>
  sfpi_inline void operator= (U u) const {
    operator= (Type (u));
  }
};
}

//////////////////////////////////////////////////////////////////////////////
// LRegs

enum class LRegs {
  LReg0, LReg1, LReg2, LReg3, LReg4, LReg5, LReg6, LReg7,
  LRegCount = SFP_LREG_COUNT,
};

namespace impl_ {

class vLReg : public vReg {
private:
  constexpr explicit vLReg (unsigned ix) : vReg (ix) {}

public:
  sfpi_inline sfpu_t operator= (const vVal& v) const {
    __builtin_rvtt_sfpwritelreg (v.get (), reg);
    return v.get ();
  }

public:
  class LRegFile {
  public:
    sfpi_inline vLReg operator[] (LRegs lr) const { return vLReg (unsigned (lr)); }
  };
};
sfpi_inline void impl_::vVal::operator= (impl_::vLReg lr) {
  // FIXME: shouldn't this pay attention to initialized?
  v = __builtin_rvtt_sfpreadlreg (lr.get ());
  initialized = true;
}
}

//////////////////////////////////////////////////////////////////////////////
// Dst regs
namespace impl_ {
class vDReg : public vReg {
private:
    constexpr explicit vDReg (unsigned i) : vReg (i) {}

public:
    // Assign register to register
    template <typename vecType, typename std::enable_if_t<std::is_base_of<impl_::vVal, vecType>::value>* = nullptr>
    sfpi_inline vecType operator=(const vecType vec) const;
    sfpi_inline void operator=(const vDReg dreg) const;
    sfpi_inline vFloat operator=(const s2vFloat16 f) const;
    sfpi_inline vInt operator=(const int i) const;
    sfpi_inline vUInt operator=(const unsigned int i) const;
    sfpi_inline vFloat operator=(const float f) const;
    sfpi_inline vFloat operator=(const double d) const;
    sfpi_inline vFloat operator=(vFloat d) const;

    // Construct operator classes from operations
    sfpi_inline vFloat operator+(const vFloat b) const;
    sfpi_inline vFloat operator-(const vFloat b) const;
    sfpi_inline vFloat operator-() const;
    sfpi_inline vFloat operator*(const vFloat b) const;

    // Conditionals
    sfpi_inline __vCond operator==(const float x) const;
    sfpi_inline __vCond operator!=(const float x) const;
    sfpi_inline __vCond operator<(const float x) const;
    sfpi_inline __vCond operator<=(const float x) const;
    sfpi_inline __vCond operator>(const float x) const;
    sfpi_inline __vCond operator>=(const float x) const;

    sfpi_inline __vCond operator==(const s2vFloat16 x) const;
    sfpi_inline __vCond operator!=(const s2vFloat16 x) const;
    sfpi_inline __vCond operator<(const s2vFloat16 x) const;
    sfpi_inline __vCond operator<=(const s2vFloat16 x) const;
    sfpi_inline __vCond operator>(const s2vFloat16 x) const;
    sfpi_inline __vCond operator>=(const s2vFloat16 x) const;

    sfpi_inline __vCond operator==(const vFloat x) const;
    sfpi_inline __vCond operator!=(const vFloat x) const;
    sfpi_inline __vCond operator<(const vFloat x) const;
    sfpi_inline __vCond operator<=(const vFloat x) const;
    sfpi_inline __vCond operator>(const vFloat x) const;
    sfpi_inline __vCond operator>=(const vFloat x) const;

public:
  class DestRegFile {
  public:
    sfpi_inline const vDReg operator[](int ix) const {
      return vDReg(ix * SFP_DESTREG_STRIDE);
    }

    // Make these void - ugly as these aren't really inc/dec
    sfpi_inline void operator++() const { *this += 1; }
    sfpi_inline void operator++(int) const { *this += 1; }
    sfpi_inline void operator+=(int i) const { __builtin_rvtt_ttincrwc(0, SFP_DESTREG_STRIDE * i, 0, 0); }
  };
};
}

//////////////////////////////////////////////////////////////////////////////

// User-accessible vector float type
class vFloat : public impl_::vVal {
public:
  vFloat() = default;

  // Construction
  sfpi_inline vFloat (impl_::sfpu_t vec)
      : vVal (vec) {
  }
  sfpi_inline vFloat (impl_::vLReg lr)
      : vVal (__builtin_rvtt_sfpreadlreg (lr.get ())) {
  }
  sfpi_inline vFloat (impl_::vDReg dreg)
      : vVal (__builtin_rvtt_sfpload (dreg.get (),
                                      SFPLOAD_MOD0_FMT_SRCB,
                                      SFPLOAD_ADDR_MODE_NOINC)) {
  }
  sfpi_inline vFloat (s2vFloat16 val)
      : vVal (__builtin_rvtt_sfpxloadi (val.get (),
                                        val.get_format ())) {
  }
  sfpi_inline vFloat (float f)
      : vVal (__builtin_rvtt_sfpxloadi (impl_::float_as_uint (f),
                                        SFPXLOADI_MOD0_FLOAT)) {
  }

  // Assignment
  sfpi_inline vFloat &operator= (vFloat in) {
    assign (in.v);
    return *this;
  }
  sfpi_inline vFloat &operator= (impl_::vLReg lr) {
    impl_::vVal::operator= (lr);
    return *this;
  }

  // Opertions
  sfpi_inline vFloat operator+ () const {
    return *this;
  }
  sfpi_inline vFloat operator- () const {
    return __builtin_rvtt_sfpmov (get (), SFPMOV_MOD1_COMPSIGN);
  }

  sfpi_inline vFloat &operator+=(vFloat);
  sfpi_inline vFloat &operator-=(vFloat);
  sfpi_inline vFloat &operator*=(vFloat);

  sfpi_inline vFloat operator++(int) { *this += 1; return *this; }
  sfpi_inline vFloat operator++() { vFloat tmp = *this; *this += 1; return tmp; }
  sfpi_inline vFloat operator--(int) { *this -= 1; return *this; }
  sfpi_inline vFloat operator--() { vFloat tmp = *this; *this -= 1; return tmp; }
};

sfpi_inline vFloat operator+ (vFloat a, vFloat b) {
  return a.flt_add (b);
}
sfpi_inline vFloat operator- (vFloat a, vFloat b) {
  // Do not use sfpmad here, the optimizer will handle that.
  return a.flt_add (-b);
}
sfpi_inline vFloat operator- (vFloat a, float b) {
  return a - vFloat (b);
}
sfpi_inline vFloat operator* (vFloat a, vFloat b) {
  return a.flt_mul (b);
}

//////////////////////////////////////////////////////////////////////////////
// User accessible float constants
constexpr impl_::vConst<vFloat> vConst0(CREG_IDX_0);
constexpr impl_::vConst<vFloat> vConst1(CREG_IDX_1);
constexpr impl_::vConst<vFloat> vConstNeg1(CREG_IDX_NEG_1);

//////////////////////////////////////////////////////////////////////////////
class vInt : public impl_::vVal {
public:
  vInt () = default;

  // Construction
  sfpi_inline vInt (impl_::sfpu_t vec)
      : vVal (vec) {
  }
  sfpi_inline vInt (impl_::vLReg lr)
      : vVal (__builtin_rvtt_sfpreadlreg (lr.get ())) {
  }
  sfpi_inline vInt (impl_::vDReg dreg)
      : vVal (__builtin_rvtt_sfpload (dreg.get(),
#if __riscv_xtttensixwh
                                      SFPLOAD_MOD0_FMT_SM32,
#else
                                      SFPLOAD_MOD0_FMT_BOB32,
#endif
                                      SFPLOAD_ADDR_MODE_NOINC)) {
    // FIXME: This should really convert from FPU's sign-magnitude integer representation
  }

  sfpi_inline vInt (vUInt val);
  sfpi_inline vInt (int16_t val)
      : vVal (__builtin_rvtt_sfpxloadi (val, SFPLOADI_MOD0_SHORT)) {
  }
  sfpi_inline vInt (uint16_t val)
      : vVal (__builtin_rvtt_sfpxloadi (val, SFPLOADI_MOD0_USHORT)) {
  }
  sfpi_inline vInt (int32_t val)
      : vVal (__builtin_rvtt_sfpxloadi (val, SFPXLOADI_MOD0_INT32)) {
  }
  sfpi_inline vInt (uint32_t val)
      : vVal (__builtin_rvtt_sfpxloadi (val, SFPXLOADI_MOD0_UINT32)) {
  }
  
  sfpi_inline vInt (int val) : vInt (int32_t (val)) { }
  sfpi_inline vInt (unsigned val) : vInt (uint32_t (val)) { }

  sfpi_inline vInt(const __vCond vc);

  // Assignment
  sfpi_inline vInt &operator= (vInt in) {
    assign(in.v);
    return *this;
  }
  sfpi_inline vInt &operator= (impl_::vLReg lr) {
    impl_::vVal::operator= (lr);
    return *this;
  }

  // Operations
  sfpi_inline vInt operator+ () const {
    return *this;
  }
  sfpi_inline vInt operator- () const;
  sfpi_inline vInt operator++ ();
  sfpi_inline vInt operator++ (int);
  sfpi_inline vInt operator-- ();
  sfpi_inline vInt operator-- (int);
  
  
  sfpi_inline vInt &operator+= (vInt);
  sfpi_inline vInt &operator-= (vInt);
  sfpi_inline vInt &operator<<= (unsigned);
  sfpi_inline vInt &operator<<= (vInt);
#if __riscv_xtttensixbh
  sfpi_inline vInt &operator>>= (unsigned);
  sfpi_inline vInt &operator>>= (vInt);
#endif
  sfpi_inline vInt &operator&= (vInt);
  sfpi_inline vInt &operator|= (vInt);
  sfpi_inline vInt &operator^= (vInt);
};

class vUInt : public impl_::vVal {
public:
  vUInt () = default;

  // Construction
  sfpi_inline vUInt (impl_::sfpu_t vec)
      : vVal (vec) {
  }
  sfpi_inline vUInt (impl_::vLReg lr)
      : vVal (__builtin_rvtt_sfpreadlreg (lr.get ())) {
  }
  sfpi_inline vUInt (impl_::vDReg dreg)
      : vVal (__builtin_rvtt_sfpload (dreg.get(),
                                      SFPLOAD_MOD0_FMT_BOB32,
                                      SFPLOAD_ADDR_MODE_NOINC)) {
  }

  sfpi_inline vUInt (vInt val);
  sfpi_inline vUInt (int16_t val)
      : vVal (__builtin_rvtt_sfpxloadi (val, SFPLOADI_MOD0_SHORT)) {
  }
  sfpi_inline vUInt (uint16_t val)
      : vVal (__builtin_rvtt_sfpxloadi (val, SFPLOADI_MOD0_USHORT)) {
  }
  sfpi_inline vUInt (int32_t val)
      : vVal (__builtin_rvtt_sfpxloadi (val, SFPXLOADI_MOD0_INT32)) {
  }
  sfpi_inline vUInt (uint32_t val)
      : vVal (__builtin_rvtt_sfpxloadi (val, SFPXLOADI_MOD0_UINT32)) {
  }

  sfpi_inline vUInt (int val) : vUInt (int32_t (val)) { }
  sfpi_inline vUInt (unsigned val) : vUInt (uint32_t (val)) { }

  sfpi_inline vUInt(const __vCond vc);

  // Assignment
  sfpi_inline vUInt &operator= (vUInt in) {
    assign(in.v);
    return *this;
  }
  sfpi_inline vUInt &operator= (impl_::vLReg lr) {
    impl_::vVal::operator= (lr);
    return *this;
  }

  // Operations
  sfpi_inline vUInt operator+ () const {
    return *this;
  }
  sfpi_inline vUInt operator- () const;
  sfpi_inline vUInt operator++ ();
  sfpi_inline vUInt operator++ (int);
  sfpi_inline vUInt operator-- ();
  sfpi_inline vUInt operator-- (int);

  sfpi_inline vUInt &operator+= (vUInt);
  sfpi_inline vUInt &operator-= (vUInt);
  sfpi_inline vUInt &operator<<= (unsigned);
  sfpi_inline vUInt &operator<<= (vUInt);
  sfpi_inline vUInt &operator>>= (unsigned);
  sfpi_inline vUInt &operator>>= (vUInt);
  sfpi_inline vUInt &operator&= (vUInt);
  sfpi_inline vUInt &operator|= (vUInt);
  sfpi_inline vUInt &operator^= (vUInt);
};

sfpi_inline vInt operator+ (vInt a, vInt b) {
  return a.int_add (b, true);
}
sfpi_inline vInt operator+ (vInt a, int32_t b) {
  return a.int_add (b, true);
}
sfpi_inline vInt operator- (vInt a, vInt b) {
  return a.int_sub (b, true);
}
sfpi_inline vUInt operator- (vUInt a, vInt b) {
  return a.int_sub (b, false);
}
sfpi_inline vInt operator- (vInt a, int32_t b) {
  return a.int_sub (b, true);
}
sfpi_inline vInt operator<< (vInt vec, unsigned amt) {
  return vec.int_shift (amt, true);
}
sfpi_inline vInt operator<< (vInt vec, vUInt amt) {
  return vec.int_shift (amt, true);
}

#if __riscv_xtttensixbh
// arithmetic shifts added in blackhole
sfpi_inline vInt operator>>(vInt vec, unsigned amt) {
  return vec.int_shift (-amt, true);
}

sfpi_inline vInt operator>>(vInt vec, vUInt amt) {
  return vec.int_shift (-amt, true);
}
#endif
sfpi_inline vInt operator~ (vInt a) {
  return a.int_not ();
}
sfpi_inline vInt operator& (vInt a, vInt b) {
  return a.int_and (b);
}
sfpi_inline vInt operator| (vInt a, vInt b) {
  return a.int_or (b);
}
sfpi_inline vInt operator^ (vInt a, vInt b) {
  return a.int_xor (b);
}

sfpi_inline vInt vInt::operator- () const {
  return vInt (0) - *this;
}

sfpi_inline vInt &vInt::operator+=(vInt a) {
  return *this = *this + a;
}
sfpi_inline vInt &vInt::operator-=(vInt a) {
  return *this = *this - a;
}
sfpi_inline vInt &vInt::operator<<=(unsigned a) {
  return *this = *this << a;
}
sfpi_inline vInt &vInt::operator<<=(vInt a) {
  return *this = *this << a;
}
#if __riscv_xtttensixbh
sfpi_inline vInt &vInt::operator>>=(unsigned a) {
  return *this = *this >> a;
}
sfpi_inline vInt &vInt::operator>>=(vInt a) {
  return *this = *this >> a;
}
#endif
sfpi_inline vInt &vInt::operator&=(vInt a) {
  return *this = *this & a;
}
sfpi_inline vInt &vInt::operator|=(vInt a) {
  return *this = *this | a;
}
sfpi_inline vInt &vInt::operator^=(vInt a) {
  return *this = *this ^ a;
}

sfpi_inline vInt vInt::operator++(const int) { *this += 1; return *this; }
sfpi_inline vInt vInt::operator++() { vInt tmp = *this; *this += 1; return tmp; }
sfpi_inline vInt vInt::operator--(const int) { *this -= 1; return *this; }
sfpi_inline vInt vInt::operator--() { vInt tmp = *this; *this -= 1; return tmp; }


sfpi_inline vUInt operator+ (vUInt a, vUInt b) {
  return a.int_add (b, false);
}
sfpi_inline vUInt operator+ (vUInt a, int32_t b) {
  return a.int_add (b, false);
}
sfpi_inline vUInt operator- (vUInt a, vUInt b) {
  // operands does a - b
  return a.int_sub (b, false);
}
sfpi_inline vUInt operator- (vUInt a, int32_t b) {
  return a.int_sub (b, false);
}
sfpi_inline vUInt operator<< (vUInt vec, unsigned amt) {
  return vec.int_shift (amt, false);
}
sfpi_inline vUInt operator<< (vUInt vec, vUInt amt) {
  return vec.int_shift (amt, false);
}

sfpi_inline vUInt operator>>(vUInt vec, unsigned amt) {
  return vec.int_shift (-amt, false);
}

sfpi_inline vUInt operator>>(vUInt vec, vUInt amt) {
  return vec.int_shift (-amt, false);
}
sfpi_inline vUInt operator~ (vUInt a) {
  return a.int_not ();
}
sfpi_inline vUInt operator& (vUInt a, vUInt b) {
  return a.int_and (b);
}
sfpi_inline vUInt operator| (vUInt a, vUInt b) {
  return a.int_or (b);
}
sfpi_inline vUInt operator^ (vUInt a, vUInt b) {
  return a.int_xor (b);
}

sfpi_inline vUInt vUInt::operator- () const {
  return vUInt (0) - *this;
}

sfpi_inline vUInt &vUInt::operator+=(vUInt a) {
  return *this = *this + a;
}
sfpi_inline vUInt &vUInt::operator-=(vUInt a) {
  return *this = *this - a;
}
sfpi_inline vUInt &vUInt::operator<<=(unsigned a) {
  return *this = *this << a;
}
sfpi_inline vUInt &vUInt::operator<<=(vUInt a) {
  return *this = *this << a;
}
sfpi_inline vUInt &vUInt::operator>>=(unsigned a) {
  return *this = *this >> a;
}
sfpi_inline vUInt &vUInt::operator>>=(vUInt a) {
  return *this = *this >> a;
}
sfpi_inline vUInt &vUInt::operator&=(vUInt a) {
  return *this = *this & a;
}
sfpi_inline vUInt &vUInt::operator|=(vUInt a) {
  return *this = *this | a;
}
sfpi_inline vUInt &vUInt::operator^=(vUInt a) {
  return *this = *this ^ a;
}

sfpi_inline vUInt vUInt::operator++(const int) { *this += 1; return *this; }
sfpi_inline vUInt vUInt::operator++() { vUInt tmp = *this; *this += 1; return tmp; }
sfpi_inline vUInt vUInt::operator--(const int) { *this -= 1; return *this; }
sfpi_inline vUInt vUInt::operator--() { vUInt tmp = *this; *this -= 1; return tmp; }


//////////////////////////////////////////////////////////////////////////////
class __vCond {
    friend class __vCCCtrl;
    friend class vInt;
    friend class vUInt;

 private:
    enum class vBoolOpType {
        vBoolOr = SFPXBOOL_MOD1_OR,
        vBoolAnd = SFPXBOOL_MOD1_AND,
        vBoolNot = SFPXBOOL_MOD1_NOT,
    };

    int result;

    sfpi_inline int get() const { return result; }

 public:
    enum __vCondOpType {
        __vCondLT = SFPXCMP_MOD1_CC_LT,
        __vCondNE = SFPXCMP_MOD1_CC_NE,
        __vCondGTE = SFPXCMP_MOD1_CC_GTE,
        __vCondEQ = SFPXCMP_MOD1_CC_EQ,
        __vCondLTE = SFPXCMP_MOD1_CC_LTE,
        __vCondGT = SFPXCMP_MOD1_CC_GT,
    };

    // Bool
    sfpi_inline __vCond(vBoolOpType t, const __vCond& a, const __vCond& b) { result = __builtin_rvtt_sfpxbool((int)t, a.get(), b.get()); }

    // Float
    sfpi_inline __vCond(const __vCondOpType t, const vFloat a, const float b) {
      result = __builtin_rvtt_sfpxfcmps(a.get(), impl_::float_as_uint(b), t | SFPXSCMP_MOD1_FMT_FLOAT);
    }

    sfpi_inline __vCond(const __vCondOpType t, const vFloat a, const s2vFloat16 b) {
      result = __builtin_rvtt_sfpxfcmps(a.get(), b.get(), t | ((b.get_format() == SFPLOADI_MOD0_FLOATA) ? SFPXSCMP_MOD1_FMT_A : SFPXSCMP_MOD1_FMT_B));
    }

    sfpi_inline __vCond(const __vCondOpType t, const vFloat a, const vFloat b) {
      result = __builtin_rvtt_sfpxfcmpv(a.get(), b.get(), t);
    }

  sfpi_inline __vCond(const __vCondOpType t, vInt a, int32_t b, unsigned mod) {
      result = __builtin_rvtt_sfpxicmps (a.get(), b, mod | t);
    }

    sfpi_inline __vCond(const __vCondOpType t, vInt a, vInt b, unsigned mod) {
      result = __builtin_rvtt_sfpxicmpv(a.get(), b.get(), mod | t);
    }

    // Create from an integer context
    sfpi_inline __vCond(const vInt a) { result = __builtin_rvtt_sfpxicmps(a.get(), 0, __vCondNE); }

    // Create boolean operations from conditional operations
    sfpi_inline const __vCond operator&&(const __vCond& b) const { return __vCond(vBoolOpType::vBoolAnd, *this, b); }
    sfpi_inline const __vCond operator||(const __vCond& b) const { return __vCond(vBoolOpType::vBoolOr, *this, b); }
    sfpi_inline const __vCond operator!() const { return __vCond(vBoolOpType::vBoolNot, *this, *this); }
};

//////////////////////////////////////////////////////////////////////////////
class __vCCCtrl {
protected:
    unsigned dep = 0;
    unsigned depth = 0;

public:
    sfpi_inline __vCCCtrl() = default;
    sfpi_inline ~__vCCCtrl();

    // Moveable, not copyable
    sfpi_inline __vCCCtrl(__vCCCtrl &&src);
    sfpi_inline __vCCCtrl &operator=(__vCCCtrl &&src);

    sfpi_inline __vCCCtrl &cc_if();
    sfpi_inline __vCCCtrl &cc_else();

    sfpi_inline void cc_cond(const __vCond& op);
  //    sfpi_inline void cc_cond(const __vIntBase& b);
  sfpi_inline void cc_cond(vInt b);
  sfpi_inline void cc_cond(vUInt b);
    sfpi_inline __vCCCtrl &cc_push();
    sfpi_inline __vCCCtrl &cc_pop();
  
    static sfpi_inline void enable_cc();
};

//////////////////////////////////////////////////////////////////////////////

sfpi_inline vInt impl_::vDReg::operator=(const int i) const
{
    vInt v(i);
    *this = v;
    return v;
}

sfpi_inline vUInt impl_::vDReg::operator=(const unsigned int i) const
{
    vUInt v(i);
    *this = v;
    return v;
}

sfpi_inline vFloat impl_::vDReg::operator+(const vFloat b) const {return vFloat(*this) + b; }
sfpi_inline vFloat impl_::vDReg::operator-(const vFloat b) const { return vFloat(*this) - b; }
sfpi_inline vFloat impl_::vDReg::operator*(const vFloat b) const  { return vFloat(*this) * b; }

sfpi_inline __vCond impl_::vDReg::operator==(const s2vFloat16 x) const {return __vCond(__vCond::__vCondEQ, vFloat(*this), x); }
sfpi_inline __vCond impl_::vDReg::operator!=(const s2vFloat16 x) const { return __vCond(__vCond::__vCondNE, vFloat(*this), x); }
sfpi_inline __vCond impl_::vDReg::operator<(const s2vFloat16 x) const { return __vCond(__vCond::__vCondLT, vFloat(*this), x); }
sfpi_inline __vCond impl_::vDReg::operator<=(const s2vFloat16 x) const { return __vCond(__vCond::__vCondLTE, vFloat(*this), x); }
sfpi_inline __vCond impl_::vDReg::operator>(const s2vFloat16 x) const { return __vCond(__vCond::__vCondGT, vFloat(*this), x); }
sfpi_inline __vCond impl_::vDReg::operator>=(const s2vFloat16 x) const { return __vCond(__vCond::__vCondGTE, vFloat(*this), x); }
sfpi_inline __vCond impl_::vDReg::operator==(const vFloat x) const {return __vCond(__vCond::__vCondEQ, vFloat(*this), x); }
sfpi_inline __vCond impl_::vDReg::operator!=(const vFloat x) const { return __vCond(__vCond::__vCondNE, vFloat(*this), x); }
sfpi_inline __vCond impl_::vDReg::operator<(const vFloat x) const { return __vCond(__vCond::__vCondLT, vFloat(*this), x); }
sfpi_inline __vCond impl_::vDReg::operator<=(const vFloat x) const { return __vCond(__vCond::__vCondLTE, vFloat(*this), x); }
sfpi_inline __vCond impl_::vDReg::operator>(const vFloat x) const { return __vCond(__vCond::__vCondGT, vFloat(*this), x); }
sfpi_inline __vCond impl_::vDReg::operator>=(const vFloat x) const { return __vCond(__vCond::__vCondGTE, vFloat(*this), x); }

sfpi_inline vFloat impl_::vDReg::operator-() const
{
    vFloat tmp = *this;
    return __builtin_rvtt_sfpmov(tmp.get(), SFPMOV_MOD1_COMPSIGN);
}

//////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////
// Comparisons
sfpi_inline __vCond operator== (vFloat a, vFloat b) { return __vCond(__vCond::__vCondEQ, a, b); }
sfpi_inline __vCond operator!= (vFloat a, vFloat b) { return __vCond(__vCond::__vCondNE, a, b); }
sfpi_inline __vCond operator< (vFloat a, vFloat b) { return __vCond(__vCond::__vCondLT, a, b); }
sfpi_inline __vCond operator<= (vFloat a, vFloat b) { return __vCond(__vCond::__vCondLTE, a, b); }
sfpi_inline __vCond operator> (vFloat a, vFloat b) { return __vCond(__vCond::__vCondGT, a, b); }
sfpi_inline __vCond operator>= (vFloat a, vFloat b) { return __vCond(__vCond::__vCondGTE, a, b); }

// FIXME: Until we get sfpxloadi optimization into sfpxfcmp, special case these compares
sfpi_inline __vCond operator== (vFloat a, float b) { return __vCond(__vCond::__vCondEQ, a, b); }
sfpi_inline __vCond operator!= (vFloat a, float b) { return __vCond(__vCond::__vCondNE, a, b); }
sfpi_inline __vCond operator< (vFloat a, float b) { return __vCond(__vCond::__vCondLT, a, b); }
sfpi_inline __vCond operator<= (vFloat a, float b) { return __vCond(__vCond::__vCondLTE, a, b); }
sfpi_inline __vCond operator> (vFloat a, float b) { return __vCond(__vCond::__vCondGT, a, b); }
sfpi_inline __vCond operator>= (vFloat a, float b) { return __vCond(__vCond::__vCondGTE, a, b); }

sfpi_inline vFloat &vFloat::operator+=(vFloat a) {
    *this = *this + a;
    return *this;
}
sfpi_inline vFloat &vFloat::operator-=(vFloat a)
{
#if __riscv_xtttensixwh
  vFloat neg1 = vConstNeg1;
  assign (__builtin_rvtt_sfpmad (neg1.get (), a.get(), v, SFPMAD_MOD1_OFFSET_NONE));
#else // __riscv_xtttensixbh
  operator+= (-a);
#endif
  return *this;
}
sfpi_inline vFloat &vFloat::operator*=(vFloat m) {
  *this = *this * m;
  return *this;
}

//////////////////////////////////////////////////////////////////////////////
sfpi_inline vFloat operator+(float a, vFloat b) { return b + a; }
sfpi_inline vFloat operator*(float a, vFloat b) { return b * a; }
sfpi_inline vFloat operator-(float a, vFloat b) { return vFloat (a) - b; }
sfpi_inline __vCond operator==(const float a, const vFloat b) { return b == a; }
sfpi_inline __vCond operator!=(const float a, const vFloat b) { return b != a; }
sfpi_inline __vCond operator<(const float a, const vFloat b) { return b > a; }
sfpi_inline __vCond operator<=(const float a, const vFloat b) { return b >= a; }
sfpi_inline __vCond operator>(const float a, const vFloat b) { return b < a; }
sfpi_inline __vCond operator>=(const float a, const vFloat b) { return b <= a; }
sfpi_inline __vCond operator==(const s2vFloat16 a, const vFloat b) { return b == a; }
sfpi_inline __vCond operator!=(const s2vFloat16 a, const vFloat b) { return b != a; }
sfpi_inline __vCond operator<(const s2vFloat16 a, const vFloat b) { return b > a; }
sfpi_inline __vCond operator<=(const s2vFloat16 a, const vFloat b) { return b >= a; }
sfpi_inline __vCond operator>(const s2vFloat16 a, const vFloat b) { return b < a; }
sfpi_inline __vCond operator>=(const s2vFloat16 a, const vFloat b) { return b <= a; }

sfpi_inline vInt operator+(int32_t a, vInt b) { return b + a; }
sfpi_inline vInt operator-(int32_t a, vInt b) { return vInt (a) - b; }
//sfpi_inline vInt operator+(const int32_t a, const vInt b) { return b + a; }
//sfpi_inline vInt operator-(const int32_t a, const vInt b) { return vInt(a) - b; }
sfpi_inline vInt operator&(const int32_t a, const vInt b) { return b & a; }
sfpi_inline vInt operator|(const int32_t a, const vInt b) { return b | a; }
sfpi_inline vInt operator^(const int32_t a, const vInt b) { return b ^ a; }
sfpi_inline vUInt operator+(const int32_t a, const vUInt b) { return b + a; }
sfpi_inline vUInt operator-(const int32_t a, const vUInt b) { return vUInt(a) - b; }
sfpi_inline vUInt operator&(const int32_t a, const vUInt b) { return b & a; }
sfpi_inline vUInt operator|(const int32_t a, const vUInt b) { return b | a; }
sfpi_inline vUInt operator^(const int32_t a, const vUInt b) { return b ^ a; }

sfpi_inline vUInt operator&(vInt a, const vUInt b) { return a & vInt (b); }
sfpi_inline vUInt operator|(vInt a, const vUInt b) { return a | vInt (b); }
sfpi_inline vUInt operator^(vInt a, const vUInt b) { return a ^ vInt (b); }

sfpi_inline vUInt operator&(vUInt a, const vInt b) { return a & vUInt (b); }
sfpi_inline vUInt operator|(vUInt a, const vInt b) { return a | vUInt (b); }
sfpi_inline vUInt operator^(vUInt a, const vInt b) { return a ^ vUInt (b); }

//////////////////////////////////////////////////////////////////////////////

sfpi_inline vInt::vInt (vUInt val)
    : vVal (val.get ()) {
}
sfpi_inline vInt::vInt(const __vCond vc)
{
  v = __builtin_rvtt_sfpxcondi (vc.get());
    initialized = true;
}

sfpi_inline  __vCond operator== (vInt a, vInt b) { return __vCond (__vCond::__vCondEQ, b, a, SFPXIADD_MOD1_SIGNED); }
sfpi_inline  __vCond operator!= (vInt a, vInt b) { return __vCond (__vCond::__vCondNE, b, a, SFPXIADD_MOD1_SIGNED); }
sfpi_inline  __vCond operator< (vInt a, vInt b) { return __vCond (__vCond::__vCondLT, b, a, SFPXIADD_MOD1_SIGNED); }
sfpi_inline  __vCond operator> (vInt a, vInt b) { return __vCond (__vCond::__vCondGT, b, a, SFPXIADD_MOD1_SIGNED); }
sfpi_inline  __vCond operator<= (vInt a, vInt b) { return __vCond (__vCond::__vCondLTE, b, a, SFPXIADD_MOD1_SIGNED); }
sfpi_inline  __vCond operator>= (vInt a, vInt b) { return __vCond (__vCond::__vCondGTE, b, a, SFPXIADD_MOD1_SIGNED); }

// FIXME: Until we get sfpxloadi optimization into sfpxfcmp, special case these compares
sfpi_inline  __vCond operator== (vInt a, int32_t b) { return __vCond(__vCond::__vCondEQ, a, b, SFPXIADD_MOD1_SIGNED); }
sfpi_inline  __vCond operator!= (vInt a, int32_t b) { return __vCond(__vCond::__vCondNE, a, b, SFPXIADD_MOD1_SIGNED); }
sfpi_inline  __vCond operator< (vInt a, int32_t b) { return __vCond(__vCond::__vCondLT, a, b, SFPXIADD_MOD1_SIGNED); }
sfpi_inline  __vCond operator> (vInt a, int32_t b) { return __vCond(__vCond::__vCondGT, a, b, SFPXIADD_MOD1_SIGNED); }
sfpi_inline  __vCond operator<= (vInt a, int32_t b) { return __vCond(__vCond::__vCondLTE, a, b, SFPXIADD_MOD1_SIGNED); }
sfpi_inline  __vCond operator>= (vInt a, int32_t b) { return __vCond(__vCond::__vCondGTE, a, b, SFPXIADD_MOD1_SIGNED); }

sfpi_inline  __vCond operator== (vInt a, int b) { return a == int32_t (b); }
sfpi_inline  __vCond operator!= (vInt a, int b) { return a != int32_t (b); }
sfpi_inline  __vCond operator< (vInt a, int b) { return a < int32_t (b); }
sfpi_inline  __vCond operator> (vInt a, int b) { return a > int32_t (b); }
sfpi_inline  __vCond operator<= (vInt a, int b) { return a <= int32_t (b); }
sfpi_inline  __vCond operator>= (vInt a, int b) { return a >= int32_t (b); }

// FIXME: These should be deprecated and removed -- mixing signed and unsigned
// in compares is not sensible. Sadly user code does this because the old
// iplementatiion permitted it :(
sfpi_inline  __vCond operator== (vInt a, vUInt b) { return a == vInt (b); }
sfpi_inline  __vCond operator!= (vInt a, vUInt b) { return a != vInt (b); }
sfpi_inline  __vCond operator< (vInt a, vUInt b) { return a < vInt (b); }
sfpi_inline  __vCond operator> (vInt a, vUInt b) { return a > vInt (b); }
sfpi_inline  __vCond operator<= (vInt a, vUInt b) { return a <= vInt (b); }
sfpi_inline  __vCond operator>= (vInt a, vUInt b) { return a >= vInt (b); }

sfpi_inline  __vCond operator== (vInt a, uint32_t b) { return a == int32_t (b); }
sfpi_inline  __vCond operator!= (vInt a, uint32_t b) { return a != int32_t (b); }
sfpi_inline  __vCond operator< (vInt a, uint32_t b) { return a < int32_t (b); }
sfpi_inline  __vCond operator> (vInt a, uint32_t b) { return a > int32_t (b); }
sfpi_inline  __vCond operator<= (vInt a, uint32_t b) { return a <= int32_t (b); }
sfpi_inline  __vCond operator>= (vInt a, uint32_t b) { return a >= int32_t (b); }

sfpi_inline  __vCond operator== (vInt a, unsigned b) { return a == uint32_t (b); }
sfpi_inline  __vCond operator!= (vInt a, unsigned b) { return a != uint32_t (b); }
sfpi_inline  __vCond operator< (vInt a, unsigned b) { return a < uint32_t (b); }
sfpi_inline  __vCond operator> (vInt a, unsigned b) { return a > uint32_t (b); }
sfpi_inline  __vCond operator<= (vInt a, unsigned b) { return a <= uint32_t (b); }
sfpi_inline  __vCond operator>= (vInt a, unsigned b) { return a >= uint32_t (b); }

//////////////////////////////////////////////////////////////////////////////
sfpi_inline vUInt::vUInt (vInt val)
    : vVal (val.get ()) {
}

sfpi_inline vUInt::vUInt(const __vCond vc)
{
    v = __builtin_rvtt_sfpxcondi(vc.get());
    initialized = true;
}

sfpi_inline  __vCond operator== (vUInt a, vUInt b) { return __vCond (__vCond::__vCondEQ, b, a, 0); }
sfpi_inline  __vCond operator!= (vUInt a, vUInt b) { return __vCond (__vCond::__vCondNE, b, a, 0); }
sfpi_inline  __vCond operator< (vUInt a, vUInt b) { return __vCond (__vCond::__vCondLT, b, a, 0); }
sfpi_inline  __vCond operator> (vUInt a, vUInt b) { return __vCond (__vCond::__vCondGT, b, a, 0); }
sfpi_inline  __vCond operator<= (vUInt a, vUInt b) { return __vCond (__vCond::__vCondLTE, b, a, 0); }
sfpi_inline  __vCond operator>= (vUInt a, vUInt b) { return __vCond (__vCond::__vCondGTE, b, a, 0); }

// FIXME: Until we get sfpxloadi optimization into sfpxfcmp, special case these compares
sfpi_inline  __vCond operator== (vUInt a, uint32_t b) { return __vCond(__vCond::__vCondEQ, a, b, 0); }
sfpi_inline  __vCond operator!= (vUInt a, uint32_t b) { return __vCond(__vCond::__vCondNE, a, b, 0); }
sfpi_inline  __vCond operator< (vUInt a, uint32_t b) { return __vCond(__vCond::__vCondLT, a, b, 0); }
sfpi_inline  __vCond operator> (vUInt a, uint32_t b) { return  __vCond(__vCond::__vCondGT, a, b, 0); }
sfpi_inline  __vCond operator<= (vUInt a, uint32_t b) { return  __vCond(__vCond::__vCondLTE, a, b, 0); }
sfpi_inline  __vCond operator>= (vUInt a, uint32_t b) { return __vCond(__vCond::__vCondGTE, a, b, 0); }

sfpi_inline  __vCond operator== (vUInt a, unsigned b) { return a == uint32_t (b); }
sfpi_inline  __vCond operator!= (vUInt a, unsigned b) { return a != uint32_t (b); }
sfpi_inline  __vCond operator< (vUInt a, unsigned b) { return a < uint32_t (b); }
sfpi_inline  __vCond operator> (vUInt a, unsigned b) { return a > uint32_t (b); }
sfpi_inline  __vCond operator<= (vUInt a, unsigned b) { return a <= uint32_t (b); }
sfpi_inline  __vCond operator>= (vUInt a, unsigned b) { return a >= uint32_t (b); }

sfpi_inline  __vCond operator== (vUInt a, int b) { return a == uint32_t (b); }
sfpi_inline  __vCond operator!= (vUInt a, int b) { return a != uint32_t (b); }
sfpi_inline  __vCond operator< (vUInt a, int b) { return a < uint32_t (b); }
sfpi_inline  __vCond operator> (vUInt a, int b) { return a > uint32_t (b); }
sfpi_inline  __vCond operator<= (vUInt a, int b) { return a <= uint32_t (b); }
sfpi_inline  __vCond operator>= (vUInt a, int b) { return a >= uint32_t (b); }

//////////////////////////////////////////////////////////////////////////////
sfpi_inline __vCCCtrl &__vCCCtrl::cc_if()
{
    dep = __builtin_rvtt_sfpxvif();
    return *this;
}

sfpi_inline void __vCCCtrl::cc_cond(const __vCond& op)
{
    __builtin_rvtt_sfpxcondb(op.get(), dep);
}

sfpi_inline void __vCCCtrl::cc_cond(vInt v) {
    __builtin_rvtt_sfpxcondb(__vCond(__vCond::__vCondNE, v, 0, 0).get(), dep);
}
sfpi_inline void __vCCCtrl::cc_cond(vUInt v) {
    __builtin_rvtt_sfpxcondb(__vCond(__vCond::__vCondNE, v, 0, 0).get(), dep);
}

sfpi_inline __vCCCtrl &__vCCCtrl::cc_else()
{
    __builtin_rvtt_sfpcompc();
    return *this;    
}

sfpi_inline __vCCCtrl &__vCCCtrl::cc_push()
 {
   depth++;
   __builtin_rvtt_sfppushc(SFPPUSHC_MOD1_PUSH);
   return *this;
 }

sfpi_inline __vCCCtrl &__vCCCtrl::cc_pop()
 {
   for (unsigned ix = depth; ix--;)
      __builtin_rvtt_sfppopc(SFPPOPC_MOD1_POP);
   return *this;
 }

sfpi_inline __vCCCtrl::__vCCCtrl(__vCCCtrl &&src)
    : dep (src.dep), depth (src.depth)
{
  src.dep = 0;
  src.depth = 0;
}

sfpi_inline __vCCCtrl::~__vCCCtrl()
{
  cc_pop();
}

sfpi_inline __vCCCtrl &__vCCCtrl::operator=(__vCCCtrl &&src)
{
  cc_pop();
  dep = src.dep, depth = src.depth;
  src.dep = 0, src.depth = 0;
  return *this;
}

sfpi_inline void __vCCCtrl::enable_cc()
{
    __builtin_rvtt_sfpencc(SFPENCC_MOD1_EI_RI, SFPENCC_IMM12_BOTH);
}

//////////////////////////////////////////////////////////////////////////////
constexpr impl_::vDReg::DestRegFile dst_reg;
constexpr impl_::vLReg::LRegFile l_reg;

//////////////////////////////////////////////////////////////////////////////
// C++17: In a function-call expression, the expression that names the function
// is sequenced before every argument expression and every default argument. [expr.pre]

    
#define v_if(x)                                 \
  { sfpi::__vCCCtrl __cc;                       \
  __cc.cc_push().cc_if().cc_cond(x); {

#define v_elseif(x)    }                        \
  __cc.cc_else().cc_push().cc_if().cc_cond(x); {

#define v_else         }                        \
  __cc.cc_else(); {

#define v_endif        }                        \
  }

#define v_block                                 \
  { sfpi::__vCCCtrl __cc;                       \
  __cc.cc_push();

#define v_and(x)                                \
  __cc.cc_if().cc_cond(x)

#define v_endblock                              \
  }

//////////////////////////////////////////////////////////////////////////////
constexpr impl_::vConst<vFloat> vConst0p8373 (CREG_IDX_0P837300003);
constexpr impl_::vConst<vFloat> vConstFloatPrgm0 (CREG_IDX_PRGM1);
constexpr impl_::vConst<vFloat> vConstFloatPrgm1 (CREG_IDX_PRGM2);
constexpr impl_::vConst<vFloat> vConstFloatPrgm2 (CREG_IDX_PRGM3);

constexpr impl_::vConst<vInt> vConstTileId (CREG_IDX_TILEID);
constexpr impl_::vConst<vInt> vConstIntPrgm0 (CREG_IDX_PRGM1);
constexpr impl_::vConst<vInt> vConstIntPrgm1 (CREG_IDX_PRGM2);
constexpr impl_::vConst<vInt> vConstIntPrgm2 (CREG_IDX_PRGM3);

//////////////////////////////////////////////////////////////////////////////
sfpi_inline __vCond impl_::vDReg::operator==(const float x) const {return __vCond(__vCond::__vCondEQ, vFloat(*this), x); }
sfpi_inline __vCond impl_::vDReg::operator!=(const float x) const { return __vCond(__vCond::__vCondNE, vFloat(*this), x); }
sfpi_inline __vCond impl_::vDReg::operator<(const float x) const { return __vCond(__vCond::__vCondLT, vFloat(*this), x); }
sfpi_inline __vCond impl_::vDReg::operator<=(const float x) const { return __vCond(__vCond::__vCondLTE, vFloat(*this), x); }
sfpi_inline __vCond impl_::vDReg::operator>(const float x) const { return __vCond(__vCond::__vCondGT, vFloat(*this), x); }
sfpi_inline __vCond impl_::vDReg::operator>=(const float x) const { return __vCond(__vCond::__vCondGTE, vFloat(*this), x); }

sfpi_inline vFloat impl_::vDReg::operator=(vFloat vec) const
{
    __builtin_rvtt_sfpstore(vec.get(), reg, SFPSTORE_MOD0_FMT_SRCB, SFPSTORE_ADDR_MODE_NOINC);
    return vec;
}

sfpi_inline vFloat impl_::vDReg::operator=(const double d) const
{
    vFloat v(static_cast<float>(d));
    *this = v;
    return v;
}

sfpi_inline vFloat impl_::vDReg::operator=(s2vFloat16 f) const
{
    vFloat v(f);
    *this = v;
    return v;
}

sfpi_inline vFloat impl_::vDReg::operator=(const float f) const
{
    vFloat v(f);
    *this = v;
    return v;
}

template <typename vecType, typename std::enable_if_t<std::is_base_of<impl_::vVal, vecType>::value>*>
sfpi_inline vecType impl_::vDReg::operator=(const vecType vec) const
{
    auto mod = SFPSTORE_MOD0_FMT_BOB32;
#if __riscv_xtttensixwh
    if constexpr (std::is_base_of<vInt, vecType>::value)
        mod = SFPSTORE_MOD0_FMT_SM32;
#endif
    __builtin_rvtt_sfpstore(vec.get(), reg, mod, SFPSTORE_ADDR_MODE_NOINC);
    return vec;
}

sfpi_inline void impl_::vDReg::operator=(const impl_::vDReg dreg) const
{
    vFloat tmp = dreg;
    __builtin_rvtt_sfpstore(tmp.get(), reg, SFPSTORE_MOD0_FMT_SRCB, SFPSTORE_ADDR_MODE_NOINC);
}

//////////////////////////////////////////////////////////////////////////////

} // namespace sfpi

//////////////////////////////////////////////////////////////////////////////
#include "sfpi_lib.h"
