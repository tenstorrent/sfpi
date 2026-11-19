// { dg-options "-mcpu=tt-qsr32-tensix -O2 -I [SFPI]/include -fno-exceptions -fno-rtti" }
// { dg-final { check-function-bodies "**" "" } }

// Verify we notice a - b is a + -1.0 * b

namespace ckernel{
    unsigned long *instrn_buffer;
}
#include <sfpi.h>

using namespace sfpi;

void sub1() {
  vFloat a = l_reg[LRegs::LReg0];
  vFloat b = l_reg[LRegs::LReg1];
  vFloat r;

  r = a - b;
  
  l_reg[LRegs::LReg3] = r;
}
/*
**_Z4sub1v:
**	# READ L0
**	# READ L1
**	SFPADD	L3, L1, L0, 1
**	# WRITE L3
**	ret
*/

void sub2() {
  vFloat a = l_reg[LRegs::LReg0];
  vFloat b = l_reg[LRegs::LReg1];

  b -= a;
  
  l_reg[LRegs::LReg3] = b;
}
/*
**_Z4sub2v:
**	# READ L0
**	# READ L1
**	SFPADD	L3, L0, L1, 1
**	# WRITE L3
**	ret
*/

void sub3() {
  vFloat a = l_reg[LRegs::LReg0];
  vFloat b = l_reg[LRegs::LReg1];
  vFloat r;

  r = -a - b;
  
  l_reg[LRegs::LReg3] = r;
}
/*
**_Z4sub3v:
**	# READ L0
**	# READ L1
**	SFPADD	L3, L1, L0, 3
**	# WRITE L3
**	ret
*/

void sub4() {
  vFloat a = l_reg[LRegs::LReg0];
  vFloat b = l_reg[LRegs::LReg1];
  vFloat r;

  v_if (a < b) {
    r = a - b;
  } v_else {
    r = b - a;
  } v_endif;
  
  l_reg[LRegs::LReg3] = r;
}
/*
**_Z4sub4v:
**	# READ L0
**	# READ L1
**	SFPGT	L1, L0, 1, 1
**	SFPADD	L3, L1, L0, 1
**	SFPCOMPC
**	SFPADD	L3, L0, L1, 1	# LV:L3
**	SFPENCC	3, 10
**	# WRITE L3
**	ret
*/

void sub5() {
  vFloat a = l_reg[LRegs::LReg0];
  vFloat b = l_reg[LRegs::LReg1];

  v_if (a < b) {
    b -= a;
  } v_else {
    b -= -a;
  } v_endif;
  
  l_reg[LRegs::LReg3] = b;
}
/*
**_Z4sub5v:
**	# READ L0
**	# READ L1
**	SFPGT	L1, L0, 1, 1
**	SFPADD	L1, L0, L1, 1	# LV:L1
**	SFPCOMPC
**	SFPADD	L1, L1, L0, 0	# LV:L1
**	SFPENCC	3, 10
**	SFPMOV	L3, L1, 2
**	# WRITE L3
**	ret
*/

void sub6() {
  vFloat a = l_reg[LRegs::LReg0];
  vFloat b = l_reg[LRegs::LReg1];
  vFloat r;

  v_if (a < b) {
    r = -a - b;
  } v_else {
    r = a - b;
  } v_endif;
  
  l_reg[LRegs::LReg3] = r;
}
/*
**_Z4sub6v:
**	# READ L0
**	# READ L1
**	SFPGT	L1, L0, 1, 1
**	SFPADD	L3, L1, L0, 3
**	SFPCOMPC
**	SFPADD	L3, L1, L0, 1	# LV:L3
**	SFPENCC	3, 10
**	# WRITE L3
**	ret
*/
