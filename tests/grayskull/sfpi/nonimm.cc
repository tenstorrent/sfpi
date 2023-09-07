/*
 * SPDX-FileCopyrightText: © 2023 Tenstorrent Inc.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <cstdio>
#include <stdlib.h>
#include "test.h"

using namespace sfpi;

const int loops = 4;


void loadif(float f, int i)
{
#pragma GCC unroll loops
    for (int idx = 0; idx < loops; idx++) {
        vFloat x = f;
        vFloat y = f;
        dst_reg[0] = x;
        dst_reg[1] = y;
    }

#pragma GCC unroll loops
    for (int idx = 0; idx < loops; idx++) {
        vFloat x = s2vFloat16b(i);
        dst_reg[0] = x;
    }

#pragma GCC unroll loops
    for (int idx = 0; idx < loops; idx++) {
        vFloat x = s2vFloat16a(i);
        dst_reg[0] = x;
    }

    // Test fallback
    vFloat x;
#pragma GCC unroll loops
    for (int idx = 0; idx < loops; idx++) {
        x = s2vFloat16b(i);
    }
    dst_reg[0] = x;
}

void loadif_lv(float f, int i)
{
#pragma GCC unroll loops
    for (int idx = 0; idx < loops; idx++) {
        // Live
        vFloat x = 1.0f;
        vFloat y = 1.0f;
        v_if(x == 2.0f) {
            x = f;
            y = f;
        }
        v_endif;
        dst_reg[0] = x;
        dst_reg[1] = y;
    }

#pragma GCC unroll loops
    for (int idx = 0; idx < loops; idx++) {
        vFloat x = 1.0f;
        v_if (x == 2.0f) {
            x = s2vFloat16b(i);
        }
        v_endif;
        dst_reg[0] = x;
    }

#pragma GCC unroll loops
    for (int idx = 0; idx < loops; idx++) {
        vFloat x = 1.0f;
        v_if (x == 2.0f) {
            x = s2vFloat16a(i);
        }
        v_endif;
        dst_reg[0] = x;
    }

    // Test fallback
    vFloat x = 1.0f;
#pragma GCC unroll loops
    for (int idx = 0; idx < loops; idx++) {
        v_if (x == 2.0f) {
            x = s2vFloat16b(i);
        }
        v_endif;
    }
    dst_reg[0] = x;
}

void loadii(int i)
{
#pragma GCC unroll loops
    for (int idx = 0; idx < loops; idx++) {
        vInt x = i;
        dst_reg[0] = x;
    }

#pragma GCC unroll loops
    for (int idx = 0; idx < loops; idx++) {
        vUInt x = i;
        dst_reg[0] = x;
    }
}

void loadii_lv(int i)
{
#pragma GCC unroll loops
    for (int idx = 0; idx < loops; idx++) {
        vInt x = 1;
        v_if (x == 2) {
            x = i;
        }
        v_endif;
        dst_reg[0] = x;
    }

#pragma GCC unroll loops
    for (int idx = 0; idx < loops; idx++) {
        vUInt x = 1;
        v_if (x == 2) {
            x = i;
        }
        v_endif;
        dst_reg[0] = x;
    }
}

void loadstore(int i)
{
#pragma GCC unroll loops
    for (int idx  = 0; idx < loops; idx++) {
        vFloat x = dst_reg[i];
        dst_reg[i] = x;
        dst_reg[i] = 1;
        dst_reg[i] = vUInt(2);
    }
}

void loadstore_lv(int i)
{
#pragma GCC unroll loops
    for (int idx  = 0; idx < loops; idx++) {
        vFloat x = 1.0f;
        v_if (x == 2.0f) {
            dst_reg[i];
        }
        v_endif;
        dst_reg[i] = x;
        dst_reg[i] = 1;
        dst_reg[i] = vUInt(2);
    }
}

// Hit fallback path
void loadstorefallback(int arg)
{
    vFloat y, z;
#pragma GCC unroll loops
    for(int x = 0; x < loops; x++) {
        y = dst_reg[arg];
        z = dst_reg[arg];
    }
    dst_reg[0] = y;
    dst_reg[0] = z;
}

void loadstorefallback_lv(int arg)
{
    vFloat y = 1.0f;
    vFloat z = 1.0f;
#pragma GCC unroll loops
    for(int x = 0; x < loops; x++) {
        v_if (y == 1.0f) {
            y = dst_reg[arg];
            z = dst_reg[arg];
        }
        v_endif;
    }
    dst_reg[0] = y;
    dst_reg[0] = z;
}

void fcmps(float f, int i)
{
    vFloat x = 1.0f;
#pragma GCC unroll loops
    for (int idx  = 0; idx < loops; idx++) {
        v_if (x == f) {
            dst_reg[0] = x;
        }
        v_endif;
        v_if (x == s2vFloat16b(i)) {
            dst_reg[0] = x;
        }
        v_endif;
        v_if (x == s2vFloat16a(i)) {
            dst_reg[0] = x;
        }
        v_endif;
        v_if (x == 3.0f) {
            dst_reg[0] = x;
        }
        v_endif;
    }
}

void muladdf(float f, int i)
{
    vFloat y = 1.0f;
    vFloat x = 1.0f;
#pragma GCC unroll loops
    for (int idx  = 0; idx < loops; idx++) {
        y = x * s2vFloat16b(i);
        x = y * s2vFloat16a(i);
        y = x * f;
        x = y + s2vFloat16b(i);
        y = x + s2vFloat16a(i);
        x = y + f;
        dst_reg[0] = x;
        dst_reg[1] = y;
    }
}

void muladdf_lv(float f, int i)
{
    vFloat y = 1.0f;
    vFloat x = 1.0f;
#pragma GCC unroll loops
    for (int idx  = 0; idx < loops; idx++) {
        v_if (x == 1.0f) {
            y = x * s2vFloat16b(i);
            x = y * s2vFloat16a(i);
            y = x * f;
            x = y + s2vFloat16b(i);
            y = x + s2vFloat16a(i);
            x = y + f;
            dst_reg[0] = x;
            dst_reg[1] = y;
        }
        v_endif;
    }
}

void icmps(int i)
{
    vInt y = 1;
    vUInt z = 1;
#pragma GCC unroll loops
    for (int idx  = 0; idx < loops; idx++) {
        v_if (y == i) {
            dst_reg[0] = 0.0f;
        }
        v_endif;
        v_if (z == 1) {
            dst_reg[0] = 0.0f;
        }
        v_endif;
    }
}

void muladdi(int i)
{
    vInt x = 1;
    vUInt y = 1;
#pragma GCC unroll loops
    for (int idx  = 0; idx < loops; idx++) {
        y = x + i;
        x = y + 1;
        y = x - i;
        x = y - 1;
    }
}

void muladdi_lv(int i)
{
    vInt x = 1;
    vUInt y = 1;
#pragma GCC unroll loops
    for (int idx  = 0; idx < loops; idx++) {
        v_if (x == 2) {
            y = x + i;
            x = y + 1;
            y = x - i;
            x = y - 1;
        }
        v_endif;
    }
}

void other(int arg, float f)
{
    vFloat y, z, q;
#pragma GCC unroll loops
    for(int x = 0; x < loops; x++) {
        y = dst_reg[arg];
        z = dst_reg[arg];

        y = setsgn(z, arg);
        q = setman(z, arg);
        z = setexp(q, arg);
        y = addexp(z, arg);

        dst_reg[arg] = y;
        dst_reg[arg] = z;
        dst_reg[arg] = q;
    }
}

void other_lv(int arg, float f)
{
    vFloat y, z, q;
    vFloat zz = 1.0f;
#pragma GCC unroll loops
    for(int x = 0; x < loops; x++) {
        v_if (zz == 1.0f) {
            y = dst_reg[arg];
            z = dst_reg[arg];

            //            y = setsgn(z, arg);
            //            q = setman(z, arg);
            //z = setexp(q, arg);
            y = addexp(z, arg);

            dst_reg[arg] = y;
            dst_reg[arg] = z;
            //            dst_reg[arg] = q;
        }
        v_endif;
    }
}

void shft(int arg)
{
    vUInt a = 1;
    vUInt b = 1;

#pragma GCC unroll loops
    for(int x = 0; x < loops; x++) {
        a = b << arg;
        b = a >> arg;
    }

    dst_reg[0] = a;
    dst_reg[0] = b;
}

void shft_lv(int arg)
{
    vUInt a = 1;
    vUInt b = 1;

#pragma GCC unroll loops
    for(int x = 0; x < loops; x++) {
        v_if (a == 1) {
            a = b << arg;
            b = a >> arg;
        }
        v_endif;
    }

    dst_reg[0] = a;
    dst_reg[0] = b;
}

void muli(int arg)
{
    vFloat y = s2vFloat16b(arg);
    vFloat z = 1.0f;
#pragma GCC unroll loops
    for(int x = 0; x < loops; x++) {
        z *= y;
    }

    dst_reg[0] = z;
    dst_reg[0] = y;
}

// Test anything w/o loop unrolling
void muli_nounroll(int arg)
{
#pragma GCC unroll 0
    for(int x = 0; x < loops; x++) {
        vFloat y = s2vFloat16b(arg);
        dst_reg[0] = y;
    }
}
