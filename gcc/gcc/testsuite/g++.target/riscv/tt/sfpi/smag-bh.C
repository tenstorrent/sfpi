// { dg-options "-mcpu=tt-bh-tensix -O2 -I [SFPI]/include -fno-exceptions -fno-rtti" }
// { dg-final { check-function-bodies "**" "" } }

namespace ckernel{
    unsigned long *instrn_buffer;
}
#include <sfpi.h>

using namespace sfpi;

void one () {
  vSMag a = dst_reg[0];
  dst_reg[1] = a;

  vSMag b = dst_reg[2].mode<DataLayout::I32>();
  dst_reg[3].mode<DataLayout::I32>() = b;
}
/*
**_Z3onev:
**	SFPLOAD	L0, 0, 4, 7
**	SFPSTORE	L0, 2, 4, 7
**	SFPLOAD	L0, 4, 4, 7
**	SFPCAST	L0, L0, 3
**	SFPSETCC	L0, 0, 0
**	SFPSETSGN	L0, L0, 0, 1	# LV:L0
**	SFPIADD	L0, L9, 0, 6	# LV:L0
**	SFPENCC	3, 10
**	SFPSTORE	L0, 6, 4, 7
**	ret
*/

void two () {
  // FIXME: Doesn't convert.
  vInt a = dst_reg[0];
  dst_reg[1] = a;

  vInt b = dst_reg[2].mode<DataLayout::SM32>();
  dst_reg[3].mode<DataLayout::SM32>() = b;

  vInt c = dst_reg[4].mode<DataLayout::I32>();
  dst_reg[5].mode<DataLayout::I32>() = c;
}

/*
**_Z3twov:
**	SFPLOAD	L0, 0, 4, 7
**	SFPSTORE	L0, 2, 4, 7
**	SFPLOAD	L0, 4, 4, 7
**	SFPSETCC	L0, 0, 0
**	SFPSETSGN	L0, L0, 0, 1	# LV:L0
**	SFPIADD	L0, L9, 0, 6	# LV:L0
**	SFPENCC	3, 10
**	SFPCAST	L0, L0, 3
**	SFPSTORE	L0, 6, 4, 7
**	SFPLOAD	L0, 8, 4, 7
**	SFPSTORE	L0, 10, 4, 7
**	ret
*/
