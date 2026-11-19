// { dg-options "-mcpu=tt-bh-tensix -O2 -I [SFPI]/include -fno-exceptions -fno-rtti" }
// { dg-final { check-function-bodies "**" "" } }

namespace ckernel{
    unsigned long *instrn_buffer;
}
#include <sfpi.h>

using namespace sfpi;

void i () {
  vConstIntPrgm0 = -31;
}
/*
**_Z1iv:
**	SFPLOADI	L0, 65505, 4
**	SFPCONFIG	12, 0, 0	# R:L0 CFG:12
**	ret
*/

void fp () {
  vConstFloatPrgm0 = 2.5f;

  vFloat inp = 0;
  vFloat den = 1.0f - inp;
  dst_reg[0] = vFloat (1.0f);
  __builtin_rvtt_sfpwritelreg (den.get (), 3);
}
/*
**_Z2fpv:
**	SFPLOADI	L0, 16416, 0
**	SFPCONFIG	12, 0, 0	# R:L0 CFG:12
**	SFPADD	L3, L9, L10, 1
**	SFPSTORE	L10, 0, 0, 7
**	# WRITE L3
**	ret
*/
