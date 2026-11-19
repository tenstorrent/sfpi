// { dg-options "-mcpu=tt-wh-tensix -O2 -I [SFPI]/include -fno-exceptions -fno-rtti" }

namespace ckernel {
extern volatile unsigned long instrn_buffer[];
}
#include "sfpi.h"

using namespace sfpi;

void foo() {
  vSMag a = dst_reg[0];
  vSMag b = dst_reg[1];
  v_if ((vBool::Cmp<vBool::LT, vBool::SMag> (a, b))) {
    dst_reg[0] = 1.0f;
  }
  v_endif;
}
// { dg-regexp " *inlined from 'void foo..' at \[^\n\r\]*/cmp-53152-wh.C:13:3:" }
// { dg-error "is not supported" "error" { target *-*-* } 0 }
