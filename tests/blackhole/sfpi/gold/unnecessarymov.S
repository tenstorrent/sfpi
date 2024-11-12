	.file	"unnecessarymov.cc"
	.option nopic
	.attribute arch, "rv32i2p0_m2p0_l2p0"
	.attribute unaligned_access, 0
	.attribute stack_align, 16
	.text
	.align	2
	.globl	_Z6whymovv
	.type	_Z6whymovv, @function
_Z6whymovv:
	SFPLOADI	L0, 1, 4
	SFPLOAD	L1, 0, 0, 7
	SFPSETCC	L1, 0, 6
	SFPLOADI	L0, 2, 4
	SFPENCC	3, 10
	SFPLOADI	L1, 3, 4
	SFPIADD	L0, L0, -29, 5
	SFPSETCC	L0, 0, 6
	SFPMOV	L0, L1, 2
	SFPLOADI	L0, 4, 4
	SFPENCC	3, 10
	SFPSTORE	0, L0, 12, 7
	ret
	.size	_Z6whymovv, .-_Z6whymovv
	.align	2
	.globl	_Z11whymov_sqrtv
	.type	_Z11whymov_sqrtv, @function
_Z11whymov_sqrtv:
	SFPLOAD	L2, 0, 0, 7
	SFPLOADI	L0, 1, 4
	SFPMOV	L1, L2, 0
	SFPIADD	L1, L0, 0, 6
	SFPMUL	L0, L1, L1, L9, 0
	SFPNOP
	SFPMUL	L0, L0, L1, L9, 0
	SFPNOP
	SFPMUL	L0, L0, L2, L9, 0
	SFPNOP
	SFPSTORE	0, L0, 0, 7
	ret
	.size	_Z11whymov_sqrtv, .-_Z11whymov_sqrtv
	.align	2
	.globl	_Z11test_icmp_vv
	.type	_Z11test_icmp_vv, @function
_Z11test_icmp_vv:
	SFPLOADI	L2, 1, 4
	SFPLOADI	L0, 2, 4
	SFPMOV	L1, L0, 0
	SFPIADD	L1, L2, 0, 6
	SFPSETCC	L1, 0, 6
	SFPIADD	L0, L0, 100, 5
	SFPMOV	L2, L0, 2
	SFPENCC	3, 10
	SFPSTORE	0, L0, 12, 7
	SFPLOADI	L3, 10, 4
	SFPLOADI	L0, 11, 4
	SFPMOV	L1, L0, 0
	SFPIADD	L1, L3, 0, 6
	SFPSETCC	L1, 0, 6
	SFPIADD	L0, L0, 700, 5
	SFPCOMPC
	SFPMOV	L1, L0, 0
	SFPIADD	L1, L3, 0, 10
	SFPIADD	L0, L0, 1200, 5
	SFPENCC	3, 10
	SFPSTORE	0, L2, 12, 7
	ret
	.size	_Z11test_icmp_vv, .-_Z11test_icmp_vv
	.align	2
	.globl	_Z20lots_of_conditionalsv
	.type	_Z20lots_of_conditionalsv, @function
_Z20lots_of_conditionalsv:
	SFPLOADI	L0, 16256, 0
	TTREPLAY	0, 3, 1, 1
	SFPSETCC	L0, 0, 0
	SFPSETCC	L0, 0, 4
	SFPCOMPC
	SFPLOADI	L1, 1, 4
	SFPPUSHC	0
	TTREPLAY	0, 3, 0, 0
	SFPMOV	L0, L1, 2
	SFPLOADI	L0, 0, 4
	SFPPOPC	0
	SFPSETCC	L0, 0, 6
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
	.ident	"GCC: (g96bd106687d) 12.2.0"
