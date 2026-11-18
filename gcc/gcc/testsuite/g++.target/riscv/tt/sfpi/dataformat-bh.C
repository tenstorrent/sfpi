// { dg-options "-mcpu=tt-bh-tensix -O2 -I [SFPI]/include -fno-exceptions -fno-rtti" }
// { dg-final { check-function-bodies "**" "" } }

namespace ckernel{
    unsigned long *instrn_buffer;
}
#include <sfpi.h>
using namespace sfpi;


template<typename T>
void load_store () {
  T val = dst_reg[0];
  dst_reg[1] = val;
}

template void load_store<vFloat>();
/*
**_Z10load_storeIN4sfpi6vFloatEEvv:
**	SFPLOAD	L0, 0, 0, 7
**	SFPSTORE	L0, 2, 0, 7
**	ret
*/

template void load_store<vFloat16a>();
/*
**_Z10load_storeIN4sfpi5impl_7vNarrowINS0_6vFloatENS0_9sFloat16aEEEEvv:
**	SFPLOAD	L0, 0, 1, 7
**	SFPSTORE	L0, 2, 1, 7
**	ret
*/

template void load_store<vFloat16b>();
/*
**_Z10load_storeIN4sfpi5impl_7vNarrowINS0_6vFloatENS0_9sFloat16bEEEEvv:
**	SFPLOAD	L0, 0, 2, 7
**	SFPSTORE	L0, 2, 2, 7
**	ret
*/
