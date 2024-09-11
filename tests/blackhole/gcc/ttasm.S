/*
 * SPDX-FileCopyrightText: © 2023 Tenstorrent Inc.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

	.option nopic
	.attribute arch, "rv32i2p0_l2p0"
	.attribute unaligned_access, 0
	.attribute stack_align, 16
	.text
	.section	.text.startup,"ax",@progbits
	.align	1
	.globl	main
	.type	main, @function
main:
        mv a0, a5
	TTADDDMAREG	1, 2, 3, 4
	TTADDRCRXY	1, 2, 3, 4, 5, 6
	TTADDRCRZW	1, 2, 3, 4, 5, 6
	TTAPOOL3S1	3, 2, 1, 4
	TTAPOOL3S2	3, 2, 1, 4
	TTATCAS	1, 0, 3, 2, 5, 7
	TTATCAS	1, 2, 3, 0, 5, 6
	TTATGETM       1
	TTATINCGET	1, 2, 3, 4, 5
	TTATINCGETPTR	1, 0, 3, 4, 1, 6, 7
	TTATINCGETPTR	0, 1, 3, 4, 0, 6, 7
	TTATRELM       1
	TTATSWAP	1, 2, 3, 4
	TTBITWOPDMAREG	1, 2, 3, 4, 5
	TTCLEARDVALID	1, 2
        TTCLREXPHIST
	TTCMPDMAREG	1, 2, 3, 4, 5
	TTCONV3S1	1, 2, 3, 4
	TTCONV3S2	1, 2, 3, 4
	TTDMANOP
	TTDOTPV	0, 1, 2, 3, 4
	TTELWADD	0, 1, 2, 3, 4
	TTELWMUL	0, 1, 2, 3, 4
	TTELWSUB	0, 1, 2, 3, 4
	TTFLUSHDMA     2
	TTGAPOOL	3, 1, 2, 1, 4
	TTGATESRCRST   12, 1
	TTGMPOOL	0, 2, 3, 1, 4
	TTINCADCXY	0, 1, 2, 3, 4
	TTINCADCZW	0, 1, 2, 3, 4
	TTINCRWC	0, 1, 2, 3
	TTLOADIND	0, 1, 2, 3, 4
	TTLOADREG	0, 4
	TTMFCONV3S1	0, 1, 2, 3
	TTMOP	0, 1, 2
	TTMOPCFG      1
	TTMOVA2D	0, 1, 2, 3, 4
	TTMOVB2A	0, 1, 2, 3
	TTMOVB2D	0, 1, 2, 3, 4
	TTMOVD2A	0, 1, 2, 3, 4
	TTMOVD2B	0, 1, 2, 3, 4
	TTMOVDBGA2D	0, 1, 2, 3, 4
	TTMPOOL3S1	2, 3, 1, 4
	TTMPOOL3S2	2, 3, 1, 4
	TTMULDMAREG	0, 1, 2, 3
	TTMVMUL	0, 1, 2, 3
	TTNOP
        SFPNOP
	TTPACR	6, 2, 1, 2, 3, 1, 0, 1, 2, 3, 1, 1
	TTPACRSETREG	0, 1, 20, 3, 2, 15, 0, 1
	TTPACRSETREG    1, 0, 15, 3, 2, 15, 0, 1
	TTPACRSETREG	0, 1, 2, 3, 0, 0, 1, 1
	TTPACRSETREG	1, 0, 2, 3, 3, 0, 1, 0
	TTRAREB
	TTRDCFG	0, 1
	TTREG2FLOP	0, 1, 2, 3, 4, 5
        TTREPLAY      0, 1, 2, 1
	TTRMWCIB0	1, 2, 3
	TTRMWCIB1	1, 2, 3
	TTRMWCIB2	1, 2, 3
	TTRMWCIB3	1, 2, 3
	TTRSTDMA
	TTSEMGET       2
	TTSEMINIT	1, 2, 3
	TTSEMPOST      2
	TTSEMWAIT	1, 2, 3
	TTSETADC	2, 1, 3, 4
	TTSETADCXX	1, 2, 3
	TTSETADCXY	1, 2, 3, 4, 5, 6
	TTSETADCZW	1, 2, 3, 4, 5, 6
	TTSETASHRMH	2, 1
	TTSETASHRMH0	2, 1
	TTSETASHRMH1	2, 1
	TTSETASHRMV    2
	TTSETC16	1, 2
	TTSETDMAREG	2, 3, 1, 4
	TTSETDVALID	2
	TTSETIBRWC	1, 2, 3
	TTSETPKEDGOF	1, 2, 3, 4
	TTSETRWC	1, 2, 3, 4, 5, 6
	TTSHIFTDMAREG	1, 2, 3, 4, 5
	TTSHIFTXA	1, 2
	TTSHIFTXB	1, 2, 3
	TTSTALLWAIT	1, 2
	TTSTOREIND	1, 0, 0, 2, 3, 4, 5
	TTSTOREIND	0, 1, 0, 2, 3, 4, 5
	TTSTOREIND	0, 0, 1, 2, 3, 4, 5
	TTSTOREREG	1, 2
	TTSUBDMAREG	1, 2, 3, 4
	TTTBUFCMD
	TTTRNSPSRCA
	TTTRNSPSRCB
	TTUNPACR	0, 2, 3, 4, 1, 1, 1, 1, 1, 1, 1, 1, 1
	TTUNPACR	1, 2, 3, 4, 0, 1, 1, 1, 1, 1, 1, 1, 1
	TTUNPACR	1, 2, 3, 4, 1, 0, 1, 1, 1, 1, 1, 1, 1
	TTUNPACR	1, 2, 3, 4, 1, 1, 0, 1, 1, 1, 1, 1, 1
	TTUNPACR	1, 2, 3, 4, 1, 1, 1, 0, 1, 1, 1, 1, 1
	TTUNPACR	1, 2, 3, 4, 1, 1, 1, 1, 0, 1, 1, 1, 1
	TTUNPACR	1, 2, 3, 4, 1, 1, 1, 1, 1, 0, 1, 1, 1
	TTUNPACR	1, 2, 3, 4, 1, 1, 1, 1, 1, 1, 0, 1, 1
	TTUNPACR	1, 2, 3, 4, 1, 1, 1, 1, 1, 1, 1, 0, 1
	TTUNPACR	1, 2, 3, 4, 1, 1, 1, 1, 1, 1, 1, 1, 0
	TTUNPACRNOP	1, 5, 3, 2, 3, 0, 1, 1, 2
	TTUNPACRNOP	1, 5, 3, 2, 2, 1, 0, 1, 2
	TTWRCFG	2, 1, 3
	TTXMOV	1, 2
	TTZEROACC	29, 1, 0, 4, 123
	TTZEROSRC	2, 1, 0, 1
	
        TTCFGSHIFTMASK  1, 2, 3, 4, 2, 5
        TTMOVDBGB2D     1, 13, 4, 7, 1282
        TTMOVDBGB2D     0, 13, 4, 7, 1282
        TTRESOURCEDECL  8, 10, 12
        SFPARECIP       L1, L2, 0, 7
        SFPGT   L2, L1, 12, 3
        SFPLE   L2, L1, 12, 3
        SFPMUL24      L4, L1, L2, L0, 0
        SFPMUL24      L4, L1, L2, L0, 1
        TTSTREAMWAIT    3, 8, 1, 2
        TTSTREAMWRCFG   2, 8, 12
	li	a0,0
	ret
	.size	main, .-main
	.ident	"GCC:   GNU) 10.2.0"
