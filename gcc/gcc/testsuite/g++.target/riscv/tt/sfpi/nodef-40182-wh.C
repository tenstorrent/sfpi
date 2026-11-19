// { dg-options "-mcpu=tt-wh-tensix -O2 -I [SFPI]/include -fno-exceptions -fno-rtti -Wall -Wextra -Wunused-parameter" }

namespace ckernel{
    extern unsigned long instrn_buffer[];
}
#include <sfpi.h>
#include <lltt.h>

// Nothing defined
// { dg-final { scan-assembler-not {\n_ZN4sfpi[0-9][_0-9a-zA-Z]*:} } }
