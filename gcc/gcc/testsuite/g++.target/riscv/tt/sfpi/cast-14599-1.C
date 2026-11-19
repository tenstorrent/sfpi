// { dg-options "-mcpu=tt-bh-tensix -O2 -I [SFPI]/include -fno-exceptions -fno-rtti" }
// { dg-final { check-function-bodies "**" "" } }

namespace ckernel{
  extern unsigned long *instrn_buffer;
}
#include <sfpi.h>

using namespace sfpi;

void i2f_rne () {
  vSMag a = l_reg[LRegs::LReg0];
  vFloat b = convert<vFloat> (a, RoundMode::NearestAway);
  l_reg[LRegs::LReg1] = b;
}
/*
**_Z7i2f_rnev:
**	# READ L0
**	SFPCAST	L1, L0, 0
**	# WRITE L1
**	ret
*/

void i2f_rns () {
  vSMag a = l_reg[LRegs::LReg0];
  vFloat b = convert<vFloat> (a, RoundMode::NearestStochastic);
  l_reg[LRegs::LReg1] = b;
}
/*
**_Z7i2f_rnsv:
**	# READ L0
**	SFPCAST	L1, L0, 1
**	# WRITE L1
**	ret
*/

#if 0
void sm2i () {
  vInt a = l_reg[LRegs::LReg0];
  vInt b = __builtin_rvtt_sfpcast(a.get(), SFPCAST_MOD1_SM32_TO_INT32);
  l_reg[LRegs::LReg1] = b;
}
/*
* *_Z4sm2iv:
* *	# READ L0
* *	SFPCAST	L1, L0, 2
* *	# WRITE L1
* *	ret
*/
#endif

void i2sm () {
  vInt a = l_reg[LRegs::LReg0];
  vInt b = __builtin_rvtt_sfpcast(a.get(), SFPCAST_MOD1_INT32_TO_SM32);
  l_reg[LRegs::LReg1] = b;
}
/*
**_Z4i2smv:
**	# READ L0
**	SFPCAST	L1, L0, 3
**	# WRITE L1
**	ret
*/

void cond () {
    vUInt a = l_reg[LRegs::LReg0];
    vUInt b = l_reg[LRegs::LReg1];
    vSMag c = l_reg[LRegs::LReg2];
    vFloat r;
    
    v_if(a == b) {
      r = convert<vFloat> (c, RoundMode::NearestAway);
    } v_else {
      r = convert<vFloat> (c, RoundMode::NearestStochastic);
    } v_endif;

    l_reg[LRegs::LReg0] = r;
}
/*
**_Z4condv:
**	# READ L0
**	# READ L1
**	# READ L2
**	SFPLE	L1, L0, 0, 1
**	SFPLE	L0, L1, 0, 1
**	SFPCAST	L0, L2, 0
**	SFPCOMPC
**	SFPCAST	L0, L2, 1	# LV:L0
**	SFPENCC	3, 10
**	# WRITE L0
**	ret
*/
