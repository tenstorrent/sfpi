// { dg-options "-mcpu=tt-bh-tensix -fno-exceptions -fno-rtti -O2 -fno-shrink-wrap" }
// { dg-final { check-function-bodies "**" "" } }

void foo () {
  auto a =  __builtin_rvtt_sfpreadlreg(0);
  __builtin_rvtt_sfpstore (nullptr, a, 4, 0, 0, 3, 7);
  auto b = __builtin_rvtt_sfpload (nullptr, 4, 0, 0, 3, 7);
  __builtin_rvtt_sfpwritelreg (b, 0);
}
/*
**_Z3foov:
**	# READ L0
**	SFPSTORE	L0, 4, 3, 7
**	# WRITE L0
**	ret
*/

extern void inc ();
void bar (unsigned i) {
  auto a =  __builtin_rvtt_sfpreadlreg(0);
  __builtin_rvtt_sfpstore (nullptr, a, 4, 0, 0, 3, 7);
  if (i) {
      auto b = __builtin_rvtt_sfpload (nullptr, 4, 0, 0, 3, 7);
      __builtin_rvtt_sfpwritelreg (b, 0);
  } else {
    inc ();
    __builtin_rvtt_sfpnop (); // we should be looking past this
    auto b = __builtin_rvtt_sfpload (nullptr, 4, 0, 0, 3, 7);
    __builtin_rvtt_sfpwritelreg (b, 0);
    inc ();
  }
}
/*
**_Z3barj:
**	addi	sp,sp,-16
**	sw	ra,12\(sp\)
**	# READ L0
**	SFPSTORE	L0, 4, 3, 7
**	beq	a0,zero,\.L[0-9]+
**	# WRITE L0
**	lw	ra,12\(sp\)
**	addi	sp,sp,16
**	jr	ra
**	call	_Z3incv
**	SFPNOP
**	# WRITE L0
**	lw	ra,12\(sp\)
**	addi	sp,sp,16
**	tail	_Z3incv
*/

void baz () {
  auto a =  __builtin_rvtt_sfpreadlreg(0);
  __builtin_rvtt_sfpstore (nullptr, a, 4, 0, 0, 3, 7);
  auto b = __builtin_rvtt_sfpload (nullptr, 4, 0, 0, 0, 7);
  __builtin_rvtt_sfpwritelreg (b, 0);
}
/*
**_Z3bazv:
**	# READ L0
**	SFPSTORE	L0, 4, 3, 7
**	SFPLOAD	L0, 4, 0, 7
**	# WRITE L0
**	ret
*/

void toto () {
  auto a =  __builtin_rvtt_sfpreadlreg(0);
  __builtin_rvtt_sfpstore (nullptr, a, 4, 0, 0, 3, 7);
  auto b = __builtin_rvtt_sfpload (nullptr, 6, 0, 0, 0, 7);
  __builtin_rvtt_sfpwritelreg (b, 0);
}
/*
**_Z4totov:
**	# READ L0
**	SFPSTORE	L0, 4, 3, 7
**	SFPLOAD	L0, 6, 0, 7
**	# WRITE L0
**	ret
*/

void ins (unsigned i) {
  auto a =  __builtin_rvtt_sfpreadlreg(0);
  __builtin_rvtt_sfpstore (nullptr, a, 4, 0, 0, 3, 7);
  while (i--) {
    auto b = __builtin_rvtt_sfpload (nullptr, 4, 0, 0, 0, 7);
    __builtin_rvtt_sfpwritelreg (b, 0);
  }
}
/*
**_Z3insj:
**	# READ L0
**	SFPSTORE	L0, 4, 3, 7
**	beq	a0,zero,\.L[0-9]+
**	addi	a0,a0,-1
**	li	a5,-1
**	SFPLOAD	L0, 4, 0, 7
**	# WRITE L0
**	addi	a0,a0,-1
**	bne	a0,a5,\.L[0-9]+
**	ret
*/
