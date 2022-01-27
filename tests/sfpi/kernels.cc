#include <cstdio>
#include "test.h"

using namespace sfpi;

#ifdef COMPILE_FOR_EMULE
// Just need this one ckernel_ops macro replacement for testing purposes...
#define TTI_SFPLOADI(lreg_ind, instr_mod0, imm16) sfpu::sfpu_lreg[lreg_ind] = sfpu_rvtt_sfploadi(instr_mod0, imm16)

#else

#include <ckernel_ops.h>

#endif

// Fake out semaphores
#undef TTI_SEMINIT
#undef TTI_SEMWAIT
#define TTI_SEMINIT(x, y, z)
#define TTI_SEMWAIT(x, y, z)
#define semaphore_post(x)

// Make this global, may keep compiler from optimizing it away
int param_global;


inline void copy_result_to_dreg0(int addr)
{
    // NOP for test
}

// Test infrastructure is set up to test float values, not ints
// Viewing the ints as floats leads to a mess (eg, denorms)
// Instead, compare in the kernel to the expected result and write a sentinal
// value for "pass" and the VecShort v value for "fail"
// Assumes this code is called in an "inner" if
sfpi_inline void set_expected_result(int addr, float sentinel, int expected, VecShort v)
{
    // Poor man's equals
    // Careful, the register is 19 bits and the immediate is sign extended 12
    // bits so comparing bit patterns w/ the MSB set won't work
    p_if (v >= expected && v < expected + 1) {
        dst_reg[addr] = sentinel;
    } p_else {
        dst_reg[addr] = v;
    }
    p_endif;
}

sfpi_inline VecCond test_interleaved_scalar_vector_cond(bool scalar_bool, VecHalf vec, float a, float b)
{
    if (scalar_bool) {
        return vec == a;
    } else {
        return vec == b;
    }
}

void test1()
{
    // Test SFPLOAD, SFPSTORE
    dst_reg[1] = dst_reg[0];

    // Out: ramp from 0..63
    copy_result_to_dreg0(1);
}

void test2()
{
    // Test SFPMOV
    dst_reg[2] = -dst_reg[0];

    // Out: ramp down from 0 to -63
    copy_result_to_dreg0(2);
}

void test3()
{
    // Test SFPENCC, SFPSETCC, SFPCOMPC, LOADI
    // Also, load after store (NOP)
    p_if(dst_reg[0] == 0.0F) {
        dst_reg[3] = 10.0F;
    } p_else {
        dst_reg[3] = 20.0F;
    }
    p_endif;

    p_if(dst_reg[0] == 2.0F) {
        VecHalf a = 30.0F;
        dst_reg[3] = a;
    }
    p_endif;

    p_if(dst_reg[0] == 3.0F) {
        VecShort a = 0x3F80;
        dst_reg[3] = a;
    }
    p_endif;

    p_if(dst_reg[0] == 4.0F) {
        VecUShort a = 0x3F80;
        dst_reg[3] = a;
    }
    p_endif;

    p_if(dst_reg[0] == 5.0F) {
        VecUShort a = 0xFFFF;
        dst_reg[3] = a;
    }
    p_endif;

    p_if(dst_reg[0] == 62.0F) {
        // Store into [62] so the compared value isn't close to the expected value
        dst_reg[3] = CReg_0p0020;
    }
    p_endif;

    p_if(dst_reg[0] == 6.0F) {
        VecHalf a = 120.0F;
        dst_reg[3] = a;
    }
    p_endif;

    // [0] = 10.0
    // [1] = 20.0
    // [2] = 30.0
    // [3] = 1.0 on emule, 0x403f on grayskull versim
    // [4] = 1.0 on emule, 0x403f on grayskull versim
    // [5] = 0xFFFF
    // [6] = 120.0F
    // [7] on 20.0F
    // except [62] = .001953

    copy_result_to_dreg0(3);
}

void test4()
{
    // Test SFPPUSHCC, SFPPOPCC, SFPMAD (in conditionals)
    // Test vector loads
    // Operators &&, ||, !

    VecHalf v = dst_reg[0];

    dst_reg[4] = v;

    p_if(v < 2.0F) {
        dst_reg[4] = 64.0F;
    }
    p_endif;
    // [0,1] = 64.0

    p_if(v < 6.0F) {
        p_if(v >= 2.0F) {
            p_if(v >= 3.0F) {
                dst_reg[4] = 65.0F;
            } p_else {
                dst_reg[4] = 66.0F;
            }
            p_endif;

            p_if(v == 5.0F) {
                dst_reg[4] = 67.0F;
            }
            p_endif;
        }
        p_endif;
    }
    p_endif;
    // [2] = 66.0
    // [3, 4] = 65.0
    // [5] = 67.0

    p_if(v >= 6.0F) {
        p_if(v < 9.0F) {
            p_if(v == 6.0F) {
                dst_reg[4] = 68.0F;
            } p_elseif(v != 8.0F) {
                dst_reg[4] = 69.0F;
            } p_else {
                dst_reg[4] = 70.0F;
            }
            p_endif;
        } p_elseif(v == 9.0F) {
            dst_reg[4] = 71.0F;
        } p_elseif(v == 10.0F) {
            dst_reg[4] = 72.0F;
        }

        p_endif;
    }
    p_endif;

    p_if(v >= 11.0F) {
        p_if(v < 18.0F && v >= 12.0F && v != 15.0F) {
            dst_reg[4] = 120.0F;
        } p_else {
            dst_reg[4] = -dst_reg[0];
        }
        p_endif;
    }
    p_endif;

    p_if(v >= 18.0F && v < 23.0F) {
        p_if(v == 19.0F || v == 21.0F) {
            dst_reg[4] = 160.0F;
        } p_else {
            dst_reg[4] = 180.0F;
        }
        p_endif;
    }
    p_endif;

    // Test ! on OP
    p_if(!(v != 23.0F)) {
        dst_reg[4] = 200.0F;
    }
    p_endif;

    p_if(!(v >= 25.0F) && !(v < 24.0F)) {
        dst_reg[4] = 220.0F;
    }
    p_endif;

    // Test ! on Boolean
    p_if(!((v < 25.0F) || (v >= 26.0F))) {
        dst_reg[4] = 240.0F;
    }
    p_endif;

    p_if((v >= 26.0F) && (v < 29.0F)) {
        dst_reg[4] = 260.0F;
        p_if(!((v >= 27.0F) && (v < 28.0F))) {
            dst_reg[4] = 270.0F;
        }
        p_endif;
    }
    p_endif;

    p_if (v == 29.0F || v == 30.0F || v == 31.0F) {
        VecHalf x = 30.0F;
        VecHalf y = 280.0F;
        p_if (v < x) {
            y += 10.0F;
        }
        p_endif;
        p_if (v == x) {
            y += 20.0F;
        }
        p_endif;
        p_if (v >= x) {
            y += 40.0F;
        }
        p_endif;
        dst_reg[4] = y;
    }
    p_endif;

    // [7] = 69.0
    // [8] = 70.0
    // [9] = 71.0
    // [10] = 72.0
    // [11] = -11.0
    // [12] = 120.0
    // [13] = 120.0
    // [14] = 120.0
    // [15] = -15.0
    // [16] = 120.0
    // [17] = 120.0
    // [18] = 180.0
    // [19] = 160.0
    // [20] = 180.0
    // [21] = 160.0
    // [22] = 180.0
    // [23] = 200.0
    // [24] = 220.0
    // [25] = 240.0
    // [26] = 270.0
    // [27] = 260.0
    // [28] = 270.0
    // [29] = 290.0
    // [30] = 340.0
    // [31] = 320.0

    // Remainder is -ramp
    copy_result_to_dreg0(4);
}

void test5()
{
    // Test SFPMAD, SFPMOV, CRegs

    p_if(dst_reg[0] == 0.0F) {
        dst_reg[5] = CReg_0 * CReg_0 + CReg_0p6929;
    } p_elseif(dst_reg[0] == 1.0F) {
        dst_reg[5] = CReg_0 * CReg_0 + CReg_0;
    } p_elseif(dst_reg[0] == 2.0F) {
        dst_reg[5] = CReg_0 * CReg_0 + CReg_Neg_1p0068;
    } p_elseif(dst_reg[0] == 3.0F) {
        dst_reg[5] = CReg_0 * CReg_0 + CReg_1p4424;
    } p_elseif(dst_reg[0] == 4.0F) {
        dst_reg[5] = CReg_0 * CReg_0 + CReg_0p8369;
    } p_elseif(dst_reg[0] == 5.0F) {
        dst_reg[5] = CReg_0 * CReg_0 + CReg_Neg_0p5;
    } p_elseif(dst_reg[0] == 6.0F) {
        dst_reg[5] = CReg_0 * CReg_0 + CReg_1;
    }
    p_endif;
    // [0] = 0.0
    // [1] = 0.692871094
    // [2] = -1.00683594
    // [3] = 1.442382813
    // [4] = 0.836914063
    // [5] = -0.5
    // [6] = 1.0

    p_if(dst_reg[0] == 7.0F) {
        dst_reg[5] = CReg_0 * CReg_0 + CReg_Neg_1;
    } p_elseif(dst_reg[0] == 8.0F) {
        dst_reg[5] = CReg_0 * CReg_0 + CReg_0p0020;
    } p_elseif(dst_reg[0] == 9.0F) {
        dst_reg[5] = CReg_0 * CReg_0 + CReg_Neg_0p6748;
    } p_elseif(dst_reg[0] == 10.0F) {
        dst_reg[5] = CReg_0 * CReg_0 + CReg_Neg_0p3447;
    } p_elseif(dst_reg[0] >= 11.0F) { // NOTE THIS IS >= TO FILL REMAINDER W/ TILE ID
        // Bogus test - low bits get lopped off by treating the int as a float
        dst_reg[5] = CReg_0 * CReg_0 + CReg_TileId;
    }
    p_endif;
    // [7] = -1.0
    // [8] = 0.001953125
    // [9] = -0.67480469
    // [10] = -0.34472656
    // [11] = TileId = 11

    VecHalf a = dst_reg[0];
    VecHalf b = 20.0F;

    // Note: loading dst_reg[0] takes a reg and comparing against a float const
    // takes a reg so can't store A, B and C across the condtionals

    p_if(dst_reg[0] == 12.0F) {
        dst_reg[5] = a * b;
    } p_elseif(dst_reg[0] == 13.0F) {
        dst_reg[5] = a + b;
    } p_elseif(dst_reg[0] == 14.0F) {
        dst_reg[5] = a * b + 0.5F;
    } p_elseif(dst_reg[0] == 15.0F) {
        dst_reg[5] = a + b + 0.5F;
    } p_elseif(dst_reg[0] == 16.0F) {
        dst_reg[5] = a * b - 0.5F;
    } p_elseif(dst_reg[0] == 17.0F) {
        dst_reg[5] = a + b - 0.5F;
    } p_elseif(dst_reg[0] == 18.0F) {
        VecHalf c = -5.0F;
        dst_reg[5] = a * b + c;
    }
    p_endif;
    // [12] = 240.0
    // [13] = 33.0
    // [14] = 280.5
    // [15] = 35.5
    // [16] = 319.5
    // [17] = 36.5
    // [18] = 355.0

    p_if(dst_reg[0] == 19.0F) {
        VecHalf c = -5.0F;
        dst_reg[5] = a * b + c + 0.5F;
    } p_elseif(dst_reg[0] == 20.0F) {
        VecHalf c = -5.0F;
        dst_reg[5] = a * b + c - 0.5F;
    } p_elseif(dst_reg[0] == 21.0F) {
        VecHalf c = -5.0F;
        VecHalf d;
        d = a * b + c - 0.5F;
        dst_reg[5] = d;
    } p_elseif(dst_reg[0] == 22.0F) {
        VecHalf c = -5.0F;
        dst_reg[5] = a * b - c;
    } p_elseif(dst_reg[0] == 23.0F) {
        dst_reg[5] = a * b + CReg_Neg_1;
    } p_elseif(dst_reg[0] == 24.0F) {
        dst_reg[5] = CReg_Neg_1 * b + CReg_Neg_1;
    }
    p_endif;
    // [19] = 375.5
    // [20] = 394.5
    // [21] = 414.5
    // [22] = 445.0
    // [23] = 459.0
    // [24] = -21.0

    p_if(dst_reg[0] == 25.0F) {
        VecHalf c = -5.0F;
        dst_reg[5] = dst_reg[0] * b + c;
    } p_elseif(dst_reg[0] == 26.0F) {
        VecHalf c = -5.0F;
        dst_reg[5] = b * dst_reg[0] + c;
    } p_elseif(dst_reg[0] == 27.0F) {
        dst_reg[5] = a * b + dst_reg[0];
    } p_elseif(dst_reg[0] == 28.0F) {
        dst_reg[5] = a * b - dst_reg[0];
    }
    p_endif;
    // [25] = 495.0
    // [26] = 515.0
    // [27] = 567.0
    // [28] = 532.0

    p_if(dst_reg[0] == 29.0F) {
        dst_reg[5] = a - b;
    } p_elseif(dst_reg[0] == 30.0F) {
        dst_reg[5] = a - b - 0.5F;
    } p_elseif(dst_reg[0] == 31.0F) {
        dst_reg[5] = dst_reg[0] - b + 0.5F;
    }
    p_endif;
    // [29] = 9.0
    // [30] = 9.5
    // [31] = 11.5
    // [32+] = TileID

    copy_result_to_dreg0(5);
}

void test6()
{
    // Note: set_expected_result uses SFPIADD so can't really be used early in
    // this routine w/o confusing things

    // SFPIADD

    dst_reg[6] = -dst_reg[0];

    p_if(dst_reg[0] < 3.0F) {
        p_if(dst_reg[0] >= 0.0F) {

            dst_reg[6] = 256.0F;

            VecShort a;
            p_if(dst_reg[0] == 0.0F) {
                a = 28;
            } p_elseif(dst_reg[0] == 1.0F) {
                a = 29;
            } p_elseif(dst_reg[0] == 2.0F) {
                a = 30;
            }
            p_endif;

            VecShort b;
            // IADD imm
            b = a - 29;
            p_if(b >= 0) {
                dst_reg[6] = 1024.0F;
            }
            p_endif;
        }
        p_endif;
    }
    p_endif;

    p_if(dst_reg[0] < 6.0F) {
        p_if(dst_reg[0] >= 3.0F) {
            dst_reg[6] = 256.0F;

            VecShort a;
            p_if(dst_reg[0] == 3.0F) {
                a = 28;
            } p_elseif(dst_reg[0] == 4.0F) {
                a = 29;
            } p_elseif(dst_reg[0] == 5.0F) {
                a = 30;
            }
            p_endif;

            VecShort b = -29;
            // IADD reg
            b = a + b;
            p_if(b < 0) {
                dst_reg[6] = 1024.0F;
            }
            p_endif;
        }
        p_endif;
    }
    p_endif;

    p_if(dst_reg[0] < 9.0F) {
        p_if(dst_reg[0] >= 6.0F) {
            dst_reg[6] = 16.0F;

            VecShort a = 3;
            p_if(dst_reg[0] == 6.0F) {
                a = 28;
            } p_elseif(dst_reg[0] == 7.0F) {
                a = 29;
            } p_elseif(dst_reg[0] == 8.0F) {
                a = 30;
            }
            p_endif;

            VecHalf b = 128.0F;
            p_if(a >= 29) {
                b = 256.0F;
            }
            p_endif;

            p_if(a < 29) {
                b = 512.0F;
            } p_elseif(a >= 30) {
                b = 1024.0F;
            }
            p_endif;

            dst_reg[6] = b;
        }
        p_endif;
    }
    p_endif;

    p_if(dst_reg[0] < 12.0F) {
        p_if(dst_reg[0] >= 9.0F) {
            dst_reg[6] = 16.0F;

            VecShort a = 3;
            p_if(dst_reg[0] == 9.0F) {
                a = 28;
            } p_elseif(dst_reg[0] == 10.0F) {
                a = 29;
            } p_elseif(dst_reg[0] == 11.0F) {
                a = 30;
            }
            p_endif;

            VecHalf b = 128.0F;
            VecShort c = 29;
            p_if(a >= c) {
                b = 256.0F;
            }
            p_endif;

            p_if(a < c) {
                b = 512.0F;
            } p_elseif(a >= 30) {
                b = 1024.0F;
            }
            p_endif;

            dst_reg[6] = b;
        }
        p_endif;
    }
    p_endif;

    p_if (dst_reg[0] == 12.0F) {
        VecShort v = 25;
        set_expected_result(6, 4.0F, 25, v);
    } p_elseif(dst_reg[0] == 13.0F) {
        VecShort a = 20;
        a = a + 12;
        set_expected_result(6, 8.0F, 32, a);
    } p_elseif(dst_reg[0] == 14.0F) {
        VecShort a = 18;
        VecShort b = -6;
        a = a + b;
        set_expected_result(6, 16.0F, 12, a);
    } p_elseif(dst_reg[0] == 15.0F) {
        VecShort a = 14;
        VecShort b = -5;
        a = b + a;
        set_expected_result(6, 32.0F, 9, a);
    }
    p_endif;

    p_if (dst_reg[0] == 16.0F) {
        VecShort v = 25;
        set_expected_result(6, 4.0F, 25, v);
    } p_elseif(dst_reg[0] == 17.0F) {
        VecShort a = 20;
        a = a - 12;
        set_expected_result(6, 8.0F, 8, a);
    } p_elseif(dst_reg[0] == 18.0F) {
        VecShort a = 18;
        VecShort b = 6;
        a = a - b;
        set_expected_result(6, 16.0F, 12, a);
    } p_elseif(dst_reg[0] == 19.0F) {
        VecShort a = 14;
        VecShort b = 5;
        a = b - a;
        set_expected_result(6, 32.0F, -9, a);
    }
    p_endif;

    p_if (dst_reg[0] == 20.0F) {
        VecUShort v = 25;
        set_expected_result(6, 4.0F, 25, reinterpret<VecShort>(v));
    } p_elseif(dst_reg[0] == 21.0F) {
        VecUShort a = 20;
        a = a - 12;
        set_expected_result(6, 8.0F, 8, reinterpret<VecShort>(a));
    } p_elseif(dst_reg[0] == 22.0F) {
        VecUShort a = 18;
        VecUShort b = 6;
        a = a - b;
        set_expected_result(6, 16.0F, 12, reinterpret<VecShort>(a));
    } p_elseif(dst_reg[0] == 23.0F) {
        VecUShort a = 14;
        VecUShort b = 5;
        a = b - a;
        set_expected_result(6, 32.0F, -9, reinterpret<VecShort>(a));
    }
    p_endif;

    p_if (dst_reg[0] == 24.0F) {
        VecShort a = 10;
        VecShort b = 20;
        a -= b;
        set_expected_result(6, 64.0F, -10, a);
    } p_elseif (dst_reg[0] == 25.0F) {
        VecShort a = 10;
        VecShort b = 20;
        a += b;
        set_expected_result(6, 128.0F, 30, a);
    }
    p_endif;

    // Pseudo-16 bit via hidden loadi
    p_if (dst_reg[0] == 26.0F) {
        VecShort a = 10;
        a += 4096;
        set_expected_result(6, 256.0F, 4106, a);
    } p_elseif (dst_reg[0] == 27.0F) {
        VecShort a = 4096;
        p_if (a >= 4096) {
            dst_reg[6] = 512.0f;
        } p_else {
            dst_reg[6] = 0.0f;
        }
        p_endif;
    }
    p_endif;

    // [0] = 256.0
    // [1] = 1024.0
    // [2] = 1024.0
    // [3] = 1024.0
    // [4] = 256.0
    // [5] = 256.0
    // [6] = 512.0
    // [7] = 256.0
    // [8] = 1024.0
    // [9] = 512.0
    // [10] = 256.0
    // [11] = 1024.0
    // [12] = 4.0
    // [13] = 8.0
    // [14] = 16.0
    // [15] = 32.0
    // [16] = 4.0
    // [17] = 8.0
    // [18] = 16.0
    // [19] = 32.0
    // [20] = 4.0
    // [21] = 8.0
    // [22] = 16.0
    // [23] = 32.0
    // [24] = 64.0
    // [25] = 128.0
    // [26] = 256.0
    // [27] = 512.0

    copy_result_to_dreg0(6);
}

void test7()
{
    // SFPEXMAN, SFPEXEXP, SFPSETEXP, SFPSETMAN
    dst_reg[7] = -dst_reg[0];
    p_if(dst_reg[0] == 1.0F) {
        VecHalf tmp = 124.0F;
        set_expected_result(7, 30.0F, 0x7C0, exman8(tmp));
    } p_elseif(dst_reg[0] == 2.0F) {
        VecHalf tmp = 124.0F;
        set_expected_result(7, 32.0F, 0x3C0, exman9(tmp));
    } p_elseif(dst_reg[0] == 3.0F) {
        VecHalf tmp = 65536.0F * 256.0F;
        set_expected_result(7, 33.0F, 0x18, exexp(tmp));
    } p_elseif(dst_reg[0] == 4.0F) {
        VecHalf tmp = 65536.0F * 256.0F;
        set_expected_result(7, 34.0F, 0x97, exexp_nodebias(tmp));
    } p_elseif(dst_reg[0] < 8.0F) {
        VecHalf tmp;
        p_if(dst_reg[0] == 5.0F) {
            // Exp < 0 for 5.0
            tmp = 0.5F;
        } p_elseif(dst_reg[0] < 8.0F) {
            // Exp > 0 for 6.0, 7.0
            tmp = 512.0F;
        }
        p_endif;

        VecShort v;
        v = exexp(tmp);
        p_if(v < 0) {
            dst_reg[7] = 32.0F;
        } p_else {
            dst_reg[7] = 64.0F;
        }
        p_endif;

        p_if (dst_reg[0] == 7.0F) {
            // Exponent is 9, save it
            set_expected_result(7, 35.0F, 9, v);
        }
        p_endif;
        // [0] = 64.0
        // [1] = 30.0
        // [2] = 32.0
        // [3] = 33.0
        // [4] = 34.0
        // [5] = 32.0
        // [6] = 64.0
        // [7] = 35.0 (exponent(512) = 8)
    } p_elseif(dst_reg[0] == 8.0F) {
        VecHalf tmp = 1.0F;
        VecHalf v = setexp(tmp, 137);
        dst_reg[7] = v;
    } p_elseif(dst_reg[0] == 9.0F) {
        VecShort exp = 0x007F; // Exponent in low bits
        VecHalf sgn_man = -1664.0F; // 1024 + 512 + 128 or 1101
        sgn_man = setexp(sgn_man, exp);
        dst_reg[7] = sgn_man;
    }
    p_endif;

    // [8] = 1024.0
    // [9] = -1.625

    p_if(dst_reg[0] == 10.0F) {
        VecHalf tmp = 1024.0F;
        VecHalf b = setman(tmp, 0x3AB);
        dst_reg[7] = b;
    } p_elseif(dst_reg[0] == 11.0F) {
        VecHalf tmp = 1024.0F;
        VecShort man = 0xBBB;
        VecHalf tmp2 = setman(tmp, man);
        dst_reg[7] = tmp2;
    }
    p_endif;

    // [10] = 1960.0 (?)
    // [11] = 1024.0

    copy_result_to_dreg0(7);
}

void test8()
{
    // SFPAND, SFPOR, SFPNOT, SFPABS
    // Atypical usage of conditionals

    dst_reg[8] = -dst_reg[0];
    p_if(dst_reg[0] == 1.0F) {
        VecUShort a = 0x05FF;
        VecUShort b = 0x0AAA;
        b &= a;
        set_expected_result(8, 16.0F, 0x00AA, static_cast<VecShort>(b));
    } p_elseif(dst_reg[0] == 2.0F) {
        VecUShort a = 0x05FF;
        VecUShort b = 0x0AAA;
        VecUShort c = a & b;
        set_expected_result(8, 16.0F, 0x00AA, static_cast<VecShort>(c));
    } p_elseif(dst_reg[0] == 3.0F) {
        VecShort a = 0x05FF;
        VecShort b = 0x0AAA;
        b &= a;
        set_expected_result(8, 16.0F, 0x00AA, b);
    } p_elseif(dst_reg[0] == 4.0F) {
        VecShort a = 0x05FF;
        VecShort b = 0x0AAA;
        VecShort c = a & b;
        set_expected_result(8, 16.0F, 0x00AA, c);
    }
    p_endif;

    p_if(dst_reg[0] == 5.0F) {
        VecUShort a = 0x0111;
        VecUShort b = 0x0444;
        b |= a;
        set_expected_result(8, 20.0F, 0x0555, static_cast<VecShort>(b));
    } p_elseif(dst_reg[0] == 6.0F) {
        VecUShort a = 0x0111;
        VecUShort b = 0x0444;
        VecUShort c = b | a;
        set_expected_result(8, 20.0F, 0x0555, static_cast<VecShort>(c));
    } p_elseif(dst_reg[0] == 7.0F) {
        VecShort a = 0x0111;
        VecShort b = 0x0444;
        b |= a;
        set_expected_result(8, 20.0F, 0x0555, b);
    } p_elseif(dst_reg[0] == 8.0F) {
        VecShort a = 0x0111;
        VecShort b = 0x0444;
        VecShort c = b | a;
        set_expected_result(8, 20.0F, 0x0555, c);
    }
    p_endif;

    p_if(dst_reg[0] == 9.0F) {
        VecUShort a = 0x0AAA;
        a = ~a;
        a &= 0x0FFF; // Tricky since ~ flips upper bits that immediates can't access
        set_expected_result(8, 22.0F, 0x0555, static_cast<VecShort>(a));
    } p_elseif(dst_reg[0] == 10.0F) {
        VecHalf a = 100.0F;
        dst_reg[8] = abs(a);
    } p_elseif(dst_reg[0] == 11.0F) {
        VecHalf a = -100.0F;
        dst_reg[8] = abs(a);
    } p_elseif(dst_reg[0] == 12.0F) {
        VecShort a = 100;
        set_expected_result(8, 24.0F, 100, abs(a));
    } p_elseif(dst_reg[0] == 13.0F) {
        VecShort a = -100;
        set_expected_result(8, 26.0F, 100, abs(a));
    }
    p_endif;

    p_if (test_interleaved_scalar_vector_cond(true, dst_reg[0], 14.0F, 15.0F)) {
        dst_reg[8] = 32.0F;
    } p_elseif(test_interleaved_scalar_vector_cond(false, dst_reg[0], 14.0F, 15.0F)) {
        dst_reg[8] = 16.0F;
    }
    p_endif;

    VecHalf tmp = dst_reg[8];
    p_block {
        p_and(dst_reg[0] >= 16.0F);

        for (int x = 0; x < 4; x++) {
            p_and(dst_reg[0] < 20.0F - x);
            tmp += 16.0F;
        }
    }
    p_endblock;
    dst_reg[8] = tmp;

    // [0] = 0
    // [1] = 16.0
    // [2] = 16.0
    // [3] = 16.0
    // [4] = 16.0
    // [5] = 20.0
    // [6] = 20.0
    // [7] = 20.0
    // [8] = 20.0
    // [9] = 22.0
    // [10] = 100.0
    // [11] = 100.0
    // [12] = 24.0
    // [13] = 26.0
    // [14] = 32.0
    // [15] = 16.0
    // [16] = 48.0
    // [17] = 31.0
    // [18] = 14.0
    // [19] = -3.0
    copy_result_to_dreg0(8);
}

void test9()
{
    // SFPMULI, SFPADDI, SFPDIVP2, SFPLZ

    dst_reg[9] = -dst_reg[0];
    p_if(dst_reg[0] == 1.0F) {
        VecHalf a = 20.0F;
        dst_reg[9] = a * 30.0F;
    } p_elseif(dst_reg[0] == 2.0F) {
        VecHalf a = 20.0F;
        a *= 40.0F;
        dst_reg[9] = a;
    } p_elseif(dst_reg[0] == 3.0F) {
        VecHalf a = 20.0F;
        dst_reg[9] = a + 30.0F;
    } p_elseif(dst_reg[0] == 4.0F) {
        VecHalf a = 20.0F;
        a += 40.0F;
        dst_reg[9] = a;
    } p_elseif(dst_reg[0] == 5.0F) {
        VecHalf a = 16.0F;
        dst_reg[9] = addexp(a, 4);
    } p_elseif(dst_reg[0] == 6.0F) {
        VecHalf a = 256.0F;
        dst_reg[9] = addexp(a, -4);
    }
    p_endif;

    p_if(dst_reg[0] == 7.0F) {
        VecShort a = 0;
        VecShort b = lz(a);
        set_expected_result(9, 38.0F, 0x13, b);
    } p_elseif(dst_reg[0] == 8.0F) {
        VecShort a = 0xFFFF;
        VecShort b = lz(a);
        set_expected_result(9, 55.0F, 0x0, b);
    } p_elseif(dst_reg[0] == 9.0F) {
        VecUShort a = 0xFFFFU;
        VecShort b = lz(a);
        set_expected_result(9, 30.0F, 0x3, b);
    } p_elseif(dst_reg[0] < 13.0F) {
        VecHalf a = dst_reg[0] - 11.0F;
        VecUShort b;

        // Relies on if chain above...
        p_if(dst_reg[0] >= 7.0F) {
            b = reinterpret<VecUShort>(lz(a));
            p_if (b != 19) {
                dst_reg[9] = 60.0F;
            } p_else {
                dst_reg[9] = 40.0F;
            }
            p_endif;
        }
        p_endif;
    }
    p_endif;

    p_if(dst_reg[0] == 13.0F) {
        VecHalf x = 1.0F;

        x *= 2.0f;
        x *= -3.0f;
        x += 4.0f;
        x += -4.0f;

        dst_reg[9] = x;
    } p_elseif(dst_reg[0] == 14.0F) {
        // MULI/ADDI don't accept fp16a
        // Ensure this goes to MAD

        VecHalf x = 1.0F;

        x *= ScalarFP16a(2.0);
        x *= ScalarFP16a(-3.0);
        x += ScalarFP16a(4.0);
        x += ScalarFP16a(-4.0);

        dst_reg[9] = x;
    }
    p_endif;

    // [1] = 600.0
    // [2] = 800.0
    // [3] = 50.0
    // [4] = 60.0
    // [5] = 256.0
    // [6] = 16.0
    // [7] = 38.0
    // [8] = 55.0
    // [9] = 30.0
    // [10] = 60.0
    // [11] = 40.0
    // [12] = 60.0
    // [13] = -6.0
    // [14] = -6.0
    copy_result_to_dreg0(9);
}

void test10()
{
    // SFPSHFT, SFTSETSGN
    dst_reg[10] = -dst_reg[0];
    p_if(dst_reg[0] == 1.0F) {
        VecUShort a = 0x015;
        VecShort shift = 6;
        VecUShort b = shft(a, shift);
        // Could write better tests if we could return and test the int result
        set_expected_result(10, 20.0F, 0x0540, static_cast<VecShort>(b));
    } p_elseif(dst_reg[0] == 2.0F) {
        VecUShort a = 0x2AAA;
        VecUShort b = shft(a, -4);
        set_expected_result(10, 22.0F, 0x02AA, static_cast<VecShort>(b));
    } p_elseif(dst_reg[0] == 3.0F) {
        VecUShort a = 0xAAAAU;
        VecShort shift = -6;
        VecUShort b = shft(a, shift);
        set_expected_result(10, 24.0F, 0x02AA, static_cast<VecShort>(b));
    } p_elseif(dst_reg[0] == 4.0F) {
        VecUShort a = 0x005A;
        VecUShort b = shft(a, 4);
        set_expected_result(10, 26.0F, 0x05A0, static_cast<VecShort>(b));
    } p_elseif(dst_reg[0] == 5.0F) {
        VecShort a = 25;
        a = a + 5;
        a += 7;
        set_expected_result(10, 28.0F, 0x25, a);
    } p_elseif(dst_reg[0] == 6.0F) {
        VecShort a = 28;
        VecShort b = 100;
        a += b;
        set_expected_result(10, 30.0F, 0x80, a);
    }
    p_endif;

    p_if(dst_reg[0] == 7.0F) {
        VecHalf a = dst_reg[0];
        dst_reg[10] = setsgn(a, 1);
    } p_elseif(dst_reg[0] == 8.0F) {
        VecHalf a = dst_reg[0];
        VecHalf b = -128.0;
        VecHalf r = setsgn(b, a);

        dst_reg[10] = r;
    } p_elseif(dst_reg[0] == 9.0F) {
        VecHalf a = -256.0F;
        dst_reg[10] = setsgn(a, 0);
    } p_elseif(dst_reg[0] == 10.0F) {
        VecHalf a = dst_reg[0];
        a += 20.0f;
        VecHalf b = -512.0F;
        VecHalf r = setsgn(a, b);

        dst_reg[10] = r;
    }
    p_endif;

    // [1] = 20.0
    // [2] = 22.0
    // [3] = 24.0
    // [4] = 26.0
    // [5] = 28.0
    // [6] = 30.0
    // [7] = -7.0
    // [8] = 128.0
    // [9] = 256.0
    // [10] = -30.0
    copy_result_to_dreg0(10);
}

void test11()
{
    // SFPLUT, SFPLOADL<n>
    dst_reg[11] = -dst_reg[0];

    VecUShort l0a = 0xFF30; // Multiply by 0.0, add 0.125
    VecUShort l1a = 0X3020; // Multiply by 0.125, add 0.25
    p_if(dst_reg[0] == 1.0F) {
        // Use L0
        VecHalf h = -0.3F;
        VecUShort l2a = 0xA010; // Mulitply by -0.25, add 0.5
        h = lut_sign(h, l0a, l1a, l2a);
        dst_reg[11] = h;
    } p_elseif(dst_reg[0] == 2.0F) {
        // Use L0
        VecHalf h = -0.3F;
        VecUShort l2a = 0xA010; // Mulitply by -0.25, add 0.5
        h = lut(h, l0a, l1a, l2a);
        dst_reg[11] = h;
    } p_elseif(dst_reg[0] == 3.0F) {
        // Use L0
        VecHalf h = -0.3F;
        VecUShort l2a = 0xA010; // Mulitply by -0.25, add 0.5
        h = lut_sign(h, l0a, l1a, l2a, -1);
        dst_reg[11] = h;
    } p_elseif(dst_reg[0] == 4.0F) {
        // Use L0
        VecHalf h = -0.3F;
        VecUShort l2a = 0xA010; // Mulitply by -0.25, add 0.5
        h = lut(h, l0a, l1a, l2a, 1);
        dst_reg[11] = h;
    } p_elseif(dst_reg[0] == 5.0F) {
        // Use L1
        VecHalf h = 1.0F;
        VecUShort l2a = 0xA010; // Mulitply by -0.25, add 0.5
        h = lut(h, l0a, l1a, l2a, 1);
        dst_reg[11] = h;
    } p_elseif(dst_reg[0] == 6.0F) {
        // Use L2
        VecHalf h = 4.0F;
        VecUShort l2a = 0xA010; // Mulitply by -0.25, add 0.5
        h = lut_sign(h, l0a, l1a, l2a);
        dst_reg[11] = h;
    }
    p_endif;

    // Clear out the LUT, re-load it w/ ASM instructions, the pull it into
    // variables for the SFPLUT
    l0a = 0;
    l1a = 0;

    // These are fakedout w/ emule
    TTI_SFPLOADI(0, SFPLOADI_MOD0_USHORT, 0xFF20); // Mulitply by 0.0, add 0.25
    TTI_SFPLOADI(1, SFPLOADI_MOD0_USHORT, 0x2010); // Mulitply by 0.25, add 0.5
    VecUShort l0b, l1b;
    LReg lr0(LRegs::LReg0);
    LReg lr1(LRegs::LReg1);
    l0b = lr0;
    l1b = lr1;

    p_if(dst_reg[0] == 7.0F) {
        // Use L0
        VecHalf h = -0.3F;
        VecUShort l2b = 0x9000;
        h = lut_sign(h, l0b, l1b, l2b);
        dst_reg[11] = h;
    } p_elseif(dst_reg[0] == 8.0F) {
        // Use L0
        VecHalf h = -0.3F;
        VecUShort l2b = 0x9000;
        h = lut(h, l0b, l1b, l2b);
        dst_reg[11] = h;
    } p_elseif(dst_reg[0] == 9.0F) {
        // Use L0
        VecHalf h = -0.3F;
        VecUShort l2b = 0x9000;
        h = lut_sign(h, l0b, l1b, l2b, -1);
        dst_reg[11] = h;
    } p_elseif(dst_reg[0] == 10.0F) {
        // Use L0
        VecHalf h = -0.3F;
        VecUShort l2b = 0x9000;
        h = lut(h, l0b, l1b, l2b, 1);
        dst_reg[11] = h;
    } p_elseif(dst_reg[0] == 11.0F) {
        // Use L1
        VecHalf h = 1.0F;
        VecUShort l2b = 0x9000;
        h = lut(h, l0b, l1b, l2b, 1);
        dst_reg[11] = h;
    } p_elseif(dst_reg[0] == 12.0F) {
        // Use L2
        VecHalf h = 4.0F;
        VecUShort l2b = 0x9000;
        h = lut_sign(h, l0b, l1b, l2b);
        dst_reg[11] = h;
    }
    p_endif;

    // [1] = 0.125
    // [2] = -0.125
    // [3] = -0.375
    // [4] = -0.625
    // [5] = 0.875
    // [6] = -0.5
    // [7] = 0.25
    // [8] = -0.25
    // [9] = -0.25
    // [10] = -0.75
    // [11] = 1.25
    // [12] = -1.0
    copy_result_to_dreg0(11);
}

void test12(int imm)
{
    // imm is 35
    // Test immediate forms of SFPLOAD, SFPLOADI, SFPSTORE, SFPIADD_I, SFPADDI
    // SFPMULI, SFPSHFT, SFPDIVP2, SFPSETEXP, SFPSETMAN, SFPSETSGN,
    // Tries to cover both positive and negative params (sign extension)
    dst_reg[12] = -dst_reg[0];

    p_if(dst_reg[0] == 1.0F) {
        dst_reg[12] = static_cast<float>(imm); // SFPLOADI
    } p_elseif(dst_reg[0] == 2.0F) {
        dst_reg[12] = static_cast<float>(-imm); // SFPLOADI
    } p_elseif(dst_reg[0] == 3.0F) {
        VecShort a = 5;
        a += imm; // SFPIADD_I
        set_expected_result(12, 25.0F, 40, a);
    } p_elseif(dst_reg[0] == 4.0F) {
        VecShort a = 5;
        a -= imm; // SFPIADD_I
        set_expected_result(12, -25.0F, -30, a);
    } p_elseif(dst_reg[0] == 5.0F) {
        VecHalf a = 3.0F;
        a += static_cast<float>(imm); // SFPADDI
        dst_reg[12] = a;
    } p_elseif(dst_reg[0] == 6.0F) {
        VecHalf a = 3.0F;
        a += static_cast<float>(-imm); // SFPADDI
        dst_reg[12] = a;
    }
    p_endif;

    p_if(dst_reg[0] == 7.0F) {
        VecUShort a = 0x4000;
        a >>= imm - 25;
        set_expected_result(12, 64.0F, 0x0010, reinterpret<VecShort>(a));
    } p_elseif(dst_reg[0] == 8.0F) {
        VecUShort a = 1;
        a <<= imm - 25;
        set_expected_result(12, 128.0F, 0x0400, reinterpret<VecShort>(a));
    } p_elseif(dst_reg[0] == 9.0F) {
        VecHalf a = 256.0F;
        dst_reg[12] = addexp(a, imm - 31);
    } p_elseif(dst_reg[0] == 10.0F) {
        VecHalf a = 256.0F;
        dst_reg[12] = addexp(a, imm - 39);
    }
    p_endif;

    p_if(dst_reg[0] == 11.0F) {
        VecHalf a = 128.0;
        VecHalf r = setsgn(a, imm - 36);
        dst_reg[12] = r;
    } p_elseif(dst_reg[0] == 12.0F) {
        VecHalf tmp = 1024.0F;
        int man = 0xBBB + 35 - imm;
        VecHalf tmp2 = setman(tmp, man);
        dst_reg[12] = tmp2;
    } p_elseif(dst_reg[0] == 13.0F) {
        int exp = 0x007F + 35 - imm; // Exponent in low bits
        VecHalf sgn_man = -1664.0F; // 1024 + 512 + 128 or 1101
        sgn_man = setexp(sgn_man, exp);
        dst_reg[12] = sgn_man;
    }
    p_endif;

    dst_reg[30 + 35 - imm] = 30.0F; // SFPSTORE
    dst_reg[30 + 35 - imm + 1] = CReg_Neg_1;

    p_if(dst_reg[0] == 14.0F) {
        dst_reg[12] = dst_reg[30 + 35 - imm]; // SFPLOAD
    }
    p_endif;
    p_if(dst_reg[0] == 15.0F) {
        dst_reg[12] = dst_reg[30 + 35 - imm + 1]; // SFPLOAD
    }
    p_endif;

    // Test for store/load nops, imm store non-imm load
    // Need to use the semaphores to get TRISC to run ahead for non-imm loads

    p_if(dst_reg[0] == 16.0F) {
        // imm store, non-imm load
        VecHalf a = 120.0F;

        TTI_SEMINIT(1, 0, p_stall::SEMAPHORE_3);
        TTI_SEMWAIT(p_stall::STALL_MATH, p_stall::SEMAPHORE_3, p_stall::STALL_ON_ZERO);

        dst_reg[12] = a;
        __builtin_rvtt_sfpnop(); // XXXXXX remove me when compiler is fixed
        a = dst_reg[imm - 23];

        semaphore_post(3);

        dst_reg[12] = a + 1.0F;
    }
    p_endif;

    p_if(dst_reg[0] == 17.0F) {
        // non-imm store, imm load
        VecHalf a = 130.0F;
        dst_reg[imm - 23] = a;
        __builtin_rvtt_sfpnop(); // XXXXXX remove me when compiler is fixed
        a = dst_reg[12];
        dst_reg[12] = a + 1.0F;
    }
    p_endif;

    p_if(dst_reg[0] == 18.0F) {
        // non-imm store, non-imm load
        VecHalf a = 140.0F;

        TTI_SEMINIT(1, 0, p_stall::SEMAPHORE_3);
        TTI_SEMWAIT(p_stall::STALL_MATH, p_stall::SEMAPHORE_3, p_stall::STALL_ON_ZERO);

        dst_reg[imm - 23] = a;
        __builtin_rvtt_sfpnop(); // XXXXXX remove me when compiler is fixed
        a = dst_reg[imm - 23];

        semaphore_post(3);

        dst_reg[12] = a + 1.0F;
    }
    p_endif;

    p_if(dst_reg[0] == 19.0F) {
        VecHalf a = 3.0F;
        a *= static_cast<float>(imm); // SFPADDI
        dst_reg[12] = a;
    } p_elseif(dst_reg[0] == 20.0F) {
        VecHalf a = 3.0F;
        a *= static_cast<float>(-imm); // SFPADDI
        dst_reg[12] = a;
    }
    p_endif;

    // [1] = 35.0F
    // [2] = -35.0F
    // [3] = 25.0F
    // [4] = -25.0F
    // [5] = 38.0F
    // [6] = -32.0F
    // [7] = 64.0F
    // [8] = 128.0F
    // [9] = 4096.0F
    // [10] = 16.0F
    // [11] = -128.0F
    // [12] = 1976.0 // due to grayskull bug, other would be 1024.0F
    // [13] = -1.625
    // [14] = 30.0F
    // [15] = -1.0
    // [16] = 121.0F
    // [17] = 131.0F
    // [18] = 141.0F
    // [19] = 105.0F
    // [20] = -105.0F

    copy_result_to_dreg0(12);
}

// Test 13 covers variable liveness, ie, keeping a variable "alive" across a
// CC narrowing instruction.  Touches every affected instruction except LOAD,
// LOADI, IADD (those are covered in random tests above) across a SETCC
void test13(int imm)
{
    // Test variable liveness

    dst_reg[13] = -dst_reg[0];

    // ABS liveness across SETCC
    {
        VecHalf x = -20.0F;
        VecHalf y = -30.0F;
        p_if (dst_reg[0] == 0.0F) {
            y = abs(x);
        }
        p_endif;
        p_if (dst_reg[0] == 0.0F || dst_reg[0] == 1.0F) {
            dst_reg[13] = y;
        }
        p_endif;
    }
    // [0] = 20.0F
    // [1] = -30.0F

    // NOT liveness across SETCC
    {
        VecShort a = 0xFAAA;
        VecShort b = 0x07BB;
        p_if (dst_reg[0] == 2.0F) {
            b = ~a;
        }
        p_endif;
        p_if (dst_reg[0] == 2.0F || dst_reg[0] == 3.0F) {
            p_if (dst_reg[0] == 2.0F) {
                set_expected_result(13, 40.0F, 0x0555, b);
            }
            p_endif;
            p_if (dst_reg[0] == 3.0F) {
                set_expected_result(13, 50.0F, 0x07BB, b);
            }
            p_endif;
        }
        p_endif;
    }
    // [2] = 40.0F
    // [3] = 50.0F

    // LZ liveness across SETCC
    {
        VecShort a = 0x0080;
        VecShort b = 0x07BB;
        p_if (dst_reg[0] == 4.0F) {
            b = lz(a);
        }
        p_endif;
        p_if (dst_reg[0] == 4.0F || dst_reg[0] == 5.0F) {
            p_if (dst_reg[0] == 4.0F) {
                set_expected_result(13, 60.0F, 11, b);
            }
            p_endif;
            p_if (dst_reg[0] == 5.0F) {
                set_expected_result(13, 70.0F, 0x07BB, b);
            }
            p_endif;
        }
        p_endif;
    }
    // [4] = 60.0F
    // [5] = 70.0F

    // MAD liveness across SETCC
    {
        VecHalf a = 90.0F;
        VecHalf b = 110.0F;
        p_if (dst_reg[0] == 6.0F) {
            b = a * a + 10.0;
        }
        p_endif;
        p_if (dst_reg[0] == 6.0F || dst_reg[0] == 7.0F) {
            dst_reg[13] = b;
        }
        p_endif;
    }
    // [6] = 8096.0
    // [7] = 110.0F

    // MOV liveness across SETCC
    {
        VecHalf a = 120.0F;
        VecHalf b = 130.0F;
        p_if (dst_reg[0] == 8.0F) {
            b = -a;
        }
        p_endif;
        p_if (dst_reg[0] == 8.0F || dst_reg[0] == 9.0F) {
            dst_reg[13] = b;
        }
        p_endif;
    }
    // [8] = -120.0F
    // [9] = 130.0F;

    // DIVP2 liveness across SETCC
    {
        VecHalf a = 140.0F;
        VecHalf b = 150.0F;
        p_if (dst_reg[0] == 10.0F) {
            b = addexp(a, 1);
        }
        p_endif;
        p_if (dst_reg[0] == 10.0F || dst_reg[0] == 11.0F) {
            dst_reg[13] = b;
        }
        p_endif;
    }
    // [10] = 280.0F
    // [11] = 150.0F

    // EXEXP liveness across SETCC
    {
        VecHalf a = 160.0F;
        VecShort b = 128;
        p_if (dst_reg[0] == 12.0F) {
            b = exexp_nodebias(a);
        }
        p_endif;
        p_if (dst_reg[0] == 12.0F || dst_reg[0] == 13.0F) {
            VecHalf tmp = 1.0F;
            dst_reg[13] = setexp(tmp, b);
        }
        p_endif;
    }
    // [12] = 128.0F
    // [13] = 2.0F

    // EXMAN liveness across SETCC
    {
        VecHalf a = 160.0F;
        VecShort b = 128;
        p_if (dst_reg[0] == 14.0F) {
            b = exman8(a);
        }
        p_endif;
        p_if (dst_reg[0] == 14.0F || dst_reg[0] == 15.0F) {
            VecHalf tmp = 128.0F;
            b = b << 9;
            dst_reg[13] = setman(tmp, b);
        }
        p_endif;
    }
    // [14] = 160.0F
    // [15] = 144.0F

    // SETEXP_I liveness across SETCC
    {
        VecHalf a = 170.0F;
        VecHalf b = 180.0F;
        p_if (dst_reg[0] == 16.0F) {
            b = setexp(a, 132);
        }
        p_endif;
        p_if (dst_reg[0] == 16.0F || dst_reg[0] == 17.0F) {
            dst_reg[13] = b;
        }
        p_endif;
    }
    // [16] = 42.5F
    // [17] = 180.0F

    // SETMAN_I liveness across SETCC
    {
        VecHalf a = 190.0F;
        VecHalf b = 200.0F;
        p_if (dst_reg[0] == 18.0F) {
            b = setman(a, 0x3AB);
        }
        p_endif;
        p_if (dst_reg[0] == 18.0F || dst_reg[0] == 19.0F) {
            dst_reg[13] = b;
        }
        p_endif;
    }
    // [18] = 245.0F
    // [19] = 200.0F

    // SETSGN_I liveness across SETCC
    {
        VecHalf a = 210.0F;
        VecHalf b = 220.0F;
        p_if (dst_reg[0] == 20.0F) {
            b = setsgn(a, 1);
        }
        p_endif;
        p_if (dst_reg[0] == 20.0F || dst_reg[0] == 21.0F) {
            dst_reg[13] = b;
        }
        p_endif;
    }
    // [20] = -210.0F
    // [21] = 220.0F

    // nonimm_dst_src using DIVP2 liveness across SETCC
    {
        VecHalf a = 140.0F;
        VecHalf b = 150.0F;
        p_if (dst_reg[0] == 22.0F) {
            b = addexp(a, imm - 34);
        }
        p_endif;
        p_if (dst_reg[0] == 22.0F || dst_reg[0] == 23.0F) {
            dst_reg[13] = b;
        }
        p_endif;
    }
    // [22] = 280.0F
    // [23] = 150.0F

    // nonimm_dst using LOADI liveness across SETCC
    {
        VecHalf b = 240.0F;
        p_if (dst_reg[0] == 24.0F) {
            b = static_cast<float>(-imm);
        }
        p_endif;
        p_if (dst_reg[0] == 24.0F || dst_reg[0] == 25.0F) {
            dst_reg[13] = b;
        }
        p_endif;
    }
    // [24] = -35.0F
    // [25] = 240.0F

    copy_result_to_dreg0(13);
}

void test14(int imm)
{
    // Test13 tests various builtins for liveness across a SETCC
    // Below test MOV liveness across COMPC, LZ, EXEXP, IADD

    dst_reg[14] = -dst_reg[0];

    // MOV liveness across COMPC
    {
        VecHalf a = 250.0F;
        VecHalf b = 260.0F;
        p_if (dst_reg[0] != 0.0F) {
            b = 160.0F;
        } p_else {
            VecHalf c = CReg_0 * CReg_0 + CReg_0;
            b = -a;
            a = c;
        }
        p_endif;
        p_if (dst_reg[0] == 0.0F || dst_reg[0] == 1.0F) {
            dst_reg[14] = b;
        }
        p_endif;
    }
    // [0] = -250.0F
    // [1] = 160.0F;

    // MOV liveness across LZ
    {
        VecHalf a = 250.0F;
        VecHalf b = 260.0F;
        VecShort tmp;

        p_if (dst_reg[0] == 2.0F) {
            p_if (tmp.lz_cc(a, LzCCNE0)) {
                VecHalf c = CReg_0 * CReg_0 + CReg_0;
                b = -a;
                a = c;
            }
            p_endif;
        }
        p_endif;
        p_if (dst_reg[0] == 2.0F || dst_reg[0] == 3.0F) {
            dst_reg[14] = b;
        }
        p_endif;
    }
    // [2] = -250.0F;
    // [3] = 260.0F

    // MOV liveness across EXEXP
    {
        VecHalf a = 270.0F;
        VecHalf b = 280.0F;
        VecShort tmp;

        p_if (dst_reg[0] == 4.0F) {
            p_if (tmp.exexp_cc(a, ExExpCCGTE0)) {
                VecHalf c = CReg_0 * CReg_0 + CReg_0;
                b = -a;
                a = c;
            }
            p_endif;
        }
        p_endif;
        p_if (dst_reg[0] == 4.0F || dst_reg[0] == 5.0F) {
            dst_reg[14] = b;
        }
        p_endif;
    }
    // [4] = -270.0F;
    // [5] = 280.0F

    // Below 2 tests are incidentally covered by tests 1..12
    // MOV liveness across IADD
    {
        VecHalf b = 300.0F;
        VecShort tmp = 5;

        p_if (dst_reg[0] == 6.0F) {
            VecHalf a = 290.0F;
            p_if (tmp >= 2) {
                VecHalf c = CReg_0 * CReg_0 + CReg_0;
                b = -a;
                a = c;
            }
            p_endif;
        }
        p_endif;
        p_if (dst_reg[0] == 6.0F || dst_reg[0] == 7.0F) {
            dst_reg[14] = b;
        }
        p_endif;
    }
    // [6] = -290.0F
    // [7] = 300.0F

    // IADD_I liveness
    {
        VecShort a = 10;
        VecShort b = 20;
        p_if (dst_reg[0] == 8.0F) {
            b = a + 30;
        }
        p_endif;
        p_if (dst_reg[0] == 8.0F || dst_reg[0] == 9.0F) {
            p_if (dst_reg[0] == 8.0F) {
                set_expected_result(14, -310.0F, 40, b);
            }
            p_endif;
            p_if (dst_reg[0] == 9.0F) {
                set_expected_result(14, 320.0F, 20, b);
            }
            p_endif;
        }
        p_endif;
    }
    // [8] = -310.0F
    // [9] = 320.0F

    // Test various issues with move/assign. Unfortunately, compiler generated
    // moves are hard/impossible to induce and not all scenarios are testable
    // w/ explicit code afaict.  The case #s below come from the Predicated
    // Variable Liveness document and similar code exists in live.cc

    // Case 2a
    // Assignment resulting in register rename
    {
        VecHalf a = -20.0f;
        VecHalf b = 30.0f;
        p_if (dst_reg[0] == 10.0f) {
            b = a;
        }
        p_endif;

        p_if (dst_reg[0] == 10.0F || dst_reg[0] == 11.0F) {
            dst_reg[14] = b;
        }
        p_endif;
    }
    // [10] = -20.0
    // [11] = 30.0

    // Case 2b
    // Assignment requiring move
    // This straddles case 2a and 3 - both values need to be preserved but the
    // compiler doesn't know that, solving case2a will solve this case as well
    {
        VecHalf a = -40.0f;
        VecHalf b = 50.0f;
        p_if (dst_reg[0] == 12.0f) {
            b = a;
        }
        p_endif;

        p_if (dst_reg[0] == 100.0f) { // always fail
            dst_reg[14] = a;
        }
        p_endif;

        p_if (dst_reg[0] == 12.0F || dst_reg[0] == 13.0F) {
            dst_reg[14] = b;
        }
        p_endif;
    }
    // [12] = -40.0
    // [13] = 50.0

    // Case 3
    // Assignment requiring move (both a and b need to be preserved)
    {
        VecHalf a = -60.0f;
        VecHalf b = 70.0f;
        p_if (dst_reg[0] == 14.0f) {
            b = a;
        }
        p_endif;

        p_if (dst_reg[0] == 100.0f) { // always fail
            dst_reg[14] = a + 1.0f;
        }
        p_endif;

        p_if (dst_reg[0] == 14.0F || dst_reg[0] == 15.0F) {
            dst_reg[14] = b + 1.0f;
        }
        p_endif;
    }
    // [14] = -59.0
    // [15] = 71.0

    // Case 4a
    // Destination as source, 2 arguments in the wrong order
    // Confirm b is correct
    {
        VecShort a = 10;
        VecShort b = 20;
        p_if (dst_reg[0] == 16.0f) {
            b = b - a;
        }
        p_endif;

        p_if (dst_reg[0] == 100.0f) { // always fail
            dst_reg[14] = a;
        }
        p_endif;

        p_if (dst_reg[0] == 16.0F) {
            set_expected_result(14, -80.0F, 10, b);
        }
        p_endif;

        p_if (dst_reg[0] == 17.0F) {
            set_expected_result(14, 90.0F, 20, b);
        }
        p_endif;
    }
    // [16] = -80.0
    // [17] = 90.0

    // Case 4b
    // Destination as source, 2 arguments in the wrong order
    // Confirm a is correct
    {
        VecShort a = 10;
        VecShort b = 20;
        p_if (dst_reg[0] == 16.0f) {
            b = b - a;
        }
        p_endif;

        p_if (dst_reg[0] == 100.0f) { // always fail
            dst_reg[14] = b;
        }
        p_endif;

        p_if (dst_reg[0] == 18.0F) {
            set_expected_result(14, -90.0F, 10, a);
        }
        p_endif;

        p_if (dst_reg[0] == 19.0F) {
            set_expected_result(14, 100.0F, 10, a);
        }
        p_endif;
    }
    // [18] = -90.0
    // [19] = 100.0

    // Case 4c
    // Destination as source 3 arguments
    // Confirm c is correct
    {
        // Out of regs doing this the typical way
        VecHalf condition = dst_reg[0] - 20.0F;
        VecShort a = 10;
        VecShort b = 20;
        VecShort c = 30;

        p_if (condition == 0.0F) {
            c = a - b;
        }
        p_endif;

        p_if (CReg_0p8369 == dst_reg[0]) { // always fail
            dst_reg[14] = a;
            dst_reg[14] = b;
        }
        p_endif;

        p_if (dst_reg[0] == 20.0F) {
            set_expected_result(14, -100.0F, -10, c);
        }
        p_endif;

        p_if (dst_reg[0] == 21.0F) {
            set_expected_result(14, 110.0F, 30, c);
        }
        p_endif;
    }
    // [20] = -100.0
    // [21] = 110.0

    // Case 4c
    // Destination as source 3 arguments
    // Confirm a is correct
    {
        // Out of regs doing this the typical way
        VecHalf condition = dst_reg[0] - 22.0F;
        VecShort a = 10;
        VecShort b = 20;
        VecShort c = 30;

        p_if (condition == 0.0F) {
            c = a - b;
        }
        p_endif;

        p_if (CReg_0p8369 == dst_reg[0]) { // always fail
            dst_reg[14] = a;
            dst_reg[14] = c;
        }
        p_endif;

        p_if (dst_reg[0] == 22.0F) {
            set_expected_result(14, -110.0F, 10, a);
        }
        p_endif;

        p_if (dst_reg[0] == 23.0F) {
            set_expected_result(14, 120.0F, 10, a);
        }
        p_endif;
    }
    // [22] = -110.0
    // [23] = 120.0

    // Case 4c
    // Destination as source 3 arguments
    // Confirm b is correct
    {
        // Out of regs doing this the typical way
        VecHalf condition = dst_reg[0] - 24.0F;
        VecShort a = 10;
        VecShort b = 20;
        VecShort c = 30;

        p_if (condition == 0.0F) {
            c = a - b;
        }
        p_endif;

        p_if (CReg_0p8369 == dst_reg[0]) { // always fail
            dst_reg[14] = c;
            dst_reg[14] = b;
        }
        p_endif;

        p_if (dst_reg[0] == 24.0F) {
            set_expected_result(14, -120.0F, 20, b);
        }
        p_endif;

        p_if (dst_reg[0] == 25.0F) {
            set_expected_result(14, 130.0F, 20, b);
        }
        p_endif;
    }
    // [24] = -120.0
    // [25] = 130.0

    // The code below tests the case where we descend down a CC cascade, pop
    // back up, then back down w/ different CC bits set.  Does the variable
    // stay live when assigned at the same CC level but in a different
    // cascade, ie, across generations?
    {
        VecHalf a;
        VecHalf b;
        VecHalf dr = dst_reg[0];

        p_if (dr == 26.0F || dr == 27.0F) {
            b = -90.0F;
        }
        p_endif;

        p_if (dr == 26.0F) {
            a = 100.0F;
        }
        p_endif;

        p_if (dr == 27.0F) {
            a = 110.0F;
        }
        p_endif;

        p_if (dr == 27.0F) {
            b = a;
        }
        p_endif;

        p_if (dr == 26.0F || dr == 27.0F) {
            dst_reg[14] = b;
        }
        p_endif;

        p_if (dr == 500.0F) {
            dst_reg[14] = a;
        }
        p_endif;
    }
    // [26] = -90.0F
    // [27] = 110.0F;

    // Test a little basic block liveness madness
    {
        VecHalf a = 200.0F;
        VecHalf b = 1.0F;

        for (int i = 0; i < imm - 30; i++) { // 0..4
            p_if (dst_reg[0] == 28.0F) {
                switch (i) {
                case 0:
                    b = 2.0f;
                    break;
                case 1:
                    b = 4.0f;
                    break;
                case 2:
                    b = 8.0f;
                    break;
                default:
                    b = b * 4.0F;
                }
            } p_elseif (dst_reg[0] >= 30.0 - i) {
                if (i % 2 == 0) {
                    b = 10.0F;
                } else {
                    b = 20.0F;
                }
            }
            p_endif;

            a = a + a * b;
        }

        p_if (dst_reg[0] == 28.0F || dst_reg[0] == 29.0F) {
            dst_reg[14] = a;
        }
        p_endif;
    }
    // [28] = 200+200*2, 600+600*4, 3000+3000*8, 27000+27000*32, 89100+89100*128 =
    //        114939000.0F or 114819072.0F when rounded
    // [29] = 200+200*1, 400+400*20, 4400+4400*20, 92400+92400*10, 1016400+1016400*20 =
    //        21344400.0F or 21233664.0F when rounded

    copy_result_to_dreg0(14);
}

//////////////////////////////////////////////////////////////////////////////
//
// The test is designed to be incremental so that if, eg, a hang occurs it
// can be narrowed down (mostly) w/o modifying the code.  Rows contains how
// many rows in the destination register will be written, ie, how far the test
// will progress before terminating.  If the MSB is set, only that one row
// will be written.
//
// It is really hard to write tests w/o relying on lots of functionality...
//
// The primary purpose of this test is to test the SFPU instructions from the
// compiler, secondarily it tests the C++ wrapper around the builtins.
//
// Once SFPENC and SFPSETCC are confirmed (row 1), subsequent tests build on
// this to store results within a row
void test_kernel(unsigned int row, bool exact)
{
    if ((!exact && row >= 1) || row == 1) {
        printf("Testing 1\n");
        test1();
    }

    if ((!exact && row >= 2) || row == 2) {
        printf("Testing 2\n");
        test2();
    }

    if ((!exact && row >= 3) || row == 3) {
        printf("Testing 3\n");
        test3();
    }

    if ((!exact && row >= 4) || row == 4) {
        printf("Testing 4\n");
        test4();
    }

    if ((!exact && row >= 5) || row == 5) {
        printf("Testing 5\n");
        test5();
    }

    if ((!exact && row >= 6) || row == 6) {
        printf("Testing 6\n");
        test6();
    }

    if ((!exact && row >= 7) || row == 7) {
        printf("Testing 7\n");
        test7();
    }

    if ((!exact && row >= 8) || row == 8) {
        printf("Testing 8\n");
        test8();
    }

    if ((!exact && row >= 9) || row == 9) {
        printf("Testing 9\n");
        test9();
    }

    if ((!exact && row >= 10) || row == 10) {
        printf("Testing 10\n");
        test10();
    }

    if ((!exact && row >= 11) || row == 11) {
        printf("Testing 11\n");
        test11();
    }

    if ((!exact && row >= 12) || row == 12) {
        printf("Testing 12\n");
        test12(param_global);
    }

    if ((!exact && row >= 13) || row == 13) {
        printf("Testing 13\n");
        test13(param_global);
    }

    if ((!exact && row >= 14) || row == 14) {
        printf("Testing 14\n");
        test14(param_global);
    }
}

int main(int argc, char* argv[])
{
#ifdef COMPILE_FOR_EMULE
    using namespace sfpu;

    int row = (argc > 1) ? atoi(argv[1]) : 1;
    bool exact = false;
    if (row > 1000) {
        row -= 1000;
        exact = true;
    }

    if (row > 15) {
        fprintf(stderr, "Row too large\n");
        exit(-1);
    }
    bool printint = (argc > 2);

    param_global = 35;

    // Init ramp from 0..63
    for (int i = 0; i < 64; i++) {
        sfpu_dreg.store_float(static_cast<float>(i), 0, i);
    }

    __builtin_rvtt_sfpencc(3, SFPENCC_MOD1_EI_RI);
    test_kernel(row, exact);

    for (int j = 0; j <= row; j++) {
        if (!exact || j == row) {
            printf("Row: %d\n", j);
            for (int i = 0; i < 64; i++) {
                if (printint) {
                    printf("%04X ", sfpu_dreg.get(j * 4, i));
                } else {
                    printf("%4.6f ", sfpu_dreg.get_float(j * 4, i));
                }
            }
            printf("\n\n");
        }
    }
#endif
}
