// { dg-options "-mcpu=tt-bh-tensix -O2 -fno-exceptions -fno-rtti" }
// { dg-final { check-function-bodies "**" "" } }

extern int a;
extern int b;

void foo () {
  b = a;
  __builtin_rvtt_sfpnop ();
  b = a;
}
/*
**_Z3foov:
**	lui	a5,%hi\(a\)
**	lw	a4,%lo\(a\)\(a5\)
**	lui	a5,%hi\(b\)
**	sw	a4,%lo\(b\)\(a5\)
**	SFPNOP
**	ret
*/
