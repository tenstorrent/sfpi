// { dg-options "-mcpu=tt-wh-tensix -O2 -I [SFPI]/include -fno-exceptions -fno-rtti" }
// { dg-final { check-function-bodies "**" "" } }

namespace ckernel{
    unsigned long *instrn_buffer;
}
#include <sfpi.h>

using namespace sfpi;

void negate () {
  auto a = vInt (1);
  auto b = -a;
  __builtin_rvtt_sfpwritelreg (b.get (), 1);

  auto c = ~a + 1;
  __builtin_rvtt_sfpwritelreg (c.get (), 2);

  auto d = 4u + c;
  __builtin_rvtt_sfpwritelreg (d.get (), 3);
}
/*
**_Z6negatev:
**	SFPLOADI	L2, 1, 2
**	SFPIADD	L1, L9, -1, 5
**	# WRITE L1
**	SFPNOT	L2, L2
**	SFPIADD	L2, L2, 1, 5
**	# WRITE L2
**	SFPIADD	L3, L2, 4, 5
**	# WRITE L3
**	ret
*/

void slength () {
  sfpi::vInt v = __builtin_rvtt_sfpreadlreg (0);
  sfpi::vUInt u = __builtin_rvtt_sfpreadlreg (7);

  // Mixed compares
  v_if (v == 0) {
    v = __builtin_rvtt_sfpreadlreg (1);
  } v_endif;

  v_if (v < 0) {
    v = __builtin_rvtt_sfpreadlreg (2);
  } v_endif;

  v_if (v > 0) {
    v = __builtin_rvtt_sfpreadlreg (3);
  } v_endif;

  v_if (v >= 0) {
    v = __builtin_rvtt_sfpreadlreg (4);
  } v_endif;

  v_if (u == 0) {
    u = __builtin_rvtt_sfpreadlreg (1);
  } v_endif;

  v_if (u < 0) {
    u = __builtin_rvtt_sfpreadlreg (2);
  } v_endif;

  v_if (u > 0) {
    u = __builtin_rvtt_sfpreadlreg (3);
  } v_endif;

  v_if (u >= 0) {
    u = __builtin_rvtt_sfpreadlreg (4);
  } v_endif;

  __builtin_rvtt_sfpwritelreg (v.get (), 0);
  __builtin_rvtt_sfpwritelreg (u.get (), 7);
}
/*
**_Z7slengthv:
**	# READ L0
**	# READ L7
**	SFPSETCC	L0, 0, 6
**	# READ L1
**	SFPMOV	L0, L1, 0	# LV:L0
**	SFPENCC	3, 10
**	SFPSETCC	L0, 0, 0
**	# READ L2
**	SFPMOV	L0, L2, 0	# LV:L0
**	SFPENCC	3, 10
**	SFPMOV	L1, L0, 2
**	SFPIADD	L1, L9, 0, 2
**	# READ L3
**	SFPMOV	L0, L3, 0	# LV:L0
**	SFPENCC	3, 10
**	SFPSETCC	L0, 0, 4
**	# READ L4
**	SFPMOV	L0, L4, 0	# LV:L0
**	SFPENCC	3, 10
**	SFPSETCC	L7, 0, 6
**	# READ L1
**	SFPMOV	L7, L1, 0	# LV:L7
**	SFPENCC	3, 10
**	SFPSETCC	L7, 0, 0
**	# READ L2
**	SFPMOV	L7, L2, 0	# LV:L7
**	SFPENCC	3, 10
**	SFPMOV	L1, L7, 2
**	SFPIADD	L1, L9, 0, 2
**	# READ L3
**	SFPMOV	L7, L3, 0	# LV:L7
**	SFPENCC	3, 10
**	SFPSETCC	L7, 0, 4
**	# READ L4
**	SFPMOV	L7, L4, 0	# LV:L7
**	SFPENCC	3, 10
**	# WRITE L0
**	# WRITE L7
**	ret
*/

void ulength () {
  sfpi::vInt v = __builtin_rvtt_sfpreadlreg (0);
  sfpi::vUInt u = __builtin_rvtt_sfpreadlreg (7);

  // Mixed compares
  v_if (v == 0x10) {
    v = __builtin_rvtt_sfpreadlreg (1);
  } v_endif;

  v_if (v < 0x10) {
    v = __builtin_rvtt_sfpreadlreg (2);
  } v_endif;

  v_if (v > 0x10) {
    v = __builtin_rvtt_sfpreadlreg (3);
  } v_endif;

  v_if (v >= 0x10) {
    v = __builtin_rvtt_sfpreadlreg (4);
  } v_endif;

  v_if (u == 0x10u) {
    u = __builtin_rvtt_sfpreadlreg (1);
  } v_endif;

  v_if (u < 0x10u) {
    u = __builtin_rvtt_sfpreadlreg (2);
  } v_endif;

  v_if (u > 0x10u) {
    u = __builtin_rvtt_sfpreadlreg (3);
  } v_endif;

  v_if (u >= 0x10u) {
    u = __builtin_rvtt_sfpreadlreg (4);
  } v_endif;

  __builtin_rvtt_sfpwritelreg (v.get (), 0);
  __builtin_rvtt_sfpwritelreg (u.get (), 7);
}
/*
**_Z7ulengthv:
**	# READ L0
**	# READ L7
**	SFPIADD	L1, L0, -16, 5
**	SFPSETCC	L1, 0, 6
**	# READ L1
**	SFPMOV	L0, L1, 0	# LV:L0
**	SFPENCC	3, 10
**	SFPIADD	L1, L0, -16, 1
**	# READ L2
**	SFPMOV	L0, L2, 0	# LV:L0
**	SFPENCC	3, 10
**	SFPLOADI	L1, 16, 2
**	SFPMOV	L2, L0, 2
**	SFPIADD	L2, L1, 0, 2
**	# READ L3
**	SFPMOV	L0, L3, 0	# LV:L0
**	SFPENCC	3, 10
**	SFPIADD	L1, L0, -16, 9
**	# READ L4
**	SFPMOV	L0, L4, 0	# LV:L0
**	SFPENCC	3, 10
**	SFPIADD	L1, L7, -16, 5
**	SFPSETCC	L1, 0, 6
**	# READ L1
**	SFPMOV	L7, L1, 0	# LV:L7
**	SFPENCC	3, 10
**	SFPIADD	L1, L7, -16, 1
**	# READ L2
**	SFPMOV	L7, L2, 0	# LV:L7
**	SFPENCC	3, 10
**	SFPLOADI	L1, 16, 2
**	SFPMOV	L2, L7, 2
**	SFPIADD	L2, L1, 0, 2
**	# READ L3
**	SFPMOV	L7, L3, 0	# LV:L7
**	SFPENCC	3, 10
**	SFPIADD	L1, L7, -16, 9
**	# READ L4
**	SFPMOV	L7, L4, 0	# LV:L7
**	SFPENCC	3, 10
**	# WRITE L0
**	# WRITE L7
**	ret
*/

void logic0 ()
{
  sfpi::vInt v = __builtin_rvtt_sfpreadlreg (0);
  sfpi::vUInt u = __builtin_rvtt_sfpreadlreg (7);

  __builtin_rvtt_sfpwritelreg ((v & 1).get (), 1);
  __builtin_rvtt_sfpwritelreg ((v | 2).get (), 2);
  __builtin_rvtt_sfpwritelreg ((v ^ 3).get (), 3);

  __builtin_rvtt_sfpwritelreg ((u & 1).get (), 4);
  __builtin_rvtt_sfpwritelreg ((u | 2).get (), 5);
  __builtin_rvtt_sfpwritelreg ((u ^ 3).get (), 6);
}
/*
**_Z6logic0v:
**	# READ L0
**	# READ L7
**	SFPLOADI	L2, 1, 2
**	SFPMOV	L1, L0, 2
**	SFPAND	L1, L2
**	# WRITE L1
**	SFPLOADI	L1, 2, 2
**	SFPMOV	L2, L0, 2
**	SFPOR	L2, L1
**	# WRITE L2
**	SFPLOADI	L1, 3, 2
**	SFPXOR	L0, L1
**	SFPMOV	L3, L0, 2
**	# WRITE L3
**	SFPLOADI	L0, 1, 2
**	SFPMOV	L4, L7, 2
**	SFPAND	L4, L0
**	# WRITE L4
**	SFPLOADI	L0, 2, 2
**	SFPMOV	L5, L7, 2
**	SFPOR	L5, L0
**	# WRITE L5
**	SFPLOADI	L0, 3, 2
**	SFPMOV	L6, L7, 2
**	SFPXOR	L6, L0
**	# WRITE L6
**	ret
*/

void logic1 ()
{
  sfpi::vInt v = __builtin_rvtt_sfpreadlreg (0);
  sfpi::vUInt u = __builtin_rvtt_sfpreadlreg (7);

  __builtin_rvtt_sfpwritelreg ((v & u).get (), 1);
  __builtin_rvtt_sfpwritelreg ((v | u).get (), 2);
  __builtin_rvtt_sfpwritelreg ((v ^ u).get (), 3);

  __builtin_rvtt_sfpwritelreg ((u & v).get (), 4);
  __builtin_rvtt_sfpwritelreg ((u | v).get (), 5);
  __builtin_rvtt_sfpwritelreg ((u ^ v).get (), 6);
}
/*
**_Z6logic1v:
**	# READ L0
**	# READ L7
**	SFPMOV	L1, L0, 2
**	SFPAND	L1, L7
**	# WRITE L1
**	SFPMOV	L2, L0, 2
**	SFPOR	L2, L7
**	# WRITE L2
**	SFPMOV	L3, L0, 2
**	SFPXOR	L3, L7
**	# WRITE L3
**	SFPMOV	L4, L7, 2
**	SFPAND	L4, L0
**	# WRITE L4
**	SFPMOV	L5, L7, 2
**	SFPOR	L5, L0
**	# WRITE L5
**	SFPMOV	L6, L7, 2
**	SFPXOR	L6, L0
**	# WRITE L6
**	ret
*/

void logic2 ()
{
  sfpi::vInt v = __builtin_rvtt_sfpreadlreg (0);
  sfpi::vUInt u = __builtin_rvtt_sfpreadlreg (7);

  __builtin_rvtt_sfpwritelreg ((v & 1u).get (), 1);
  __builtin_rvtt_sfpwritelreg ((v | 2u).get (), 2);
  __builtin_rvtt_sfpwritelreg ((v ^ 3u).get (), 3);
  __builtin_rvtt_sfpwritelreg ((u & 1u).get (), 4);
  __builtin_rvtt_sfpwritelreg ((u | 2u).get (), 5);
  __builtin_rvtt_sfpwritelreg ((u ^ 3u).get (), 6);

}
/*
**_Z6logic2v:
**	# READ L0
**	# READ L7
**	SFPLOADI	L2, 1, 2
**	SFPMOV	L1, L0, 2
**	SFPAND	L1, L2
**	# WRITE L1
**	SFPLOADI	L1, 2, 2
**	SFPMOV	L2, L0, 2
**	SFPOR	L2, L1
**	# WRITE L2
**	SFPLOADI	L1, 3, 2
**	SFPXOR	L0, L1
**	SFPMOV	L3, L0, 2
**	# WRITE L3
**	SFPLOADI	L0, 1, 2
**	SFPMOV	L4, L7, 2
**	SFPAND	L4, L0
**	# WRITE L4
**	SFPLOADI	L0, 2, 2
**	SFPMOV	L5, L7, 2
**	SFPOR	L5, L0
**	# WRITE L5
**	SFPLOADI	L0, 3, 2
**	SFPMOV	L6, L7, 2
**	SFPXOR	L6, L0
**	# WRITE L6
**	ret
*/
