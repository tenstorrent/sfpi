// { dg-options "-mcpu=tt-qsr32-tensix -O2 -I [SFPI]/include -fno-exceptions -fno-rtti" }
// { dg-final { check-function-bodies "**" "" } }

namespace ckernel{
    unsigned long *instrn_buffer;
}
#include <sfpi.h>

using namespace sfpi;

namespace recip {
void foo () {
  // Basic test
  vFloat a = l_reg[LRegs::LReg0];
  vFloat r = approx_recip (a);
  l_reg[LRegs::LReg1] = r;
}
/*
**_ZN5recip3fooEv:
**	# READ L0
**	SFPNONLINEAR	L1, L0, 0
**	# WRITE L1
**	ret
*/

void bar () {
  // Live value test
  vFloat a = l_reg[LRegs::LReg0];
  vFloat b = l_reg[LRegs::LReg1];
  vFloat c = l_reg[LRegs::LReg2];
  
  vFloat r;
  v_if (a < b) {
    r = c;
  } v_else {
    r = approx_recip (a);
  } v_endif;

  l_reg[LRegs::LReg3] = r;
}
/*
**_ZN5recip3barEv:
**	# READ L0
**	# READ L1
**	# READ L2
**	SFPGT	L1, L0, 1, 1
**	SFPCOMPC
**	SFPNONLINEAR	L2, L0, 0	# LV:L2
**	SFPMOV	L3, L2, 2
**	SFPENCC	3, 10
**	# WRITE L3
**	ret
*/
}

namespace negrecip {
void foo () {
  // Basic test
  vFloat a = l_reg[LRegs::LReg0];
  vFloat r = approx_recip (a, RecipMode::IfNegative);
  l_reg[LRegs::LReg1] = r;
}
/*
**_ZN8negrecip3fooEv:
**	# READ L0
**	SFPNONLINEAR	L1, L0, 1
**	# WRITE L1
**	ret
*/

void bar () {
  // Live value test
  vFloat a = l_reg[LRegs::LReg0];
  vFloat b = l_reg[LRegs::LReg1];
  vFloat c = l_reg[LRegs::LReg2];
  
  vFloat r;
  v_if (a < b) {
    r = c;
  } v_else {
    r = approx_recip (a, RecipMode::IfNegative);
  } v_endif;

  l_reg[LRegs::LReg3] = r;
}
/*
**_ZN8negrecip3barEv:
**	# READ L0
**	# READ L1
**	# READ L2
**	SFPGT	L1, L0, 1, 1
**	SFPCOMPC
**	SFPNONLINEAR	L2, L0, 1	# LV:L2
**	SFPMOV	L3, L2, 2
**	SFPENCC	3, 10
**	# WRITE L3
**	ret
*/
}

namespace expon {
void foo () {
  // Basic test
  vFloat a = l_reg[LRegs::LReg0];
  vFloat r = approx_exp (a);
  l_reg[LRegs::LReg1] = r;
}
/*
**_ZN5expon3fooEv:
**	# READ L0
**	SFPNONLINEAR	L1, L0, 4
**	# WRITE L1
**	ret
*/

void bar () {
  // Live value test
  vFloat a = l_reg[LRegs::LReg0];
  vFloat b = l_reg[LRegs::LReg1];
  vFloat c = l_reg[LRegs::LReg2];
  
  vFloat r;
  v_if (a < b) {
    r = c;
  } v_else {
    r = approx_exp (a);
  } v_endif;
  l_reg[LRegs::LReg3] = r;
}
/*
**_ZN5expon3barEv:
**	# READ L0
**	# READ L1
**	# READ L2
**	SFPGT	L1, L0, 1, 1
**	SFPCOMPC
**	SFPNONLINEAR	L2, L0, 4	# LV:L2
**	SFPMOV	L3, L2, 2
**	SFPENCC	3, 10
**	# WRITE L3
**	ret
*/
}
