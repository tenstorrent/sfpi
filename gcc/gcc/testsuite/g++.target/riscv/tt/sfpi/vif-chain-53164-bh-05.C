// { dg-options "-mcpu=tt-bh-tensix -O2 -I [SFPI]/include -fno-exceptions -fno-rtti" }

namespace ckernel{
    unsigned long *instrn_buffer;
}
#include <sfpi.h>

using namespace sfpi;

void five () {
  vFloat a = l_reg[LRegs::LReg0];
  v_block {
    v_and (a < 0.0f);
    v_else {
    }
  } v_endblock;
}
// { dg-regexp " *inlined from 'void five..' at \[^\n\r\]*/vif-chain-53164-bh-05.C:14:5:" }
// { dg-error "incorrect sequencing of 'v_if'" "error" { target *-*-* } 0 }
