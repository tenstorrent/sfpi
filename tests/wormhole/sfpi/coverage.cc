#include <stdio.h>
#include <stdlib.h>
#include "test.h"

using namespace sfpi;

void test_load_store()
{
    vFloat a, b;

    a = dst_reg[0];
    b = dst_reg[1];
    vFloat c = dst_reg[2];

    dst_reg[3] = a;

    dst_reg[4] = b;
    dst_reg[5] = c;

    dst_reg[6] = dst_reg[7];

    a = b;
    dst_reg[8] = a;

    dst_reg[9] = vConst1;
}

void test_add()
{
    dst_reg[0] = dst_reg[1] + dst_reg[2];
    dst_reg[3] = dst_reg[4] + dst_reg[5] + 0.5F;
    dst_reg[6] = dst_reg[7] + dst_reg[8] - 0.5F;

    vFloat a = dst_reg[9];
    vFloat b = dst_reg[10];
    vFloat c = a + b - 0.5F;

    a += b;
    a += dst_reg[0];
    a += vConst1;

    dst_reg[11] = c;
    dst_reg[12] = a + b + 0.5F;

    dst_reg[13] = vConst0 + vConst1;
    dst_reg[14] = vConst1 + dst_reg[12];
    dst_reg[15] = dst_reg[12] + vConst0p8737 + 0.5F;
    dst_reg[16] = c + vConstPrgm0 - 0.5F;

    dst_reg[17] = a + b + c;
    dst_reg[18] = a + b + c + dst_reg[0] + vConst1 + 5.0f + 0.5F;
}

void test_sub()
{
    dst_reg[0] = dst_reg[1] - dst_reg[2];
    dst_reg[3] = dst_reg[4] - dst_reg[5] + 0.5F;
    dst_reg[6] = dst_reg[7] - dst_reg[8] - 0.5F;

    vFloat a = dst_reg[9];
    vFloat b = dst_reg[10];
    vFloat c = a - b - 0.5F;

    a -= b;
    a -= dst_reg[0];
    a -= vConst1;
    a -= b + c;

    dst_reg[11] = c;
    dst_reg[12] = a - b + 0.5F;

    dst_reg[17] = a + b - c;
    // Releive reg pressure...
    a = dst_reg[9];
    dst_reg[18] = a - b - c - dst_reg[0] - vConst1 - 5.0f + 0.5F;

    dst_reg[13] = vConst0 - vConst1;
    dst_reg[14] = vConst1 - dst_reg[12];
    dst_reg[15] = dst_reg[12] - vConstPrgm1 + 0.5F;
    dst_reg[16] = c - vConstPrgm2 - 0.5F;
}

void test_mul()
{
    dst_reg[0] = dst_reg[1] * dst_reg[2];
    dst_reg[3] = dst_reg[4] * dst_reg[5] + 0.5F;
    dst_reg[6] = dst_reg[7] * dst_reg[8] - 0.5F;

    vFloat a = dst_reg[9];
    vFloat b = dst_reg[10];
    vFloat c = a * b - 0.5F;

    a *= b;
    a *= -b;
    a *= dst_reg[0];
    a *= -dst_reg[0];
    a *= vConst1;

    dst_reg[11] = c;
    dst_reg[12] = a * b + 0.5F;

    dst_reg[13] = vConst0 * vConst1;
    dst_reg[14] = vConst1 * dst_reg[12];
    dst_reg[15] = dst_reg[12] * vConstPrgm3 + 0.5F;
    dst_reg[16] = c * vConstPrgm3 - 0.5F;

    dst_reg[17] = a * b * c;
    dst_reg[18] = a * b * c * dst_reg[0] * vConst1 * 5.0f + 0.5F;
}

void test_mad()
{
    dst_reg[0] = dst_reg[1] * dst_reg[2] + dst_reg[3];
    dst_reg[4] = dst_reg[5] * dst_reg[6] + dst_reg[7] + 0.5F;
    dst_reg[8] = dst_reg[9] * dst_reg[10] + dst_reg[11] - 0.5F;

    vFloat a = dst_reg[12];
    vFloat b = dst_reg[13];
    vFloat c = dst_reg[14];
    vFloat d = a * b + c - 0.5F;
    dst_reg[15] = d;
    dst_reg[16] = a * b + c + 0.5F;

    dst_reg[17] = a * b + vConst1 - 0.5F;
    dst_reg[18] = vConst0 * b + vConst1 + 0.5F;
    dst_reg[19] = vConst0 * -b + -a + 0.5F;
}

void test_loadi(int32_t i, uint32_t ui)
{
    vInt a = 10;
    a = 255;
    dst_reg[0] = a;
    a = -255;
    dst_reg[0] = a;
    a = i;
    dst_reg[0] = a;
    a = ui;
    dst_reg[0] = a;

    vUInt b = 20;
    b = 255U;
    dst_reg[1] = b;
    b = -255U;
    dst_reg[1] = b;
    b = i;
    dst_reg[1] = b;
    b = ui;
    dst_reg[1] = b;

    vFloat c;
    c = s2vFloat16a(1.0F);
    dst_reg[2] = c;
    c = s2vFloat16a(0x3F80);
    dst_reg[2] = c;
    c = s2vFloat16a(0x3F80U);
    dst_reg[2] = c;
    c = s2vFloat16b(1.0F);
    dst_reg[2] = c;
    c = s2vFloat16b(0x3F80);
    dst_reg[2] = c;
    c = s2vFloat16b(0x3F80U);
    dst_reg[2] = c;
    c = s2vFloat16b(i);
    dst_reg[2] = c;
    c = s2vFloat16b(ui);
    dst_reg[2] = c;

    vFloat f;
    f = 3.0f;
    dst_reg[5] = f;
    f = -3.0f;
    dst_reg[5] = f;

    vFloat g = 3.1f;
    dst_reg[6] = g;

    vFloat h = 3.0;
    dst_reg[7] = h;
}

void test_control_flow(int count)
{
    vFloat v;

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
    vFloat x = dst_reg[0];
    vFloat a = 0.34F;
    vFloat c = x * (x * a + -0.3F) + .5F;
    dst_reg[2] = c;
}

void test_exman_exexp()
{
    vFloat v1;

    v1 = dst_reg[0];
    vInt v2 = exman8(v1);
    v2 = exman9(v1);
    vInt v3 = exexp(v1);
    v3 = exexp_nodebias(v1);
    dst_reg[3] = v3;

    vInt v4;
    v_if (v4.exexp_cc(v1, ExExpCCLT0)); {
    }
    v_endif;

    v_if (v4.exexp_nodebias_cc(v1, ExExpCCLT0)); {
    }
    v_endif;
}

void test_setman_setexp_addexp_setsgn()
{
    vFloat v1 = 1.0f;
    vInt v2 = 1;
    vInt v3 = 1;

    v1 = setexp(v1, 0x3ff);
    v1 = setexp(v1, v2);
    v1 = setexp(v1, v3);

    v1 = setman(v1, 0x3ff);
    v1 = setman(v1, v2);
    v1 = setman(v1, v3);

    vFloat v5 = dst_reg[1];
    v1 = addexp(v5, 20);
    v1 = addexp(v5, -20);

    v1 = setsgn(v5, 1);
    v1 = setsgn(v5, -1);
    v1 = setsgn(v5, v2);
    vFloat v6 = dst_reg[2];
    v1 = setsgn(v5, v6);

    dst_reg[2] = v1;
}

void test_dreg_conditional_const()
{
    vFloat v = dst_reg[0];

    v_if(dst_reg[0] < 5.0F) {
        v_if(dst_reg[0] >= 2.0F) {
            dst_reg[2] = v;
        } v_elseif(dst_reg[0] != 4) {
            dst_reg[3] = v;
        }
        v_endif;

        dst_reg[4] = v;
    } v_elseif(dst_reg[0] >= 6.0F) {
        v_if(dst_reg[0] == 0.0F); {
            dst_reg[5] = v;
        }
        v_endif;
    } v_elseif(!(dst_reg[0] >= 6.0F)) {
        dst_reg[6] = v;
    } v_else {
        dst_reg[7] = v;
    }
    v_endif;

    v_if(dst_reg[0] <= 7.0F) {
        dst_reg[8] = v;
    } v_elseif (!(dst_reg[0] <= 8.0F)) {
        dst_reg[9] = v;
    } v_elseif (dst_reg[0] > 9.0F) {
        dst_reg[10] = v;
    } v_elseif (!(dst_reg[0] > 10.0F)) {
        dst_reg[11] = v;
    }
    v_endif;

    dst_reg[8] = v;
}

void test_vhalf_conditional_const()
{
    vFloat v = dst_reg[0];

    v_if(v < 5.0F) {
        v_if(v >= 2.0F) {
            dst_reg[2] = v;
        } v_elseif(v != 4) {
            dst_reg[3] = v;
        }
        v_endif;

        dst_reg[4] = v;
    } v_elseif(v >= 6.0F) {
        v_if(v == 0.0F); {
            dst_reg[5] = v;
        }
        v_endif;
    } v_elseif(!(v >= 6.0F)) {
        dst_reg[6] = v;
    } v_else {
        dst_reg[7] = v;
    }
    v_endif;

    v_if(v <= 7.0F) {
        dst_reg[8] = v;
    } v_elseif (!(v <= 8.0F)) {
        dst_reg[9] = v;
    } v_elseif (v > 9.0F) {
        dst_reg[10] = v;
    } v_elseif (!(v > 10.0F)) {
        dst_reg[11] = v;
    }
    v_endif;

    dst_reg[8] = v;
}

void test_vhalf_conditional()
{
    vFloat v = dst_reg[0];
    vFloat x = 1.0f;

    v_if(v < x) {
        v_if(v >= x) {
            dst_reg[2] = v;
        } v_elseif(v != x) {
            dst_reg[3] = v;
        }
        v_endif;

        dst_reg[4] = v;
    } v_elseif(v >= x) {
        v_if(v == 0.0F); {
            dst_reg[5] = v;
        }
        v_endif;
    } v_elseif(!(v >= x)) {
        dst_reg[6] = v;
    } v_else {
        dst_reg[7] = v;
    }
    v_endif;

    v_if(v <= x) {
        dst_reg[8] = v;
    } v_elseif (!(v <= x)) {
        dst_reg[9] = v;
    } v_elseif (v > x) {
        dst_reg[10] = v;
    } v_elseif (!(v > x)) {
        dst_reg[11] = v;
    }
    v_endif;

    dst_reg[8] = v;
}

void test_creg_conditional()
{
    vFloat v = dst_reg[0];

    v_if(v < vConst1) {
        v_if(v >= vConst1) {
            dst_reg[2] = v;
        } v_elseif(v != vConst1) {
            dst_reg[3] = v;
        }
        v_endif;

        dst_reg[4] = v;
    } v_elseif(v >= vConst1) {
        v_if(v == vConst1); {
            dst_reg[5] = v;
        }
        v_endif;
    } v_elseif(!(v >= vConst1)) {
        dst_reg[6] = v;
    } v_else {
        dst_reg[7] = v;
    }
    v_endif;

    v_if(v <= vConst1) {
        dst_reg[8] = v;
    } v_elseif (!(v <= vConst1)) {
        dst_reg[9] = v;
    } v_elseif (v > vConst1) {
        dst_reg[10] = v;
    } v_elseif (!(v > vConst1)) {
        dst_reg[11] = v;
    }
    v_endif;

    dst_reg[8] = v;
}

void test_creg_conditional_rev()
{
    vFloat v = dst_reg[0];

    v_if(vConst1 < v) {
        v_if(vConst1 >= v) {
            dst_reg[2] = v;
        } v_elseif(vConst1 != v) {
            dst_reg[3] = v;
        }
        v_endif;

        dst_reg[4] = v;
    } v_elseif(vConst1 >= v) {
        v_if(vConst1 == v); {
            dst_reg[5] = v;
        }
        v_endif;
    } v_elseif(!(vConst1 >= v)) {
        dst_reg[6] = v;
    } v_else {
        dst_reg[7] = v;
    }
    v_endif;

    v_if(vConst1 <= v) {
        dst_reg[8] = v;
    } v_elseif (!(vConst1 <= v)) {
        dst_reg[9] = v;
    } v_elseif (vConst1 > v) {
        dst_reg[10] = v;
    } v_elseif (!(vConst1 > v)) {
        dst_reg[11] = v;
    }
    v_endif;

    dst_reg[8] = v;
}

void test_bitwise()
{
    vInt v1, v2;

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
    dst_reg[7] = ((v1 & v2) | vInt(0xAA)) & ~v2;

    vUInt v3, v4;
    v3 = 3U;
    v4 = 4U;

    v3 |= v4;
    v3 |= 0xAAU;
    dst_reg[8] = v3;
    v3 &= v4;
    v3 &= 0xAAU;
    dst_reg[9] = v4;
    dst_reg[10] = ~v3;

    dst_reg[11] = v3 | v4;
    dst_reg[12] = v3 & v4;
    dst_reg[13] = ((v3 & v4) | vInt(0xAA)) & ~v4;
}

void test_abs()
{
    vInt v1 = -5;
    vInt v2 = abs(v1);

    v2 = abs(v2) + 1;

    dst_reg[0] = v1;
    dst_reg[1] = v2;

    vFloat v3 = -6.0f;
    vFloat v4 = abs(v3);
    v4 = abs(v4) + 1.0f;

    dst_reg[2] = v3;
    dst_reg[3] = v4;
}

void test_lz()
{
    vInt v1;
    vUInt v2;
    vFloat v3;

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
    vInt v1 = 1;
    vInt v2 = 2;
    vUInt v3 = 3;
    vUInt v4;

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
    vFloat a = dst_reg[0];
    vFloat b = dst_reg[1];
    vFloat c = dst_reg[2];
    c = dst_reg[2];
    c = a + b;
    dst_reg[2] = a + b;
    dst_reg[2] = a + b;

    // XXXX fix when operator= is no longer defined to return void
    // dst_reg[2] = (c = a + b);
}

void test_muli_addi()
{
    vFloat a = dst_reg[0];

    a += 12;
    a += s2vFloat16b(16.0f);
    a += s2vFloat16a(16.0f);
    a *= 16;
    a *= s2vFloat16b(16.0f);
    a *= s2vFloat16a(16.0f);

    dst_reg[1] = a;

    vFloat b, c;

    c = a + 12;
    c = a + s2vFloat16b(16.0f);
    c = a + s2vFloat16a(16.0f);

    b = a * 16;
    b = a * s2vFloat16b(16.0f);
    b = a * s2vFloat16a(16.0f);

    dst_reg[2] = b;
    dst_reg[3] = c;

    // XXXX fix when operator= is no longer defined to return void
    // dst_reg[2] = (c = a + b);
}

void test_iadd()
{
    vInt v1, v2;

    v1 = 12;
    v2 = v1 + 10;
    v2 += v1;
    v2 += 10;
    vInt v3 = v1 + v2;
    v2 -= v3;
    v2 -= 10;
    v3 = v1 - v2;
    v3 += vConstTileId;
    v2 = v1 + vConstTileId;
    v2 = v1 - vConstTileId;
    v2 -= vConstTileId;
    dst_reg[1] = v1;
    dst_reg[2] = v2;
    dst_reg[3] = v3;

    v_if (v1.add_cc(vConstTileId, IAddCCGTE0)) {
        dst_reg[4] = 0.0F;
    }
    v_endif;

    vUInt v4, v5;

    v4 = 12U;
    v5 = v4 + 10U;
    v5 += v4;
    v5 += 10U;
    vUInt v6 = v4 + v5;
    v5 -= v6;
    v5 -= 10U;
    v6 = v4 - v5;
    v6 += vConstTileId;
    v5 = v4 + vConstTileId;
    v5 = v4 - vConstTileId;
    v5 -= vConstTileId;
    dst_reg[1] = v4;
    dst_reg[2] = v5;
    dst_reg[3] = v6;

    v_if (v4.add_cc(vConstTileId, IAddCCGTE0)) {
        dst_reg[4] = 0.0F;
    }
    v_endif;
}

void test_icmp_i()
{
    vInt a = 5;

    v_if (a == 1) {
        a += 100;
    } v_elseif (a != 2) {
        a += 200;
    } v_elseif (a < 3) {
        a += 300;
    } v_elseif (a <= 4) {
        a += 400;
    } v_elseif (a > 5) {
        a += 500;
    } v_elseif (a >= 6) {
        a += 600;
    }
    v_endif;

    dst_reg[0] = a;

    vUInt b = 16;

    v_if (b == 1) {
        b += 100;
    } v_elseif (b != 2) {
        b += 200;
    } v_elseif (b < 3) {
        b += 300;
    } v_elseif (b <= 4) {
        b += 400;
    } v_elseif (b > 5) {
        b += 500;
    } v_elseif (b >= 6) {
        b += 600;
    }
    v_endif;

    dst_reg[1] = b;
}

void test_icmp_v()
{
    vInt a = 1;
    vInt b = 2;

    v_if (a == b) {
        b += 100;
    } v_elseif (a != b) {
        b += 200;
    } v_elseif (a < b) {
        b += 300;
    } v_elseif (a <= b) {
        b += 400;
    } v_elseif (a > b) {
        b += 500;
    } v_elseif (a >= b) {
        b += 600;
    }
    v_endif;
    dst_reg[0] = b;

    vUInt c = 10;
    vUInt d = 11;

    v_if (c == d) {
        d += 700;
    } v_elseif (c != d) {
        d += 800;
    } v_elseif (c < d) {
        d += 900;
    } v_elseif (c <= d) {
        d += 1000;
    } v_elseif (c > d) {
        d += 1100;
    } v_elseif (c >= d) {
        d += 1200;
    }
    v_endif;
    dst_reg[0] = b;
}

void lots_of_conditionals()
{
    vFloat x = 1.0f;

    v_if ((x == 0.0f && x == 0.0f) || (x == 0.0f && x == 0.0f)) {
    }
    v_endif;

    v_if(((x >= 0.0f && x < 0.0f) ||
          (x == 0.0f && x != 0.0f)) ||
         ((x == 0.0f && x != 0.0f) ||
          (x == 0.0f && x != 0.0f))) {
    }
    v_endif;

    v_if(((x >= 0.0f && x < 0.0f) ||
          (x == 0.0f || x != 0.0f)) &&
         (!(x == 0.0f && x != 0.0f) ||
          !(x == 0.0f || x != 0.0f))) {
    }
    v_endif;

    v_if(!(x >= 0.0f && x < 0.0f) && !(x >= 0.0f && x < 0.0f)) {
    }
    v_endif;
}

void test_lut()
{
    vInt a = 0;
    vInt b = 1;
    vInt c = 2;
    vFloat d = 1.0f;

    d = lut(d, a, b, c, 1);
    dst_reg[1] = lut_sign(d, a, b, c, -1);
}

void stupid_example(unsigned int value)
{
    // dst_reg[n] loads into a temporary LREG
    vFloat a = dst_reg[0] + 2.0F;

    // This emits a load, move, mad
    dst_reg[3] = a * -dst_reg[1] + vConst0 + 0.5F;

    // This emits a load, loadi, mad (a * dst_reg[] goes down the mad path)
    dst_reg[4] = a * dst_reg[1] + 1.2F;

    // This emits a 2 loadis and a mad
    dst_reg[4] = a * 1.5F + 1.2F;

    // This emits a loadi (into tmp), loadi (as a temp for 1.2F) and a mad
    vFloat tmp = s2vFloat16a(value);
    dst_reg[5] = a * tmp + 1.2F;
    
    v_if ((a >= 1.0F && a < 8.0F) || (a >= 12.0F && a < 16.0F)) {
        vInt b = exexp_nodebias(a);
        v_if (b >= 0) {
            dst_reg[6] = setexp(a, 127);
        }
        v_endif;
    } v_elseif (a == s2vFloat16a(3.0F)) {
        dst_reg[7] = abs(a);
    } v_else {
        vInt exp = lz(a) - 19;
        exp = ~exp;
        exp &= 0xAA;
        dst_reg[8] = -setexp(a, exp);
    }
    v_endif;
}

#if 0
// XXXXX bug
// This behavior was removed because it doesn't play w/ predicated
// execution, e.g.:
//  v_if (x == 0 && (y = y + 1) < 0)
// So just precluding it for now
void test_operator_equals()
{
    vFloat x, y, z;

    y = (x = 1.0F);
    x = (y *= x);
    z = (x -= y);

    dst_reg[0] = x;
    dst_reg[1] = y;
    dst_reg[2] = z;

    vInt a, b;
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
    test_loadi(10, 20);
    test_control_flow(5);
    test_mad_imm();
    test_setman_setexp_addexp_setsgn();
    test_dreg_conditional_const();
    test_vhalf_conditional_const();
    test_vhalf_conditional();
    test_creg_conditional();
    test_creg_conditional_rev();
    test_bitwise();
    test_abs();
    test_lz();
    test_shft();
    test_complex();
    test_muli_addi();
    test_iadd();
    test_icmp_i();
    test_icmp_v();
    test_lut();
    stupid_example(argc);
    //    test_operator_equals();
}
