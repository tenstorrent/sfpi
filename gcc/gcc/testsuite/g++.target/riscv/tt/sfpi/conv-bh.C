// { dg-options "-mcpu=tt-bh-tensix -O2 -I [SFPI]/include -fno-exceptions -fno-rtti" }
// { dg-final { check-function-bodies "**" "" } }

namespace ckernel{
    unsigned long *instrn_buffer;
}
#include <sfpi.h>

using namespace sfpi;

void one () {
  vFloat a = dst_reg[0];

  dst_reg[1] = convert<vFloat16a> (a);
  dst_reg[2] = convert<vFloat16b> (a);
  dst_reg[3] = convert<vSMag16> (a);
  dst_reg[4] = convert<vUInt16> (a);
}
/*
**_Z3onev:
**	SFPLOAD	L0, 0, 0, 7
**	SFPSTOCHRND	L1, L9, L0, 0, 0, 1
**	SFPSTORE	L1, 2, 1, 7
**	SFPSTOCHRND	L1, L9, L0, 0, 1, 1
**	SFPSTORE	L1, 4, 2, 7
**	SFPSTOCHRND	L1, L9, L0, 0, 7, 1
**	SFPSTORE	L1, 6, 8, 7
**	SFPSTOCHRND	L0, L9, L0, 0, 6, 1
**	SFPSTORE	L0, 8, 6, 7
**	ret
*/

void two () {
  vInt a = dst_reg[0];

  dst_reg[3] = convert<vSMag> (a);
}
/*
**_Z3twov:
**	SFPLOAD	L0, 0, 4, 7
**	SFPCAST	L0, L0, 3
**	SFPSTORE	L0, 6, 4, 7
**	ret
*/

void three () {
  vSMag a = dst_reg[0];

  dst_reg[3] = as<vUInt> (convert<vInt> (a));
  dst_reg[4] = convert<vFloat> (a);
}
/*
**_Z5threev:
**	SFPLOAD	L0, 0, 4, 7
**	SFPSETCC	L0, 0, 0
**	SFPMOV	L1, L0, 2
**	SFPSETSGN	L1, L0, 0, 1	# LV:L1
**	SFPIADD	L1, L9, 0, 6	# LV:L1
**	SFPENCC	3, 10
**	SFPSTORE	L1, 6, 4, 7
**	SFPCAST	L0, L0, 1
**	SFPSTORE	L0, 8, 0, 7
**	ret
*/

void available () {
  l_reg[LRegs::LReg0] = convert<vFloat> (vFloat (0.0f));
  l_reg[LRegs::LReg0] = convert<vFloat> (vInt (0));
  //l_reg[LRegs::LReg0] = convert<vFloat> (vUInt (0));
  l_reg[LRegs::LReg0] = convert<vFloat> (vSMag (0));
  l_reg[LRegs::LReg0] = convert<vFloat> (vMag (0));
  //l_reg[LRegs::LReg0] = convert<vInt> (vFloat (0.0f));
  l_reg[LRegs::LReg0] = convert<vInt> (vInt (0));
  //l_reg[LRegs::LReg0] = convert<vInt> (vUInt (0));
  l_reg[LRegs::LReg0] = convert<vInt> (vSMag (0));
  l_reg[LRegs::LReg0] = convert<vInt> (vMag (0));
  //l_reg[LRegs::LReg0] = convert<vUInt> (vFloat (0.0f));
  //l_reg[LRegs::LReg0] = convert<vUInt> (vInt (0));
  //l_reg[LRegs::LReg0] = convert<vUInt> (vUInt (0));
  //l_reg[LRegs::LReg0] = convert<vUInt> (vSMag (0));
  //l_reg[LRegs::LReg0] = convert<vUInt> (vMag (0));

  //l_reg[LRegs::LReg0] = convert<vSMag> (vFloat (0.0f));
  l_reg[LRegs::LReg0] = convert<vSMag> (vInt (0));
  //l_reg[LRegs::LReg0] = convert<vSMag> (vUInt (0));
  l_reg[LRegs::LReg0] = convert<vSMag> (vSMag (0));
  l_reg[LRegs::LReg0] = convert<vSMag> (vMag (0));

  //l_reg[LRegs::LReg0] = convert<vMag> (vFloat (0.0f));
  //l_reg[LRegs::LReg0] = convert<vMag> (vInt (0));
  //l_reg[LRegs::LReg0] = convert<vMag> (vUInt (0));
  //l_reg[LRegs::LReg0] = convert<vMag> (vSMag (0));
  l_reg[LRegs::LReg0] = convert<vMag> (vMag (0));

  l_reg[LRegs::LReg0] = convert<vFloat16a> (vFloat (0.0f));
  l_reg[LRegs::LReg0] = convert<vFloat16a> (vInt (0));
  //l_reg[LRegs::LReg0] = convert<vFloat16a> (vUInt (0));
  l_reg[LRegs::LReg0] = convert<vFloat16a> (vSMag (0));
  l_reg[LRegs::LReg0] = convert<vFloat16a> (vMag (0));

  l_reg[LRegs::LReg0] = convert<vFloat16b> (vFloat (0.0f));
  l_reg[LRegs::LReg0] = convert<vFloat16b> (vInt (0));
  //l_reg[LRegs::LReg0] = convert<vFloat16b> (vUInt (0));
  l_reg[LRegs::LReg0] = convert<vFloat16b> (vSMag (0));
  l_reg[LRegs::LReg0] = convert<vFloat16b> (vMag (0));

  l_reg[LRegs::LReg0] = convert<vUInt16> (vFloat (0.0f));
  //l_reg[LRegs::LReg0] = convert<vUInt16> (vInt (0));
  //l_reg[LRegs::LReg0] = convert<vUInt16> (vUInt (0));
  //l_reg[LRegs::LReg0] = convert<vUInt16> (vSMag (0));
  //l_reg[LRegs::LReg0] = convert<vUInt16> (vMag (0));

  l_reg[LRegs::LReg0] = convert<vUInt8> (vFloat (0.0f));
  //l_reg[LRegs::LReg0] = convert<vUInt8> (vInt (0));
  //l_reg[LRegs::LReg0] = convert<vUInt8> (vUInt (0));
  //l_reg[LRegs::LReg0] = convert<vUInt8> (vSMag (0));
  //l_reg[LRegs::LReg0] = convert<vUInt8> (vMag (0));

  l_reg[LRegs::LReg0] = convert<vSMag16> (vFloat (0.0f));
  //l_reg[LRegs::LReg0] = convert<vSMag16> (vInt (0));
  //l_reg[LRegs::LReg0] = convert<vSMag16> (vUInt (0));
  //l_reg[LRegs::LReg0] = convert<vSMag16> (vSMag (0));
  //l_reg[LRegs::LReg0] = convert<vSMag16> (vMag (0));

  l_reg[LRegs::LReg0] = convert<vSMag8> (vFloat (0.0f));
  //l_reg[LRegs::LReg0] = convert<vSMag8> (vInt (0));
  //l_reg[LRegs::LReg0] = convert<vSMag8> (vUInt (0));
  //l_reg[LRegs::LReg0] = convert<vSMag8> (vSMag (0));
  //l_reg[LRegs::LReg0] = convert<vSMag8> (vMag (0));
}
