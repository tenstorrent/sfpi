// { dg-options "-mcpu=tt-bh-tensix -fno-exceptions -fno-rtti -O2" }
// { dg-final { check-function-bodies "**" "" } }

extern volatile unsigned long iptr[];

void muladd1 () {
  auto a = __builtin_rvtt_sfpreadlreg (0);
  auto b = __builtin_rvtt_sfpreadlreg (1);
  auto c = __builtin_rvtt_sfpreadlreg (2);
  
  auto p = __builtin_rvtt_sfpmul (a, b, 1);
  auto r = __builtin_rvtt_sfpadd (p, c, 2);

  __builtin_rvtt_sfpwritelreg (r, 3);
}
/*
**_Z7muladd1v:
**	# READ L0
**	# READ L1
**	# READ L2
**	SFPMAD	L3, L0, L1, L2, 3
**	# WRITE L3
**	ret
*/

void muladd2 () {
  auto a = __builtin_rvtt_sfpreadlreg (0);
  auto b = __builtin_rvtt_sfpreadlreg (1);
  auto c = __builtin_rvtt_sfpreadlreg (2);
  
  auto p = __builtin_rvtt_sfpmul (a, b, 1);
  auto r = __builtin_rvtt_sfpadd (c, p, 1);

  __builtin_rvtt_sfpwritelreg (r, 3);
}
/*
**_Z7muladd2v:
**	# READ L0
**	# READ L1
**	# READ L2
**	SFPMAD	L3, L0, L1, L2, 3
**	# WRITE L3
**	ret
*/

void muladd_lv1 () {
  auto a = __builtin_rvtt_sfpreadlreg (0);
  auto b = __builtin_rvtt_sfpreadlreg (1);
  auto c = __builtin_rvtt_sfpreadlreg (2);
  auto d = __builtin_rvtt_sfpreadlreg (3);
  auto e = __builtin_rvtt_sfpreadlreg (4);
  
  auto p = __builtin_rvtt_sfpmul_lv (d, a, b, 0);
  auto r = __builtin_rvtt_sfpadd_lv (e, p, c, 0);

  __builtin_rvtt_sfpwritelreg (r, 3);
}
/*
**_Z10muladd_lv1v:
**	# READ L0
**	# READ L1
**	# READ L2
**	# READ L3
**	# READ L4
**	SFPMOV	L3, L4, 2
**	SFPMAD	L3, L0, L1, L2, 0	# LV:L3
**	# WRITE L3
**	ret
*/

void muladd_lv2 () {
  auto a = __builtin_rvtt_sfpreadlreg (0);
  auto b = __builtin_rvtt_sfpreadlreg (1);
  auto c = __builtin_rvtt_sfpreadlreg (2);
  auto d = __builtin_rvtt_sfpreadlreg (3);
  
  auto p = __builtin_rvtt_sfpmul_lv (d, a, b, 0);
  auto r = __builtin_rvtt_sfpadd_lv (p, p, c, 0);

  __builtin_rvtt_sfpwritelreg (r, 3);
}
/*
**_Z10muladd_lv2v:
**	# READ L0
**	# READ L1
**	# READ L2
**	# READ L3
**	SFPMAD	L3, L0, L1, L2, 0	# LV:L3
**	# WRITE L3
**	ret
*/

void mulno1 () {
  auto a = __builtin_rvtt_sfpreadlreg (0);
  auto b = __builtin_rvtt_sfpreadlreg (1);
  
  auto p = __builtin_rvtt_sfpmul (a, b, 0);
  auto r = __builtin_rvtt_sfpadd (p, p, 0);

  __builtin_rvtt_sfpwritelreg (r, 3);
}
/*
**_Z6mulno1v:
**	# READ L0
**	# READ L1
**	SFPMUL	L3, L0, L1, 0
**	SFPADD	L3, L3, L3, 0
**	# WRITE L3
**	ret
*/

void mulno2 () {
  auto a = __builtin_rvtt_sfpreadlreg (0);
  auto b = __builtin_rvtt_sfpreadlreg (1);
  auto c = __builtin_rvtt_sfpreadlreg (2);
  
  auto p = __builtin_rvtt_sfpmul (a, b, 0);
  auto r = __builtin_rvtt_sfpadd_lv (p, p, c, 0);

  __builtin_rvtt_sfpwritelreg (r, 3);
}
/*
**_Z6mulno2v:
**	# READ L0
**	# READ L1
**	# READ L2
**	SFPMUL	L3, L0, L1, 0
**	SFPADD	L3, L3, L2, 0	# LV:L3
**	# WRITE L3
**	ret
*/

void muln1mul () {
  auto a = __builtin_rvtt_sfpreadlreg (0);
  auto b = __builtin_rvtt_sfpreadlreg (1);

  auto p = __builtin_rvtt_sfpmov (a, 1);
  auto r = __builtin_rvtt_sfpmul (p, b, 0);

  __builtin_rvtt_sfpwritelreg (r, 3);
}
/*
**_Z8muln1mulv:
**	# READ L0
**	# READ L1
**	SFPMUL	L3, L0, L1, 1
**	# WRITE L3
**	ret
*/

void muln1add () {
  auto a = __builtin_rvtt_sfpreadlreg (0);
  auto b = __builtin_rvtt_sfpreadlreg (1);

  auto p = __builtin_rvtt_sfpmov (a, 1);
  auto r = __builtin_rvtt_sfpadd (p, b, 0);

  __builtin_rvtt_sfpwritelreg (r, 3);
}
/*
**_Z8muln1addv:
**	# READ L0
**	# READ L1
**	SFPADD	L3, L0, L1, 1
**	# WRITE L3
**	ret
*/

void muln1muladd () {
  auto a = __builtin_rvtt_sfpreadlreg (0);
  auto b = __builtin_rvtt_sfpreadlreg (1);
  auto c = __builtin_rvtt_sfpreadlreg (2);

  auto p = __builtin_rvtt_sfpmov (a, 1);
  auto q = __builtin_rvtt_sfpmul (p, b, 0);
  auto r = __builtin_rvtt_sfpadd (q, c, 0);

  __builtin_rvtt_sfpwritelreg (r, 3);
}
/*
**_Z11muln1muladdv:
**	# READ L0
**	# READ L1
**	# READ L2
**	SFPMAD	L3, L0, L1, L2, 1
**	# WRITE L3
**	ret
*/

void muln1muladdmuln1 () {
  auto a = __builtin_rvtt_sfpreadlreg (0);
  auto b = __builtin_rvtt_sfpreadlreg (1);
  auto c = __builtin_rvtt_sfpreadlreg (2);

  auto p = __builtin_rvtt_sfpmov (a, 1);
  auto q = __builtin_rvtt_sfpmul (p, b, 0);
  auto r = __builtin_rvtt_sfpadd (q, c, 0);
  auto s = __builtin_rvtt_sfpmov (r, 1);

  __builtin_rvtt_sfpwritelreg (s, 3);
}
/*
**_Z16muln1muladdmuln1v:
**	# READ L0
**	# READ L1
**	# READ L2
**	SFPMAD	L3, L0, L1, L2, 2
**	# WRITE L3
**	ret
*/

void loadimul () {
  auto a = __builtin_rvtt_sfpreadlreg (0);
  auto c = __builtin_rvtt_sfploadi (nullptr, 0x1234, 0, 0, 0);
  auto r = __builtin_rvtt_sfpmul (a, c, 0);
  __builtin_rvtt_sfpwritelreg (r, 1);
}
/*
**_Z8loadimulv:
**	# READ L0
**	SFPMULI	L0, 4660, 0
**	SFPMOV	L1, L0, 2
**	# WRITE L1
**	ret
*/

void loadimula () {
  auto a = __builtin_rvtt_sfpreadlreg (0);
  auto c = __builtin_rvtt_sfploadi (nullptr, 0x1234, 0, 0, 0);
  auto r = __builtin_rvtt_sfpmul (a, c, 0);
  __builtin_rvtt_sfpwritelreg (r, 1);
  __builtin_rvtt_sfpwritelreg (c, 2);
}
/*
**_Z9loadimulav:
**	# READ L0
**	SFPLOADI	L2, 4660, 0
**	SFPMULI	L0, 4660, 0
**	SFPMOV	L1, L0, 2
**	# WRITE L1
**	# WRITE L2
**	ret
*/

void loadiadd () {
  auto a = __builtin_rvtt_sfpreadlreg (0);
  auto c = __builtin_rvtt_sfploadi (nullptr, 0x1234, 0, 0, 0);
  auto r = __builtin_rvtt_sfpadd (a, c, 0);
  __builtin_rvtt_sfpwritelreg (r, 1);
}
/*
**_Z8loadiaddv:
**	# READ L0
**	SFPADDI	L0, 4660, 0
**	SFPMOV	L1, L0, 2
**	# WRITE L1
**	ret
*/

void loadimul1 (unsigned v) {
  auto a = __builtin_rvtt_sfpreadlreg (0);
  auto c = __builtin_rvtt_sfploadi (iptr, v, 0, 0, 0);
  auto r = __builtin_rvtt_sfpmul (a, c, 0);
  __builtin_rvtt_sfpwritelreg (r, 1);
}
/*
**_Z9loadimul1j:
**	# READ L0
**	slli	a0,a0,16
**	li	a5, 1946157056	# 1:74000000
**	srli	a0,a0,8
**	add	a0,a0,a5
**	lui	a5,%hi\(iptr\)
**	sw	a0, %lo\(iptr\)\(a5\)	# 1:SFPMULI	L0, -, 0
**	SFPMOV	L1, L0, 2
**	# WRITE L1
**	ret
*/

void loadimul1a (unsigned v) {
  auto a = __builtin_rvtt_sfpreadlreg (0);
  auto c = __builtin_rvtt_sfploadi (iptr, v, 0, 0, 0);
  auto r = __builtin_rvtt_sfpmul (a, c, 0);
  __builtin_rvtt_sfpwritelreg (r, 1);
  __builtin_rvtt_sfpwritelreg (c, 2);
}
/*
**_Z10loadimul1aj:
**	# READ L0
**	zext.h	a0,a0
**	li	a5, 1897922560	# 1:71200000
**	add	a0,a0,a5
**	lui	a5,%hi\(iptr\)
**	sw	a0, %lo\(iptr\)\(a5\)	# 1:SFPLOADI	L2, -, 0
**	SFPMUL	L1, L0, L2, 0
**	# WRITE L1
**	# WRITE L2
**	ret
*/

void loadimul1b (unsigned v) {
  auto a = __builtin_rvtt_sfpreadlreg (0);
  auto b = __builtin_rvtt_sfpreadlreg (1);
  auto c = __builtin_rvtt_sfploadi (iptr, v, 0, 0, 0);
  auto r1 = __builtin_rvtt_sfpmul (a, c, 0);
  auto r2 = __builtin_rvtt_sfpmul (b, c, 0);
  __builtin_rvtt_sfpwritelreg (r1, 0);
  __builtin_rvtt_sfpwritelreg (r2, 1);
}
/*
**_Z10loadimul1bj:
**	# READ L0
**	# READ L1
**	slli	a0,a0,16
**	li	a4, 1946157056	# 1:74000000
**	srli	a0,a0,8
**	lui	a5,%hi\(iptr\)
**	add	a0,a0,a4
**	addi	a5,a5,%lo\(iptr\)
**	sw	a0, 0\(a5\)	# 1:SFPMULI	L0, -, 0
**	li	a4,16
**	xor	a4,a4,a0
**	sw	a4, 0\(a5\)	# 1:SFPMULI	L1, -, 0
**	# WRITE L0
**	# WRITE L1
**	ret
*/

void loadimul2 (unsigned v) {
  auto id = __builtin_rvtt_synth_opcode (1, 0);
  
  auto a = __builtin_rvtt_sfpreadlreg (0);
  auto c = __builtin_rvtt_sfploadi (iptr, v, id + (v & 0xffff), 1, 0);
  auto r = __builtin_rvtt_sfpmul (a, c, 0);
  __builtin_rvtt_sfpwritelreg (r, 1);

  auto d = __builtin_rvtt_sfploadi (iptr, v, id + (v & 0xffff), 1, 0);
  __builtin_rvtt_sfpwritelreg (d, 2);
}
/*
**_Z9loadimul2j:
**	li	a5, 1896873984	# 1:71100000
**	# READ L0
**	zext.h	a0,a0
**	add	a0,a0,a5
**	lui	a5,%hi\(iptr\)
**	addi	a5,a5,%lo\(iptr\)
**	sw	a0, 0\(a5\)	# 1:SFPLOADI	L1, -, 0
**	SFPMUL	L1, L0, L1, 0
**	# WRITE L1
**	li	a4,3145728
**	xor	a4,a4,a0
**	sw	a4, 0\(a5\)	# 1:SFPLOADI	L2, -, 0
**	# WRITE L2
**	ret
*/
