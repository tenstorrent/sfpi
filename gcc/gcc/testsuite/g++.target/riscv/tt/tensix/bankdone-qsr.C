// { dg-options "-mcpu=tt-qsr32-tensix -fno-exceptions -fno-rtti -O2" }
// { dg-final { check-function-bodies "**" "" } }

void foo1() {
  __builtin_rvtt_sfpbankdone(1, 1, 1);
}
/*
**_Z4foo1v:
**	SFPBANKDONE	1, 1, 1
**	ret
*/

void foo2() {
  __builtin_rvtt_sfpbankdone(1, 0, 0);
}
/*
**_Z4foo2v:
**	SFPBANKDONE	1, 0, 0
**	ret
*/

void foo3() {
  __builtin_rvtt_sfpbankdone(0, 1, 0);
}
/*
**_Z4foo3v:
**	SFPBANKDONE	0, 1, 0
**	ret
*/

void foo4() {
  __builtin_rvtt_sfpbankdone(0, 0, 1);
}
/*
**_Z4foo4v:
**	SFPBANKDONE	0, 0, 1
**	ret
*/
