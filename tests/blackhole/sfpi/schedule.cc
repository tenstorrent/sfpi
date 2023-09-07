/*
 * SPDX-FileCopyrightText: © 2023 Tenstorrent Inc.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <stdio.h>
#include <stdlib.h>
#include "test.h"

using namespace sfpi;

void basic_add_nop()
{
    dst_reg[0] = dst_reg[0] + dst_reg[0];
}

void basic_mul_nop()
{
    dst_reg[0] = dst_reg[0] * dst_reg[0];
}

void basic_addi_nop()
{
    vFloat x = 1.0f;
    dst_reg[0] = x + 1.0f;
}

void basic_muli_nop()
{
    vFloat x = 1.0f;
    dst_reg[0] = x * 1.0f;
}

void basic_addi_nop_imm(int imm)
{
    vFloat x = 1.0f;
    dst_reg[imm] = x + 1.0f;
}

void loop_nop()
{
    vFloat x = dst_reg[0] * 2.0f;
    #pragma GCC unroll 0
    for (int i = 0; i < 4; i++) {
        dst_reg[0] = x;
        x *= 2.0f;
    }
}

void loop_no_nop()
{
    vFloat x = dst_reg[0] * 2.0f;
    #pragma GCC unroll 0
    for (int i = 0; i < 4; i++) {
        dst_reg[0] = dst_reg[1];
        x *= 2.0f;
    }
}
