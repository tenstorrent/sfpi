// { dg-options "-mcpu=tt-bh-tensix -O2 -I [SFPI]/include -fno-exceptions -fno-rtti" }

namespace ckernel{
    unsigned long *instrn_buffer;
}
#include <sfpi.h>

using namespace sfpi;

void six () {
  vFloat a = l_reg[LRegs::LReg0];
  v_if (a < 0.0f) {
  } v_else {
  } v_elseif (a > 0.0f) {
  } v_endif;
}
// { dg-regexp " *inlined from 'void six..' at \[^\n\r\]*vif-chain-53164-bh-06.C:14:5:" }
// { dg-error "incorrect sequencing of 'v_if'" "error" { target *-*-* } 0 }
