// { dg-options "-mcpu=tt-bh-tensix -fno-exceptions -fno-rtti -O2" }
// { dg-final { check-function-bodies "**" "" } }

void one () {
  auto zero = __builtin_rvtt_sfpxloadi (0, 0, 0, 0, 31);
  auto neg7ff = __builtin_rvtt_sfpxloadi (0, -0x7ff, 0, 0, 31);
  auto neg800 = __builtin_rvtt_sfpxloadi (0, -0x800, 0, 0, 31);
  auto pos7ff = __builtin_rvtt_sfpxloadi (0, 0x7ff, 0, 0, 31);
  auto pos800 = __builtin_rvtt_sfpxloadi (0, 0x800, 0, 0, 31);

  auto val = __builtin_rvtt_sfpreadlreg (0);

  val = __builtin_rvtt_sfpiadd_v (val, zero, 4);
  val = __builtin_rvtt_sfpiadd_v (val, neg7ff, 4);
  val = __builtin_rvtt_sfpiadd_v (val, neg800, 4);
  val = __builtin_rvtt_sfpiadd_v (val, pos7ff, 4);
  val = __builtin_rvtt_sfpiadd_v (val, pos800, 4);
  __builtin_rvtt_sfpwritelreg (val, 0);
}
/*
**_Z3onev:
**	SFPLOADI	L1, 2048, 2
**	# READ L0
**	SFPIADD	L0, L0, -2047, 5
**	SFPIADD	L0, L0, -2048, 5
**	SFPIADD	L0, L0, 2047, 5
**	SFPIADD	L0, L1, 0, 4
**	# WRITE L0
**	ret
*/

void two () {
  auto zero = __builtin_rvtt_sfpxloadi (0, 0, 0, 0, 31);
  auto neg7ff = __builtin_rvtt_sfpxloadi (0, -0x7ff, 0, 0, 31);
  auto neg800 = __builtin_rvtt_sfpxloadi (0, -0x800, 0, 0, 31);
  auto pos7ff = __builtin_rvtt_sfpxloadi (0, 0x7ff, 0, 0, 31);
  auto pos800 = __builtin_rvtt_sfpxloadi (0, 0x800, 0, 0, 31);

  auto val = __builtin_rvtt_sfpreadlreg (0);

  val = __builtin_rvtt_sfpiadd_v (zero, val, 4);
  val = __builtin_rvtt_sfpiadd_v (neg7ff, val, 4);
  val = __builtin_rvtt_sfpiadd_v (neg800, val, 4);
  val = __builtin_rvtt_sfpiadd_v (pos7ff, val, 4);
  val = __builtin_rvtt_sfpiadd_v (pos800, val, 4);
  __builtin_rvtt_sfpwritelreg (val, 0);
}
/*
**_Z3twov:
**	SFPLOADI	L2, 2048, 2
**	# READ L0
**	SFPIADD	L1, L0, -2047, 5
**	SFPIADD	L1, L1, -2048, 5
**	SFPIADD	L1, L1, 2047, 5
**	SFPMOV	L0, L2, 2
**	SFPIADD	L0, L1, 0, 4
**	# WRITE L0
**	ret
*/

void three () {
  auto zero = __builtin_rvtt_sfpxloadi (0, 0, 0, 0, 31);
  auto neg7ff = __builtin_rvtt_sfpxloadi (0, -0x7ff, 0, 0, 31);
  auto neg800 = __builtin_rvtt_sfpxloadi (0, -0x800, 0, 0, 31);
  auto pos7ff = __builtin_rvtt_sfpxloadi (0, 0x7ff, 0, 0, 31);
  auto pos800 = __builtin_rvtt_sfpxloadi (0, 0x800, 0, 0, 31);

  auto val = __builtin_rvtt_sfpreadlreg (0);

  val = __builtin_rvtt_sfpiadd_v (zero, val, 6);
  val = __builtin_rvtt_sfpiadd_v (neg7ff, val, 6);
  val = __builtin_rvtt_sfpiadd_v (neg800, val, 6);
  val = __builtin_rvtt_sfpiadd_v (pos7ff, val, 6);
  val = __builtin_rvtt_sfpiadd_v (pos800, val, 6);
  __builtin_rvtt_sfpwritelreg (val, 0);
}
/*
**_Z5threev:
**	SFPLOADI	L1, 63488, 4
**	# READ L0
**	SFPIADD	L0, L0, 2047, 5
**	SFPIADD	L1, L0, 0, 6
**	SFPIADD	L0, L1, -2047, 5
**	SFPIADD	L0, L0, -2048, 5
**	# WRITE L0
**	ret
*/
