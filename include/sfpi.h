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
//   class vSMag - 32bit signed-magnitude elements
//   class vMag - 31bit magnitude elements in u32 format
//   class vFloat16a - fp16a elements held in fp32 format
//   class vFloat16b - fp16b elements held in fp32 format
//   class vUInt16 - uint16 elenents held in u32 format
//   class vUInt8 - uint8 elenents held in u32 format
//   class vSMag16 - smag16 elenents held in sm32 format
//   class vSMag8 - smag8 elenents held in sm32 format
//   class vBool - vector boolean type
//
//  These are held in the lregs and may be transfered to/from the dst regs (and
//  quasar srcs regs).  Overloaded operators are defined to permit such things as:
//     vFloat a, b, c, d;
//     vFloat d = a * b + c;
//
// Destination Register:
//   constexpr impl_::DstRegFile dst_reg;
//
//   The Destination Register is modeled by a global variable which is
//   essentially an array of class vDReg.  vDRegs provide much the same
//   functionality as LRegs, eg, the following is legal:
//       dst_reg[0] = vFloat(dst_reg[1]) * dst_reg[2] + dst_reg[3];
//   The above is expanded out to load local registers, perform the operation
//   and store back into the destination register.  Any missing functionality
//   (eg, there is no load immediate) accessed through LRegs.
//
//   You can modify the load/store mod and addr_mode operands:
//       dst_reg[0].mode<MOD_OP>(ADDR_MODE) ...
//   Both MOD_OP and ADDR_MODE are optional here.
//
// Quasar SrcS Register:
//   Unlike dst_reg, you need a local variable, because it contains some state:
//   SrcSView<N> src_reg; // N = 0, 1, 2
//   src_reg[0] = vFloat(dst_reg[0]);
//
//   The same `mode` call can be used to modify mod and addr_mode operand.  You
//   can also use a `done` call to set the done flag:
//     src_reg[0].done() = vFloat(src_reg[0].mode<DataLayout::F16b>());
//
// Constant Registers:
//   The TILEID is available as vConstTileId.
//
//   Programmable constant regisers are available via
//   vConst{Float,Int}Prgm[0-2], the int and float variants are NOT different
//   registers. These may be assigned to, but that is an expensive operation.
//
//
// Predicated Execution:
//   macros v_if(), v_elseif(), v_else, v_endif
//
//   The vBool class is used to evaluate conditions, and the impl_::CC class
//   manages CCreg state. By convention the test infastructure indents
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
//
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
// 16-bit machines died, or went to that farm upstate for retured pets. But
// anyway, riscv embedded ABIs have int32_t as long, not int, and we have to
// deal with it.
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
  // want to accidentally do that at runtime (type-punning consteval fns not
  // available to us in the version of C++ this is for). Besides why would you
  // want it?  (The compiler is quite capable of spotting bit patterns that are
  // suitable for loading as an float16a, rather than lo/hi pair).
  sfpi_inline explicit sFloat16a (float) = delete;  

  // Reinterpret the bit pattern. Generally because you've computed something
  // dynamically. If you're using these with literals, you're just obfuscating
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

public:
  sfpi_inline uint32_t get () const { return val; }
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

using vFloat16a = impl_::vFloat16a;
using vFloat16b = impl_::vFloat16b;

//////////////////////////////////////////////////////////////////////////////
class vInt : public impl_::vVal {
public:
  sfpi_inline vInt () = default;
  sfpi_inline vInt (vInt const &) = default;
  sfpi_inline vInt &operator= (vInt const &) = default;

public:
  sfpi_inline vInt (impl_::sfpu_t);
  sfpi_inline vInt (impl_::vMag);
  sfpi_inline explicit vInt (vUInt);
  sfpi_inline vInt (impl_::vUInt16);
  sfpi_inline vInt (int16_t);
  sfpi_inline vInt (uint16_t);
  sfpi_inline vInt (int32_t);
  sfpi_inline vInt (uint32_t);
  sfpi_inline vInt (int);
  sfpi_inline vInt (unsigned);

public:
  __SFPI_DEPRECATED("Implicit vInt->vUInt conversion deprecated, use an explicit cast or restructure")
  sfpi_inline operator vUInt () const;

public:
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

#if 0
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
#else
template<typename T,
         typename std::enable_if_t<std::is_integral<T>::value>* = nullptr>
sfpi_inline vInt operator+ (vInt a, T b) { return a + vInt (b); }
template<typename T,
         typename std::enable_if_t<std::is_integral<T>::value>* = nullptr>
sfpi_inline vInt operator- (vInt a, T b) { return a - vInt (b); }
template<typename T,
         typename std::enable_if_t<std::is_integral<T>::value>* = nullptr>
sfpi_inline vInt operator& (vInt a, T b) { return a & vInt (b); }
template<typename T,
         typename std::enable_if_t<std::is_integral<T>::value>* = nullptr>
sfpi_inline vInt operator| (vInt a, T b) { return a | vInt (b); }
template<typename T,
         typename std::enable_if_t<std::is_integral<T>::value>* = nullptr>
sfpi_inline vInt operator^ (vInt a, T b) { return a ^ vInt (b); }

template<typename T,
         typename std::enable_if_t<std::is_integral<T>::value>* = nullptr>
sfpi_inline vInt operator+ (T a, vInt b) { return b + a; }
template<typename T,
         typename std::enable_if_t<std::is_integral<T>::value>* = nullptr>
sfpi_inline vInt operator- (T a, vInt b) { return vInt (a) - b; }
template<typename T,
         typename std::enable_if_t<std::is_integral<T>::value>* = nullptr>
sfpi_inline vInt operator& (T a, vInt b) { return b & a; }
template<typename T,
         typename std::enable_if_t<std::is_integral<T>::value>* = nullptr>
sfpi_inline vInt operator| (T a, vInt b) { return b | a; }
template<typename T,
         typename std::enable_if_t<std::is_integral<T>::value>* = nullptr>
sfpi_inline vInt operator^ (T a, vInt b) { return b ^ a; }
#endif

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
  sfpi_inline vUInt (int);
  sfpi_inline vUInt (unsigned);

public:
  __SFPI_DEPRECATED("Implicit vUInt->vInt conversion deprecated, use an explicit cast or restructure")
  sfpi_inline operator vInt () const;

public:
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
using vUInt16 = impl_::vUInt16;
using vUInt8 = impl_::vUInt8;
using vMag = impl_::vMag;

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

#if 0
sfpi_inline vUInt operator+ (vUInt, int32_t);
sfpi_inline vUInt operator- (vUInt, int32_t);

sfpi_inline vUInt operator& (vUInt, uint32_t);
sfpi_inline vUInt operator| (vUInt, uint32_t);
sfpi_inline vUInt operator^ (vUInt, uint32_t);

sfpi_inline vUInt operator& (vUInt, unsigned);
sfpi_inline vUInt operator| (vUInt, unsigned);
sfpi_inline vUInt operator^ (vUInt, unsigned);
#else
template<typename T,
         typename std::enable_if_t<std::is_integral<T>::value>* = nullptr>
sfpi_inline vUInt operator+ (vUInt a, T b) { return a + vUInt (b); }
template<typename T,
         typename std::enable_if_t<std::is_integral<T>::value>* = nullptr>
sfpi_inline vUInt operator- (vUInt a, T b) { return a - vUInt (b); }
template<typename T,
         typename std::enable_if_t<std::is_integral<T>::value>* = nullptr>
sfpi_inline vUInt operator& (vUInt a, T b) { return a & vUInt (b); }
template<typename T,
         typename std::enable_if_t<std::is_integral<T>::value>* = nullptr>
sfpi_inline vUInt operator| (vUInt a, T b) { return a | vUInt (b); }
template<typename T,
         typename std::enable_if_t<std::is_integral<T>::value>* = nullptr>
sfpi_inline vUInt operator^ (vUInt a, T b) { return a ^ vUInt (b); }

template<typename T,
         typename std::enable_if_t<std::is_integral<T>::value>* = nullptr>
sfpi_inline vUInt operator+ (T a, vUInt b) { return b + a; }
template<typename T,
         typename std::enable_if_t<std::is_integral<T>::value>* = nullptr>
sfpi_inline vUInt operator- (T a, vUInt b) { return vUInt (a) - b; }
template<typename T,
         typename std::enable_if_t<std::is_integral<T>::value>* = nullptr>
sfpi_inline vUInt operator& (T a, vUInt b) { return b & a; }
template<typename T,
         typename std::enable_if_t<std::is_integral<T>::value>* = nullptr>
sfpi_inline vUInt operator| (T a, vUInt b) { return b | a; }
template<typename T,
         typename std::enable_if_t<std::is_integral<T>::value>* = nullptr>
sfpi_inline vUInt operator^ (T a, vUInt b) { return b ^ a; }
#endif

sfpi_inline vMag operator>> (vMag, unsigned);
sfpi_inline vMag operator>> (vMag, int);
sfpi_inline vMag operator>> (vMag, vUInt);

sfpi_inline vMag operator& (vMag, unsigned);
sfpi_inline vMag operator& (vMag, int);
sfpi_inline vMag operator& (vMag, vMag);

//////////////////////////////////////////////////////////////////////////////
// Sign-Magnitude, a container
class vSMag : public impl_::vVal {
public:
  sfpi_inline vSMag () = default;
  sfpi_inline vSMag (vSMag const &) = default;
  sfpi_inline vSMag &operator= (vSMag const &) = default;

public:
  sfpi_inline vSMag (impl_::sfpu_t);
  sfpi_inline vSMag (uint32_t);
  sfpi_inline vSMag (int32_t);
  sfpi_inline vSMag (unsigned);
  sfpi_inline vSMag (int);
  sfpi_inline vSMag (impl_::vMag);
};
using vSMag16 = impl_::vSMag16;
using vSMag8 = impl_::vSMag8;

sfpi_inline vUInt operator& (vSMag, unsigned);

//////////////////////////////////////////////////////////////////////////////

#if 0
sfpi_inline vUInt operator& (vUInt, int);
sfpi_inline vUInt operator| (vUInt, int);
sfpi_inline vUInt operator^ (vUInt, int);

sfpi_inline vInt operator+ (int32_t a, vInt b) { return b + a; }
sfpi_inline vInt operator- (int32_t a, vInt b) { return vInt (a) - b; }
sfpi_inline vInt operator& (int32_t a, vInt b) { return b & a; }
sfpi_inline vInt operator| (int32_t a, vInt b) { return b | a; }
sfpi_inline vInt operator^ (int32_t a, vInt b) { return b ^ a; }

sfpi_inline vUInt operator+ (int32_t a, vUInt b) { return b + a; }
sfpi_inline vUInt operator- (int32_t a, vUInt b) { return vUInt (a) - b; }
sfpi_inline vUInt operator& (int32_t a, vUInt b) { return b & uint32_t (a); }
sfpi_inline vUInt operator| (int32_t a, vUInt b) { return b | uint32_t (a); }
sfpi_inline vUInt operator^ (int32_t a, vUInt b) { return b ^ uint32_t (a); }
#endif

//////////////////////////////////////////////////////////////////////////////
// Permitted cross-type operators
sfpi_inline vUInt operator- (vUInt, vInt);
sfpi_inline vUInt operator& (vUInt, vInt);
sfpi_inline vUInt operator| (vUInt, vInt);
sfpi_inline vUInt operator^ (vUInt, vInt);

sfpi_inline vUInt operator& (vInt, vUInt);
sfpi_inline vUInt operator| (vInt, vUInt);
sfpi_inline vUInt operator^ (vInt, vUInt);

sfpi_inline vInt operator+ (vInt, vMag);
sfpi_inline vInt operator+ (vMag, vInt);
sfpi_inline vInt operator- (vInt, vMag);
sfpi_inline vInt operator- (vMag, vInt);
sfpi_inline vUInt operator+ (vUInt, vMag);
sfpi_inline vUInt operator+ (vMag, vUInt);
sfpi_inline vUInt operator- (vUInt, vMag);
sfpi_inline vUInt operator- (vMag, vUInt);

//////////////////////////////////////////////////////////////////////////////

class vBool {
  friend class impl_::CC;

public:
  enum Logic : unsigned char {
       And = SFPXLOGIC_MOD1_AND,
        Or = SFPXLOGIC_MOD1_OR,
       Not = SFPXLOGIC_MOD1_NOT,
    Nearby = SFPXLOGIC_MOD1_NEARBY,
  };

  enum Cond : unsigned char {
    EQ = SFPXCMP_MOD1_CC_EQ,
    NE = SFPXCMP_MOD1_CC_NE,
    LT = SFPXCMP_MOD1_CC_LT,
    GE = SFPXCMP_MOD1_CC_GE,
    GT = SFPXCMP_MOD1_CC_GT,
    LE = SFPXCMP_MOD1_CC_LE,
  };

  enum Type : unsigned char {
     UInt = SFPXCMP_MOD1_TYPE_UINT << SFPXCMP_MOD1_TYPE_SHIFT,
      Int = SFPXCMP_MOD1_TYPE_INT << SFPXCMP_MOD1_TYPE_SHIFT,
     SMag = SFPXCMP_MOD1_TYPE_SMAG << SFPXCMP_MOD1_TYPE_SHIFT,
    Float = SFPXCMP_MOD1_TYPE_FLOAT << SFPXCMP_MOD1_TYPE_SHIFT,
  };

private:
  int result;

public:
  template<Cond C, Type T>
  struct Cmp {
  private:
    // We need to squish the two operands into a single member so that the
    // compiler's clobber notation doesn't force this into memory
    impl_::sfpu2_t v;

  public:
    sfpi_inline Cmp (Cmp const &) = default;
    sfpi_inline Cmp &operator= (Cmp const &) = default;

  public:
    sfpi_inline Cmp (impl_::vVal, impl_::vVal);

  public:
    sfpi_inline impl_::sfpu_t get (unsigned ix) const { return __builtin_rvtt_sfpselect2 (v, ix); }
  };

public:
  sfpi_inline vBool (vBool const &) = default;
  sfpi_inline vBool &operator= (vBool const &) = default;

public:
  sfpi_inline vBool (Logic, vBool, vBool);

public:
  template<Cond C, Type T>
  sfpi_inline vBool (Cmp<C, T>);

public:
  sfpi_inline vBool (vInt);
  sfpi_inline vBool (vUInt);
  sfpi_inline vBool (vSMag);

private:
  sfpi_inline int get () const { return result; }
};

sfpi_inline vBool operator&& (vBool, vBool);
sfpi_inline vBool operator|| (vBool, vBool);
sfpi_inline vBool operator! (vBool);

//////////////////////////////////////////////////////////////////////////////
// Comparisons

sfpi_inline vBool::Cmp<vBool::LT, vBool::Float> operator< (vFloat, vFloat);
sfpi_inline vBool::Cmp<vBool::GE, vBool::Float> operator>= (vFloat, vFloat);
sfpi_inline vBool::Cmp<vBool::EQ, vBool::Float> operator== (vFloat, vFloat);
sfpi_inline vBool::Cmp<vBool::NE, vBool::Float> operator!= (vFloat, vFloat);
sfpi_inline vBool::Cmp<vBool::GT, vBool::Float> operator> (vFloat, vFloat);
sfpi_inline vBool::Cmp<vBool::LE, vBool::Float> operator<= (vFloat, vFloat);

sfpi_inline vBool::Cmp<vBool::LT, vBool::Int> operator< (vInt, vInt);
sfpi_inline vBool::Cmp<vBool::GE, vBool::Int> operator>= (vInt, vInt);
sfpi_inline vBool::Cmp<vBool::EQ, vBool::Int> operator== (vInt, vInt);
sfpi_inline vBool::Cmp<vBool::NE, vBool::Int> operator!= (vInt, vInt);
sfpi_inline vBool::Cmp<vBool::GT, vBool::Int> operator> (vInt, vInt);
sfpi_inline vBool::Cmp<vBool::LE, vBool::Int> operator<= (vInt, vInt);

// We need these overloads as vMag decays to vUInt and hence ambiguous
sfpi_inline  auto operator== (vInt a, vMag b) { return a == vInt (b); }
sfpi_inline  auto operator!= (vInt a, vMag b) { return a != vInt (b); }
sfpi_inline  auto operator< (vInt a, vMag b) { return a < vInt (b); }
sfpi_inline  auto operator> (vInt a, vMag b) { return a > vInt (b); }
sfpi_inline  auto operator<= (vInt a, vMag b) { return a <= vInt (b); }
sfpi_inline  auto operator>= (vInt a, vMag b) { return a >= vInt (b); }

sfpi_inline  vBool::Cmp<vBool::LT, vBool::UInt> operator< (vUInt, vUInt);
sfpi_inline  vBool::Cmp<vBool::GE, vBool::UInt> operator>= (vUInt, vUInt);
sfpi_inline  vBool::Cmp<vBool::EQ, vBool::UInt> operator== (vUInt, vUInt);
sfpi_inline  vBool::Cmp<vBool::NE, vBool::UInt> operator!= (vUInt, vUInt);
sfpi_inline  vBool::Cmp<vBool::GT, vBool::UInt> operator> (vUInt, vUInt);
sfpi_inline  vBool::Cmp<vBool::LE, vBool::UInt> operator<= (vUInt, vUInt);

sfpi_inline vBool::Cmp<vBool::EQ, vBool::SMag> operator== (vSMag, vSMag);
sfpi_inline vBool::Cmp<vBool::NE, vBool::SMag> operator!= (vSMag, vSMag);

// Mark an ordering compare as 'close', specifically the compared values are
// known to be within <2^31 of each other
sfpi_inline vBool nearby (vBool);

#if __riscv_xtttensixbh || __riscv_xtttensixqsr
template<vBool::Cond C, vBool::Type T>
sfpi_inline vUInt mask (vBool::Cmp<C, T>);
#endif

//////////////////////////////////////////////////////////////////////////////
// C++17: In a function-call expression, the expression that names the function
// is sequenced before every argument expression and every default argument. [expr.pre]

#define v_if(x)                                         \
  { sfpi::impl_::CC __cc;                               \
  __cc.pred (sfpi::SFPXPRED_MOD1_IF                     \
             | sfpi::SFPXPRED_MOD1_PUSH).cond (x);      \
  {

#define v_elseif(x)                                     \
  } __cc.pred (sfpi::SFPXPRED_MOD1_ELSE                 \
               | sfpi::SFPXPRED_MOD1_IF                 \
               | sfpi::SFPXPRED_MOD1_PUSH).cond (x); {

#define v_else                                  \
  } __cc.pred (sfpi::SFPXPRED_MOD1_ELSE); {

#define v_endif                                 \
  } __cc.pred (sfpi::SFPXPRED_MOD1_END);      \
  }

#define v_block                                 \
  { sfpi::impl_::CC __cc;                       \
  __cc.pred (sfpi::SFPXPRED_MOD1_PUSH);

#define v_and(x)                                \
  __cc.pred(sfpi::SFPXPRED_MOD1_IF).cond (x);

#define v_endblock                              \
  __cc.pred(sfpi::SFPXPRED_MOD1_END);         \
  }

//////////////////////////////////////////////////////////////////////////////
// LRegs

enum class LRegs : uint8_t {
  LReg0, LReg1, LReg2, LReg3, LReg4, LReg5, LReg6, LReg7,
  LRegCount = SFP_LREG_COUNT,
};

constexpr impl_::LRegFile l_reg;
constexpr impl_::DstRegFile dst_reg;
#if __riscv_xtttensixqsr
template<unsigned Slice> using SrcSView = impl_::SrcSRegFile<Slice>;

using UnpackSrcS __SFPI_DEPRECATED ("Use SrcSView<0>") = impl_::SrcSRegFile<0>;
using ComputeSrcS __SFPI_DEPRECATED ("Use SrcSView<1>") = impl_::SrcSRegFile<1>;
using PackSrcS __SFPI_DEPRECATED ("Use SrcSView<2>") = impl_::SrcSRegFile<2>;
#endif

//////////////////////////////////////////////////////////////////////////////
// User accessible float constants

constexpr impl_::LRegFile::vCReg<vFloat> vConstFloatPrgm0 (CREG_IDX_PRGM1);
constexpr impl_::LRegFile::vCReg<vInt> vConstIntPrgm0 (CREG_IDX_PRGM1);
constexpr impl_::LRegFile::vCReg<vFloat> vConstFloatPrgm1 (CREG_IDX_PRGM2);
constexpr impl_::LRegFile::vCReg<vInt> vConstIntPrgm1 (CREG_IDX_PRGM2);
constexpr impl_::LRegFile::vCReg<vFloat> vConstFloatPrgm2 (CREG_IDX_PRGM3);
constexpr impl_::LRegFile::vCReg<vInt> vConstIntPrgm2 (CREG_IDX_PRGM3);
#if !(__riscv_xtttensixwh || __riscv_xtttensixbh)
constexpr impl_::LRegFile::vCReg<vFloat> vConstFloatPrgm3 (CREG_IDX_PRGM0);
constexpr impl_::LRegFile::vCReg<vInt> vConstIntPrgm3 (CREG_IDX_PRGM0);
#endif

constexpr impl_::LRegFile::vCReg<vInt> vConstTileId (CREG_IDX_TILEID);

//////////////////////////////////////////////////////////////////////////////

class lreg_pressure {
  bool up;
public:
  sfpi_inline lreg_pressure (bool up = true) : up (up) {
    __builtin_rvtt_lreg_pressure (up);
  }
  sfpi_inline ~lreg_pressure () {
    __builtin_rvtt_lreg_pressure (!up);
  }
};

} // namespace sfpi

//////////////////////////////////////////////////////////////////////////////
#include "sfpi_funcs.h"
#include "sfpi_lib.h"
