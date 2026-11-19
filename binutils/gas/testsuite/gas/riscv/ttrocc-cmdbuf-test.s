target:
    tt.rocc.cmdbuf_wr_reg                   x0, 0, 8, a7, x0

    tt.rocc.cmdbuf_rd_reg                   a7, 1, 5, x0, x0

    tt.rocc.cmdbuf_get_vc_space             a7,0

    tt.rocc.cmdbuf_get_vc_space_vc          a7, 1, a6

    tt.rocc.cmdbuf_wr_sent                  a7,1

    tt.rocc.cmdbuf_wr_sent_trid             a7,1,a6

    tt.rocc.cmdbuf_tr_ack                   a7,0

    tt.rocc.cmdbuf_tr_ack_trid              a7,1,a6

    tt.rocc.cmdbuf_reset                    0
    tt.rocc.cmdbuf_reset                    1

    tt.rocc.cmdbuf_idma_get_vc_space        a7,1
    tt.rocc.cmdbuf_idma_get_vc_space        a4,0

    tt.rocc.cmdbuf_idma_get_vc_space_vc     a7, 1, a6

    tt.rocc.cmdbuf_idma_tr_ack              a7,0

    tt.rocc.cmdbuf_idma_tr_ack_trid         a7,1,a6

    tt.rocc.cmdbuf_issue_trans              0
    tt.rocc.cmdbuf_issue_trans              1

    tt.rocc.cmdbuf_issue_inline_trans       0,a7

    tt.rocc.cmdbuf_issue_inline_addr_trans         1,a7,a6
    tt.rocc.cmdbuf_issue_inline_addr_trans         0,a6,a5

    tt.rocc.cmdbuf_issue_read1_trans        1,a7
    tt.rocc.cmdbuf_issue_read1_trans        0,a6

    tt.rocc.cmdbuf_issue_read2_trans        0,a6,a7
    tt.rocc.cmdbuf_issue_read2_trans        1,a5,a6

    tt.rocc.cmdbuf_issue_write1_trans       1,a7
    tt.rocc.cmdbuf_issue_write1_trans       0,a6

    tt.rocc.cmdbuf_issue_write2_trans       1,a6,a7
    tt.rocc.cmdbuf_issue_write2_trans       0,a5,a6

    tt.rocc.cmdbuf_read_tiles_to_process_tr_ack a0, 0
    tt.rocc.cmdbuf_read_tiles_to_process_tr_ack a7, 1

    tt.rocc.cmdbuf_read_tiles_to_process_tr_ack_tr_id a0,0,a1
    tt.rocc.cmdbuf_read_tiles_to_process_tr_ack_tr_id a6,1,a7

    tt.rocc.cmdbuf_read_tiles_to_process_wr_sent a0,0
    tt.rocc.cmdbuf_read_tiles_to_process_wr_sent a7,1

    tt.rocc.cmdbuf_read_tiles_to_process_wr_sent_tr_id a0,0,a1
    tt.rocc.cmdbuf_read_tiles_to_process_wr_sent_tr_id a6,1,a7

    tt.rocc.cmdbuf_read_tiles_to_process_idma_tr_ack a0,0
    tt.rocc.cmdbuf_read_tiles_to_process_idma_tr_ack a6,1

    tt.rocc.cmdbuf_read_tiles_to_process_idma_tr_ack_tr_id a0,0,a1
    tt.rocc.cmdbuf_read_tiles_to_process_idma_tr_ack_tr_id a6,1,a7

    tt.rocc.cmdbuf_clear_tiles_to_process_tr_ack 0,a1
    tt.rocc.cmdbuf_clear_tiles_to_process_tr_ack 1,a7

    tt.rocc.cmdbuf_clear_tiles_to_process_wr_sent 0,a1
    tt.rocc.cmdbuf_clear_tiles_to_process_wr_sent 1,a7

    tt.rocc.cmdbuf_clear_tiles_to_process_idma_tr_ack 0,a1
    tt.rocc.cmdbuf_clear_tiles_to_process_idma_tr_ack 1,a7
