	.file	"live.cc"
	.option nopic
	.attribute arch, "rv32i2p0_y2p0"
	.attribute unaligned_access, 0
	.attribute stack_align, 16
	.text
	.align	1
	.globl	_Z9abs_setccv
	.type	_Z9abs_setccv, @function
_Z9abs_setccv:
	SFPLOADI	L1, 0, 49568
	SFPLOADI	L0, 0, 49648
	SFPLOAD	L2, 1, 0
	SFPSETCC	0, L2, 6
	SFPNOP
	SFPABS	L1, L0, 1
	SFPNOP
	SFPNOP
	SFPENCC	3, 10
	SFPNOP
	SFPSTORE	L0, 0, 52
	ret
	.size	_Z9abs_setccv, .-_Z9abs_setccv
	.align	1
	.globl	_Z18rename_move_case2av
	.type	_Z18rename_move_case2av, @function
_Z18rename_move_case2av:
	SFPLOADI	L1, 0, 16256
	SFPLOADI	L0, 0, 16384
	SFPLOAD	L2, 1, 0
	SFPSETCC	0, L2, 6
	SFPNOP
	SFPMOV	L1, L0, 0
	SFPNOP
	SFPNOP
	SFPENCC	3, 10
	SFPNOP
	SFPSTORE	L0, 0, 0
	ret
	.size	_Z18rename_move_case2av, .-_Z18rename_move_case2av
	.align	1
	.globl	_Z16copy_move_case2bv
	.type	_Z16copy_move_case2bv, @function
_Z16copy_move_case2bv:
	SFPLOADI	L1, 0, 16256
	SFPLOADI	L0, 0, 16384
	SFPLOAD	L2, 1, 0
	SFPSETCC	0, L2, 6
	SFPNOP
	SFPMOV	L1, L0, 0
	SFPNOP
	SFPNOP
	SFPENCC	3, 10
	SFPNOP
	SFPSTORE	L1, 0, 0
	SFPSTORE	L0, 0, 0
	ret
	.size	_Z16copy_move_case2bv, .-_Z16copy_move_case2bv
	.align	1
	.globl	_Z15copy_move_case3v
	.type	_Z15copy_move_case3v, @function
_Z15copy_move_case3v:
	SFPLOADI	L0, 0, 16256
	SFPLOADI	L1, 0, 16384
	SFPLOAD	L2, 1, 0
	SFPSETCC	0, L2, 6
	SFPNOP
	SFPMOV	L0, L1, 0
	SFPNOP
	SFPNOP
	SFPENCC	3, 10
	SFPNOP
	SFPLOADI	L2, 0, 16448
	SFPADD	L10, L0, L2, L0, 0
	SFPNOP
	SFPNOP
	SFPSTORE	L0, 0, 0
	SFPLOADI	L0, 0, 16512
	SFPADD	L10, L1, L0, L0, 0
	SFPNOP
	SFPNOP
	SFPSTORE	L0, 0, 0
	ret
	.size	_Z15copy_move_case3v, .-_Z15copy_move_case3v
	.align	1
	.globl	_Z19internal_move_case4v
	.type	_Z19internal_move_case4v, @function
_Z19internal_move_case4v:
	SFPLOADI	L0, 4, 10
	SFPLOADI	L2, 4, 20
	SFPLOAD	L1, 1, 0
	SFPSETCC	0, L1, 6
	SFPNOP
	SFPMOV	L2, L1, 0
	SFPNOP
	SFPNOP
	SFPIADD	0, L0, L1, 6
	SFPNOP
	SFPNOP
	SFPMOV	L1, L0, 0
	SFPNOP
	SFPNOP
	SFPENCC	3, 10
	SFPNOP
	SFPSTORE	L0, 2, 0
	SFPSTORE	L2, 2, 0
	ret
	.size	_Z19internal_move_case4v, .-_Z19internal_move_case4v
	.align	1
	.globl	_Z19internal_move_case5v
	.type	_Z19internal_move_case5v, @function
_Z19internal_move_case5v:
	SFPLOADI	L3, 4, 10
	SFPLOADI	L2, 4, 20
	SFPLOADI	L0, 4, 30
	SFPLOAD	L1, 1, 0
	SFPSETCC	0, L1, 6
	SFPNOP
	SFPMOV	L2, L1, 0
	SFPNOP
	SFPNOP
	SFPIADD	0, L3, L1, 6
	SFPNOP
	SFPNOP
	SFPMOV	L1, L0, 0
	SFPNOP
	SFPNOP
	SFPENCC	3, 10
	SFPNOP
	SFPSTORE	L3, 2, 0
	SFPSTORE	L2, 2, 0
	SFPSTORE	L0, 2, 0
	ret
	.size	_Z19internal_move_case5v, .-_Z19internal_move_case5v
	.align	1
	.globl	_Z4bb_1v
	.type	_Z4bb_1v, @function
_Z4bb_1v:
	addi	sp,sp,-16
	sw	ra,12(sp)
	sw	s0,8(sp)
	SFPLOADI	L0, 4, 1
	SFPLOADI	L1, 4, 2
	li	s0,0
	j	.L13
.L17:
	SFPIADD	-3, L0, L2, 9
	SFPNOP
	SFPNOP
	SFPNOP
	call	rand
	bne	a0,zero,.L15
	SFPIADD	4, L1, L0, 5
	SFPNOP
	SFPNOP
	SFPCOMPC
	SFPNOP
	SFPPUSHC
	SFPIADD	-5, L0, L2, 9
	SFPNOP
	SFPNOP
	SFPNOP
	call	rand
	bne	a0,zero,.L16
	SFPIADD	6, L1, L0, 5
	SFPNOP
	SFPNOP
	SFPPOPC
	SFPNOP
.L15:
	SFPENCC	3, 10
	SFPNOP
	addi	s0,s0,1
.L13:
	call	rand
	bgt	a0,s0,.L17
	SFPSTORE	L0, 2, 0
	lw	ra,12(sp)
	lw	s0,8(sp)
	addi	sp,sp,16
	jr	ra
.L16:
	SFPPOPC
	SFPNOP
	SFPENCC	3, 10
	SFPNOP
	SFPSTORE	L0, 2, 0
	lw	ra,12(sp)
	lw	s0,8(sp)
	addi	sp,sp,16
	jr	ra
	.size	_Z4bb_1v, .-_Z4bb_1v
	.align	1
	.globl	_Z4bb_2v
	.type	_Z4bb_2v, @function
_Z4bb_2v:
	addi	sp,sp,-16
	sw	ra,12(sp)
	sw	s0,8(sp)
	sw	s1,4(sp)
	SFPLOADI	L0, 4, 1
	SFPLOADI	L1, 4, 2
	li	s0,0
	li	s1,1
.L23:
	call	rand
	bgt	a0,s0,.L25
	SFPSTORE	L0, 2, 0
	lw	ra,12(sp)
	lw	s0,8(sp)
	lw	s1,4(sp)
	addi	sp,sp,16
	jr	ra
.L25:
	beq	s0,zero,.L20
	bne	s0,s1,.L22
	SFPIADD	-4, L0, L0, 9
	SFPNOP
	SFPNOP
	SFPNOP
.L22:
	SFPIADD	-5, L1, L0, 5
	SFPNOP
	SFPNOP
	SFPENCC	3, 10
	SFPNOP
	addi	s0,s0,1
	j	.L23
.L20:
	SFPIADD	-3, L1, L0, 5
	SFPNOP
	SFPNOP
	SFPIADD	-5, L1, L0, 5
	SFPNOP
	SFPNOP
	SFPENCC	3, 10
	SFPNOP
	addi	s0,s0,1
	j	.L23
	.size	_Z4bb_2v, .-_Z4bb_2v
	.align	1
	.globl	_Z4bb_3v
	.type	_Z4bb_3v, @function
_Z4bb_3v:
	addi	sp,sp,-16
	sw	ra,12(sp)
	sw	s0,8(sp)
	sw	s1,4(sp)
	SFPLOADI	L0, 4, 1
	SFPLOADI	L1, 4, 2
	li	s0,0
	li	s1,2
	call	rand
	ble	a0,s0,.L27
.L35:
	call	rand
	beq	a0,zero,.L27
	SFPIADD	-3, L0, L2, 9
	SFPNOP
	SFPNOP
	SFPNOP
	call	rand
	bgt	a0,s1,.L28
	SFPIADD	-4, L1, L0, 5
	SFPNOP
	SFPNOP
	SFPCOMPC
	SFPNOP
	SFPIADD	-5, L0, L2, 9
	SFPNOP
	SFPNOP
	SFPNOP
	SFPIADD	-6, L1, L0, 5
	SFPNOP
	SFPNOP
	SFPENCC	3, 10
	SFPNOP
	addi	s0,s0,1
.L36:
	call	rand
	bgt	a0,s0,.L35
.L27:
	SFPSTORE	L0, 2, 0
	lw	ra,12(sp)
	lw	s0,8(sp)
	lw	s1,4(sp)
	addi	sp,sp,16
	jr	ra
.L28:
	SFPIADD	-6, L1, L0, 5
	SFPNOP
	SFPNOP
	SFPENCC	3, 10
	SFPNOP
	addi	s0,s0,1
	j	.L36
	.size	_Z4bb_3v, .-_Z4bb_3v
	.align	1
	.globl	_Z4bb_4v
	.type	_Z4bb_4v, @function
_Z4bb_4v:
	addi	sp,sp,-16
	sw	ra,12(sp)
	sw	s0,8(sp)
	SFPLOADI	L0, 4, 2
	SFPIADD	-1, L0, L1, 9
	SFPNOP
	SFPNOP
	SFPNOP
	call	rand
	li	a5,5
	beq	a0,a5,.L41
	call	rand
	li	s0,6
	beq	a0,s0,.L40
	call	rand
	beq	a0,s0,.L40
.L41:
.L39:
	SFPIADD	-2, L0, L0, 9
	SFPNOP
	SFPNOP
	SFPNOP
	SFPLOADI	L0, 4, 5
	SFPENCC	3, 10
	SFPNOP
	SFPSTORE	L0, 2, 0
	lw	ra,12(sp)
	lw	s0,8(sp)
	addi	sp,sp,16
	jr	ra
.L40:
	SFPIADD	-3, L0, L1, 9
	SFPNOP
	SFPNOP
	SFPNOP
	SFPIADD	-4, L0, L0, 9
	SFPNOP
	SFPNOP
	SFPNOP
	SFPLOADI	L0, 4, 5
	SFPENCC	3, 10
	SFPNOP
	SFPSTORE	L0, 2, 0
	lw	ra,12(sp)
	lw	s0,8(sp)
	addi	sp,sp,16
	jr	ra
	.size	_Z4bb_4v, .-_Z4bb_4v
	.align	1
	.globl	_Z11bb_not_livev
	.type	_Z11bb_not_livev, @function
_Z11bb_not_livev:
	addi	sp,sp,-16
	sw	ra,12(sp)
	sw	s0,8(sp)
	SFPLOADI	L0, 4, 1
	SFPLOADI	L0, 4, 2
	li	s0,0
	j	.L45
.L47:
	SFPIADD	-3, L0, L1, 5
	SFPNOP
	SFPNOP
	addi	s0,s0,1
.L45:
	call	rand
	bgt	a0,s0,.L47
	SFPIADD	-4, L0, L0, 5
	SFPNOP
	SFPNOP
	SFPSTORE	L0, 2, 0
	lw	ra,12(sp)
	lw	s0,8(sp)
	addi	sp,sp,16
	jr	ra
	.size	_Z11bb_not_livev, .-_Z11bb_not_livev
	.align	1
	.globl	_Z14popc_unrollingv
	.type	_Z14popc_unrollingv, @function
_Z14popc_unrollingv:
	SFPLOADI	L0, 4, 1
	SFPLOADI	L1, 4, 1
	SFPIADD	-2, L0, L2, 9
	SFPNOP
	SFPNOP
	SFPNOP
	SFPIADD	-1, L1, L0, 5
	SFPNOP
	SFPNOP
	SFPCOMPC
	SFPNOP
	SFPPUSHC
	SFPIADD	-3, L0, L2, 9
	SFPNOP
	SFPNOP
	SFPNOP
	SFPIADD	-1, L1, L0, 5
	SFPNOP
	SFPNOP
	SFPCOMPC
	SFPNOP
	SFPPUSHC
	SFPIADD	-4, L0, L2, 9
	SFPNOP
	SFPNOP
	SFPNOP
	SFPIADD	-1, L1, L0, 5
	SFPNOP
	SFPNOP
	SFPCOMPC
	SFPNOP
	SFPIADD	-5, L0, L2, 9
	SFPNOP
	SFPNOP
	SFPNOP
	SFPIADD	-1, L1, L0, 5
	SFPNOP
	SFPNOP
	SFPPOPC
	SFPNOP
	SFPPOPC
	SFPNOP
	SFPENCC	3, 10
	SFPNOP
	SFPIADD	-6, L0, L2, 9
	SFPNOP
	SFPNOP
	SFPNOP
	SFPIADD	-1, L1, L0, 5
	SFPNOP
	SFPNOP
	SFPCOMPC
	SFPNOP
	SFPPUSHC
	SFPIADD	-7, L0, L2, 9
	SFPNOP
	SFPNOP
	SFPNOP
	SFPIADD	-1, L1, L0, 5
	SFPNOP
	SFPNOP
	SFPCOMPC
	SFPNOP
	SFPIADD	-8, L0, L2, 9
	SFPNOP
	SFPNOP
	SFPNOP
	SFPIADD	-1, L1, L0, 5
	SFPNOP
	SFPNOP
	SFPPOPC
	SFPNOP
	SFPENCC	3, 10
	SFPNOP
	SFPSTORE	L0, 2, 0
	ret
	.size	_Z14popc_unrollingv, .-_Z14popc_unrollingv
	.align	1
	.globl	_Z18double_assign_evenv
	.type	_Z18double_assign_evenv, @function
_Z18double_assign_evenv:
	SFPLOADI	L0, 4, 1
	SFPLOADI	L1, 4, 2
	SFPIADD	-3, L0, L2, 5
	SFPNOP
	SFPNOP
	SFPSETCC	0, L2, 6
	SFPNOP
	SFPAND	L0, L1
	SFPNOP
	SFPNOP
	SFPMOV	L1, L0, 0
	SFPNOP
	SFPNOP
	SFPLOADI	L3, 4, 5
	SFPLOADI	L2, 4, 7
	SFPLOADI	L1, 4, 8
	SFPSTORE	L3, 2, 0
	SFPSTORE	L2, 2, 0
	SFPSTORE	L1, 2, 0
	SFPNOT	L0, L0
	SFPNOP
	SFPNOP
	SFPENCC	3, 10
	SFPNOP
	SFPSTORE	L0, 2, 0
	ret
	.size	_Z18double_assign_evenv, .-_Z18double_assign_evenv
	.align	1
	.globl	_Z10generationv
	.type	_Z10generationv, @function
_Z10generationv:
	SFPLOADI	L0, 4, 1
	SFPIADD	-2, L0, L1, 5
	SFPNOP
	SFPNOP
	SFPSETCC	0, L1, 2
	SFPNOP
	SFPIADD	-3, L0, L1, 5
	SFPNOP
	SFPNOP
	SFPSETCC	0, L1, 2
	SFPNOP
	SFPCOMPC
	SFPNOP
	SFPLOADI	L2, 4, 4
	SFPENCC	3, 10
	SFPNOP
	SFPIADD	-5, L0, L1, 5
	SFPNOP
	SFPNOP
	SFPSETCC	0, L1, 6
	SFPNOP
	SFPLOADI	L2, 4, -6
	SFPENCC	3, 10
	SFPNOP
	SFPIADD	-7, L0, L1, 5
	SFPNOP
	SFPNOP
	SFPSETCC	0, L1, 6
	SFPNOP
	SFPLOADI	L1, 4, 8
	SFPENCC	3, 10
	SFPNOP
	SFPIADD	-9, L0, L3, 5
	SFPNOP
	SFPNOP
	SFPSETCC	0, L3, 6
	SFPNOP
	SFPLOADI	L1, 4, 10
	SFPENCC	3, 10
	SFPNOP
	SFPIADD	-11, L0, L3, 5
	SFPNOP
	SFPNOP
	SFPSETCC	0, L3, 2
	SFPNOP
	SFPIADD	-12, L0, L3, 5
	SFPNOP
	SFPNOP
	SFPSETCC	0, L3, 2
	SFPNOP
	SFPCOMPC
	SFPNOP
	SFPSTORE	L2, 2, 56
	SFPENCC	3, 10
	SFPNOP
	SFPIADD	-13, L0, L0, 5
	SFPNOP
	SFPNOP
	SFPSETCC	0, L0, 6
	SFPNOP
	SFPSTORE	L1, 2, 56
	SFPENCC	3, 10
	SFPNOP
	ret
	.size	_Z10generationv, .-_Z10generationv
	.align	1
	.globl	_Z21prop_thru_pseudo_livev
	.type	_Z21prop_thru_pseudo_livev, @function
_Z21prop_thru_pseudo_livev:
	SFPLOADI	L0, 4, 1
	SFPLOADI	L1, 4, 2
	SFPIADD	-4, L0, L2, 5
	SFPNOP
	SFPNOP
	SFPSETCC	0, L2, 6
	SFPNOP
	SFPMOV	L1, L2, 0
	SFPNOP
	SFPNOP
	SFPAND	L0, L2
	SFPNOP
	SFPNOP
	SFPMOV	L2, L0, 0
	SFPNOP
	SFPNOP
	SFPMOV	L1, L2, 0
	SFPNOP
	SFPNOP
	SFPOR	L0, L2
	SFPNOP
	SFPNOP
	SFPMOV	L2, L0, 0
	SFPNOP
	SFPNOP
	SFPMOV	L0, L2, 0
	SFPNOP
	SFPNOP
	SFPIADD	0, L1, L2, 4
	SFPNOP
	SFPNOP
	SFPMOV	L2, L0, 0
	SFPNOP
	SFPNOP
	SFPMOV	L0, L2, 0
	SFPNOP
	SFPNOP
	SFPAND	L1, L2
	SFPNOP
	SFPNOP
	SFPMOV	L2, L0, 0
	SFPNOP
	SFPNOP
	SFPMOV	L0, L2, 0
	SFPNOP
	SFPNOP
	SFPOR	L1, L2
	SFPNOP
	SFPNOP
	SFPMOV	L2, L0, 0
	SFPNOP
	SFPNOP
	SFPIADD	0, L0, L1, 4
	SFPNOP
	SFPNOP
	SFPMOV	L1, L0, 0
	SFPNOP
	SFPNOP
	SFPENCC	3, 10
	SFPNOP
	SFPSTORE	L0, 2, 0
	ret
	.size	_Z21prop_thru_pseudo_livev, .-_Z21prop_thru_pseudo_livev
	.align	1
	.globl	_Z18do_not_fold_assignv
	.type	_Z18do_not_fold_assignv, @function
_Z18do_not_fold_assignv:
	SFPLOADI	L0, 4, 1
	SFPLOADI	L1, 4, 2
	SFPSETCC	0, L0, 0
	SFPNOP
	SFPMOV	L1, L0, 0
	SFPNOP
	SFPNOP
	SFPENCC	3, 10
	SFPNOP
	SFPSTORE	L0, 2, 0
	ret
	.size	_Z18do_not_fold_assignv, .-_Z18do_not_fold_assignv
	.section	.text.startup,"ax",@progbits
	.align	1
	.globl	main
	.type	main, @function
main:
	addi	sp,sp,-16
	sw	ra,12(sp)
	call	_Z9abs_setccv
	SFPLOADI	L1, 0, 16256
	SFPLOADI	L0, 0, 16384
	SFPLOAD	L2, 1, 0
	SFPSETCC	0, L2, 6
	SFPNOP
	SFPMOV	L1, L0, 0
	SFPNOP
	SFPNOP
	SFPENCC	3, 10
	SFPNOP
	SFPSTORE	L0, 0, 0
	call	_Z16copy_move_case2bv
	call	_Z15copy_move_case3v
	call	_Z19internal_move_case4v
	call	_Z19internal_move_case5v
	call	_Z4bb_1v
	call	_Z4bb_2v
	call	_Z4bb_3v
	call	_Z4bb_4v
	call	_Z11bb_not_livev
	call	_Z14popc_unrollingv
	call	_Z18double_assign_evenv
	call	_Z10generationv
	call	_Z21prop_thru_pseudo_livev
	SFPLOADI	L0, 4, 1
	SFPLOADI	L1, 4, 2
	SFPSETCC	0, L0, 0
	SFPNOP
	SFPMOV	L1, L0, 0
	SFPNOP
	SFPNOP
	SFPENCC	3, 10
	SFPNOP
	SFPSTORE	L0, 2, 0
	lw	ra,12(sp)
	li	a0,0
	addi	sp,sp,16
	jr	ra
	.size	main, .-main
	.globl	_ZN7ckernel13instrn_bufferE
	.section	.sbss,"aw",@nobits
	.align	2
	.type	_ZN7ckernel13instrn_bufferE, @object
	.size	_ZN7ckernel13instrn_bufferE, 4
_ZN7ckernel13instrn_bufferE:
	.zero	4
	.ident	"GCC: (GNU) 10.2.0"
