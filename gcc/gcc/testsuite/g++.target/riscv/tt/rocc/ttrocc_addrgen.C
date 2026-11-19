// { dg-do-compile }
// { dg-additional-options "-mcpu=tt-qsr64-rocc -mno-tt-fix-qsrrocc" }

int
main (void)
{
  __builtin_riscv_ttrocc_addrgen_wr_reg (0, 4, 15); // { dg-final { "scan-assembler" "tt\.rocc\.addrgen_wr_reg" } }

  unsigned long to_write = 15;
  __builtin_riscv_ttrocc_addrgen_wr_reg (0, 4, to_write); // { dg-final { "scan-assembler" "tt\.rocc\.addrgen_wr_reg" } }

  unsigned long rd0 = __builtin_riscv_ttrocc_addrgen_rd_reg (0, 4); // { dg-final { "scan-assembler" "tt\.rocc\.addrgen_rd_reg" } }

  __builtin_riscv_ttrocc_addrgen_reset (1); // { dg-final { "scan-assembler" "tt\.rocc\.addrgen_reset" } }
  __builtin_riscv_ttrocc_addrgen_reset_counters (1); // { dg-final { "scan-assembler" "tt\.rocc\.addrgen_reset_counter" } }

  unsigned long r0 = __builtin_riscv_ttrocc_addrgen_peek_src (1); // { dg-final { "scan-assembler" "tt\.rocc\.addrgen_peek_src" } }
  unsigned long r1 = __builtin_riscv_ttrocc_addrgen_pop_src (1); // { dg-final { "scan-assembler" "tt\.rocc\.addrgen_pop_src" } }
  unsigned long r2 = __builtin_riscv_ttrocc_addrgen_pop_x_src (1, 1); // { dg-final { "scan-assembler" "tt\.rocc\.addrgen_pop_x_src" } }

  unsigned long r3 = __builtin_riscv_ttrocc_addrgen_peek_dest (1); // { dg-final { "scan-assembler" "tt\.rocc\.addrgen_peek_dest" } }
  unsigned long r4 = __builtin_riscv_ttrocc_addrgen_pop_dest (1); // { dg-final { "scan-assembler" "tt\.rocc\.addrgen_pop_dest" } }
  unsigned long r5 = __builtin_riscv_ttrocc_addrgen_pop_x_dest (1, 1); // { dg-final { "scan-assembler" "tt\.rocc\.addrgen_pop_x_dest" } }

  unsigned long r6 = __builtin_riscv_ttrocc_addrgen_pop_both (1, 1, 1); // { dg-final { "scan-assembler" "tt\.rocc\.addrgen_pop_both" } }

  __builtin_riscv_ttrocc_addrgen_push_src (1); // { dg-final { "scan-assembler" "tt\.rocc\.addrgen_push_src" } }
  __builtin_riscv_ttrocc_addrgen_push_src_pop_x (1, 1); // { dg-final { "scan-assembler" "tt\.rocc\.addrgen_push_src_pop_x" } }

  __builtin_riscv_ttrocc_addrgen_push_dest (1); // { dg-final { "scan-assembler" "tt\.rocc\.addrgen_push_dest" } }
  __builtin_riscv_ttrocc_addrgen_push_dest_pop_x (1, 1); // { dg-final { "scan-assembler" "tt\.rocc\.addrgen_push_dest_pop_x" } }

  __builtin_riscv_ttrocc_addrgen_push_both (1); // { dg-final { "scan-assembler" "tt\.rocc\.addrgen_push_both" } }
  __builtin_riscv_ttrocc_addrgen_push_both_pop_x (1, 1, 1); // { dg-final { "scan-assembler" "tt\.rocc\.addrgen_push_both_pop_x" } }
}
