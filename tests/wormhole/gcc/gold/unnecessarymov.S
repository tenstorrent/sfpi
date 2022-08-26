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
	SFPLOADI	L0, 3, 4
	SFPLOAD	L1, 0, 1, 0
	SFPSETCC	L1, 0, 6
	SFPLOADI	L0, 28, 4
	SFPENCC	3, 10
	SFPLOADI	L1, 17152, 0
	SFPIADD	L0, L0, -29, 1
	SFPMOV	L0, L1, 2
	SFPLOADI	L0, 17408, 0
	SFPENCC	3, 10
	SFPSTORE	24, L0, 0, 0
	ret
	.size	_Z6whymovv, .-_Z6whymovv
	.section	.text.startup,"ax",@progbits
	.align	1
	.globl	main
	.type	main, @function
main:
	SFPLOADI	L0, 3, 4
	SFPLOAD	L1, 0, 1, 0
	SFPSETCC	L1, 0, 6
	SFPLOADI	L0, 28, 4
	SFPENCC	3, 10
	SFPLOADI	L1, 17152, 0
	SFPIADD	L0, L0, -29, 1
	SFPMOV	L0, L1, 2
	SFPLOADI	L0, 17408, 0
	SFPENCC	3, 10
	SFPSTORE	24, L0, 0, 0
	li	a0,0
	ret
	.size	main, .-main
	.ident	"GCC: (GNU) 10.2.0"
