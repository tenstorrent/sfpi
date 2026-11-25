// { dg-options "-mcpu=tt-bh-tensix -fno-exceptions -fno-rtti -O2" }
// { dg-final { check-function-bodies "**" "" } }

void one ()
{
  __builtin_rvtt_ttreplay (nullptr, 0, 4, 0, 0, 0, 0);
  auto x = __builtin_rvtt_sfpload (0, 0, 0, 0, 0, 0);
  x = __builtin_rvtt_sfpmul (x, x, 0);
  x = __builtin_rvtt_sfpmul (x, x, 0);
  x = __builtin_rvtt_sfpmul (x, x, 0);
  x = __builtin_rvtt_sfpmul (x, x, 0);
  x = __builtin_rvtt_sfpmul (x, x, 0);
  x = __builtin_rvtt_sfpmul (x, x, 0);
  x = __builtin_rvtt_sfpmul (x, x, 0);
  x = __builtin_rvtt_sfpmul (x, x, 0);
  x = __builtin_rvtt_sfpmul (x, x, 0);
  x = __builtin_rvtt_sfpmul (x, x, 0);
  x = __builtin_rvtt_sfpmul (x, x, 0);
  __builtin_rvtt_sfpstore (0, x, 0, 0, 0, 0, 0);
}
/*
**_Z3onev:
**	TTREPLAY	0, 4, 0, 0
**	SFPLOAD	L0, 0, 0, 0
**	TTREPLAY	4, 5, 1, 1
**	SFPMUL	L0, L0, L0, 0
**	SFPMUL	L0, L0, L0, 0
**	SFPMUL	L0, L0, L0, 0
**	SFPMUL	L0, L0, L0, 0
**	SFPMUL	L0, L0, L0, 0
**	TTREPLAY	4, 5, 0, 0
**	SFPMUL	L0, L0, L0, 0
**	SFPSTORE	L0, 0, 0, 0
**	ret
*/

void two ()
{
  __builtin_rvtt_ttreplay (nullptr, 0, 4, 0, 0, 0, 0);
  {
    auto x = __builtin_rvtt_sfpload (0, 0, 0, 0, 0, 0);
    x = __builtin_rvtt_sfpmul (x, x, 0);
    x = __builtin_rvtt_sfpmul (x, x, 0);
    x = __builtin_rvtt_sfpmul (x, x, 0);
    x = __builtin_rvtt_sfpmul (x, x, 0);
    __builtin_rvtt_sfpstore (0, x, 0, 0, 0, 0, 0);
  }
  __builtin_rvtt_ttreplay (nullptr, 0, 4, 0, 0, 0, 0);
  {
    auto x = __builtin_rvtt_sfpload (0, 0, 0, 0, 0, 0);
    x = __builtin_rvtt_sfpmul (x, x, 0);
    x = __builtin_rvtt_sfpmul (x, x, 0);
    x = __builtin_rvtt_sfpmul (x, x, 0);
    x = __builtin_rvtt_sfpmul (x, x, 0);
    __builtin_rvtt_sfpstore (0, x, 0, 0, 0, 0, 0);
  }
}
/*
**_Z3twov:
**	TTREPLAY	0, 4, 0, 0
**	TTREPLAY	4, 6, 1, 1
**	SFPLOAD	L0, 0, 0, 0
**	SFPMUL	L0, L0, L0, 0
**	SFPMUL	L0, L0, L0, 0
**	SFPMUL	L0, L0, L0, 0
**	SFPMUL	L0, L0, L0, 0
**	SFPSTORE	L0, 0, 0, 0
**	TTREPLAY	0, 4, 0, 0
**	TTREPLAY	4, 6, 0, 0
**	ret
*/
