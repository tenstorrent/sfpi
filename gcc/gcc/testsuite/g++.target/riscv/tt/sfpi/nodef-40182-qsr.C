// { dg-options "-mcpu=tt-qsr32-tensix -O2 -I [SFPI]/include -fno-exceptions -fno-rtti -Wall -Wextra -Wunused-parameter" }

namespace ckernel{
    extern unsigned long instrn_buffer[];
}
#include <lltt.h>
#include <sfpi.h>

// Nothing defined
// { dg-final { scan-assembler-not {\n_ZN4sfpi[0-9][_0-9a-zA-Z]*:} } }
