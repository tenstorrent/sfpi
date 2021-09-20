typedef float v64sf __attribute__((vector_size(64*4)));

// Ensures that there is no prologue/epilogue in the generated assembly.
void limits()  __attribute__((naked));

constexpr int offset = 0;

void limits()
{
    v64sf a;

    a = __builtin_riscv_sfpload(0, 0, 0x0000 + offset);
    a = __builtin_riscv_sfpload(0, 0, 0xFFFF + offset);

    __builtin_riscv_sfpstore_v(nullptr, a, 0, 0x0000 + offset);
    __builtin_riscv_sfpstore_v(nullptr, a, 0, 0xFFFF + offset);

    a = __builtin_riscv_sfploadi(nullptr, 2, 0x0000 + offset);
    a = __builtin_riscv_sfploadi(nullptr, 2, 0xFFFF + offset);
    a = __builtin_riscv_sfploadi(nullptr, 4, -32768 + offset);
    a = __builtin_riscv_sfploadi(nullptr, 4,  32767 + offset);

    // Should be unsigned
    a = __builtin_riscv_sfpdivp2(0x07FF + offset, a, 0);
    a = __builtin_riscv_sfpdivp2(-1 + offset, a, 0);
    a = __builtin_riscv_sfpdivp2(-2048 + offset, a, 0);

    // Signed
    a = __builtin_riscv_sfpiadd_i(nullptr, a, 0x0FFF + offset, 1);
    a = __builtin_riscv_sfpiadd_i(nullptr, a, 0x07FF + offset, 1);

    // doesn't error on too big/small
    a = __builtin_riscv_sfpiadd_i(nullptr, a, -2048 + offset, 1);
    a = __builtin_riscv_sfpiadd_i(nullptr, a, -1 + offset, 1);


    // Signed
    a = __builtin_riscv_sfpshft_i(a, 0X07FF + offset);
    a = __builtin_riscv_sfpshft_i(a, -1 + offset);
    a = __builtin_riscv_sfpshft_i(a, -2048 + offset);

    // Should be unsigned
    a = __builtin_riscv_sfpsetexp_i(0x07FF + offset, a);

    // Should be unsigned 
    a = __builtin_riscv_sfpsetman_i(0x07FF + offset, a);

    // Unsigned, doesn't check limit
    a = __builtin_riscv_sfpsetsgn_i(0x0FFF + offset, a);
}
