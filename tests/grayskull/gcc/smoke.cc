typedef float v64sf __attribute__((vector_size(64*4)));

// Ensures that there is no prologue/epilogue in the generated assembly.
void smoke()  __attribute__((naked));
void smokier()  __attribute__((naked));

constexpr unsigned int SFPMOV_MOD1_COMPSIGN = 1;
constexpr unsigned int SFPABS_MOD1_FLOAT = 1;
constexpr unsigned int SFPEXEXP_MOD1_DEBIAS = 0;
constexpr unsigned int SFPEXEXP_MOD1_NODEBIAS = 1;
constexpr unsigned int SFPEXMAN_MOD1_PAD8 = 1;


void smoke()
{
    v64sf a, b, c, d;

    a = __builtin_rvtt_gs_sfpload(nullptr, 1, 0, 0, 0);
    b = __builtin_rvtt_gs_sfpload(nullptr, 1, 16, 0, 0);
    c = __builtin_rvtt_gs_sfpload(nullptr, 1, 32, 0, 0);

    v64sf lr6, lr7, lr8;
    lr6 = __builtin_rvtt_sfpassignlreg(6);
    lr7 = __builtin_rvtt_sfpassignlreg(7);
    lr8 = __builtin_rvtt_sfpassignlreg(8);

    d = __builtin_rvtt_gs_sfpmad(  a,   b,   c, 1);
    d = __builtin_rvtt_gs_sfpmad(  a,   b, lr6, 1);
    d = __builtin_rvtt_gs_sfpmad(  a, lr7,   c, 1);
    d = __builtin_rvtt_gs_sfpmad(  a, lr7, lr6, 1);
    d = __builtin_rvtt_gs_sfpmad(lr8,   b,   c, 1);
    d = __builtin_rvtt_gs_sfpmad(lr8,   b, lr6, 1);
    d = __builtin_rvtt_gs_sfpmad(lr8, lr7,   c, 1);
    d = __builtin_rvtt_gs_sfpmad(lr8, lr7, lr6, 1);

    d = __builtin_rvtt_gs_sfpmul(lr8, lr7, 1);
    d = __builtin_rvtt_gs_sfpadd(lr8, lr7, 1);

    __builtin_rvtt_gs_sfpencc(2, 8);

    int cond;
    a = __builtin_rvtt_gs_sfpload(nullptr, 1, 0, 0, 0);  // reg pressu, 0, 0re
    d = __builtin_rvtt_gs_sfpload(nullptr, 1, 0, 0, 0);  // reg pressu, 0, 0re
    __builtin_rvtt_gs_sfppushc();
    int top = __builtin_rvtt_sfpxvif();
    __builtin_rvtt_gs_sfpsetcc_v(a, 12);
    __builtin_rvtt_gs_sfpsetcc_i(1, 12);
    cond = __builtin_rvtt_sfpxicmps(nullptr, a, 0, 0, 0, 4);
    __builtin_rvtt_sfpxcondb(cond, top);
    top = __builtin_rvtt_sfpxvif();
    cond = __builtin_rvtt_sfpxicmpv(a, d, 4);
    __builtin_rvtt_sfpxcondb(cond, top);
    top = __builtin_rvtt_sfpxvif();
    cond = __builtin_rvtt_gs_sfpxfcmps(nullptr, a, 0, 0, 0, 4);
    __builtin_rvtt_sfpxcondb(cond, top);
    top = __builtin_rvtt_sfpxvif();
    cond = __builtin_rvtt_gs_sfpxfcmpv(a, d, 4);
    __builtin_rvtt_sfpxcondb(cond, top);
    __builtin_rvtt_gs_sfpcompc();
    __builtin_rvtt_gs_sfppopc();

    a = __builtin_rvtt_gs_sfpxloadi(nullptr, 0, 12, 0, 0);
    a = __builtin_rvtt_gs_sfpxloadi(nullptr, 0, -12, 0, 0);
    __builtin_rvtt_gs_sfpstore(nullptr, a, 2, 0, 0, 0);
    __builtin_rvtt_gs_sfpstore(nullptr, a, 2, 4, 0, 0);
    v64sf lr13 = __builtin_rvtt_sfpassignlreg(13);
    __builtin_rvtt_gs_sfpstore(nullptr, lr13, 2, 4, 0, 0);
    b = __builtin_rvtt_gs_sfpload(nullptr, 1, 0, 0, 0);  // reg pressu, 0, 0re
    v64sf e = __builtin_rvtt_gs_sfpmad(a, b, c, 1);
    __builtin_rvtt_gs_sfpstore(nullptr, e, 2, 4, 0, 0);

    __builtin_rvtt_gs_sfpnop();

    v64sf v1, v2;
    __builtin_rvtt_gs_sfpstore(nullptr, b, 2, 1, 0, 0);
    v1 = __builtin_rvtt_gs_sfpload(nullptr, 2, 0, 0, 0);
    v2 = __builtin_rvtt_gs_sfpmov(v1, SFPMOV_MOD1_COMPSIGN);

    __builtin_rvtt_gs_sfpstore(nullptr, c, 2, 2, 0, 0);
    __builtin_rvtt_gs_sfpstore(nullptr, d, 2, 3, 0, 0);
    __builtin_rvtt_gs_sfpstore(nullptr, v2, 2, 10, 0, 0);

    v2 = __builtin_rvtt_gs_sfpexexp(v1, SFPEXEXP_MOD1_DEBIAS);
    v2 = __builtin_rvtt_gs_sfpexman(v1, SFPEXMAN_MOD1_PAD8);

    v2 = __builtin_rvtt_gs_sfpsetexp_i(nullptr, 23, 0, 0, v1);
    v2 = __builtin_rvtt_gs_sfpsetexp_v(v2, v1);

    v2 = __builtin_rvtt_gs_sfpsetman_i(nullptr, 23, 0, 0, v1);
    v2 = __builtin_rvtt_gs_sfpsetman_v(v2, v1);

    v2 = __builtin_rvtt_gs_sfpabs(v1, SFPABS_MOD1_FLOAT);
    v2 = __builtin_rvtt_gs_sfpand(v2, v1);

    v2 = __builtin_rvtt_gs_sfpor(v2, v1);
    v2 = __builtin_rvtt_gs_sfpnot(v1);

    v2 = __builtin_rvtt_gs_sfpmuli(nullptr, v2, 32, 0, 0, 1);
    v2 = __builtin_rvtt_gs_sfpaddi(nullptr, v2, 32, 0, 0, 0);

    v2 = __builtin_rvtt_gs_sfpdivp2(nullptr, 32, 0, 0, v1, 1);

    v2 = __builtin_rvtt_gs_sfplz(v1, 9);

    v2 = __builtin_rvtt_gs_sfpshft_i(nullptr, v2, 10, 0, 0);
    v2 = __builtin_rvtt_gs_sfpshft_v(v2, v1);

    v1 = __builtin_rvtt_gs_sfpxiadd_i(nullptr, v1, 10, 0, 0, 0);

    v2 = __builtin_rvtt_gs_sfpsetsgn_i(nullptr, 10, 0, 0, v1);
    v2 = __builtin_rvtt_gs_sfpsetsgn_v(v2, v1);

    __builtin_rvtt_gs_sfpstore(nullptr, v1, 2, 6, 0, 0);
    __builtin_rvtt_gs_sfpstore(nullptr, v2, 2, 8, 0, 0);

    v2 = __builtin_rvtt_gs_sfplut(v2, d, c, v1, 1);
}

void smoke_live()
{
    v64sf a, d;

    a = __builtin_rvtt_gs_sfpload(nullptr, 1, 0, 0, 0);
    a = __builtin_rvtt_gs_sfpload_lv(nullptr, a, 1, 0, 0, 0);

    d = __builtin_rvtt_gs_sfpload(nullptr, 1, 0, 0, 0);
    d = __builtin_rvtt_gs_sfpmad_lv(d,   a,   a,   a, 1);
    d = __builtin_rvtt_gs_sfpmul_lv(d,   a,   a, 1);
    d = __builtin_rvtt_gs_sfpadd_lv(d,   a,   a, 1);

    a = __builtin_rvtt_gs_sfpxloadi_lv(nullptr, a, 0, 12, 0, 0);

    a = __builtin_rvtt_gs_sfpmov_lv(a, d, SFPMOV_MOD1_COMPSIGN);

    a = __builtin_rvtt_gs_sfpexman_lv(a, d, SFPEXMAN_MOD1_PAD8);

    a = __builtin_rvtt_gs_sfpsetexp_i_lv(nullptr, a, 23, 0, 0, d);
    a = __builtin_rvtt_gs_sfpsetman_i_lv(nullptr, a, 23, 0, 0, d);

    a = __builtin_rvtt_gs_sfpabs_lv(a, d, SFPABS_MOD1_FLOAT);
    a = __builtin_rvtt_gs_sfpnot_lv(a, d);

    a = __builtin_rvtt_gs_sfpdivp2_lv(nullptr, a, 32, 0, 0, d, 1);

    __builtin_rvtt_gs_sfppushc();

    a = __builtin_rvtt_gs_sfplz_lv(a, d, 2);

    a = __builtin_rvtt_gs_sfpexexp_lv(a, d, SFPEXEXP_MOD1_DEBIAS);

    a = __builtin_rvtt_gs_sfpxiadd_i_lv(nullptr, a, d, 10, 0, 0, 4);

    __builtin_rvtt_gs_sfppopc();

    a = __builtin_rvtt_gs_sfpsetsgn_i_lv(nullptr, a, 10, 0, 0, d);
}

void smokier()
{
    v64sf a = __builtin_rvtt_sfpassignlreg(2);
    v64sf b = __builtin_rvtt_sfpassignlreg(0);
    v64sf c = __builtin_rvtt_sfpassignlreg(1);
    v64sf d = __builtin_rvtt_gs_sfpload(nullptr, 1, 20, 0, 0);

    d = __builtin_rvtt_gs_sfpmad(a, b, c, 3);
    d = __builtin_rvtt_gs_sfplut(b, c, a, d, 7);

    __builtin_rvtt_gs_sfpstore(nullptr, a, 2, 20, 0, 0);
    __builtin_rvtt_gs_sfpstore(nullptr, b, 2, 20, 0, 0);
    __builtin_rvtt_gs_sfpstore(nullptr, c, 2, 20, 0, 0);
    __builtin_rvtt_gs_sfpstore(nullptr, d, 2, 20, 0, 0);

    v64sf e = __builtin_rvtt_gs_sfpload(nullptr, 1, 20, 0, 0);
    v64sf f = __builtin_rvtt_gs_sfpload(nullptr, 1, 20, 0, 0);
    v64sf g = __builtin_rvtt_gs_sfpload(nullptr, 1, 20, 0, 0);

    __builtin_rvtt_gs_sfpstore(nullptr, e, 2, 20, 0, 0);
    __builtin_rvtt_gs_sfpstore(nullptr, f, 2, 20, 0, 0);
    __builtin_rvtt_gs_sfpstore(nullptr, g, 2, 20, 0, 0);
    __builtin_rvtt_gs_sfppreservelreg(c, 2);
}

void smokiest()
{
    v64sf a, b, orig_a;

    a = __builtin_rvtt_gs_sfpload(nullptr, 2, 0, 0, 0);
    b = __builtin_rvtt_gs_sfpload(nullptr, 2, 1, 0, 0);

    orig_a = a;
    a = b;
    a = __builtin_rvtt_gs_sfpsetexp_v(a, orig_a);
   
    __builtin_rvtt_gs_sfpstore(nullptr, a, 2, 6, 0, 0);
}


void assignlr()
{
    v64sf x;

    x = __builtin_rvtt_sfpassignlreg(0);
    __builtin_rvtt_gs_sfpstore(nullptr, x, 0, 0, 0, 0);
    x =__builtin_rvtt_sfpassignlreg(1);
    __builtin_rvtt_gs_sfpstore(nullptr, x, 0, 0, 0, 0);
    x =__builtin_rvtt_sfpassignlreg(2);
    __builtin_rvtt_gs_sfpstore(nullptr, x, 0, 0, 0, 0);
    x =__builtin_rvtt_sfpassignlreg(3);
    __builtin_rvtt_gs_sfpstore(nullptr, x, 0, 0, 0, 0);
    x =__builtin_rvtt_sfpassignlreg(4);
    __builtin_rvtt_gs_sfpstore(nullptr, x, 0, 0, 0, 0);
    x =__builtin_rvtt_sfpassignlreg(5);
    __builtin_rvtt_gs_sfpstore(nullptr, x, 0, 0, 0, 0);
    x =__builtin_rvtt_sfpassignlreg(6);
    __builtin_rvtt_gs_sfpstore(nullptr, x, 0, 0, 0, 0);
    x =__builtin_rvtt_sfpassignlreg(7);
    __builtin_rvtt_gs_sfpstore(nullptr, x, 0, 0, 0, 0);
    x =__builtin_rvtt_sfpassignlreg(8);
    __builtin_rvtt_gs_sfpstore(nullptr, x, 0, 0, 0, 0);
    x =__builtin_rvtt_sfpassignlreg(9);
    __builtin_rvtt_gs_sfpstore(nullptr, x, 0, 0, 0, 0);
    x =__builtin_rvtt_sfpassignlreg(10);
    __builtin_rvtt_gs_sfpstore(nullptr, x, 0, 0, 0, 0);
    x =__builtin_rvtt_sfpassignlreg(11);
    __builtin_rvtt_gs_sfpstore(nullptr, x, 0, 0, 0, 0);
    x =__builtin_rvtt_sfpassignlreg(12);
    __builtin_rvtt_gs_sfpstore(nullptr, x, 0, 0, 0, 0);
    x =__builtin_rvtt_sfpassignlreg(13);
    __builtin_rvtt_gs_sfpstore(nullptr, x, 0, 0, 0, 0);
    x =__builtin_rvtt_sfpassignlreg(14);
    __builtin_rvtt_gs_sfpstore(nullptr, x, 0, 0, 0, 0);
    x =__builtin_rvtt_sfpassignlreg(15);
    __builtin_rvtt_gs_sfpstore(nullptr, x, 0, 0, 0, 0);
}

int main(int argc, char* argv[])
{
    assignlr();
    smoke();
    smoke_live();
    smokier();
    smokiest();
}
