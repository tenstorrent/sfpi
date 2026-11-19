// { dg-options "-mcpu=tt-bh-tensix -O2 -I [SFPI]/include -fno-exceptions -fno-rtti" }
// { dg-final { check-function-bodies "**" "" } }

namespace ckernel{
    unsigned long *instrn_buffer;
}
#include <sfpi.h>

using namespace sfpi;

void fnuvv () {
    vUInt a = l_reg[LRegs::LReg0];
    vUInt b = l_reg[LRegs::LReg1];
    vFloat c = l_reg[LRegs::LReg2];
    vFloat d = l_reg[LRegs::LReg3];
    vFloat r;
    
    v_if(a != b) {
      r = c;
    } v_else {
      r = d;
    } v_endif;

    l_reg[LRegs::LReg0] = r;
}
/*
**_Z5fnuvvv:
**	# READ L0
**	# READ L1
**	# READ L2
**	# READ L3
**	SFPIADD	L1, L0, 0, 6
**	SFPSETCC	L1, 0, 2
**	SFPCOMPC
**	SFPMOV	L0, L2, 2
**	SFPMOV	L0, L3, 0	# LV:L0
**	SFPENCC	3, 10
**	# WRITE L0
**	ret
*/

void fnuvi (int i) {
    vUInt a = l_reg[LRegs::LReg0];
    vFloat c = l_reg[LRegs::LReg2];
    vFloat d = l_reg[LRegs::LReg3];
    vFloat r;
    
    v_if(a != i) {
      r = c;
    } v_else {
      r = d;
    } v_endif;

    l_reg[LRegs::LReg0] = r;
}
/*
**_Z5fnuvii:
**	# READ L0
**	# READ L2
**	# READ L3
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
**	SFPIADD	L1, L0, 0, 6
**	SFPSETCC	L1, 0, 2
**	SFPCOMPC
**	SFPMOV	L0, L2, 2
**	SFPMOV	L0, L3, 0	# LV:L0
**	SFPENCC	3, 10
**	# WRITE L0
**	ret
*/

void fnuvc () {
    vUInt a = l_reg[LRegs::LReg0];
    vFloat c = l_reg[LRegs::LReg2];
    vFloat d = l_reg[LRegs::LReg3];
    vFloat r;
    
    v_if(a != 5) {
      r = c;
    } v_else {
      r = d;
    } v_endif;

    l_reg[LRegs::LReg0] = r;
}
/*
**_Z5fnuvcv:
**	# READ L0
**	# READ L2
**	# READ L3
**	SFPIADD	L0, L0, -5, 5
**	SFPSETCC	L0, 0, 2
**	SFPCOMPC
**	SFPMOV	L0, L2, 2
**	SFPMOV	L0, L3, 0	# LV:L0
**	SFPENCC	3, 10
**	# WRITE L0
**	ret
*/

void fns () {
    vInt a = l_reg[LRegs::LReg0];
    vInt b = l_reg[LRegs::LReg1];
    vFloat c = l_reg[LRegs::LReg2];
    vFloat d = l_reg[LRegs::LReg3];
    vFloat r;
    
    v_if(a != b) {
      r = c;
    } v_else {
      r = d;
    } v_endif;

    l_reg[LRegs::LReg0] = r;
}
/*
**_Z3fnsv:
**	# READ L0
**	# READ L1
**	# READ L2
**	# READ L3
**	SFPIADD	L1, L0, 0, 6
**	SFPSETCC	L1, 0, 2
**	SFPCOMPC
**	SFPMOV	L0, L2, 2
**	SFPMOV	L0, L3, 0	# LV:L0
**	SFPENCC	3, 10
**	# WRITE L0
**	ret
*/
