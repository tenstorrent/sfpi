typedef float v64sf __attribute__((vector_size(64*4)));

// Ensures that there is no prologue/epilogue in the generated assembly.
void limits()  __attribute__((naked));

constexpr int offset = 0;

int main(int argc, char* argv[])
{
    v64sf a = __builtin_riscv_sfpload(nullptr, 0, 0x0000);

    a = __builtin_riscv_sfpload(nullptr, 0, 0x0000 + offset);
    a = __builtin_riscv_sfpload(nullptr, 0, 0xFFFF + offset);

    __builtin_riscv_sfpstore(nullptr, a, 0, 0x0000 + offset);
    __builtin_riscv_sfpstore(nullptr, a, 0, 0xFFFF + offset);

    // Signed 16 bit
    a = __builtin_riscv_sfploadi(nullptr, 2, 0x0000 + offset);
    a = __builtin_riscv_sfploadi(nullptr, 2, 0xFFFF + offset);
    a = __builtin_riscv_sfploadi(nullptr, 4, -32768 + offset);
    a = __builtin_riscv_sfploadi(nullptr, 4,  32767 + offset);

    // Unsigned 16 bit
    a = __builtin_riscv_sfpaddi(nullptr, a, 0x0000 + offset, 0);
    a = __builtin_riscv_sfpaddi(nullptr, a, 0xFFFF + offset, 0);

    // Unsigned 16 bit
    a = __builtin_riscv_sfpmuli(nullptr, a, 0x0000 + offset, 0);
    a = __builtin_riscv_sfpmuli(nullptr, a, 0xFFFF + offset, 0);

    // Signed 12 bit
    a = __builtin_riscv_sfpdivp2(nullptr, 0x07FF + offset, a, 0);
    a = __builtin_riscv_sfpdivp2(nullptr, -1 + offset, a, 0);
    a = __builtin_riscv_sfpdivp2(nullptr, -2048 + offset, a, 0);
    a = __builtin_riscv_sfpdivp2(nullptr, 0x07FF + offset, a, 1);
    a = __builtin_riscv_sfpdivp2(nullptr, -1 + offset, a, 1);
    a = __builtin_riscv_sfpdivp2(nullptr, -2048 + offset, a, 1);

    // Signed 12 bit
    __builtin_riscv_sfppushc();
    a = __builtin_riscv_sfpiadd_i(nullptr, a, 0x07FF + offset, 1);
    a = __builtin_riscv_sfpiadd_i(nullptr, a, -1 + offset, 1);
    a = __builtin_riscv_sfpiadd_i(nullptr, a, -2048 + offset, 1);
    __builtin_riscv_sfppopc();

    // Signed 12 bit
    a = __builtin_riscv_sfpshft_i(nullptr, a, 0x07FF + offset);
    a = __builtin_riscv_sfpshft_i(nullptr, a, -1 + offset);
    a = __builtin_riscv_sfpshft_i(nullptr, a, -2048 + offset);

    // Unsigned 12 bit
    a = __builtin_riscv_sfpsetexp_i(nullptr, 0 + offset, a);
    a = __builtin_riscv_sfpsetexp_i(nullptr, 0x0FFF + offset, a);

    // Unsigned 12 bit
    a = __builtin_riscv_sfpsetman_i(nullptr, 0 + offset, a);
    a = __builtin_riscv_sfpsetman_i(nullptr, 0x0FFF + offset, a);

    // Unsigned 12 bit
    a = __builtin_riscv_sfpsetsgn_i(nullptr, 0 + offset, a);
    a = __builtin_riscv_sfpsetsgn_i(nullptr, 0x0FFF + offset, a);
}
