target:
    tt.rocc.scmdbuf_wr_reg                  x0, 49, a1, x0
    tt.rocc.scmdbuf_wr_reg                  x0, -1, a1, x0

    tt.rocc.scmdbuf_rd_reg                  a0, 49, x0, x0
    tt.rocc.scmdbuf_rd_reg                  a0, -1, x0, x0
