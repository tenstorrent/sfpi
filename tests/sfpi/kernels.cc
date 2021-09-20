#include <stdio.h>
#include <stdlib.h>
#include "test.h"

using namespace sfpi;

#ifdef COMPILE_FOR_EMULE
// Just need this one ckernel_ops macro replacement for testing purposes...
#define TTI_SFPLOADI(lreg_ind, instr_mod0, imm16) sfpu::sfpu_lreg[lreg_ind] = sfpu_rvtt_sfploadi(instr_mod0, imm16)

#else

#include <ckernel_ops.h>

#endif

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
        dregs[addr] = sentinel;
    } p_else {
        dregs[addr] = v;
    }
    p_endif;
}

void test1()
{
    // Test SFPLOAD, SFPSTORE
    dregs[1] = dregs[0];

    // Out: ramp from 0..63
    copy_result_to_dreg0(1);
}

void test2()
{
    // Test SFPMOV
    dregs[2] = -dregs[0];

    // Out: ramp down from 0 to -63
    copy_result_to_dreg0(2);
}

void test3()
{
    // Test SFPENCC, SFPSETCC, SFPCOMPC, LOADI
    p_if(dregs[0] == 0.0F) {
        dregs[3] = 10.0F;
    } p_else {
        dregs[3] = 20.0F;
    }
    p_endif;

    p_if(dregs[0] == 2.0F) {
        VecHalf a = 30.0F;
        dregs[3] = a;
    }
    p_endif;

    p_if(dregs[0] == 3.0F) {
        VecShort a = 0x3F80;
        dregs[3] = a;
    }
    p_endif;

    p_if(dregs[0] == 4.0F) {
        VecUShort a = 0x3F80;
        dregs[3] = a;
    }
    p_endif;

    p_if(dregs[0] == 5.0F) {
        VecUShort a = 0xFFFF;
        dregs[3] = a;
    }
    p_endif;

    p_if(dregs[0] == 62.0F) {
        // Store into [62] so the compared value isn't close to the expected value
        dregs[3] = CReg_0p001953125;
    }
    p_endif;

    // [0] = 10.0
    // [1] = 20.0
    // [2] = 30.0
    // [3] = 1.0
    // [4] = 1.0
    // [5] = 0xFFFF
    // [6] on 20.0F
    // except [62] = .001953

    copy_result_to_dreg0(3);
}

void test4()
{
    // Test SFPPUSHCC, SFPPOPCC, SFPMAD (in conditionals)
    // Test vector loads
    // Operators &&, ||, !

    VecHalf v = dregs[0];

    dregs[4] = v;

    p_if(v < 2.0F) {
        dregs[4] = 64.0F;
    }
    p_endif;
    // [0,1] = 64.0

    p_if(v < 6.0F) {
        p_if(v >= 2.0F) {
            p_if(v >= 3.0F) {
                dregs[4] = 65.0F;
            } p_else {
                dregs[4] = 66.0F;
            }
            p_endif;

            p_if(v == 5.0F) {
                dregs[4] = 67.0F;
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
                dregs[4] = 68.0F;
            } p_elseif(v != 8.0F) {
                dregs[4] = 69.0F;
            } p_else {
                dregs[4] = 70.0F;
            }
            p_endif;
        } p_elseif(v == 9.0F) {
            dregs[4] = 71.0F;
        } p_elseif(v == 10.0F) {
            dregs[4] = 72.0F;
        }

        p_endif;
    }
    p_endif;

    p_if(v >= 11.0F) {
        p_if(v < 18.0F && v >= 12.0F && v != 15.0F) {
            dregs[4] = 120.0F;
        } p_else {
            dregs[4] = -dregs[0];
        }
        p_endif;
    }
    p_endif;

    p_if(v >= 18.0F && v < 23.0F) {
        p_if(v == 19.0F || v == 21.0F) {
            dregs[4] = 160.0F;
        } p_else {
            dregs[4] = 180.0F;
        }
        p_endif;
    }
    p_endif;

    // Test ! on OP
    p_if(!(v != 23.0F)) {
        dregs[4] = 200.0F;
    }
    p_endif;

    p_if(!(v >= 25.0F) && !(v < 24.0F)) {
        dregs[4] = 220.0F;
    }
    p_endif;

    // Test ! on Boolean
    p_if(!((v < 25.0F) || (v >= 26.0F))) {
        dregs[4] = 240.0F;
    }
    p_endif;

    p_if((v >= 26.0F) && (v < 29.0F)) {
        dregs[4] = 260.0F;
        p_if(!((v >= 27.0F) && (v < 28.0F))) {
            dregs[4] = 270.0F;
        }
        p_endif;
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
    // [23] = 200.0F
    // [24] = 220.0F
    // [25] = 240.0F
    // [26] = 270.0F
    // [27] = 260.0F
    // [28] = 270.0F

    // Remainder is -ramp
    copy_result_to_dreg0(4);
}

void test5()
{
    // Test SFPMAD, SFPMOV, CRegs

    p_if(dregs[0] == 0.0F) {
        dregs[5] = CReg_0 * CReg_0 + CReg_0p692871094;
    } p_elseif(dregs[0] == 1.0F) {
        dregs[5] = CReg_0 * CReg_0 + CReg_0;
    } p_elseif(dregs[0] == 2.0F) {
        dregs[5] = CReg_0 * CReg_0 + CReg_Neg_1p00683594;
    } p_elseif(dregs[0] == 3.0F) {
        dregs[5] = CReg_0 * CReg_0 + CReg_1p442382813;
    } p_elseif(dregs[0] == 4.0F) {
        dregs[5] = CReg_0 * CReg_0 + CReg_0p836914063;
    } p_elseif(dregs[0] == 5.0F) {
        dregs[5] = CReg_0 * CReg_0 + CReg_Neg_0p5;
    } p_elseif(dregs[0] == 6.0F) {
        dregs[5] = CReg_0 * CReg_0 + CReg_1;
    }
    p_endif;
    // [0] = 0.0
    // [1] = 0.692871094
    // [2] = -1.00683594
    // [3] = 1.442382813
    // [4] = 0.836914063
    // [5] = -0.5
    // [6] = 1.0
    p_if(dregs[0] == 7.0F) {
        dregs[5] = CReg_0 * CReg_0 + CReg_Neg_1;
    } p_elseif(dregs[0] == 8.0F) {
        dregs[5] = CReg_0 * CReg_0 + CReg_0p001953125;
    } p_elseif(dregs[0] == 9.0F) {
        dregs[5] = CReg_0 * CReg_0 + CReg_Neg_0p67480469;
    } p_elseif(dregs[0] == 10.0F) {
        dregs[5] = CReg_0 * CReg_0 + CReg_Neg_0p34472656;
    } p_elseif(dregs[0] >= 11.0F) { // NOTE THIS IS >= TO FILL REMAINDER W/ TILE ID
        // Bogus test - low bits get lopped off by treating the int as a float
        dregs[5] = CReg_0 * CReg_0 + CReg_TileId;
    }
    p_endif;
    // [7] = -1.0
    // [8] = 0.001953125
    // [9] = -0.67480469
    // [10] = -0.34472656
    // [11] = TileId = 11

    VecHalf a = dregs[0];
    VecHalf b = 20.0F;

    // Note: loading dregs[0] takes a reg and comparing against a float const
    // takes a reg so can't store A, B and C across the condtionals

    p_if(dregs[0] == 12.0F) {
        dregs[5] = a * b;
    } p_elseif(dregs[0] == 13.0F) {
        dregs[5] = a + b;
    } p_elseif(dregs[0] == 14.0F) {
        dregs[5] = a * b + kHalf;
    } p_elseif(dregs[0] == 15.0F) {
        dregs[5] = a + b + kHalf;
    } p_elseif(dregs[0] == 16.0F) {
        dregs[5] = a * b - kHalf;
    } p_elseif(dregs[0] == 17.0F) {
        dregs[5] = a + b - kHalf;
    } p_elseif(dregs[0] == 18.0F) {
        VecHalf c = -5.0F;
        dregs[5] = a * b + c;
    }
    p_endif;
    // [12] = 240.0
    // [13] = 33.0
    // [14] = 280.5
    // [15] = 35.5
    // [16] = 319.5
    // [17] = 36.5
    // [18] = 355.0

    p_if(dregs[0] == 19.0F) {
        VecHalf c = -5.0F;
        dregs[5] = a * b + c + kHalf;
    } p_elseif(dregs[0] == 20.0F) {
        VecHalf c = -5.0F;
        dregs[5] = a * b + c - kHalf;
    } p_elseif(dregs[0] == 21.0F) {
        VecHalf c = -5.0F;
        VecHalf d;
        d = a * b + c - kHalf;
        dregs[5] = d;
    } p_elseif(dregs[0] == 22.0F) {
        VecHalf c = -5.0F;
        dregs[5] = a * b - c;
    } p_elseif(dregs[0] == 23.0F) {
        dregs[5] = a * b + CReg_Neg_1;
    } p_elseif(dregs[0] == 24.0F) {
        dregs[5] = CReg_Neg_1 * b + CReg_Neg_1;
    }
    p_endif;
    // [19] = 375.5
    // [20] = 394.5
    // [21] = 414.5
    // [22] = 445.0
    // [23] = 459.0
    // [24] = -21.0

    p_if(dregs[0] == 25.0F) {
        VecHalf c = -5.0F;
        dregs[5] = dregs[0] * b + c;
    } p_elseif(dregs[0] == 26.0F) {
        VecHalf c = -5.0F;
        dregs[5] = b * dregs[0] + c;
    } p_elseif(dregs[0] == 27.0F) {
        dregs[5] = a * b + dregs[0];
    } p_elseif(dregs[0] == 28.0F) {
        dregs[5] = a * b - dregs[0];
    }
    p_endif;
    // [25] = 495.0
    // [26] = 515.0
    // [27] = 567.0
    // [28] = 532.0

    p_if(dregs[0] == 29.0F) {
        dregs[5] = a - b;
    } p_elseif(dregs[0] == 30.0F) {
        dregs[5] = a - b - kHalf;
    } p_elseif(dregs[0] == 31.0F) {
        dregs[5] = dregs[0] - b + kHalf;
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
    // Note: set_expected_result uses SFPIADD so can't really be used in this
    // routine w/o confusing things

    // SFPIADD

    dregs[6] = -dregs[0];

    p_if(dregs[0] < 3.0F) {
        p_if(dregs[0] >= 0.0F) {

            dregs[6] = 256.0F;

            VecShort a;
            p_if(dregs[0] == 0.0F) {
                a = 28;
            } p_elseif(dregs[0] == 1.0F) {
                a = 29;
            } p_elseif(dregs[0] == 2.0F) {
                a = 30;
            }
            p_endif;

            VecShort b;
            p_if(b.add_cc(a, -29, IAddCCGTE0)) {
                dregs[6] = 1024.0F;
            }
            p_endif;
        }
        p_endif;
    }
    p_endif;

    p_if(dregs[0] < 6.0F) {
        p_if(dregs[0] >= 3.0F) {
            dregs[6] = 256.0F;

            VecShort a;
            p_if(dregs[0] == 3.0F) {
                a = 28;
            } p_elseif(dregs[0] == 4.0F) {
                a = 29;
            } p_elseif(dregs[0] == 5.0F) {
                a = 30;
            }
            p_endif;

            VecShort b = -29;
            p_if(b.add_cc(a, IAddCCLT0)) {
                dregs[6] = 1024.0F;
            }
            p_endif;
        }
        p_endif;
    }
    p_endif;

    p_if(dregs[0] < 9.0F) {
        p_if(dregs[0] >= 6.0F) {
            dregs[6] = 16.0F;

            VecShort a = 3;
            p_if(dregs[0] == 6.0F) {
                a = 28;
            } p_elseif(dregs[0] == 7.0F) {
                a = 29;
            } p_elseif(dregs[0] == 8.0F) {
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

            dregs[6] = b;
        }
        p_endif;
    }
    p_endif;

    p_if(dregs[0] < 12.0F) {
        p_if(dregs[0] >= 9.0F) {
            dregs[6] = 16.0F;

            VecShort a = 3;
            p_if(dregs[0] == 9.0F) {
                a = 28;
            } p_elseif(dregs[0] == 10.0F) {
                a = 29;
            } p_elseif(dregs[0] == 11.0F) {
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

            dregs[6] = b;
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
    copy_result_to_dreg0(6);
}

void test7()
{
    // SFPEXMAN, SFPEXEXP, SFPSETEXP, SFPSETMAN
    dregs[7] = -dregs[0];
    p_if(dregs[0] == 1.0F) {
        VecHalf tmp = 124.0F;
        set_expected_result(7, 30.0F, 0x7C0, tmp.ex_man(ExManPad8));
    } p_elseif(dregs[0] == 2.0F) {
        VecHalf tmp = 124.0F;
        set_expected_result(7, 32.0F, 0x3C0, tmp.ex_man(ExManPad9));
    } p_elseif(dregs[0] == 3.0F) {
        VecHalf tmp = 65536.0F * 256.0F;
        set_expected_result(7, 33.0F, 0x18, tmp.ex_exp(ExExpDoDebias));
    } p_elseif(dregs[0] == 4.0F) {
        VecHalf tmp = 65536.0F * 256.0F;
        set_expected_result(7, 34.0F, 0x97, tmp.ex_exp(ExExpNoDebias));
    } p_elseif(dregs[0] < 8.0F) {
        VecHalf tmp;
        p_if(dregs[0] == 5.0F) {
            // Exp < 0 for 5.0
            tmp = 0.5F;
        } p_elseif(dregs[0] < 8.0F) {
            // Exp > 0 for 6.0, 7.0
            tmp = 512.0F;
        }
        p_endif;

        VecShort v;
        p_if(v.ex_exp_cc(tmp, ExExpDoDebias, ExExpCCLT0)) {
            dregs[7] = 32.0F;
        } p_else {
            dregs[7] = 64.0F;
        }            
        p_endif;

        p_if (dregs[0] == 7.0F) {
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
    } p_elseif(dregs[0] == 8.0F) {
        VecHalf tmp = 1.0F;
        VecHalf v = tmp.set_exp(137);
        dregs[7] = v;
    } p_elseif(dregs[0] == 9.0F) {
        VecShort exp = 0x007F; // Exponent in low bits
        VecHalf sgn_man = -1664.0F; // 1024 + 512 + 128 or 1101
        sgn_man = sgn_man.set_exp(exp);
        dregs[7] = sgn_man;
    }
    p_endif;

    // [8] = 1024.0
    // [9] = -1.625

    p_if(dregs[0] == 10.0F) {
        VecHalf tmp = 1024.0F;
        VecHalf b = tmp.set_man(0x3AB);
        dregs[7] = b;
    } p_elseif(dregs[0] == 11.0F) {
        VecHalf tmp = 1024.0F;
        VecShort man = 0xBBB;
        VecHalf tmp2 = tmp.set_man(man);
        dregs[7] = tmp2;
    }
    p_endif;

    // [10] = 1960.0 (?)
    // [11] = 1024.0

    copy_result_to_dreg0(7);
}

void test8()
{
    // SFPAND, SFPOR, SFPNOT, SFPABS

    dregs[8] = -dregs[0];
    po_if(dregs[0] == 1.0F) {
        VecUShort a = 0x05FF;
        VecUShort b = 0x0AAA;
        b &= a;
        set_expected_result(8, 16.0F, 0x00AA, static_cast<VecShort>(b));
    } p_elseif(dregs[0] == 2.0F) {
        VecUShort a = 0x05FF;
        VecUShort b = 0x0AAA;
        VecUShort c = a & b;
        set_expected_result(8, 16.0F, 0x00AA, static_cast<VecShort>(c));
    } p_elseif(dregs[0] == 3.0F) {
        VecShort a = 0x05FF;
        VecShort b = 0x0AAA;
        b &= a;
        set_expected_result(8, 16.0F, 0x00AA, b);
    } p_elseif(dregs[0] == 4.0F) {
        VecShort a = 0x05FF;
        VecShort b = 0x0AAA;
        VecShort c = a & b;
        set_expected_result(8, 16.0F, 0x00AA, c);
    }
    p_endif;

    po_if(dregs[0] == 5.0F) {
        VecUShort a = 0x0111;
        VecUShort b = 0x0444;
        b |= a;
        set_expected_result(8, 20.0F, 0x0555, static_cast<VecShort>(b));
    } p_elseif(dregs[0] == 6.0F) {
        VecUShort a = 0x0111;
        VecUShort b = 0x0444;
        VecUShort c = b | a;
        set_expected_result(8, 20.0F, 0x0555, static_cast<VecShort>(c));
    } p_elseif(dregs[0] == 7.0F) {
        VecShort a = 0x0111;
        VecShort b = 0x0444;
        b |= a;
        set_expected_result(8, 20.0F, 0x0555, b);
    } p_elseif(dregs[0] == 8.0F) {
        VecShort a = 0x0111;
        VecShort b = 0x0444;
        VecShort c = b | a;
        set_expected_result(8, 20.0F, 0x0555, c);
    }
    p_endif;

    po_if(dregs[0] == 9.0F) {
        VecUShort a = 0x0AAA;
        a = ~a;
        a &= 0x0FFF; // Tricky since ~ flips upper bits that immediates can't access
        set_expected_result(8, 22.0F, 0x0555, static_cast<VecShort>(a));
    } p_elseif(dregs[0] == 10.0F) {
        VecHalf a = 100.0F;
        dregs[8] = a.abs();
    } p_elseif(dregs[0] == 11.0F) {
        VecHalf a = -100.0F;
        dregs[8] = a.abs();
    } p_elseif(dregs[0] == 12.0F) {
        VecShort a = 100;
        set_expected_result(8, 24.0F, 100, a.abs());
    } p_elseif(dregs[0] == 13.0F) {
        VecShort a = -100;
        set_expected_result(8, 26.0F, 100, a.abs());
    }
    p_endif;

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
    copy_result_to_dreg0(8);
}

void test9()
{
    // SFPMULI, SFPADDI, SFPDIVP2, SFPLZ

    dregs[9] = -dregs[0];
    p_if(dregs[0] == 1.0F) {
        VecHalf a = 20.0F;
        dregs[9] = a * 30.0F;
    } p_elseif(dregs[0] == 2.0F) {
        VecHalf a = 20.0F;
        a *= 40.0F;
        dregs[9] = a;
    } p_elseif(dregs[0] == 3.0F) {
        VecHalf a = 20.0F;
        dregs[9] = a + 30.0F;
    } p_elseif(dregs[0] == 4.0F) {
        VecHalf a = 20.0F;
        a += 40.0F;
        dregs[9] = a;
    } p_elseif(dregs[0] == 5.0F) {
        VecHalf a = 16.0F;
        VecHalf b;
        b.add_exp(a, 4);
        dregs[9] = b;
    } p_elseif(dregs[0] == 6.0F) {
        VecHalf a = 256.0F;
        VecHalf b;
        b = 3.3f;
        b.add_exp(a, -4);
        dregs[9] = b;
    }
    p_endif;

    p_if(dregs[0] == 7.0F) {
        VecShort a = 0;
        VecShort b = a.lz();
        set_expected_result(9, 38.0F, 0x13, b);
    } p_elseif(dregs[0] == 8.0F) {
        VecShort a = 0xFFFF;
        VecShort b = a.lz();
        set_expected_result(9, 55.0F, 0x0, b);
    } p_elseif(dregs[0] == 9.0F) {
        VecUShort a = 0xFFFF;
        VecShort b = a.lz();
        dregs[9] = 14.0f;
        set_expected_result(9, 30.0F, 0x3, b);
    } p_elseif(dregs[0] < 13.0F) {
        VecHalf a = dregs[0] - 11.0F;
        VecUShort b;

        // Relies on if chain above...
        p_if(dregs[0] >= 7.0F) {
            p_if(b.lz_cc(a, LzCCNE0)) {
                dregs[9] = 60.0F;
            } p_else {
                dregs[9] = 40.0F;
            }
            p_endif;
        }
        p_endif;
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
    copy_result_to_dreg0(9);
}

void test10()
{
    // SFPSHFT, SFTSETSGN
    dregs[10] = -dregs[0];
    p_if(dregs[0] == 1.0F) {
        VecUShort a = 0x015;
        VecShort shft = 6;
        VecUShort b = a.shft(shft);
        // Could write better tests if we could return and test the int result
        set_expected_result(10, 20.0F, 0x0540, static_cast<VecShort>(b));
    } p_elseif(dregs[0] == 2.0F) {
        VecUShort a = 0x2AAA;
        VecUShort b = a.shft(-4);
        set_expected_result(10, 22.0F, 0x02AA, static_cast<VecShort>(b));
    } p_elseif(dregs[0] == 3.0F) {
        VecUShort a = 0xAAAA;
        VecShort shft = -6;
        VecUShort b = a.shft(shft);
        set_expected_result(10, 24.0F, 0x02AA, static_cast<VecShort>(b));
    } p_elseif(dregs[0] == 4.0F) {
        VecUShort a = 0x005A;
        VecUShort b = a.shft(4);
        set_expected_result(10, 26.0F, 0x05A0, static_cast<VecShort>(b));
    } p_elseif(dregs[0] == 5.0F) {
        VecShort a = 25;
        a = a + 5;
        a += 7;
        set_expected_result(10, 28.0F, 0x25, a);
    } p_elseif(dregs[0] == 6.0F) {
        VecShort a = 28;
        VecShort b = 100;
        a += b;
        set_expected_result(10, 30.0F, 0x80, a);
    }
    p_endif;

    p_if(dregs[0] == 7.0F) {
        VecHalf a = dregs[0];
        dregs[10] = a.set_sgn(1);
    } p_elseif(dregs[0] == 8.0F) {
        VecHalf a = dregs[0];
        VecHalf b = -128.0;
        VecHalf r = b.set_sgn(a);

        dregs[10] = r;
    } p_elseif(dregs[0] == 9.0F) {
        VecHalf a = -256.0F;
        dregs[10] = a.set_sgn(0);
    } p_elseif(dregs[0] == 10.0F) {
        VecHalf a = dregs[0];
        VecHalf b = -512.0F;
        VecHalf r = a.set_sgn(b);

        dregs[10] = r;
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
    // [10] = -10.0
    copy_result_to_dreg0(10);
}

void test11()
{
    // SFPLUT, SFPLOADL<n>
    dregs[11] = -dregs[0];

    VecUShort l0a = 0xFF30; // Multiply by 0.0, add 0.125
    VecUShort l1a = 0X3020; // Multiply by 0.125, add 0.25
    p_if(dregs[0] == 1.0F) {
        // Use L0
        VecHalf h = -0.3F;
        VecUShort l2a = 0xA010; // Mulitply by -0.25, add 0.5
        h.lut(l0a, l1a, l2a);
        dregs[11] = h;
    } p_elseif(dregs[0] == 2.0F) {
        // Use L0
        VecHalf h = -0.3F;
        VecUShort l2a = 0xA010; // Mulitply by -0.25, add 0.5
        h.lut(l0a, l1a, l2a, LUTSgnRetain);
        dregs[11] = h;
    } p_elseif(dregs[0] == 3.0F) {
        // Use L0
        VecHalf h = -0.3F;
        VecUShort l2a = 0xA010; // Mulitply by -0.25, add 0.5
        h.lut(l0a, l1a, l2a, LUTSgnUpdate, LUTOffsetNeg);
        dregs[11] = h;
    } p_elseif(dregs[0] == 4.0F) {
        // Use L0
        VecHalf h = -0.3F;
        VecUShort l2a = 0xA010; // Mulitply by -0.25, add 0.5
        h.lut(l0a, l1a, l2a, LUTSgnRetain, LUTOffsetPos);
        dregs[11] = h;
    } p_elseif(dregs[0] == 5.0F) {
        // Use L1
        VecHalf h = 1.0F;
        VecUShort l2a = 0xA010; // Mulitply by -0.25, add 0.5
        h.lut(l0a, l1a, l2a, LUTSgnRetain, LUTOffsetPos);
        dregs[11] = h;
    } p_elseif(dregs[0] == 6.0F) {
        // Use L2
        VecHalf h = 4.0F;
        VecUShort l2a = 0xA010; // Mulitply by -0.25, add 0.5
        h.lut(l0a, l1a, l2a, LUTSgnUpdate);
        dregs[11] = h;
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
    l0b.load_lreg(0);
    l1b.load_lreg(1);

    p_if(dregs[0] == 7.0F) {
        // Use L0
        VecHalf h = -0.3F;
        VecUShort l2b = 0x9000;
        h.lut(l0b, l1b, l2b);
        dregs[11] = h;
    } p_elseif(dregs[0] == 8.0F) {
        // Use L0
        VecHalf h = -0.3F;
        VecUShort l2b = 0x9000;
        h.lut(l0b, l1b, l2b, LUTSgnRetain);
        dregs[11] = h;
    } p_elseif(dregs[0] == 9.0F) {
        // Use L0
        VecHalf h = -0.3F;
        VecUShort l2b = 0x9000;
        h.lut(l0b, l1b, l2b, LUTSgnUpdate, LUTOffsetNeg);
        dregs[11] = h;
    } p_elseif(dregs[0] == 10.0F) {
        // Use L0
        VecHalf h = -0.3F;
        VecUShort l2b = 0x9000;
        h.lut(l0b, l1b, l2b, LUTSgnRetain, LUTOffsetPos);
        dregs[11] = h;
    } p_elseif(dregs[0] == 11.0F) {
        // Use L1
        VecHalf h = 1.0F;
        VecUShort l2b = 0x9000;
        h.lut(l0b, l1b, l2b, LUTSgnRetain, LUTOffsetPos);
        dregs[11] = h;
    } p_elseif(dregs[0] == 12.0F) {
        // Use L2
        VecHalf h = 4.0F;
        VecUShort l2b = 0x9000;
        h.lut(l0b, l1b, l2b, LUTSgnUpdate);
        dregs[11] = h;
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
    dregs[12] = -dregs[0];

    p_if(dregs[0] == 1.0F) {
        dregs[12] = static_cast<float>(imm); // SFPLOADI
    } p_elseif(dregs[0] == 2.0F) {
        VecShort a = 5;
        a += imm; // SFPIADD_I
        set_expected_result(12, 25.0F, 40, a);
    } p_elseif(dregs[0] == 3.0F) {
        VecHalf a = 3.0F;
        a += static_cast<float>(imm); // SFPADDI
        dregs[12] = a;
    }
    p_endif;

    dregs[imm] = 30.0F; // SFPSTORE

    p_if(dregs[0] == 4.0F) {
        dregs[12] = dregs[imm]; // SFPLOAD
    }
    p_endif;

    // [1] = 35.0F
    // [2] = 25.0F
    // [3] = 38.0F
    // [4] = 30.0F

    copy_result_to_dreg0(12);
}

// Here's the full list of grayskull instructions subject to the CC liveness issue
// I think the wrapper will hide some of these
// sfplut
// sfpdivp2
// sfpexexp
// sfpexman
// setexp_i
// setman_i
// setsgn_i


// sfpmov
// sfpload
// sfploadi
// sfpiadd_i
// abs X
// not X
// lz X
// mad


void test13()
{
    // Test variable liveness

    dregs[13] = -dregs[0];

    // ABS liveness across SETCC
    {
        VecHalf x = -20.0F;
        VecHalf y = -30.0F;
        p_if (dregs[0] == 1.0F) {
            y = x.abs();
        }
        p_endif;
        p_if (dregs[0] == 1.0F || dregs[0] == 2.0F) {
            dregs[13] = y;
        }
        p_endif;
    }
    // [1] = 20.0F
    // [2] = -30.0F

    // NOT liveness across SETCC
    {
        VecShort a = 0xFAAA;
        VecShort b = 0x07BB;
        p_if (dregs[0] == 3.0F) {
            b = ~a;
        }
        p_endif;
        p_if (dregs[0] == 3.0F || dregs[0] == 4.0F) {
            p_if (dregs[0] == 3.0F) {
                set_expected_result(13, 40.0F, 0x0555, b);
            }
            p_endif;
            p_if (dregs[0] == 4.0F) {
                set_expected_result(13, 50.0F, 0x07BB, b);
            }
            p_endif;
        }
        p_endif;
    }
    // [3] = 40.0F
    // [4] = 50.0F

    // LZ liveness across SETCC
    {
        VecShort a = 0x0080;
        VecShort b = 0x07BB;
        p_if (dregs[0] == 5.0F) {
            b = a.lz();
        }
        p_endif;
        p_if (dregs[0] == 5.0F || dregs[0] == 6.0F) {
            p_if (dregs[0] == 5.0F) {
                set_expected_result(13, 60.0F, 11, b);
            }
            p_endif;
            p_if (dregs[0] == 6.0F) {
                set_expected_result(13, 70.0F, 0x07BB, b);
            }
            p_endif;
        }
        p_endif;
    }
    // [5] = 60.0F
    // [6] = 70.0F

    // IADD_I liveness across SETCC
    {
        VecShort a = 10;
        VecShort b = 20;
        p_if (dregs[0] == 7.0F) {
            b = a + 30;
        }
        p_endif;
        p_if (dregs[0] == 7.0F || dregs[0] == 8.0F) {
            p_if (dregs[0] == 7.0F) {
                set_expected_result(13, 80.0F, 40, b);
            }
            p_endif;
            p_if (dregs[0] == 8.0F) {
                set_expected_result(13, 90.0F, 20, b);
            }
            p_endif;
        }
        p_endif;
    }
    // [7] = 80.0F
    // [8] = 90.0F

    // MAD liveness across SETCC
    {
        VecHalf a = 90.0F;
        VecHalf b = 110.0F;
        p_if (dregs[0] == 9.0F) {
            b = a + 10;
        }
        p_endif;
        p_if (dregs[0] == 9.0F || dregs[0] == 10.0F) {
            dregs[13] = b;
        }
        p_endif;
    }
    // [9] = 100.0F
    // [10] = 110.0F

    // MOV liveness across SETCC
    {
        VecHalf a = 120.0F;
        VecHalf b = 130.0F;
        p_if (dregs[0] == 11.0F) {
            b = -a;
        }
        p_endif;
        p_if (dregs[0] == 11.0F || dregs[0] == 12.0F) {
            dregs[13] = b;
        }
        p_endif;
    }
    // [11] = -120.0F
    // [12] = 130.0F;

    // Above test various builtins for liveness across a SETCC
    // Below test MOV liveness across COMPC, LZ, IADD, EXEXP

    // MOV liveness across COMPC
    {
        VecHalf a = 140.0F;
        VecHalf b = 150.0F;
        p_if (dregs[0] != 13.0F) {
            b = 160.0F;
        } p_else {
            VecHalf c = CReg_0 * CReg_0 + CReg_0;
            b = -a;
            a = c;
        }
        p_endif;
        p_if (dregs[0] == 13.0F || dregs[0] == 14.0F) {
            dregs[13] = b;
        }
        p_endif;
    }
    // [13] = -140.0F
    // [14] = 160.0F;

    copy_result_to_dreg0(13);
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
        test13();
    }
#if 0
    // TODO:
    //   test || w/ add_cc, lz_cc and ex_exp_cc.
#endif
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
