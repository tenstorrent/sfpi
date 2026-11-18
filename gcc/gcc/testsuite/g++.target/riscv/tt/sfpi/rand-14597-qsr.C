// { dg-options "-mcpu=tt-qsr32-tensix -O2 -I [SFPI]/include -fno-exceptions -fno-rtti" }
// { dg-final { check-function-bodies "**" "" } }

namespace ckernel{
    unsigned long *instrn_buffer;
}
#include <sfpi.h>

using namespace sfpi;

void foo () {
  // Basic test
  vInt r = rand ();
  l_reg[LRegs::LReg0] = r;
}
/*
**_Z3foov:
**	SFPMOV	L0, L9, 8	# CFG:9
**	# WRITE L0
**	ret
*/

void bar () {
  // Live value test
  vFloat a = l_reg[LRegs::LReg0];
  vFloat b = l_reg[LRegs::LReg1];
  vInt c = l_reg[LRegs::LReg2];
  
  vInt r;
  v_if (a < b) {
    r = c;
  } v_else {
    r = rand ();
  } v_endif;
  
  l_reg[LRegs::LReg0] = r;
}
/*
**_Z3barv:
**	# READ L0
**	# READ L1
**	# READ L2
**	SFPGT	L1, L0, 1, 1
**	SFPCOMPC
**	SFPMOV	L0, L2, 2
**	SFPMOV	L0, L9, 8	# LV:L0 CFG:9
**	SFPENCC	3, 10
**	# WRITE L0
**	ret
*/

void baz () {
  // Do not CSE
  vInt r = rand () + rand();
  l_reg[LRegs::LReg0] = r;
}
/*
**_Z3bazv:
**	SFPMOV	L0, L9, 8	# CFG:9
**	SFPMOV	L1, L9, 8	# CFG:9
**	SFPIADD	L0, L1, 0, 4
**	# WRITE L0
**	ret
*/
