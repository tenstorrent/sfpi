// { dg-options "-mcpu=tt-bh-tensix -O2 -I [SFPI]/include -fno-exceptions -fno-rtti" }

namespace ckernel{
    unsigned long *instrn_buffer;
}
#include <sfpi.h>

using namespace sfpi;

void three (unsigned i) {
  vFloat a = l_reg[LRegs::LReg0];
  v_if (a > 0) {
  } v_else {
    if (i)
      goto label;
  } v_endif;
  label:;
}
// { dg-regexp " *inlined from 'void three.unsigned int.' at \[^\n\r\]*/vif-chain-53164-bh-03.C:16:5:" }
// { dg-error "control flow out of 'v_if' sequence" "error" { target *-*-* } 0 }
