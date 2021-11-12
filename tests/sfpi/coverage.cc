#include <stdio.h>
#include <stdlib.h>
#include "test.h"

using namespace sfpi;


// test_load_store
//
// Generates the following:
//      SFPLOAD L0, 1, 0
//      SFPLOAD L1, 1, 1
//      SFPLOAD L2, 1, 2
//      SFPSTORE        L0, 0, 3
//      SFPSTORE        L1, 0, 4
//      SFPSTORE        L2, 0, 5
//      SFPLOAD L0, 1, 7
//      SFPSTORE        L0, 0, 6
//      SFPSTORE        L1, 0, 8
void test_load_store()
{
    VecHalf a, b;

    a = dregs[0];
    b = dregs[1];
    VecHalf c = dregs[2];

    dregs[3] = a;

    dregs[4] = b;
    dregs[5] = c;

    dregs[6] = dregs[7];

    a = b;
    dregs[8] = a;

    //    dregs[9] = CReg_Neg_1;
}

// test_add
//
// Generates the following:
//      SFPLOAD L0, 1, 1
//      SFPLOAD L1, 1, 2
//      SFPMAD  L0, L10, L1, L0, 0
//      SFPSTORE        L0, 0, 0
//      SFPLOAD L0, 1, 4
//      SFPLOAD L1, 1, 5
//      SFPMAD  L0, L10, L1, L0, 1
//      SFPSTORE        L0, 0, 3
//      SFPLOAD L0, 1, 7
//      SFPLOAD L1, 1, 8
//      SFPMAD  L0, L10, L1, L0, 3
//      SFPSTORE        L0, 0, 6
//      SFPLOAD L0, 1, 9
//      SFPLOAD L1, 1, 10
//      SFPMAD  L0, L10, L1, L2, 3
//      SFPSTORE        L2, 0, 11
//      SFPMAD  L0, L10, L1, L0, 1
//      SFPSTORE        L0, 0, 12
//      SFPMAD  L4, L10, L11, L1, 0
//      SFPSTORE        L1, 0, 13
//      SFPLOAD L1, 1, 12
//      SFPMAD  L11, L10, L1, L1, 0
//      SFPSTORE        L1, 0, 14
//      SFPLOAD L1, 1, 12
//      SFPMAD  L1, L10, L12, L1, 1
//      SFPSTORE        L1, 0, 15
//      SFPMAD  L0, L10, L13, L0, 3
//      SFPSTORE        L0, 0, 15
void test_add()
{
    dregs[0] = dregs[1] + dregs[2];
    dregs[3] = dregs[4] + dregs[5] + kHalf;
    dregs[6] = dregs[7] + dregs[8] - kHalf;

    VecHalf a = dregs[9];
    VecHalf b = dregs[10];
    VecHalf c = a + b - kHalf;

    a += b;
    a += dregs[0];

    dregs[11] = c;
    dregs[12] = a + b + kHalf;

    dregs[13] = CReg_0 + CReg_Neg_1;
    dregs[14] = CReg_Neg_1 + dregs[12];
    dregs[15] = dregs[12] + CReg_0p001953125 + kHalf;
    dregs[16] = c + CReg_Neg_0p67480469 - kHalf;

    //TTFIXME XXXX 
    //    dregs[17] = a + b + c;
}

void test_sub()
{
    dregs[0] = dregs[1] - dregs[2];
    dregs[3] = dregs[4] - dregs[5] + kHalf;

    VecHalf a = dregs[6];
    VecHalf b = dregs[7];

    a -= b;
    a -= dregs[0];

    dregs[8] = -b;
    dregs[9] = a - b - kHalf;

    dregs[10] = CReg_Neg_1 - b;

    dregs[11] = CReg_Neg_1 * a - b;
    dregs[14] = -dregs[12] * -a - dregs[13];
}

// test_mul
//
// Generates the following:
//      SFPLOAD L0, 1, 1
//      SFPLOAD L1, 1, 2
//      SFPMAD  L0, L1, L4, L0, 0
//      SFPSTORE        L0, 0, 0
//      SFPLOAD L0, 1, 4
//      SFPLOAD L1, 1, 5
//      SFPMAD  L0, L1, L4, L0, 1
//      SFPSTORE        L0, 0, 3
//      SFPLOAD L0, 1, 7
//      SFPLOAD L1, 1, 8
//      SFPMAD  L0, L1, L4, L0, 3
//      SFPSTORE        L0, 0, 6
//      SFPLOAD L0, 1, 9
//      SFPLOAD L1, 1, 10
//      SFPMAD  L0, L1, L4, L2, 3
//      SFPSTORE        L2, 0, 11
//      SFPMAD  L0, L1, L4, L0, 1
//      SFPSTORE        L0, 0, 12
//      SFPMAD  L4, L11, L4, L1, 0
//      SFPSTORE        L1, 0, 13
//      SFPLOAD L1, 1, 12
//      SFPMAD  L11, L1, L4, L1, 0
//      SFPSTORE        L1, 0, 14
//      SFPLOAD L1, 1, 12
//      SFPMAD  L1, L12, L4, L1, 1
//      SFPSTORE        L1, 0, 15
//      SFPMAD  L0, L13, L4, L0, 3
//      SFPSTORE        L0, 0, 15
void test_mul()
{
    dregs[0] = dregs[1] * dregs[2];
    dregs[3] = dregs[4] * dregs[5] + kHalf;
    dregs[6] = dregs[7] * dregs[8] - kHalf;

    VecHalf a = dregs[9];
    VecHalf b = dregs[10];
    VecHalf c = a * b - kHalf;

    a *= b;
    a *= dregs[0];

    dregs[11] = c;
    dregs[12] = a * b;
    dregs[12] = a * b + kHalf;

    dregs[13] = CReg_0 * CReg_Neg_1;
    dregs[14] = CReg_Neg_1 * dregs[12];
    dregs[15] = dregs[12] * CReg_0p001953125 + kHalf;
    dregs[15] = c * CReg_Neg_0p67480469 - kHalf;

    dregs[16] = a * b * c;
    dregs[17] = CReg_0 * b * c;
}

// test_mad
//
// Generates the following:
//      SFPLOAD L0, 1, 1
//      SFPLOAD L1, 1, 2
//      SFPLOAD L2, 1, 3
//      SFPMAD  L0, L1, L2, L0, 0
//      SFPSTORE        L0, 0, 0
//      SFPLOAD L0, 1, 5
//      SFPLOAD L1, 1, 6
//      SFPLOAD L2, 1, 7
//      SFPMAD  L0, L1, L2, L0, 1
//      SFPSTORE        L0, 0, 4
//      SFPLOAD L0, 1, 9
//      SFPLOAD L1, 1, 10
//      SFPLOAD L2, 1, 11
//      SFPMAD  L0, L1, L2, L0, 3
//      SFPSTORE        L0, 0, 8
//      SFPLOAD L0, 1, 12
//      SFPLOAD L1, 1, 13
//      SFPLOAD L2, 1, 14
//      SFPMAD  L0, L1, L2, L3, 3
//      SFPSTORE        L3, 0, 15
//      SFPMAD  L0, L1, L2, L0, 1
//      SFPSTORE        L0, 0, 16
//      SFPMAD  L0, L1, L11, L0, 3
//      SFPSTORE        L0, 0, 17
//      SFPMAD  L4, L1, L11, L0, 1
//      SFPSTORE        L0, 0, 18
void test_mad()
{
    dregs[0] = dregs[1] * dregs[2] + dregs[3];
    dregs[4] = dregs[5] * dregs[6] + dregs[7] + kHalf;
    dregs[8] = dregs[9] * dregs[10] + dregs[11] - kHalf;

    VecHalf a = dregs[12];
    VecHalf b = dregs[13];
    VecHalf c = dregs[14];
    VecHalf d = a * b + c - kHalf;
    dregs[15] = d;
    dregs[16] = a * b + c + kHalf;

    dregs[17] = a * b + CReg_Neg_1 - kHalf;
    dregs[18] = CReg_0 * b + CReg_Neg_1 + kHalf;
}

// test_permute_ops
//
void test_permute_ops()
{
    VecHalf b = dregs[1];
    VecHalf c = dregs[2];

    dregs[3] = dregs[4] * dregs[5] + c;
    dregs[6] = dregs[7] * b + dregs[8];
    dregs[9] = dregs[10] * b + c;

    // Use reloads below to address register pressure
    VecHalf a = dregs[0];
    dregs[11] = a * dregs[12] + dregs[13];

    b = dregs[1];
    c = dregs[2];

    dregs[14] = a * dregs[15] + c;
    dregs[19] = a * b + c;
}

void test_loadi(int32_t i, uint32_t ui)
{
    VecShort a;
    a.loadi(-255);
    a = 255;
    a = -255;
    dregs[0] = a;

    VecUShort b;
    b = 255;
    b = -255;
    b.loadi(255U);
    dregs[1] = b;

    VecHalf c;
    c = ScalarFP16a(1.0F);
    c = ScalarFP16a(0x3F80);
    c = ScalarFP16a(0x3F80U);
    c = ScalarFP16b(1.0F);
    c = ScalarFP16b(0x3F80);
    c = ScalarFP16b(0x3F80U);
    c = ScalarFP16b(i);
    c = ScalarFP16b(ui);

    VecHalf f;
    f = 3.0f;
    dregs[5] = f;

    VecHalf g = 3.1f;
    dregs[6] = g;
}

void test_control_flow(int count)
{
    VecHalf v;

    v = 1.5F;
    for (int i = 0; i < count; i++) {
        if (i & 1) {
            v = dregs[1];
        } else {
            dregs[2] = v;
        }
    }
}

void test_mad_imm()
{
    VecHalf x = dregs[0];
    VecHalf a = 0.34F;
    VecHalf c = x * (x * a + -0.3F) + .5F;
    dregs[2] = c;
}

void test_man_exp()
{
    VecHalf v1;

    v1 = dregs[0];
    VecShort v2 = v1.ex_man(ExManPad9);
    VecShort v3 = v1.ex_exp(ExExpNoDebias);
    dregs[3] = v3;

    CCCtrl cco(true);
    VecShort v4;
    cco.cc_if(v4.ex_exp_cc(v1, ExExpDoDebias, ExExpCCLT0)); {
    }

    //    v1.set_man(v2);
    v1.set_exp(v2);

    VecHalf v5 = dregs[1];
    v1.add_exp(v5, 20);
    v5 = v1.set_exp(10);
    v5 = v1.set_man(10);

    dregs[2] = v2;
    dregs[4] = v4;
}

void test_dreg_conditional()
{
    VecHalf v = dregs[0];

    p_if(v < 5.0F) {
        p_if(v >= 2.0F) {
            dregs[2] = v;
        } p_elseif(v != 4) {
            dregs[3] = v;
        }
        p_endif;

        dregs[4] = v;
    } p_elseif(v >= 6.0F) {
        p_if(v == 0.0F); {
            dregs[5] = v;
        }
        p_endif;
    } p_elseif(!(v >= 6.0F)) {
        dregs[6] = v;
    } p_else {
        dregs[7] = v;
    }
    p_endif;

    dregs[8] = v;
}

void test_vhalf_conditional()
{
    VecHalf v = dregs[0];
    VecHalf x = 5.0F;

    p_if(v < x) {
        p_if(v >= x) {
            dregs[2] = v;
        } p_elseif(v != x) {
            dregs[3] = v;
        }
        p_endif;

        dregs[4] = v;
    } p_elseif(v >= x) {
        p_if(v == x); {
            dregs[5] = v;
        }
        p_endif;
    } p_elseif(!(v >= x)) {
        dregs[6] = v;
    } p_else {
        dregs[7] = v;
    }
    p_endif;

    dregs[8] = v;
}

void test_creg_conditional1()
{
    VecHalf v = dregs[0];

    p_if(v < CReg_Neg_1) {
        p_if(v >= CReg_Neg_1) {
            dregs[2] = v;
        } p_elseif(v != CReg_Neg_1) {
            dregs[3] = v;
        }
        p_endif;

        dregs[4] = v;
    } p_elseif(v >= CReg_Neg_1) {
        p_if(v == CReg_Neg_1); {
            dregs[5] = v;
        }
        p_endif;
    } p_elseif(!(v >= CReg_Neg_1)) {
        dregs[6] = v;
    } p_else {
        dregs[7] = v;
    }
    p_endif;

    dregs[8] = v;
}

void test_creg_conditional2()
{
    VecHalf v = dregs[0];

    p_if(CReg_Neg_1 < v) {
        p_if(CReg_Neg_1 >= v) {
            dregs[2] = v;
        } p_elseif(CReg_Neg_1 != v) {
            dregs[3] = v;
        }
        p_endif;

        dregs[4] = v;
    } p_elseif(CReg_Neg_1 >= v) {
        p_if(CReg_Neg_1 == v); {
            dregs[5] = v;
        }
        p_endif;
    } p_elseif(!(CReg_Neg_1 >= v)) {
        dregs[6] = v;
    } p_else {
        dregs[7] = v;
    }
    p_endif;

    dregs[8] = v;
}

void test_bitwise()
{
    VecShort v1, v2;

    v1 = 1;
    v2 = 2;

    v1 |= v2;
    v1 |= 0xAA;
    dregs[2] = v1;
    v1 &= v2;
    v1 &= 0xAA;
    dregs[3] = v2;
    dregs[4] = ~v1;

    dregs[5] = v1 | v2;
    dregs[6] = v1 & v2;

    VecUShort v3, v4;
    v3 = 3;
    v4 = 4;
    v3 |= v4;
    v3 |= 0xAA;
    dregs[2] = v3;
    v3 &= v4;
    v3 &= 0xAA;
    dregs[3] = v4;
    dregs[4] = ~v3;

    dregs[5] = v3 | v4;
    dregs[6] = v3 & v4;
}

void test_abs()
{
    VecShort v1 = -5;
    VecShort v2 = v1.abs();

    v2 = v2.abs() + 1;

    dregs[0] = v1;
    dregs[1] = v2;

    VecHalf v3 = -6.0f;
    VecHalf v4 = v3.abs();
    v4 = v4.abs() + 1.0f;

    dregs[2] = v3;
    dregs[3] = v4;
}

void test_lz()
{
    VecShort v1;
    VecUShort v2;
    VecHalf v3;

    v3 = 10.0f;
    v1 = v3.lz() + 1;
    v2 = v3.lz() + 1;
    v1 = v1.lz() + 1;
    v2 = v2.lz() + 1;
    dregs[0] = v1;
    dregs[1] = v2;
    dregs[2] = v3;
}

void test_shft()
{
    VecShort v1 = 1;
    VecUShort v2 = 2;
    VecUShort v3;

    v3 = v2 >> 3;
    v3 = v2 << 4;
    v1 = v1 << 5;
    v3 <<= 6;
    v3 >>= 7;
    v2 = v3.shft(v1);

    dregs[0] = v1;
    dregs[1] = v2;
    dregs[2] = v3;
}

// Had issues w/ implict copy/move, test some of that here
void test_complex()
{
    VecHalf a = dregs[0];
    VecHalf b = dregs[1];
    VecHalf c = dregs[2];
    c = dregs[2];
    c = a + b;
    dregs[2] = a + b;
    dregs[2] = a + b;

    // Do we want to support this? Presently operator= is defined to return void
    // dregs[2] = (c = a + b);
}

void test_muli_addi()
{
    VecHalf a = dregs[0];

    a *= 16;
    a += 12;

    dregs[1] = a;

    VecHalf b, c;

    b = a * 16;
    c = a + 12;

    dregs[2] = b;
    dregs[3] = c;

    // Do we want to support this? Presently operator= is defined to return void
    // dregs[2] = (c = a + b);
}

void test_iadd()
{
    VecShort v1, v2;

    v1 = 12;
    v2 = v1 + 10;
    v2 += v1;
    v2 += 10;
    VecShort v3 = v1 + v2;
    v2 -= v3;
    v2 -= 10;
    v3 = v1 - v2;
    v3 += CReg_TileId;
    v2 = v1 + CReg_TileId;
    //    v2 = v1 - CReg_TileId;
    dregs[1] = v1;
    dregs[2] = v2;
    dregs[3] = v3;

    p_if (v1.add_cc(CReg_TileId, IAddCCGTE0)) {
        dregs[4] = 0.0F;
    }
    p_endif;

    VecUShort v4, v5;

    v4 = 12;
    v5 = v4 + 10;
    v5 += v4;
    v5 += 10;
    VecUShort v6 = v4 + v5;
    v5 -= v6;
    v5 -= 10;
    v6 = v4 - v5;
    v6 += CReg_TileId;
    v5 = v4 + CReg_TileId;
    //    v5 = v4 - CReg_TileId;
    dregs[1] = v4;
    dregs[2] = v5;
    dregs[3] = v6;

    p_if (v4.add_cc(CReg_TileId, IAddCCGTE0)) {
        dregs[4] = 0.0F;
    }
    p_endif;
}

void test_set_sgn()
{
    VecHalf v1, v2;
    
    v1 = dregs[0];

    v2 = v1.set_sgn(1);
    v2 = v2.set_sgn(v1);
}

void test_short_cond()
{
    VecShort b = 16;

    p_if (b == 1) {
        b += 100;
    } p_elseif (b != 2) {
        b += 200;
    } p_elseif (b >= 3) {
        b += 300;
    } p_elseif (b < 4) {
        b += 400;
    }
    p_endif;

    p_if (b == 0) {
        b += 500;
    } p_elseif (b != 0) {
        b += 600;
    } p_elseif (b >= 0) {
        b += 700;
    } p_elseif (b < 0) {
        b += 800;
    }
    p_endif;

    dregs[1] = b.reinterpret<VecHalf>();
}

// test_intrinsics
//
// Generates the following:
//      SFPLOAD L0, 1, 3
//      SFPMAD  L0, L0, L0, L0, 1
//      SFPSTORE        L0, 0, 6
void test_intrinsics()
{
    __builtin_rvtt_sfpnop();
    __builtin_rvtt_sfpnop();

    __rvtt_vec_t v1 = __builtin_rvtt_sfpload(SFPLOAD_MOD0_REBIAS_EXP, 3);
    __rvtt_vec_t v2 = __builtin_rvtt_sfploadi(SFPLOADI_MOD0_SHORT, 12);

    v1 = __builtin_rvtt_sfpmad(v1, v1, v1, 1);

    __builtin_rvtt_sfpstore(v1, SFPSTORE_MOD0_FLOAT_REBIAS_EXP, 6);
    __builtin_rvtt_sfpstore(__builtin_rvtt_sfpassignlr(CReg_Neg_1.get()), SFPSTORE_MOD0_FLOAT_REBIAS_EXP, 6);

    v2 = __builtin_rvtt_sfpmov(v1, SFPMOV_MOD1_COMPSIGN);

    __builtin_rvtt_sfpencc(3, 10);
    __builtin_rvtt_sfpcompc();
    __builtin_rvtt_sfppushc();
    __builtin_rvtt_sfppopc();
    __builtin_rvtt_sfpsetcc_i(1, 1);
    __builtin_rvtt_sfpsetcc_v(v1, 2);

    v2 = __builtin_rvtt_sfpexexp(v1, SFPEXEXP_MOD1_DEBIAS);
    v2 = __builtin_rvtt_sfpexman(v1, SFPEXMAN_MOD1_PAD8);
    v1 = __builtin_rvtt_sfpsetexp_i(23, v1);
    v1 = __builtin_rvtt_sfpsetexp_v(v1, v2);
    v1 = __builtin_rvtt_sfpsetman_i(23, v1);
    v1 = __builtin_rvtt_sfpsetman_v(v1, v2);

    v2 = __builtin_rvtt_sfpand(v2, v1);
    v2 = __builtin_rvtt_sfpor(v2, v1);
    v2 = __builtin_rvtt_sfpnot(v1);

    v2 = __builtin_rvtt_sfpabs(v1, SFPABS_MOD1_FLOAT);
    v2 = __builtin_rvtt_sfpmuli(v2, 32, 1);
    v2 = __builtin_rvtt_sfpaddi(v2, 32, 0);
    v2 = __builtin_rvtt_sfpdivp2(32, v1, 1);

    v2 = __builtin_rvtt_sfplz(v1, 2);

    v2 = __builtin_rvtt_sfpshft_i(v2, -10);
    v2 = __builtin_rvtt_sfpshft_v(v2, v1);

    v2 = __builtin_rvtt_sfpiadd_i(10, v1, 2);
    v2 = __builtin_rvtt_sfpiadd_v(v2, v1, 1);

    v2 = __builtin_rvtt_sfpsetsgn_i(10, v1);
    v2 = __builtin_rvtt_sfpsetsgn_v(v2, v1);
}

void lots_of_conditionals()
{
    VecHalf x = 1.0f;
    p_if(((x >= 0.0f && x < 0.0f) ||
          (x == 0.0f && x != 0.0f)) ||
         ((x == 0.0f && x != 0.0f) ||
          (x == 0.0f && x != 0.0f))) {
    }
    p_endif;

    p_if(((x >= 0.0f && x < 0.0f) ||
          (x == 0.0f || x != 0.0f)) &&
         (!(x == 0.0f && x != 0.0f) ||
          !(x == 0.0f || x != 0.0f))) {
    }
    p_endif;
}

void stupid_example()
{
    // dregs[n] loads into a temporary LREG
    VecHalf a = dregs[0] + 2.0F;

    // This emits a load, move, mad
    dregs[3] = a * -dregs[1] + CReg_0p692871094 + kHalf;

    // This emits a load, loadi, mad (a * dregs[] goes down the mad path)
    dregs[4] = a * dregs[1] + 1.2F;

    // Carefull, this emits a muli followed by an addi (a * const goes down the muli path)
    dregs[4] = a * 1.5F + 1.2F;

    // This emits a loadi (into tmp), loadi (as a temp for 1.2F) and a mad
    VecHalf tmp = 1.5F;
    dregs[5] = a * tmp + 1.2F;
    
    p_if ((a >= 4.0F && a < 8.0F) || (a >= 12.0F && a < 16.0F)) {
        VecShort b = a.ex_exp(ExExpNoDebias);
        b &= 0xAA;
        p_tail_if (b >= 130) {
            dregs[6] = a.set_exp(127);
        }
    } p_elseif (a == 20.0F) {
        dregs[7] = a.abs();
    } p_else {
        VecShort exp = a.lz() - 19;
        exp = ~exp;
        dregs[8] = -a.set_exp(exp);
    }
    p_endif;
}

#if 0
// XXXXX bug
// This behavior was removed because it doesn't play w/ predicated
// execution, e.g.:
//  p_if (x == 0 && (y = y + 1) < 0)
// So just precluding it for now
void test_operator_equals()
{
    VecHalf x, y, z;

    y = (x = 1.0F);
    x = (y *= x);
    z = (x -= y);

    dregs[0] = x;
    dregs[1] = y;
    dregs[2] = z;

    VecShort a, b;
    a = (b = 1);
    dregs[0] = a;
}
#endif

int main(int argc, char* argv[])
{
    test_load_store();
    test_add();
    test_sub();
    test_mul();
    test_mad();
    test_permute_ops();
    test_loadi(10, 20);
    test_control_flow(5);
    test_mad_imm();
    test_man_exp();
    test_dreg_conditional();
    test_vhalf_conditional();
    test_creg_conditional1();
    test_creg_conditional2();
    test_bitwise();
    test_abs();
    test_lz();
    test_shft();
    test_complex();
    test_muli_addi();
    test_iadd();
    test_set_sgn();
    test_short_cond();
    test_intrinsics();
    stupid_example();
    //    test_operator_equals();
}
