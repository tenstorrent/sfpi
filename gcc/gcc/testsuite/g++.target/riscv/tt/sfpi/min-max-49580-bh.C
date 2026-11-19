// { dg-options "-mcpu=tt-bh-tensix -O2 -I [SFPI]/include -fno-exceptions -fno-rtti" }
// { dg-final { check-function-bodies "**" "" } }

namespace ckernel{
  extern volatile unsigned long instrn_buffer[];
}
#include <sfpi.h>

using namespace sfpi;

void min1 () {
  vUInt x = l_reg[LRegs::LReg0];

  x = min (x, 0);
  l_reg[LRegs::LReg0] = x;

  x = min (x, 0x80000000);
  l_reg[LRegs::LReg0] = x;
}
/*
**_Z4min1v:
**	# READ L0
**	SFPNOT	L0, L0
**	SFPNOT	L1, L9
**	SFPSWAP	L0, L1, 1
**	SFPNOT	L0, L0
**	# WRITE L0
**	SFPLOADI	L1, 32768, 0
**	SFPSWAP	L1, L0, 1
**	# WRITE L0
**	ret
*/

void max1 () {
  vUInt x = l_reg[LRegs::LReg0];

  x = max (x, 0);
  l_reg[LRegs::LReg0] = x;

  x = max (x, 0x80000000);
  l_reg[LRegs::LReg0] = x;
}
/*
**_Z4max1v:
**	# READ L0
**	SFPNOT	L0, L0
**	SFPNOT	L1, L9
**	SFPSWAP	L1, L0, 1
**	SFPNOT	L0, L0
**	# WRITE L0
**	SFPLOADI	L1, 32768, 0
**	SFPSWAP	L0, L1, 1
**	# WRITE L0
**	ret
*/

void clamp1 () {
  vUInt x = l_reg[LRegs::LReg0];

  x = clamp (x, 0, 10);
  l_reg[LRegs::LReg0] = x;

  x = clamp (x, 0x80000000, 0x80000010);
  l_reg[LRegs::LReg0] = x;
}
/*
**_Z6clamp1v:
**	# READ L0
**	SFPNOT	L0, L0
**	SFPNOT	L1, L9
**	SFPSWAP	L1, L0, 1
**	SFPLOADI	L1, 10, 2
**	SFPNOT	L1, L1
**	SFPSWAP	L0, L1, 1
**	SFPNOT	L0, L0
**	# WRITE L0
**	SFPLOADI	L1, 32768, 0
**	SFPSWAP	L0, L1, 1
**	SFPLOADI	L1, 16, 2
**	SFPLOADI	L1, 32768, 8	# LV:L1
**	SFPSWAP	L1, L0, 1
**	# WRITE L0
**	ret
*/
