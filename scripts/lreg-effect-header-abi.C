// Compile with the candidate target compiler at both -O0 and -O2.
#include <lltt.h>
#ifndef LLTT_HAS_LREG_EFFECT
#error "The candidate compiler must expose the raw LREG effect builtin"
#endif

void lreg_effect_constant_masks() {
    lltt::lreg_effect(0x1u, 0x2u);
}

void lreg_effect_dynamic_masks(unsigned reads, unsigned writes) {
    lltt::lreg_effect(reads, writes);
}
