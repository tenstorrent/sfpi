// { dg-options "-mcpu=tt-bh-tensix -O2 -I [SFPI]/include -fno-exceptions -fno-rtti" }
// { dg-final { check-function-bodies "**" "" } }

namespace ckernel{
    unsigned long *instrn_buffer;
}
#include <sfpi.h>

using namespace sfpi;

void empty () {
  v_block {
  } v_endblock;
}
/*
**_Z5emptyv:
**	SFPENCC	3, 10
**	ret
*/

void split (unsigned i) {
  vFloat a = l_reg[LRegs::LReg0];
  vFloat b = l_reg[LRegs::LReg1];
  v_if (a < b) {
    if (i & 1)
      {
	v_if (a < 5.0f) {
	  a = 6.0f;
	} v_endif;
      }
  } v_else {
    b = 5.0f;
  } v_endif;
  l_reg[LRegs::LReg0] = a;
  l_reg[LRegs::LReg1] = b;
}
/*
**_Z5splitj:
**	# READ L0
**	# READ L1
**	SFPGT	L1, L0, 0, 1
**	andi	a0,a0,1
**	beq	a0,zero,.L[0-9]+
**	SFPPUSHC	0
**	SFPLOADI	L2, 16544, 0
**	SFPGT	L2, L0, 0, 1
**	SFPLOADI	L0, 16576, 0	# LV:L0
**	SFPPOPC	0
**	SFPCOMPC
**	SFPLOADI	L1, 16544, 0	# LV:L1
**	SFPENCC	3, 10
**	# WRITE L0
**	# WRITE L1
**	ret
*/

void loop () {
  vFloat a = l_reg[LRegs::LReg0];
  vFloat b = l_reg[LRegs::LReg1];
  v_if (a < b) {
#pragma GCC unroll 0
    for (unsigned i = 5; i--;)
      v_and (a < sFloat16b (0x3f80 + (i << 4)));
  } v_else {
    b = 5.0f;
  } v_endif;
  l_reg[LRegs::LReg0] = a;
  l_reg[LRegs::LReg1] = b;
}
/*
**_Z4loopv:
**	# READ L0
**	# READ L1
**	SFPGT	L1, L0, 0, 1
**	lui	a5,%hi\(_ZN7ckernel13instrn_bufferE\)
**	li	a3,16384
**	lw	a1,%lo\(_ZN7ckernel13instrn_bufferE\)\(a5\)
**	li	a2, 1897922560	# 1:71200000
**	addi	a5,a3,-64
**	addi	a3,a3,-144
**	add	a4,(a5,a2|a2,a5)
**	sw	a4, 0\(a1\)	# 1:SFPLOADI	L2, -, 0
**	SFPGT	L2, L0, 0, 1
**	addi	a5,a5,-16
**	bne	a5,a3,.L[0-9]+
**	SFPCOMPC
**	SFPLOADI	L1, 16544, 0	# LV:L1
**	SFPENCC	3, 10
**	# WRITE L0
**	# WRITE L1
**	ret
*/
