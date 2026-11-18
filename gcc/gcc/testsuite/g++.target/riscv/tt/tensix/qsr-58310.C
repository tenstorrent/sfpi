// { dg-options "-mcpu=tt-qsr32-tensix -fno-exceptions -fno-rtti -O2 -fno-shrink-wrap" }
// { dg-final { check-function-bodies "**" "" } }

extern unsigned long volatile iptr[1];

void foo (unsigned ix) {
  auto a = __builtin_rvtt_sfpreadlreg (0);
  __builtin_rvtt_sfpstoresrcs (nullptr, a, 0, 0, 0, 3, 7, 0);
  a = __builtin_rvtt_sfploadsrcs_lv (nullptr, a, 2, 0, 0, 3, 4, 1);
  auto b = __builtin_rvtt_sfploadsrcs (nullptr, 0, 0, 0, 3, 7, 0);
  __builtin_rvtt_sfpwritelreg (a, 0);
  __builtin_rvtt_sfpwritelreg (b, 1);
}
/*
**_Z3fooj:
**	# READ L0
**	SFPSTORE	L0, 0, 3, 7, 1, 0
**	SFPLOAD	L0, 2, 3, 4, 1, 1	# LV:L0
**	SFPLOAD	L1, 0, 3, 7, 1, 0
**	# WRITE L0
**	# WRITE L1
**	ret
*/
