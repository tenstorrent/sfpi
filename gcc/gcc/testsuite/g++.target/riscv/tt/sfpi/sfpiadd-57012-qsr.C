// { dg-options "-mcpu=tt-qsr32-tensix -O2 -I [SFPI]/include -fno-exceptions -fno-rtti" }
// { dg-final { check-function-bodies "**" "" } }

namespace ckernel{
    unsigned long *instrn_buffer;
}
#include <sfpi.h>

using namespace sfpi;

void orig () {
  vFloat y = l_reg[LRegs::LReg0];
  sfpi::vInt e = sfpi::exexp(y, sfpi::ExponentMode::Biased);

  v_block {
    sfpi::vInt e_lt_255 = (__builtin_rvtt_sfpiadd_i) (nullptr, e.get(), -255, 0, 0, sfpi::SFPIADD_MOD1_CC_LT0);
    y = sfpi::setexp(y, e);
    v_if(e_lt_255 < -254) {
      y = 0.0f;
    }
    v_endif;
  }
  v_endblock;

  l_reg[LRegs::LReg2] = y;
}
/*
**_Z4origv:
**	# READ L0
**	SFPEXEXP	L1, L0, 1
**	SFPIADD	L3, L1, -255, 1
**	SFPMOV	L2, L0, 2
**	SFPMOV	L2, L1, 0	# LV:L2
**	SFPSETEXP	L2, L0, 0, 0	# LV:L2
**	SFPLOADI	L0, 65282, 4
**	SFPGT	L0, L3, 0, 1
**	SFPMOV	L2, L9, 0	# LV:L2
**	SFPENCC	3, 10
**	# WRITE L2
**	ret
*/

void opt () {
  vFloat y = l_reg[LRegs::LReg0];
  sfpi::vInt e = sfpi::exexp(y, sfpi::ExponentMode::Biased);

  sfpi::vInt e_lt_255 = e - 255;
  v_if (e_lt_255 < 0) {
    y = sfpi::setexp(y, e);
    v_if(e_lt_255 < -254) {
      y = 0.0f;
    }
    v_endif;
  }
  v_endif;

  l_reg[LRegs::LReg2] = y;
}
/*
**_Z3optv:
**	# READ L0
**	SFPEXEXP	L1, L0, 1
**	SFPIADD	L3, L1, -255, 1
**	SFPMOV	L2, L0, 2
**	SFPMOV	L2, L1, 0	# LV:L2
**	SFPSETEXP	L2, L0, 0, 0	# LV:L2
**	SFPLOADI	L0, 65282, 4
**	SFPGT	L0, L3, 0, 1
**	SFPMOV	L2, L9, 0	# LV:L2
**	SFPENCC	3, 10
**	# WRITE L2
**	ret
*/

void better () {
  vFloat y = l_reg[LRegs::LReg0];
  sfpi::vInt e = sfpi::exexp(y, sfpi::ExponentMode::Biased);

  v_if (nearby (e < 255)) {
    y = sfpi::setexp(y, e);
    v_if (e == 0) {
      y = 0.0f;
    } v_endif;
  } v_endif;

  l_reg[LRegs::LReg2] = y;
}
/*
**_Z6betterv:
**	# READ L0
**	SFPEXEXP	L1, L0, 1
**	SFPIADD	L8, L1, -255, 1
**	SFPMOV	L2, L0, 2
**	SFPMOV	L2, L1, 0	# LV:L2
**	SFPSETEXP	L2, L0, 0, 0	# LV:L2
**	SFPSETCC	L1, 0, 6
**	SFPMOV	L2, L9, 0	# LV:L2
**	SFPENCC	3, 10
**	# WRITE L2
**	ret
*/
