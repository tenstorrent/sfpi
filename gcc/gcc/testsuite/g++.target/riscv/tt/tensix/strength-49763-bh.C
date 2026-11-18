// { dg-options "-mcpu=tt-bh-tensix -O2 -fno-exceptions -fno-rtti" }
// { dg-final { check-function-bodies "**" "" } }

void mul1 ()
{
  auto a = __builtin_rvtt_sfpreadlreg (0);
  
  a = __builtin_rvtt_sfpmul (a, __builtin_rvtt_sfploadi (nullptr, 0x3f80, 0, 0, 0), 0);
  __builtin_rvtt_sfpwritelreg (a, 0);
}
/*
**_Z4mul1v:
**	# READ L0
**	# WRITE L0
**	ret
*/

void mulneg1 ()
{
  auto a = __builtin_rvtt_sfpreadlreg (0);
  
  a = __builtin_rvtt_sfpmul (a, __builtin_rvtt_sfploadi (nullptr, 0xbf80, 0, 0, 0), 0);
  __builtin_rvtt_sfpwritelreg (a, 0);
}
/*
**_Z7mulneg1v:
**	# READ L0
**	SFPMOV	L0, L0, 1
**	# WRITE L0
**	ret
*/

void mul0 ()
{
  auto a = __builtin_rvtt_sfpreadlreg (0);
  
  a = __builtin_rvtt_sfpmul (a, __builtin_rvtt_sfploadi (nullptr, 0, 0, 0, 0), 0);
  __builtin_rvtt_sfpwritelreg (a, 0);
}
/*
**_Z4mul0v:
**	# READ L0
**	SFPMUL	L0, L0, L9, 0
**	# WRITE L0
**	ret
*/

void add0 ()
{
  auto a = __builtin_rvtt_sfpreadlreg (0);
  
  a = __builtin_rvtt_sfpadd (a, __builtin_rvtt_sfploadi (nullptr, 0, 0, 0, 0), 0);
  __builtin_rvtt_sfpwritelreg (a, 0);
}
/*
**_Z4add0v:
**	# READ L0
**	SFPADD	L0, L0, L9, 0
**	# WRITE L0
**	ret
*/

void negate1 ()
{
  auto pos_one = __builtin_rvtt_sfploadi (nullptr, 0x3f80, 0, 0, 0);
  auto zero = __builtin_rvtt_sfploadi (nullptr, 0, 0, 0, 0);
  auto neg_one = __builtin_rvtt_sfploadi (nullptr, 0xbf80, 0, 0, 0);

  __builtin_rvtt_sfpwritelreg (__builtin_rvtt_sfpmov (pos_one, 1), 0);
  __builtin_rvtt_sfpwritelreg (__builtin_rvtt_sfpmov (zero, 1), 1);
  __builtin_rvtt_sfpwritelreg (__builtin_rvtt_sfpmov (neg_one, 1), 2);
}
/*
**_Z7negate1v:
**	SFPMOV	L0, L11, 2
**	# WRITE L0
**	SFPMOV	L1, L9, 1
**	# WRITE L1
**	SFPMOV	L2, L10, 2
**	# WRITE L2
**	ret
*/

void negate2 ()
{
  auto pos_one = __builtin_rvtt_sfploadi (nullptr, 0x3f80, 0, 0, 0);
  auto zero = __builtin_rvtt_sfploadi (nullptr, 0, 0, 0, 0);
  auto neg_one = __builtin_rvtt_sfploadi (nullptr, 0xbf80, 0, 0, 0);

  __builtin_rvtt_sfpwritelreg (__builtin_rvtt_sfpmul (pos_one, neg_one, 0), 0);
  __builtin_rvtt_sfpwritelreg (__builtin_rvtt_sfpmul (zero, neg_one, 0), 1);
  __builtin_rvtt_sfpwritelreg (__builtin_rvtt_sfpmul (neg_one, neg_one, 0), 2);
}
/*
**_Z7negate2v:
**	SFPMOV	L0, L11, 2
**	# WRITE L0
**	SFPMOV	L1, L9, 1
**	# WRITE L1
**	SFPMOV	L2, L10, 2
**	# WRITE L2
**	ret
*/

// Only one mul input works, and it might be the second one
void commute1 ()
{
  auto a = __builtin_rvtt_sfpreadlreg (0);
  auto b = __builtin_rvtt_sfpreadlreg (1);
  auto c = __builtin_rvtt_sfpreadlreg (2);

  auto p1 = __builtin_rvtt_sfpmul (a, b, 0);
  auto p2 = __builtin_rvtt_sfpmul (c, p1, 0);

  auto neg_p1 = __builtin_rvtt_sfpmov (p1, 1);
  auto sum = __builtin_rvtt_sfpadd (p2, neg_p1, 0);

  __builtin_rvtt_sfpwritelreg (sum, 3);
}
/*
**_Z8commute1v:
**	# READ L0
**	# READ L1
**	# READ L2
**	SFPMUL	L3, L0, L1, 0
**	SFPMAD	L3, L2, L3, L3, 2
**	# WRITE L3
**	ret
*/

// Only one mul input works, and it might be the second one
void commute2 ()
{
  auto a = __builtin_rvtt_sfpreadlreg (0);
  auto b = __builtin_rvtt_sfpreadlreg (1);
  auto c = __builtin_rvtt_sfpreadlreg (2);

  auto p1 = __builtin_rvtt_sfpmul (a, b, 0);
  auto p2 = __builtin_rvtt_sfpmul (c, p1, 0);

  auto neg_p1 = __builtin_rvtt_sfpmov (p1, 1);
  auto sum = __builtin_rvtt_sfpadd (neg_p1, p2, 0);

  __builtin_rvtt_sfpwritelreg (sum, 3);
}
/*
**_Z8commute2v:
**	# READ L0
**	# READ L1
**	# READ L2
**	SFPMUL	L3, L0, L1, 0
**	SFPMAD	L3, L2, L3, L3, 2
**	# WRITE L3
**	ret
*/
