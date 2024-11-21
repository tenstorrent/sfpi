	.file	"sfplut.cc"
	.option nopic
	.attribute arch, "rv32i2p0_xttgs1p0"
	.attribute unaligned_access, 0
	.attribute stack_align, 16
	.text
	.section	.text.startup,"ax",@progbits
	.align	2
	.globl	main
	.type	main, @function
main:
	SFPLOADI	L2, 12, 0
	SFPLOADI	L0, 13, 0
	SFPLOADI	L1, 14, 0
	SFPLOADI	L3, 6, 0
	SFPLUT	L3, 5
	TTNOP
	TTNOP
	SFPLUT	L3, 5
	TTNOP
	TTNOP
	li	a0,0
	ret
	.size	main, .-main
	.ident	"GCC: (g323f2891276) 12.2.0"
