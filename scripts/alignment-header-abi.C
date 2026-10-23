// Instantiate the interfaces shared by current upstream and the campaign.
namespace ckernel {
volatile unsigned long *instrn_buffer;
}
#include <sfpi.h>
#include <lltt.h>

using namespace sfpi;

void alignment_header_transpose_abi() {
    vUInt a = dst_reg[0], b = dst_reg[1], c = dst_reg[2], d = dst_reg[3];
    vUInt e = dst_reg[4], f = dst_reg[5], g = dst_reg[6], h = dst_reg[7];
    transp8(a, b, c, d, e, f, g, h);
    dst_reg[0] = a; dst_reg[1] = b; dst_reg[2] = c; dst_reg[3] = d;
    dst_reg[4] = e; dst_reg[5] = f; dst_reg[6] = g; dst_reg[7] = h;
}

void alignment_header_interfaces_abi() {
    vFloat a = dst_reg[0], b = dst_reg[1];
    sort2<SortOrder::Ascending>(a, b);
    sort2_rows<RowPattern::Min01Max23>(a, b);
    vInt rounded = round(a);
    dst_reg[0] = rounded;
    v_if (nearby (lane_col() < 4)) {
        dst_reg[1] = a;
    } v_else {
        dst_reg[1] = b;
    } v_endif;
#if __riscv_xtttensixbh || __riscv_xtttensixqsr
    dst_reg[2] = mask (a <= b);
#endif
#if __riscv_xtttensixqsr
    SrcSView<1> src;
    dst_reg[3] = vFloat(src[0]);
#else
    lltt::setrwc<0, 0, 0, 0, 0, 0>();
    dst_face_advance();
#endif
}

#ifdef LLTT_HAS_LREG_EFFECT
void alignment_header_raw_effect_abi() {
    // No raw word is needed to validate wrapper/marker ABI and erasure.
    lltt::lreg_effect(0x1u, 0x2u);
}
#endif
