namespace ckernel {
unsigned *instrn_buffer;
}
#include <sfpi.h>

using namespace sfpi;

/* Exhaustive enumeration found this DFG.  Source order peaks at eleven and the
   deterministic list heuristic remains above eight, while the MILP finds and
   independently validates an eight-register topological schedule.  Current
   IRA nevertheless spills: this intentionally captures the M2 physical-
   coloring boundary and is not a passing DejaGNU fixture.  */
void
test ()
{
  vFloat i0 = l_reg[LRegs::LReg0];
  vFloat i1 = l_reg[LRegs::LReg1];
  vFloat i2 = l_reg[LRegs::LReg2];
  vFloat i3 = l_reg[LRegs::LReg3];
  vFloat i4 = l_reg[LRegs::LReg4];
  vFloat i5 = l_reg[LRegs::LReg5];
  vFloat c = vConstFloatPrgm0;

  vFloat y0 = i5 + i4;
  vFloat y1 = i0 * i2 + i5;
  vFloat y2 = i5 * i5 + y1;
  vFloat y3 = i1 + i0;
  vFloat y4 = i4 + c;
  vFloat y5 = i5 + c;
  vFloat y6 = i1 * y2 + i3;
  vFloat y7 = i5 * y6 + i4;
  vFloat y8 = i1 * i0 + y5;
  vFloat y9 = i3 * i5 + y2;

  l_reg[LRegs::LReg0] = y0;
  l_reg[LRegs::LReg1] = y3;
  l_reg[LRegs::LReg2] = y4;
  l_reg[LRegs::LReg3] = y7;
  l_reg[LRegs::LReg4] = y8;
  l_reg[LRegs::LReg5] = y9;
  l_reg[LRegs::LReg6] = i4;
}
