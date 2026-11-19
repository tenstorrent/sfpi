// { dg-options "-mcpu=tt-bh-tensix -O2 -I [SFPI]/include -fno-exceptions -fno-rtti" }
// { dg-final { check-function-bodies "**" "" } }

namespace ckernel {
extern volatile unsigned long instrn_buffer[];
}
#include "sfpi.h"

using namespace sfpi;

void loop_diff (int i) {
  vUInt a = l_reg[LRegs::LReg0];

#pragma GCC unroll 4
  for (int ix = 0; ix < 4; ix++) {
    a = (__builtin_rvtt_sfpshft_i) (::ckernel::instrn_buffer, a.get(), i + ix, 0, 0, SFPSHFT_MOD1_LOGICAL);
  }

  l_reg[LRegs::LReg3] = a;
}
/*
**_Z9loop_diffi:
**	# READ L0
**	slli	a5,a0,12
**	li	a3,16773120
**	and	a5,a5,a3
**	li	a4, 2046820405	# 5:7a000035
**	add	a5,a5,a4
**	lui	a4,%hi\(_ZN7ckernel13instrn_bufferE\)
**	addi	a4,a4,%lo\(_ZN7ckernel13instrn_bufferE\)
**	sw	a5, 0\(a4\)	# 5:SFPSHFT	L3, L0, -, 5
**	addi	a5,a0,1
**	slli	a5,a5,12
**	and	a5,a5,a3
**	li	a2, 2046821173	# 4:7a000335
**	add	a5,a5,a2
**	sw	a5, 0\(a4\)	# 4:SFPSHFT	L3, L3, -, 5
**	addi	a5,a0,2
**	slli	a5,a5,12
**	and	a5,a5,a3
**	li	a2, 2046821173	# 3:7a000335
**	add	a5,a5,a2
**	sw	a5, 0\(a4\)	# 3:SFPSHFT	L3, L3, -, 5
**	addi	a5,a0,3
**	slli	a5,a5,12
**	and	a5,a5,a3
**	li	a3, 2046821173	# 2:7a000335
**	add	a5,a5,a3
**	sw	a5, 0\(a4\)	# 2:SFPSHFT	L3, L3, -, 5
**	# WRITE L3
**	ret
*/

void loop_common (int i) {
  vUInt a = l_reg[LRegs::LReg0];

#pragma GCC unroll 4
  for (int ix = 0; ix < 4; ix++) {
    a = (__builtin_rvtt_sfpshft_i) (::ckernel::instrn_buffer, a.get(), i, 0, 0, SFPSHFT_MOD1_LOGICAL);
  }

  l_reg[LRegs::LReg3] = a;
}
/*
**_Z11loop_commoni:
**	# READ L0
**	slli	a5,a0,12
**	li	a4,16773120
**	and	a5,a5,a4
**	li	a3, 2046821173	# 1:7a000335
**	lui	a4,%hi\(_ZN7ckernel13instrn_bufferE\)
**	add	a5,a5,a3
**	addi	a4,a4,%lo\(_ZN7ckernel13instrn_bufferE\)
**	li	a3,768
**	xor	a3,a3,a5
**	sw	a3, 0\(a4\)	# 1:SFPSHFT	L3, L0, -, 5
**	sw	a5, 0\(a4\)	# 1:SFPSHFT	L3, L3, -, 5
**	sw	a5, 0\(a4\)	# 1:SFPSHFT	L3, L3, -, 5
**	sw	a5, 0\(a4\)	# 1:SFPSHFT	L3, L3, -, 5
**	# WRITE L3
**	ret
*/

