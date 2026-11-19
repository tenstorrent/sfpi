// { dg-options "-mcpu=tt-wh-tensix -O2 -I [SFPI]/include -fno-exceptions -fno-rtti" }
// { dg-final { check-function-bodies "**" "" } }

namespace ckernel{
    unsigned long *instrn_buffer;
}
#include <sfpi.h>

using namespace sfpi;

void f1() {
  vUInt a = l_reg[LRegs::LReg1];

  vUInt r = (__builtin_rvtt_sfpshft_i) (nullptr, a.get(), 2, 0, 0, 0);
  l_reg[LRegs::LReg3] = r;
}
/*
**_Z2f1v:
**	# READ L1
**	SFPSHFT	L1, L0, 2, 1
**	SFPMOV	L3, L1, 2
**	# WRITE L3
**	ret
*/

void f3(int s) {
  vUInt a = l_reg[LRegs::LReg1];

  vUInt r = (__builtin_rvtt_sfpshft_i) (::ckernel::instrn_buffer, a.get(), s, 0, 0, 0);
  l_reg[LRegs::LReg3] = r;
}
/*
**_Z2f3i:
**	# READ L1
**	slli	a0,a0,12
**	li	a5,16773120
**	lui	a4,%hi\(_ZN7ckernel13instrn_bufferE\)
**	and	a0,a0,a5
**	lw	a4,%lo\(_ZN7ckernel13instrn_bufferE\)\(a4\)
**	li	a5, 2046820369	# 1:7a000011
**	add	a0,a0,a5
**	sw	a0, 0\(a4\)	# 1:SFPSHFT	L1, L0, -, 1
**	SFPMOV	L3, L1, 2
**	# WRITE L3
**	ret
*/

void f5() {
  vUInt a = l_reg[LRegs::LReg1];
  vUInt b = l_reg[LRegs::LReg2];

  vUInt r = __builtin_rvtt_sfpshft_v (a.get(), b.get(), 0);
  l_reg[LRegs::LReg3] = r;
}
/*
**_Z2f5v:
**	# READ L1
**	# READ L2
**	SFPSHFT	L1, L2, 0, 0
**	SFPMOV	L3, L1, 2
**	# WRITE L3
**	ret
*/
