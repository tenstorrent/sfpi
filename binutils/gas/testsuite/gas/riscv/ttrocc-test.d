#as: -march=rv32im_xttroccqsr
#source: ttrocc-test.s
#objdump: -d

.*:[    ]+file format .*


Disassembly of section .text:

0+000 <target>:
[ 	]+0:[ 	]+a005a00b[ 	]+tt.rocc.cmdbuf_wr_reg[ 	]+zero,1,16,a1,zero
[ 	]+4:[ 	]+2073228b[ 	]+tt.rocc.cmdbuf_wr_reg[ 	]+t0,0,16,t1,t2
[ 	]+8:[ 	]+a073228b[ 	]+tt.rocc.cmdbuf_wr_reg[ 	]+t0,1,16,t1,t2
[ 	]+c:[ 	]+0ac5a50b[ 	]+tt.rocc.cmdbuf_wr_reg[ 	]+a0,0,5,a1,a2
[ 	]+10:[ 	]+8ac5a50b[ 	]+tt.rocc.cmdbuf_wr_reg[ 	]+a0,1,5,a1,a2
[ 	]+14:[ 	]+b000450b[ 	]+tt.rocc.cmdbuf_rd_reg[ 	]+a0,1,24,zero,zero
[ 	]+18:[ 	]+2073428b[ 	]+tt.rocc.cmdbuf_rd_reg[ 	]+t0,0,16,t1,t2
[ 	]+1c:[ 	]+a073428b[ 	]+tt.rocc.cmdbuf_rd_reg[ 	]+t0,1,16,t1,t2
[ 	]+20:[ 	]+0ac5c50b[ 	]+tt.rocc.cmdbuf_rd_reg[ 	]+a0,0,5,a1,a2
[ 	]+24:[ 	]+8ac5c50b[ 	]+tt.rocc.cmdbuf_rd_reg[ 	]+a0,1,5,a1,a2
[ 	]+28:[ 	]+fc00450b[ 	]+tt.rocc.cmdbuf_get_vc_space[ 	]+a0,1
[ 	]+2c:[ 	]+7c00450b[ 	]+tt.rocc.cmdbuf_get_vc_space[ 	]+a0,0
[ 	]+30:[ 	]+fc00468b[ 	]+tt.rocc.cmdbuf_get_vc_space[ 	]+a3,1
[ 	]+34:[ 	]+7c00468b[ 	]+tt.rocc.cmdbuf_get_vc_space[ 	]+a3,0
[ 	]+38:[ 	]+7c05e50b[ 	]+tt.rocc.cmdbuf_get_vc_space_vc[ 	]+a0,0,a1
[ 	]+3c:[ 	]+fc05e50b[ 	]+tt.rocc.cmdbuf_get_vc_space_vc[ 	]+a0,1,a1
[ 	]+40:[ 	]+7a00450b[ 	]+tt.rocc.cmdbuf_wr_sent[ 	]+a0,0
[ 	]+44:[ 	]+fa00450b[ 	]+tt.rocc.cmdbuf_wr_sent[ 	]+a0,1
[ 	]+48:[ 	]+7a05e50b[ 	]+tt.rocc.cmdbuf_wr_sent_trid[ 	]+a0,0,a1
[ 	]+4c:[ 	]+fa05e50b[ 	]+tt.rocc.cmdbuf_wr_sent_trid[ 	]+a0,1,a1
[ 	]+50:[ 	]+7800450b[ 	]+tt.rocc.cmdbuf_tr_ack[ 	]+a0,0
[ 	]+54:[ 	]+f800450b[ 	]+tt.rocc.cmdbuf_tr_ack[ 	]+a0,1
[ 	]+58:[ 	]+7805e50b[ 	]+tt.rocc.cmdbuf_tr_ack_trid[ 	]+a0,0,a1
[ 	]+5c:[ 	]+f805e50b[ 	]+tt.rocc.cmdbuf_tr_ack_trid[ 	]+a0,1,a1
[ 	]+60:[ 	]+7600000b[ 	]+tt.rocc.cmdbuf_reset[ 	]+0
[ 	]+64:[ 	]+f600000b[ 	]+tt.rocc.cmdbuf_reset[ 	]+1
[ 	]+68:[ 	]+7400450b[ 	]+tt.rocc.cmdbuf_idma_get_vc_space[ 	]+a0,0
[ 	]+6c:[ 	]+f400450b[ 	]+tt.rocc.cmdbuf_idma_get_vc_space[ 	]+a0,1
[ 	]+70:[ 	]+7405e50b[ 	]+tt.rocc.cmdbuf_idma_get_vc_space_vc[ 	]+a0,0,a1
[ 	]+74:[ 	]+f405e50b[ 	]+tt.rocc.cmdbuf_idma_get_vc_space_vc[ 	]+a0,1,a1
[ 	]+78:[ 	]+7200450b[ 	]+tt.rocc.cmdbuf_idma_tr_ack[ 	]+a0,0
[ 	]+7c:[ 	]+f200450b[ 	]+tt.rocc.cmdbuf_idma_tr_ack[ 	]+a0,1
[ 	]+80:[ 	]+7205e50b[ 	]+tt.rocc.cmdbuf_idma_tr_ack_trid[ 	]+a0,0,a1
[ 	]+84:[ 	]+f205e50b[ 	]+tt.rocc.cmdbuf_idma_tr_ack_trid[ 	]+a0,1,a1
[ 	]+88:[ 	]+7e00000b[ 	]+tt.rocc.cmdbuf_issue_trans[ 	]+0
[ 	]+8c:[ 	]+fe00000b[ 	]+tt.rocc.cmdbuf_issue_trans[ 	]+1
[ 	]+90:[ 	]+7e05a00b[ 	]+tt.rocc.cmdbuf_issue_inline_trans[ 	]+0,a1
[ 	]+94:[ 	]+fe05a00b[ 	]+tt.rocc.cmdbuf_issue_inline_trans[ 	]+1,a1
[ 	]+98:[ 	]+7ec5b00b[ 	]+tt.rocc.cmdbuf_issue_inline_addr_trans[ 	]+0,a1,a2
[ 	]+9c:[ 	]+fec5b00b[ 	]+tt.rocc.cmdbuf_issue_inline_addr_trans[ 	]+1,a1,a2
[ 	]+a0:[ 	]+7005a00b[ 	]+tt.rocc.cmdbuf_issue_read1_trans[ 	]+0,a1
[ 	]+a4:[ 	]+f005a00b[ 	]+tt.rocc.cmdbuf_issue_read1_trans[ 	]+1,a1
[ 	]+a8:[ 	]+70c5b00b[ 	]+tt.rocc.cmdbuf_issue_read2_trans[ 	]+0,a1,a2
[ 	]+ac:[ 	]+f0c5b00b[ 	]+tt.rocc.cmdbuf_issue_read2_trans[ 	]+1,a1,a2
[ 	]+b0:[ 	]+6e05a00b[ 	]+tt.rocc.cmdbuf_issue_write1_trans[ 	]+0,a1
[ 	]+b4:[ 	]+ee05a00b[ 	]+tt.rocc.cmdbuf_issue_write1_trans[ 	]+1,a1
[ 	]+b8:[ 	]+6ec5b00b[ 	]+tt.rocc.cmdbuf_issue_write2_trans[ 	]+0,a1,a2
[ 	]+bc:[ 	]+eec5b00b[ 	]+tt.rocc.cmdbuf_issue_write2_trans[ 	]+1,a1,a2
[ 	]+c0:[ 	]+4205a05b[ 	]+tt.rocc.dbg_postcode[ 	]+a1
[ 	]+c4:[ 	]+4400005b[ 	]+tt.rocc.noc_fence
[ 	]+c8:[ 	]+46c5b05b[ 	]+tt.rocc.llk_intf_write[ 	]+a1,a2
[ 	]+cc:[ 	]+4605e55b[ 	]+tt.rocc.llk_intf_read[ 	]+a0,a1
[ 	]+d0:[ 	]+48c5b05b[ 	]+tt.rocc.fds_intf_write[ 	]+a1,a2
[ 	]+d4:[ 	]+4805e55b[ 	]+tt.rocc.fds_intf_read[ 	]+a0,a1
[ 	]+d8:[ 	]+0600455b[ 	]+tt.rocc.cs_alloc[ 	]+a0
[ 	]+dc:[ 	]+0405a05b[ 	]+tt.rocc.cs_dealloc[ 	]+a1
[ 	]+e0:[ 	]+0205a05b[ 	]+tt.rocc.cs_save[ 	]+a1
[ 	]+e4:[ 	]+0005a05b[ 	]+tt.rocc.cs_restore[ 	]+a1
[ 	]+e8:[ 	]+8405a02b[ 	]+tt.rocc.scmdbuf_wr_reg[ 	]+zero,2,a1,zero
[ 	]+ec:[ 	]+8600452b[ 	]+tt.rocc.scmdbuf_rd_reg[ 	]+a0,3,zero,zero
[ 	]+f0:[ 	]+fc00452b[ 	]+tt.rocc.scmdbuf_get_vc_space[ 	]+a0
[ 	]+f4:[ 	]+fc05e52b[ 	]+tt.rocc.scmdbuf_get_vc_space_vc[ 	]+a0,a1
[ 	]+f8:[ 	]+fa00452b[ 	]+tt.rocc.scmdbuf_wr_sent[ 	]+a0
[ 	]+fc:[ 	]+fa05e52b[ 	]+tt.rocc.scmdbuf_wr_sent_trid[ 	]+a0,a1
[ 	]+100:[ 	]+f800452b[ 	]+tt.rocc.scmdbuf_tr_ack[ 	]+a0
[ 	]+104:[ 	]+f805e52b[ 	]+tt.rocc.scmdbuf_tr_ack_trid[ 	]+a0,a1
[ 	]+108:[ 	]+f600002b[ 	]+tt.rocc.scmdbuf_reset
[ 	]+10c:[ 	]+fe00002b[ 	]+tt.rocc.scmdbuf_issue_trans
[ 	]+110:[ 	]+fe05a02b[ 	]+tt.rocc.scmdbuf_issue_inline_trans[ 	]+a1
[ 	]+114:[ 	]+fec5b02b[ 	]+tt.rocc.scmdbuf_issue_inline_addr_trans[ 	]+a1,a2
[ 	]+118:[ 	]+f005a02b[ 	]+tt.rocc.scmdbuf_issue_read1_trans[ 	]+a1
[ 	]+11c:[ 	]+f0c5b02b[ 	]+tt.rocc.scmdbuf_issue_read2_trans[ 	]+a1,a2
[ 	]+120:[ 	]+ee05a02b[ 	]+tt.rocc.scmdbuf_issue_write1_trans[ 	]+a1
[ 	]+124:[ 	]+eec5b02b[ 	]+tt.rocc.scmdbuf_issue_write2_trans[ 	]+a1,a2
[ 	]+128:[ 	]+4405a02b[ 	]+tt.rocc.addrgen_wr_reg[ 	]+zero,1,2,a1,zero
[ 	]+12c:[ 	]+0200452b[ 	]+tt.rocc.addrgen_rd_reg[ 	]+a0,0,1,zero,zero
[ 	]+130:[ 	]+7400002b[ 	]+tt.rocc.addrgen_reset[ 	]+1
[ 	]+134:[ 	]+3400202b[ 	]+tt.rocc.addrgen_reset_counters[ 	]+0,zero
[ 	]+138:[ 	]+3e00452b[ 	]+tt.rocc.addrgen_peek_src[ 	]+a0,0
[ 	]+13c:[ 	]+7e00e52b[ 	]+tt.rocc.addrgen_pop_src[ 	]+a0,1
[ 	]+140:[ 	]+3e05e52b[ 	]+tt.rocc.addrgen_pop_x_src[ 	]+a0,0,a1
[ 	]+144:[ 	]+3c00e52b[ 	]+tt.rocc.addrgen_pop_dest[ 	]+a0,0
[ 	]+148:[ 	]+3c05e52b[ 	]+tt.rocc.addrgen_pop_x_dest[ 	]+a0,0,a1
[ 	]+14c:[ 	]+72c5f52b[ 	]+tt.rocc.addrgen_pop_both[ 	]+a0,1,a1,a2
[ 	]+150:[ 	]+3800002b[ 	]+tt.rocc.addrgen_push_src[ 	]+0
[ 	]+154:[ 	]+7805a02b[ 	]+tt.rocc.addrgen_push_src_pop_x[ 	]+1,a1
[ 	]+158:[ 	]+3600002b[ 	]+tt.rocc.addrgen_push_dest[ 	]+0
[ 	]+15c:[ 	]+3605a02b[ 	]+tt.rocc.addrgen_push_dest_pop_x[ 	]+0,a1
[ 	]+160:[ 	]+7a00002b[ 	]+tt.rocc.addrgen_push_both[ 	]+1
[ 	]+164:[ 	]+3ac5b02b[ 	]+tt.rocc.addrgen_push_both_pop_x[ 	]+0,a1,a2
