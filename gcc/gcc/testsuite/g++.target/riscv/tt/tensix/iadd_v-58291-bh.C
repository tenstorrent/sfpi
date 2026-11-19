// { dg-options "-mcpu=tt-bh-tensix -O2 -fno-exceptions -fno-rtti" }
// { dg-final { check-function-bodies "**" "" } }

void foo ()
{
  auto a = __builtin_rvtt_sfpxloadi (nullptr, 0x12340000, 0, 0, -32);
  auto b = __builtin_rvtt_sfpxloadi (nullptr, 0x123, 0, 0, -32);
  auto c = __builtin_rvtt_sfpiadd_v (a, b, 6);
  __builtin_rvtt_sfpwritelreg (c, 0);
}
/*
**_Z3foov:
**	SFPLOADI	L0, 4660, 0
**	SFPLOADI	L1, 291, 2
**	SFPIADD	L0, L1, 0, 6
**	# WRITE L0
**	ret
*/
