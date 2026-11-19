// { dg-options "-mcpu=tt-bh-tensix -O2 -I [SFPI]/include -fno-exceptions -fno-rtti" }
// { dg-final { check-function-bodies "**" "" } }

namespace ckernel {
extern volatile unsigned long instrn_buffer[];
}
#include "sfpi.h"

using namespace sfpi;

void one(int s) {
  vUInt a = l_reg[LRegs::LReg0];

  vUInt r = (__builtin_rvtt_sfpshft_i) (::ckernel::instrn_buffer, a.get(), s, 0, 0, SFPSHFT_MOD1_ARITHMETIC);
  l_reg[LRegs::LReg3] = r;
}
/*
**_Z3onei:
**	# READ L0
**	slli	a0,a0,12
**	li	a5,16773120
**	and	a0,a0,a5
**	li	a5, 2046820407	# 1:7a000037
**	add	a0,a0,a5
**	lui	a5,%hi\(_ZN7ckernel13instrn_bufferE\)
**	sw	a0, %lo\(_ZN7ckernel13instrn_bufferE\)\(a5\)	# 1:SFPSHFT	L3, L0, -, 7
**	# WRITE L3
**	ret
*/

// GCC's RTL doesnt DTRT in this case, needs a lo_sum pass
void two(int s) {
  vUInt a = l_reg[LRegs::LReg0];

  vUInt r = (__builtin_rvtt_sfpshft_i) (::ckernel::instrn_buffer, a.get(), s, 0, 0, SFPSHFT_MOD1_ARITHMETIC);
  r = (__builtin_rvtt_sfpshft_i) (::ckernel::instrn_buffer, r.get(), s, 0, 0, SFPSHFT_MOD1_ARITHMETIC);
  l_reg[LRegs::LReg3] = r;
}
/*
**_Z3twoi:
**	# READ L0
**	slli	a0,a0,12
**	li	a5,16773120
**	and	a0,a0,a5
**	lui	a4,%hi\(_ZN7ckernel13instrn_bufferE\)
**	li	a5, 2046820407	# 1:7a000037
**	add	a5,(a0,a5|a5,a0)
**	addi	a4,a4,%lo\(_ZN7ckernel13instrn_bufferE\)
**	sw	a5, 0\(a4\)	# 1:SFPSHFT	L3, L0, -, 7
**	li	a5, 2046821175	# 2:7a000337
**	add	a0,a0,a5
**	sw	a0, 0\(a4\)	# 2:SFPSHFT	L3, L3, -, 7
**	# WRITE L3
**	ret
*/
