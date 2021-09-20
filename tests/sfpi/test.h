#pragma once

namespace ckernel {
    volatile unsigned int *instrn_buffer;
};

#ifdef COMPILE_FOR_EMULE
#include "sfpu.h"
#endif

#include <sfpi.h>
