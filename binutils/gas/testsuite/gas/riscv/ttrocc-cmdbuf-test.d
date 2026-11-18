#as: -march=rv32im_xttroccqsr
#source: ttrocc-cmdbuf-test.s
#objdump: -d

.*:[    ]+file format .*


Disassembly of section .text:

0+000 <target>:
[ 	]+0:[ 	]+1008a00b[ 	]+tt.rocc.cmdbuf_wr_reg[ 	]+zero,0,8,a7,zero
[ 	]+4:[ 	]+8a00488b[ 	]+tt.rocc.cmdbuf_rd_reg[ 	]+a7,1,5,zero,zero
[ 	]+8:[ 	]+7c00488b[ 	]+tt.rocc.cmdbuf_get_vc_space[ 	]+a7,0
[ 	]+c:[ 	]+fc08688b[ 	]+tt.rocc.cmdbuf_get_vc_space_vc[ 	]+a7,1,a6
[ 	]+10:[ 	]+fa00488b[ 	]+tt.rocc.cmdbuf_wr_sent[ 	]+a7,1
[ 	]+14:[ 	]+fa08688b[ 	]+tt.rocc.cmdbuf_wr_sent_trid[ 	]+a7,1,a6
[ 	]+18:[ 	]+7800488b[ 	]+tt.rocc.cmdbuf_tr_ack[ 	]+a7,0
[ 	]+1c:[ 	]+f808688b[ 	]+tt.rocc.cmdbuf_tr_ack_trid[ 	]+a7,1,a6
[ 	]+20:[ 	]+7600000b[ 	]+tt.rocc.cmdbuf_reset[ 	]+0
[ 	]+24:[ 	]+f600000b[ 	]+tt.rocc.cmdbuf_reset[ 	]+1
[ 	]+28:[ 	]+f400488b[ 	]+tt.rocc.cmdbuf_idma_get_vc_space[ 	]+a7,1
[ 	]+2c:[ 	]+7400470b[ 	]+tt.rocc.cmdbuf_idma_get_vc_space[ 	]+a4,0
[ 	]+30:[ 	]+f408688b[ 	]+tt.rocc.cmdbuf_idma_get_vc_space_vc[ 	]+a7,1,a6
[ 	]+34:[ 	]+7200488b[ 	]+tt.rocc.cmdbuf_idma_tr_ack[ 	]+a7,0
[ 	]+38:[ 	]+f208688b[ 	]+tt.rocc.cmdbuf_idma_tr_ack_trid[ 	]+a7,1,a6
[ 	]+3c:[ 	]+7e00000b[ 	]+tt.rocc.cmdbuf_issue_trans[ 	]+0
[ 	]+40:[ 	]+fe00000b[ 	]+tt.rocc.cmdbuf_issue_trans[ 	]+1
[ 	]+44:[ 	]+7e08a00b[ 	]+tt.rocc.cmdbuf_issue_inline_trans[ 	]+0,a7
[ 	]+48:[ 	]+ff08b00b[ 	]+tt.rocc.cmdbuf_issue_inline_addr_trans[ 	]+1,a7,a6
[ 	]+4c:[ 	]+7ef8300b[ 	]+tt.rocc.cmdbuf_issue_inline_addr_trans[ 	]+0,a6,a5
[ 	]+50:[ 	]+f008a00b[ 	]+tt.rocc.cmdbuf_issue_read1_trans[ 	]+1,a7
[ 	]+54:[ 	]+7008200b[ 	]+tt.rocc.cmdbuf_issue_read1_trans[ 	]+0,a6
[ 	]+58:[ 	]+7118300b[ 	]+tt.rocc.cmdbuf_issue_read2_trans[ 	]+0,a6,a7
[ 	]+5c:[ 	]+f107b00b[ 	]+tt.rocc.cmdbuf_issue_read2_trans[ 	]+1,a5,a6
[ 	]+60:[ 	]+ee08a00b[ 	]+tt.rocc.cmdbuf_issue_write1_trans[ 	]+1,a7
[ 	]+64:[ 	]+6e08200b[ 	]+tt.rocc.cmdbuf_issue_write1_trans[ 	]+0,a6
[ 	]+68:[ 	]+ef18300b[ 	]+tt.rocc.cmdbuf_issue_write2_trans[ 	]+1,a6,a7
[ 	]+6c:[ 	]+6f07b00b[ 	]+tt.rocc.cmdbuf_issue_write2_trans[ 	]+0,a5,a6
[ 	]+70:[ 	]+6c00450b[ 	]+tt.rocc.cmdbuf_read_tiles_to_process_tr_ack[ 	]+a0,0
[ 	]+74:[ 	]+ec00488b[ 	]+tt.rocc.cmdbuf_read_tiles_to_process_tr_ack[ 	]+a7,1
[ 	]+78:[ 	]+6c05e50b[ 	]+tt.rocc.cmdbuf_read_tiles_to_process_tr_ack_tr_id[ 	]+a0,0,a1
[ 	]+7c:[ 	]+ec08e80b[ 	]+tt.rocc.cmdbuf_read_tiles_to_process_tr_ack_tr_id[ 	]+a6,1,a7
[ 	]+80:[ 	]+6a00450b[ 	]+tt.rocc.cmdbuf_read_tiles_to_process_wr_sent[ 	]+a0,0
[ 	]+84:[ 	]+ea00488b[ 	]+tt.rocc.cmdbuf_read_tiles_to_process_wr_sent[ 	]+a7,1
[ 	]+88:[ 	]+6a05e50b[ 	]+tt.rocc.cmdbuf_read_tiles_to_process_wr_sent_tr_id[ 	]+a0,0,a1
[ 	]+8c:[ 	]+ea08e80b[ 	]+tt.rocc.cmdbuf_read_tiles_to_process_wr_sent_tr_id[ 	]+a6,1,a7
[ 	]+90:[ 	]+6800450b[ 	]+tt.rocc.cmdbuf_read_tiles_to_process_idma_tr_ack[ 	]+a0,0
[ 	]+94:[ 	]+e800480b[ 	]+tt.rocc.cmdbuf_read_tiles_to_process_idma_tr_ack[ 	]+a6,1
[ 	]+98:[ 	]+6805e50b[ 	]+tt.rocc.cmdbuf_read_tiles_to_process_idma_tr_ack_tr_id[ 	]+a0,0,a1
[ 	]+9c:[ 	]+e808e80b[ 	]+tt.rocc.cmdbuf_read_tiles_to_process_idma_tr_ack_tr_id[ 	]+a6,1,a7
[ 	]+a0:[ 	]+6605a00b[ 	]+tt.rocc.cmdbuf_clear_tiles_to_process_tr_ack[ 	]+0,a1
[ 	]+a4:[ 	]+e608a00b[ 	]+tt.rocc.cmdbuf_clear_tiles_to_process_tr_ack[ 	]+1,a7
[ 	]+a8:[ 	]+6405a00b[ 	]+tt.rocc.cmdbuf_clear_tiles_to_process_wr_sent[ 	]+0,a1
[ 	]+ac:[ 	]+e408a00b[ 	]+tt.rocc.cmdbuf_clear_tiles_to_process_wr_sent[ 	]+1,a7
[ 	]+b0:[ 	]+6205a00b[ 	]+tt.rocc.cmdbuf_clear_tiles_to_process_idma_tr_ack[ 	]+0,a1
[ 	]+b4:[ 	]+e208a00b[ 	]+tt.rocc.cmdbuf_clear_tiles_to_process_idma_tr_ack[ 	]+1,a7
