	.file	"pushpop.cc"
	.option nopic
	.attribute arch, "rv32i2p0_m2p0_w"
	.attribute unaligned_access, 0
	.attribute stack_align, 16
	.text
	.align	2
	.globl	_Z10simple_yesv
	.type	_Z10simple_yesv, @function
_Z10simple_yesv:
	SFPLOADI	L0, 0, 4
	SFPIADD	L1, L0, -1, 9
	SFPIADD	L1, L0, -2, 9
	SFPLOADI	L0, 3, 4
	SFPENCC	3, 10
	ret
	.size	_Z10simple_yesv, .-_Z10simple_yesv
	.align	2
	.globl	_Z10double_yesv
	.type	_Z10double_yesv, @function
_Z10double_yesv:
	SFPLOADI	L0, 0, 4
	SFPIADD	L1, L0, -1, 9
	SFPIADD	L1, L0, -2, 9
	SFPLOADI	L0, 3, 4
	SFPIADD	L1, L0, -4, 9
	SFPLOADI	L0, 5, 4
	SFPENCC	3, 10
	ret
	.size	_Z10double_yesv, .-_Z10double_yesv
	.align	2
	.globl	_Z10both_noyesv
	.type	_Z10both_noyesv, @function
_Z10both_noyesv:
	SFPLOADI	L0, 0, 4
	SFPIADD	L1, L0, -1, 9
	SFPPUSHC	0
	SFPIADD	L1, L0, -2, 9
	SFPLOADI	L0, 3, 4
	SFPPOPC	0
	SFPIADD	L1, L0, -4, 9
	SFPLOADI	L0, 5, 4
	SFPENCC	3, 10
	ret
	.size	_Z10both_noyesv, .-_Z10both_noyesv
	.align	2
	.globl	_Z15consecutive_yesv
	.type	_Z15consecutive_yesv, @function
_Z15consecutive_yesv:
	SFPLOADI	L0, 0, 4
	SFPIADD	L1, L0, -1, 9
	SFPIADD	L1, L0, -2, 9
	SFPLOADI	L0, 3, 4
	SFPENCC	3, 10
	SFPIADD	L1, L0, -4, 9
	SFPIADD	L1, L0, -5, 9
	SFPLOADI	L0, 5, 4
	SFPENCC	3, 10
	ret
	.size	_Z15consecutive_yesv, .-_Z15consecutive_yesv
	.align	2
	.globl	_Z8else_yesv
	.type	_Z8else_yesv, @function
_Z8else_yesv:
	SFPLOADI	L0, 0, 4
	SFPIADD	L1, L0, -1, 9
	SFPLOADI	L0, 2, 4
	SFPCOMPC
	SFPIADD	L1, L0, -3, 9
	SFPLOADI	L0, 4, 4
	SFPENCC	3, 10
	SFPIADD	L1, L0, -5, 9
	SFPLOADI	L0, 6, 4
	SFPCOMPC
	SFPIADD	L1, L0, -7, 9
	SFPIADD	L1, L0, -8, 9
	SFPLOADI	L0, 9, 4
	SFPENCC	3, 10
	ret
	.size	_Z8else_yesv, .-_Z8else_yesv
	.align	2
	.globl	_Z9block_yesv
	.type	_Z9block_yesv, @function
_Z9block_yesv:
	SFPLOADI	L0, 0, 4
	SFPIADD	L1, L0, -1, 9
	SFPLOADI	L0, 2, 4
	SFPIADD	L1, L0, -3, 9
	SFPLOADI	L0, 4, 4
	SFPENCC	3, 10
	ret
	.size	_Z9block_yesv, .-_Z9block_yesv
	.align	2
	.globl	_Z16no_yes_no_yes_nov
	.type	_Z16no_yes_no_yes_nov, @function
_Z16no_yes_no_yes_nov:
	SFPLOADI	L0, 0, 4
	SFPIADD	L1, L0, -1, 9
	SFPIADD	L1, L0, -2, 9
	SFPPUSHC	0
	SFPIADD	L1, L0, -3, 9
	SFPIADD	L1, L0, -4, 9
	SFPPUSHC	0
	SFPIADD	L1, L0, -5, 9
	SFPLOADI	L0, 6, 4
	SFPPOPC	0
	SFPLOADI	L0, 7, 4
	SFPPOPC	0
	SFPLOADI	L0, 8, 4
	SFPENCC	3, 10
	ret
	.size	_Z16no_yes_no_yes_nov, .-_Z16no_yes_no_yes_nov
	.align	2
	.globl	_Z6bb_yesi
	.type	_Z6bb_yesi, @function
_Z6bb_yesi:
	SFPLOADI	L0, 0, 4
	SFPIADD	L1, L0, -1, 9
	ble	a0,zero,.L10
	li	a5,0
.L13:
	SFPPUSHC	0
	SFPIADD	L1, L0, -2, 9
	andi	a4,a5,1
	beq	a4,zero,.L14
	SFPPOPC	0
	addi	a5,a5,1
	bne	a0,a5,.L13
.L10:
	SFPENCC	3, 10
	ret
.L14:
	SFPIADD	L1, L0, -3, 9
	SFPLOADI	L0, 4, 4
	SFPPOPC	0
	addi	a5,a5,1
	bne	a0,a5,.L13
	SFPENCC	3, 10
	ret
	.size	_Z6bb_yesi, .-_Z6bb_yesi
	.align	2
	.globl	_Z9func_callv
	.type	_Z9func_callv, @function
_Z9func_callv:
	addi	sp,sp,-16
	sw	ra,12(sp)
	SFPLOADI	L0, 0, 4
	SFPIADD	L1, L0, -1, 9
	call	rand
	SFPIADD	L1, L0, -2, 9
	SFPLOADI	L0, 3, 4
	SFPENCC	3, 10
	lw	ra,12(sp)
	addi	sp,sp,16
	jr	ra
	.size	_Z9func_callv, .-_Z9func_callv
	.align	2
	.globl	_Z8compc_nov
	.type	_Z8compc_nov, @function
_Z8compc_nov:
	SFPLOADI	L0, 0, 4
	SFPIADD	L1, L0, -1, 9
	SFPPUSHC	0
	SFPIADD	L1, L0, -2, 9
	SFPLOADI	L0, 3, 4
	SFPCOMPC
	SFPLOADI	L0, 4, 4
	SFPPOPC	0
	SFPENCC	3, 10
	ret
	.size	_Z8compc_nov, .-_Z8compc_nov
	.align	2
	.globl	_Z10cascade_nov
	.type	_Z10cascade_nov, @function
_Z10cascade_nov:
	SFPLOADI	L0, 0, 4
	SFPIADD	L1, L0, -1, 9
	SFPLOADI	L0, 2, 4
	TTREPLAY	0, 2, 1, 1
	SFPCOMPC
	SFPPUSHC	0
	SFPIADD	L1, L0, -3, 9
	SFPLOADI	L0, 4, 4
	TTREPLAY	0, 2, 0, 0
	SFPIADD	L1, L0, -5, 9
	SFPLOADI	L0, 6, 4
	TTREPLAY	0, 2, 0, 0
	SFPIADD	L1, L0, -7, 9
	SFPLOADI	L0, 7, 4
	SFPCOMPC
	SFPIADD	L1, L0, -8, 9
	SFPLOADI	L0, 8, 4
	SFPPOPC	0
	SFPPOPC	0
	SFPPOPC	0
	SFPENCC	3, 10
	ret
	.size	_Z10cascade_nov, .-_Z10cascade_nov
	.align	2
	.globl	_Z12intervene_nov
	.type	_Z12intervene_nov, @function
_Z12intervene_nov:
	SFPLOADI	L0, 0, 4
	SFPIADD	L1, L0, -1, 9
	SFPPUSHC	0
	SFPIADD	L1, L0, -2, 9
	SFPLOADI	L0, 3, 4
	SFPPOPC	0
	SFPLOADI	L0, 4, 4
	SFPENCC	3, 10
	ret
	.size	_Z12intervene_nov, .-_Z12intervene_nov
	.align	2
	.globl	_Z18compc_intervene_nov
	.type	_Z18compc_intervene_nov, @function
_Z18compc_intervene_nov:
	SFPLOADI	L0, 0, 4
	SFPIADD	L1, L0, -1, 9
	SFPPUSHC	0
	SFPIADD	L1, L0, -2, 9
	SFPLOADI	L0, 3, 4
	SFPCOMPC
	SFPLOADI	L0, 4, 4
	SFPPOPC	0
	SFPLOADI	L0, 5, 4
	SFPENCC	3, 10
	ret
	.size	_Z18compc_intervene_nov, .-_Z18compc_intervene_nov
	.align	2
	.globl	_Z10fn_call_nov
	.type	_Z10fn_call_nov, @function
_Z10fn_call_nov:
	addi	sp,sp,-16
	sw	ra,12(sp)
	SFPLOADI	L0, 0, 4
	SFPIADD	L1, L0, -1, 9
	SFPPUSHC	0
	SFPIADD	L1, L0, -2, 9
	SFPLOADI	L0, 3, 4
	SFPPOPC	0
	call	rand
	SFPENCC	3, 10
	SFPSTORE	0, L0, 12, 3
	lw	ra,12(sp)
	addi	sp,sp,16
	jr	ra
	.size	_Z10fn_call_nov, .-_Z10fn_call_nov
	.align	2
	.globl	_Z24control_flow_v_if_switchii
	.type	_Z24control_flow_v_if_switchii, @function
_Z24control_flow_v_if_switchii:
	SFPLOADI	L0, 1, 4
	li	a5,20
	beq	a0,a5,.L24
	li	a5,30
	beq	a0,a5,.L25
	li	a5,10
	beq	a0,a5,.L28
.L26:
	SFPLOADI	L0, 8, 4
	SFPSTORE	0, L0, 12, 3
	ret
.L24:
	SFPIADD	L1, L0, -4, 1
	SFPLOADI	L0, 5, 4
	SFPENCC	3, 10
	SFPLOADI	L0, 8, 4
	SFPSTORE	0, L0, 12, 3
	ret
.L28:
	SFPIADD	L1, L0, -2, 1
	SFPLOADI	L0, 3, 4
	SFPENCC	3, 10
	SFPLOADI	L0, 8, 4
	SFPSTORE	0, L0, 12, 3
	ret
.L25:
	SFPIADD	L1, L0, -8, 1
	SFPLOADI	L0, 9, 4
	li	a5,40
	ble	a1,a5,.L29
	SFPENCC	3, 10
	SFPLOADI	L0, 8, 4
	SFPSTORE	0, L0, 12, 3
	ret
.L29:
	SFPCOMPC
	SFPIADD	L1, L0, -10, 1
	SFPLOADI	L0, 11, 4
	SFPENCC	3, 10
	j	.L26
	.size	_Z24control_flow_v_if_switchii, .-_Z24control_flow_v_if_switchii
	.align	2
	.globl	_Z25control_flow_pif_continuei
	.type	_Z25control_flow_pif_continuei, @function
_Z25control_flow_pif_continuei:
	addi	sp,sp,-16
	sw	ra,12(sp)
	sw	s0,8(sp)
	sw	s1,4(sp)
	SFPLOADI	L0, 1, 4
	ble	a0,zero,.L30
	mv	s1,a0
	li	s0,0
.L34:
	SFPIADD	L1, L0, -2, 1
	SFPLOADI	L0, 2, 4
	SFPPUSHC	0
	SFPIADD	L1, L0, -1, 1
	SFPLOADI	L0, 4, 4
	call	rand
	beq	a0,s1,.L36
	SFPCOMPC
	SFPLOADI	L0, 5, 4
	SFPPOPC	0
	SFPLOADI	L0, 7, 4
	SFPENCC	3, 10
	SFPLOADI	L0, 8, 4
	addi	s0,s0,1
	bne	s1,s0,.L34
.L30:
	lw	ra,12(sp)
	lw	s0,8(sp)
	lw	s1,4(sp)
	addi	sp,sp,16
	jr	ra
.L36:
	SFPPOPC	0
	SFPENCC	3, 10
	addi	s0,s0,1
	bne	s0,s1,.L34
	lw	ra,12(sp)
	lw	s0,8(sp)
	lw	s1,4(sp)
	addi	sp,sp,16
	jr	ra
	.size	_Z25control_flow_pif_continuei, .-_Z25control_flow_pif_continuei
	.align	2
	.globl	_Z21control_flow_pif_gotoi
	.type	_Z21control_flow_pif_gotoi, @function
_Z21control_flow_pif_gotoi:
	addi	sp,sp,-16
	sw	ra,12(sp)
	SFPLOADI	L0, 1, 4
	SFPIADD	L1, L0, -2, 1
	SFPLOADI	L0, 2, 4
	SFPPUSHC	0
	SFPIADD	L1, L0, -1, 1
	SFPLOADI	L0, 4, 4
	call	rand
	li	a5,1
	beq	a0,a5,.L41
	SFPCOMPC
	SFPLOADI	L0, 5, 4
	SFPPOPC	0
	SFPLOADI	L0, 7, 4
	SFPENCC	3, 10
	lw	ra,12(sp)
	addi	sp,sp,16
	jr	ra
.L39:
.L41:
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
	.ident	"GCC: (g96bd106687d) 12.2.0"
