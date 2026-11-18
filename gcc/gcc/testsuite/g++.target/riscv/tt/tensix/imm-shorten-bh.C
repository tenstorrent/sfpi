// { dg-options "-mcpu=tt-bh-tensix -fno-exceptions -fno-rtti -O2" }
// { dg-final { check-function-bodies "**" "" } }

extern volatile unsigned long iptr[];

namespace lohi {
void foo () {
  auto a = __builtin_rvtt_sfploadi (0, 0x1234, 0, 0, 2);
  __builtin_rvtt_sfpwritelreg (a, 0);
  auto b = __builtin_rvtt_sfploadi_lv (0, a, 0x3f80, 0, 0, 8);
  __builtin_rvtt_sfpwritelreg (b, 0);
}
/*
**_ZN4lohi3fooEv:
**	SFPLOADI	L0, 4660, 2
**	# WRITE L0
**	SFPLOADI	L0, 16256, 8	# LV:L0
**	# WRITE L0
**	ret
*/

void bar () {
  auto a = __builtin_rvtt_sfploadi (0, 0x1234, 0, 0, 2);
  __builtin_rvtt_sfpwritelreg (a, 0);
  auto b = __builtin_rvtt_sfploadi_lv (0, a, 0, 0, 0, 8);
  __builtin_rvtt_sfpwritelreg (b, 0);
}
/*
**_ZN4lohi3barEv:
**	SFPLOADI	L0, 4660, 2
**	# WRITE L0
**	SFPLOADI	L0, 4660, 2
**	# WRITE L0
**	ret
*/
}

namespace hilo {
void foo () {
  auto a = __builtin_rvtt_sfploadi (0, 0x3f80, 0, 0, 0);
  __builtin_rvtt_sfpwritelreg (a, 0);
  auto b = __builtin_rvtt_sfploadi_lv (0, a, 0x1234, 0, 0, 10);
  __builtin_rvtt_sfpwritelreg (b, 0);
}
/*
**_ZN4hilo3fooEv:
**	SFPMOV	L0, L10, 2
**	# WRITE L0
**	SFPLOADI	L0, 4660, 10	# LV:L0
**	# WRITE L0
**	ret
*/

void bar () {
  auto a = __builtin_rvtt_sfploadi (0, 0x0000000, 0, 0, 0);
  __builtin_rvtt_sfpwritelreg (a, 0);
  auto b = __builtin_rvtt_sfploadi_lv (0, a, 0x1234, 0, 0, 10);
  __builtin_rvtt_sfpwritelreg (b, 0);
}
/*
**_ZN4hilo3barEv:
**	SFPMOV	L0, L9, 2
**	# WRITE L0
**	SFPLOADI	L0, 4660, 2
**	# WRITE L0
**	ret
*/
}
