	.attribute arch, "rv32i2p0_xtttensixwh"

	sfpload L0,0,0,0
	sfpload L7,0,0,0
	sfpload L0,-0x2000,0,0
	sfpload L0,0x1fff,0,0
	sfpload L0,0x3fff,0,0
	sfpload L0,0,1,0
	sfpload L0,0,2,0
	sfpload L0,0,3,0
	sfpload L0,0,4,0
	sfpload L0,0,5,0
	sfpload L0,0,6,0
	sfpload L0,0,7,0
	sfpload L0,0,8,0
	sfpload L0,0,9,0
	sfpload L0,0,10,0
	sfpload L0,0,12,0
	sfpload L0,0,13,0
	sfpload L0,0,14,0
	sfpload L0,0,15,0
	sfpload L0,0,0,3

	sfploadi L0,-32768,0
	sfploadi L3,-8192,4
	sfploadi L4,8191,8
	sfploadi L7,32767,10

	sfpstore L0,0,0,0
	sfpstore L15,0,0,0
	sfpstore L0,-0x2000,0,0
	sfpstore L0,0x1fff,0,0
	sfpstore L0,0x3fff,0,0
	sfpstore L0,0,1,0
	sfpstore L0,0,2,0
	sfpstore L0,0,3,0
	sfpstore L0,0,4,0
	sfpstore L0,0,5,0
	sfpstore L0,0,6,0
	sfpstore L0,0,7,0
	sfpstore L0,0,8,0
	sfpstore L0,0,9,0
	sfpstore L0,0,10,0
	sfpstore L0,0,12,0
	sfpstore L0,0,13,0
	sfpstore L0,0,14,0
	sfpstore L0,0,15,0
	sfpstore L0,0,0,3

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
	sfpshft L0,L0,-1,1
	sfpshft L0,L0,-0x800,1
	sfpshft L0,L0,+0x7ff,1

	sfpabs L0,L0,0
	sfpabs L7,L15,1

	sfpand L0,L15

	sfpor L7,L0

	sfpnot L4,L8

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

	sfpmov L0,L0,0
	sfpmov L7,L0,0
	sfpmov L0,L15,0
	sfpmov L0,L0,1
	sfpmov L0,L0,2
	sfpmov L0,L0,8

	ttnop

	sfpnop

	ttincrwc 0,0,0,0
	ttincrwc 63,0,0,0
	ttincrwc 0,15,0,0
	ttincrwc 0,0,15,0
	ttincrwc 0,0,0,15
	ttincrwc 63,15,15,15

	ttreplay 0,0,0,0
	ttreplay 1023,0,0,0
	ttreplay 0,1023,0,0
	ttreplay 0,0,7,0
	ttreplay 0,0,0,1
	ttreplay 1023,1023,7,1

	sfpxor L4,L8

	sfpcast L0,L0,0
	sfpcast L7,L15,1

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
	sfpconfig 8,65535,8
	sfpconfig 11,0,12
	sfpconfig 15,65535,14

	sfpswap L0,L15,0
	sfpswap L7,L0,1
	sfpswap L11,L7,1

	sfplutfp32 L0,0
	sfplutfp32 L7,14

	sfptransp

	sfploadmacro 0,L0,0,0,0
	sfploadmacro 3,L0,0,0,0
	sfploadmacro 0,L7,1,0,0
	sfploadmacro 0,L0,1022,0,0
	sfploadmacro 0,L0,0,15,0
	sfploadmacro 0,L0,0,0,3

	sfpstochrnd L0,L0,L0,0,0,0
	sfpstochrnd L7,L15,L15,31,12,1

	ttadddmareg 0,0,0,0
	ttadddmareg 1,0,0,0
	ttadddmareg 0,63,0,0
	ttadddmareg 0,0,63,0
	ttadddmareg 0,0,0,63
	ttadddmareg 1,63,63,63

	ttsubdmareg 0,0,0,0
	ttsubdmareg 1,0,0,0
	ttsubdmareg 0,63,0,0
	ttsubdmareg 0,0,63,0
	ttsubdmareg 0,0,0,63
	ttsubdmareg 1,63,63,63

	ttmuldmareg 0,0,0,0
	ttmuldmareg 1,0,0,0
	ttmuldmareg 0,63,0,0
	ttmuldmareg 0,0,63,0
	ttmuldmareg 0,0,0,63
	ttmuldmareg 1,63,63,63

	ttsetdmareg 0,0,0,0
	ttsetdmareg 3,0,0,0
	ttsetdmareg 0,16383,0,0
	ttsetdmareg 0,0,1,0
	ttsetdmareg 0,0xffff,1,0
	ttsetdmareg 0,0,0,127
	ttsetdmareg 3,16383,1,127

	ttshiftdmareg 0,0,0,0,0
	ttshiftdmareg 1,0,0,0,0
	ttshiftdmareg 0,31,0,0,0
	ttshiftdmareg 0,0,63,0,0
	ttshiftdmareg 0,0,0,63,0
	ttshiftdmareg 0,0,0,0,63
	ttshiftdmareg 1,31,63,63,63

	ttcmpdmareg 0,0,0,0,0
	ttcmpdmareg 1,0,0,0,0
	ttcmpdmareg 0,31,0,0,0
	ttcmpdmareg 0,0,63,0,0
	ttcmpdmareg 0,0,0,63,0
	ttcmpdmareg 0,0,0,0,63
	ttcmpdmareg 1,31,63,63,63

	ttbitwopdmareg 0,0,0,0,0
	ttbitwopdmareg 1,0,0,0,0
	ttbitwopdmareg 0,31,0,0,0
	ttbitwopdmareg 0,0,63,0,0
	ttbitwopdmareg 0,0,0,63,0
	ttbitwopdmareg 0,0,0,0,63
	ttbitwopdmareg 1,31,63,63,63

	ttaddrcrxy 0,0,0,0,0,0
	ttaddrcrxy 7,0,0,0,0,0
	ttaddrcrxy 0,63,0,0,0,0
	ttaddrcrxy 0,0,7,0,0,0
	ttaddrcrxy 0,0,0,7,0,0
	ttaddrcrxy 0,0,0,0,7,0
	ttaddrcrxy 0,0,0,0,0,63
	ttaddrcrxy 7,63,7,7,7,63

	ttaddrcrzw 0,0,0,0,0,0
	ttaddrcrzw 7,0,0,0,0,0
	ttaddrcrzw 0,63,0,0,0,0
	ttaddrcrzw 0,0,7,0,0,0
	ttaddrcrzw 0,0,0,7,0,0
	ttaddrcrzw 0,0,0,0,7,0
	ttaddrcrzw 0,0,0,0,0,63
	ttaddrcrzw 7,63,7,7,7,63

	ttapool3s1 0,0,0,0
	ttapool3s1 3,0,0,0
	ttapool3s1 0,127,0,0
	ttapool3s1 0,0,1,0
	ttapool3s1 0,0,0,16383
	ttapool3s1 3,127,1,16383

	ttapool3s2 0,0,0,0
	ttapool3s2 3,0,0,0
	ttapool3s2 0,127,0,0
	ttapool3s2 0,0,1,0
	ttapool3s2 0,0,0,16383
	ttapool3s2 3,127,1,16383

	ttatcas 0,0,0,0,0,0
	ttatcas 1,0,0,0,0,0
	ttatcas 0,15,0,0,0,0
	ttatcas 0,0,15,0,0,0
	ttatcas 0,0,0,3,0,0
	ttatcas 0,0,0,0,63,0
	ttatcas 0,0,0,0,0,63
	ttatcas 1,15,15,3,63,63

	ttatgetm 0
	ttatgetm 16777215

	ttatincget 0,0,0,0,0
	ttatincget 1,0,0,0,0
	ttatincget 0,15,0,0,0
	ttatincget 0,0,3,0,0
	ttatincget 0,0,0,63,0
	ttatincget 0,0,0,0,63
	ttatincget 1,15,3,63,63

	ttatincgetptr 0,0,0,0,0,0,0
	ttatincgetptr 1,0,0,0,0,0,0
	ttatincgetptr 0,1,0,0,0,0,0
	ttatincgetptr 0,0,15,0,0,0,0
	ttatincgetptr 0,0,0,15,0,0,0
	ttatincgetptr 0,0,0,0,3,0,0
	ttatincgetptr 0,0,0,0,0,63,0
	ttatincgetptr 0,0,0,0,0,0,63
	ttatincgetptr 1,1,15,15,3,63,63

	ttatrelm 0
	ttatrelm 16777215

	ttatswap 0,0,0,0
	ttatswap 1,0,0,0
	ttatswap 0,15,0,0
	ttatswap 0,0,63,0
	ttatswap 0,0,0,63
	ttatswap 1,15,63,63

	ttcleardvalid 0,0
	ttcleardvalid 3,0
	ttcleardvalid 0,4194303
	ttcleardvalid 3,4194303

	ttclrexphist

	ttconv3s1 0,0,0,0
	ttconv3s1 3,0,0,0
	ttconv3s1 0,31,0,0
	ttconv3s1 0,0,3,0
	ttconv3s1 0,0,0,16383
	ttconv3s1 3,31,3,16383

	ttconv3s2 0,0,0,0
	ttconv3s2 3,0,0,0
	ttconv3s2 0,31,0,0
	ttconv3s2 0,0,3,0
	ttconv3s2 0,0,0,16383
	ttconv3s2 3,31,3,16383

	ttdmanop

	ttdotpv 0,0,0,0,0
	ttdotpv 3,0,0,0,0
	ttdotpv 0,1,0,0,0
	ttdotpv 0,0,3,0,0
	ttdotpv 0,0,0,15,0
	ttdotpv 0,0,0,0,16383
	ttdotpv 3,1,3,15,16383

	ttelwadd 0,0,0,0,0
	ttelwadd 3,0,0,0,0
	ttelwadd 0,1,0,0,0
	ttelwadd 0,0,3,0,0
	ttelwadd 0,0,0,15,0
	ttelwadd 0,0,0,0,16383
	ttelwadd 3,1,3,15,16383

	ttelwmul 0,0,0,0,0
	ttelwmul 3,0,0,0,0
	ttelwmul 0,1,0,0,0
	ttelwmul 0,0,3,0,0
	ttelwmul 0,0,0,15,0
	ttelwmul 0,0,0,0,16383
	ttelwmul 3,1,3,15,16383

	ttelwsub 0,0,0,0,0
	ttelwsub 3,0,0,0,0
	ttelwsub 0,1,0,0,0
	ttelwsub 0,0,3,0,0
	ttelwsub 0,0,0,15,0
	ttelwsub 0,0,0,0,16383
	ttelwsub 3,1,3,15,16383

	ttflushdma 0
	ttflushdma 16777215

	ttgapool 0,0,0,0,0
	ttgapool 3,0,0,0,0
	ttgapool 0,7,0,0,0
	ttgapool 0,0,15,0,0
	ttgapool 0,0,0,1,0
	ttgapool 0,0,0,0,16383
	ttgapool 3,7,15,1,16383

	ttgatesrcrst 0,0
	ttgatesrcrst 8388607,0
	ttgatesrcrst 0,1
	ttgatesrcrst 8388607,1

	ttgmpool 0,0,0,0,0
	ttgmpool 3,0,0,0,0
	ttgmpool 0,7,0,0,0
	ttgmpool 0,0,15,0,0
	ttgmpool 0,0,0,1,0
	ttgmpool 0,0,0,0,16383
	ttgmpool 3,7,15,1,16383

	ttincadcxy 0,0,0,0,0
	ttincadcxy 7,0,0,0,0
	ttincadcxy 0,63,0,0,0
	ttincadcxy 0,0,7,0,0
	ttincadcxy 0,0,0,7,0
	ttincadcxy 0,0,0,0,7
	ttincadcxy 7,63,7,7,7

	ttincadczw 0,0,0,0,0
	ttincadczw 7,0,0,0,0
	ttincadczw 0,63,0,0,0
	ttincadczw 0,0,7,0,0
	ttincadczw 0,0,0,7,0
	ttincadczw 0,0,0,0,7
	ttincadczw 7,63,7,7,7

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

	ttmfconv3s1 0,0,0,0
	ttmfconv3s1 3,0,0,0
	ttmfconv3s1 0,31,0,0
	ttmfconv3s1 0,0,3,0
	ttmfconv3s1 0,0,0,16383
	ttmfconv3s1 3,31,3,16383

	ttmop 0,0,0
	ttmop 1,0,0
	ttmop 0,127,0
	ttmop 0,0,32767
	ttmop 1,127,32767

	ttmopcfg 0
	ttmopcfg 16777215

	ttmova2d 0,0,0,0,0
	ttmova2d 1,0,0,0,0
	ttmova2d 0,63,0,0,0
	ttmova2d 0,0,3,0,0
	ttmova2d 0,0,0,3,0
	ttmova2d 0,0,0,0,4095
	ttmova2d 1,63,3,3,4095

	ttmovb2a 0,0,0,0
	ttmovb2a 7,0,0,0
	ttmovb2a 0,3,0,0
	ttmovb2a 0,0,3,0
	ttmovb2a 0,0,0,4095
	ttmovb2a 7,3,3,4095

	ttmovb2d 0,0,0,0,0
	ttmovb2d 1,0,0,0,0
	ttmovb2d 0,63,0,0,0
	ttmovb2d 0,0,3,0,0
	ttmovb2d 0,0,0,7,0
	ttmovb2d 0,0,0,0,2047
	ttmovb2d 1,63,3,7,2047

	ttmovd2a 0,0,0,0,0
	ttmovd2a 1,0,0,0,0
	ttmovd2a 0,63,0,0,0
	ttmovd2a 0,0,3,0,0
	ttmovd2a 0,0,0,3,0
	ttmovd2a 0,0,0,0,1
	ttmovd2a 1,63,3,3,1

	ttmovd2b 0,0,0,0,0
	ttmovd2b 1,0,0,0,0
	ttmovd2b 0,63,0,0,0
	ttmovd2b 0,0,3,0,0
	ttmovd2b 0,0,0,3,0
	ttmovd2b 0,0,0,0,1
	ttmovd2b 1,63,3,3,1

	ttmovdbga2d 0,0,0,0,0
	ttmovdbga2d 1,0,0,0,0
	ttmovdbga2d 0,63,0,0,0
	ttmovdbga2d 0,0,3,0,0
	ttmovdbga2d 0,0,0,7,0
	ttmovdbga2d 0,0,0,0,1
	ttmovdbga2d 1,63,3,7,1

	ttmpool3s1 0,0,0,0
	ttmpool3s1 3,0,0,0
	ttmpool3s1 0,127,0,0
	ttmpool3s1 0,0,1,0
	ttmpool3s1 0,0,0,16383
	ttmpool3s1 3,127,1,16383

	ttmpool3s2 0,0,0,0
	ttmpool3s2 3,0,0,0
	ttmpool3s2 0,127,0,0
	ttmpool3s2 0,0,1,0
	ttmpool3s2 0,0,0,16383
	ttmpool3s2 3,127,1,16383

	ttmvmul 0,0,0,0
	ttmvmul 3,0,0,0
	ttmvmul 0,7,0,0
	ttmvmul 0,0,15,0
	ttmvmul 0,0,0,16383
	ttmvmul 3,7,15,16383

	ttpacr 0,0,0,0,0,0,0
	ttpacr 7,0,0,0,0,0,0
	ttpacr 0,7,0,0,0,0,0
	ttpacr 0,0,1,0,0,0,0
	ttpacr 0,0,0,1,0,0,0
	ttpacr 0,0,0,0,1,0,0
	ttpacr 0,0,0,0,0,1,0
	ttpacr 0,0,0,0,0,0,1
	ttpacr 7,7,1,1,1,1,1

	ttpacrsetreg 0,0,0,0,0,0,0
	ttpacrsetreg 1,0,0,0,0,0,0
	ttpacrsetreg 0,1,0,0,0,0,0
	ttpacrsetreg 0,0,1023,0,0,0,0
	ttpacrsetreg 0,0,0,3,0,0,0
	ttpacrsetreg 0,0,0,0,3,0,0
	ttpacrsetreg 0,0,0,0,0,1,0
	ttpacrsetreg 0,0,0,0,0,0,1
	ttpacrsetreg 1,1,1023,3,3,1,1

	ttrareb

	ttrdcfg 0,0
	ttrdcfg 255,0
	ttrdcfg 0,32767
	ttrdcfg 255,32767

	ttreg2flop 0,0,0,0,0,0
	ttreg2flop 3,0,0,0,0,0
	ttreg2flop 0,3,0,0,0,0
	ttreg2flop 0,0,3,0,0,0
	ttreg2flop 0,0,0,3,0,0
	ttreg2flop 0,0,0,0,767,0
	ttreg2flop 0,0,0,0,0,63
	ttreg2flop 3,3,3,3,767,63

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

	ttrstdma

	ttsemget 0
	ttsemget 3145727

	ttseminit 0,0,0
	ttseminit 15,0,0
	ttseminit 0,15,0
	ttseminit 0,0,12287
	ttseminit 15,15,12287

	ttsempost 0
	ttsempost 3145727

	ttsemwait 0,0,0
	ttsemwait 511,0,0
	ttsemwait 0,8191,0
	ttsemwait 0,0,3
	ttsemwait 511,8191,3

	ttsetadc 0,0,0,0
	ttsetadc 7,0,0,0
	ttsetadc 0,1,0,0
	ttsetadc 0,0,3,0
	ttsetadc 0,0,0,16383
	ttsetadc 7,1,3,16383

	ttsetadcxx 0,0,0
	ttsetadcxx 7,0,0
	ttsetadcxx 0,2047,0
	ttsetadcxx 0,0,1023
	ttsetadcxx 7,2047,1023

	ttsetadcxy 0,0,0,0,0,0
	ttsetadcxy 7,0,0,0,0,0
	ttsetadcxy 0,63,0,0,0,0
	ttsetadcxy 0,0,7,0,0,0
	ttsetadcxy 0,0,0,7,0,0
	ttsetadcxy 0,0,0,0,7,0
	ttsetadcxy 0,0,0,0,0,63
	ttsetadcxy 7,63,7,7,7,63

	ttsetadczw 0,0,0,0,0,0
	ttsetadczw 7,0,0,0,0,0
	ttsetadczw 0,63,0,0,0,0
	ttsetadczw 0,0,7,0,0,0
	ttsetadczw 0,0,0,7,0,0
	ttsetadczw 0,0,0,0,7,0
	ttsetadczw 0,0,0,0,0,63
	ttsetadczw 7,63,7,7,7,63

	ttsetashrmh 0,0
	ttsetashrmh 8388607,0
	ttsetashrmh 0,1
	ttsetashrmh 8388607,1

	ttsetashrmh0 0,0
	ttsetashrmh0 8388607,0
	ttsetashrmh0 0,1
	ttsetashrmh0 8388607,1

	ttsetashrmh1 0,0
	ttsetashrmh1 8388607,0
	ttsetashrmh1 0,1
	ttsetashrmh1 8388607,1

	ttsetashrmv 0
	ttsetashrmv 16777215

	ttsetc16 0,0
	ttsetc16 255,0
	ttsetc16 0,65535
	ttsetc16 255,65535

	ttsetdvalid 0
	ttsetdvalid 65535

	ttsetibrwc 0,0,0
	ttsetibrwc 15,0,0
	ttsetibrwc 0,4095,0
	ttsetibrwc 0,0,63
	ttsetibrwc 15,4095,63

	ttsetpkedgof 0,0,0,0
	ttsetpkedgof 4095,0,0,0
	ttsetpkedgof 0,15,0,0
	ttsetpkedgof 0,0,15,0
	ttsetpkedgof 0,0,0,15
	ttsetpkedgof 4095,15,15,15

	ttsetrwc 0,0,0,0,0,0
	ttsetrwc 3,0,0,0,0,0
	ttsetrwc 0,15,0,0,0,0
	ttsetrwc 0,0,15,0,0,0
	ttsetrwc 0,0,0,15,0,0
	ttsetrwc 0,0,0,0,15,0
	ttsetrwc 0,0,0,0,0,63
	ttsetrwc 3,15,15,15,15,63

	ttshiftxa 0,0
	ttshiftxa 262143,0
	ttshiftxa 0,3
	ttshiftxa 262143,3

	ttshiftxb 0,0,0
	ttshiftxb 511,0,0
	ttshiftxb 0,15,0
	ttshiftxb 0,0,1023
	ttshiftxb 511,15,1023

	ttstallwait 0,0
	ttstallwait 511,0
	ttstallwait 0,255
	ttstallwait 511,255

	ttstoreind 0,0,0,0,0,0,0
	ttstoreind 1,0,0,0,0,0,0
	ttstoreind 0,1,0,0,0,0,0
	ttstoreind 0,0,1,0,0,0,0
	ttstoreind 0,0,0,127,0,0,0
	ttstoreind 0,0,0,0,3,0,0
	ttstoreind 0,0,0,0,0,63,0
	ttstoreind 0,0,0,0,0,0,63
	ttstoreind 1,1,1,127,3,63,63

	ttstorereg 0,0
	ttstorereg 15,0
	ttstorereg 0,16383
	ttstorereg 15,16383

	tttbufcmd

	tttrnspsrca

	tttrnspsrcb

	ttunpacr 0,0,0,0,0,0,0,0,0,0,0,0,0
	ttunpacr 1,0,0,0,0,0,0,0,0,0,0,0,0
	ttunpacr 0,255,0,0,0,0,0,0,0,0,0,0,0
	ttunpacr 0,0,1,0,0,0,0,0,0,0,0,0,0
	ttunpacr 0,0,0,7,0,0,0,0,0,0,0,0,0
	ttunpacr 0,0,0,0,3,0,0,0,0,0,0,0,0
	ttunpacr 0,0,0,0,0,1,0,0,0,0,0,0,0
	ttunpacr 0,0,0,0,0,0,1,0,0,0,0,0,0
	ttunpacr 0,0,0,0,0,0,0,1,0,0,0,0,0
	ttunpacr 0,0,0,0,0,0,0,0,1,0,0,0,0
	ttunpacr 0,0,0,0,0,0,0,0,0,1,0,0,0
	ttunpacr 0,0,0,0,0,0,0,0,0,0,1,0,0
	ttunpacr 0,0,0,0,0,0,0,0,0,0,0,1,0
	ttunpacr 0,0,0,0,0,0,0,0,0,0,0,0,1
 	ttunpacr 1,255,1,7,3,1,1,1,1,1,1,1,1

	ttunpacr_nop 0,0
	ttunpacr_nop 1,0
	ttunpacr_nop 0,1023
	ttunpacr_nop 1,1023

	ttwrcfg 0,0,0
	ttwrcfg 255,0,0
	ttwrcfg 0,1,0
	ttwrcfg 0,0,32767
	ttwrcfg 255,1,32767

	ttxmov 0,0
	ttxmov 1,0
	ttxmov 0,8388607
	ttxmov 1,8388607

	ttzeroacc 0,0,0
	ttzeroacc 31,0,0
	ttzeroacc 0,7,0
	ttzeroacc 0,0,16383
	ttzeroacc 31,7,16383

	ttzerosrc 0,0,0,0
	ttzerosrc 1048575,0,0,0
	ttzerosrc 0,1,0,0
	ttzerosrc 0,0,1,0
	ttzerosrc 0,0,0,3
	ttzerosrc 1048575,1,1,3
