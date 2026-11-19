// { dg-options "-mcpu=tt-bh-tensix -O2 -I [SFPI]/include -fno-exceptions -fno-rtti" }
// { dg-final { check-function-bodies "**" "" } }

// There was an LZ-SETCC peephole, but it has no knowledge of intermediate CC
// setting and so borked.

namespace ckernel{
    unsigned long *instrn_buffer;
}
#include <sfpi.h>

using namespace sfpi;

void nan () {
  vFloat v = l_reg[LRegs::LReg0];
  vInt r = 0;

  v_if (is_nan (v)) r = 1; v_endif;
  l_reg[LRegs::LReg1] = r;
}
/*
**_Z3nanv:
**	# READ L0
**	SFPEXEXP	L1, L0, 1
**	SFPIADD	L1, L1, -255, 9
**	SFPEXMAN	L0, L0, 1
**	SFPSETCC	L0, 0, 2
**	SFPMOV	L1, L9, 2
**	SFPLOADI	L1, 1, 2	# LV:L1
**	SFPENCC	3, 10
**	# WRITE L1
**	ret
*/

void finite () {
  vFloat v = l_reg[LRegs::LReg0];
  vInt r = 0;

  v_if (is_finite (v)) r = 1; v_endif;
  l_reg[LRegs::LReg1] = r;
}
/*
**_Z6finitev:
**	# READ L0
**	SFPEXEXP	L0, L0, 1
**	SFPIADD	L0, L0, -255, 1
**	SFPMOV	L1, L9, 2
**	SFPLOADI	L1, 1, 2	# LV:L1
**	SFPENCC	3, 10
**	# WRITE L1
**	ret
*/

void normal () {
  vFloat v = l_reg[LRegs::LReg0];
  vInt r = 0;

  v_if (is_normal (v)) r = 1; v_endif;
  l_reg[LRegs::LReg1] = r;
}
/*
**_Z6normalv:
**	# READ L0
**	SFPEXEXP	L0, L0, 1
**	SFPIADD	L1, L0, -255, 1
**	SFPSETCC	L0, 0, 2
**	SFPMOV	L1, L9, 2
**	SFPLOADI	L1, 1, 2	# LV:L1
**	SFPENCC	3, 10
**	# WRITE L1
**	ret
*/

void subnormal () {
  vFloat v = l_reg[LRegs::LReg0];
  vInt r = 0;

  v_if (is_subnormal (v)) r = 1; v_endif;
  l_reg[LRegs::LReg1] = r;
}
/*
**_Z9subnormalv:
**	# READ L0
**	SFPEXEXP	L1, L0, 1
**	SFPSETCC	L1, 0, 6
**	SFPEXMAN	L0, L0, 1
**	SFPSETCC	L0, 0, 2
**	SFPMOV	L1, L9, 2
**	SFPLOADI	L1, 1, 2	# LV:L1
**	SFPENCC	3, 10
**	# WRITE L1
**	ret
*/

void zero () {
  vFloat v = l_reg[LRegs::LReg0];
  vInt r = 0;

  v_if (is_zero (v)) r = 1; v_endif;
  l_reg[LRegs::LReg1] = r;
}
/*
**_Z4zerov:
**	# READ L0
**	SFPSHFT	L0, L0, 1, 5
**	SFPSETCC	L0, 0, 6
**	SFPMOV	L1, L9, 2
**	SFPLOADI	L1, 1, 2	# LV:L1
**	SFPENCC	3, 10
**	# WRITE L1
**	ret
*/

void inf () {
  vFloat v = l_reg[LRegs::LReg0];
  vInt r = 0;

  v_if (is_inf (v)) r = 1; v_endif;
  l_reg[LRegs::LReg1] = r;
}
/*
**_Z3infv:
**	# READ L0
**	SFPEXEXP	L1, L0, 1
**	SFPIADD	L1, L1, -255, 9
**	SFPEXMAN	L0, L0, 1
**	SFPSETCC	L0, 0, 6
**	SFPMOV	L1, L9, 2
**	SFPLOADI	L1, 1, 2	# LV:L1
**	SFPENCC	3, 10
**	# WRITE L1
**	ret
*/

void pos () {
  vFloat v = l_reg[LRegs::LReg0];
  vInt r = 0;

  v_if (is_pos (v)) r = 1; v_endif;
  l_reg[LRegs::LReg1] = r;
}
/*
**_Z3posv:
**	# READ L0
**	SFPLZ	L0, L0, 0
**	SFPSETCC	L0, 0, 2
**	SFPMOV	L1, L9, 2
**	SFPLOADI	L1, 1, 2	# LV:L1
**	SFPENCC	3, 10
**	# WRITE L1
**	ret
*/

void neg () {
  vFloat v = l_reg[LRegs::LReg0];
  vInt r = 0;

  v_if (is_neg (v)) r = 1; v_endif;
  l_reg[LRegs::LReg1] = r;
}
/*
**_Z3negv:
**	# READ L0
**	SFPLZ	L0, L0, 0
**	SFPSETCC	L0, 0, 6
**	SFPMOV	L1, L9, 2
**	SFPLOADI	L1, 1, 2	# LV:L1
**	SFPENCC	3, 10
**	# WRITE L1
**	ret
*/

void pos_inf  () {
  vFloat v = l_reg[LRegs::LReg0];
  vInt r = 0;

  v_if (is_pos (v) && is_inf (v)) r = 1; v_endif;
  l_reg[LRegs::LReg1] = r;
}
/*
**_Z7pos_infv:
**	# READ L0
**	SFPLZ	L1, L0, 0
**	SFPSETCC	L1, 0, 2
**	SFPEXEXP	L1, L0, 1
**	SFPIADD	L1, L1, -255, 9
**	SFPEXMAN	L0, L0, 1
**	SFPSETCC	L0, 0, 6
**	SFPMOV	L1, L9, 2
**	SFPLOADI	L1, 1, 2	# LV:L1
**	SFPENCC	3, 10
**	# WRITE L1
**	ret
*/

void neg_inf () {
  vFloat v = l_reg[LRegs::LReg0];
  vInt r = 0;

  v_if (is_neg (v) && is_inf (v)) r = 1; v_endif;
  l_reg[LRegs::LReg1] = r;
}
/*
**_Z7neg_infv:
**	# READ L0
**	SFPLZ	L1, L0, 0
**	SFPSETCC	L1, 0, 6
**	SFPEXEXP	L1, L0, 1
**	SFPIADD	L1, L1, -255, 9
**	SFPEXMAN	L0, L0, 1
**	SFPSETCC	L0, 0, 6
**	SFPMOV	L1, L9, 2
**	SFPLOADI	L1, 1, 2	# LV:L1
**	SFPENCC	3, 10
**	# WRITE L1
**	ret
*/

void borked ()
{
  vFloat in = l_reg[LRegs::LReg0];
    sfpi::vInt exp     = sfpi::exexp(in);
    sfpi::vInt man     = sfpi::exman(in);
    sfpi::vInt pos     = sfpi::lz(sfpi::as<sfpi::vUInt>(in));
    sfpi::vFloat out   = 0.0f;
    v_if (pos != 0 && exp == 128 && man == 0)
    {
        out = 1.0f;
    }
    v_endif;
    l_reg[LRegs::LReg1] = out;
}
/*
**_Z6borkedv:
**	# READ L0
**	SFPEXEXP	L1, L0, 0
**	SFPEXMAN	L2, L0, 1
**	SFPLZ	L0, L0, 0
**	SFPSETCC	L0, 0, 2
**	SFPIADD	L0, L1, -128, 5
**	SFPSETCC	L0, 0, 6
**	SFPSETCC	L2, 0, 6
**	SFPMOV	L1, L9, 2
**	SFPMOV	L1, L10, 0	# LV:L1
**	SFPENCC	3, 10
**	# WRITE L1
**	ret
*/
