	.attribute arch, "rv32i2p0_xtttensixqsr"

	sfpload L0,0,0,0,0,0
	sfpload L7,0,0,0,0,0
	sfpload L0,0x3ff,0,0,0,0
	sfpload L0,0,1,0,0,0
	sfpload L0,0,2,0,0,0
	sfpload L0,0,3,0,0,0
	sfpload L0,0,6,0,0,0
	sfpload L0,0,7,0,0,0
	sfpload L0,0,8,0,0,0
	sfpload L0,0,9,0,0,0
	sfpload L0,0,10,0,0,0
	sfpload L0,0,14,0,0,0
	sfpload L0,0,15,0,0,0
	sfpload L0,0,0,7,0,0
	sfpload L0,0,0,0,1,0
	sfpload L0,0,0,0,0,1

	sfploadi L0,-32768,0
	sfploadi L3,-8192,4
	sfploadi L4,8191,8
	sfploadi L7,32767,10

	sfpstore L0,0,0,0,0,0
	sfpstore L15,0,0,0,0,0
	sfpstore L0,0x03ff,0,0,0,0
	sfpstore L0,0,1,0,0,0
	sfpstore L0,0,2,0,0,0
	sfpstore L0,0,3,0,0,0
	sfpstore L0,0,4,0,0,0
	sfpstore L0,0,5,0,0,0
	sfpstore L0,0,6,0,0,0
	sfpstore L0,0,7,0,0,0
	sfpstore L0,0,8,0,0,0
	sfpstore L0,0,9,0,0,0
	sfpstore L0,0,10,0,0,0
	sfpstore L0,0,14,0,0,0
	sfpstore L0,0,15,0,0,0
	sfpstore L0,0,0,7,0,0
	sfpstore L0,0,0,0,1,0
	sfpstore L0,0,0,0,0,1

	sfplut L0,0
	sfplut L7,4

	sfpmuli L0,0,0
	sfpmuli L7,65535,0

	sfpaddi L0,0,0
	sfpaddi L7,65535,0

	sfpdivp2 L0,L0,0,0
	sfpdivp2 L7,L0,0,0
	sfpdivp2 L0,L15,0,0
	sfpdivp2 L0,L0,0,1
	sfpdivp2 L0,L0,-1,0
	sfpdivp2 L0,L0,-0x800,0
	sfpdivp2 L0,L0,+0x7ff,0

	sfpexexp L0,L0,0
	sfpexexp L3,L7,3
	sfpexexp L4,L8,8
	sfpexexp L7,L15,11

	sfpexman L0,L0,0
	sfpexman L7,L15,1

	sfpiadd L0,L0,0,0
	sfpiadd L7,L0,0,0
	sfpiadd L0,L15,0,0
	sfpiadd L0,L0,0,1
	sfpiadd L0,L0,0,2
	sfpiadd L0,L0,0,4
	sfpiadd L0,L0,0,5
	sfpiadd L0,L0,0,6
	sfpiadd L0,L0,0,8
	sfpiadd L0,L0,0,9
	sfpiadd L0,L0,0,10
	sfpiadd L0,L0,-0x800,1
	sfpiadd L0,L0,0x7ff,1

	sfpshft L0,L0,0,0
	sfpshft L7,L0,0,0
	sfpshft L0,L15,0,0
	sfpshft L0,L0,0,1
	sfpshft L0,L0,0,2
	sfpshft L0,L0,0,3
	sfpshft L0,L0,0,5
	sfpshft L0,L0,0,7
	sfpshft L0,L0,-1,1
	sfpshft L0,L0,-0x800,1
	sfpshft L0,L0,+0x7ff,1

	sfpabs L0,L0,0
	sfpabs L7,L15,1

	sfpand L0,L0
	sfpand L7,L0
	sfpand L0,L15
	sfpand L7,L15

	sfpor L0,L0
	sfpor L7,L0
	sfpor L0,L15
	sfpor L7,L15

	sfpxor L0,L0
	sfpxor L7,L0
	sfpxor L0,L15
	sfpxor L7,L15

	sfpnot L0,L0
	sfpnot L7,L0
	sfpnot L0,L15
	sfpnot L7,L15

	sfplz L0,L0,0
	sfplz L7,L15,14

	sfpsetexp L0,L0,0,0
	sfpsetexp L7,L0,0,0
	sfpsetexp L0,L15,0,0
	sfpsetexp L0,L0,0,1
	sfpsetexp L0,L0,0,2
	sfpsetexp L0,L0,4095,1

	sfpsetman L0,L0,0,0
	sfpsetman L7,L0,0,0
	sfpsetman L0,L15,0,0
	sfpsetman L0,L0,0,1
	sfpsetman L0,L0,4095,1

	sfpsetsgn L0,L0,0,0
	sfpsetsgn L7,L0,0,0
	sfpsetsgn L0,L15,0,0
	sfpsetsgn L0,L0,0,1
	sfpsetsgn L0,L0,4095,1

	sfpmad L0,L0,L15,L0,0
	sfpmad L7,L15,L0,L15,0

	sfpadd L0,L15,L0,0
	sfpadd L7,L0,L15,0

	sfpmul L0,L0,L15,0
	sfpmul L7,L15,L0,0

	sfpencc 0,0
	sfpencc 0,1
	sfpencc 0,2
	sfpencc 4095,2
	sfpencc 0,8
	sfpencc 0,9
	sfpencc 0,10

	sfpcompc

	sfppushc 0
	sfppushc 15

	sfppopc 0
	sfppopc 15

	sfpsetcc L0,0,0
	sfpsetcc L7,0,0
	sfpsetcc L0,0,15
	sfpsetcc L0,4095,1
	sfpsetcc L0,2048,0
	sfpsetcc L0,2048,2
	sfpsetcc L0,2048,4
	sfpsetcc L0,2048,6

	sfpmov L0,L0,0
	sfpmov L7,L0,0
	sfpmov L0,L15,0
	sfpmov L0,L0,1
	sfpmov L0,L0,2
	sfpmov L0,L0,8

	ttnop

	sfpnop
	sfpbankdone 0,0,0
	sfpbankdone 1,0,0
	sfpbankdone 0,1,0
	sfpbankdone 0,0,1
	sfpbankdone 1,1,1

	ttincrwc 0,0,0,0
	ttincrwc 63,0,0,0
	ttincrwc 0,15,0,0
	ttincrwc 0,0,15,0
	ttincrwc 0,0,0,15
	ttincrwc 63,15,15,15

	ttreplay 0,0,0,0,0,0
	ttreplay 1023,0,0,0,0,0
	ttreplay 0,1023,0,0,0,0
	ttreplay 0,0,1,0,0,0
	ttreplay 0,0,0,1,0,0
	ttreplay 0,0,0,0,1,0
	ttreplay 0,0,0,0,0,1
	ttreplay 1023,1023,1,1,1,1

	sfpcast L0,L0,0
	sfpcast L7,L0,0
	sfpcast L0,L15,0
	sfpcast L0,L0,0
	sfpcast L0,L0,1
	sfpcast L0,L0,2
	sfpcast L0,L0,3
	sfpcast L0,L0,4
	sfpcast L0,L0,5

	sfpshft2 L0,L0,0,0
	sfpshft2 L7,L0,0,0
	sfpshft2 L0,L15,0,0
	sfpshft2 L0,L0,0,1
	sfpshft2 L0,L0,0,2
	sfpshft2 L0,L0,0,3
	sfpshft2 L0,L0,0,4
	sfpshft2 L0,L0,0,5
	sfpshft2 L0,L0,0,6

	sfpconfig 0,0,0
	sfpconfig 8,65535,3
	sfpconfig 11,0,1
	sfpconfig 15,65535,1

	sfpswap L0,L15,0,0
	sfpswap L7,L0,0,1
	sfpswap L15,L7,0,1

	sfplutfp32 L0,0
	sfplutfp32 L7,14

	sfptransp

	sfploadmacro 0,L0,0,0,0,0
	sfploadmacro 3,L0,0,0,0,0
	sfploadmacro 0,L7,0,0,0,0
	sfploadmacro 0,L0,1023,0,0,0
	sfploadmacro 0,L0,0,15,0,0
	sfploadmacro 0,L0,0,0,7,0
	sfploadmacro 0,L0,0,0,0,1

	sfpstochrnd L0,L0,L0,0,0,0
	sfpstochrnd L7,L15,L15,31,13,2

	sfparecip L0,L0,0
	sfparecip L0,L0,1
	sfparecip L0,L0,2
	sfparecip L7,L0,0
	sfparecip L0,L15,0

	sfpnonlinear L0,L0,0
	sfpnonlinear L0,L0,1
	sfpnonlinear L0,L0,2
	sfpnonlinear L0,L0,3
	sfpnonlinear L0,L0,4
	sfpnonlinear L0,L0,5
	sfpnonlinear L7,L0,0
	sfpnonlinear L0,L15,0

	sfpgt L0,L0,0,0
	sfpgt L7,L0,0,0
	sfpgt L0,L15,0,0
	sfpgt L0,L0,0,1
	sfpgt L0,L0,0,2
	sfpgt L0,L0,0,4
	sfpgt L0,L0,0,8
	sfpgt L0,L0,0,15

	sfpmul24 L0,L0,L0,0
	sfpmul24 L0,L0,L0,1
	sfpmul24 L0,L0,L0,4
	sfpmul24 L0,L0,L0,5
	sfpmul24 L7,L0,L0,0
	sfpmul24 L0,L15,L0,0
	sfpmul24 L0,L0,L15,0

	sfple L0,L0,0,0
	sfple L7,L0,0,0
	sfple L0,L15,0,0
	sfple L0,L0,0,1
	sfple L0,L0,0,2
	sfple L0,L0,0,4
	sfple L0,L0,0,8
	sfple L0,L0,0,15

	ttatcas 0,0,0,0
	ttatcas 15,0,0,0
	ttatcas 0,15,0,0
	ttatcas 0,0,3,0
	ttatcas 0,0,0,63
	ttatcas 15,15,3,63

	ttatgetm 0
	ttatgetm 65535

	ttatincget 0,0,0,0
	ttatincget 15,0,0,0
	ttatincget 0,3,0,0
	ttatincget 0,0,63,0
	ttatincget 0,0,0,63
	ttatincget 15,3,63,63

	ttatincgetptr 0,0,0,0,0,0
	ttatincgetptr 1,0,0,0,0,0
	ttatincgetptr 0,15,0,0,0,0
	ttatincgetptr 0,0,15,0,0,0
	ttatincgetptr 0,0,0,3,0,0
	ttatincgetptr 0,0,0,0,63,0
	ttatincgetptr 0,0,0,0,0,63
	ttatincgetptr 1,15,15,3,63,63

	ttatrelm 0
	ttatrelm 65535

	ttatswap 0,0,0
	ttatswap 511,0,0
	ttatswap 0,255,0
	ttatswap 0,0,63
	ttatswap 511,255,63

	ttcleardvalid 0,0,0,0,0,0
	ttcleardvalid 3,0,0,0,0,0
	ttcleardvalid 0,3,0,0,0,0
	ttcleardvalid 0,0,15,0,0,0
	ttcleardvalid 0,0,0,15,0,0
	ttcleardvalid 0,0,0,0,15,0
	ttcleardvalid 0,0,0,0,0,3
	ttcleardvalid 3,3,15,15,15,3

	ttdmanop

	ttelwadd 0,0,0,0,0
	ttelwadd 3,0,0,0,0
	ttelwadd 0,1,0,0,0
	ttelwadd 0,0,3,0,0
	ttelwadd 0,0,0,7,0
	ttelwadd 0,0,0,0,0x7ff
	ttelwadd 3,1,3,7,0x7ff

	ttelwadddi 0,0,0,0,0,0
	ttelwadddi 3,0,0,0,0,0
	ttelwadddi 0,7,0,0,0,0
	ttelwadddi 0,0,15,0,0,0
	ttelwadddi 0,0,0,15,0,0
	ttelwadddi 0,0,0,0,7,0
	ttelwadddi 0,0,0,0,0,255
	ttelwadddi 3,7,15,15,7,255

	ttelwmul 0,0,0,0,0
	ttelwmul 3,0,0,0,0
	ttelwmul 0,1,0,0,0
	ttelwmul 0,0,3,0,0
	ttelwmul 0,0,0,7,0
	ttelwmul 0,0,0,0,0x7ff
	ttelwmul 3,1,3,7,0x7ff

	ttelwmuldi 0,0,0,0,0,0
	ttelwmuldi 3,0,0,0,0,0
	ttelwmuldi 0,7,0,0,0,0
	ttelwmuldi 0,0,15,0,0,0
	ttelwmuldi 0,0,0,15,0,0
	ttelwmuldi 0,0,0,0,7,0
	ttelwmuldi 0,0,0,0,0,255
	ttelwmuldi 3,7,15,15,7,255

	ttelwsub 0,0,0,0,0
	ttelwsub 3,0,0,0,0
	ttelwsub 0,1,0,0,0
	ttelwsub 0,0,3,0,0
	ttelwsub 0,0,0,7,0
	ttelwsub 0,0,0,0,0x7ff
	ttelwsub 3,1,3,7,0x7ff

	ttelwsubdi 0,0,0,0,0,0
	ttelwsubdi 3,0,0,0,0,0
	ttelwsubdi 0,7,0,0,0,0
	ttelwsubdi 0,0,15,0,0,0
	ttelwsubdi 0,0,0,15,0,0
	ttelwsubdi 0,0,0,0,7,0
	ttelwsubdi 0,0,0,0,0,255
	ttelwsubdi 3,7,15,15,7,255

	ttgapool 0,0,0,0,0
	ttgapool 3,0,0,0,0
	ttgapool 0,7,0,0,0
	ttgapool 0,0,7,0,0
	ttgapool 0,0,0,1,0
	ttgapool 0,0,0,0,0x7ff
	ttgapool 3,7,7,1,0x7ff

	ttgmpool 0,0,0,0,0
	ttgmpool 3,0,0,0,0
	ttgmpool 0,7,0,0,0
	ttgmpool 0,0,7,0,0
	ttgmpool 0,0,0,1,0
	ttgmpool 0,0,0,0,0x7ff
	ttgmpool 3,7,7,1,0x7ff

	ttloadind 0,0,0,0,0
	ttloadind 3,0,0,0,0
	ttloadind 0,255,0,0,0
	ttloadind 0,0,3,0,0
	ttloadind 0,0,0,63,0
	ttloadind 0,0,0,0,63
	ttloadind 3,255,3,63,63

	ttloadreg 0,0
	ttloadreg 15,0
	ttloadreg 0,16383
	ttloadreg 15,16383

	ttmop 0,0,0,0
	ttmop 1,0,0,0
	ttmop 0,1,0,0
	ttmop 0,0,127,0
	ttmop 0,0,0,32767
	ttmop 1,1,127,32767

	ttmopcfg 0
	ttmopcfg 16777215

	ttmova2d 0,0,0,0,0
	ttmova2d 1,0,0,0,0
	ttmova2d 0,63,0,0,0
	ttmova2d 0,0,7,0,0
	ttmova2d 0,0,0,3,0
	ttmova2d 0,0,0,0,2047
	ttmova2d 1,63,7,3,2047

	ttmovb2a 0,0,0,0
	ttmovb2a 7,0,0,0
	ttmovb2a 0,7,0,0
	ttmovb2a 0,0,3,0
	ttmovb2a 0,0,0,4095
	ttmovb2a 7,7,3,4095

	ttmovb2d 0,0,0,0,0,0
	ttmovb2d 1,0,0,0,0,0
	ttmovb2d 0,63,0,0,0,0
	ttmovb2d 0,0,7,0,0,0
	ttmovb2d 0,0,0,3,0,0
	ttmovb2d 0,0,0,0,1,0
	ttmovb2d 0,0,0,0,0,2047
	ttmovb2d 1,63,7,3,1,2047

	ttmovd2a 0,0,0,0,0
	ttmovd2a 1,0,0,0,0
	ttmovd2a 0,63,0,0,0
	ttmovd2a 0,0,7,0,0
	ttmovd2a 0,0,0,3,0
	ttmovd2a 0,0,0,0,1
	ttmovd2a 1,63,7,3,1

	ttmovd2b 0,0,0,0,0,0
	ttmovd2b 1,0,0,0,0,0
	ttmovd2b 0,63,0,0,0,0
	ttmovd2b 0,0,7,0,0,0
	ttmovd2b 0,0,0,3,0,0
	ttmovd2b 0,0,0,0,1,0
	ttmovd2b 0,0,0,0,0,2047
	ttmovd2b 1,63,7,3,1,2047

	ttmovdbga2d 0,0,0,0,0
	ttmovdbga2d 1,0,0,0,0
	ttmovdbga2d 0,63,0,0,0
	ttmovdbga2d 0,0,7,0,0
	ttmovdbga2d 0,0,0,3,0
	ttmovdbga2d 0,0,0,0,2047
	ttmovdbga2d 1,63,7,3,2047

	ttmvmul 0,0,0,0
	ttmvmul 3,0,0,0
	ttmvmul 0,7,0,0
	ttmvmul 0,0,7,0
	ttmvmul 0,0,0,0x7ff
	ttmvmul 3,7,7,0x7ff

	ttmvmuldi 0,0,0,0,0,0
	ttmvmuldi 3,0,0,0,0,0
	ttmvmuldi 0,7,0,0,0,0
	ttmvmuldi 0,0,15,0,0,0
	ttmvmuldi 0,0,0,15,0,0
	ttmvmuldi 0,0,0,0,7,0
	ttmvmuldi 0,0,0,0,0,255
	ttmvmuldi 3,7,15,15,7,255

	ttrdcfg 0,0
	ttrdcfg 255,0
	ttrdcfg 0,32767
	ttrdcfg 255,32767

	ttrmwcib0 0,0,0
	ttrmwcib0 255,0,0
	ttrmwcib0 0,255,0
	ttrmwcib0 0,0,255
	ttrmwcib0 255,255,255

	ttrmwcib1 0,0,0
	ttrmwcib1 255,0,0
	ttrmwcib1 0,255,0
	ttrmwcib1 0,0,255
	ttrmwcib1 255,255,255

	ttrmwcib2 0,0,0
	ttrmwcib2 255,0,0
	ttrmwcib2 0,255,0
	ttrmwcib2 0,0,255
	ttrmwcib2 255,255,255

	ttrmwcib3 0,0,0
	ttrmwcib3 255,0,0
	ttrmwcib3 0,255,0
	ttrmwcib3 0,0,255
	ttrmwcib3 255,255,255

	ttsemget 0,0
	ttsemget 31,0
	ttsemget 0,127
	ttsemget 31,127

	ttseminit 0,0,0,0
	ttseminit 15,0,0,0
	ttseminit 0,15,0,0
	ttseminit 0,0,31,0
	ttseminit 0,0,0,127
	ttseminit 15,15,31,127

	ttsempost 0,0
	ttsempost 31,0
	ttsempost 0,127
	ttsempost 31,127

	ttsemwait 0,0,0,0
	ttsemwait 511,0,0,0
	ttsemwait 0,3,0,0
	ttsemwait 0,0,31,0
	ttsemwait 0,0,0,127
	ttsemwait 511,3,31,127

	ttsetdvalid 0
	ttsetdvalid 31

	ttsetrwc 0,0,0,0
	ttsetrwc 3,0,0,0
	ttsetrwc 0,15,0,0
	ttsetrwc 0,0,0xfff,0
	ttsetrwc 0,0,0,63
	ttsetrwc 3,15,0xfff,63

	ttshiftxb 0,0,0
	ttshiftxb 1023,0,0
	ttshiftxb 0,15,0
	ttshiftxb 0,0,1023
	ttshiftxb 1023,15,1023

	ttstallwait 0,0,0,0
	ttstallwait 511,0,0,0
	ttstallwait 0,31,0,0
	ttstallwait 0,0,31,0
	ttstallwait 0,0,0,31
	ttstallwait 511,31,31,31

	ttstoreind 0,0,0,0,0,0
	ttstoreind 3,0,0,0,0,0
	ttstoreind 0,1,0,0,0,0
	ttstoreind 0,0,127,0,0,0
	ttstoreind 0,0,0,3,0,0
	ttstoreind 0,0,0,0,63,0
	ttstoreind 0,0,0,0,0,63
	ttstoreind 3,1,127,3,63,63

	ttstorereg 0,0
	ttstorereg 15,0
	ttstorereg 0,16383
	ttstorereg 15,16383

	ttunpacr_nop 0,0,0,0,0,0
	ttunpacr_nop 3,0,0,0,0,0
	ttunpacr_nop 0,1,0,0,0,0
	ttunpacr_nop 0,0,1,0,0,0
	ttunpacr_nop 0,0,0,1,0,0
	ttunpacr_nop 0,0,0,0,3,0
	ttunpacr_nop 0,0,0,0,0,3
	ttunpacr_nop 3,1,1,1,3,3

	ttwrcfg 0,0,0
	ttwrcfg 255,0,0
	ttwrcfg 0,1,0
	ttwrcfg 0,0,32767
	ttwrcfg 255,1,32767

	ttzeroacc 0,0,0,0,0
	ttzeroacc 31,0,0,0,0
	ttzeroacc 0,1,0,0,0
	ttzeroacc 0,0,1,0,0
	ttzeroacc 0,0,0,7,0
	ttzeroacc 0,0,0,0,16383
	ttzeroacc 31,1,1,7,16383

	ttzerosrc 0,0,0,0,0,0,0
	ttzerosrc 1,0,0,0,0,0,0
	ttzerosrc 0,1,0,0,0,0,0
	ttzerosrc 0,0,1,0,0,0,0
	ttzerosrc 0,0,0,1,0,0,0
	ttzerosrc 0,0,0,0,1,0,0
	ttzerosrc 0,0,0,0,0,1,0
	ttzerosrc 0,0,0,0,0,0,3
	ttzerosrc 1,1,1,1,1,1,3

	ttcfgshiftmask 0,0,0,0,0,0
	ttcfgshiftmask 255,0,0,0,0,0
	ttcfgshiftmask 0,1,0,0,0,0
	ttcfgshiftmask 0,0,7,0,0,0
	ttcfgshiftmask 0,0,0,31,0,0
	ttcfgshiftmask 0,0,0,0,31,0
	ttcfgshiftmask 0,0,0,0,0,3
	ttcfgshiftmask 255,1,7,31,31,3

	ttmovdbgb2d 0,0,0,0,0,0
	ttmovdbgb2d 1,0,0,0,0,0
	ttmovdbgb2d 0,63,0,0,0,0
	ttmovdbgb2d 0,0,7,0,0,0
	ttmovdbgb2d 0,0,0,3,0,0
	ttmovdbgb2d 0,0,0,0,1,0
	ttmovdbgb2d 0,0,0,0,0,2047
	ttmovdbgb2d 1,63,7,3,1,2047

	ttresourcedecl 0,0,0
	ttresourcedecl 15,0,0
	ttresourcedecl 0,1023,0
	ttresourcedecl 0,0,31
	ttresourcedecl 15,1023,31

# new insns

	ttaddgpr 0,0,0,0
	ttaddgpr 1,0,0,0
	ttaddgpr 0,63,0,0
	ttaddgpr 0,0,63,0
	ttaddgpr 0,0,0,63
	ttaddgpr 1,63,63,63

	ttbitwopgpr 0,0,0,0,0
	ttbitwopgpr 1,0,0,0,0
	ttbitwopgpr 0,31,0,0,0
	ttbitwopgpr 0,0,63,0,0
	ttbitwopgpr 0,0,0,63,0
	ttbitwopgpr 0,0,0,0,63
	ttbitwopgpr 1,31,63,63,63

	ttcmpgpr 0,0,0,0,0
	ttcmpgpr 1,0,0,0,0
	ttcmpgpr 0,31,0,0,0
	ttcmpgpr 0,0,63,0,0
	ttcmpgpr 0,0,0,63,0
	ttcmpgpr 0,0,0,0,63
	ttcmpgpr 1,31,63,63,63

	ttmulgpr 0,0,0,0
	ttmulgpr 1,0,0,0
	ttmulgpr 0,63,0,0
	ttmulgpr 0,0,63,0
	ttmulgpr 0,0,0,63
	ttmulgpr 1,63,63,63

	ttsetgpr 0,0,0,0
	ttsetgpr 3,16383,0,0
	ttsetgpr 0,0,1,0
	ttsetgpr 0,0,0,63
	ttsetgpr 3,16383,1,63

	ttshiftgpr 0,0,0,0,0
	ttshiftgpr 1,0,0,0,0
	ttshiftgpr 0,31,0,0,0
	ttshiftgpr 0,0,63,0,0
	ttshiftgpr 0,0,0,63,0
	ttshiftgpr 0,0,0,0,63
	ttshiftgpr 1,31,63,63,63

	ttsubgpr 0,0,0,0
	ttsubgpr 1,0,0,0
	ttsubgpr 0,63,0,0
	ttsubgpr 0,0,63,0
	ttsubgpr 0,0,0,63
	ttsubgpr 1,63,63,63

	ttcommit_shadow 0
	ttcommit_shadow (1<<20)-1

	tthalt

	ttinc_dst_tile_Face_row_idx 0,0,0
	ttinc_dst_tile_Face_row_idx 3,0,0
	ttinc_dst_tile_Face_row_idx 0,7,0
	ttinc_dst_tile_Face_row_idx 0,0,(1<<18)-1
	ttinc_dst_tile_Face_row_idx 3,7,(1<<18)-1

	ttinc_src_tile_Face_row_idx 0,0,0
	ttinc_src_tile_Face_row_idx 3,0,0
	ttinc_src_tile_Face_row_idx 0,7,0
	ttinc_src_tile_Face_row_idx 0,0,(1<<18)-1
	ttinc_src_tile_Face_row_idx 3,7,(1<<18)-1

	ttset_dst_tile_Face_row_idx 0,0,0
	ttset_dst_tile_Face_row_idx 3,0,0
	ttset_dst_tile_Face_row_idx 0,7,0
	ttset_dst_tile_Face_row_idx 0,0,(1<<18)-1
	ttset_dst_tile_Face_row_idx 3,7,(1<<18)-1

	ttset_src_tile_Face_row_idx 0,0,0
	ttset_src_tile_Face_row_idx 3,0,0
	ttset_src_tile_Face_row_idx 0,7,0
	ttset_src_tile_Face_row_idx 0,0,(1<<18)-1
	ttset_src_tile_Face_row_idx 3,7,(1<<18)-1

	ttpacr_stride 0,0,0,0,0,0,0,0
	ttpacr_stride 63,0,0,0,0,0,0,0
	ttpacr_stride 0,1,0,0,0,0,0,0
	ttpacr_stride 0,0,7,0,0,0,0,0
	ttpacr_stride 0,0,0,1,0,0,0,0
	ttpacr_stride 0,0,0,0,63,0,0,0
	ttpacr_stride 0,0,0,0,0,31,0,0
	ttpacr_stride 0,0,0,0,0,0,1,0
	ttpacr_stride 0,0,0,0,0,0,0,1
	ttpacr_stride 63,1,7,1,63,31,1,1

	ttpacr_untilize 0,0,0,0,0,0
	ttpacr_untilize 3,0,0,0,0,0
	ttpacr_untilize 0,3,0,0,0,0
	ttpacr_untilize 0,0,3,0,0,0
	ttpacr_untilize 0,0,0,1,0,0
	ttpacr_untilize 0,0,0,0,31,0
	ttpacr_untilize 0,0,0,0,0,1
	ttpacr_untilize 3,3,3,1,31,1

	ttpacr0_face 0,0,0,0,0,0
	ttpacr0_face 3,0,0,0,0,0
	ttpacr0_face 0,3,0,0,0,0
	ttpacr0_face 0,0,7,0,0,0
	ttpacr0_face 0,0,0,3,0,0
	ttpacr0_face 0,0,0,0,31,0
	ttpacr0_face 0,0,0,0,0,1
	ttpacr0_face 3,3,7,3,31,1

	ttpacr0_face_inc 0,0,0,0,0,0
	ttpacr0_face_inc 3,0,0,0,0,0
	ttpacr0_face_inc 0,3,0,0,0,0
	ttpacr0_face_inc 0,0,7,0,0,0
	ttpacr0_face_inc 0,0,0,3,0,0
	ttpacr0_face_inc 0,0,0,0,31,0
	ttpacr0_face_inc 0,0,0,0,0,1
	ttpacr0_face_inc 3,3,7,3,31,1

	ttpacr0_row 0,0,0,0,0,0,0,0
	ttpacr0_row 15,0,0,0,0,0,0,0
	ttpacr0_row 0,15,0,0,0,0,0,0
	ttpacr0_row 0,0,3,0,0,0,0,0
	ttpacr0_row 0,0,0,3,0,0,0,0
	ttpacr0_row 0,0,0,0,7,0,0,0
	ttpacr0_row 0,0,0,0,0,3,0,0
	ttpacr0_row 0,0,0,0,0,0,31,0
	ttpacr0_row 0,0,0,0,0,0,0,1
	ttpacr0_row 15,15,3,3,7,3,31,1

	ttpacr0_row_inc 0,0,0,0,0,0,0,0
	ttpacr0_row_inc 15,0,0,0,0,0,0,0
	ttpacr0_row_inc 0,15,0,0,0,0,0,0
	ttpacr0_row_inc 0,0,3,0,0,0,0,0
	ttpacr0_row_inc 0,0,0,3,0,0,0,0
	ttpacr0_row_inc 0,0,0,0,7,0,0,0
	ttpacr0_row_inc 0,0,0,0,0,3,0,0
	ttpacr0_row_inc 0,0,0,0,0,0,31,0
	ttpacr0_row_inc 0,0,0,0,0,0,0,1
	ttpacr0_row_inc 15,15,3,3,7,3,31,1

	ttpacr0_tile 0,0,0,0
	ttpacr0_tile 255,0,0,0
	ttpacr0_tile 0,511,0,0
	ttpacr0_tile 0,0,31,0
	ttpacr0_tile 0,0,0,1
	ttpacr0_tile 255,511,31,1

	ttpacr0_tile_inc 0,0,0,0
	ttpacr0_tile_inc 255,0,0,0
	ttpacr0_tile_inc 0,511,0,0
	ttpacr0_tile_inc 0,0,31,0
	ttpacr0_tile_inc 0,0,0,1
	ttpacr0_tile_inc 255,511,31,1

	ttpacr1_face 0,0,0,0,0,0
	ttpacr1_face 3,0,0,0,0,0
	ttpacr1_face 0,3,0,0,0,0
	ttpacr1_face 0,0,7,0,0,0
	ttpacr1_face 0,0,0,3,0,0
	ttpacr1_face 0,0,0,0,31,0
	ttpacr1_face 0,0,0,0,0,1
	ttpacr1_face 3,3,7,3,31,1

	ttpacr1_face_inc 0,0,0,0,0,0
	ttpacr1_face_inc 3,0,0,0,0,0
	ttpacr1_face_inc 0,3,0,0,0,0
	ttpacr1_face_inc 0,0,7,0,0,0
	ttpacr1_face_inc 0,0,0,3,0,0
	ttpacr1_face_inc 0,0,0,0,31,0
	ttpacr1_face_inc 0,0,0,0,0,1
	ttpacr1_face_inc 3,3,7,3,31,1

	ttpacr1_row 0,0,0,0,0,0,0,0
	ttpacr1_row 15,0,0,0,0,0,0,0
	ttpacr1_row 0,15,0,0,0,0,0,0
	ttpacr1_row 0,0,3,0,0,0,0,0
	ttpacr1_row 0,0,0,3,0,0,0,0
	ttpacr1_row 0,0,0,0,7,0,0,0
	ttpacr1_row 0,0,0,0,0,3,0,0
	ttpacr1_row 0,0,0,0,0,0,31,0
	ttpacr1_row 0,0,0,0,0,0,0,1
	ttpacr1_row 15,15,3,3,7,3,31,1

	ttpacr1_row_inc 0,0,0,0,0,0,0,0
	ttpacr1_row_inc 15,0,0,0,0,0,0,0
	ttpacr1_row_inc 0,15,0,0,0,0,0,0
	ttpacr1_row_inc 0,0,3,0,0,0,0,0
	ttpacr1_row_inc 0,0,0,3,0,0,0,0
	ttpacr1_row_inc 0,0,0,0,7,0,0,0
	ttpacr1_row_inc 0,0,0,0,0,3,0,0
	ttpacr1_row_inc 0,0,0,0,0,0,31,0
	ttpacr1_row_inc 0,0,0,0,0,0,0,1
	ttpacr1_row_inc 15,15,3,3,7,3,31,1

	ttpacr1_tile 0,0,0,0
	ttpacr1_tile 255,0,0,0
	ttpacr1_tile 0,511,0,0
	ttpacr1_tile 0,0,31,0
	ttpacr1_tile 0,0,0,1
	ttpacr1_tile 255,511,31,1

	ttpacr1_tile_inc 0,0,0,0
	ttpacr1_tile_inc 255,0,0,0
	ttpacr1_tile_inc 0,511,0,0
	ttpacr1_tile_inc 0,0,31,0
	ttpacr1_tile_inc 0,0,0,1
	ttpacr1_tile_inc 255,511,31,1

	ttpop_tiles 0,0,0
	ttpop_tiles 7,0,0
	ttpop_tiles 0,1023,0
	ttpop_tiles 0,0,31
	ttpop_tiles 7,1023,31

	ttpush_tiles 0,0,0
	ttpush_tiles 3,0,0
	ttpush_tiles 0,1023,0
	ttpush_tiles 0,0,31
	ttpush_tiles 3,1023,31

	ttrv_pacr x0,x0,x0
	ttrv_pacr x31,x0,x0
	ttrv_pacr x0,x31,x0
	ttrv_pacr x0,x0,x31
	ttrv_pacr x31,x31,x31

	ttrv_unpacr x0,x0,x0
	ttrv_unpacr x31,x0,x0
	ttrv_unpacr x0,x31,x0
	ttrv_unpacr x0,x0,x31
	ttrv_unpacr x31,x31,x31

	ttrv_wrcfg x0,x0,x0,0,0
	ttrv_wrcfg x31,x0,x0,0,0
	ttrv_wrcfg x0,x31,x0,0,0
	ttrv_wrcfg x0,x0,x31,0,0
	ttrv_wrcfg x0,x0,x0,255,0
	ttrv_wrcfg x0,x0,x0,0,1
	ttrv_wrcfg x31,x31,x31,255,1

	ttunpacr_dest_face 0,0,0,0,0,0
	ttunpacr_dest_face 3,0,0,0,0,0
	ttunpacr_dest_face 0,3,0,0,0,0
	ttunpacr_dest_face 0,0,3,0,0,0
	ttunpacr_dest_face 0,0,0,7,0,0
	ttunpacr_dest_face 0,0,0,0,31,0
	ttunpacr_dest_face 0,0,0,0,0,1
	ttunpacr_dest_face 3,3,3,7,31,1

	ttunpacr_dest_face_inc 0,0,0,0,0,0
	ttunpacr_dest_face_inc 3,0,0,0,0,0
	ttunpacr_dest_face_inc 0,3,0,0,0,0
	ttunpacr_dest_face_inc 0,0,3,0,0,0
	ttunpacr_dest_face_inc 0,0,0,7,0,0
	ttunpacr_dest_face_inc 0,0,0,0,31,0
	ttunpacr_dest_face_inc 0,0,0,0,0,1
	ttunpacr_dest_face_inc 3,3,3,7,31,1

	ttunpacr_dest_row 0,0,0,0,0,0,0,0
	ttunpacr_dest_row 15,0,0,0,0,0,0,0
	ttunpacr_dest_row 0,15,0,0,0,0,0,0
	ttunpacr_dest_row 0,0,3,0,0,0,0,0
	ttunpacr_dest_row 0,0,0,3,0,0,0,0
	ttunpacr_dest_row 0,0,0,0,3,0,0,0
	ttunpacr_dest_row 0,0,0,0,0,7,0,0
	ttunpacr_dest_row 0,0,0,0,0,0,31,0
	ttunpacr_dest_row 0,0,0,0,0,0,0,1
	ttunpacr_dest_row 15,15,3,3,3,7,31,1

	ttunpacr_dest_row_inc 0,0,0,0,0,0,0,0
	ttunpacr_dest_row_inc 15,0,0,0,0,0,0,0
	ttunpacr_dest_row_inc 0,15,0,0,0,0,0,0
	ttunpacr_dest_row_inc 0,0,3,0,0,0,0,0
	ttunpacr_dest_row_inc 0,0,0,3,0,0,0,0
	ttunpacr_dest_row_inc 0,0,0,0,3,0,0,0
	ttunpacr_dest_row_inc 0,0,0,0,0,7,0,0
	ttunpacr_dest_row_inc 0,0,0,0,0,0,31,0
	ttunpacr_dest_row_inc 0,0,0,0,0,0,0,1
	ttunpacr_dest_row_inc 15,15,3,3,3,7,31,1

	ttunpacr_dest_stride 0,0,0,0,0,0,0
	ttunpacr_dest_stride 15,0,0,0,0,0,0
	ttunpacr_dest_stride 0,7,0,0,0,0,0
	ttunpacr_dest_stride 0,0,1,0,0,0,0
	ttunpacr_dest_stride 0,0,0,7,0,0,0
	ttunpacr_dest_stride 0,0,0,0,63,0,0
	ttunpacr_dest_stride 0,0,0,0,0,31,0
	ttunpacr_dest_stride 0,0,0,0,0,0,1
	ttunpacr_dest_stride 15,7,1,7,63,31,1

	ttunpacr_dest_tile 0,0,0,0
	ttunpacr_dest_tile 511,0,0,0
	ttunpacr_dest_tile 0,255,0,0
	ttunpacr_dest_tile 0,0,31,0
	ttunpacr_dest_tile 0,0,0,1
	ttunpacr_dest_tile 511,255,31,1

	ttunpacr_dest_tile_inc 0,0,0,0
	ttunpacr_dest_tile_inc 511,0,0,0
	ttunpacr_dest_tile_inc 0,255,0,0
	ttunpacr_dest_tile_inc 0,0,31,0
	ttunpacr_dest_tile_inc 0,0,0,1
	ttunpacr_dest_tile_inc 511,255,31,1

	ttunpacr_tile_misc 0,0,0,0,0,0,0
	ttunpacr_tile_misc 7,0,0,0,0,0,0
	ttunpacr_tile_misc 0,63,0,0,0,0,0
	ttunpacr_tile_misc 0,0,1,0,0,0,0
	ttunpacr_tile_misc 0,0,0,3,0,0,0
	ttunpacr_tile_misc 0,0,0,0,31,0,0
	ttunpacr_tile_misc 0,0,0,0,0,31,0
	ttunpacr_tile_misc 0,0,0,0,0,0,1
	ttunpacr_tile_misc 7,63,1,3,31,31,1

	ttunpacr_tileize 0,0,0,0,0,0
	ttunpacr_tileize 3,0,0,0,0,0
	ttunpacr_tileize 0,3,0,0,0,0
	ttunpacr_tileize 0,0,3,0,0,0
	ttunpacr_tileize 0,0,0,3,0,0
	ttunpacr_tileize 0,0,0,0,31,0
	ttunpacr_tileize 0,0,0,0,0,1
	ttunpacr_tileize 3,3,3,3,31,1

	ttunpacr0_face 0,0,0,0,0,0
	ttunpacr0_face 3,0,0,0,0,0
	ttunpacr0_face 0,3,0,0,0,0
	ttunpacr0_face 0,0,3,0,0,0
	ttunpacr0_face 0,0,0,7,0,0
	ttunpacr0_face 0,0,0,0,31,0
	ttunpacr0_face 0,0,0,0,0,1
	ttunpacr0_face 3,3,3,7,31,1

	ttunpacr0_face_inc 0,0,0,0,0,0
	ttunpacr0_face_inc 3,0,0,0,0,0
	ttunpacr0_face_inc 0,3,0,0,0,0
	ttunpacr0_face_inc 0,0,3,0,0,0
	ttunpacr0_face_inc 0,0,0,7,0,0
	ttunpacr0_face_inc 0,0,0,0,31,0
	ttunpacr0_face_inc 0,0,0,0,0,1
	ttunpacr0_face_inc 3,3,3,7,31,1

	ttunpacr0_row 0,0,0,0,0,0,0,0
	ttunpacr0_row 15,0,0,0,0,0,0,0
	ttunpacr0_row 0,15,0,0,0,0,0,0
	ttunpacr0_row 0,0,3,0,0,0,0,0
	ttunpacr0_row 0,0,0,3,0,0,0,0
	ttunpacr0_row 0,0,0,0,3,0,0,0
	ttunpacr0_row 0,0,0,0,0,7,0,0
	ttunpacr0_row 0,0,0,0,0,0,31,0
	ttunpacr0_row 0,0,0,0,0,0,0,1
	ttunpacr0_row 15,15,3,3,3,7,31,1

	ttunpacr0_row_inc 0,0,0,0,0,0,0,0
	ttunpacr0_row_inc 15,0,0,0,0,0,0,0
	ttunpacr0_row_inc 0,15,0,0,0,0,0,0
	ttunpacr0_row_inc 0,0,3,0,0,0,0,0
	ttunpacr0_row_inc 0,0,0,3,0,0,0,0
	ttunpacr0_row_inc 0,0,0,0,3,0,0,0
	ttunpacr0_row_inc 0,0,0,0,0,7,0,0
	ttunpacr0_row_inc 0,0,0,0,0,0,31,0
	ttunpacr0_row_inc 0,0,0,0,0,0,0,1
	ttunpacr0_row_inc 15,15,3,3,3,7,31,1

	ttunpacr0_stride 0,0,0,0,0,0,0
	ttunpacr0_stride 15,0,0,0,0,0,0
	ttunpacr0_stride 0,7,0,0,0,0,0
	ttunpacr0_stride 0,0,1,0,0,0,0
	ttunpacr0_stride 0,0,0,7,0,0,0
	ttunpacr0_stride 0,0,0,0,63,0,0
	ttunpacr0_stride 0,0,0,0,0,31,0
	ttunpacr0_stride 0,0,0,0,0,0,1
	ttunpacr0_stride 15,7,1,7,63,31,1

	ttunpacr0_tile 0,0,0,0
	ttunpacr0_tile 511,0,0,0
	ttunpacr0_tile 0,255,0,0
	ttunpacr0_tile 0,0,31,0
	ttunpacr0_tile 0,0,0,1
	ttunpacr0_tile 511,255,31,1

	ttunpacr0_tile_inc 0,0,0,0
	ttunpacr0_tile_inc 511,0,0,0
	ttunpacr0_tile_inc 0,255,0,0
	ttunpacr0_tile_inc 0,0,31,0
	ttunpacr0_tile_inc 0,0,0,1
	ttunpacr0_tile_inc 511,255,31,1

	ttunpacr1_face 0,0,0,0,0,0
	ttunpacr1_face 3,0,0,0,0,0
	ttunpacr1_face 0,3,0,0,0,0
	ttunpacr1_face 0,0,3,0,0,0
	ttunpacr1_face 0,0,0,7,0,0
	ttunpacr1_face 0,0,0,0,31,0
	ttunpacr1_face 0,0,0,0,0,1
	ttunpacr1_face 3,3,3,7,31,1

	ttunpacr1_face_inc 0,0,0,0,0,0
	ttunpacr1_face_inc 3,0,0,0,0,0
	ttunpacr1_face_inc 0,3,0,0,0,0
	ttunpacr1_face_inc 0,0,3,0,0,0
	ttunpacr1_face_inc 0,0,0,7,0,0
	ttunpacr1_face_inc 0,0,0,0,31,0
	ttunpacr1_face_inc 0,0,0,0,0,1
	ttunpacr1_face_inc 3,3,3,7,31,1

	ttunpacr1_row 0,0,0,0,0,0,0,0
	ttunpacr1_row 15,0,0,0,0,0,0,0
	ttunpacr1_row 0,15,0,0,0,0,0,0
	ttunpacr1_row 0,0,3,0,0,0,0,0
	ttunpacr1_row 0,0,0,3,0,0,0,0
	ttunpacr1_row 0,0,0,0,3,0,0,0
	ttunpacr1_row 0,0,0,0,0,7,0,0
	ttunpacr1_row 0,0,0,0,0,0,31,0
	ttunpacr1_row 0,0,0,0,0,0,0,1
	ttunpacr1_row 15,15,3,3,3,7,31,1

	ttunpacr1_row_inc 0,0,0,0,0,0,0,0
	ttunpacr1_row_inc 15,0,0,0,0,0,0,0
	ttunpacr1_row_inc 0,15,0,0,0,0,0,0
	ttunpacr1_row_inc 0,0,3,0,0,0,0,0
	ttunpacr1_row_inc 0,0,0,3,0,0,0,0
	ttunpacr1_row_inc 0,0,0,0,3,0,0,0
	ttunpacr1_row_inc 0,0,0,0,0,7,0,0
	ttunpacr1_row_inc 0,0,0,0,0,0,31,0
	ttunpacr1_row_inc 0,0,0,0,0,0,0,1
	ttunpacr1_row_inc 15,15,3,3,3,7,31,1

	ttunpacr1_stride 0,0,0,0,0,0,0
	ttunpacr1_stride 15,0,0,0,0,0,0
	ttunpacr1_stride 0,7,0,0,0,0,0
	ttunpacr1_stride 0,0,1,0,0,0,0
	ttunpacr1_stride 0,0,0,7,0,0,0
	ttunpacr1_stride 0,0,0,0,63,0,0
	ttunpacr1_stride 0,0,0,0,0,31,0
	ttunpacr1_stride 0,0,0,0,0,0,1
	ttunpacr1_stride 15,7,1,7,63,31,1

	ttunpacr1_tile 0,0,0,0
	ttunpacr1_tile 511,0,0,0
	ttunpacr1_tile 0,255,0,0
	ttunpacr1_tile 0,0,31,0
	ttunpacr1_tile 0,0,0,1
	ttunpacr1_tile 511,255,31,1

	ttunpacr1_tile_inc 0,0,0,0
	ttunpacr1_tile_inc 511,0,0,0
	ttunpacr1_tile_inc 0,255,0,0
	ttunpacr1_tile_inc 0,0,31,0
	ttunpacr1_tile_inc 0,0,0,1
	ttunpacr1_tile_inc 511,255,31,1

	ttunpacr2_face 0,0,0,0,0,0
	ttunpacr2_face 3,0,0,0,0,0
	ttunpacr2_face 0,3,0,0,0,0
	ttunpacr2_face 0,0,3,0,0,0
	ttunpacr2_face 0,0,0,7,0,0
	ttunpacr2_face 0,0,0,0,31,0
	ttunpacr2_face 0,0,0,0,0,1
	ttunpacr2_face 3,3,3,7,31,1

	ttunpacr2_face_inc 0,0,0,0,0,0
	ttunpacr2_face_inc 3,0,0,0,0,0
	ttunpacr2_face_inc 0,3,0,0,0,0
	ttunpacr2_face_inc 0,0,3,0,0,0
	ttunpacr2_face_inc 0,0,0,7,0,0
	ttunpacr2_face_inc 0,0,0,0,31,0
	ttunpacr2_face_inc 0,0,0,0,0,1
	ttunpacr2_face_inc 3,3,3,7,31,1

	ttunpacr2_row 0,0,0,0,0,0,0,0
	ttunpacr2_row 15,0,0,0,0,0,0,0
	ttunpacr2_row 0,15,0,0,0,0,0,0
	ttunpacr2_row 0,0,3,0,0,0,0,0
	ttunpacr2_row 0,0,0,3,0,0,0,0
	ttunpacr2_row 0,0,0,0,3,0,0,0
	ttunpacr2_row 0,0,0,0,0,7,0,0
	ttunpacr2_row 0,0,0,0,0,0,31,0
	ttunpacr2_row 0,0,0,0,0,0,0,1
	ttunpacr2_row 15,15,3,3,3,7,31,1

	ttunpacr2_row_inc 0,0,0,0,0,0,0,0
	ttunpacr2_row_inc 15,0,0,0,0,0,0,0
	ttunpacr2_row_inc 0,15,0,0,0,0,0,0
	ttunpacr2_row_inc 0,0,3,0,0,0,0,0
	ttunpacr2_row_inc 0,0,0,3,0,0,0,0
	ttunpacr2_row_inc 0,0,0,0,3,0,0,0
	ttunpacr2_row_inc 0,0,0,0,0,7,0,0
	ttunpacr2_row_inc 0,0,0,0,0,0,31,0
	ttunpacr2_row_inc 0,0,0,0,0,0,0,1
	ttunpacr2_row_inc 15,15,3,3,3,7,31,1

	ttunpacr2_stride 0,0,0,0,0,0,0
	ttunpacr2_stride 15,0,0,0,0,0,0
	ttunpacr2_stride 0,7,0,0,0,0,0
	ttunpacr2_stride 0,0,1,0,0,0,0
	ttunpacr2_stride 0,0,0,7,0,0,0
	ttunpacr2_stride 0,0,0,0,63,0,0
	ttunpacr2_stride 0,0,0,0,0,31,0
	ttunpacr2_stride 0,0,0,0,0,0,1
	ttunpacr2_stride 15,7,1,7,63,31,1

	ttunpacr2_tile 0,0,0,0
	ttunpacr2_tile 511,0,0,0
	ttunpacr2_tile 0,255,0,0
	ttunpacr2_tile 0,0,31,0
	ttunpacr2_tile 0,0,0,1
	ttunpacr2_tile 511,255,31,1

	ttunpacr2_tile_inc 0,0,0,0
	ttunpacr2_tile_inc 511,0,0,0
	ttunpacr2_tile_inc 0,255,0,0
	ttunpacr2_tile_inc 0,0,31,0
	ttunpacr2_tile_inc 0,0,0,1
	ttunpacr2_tile_inc 511,255,31,1

	ttwait_free 0,0,0
	ttwait_free 511,0,0
	ttwait_free 0,1023,0
	ttwait_free 0,0,31
	ttwait_free 511,1023,31

	ttwait_tiles 0,0,0
	ttwait_tiles 511,0,0
	ttwait_tiles 0,1023,0
	ttwait_tiles 0,0,31
	ttwait_tiles 511,1023,31
