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
    vInt x = 1;
    vInt y = 2;

    vInt z = x + y;

    dst_reg[0] = x;
    dst_reg[2] = z;
}

// Ops already in correct order, do nothing
void remove_add2_nop()
{
    vInt x = 1;
    vInt y = 2;

    vInt z = y + x;

    dst_reg[0] = x;
    dst_reg[2] = z;
}

// Can't swap due to subsequent use
void replace_add3()
{
    vInt x = 1;
    vInt y = 2;

    vInt z = x + y;

    dst_reg[0] = x;
    dst_reg[1] = y;
    dst_reg[2] = z;
}

// Can't swap w/ a subtract
void replace_sub1()
{
    vInt x = 1;
    vInt y = 2;

    vInt z = y - x;

    dst_reg[0] = x;
    dst_reg[2] = z;
}

// Can swap
void remove_and1()
{
    vInt x = 1;
    vInt y = 2;

    vInt z = x & y;

    dst_reg[0] = x;
    dst_reg[2] = z;
}

// Ops already in correct order, do nothing
void remove_and2_nop()
{
    vInt x = 1;
    vInt y = 2;

    vInt z = y & x;

    dst_reg[0] = x;
    dst_reg[2] = z;
}

// Can't swap due to subsequent use
void replace_and3()
{
    vInt x = 1;
    vInt y = 2;

    vInt z = x & y;

    dst_reg[0] = x;
    dst_reg[1] = y;
    dst_reg[2] = z;
}

// Can swap
void remove_or1()
{
    vInt x = 1;
    vInt y = 2;

    vInt z = x | y;

    dst_reg[0] = x;
    dst_reg[2] = z;
}

// Ops already in correct order, do nothing
void remove_or2_nop()
{
    vInt x = 1;
    vInt y = 2;

    vInt z = y | x;

    dst_reg[0] = x;
    dst_reg[2] = z;
}

// Can't swap due to subsequent use
void replace_or3()
{
    vInt x = 1;
    vInt y = 2;

    vInt z = x | y;

    dst_reg[0] = x;
    dst_reg[1] = y;
    dst_reg[2] = z;
}

// Test that the move happens in the right place and uses the right resulting
// register w/ a liveness issue
void replace_live_or4()
{
    vInt x = 1;
    vInt y = 2;

    vInt z = 3;
    v_if (x == 4) {
        z = x | y;
    }
    v_endif;

    dst_reg[0] = x;
    dst_reg[1] = y;
    dst_reg[2] = z;
}

void remove_cmp1()
{
    vInt x = 1;
    vInt y = 2;

    // Below get combined
    vInt z = y + x;
    v_if (z < 0) {
    }
    v_endif;

    dst_reg[0] = x;
    dst_reg[2] = z;
    
}

void replace_cmp2()
{
    vInt x = 1;
    vInt y = 2;

    // Below get combined, can't remove the move due to subtract
    vInt z = y - x;
    v_if (z < 0) {
    }
    v_endif;

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
    vFloat x = dst_reg[0];
    vFloat y;

    y = x * 2.0f;

    dst_reg[0] = x;
    dst_reg[0] = y;
}

// See comment above
void replace_addi()
{
    vFloat x = dst_reg[0];
    vFloat y;

    y = x + 2.0f;

    dst_reg[0] = x;
    dst_reg[0] = y;
}

void replace_shft()
{
    vInt x = 1;
    vInt y;

    y = x << 1;

    dst_reg[0] = x;
    dst_reg[2] = y;
}

void replace_creg()
{
    vInt x = vConstTileId;
    vInt y = vConstTileId;
    dst_reg[0] = x << 1;
    dst_reg[0] = x & y;
    dst_reg[0] = y | x;
}

void bb(int argc)
{
    vInt a = 1;
    vInt b = 2;
    vInt c = 3;

    if (argc == 0) {
        c = a | b;
    }

    dst_reg[0] = c;
}

void bb2(int argc)
{
    vInt a = 1;
    vInt b = 2;
    vInt c = 3;

    if (argc == 0) {
        c = a | b;
    }

    dst_reg[0] = a;
    dst_reg[0] = c;
}

void bb3(int argc)
{
    vInt a = 1;
    vInt c = 3;

    if (argc == 0) {
        c = a | c;
    }

    dst_reg[0] = a;
    dst_reg[0] = c;
}

// Setexp, setman, setsgn use dst as src but can't swap the order of args
// Setexp, setman, setsgn use dst as src but can't swap the order of args
// Test that moves get generated properly
void setexp_mov()
{
    vFloat x = 1.0f;
    vInt y = 50;

    vFloat z = setexp(x, y);

    dst_reg[0] = x;
    dst_reg[1] = y;
    dst_reg[2] = z;
}

void setman_mov()
{
    vFloat x = 1.0f;
    vInt y = 50;

    vFloat z = setman(x, y);

    dst_reg[0] = x;
    dst_reg[1] = y;
    dst_reg[2] = z;
}

void setsgn_mov()
{
    vFloat x = 1.0f;
    vInt y = 50;

    vFloat z = setsgn(x, y);

    dst_reg[0] = x;
    dst_reg[1] = y;
    dst_reg[2] = z;
}

// Can't swap due to subsequent use
void replace_xor3()
{
    vInt x = 1;
    vInt y = 2;

    vInt z = x ^ y;

    dst_reg[0] = x;
    dst_reg[1] = y;
    dst_reg[2] = z;
}

void swap_const_reg_or()
{
    vInt x = 2;

    vInt y = x | vConstTileId;

    dst_reg[0] = x;
    dst_reg[0] = y;
}

void move_const_reg_or()
{
    vInt x = 2;

    vInt y = vConstTileId | x;

    dst_reg[0] = y;
}

// OK, stupid case, but could happen: both params are const regs
void move_const_reg2_or()
{
    vInt y = vConstTileId | vInt(vConstTileId);

    dst_reg[0] = y;
}
