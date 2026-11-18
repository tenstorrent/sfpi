// { dg-options "-mcpu=tt-qsr32-tensix -fno-exceptions -fno-rtti -O2" }
// { dg-final { check-function-bodies "**" "" } }

void one ()
{
  auto x = __builtin_rvtt_sfpload (0, 0, 0, 0, 0, 0);
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
**	SFPLOAD	L0, 0, 0, 0, 0, 0
**	TTREPLAY	0, 4, 0, 0, 0, 1
**	SFPMUL	L0, L0, L0, 0
**	SFPMUL	L0, L0, L0, 0
**	SFPMUL	L0, L0, L0, 0
**	SFPMUL	L0, L0, L0, 0
**	TTREPLAY	0, 4, 0, 0, 0, 0
**	TTREPLAY	0, 4, 0, 0, 0, 0
**	SFPSTORE	L0, 0, 0, 0, 0, 0
**	ret
*/

void two (volatile unsigned *ptr)
{
  auto x = __builtin_rvtt_sfpload (0, 0, 0, 0, 0, 0);
  x = __builtin_rvtt_sfpmul (x, x, 0);
  x = __builtin_rvtt_sfpmul (x, x, 0);
  x = __builtin_rvtt_sfpmul (x, x, 0);
  x = __builtin_rvtt_sfpmul (x, x, 0);
  x = __builtin_rvtt_sfpmul (x, x, 0);
  (*ptr)++;
  x = __builtin_rvtt_sfpmul (x, x, 0);
  x = __builtin_rvtt_sfpmul (x, x, 0);
  x = __builtin_rvtt_sfpmul (x, x, 0);
  x = __builtin_rvtt_sfpmul (x, x, 0);
  x = __builtin_rvtt_sfpmul (x, x, 0);
  __builtin_rvtt_sfpstore (0, x, 0, 0, 0, 0, 0);
}
/*
**_Z3twoPVj:
**	SFPLOAD	L0, 0, 0, 0, 0, 0
**	TTREPLAY	0, 5, 0, 0, 0, 1
**	SFPMUL	L0, L0, L0, 0
**	SFPMUL	L0, L0, L0, 0
**	SFPMUL	L0, L0, L0, 0
**	SFPMUL	L0, L0, L0, 0
**	SFPMUL	L0, L0, L0, 0
**	TTREPLAY	0, 5, 0, 0, 0, 0
**	lw	a5,0\(a0\)
**	addi	a5,a5,1
**	sw	a5,0\(a0\)
**	TTREPLAY	0, 5, 0, 0, 0, 0
**	SFPSTORE	L0, 0, 0, 0, 0, 0
**	ret
*/

void three ()
{
  {
    auto x = __builtin_rvtt_sfpload (0, 0, 0, 0, 0, 0);
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

  {
    auto x = __builtin_rvtt_sfpload (0, 0, 0, 0, 0, 0);
    x = __builtin_rvtt_sfpadd (x, x, 0);
    x = __builtin_rvtt_sfpadd (x, x, 0);
    x = __builtin_rvtt_sfpadd (x, x, 0);
    x = __builtin_rvtt_sfpadd (x, x, 0);
    x = __builtin_rvtt_sfpadd (x, x, 0);
    x = __builtin_rvtt_sfpadd (x, x, 0);
    x = __builtin_rvtt_sfpadd (x, x, 0);
    x = __builtin_rvtt_sfpadd (x, x, 0);
    __builtin_rvtt_sfpstore (0, x, 0, 0, 0, 0, 0);
  }
}
/*
**_Z5threev:
**	SFPLOAD	L0, 0, 0, 0, 0, 0
**	TTREPLAY	0, 4, 0, 0, 0, 1
**	SFPMUL	L0, L0, L0, 0
**	SFPMUL	L0, L0, L0, 0
**	SFPMUL	L0, L0, L0, 0
**	SFPMUL	L0, L0, L0, 0
**	TTREPLAY	0, 4, 0, 0, 0, 0
**	TTREPLAY	0, 4, 0, 0, 0, 0
**	SFPSTORE	L0, 0, 0, 0, 0, 0
**	SFPLOAD	L0, 0, 0, 0, 0, 0
**	TTREPLAY	4, 4, 0, 0, 0, 1
**	SFPADD	L0, L0, L0, 0
**	SFPADD	L0, L0, L0, 0
**	SFPADD	L0, L0, L0, 0
**	SFPADD	L0, L0, L0, 0
**	TTREPLAY	4, 4, 0, 0, 0, 0
**	TTREPLAY	4, 4, 0, 0, 0, 0
**	SFPSTORE	L0, 0, 0, 0, 0, 0
**	ret
*/

void four (volatile unsigned *ptr)
{
  auto x = __builtin_rvtt_sfpload (0, 0, 0, 0, 0, 0);
  x = __builtin_rvtt_sfpmul (x, x, 0);
  x = __builtin_rvtt_sfpmul (x, x, 0);
  x = __builtin_rvtt_sfpmul (x, x, 0);
  x = __builtin_rvtt_sfpmul (x, x, 0);
  (*ptr)++;
  x = __builtin_rvtt_sfpadd (x, x, 0);
  x = __builtin_rvtt_sfpadd (x, x, 0);
  x = __builtin_rvtt_sfpadd (x, x, 0);
  x = __builtin_rvtt_sfpadd (x, x, 0);
  x = __builtin_rvtt_sfpmul (x, x, 0);
  x = __builtin_rvtt_sfpmul (x, x, 0);
  x = __builtin_rvtt_sfpmul (x, x, 0);
  x = __builtin_rvtt_sfpmul (x, x, 0);
  x = __builtin_rvtt_sfpadd (x, x, 0);
  x = __builtin_rvtt_sfpadd (x, x, 0);
  x = __builtin_rvtt_sfpadd (x, x, 0);
  x = __builtin_rvtt_sfpadd (x, x, 0);
  __builtin_rvtt_sfpstore (0, x, 0, 0, 0, 0, 0);
}
/*
**_Z4fourPVj:
**	SFPLOAD	L0, 0, 0, 0, 0, 0
**	TTREPLAY	0, 4, 0, 0, 0, 1
**	SFPMUL	L0, L0, L0, 0
**	SFPMUL	L0, L0, L0, 0
**	SFPMUL	L0, L0, L0, 0
**	SFPMUL	L0, L0, L0, 0
**	TTREPLAY	0, 4, 0, 0, 0, 0
**	lw	a5,0\(a0\)
**	addi	a5,a5,1
**	sw	a5,0\(a0\)
**	TTREPLAY	4, 4, 0, 0, 0, 1
**	SFPADD	L0, L0, L0, 0
**	SFPADD	L0, L0, L0, 0
**	SFPADD	L0, L0, L0, 0
**	SFPADD	L0, L0, L0, 0
**	TTREPLAY	4, 4, 0, 0, 0, 0
**	TTREPLAY	0, 4, 0, 0, 0, 0
**	TTREPLAY	4, 4, 0, 0, 0, 0
**	SFPSTORE	L0, 0, 0, 0, 0, 0
**	ret
*/


void five ()
{
#pragma GCC unroll 8
  for (unsigned ix = 0; ix != 8; ix++)
    {
      auto x = __builtin_rvtt_sfpload (0, 0, 0, 0, 0, 0);
      x = __builtin_rvtt_sfpmul (x, x, 0);
      x = __builtin_rvtt_sfpmul (x, x, 0);
      x = __builtin_rvtt_sfpadd (x, x, 0);
      x = __builtin_rvtt_sfpadd (x, x, 0);
      __builtin_rvtt_sfpstore (0, x, 0, 0, 0, 0, 0);
      __builtin_rvtt_ttincrwc (0, 0, 0, 0);
    }
}
/*
**_Z4fivev:
**	TTREPLAY	0, 7, 0, 0, 0, 1
**	SFPLOAD	L0, 0, 0, 0, 0, 0
**	SFPMUL	L0, L0, L0, 0
**	SFPMUL	L0, L0, L0, 0
**	SFPADD	L0, L0, L0, 0
**	SFPADD	L0, L0, L0, 0
**	SFPSTORE	L0, 0, 0, 0, 0, 0
**	TTINCRWC	0, 0, 0, 0
**	TTREPLAY	0, 7, 0, 0, 0, 0
**	TTREPLAY	0, 7, 0, 0, 0, 0
**	TTREPLAY	0, 7, 0, 0, 0, 0
**	TTREPLAY	0, 7, 0, 0, 0, 0
**	TTREPLAY	0, 7, 0, 0, 0, 0
**	TTREPLAY	0, 7, 0, 0, 0, 0
**	TTREPLAY	0, 7, 0, 0, 0, 0
**	TTREPLAY	0, 7, 0, 0, 0, 0
**	ret
*/

extern volatile unsigned long ibuf[];
void six (unsigned bits)
{
#pragma GCC unroll 2
  for (unsigned ix = 0; ix != 8; ix++)
    {
      auto x = __builtin_rvtt_sfpload (0, 0, 0, 0, 0, 0);
      x = __builtin_rvtt_sfpmul (x, x, 0);
      x = __builtin_rvtt_sfpshft_i (ibuf, x, bits, 0, 0, 0);
      x = __builtin_rvtt_sfpadd (x, x, 0);
      __builtin_rvtt_sfpstore (0, x, 0, 0, 0, 0, 0);
    }
}
/*
**_Z3sixj:
**	li	a5,16773120
**	slli	a0,a0,12
**	and	a0,a0,a5
**	li	a3, 2046820357	# 1:7a000005
**	lui	a4,%hi\(ibuf\)
**	add	a3,a3,a0
**	addi	a4,a4,%lo\(ibuf\)
**	li	a5,8
**	TTREPLAY	0, 5, 0, 0, 0, 1
**	SFPLOAD	L0, 0, 0, 0, 0, 0
**	SFPMUL	L0, L0, L0, 0
**	sw	a3, 0\(a4\)	# 1:SFPSHFT	L0, L0, -, 5
**	SFPADD	L0, L0, L0, 0
**	SFPSTORE	L0, 0, 0, 0, 0, 0
**	TTREPLAY	0, 5, 0, 0, 0, 0
**	TTREPLAY	0, 5, 0, 0, 0, 0
**	addi	a5,a5,-2
**	bne	a5,zero,.L[0-9]+
**	ret
*/
