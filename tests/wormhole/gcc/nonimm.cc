#include <cstdio>

// Test non-immediate (TT_) forms of instructions

typedef float v64sf __attribute__((vector_size(64*4)));

namespace ckernel {
    volatile unsigned int *instrn_buffer;
};
using namespace ckernel;

#include "ckernel_ops.h"

void macro(int value)
{
    TT_SFPLOAD(2, 6, 0, value);
    TT_SFPLOADI(2, 6, value);
    TT_SFPSTORE(2, 6, 0, value);
    TT_SFPIADD(value, 2, 3, 5);
    TT_SFPADDI(value, 2, 10);
    TT_SFPSHFT(value, 2, 3, 1);
    TT_SFPDIVP2(value, 2, 3, 1);
    TT_SFPSETEXP(value, 2, 3, 1);
    TT_SFPSETMAN(value, 2, 3, 1);
    TT_SFPSETSGN(value, 2, 3, 1);
}

void macro_checked(int value)
{
    TT_SFPLOAD(2, 6, 0, value & 0xFFFF);
    TT_SFPLOADI(2, 6, value & 0xFFFF);
    TT_SFPSTORE(2, 6, 0, value & 0xFFFF);
    TT_SFPIADD(value & 0x0FFF, 2, 3, 5);
    TT_SFPADDI(value & 0xFFFF, 2, 10);
    TT_SFPSHFT(value & 0x0FFF, 2, 3, 1);
    TT_SFPDIVP2(value & 0x0FFF, 2, 3, 1);
    TT_SFPSETEXP(value & 0x0FFF, 2, 3, 1);
    TT_SFPSETMAN(value & 0x0FFF, 2, 3, 1);
    TT_SFPSETSGN(value & 0x0FFF, 2, 3, 1);
}

void macro_reuse_checked(int value)
{
    // Convtrived
    TT_SFPLOAD(2, 6, 0, value & 0xFFFF);
    TT_SFPLOAD(2, 6, 0, (value + 1) & 0xFFFF);
    TT_SFPLOAD(2, 6, 0, (value + 2) & 0xFFFF);
    TT_SFPLOAD(2, 6, 0, (value + 3) & 0xFFFF);

    TT_SFPSTORE(2, 6, 0, value & 0xFFFF);
    TT_SFPSTORE(2, 6, 0, (value + 1) & 0xFFFF);
    TT_SFPSTORE(2, 6, 0, (value + 2) & 0xFFFF);
    TT_SFPSTORE(2, 6, 0, (value + 3) & 0xFFFF);
}

void load_imm(short int addr)
{
    v64sf x = __builtin_rvtt_wh_sfpload((void *)instrn_buffer, 1, 0, 10, 0, 0);
    v64sf y = __builtin_rvtt_wh_sfpload((void *)instrn_buffer, 1, 0, 20, 0, 0);
    v64sf z = __builtin_rvtt_wh_sfpload((void *)instrn_buffer, 1, 0, 30, 0, 0);
    v64sf a = __builtin_rvtt_wh_sfpload((void *)instrn_buffer, 1, 0, 40, 0, 0);
    __builtin_rvtt_wh_sfpstore((void *)instrn_buffer, x, 0, 2, 1, 0, 0);
    __builtin_rvtt_wh_sfpstore((void *)instrn_buffer, y, 0, 2, 10, 0, 0);
    __builtin_rvtt_wh_sfpstore((void *)instrn_buffer, z, 0, 2, 20, 0, 0);
    __builtin_rvtt_wh_sfpstore((void *)instrn_buffer, a, 0, 2, 30, 0, 0);
}

void load_reg(short int addr)
{
    v64sf x = __builtin_rvtt_wh_sfpload((void *)instrn_buffer, 1, 0, addr, 0, 0);
    v64sf y = __builtin_rvtt_wh_sfpload((void *)instrn_buffer, 1, 0, addr+10, 0, 0);
    v64sf z = __builtin_rvtt_wh_sfpload((void *)instrn_buffer, 1, 0, addr+20, 0, 0);
    v64sf a = __builtin_rvtt_wh_sfpload((void *)instrn_buffer, 1, 0, addr+30, 0, 0);
    __builtin_rvtt_wh_sfpstore((void *)instrn_buffer, x, 0, 2, 1, 0, 0);
    __builtin_rvtt_wh_sfpstore((void *)instrn_buffer, y, 0, 2, 10, 0, 0);
    __builtin_rvtt_wh_sfpstore((void *)instrn_buffer, z, 0, 2, 20, 0, 0);
    __builtin_rvtt_wh_sfpstore((void *)instrn_buffer, a, 0, 2, 30, 0, 0);
}

void load_both(short int addr)
{
    v64sf x = __builtin_rvtt_wh_sfpload((void *)instrn_buffer, 1, 0, 10, 0, 0);
    v64sf y = __builtin_rvtt_wh_sfpload((void *)instrn_buffer, 1, 0, 20, 0, 0);
    v64sf z = __builtin_rvtt_wh_sfpload((void *)instrn_buffer, 1, 0, addr, 0, 0);
    v64sf a = __builtin_rvtt_wh_sfpload((void *)instrn_buffer, 1, 0, addr+10, 0, 0);
    __builtin_rvtt_wh_sfpstore((void *)instrn_buffer, x, 0, 2, 1, 0, 0);
    __builtin_rvtt_wh_sfpstore((void *)instrn_buffer, y, 0, 2, 10, 0, 0);
    __builtin_rvtt_wh_sfpstore((void *)instrn_buffer, z, 0, 2, 20, 0, 0);
    __builtin_rvtt_wh_sfpstore((void *)instrn_buffer, a, 0, 2, 30, 0, 0);
}

void loadi_imm(short int value)
{
    v64sf x = __builtin_rvtt_wh_sfpxloadi((void *)instrn_buffer, 0, 10, 0, 0);
    v64sf y = __builtin_rvtt_wh_sfpxloadi((void *)instrn_buffer, 4, 10, 0, 0);
    v64sf z = __builtin_rvtt_wh_sfpxloadi((void *)instrn_buffer, 8, 20, 0, 0);
    v64sf a = __builtin_rvtt_wh_sfpxloadi((void *)instrn_buffer, 10, 30, 0, 0);
    __builtin_rvtt_wh_sfpstore((void *)instrn_buffer, x, 0, 2, 1, 0, 0);
    __builtin_rvtt_wh_sfpstore((void *)instrn_buffer, y, 0, 2, 10, 0, 0);
    __builtin_rvtt_wh_sfpstore((void *)instrn_buffer, z, 0, 2, 20, 0, 0);
    __builtin_rvtt_wh_sfpstore((void *)instrn_buffer, a, 0, 2, 30, 0, 0);
}

void loadi_reg(short int value)
{
    v64sf x = __builtin_rvtt_wh_sfpxloadi((void *)instrn_buffer, 0, value, 0, 0);
    v64sf y = __builtin_rvtt_wh_sfpxloadi((void *)instrn_buffer, 4, value+1, 0, 0);
    v64sf z = __builtin_rvtt_wh_sfpxloadi((void *)instrn_buffer, 8, value+2, 0, 0);
    v64sf a = __builtin_rvtt_wh_sfpxloadi((void *)instrn_buffer, 12, value+3, 0, 0);
    __builtin_rvtt_wh_sfpstore((void *)instrn_buffer, x, 0, 2, 1, 0, 0);
    __builtin_rvtt_wh_sfpstore((void *)instrn_buffer, y, 0, 2, 10, 0, 0);
    __builtin_rvtt_wh_sfpstore((void *)instrn_buffer, z, 0, 2, 20, 0, 0);
    __builtin_rvtt_wh_sfpstore((void *)instrn_buffer, a, 0, 2, 30, 0, 0);
}

void loadi_both(short int value)
{
    v64sf x = __builtin_rvtt_wh_sfpxloadi((void *)instrn_buffer, 0, 10, 0, 0);
    v64sf y = __builtin_rvtt_wh_sfpxloadi((void *)instrn_buffer, 4, 20, 0, 0);
    v64sf z = __builtin_rvtt_wh_sfpxloadi((void *)instrn_buffer, 8, value, 0, 0);
    v64sf a = __builtin_rvtt_wh_sfpxloadi((void *)instrn_buffer, 12, value+1, 0, 0);
    __builtin_rvtt_wh_sfpstore((void *)instrn_buffer, x, 0, 2, 1, 0, 0);
    __builtin_rvtt_wh_sfpstore((void *)instrn_buffer, y, 0, 2, 10, 0, 0);
    __builtin_rvtt_wh_sfpstore((void *)instrn_buffer, z, 0, 2, 20, 0, 0);
    __builtin_rvtt_wh_sfpstore((void *)instrn_buffer, a, 0, 2, 30, 0, 0);
}

void store_imm(short int addr)
{
    v64sf x = __builtin_rvtt_wh_sfpload((void *)instrn_buffer, 1, 0, 10, 0, 0);
    __builtin_rvtt_wh_sfpstore((void *)instrn_buffer, x, 0, 2, 10, 0, 0);
    __builtin_rvtt_wh_sfpstore((void *)instrn_buffer, x, 0, 2, 20, 0, 0);
    __builtin_rvtt_wh_sfpstore((void *)instrn_buffer, x, 0, 2, 30, 0, 0);
    __builtin_rvtt_wh_sfpstore((void *)instrn_buffer, x, 0, 2, 40, 0, 0);
}

void store_reg(short int addr)
{
    v64sf x = __builtin_rvtt_wh_sfpload((void *)instrn_buffer, 1, 0, 10, 0, 0);
    __builtin_rvtt_wh_sfpstore((void *)instrn_buffer, x, 0, 2, addr, 0, 0);
    __builtin_rvtt_wh_sfpstore((void *)instrn_buffer, x, 2, 0, addr+1, 0, 0);
    __builtin_rvtt_wh_sfpstore((void *)instrn_buffer, x, 2, 0, addr+2, 0, 0);
    __builtin_rvtt_wh_sfpstore((void *)instrn_buffer, x, 2, 0, addr+3, 0, 0);
}

void store_both(short int addr)
{
    v64sf x = __builtin_rvtt_wh_sfpload((void *)instrn_buffer, 1, 0, 10, 0, 0);
    __builtin_rvtt_wh_sfpstore((void *)instrn_buffer, x, 0, 2, 10, 0, 0);
    __builtin_rvtt_wh_sfpstore((void *)instrn_buffer, x, 0, 2, 20, 0, 0);
    __builtin_rvtt_wh_sfpstore((void *)instrn_buffer, x, 2, 0, addr+1, 0, 0);
    __builtin_rvtt_wh_sfpstore((void *)instrn_buffer, x, 2, 0, addr+2, 0, 0);
}

void store_r_imm(short int addr)
{
    v64sf lr13 = __builtin_rvtt_sfpassignlr(13);
    __builtin_rvtt_wh_sfpstore((void *)instrn_buffer, lr13, 0, 2, 10, 0, 0);
    __builtin_rvtt_wh_sfpstore((void *)instrn_buffer, lr13, 0, 2, 20, 0, 0);
    __builtin_rvtt_wh_sfpstore((void *)instrn_buffer, lr13, 0, 2, 10, 0, 0);
    __builtin_rvtt_wh_sfpstore((void *)instrn_buffer, lr13, 0, 2, 20, 0, 0);
}

void store_r_reg(short int addr)
{
    v64sf lr13 = __builtin_rvtt_sfpassignlr(13);
    __builtin_rvtt_wh_sfpstore((void *)instrn_buffer, lr13, 0, 2, addr, 0, 0);
    __builtin_rvtt_wh_sfpstore((void *)instrn_buffer, lr13, 2, 0, addr+1, 0, 0);
    __builtin_rvtt_wh_sfpstore((void *)instrn_buffer, lr13, 2, 0, addr+2, 0, 0);
    __builtin_rvtt_wh_sfpstore((void *)instrn_buffer, lr13, 2, 0, addr+3, 0, 0);
}

void store_r_both(short int addr)
{
    v64sf lr13 = __builtin_rvtt_sfpassignlr(13);
    __builtin_rvtt_wh_sfpstore((void *)instrn_buffer, lr13, 0, 2, 10, 0, 0);
    __builtin_rvtt_wh_sfpstore((void *)instrn_buffer, lr13, 0, 2, 20, 0, 0);
    __builtin_rvtt_wh_sfpstore((void *)instrn_buffer, lr13, 0, 2, addr, 0, 0);
    __builtin_rvtt_wh_sfpstore((void *)instrn_buffer, lr13, 2, 0, addr+1, 0, 0);
}

void addi_imm(short int value)
{
    v64sf x = __builtin_rvtt_wh_sfpload((void *)instrn_buffer, 1, 0, 10, 0, 0);
    v64sf y = __builtin_rvtt_wh_sfpload((void *)instrn_buffer, 1, 0, 20, 0, 0);
    v64sf z = __builtin_rvtt_wh_sfpload((void *)instrn_buffer, 1, 0, 30, 0, 0);
    v64sf a = __builtin_rvtt_wh_sfpload((void *)instrn_buffer, 1, 0, 40, 0, 0);
    x = __builtin_rvtt_wh_sfpaddi((void *)instrn_buffer, x, 1, 0, 0, 0);
    y = __builtin_rvtt_wh_sfpaddi((void *)instrn_buffer, y, 2, 0, 0, 0);
    z = __builtin_rvtt_wh_sfpaddi((void *)instrn_buffer, z, 3, 0, 0, 0);
    a = __builtin_rvtt_wh_sfpaddi((void *)instrn_buffer, a, 4, 0, 0, 0);
    __builtin_rvtt_wh_sfpstore((void *)instrn_buffer, x, 0, 2, 1, 0, 0);
    __builtin_rvtt_wh_sfpstore((void *)instrn_buffer, y, 0, 2, 10, 0, 0);
    __builtin_rvtt_wh_sfpstore((void *)instrn_buffer, z, 0, 2, 20, 0, 0);
    __builtin_rvtt_wh_sfpstore((void *)instrn_buffer, a, 0, 2, 30, 0, 0);
}

void addi_reg(short int value)
{
    v64sf x = __builtin_rvtt_wh_sfpload((void *)instrn_buffer, 1, 0, 10, 0, 0);
    v64sf y = __builtin_rvtt_wh_sfpload((void *)instrn_buffer, 1, 0, 20, 0, 0);
    v64sf z = __builtin_rvtt_wh_sfpload((void *)instrn_buffer, 1, 0, 30, 0, 0);
    v64sf a = __builtin_rvtt_wh_sfpload((void *)instrn_buffer, 1, 0, 40, 0, 0);
    x = __builtin_rvtt_wh_sfpaddi((void *)instrn_buffer, x, value, 0, 0, 0);
    y = __builtin_rvtt_wh_sfpaddi((void *)instrn_buffer, y, value+1, 0, 0, 0);
    z = __builtin_rvtt_wh_sfpaddi((void *)instrn_buffer, z, value+2, 0, 0, 0);
    a = __builtin_rvtt_wh_sfpaddi((void *)instrn_buffer, a, value+3, 0, 0, 0);
    __builtin_rvtt_wh_sfpstore((void *)instrn_buffer, x, 0, 2, 1, 0, 0);
    __builtin_rvtt_wh_sfpstore((void *)instrn_buffer, y, 0, 2, 10, 0, 0);
    __builtin_rvtt_wh_sfpstore((void *)instrn_buffer, z, 0, 2, 20, 0, 0);
    __builtin_rvtt_wh_sfpstore((void *)instrn_buffer, a, 0, 2, 30, 0, 0);
}

void addi_both(short int value)
{
    v64sf x = __builtin_rvtt_wh_sfpload((void *)instrn_buffer, 1, 0, 10, 0, 0);
    v64sf y = __builtin_rvtt_wh_sfpload((void *)instrn_buffer, 1, 0, 20, 0, 0);
    v64sf z = __builtin_rvtt_wh_sfpload((void *)instrn_buffer, 1, 0, 30, 0, 0);
    v64sf a = __builtin_rvtt_wh_sfpload((void *)instrn_buffer, 1, 0, 40, 0, 0);
    x = __builtin_rvtt_wh_sfpaddi((void *)instrn_buffer, x, 1, 0, 0, 0);
    y = __builtin_rvtt_wh_sfpaddi((void *)instrn_buffer, y, 2, 0, 0, 0);
    z = __builtin_rvtt_wh_sfpaddi((void *)instrn_buffer, z, value, 0, 0, 0);
    a = __builtin_rvtt_wh_sfpaddi((void *)instrn_buffer, a, value+1, 0, 0, 0);
    __builtin_rvtt_wh_sfpstore((void *)instrn_buffer, x, 0, 2, 1, 0, 0);
    __builtin_rvtt_wh_sfpstore((void *)instrn_buffer, y, 0, 2, 10, 0, 0);
    __builtin_rvtt_wh_sfpstore((void *)instrn_buffer, z, 0, 2, 20, 0, 0);
    __builtin_rvtt_wh_sfpstore((void *)instrn_buffer, a, 0, 2, 30, 0, 0);
}

void xiadd_i_imm(short int value)
{
    v64sf x = __builtin_rvtt_wh_sfpload((void *)instrn_buffer, 1, 0, 10, 0, 0);
    v64sf y = __builtin_rvtt_wh_sfpload((void *)instrn_buffer, 1, 0, 20, 0, 0);
    v64sf z = __builtin_rvtt_wh_sfpload((void *)instrn_buffer, 1, 0, 30, 0, 0);
    v64sf a = __builtin_rvtt_wh_sfpload((void *)instrn_buffer, 1, 0, 40, 0, 0);
    x = __builtin_rvtt_wh_sfpxiadd_i((void *)instrn_buffer, x, 1, 0, 0, 0);
    y = __builtin_rvtt_wh_sfpxiadd_i((void *)instrn_buffer, y, 2, 0, 0, 0);
    z = __builtin_rvtt_wh_sfpxiadd_i((void *)instrn_buffer, z, 3, 0, 0, 0);
    a = __builtin_rvtt_wh_sfpxiadd_i((void *)instrn_buffer, a, 4, 0, 0, 0);
    __builtin_rvtt_wh_sfpstore((void *)instrn_buffer, x, 0, 2, 1, 0, 0);
    __builtin_rvtt_wh_sfpstore((void *)instrn_buffer, y, 0, 2, 10, 0, 0);
    __builtin_rvtt_wh_sfpstore((void *)instrn_buffer, z, 0, 2, 20, 0, 0);
    __builtin_rvtt_wh_sfpstore((void *)instrn_buffer, a, 0, 2, 30, 0, 0);
}

void xiadd_i_reg(short int value)
{
    v64sf x = __builtin_rvtt_wh_sfpload((void *)instrn_buffer, 1, 0, 10, 0, 0);
    v64sf y = __builtin_rvtt_wh_sfpload((void *)instrn_buffer, 1, 0, 20, 0, 0);
    v64sf z = __builtin_rvtt_wh_sfpload((void *)instrn_buffer, 1, 0, 30, 0, 0);
    x = __builtin_rvtt_wh_sfpxiadd_i((void *)instrn_buffer, x, value+0, 0, 0, 0);
    y = __builtin_rvtt_wh_sfpxiadd_i((void *)instrn_buffer, y, value+1, 0, 0, 0);
    z = __builtin_rvtt_wh_sfpxiadd_i((void *)instrn_buffer, z, value+2, 0, 0, 0);
    __builtin_rvtt_wh_sfpstore((void *)instrn_buffer, x, 0, 2, 1, 0, 0);
    __builtin_rvtt_wh_sfpstore((void *)instrn_buffer, y, 0, 2, 10, 0, 0);
    __builtin_rvtt_wh_sfpstore((void *)instrn_buffer, z, 0, 2, 20, 0, 0);
}

void xiadd_i_both(short int value)
{
    v64sf x = __builtin_rvtt_wh_sfpload((void *)instrn_buffer, 1, 0, 10, 0, 0);
    v64sf y = __builtin_rvtt_wh_sfpload((void *)instrn_buffer, 1, 0, 20, 0, 0);
    v64sf z = __builtin_rvtt_wh_sfpload((void *)instrn_buffer, 1, 0, 30, 0, 0);
    x = __builtin_rvtt_wh_sfpxiadd_i((void *)instrn_buffer, x, 1,       0, 0, 0);
    y = __builtin_rvtt_wh_sfpxiadd_i((void *)instrn_buffer, y, 2,       0, 0, 0);
    z = __builtin_rvtt_wh_sfpxiadd_i((void *)instrn_buffer, z, value+0, 0, 0, 0);
    __builtin_rvtt_wh_sfpstore((void *)instrn_buffer, x, 0, 2, 1, 0, 0);
    __builtin_rvtt_wh_sfpstore((void *)instrn_buffer, y, 0, 2, 10, 0, 0);
    __builtin_rvtt_wh_sfpstore((void *)instrn_buffer, z, 0, 2, 20, 0, 0);
}

void shft_i_imm(short int value)
{
    v64sf x = __builtin_rvtt_wh_sfpload((void *)instrn_buffer, 1, 0, 10, 0, 0);
    v64sf y = __builtin_rvtt_wh_sfpload((void *)instrn_buffer, 1, 0, 20, 0, 0);
    v64sf z = __builtin_rvtt_wh_sfpload((void *)instrn_buffer, 1, 0, 30, 0, 0);
    v64sf a = __builtin_rvtt_wh_sfpload((void *)instrn_buffer, 1, 0, 40, 0, 0);
    x = __builtin_rvtt_wh_sfpshft_i((void *)instrn_buffer, x, 1, 0, 0);
    y = __builtin_rvtt_wh_sfpshft_i((void *)instrn_buffer, y, 2, 0, 0);
    z = __builtin_rvtt_wh_sfpshft_i((void *)instrn_buffer, z, 3, 0, 0);
    a = __builtin_rvtt_wh_sfpshft_i((void *)instrn_buffer, a, 4, 0, 0);
    __builtin_rvtt_wh_sfpstore((void *)instrn_buffer, x, 0, 2, 1, 0, 0);
    __builtin_rvtt_wh_sfpstore((void *)instrn_buffer, y, 0, 2, 10, 0, 0);
    __builtin_rvtt_wh_sfpstore((void *)instrn_buffer, z, 0, 2, 20, 0, 0);
    __builtin_rvtt_wh_sfpstore((void *)instrn_buffer, a, 0, 2, 30, 0, 0);
}

void shft_i_reg(short int value)
{
    v64sf x = __builtin_rvtt_wh_sfpload((void *)instrn_buffer, 1, 0, 10, 0, 0);
    v64sf y = __builtin_rvtt_wh_sfpload((void *)instrn_buffer, 1, 0, 20, 0, 0);
    v64sf z = __builtin_rvtt_wh_sfpload((void *)instrn_buffer, 1, 0, 30, 0, 0);
    v64sf a = __builtin_rvtt_wh_sfpload((void *)instrn_buffer, 1, 0, 40, 0, 0);
    x = __builtin_rvtt_wh_sfpshft_i((void *)instrn_buffer, x, value+0, 0, 0);
    y = __builtin_rvtt_wh_sfpshft_i((void *)instrn_buffer, y, value+1, 0, 0);
    z = __builtin_rvtt_wh_sfpshft_i((void *)instrn_buffer, z, value+2, 0, 0);
    a = __builtin_rvtt_wh_sfpshft_i((void *)instrn_buffer, a, value+3, 0, 0);
    __builtin_rvtt_wh_sfpstore((void *)instrn_buffer, x, 0, 2, 1, 0, 0);
    __builtin_rvtt_wh_sfpstore((void *)instrn_buffer, y, 0, 2, 10, 0, 0);
    __builtin_rvtt_wh_sfpstore((void *)instrn_buffer, z, 0, 2, 20, 0, 0);
    __builtin_rvtt_wh_sfpstore((void *)instrn_buffer, a, 0, 2, 30, 0, 0);
}

void shft_i_both(short int value)
{
    v64sf x = __builtin_rvtt_wh_sfpload((void *)instrn_buffer, 1, 0, 10, 0, 0);
    v64sf y = __builtin_rvtt_wh_sfpload((void *)instrn_buffer, 1, 0, 20, 0, 0);
    v64sf z = __builtin_rvtt_wh_sfpload((void *)instrn_buffer, 1, 0, 30, 0, 0);
    v64sf a = __builtin_rvtt_wh_sfpload((void *)instrn_buffer, 1, 0, 40, 0, 0);
    x = __builtin_rvtt_wh_sfpshft_i((void *)instrn_buffer, x, 1, 0, 0);
    y = __builtin_rvtt_wh_sfpshft_i((void *)instrn_buffer, y, 2, 0, 0);
    z = __builtin_rvtt_wh_sfpshft_i((void *)instrn_buffer, z, value+0, 0, 0);
    a = __builtin_rvtt_wh_sfpshft_i((void *)instrn_buffer, a, value+1, 0, 0);
    __builtin_rvtt_wh_sfpstore((void *)instrn_buffer, x, 0, 2, 1, 0, 0);
    __builtin_rvtt_wh_sfpstore((void *)instrn_buffer, y, 0, 2, 10, 0, 0);
    __builtin_rvtt_wh_sfpstore((void *)instrn_buffer, z, 0, 2, 20, 0, 0);
    __builtin_rvtt_wh_sfpstore((void *)instrn_buffer, a, 0, 2, 30, 0, 0);
}

void divp2_imm(short int value)
{
    v64sf x = __builtin_rvtt_wh_sfpload((void *)instrn_buffer, 1, 0, 10, 0, 0);
    v64sf y = __builtin_rvtt_wh_sfpload((void *)instrn_buffer, 1, 0, 20, 0, 0);
    v64sf z = __builtin_rvtt_wh_sfpload((void *)instrn_buffer, 1, 0, 30, 0, 0);
    v64sf a = __builtin_rvtt_wh_sfpload((void *)instrn_buffer, 1, 0, 40, 0, 0);
    x = __builtin_rvtt_wh_sfpdivp2((void *)instrn_buffer, 0x1A, 0, 0, z, 0);
    y = __builtin_rvtt_wh_sfpdivp2((void *)instrn_buffer, 0x2A, 0, 0, y, 1);
    z = __builtin_rvtt_wh_sfpdivp2((void *)instrn_buffer, 0x3A, 0, 0, z, 0);
    a = __builtin_rvtt_wh_sfpdivp2((void *)instrn_buffer, 0x4A, 0, 0, a, 1);
    __builtin_rvtt_wh_sfpstore((void *)instrn_buffer, x, 0, 2, 1, 0, 0);
    __builtin_rvtt_wh_sfpstore((void *)instrn_buffer, y, 0, 2, 10, 0, 0);
    __builtin_rvtt_wh_sfpstore((void *)instrn_buffer, z, 0, 2, 20, 0, 0);
    __builtin_rvtt_wh_sfpstore((void *)instrn_buffer, a, 0, 2, 30, 0, 0);
}

void divp2_reg(short int value)
{
    v64sf x = __builtin_rvtt_wh_sfpload((void *)instrn_buffer, 1, 0, 10, 0, 0);
    v64sf y = __builtin_rvtt_wh_sfpload((void *)instrn_buffer, 1, 0, 20, 0, 0);
    v64sf z = __builtin_rvtt_wh_sfpload((void *)instrn_buffer, 1, 0, 30, 0, 0);
    v64sf a = __builtin_rvtt_wh_sfpload((void *)instrn_buffer, 1, 0, 40, 0, 0);
    x = __builtin_rvtt_wh_sfpdivp2((void *)instrn_buffer, value+0, 0, 0, a, 0);
    y = __builtin_rvtt_wh_sfpdivp2((void *)instrn_buffer, value+1, 0, 0, y, 1);
    z = __builtin_rvtt_wh_sfpdivp2((void *)instrn_buffer, value+2, 0, 0, z, 0);
    a = __builtin_rvtt_wh_sfpdivp2((void *)instrn_buffer, value+3, 0, 0, a, 1);
    __builtin_rvtt_wh_sfpstore((void *)instrn_buffer, x, 0, 2, 1, 0, 0);
    __builtin_rvtt_wh_sfpstore((void *)instrn_buffer, y, 0, 2, 10, 0, 0);
    __builtin_rvtt_wh_sfpstore((void *)instrn_buffer, z, 0, 2, 20, 0, 0);
    __builtin_rvtt_wh_sfpstore((void *)instrn_buffer, a, 0, 2, 30, 0, 0);
}

void divp2_both(short int value)
{
    v64sf x = __builtin_rvtt_wh_sfpload((void *)instrn_buffer, 1, 0, 10, 0, 0);
    v64sf y = __builtin_rvtt_wh_sfpload((void *)instrn_buffer, 1, 0, 20, 0, 0);
    v64sf z = __builtin_rvtt_wh_sfpload((void *)instrn_buffer, 1, 0, 30, 0, 0);
    v64sf a = __builtin_rvtt_wh_sfpload((void *)instrn_buffer, 1, 0, 40, 0, 0);
    x = __builtin_rvtt_wh_sfpdivp2((void *)instrn_buffer, 0x1B, 0, 0, a, 0);
    y = __builtin_rvtt_wh_sfpdivp2((void *)instrn_buffer, 0X1B, 0, 0, y, 1);
    z = __builtin_rvtt_wh_sfpdivp2((void *)instrn_buffer, value+0, 0, 0, z, 0);
    a = __builtin_rvtt_wh_sfpdivp2((void *)instrn_buffer, value+1, 0, 0, a, 1);
    __builtin_rvtt_wh_sfpstore((void *)instrn_buffer, x, 0, 2, 1, 0, 0);
    __builtin_rvtt_wh_sfpstore((void *)instrn_buffer, y, 0, 2, 10, 0, 0);
    __builtin_rvtt_wh_sfpstore((void *)instrn_buffer, z, 0, 2, 20, 0, 0);
    __builtin_rvtt_wh_sfpstore((void *)instrn_buffer, a, 0, 2, 30, 0, 0);
}

void setexp_i_imm(short int value)
{
    v64sf x = __builtin_rvtt_wh_sfpload((void *)instrn_buffer, 1, 0, 10, 0, 0);
    v64sf y = __builtin_rvtt_wh_sfpload((void *)instrn_buffer, 1, 0, 20, 0, 0);
    v64sf z = __builtin_rvtt_wh_sfpload((void *)instrn_buffer, 1, 0, 30, 0, 0);
    v64sf a = __builtin_rvtt_wh_sfpload((void *)instrn_buffer, 1, 0, 40, 0, 0);
    x = __builtin_rvtt_wh_sfpsetexp_i((void *)instrn_buffer, 0xAA, 0, 0, x);
    y = __builtin_rvtt_wh_sfpsetexp_i((void *)instrn_buffer, 0xAA, 0, 0, y);
    z = __builtin_rvtt_wh_sfpsetexp_i((void *)instrn_buffer, 0xAA, 0, 0, z);
    a = __builtin_rvtt_wh_sfpsetexp_i((void *)instrn_buffer, 0xAA, 0, 0, a);
    __builtin_rvtt_wh_sfpstore((void *)instrn_buffer, x, 0, 2, 1, 0, 0);
    __builtin_rvtt_wh_sfpstore((void *)instrn_buffer, y, 0, 2, 10, 0, 0);
    __builtin_rvtt_wh_sfpstore((void *)instrn_buffer, z, 0, 2, 20, 0, 0);
    __builtin_rvtt_wh_sfpstore((void *)instrn_buffer, a, 0, 2, 30, 0, 0);
}

void setexp_i_reg(short int value)
{
    v64sf x = __builtin_rvtt_wh_sfpload((void *)instrn_buffer, 1, 0, 10, 0, 0);
    v64sf y = __builtin_rvtt_wh_sfpload((void *)instrn_buffer, 1, 0, 20, 0, 0);
    v64sf z = __builtin_rvtt_wh_sfpload((void *)instrn_buffer, 1, 0, 30, 0, 0);
    v64sf a = __builtin_rvtt_wh_sfpload((void *)instrn_buffer, 1, 0, 40, 0, 0);
    x = __builtin_rvtt_wh_sfpsetexp_i((void *)instrn_buffer, value+0, 0, 0, x);
    y = __builtin_rvtt_wh_sfpsetexp_i((void *)instrn_buffer, value+1, 0, 0, y);
    z = __builtin_rvtt_wh_sfpsetexp_i((void *)instrn_buffer, value+2, 0, 0, z);
    a = __builtin_rvtt_wh_sfpsetexp_i((void *)instrn_buffer, value+3, 0, 0, a);
    __builtin_rvtt_wh_sfpstore((void *)instrn_buffer, x, 0, 2, 1, 0, 0);
    __builtin_rvtt_wh_sfpstore((void *)instrn_buffer, y, 0, 2, 10, 0, 0);
    __builtin_rvtt_wh_sfpstore((void *)instrn_buffer, z, 0, 2, 20, 0, 0);
    __builtin_rvtt_wh_sfpstore((void *)instrn_buffer, a, 0, 2, 30, 0, 0);
}

void setexp_i_both(short int value)
{
    v64sf x = __builtin_rvtt_wh_sfpload((void *)instrn_buffer, 1, 0, 10, 0, 0);
    v64sf y = __builtin_rvtt_wh_sfpload((void *)instrn_buffer, 1, 0, 20, 0, 0);
    v64sf z = __builtin_rvtt_wh_sfpload((void *)instrn_buffer, 1, 0, 30, 0, 0);
    v64sf a = __builtin_rvtt_wh_sfpload((void *)instrn_buffer, 1, 0, 40, 0, 0);
    x = __builtin_rvtt_wh_sfpsetexp_i((void *)instrn_buffer, 0xBB, 0, 0, x);
    y = __builtin_rvtt_wh_sfpsetexp_i((void *)instrn_buffer, 0XBB, 0, 0, y);
    z = __builtin_rvtt_wh_sfpsetexp_i((void *)instrn_buffer, value+0, 0, 0, z);
    a = __builtin_rvtt_wh_sfpsetexp_i((void *)instrn_buffer, value+1, 0, 0, a);
    __builtin_rvtt_wh_sfpstore((void *)instrn_buffer, x, 0, 2, 1, 0, 0);
    __builtin_rvtt_wh_sfpstore((void *)instrn_buffer, y, 0, 2, 10, 0, 0);
    __builtin_rvtt_wh_sfpstore((void *)instrn_buffer, z, 0, 2, 20, 0, 0);
    __builtin_rvtt_wh_sfpstore((void *)instrn_buffer, a, 0, 2, 30, 0, 0);
}

void setman_i_imm(short int value)
{
    v64sf x = __builtin_rvtt_wh_sfpload((void *)instrn_buffer, 1, 0, 10, 0, 0);
    v64sf y = __builtin_rvtt_wh_sfpload((void *)instrn_buffer, 1, 0, 20, 0, 0);
    v64sf z = __builtin_rvtt_wh_sfpload((void *)instrn_buffer, 1, 0, 30, 0, 0);
    v64sf a = __builtin_rvtt_wh_sfpload((void *)instrn_buffer, 1, 0, 40, 0, 0);
    x = __builtin_rvtt_wh_sfpsetman_i((void *)instrn_buffer, 0xAA, 0, 0, x, 1);
    y = __builtin_rvtt_wh_sfpsetman_i((void *)instrn_buffer, 0xAA, 0, 0, y, 1);
    z = __builtin_rvtt_wh_sfpsetman_i((void *)instrn_buffer, 0xAA, 0, 0, z, 1);
    a = __builtin_rvtt_wh_sfpsetman_i((void *)instrn_buffer, 0xAA, 0, 0, a, 1);
    __builtin_rvtt_wh_sfpstore((void *)instrn_buffer, x, 0, 2, 1, 0, 0);
    __builtin_rvtt_wh_sfpstore((void *)instrn_buffer, y, 0, 2, 10, 0, 0);
    __builtin_rvtt_wh_sfpstore((void *)instrn_buffer, z, 0, 2, 20, 0, 0);
    __builtin_rvtt_wh_sfpstore((void *)instrn_buffer, a, 0, 2, 30, 0, 0);
}

void setman_i_reg(short int value)
{
    v64sf x = __builtin_rvtt_wh_sfpload((void *)instrn_buffer, 1, 0, 10, 0, 0);
    v64sf y = __builtin_rvtt_wh_sfpload((void *)instrn_buffer, 1, 0, 20, 0, 0);
    v64sf z = __builtin_rvtt_wh_sfpload((void *)instrn_buffer, 1, 0, 30, 0, 0);
    v64sf a = __builtin_rvtt_wh_sfpload((void *)instrn_buffer, 1, 0, 40, 0, 0);
    x = __builtin_rvtt_wh_sfpsetman_i((void *)instrn_buffer, value+0, 0, 0, x, 1);
    y = __builtin_rvtt_wh_sfpsetman_i((void *)instrn_buffer, value+1, 0, 0, y, 1);
    z = __builtin_rvtt_wh_sfpsetman_i((void *)instrn_buffer, value+2, 0, 0, z, 1);
    a = __builtin_rvtt_wh_sfpsetman_i((void *)instrn_buffer, value+3, 0, 0, a, 1);
    __builtin_rvtt_wh_sfpstore((void *)instrn_buffer, x, 0, 2, 1, 0, 0);
    __builtin_rvtt_wh_sfpstore((void *)instrn_buffer, y, 0, 2, 10, 0, 0);
    __builtin_rvtt_wh_sfpstore((void *)instrn_buffer, z, 0, 2, 20, 0, 0);
    __builtin_rvtt_wh_sfpstore((void *)instrn_buffer, a, 0, 2, 30, 0, 0);
}

void setman_i_both(short int value)
{
    v64sf x = __builtin_rvtt_wh_sfpload((void *)instrn_buffer, 1, 0, 10, 0, 0);
    v64sf y = __builtin_rvtt_wh_sfpload((void *)instrn_buffer, 1, 0, 20, 0, 0);
    v64sf z = __builtin_rvtt_wh_sfpload((void *)instrn_buffer, 1, 0, 30, 0, 0);
    v64sf a = __builtin_rvtt_wh_sfpload((void *)instrn_buffer, 1, 0, 40, 0, 0);
    x = __builtin_rvtt_wh_sfpsetman_i((void *)instrn_buffer, 0xBB, 0, 0, x, 1);
    y = __builtin_rvtt_wh_sfpsetman_i((void *)instrn_buffer, 0XBB, 0, 0, y, 1);
    z = __builtin_rvtt_wh_sfpsetman_i((void *)instrn_buffer, value+0, 0, 0, z, 1);
    a = __builtin_rvtt_wh_sfpsetman_i((void *)instrn_buffer, value+1, 0, 0, a, 1);
    __builtin_rvtt_wh_sfpstore((void *)instrn_buffer, x, 0, 2, 1, 0, 0);
    __builtin_rvtt_wh_sfpstore((void *)instrn_buffer, y, 0, 2, 10, 0, 0);
    __builtin_rvtt_wh_sfpstore((void *)instrn_buffer, z, 0, 2, 20, 0, 0);
    __builtin_rvtt_wh_sfpstore((void *)instrn_buffer, a, 0, 2, 30, 0, 0);
}

void setsgn_i_imm(short int value)
{
    v64sf x = __builtin_rvtt_wh_sfpload((void *)instrn_buffer, 1, 0, 10, 0, 0);
    v64sf y = __builtin_rvtt_wh_sfpload((void *)instrn_buffer, 1, 0, 20, 0, 0);
    v64sf z = __builtin_rvtt_wh_sfpload((void *)instrn_buffer, 1, 0, 30, 0, 0);
    v64sf a = __builtin_rvtt_wh_sfpload((void *)instrn_buffer, 1, 0, 40, 0, 0);
    x = __builtin_rvtt_wh_sfpsetsgn_i((void *)instrn_buffer, 0x7AA, 0, 0, x);
    y = __builtin_rvtt_wh_sfpsetsgn_i((void *)instrn_buffer, 0x7AA, 0, 0, y);
    z = __builtin_rvtt_wh_sfpsetsgn_i((void *)instrn_buffer, 0x7AA, 0, 0, z);
    a = __builtin_rvtt_wh_sfpsetsgn_i((void *)instrn_buffer, 0x7AA, 0, 0, a);
    __builtin_rvtt_wh_sfpstore((void *)instrn_buffer, x, 0, 2, 1, 0, 0);
    __builtin_rvtt_wh_sfpstore((void *)instrn_buffer, y, 0, 2, 10, 0, 0);
    __builtin_rvtt_wh_sfpstore((void *)instrn_buffer, z, 0, 2, 20, 0, 0);
    __builtin_rvtt_wh_sfpstore((void *)instrn_buffer, a, 0, 2, 30, 0, 0);
}

void setsgn_i_reg(unsigned int value)
{
    v64sf x = __builtin_rvtt_wh_sfpload((void *)instrn_buffer, 1, 0, 10, 0, 0);
    v64sf y = __builtin_rvtt_wh_sfpload((void *)instrn_buffer, 1, 0, 20, 0, 0);
    v64sf z = __builtin_rvtt_wh_sfpload((void *)instrn_buffer, 1, 0, 30, 0, 0);
    v64sf a = __builtin_rvtt_wh_sfpload((void *)instrn_buffer, 1, 0, 40, 0, 0);
    x = __builtin_rvtt_wh_sfpsetsgn_i((void *)instrn_buffer, value+0, 0, 0, x);
    y = __builtin_rvtt_wh_sfpsetsgn_i((void *)instrn_buffer, value+1, 0, 0, y);
    z = __builtin_rvtt_wh_sfpsetsgn_i((void *)instrn_buffer, value+2, 0, 0, z);
    a = __builtin_rvtt_wh_sfpsetsgn_i((void *)instrn_buffer, value+3, 0, 0, a);
    __builtin_rvtt_wh_sfpstore((void *)instrn_buffer, x, 0, 2, 1, 0, 0);
    __builtin_rvtt_wh_sfpstore((void *)instrn_buffer, y, 0, 2, 10, 0, 0);
    __builtin_rvtt_wh_sfpstore((void *)instrn_buffer, z, 0, 2, 20, 0, 0);
    __builtin_rvtt_wh_sfpstore((void *)instrn_buffer, a, 0, 2, 30, 0, 0);
}

void setsgn_i_both(short int value)
{
    v64sf x = __builtin_rvtt_wh_sfpload((void *)instrn_buffer, 1, 0, 10, 0, 0);
    v64sf y = __builtin_rvtt_wh_sfpload((void *)instrn_buffer, 1, 0, 20, 0, 0);
    v64sf z = __builtin_rvtt_wh_sfpload((void *)instrn_buffer, 1, 0, 30, 0, 0);
    v64sf a = __builtin_rvtt_wh_sfpload((void *)instrn_buffer, 1, 0, 40, 0, 0);
    x = __builtin_rvtt_wh_sfpsetsgn_i((void *)instrn_buffer, 0x7BB, 0, 0, x);
    y = __builtin_rvtt_wh_sfpsetsgn_i((void *)instrn_buffer, 0X7BB, 0, 0, y);
    z = __builtin_rvtt_wh_sfpsetsgn_i((void *)instrn_buffer, value+0, 0, 0, z);
    a = __builtin_rvtt_wh_sfpsetsgn_i((void *)instrn_buffer, value+1, 0, 0, a);
    __builtin_rvtt_wh_sfpstore((void *)instrn_buffer, x, 0, 2, 1, 0, 0);
    __builtin_rvtt_wh_sfpstore((void *)instrn_buffer, y, 0, 2, 10, 0, 0);
    __builtin_rvtt_wh_sfpstore((void *)instrn_buffer, z, 0, 2, 20, 0, 0);
    __builtin_rvtt_wh_sfpstore((void *)instrn_buffer, a, 0, 2, 30, 0, 0);
}

void multi_reuse(int value)
{
    v64sf x;
    x = __builtin_rvtt_wh_sfpload(nullptr, 0, 0, 0, 0, 0);
    x = __builtin_rvtt_wh_sfpsetsgn_i(nullptr, value, 0, 0, x);
    x = __builtin_rvtt_wh_sfpsetsgn_i(nullptr, value, 0, 0, x);
    x = __builtin_rvtt_wh_sfpsetsgn_i(nullptr, value, 0, 0, x);
    x = __builtin_rvtt_wh_sfpsetsgn_i(nullptr, value, 0, 0, x);
    __builtin_rvtt_wh_sfpstore((void *)instrn_buffer, x, 0, 2, 0, 0, 0);
}

void loop_macro(int value)
{
    for (int i = 0; i < 4; i++) {
        macro(value);
    }
}

void loop_compiler(int value)
{
    for (int i = 0; i < 4; i++) {
        load_reg(value);
    }
}

int main(int argc, char* argv[])
{
    macro(argc);
    macro_checked(argc);

    load_imm(argc);
    load_reg(argc);
    load_both(argc);

    loadi_imm(argc);
    loadi_reg(argc);
    loadi_both(argc);

    store_imm(argc);
    store_reg(argc);
    store_both(argc);

    store_r_imm(argc);
    store_r_reg(argc);
    store_r_both(argc);

    addi_imm(argc);
    addi_reg(argc);
    addi_both(argc);

    xiadd_i_imm(argc);
    xiadd_i_reg(argc);
    xiadd_i_both(argc);

    shft_i_imm(argc);
    shft_i_reg(argc);
    shft_i_both(argc);

    divp2_imm(argc);
    divp2_reg(argc);
    divp2_both(argc);

    setexp_i_imm(argc);
    setexp_i_reg(argc);
    setexp_i_both(argc);

    setman_i_imm(argc);
    setman_i_reg(argc);
    setman_i_both(argc);

    setsgn_i_imm(argc);
    setsgn_i_reg(argc);
    setsgn_i_both(argc);

    multi_reuse(argc);

    loop_macro(argc);
    loop_compiler(argc);

    printf("%p\n", instrn_buffer);
}

