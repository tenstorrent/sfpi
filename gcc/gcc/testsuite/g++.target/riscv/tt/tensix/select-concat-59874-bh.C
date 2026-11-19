// { dg-options "-mcpu=tt-bh-tensix -O2 -I [SFPI]/include -fno-exceptions -fno-rtti" }
// { dg-final { check-function-bodies "**" "" } }

namespace ckernel{
    unsigned long *instrn_buffer;
}
#include <sfpi.h>

using namespace sfpi;

void one () {
  auto a = __builtin_rvtt_sfpreadlreg (0);
  auto b = __builtin_rvtt_sfpreadlreg (1);
  auto v = __builtin_rvtt_sfpconcat2 (a, b);
  auto c = __builtin_rvtt_sfpselect2 (v, 0);
  __builtin_rvtt_sfpwritelreg (c, 2);
}
/*
**_Z3onev:
**	# READ L0
**	SFPMOV	L2, L0, 2
**	# WRITE L2
**	ret
*/

void two () {
  auto a = __builtin_rvtt_sfpreadlreg (0);
  auto b = __builtin_rvtt_sfpreadlreg (1);
  auto v = __builtin_rvtt_sfpconcat2 (a, b);
  auto c = __builtin_rvtt_sfpselect2 (v, 0);
  auto d = __builtin_rvtt_sfpselect2 (v, 1);
  __builtin_rvtt_sfpwritelreg (c, 2);
  __builtin_rvtt_sfpwritelreg (d, 3);
}
/*
**_Z3twov:
**	# READ L0
**	# READ L1
**	SFPMOV	L2, L0, 2
**	# WRITE L2
**	SFPMOV	L3, L1, 2
**	# WRITE L3
**	ret
*/
