// { dg-options "-mcpu=tt-bh-tensix -O2 -I [SFPI]/include -fno-exceptions -fno-rtti" }
// { dg-final { check-function-bodies "**" "" } }

namespace ckernel{
    unsigned long *instrn_buffer;
}
#include <sfpi.h>

using namespace sfpi;

void lt () {
  vFloat a = l_reg[LRegs::LReg0];
  vFloat b = l_reg[LRegs::LReg1];

  l_reg[LRegs::LReg2] = mask (a < b);
}
/*
**_Z2ltv:
**	# READ L0
**	# READ L1
**	SFPGT	L1, L0, 0, 8
**	SFPMOV	L2, L1, 2
**	# WRITE L2
**	ret
*/

void ge () {
  vFloat a = l_reg[LRegs::LReg0];
  vFloat b = l_reg[LRegs::LReg1];

  l_reg[LRegs::LReg2] = mask (a >= b);
}
/*
**_Z2gev:
**	# READ L0
**	# READ L1
**	SFPLE	L1, L0, 0, 8
**	SFPMOV	L2, L1, 2
**	# WRITE L2
**	ret
*/

void eq () {
  vSMag a = l_reg[LRegs::LReg0];
  vSMag b = l_reg[LRegs::LReg1];

  l_reg[LRegs::LReg2] = mask (a == b);
}
/*
**_Z2eqv:
**	# READ L0
**	# READ L1
**	SFPMOV	L2, L0, 2
**	SFPLE	L2, L1, 0, 8
**	SFPLE	L1, L0, 0, 8
**	SFPAND	L2, L2, L1, 1
**	# WRITE L2
**	ret
*/

void ne () {
  vSMag a = l_reg[LRegs::LReg0];
  vSMag b = l_reg[LRegs::LReg1];

  l_reg[LRegs::LReg2] = mask (a != b);
}
/*
**_Z2nev:
**	# READ L0
**	# READ L1
**	SFPMOV	L2, L0, 2
**	SFPGT	L2, L1, 0, 8
**	SFPGT	L1, L0, 0, 8
**	SFPOR	L2, L2, L1, 1
**	# WRITE L2
**	ret
*/

void gt () {
  vFloat a = l_reg[LRegs::LReg0];
  vFloat b = l_reg[LRegs::LReg1];

  l_reg[LRegs::LReg2] = mask (a > b);
}
/*
**_Z2gtv:
**	# READ L0
**	# READ L1
**	SFPGT	L0, L1, 0, 8
**	SFPMOV	L2, L0, 2
**	# WRITE L2
**	ret
*/

void le () {
  vFloat a = l_reg[LRegs::LReg0];
  vFloat b = l_reg[LRegs::LReg1];

  l_reg[LRegs::LReg2] = mask (a <= b);
}
/*
**_Z2lev:
**	# READ L0
**	# READ L1
**	SFPLE	L0, L1, 0, 8
**	SFPMOV	L2, L0, 2
**	# WRITE L2
**	ret
*/
