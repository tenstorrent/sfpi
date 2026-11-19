// { dg-options "-mcpu=tt-bh-tensix -O2 -fno-exceptions -fno-rtti" }
// { dg-final { check-function-bodies "**" "" } }

extern volatile unsigned long iptr[];

void addv () {
  auto a = __builtin_rvtt_sfpload (nullptr, 0, 0, 0, 0, 0);
  auto one = __builtin_rvtt_sfpreadlreg (10);
  auto zero = __builtin_rvtt_sfpreadlreg (9);

  __builtin_rvtt_sfppushc (0);

  __builtin_rvtt_sfpiadd_v (one, a, 2);
  a = __builtin_rvtt_sfpassign_lv (a, zero);

  __builtin_rvtt_sfppopc (0);
  __builtin_rvtt_sfpstore (nullptr, a, 0, 0, 0, 0, 0);
}
/*
**_Z4addvv:
**	SFPLOAD	L0, 0, 0, 0
**	SFPMOV	L1, L10, 2
**	SFPIADD	L1, L0, 0, 2
**	SFPMOV	L0, L9, 0	# LV:L0
**	SFPENCC	3, 10
**	SFPSTORE	L0, 0, 0, 0
**	ret
*/

void addi () {
  auto a = __builtin_rvtt_sfpload (nullptr, 0, 0, 0, 0, 0);
  auto one = __builtin_rvtt_sfpreadlreg (10);
  auto zero = __builtin_rvtt_sfpreadlreg (9);

  __builtin_rvtt_sfppushc (0);

  __builtin_rvtt_sfpiadd_i (nullptr, a, 10, 0, 0, 0);
  a = __builtin_rvtt_sfpassign_lv (a, zero);

  __builtin_rvtt_sfppopc (0);
  __builtin_rvtt_sfpstore (nullptr, a, 0, 0, 0, 0, 0);
}
/*
**_Z4addiv:
**	SFPLOAD	L0, 0, 0, 0
**	SFPIADD	L1, L0, 10, 1
**	SFPMOV	L0, L9, 0	# LV:L0
**	SFPENCC	3, 10
**	SFPSTORE	L0, 0, 0, 0
**	ret
*/

void addi (unsigned val) {
  auto a = __builtin_rvtt_sfpload (nullptr, 0, 0, 0, 0, 0);
  auto one = __builtin_rvtt_sfpreadlreg (10);
  auto zero = __builtin_rvtt_sfpreadlreg (9);

  __builtin_rvtt_sfppushc (0);

  __builtin_rvtt_sfpiadd_i (iptr, a, val, 0, 0, 0);
  a = __builtin_rvtt_sfpassign_lv (a, zero);

  __builtin_rvtt_sfppopc (0);
  __builtin_rvtt_sfpstore (nullptr, a, 0, 0, 0, 0, 0);
}

/*
**_Z4addij:
**	SFPLOAD	L0, 0, 0, 0
**	zext.h	a4,a0
**	li	a5, 1897005056	# 1:71120000
**	add	a4,a4,a5
**	lui	a5,%hi\(iptr\)
**	addi	a5,a5,%lo\(iptr\)
**	sw	a4, 0\(a5\)	# 1:SFPLOADI	L1, -, 2
**	srli	a0,a0,16
**	li	a4, 1897398272	# 2:71180000
**	add	a0,a0,a4
**	sw	a0, 0\(a5\)	# 2:SFPLOADI	L1, -, 8	# LV:L1
**	SFPMOV	L2, L0, 2
**	SFPIADD	L2, L1, 0, 0
**	SFPMOV	L0, L9, 0	# LV:L0
**	SFPENCC	3, 10
**	SFPSTORE	L0, 0, 0, 0
**	ret
*/

void exexp () {
  auto a = __builtin_rvtt_sfpload (nullptr, 0, 0, 0, 0, 0);
  auto one = __builtin_rvtt_sfpreadlreg (10);
  auto zero = __builtin_rvtt_sfpreadlreg (9);

  __builtin_rvtt_sfppushc (0);

  __builtin_rvtt_sfpexexp (a, 2);
  a = __builtin_rvtt_sfpassign_lv (a, zero);

  __builtin_rvtt_sfppopc (0);
  __builtin_rvtt_sfpstore (nullptr, a, 0, 0, 0, 0, 0);
}
/*
**_Z5exexpv:
**	SFPLOAD	L0, 0, 0, 0
**	SFPEXEXP	L1, L0, 2
**	SFPMOV	L0, L9, 0	# LV:L0
**	SFPENCC	3, 10
**	SFPSTORE	L0, 0, 0, 0
**	ret
*/

void lz () {
  auto a = __builtin_rvtt_sfpload (nullptr, 0, 0, 0, 0, 0);
  auto one = __builtin_rvtt_sfpreadlreg (10);
  auto zero = __builtin_rvtt_sfpreadlreg (9);

  __builtin_rvtt_sfppushc (0);

  __builtin_rvtt_sfplz (a, 2);
  a = __builtin_rvtt_sfpassign_lv (a, zero);

  __builtin_rvtt_sfppopc (0);
  __builtin_rvtt_sfpstore (nullptr, a, 0, 0, 0, 0, 0);
}
/*
**_Z2lzv:
**	SFPLOAD	L0, 0, 0, 0
**	SFPLZ	L1, L0, 2
**	SFPMOV	L0, L9, 0	# LV:L0
**	SFPENCC	3, 10
**	SFPSTORE	L0, 0, 0, 0
**	ret
*/
