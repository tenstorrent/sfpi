// { dg-options "-mcpu=tt-bh-tensix -O2 -fno-exceptions -fno-rtti" }
// { dg-final { check-function-bodies "**" "" } }

void vecdiff () {
  auto a = __builtin_rvtt_sfpreadlreg (0);
  auto b = __builtin_rvtt_sfpreadlreg (1);
  auto c = __builtin_rvtt_sfpreadlreg (2);

  __builtin_rvtt_sfppushc (0);
  a = __builtin_rvtt_sfpshft_v_lv (a, b, c, 0);
  __builtin_rvtt_sfppopc (0);
  __builtin_rvtt_sfpwritelreg (a, 0);
}
/*
**_Z7vecdiffv:
**	# READ L0
**	# READ L1
**	# READ L2
**	SFPMOV	L0, L1, 0	# LV:L0
**	SFPSHFT	L0, L2, 0, 0	# LV:L0
**	SFPENCC	3, 10
**	# WRITE L0
**	ret
*/

void vecsame () {
  auto a = __builtin_rvtt_sfpreadlreg (0);
  auto c = __builtin_rvtt_sfpreadlreg (2);

  __builtin_rvtt_sfppushc (0);
  a = __builtin_rvtt_sfpshft_v_lv (a, a, c, 0);
  __builtin_rvtt_sfppopc (0);
  __builtin_rvtt_sfpwritelreg (a, 0);
}
/*
**_Z7vecsamev:
**	# READ L0
**	# READ L2
**	SFPSHFT	L0, L2, 0, 0	# LV:L0
**	SFPENCC	3, 10
**	# WRITE L0
**	ret
*/

void scldiff () {
  auto a = __builtin_rvtt_sfpreadlreg (0);
  auto b = __builtin_rvtt_sfpreadlreg (1);

  __builtin_rvtt_sfppushc (0);
  a = __builtin_rvtt_sfpshft_i_lv (nullptr, a, b, 3, 0, 0, 0);
  __builtin_rvtt_sfppopc (0);
  __builtin_rvtt_sfpwritelreg (a, 0);
}
/*
**_Z7scldiffv:
**	# READ L0
**	# READ L1
**	SFPSHFT	L0, L1, 3, 5	# LV:L0
**	SFPENCC	3, 10
**	# WRITE L0
**	ret
*/

void sclsame () {
  auto a = __builtin_rvtt_sfpreadlreg (0);

  __builtin_rvtt_sfppushc (0);
  a = __builtin_rvtt_sfpshft_i_lv (nullptr, a, a, 3, 0, 0, 0);
  __builtin_rvtt_sfppopc (0);
  __builtin_rvtt_sfpwritelreg (a, 0);
}
/*
**_Z7sclsamev:
**	# READ L0
**	SFPSHFT	L0, L0, 3, 5	# LV:L0
**	SFPENCC	3, 10
**	# WRITE L0
**	ret
*/
