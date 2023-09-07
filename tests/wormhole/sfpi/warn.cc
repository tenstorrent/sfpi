/*
 * SPDX-FileCopyrightText: © 2023 Tenstorrent Inc.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

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

    __builtin_rvtt_wh_sfpsetcc_v(a, 12);
}

void intinc()
{
    vInt x;
    x++;
}

// Hit a crashes on some of the below due to not checking SSA_NAME_DEF_STMT
// for null result.  Testing a bunch of them...
void crash_mad_uninit()
{
    vFloat x, y;

    dst_reg[0] = x + y;
}

void crash_int_add_fold_uninit()
{
    vInt x, y;

    vInt z = x + y;
    v_if (z == 0) {
    }
    v_endif;
}

void crash_cond_uninit()
{
    vInt x;

    v_if (x == 0) {
    }
    v_endif;
}

#endif

// The errors below cause the compiler to bail out, try one at a time
#ifdef POP_PUSH
void popwithoutpush()
{
    __builtin_rvtt_wh_sfppopc(0);
}
#endif


#ifdef PUSH_POP
void pushwithoutpop()
{
    __builtin_rvtt_sfppushc();
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
    vFloat f = 1.0f;
    vFloat g = 1.0f;
    vFloat h = 1.0f;
    vFloat i = 1.0f;
    dst_reg[0] = a;
    dst_reg[0] = b;
    dst_reg[0] = c;
    dst_reg[0] = d;
    dst_reg[0] = e;
    dst_reg[0] = f;
    dst_reg[0] = g;
    dst_reg[0] = h;
    dst_reg[0] = i;
}
#endif
