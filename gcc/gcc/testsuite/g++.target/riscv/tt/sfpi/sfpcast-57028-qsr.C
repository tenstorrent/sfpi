// { dg-options "-mcpu=tt-qsr32-tensix -O2 -I [SFPI]/include -fno-exceptions -fno-rtti" }
// { dg-final { check-function-bodies "**" "" } }

namespace ckernel{
    unsigned long *instrn_buffer;
}
#include <sfpi.h>

using namespace sfpi;

void one () {
  vInt a = l_reg[LRegs::LReg0];
  vSMag s = convert<vSMag> (a);
  auto i = convert<vInt> (s);
  l_reg[LRegs::LReg0] = i;
}
/*
**_Z3onev:
**	# READ L0
**	# WRITE L0
**	ret
*/

void two () {
  vInt a = dst_reg[0].mode<DataLayout::SM32> ();
  dst_reg[0] = a;
  dst_reg[1].mode<DataLayout::SM32> () = a;
}
/*
**_Z3twov:
**	SFPLOAD	L0, 0, 4, 7, 0, 0
**	SFPCAST	L1, L0, 2
**	SFPSTORE	L1, 0, 4, 7, 0, 0
**	SFPSTORE	L0, 2, 4, 7, 0, 0
**	ret
*/

