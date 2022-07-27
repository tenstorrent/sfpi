	.file	"unnecessarymov.cc"
	.option nopic
	.attribute arch, "rv32i2p0_m2p0_w2p0"
	.attribute unaligned_access, 0
	.attribute stack_align, 16
	.text
	.align	1
	.globl	_Z6whymovv
	.type	_Z6whymovv, @function
_Z6whymovv:
	SFPLOADI	L0, 4, 3
	SFPLOAD	L1, 1, 0, 0
	SFPSETCC	0, L1, 6
	SFPLOADI	L0, 4, 28
	SFPENCC	3, 10
	SFPLOADI	L1, 0, 17152
	SFPIADD	-29, L0, L0, 1
	SFPMOV	L1, L0, 2
	SFPLOADI	L0, 0, 17408
	SFPENCC	3, 10
	SFPSTORE	L0, 0, 0, 24
	ret
	.size	_Z6whymovv, .-_Z6whymovv
	.section	.text.startup,"ax",@progbits
	.align	1
	.globl	main
	.type	main, @function
main:
	SFPLOADI	L0, 4, 3
	SFPLOAD	L1, 1, 0, 0
	SFPSETCC	0, L1, 6
	SFPLOADI	L0, 4, 28
	SFPENCC	3, 10
	SFPLOADI	L1, 0, 17152
	SFPIADD	-29, L0, L0, 1
	SFPMOV	L1, L0, 2
	SFPLOADI	L0, 0, 17408
	SFPENCC	3, 10
	SFPSTORE	L0, 0, 0, 24
	li	a0,0
	ret
	.size	main, .-main
	.ident	"GCC: (GNU) 10.2.0"
