// { dg-options "-mcpu=tt-bh-tensix -fno-exceptions -fno-rtti -O2" }
// { dg-final { check-function-bodies "**" "" } }

extern volatile unsigned long iptr[];

void zero ()
{
  auto val = __builtin_rvtt_sfpreadlreg (0);
  val =  __builtin_rvtt_sfpsetman_i (nullptr, val, 4, 0, 0, 0);
  __builtin_rvtt_sfpwritelreg (val, 0);
}
/*
**_Z4zerov:
**	# READ L0
**	SFPSETMAN	L0, L0, 4, 1
**	# WRITE L0
**	ret
*/

void one ()
{
  auto val = __builtin_rvtt_sfpreadlreg (0);
  val =  __builtin_rvtt_sfpsetman_i (nullptr, val, 0x7fffff, 0, 0, 0);
  __builtin_rvtt_sfpwritelreg (val, 0);
}
/*
**_Z3onev:
**	# READ L0
**	SFPLOADI	L1, 65535, 2
**	SFPLOADI	L1, 127, 8	# LV:L1
**	SFPSETMAN	L1, L0, 0, 0
**	SFPMOV	L0, L1, 2
**	# WRITE L0
**	ret
*/

void two ()
{
  auto val = __builtin_rvtt_sfpreadlreg (0);
  auto cst = __builtin_rvtt_sfpxloadi (nullptr, 4, 0, 0, 16);
  val =  __builtin_rvtt_sfpsetman_v (val, cst, 0);
  __builtin_rvtt_sfpwritelreg (val, 0);
}
/*
**_Z3twov:
**	# READ L0
**	SFPSETMAN	L0, L0, 4, 1
**	# WRITE L0
**	ret
*/

void three ()
{
  auto val = __builtin_rvtt_sfpreadlreg (0);
  auto cst = __builtin_rvtt_sfpxloadi (nullptr, 0, 0, 0, 16);
  val =  __builtin_rvtt_sfpsetman_v (val, cst, 0);
  __builtin_rvtt_sfpwritelreg (val, 0);
}
/*
**_Z5threev:
**	# READ L0
**	SFPSETMAN	L0, L0, 0, 1
**	# WRITE L0
**	ret
*/

void four ()
{
  auto val = __builtin_rvtt_sfpreadlreg (0);
  auto cst = __builtin_rvtt_sfpreadlreg (9);
  val =  __builtin_rvtt_sfpsetman_v (val, cst, 0);
  __builtin_rvtt_sfpwritelreg (val, 0);
}
/*
**_Z4fourv:
**	# READ L0
**	SFPSETMAN	L0, L0, 0, 1
**	# WRITE L0
**	ret
*/

void five ()
{
  auto val = __builtin_rvtt_sfpreadlreg (0);
  val =  __builtin_rvtt_sfpiadd_i (nullptr, val, 4, 0, 0, 4);
  __builtin_rvtt_sfpwritelreg (val, 0);
}  
/*
**_Z4fivev:
**	# READ L0
**	SFPIADD	L0, L0, 4, 5
**	# WRITE L0
**	ret
*/

void five0 ()
{
  auto val = __builtin_rvtt_sfpreadlreg (0);
  val =  __builtin_rvtt_sfpiadd_i (nullptr, val, 0x7ff, 0, 0, 4);
  __builtin_rvtt_sfpwritelreg (val, 0);
}  
/*
**_Z5five0v:
**	# READ L0
**	SFPIADD	L0, L0, 2047, 5
**	# WRITE L0
**	ret
*/

void five1 ()
{
  auto val = __builtin_rvtt_sfpreadlreg (0);
  val =  __builtin_rvtt_sfpiadd_i (nullptr, val, -0x800, 0, 0, 4);
  __builtin_rvtt_sfpwritelreg (val, 0);
}  
/*
**_Z5five1v:
**	# READ L0
**	SFPIADD	L0, L0, -2048, 5
**	# WRITE L0
**	ret
*/

void six ()
{
  auto val = __builtin_rvtt_sfpreadlreg (0);
  auto cst = __builtin_rvtt_sfpreadlreg (9);
  val =  __builtin_rvtt_sfpiadd_v (val, cst, 4);
  __builtin_rvtt_sfpwritelreg (val, 0);
}  
/*
**_Z3sixv:
**	# READ L0
**	# WRITE L0
**	ret
*/

void seven ()
{
  __builtin_rvtt_sfppushc (0);
  auto val = __builtin_rvtt_sfpreadlreg (0);
  auto cst = __builtin_rvtt_sfpreadlreg (9);
  val =  __builtin_rvtt_sfpiadd_v (val, cst, 0);
  __builtin_rvtt_sfpwritelreg (val, 0);
  __builtin_rvtt_sfppopc (0);
}  
/*
**_Z5sevenv:
**	# READ L0
**	SFPIADD	L0, L0, 0, 1
**	# WRITE L0
**	SFPENCC	3, 10
**	ret
*/

void eight ()
{
  auto val = __builtin_rvtt_sfpreadlreg (0);
  auto cst = __builtin_rvtt_sfpxloadi (nullptr, 4, 0, 0, 16);
  val =  __builtin_rvtt_sfpiadd_v (cst, val, 6);
  __builtin_rvtt_sfpwritelreg (val, 0);
}  
/*
**_Z5eightv:
**	# READ L0
**	SFPIADD	L0, L0, -4, 5
**	# WRITE L0
**	ret
*/

void float16a () {
  auto a = __builtin_rvtt_sfpxloadi (nullptr, 0x3f804000, 0, 0, -32);
  __builtin_rvtt_sfpwritelreg (a, 0);
}
/*
**_Z8float16av:
**	SFPLOADI	L0, 15362, 1
**	# WRITE L0
**	ret
*/
