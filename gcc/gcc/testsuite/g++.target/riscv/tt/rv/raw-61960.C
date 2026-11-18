// { dg-do compile }
// { dg-additional-options "-mcpu=tt-wh -O2 -fno-exceptions" }

int frob (short *ptr, int *p)
{
  *ptr = 5;

  __asm__ ("boof" ::: "a0");

  return *p;
}
/*
**_Z4frobPsPi
**	li	a5,5
**	sh	a5,0\(a0\)
**	lhu	zero,0\(a0\)
**	boof
**	lw	a0,0\(a1\)
**	ret
*/
