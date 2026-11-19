// { dg-options "-mcpu=tt-bh-tensix -O2 -I [SFPI]/include -fno-exceptions -fno-rtti" }
// { dg-final { check-function-bodies "**" "" } }

namespace ckernel{
  extern volatile unsigned long instrn_buffer[];
}
#include <sfpi.h>

using namespace sfpi;

void ldexp0 () {
  vFloat x = l_reg[LRegs::LReg0];

  x = ldexp (x, 22, LdexpMode::Fast);
  l_reg[LRegs::LReg0] = x;
}
/*
**_Z6ldexp0v:
**	# READ L0
**	SFPDIVP2	L0, L0, 22, 1
**	# WRITE L0
**	ret
*/

void ldexp1 () {
  vFloat x = l_reg[LRegs::LReg0];

  x = ldexp (x, 22);
  l_reg[LRegs::LReg0] = x;
}
/*
**_Z6ldexp1v:
**	# READ L0
**	SFPLOADI	L1, 127, 2
**	SFPIADD	L1, L1, 22, 5
**	SFPSETEXP	L1, L9, 0, 0
**	SFPMUL	L0, L0, L1, 0
**	# WRITE L0
**	ret
*/

void ldexp2 () {
  vFloat x = l_reg[LRegs::LReg0];
  vInt scale = l_reg[LRegs::LReg1];

  x = ldexp (x, scale);
  l_reg[LRegs::LReg0] = x;
}
/*
**_Z6ldexp2v:
**	# READ L0
**	# READ L1
**	SFPIADD	L1, L1, 127, 5
**	SFPSETEXP	L1, L9, 0, 0
**	SFPMUL	L0, L0, L1, 0
**	# WRITE L0
**	ret
*/

void ldexp3 () {
  vFloat x = l_reg[LRegs::LReg0];
  vInt scale = l_reg[LRegs::LReg1];

  x = ldexp (x, scale, LdexpMode::Fast);
  l_reg[LRegs::LReg2] = x;
}
/*
**_Z6ldexp3v:
**	# READ L0
**	# READ L1
**	SFPEXEXP	L2, L0, 1
**	SFPIADD	L2, L1, 0, 4
**	SFPSETEXP	L2, L0, 0, 0
**	# WRITE L2
**	ret
*/
