// Test immediate (TT_) forms of instructions

typedef float v64sf __attribute__((vector_size(64*4)));

namespace ckernel {
    volatile unsigned int *instrn_buffer;
};
using namespace ckernel;

#include "ckernel_ops.h"

void macro(int value)
{
    TT_SFPLOADI(2, 6, value);
    TT_SFPLOAD(2, 6, value);
    TT_SFPSTORE(2, 6, value);
    TT_SFPADDI(value, 20, 10);
    TT_SFPIADD(value, 2, 3, 5);
}

void load_imm(short int addr)
{
    v64sf x = __builtin_riscv_sfpload((void *)instrn_buffer, 1, 10);
    v64sf y = __builtin_riscv_sfpload((void *)instrn_buffer, 1, 20);
    v64sf z = __builtin_riscv_sfpload((void *)instrn_buffer, 1, 30);
    v64sf a = __builtin_riscv_sfpload((void *)instrn_buffer, 1, 40);
    __builtin_riscv_sfpstore_v((void *)instrn_buffer, x, 1, 1);
    __builtin_riscv_sfpstore_v((void *)instrn_buffer, y, 1, 10);
    __builtin_riscv_sfpstore_v((void *)instrn_buffer, z, 1, 20);
    __builtin_riscv_sfpstore_v((void *)instrn_buffer, a, 1, 30);
}

void load_reg(short int addr)
{
    v64sf x = __builtin_riscv_sfpload((void *)instrn_buffer, 1, addr);
    v64sf y = __builtin_riscv_sfpload((void *)instrn_buffer, 1, addr+10);
    v64sf z = __builtin_riscv_sfpload((void *)instrn_buffer, 1, addr+20);
    v64sf a = __builtin_riscv_sfpload((void *)instrn_buffer, 1, addr+30);
    __builtin_riscv_sfpstore_v((void *)instrn_buffer, x, 1, 1);
    __builtin_riscv_sfpstore_v((void *)instrn_buffer, y, 1, 10);
    __builtin_riscv_sfpstore_v((void *)instrn_buffer, z, 1, 20);
    __builtin_riscv_sfpstore_v((void *)instrn_buffer, a, 1, 30);
}

void load_both(short int addr)
{
    v64sf x = __builtin_riscv_sfpload((void *)instrn_buffer, 1, 10);
    v64sf y = __builtin_riscv_sfpload((void *)instrn_buffer, 1, 20);
    v64sf z = __builtin_riscv_sfpload((void *)instrn_buffer, 1, addr);
    v64sf a = __builtin_riscv_sfpload((void *)instrn_buffer, 1, addr+10);
    __builtin_riscv_sfpstore_v((void *)instrn_buffer, x, 1, 1);
    __builtin_riscv_sfpstore_v((void *)instrn_buffer, y, 1, 10);
    __builtin_riscv_sfpstore_v((void *)instrn_buffer, z, 1, 20);
    __builtin_riscv_sfpstore_v((void *)instrn_buffer, a, 1, 30);
}

void loadi_imm(short int value)
{
    v64sf x = __builtin_riscv_sfploadi((void *)instrn_buffer, 0, 10);
    v64sf y = __builtin_riscv_sfploadi((void *)instrn_buffer, 4, 10);
    v64sf z = __builtin_riscv_sfploadi((void *)instrn_buffer, 8, 20);
    v64sf a = __builtin_riscv_sfploadi((void *)instrn_buffer, 12, 30);
    __builtin_riscv_sfpstore_v((void *)instrn_buffer, x, 1, 1);
    __builtin_riscv_sfpstore_v((void *)instrn_buffer, y, 1, 10);
    __builtin_riscv_sfpstore_v((void *)instrn_buffer, z, 1, 20);
    __builtin_riscv_sfpstore_v((void *)instrn_buffer, a, 1, 30);
}

void loadi_reg(short int value)
{
    v64sf x = __builtin_riscv_sfploadi((void *)instrn_buffer, 0, value);
    v64sf y = __builtin_riscv_sfploadi((void *)instrn_buffer, 4, value+1);
    v64sf z = __builtin_riscv_sfploadi((void *)instrn_buffer, 8, value+2);
    v64sf a = __builtin_riscv_sfploadi((void *)instrn_buffer, 12, value+3);
    __builtin_riscv_sfpstore_v((void *)instrn_buffer, x, 1, 1);
    __builtin_riscv_sfpstore_v((void *)instrn_buffer, y, 1, 10);
    __builtin_riscv_sfpstore_v((void *)instrn_buffer, z, 1, 20);
    __builtin_riscv_sfpstore_v((void *)instrn_buffer, a, 1, 30);
}

void loadi_both(short int value)
{
    v64sf x = __builtin_riscv_sfploadi((void *)instrn_buffer, 0, 10);
    v64sf y = __builtin_riscv_sfploadi((void *)instrn_buffer, 4, 20);
    v64sf z = __builtin_riscv_sfploadi((void *)instrn_buffer, 8, value);
    v64sf a = __builtin_riscv_sfploadi((void *)instrn_buffer, 12, value+1);
    __builtin_riscv_sfpstore_v((void *)instrn_buffer, x, 1, 1);
    __builtin_riscv_sfpstore_v((void *)instrn_buffer, y, 1, 10);
    __builtin_riscv_sfpstore_v((void *)instrn_buffer, z, 1, 20);
    __builtin_riscv_sfpstore_v((void *)instrn_buffer, a, 1, 30);
}

void store_v_imm(short int addr)
{
    v64sf x = __builtin_riscv_sfpload((void *)instrn_buffer, 1, 10);
    __builtin_riscv_sfpstore_v((void *)instrn_buffer, x, 1, 10);
    __builtin_riscv_sfpstore_v((void *)instrn_buffer, x, 1, 20);
    __builtin_riscv_sfpstore_v((void *)instrn_buffer, x, 1, 30);
    __builtin_riscv_sfpstore_v((void *)instrn_buffer, x, 1, 40);
}

void store_v_reg(short int addr)
{
    v64sf x = __builtin_riscv_sfpload((void *)instrn_buffer, 1, 10);
    __builtin_riscv_sfpstore_v((void *)instrn_buffer, x, 1, addr);
    __builtin_riscv_sfpstore_v((void *)instrn_buffer, x, 1, addr+1);
    __builtin_riscv_sfpstore_v((void *)instrn_buffer, x, 1, addr+2);
    __builtin_riscv_sfpstore_v((void *)instrn_buffer, x, 1, addr+3);
}

void store_v_both(short int addr)
{
    v64sf x = __builtin_riscv_sfpload((void *)instrn_buffer, 1, 10);
    __builtin_riscv_sfpstore_v((void *)instrn_buffer, x, 1, 10);
    __builtin_riscv_sfpstore_v((void *)instrn_buffer, x, 1, 20);
    __builtin_riscv_sfpstore_v((void *)instrn_buffer, x, 1, addr+1);
    __builtin_riscv_sfpstore_v((void *)instrn_buffer, x, 1, addr+2);
}

void store_r_imm(short int addr)
{
    __builtin_riscv_sfpstore_r((void *)instrn_buffer, 13, 1, 10);
    __builtin_riscv_sfpstore_r((void *)instrn_buffer, 13, 1, 20);
    __builtin_riscv_sfpstore_r((void *)instrn_buffer, 13, 1, 10);
    __builtin_riscv_sfpstore_r((void *)instrn_buffer, 13, 1, 20);
}

void store_r_reg(short int addr)
{
    __builtin_riscv_sfpstore_r((void *)instrn_buffer, 13, 1, addr);
    __builtin_riscv_sfpstore_r((void *)instrn_buffer, 13, 1, addr+1);
    __builtin_riscv_sfpstore_r((void *)instrn_buffer, 13, 1, addr+2);
    __builtin_riscv_sfpstore_r((void *)instrn_buffer, 13, 1, addr+3);
}

void store_r_both(short int addr)
{
    __builtin_riscv_sfpstore_r((void *)instrn_buffer, 13, 1, 10);
    __builtin_riscv_sfpstore_r((void *)instrn_buffer, 13, 1, 20);
    __builtin_riscv_sfpstore_r((void *)instrn_buffer, 13, 1, addr);
    __builtin_riscv_sfpstore_r((void *)instrn_buffer, 13, 1, addr+1);
}

void addi_imm(short int value)
{
    v64sf x = __builtin_riscv_sfpload((void *)instrn_buffer, 1, 10);
    v64sf y = __builtin_riscv_sfpload((void *)instrn_buffer, 1, 20);
    v64sf z = __builtin_riscv_sfpload((void *)instrn_buffer, 1, 30);
    v64sf a = __builtin_riscv_sfpload((void *)instrn_buffer, 1, 40);
    x = __builtin_riscv_sfpaddi((void *)instrn_buffer, x, 1, 1);
    y = __builtin_riscv_sfpaddi((void *)instrn_buffer, y, 2, 10);
    z = __builtin_riscv_sfpaddi((void *)instrn_buffer, z, 3, 5);
    a = __builtin_riscv_sfpaddi((void *)instrn_buffer, a, 4, 7);
    __builtin_riscv_sfpstore_v((void *)instrn_buffer, x, 1, 1);
    __builtin_riscv_sfpstore_v((void *)instrn_buffer, y, 1, 10);
    __builtin_riscv_sfpstore_v((void *)instrn_buffer, z, 1, 20);
    __builtin_riscv_sfpstore_v((void *)instrn_buffer, a, 1, 30);
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
    __builtin_riscv_sfpstore_v((void *)instrn_buffer, x, 1, 1);
    __builtin_riscv_sfpstore_v((void *)instrn_buffer, y, 1, 10);
    __builtin_riscv_sfpstore_v((void *)instrn_buffer, z, 1, 20);
    __builtin_riscv_sfpstore_v((void *)instrn_buffer, a, 1, 30);
}

void addi_both(short int value)
{
    v64sf x = __builtin_riscv_sfpload((void *)instrn_buffer, 1, 10);
    v64sf y = __builtin_riscv_sfpload((void *)instrn_buffer, 1, 20);
    v64sf z = __builtin_riscv_sfpload((void *)instrn_buffer, 1, 30);
    v64sf a = __builtin_riscv_sfpload((void *)instrn_buffer, 1, 40);
    x = __builtin_riscv_sfpaddi((void *)instrn_buffer, x, 1, 1);
    y = __builtin_riscv_sfpaddi((void *)instrn_buffer, y, 2, 10);
    z = __builtin_riscv_sfpaddi((void *)instrn_buffer, z, value, 5);
    a = __builtin_riscv_sfpaddi((void *)instrn_buffer, a, value+1, 7);
    __builtin_riscv_sfpstore_v((void *)instrn_buffer, x, 1, 1);
    __builtin_riscv_sfpstore_v((void *)instrn_buffer, y, 1, 10);
    __builtin_riscv_sfpstore_v((void *)instrn_buffer, z, 1, 20);
    __builtin_riscv_sfpstore_v((void *)instrn_buffer, a, 1, 30);
}

void iadd_i_imm(short int value)
{
    v64sf x = __builtin_riscv_sfpload((void *)instrn_buffer, 1, 10);
    v64sf y = __builtin_riscv_sfpload((void *)instrn_buffer, 1, 20);
    v64sf z = __builtin_riscv_sfpload((void *)instrn_buffer, 1, 30);
    v64sf a = __builtin_riscv_sfpload((void *)instrn_buffer, 1, 40);
    x = __builtin_riscv_sfpiadd_i((void *)instrn_buffer, x, 1, 1);
    y = __builtin_riscv_sfpiadd_i((void *)instrn_buffer, y, 2, 1);
    z = __builtin_riscv_sfpiadd_i((void *)instrn_buffer, z, 3, 1);
    a = __builtin_riscv_sfpiadd_i((void *)instrn_buffer, a, 4, 1);
    __builtin_riscv_sfpstore_v((void *)instrn_buffer, x, 1, 1);
    __builtin_riscv_sfpstore_v((void *)instrn_buffer, y, 1, 10);
    __builtin_riscv_sfpstore_v((void *)instrn_buffer, z, 1, 20);
    __builtin_riscv_sfpstore_v((void *)instrn_buffer, a, 1, 30);
}

void iadd_i_reg(short int value)
{
    v64sf x = __builtin_riscv_sfpload((void *)instrn_buffer, 1, 10);
    v64sf y = __builtin_riscv_sfpload((void *)instrn_buffer, 1, 20);
    v64sf z = __builtin_riscv_sfpload((void *)instrn_buffer, 1, 30);
    v64sf a = __builtin_riscv_sfpload((void *)instrn_buffer, 1, 40);
    x = __builtin_riscv_sfpiadd_i((void *)instrn_buffer, x, value+0, 1);
    y = __builtin_riscv_sfpiadd_i((void *)instrn_buffer, y, value+1, 1);
    z = __builtin_riscv_sfpiadd_i((void *)instrn_buffer, z, value+2, 1);
    a = __builtin_riscv_sfpiadd_i((void *)instrn_buffer, a, value+3, 1);
    __builtin_riscv_sfpstore_v((void *)instrn_buffer, x, 1, 1);
    __builtin_riscv_sfpstore_v((void *)instrn_buffer, y, 1, 10);
    __builtin_riscv_sfpstore_v((void *)instrn_buffer, z, 1, 20);
    __builtin_riscv_sfpstore_v((void *)instrn_buffer, a, 1, 30);
}

void iadd_i_both(short int value)
{
    v64sf x = __builtin_riscv_sfpload((void *)instrn_buffer, 1, 10);
    v64sf y = __builtin_riscv_sfpload((void *)instrn_buffer, 1, 20);
    v64sf z = __builtin_riscv_sfpload((void *)instrn_buffer, 1, 30);
    v64sf a = __builtin_riscv_sfpload((void *)instrn_buffer, 1, 40);
    x = __builtin_riscv_sfpiadd_i((void *)instrn_buffer, x, 1,       1);
    y = __builtin_riscv_sfpiadd_i((void *)instrn_buffer, y, 2,       1);
    z = __builtin_riscv_sfpiadd_i((void *)instrn_buffer, z, value+0, 1);
    a = __builtin_riscv_sfpiadd_i((void *)instrn_buffer, a, value+1, 1);
    __builtin_riscv_sfpstore_v((void *)instrn_buffer, x, 1, 1);
    __builtin_riscv_sfpstore_v((void *)instrn_buffer, y, 1, 10);
    __builtin_riscv_sfpstore_v((void *)instrn_buffer, z, 1, 20);
    __builtin_riscv_sfpstore_v((void *)instrn_buffer, a, 1, 30);
}
