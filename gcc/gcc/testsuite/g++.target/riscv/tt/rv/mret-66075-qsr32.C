// { dg-options "-mcpu=tt-qsr32 -fno-exceptions -fno-rtti -O2" }
// { dg-final { check-function-bodies "**" "" } }

// The mret erratum fix is on by default for tt-qsr32.

void __attribute__ ((interrupt ("machine"))) handler ()
{
}
/*
**_Z7handlerv:
**	#
**1:	mret; j 1b
*/
