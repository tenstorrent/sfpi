// { dg-options "-mcpu=tt-bh-tensix -O2 -fno-exceptions -fno-rtti" }
// { dg-final { check-function-bodies "**" "" } }

void foo () {
  auto a = __builtin_rvtt_sfpreadlreg (0);

#define P0(V) a = __builtin_rvtt_sfpmuli (nullptr, a, V, 0, 0, 0)
#define P1(V) P0(V); P0(V+1)
#define P2(V) P1(V); P1(V + 2)
#define P3(V) P2(V); P2(V + 4)
#define P4(V) P3(V); P3(V + 8)
#define P5(V) P4(V); P4(V + 16)

  P5(0);  P1(32);
  P5(0);  P1(32);
  P5(0);  P1(32);

  __builtin_rvtt_sfpwritelreg (a, 0);  
}
/*
**_Z3foov:
**	# READ L0
**	TTREPLAY	0, 32, 1, 1
**	SFPMULI	L0, 0, 0
**	SFPMULI	L0, 1, 0
**	SFPMULI	L0, 2, 0
**	SFPMULI	L0, 3, 0
**	SFPMULI	L0, 4, 0
**	SFPMULI	L0, 5, 0
**	SFPMULI	L0, 6, 0
**	SFPMULI	L0, 7, 0
**	SFPMULI	L0, 8, 0
**	SFPMULI	L0, 9, 0
**	SFPMULI	L0, 10, 0
**	SFPMULI	L0, 11, 0
**	SFPMULI	L0, 12, 0
**	SFPMULI	L0, 13, 0
**	SFPMULI	L0, 14, 0
**	SFPMULI	L0, 15, 0
**	SFPMULI	L0, 16, 0
**	SFPMULI	L0, 17, 0
**	SFPMULI	L0, 18, 0
**	SFPMULI	L0, 19, 0
**	SFPMULI	L0, 20, 0
**	SFPMULI	L0, 21, 0
**	SFPMULI	L0, 22, 0
**	SFPMULI	L0, 23, 0
**	SFPMULI	L0, 24, 0
**	SFPMULI	L0, 25, 0
**	SFPMULI	L0, 26, 0
**	SFPMULI	L0, 27, 0
**	SFPMULI	L0, 28, 0
**	SFPMULI	L0, 29, 0
**	SFPMULI	L0, 30, 0
**	SFPMULI	L0, 31, 0
**	SFPMULI	L0, 32, 0
**	SFPMULI	L0, 33, 0
**	TTREPLAY	0, 32, 0, 0
**	SFPMULI	L0, 32, 0
**	SFPMULI	L0, 33, 0
**	TTREPLAY	0, 32, 0, 0
**	SFPMULI	L0, 32, 0
**	SFPMULI	L0, 33, 0
**	# WRITE L0
**	ret
*/
