// { dg-options "-mcpu=tt-wh-tensix -O2 -I [SFPI]/include -fno-exceptions -fno-rtti" }
// { dg-final { check-function-bodies "**" "" } }

namespace ckernel{
    unsigned long *instrn_buffer;
}
#include <sfpi.h>

using namespace sfpi;

void one (float f) {
  l_reg[LRegs::LReg0] = vFloat (f);
  l_reg[LRegs::LReg1] = vFloat (1.0f);
  l_reg[LRegs::LReg2] = vFloat (0.0f);
  l_reg[LRegs::LReg3] = vFloat (0.5f);
  l_reg[LRegs::LReg4] = vFloat (2.0f);
  l_reg[LRegs::LReg5] = vFloat (0x1.8p23f);
  l_reg[LRegs::LReg6] = vFloat (0x1.abcdefp0f);
}
/*
**_Z3onef:
**	slli	a5,a0,16
**	lui	a3,%hi\(_ZN7ckernel13instrn_bufferE\)
**	li	a4, 1895956480	# 1:71020000
**	lw	a3,%lo\(_ZN7ckernel13instrn_bufferE\)\(a3\)
**	srli	a5,a5,16
**	add	a5,a5,a4
**	sw	a5, 0\(a3\)	# 1:SFPLOADI	L0, -, 2
**	srli	a0,a0,16
**	li	a5, 1896349696	# 2:71080000
**	add	a0,a0,a5
**	sw	a0, 0\(a3\)	# 2:SFPLOADI	L0, -, 8	# LV:L0
**	# WRITE L0
**	SFPMOV	L1, L10, 2
**	# WRITE L1
**	SFPMOV	L2, L9, 2
**	# WRITE L2
**	SFPLOADI	L3, 16128, 0
**	# WRITE L3
**	SFPLOADI	L4, 16384, 0
**	# WRITE L4
**	SFPLOADI	L5, 19264, 0
**	# WRITE L5
**	SFPLOADI	L6, 59128, 2
**	SFPLOADI	L6, 16341, 8	# LV:L6
**	# WRITE L6
**	ret
*/

void two (unsigned b) {
  l_reg[LRegs::LReg0] = vFloat (sFloat16b (b));
  l_reg[LRegs::LReg1] = vFloat (sFloat16b (1.0f));
  l_reg[LRegs::LReg2] = vFloat (sFloat16b (0x4000));
  l_reg[LRegs::LReg3] = vFloat (sFloat16b (0x4800));
  l_reg[LRegs::LReg4] = vFloat (sFloat16b (0x4b40));
}
/*
**_Z3twoj:
**	slli	a0,a0,16
**	lui	a4,%hi\(_ZN7ckernel13instrn_bufferE\)
**	li	a5, 1895825408	# 1:71000000
**	lw	a4,%lo\(_ZN7ckernel13instrn_bufferE\)\(a4\)
**	srli	a0,a0,16
**	add	a0,a0,a5
**	sw	a0, 0\(a4\)	# 1:SFPLOADI	L0, -, 0
**	# WRITE L0
**	SFPMOV	L1, L10, 2
**	# WRITE L1
**	SFPLOADI	L2, 16384, 0
**	# WRITE L2
**	SFPLOADI	L3, 18432, 0
**	# WRITE L3
**	SFPLOADI	L4, 19264, 0
**	# WRITE L4
**	ret
*/

void three (unsigned b) {
  l_reg[LRegs::LReg0] = vInt (1);
  l_reg[LRegs::LReg1] = vInt (0x00010000);
  l_reg[LRegs::LReg2] = vInt (0x40000000);
  l_reg[LRegs::LReg3] = vInt (0x7fffffff);
  l_reg[LRegs::LReg4] = vInt (0xffff8000);
  l_reg[LRegs::LReg5] = vInt (0x7fff);
  l_reg[LRegs::LReg6] = vInt (0x8000);
  l_reg[LRegs::LReg7] = vInt (-1);
}
/*
**_Z5threej:
**	SFPLOADI	L0, 1, 2
**	# WRITE L0
**	SFPLOADI	L1, 1, 0
**	# WRITE L1
**	SFPLOADI	L2, 16384, 0
**	# WRITE L2
**	SFPLOADI	L3, 65535, 2
**	SFPLOADI	L3, 32767, 8	# LV:L3
**	# WRITE L3
**	SFPLOADI	L4, 32768, 4
**	# WRITE L4
**	SFPLOADI	L5, 32767, 2
**	# WRITE L5
**	SFPLOADI	L6, 32768, 2
**	# WRITE L6
**	SFPLOADI	L7, 65535, 4
**	# WRITE L7
**	ret
*/

void four ()
{
#pragma GCC unroll 2
  for (unsigned ix = 0; ix != 2; ix++) {
    vUInt a = l_reg[LRegs::LReg0];
    v_if (a < 0x7fffffff + ix) {
      a = 0;
    } v_endif;
    l_reg[LRegs::LReg1] = a;
  }
}
/*
**_Z4fourv:
**	# READ L0
**	SFPLOADI	L1, 65535, 2
**	SFPLOADI	L1, 32767, 8	# LV:L1
**	TTREPLAY	0, 4, 1, 1
**	SFPIADD	L1, L0, 0, 2
**	SFPMOV	L0, L9, 0	# LV:L0
**	SFPMOV	L1, L0, 2
**	SFPENCC	3, 10
**	# WRITE L1
**	# READ L0
**	SFPLOADI	L1, 32768, 0
**	TTREPLAY	0, 4, 0, 0
**	# WRITE L1
**	ret
*/

void loop ()
{
  static const float vals[] = {
    0.0f, 1.0f, -1.0f,
    0x1.5ap1f, // e representable f16b
    0x1.924p1f,  // nearly pi representable as f16a
    0x1.9e3779bp0f // golden ration as f32
  };

  vFloat val = l_reg[LRegs::LReg0];
#pragma GCC unroll 64
  for (unsigned ix = 0; ix != sizeof (vals) / sizeof (vals[0]); ix++)
      val *= vals[ix];
  l_reg[LRegs::LReg1] = val;
}
/*
**_Z4loopv:
**	# READ L0
**	SFPMUL	L1, L0, L9, 0
**	SFPNOP
**	SFPMOV	L1, L1, 1
**	SFPMULI	L1, 16429, 0
**	SFPLOADI	L0, 16969, 1
**	SFPMUL	L1, L1, L0, 0
**	SFPLOADI	L0, 7101, 2
**	SFPLOADI	L0, 16335, 8	# LV:L0
**	SFPMUL	L1, L1, L0, 0
**	SFPNOP
**	# WRITE L1
**	ret
*/

void cfg () {
  constexpr impl_::LRegFile::vCReg<vFloat> neg1 (CREG_IDX_NEG_1);
  neg1 = -1.0f;
}
/*
**_Z3cfgv:
**	SFPCONFIG	11, 0, 1	# CFG:11
**	ret
*/
