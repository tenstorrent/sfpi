// { dg-options "-mcpu=tt-bh-tensix -O2 -I [SFPI]/include -fno-exceptions -fno-rtti" }
// { dg-final { check-function-bodies "**" "" } }

namespace ckernel{
  extern volatile unsigned long instrn_buffer[];
}
#include <sfpi.h>

using namespace sfpi;

void poly1 () {
  vFloat x = l_reg[LRegs::LReg0];

  x = polynomial (x, 1.0f, 2.0f, vFloat(l_reg[LRegs::LReg1]));
  l_reg[LRegs::LReg0] = x;
}
/*
**_Z5poly1v:
**	# READ L0
**	# READ L1
**	SFPLOADI	L2, 16384, 0
**	SFPMAD	L1, L0, L1, L2, 0
**	SFPMAD	L0, L0, L1, L10, 0
**	# WRITE L0
**	ret
*/

void poly2 () {
  vFloat x = l_reg[LRegs::LReg0];

  x = polynomial (x, 1.0f, 0.0f, vFloat(l_reg[LRegs::LReg1]));
  l_reg[LRegs::LReg0] = x;
}
/*
**_Z5poly2v:
**	# READ L0
**	# READ L1
**	SFPMAD	L1, L0, L1, L9, 0
**	SFPMAD	L0, L0, L1, L10, 0
**	# WRITE L0
**	ret
*/
