// { dg-options "-mcpu=tt-wh-tensix -O2 -I [SFPI]/include -fno-exceptions -fno-rtti" }
// { dg-final { check-function-bodies "**" "" } }

namespace ckernel{
    unsigned long *instrn_buffer;
}
#include <sfpi.h>

using namespace sfpi;

template<unsigned ADDR>
void load_imm () {
  vFloat r = (__builtin_rvtt_sfpload) (nullptr, ADDR, 0, 0, 7, 3);
  l_reg[LRegs::LReg3] = r;
}
template void load_imm<0> ();
/*
**_Z8load_immILj0EEvv:
**	SFPLOAD	L3, 0, 7, 3
**	# WRITE L3
**	ret
*/

template void load_imm<0x3fff> ();
/*
**_Z8load_immILj16383EEvv:
**	SFPLOAD	L3, 16383, 7, 3
**	# WRITE L3
**	ret
*/

void load_var (unsigned addr) {
  vFloat r = (__builtin_rvtt_sfpload) (::ckernel::instrn_buffer, addr, 0, 0, 7, 3);
  l_reg[LRegs::LReg3] = r;
}
/*
**_Z8load_varj:
**	slli	a0,a0,18
**	lui	a4,%hi\(_ZN7ckernel13instrn_bufferE\)
**	li	a5, 1882701824	# 1:7037c000
**	lw	a4,%lo\(_ZN7ckernel13instrn_bufferE\)\(a4\)
**	srli	a0,a0,18
**	add	a0,a0,a5
**	sw	a0, 0\(a4\)	# 1:SFPLOAD	L3, -, 7, 3
**	# WRITE L3
**	ret
*/

template<unsigned ADDR>
void store_imm () {
  vFloat r = l_reg[LRegs::LReg3];
  (__builtin_rvtt_sfpstore) (nullptr, r.get (), ADDR, 0, 0, 7, 3);
}
template void store_imm<0> ();
/*
**_Z9store_immILj0EEvv:
**	# READ L3
**	SFPSTORE	L3, 0, 7, 3
**	ret
*/

template void store_imm<0x3fff> ();
/*
**_Z9store_immILj16383EEvv:
**	# READ L3
**	SFPSTORE	L3, 16383, 7, 3
**	ret
*/

void store_var (unsigned addr) {
  vFloat r = l_reg[LRegs::LReg3];
  (__builtin_rvtt_sfpstore) (::ckernel::instrn_buffer, r.get (), addr, 0, 0, 7, 3);
}
/*
**_Z9store_varj:
**	# READ L3
**	slli	a0,a0,18
**	lui	a4,%hi\(_ZN7ckernel13instrn_bufferE\)
**	li	a5, 1916256256	# 1:7237c000
**	lw	a4,%lo\(_ZN7ckernel13instrn_bufferE\)\(a4\)
**	srli	a0,a0,18
**	add	a0,a0,a5
**	sw	a0, 0\(a4\)	# 1:SFPSTORE	L3, -, 7, 3
**	ret
*/
