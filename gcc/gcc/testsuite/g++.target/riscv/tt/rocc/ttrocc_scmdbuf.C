// { dg-do-compile }
// { dg-additional-options "-mcpu=tt-qsr64-rocc" }

int main(void) {
  __builtin_riscv_ttrocc_scmdbuf_wr_reg(4, 15); // { dg-final { "scan-assembler" "tt\.rocc\.scmdbuf_wr_reg"} }

  unsigned long to_write = 15;
  __builtin_riscv_ttrocc_scmdbuf_wr_reg(4, to_write); // { dg-final { "scan-assembler" "tt\.rocc\.scmdbuf_wr_reg"} }

  unsigned long rd0 = __builtin_riscv_ttrocc_scmdbuf_rd_reg(4); // { dg-final { "scan-assembler" "tt\.rocc\.scmdbuf_rd_reg"} }

  unsigned long rd1 = __builtin_riscv_ttrocc_scmdbuf_get_vc_space(); // { dg-final { "scan-assembler" "tt\.rocc\.scmdbuf_get_vc_space"} }
  unsigned long rd2 = __builtin_riscv_ttrocc_scmdbuf_get_vc_space_vc(15); // { dg-final { "scan-assembler" "tt\.rocc\.scmdbuf_get_vc_space_vc"} }
  
  unsigned long rd3 = __builtin_riscv_ttrocc_scmdbuf_wr_sent(); // { dg-final { "scan-assembler" "tt\.rocc\.scmdbuf_wr_sent"} }
  unsigned long rd4 = __builtin_riscv_ttrocc_scmdbuf_wr_sent_trid(15); // { dg-final { "scan-assembler" "tt\.rocc\.scmdbuf_wr_sent_trid"} }
  
  unsigned long rd5 = __builtin_riscv_ttrocc_scmdbuf_tr_ack(); // { dg-final { "scan-assembler" "tt\.rocc\.scmdbuf_tr_ack"} }
  unsigned long rd6 = __builtin_riscv_ttrocc_scmdbuf_tr_ack_trid(15); // { dg-final { "scan-assembler" "tt\.rocc\.scmdbuf_tr_ack_trid"} }
  
  __builtin_riscv_ttrocc_scmdbuf_reset(); // { dg-final { "scan-assembler" "tt\.rocc\.scmdbuf_reset"} }

  unsigned long rd11 = __builtin_riscv_ttrocc_scmdbuf_read_tiles_to_process_tr_ack(); // { dg-final { "scan-assembler" "tt\.rocc\.scmdbuf_read_tiles_to_process_tr_ack" } }
  unsigned long rd12 = __builtin_riscv_ttrocc_scmdbuf_read_tiles_to_process_tr_ack_tr_id(15); // { dg-final { "scan-assembler" "tt\.rocc\.scmdbuf_read_tiles_to_process_tr_ack_tr_id" } }
  unsigned long rd13 = __builtin_riscv_ttrocc_scmdbuf_read_tiles_to_process_wr_sent(); // { dg-final { "scan-assembler" "tt\.rocc\.scmdbuf_read_tiles_to_process_wr_sent" } }
  unsigned long rd14 = __builtin_riscv_ttrocc_scmdbuf_read_tiles_to_process_wr_sent_tr_id(15); // { dg-final { "scan-assembler" "tt\.rocc\.scmdbuf_read_tiles_to_process_wr_sent_tr_id" } }
  unsigned long rd15 = __builtin_riscv_ttrocc_scmdbuf_read_tiles_to_process_idma_tr_ack(); // { dg-final { "scan-assembler" "tt\.rocc\.scmdbuf_read_tiles_to_process_idma_tr_ack" } }
  unsigned long rd16 = __builtin_riscv_ttrocc_scmdbuf_read_tiles_to_process_idma_tr_ack_tr_id(15); // { dg-final { "scan-assembler" "tt\.rocc\.scmdbuf_read_tiles_to_process_idma_tr_ack_tr_id" } }

  __builtin_riscv_ttrocc_scmdbuf_clear_tiles_to_process_tr_ack(15); // { dg-final { "scan-assembler" "tt\.rocc\.scmdbuf_clear_tiles_to_process_tr_ack" } }
  __builtin_riscv_ttrocc_scmdbuf_clear_tiles_to_process_wr_sent(15); // { dg-final { "scan-assembler" "tt\.rocc\.scmdbuf_clear_tiles_to_process_wr_sent" } }
  __builtin_riscv_ttrocc_scmdbuf_clear_tiles_to_process_idma_tr_ack(15); // { dg-final { "scan-assembler" "tt\.rocc\.scmdbuf_clear_tiles_to_process_idma_tr_ack" } }
}
