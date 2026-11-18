#as: -march=rv32i_xtttensixwh1
#source: x-tt-nop.s
#objdump: -dr

.*:[ 	]+file format .*

Disassembly of section .text:

0+000 <target>:
[ 	]+[0-9a-f]+:[ 	]+08000000[ 	]+ttnop
