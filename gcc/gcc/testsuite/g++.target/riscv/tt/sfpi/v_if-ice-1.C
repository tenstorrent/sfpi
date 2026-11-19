// { dg-options "-O2 -I [SFPI]/include -fno-exceptions -fno-rtti" }

#if __riscv_tt_grayskull || __riscv_tt_wormhole || __riscv_tt_blackhole

namespace ckernel{
    unsigned long *instrn_buffer;
}
#include <sfpi.h>

using namespace sfpi;

// checking ICE with bad SSA VDEF
void fnuvv () {
    vUInt a = l_reg[LRegs::LReg0];
    v_if(a >= 0) {
    } v_endif;
}

#endif
