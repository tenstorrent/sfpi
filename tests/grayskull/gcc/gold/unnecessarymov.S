	.file	"unnecessarymov.cc"
	.option nopic
	.attribute arch, "rv32i2p0_y"
	.attribute unaligned_access, 0
	.attribute stack_align, 16
	.text
	.align	2
	.globl	_Z6whymovv
	.type	_Z6whymovv, @function
_Z6whymovv:
	SFPLOADI	L0, 3, 4
	SFPLOAD	L1, 0, 1
	SFPSETCC	L1, 0, 6
	TTNOP
	SFPLOADI	L0, 28, 4
	SFPENCC	3, 10
	TTNOP
	SFPLOADI	L1, 17152, 0
	SFPIADD	L0, L0, -29, 1
	TTNOP
	TTNOP
	TTNOP
	SFPPUSHC
	SFPENCC	3, 2
	TTNOP
	SFPMOV	L0, L1, 0
	TTNOP
	TTNOP
	SFPPOPC
	TTNOP
	SFPLOADI	L0, 17408, 0
	SFPENCC	3, 10
	TTNOP
	SFPSTORE	24, L0, 0
	ret
	.size	_Z6whymovv, .-_Z6whymovv
	.section	.text.startup,"ax",@progbits
	.align	2
	.globl	main
	.type	main, @function
main:
	SFPLOADI	L0, 3, 4
	SFPLOAD	L1, 0, 1
	SFPSETCC	L1, 0, 6
	TTNOP
	SFPLOADI	L0, 28, 4
	SFPENCC	3, 10
	TTNOP
	SFPLOADI	L1, 17152, 0
	SFPIADD	L0, L0, -29, 1
	TTNOP
	TTNOP
	TTNOP
	SFPPUSHC
	SFPENCC	3, 2
	TTNOP
	SFPMOV	L0, L1, 0
	TTNOP
	TTNOP
	SFPPOPC
	TTNOP
	SFPLOADI	L0, 17408, 0
	SFPENCC	3, 10
	TTNOP
	SFPSTORE	24, L0, 0
	li	a0,0
	ret
	.size	main, .-main
	.ident	"GCC: (g96bd106687d) 12.2.0"
