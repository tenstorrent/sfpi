// { dg-do-compile }
// { dg-additional-options "-mcpu=tt-qsr64-rocc" }

int main(void) {
  __builtin_riscv_ttrocc_cmdbuf_wr_reg(0, 4, 15); // { dg-final { "scan-assembler" "tt\.rocc\.cmdbuf_wr_reg" } }

  unsigned long to_write = 15;
  __builtin_riscv_ttrocc_cmdbuf_wr_reg(0, 4, to_write); // { dg-final { "scan-assembler" "tt\.rocc\.cmdbuf_wr_reg" } }

  unsigned long rd0 = __builtin_riscv_ttrocc_cmdbuf_rd_reg(0, 4); // { dg-final { "scan-assembler" "tt\.rocc\.cmdbuf_rd_reg" } }

  unsigned long rd1 = __builtin_riscv_ttrocc_cmdbuf_get_vc_space(0); // { dg-final { "scan-assembler" "tt\.rocc\.cmdbuf_get_vc_space" } }
  unsigned long rd2 = __builtin_riscv_ttrocc_cmdbuf_get_vc_space_vc(0, 15); // { dg-final { "scan-assembler" "tt\.rocc\.cmdbuf_get_vc_space_vc" } }
  
  unsigned long rd3 = __builtin_riscv_ttrocc_cmdbuf_wr_sent(0); // { dg-final { "scan-assembler" "tt\.rocc\.cmdbuf_wr_sent" } }
  unsigned long rd4 = __builtin_riscv_ttrocc_cmdbuf_wr_sent_trid(0, 15); // { dg-final { "scan-assembler" "tt\.rocc\.cmdbuf_wr_sent_trid" } }
  
  unsigned long rd5 = __builtin_riscv_ttrocc_cmdbuf_tr_ack(0); // { dg-final { "scan-assembler" "tt\.rocc\.cmdbuf_tr_ack" } }
  unsigned long rd6 = __builtin_riscv_ttrocc_cmdbuf_tr_ack_trid(0, 15); // { dg-final { "scan-assembler" "tt\.rocc\.cmdbuf_tr_ack_trid" } }
  
  __builtin_riscv_ttrocc_cmdbuf_reset(0); // { dg-final { "scan-assembler" "tt\.rocc\.cmdbuf_reset" } }
  
  unsigned long rd7 = __builtin_riscv_ttrocc_cmdbuf_idma_get_vc_space(0); // { dg-final { "scan-assembler" "tt\.rocc\.cmdbuf_idma_get_vc_space" } }
  unsigned long rd8 = __builtin_riscv_ttrocc_cmdbuf_idma_get_vc_space_vc(0, 15); // { dg-final { "scan-assembler" "tt\.rocc\.cmdbuf_idma_get_vc_space_vc" } }
  unsigned long rd9 = __builtin_riscv_ttrocc_cmdbuf_idma_tr_ack(0); // { dg-final { "scan-assembler" "tt\.rocc\.cmdbuf_idma_tr_ack" } }
  unsigned long rd10 = __builtin_riscv_ttrocc_cmdbuf_idma_tr_ack_trid(0, 15); // { dg-final { "scan-assembler" "tt\.rocc\.cmdbuf_idma_tr_ack_trid" } }

  unsigned long rd11 = __builtin_riscv_ttrocc_cmdbuf_read_tiles_to_process_tr_ack(0); // { dg-final { "scan-assembler" "tt\.rocc\.cmdbuf_read_tiles_to_process_tr_ack" } }
  unsigned long rd12 = __builtin_riscv_ttrocc_cmdbuf_read_tiles_to_process_tr_ack_tr_id(0, 0); // { dg-final { "scan-assembler" "tt\.rocc\.cmdbuf_read_tiles_to_process_tr_ack_tr_id" } }
  unsigned long rd13 = __builtin_riscv_ttrocc_cmdbuf_read_tiles_to_process_wr_sent(0); // { dg-final { "scan-assembler" "tt\.rocc\.cmdbuf_read_tiles_to_process_wr_sent" } }
  unsigned long rd14 = __builtin_riscv_ttrocc_cmdbuf_read_tiles_to_process_wr_sent_tr_id(0, 0); // { dg-final { "scan-assembler" "tt\.rocc\.cmdbuf_read_tiles_to_process_wr_sent_tr_id" } }
  unsigned long rd15 = __builtin_riscv_ttrocc_cmdbuf_read_tiles_to_process_idma_tr_ack(0); // { dg-final { "scan-assembler" "tt\.rocc\.cmdbuf_read_tiles_to_process_idma_tr_ack" } }
  unsigned long rd16 = __builtin_riscv_ttrocc_cmdbuf_read_tiles_to_process_idma_tr_ack_tr_id(0, 0); // { dg-final { "scan-assembler" "tt\.rocc\.cmdbuf_read_tiles_to_process_idma_tr_ack_tr_id" } }

  __builtin_riscv_ttrocc_cmdbuf_clear_tiles_to_process_tr_ack(0, 0); // { dg-final { "scan-assembler" "tt\.rocc\.cmdbuf_clear_tiles_to_process_tr_ack" } }
  __builtin_riscv_ttrocc_cmdbuf_clear_tiles_to_process_wr_sent(0, 0); // { dg-final { "scan-assembler" "tt\.rocc\.cmdbuf_clear_tiles_to_process_wr_sent" } }
  __builtin_riscv_ttrocc_cmdbuf_clear_tiles_to_process_idma_tr_ack(0, 0); // { dg-final { "scan-assembler" "tt\.rocc\.cmdbuf_clear_tiles_to_process_idma_tr_ack" } }
  
}
