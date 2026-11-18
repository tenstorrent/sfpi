// { dg-options "-mcpu=tt-qsr32-tensix -O2 -I [SFPI]/include -fno-exceptions -fno-rtti" }
// { dg-final { check-function-bodies "**" "" } }

namespace ckernel{
    unsigned long *instrn_buffer;
}
#include <sfpi.h>

using namespace sfpi;

void f5() {
  vUInt a = l_reg[LRegs::LReg0];
  vUInt b = l_reg[LRegs::LReg1];

  vUInt r = a << b;
  l_reg[LRegs::LReg3] = r;
}
/*
**_Z2f5v:
**	# READ L0
**	# READ L1
**	SFPSHFT	L0, L1, 0, 0
**	SFPMOV	L3, L0, 2
**	# WRITE L3
**	ret
*/

void f5r() {
  vUInt a = l_reg[LRegs::LReg0];
  vUInt b = l_reg[LRegs::LReg1];

  vUInt r = a >> b;
  l_reg[LRegs::LReg3] = r;
}
/*
**_Z3f5rv:
**	# READ L0
**	# READ L1
**	SFPIADD	L1, L9, 0, 6
**	SFPSHFT	L0, L1, 0, 0
**	SFPMOV	L3, L0, 2
**	# WRITE L3
**	ret
*/

void f6() {
  vInt a = l_reg[LRegs::LReg0];
  vUInt b = l_reg[LRegs::LReg1];

  vInt r = a << b;
  l_reg[LRegs::LReg3] = r;
}
/*
**_Z2f6v:
**	# READ L0
**	# READ L1
**	SFPSHFT	L0, L1, 0, 2
**	SFPMOV	L3, L0, 2
**	# WRITE L3
**	ret
*/

void f6r() {
  vInt a = l_reg[LRegs::LReg0];
  vUInt b = l_reg[LRegs::LReg1];

  vInt r = a >> b;
  l_reg[LRegs::LReg3] = r;
}
/*
**_Z3f6rv:
**	# READ L0
**	# READ L1
**	SFPIADD	L1, L9, 0, 6
**	SFPSHFT	L0, L1, 0, 2
**	SFPMOV	L3, L0, 2
**	# WRITE L3
**	ret
*/
