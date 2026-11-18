// { dg-do assemble }
// { dg-options "-mcpu=tt-qsr64-rocc -O2" }

#include <cstdint>
void push_src()  {
  __builtin_riscv_ttrocc_addrgen_push_src(1);
}
void push_both() {
  __builtin_riscv_ttrocc_addrgen_push_both(1);
}
void push_both_pop_x(uint64_t s, uint64_t d) {
  __builtin_riscv_ttrocc_addrgen_push_both_pop_x(1, s, d);
}
