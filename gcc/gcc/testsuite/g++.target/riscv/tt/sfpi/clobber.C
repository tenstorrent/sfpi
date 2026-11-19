// { dg-options "-mcpu=tt-bh-tensix -O2 -I [SFPI]/include -fno-exceptions -fno-rtti" }
// { dg-final { check-function-bodies "**" "" } }

namespace ckernel{
    unsigned long *instrn_buffer;
}
#include <sfpi.h>

using namespace sfpi;

void copy () {
  auto x = vFloat (5.0f);
  l_reg[LRegs::LReg0] = l_reg[LRegs::LReg1];
  dst_reg[0] = x;
}
/*
**_Z4copyv:
**	SFPLOADI	L2, 16544, 0
**	# READ L1
**	SFPMOV	L0, L1, 2
**	# WRITE L0
**	SFPSTORE	L2, 0, 0, 7
**	ret
*/

void clobber () {
  auto x = vFloat (5.0f);
  l_reg[LRegs::LReg0].in_use ();
  dst_reg[0] = x;
}
/*
**_Z7clobberv:
**	SFPLOADI	L1, 16544, 0
**	# READ L0
**	# WRITE L0
**	SFPSTORE	L1, 0, 0, 7
**	ret
*/
