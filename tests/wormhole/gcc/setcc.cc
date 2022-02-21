typedef float v64sf __attribute__((vector_size(64*4)));

void setcc()
{
    v64sf a;
    a = __builtin_rvtt_wh_sfpload(nullptr, 1, 32);

    __builtin_rvtt_wh_sfppushc();
    __builtin_rvtt_wh_sfpsetcc_v(a, 12);
    __builtin_rvtt_wh_sfpcompc();
    __builtin_rvtt_wh_sfppopc();
}
