// { dg-options "-mcpu=tt-qsr64-rocc -O2 -fno-exceptions -fno-rtti" }
// { dg-final { check-function-bodies "**" "" } }

// Minimal test to make sure not broken

void frob () {
  __builtin_riscv_ttrocc_cmdbuf_reset(0);
}
/*
**_Z4frobv:
**	tt.rocc.cmdbuf_reset	0
**	ret
*/
