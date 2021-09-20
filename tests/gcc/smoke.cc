typedef float v64sf __attribute__((vector_size(64*4)));

// Ensures that there is no prologue/epilogue in the generated assembly.
void smoke()  __attribute__((naked));
void smokier()  __attribute__((naked));

constexpr unsigned int SFPMOV_MOD1_COMPSIGN = 1;
constexpr unsigned int SFPABS_MOD1_FLOAT = 1;
constexpr unsigned int SFPEXEXP_MOD1_DEBIAS = 0;
constexpr unsigned int SFPEXEXP_MOD1_NODEBIAS = 1;
constexpr unsigned int SFPEXMAN_MOD1_PAD8 = 1;


void smokier()
{
    v64sf a = __builtin_riscv_sfploadl2(3);
    v64sf b = __builtin_riscv_sfploadl0(3);
    v64sf c = __builtin_riscv_sfploadl1(3);
    v64sf d = __builtin_riscv_sfpload(nullptr, 1, 20);

    d = __builtin_riscv_sfpmad_vvv(a, b, c, 2);
    d = __builtin_riscv_sfplut(b, c, a, d, 12);

    __builtin_riscv_sfpstore_v(nullptr, a, 2, 20);
    __builtin_riscv_sfpstore_v(nullptr, b, 2, 20);
    __builtin_riscv_sfpstore_v(nullptr, c, 2, 20);
    __builtin_riscv_sfpstore_v(nullptr, d, 2, 20);
}

void smoke()
{
    v64sf a, b, c, d;

    a = __builtin_riscv_sfpload(nullptr, 1, 0);
    b = __builtin_riscv_sfpload(nullptr, 1, 16);
    c = __builtin_riscv_sfpload(nullptr, 1, 32);
    d = __builtin_riscv_sfpmad_vvv(a, b, c, 2);
    d = __builtin_riscv_sfpmad_vvr(a, b, 6, 2);
    d = __builtin_riscv_sfpmad_vrv(a, 7, c, 2);
    d = __builtin_riscv_sfpmad_vrr(a, 7, 6, 2);
    d = __builtin_riscv_sfpmad_rvv(8, b, c, 2);
    d = __builtin_riscv_sfpmad_rvr(8, b, 6, 2);
    d = __builtin_riscv_sfpmad_rrv(8, 7, c, 2);
    d = __builtin_riscv_sfpmad_rrr(8, 7, 6, 2);

    __builtin_riscv_sfpencc(2, 12);
    __builtin_riscv_sfppushc();
    __builtin_riscv_sfppopc();
    __builtin_riscv_sfpsetcc_v(a, 12);
    __builtin_riscv_sfpsetcc_i(3, 12);

    a = __builtin_riscv_sfploadi(nullptr, 0, 12);
    a = __builtin_riscv_sfploadi(nullptr, 0, -12);
    __builtin_riscv_sfpstore_v(nullptr, a, 2, 0);
    __builtin_riscv_sfpstore_v(nullptr, a, 2, 4);
    __builtin_riscv_sfpstore_r(nullptr, 13, 2, 4);
    v64sf e = __builtin_riscv_sfpmad_vvv(a, b, c, 2);
    __builtin_riscv_sfpcompc();
    __builtin_riscv_sfpstore_v(nullptr, e, 2, 4);

    __builtin_riscv_sfpnop();

    v64sf v1, v2;
    __builtin_riscv_sfpstore_v(nullptr, b, 2, 1);
    v1 = __builtin_riscv_sfpload(nullptr, 2, 0);
    v2 = __builtin_riscv_sfpmov(v1, SFPMOV_MOD1_COMPSIGN);

    __builtin_riscv_sfpstore_v(nullptr, c, 2, 2);
    __builtin_riscv_sfpstore_v(nullptr, d, 2, 3);
    __builtin_riscv_sfpstore_v(nullptr, v2, 2, 10);

    v2 = __builtin_riscv_sfpexexp(v1, SFPEXEXP_MOD1_DEBIAS);
    v2 = __builtin_riscv_sfpexman(v1, SFPEXMAN_MOD1_PAD8);

    v2 = __builtin_riscv_sfpsetexp_i(23, v1);
    v2 = __builtin_riscv_sfpsetexp_v(v2, v1);

    v2 = __builtin_riscv_sfpsetman_i(23, v1);
    v2 = __builtin_riscv_sfpsetman_v(v2, v1);

    v2 = __builtin_riscv_sfpabs(v1, SFPABS_MOD1_FLOAT);
    v2 = __builtin_riscv_sfpand(v2, v1);

    v2 = __builtin_riscv_sfpor(v2, v1);
    v2 = __builtin_riscv_sfpnot(v1);

    v2 = __builtin_riscv_sfpmuli(v2, 32, 1);
    v2 = __builtin_riscv_sfpaddi(nullptr, v2, 32, 0);

    v2 = __builtin_riscv_sfpdivp2(32, v1, 1);

    v2 = __builtin_riscv_sfplz(v1, 2);

    v2 = __builtin_riscv_sfpshft_i(v2, 10);
    v2 = __builtin_riscv_sfpshft_v(v2, v1);

    v1 = __builtin_riscv_sfpiadd_i(nullptr, v1, 10, 4);
    v2 = __builtin_riscv_sfpiadd_v(v2, v1, 8);

    v2 = __builtin_riscv_sfpsetsgn_i(10, v1);
    v2 = __builtin_riscv_sfpsetsgn_v(v2, v1);

    __builtin_riscv_sfpstore_v(nullptr, v1, 2, 6);
    __builtin_riscv_sfpstore_v(nullptr, v2, 2, 8);

    v2 = __builtin_riscv_sfpiadd_r(v2, 13, 4);

    v2 = __builtin_riscv_sfplut(v2, d, c, v1, 12);
}


void smoke_assign()
{
    v64sf a, b, orig_a;

    a = __builtin_riscv_sfpload(nullptr, 2, 0);
    b = __builtin_riscv_sfpload(nullptr, 2, 1);

    orig_a = a;
    a = b;
    a = __builtin_riscv_sfpsetexp_v(a, orig_a);
   
    __builtin_riscv_sfpstore_v(nullptr, a, 2, 6);
}
