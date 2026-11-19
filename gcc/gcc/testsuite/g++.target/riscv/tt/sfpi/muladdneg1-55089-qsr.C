// { dg-options "-mcpu=tt-qsr32-tensix -O2 -I [SFPI]/include -fno-exceptions -fno-rtti -Wall -Wextra -Wunused-parameter" }
// { dg-final { check-function-bodies "**" "" } }

namespace ckernel{
    extern unsigned long instrn_buffer[];
}
#include <sfpi.h>

using namespace sfpi;

void one () {
  vFloat a = l_reg[LRegs::LReg0];

  a = a * vFloat (-1.0f);
  l_reg[LRegs::LReg0] = a;

  a = a + vFloat (-1.0f);
  l_reg[LRegs::LReg0] = a;

  a = a * a + vFloat (-1.0f);
  l_reg[LRegs::LReg0] = a;

  a = a * vFloat (-1.0f) + a;
  l_reg[LRegs::LReg0] = a;
}
/*
**_Z3onev:
**	# READ L0
**	SFPMUL	L0, L0, L10, 1
**	# WRITE L0
**	SFPADD	L0, L0, L10, 2
**	# WRITE L0
**	SFPMAD	L0, L0, L0, L10, 2
**	# WRITE L0
**	SFPMAD	L0, L0, L10, L0, 1
**	# WRITE L0
**	ret
*/

void two () {
  lreg_pressure _;
  vFloat a = l_reg[LRegs::LReg0];

  a = a * vFloat (-1.0f);
  l_reg[LRegs::LReg0] = a;

  a = a + vFloat (-1.0f);
  l_reg[LRegs::LReg0] = a;

  a = a * a + vFloat (-1.0f);
  l_reg[LRegs::LReg0] = a;

  a = a * vFloat (-1.0f) + a;
  l_reg[LRegs::LReg0] = a;
}
/*
**_Z3twov:
**	# READ L0
**	SFPMUL	L0, L0, L10, 1
**	# WRITE L0
**	SFPADD	L0, L0, L10, 2
**	# WRITE L0
**	SFPMAD	L0, L0, L0, L10, 2
**	# WRITE L0
**	SFPMAD	L0, L0, L10, L0, 1
**	# WRITE L0
**	ret
*/
