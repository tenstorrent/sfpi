// { dg-options "-mcpu=tt-qsr32-tensix -fno-exceptions -fno-rtti -O2 -fno-shrink-wrap" }
// { dg-final { check-function-bodies "**" "" } }

extern unsigned long volatile iptr[1];

void foo (unsigned ix) {
  auto a = __builtin_rvtt_sfploadsrcs (nullptr, 0, 0, 0, 3, 4, 1);
  __builtin_rvtt_sfpwritelreg (a, 0);
  auto b = __builtin_rvtt_sfploadsrcs (iptr,   ix, 0, 0, 3, 4, 1);
  __builtin_rvtt_sfpwritelreg (b, 0);
}
/*
**_Z3fooj:
**	SFPLOAD	L0, 0, 3, 4, 1, 1
**	# WRITE L0
**	andi	a0,a0,1023
**	li	a5, 1879280640	# 1:70038c00
**	add	a0,a0,a5
**	lui	a5,%hi\(iptr\)
**	sw	a0, %lo\(iptr\)\(a5\)	# 1:SFPLOAD	L0, -, 3, 4, 1, 1
**	# WRITE L0
**	ret
*/

void bar (unsigned ix) {
  auto a = __builtin_rvtt_sfpreadlreg (0);
  __builtin_rvtt_sfpstoresrcs (nullptr, a, 0, 0, 0, 3, 4, 1);
  auto b = __builtin_rvtt_sfpreadlreg (0);
  __builtin_rvtt_sfpstoresrcs (iptr,    b,ix, 0, 0, 3, 4, 1);
}
/*
**_Z3barj:
**	# READ L0
**	SFPSTORE	L0, 0, 3, 4, 1, 1
**	# READ L0
**	andi	a0,a0,1023
**	li	a5, 1912835072	# 1:72038c00
**	add	a0,a0,a5
**	lui	a5,%hi\(iptr\)
**	sw	a0, %lo\(iptr\)\(a5\)	# 1:SFPSTORE	L0, -, 3, 4, 1, 1
**	ret
*/
