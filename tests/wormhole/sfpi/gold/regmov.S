	.file	"regmov.cc"
	.option nopic
	.attribute arch, "rv32i2p0_m2p0_w2p0"
	.attribute unaligned_access, 0
	.attribute stack_align, 16
	.text
	.align	1
	.globl	_Z11remove_add1v
	.type	_Z11remove_add1v, @function
_Z11remove_add1v:
	SFPLOADI	L1, 4, 1
	SFPLOADI	L0, 4, 2
	SFPIADD	0, L1, L0, 4
	SFPSTORE	L1, 0, 3, 0
	SFPSTORE	L0, 0, 3, 4
	ret
	.size	_Z11remove_add1v, .-_Z11remove_add1v
	.align	1
	.globl	_Z15remove_add2_nopv
	.type	_Z15remove_add2_nopv, @function
_Z15remove_add2_nopv:
	SFPLOADI	L1, 4, 1
	SFPLOADI	L0, 4, 2
	SFPIADD	0, L1, L0, 4
	SFPSTORE	L1, 0, 3, 0
	SFPSTORE	L0, 0, 3, 4
	ret
	.size	_Z15remove_add2_nopv, .-_Z15remove_add2_nopv
	.align	1
	.globl	_Z12replace_add3v
	.type	_Z12replace_add3v, @function
_Z12replace_add3v:
	SFPLOADI	L2, 4, 1
	SFPLOADI	L1, 4, 2
	SFPMOV	L1, L0, 0
	SFPIADD	0, L2, L0, 4
	SFPSTORE	L2, 0, 3, 0
	SFPSTORE	L1, 0, 3, 2
	SFPSTORE	L0, 0, 3, 4
	ret
	.size	_Z12replace_add3v, .-_Z12replace_add3v
	.align	1
	.globl	_Z12replace_sub1v
	.type	_Z12replace_sub1v, @function
_Z12replace_sub1v:
	SFPLOADI	L1, 4, 1
	SFPLOADI	L2, 4, 2
	SFPMOV	L1, L0, 0
	SFPIADD	0, L2, L0, 6
	SFPSTORE	L1, 0, 3, 0
	SFPSTORE	L0, 0, 3, 4
	ret
	.size	_Z12replace_sub1v, .-_Z12replace_sub1v
	.align	1
	.globl	_Z11remove_and1v
	.type	_Z11remove_and1v, @function
_Z11remove_and1v:
	SFPLOADI	L1, 4, 1
	SFPLOADI	L0, 4, 2
	SFPAND	L1, L0
	SFPSTORE	L1, 0, 3, 0
	SFPSTORE	L0, 0, 3, 4
	ret
	.size	_Z11remove_and1v, .-_Z11remove_and1v
	.align	1
	.globl	_Z15remove_and2_nopv
	.type	_Z15remove_and2_nopv, @function
_Z15remove_and2_nopv:
	SFPLOADI	L1, 4, 1
	SFPLOADI	L0, 4, 2
	SFPAND	L1, L0
	SFPSTORE	L1, 0, 3, 0
	SFPSTORE	L0, 0, 3, 4
	ret
	.size	_Z15remove_and2_nopv, .-_Z15remove_and2_nopv
	.align	1
	.globl	_Z12replace_and3v
	.type	_Z12replace_and3v, @function
_Z12replace_and3v:
	SFPLOADI	L2, 4, 1
	SFPLOADI	L1, 4, 2
	SFPMOV	L2, L0, 0
	SFPAND	L1, L0
	SFPSTORE	L2, 0, 3, 0
	SFPSTORE	L1, 0, 3, 2
	SFPSTORE	L0, 0, 3, 4
	ret
	.size	_Z12replace_and3v, .-_Z12replace_and3v
	.align	1
	.globl	_Z10remove_or1v
	.type	_Z10remove_or1v, @function
_Z10remove_or1v:
	SFPLOADI	L1, 4, 1
	SFPLOADI	L0, 4, 2
	SFPOR	L1, L0
	SFPSTORE	L1, 0, 3, 0
	SFPSTORE	L0, 0, 3, 4
	ret
	.size	_Z10remove_or1v, .-_Z10remove_or1v
	.align	1
	.globl	_Z14remove_or2_nopv
	.type	_Z14remove_or2_nopv, @function
_Z14remove_or2_nopv:
	SFPLOADI	L1, 4, 1
	SFPLOADI	L0, 4, 2
	SFPOR	L1, L0
	SFPSTORE	L1, 0, 3, 0
	SFPSTORE	L0, 0, 3, 4
	ret
	.size	_Z14remove_or2_nopv, .-_Z14remove_or2_nopv
	.align	1
	.globl	_Z11replace_or3v
	.type	_Z11replace_or3v, @function
_Z11replace_or3v:
	SFPLOADI	L2, 4, 1
	SFPLOADI	L1, 4, 2
	SFPMOV	L2, L0, 0
	SFPOR	L1, L0
	SFPSTORE	L2, 0, 3, 0
	SFPSTORE	L1, 0, 3, 2
	SFPSTORE	L0, 0, 3, 4
	ret
	.size	_Z11replace_or3v, .-_Z11replace_or3v
	.align	1
	.globl	_Z16replace_live_or4v
	.type	_Z16replace_live_or4v, @function
_Z16replace_live_or4v:
	SFPLOADI	L1, 4, 1
	SFPLOADI	L3, 4, 2
	SFPLOADI	L0, 4, 3
	SFPIADD	-4, L1, L2, 5
	SFPSETCC	0, L2, 6
	SFPMOV	L1, L2, 0
	SFPOR	L3, L2
	SFPMOV	L2, L0, 0
	SFPENCC	3, 10
	SFPSTORE	L1, 0, 3, 0
	SFPSTORE	L3, 0, 3, 2
	SFPSTORE	L0, 0, 3, 4
	ret
	.size	_Z16replace_live_or4v, .-_Z16replace_live_or4v
	.align	1
	.globl	_Z11remove_cmp1v
	.type	_Z11remove_cmp1v, @function
_Z11remove_cmp1v:
	SFPLOADI	L1, 4, 1
	SFPLOADI	L0, 4, 2
	SFPIADD	0, L1, L0, 0
	SFPENCC	3, 10
	SFPSTORE	L1, 0, 3, 0
	SFPSTORE	L0, 0, 3, 4
	ret
	.size	_Z11remove_cmp1v, .-_Z11remove_cmp1v
	.align	1
	.globl	_Z12replace_cmp2v
	.type	_Z12replace_cmp2v, @function
_Z12replace_cmp2v:
	SFPLOADI	L1, 4, 1
	SFPLOADI	L2, 4, 2
	SFPMOV	L1, L0, 0
	SFPIADD	0, L2, L0, 2
	SFPENCC	3, 10
	SFPSTORE	L1, 0, 3, 0
	SFPSTORE	L0, 0, 3, 4
	ret
	.size	_Z12replace_cmp2v, .-_Z12replace_cmp2v
	.align	1
	.globl	_Z12replace_muliv
	.type	_Z12replace_muliv, @function
_Z12replace_muliv:
	SFPLOAD	L1, 0, 3, 0
	SFPLOADI	L0, 0, 16384
	SFPMUL	L1, L0, L9, L0, 0
	SFPSTORE	L1, 0, 3, 0
	SFPSTORE	L0, 0, 3, 0
	ret
	.size	_Z12replace_muliv, .-_Z12replace_muliv
	.align	1
	.globl	_Z12replace_addiv
	.type	_Z12replace_addiv, @function
_Z12replace_addiv:
	SFPLOAD	L1, 0, 3, 0
	SFPLOADI	L0, 0, 16384
	SFPADD	L10, L1, L0, L0, 0
	SFPSTORE	L1, 0, 3, 0
	SFPSTORE	L0, 0, 3, 0
	ret
	.size	_Z12replace_addiv, .-_Z12replace_addiv
	.align	1
	.globl	_Z12replace_shftv
	.type	_Z12replace_shftv, @function
_Z12replace_shftv:
	SFPLOADI	L1, 4, 1
	SFPMOV	L1, L0, 0
	SFPSHFT	1, L0, L0, 1
	SFPSTORE	L1, 0, 3, 0
	SFPSTORE	L0, 0, 3, 4
	ret
	.size	_Z12replace_shftv, .-_Z12replace_shftv
	.align	1
	.globl	_Z12replace_cregv
	.type	_Z12replace_cregv, @function
_Z12replace_cregv:
	SFPMOV	L13, L0, 0
	SFPSHFT	1, L0, L0, 1
	SFPSTORE	L0, 0, 3, 0
	SFPMOV	L13, L0, 0
	SFPAND	L14, L0
	SFPSTORE	L0, 0, 3, 0
	SFPMOV	L14, L0, 0
	SFPOR	L13, L0
	SFPSTORE	L0, 0, 3, 0
	ret
	.size	_Z12replace_cregv, .-_Z12replace_cregv
	.align	1
	.globl	_Z2bbi
	.type	_Z2bbi, @function
_Z2bbi:
	SFPLOADI	L1, 4, 1
	SFPLOADI	L2, 4, 2
	SFPLOADI	L0, 4, 3
	bne	a0,zero,.L20
	SFPWNOP
	SFPMOV	L1, L0, 2
	SFPOR	L2, L0
.L20:
	SFPSTORE	L0, 0, 3, 0
	ret
	.size	_Z2bbi, .-_Z2bbi
	.align	1
	.globl	_Z7bb_livei
	.type	_Z7bb_livei, @function
_Z7bb_livei:
	SFPLOADI	L0, 4, 1
	SFPLOADI	L2, 4, 2
	SFPLOADI	L1, 4, 3
	SFPSETCC	0, L0, 6
	bne	a0,zero,.L22
	SFPOR	L2, L0
	SFPMOV	L0, L1, 0
.L22:
	SFPENCC	3, 10
	SFPSTORE	L1, 0, 3, 0
	ret
	.size	_Z7bb_livei, .-_Z7bb_livei
	.align	1
	.globl	_Z3bb2i
	.type	_Z3bb2i, @function
_Z3bb2i:
	SFPLOADI	L1, 4, 1
	SFPLOADI	L2, 4, 2
	SFPLOADI	L0, 4, 3
	bne	a0,zero,.L24
	SFPWNOP
	SFPMOV	L2, L0, 2
	SFPOR	L1, L0
.L24:
	SFPSTORE	L1, 0, 3, 0
	SFPSTORE	L0, 0, 3, 0
	ret
	.size	_Z3bb2i, .-_Z3bb2i
	.align	1
	.globl	_Z8bb2_livei
	.type	_Z8bb2_livei, @function
_Z8bb2_livei:
	SFPLOADI	L0, 4, 1
	SFPLOADI	L2, 4, 2
	SFPLOADI	L1, 4, 3
	SFPSETCC	0, L0, 6
	bne	a0,zero,.L26
	SFPOR	L0, L2
	SFPMOV	L2, L1, 0
.L26:
	SFPENCC	3, 10
	SFPSTORE	L0, 0, 3, 0
	SFPSTORE	L1, 0, 3, 0
	ret
	.size	_Z8bb2_livei, .-_Z8bb2_livei
	.align	1
	.globl	_Z3bb3i
	.type	_Z3bb3i, @function
_Z3bb3i:
	SFPLOADI	L1, 4, 1
	SFPLOADI	L0, 4, 3
	bne	a0,zero,.L28
	SFPOR	L1, L0
.L28:
	SFPSTORE	L1, 0, 3, 0
	SFPSTORE	L0, 0, 3, 0
	ret
	.size	_Z3bb3i, .-_Z3bb3i
	.align	1
	.globl	_Z8bb_loop1i
	.type	_Z8bb_loop1i, @function
_Z8bb_loop1i:
	SFPLOADI	L2, 4, 1
	SFPLOADI	L1, 4, 2
	SFPLOADI	L0, 4, 3
	beq	a0,zero,.L30
.L31:
	SFPMOV	L2, L0, 0
	SFPOR	L1, L0
	addi	a0,a0,-1
	bne	a0,zero,.L31
.L30:
	SFPSTORE	L0, 0, 3, 0
	ret
	.size	_Z8bb_loop1i, .-_Z8bb_loop1i
	.align	1
	.globl	_Z8bb_loop2i
	.type	_Z8bb_loop2i, @function
_Z8bb_loop2i:
	SFPLOADI	L1, 4, 1
	SFPLOADI	L0, 4, 2
	beq	a0,zero,.L37
.L38:
	SFPOR	L1, L0
	addi	a0,a0,-1
	bne	a0,zero,.L38
.L37:
	SFPSTORE	L1, 0, 3, 0
	SFPSTORE	L0, 0, 3, 0
	ret
	.size	_Z8bb_loop2i, .-_Z8bb_loop2i
	.align	1
	.globl	_Z8bb_loop3i
	.type	_Z8bb_loop3i, @function
_Z8bb_loop3i:
	SFPLOADI	L1, 4, 1
	SFPLOADI	L0, 4, 2
	beq	a0,zero,.L44
.L45:
	SFPOR	L1, L0
	addi	a0,a0,-1
	bne	a0,zero,.L45
.L44:
	SFPSTORE	L1, 0, 3, 0
	ret
	.size	_Z8bb_loop3i, .-_Z8bb_loop3i
	.align	1
	.globl	_Z10setexp_movv
	.type	_Z10setexp_movv, @function
_Z10setexp_movv:
	SFPLOADI	L2, 0, 16256
	SFPLOADI	L1, 4, 50
	SFPMOV	L1, L0, 0
	SFPSETEXP	0, L2, L0, 0
	SFPSTORE	L2, 0, 3, 0
	SFPSTORE	L1, 0, 3, 2
	SFPSTORE	L0, 0, 3, 4
	ret
	.size	_Z10setexp_movv, .-_Z10setexp_movv
	.align	1
	.globl	_Z10setman_movv
	.type	_Z10setman_movv, @function
_Z10setman_movv:
	SFPLOADI	L2, 0, 16256
	SFPLOADI	L1, 4, 50
	SFPMOV	L1, L0, 0
	SFPSETMAN	0, L2, L0, 0
	SFPSTORE	L2, 0, 3, 0
	SFPSTORE	L1, 0, 3, 2
	SFPSTORE	L0, 0, 3, 4
	ret
	.size	_Z10setman_movv, .-_Z10setman_movv
	.align	1
	.globl	_Z10setsgn_movv
	.type	_Z10setsgn_movv, @function
_Z10setsgn_movv:
	SFPLOADI	L2, 0, 16256
	SFPLOADI	L1, 4, 50
	SFPMOV	L1, L0, 0
	SFPSETSGN	0, L2, L0, 0
	SFPSTORE	L2, 0, 3, 0
	SFPSTORE	L1, 0, 3, 2
	SFPSTORE	L0, 0, 3, 4
	ret
	.size	_Z10setsgn_movv, .-_Z10setsgn_movv
	.align	1
	.globl	_Z11remove_xor1v
	.type	_Z11remove_xor1v, @function
_Z11remove_xor1v:
	SFPLOADI	L1, 4, 1
	SFPLOADI	L0, 4, 2
	SFPXOR	L1, L0
	SFPSTORE	L1, 0, 3, 0
	SFPSTORE	L0, 0, 3, 4
	ret
	.size	_Z11remove_xor1v, .-_Z11remove_xor1v
	.align	1
	.globl	_Z15remove_xor2_nopv
	.type	_Z15remove_xor2_nopv, @function
_Z15remove_xor2_nopv:
	SFPLOADI	L1, 4, 1
	SFPLOADI	L0, 4, 2
	SFPXOR	L1, L0
	SFPSTORE	L1, 0, 3, 0
	SFPSTORE	L0, 0, 3, 4
	ret
	.size	_Z15remove_xor2_nopv, .-_Z15remove_xor2_nopv
	.align	1
	.globl	_Z12replace_xor3v
	.type	_Z12replace_xor3v, @function
_Z12replace_xor3v:
	SFPLOADI	L2, 4, 1
	SFPLOADI	L1, 4, 2
	SFPMOV	L2, L0, 0
	SFPXOR	L1, L0
	SFPSTORE	L2, 0, 3, 0
	SFPSTORE	L1, 0, 3, 2
	SFPSTORE	L0, 0, 3, 4
	ret
	.size	_Z12replace_xor3v, .-_Z12replace_xor3v
	.align	1
	.globl	_Z17swap_const_reg_orv
	.type	_Z17swap_const_reg_orv, @function
_Z17swap_const_reg_orv:
	SFPLOADI	L1, 4, 2
	SFPMOV	L1, L0, 0
	SFPOR	L15, L0
	SFPSTORE	L1, 0, 3, 0
	SFPSTORE	L0, 0, 3, 0
	ret
	.size	_Z17swap_const_reg_orv, .-_Z17swap_const_reg_orv
	.align	1
	.globl	_Z17move_const_reg_orv
	.type	_Z17move_const_reg_orv, @function
_Z17move_const_reg_orv:
	SFPLOADI	L0, 4, 2
	SFPOR	L15, L0
	SFPSTORE	L0, 0, 3, 0
	ret
	.size	_Z17move_const_reg_orv, .-_Z17move_const_reg_orv
	.align	1
	.globl	_Z18move_const_reg2_orv
	.type	_Z18move_const_reg2_orv, @function
_Z18move_const_reg2_orv:
	SFPMOV	L15, L0, 0
	SFPOR	L15, L0
	SFPSTORE	L0, 0, 3, 0
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
