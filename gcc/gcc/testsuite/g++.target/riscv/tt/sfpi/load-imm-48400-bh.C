// { dg-options "-mcpu=tt-bh-tensix -O2 -I [SFPI]/include -fno-exceptions -fno-rtti" }
// { dg-final { check-function-bodies "**" "" } }

namespace ckernel{
  extern unsigned long *instrn_buffer;
}
#include <sfpi.h>

using namespace sfpi;

void loadp8373 ()
{
  vFloat a = 0.8373f;
  l_reg[LRegs::LReg0] = a;
}
/*
**_Z9loadp8373v:
**	SFPMOV	L0, L8, 2
**	# WRITE L0
**	ret
*/

void near1 ()
{
  vFloat a = 1.0f + 0x0.000ffep0f;
  vFloat b = 1.0f - 0x0.0007ffp0f;
  vFloat c = 1.0f + 0x0.001000p0f;
  vFloat d = 1.0f - 0x0.000800p0f;
  l_reg[LRegs::LReg0] = a;
  l_reg[LRegs::LReg1] = b;
  l_reg[LRegs::LReg2] = c;
  l_reg[LRegs::LReg3] = d;
}
/*
**_Z5near1v:
**	SFPIADD	L0, L10, 2047, 5
**	SFPIADD	L1, L10, -2047, 5
**	SFPLOADI	L2, 2048, 2
**	SFPLOADI	L2, 16256, 8	# LV:L2
**	SFPIADD	L3, L10, -2048, 5
**	# WRITE L0
**	# WRITE L1
**	# WRITE L2
**	# WRITE L3
**	ret
*/

void nearn1 ()
{
  vFloat a = -1.0f - 0x0.000ffep0f;
  vFloat b = -1.0f + 0x0.0007ffp0f;
  vFloat c = -1.0f - 0x0.001000p0f;
  vFloat d = -1.0f + 0x0.000800p0f;
  l_reg[LRegs::LReg0] = a;
  l_reg[LRegs::LReg1] = b;
  l_reg[LRegs::LReg2] = c;
  l_reg[LRegs::LReg3] = d;
}
/*
**_Z6nearn1v:
**	SFPIADD	L0, L11, 2047, 5
**	SFPIADD	L1, L11, -2047, 5
**	SFPLOADI	L2, 2048, 2
**	SFPLOADI	L2, 49024, 8	# LV:L2
**	SFPIADD	L3, L11, -2048, 5
**	# WRITE L0
**	# WRITE L1
**	# WRITE L2
**	# WRITE L3
**	ret
*/

void nearp8373 ()
{
  vFloat a = 0.8373f + 0x0.000ffep-1f;
  vFloat b = 0.8373f - 0x0.0007ffp0f;
  vFloat c = 0.8373f + 0x0.001000p-1f;
  vFloat d = 0.8373f - 0x0.000800p0f;
  l_reg[LRegs::LReg0] = a;
  l_reg[LRegs::LReg1] = b;
  l_reg[LRegs::LReg2] = c;
  l_reg[LRegs::LReg3] = d;
}
/*
**_Z9nearp8373v:
**	SFPIADD	L0, L8, 2047, 5
**	SFPIADD	L1, L8, -2047, 5
**	SFPLOADI	L2, 24907, 2
**	SFPLOADI	L2, 16214, 8	# LV:L2
**	SFPIADD	L3, L8, -2048, 5
**	# WRITE L0
**	# WRITE L1
**	# WRITE L2
**	# WRITE L3
**	ret
*/

// I learnt this trick from LLK.  Make sure I get it right!
void llk ()
{
  vFloat a = 0.999963462f;
  vFloat b = as<vFloat>(as<vInt>(vFloat(1.0f)) - 613);
  l_reg[LRegs::LReg0] = a;
  l_reg[LRegs::LReg1] = b;
}
/*
**_Z3llkv:
**	SFPIADD	L0, L10, -613, 5
**	SFPIADD	L1, L10, -613, 5
**	# WRITE L0
**	# WRITE L1
**	ret
*/

void shft ()
{
  vInt a = 0x003f8000;
  vInt b = 0xffbf8000;
  vInt c = 0x56594b00;
  l_reg[LRegs::LReg0] = a;
  l_reg[LRegs::LReg1] = b;
  l_reg[LRegs::LReg2] = c;
}
/*
**_Z4shftv:
**	SFPSHFT	L0, L10, -8, 5
**	SFPSHFT	L1, L11, -8, 7
**	SFPSHFT	L2, L8, 8, 5
**	# WRITE L0
**	# WRITE L1
**	# WRITE L2
**	ret
*/
