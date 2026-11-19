// { dg-options "-mcpu=tt-qsr32-tensix -fno-exceptions -fno-rtti -O2" }
// { dg-final { check-function-bodies "**" "" } }

void storesrcs() {
  auto v0 = __builtin_rvtt_sfpreadlreg (0);

  __builtin_rvtt_sfpstoresrcs (nullptr, v0, 0, 0, 0, 0, 0, 0);
  __builtin_rvtt_sfpstoresrcs (nullptr, v0, 0, 0, 0, 0, 0, 1);
}
/* 
**_Z9storesrcsv:
**	# READ L0
**	SFPSTORE	L0, 0, 0, 0, 1, 0
**	SFPSTORE	L0, 0, 0, 0, 1, 1
**	ret
*/

void loadsrcs() {
  auto v0 = __builtin_rvtt_sfploadsrcs (nullptr, 0, 0, 0, 0, 0, 0);
  auto v1 = __builtin_rvtt_sfploadsrcs (nullptr, 0, 0, 0, 0, 0, 1);

  __builtin_rvtt_sfpwritelreg (v0, 0);
  __builtin_rvtt_sfpwritelreg (v1, 0);
} 
/*
**_Z8loadsrcsv:
**  SFPLOAD	L0, 0, 0, 0, 1, 0
**  SFPLOAD	L1, 0, 0, 0, 1, 1
**  # WRITE L0
**  SFPMOV	L0, L1, 2
**  # WRITE L0
**  ret
*/

void loadsrcs_lv() {
  auto x =  __builtin_rvtt_sfpreadlreg (9);
  auto v0 = __builtin_rvtt_sfploadsrcs_lv (nullptr, x, 0, 0, 0, 0, 0, 0);
  auto v1 = __builtin_rvtt_sfploadsrcs_lv (nullptr, x, 0, 0, 0, 0, 0, 1);

  __builtin_rvtt_sfpwritelreg (v0, 0);
  __builtin_rvtt_sfpwritelreg (v1, 0);
} 
/*
**_Z11loadsrcs_lvv:
**  SFPMOV	L0, L9, 2
**  SFPLOAD	L0, 0, 0, 0, 1, 0	# LV:L0
**  SFPMOV	L1, L9, 2
**  SFPLOAD	L1, 0, 0, 0, 1, 1	# LV:L1
**  # WRITE L0
**  SFPMOV	L0, L1, 2
**  # WRITE L0
**  ret
*/
