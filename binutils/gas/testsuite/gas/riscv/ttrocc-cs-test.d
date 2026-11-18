#as: -march=rv32im_xttroccqsr
#source: ttrocc-cs-test.s
#objdump: -d

.*:[    ]+file format .*


Disassembly of section .text:

0+000 <target>:
[ 	]+0:[ 	]+0600455b[ 	]+tt.rocc.cs_alloc[ 	]+a0
[ 	]+4:[ 	]+060045db[ 	]+tt.rocc.cs_alloc[ 	]+a1
[ 	]+8:[ 	]+0600465b[ 	]+tt.rocc.cs_alloc[ 	]+a2
[ 	]+c:[ 	]+060046db[ 	]+tt.rocc.cs_alloc[ 	]+a3
[ 	]+10:[ 	]+0600475b[ 	]+tt.rocc.cs_alloc[ 	]+a4
[ 	]+14:[ 	]+060047db[ 	]+tt.rocc.cs_alloc[ 	]+a5
[ 	]+18:[ 	]+0600485b[ 	]+tt.rocc.cs_alloc[ 	]+a6
[ 	]+1c:[ 	]+060048db[ 	]+tt.rocc.cs_alloc[ 	]+a7
[ 	]+20:[ 	]+0600405b[ 	]+tt.rocc.cs_alloc[ 	]+zero
[ 	]+24:[ 	]+06004fdb[ 	]+tt.rocc.cs_alloc[ 	]+t6
[ 	]+28:[ 	]+0405205b[ 	]+tt.rocc.cs_dealloc[ 	]+a0
[ 	]+2c:[ 	]+0405a05b[ 	]+tt.rocc.cs_dealloc[ 	]+a1
[ 	]+30:[ 	]+0406205b[ 	]+tt.rocc.cs_dealloc[ 	]+a2
[ 	]+34:[ 	]+0406a05b[ 	]+tt.rocc.cs_dealloc[ 	]+a3
[ 	]+38:[ 	]+0407205b[ 	]+tt.rocc.cs_dealloc[ 	]+a4
[ 	]+3c:[ 	]+0407a05b[ 	]+tt.rocc.cs_dealloc[ 	]+a5
[ 	]+40:[ 	]+0408205b[ 	]+tt.rocc.cs_dealloc[ 	]+a6
[ 	]+44:[ 	]+0408a05b[ 	]+tt.rocc.cs_dealloc[ 	]+a7
[ 	]+48:[ 	]+0400205b[ 	]+tt.rocc.cs_dealloc[ 	]+zero
[ 	]+4c:[ 	]+040fa05b[ 	]+tt.rocc.cs_dealloc[ 	]+t6
[ 	]+50:[ 	]+0205205b[ 	]+tt.rocc.cs_save[ 	]+a0
[ 	]+54:[ 	]+0205a05b[ 	]+tt.rocc.cs_save[ 	]+a1
[ 	]+58:[ 	]+0206205b[ 	]+tt.rocc.cs_save[ 	]+a2
[ 	]+5c:[ 	]+0206a05b[ 	]+tt.rocc.cs_save[ 	]+a3
[ 	]+60:[ 	]+0207205b[ 	]+tt.rocc.cs_save[ 	]+a4
[ 	]+64:[ 	]+0207a05b[ 	]+tt.rocc.cs_save[ 	]+a5
[ 	]+68:[ 	]+0208205b[ 	]+tt.rocc.cs_save[ 	]+a6
[ 	]+6c:[ 	]+0208a05b[ 	]+tt.rocc.cs_save[ 	]+a7
[ 	]+70:[ 	]+0200205b[ 	]+tt.rocc.cs_save[ 	]+zero
[ 	]+74:[ 	]+020fa05b[ 	]+tt.rocc.cs_save[ 	]+t6
[ 	]+78:[ 	]+0005205b[ 	]+tt.rocc.cs_restore[ 	]+a0
[ 	]+7c:[ 	]+0005a05b[ 	]+tt.rocc.cs_restore[ 	]+a1
[ 	]+80:[ 	]+0006205b[ 	]+tt.rocc.cs_restore[ 	]+a2
[ 	]+84:[ 	]+0006a05b[ 	]+tt.rocc.cs_restore[ 	]+a3
[ 	]+88:[ 	]+0007205b[ 	]+tt.rocc.cs_restore[ 	]+a4
[ 	]+8c:[ 	]+0007a05b[ 	]+tt.rocc.cs_restore[ 	]+a5
[ 	]+90:[ 	]+0008205b[ 	]+tt.rocc.cs_restore[ 	]+a6
[ 	]+94:[ 	]+0008a05b[ 	]+tt.rocc.cs_restore[ 	]+a7
[ 	]+98:[ 	]+0000205b[ 	]+tt.rocc.cs_restore[ 	]+zero
[ 	]+9c:[ 	]+000fa05b[ 	]+tt.rocc.cs_restore[ 	]+t6
