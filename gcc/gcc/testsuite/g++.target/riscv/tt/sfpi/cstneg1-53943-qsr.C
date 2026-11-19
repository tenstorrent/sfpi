// { dg-options "-mcpu=tt-qsr32-tensix -O2 -I [SFPI]/include -fno-exceptions -fno-rtti" }
// { dg-final { check-function-bodies "**" "" } }

// Verify we notice a - b is a + -1.0 * b

namespace ckernel{
    unsigned long *instrn_buffer;
}
#include <sfpi.h>

using namespace sfpi;

void pos1 () {
  l_reg[LRegs::LReg0] = vFloat (1.0f);
  l_reg[LRegs::LReg1] = as<vFloat>(vUInt (0x3f800000 + 0x42));
  l_reg[LRegs::LReg2] = as<vFloat>(vUInt (0x3f800000 + 0x5542));
}
/*
**_Z4pos1v:
**	SFPMOV	L0, L10, 2
**	# WRITE L0
**	SFPIADD	L1, L0, 66, 5
**	# WRITE L1
**	SFPLOADI	L2, 21826, 2
**	SFPLOADI	L2, 16256, 8	# LV:L2
**	# WRITE L2
**	ret
*/

void neg1 () {
  l_reg[LRegs::LReg0] = vFloat (-1.0f);
  l_reg[LRegs::LReg1] = as<vFloat>(vUInt (0xbf800000 + 0x42));
  l_reg[LRegs::LReg2] = as<vFloat>(vUInt (0xbf800000 + 0x5542));
}
/*
**_Z4neg1v:
**	SFPLOADI	L0, 49024, 0
**	# WRITE L0
**	SFPLOADI	L1, 66, 2
**	SFPLOADI	L1, 49024, 8	# LV:L1
**	# WRITE L1
**	SFPLOADI	L2, 21826, 2
**	SFPLOADI	L2, 49024, 8	# LV:L2
**	# WRITE L2
**	ret
*/
