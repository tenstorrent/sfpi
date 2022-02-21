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

using namespace sfpi;

void simple_yes()
{
    vInt a = 0;

    v_if (a >= 1) {
        v_if (a >= 2) {
            a = 3;
        }
        v_endif;
    }
    v_endif;
}

void double_yes()
{
    vInt a = 0;

    v_if (a >= 1) {
        v_if (a >= 2) {
            a = 3;
            v_if (a >= 4) {
                a = 5;
            }
            v_endif;
        }
        v_endif;
    }
    v_endif;
}

void both_noyes()
{
    vInt a = 0;

    v_if (a >= 1) {
        v_if (a >= 2) {
            a = 3;
        }
        v_endif;
        v_if (a >= 4) {
            a = 5;
        }
        v_endif;
    }
    v_endif;
}

void consecutive_yes()
{
    vInt a = 0;

    v_if (a >= 1) {
        v_if (a >= 2) {
            a = 3;
        }
        v_endif;
    }
    v_endif;
    v_if (a >= 4) {
        v_if (a >= 5) {
            a = 5;
        }
        v_endif;
    }
    v_endif;
}

void else_yes()
{
    vInt a = 0;

    v_if (a >= 1) {
        a = 2;
    } v_else {
        v_if (a >= 3) {
            a = 4;
        }
        v_endif;
    }
    v_endif;

    v_if (a >= 5) {
        a = 6;
    } v_elseif (a >= 7) {
        v_if (a >= 8) {
            a = 9;
        }
        v_endif;
    }
    v_endif;
}

void block_yes()
{
    vInt a = 0;

    v_if (a >= 1) {
        a = 2;
        v_block {
            v_and(a >= 3);
            a = 4;
        }
        p_endblock;
    }
    v_endif;
}

void no_yes_no_yes_no()
{
    vInt a = 0;

    v_if (a >= 1) {
        v_if (a >= 2) {
            v_if (a >= 3) {
                v_if (a >= 4) {
                    v_if (a >= 5) {
                        a = 6;
                    }
                    v_endif;
                    a = 7;
                }
                v_endif;
            }
            v_endif;
            a = 8;
        }
        v_endif;
    }
    v_endif;
}

void bb_yes(int v)
{
    vInt a = 0;

    v_if (a >= 1) {
        for (int x = 0; x < v; x++) {
            v_if (a >= 2) {
                if (x % 2 == 0) {
                    v_if (a >= 3) {
                        a = 4;
                    }
                    v_endif;
                }
            }
            v_endif;
        }
    }
    v_endif;
}

void func_call()
{
    vInt a = 0;

    v_if (a >= 1) {
        rand();
        v_if (a >= 2) {
            a = 3;
        }
    v_endif;
    }
    v_endif;
}

void compc_no()
{
    vInt a = 0;

    v_if (a >= 1) {
        v_if (a >= 2) {
            a = 3;
        } v_else {
            a = 4;
        }
        v_endif;
    }
    v_endif;
}

void cascade_no()
{
    vInt a = 0;

    v_if (a >= 1) {
        a = 2;
    } v_elseif (a >= 3) {
        a = 4;
    } v_elseif (a >= 5) {
        a = 6;
    } v_elseif (a >= 7) {
        a = 7;
    } v_elseif (a >= 8) {
        a = 8;
    }
    v_endif;
}

void intervene_no()
{
    vInt a = 0;

    v_if (a >= 1) {
        v_if (a >= 2) {
            a = 3;
        }
        v_endif;

        a = 4;
    }
    v_endif;
}

void compc_intervene_no()
{
    vInt a = 0;

    v_if (a >= 1) {
        v_if (a >= 2) {
            a = 3;
        } v_else {
            a = 4;
        }
        v_endif;

        a = 5;
    }
    v_endif;
}

void fn_call_no()
{
    vInt a = 0;

    v_if (a >= 1) {
        v_if (a >= 2) {
            a = 3;
        }
        v_endif;

        rand();
    }
    v_endif;

    dst_reg[0] = a;
}

void control_flow_v_if_switch(int v1, int v2)
{
    vInt v = 1;

    switch (v1) {
    case 10:
        v_if (v < 2) {
            v = 3;
            break;
        }
        v_endif;
    case 20:
        v_if (v < 4) {
            v = 5;
            break;
        } v_elseif (v < 6) {
            v = 7;
        }
        v_endif;
    case 30:
        v_if (v < 8) {
            v = 9;
            if (v2 > 40) {
                break;
            }
        } v_elseif (v < 10) {
            v = 11;
        }
        v_endif;
    }

    v = 8;
    dst_reg[0] = v;
}

void control_flow_pif_continue(int val)
{
    vInt v = 1;

    for (int i = 0; i < val; i++) {
        v_if (v < 2) {
            v = 2;
            v_if (v < 1) {
                v = 4;
                if (val == rand()) {
                    continue;
                }
            } v_else {
                v = 5;
            }
            v_endif;
            v = 7;
        }
        v_endif;
        v = 8;
    }
}

void control_flow_pif_goto(int val)
{
    vInt v = 1;

    v_if (v < 2) {
        v = 2;
        v_if (v < 1) {
            v = 4;
            if (rand() == 1) {
                goto out;
            }
        } v_else {
            v = 5;
        }
        v_endif;
        v = 7;
    }
    v_endif;

 out:
    v = 8;
}
