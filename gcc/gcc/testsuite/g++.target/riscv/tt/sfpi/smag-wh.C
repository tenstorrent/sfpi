// { dg-options "-mcpu=tt-wh-tensix -O2 -I [SFPI]/include -fno-exceptions -fno-rtti" }
// { dg-final { check-function-bodies "**" "" } }

namespace ckernel{
    unsigned long *instrn_buffer;
}
#include <sfpi.h>

using namespace sfpi;

void one () {
  vSMag a = dst_reg[0];
  dst_reg[1] = a;

  vSMag b = dst_reg[2].mode<DataLayout::I32>();
  dst_reg[3].mode<DataLayout::I32>() = b;
}
/*
**_Z3onev:
**	SFPLOAD	L0, 0, 4, 3
**	SFPSTORE	L0, 2, 4, 3
**	SFPLOAD	L0, 4, 12, 3
**	SFPSTORE	L0, 6, 12, 3
**	ret
*/

void two () {
  vInt a = dst_reg[0];
  dst_reg[1] = a;


  vInt b = dst_reg[2].mode<DataLayout::SM32>();
  dst_reg[3].mode<DataLayout::SM32>() = b;

  vInt c = dst_reg[4].mode<DataLayout::I32>();
  dst_reg[5].mode<DataLayout::I32>() = c;
}

/*
**_Z3twov:
**	SFPLOAD	L0, 0, 12, 3
**	SFPSTORE	L0, 2, 12, 3
**	SFPLOAD	L0, 4, 12, 3
**	SFPSTORE	L0, 6, 12, 3
**	SFPLOAD	L0, 8, 4, 3
**	SFPSTORE	L0, 10, 4, 3
**	ret
*/
