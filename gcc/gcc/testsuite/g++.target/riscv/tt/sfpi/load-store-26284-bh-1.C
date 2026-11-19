// { dg-options "-mcpu=tt-bh-tensix -O2 -I [SFPI]/include -fno-exceptions -fno-rtti" }
// { dg-final { check-function-bodies "**" "" } }

namespace ckernel{
    unsigned long *instrn_buffer;
}
#include <sfpi.h>

using namespace sfpi;

template<unsigned ADDR>
void load_imm () {
  vFloat r = (__builtin_rvtt_sfpload) (nullptr, ADDR, 0, 0, 3, 7);
  l_reg[LRegs::LReg3] = r;
}
template void load_imm<0> ();
/*
**_Z8load_immILj0EEvv:
**	SFPLOAD	L3, 0, 3, 7
**	# WRITE L3
**	ret
*/

template void load_imm<0x1fff> ();
/*
**_Z8load_immILj8191EEvv:
**	SFPLOAD	L3, 8191, 3, 7
**	# WRITE L3
**	ret
*/

void load_var (unsigned addr) {
  vFloat r = (__builtin_rvtt_sfpload) (::ckernel::instrn_buffer, addr, 0, 0, 3, 7);
  l_reg[LRegs::LReg3] = r;
}
/*
**_Z8load_varj:
**	slli	a0,a0,19
**	lui	a4,%hi\(_ZN7ckernel13instrn_bufferE\)
**	li	a5, 1882447872	# 1:7033e000
**	lw	a4,%lo\(_ZN7ckernel13instrn_bufferE\)\(a4\)
**	srli	a0,a0,19
**	add	a0,a0,a5
**	sw	a0, 0\(a4\)	# 1:SFPLOAD	L3, -, 3, 7
**	# WRITE L3
**	ret
*/

template<unsigned ADDR>
void store_imm () {
  vFloat r = l_reg[LRegs::LReg3];
  (__builtin_rvtt_sfpstore) (nullptr, r.get (), ADDR, 0, 0, 3, 7);
}
template void store_imm<0> ();
/*
**_Z9store_immILj0EEvv:
**	# READ L3
**	SFPSTORE	L3, 0, 3, 7
**	ret
*/

template void store_imm<0x1fff> ();
/*
**_Z9store_immILj8191EEvv:
**	# READ L3
**	SFPSTORE	L3, 8191, 3, 7
**	ret
*/

void store_var (unsigned addr) {
    vFloat r = l_reg[LRegs::LReg3];
    (__builtin_rvtt_sfpstore) (::ckernel::instrn_buffer, r.get (), addr, 0, 0, 3, 7);
}
/*
**_Z9store_varj:
**	# READ L3
**	slli	a0,a0,19
**	lui	a4,%hi\(_ZN7ckernel13instrn_bufferE\)
**	li	a5, 1916002304	# 1:7233e000
**	lw	a4,%lo\(_ZN7ckernel13instrn_bufferE\)\(a4\)
**	srli	a0,a0,19
**	add	a0,a0,a5
**	sw	a0, 0\(a4\)	# 1:SFPSTORE	L3, -, 3, 7
**	ret
*/
