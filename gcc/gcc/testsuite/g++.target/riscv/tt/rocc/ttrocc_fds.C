// { dg-do-compile }
// { dg-additional-options "-mcpu=tt-qsr64-rocc" }

int main (void) {
  __builtin_riscv_ttrocc_fds_intf_write(0, 15); // { dg-final { "scan-assembler" "tt\.rocc\.fds_intf_write" } }
  unsigned long value = __builtin_riscv_ttrocc_fds_intf_read(0); // { dg-final { "scan-assembler" "tt\.rocc\.fds_intf_read" } }
}
