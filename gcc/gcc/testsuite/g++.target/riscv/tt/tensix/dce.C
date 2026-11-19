// { dg-options "-mcpu=tt-bh-tensix -O2 -fno-exceptions -fno-rtti" }
// { dg-final { check-function-bodies "**" "" } }

void loop (unsigned ix) {
  auto a = __builtin_rvtt_sfpreadlreg (0);
  auto b = __builtin_rvtt_sfpreadlreg (1);

  while (ix--)
    {
      a = __builtin_rvtt_sfpmul (a, a, 0);
      b = __builtin_rvtt_sfpmul (b, b, 0);
    }

  __builtin_rvtt_sfpwritelreg (a, 0);
}
/*
**_Z4loopj:
**	# READ L0
**	beq	a0,zero,.L2
**	addi	a0,a0,-1
**	li	a5,-1
**	SFPMUL	L0, L0, L0, 0
**	addi	a0,a0,-1
**	bne	a0,a5,.L3
**	# WRITE L0
**	ret
*/

void warn () {
  __builtin_rvtt_sfpwritelreg (__builtin_rvtt_sfpreadlreg (1), 1);
  __builtin_rvtt_sfpreadlreg (9);
}
/*
**_Z4warnv:
**	# READ L1
**	# WRITE L1
**	ret
*/
