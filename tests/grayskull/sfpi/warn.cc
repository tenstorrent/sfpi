#include <stdio.h>
#include <stdlib.h>
#include <ckernel_ops.h>
#include <cstdint>
#include "test.h"
#include <limits>

using namespace sfpi;

typedef float v64sf __attribute__((vector_size(64*4)));

extern void deref(vFloat *x);

#if !defined(PUSH_POP) && !defined(POP_PUSH) && !defined(SPILL)
#define BASE
#endif

#ifdef BASE
vFloat abc = 5.0f;

void global()
{
    abc = 3.0f;
}

void pointeruse(vFloat *a)
{
    *a = 5.0f;
}

void address()
{
    vFloat x = 1.0f;
    deref(&x);
}

void unnit()
{
    vFloat x;

    dst_reg[0] = x;
}

void argcall(vFloat x)
{
    dst_reg[0] = x;
}

vFloat ret()
{
    vFloat x = 1.0f;

    return x;
}

// Double problems cancel: there is no initialization of x which is not
// detected because it is not used by an sfpu instruction.
void warnandwrite()
{
    vFloat x;

    deref(&x);
}

// The errors below should never occur when using the wrapper
void setccnotinsidepushpop()
{
    v64sf a = {1};

    __builtin_rvtt_gs_sfpsetcc_v(a, 12);
}
#endif

// The errors below cause the compiler to bail out, try one at a time
#ifdef POP_PUSH
void popwithoutpush()
{
    __builtin_rvtt_gs_sfppopc();
}
#endif


#ifdef PUSH_POP
void pushwithoutpop()
{
    __builtin_rvtt_gs_sfppushc();
}
#endif

#ifdef SPILL
void spill()
{
    vFloat a = 1.0f;
    vFloat b = 1.0f;
    vFloat c = 1.0f;
    vFloat d = 1.0f;
    vFloat e = 1.0f;
    dst_reg[0] = a;
    dst_reg[0] = b;
    dst_reg[0] = c;
    dst_reg[0] = d;
    dst_reg[0] = e;
}
#endif
