// { dg-options "-mcpu=tt-bh-tensix -O2 -I [SFPI]/include -fno-exceptions -fno-rtti" }
// { dg-final { check-function-bodies "**" "" } }

namespace ckernel{
    unsigned long *instrn_buffer;
}
#include <sfpi.h>

using namespace sfpi;

void one () {
  vFloat a = l_reg[LRegs::LReg0];

  l_reg[LRegs::LReg0] = a * 1.5f + 2.0f;
}
/*
**_Z3onev:
**	# READ L0
**	SFPLOADI	L1, 16320, 0
**	SFPLOADI	L2, 16384, 0
**	SFPMAD	L0, L0, L1, L2, 0
**	# WRITE L0
**	ret
*/

void two () {
  lreg_pressure _;
  vFloat a = l_reg[LRegs::LReg0];

  l_reg[LRegs::LReg0] = a * 1.5f + 2.0f;
}
/*
**_Z3twov:
**	# READ L0
**	SFPMULI	L0, 16320, 0
**	SFPADDI	L0, 16384, 0
**	# WRITE L0
**	ret
*/
