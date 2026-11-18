target:
    tt.rocc.cmdbuf_wr_reg                   x0, 1, 16, a1, x0
    tt.rocc.cmdbuf_wr_reg                   x5, 0, 16, x6, x7
    tt.rocc.cmdbuf_wr_reg                   x5, 1, 16, x6, x7
    tt.rocc.cmdbuf_wr_reg                   a0, 0, 5, a1, a2
    tt.rocc.cmdbuf_wr_reg                   a0, 1, 5, a1, a2

    tt.rocc.cmdbuf_rd_reg                   a0, 1, 24, x0, x0
    tt.rocc.cmdbuf_rd_reg                   x5, 0, 16, x6, x7
    tt.rocc.cmdbuf_rd_reg                   x5, 1, 16, x6, x7
    tt.rocc.cmdbuf_rd_reg                   a0, 0, 5, a1, a2
    tt.rocc.cmdbuf_rd_reg                   a0, 1, 5, a1, a2

    tt.rocc.cmdbuf_get_vc_space             a0, 1
    tt.rocc.cmdbuf_get_vc_space             a0, 0
    tt.rocc.cmdbuf_get_vc_space             a3, 1
    tt.rocc.cmdbuf_get_vc_space             a3, 0

    tt.rocc.cmdbuf_get_vc_space_vc          a0, 0, a1
    tt.rocc.cmdbuf_get_vc_space_vc          a0, 1, a1

    tt.rocc.cmdbuf_wr_sent                  a0, 0
    tt.rocc.cmdbuf_wr_sent                  a0, 1

    tt.rocc.cmdbuf_wr_sent_trid             a0, 0, a1
    tt.rocc.cmdbuf_wr_sent_trid             a0, 1, a1

    tt.rocc.cmdbuf_tr_ack                   a0, 0
    tt.rocc.cmdbuf_tr_ack                   a0, 1

    tt.rocc.cmdbuf_tr_ack_trid              a0, 0, a1
    tt.rocc.cmdbuf_tr_ack_trid              a0, 1, a1

    tt.rocc.cmdbuf_reset                    0
    tt.rocc.cmdbuf_reset                    1

    tt.rocc.cmdbuf_idma_get_vc_space        a0, 0
    tt.rocc.cmdbuf_idma_get_vc_space        a0, 1

    tt.rocc.cmdbuf_idma_get_vc_space_vc     a0, 0, a1
    tt.rocc.cmdbuf_idma_get_vc_space_vc     a0, 1, a1

    tt.rocc.cmdbuf_idma_tr_ack              a0, 0
    tt.rocc.cmdbuf_idma_tr_ack              a0, 1

    tt.rocc.cmdbuf_idma_tr_ack_trid         a0, 0, a1
    tt.rocc.cmdbuf_idma_tr_ack_trid         a0, 1, a1

    tt.rocc.cmdbuf_issue_trans              0
    tt.rocc.cmdbuf_issue_trans              1

    tt.rocc.cmdbuf_issue_inline_trans       0,a1
    tt.rocc.cmdbuf_issue_inline_trans       1,a1

    tt.rocc.cmdbuf_issue_inline_addr_trans         0,a1,a2
    tt.rocc.cmdbuf_issue_inline_addr_trans         1,a1,a2

    tt.rocc.cmdbuf_issue_read1_trans        0,a1
    tt.rocc.cmdbuf_issue_read1_trans        1,a1

    tt.rocc.cmdbuf_issue_read2_trans        0,a1,a2
    tt.rocc.cmdbuf_issue_read2_trans        1,a1,a2

    tt.rocc.cmdbuf_issue_write1_trans       0,a1
    tt.rocc.cmdbuf_issue_write1_trans       1,a1

    tt.rocc.cmdbuf_issue_write2_trans       0,a1,a2
    tt.rocc.cmdbuf_issue_write2_trans       1,a1,a2

    tt.rocc.dbg_postcode                    a1
    tt.rocc.noc_fence
    tt.rocc.llk_intf_write                  a1, a2
    tt.rocc.llk_intf_read                   a0, a1
    tt.rocc.fds_intf_write                  a1, a2
    tt.rocc.fds_intf_read                   a0, a1

    tt.rocc.cs_alloc                        a0
    tt.rocc.cs_dealloc                      a1
    tt.rocc.cs_save                         a1
    tt.rocc.cs_restore                      a1

    tt.rocc.scmdbuf_wr_reg                  x0, 2, a1, x0
    tt.rocc.scmdbuf_rd_reg                  a0, 3, x0, x0
    tt.rocc.scmdbuf_get_vc_space            a0
    tt.rocc.scmdbuf_get_vc_space_vc         a0,a1
    tt.rocc.scmdbuf_wr_sent                 a0
    tt.rocc.scmdbuf_wr_sent_trid            a0,a1
    tt.rocc.scmdbuf_tr_ack                  a0
    tt.rocc.scmdbuf_tr_ack_trid             a0,a1
    tt.rocc.scmdbuf_reset
    tt.rocc.scmdbuf_issue_trans
    tt.rocc.scmdbuf_issue_inline_trans      a1
    tt.rocc.scmdbuf_issue_inline_addr_trans a1,a2
    tt.rocc.scmdbuf_issue_read1_trans       a1
    tt.rocc.scmdbuf_issue_read2_trans       a1,a2
    tt.rocc.scmdbuf_issue_write1_trans      a1
    tt.rocc.scmdbuf_issue_write2_trans      a1,a2

    tt.rocc.addrgen_wr_reg                  x0, 1, 2, a1, x0
    tt.rocc.addrgen_rd_reg                  a0, 0, 1, x0, x0
    tt.rocc.addrgen_reset                   1
    tt.rocc.addrgen_reset_counters          0, x0
    tt.rocc.addrgen_peek_src                a0, 0
    tt.rocc.addrgen_pop_src                 a0, 1
    tt.rocc.addrgen_pop_x_src               a0, 0, a1
    tt.rocc.addrgen_pop_dest                a0, 0
    tt.rocc.addrgen_pop_x_dest              a0, 0, a1
    tt.rocc.addrgen_pop_both                a0, 1, a1, a2
    tt.rocc.addrgen_push_src                0
    tt.rocc.addrgen_push_src_pop_x          1,a1
    tt.rocc.addrgen_push_dest               0
    tt.rocc.addrgen_push_dest_pop_x         0,a1
    tt.rocc.addrgen_push_both               1
    tt.rocc.addrgen_push_both_pop_x         0,a1, a2
