#include <stdio.h>
typedef float v64sf __attribute__((vector_size(64*4)));

// Ensures that there is no prologue/epilogue in the generated assembly.
void limits()  __attribute__((naked));

#ifndef LIMIT_OFFSET
#define LIMIT_OFFSET 0
#endif

#ifndef PASS_OFFSET
#define PASS_OFFSET LIMIT_OFFSET
#endif

#ifndef FAIL_OFFSET
#define FAIL_OFFSET LIMIT_OFFSET
#endif

constexpr int pass_offset = PASS_OFFSET;
constexpr int fail_offset = FAIL_OFFSET;

int main(int argc, char* argv[])
{
    v64sf a = __builtin_rvtt_wh_sfpload(nullptr, 0, 0x0000);

    a = __builtin_rvtt_wh_sfpload(nullptr, 0, 0x0000 + fail_offset);
    a = __builtin_rvtt_wh_sfpload(nullptr, 0, 0xFFFF + fail_offset);

    __builtin_rvtt_wh_sfpstore(nullptr, a, 0, 0x0000 + fail_offset);
    __builtin_rvtt_wh_sfpstore(nullptr, a, 0, 0xFFFF + fail_offset);

    // Unsigned 16 bit
    a = __builtin_rvtt_wh_sfploadi_ex(nullptr, 2, 0x0000 + fail_offset);
    __builtin_rvtt_wh_sfpstore(nullptr, a, 0, 0);
    a = __builtin_rvtt_wh_sfploadi_ex(nullptr, 2, 0xFFFF + fail_offset);
    __builtin_rvtt_wh_sfpstore(nullptr, a, 0, 0);

    // Signed 16 bit
    a = __builtin_rvtt_wh_sfploadi_ex(nullptr, 4, -32768 + fail_offset);
    __builtin_rvtt_wh_sfpstore(nullptr, a, 0, 0);
    a = __builtin_rvtt_wh_sfploadi_ex(nullptr, 4,  32767 + fail_offset);
    __builtin_rvtt_wh_sfpstore(nullptr, a, 0, 0);

    // Signed 16-32 bit crossover
    a = __builtin_rvtt_wh_sfploadi_ex(nullptr, 16, -32768 + pass_offset);
    __builtin_rvtt_wh_sfpstore(nullptr, a, 0, 0);
    a = __builtin_rvtt_wh_sfploadi_ex(nullptr, 16,  32767 + pass_offset);
    __builtin_rvtt_wh_sfpstore(nullptr, a, 0, 0);
    a = __builtin_rvtt_wh_sfploadi_ex(nullptr, 16,  65535 + pass_offset);
    __builtin_rvtt_wh_sfpstore(nullptr, a, 0, 0);

    // Unsigned 16-32 bit crossover
    a = __builtin_rvtt_wh_sfploadi_ex(nullptr, 17,  0 + pass_offset);
    __builtin_rvtt_wh_sfpstore(nullptr, a, 0, 0);
    a = __builtin_rvtt_wh_sfploadi_ex(nullptr, 17,  0xFFFF + pass_offset);
    __builtin_rvtt_wh_sfpstore(nullptr, a, 0, 0);

    // Float values
    // Easy fp16b
    a = __builtin_rvtt_wh_sfploadi_ex(nullptr, 18,  0x3f800000);
    __builtin_rvtt_wh_sfpstore(nullptr, a, 0, 0);

    // fp16b
    a = __builtin_rvtt_wh_sfploadi_ex(nullptr, 18,  0x3faf0000);
    __builtin_rvtt_wh_sfpstore(nullptr, a, 0, 0);

    // 32 bit float
    a = __builtin_rvtt_wh_sfploadi_ex(nullptr, 18,  0x3faf0001);
    __builtin_rvtt_wh_sfpstore(nullptr, a, 0, 0);

    // fp16a
    a = __builtin_rvtt_wh_sfploadi_ex(nullptr, 18,  0x3fa6e000);
    __builtin_rvtt_wh_sfpstore(nullptr, a, 0, 0);

    // 32 bit float
    a = __builtin_rvtt_wh_sfploadi_ex(nullptr, 18,  0x3fa6e001);
    __builtin_rvtt_wh_sfpstore(nullptr, a, 0, 0);

    // Exp on the edge of fp16a
    a = __builtin_rvtt_wh_sfploadi_ex(nullptr, 18,  0x47a6e000);
    __builtin_rvtt_wh_sfpstore(nullptr, a, 0, 0);

    // 32 bit float
    a = __builtin_rvtt_wh_sfploadi_ex(nullptr, 18,  0x4826e000);
    __builtin_rvtt_wh_sfpstore(nullptr, a, 0, 0);

    // Exp on the edge of fp16a
    a = __builtin_rvtt_wh_sfploadi_ex(nullptr, 18,  0x3826e000);
    __builtin_rvtt_wh_sfpstore(nullptr, a, 0, 0);

    // 32 bit float
    a = __builtin_rvtt_wh_sfploadi_ex(nullptr, 18,  0x37a6e000);
    __builtin_rvtt_wh_sfpstore(nullptr, a, 0, 0);

    // Unsigned 16 bit
    a = __builtin_rvtt_wh_sfpaddi(nullptr, a, 0x0000 + fail_offset, 0);
    a = __builtin_rvtt_wh_sfpaddi(nullptr, a, 0xFFFF + fail_offset, 0);

    // Unsigned 16 bit
    a = __builtin_rvtt_wh_sfpmuli(nullptr, a, 0x0000 + fail_offset, 0);
    a = __builtin_rvtt_wh_sfpmuli(nullptr, a, 0xFFFF + fail_offset, 0);

    // Signed 12 bit
    a = __builtin_rvtt_wh_sfpdivp2(nullptr, 0x07FF + pass_offset, a, 0);
    a = __builtin_rvtt_wh_sfpdivp2(nullptr, -1 + pass_offset, a, 0);
    a = __builtin_rvtt_wh_sfpdivp2(nullptr, -2048 + pass_offset, a, 0);
    a = __builtin_rvtt_wh_sfpdivp2(nullptr, 0x07FF + pass_offset, a, 1);
    a = __builtin_rvtt_wh_sfpdivp2(nullptr, -1 + pass_offset, a, 1);
    a = __builtin_rvtt_wh_sfpdivp2(nullptr, -2048 + pass_offset, a, 1);

    // Signed 12 bit
    a = __builtin_rvtt_wh_sfpiadd_i(nullptr, a, 2047 + pass_offset, 5);
    a = __builtin_rvtt_wh_sfpiadd_i(nullptr, a, -1 + pass_offset, 5);
    a = __builtin_rvtt_wh_sfpiadd_i(nullptr, a, -2048 + pass_offset, 5);

    // Signed 16 bit
    a = __builtin_rvtt_wh_sfpiadd_i(nullptr, a, 32767 + pass_offset, 5);
    a = __builtin_rvtt_wh_sfpiadd_i(nullptr, a, -1 + pass_offset, 5);
    a = __builtin_rvtt_wh_sfpiadd_i(nullptr, a, -32768 + pass_offset, 5);

    // Signed 12 bit
    a = __builtin_rvtt_wh_sfpiadd_i_ex(nullptr, a, 2047 + pass_offset, 8);
    a = __builtin_rvtt_wh_sfpiadd_i_ex(nullptr, a, -1 + pass_offset, 8);
    a = __builtin_rvtt_wh_sfpiadd_i_ex(nullptr, a, -2048 + pass_offset, 8);

    // Signed 16 bit
    a = __builtin_rvtt_wh_sfpiadd_i_ex(nullptr, a, 32767 + pass_offset, 8);
    a = __builtin_rvtt_wh_sfpiadd_i_ex(nullptr, a, -1 + pass_offset, 8);
    a = __builtin_rvtt_wh_sfpiadd_i_ex(nullptr, a, -32768 + pass_offset, 8);

    // Signed 32 bit
    a = __builtin_rvtt_wh_sfpiadd_i_ex(nullptr, a, 65535 + pass_offset, 8);
    a = __builtin_rvtt_wh_sfpiadd_i_ex(nullptr, a, -65536 + pass_offset, 8);

    // Unsigned 12 bit
    a = __builtin_rvtt_wh_sfpiadd_i_ex(nullptr, a, 0 + pass_offset, 0);
    a = __builtin_rvtt_wh_sfpiadd_i_ex(nullptr, a, 4095 + pass_offset, 0);

    // Unsigned 16 bit
    a = __builtin_rvtt_wh_sfpiadd_i_ex(nullptr, a, 0 + pass_offset, 0);
    a = __builtin_rvtt_wh_sfpiadd_i_ex(nullptr, a, 65535 + pass_offset, 0);

    // Unsigned 32 bit
    a = __builtin_rvtt_wh_sfpiadd_i_ex(nullptr, a, 65535 + pass_offset, 0);
    a = __builtin_rvtt_wh_sfpiadd_i_ex(nullptr, a, 65536 + pass_offset, 0);

    // Signed 12 bit
    a = __builtin_rvtt_wh_sfpshft_i(nullptr, a, 0x07FF + pass_offset);
    a = __builtin_rvtt_wh_sfpshft_i(nullptr, a, -1 + pass_offset);
    a = __builtin_rvtt_wh_sfpshft_i(nullptr, a, -2048 + pass_offset);

    // Unsigned 12 bit
    a = __builtin_rvtt_wh_sfpsetexp_i(nullptr, 0 + pass_offset, a);
    a = __builtin_rvtt_wh_sfpsetexp_i(nullptr, 0x0FFF + pass_offset, a);

    // Unsigned 12 bit
    a = __builtin_rvtt_wh_sfpsetman_i(nullptr, 0 + pass_offset, a, 0);
    a = __builtin_rvtt_wh_sfpsetman_i(nullptr, 0x0FFF + pass_offset, a, 0);

    // Unsigned 16 bit
    a = __builtin_rvtt_wh_sfpsetman_i(nullptr, 0x1000 + pass_offset, a, 0);

    // Unsigned 12 bit
    a = __builtin_rvtt_wh_sfpsetsgn_i(nullptr, 0 + pass_offset, a);
    a = __builtin_rvtt_wh_sfpsetsgn_i(nullptr, 0x0FFF + pass_offset, a);

    // Unsigned 12 bit
    __builtin_rvtt_wh_sfppushc();
    __builtin_rvtt_wh_sfpsetcc_i(0 + pass_offset, 1);
    __builtin_rvtt_wh_sfpsetcc_i(0x0FFF + pass_offset, 1);
    __builtin_rvtt_wh_sfppopc();

    // Unsigned 12 bit
    __builtin_rvtt_wh_sfpencc(0 + pass_offset, 2);
    __builtin_rvtt_wh_sfpencc(0x0FFF + pass_offset, 2);

    __builtin_rvtt_wh_sfppushc();
    // 1.0 in different fmts
    __builtin_rvtt_wh_sfpscmp_ex(nullptr, a, 0x3f80, 9);
    __builtin_rvtt_wh_sfpscmp_ex(nullptr, a, 0x3f80, 17);
    __builtin_rvtt_wh_sfpscmp_ex(nullptr, a, 0x3f800000, 33);

    // -1.0 in different fmts
    __builtin_rvtt_wh_sfpscmp_ex(nullptr, a, 0xbf80, 9);
    __builtin_rvtt_wh_sfpscmp_ex(nullptr, a, 0xbf80, 17);
    __builtin_rvtt_wh_sfpscmp_ex(nullptr, a, 0xbf800000, 33);

    // Not a register in different formats
    __builtin_rvtt_wh_sfpscmp_ex(nullptr, a, 0xbfa0, 9);
    __builtin_rvtt_wh_sfpscmp_ex(nullptr, a, 0xbfa0, 17);
    __builtin_rvtt_wh_sfpscmp_ex(nullptr, a, 0xbfa00000, 33);
    __builtin_rvtt_wh_sfpscmp_ex(nullptr, a, 0x3fa6e001, 33);

    __builtin_rvtt_wh_sfppopc();
}
