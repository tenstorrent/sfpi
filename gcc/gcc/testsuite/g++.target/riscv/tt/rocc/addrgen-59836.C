// { dg-do-compile }
// { dg-additional-options "-mcpu=tt-qsr64-rocc -O2" }
// { dg-final { check-function-bodies "**" "" } }

long hang () {
  long a = __builtin_riscv_ttrocc_addrgen_peek_src (0);
  long b = __builtin_riscv_ttrocc_addrgen_peek_dest (0);
  long c = __builtin_riscv_ttrocc_addrgen_pop_x_src (0, 1);
  long d = __builtin_riscv_ttrocc_addrgen_pop_x_dest (0, 1);
  return a ^ b ^ c ^ d;
}
/*
**_Z4hangv:
**	tt.rocc.nop
**	tt.rocc.addrgen_peek_src	a5,0
**	tt.rocc.nop
**	tt.rocc.addrgen_peek_dest	a3,0
**	li	a0,1
**	tt.rocc.nop
**	tt.rocc.addrgen_pop_x_src	a4,0,a0
**	tt.rocc.nop
**	tt.rocc.addrgen_pop_x_dest	a0,0,a0
**	xor	a5,a5,a3
**	xor	a5,a5,a4
**	xor	a0,a5,a0
**	ret
*/

long hang2 () {
  __builtin_riscv_ttrocc_addrgen_push_src (0);
  return __builtin_riscv_ttrocc_addrgen_pop_x_src (0, 1);
}
/*
**_Z5hang2v:
**	tt.rocc.addrgen_push_src	0
**	li	a0,1
**	tt.rocc.nop
**	tt.rocc.addrgen_pop_x_src	a0,0,a0
**	ret
*/

long barrier() {
  __builtin_riscv_ttrocc_nop ();
  return __builtin_riscv_ttrocc_addrgen_peek_src (0);
}
/*
**_Z7barrierv:
**	tt.rocc.nop
**	tt.rocc.addrgen_peek_src	a0,0
**	ret
*/

void ret() {
  __builtin_riscv_ttrocc_addrgen_push_src (0);
}
/*
**_Z3retv:
**	tt.rocc.addrgen_push_src	0
**	tt.rocc.nop
**	ret
*/

extern void foo ();

long call() {
  __builtin_riscv_ttrocc_nop ();
  foo ();
  return __builtin_riscv_ttrocc_addrgen_peek_src (0);
}
/*
**_Z4callv:
**	addi	sp,sp,-16
**	sd	ra,8\(sp\)
**	tt.rocc.nop
**	call	_Z3foov
**	tt.rocc.nop
**	tt.rocc.addrgen_peek_src	a0,0
**	ld	ra,8\(sp\)
**	addi	sp,sp,16
**	jr	ra
*/

void tail() {
  __builtin_riscv_ttrocc_addrgen_push_src (0);
  foo ();
}
/*
**_Z4tailv:
**	tt.rocc.addrgen_push_src	0
**	tt.rocc.nop
**	tail	_Z3foov
*/

void block(int ix) {
  __builtin_riscv_ttrocc_addrgen_push_src (0);
  if (ix)
    __builtin_riscv_ttrocc_addrgen_push_dest (0);
#pragma GCC unroll 0
  while (ix--)
    __builtin_riscv_ttrocc_addrgen_push_src (1);
}
/*
**_Z5blocki:
**	tt.rocc.addrgen_push_src	0
**	beq	a0,zero,.L[0-9]*
**	tt.rocc.nop
**	tt.rocc.addrgen_push_dest	0
**	addiw	a0,a0,-1
**	li	a5,-1
**	tt.rocc.nop
**	tt.rocc.addrgen_push_src	1
**	addiw	a0,a0,-1
**	bne	a0,a5,.L[0-9]*
**	tt.rocc.nop
**	ret
*/
