// { dg-options "-mcpu=tt-bh-tensix -O2 -I [SFPI]/include -fno-exceptions -fno-rtti" }
// { dg-final { check-function-bodies "**" "" } }

namespace ckernel{
    unsigned long *instrn_buffer;
}
#include <sfpi.h>

using namespace sfpi;

void one () {
  vInt a = dst_reg[0];
  vMag b = dst_reg[1];

  v_if (a < b) {
    a = 0;
  } v_endif;

  dst_reg[0] = a;
}
/*
**_Z3onev:
**	SFPLOAD	L0, 0, 4, 7
**	SFPLOAD	L1, 2, 4, 7
**	SFPIADD	L1, L0, 0, 2
**	SFPMOV	L0, L9, 0	# LV:L0
**	SFPENCC	3, 10
**	SFPSTORE	L0, 0, 4, 7
**	ret
*/
