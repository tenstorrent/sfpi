// { dg-options "-mcpu=tt-qsr32-tensix -O2 -I [SFPI]/include -fno-exceptions -fno-rtti" }
// { dg-final { check-function-bodies "**" "" } }

namespace ckernel{
    unsigned long *instrn_buffer;
}
#include <sfpi.h>

using namespace sfpi;

void LUT () {
  vFloat v = dst_reg[0];
  sLut8si a0 (1.0f, 0.75f);
  sLut8si a1 (0.0f, 2.0f);
  sLut8si a2 (0.125f, 0.0625f);

  v = lut (v, lut_init (a0, a1, a2));
  dst_reg[0] = v;
}
/*
**_Z3LUTv:
**	SFPLOAD	L3, 0, 0, 7, 0, 0
**	SFPCONFIG	9, 24, 1	# CFG:9
**	SFPCONFIG	10, 65295, 1	# CFG:10
**	SFPLOADI	L0, 12352, 2
**	SFPCONFIG	11, 0, 0	# R:L0 CFG:11
**	SFPLUT	L3, 4	# R:L3
**	SFPSTORE	L3, 0, 0, 7, 0, 0
**	ret
*/

void LUT_SIGN () {
  vFloat v = dst_reg[0];
  sLut8si a0 (1.0f, 0.75f);
  sLut8si a1 (0.0f, 2.0f);
  sLut8si a2 (0.125f, 0.0625f);

  v = lut (v, lut_init (a0, a1, a2), LutSign::Update);
  dst_reg[0] = v;
}
/*
**_Z8LUT_SIGNv:
**	SFPLOAD	L3, 0, 0, 7, 0, 0
**	SFPCONFIG	9, 24, 1	# CFG:9
**	SFPCONFIG	10, 65295, 1	# CFG:10
**	SFPLOADI	L0, 12352, 2
**	SFPCONFIG	11, 0, 0	# R:L0 CFG:11
**	SFPLUT	L3, 0	# R:L3
**	SFPSTORE	L3, 0, 0, 7, 0, 0
**	ret
*/

void LUT2_FP32 () {
  vFloat v = dst_reg[0];

  v = lut (v, lut_init (sLut32si (1.0f, 2.0f), sLut32si (3.0f, 4.0f), sLut32si (5.0f, 6.0f)));
  dst_reg[0] = v;
}
/*
**_Z9LUT2_FP32v:
**	SFPLOAD	L3, 0, 0, 7, 0, 0
**	SFPMOV	L0, L10, 2
**	SFPCONFIG	9, 0, 0	# R:L0 CFG:9
**	SFPLOADI	L0, 16384, 0
**	SFPCONFIG	12, 0, 0	# R:L0 CFG:12
**	SFPLOADI	L0, 16448, 0
**	SFPCONFIG	10, 0, 0	# R:L0 CFG:10
**	SFPLOADI	L0, 16512, 0
**	SFPCONFIG	13, 0, 0	# R:L0 CFG:13
**	SFPLOADI	L0, 16544, 0
**	SFPCONFIG	11, 0, 0	# R:L0 CFG:11
**	SFPLOADI	L0, 16576, 0
**	SFPCONFIG	14, 0, 0	# R:L0 CFG:14
**	SFPLUTFP32	L3, 4	# R:L3
**	SFPSTORE	L3, 0, 0, 7, 0, 0
**	ret
*/

void LUT2_FP16_3 () {
  vFloat v = dst_reg[0];
  sLut16si a0 (1.0f, 2.0f);
  sLut16si a1 (3.0f, 4.0f);
  sLut16si a2 (5.0f, 6.0f);

  v = lut (v, lut_init (a0, a1, a2));
  dst_reg[0] = v;
}
/*
**_Z11LUT2_FP16_3v:
**	SFPLOAD	L3, 0, 0, 7, 0, 0
**	SFPLOADI	L0, 8194, 1
**	SFPCONFIG	9, 0, 0	# R:L0 CFG:9
**	SFPLOADI	L0, 17408, 2
**	SFPLOADI	L0, 16896, 8	# LV:L0
**	SFPCONFIG	10, 0, 0	# R:L0 CFG:10
**	SFPLOADI	L0, 17920, 2
**	SFPLOADI	L0, 17664, 8	# LV:L0
**	SFPCONFIG	11, 0, 0	# R:L0 CFG:11
**	SFPLUTFP32	L3, 14	# R:L3
**	SFPSTORE	L3, 0, 0, 7, 0, 0
**	ret
*/

void LUT2_FP16_6a () {
  vFloat v = dst_reg[0];
  sLut16ss s01 (2.0f, 1.0f);
  sLut16ss s23 (4.0f, 3.0f);
  sLut16ss s45 (6.0f, 5.0f);
  sLut16ii i01 (12.0f, 11.0f);
  sLut16ii i23 (14.0f, 13.0f);
  sLut16ii i45 (16.0f, 15.0f);

  v = lut (v, lut_init (s01, i01, s23, i23, s45, i45));
  dst_reg[0] = v;
}
/*
**_Z12LUT2_FP16_6av:
**	SFPLOAD	L3, 0, 0, 7, 0, 0
**	SFPLOADI	L0, 8194, 1
**	SFPCONFIG	9, 0, 0	# R:L0 CFG:9
**	SFPLOADI	L0, 18944, 2
**	SFPLOADI	L0, 18816, 8	# LV:L0
**	SFPCONFIG	12, 0, 0	# R:L0 CFG:12
**	SFPLOADI	L0, 17408, 2
**	SFPLOADI	L0, 16896, 8	# LV:L0
**	SFPCONFIG	10, 0, 0	# R:L0 CFG:10
**	SFPLOADI	L0, 19200, 2
**	SFPLOADI	L0, 19072, 8	# LV:L0
**	SFPCONFIG	13, 0, 0	# R:L0 CFG:13
**	SFPLOADI	L0, 17920, 2
**	SFPLOADI	L0, 17664, 8	# LV:L0
**	SFPCONFIG	11, 0, 0	# R:L0 CFG:11
**	SFPLOADI	L0, 19456, 2
**	SFPLOADI	L0, 19328, 8	# LV:L0
**	SFPCONFIG	14, 0, 0	# R:L0 CFG:14
**	SFPLUTFP32	L3, 6	# R:L3
**	SFPSTORE	L3, 0, 0, 7, 0, 0
**	ret
*/

void LUT2_FP16_6b () {
  vFloat v = dst_reg[0];
  sLut16ss s01 (2.0f, 1.0f);
  sLut16ss s23 (4.0f, 3.0f);
  sLut16ss s45 (6.0f, 5.0f);
  sLut16ii i01 (12.0f, 11.0f);
  sLut16ii i23 (14.0f, 13.0f);
  sLut16ii i45 (16.0f, 15.0f);

  v = lut (v, lut_init<LutMode::Fp16x6_HWM4> (s01, i01, s23, i23, s45, i45));
  dst_reg[0] = v;
}
/*
**_Z12LUT2_FP16_6bv:
**	SFPLOAD	L3, 0, 0, 7, 0, 0
**	SFPLOADI	L0, 8194, 1
**	SFPCONFIG	9, 0, 0	# R:L0 CFG:9
**	SFPLOADI	L0, 18944, 2
**	SFPLOADI	L0, 18816, 8	# LV:L0
**	SFPCONFIG	12, 0, 0	# R:L0 CFG:12
**	SFPLOADI	L0, 17408, 2
**	SFPLOADI	L0, 16896, 8	# LV:L0
**	SFPCONFIG	10, 0, 0	# R:L0 CFG:10
**	SFPLOADI	L0, 19200, 2
**	SFPLOADI	L0, 19072, 8	# LV:L0
**	SFPCONFIG	13, 0, 0	# R:L0 CFG:13
**	SFPLOADI	L0, 17920, 2
**	SFPLOADI	L0, 17664, 8	# LV:L0
**	SFPCONFIG	11, 0, 0	# R:L0 CFG:11
**	SFPLOADI	L0, 19456, 2
**	SFPLOADI	L0, 19328, 8	# LV:L0
**	SFPCONFIG	14, 0, 0	# R:L0 CFG:14
**	SFPLUTFP32	L3, 7	# R:L3
**	SFPSTORE	L3, 0, 0, 7, 0, 0
**	ret
*/

void llk4a () {
  vFloat v = dst_reg[0];

  lut_init (0, sLut16ss (0x322B, 0x37E7), sLut16ii (0x86D8, 0xB122));
  lut_init (1, sLut16ss (0x38F3, 0x38E1), sLut16ii (0xB479, 0xB437));
  lut_init (2, sLut16ss (0x3852, 0x3800), sLut16ii (0xAFA4, 0x7C00));

  v = lut<LutMode::Fp16x6_HWM3> (v);
  dst_reg[0] = v;
}
/*
**_Z5llk4av:
**	SFPLOAD	L3, 0, 0, 7, 0, 0
**	SFPLOADI	L0, 12843, 2
**	SFPLOADI	L0, 14311, 8	# LV:L0
**	SFPCONFIG	9, 0, 0	# R:L0 CFG:9
**	SFPLOADI	L0, 34520, 2
**	SFPLOADI	L0, 45346, 8	# LV:L0
**	SFPCONFIG	12, 0, 0	# R:L0 CFG:12
**	SFPLOADI	L0, 14579, 2
**	SFPLOADI	L0, 14561, 8	# LV:L0
**	SFPCONFIG	10, 0, 0	# R:L0 CFG:10
**	SFPLOADI	L0, 46201, 2
**	SFPLOADI	L0, 46135, 8	# LV:L0
**	SFPCONFIG	13, 0, 0	# R:L0 CFG:13
**	SFPLOADI	L0, 14418, 2
**	SFPLOADI	L0, 14336, 8	# LV:L0
**	SFPCONFIG	11, 0, 0	# R:L0 CFG:11
**	SFPLOADI	L0, 44964, 2
**	SFPLOADI	L0, 31744, 8	# LV:L0
**	SFPCONFIG	14, 0, 0	# R:L0 CFG:14
**	SFPLUTFP32	L3, 6	# R:L3
**	SFPSTORE	L3, 0, 0, 7, 0, 0
**	ret
*/

void llk4b () {
  vFloat v = dst_reg[0];

  lut_init (0, sLut16ss (0.1928f, 0.4939f), sLut16ii (-0.00010443f, -0.1604f));
  lut_init (1, sLut16ss (0.6188f, 0.6099f), sLut16ii (-0.2795f, -0.2635f));
  lut_init (2, sLut16ss (0.5402f, 0.50f), sLut16ii (-0.1194f, 0.0f));

  v = lut<LutMode::Fp16x6_HWM3> (v);
  dst_reg[0] = v;
}
/*
**_Z5llk4bv:
**	SFPLOAD	L3, 0, 0, 7, 0, 0
**	SFPLOADI	L0, 12843, 2
**	SFPLOADI	L0, 14311, 8	# LV:L0
**	SFPCONFIG	9, 0, 0	# R:L0 CFG:9
**	SFPLOADI	L0, 34520, 2
**	SFPLOADI	L0, 45346, 8	# LV:L0
**	SFPCONFIG	12, 0, 0	# R:L0 CFG:12
**	SFPLOADI	L0, 14579, 2
**	SFPLOADI	L0, 14561, 8	# LV:L0
**	SFPCONFIG	10, 0, 0	# R:L0 CFG:10
**	SFPLOADI	L0, 46201, 2
**	SFPLOADI	L0, 46135, 8	# LV:L0
**	SFPCONFIG	13, 0, 0	# R:L0 CFG:13
**	SFPLOADI	L0, 14418, 2
**	SFPLOADI	L0, 14336, 8	# LV:L0
**	SFPCONFIG	11, 0, 0	# R:L0 CFG:11
**	SFPLOADI	L0, 44964, 2
**	SFPLOADI	L0, 31744, 8	# LV:L0
**	SFPCONFIG	14, 0, 0	# R:L0 CFG:14
**	SFPLUTFP32	L3, 6	# R:L3
**	SFPSTORE	L3, 0, 0, 7, 0, 0
**	ret
*/

void llk4c () {
  vFloat v = dst_reg[0];

  lut_init (sLut16si (0.1928f, -0.00010443f),
	    sLut16si (0.4939f, -0.1604f),
	    sLut16si (0.6188f, -0.2795f),
	    sLut16si (0.6099f, -0.2635f),
	    sLut16si (0.5402f, -0.1194f),
	    sLut16si (0.50f,    0.0f));

  v = lut<LutMode::Fp16x6_HWM3> (v);
  dst_reg[0] = v;
}
/*
**_Z5llk4cv:
**	SFPLOAD	L3, 0, 0, 7, 0, 0
**	SFPLOADI	L0, 12843, 2
**	SFPLOADI	L0, 14311, 8	# LV:L0
**	SFPCONFIG	9, 0, 0	# R:L0 CFG:9
**	SFPLOADI	L0, 34520, 2
**	SFPLOADI	L0, 45346, 8	# LV:L0
**	SFPCONFIG	12, 0, 0	# R:L0 CFG:12
**	SFPLOADI	L0, 14579, 2
**	SFPLOADI	L0, 14561, 8	# LV:L0
**	SFPCONFIG	10, 0, 0	# R:L0 CFG:10
**	SFPLOADI	L0, 46201, 2
**	SFPLOADI	L0, 46135, 8	# LV:L0
**	SFPCONFIG	13, 0, 0	# R:L0 CFG:13
**	SFPLOADI	L0, 14418, 2
**	SFPLOADI	L0, 14336, 8	# LV:L0
**	SFPCONFIG	11, 0, 0	# R:L0 CFG:11
**	SFPLOADI	L0, 44964, 2
**	SFPLOADI	L0, 31744, 8	# LV:L0
**	SFPCONFIG	14, 0, 0	# R:L0 CFG:14
**	SFPLUTFP32	L3, 6	# R:L3
**	SFPSTORE	L3, 0, 0, 7, 0, 0
**	ret
*/
