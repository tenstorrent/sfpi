// { dg-do-compile }
// { dg-additional-options "-mcpu=tt-qsr64-rocc" }

int main(void) {
  __builtin_riscv_ttrocc_dbg_postcode(1); // { dg-final { "scan-assembler" "tt\.rocc\.dbg_postcode" } }
}
