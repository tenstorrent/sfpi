// { dg-options "-mcpu=tt-bh-tensix -O2 -I [SFPI]/include -fno-exceptions -fno-rtti" }

namespace ckernel{
    unsigned long *instrn_buffer;
}
#include <sfpi.h>

using namespace sfpi;

void one () {
  vFloat a = l_reg[LRegs::LReg0];
  v_if (a > 0) {
    return;
  } v_endif;
}
// { dg-regexp " *inlined from 'void one..' at \[^\n\r\]*/vif-chain-53164-bh-01.C:12:3:" }
// { dg-error "unexpectedly terminated 'v_if'" "error" { target *-*-* } 0 }
