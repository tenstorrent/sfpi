typedef float v64sf __attribute__((vector_size(64*4)));

void setcc()
{
    v64sf a;
    a = __builtin_riscv_sfpload(nullptr, 1, 32);

    __builtin_riscv_sfppushc();
    __builtin_riscv_sfpsetcc_v(a, 12);
    __builtin_riscv_sfpcompc();
    __builtin_riscv_sfppopc();
}
