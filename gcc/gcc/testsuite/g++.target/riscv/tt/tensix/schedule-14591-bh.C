// { dg-options "-mcpu=tt-bh-tensix -fno-exceptions -fno-rtti -O2 -fno-shrink-wrap" }
// { dg-final { check-function-bodies "**" "" } }

extern volatile unsigned long iptr[];

// Positive tests: SFPMAD followed by an MAD-pipeline consumer with register
// dependency.  BH scoreboarding is broken for these consumers, so a
// NOP must be inserted between the MAD pipeline producer and the
// MAD-pipeline consumer.

namespace nop {

void mad_and () {
  auto a = __builtin_rvtt_sfpreadlreg(0);
  auto b = __builtin_rvtt_sfpreadlreg(1);
  auto neg1 = __builtin_rvtt_sfpreadlreg(11);
  auto r = __builtin_rvtt_sfpmad(neg1, a, b, 0);
  auto s = __builtin_rvtt_sfpand(r, b);
  __builtin_rvtt_sfpwritelreg(s, 3);
}
/*
**_ZN3nop7mad_andEv:
** 	# READ L0
**	# READ L1
**	SFPMAD	L0, L11, L0, L1, 0
**	SFPNOP
**	SFPAND	L3, L0, L1, 1
**	# WRITE L3
**	ret
*/

void mad_or () {
  auto a = __builtin_rvtt_sfpreadlreg(0);
  auto b = __builtin_rvtt_sfpreadlreg(1);
  auto neg1 = __builtin_rvtt_sfpreadlreg(11);
  auto r = __builtin_rvtt_sfpmad(neg1, a, b, 0);
  auto s = __builtin_rvtt_sfpor(r, b);
  __builtin_rvtt_sfpwritelreg(s, 3);
}
/*
**_ZN3nop6mad_orEv:
** 	# READ L0
**	# READ L1
**	SFPMAD	L0, L11, L0, L1, 0
**	SFPNOP
**	SFPOR	L3, L0, L1, 1
**	# WRITE L3
**	ret
*/

void mad_shftv () {
  auto a = __builtin_rvtt_sfpreadlreg(0);
  auto b = __builtin_rvtt_sfpreadlreg(1);
  auto neg1 = __builtin_rvtt_sfpreadlreg(11);
  auto r = __builtin_rvtt_sfpmad(neg1, a, b, 0);
  auto s = __builtin_rvtt_sfpshft_v(r, b, 0);
  __builtin_rvtt_sfpstore(nullptr, s, 0, 0, 0, 0, 0);
}
/*
**_ZN3nop9mad_shftvEv:
** 	# READ L0
**	# READ L1
**	SFPMAD	L0, L11, L0, L1, 0
**	SFPNOP
**	SFPSHFT	L0, L1, 0, 0
**	SFPSTORE	L0, 0, 0, 0
**	ret
*/

void mad_iadd () {
  auto a = __builtin_rvtt_sfpreadlreg(0);
  auto b = __builtin_rvtt_sfpreadlreg(1);
  auto neg1 = __builtin_rvtt_sfpreadlreg(11);
  auto r = __builtin_rvtt_sfpmad(neg1, a, b, 0);
  auto s = __builtin_rvtt_sfpiadd_v(r, b, 4);
  __builtin_rvtt_sfpstore(nullptr, s, 0, 0, 0, 0, 0);
}
/*
**_ZN3nop8mad_iaddEv:
** 	# READ L0
**	# READ L1
**	SFPMAD	L0, L11, L0, L1, 0
**	SFPNOP
**	SFPIADD	L0, L1, 0, 4
**	SFPSTORE	L0, 0, 0, 0
**	ret
*/

void mad_config () {
  auto a = __builtin_rvtt_sfpreadlreg(0);
  auto b = __builtin_rvtt_sfpreadlreg(1);
  auto neg1 = __builtin_rvtt_sfpreadlreg(11);
  auto r = __builtin_rvtt_sfpmad(neg1, a, b, 0);
  __builtin_rvtt_sfpwriteconfig_v(r, 0, 0);
}
/*
**_ZN3nop10mad_configEv:
** 	# READ L0
**	# READ L1
**	SFPMAD	L0, L11, L0, L1, 0
**	SFPNOP
**	SFPCONFIG	0, 0, 0	# R:L0 CFG:0
**	ret
*/

void mad_swap () {
  auto a = __builtin_rvtt_sfpreadlreg(0);
  auto b = __builtin_rvtt_sfpreadlreg(1);
  auto neg1 = __builtin_rvtt_sfpreadlreg(11);
  auto r = __builtin_rvtt_sfpmad(neg1, a, b, 0);
  auto s = __builtin_rvtt_sfpswap(r, b, 0);
  auto s0 = __builtin_rvtt_sfpselect2(s, 0);
  __builtin_rvtt_sfpstore(nullptr, s0, 0, 0, 0, 0, 0);
}
/*
**_ZN3nop8mad_swapEv:
** 	# READ L0
**	# READ L1
**	SFPMAD	L0, L11, L0, L1, 0
**	SFPNOP
**	SFPSWAP	L0, L1, 0
**	SFPSTORE	L0, 0, 0, 0
**	ret
*/

void mad_swap_cst1 () {
  auto a = __builtin_rvtt_sfpreadlreg(0);
  auto b = __builtin_rvtt_sfpreadlreg(1);
  auto neg1 = __builtin_rvtt_sfpreadlreg(11);
  auto c10 = __builtin_rvtt_sfpreadlreg(10);
  auto r = __builtin_rvtt_sfpmad(neg1, a, b, 0);
  auto s = __builtin_rvtt_sfpswap(r, c10, 0);
  auto s0 = __builtin_rvtt_sfpselect2(s, 0);
  __builtin_rvtt_sfpstore(nullptr, s0, 0, 0, 0, 0, 0);
}
/*
**_ZN3nop13mad_swap_cst1Ev:
** 	# READ L0
**	# READ L1
**	SFPMAD	L0, L11, L0, L1, 0
**	SFPNOP
**	SFPSWAP	L0, L10, 0
**	SFPSTORE	L0, 0, 0, 0
**	ret
*/

void mad_swap_cst2 () {
  auto a = __builtin_rvtt_sfpreadlreg(0);
  auto b = __builtin_rvtt_sfpreadlreg(1);
  auto neg1 = __builtin_rvtt_sfpreadlreg(11);
  auto c10 = __builtin_rvtt_sfpreadlreg(10);
  auto r = __builtin_rvtt_sfpmad(neg1, a, b, 0);
  auto s = __builtin_rvtt_sfpswap(c10, r, 0);
  auto s1 = __builtin_rvtt_sfpselect2(s, 1);
  __builtin_rvtt_sfpstore(nullptr, s1, 0, 0, 0, 0, 0);
}
/*
**_ZN3nop13mad_swap_cst2Ev:
** 	# READ L0
**	# READ L1
**	SFPMAD	L0, L11, L0, L1, 0
**	SFPNOP
**	SFPSWAP	L10, L0, 0
**	SFPSTORE	L0, 0, 0, 0
**	ret
*/

void mad_shft2 () {
  auto a = __builtin_rvtt_sfpreadlreg(0);
  auto b = __builtin_rvtt_sfpreadlreg(1);
  auto neg1 = __builtin_rvtt_sfpreadlreg(11);
  auto r = __builtin_rvtt_sfpmad(neg1, a, b, 0);
  auto s = __builtin_rvtt_sfpshft2_subvec_shfl1(r, 3);
  __builtin_rvtt_sfpstore(nullptr, s, 0, 0, 0, 0, 0);
}
/*
**_ZN3nop9mad_shft2Ev:
** 	# READ L0
**	# READ L1
**	SFPMAD	L0, L11, L0, L1, 0
**	SFPNOP
**	SFPSHFT2	L0, L0, 0, 3
**	SFPNOP
**	SFPSTORE	L0, 0, 0, 0
**	ret
*/

}

// Negative tests: SFPMAD followed by a non-MAD-pipeline consumer with
// register dependency (mad_mul), or an MAD-pipeline consumer with no
// register dependency (mad_swap_cst3).  No NOP should be inserted
// for the mad_pipeline delay.

namespace nonop {

void mad_shfti () {
  auto a = __builtin_rvtt_sfpreadlreg(0);
  auto b = __builtin_rvtt_sfpreadlreg(1);
  auto neg1 = __builtin_rvtt_sfpreadlreg(11);
  auto r = __builtin_rvtt_sfpmad(neg1, a, b, 0);
  auto s = __builtin_rvtt_sfpshft_i(iptr, r, 2, 0, 0, 0);
  __builtin_rvtt_sfpwritelreg(s, 3);
}
/*
**_ZN5nonop9mad_shftiEv:
** 	# READ L0
**	# READ L1
**	SFPMAD	L0, L11, L0, L1, 0
**	SFPSHFT	L3, L0, 2, 5
**	# WRITE L3
**	ret
*/

void mad_mul () {
  auto a = __builtin_rvtt_sfpreadlreg(0);
  auto b = __builtin_rvtt_sfpreadlreg(1);
  auto neg1 = __builtin_rvtt_sfpreadlreg(11);
  auto r = __builtin_rvtt_sfpmad(neg1, a, b, 0);
  auto s = __builtin_rvtt_sfpmul(r, b, 0);
  __builtin_rvtt_sfpstore(nullptr, s, 0, 0, 0, 0, 0);
}
/*
**_ZN5nonop7mad_mulEv:
** 	# READ L0
**	# READ L1
**	SFPMAD	L0, L11, L0, L1, 0
**	SFPMUL	L0, L0, L1, 0
**	SFPSTORE	L0, 0, 0, 0
**	ret
*/

void mad_swap_cst3 () {
  auto a = __builtin_rvtt_sfpreadlreg(0);
  auto b = __builtin_rvtt_sfpreadlreg(1);
  auto neg1 = __builtin_rvtt_sfpreadlreg(11);
  auto c10 = __builtin_rvtt_sfpreadlreg(10);
  auto r = __builtin_rvtt_sfpmad(neg1, a, b, 0);
  auto s = __builtin_rvtt_sfpswap(c10, neg1, 0);
  __builtin_rvtt_sfpstore(nullptr, r, 0, 0, 0, 0, 0);
}
/*
**_ZN5nonop13mad_swap_cst3Ev:
** 	# READ L0
**	# READ L1
**	SFPMAD	L0, L11, L0, L1, 0
**	SFPSTORE	L0, 0, 0, 0
**	ret
*/

void mad_iadd () {
  auto a = __builtin_rvtt_sfpreadlreg(0);
  auto b = __builtin_rvtt_sfpreadlreg(1);
  auto neg1 = __builtin_rvtt_sfpreadlreg(11);
  auto r = __builtin_rvtt_sfpmad(neg1, a, b, 0);
  auto s = __builtin_rvtt_sfpiadd_i(nullptr, r, 5, 0, 0, 4);
  __builtin_rvtt_sfpstore(nullptr, s, 0, 0, 0, 0, 0);
}
/*
**_ZN5nonop8mad_iaddEv:
** 	# READ L0
**	# READ L1
**	SFPMAD	L0, L11, L0, L1, 0
**	SFPIADD	L0, L0, 5, 5
**	SFPSTORE	L0, 0, 0, 0
**	ret
*/
}
