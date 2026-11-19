// { dg-options "-mcpu=tt-bh-tensix -O2 -fno-exceptions -fno-rtti" }
// { dg-final { check-function-bodies "**" "" } }

void foo ()
{
  asm ("ebreak");
}
/*
**_Z3foov:
**	ebreak
**	nop
**	nop
**	nop
**	nop
**	nop
**	nop
**	nop
**	nop
**	ret
*/

void bar ()
{
  int b = 0;
  asm ("ebreak" :: "r" (b));
}
/*
**_Z3barv:
**	li	a5,0
**	ebreak
**	nop
**	nop
**	nop
**	nop
**	nop
**	nop
**	nop
**	nop
**	ret
*/

void baz ()
{
  int b;
  asm volatile ("ebreak" : "=r" (b));
}
/*
**_Z3bazv:
**	ebreak
**	nop
**	nop
**	nop
**	nop
**	nop
**	nop
**	nop
**	nop
**	ret
*/
