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

    a = dst_reg[-1];
    a = dst_reg[0xFFFFFF];
    dst_reg[-1] = a;
    dst_reg[0xFFFFFF] = a;
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
    dst_reg[15] = dst_reg[12] + vConst0p8373 + 0.5F;
    dst_reg[16] = c + vConstFloatPrgm0 - 0.5F;

    dst_reg[17] = a + b + c;
    dst_reg[18] = a + b + c + dst_reg[0] + vConst1 + 5.0f + 0.5F;

    dst_reg[19] = 1.0f + a;
    dst_reg[20] = 1.0f + dst_reg[19];
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
    dst_reg[15] = dst_reg[12] - vConstFloatPrgm1 + 0.5F;
    dst_reg[16] = c - vConstFloatPrgm2 - 0.5F;

    dst_reg[17] = 1.0f - a;
    dst_reg[18] = 1.0f - dst_reg[19];
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
    dst_reg[15] = dst_reg[12] * vConstFloatPrgm2 + 0.5F;
    dst_reg[16] = c * vConstFloatPrgm1 - 0.5F;

    dst_reg[17] = a * b * c;
    dst_reg[18] = a * b * c * dst_reg[0] * vConst1 * 5.0f + 0.5F;

    dst_reg[19] = 1.0f * a;
    dst_reg[20] = 1.0f * dst_reg[19];
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

void test_incdec()
{
    {
        vFloat x = 1.0f;

        dst_reg[0] = x++;
        dst_reg[1] = ++x;
        dst_reg[2] = x;

        dst_reg[0] = x--;
        dst_reg[1] = --x;
        dst_reg[2] = x;
    }
    {
        vInt x = 1;

        dst_reg[0] = x++;
        dst_reg[1] = ++x;
        dst_reg[2] = x;

        dst_reg[0] = x--;
        dst_reg[1] = --x;
        dst_reg[2] = x;
    }

    {
        vUInt x = 1;

        dst_reg[0] = x++;
        dst_reg[1] = ++x;
        dst_reg[2] = x;

        dst_reg[0] = x--;
        dst_reg[1] = --x;
        dst_reg[2] = x;
    }
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
    v_if ((v4 = exexp(v1)) < 0) {
    }
    v_endif;

    v_if ((v4 = exexp_nodebias(v1)) < 0) {
    }
    v_endif;
}

void test_setman_setexp_addexp_setsgn(int val)
{
    vFloat v1 = 1.0f;
    vInt v2 = 1;
    vInt v3 = 1;

    v1 = setexp(v1, 0x3ff);
    v1 = setexp(v1, v2);
    v1 = setexp(v1, v3);
    v1 = setexp(v1, 0xFFFFF);
    v1 = setexp(v1, val);
    v1 = setexp(v1, -1);

    v1 = setman(v1, 0x3ff);
    v1 = setman(v1, v2);
    v1 = setman(v1, v3);
    v1 = setman(v1, 0xFFFFF);
    v1 = setman(v1, val);
    v1 = setman(v1, -1);

    vFloat v5 = dst_reg[1];
    v1 = addexp(v5, 20);
    v1 = addexp(v5, -20);
    v1 = addexp(v5, val);
    v1 = addexp(v5, 0xFFFFF);

    v1 = setsgn(v5, 1);
    v1 = setsgn(v5, -1);
    v1 = setsgn(v5, v2);
    v1 = setsgn(v5, val);
    v1 = setsgn(v5, 0xFFFFF);

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

void test_vhalf_conditional_reverse()
{
    vFloat x = 1.0f;

    v_if(2.0f < x) {
        v_if(2.0f >= x) {
            dst_reg[2] = 2.0f;
        } v_elseif(2.0f != x) {
            dst_reg[3] = 2.0f;
        }
        v_endif;

        v_if(2.0f == x) {
            dst_reg[4] = 2.0f;
        }
        v_endif;

        dst_reg[4] = 2.0f;
    } v_elseif(2.0f > x) {
        dst_reg[5] = 2.0f;
    } v_elseif(!(2.0f >= x)) {
        dst_reg[6] = 2.0f;
    } v_else {
        dst_reg[7] = 2.0f;
    }
    v_endif;

    v_if(2.0f <= x) {
        dst_reg[8] = 2.0f;
    } v_elseif (!(2.0f <= x)) {
        dst_reg[9] = 2.0f;
    } v_elseif (2.0f > x) {
        dst_reg[10] = 2.0f;
    } v_elseif (!(2.0f > x)) {
        dst_reg[11] = 2.0f;
    }
    v_endif;

    dst_reg[8] = 2.0f;
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

void test_conditional_outside_vif()
{
    vFloat x = 1.0f;
    vInt y = (x == 0.0f);
    vUInt z = (x == 0.0f);

    v_if (y == 0) {
    }
    v_endif;

    v_if (z != 0) {
    }
    v_endif;

    v_if (y) {
    } v_elseif(y) {
    }
    v_endif;

    v_if (z) {
    } v_elseif(z) {
    }
    v_endif;
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

    v1 = 8;
    v1 = 5 | v1;
    v1 = 5 & v1;
    v1 = 5 ^ v1;
    dst_reg[14] = v1;

    v3 = 5 | v3;
    v3 = 5 & v3;
    v3 = 5 ^ v3;
    dst_reg[15] = v3;
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

    v1 = lz_nosgn(v3) + 1;
    v2 = lz_nosgn(v3) + 1;
    v1 = lz_nosgn(v1) + 1;
    v2 = lz_nosgn(v2) + 1;

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

    v_if ((v1 = v1 + vConstTileId) >= 0) {
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

    v_if ((v4 = v4 + vConstTileId) >= 0) {
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

void test_icmp_i_rev()
{
    vInt a = 5;

    v_if (1 == a) {
        a += 100;
    } v_elseif (2 != a) {
        a += 200;
    } v_elseif (3 < a) {
        a += 300;
    } v_elseif (4 <= a) {
        a += 400;
    } v_elseif (5 > a) {
        a += 500;
    } v_elseif (6 >= a) {
        a += 600;
    }
    v_endif;

    dst_reg[0] = a;

    vUInt b = 16;

    v_if (1 == b) {
        b += 100;
    } v_elseif (2 != b) {
        b += 200;
    } v_elseif (3 < b) {
        b += 300;
    } v_elseif (4 <= b) {
        b += 400;
    } v_elseif (5 > b) {
        b += 500;
    } v_elseif (6 >= b) {
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

    d = lut(d, a, b, c);
    dst_reg[1] = lut_sign(d, a, b, c);
}

void test_lut2a()
{
    vFloat r, in;
    vUInt l0, l1, l2;
    in = 1.0f;
    l0 = 1;
    l1 = 2;
    l2 = 3;

    r = lut2(in, l0, l1, l2);
    dst_reg[0] = r;
    r = lut2_sign(in, l0, l1, l2);
    dst_reg[1] = r;
}

void test_lut2b()
{
    vFloat r, in;
    vFloat l0, l1, l2, l3, l4, l5;
    in = 1.0f;
    l0 = 1;
    l1 = 2;
    l2 = 3;
    l3 = 4;
    l4 = 5;
    l5 = 6;

    r = lut2(in, l0, l1, l2, l3, l4, l5);
    dst_reg[0] = r;
    r = lut2_sign(in, l0, l1, l2, l3, l4, l5);
    dst_reg[1] = r;
}

void test_lut2c()
{
    vFloat r, in;
    vUInt l0, l1, l2, l3, l4, l5;
    in = 1.0f;
    l0 = 1;
    l1 = 2;
    l2 = 3;
    l3 = 4;
    l4 = 5;
    l5 = 6;

    r = lut2(in, l0, l1, l2, l3, l4, l5);
    dst_reg[0] = r;
    r = lut2(in, l0, l1, l2, l3, l4, l5, 0);
    dst_reg[1] = r;
    r = lut2(in, l0, l1, l2, l3, l4, l5, 8);
    dst_reg[2] = r;
    r = lut2_sign(in, l0, l1, l2, l3, l4, l5);
    dst_reg[3] = r;
    r = lut2_sign(in, l0, l1, l2, l3, l4, l5, 0);
    dst_reg[4] = r;
    r = lut2_sign(in, l0, l1, l2, l3, l4, l5, 8);
    dst_reg[5] = r;
}

void test_cast()
{
    vInt a = 1;
    vFloat b;
    b = int_to_float(a, 0);
    dst_reg[0] = b;
    b = int_to_float(a, 1);
    dst_reg[1] = b;

    b = int_to_float(a, 0xFFFFFF); // invalid value
    dst_reg[1] = b;
}

void test_stochrnd(int val)
{
    vInt a = 1;
    vFloat b;
    b = int_to_float(a, 0);
    b = int_to_float(a, 1);

    vFloat f = 1.0f;
    vInt di;
    vUInt du;

    du = float_to_fp16a(f);
    du = float_to_fp16a(f, 0);
    du = float_to_fp16a(f, 2); // invalid

    du = float_to_fp16b(f);
    du = float_to_fp16b(f, 0);
    du = float_to_fp16b(f, 2); // invalid

    du = float_to_uint8(f);
    du = float_to_uint8(f, 0);
    du = float_to_uint8(f, 2); // invalid

    du = float_to_int8(f);
    du = float_to_int8(f, 0);
    du = float_to_int8(f, 2); // invalid

    du = float_to_uint16(f);
    du = float_to_uint16(f, 0);
    du = float_to_uint16(f, 2); // invalid

    du = float_to_int16(f);
    du = float_to_int16(f, 0);
    du = float_to_int16(f, 2); // invalid

    vInt descale = 3;
    du = int32_to_uint8(a, descale, 0);
    du = int32_to_uint8(a, descale, 1);
    du = int32_to_uint8(a, descale, 2); // invalid

    du = int32_to_uint8(a, 3, 0);
    du = int32_to_uint8(a, val, 0);
    du = int32_to_uint8(a, 0xffffff, 1);
    du = int32_to_uint8(a, 0xffffff, 2); // invalid

    du = int32_to_int8(a, descale, 0);
    du = int32_to_int8(a, descale, 1);
    du = int32_to_int8(a, descale, 2); // invalid

    du = int32_to_int8(a, 3, 0);
    du = int32_to_int8(a, val, 0);
    du = int32_to_int8(a, 0xffffff, 1);
    du = int32_to_int8(a, 0xffffff, 2); // invalid
}

void subvec_shfl()
{
    vFloat x, y;
    x = 1.0f;

    y = subvec_shflror1(x);
    y = subvec_shflshr1(x);
}

void many_regs()
{
    vFloat x0 = dst_reg[0];
    vFloat x1 = dst_reg[1];
    vFloat x2 = dst_reg[2];
    vFloat x3 = dst_reg[3];
    vFloat x4 = dst_reg[4];
    vFloat x5 = dst_reg[5];
    vFloat x6 = dst_reg[6];
    vFloat x7 = dst_reg[7];

    dst_reg[0] = x0;
    dst_reg[1] = x1;
    dst_reg[2] = x2;
    dst_reg[3] = x3;
    dst_reg[4] = x4;
    dst_reg[5] = x5;
    dst_reg[6] = x6;
    dst_reg[7] = x7;
}

void lra()
{
    vFloat x = l_reg[1];
    vFloat v = l_reg[0];
    vInt a = l_reg[4];
    vFloat y = l_reg[2];
    vUInt b = l_reg[5];
    vInt z = l_reg[3];
    vUInt d = l_reg[7];
    vUInt c = l_reg[6];

    dst_reg [0] = b;
    dst_reg[0] = d;
    dst_reg[0] = c;
    dst_reg[0] = v;
    dst_reg[0] = x;
    dst_reg[0] = y;
    dst_reg [0] = z;
    dst_reg[0] = a;

    l_reg[0] = v;
    l_reg[1] = x;
    l_reg[2] = y;
    l_reg[3] = z;
    l_reg[4] = a;
    l_reg[5] = b;
    l_reg[6] = c;
    l_reg[7] = d;
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

void test_operator_equals()
{
    vFloat x, y, z;

    y = (x = 1.0F);
    dst_reg[0] = x;
    dst_reg[1] = y;

    y = (z = y);
    dst_reg[0] = x;
    dst_reg[1] = y;
    dst_reg[2] = z;

    x = (y *= x);
    dst_reg[0] = x;
    dst_reg[1] = y;
    dst_reg[2] = z;

    z = (x += y);
    dst_reg[0] = x;
    dst_reg[1] = y;
    dst_reg[2] = z;

    z = (x -= y);
    dst_reg[0] = x;
    dst_reg[1] = y;
    dst_reg[2] = z;

    {
        vInt a, b;
        a = (b = 1);
        dst_reg[0] = a;
        dst_reg[0] = b;
        a = (b += 1);
        dst_reg[0] = a;
        dst_reg[0] = b;
        a = (b |= 1);
        dst_reg[0] = a;
        dst_reg[0] = b;
        a = (b &= 1);
        dst_reg[0] = a;
        dst_reg[0] = b;
        a = (b ^= 1);
        dst_reg[0] = a;
        dst_reg[0] = b;
        a = (b <<= 1);
        dst_reg[0] = a;
        dst_reg[0] = b;
    }
    {
        vUInt a, b;
        a = (b = 1);
        dst_reg[0] = a;
        dst_reg[0] = b;
        a = (b += 1);
        dst_reg[0] = a;
        dst_reg[0] = b;
        a = (b |= 1);
        dst_reg[0] = a;
        dst_reg[0] = b;
        a = (b &= 1);
        dst_reg[0] = a;
        dst_reg[0] = b;
        a = (b ^= 1);
        dst_reg[0] = a;
        dst_reg[0] = b;
        a = (b >>= 1);
        dst_reg[0] = a;
        dst_reg[0] = b;
        a = (b <<= 1);
        dst_reg[0] = a;
        dst_reg[0] = b;
    }
}

int main(int argc, char* argv[])
{
    test_load_store();
    test_add();
    test_sub();
    test_mul();
    test_mad();
    test_incdec();
    test_loadi(10, 20);
    test_control_flow(5);
    test_mad_imm();
    test_setman_setexp_addexp_setsgn(argc);
    test_dreg_conditional_const();
    test_vhalf_conditional_const();
    test_vhalf_conditional();
    test_creg_conditional();
    test_creg_conditional_rev();
    test_conditional_outside_vif();
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
    test_lut2a();
    test_lut2b();
    test_lut2c();
    many_regs();
    subvec_shfl();
    test_stochrnd(argc);
    stupid_example(argc);
    test_operator_equals();
}
