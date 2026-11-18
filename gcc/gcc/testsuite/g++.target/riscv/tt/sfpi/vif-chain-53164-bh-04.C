// { dg-options "-mcpu=tt-bh-tensix -O2 -I [SFPI]/include -fno-exceptions -fno-rtti" }

namespace ckernel{
    unsigned long *instrn_buffer;
}
#include <sfpi.h>

using namespace sfpi;

void four (unsigned i) {
  vFloat a = l_reg[LRegs::LReg0];
  label:;
  v_if (a > 0) {
  } v_else {
    if (i)
      goto label;
  } v_endif;
}
// { dg-regexp " *inlined from 'void four.unsigned int.' at \[^\n\r\]*/vif-chain-53164-bh-04.C:14:5:" }
// { dg-error "unexpectedly terminated 'v_if' sequence" "error" { target *-*-* } 0 }
