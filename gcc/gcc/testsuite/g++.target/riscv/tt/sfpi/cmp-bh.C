// { dg-options "-mcpu=tt-bh-tensix -O2 -I [SFPI]/include -fno-exceptions -fno-rtti" }
// { dg-final { check-function-bodies "**" "" } }

namespace ckernel{
    unsigned long *instrn_buffer;
}
#include <sfpi.h>

using namespace sfpi;

void one () {
  vSMag a = dst_reg[0];
  vSMag b = dst_reg[1];
  vInt r = 0;

  v_if (a == b)
    r = 1;
  v_elseif (a == 2)
    r = 2;
  v_elseif (b == -3)
    r = 3;
  v_endif;

  dst_reg[2] = r;
}

/*
**_Z3onev:
**	SFPLOAD	L1, 0, 4, 7
**	SFPLOAD	L2, 2, 4, 7
**	SFPLE	L2, L1, 0, 1
**	SFPLE	L1, L2, 0, 1
**	SFPMOV	L0, L9, 2
**	SFPLOADI	L0, 1, 2	# LV:L0
**	SFPCOMPC
**	SFPPUSHC	0
**	SFPIADD	L1, L1, -2, 5
**	SFPSETCC	L1, 0, 6
**	SFPLOADI	L0, 2, 2	# LV:L0
**	SFPCOMPC
**	SFPLOADI	L1, 3, 2
**	SFPLOADI	L1, 32768, 8	# LV:L1
**	SFPLE	L1, L2, 0, 1
**	SFPLE	L2, L1, 0, 1
**	SFPLOADI	L0, 3, 2	# LV:L0
**	SFPPOPC	0
**	SFPENCC	3, 10
**	SFPSTORE	L0, 4, 4, 7
**	ret
*/

void two () {
  vSMag a = dst_reg[0];
  vSMag b = dst_reg[1];
  vInt r = 0;

  v_if (a != b)
    r = 1;
  v_elseif (a != 2)
    r = 2;
  v_elseif (b != -3)
    r = 3;
  v_endif;

  dst_reg[2] = r;
}
/*
**_Z3twov:
**	SFPLOAD	L1, 0, 4, 7
**	SFPLOAD	L2, 2, 4, 7
**	SFPMOV	L0, L2, 2
**	SFPIADD	L0, L1, 0, 6
**	SFPSETCC	L0, 0, 2
**	SFPMOV	L0, L9, 2
**	SFPLOADI	L0, 1, 2	# LV:L0
**	SFPCOMPC
**	SFPPUSHC	0
**	SFPIADD	L1, L1, -2, 5
**	SFPSETCC	L1, 0, 2
**	SFPLOADI	L0, 2, 2	# LV:L0
**	SFPCOMPC
**	SFPLOADI	L1, 3, 2
**	SFPLOADI	L1, 32768, 8	# LV:L1
**	SFPIADD	L1, L2, 0, 6
**	SFPSETCC	L1, 0, 2
**	SFPLOADI	L0, 3, 2	# LV:L0
**	SFPPOPC	0
**	SFPENCC	3, 10
**	SFPSTORE	L0, 4, 4, 7
**	ret
*/

void fp16b () {
  vFloat a = dst_reg[0];
  vInt b = dst_reg[1];

  v_if (b < 0x7f80000) {
    a = 0;
  } v_endif;
  

  dst_reg[0] = a;
}
/*
**_Z5fp16bv:
**	SFPLOAD	L0, 0, 0, 7
**	SFPLOAD	L2, 2, 4, 7
**	SFPLOADI	L1, 2040, 0
**	SFPIADD	L1, L2, 0, 2
**	SFPMOV	L0, L9, 0	# LV:L0
**	SFPENCC	3, 10
**	SFPSTORE	L0, 0, 0, 7
**	ret
*/

void gt () {
  vFloat a = dst_reg[0];
  vInt b = dst_reg[1];
  vInt c = dst_reg[2];

  v_if (b > 0) {
    a = 0;
  } v_elseif (c > 0) {
    a = 1.0f;
  } v_endif;
  

  dst_reg[0] = a;
}
/*
**_Z2gtv:
**	SFPLOAD	L0, 0, 0, 7
**	SFPLOAD	L2, 2, 4, 7
**	SFPLOAD	L1, 4, 4, 7
**	SFPIADD	L2, L9, 0, 2
**	SFPMOV	L0, L9, 0	# LV:L0
**	SFPCOMPC
**	SFPIADD	L1, L9, 0, 2
**	SFPMOV	L0, L10, 0	# LV:L0
**	SFPENCC	3, 10
**	SFPSTORE	L0, 0, 0, 7
**	ret
*/

void le () {
  vFloat a = dst_reg[0];
  vInt b = dst_reg[1];
  vInt c = dst_reg[2];

  v_if (b <= 0) {
    a = 0;
  } v_elseif (c <= 0) {
    a = 1.0f;
  } v_endif;
  
  dst_reg[0] = a;
}
/*
**_Z2lev:
**	SFPLOAD	L0, 0, 0, 7
**	SFPLOAD	L2, 2, 4, 7
**	SFPLOAD	L1, 4, 4, 7
**	SFPIADD	L2, L9, 0, 10
**	SFPMOV	L0, L9, 0	# LV:L0
**	SFPCOMPC
**	SFPIADD	L1, L9, 0, 10
**	SFPMOV	L0, L10, 0	# LV:L0
**	SFPENCC	3, 10
**	SFPSTORE	L0, 0, 0, 7
**	ret
*/

void eqoreq () {
  vFloat a = dst_reg[0];
  vInt b = dst_reg[1];
  vInt c = dst_reg[2];

  v_if (b == 0 || c == 0) {
    a = 0;
  } v_elseif (b == 0 || c == 0) {
    a = 1.0f;
  } v_endif;
  
  dst_reg[0] = a;
}
/*
**_Z6eqoreqv:
**	SFPLOAD	L0, 0, 0, 7
**	SFPLOAD	L2, 2, 4, 7
**	SFPLOAD	L1, 4, 4, 7
**	SFPSETCC	L2, 0, 2
**	SFPSETCC	L1, 0, 2
**	SFPCOMPC
**	SFPMOV	L0, L9, 0	# LV:L0
**	SFPCOMPC
**	SFPPUSHC	0
**	SFPSETCC	L2, 0, 2
**	SFPSETCC	L1, 0, 2
**	SFPCOMPC
**	SFPMOV	L0, L10, 0	# LV:L0
**	SFPPOPC	0
**	SFPENCC	3, 10
**	SFPSTORE	L0, 0, 0, 7
**	ret
*/
