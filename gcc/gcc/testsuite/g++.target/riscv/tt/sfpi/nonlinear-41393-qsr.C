// { dg-options "-mcpu=tt-qsr32-tensix -O2 -I [SFPI]/include -fno-exceptions -fno-rtti" }
// { dg-final { check-function-bodies "**" "" } }

namespace ckernel{
    unsigned long *instrn_buffer;
}
#include <sfpi.h>

using namespace sfpi;

void foo () {
  // Basic test
  vFloat a = l_reg[LRegs::LReg0];
  a = rectified_linear_unit (a);
  a = approx_sqrt (a);
  a = approx_tanh (a);
  l_reg[LRegs::LReg1] = a;
}
/*
**_Z3foov:
**	# READ L0
**	SFPNONLINEAR	L1, L0, 2
**	SFPNONLINEAR	L1, L1, 3
**	SFPNONLINEAR	L1, L1, 5
**	# WRITE L1
**	ret
*/

void bar () {
  // Live value test
  vFloat a = l_reg[LRegs::LReg0];
  vFloat b = l_reg[LRegs::LReg1];
  
  vFloat r;
  v_if (a < b) {
    a = approx_sqrt (a);
  } v_else {
    a = approx_tanh (a);
  } v_endif;

  l_reg[LRegs::LReg3] = a;
}
/*
**_Z3barv:
**	# READ L0
**	# READ L1
**	SFPGT	L1, L0, 1, 1
**	SFPNONLINEAR	L0, L0, 3	# LV:L0
**	SFPCOMPC
**	SFPNONLINEAR	L0, L0, 5	# LV:L0
**	SFPENCC	3, 10
**	SFPMOV	L3, L0, 2
**	# WRITE L3
**	ret
*/
