// { dg-options "-mcpu=tt-bh-tensix -O2 -fno-exceptions -fno-rtti" }
// { dg-final { check-function-bodies "**" "" } }

void mulinone () {
  auto a = __builtin_rvtt_sfpreadlreg (0);

  __builtin_rvtt_sfppushc (0);
  a = __builtin_rvtt_sfpmuli (nullptr, a, 0, 0, 0, 0);
  __builtin_rvtt_sfppopc (0);
  __builtin_rvtt_sfpwritelreg (a, 0);
}
/*
**_Z8mulinonev:
**	# READ L0
**	SFPMULI	L0, 0, 0
**	SFPENCC	3, 10
**	# WRITE L0
**	ret
*/

void mulidiff () {
  auto a = __builtin_rvtt_sfpreadlreg (0);
  auto b = __builtin_rvtt_sfpreadlreg (1);

  __builtin_rvtt_sfppushc (0);
  a = __builtin_rvtt_sfpmuli_lv (nullptr, a, b, 0, 0, 0, 0);
  __builtin_rvtt_sfppopc (0);
  __builtin_rvtt_sfpwritelreg (a, 0);
}
/*
**_Z8mulidiffv:
**	# READ L0
**	# READ L1
**	SFPMOV	L0, L1, 0	# LV:L0
**	SFPMULI	L0, 0, 0	# LV:L0
**	SFPENCC	3, 10
**	# WRITE L0
**	ret
*/

void mulisame () {
  auto a = __builtin_rvtt_sfpreadlreg (0);

  __builtin_rvtt_sfppushc (0);
  a = __builtin_rvtt_sfpmuli_lv (nullptr, a, a, 0, 0, 0, 0);
  __builtin_rvtt_sfppopc (0);
  __builtin_rvtt_sfpwritelreg (a, 0);
}
/*
**_Z8mulisamev:
**	# READ L0
**	SFPMULI	L0, 0, 0	# LV:L0
**	SFPENCC	3, 10
**	# WRITE L0
**	ret
*/

void addinone () {
  auto a = __builtin_rvtt_sfpreadlreg (0);

  __builtin_rvtt_sfppushc (0);
  a = __builtin_rvtt_sfpaddi (nullptr, a, 0, 0, 0, 0);
  __builtin_rvtt_sfppopc (0);
  __builtin_rvtt_sfpwritelreg (a, 0);
}
/*
**_Z8addinonev:
**	# READ L0
**	SFPADDI	L0, 0, 0
**	SFPENCC	3, 10
**	# WRITE L0
**	ret
*/

void addidiff () {
  auto a = __builtin_rvtt_sfpreadlreg (0);
  auto b = __builtin_rvtt_sfpreadlreg (1);

  __builtin_rvtt_sfppushc (0);
  a = __builtin_rvtt_sfpaddi_lv (nullptr, a, b, 0, 0, 0, 0);
  __builtin_rvtt_sfppopc (0);
  __builtin_rvtt_sfpwritelreg (a, 0);
}
/*
**_Z8addidiffv:
**	# READ L0
**	# READ L1
**	SFPMOV	L0, L1, 0	# LV:L0
**	SFPADDI	L0, 0, 0	# LV:L0
**	SFPENCC	3, 10
**	# WRITE L0
**	ret
*/

void addisame () {
  auto a = __builtin_rvtt_sfpreadlreg (0);

  __builtin_rvtt_sfppushc (0);
  a = __builtin_rvtt_sfpaddi_lv (nullptr, a, a, 0, 0, 0, 0);
  __builtin_rvtt_sfppopc (0);
  __builtin_rvtt_sfpwritelreg (a, 0);
}
/*
**_Z8addisamev:
**	# READ L0
**	SFPADDI	L0, 0, 0	# LV:L0
**	SFPENCC	3, 10
**	# WRITE L0
**	ret
*/

