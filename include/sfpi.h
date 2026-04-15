/* -*- C++ -*-
 * SPDX-FileCopyrightText: © 2023-2026 Tenstorrent Inc.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

///////////////////////////////////////////////////////////////////////////////
// sfpi.h: SFPu Interface
//
// This file provides a C++ wrapper around __builtin functions that map to SFPU
// instructions.  If you find yourself needing to use the builtins directly,
// (because the sfpi API is inadequate), file an issue explaining the problem.
//
// Vector types:
//   class vUInt - 32bit unsigned elements
//   class vInt  - 32bit signed (2's complement) elements
//   class vFloat- 32bit IEEE 754 floating point elements
//
//  These are held in the lregs and may be transfered to/from the dst regs (and
//  quasar srcs regs).  Overloaded operators are defined to permit such things as:
//     vFloat a, b, c, d;
//     vFloat d = a * b + c;
//
// Destination Register:
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
//   template<typename T> class impl_::vConst<T>
//   constexpr impl_::vConst<vFloat> vConst0p6928(5);
//
//   The constant value local registers are used in expressions by referencing
//   one of the names above and using them in mathematical operations such as:
//       a = b * c + vConst0p6928;
//
// Predicated Execution:
//   macros v_if(), v_elseif(), v_else, v_endif
//
//   The class __vCCCtrl is used in conjunction with the LReg based classes to
//   enable predicated execution.  By convention the test infastructure indents
//   the code as if executing if/then/else in C++, for example:
//     v_if (CONDITION)
//       True lanes affected here
//     v_elseif (CONDITION)
//       Now-true lanes affected
//     v_else
//       Never-true lanes affected
//     v_endif
//   Remember, because different vector lanes may be differently enabled,
//   control flow goes through each block of the conditional. Changing control
//   flow into or out of the conditional blocks is not permitted.
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
//  Undesired features

//    There are some undesired features that have made their way into the
//    implementation.  These are subject to deprecation and removal. The
//    primary issue is the mixing of signed and unsigned vector types. The end
//    goal is to have no mixed-signedness overloads and no implicit conversions
//    between the vector types
//
///////////////////////////////////////////////////////////////////////////////
#pragma once

#include <cstdint>
#include <type_traits>
#include "sfpi_classes.h"

static_assert (std::is_same_v<int32_t, long>, "int32_t is not expected type of long");
// Why am I checking int32_t here? Good question. The answer is analysis tools
// sometimes are misconfigured and pick 'int' as the type of int32_t.  But this
// is built for int32_t is long. It's not possible (without majorly invasive
// pretty-much untestable template jiggery pokery) to make this agnostic,
// because we need to deal with both integer literals and int32_t[*].
//
// The history here goes way back to when embedded systems were commonly 16-bit
// machines, and int was 16 bits. So long would be the natural type for
// int32_t.  Then some embedded machines became 32-bits. But libraries remained
// using long for int32_t, as that avoids changing the type of int32_t. Of
// course later C mandated int needed to be at least 32 bits, and all the
// 16-bit machines died, or when to the upstate pet farm. But anyway, riscv
// embedded ABIs have int32_t as long, not int, and we have to deal with it.
//
// [*] It might be possible once we've removed the library's promiscuity in
// mixing vInt/vUInt types, so that there's only one valid promotion for an int
// type in the overload set. But we're not there yet.

namespace sfpi {

// Scalar float types, these are just containers. Used for initializing a
// vector. Initializing with an integral type is a bitcopy. Initializing with a
// float type is a rounding towards zero (which happens to also be a bit-copy).
class sFloat16a {
  uint16_t val;

public:
  // There is no float ctor, because that's a complex bit operation. We don't
  // want to accidentally do that at runtime (consteval fns not yet available
  // to us). Besides why would you want it?  (The compiler is quite capable of
  // spotting bit patterns that are suitable for loading as an float16a, rather
  // than lo/hi pair).

  // Reinterpret the bit pattern. Generally because you've computed something
  // dynamically. If you;re using these with literals, you're just obfuscating
  // your code.  
  sfpi_inline explicit sFloat16a (uint32_t v) : val (v) {}
  sfpi_inline explicit sFloat16a (int32_t v) : val (v) {}
  sfpi_inline explicit sFloat16a (unsigned v) : sFloat16a (uint32_t (v)) {}
  sfpi_inline explicit sFloat16a (signed v) : sFloat16a (int32_t (v)) {}

  sfpi_inline uint32_t get () const { return val; }
};

class sFloat16b {
  uint16_t val;

public:
  // This rounds to zero (by truncating the mantissa).  The only reason you'd
  // want to use this on a literal is to force it to be representable as a
  // float16b and therefore use a single load.  The compiler is quite capable
  // of spotting constant loads that can do that.
  sfpi_inline explicit sFloat16b (float f);

  // Reinterpret the bit pattern. Generally because you've computed something
  // dynamically. If you;re using these with literals, you're just obfuscating
  // your code.
  sfpi_inline explicit sFloat16b (int32_t v) : val (v) {}
  sfpi_inline explicit sFloat16b (uint32_t v) : val (v) {}
  sfpi_inline explicit sFloat16b (unsigned v) : sFloat16b (uint32_t (v)) {}
  sfpi_inline explicit sFloat16b (signed v) : sFloat16b (int32_t (v)) {}

  __SFPI_DEPRECATED ("convert to float first")   // This will removed
  sfpi_inline explicit sFloat16b (double f) : sFloat16b (float (f)) {}

public:
  sfpi_inline uint32_t get () const { return val; }
};

// Sadly typedefs deprecations are silent, so use a wrapper class
class __SFPI_DEPRECATED ("use sfpi::sFloat16a")   // This will removed
  s2vFloat16a  : public sFloat16a {
 public:
  using sFloat16a::sFloat16a;
};
class __SFPI_DEPRECATED ("use sfpi::sFloat16a")   // This will removed
  s2vFloat16b : public sFloat16b {
public:
  using sFloat16b::sFloat16b;
};

//////////////////////////////////////////////////////////////////////////////

class vFloat : public impl_::vVal {
public:
  sfpi_inline vFloat () = default;
  sfpi_inline vFloat (vFloat const &) = default;
  sfpi_inline vFloat &operator= (vFloat const &) = default;

public:
  sfpi_inline vFloat (impl_::sfpu_t);
  sfpi_inline vFloat (sFloat16a);
  sfpi_inline vFloat (sFloat16b);
  sfpi_inline vFloat (float);
  
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
  sfpi_inline vInt () = default;
  sfpi_inline vInt (vInt const &) = default;
  sfpi_inline vInt &operator= (vInt const &) = default;

public:
  sfpi_inline vInt (impl_::sfpu_t);
  sfpi_inline explicit vInt (vUInt);
  sfpi_inline vInt (int16_t);
  sfpi_inline vInt (uint16_t);
  sfpi_inline vInt (int32_t);
  sfpi_inline vInt (uint32_t);
  sfpi_inline vInt (int val) : vInt (int32_t (val)) {};
  sfpi_inline vInt (unsigned val) : vInt (uint32_t (val)) {}
  sfpi_inline vInt (impl_::vCond);

  sfpi_inline vInt &operator+= (vInt);
  sfpi_inline vInt &operator-= (vInt);
  sfpi_inline vInt &operator<<= (unsigned);
  sfpi_inline vInt &operator<<= (vUInt);
#if __riscv_xtttensixbh || __riscv_xtttensixqsr
  sfpi_inline vInt &operator>>= (unsigned);
  sfpi_inline vInt &operator>>= (vUInt);
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
#if __riscv_xtttensixbh || __riscv_xtttensixqsr
// arithmetic shifts added in blackhole
sfpi_inline vInt operator>> (vInt, vUInt);
#endif
sfpi_inline vInt operator& (vInt, vInt);
sfpi_inline vInt operator| (vInt, vInt);
sfpi_inline vInt operator^ (vInt, vInt);

sfpi_inline vInt operator<< (vInt, unsigned);
#if __riscv_xtttensixbh || __riscv_xtttensixqsr
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
  sfpi_inline vUInt () = default;
  sfpi_inline vUInt (vUInt const &) = default;
  sfpi_inline vUInt &operator= (vUInt const &) = default;

public:
  sfpi_inline vUInt (impl_::sfpu_t);
  sfpi_inline explicit vUInt (vInt);
  sfpi_inline vUInt (int16_t);
  sfpi_inline vUInt (uint16_t);
  sfpi_inline vUInt (int32_t);
  sfpi_inline vUInt (uint32_t);
  sfpi_inline vUInt (int val) : vUInt (int32_t (val)) {}
  sfpi_inline vUInt (unsigned val) : vUInt (uint32_t (val)) {}
  sfpi_inline vUInt (impl_::vCond);

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

constexpr impl_::vDReg<>::RegFile dst_reg;
constexpr impl_::vLReg::RegFile l_reg;

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
