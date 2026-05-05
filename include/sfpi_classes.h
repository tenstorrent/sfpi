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
#define __builtin_rvtt_sfpstore(src, addr, mod0, mode) \
  __builtin_rvtt_sfpstore(ckernel::instrn_buffer, src, addr, 0, 0, mod0, mode)
#define __builtin_rvtt_sfploadsrcs(addr, mod0, mode, done)          \
  __builtin_rvtt_sfploadsrcs(ckernel::instrn_buffer, addr, 0, 0, mod0, mode, done)
#define __builtin_rvtt_sfpstoresrcs(src, addr, mod0, mode, done) \
  __builtin_rvtt_sfpstoresrcs(ckernel::instrn_buffer, src, addr, 0, 0, mod0, mode, done)

#define __builtin_rvtt_sfpsetexp_i(src, imm, mod1) __builtin_rvtt_sfpsetexp_i(ckernel::instrn_buffer, src, imm, 0, 0, mod1)
#define __builtin_rvtt_sfpsetman_i(src, imm, mod1) __builtin_rvtt_sfpsetman_i(ckernel::instrn_buffer, src, imm, 0, 0, mod1)
#define __builtin_rvtt_sfpsetsgn_i(src, imm, mod1) __builtin_rvtt_sfpsetsgn_i(ckernel::instrn_buffer, src, imm, 0, 0, mod1)

#define __builtin_rvtt_sfpshft_i(src, imm, mod1) __builtin_rvtt_sfpshft_i(ckernel::instrn_buffer, src, imm, 0, 0, mod1)
#define __builtin_rvtt_sfpdivp2(src, imm, mod1) __builtin_rvtt_sfpdivp2(ckernel::instrn_buffer, src, imm, 0, 0, mod1)
#define __builtin_rvtt_sfpstochrnd_i(src, imm, mod1, mode) \
  __builtin_rvtt_sfpstochrnd_i(ckernel::instrn_buffer, src, imm, 0, 0, mod1, mode)

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
class vSMag;
class sFloat16a;
class sFloat16b;
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

template<typename Vector, typename Scalar>
class vNarrow : public Vector {
public:
  sfpi_inline vNarrow () = default;
  sfpi_inline vNarrow (vNarrow const &) = default;
  sfpi_inline vNarrow &operator= (vNarrow const &) = default;

public:
  sfpi_inline explicit vNarrow (impl_::sfpu_t val) : Vector (val) {}
  // Disable elemental ctor if the type is void
  template<typename Type, typename std::enable_if<std::is_same<Scalar, Type>::value> * = nullptr>
  sfpi_inline explicit vNarrow (Type val) : Vector (val) {};
};
using vFloat16a = vNarrow<vFloat, sFloat16a>;
using vFloat16b = vNarrow<vFloat, sFloat16b>;
using vUInt16 = vNarrow<vUInt, uint16_t>;
using vSMag16 = vNarrow<vSMag, void>;

//////////////////////////////////////////////////////////////////////////////

class LRegFile {
public:
  class vReg {
  private:
    unsigned reg;

  public:
    sfpi_inline vReg (vReg const &) = default;
    sfpi_inline vReg const &operator= (vReg const &src) const {
      *this = vVal (src);
      return *this;
    }

  public:
    sfpi_inline constexpr explicit vReg (int r) : reg (r) {}
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
    void in_use () const {
      *this = *this;
    }  
  };

public:
  template<typename Type, typename std::enable_if_t<std::is_base_of<impl_::vVal, Type>::value>* = nullptr>
  class vCReg {
  private:
    vReg lreg;

  public:
    sfpi_inline vCReg (vCReg const &) = default;
    sfpi_inline void operator= (vCReg &) = delete;

  public:
    sfpi_inline operator Type () const {
      return lreg;
    }

  public:
    sfpi_inline constexpr explicit vCReg (int r) : lreg (r) {}
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

public:
  constexpr LRegFile () = default;
  LRegFile (LRegFile const &) = delete;
  LRegFile (LRegFile &&) = delete;
  LRegFile &operator= (LRegFile const &) = delete;
  LRegFile &operator= (LRegFile &&) = delete;

public:
  sfpi_inline vReg operator[] (LRegs lr) const { return vReg (unsigned (lr)); }
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
class vReg_ {
private:
  int reg;
  short addr_mode = -1;

public:
  sfpi_inline constexpr vReg_ (vReg_ const &) = default;
  sfpi_inline vReg_ &operator= (vReg_ const &) = delete;

protected:
  sfpi_inline constexpr explicit vReg_ (int r) : reg (r) {}

  template<int OldMod>
  sfpi_inline constexpr explicit vReg_ (vReg_<Derived, OldMod> const &src, int addr_mode = -1)
      : reg (src.get ()), addr_mode (addr_mode) {}

public:
  sfpi_inline constexpr int get () const { return reg; }

public:
  template <int NewMod = -1>
  sfpi_inline constexpr Derived<NewMod> mode (int mode = -1) const {
    return Derived<NewMod >= 0 ? NewMod : Mod>
        (static_cast<Derived<Mod> const &> (*this), mode >= 0 ? mode : addr_mode);
  }
    
public:
  // These are not templates to allow type conversion of the operand
  sfpi_inline void operator= (vFloat) const;
  sfpi_inline operator vFloat () const;
  sfpi_inline void operator= (vFloat16a) const;
  sfpi_inline operator vFloat16a () const;
  sfpi_inline void operator= (vFloat16b) const;
  sfpi_inline operator vFloat16b () const;

  sfpi_inline void operator= (vInt) const;
  sfpi_inline operator vInt () const;

  sfpi_inline void operator= (vUInt) const;
  sfpi_inline operator vUInt () const;
  sfpi_inline void operator= (vUInt16) const;
  sfpi_inline operator vUInt16 () const;

  sfpi_inline void operator= (vSMag) const;
  sfpi_inline operator vSMag () const;
  sfpi_inline void operator= (vSMag16) const;
  sfpi_inline operator vSMag16 () const;

  // Convenience
  sfpi_inline void operator= (float) const;

private:
  void write (sfpu_t val, unsigned mod) const {
    int mode = addr_mode < 0 ? SFPSTORE_ADDR_MODE_NOINC : addr_mode;
    static_cast<Derived<Mod> const *> (this)->write (val, mod, mode);
  }
  
  sfpu_t read (unsigned mod) const {
    int mode = addr_mode < 0 ? SFPLOAD_ADDR_MODE_NOINC : addr_mode;
    return static_cast<Derived<Mod> const *> (this)->read (mod, mode);
  }
};

// Dst regs
class DstRegFile {
public:
  template<int Mod = -1>
  class vReg : public vReg_<vReg, Mod> {
  public:
    sfpi_inline constexpr explicit vReg (int r) : vReg_<vReg, Mod> (r) {}
    template<int OldMod>
    sfpi_inline constexpr explicit vReg (vReg<OldMod> const &src, int addr_mode)
        : vReg_<vReg, Mod> (src, addr_mode) {}
    sfpi_inline constexpr explicit vReg (vReg const &) = default;
    using vReg_<vReg, Mod>::operator=;

  public:
    // Deprecated 2026-04-14
    __SFPI_DEPRECATED ("Convert to vFloat, vInt or vUInt first")
    sfpi_inline void operator= (vReg const &dreg) const;
    __SFPI_DEPRECATED ("Convert to vFloat, vInt or vUint first")
    sfpi_inline vFloat operator- () const;

  public:
    sfpi_inline void write (sfpu_t val, unsigned mod, unsigned addr_mode) const {
      __builtin_rvtt_sfpstore (val, this->get (), mod, addr_mode);
    }
    
    sfpi_inline sfpu_t read (unsigned mod, unsigned addr_mode) const {
      return __builtin_rvtt_sfpload (this->get (), mod, addr_mode);
    }
  };

public:
  constexpr DstRegFile () = default;
  DstRegFile (DstRegFile const &) = delete;
  DstRegFile (DstRegFile &&) = delete;
  DstRegFile &operator= (DstRegFile const &) = delete;
  DstRegFile &operator= (DstRegFile &&) = delete;

public:
  sfpi_inline constexpr vReg<> operator[] (int ix) const {
    return vReg<> (ix * SFP_DESTREG_STRIDE);
  }

  // Make these void - ugly as these aren't really inc/dec
  sfpi_inline constexpr void operator++ () const { *this += 1; }
  sfpi_inline constexpr void operator++ (int) const { *this += 1; }
  sfpi_inline constexpr void operator+= (int ix) const {
    __builtin_rvtt_ttincrwc (0, SFP_DESTREG_STRIDE * ix, 0, 0);
  }
};

#if __riscv_xtttensixqsr
// SrcS regs
template<unsigned Slice>
class SrcSRegFile {
public:
  template<int Mod = -1>
  class vReg : public vReg_<vReg, Mod> {
    template<int> friend class vReg;

  private:
    bool is_done = false;

  public:
    sfpi_inline constexpr explicit vReg (int r) : vReg_<vReg, Mod> (r) {}
    template<int OldMod>
    sfpi_inline constexpr explicit vReg (vReg<OldMod> const &src, int addr_mode)
        : vReg_<vReg, Mod> (src, addr_mode), is_done (src.is_done) {}
    sfpi_inline constexpr explicit vReg (vReg const &) = default;
    using vReg_<vReg, Mod>::operator=;

  public:
    sfpi_inline constexpr vReg &done (bool done = true) {
      is_done = done;
      return *this;
    }

  public:
    sfpi_inline void write (sfpu_t val, unsigned mod, unsigned addr_mode) const {
      __builtin_rvtt_sfpstoresrcs (val, this->get (), mod, addr_mode, is_done);
    }
    
    sfpi_inline sfpu_t read (unsigned mod, unsigned addr_mode) const {
      return __builtin_rvtt_sfploadsrcs (this->get (), mod, addr_mode, is_done);
    }
  };

private:
  unsigned offset = SFP_SRCSREG_BASE + Slice * (SFP_SRCSREG_STRIDE * SFP_SRCSREG_COUNT);

public:
  constexpr SrcSRegFile () = default;
  SrcSRegFile (SrcSRegFile const &) = delete;
  SrcSRegFile (SrcSRegFile &&) = delete;
  SrcSRegFile &operator= (SrcSRegFile const &) = delete;
  SrcSRegFile &operator= (SrcSRegFile &&) = delete;

public:
  sfpi_inline constexpr vReg<> operator[] (int ix) const {
    return vReg<> (ix * SFP_SRCSREG_STRIDE + offset);
  }

  // Make these void - ugly as these aren't really inc/dec
  sfpi_inline void operator++ () { *this += 1; }
  sfpi_inline void operator++ (int) { *this += 1; }
  sfpi_inline void operator+= (int ix) {
    offset += ix * SFP_SRCSREG_STRIDE;
  }
};

#endif
}
}
