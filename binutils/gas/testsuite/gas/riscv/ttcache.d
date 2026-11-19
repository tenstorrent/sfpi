#as:
#source: ttcache.s
#objdump: -d

.*:[    ]+file format .*


Disassembly of section .text:

0+000[ 	]+<.text>:
[	 ]+[0-9a-f]+:[	 ]+fc000073[	 ]+tt.cache.cflush.d.l1[	 ]+zero
[	 ]+[0-9a-f]+:[	 ]+fc0f8073[	 ]+tt.cache.cflush.d.l1[	 ]+t6
[	 ]+[0-9a-f]+:[	 ]+fc200073[	 ]+tt.cache.cdiscard.d.l1[	 ]+zero
[	 ]+[0-9a-f]+:[	 ]+fc2f8073[	 ]+tt.cache.cdiscard.d.l1[	 ]+t6
