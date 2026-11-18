// { dg-options "-mcpu=tt-bh-tensix -fno-exceptions -fno-rtti -O2 -fno-shrink-wrap" }
// { dg-final { check-function-bodies "**" "" } }

extern unsigned long volatile iptr[1];

void foo (unsigned i) {
  auto a = __builtin_rvtt_sfpreadlreg (0);
  auto b = __builtin_rvtt_sfpreadlreg (1);
  auto c = __builtin_rvtt_sfpreadlreg (2);
  auto d = __builtin_rvtt_sfpreadlreg (3);
  auto r = __builtin_rvtt_sfpshft2_subvec_copy4 (a, b, c, d, 1);
  auto r0 = __builtin_rvtt_sfpselect4 (r, 0);
  auto r1 = __builtin_rvtt_sfpselect4 (r, 1);
  auto r2 = __builtin_rvtt_sfpselect4 (r, 2);
  auto r3 = __builtin_rvtt_sfpselect4 (r, 3);
  __builtin_rvtt_sfpwritelreg (r0, 0);
  __builtin_rvtt_sfpwritelreg (r1, 1);
  __builtin_rvtt_sfpwritelreg (r2, 2);
  __builtin_rvtt_sfpwritelreg (r3, 3);
}
/*
**_Z3fooj:
**	# READ L0
**	# READ L1
**	# READ L2
**	SFPMOV	L4, L2, 2
**	# READ L3
**	SFPMOV	L2, L1, 2
**	SFPMOV	L1, L0, 2
**	SFPMOV	L0, L3, 2
**	SFPMOV	L3, L4, 2
**	SFPSHFT2	L0, L0, 0, 1
**	# WRITE L0
**	# WRITE L1
**	# WRITE L2
**	# WRITE L3
**	ret
*/

void setexp (unsigned i) {
  auto a = __builtin_rvtt_sfpreadlreg (2);
  auto b = __builtin_rvtt_sfpsetexp_i (nullptr, a, 0, 0, 0, 0);
  __builtin_rvtt_sfpwritelreg (b, 1);
  a = __builtin_rvtt_sfpreadlreg (2);
  b = __builtin_rvtt_sfpsetexp_i (iptr, a, i, 0, 0, 0);
  __builtin_rvtt_sfpwritelreg (b, 1);
}
/*
**_Z6setexpj:
**	# READ L2
**	SFPSETEXP	L1, L2, 0, 1
**	# WRITE L1
**	# READ L2
**	andi	a0,a0,255
**	li	a5, 2181038609	# 1:82000211
**	slli	a0,a0,12
**	add	a0,a0,a5
**	lui	a5,%hi\(iptr\)
**	sw	a0, %lo\(iptr\)\(a5\)	# 1:SFPSETEXP	L1, L2, -, 1
**	# WRITE L1
**	ret
*/

void setsgn (unsigned i) {
  auto a = __builtin_rvtt_sfpreadlreg (2);
  auto b = __builtin_rvtt_sfpsetsgn_i (nullptr, a, 0, 0, 0, 0);
  __builtin_rvtt_sfpwritelreg (b, 6);
  a = __builtin_rvtt_sfpreadlreg (2);
  b = __builtin_rvtt_sfpsetsgn_i (iptr, a, i, 0, 0, 0);
  __builtin_rvtt_sfpwritelreg (b, 6);
}
/*
**_Z6setsgnj:
**	# READ L2
**	SFPSETSGN	L6, L2, 0, 1
**	# WRITE L6
**	# READ L2
**	andi	a0,a0,1
**	li	a5, 2298479201	# 1:89000261
**	slli	a0,a0,12
**	add	a0,a0,a5
**	lui	a5,%hi\(iptr\)
**	sw	a0, %lo\(iptr\)\(a5\)	# 1:SFPSETSGN	L6, L2, -, 1
**	# WRITE L6
**	ret
*/

void divp2 (unsigned i) {
  auto a = __builtin_rvtt_sfpreadlreg (2);
  auto b = __builtin_rvtt_sfpdivp2 (nullptr, a, 0, 0, 0, 0);
  __builtin_rvtt_sfpwritelreg (b, 6);
  a = __builtin_rvtt_sfpreadlreg (2);
  b = __builtin_rvtt_sfpdivp2 (iptr, a, i, 0, 0, 0);
  __builtin_rvtt_sfpwritelreg (b, 6);
}
/*
**_Z5divp2j:
**	# READ L2
**	SFPDIVP2	L6, L2, 0, 0
**	# WRITE L6
**	# READ L2
**	slli	a0,a0,12
**	li	a5,16773120
**	and	a0,a0,a5
**	li	a5, 1979712096	# 1:76000260
**	add	a0,a0,a5
**	lui	a5,%hi\(iptr\)
**	sw	a0, %lo\(iptr\)\(a5\)	# 1:SFPDIVP2	L6, L2, -, 0
**	# WRITE L6
**	ret
*/

void stochrnd (unsigned i) {
  auto a = __builtin_rvtt_sfpreadlreg (2);
  auto b = __builtin_rvtt_sfpstochrnd_descale_i (nullptr, a, 0, 0, 0, 4, 0);
  __builtin_rvtt_sfpwritelreg (b, 6);
  a = __builtin_rvtt_sfpreadlreg (2);
  b = __builtin_rvtt_sfpstochrnd_descale_i (iptr, a, i, 0, 0, 4, 0);
  __builtin_rvtt_sfpwritelreg (b, 6);
}
/*
**_Z8stochrndj:
**	# READ L2
**	SFPSTOCHRND	L6, L9, L2, 0, 12, 0
**	# WRITE L6
**	# READ L2
**	andi	a0,a0,31
**	li	a5, 2382402156	# 1:8e00926c
**	slli	a0,a0,16
**	add	a0,a0,a5
**	lui	a5,%hi\(iptr\)
**	sw	a0, %lo\(iptr\)\(a5\)	# 1:SFPSTOCHRND	L6, L9, L2, -, 12, 0
**	# WRITE L6
**	ret
*/
