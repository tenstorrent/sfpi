// { dg-options "-mcpu=tt-bh-tensix -O2 -I [SFPI]/include -fno-exceptions -fno-rtti" }
// { dg-final { check-function-bodies "**" "" } }

namespace ckernel{
    unsigned long *instrn_buffer;
}
#include <sfpi.h>

using namespace sfpi;

namespace sexp {

void one () {
  vFloat y = 0.0f;
  vInt flags = dst_reg[0];

  v_if (flags < 0) {
    y = setexp(y, flags);
  } v_endif;

  dst_reg[0] = y;
}
/*
**_ZN4sexp3oneEv:
**	SFPLOAD	L1, 0, 4, 7
**	SFPSETCC	L1, 0, 0
**	SFPMOV	L0, L9, 2
**	SFPMOV	L0, L1, 0	# LV:L0
**	SFPSETEXP	L0, L9, 0, 0	# LV:L0
**	SFPENCC	3, 10
**	SFPSTORE	L0, 0, 0, 7
**	ret
*/

void two () {
  vFloat y = 0.0f;
  vFloat z = dst_reg[1];
  vInt flags = dst_reg[0];

  v_if (flags < 0) {
    y = setexp(z, flags);
  } v_endif;

  dst_reg[0] = y;
}
/*
**_ZN4sexp3twoEv:
**	SFPLOAD	L2, 2, 0, 7
**	SFPLOAD	L1, 0, 4, 7
**	SFPSETCC	L1, 0, 0
**	SFPMOV	L0, L9, 2
**	SFPMOV	L0, L1, 0	# LV:L0
**	SFPSETEXP	L0, L2, 0, 0	# LV:L0
**	SFPENCC	3, 10
**	SFPSTORE	L0, 0, 0, 7
**	ret
*/
}

namespace sadd {

void one () {
  vInt y = dst_reg[0];
  v_if (y < 0) {
    y = 0 - y;
  } v_endif;

  dst_reg[0] = y;
}
/*
**_ZN4sadd3oneEv:
**	SFPLOAD	L0, 0, 4, 7
**	SFPSETCC	L0, 0, 0
**	SFPIADD	L0, L9, 0, 6	# LV:L0
**	SFPENCC	3, 10
**	SFPSTORE	L0, 0, 4, 7
**	ret
*/

void two () {
  vInt y = dst_reg[0];
  vInt r = 0;
  v_if (y < 0) {
    r = 0 - y;
  } v_endif;

  dst_reg[0] = r;
}
/*
**_ZN4sadd3twoEv:
**	SFPLOAD	L1, 0, 4, 7
**	SFPSETCC	L1, 0, 0
**	SFPMOV	L0, L9, 2
**	SFPMOV	L0, L1, 0	# LV:L0
**	SFPIADD	L0, L9, 0, 6	# LV:L0
**	SFPENCC	3, 10
**	SFPSTORE	L0, 0, 4, 7
**	ret
*/

}
