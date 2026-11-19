// { dg-do-compile }
// { dg-additional-options "-mcpu=tt-qsr64-rocc" }

int main (void) {
  __builtin_riscv_ttrocc_wr_tiles_to_process_thres_tr_ack(0, 15); // { dg-final { "scan-assembler" "tt\.rocc\.wr_tiles_to_process_thres_tr_ack" } }
  __builtin_riscv_ttrocc_wr_tiles_to_process_thres_wr_sent(0, 15); // { dg-final { "scan-assembler" "tt\.rocc\.wr_tiles_to_process_thres_wr_sent" } }
  __builtin_riscv_ttrocc_wr_tiles_to_process_thres_idma_tr_ack(0, 15); // { dg-final { "scan-assembler" "tt\.rocc\.wr_tiles_to_process_thres_idma_tr_ack" } }

  unsigned long value0 = __builtin_riscv_ttrocc_rd_tiles_to_process_thres_tr_ack(0); // { dg-final { "scan-assembler" "tt\.rocc\.rd_tiles_to_process_thres_tr_ack" } }
  unsigned long value1 = __builtin_riscv_ttrocc_rd_tiles_to_process_thres_wr_sent(0); // { dg-final { "scan-assembler" "tt\.rocc\.rd_tiles_to_process_thres_wr_sent" } }
  unsigned long value2 = __builtin_riscv_ttrocc_rd_tiles_to_process_thres_idma_tr_ack(0); // { dg-final { "scan-assembler" "tt\.rocc\.rd_tiles_to_process_thres_idma_tr_ack" } }
}
