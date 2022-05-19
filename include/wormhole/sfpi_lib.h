///////////////////////////////////////////////////////////////////////////////
// sfpi_lib.h: SFPu Interface library implementation for Grayskull
///////////////////////////////////////////////////////////////////////////////

#pragma once

namespace sfpi {

//////////////////////////////////////////////////////////////////////////////
// Functional math library
//////////////////////////////////////////////////////////////////////////////
sfpi_inline vFloat lut(const vFloat v, const vUInt l0, const vUInt l1, const vUInt l2)
{
    return __builtin_rvtt_sfplut(l0.get(), l1.get(), l2.get(), v.get(), SFPLUT_MOD0_SGN_RETAIN);
}

sfpi_inline vFloat lut_sign(const vFloat v, const vUInt l0, const vUInt l1, const vUInt l2)
{
    return __builtin_rvtt_sfplut(l0.get(), l1.get(), l2.get(), v.get(), SFPLUT_MOD0_SGN_UPDATE);
}

sfpi_inline vFloat lut2(const vFloat v, const vUInt l0, const vUInt l1, const vUInt l2)
{
    return __builtin_rvtt_sfplutfp32_3r(l0.get(), l1.get(), l2.get(),
                                        v.get(), SFPLUTFP32_MOD0_FP16_3ENTRY_TABLE | SFPLUTFP32_MOD0_SGN_RETAIN);
}

sfpi_inline vFloat lut2_sign(const vFloat v, const vUInt l0, const vUInt l1, const vUInt l2)
{
    return __builtin_rvtt_sfplutfp32_3r(l0.get(), l1.get(), l2.get(),
                                        v.get(), SFPLUTFP32_MOD0_FP16_3ENTRY_TABLE | SFPLUTFP32_MOD0_SGN_UPDATE);
}

sfpi_inline vFloat lut2(const vFloat v,
                        const vFloat a0, const vFloat a1, const vFloat a2,
                        const vFloat b0, const vFloat b1, const vFloat b2)
{
    return __builtin_rvtt_sfplutfp32_6r(a0.get(), a1.get(), a2.get(),
                                        b0.get(), b1.get(), b2.get(),
                                        v.get(), SFPLUTFP32_MOD0_FP32_3ENTRY_TABLE | SFPLUTFP32_MOD0_SGN_RETAIN);
}

sfpi_inline vFloat lut2_sign(const vFloat v,
                             const vFloat a0, const vFloat a1, const vFloat a2,
                             const vFloat b0, const vFloat b1, const vFloat b2)
{
    return __builtin_rvtt_sfplutfp32_6r(a0.get(), a1.get(), a2.get(),
                                        b0.get(), b1.get(), b2.get(),
                                        v.get(), SFPLUTFP32_MOD0_FP32_3ENTRY_TABLE | SFPLUTFP32_MOD0_SGN_UPDATE);
}

sfpi_inline vFloat lut2(const vFloat v,
                        const vUInt a01, const vUInt a23, const vUInt a45,
                        const vUInt b01, const vUInt b23, const vUInt b45, const int mode = 1)
{
    unsigned int mod = (mode == 1) ? SFPLUTFP32_MOD0_FP16_6ENTRY_TABLE1 : SFPLUTFP32_MOD0_FP16_6ENTRY_TABLE2;
    return __builtin_rvtt_sfplutfp32_6r(a01.get(), a23.get(), a45.get(),
                                        b01.get(), b23.get(), b45.get(),
                                        v.get(), mod | SFPLUTFP32_MOD0_SGN_RETAIN);
}

sfpi_inline vFloat lut2_sign(const vFloat v,
                             const vUInt a01, const vUInt a23, const vUInt a45,
                             const vUInt b01, const vUInt b23, const vUInt b45, const int mode = 1)
{
    unsigned int mod = (mode == 1) ? SFPLUTFP32_MOD0_FP16_6ENTRY_TABLE1 : SFPLUTFP32_MOD0_FP16_6ENTRY_TABLE2;
    return __builtin_rvtt_sfplutfp32_6r(a01.get(), a23.get(), a45.get(),
                                        b01.get(), b23.get(), b45.get(),
                                        v.get(), mod | SFPLUTFP32_MOD0_SGN_UPDATE);
}

sfpi_inline vInt exexp(const vFloat v)
{
    return __builtin_rvtt_sfpexexp(v.get(), SFPEXEXP_MOD1_DEBIAS);
}

sfpi_inline vInt exexp_nodebias(const vFloat v)
{
    return __builtin_rvtt_sfpexexp(v.get(), SFPEXEXP_MOD1_NODEBIAS);
}

sfpi_inline vInt exman8(const vFloat v)
{
    return __builtin_rvtt_sfpexman(v.get(), SFPEXMAN_MOD1_PAD8);
}

sfpi_inline vInt exman9(const vFloat v)
{
    return __builtin_rvtt_sfpexman(v.get(), SFPEXMAN_MOD1_PAD9);
}

sfpi_inline vFloat setexp(const vFloat v, const uint32_t exp)
{
    return __builtin_rvtt_sfpsetexp_i(exp, v.get());
}

sfpi_inline vFloat setexp(const vFloat v, const vIntBase exp)
{
    // Odd: dst is both exponent and result so undergoes a type change
    // If exp is not used later, compiler renames tmp and doesn't issue a mov
    return __builtin_rvtt_sfpsetexp_v(exp.get(), v.get());
}

sfpi_inline vFloat setman(const vFloat v, const uint32_t man)
{
    return __builtin_rvtt_sfpsetman_i(man, v.get(), 0);
}

sfpi_inline vFloat setman(const vFloat v, const vIntBase man)
{
    // Grayskull HW bug, is this useful?  Should there be a "Half" form?
    // Odd: dst is both man and result so undergoes a type change
    // If man is not used later, compiler renames tmp and doesn't issue a mov
    return __builtin_rvtt_sfpsetman_v(man.get(), v.get());
}

sfpi_inline vFloat addexp(const vFloat in, const int32_t exp)
{
    return __builtin_rvtt_sfpdivp2(exp, in.get(), SFPSDIVP2_MOD1_ADD);
}

template <typename vType, typename std::enable_if_t<std::is_base_of<vBase, vType>::value>* = nullptr>
sfpi_inline vType setsgn(const vType v, const int32_t sgn)
{
    return __builtin_rvtt_sfpsetsgn_i(sgn, v.get());
}

template <typename vTypeA, typename vTypeB,
    typename std::enable_if_t<std::is_base_of<vBase, vTypeA>::value>* = nullptr,
    typename std::enable_if_t<std::is_base_of<vBase, vTypeB>::value>* = nullptr>
sfpi_inline vTypeA setsgn(const vTypeA v, const vTypeB sgn)
{
    // Odd: dst is both sgn and result so undergoes a type change
    // If sgn is not used later, compiler renames tmp and doesn't issue a mov
    return __builtin_rvtt_sfpsetsgn_v(sgn.get(), v.get());
}

template <typename vType, typename std::enable_if_t<std::is_base_of<vBase, vType>::value>* = nullptr>
sfpi_inline vType setsgn(const vType v, const vInt sgn)
{
    // Odd: dst is both sgn and result so undergoes a type change
    // If sgn is not used later, compiler renames tmp and doesn't issue a mov
    return __builtin_rvtt_sfpsetsgn_v(sgn.get(), v.get());
}

template <typename vType, typename std::enable_if_t<std::is_base_of<vBase, vType>::value>* = nullptr>
sfpi_inline vInt lz(const vType v)
{
    return vInt(__builtin_rvtt_sfplz(v.get(), SFPLZ_MOD1_CC_NONE));
}

template <typename vType, typename std::enable_if_t<std::is_base_of<vBase, vType>::value>* = nullptr>
sfpi_inline vInt lz_nosgn(const vType v)
{
    return vInt(__builtin_rvtt_sfplz(v.get(), SFPLZ_MOD1_NOSGN_CC_NONE));
}

sfpi_inline vFloat abs(const vFloat v)
{
    return __builtin_rvtt_sfpabs(v.get(), SFPABS_MOD1_FLOAT);
}

sfpi_inline vInt abs(const vInt v)
{
    return __builtin_rvtt_sfpabs(v.get(), SFPABS_MOD1_INT);
}

sfpi_inline vUInt shft(const vUInt v, const vInt amt)
{
    return __builtin_rvtt_sfpshft_v(v.get(), amt.get());
}

template <typename vType, typename std::enable_if_t<std::is_base_of<vBase, vType>::value>* = nullptr>
sfpi_inline vType reinterpret(const vBase v)
{
    return vType(v.get());
}

sfpi_inline vFloat int2float(vInt in, int round_mode = 1)
{
    return __builtin_rvtt_sfpcast(in.get(), round_mode ? SFPCAST_MOD1_RND_STOCH : SFPCAST_MOD1_RND_EVEN);
}

sfpi_inline vUInt float2fp16a(vFloat in, int round_mode = 1)
{
    return __builtin_rvtt_sfpstochrnd_i(round_mode ? SFPSTOCHRND_RND_STOCH : SFPSTOCHRND_RND_EVEN,
                                        0,
                                        in.get(),
                                        SFPSTOCHRND_MOD1_FP32_TO_FP16A);
}

sfpi_inline vUInt float2fp16b(vFloat in, int round_mode = 1)
{
    return __builtin_rvtt_sfpstochrnd_i(round_mode ? SFPSTOCHRND_RND_STOCH : SFPSTOCHRND_RND_EVEN,
                                        0,
                                        in.get(),
                                        SFPSTOCHRND_MOD1_FP32_TO_FP16B);
}

sfpi_inline vUInt float2uint8(vFloat in, int round_mode = 1)
{
    return __builtin_rvtt_sfpstochrnd_i(round_mode ? SFPSTOCHRND_RND_STOCH : SFPSTOCHRND_RND_EVEN,
                                        0,
                                        in.get(),
                                        SFPSTOCHRND_MOD1_FP32_TO_UINT8);
}

sfpi_inline vUInt float2int8(vFloat in, int round_mode = 1)
{
    return __builtin_rvtt_sfpstochrnd_i(round_mode ? SFPSTOCHRND_RND_STOCH : SFPSTOCHRND_RND_EVEN,
                                        0,
                                        in.get(),
                                        SFPSTOCHRND_MOD1_FP32_TO_INT8);
}

sfpi_inline vUInt int322uint8(vInt in, vUInt descale, int round_mode = 1)
{
    return __builtin_rvtt_sfpstochrnd_v(round_mode ? SFPSTOCHRND_RND_STOCH : SFPSTOCHRND_RND_EVEN,
                                        descale.get(),
                                        in.get(),
                                        SFPSTOCHRND_MOD1_INT32_TO_UINT8);
}

sfpi_inline vUInt int322uint8(vInt in, unsigned int descale, int round_mode = 1)
{
    return __builtin_rvtt_sfpstochrnd_i(round_mode ? SFPSTOCHRND_RND_STOCH : SFPSTOCHRND_RND_EVEN,
                                        descale,
                                        in.get(),
                                        SFPSTOCHRND_MOD1_INT32_TO_UINT8 | SFPSTOCHRND_MOD1_IMM8);
}

sfpi_inline vUInt int322int8(vInt in, vUInt descale, int round_mode = 1)
{
    return __builtin_rvtt_sfpstochrnd_v(round_mode ? SFPSTOCHRND_RND_STOCH : SFPSTOCHRND_RND_EVEN,
                                        descale.get(),
                                        in.get(),
                                        SFPSTOCHRND_MOD1_INT32_TO_INT8);
}

sfpi_inline vUInt int322int8(vInt in, unsigned int descale, int round_mode = 1)
{
    return __builtin_rvtt_sfpstochrnd_i(round_mode ? SFPSTOCHRND_RND_STOCH : SFPSTOCHRND_RND_EVEN,
                                        descale,
                                        in.get(),
                                        SFPSTOCHRND_MOD1_INT32_TO_INT8 | SFPSTOCHRND_MOD1_IMM8);
}

sfpi_inline vUInt float2uint16(vFloat in, int round_mode = 1)
{
    return __builtin_rvtt_sfpstochrnd_i(round_mode ? SFPSTOCHRND_RND_STOCH : SFPSTOCHRND_RND_EVEN,
                                        0,
                                        in.get(),
                                        SFPSTOCHRND_MOD1_FP32_TO_UINT16);
}

sfpi_inline vUInt float2int16(vFloat in, int round_mode = 1)
{
    return __builtin_rvtt_sfpstochrnd_i(round_mode ? SFPSTOCHRND_RND_STOCH : SFPSTOCHRND_RND_EVEN,
                                        0,
                                        in.get(),
                                        SFPSTOCHRND_MOD1_FP32_TO_INT16);
}

sfpi_inline void subvec_transp(vBase& l0, vBase& l1, vBase& l2, vBase& l3)
{
    __builtin_rvtt_sfptransp(l0.get(), l1.get(), l2.get(), l3.get());
}

sfpi_inline __rvtt_vec_t subvec_shflror1(const vBase& src)

{
    return __builtin_rvtt_sfpshft2_e(src.get(), SFPSHFT2_MOD1_SUBVEC_SHFLROR1);
}

sfpi_inline __rvtt_vec_t subvec_shflshr1(const vBase& src)

{
    return __builtin_rvtt_sfpshft2_e(src.get(), SFPSHFT2_MOD1_SUBVEC_SHFLSHR1);
}

sfpi_inline void vec_swap(vBase& dst, vBase& src)
{
    __builtin_rvtt_sfpswap(dst.get(), src.get(), SFPSWAP_MOD1_SWAP);
}

sfpi_inline void vec_min_max(vBase& dst, vBase& src)
{
    __builtin_rvtt_sfpswap(dst.get(), src.get(), SFPSWAP_MOD1_VEC_MIN_MAX);
}

} // namespace sfpi
