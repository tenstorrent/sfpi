#include <stdlib.h>

#include <cstdio>
#include "test.h"

using namespace sfpi;

void iadd_i_yes1()
{
    vInt a = 3;

    a = a - 5;
    v_if (a < 0) {
        dst_reg[0] = vConst1;
    }
    v_endif;
}

void iadd_i_yes1_live()
{
    vInt a = 3;

    v_if (a < 0) {
        a = a - 5;
        v_if (a < 0) {
            dst_reg[0] = vConst1;
        }
        v_endif;
    }
    v_endif;
}

void iadd_i_yes2()
{
    vInt a = 3;
    v_if ((a = a - 7) < 0) {
        dst_reg[0] = vConst1;
    }
    v_endif;
}

void iadd_i_yes3a()
{
    vInt a = 3;
    vInt b = 4;

    v_if ((a = a - 5) < 0 && b == 6) {
        dst_reg[0] = vConst1;
    }
    v_endif;
    dst_reg[0] = a;
    dst_reg[0] = b;
}

void iadd_i_yes3b()
{
    vInt a = 3;
    vInt b = 4;

    v_if (b == 6 && (a = a - 5) < 0) {
        dst_reg[0] = vConst1;
    }
    v_endif;
}

void iadd_i_yes3c()
{
    vInt a = 3;
    vInt b = 4;

    v_if (b == 6 && ((a = a - 5) < 0)) {
        dst_reg[0] = vConst1;
    }
    v_endif;
}

void iadd_i_yes3d()
{
    vInt a = 3;

    a = a - 5;
    v_if ((a = a - 0) < 0) {
        dst_reg[0] = vConst1;
    }
    v_endif;
}

void iadd_i_yes4a()
{
    vInt a = 3;
    vInt b = 4;

    // SetCC comes after iadds
    a = a - 5;
    v_if (a < 0 && b == 6) {
        dst_reg[0] = vConst1;
    }
    v_endif;
}

void iadd_i_yes5()
{
    vInt a = 3;

    // Testing 2 things (+ and >=)
    a = a + 5;
    v_if (a >= 0) {
        dst_reg[0] = vConst1;
    }
    v_endif;
}

void iadd_i_yes6()
{
    vInt a = 3;
    vInt b = 4;

    // Make sure registers in new iadd are correct
    v_if (a >= 5) {
        a = b + 6;
        v_if (a >= 0) {
            dst_reg[0] = vConst1;
        }
        v_endif;
    }
    v_endif;
}

void iadd_i_yes7()
{
    vInt a = 3;
    vInt b = 4;

    v_if ((a = b + 6) >= 0) {
        dst_reg[0] = vConst1;
    }
    v_endif;
    dst_reg[0] = a;
    dst_reg[0] = b;
}

void iadd_i_yes8()
{
    vInt a = 3;
    vInt b;

    // Make sure registers are correct, results carry through
    b = a + 5;
    v_if (b < 0) {
        dst_reg[0] = vConst1;
    }
    v_endif;
    dst_reg[1] = a;
    dst_reg[2] = b;
}

void iadd_i_yes9()
{
    vInt a = 3;
    vInt b = 5;

    a = a - 7;
    v_if (a < 0) {
        v_if (b < 9) {
            dst_reg[0] = vConst1;
        }
        v_endif;
    }
    v_endif;
}

void iadd_i_no1()
{
    vInt a = 3;
    vInt b = 4;

    // No, mismatched arguments
    b = b - 6;
    v_if (a < 0) {
        dst_reg[0] = vConst1;
    }
    v_endif;
}

void iadd_i_no2()
{
    vInt a = 3;
    vInt b;

    // No, mismatched arguments
    b = a - 5;
    v_if (a < 0) {
        dst_reg[0] = vConst1;
    }
    v_endif;
}

void iadd_i_no3()
{
    vInt a = 3;

    a = a - 5;
    // No then yes
    v_if ((a = a - 7) < 0) {
        dst_reg[0] = vConst1;
    }
    v_endif;
    dst_reg[0] = a;
}

void iadd_i_no4()
{
    vInt a = 3;
    vInt b = 4;

    // No, no CC
    a = a - 5;
    a = a - 7;
    a = b - 6;
    a = a - 9;
    dst_reg[0] = a;
}

void iadd_i_no4b()
{
    vInt a = 3;
    vInt b = 4;

    // Can't assign a after testing b == 6
    a = a - 5;
    v_if (b >= 6 && a < 0) {
        dst_reg[0] = vConst1;
    }
    v_endif;
}

void iadd_i_no5()
{
    vInt a = 3;
    vInt b = 4;

    // No, mismatched arguments and both set CC
    v_if (a < 0 && b < 0) {
        dst_reg[0] = vConst1;
    }
    v_endif;
}

void iadd_i_no6()
{
    vInt a = 3;

    // No, 2nd doesn't set the CC
    a = a + 5;
    // Yeah, really
    a = a + 0;
}

void iadd_i_no7()
{
    vInt a = 3;
    vInt b;

    // No, intervening use
    a = a - 5;
    b = a + 7;
    v_if (a < 0) {
        dst_reg[0] = vConst1;
    }
    v_endif;
}

void iadd_i_no8()
{
    vInt a = 3;
    vInt b = 5;

    // No, intervening CC
    a = a - 7;
    v_if (b < 9) {
        v_if (a < 0) {
            dst_reg[0] = vConst1;
        }
        v_endif;
    }
    v_endif;
}

void iadd_i_no9()
{
    vInt a = 3;

    // No - combines w/ the +0
    a = a - 3;
    v_if ((a = a + 0) < 0) {
        dst_reg[0] = vConst1;
    }
    v_endif;

    dst_reg[0] = a;
}

void iadd_i_no10()
{
    vInt a = 3;

    int x = rand();

    // No, crosses BB
    a = a - 5;

    if (x == 0) {
        v_if (a < 0) {
            dst_reg[0] = vConst1;
        }
        v_endif;
    } else if (x == 1) {
        v_if (a < 0) {
            dst_reg[0] = vConst0;
        }
        v_endif;
    }
}

void iadd_i_no11()
{
    vInt a = 3;

    // No, intervening COMPC
    a = a - 5;
    v_if (a < 0) {
        a = a + 2;
    } v_elseif(a < 0) {
        dst_reg[0] = vConst1;
    }
    v_endif;
}

void iadd_i_no12(unsigned int imm)
{
    vInt a = 3;

    // No, intervening COMPC
    a = a - 5;
    v_if (a < imm) {
        a = a + 2;
    }
    v_endif;
    dst_reg[0] = a;
}

void iadd_i_no_bb1(int count)
{
    vInt a = 1;

    a = a - 5;

#pragma GCC unroll 0
    for (int i = 0; i < count; i++) {
        v_if (a < 0) {
            a += 1;
        }
        v_endif;
    }
    dst_reg[0] = a;
}

void iadd_v_yes1()
{
    vInt a = 3;
    vInt b = 4;

    a = b - a;
    v_if (a < 0) {
        dst_reg[0] = vConst1;
    }
    v_endif;
}

void iadd_v_yes2()
{
    vInt a = 3;
    vInt b = 4;

    a = a - b;
    v_if (a >= 0) {
        dst_reg[0] = vConst1;
    }
    v_endif;
}

void iadd_v_yes3()
{
    vInt a = 3;
    vInt b = 4;

    a = a + b;
    v_if (a < 0) {
        dst_reg[0] = vConst1;
    }
    v_endif;
}

void iadd_v_yes4a()
{
    vInt a = 3;
    vInt b = 4;

    // No move requried to save the dst
    v_if (a - b >= 0) {
        dst_reg[0] = vConst1;
    }
    v_endif;
}

void iadd_v_yes4b()
{
    vInt a = 3;
    vInt b = 4;

    // Requires a move to save the dst
    v_if (a + b < 0) {
        dst_reg[0] = vConst1;
    }
    v_endif;

    dst_reg[0] = b;
}

void iadd_v_no1()
{
    vInt a = 3;
    vInt b = 4;

    a = b + a;
    // No, compare against non-zero
    v_if (a < 5) {
        dst_reg[0] = vConst1;
    }
    v_endif;
}

void iadd_v_no2()
{
    vInt a = 3;
    vInt b = 4;

    // No, iadd_v can't use imm
    v_if (a - b < 5) {
        dst_reg[0] = vConst1;
    }
    v_endif;
}

void iadd_v_no3()
{
    vInt a = 3;
    vInt b = 4;

    // No, 2nd iadd doesn't set the CC
    a = a - b;
    a = a - 0;
}

void iadd_v_no4()
{
    vInt a = 3;
    vInt b = 4;

    // No, reversing the ops
    v_if (a - b < 0) {
        a = a - 5;
        dst_reg[0] = vConst1;
    }
    v_endif;
}

void iadd_v_no_bb1(int count)
{
    vInt a = 1;
    vInt b = 2;

    a = b - a;

#pragma GCC unroll 0
    for (int i = 0; i < count; i++) {
        v_if (a < 0) {
            a += 1;
        }
        v_endif;
    }
    dst_reg[0] = a;
}

void lz_yes1()
{
    vFloat a = 3.0f;
    vInt b;

    b = lz(a);
    v_if (a == 0) {
        dst_reg[0] = vConst1;
    }
    v_endif;
    dst_reg[0] = a;
    dst_reg[0] = b;
}

void lz_yes2()
{
    vFloat a = 3.0f;
    vInt b;

    b = lz(a);
    v_if (a != 0) {
        dst_reg[0] = vConst1;
    }
    v_endif;
    dst_reg[0] = a;
    dst_reg[0] = b;
}

void lz_no1()
{
    vFloat a = 3.0f;
    vInt b;

    b = lz(a);
    v_if (b != 0) {
        dst_reg[0] = vConst1;
    }
    v_endif;
    dst_reg[0] = a;
    dst_reg[0] = b;
}

void lz_no2()
{
    vFloat a = 3.0f;
    vInt b;

    b = lz(a);
    v_if (a >= 0) {
        dst_reg[0] = vConst1;
    }
    v_endif;
    dst_reg[0] = a;
    dst_reg[0] = b;
}

void lz_no3()
{
    vFloat a = 3.0f;
    vInt b;

    b = lz(a);
    v_if (a < 0) {
        dst_reg[0] = vConst1;
    }
    v_endif;
    dst_reg[0] = a;
    dst_reg[0] = b;
}

void lz_no_bb1(int count)
{
    vFloat a = 3.0f;
    vInt b;

    b = lz(a);

#pragma GCC unroll 0
    for (int i = 0; i < count; i++) {
        v_if (a < 0) {
            a += 1;
        }
        v_endif;
    }
    dst_reg[0] = a;
}

void exexp_yes1()
{
    vFloat a = 3.0f;
    vInt b;

    b = exexp(a);
    v_if (b >= 0) {
        dst_reg[0] = vConst1;
    }
    v_endif;
    dst_reg[0] = a;
    dst_reg[0] = b;
}

void exexp_yes1_live()
{
    vFloat a = 3.0f;
    vInt b = 1;

    v_if (a < 0) {
        b = exexp(a);
        v_if (b >= 0) {
            dst_reg[0] = vConst1;
        }
        v_endif;
    }
    v_endif;
    dst_reg[0] = a;
    dst_reg[0] = b;
}

void exexp_yes2()
{
    vFloat a = 3.0f;
    vInt b;

    b = exexp(a);
    v_if (b < 0) {
        dst_reg[0] = vConst1;
    }
    v_endif;
    dst_reg[0] = a;
    dst_reg[0] = b;
}

void exexp_yes3()
{
    vFloat a = 3.0f;

    a = reinterpret<vFloat>(exexp(a));
    v_if (reinterpret<vInt>(a) < 0) {
        dst_reg[0] = vConst1;
    }
    v_endif;
    dst_reg[0] = a;
}

void exexp_no1()
{
    vFloat a = 3.0f;
    vInt b;

    // No, compare not against 0
    b = exexp(a);
    v_if (b < 5) {
        dst_reg[0] = vConst1;
    }
    v_endif;
    dst_reg[0] = a;
    dst_reg[0] = b;
}

void exexp_no2()
{
    vFloat a = 3.0f;
    vInt b;

    b = exexp_nodebias(a);
    // No, nodebias doesn't have a set CC to sign option
    v_if (b >= 0) {
        dst_reg[0] = vConst1;
    }
    v_endif;
    dst_reg[0] = a;
    dst_reg[0] = b;
}

void exexp_no_bb1(int count)
{
    vFloat a = 3.0f;
    vInt b;

    b = exexp(a);

#pragma GCC unroll 0
    for (int i = 0; i < count; i++) {
        v_if (b >= 0) {
            dst_reg[0] = vConst1;
        }
        v_endif;
    }

    dst_reg[0] = a;
    dst_reg[0] = b;
}

void mad_yes1()
{
    vFloat a = dst_reg[0];
    vFloat b = dst_reg[1];
    vFloat c = dst_reg[2];
    vFloat d;

    d = a * b + c;

    dst_reg[0] = d;
}

// This will result in out of registers if mad conversion fails
void mad_yes2()
{
    vFloat a = dst_reg[0];
    vFloat b = dst_reg[1];
    vFloat c = dst_reg[2];
    vFloat d, tmp;

    tmp = a * b;
    d = c + tmp;

    dst_reg[0] = d;

    tmp = a * b;
    d = tmp + c;

    dst_reg[0] = d;
}

void mad_yes3()
{
    vFloat a = dst_reg[0];
    vFloat b = dst_reg[1];
    vFloat c = dst_reg[2];
    vFloat d, tmp;

    d = a * (vConst1 * (a * b + c) + vConst1) + c;

    dst_reg[0] = d;
}

// This isn't handled due to concern about register pressure
// Reordering code would make it happen, so not a huge concern
void mad_maybe_future1()
{
    vFloat a = dst_reg[0];
    vFloat b = dst_reg[1];
    vFloat c = dst_reg[2];
    vFloat d = 4.0f;
    vFloat tmp1, tmp2;

    tmp1 = a * b;
    tmp2 = c * d;

    dst_reg[0] = tmp1 + c;
    dst_reg[0] = tmp2 + c;
}

// This is not handled, but could be
void mad_maybe_future2()
{
    vFloat a = dst_reg[0];
    vFloat b = dst_reg[1];
    vFloat tmp, d, e;
    
    tmp = a * b;
    d = tmp + dst_reg[2];
    e = tmp + dst_reg[3];

    dst_reg[0] = d;
    dst_reg[0] = e;
}

void mad_live_yes1()
{
    vFloat a = dst_reg[0];
    vFloat b = dst_reg[1];
    vFloat c = dst_reg[2];
    vFloat d = dst_reg[3];

    v_if (a == 0.0f) {
        vFloat tmp = a * b;
        d = tmp + c;
    }
    v_endif;

    dst_reg[0] = d;
}

// This works on Grayskull w/o creating a mad, but will fail if the mad is
// created by moving the multiply down to the add
void mad_reg_pressure_no4()
{
    vFloat a = dst_reg[0];
    vFloat b = dst_reg[1];
    vFloat c = dst_reg[2];
    vFloat d;

    vFloat tmp = a * b;
    vInt x = exexp(a);
    vInt y = exexp(b);
    d = tmp + c;

    dst_reg[0] = c;
    dst_reg[0] = d;
    dst_reg[1] = x;
    dst_reg[1] = y;
}

void mad_no1()
{
    vFloat a = dst_reg[0];
    vFloat b = dst_reg[1];
    vFloat c = dst_reg[2];
    vFloat d, tmp;

    // Subsequent use
    tmp = a * b;
    d = c + tmp;

    dst_reg[0] = tmp;
    dst_reg[0] = d;
}

void mad_no2()
{
    vFloat a = dst_reg[0];
    vFloat b = dst_reg[1];
    vFloat c = dst_reg[2];
    vFloat d, tmp;

    // Prior use
    tmp = a * b;
    dst_reg[0] = tmp;
    d = c + tmp;

    dst_reg[0] = d;
}

void mad_no3()
{
    vFloat a = dst_reg[0];
    vFloat b = dst_reg[1];
    vFloat c = dst_reg[2];
    vFloat d, tmp;

    // Intervening CC
    tmp = a * b;
    v_if (a >= 0.0F) {
    }
    v_endif;

    d = c + tmp;

    dst_reg[0] = d;
}

// Since mads increase register pressure, not going to let the compiler turn
// this into a mad, require instructions to be "closer" together
void mad_live_no1()
{
    vFloat a = dst_reg[0];
    vFloat b = dst_reg[1];
    vFloat c = dst_reg[2];
    vFloat d = dst_reg[3];
    vFloat e = a * b;

    // This can be a yes because e is not used later
    // and d is at a higher CC level than e
    v_if (a == 0.0f) {
        d = e + c;
    }
    v_endif;

    dst_reg[0] = d;
}

void mad_live_no2()
{
    vFloat a = dst_reg[0];
    vFloat b = dst_reg[1];
    vFloat c = dst_reg[2];
    vFloat d = dst_reg[3];
    vFloat e = a * b;

    v_if (a == 0.0f) {
        d = e + c;
    }
    v_endif;

    dst_reg[0] = d;
    dst_reg[0] = e;
}

void mad_no_bb1(int count)
{
    vFloat a = dst_reg[0];
    vFloat b = dst_reg[1];
    vFloat c = dst_reg[2];

    vFloat d = a * b;

#pragma GCC unroll 0
    for (int i = 0; i < count; i++) {
        d = d + c;
    }

    dst_reg[0] = d;
}

void addi_yes1()
{
    vFloat a = 1.0f;
    vFloat b = 2.0f;

    vFloat c = a + b;
    dst_reg[0] = c;
}

// Will turn into 2 addi's but still have the loadis
void addi_yes_caveat()
{
    vFloat a = 1.0f;
    vFloat b = 2.0f;

    vFloat c = a + b;
    dst_reg[0] = c;

    c = b + a;
    dst_reg[1] = c;
}

// Will ditch one of the loadis
void addi_yes2()
{
    vFloat a = 1.0f;
    vFloat b = 2.0f;

    vFloat c = a + b;
    vFloat d = c + b;
    vFloat e = d + b;
    dst_reg[0] = e;
}

void addi_yes3()
{
    vFloat a = dst_reg[0];
    vFloat b = dst_reg[1];

    vFloat c = a * b + 1.0f + 2.0f;  // Mad + Addi
    dst_reg[0] = c;
}

// Pick the right one based on subsequent use
void addi_yes4()
{
    {
        vFloat a = 1.0f;
        vFloat b = 2.0f;

        vFloat c = a + b;
        dst_reg[0] = c;
        dst_reg[0] = a;
    }
    {
        vFloat a = 1.0f;
        vFloat b = 2.0f;

        vFloat c = a + b;
        dst_reg[0] = c;
        dst_reg[0] = b;
    }
}

void addi_yes_imm1(unsigned int imm)
{
    vFloat a = 1.0f;
    vFloat b = s2vFloat16b(imm);

    vFloat c = a + b;
    dst_reg[0] = c;
}

void addi_yes_imm2(unsigned int imm)
{
    vFloat a = s2vFloat16b(imm);
    vFloat b = 1.0f;

    vFloat c = a + b;
    dst_reg[0] = c;
}

void addi_live_yes1()
{
    vFloat b = 2.0f;

    v_if (dst_reg[0] == 0.0f) {
        vFloat a = 3.0f;
        b = a + b;
    }
    v_endif;

    dst_reg[0] = b;
}

void addi_live_yes2()
{
    vFloat b = 2.0f;

    v_if (dst_reg[0] == 0.0f) {
        vFloat a = 3.0f;
        b = b + a;
    }
    v_endif;

    dst_reg[0] = b;
}

void addi_live_yes3_future()
{
    vFloat b = 2.0f;
    vFloat a = 3.0f;

    // Could handle this, but opens up more potential for bugs
    v_if (dst_reg[0] == 0.0f) {
        b = b + a;
    }
    v_endif;

    dst_reg[0] = b;
}

void addi_live_yes4_future()
{
    vFloat a = 3.0f;
    vFloat b = 2.0f;

    v_if (dst_reg[0] == 0.0f) {
        // This is a loadi_lv
        // Could handle this, but opens up more potential for bugs
        a = 4.0f;
        b = b + a;
    }
    v_endif;

    dst_reg[0] = b;
}

void addi_live_no2()
{
    vFloat b = 2.0f;
    vFloat c = 1.0f;

    v_if (dst_reg[0] == 0.0f) {
        // Requires a move of addi result into C, non-optimal
        vFloat a = 3.0f;
        c = a + b;
    }
    v_endif;

    dst_reg[0] = c;
}

void addi_live_no3()
{
    vFloat a = 1.0f;
    vFloat b = 2.0f;
    vFloat c = 1.0f;

    v_if (dst_reg[0] == 0.0f) {
        // Requires a move of addi result into C, non-optimal
        a = 3.0f;
        c = a + b;
    }
    v_endif;

    dst_reg[0] = c;
}

void addi_live_no4()
{
    vFloat a = 1.0f;
    vFloat b = dst_reg[0];
    vFloat c = 1.0f;

    v_if (dst_reg[0] == 0.0f) {
        // No.  Could do an addi since a is not reused,
        // but not particularly interesting
        a = 3.0f;
        c = a + b;
    }
    v_endif;

    dst_reg[0] = c;
}

void addi_no1()
{
    vFloat a = 1.0f;
    vFloat b = dst_reg[0];

    // a not a pure loadi
    v_if (dst_reg[0] == 0.0f) {
        a = 3.0f;
    }
    v_endif;

    vFloat c = a + b;

    dst_reg[0] = c;
}

// Subsequent use makes this perf-undesirable
void addi_no2()
{
    vFloat a = 1.0f;
    vFloat b = 2.0f;

    vFloat c = a + b;
    dst_reg[0] = a;
    dst_reg[0] = b;
    dst_reg[0] = c;
}

void addi_no_bb1(int count)
{
    vFloat a = 1.0f;
    vFloat b = 2.0f;

    vFloat c;
#pragma GCC unroll 0
    for (int i = 0; i < count; i++) {
        c = a + b;
    }

    dst_reg[0] = c;
}

// Muli and addi are handled w/ the same code, don't need to heavily
// test muli
void muli_yes1()
{
    vFloat a = 1.0f;
    vFloat b = 2.0f;

    vFloat c = a * b;
    dst_reg[0] = c;
}

// Will turn into 2 addi's but still have the loadis
void muli_yes_caveat()
{
    vFloat a = 1.0f;
    vFloat b = 2.0f;

    vFloat c = a * b;
    dst_reg[0] = c;

    c = b * a;
    dst_reg[1] = c;
}

void add_plus_half_yes1()
{
#if 0
    vFloat a = dst_reg[0];
    vFloat b = dst_reg[1];
    vFloat c = dst_reg[2];

    dst_reg[0] = a + b + 0.5f;
    dst_reg[1] = a + b - 0.5f;
    dst_reg[2] = a * b + 0.5f;  // becomes a loadi+mad
    dst_reg[3] = a * b - 0.5f;
    dst_reg[4] = a * b + c + 0.5f;
    dst_reg[5] = a * b + c - 0.5f;

    // Have to reload a otherwise optimizer bails as addi trashes the src
    a = dst_reg[0];
    dst_reg[6] = a + 1.0f + 0.5f; // becomes a loadi and an add
    a = dst_reg[0];
    dst_reg[7] = a + 1.0f - 0.5f; // becomes a loadi and an add
    a = dst_reg[0];
    dst_reg[8] = a * 1.0f + 0.5f; // becomes 2 loadis and a mad
    a = dst_reg[0];
    dst_reg[9] = a * 1.0f - 0.5f; // becomes 2 loadis and a mad
#endif
}

void add_plus_half_yes2()
{
    vFloat a = dst_reg[0];
    vFloat b = dst_reg[1];
    vFloat c = dst_reg[2];

    vFloat half = 0.5f;
    dst_reg[1] = a * b + half + c;
}

void add_plus_half_yes3()
{
    vFloat a = dst_reg[0];
    vFloat b = dst_reg[1];
    vFloat c = dst_reg[2];

    // Be sure the half doesn't get deleted when it shouldn't
    vFloat half = 0.5f;
    dst_reg[1] = a * b + half + c;
    dst_reg[2] = half;
}

// Liveness not handled yet
// This is the case that is most important
void add_plus_half_live_yes1_future()
{
    vFloat a = dst_reg[0];
    vFloat b = dst_reg[1];

    v_if (a < 0.0f) {
        a = a + b + .5f;
    }
    v_endif;

    dst_reg[2] = a;
}

void add_plus_half_live_yes2_future()
{
    vFloat a = dst_reg[0];
    vFloat b = dst_reg[1];

    v_if (a < 0.0f) {
        a = a + b;
        a = a + 0.5f;
    }
    v_endif;

    dst_reg[2] = a;
}

void add_plus_half_live_yes3_future()
{
    vFloat a = dst_reg[0];
    vFloat b = dst_reg[1];
    vFloat c = dst_reg[2];

    v_if (a < 0.0f) {
        c = a + b;
        c = a + 0.5f;
    }
    v_endif;

    dst_reg[3] = a;
}

void add_plus_half_live_no1()
{
    vFloat a = dst_reg[0];
    vFloat b = dst_reg[1];

    a = a + b;
    v_if (a < 0.0f) {
        a = a + 0.5f;
    }
    v_endif;

    dst_reg[2] = a;
}

void add_plus_half_live_no2()
{
    vFloat a = dst_reg[0];
    vFloat b = dst_reg[1];

    v_if (a < 0.0f) {
        a = a + b;
    }
    v_endif;

    a = a + 0.5f;

    dst_reg[2] = a;
}

void add_plus_half_no1(unsigned int imm)
{
    vFloat a = dst_reg[0];
    vFloat b = dst_reg[1];
    vFloat c = dst_reg[2];

    dst_reg[0] = a * b + c + s2vFloat16b(imm);
}

void add_plus_half_no2()
{
    vFloat a = dst_reg[0];
    vFloat b = dst_reg[1];
    vFloat c = dst_reg[2];

    vFloat half = 0.5f;
    dst_reg[1] = a * b + half + c + half;
}

// This could be handled fairly easily
void add_plus_half_no_bb1(int count)
{
    vFloat a = dst_reg[0];
    vFloat b = dst_reg[1];
    vFloat half = 0.5f;

    vFloat c = a + b;

#pragma GCC unroll 0
    for (int i = 0; i < count; i++) {
        c = c + half;
    }
    dst_reg[0] = c;
}

// The compiler can select the other non-fp16b value
void bail_on_fp16b_1()
{
    vFloat a = 3.2f;
    a = a + 1.0f;
    dst_reg[0] = a;
}

void bail_on_fp16b_2()
{
    vFloat a = 1.0f;
    a = a + 3.2f;
    dst_reg[0] = a;
}

// Neither will work...
void bail_on_fp16b_3()
{
    vFloat a = 3.2f;
    a = a + 4.3;
    dst_reg[0] = a;
}

void complexish()
{
    vFloat a = dst_reg[0];
    vFloat b = dst_reg[1];
    vFloat c = 14.0f;

    dst_reg[4] = (((((a * b + 3.0 + 0.5f) * dst_reg[2] + dst_reg[3]) * 8.0f + 0.5f) + 10.0f) * 12.0f -0.5f) * c * 0.0f;

    a = dst_reg[5];
    b = dst_reg[6];
    dst_reg[9] = (((((a * b - 3.0 + 0.5f) * -dst_reg[7] - dst_reg[8]) * 8.0f - 0.5f) - 10.0f) * 12.0f -0.5f) * -c * 0.0f;
}
