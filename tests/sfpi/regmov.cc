// Test the sfpu move pass which will swap operands if possible to avoid a
// move and otherwise inject a "simple" move to avoid a compiler generated CC
// safe "long" move later

#include <stdlib.h>

#include <cstdio>
#include "test.h"

using namespace sfpi;

// Can swap
void remove_add1()
{
    VecShort x = 1;
    VecShort y = 2;

    VecShort z = x + y;

    dst_reg[0] = x;
    dst_reg[2] = z;
}

// Ops already in correct order, do nothing
void remove_add2_nop()
{
    VecShort x = 1;
    VecShort y = 2;

    VecShort z = y + x;

    dst_reg[0] = x;
    dst_reg[2] = z;
}

// Can't swap due to subsequent use
void replace_add3()
{
    VecShort x = 1;
    VecShort y = 2;

    VecShort z = x + y;

    dst_reg[0] = x;
    dst_reg[1] = y;
    dst_reg[2] = z;
}

// Can't swap w/ a subtract
void replace_sub1()
{
    VecShort x = 1;
    VecShort y = 2;

    VecShort z = y - x;

    dst_reg[0] = x;
    dst_reg[2] = z;
}

// Can swap
void remove_and1()
{
    VecShort x = 1;
    VecShort y = 2;

    VecShort z = x & y;

    dst_reg[0] = x;
    dst_reg[2] = z;
}

// Ops already in correct order, do nothing
void remove_and2_nop()
{
    VecShort x = 1;
    VecShort y = 2;

    VecShort z = y & x;

    dst_reg[0] = x;
    dst_reg[2] = z;
}

// Can't swap due to subsequent use
void replace_and3()
{
    VecShort x = 1;
    VecShort y = 2;

    VecShort z = x & y;

    dst_reg[0] = x;
    dst_reg[1] = y;
    dst_reg[2] = z;
}

// Can swap
void remove_or1()
{
    VecShort x = 1;
    VecShort y = 2;

    VecShort z = x | y;

    dst_reg[0] = x;
    dst_reg[2] = z;
}

// Ops already in correct order, do nothing
void remove_or2_nop()
{
    VecShort x = 1;
    VecShort y = 2;

    VecShort z = y | x;

    dst_reg[0] = x;
    dst_reg[2] = z;
}

// Can't swap due to subsequent use
void replace_or3()
{
    VecShort x = 1;
    VecShort y = 2;

    VecShort z = x | y;

    dst_reg[0] = x;
    dst_reg[1] = y;
    dst_reg[2] = z;
}

void remove_cmp1()
{
    VecShort x = 1;
    VecShort y = 2;

    // Below get combined
    VecShort z = y + x;
    p_if (z < 0) {
    }
    p_endif;

    dst_reg[0] = x;
    dst_reg[2] = z;
    
}

void replace_cmp2()
{
    VecShort x = 1;
    VecShort y = 2;

    // Below get combined, can't remove the move due to subtract
    VecShort z = y - x;
    p_if (z < 0) {
    }
    p_endif;

    dst_reg[0] = x;
    dst_reg[2] = z;
}

// These next two tests used to generate muli/addi which trashed the
// src register require the compiler to generate a mov to preserve the src.
// However, after moving mad/addi/addhalf generation into the compiler, the
// compiler won't emit the instructions that this used to test (uses loadi to
// avoid the mov which is faster).  I'm leaving the tests just the same
void replace_muli()
{
    VecHalf x = dst_reg[0];
    VecHalf y;

    y = x * 2.0f;

    dst_reg[0] = x;
    dst_reg[0] = y;
}

// See comment above
void replace_addi()
{
    VecHalf x = dst_reg[0];
    VecHalf y;

    y = x + 2.0f;

    dst_reg[0] = x;
    dst_reg[0] = y;
}

void replace_shft()
{
    VecShort x = 1;
    VecShort y;

    y = x << 1;

    dst_reg[0] = x;
    dst_reg[2] = y;
}

void replace_creg()
{
    VecShort x = CReg_1;
    VecShort y = CReg_Neg_1;
    dst_reg[0] = x << 1;
    dst_reg[0] = x & y;
    dst_reg[0] = y | x;
}
