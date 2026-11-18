// { dg-do-compile }
// { dg-additional-options "-mcpu=tt-qsr64-rocc -O2" }
// { dg-final { check-function-bodies "**" "" } }

long src () {
  return __builtin_riscv_ttrocc_addrgen_pop_src (0);
}
/*
**_Z3srcv:
**	li	a0,1
**	tt.rocc.nop
**	tt.rocc.addrgen_pop_x_src	a0,0,a0
**	ret
*/
  
long dst () {
  return __builtin_riscv_ttrocc_addrgen_pop_dest (0);
}
/*
**_Z3dstv:
**	li	a0,1
**	tt.rocc.nop
**	tt.rocc.addrgen_pop_x_dest	a0,0,a0
**	ret
*/
