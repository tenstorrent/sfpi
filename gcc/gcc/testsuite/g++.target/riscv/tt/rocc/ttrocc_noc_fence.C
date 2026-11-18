// { dg-do-compile }
// { dg-additional-options "-mcpu=tt-qsr64-rocc" }

int main(void) {
  __builtin_riscv_ttrocc_noc_fence(); // { dg-final { "scan-assembler" "tt\.rocc\.noc_fence" } }
}
