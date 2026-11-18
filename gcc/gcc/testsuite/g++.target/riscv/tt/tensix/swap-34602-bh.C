// { dg-options "-mcpu=tt-bh-tensix -fno-exceptions -fno-rtti -O2 -mno-tt-tensix-optimize-dce" }
// { dg-final { check-function-bodies "**" "" } }

// tos -- how we originally behaved (preserve these cases)
namespace tos {
void dep () {
  auto v0 = __builtin_rvtt_sfpload (nullptr, 0, 0, 0, 0, 0);
  auto v1 = __builtin_rvtt_sfpload (nullptr, 0, 0, 0, 0, 0);

  auto r = __builtin_rvtt_sfpswap (v0, v1, 0);
  v0 = __builtin_rvtt_sfpselect2 (r, 0);
  v1 = __builtin_rvtt_sfpselect2 (r, 1);
  // NOP
  __builtin_rvtt_sfpstore (nullptr, v0, 0, 0, 0, 0, 0);
  __builtin_rvtt_sfpstore (nullptr, v1, 0, 0, 0, 0, 0);
}
/*
**_ZN3tos3depEv:
**	SFPLOAD	L1, 0, 0, 0
**	SFPLOAD	L0, 0, 0, 0
**	SFPSWAP	L1, L0, 0
**	SFPSTORE	L1, 0, 0, 0
**	SFPSTORE	L0, 0, 0, 0
**	ret
*/

void nondep () {
  auto v0 = __builtin_rvtt_sfpload (nullptr, 0, 0, 0, 0, 0);
  auto v1 = __builtin_rvtt_sfpload (nullptr, 0, 0, 0, 0, 0);

  auto r = __builtin_rvtt_sfpswap (v0, v1, 0);
  v0 = __builtin_rvtt_sfpselect2 (r, 0);
  v1 = __builtin_rvtt_sfpselect2 (r, 1);
  // NOP
  __builtin_rvtt_sfpload (nullptr, 0, 0, 0, 0, 0);
  __builtin_rvtt_sfpstore (nullptr, v0, 0, 0, 0, 0, 0);
  __builtin_rvtt_sfpstore (nullptr, v1, 0, 0, 0, 0, 0);
}
/*
**_ZN3tos6nondepEv:
**	SFPLOAD	L1, 0, 0, 0
**	SFPLOAD	L0, 0, 0, 0
**	SFPSWAP	L1, L0, 0
**	SFPLOAD	L2, 0, 0, 0
**	SFPSTORE	L1, 0, 0, 0
**	SFPSTORE	L0, 0, 0, 0
**	ret
*/

}

// tng -- how we do it properly
namespace tng {

void dep () {
  auto v0 = __builtin_rvtt_sfpload (nullptr, 0, 0, 0, 0, 0);
  auto v1 = __builtin_rvtt_sfpload (nullptr, 0, 0, 0, 0, 0);

  auto r = __builtin_rvtt_sfpswap (v0, v1, 0);

  v0 = __builtin_rvtt_sfpselect2 (r, 0);
  v1 = __builtin_rvtt_sfpselect2 (r, 1);  

  __builtin_rvtt_sfpstore (nullptr, v0, 0, 0, 0, 0, 0);
  __builtin_rvtt_sfpstore (nullptr, v1, 0, 0, 0, 0, 0);
}
/*
**_ZN3tng3depEv:
**	SFPLOAD	L1, 0, 0, 0
**	SFPLOAD	L0, 0, 0, 0
**	SFPSWAP	L1, L0, 0
**	SFPSTORE	L1, 0, 0, 0
**	SFPSTORE	L0, 0, 0, 0
**	ret
*/

void nondep () {
  auto v0 = __builtin_rvtt_sfpload (nullptr, 0, 0, 0, 0, 0);
  auto v1 = __builtin_rvtt_sfpload (nullptr, 0, 0, 0, 0, 0);

  auto r = __builtin_rvtt_sfpswap (v0, v1, 0);
  __builtin_rvtt_sfpload (nullptr, 0, 0, 0, 0, 0);

  v0 = __builtin_rvtt_sfpselect2 (r, 0);
  v1 = __builtin_rvtt_sfpselect2 (r, 1);  

  __builtin_rvtt_sfpstore (nullptr, v0, 0, 0, 0, 0, 0);
  __builtin_rvtt_sfpstore (nullptr, v1, 0, 0, 0, 0, 0);
}
/*
**_ZN3tng6nondepEv:
**	SFPLOAD	L1, 0, 0, 0
**	SFPLOAD	L0, 0, 0, 0
**	SFPSWAP	L1, L0, 0
**	SFPLOAD	L2, 0, 0, 0
**	SFPSTORE	L1, 0, 0, 0
**	SFPSTORE	L0, 0, 0, 0
**	ret
*/

void live () {
  auto v0 = __builtin_rvtt_sfpload (nullptr, 0, 0, 0, 0, 0);
  auto v1 = __builtin_rvtt_sfpload (nullptr, 0, 0, 0, 0, 0);

  auto r = __builtin_rvtt_sfpswap (v0, v1, 0);
  // NOP
  auto r0 = __builtin_rvtt_sfpselect2 (r, 0);
  auto r1 = __builtin_rvtt_sfpselect2 (r, 1);  

  __builtin_rvtt_sfpstore (nullptr, r0, 0, 0, 0, 0, 0);
  __builtin_rvtt_sfpstore (nullptr, r1, 0, 0, 0, 0, 0);
  __builtin_rvtt_sfpstore (nullptr, v0, 0, 0, 0, 0, 0);
  __builtin_rvtt_sfpstore (nullptr, v1, 0, 0, 0, 0, 0);
}
/*
**_ZN3tng4liveEv:
**	SFPLOAD	L1, 0, 0, 0
**	SFPLOAD	L0, 0, 0, 0
**	SFPMOV	L3, L1, 2
**	SFPMOV	L2, L0, 2
**	SFPSWAP	L3, L2, 0
**	SFPSTORE	L3, 0, 0, 0
**	SFPSTORE	L2, 0, 0, 0
**	SFPSTORE	L1, 0, 0, 0
**	SFPSTORE	L0, 0, 0, 0
**	ret
*/

void dead1 () {
  auto v0 = __builtin_rvtt_sfpload (nullptr, 0, 0, 0, 0, 0);
  auto v1 = __builtin_rvtt_sfpload (nullptr, 0, 0, 0, 0, 0);

  auto r = __builtin_rvtt_sfpswap (v0, v1, 0);

  v0 = __builtin_rvtt_sfpselect2 (r, 0);

  __builtin_rvtt_sfpstore (nullptr, v0, 0, 0, 0, 0, 0);
}
/*
**_ZN3tng5dead1Ev:
**	SFPLOAD	L0, 0, 0, 0
**	SFPLOAD	L1, 0, 0, 0
**	SFPSWAP	L0, L1, 0
**	SFPSTORE	L0, 0, 0, 0
**	ret
*/

void dead0 () {
  auto v0 = __builtin_rvtt_sfpload (nullptr, 0, 0, 0, 0, 0);
  auto v1 = __builtin_rvtt_sfpload (nullptr, 0, 0, 0, 0, 0);

  auto r = __builtin_rvtt_sfpswap (v0, v1, 0);

  v1 = __builtin_rvtt_sfpselect2 (r, 1);

  __builtin_rvtt_sfpstore (nullptr, v1, 0, 0, 0, 0, 0);
}
/*
**_ZN3tng5dead0Ev:
**	SFPLOAD	L1, 0, 0, 0
**	SFPLOAD	L0, 0, 0, 0
**	SFPSWAP	L1, L0, 0
**	SFPSTORE	L0, 0, 0, 0
**	ret
*/

void cstdead1 () {
  auto v0 = __builtin_rvtt_sfpload (nullptr, 0, 0, 0, 0, 0);
  auto v1 = __builtin_rvtt_sfpreadlreg (8);

  auto r = __builtin_rvtt_sfpswap (v0, v1, 0);

  v0 = __builtin_rvtt_sfpselect2 (r, 0);

  __builtin_rvtt_sfpstore (nullptr, v0, 0, 0, 0, 0, 0);
}
/*
**_ZN3tng8cstdead1Ev:
**	SFPLOAD	L0, 0, 0, 0
**	SFPSWAP	L0, L8, 0
**	SFPSTORE	L0, 0, 0, 0
**	ret
*/

void cstdead1a () {
  auto v0 = __builtin_rvtt_sfpload (nullptr, 0, 0, 0, 0, 0);
  auto v1 = __builtin_rvtt_sfpreadlreg (8);

  auto r = __builtin_rvtt_sfpswap (v0, v1, 0);

  v0 = __builtin_rvtt_sfpselect2 (r, 0);
  auto dead = __builtin_rvtt_sfpselect2 (r, 1);

  __builtin_rvtt_sfpstore (nullptr, v0, 0, 0, 0, 0, 0);
}
/*
**_ZN3tng9cstdead1aEv:
**	SFPLOAD	L0, 0, 0, 0
**	SFPSWAP	L0, L8, 0
**	SFPSTORE	L0, 0, 0, 0
**	ret
*/

void cstdead0 () {
  auto v0 = __builtin_rvtt_sfpreadlreg (8);
  auto v1 = __builtin_rvtt_sfpload (nullptr, 0, 0, 0, 0, 0);

  auto r = __builtin_rvtt_sfpswap (v0, v1, 0);

  v1 = __builtin_rvtt_sfpselect2 (r, 1);

  __builtin_rvtt_sfpstore (nullptr, v1, 0, 0, 0, 0, 0);
}
/*
**_ZN3tng8cstdead0Ev:
**	SFPLOAD	L0, 0, 0, 0
**	SFPSWAP	L8, L0, 0
**	SFPSTORE	L0, 0, 0, 0
**	ret
*/

void cstdead0a () {
  auto v0 = __builtin_rvtt_sfpreadlreg (8);
  auto v1 = __builtin_rvtt_sfpload (nullptr, 0, 0, 0, 0, 0);

  auto r = __builtin_rvtt_sfpswap (v0, v1, 0);

  auto dead = __builtin_rvtt_sfpselect2 (r, 0);
  v1 = __builtin_rvtt_sfpselect2 (r, 1);

  __builtin_rvtt_sfpstore (nullptr, v1, 0, 0, 0, 0, 0);
}
/*
**_ZN3tng9cstdead0aEv:
**	SFPLOAD	L0, 0, 0, 0
**	SFPSWAP	L8, L0, 0
**	SFPSTORE	L0, 0, 0, 0
**	ret
*/

void cstdead01 () {
  auto v0 = __builtin_rvtt_sfpreadlreg (8);
  auto v1 = __builtin_rvtt_sfpload (nullptr, 0, 0, 0, 0, 0);

  auto r = __builtin_rvtt_sfpswap (v0, v1, 0);
}
/*
**_ZN3tng9cstdead01Ev:
**	SFPLOAD	L0, 0, 0, 0
**	SFPSWAP	L8, L0, 0
**	ret
*/

void cstdead01a () {
  auto v0 = __builtin_rvtt_sfpreadlreg (8);
  auto v1 = __builtin_rvtt_sfpload (nullptr, 0, 0, 0, 0, 0);

  auto r = __builtin_rvtt_sfpswap (v0, v1, 0);
  auto dead0 = __builtin_rvtt_sfpselect2 (r, 0);
  auto dead1 = __builtin_rvtt_sfpselect2 (r, 1);
}
/*
**_ZN3tng10cstdead01aEv:
**	SFPLOAD	L0, 0, 0, 0
**	SFPSWAP	L8, L0, 0
**	ret
*/

void cst1 () {
  auto v0 = __builtin_rvtt_sfpload (nullptr, 0, 0, 0, 0, 0);
  auto v1 = __builtin_rvtt_sfpreadlreg (8);

  auto r = __builtin_rvtt_sfpswap (v0, v1, 0);

  v0 = __builtin_rvtt_sfpselect2 (r, 0);
  v1 = __builtin_rvtt_sfpselect2 (r, 1);

  __builtin_rvtt_sfpstore (nullptr, v0, 0, 0, 0, 0, 0);
  __builtin_rvtt_sfpstore (nullptr, v1, 0, 0, 0, 0, 0);
}
/*
**_ZN3tng4cst1Ev:
**	SFPLOAD	L0, 0, 0, 0
**	SFPMOV	L1, L8, 2
**	SFPSWAP	L0, L1, 0
**	SFPSTORE	L0, 0, 0, 0
**	SFPSTORE	L1, 0, 0, 0
**	ret
*/

void cst0 () {
  auto v0 = __builtin_rvtt_sfpreadlreg (8);
  auto v1 = __builtin_rvtt_sfpload (nullptr, 0, 0, 0, 0, 0);

  auto r = __builtin_rvtt_sfpswap (v0, v1, 0);

  v0 = __builtin_rvtt_sfpselect2 (r, 0);
  v1 = __builtin_rvtt_sfpselect2 (r, 1);

  __builtin_rvtt_sfpstore (nullptr, v0, 0, 0, 0, 0, 0);
  __builtin_rvtt_sfpstore (nullptr, v1, 0, 0, 0, 0, 0);
}
/*
**_ZN3tng4cst0Ev:
**	SFPLOAD	L0, 0, 0, 0
**	SFPMOV	L1, L8, 2
**	SFPSWAP	L1, L0, 0
**	SFPSTORE	L1, 0, 0, 0
**	SFPSTORE	L0, 0, 0, 0
**	ret
*/
}
