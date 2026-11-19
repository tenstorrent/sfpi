// { dg-options "-mcpu=tt-bh-tensix -O2 -I [SFPI]/include -fno-exceptions -fno-rtti" }
// { dg-final { check-function-bodies "**" "" } }

namespace ckernel{
    unsigned long *instrn_buffer;
}
#include <sfpi.h>

using namespace sfpi;

void foo () {
  // Basic test
  vFloat a = l_reg[LRegs::LReg0];
  vFloat b = l_reg[LRegs::LReg1];
  vFloat r_lo = __builtin_rvtt_sfpmul24 (a.get(), b.get(), SFPMUL24_MOD1_LOWER);
  vFloat r_hi = __builtin_rvtt_sfpmul24 (a.get(), b.get(), SFPMUL24_MOD1_UPPER);
  l_reg[LRegs::LReg2] = r_lo;
  l_reg[LRegs::LReg3] = r_hi;
}
/*
**_Z3foov:
**	# READ L0
**	# READ L1
**	SFPMUL24	L2, L0, L1, 0
**	SFPMUL24	L3, L0, L1, 1
**	# WRITE L2
**	# WRITE L3
**	ret
*/

void bar () {
  // Live value test
  vFloat a = l_reg[LRegs::LReg0];
  vFloat b = l_reg[LRegs::LReg1];
  vFloat c = l_reg[LRegs::LReg4];
  vFloat d = l_reg[LRegs::LReg5];
  vFloat r_lo, r_hi;
  
  v_if (a < b) {
    r_lo = c;
    r_hi = d;
  } v_else {
    r_lo = __builtin_rvtt_sfpmul24 (a.get(), b.get(), SFPMUL24_MOD1_LOWER);
    r_hi = __builtin_rvtt_sfpmul24 (a.get(), b.get(), SFPMUL24_MOD1_UPPER);
  } v_endif;

  l_reg[LRegs::LReg2] = r_lo;
  l_reg[LRegs::LReg3] = r_hi;
}
/*
**_Z3barv:
**	# READ L0
**	# READ L1
**	# READ L4
**	# READ L5
**	SFPGT	L1, L0, 0, 1
**	SFPCOMPC
**	SFPMOV	L2, L4, 2
**	SFPMUL24	L2, L0, L1, 0	# LV:L2
**	SFPMOV	L3, L5, 2
**	SFPMUL24	L3, L0, L1, 1	# LV:L3
**	SFPENCC	3, 10
**	# WRITE L2
**	# WRITE L3
**	ret
*/
