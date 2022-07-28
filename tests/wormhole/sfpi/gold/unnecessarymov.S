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
	SFPLOADI	L0, 4, 1
	SFPLOAD	L1, 0, 3, 0
	SFPSETCC	0, L1, 6
	SFPLOADI	L0, 4, 2
	SFPENCC	3, 10
	SFPLOADI	L1, 4, 3
	SFPIADD	-29, L0, L0, 5
	SFPSETCC	0, L0, 6
	SFPMOV	L1, L0, 2
	SFPLOADI	L0, 4, 4
	SFPENCC	3, 10
	SFPSTORE	L0, 0, 3, 0
	ret
	.size	_Z6whymovv, .-_Z6whymovv
	.align	1
	.globl	_Z11whymov_sqrtv
	.type	_Z11whymov_sqrtv, @function
_Z11whymov_sqrtv:
	SFPLOAD	L2, 0, 3, 0
	SFPLOADI	L3, 4, 1
	SFPMOV	L2, L0, 0
	SFPIADD	0, L3, L0, 6
	SFPMOV	L0, L1, 2
	SFPMUL	L0, L0, L9, L0, 0
	SFPWNOP
	SFPMUL	L0, L1, L9, L0, 0
	SFPWNOP
	SFPMUL	L0, L2, L9, L0, 0
	SFPWNOP
	SFPSTORE	L0, 0, 3, 0
	ret
	.size	_Z11whymov_sqrtv, .-_Z11whymov_sqrtv
	.align	1
	.globl	_Z11test_icmp_vv
	.type	_Z11test_icmp_vv, @function
_Z11test_icmp_vv:
	SFPLOADI	L2, 4, 1
	SFPLOADI	L0, 4, 2
	SFPMOV	L0, L1, 0
	SFPIADD	0, L2, L1, 6
	SFPSETCC	0, L1, 6
	SFPIADD	100, L0, L0, 5
	SFPMOV	L0, L2, 2
	SFPENCC	3, 10
	SFPSTORE	L0, 0, 3, 0
	SFPLOADI	L3, 4, 10
	SFPLOADI	L0, 4, 11
	SFPMOV	L0, L1, 0
	SFPIADD	0, L3, L1, 6
	SFPSETCC	0, L1, 6
	SFPIADD	700, L0, L0, 5
	SFPCOMPC
	SFPMOV	L0, L1, 0
	SFPIADD	0, L3, L1, 10
	SFPIADD	1200, L0, L0, 5
	SFPENCC	3, 10
	SFPSTORE	L2, 0, 3, 0
	ret
	.size	_Z11test_icmp_vv, .-_Z11test_icmp_vv
	.align	1
	.globl	_Z20lots_of_conditionalsv
	.type	_Z20lots_of_conditionalsv, @function
_Z20lots_of_conditionalsv:
	SFPLOADI	L0, 0, 16256
	SFPSETCC	0, L0, 0
	SFPSETCC	0, L0, 4
	SFPCOMPC
	SFPLOADI	L1, 4, 1
	SFPPUSHC	0
	SFPSETCC	0, L0, 0
	SFPSETCC	0, L0, 4
	SFPCOMPC
	SFPMOV	L1, L0, 2
	SFPLOADI	L0, 4, 0
	SFPPOPC	0
	SFPSETCC	0, L0, 6
	SFPENCC	3, 10
	ret
	.size	_Z20lots_of_conditionalsv, .-_Z20lots_of_conditionalsv
	.globl	_ZN7ckernel13instrn_bufferE
	.section	.sbss,"aw",@nobits
	.align	2
	.type	_ZN7ckernel13instrn_bufferE, @object
	.size	_ZN7ckernel13instrn_bufferE, 4
_ZN7ckernel13instrn_bufferE:
	.zero	4
	.ident	"GCC: (GNU) 10.2.0"
