// { dg-options "-mcpu=tt-bh-tensix -O2 -I [SFPI]/include -fno-exceptions -fno-rtti" }
// { dg-final { check-function-bodies "**" "" } }

namespace ckernel{
    unsigned long *instrn_buffer;
}
#include <sfpi.h>

using namespace sfpi;

void one () {
  vFloat a = dst_reg[0];
  vFloat b = dst_reg[2].mode<DataLayout::F32> (2);

  dst_reg[4] = a;
  dst_reg[5].mode<DataLayout::F16b> () = b;
}
/*
**_Z3onev:
**	SFPLOAD	L1, 0, 0, 7
**	SFPLOAD	L0, 4, 3, 2
**	SFPSTORE	L1, 8, 0, 7
**	SFPSTORE	L0, 10, 2, 7
**	ret
*/

void two () {
  vInt a = dst_reg[0];
  vUInt b = dst_reg[1];

  dst_reg[2] = a;
  dst_reg[3] = b;
}
/*
**_Z3twov:
**	SFPLOAD	L1, 0, 4, 7
**	SFPLOAD	L0, 2, 4, 7
**	SFPSTORE	L1, 4, 4, 7
**	SFPSTORE	L0, 6, 4, 7
**	ret
*/

void three () {
  vFloat16a a = dst_reg[0];
  vFloat16b b = dst_reg[1];

  dst_reg[0] = a;
  dst_reg[1] = b;
}
/*
**_Z5threev:
**	SFPLOAD	L1, 0, 1, 7
**	SFPLOAD	L0, 2, 2, 7
**	SFPSTORE	L1, 0, 1, 7
**	SFPSTORE	L0, 2, 2, 7
**	ret
*/

void four () {
  vUInt16 a = dst_reg[0];
  vSMag b = dst_reg[1];
  vSMag16 c = dst_reg[2];

  dst_reg[0] = a;
  dst_reg[1] = b;
  dst_reg[2] = c;
}
/*
**_Z4fourv:
**	SFPLOAD	L2, 0, 6, 7
**	SFPLOAD	L1, 2, 4, 7
**	SFPLOAD	L0, 4, 8, 7
**	SFPSTORE	L2, 0, 6, 7
**	SFPSTORE	L1, 2, 4, 7
**	SFPSTORE	L0, 4, 8, 7
**	ret
*/
