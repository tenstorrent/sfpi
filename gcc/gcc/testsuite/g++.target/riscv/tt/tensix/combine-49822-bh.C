// { dg-options "-mcpu=tt-bh-tensix -O2 -fno-exceptions -fno-rtti" }
// { dg-final { check-function-bodies "**" "" } }

void notnot () {
  auto a = __builtin_rvtt_sfpreadlreg (0);

  a = __builtin_rvtt_sfpnot (a);
  a = __builtin_rvtt_sfpnot (a);

  __builtin_rvtt_sfpwritelreg (a, 0);
}
/*
**_Z6notnotv:
**	# READ L0
**	# WRITE L0
**	ret
*/

void notnotlv () {
  auto a = __builtin_rvtt_sfpreadlreg (0);
  auto lv = __builtin_rvtt_sfpreadlreg (1);

  a = __builtin_rvtt_sfpnot_lv (lv, a);
  a = __builtin_rvtt_sfpnot_lv (a, a);

  __builtin_rvtt_sfpwritelreg (a, 0);
}
/*
**_Z8notnotlvv:
**	# READ L0
**	# READ L1
**	SFPMOV	L1, L0, 0	# LV:L1
**	SFPMOV	L0, L1, 2
**	# WRITE L0
**	ret
*/

void notnotlv2 () {
  auto a = __builtin_rvtt_sfpreadlreg (0);

  a = __builtin_rvtt_sfpnot_lv (a, a);
  a = __builtin_rvtt_sfpnot_lv (a, a);

  __builtin_rvtt_sfpwritelreg (a, 0);
}
/*
**_Z9notnotlv2v:
**	# READ L0
**	# WRITE L0
**	ret
*/

void negneg () {
  auto a = __builtin_rvtt_sfpreadlreg (0);

  a = __builtin_rvtt_sfpmov (a, 1);
  a = __builtin_rvtt_sfpmov (a, 1);

  __builtin_rvtt_sfpwritelreg (a, 0);
}
/*
**_Z6negnegv:
**	# READ L0
**	# WRITE L0
**	ret
*/

