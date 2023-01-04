	.file	"regmov.cc"
	.option nopic
	.attribute arch, "rv32i2p0_y2p0"
	.attribute unaligned_access, 0
	.attribute stack_align, 16
	.text
	.align	1
	.globl	_Z11remove_add1v
	.type	_Z11remove_add1v, @function
_Z11remove_add1v:
	SFPLOADI	L1, 1, 4
	SFPLOADI	L0, 2, 4
	SFPIADD	L0, L1, 0, 4
	TTNOP
	TTNOP
	SFPSTORE	0, L1, 2
	SFPSTORE	8, L0, 2
	ret
	.size	_Z11remove_add1v, .-_Z11remove_add1v
	.align	1
	.globl	_Z15remove_add2_nopv
	.type	_Z15remove_add2_nopv, @function
_Z15remove_add2_nopv:
	SFPLOADI	L1, 1, 4
	SFPLOADI	L0, 2, 4
	SFPIADD	L0, L1, 0, 4
	TTNOP
	TTNOP
	SFPSTORE	0, L1, 2
	SFPSTORE	8, L0, 2
	ret
	.size	_Z15remove_add2_nopv, .-_Z15remove_add2_nopv
	.align	1
	.globl	_Z12replace_add3v
	.type	_Z12replace_add3v, @function
_Z12replace_add3v:
	SFPLOADI	L2, 1, 4
	SFPLOADI	L1, 2, 4
	SFPMOV	L0, L1, 0
	TTNOP
	TTNOP
	SFPIADD	L0, L2, 0, 4
	TTNOP
	TTNOP
	SFPSTORE	0, L2, 2
	SFPSTORE	4, L1, 2
	SFPSTORE	8, L0, 2
	ret
	.size	_Z12replace_add3v, .-_Z12replace_add3v
	.align	1
	.globl	_Z12replace_sub1v
	.type	_Z12replace_sub1v, @function
_Z12replace_sub1v:
	SFPLOADI	L1, 1, 4
	SFPLOADI	L2, 2, 4
	SFPMOV	L0, L1, 0
	TTNOP
	TTNOP
	SFPIADD	L0, L2, 0, 6
	TTNOP
	TTNOP
	SFPSTORE	0, L1, 2
	SFPSTORE	8, L0, 2
	ret
	.size	_Z12replace_sub1v, .-_Z12replace_sub1v
	.align	1
	.globl	_Z11remove_and1v
	.type	_Z11remove_and1v, @function
_Z11remove_and1v:
	SFPLOADI	L1, 1, 4
	SFPLOADI	L0, 2, 4
	SFPAND	L0, L1
	TTNOP
	TTNOP
	SFPSTORE	0, L1, 2
	SFPSTORE	8, L0, 2
	ret
	.size	_Z11remove_and1v, .-_Z11remove_and1v
	.align	1
	.globl	_Z15remove_and2_nopv
	.type	_Z15remove_and2_nopv, @function
_Z15remove_and2_nopv:
	SFPLOADI	L1, 1, 4
	SFPLOADI	L0, 2, 4
	SFPAND	L0, L1
	TTNOP
	TTNOP
	SFPSTORE	0, L1, 2
	SFPSTORE	8, L0, 2
	ret
	.size	_Z15remove_and2_nopv, .-_Z15remove_and2_nopv
	.align	1
	.globl	_Z12replace_and3v
	.type	_Z12replace_and3v, @function
_Z12replace_and3v:
	SFPLOADI	L2, 1, 4
	SFPLOADI	L1, 2, 4
	SFPMOV	L0, L2, 0
	TTNOP
	TTNOP
	SFPAND	L0, L1
	TTNOP
	TTNOP
	SFPSTORE	0, L2, 2
	SFPSTORE	4, L1, 2
	SFPSTORE	8, L0, 2
	ret
	.size	_Z12replace_and3v, .-_Z12replace_and3v
	.align	1
	.globl	_Z10remove_or1v
	.type	_Z10remove_or1v, @function
_Z10remove_or1v:
	SFPLOADI	L1, 1, 4
	SFPLOADI	L0, 2, 4
	SFPOR	L0, L1
	TTNOP
	TTNOP
	SFPSTORE	0, L1, 2
	SFPSTORE	8, L0, 2
	ret
	.size	_Z10remove_or1v, .-_Z10remove_or1v
	.align	1
	.globl	_Z14remove_or2_nopv
	.type	_Z14remove_or2_nopv, @function
_Z14remove_or2_nopv:
	SFPLOADI	L1, 1, 4
	SFPLOADI	L0, 2, 4
	SFPOR	L0, L1
	TTNOP
	TTNOP
	SFPSTORE	0, L1, 2
	SFPSTORE	8, L0, 2
	ret
	.size	_Z14remove_or2_nopv, .-_Z14remove_or2_nopv
	.align	1
	.globl	_Z11replace_or3v
	.type	_Z11replace_or3v, @function
_Z11replace_or3v:
	SFPLOADI	L2, 1, 4
	SFPLOADI	L1, 2, 4
	SFPMOV	L0, L2, 0
	TTNOP
	TTNOP
	SFPOR	L0, L1
	TTNOP
	TTNOP
	SFPSTORE	0, L2, 2
	SFPSTORE	4, L1, 2
	SFPSTORE	8, L0, 2
	ret
	.size	_Z11replace_or3v, .-_Z11replace_or3v
	.align	1
	.globl	_Z16replace_live_or4v
	.type	_Z16replace_live_or4v, @function
_Z16replace_live_or4v:
	SFPLOADI	L1, 1, 4
	SFPLOADI	L3, 2, 4
	SFPLOADI	L0, 3, 4
	SFPIADD	L2, L1, -4, 5
	TTNOP
	TTNOP
	SFPSETCC	L2, 0, 6
	TTNOP
	SFPMOV	L2, L1, 0
	TTNOP
	TTNOP
	SFPOR	L2, L3
	TTNOP
	TTNOP
	SFPMOV	L0, L2, 0
	TTNOP
	TTNOP
	SFPENCC	3, 10
	TTNOP
	SFPSTORE	0, L1, 2
	SFPSTORE	4, L3, 2
	SFPSTORE	8, L0, 2
	ret
	.size	_Z16replace_live_or4v, .-_Z16replace_live_or4v
	.align	1
	.globl	_Z11remove_cmp1v
	.type	_Z11remove_cmp1v, @function
_Z11remove_cmp1v:
	SFPLOADI	L1, 1, 4
	SFPLOADI	L0, 2, 4
	SFPIADD	L0, L1, 0, 0
	TTNOP
	TTNOP
	TTNOP
	SFPENCC	3, 10
	TTNOP
	SFPSTORE	0, L1, 2
	SFPSTORE	8, L0, 2
	ret
	.size	_Z11remove_cmp1v, .-_Z11remove_cmp1v
	.align	1
	.globl	_Z12replace_cmp2v
	.type	_Z12replace_cmp2v, @function
_Z12replace_cmp2v:
	SFPLOADI	L1, 1, 4
	SFPLOADI	L2, 2, 4
	SFPMOV	L0, L1, 0
	TTNOP
	TTNOP
	SFPIADD	L0, L2, 0, 2
	TTNOP
	TTNOP
	TTNOP
	SFPENCC	3, 10
	TTNOP
	SFPSTORE	0, L1, 2
	SFPSTORE	8, L0, 2
	ret
	.size	_Z12replace_cmp2v, .-_Z12replace_cmp2v
	.align	1
	.globl	_Z12replace_muliv
	.type	_Z12replace_muliv, @function
_Z12replace_muliv:
	SFPLOAD	L1, 0, 1
	SFPLOADI	L0, 16384, 0
	SFPMUL	L0, L1, L0, L4, 0
	TTNOP
	TTNOP
	SFPSTORE	0, L1, 0
	SFPSTORE	0, L0, 0
	ret
	.size	_Z12replace_muliv, .-_Z12replace_muliv
	.align	1
	.globl	_Z12replace_addiv
	.type	_Z12replace_addiv, @function
_Z12replace_addiv:
	SFPLOAD	L1, 0, 1
	SFPLOADI	L0, 16384, 0
	SFPADD	L0, L10, L1, L0, 0
	TTNOP
	TTNOP
	SFPSTORE	0, L1, 0
	SFPSTORE	0, L0, 0
	ret
	.size	_Z12replace_addiv, .-_Z12replace_addiv
	.align	1
	.globl	_Z12replace_shftv
	.type	_Z12replace_shftv, @function
_Z12replace_shftv:
	SFPLOADI	L1, 1, 4
	SFPMOV	L0, L1, 0
	TTNOP
	TTNOP
	SFPSHFT	L0, L0, 1, 1
	TTNOP
	TTNOP
	SFPSTORE	0, L1, 2
	SFPSTORE	8, L0, 2
	ret
	.size	_Z12replace_shftv, .-_Z12replace_shftv
	.align	1
	.globl	_Z12replace_cregv
	.type	_Z12replace_cregv, @function
_Z12replace_cregv:
	SFPMOV	L0, L15, 0
	TTNOP
	TTNOP
	SFPSHFT	L0, L0, 1, 1
	TTNOP
	TTNOP
	SFPSTORE	0, L0, 2
	SFPMOV	L0, L15, 0
	TTNOP
	TTNOP
	SFPAND	L0, L15
	TTNOP
	TTNOP
	SFPSTORE	0, L0, 2
	SFPMOV	L0, L15, 0
	TTNOP
	TTNOP
	SFPOR	L0, L15
	TTNOP
	TTNOP
	SFPSTORE	0, L0, 2
	ret
	.size	_Z12replace_cregv, .-_Z12replace_cregv
	.align	1
	.globl	_Z2bbi
	.type	_Z2bbi, @function
_Z2bbi:
	SFPLOADI	L1, 1, 4
	SFPLOADI	L2, 2, 4
	SFPLOADI	L0, 3, 4
	bne	a0,zero,.L20
	SFPPUSHC
	SFPENCC	3, 2
	TTNOP
	SFPMOV	L0, L1, 0
	TTNOP
	TTNOP
	SFPPOPC
	TTNOP
	SFPOR	L0, L2
	TTNOP
	TTNOP
.L20:
	SFPSTORE	0, L0, 2
	ret
	.size	_Z2bbi, .-_Z2bbi
	.align	1
	.globl	_Z3bb2i
	.type	_Z3bb2i, @function
_Z3bb2i:
	SFPLOADI	L1, 1, 4
	SFPLOADI	L2, 2, 4
	SFPLOADI	L0, 3, 4
	bne	a0,zero,.L22
	SFPPUSHC
	SFPENCC	3, 2
	TTNOP
	SFPMOV	L0, L2, 0
	TTNOP
	TTNOP
	SFPPOPC
	TTNOP
	SFPOR	L0, L1
	TTNOP
	TTNOP
.L22:
	SFPSTORE	0, L1, 2
	SFPSTORE	0, L0, 2
	ret
	.size	_Z3bb2i, .-_Z3bb2i
	.align	1
	.globl	_Z3bb3i
	.type	_Z3bb3i, @function
_Z3bb3i:
	SFPLOADI	L1, 1, 4
	SFPLOADI	L0, 3, 4
	bne	a0,zero,.L24
	SFPOR	L0, L1
	TTNOP
	TTNOP
.L24:
	SFPSTORE	0, L1, 2
	SFPSTORE	0, L0, 2
	ret
	.size	_Z3bb3i, .-_Z3bb3i
	.align	1
	.globl	_Z10setexp_movv
	.type	_Z10setexp_movv, @function
_Z10setexp_movv:
	SFPLOADI	L2, 16256, 0
	SFPLOADI	L1, 50, 4
	SFPMOV	L0, L1, 0
	TTNOP
	TTNOP
	SFPSETEXP	L0, L2, 0, 0
	TTNOP
	TTNOP
	SFPSTORE	0, L2, 0
	SFPSTORE	4, L1, 2
	SFPSTORE	8, L0, 0
	ret
	.size	_Z10setexp_movv, .-_Z10setexp_movv
	.align	1
	.globl	_Z10setman_movv
	.type	_Z10setman_movv, @function
_Z10setman_movv:
	SFPLOADI	L2, 16256, 0
	SFPLOADI	L1, 50, 4
	SFPMOV	L0, L1, 0
	TTNOP
	TTNOP
	SFPSETMAN	L0, L2, 0, 0
	TTNOP
	TTNOP
	SFPSTORE	0, L2, 0
	SFPSTORE	4, L1, 2
	SFPSTORE	8, L0, 0
	ret
	.size	_Z10setman_movv, .-_Z10setman_movv
	.align	1
	.globl	_Z10setsgn_movv
	.type	_Z10setsgn_movv, @function
_Z10setsgn_movv:
	SFPLOADI	L2, 16256, 0
	SFPLOADI	L1, 50, 4
	SFPMOV	L0, L1, 0
	TTNOP
	TTNOP
	SFPSETSGN	L0, L2, 0, 0
	TTNOP
	TTNOP
	SFPSTORE	0, L2, 0
	SFPSTORE	4, L1, 2
	SFPSTORE	8, L0, 0
	ret
	.size	_Z10setsgn_movv, .-_Z10setsgn_movv
	.align	1
	.globl	_Z12replace_xor3v
	.type	_Z12replace_xor3v, @function
_Z12replace_xor3v:
	SFPLOADI	L3, 1, 4
	SFPLOADI	L2, 2, 4
	SFPMOV	L0, L3, 0
	TTNOP
	TTNOP
	SFPOR	L0, L2
	TTNOP
	TTNOP
	SFPMOV	L1, L3, 0
	TTNOP
	TTNOP
	SFPAND	L1, L2
	TTNOP
	TTNOP
	SFPNOT	L1, L1
	TTNOP
	TTNOP
	SFPAND	L0, L1
	TTNOP
	TTNOP
	SFPSTORE	0, L3, 2
	SFPSTORE	4, L2, 2
	SFPSTORE	8, L0, 2
	ret
	.size	_Z12replace_xor3v, .-_Z12replace_xor3v
	.align	1
	.globl	_Z17swap_const_reg_orv
	.type	_Z17swap_const_reg_orv, @function
_Z17swap_const_reg_orv:
	SFPLOADI	L1, 2, 4
	SFPMOV	L0, L1, 0
	TTNOP
	TTNOP
	SFPOR	L0, L15
	TTNOP
	TTNOP
	SFPSTORE	0, L1, 2
	SFPSTORE	0, L0, 2
	ret
	.size	_Z17swap_const_reg_orv, .-_Z17swap_const_reg_orv
	.align	1
	.globl	_Z17move_const_reg_orv
	.type	_Z17move_const_reg_orv, @function
_Z17move_const_reg_orv:
	SFPLOADI	L0, 2, 4
	SFPOR	L0, L15
	TTNOP
	TTNOP
	SFPSTORE	0, L0, 2
	ret
	.size	_Z17move_const_reg_orv, .-_Z17move_const_reg_orv
	.align	1
	.globl	_Z18move_const_reg2_orv
	.type	_Z18move_const_reg2_orv, @function
_Z18move_const_reg2_orv:
	SFPMOV	L0, L15, 0
	TTNOP
	TTNOP
	SFPOR	L0, L15
	TTNOP
	TTNOP
	SFPSTORE	0, L0, 2
	ret
	.size	_Z18move_const_reg2_orv, .-_Z18move_const_reg2_orv
	.globl	_ZN7ckernel13instrn_bufferE
	.section	.sbss,"aw",@nobits
	.align	2
	.type	_ZN7ckernel13instrn_bufferE, @object
	.size	_ZN7ckernel13instrn_bufferE, 4
_ZN7ckernel13instrn_bufferE:
	.zero	4
	.ident	"GCC: (GNU) 10.2.0"
