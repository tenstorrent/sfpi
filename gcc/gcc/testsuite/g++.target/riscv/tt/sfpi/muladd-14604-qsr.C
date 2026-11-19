// { dg-options "-mcpu=tt-qsr32-tensix -O2 -I [SFPI]/include -fno-exceptions -fno-rtti" }
// { dg-final { check-function-bodies "**" "" } }

namespace ckernel{
    unsigned long *instrn_buffer;
}
#include <sfpi.h>

using namespace sfpi;

void muladd() {
  vFloat a = l_reg[LRegs::LReg0];
  vFloat b = l_reg[LRegs::LReg1];
  vFloat c = l_reg[LRegs::LReg2];
  vFloat r;

  r = b * c + a;
  
  l_reg[LRegs::LReg3] = r;
}
/*
**_Z6muladdv:
**	# READ L0
**	# READ L1
**	# READ L2
**	SFPMAD	L3, L1, L2, L0, 0
**	# WRITE L3
**	ret
*/

void mulsub() {
  vFloat a = l_reg[LRegs::LReg0];
  vFloat b = l_reg[LRegs::LReg1];
  vFloat c = l_reg[LRegs::LReg2];
  vFloat r;

  r = b * c - a;
  
  l_reg[LRegs::LReg3] = r;
}
/*
**_Z6mulsubv:
**	# READ L0
**	# READ L1
**	# READ L2
**	SFPMAD	L3, L1, L2, L0, 2
**	# WRITE L3
**	ret
*/

void negmuladd() {
  vFloat a = l_reg[LRegs::LReg0];
  vFloat b = l_reg[LRegs::LReg1];
  vFloat c = l_reg[LRegs::LReg2];
  vFloat r;

  r = b * -c + a;
  
  l_reg[LRegs::LReg3] = r;
}
/*
**_Z9negmuladdv:
**	# READ L0
**	# READ L1
**	# READ L2
**	SFPMAD	L3, L2, L1, L0, 1
**	# WRITE L3
**	ret
*/

void negmulsub() {
  vFloat a = l_reg[LRegs::LReg0];
  vFloat b = l_reg[LRegs::LReg1];
  vFloat c = l_reg[LRegs::LReg2];
  vFloat r;

  r = b * -c - a;
  
  l_reg[LRegs::LReg3] = r;
}
/*
**_Z9negmulsubv:
**	# READ L0
**	# READ L1
**	# READ L2
**	SFPMAD	L3, L2, L1, L0, 3
**	# WRITE L3
**	ret
*/

void negmuladd2() {
  vFloat a = l_reg[LRegs::LReg0];
  vFloat b = l_reg[LRegs::LReg1];
  vFloat c = l_reg[LRegs::LReg2];
  vFloat r;

  r = -b * c + a;
  
  l_reg[LRegs::LReg3] = r;
}
/*
**_Z10negmuladd2v:
**	# READ L0
**	# READ L1
**	# READ L2
**	SFPMAD	L3, L1, L2, L0, 1
**	# WRITE L3
**	ret
*/

void negmulsub2() {
  vFloat a = l_reg[LRegs::LReg0];
  vFloat b = l_reg[LRegs::LReg1];
  vFloat c = l_reg[LRegs::LReg2];
  vFloat r;

  r = -b * c - a;
  
  l_reg[LRegs::LReg3] = r;
}
/*
**_Z10negmulsub2v:
**	# READ L0
**	# READ L1
**	# READ L2
**	SFPMAD	L3, L1, L2, L0, 3
**	# WRITE L3
**	ret
*/
