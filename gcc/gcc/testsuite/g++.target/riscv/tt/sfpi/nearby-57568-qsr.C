// { dg-options "-mcpu=tt-qsr32-tensix -O2 -I [SFPI]/include -fno-exceptions -fno-rtti" }
// { dg-final { check-function-bodies "**" "" } }

namespace ckernel{
    unsigned long *instrn_buffer;
}
#include <sfpi.h>

using namespace sfpi;

void Foo () {
  vInt a = l_reg[LRegs::LReg0];
  vInt b = l_reg[LRegs::LReg1];

  v_if (nearby (a < b)) {
    a = 0;
  } v_endif;
  l_reg[LRegs::LReg0] = a;
}
/*
**_Z3Foov:
**	# READ L0
**	# READ L1
**	SFPIADD	L1, L0, 0, 2
**	SFPMOV	L0, L9, 0	# LV:L0
**	SFPENCC	3, 10
**	# WRITE L0
**	ret
*/
