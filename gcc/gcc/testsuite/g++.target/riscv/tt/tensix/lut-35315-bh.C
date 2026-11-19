// { dg-options "-mcpu=tt-bh-tensix -fno-exceptions -fno-rtti -O2 -fno-shrink-wrap" }
// { dg-final { check-function-bodies "**" "" } }

void one ()
{
  auto v0 = __builtin_rvtt_sfpload (nullptr, 0, 0, 0, 0, 0);
  auto v1 = __builtin_rvtt_sfpload (nullptr, 0, 0, 0, 0, 0);
  auto v2 = __builtin_rvtt_sfpload (nullptr, 0, 0, 0, 0, 0);
  auto v3 = __builtin_rvtt_sfpload (nullptr, 0, 0, 0, 0, 0);
  auto r = __builtin_rvtt_sfplut (v0, v1, v2, v3, 0);
  __builtin_rvtt_sfpstore (nullptr, r, 0, 0, 0, 0, 0);
}
/*
**_Z3onev:
**	SFPLOAD	L0, 0, 0, 0
**	SFPLOAD	L1, 0, 0, 0
**	SFPLOAD	L2, 0, 0, 0
**	SFPLOAD	L3, 0, 0, 0
**	SFPLUT	L3, 0	# R:L0,L1,L2,L3
**	SFPSTORE	L3, 0, 0, 0
**	ret
*/

void two ()
{
  auto v0 = __builtin_rvtt_sfpload (nullptr, 0, 0, 0, 0, 0);
  auto v1 = __builtin_rvtt_sfpload (nullptr, 0, 0, 0, 0, 0);
  auto v2 = __builtin_rvtt_sfpload (nullptr, 0, 0, 0, 0, 0);
  auto v3 = __builtin_rvtt_sfpload (nullptr, 0, 0, 0, 0, 0);
  auto v4 = __builtin_rvtt_sfpload (nullptr, 0, 0, 0, 0, 0);
  auto v5 = __builtin_rvtt_sfpload (nullptr, 0, 0, 0, 0, 0);
  auto v6 = __builtin_rvtt_sfpload (nullptr, 0, 0, 0, 0, 0);
  auto r = __builtin_rvtt_sfplutfp32_6r (v0, v1, v2, v3, v4, v5, v6, 2);
  __builtin_rvtt_sfpstore (nullptr, r, 0, 0, 0, 0, 0);
}
/*
**_Z3twov:
**	SFPLOAD	L0, 0, 0, 0
**	SFPLOAD	L1, 0, 0, 0
**	SFPLOAD	L2, 0, 0, 0
**	SFPLOAD	L4, 0, 0, 0
**	SFPLOAD	L5, 0, 0, 0
**	SFPLOAD	L6, 0, 0, 0
**	SFPLOAD	L3, 0, 0, 0
**	SFPLUTFP32	L0, 2	# R:L0,L1,L2,L4,L5,L6,L3
**	SFPSTORE	L0, 0, 0, 0
**	ret
*/

void three ()
{
  auto v0 = __builtin_rvtt_sfpload (nullptr, 0, 0, 0, 0, 0);
  auto v1 = __builtin_rvtt_sfpload (nullptr, 0, 0, 0, 0, 0);
  auto v2 = __builtin_rvtt_sfpload (nullptr, 0, 0, 0, 0, 0);
  auto v3 = __builtin_rvtt_sfpload (nullptr, 0, 0, 0, 0, 0);
  auto r = __builtin_rvtt_sfplutfp32_3r (v0, v1, v2, v3, 10);
  __builtin_rvtt_sfpstore (nullptr, r, 0, 0, 0, 0, 0);
}
/*
**_Z5threev:
**	SFPLOAD	L0, 0, 0, 0
**	SFPLOAD	L1, 0, 0, 0
**	SFPLOAD	L2, 0, 0, 0
**	SFPLOAD	L3, 0, 0, 0
**	SFPLOADI	L7, 0, 2
**	SFPLUTFP32	L0, 10	# R:L0,L1,L2,L3,L7
**	SFPSTORE	L0, 0, 0, 0
**	ret
*/

void four ()
{
#pragma GCC unroll 4
  for (unsigned ix = 0; ix != 4; ix++)
    {
      auto v0 = __builtin_rvtt_sfpload (nullptr, 0, 0, 0, 0, 0);
      auto v1 = __builtin_rvtt_sfpload (nullptr, 0, 0, 0, 0, 0);
      auto v2 = __builtin_rvtt_sfpload (nullptr, 0, 0, 0, 0, 0);
      auto v3 = __builtin_rvtt_sfpload (nullptr, 0, 0, 0, 0, 0);
      auto r = __builtin_rvtt_sfplutfp32_3r (v0, v1, v2, v3, 10);
      __builtin_rvtt_sfpstore (nullptr, r, 0, 0, 0, 0, 0);
    }
}
/*
**_Z4fourv:
**	TTREPLAY	0, 7, 1, 1
**	SFPLOAD	L0, 0, 0, 0
**	SFPLOAD	L1, 0, 0, 0
**	SFPLOAD	L2, 0, 0, 0
**	SFPLOAD	L3, 0, 0, 0
**	SFPLOADI	L7, 0, 2
**	SFPLUTFP32	L0, 10	# R:L0,L1,L2,L3,L7
**	SFPSTORE	L0, 0, 0, 0
**	TTREPLAY	0, 7, 0, 0
**	TTREPLAY	0, 7, 0, 0
**	TTREPLAY	0, 7, 0, 0
**	ret
*/
