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
sfpi_inline vCondComp vDReg::operator==(const float x) const {return vCondComp(vCondComp::CompEQ, vFloat(*this), x); }
sfpi_inline vCondComp vDReg::operator!=(const float x) const { return vCondComp(vCondComp::CompNE, vFloat(*this), x); }
sfpi_inline vCondComp vDReg::operator<(const float x) const { return vCondComp(vCondComp::CompLT, vFloat(*this), x); }
sfpi_inline vCondComp vDReg::operator<=(const float x) const { return vCondComp(vCondComp::CompLTE, vFloat(*this), x, true, false); }
sfpi_inline vCondComp vDReg::operator>(const float x) const { return vCondComp(vCondComp::CompGT, vFloat(*this), x, false, true); }
sfpi_inline vCondComp vDReg::operator>=(const float x) const { return vCondComp(vCondComp::CompGTE, vFloat(*this), x); }

template <>
sfpi_inline void vDReg::operator=(const vFloat vec) const
{
    __builtin_rvtt_sfpstore(vec.get(), SFPSTORE_MOD0_FMT_SRCB, SFPSTORE_ADDR_MODE_NOINC, reg);
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
    __builtin_rvtt_sfpstore(vec.get(), SFPSTORE_MOD0_FMT_SRCB, SFPSTORE_ADDR_MODE_NOINC, reg);
}

sfpi_inline void vDReg::operator=(const vDReg dreg) const
{
    vFloat tmp = dreg;
    __builtin_rvtt_sfpstore(tmp.get(), SFPSTORE_MOD0_FMT_SRCB, SFPSTORE_ADDR_MODE_NOINC, reg);
}

sfpi_inline void vDReg::operator=(const vConstFloat creg) const
{
    __rvtt_vec_t lr = __builtin_rvtt_sfpassignlr(creg.get());
    __builtin_rvtt_sfpstore(lr, SFPSTORE_MOD0_FMT_SRCB, SFPSTORE_ADDR_MODE_NOINC, reg);
}

//////////////////////////////////////////////////////////////////////////////
sfpi_inline vCondComp vFloat::operator==(const float x) const { return vCondComp(vCondComp::CompEQ, *this, x); }
sfpi_inline vCondComp vFloat::operator!=(const float x) const { return vCondComp(vCondComp::CompNE, *this, x); }
sfpi_inline vCondComp vFloat::operator<(const float x) const { return vCondComp(vCondComp::CompLT, *this, x); }
sfpi_inline vCondComp vFloat::operator<=(const float x) const { return vCondComp(vCondComp::CompLTE, *this, x, true, false); }
sfpi_inline vCondComp vFloat::operator>(const float x) const { return vCondComp(vCondComp::CompGT, *this, x, false, true); }
sfpi_inline vCondComp vFloat::operator>=(const float x) const { return vCondComp(vCondComp::CompGTE, *this, x); }

sfpi_inline void vFloat::operator-=(const vFloat a)
{
    __rvtt_vec_t neg1 = __builtin_rvtt_sfpassignlr(vConstNeg1.get());
    assign(__builtin_rvtt_sfpmad(neg1, a.get(), v, SFPMAD_MOD1_OFFSET_NONE));
}

sfpi_inline vFloat::vFloat(const vDReg dreg)
{
    v = __builtin_rvtt_sfpload(SFPLOAD_MOD0_FMT_SRCB, SFPLOAD_ADDR_MODE_NOINC, dreg.get());
    initialized = true;
}

sfpi_inline void vFloat::loadf(const float val)
{
    assign(__builtin_rvtt_sfploadi_ex(SFPLOADI_EX_MOD0_FLOAT, f32asui(val)));
}

//////////////////////////////////////////////////////////////////////////////
sfpi_inline void vIntBase::loadsi(int32_t val)
{
    assign(__builtin_rvtt_sfploadi_ex(SFPLOADI_EX_MOD0_INT32, val));
}

sfpi_inline void vIntBase::loadui(uint32_t val)
{
    assign(__builtin_rvtt_sfploadi_ex(SFPLOADI_EX_MOD0_UINT32, val));
}

template <typename vType, typename std::enable_if_t<std::is_base_of<vIntBase, vType>::value>*>
sfpi_inline vType vIntBase::operator^(const vType b) const
{
    return __builtin_rvtt_sfpxor(v, b.get());
}

template <typename vType, typename std::enable_if_t<std::is_base_of<vIntBase, vType>::value>*>
sfpi_inline void vIntBase::operator^=(const vType b)
{
    v = __builtin_rvtt_sfpxor(v, b.get());
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
sfpi_inline void vCond::emit(bool negate) const
{
    uint32_t use_mod1 = negate ? neg_mod1 : mod1;

    switch (type) {
    case vCond::vCondOpType::CompareFloat:
        __builtin_rvtt_sfpscmp_ex(op_a.get_vec().get(), op_b.get_uint(),
                                  use_mod1 | SFPSCMP_EX_MOD1_FMT_FLOAT);
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
    LReg4 = 4,
    LReg5 = 5,
    LReg6 = 6,
    LReg7 = 7,
    LRegCount = SFP_LREG_COUNT,
};

LRegAssigner::LRegAssigner() : lregs{LRegs::LReg0, LRegs::LReg1, LRegs::LReg2, LRegs::LReg3, LRegs::LReg4, LRegs::LReg5, LRegs::LReg6, LRegs::LReg7} {}

} // namespace sfpi
