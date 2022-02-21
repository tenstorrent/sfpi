///////////////////////////////////////////////////////////////////////////////
// sfpi_imp.h: SFPu Interface implementation for Grayskull
///////////////////////////////////////////////////////////////////////////////

#pragma once

namespace sfpi {

//////////////////////////////////////////////////////////////////////////////
constexpr vConst vConst0(CREG_IDX_0);
constexpr vConst vConst0p6929(CREG_IDX_0P692871094);
constexpr vConst vConstNeg1p0068(CREG_IDX_NEG_1P00683594);
constexpr vConst vConst1p4424(CREG_IDX_1P442382813);
constexpr vConst vConst0p8369(CREG_IDX_0P836914063);
constexpr vConst vConstNeg0p5(CREG_IDX_NEG_0P5);
constexpr vConst vConst1(CREG_IDX_1);
constexpr vConst vConstNeg1(CREG_IDX_NEG_1);
constexpr vConst vConst0p0020(CREG_IDX_0P001953125);
constexpr vConst vConstNeg0p6748(CREG_IDX_NEG_0P67480469);
constexpr vConst vConstNeg0p3447(CREG_IDX_NEG_0P34472656);
constexpr vConst vConstTileId(CREG_IDX_TILEID);

constexpr RegFile<vDReg, 64> dst_reg;

//////////////////////////////////////////////////////////////////////////////
namespace sfpi_int {

sfpi_inline vFloat fp_add(const vFloat a, const vFloat b)
{
    return __builtin_rvtt_sfpadd(a.get(), b.get(), 0);
}

sfpi_inline vFloat fp_sub(const vFloat a, const vFloat b)
{
    __rvtt_vec_t neg1 = __builtin_rvtt_sfpassignlr(vConstNeg1.get());
    return __builtin_rvtt_sfpmad(neg1, b.get(), a.get(), 0);
}

sfpi_inline vFloat fp_mul(const vFloat a, const vFloat b)
{
    return __builtin_rvtt_sfpmul(a.get(), b.get(), 0);
}

}

//////////////////////////////////////////////////////////////////////////////
template<class TYPE, int N>
constexpr TYPE RegFile<TYPE, N>::operator[](const int x) const {
    return TYPE(vRegBaseInitializer(x));
}

sfpi_inline vFloat vDReg::operator+(const vFloat b) const {return sfpi_int::fp_add(vFloat(*this), b); }
sfpi_inline vFloat vDReg::operator-(const vFloat b) const { return sfpi_int::fp_sub(vFloat(*this), b); }
sfpi_inline vFloat vDReg::operator*(const vFloat b) const  { return sfpi_int::fp_mul(vFloat(*this), b); }
sfpi_inline vCondComp vDReg::operator==(const float x) const {return vCondComp(vCondComp::CompEQ, vFloat(*this), s2vFloat16(x)); }
sfpi_inline vCondComp vDReg::operator!=(const float x) const { return vCondComp(vCondComp::CompNE, vFloat(*this), s2vFloat16(x)); }
sfpi_inline vCondComp vDReg::operator<(const float x) const { return vCondComp(vCondComp::CompLT, vFloat(*this), s2vFloat16(x)); }
sfpi_inline vCondComp vDReg::operator<=(const float x) const { return vCondComp(vCondComp::CompLTE, vFloat(*this), s2vFloat16(x), true, false); }
sfpi_inline vCondComp vDReg::operator>(const float x) const { return vCondComp(vCondComp::CompGT, vFloat(*this), s2vFloat16(x), false, true); }
sfpi_inline vCondComp vDReg::operator>=(const float x) const { return vCondComp(vCondComp::CompGTE, vFloat(*this), s2vFloat16(x)); }
sfpi_inline vCondComp vDReg::operator==(const s2vFloat16 x) const {return vCondComp(vCondComp::CompEQ, vFloat(*this), x); }
sfpi_inline vCondComp vDReg::operator!=(const s2vFloat16 x) const { return vCondComp(vCondComp::CompNE, vFloat(*this), x); }
sfpi_inline vCondComp vDReg::operator<(const s2vFloat16 x) const { return vCondComp(vCondComp::CompLT, vFloat(*this), x); }
sfpi_inline vCondComp vDReg::operator<=(const s2vFloat16 x) const { return vCondComp(vCondComp::CompLTE, vFloat(*this), x, true, false); }
sfpi_inline vCondComp vDReg::operator>(const s2vFloat16 x) const { return vCondComp(vCondComp::CompGT, vFloat(*this), x, false, true); }
sfpi_inline vCondComp vDReg::operator>=(const s2vFloat16 x) const { return vCondComp(vCondComp::CompGTE, vFloat(*this), x); }
sfpi_inline vCondComp vDReg::operator==(const vFloat x) const {return vCondComp(vCondComp::CompEQ, vFloat(*this), x); }
sfpi_inline vCondComp vDReg::operator!=(const vFloat x) const { return vCondComp(vCondComp::CompNE, vFloat(*this), x); }
sfpi_inline vCondComp vDReg::operator<(const vFloat x) const { return vCondComp(vCondComp::CompLT, vFloat(*this), x); }
sfpi_inline vCondComp vDReg::operator<=(const vFloat x) const { return vCondComp(vCondComp::CompLTE, vFloat(*this), x, true, false); }
sfpi_inline vCondComp vDReg::operator>(const vFloat x) const { return vCondComp(vCondComp::CompGT, vFloat(*this), x, false, true); }
sfpi_inline vCondComp vDReg::operator>=(const vFloat x) const { return vCondComp(vCondComp::CompGTE, vFloat(*this), x); }

template <>
sfpi_inline void vDReg::operator=(const vFloat vec) const
{
    // For  half, rebias
    __builtin_rvtt_sfpstore(vec.get(), SFPSTORE_MOD0_FLOAT_REBIAS_EXP, reg);
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
    // For short/ushort, do not rebias
    __builtin_rvtt_sfpstore(vec.get(), SFPSTORE_MOD0_INT, reg);
}

sfpi_inline void vDReg::operator=(const vDReg dreg) const
{
    vFloat tmp = dreg;
    __builtin_rvtt_sfpstore(tmp.get(), SFPSTORE_MOD0_FLOAT_REBIAS_EXP, reg);
}

sfpi_inline void vDReg::operator=(const vConst creg) const
{
    __rvtt_vec_t lr = __builtin_rvtt_sfpassignlr(creg.get());
    __builtin_rvtt_sfpstore(lr, SFPSTORE_MOD0_FLOAT_REBIAS_EXP, reg);
}

sfpi_inline vFloat vDReg::operator-() const
{
    vFloat tmp = *this;
    return __builtin_rvtt_sfpmov(tmp.get(), SFPMOV_MOD1_COMPSIGN);
}

//////////////////////////////////////////////////////////////////////////////
sfpi_inline vFloat vConst::operator+(const vFloat b) const { return sfpi_int::fp_add(vFloat(*this), b); }
sfpi_inline vFloat vConst::operator-(const vFloat b) const { return sfpi_int::fp_sub(vFloat(*this), b); }
sfpi_inline vFloat vConst::operator*(const vFloat b) const { return sfpi_int::fp_mul(vFloat(*this), b); }
sfpi_inline vCondComp vConst::operator==(const vFloat x) const { return vCondComp(vCondComp::CompEQ, *this, x); }
sfpi_inline vCondComp vConst::operator!=(const vFloat x) const { return vCondComp(vCondComp::CompNE, *this, x); }
sfpi_inline vCondComp vConst::operator<(const vFloat x) const { return vCondComp(vCondComp::CompLT, *this, x); }
sfpi_inline vCondComp vConst::operator<=(const vFloat x) const { return vCondComp(vCondComp::CompLTE, vFloat(*this), x, true, false); }
sfpi_inline vCondComp vConst::operator>(const vFloat x) const { return vCondComp(vCondComp::CompGT, vFloat(*this), x, false, true); }
sfpi_inline vCondComp vConst::operator>=(const vFloat x) const { return vCondComp(vCondComp::CompGTE, *this, x); }

sfpi_inline vIntBase vConst::operator<<(uint32_t amt) const
{
    __rvtt_vec_t v = __builtin_rvtt_sfpassignlr(reg);
    return __builtin_rvtt_sfpshft_i(v, amt);
}

sfpi_inline vUInt vConst::operator>>(uint32_t amt) const
{
    __rvtt_vec_t v = __builtin_rvtt_sfpassignlr(reg);
    return __builtin_rvtt_sfpshft_i(v, -amt);
}

//////////////////////////////////////////////////////////////////////////////
sfpi_inline vFloat vFloat::operator+(const vFloat b) const { return sfpi_int::fp_add(*this, b); }
sfpi_inline vFloat vFloat::operator-(const vFloat b) const { return sfpi_int::fp_sub(*this, b); }
sfpi_inline vFloat vFloat::operator-(const float b) const { return sfpi_int::fp_add(*this, vFloat(-b)); }
sfpi_inline vFloat vFloat::operator-(const s2vFloat16 b) const { return sfpi_int::fp_add(*this, b.negate()); }
sfpi_inline vFloat vFloat::operator*(const vFloat b) const { return sfpi_int::fp_mul(*this, b); }
sfpi_inline vCondComp vFloat::operator==(const float x) const { return vCondComp(vCondComp::CompEQ, *this, s2vFloat16(x)); }
sfpi_inline vCondComp vFloat::operator!=(const float x) const { return vCondComp(vCondComp::CompNE, *this, s2vFloat16(x)); }
sfpi_inline vCondComp vFloat::operator<(const float x) const { return vCondComp(vCondComp::CompLT, *this, s2vFloat16(x)); }
sfpi_inline vCondComp vFloat::operator<=(const float x) const { return vCondComp(vCondComp::CompLTE, *this, s2vFloat16(x), true, false); }
sfpi_inline vCondComp vFloat::operator>(const float x) const { return vCondComp(vCondComp::CompGT, *this, s2vFloat16(x), false, true); }
sfpi_inline vCondComp vFloat::operator>=(const float x) const { return vCondComp(vCondComp::CompGTE, *this, s2vFloat16(x)); }
sfpi_inline vCondComp vFloat::operator==(const s2vFloat16 x) const { return vCondComp(vCondComp::CompEQ, *this, x); }
sfpi_inline vCondComp vFloat::operator!=(const s2vFloat16 x) const { return vCondComp(vCondComp::CompNE, *this, x); }
sfpi_inline vCondComp vFloat::operator<(const s2vFloat16 x) const { return vCondComp(vCondComp::CompLT, *this, x); }
sfpi_inline vCondComp vFloat::operator<=(const s2vFloat16 x) const { return vCondComp(vCondComp::CompLTE, *this, x, true, false); }
sfpi_inline vCondComp vFloat::operator>(const s2vFloat16 x) const { return vCondComp(vCondComp::CompGT, *this, x, false, true); }
sfpi_inline vCondComp vFloat::operator>=(const s2vFloat16 x) const { return vCondComp(vCondComp::CompGTE, *this, x); }
sfpi_inline vCondComp vFloat::operator==(const vFloat x) const { return vCondComp(vCondComp::CompEQ, *this, x); }
sfpi_inline vCondComp vFloat::operator!=(const vFloat x) const { return vCondComp(vCondComp::CompNE, *this, x); }
sfpi_inline vCondComp vFloat::operator<(const vFloat x) const { return vCondComp(vCondComp::CompLT, *this, x); }
sfpi_inline vCondComp vFloat::operator<=(const vFloat x) const { return vCondComp(vCondComp::CompLTE, *this, x, true, false); }
sfpi_inline vCondComp vFloat::operator>(const vFloat x) const { return vCondComp(vCondComp::CompGT, *this, x, false, true); }
sfpi_inline vCondComp vFloat::operator>=(const vFloat x) const { return vCondComp(vCondComp::CompGTE, *this, x); }

sfpi_inline void vFloat::operator*=(const vFloat m)
{
    assign(__builtin_rvtt_sfpmul(v, m.get(), SFPMAD_MOD1_OFFSET_NONE));
}

sfpi_inline void vFloat::operator+=(const vFloat a)
{
    assign(__builtin_rvtt_sfpadd(v, a.get(), SFPMAD_MOD1_OFFSET_NONE));
}

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

sfpi_inline vFloat::vFloat(const vConst creg)
{
    v = __builtin_rvtt_sfpassignlr(creg.get());
    initialized = true;
}

sfpi_inline vFloat vFloat::operator-() const
{
    return __builtin_rvtt_sfpmov(v, SFPMOV_MOD1_COMPSIGN);
}

sfpi_inline void vFloat::loadf(const float val)
{
    loadf16(s2vFloat16b(val));
}

sfpi_inline void vFloat::loadf16(const s2vFloat16 val)
{
    assign(__builtin_rvtt_sfploadi_ex(val.get_format(), val.get()));
}

//////////////////////////////////////////////////////////////////////////////
sfpi_inline void vIntBase::loadss(int16_t val)
{
    assign(__builtin_rvtt_sfploadi_ex(SFPLOADI_MOD0_SHORT, val));
}

sfpi_inline void vIntBase::loadus(uint16_t val)
{
    assign(__builtin_rvtt_sfploadi_ex(SFPLOADI_MOD0_USHORT, val));
}

sfpi_inline void vIntBase::loadsi(int32_t val)
{
    assign(__builtin_rvtt_sfploadi_ex(SFPLOADI_MOD0_SHORT, val));
}

sfpi_inline void vIntBase::loadui(uint32_t val)
{
    assign(__builtin_rvtt_sfploadi_ex(SFPLOADI_MOD0_USHORT, val));
}

template <typename vType, typename std::enable_if_t<std::is_base_of<vIntBase, vType>::value>*>
sfpi_inline vType vIntBase::operator&(const vType b) const
{
    return __builtin_rvtt_sfpand(v, b.get());
}

template <typename vType, typename std::enable_if_t<std::is_base_of<vIntBase, vType>::value>*>
sfpi_inline void vIntBase::operator&=(const vType b)
{
    v = __builtin_rvtt_sfpand(v, b.get());
}

template <typename vType, typename std::enable_if_t<std::is_base_of<vIntBase, vType>::value>*>
sfpi_inline vType vIntBase::operator|(const vType b) const
{
    return __builtin_rvtt_sfpor(v, b.get());
}

template <typename vType, typename std::enable_if_t<std::is_base_of<vIntBase, vType>::value>*>
sfpi_inline void vIntBase::operator|=(const vType b)
{
    v = __builtin_rvtt_sfpor(v, b.get());
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
    __rvtt_vec_t tmp1, tmp2, ntmp2;

    tmp1 = __builtin_rvtt_sfpor(v, b.get());
    tmp2 = __builtin_rvtt_sfpand(v, b.get());
    ntmp2 = __builtin_rvtt_sfpnot(tmp2);

    v = __builtin_rvtt_sfpand(tmp1, ntmp2);
}

template <typename vType, typename std::enable_if_t<std::is_base_of<vIntBase, vType>::value>*>
sfpi_inline vType vIntBase::operator~() const
{
    return __builtin_rvtt_sfpnot(v);
}

template <typename vType, typename std::enable_if_t<std::is_base_of<vIntBase, vType>::value>*>
sfpi_inline vType vIntBase::operator<<(uint32_t amt) const
{
    return __builtin_rvtt_sfpshft_i(v, amt);
}

template <typename vType, typename std::enable_if_t<std::is_base_of<vIntBase, vType>::value>*>
sfpi_inline void vIntBase::operator<<=(uint32_t amt)
{
    assign((static_cast<vType>(*this) << amt).get());
}

template <typename vType, typename std::enable_if_t<std::is_base_of<vIntBase, vType>::value>*>
sfpi_inline vType vIntBase::add(int32_t val, unsigned int mod_base) const
{
    return __builtin_rvtt_sfpiadd_i_ex(v, val, mod_base);
}

template <typename vType, typename std::enable_if_t<std::is_base_of<vIntBase, vType>::value>*>
sfpi_inline vType vIntBase::operator+(const vIntBase val) const
{
    return __builtin_rvtt_sfpiadd_v_ex(val.get(), v, 0);
}

template <typename vType, typename std::enable_if_t<std::is_base_of<vIntBase, vType>::value>*>
sfpi_inline vType vIntBase::operator+(const vConst val) const
{
    __rvtt_vec_t c = __builtin_rvtt_sfpassignlr(val.get());
    return __builtin_rvtt_sfpiadd_v_ex(c, v, 0);
}

template <typename vType, typename std::enable_if_t<std::is_base_of<vIntBase, vType>::value>*>
sfpi_inline vType vIntBase::sub(int32_t val, unsigned int mod_base) const
{
    return __builtin_rvtt_sfpiadd_i_ex(v, val, mod_base | SFPIADD_EX_MOD1_IS_SUB);
}

template <typename vType, typename std::enable_if_t<std::is_base_of<vIntBase, vType>::value>*>
sfpi_inline vType vIntBase::operator-(const vIntBase val) const
{
    return __builtin_rvtt_sfpiadd_v_ex(val.get(), v, SFPIADD_EX_MOD1_IS_SUB);
}

template <typename vType, typename std::enable_if_t<std::is_base_of<vIntBase, vType>::value>*>
sfpi_inline vType vIntBase::operator-(const vConst val) const
{
    __rvtt_vec_t c = __builtin_rvtt_sfpassignlr(val.get());
    return __builtin_rvtt_sfpiadd_v_ex(c, v, SFPIADD_EX_MOD1_IS_SUB);
}

template <typename vType, typename std::enable_if_t<std::is_base_of<vIntBase, vType>::value>*>
sfpi_inline void vIntBase::add_eq(int32_t val, unsigned int mod_base)
{
    assign(__builtin_rvtt_sfpiadd_i_ex(v, val, mod_base));
}

template <typename vType, typename std::enable_if_t<std::is_base_of<vIntBase, vType>::value>*>
sfpi_inline void vIntBase::operator+=(const vIntBase val)
{
    assign(__builtin_rvtt_sfpiadd_v_ex(v, val.get(), 0));
}

template <typename vType, typename std::enable_if_t<std::is_base_of<vIntBase, vType>::value>*>
sfpi_inline void vIntBase::operator+=(const vConst val)
{
    __rvtt_vec_t c = __builtin_rvtt_sfpassignlr(val.get());
    assign(__builtin_rvtt_sfpiadd_v_ex(c, v, 0));
}

template <typename vType, typename std::enable_if_t<std::is_base_of<vIntBase, vType>::value>*>
sfpi_inline void vIntBase::sub_eq(int32_t val, unsigned int mod_base)
{
    assign(__builtin_rvtt_sfpiadd_i_ex(v, val, mod_base | SFPIADD_EX_MOD1_IS_SUB));
}

template <typename vType, typename std::enable_if_t<std::is_base_of<vIntBase, vType>::value>*>
sfpi_inline void vIntBase::operator-=(const vIntBase val)
{
    assign(__builtin_rvtt_sfpiadd_v_ex(val.get(), v, SFPIADD_EX_MOD1_IS_SUB));
}

template <typename vType, typename std::enable_if_t<std::is_base_of<vIntBase, vType>::value>*>
sfpi_inline void vIntBase::operator-=(const vConst val)
{
    __rvtt_vec_t c = __builtin_rvtt_sfpassignlr(val.get());
    assign(__builtin_rvtt_sfpiadd_v_ex(c, v, SFPIADD_EX_MOD1_IS_SUB));
}

sfpi_inline vUInt vUInt::operator>>(uint32_t amt) const
{
    return __builtin_rvtt_sfpshft_i(v, -amt);
}

sfpi_inline void vUInt::operator>>=(uint32_t amt)
{
    assign((*this >> amt).get());
}

// Recursively process the conditional tree
// gcc inline bails out due to complexity and always_inline doesn't allow
// recursive inlining.  Instead, manually expand the code with "recursive"
// macro usage up to reasonable compile time limits.  Eventually this has to
// be pushed down into the compiler
// Use De Morgan's laws to convert || to !&& pass the ! down as we go
// Each node may be negated from above, if the current node is an OR, then it
// always negates itself and upates the propagated negate by flipping it
//
// Descending the LHS uses the last PUSHC as the "gate" against which a COMPC
// can be issued, however, descending the RHS would mess up the results from
// the LHS w/o a new gate, hence the PUSHC prior to the RHS.  The POPC would
// destroy the results of the RHS and so those results are saved/restored with
// saved_enables.  This uses 1 reg per level for now, which could be optimized
// to just 1 reg total w/ some compiler work to clean up unused LOADIs that
// are set multiple times to the same register (this is a tradeoff of
// imperfect performance for what is likely a rare case anyway).
#define __sfpi_cond_emit_loop(leftside, rightside)                      \
{                                                                       \
    const vBool* local_node = node;                                   \
    bool negate_node = node->negate;                                    \
    bool negate_child = negate;                                         \
    bool descended_right = false;                                       \
    vInt saved_enables = 1;                                         \
                                                                        \
    vBool::vBoolType node_type = negate ? node->get_neg_type() : node->get_type(); \
    if (node_type == vBool::vBoolType::Or) {                        \
        negate_node = !negate_node;                                     \
        negate_child = !negate_child;                                   \
    }                                                                   \
                                                                        \
    if (node->op_a.get_type() == vCondOperand::Type::vBool) {          \
        node = &node->op_a.get_cond();                                  \
        leftside;                                                       \
        node = local_node;                                              \
    } else {                                                            \
        node->op_a.get_op().emit(negate_child);                         \
    }                                                                   \
                                                                        \
    if (node->op_b.get_type() == vCondOperand::Type::vBool) {          \
        cc->push();                                                     \
        node = &node->op_b.get_cond();                                  \
        rightside;                                                      \
        descended_right = true;                                         \
        node = local_node;                                              \
    } else {                                                            \
        bool restore = false;                                           \
        if (node->op_b.get_op().issues_compc(negate_child)) {           \
            restore = true;                                             \
            cc->push();                                                 \
        }                                                               \
        node->op_b.get_op().emit(negate_child);                         \
        if (restore) {                                                  \
            saved_enables = 0;                                          \
            cc->pop();                                                  \
            __builtin_rvtt_sfpsetcc_v(saved_enables.get(), SFPSETCC_MOD1_LREG_EQ0); \
        }                                                               \
    }                                                                   \
                                                                        \
    if (descended_right) {                                              \
        saved_enables = 0;                                              \
        cc->pop();                                                      \
        __builtin_rvtt_sfpsetcc_v(saved_enables.get(), SFPSETCC_MOD1_LREG_EQ0); \
    }                                                                   \
    if (negate_node) {                                                  \
        __builtin_rvtt_sfpcompc();                                      \
    }                                                                   \
}

#define __sfpi_cond_emit_loop_mid(leftside, rightside)                  \
    bool negate = negate_child;                                         \
    __sfpi_cond_emit_loop(leftside, rightside)

#define __sfpi_cond_emit_error __builtin_rvtt_sfpillegal();
sfpi_inline void vBool::emit(vCCCtrl *cc, bool negate) const
{
    const vBool* node = this;

    __sfpi_cond_emit_loop(
        __sfpi_cond_emit_loop_mid(__sfpi_cond_emit_loop_mid(__sfpi_cond_emit_error, __sfpi_cond_emit_error),
                                  __sfpi_cond_emit_loop_mid(__sfpi_cond_emit_error, __sfpi_cond_emit_error)),
        __sfpi_cond_emit_loop_mid(__sfpi_cond_emit_loop_mid(__sfpi_cond_emit_error, __sfpi_cond_emit_error),
                                  __sfpi_cond_emit_loop_mid(__sfpi_cond_emit_error, __sfpi_cond_emit_error)));
}

sfpi_inline vCond::vCond(vCondOpType t, const vInt a, int32_t i, uint32_t m, uint32_t nm, bool c, bool nc) :
 type(t), op_a(a), op_b(), imm(i), mod1(m), neg_mod1(nm), comp(c), neg_comp(nc)
{
}

sfpi_inline const vCond vCond::operator!() const
{
    vCond result(*this);

    result.mod1 = neg_mod1;
    result.neg_mod1 = mod1;

    return result;
}

sfpi_inline void vCond::emit(bool negate) const
{
    uint32_t use_mod1 = negate ? neg_mod1 : mod1;

    switch (type) {
    case vCond::vCondOpType::CompareFloat:
        __builtin_rvtt_sfpscmp_ex(op_a.get_vec().get(), op_b.get_scalarfp().get(),
                                  use_mod1 | (op_b.get_scalarfp().get_format() ==
                                              SFPLOADI_MOD0_FLOATA ? SFPSCMP_EX_MOD1_FMT_A : 0));
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
sfpi_inline vCCCtrl::vCCCtrl() : push_count(0)
{
    push();
}

sfpi_inline void vCCCtrl::cc_if(const vBool& op)
{
    op.emit(this);
}

sfpi_inline void vCCCtrl::cc_if(const vCond& op)
{
    op.emit(false);
}

sfpi_inline void vCCCtrl::cc_elseif(const vBool& op)
{
    cc_if(op);
}

sfpi_inline void vCCCtrl::cc_elseif(const vCond& op)
{
    cc_if(op);
}

sfpi_inline void vCCCtrl::cc_else() const
{
    __builtin_rvtt_sfpcompc();
}

sfpi_inline vCCCtrl::~vCCCtrl()
{
    while (push_count != 0) {
        pop();
    }
}

sfpi_inline void vCCCtrl::push()
{
    push_count++;
    __builtin_rvtt_sfppushc();
}

sfpi_inline void vCCCtrl::pop()
{
    push_count--;
    __builtin_rvtt_sfppopc();
}

sfpi_inline void vCCCtrl::enable_cc()
{
    __builtin_rvtt_sfpencc(SFPENCC_IMM12_BOTH, SFPENCC_MOD1_EI_RI);
}

} // namespace sfpi
