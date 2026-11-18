// { dg-options "-mcpu=tt-bh-tensix -O2 -fno-exceptions -fno-rtti" }
// { dg-final { check-function-bodies "**" "" } }

namespace gt {

void foo ()
{
  auto a = __builtin_rvtt_sfpreadlreg (8);
  auto b = __builtin_rvtt_sfpreadlreg (9);

  auto c = __builtin_rvtt_sfpgt (a, b, 8);
  __builtin_rvtt_sfpwritelreg (c, 3);
}
/*
**_ZN2gt3fooEv:
**	SFPMOV	L3, L8, 2
**	SFPGT	L3, L9, 0, 8
**	# WRITE L3
**	ret
*/

void bar ()
{
  auto a = __builtin_rvtt_sfpreadlreg (8);
  auto b = __builtin_rvtt_sfpreadlreg (9);

  __builtin_rvtt_sfppushc (0);
  auto c = __builtin_rvtt_sfpgt (a, b, 1);
  __builtin_rvtt_sfppopc (0);
  __builtin_rvtt_sfpwritelreg (a, 3);
}
/*
**_ZN2gt3barEv:
**	SFPMOV	L3, L8, 2
**	SFPGT	L8, L9, 0, 1
**	SFPENCC	3, 10
**	# WRITE L3
**	ret
*/

void baz ()
{
  auto a = __builtin_rvtt_sfpreadlreg (8);
  auto b = __builtin_rvtt_sfpreadlreg (9);

  __builtin_rvtt_sfppushc (0);
  auto c = __builtin_rvtt_sfpgt (a, b, 8 + 1);
  __builtin_rvtt_sfppopc (0);
}
/*
**_ZN2gt3bazEv:
**	SFPGT	L8, L9, 0, 1
**	SFPENCC	3, 10
**	ret
*/
}

namespace le {

void foo ()
{
  auto a = __builtin_rvtt_sfpreadlreg (8);
  auto b = __builtin_rvtt_sfpreadlreg (9);

  auto c = __builtin_rvtt_sfple (a, b, 8);
  __builtin_rvtt_sfpwritelreg (c, 3);
}
/*
**_ZN2le3fooEv:
**	SFPMOV	L3, L8, 2
**	SFPLE	L3, L9, 0, 8
**	# WRITE L3
**	ret
*/

void bar ()
{
  auto a = __builtin_rvtt_sfpreadlreg (8);
  auto b = __builtin_rvtt_sfpreadlreg (9);

  __builtin_rvtt_sfppushc (0);
  auto c = __builtin_rvtt_sfple (a, b, 1);
  __builtin_rvtt_sfppopc (0);
  __builtin_rvtt_sfpwritelreg (c, 3);
}
/*
**_ZN2le3barEv:
**	SFPMOV	L3, L8, 2
**	SFPLE	L3, L9, 0, 9
**	SFPENCC	3, 10
**	# WRITE L3
**	ret
*/

void baz ()
{
  auto a = __builtin_rvtt_sfpreadlreg (8);
  auto b = __builtin_rvtt_sfpreadlreg (9);

  __builtin_rvtt_sfppushc (0);
  auto c = __builtin_rvtt_sfple (a, b, 8 + 1);
  __builtin_rvtt_sfppopc (0);
  auto d = __builtin_rvtt_sfpiadd_v (c, c, 4);
}
/*
**_ZN2le3bazEv:
**	SFPLE	L8, L9, 0, 1
**	SFPENCC	3, 10
**	ret
*/

void toto ()
{
  auto a = __builtin_rvtt_sfpreadlreg (8);
  auto b = __builtin_rvtt_sfpreadlreg (9);

  auto c = __builtin_rvtt_sfple (a, b, 0);
}
/*
**_ZN2le4totoEv:
**	ret
*/
}
