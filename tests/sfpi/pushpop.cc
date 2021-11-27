#include <stdlib.h>

// Test push/pop optimizations
//
// The compiler optimizes 2 things:
// 1) The outermost pushc/popc: remove pushc, turn popc into encc
// 2) Tail popc: if 2 popcs occur without intervening instructions and the
//    inner pushc/popc did not have a compc in between, the inner pushc/popc
//    can be removed.  Tricky because you have to track the prior popc

#include <cstdio>
#include "test.h"

extern void make_a_call();

using namespace sfpi;

void simple_yes()
{
    VecShort a = 0;

    p_if (a >= 1) {
        p_if (a >= 2) {
            a = 3;
        }
        p_endif;
    }
    p_endif;
}


void double_yes()
{
    VecShort a = 0;

    p_if (a >= 1) {
        p_if (a >= 2) {
            a = 3;
            p_if (a >= 4) {
                a = 5;
            }
            p_endif;
        }
        p_endif;
    }
    p_endif;
}

void both_noyes()
{
    VecShort a = 0;

    p_if (a >= 1) {
        p_if (a >= 2) {
            a = 3;
        }
        p_endif;
        p_if (a >= 4) {
            a = 5;
        }
        p_endif;
    }
    p_endif;
}

void consecutive_yes()
{
    VecShort a = 0;

    p_if (a >= 1) {
        p_if (a >= 2) {
            a = 3;
        }
        p_endif;
    }
    p_endif;
    p_if (a >= 4) {
        p_if (a >= 5) {
            a = 5;
        }
        p_endif;
    }
    p_endif;
}

void else_yes()
{
    VecShort a = 0;

    p_if (a >= 1) {
        a = 2;
    } p_else {
        p_if (a >= 3) {
            a = 4;
        }
        p_endif;
    }
    p_endif;

    p_if (a >= 5) {
        a = 6;
    } p_elseif (a >= 7) {
        p_if (a >= 8) {
            a = 9;
        }
        p_endif;
    }
    p_endif;
}

void block_yes()
{
    VecShort a = 0;

    p_if (a >= 1) {
        a = 2;
        p_block {
            p_and(a >= 3);
            a = 4;
        }
        p_endblock;
    }
    p_endif;
}

void no_yes_no_yes_no()
{
    VecShort a = 0;

    p_if (a >= 1) {
        p_if (a >= 2) {
            p_if (a >= 3) {
                p_if (a >= 4) {
                    p_if (a >= 5) {
                        a = 6;
                    }
                    p_endif;
                    a = 7;
                }
                p_endif;
            }
            p_endif;
            a = 8;
        }
        p_endif;
    }
    p_endif;
}

void bb_yes(int v)
{
    VecShort a = 0;

    p_if (a >= 1) {
        for (int x = 0; x < v; x++) {
            p_if (a >= 2) {
                if (x % 2 == 0) {
                    p_if (a >= 3) {
                        a = 4;
                    }
                    p_endif;
                }
            }
            p_endif;
        }
    }
    p_endif;
}

void compc_no()
{
    VecShort a = 0;

    p_if (a >= 1) {
        p_if (a >= 2) {
            a = 3;
        } p_else {
            a = 4;
        }
        p_endif;
    }
    p_endif;
}

void intervene_no()
{
    VecShort a = 0;

    p_if (a >= 1) {
        p_if (a >= 2) {
            a = 3;
        }
        p_endif;

        a = 4;
    }
    p_endif;
}

void compc_intervene_no()
{
    VecShort a = 0;

    p_if (a >= 1) {
        p_if (a >= 2) {
            a = 3;
        } p_else {
            a = 4;
        }
        p_endif;

        a = 5;
    }
    p_endif;
}

void fn_call_no()
{
    VecShort a = 0;

    p_if (a >= 1) {
        p_if (a >= 2) {
            a = 3;
        }
        p_endif;

        make_a_call();
    }
    p_endif;
}

void control_flow_p_if_switch(int v1, int v2)
{
    VecShort v = 1;

    switch (v1) {
    case 10:
        p_if (v < 2) {
            v = 3;
            break;
        }
        p_endif;
    case 20:
        p_if (v < 4) {
            v = 5;
            break;
        } p_elseif (v < 6) {
            v = 7;
        }
        p_endif;
    case 30:
        p_if (v < 8) {
            v = 9;
            if (v2 > 40) {
                break;
            }
        } p_elseif (v < 10) {
            v = 11;
        }
        p_endif;
    }

    v = 8;
    dst_reg[0] = v;
}

void control_flow_pif_continue(int val)
{
    VecShort v = 1;

    for (int i = 0; i < val; i++) {
        p_if (v < 2) {
            v = 2;
            p_if (v < 1) {
                v = 4;
                if (val == rand()) {
                    continue;
                }
            }
            p_else {
                v = 5;
            }
            p_endif;
            v = 7;
        }
        p_endif;
        v = 8;
    }
}

void control_flow_pif_goto(int val)
{
    VecShort v = 1;

    p_if (v < 2) {
        v = 2;
        p_if (v < 1) {
            v = 4;
            if (rand() == 1) {
                goto out;
            }
        } p_else {
            v = 5;
        }
        p_endif;
        v = 7;
    }
    p_endif;

 out:
    v = 8;
}
