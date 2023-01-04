	.file	"half.cc"
	.option nopic
	.attribute arch, "rv32i2p0"
	.attribute unaligned_access, 0
	.attribute stack_align, 16
	.text
	.align	2
	.globl	_Z7replacePVt
	.type	_Z7replacePVt, @function
_Z7replacePVt:
	lhu	a5,0(a0)
	lui	a4,%hi(dst)
	lw	a4,%lo(dst)(a4)
	addi	a5,a5,10
	slli	a5,a5,16
	srli	a5,a5,16
	sh	a5,0(a4)
	ret
	.size	_Z7replacePVt, .-_Z7replacePVt
	.align	2
	.globl	_Z8replace2PVt
	.type	_Z8replace2PVt, @function
_Z8replace2PVt:
	lhu	a5,0(a0)
	lui	a4,%hi(dst)
	lw	a4,%lo(dst)(a4)
	addi	a5,a5,10
	slli	a5,a5,16
	srli	a5,a5,16
	sh	a5,0(a4)
	sh	a5,0(a4)
	ret
	.size	_Z8replace2PVt, .-_Z8replace2PVt
	.align	2
	.globl	_Z8replace3PVt
	.type	_Z8replace3PVt, @function
_Z8replace3PVt:
	lhu	a5,0(a0)
	lui	a4,%hi(dst)
	lw	a4,%lo(dst)(a4)
	addi	a5,a5,10
	slli	a5,a5,16
	srli	a5,a5,16
	sh	a5,0(a4)
	sh	a5,0(a4)
	sh	a5,0(a4)
	ret
	.size	_Z8replace3PVt, .-_Z8replace3PVt
	.align	2
	.globl	_Z18replace_word_writePVt
	.type	_Z18replace_word_writePVt, @function
_Z18replace_word_writePVt:
	lhu	a5,0(a0)
	lui	a4,%hi(dstw)
	lw	a4,%lo(dstw)(a4)
	slli	a5,a5,16
	srli	a5,a5,16
	sw	a5,0(a4)
	ret
	.size	_Z18replace_word_writePVt, .-_Z18replace_word_writePVt
	.align	2
	.globl	_Z14keep_later_usePVt
	.type	_Z14keep_later_usePVt, @function
_Z14keep_later_usePVt:
	lhu	a0,0(a0)
	lui	a5,%hi(dst)
	lw	a5,%lo(dst)(a5)
	addi	a0,a0,10
	slli	a0,a0,16
	srli	a0,a0,16
	sh	a0,0(a5)
	ret
	.size	_Z14keep_later_usePVt, .-_Z14keep_later_usePVt
	.align	2
	.globl	_Z15keep_later_use2Pt
	.type	_Z15keep_later_use2Pt, @function
_Z15keep_later_use2Pt:
	lhu	a5,0(a0)
	lui	a4,%hi(dst)
	lw	a4,%lo(dst)(a4)
	addi	a0,a5,10
	slli	a0,a0,16
	srli	a0,a0,16
	sh	a5,0(a4)
	ret
	.size	_Z15keep_later_use2Pt, .-_Z15keep_later_use2Pt
	.align	2
	.globl	_Z15keep_later_use2PVt
	.type	_Z15keep_later_use2PVt, @function
_Z15keep_later_use2PVt:
	lhu	a0,0(a0)
	lui	a5,%hi(dst)
	lw	a5,%lo(dst)(a5)
	addi	a0,a0,10
	slli	a0,a0,16
	srli	a0,a0,16
	sh	a0,0(a5)
	sh	a0,0(a5)
	ret
	.size	_Z15keep_later_use2PVt, .-_Z15keep_later_use2PVt
	.align	2
	.globl	_Z7keep_bbPVti
	.type	_Z7keep_bbPVti, @function
_Z7keep_bbPVti:
	lhu	a0,0(a0)
	lui	a5,%hi(dst)
	lw	a4,%lo(dst)(a5)
	addi	a0,a0,10
	slli	a0,a0,16
	srli	a0,a0,16
	sh	a0,0(a4)
	ble	a1,zero,.L10
	li	a5,0
.L11:
	sh	zero,0(a4)
	addi	a5,a5,1
	bne	a1,a5,.L11
.L10:
	ret
	.size	_Z7keep_bbPVti, .-_Z7keep_bbPVti
	.align	2
	.globl	_Z15keep_word_writePVt
	.type	_Z15keep_word_writePVt, @function
_Z15keep_word_writePVt:
	lhu	a5,0(a0)
	lui	a4,%hi(dstw)
	lw	a4,%lo(dstw)(a4)
	addi	a5,a5,10
	slli	a5,a5,16
	srli	a5,a5,16
	sw	a5,0(a4)
	ret
	.size	_Z15keep_word_writePVt, .-_Z15keep_word_writePVt
	.align	2
	.globl	_Z14keep_contortedPVt
	.type	_Z14keep_contortedPVt, @function
_Z14keep_contortedPVt:
	lhu	a5,0(a0)
	addi	a5,a5,10
	slli	a5,a5,16
	srli	a5,a5,16
	sh	a5,0(a5)
	ret
	.size	_Z14keep_contortedPVt, .-_Z14keep_contortedPVt
	.align	2
	.globl	_Z18keep_word_load_retPVj
	.type	_Z18keep_word_load_retPVj, @function
_Z18keep_word_load_retPVj:
	lw	a0,0(a0)
	lui	a5,%hi(dst)
	lw	a5,%lo(dst)(a5)
	slli	a0,a0,16
	srli	a0,a0,16
	sh	a0,0(a5)
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
	lhu	a3,0(a0)
	lhu	a5,0(a0)
	lhu	a4,0(a0)
	add	a0,a5,a3
	add	a0,a0,a4
	slli	a0,a0,16
	srli	a0,a0,16
	ret
	.size	_Z9keep_ret2PVt, .-_Z9keep_ret2PVt
	.align	2
	.globl	_Z10keep_paramPVjPt
	.type	_Z10keep_paramPVjPt, @function
_Z10keep_paramPVjPt:
	lw	a5,0(a0)
	sh	a5,0(a1)
	ret
	.size	_Z10keep_paramPVjPt, .-_Z10keep_paramPVjPt
	.align	2
	.globl	_Z6use_fnj
	.type	_Z6use_fnj, @function
_Z6use_fnj:
	lui	a5,%hi(dstw)
	lw	a5,%lo(dstw)(a5)
	sw	a0,0(a5)
	ret
	.size	_Z6use_fnj, .-_Z6use_fnj
	.align	2
	.globl	_Z17replace_func_callPVt
	.type	_Z17replace_func_callPVt, @function
_Z17replace_func_callPVt:
	lhu	a0,0(a0)
	slli	a0,a0,16
	srli	a0,a0,16
	tail	_Z6use_fnj
	.size	_Z17replace_func_callPVt, .-_Z17replace_func_callPVt
	.align	2
	.globl	_Z14keep_func_callPVt
	.type	_Z14keep_func_callPVt, @function
_Z14keep_func_callPVt:
	addi	sp,sp,-16
	sw	s0,8(sp)
	lhu	s0,0(a0)
	sw	s1,4(sp)
	mv	s1,a0
	addi	s0,s0,10
	slli	s0,s0,16
	srli	s0,s0,16
	mv	a0,s0
	sw	ra,12(sp)
	call	_Z6use_fnj
	lw	ra,12(sp)
	sh	s0,0(s1)
	lw	s0,8(sp)
	lw	s1,4(sp)
	addi	sp,sp,16
	jr	ra
	.size	_Z14keep_func_callPVt, .-_Z14keep_func_callPVt
	.align	2
	.globl	_Z15keep_func_call2PVt
	.type	_Z15keep_func_call2PVt, @function
_Z15keep_func_call2PVt:
	lhu	a0,0(a0)
	addi	a0,a0,10
	slli	a0,a0,16
	srli	a0,a0,16
	tail	_Z6use_fnj
	.size	_Z15keep_func_call2PVt, .-_Z15keep_func_call2PVt
	.align	2
	.globl	_Z8keep_asmPVt
	.type	_Z8keep_asmPVt, @function
_Z8keep_asmPVt:
	lhu	a5,0(a0)
	addi	a5,a5,5
	slli	a5,a5,16
	srli	a5,a5,16
 #APP
# 151 "half.cc" 1
	.word (3)
# 0 "" 2
 #NO_APP
	lui	a4,%hi(dst)
	lw	a4,%lo(dst)(a4)
	sh	a5,0(a4)
	ret
	.size	_Z8keep_asmPVt, .-_Z8keep_asmPVt
	.section	.text.startup,"ax",@progbits
	.align	2
	.globl	main
	.type	main, @function
main:
	li	a5,-4096
	lhu	a3,-274(a5)
	addi	sp,sp,-16
	sw	s2,0(sp)
	lui	s2,%hi(dst)
	lw	a4,%lo(dst)(s2)
	addi	a3,a3,10
	slli	a3,a3,16
	sw	ra,12(sp)
	sw	s0,8(sp)
	sw	s1,4(sp)
	srli	a3,a3,16
	sh	a3,0(a4)
	lhu	a3,-274(a5)
	lui	a2,%hi(dstw)
	lw	a2,%lo(dstw)(a2)
	addi	a3,a3,10
	slli	a3,a3,16
	srli	a3,a3,16
	sh	a3,0(a4)
	sh	a3,0(a4)
	lhu	a3,-274(a5)
	addi	a3,a3,10
	slli	a3,a3,16
	srli	a3,a3,16
	sh	a3,0(a4)
	sh	a3,0(a4)
	sh	a3,0(a4)
	lhu	a3,-274(a5)
	slli	a3,a3,16
	srli	a3,a3,16
	sw	a3,0(a2)
	lhu	a3,-274(a5)
	addi	a3,a3,10
	slli	a3,a3,16
	srli	a3,a3,16
	sh	a3,0(a4)
	lhu	a3,-274(a5)
	addi	a3,a3,10
	slli	a3,a3,16
	srli	a3,a3,16
	sh	a3,0(a4)
	sh	a3,0(a4)
	lhu	a5,-274(a5)
	addi	a5,a5,10
	slli	a5,a5,16
	srli	a5,a5,16
	sh	a5,0(a4)
	ble	a0,zero,.L26
	li	a5,0
.L27:
	sh	zero,0(a4)
	addi	a5,a5,1
	bne	a0,a5,.L27
.L26:
	li	s0,-4096
	lhu	a5,-274(s0)
	addi	a5,a5,10
	slli	a5,a5,16
	srli	a5,a5,16
	sw	a5,0(a2)
	lhu	a5,-274(s0)
	addi	a5,a5,10
	slli	a5,a5,16
	srli	a5,a5,16
	sh	a5,0(a5)
	lhu	a5,-274(s0)
	lhu	a2,-272(s0)
	slli	a5,a5,16
	srli	a5,a5,16
	sh	a5,0(a4)
	lhu	a5,-274(s0)
	lhu	a5,-274(s0)
	lhu	a5,-274(s0)
	lhu	a5,-274(s0)
	lhu	a5,-274(s0)
	lhu	a5,-272(s0)
	lhu	a0,-274(s0)
	slli	a0,a0,16
	srli	a0,a0,16
	call	_Z6use_fnj
	lhu	s1,-274(s0)
	addi	s1,s1,10
	slli	s1,s1,16
	srli	s1,s1,16
	mv	a0,s1
	call	_Z6use_fnj
	sh	s1,-274(s0)
	lhu	a0,-274(s0)
	addi	a0,a0,10
	slli	a0,a0,16
	srli	a0,a0,16
	call	_Z6use_fnj
	lhu	a5,-274(s0)
	addi	a5,a5,5
	slli	a5,a5,16
	srli	a5,a5,16
 #APP
# 151 "half.cc" 1
	.word (3)
# 0 "" 2
 #NO_APP
	lw	a4,%lo(dst)(s2)
	lw	ra,12(sp)
	lw	s0,8(sp)
	sh	a5,0(a4)
	lw	s1,4(sp)
	lw	s2,0(sp)
	li	a0,0
	addi	sp,sp,16
	jr	ra
	.size	main, .-main
	.globl	dstg
	.globl	dstw
	.globl	dst
	.section	.sdata,"aw"
	.align	2
	.type	dstg, @object
	.size	dstg, 4
dstg:
	.word	-4370
	.type	dstw, @object
	.size	dstw, 4
dstw:
	.word	-4370
	.type	dst, @object
	.size	dst, 4
dst:
	.word	-4370
	.ident	"GCC: (GNU) 10.2.0"
