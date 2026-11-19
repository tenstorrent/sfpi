// { dg-options "-mcpu=tt-bh-tensix -fno-exceptions -fno-rtti -O2 -fno-shrink-wrap" }
// { dg-final { check-function-bodies "**" "" } }


using __v64sf_t [[gnu::vector_size(64 * sizeof(float))]] = float;

namespace cst {
void foo () {
  auto x =  __builtin_rvtt_sfpreadlreg (9);
  auto f = __builtin_rvtt_sfpload (nullptr, 0, 0, 0, 0, 0);
  auto y = __builtin_rvtt_sfpmul (f, x, 0);
  __builtin_rvtt_sfpwritelreg (y, 0);
}
/*
**_ZN3cst3fooEv:
**	SFPLOAD	L0, 0, 0, 0
**	SFPMUL	L0, L0, L9, 0
**	# WRITE L0
**	ret
*/

void bar (int i) {
  auto x =  __builtin_rvtt_sfpreadlreg(9);
  if (i) {
    auto g = __builtin_rvtt_sfpload (nullptr, 0, 0, 0, 0, 0);
    g = __builtin_rvtt_sfpmul (g, g, 0);
    __builtin_rvtt_sfpwritelreg (g, 0);
  }
  auto f = __builtin_rvtt_sfpload (nullptr, 0, 0, 0, 0, 0);
  auto y = __builtin_rvtt_sfpmul (f, x, 0);
  __builtin_rvtt_sfpwritelreg (y, 0);
}
/*
**_ZN3cst3barEi:
**	beq	a0,zero,.L[0-9]+
**	SFPLOAD	L0, 0, 0, 0
**	SFPMUL	L0, L0, L0, 0
**	# WRITE L0
**	SFPLOAD	L0, 0, 0, 0
**	SFPMUL	L0, L0, L9, 0
**	# WRITE L0
**	ret
*/

void baz () {
  auto x =  __builtin_rvtt_sfpreadlreg(9);
  __builtin_rvtt_sfpstore (nullptr, x, 0, 0, 0, 0, 0);
}
/*
**_ZN3cst3bazEv:
**	SFPSTORE	L9, 0, 0, 0
**	ret
*/
void sub5() {
  auto a = __builtin_rvtt_sfpreadlreg(0);
  auto b = __builtin_rvtt_sfpreadlreg(1);

  auto neg1 = __builtin_rvtt_sfpreadlreg (11);
  b = __builtin_rvtt_sfpassign_lv (b, __builtin_rvtt_sfpmad (neg1, a, b, 0));
  b = __builtin_rvtt_sfpassign_lv (b, __builtin_rvtt_sfpmad (neg1, a, b, 0));

  __builtin_rvtt_sfpwritelreg (b, 3);
}
/*
**_ZN3cst4sub5Ev:
**	# READ L0
**	# READ L1
**	SFPMAD	L1, L11, L0, L1, 0
**	SFPMAD	L3, L11, L0, L1, 0
**	# WRITE L3
**	ret
*/

void swap () {
  auto x = __builtin_rvtt_sfpreadlreg(9);
  auto y = __builtin_rvtt_sfpload (nullptr, 0, 0, 0, 0, 0);
  auto r = __builtin_rvtt_sfpswap (x, y, 0);
  x = __builtin_rvtt_sfpselect2 (r, 0);
  y = __builtin_rvtt_sfpselect2 (r, 1);
  auto z = __builtin_rvtt_sfpmul (x, y, 0);
  __builtin_rvtt_sfpwritelreg (z, 1);
}
/*
**_ZN3cst4swapEv:
**	SFPLOAD	L1, 0, 0, 0
**	SFPMOV	L0, L9, 2
**	SFPSWAP	L0, L1, 0
**	SFPMUL	L1, L0, L1, 0
**	# WRITE L1
**	ret
*/
}

namespace vol {
void foo () {
  auto x =  __builtin_rvtt_sfpreadlreg(1);
  auto f = __builtin_rvtt_sfpload (nullptr, 0, 0, 0, 0, 0);
  auto y = __builtin_rvtt_sfpmul (f, x, 0);
  __builtin_rvtt_sfpwritelreg (y, 0);
}
/*
**_ZN3vol3fooEv:
**	# READ L1
**	SFPLOAD	L0, 0, 0, 0
**	SFPMUL	L0, L0, L1, 0
**	# WRITE L0
**	ret
*/

void bar (int i) {
  auto x =  __builtin_rvtt_sfpreadlreg(1);
  if (i) {
    auto g = __builtin_rvtt_sfpload (nullptr, 0, 0, 0, 0, 0);
    g = __builtin_rvtt_sfpmul (g, g, 0);
    __builtin_rvtt_sfpwritelreg (g, 0);
  }
  auto f = __builtin_rvtt_sfpload (nullptr, 0, 0, 0, 0, 0);
  auto y = __builtin_rvtt_sfpmul (f, x, 0);
  __builtin_rvtt_sfpwritelreg (y, 0);
}
/*
**_ZN3vol3barEi:
**	# READ L1
**	beq	a0,zero,.L[0-9]+
**	SFPLOAD	L0, 0, 0, 0
**	SFPMUL	L0, L0, L0, 0
**	# WRITE L0
**	SFPLOAD	L0, 0, 0, 0
**	SFPMUL	L0, L0, L1, 0
**	# WRITE L0
**	ret
*/
}

namespace lv {
void foo () {
  auto x =  __builtin_rvtt_sfpreadlreg(9);
  auto f = __builtin_rvtt_sfpload_lv (nullptr, x, 0, 0, 0, 0, 0);
  auto y = __builtin_rvtt_sfpmul (f, x, 0);
  __builtin_rvtt_sfpwritelreg (y, 0);
}
/*
**_ZN2lv3fooEv:
**	SFPMOV	L0, L9, 2
**	SFPLOAD	L0, 0, 0, 0	# LV:L0
**	SFPMUL	L0, L0, L9, 0
**	# WRITE L0
**	ret
*/

void bar (int i) {
  auto x =  __builtin_rvtt_sfpreadlreg(9);
  if (i) {
    auto g = __builtin_rvtt_sfpload (nullptr, 0, 0, 0, 0, 0);
    g = __builtin_rvtt_sfpmul_lv (x, g, g, 0);
    __builtin_rvtt_sfpwritelreg (g, 1);
  }
  auto f = __builtin_rvtt_sfpload (nullptr, 0, 0, 0, 0, 0);
  auto y = __builtin_rvtt_sfpmul_lv (x, f, x, 0);
  __builtin_rvtt_sfpwritelreg (y, 1);
}
/*
**_ZN2lv3barEi:
**	beq	a0,zero,.L[0-9]+
**	SFPLOAD	L0, 0, 0, 0
**	SFPMOV	L1, L9, 2
**	SFPMUL	L1, L0, L0, 0	# LV:L1
**	# WRITE L1
**	SFPLOAD	L0, 0, 0, 0
**	SFPMOV	L1, L9, 2
**	SFPMUL	L1, L0, L1, 0	# LV:L1
**	# WRITE L1
**	ret
*/
}

int frob ();

void loop (int i) {
  auto x = __builtin_rvtt_sfpreadlreg (9);

#pragma GCC unroll(0)
  while (i--) {
    if (frob ())
      {
	auto r = __builtin_rvtt_sfpreadlreg (10);
	r = __builtin_rvtt_sfpmul (r, r, 0);
	__builtin_rvtt_sfpwritelreg (r, 0);
      }
  
    auto y = __builtin_rvtt_sfpmul (x, x, 0);
    __builtin_rvtt_sfpwritelreg (y, 0);
  }
}
/*
**_Z4loopi:
** 	addi	sp,sp,-16
**	sw	ra,12\(sp\)
**	sw	s0,8\(sp\)
**	beq	a0,zero,.L[0-9]+
**	mv	s0,a0
**	call	_Z4frobv
**	beq	a0,zero,.L[0-9]+
**	SFPMUL	L0, L10, L10, 0
**	# WRITE L0
**	SFPMUL	L0, L9, L9, 0
**	# WRITE L0
**	addi	s0,s0,-1
**	bne	s0,zero,.L[0-9]+
**	lw	ra,12\(sp\)
**	lw	s0,8\(sp\)
**	addi	sp,sp,16
**	jr	ra
*/
