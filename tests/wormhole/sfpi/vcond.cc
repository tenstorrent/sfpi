/*
 * SPDX-FileCopyrightText: © 2023 Tenstorrent Inc.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

// Test to be sure uses of vCond work properly
//
// vCond came about when the wrapper handled conditionals and its
// functionality worked "for free".  After pushing the conditional code into
// the compiler the question became: should vCond be exposed or not?  I think
// it provides useful functionality in some instances, but also the generality
// results in complexity:
//  1) Usefull to intermix RISCV code w/ a conditional.  This is easy when a
//     function returns a vCond which is then used in a v_if
//  2) Complex when the user creates a vCond outside of a v_if and then uses
//     it in multiple v_ifs.  The compiler handles this by storing the CC
//     state in a register and the re-testing the register as needed in the
//     v_if.
//
// This file tests this functionality.

#include <stdlib.h>

#include <cstdio>
#include "test.h"

using namespace sfpi;

// This file tests orphaned code...
#pragma GCC diagnostic ignored "-Wunused-but-set-variable"

inline vInt predicate(vFloat v, int test)
{
    if (test == 0) return (v < 0.0f);
    return (v >= 0.0f);
}

inline vInt predicate2(vFloat v, int test)
{
    if (test == 0) return (v < 0.0f && v != 0.0f);
    return (v >= 0.0f && v != 0.0f);
}

inline vInt predicate3(vFloat v, int test)
{
    if (test == 0) return (v != 0.0f || v <= 0.0f);
    return (v != 0.0f || v <= 0.0f);
}

inline vInt predicate4(vFloat v, int test)
{
    while (test--) {
        v_if (v < 3.0) {
            if (test == 3) {
                break;
            }
            v -= 1.0f;
        }
        v_endif;
    }

    return v == 0.0f;
}

void vcond_inside_inline()
{
    vFloat x = 1.0f;

    v_if (predicate(x, 5)) {
        x = 2.0f;
    }
    v_endif;

    dst_reg[0] = x;
}

void vcond_inside_noninline(int test)
{
    vFloat x = 1.0f;

    v_if (predicate(x, test)) {
        x = 2.0f;
    }
    v_endif;

    dst_reg[0] = x;
}

void vcond_outside_inline()
{
    vFloat x = 1.0f;
    vInt y = predicate(x, 5);

    v_if (y == 0) {
        x = 2.0f;
    }
    v_endif;

    dst_reg[0] = x;
}

void vcond_outside_noninline(int test)
{
    vFloat x = 1.0f;
    vInt y = predicate(x, test);

    v_if (y == 0) {
        x = 2.0f;
    }
    v_endif;

    dst_reg[0] = x;
}

void vcond_orphan_inline(int test)
{
    vFloat x = 1.0f;
    vInt y = predicate(x, 5);
}

void vcond_orphan_noninline(int test)
{
    vFloat x = 1.0f;
    vInt y = predicate(x, test);
}

void vcond_asbool_inline(int test)
{
    vFloat x = 1.0f;
    vInt y = predicate(x, 5);

    dst_reg[0] = y;
}

void vcond_asbool_noninline(int test)
{
    vFloat x = 1.0f;
    vInt y = predicate(x, test);

    dst_reg[0] = y;
}

void vcond_inside_inline2()
{
    vFloat x = 1.0f;

    v_if (predicate2(x, 5)) {
        x = 2.0f;
    }
    v_endif;

    dst_reg[0] = x;
}

void vcond_inside_noninline2(int test)
{
    vFloat x = 1.0f;

    v_if (predicate2(x, test)) {
        x = 2.0f;
    }
    v_endif;

    dst_reg[0] = x;
}

void vcond_outside_inline2()
{
    vFloat x = 1.0f;
    vInt y = predicate2(x, 5);

    v_if (y == 0) {
        x = 2.0f;
    }
    v_endif;

    dst_reg[0] = x;
}

void vcond_outside_noninline2(int test)
{
    vFloat x = 1.0f;
    vInt y = predicate2(x, test);

    v_if (y == 0) {
        x = 2.0f;
    }
    v_endif;

    dst_reg[0] = x;
}

void vcond_orphan_inline2(int test)
{
    vFloat x = 1.0f;
    vInt y = predicate2(x, 5);
}

void vcond_orphan_noninline2(int test)
{
    vFloat x = 1.0f;
    vInt y = predicate2(x, test);
}

void vcond_asbool_inline2(int test)
{
    vFloat x = 1.0f;
    vInt y = predicate2(x, 5);

    dst_reg[0] = y;
}

void vcond_asbool_noninline2(int test)
{
    vFloat x = 1.0f;
    vInt y = predicate2(x, test);

    dst_reg[0] = y;
}

void vcond_inside_inline3()
{
    vFloat x = 1.0f;

    v_if (predicate3(x, 5)) {
        x = 2.0f;
    }
    v_endif;

    dst_reg[0] = x;
}

void vcond_inside_noninline3(int test)
{
    vFloat x = 1.0f;

    v_if (predicate3(x, test)) {
        x = 2.0f;
    }
    v_endif;

    dst_reg[0] = x;
}

void vcond_outside_inline3()
{
    vFloat x = 1.0f;
    vInt y = predicate3(x, 5);

    v_if (y == 0) {
        x = 2.0f;
    }
    v_endif;

    dst_reg[0] = x;
}

void vcond_outside_noninline3(int test)
{
    vFloat x = 1.0f;
    vInt y = predicate3(x, test);

    v_if (y == 0) {
        x = 2.0f;
    }
    v_endif;

    dst_reg[0] = x;
}

void vcond_inside_elseif_inline3()
{
    vFloat x = 1.0f;

    v_if (x == 0.0f) {
        x = 2.0f;
    } v_elseif (predicate3(x, 5)) {
        x = 3.0f;
    }
    v_endif;

    dst_reg[0] = x;
}

void vcond_inside_elseif_noninline3(int test)
{
    vFloat x = 1.0f;

    v_if (x == 0.0f) {
        x = 2.0f;
    } v_elseif (predicate3(x, test)) {
        x = 3.0f;
    }
    v_endif;

    dst_reg[0] = x;
}

void vcond_outside_elseif_inline3()
{
    vFloat x = 1.0f;
    vInt y = predicate3(x, 5);

    v_if (x == 0.0f) {
        x = 2.0f;
    } v_elseif (y == 0) {
        x = 3.0f;
    }
    v_endif;

    dst_reg[0] = x;
}

void vcond_outside_elseif_noninline3(int test)
{
    vFloat x = 1.0f;
    vInt y = predicate3(x, test);

    v_if (x == 0.0f) {
        x = 2.0f;
    } v_elseif (y == 0) {
        x = 3.0f;
    }
    v_endif;

    dst_reg[0] = x;
}

void vcond_inside_inline4()
{
    vFloat x = 1.0f;

    v_if (predicate4(x, 5)) {
        x = 2.0f;
    }
    v_endif;

    dst_reg[0] = x;
}

void vcond_inside_noninline4(int test)
{
    vFloat x = 1.0f;

    v_if (predicate4(x, test)) {
        x = 2.0f;
    }
    v_endif;

    dst_reg[0] = x;
}

void vcond_outside_inline4()
{
    vFloat x = 1.0f;
    vInt y = predicate4(x, 5);

    v_if (y == 0) {
        x = 2.0f;
    }
    v_endif;

    dst_reg[0] = x;
}

void vcond_outside_noninline4(int test)
{
    vFloat x = 1.0f;
    vInt y = predicate4(x, test);

    v_if (y == 0) {
        x = 2.0f;
    }
    v_endif;

    dst_reg[0] = x;
}
