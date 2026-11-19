// { dg-options "-mcpu=tt-qsr32-tensix -fno-exceptions -fno-rtti -O2 -fno-shrink-wrap" }
// { dg-final { check-function-bodies "**" "" } }

void foo () {
  {
    auto a =  __builtin_rvtt_sfpreadlreg (0);
    __builtin_rvtt_sfpstore (nullptr, a, 4, 0, 0, 3, 7);
    auto b = __builtin_rvtt_sfpload (nullptr, 4, 0, 0, 3, 7);
    __builtin_rvtt_sfpwritelreg (b, 0);
  }
  {
    auto a =  __builtin_rvtt_sfpreadlreg (1);
    __builtin_rvtt_sfpstore (nullptr, a, 4, 0, 0, 3, 7);
    auto b = __builtin_rvtt_sfpload (nullptr, 8, 0, 0, 3, 7);
    b = __builtin_rvtt_sfpload (nullptr, 4, 0, 0, 3, 7);
    __builtin_rvtt_sfpwritelreg (b, 1);
  }
}
/*
**_Z3foov:
**	# READ L0
**	SFPSTORE	L0, 4, 3, 7, 0, 0
**	# WRITE L0
**	# READ L1
**	SFPSTORE	L1, 4, 3, 7, 0, 0
**	SFPLOAD	L0, 8, 3, 7, 0, 0
**	SFPLOAD	L1, 4, 3, 7, 0, 0
**	# WRITE L1
**	ret
*/

extern void inc ();
void bar (unsigned i) {
  auto a =  __builtin_rvtt_sfpreadlreg (0);
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
**	SFPSTORE	L0, 4, 3, 7, 0, 0
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
  auto a =  __builtin_rvtt_sfpreadlreg (0);
  __builtin_rvtt_sfpstore (nullptr, a, 4, 0, 0, 3, 7);
  auto b = __builtin_rvtt_sfpload (nullptr, 4, 0, 0, 0, 7);
  __builtin_rvtt_sfpwritelreg (b, 0);
}
/*
**_Z3bazv:
**	# READ L0
**	SFPSTORE	L0, 4, 3, 7, 0, 0
**	SFPNOP
**	SFPLOAD	L0, 4, 0, 7, 0, 0
**	# WRITE L0
**	ret
*/

void toto () {
  auto a =  __builtin_rvtt_sfpreadlreg (0);
  __builtin_rvtt_sfpstore (nullptr, a, 4, 0, 0, 3, 7);
  auto b = __builtin_rvtt_sfpload (nullptr, 6, 0, 0, 0, 7);
  __builtin_rvtt_sfpwritelreg (b, 0);
}
/*
**_Z4totov:
**	# READ L0
**	SFPSTORE	L0, 4, 3, 7, 0, 0
**	SFPLOAD	L0, 6, 0, 7, 0, 0
**	# WRITE L0
**	ret
*/

void ins (unsigned i) {
  auto a =  __builtin_rvtt_sfpreadlreg (0);
  __builtin_rvtt_sfpstore (nullptr, a, 4, 0, 0, 3, 7);
  while (i--) {
    auto b = __builtin_rvtt_sfpload (nullptr, 4, 0, 0, 0, 7);
    __builtin_rvtt_sfpwritelreg (b, 0);
  }
}
/*
**_Z3insj:
**	# READ L0
**	SFPSTORE	L0, 4, 3, 7, 0, 0
**	SFPNOP
**	beq	a0,zero,\.L[0-9]+
**	addi	a0,a0,-1
**	li	a5,-1
**	SFPLOAD	L0, 4, 0, 7, 0, 0
**	# WRITE L0
**	addi	a0,a0,-1
**	bne	a0,a5,\.L[0-9]+
**	ret
*/

void src () {
  {
    auto a =  __builtin_rvtt_sfpreadlreg (0);
    __builtin_rvtt_sfpstoresrcs (nullptr, a, 4, 0, 0, 3, 7, 0);
    auto b = __builtin_rvtt_sfploadsrcs (nullptr, 4, 0, 0, 3, 7, 0);
    __builtin_rvtt_sfpwritelreg (b, 0);
  }
  {
    auto a =  __builtin_rvtt_sfpreadlreg (1);
    __builtin_rvtt_sfpstoresrcs (nullptr, a, 4, 0, 0, 3, 7, 0);
    auto b = __builtin_rvtt_sfploadsrcs (nullptr, 4, 0, 0, 3, 7, 1);
    __builtin_rvtt_sfpwritelreg (b, 1);
  }
  {
    auto a =  __builtin_rvtt_sfpreadlreg (2);
    __builtin_rvtt_sfpstoresrcs (nullptr, a, 4, 0, 0, 3, 7, 1);
    auto b = __builtin_rvtt_sfploadsrcs (nullptr, 4, 0, 0, 3, 7, 0);
    __builtin_rvtt_sfpwritelreg (b, 2);
  }
}
/*
**_Z3srcv:
**	# READ L0
**	SFPSTORE	L0, 4, 3, 7, 1, 0
**	# WRITE L0
**	# READ L1
**	SFPSTORE	L1, 4, 3, 7, 1, 0
**	SFPLOAD	L1, 4, 3, 7, 1, 1
**	# WRITE L1
**	# READ L2
**	SFPSTORE	L2, 4, 3, 7, 1, 1
**	# WRITE L2
**	ret
*/

void other () {
  {
    auto a =  __builtin_rvtt_sfpreadlreg (0);
    __builtin_rvtt_sfpstoresrcs (nullptr, a, 4, 0, 0, 3, 7, 0);
    __builtin_rvtt_sfpbankdone (true, false, false);
    auto b = __builtin_rvtt_sfploadsrcs (nullptr, 4, 0, 0, 3, 7, 0);
    __builtin_rvtt_sfpwritelreg (b, 0);
  }
  {
    auto a =  __builtin_rvtt_sfpreadlreg (1);
    __builtin_rvtt_sfpstoresrcs (nullptr, a, 4, 0, 0, 3, 7, 0);
    __builtin_rvtt_ttincrwc (0, 0, 0, 0);
    auto b = __builtin_rvtt_sfploadsrcs (nullptr, 4, 0, 0, 3, 7, 0);
    __builtin_rvtt_sfpwritelreg (b, 1);
  }
  {
    auto a =  __builtin_rvtt_sfpreadlreg (2);
    __builtin_rvtt_sfpstore (nullptr, a, 4, 0, 0, 3, 7);
    __builtin_rvtt_ttincrwc (0, 0, 0, 0);
    auto b = __builtin_rvtt_sfpload (nullptr, 4, 0, 0, 3, 7);
    __builtin_rvtt_sfpwritelreg (b, 2);
  }
}
/*
**_Z5otherv:
**	# READ L0
**	SFPSTORE	L0, 4, 3, 7, 1, 0
**	SFPBANKDONE	1, 0, 0
**	SFPLOAD	L0, 4, 3, 7, 1, 0
**	# WRITE L0
**	# READ L1
**	SFPSTORE	L1, 4, 3, 7, 1, 0
**	TTINCRWC	0, 0, 0, 0
**	SFPLOAD	L1, 4, 3, 7, 1, 0
**	# WRITE L1
**	# READ L2
**	SFPSTORE	L2, 4, 3, 7, 0, 0
**	TTINCRWC	0, 0, 0, 0
**	SFPLOAD	L2, 4, 3, 7, 0, 0
**	# WRITE L2
**	ret
*/
