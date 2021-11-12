#include <stdio.h>
#include <stdlib.h>
#include <ckernel_ops.h>
#include <cstdint>
#include <limits>
#include "test.h"

using namespace sfpi;

//typedef unsigned int uint;
typedef uint32_t uint;

template <bool HAS_BASE_SCALING>
void calculate_log_body(const int log_base_scale_factor)
{
    ////////////////////////////
    // Load From dest + "normalize to calculation range"
    ////////////////////////////
    VecHalf in = dregs[0];
    VecHalf x = in.set_exp(127);    // set exp to exp bias (put in range of 1-2)
    x = x + CReg_Neg_1;             // Subtract 1 to get x in ln(x+1)

    ////////////////////////////
    // Calculate Cheby Approximation using Horner Form Multiplication: 3rd Order
    // x* ( x* (A*x + B) + C) + D
    ////////////////////////////
    VecHalf a = ScalarFP16a(0.1058F);     // A :0.1058, B: -0.3942, C: 0.9813, D: 0.006
    VecHalf series_result = x * (x * (x * a + ScalarFP16a(-0.3942F)) + ScalarFP16a(0.9813F)) + ScalarFP16a(0.006F);

    ////////////////////////////
    // Convert exponent to float
    ////////////////////////////
    // Extract exponent and calculate abs value.  Save sign into partial reg
    // Reload argument to reduce register pressure
    VecShort exp = 0;
    p_if (in != 0.0F && exp.ex_exp_cc(in, ExExpDoDebias, ExExpCCLT0)) {
        exp = exp.abs();
        in = in.set_sgn(1);
    }
    p_endif;

    // Calculate exponent of the exponent value. Done by using LZ
    // Get leading zero.  If not zero, we do 19 + ~LZ to get exponent value (mathematically == 19 - LZ - 1)
    VecShort new_exp = 0;
    p_if (exp != 0 && new_exp.lz_cc(exp, LzCCNE0)) {
        new_exp = ~new_exp;
        p_tail_if (new_exp.add_cc(new_exp, 19, IAddCCGTE0)) {
            new_exp += 127;
        }
    }
    p_endif;

    // in, new_exp, exp, series_result
    VecHalf result = in.set_exp(new_exp);
    VecShort shft = exp.lz(); // result, exp, shft, series_result
    shft += 1;// XXXXXXX fix when addop is gone
    exp = (exp.reinterpret<VecUShort>()).shft(shft).reinterpret<VecShort>();
    result = result.set_man(exp);
    result = result * CReg_0p692871094 + series_result; // exp correction: ln(1+x) + exp*ln(2)

    if constexpr (HAS_BASE_SCALING) {
        result *= ScalarFP16a(log_base_scale_factor);
    }
    
    ////////////////////////////
    // Base case when input is 0. ln(0) = -inf
    ////////////////////////////
    p_if (dregs[0] == 0.0F) {
        result = -std::numeric_limits<float>::infinity();
    }
    p_endif;

    dregs[0] = result;

    TTI_INCRWC(0, 4, 0, 0);
}

template <bool APPROXIMATION_MODE, bool HAS_BASE_SCALING>
void calculate_log(uint log_base_scale_factor)
{
    calculate_log_body<HAS_BASE_SCALING>(log_base_scale_factor);
    calculate_log_body<HAS_BASE_SCALING>(log_base_scale_factor);
    calculate_log_body<HAS_BASE_SCALING>(log_base_scale_factor);
    calculate_log_body<HAS_BASE_SCALING>(log_base_scale_factor);
}

int main(int argc, char *argv[])
{
    calculate_log<true, false>(argc);
}
