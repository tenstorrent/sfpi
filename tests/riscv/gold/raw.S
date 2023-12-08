	.file	"raw.cc"
	.option nopic
	.attribute arch, "rv32i2p0_y2p0"
	.attribute unaligned_access, 0
	.attribute stack_align, 16
	.text
	.align	1
	.globl	_Z8basic_qiPVh
	.type	_Z8basic_qiPVh, @function
_Z8basic_qiPVh:
	li	a5,5
	sb	a5,4(a0)
	lbu	zero,4(a0)
	ret
	.size	_Z8basic_qiPVh, .-_Z8basic_qiPVh
	.align	1
	.globl	_Z8basic_hiPVt
	.type	_Z8basic_hiPVt, @function
_Z8basic_hiPVt:
	li	a5,5
	sh	a5,8(a0)
	lhu	zero,8(a0)
	ret
	.size	_Z8basic_hiPVt, .-_Z8basic_hiPVt
	.align	1
	.globl	_Z8basic_siPVj
	.type	_Z8basic_siPVj, @function
_Z8basic_siPVj:
	li	a5,5
	sw	a5,16(a0)
	ret
	.size	_Z8basic_siPVj, .-_Z8basic_siPVj
	.align	1
	.globl	_Z11basic_l1_qiU11rvtt_l1_ptrPVh
	.type	_Z11basic_l1_qiU11rvtt_l1_ptrPVh, @function
_Z11basic_l1_qiU11rvtt_l1_ptrPVh:
	li	a5,5
	sb	a5,0(a0)
	lbu	a5,0(a0)
	mv	a5,a5	# GS L1 war
	ret
	.size	_Z11basic_l1_qiU11rvtt_l1_ptrPVh, .-_Z11basic_l1_qiU11rvtt_l1_ptrPVh
	.align	1
	.globl	_Z11basic_l1_hiU11rvtt_l1_ptrPVt
	.type	_Z11basic_l1_hiU11rvtt_l1_ptrPVt, @function
_Z11basic_l1_hiU11rvtt_l1_ptrPVt:
	li	a5,5
	sh	a5,0(a0)
	lhu	a5,0(a0)
	mv	a5,a5	# GS L1 war
	ret
	.size	_Z11basic_l1_hiU11rvtt_l1_ptrPVt, .-_Z11basic_l1_hiU11rvtt_l1_ptrPVt
	.align	1
	.globl	_Z11basic_l1_siU11rvtt_l1_ptrPVj
	.type	_Z11basic_l1_siU11rvtt_l1_ptrPVj, @function
_Z11basic_l1_siU11rvtt_l1_ptrPVj:
	li	a5,5
	sw	a5,0(a0)
	ret
	.size	_Z11basic_l1_siU11rvtt_l1_ptrPVj, .-_Z11basic_l1_siU11rvtt_l1_ptrPVj
	.align	1
	.globl	_Z12basic_reg_qiU12rvtt_reg_ptrPVh
	.type	_Z12basic_reg_qiU12rvtt_reg_ptrPVh, @function
_Z12basic_reg_qiU12rvtt_reg_ptrPVh:
	li	a5,5
	sb	a5,0(a0)
	ret
	.size	_Z12basic_reg_qiU12rvtt_reg_ptrPVh, .-_Z12basic_reg_qiU12rvtt_reg_ptrPVh
	.align	1
	.globl	_Z12basic_reg_hiU12rvtt_reg_ptrPVt
	.type	_Z12basic_reg_hiU12rvtt_reg_ptrPVt, @function
_Z12basic_reg_hiU12rvtt_reg_ptrPVt:
	li	a5,5
	sh	a5,0(a0)
	ret
	.size	_Z12basic_reg_hiU12rvtt_reg_ptrPVt, .-_Z12basic_reg_hiU12rvtt_reg_ptrPVt
	.align	1
	.globl	_Z12basic_reg_siU12rvtt_reg_ptrPVj
	.type	_Z12basic_reg_siU12rvtt_reg_ptrPVj, @function
_Z12basic_reg_siU12rvtt_reg_ptrPVj:
	li	a5,5
	sw	a5,0(a0)
	ret
	.size	_Z12basic_reg_siU12rvtt_reg_ptrPVj, .-_Z12basic_reg_siU12rvtt_reg_ptrPVj
	.align	1
	.globl	_Z15multiple_storesPVtPVj
	.type	_Z15multiple_storesPVtPVj, @function
_Z15multiple_storesPVtPVj:
	li	a5,5
	sh	a5,0(a0)
	sh	a5,2(a0)
	sw	a5,0(a1)
	sh	a5,4(a0)
	sh	a5,6(a0)
	sw	a5,8(a1)
	sh	a5,8(a0)
	lhu	zero,8(a0)
	ret
	.size	_Z15multiple_storesPVtPVj, .-_Z15multiple_storesPVtPVj
	.align	1
	.globl	_Z26multiple_stores_with_loadsPVtPVj
	.type	_Z26multiple_stores_with_loadsPVtPVj, @function
_Z26multiple_stores_with_loadsPVtPVj:
	li	a5,5
	sh	a5,0(a0)
	sh	a5,2(a0)
	sw	a5,0(a1)
	lhu	zero,2(a0)
	lw	a5,0(a1)
	sh	a5,4(a0)
	sh	a5,6(a0)
	lhu	zero,6(a0)
	lhu	a5,2(a0)
	sw	a5,8(a1)
	sh	a5,8(a0)
	lhu	zero,8(a0)
	ret
	.size	_Z26multiple_stores_with_loadsPVtPVj, .-_Z26multiple_stores_with_loadsPVtPVj
	.align	1
	.globl	_Z17change_src_reg_r0PVt
	.type	_Z17change_src_reg_r0PVt, @function
_Z17change_src_reg_r0PVt:
	li	a5,5
	sh	a5,0(a0)
	li	a5,6
	sh	a5,0(a0)
	lhu	zero,0(a0)
	ret
	.size	_Z17change_src_reg_r0PVt, .-_Z17change_src_reg_r0PVt
	.align	1
	.globl	_Z17change_src_reg_rnU11rvtt_l1_ptrPVt
	.type	_Z17change_src_reg_rnU11rvtt_l1_ptrPVt, @function
_Z17change_src_reg_rnU11rvtt_l1_ptrPVt:
	li	a5,5
	sh	a5,0(a0)
	lhu	a5,0(a0)
	li	a5,6
	sh	a5,0(a0)
	lhu	a5,0(a0)
	mv	a5,a5	# GS L1 war
	ret
	.size	_Z17change_src_reg_rnU11rvtt_l1_ptrPVt, .-_Z17change_src_reg_rnU11rvtt_l1_ptrPVt
	.align	1
	.globl	_Z17change_dst_reg_rnPPVti
	.type	_Z17change_dst_reg_rnPPVti, @function
_Z17change_dst_reg_rnPPVti:
	lw	a5,0(a0)
	li	a4,5
	slli	a1,a1,1
	sh	a4,0(a5)
	lhu	zero,0(a5)
	add	a5,a5,a1
	sw	a5,0(a0)
	sh	a4,0(a5)
	lhu	zero,0(a5)
	ret
	.size	_Z17change_dst_reg_rnPPVti, .-_Z17change_dst_reg_rnPPVti
	.align	1
	.globl	_Z12conditional1PVtb
	.type	_Z12conditional1PVtb, @function
_Z12conditional1PVtb:
	li	a5,5
	sh	a5,0(a0)
	lhu	zero,0(a0)
	beq	a1,zero,.L17
	li	a5,6
	sh	a5,0(a0)
	lhu	zero,0(a0)
	ret
.L17:
	li	a5,7
	sh	a5,0(a0)
	lhu	zero,0(a0)
	ret
	.size	_Z12conditional1PVtb, .-_Z12conditional1PVtb
	.align	1
	.globl	_Z6stupidPVjt
	.type	_Z6stupidPVjt, @function
_Z6stupidPVjt:
	addi	a1,a1,16
	slli	a1,a1,16
	srli	a1,a1,16
	sw	a1,0(a0)
	ret
	.size	_Z6stupidPVjt, .-_Z6stupidPVjt
	.align	1
	.globl	_Z18l1_hll_interactioniU11rvtt_l1_ptrPtS_
	.type	_Z18l1_hll_interactioniU11rvtt_l1_ptrPtS_, @function
_Z18l1_hll_interactioniU11rvtt_l1_ptrPtS_:
	ble	a0,zero,.L20
	or	a4,a2,a1
	addi	a3,a0,-1
	andi	a4,a4,3
	sltiu	a3,a3,12
	addi	a5,a2,2
	seqz	a4,a4
	xori	a3,a3,1
	sub	a5,a1,a5
	and	a4,a4,a3
	snez	a5,a5
	and	a5,a5,a4
	beq	a5,zero,.L22
	srli	a6,a0,1
	slli	a6,a6,2
	mv	a4,a2
	mv	a5,a1
	add	a6,a6,a2
.L23:
	lw	a3,0(a4)
	addi	a4,a4,4
	addi	a5,a5,4
	sw	a3,-4(a5)
	bne	a4,a6,.L23
	andi	a5,a0,-2
	beq	a0,a5,.L20
	slli	a5,a5,1
	add	a2,a2,a5
	lhu	a4,0(a2)
	add	a5,a1,a5
	sh	a4,0(a5)
	lhu	a4,0(a5)
	mv	a4,a4	# GS L1 war
	ret
.L22:
	slli	a0,a0,1
	mv	a5,a1
	add	a0,a2,a0
.L26:
	lhu	a4,0(a2)
	addi	a2,a2,2
	addi	a5,a5,2
	sh	a4,-2(a5)
	lhu	a4,-2(a5)
	bne	a2,a0,.L26
.L20:
	mv	a4,a4	# GS L1 war
	ret
	.size	_Z18l1_hll_interactioniU11rvtt_l1_ptrPtS_, .-_Z18l1_hll_interactioniU11rvtt_l1_ptrPtS_
	.align	1
	.globl	_Z7get_ptrv
	.type	_Z7get_ptrv, @function
_Z7get_ptrv:
	li	a0,4
	ret
	.size	_Z7get_ptrv, .-_Z7get_ptrv
	.align	1
	.globl	_Z19l1_hll_interaction2iPt
	.type	_Z19l1_hll_interaction2iPt, @function
_Z19l1_hll_interaction2iPt:
	addi	sp,sp,-16
	sw	s1,4(sp)
	mv	s1,a0
	sw	s0,8(sp)
	sw	ra,12(sp)
	mv	s0,a1
	call	_Z7get_ptrv
	ble	s1,zero,.L29
	addi	a4,s1,-1
	addi	a3,s0,2
	sltiu	a4,a4,12
	sub	a3,a0,a3
	or	a5,s0,a0
	xori	a4,a4,1
	snez	a3,a3
	andi	a5,a5,3
	and	a4,a4,a3
	seqz	a5,a5
	and	a5,a5,a4
	beq	a5,zero,.L31
	srli	a2,s1,1
	slli	a2,a2,2
	mv	a4,s0
	mv	a5,a0
	add	a2,a2,s0
.L32:
	lw	a3,0(a4)
	addi	a4,a4,4
	addi	a5,a5,4
	sw	a3,-4(a5)
	bne	a4,a2,.L32
	andi	a5,s1,-2
	beq	s1,a5,.L29
	slli	a5,a5,1
	add	s0,s0,a5
	lhu	a4,0(s0)
	add	a5,a0,a5
	sh	a4,0(a5)
	lhu	a4,0(a5)
.L29:
	mv	a4,a4	# GS L1 war
	lw	ra,12(sp)
	lw	s0,8(sp)
	lw	s1,4(sp)
	addi	sp,sp,16
	jr	ra
.L31:
	slli	s1,s1,1
	mv	a5,a0
	add	s1,s0,s1
.L35:
	lhu	a4,0(s0)
	addi	s0,s0,2
	addi	a5,a5,2
	sh	a4,-2(a5)
	lhu	a4,-2(a5)
	bne	s0,s1,.L35
	mv	a4,a4	# GS L1 war
	lw	ra,12(sp)
	lw	s0,8(sp)
	lw	s1,4(sp)
	addi	sp,sp,16
	jr	ra
	.size	_Z19l1_hll_interaction2iPt, .-_Z19l1_hll_interaction2iPt
	.align	1
	.globl	_Z9do_littlePVh
	.type	_Z9do_littlePVh, @function
_Z9do_littlePVh:
	lbu	a5,0(a0)
	ret
	.size	_Z9do_littlePVh, .-_Z9do_littlePVh
	.align	1
	.globl	_Z8fn_call1PVhb
	.type	_Z8fn_call1PVhb, @function
_Z8fn_call1PVhb:
	addi	sp,sp,-16
	sw	s0,8(sp)
	sw	ra,12(sp)
	li	a4,5
	sb	a4,0(a0)
	mv	s0,a1
	lbu	zero,0(a0)
	call	_Z9do_littlePVh
	lw	ra,12(sp)
	snez	a0,s0
	lw	s0,8(sp)
	addi	a0,a0,4
	addi	sp,sp,16
	jr	ra
	.size	_Z8fn_call1PVhb, .-_Z8fn_call1PVhb
	.align	1
	.globl	_Z8fn_call2PVhb
	.type	_Z8fn_call2PVhb, @function
_Z8fn_call2PVhb:
	addi	sp,sp,-32
	li	a5,5
	sw	s1,20(sp)
	sb	a5,0(a0)
	mv	s1,a1
	sw	s0,24(sp)
	sw	ra,28(sp)
	mv	s0,a0
	lbu	zero,0(a0)
	call	_Z9do_littlePVh
	beq	s1,zero,.L44
	lbu	a5,0(s0)
	sb	a5,14(sp)
	lbu	zero,14(sp)
.L44:
	lbu	a5,1(s0)
	sb	a5,15(sp)
	lbu	zero,15(sp)
	.size	_Z8fn_call2PVhb, .-_Z8fn_call2PVhb
	.section	.text.startup,"ax",@progbits
	.align	1
	.globl	main
	.type	main, @function
main:
	li	a0,0
	ret
	.size	main, .-main
	.ident	"GCC: (GNU) 10.2.0"
