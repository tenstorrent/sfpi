// { dg-options "-march=rv32imac -DARCH_BLACKHOLE -O2 -I [SFPI]/include -fno-exceptions -fno-rtti" }

namespace ckernel{
  extern unsigned long instrn_buffer[];
}
#include <lltt.h>
#include <sfpi.h>
