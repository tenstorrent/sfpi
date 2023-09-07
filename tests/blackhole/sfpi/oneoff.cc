#include <cstdio>
#include <stdlib.h>
#include "test.h"

using namespace sfpi;

void foo()
{
    vFloat x = 1.0;
    vFloat y = 2.0;
    dst_reg[0] = x * y;
}
