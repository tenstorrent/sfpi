// { dg-options "-mcpu=tt-bh-tensix -O2 -fno-exceptions -fno-rtti" }
// { dg-final { check-function-bodies "**" "" } }

void sfpiadd_1 () {
  auto a = __builtin_rvtt_sfpreadlreg (0);
  auto b = __builtin_rvtt_sfpreadlreg (1);
  auto c = __builtin_rvtt_sfpreadlreg (2);

  auto res = __builtin_rvtt_sfpiadd_v_lv (b, b, a, 4);
  res = __builtin_rvtt_sfpiadd_v_lv (c, c, res, 6);
  __builtin_rvtt_sfpwritelreg (res, 3);
}
/*
**_Z9sfpiadd_1v:
**	# READ L0
**	# READ L1
**	# READ L2
**	SFPMOV	L3, L2, 2
**	SFPIADD	L1, L0, 0, 4	# LV:L1
**	SFPIADD	L3, L1, 0, 6	# LV:L3
**	# WRITE L3
**	ret
*/

void sfpiadd_2 () {
  auto a = __builtin_rvtt_sfpreadlreg (0);
  auto b = __builtin_rvtt_sfpreadlreg (1);
  auto c = __builtin_rvtt_sfpreadlreg (2);

  auto res = __builtin_rvtt_sfpiadd_v_lv (b, a, b, 4);
  res = __builtin_rvtt_sfpiadd_v_lv (c, res, c, 6);
  __builtin_rvtt_sfpwritelreg (res, 3);
}
/*
**_Z9sfpiadd_2v:
**	# READ L0
**	# READ L1
**	# READ L2
**	SFPIADD	L1, L0, 0, 4	# LV:L1
**	SFPMOV	L3, L2, 2
**	SFPMOV	L3, L1, 0	# LV:L3
**	SFPIADD	L3, L2, 0, 6	# LV:L3
**	# WRITE L3
**	ret
*/

void sfpand_or_xor_1 () {
  auto a = __builtin_rvtt_sfpreadlreg (0);
  auto b = __builtin_rvtt_sfpreadlreg (1);
  auto c = __builtin_rvtt_sfpreadlreg (2);
  auto d = __builtin_rvtt_sfpreadlreg (3);

  auto res = __builtin_rvtt_sfpand_lv (b, a, b);
  res = __builtin_rvtt_sfpor_lv (c, res, c);
  res = __builtin_rvtt_sfpxor_lv (d, res, d);
  __builtin_rvtt_sfpwritelreg (res, 4);
}
/*
**_Z15sfpand_or_xor_1v:
**	# READ L0
**	# READ L1
**	# READ L2
**	# READ L3
**	SFPMOV	L4, L3, 2
**	SFPAND	L1, L0, L1, 1	# LV:L1
**	SFPOR	L2, L1, L2, 1	# LV:L2
**	SFPXOR	L4, L2	# LV:L4
**	# WRITE L4
**	ret
*/

void sfpand_or_xor_2 () {
  auto a = __builtin_rvtt_sfpreadlreg (0);
  auto b = __builtin_rvtt_sfpreadlreg (1);
  auto c = __builtin_rvtt_sfpreadlreg (2);
  auto d = __builtin_rvtt_sfpreadlreg (3);

  auto res = __builtin_rvtt_sfpand_lv (b, b, a);
  res = __builtin_rvtt_sfpor_lv (c, c, res);
  res = __builtin_rvtt_sfpxor_lv (d, d, res);
  __builtin_rvtt_sfpwritelreg (res, 4);
}
/*
**_Z15sfpand_or_xor_2v:
**	# READ L0
**	# READ L1
**	# READ L2
**	# READ L3
**	SFPMOV	L4, L3, 2
**	SFPAND	L1, L1, L0, 1	# LV:L1
**	SFPOR	L2, L2, L1, 1	# LV:L2
**	SFPXOR	L4, L2	# LV:L4
**	# WRITE L4
**	ret
*/
