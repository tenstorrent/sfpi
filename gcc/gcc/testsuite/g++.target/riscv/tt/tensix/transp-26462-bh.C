// { dg-options "-mcpu=tt-bh-tensix -fno-exceptions -fno-rtti -O2" }
// { dg-final { check-function-bodies "**" "" } }

namespace tng {
void one ()
{
  auto v0 = __builtin_rvtt_sfpload (nullptr, 0, 0, 0, 0, 0);
  auto v1 = __builtin_rvtt_sfpload (nullptr, 0, 0, 0, 0, 0);
  auto v2 = __builtin_rvtt_sfpload (nullptr, 0, 0, 0, 0, 0);
  auto v3 = __builtin_rvtt_sfpload (nullptr, 0, 0, 0, 0, 0);
  auto v4 = __builtin_rvtt_sfpload (nullptr, 0, 0, 0, 0, 0);
  auto v5 = __builtin_rvtt_sfpload (nullptr, 0, 0, 0, 0, 0);
  auto v6 = __builtin_rvtt_sfpload (nullptr, 0, 0, 0, 0, 0);
  auto v7 = __builtin_rvtt_sfpload (nullptr, 0, 0, 0, 0, 0);

  auto r = __builtin_rvtt_sfptransp (v0, v1, v2, v3, v4, v5, v6, v7);
  v0 = __builtin_rvtt_sfpselect8 (r, 0);
  v1 = __builtin_rvtt_sfpselect8 (r, 1);
  v2 = __builtin_rvtt_sfpselect8 (r, 2);
  v3 = __builtin_rvtt_sfpselect8 (r, 3);
  v4 = __builtin_rvtt_sfpselect8 (r, 4);
  v5 = __builtin_rvtt_sfpselect8 (r, 5);
  v6 = __builtin_rvtt_sfpselect8 (r, 6);
  v7 = __builtin_rvtt_sfpselect8 (r, 7);

  __builtin_rvtt_sfpstore (nullptr, v0, 0, 0, 0, 0, 0);
  __builtin_rvtt_sfpstore (nullptr, v1, 0, 0, 0, 0, 0);
  __builtin_rvtt_sfpstore (nullptr, v2, 0, 0, 0, 0, 0);
  __builtin_rvtt_sfpstore (nullptr, v3, 0, 0, 0, 0, 0);
  __builtin_rvtt_sfpstore (nullptr, v4, 0, 0, 0, 0, 0);
  __builtin_rvtt_sfpstore (nullptr, v5, 0, 0, 0, 0, 0);
  __builtin_rvtt_sfpstore (nullptr, v6, 0, 0, 0, 0, 0);
  __builtin_rvtt_sfpstore (nullptr, v7, 0, 0, 0, 0, 0);
}
/*
**_ZN3tng3oneEv:
**	SFPLOAD	L0, 0, 0, 0
**	SFPLOAD	L1, 0, 0, 0
**	SFPLOAD	L2, 0, 0, 0
**	SFPLOAD	L3, 0, 0, 0
**	SFPLOAD	L4, 0, 0, 0
**	SFPLOAD	L5, 0, 0, 0
**	SFPLOAD	L6, 0, 0, 0
**	SFPLOAD	L7, 0, 0, 0
**	SFPTRANSP
**	SFPSTORE	L0, 0, 0, 0
**	SFPSTORE	L1, 0, 0, 0
**	SFPSTORE	L2, 0, 0, 0
**	SFPSTORE	L3, 0, 0, 0
**	SFPSTORE	L4, 0, 0, 0
**	SFPSTORE	L5, 0, 0, 0
**	SFPSTORE	L6, 0, 0, 0
**	SFPSTORE	L7, 0, 0, 0
**	ret
*/

void two ()
{
  auto v0 = __builtin_rvtt_sfpload (nullptr, 0, 0, 0, 0, 0);
  auto v1 = __builtin_rvtt_sfpload (nullptr, 0, 0, 0, 0, 0);
  auto v2 = __builtin_rvtt_sfpload (nullptr, 0, 0, 0, 0, 0);
  auto v3 = __builtin_rvtt_sfpload (nullptr, 0, 0, 0, 0, 0);

  auto r = __builtin_rvtt_sfptransp (v0, v1, v2, v3, v0, v1, v2, v3);
  v0 = __builtin_rvtt_sfpselect8 (r, 0);
  v1 = __builtin_rvtt_sfpselect8 (r, 1);

  __builtin_rvtt_sfpstore (nullptr, v0, 0, 0, 0, 0, 0);
  __builtin_rvtt_sfpstore (nullptr, v1, 0, 0, 0, 0, 0);
}
/*
**_ZN3tng3twoEv:
**	SFPLOAD	L0, 0, 0, 0
**	SFPLOAD	L1, 0, 0, 0
**	SFPLOAD	L2, 0, 0, 0
**	SFPLOAD	L3, 0, 0, 0
**	SFPMOV	L4, L0, 2
**	SFPMOV	L5, L1, 2
**	SFPMOV	L6, L2, 2
**	SFPMOV	L7, L3, 2
**	SFPTRANSP
**	SFPSTORE	L0, 0, 0, 0
**	SFPSTORE	L1, 0, 0, 0
**	ret
*/

void three ()
{
  auto v0 = __builtin_rvtt_sfpreadlreg (8);
  auto v1 = __builtin_rvtt_sfpload (nullptr, 0, 0, 0, 0, 0);
  auto v2 = __builtin_rvtt_sfpreadlreg (8);
  auto v3 = __builtin_rvtt_sfpload (nullptr, 0, 0, 0, 0, 0);

  auto r = __builtin_rvtt_sfptransp (v0, v1, v2, v3, v0, v1, v2, v3);
  v0 = __builtin_rvtt_sfpselect8 (r, 0);
  v1 = __builtin_rvtt_sfpselect8 (r, 1);
  v2 = __builtin_rvtt_sfpselect8 (r, 2);
  v3 = __builtin_rvtt_sfpselect8 (r, 3);

  __builtin_rvtt_sfpstore (nullptr, v0, 0, 0, 0, 0, 0);
  __builtin_rvtt_sfpstore (nullptr, v1, 0, 0, 0, 0, 0);
  __builtin_rvtt_sfpstore (nullptr, v2, 0, 0, 0, 0, 0);
  __builtin_rvtt_sfpstore (nullptr, v3, 0, 0, 0, 0, 0);
}
/*
**_ZN3tng5threeEv:
**	SFPLOAD	L1, 0, 0, 0
**	SFPLOAD	L3, 0, 0, 0
**	SFPMOV	L5, L1, 2
**	SFPMOV	L7, L3, 2
**	SFPMOV	L0, L8, 2
**	SFPMOV	L2, L0, 2
**	SFPMOV	L4, L0, 2
**	SFPMOV	L6, L0, 2
**	SFPTRANSP
**	SFPSTORE	L0, 0, 0, 0
**	SFPSTORE	L1, 0, 0, 0
**	SFPSTORE	L2, 0, 0, 0
**	SFPSTORE	L3, 0, 0, 0
**	ret
*/
}

