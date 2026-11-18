target:
    tt.rocc.scmdbuf_wr_reg                  x0, 0, a7, x0
    tt.rocc.scmdbuf_wr_reg                  x0, 48, a0, x0

    tt.rocc.scmdbuf_rd_reg                  a7,0, x0, x0
    tt.rocc.scmdbuf_rd_reg                  a6,48, x0,x0

    tt.rocc.scmdbuf_get_vc_space            a1
    tt.rocc.scmdbuf_get_vc_space            a7

    tt.rocc.scmdbuf_get_vc_space_vc         a1,a7

    tt.rocc.scmdbuf_wr_sent                 a1
    tt.rocc.scmdbuf_wr_sent                 a7

    tt.rocc.scmdbuf_wr_sent_trid            a1,a7
    tt.rocc.scmdbuf_wr_sent_trid            a7,a0

    tt.rocc.scmdbuf_tr_ack                  a1
    tt.rocc.scmdbuf_tr_ack                  a7

    tt.rocc.scmdbuf_tr_ack_trid             a1,a7
    tt.rocc.scmdbuf_tr_ack_trid             a7,a0

    tt.rocc.scmdbuf_reset

    tt.rocc.scmdbuf_issue_trans

    tt.rocc.scmdbuf_issue_inline_trans      a0
    tt.rocc.scmdbuf_issue_inline_trans      a7

    tt.rocc.scmdbuf_issue_inline_addr_trans a0,a7
    tt.rocc.scmdbuf_issue_inline_addr_trans a7,a0

    tt.rocc.scmdbuf_issue_read1_trans       a0
    tt.rocc.scmdbuf_issue_read1_trans       a7

    tt.rocc.scmdbuf_issue_read2_trans       a0,a7
    tt.rocc.scmdbuf_issue_read2_trans       a7,a0

    tt.rocc.scmdbuf_issue_write1_trans      a0
    tt.rocc.scmdbuf_issue_write1_trans      a7

    tt.rocc.scmdbuf_issue_write2_trans      a0,a7
    tt.rocc.scmdbuf_issue_write2_trans      a7,a0

    tt.rocc.scmdbuf_read_tiles_to_process_tr_ack      a0
    tt.rocc.scmdbuf_read_tiles_to_process_tr_ack      a6

    tt.rocc.scmdbuf_read_tiles_to_process_tr_ack_tr_id a0,a1
    tt.rocc.scmdbuf_read_tiles_to_process_tr_ack_tr_id a6,a7

    tt.rocc.scmdbuf_read_tiles_to_process_wr_sent a0
    tt.rocc.scmdbuf_read_tiles_to_process_wr_sent a6

    tt.rocc.scmdbuf_read_tiles_to_process_wr_sent_tr_id a0,a1
    tt.rocc.scmdbuf_read_tiles_to_process_wr_sent_tr_id a6,a7

    tt.rocc.scmdbuf_read_tiles_to_process_idma_tr_ack a0
    tt.rocc.scmdbuf_read_tiles_to_process_idma_tr_ack a6

    tt.rocc.scmdbuf_read_tiles_to_process_idma_tr_ack_tr_id a0,a1
    tt.rocc.scmdbuf_read_tiles_to_process_idma_tr_ack_tr_id a6,a7

    tt.rocc.scmdbuf_clear_tiles_to_process_tr_ack a1
    tt.rocc.scmdbuf_clear_tiles_to_process_tr_ack a7

    tt.rocc.scmdbuf_clear_tiles_to_process_wr_sent a1
    tt.rocc.scmdbuf_clear_tiles_to_process_wr_sent a7

    tt.rocc.scmdbuf_clear_tiles_to_process_idma_tr_ack a1
    tt.rocc.scmdbuf_clear_tiles_to_process_idma_tr_ack a7
