// { dg-options "-mcpu=tt-wh -fno-exceptions -fno-rtti -O2" }
// { dg-final { check-function-bodies "**" "" } }

unsigned udiv3(unsigned n)  { return n / 3; }
/*
**_Z5udiv3j:
**	li	a5,-1431654400
**	addi	a5,a5,-1365
**	mulhu	a0,a0,a5
**	srli	a0,a0,1
**	ret
*/

unsigned udiv7(unsigned n)  { return n / 7; }
/*
**_Z5udiv7j:
**	li	a5,613568512
**	addi	a5,a5,-1755
**	mulhu	a5,a0,a5
**	sub	a0,a0,a5
**	srli	a0,a0,1
**	add	a0,a0,a5
**	srli	a0,a0,2
**	ret
*/

unsigned udiv10(unsigned n) { return n / 10; }
/*
**_Z6udiv10j:
**	li	a5,-858992640
**	addi	a5,a5,-819
**	mulhu	a0,a0,a5
**	srli	a0,a0,3
**	ret
*/

unsigned udiv100(unsigned n){ return n / 100; }
/*
**_Z7udiv100j:
**	li	a5,1374388224
**	addi	a5,a5,1311
**	mulhu	a0,a0,a5
**	srli	a0,a0,5
**	ret
*/

int sdiv3(int n)  { return n / 3; }
/*
**_Z5sdiv3i:
**	li	a5,1431654400
**	addi	a5,a5,1366
**	mulh	a5,a0,a5
**	srai	a0,a0,31
**	sub	a0,a5,a0
**	ret
*/

int sdiv7(int n)  { return n / 7; }
/*
**_Z5sdiv7i:
**	li	a5,-1840701440
**	addi	a5,a5,1171
**	mulh	a5,a0,a5
**	srai	a4,a0,31
**	add	a0,a0,a5
**	srai	a0,a0,2
**	sub	a0,a0,a4
**	ret
*/

unsigned umod7(unsigned n) { return n % 7; }
/*
**_Z5umod7j:
**	li	a4,613568512
**	addi	a4,a4,-1755
**	mulhu	a4,a0,a4
**	sub	a5,a0,a4
**	srli	a5,a5,1
**	add	a5,a5,a4
**	srli	a5,a5,2
**	slli	a4,a5,3
**	sub	a5,a4,a5
**	sub	a0,a0,a5
**	ret
*/

int smod7(int n)           { return n % 7; }
/*
**_Z5smod7i:
**	li	a5,-1840701440
**	addi	a5,a5,1171
**	mulh	a5,a0,a5
**	srai	a4,a0,31
**	add	a5,a0,a5
**	srai	a5,a5,2
**	sub	a5,a5,a4
**	slli	a4,a5,3
**	sub	a5,a4,a5
**	sub	a0,a0,a5
**	ret
*/
