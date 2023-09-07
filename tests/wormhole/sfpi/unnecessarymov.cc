/*
 * SPDX-FileCopyrightText: © 2023 Tenstorrent Inc.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

// Test cases for compiler generated internal moves
#include <stdio.h>
#include <stdlib.h>
#include "test.h"

using namespace sfpi;

void whymov()
{
    vInt a = 1;
    vFloat d = dst_reg[0];
    v_if (d == 0.0f) {
        a = 2;
    }
    v_endif;
    vInt b = 3;
    v_if (a == 29) {
        b = 4;
    }
    v_endif;

    dst_reg[0] = b;
}

void whymov_sqrt()
{
    vFloat val = dst_reg[0];
    vFloat approx = reinterpret<vFloat>(1 - (reinterpret<vUInt>(val)));
    approx = (approx * approx) * approx;
    dst_reg[0] = approx * val;
}

void test_icmp_v()
{
    vInt a = 1;
    vInt b = 2;

    v_if (a == b) {
        b += 100;
    }
    v_endif;
    dst_reg[0] = b; // Not needed, but interesting.  ASM shows 2 regs as
                    // synonyms for the value

    vUInt c = 10;
    vUInt d = 11;

    v_if (c == d) {
        d += 700;
    } v_elseif (c >= d) {
        d += 1200;
    }
    v_endif;
    dst_reg[0] = b;
}

void lots_of_conditionals()
{
    vFloat x = 1.0f;

    v_if((x >= 0.0f || x < 0.0f) && (x >= 0.0f || x < 0.0f)) {
    }
    v_endif;
}
