target:
    tt.rocc.dbg_postcode                    a0
    tt.rocc.dbg_postcode                    a1
    tt.rocc.dbg_postcode                    a2
    tt.rocc.dbg_postcode                    a3
    tt.rocc.dbg_postcode                    a4
    tt.rocc.dbg_postcode                    a5
    tt.rocc.dbg_postcode                    a6
    tt.rocc.dbg_postcode                    a7
    tt.rocc.dbg_postcode                    x0
    tt.rocc.dbg_postcode                    x31

    tt.rocc.noc_fence

    tt.rocc.llk_intf_write                  a0, a0
    tt.rocc.llk_intf_write                  a1, a0
    tt.rocc.llk_intf_write                  a0, a1
    tt.rocc.llk_intf_write                  a1, a1
    tt.rocc.llk_intf_write                  a1, a2
    tt.rocc.llk_intf_write                  a1, a3
    tt.rocc.llk_intf_write                  a1, a4
    tt.rocc.llk_intf_write                  a1, a5
    tt.rocc.llk_intf_write                  a1, a6
    tt.rocc.llk_intf_write                  a1, a7
    tt.rocc.llk_intf_write                  a2, a1
    tt.rocc.llk_intf_write                  a3, a1
    tt.rocc.llk_intf_write                  a4, a1
    tt.rocc.llk_intf_write                  a5, a1
    tt.rocc.llk_intf_write                  a6, a1
    tt.rocc.llk_intf_write                  a7, a1
    tt.rocc.llk_intf_write                  x0, x0
    tt.rocc.llk_intf_write                  x0, x31
    tt.rocc.llk_intf_write                  x31, x0
    tt.rocc.llk_intf_write                  x31, x31

    tt.rocc.llk_intf_read                  a0, a0
    tt.rocc.llk_intf_read                  a1, a0
    tt.rocc.llk_intf_read                  a0, a1
    tt.rocc.llk_intf_read                  a1, a1
    tt.rocc.llk_intf_read                  a1, a2
    tt.rocc.llk_intf_read                  a1, a3
    tt.rocc.llk_intf_read                  a1, a4
    tt.rocc.llk_intf_read                  a1, a5
    tt.rocc.llk_intf_read                  a1, a6
    tt.rocc.llk_intf_read                  a1, a7
    tt.rocc.llk_intf_read                  a2, a1
    tt.rocc.llk_intf_read                  a3, a1
    tt.rocc.llk_intf_read                  a4, a1
    tt.rocc.llk_intf_read                  a5, a1
    tt.rocc.llk_intf_read                  a6, a1
    tt.rocc.llk_intf_read                  a7, a1
    tt.rocc.llk_intf_read                  x0, x0
    tt.rocc.llk_intf_read                  x0, x31
    tt.rocc.llk_intf_read                  x31, x0
    tt.rocc.llk_intf_read                  x31, x31

    tt.rocc.fds_intf_write                  a0, a0
    tt.rocc.fds_intf_write                  a1, a0
    tt.rocc.fds_intf_write                  a0, a1
    tt.rocc.fds_intf_write                  a1, a1
    tt.rocc.fds_intf_write                  a1, a2
    tt.rocc.fds_intf_write                  a1, a3
    tt.rocc.fds_intf_write                  a1, a4
    tt.rocc.fds_intf_write                  a1, a5
    tt.rocc.fds_intf_write                  a1, a6
    tt.rocc.fds_intf_write                  a1, a7
    tt.rocc.fds_intf_write                  a2, a1
    tt.rocc.fds_intf_write                  a3, a1
    tt.rocc.fds_intf_write                  a4, a1
    tt.rocc.fds_intf_write                  a5, a1
    tt.rocc.fds_intf_write                  a6, a1
    tt.rocc.fds_intf_write                  a7, a1
    tt.rocc.fds_intf_write                  x0, x0
    tt.rocc.fds_intf_write                  x0, x31
    tt.rocc.fds_intf_write                  x31, x0
    tt.rocc.fds_intf_write                  x31, x31

    tt.rocc.fds_intf_read                  a0, a0
    tt.rocc.fds_intf_read                  a1, a0
    tt.rocc.fds_intf_read                  a0, a1
    tt.rocc.fds_intf_read                  a1, a1
    tt.rocc.fds_intf_read                  a1, a2
    tt.rocc.fds_intf_read                  a1, a3
    tt.rocc.fds_intf_read                  a1, a4
    tt.rocc.fds_intf_read                  a1, a5
    tt.rocc.fds_intf_read                  a1, a6
    tt.rocc.fds_intf_read                  a1, a7
    tt.rocc.fds_intf_read                  a2, a1
    tt.rocc.fds_intf_read                  a3, a1
    tt.rocc.fds_intf_read                  a4, a1
    tt.rocc.fds_intf_read                  a5, a1
    tt.rocc.fds_intf_read                  a6, a1
    tt.rocc.fds_intf_read                  a7, a1
    tt.rocc.fds_intf_read                  x0, x0
    tt.rocc.fds_intf_read                  x0, x31
    tt.rocc.fds_intf_read                  x31, x0
    tt.rocc.fds_intf_read                  x31, x31

    tt.rocc.wr_tiles_to_process_thres_tr_ack       a1,a2
    tt.rocc.wr_tiles_to_process_thres_tr_ack       a6,a7

    tt.rocc.wr_tiles_to_process_thres_wr_sent      a1,a2
    tt.rocc.wr_tiles_to_process_thres_wr_sent      a6,a7

    tt.rocc.wr_tiles_to_process_thres_idma_tr_ack  a1,a2
    tt.rocc.wr_tiles_to_process_thres_idma_tr_ack  a6,a7

    tt.rocc.rd_tiles_to_process_thres_tr_ack       a0,a1
    tt.rocc.rd_tiles_to_process_thres_tr_ack       a6,a7

    tt.rocc.rd_tiles_to_process_thres_wr_sent      a0,a1
    tt.rocc.rd_tiles_to_process_thres_wr_sent      a6,a7

    tt.rocc.rd_tiles_to_process_thres_idma_tr_ack  a0,a1
    tt.rocc.rd_tiles_to_process_thres_idma_tr_ack  a6,a7
