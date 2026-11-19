// { dg-options "-mcpu=tt-bh-tensix -O2 -I [SFPI]/include -fno-exceptions -fno-rtti" }
// { dg-final { check-function-bodies "**" "" } }

namespace ckernel{
extern unsigned long *instrn_buffer;
}
#include <sfpi.h>

using namespace sfpi;

void bad()
{
  vUInt i = l_reg[LRegs::LReg0];
  v_if (i > 0) {
  } v_endif;
}
/*
**_Z3badv:
**	# READ L0
**	SFPIADD	L0, L9, 0, 2
**	SFPENCC	3, 10
**	ret
*/
