///////////////////////////////////////////////////////////////////////////////
// sfpi_imp.h: SFPu Interface implementation for Grayskull
///////////////////////////////////////////////////////////////////////////////

#pragma once

namespace sfpi {

//////////////////////////////////////////////////////////////////////////////
constexpr vConstFloat vConst0p6929(CREG_IDX_0P692871094);
constexpr vConstFloat vConstNeg1p0068(CREG_IDX_NEG_1P00683594);
constexpr vConstFloat vConst1p4424(CREG_IDX_1P442382813);
constexpr vConstFloat vConst0p8369(CREG_IDX_0P836914063);
constexpr vConstFloat vConstNeg0p5(CREG_IDX_NEG_0P5);
constexpr vConstFloat vConst0p0020(CREG_IDX_0P001953125);
constexpr vConstFloat vConstNeg0p6748(CREG_IDX_NEG_0P67480469);
constexpr vConstFloat vConstNeg0p3447(CREG_IDX_NEG_0P34472656);
constexpr vConstIntBase vConstTileId(CREG_IDX_TILEID);

//////////////////////////////////////////////////////////////////////////////
sfpi_inline vCondComp vDReg::operator==(const float x) const {return vCondComp(vCondComp::CompEQ, vFloat(*this), s2vFloat16(x)); }
sfpi_inline vCondComp vDReg::operator!=(const float x) const { return vCondComp(vCondComp::CompNE, vFloat(*this), s2vFloat16(x)); }
sfpi_inline vCondComp vDReg::operator<(const float x) const { return vCondComp(vCondComp::CompLT, vFloat(*this), s2vFloat16(x)); }
sfpi_inline vCondComp vDReg::operator<=(const float x) const { return vCondComp(vCondComp::CompLTE, vFloat(*this), s2vFloat16(x), true, false); }
sfpi_inline vCondComp vDReg::operator>(const float x) const { return vCondComp(vCondComp::CompGT, vFloat(*this), s2vFloat16(x), false, true); }
sfpi_inline vCondComp vDReg::operator>=(const float x) const { return vCondComp(vCondComp::CompGTE, vFloat(*this), s2vFloat16(x)); }

template <>
sfpi_inline void vDReg::operator=(const vFloat vec) const
{
    __builtin_rvtt_sfpstore(vec.get(), SFPSTORE_MOD0_FLOAT_REBIAS_EXP, reg);
}

sfpi_inline void vDReg::operator=(const double d) const
{
    *this = static_cast<float>(d);
}

sfpi_inline void vDReg::operator=(s2vFloat16 f) const
{
    vFloat v(f);
    *this = v;
}

sfpi_inline void vDReg::operator=(const float f) const
{
    vFloat v(f);
    *this = v;
}

template <typename vecType, typename std::enable_if_t<std::is_base_of<vBase, vecType>::value>*>
sfpi_inline void vDReg::operator=(const vecType vec) const
{
    __builtin_rvtt_sfpstore(vec.get(), SFPSTORE_MOD0_INT, reg);
}

sfpi_inline void vDReg::operator=(const vDReg dreg) const
{
    vFloat tmp = dreg;
    __builtin_rvtt_sfpstore(tmp.get(), SFPSTORE_MOD0_FLOAT_REBIAS_EXP, reg);
}

sfpi_inline void vDReg::operator=(const vConstFloat creg) const
{
    __rvtt_vec_t lr = __builtin_rvtt_sfpassignlr(creg.get());
    __builtin_rvtt_sfpstore(lr, SFPSTORE_MOD0_FLOAT_REBIAS_EXP, reg);
}

//////////////////////////////////////////////////////////////////////////////
sfpi_inline vCondComp vFloat::operator==(const float x) const { return vCondComp(vCondComp::CompEQ, *this, s2vFloat16(x)); }
sfpi_inline vCondComp vFloat::operator!=(const float x) const { return vCondComp(vCondComp::CompNE, *this, s2vFloat16(x)); }
sfpi_inline vCondComp vFloat::operator<(const float x) const { return vCondComp(vCondComp::CompLT, *this, s2vFloat16(x)); }
sfpi_inline vCondComp vFloat::operator<=(const float x) const { return vCondComp(vCondComp::CompLTE, *this, s2vFloat16(x), true, false); }
sfpi_inline vCondComp vFloat::operator>(const float x) const { return vCondComp(vCondComp::CompGT, *this, s2vFloat16(x), false, true); }
sfpi_inline vCondComp vFloat::operator>=(const float x) const { return vCondComp(vCondComp::CompGTE, *this, s2vFloat16(x)); }

sfpi_inline void vFloat::operator-=(const vFloat a)
{
    __rvtt_vec_t neg1 = __builtin_rvtt_sfpassignlr(vConstNeg1.get());
    assign(__builtin_rvtt_sfpmad(neg1, a.get(), v, SFPMAD_MOD1_OFFSET_NONE));
}

sfpi_inline vFloat::vFloat(const vDReg dreg)
{
    v = __builtin_rvtt_sfpload(SFPLOAD_MOD0_REBIAS_EXP, dreg.get());
    initialized = true;
}

sfpi_inline void vFloat::loadf(const float val)
{
    loadf16(s2vFloat16b(val));
}

//////////////////////////////////////////////////////////////////////////////
sfpi_inline void vIntBase::loadsi(int32_t val)
{
    assign(__builtin_rvtt_sfploadi_ex(SFPLOADI_MOD0_SHORT, val));
}

sfpi_inline void vIntBase::loadui(uint32_t val)
{
    assign(__builtin_rvtt_sfploadi_ex(SFPLOADI_MOD0_USHORT, val));
}

template <typename vType, typename std::enable_if_t<std::is_base_of<vIntBase, vType>::value>*>
sfpi_inline vType vIntBase::operator^(const vType b) const
{
    __rvtt_vec_t tmp1, tmp2, ntmp2;

    tmp1 = __builtin_rvtt_sfpor(v, b.get());
    tmp2 = __builtin_rvtt_sfpand(v, b.get());
    ntmp2 = __builtin_rvtt_sfpnot(tmp2);

    return __builtin_rvtt_sfpand(tmp1, ntmp2);
}

template <typename vType, typename std::enable_if_t<std::is_base_of<vIntBase, vType>::value>*>
sfpi_inline void vIntBase::operator^=(const vType b)
{
    __rvtt_vec_t tmp1;

    tmp1 = __builtin_rvtt_sfpor(v, b.get());
    v = __builtin_rvtt_sfpand(v, b.get());
    v = __builtin_rvtt_sfpnot(v);
    v = __builtin_rvtt_sfpand(v, tmp1);
}

//////////////////////////////////////////////////////////////////////////////
sfpi_inline void vCond::emit(bool negate) const
{
    uint32_t use_mod1 = negate ? neg_mod1 : mod1;

    switch (type) {

    case vCond::vCondOpType::CompareFloat:
        // Not used on Grayskull
        break;

    case vCond::vCondOpType::CompareFloat16:
        __builtin_rvtt_sfpscmp_ex(op_a.get_vec().get(), op_b.get_scalarfp().get(),
                                  use_mod1 | (op_b.get_scalarfp().get_format() ==
                                              SFPLOADI_MOD0_FLOATA ? SFPSCMP_EX_MOD1_FMT_A : SFPSCMP_EX_MOD1_FMT_B));
        break;

    case vCond::vCondOpType::ComparevFloat:
        __builtin_rvtt_sfpvcmp_ex(op_a.get_vec().get(), op_b.get_vec().get(), use_mod1);
        break;

    case vCond::vCondOpType::CompareInt:
        __builtin_rvtt_sfpiadd_i_ex(op_a.get_vec().get(), imm, use_mod1 | SFPIADD_EX_MOD1_IS_SUB);
        break;

    case vCond::vCondOpType::ComparevInt:
        // Remember: dst gets negated
        __builtin_rvtt_sfpiadd_v_ex(op_b.get_vec().get(), op_a.get_vec().get(),
                                    use_mod1 | SFPIADD_EX_MOD1_IS_SUB);
        break;

    case vCond::vCondOpType::ExExp:
        op_b.get_vec_ptr()->get() = __builtin_rvtt_sfpexexp(op_a.get_vec().get(), use_mod1);
        break;

    case vCond::vCondOpType::Lz:
        op_b.get_vec_ptr()->get() = __builtin_rvtt_sfplz(op_a.get_vec().get(), use_mod1);
        break;

    case vCond::vCondOpType::IAddI:
        // This is legacy code for add_cc implementation
        op_b.get_vec_ptr()->get() = __builtin_rvtt_sfpiadd_i(imm, op_a.get_vec().get(), use_mod1);
        break;

    case vCond::vCondOpType::IAddV:
        // This is legacy code for add_cc implementation
        op_b.get_vec_ptr()->get() = __builtin_rvtt_sfpiadd_v(op_b.get_vec_ptr()->get(),
                                                             op_a.get_vec().get(),
                                                             use_mod1);
        break;
    }
}

//////////////////////////////////////////////////////////////////////////////
enum class LRegs {
    LReg0 = 0,
    LReg1 = 1,
    LReg2 = 2,
    LReg3 = 3,
    LRegCount = SFP_LREG_COUNT,
};

LRegAssigner::LRegAssigner() : lregs{LRegs::LReg0, LRegs::LReg1, LRegs::LReg2, LRegs::LReg3} {}

} // namespace sfpi
