// { dg-options "-mcpu=tt-wh-tensix -O2 -I [SFPI]/include -fno-exceptions -fno-rtti" }
// { dg-final { check-function-bodies "**" "" } }

namespace ckernel{
    unsigned long *instrn_buffer;
}
#include <sfpi.h>

using namespace sfpi;

void one () {
  vFloat a = dst_reg[0];

  dst_reg[1] = convert<vFloat16a> (a);
  dst_reg[2] = convert<vFloat16b> (a);
  dst_reg[3] = convert<vSMag16> (a);
  dst_reg[4] = convert<vUInt16> (a);
}
/*
**_Z3onev:
**	SFPLOAD	L0, 0, 0, 3
**	SFPSTOCHRND	L1, L9, L0, 0, 0, 1
**	SFPSTORE	L1, 2, 1, 3
**	SFPSTOCHRND	L1, L9, L0, 0, 1, 1
**	SFPSTORE	L1, 4, 2, 3
**	SFPSTOCHRND	L1, L9, L0, 0, 7, 1
**	SFPSTORE	L1, 6, 8, 3
**	SFPSTOCHRND	L0, L9, L0, 0, 6, 1
**	SFPSTORE	L0, 8, 6, 3
**	ret
*/

void two () {
  vInt a = dst_reg[0];

  dst_reg[3] = convert<vSMag> (a);
}
/*
**_Z3twov:
**	SFPLOAD	L0, 0, 12, 3
**	SFPSETCC	L0, 0, 0
**	SFPSETSGN	L0, L0, 0, 1	# LV:L0
**	SFPIADD	L0, L9, 0, 6	# LV:L0
**	SFPENCC	3, 10
**	SFPSTORE	L0, 6, 4, 3
**	ret
*/

void three () {
  vSMag a = dst_reg[0];

  dst_reg[3] = as<vUInt> (convert<vInt> (a));
  dst_reg[4] = convert<vFloat> (a);
}
/*
**_Z5threev:
**	SFPLOAD	L0, 0, 4, 3
**	SFPSETCC	L0, 0, 0
**	SFPMOV	L1, L0, 2
**	SFPSETSGN	L1, L0, 0, 1	# LV:L1
**	SFPIADD	L1, L9, 0, 6	# LV:L1
**	SFPENCC	3, 10
**	SFPSTORE	L1, 6, 4, 3
**	SFPCAST	L0, L0, 1
**	SFPSTORE	L0, 8, 0, 3
**	ret
*/
