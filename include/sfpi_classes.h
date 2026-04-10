/* -*- C++ -*-
 * SPDX-FileCopyrightText: © 2023-2026 Tenstorrent Inc.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

// Precondition SFPI implementation file.  Not for end use consumption. See sfpi.h

#pragma once

#include "tensix_builtins.h"
#include "sfpi_constants.h"

#define __builtin_rvtt_sfpxicmps(src, imm, mod1) __builtin_rvtt_sfpxicmps(ckernel::instrn_buffer, src, imm, 0, 0, mod1)
#define __builtin_rvtt_sfpxfcmps(src, imm, mod1) __builtin_rvtt_sfpxfcmps(ckernel::instrn_buffer, src, imm, 0, 0, mod1)
#define __builtin_rvtt_sfpxiadd_i(src, imm, mod1) __builtin_rvtt_sfpxiadd_i(ckernel::instrn_buffer, src, imm, 0, 0, mod1)
#define __builtin_rvtt_sfpxloadi(imm, bits) __builtin_rvtt_sfpxloadi(ckernel::instrn_buffer, imm, 0, 0, bits)

#define __builtin_rvtt_sfploadi(imm, mod0) __builtin_rvtt_sfploadi(ckernel::instrn_buffer, imm, 0, 0, mod0)
#define __builtin_rvtt_sfpload(addr, mod0, mode) __builtin_rvtt_sfpload(ckernel::instrn_buffer, addr, 0, 0, mod0, mode)
#define __builtin_rvtt_sfpstore(src, addr, mod0, mode) __builtin_rvtt_sfpstore(ckernel::instrn_buffer, src, addr, 0, 0, mod0, mode)

#define __builtin_rvtt_sfpsetexp_i(src, imm, mod1) __builtin_rvtt_sfpsetexp_i(ckernel::instrn_buffer, src, imm, 0, 0, mod1)
#define __builtin_rvtt_sfpsetman_i(src, imm, mod1) __builtin_rvtt_sfpsetman_i(ckernel::instrn_buffer, src, imm, 0, 0, mod1)
#define __builtin_rvtt_sfpsetsgn_i(src, imm, mod1) __builtin_rvtt_sfpsetsgn_i(ckernel::instrn_buffer, src, imm, 0, 0, mod1)

#define __builtin_rvtt_sfpshft_i(src, imm, mod1) __builtin_rvtt_sfpshft_i(ckernel::instrn_buffer, src, imm, 0, 0, mod1)
#define __builtin_rvtt_sfpdivp2(src, imm, mod1) __builtin_rvtt_sfpdivp2(ckernel::instrn_buffer, src, imm, 0, 0, mod1)
#define __builtin_rvtt_sfpstochrnd_i(src, imm, mod1, mode) __builtin_rvtt_sfpstochrnd_i(ckernel::instrn_buffer, src, imm, 0, 0, mod1, mode)

#define sfpi_inline __attribute__((always_inline)) inline

// Turn on deprecations byy default
#if defined (SFPI_MARK_DEPRECATED) ? SFPI_MARK_DEPRECATED : 1
#define __SFPI_DEPRECATED(X) [[deprecated (X)]]
#else
#define __SFPI_DEPRECATED(X)
#endif
#define __SFPI_MIXED_SIGNEDNESS __SFPI_DEPRECATED("This function mizes signed and unsigned types, be explicit about signedness")

namespace sfpi {

class vFloat;
class vInt;
class vUInt;
enum class LRegs : uint8_t;

namespace impl_ {

using sfpu_t = ::__xtt_vector;

class vVal;
class vReg;
class vLReg;
class vCond;

sfpi_inline uint32_t float_as_uint (float val);

//////////////////////////////////////////////////////////////////////////////
class vReg { // A register.  Holds the register number
private:
  int reg;

public:
  constexpr explicit vReg (int r) : reg (r) {}
  constexpr int get () const { return reg; }
};

class vVal { // A value, holds value and intialized flag
private:
  // FIXME: we want to remove this, with better LV optimization/handling
  bool initialized = false;
  sfpu_t v;

public:
  sfpi_inline vVal () {}
  sfpi_inline vVal (vVal const &) = default;
  sfpi_inline vVal &operator= (vVal const &val) {
    v = initialized ? __builtin_rvtt_sfpassign_lv (v, val.get ()) : val.get ();
    initialized = true;
    return *this;
  }

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
  sfpi_inline sfpu_t int_shift (vVal b, bool is_signed __attribute__ ((unused))) const {
    return __builtin_rvtt_sfpshft_v (get (), b.get (),
#if __riscv_xtttensixbh || __riscv_xtttensixqsr
                                     is_signed ? SFPSHFT_MOD1_ARITHMETIC :
#endif
                                     SFPSHFT_MOD1_LOGICAL);
  }
  sfpi_inline sfpu_t int_shift (int b, bool is_signed __attribute__ ((unused))) const {
    return __builtin_rvtt_sfpshft_i (get (), b,
#if __riscv_xtttensixbh || __riscv_xtttensixqsr
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

//////////////////////////////////////////////////////////////////////////////

// A constant register
template<typename Type, typename std::enable_if_t<std::is_base_of<impl_::vVal, Type>::value>* = nullptr>
class vConst : public vReg {
public:
  sfpi_inline constexpr explicit vConst (int r) : impl_::vReg(r) {}
  sfpi_inline vConst (vConst const &) = default;

  sfpi_inline operator Type () const {
    return __builtin_rvtt_sfpreadlreg (get ());
  }

  sfpi_inline void operator= (vConst &) = delete;

  sfpi_inline void operator= (Type t) const {
    __builtin_rvtt_sfpwriteconfig_v (t.get (), get ());
  }

  template<typename U,
           std::enable_if_t<std::is_constructible<Type, U const &>::value> * = nullptr>
  sfpi_inline void operator= (U u) const {
    operator= (Type (u));
  }
};

class vLReg : public vReg {
private:
  sfpi_inline constexpr explicit vLReg (unsigned ix) : vReg (ix) {}

public:
  sfpi_inline vLReg (vLReg const &) = default;
  sfpi_inline vLReg &operator= (vLReg const &) = delete;

public:
  sfpi_inline sfpu_t operator= (vVal v) const {
    __builtin_rvtt_sfpwritelreg (v.get (), get ());
    return v.get ();
  }

public:
  class LRegFile {
  public:
    constexpr LRegFile () = default;
    LRegFile (LRegFile const &) = delete;
    LRegFile (LRegFile &&) = delete;
    LRegFile &operator= (LRegFile const &) = delete;
    LRegFile &operator= (LRegFile &&) = delete;

  public:
    sfpi_inline vLReg operator[] (LRegs lr) const { return vLReg (unsigned (lr)); }
  };
};
sfpi_inline void impl_::vVal::operator= (impl_::vLReg lr) {
  // FIXME: shouldn't this pay attention to initialized?
  v = __builtin_rvtt_sfpreadlreg (lr.get ());
  initialized = true;
}

class vCond {
  friend class sfpi::vInt; // conversion op from here?
  friend class sfpi::vUInt;

public:
  enum BoolOp {
    Or = SFPXBOOL_MOD1_OR,
    And = SFPXBOOL_MOD1_AND,
    Not = SFPXBOOL_MOD1_NOT,
  };

  enum CondOp {
    LT = SFPXCMP_MOD1_CC_LT,
    NE = SFPXCMP_MOD1_CC_NE,
    GTE = SFPXCMP_MOD1_CC_GTE,
    EQ = SFPXCMP_MOD1_CC_EQ,
    LTE = SFPXCMP_MOD1_CC_LTE,
    GT = SFPXCMP_MOD1_CC_GT,
  };

private:
  int result;

public:
  sfpi_inline vCond (vCond const &) = default;
  sfpi_inline vCond &operator= (vCond const &) = default;

public:
  sfpi_inline vCond (BoolOp, vCond, vCond);
  sfpi_inline vCond (CondOp, vFloat, float);
  sfpi_inline vCond (CondOp, vFloat, vFloat);
  sfpi_inline vCond (CondOp, vInt, int32_t, unsigned);
  sfpi_inline vCond (CondOp, vInt, vInt, unsigned);
  sfpi_inline vCond (vInt);

private:
  sfpi_inline int get () const { return result; }

public:
  class CC {
  private:
    unsigned dep = 0;
    unsigned depth = 0;

  public:
    sfpi_inline CC () = default;
    sfpi_inline ~CC ();

    // Moveable, not copyable
    sfpi_inline CC (CC &&);
    sfpi_inline CC &operator= (CC &&);

    sfpi_inline CC &if_();
    sfpi_inline CC &else_();

    sfpi_inline void cond (vCond);
    sfpi_inline void cond (vInt);
    sfpi_inline void cond (vUInt);

    sfpi_inline CC &push ();
    sfpi_inline CC &pop ();
  };
};

sfpi_inline vCond operator&& (vCond, vCond);
sfpi_inline vCond operator|| (vCond, vCond);
sfpi_inline vCond operator! (vCond);

//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// Dst regs
class vDReg : public vReg {
private:
    constexpr explicit vDReg (unsigned i) : vReg (i) {}

public:
  sfpi_inline vDReg (vDReg const &) = default;
  sfpi_inline vDReg &operator= (vDReg const &) = delete;

public:
  __SFPI_DEPRECATED ("Convert to vFloat, vInt or vUInt first")
  sfpi_inline void operator=(const vDReg dreg) const;

  sfpi_inline vFloat operator= (vFloat) const;
  sfpi_inline vInt operator= (vInt) const;
  sfpi_inline vUInt operator= (vUInt) const;
  sfpi_inline vFloat operator= (float) const;

  sfpi_inline operator vFloat () const;
  sfpi_inline operator vInt () const;
  sfpi_inline operator vUInt () const;

  __SFPI_DEPRECATED ("Convert to vFloat, vInt or vUint first")
  sfpi_inline vFloat operator-() const;

public:
  class DestRegFile {
  public:
    constexpr DestRegFile () = default;
    DestRegFile (DestRegFile const &) = delete;
    DestRegFile (DestRegFile &&) = delete;
    DestRegFile &operator= (DestRegFile const &) = delete;
    DestRegFile &operator= (DestRegFile &&) = delete;

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
}
