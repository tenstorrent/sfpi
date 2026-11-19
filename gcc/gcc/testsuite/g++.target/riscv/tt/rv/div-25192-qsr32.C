// { dg-options "-mcpu=tt-qsr32 -fno-exceptions -fno-rtti -O2" }
// { dg-final { check-function-bodies "**" "" } }

unsigned udiv3(unsigned n)  { return n / 3; }
/*
**_Z5udiv3j:
**	li	a5,3
**	divu	a0,a0,a5
**	ret
*/

unsigned udiv7(unsigned n)  { return n / 7; }
/*
**_Z5udiv7j:
**	li	a5,7
**	divu	a0,a0,a5
**	ret
*/

unsigned udiv10(unsigned n) { return n / 10; }
/*
**_Z6udiv10j:
**	li	a5,10
**	divu	a0,a0,a5
**	ret
*/

unsigned udiv100(unsigned n){ return n / 100; }
/*
**_Z7udiv100j:
**	li	a5,100
**	divu	a0,a0,a5
**	ret
*/

int sdiv3(int n)  { return n / 3; }
/*
**_Z5sdiv3i:
**	li	a5,3
**	div	a0,a0,a5
**	ret
*/

int sdiv7(int n)  { return n / 7; }
/*
**_Z5sdiv7i:
**	li	a5,7
**	div	a0,a0,a5
**	ret
*/

unsigned umod7(unsigned n) { return n % 7; }
/*
**_Z5umod7j:
**	li	a5,7
**	remu	a0,a0,a5
**	ret
*/

int smod7(int n)           { return n % 7; }
/*
**_Z5smod7i:
**	li	a5,7
**	rem	a0,a0,a5
**	ret
*/
