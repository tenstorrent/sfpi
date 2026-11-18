// { dg-options "-mcpu=tt-qsr32-tensix -O2 -I [SFPI]/include -fno-exceptions -fno-rtti" }
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
**	SFPLOAD	L0, 0, 0, 7, 0, 0
**	SFPSTOCHRND	L1, L9, L0, 0, 0, 1
**	SFPSTORE	L1, 2, 1, 7, 0, 0
**	SFPSTOCHRND	L1, L9, L0, 0, 1, 1
**	SFPSTORE	L1, 4, 2, 7, 0, 0
**	SFPSTOCHRND	L1, L9, L0, 0, 7, 1
**	SFPSTORE	L1, 6, 8, 7, 0, 0
**	SFPSTOCHRND	L0, L9, L0, 0, 6, 1
**	SFPSTORE	L0, 8, 6, 7, 0, 0
**	ret
*/

void two () {
  vInt a = dst_reg[0];

  dst_reg[3] = convert<vSMag> (a);
}
/*
**_Z3twov:
**	SFPLOAD	L0, 0, 4, 7, 0, 0
**	SFPCAST	L0, L0, 3
**	SFPSTORE	L0, 6, 4, 7, 0, 0
**	ret
*/

void three () {
  vSMag a = dst_reg[0];

  dst_reg[3] = as<vUInt> (convert<vInt> (a));
  dst_reg[4] = convert<vFloat> (a);
}
/*
**_Z5threev:
**	SFPLOAD	L0, 0, 4, 7, 0, 0
**	SFPCAST	L1, L0, 2
**	SFPSTORE	L1, 6, 4, 7, 0, 0
**	SFPCAST	L0, L0, 1
**	SFPSTORE	L0, 8, 0, 7, 0, 0
**	ret
*/

void twostep1 () {
  vInt a = l_reg[LRegs::LReg0];

  l_reg[LRegs::LReg0] = convert<vFloat> (a);
}
/*
**_Z8twostep1v:
**	# READ L0
**	SFPCAST	L0, L0, 3
**	SFPCAST	L0, L0, 1
**	# WRITE L0
**	ret
*/

void twostep2 () {
  vFloat a = l_reg[LRegs::LReg0];

  l_reg[LRegs::LReg0] = convert<vInt> (a);
}
/*
**_Z8twostep2v:
**	# READ L0
**	SFPCAST	L0, L0, 5
**	SFPCAST	L0, L0, 2
**	# WRITE L0
**	ret
*/
