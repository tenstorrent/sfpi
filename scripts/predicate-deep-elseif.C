namespace ckernel { volatile unsigned long *instrn_buffer; }
#include <sfpi.h>
using namespace sfpi;
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
