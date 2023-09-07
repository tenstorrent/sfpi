/*
 * SPDX-FileCopyrightText: © 2023 Tenstorrent Inc.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

///////////////////////////////////////////////////////////////////////////////
// sfpi_imp.h: SFPu Interface implementation for Grayskull
///////////////////////////////////////////////////////////////////////////////

#pragma once

namespace sfpi {

//////////////////////////////////////////////////////////////////////////////
constexpr __vConstFloat vConst0p8373(CREG_IDX_0P837300003);
constexpr __vConstFloat vConstFloatPrgm0(CREG_IDX_PRGM1);
constexpr __vConstFloat vConstFloatPrgm1(CREG_IDX_PRGM2);
constexpr __vConstFloat vConstFloatPrgm2(CREG_IDX_PRGM3);

constexpr __vConstIntBase vConstTileId(CREG_IDX_TILEID);
constexpr __vConstIntBase vConstIntPrgm0(CREG_IDX_PRGM1);
constexpr __vConstIntBase vConstIntPrgm1(CREG_IDX_PRGM2);
constexpr __vConstIntBase vConstIntPrgm2(CREG_IDX_PRGM3);

//////////////////////////////////////////////////////////////////////////////
sfpi_inline __vCond __vDReg::operator==(const float x) const {return __vCond(__vCond::__vCondEQ, vFloat(*this), x); }
sfpi_inline __vCond __vDReg::operator!=(const float x) const { return __vCond(__vCond::__vCondNE, vFloat(*this), x); }
sfpi_inline __vCond __vDReg::operator<(const float x) const { return __vCond(__vCond::__vCondLT, vFloat(*this), x); }
sfpi_inline __vCond __vDReg::operator<=(const float x) const { return __vCond(__vCond::__vCondLTE, vFloat(*this), x); }
sfpi_inline __vCond __vDReg::operator>(const float x) const { return __vCond(__vCond::__vCondGT, vFloat(*this), x); }
sfpi_inline __vCond __vDReg::operator>=(const float x) const { return __vCond(__vCond::__vCondGTE, vFloat(*this), x); }

template <>
sfpi_inline vFloat __vDReg::operator=(const vFloat vec) const
{
    __builtin_rvtt_sfpstore(vec.get(), SFPSTORE_MOD0_FMT_SRCB, SFPSTORE_ADDR_MODE_NOINC, reg);
    return vec;
}

sfpi_inline vFloat __vDReg::operator=(const double d) const
{
    vFloat v(static_cast<float>(d));
    *this = v;
    return v;
}

sfpi_inline vFloat __vDReg::operator=(s2vFloat16 f) const
{
    vFloat v(f);
    *this = v;
    return v;
}

sfpi_inline vFloat __vDReg::operator=(const float f) const
{
    vFloat v(f);
    *this = v;
    return v;
}

template <typename vecType, typename std::enable_if_t<std::is_base_of<__vBase, vecType>::value>*>
sfpi_inline vecType __vDReg::operator=(const vecType vec) const
{
    __builtin_rvtt_sfpstore(vec.get(), SFPSTORE_MOD0_FMT_SRCB, SFPSTORE_ADDR_MODE_NOINC, reg);
    return vec;
}

sfpi_inline void __vDReg::operator=(const __vDReg dreg) const
{
    vFloat tmp = dreg;
    __builtin_rvtt_sfpstore(tmp.get(), SFPSTORE_MOD0_FMT_SRCB, SFPSTORE_ADDR_MODE_NOINC, reg);
}

sfpi_inline vFloat __vDReg::operator=(const __vConstFloat creg) const
{
    __rvtt_vec_t lr = __builtin_rvtt_sfpassignlreg(creg.get());
    __builtin_rvtt_sfpstore(lr, SFPSTORE_MOD0_FMT_SRCB, SFPSTORE_ADDR_MODE_NOINC, reg);
    return vFloat(lr);
}

//////////////////////////////////////////////////////////////////////////////
sfpi_inline __vCond vFloat::operator==(const float x) const { return __vCond(__vCond::__vCondEQ, *this, x); }
sfpi_inline __vCond vFloat::operator!=(const float x) const { return __vCond(__vCond::__vCondNE, *this, x); }
sfpi_inline __vCond vFloat::operator<(const float x) const { return __vCond(__vCond::__vCondLT, *this, x); }
sfpi_inline __vCond vFloat::operator<=(const float x) const { return __vCond(__vCond::__vCondLTE, *this, x); }
sfpi_inline __vCond vFloat::operator>(const float x) const { return __vCond(__vCond::__vCondGT, *this, x); }
sfpi_inline __vCond vFloat::operator>=(const float x) const { return __vCond(__vCond::__vCondGTE, *this, x); }

sfpi_inline vFloat vFloat::operator-=(const vFloat a)
{
    __rvtt_vec_t neg1 = __builtin_rvtt_sfpassignlreg(vConstNeg1.get());
    assign(__builtin_rvtt_sfpmad(neg1, a.get(), v, SFPMAD_MOD1_OFFSET_NONE));
    return v;
}

sfpi_inline vFloat::vFloat(const __vDReg dreg)
{
    v = __builtin_rvtt_sfpload(SFPLOAD_MOD0_FMT_SRCB, SFPLOAD_ADDR_MODE_NOINC, dreg.get());
    initialized = true;
}

sfpi_inline void vFloat::loadf(const float val)
{
    assign(__builtin_rvtt_sfpxloadi(SFPXLOADI_MOD0_FLOAT, __f32asui(val)));
}

//////////////////////////////////////////////////////////////////////////////
sfpi_inline void __vIntBase::loadsi(int32_t val)
{
    assign(__builtin_rvtt_sfpxloadi(SFPXLOADI_MOD0_INT32, val));
}

sfpi_inline void __vIntBase::loadui(uint32_t val)
{
    assign(__builtin_rvtt_sfpxloadi(SFPXLOADI_MOD0_UINT32, val));
}

template <typename vType, typename std::enable_if_t<std::is_base_of<__vIntBase, vType>::value>*>
sfpi_inline vType __vIntBase::operator^(const vType b) const
{
    return __builtin_rvtt_sfpxor(v, b.get());
}

template <typename vType, typename std::enable_if_t<std::is_base_of<__vIntBase, vType>::value>*>
sfpi_inline vType __vIntBase::operator^=(const vType b)
{
    v = __builtin_rvtt_sfpxor(v, b.get());
    return v;
}

sfpi_inline void __vConstFloat::operator=(const float in) const
{
    vFloat tmp = in;
    __builtin_rvtt_sfpconfig_v(tmp.get(), get());
}

sfpi_inline void __vConstFloat::operator=(const s2vFloat16 in) const
{
    vFloat tmp = in;
    __builtin_rvtt_sfpconfig_v(tmp.get(), get());
}

sfpi_inline void __vConstIntBase::operator=(const int in) const
{
    vInt tmp = in;
    __builtin_rvtt_sfpconfig_v(tmp.get(), get());
}

enum class LRegs {
    LReg0 = 0,
    LReg1 = 1,
    LReg2 = 2,
    LReg3 = 3,
    LReg4 = 4,
    LReg5 = 5,
    LReg6 = 6,
    LReg7 = 7,
    LRegCount = SFP_LREG_COUNT,
};

} // namespace sfpi
