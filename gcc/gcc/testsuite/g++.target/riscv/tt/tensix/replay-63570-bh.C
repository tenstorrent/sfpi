// { dg-options "-mcpu=tt-bh-tensix -O2 -fno-exceptions -fno-rtti" }
// { dg-final { check-function-bodies "**" "" } }

extern long unsigned iptr[];

void one (unsigned i, unsigned j)
{
  __builtin_rvtt_ttreplay (iptr, i, j, 0, 0, 0, 0);
  __builtin_rvtt_ttreplay (iptr, i, 1, 0, 0, 0, 0);
  __builtin_rvtt_ttreplay (iptr, 0, j, 0, 0, 0, 0);
}
/*
**_Z3onejj:
**	andi	a0,a0,31
**	slli	a0,a0,14
**	slli	a1,a1,4
**	li	a5, 67108864	# 1:4000000
**	andi	a1,a1,1008
**	add	a5,a5,a0
**	lui	a4,%hi\(iptr\)
**	add	a5,a5,a1
**	addi	a4,a4,%lo\(iptr\)
**	sw	a5, 0\(a4\)	# 1:TTREPLAY	-, -, 0, 0
**	li	a5, 67108880	# 2:4000010
**	add	a5,a5,a0
**	sw	a5, 0\(a4\)	# 2:TTREPLAY	-, 1, 0, 0
**	li	a5, 67108864	# 3:4000000
**	add	a5,a5,a1
**	sw	a5, 0\(a4\)	# 3:TTREPLAY	0, -, 0, 0
**	ret
*/
