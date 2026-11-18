// { dg-options "-mcpu=tt-wh-tensix -fno-exceptions -fno-rtti -O2" }
// { dg-final { check-function-bodies "**" "" } }

void one ()
{
  __builtin_rvtt_ttreplay (nullptr, 4, 0, 0, 0, 1, 1);
  auto x = __builtin_rvtt_sfpload (0, 0, 0, 0, 0, 0);
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
**	TTREPLAY	0, 4, 1, 1
**	SFPLOAD	L0, 0, 0, 0
**	SFPMUL	L0, L0, L0, 0
**	SFPNOP
**	SFPMUL	L0, L0, L0, 0
**	TTREPLAY	4, 4, 1, 1
**	SFPNOP
**	SFPMUL	L0, L0, L0, 0
**	SFPNOP
**	SFPMUL	L0, L0, L0, 0
**	TTREPLAY	4, 4, 0, 0
**	SFPNOP
**	SFPSTORE	L0, 0, 0, 0
**	ret
*/

void two (volatile unsigned *ptr)
{
  if (*ptr)
    {
      __builtin_rvtt_ttreplay (nullptr, 2, 0, 0, 2, 1, 1);
      __builtin_rvtt_sfpnop ();
      __builtin_rvtt_sfpnop ();
    }

  auto x = __builtin_rvtt_sfpload (0, 0, 0, 0, 0, 0);
  x = __builtin_rvtt_sfpmul (x, x, 0);
  x = __builtin_rvtt_sfpmul (x, x, 0);
  x = __builtin_rvtt_sfpmul (x, x, 0);
  (*ptr)++;
  x = __builtin_rvtt_sfpmul (x, x, 0);
  x = __builtin_rvtt_sfpmul (x, x, 0);
  x = __builtin_rvtt_sfpmul (x, x, 0);
  __builtin_rvtt_sfpstore (0, x, 0, 0, 0, 0, 0);
}
/*
**_Z3twoPVj:
**	lw	a5,0\(a0\)
**	beq	a5,zero,.L[0-9]+
**	TTREPLAY	2, 2, 1, 1
**	SFPNOP
**	SFPNOP
**	SFPLOAD	L0, 0, 0, 0
**	TTREPLAY	4, 6, 1, 1
**	SFPMUL	L0, L0, L0, 0
**	SFPNOP
**	SFPMUL	L0, L0, L0, 0
**	SFPNOP
**	SFPMUL	L0, L0, L0, 0
**	SFPNOP
**	lw	a5,0\(a0\)
**	addi	a5,a5,1
**	sw	a5,0\(a0\)
**	TTREPLAY	4, 6, 0, 0
**	SFPSTORE	L0, 0, 0, 0
**	ret
*/

void three (volatile unsigned *ptr)
{
  if (*ptr)
    {
      __builtin_rvtt_ttreplay (nullptr, 4, 0, 0, 2, 1, 1);
      __builtin_rvtt_sfpnop ();
    }

  auto x = __builtin_rvtt_sfpload (0, 0, 0, 0, 0, 0);
  x = __builtin_rvtt_sfpmul (x, x, 0);
  x = __builtin_rvtt_sfpmul (x, x, 0);
  x = __builtin_rvtt_sfpmul (x, x, 0);
  x = __builtin_rvtt_sfpmul (x, x, 0);
  x = __builtin_rvtt_sfpmul (x, x, 0);
  x = __builtin_rvtt_sfpmul (x, x, 0);
  __builtin_rvtt_sfpstore (0, x, 0, 0, 0, 0, 0);
}
/*
**_Z5threePVj:
**	lw	a5,0\(a0\)
**	beq	a5,zero,.L9
**	TTREPLAY	2, 4, 1, 1
**	SFPNOP
**	SFPLOAD	L0, 0, 0, 0
**	SFPMUL	L0, L0, L0, 0
**	SFPNOP
**	SFPMUL	L0, L0, L0, 0
**	SFPNOP
**	SFPMUL	L0, L0, L0, 0
**	SFPNOP
**	SFPMUL	L0, L0, L0, 0
**	SFPNOP
**	SFPMUL	L0, L0, L0, 0
**	SFPNOP
**	SFPMUL	L0, L0, L0, 0
**	SFPNOP
**	SFPSTORE	L0, 0, 0, 0
**	ret
*/
