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
    TT_SFPLOAD(2, 6, value);
    TT_SFPLOADI(2, 6, value);
    TT_SFPSTORE(2, 6, value);
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
    TT_SFPLOAD(2, 6, value & 0xFFFF);
    TT_SFPLOADI(2, 6, value & 0xFFFF);
    TT_SFPSTORE(2, 6, value & 0xFFFF);
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
    TT_SFPLOAD(2, 6, value & 0xFFFF);
    TT_SFPLOAD(2, 6, (value + 1) & 0xFFFF);
    TT_SFPLOAD(2, 6, (value + 2) & 0xFFFF);
    TT_SFPLOAD(2, 6, (value + 3) & 0xFFFF);

    TT_SFPSTORE(2, 6, value & 0xFFFF);
    TT_SFPSTORE(2, 6, (value + 1) & 0xFFFF);
    TT_SFPSTORE(2, 6, (value + 2) & 0xFFFF);
    TT_SFPSTORE(2, 6, (value + 3) & 0xFFFF);
}

void load_imm(short int addr)
{
    v64sf x = __builtin_riscv_sfpload((void *)instrn_buffer, 1, 10);
    v64sf y = __builtin_riscv_sfpload((void *)instrn_buffer, 1, 20);
    v64sf z = __builtin_riscv_sfpload((void *)instrn_buffer, 1, 30);
    v64sf a = __builtin_riscv_sfpload((void *)instrn_buffer, 1, 40);
    __builtin_riscv_sfpstore((void *)instrn_buffer, x, 2, 1);
    __builtin_riscv_sfpstore((void *)instrn_buffer, y, 2, 10);
    __builtin_riscv_sfpstore((void *)instrn_buffer, z, 2, 20);
    __builtin_riscv_sfpstore((void *)instrn_buffer, a, 2, 30);
}

void load_reg(short int addr)
{
    v64sf x = __builtin_riscv_sfpload((void *)instrn_buffer, 1, addr);
    v64sf y = __builtin_riscv_sfpload((void *)instrn_buffer, 1, addr+10);
    v64sf z = __builtin_riscv_sfpload((void *)instrn_buffer, 1, addr+20);
    v64sf a = __builtin_riscv_sfpload((void *)instrn_buffer, 1, addr+30);
    __builtin_riscv_sfpstore((void *)instrn_buffer, x, 2, 1);
    __builtin_riscv_sfpstore((void *)instrn_buffer, y, 2, 10);
    __builtin_riscv_sfpstore((void *)instrn_buffer, z, 2, 20);
    __builtin_riscv_sfpstore((void *)instrn_buffer, a, 2, 30);
}

void load_both(short int addr)
{
    v64sf x = __builtin_riscv_sfpload((void *)instrn_buffer, 1, 10);
    v64sf y = __builtin_riscv_sfpload((void *)instrn_buffer, 1, 20);
    v64sf z = __builtin_riscv_sfpload((void *)instrn_buffer, 1, addr);
    v64sf a = __builtin_riscv_sfpload((void *)instrn_buffer, 1, addr+10);
    __builtin_riscv_sfpstore((void *)instrn_buffer, x, 2, 1);
    __builtin_riscv_sfpstore((void *)instrn_buffer, y, 2, 10);
    __builtin_riscv_sfpstore((void *)instrn_buffer, z, 2, 20);
    __builtin_riscv_sfpstore((void *)instrn_buffer, a, 2, 30);
}

void loadi_imm(short int value)
{
    v64sf x = __builtin_riscv_sfploadi((void *)instrn_buffer, 0, 10);
    v64sf y = __builtin_riscv_sfploadi((void *)instrn_buffer, 4, 10);
    v64sf z = __builtin_riscv_sfploadi((void *)instrn_buffer, 8, 20);
    v64sf a = __builtin_riscv_sfploadi((void *)instrn_buffer, 12, 30);
    __builtin_riscv_sfpstore((void *)instrn_buffer, x, 2, 1);
    __builtin_riscv_sfpstore((void *)instrn_buffer, y, 2, 10);
    __builtin_riscv_sfpstore((void *)instrn_buffer, z, 2, 20);
    __builtin_riscv_sfpstore((void *)instrn_buffer, a, 2, 30);
}

void loadi_reg(short int value)
{
    v64sf x = __builtin_riscv_sfploadi((void *)instrn_buffer, 0, value);
    v64sf y = __builtin_riscv_sfploadi((void *)instrn_buffer, 4, value+1);
    v64sf z = __builtin_riscv_sfploadi((void *)instrn_buffer, 8, value+2);
    v64sf a = __builtin_riscv_sfploadi((void *)instrn_buffer, 12, value+3);
    __builtin_riscv_sfpstore((void *)instrn_buffer, x, 2, 1);
    __builtin_riscv_sfpstore((void *)instrn_buffer, y, 2, 10);
    __builtin_riscv_sfpstore((void *)instrn_buffer, z, 2, 20);
    __builtin_riscv_sfpstore((void *)instrn_buffer, a, 2, 30);
}

void loadi_both(short int value)
{
    v64sf x = __builtin_riscv_sfploadi((void *)instrn_buffer, 0, 10);
    v64sf y = __builtin_riscv_sfploadi((void *)instrn_buffer, 4, 20);
    v64sf z = __builtin_riscv_sfploadi((void *)instrn_buffer, 8, value);
    v64sf a = __builtin_riscv_sfploadi((void *)instrn_buffer, 12, value+1);
    __builtin_riscv_sfpstore((void *)instrn_buffer, x, 2, 1);
    __builtin_riscv_sfpstore((void *)instrn_buffer, y, 2, 10);
    __builtin_riscv_sfpstore((void *)instrn_buffer, z, 2, 20);
    __builtin_riscv_sfpstore((void *)instrn_buffer, a, 2, 30);
}

void store_imm(short int addr)
{
    v64sf x = __builtin_riscv_sfpload((void *)instrn_buffer, 1, 10);
    __builtin_riscv_sfpstore((void *)instrn_buffer, x, 2, 10);
    __builtin_riscv_sfpstore((void *)instrn_buffer, x, 2, 20);
    __builtin_riscv_sfpstore((void *)instrn_buffer, x, 2, 30);
    __builtin_riscv_sfpstore((void *)instrn_buffer, x, 2, 40);
}

void store_reg(short int addr)
{
    v64sf x = __builtin_riscv_sfpload((void *)instrn_buffer, 1, 10);
    __builtin_riscv_sfpstore((void *)instrn_buffer, x, 2, addr);
    __builtin_riscv_sfpstore((void *)instrn_buffer, x, 2, addr+1);
    __builtin_riscv_sfpstore((void *)instrn_buffer, x, 2, addr+2);
    __builtin_riscv_sfpstore((void *)instrn_buffer, x, 2, addr+3);
}

void store_both(short int addr)
{
    v64sf x = __builtin_riscv_sfpload((void *)instrn_buffer, 1, 10);
    __builtin_riscv_sfpstore((void *)instrn_buffer, x, 2, 10);
    __builtin_riscv_sfpstore((void *)instrn_buffer, x, 2, 20);
    __builtin_riscv_sfpstore((void *)instrn_buffer, x, 2, addr+1);
    __builtin_riscv_sfpstore((void *)instrn_buffer, x, 2, addr+2);
}

void store_r_imm(short int addr)
{
    v64sf lr13 = __builtin_riscv_sfpassignlr(13);
    __builtin_riscv_sfpstore((void *)instrn_buffer, lr13, 2, 10);
    __builtin_riscv_sfpstore((void *)instrn_buffer, lr13, 2, 20);
    __builtin_riscv_sfpstore((void *)instrn_buffer, lr13, 2, 10);
    __builtin_riscv_sfpstore((void *)instrn_buffer, lr13, 2, 20);
}

void store_r_reg(short int addr)
{
    v64sf lr13 = __builtin_riscv_sfpassignlr(13);
    __builtin_riscv_sfpstore((void *)instrn_buffer, lr13, 2, addr);
    __builtin_riscv_sfpstore((void *)instrn_buffer, lr13, 2, addr+1);
    __builtin_riscv_sfpstore((void *)instrn_buffer, lr13, 2, addr+2);
    __builtin_riscv_sfpstore((void *)instrn_buffer, lr13, 2, addr+3);
}

void store_r_both(short int addr)
{
    v64sf lr13 = __builtin_riscv_sfpassignlr(13);
    __builtin_riscv_sfpstore((void *)instrn_buffer, lr13, 2, 10);
    __builtin_riscv_sfpstore((void *)instrn_buffer, lr13, 2, 20);
    __builtin_riscv_sfpstore((void *)instrn_buffer, lr13, 2, addr);
    __builtin_riscv_sfpstore((void *)instrn_buffer, lr13, 2, addr+1);
}

void addi_imm(short int value)
{
    v64sf x = __builtin_riscv_sfpload((void *)instrn_buffer, 1, 10);
    v64sf y = __builtin_riscv_sfpload((void *)instrn_buffer, 1, 20);
    v64sf z = __builtin_riscv_sfpload((void *)instrn_buffer, 1, 30);
    v64sf a = __builtin_riscv_sfpload((void *)instrn_buffer, 1, 40);
    x = __builtin_riscv_sfpaddi((void *)instrn_buffer, x, 1, 1);
    y = __builtin_riscv_sfpaddi((void *)instrn_buffer, y, 2, 1);
    z = __builtin_riscv_sfpaddi((void *)instrn_buffer, z, 3, 3);
    a = __builtin_riscv_sfpaddi((void *)instrn_buffer, a, 4, 3);
    __builtin_riscv_sfpstore((void *)instrn_buffer, x, 2, 1);
    __builtin_riscv_sfpstore((void *)instrn_buffer, y, 2, 10);
    __builtin_riscv_sfpstore((void *)instrn_buffer, z, 2, 20);
    __builtin_riscv_sfpstore((void *)instrn_buffer, a, 2, 30);
}

void addi_reg(short int value)
{
    v64sf x = __builtin_riscv_sfpload((void *)instrn_buffer, 1, 10);
    v64sf y = __builtin_riscv_sfpload((void *)instrn_buffer, 1, 20);
    v64sf z = __builtin_riscv_sfpload((void *)instrn_buffer, 1, 30);
    v64sf a = __builtin_riscv_sfpload((void *)instrn_buffer, 1, 40);
    x = __builtin_riscv_sfpaddi((void *)instrn_buffer, x, value, 1);
    y = __builtin_riscv_sfpaddi((void *)instrn_buffer, y, value+1, 10);
    z = __builtin_riscv_sfpaddi((void *)instrn_buffer, z, value+2, 5);
    a = __builtin_riscv_sfpaddi((void *)instrn_buffer, a, value+3, 7);
    __builtin_riscv_sfpstore((void *)instrn_buffer, x, 2, 1);
    __builtin_riscv_sfpstore((void *)instrn_buffer, y, 2, 10);
    __builtin_riscv_sfpstore((void *)instrn_buffer, z, 2, 20);
    __builtin_riscv_sfpstore((void *)instrn_buffer, a, 2, 30);
}

void addi_both(short int value)
{
    v64sf x = __builtin_riscv_sfpload((void *)instrn_buffer, 1, 10);
    v64sf y = __builtin_riscv_sfpload((void *)instrn_buffer, 1, 20);
    v64sf z = __builtin_riscv_sfpload((void *)instrn_buffer, 1, 30);
    v64sf a = __builtin_riscv_sfpload((void *)instrn_buffer, 1, 40);
    x = __builtin_riscv_sfpaddi((void *)instrn_buffer, x, 1, 1);
    y = __builtin_riscv_sfpaddi((void *)instrn_buffer, y, 2, 3);
    z = __builtin_riscv_sfpaddi((void *)instrn_buffer, z, value, 5);
    a = __builtin_riscv_sfpaddi((void *)instrn_buffer, a, value+1, 7);
    __builtin_riscv_sfpstore((void *)instrn_buffer, x, 2, 1);
    __builtin_riscv_sfpstore((void *)instrn_buffer, y, 2, 10);
    __builtin_riscv_sfpstore((void *)instrn_buffer, z, 2, 20);
    __builtin_riscv_sfpstore((void *)instrn_buffer, a, 2, 30);
}

void iadd_i_imm(short int value)
{
    v64sf x = __builtin_riscv_sfpload((void *)instrn_buffer, 1, 10);
    v64sf y = __builtin_riscv_sfpload((void *)instrn_buffer, 1, 20);
    v64sf z = __builtin_riscv_sfpload((void *)instrn_buffer, 1, 30);
    v64sf a = __builtin_riscv_sfpload((void *)instrn_buffer, 1, 40);
    x = __builtin_riscv_sfpiadd_i((void *)instrn_buffer, x, 1, 1);
    y = __builtin_riscv_sfpiadd_i((void *)instrn_buffer, y, 2, 2);
    z = __builtin_riscv_sfpiadd_i((void *)instrn_buffer, z, 3, 4);
    a = __builtin_riscv_sfpiadd_i((void *)instrn_buffer, a, 4, 5);
    __builtin_riscv_sfpstore((void *)instrn_buffer, x, 2, 1);
    __builtin_riscv_sfpstore((void *)instrn_buffer, y, 2, 10);
    __builtin_riscv_sfpstore((void *)instrn_buffer, z, 2, 20);
    __builtin_riscv_sfpstore((void *)instrn_buffer, a, 2, 30);
}

void iadd_i_reg(short int value)
{
    v64sf x = __builtin_riscv_sfpload((void *)instrn_buffer, 1, 10);
    v64sf y = __builtin_riscv_sfpload((void *)instrn_buffer, 1, 20);
    v64sf z = __builtin_riscv_sfpload((void *)instrn_buffer, 1, 30);
    v64sf a = __builtin_riscv_sfpload((void *)instrn_buffer, 1, 40);
    x = __builtin_riscv_sfpiadd_i((void *)instrn_buffer, x, value+0, 1);
    y = __builtin_riscv_sfpiadd_i((void *)instrn_buffer, y, value+1, 2);
    z = __builtin_riscv_sfpiadd_i((void *)instrn_buffer, z, value+2, 4);
    a = __builtin_riscv_sfpiadd_i((void *)instrn_buffer, a, value+3, 5);
    __builtin_riscv_sfpstore((void *)instrn_buffer, x, 2, 1);
    __builtin_riscv_sfpstore((void *)instrn_buffer, y, 2, 10);
    __builtin_riscv_sfpstore((void *)instrn_buffer, z, 2, 20);
    __builtin_riscv_sfpstore((void *)instrn_buffer, a, 2, 30);
}

void iadd_i_both(short int value)
{
    v64sf x = __builtin_riscv_sfpload((void *)instrn_buffer, 1, 10);
    v64sf y = __builtin_riscv_sfpload((void *)instrn_buffer, 1, 20);
    v64sf z = __builtin_riscv_sfpload((void *)instrn_buffer, 1, 30);
    v64sf a = __builtin_riscv_sfpload((void *)instrn_buffer, 1, 40);
    x = __builtin_riscv_sfpiadd_i((void *)instrn_buffer, x, 1,       1);
    y = __builtin_riscv_sfpiadd_i((void *)instrn_buffer, y, 2,       2);
    z = __builtin_riscv_sfpiadd_i((void *)instrn_buffer, z, value+0, 4);
    a = __builtin_riscv_sfpiadd_i((void *)instrn_buffer, a, value+1, 5);
    __builtin_riscv_sfpstore((void *)instrn_buffer, x, 2, 1);
    __builtin_riscv_sfpstore((void *)instrn_buffer, y, 2, 10);
    __builtin_riscv_sfpstore((void *)instrn_buffer, z, 2, 20);
    __builtin_riscv_sfpstore((void *)instrn_buffer, a, 2, 30);
}

void shft_i_imm(short int value)
{
    v64sf x = __builtin_riscv_sfpload((void *)instrn_buffer, 1, 10);
    v64sf y = __builtin_riscv_sfpload((void *)instrn_buffer, 1, 20);
    v64sf z = __builtin_riscv_sfpload((void *)instrn_buffer, 1, 30);
    v64sf a = __builtin_riscv_sfpload((void *)instrn_buffer, 1, 40);
    x = __builtin_riscv_sfpshft_i((void *)instrn_buffer, x, 1);
    y = __builtin_riscv_sfpshft_i((void *)instrn_buffer, y, 2);
    z = __builtin_riscv_sfpshft_i((void *)instrn_buffer, z, 3);
    a = __builtin_riscv_sfpshft_i((void *)instrn_buffer, a, 4);
    __builtin_riscv_sfpstore((void *)instrn_buffer, x, 2, 1);
    __builtin_riscv_sfpstore((void *)instrn_buffer, y, 2, 10);
    __builtin_riscv_sfpstore((void *)instrn_buffer, z, 2, 20);
    __builtin_riscv_sfpstore((void *)instrn_buffer, a, 2, 30);
}

void shft_i_reg(short int value)
{
    v64sf x = __builtin_riscv_sfpload((void *)instrn_buffer, 1, 10);
    v64sf y = __builtin_riscv_sfpload((void *)instrn_buffer, 1, 20);
    v64sf z = __builtin_riscv_sfpload((void *)instrn_buffer, 1, 30);
    v64sf a = __builtin_riscv_sfpload((void *)instrn_buffer, 1, 40);
    x = __builtin_riscv_sfpshft_i((void *)instrn_buffer, x, value+0);
    y = __builtin_riscv_sfpshft_i((void *)instrn_buffer, y, value+1);
    z = __builtin_riscv_sfpshft_i((void *)instrn_buffer, z, value+2);
    a = __builtin_riscv_sfpshft_i((void *)instrn_buffer, a, value+3);
    __builtin_riscv_sfpstore((void *)instrn_buffer, x, 2, 1);
    __builtin_riscv_sfpstore((void *)instrn_buffer, y, 2, 10);
    __builtin_riscv_sfpstore((void *)instrn_buffer, z, 2, 20);
    __builtin_riscv_sfpstore((void *)instrn_buffer, a, 2, 30);
}

void shft_i_both(short int value)
{
    v64sf x = __builtin_riscv_sfpload((void *)instrn_buffer, 1, 10);
    v64sf y = __builtin_riscv_sfpload((void *)instrn_buffer, 1, 20);
    v64sf z = __builtin_riscv_sfpload((void *)instrn_buffer, 1, 30);
    v64sf a = __builtin_riscv_sfpload((void *)instrn_buffer, 1, 40);
    x = __builtin_riscv_sfpshft_i((void *)instrn_buffer, x, 1);
    y = __builtin_riscv_sfpshft_i((void *)instrn_buffer, y, 2);
    z = __builtin_riscv_sfpshft_i((void *)instrn_buffer, z, value+0);
    a = __builtin_riscv_sfpshft_i((void *)instrn_buffer, a, value+1);
    __builtin_riscv_sfpstore((void *)instrn_buffer, x, 2, 1);
    __builtin_riscv_sfpstore((void *)instrn_buffer, y, 2, 10);
    __builtin_riscv_sfpstore((void *)instrn_buffer, z, 2, 20);
    __builtin_riscv_sfpstore((void *)instrn_buffer, a, 2, 30);
}

void divp2_imm(short int value)
{
    v64sf x = __builtin_riscv_sfpload((void *)instrn_buffer, 1, 10);
    v64sf y = __builtin_riscv_sfpload((void *)instrn_buffer, 1, 20);
    v64sf z = __builtin_riscv_sfpload((void *)instrn_buffer, 1, 30);
    v64sf a = __builtin_riscv_sfpload((void *)instrn_buffer, 1, 40);
    x = __builtin_riscv_sfpdivp2((void *)instrn_buffer, 0x1A, z, 0);
    y = __builtin_riscv_sfpdivp2((void *)instrn_buffer, 0x2A, y, 1);
    z = __builtin_riscv_sfpdivp2((void *)instrn_buffer, 0x3A, z, 0);
    a = __builtin_riscv_sfpdivp2((void *)instrn_buffer, 0x4A, a, 1);
    __builtin_riscv_sfpstore((void *)instrn_buffer, x, 2, 1);
    __builtin_riscv_sfpstore((void *)instrn_buffer, y, 2, 10);
    __builtin_riscv_sfpstore((void *)instrn_buffer, z, 2, 20);
    __builtin_riscv_sfpstore((void *)instrn_buffer, a, 2, 30);
}

void divp2_reg(short int value)
{
    v64sf x = __builtin_riscv_sfpload((void *)instrn_buffer, 1, 10);
    v64sf y = __builtin_riscv_sfpload((void *)instrn_buffer, 1, 20);
    v64sf z = __builtin_riscv_sfpload((void *)instrn_buffer, 1, 30);
    v64sf a = __builtin_riscv_sfpload((void *)instrn_buffer, 1, 40);
    x = __builtin_riscv_sfpdivp2((void *)instrn_buffer, value+0, a, 0);
    y = __builtin_riscv_sfpdivp2((void *)instrn_buffer, value+1, y, 1);
    z = __builtin_riscv_sfpdivp2((void *)instrn_buffer, value+2, z, 0);
    a = __builtin_riscv_sfpdivp2((void *)instrn_buffer, value+3, a, 1);
    __builtin_riscv_sfpstore((void *)instrn_buffer, x, 2, 1);
    __builtin_riscv_sfpstore((void *)instrn_buffer, y, 2, 10);
    __builtin_riscv_sfpstore((void *)instrn_buffer, z, 2, 20);
    __builtin_riscv_sfpstore((void *)instrn_buffer, a, 2, 30);
}

void divp2_both(short int value)
{
    v64sf x = __builtin_riscv_sfpload((void *)instrn_buffer, 1, 10);
    v64sf y = __builtin_riscv_sfpload((void *)instrn_buffer, 1, 20);
    v64sf z = __builtin_riscv_sfpload((void *)instrn_buffer, 1, 30);
    v64sf a = __builtin_riscv_sfpload((void *)instrn_buffer, 1, 40);
    x = __builtin_riscv_sfpdivp2((void *)instrn_buffer, 0x1B, a, 0);
    y = __builtin_riscv_sfpdivp2((void *)instrn_buffer, 0X1B, y, 1);
    z = __builtin_riscv_sfpdivp2((void *)instrn_buffer, value+0, z, 0);
    a = __builtin_riscv_sfpdivp2((void *)instrn_buffer, value+1, a, 1);
    __builtin_riscv_sfpstore((void *)instrn_buffer, x, 2, 1);
    __builtin_riscv_sfpstore((void *)instrn_buffer, y, 2, 10);
    __builtin_riscv_sfpstore((void *)instrn_buffer, z, 2, 20);
    __builtin_riscv_sfpstore((void *)instrn_buffer, a, 2, 30);
}

void setexp_i_imm(short int value)
{
    v64sf x = __builtin_riscv_sfpload((void *)instrn_buffer, 1, 10);
    v64sf y = __builtin_riscv_sfpload((void *)instrn_buffer, 1, 20);
    v64sf z = __builtin_riscv_sfpload((void *)instrn_buffer, 1, 30);
    v64sf a = __builtin_riscv_sfpload((void *)instrn_buffer, 1, 40);
    x = __builtin_riscv_sfpsetexp_i((void *)instrn_buffer, 0xAA, x);
    y = __builtin_riscv_sfpsetexp_i((void *)instrn_buffer, 0xAA, y);
    z = __builtin_riscv_sfpsetexp_i((void *)instrn_buffer, 0xAA, z);
    a = __builtin_riscv_sfpsetexp_i((void *)instrn_buffer, 0xAA, a);
    __builtin_riscv_sfpstore((void *)instrn_buffer, x, 2, 1);
    __builtin_riscv_sfpstore((void *)instrn_buffer, y, 2, 10);
    __builtin_riscv_sfpstore((void *)instrn_buffer, z, 2, 20);
    __builtin_riscv_sfpstore((void *)instrn_buffer, a, 2, 30);
}

void setexp_i_reg(short int value)
{
    v64sf x = __builtin_riscv_sfpload((void *)instrn_buffer, 1, 10);
    v64sf y = __builtin_riscv_sfpload((void *)instrn_buffer, 1, 20);
    v64sf z = __builtin_riscv_sfpload((void *)instrn_buffer, 1, 30);
    v64sf a = __builtin_riscv_sfpload((void *)instrn_buffer, 1, 40);
    x = __builtin_riscv_sfpsetexp_i((void *)instrn_buffer, value+0, x);
    y = __builtin_riscv_sfpsetexp_i((void *)instrn_buffer, value+1, y);
    z = __builtin_riscv_sfpsetexp_i((void *)instrn_buffer, value+2, z);
    a = __builtin_riscv_sfpsetexp_i((void *)instrn_buffer, value+3, a);
    __builtin_riscv_sfpstore((void *)instrn_buffer, x, 2, 1);
    __builtin_riscv_sfpstore((void *)instrn_buffer, y, 2, 10);
    __builtin_riscv_sfpstore((void *)instrn_buffer, z, 2, 20);
    __builtin_riscv_sfpstore((void *)instrn_buffer, a, 2, 30);
}

void setexp_i_both(short int value)
{
    v64sf x = __builtin_riscv_sfpload((void *)instrn_buffer, 1, 10);
    v64sf y = __builtin_riscv_sfpload((void *)instrn_buffer, 1, 20);
    v64sf z = __builtin_riscv_sfpload((void *)instrn_buffer, 1, 30);
    v64sf a = __builtin_riscv_sfpload((void *)instrn_buffer, 1, 40);
    x = __builtin_riscv_sfpsetexp_i((void *)instrn_buffer, 0xBB, x);
    y = __builtin_riscv_sfpsetexp_i((void *)instrn_buffer, 0XBB, y);
    z = __builtin_riscv_sfpsetexp_i((void *)instrn_buffer, value+0, z);
    a = __builtin_riscv_sfpsetexp_i((void *)instrn_buffer, value+1, a);
    __builtin_riscv_sfpstore((void *)instrn_buffer, x, 2, 1);
    __builtin_riscv_sfpstore((void *)instrn_buffer, y, 2, 10);
    __builtin_riscv_sfpstore((void *)instrn_buffer, z, 2, 20);
    __builtin_riscv_sfpstore((void *)instrn_buffer, a, 2, 30);
}

void setman_i_imm(short int value)
{
    v64sf x = __builtin_riscv_sfpload((void *)instrn_buffer, 1, 10);
    v64sf y = __builtin_riscv_sfpload((void *)instrn_buffer, 1, 20);
    v64sf z = __builtin_riscv_sfpload((void *)instrn_buffer, 1, 30);
    v64sf a = __builtin_riscv_sfpload((void *)instrn_buffer, 1, 40);
    x = __builtin_riscv_sfpsetman_i((void *)instrn_buffer, 0xAA, x);
    y = __builtin_riscv_sfpsetman_i((void *)instrn_buffer, 0xAA, y);
    z = __builtin_riscv_sfpsetman_i((void *)instrn_buffer, 0xAA, z);
    a = __builtin_riscv_sfpsetman_i((void *)instrn_buffer, 0xAA, a);
    __builtin_riscv_sfpstore((void *)instrn_buffer, x, 2, 1);
    __builtin_riscv_sfpstore((void *)instrn_buffer, y, 2, 10);
    __builtin_riscv_sfpstore((void *)instrn_buffer, z, 2, 20);
    __builtin_riscv_sfpstore((void *)instrn_buffer, a, 2, 30);
}

void setman_i_reg(short int value)
{
    v64sf x = __builtin_riscv_sfpload((void *)instrn_buffer, 1, 10);
    v64sf y = __builtin_riscv_sfpload((void *)instrn_buffer, 1, 20);
    v64sf z = __builtin_riscv_sfpload((void *)instrn_buffer, 1, 30);
    v64sf a = __builtin_riscv_sfpload((void *)instrn_buffer, 1, 40);
    x = __builtin_riscv_sfpsetman_i((void *)instrn_buffer, value+0, x);
    y = __builtin_riscv_sfpsetman_i((void *)instrn_buffer, value+1, y);
    z = __builtin_riscv_sfpsetman_i((void *)instrn_buffer, value+2, z);
    a = __builtin_riscv_sfpsetman_i((void *)instrn_buffer, value+3, a);
    __builtin_riscv_sfpstore((void *)instrn_buffer, x, 2, 1);
    __builtin_riscv_sfpstore((void *)instrn_buffer, y, 2, 10);
    __builtin_riscv_sfpstore((void *)instrn_buffer, z, 2, 20);
    __builtin_riscv_sfpstore((void *)instrn_buffer, a, 2, 30);
}

void setman_i_both(short int value)
{
    v64sf x = __builtin_riscv_sfpload((void *)instrn_buffer, 1, 10);
    v64sf y = __builtin_riscv_sfpload((void *)instrn_buffer, 1, 20);
    v64sf z = __builtin_riscv_sfpload((void *)instrn_buffer, 1, 30);
    v64sf a = __builtin_riscv_sfpload((void *)instrn_buffer, 1, 40);
    x = __builtin_riscv_sfpsetman_i((void *)instrn_buffer, 0xBB, x);
    y = __builtin_riscv_sfpsetman_i((void *)instrn_buffer, 0XBB, y);
    z = __builtin_riscv_sfpsetman_i((void *)instrn_buffer, value+0, z);
    a = __builtin_riscv_sfpsetman_i((void *)instrn_buffer, value+1, a);
    __builtin_riscv_sfpstore((void *)instrn_buffer, x, 2, 1);
    __builtin_riscv_sfpstore((void *)instrn_buffer, y, 2, 10);
    __builtin_riscv_sfpstore((void *)instrn_buffer, z, 2, 20);
    __builtin_riscv_sfpstore((void *)instrn_buffer, a, 2, 30);
}

void setsgn_i_imm(short int value)
{
    v64sf x = __builtin_riscv_sfpload((void *)instrn_buffer, 1, 10);
    v64sf y = __builtin_riscv_sfpload((void *)instrn_buffer, 1, 20);
    v64sf z = __builtin_riscv_sfpload((void *)instrn_buffer, 1, 30);
    v64sf a = __builtin_riscv_sfpload((void *)instrn_buffer, 1, 40);
    x = __builtin_riscv_sfpsetsgn_i((void *)instrn_buffer, 0x7AA, x);
    y = __builtin_riscv_sfpsetsgn_i((void *)instrn_buffer, 0x7AA, y);
    z = __builtin_riscv_sfpsetsgn_i((void *)instrn_buffer, 0x7AA, z);
    a = __builtin_riscv_sfpsetsgn_i((void *)instrn_buffer, 0x7AA, a);
    __builtin_riscv_sfpstore((void *)instrn_buffer, x, 2, 1);
    __builtin_riscv_sfpstore((void *)instrn_buffer, y, 2, 10);
    __builtin_riscv_sfpstore((void *)instrn_buffer, z, 2, 20);
    __builtin_riscv_sfpstore((void *)instrn_buffer, a, 2, 30);
}

void setsgn_i_reg(unsigned int value)
{
    v64sf x = __builtin_riscv_sfpload((void *)instrn_buffer, 1, 10);
    v64sf y = __builtin_riscv_sfpload((void *)instrn_buffer, 1, 20);
    v64sf z = __builtin_riscv_sfpload((void *)instrn_buffer, 1, 30);
    v64sf a = __builtin_riscv_sfpload((void *)instrn_buffer, 1, 40);
    x = __builtin_riscv_sfpsetsgn_i((void *)instrn_buffer, value+0, x);
    y = __builtin_riscv_sfpsetsgn_i((void *)instrn_buffer, value+1, y);
    z = __builtin_riscv_sfpsetsgn_i((void *)instrn_buffer, value+2, z);
    a = __builtin_riscv_sfpsetsgn_i((void *)instrn_buffer, value+3, a);
    __builtin_riscv_sfpstore((void *)instrn_buffer, x, 2, 1);
    __builtin_riscv_sfpstore((void *)instrn_buffer, y, 2, 10);
    __builtin_riscv_sfpstore((void *)instrn_buffer, z, 2, 20);
    __builtin_riscv_sfpstore((void *)instrn_buffer, a, 2, 30);
}

void setsgn_i_both(short int value)
{
    v64sf x = __builtin_riscv_sfpload((void *)instrn_buffer, 1, 10);
    v64sf y = __builtin_riscv_sfpload((void *)instrn_buffer, 1, 20);
    v64sf z = __builtin_riscv_sfpload((void *)instrn_buffer, 1, 30);
    v64sf a = __builtin_riscv_sfpload((void *)instrn_buffer, 1, 40);
    x = __builtin_riscv_sfpsetsgn_i((void *)instrn_buffer, 0x7BB, x);
    y = __builtin_riscv_sfpsetsgn_i((void *)instrn_buffer, 0X7BB, y);
    z = __builtin_riscv_sfpsetsgn_i((void *)instrn_buffer, value+0, z);
    a = __builtin_riscv_sfpsetsgn_i((void *)instrn_buffer, value+1, a);
    __builtin_riscv_sfpstore((void *)instrn_buffer, x, 2, 1);
    __builtin_riscv_sfpstore((void *)instrn_buffer, y, 2, 10);
    __builtin_riscv_sfpstore((void *)instrn_buffer, z, 2, 20);
    __builtin_riscv_sfpstore((void *)instrn_buffer, a, 2, 30);
}

void multi_reuse(int value)
{
    v64sf x;
    x = __builtin_riscv_sfpload(nullptr, 0, 0);
    x = __builtin_riscv_sfpsetsgn_i(nullptr, value, x);
    x = __builtin_riscv_sfpsetsgn_i(nullptr, value, x);
    x = __builtin_riscv_sfpsetsgn_i(nullptr, value, x);
    x = __builtin_riscv_sfpsetsgn_i(nullptr, value, x);
    __builtin_riscv_sfpstore((void *)instrn_buffer, x, 2, 0);
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

    iadd_i_imm(argc);
    iadd_i_reg(argc);
    iadd_i_both(argc);

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

