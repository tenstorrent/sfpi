// { dg-options "-mcpu=tt-qsr32-tensix -O2 -fno-exceptions -fno-rtti" }
// { dg-final { check-function-bodies "**" "" } }

void foo () {
  auto r = __builtin_rvtt_sfpreadlreg (2);
  __builtin_rvtt_sfpwriteconfig_v (r, 0, 11);
  __builtin_rvtt_sfpwriteconfig_i (nullptr, 0x00ff, 0, 0, 0, 12);
}
/*
**_Z3foov:
**	# READ L2
**	SFPMOV	L0, L2, 2
**	SFPCONFIG	11, 0, 0	# R:L0 CFG:11
**	SFPCONFIG	12, 255, 1	# CFG:12
**	ret
*/

void bar () {
  auto r = __builtin_rvtt_sfpxloadi (nullptr, 0x00ff, 0, 0, -32);
  __builtin_rvtt_sfpwriteconfig_v (r, 0, 9);
}
/*
**_Z3barv:
**	SFPCONFIG	9, 255, 1	# CFG:9
**	ret
*/

void neg () {
  auto r = __builtin_rvtt_sfpxloadi (nullptr, 0xbf800000, 0, 0, -32);
  __builtin_rvtt_sfpwriteconfig_v (r, 0, 11);
  r = __builtin_rvtt_sfpxloadi (nullptr, 0x3f800000, 0, 0, -32);
  __builtin_rvtt_sfpwriteconfig_v (r, 0, 11);
}
/*
**_Z3negv:
**	SFPCONFIG	11, 0, 1	# CFG:11
**	SFPMOV	L0, L10, 2
**	SFPCONFIG	11, 0, 0	# R:L0 CFG:11
**	ret
*/

void no () {
  auto r = __builtin_rvtt_sfpxloadi (nullptr, 0x3f800000, 0, 0, -32);
  __builtin_rvtt_sfpwriteconfig_v (r, 0, 9);
}
/*
**_Z2nov:
**	SFPMOV	L0, L10, 2
**	SFPCONFIG	9, 0, 0	# R:L0 CFG:9
**	ret
*/
