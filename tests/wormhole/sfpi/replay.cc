// Test the sfpu replay pass
// Current implementation is fairly simple and will miss the opportunity to
// use replay in many "complex" situations (including unrolled loops).  So,
// much of this testing is fairly simple...

#include <stdlib.h>

#include <cstdio>
#include <ckernel_ops.h>
#include "test.h"

using namespace sfpi;

void no_replay()
{
    dst_reg[0] = dst_reg[0];
}

// Simplest case
void replay4()
{
    for (int i = 0; i < 4; i++) {
        vFloat x = dst_reg[0];
        vFloat y = addexp(x, 3);
        dst_reg[0] = y;
    }
}

// Simple case, however due to the short length of the insn sequence and the
// high unroll count this basically unrolls by 2 and generates 6 replays
void replay6()
{
    for (int i = 0; i < 12; i++) {
        vFloat x = dst_reg[0];
        vFloat y = addexp(x, 3);
        dst_reg[0] = y;
    }
}

// This is interesting because the LOAD doesn't happen at the start and the
// last iteration of the loop doesn't have an sfpmov
void replay_load_in_the_middle()
{
    vInt x = 1;

    for (int i = 0; i < 8; i++) {
        x++;
        v_if(x == 4) {
            x = 0;
        }
        v_endif;

        vFloat y = dst_reg[0];
        y = setexp(y, x);
        dst_reg[0] = y;
    }
}

void replay_assignlreg_const_reg()
{
    for (int i = 0; i < 8; i++) {
        vFloat x = dst_reg[0];

        vUInt lr3 = l_reg[LRegs::LReg3];

        x = setman(x, lr3);
        dst_reg[0] = x;

        l_reg[LRegs::LReg3] = lr3;
    }
}

void replay_partial_top()
{
    vFloat x = dst_reg[0];
    x = x * 5.0f;
    dst_reg[0] = x;

    x = dst_reg[0];
    x = x * 5.0f;
    dst_reg[0] = x;

    x = dst_reg[0];
    x = x * 5.0f;
    dst_reg[0] = x;

    x = dst_reg[0];
    x = x * 5.0f;
    dst_reg[0] = abs(x);
}

void replay_partial_bot()
{
    vFloat x = dst_reg[0];
    x = x * 5.0f;
    dst_reg[0] = x;

    x = dst_reg[0];
    x = x * 5.0f;
    dst_reg[0] = x;

    x = dst_reg[0];
    x = x * 5.0f;
    dst_reg[0] = x;

    x = 3.2f;
    x = x * 5.0f;
    dst_reg[0] = x;
}

void replay_nonimm(int in)
{
    for (int i = 0; i < 8; i++) {
        vFloat x = dst_reg[0];
        vInt y = in;
        x = setman(x, y);
        dst_reg[0] = x;
    }
}

// Modify the insn in a loop, no replay
void replay_nonimm2(int in)
{
    #pragma GCC unroll 8
    for (int i = 0; i < 8; i++) {
        vFloat x = dst_reg[0];
        vInt y = in + i;
        x = setman(x, y);
        dst_reg[0] = x;
    }
}

// Modify again, but replay should work for the first couple loops
void replay_nonimm3(int in)
{
    #pragma GCC unroll 8
    for (int i = 0; i < 8; i++) {
        dst_reg[0] = dst_reg[in];
    }
    dst_reg[0] = dst_reg[in+1];
    #pragma GCC unroll 8
    for (int i = 0; i < 8; i++) {
        dst_reg[0] = dst_reg[in];
    }
}

int replay_jump_in_middle(int in1, int in2)
{
    #pragma GCC unroll 8
    for (int i = 0; i < 8; i++) {
        vFloat x = dst_reg[0];
        x = setman(x, 5);
        if (i & in1) {
            in1 ^= in2;
        }
        dst_reg[0] = x;
    }

    return in1;
}

void replay_tti_asm()
{
    #pragma GCC unroll 8
    for (int i = 0; i < 8; i++) {
        vFloat x = dst_reg[0];
        x = x * 5.0f;
        TTI_SFPMUL(0, 0, 0, 0, 0);
        dst_reg[0] = x;
    }
}
