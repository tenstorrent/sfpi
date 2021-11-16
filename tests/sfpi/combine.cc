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
        dregs[0] = CReg_1;
    }
    p_endif;
}

#if ASSIGNMENTS_IN_COND
void iadd_i_yes2()
{
    VecShort a = 3;
    VecShort b ((a = a - 5));
    p_if ((a = a - 7) < 0) {
        dregs[0] = CReg_1;
    }
    p_endif;
}

void iadd_i_yes3a()
{
    VecShort a = 3;
    VecShort b = 4;

    p_if ((a = a - 5) < 0 && b == 6) {
        dregs[0] = CReg_1;
    }
    p_endif;
}

void iadd_i_yes3b()
{
    VecShort a = 3;
    VecShort b = 4;

    p_if (b == 6 && (a = a - 5) < 0) {
        dregs[0] = CReg_1;
    }
    p_endif;
}
#endif

void iadd_i_yes3c()
{
    VecShort a = 3;
    VecShort b = 4;

    p_if (b == 6 && a.add_cc(a, -5, IAddCCLT0)) {
        dregs[0] = CReg_1;
    }
    p_endif;
}

void iadd_i_yes4a()
{
    VecShort a = 3;
    VecShort b = 4;

    a = a - 5;
    p_if (a < 0 && b == 6) {
        dregs[0] = CReg_1;
    }
    p_endif;
}

void iadd_i_yes4b()
{
    VecShort a = 3;
    VecShort b = 4;

    // Eventually.  Can't use peephole
    a = a - 5;
    p_if (b == 6 && a < 0) {
        dregs[0] = CReg_1;
    }
    p_endif;
}

void iadd_i_yes5()
{
    VecShort a = 3;

    // Testing 2 things...
    a = a + 5;
    p_if (a >= 0) {
        dregs[0] = CReg_1;
    }
    p_endif;
}

void iadd_i_yes6()
{
    VecShort a = 3;
    VecShort b = 4;

    a = b + 6;
    p_if (a >= 0) {
        dregs[0] = CReg_1;
    }
    p_endif;
}

#if ASSIGNMENTS_IN_COND
void iadd_i_yes7()
{
    VecShort a = 3;
    VecShort b = 4;

    p_if ((a = b + 6) >= 0) {
        dregs[0] = CReg_1;
    }
    p_endif;
}
#endif

void iadd_i_yes8()
{
    VecShort a = 3;
    VecShort b;

    // Not live
    b = a + 5;
    p_if (b < 0) {
        dregs[0] = CReg_1;
    }
    p_endif;
    dregs[0] = a;
    dregs[0] = b;
}

void iadd_i_no1()
{
    VecShort a = 3;
    VecShort b = 4;

    b = b - 6;
    p_if (a < 0) {
        dregs[0] = CReg_1;
    }
    p_endif;
}

void iadd_i_no2()
{
    VecShort a = 3;
    VecShort b;

    b = a - 5;
    p_if (a < 0) {
        dregs[0] = CReg_1;
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
        dregs[0] = CReg_1;
    }
    p_endif;
}
#endif

void iadd_i_no4()
{
    VecShort a = 3;
    VecShort b = 4;

    a = a - 5;
    a = a - 7;
    a = b - 6;
    a = a - 9;
    dregs[0] = a;
}

void iadd_i_no5()
{
    VecShort a = 3;
    VecShort b = 4;

    p_if (a < 0 && b < 0) {
        dregs[0] = CReg_1;
    }
    p_endif;
}

void iadd_i_no6()
{
    VecShort a = 3;

    a = a + 5;
    // Yeah, really
    a = a + 0;
}

void iadd_v_yes1()
{
    VecShort a = 3;
    VecShort b = 4;

    a = b - a;
    p_if (a < 0) {
        dregs[0] = CReg_1;
    }
    p_endif;
}

void iadd_v_yes2()
{
    VecShort a = 3;
    VecShort b = 4;

    a = a - b;
    p_if (a >= 0) {
        dregs[0] = CReg_1;
    }
    p_endif;
}

void iadd_v_yes3()
{
    VecShort a = 3;
    VecShort b = 4;

    a = a + b;
    p_if (a < 0) {
        dregs[0] = CReg_1;
    }
    p_endif;
}

void iadd_v_yes4a()
{
    VecShort a = 3;
    VecShort b = 4;

    // No move requried to save the dst
    p_if (a - b >= 0) {
        dregs[0] = CReg_1;
    }
    p_endif;
}

void iadd_v_yes4b()
{
    VecShort a = 3;
    VecShort b = 4;

    // Requires a move to save the dst
    p_if (a + b < 0) {
        dregs[0] = CReg_1;
    }
    p_endif;
}

void iadd_v_no1()
{
    VecShort a = 3;
    VecShort b = 4;

    a = b + a;
    p_if (a < 5) {
        dregs[0] = CReg_1;
    }
    p_endif;
}

void iadd_v_no2()
{
    VecShort a = 3;
    VecShort b = 4;

    p_if (a - b < 5) {
        dregs[0] = CReg_1;
    }
    p_endif;
}

void iadd_v_no3()
{
    VecShort a = 3;
    VecShort b = 4;

    a = a - b;
    a = a - 0;
}

void iadd_v_no4()
{
    VecShort a = 3;
    VecShort b = 4;

    p_if (a - b < 0) {
        a = a - 5;
        dregs[0] = CReg_1;
    }
    p_endif;
}

void lz_yes1()
{
    VecHalf a = 3.0f;
    VecShort b;

    b = lz(a);
    p_if (a == 0) {
        dregs[0] = CReg_1;
    }
    p_endif;
    dregs[0] = a;
    dregs[0] = b;
}

void lz_yes2()
{
    VecHalf a = 3.0f;
    VecShort b;

    b = lz(a);
    p_if (a != 0) {
        dregs[0] = CReg_1;
    }
    p_endif;
    dregs[0] = a;
    dregs[0] = b;
}

void lz_no1()
{
    VecHalf a = 3.0f;
    VecShort b;

    b = lz(a);
    p_if (b != 0) {
        dregs[0] = CReg_1;
    }
    p_endif;
    dregs[0] = a;
    dregs[0] = b;
}

void lz_no2()
{
    VecHalf a = 3.0f;
    VecShort b;

    b = lz(a);
    p_if (a >= 0) {
        dregs[0] = CReg_1;
    }
    p_endif;
    dregs[0] = a;
    dregs[0] = b;
}

void lz_no3()
{
    VecHalf a = 3.0f;
    VecShort b;

    b = lz(a);
    p_if (a < 0) {
        dregs[0] = CReg_1;
    }
    p_endif;
    dregs[0] = a;
    dregs[0] = b;
}

void exexp_yes1()
{
    VecHalf a = 3.0f;
    VecShort b;

    b = exexp(a);
    p_if (b >= 0) {
        dregs[0] = CReg_1;
    }
    p_endif;
    dregs[0] = a;
    dregs[0] = b;
}

void exexp_yes2()
{
    VecHalf a = 3.0f;
    VecShort b;

    b = exexp(a);
    p_if (b < 0) {
        dregs[0] = CReg_1;
    }
    p_endif;
    dregs[0] = a;
    dregs[0] = b;
}

void exexp_yes3()
{
    VecHalf a = 3.0f;

    a = reinterpret<VecHalf>(exexp(a));
    p_if (reinterpret<VecShort>(a) < 0) {
        dregs[0] = CReg_1;
    }
    p_endif;
    dregs[0] = a;
}

void exexp_no1()
{
    VecHalf a = 3.0f;
    VecShort b;

    b = exexp(a);
    p_if (b < 5) {
        dregs[0] = CReg_1;
    }
    p_endif;
    dregs[0] = a;
    dregs[0] = b;
}

void exexp_no2()
{
    VecHalf a = 3.0f;
    VecShort b;

    b = exexp_nodebias(a);
    p_if (b >= 0) {
        dregs[0] = CReg_1;
    }
    p_endif;
    dregs[0] = a;
    dregs[0] = b;
}
