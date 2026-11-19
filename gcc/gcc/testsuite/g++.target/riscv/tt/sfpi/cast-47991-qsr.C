// { dg-options "-mcpu=tt-qsr32-tensix -O2 -I [SFPI]/include -fno-exceptions -fno-rtti" }
// { dg-final { check-function-bodies "**" "" } }

namespace ckernel{
  extern unsigned long *instrn_buffer;
}
#include <sfpi.h>

using namespace sfpi;

void f2s_rne () {
  vFloat a = l_reg[LRegs::LReg0];
  auto r = convert<vSMag>(a, RoundMode::NearestEven);
  l_reg[LRegs::LReg1] = r;
}
/*
**_Z7f2s_rnev:
**	# READ L0
**	SFPCAST	L1, L0, 4
**	# WRITE L1
**	ret
*/

void f2s_rns () {
  vFloat a = l_reg[LRegs::LReg0];
  auto r = convert<vSMag>(a, RoundMode::NearestStochastic);
  l_reg[LRegs::LReg1] = r;
}
/*
**_Z7f2s_rnsv:
**	# READ L0
**	SFPCAST	L1, L0, 5
**	# WRITE L1
**	ret
*/
