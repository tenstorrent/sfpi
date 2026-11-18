#as: -march=rv32im_xttroccqsr
#source: ttrocc-misc-test.s
#objdump: -d

.*:[    ]+file format .*


Disassembly of section .text:

0+000 <target>:
[ 	]+0:[ 	]+4205205b[ 	]+tt.rocc.dbg_postcode[ 	]+a0
[ 	]+4:[ 	]+4205a05b[ 	]+tt.rocc.dbg_postcode[ 	]+a1
[ 	]+8:[ 	]+4206205b[ 	]+tt.rocc.dbg_postcode[ 	]+a2
[ 	]+c:[ 	]+4206a05b[ 	]+tt.rocc.dbg_postcode[ 	]+a3
[ 	]+10:[ 	]+4207205b[ 	]+tt.rocc.dbg_postcode[ 	]+a4
[ 	]+14:[ 	]+4207a05b[ 	]+tt.rocc.dbg_postcode[ 	]+a5
[ 	]+18:[ 	]+4208205b[ 	]+tt.rocc.dbg_postcode[ 	]+a6
[ 	]+1c:[ 	]+4208a05b[ 	]+tt.rocc.dbg_postcode[ 	]+a7
[ 	]+20:[ 	]+4200205b[ 	]+tt.rocc.dbg_postcode[ 	]+zero
[ 	]+24:[ 	]+420fa05b[ 	]+tt.rocc.dbg_postcode[ 	]+t6
[ 	]+28:[ 	]+4400005b[ 	]+tt.rocc.noc_fence
[ 	]+2c:[ 	]+46a5305b[ 	]+tt.rocc.llk_intf_write[ 	]+a0,a0
[ 	]+30:[ 	]+46a5b05b[ 	]+tt.rocc.llk_intf_write[ 	]+a1,a0
[ 	]+34:[ 	]+46b5305b[ 	]+tt.rocc.llk_intf_write[ 	]+a0,a1
[ 	]+38:[ 	]+46b5b05b[ 	]+tt.rocc.llk_intf_write[ 	]+a1,a1
[ 	]+3c:[ 	]+46c5b05b[ 	]+tt.rocc.llk_intf_write[ 	]+a1,a2
[ 	]+40:[ 	]+46d5b05b[ 	]+tt.rocc.llk_intf_write[ 	]+a1,a3
[ 	]+44:[ 	]+46e5b05b[ 	]+tt.rocc.llk_intf_write[ 	]+a1,a4
[ 	]+48:[ 	]+46f5b05b[ 	]+tt.rocc.llk_intf_write[ 	]+a1,a5
[ 	]+4c:[ 	]+4705b05b[ 	]+tt.rocc.llk_intf_write[ 	]+a1,a6
[ 	]+50:[ 	]+4715b05b[ 	]+tt.rocc.llk_intf_write[ 	]+a1,a7
[ 	]+54:[ 	]+46b6305b[ 	]+tt.rocc.llk_intf_write[ 	]+a2,a1
[ 	]+58:[ 	]+46b6b05b[ 	]+tt.rocc.llk_intf_write[ 	]+a3,a1
[ 	]+5c:[ 	]+46b7305b[ 	]+tt.rocc.llk_intf_write[ 	]+a4,a1
[ 	]+60:[ 	]+46b7b05b[ 	]+tt.rocc.llk_intf_write[ 	]+a5,a1
[ 	]+64:[ 	]+46b8305b[ 	]+tt.rocc.llk_intf_write[ 	]+a6,a1
[ 	]+68:[ 	]+46b8b05b[ 	]+tt.rocc.llk_intf_write[ 	]+a7,a1
[ 	]+6c:[ 	]+4600305b[ 	]+tt.rocc.llk_intf_write[ 	]+zero,zero
[ 	]+70:[ 	]+47f0305b[ 	]+tt.rocc.llk_intf_write[ 	]+zero,t6
[ 	]+74:[ 	]+460fb05b[ 	]+tt.rocc.llk_intf_write[ 	]+t6,zero
[ 	]+78:[ 	]+47ffb05b[ 	]+tt.rocc.llk_intf_write[ 	]+t6,t6
[ 	]+7c:[ 	]+4605655b[ 	]+tt.rocc.llk_intf_read[ 	]+a0,a0
[ 	]+80:[ 	]+460565db[ 	]+tt.rocc.llk_intf_read[ 	]+a1,a0
[ 	]+84:[ 	]+4605e55b[ 	]+tt.rocc.llk_intf_read[ 	]+a0,a1
[ 	]+88:[ 	]+4605e5db[ 	]+tt.rocc.llk_intf_read[ 	]+a1,a1
[ 	]+8c:[ 	]+460665db[ 	]+tt.rocc.llk_intf_read[ 	]+a1,a2
[ 	]+90:[ 	]+4606e5db[ 	]+tt.rocc.llk_intf_read[ 	]+a1,a3
[ 	]+94:[ 	]+460765db[ 	]+tt.rocc.llk_intf_read[ 	]+a1,a4
[ 	]+98:[ 	]+4607e5db[ 	]+tt.rocc.llk_intf_read[ 	]+a1,a5
[ 	]+9c:[ 	]+460865db[ 	]+tt.rocc.llk_intf_read[ 	]+a1,a6
[ 	]+a0:[ 	]+4608e5db[ 	]+tt.rocc.llk_intf_read[ 	]+a1,a7
[ 	]+a4:[ 	]+4605e65b[ 	]+tt.rocc.llk_intf_read[ 	]+a2,a1
[ 	]+a8:[ 	]+4605e6db[ 	]+tt.rocc.llk_intf_read[ 	]+a3,a1
[ 	]+ac:[ 	]+4605e75b[ 	]+tt.rocc.llk_intf_read[ 	]+a4,a1
[ 	]+b0:[ 	]+4605e7db[ 	]+tt.rocc.llk_intf_read[ 	]+a5,a1
[ 	]+b4:[ 	]+4605e85b[ 	]+tt.rocc.llk_intf_read[ 	]+a6,a1
[ 	]+b8:[ 	]+4605e8db[ 	]+tt.rocc.llk_intf_read[ 	]+a7,a1
[ 	]+bc:[ 	]+4600605b[ 	]+tt.rocc.llk_intf_read[ 	]+zero,zero
[ 	]+c0:[ 	]+460fe05b[ 	]+tt.rocc.llk_intf_read[ 	]+zero,t6
[ 	]+c4:[ 	]+46006fdb[ 	]+tt.rocc.llk_intf_read[ 	]+t6,zero
[ 	]+c8:[ 	]+460fefdb[ 	]+tt.rocc.llk_intf_read[ 	]+t6,t6
[ 	]+cc:[ 	]+48a5305b[ 	]+tt.rocc.fds_intf_write[ 	]+a0,a0
[ 	]+d0:[ 	]+48a5b05b[ 	]+tt.rocc.fds_intf_write[ 	]+a1,a0
[ 	]+d4:[ 	]+48b5305b[ 	]+tt.rocc.fds_intf_write[ 	]+a0,a1
[ 	]+d8:[ 	]+48b5b05b[ 	]+tt.rocc.fds_intf_write[ 	]+a1,a1
[ 	]+dc:[ 	]+48c5b05b[ 	]+tt.rocc.fds_intf_write[ 	]+a1,a2
[ 	]+e0:[ 	]+48d5b05b[ 	]+tt.rocc.fds_intf_write[ 	]+a1,a3
[ 	]+e4:[ 	]+48e5b05b[ 	]+tt.rocc.fds_intf_write[ 	]+a1,a4
[ 	]+e8:[ 	]+48f5b05b[ 	]+tt.rocc.fds_intf_write[ 	]+a1,a5
[ 	]+ec:[ 	]+4905b05b[ 	]+tt.rocc.fds_intf_write[ 	]+a1,a6
[ 	]+f0:[ 	]+4915b05b[ 	]+tt.rocc.fds_intf_write[ 	]+a1,a7
[ 	]+f4:[ 	]+48b6305b[ 	]+tt.rocc.fds_intf_write[ 	]+a2,a1
[ 	]+f8:[ 	]+48b6b05b[ 	]+tt.rocc.fds_intf_write[ 	]+a3,a1
[ 	]+fc:[ 	]+48b7305b[ 	]+tt.rocc.fds_intf_write[ 	]+a4,a1
[ 	]+100:[ 	]+48b7b05b[ 	]+tt.rocc.fds_intf_write[ 	]+a5,a1
[ 	]+104:[ 	]+48b8305b[ 	]+tt.rocc.fds_intf_write[ 	]+a6,a1
[ 	]+108:[ 	]+48b8b05b[ 	]+tt.rocc.fds_intf_write[ 	]+a7,a1
[ 	]+10c:[ 	]+4800305b[ 	]+tt.rocc.fds_intf_write[ 	]+zero,zero
[ 	]+110:[ 	]+49f0305b[ 	]+tt.rocc.fds_intf_write[ 	]+zero,t6
[ 	]+114:[ 	]+480fb05b[ 	]+tt.rocc.fds_intf_write[ 	]+t6,zero
[ 	]+118:[ 	]+49ffb05b[ 	]+tt.rocc.fds_intf_write[ 	]+t6,t6
[ 	]+11c:[ 	]+4805655b[ 	]+tt.rocc.fds_intf_read[ 	]+a0,a0
[ 	]+120:[ 	]+480565db[ 	]+tt.rocc.fds_intf_read[ 	]+a1,a0
[ 	]+124:[ 	]+4805e55b[ 	]+tt.rocc.fds_intf_read[ 	]+a0,a1
[ 	]+128:[ 	]+4805e5db[ 	]+tt.rocc.fds_intf_read[ 	]+a1,a1
[ 	]+12c:[ 	]+480665db[ 	]+tt.rocc.fds_intf_read[ 	]+a1,a2
[ 	]+130:[ 	]+4806e5db[ 	]+tt.rocc.fds_intf_read[ 	]+a1,a3
[ 	]+134:[ 	]+480765db[ 	]+tt.rocc.fds_intf_read[ 	]+a1,a4
[ 	]+138:[ 	]+4807e5db[ 	]+tt.rocc.fds_intf_read[ 	]+a1,a5
[ 	]+13c:[ 	]+480865db[ 	]+tt.rocc.fds_intf_read[ 	]+a1,a6
[ 	]+140:[ 	]+4808e5db[ 	]+tt.rocc.fds_intf_read[ 	]+a1,a7
[ 	]+144:[ 	]+4805e65b[ 	]+tt.rocc.fds_intf_read[ 	]+a2,a1
[ 	]+148:[ 	]+4805e6db[ 	]+tt.rocc.fds_intf_read[ 	]+a3,a1
[ 	]+14c:[ 	]+4805e75b[ 	]+tt.rocc.fds_intf_read[ 	]+a4,a1
[ 	]+150:[ 	]+4805e7db[ 	]+tt.rocc.fds_intf_read[ 	]+a5,a1
[ 	]+154:[ 	]+4805e85b[ 	]+tt.rocc.fds_intf_read[ 	]+a6,a1
[ 	]+158:[ 	]+4805e8db[ 	]+tt.rocc.fds_intf_read[ 	]+a7,a1
[ 	]+15c:[ 	]+4800605b[ 	]+tt.rocc.fds_intf_read[ 	]+zero,zero
[ 	]+160:[ 	]+480fe05b[ 	]+tt.rocc.fds_intf_read[ 	]+zero,t6
[ 	]+164:[ 	]+48006fdb[ 	]+tt.rocc.fds_intf_read[ 	]+t6,zero
[ 	]+168:[ 	]+480fefdb[ 	]+tt.rocc.fds_intf_read[ 	]+t6,t6
[ 	]+16c:[ 	]+4ec5b05b[ 	]+tt.rocc.wr_tiles_to_process_thres_tr_ack[ 	]+a1,a2
[ 	]+170:[ 	]+4f18305b[ 	]+tt.rocc.wr_tiles_to_process_thres_tr_ack[ 	]+a6,a7
[ 	]+174:[ 	]+4cc5b05b[ 	]+tt.rocc.wr_tiles_to_process_thres_wr_sent[ 	]+a1,a2
[ 	]+178:[ 	]+4d18305b[ 	]+tt.rocc.wr_tiles_to_process_thres_wr_sent[ 	]+a6,a7
[ 	]+17c:[ 	]+4ac5b05b[ 	]+tt.rocc.wr_tiles_to_process_thres_idma_tr_ack[ 	]+a1,a2
[ 	]+180:[ 	]+4b18305b[ 	]+tt.rocc.wr_tiles_to_process_thres_idma_tr_ack[ 	]+a6,a7
[ 	]+184:[ 	]+4e05e55b[ 	]+tt.rocc.rd_tiles_to_process_thres_tr_ack[ 	]+a0,a1
[ 	]+188:[ 	]+4e08e85b[ 	]+tt.rocc.rd_tiles_to_process_thres_tr_ack[ 	]+a6,a7
[ 	]+18c:[ 	]+4c05e55b[ 	]+tt.rocc.rd_tiles_to_process_thres_wr_sent[ 	]+a0,a1
[ 	]+190:[ 	]+4c08e85b[ 	]+tt.rocc.rd_tiles_to_process_thres_wr_sent[ 	]+a6,a7
[ 	]+194:[ 	]+4a05e55b[ 	]+tt.rocc.rd_tiles_to_process_thres_idma_tr_ack[ 	]+a0,a1
[ 	]+198:[ 	]+4a08e85b[ 	]+tt.rocc.rd_tiles_to_process_thres_idma_tr_ack[ 	]+a6,a7
