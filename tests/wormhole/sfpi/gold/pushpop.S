	.file	"pushpop.cc"
	.option nopic
	.attribute arch, "rv32i2p0_m2p0_w2p0"
	.attribute unaligned_access, 0
	.attribute stack_align, 16
	.text
	.align	1
	.globl	_Z10simple_yesv
	.type	_Z10simple_yesv, @function
_Z10simple_yesv:
	SFPLOADI	L0, 4, 0
	SFPIADD	-1, L0, L1, 9
	SFPIADD	-2, L0, L1, 9
	SFPLOADI	L0, 4, 3
	SFPENCC	3, 10
	ret
	.size	_Z10simple_yesv, .-_Z10simple_yesv
	.align	1
	.globl	_Z10double_yesv
	.type	_Z10double_yesv, @function
_Z10double_yesv:
	SFPLOADI	L0, 4, 0
	SFPIADD	-1, L0, L1, 9
	SFPIADD	-2, L0, L1, 9
	SFPLOADI	L0, 4, 3
	SFPIADD	-4, L0, L1, 9
	SFPLOADI	L0, 4, 5
	SFPENCC	3, 10
	ret
	.size	_Z10double_yesv, .-_Z10double_yesv
	.align	1
	.globl	_Z10both_noyesv
	.type	_Z10both_noyesv, @function
_Z10both_noyesv:
	SFPLOADI	L0, 4, 0
	SFPIADD	-1, L0, L1, 9
	SFPPUSHC	0
	SFPIADD	-2, L0, L1, 9
	SFPLOADI	L0, 4, 3
	SFPPOPC	0
	SFPIADD	-4, L0, L1, 9
	SFPLOADI	L0, 4, 5
	SFPENCC	3, 10
	ret
	.size	_Z10both_noyesv, .-_Z10both_noyesv
	.align	1
	.globl	_Z15consecutive_yesv
	.type	_Z15consecutive_yesv, @function
_Z15consecutive_yesv:
	SFPLOADI	L0, 4, 0
	SFPIADD	-1, L0, L1, 9
	SFPIADD	-2, L0, L1, 9
	SFPLOADI	L0, 4, 3
	SFPENCC	3, 10
	SFPIADD	-4, L0, L1, 9
	SFPIADD	-5, L0, L1, 9
	SFPLOADI	L0, 4, 5
	SFPENCC	3, 10
	ret
	.size	_Z15consecutive_yesv, .-_Z15consecutive_yesv
	.align	1
	.globl	_Z8else_yesv
	.type	_Z8else_yesv, @function
_Z8else_yesv:
	SFPLOADI	L0, 4, 0
	SFPIADD	-1, L0, L1, 9
	SFPLOADI	L0, 4, 2
	SFPCOMPC
	SFPIADD	-3, L0, L1, 9
	SFPLOADI	L0, 4, 4
	SFPENCC	3, 10
	SFPIADD	-5, L0, L1, 9
	SFPLOADI	L0, 4, 6
	SFPCOMPC
	SFPIADD	-7, L0, L1, 9
	SFPIADD	-8, L0, L1, 9
	SFPLOADI	L0, 4, 9
	SFPENCC	3, 10
	ret
	.size	_Z8else_yesv, .-_Z8else_yesv
	.align	1
	.globl	_Z9block_yesv
	.type	_Z9block_yesv, @function
_Z9block_yesv:
	SFPLOADI	L0, 4, 0
	SFPIADD	-1, L0, L1, 9
	SFPLOADI	L0, 4, 2
	SFPIADD	-3, L0, L1, 9
	SFPLOADI	L0, 4, 4
	SFPENCC	3, 10
	ret
	.size	_Z9block_yesv, .-_Z9block_yesv
	.align	1
	.globl	_Z16no_yes_no_yes_nov
	.type	_Z16no_yes_no_yes_nov, @function
_Z16no_yes_no_yes_nov:
	SFPLOADI	L0, 4, 0
	SFPIADD	-1, L0, L1, 9
	SFPIADD	-2, L0, L1, 9
	SFPPUSHC	0
	SFPIADD	-3, L0, L1, 9
	SFPIADD	-4, L0, L1, 9
	SFPPUSHC	0
	SFPIADD	-5, L0, L1, 9
	SFPLOADI	L0, 4, 6
	SFPPOPC	0
	SFPLOADI	L0, 4, 7
	SFPPOPC	0
	SFPLOADI	L0, 4, 8
	SFPENCC	3, 10
	ret
	.size	_Z16no_yes_no_yes_nov, .-_Z16no_yes_no_yes_nov
	.align	1
	.globl	_Z6bb_yesi
	.type	_Z6bb_yesi, @function
_Z6bb_yesi:
	SFPLOADI	L0, 4, 0
	SFPIADD	-1, L0, L1, 9
	bgt	a0,zero,.L15
	SFPENCC	3, 10
	ret
.L15:
	li	a5,0
.L13:
	SFPPUSHC	0
	SFPIADD	-2, L0, L1, 9
	andi	a4,a5,1
	beq	a4,zero,.L16
	SFPPOPC	0
	addi	a5,a5,1
	bne	a0,a5,.L13
	SFPENCC	3, 10
	ret
.L16:
	SFPIADD	-3, L0, L1, 9
	SFPLOADI	L0, 4, 4
	SFPPOPC	0
	addi	a5,a5,1
	bne	a5,a0,.L13
	SFPENCC	3, 10
	ret
	.size	_Z6bb_yesi, .-_Z6bb_yesi
	.align	1
	.globl	_Z9func_callv
	.type	_Z9func_callv, @function
_Z9func_callv:
	addi	sp,sp,-16
	sw	ra,12(sp)
	SFPLOADI	L0, 4, 0
	SFPIADD	-1, L0, L1, 9
	call	rand
	SFPIADD	-2, L0, L1, 9
	SFPLOADI	L0, 4, 3
	SFPENCC	3, 10
	lw	ra,12(sp)
	addi	sp,sp,16
	jr	ra
	.size	_Z9func_callv, .-_Z9func_callv
	.align	1
	.globl	_Z8compc_nov
	.type	_Z8compc_nov, @function
_Z8compc_nov:
	SFPLOADI	L0, 4, 0
	SFPIADD	-1, L0, L1, 9
	SFPPUSHC	0
	SFPIADD	-2, L0, L1, 9
	SFPLOADI	L0, 4, 3
	SFPCOMPC
	SFPLOADI	L0, 4, 4
	SFPPOPC	0
	SFPENCC	3, 10
	ret
	.size	_Z8compc_nov, .-_Z8compc_nov
	.align	1
	.globl	_Z10cascade_nov
	.type	_Z10cascade_nov, @function
_Z10cascade_nov:
	SFPLOADI	L0, 4, 0
	SFPIADD	-1, L0, L1, 9
	SFPLOADI	L0, 4, 2
	SFPCOMPC
	SFPPUSHC	0
	SFPIADD	-3, L0, L1, 9
	SFPLOADI	L0, 4, 4
	SFPCOMPC
	SFPPUSHC	0
	SFPIADD	-5, L0, L1, 9
	SFPLOADI	L0, 4, 6
	SFPCOMPC
	SFPPUSHC	0
	SFPIADD	-7, L0, L1, 9
	SFPLOADI	L0, 4, 7
	SFPCOMPC
	SFPIADD	-8, L0, L1, 9
	SFPLOADI	L0, 4, 8
	SFPPOPC	0
	SFPPOPC	0
	SFPPOPC	0
	SFPENCC	3, 10
	ret
	.size	_Z10cascade_nov, .-_Z10cascade_nov
	.align	1
	.globl	_Z12intervene_nov
	.type	_Z12intervene_nov, @function
_Z12intervene_nov:
	SFPLOADI	L0, 4, 0
	SFPIADD	-1, L0, L1, 9
	SFPPUSHC	0
	SFPIADD	-2, L0, L1, 9
	SFPLOADI	L0, 4, 3
	SFPPOPC	0
	SFPLOADI	L0, 4, 4
	SFPENCC	3, 10
	ret
	.size	_Z12intervene_nov, .-_Z12intervene_nov
	.align	1
	.globl	_Z18compc_intervene_nov
	.type	_Z18compc_intervene_nov, @function
_Z18compc_intervene_nov:
	SFPLOADI	L0, 4, 0
	SFPIADD	-1, L0, L1, 9
	SFPPUSHC	0
	SFPIADD	-2, L0, L1, 9
	SFPLOADI	L0, 4, 3
	SFPCOMPC
	SFPLOADI	L0, 4, 4
	SFPPOPC	0
	SFPLOADI	L0, 4, 5
	SFPENCC	3, 10
	ret
	.size	_Z18compc_intervene_nov, .-_Z18compc_intervene_nov
	.align	1
	.globl	_Z10fn_call_nov
	.type	_Z10fn_call_nov, @function
_Z10fn_call_nov:
	addi	sp,sp,-16
	sw	ra,12(sp)
	SFPLOADI	L0, 4, 0
	SFPIADD	-1, L0, L1, 9
	SFPPUSHC	0
	SFPIADD	-2, L0, L1, 9
	SFPLOADI	L0, 4, 3
	SFPPOPC	0
	call	rand
	SFPENCC	3, 10
	SFPSTORE	L0, 0, 3, 0
	lw	ra,12(sp)
	addi	sp,sp,16
	jr	ra
	.size	_Z10fn_call_nov, .-_Z10fn_call_nov
	.align	1
	.globl	_Z24control_flow_v_if_switchii
	.type	_Z24control_flow_v_if_switchii, @function
_Z24control_flow_v_if_switchii:
	SFPLOADI	L0, 4, 1
	li	a5,20
	beq	a0,a5,.L26
	li	a5,30
	beq	a0,a5,.L27
	li	a5,10
	beq	a0,a5,.L30
	SFPLOADI	L0, 4, 8
	SFPSTORE	L0, 0, 3, 0
	ret
.L26:
	SFPIADD	-4, L0, L1, 1
	SFPLOADI	L0, 4, 5
	SFPENCC	3, 10
	SFPLOADI	L0, 4, 8
	SFPSTORE	L0, 0, 3, 0
	ret
.L30:
	SFPIADD	-2, L0, L1, 1
	SFPLOADI	L0, 4, 3
	SFPENCC	3, 10
	SFPLOADI	L0, 4, 8
	SFPSTORE	L0, 0, 3, 0
	ret
.L27:
	SFPIADD	-8, L0, L1, 1
	SFPLOADI	L0, 4, 9
	li	a5,40
	bgt	a1,a5,.L29
	SFPCOMPC
	SFPIADD	-10, L0, L1, 1
	SFPLOADI	L0, 4, 11
	SFPENCC	3, 10
	SFPLOADI	L0, 4, 8
	SFPSTORE	L0, 0, 3, 0
	ret
.L29:
	SFPENCC	3, 10
	SFPLOADI	L0, 4, 8
	SFPSTORE	L0, 0, 3, 0
	ret
	.size	_Z24control_flow_v_if_switchii, .-_Z24control_flow_v_if_switchii
	.align	1
	.globl	_Z25control_flow_pif_continuei
	.type	_Z25control_flow_pif_continuei, @function
_Z25control_flow_pif_continuei:
	addi	sp,sp,-16
	sw	ra,12(sp)
	sw	s0,8(sp)
	sw	s1,4(sp)
	SFPLOADI	L0, 4, 1
	ble	a0,zero,.L31
	mv	s1,a0
	li	s0,0
.L35:
	SFPIADD	-2, L0, L1, 1
	SFPLOADI	L0, 4, 2
	SFPPUSHC	0
	SFPIADD	-1, L0, L1, 1
	SFPLOADI	L0, 4, 4
	call	rand
	beq	a0,s1,.L37
	SFPCOMPC
	SFPLOADI	L0, 4, 5
	SFPPOPC	0
	SFPLOADI	L0, 4, 7
	SFPENCC	3, 10
	SFPLOADI	L0, 4, 8
	addi	s0,s0,1
	bne	s1,s0,.L35
.L31:
	lw	ra,12(sp)
	lw	s0,8(sp)
	lw	s1,4(sp)
	addi	sp,sp,16
	jr	ra
.L37:
	SFPPOPC	0
	SFPENCC	3, 10
	addi	s0,s0,1
	bne	s0,s1,.L35
	lw	ra,12(sp)
	lw	s0,8(sp)
	lw	s1,4(sp)
	addi	sp,sp,16
	jr	ra
	.size	_Z25control_flow_pif_continuei, .-_Z25control_flow_pif_continuei
	.align	1
	.globl	_Z21control_flow_pif_gotoi
	.type	_Z21control_flow_pif_gotoi, @function
_Z21control_flow_pif_gotoi:
	addi	sp,sp,-16
	sw	ra,12(sp)
	SFPLOADI	L0, 4, 1
	SFPIADD	-2, L0, L1, 1
	SFPLOADI	L0, 4, 2
	SFPPUSHC	0
	SFPIADD	-1, L0, L1, 1
	SFPLOADI	L0, 4, 4
	call	rand
	li	a5,1
	beq	a0,a5,.L42
	SFPCOMPC
	SFPLOADI	L0, 4, 5
	SFPPOPC	0
	SFPLOADI	L0, 4, 7
	SFPENCC	3, 10
	lw	ra,12(sp)
	addi	sp,sp,16
	jr	ra
.L40:
.L42:
	SFPPOPC	0
	SFPENCC	3, 10
	lw	ra,12(sp)
	addi	sp,sp,16
	jr	ra
	.size	_Z21control_flow_pif_gotoi, .-_Z21control_flow_pif_gotoi
	.globl	_ZN7ckernel13instrn_bufferE
	.section	.sbss,"aw",@nobits
	.align	2
	.type	_ZN7ckernel13instrn_bufferE, @object
	.size	_ZN7ckernel13instrn_bufferE, 4
_ZN7ckernel13instrn_bufferE:
	.zero	4
	.ident	"GCC: (GNU) 10.2.0"
