// { dg-options "-mcpu=tt-qsr32-tensix -O2 -I [SFPI]/include -fno-exceptions -fno-rtti" }
// { dg-final { check-function-bodies "**" "" } }

namespace ckernel{
    unsigned long *instrn_buffer;
}
#include <sfpi.h>

using namespace sfpi;

void vint () {
  { vInt a = dst_reg[0].mode<DataLayout::U8> ();
    l_reg[LRegs::LReg0] = a; }
  { vInt a = l_reg[LRegs::LReg0];
    dst_reg[1].mode<DataLayout::U8> () = a; }
  { vInt a = dst_reg[0].mode<DataLayout::SM8> ();
    l_reg[LRegs::LReg0] = a; }
  { vInt a = l_reg[LRegs::LReg0];
    dst_reg[1].mode<DataLayout::SM8> () = a; }
}
/*
**_Z4vintv:
**	SFPLOAD	L0, 0, 11, 7, 0, 0
**	# WRITE L0
**	# READ L0
**	SFPSTORE	L0, 2, 11, 7, 0, 0
**	SFPLOAD	L0, 0, 5, 7, 0, 0
**	SFPCAST	L0, L0, 2
**	# WRITE L0
**	# READ L0
**	SFPCAST	L0, L0, 3
**	SFPSTORE	L0, 2, 5, 7, 0, 0
**	ret
*/

void vuint () {
  { vUInt a = dst_reg[0].mode<DataLayout::U8> ();
    l_reg[LRegs::LReg0] = a; }
  { vUInt a = l_reg[LRegs::LReg0];
    dst_reg[1].mode<DataLayout::U8> () = a; }
}
/*
**_Z5vuintv:
**	SFPLOAD	L0, 0, 11, 7, 0, 0
**	# WRITE L0
**	# READ L0
**	SFPSTORE	L0, 2, 11, 7, 0, 0
**	ret
*/

void vsmag () {
  { vSMag a = dst_reg[0].mode<DataLayout::U8> ();
    l_reg[LRegs::LReg0] = a; }

 { vSMag a = dst_reg[0].mode<DataLayout::SM8> ();
    l_reg[LRegs::LReg0] = a; }
  { vSMag a = l_reg[LRegs::LReg0];
    dst_reg[1].mode<DataLayout::SM8> () = a; }
}
/*
**_Z5vsmagv:
**	SFPLOAD	L0, 0, 11, 7, 0, 0
**	# WRITE L0
**	SFPLOAD	L0, 0, 5, 7, 0, 0
**	# WRITE L0
**	# READ L0
**	SFPSTORE	L0, 2, 5, 7, 0, 0
**	ret
*/
