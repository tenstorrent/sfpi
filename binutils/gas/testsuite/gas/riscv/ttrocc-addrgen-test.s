target:
    tt.rocc.addrgen_wr_reg                        zero, 1, 0, a0, x0
    tt.rocc.addrgen_wr_reg                        zero, 1, 24, a7, x0
    tt.rocc.addrgen_wr_reg                        zero, 1, 16, x0, x0
    tt.rocc.addrgen_wr_reg                        zero, 1, 8, x31, x0
    tt.rocc.addrgen_wr_reg                        zero, 0, 15, a0, x0
    tt.rocc.addrgen_wr_reg                        zero, 0, 0, a7, x0
    tt.rocc.addrgen_wr_reg                        zero, 0, 1, x0, x0
    tt.rocc.addrgen_wr_reg                        zero, 0, 19, x31, x0

    tt.rocc.addrgen_rd_reg                        a0, 1,0, x0, x0
    tt.rocc.addrgen_rd_reg                        a7, 1,24, x0, x0
    tt.rocc.addrgen_rd_reg                        x0, 1, 16, x0, x0
    tt.rocc.addrgen_rd_reg                        x31, 1, 8,x0, x0
    tt.rocc.addrgen_rd_reg                        a0, 0, 15, x0, x0
    tt.rocc.addrgen_rd_reg                        a7, 0, 0, x0, x0
    tt.rocc.addrgen_rd_reg                        x0, 0, 1, x0, x0
    tt.rocc.addrgen_rd_reg                        x31, 0, 19, x0, x0

    tt.rocc.addrgen_reset                         0
    tt.rocc.addrgen_reset                         1

    tt.rocc.addrgen_reset_counters                0, a0
    tt.rocc.addrgen_reset_counters                1, a0
    tt.rocc.addrgen_reset_counters                0, a7
    tt.rocc.addrgen_reset_counters                1, a7
    tt.rocc.addrgen_reset_counters                0, x0
    tt.rocc.addrgen_reset_counters                1, x0
    tt.rocc.addrgen_reset_counters                0, x31
    tt.rocc.addrgen_reset_counters                1, x31

    tt.rocc.addrgen_peek_src                a0, 0
    tt.rocc.addrgen_peek_src                a7, 0
    tt.rocc.addrgen_peek_src                x0, 0
    tt.rocc.addrgen_peek_src                x31, 0
    tt.rocc.addrgen_peek_src                a0, 1
    tt.rocc.addrgen_peek_src                a7, 1
    tt.rocc.addrgen_peek_src                x0, 1
    tt.rocc.addrgen_peek_src                x31, 1

    tt.rocc.addrgen_pop_src                a0, 0
    tt.rocc.addrgen_pop_src                a7, 0
    tt.rocc.addrgen_pop_src                x0, 0
    tt.rocc.addrgen_pop_src                x31, 0
    tt.rocc.addrgen_pop_src                a0, 1
    tt.rocc.addrgen_pop_src                a7, 1
    tt.rocc.addrgen_pop_src                x0, 1
    tt.rocc.addrgen_pop_src                x31, 1

    tt.rocc.addrgen_pop_x_src                a0,0,a0
    tt.rocc.addrgen_pop_x_src                a7,0,a0
    tt.rocc.addrgen_pop_x_src                a0,0,a7
    tt.rocc.addrgen_pop_x_src                x0,0,x31
    tt.rocc.addrgen_pop_x_src                x31,0,x0
    tt.rocc.addrgen_pop_x_src                a0,1,a0
    tt.rocc.addrgen_pop_x_src                a7,1,a0
    tt.rocc.addrgen_pop_x_src                a0,1,a7
    tt.rocc.addrgen_pop_x_src                x0,1,x31
    tt.rocc.addrgen_pop_x_src                x31,1,x0

    tt.rocc.addrgen_peek_dest                a0,0
    tt.rocc.addrgen_peek_dest                a7,0
    tt.rocc.addrgen_peek_dest                x0,0
    tt.rocc.addrgen_peek_dest                x31,0
    tt.rocc.addrgen_peek_dest                a0,1
    tt.rocc.addrgen_peek_dest                a7,1
    tt.rocc.addrgen_peek_dest                x0,1
    tt.rocc.addrgen_peek_dest                x31,1

    tt.rocc.addrgen_pop_dest                a0,0
    tt.rocc.addrgen_pop_dest                a7,0
    tt.rocc.addrgen_pop_dest                x0,0
    tt.rocc.addrgen_pop_dest                x31,0
    tt.rocc.addrgen_pop_dest                a0,1
    tt.rocc.addrgen_pop_dest                a7,1
    tt.rocc.addrgen_pop_dest                x0,1
    tt.rocc.addrgen_pop_dest                x31,1

    tt.rocc.addrgen_pop_x_dest              a7,1,a0
    tt.rocc.addrgen_pop_x_dest              x0,0,x31

    tt.rocc.addrgen_pop_both                a7,0,a0,a1
    tt.rocc.addrgen_pop_both                x31,1,x0,a7

    tt.rocc.addrgen_push_src                1

    tt.rocc.addrgen_push_src_pop_x          0,a7
    tt.rocc.addrgen_push_src_pop_x          1,a0

    tt.rocc.addrgen_push_dest               1

    tt.rocc.addrgen_push_dest_pop_x         1,a7
    tt.rocc.addrgen_push_dest_pop_x         0,a0

    tt.rocc.addrgen_push_both               0

    tt.rocc.addrgen_push_both_pop_x         1,a0,a1
    tt.rocc.addrgen_push_both_pop_x         0,a1,a7
