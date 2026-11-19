// { dg-options "-mcpu=tt-bh-tensix -O2 -I [SFPI]/include -fno-exceptions -fno-rtti" }
// { dg-final { check-function-bodies "**" "" } }

namespace ckernel{
    unsigned long *instrn_buffer;
}
#include <sfpi.h>

using namespace sfpi;

void one () {
  vUInt a = dst_reg[0].mode<DataLayout::LO16>();
  vUInt b = dst_reg[2].mode<DataLayout::HI16>();

  dst_reg[4].mode<DataLayout::LO16>() = a;
  dst_reg[6].mode<DataLayout::HI16>() = b;
}
/*
**_Z3onev:
**	SFPLOAD	L1, 0, 9, 7
**	SFPLOAD	L0, 4, 7, 7
**	SFPSTORE	L1, 8, 9, 7
**	SFPSTORE	L0, 12, 7, 7
**	ret
*/

void two () {
  vUInt16 a = dst_reg[0].mode<DataLayout::LO16>();

  dst_reg[4].mode<DataLayout::LO16>() = a;
  dst_reg[6].mode<DataLayout::HI16>() = a;
}

/*
**_Z3twov:
**	SFPLOAD	L0, 0, 9, 7
**	SFPSTORE	L0, 8, 9, 7
**	SFPSTORE	L0, 12, 7, 7
**	ret
*/

void three () {
  vMag a (10);

  dst_reg[4].mode<DataLayout::LO16>() = a;
  dst_reg[6].mode<DataLayout::HI16>() = a;
}

/*
**_Z5threev:
**	SFPLOADI	L0, 10, 2
**	SFPSTORE	L0, 8, 9, 7
**	SFPSTORE	L0, 12, 7, 7
**	ret
*/
