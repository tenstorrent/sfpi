///////////////////////////////////////////////////////////////////////////////
// sfpi_imp.h: SFPu Interface implementation for Grayskull
///////////////////////////////////////////////////////////////////////////////

#pragma once

namespace sfpi {

//////////////////////////////////////////////////////////////////////////////
constexpr vConstFloat vConst0p8373(CREG_IDX_0P837300003);
constexpr vConstFloat vConstFloatPrgm0(CREG_IDX_PRGM1);
constexpr vConstFloat vConstFloatPrgm1(CREG_IDX_PRGM2);
constexpr vConstFloat vConstFloatPrgm2(CREG_IDX_PRGM3);

constexpr vConstIntBase vConstTileId(CREG_IDX_TILEID);
constexpr vConstIntBase vConstIntPrgm0(CREG_IDX_PRGM1);
constexpr vConstIntBase vConstIntPrgm1(CREG_IDX_PRGM2);
constexpr vConstIntBase vConstIntPrgm2(CREG_IDX_PRGM3);

//////////////////////////////////////////////////////////////////////////////
sfpi_inline vCond vDReg::operator==(const float x) const {return vCond(vCond::vCondEQ, vFloat(*this), x); }
sfpi_inline vCond vDReg::operator!=(const float x) const { return vCond(vCond::vCondNE, vFloat(*this), x); }
sfpi_inline vCond vDReg::operator<(const float x) const { return vCond(vCond::vCondLT, vFloat(*this), x); }
sfpi_inline vCond vDReg::operator<=(const float x) const { return vCond(vCond::vCondLTE, vFloat(*this), x); }
sfpi_inline vCond vDReg::operator>(const float x) const { return vCond(vCond::vCondGT, vFloat(*this), x); }
sfpi_inline vCond vDReg::operator>=(const float x) const { return vCond(vCond::vCondGTE, vFloat(*this), x); }

template <>
sfpi_inline vFloat vDReg::operator=(const vFloat vec) const
{
    __builtin_rvtt_sfpstore(vec.get(), SFPSTORE_MOD0_FMT_SRCB, SFPSTORE_ADDR_MODE_NOINC, reg);
    return vec;
}

sfpi_inline vFloat vDReg::operator=(const double d) const
{
    vFloat v(static_cast<float>(d));
    *this = v;
    return v;
}

sfpi_inline vFloat vDReg::operator=(s2vFloat16 f) const
{
    vFloat v(f);
    *this = v;
    return v;
}

sfpi_inline vFloat vDReg::operator=(const float f) const
{
    vFloat v(f);
    *this = v;
    return v;
}

template <typename vecType, typename std::enable_if_t<std::is_base_of<vBase, vecType>::value>*>
sfpi_inline vecType vDReg::operator=(const vecType vec) const
{
    __builtin_rvtt_sfpstore(vec.get(), SFPSTORE_MOD0_FMT_SRCB, SFPSTORE_ADDR_MODE_NOINC, reg);
    return vec;
}

sfpi_inline void vDReg::operator=(const vDReg dreg) const
{
    vFloat tmp = dreg;
    __builtin_rvtt_sfpstore(tmp.get(), SFPSTORE_MOD0_FMT_SRCB, SFPSTORE_ADDR_MODE_NOINC, reg);
}

sfpi_inline vFloat vDReg::operator=(const vConstFloat creg) const
{
    __rvtt_vec_t lr = __builtin_rvtt_sfpassignlr(creg.get());
    __builtin_rvtt_sfpstore(lr, SFPSTORE_MOD0_FMT_SRCB, SFPSTORE_ADDR_MODE_NOINC, reg);
    return vFloat(lr);
}

//////////////////////////////////////////////////////////////////////////////
sfpi_inline vCond vFloat::operator==(const float x) const { return vCond(vCond::vCondEQ, *this, x); }
sfpi_inline vCond vFloat::operator!=(const float x) const { return vCond(vCond::vCondNE, *this, x); }
sfpi_inline vCond vFloat::operator<(const float x) const { return vCond(vCond::vCondLT, *this, x); }
sfpi_inline vCond vFloat::operator<=(const float x) const { return vCond(vCond::vCondLTE, *this, x); }
sfpi_inline vCond vFloat::operator>(const float x) const { return vCond(vCond::vCondGT, *this, x); }
sfpi_inline vCond vFloat::operator>=(const float x) const { return vCond(vCond::vCondGTE, *this, x); }

sfpi_inline vFloat vFloat::operator-=(const vFloat a)
{
    __rvtt_vec_t neg1 = __builtin_rvtt_sfpassignlr(vConstNeg1.get());
    assign(__builtin_rvtt_sfpmad(neg1, a.get(), v, SFPMAD_MOD1_OFFSET_NONE));
    return v;
}

sfpi_inline vFloat::vFloat(const vDReg dreg)
{
    v = __builtin_rvtt_sfpload(SFPLOAD_MOD0_FMT_SRCB, SFPLOAD_ADDR_MODE_NOINC, dreg.get());
    initialized = true;
}

sfpi_inline void vFloat::loadf(const float val)
{
    assign(__builtin_rvtt_sfpxloadi(SFPXLOADI_MOD0_FLOAT, f32asui(val)));
}

//////////////////////////////////////////////////////////////////////////////
sfpi_inline void vIntBase::loadsi(int32_t val)
{
    assign(__builtin_rvtt_sfpxloadi(SFPXLOADI_MOD0_INT32, val));
}

sfpi_inline void vIntBase::loadui(uint32_t val)
{
    assign(__builtin_rvtt_sfpxloadi(SFPXLOADI_MOD0_UINT32, val));
}

template <typename vType, typename std::enable_if_t<std::is_base_of<vIntBase, vType>::value>*>
sfpi_inline vType vIntBase::operator^(const vType b) const
{
    return __builtin_rvtt_sfpxor(v, b.get());
}

template <typename vType, typename std::enable_if_t<std::is_base_of<vIntBase, vType>::value>*>
sfpi_inline vType vIntBase::operator^=(const vType b)
{
    v = __builtin_rvtt_sfpxor(v, b.get());
    return v;
}

sfpi_inline void vConstFloat::operator=(const vFloat in) const
{
    __builtin_rvtt_sfpconfig_v(in.get(), get());
}

sfpi_inline void vConstIntBase::operator=(const vInt in) const
{
    __builtin_rvtt_sfpconfig_v(in.get(), get());
}

//////////////////////////////////////////////////////////////////////////////
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

LRegAssigner::LRegAssigner() : lregs{LRegs::LReg0, LRegs::LReg1, LRegs::LReg2, LRegs::LReg3, LRegs::LReg4, LRegs::LReg5, LRegs::LReg6, LRegs::LReg7} {}

} // namespace sfpi
