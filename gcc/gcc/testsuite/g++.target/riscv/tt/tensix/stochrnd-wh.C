// { dg-options "-mcpu=tt-wh-tensix -O2 -fno-exceptions -fno-rtti" }
// { dg-final { check-function-bodies "**" "" } }

void foo () {
    auto a = __builtin_rvtt_sfpreadlreg (0);
    auto r = __builtin_rvtt_sfpstochrnd_descale_i (nullptr, a, 0x1f, 0, 0, 4, 1);
    __builtin_rvtt_sfpwritelreg (r, 0);
}
/*
**_Z3foov:
**	# READ L0
**	SFPSTOCHRND	L0, L9, L0, 31, 12, 1
**	# WRITE L0
**	ret
*/
void bar () {
    auto a = __builtin_rvtt_sfpreadlreg (0);
    auto b = __builtin_rvtt_sfpreadlreg (1);
    auto r = __builtin_rvtt_sfpstochrnd_descale_v (b, a, 5, 1);
    __builtin_rvtt_sfpwritelreg (r, 0);
}
/*
**_Z3barv:
**	# READ L0
**	# READ L1
**	SFPSTOCHRND	L0, L0, L1, 0, 5, 1
**	# WRITE L0
**	ret
*/
