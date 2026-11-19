// { dg-options "-mcpu=tt-bh-tensix -fno-exceptions -fno-rtti -O2" }
// { dg-final { check-function-bodies "**" "" } }

unsigned long volatile *ptr;

void foo ()
{
  auto a = __builtin_rvtt_sfpload (nullptr, 0, 0, 0, 0, 0);
  auto b = __builtin_rvtt_sfpxloadi (nullptr, 0x123456, 0, 0, -32);
  auto c = __builtin_rvtt_sfpmul (a, b, 0);
  __builtin_rvtt_sfpstore (nullptr, c, 0, 0, 0, 0, 0);
}
/*
**_Z3foov:
**	SFPLOAD	L0, 0, 0, 0
**	SFPLOADI	L1, 13398, 2
**	SFPLOADI	L1, 18, 8	# LV:L1
**	SFPMUL	L0, L0, L1, 0
**	SFPSTORE	L0, 0, 0, 0
**	ret
*/

void bar ()
{
  auto a = __builtin_rvtt_sfpload (nullptr, 0, 0, 0, 0, 0);
  auto b = __builtin_rvtt_sfpxloadi (nullptr, 0x48000000, 0, 0, -32);
  auto c = __builtin_rvtt_sfpmul (a, b, 0);
  __builtin_rvtt_sfpstore (nullptr, c, 0, 0, 0, 0, 0);
}
/*
**_Z3barv:
**	SFPLOAD	L0, 0, 0, 0
**	SFPMULI	L0, 18432, 0
**	SFPSTORE	L0, 0, 0, 0
**	ret
*/

void baz ()
{
  auto a = __builtin_rvtt_sfpload (nullptr, 0, 0, 0, 0, 0);
  auto b = __builtin_rvtt_sfploadi (nullptr, 0x4800, 0, 0, 0);
  auto c = __builtin_rvtt_sfpmul (a, b, 0);
  __builtin_rvtt_sfpstore (nullptr, c, 0, 0, 0, 0, 0);
}
/*
**_Z3bazv:
**	SFPLOAD	L0, 0, 0, 0
**	SFPMULI	L0, 18432, 0
**	SFPSTORE	L0, 0, 0, 0
**	ret
*/

void toto (unsigned j)
{
  auto a = __builtin_rvtt_sfpload (nullptr, 0, 0, 0, 0, 0);
  auto b = __builtin_rvtt_sfploadi (ptr, j, 0, 0, 0);
  auto c = __builtin_rvtt_sfpmul (a, b, 0);
  __builtin_rvtt_sfpstore (nullptr, c, 0, 0, 0, 0, 0);
}
/*
**_Z4totoj:
**	SFPLOAD	L0, 0, 0, 0
**	slli	a0,a0,16
**	lui	a4,%hi\(ptr\)
**	li	a5, 1946157056	# 1:74000000
**	lw	a4,%lo\(ptr\)\(a4\)
**	srli	a0,a0,8
**	add	a0,a0,a5
**	sw	a0, 0\(a4\)	# 1:SFPMULI	L0, -, 0
**	SFPSTORE	L0, 0, 0, 0
**	ret
*/
