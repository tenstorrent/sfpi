target:
    tt.rocc.addrgen_wr_reg                  x0, 2, 0, a1, x0
    tt.rocc.addrgen_wr_reg                  x0, -1, 0, a1, x0
    tt.rocc.addrgen_wr_reg                  x0, 0, 25, a1, x0
    tt.rocc.addrgen_wr_reg                  x0, 0, -1, a1, x0

    tt.rocc.addrgen_rd_reg                  a0, 2, 0, x0, x0
    tt.rocc.addrgen_rd_reg                  a0, -1, 0, x0, x0
    tt.rocc.addrgen_rd_reg                  a0, 0, 25, x0, x0
    tt.rocc.addrgen_rd_reg                  a0, 0, -1, x0, x0

    tt.rocc.addrgen_reset                   2
    tt.rocc.addrgen_reset                   -1

    tt.rocc.addrgen_reset_counters          2, x0
    tt.rocc.addrgen_reset_counters          -1, x0

    tt.rocc.addrgen_peek_src                a0, 2
    tt.rocc.addrgen_peek_src                a0, -1

    tt.rocc.addrgen_pop_src                 a0, 2
    tt.rocc.addrgen_pop_src                 a0, -1

    tt.rocc.addrgen_pop_x_src               a0, 2, a1
    tt.rocc.addrgen_pop_x_src               a0, -1, a1

    tt.rocc.addrgen_pop_dest                a0, 2
    tt.rocc.addrgen_pop_dest                a0, -1

    tt.rocc.addrgen_pop_x_dest              a0, 2, a1
    tt.rocc.addrgen_pop_x_dest              a0, -1, a1

    tt.rocc.addrgen_pop_both                a0, 2, a1, a2
    tt.rocc.addrgen_pop_both                a0, -1, a1, a2

    tt.rocc.addrgen_push_src                2
    tt.rocc.addrgen_push_src                -1

    tt.rocc.addrgen_push_src_pop_x          2, a1
    tt.rocc.addrgen_push_src_pop_x          -1, a1

    tt.rocc.addrgen_push_dest               2
    tt.rocc.addrgen_push_dest               -1

    tt.rocc.addrgen_push_dest_pop_x         2, a1
    tt.rocc.addrgen_push_dest_pop_x         -1, a1

    tt.rocc.addrgen_push_both               2
    tt.rocc.addrgen_push_both               -1

    tt.rocc.addrgen_push_both_pop_x         2, a1, a2
    tt.rocc.addrgen_push_both_pop_x         -1, a1, a2
