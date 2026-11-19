// { dg-options "-mcpu=tt-bh-tensix -O2 -I [SFPI]/include -fno-exceptions -fno-rtti" }
// { dg-final { check-function-bodies "**" "" } }

namespace ckernel{
    unsigned long *instrn_buffer;
}
#include <sfpi.h>

using namespace sfpi;

void foo () {
  vFloat a = l_reg[LRegs::LReg0];
  vFloat r = setman(a, 4095);
  l_reg[LRegs::LReg0] = r;
}
/*
**_Z3foov:
**	# READ L0
**	SFPSETMAN	L0, L0, 4095, 1
**	# WRITE L0
**	ret
*/

void bar (unsigned man) {
  vFloat a = l_reg[LRegs::LReg0];
  vFloat r = setman(a, man);
  l_reg[LRegs::LReg0] = r;
}
/*
**_Z3barj:
**	# READ L0
**	zext.h	a5,a0
**	lui	a3,%hi\(_ZN7ckernel13instrn_bufferE\)
**	li	a4, 1897005056	# 1:71120000
**	lw	a3,%lo\(_ZN7ckernel13instrn_bufferE\)\(a3\)
**	add	a5,a5,a4
**	sw	a5, 0\(a3\)	# 1:SFPLOADI	L1, -, 2
**	srli	a0,a0,16
**	li	a5, 1897398272	# 2:71180000
**	add	a0,a0,a5
**	sw	a0, 0\(a3\)	# 2:SFPLOADI	L1, -, 8	# LV:L1
**	SFPSETMAN	L1, L0, 0, 0
**	SFPMOV	L0, L1, 2
**	# WRITE L0
**	ret
*/

void baz () {
  vFloat a = l_reg[LRegs::LReg0];
  vFloat r = setman(a, 4096);
  l_reg[LRegs::LReg0] = r;
}
/*
**_Z3bazv:
**	# READ L0
**	SFPLOADI	L1, 4096, 2
**	SFPSETMAN	L1, L0, 0, 0
**	SFPMOV	L0, L1, 2
**	# WRITE L0
**	ret
*/

void toto () {
  vFloat a = l_reg[LRegs::LReg0];
  vFloat r = setman(a, 0x123456);
  l_reg[LRegs::LReg0] = r;
}
/*
**_Z4totov:
**	# READ L0
**	SFPLOADI	L1, 13398, 2
**	SFPLOADI	L1, 18, 8	# LV:L1
**	SFPSETMAN	L1, L0, 0, 0
**	SFPMOV	L0, L1, 2
**	# WRITE L0
**	ret
*/
