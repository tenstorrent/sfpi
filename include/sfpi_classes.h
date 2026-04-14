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

// Turn on deprecations by default
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

sfpi_inline uint32_t float_as_uint (float val);

//////////////////////////////////////////////////////////////////////////////
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

class vLReg {
private:
  unsigned reg;

public:
  sfpi_inline vLReg (vLReg const &) = default;
  sfpi_inline vLReg &operator= (vLReg const &) = delete;

public:
  sfpi_inline constexpr explicit vLReg (int r) : reg (r) {}
  sfpi_inline constexpr int get () const { return reg; }

public:
  sfpi_inline sfpu_t operator= (vVal v) const {
    __builtin_rvtt_sfpwritelreg (v.get (), get ());
    return v.get ();
  }
  sfpi_inline operator sfpu_t () const {
    return __builtin_rvtt_sfpreadlreg (get ());
  }
  template<typename Type, typename std::enable_if_t<std::is_base_of<impl_::vVal, Type>::value>* = nullptr>
  sfpi_inline operator Type () const {
    return sfpu_t (*this);
  }

public:
  class RegFile {
  public:
    constexpr RegFile () = default;
    RegFile (RegFile const &) = delete;
    RegFile (RegFile &&) = delete;
    RegFile &operator= (RegFile const &) = delete;
    RegFile &operator= (RegFile &&) = delete;

  public:
    sfpi_inline vLReg operator[] (LRegs lr) const { return vLReg (unsigned (lr)); }
  };
};

// A constant register
template<typename Type, typename std::enable_if_t<std::is_base_of<impl_::vVal, Type>::value>* = nullptr>
class vConst {
private:
  vLReg lreg;

public:
  sfpi_inline vConst (vConst const &) = default;
  sfpi_inline void operator= (vConst &) = delete;

public:
  sfpi_inline operator Type () const {
    return lreg;
  }

public:
  sfpi_inline constexpr explicit vConst (int r) : lreg (r) {}
  sfpi_inline void operator= (Type t) const {
    __builtin_rvtt_sfpwriteconfig_v (t.get (), lreg.get ());
  }

  // Assign from constructable scalar
  template<typename U,
           std::enable_if_t<std::is_constructible<Type, U const &>::value> * = nullptr>
  sfpi_inline void operator= (U u) const {
    operator= (Type (u));
  }
};

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
template<template<int> typename Derived, int Mod = -1>
class vReg { // A register.  Holds the register number
private:
  int reg;
  short addr_mode = -1;

public:
  sfpi_inline vReg (vReg const &) = default;
  sfpi_inline vReg &operator= (vReg const &) = delete;

protected:
  sfpi_inline constexpr explicit vReg (int r) : reg (r) {}

  template<int OldMod>
  sfpi_inline constexpr explicit vReg (vReg<Derived, OldMod> const &src, int addr_mode = -1)
      : reg (src.get ()), addr_mode (addr_mode) {}

public:
  sfpi_inline constexpr int get () const { return reg; }

public:
  template <int NewMod = -1>
  sfpi_inline constexpr Derived<NewMod> mode (int mode = -1) {
    return Derived<NewMod >= 0 ? NewMod : Mod> (static_cast<Derived<Mod> &> (*this), mode >= 0 ? mode : addr_mode);
  }

public:
  // These are not templates to allow type conversion of the operand
  sfpi_inline vFloat operator= (vFloat f) const;
  sfpi_inline operator vFloat () const;
  sfpi_inline vInt operator= (vInt i) const;
  sfpi_inline operator vInt () const;
  sfpi_inline vUInt operator= (vUInt u) const;
  sfpi_inline operator vUInt () const;

  // Convenience
  sfpi_inline vFloat operator= (float) const;

private:
  void write_ (sfpu_t, unsigned) const;
  sfpu_t read_ (unsigned) const;
};

// Dst regs
template<int Mod = -1>
class vDReg : public vReg<vDReg, Mod> {
private:
  constexpr explicit vDReg (unsigned r) : vReg<vDReg, Mod> (r) {}

public:
  template<int OldMod>
  constexpr explicit vDReg (vDReg<OldMod> const &src, int addr_mode) : vReg<vDReg, Mod> (src, addr_mode) {}

public:
  sfpi_inline vDReg (vDReg const &) = default;

public:
  __SFPI_DEPRECATED ("Convert to vFloat, vInt or vUInt first")
  sfpi_inline void operator=(const vDReg dreg) const;

  using vReg<vDReg, Mod>::operator=;

  __SFPI_DEPRECATED ("Convert to vFloat, vInt or vUint first")
  sfpi_inline vFloat operator-() const;

public:
  sfpi_inline void write_ (sfpu_t, unsigned mod, unsigned addr_mode) const;
  sfpi_inline sfpu_t read_ (unsigned mod, unsigned addr_mode) const;

public:
  class RegFile {
  public:
    constexpr RegFile () = default;
    RegFile (RegFile const &) = delete;
    RegFile (RegFile &&) = delete;
    RegFile &operator= (RegFile const &) = delete;
    RegFile &operator= (RegFile &&) = delete;

  public:
    sfpi_inline vDReg operator[](int ix) const {
      return vDReg (ix * SFP_DESTREG_STRIDE);
    }

    // Make these void - ugly as these aren't really inc/dec
    sfpi_inline void operator++ () const { *this += 1; }
    sfpi_inline void operator++ (int) const { *this += 1; }
    sfpi_inline void operator+= (int i) const {
      __builtin_rvtt_ttincrwc (0, SFP_DESTREG_STRIDE * i, 0, 0);
    }
  };
};
}
}
