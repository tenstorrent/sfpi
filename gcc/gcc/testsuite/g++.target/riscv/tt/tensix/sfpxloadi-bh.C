// { dg-options "-mcpu=tt-bh-tensix -fno-exceptions -fno-rtti -O2" }
// { dg-final { check-function-bodies "**" "" } }

unsigned long volatile *buf;

void cst ()
{
  auto c = __builtin_rvtt_sfpxloadi (buf, 0x12345678, 0, 0, 31);
  __builtin_rvtt_sfpstore (0, c, 0, 0, 0, 0, 0);
}
/*
**_Z3cstv:
**	SFPLOADI	L0, 22136, 2
**	SFPLOADI	L0, 4660, 8	# LV:L0
**	SFPSTORE	L0, 0, 0, 0
**	ret
*/

void var (int x)
{
  auto v = __builtin_rvtt_sfpxloadi (buf, x, 0, 0, 31);
  __builtin_rvtt_sfpstore (0, v, 0, 0, 0, 0, 0);
}
/*
**_Z3vari:
**	lui	a3,%hi\(buf\)
**	zext.h	a5,a0
**	li	a4, 1895956480	# 1:71020000
**	lw	a3,%lo\(buf\)\(a3\)
**	add	a5,a5,a4
**	sw	a5, 0\(a3\)	# 1:SFPLOADI	L0, -, 2
**	srli	a0,a0,16
**	li	a5, 1896349696	# 2:71080000
**	add	a0,a0,a5
**	sw	a0, 0\(a3\)	# 2:SFPLOADI	L0, -, 8	# LV:L0
**	SFPSTORE	L0, 0, 0, 0
**	ret
*/
