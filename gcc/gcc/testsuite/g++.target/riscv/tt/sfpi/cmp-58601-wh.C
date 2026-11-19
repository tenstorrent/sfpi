// { dg-options "-mcpu=tt-wh-tensix -O2 -I [SFPI]/include -fno-exceptions -fno-rtti" }
// { dg-final { check-function-bodies "**" "" } }

namespace ckernel {
extern volatile unsigned long instrn_buffer[];
}
#include <sfpi.h>
using namespace sfpi;

void ne_cst() {
  vFloat v = l_reg[LRegs::LReg0];
  v_if (v != 0.5f) {
    v = 1.0f;
  } v_endif;
  l_reg[LRegs::LReg0] = v;
}
/*
**_Z6ne_cstv:
**	# READ L0
**	SFPLOADI	L1, 16128, 0
**	SFPIADD	L1, L0, 0, 6
**	SFPSETCC	L1, 0, 2
**	SFPMOV	L0, L10, 0	# LV:L0
**	SFPENCC	3, 10
**	# WRITE L0
**	ret
*/

void eq_cst() {
  vFloat v = l_reg[LRegs::LReg0];
  v_if (v == 0.5f) {
    v = 1.0f;
  } v_endif;
  l_reg[LRegs::LReg0] = v;
}
/*
**_Z6eq_cstv:
**	# READ L0
**	SFPLOADI	L1, 16128, 0
**	SFPIADD	L1, L0, 0, 6
**	SFPSETCC	L1, 0, 6
**	SFPMOV	L0, L10, 0	# LV:L0
**	SFPENCC	3, 10
**	# WRITE L0
**	ret
*/

void ne_vec()   {
  vFloat v = l_reg[LRegs::LReg0];
  vFloat w = l_reg[LRegs::LReg1];
  v_if (v != w) {
    v = 1.0f;
  } v_endif;
  l_reg[LRegs::LReg0] = v;
}
/*
**_Z6ne_vecv:
**	# READ L0
**	# READ L1
**	SFPIADD	L1, L0, 0, 6
**	SFPSETCC	L1, 0, 2
**	SFPMOV	L0, L10, 0	# LV:L0
**	SFPENCC	3, 10
**	# WRITE L0
**	ret
*/

void eq_vec()   {
  vFloat v = l_reg[LRegs::LReg0];
  vFloat w = l_reg[LRegs::LReg1];
  v_if (v == w) {
    v = 1.0f;
  } v_endif;
  l_reg[LRegs::LReg0] = v;  
}
/*
**_Z6eq_vecv:
**	# READ L0
**	# READ L1
**	SFPIADD	L1, L0, 0, 6
**	SFPSETCC	L1, 0, 6
**	SFPMOV	L0, L10, 0	# LV:L0
**	SFPENCC	3, 10
**	# WRITE L0
**	ret
*/
