#include <stdio.h>
#include <stdlib.h>
#include "test.h"

using namespace sfpi;

// test_load_store
void test_load_store()
{
    VecHalf a, b;

    a = dst_reg[0];
    b = dst_reg[1];
    VecHalf c = dst_reg[2];

    dst_reg[3] = a;

    dst_reg[4] = b;
    dst_reg[5] = c;

    dst_reg[6] = dst_reg[7];

    a = b;
    dst_reg[8] = a;

    //    dst_reg[9] = CReg_Neg_1;
}

// test_add
void test_add()
{
    dst_reg[0] = dst_reg[1] + dst_reg[2];
    dst_reg[3] = dst_reg[4] + dst_reg[5] + 0.5F;
    dst_reg[6] = dst_reg[7] + dst_reg[8] - 0.5F;

    VecHalf a = dst_reg[9];
    VecHalf b = dst_reg[10];
    VecHalf c = a + b - 0.5F;

    a += b;
    a += dst_reg[0];

    dst_reg[11] = c;
    dst_reg[12] = a + b + 0.5F;

    dst_reg[13] = CReg_0 + CReg_Neg_1;
    dst_reg[14] = CReg_Neg_1 + dst_reg[12];
    dst_reg[15] = dst_reg[12] + CReg_0p0020 + 0.5F;
    dst_reg[16] = c + CReg_Neg_0p6748 - 0.5F;

    //TTFIXME XXXX 
    //    dst_reg[17] = a + b + c;
}

void test_sub()
{
    dst_reg[0] = dst_reg[1] - dst_reg[2];
    dst_reg[3] = dst_reg[4] - dst_reg[5] + 0.5F;

    VecHalf a = dst_reg[6];
    VecHalf b = dst_reg[7];

    a -= b;
    a -= dst_reg[0];

    dst_reg[8] = -b;
    dst_reg[9] = a - b - 0.5F;

    dst_reg[10] = CReg_Neg_1 - b;

    dst_reg[11] = CReg_Neg_1 * a - b;
    dst_reg[14] = -dst_reg[12] * -a - dst_reg[13];
}

// test_mul
void test_mul()
{
    dst_reg[0] = dst_reg[1] * dst_reg[2];
    dst_reg[3] = dst_reg[4] * dst_reg[5] + 0.5F;
    dst_reg[6] = dst_reg[7] * dst_reg[8] - 0.5F;

    VecHalf a = dst_reg[9];
    VecHalf b = dst_reg[10];
    VecHalf c = a * b - 0.5F;

    a *= b;
    a *= dst_reg[0];

    dst_reg[11] = c;
    dst_reg[12] = a * b;
    dst_reg[12] = a * b + 0.5F;

    dst_reg[13] = CReg_0 * CReg_Neg_1;
    dst_reg[14] = CReg_Neg_1 * dst_reg[12];
    dst_reg[15] = dst_reg[12] * CReg_0p0020 + 0.5F;
    dst_reg[15] = c * CReg_Neg_0p6748 - 0.5F;

    dst_reg[16] = a * b * c;
    dst_reg[17] = CReg_0 * b * c;
}

// test_mad
void test_mad()
{
    dst_reg[0] = dst_reg[1] * dst_reg[2] + dst_reg[3];
    dst_reg[4] = dst_reg[5] * dst_reg[6] + dst_reg[7] + 0.5F;
    dst_reg[8] = dst_reg[9] * dst_reg[10] + dst_reg[11] - 0.5F;

    VecHalf a = dst_reg[12];
    VecHalf b = dst_reg[13];
    VecHalf c = dst_reg[14];
    VecHalf d = a * b + c - 0.5F;
    dst_reg[15] = d;
    dst_reg[16] = a * b + c + 0.5F;

    dst_reg[17] = a * b + CReg_Neg_1 - 0.5F;
    dst_reg[18] = CReg_0 * b + CReg_Neg_1 + 0.5F;
}

// test_permute_ops
//
void test_permute_ops()
{
    VecHalf b = dst_reg[1];
    VecHalf c = dst_reg[2];

    dst_reg[3] = dst_reg[4] * dst_reg[5] + c;
    dst_reg[6] = dst_reg[7] * b + dst_reg[8];
    dst_reg[9] = dst_reg[10] * b + c;

    // Use reloads below to address register pressure
    VecHalf a = dst_reg[0];
    dst_reg[11] = a * dst_reg[12] + dst_reg[13];

    b = dst_reg[1];
    c = dst_reg[2];

    dst_reg[14] = a * dst_reg[15] + c;
    dst_reg[19] = a * b + c;
}

void test_loadi(int32_t i, uint32_t ui)
{
    VecShort a;
    a = 255;
    a = -255;
    dst_reg[0] = a;

    VecUShort b;
    b = 255U;
    b = -255U;
    dst_reg[1] = b;

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
    dst_reg[5] = f;

    VecHalf g = 3.1f;
    dst_reg[6] = g;
}

void test_control_flow(int count)
{
    VecHalf v;

    v = 1.5F;
    for (int i = 0; i < count; i++) {
        if (i & 1) {
            v = dst_reg[1];
        } else {
            dst_reg[2] = v;
        }
    }
}

void test_mad_imm()
{
    VecHalf x = dst_reg[0];
    VecHalf a = 0.34F;
    VecHalf c = x * (x * a + -0.3F) + .5F;
    dst_reg[2] = c;
}

void test_man_exp()
{
    VecHalf v1;

    v1 = dst_reg[0];
    VecShort v2 = exman9(v1);
    VecShort v3 = exexp(v1);
    dst_reg[3] = v3;

    VecShort v4;
    p_if (v4.exexp_cc(v1, ExExpCCLT0)); {
    }
    p_endif;

    p_if (v4.exexp_nodebias_cc(v1, ExExpCCLT0)); {
    }
    p_endif;

    //    v1.set_man(v2);
    v1 = setexp(v1, v2);

    VecHalf v5 = dst_reg[1];
    v1 = addexp(v5, 20);
    v5 = setexp(v1, 10);
    v5 = setman(v1, 10);

    dst_reg[2] = v2;
    dst_reg[4] = v4;
}

void test_dreg_conditional()
{
    VecHalf v = dst_reg[0];

    p_if(v < 5.0F) {
        p_if(v >= 2.0F) {
            dst_reg[2] = v;
        } p_elseif(v != 4) {
            dst_reg[3] = v;
        }
        p_endif;

        dst_reg[4] = v;
    } p_elseif(v >= 6.0F) {
        p_if(v == 0.0F); {
            dst_reg[5] = v;
        }
        p_endif;
    } p_elseif(!(v >= 6.0F)) {
        dst_reg[6] = v;
    } p_else {
        dst_reg[7] = v;
    }
    p_endif;

    dst_reg[8] = v;
}

void test_vhalf_conditional()
{
    VecHalf v = dst_reg[0];
    VecHalf x = 5.0F;

    p_if(v < x) {
        p_if(v >= x) {
            dst_reg[2] = v;
        } p_elseif(v != x) {
            dst_reg[3] = v;
        }
        p_endif;

        dst_reg[4] = v;
    } p_elseif(v >= x) {
        p_if(v == x); {
            dst_reg[5] = v;
        }
        p_endif;
    } p_elseif(!(v >= x)) {
        dst_reg[6] = v;
    } p_else {
        dst_reg[7] = v;
    }
    p_endif;

    dst_reg[8] = v;
}

void test_creg_conditional1()
{
    VecHalf v = dst_reg[0];

    p_if(v < CReg_Neg_1) {
        p_if(v >= CReg_Neg_1) {
            dst_reg[2] = v;
        } p_elseif(v != CReg_Neg_1) {
            dst_reg[3] = v;
        }
        p_endif;

        dst_reg[4] = v;
    } p_elseif(v >= CReg_Neg_1) {
        p_if(v == CReg_Neg_1); {
            dst_reg[5] = v;
        }
        p_endif;
    } p_elseif(!(v >= CReg_Neg_1)) {
        dst_reg[6] = v;
    } p_else {
        dst_reg[7] = v;
    }
    p_endif;

    dst_reg[8] = v;
}

void test_creg_conditional2()
{
    VecHalf v = dst_reg[0];

    p_if(CReg_Neg_1 < v) {
        p_if(CReg_Neg_1 >= v) {
            dst_reg[2] = v;
        } p_elseif(CReg_Neg_1 != v) {
            dst_reg[3] = v;
        }
        p_endif;

        dst_reg[4] = v;
    } p_elseif(CReg_Neg_1 >= v) {
        p_if(CReg_Neg_1 == v); {
            dst_reg[5] = v;
        }
        p_endif;
    } p_elseif(!(CReg_Neg_1 >= v)) {
        dst_reg[6] = v;
    } p_else {
        dst_reg[7] = v;
    }
    p_endif;

    dst_reg[8] = v;
}

void test_bitwise()
{
    VecShort v1, v2;

    v1 = 1;
    v2 = 2;

    v1 |= v2;
    v1 |= 0xAA;
    dst_reg[2] = v1;
    v1 &= v2;
    v1 &= 0xAA;
    dst_reg[3] = v2;
    dst_reg[4] = ~v1;

    dst_reg[5] = v1 | v2;
    dst_reg[6] = v1 & v2;

    VecUShort v3, v4;
    v3 = 3U;
    v4 = 4U;
    v3 |= v4;
    v3 |= 0xAAU;
    dst_reg[2] = v3;
    v3 &= v4;
    v3 &= 0xAAU;
    dst_reg[3] = v4;
    dst_reg[4] = ~v3;

    dst_reg[5] = v3 | v4;
    dst_reg[6] = v3 & v4;
}

void test_abs()
{
    VecShort v1 = -5;
    VecShort v2 = abs(v1);

    v2 = abs(v2) + 1;

    dst_reg[0] = v1;
    dst_reg[1] = v2;

    VecHalf v3 = -6.0f;
    VecHalf v4 = abs(v3);
    v4 = abs(v4) + 1.0f;

    dst_reg[2] = v3;
    dst_reg[3] = v4;
}

void test_lz()
{
    VecShort v1;
    VecUShort v2;
    VecHalf v3;

    v3 = 10.0f;
    v1 = lz(v3) + 1;
    v2 = lz(v3) + 1;
    v1 = lz(v1) + 1;
    v2 = lz(v2) + 1;
    dst_reg[0] = v1;
    dst_reg[1] = v2;
    dst_reg[2] = v3;
}

void test_shft()
{
    VecShort v1 = 1;
    VecShort v2 = 2;
    VecUShort v3 = 3;
    VecUShort v4;

    v1 = v1 << -4;
    v1 = v2 << 5;
    v1 <<= 6;

    v4 = v3 >> -7;
    v4 = v3 << 8;

    v3 = v3 >> -9;
    v3 = v3 << 10;

    v3 <<= -11;
    v3 >>= 12;

    v2 = shft(v3, v1);

    dst_reg[0] = v1;
    dst_reg[1] = v2;
    dst_reg[2] = v3;
}

// Had issues w/ implict copy/move, test some of that here
void test_complex()
{
    VecHalf a = dst_reg[0];
    VecHalf b = dst_reg[1];
    VecHalf c = dst_reg[2];
    c = dst_reg[2];
    c = a + b;
    dst_reg[2] = a + b;
    dst_reg[2] = a + b;

    // Do we want to support this? Presently operator= is defined to return void
    // dst_reg[2] = (c = a + b);
}

void test_muli_addi()
{
    VecHalf a = dst_reg[0];

    a *= 16;
    a += 12;

    dst_reg[1] = a;

    VecHalf b, c;

    b = a * 16;
    c = a + 12;

    dst_reg[2] = b;
    dst_reg[3] = c;

    // Do we want to support this? Presently operator= is defined to return void
    // dst_reg[2] = (c = a + b);
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
    dst_reg[1] = v1;
    dst_reg[2] = v2;
    dst_reg[3] = v3;

    p_if (v1.add_cc(CReg_TileId, IAddCCGTE0)) {
        dst_reg[4] = 0.0F;
    }
    p_endif;

    VecUShort v4, v5;

    v4 = 12U;
    v5 = v4 + 10U;
    v5 += v4;
    v5 += 10U;
    VecUShort v6 = v4 + v5;
    v5 -= v6;
    v5 -= 10U;
    v6 = v4 - v5;
    v6 += CReg_TileId;
    v5 = v4 + CReg_TileId;
    //    v5 = v4 - CReg_TileId;
    dst_reg[1] = v4;
    dst_reg[2] = v5;
    dst_reg[3] = v6;

    p_if (v4.add_cc(CReg_TileId, IAddCCGTE0)) {
        dst_reg[4] = 0.0F;
    }
    p_endif;
}

void test_icmp()
{
    VecShort a = 1;
    VecShort b = 2;

    p_if (a < b) {
    }
    p_endif;
    p_if (a >= b) {
    }
    p_endif;
#if 0
    p_if (a == b) {
    }
    p_endif;
    p_if (a != b) {
    }
    p_endif;
#endif

    p_if (a < 3) {
    }
    p_endif;
    p_if (a == 4) {
    }
    p_endif;
    p_if (a != 5) {
    }
    p_endif;
    p_if (a >= 6) {
    }
    p_endif;

    VecUShort c = 10;
    VecUShort d = 11;

    p_if (c < d) {
    }
    p_endif;
    p_if (c >= d) {
    }
    p_endif;
#if 0
    p_if (c == d) {
    }
    p_endif;
    p_if (c != d) {
    }
    p_endif;
#endif

    p_if (c < 13) {
    }
    p_endif;
    p_if (c == 14) {
    }
    p_endif;
    p_if (c != 15) {
    }
    p_endif;
    p_if (c >= 16) {
    }
    p_endif;
}

void test_iadd2()
{
#if 0
    int32_t x = 0; //3343343;

    VecUShort a = x;

    p_if (a < 0x801) {
    }
    p_endif;
#endif
}

void test_set_sgn()
{
    VecHalf v1, v2;
    
    v1 = dst_reg[0];

    v2 = setsgn(v1, 1);
    v2 = setsgn(v2, v1);
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

    dst_reg[1] = reinterpret<VecHalf>(b);
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

void stupid_example(unsigned int value)
{
    // dst_reg[n] loads into a temporary LREG
    VecHalf a = dst_reg[0] + 2.0F;

    // This emits a load, move, mad
    dst_reg[3] = a * -dst_reg[1] + CReg_0p6929 + 0.5F;

    // This emits a load, loadi, mad (a * dst_reg[] goes down the mad path)
    dst_reg[4] = a * dst_reg[1] + 1.2F;

    // This emits a 2 loadis and a mad
    dst_reg[4] = a * 1.5F + 1.2F;

    // This emits a loadi (into tmp), loadi (as a temp for 1.2F) and a mad
    VecHalf tmp = ScalarFP16a(value);
    dst_reg[5] = a * tmp + 1.2F;
    
    p_if ((a >= 1.0F && a < 8.0F) || (a >= 12.0F && a < 16.0F)) {
        VecShort b = exexp_nodebias(a);
        p_if (b >= 0) {
            dst_reg[6] = setexp(a, 127);
        }
        p_endif;
    } p_elseif (a == ScalarFP16a(3.0F)) {
        dst_reg[7] = abs(a);
    } p_else {
        VecShort exp = lz(a) - 19;
        exp = ~exp;
        exp &= 0xAA;
        dst_reg[8] = -setexp(a, exp);
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

    dst_reg[0] = x;
    dst_reg[1] = y;
    dst_reg[2] = z;

    VecShort a, b;
    a = (b = 1);
    dst_reg[0] = a;
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
    stupid_example(argc);
    //    test_operator_equals();
}
