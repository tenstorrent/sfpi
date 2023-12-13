	.file	"regmov.cc"
	.option nopic
	.attribute arch, "rv32i2p0_m2p0_l2p0"
	.attribute unaligned_access, 0
	.attribute stack_align, 16
	.text
	.align	2
	.globl	_Z11remove_add1v
	.type	_Z11remove_add1v, @function
_Z11remove_add1v:
	SFPLOADI	L1, 1, 4
	SFPLOADI	L0, 2, 4
	SFPIADD	L0, L1, 0, 4
	SFPSTORE	0, L1, 12, 7
	SFPSTORE	4, L0, 12, 7
	ret
	.size	_Z11remove_add1v, .-_Z11remove_add1v
	.align	2
	.globl	_Z15remove_add2_nopv
	.type	_Z15remove_add2_nopv, @function
_Z15remove_add2_nopv:
	SFPLOADI	L1, 1, 4
	SFPLOADI	L0, 2, 4
	SFPIADD	L0, L1, 0, 4
	SFPSTORE	0, L1, 12, 7
	SFPSTORE	4, L0, 12, 7
	ret
	.size	_Z15remove_add2_nopv, .-_Z15remove_add2_nopv
	.align	2
	.globl	_Z12replace_add3v
	.type	_Z12replace_add3v, @function
_Z12replace_add3v:
	SFPLOADI	L2, 1, 4
	SFPLOADI	L1, 2, 4
	SFPMOV	L0, L1, 0
	SFPIADD	L0, L2, 0, 4
	SFPSTORE	0, L2, 12, 7
	SFPSTORE	2, L1, 12, 7
	SFPSTORE	4, L0, 12, 7
	ret
	.size	_Z12replace_add3v, .-_Z12replace_add3v
	.align	2
	.globl	_Z12replace_sub1v
	.type	_Z12replace_sub1v, @function
_Z12replace_sub1v:
	SFPLOADI	L1, 1, 4
	SFPLOADI	L2, 2, 4
	SFPMOV	L0, L1, 0
	SFPIADD	L0, L2, 0, 6
	SFPSTORE	0, L1, 12, 7
	SFPSTORE	4, L0, 12, 7
	ret
	.size	_Z12replace_sub1v, .-_Z12replace_sub1v
	.align	2
	.globl	_Z11remove_and1v
	.type	_Z11remove_and1v, @function
_Z11remove_and1v:
	SFPLOADI	L1, 1, 4
	SFPLOADI	L0, 2, 4
	SFPAND	L0, L1
	SFPSTORE	0, L1, 12, 7
	SFPSTORE	4, L0, 12, 7
	ret
	.size	_Z11remove_and1v, .-_Z11remove_and1v
	.align	2
	.globl	_Z15remove_and2_nopv
	.type	_Z15remove_and2_nopv, @function
_Z15remove_and2_nopv:
	SFPLOADI	L1, 1, 4
	SFPLOADI	L0, 2, 4
	SFPAND	L0, L1
	SFPSTORE	0, L1, 12, 7
	SFPSTORE	4, L0, 12, 7
	ret
	.size	_Z15remove_and2_nopv, .-_Z15remove_and2_nopv
	.align	2
	.globl	_Z12replace_and3v
	.type	_Z12replace_and3v, @function
_Z12replace_and3v:
	SFPLOADI	L2, 1, 4
	SFPLOADI	L1, 2, 4
	SFPMOV	L0, L2, 0
	SFPAND	L0, L1
	SFPSTORE	0, L2, 12, 7
	SFPSTORE	2, L1, 12, 7
	SFPSTORE	4, L0, 12, 7
	ret
	.size	_Z12replace_and3v, .-_Z12replace_and3v
	.align	2
	.globl	_Z10remove_or1v
	.type	_Z10remove_or1v, @function
_Z10remove_or1v:
	SFPLOADI	L1, 1, 4
	SFPLOADI	L0, 2, 4
	SFPOR	L0, L1
	SFPSTORE	0, L1, 12, 7
	SFPSTORE	4, L0, 12, 7
	ret
	.size	_Z10remove_or1v, .-_Z10remove_or1v
	.align	2
	.globl	_Z14remove_or2_nopv
	.type	_Z14remove_or2_nopv, @function
_Z14remove_or2_nopv:
	SFPLOADI	L1, 1, 4
	SFPLOADI	L0, 2, 4
	SFPOR	L0, L1
	SFPSTORE	0, L1, 12, 7
	SFPSTORE	4, L0, 12, 7
	ret
	.size	_Z14remove_or2_nopv, .-_Z14remove_or2_nopv
	.align	2
	.globl	_Z11replace_or3v
	.type	_Z11replace_or3v, @function
_Z11replace_or3v:
	SFPLOADI	L2, 1, 4
	SFPLOADI	L1, 2, 4
	SFPMOV	L0, L2, 0
	SFPOR	L0, L1
	SFPSTORE	0, L2, 12, 7
	SFPSTORE	2, L1, 12, 7
	SFPSTORE	4, L0, 12, 7
	ret
	.size	_Z11replace_or3v, .-_Z11replace_or3v
	.align	2
	.globl	_Z16replace_live_or4v
	.type	_Z16replace_live_or4v, @function
_Z16replace_live_or4v:
	SFPLOADI	L1, 1, 4
	SFPLOADI	L3, 2, 4
	SFPLOADI	L0, 3, 4
	SFPIADD	L2, L1, -4, 5
	SFPSETCC	L2, 0, 6
	SFPMOV	L2, L1, 0
	SFPOR	L2, L3
	SFPMOV	L0, L2, 0
	SFPENCC	3, 10
	SFPSTORE	0, L1, 12, 7
	SFPSTORE	2, L3, 12, 7
	SFPSTORE	4, L0, 12, 7
	ret
	.size	_Z16replace_live_or4v, .-_Z16replace_live_or4v
	.align	2
	.globl	_Z11remove_cmp1v
	.type	_Z11remove_cmp1v, @function
_Z11remove_cmp1v:
	SFPLOADI	L1, 1, 4
	SFPLOADI	L0, 2, 4
	SFPIADD	L0, L1, 0, 0
	SFPENCC	3, 10
	SFPSTORE	0, L1, 12, 7
	SFPSTORE	4, L0, 12, 7
	ret
	.size	_Z11remove_cmp1v, .-_Z11remove_cmp1v
	.align	2
	.globl	_Z12replace_cmp2v
	.type	_Z12replace_cmp2v, @function
_Z12replace_cmp2v:
	SFPLOADI	L1, 1, 4
	SFPLOADI	L2, 2, 4
	SFPMOV	L0, L1, 0
	SFPIADD	L0, L2, 0, 2
	SFPENCC	3, 10
	SFPSTORE	0, L1, 12, 7
	SFPSTORE	4, L0, 12, 7
	ret
	.size	_Z12replace_cmp2v, .-_Z12replace_cmp2v
	.align	2
	.globl	_Z12replace_muliv
	.type	_Z12replace_muliv, @function
_Z12replace_muliv:
	SFPLOAD	L1, 0, 0, 7
	SFPLOADI	L0, 16384, 0
	SFPMUL	L0, L1, L0, L9, 0
	SFPSTORE	0, L1, 0, 7
	SFPSTORE	0, L0, 0, 7
	ret
	.size	_Z12replace_muliv, .-_Z12replace_muliv
	.align	2
	.globl	_Z12replace_addiv
	.type	_Z12replace_addiv, @function
_Z12replace_addiv:
	SFPLOAD	L1, 0, 0, 7
	SFPLOADI	L0, 16384, 0
	SFPADD	L0, L10, L1, L0, 0
	SFPSTORE	0, L1, 0, 7
	SFPSTORE	0, L0, 0, 7
	ret
	.size	_Z12replace_addiv, .-_Z12replace_addiv
	.align	2
	.globl	_Z12replace_shftv
	.type	_Z12replace_shftv, @function
_Z12replace_shftv:
	SFPLOADI	L1, 1, 4
	SFPMOV	L0, L1, 0
	SFPSHFT	L0, L0, 1, 1
	SFPSTORE	0, L1, 12, 7
	SFPSTORE	4, L0, 12, 7
	ret
	.size	_Z12replace_shftv, .-_Z12replace_shftv
	.align	2
	.globl	_Z12replace_cregv
	.type	_Z12replace_cregv, @function
_Z12replace_cregv:
	SFPMOV	L0, L13, 0
	SFPSHFT	L0, L0, 1, 1
	SFPSTORE	0, L0, 12, 7
	SFPMOV	L0, L13, 0
	SFPAND	L0, L14
	SFPSTORE	0, L0, 12, 7
	SFPMOV	L0, L14, 0
	SFPOR	L0, L13
	SFPSTORE	0, L0, 12, 7
	ret
	.size	_Z12replace_cregv, .-_Z12replace_cregv
	.align	2
	.globl	_Z2bbi
	.type	_Z2bbi, @function
_Z2bbi:
	SFPLOADI	L1, 1, 4
	SFPLOADI	L2, 2, 4
	SFPLOADI	L0, 3, 4
	bne	a0,zero,.L20
	SFPMOV	L0, L1, 2
	SFPOR	L0, L2
.L20:
	SFPSTORE	0, L0, 12, 7
	ret
	.size	_Z2bbi, .-_Z2bbi
	.align	2
	.globl	_Z7bb_livei
	.type	_Z7bb_livei, @function
_Z7bb_livei:
	SFPLOADI	L0, 1, 4
	SFPLOADI	L2, 2, 4
	SFPLOADI	L1, 3, 4
	SFPSETCC	L0, 0, 6
	bne	a0,zero,.L22
	SFPOR	L0, L2
	SFPMOV	L1, L0, 0
.L22:
	SFPENCC	3, 10
	SFPSTORE	0, L1, 12, 7
	ret
	.size	_Z7bb_livei, .-_Z7bb_livei
	.align	2
	.globl	_Z3bb2i
	.type	_Z3bb2i, @function
_Z3bb2i:
	SFPLOADI	L1, 1, 4
	SFPLOADI	L2, 2, 4
	SFPLOADI	L0, 3, 4
	bne	a0,zero,.L24
	SFPMOV	L0, L2, 2
	SFPOR	L0, L1
.L24:
	SFPSTORE	0, L1, 12, 7
	SFPSTORE	0, L0, 12, 7
	ret
	.size	_Z3bb2i, .-_Z3bb2i
	.align	2
	.globl	_Z8bb2_livei
	.type	_Z8bb2_livei, @function
_Z8bb2_livei:
	SFPLOADI	L0, 1, 4
	SFPLOADI	L2, 2, 4
	SFPLOADI	L1, 3, 4
	SFPSETCC	L0, 0, 6
	bne	a0,zero,.L26
	SFPOR	L2, L0
	SFPMOV	L1, L2, 0
.L26:
	SFPENCC	3, 10
	SFPSTORE	0, L0, 12, 7
	SFPSTORE	0, L1, 12, 7
	ret
	.size	_Z8bb2_livei, .-_Z8bb2_livei
	.align	2
	.globl	_Z3bb3i
	.type	_Z3bb3i, @function
_Z3bb3i:
	SFPLOADI	L1, 1, 4
	SFPLOADI	L0, 3, 4
	bne	a0,zero,.L28
	SFPOR	L0, L1
.L28:
	SFPSTORE	0, L1, 12, 7
	SFPSTORE	0, L0, 12, 7
	ret
	.size	_Z3bb3i, .-_Z3bb3i
	.align	2
	.globl	_Z8bb_loop1i
	.type	_Z8bb_loop1i, @function
_Z8bb_loop1i:
	SFPLOADI	L2, 1, 4
	SFPLOADI	L1, 2, 4
	SFPLOADI	L0, 3, 4
	beq	a0,zero,.L30
.L31:
	SFPMOV	L0, L2, 0
	SFPOR	L0, L1
	addi	a0,a0,-1
	bne	a0,zero,.L31
.L30:
	SFPSTORE	0, L0, 12, 7
	ret
	.size	_Z8bb_loop1i, .-_Z8bb_loop1i
	.align	2
	.globl	_Z8bb_loop2i
	.type	_Z8bb_loop2i, @function
_Z8bb_loop2i:
	SFPLOADI	L1, 1, 4
	SFPLOADI	L0, 2, 4
	beq	a0,zero,.L37
.L38:
	SFPOR	L0, L1
	addi	a0,a0,-1
	bne	a0,zero,.L38
.L37:
	SFPSTORE	0, L1, 12, 7
	SFPSTORE	0, L0, 12, 7
	ret
	.size	_Z8bb_loop2i, .-_Z8bb_loop2i
	.align	2
	.globl	_Z8bb_loop3i
	.type	_Z8bb_loop3i, @function
_Z8bb_loop3i:
	SFPLOADI	L1, 1, 4
	SFPLOADI	L0, 2, 4
	beq	a0,zero,.L44
.L45:
	SFPOR	L0, L1
	addi	a0,a0,-1
	bne	a0,zero,.L45
.L44:
	SFPSTORE	0, L1, 12, 7
	ret
	.size	_Z8bb_loop3i, .-_Z8bb_loop3i
	.align	2
	.globl	_Z10setexp_movv
	.type	_Z10setexp_movv, @function
_Z10setexp_movv:
	SFPLOADI	L2, 16256, 0
	SFPLOADI	L1, 50, 4
	SFPMOV	L0, L1, 0
	SFPSETEXP	L0, L2, 0, 0
	SFPSTORE	0, L2, 0, 7
	SFPSTORE	2, L1, 12, 7
	SFPSTORE	4, L0, 0, 7
	ret
	.size	_Z10setexp_movv, .-_Z10setexp_movv
	.align	2
	.globl	_Z10setman_movv
	.type	_Z10setman_movv, @function
_Z10setman_movv:
	SFPLOADI	L2, 16256, 0
	SFPLOADI	L1, 50, 4
	SFPMOV	L0, L1, 0
	SFPSETMAN	L0, L2, 0, 0
	SFPSTORE	0, L2, 0, 7
	SFPSTORE	2, L1, 12, 7
	SFPSTORE	4, L0, 0, 7
	ret
	.size	_Z10setman_movv, .-_Z10setman_movv
	.align	2
	.globl	_Z10setsgn_movv
	.type	_Z10setsgn_movv, @function
_Z10setsgn_movv:
	SFPLOADI	L2, 16256, 0
	SFPLOADI	L1, 50, 4
	SFPMOV	L0, L1, 0
	SFPSETSGN	L0, L2, 0, 0
	SFPSTORE	0, L2, 0, 7
	SFPSTORE	2, L1, 12, 7
	SFPSTORE	4, L0, 0, 7
	ret
	.size	_Z10setsgn_movv, .-_Z10setsgn_movv
	.align	2
	.globl	_Z11remove_xor1v
	.type	_Z11remove_xor1v, @function
_Z11remove_xor1v:
	SFPLOADI	L1, 1, 4
	SFPLOADI	L0, 2, 4
	SFPXOR	L0, L1
	SFPSTORE	0, L1, 12, 7
	SFPSTORE	4, L0, 12, 7
	ret
	.size	_Z11remove_xor1v, .-_Z11remove_xor1v
	.align	2
	.globl	_Z15remove_xor2_nopv
	.type	_Z15remove_xor2_nopv, @function
_Z15remove_xor2_nopv:
	SFPLOADI	L1, 1, 4
	SFPLOADI	L0, 2, 4
	SFPXOR	L0, L1
	SFPSTORE	0, L1, 12, 7
	SFPSTORE	4, L0, 12, 7
	ret
	.size	_Z15remove_xor2_nopv, .-_Z15remove_xor2_nopv
	.align	2
	.globl	_Z12replace_xor3v
	.type	_Z12replace_xor3v, @function
_Z12replace_xor3v:
	SFPLOADI	L2, 1, 4
	SFPLOADI	L1, 2, 4
	SFPMOV	L0, L2, 0
	SFPXOR	L0, L1
	SFPSTORE	0, L2, 12, 7
	SFPSTORE	2, L1, 12, 7
	SFPSTORE	4, L0, 12, 7
	ret
	.size	_Z12replace_xor3v, .-_Z12replace_xor3v
	.align	2
	.globl	_Z17swap_const_reg_orv
	.type	_Z17swap_const_reg_orv, @function
_Z17swap_const_reg_orv:
	SFPLOADI	L1, 2, 4
	SFPMOV	L0, L1, 0
	SFPOR	L0, L15
	SFPSTORE	0, L1, 12, 7
	SFPSTORE	0, L0, 12, 7
	ret
	.size	_Z17swap_const_reg_orv, .-_Z17swap_const_reg_orv
	.align	2
	.globl	_Z17move_const_reg_orv
	.type	_Z17move_const_reg_orv, @function
_Z17move_const_reg_orv:
	SFPLOADI	L0, 2, 4
	SFPOR	L0, L15
	SFPSTORE	0, L0, 12, 7
	ret
	.size	_Z17move_const_reg_orv, .-_Z17move_const_reg_orv
	.align	2
	.globl	_Z18move_const_reg2_orv
	.type	_Z18move_const_reg2_orv, @function
_Z18move_const_reg2_orv:
	SFPMOV	L0, L15, 0
	SFPOR	L0, L15
	SFPSTORE	0, L0, 12, 7
	ret
	.size	_Z18move_const_reg2_orv, .-_Z18move_const_reg2_orv
	.align	2
	.globl	_Z8move_lutv
	.type	_Z8move_lutv, @function
_Z8move_lutv:
	SFPLOADI	L0, 1, 4
	SFPLOADI	L1, 2, 4
	SFPLOADI	L2, 3, 4
	SFPLOADI	L4, 16256, 0
	SFPMOV	L3, L4, 0
	SFPLUT	L3, 4
	SFPNOP
	SFPSTORE	0, L3, 0, 7
	SFPSTORE	2, L4, 0, 7
	ret
	.size	_Z8move_lutv, .-_Z8move_lutv
	.globl	_ZN7ckernel13instrn_bufferE
	.section	.sbss,"aw",@nobits
	.align	2
	.type	_ZN7ckernel13instrn_bufferE, @object
	.size	_ZN7ckernel13instrn_bufferE, 4
_ZN7ckernel13instrn_bufferE:
	.zero	4
	.ident	"GCC: (GNU) 10.2.0"
