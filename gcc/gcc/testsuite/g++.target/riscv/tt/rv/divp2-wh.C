// { dg-options "-mcpu=tt-wh-tensix -O2 -fno-exceptions -fno-rtti" }
// { dg-final { check-function-bodies "**" "" } }

void foo () {
    auto a = __builtin_rvtt_sfpreadlreg (0);
    auto r = __builtin_rvtt_sfpdivp2 (nullptr, a, 4, 0, 0, 0);
    __builtin_rvtt_sfpwritelreg (r, 0);
}
/*
**_Z3foov:
**	# READ L0
**	SFPDIVP2	L0, L0, 4, 0
**	# WRITE L0
**	ret
*/
