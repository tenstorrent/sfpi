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

int main(int argc, char* argv[])
{
    abs_setcc();
    rename_move_case2a();
    copy_move_case2b();
    copy_move_case3();
    internal_move_case4();
    internal_move_case5();
}
