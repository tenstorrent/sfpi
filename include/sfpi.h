/* -*- C++ -*-
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

#include <cstdint>
#include <type_traits>
#include "sfpi_classes.h"

namespace sfpi {

//////////////////////////////////////////////////////////////////////////////

// User-accessible vector float type
class vFloat : public impl_::vVal {
public:
  vFloat () = default;
  sfpi_inline vFloat (impl_::sfpu_t);
  sfpi_inline vFloat (impl_::vLReg);
  sfpi_inline vFloat (impl_::vDReg);
  sfpi_inline vFloat (s2vFloat16a);
  sfpi_inline vFloat (s2vFloat16b);
  sfpi_inline vFloat (float);
  
  sfpi_inline vFloat &operator= (vFloat);
  sfpi_inline vFloat &operator= (impl_::vLReg);
  
  sfpi_inline vFloat &operator+= (vFloat);
  sfpi_inline vFloat &operator-= (vFloat);
  sfpi_inline vFloat &operator*= (vFloat);

  sfpi_inline vFloat operator++ (int);
  sfpi_inline vFloat operator++ ();
  sfpi_inline vFloat operator-- (int);
  sfpi_inline vFloat operator-- ();
};

sfpi_inline vFloat operator+ (vFloat);
sfpi_inline vFloat operator- (vFloat);

sfpi_inline vFloat operator+ (vFloat, vFloat);
sfpi_inline vFloat operator- (vFloat, vFloat);
sfpi_inline vFloat operator* (vFloat, vFloat);

sfpi_inline vFloat operator+ (float, vFloat);
sfpi_inline vFloat operator- (vFloat, float);
sfpi_inline vFloat operator- (float, vFloat);
sfpi_inline vFloat operator* (float, vFloat);

//////////////////////////////////////////////////////////////////////////////
class vInt : public impl_::vVal {
public:
  vInt () = default;
  sfpi_inline vInt (impl_::sfpu_t);
  sfpi_inline vInt (impl_::vLReg);
  sfpi_inline vInt (impl_::vDReg);
  sfpi_inline vInt (vUInt);
  sfpi_inline vInt (int16_t);
  sfpi_inline vInt (uint16_t);
  sfpi_inline vInt (int32_t);
  sfpi_inline vInt (uint32_t);
  sfpi_inline vInt (int);
  sfpi_inline vInt (unsigned);
  sfpi_inline vInt (impl_::vCond);

  sfpi_inline vInt &operator= (vInt);
  sfpi_inline vInt &operator= (impl_::vLReg);

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

  sfpi_inline vInt operator++ ();
  sfpi_inline vInt operator++ (int);
  sfpi_inline vInt operator-- ();
  sfpi_inline vInt operator-- (int);
};

sfpi_inline vInt operator+ (vInt);
sfpi_inline vInt operator- (vInt);
sfpi_inline vInt operator~ (vInt);

sfpi_inline vInt operator+ (vInt, vInt);
sfpi_inline vInt operator- (vInt, vInt);

sfpi_inline vInt operator<< (vInt, vUInt);
#if __riscv_xtttensixbh
// arithmetic shifts added in blackhole
sfpi_inline vInt operator>> (vInt, vUInt);
#endif
sfpi_inline vInt operator& (vInt, vInt);
sfpi_inline vInt operator| (vInt, vInt);
sfpi_inline vInt operator^ (vInt, vInt);

sfpi_inline vInt operator<< (vInt, unsigned);
#if __riscv_xtttensixbh
// arithmetic shifts added in blackhole
sfpi_inline vInt operator>> (vInt, unsigned);
#endif

sfpi_inline vInt operator+ (vInt, int32_t);
sfpi_inline vInt operator- (vInt, int32_t);
sfpi_inline vInt operator& (vInt, int32_t);
sfpi_inline vInt operator| (vInt, int32_t);
sfpi_inline vInt operator^ (vInt, int32_t);

sfpi_inline vInt operator& (vInt, int);
sfpi_inline vInt operator| (vInt, int);
sfpi_inline vInt operator^ (vInt, int);

sfpi_inline vInt operator& (vInt, unsigned);
sfpi_inline vInt operator| (vInt, unsigned);
sfpi_inline vInt operator^ (vInt, unsigned);

sfpi_inline vInt operator+ (int32_t a, vInt b) { return b + a; }
sfpi_inline vInt operator- (int32_t a, vInt b) { return vInt (a) - b; }
sfpi_inline vInt operator& (int32_t a, vInt b) { return b & a; }
sfpi_inline vInt operator| (int32_t a, vInt b) { return b | a; }
sfpi_inline vInt operator^ (int32_t a, vInt b) { return b ^ a; }

//////////////////////////////////////////////////////////////////////////////
class vUInt : public impl_::vVal {
public:
  vUInt () = default;
  sfpi_inline vUInt (impl_::sfpu_t vec);
  sfpi_inline vUInt (impl_::vLReg lr);
  sfpi_inline vUInt (impl_::vDReg dreg);
  sfpi_inline vUInt (vInt val);
  sfpi_inline vUInt (int16_t val);
  sfpi_inline vUInt (uint16_t val);
  sfpi_inline vUInt (int32_t val);
  sfpi_inline vUInt (uint32_t val);
  sfpi_inline vUInt (int val);
  sfpi_inline vUInt (unsigned val);

  sfpi_inline vUInt (impl_::vCond vc);

  // Assignment
  sfpi_inline vUInt &operator= (vUInt); 
  sfpi_inline vUInt &operator= (impl_::vLReg);

  // Operations
  sfpi_inline vUInt operator++ ();
  sfpi_inline vUInt operator-- ();

  sfpi_inline vUInt operator++ (int);
  sfpi_inline vUInt operator-- (int);

  sfpi_inline vUInt &operator+= (vUInt);
  sfpi_inline vUInt &operator-= (vUInt);
  sfpi_inline vUInt &operator<<= (vUInt);
  sfpi_inline vUInt &operator>>= (vUInt);
  sfpi_inline vUInt &operator&= (vUInt);
  sfpi_inline vUInt &operator|= (vUInt);
  sfpi_inline vUInt &operator^= (vUInt);

  sfpi_inline vUInt &operator<<= (unsigned);
  sfpi_inline vUInt &operator>>= (unsigned);
};

sfpi_inline vUInt operator+ (vUInt);
sfpi_inline vUInt operator- (vUInt);
sfpi_inline vUInt operator~ (vUInt);

sfpi_inline vUInt operator+ (vUInt, vUInt);
sfpi_inline vUInt operator- (vUInt, vUInt);
sfpi_inline vUInt operator<< (vUInt, vUInt);
sfpi_inline vUInt operator>> (vUInt, vUInt);
sfpi_inline vUInt operator& (vUInt, vUInt);
sfpi_inline vUInt operator| (vUInt, vUInt);
sfpi_inline vUInt operator^ (vUInt, vUInt);

sfpi_inline vUInt operator<< (vUInt, unsigned);
sfpi_inline vUInt operator>> (vUInt, unsigned);

sfpi_inline vUInt operator+ (vUInt, int32_t);
sfpi_inline vUInt operator- (vUInt, int32_t);

sfpi_inline vUInt operator& (vUInt, uint32_t);
sfpi_inline vUInt operator| (vUInt, uint32_t);
sfpi_inline vUInt operator^ (vUInt, uint32_t);

sfpi_inline vUInt operator- (vUInt, vInt);
sfpi_inline vUInt operator& (vUInt, vInt);
sfpi_inline vUInt operator| (vUInt, vInt);
sfpi_inline vUInt operator^ (vUInt, vInt);

sfpi_inline vUInt operator& (vInt, vUInt);
sfpi_inline vUInt operator| (vInt, vUInt);
sfpi_inline vUInt operator^ (vInt, vUInt);

sfpi_inline vUInt operator& (vUInt, unsigned);
sfpi_inline vUInt operator| (vUInt, unsigned);
sfpi_inline vUInt operator^ (vUInt, unsigned);

sfpi_inline vUInt operator& (vUInt, int);
sfpi_inline vUInt operator| (vUInt, int);
sfpi_inline vUInt operator^ (vUInt, int);

sfpi_inline vUInt operator+ (int32_t a, vUInt b) { return b + a; }
sfpi_inline vUInt operator- (int32_t a, vUInt b) { return vUInt (a) - b; }
sfpi_inline vUInt operator& (int32_t a, vUInt b) { return b & uint32_t (a); }
sfpi_inline vUInt operator| (int32_t a, vUInt b) { return b | uint32_t (a); }
sfpi_inline vUInt operator^ (int32_t a, vUInt b) { return b ^ uint32_t (a); }

//////////////////////////////////////////////////////////////////////////////
// Comparisons
sfpi_inline impl_::vCond operator== (vFloat a, vFloat b) { return impl_::vCond (impl_::vCond::EQ, a, b); }
sfpi_inline impl_::vCond operator!= (vFloat a, vFloat b) { return impl_::vCond (impl_::vCond::NE, a, b); }
sfpi_inline impl_::vCond operator< (vFloat a, vFloat b) { return impl_::vCond (impl_::vCond::LT, a, b); }
sfpi_inline impl_::vCond operator> (vFloat a, vFloat b) { return impl_::vCond (impl_::vCond::GT, a, b); }
sfpi_inline impl_::vCond operator<= (vFloat a, vFloat b) { return impl_::vCond (impl_::vCond::LTE, a, b); }
sfpi_inline impl_::vCond operator>= (vFloat a, vFloat b) { return impl_::vCond (impl_::vCond::GTE, a, b); }

// FIXME: Until we get sfpxloadi optimization into sfpxfcmp, special case these compares
sfpi_inline impl_::vCond operator== (vFloat a, float b) { return impl_::vCond (impl_::vCond::EQ, a, b); }
sfpi_inline impl_::vCond operator!= (vFloat a, float b) { return impl_::vCond (impl_::vCond::NE, a, b); }
sfpi_inline impl_::vCond operator< (vFloat a, float b) { return impl_::vCond (impl_::vCond::LT, a, b); }
sfpi_inline impl_::vCond operator> (vFloat a, float b) { return impl_::vCond (impl_::vCond::GT, a, b); }
sfpi_inline impl_::vCond operator<= (vFloat a, float b) { return impl_::vCond (impl_::vCond::LTE, a, b); }
sfpi_inline impl_::vCond operator>= (vFloat a, float b) { return impl_::vCond (impl_::vCond::GTE, a, b); }

//////////////////////////////////////////////////////////////////////////////
sfpi_inline impl_::vCond operator== (float a, vFloat b) { return b == a; }
sfpi_inline impl_::vCond operator!= (float a, vFloat b) { return b != a; }
sfpi_inline impl_::vCond operator< (float a, vFloat b) { return b > a; }
sfpi_inline impl_::vCond operator> (float a, vFloat b) { return b < a; }
sfpi_inline impl_::vCond operator<= (float a, vFloat b) { return b >= a; }
sfpi_inline impl_::vCond operator>= (float a, vFloat b) { return b <= a; }

#if 0
sfpi_inline impl_::vCond operator== (s2vFloat16 a, vFloat b) { return b == a; }
sfpi_inline impl_::vCond operator!= (s2vFloat16 a, vFloat b) { return b != a; }
sfpi_inline impl_::vCond operator< (s2vFloat16 a, vFloat b) { return b > a; }
sfpi_inline impl_::vCond operator> (s2vFloat16 a, vFloat b) { return b < a; }
sfpi_inline impl_::vCond operator<= (s2vFloat16 a, vFloat b) { return b >= a; }
sfpi_inline impl_::vCond operator>= (s2vFloat16 a, vFloat b) { return b <= a; }
#endif
//////////////////////////////////////////////////////////////////////////////

sfpi_inline  impl_::vCond operator== (vInt a, vInt b) { return impl_::vCond (impl_::vCond::EQ, b, a, SFPXIADD_MOD1_SIGNED); }
sfpi_inline  impl_::vCond operator!= (vInt a, vInt b) { return impl_::vCond (impl_::vCond::NE, b, a, SFPXIADD_MOD1_SIGNED); }
sfpi_inline  impl_::vCond operator< (vInt a, vInt b) { return impl_::vCond (impl_::vCond::LT, b, a, SFPXIADD_MOD1_SIGNED); }
sfpi_inline  impl_::vCond operator> (vInt a, vInt b) { return impl_::vCond (impl_::vCond::GT, b, a, SFPXIADD_MOD1_SIGNED); }
sfpi_inline  impl_::vCond operator<= (vInt a, vInt b) { return impl_::vCond (impl_::vCond::LTE, b, a, SFPXIADD_MOD1_SIGNED); }
sfpi_inline  impl_::vCond operator>= (vInt a, vInt b) { return impl_::vCond (impl_::vCond::GTE, b, a, SFPXIADD_MOD1_SIGNED); }

// FIXME: Until we get sfpxloadi optimization into sfpxfcmp, special case these compares
sfpi_inline  impl_::vCond operator== (vInt a, int32_t b) { return impl_::vCond (impl_::vCond::EQ, a, b, SFPXIADD_MOD1_SIGNED); }
sfpi_inline  impl_::vCond operator!= (vInt a, int32_t b) { return impl_::vCond (impl_::vCond::NE, a, b, SFPXIADD_MOD1_SIGNED); }
sfpi_inline  impl_::vCond operator< (vInt a, int32_t b) { return impl_::vCond (impl_::vCond::LT, a, b, SFPXIADD_MOD1_SIGNED); }
sfpi_inline  impl_::vCond operator> (vInt a, int32_t b) { return impl_::vCond (impl_::vCond::GT, a, b, SFPXIADD_MOD1_SIGNED); }
sfpi_inline  impl_::vCond operator<= (vInt a, int32_t b) { return impl_::vCond (impl_::vCond::LTE, a, b, SFPXIADD_MOD1_SIGNED); }
sfpi_inline  impl_::vCond operator>= (vInt a, int32_t b) { return impl_::vCond (impl_::vCond::GTE, a, b, SFPXIADD_MOD1_SIGNED); }

sfpi_inline  impl_::vCond operator== (vInt a, int b) { return a == int32_t (b); }
sfpi_inline  impl_::vCond operator!= (vInt a, int b) { return a != int32_t (b); }
sfpi_inline  impl_::vCond operator< (vInt a, int b) { return a < int32_t (b); }
sfpi_inline  impl_::vCond operator> (vInt a, int b) { return a > int32_t (b); }
sfpi_inline  impl_::vCond operator<= (vInt a, int b) { return a <= int32_t (b); }
sfpi_inline  impl_::vCond operator>= (vInt a, int b) { return a >= int32_t (b); }

// FIXME: These should be deprecated and removed -- mixing signed and unsigned
// in compares is not sensible. Sadly user code does this because the old
// iplementatiion permitted it :(
sfpi_inline  impl_::vCond operator== (vInt a, vUInt b) { return a == vInt (b); }
sfpi_inline  impl_::vCond operator!= (vInt a, vUInt b) { return a != vInt (b); }
sfpi_inline  impl_::vCond operator< (vInt a, vUInt b) { return a < vInt (b); }
sfpi_inline  impl_::vCond operator> (vInt a, vUInt b) { return a > vInt (b); }
sfpi_inline  impl_::vCond operator<= (vInt a, vUInt b) { return a <= vInt (b); }
sfpi_inline  impl_::vCond operator>= (vInt a, vUInt b) { return a >= vInt (b); }

sfpi_inline  impl_::vCond operator== (vInt a, uint32_t b) { return a == int32_t (b); }
sfpi_inline  impl_::vCond operator!= (vInt a, uint32_t b) { return a != int32_t (b); }
sfpi_inline  impl_::vCond operator< (vInt a, uint32_t b) { return a < int32_t (b); }
sfpi_inline  impl_::vCond operator> (vInt a, uint32_t b) { return a > int32_t (b); }
sfpi_inline  impl_::vCond operator<= (vInt a, uint32_t b) { return a <= int32_t (b); }
sfpi_inline  impl_::vCond operator>= (vInt a, uint32_t b) { return a >= int32_t (b); }

sfpi_inline  impl_::vCond operator== (vInt a, unsigned b) { return a == uint32_t (b); }
sfpi_inline  impl_::vCond operator!= (vInt a, unsigned b) { return a != uint32_t (b); }
sfpi_inline  impl_::vCond operator< (vInt a, unsigned b) { return a < uint32_t (b); }
sfpi_inline  impl_::vCond operator> (vInt a, unsigned b) { return a > uint32_t (b); }
sfpi_inline  impl_::vCond operator<= (vInt a, unsigned b) { return a <= uint32_t (b); }
sfpi_inline  impl_::vCond operator>= (vInt a, unsigned b) { return a >= uint32_t (b); }

//////////////////////////////////////////////////////////////////////////////

sfpi_inline  impl_::vCond operator== (vUInt a, vUInt b) { return impl_::vCond (impl_::vCond::EQ, b, a, 0); }
sfpi_inline  impl_::vCond operator!= (vUInt a, vUInt b) { return impl_::vCond (impl_::vCond::NE, b, a, 0); }
sfpi_inline  impl_::vCond operator< (vUInt a, vUInt b) { return impl_::vCond (impl_::vCond::LT, b, a, 0); }
sfpi_inline  impl_::vCond operator> (vUInt a, vUInt b) { return impl_::vCond (impl_::vCond::GT, b, a, 0); }
sfpi_inline  impl_::vCond operator<= (vUInt a, vUInt b) { return impl_::vCond (impl_::vCond::LTE, b, a, 0); }
sfpi_inline  impl_::vCond operator>= (vUInt a, vUInt b) { return impl_::vCond (impl_::vCond::GTE, b, a, 0); }

// FIXME: Until we get sfpxloadi optimization into sfpxfcmp, special case these compares
sfpi_inline  impl_::vCond operator== (vUInt a, uint32_t b) { return impl_::vCond (impl_::vCond::EQ, a, b, 0); }
sfpi_inline  impl_::vCond operator!= (vUInt a, uint32_t b) { return impl_::vCond (impl_::vCond::NE, a, b, 0); }
sfpi_inline  impl_::vCond operator< (vUInt a, uint32_t b) { return impl_::vCond (impl_::vCond::LT, a, b, 0); }
sfpi_inline  impl_::vCond operator> (vUInt a, uint32_t b) { return impl_::vCond (impl_::vCond::GT, a, b, 0); }
sfpi_inline  impl_::vCond operator<= (vUInt a, uint32_t b) { return impl_::vCond (impl_::vCond::LTE, a, b, 0); }
sfpi_inline  impl_::vCond operator>= (vUInt a, uint32_t b) { return impl_::vCond (impl_::vCond::GTE, a, b, 0); }

sfpi_inline  impl_::vCond operator== (vUInt a, unsigned b) { return a == uint32_t (b); }
sfpi_inline  impl_::vCond operator!= (vUInt a, unsigned b) { return a != uint32_t (b); }
sfpi_inline  impl_::vCond operator< (vUInt a, unsigned b) { return a < uint32_t (b); }
sfpi_inline  impl_::vCond operator> (vUInt a, unsigned b) { return a > uint32_t (b); }
sfpi_inline  impl_::vCond operator<= (vUInt a, unsigned b) { return a <= uint32_t (b); }
sfpi_inline  impl_::vCond operator>= (vUInt a, unsigned b) { return a >= uint32_t (b); }

sfpi_inline  impl_::vCond operator== (vUInt a, int b) { return a == uint32_t (b); }
sfpi_inline  impl_::vCond operator!= (vUInt a, int b) { return a != uint32_t (b); }
sfpi_inline  impl_::vCond operator< (vUInt a, int b) { return a < uint32_t (b); }
sfpi_inline  impl_::vCond operator> (vUInt a, int b) { return a > uint32_t (b); }
sfpi_inline  impl_::vCond operator<= (vUInt a, int b) { return a <= uint32_t (b); }
sfpi_inline  impl_::vCond operator>= (vUInt a, int b) { return a >= uint32_t (b); }

//////////////////////////////////////////////////////////////////////////////
// C++17: In a function-call expression, the expression that names the function
// is sequenced before every argument expression and every default argument. [expr.pre]

#define v_if(x)                                 \
  { sfpi::impl_::vCond::CC __cc;                \
  __cc.push ().if_().cond (x);                  \
  {

#define v_elseif(x)                             \
  } __cc.else_().push().if_().cond (x); {

#define v_else                                  \
  } __cc.else_(); {

#define v_endif                                 \
  }                                             \
  }

#define v_block                                 \
  { sfpi::impl_::vCond::CC __cc;                \
  __cc.push ();

#define v_and(x)                                \
  __cc.if_().cond (x)

#define v_endblock                              \
  }

//////////////////////////////////////////////////////////////////////////////
// LRegs

enum class LRegs : uint8_t {
  LReg0, LReg1, LReg2, LReg3, LReg4, LReg5, LReg6, LReg7,
  LRegCount = SFP_LREG_COUNT,
};

constexpr impl_::vDReg::DestRegFile dst_reg;
constexpr impl_::vLReg::LRegFile l_reg;

//////////////////////////////////////////////////////////////////////////////
// User accessible float constants
constexpr impl_::vConst<vFloat> vConst0(CREG_IDX_0);
constexpr impl_::vConst<vFloat> vConst1(CREG_IDX_1);
constexpr impl_::vConst<vFloat> vConstNeg1(CREG_IDX_NEG_1);

constexpr impl_::vConst<vFloat> vConst0p8373 (CREG_IDX_0P837300003);
constexpr impl_::vConst<vFloat> vConstFloatPrgm0 (CREG_IDX_PRGM1);
constexpr impl_::vConst<vFloat> vConstFloatPrgm1 (CREG_IDX_PRGM2);
constexpr impl_::vConst<vFloat> vConstFloatPrgm2 (CREG_IDX_PRGM3);

constexpr impl_::vConst<vInt> vConstTileId (CREG_IDX_TILEID);
constexpr impl_::vConst<vInt> vConstIntPrgm0 (CREG_IDX_PRGM1);
constexpr impl_::vConst<vInt> vConstIntPrgm1 (CREG_IDX_PRGM2);
constexpr impl_::vConst<vInt> vConstIntPrgm2 (CREG_IDX_PRGM3);

//////////////////////////////////////////////////////////////////////////////

} // namespace sfpi

//////////////////////////////////////////////////////////////////////////////
#include "sfpi_funcs.h"
#include "sfpi_lib.h"
