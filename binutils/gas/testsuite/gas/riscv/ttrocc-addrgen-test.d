#as: -march=rv32im_xttroccqsr
#source: ttrocc-addrgen-test.s
#objdump: -d

.*:[    ]+file format .*


Disassembly of section .text:

0+000 <target>:
[ 	]+0:[ 	]+4005202b[ 	]+tt.rocc.addrgen_wr_reg[ 	]+zero,1,0,a0,zero
[ 	]+4:[ 	]+7008a02b[ 	]+tt.rocc.addrgen_wr_reg[ 	]+zero,1,24,a7,zero
[ 	]+8:[ 	]+6000202b[ 	]+tt.rocc.addrgen_wr_reg[ 	]+zero,1,16,zero,zero
[ 	]+c:[ 	]+500fa02b[ 	]+tt.rocc.addrgen_wr_reg[ 	]+zero,1,8,t6,zero
[ 	]+10:[ 	]+1e05202b[ 	]+tt.rocc.addrgen_wr_reg[ 	]+zero,0,15,a0,zero
[ 	]+14:[ 	]+0008a02b[ 	]+tt.rocc.addrgen_wr_reg[ 	]+zero,0,0,a7,zero
[ 	]+18:[ 	]+0200202b[ 	]+tt.rocc.addrgen_wr_reg[ 	]+zero,0,1,zero,zero
[ 	]+1c:[ 	]+260fa02b[ 	]+tt.rocc.addrgen_wr_reg[ 	]+zero,0,19,t6,zero
[ 	]+20:[ 	]+4000452b[ 	]+tt.rocc.addrgen_rd_reg[ 	]+a0,1,0,zero,zero
[ 	]+24:[ 	]+700048ab[ 	]+tt.rocc.addrgen_rd_reg[ 	]+a7,1,24,zero,zero
[ 	]+28:[ 	]+6000402b[ 	]+tt.rocc.addrgen_rd_reg[ 	]+zero,1,16,zero,zero
[ 	]+2c:[ 	]+50004fab[ 	]+tt.rocc.addrgen_rd_reg[ 	]+t6,1,8,zero,zero
[ 	]+30:[ 	]+1e00452b[ 	]+tt.rocc.addrgen_rd_reg[ 	]+a0,0,15,zero,zero
[ 	]+34:[ 	]+000048ab[ 	]+tt.rocc.addrgen_rd_reg[ 	]+a7,0,0,zero,zero
[ 	]+38:[ 	]+0200402b[ 	]+tt.rocc.addrgen_rd_reg[ 	]+zero,0,1,zero,zero
[ 	]+3c:[ 	]+26004fab[ 	]+tt.rocc.addrgen_rd_reg[ 	]+t6,0,19,zero,zero
[ 	]+40:[ 	]+3400002b[ 	]+tt.rocc.addrgen_reset[ 	]+0
[ 	]+44:[ 	]+7400002b[ 	]+tt.rocc.addrgen_reset[ 	]+1
[ 	]+48:[ 	]+3405202b[ 	]+tt.rocc.addrgen_reset_counters[ 	]+0,a0
[ 	]+4c:[ 	]+7405202b[ 	]+tt.rocc.addrgen_reset_counters[ 	]+1,a0
[ 	]+50:[ 	]+3408a02b[ 	]+tt.rocc.addrgen_reset_counters[ 	]+0,a7
[ 	]+54:[ 	]+7408a02b[ 	]+tt.rocc.addrgen_reset_counters[ 	]+1,a7
[ 	]+58:[ 	]+3400202b[ 	]+tt.rocc.addrgen_reset_counters[ 	]+0,zero
[ 	]+5c:[ 	]+7400202b[ 	]+tt.rocc.addrgen_reset_counters[ 	]+1,zero
[ 	]+60:[ 	]+340fa02b[ 	]+tt.rocc.addrgen_reset_counters[ 	]+0,t6
[ 	]+64:[ 	]+740fa02b[ 	]+tt.rocc.addrgen_reset_counters[ 	]+1,t6
[ 	]+68:[ 	]+3e00452b[ 	]+tt.rocc.addrgen_peek_src[ 	]+a0,0
[ 	]+6c:[ 	]+3e0048ab[ 	]+tt.rocc.addrgen_peek_src[ 	]+a7,0
[ 	]+70:[ 	]+3e00402b[ 	]+tt.rocc.addrgen_peek_src[ 	]+zero,0
[ 	]+74:[ 	]+3e004fab[ 	]+tt.rocc.addrgen_peek_src[ 	]+t6,0
[ 	]+78:[ 	]+7e00452b[ 	]+tt.rocc.addrgen_peek_src[ 	]+a0,1
[ 	]+7c:[ 	]+7e0048ab[ 	]+tt.rocc.addrgen_peek_src[ 	]+a7,1
[ 	]+80:[ 	]+7e00402b[ 	]+tt.rocc.addrgen_peek_src[ 	]+zero,1
[ 	]+84:[ 	]+7e004fab[ 	]+tt.rocc.addrgen_peek_src[ 	]+t6,1
[ 	]+88:[ 	]+3e00e52b[ 	]+tt.rocc.addrgen_pop_src[ 	]+a0,0
[ 	]+8c:[ 	]+3e00e8ab[ 	]+tt.rocc.addrgen_pop_src[ 	]+a7,0
[ 	]+90:[ 	]+3e00e02b[ 	]+tt.rocc.addrgen_pop_src[ 	]+zero,0
[ 	]+94:[ 	]+3e00efab[ 	]+tt.rocc.addrgen_pop_src[ 	]+t6,0
[ 	]+98:[ 	]+7e00e52b[ 	]+tt.rocc.addrgen_pop_src[ 	]+a0,1
[ 	]+9c:[ 	]+7e00e8ab[ 	]+tt.rocc.addrgen_pop_src[ 	]+a7,1
[ 	]+a0:[ 	]+7e00e02b[ 	]+tt.rocc.addrgen_pop_src[ 	]+zero,1
[ 	]+a4:[ 	]+7e00efab[ 	]+tt.rocc.addrgen_pop_src[ 	]+t6,1
[ 	]+a8:[ 	]+3e05652b[ 	]+tt.rocc.addrgen_pop_x_src[ 	]+a0,0,a0
[ 	]+ac:[ 	]+3e0568ab[ 	]+tt.rocc.addrgen_pop_x_src[ 	]+a7,0,a0
[ 	]+b0:[ 	]+3e08e52b[ 	]+tt.rocc.addrgen_pop_x_src[ 	]+a0,0,a7
[ 	]+b4:[ 	]+3e0fe02b[ 	]+tt.rocc.addrgen_pop_x_src[ 	]+zero,0,t6
[ 	]+b8:[ 	]+3e006fab[ 	]+tt.rocc.addrgen_pop_x_src[ 	]+t6,0,zero
[ 	]+bc:[ 	]+7e05652b[ 	]+tt.rocc.addrgen_pop_x_src[ 	]+a0,1,a0
[ 	]+c0:[ 	]+7e0568ab[ 	]+tt.rocc.addrgen_pop_x_src[ 	]+a7,1,a0
[ 	]+c4:[ 	]+7e08e52b[ 	]+tt.rocc.addrgen_pop_x_src[ 	]+a0,1,a7
[ 	]+c8:[ 	]+7e0fe02b[ 	]+tt.rocc.addrgen_pop_x_src[ 	]+zero,1,t6
[ 	]+cc:[ 	]+7e006fab[ 	]+tt.rocc.addrgen_pop_x_src[ 	]+t6,1,zero
[ 	]+d0:[ 	]+3c00452b[ 	]+tt.rocc.addrgen_peek_dest[ 	]+a0,0
[ 	]+d4:[ 	]+3c0048ab[ 	]+tt.rocc.addrgen_peek_dest[ 	]+a7,0
[ 	]+d8:[ 	]+3c00402b[ 	]+tt.rocc.addrgen_peek_dest[ 	]+zero,0
[ 	]+dc:[ 	]+3c004fab[ 	]+tt.rocc.addrgen_peek_dest[ 	]+t6,0
[ 	]+e0:[ 	]+7c00452b[ 	]+tt.rocc.addrgen_peek_dest[ 	]+a0,1
[ 	]+e4:[ 	]+7c0048ab[ 	]+tt.rocc.addrgen_peek_dest[ 	]+a7,1
[ 	]+e8:[ 	]+7c00402b[ 	]+tt.rocc.addrgen_peek_dest[ 	]+zero,1
[ 	]+ec:[ 	]+7c004fab[ 	]+tt.rocc.addrgen_peek_dest[ 	]+t6,1
[ 	]+f0:[ 	]+3c00e52b[ 	]+tt.rocc.addrgen_pop_dest[ 	]+a0,0
[ 	]+f4:[ 	]+3c00e8ab[ 	]+tt.rocc.addrgen_pop_dest[ 	]+a7,0
[ 	]+f8:[ 	]+3c00e02b[ 	]+tt.rocc.addrgen_pop_dest[ 	]+zero,0
[ 	]+fc:[ 	]+3c00efab[ 	]+tt.rocc.addrgen_pop_dest[ 	]+t6,0
[ 	]+100:[ 	]+7c00e52b[ 	]+tt.rocc.addrgen_pop_dest[ 	]+a0,1
[ 	]+104:[ 	]+7c00e8ab[ 	]+tt.rocc.addrgen_pop_dest[ 	]+a7,1
[ 	]+108:[ 	]+7c00e02b[ 	]+tt.rocc.addrgen_pop_dest[ 	]+zero,1
[ 	]+10c:[ 	]+7c00efab[ 	]+tt.rocc.addrgen_pop_dest[ 	]+t6,1
[ 	]+110:[ 	]+7c0568ab[ 	]+tt.rocc.addrgen_pop_x_dest[ 	]+a7,1,a0
[ 	]+114:[ 	]+3c0fe02b[ 	]+tt.rocc.addrgen_pop_x_dest[ 	]+zero,0,t6
[ 	]+118:[ 	]+32b578ab[ 	]+tt.rocc.addrgen_pop_both[ 	]+a7,0,a0,a1
[ 	]+11c:[ 	]+73107fab[ 	]+tt.rocc.addrgen_pop_both[ 	]+t6,1,zero,a7
[ 	]+120:[ 	]+7800002b[ 	]+tt.rocc.addrgen_push_src[ 	]+1
[ 	]+124:[ 	]+3808a02b[ 	]+tt.rocc.addrgen_push_src_pop_x[ 	]+0,a7
[ 	]+128:[ 	]+7805202b[ 	]+tt.rocc.addrgen_push_src_pop_x[ 	]+1,a0
[ 	]+12c:[ 	]+7600002b[ 	]+tt.rocc.addrgen_push_dest[ 	]+1
[ 	]+130:[ 	]+7608a02b[ 	]+tt.rocc.addrgen_push_dest_pop_x[ 	]+1,a7
[ 	]+134:[ 	]+3605202b[ 	]+tt.rocc.addrgen_push_dest_pop_x[ 	]+0,a0
[ 	]+138:[ 	]+3a00002b[ 	]+tt.rocc.addrgen_push_both[ 	]+0
[ 	]+13c:[ 	]+7ab5302b[ 	]+tt.rocc.addrgen_push_both_pop_x[ 	]+1,a0,a1
[ 	]+140:[ 	]+3b15b02b[ 	]+tt.rocc.addrgen_push_both_pop_x[ 	]+0,a1,a7
