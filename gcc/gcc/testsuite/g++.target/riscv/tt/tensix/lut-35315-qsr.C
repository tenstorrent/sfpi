// { dg-options "-mcpu=tt-qsr32-tensix -fno-exceptions -fno-rtti -O2 -fno-shrink-wrap" }
// { dg-final { check-function-bodies "**" "" } }

void one ()
{
  auto v0 = __builtin_rvtt_sfpload (nullptr, 0, 0, 0, 0, 0);
  auto r = __builtin_rvtt_sfplut (v0, 0);
  __builtin_rvtt_sfpstore (nullptr, r, 0, 0, 0, 0, 0);
}
/*
**_Z3onev:
**	SFPLOAD	L3, 0, 0, 0, 0, 0
**	SFPLUT	L3, 0	# R:L3
**	SFPSTORE	L3, 0, 0, 0, 0, 0
**	ret
*/

void two ()
{
  auto v0 = __builtin_rvtt_sfpload (nullptr, 0, 0, 0, 0, 0);
  auto r = __builtin_rvtt_sfplutfp32 (v0, 0);
  __builtin_rvtt_sfpstore (nullptr, r, 0, 0, 0, 0, 0);
}
/*
**_Z3twov:
**	SFPLOAD	L3, 0, 0, 0, 0, 0
**	SFPLUTFP32	L3, 0	# R:L3
**	SFPSTORE	L3, 0, 0, 0, 0, 0
**	ret
*/
