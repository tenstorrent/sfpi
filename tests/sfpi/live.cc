// Test cases for esoteric liveness.  See liveness word doc
#include <stdio.h>
#include <stdlib.h>
#include "test.h"

using namespace sfpi;

void abs_setcc()
// ABS liveness across SETCC
{
    VecHalf x = -20.0F;
    VecHalf y = -30.0F;
    p_if (dst_reg[0] == 0.0F) {
        y = abs(x);
    }
    p_endif;
    dst_reg[13] = y;
}

// Assignment resulting in register rename
void rename_move_case2a()
{
    VecHalf a = 1.0f;  // LOADI
    VecHalf b = 2.0f;  // LOADI
    p_if (dst_reg[0] == 0.0f) {  // PUSHC, ..., SETCC
        b = a;         // should generate MOV
    }
    p_endif;           // POPC
    dst_reg[0] = b;      // STORE
}

// Assignment requiring move
// This straddles case 2a and 3 - both values need to be preserved but the
// compiler doesn't know that, solving case2a will solve this case as well
void copy_move_case2b()
{
    VecHalf a = 1.0f;  // LOADI
    VecHalf b = 2.0f;  // LOADI
    p_if (dst_reg[0] == 0.0f) {  // PUSHC, ..., SETCC
        b = a;         // should generate MOV
    }
    p_endif;           // POPC
    dst_reg[0] = a; // STORE
    dst_reg[0] = b; // STORE
}

// Assignment requiring move (both a and b need to be preserved)
void copy_move_case3()
{
    VecHalf a = 1.0f;  // LOADI
    VecHalf b = 2.0f;  // LOADI
    p_if (dst_reg[0] == 0.0f) {  // PUSHC, ..., SETCC
        b = a;         // should generate MOV
    }
    p_endif;           // POPC
    dst_reg[0] = a + 1.0f; // STORE
    dst_reg[0] = b + 1.0f; // STORE
}

// Destination as source, 2 arguments in the wrong order
void internal_move_case4()
{
    VecShort a = 10; // LOADI
    VecShort b = 20; // LOADI
    p_if (dst_reg[0] == 0.0f) {      // PUSHC, ..., SETCC
        a = a - b;   // CCMOV, IADD
        // Wrapper emits:
        //   tmp = b;
        //   tmp = a - tmp;
        //   a = tmp;
    }
    p_endif;         // POPC
    dst_reg[0] = a;    // STORE
    dst_reg[0] = b;    // STORE
}

// Destination as source 3 arguments
void internal_move_case5()
{
    VecShort a = 10; // LOADI
    VecShort b = 20; // LOADI
    VecShort c = 30; // LOADI
    p_if (dst_reg[0] == 0.0f) {      // PUSHC, ..., SETCC
        c = a - b;   // MOV, IADD

        // Wrapper emits:
        //   tmp = b;
        //   tmp = a - tmp;
        //   c = tmp;

        // really
        // c = b
        // c = a - c
    }
    p_endif;         // POPC
    dst_reg[0] = a;    // STORE
    dst_reg[0] = b;    // STORE
    dst_reg[0] = c;    // STORE
}

void bb_1()
{
    VecShort a = 1;
    VecShort b = 2;

    for (int i = 0; i < rand(); i++) {
        p_if (a >= 3) {
            if (rand()) {
                continue;
            }
            a = b + 4;
        } p_elseif (a >= 5) {
            if (rand()) {
                break;
            }
            a = b + 6;
        }
        p_endif;
    }

    dst_reg[0] = a;
}

void bb_2()
{
    VecShort a = 1;
    VecShort b = 2;

    for (int i = 0; i < rand(); i++) {
        p_block {
            switch (i) {
            case 0:
                a = b - 3;
                break;
            case 1:
                p_and(a >= 4);
                break;
            default:
                break;
            }

            // Use a builtin...
            a = b - 5;
        }

        p_endblock;
    }

    dst_reg[0] = a;
}

void bb_3()
{
    VecShort a = 1;
    VecShort b = 2;

    for (int i = 0; i < rand(); i++) {
        if (rand() == 0) {
            goto target2;
        }

        p_if (a >= 3) {
            if (rand() > 2) {
                goto target1;
            }
            a = b - 4;
        } p_elseif(a >= 5) {
        target1:
            a = b - 6;
        }
        p_endif;
    }
 target2:

    dst_reg[0] = a;
}

void bb_4()
{
    VecShort a = 1;
    VecShort b = 2;

    p_block {

        p_and(b >= 1);

        if (rand() == 5)
            goto seq1;

        if (rand() == 6)
            goto seq2;

        if (rand() == 6)
            goto seq2;

    seq1:
        p_and(b >= 2);
        goto join;

    seq2:
        p_and(b >= 3);
        p_and(b >= 4);
        
    join:
        a = 5;
    }
    p_endif;

    dst_reg[0] = a;
}

// Test cleaning up args in PHI nodes after deleting the live assignment
void bb_not_live()
{
    VecShort a = 1;
    VecShort b = 2;

    for (int i = 0; i < rand(); i++) {
        a = b - 3;
    }

    a = b - 4;

    dst_reg[0] = a;
}

// Liveness pass (used to) handle unrolling the POPC loop
void popc_unrolling()
{
    VecShort a = 1;
    VecShort b = 1;

    p_if (a >= 2) {
        a = b - 1;
    } p_elseif (a >= 3) {
        a = b - 1;
    } p_elseif (a >= 4) {
        a = b - 1;
    } p_elseif (a >= 5) {
        a = b - 1;
    }
    p_endif;

    p_if (a >= 6) {
        a = b - 1;
    } p_elseif (a >= 7) {
        a = b - 1;
    } p_elseif (a >= 8) {
        a = b - 1;
    }
    p_endif;

    dst_reg[0] = a;
}


// This tests to be sure that 2 assignments at a deeper CC level both work,
// ie, that the 2nd one doesn't find the prior assignment and use its
// liveness, but continues up the chain to the origin
void double_assign_even()
{
    VecShort a = 1;
    VecShort b = 2;

    p_if (a == 3) {
        a = b & a;
        VecShort b = 5;
        VecShort c = 7;
        VecShort d = 8;
        dst_reg[0] = b;
        dst_reg[0] = c;
        dst_reg[0] = d;

        a = ~a;
    }
    p_endif;

    dst_reg[0] = a;
}

// The code below tests the case where we descend down a CC cascade, pop
// back up, then back down w/ different CC bits set.  Does the variable
// stay live when assigned at the same CC level but in a different
// cascade, ie, across generations?
void generation()
{
    VecHalf a;
    VecHalf b;
    VecHalf dr = dst_reg[0];

    p_if (dr == 20.0F || dr == 21.0F) {
        b = -90.0F;
    }
    p_endif;

    p_if (dr == 20.0F) {
        a = 100.0F;
    }
    p_endif;

    p_if (dr == 21.0F) {
        a = 110.0F;
    }
    p_endif;

    p_if (dr == 21.0F) {
        b = a;
    }
    p_endif;

    p_if (dr == 20.0F || dr == 21.0F) {
        dst_reg[14] = b;
    }
    p_endif;

    p_if (dr == 500.0F) {
        dst_reg[14] = a;
    }
    p_endif;
}

// Can liveness be determined when there is an intervening pseudo live insn
void prop_thru_pseudo_live()
{
    VecShort a = 1;
    VecShort b = 2;

    p_if (a == 4) {
        a = b & a;
        a = b | a;
        a = b + a;

        a = a & b;
        a = a | b;
        a = a + b;
    }
    p_endif;

    dst_reg[0] = a;
}

int main(int argc, char* argv[])
{
    abs_setcc();
    rename_move_case2a();
    copy_move_case2b();
    copy_move_case3();
    internal_move_case4();
    internal_move_case5();
    bb_1();
    bb_2();
    bb_3();
    bb_4();
    bb_not_live();
    popc_unrolling();
    double_assign_even();
    generation();
    prop_thru_pseudo_live();
}
