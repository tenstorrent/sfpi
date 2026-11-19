// { dg-options "-mcpu=tt-bh-tensix -fno-exceptions -fno-rtti -O2" }
// { dg-final { check-function-bodies "**" "" } }

void mad () {
  auto a = __builtin_rvtt_sfpreadlreg (0);
  auto two = __builtin_rvtt_sfpxloadi (nullptr, 0x3fa00000, 0, 0, -32);
  auto three = __builtin_rvtt_sfpxloadi (nullptr, 0x3fb00000, 0, 0, -32);

  auto p = __builtin_rvtt_sfpmul (a, two, 0);
  auto r = __builtin_rvtt_sfpadd (p, three, 0);
  __builtin_rvtt_sfpwritelreg (r, 0);
}
/*
**_Z3madv:
**	# READ L0
**	SFPLOADI	L1, 16288, 0
**	SFPLOADI	L2, 16304, 0
**	SFPMAD	L0, L0, L1, L2, 0
**	# WRITE L0
**	ret
*/

void muliaddi () {
  auto a = __builtin_rvtt_sfpreadlreg (0);
  auto two = __builtin_rvtt_sfpxloadi (nullptr, 0x3fa00000, 0, 0, -32);
  auto three = __builtin_rvtt_sfpxloadi (nullptr, 0x3fb00000, 0, 0, -32);

  __builtin_rvtt_lreg_pressure (1);
  auto p = __builtin_rvtt_sfpmul (a, two, 0);
  auto r = __builtin_rvtt_sfpadd (p, three, 0);
  __builtin_rvtt_lreg_pressure (0);
  __builtin_rvtt_sfpwritelreg (r, 0);
}
/*
**_Z8muliaddiv:
**	# READ L0
**	SFPMULI	L0, 16288, 0
**	SFPADDI	L0, 16304, 0
**	# WRITE L0
**	ret
*/
