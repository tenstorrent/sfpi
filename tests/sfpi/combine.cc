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
    a = b + 6;
    p_if (a >= 0) {
        dst_reg[0] = CReg_1;
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
