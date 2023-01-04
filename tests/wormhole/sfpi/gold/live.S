	.file	"live.cc"
	.option nopic
	.attribute arch, "rv32i2p0_m2p0_w2p0"
	.attribute unaligned_access, 0
	.attribute stack_align, 16
	.text
	.align	1
	.globl	_Z9abs_setccv
	.type	_Z9abs_setccv, @function
_Z9abs_setccv:
	SFPLOADI	L1, 49568, 0
	SFPLOADI	L0, 49648, 0
	SFPLOAD	L2, 0, 0, 3
	SFPSETCC	L2, 0, 6
	SFPABS	L0, L1, 1
	SFPENCC	3, 10
	SFPSTORE	26, L0, 0, 3
	ret
	.size	_Z9abs_setccv, .-_Z9abs_setccv
	.align	1
	.globl	_Z18rename_move_case2av
	.type	_Z18rename_move_case2av, @function
_Z18rename_move_case2av:
	SFPLOADI	L1, 16256, 0
	SFPLOADI	L0, 16384, 0
	SFPLOAD	L2, 0, 0, 3
	SFPSETCC	L2, 0, 6
	SFPMOV	L0, L1, 0
	SFPENCC	3, 10
	SFPSTORE	0, L0, 0, 3
	ret
	.size	_Z18rename_move_case2av, .-_Z18rename_move_case2av
	.align	1
	.globl	_Z16copy_move_case2bv
	.type	_Z16copy_move_case2bv, @function
_Z16copy_move_case2bv:
	SFPLOADI	L1, 16256, 0
	SFPLOADI	L0, 16384, 0
	SFPLOAD	L2, 0, 0, 3
	SFPSETCC	L2, 0, 6
	SFPMOV	L0, L1, 0
	SFPENCC	3, 10
	SFPSTORE	0, L1, 0, 3
	SFPSTORE	0, L0, 0, 3
	ret
	.size	_Z16copy_move_case2bv, .-_Z16copy_move_case2bv
	.align	1
	.globl	_Z15copy_move_case3v
	.type	_Z15copy_move_case3v, @function
_Z15copy_move_case3v:
	SFPLOADI	L1, 16256, 0
	SFPLOADI	L0, 16384, 0
	SFPLOAD	L2, 0, 0, 3
	SFPSETCC	L2, 0, 6
	SFPMOV	L0, L1, 0
	SFPENCC	3, 10
	SFPADDI	L1, 16448, 0
	SFPNOP
	SFPSTORE	0, L1, 0, 3
	SFPADDI	L0, 16512, 0
	SFPNOP
	SFPSTORE	0, L0, 0, 3
	ret
	.size	_Z15copy_move_case3v, .-_Z15copy_move_case3v
	.align	1
	.globl	_Z19internal_move_case4v
	.type	_Z19internal_move_case4v, @function
_Z19internal_move_case4v:
	SFPLOADI	L0, 10, 4
	SFPLOADI	L2, 20, 4
	SFPLOAD	L1, 0, 0, 3
	SFPSETCC	L1, 0, 6
	SFPMOV	L1, L2, 0
	SFPIADD	L1, L0, 0, 6
	SFPMOV	L0, L1, 0
	SFPENCC	3, 10
	SFPSTORE	0, L0, 0, 3
	SFPSTORE	0, L2, 0, 3
	ret
	.size	_Z19internal_move_case4v, .-_Z19internal_move_case4v
	.align	1
	.globl	_Z19internal_move_case5v
	.type	_Z19internal_move_case5v, @function
_Z19internal_move_case5v:
	SFPLOADI	L3, 10, 4
	SFPLOADI	L2, 20, 4
	SFPLOADI	L0, 30, 4
	SFPLOAD	L1, 0, 0, 3
	SFPSETCC	L1, 0, 6
	SFPMOV	L1, L2, 0
	SFPIADD	L1, L3, 0, 6
	SFPMOV	L0, L1, 0
	SFPENCC	3, 10
	SFPSTORE	0, L3, 0, 3
	SFPSTORE	0, L2, 0, 3
	SFPSTORE	0, L0, 0, 3
	ret
	.size	_Z19internal_move_case5v, .-_Z19internal_move_case5v
	.align	1
	.globl	_Z4bb_1v
	.type	_Z4bb_1v, @function
_Z4bb_1v:
	addi	sp,sp,-16
	sw	ra,12(sp)
	sw	s0,8(sp)
	SFPLOADI	L0, 1, 4
	SFPLOADI	L1, 2, 4
	li	s0,0
	j	.L13
.L17:
	SFPIADD	L2, L0, -3, 9
	call	rand
	bne	a0,zero,.L15
	SFPIADD	L0, L1, 4, 5
	SFPCOMPC
	SFPPUSHC	0
	SFPIADD	L2, L0, -5, 9
	call	rand
	bne	a0,zero,.L16
	SFPIADD	L0, L1, 6, 5
	SFPPOPC	0
.L15:
	SFPENCC	3, 10
	addi	s0,s0,1
.L13:
	call	rand
	bgt	a0,s0,.L17
	SFPSTORE	0, L0, 0, 3
	lw	ra,12(sp)
	lw	s0,8(sp)
	addi	sp,sp,16
	jr	ra
.L16:
	SFPPOPC	0
	SFPENCC	3, 10
	SFPSTORE	0, L0, 0, 3
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
	SFPLOADI	L0, 1, 4
	SFPLOADI	L1, 2, 4
	li	s0,0
	li	s1,1
.L23:
	call	rand
	bgt	a0,s0,.L25
	SFPSTORE	0, L0, 0, 3
	lw	ra,12(sp)
	lw	s0,8(sp)
	lw	s1,4(sp)
	addi	sp,sp,16
	jr	ra
.L25:
	beq	s0,zero,.L20
	bne	s0,s1,.L22
	SFPIADD	L0, L0, -4, 9
.L22:
	SFPIADD	L0, L1, -5, 5
	SFPENCC	3, 10
	addi	s0,s0,1
	j	.L23
.L20:
	SFPIADD	L0, L1, -3, 5
	SFPIADD	L0, L1, -5, 5
	SFPENCC	3, 10
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
	SFPLOADI	L0, 1, 4
	SFPLOADI	L1, 2, 4
	li	s0,0
	li	s1,2
	call	rand
	ble	a0,s0,.L27
.L35:
	call	rand
	beq	a0,zero,.L27
	SFPIADD	L2, L0, -3, 9
	call	rand
	bgt	a0,s1,.L28
	SFPIADD	L0, L1, -4, 5
	SFPCOMPC
	SFPIADD	L2, L0, -5, 9
	SFPIADD	L0, L1, -6, 5
	SFPENCC	3, 10
	addi	s0,s0,1
.L36:
	call	rand
	bgt	a0,s0,.L35
.L27:
	SFPSTORE	0, L0, 0, 3
	lw	ra,12(sp)
	lw	s0,8(sp)
	lw	s1,4(sp)
	addi	sp,sp,16
	jr	ra
.L28:
	SFPIADD	L0, L1, -6, 5
	SFPENCC	3, 10
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
	SFPLOADI	L0, 2, 4
	SFPIADD	L1, L0, -1, 9
	call	rand
	li	a5,5
	beq	a0,a5,.L41
	li	s0,6
	call	rand
	beq	a0,s0,.L40
	call	rand
	beq	a0,s0,.L40
.L41:
.L39:
	SFPIADD	L0, L0, -2, 9
	SFPLOADI	L0, 5, 4
	SFPENCC	3, 10
	SFPSTORE	0, L0, 0, 3
	lw	ra,12(sp)
	lw	s0,8(sp)
	addi	sp,sp,16
	jr	ra
.L40:
	SFPIADD	L1, L0, -3, 9
	SFPIADD	L0, L0, -4, 9
	SFPLOADI	L0, 5, 4
	SFPENCC	3, 10
	SFPSTORE	0, L0, 0, 3
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
	SFPLOADI	L0, 1, 4
	SFPLOADI	L0, 2, 4
	li	s0,0
	j	.L45
.L47:
	SFPIADD	L1, L0, -3, 5
	addi	s0,s0,1
.L45:
	call	rand
	bgt	a0,s0,.L47
	SFPIADD	L0, L0, -4, 5
	SFPSTORE	0, L0, 0, 3
	lw	ra,12(sp)
	lw	s0,8(sp)
	addi	sp,sp,16
	jr	ra
	.size	_Z11bb_not_livev, .-_Z11bb_not_livev
	.align	1
	.globl	_Z14popc_unrollingv
	.type	_Z14popc_unrollingv, @function
_Z14popc_unrollingv:
	SFPLOADI	L0, 1, 4
	SFPLOADI	L1, 1, 4
	SFPIADD	L2, L0, -2, 9
	TTREPLAY	0, 3, 1, 1
	SFPIADD	L0, L1, -1, 5
	SFPCOMPC
	SFPPUSHC	0
	SFPIADD	L2, L0, -3, 9
	TTREPLAY	0, 3, 0, 0
	SFPIADD	L2, L0, -4, 9
	TTREPLAY	0, 2, 0, 0
	SFPIADD	L2, L0, -5, 9
	SFPIADD	L0, L1, -1, 5
	SFPPOPC	0
	SFPPOPC	0
	SFPENCC	3, 10
	SFPIADD	L2, L0, -6, 9
	TTREPLAY	0, 3, 0, 0
	SFPIADD	L2, L0, -7, 9
	TTREPLAY	0, 2, 0, 0
	SFPIADD	L2, L0, -8, 9
	SFPIADD	L0, L1, -1, 5
	SFPPOPC	0
	SFPENCC	3, 10
	SFPSTORE	0, L0, 0, 3
	ret
	.size	_Z14popc_unrollingv, .-_Z14popc_unrollingv
	.align	1
	.globl	_Z18double_assign_evenv
	.type	_Z18double_assign_evenv, @function
_Z18double_assign_evenv:
	SFPLOADI	L0, 1, 4
	SFPLOADI	L1, 2, 4
	SFPIADD	L2, L0, -3, 5
	SFPSETCC	L2, 0, 6
	SFPAND	L1, L0
	SFPMOV	L0, L1, 0
	SFPLOADI	L3, 5, 4
	SFPLOADI	L2, 7, 4
	SFPLOADI	L1, 8, 4
	SFPSTORE	0, L3, 0, 3
	SFPSTORE	0, L2, 0, 3
	SFPSTORE	0, L1, 0, 3
	SFPNOT	L0, L0
	SFPENCC	3, 10
	SFPSTORE	0, L0, 0, 3
	ret
	.size	_Z18double_assign_evenv, .-_Z18double_assign_evenv
	.align	1
	.globl	_Z10generationv
	.type	_Z10generationv, @function
_Z10generationv:
	SFPLOADI	L0, 1, 4
	SFPIADD	L1, L0, -2, 5
	SFPSETCC	L1, 0, 2
	SFPIADD	L1, L0, -3, 5
	SFPSETCC	L1, 0, 2
	SFPCOMPC
	SFPLOADI	L2, 4, 4
	SFPENCC	3, 10
	SFPIADD	L1, L0, -5, 5
	SFPSETCC	L1, 0, 6
	SFPLOADI	L2, -6, 4
	SFPENCC	3, 10
	SFPIADD	L1, L0, -7, 5
	SFPSETCC	L1, 0, 6
	SFPLOADI	L1, 8, 4
	SFPENCC	3, 10
	SFPIADD	L3, L0, -9, 5
	SFPSETCC	L3, 0, 6
	SFPLOADI	L1, 10, 4
	SFPENCC	3, 10
	SFPIADD	L3, L0, -11, 5
	SFPSETCC	L3, 0, 2
	SFPIADD	L3, L0, -12, 5
	SFPSETCC	L3, 0, 2
	SFPCOMPC
	SFPSTORE	28, L2, 0, 3
	SFPENCC	3, 10
	SFPIADD	L0, L0, -13, 5
	SFPSETCC	L0, 0, 6
	SFPSTORE	28, L1, 0, 3
	SFPENCC	3, 10
	ret
	.size	_Z10generationv, .-_Z10generationv
	.align	1
	.globl	_Z21prop_thru_pseudo_livev
	.type	_Z21prop_thru_pseudo_livev, @function
_Z21prop_thru_pseudo_livev:
	SFPLOADI	L0, 1, 4
	SFPLOADI	L1, 2, 4
	SFPIADD	L2, L0, -4, 5
	SFPSETCC	L2, 0, 6
	SFPMOV	L2, L1, 0
	SFPAND	L2, L0
	SFPMOV	L0, L2, 0
	SFPMOV	L2, L1, 0
	SFPOR	L2, L0
	TTREPLAY	0, 2, 1, 1
	SFPMOV	L0, L2, 0
	SFPMOV	L2, L0, 0
	SFPIADD	L2, L1, 0, 4
	TTREPLAY	0, 2, 0, 0
	SFPAND	L2, L1
	TTREPLAY	0, 2, 0, 0
	SFPOR	L2, L1
	SFPMOV	L0, L2, 0
	SFPIADD	L1, L0, 0, 4
	SFPMOV	L0, L1, 0
	SFPENCC	3, 10
	SFPSTORE	0, L0, 0, 3
	ret
	.size	_Z21prop_thru_pseudo_livev, .-_Z21prop_thru_pseudo_livev
	.align	1
	.globl	_Z18do_not_fold_assignv
	.type	_Z18do_not_fold_assignv, @function
_Z18do_not_fold_assignv:
	SFPLOADI	L0, 1, 4
	SFPLOADI	L1, 2, 4
	SFPSETCC	L0, 0, 0
	SFPMOV	L0, L1, 0
	SFPENCC	3, 10
	SFPSTORE	0, L0, 0, 3
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
	call	_Z18rename_move_case2av
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
	SFPLOADI	L0, 1, 4
	SFPLOADI	L1, 2, 4
	SFPSETCC	L0, 0, 0
	SFPMOV	L0, L1, 0
	SFPENCC	3, 10
	SFPSTORE	0, L0, 0, 3
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
