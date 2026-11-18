// { dg-options "-mcpu=tt-qsr32-tensix -O2 -I [SFPI]/include -fno-exceptions -fno-rtti" }
// { dg-final { check-function-bodies "**" "" } }

namespace ckernel{
    unsigned long *instrn_buffer;
}
#include <sfpi.h>

using namespace sfpi;

void f1() {
  vUInt a = l_reg[LRegs::LReg0];

  vUInt r = (__builtin_rvtt_sfpshft_i) (nullptr, a.get(), 2, 0, 0, SFPSHFT_MOD1_LOGICAL);
  l_reg[LRegs::LReg3] = r;
}
/*
**_Z2f1v:
**	# READ L0
**	SFPSHFT	L3, L0, 2, 5
**	# WRITE L3
**	ret
*/

void f2() {
  vInt a = l_reg[LRegs::LReg0];

  vInt r = (__builtin_rvtt_sfpshft_i) (nullptr, a.get(), 2, 0, 0, SFPSHFT_MOD1_ARITHMETIC);
  l_reg[LRegs::LReg3] = r;
}
/*
**_Z2f2v:
**	# READ L0
**	SFPSHFT	L3, L0, 2, 7
**	# WRITE L3
**	ret
*/

void f3(int s) {
  vUInt a = l_reg[LRegs::LReg0];

  vUInt r = (__builtin_rvtt_sfpshft_i) (::ckernel::instrn_buffer, a.get(), s, 0, 0, SFPSHFT_MOD1_LOGICAL);
  l_reg[LRegs::LReg3] = r;
}
/*
**_Z2f3i:
**	# READ L0
**	slli	a0,a0,12
**	li	a5,16773120
**	lui	a4,%hi\(_ZN7ckernel13instrn_bufferE\)
**	and	a0,a0,a5
**	lw	a4,%lo\(_ZN7ckernel13instrn_bufferE\)\(a4\)
**	li	a5, 2046820405	# 1:7a000035
**	add	a0,a0,a5
**	sw	a0, 0\(a4\)	# 1:SFPSHFT	L3, L0, -, 5
**	# WRITE L3
**	ret
*/

void f4(int s) {
  vInt a = l_reg[LRegs::LReg0];

  vInt r = (__builtin_rvtt_sfpshft_i) (::ckernel::instrn_buffer, a.get(), s, 0, 0, SFPSHFT_MOD1_ARITHMETIC);
  l_reg[LRegs::LReg3] = r;
}
/*
**_Z2f4i:
**	# READ L0
**	slli	a0,a0,12
**	li	a5,16773120
**	lui	a4,%hi\(_ZN7ckernel13instrn_bufferE\)
**	and	a0,a0,a5
**	lw	a4,%lo\(_ZN7ckernel13instrn_bufferE\)\(a4\)
**	li	a5, 2046820407	# 1:7a000037
**	add	a0,a0,a5
**	sw	a0, 0\(a4\)	# 1:SFPSHFT	L3, L0, -, 7
**	# WRITE L3
**	ret
*/

void f5() {
  vUInt a = l_reg[LRegs::LReg0];
  vInt b = l_reg[LRegs::LReg1];

  vUInt r = __builtin_rvtt_sfpshft_v (a.get(), b.get(), SFPSHFT_MOD1_LOGICAL);
  l_reg[LRegs::LReg3] = r;
}
/*
**_Z2f5v:
**	# READ L0
**	# READ L1
**	SFPSHFT	L0, L1, 0, 0
**	SFPMOV	L3, L0, 2
**	# WRITE L3
**	ret
*/

void f6() {
  vInt a = l_reg[LRegs::LReg0];
  vInt b = l_reg[LRegs::LReg1];

  vInt r = __builtin_rvtt_sfpshft_v (a.get(), b.get(), SFPSHFT_MOD1_ARITHMETIC);
  l_reg[LRegs::LReg3] = r;
}
/*
**_Z2f6v:
**	# READ L0
**	# READ L1
**	SFPSHFT	L0, L1, 0, 2
**	SFPMOV	L3, L0, 2
**	# WRITE L3
**	ret
*/
