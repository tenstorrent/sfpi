// Wormhole test to make sure SFPNOPS are inserted, even when the
// dependency is several BB's away. Somewhat awkward to arrange such a
// testcase, this is probably fragile

// { dg-do compile }
// { dg-additional-options "-mcpu=tt-wh-tensix -mabi=ilp32 -O3" }

using vec_t = __xtt_vector;

inline void calculate_power_iterative(unsigned exponent) {
#pragma GCC unroll 8
  for (int d = 0; d < 8; d++) {
    vec_t in = __builtin_rvtt_sfpload(nullptr, 0, 0, 0, 0, 3);
    vec_t acc = __builtin_rvtt_sfpxloadi(nullptr, 0x3f800000, 0, 0, -32);
    for (unsigned i = 0; i < exponent; i++)
      acc = __builtin_rvtt_sfpassign_lv(
	      acc, __builtin_rvtt_sfpmul(acc, in, 0));

    __builtin_rvtt_sfpstore(nullptr, acc, 0, 0, 0, 12, 3);
    __builtin_rvtt_ttincrwc(0, 2, 0, 0);
  }
}

inline void llk_math_eltwise_unary_sfpu_params(unsigned dst_index,
					       int, unsigned arg) {
#pragma GCC unroll 0
  for (int face = 0; face < 4; face++)
    calculate_power_iterative(arg);
}

inline void llk_math_eltwise_unary_sfpu_power(unsigned dst_index, int pow = 0,
					      int vector_mode = 4) {
  llk_math_eltwise_unary_sfpu_params(dst_index, 4, pow);
}

__attribute__((always_inline))
inline void power_tile(unsigned, unsigned param0) {
  llk_math_eltwise_unary_sfpu_power(0, param0);
}
  
extern unsigned *args;

static void math_main() {
  int ix = 0;
  const auto height = args[ix++];
  const auto width = args[ix++];
  const auto p = args[ix++];

  for (unsigned jx = 0; jx < height; jx++)
    for (unsigned ix = 0; ix < width; ix++)
      power_tile(0, p);
}

void run_kernel() {
  math_main();
}

// Same number of SFPMULs as SFPNOPs
// { dg-final { scan-assembler-times {SFPMUL[^A-Z]} 8 } }
// { dg-final { scan-assembler-times {SFPNOP[^A-Z]} 8 } }
