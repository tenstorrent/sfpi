#as: -march=rv32im_xtttensixbh
#source: muladd-bh.s
#objdump: -d

.*:[    ]+file format .*


Disassembly of section .text:

0+000[  ]+<.text>:
[ 	]+[0-9a-f]+:[ 	]+10048c02[ 	]+sfpmad[ 	]+L0,L1,L2,L3,0
[ 	]+[0-9a-f]+:[ 	]+10048c06[ 	]+sfpmad[ 	]+L0,L1,L2,L3,1
[ 	]+[0-9a-f]+:[ 	]+10048c0a[ 	]+sfpmad[ 	]+L0,L1,L2,L3,2
[ 	]+[0-9a-f]+:[ 	]+10048c0e[ 	]+sfpmad[ 	]+L0,L1,L2,L3,3
[ 	]+[0-9a-f]+:[ 	]+14284802[ 	]+sfpadd[ 	]+L0,L1,L2,0
[ 	]+[0-9a-f]+:[ 	]+14284806[ 	]+sfpadd[ 	]+L0,L1,L2,1
[ 	]+[0-9a-f]+:[ 	]+1428480a[ 	]+sfpadd[ 	]+L0,L1,L2,2
[ 	]+[0-9a-f]+:[ 	]+1428480e[ 	]+sfpadd[ 	]+L0,L1,L2,3
[ 	]+[0-9a-f]+:[ 	]+1808e402[ 	]+sfpmul[ 	]+L0,L2,L3,0
[ 	]+[0-9a-f]+:[ 	]+1808e406[ 	]+sfpmul[ 	]+L0,L2,L3,1
[ 	]+[0-9a-f]+:[ 	]+1808e40a[ 	]+sfpmul[ 	]+L0,L2,L3,2
[ 	]+[0-9a-f]+:[ 	]+1808e40e[ 	]+sfpmul[ 	]+L0,L2,L3,3
