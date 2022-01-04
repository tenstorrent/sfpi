#include <stdio.h>
typedef float v64sf __attribute__((vector_size(64*4)));

// Ensures that there is no prologue/epilogue in the generated assembly.
void limits()  __attribute__((naked));

#ifndef LIMIT_OFFSET
#define LIMIT_OFFSET 0
#endif

#ifndef LIMIT_OFFSET12
#define LIMIT_OFFSET12 LIMIT_OFFSET
#endif

#ifndef LIMIT_OFFSET16
#define LIMIT_OFFSET16 LIMIT_OFFSET
#endif

constexpr int offset12 = LIMIT_OFFSET12;
constexpr int offset16 = LIMIT_OFFSET16;

int main(int argc, char* argv[])
{
    v64sf a = __builtin_riscv_sfpload(nullptr, 0, 0x0000);

    a = __builtin_riscv_sfpload(nullptr, 0, 0x0000 + offset16);
    a = __builtin_riscv_sfpload(nullptr, 0, 0xFFFF + offset16);

    __builtin_riscv_sfpstore(nullptr, a, 0, 0x0000 + offset16);
    __builtin_riscv_sfpstore(nullptr, a, 0, 0xFFFF + offset16);

    // Signed 16 bit
    a = __builtin_riscv_sfploadi(nullptr, 2, 0x0000 + offset16);
    a = __builtin_riscv_sfploadi(nullptr, 2, 0xFFFF + offset16);
    a = __builtin_riscv_sfploadi(nullptr, 4, -32768 + offset16);
    a = __builtin_riscv_sfploadi(nullptr, 4,  32767 + offset16);

    // Unsigned 16 bit
    a = __builtin_riscv_sfpaddi(nullptr, a, 0x0000 + offset16, 0);
    a = __builtin_riscv_sfpaddi(nullptr, a, 0xFFFF + offset16, 0);

    // Unsigned 16 bit
    a = __builtin_riscv_sfpmuli(nullptr, a, 0x0000 + offset16, 0);
    a = __builtin_riscv_sfpmuli(nullptr, a, 0xFFFF + offset16, 0);

    // Signed 12 bit
    a = __builtin_riscv_sfpdivp2(nullptr, 0x07FF + offset12, a, 0);
    a = __builtin_riscv_sfpdivp2(nullptr, -1 + offset12, a, 0);
    a = __builtin_riscv_sfpdivp2(nullptr, -2048 + offset12, a, 0);
    a = __builtin_riscv_sfpdivp2(nullptr, 0x07FF + offset12, a, 1);
    a = __builtin_riscv_sfpdivp2(nullptr, -1 + offset12, a, 1);
    a = __builtin_riscv_sfpdivp2(nullptr, -2048 + offset12, a, 1);

    // Signed 12 bit
    a = __builtin_riscv_sfpiadd_i(nullptr, a, 0x07FF + offset12, 5);
    a = __builtin_riscv_sfpiadd_i(nullptr, a, -1 + offset12, 5);
    a = __builtin_riscv_sfpiadd_i(nullptr, a, -2048 + offset12, 5);

    // Signed 16 bit
    a = __builtin_riscv_sfpiadd_i(nullptr, a, 0x7FFF + offset12, 5);
    a = __builtin_riscv_sfpiadd_i(nullptr, a, -1 + offset12, 5);
    a = __builtin_riscv_sfpiadd_i(nullptr, a, -2048 + offset12, 5);

    // Signed 12 bit
    a = __builtin_riscv_sfpshft_i(nullptr, a, 0x07FF + offset12);
    a = __builtin_riscv_sfpshft_i(nullptr, a, -1 + offset12);
    a = __builtin_riscv_sfpshft_i(nullptr, a, -2048 + offset12);

    // Unsigned 12 bit
    a = __builtin_riscv_sfpsetexp_i(nullptr, 0 + offset12, a);
    a = __builtin_riscv_sfpsetexp_i(nullptr, 0x0FFF + offset12, a);

    // Unsigned 12 bit
    a = __builtin_riscv_sfpsetman_i(nullptr, 0 + offset12, a);
    a = __builtin_riscv_sfpsetman_i(nullptr, 0x0FFF + offset12, a);

    // Unsigned 12 bit
    a = __builtin_riscv_sfpsetsgn_i(nullptr, 0 + offset12, a);
    a = __builtin_riscv_sfpsetsgn_i(nullptr, 0x0FFF + offset12, a);

    // Unsigned 12 bit
    __builtin_riscv_sfppushc();
    __builtin_riscv_sfpsetcc_i(0 + offset12, 1);
    __builtin_riscv_sfpsetcc_i(0x0FFF + offset12, 1);
    __builtin_riscv_sfppopc();

    // Unsigned 12 bit
    __builtin_riscv_sfpencc(0 + offset12, 2);
    __builtin_riscv_sfpencc(0x0FFF + offset12, 2);
}
