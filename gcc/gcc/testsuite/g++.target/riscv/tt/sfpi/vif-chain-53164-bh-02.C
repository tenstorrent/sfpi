// { dg-options "-mcpu=tt-bh-tensix -O2 -I [SFPI]/include -fno-exceptions -fno-rtti" }

namespace ckernel{
    unsigned long *instrn_buffer;
}
#include <sfpi.h>

using namespace sfpi;

void two (unsigned i) {
  vFloat a = l_reg[LRegs::LReg0];
  v_if (a > 0) {
    if (i)
      return;
  } v_endif;
}
// { dg-regexp " *inlined from 'void two.unsigned int.' at \[^\n\r\]*/vif-chain-53164-bh-02.C:15:5:" }
// { dg-error "control flow out of 'v_if' sequence" "error" { target *-*-* } 0 }
