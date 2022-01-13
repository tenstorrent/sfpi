	.file	"regmov.cc"
	.option nopic
	.attribute arch, "rv32i2p0_y2p0"
	.attribute unaligned_access, 0
	.attribute stack_align, 16
	.text
	.align	1
	.globl	_Z15remove_add1_yesv
	.type	_Z15remove_add1_yesv, @function
_Z15remove_add1_yesv:
	SFPLOADI	L1, 4, 1
	SFPLOADI	L0, 4, 2
	SFPIADD	0, L1, L0, 4
	SFPNOP
	SFPNOP
	SFPSTORE	L1, 2, 0
	SFPSTORE	L0, 2, 8
	ret
	.size	_Z15remove_add1_yesv, .-_Z15remove_add1_yesv
	.align	1
	.globl	_Z15remove_add2_nopv
	.type	_Z15remove_add2_nopv, @function
_Z15remove_add2_nopv:
	SFPLOADI	L1, 4, 1
	SFPLOADI	L0, 4, 2
	SFPIADD	0, L1, L0, 4
	SFPNOP
	SFPNOP
	SFPSTORE	L1, 2, 0
	SFPSTORE	L0, 2, 8
	ret
	.size	_Z15remove_add2_nopv, .-_Z15remove_add2_nopv
	.align	1
	.globl	_Z14remove_add1_nov
	.type	_Z14remove_add1_nov, @function
_Z14remove_add1_nov:
	SFPLOADI	L2, 4, 1
	SFPLOADI	L1, 4, 2
	SFPMOV	L1, L0, 0
	SFPNOP
	SFPNOP
	SFPIADD	0, L2, L0, 4
	SFPNOP
	SFPNOP
	SFPSTORE	L2, 2, 0
	SFPSTORE	L1, 2, 4
	SFPSTORE	L0, 2, 8
	ret
	.size	_Z14remove_add1_nov, .-_Z14remove_add1_nov
	.align	1
	.globl	_Z14remove_sub1_nov
	.type	_Z14remove_sub1_nov, @function
_Z14remove_sub1_nov:
	SFPLOADI	L1, 4, 1
	SFPLOADI	L2, 4, 2
	SFPMOV	L1, L0, 0
	SFPNOP
	SFPNOP
	SFPIADD	0, L2, L0, 6
	SFPNOP
	SFPNOP
	SFPSTORE	L1, 2, 0
	SFPSTORE	L0, 2, 8
	ret
	.size	_Z14remove_sub1_nov, .-_Z14remove_sub1_nov
	.align	1
	.globl	_Z15remove_and1_yesv
	.type	_Z15remove_and1_yesv, @function
_Z15remove_and1_yesv:
	SFPLOADI	L1, 4, 1
	SFPLOADI	L0, 4, 2
	SFPAND	L1, L0
	SFPNOP
	SFPNOP
	SFPSTORE	L1, 2, 0
	SFPSTORE	L0, 2, 8
	ret
	.size	_Z15remove_and1_yesv, .-_Z15remove_and1_yesv
	.align	1
	.globl	_Z15remove_and2_nopv
	.type	_Z15remove_and2_nopv, @function
_Z15remove_and2_nopv:
	SFPLOADI	L1, 4, 1
	SFPLOADI	L0, 4, 2
	SFPAND	L1, L0
	SFPNOP
	SFPNOP
	SFPSTORE	L1, 2, 0
	SFPSTORE	L0, 2, 8
	ret
	.size	_Z15remove_and2_nopv, .-_Z15remove_and2_nopv
	.align	1
	.globl	_Z14remove_and1_nov
	.type	_Z14remove_and1_nov, @function
_Z14remove_and1_nov:
	SFPLOADI	L2, 4, 1
	SFPLOADI	L1, 4, 2
	SFPMOV	L2, L0, 0
	SFPNOP
	SFPNOP
	SFPAND	L1, L0
	SFPNOP
	SFPNOP
	SFPSTORE	L2, 2, 0
	SFPSTORE	L1, 2, 4
	SFPSTORE	L0, 2, 8
	ret
	.size	_Z14remove_and1_nov, .-_Z14remove_and1_nov
	.align	1
	.globl	_Z14remove_or1_yesv
	.type	_Z14remove_or1_yesv, @function
_Z14remove_or1_yesv:
	SFPLOADI	L1, 4, 1
	SFPLOADI	L0, 4, 2
	SFPOR	L1, L0
	SFPNOP
	SFPNOP
	SFPSTORE	L1, 2, 0
	SFPSTORE	L0, 2, 8
	ret
	.size	_Z14remove_or1_yesv, .-_Z14remove_or1_yesv
	.align	1
	.globl	_Z14remove_or2_nopv
	.type	_Z14remove_or2_nopv, @function
_Z14remove_or2_nopv:
	SFPLOADI	L1, 4, 1
	SFPLOADI	L0, 4, 2
	SFPOR	L1, L0
	SFPNOP
	SFPNOP
	SFPSTORE	L1, 2, 0
	SFPSTORE	L0, 2, 8
	ret
	.size	_Z14remove_or2_nopv, .-_Z14remove_or2_nopv
	.align	1
	.globl	_Z13remove_or1_nov
	.type	_Z13remove_or1_nov, @function
_Z13remove_or1_nov:
	SFPLOADI	L2, 4, 1
	SFPLOADI	L1, 4, 2
	SFPMOV	L2, L0, 0
	SFPNOP
	SFPNOP
	SFPOR	L1, L0
	SFPNOP
	SFPNOP
	SFPSTORE	L2, 2, 0
	SFPSTORE	L1, 2, 4
	SFPSTORE	L0, 2, 8
	ret
	.size	_Z13remove_or1_nov, .-_Z13remove_or1_nov
	.align	1
	.globl	_Z14remove_cmp_yesv
	.type	_Z14remove_cmp_yesv, @function
_Z14remove_cmp_yesv:
	SFPLOADI	L1, 4, 1
	SFPLOADI	L0, 4, 2
	SFPIADD	0, L1, L0, 0
	SFPNOP
	SFPNOP
	SFPNOP
	SFPENCC	3, 10
	SFPNOP
	SFPSTORE	L1, 2, 0
	SFPSTORE	L0, 2, 8
	ret
	.size	_Z14remove_cmp_yesv, .-_Z14remove_cmp_yesv
	.align	1
	.globl	_Z13remove_cmp_nov
	.type	_Z13remove_cmp_nov, @function
_Z13remove_cmp_nov:
	SFPLOADI	L1, 4, 1
	SFPLOADI	L2, 4, 2
	SFPMOV	L1, L0, 0
	SFPNOP
	SFPNOP
	SFPIADD	0, L2, L0, 2
	SFPNOP
	SFPNOP
	SFPNOP
	SFPENCC	3, 10
	SFPNOP
	SFPSTORE	L1, 2, 0
	SFPSTORE	L0, 2, 8
	ret
	.size	_Z13remove_cmp_nov, .-_Z13remove_cmp_nov
	.globl	_ZN7ckernel13instrn_bufferE
	.section	.sbss,"aw",@nobits
	.align	2
	.type	_ZN7ckernel13instrn_bufferE, @object
	.size	_ZN7ckernel13instrn_bufferE, 4
_ZN7ckernel13instrn_bufferE:
	.zero	4
	.ident	"GCC: (GNU) 10.2.0"
