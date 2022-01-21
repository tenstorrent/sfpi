#include <stdlib.h>

#include <cstdio>
#include "test.h"

// XXXX BUG
// operator= returns void for now to preclude using assignments in predicated
// conditionals as the compiler doesn't understand order of operations and so
// hoists the assignment outside of boolean expressions
// Behvaior precluded by the wrapper for now
#define ASSIGNMENTS_IN_COND 0

using namespace sfpi;

void iadd_i_yes1()
{
    VecShort a = 3;

    a = a - 5;
    p_if (a < 0) {
        dst_reg[0] = CReg_1;
    }
    p_endif;
}

void iadd_i_yes1_live()
{
    VecShort a = 3;

    p_if (a < 0) {
        a = a - 5;
        p_if (a < 0) {
            dst_reg[0] = CReg_1;
        }
        p_endif;
    }
    p_endif;
}

#if ASSIGNMENTS_IN_COND
void iadd_i_yes2()
{
    VecShort a = 3;
    VecShort b = 5;
    p_if ((a = a - 7) < 0) {
        dst_reg[0] = CReg_1;
    }
    p_endif;
}

void iadd_i_yes3a()
{
    VecShort a = 3;
    VecShort b = 4;

    p_if ((a = a - 5) < 0 && b == 6) {
        dst_reg[0] = CReg_1;
    }
    p_endif;
}

void iadd_i_yes3b()
{
    VecShort a = 3;
    VecShort b = 4;

    p_if (b == 6 && (a = a - 5) < 0) {
        dst_reg[0] = CReg_1;
    }
    p_endif;
}
#endif

void iadd_i_yes3c()
{
    VecShort a = 3;
    VecShort b = 4;

    // Wrapper handles this, compiler doesn't change anything
    p_if (b == 6 && a.add_cc(a, -5, IAddCCLT0)) {
        dst_reg[0] = CReg_1;
    }
    p_endif;
}

void iadd_i_yes3d()
{
    VecShort a = 3;

    // Wrapper handles this, compiler doesn't change anything
    a = a - 5;
    p_if (a.add_cc(a, 0, IAddCCLT0)) {
        dst_reg[0] = CReg_1;
    }
    p_endif;
}

void iadd_i_yes4a()
{
    VecShort a = 3;
    VecShort b = 4;

    // SetCC comes after iadds
    a = a - 5;
    p_if (a < 0 && b == 6) {
        dst_reg[0] = CReg_1;
    }
    p_endif;
}

void iadd_i_yes5()
{
    VecShort a = 3;

    // Testing 2 things (+ and >=)
    a = a + 5;
    p_if (a >= 0) {
        dst_reg[0] = CReg_1;
    }
    p_endif;
}

void iadd_i_yes6()
{
    VecShort a = 3;
    VecShort b = 4;

    // Make sure registers in new iadd are correct
    p_if (a >= 5) {
        a = b + 6;
        p_if (a >= 0) {
            dst_reg[0] = CReg_1;
        }
        p_endif;
    }
    p_endif;
}

#if ASSIGNMENTS_IN_COND
void iadd_i_yes7()
{
    VecShort a = 3;
    VecShort b = 4;

    p_if ((a = b + 6) >= 0) {
        dst_reg[0] = CReg_1;
    }
    p_endif;
}
#endif

void iadd_i_yes8()
{
    VecShort a = 3;
    VecShort b;

    // Make sure registers are correct, results carry through
    b = a + 5;
    p_if (b < 0) {
        dst_reg[0] = CReg_1;
    }
    p_endif;
    dst_reg[1] = a;
    dst_reg[2] = b;
}

void iadd_i_yes9()
{
    VecShort a = 3;
    VecShort b = 5;

    a = a - 7;
    p_if (a < 0) {
        p_if (b < 9) {
            dst_reg[0] = CReg_1;
        }
        p_endif;
    }
    p_endif;
}

void iadd_i_no1()
{
    VecShort a = 3;
    VecShort b = 4;

    // No, mismatched arguments
    b = b - 6;
    p_if (a < 0) {
        dst_reg[0] = CReg_1;
    }
    p_endif;
}

void iadd_i_no2()
{
    VecShort a = 3;
    VecShort b;

    // No, mismatched arguments
    b = a - 5;
    p_if (a < 0) {
        dst_reg[0] = CReg_1;
    }
    p_endif;
}

#if ASSIGNMENTS_IN_COND
void iadd_i_no3()
{
    VecShort a = 3;

    a = a - 5;
    // No then yes
    p_if ((a = a - 7) < 0) {
        dst_reg[0] = CReg_1;
    }
    p_endif;
}
#endif

void iadd_i_no4()
{
    VecShort a = 3;
    VecShort b = 4;

    // No, no CC
    a = a - 5;
    a = a - 7;
    a = b - 6;
    a = a - 9;
    dst_reg[0] = a;
}

void iadd_i_no4b()
{
    VecShort a = 3;
    VecShort b = 4;

    // Can't assign a after testing b == 6
    a = a - 5;
    p_if (b >= 6 && a < 0) {
        dst_reg[0] = CReg_1;
    }
    p_endif;
}

void iadd_i_no5()
{
    VecShort a = 3;
    VecShort b = 4;

    // No, mismatched arguments and both set CC
    p_if (a < 0 && b < 0) {
        dst_reg[0] = CReg_1;
    }
    p_endif;
}

void iadd_i_no6()
{
    VecShort a = 3;

    // No, 2nd doesn't set the CC
    a = a + 5;
    // Yeah, really
    a = a + 0;
}

void iadd_i_no7()
{
    VecShort a = 3;
    VecShort b;

    // No, intervening use
    a = a - 5;
    b = a + 7;
    p_if (a < 0) {
        dst_reg[0] = CReg_1;
    }
    p_endif;
}

void iadd_i_no8()
{
    VecShort a = 3;
    VecShort b = 5;

    // No, intervening CC
    a = a - 7;
    p_if (b < 9) {
        p_if (a < 0) {
            dst_reg[0] = CReg_1;
        }
        p_endif;
    }
    p_endif;
}

void iadd_i_no9()
{
    VecShort a = 3;

    // No, subsequent use of a
    a = a - 3;
    p_if (a.add_cc(a, 0, IAddCCLT0)) {
        dst_reg[0] = CReg_1;
    }
    p_endif;

    dst_reg[0] = a;
}

void iadd_i_no10()
{
    VecShort a = 3;

    int x = rand();

    // No, crosses BB
    a = a - 5;

    if (x == 0) {
        p_if (a < 0) {
            dst_reg[0] = CReg_1;
        }
        p_endif;
    } else if (x == 1) {
        p_if (a < 0) {
            dst_reg[0] = CReg_Neg_0p5;
        }
        p_endif;
    }
}

void iadd_i_no11()
{
    VecShort a = 3;

    // No, intervening COMPC
    a = a - 5;
    p_if (a < 0) {
        a = a + 2;
    } p_elseif(a < 0) {
        dst_reg[0] = CReg_1;
    }
    p_endif;
}

void iadd_i_no12(unsigned int imm)
{
    VecShort a = 3;

    // No, intervening COMPC
    a = a - 5;
    p_if (a < imm) {
        a = a + 2;
    }
    p_endif;
    dst_reg[0] = a;
}

void iadd_v_yes1()
{
    VecShort a = 3;
    VecShort b = 4;

    a = b - a;
    p_if (a < 0) {
        dst_reg[0] = CReg_1;
    }
    p_endif;
}

void iadd_v_yes2()
{
    VecShort a = 3;
    VecShort b = 4;

    a = a - b;
    p_if (a >= 0) {
        dst_reg[0] = CReg_1;
    }
    p_endif;
}

void iadd_v_yes3()
{
    VecShort a = 3;
    VecShort b = 4;

    a = a + b;
    p_if (a < 0) {
        dst_reg[0] = CReg_1;
    }
    p_endif;
}

void iadd_v_yes4a()
{
    VecShort a = 3;
    VecShort b = 4;

    // No move requried to save the dst
    p_if (a - b >= 0) {
        dst_reg[0] = CReg_1;
    }
    p_endif;
}

void iadd_v_yes4b()
{
    VecShort a = 3;
    VecShort b = 4;

    // Requires a move to save the dst
    p_if (a + b < 0) {
        dst_reg[0] = CReg_1;
    }
    p_endif;

    dst_reg[0] = b;
}

void iadd_v_no1()
{
    VecShort a = 3;
    VecShort b = 4;

    a = b + a;
    // No, compare against non-zero
    p_if (a < 5) {
        dst_reg[0] = CReg_1;
    }
    p_endif;
}

void iadd_v_no2()
{
    VecShort a = 3;
    VecShort b = 4;

    // No, iadd_v can't use imm
    p_if (a - b < 5) {
        dst_reg[0] = CReg_1;
    }
    p_endif;
}

void iadd_v_no3()
{
    VecShort a = 3;
    VecShort b = 4;

    // No, 2nd iadd doesn't set the CC
    a = a - b;
    a = a - 0;
}

void iadd_v_no4()
{
    VecShort a = 3;
    VecShort b = 4;

    // No, reversing the ops
    p_if (a - b < 0) {
        a = a - 5;
        dst_reg[0] = CReg_1;
    }
    p_endif;
}

void lz_yes1()
{
    VecHalf a = 3.0f;
    VecShort b;

    b = lz(a);
    p_if (a == 0) {
        dst_reg[0] = CReg_1;
    }
    p_endif;
    dst_reg[0] = a;
    dst_reg[0] = b;
}

void lz_yes2()
{
    VecHalf a = 3.0f;
    VecShort b;

    b = lz(a);
    p_if (a != 0) {
        dst_reg[0] = CReg_1;
    }
    p_endif;
    dst_reg[0] = a;
    dst_reg[0] = b;
}

void lz_no1()
{
    VecHalf a = 3.0f;
    VecShort b;

    b = lz(a);
    p_if (b != 0) {
        dst_reg[0] = CReg_1;
    }
    p_endif;
    dst_reg[0] = a;
    dst_reg[0] = b;
}

void lz_no2()
{
    VecHalf a = 3.0f;
    VecShort b;

    b = lz(a);
    p_if (a >= 0) {
        dst_reg[0] = CReg_1;
    }
    p_endif;
    dst_reg[0] = a;
    dst_reg[0] = b;
}

void lz_no3()
{
    VecHalf a = 3.0f;
    VecShort b;

    b = lz(a);
    p_if (a < 0) {
        dst_reg[0] = CReg_1;
    }
    p_endif;
    dst_reg[0] = a;
    dst_reg[0] = b;
}

void exexp_yes1()
{
    VecHalf a = 3.0f;
    VecShort b;

    b = exexp(a);
    p_if (b >= 0) {
        dst_reg[0] = CReg_1;
    }
    p_endif;
    dst_reg[0] = a;
    dst_reg[0] = b;
}

void exexp_yes1_live()
{
    VecHalf a = 3.0f;
    VecShort b = 1;

    p_if (a < 0) {
        b = exexp(a);
        p_if (b >= 0) {
            dst_reg[0] = CReg_1;
        }
        p_endif;
    }
    p_endif;
    dst_reg[0] = a;
    dst_reg[0] = b;
}

void exexp_yes2()
{
    VecHalf a = 3.0f;
    VecShort b;

    b = exexp(a);
    p_if (b < 0) {
        dst_reg[0] = CReg_1;
    }
    p_endif;
    dst_reg[0] = a;
    dst_reg[0] = b;
}

void exexp_yes3()
{
    VecHalf a = 3.0f;

    a = reinterpret<VecHalf>(exexp(a));
    p_if (reinterpret<VecShort>(a) < 0) {
        dst_reg[0] = CReg_1;
    }
    p_endif;
    dst_reg[0] = a;
}

void exexp_no1()
{
    VecHalf a = 3.0f;
    VecShort b;

    // No, compare not against 0
    b = exexp(a);
    p_if (b < 5) {
        dst_reg[0] = CReg_1;
    }
    p_endif;
    dst_reg[0] = a;
    dst_reg[0] = b;
}

void exexp_no2()
{
    VecHalf a = 3.0f;
    VecShort b;

    b = exexp_nodebias(a);
    // No, nodebias doesn't have a set CC to sign option
    p_if (b >= 0) {
        dst_reg[0] = CReg_1;
    }
    p_endif;
    dst_reg[0] = a;
    dst_reg[0] = b;
}

void mad_yes1()
{
    VecHalf a = dst_reg[0];
    VecHalf b = dst_reg[1];
    VecHalf c = dst_reg[2];
    VecHalf d;

    d = a * b + c;

    dst_reg[0] = d;
}

// This will result in out of registers if mad conversion fails
void mad_yes2()
{
    VecHalf a = dst_reg[0];
    VecHalf b = dst_reg[1];
    VecHalf c = dst_reg[2];
    VecHalf d, tmp;

    tmp = a * b;
    d = c + tmp;

    dst_reg[0] = d;

    tmp = a * b;
    d = tmp + c;

    dst_reg[0] = d;
}

void mad_yes3()
{
    VecHalf a = dst_reg[0];
    VecHalf b = dst_reg[1];
    VecHalf c = dst_reg[2];
    VecHalf d, tmp;

    d = a * (CReg_1 * (a * b + c) + CReg_1) + c;

    dst_reg[0] = d;
}

// This isn't handled due to concern about register pressure
// Reordering code would make it happen, so not a huge concern
void mad_maybe_future1()
{
    VecHalf a = dst_reg[0];
    VecHalf b = dst_reg[1];
    VecHalf c = dst_reg[2];
    VecHalf d = 4.0f;
    VecHalf tmp1, tmp2;

    tmp1 = a * b;
    tmp2 = c * d;

    dst_reg[0] = tmp1 + c;
    dst_reg[0] = tmp2 + c;
}

// This is not handled, but could be
void mad_maybe_future2()
{
    VecHalf a = dst_reg[0];
    VecHalf b = dst_reg[1];
    VecHalf tmp, d, e;
    
    tmp = a * b;
    d = tmp + dst_reg[2];
    e = tmp + dst_reg[3];

    dst_reg[0] = d;
    dst_reg[0] = e;
}

void mad_live_yes1()
{
    VecHalf a = dst_reg[0];
    VecHalf b = dst_reg[1];
    VecHalf c = dst_reg[2];
    VecHalf d = dst_reg[3];

    p_if (a == 0.0f) {
        VecHalf tmp = a * b;
        d = tmp + c;
    }
    p_endif;

    dst_reg[0] = d;
}

// This works on Grayskull w/o creating a mad, but will fail if the mad is
// created by moving the multiply down to the add
void mad_reg_pressure_no4()
{
    VecHalf a = dst_reg[0];
    VecHalf b = dst_reg[1];
    VecHalf c = dst_reg[2];
    VecHalf d;

    VecHalf tmp = a * b;
    VecShort x = exexp(a);
    VecShort y = exexp(b);
    d = tmp + c;

    dst_reg[0] = c;
    dst_reg[0] = d;
    dst_reg[1] = x;
    dst_reg[1] = y;
}

void mad_no1()
{
    VecHalf a = dst_reg[0];
    VecHalf b = dst_reg[1];
    VecHalf c = dst_reg[2];
    VecHalf d, tmp;

    // Subsequent use
    tmp = a * b;
    d = c + tmp;

    dst_reg[0] = tmp;
    dst_reg[0] = d;
}

void mad_no2()
{
    VecHalf a = dst_reg[0];
    VecHalf b = dst_reg[1];
    VecHalf c = dst_reg[2];
    VecHalf d, tmp;

    // Prior use
    tmp = a * b;
    dst_reg[0] = tmp;
    d = c + tmp;

    dst_reg[0] = d;
}

void mad_no3()
{
    VecHalf a = dst_reg[0];
    VecHalf b = dst_reg[1];
    VecHalf c = dst_reg[2];
    VecHalf d, tmp;

    // Intervening CC
    tmp = a * b;
    p_if (a >= 0.0F) {
    }
    p_endif;

    d = c + tmp;

    dst_reg[0] = d;
}

// Since mads increase register pressure, not going to let the compiler turn
// this into a mad, require instructions to be "closer" together
void mad_live_no1()
{
    VecHalf a = dst_reg[0];
    VecHalf b = dst_reg[1];
    VecHalf c = dst_reg[2];
    VecHalf d = dst_reg[3];
    VecHalf e = a * b;

    // This can be a yes because e is not used later
    // and d is at a higher CC level than e
    p_if (a == 0.0f) {
        d = e + c;
    }
    p_endif;

    dst_reg[0] = d;
}

void mad_live_no2()
{
    VecHalf a = dst_reg[0];
    VecHalf b = dst_reg[1];
    VecHalf c = dst_reg[2];
    VecHalf d = dst_reg[3];
    VecHalf e = a * b;

    p_if (a == 0.0f) {
        d = e + c;
    }
    p_endif;

    dst_reg[0] = d;
    dst_reg[0] = e;
}

void addi_yes1()
{
    VecHalf a = 1.0f;
    VecHalf b = 2.0f;

    VecHalf c = a + b;
    dst_reg[0] = c;
}

// Will turn into 2 addi's but still have the loadis
void addi_yes_caveat()
{
    VecHalf a = 1.0f;
    VecHalf b = 2.0f;

    VecHalf c = a + b;
    dst_reg[0] = c;

    c = b + a;
    dst_reg[1] = c;
}

// Will ditch one of the loadis
void addi_yes2()
{
    VecHalf a = 1.0f;
    VecHalf b = 2.0f;

    VecHalf c = a + b;
    VecHalf d = c + b;
    VecHalf e = d + b;
    dst_reg[0] = e;
}

void addi_yes3()
{
    VecHalf a = dst_reg[0];
    VecHalf b = dst_reg[1];

    VecHalf c = a * b + 1.0f + 2.0f;  // Mad + Addi
    dst_reg[0] = c;
}

// Pick the right one based on subsequent use
void addi_yes4()
{
    {
        VecHalf a = 1.0f;
        VecHalf b = 2.0f;

        VecHalf c = a + b;
        dst_reg[0] = c;
        dst_reg[0] = a;
    }
    {
        VecHalf a = 1.0f;
        VecHalf b = 2.0f;

        VecHalf c = a + b;
        dst_reg[0] = c;
        dst_reg[0] = b;
    }
}

void addi_yes_imm1(unsigned int imm)
{
    VecHalf a = 1.0f;
    VecHalf b = ScalarFP16b(imm);

    VecHalf c = a + b;
    dst_reg[0] = c;
}

void addi_yes_imm2(unsigned int imm)
{
    VecHalf a = ScalarFP16b(imm);
    VecHalf b = 1.0f;

    VecHalf c = a + b;
    dst_reg[0] = c;
}

void addi_live_yes1()
{
    VecHalf b = 2.0f;

    p_if (dst_reg[0] == 0.0f) {
        VecHalf a = 3.0f;
        b = a + b;
    }
    p_endif;

    dst_reg[0] = b;
}

void addi_live_yes2()
{
    VecHalf b = 2.0f;

    p_if (dst_reg[0] == 0.0f) {
        VecHalf a = 3.0f;
        b = b + a;
    }
    p_endif;

    dst_reg[0] = b;
}

void addi_live_yes3_future()
{
    VecHalf b = 2.0f;
    VecHalf a = 3.0f;

    // Could handle this, but opens up more potential for bugs
    p_if (dst_reg[0] == 0.0f) {
        b = b + a;
    }
    p_endif;

    dst_reg[0] = b;
}

void addi_live_yes4_future()
{
    VecHalf a = 3.0f;
    VecHalf b = 2.0f;

    p_if (dst_reg[0] == 0.0f) {
        // This is a loadi_lv
        // Could handle this, but opens up more potential for bugs
        a = 4.0f;
        b = b + a;
    }
    p_endif;

    dst_reg[0] = b;
}

void addi_live_no2()
{
    VecHalf b = 2.0f;
    VecHalf c = 1.0f;

    p_if (dst_reg[0] == 0.0f) {
        // Requires a move of addi result into C, non-optimal
        VecHalf a = 3.0f;
        c = a + b;
    }
    p_endif;

    dst_reg[0] = c;
}

void addi_live_no3()
{
    VecHalf a = 1.0f;
    VecHalf b = 2.0f;
    VecHalf c = 1.0f;

    p_if (dst_reg[0] == 0.0f) {
        // Requires a move of addi result into C, non-optimal
        a = 3.0f;
        c = a + b;
    }
    p_endif;

    dst_reg[0] = c;
}

void addi_live_no4()
{
    VecHalf a = 1.0f;
    VecHalf b = dst_reg[0];
    VecHalf c = 1.0f;

    p_if (dst_reg[0] == 0.0f) {
        // No.  Could do an addi since a is not reused,
        // but not particularly interesting
        a = 3.0f;
        c = a + b;
    }
    p_endif;

    dst_reg[0] = c;
}

void addi_no1()
{
    VecHalf a = 1.0f;
    VecHalf b = dst_reg[0];

    // a not a pure loadi
    p_if (dst_reg[0] == 0.0f) {
        a = 3.0f;
    }
    p_endif;

    VecHalf c = a + b;

    dst_reg[0] = c;
}

// Subsequent use makes this perf-undesirable
void addi_no2()
{
    VecHalf a = 1.0f;
    VecHalf b = 2.0f;

    VecHalf c = a + b;
    dst_reg[0] = a;
    dst_reg[0] = b;
    dst_reg[0] = c;
}

// Muli and addi are handled w/ the same code, don't need to heavily
// test muli
void muli_yes1()
{
    VecHalf a = 1.0f;
    VecHalf b = 2.0f;

    VecHalf c = a * b;
    dst_reg[0] = c;
}

// Will turn into 2 addi's but still have the loadis
void muli_yes_caveat()
{
    VecHalf a = 1.0f;
    VecHalf b = 2.0f;

    VecHalf c = a * b;
    dst_reg[0] = c;

    c = b * a;
    dst_reg[1] = c;
}

void add_plus_half_yes1()
{
    VecHalf a = dst_reg[0];
    VecHalf b = dst_reg[1];
    VecHalf c = dst_reg[2];

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
}

void add_plus_half_yes2()
{
    VecHalf a = dst_reg[0];
    VecHalf b = dst_reg[1];
    VecHalf c = dst_reg[2];

    VecHalf half = 0.5f;
    dst_reg[1] = a * b + half + c;
}

void add_plus_half_yes3()
{
    VecHalf a = dst_reg[0];
    VecHalf b = dst_reg[1];
    VecHalf c = dst_reg[2];

    // Be sure the half doesn't get deleted when it shouldn't
    VecHalf half = 0.5f;
    dst_reg[1] = a * b + half + c;
    dst_reg[2] = half;
}

// Liveness not handled yet
// This is the case that is most important
void add_plus_half_live_yes1_future()
{
    VecHalf a = dst_reg[0];
    VecHalf b = dst_reg[1];

    p_if (a < 0.0f) {
        a = a + b + .5f;
    }
    p_endif;

    dst_reg[2] = a;
}

void add_plus_half_live_yes2_future()
{
    VecHalf a = dst_reg[0];
    VecHalf b = dst_reg[1];

    p_if (a < 0.0f) {
        a = a + b;
        a = a + 0.5f;
    }
    p_endif;

    dst_reg[2] = a;
}

void add_plus_half_live_yes3_future()
{
    VecHalf a = dst_reg[0];
    VecHalf b = dst_reg[1];
    VecHalf c = dst_reg[2];

    p_if (a < 0.0f) {
        c = a + b;
        c = a + 0.5f;
    }
    p_endif;

    dst_reg[3] = a;
}

void add_plus_half_live_no1()
{
    VecHalf a = dst_reg[0];
    VecHalf b = dst_reg[1];

    a = a + b;
    p_if (a < 0.0f) {
        a = a + 0.5f;
    }
    p_endif;

    dst_reg[2] = a;
}

void add_plus_half_live_no2()
{
    VecHalf a = dst_reg[0];
    VecHalf b = dst_reg[1];

    p_if (a < 0.0f) {
        a = a + b;
    }
    p_endif;

    a = a + 0.5f;

    dst_reg[2] = a;
}

void add_plus_half_no1(unsigned int imm)
{
    VecHalf a = dst_reg[0];
    VecHalf b = dst_reg[1];
    VecHalf c = dst_reg[2];

    dst_reg[0] = a * b + c + ScalarFP16b(imm);
}

void add_plus_half_no2()
{
    VecHalf a = dst_reg[0];
    VecHalf b = dst_reg[1];
    VecHalf c = dst_reg[2];

    VecHalf half = 0.5f;
    dst_reg[1] = a * b + half + c + half;
}

void complexish()
{
    VecHalf a = dst_reg[0];
    VecHalf b = dst_reg[1];
    VecHalf c = 14.0f;

    dst_reg[4] = (((((a * b + 3.0 + 0.5f) * dst_reg[2] + dst_reg[3]) * 8.0f + 0.5f) + 10.0f) * 12.0f -0.5f) * c * 0.0f;

    a = dst_reg[5];
    b = dst_reg[6];
    dst_reg[9] = (((((a * b - 3.0 + 0.5f) * -dst_reg[7] - dst_reg[8]) * 8.0f - 0.5f) - 10.0f) * 12.0f -0.5f) * -c * 0.0f;
}
