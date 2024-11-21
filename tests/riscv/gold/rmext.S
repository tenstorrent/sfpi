	.file	"rmext.cc"
	.option nopic
	.attribute arch, "rv32i2p0_xttgs1p0"
	.attribute unaligned_access, 0
	.attribute stack_align, 16
	.text
	.align	2
	.globl	_Z17load_baseline_uhiPt
	.type	_Z17load_baseline_uhiPt, @function
_Z17load_baseline_uhiPt:
	lhu	a0,0(a0)
	ret
	.size	_Z17load_baseline_uhiPt, .-_Z17load_baseline_uhiPt
	.align	2
	.globl	_Z17load_baseline_uqiPh
	.type	_Z17load_baseline_uqiPh, @function
_Z17load_baseline_uqiPh:
	lbu	a0,0(a0)
	ret
	.size	_Z17load_baseline_uqiPh, .-_Z17load_baseline_uqiPh
	.align	2
	.globl	_Z16load_baseline_hiPs
	.type	_Z16load_baseline_hiPs, @function
_Z16load_baseline_hiPs:
	lh	a0,0(a0)
	ret
	.size	_Z16load_baseline_hiPs, .-_Z16load_baseline_hiPs
	.align	2
	.globl	_Z17load_baseline_uqiPc
	.type	_Z17load_baseline_uqiPc, @function
_Z17load_baseline_uqiPc:
	lbu	a0,0(a0)
	ret
	.size	_Z17load_baseline_uqiPc, .-_Z17load_baseline_uqiPc
	.align	2
	.globl	_Z8load_uhiPVt
	.type	_Z8load_uhiPVt, @function
_Z8load_uhiPVt:
	lhu	a0,0(a0)
	ret
	.size	_Z8load_uhiPVt, .-_Z8load_uhiPVt
	.align	2
	.globl	_Z8load_uqiPVh
	.type	_Z8load_uqiPVh, @function
_Z8load_uqiPVh:
	lbu	a0,0(a0)
	ret
	.size	_Z8load_uqiPVh, .-_Z8load_uqiPVh
	.align	2
	.globl	_Z7load_hiPVs
	.type	_Z7load_hiPVs, @function
_Z7load_hiPVs:
	lhu	a0,0(a0)
	slli	a0,a0,16
	srai	a0,a0,16
	ret
	.size	_Z7load_hiPVs, .-_Z7load_hiPVs
	.align	2
	.globl	_Z7load_qiPVc
	.type	_Z7load_qiPVc, @function
_Z7load_qiPVc:
	lbu	a0,0(a0)
	ret
	.size	_Z7load_qiPVc, .-_Z7load_qiPVc
	.align	2
	.globl	_Z18store_baseline_uhiPt
	.type	_Z18store_baseline_uhiPt, @function
_Z18store_baseline_uhiPt:
	lhu	a5,0(a0)
	lui	a4,%hi(dstuhi)
	lw	a4,%lo(dstuhi)(a4)
	addi	a5,a5,10
	sh	a5,0(a4)
	lhu	zero,0(a4)
	ret
	.size	_Z18store_baseline_uhiPt, .-_Z18store_baseline_uhiPt
	.align	2
	.globl	_Z9store_uhiPVt
	.type	_Z9store_uhiPVt, @function
_Z9store_uhiPVt:
	lhu	a5,0(a0)
	lui	a4,%hi(dstuhi)
	lw	a4,%lo(dstuhi)(a4)
	addi	a5,a5,10
	sh	a5,0(a4)
	lhu	zero,0(a4)
	ret
	.size	_Z9store_uhiPVt, .-_Z9store_uhiPVt
	.align	2
	.globl	_Z9store_uqiPVh
	.type	_Z9store_uqiPVh, @function
_Z9store_uqiPVh:
	lbu	a5,0(a0)
	lui	a4,%hi(dstuqi)
	lw	a4,%lo(dstuqi)(a4)
	addi	a5,a5,10
	sb	a5,0(a4)
	lbu	zero,0(a4)
	ret
	.size	_Z9store_uqiPVh, .-_Z9store_uqiPVh
	.align	2
	.globl	_Z8keep_uqiPVh
	.type	_Z8keep_uqiPVh, @function
_Z8keep_uqiPVh:
	lbu	a5,0(a0)
	lui	a4,%hi(dstuhi)
	lw	a4,%lo(dstuhi)(a4)
	addi	a5,a5,10
	andi	a5,a5,0xff
	sh	a5,0(a4)
	lhu	zero,0(a4)
	ret
	.size	_Z8keep_uqiPVh, .-_Z8keep_uqiPVh
	.align	2
	.globl	_Z16double_store_uhiPVt
	.type	_Z16double_store_uhiPVt, @function
_Z16double_store_uhiPVt:
	lhu	a5,0(a0)
	lui	a4,%hi(dstuhi)
	lw	a4,%lo(dstuhi)(a4)
	addi	a5,a5,10
	sh	a5,0(a4)
	sh	a5,0(a4)
	lhu	zero,0(a4)
	ret
	.size	_Z16double_store_uhiPVt, .-_Z16double_store_uhiPVt
	.align	2
	.globl	_Z20double_store_uhi_retPVt
	.type	_Z20double_store_uhi_retPVt, @function
_Z20double_store_uhi_retPVt:
	lhu	a0,0(a0)
	lui	a5,%hi(dstuhi)
	addi	a0,a0,10
	lw	a5,%lo(dstuhi)(a5)
	slli	a0,a0,16
	srli	a0,a0,16
	sh	a0,0(a5)
	sh	a0,0(a5)
	lhu	zero,0(a5)
	ret
	.size	_Z20double_store_uhi_retPVt, .-_Z20double_store_uhi_retPVt
	.align	2
	.globl	_Z20double_store_uhi_usePVt
	.type	_Z20double_store_uhi_usePVt, @function
_Z20double_store_uhi_usePVt:
	lhu	a5,0(a0)
	lui	a4,%hi(dstuhi)
	lw	a4,%lo(dstuhi)(a4)
	addi	a3,a5,10
	sh	a3,0(a4)
	sh	a3,0(a4)
	addi	a5,a5,15
	sh	a5,0(a4)
	lhu	zero,0(a4)
	ret
	.size	_Z20double_store_uhi_usePVt, .-_Z20double_store_uhi_usePVt
	.align	2
	.globl	_Z21double_store_uhi_use2PVtt
	.type	_Z21double_store_uhi_use2PVtt, @function
_Z21double_store_uhi_use2PVtt:
	lhu	a5,0(a0)
	lui	a4,%hi(dstuhi)
	addi	a5,a5,10
	lw	a4,%lo(dstuhi)(a4)
	slli	a5,a5,16
	srli	a5,a5,16
	sh	a5,0(a4)
	sh	a5,0(a4)
	add	a5,a5,a1
	sh	a5,0(a4)
	lhu	zero,0(a4)
	ret
	.size	_Z21double_store_uhi_use2PVtt, .-_Z21double_store_uhi_use2PVtt
	.align	2
	.globl	_Z16double_store_uqiPVh
	.type	_Z16double_store_uqiPVh, @function
_Z16double_store_uqiPVh:
	lbu	a5,0(a0)
	lui	a4,%hi(dstuqi)
	lw	a3,%lo(dstuqi)(a4)
	addi	a5,a5,10
	sb	a5,0(a3)
	lbu	zero,0(a3)
	lw	a4,%lo(dstuqi)(a4)
	sb	a5,0(a4)
	lbu	zero,0(a4)
	ret
	.size	_Z16double_store_uqiPVh, .-_Z16double_store_uqiPVh
	.align	2
	.globl	_Z16triple_store_uhiPVt
	.type	_Z16triple_store_uhiPVt, @function
_Z16triple_store_uhiPVt:
	lhu	a5,0(a0)
	lui	a4,%hi(dstuhi)
	lw	a4,%lo(dstuhi)(a4)
	addi	a5,a5,10
	sh	a5,0(a4)
	sh	a5,0(a4)
	sh	a5,0(a4)
	lhu	zero,0(a4)
	ret
	.size	_Z16triple_store_uhiPVt, .-_Z16triple_store_uhiPVt
	.align	2
	.globl	_Z16triple_store_uqiPVh
	.type	_Z16triple_store_uqiPVh, @function
_Z16triple_store_uqiPVh:
	lbu	a5,0(a0)
	lui	a4,%hi(dstuqi)
	lw	a3,%lo(dstuqi)(a4)
	addi	a5,a5,10
	sb	a5,0(a3)
	lbu	zero,0(a3)
	lw	a3,%lo(dstuqi)(a4)
	sb	a5,0(a3)
	lbu	zero,0(a3)
	lw	a4,%lo(dstuqi)(a4)
	sb	a5,0(a4)
	lbu	zero,0(a4)
	ret
	.size	_Z16triple_store_uqiPVh, .-_Z16triple_store_uqiPVh
	.align	2
	.globl	_Z17replace_write_uhiPVt
	.type	_Z17replace_write_uhiPVt, @function
_Z17replace_write_uhiPVt:
	lhu	a5,0(a0)
	lui	a4,%hi(dstusi)
	lw	a4,%lo(dstusi)(a4)
	slli	a5,a5,16
	srli	a5,a5,16
	sw	a5,0(a4)
	ret
	.size	_Z17replace_write_uhiPVt, .-_Z17replace_write_uhiPVt
	.align	2
	.globl	_Z17replace_write_uqiPVh
	.type	_Z17replace_write_uqiPVh, @function
_Z17replace_write_uqiPVh:
	lbu	a5,0(a0)
	lui	a4,%hi(dstusi)
	lw	a4,%lo(dstusi)(a4)
	andi	a5,a5,0xff
	sw	a5,0(a4)
	ret
	.size	_Z17replace_write_uqiPVh, .-_Z17replace_write_uqiPVh
	.align	2
	.globl	_Z14keep_later_usePVt
	.type	_Z14keep_later_usePVt, @function
_Z14keep_later_usePVt:
	lhu	a0,0(a0)
	lui	a5,%hi(dstuhi)
	addi	a0,a0,10
	lw	a5,%lo(dstuhi)(a5)
	slli	a0,a0,16
	srli	a0,a0,16
	sh	a0,0(a5)
	lhu	zero,0(a5)
	ret
	.size	_Z14keep_later_usePVt, .-_Z14keep_later_usePVt
	.align	2
	.globl	_Z15keep_later_use2Pt
	.type	_Z15keep_later_use2Pt, @function
_Z15keep_later_use2Pt:
	lhu	a5,0(a0)
	lui	a4,%hi(dstuhi)
	lw	a4,%lo(dstuhi)(a4)
	addi	a0,a5,10
	slli	a0,a0,16
	sh	a5,0(a4)
	srli	a0,a0,16
	lhu	zero,0(a4)
	ret
	.size	_Z15keep_later_use2Pt, .-_Z15keep_later_use2Pt
	.align	2
	.globl	_Z15keep_later_use3PVt
	.type	_Z15keep_later_use3PVt, @function
_Z15keep_later_use3PVt:
	lhu	a0,0(a0)
	lui	a5,%hi(dstuhi)
	addi	a0,a0,10
	lw	a5,%lo(dstuhi)(a5)
	slli	a0,a0,16
	srli	a0,a0,16
	sh	a0,0(a5)
	sh	a0,0(a5)
	lhu	zero,0(a5)
	ret
	.size	_Z15keep_later_use3PVt, .-_Z15keep_later_use3PVt
	.align	2
	.globl	_Z7keep_bbPVti
	.type	_Z7keep_bbPVti, @function
_Z7keep_bbPVti:
	lhu	a0,0(a0)
	lui	a5,%hi(dstuhi)
	addi	a0,a0,10
	lw	a4,%lo(dstuhi)(a5)
	slli	a0,a0,16
	srli	a0,a0,16
	sh	a0,0(a4)
	lhu	zero,0(a4)
	ble	a1,zero,.L27
	li	a5,0
.L28:
	sh	zero,0(a4)
	addi	a5,a5,1
	bne	a1,a5,.L28
.L27:
	ret
	.size	_Z7keep_bbPVti, .-_Z7keep_bbPVti
	.align	2
	.globl	_Z15keep_word_writePVt
	.type	_Z15keep_word_writePVt, @function
_Z15keep_word_writePVt:
	lhu	a5,0(a0)
	lui	a4,%hi(dstusi)
	addi	a5,a5,10
	lw	a4,%lo(dstusi)(a4)
	slli	a5,a5,16
	srli	a5,a5,16
	sw	a5,0(a4)
	ret
	.size	_Z15keep_word_writePVt, .-_Z15keep_word_writePVt
	.align	2
	.globl	_Z15keep_word_writePVh
	.type	_Z15keep_word_writePVh, @function
_Z15keep_word_writePVh:
	lbu	a5,0(a0)
	lui	a4,%hi(dstusi)
	lw	a4,%lo(dstusi)(a4)
	addi	a5,a5,10
	andi	a5,a5,0xff
	sw	a5,0(a4)
	ret
	.size	_Z15keep_word_writePVh, .-_Z15keep_word_writePVh
	.align	2
	.globl	_Z17replace_contortedPVt
	.type	_Z17replace_contortedPVt, @function
_Z17replace_contortedPVt:
	lhu	a5,0(a0)
	addi	a5,a5,10
	sh	a5,0(a4)
	lhu	zero,0(a4)
	ret
	.size	_Z17replace_contortedPVt, .-_Z17replace_contortedPVt
	.align	2
	.globl	_Z14keep_word_loadPVj
	.type	_Z14keep_word_loadPVj, @function
_Z14keep_word_loadPVj:
	lw	a5,0(a0)
	lui	a4,%hi(dstuhi)
	lw	a4,%lo(dstuhi)(a4)
	addi	a5,a5,2
	sh	a5,0(a4)
	lhu	zero,0(a4)
	ret
	.size	_Z14keep_word_loadPVj, .-_Z14keep_word_loadPVj
	.align	2
	.globl	_Z23replace_word_load_storePVj
	.type	_Z23replace_word_load_storePVj, @function
_Z23replace_word_load_storePVj:
	lui	a5,%hi(dstuhi)
	lw	a4,0(a0)
	lw	a5,%lo(dstuhi)(a5)
	sh	a4,0(a5)
	lhu	zero,0(a5)
	ret
	.size	_Z23replace_word_load_storePVj, .-_Z23replace_word_load_storePVj
	.align	2
	.globl	_Z18keep_word_load_retPVj
	.type	_Z18keep_word_load_retPVj, @function
_Z18keep_word_load_retPVj:
	lw	a0,0(a0)
	lui	a5,%hi(dstuhi)
	lw	a5,%lo(dstuhi)(a5)
	slli	a0,a0,16
	srli	a0,a0,16
	sh	a0,0(a5)
	lhu	zero,0(a5)
	ret
	.size	_Z18keep_word_load_retPVj, .-_Z18keep_word_load_retPVj
	.align	2
	.globl	_Z8keep_retPVt
	.type	_Z8keep_retPVt, @function
_Z8keep_retPVt:
	lhu	a0,0(a0)
	addi	a0,a0,5
	slli	a0,a0,16
	srli	a0,a0,16
	ret
	.size	_Z8keep_retPVt, .-_Z8keep_retPVt
	.align	2
	.globl	_Z9keep_ret2PVt
	.type	_Z9keep_ret2PVt, @function
_Z9keep_ret2PVt:
	lhu	a5,0(a0)
	lhu	a3,0(a0)
	lhu	a4,0(a0)
	add	a0,a5,a3
	add	a0,a0,a4
	slli	a0,a0,16
	srli	a0,a0,16
	ret
	.size	_Z9keep_ret2PVt, .-_Z9keep_ret2PVt
	.align	2
	.globl	_Z13replace_paramPVjPt
	.type	_Z13replace_paramPVjPt, @function
_Z13replace_paramPVjPt:
	lw	a5,0(a0)
	sh	a5,0(a1)
	lhu	zero,0(a1)
	ret
	.size	_Z13replace_paramPVjPt, .-_Z13replace_paramPVjPt
	.align	2
	.globl	_Z10real_worldPVt
	.type	_Z10real_worldPVt, @function
_Z10real_worldPVt:
	lhu	a5,0(a0)
	addi	a5,a5,1
	sh	a5,0(a0)
	lhu	zero,0(a0)
	lhu	a4,2(a0)
	sub	a0,a5,a4
	seqz	a0,a0
	ret
	.size	_Z10real_worldPVt, .-_Z10real_worldPVt
	.align	2
	.globl	_Z14mask_and_shiftt
	.type	_Z14mask_and_shiftt, @function
_Z14mask_and_shiftt:
	addi	a0,a0,1
	slli	a0,a0,16
	srli	a0,a0,12
	ret
	.size	_Z14mask_and_shiftt, .-_Z14mask_and_shiftt
	.align	2
	.globl	_Z15mask_and_shift2t
	.type	_Z15mask_and_shift2t, @function
_Z15mask_and_shift2t:
	addi	a0,a0,1
	slli	a0,a0,16
	srli	a0,a0,16
	slli	a5,a0,4
	add	a0,a5,a0
	ret
	.size	_Z15mask_and_shift2t, .-_Z15mask_and_shift2t
	.align	2
	.globl	_Z15mask_and_shift3s
	.type	_Z15mask_and_shift3s, @function
_Z15mask_and_shift3s:
	addi	a0,a0,1
	slli	a0,a0,16
	srai	a0,a0,12
	ret
	.size	_Z15mask_and_shift3s, .-_Z15mask_and_shift3s
	.align	2
	.globl	_Z15mask_and_shift4s
	.type	_Z15mask_and_shift4s, @function
_Z15mask_and_shift4s:
	addi	a0,a0,1
	slli	a0,a0,16
	srai	a0,a0,16
	slli	a5,a0,4
	add	a0,a5,a0
	ret
	.size	_Z15mask_and_shift4s, .-_Z15mask_and_shift4s
	.align	2
	.globl	_Z15mask_and_shift5t
	.type	_Z15mask_and_shift5t, @function
_Z15mask_and_shift5t:
	addi	a0,a0,1
	slli	a0,a0,4
	slli	a0,a0,16
	srli	a0,a0,16
	ret
	.size	_Z15mask_and_shift5t, .-_Z15mask_and_shift5t
	.align	2
	.globl	_Z15mask_and_shift6t
	.type	_Z15mask_and_shift6t, @function
_Z15mask_and_shift6t:
	addi	a0,a0,1
	slli	a5,a0,4
	add	a0,a0,a5
	slli	a0,a0,16
	srli	a0,a0,16
	ret
	.size	_Z15mask_and_shift6t, .-_Z15mask_and_shift6t
	.align	2
	.globl	_Z15mask_and_shift7s
	.type	_Z15mask_and_shift7s, @function
_Z15mask_and_shift7s:
	addi	a0,a0,1
	slli	a0,a0,16
	srai	a0,a0,12
	slli	a0,a0,16
	srai	a0,a0,16
	ret
	.size	_Z15mask_and_shift7s, .-_Z15mask_and_shift7s
	.align	2
	.globl	_Z15mask_and_shift8s
	.type	_Z15mask_and_shift8s, @function
_Z15mask_and_shift8s:
	addi	a0,a0,1
	slli	a5,a0,4
	add	a0,a0,a5
	slli	a0,a0,16
	srai	a0,a0,16
	ret
	.size	_Z15mask_and_shift8s, .-_Z15mask_and_shift8s
	.section	.text.startup,"ax",@progbits
	.align	2
	.globl	main
	.type	main, @function
main:
	li	a5,-4096
	lhu	a4,-274(a5)
	lbu	a4,-274(a5)
	lhu	a4,-274(a5)
	lbu	a4,-274(a5)
	lhu	a4,-274(a5)
	lui	a2,%hi(dstuhi)
	lw	a3,%lo(dstuhi)(a2)
	addi	a4,a4,10
	sh	a4,0(a3)
	lhu	zero,0(a3)
	lbu	a4,-274(a5)
	lui	a3,%hi(dstuqi)
	lw	a1,%lo(dstuqi)(a3)
	addi	a4,a4,10
	sb	a4,0(a1)
	lbu	zero,0(a1)
	lbu	a4,-274(a5)
	lw	a1,%lo(dstuhi)(a2)
	addi	a4,a4,10
	andi	a4,a4,0xff
	sh	a4,0(a1)
	lhu	zero,0(a1)
	lhu	a4,-274(a5)
	lw	a6,%lo(dstuqi)(a3)
	addi	a4,a4,10
	sh	a4,0(a1)
	sh	a4,0(a1)
	lhu	zero,0(a1)
	lbu	a4,-274(a5)
	addi	a4,a4,10
	sb	a4,0(a6)
	lbu	zero,0(a6)
	lw	a3,%lo(dstuqi)(a3)
	sb	a4,0(a3)
	lbu	zero,0(a3)
	lhu	a4,-274(a5)
	lui	a3,%hi(dstusi)
	lw	a1,%lo(dstusi)(a3)
	slli	a4,a4,16
	srli	a4,a4,16
	sw	a4,0(a1)
	lbu	a3,-274(a5)
	lw	a4,%lo(dstuhi)(a2)
	andi	a3,a3,0xff
	sw	a3,0(a1)
	lhu	a3,-274(a5)
	addi	a3,a3,10
	sh	a3,0(a4)
	lhu	zero,0(a4)
	lhu	a3,-274(a5)
	sh	a3,0(a4)
	lhu	zero,0(a4)
	lhu	a3,-274(a5)
	addi	a3,a3,10
	sh	a3,0(a4)
	sh	a3,0(a4)
	lhu	zero,0(a4)
	lhu	a5,-274(a5)
	addi	a5,a5,10
	sh	a5,0(a4)
	lhu	zero,0(a4)
	ble	a0,zero,.L49
	li	a5,0
.L50:
	sh	zero,0(a4)
	addi	a5,a5,1
	bne	a0,a5,.L50
.L49:
	li	a0,0
	ret
	.size	main, .-main
	.globl	dstg
	.globl	dstusi
	.globl	dstuhi
	.globl	dstuqi
	.section	.sdata,"aw"
	.align	2
	.type	dstg, @object
	.size	dstg, 4
dstg:
	.word	-4370
	.type	dstusi, @object
	.size	dstusi, 4
dstusi:
	.word	-4370
	.type	dstuhi, @object
	.size	dstuhi, 4
dstuhi:
	.word	-4370
	.type	dstuqi, @object
	.size	dstuqi, 4
dstuqi:
	.word	-4370
	.ident	"GCC: (g323f2891276) 12.2.0"
