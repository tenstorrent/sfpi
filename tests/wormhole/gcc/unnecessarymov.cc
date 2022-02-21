#if 0
#include "test.h"

using namespace sfpi;

// This is the wrapper code, it generates the builtins below (I removed the
// PUSHC/POPC for clarity, doesn't affect the behavior)
void whymov()
{
    VecShort a = 3;

    p_if(dregs[0] == 0.0F) {
        a = 28;
    }
    p_endif;

    VecHalf b = 128.0F;
    // Here a is in L0 and b in L1
    p_if(a < 29) {
        // Here compiler injects a move of L1 to L0
        // Loads into L0
        b = 512.0F;
    }
    p_endif;

    // Stores L0
    dregs[6] = b;
}

#else

typedef float v64sf __attribute__((vector_size(64*4)));

#if 0
        SFPLOADI        L0, 4, 3
        SFPLOAD L1, 1, 0
        SFPSETCC        0, L1, 6
        SFPNOP
        SFPLOADI        L0, 4, 28
        SFPLOADI        L1, 0, 17152
        SFPIADD -29, L0, L0, 1
        SFPNOP
        SFPNOP
        SFPNOP
// This is the unnecessary move (and accompanying CC state)
        SFPPUSHC
        SFPENCC 3, 2
        SFPNOP
        SFPMOV  L1, L0, 0
        SFPNOP
        SFPNOP
        SFPPOPC
        SFPNOP
// to here
        // This is a live version of load that preserves L0
        SFPLOADI        L0, 0, 17408
        SFPSTORE        L0, 0, 24
#endif

void whymov()
{
    v64sf a = __builtin_rvtt_wh_sfploadi(nullptr, 4, 3);
    v64sf tmp = __builtin_rvtt_wh_sfpload(nullptr, 1, 0);
    __builtin_rvtt_wh_sfppushc();
    __builtin_rvtt_wh_sfpsetcc_v(tmp, 6);
    a = __builtin_rvtt_wh_sfploadi_lv(nullptr, a, 4, 28);
    __builtin_rvtt_wh_sfppopc();
    v64sf b = __builtin_rvtt_wh_sfploadi(nullptr, 0, 17152);
    __builtin_rvtt_wh_sfppushc();
    __builtin_rvtt_wh_sfpiadd_i(nullptr, a, -29, 1);
    b = __builtin_rvtt_wh_sfploadi_lv(nullptr, b, 0, 17408);
    __builtin_rvtt_wh_sfppopc();
    __builtin_rvtt_wh_sfpstore(nullptr, b, 0, 24);
}

#endif

int main(int argc, char* argv[])
{
    whymov();
}
