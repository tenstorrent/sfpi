// { dg-options "-mcpu=tt-bh-tensix -O2 -fno-exceptions -fno-rtti" }
// { dg-final { check-function-bodies "**" "" } }

void foo () {
  __builtin_rvtt_sfpwriteconfig_i (nullptr, 0x3f801110, 0, 0, 0, 6);
}
/*
**_Z3foov:
**	SFPLOADI	L0, 4368, 2
**	SFPLOADI	L0, 16256, 8	# LV:L0
**	SFPCONFIG	0, 0, 0	# R:L0 CFG:0
**	ret
*/
