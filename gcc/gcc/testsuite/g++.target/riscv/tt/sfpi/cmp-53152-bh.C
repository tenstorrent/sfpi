// { dg-options "-mcpu=tt-bh-tensix -O2 -I [SFPI]/include -fno-exceptions -fno-rtti" }
// { dg-final { check-function-bodies "**" "" } }

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
/*
**_Z3foov:
**	SFPLOAD	L1, 0, 4, 7
**	SFPLOAD	L0, 2, 4, 7
**	SFPGT	L0, L1, 0, 1
**	SFPSTORE	L10, 0, 0, 7
**	SFPENCC	3, 10
**	ret
*/

void bar() {
  vSMag a = dst_reg[0];
  vSMag b = dst_reg[1];
  v_if (a < b) {
    dst_reg[0] = 1.0f;
  }
  v_endif;
}
/*
**_Z3barv:
**	SFPLOAD	L1, 0, 4, 7
**	SFPLOAD	L0, 2, 4, 7
**	SFPGT	L0, L1, 0, 1
**	SFPSTORE	L10, 0, 0, 7
**	SFPENCC	3, 10
**	ret
*/
