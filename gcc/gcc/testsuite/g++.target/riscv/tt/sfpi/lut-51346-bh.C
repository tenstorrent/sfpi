// { dg-options "-mcpu=tt-bh-tensix -O2 -I [SFPI]/include -fno-exceptions -fno-rtti" }
// { dg-final { check-function-bodies "**" "" } }

namespace ckernel{
    unsigned long *instrn_buffer;
}
#include <sfpi.h>

using namespace sfpi;

void LUT () {
  vFloat v = dst_reg[0];
  vLut8si a0 (1.0f, 0.75f);
  vLut8si a1 (0.0f, 2.0f);
  vLut8si a2 (0.125f, 0.0625f);

  v = lut (v, a0, a1, a2);
  dst_reg[0] = v;
}
/*
**_Z3LUTv:
**	SFPLOAD	L3, 0, 0, 7
**	SFPLOADI	L0, 24, 2
**	SFPLOADI	L1, 65295, 2
**	SFPLOADI	L2, 12352, 2
**	SFPLUT	L3, 4	# R:L0,L1,L2,L3
**	SFPSTORE	L3, 0, 0, 7
**	ret
*/

void LUT_SIGN () {
  vFloat v = dst_reg[0];
  vLut8si a0 (1.0f, 0.75f);
  vLut8si a1 (0.0f, 2.0f);
  vLut8si a2 (0.125f, 0.0625f);

  v = lut (v, a0, a1, a2, LutSign::Update);
  dst_reg[0] = v;
}
/*
**_Z8LUT_SIGNv:
**	SFPLOAD	L3, 0, 0, 7
**	SFPLOADI	L0, 24, 2
**	SFPLOADI	L1, 65295, 2
**	SFPLOADI	L2, 12352, 2
**	SFPLUT	L3, 0	# R:L0,L1,L2,L3
**	SFPSTORE	L3, 0, 0, 7
**	ret
*/

void LUT2_FP32 () {
  vFloat v = dst_reg[0];

  v = lut (v, vLut32si (1.0f, 2.0f), vLut32si (3.0f, 4.0f), vLut32si (5.0f, 6.0f));
  dst_reg[0] = v;
}
/*
**_Z9LUT2_FP32v:
**	SFPLOAD	L3, 0, 0, 7
**	SFPLOADI	L4, 16384, 0
**	SFPLOADI	L1, 16448, 0
**	SFPLOADI	L5, 16512, 0
**	SFPLOADI	L2, 16544, 0
**	SFPLOADI	L6, 16576, 0
**	SFPMOV	L0, L10, 2
**	SFPLUTFP32	L1, 4	# R:L0,L1,L2,L4,L5,L6,L3
**	SFPSTORE	L1, 0, 0, 7
**	ret
*/

void LUT2_FP16_3 () {
  vFloat v = dst_reg[0];
  vLut16si a0 (1.0f, 2.0f);
  vLut16si a1 (3.0f, 4.0f);
  vLut16si a2 (5.0f, 6.0f);

  v = lut (v, a0, a1, a2);
  dst_reg[0] = v;
}
/*
**_Z11LUT2_FP16_3v:
**	SFPLOAD	L3, 0, 0, 7
**	SFPLOADI	L0, 8194, 1
**	SFPLOADI	L1, 17408, 2
**	SFPLOADI	L1, 16896, 8	# LV:L1
**	SFPLOADI	L2, 17920, 2
**	SFPLOADI	L2, 17664, 8	# LV:L2
**	SFPLOADI	L7, 0, 2
**	SFPLUTFP32	L0, 14	# R:L0,L1,L2,L3,L7
**	SFPSTORE	L0, 0, 0, 7
**	ret
*/

void LUT2_FP16_6a () {
  vFloat v = dst_reg[0];
  vLut16ss s01 (2.0f, 1.0f);
  vLut16ss s23 (4.0f, 3.0f);
  vLut16ss s45 (6.0f, 5.0f);
  vLut16ii i01 (12.0f, 11.0f);
  vLut16ii i23 (14.0f, 13.0f);
  vLut16ii i45 (16.0f, 15.0f);

  v = lut (v, s01, i01, s23, i23, s45, i45);
  dst_reg[0] = v;
}
/*
**_Z12LUT2_FP16_6av:
**	SFPLOAD	L3, 0, 0, 7
**	SFPLOADI	L0, 8194, 1
**	SFPLOADI	L1, 17408, 2
**	SFPLOADI	L1, 16896, 8	# LV:L1
**	SFPLOADI	L2, 17920, 2
**	SFPLOADI	L2, 17664, 8	# LV:L2
**	SFPLOADI	L4, 18944, 2
**	SFPLOADI	L4, 18816, 8	# LV:L4
**	SFPLOADI	L5, 19200, 2
**	SFPLOADI	L5, 19072, 8	# LV:L5
**	SFPLOADI	L6, 19456, 2
**	SFPLOADI	L6, 19328, 8	# LV:L6
**	SFPLUTFP32	L0, 6	# R:L0,L1,L2,L4,L5,L6,L3
**	SFPSTORE	L0, 0, 0, 7
**	ret
*/

void LUT2_FP16_6b () {
  vFloat v = dst_reg[0];
  vLut16ss s01 (2.0f, 1.0f);
  vLut16ss s23 (4.0f, 3.0f);
  vLut16ss s45 (6.0f, 5.0f);
  vLut16ii i01 (12.0f, 11.0f);
  vLut16ii i23 (14.0f, 13.0f);
  vLut16ii i45 (16.0f, 15.0f);

  v = lut<LutMode::Fp16x6_HWM4> (v, s01, i01, s23, i23, s45, i45);
  dst_reg[0] = v;
}
/*
**_Z12LUT2_FP16_6bv:
**	SFPLOAD	L3, 0, 0, 7
**	SFPLOADI	L0, 8194, 1
**	SFPLOADI	L1, 17408, 2
**	SFPLOADI	L1, 16896, 8	# LV:L1
**	SFPLOADI	L2, 17920, 2
**	SFPLOADI	L2, 17664, 8	# LV:L2
**	SFPLOADI	L4, 18944, 2
**	SFPLOADI	L4, 18816, 8	# LV:L4
**	SFPLOADI	L5, 19200, 2
**	SFPLOADI	L5, 19072, 8	# LV:L5
**	SFPLOADI	L6, 19456, 2
**	SFPLOADI	L6, 19328, 8	# LV:L6
**	SFPLUTFP32	L0, 7	# R:L0,L1,L2,L4,L5,L6,L3
**	SFPSTORE	L0, 0, 0, 7
**	ret
*/

void llk1b () {
  sfpi::l_reg[sfpi::LRegs::LReg0] = sfpi::vLut8si(0.90625f, 0.0f);
  sfpi::l_reg[sfpi::LRegs::LReg1] = sfpi::vLut8si(0.09375f, 0.8125f);
  sfpi::l_reg[sfpi::LRegs::LReg2] = sfpi::vLut8si(0.0f, 1.0f);

  sfpi::vLut8si l0 = l_reg[sfpi::LRegs::LReg0];
  sfpi::vLut8si l1 = l_reg[sfpi::LRegs::LReg1];
  sfpi::vLut8si l2 = l_reg[sfpi::LRegs::LReg2];

  sfpi::vFloat val = sfpi::dst_reg[0];
  val = sfpi::lut(val, l0, l1, l2);
  sfpi::dst_reg[0] = val;
}
/*
**_Z5llk1bv:
**	SFPLOADI	L0, 7679, 2
**	# WRITE L0
**	SFPLOADI	L1, 18458, 2
**	# WRITE L1
**	SFPLOADI	L2, 65280, 2
**	# WRITE L2
**	# READ L0
**	# READ L1
**	# READ L2
**	SFPLOAD	L3, 0, 0, 7
**	SFPLUT	L3, 4	# R:L0,L1,L2,L3
**	SFPSTORE	L3, 0, 0, 7
**	ret
*/

void llk3 () {
  sfpi::l_reg[sfpi::LRegs::LReg0] = sfpi::vLut8si(0.22656f, 0.0f);
  sfpi::l_reg[sfpi::LRegs::LReg1] = sfpi::vLut8si(0.26562f, -0.04687f);
  sfpi::l_reg[sfpi::LRegs::LReg2] = sfpi::vLut8si(0.0f, 0.5f);

  sfpi::vLut8si l0 = l_reg[sfpi::LRegs::LReg0];
  sfpi::vLut8si l1 = l_reg[sfpi::LRegs::LReg1];
  sfpi::vLut8si l2 = l_reg[sfpi::LRegs::LReg2];

  sfpi::vFloat val = sfpi::dst_reg[0];
  val = sfpi::lut(val, l0, l1, l2);
  sfpi::dst_reg[0] = val;
}
/*
**_Z4llk3v:
**	SFPLOADI	L0, 15871, 2
**	# WRITE L0
**	SFPLOADI	L1, 8664, 2
**	# WRITE L1
**	SFPLOADI	L2, 65296, 2
**	# WRITE L2
**	# READ L0
**	# READ L1
**	# READ L2
**	SFPLOAD	L3, 0, 0, 7
**	SFPLUT	L3, 4	# R:L0,L1,L2,L3
**	SFPSTORE	L3, 0, 0, 7
**	ret
*/

void llk4b () {
  sfpi::vLut16ss l0(0.1928f, 0.4939f);
  sfpi::vLut16ss l1(0.6188f, 0.6099f);
  sfpi::vLut16ss l2(0.5402f, 0.5f);
  sfpi::vLut16ii l4(-0.00010443f, -0.1604f);
  sfpi::vLut16ii l5(-0.2796f, -0.2635f);
  sfpi::vLut16ii l6(-0.1194f, 0.0f);

  sfpi::vFloat val = sfpi::dst_reg[0];
  val = sfpi::lut(val, l0, l4, l1, l5, l2, l6, LutSign::Update);
  sfpi::dst_reg[0] = val;
}
/*
**_Z5llk4bv:
**	SFPLOADI	L0, 12843, 2
**	SFPLOADI	L0, 14311, 8	# LV:L0
**	SFPLOADI	L1, 14579, 2
**	SFPLOADI	L1, 14561, 8	# LV:L1
**	SFPLOADI	L2, 14418, 2
**	SFPLOADI	L2, 14336, 8	# LV:L2
**	SFPLOADI	L3, 34520, 2
**	SFPLOADI	L3, 45346, 8	# LV:L3
**	SFPMOV	L4, L3, 2
**	SFPLOADI	L3, 46201, 2
**	SFPLOADI	L3, 46135, 8	# LV:L3
**	SFPMOV	L5, L3, 2
**	SFPLOADI	L6, 44964, 2
**	SFPLOADI	L6, 31744, 8	# LV:L6
**	SFPLOAD	L3, 0, 0, 7
**	SFPLUTFP32	L0, 2	# R:L0,L1,L2,L4,L5,L6,L3
**	SFPSTORE	L0, 0, 0, 7
**	ret
*/

void llk5b () {
  sfpi::vLut16ss l0(0.2452f, 0.2173f);
  sfpi::vLut16ss l1(0.1731f, 0.1262f);
  sfpi::vLut16ss l2(0.0485, 0.0f);
  sfpi::vLut16ii l4(-0.0004997f, 0.0152f);
  sfpi::vLut16ii l5(0.05988f, 0.1298f);
  sfpi::vLut16ii l6(0.2998f, 0.4998f);

  sfpi::vFloat val = sfpi::dst_reg[0];
  val = sfpi::lut(val, l0, l4, l1, l5, l2, l6);
  sfpi::dst_reg[0] = val;
}
/*
**_Z5llk5bv:
**	SFPLOADI	L0, 13273, 2
**	SFPLOADI	L0, 13044, 8	# LV:L0
**	SFPLOADI	L1, 12682, 2
**	SFPLOADI	L1, 12298, 8	# LV:L1
**	SFPLOADI	L2, 10805, 2
**	SFPLOADI	L2, 31744, 8	# LV:L2
**	SFPLOADI	L3, 36888, 2
**	SFPLOADI	L3, 9160, 8	# LV:L3
**	SFPMOV	L4, L3, 2
**	SFPLOADI	L3, 11178, 2
**	SFPLOADI	L3, 12327, 8	# LV:L3
**	SFPMOV	L5, L3, 2
**	SFPLOADI	L6, 13516, 2
**	SFPLOADI	L6, 14335, 8	# LV:L6
**	SFPLOAD	L3, 0, 0, 7
**	SFPLUTFP32	L0, 6	# R:L0,L1,L2,L4,L5,L6,L3
**	SFPSTORE	L0, 0, 0, 7
**	ret
*/

void llk6b () {
  sfpi::vLut16ss l0(0.1928f, 0.4939f);
  sfpi::vLut16ss l1(0.6188f, 0.6099f);
  sfpi::vLut16ss l2(0.5402f, 0.5f);
  sfpi::vLut16ii l4(-0.00010443f, -0.1604f);
  sfpi::vLut16ii l5(-0.2795f, -0.2635f);
  sfpi::vLut16ii l6(-0.1194f, 0.0f);

  sfpi::vFloat val = sfpi::dst_reg[0];
  val = sfpi::lut(val, l0, l4, l1, l5, l2, l6);
  sfpi::dst_reg[0] = val;
}
/*
**_Z5llk6bv:
**	SFPLOADI	L0, 12843, 2
**	SFPLOADI	L0, 14311, 8	# LV:L0
**	SFPLOADI	L1, 14579, 2
**	SFPLOADI	L1, 14561, 8	# LV:L1
**	SFPLOADI	L2, 14418, 2
**	SFPLOADI	L2, 14336, 8	# LV:L2
**	SFPLOADI	L3, 34520, 2
**	SFPLOADI	L3, 45346, 8	# LV:L3
**	SFPMOV	L4, L3, 2
**	SFPLOADI	L3, 46201, 2
**	SFPLOADI	L3, 46135, 8	# LV:L3
**	SFPMOV	L5, L3, 2
**	SFPLOADI	L6, 44964, 2
**	SFPLOADI	L6, 31744, 8	# LV:L6
**	SFPLOAD	L3, 0, 0, 7
**	SFPLUTFP32	L0, 6	# R:L0,L1,L2,L4,L5,L6,L3
**	SFPSTORE	L0, 0, 0, 7
**	ret
*/
