///////////////////////////////////////////////////////////////////////////////
// sfpi_imp.h: SFPu Interface implementation for Grayskull
///////////////////////////////////////////////////////////////////////////////

#pragma once

namespace sfpi {

//////////////////////////////////////////////////////////////////////////////
constexpr __vConstFloat vConst0p6929(CREG_IDX_0P692871094);
constexpr __vConstFloat vConstNeg1p0068(CREG_IDX_NEG_1P00683594);
constexpr __vConstFloat vConst1p4424(CREG_IDX_1P442382813);
constexpr __vConstFloat vConst0p8369(CREG_IDX_0P836914063);
constexpr __vConstFloat vConstNeg0p5(CREG_IDX_NEG_0P5);
constexpr __vConstFloat vConst0p0020(CREG_IDX_0P001953125);
constexpr __vConstFloat vConstNeg0p6748(CREG_IDX_NEG_0P67480469);
constexpr __vConstFloat vConstNeg0p3447(CREG_IDX_NEG_0P34472656);
constexpr __vConstIntBase vConstTileId(CREG_IDX_TILEID);

//////////////////////////////////////////////////////////////////////////////
sfpi_inline __vCond __vDReg::operator==(const float x) const {return __vCond(__vCond::__vCondEQ, vFloat(*this), s2vFloat16(x)); }
sfpi_inline __vCond __vDReg::operator!=(const float x) const { return __vCond(__vCond::__vCondNE, vFloat(*this), s2vFloat16(x)); }
sfpi_inline __vCond __vDReg::operator<(const float x) const { return __vCond(__vCond::__vCondLT, vFloat(*this), s2vFloat16(x)); }
sfpi_inline __vCond __vDReg::operator<=(const float x) const { return __vCond(__vCond::__vCondLTE, vFloat(*this), s2vFloat16(x)); }
sfpi_inline __vCond __vDReg::operator>(const float x) const { return __vCond(__vCond::__vCondGT, vFloat(*this), s2vFloat16(x)); }
sfpi_inline __vCond __vDReg::operator>=(const float x) const { return __vCond(__vCond::__vCondGTE, vFloat(*this), s2vFloat16(x)); }

template <>
sfpi_inline vFloat __vDReg::operator=(const vFloat vec) const
{
    __builtin_rvtt_sfpstore(vec.get(), SFPSTORE_MOD0_FLOAT_REBIAS_EXP, reg);
    return vec;
}

sfpi_inline vFloat __vDReg::operator=(const double d) const
{
    *this = static_cast<float>(d);
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
    __builtin_rvtt_sfpstore(vec.get(), SFPSTORE_MOD0_INT, reg);
    return vec;
}

sfpi_inline void __vDReg::operator=(const __vDReg dreg) const
{
    vFloat tmp = dreg;
    __builtin_rvtt_sfpstore(tmp.get(), SFPSTORE_MOD0_FLOAT_REBIAS_EXP, reg);
}

sfpi_inline vFloat __vDReg::operator=(const __vConstFloat creg) const
{
    __rvtt_vec_t lr = __builtin_rvtt_sfpassignlreg(creg.get());
    __builtin_rvtt_sfpstore(lr, SFPSTORE_MOD0_FLOAT_REBIAS_EXP, reg);
    return vFloat(lr);
}

//////////////////////////////////////////////////////////////////////////////
sfpi_inline __vCond vFloat::operator==(const float x) const { return __vCond(__vCond::__vCondEQ, *this, s2vFloat16(x)); }
sfpi_inline __vCond vFloat::operator!=(const float x) const { return __vCond(__vCond::__vCondNE, *this, s2vFloat16(x)); }
sfpi_inline __vCond vFloat::operator<(const float x) const { return __vCond(__vCond::__vCondLT, *this, s2vFloat16(x)); }
sfpi_inline __vCond vFloat::operator<=(const float x) const { return __vCond(__vCond::__vCondLTE, *this, s2vFloat16(x)); }
sfpi_inline __vCond vFloat::operator>(const float x) const { return __vCond(__vCond::__vCondGT, *this, s2vFloat16(x)); }
sfpi_inline __vCond vFloat::operator>=(const float x) const { return __vCond(__vCond::__vCondGTE, *this, s2vFloat16(x)); }

sfpi_inline vFloat vFloat::operator-=(const vFloat a)
{
    __rvtt_vec_t neg1 = __builtin_rvtt_sfpassignlreg(vConstNeg1.get());
    assign(__builtin_rvtt_sfpmad(neg1, a.get(), v, SFPMAD_MOD1_OFFSET_NONE));
    return v;
}

sfpi_inline vFloat::vFloat(const __vDReg dreg)
{
    v = __builtin_rvtt_sfpload(SFPLOAD_MOD0_REBIAS_EXP, dreg.get());
    initialized = true;
}

sfpi_inline void vFloat::loadf(const float val)
{
    loadf16(s2vFloat16b(val));
}

//////////////////////////////////////////////////////////////////////////////
sfpi_inline void __vIntBase::loadsi(int32_t val)
{
    assign(__builtin_rvtt_sfpxloadi(SFPLOADI_MOD0_SHORT, val));
}

sfpi_inline void __vIntBase::loadui(uint32_t val)
{
    assign(__builtin_rvtt_sfpxloadi(SFPLOADI_MOD0_USHORT, val));
}

template <typename vType, typename std::enable_if_t<std::is_base_of<__vIntBase, vType>::value>*>
sfpi_inline vType __vIntBase::operator^(const vType b) const
{
    __rvtt_vec_t tmp1, tmp2, ntmp2;

    tmp1 = __builtin_rvtt_sfpor(v, b.get());
    tmp2 = __builtin_rvtt_sfpand(v, b.get());
    ntmp2 = __builtin_rvtt_sfpnot(tmp2);

    return __builtin_rvtt_sfpand(tmp1, ntmp2);
}

template <typename vType, typename std::enable_if_t<std::is_base_of<__vIntBase, vType>::value>*>
sfpi_inline vType __vIntBase::operator^=(const vType b)
{
    __rvtt_vec_t tmp1;

    tmp1 = __builtin_rvtt_sfpor(v, b.get());
    v = __builtin_rvtt_sfpand(v, b.get());
    v = __builtin_rvtt_sfpnot(v);
    v = __builtin_rvtt_sfpand(v, tmp1);
    return v;
}

} // namespace sfpi
