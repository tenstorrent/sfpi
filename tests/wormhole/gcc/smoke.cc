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

    a = __builtin_rvtt_wh_sfpload(nullptr, 0, 1, 0);
    b = __builtin_rvtt_wh_sfpload(nullptr, 0, 1, 16);
    c = __builtin_rvtt_wh_sfpload(nullptr, 0, 1, 32);

    v64sf lr6, lr7, lr8;
    lr6 = __builtin_rvtt_wh_sfpassignlr(6);
    lr7 = __builtin_rvtt_wh_sfpassignlr(7);
    lr8 = __builtin_rvtt_wh_sfpassignlr(8);

    d = __builtin_rvtt_wh_sfpmad(  a,   b,   c, 0);
    d = __builtin_rvtt_wh_sfpmad(  a,   b, lr6, 0);
    d = __builtin_rvtt_wh_sfpmad(  a, lr7,   c, 0);
    d = __builtin_rvtt_wh_sfpmad(  a, lr7, lr6, 0);
    d = __builtin_rvtt_wh_sfpmad(lr8,   b,   c, 0);
    d = __builtin_rvtt_wh_sfpmad(lr8,   b, lr6, 0);
    d = __builtin_rvtt_wh_sfpmad(lr8, lr7,   c, 0);
    d = __builtin_rvtt_wh_sfpmad(lr8, lr7, lr6, 0);

    d = __builtin_rvtt_wh_sfpmul(lr8, lr7, 0);
    d = __builtin_rvtt_wh_sfpadd(lr8, lr7, 0);

    __builtin_rvtt_wh_sfpencc(2, 8);

    __builtin_rvtt_wh_sfppushc(0);
    __builtin_rvtt_wh_sfppushc(0);
    __builtin_rvtt_wh_sfpsetcc_v(a, 12);
    __builtin_rvtt_wh_sfpsetcc_i(3, 12);
    __builtin_rvtt_wh_sfpscmp_ex(nullptr, a, 0, 4);
    __builtin_rvtt_wh_sfpvcmp_ex(a, d, 4);
    __builtin_rvtt_wh_sfppopc();
    __builtin_rvtt_wh_sfpcompc();
    __builtin_rvtt_wh_sfppopc();

    a = __builtin_rvtt_wh_sfploadi_ex(nullptr, 0, 12);
    a = __builtin_rvtt_wh_sfploadi_ex(nullptr, 0, -12);
    __builtin_rvtt_wh_sfpstore(nullptr, a, 0, 2, 0);
    __builtin_rvtt_wh_sfpstore(nullptr, a, 0, 2, 4);
    v64sf lr13 = __builtin_rvtt_wh_sfpassignlr(13);
    __builtin_rvtt_wh_sfpstore(nullptr, lr13, 0, 2, 4);
    v64sf e = __builtin_rvtt_wh_sfpmad(a, b, c, 0);
    __builtin_rvtt_wh_sfpstore(nullptr, e, 0, 2, 4);

    __builtin_rvtt_wh_sfpnop();

    v64sf v1, v2;
    __builtin_rvtt_wh_sfpstore(nullptr, b, 0, 2, 1);
    v1 = __builtin_rvtt_wh_sfpload(nullptr, 0, 2, 0);
    v2 = __builtin_rvtt_wh_sfpmov(v1, SFPMOV_MOD1_COMPSIGN);

    __builtin_rvtt_wh_sfpstore(nullptr, c, 0, 2, 2);
    __builtin_rvtt_wh_sfpstore(nullptr, d, 0, 2, 3);
    __builtin_rvtt_wh_sfpstore(nullptr, v2, 0, 2, 10);

    v2 = __builtin_rvtt_wh_sfpexexp(v1, SFPEXEXP_MOD1_DEBIAS);
    v2 = __builtin_rvtt_wh_sfpexman(v1, SFPEXMAN_MOD1_PAD8);

    v2 = __builtin_rvtt_wh_sfpsetexp_i(nullptr, 23, v1);
    v2 = __builtin_rvtt_wh_sfpsetexp_v(v2, v1);

    v2 = __builtin_rvtt_wh_sfpsetman_i(nullptr, 23, v1);
    v2 = __builtin_rvtt_wh_sfpsetman_v(v2, v1);

    v2 = __builtin_rvtt_wh_sfpabs(v1, SFPABS_MOD1_FLOAT);
    v2 = __builtin_rvtt_wh_sfpand(v2, v1);

    v2 = __builtin_rvtt_wh_sfpor(v2, v1);
    v2 = __builtin_rvtt_wh_sfpnot(v1);

    v2 = __builtin_rvtt_wh_sfpmuli(nullptr, v2, 32, 0);
    v2 = __builtin_rvtt_wh_sfpaddi(nullptr, v2, 32, 0);

    v2 = __builtin_rvtt_wh_sfpdivp2(nullptr, 32, v1, 1);

    v2 = __builtin_rvtt_wh_sfplz(v1, 2);

    v2 = __builtin_rvtt_wh_sfpshft_i(nullptr, v2, 10);
    v2 = __builtin_rvtt_wh_sfpshft_v(v2, v1);

    v1 = __builtin_rvtt_wh_sfpiadd_i_ex(nullptr, v1, 10, 0);
    v2 = __builtin_rvtt_wh_sfpiadd_v(v2, v1, 5);

    v2 = __builtin_rvtt_wh_sfpsetsgn_i(nullptr, 10, v1);
    v2 = __builtin_rvtt_wh_sfpsetsgn_v(v2, v1);

    __builtin_rvtt_wh_sfpstore(nullptr, v1, 0, 2, 6);
    __builtin_rvtt_wh_sfpstore(nullptr, v2, 0, 2, 8);

    v2 = __builtin_rvtt_wh_sfpiadd_v(v2, lr13, 4);

    v2 = __builtin_rvtt_wh_sfpxor(v2, v1);
    v2 = __builtin_rvtt_wh_sfpcast(v1, 1);
    v2 = __builtin_rvtt_wh_sfpshft2_e(v1, 3);
    v2 = __builtin_rvtt_wh_sfpstochrnd_i(nullptr, 1, 3, v1, 1);
    v2 = __builtin_rvtt_wh_sfpstochrnd_v(1, v2, v1, 1);
    __builtin_rvtt_wh_sfpconfig_v(v2, 0);

    v2 = __builtin_rvtt_wh_sfplut(v2, d, c, v1, 0);
}

void smoke_live()
{
    v64sf a, d;

    a = __builtin_rvtt_wh_sfpload(nullptr, 0, 1, 0);
    a = __builtin_rvtt_wh_sfpload_lv(nullptr, a, 0, 1, 0);

    d = __builtin_rvtt_wh_sfpload(nullptr, 0, 1, 0);
    d = __builtin_rvtt_wh_sfpmad_lv(d,   a,   a,   a, 0);
    d = __builtin_rvtt_wh_sfpmul_lv(d,   a,   a, 0);
    d = __builtin_rvtt_wh_sfpadd_lv(d,   a,   a, 0);

    a = __builtin_rvtt_wh_sfploadi_ex_lv(nullptr, a, 0, 12);

    a = __builtin_rvtt_wh_sfpmov_lv(a, d, SFPMOV_MOD1_COMPSIGN);

    a = __builtin_rvtt_wh_sfpexman_lv(a, d, SFPEXMAN_MOD1_PAD8);

    a = __builtin_rvtt_wh_sfpsetexp_i_lv(nullptr, a, 23, d);
    a = __builtin_rvtt_wh_sfpsetman_i_lv(nullptr, a, 23, d, 0);

    a = __builtin_rvtt_wh_sfpabs_lv(a, d, SFPABS_MOD1_FLOAT);
    a = __builtin_rvtt_wh_sfpnot_lv(a, d);

    a = __builtin_rvtt_wh_sfpdivp2_lv(nullptr, a, 32, d, 1);

    __builtin_rvtt_wh_sfppushc(0);

    a = __builtin_rvtt_wh_sfplz_lv(a, d, 2);

    a = __builtin_rvtt_wh_sfpexexp_lv(a, d, SFPEXEXP_MOD1_DEBIAS);

    a = __builtin_rvtt_wh_sfpiadd_i_ex_lv(nullptr, a, d, 10, 4);

    __builtin_rvtt_wh_sfppopc();

    a = __builtin_rvtt_wh_sfpsetsgn_i_lv(nullptr, a, 10, d);

    a = __builtin_rvtt_wh_sfpcast_lv(a, d, 1);
    a = __builtin_rvtt_wh_sfpshft2_e_lv(a, d, 3);
    // XXXXXX bumping into bad gimple stuff again
    //a = __builtin_rvtt_wh_sfpstochrnd_i_lv(nullptr, a, 1, 3, d, 1);
    a = __builtin_rvtt_wh_sfpstochrnd_v_lv(a, 1, a, d, 1);
}

void smokier()
{
    v64sf a = __builtin_rvtt_wh_sfpassignlr(2);
    v64sf b = __builtin_rvtt_wh_sfpassignlr(0);
    v64sf c = __builtin_rvtt_wh_sfpassignlr(1);
    v64sf d = __builtin_rvtt_wh_sfpload(nullptr, 0, 1, 20);

    d = __builtin_rvtt_wh_sfpmad(a, b, c, 0);
    d = __builtin_rvtt_wh_sfplut(b, c, a, d, 4);

    __builtin_rvtt_wh_sfpstore(nullptr, a, 0, 2, 20);
    __builtin_rvtt_wh_sfpstore(nullptr, b, 0, 2, 20);
    __builtin_rvtt_wh_sfpstore(nullptr, c, 0, 2, 20);
    __builtin_rvtt_wh_sfpstore(nullptr, d, 0, 2, 20);

    v64sf e = __builtin_rvtt_wh_sfpload(nullptr, 0, 1, 20);
    v64sf f = __builtin_rvtt_wh_sfpload(nullptr, 0, 1, 20);
    v64sf g = __builtin_rvtt_wh_sfpload(nullptr, 0, 1, 20);

    __builtin_rvtt_wh_sfpstore(nullptr, e, 0, 2, 20);
    __builtin_rvtt_wh_sfpstore(nullptr, f, 0, 2, 20);
    __builtin_rvtt_wh_sfpstore(nullptr, g, 0, 2, 20);
    __builtin_rvtt_wh_sfpkeepalive(c, 2);
}

void smokiest()
{
    v64sf a, b, orig_a;

    a = __builtin_rvtt_wh_sfpload(nullptr, 0, 2, 0);
    b = __builtin_rvtt_wh_sfpload(nullptr, 0, 2, 1);

    orig_a = a;
    a = b;
    a = __builtin_rvtt_wh_sfpsetexp_v(a, orig_a);
   
    __builtin_rvtt_wh_sfpstore(nullptr, a, 0, 2, 6);
}


void assignlr()
{
    v64sf x;

    x = __builtin_rvtt_wh_sfpassignlr(0);
    __builtin_rvtt_wh_sfpstore(nullptr, x, 0, 0, 0);
    x =__builtin_rvtt_wh_sfpassignlr(1);
    __builtin_rvtt_wh_sfpstore(nullptr, x, 0, 0, 0);
    x =__builtin_rvtt_wh_sfpassignlr(2);
    __builtin_rvtt_wh_sfpstore(nullptr, x, 0, 0, 0);
    x =__builtin_rvtt_wh_sfpassignlr(3);
    __builtin_rvtt_wh_sfpstore(nullptr, x, 0, 0, 0);
    x =__builtin_rvtt_wh_sfpassignlr(4);
    __builtin_rvtt_wh_sfpstore(nullptr, x, 0, 0, 0);
    x =__builtin_rvtt_wh_sfpassignlr(5);
    __builtin_rvtt_wh_sfpstore(nullptr, x, 0, 0, 0);
    x =__builtin_rvtt_wh_sfpassignlr(6);
    __builtin_rvtt_wh_sfpstore(nullptr, x, 0, 0, 0);
    x =__builtin_rvtt_wh_sfpassignlr(7);
    __builtin_rvtt_wh_sfpstore(nullptr, x, 0, 0, 0);
    x =__builtin_rvtt_wh_sfpassignlr(8);
    __builtin_rvtt_wh_sfpstore(nullptr, x, 0, 0, 0);
    x =__builtin_rvtt_wh_sfpassignlr(9);
    __builtin_rvtt_wh_sfpstore(nullptr, x, 0, 0, 0);
    x =__builtin_rvtt_wh_sfpassignlr(10);
    __builtin_rvtt_wh_sfpstore(nullptr, x, 0, 0, 0);
    x =__builtin_rvtt_wh_sfpassignlr(11);
    __builtin_rvtt_wh_sfpstore(nullptr, x, 0, 0, 0);
    x =__builtin_rvtt_wh_sfpassignlr(12);
    __builtin_rvtt_wh_sfpstore(nullptr, x, 0, 0, 0);
    x =__builtin_rvtt_wh_sfpassignlr(13);
    __builtin_rvtt_wh_sfpstore(nullptr, x, 0, 0, 0);
    x =__builtin_rvtt_wh_sfpassignlr(14);
    __builtin_rvtt_wh_sfpstore(nullptr, x, 0, 0, 0);
    x =__builtin_rvtt_wh_sfpassignlr(15);
    __builtin_rvtt_wh_sfpstore(nullptr, x, 0, 0, 0);
}

int main(int argc, char* argv[])
{
    assignlr();
    smoke();
    smoke_live();
    smokier();
    smokiest();
}
