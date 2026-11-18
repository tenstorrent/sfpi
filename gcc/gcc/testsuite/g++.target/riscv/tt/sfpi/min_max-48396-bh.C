// { dg-options "-mcpu=tt-bh-tensix -O2 -I [SFPI]/include -fno-exceptions -fno-rtti" }
// { dg-final { check-function-bodies "**" "" } }

namespace ckernel{
  extern unsigned long *instrn_buffer;
}
#include <sfpi.h>

using namespace sfpi;

void min_max_1 () {
  vFloat a = l_reg[LRegs::LReg0];
  vFloat b = l_reg[LRegs::LReg1];
  auto [lx, ry] = min_max (a, b);
  auto [l0, r0] = min_max (lx, ry, 0x00000000); // min,min,min,min MIN_MAX      1
  auto [l1, r1] = min_max (l0, r0, 0x000000ff); // min,min,min,max MAX123,MIN0 ~5
  auto [l2, r2] = min_max (l1, r1, 0x0000ff00); // min,min,max,min MAX023,MIN1 ~6
  auto [l3, r3] = min_max (l2, r2, 0x0000ffff); // min,min,max,max MAX23,MIN01 ~2
  auto [l4, r4] = min_max (l3, r3, 0x00ff0000); // min,max,min,min MAX013,MIN2 ~7
  auto [l5, r5] = min_max (l4, r4, 0x00ff00ff); // min,max,min,max MAX13,MIN02 ~3
  auto [l6, r6] = min_max (l5, r5, 0x00ffff00); // min,max,max,min MIN03_MAX12  4
  auto [l7, r7] = min_max (l6, r6, 0x00ffffff); // min,max,max,max MIN3_MAX012  8
  auto [l8, r8] = min_max (l7, r7, 0xff000000); // max,min,min,min MAX012,MIN3 ~8
  auto [l9, r9] = min_max (l8, r8, 0xff0000ff); // max,min,min,max MAX12,MIN03 ~4
  auto [la, ra] = min_max (l9, r9, 0xff00ff00); // max,min,max,min MIN02_MAX13  3
  auto [lb, rb] = min_max (la, ra, 0xff00ffff); // max,min,max,min MIN2_MAX013  7
  auto [lc, rc] = min_max (lb, rb, 0xffff0000); // max,max,min,min MIN01_MAX23  2
  auto [ld, rd] = min_max (lc, rc, 0xffff00ff); // max,max,min,max MIN1_MAX023  6
  auto [le, re] = min_max (ld, rd, 0xffffff00); // max,max,max,min MIN0_MAX123  5
  auto [lf, rf] = min_max (le, re, 0xffffffff); // max,max,max,max MAX,MIN     ~1
  l_reg[LRegs::LReg0] = lf;
  l_reg[LRegs::LReg1] = rf;
}
/*
**_Z9min_max_1v:
**	# READ L0
**	# READ L1
**	SFPSWAP	L0, L1, 1
**	SFPSWAP	L0, L1, 1
**	SFPSWAP	L1, L0, 5
**	SFPSWAP	L1, L0, 6
**	SFPSWAP	L1, L0, 2
**	SFPSWAP	L1, L0, 7
**	SFPSWAP	L1, L0, 3
**	SFPSWAP	L0, L1, 4
**	SFPSWAP	L0, L1, 8
**	SFPSWAP	L1, L0, 8
**	SFPSWAP	L1, L0, 4
**	SFPSWAP	L0, L1, 3
**	SFPSWAP	L0, L1, 7
**	SFPSWAP	L0, L1, 2
**	SFPSWAP	L0, L1, 6
**	SFPSWAP	L0, L1, 5
**	SFPSWAP	L1, L0, 1
**	# WRITE L0
**	# WRITE L1
**	ret
*/

void min_max_2 () {
  vFloat a = l_reg[LRegs::LReg0];
  vFloat b = l_reg[LRegs::LReg1];
  auto [lx, ry] = min_max (a, b);
  auto [l0, r0] = min_max (lx, ry, 0x0); // min,min,min,min MIN_MAX      1
  auto [l1, r1] = min_max (l0, r0, 0x1); // min,min,min,max MAX123,MIN0 ~5
  auto [l2, r2] = min_max (l1, r1, 0x2); // min,min,max,min MAX023,MIN1 ~6
  auto [l3, r3] = min_max (l2, r2, 0x3); // min,min,max,max MAX23,MIN01 ~2
  auto [l4, r4] = min_max (l3, r3, 0x4); // min,max,min,min MAX013,MIN2 ~7
  auto [l5, r5] = min_max (l4, r4, 0x5); // min,max,min,max MAX13,MIN02 ~3
  auto [l6, r6] = min_max (l5, r5, 0x6); // min,max,max,min MIN03_MAX12  4
  auto [l7, r7] = min_max (l6, r6, 0x7); // min,max,max,max MIN3_MAX012  8
  auto [l8, r8] = min_max (l7, r7, 0x8); // max,min,min,min MAX012,MIN3 ~8
  auto [l9, r9] = min_max (l8, r8, 0x9); // max,min,min,max MAX12,MIN03 ~4
  auto [la, ra] = min_max (l9, r9, 0xa); // max,min,max,min MIN02_MAX13  3
  auto [lb, rb] = min_max (la, ra, 0xb); // max,min,max,min MIN2_MAX013  7
  auto [lc, rc] = min_max (lb, rb, 0xc); // max,max,min,min MIN01_MAX23  2
  auto [ld, rd] = min_max (lc, rc, 0xd); // max,max,min,max MIN1_MAX023  6
  auto [le, re] = min_max (ld, rd, 0xe); // max,max,max,min MIN0_MAX123  5
  auto [lf, rf] = min_max (le, re, 0xf); // max,max,max,max MAX,MIN     ~1
  l_reg[LRegs::LReg0] = lf;
  l_reg[LRegs::LReg1] = rf;
}
/*
**_Z9min_max_2v:
**	tail	_Z9min_max_1v
*/

void min () {
  vFloat a = l_reg[LRegs::LReg0];
  vFloat b = l_reg[LRegs::LReg1];
  l_reg[LRegs::LReg0] = sfpi::min (a, b);
}
/*
**_Z3minv:
**	# READ L0
**	# READ L1
**	SFPSWAP	L0, L1, 1
**	# WRITE L0
**	ret
*/

void max () {
  vFloat a = l_reg[LRegs::LReg0];
  vFloat b = l_reg[LRegs::LReg1];
  l_reg[LRegs::LReg0] = sfpi::max (a, b);
}
/*
**_Z3maxv:
**	# READ L0
**	# READ L1
**	SFPSWAP	L1, L0, 1
**	# WRITE L0
**	ret
*/

void clamp () {
  vFloat a = l_reg[LRegs::LReg0];
  vFloat lower = l_reg[LRegs::LReg1];
  vFloat upper = l_reg[LRegs::LReg2];
  l_reg[LRegs::LReg0] = sfpi::clamp (a, lower, upper);
}
/*
**_Z5clampv:
**	# READ L0
**	# READ L1
**	# READ L2
**	SFPSWAP	L1, L0, 1
**	SFPSWAP	L0, L2, 1
**	# WRITE L0
**	ret
*/

void clamp2 () {
  vFloat a = l_reg[LRegs::LReg0];
  l_reg[LRegs::LReg0] = sfpi::clamp (a, 0.0f, 1.0f);
}
/*
**_Z6clamp2v:
**	# READ L0
**	SFPSWAP	L9, L0, 1
**	SFPSWAP	L0, L10, 1
**	# WRITE L0
**	ret
*/

void clamp3 () {
  vFloat a = l_reg[LRegs::LReg0];
  l_reg[LRegs::LReg0] = sfpi::symmetric_clamp (a, 1.0f);
}
/*
**_Z6clamp3v:
**	# READ L0
**	SFPABS	L1, L0, 1
**	SFPSWAP	L1, L10, 1
**	SFPSETSGN	L0, L1, 0, 0
**	# WRITE L0
**	ret
*/

void clamp4a () {
  vFloat a = l_reg[LRegs::LReg0];
  l_reg[LRegs::LReg0] = sfpi::clamp (a, -10.0f, 10.0f);
}
/*
**_Z7clamp4av:
**	# READ L0
**	SFPLOADI	L2, 49440, 0
**	SFPLOADI	L1, 16672, 0
**	SFPSWAP	L2, L0, 1
**	SFPSWAP	L0, L1, 1
**	# WRITE L0
**	ret
*/

void clamp4b () {
  vFloat a = l_reg[LRegs::LReg0];
  l_reg[LRegs::LReg0] = sfpi::symmetric_clamp (a, 10.0f);
}
/*
**_Z7clamp4bv:
**	# READ L0
**	SFPABS	L1, L0, 1
**	SFPLOADI	L2, 16672, 0
**	SFPSWAP	L1, L2, 1
**	SFPSETSGN	L0, L1, 0, 0
**	# WRITE L0
**	ret
*/

// ICEd, see #35446 comment

void ice ()
{
  vFloat a = l_reg[LRegs::LReg0];
  vFloat c = l_reg[LRegs::LReg1];

  v_if (c != 0) {
    a = max (a, 0.0f);
    
  } v_endif;
  l_reg[LRegs::LReg2] = a;
}
/*
**_Z3icev:
**	# READ L0
**	# READ L1
**	SFPSETCC	L1, 0, 2
**	SFPMOV	L1, L0, 2
**	SFPSWAP	L9, L1, 1
**	SFPMOV	L0, L1, 0	# LV:L0
**	SFPMOV	L2, L0, 2
**	SFPENCC	3, 10
**	# WRITE L2
**	ret
*/

void minl ()
{
  vFloat a = l_reg[LRegs::LReg0];
  vFloat b = l_reg[LRegs::LReg1];

  a = min (a, b);
  l_reg[LRegs::LReg0] = a;
  l_reg[LRegs::LReg1] = b;
}
/*
**_Z4minlv:
**	# READ L0
**	# READ L1
**	SFPMOV	L2, L1, 2
**	SFPSWAP	L0, L2, 1
**	# WRITE L0
**	# WRITE L1
**	ret
*/

void maxl ()
{
  vFloat a = l_reg[LRegs::LReg0];
  vFloat b = l_reg[LRegs::LReg1];

  a = max (a, b);
  l_reg[LRegs::LReg0] = a;
  l_reg[LRegs::LReg1] = b;
}
/*
**_Z4maxlv:
**	# READ L0
**	# READ L1
**	SFPMOV	L2, L1, 2
**	SFPSWAP	L2, L0, 1
**	# WRITE L0
**	# WRITE L1
**	ret
*/

void clampl ()
{
  vFloat a = l_reg[LRegs::LReg0];
  vFloat b = l_reg[LRegs::LReg1];
  vFloat c = l_reg[LRegs::LReg2];

  a = clamp (a, b, c);
  l_reg[LRegs::LReg0] = a;
  l_reg[LRegs::LReg1] = b;
  l_reg[LRegs::LReg2] = c;
}
/*
**_Z6clamplv:
**	# READ L0
**	# READ L1
**	# READ L2
**	SFPMOV	L3, L1, 2
**	SFPSWAP	L3, L0, 1
**	SFPMOV	L3, L2, 2
**	SFPSWAP	L0, L3, 1
**	# WRITE L0
**	# WRITE L1
**	# WRITE L2
**	ret
*/
