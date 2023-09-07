/*
 * SPDX-FileCopyrightText: © 2023 Tenstorrent Inc.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <stdio.h>
#include <stdlib.h>
#include "test.h"

using namespace sfpi;

void basic_nop(int imm)
{
    {
        // imm store, imm load
        vFloat a = 120.0F;
        dst_reg[12] = a;
        a = dst_reg[12];
    }

    {
        // imm store, non-imm load
        vFloat a = 120.0F;
        dst_reg[12] = a;
        a = dst_reg[imm - 23];
    }

    {
        // non-imm store, imm load
        vFloat a = 130.0F;
        dst_reg[imm - 23] = a;
        a = dst_reg[12];
    }

    {
        // non-imm store, non-imm load
        vFloat a = 140.0F;
        dst_reg[imm - 23] = a;
        a = dst_reg[imm - 23];
    }
}

void basic_incrwc_nonop(int imm)
{
    {
        // imm store, imm load
        vFloat a = 120.0F;
        dst_reg[12] = a;
        dst_reg++;
        a = dst_reg[12];
    }

    {
        // imm store, non-imm load
        vFloat a = 120.0F;
        dst_reg[12] = a;
        dst_reg++;
        a = dst_reg[imm - 23];
    }

    {
        // non-imm store, imm load
        vFloat a = 130.0F;
        dst_reg[imm - 23] = a;
        dst_reg++;
        a = dst_reg[12];
    }

    {
        // non-imm store, non-imm load
        vFloat a = 140.0F;
        dst_reg[imm - 23] = a;
        dst_reg++;
        a = dst_reg[imm - 23];
    }
}

void loop_nop()
{
    dst_reg[0] = 1.0f;
    #pragma GCC unroll 0
    for (int i = 0; i < 4; i++) {
        vFloat a = dst_reg[12];
        dst_reg[0] = a;
    }
}

void loop_nonop()
{
    vFloat trash;

    dst_reg[0] = 1.0f;
    #pragma GCC unroll 0
    for (int i = 0; i < 4; i++) {
        vFloat a = 1.0f;
        trash = dst_reg[0];
        dst_reg[0] = a;
    }
    dst_reg[0] = trash;
}

void loop_nop(int imm)
{
    dst_reg[0] = 1.0f;
    #pragma GCC unroll 0
    for (int i = 0; i < 4; i++) {
        vFloat a = dst_reg[imm];
        dst_reg[0] = a;
    }
}
