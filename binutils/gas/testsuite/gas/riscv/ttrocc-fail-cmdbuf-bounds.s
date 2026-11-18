target:
    tt.rocc.cmdbuf_wr_reg                   x0, 2, 0, a1, x0
    tt.rocc.cmdbuf_wr_reg                   x0, -1, 0, a1, x0
    tt.rocc.cmdbuf_wr_reg                   x0, 0, 49, a1, x0
    tt.rocc.cmdbuf_wr_reg                   x0, 0, -1, a1, x0

    tt.rocc.cmdbuf_rd_reg                   a0, 2, 0, x0, x0
    tt.rocc.cmdbuf_rd_reg                   a0, -1, 0, x0, x0
    tt.rocc.cmdbuf_rd_reg                   a0, 0, 49, x0, x0
    tt.rocc.cmdbuf_rd_reg                   a0, 0, -1, x0, x0

    tt.rocc.cmdbuf_get_vc_space             a0, 2
    tt.rocc.cmdbuf_get_vc_space             a0, -1

    tt.rocc.cmdbuf_get_vc_space_vc          a0, 2, a1
    tt.rocc.cmdbuf_get_vc_space_vc          a0, -1, a1

    tt.rocc.cmdbuf_wr_sent                  a0, 2
    tt.rocc.cmdbuf_wr_sent                  a0, -1

    tt.rocc.cmdbuf_wr_sent_trid             a0, 2, a1
    tt.rocc.cmdbuf_wr_sent_trid             a0, -1, a1

    tt.rocc.cmdbuf_tr_ack                   a0, 2
    tt.rocc.cmdbuf_tr_ack                   a0, -1

    tt.rocc.cmdbuf_tr_ack_trid              a0, 2, a1
    tt.rocc.cmdbuf_tr_ack_trid              a0, -1, a1

    tt.rocc.cmdbuf_reset                    2
    tt.rocc.cmdbuf_reset                    -1

    tt.rocc.cmdbuf_idma_get_vc_space        a0, 2
    tt.rocc.cmdbuf_idma_get_vc_space        a0, -1

    tt.rocc.cmdbuf_idma_get_vc_space_vc     a0, 2, a1
    tt.rocc.cmdbuf_idma_get_vc_space_vc     a0, -1, a1

    tt.rocc.cmdbuf_idma_tr_ack              a0, 2
    tt.rocc.cmdbuf_idma_tr_ack              a0, -1

    tt.rocc.cmdbuf_idma_tr_ack_trid         a0, 2, a1
    tt.rocc.cmdbuf_idma_tr_ack_trid         a0, -1, a1

    tt.rocc.cmdbuf_issue_trans              2
    tt.rocc.cmdbuf_issue_trans              -1

    tt.rocc.cmdbuf_issue_inline_trans       2,a1
    tt.rocc.cmdbuf_issue_inline_trans       -1,a1

    tt.rocc.cmdbuf_issue_inline_addr_trans  2,a1,a2
    tt.rocc.cmdbuf_issue_inline_addr_trans  -1,a1,a2

    tt.rocc.cmdbuf_issue_read1_trans        2,a1
    tt.rocc.cmdbuf_issue_read1_trans        -1,a1

    tt.rocc.cmdbuf_issue_read2_trans        2,a1,a2
    tt.rocc.cmdbuf_issue_read2_trans        -1,a1,a2

    tt.rocc.cmdbuf_issue_write1_trans       2,a1
    tt.rocc.cmdbuf_issue_write1_trans       -1,a1

    tt.rocc.cmdbuf_issue_write2_trans       2,a1,a2
    tt.rocc.cmdbuf_issue_write2_trans       -1,a1,a2

    tt.rocc.cmdbuf_read_tiles_to_process_tr_ack a0, 2
    tt.rocc.cmdbuf_read_tiles_to_process_tr_ack a7, -1

    tt.rocc.cmdbuf_read_tiles_to_process_tr_ack_tr_id a0,2,a1
    tt.rocc.cmdbuf_read_tiles_to_process_tr_ack_tr_id a6,-1,a7

    tt.rocc.cmdbuf_read_tiles_to_process_wr_sent a0,2
    tt.rocc.cmdbuf_read_tiles_to_process_wr_sent a7,-1

    tt.rocc.cmdbuf_read_tiles_to_process_wr_sent_tr_id a0,2,a1
    tt.rocc.cmdbuf_read_tiles_to_process_wr_sent_tr_id a6,-1,a7

    tt.rocc.cmdbuf_read_tiles_to_process_idma_tr_ack a0,2
    tt.rocc.cmdbuf_read_tiles_to_process_idma_tr_ack a6,-1

    tt.rocc.cmdbuf_read_tiles_to_process_idma_tr_ack_tr_id a0,2,a1
    tt.rocc.cmdbuf_read_tiles_to_process_idma_tr_ack_tr_id a6,-1,a7

    tt.rocc.cmdbuf_clear_tiles_to_process_tr_ack 2,a1
    tt.rocc.cmdbuf_clear_tiles_to_process_tr_ack -1,a7

    tt.rocc.cmdbuf_clear_tiles_to_process_wr_sent 2,a1
    tt.rocc.cmdbuf_clear_tiles_to_process_wr_sent -1,a7

    tt.rocc.cmdbuf_clear_tiles_to_process_idma_tr_ack 2,a1
    tt.rocc.cmdbuf_clear_tiles_to_process_idma_tr_ack -1,a7
