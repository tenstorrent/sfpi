// { dg-options "-mcpu=tt-bh-tensix -O2 -fno-exceptions -fno-rtti" }
// { dg-final { check-function-bodies "**" "" } }

extern volatile unsigned long ibuf[];

void foo (unsigned v) {
  __builtin_rvtt_ttinsn (nullptr, 0x12345678, 0, 0, 0);
  __builtin_rvtt_ttinsn (ibuf, v, 0, 0, 0);
}
/*
**_Z3fooj:
**	.ttinsn	305419896
**	lui	a5,%hi\(ibuf\)
**	sw	a0,%lo\(ibuf\)\(a5\)
**	ret
*/
