// Exercise the SFPI predicate helpers against the compiler's explicit
// PUSH/ELSE/END graph contract.  Each branch changes a live SFPU result.
namespace ckernel {
volatile unsigned long *instrn_buffer;
}

#include <sfpi.h>
using namespace sfpi;

void predicate_header_abi() {
    vFloat a = l_reg[LRegs::LReg3];
    vFloat b = l_reg[LRegs::LReg4];
    vFloat result = 0.0f;

    v_if (a < 1.0f) {
        result = 1.0f;
        v_if (b > 2.0f) {
            result = 2.0f;
        } v_else {
            result = 3.0f;
        } v_endif;
    } v_elseif (a < 2.0f) {
        result = 4.0f;
    } v_else {
        result = 5.0f;
    } v_endif;

    v_block
        v_and (a > 0.0f);
        v_and (b < 0.0f);
        result = 6.0f;
    v_endblock;
    dst_reg[0] = result;
}

void predicate_header_deep_elseif_abi() {
    vFloat value = l_reg[LRegs::LReg3];
    vFloat result = 0.0f;
    v_if (value < 1.0f) {
        result = 1.0f;
    } v_elseif (value < 2.0f) {
        result = 2.0f;
    } v_elseif (value < 3.0f) {
        result = 3.0f;
    } v_elseif (value < 4.0f) {
        result = 4.0f;
    } v_elseif (value < 5.0f) {
        result = 5.0f;
    } v_else {
        result = 6.0f;
    } v_endif;
    dst_reg[1] = result;
}

void predicate_header_move_abi() {
    vFloat value = l_reg[LRegs::LReg3];
    vFloat result = 0.0f;
    {
        sfpi::impl_::CC active;
        active.push().if_().cond (value < 1.0f);
        result = 1.0f;
        sfpi::impl_::CC owner;
        owner = static_cast<sfpi::impl_::CC &&>(active);
        sfpi::impl_::CC final_owner(static_cast<sfpi::impl_::CC &&>(owner));
    }
    dst_reg[2] = result;
}

void predicate_header_explicit_move_abi() {
    vFloat value = l_reg[LRegs::LReg3];
    vFloat result = 0.0f;
    {
        sfpi::impl_::CC source(true);
        source.push().if_().cond (value < 1.0f);
        result = 1.0f;
        sfpi::impl_::CC owner;
        owner = static_cast<sfpi::impl_::CC &&>(source);
        sfpi::impl_::CC final_owner(static_cast<sfpi::impl_::CC &&>(owner));
        final_owner.end_();
    }
    dst_reg[3] = result;
}
