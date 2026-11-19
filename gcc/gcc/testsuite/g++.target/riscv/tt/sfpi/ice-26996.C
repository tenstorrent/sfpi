// { dg-options "-mcpu=tt-wh-tensix -O2 -I [SFPI]/include -fno-exceptions -fno-rtti" }

namespace ckernel{
extern unsigned long *instrn_buffer;
}
#include <sfpi.h>

using namespace sfpi;

void bad()
{
  vFloat a = l_reg[LRegs::LReg0];
  vFloat b = l_reg[LRegs::LReg1];

  b = -b;
  (void) (a + b);

  l_reg[LRegs::LReg0] = a;
  l_reg[LRegs::LReg1] = b;
}
