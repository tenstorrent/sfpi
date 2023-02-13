	.file	"gs_arbiter_war.cc"
	.option nopic
	.attribute arch, "rv32i2p0"
	.attribute unaligned_access, 0
	.attribute stack_align, 16
	.text
	.align	2
	.globl	_Z11lotsoparamsiiiiiiiiiii
	.type	_Z11lotsoparamsiiiiiiiiiii, @function
_Z11lotsoparamsiiiiiiiiiii:
	add	a0,a0,a1
	add	a0,a0,a2
	add	a0,a0,a3
	add	a0,a0,a4
	add	a0,a0,a5
	add	a0,a0,a6
	add	a0,a0,a7
	lw	a7,0(sp)
	lw	a5,8(sp)
	add	a0,a0,a7
	lw	a7,4(sp)
	add	a0,a0,a7
	add	a0,a0,a5
	ret
	.size	_Z11lotsoparamsiiiiiiiiiii, .-_Z11lotsoparamsiiiiiiiiiii
	.align	2
	.globl	_Z6simpleU11rvtt_l1_ptrPiS_
	.type	_Z6simpleU11rvtt_l1_ptrPiS_, @function
_Z6simpleU11rvtt_l1_ptrPiS_:
	lw	a5,0(a0)
	lw	a6,0(a1)
	lw	a2,8(a1)
	lw	a3,12(a1)
	lw	a4,16(a1)
	lw	a0,20(a1)
	add	a5,a5,a6
	add	a5,a5,a2
	add	a5,a5,a3
	add	a5,a5,a4
	add	a0,a5,a0
	ret
	.size	_Z6simpleU11rvtt_l1_ptrPiS_, .-_Z6simpleU11rvtt_l1_ptrPiS_
	.align	2
	.globl	_Z4copyU11rvtt_l1_ptrPiS_
	.type	_Z4copyU11rvtt_l1_ptrPiS_, @function
_Z4copyU11rvtt_l1_ptrPiS_:
	lw	a4,0(a0)
	lw	a5,0(a1)
	lw	t5,4(a1)
	lw	t4,8(a1)
	lw	t3,12(a1)
	lw	t1,16(a1)
	lw	a7,20(a1)
	lw	a6,24(a1)
	lw	a2,28(a1)
	lw	a3,32(a1)
	add	a5,a5,a4
	sw	a5,0(a1)
	lw	t6,4(a0)
	lw	a4,36(a1)
	lw	a5,40(a1)
	add	t5,t5,t6
	sw	t5,4(a1)
	lw	t5,8(a0)
	add	t4,t4,t5
	sw	t4,8(a1)
	lw	t4,12(a0)
	add	t3,t3,t4
	sw	t3,12(a1)
	lw	t3,16(a0)
	add	t1,t1,t3
	sw	t1,16(a1)
	lw	t1,20(a0)
	add	a7,a7,t1
	sw	a7,20(a1)
	lw	a7,24(a0)
	add	a6,a6,a7
	sw	a6,24(a1)
	lw	a6,28(a0)
	add	a2,a2,a6
	sw	a2,28(a1)
	lw	a2,32(a0)
	add	a3,a3,a2
	sw	a3,32(a1)
	lw	a3,36(a0)
	add	a4,a4,a3
	sw	a4,36(a1)
	lw	a4,40(a0)
	add	a5,a5,a4
	sw	a5,40(a1)
	lw	a6,44(a0)
	lw	a2,44(a1)
	lw	a3,48(a1)
	lw	a4,52(a1)
	lw	a5,56(a1)
	add	a2,a2,a6
	sw	a2,44(a1)
	lw	a2,48(a0)
	add	a3,a3,a2
	sw	a3,48(a1)
	lw	a3,52(a0)
	add	a4,a4,a3
	sw	a4,52(a1)
	lw	a4,56(a0)
	add	a5,a5,a4
	sw	a5,56(a1)
	ret
	.size	_Z4copyU11rvtt_l1_ptrPiS_, .-_Z4copyU11rvtt_l1_ptrPiS_
	.align	2
	.globl	_Z11double_loadU11rvtt_l1_ptrPiS_
	.type	_Z11double_loadU11rvtt_l1_ptrPiS_, @function
_Z11double_loadU11rvtt_l1_ptrPiS_:
	lw	a5,0(a0)
	lw	a2,4(a0)
	lw	a7,0(a1)
	lw	a6,8(a1)
	lw	a3,12(a1)
	lw	a4,16(a1)
	lw	a0,20(a1)
	add	a5,a5,a7
	add	a5,a5,a6
	add	a5,a5,a2
	add	a5,a5,a3
	add	a5,a5,a4
	add	a0,a5,a0
	ret
	.size	_Z11double_loadU11rvtt_l1_ptrPiS_, .-_Z11double_loadU11rvtt_l1_ptrPiS_
	.align	2
	.globl	_Z5call1U11rvtt_l1_ptrPiS_
	.type	_Z5call1U11rvtt_l1_ptrPiS_, @function
_Z5call1U11rvtt_l1_ptrPiS_:
	addi	sp,sp,-16
	sw	s0,8(sp)
	sw	ra,12(sp)
	mv	s0,a0
	call	_Z6simpleU11rvtt_l1_ptrPiS_
	lw	a5,0(s0)
	lw	ra,12(sp)
	lw	s0,8(sp)
	addi	sp,sp,16
	add	a0,a0,a5
	jr	ra
	.size	_Z5call1U11rvtt_l1_ptrPiS_, .-_Z5call1U11rvtt_l1_ptrPiS_
	.align	2
	.globl	_Z5call2U11rvtt_l1_ptrPiS_
	.type	_Z5call2U11rvtt_l1_ptrPiS_, @function
_Z5call2U11rvtt_l1_ptrPiS_:
	lw	a0,0(a0)
	addi	sp,sp,-32
	li	a5,10
	sw	a5,8(sp)
	li	a5,9
	sw	a5,4(sp)
	li	a5,8
	sw	a5,0(sp)
	li	a7,7
	li	a6,6
	li	a5,5
	li	a4,4
	li	a3,3
	li	a2,2
	li	a1,1
	sw	ra,28(sp)
	call	_Z11lotsoparamsiiiiiiiiiii
	lw	ra,28(sp)
	addi	sp,sp,32
	jr	ra
	.size	_Z5call2U11rvtt_l1_ptrPiS_, .-_Z5call2U11rvtt_l1_ptrPiS_
	.align	2
	.globl	_Z5call3U11rvtt_l1_ptrPiiiiiiiiiii
	.type	_Z5call3U11rvtt_l1_ptrPiiiiiiiiiii, @function
_Z5call3U11rvtt_l1_ptrPiiiiiiiiiii:
	lw	a0,0(a0)
	addi	sp,sp,-16
	lw	t4,16(sp)
	lw	t3,20(sp)
	lw	t1,24(sp)
	sw	a2,20(sp)
	sw	a1,24(sp)
	sw	a3,16(sp)
	mv	t6,a4
	mv	t5,a5
	mv	a4,a7
	mv	a5,a6
	sw	a0,12(sp)
	lw	a0,12(sp)
	mv	a7,t6
	mv	a6,t5
	mv	a3,t4
	mv	a2,t3
	mv	a1,t1
	addi	sp,sp,16
	tail	_Z11lotsoparamsiiiiiiiiiii
	.size	_Z5call3U11rvtt_l1_ptrPiiiiiiiiiii, .-_Z5call3U11rvtt_l1_ptrPiiiiiiiiiii
	.align	2
	.globl	_Z7at_exitU11rvtt_l1_ptrPiS_
	.type	_Z7at_exitU11rvtt_l1_ptrPiS_, @function
_Z7at_exitU11rvtt_l1_ptrPiS_:
	lw	a0,0(a0)
	ret
	.size	_Z7at_exitU11rvtt_l1_ptrPiS_, .-_Z7at_exitU11rvtt_l1_ptrPiS_
	.align	2
	.globl	_Z11clear_by_llU11rvtt_l1_ptrPiS_
	.type	_Z11clear_by_llU11rvtt_l1_ptrPiS_, @function
_Z11clear_by_llU11rvtt_l1_ptrPiS_:
	lw	a7,0(a0)
	lw	a5,0(a1)
	lw	a4,4(a1)
	lw	a6,8(a1)
	lw	a2,12(a1)
	add	a3,a5,a4
	lw	a0,16(a1)
	add	a5,a5,a7
	add	a5,a5,a4
	add	a5,a5,a6
	add	a5,a5,a2
	add	a5,a5,a3
	add	a0,a5,a0
	ret
	.size	_Z11clear_by_llU11rvtt_l1_ptrPiS_, .-_Z11clear_by_llU11rvtt_l1_ptrPiS_
	.align	2
	.globl	_Z11restrict_llU11rvtt_l1_ptrPViS0_i
	.type	_Z11restrict_llU11rvtt_l1_ptrPViS0_i, @function
_Z11restrict_llU11rvtt_l1_ptrPViS0_i:
	lw	a3,0(a0)
	li	a5,0
	ble	a2,zero,.L15
	lw	a5,4(a0)
	lw	a5,4(a1)
.L15:
	lw	a0,0(a1)
	lw	a4,0(a1)
	lw	a7,0(a1)
	lw	a6,0(a1)
	lw	a2,0(a1)
	sw	a0,0(a1)
	sw	a4,0(a1)
	sw	a7,0(a1)
	sw	a6,0(a1)
	sw	a2,0(a1)
	blt	a5,a3,.L14
	mv	a0,a4
.L14:
	ret
	.size	_Z11restrict_llU11rvtt_l1_ptrPViS0_i, .-_Z11restrict_llU11rvtt_l1_ptrPViS0_i
	.align	2
	.globl	_Z11resolve_allU11rvtt_l1_ptrPViS0_i
	.type	_Z11resolve_allU11rvtt_l1_ptrPViS0_i, @function
_Z11resolve_allU11rvtt_l1_ptrPViS0_i:
	lw	a4,0(a0)
	li	a5,0
	ble	a2,zero,.L19
	lw	a5,4(a0)
	lw	a5,4(a1)
.L19:
	lw	a0,0(a1)
	lw	a2,0(a1)
	lw	a3,0(a1)
	sw	a0,0(a1)
	sw	a2,0(a1)
	sw	a3,0(a1)
	sw	a4,0(a1)
	sw	a5,0(a1)
	ret
	.size	_Z11resolve_allU11rvtt_l1_ptrPViS0_i, .-_Z11resolve_allU11rvtt_l1_ptrPViS0_i
	.align	2
	.globl	_Z14no_resolve_allU11rvtt_l1_ptrPViS0_i
	.type	_Z14no_resolve_allU11rvtt_l1_ptrPViS0_i, @function
_Z14no_resolve_allU11rvtt_l1_ptrPViS0_i:
	lw	a4,0(a0)
	li	a5,0
	ble	a2,zero,.L22
	lw	a5,4(a0)
	lw	a5,4(a1)
.L22:
	lw	a6,0(a1)
	lw	a0,0(a1)
	lw	a2,0(a1)
	lw	a3,0(a1)
	sw	a6,0(a1)
	sw	a0,0(a1)
	sw	a2,0(a1)
	sw	a3,0(a1)
	sw	a4,0(a1)
	sw	a5,0(a1)
	ret
	.size	_Z14no_resolve_allU11rvtt_l1_ptrPViS0_i, .-_Z14no_resolve_allU11rvtt_l1_ptrPViS0_i
	.align	2
	.globl	_Z5loop1U11rvtt_l1_ptrPiS_
	.type	_Z5loop1U11rvtt_l1_ptrPiS_, @function
_Z5loop1U11rvtt_l1_ptrPiS_:
	lw	a0,0(a0)
	addi	a3,a1,16
	li	a5,0
.L25:
	lw	a4,0(a1)
	addi	a1,a1,4
	add	a5,a5,a4
	bne	a3,a1,.L25
	add	a0,a0,a5
	ret
	.size	_Z5loop1U11rvtt_l1_ptrPiS_, .-_Z5loop1U11rvtt_l1_ptrPiS_
	.align	2
	.globl	_Z13loop1_preemptU11rvtt_l1_ptrPiS_
	.type	_Z13loop1_preemptU11rvtt_l1_ptrPiS_, @function
_Z13loop1_preemptU11rvtt_l1_ptrPiS_:
	lw	a0,0(a0)
	mv	a0,a0	# GS L1 war
	addi	a3,a1,16
	li	a5,0
.L28:
	lw	a4,0(a1)
	addi	a1,a1,4
	add	a5,a5,a4
	bne	a3,a1,.L28
	add	a0,a0,a5
	ret
	.size	_Z13loop1_preemptU11rvtt_l1_ptrPiS_, .-_Z13loop1_preemptU11rvtt_l1_ptrPiS_
	.align	2
	.globl	_Z5loop2U11rvtt_l1_ptrPiS_
	.type	_Z5loop2U11rvtt_l1_ptrPiS_, @function
_Z5loop2U11rvtt_l1_ptrPiS_:
	addi	a6,a0,16
	li	a4,0
	li	a5,0
.L31:
	lw	a3,0(a0)
	lw	a2,0(a1)
	addi	a0,a0,4
	addi	a1,a1,4
	add	a5,a5,a2
	add	a4,a4,a3
	bne	a6,a0,.L31
	add	a0,a4,a5
	ret
	.size	_Z5loop2U11rvtt_l1_ptrPiS_, .-_Z5loop2U11rvtt_l1_ptrPiS_
	.align	2
	.globl	_Z5loop3U11rvtt_l1_ptrPiS_
	.type	_Z5loop3U11rvtt_l1_ptrPiS_, @function
_Z5loop3U11rvtt_l1_ptrPiS_:
	lw	a0,0(a0)
	addi	a2,a1,16
	li	a5,0
.L36:
	lw	a4,0(a1)
	lw	a3,40(a1)
	beq	a4,a3,.L38
	lw	a4,4(a1)
.L38:
	addi	a1,a1,4
	add	a5,a5,a4
	bne	a2,a1,.L36
	add	a0,a5,a0
	ret
	.size	_Z5loop3U11rvtt_l1_ptrPiS_, .-_Z5loop3U11rvtt_l1_ptrPiS_
	.align	2
	.globl	_Z13loop3_preemptU11rvtt_l1_ptrPiS_
	.type	_Z13loop3_preemptU11rvtt_l1_ptrPiS_, @function
_Z13loop3_preemptU11rvtt_l1_ptrPiS_:
	lw	a0,0(a0)
	mv	a0,a0	# GS L1 war
	addi	a2,a1,16
	li	a5,0
.L42:
	lw	a4,0(a1)
	lw	a3,40(a1)
	beq	a4,a3,.L44
	lw	a4,4(a1)
.L44:
	addi	a1,a1,4
	add	a5,a5,a4
	bne	a2,a1,.L42
	add	a0,a5,a0
	ret
	.size	_Z13loop3_preemptU11rvtt_l1_ptrPiS_, .-_Z13loop3_preemptU11rvtt_l1_ptrPiS_
	.align	2
	.globl	_Z5loop4U11rvtt_l1_ptrPiS_
	.type	_Z5loop4U11rvtt_l1_ptrPiS_, @function
_Z5loop4U11rvtt_l1_ptrPiS_:
	addi	a2,a1,16
	li	a6,0
	li	a3,0
.L48:
	lw	a5,0(a1)
	lw	a4,40(a1)
	beq	a5,a4,.L50
	add	a3,a3,a5
.L47:
	addi	a1,a1,4
	bne	a2,a1,.L48
	add	a0,a3,a6
	ret
.L50:
	lw	a5,0(a0)
	addi	a0,a0,4
	add	a6,a6,a5
	j	.L47
	.size	_Z5loop4U11rvtt_l1_ptrPiS_, .-_Z5loop4U11rvtt_l1_ptrPiS_
	.align	2
	.globl	_Z5loop5U11rvtt_l1_ptrPiS_
	.type	_Z5loop5U11rvtt_l1_ptrPiS_, @function
_Z5loop5U11rvtt_l1_ptrPiS_:
	mv	a4,a0
	addi	a2,a1,16
	li	a0,0
.L55:
	lw	a5,0(a1)
	lw	a3,40(a1)
	beq	a5,a3,.L57
	addi	a1,a1,4
	add	a0,a0,a5
	bne	a2,a1,.L55
	ret
.L57:
	lw	a5,0(a4)
	addi	a1,a1,4
	addi	a4,a4,4
	add	a0,a0,a5
	bne	a1,a2,.L55
	ret
	.size	_Z5loop5U11rvtt_l1_ptrPiS_, .-_Z5loop5U11rvtt_l1_ptrPiS_
	.align	2
	.globl	_Z5cond1U11rvtt_l1_ptrPiS_b
	.type	_Z5cond1U11rvtt_l1_ptrPiS_b, @function
_Z5cond1U11rvtt_l1_ptrPiS_b:
	beq	a2,zero,.L59
	lw	a0,0(a0)
	lw	a5,4(a1)
	add	a0,a0,a5
	ret
.L59:
	lw	a0,0(a1)
	lw	a5,4(a1)
	add	a0,a0,a5
	ret
	.size	_Z5cond1U11rvtt_l1_ptrPiS_b, .-_Z5cond1U11rvtt_l1_ptrPiS_b
	.align	2
	.globl	_Z5cond2U11rvtt_l1_ptrPiS_b
	.type	_Z5cond2U11rvtt_l1_ptrPiS_b, @function
_Z5cond2U11rvtt_l1_ptrPiS_b:
	lw	a0,0(a0)
	beq	a2,zero,.L62
	lw	a5,0(a1)
	add	a0,a0,a5
	ret
.L62:
	lw	a5,4(a1)
	lw	a2,8(a1)
	lw	a3,12(a1)
	lw	a4,16(a1)
	add	a5,a5,a2
	add	a5,a5,a3
	add	a5,a5,a4
	add	a0,a5,a0
	ret
	.size	_Z5cond2U11rvtt_l1_ptrPiS_b, .-_Z5cond2U11rvtt_l1_ptrPiS_b
	.align	2
	.globl	_Z5cond3U11rvtt_l1_ptrPiS_b
	.type	_Z5cond3U11rvtt_l1_ptrPiS_b, @function
_Z5cond3U11rvtt_l1_ptrPiS_b:
	lw	a0,0(a0)
	beq	a2,zero,.L65
	lw	a5,0(a1)
	add	a0,a0,a5
	ret
.L65:
	lw	a5,4(a1)
	lw	a6,8(a1)
	lw	a2,12(a1)
	lw	a3,16(a1)
	lw	a4,20(a1)
	add	a5,a5,a6
	add	a5,a5,a2
	add	a5,a5,a3
	add	a5,a5,a4
	add	a0,a5,a0
	ret
	.size	_Z5cond3U11rvtt_l1_ptrPiS_b, .-_Z5cond3U11rvtt_l1_ptrPiS_b
	.align	2
	.globl	_Z7complexU11rvtt_l1_ptrPiS_b
	.type	_Z7complexU11rvtt_l1_ptrPiS_b, @function
_Z7complexU11rvtt_l1_ptrPiS_b:
	mv	a5,a0
	lw	a0,0(a0)
	beq	a2,zero,.L68
	lw	a4,0(a1)
	add	a0,a0,a4
.L68:
	li	a4,5
	ble	a0,a4,.L69
	lw	a4,4(a1)
	add	a0,a0,a4
.L69:
	ble	a0,zero,.L67
	lw	a3,4(a1)
	li	a4,33
	add	a0,a0,a3
	ble	a0,a4,.L67
	addi	a1,a1,8
	li	a4,0
	li	a2,33
	j	.L71
.L72:
	lw	a3,0(a1)
	addi	a1,a1,4
	add	a0,a0,a3
	ble	a0,a2,.L67
.L71:
	lw	a3,0(a5)
	addi	a4,a4,1
	addi	a5,a5,4
	add	a0,a0,a3
	bgt	a0,a4,.L72
.L67:
	ret
	.size	_Z7complexU11rvtt_l1_ptrPiS_b, .-_Z7complexU11rvtt_l1_ptrPiS_b
	.align	2
	.globl	_Z8complex2U11rvtt_l1_ptrPiS_ii
	.type	_Z8complex2U11rvtt_l1_ptrPiS_ii, @function
_Z8complex2U11rvtt_l1_ptrPiS_ii:
	mv	a4,a0
	lw	a0,0(a0)
	beq	a2,zero,.L77
	lw	a5,0(a1)
	add	a0,a0,a5
.L77:
	li	a5,5
	ble	a0,a5,.L78
	lw	a5,4(a1)
	add	a0,a0,a5
.L78:
	ble	a0,zero,.L76
	lw	a5,4(a1)
	add	a0,a0,a5
	and	a5,a2,a0
	beq	a5,zero,.L76
	addi	a1,a1,8
	li	a6,0
.L80:
	and	a5,a3,a0
	lw	a7,0(a4)
	addi	a6,a6,1
	beq	a5,zero,.L81
	add	a0,a0,a7
.L81:
	ble	a0,a6,.L76
	lw	a5,0(a1)
	addi	a4,a4,4
	addi	a1,a1,4
	add	a0,a0,a5
	and	a5,a2,a0
	bne	a5,zero,.L80
.L76:
	ret
	.size	_Z8complex2U11rvtt_l1_ptrPiS_ii, .-_Z8complex2U11rvtt_l1_ptrPiS_ii
	.align	2
	.globl	_Z11remove_war1v
	.type	_Z11remove_war1v, @function
_Z11remove_war1v:
	mv	0,0	# GS L1 war
	ret
	.size	_Z11remove_war1v, .-_Z11remove_war1v
	.align	2
	.globl	_Z11remove_war2v
	.type	_Z11remove_war2v, @function
_Z11remove_war2v:
	mv	0,0	# GS L1 war
	mv	0,0	# GS L1 war
	ret
	.size	_Z11remove_war2v, .-_Z11remove_war2v
	.align	2
	.globl	_Z11remove_war3v
	.type	_Z11remove_war3v, @function
_Z11remove_war3v:
	mv	0,0	# GS L1 war
	mv	0,0	# GS L1 war
	mv	0,0	# GS L1 war
	ret
	.size	_Z11remove_war3v, .-_Z11remove_war3v
	.align	2
	.globl	_Z11remove_war4U11rvtt_l1_ptrPiS_
	.type	_Z11remove_war4U11rvtt_l1_ptrPiS_, @function
_Z11remove_war4U11rvtt_l1_ptrPiS_:
	lw	a5,0(a0)
	lw	a6,8(a1)
	lw	a0,0(a1)
	lw	a2,12(a1)
	lw	a3,16(a1)
	lw	a4,20(a1)
	add	a0,a0,a6
	add	a0,a0,a2
	add	a0,a0,a3
	add	a0,a0,a4
	add	a0,a0,a5
	mv	a5,a5	# GS L1 war
	ret
	.size	_Z11remove_war4U11rvtt_l1_ptrPiS_, .-_Z11remove_war4U11rvtt_l1_ptrPiS_
	.align	2
	.globl	_Z7cancel1U11rvtt_l1_ptrPiS_
	.type	_Z7cancel1U11rvtt_l1_ptrPiS_, @function
_Z7cancel1U11rvtt_l1_ptrPiS_:
	lw	a2,0(a0)
	lw	a5,0(a1)
	lw	a3,8(a1)
	lw	a4,12(a1)
	lw	a0,16(a1)
	add	a5,a5,a2
	add	a5,a5,a3
	add	a5,a5,a4
	add	a0,a5,a0
	ret
	.size	_Z7cancel1U11rvtt_l1_ptrPiS_, .-_Z7cancel1U11rvtt_l1_ptrPiS_
	.align	2
	.globl	_Z7cancel2U11rvtt_l1_ptrPiS_S_
	.type	_Z7cancel2U11rvtt_l1_ptrPiS_S_, @function
_Z7cancel2U11rvtt_l1_ptrPiS_S_:
	lw	a4,0(a0)
	lw	a3,0(a1)
	mv	a5,a0
	li	a0,0
	add	a4,a4,a3
	sw	a4,0(a2)
	lw	a4,4(a5)
	lw	a3,4(a1)
	add	a4,a4,a3
	sw	a4,4(a2)
	lw	a4,8(a5)
	lw	a3,8(a1)
	add	a4,a4,a3
	sw	a4,8(a2)
	lw	a4,12(a5)
	lw	a3,12(a1)
	add	a4,a4,a3
	sw	a4,12(a2)
	lw	a4,16(a5)
	lw	a3,16(a1)
	add	a4,a4,a3
	sw	a4,16(a2)
	lw	a5,20(a5)
	lw	a4,20(a1)
	add	a5,a5,a4
	sw	a5,20(a2)
	ret
	.size	_Z7cancel2U11rvtt_l1_ptrPiS_S_, .-_Z7cancel2U11rvtt_l1_ptrPiS_S_
	.align	2
	.globl	_Z7cancel3U11rvtt_l1_ptrPiS_S_
	.type	_Z7cancel3U11rvtt_l1_ptrPiS_S_, @function
_Z7cancel3U11rvtt_l1_ptrPiS_S_:
	lw	a5,4(a0)
	lw	a2,0(a0)
	lw	t3,8(a0)
	addi	sp,sp,-16
	sw	s0,12(sp)
	lw	s0,12(a0)
	lw	t4,16(a0)
	lw	a6,20(a0)
	lw	a0,24(a1)
	lw	t1,52(a1)
	lw	a3,48(a1)
	lw	a7,76(a1)
	lw	a4,72(a1)
	add	a2,a2,a5
	lw	t2,80(a1)
	lw	a5,96(a1)
	lw	t0,100(a1)
	add	a2,a2,a0
	add	a2,a2,t3
	lw	t6,104(a1)
	lw	a0,120(a1)
	lw	t5,124(a1)
	add	a3,a3,t1
	lw	t3,108(a1)
	lw	t1,128(a1)
	add	a3,a3,a2
	add	a4,a4,a7
	add	a3,a3,s0
	lw	a7,132(a1)
	add	a4,a4,t2
	add	a5,a5,t0
	lw	a2,136(a1)
	add	a4,a4,a3
	add	a5,a5,t6
	add	a0,a0,t5
	lw	s0,12(sp)
	add	a4,a4,t4
	add	a5,a5,t3
	add	a0,a0,t1
	add	a5,a5,a4
	add	a0,a0,a7
	add	a5,a5,a6
	add	a0,a0,a2
	add	a0,a0,a5
	addi	sp,sp,16
	jr	ra
	.size	_Z7cancel3U11rvtt_l1_ptrPiS_S_, .-_Z7cancel3U11rvtt_l1_ptrPiS_S_
	.align	2
	.globl	_Z11war_at_exitU11rvtt_l1_ptrPi
	.type	_Z11war_at_exitU11rvtt_l1_ptrPi, @function
_Z11war_at_exitU11rvtt_l1_ptrPi:
	lw	a0,0(a0)
	ret
	.size	_Z11war_at_exitU11rvtt_l1_ptrPi, .-_Z11war_at_exitU11rvtt_l1_ptrPi
	.align	2
	.globl	_Z12split_bb1_siU11rvtt_l1_ptrPiS_b
	.type	_Z12split_bb1_siU11rvtt_l1_ptrPiS_b, @function
_Z12split_bb1_siU11rvtt_l1_ptrPiS_b:
	lw	a0,0(a0)
	lw	a4,0(a1)
	beq	a2,zero,.L105
	mv	a5,a4
	mv	a4,a0
	mv	a0,a5
.L105:
	lw	a5,12(a1)
	lw	a7,16(a1)
	lw	a6,20(a1)
	lw	a2,24(a1)
	lw	a3,28(a1)
	add	a5,a5,a7
	add	a5,a5,a6
	add	a5,a5,a2
	add	a5,a5,a3
	add	a5,a5,a4
	add	a0,a5,a0
	ret
	.size	_Z12split_bb1_siU11rvtt_l1_ptrPiS_b, .-_Z12split_bb1_siU11rvtt_l1_ptrPiS_b
	.align	2
	.globl	_Z13split_bb1_usiU11rvtt_l1_ptrPjPib
	.type	_Z13split_bb1_usiU11rvtt_l1_ptrPjPib, @function
_Z13split_bb1_usiU11rvtt_l1_ptrPjPib:
	lw	a0,0(a0)
	lw	a4,0(a1)
	beq	a2,zero,.L110
	mv	a5,a4
	mv	a4,a0
	mv	a0,a5
.L110:
	lw	a5,12(a1)
	lw	a7,16(a1)
	lw	a6,20(a1)
	lw	a2,24(a1)
	lw	a3,28(a1)
	add	a5,a5,a7
	add	a5,a5,a6
	add	a5,a5,a2
	add	a5,a5,a3
	add	a5,a5,a4
	add	a0,a5,a0
	ret
	.size	_Z13split_bb1_usiU11rvtt_l1_ptrPjPib, .-_Z13split_bb1_usiU11rvtt_l1_ptrPjPib
	.align	2
	.globl	_Z12split_bb1_hiU11rvtt_l1_ptrPsPib
	.type	_Z12split_bb1_hiU11rvtt_l1_ptrPsPib, @function
_Z12split_bb1_hiU11rvtt_l1_ptrPsPib:
	lh	a0,0(a0)
	lw	a4,0(a1)
	beq	a2,zero,.L115
	mv	a5,a4
	mv	a4,a0
	mv	a0,a5
.L115:
	lw	a5,12(a1)
	lw	a7,16(a1)
	lw	a6,20(a1)
	lw	a2,24(a1)
	lw	a3,28(a1)
	add	a5,a5,a7
	add	a5,a5,a6
	add	a5,a5,a2
	add	a5,a5,a3
	add	a5,a5,a4
	add	a0,a5,a0
	ret
	.size	_Z12split_bb1_hiU11rvtt_l1_ptrPsPib, .-_Z12split_bb1_hiU11rvtt_l1_ptrPsPib
	.align	2
	.globl	_Z13split_bb1_uhiU11rvtt_l1_ptrPtPib
	.type	_Z13split_bb1_uhiU11rvtt_l1_ptrPtPib, @function
_Z13split_bb1_uhiU11rvtt_l1_ptrPtPib:
	lhu	a0,0(a0)
	lw	a4,0(a1)
	beq	a2,zero,.L120
	mv	a5,a4
	mv	a4,a0
	mv	a0,a5
.L120:
	lw	a5,12(a1)
	lw	a7,16(a1)
	lw	a6,20(a1)
	lw	a2,24(a1)
	lw	a3,28(a1)
	add	a5,a5,a7
	add	a5,a5,a6
	add	a5,a5,a2
	add	a5,a5,a3
	add	a5,a5,a4
	add	a0,a5,a0
	ret
	.size	_Z13split_bb1_uhiU11rvtt_l1_ptrPtPib, .-_Z13split_bb1_uhiU11rvtt_l1_ptrPtPib
	.align	2
	.globl	_Z12split_bb1_qiU11rvtt_l1_ptrPcPib
	.type	_Z12split_bb1_qiU11rvtt_l1_ptrPcPib, @function
_Z12split_bb1_qiU11rvtt_l1_ptrPcPib:
	lbu	a0,0(a0)
	lw	a4,0(a1)
	beq	a2,zero,.L125
	mv	a5,a4
	mv	a4,a0
	mv	a0,a5
.L125:
	lw	a5,12(a1)
	lw	a7,16(a1)
	lw	a6,20(a1)
	lw	a2,24(a1)
	lw	a3,28(a1)
	add	a5,a5,a7
	add	a5,a5,a6
	add	a5,a5,a2
	add	a5,a5,a3
	add	a5,a5,a4
	add	a0,a5,a0
	ret
	.size	_Z12split_bb1_qiU11rvtt_l1_ptrPcPib, .-_Z12split_bb1_qiU11rvtt_l1_ptrPcPib
	.align	2
	.globl	_Z13split_bb1_uqiU11rvtt_l1_ptrPhPib
	.type	_Z13split_bb1_uqiU11rvtt_l1_ptrPhPib, @function
_Z13split_bb1_uqiU11rvtt_l1_ptrPhPib:
	lbu	a0,0(a0)
	lw	a4,0(a1)
	beq	a2,zero,.L130
	mv	a5,a4
	mv	a4,a0
	mv	a0,a5
.L130:
	lw	a5,12(a1)
	lw	a7,16(a1)
	lw	a6,20(a1)
	lw	a2,24(a1)
	lw	a3,28(a1)
	add	a5,a5,a7
	add	a5,a5,a6
	add	a5,a5,a2
	add	a5,a5,a3
	add	a5,a5,a4
	add	a0,a5,a0
	ret
	.size	_Z13split_bb1_uqiU11rvtt_l1_ptrPhPib, .-_Z13split_bb1_uqiU11rvtt_l1_ptrPhPib
	.align	2
	.globl	_Z13split_bb1_ptrPU11rvtt_l1_ptrPjPPib
	.type	_Z13split_bb1_ptrPU11rvtt_l1_ptrPjPPib, @function
_Z13split_bb1_ptrPU11rvtt_l1_ptrPjPPib:
	lw	a3,0(a1)
	lw	a4,0(a0)
	beq	a2,zero,.L135
	mv	a5,a3
	mv	a3,a4
	mv	a4,a5
.L135:
	lw	a2,16(a1)
	lw	a5,12(a1)
	lw	a0,20(a1)
	lw	a7,24(a1)
	lw	a6,0(a2)
	lw	a5,0(a5)
	lw	a2,28(a1)
	lw	a0,0(a0)
	lw	a1,0(a7)
	lw	a2,0(a2)
	add	a5,a5,a6
	lw	a3,0(a3)
	add	a5,a5,a0
	add	a5,a5,a1
	lw	a0,0(a4)
	add	a5,a5,a2
	add	a5,a5,a3
	add	a0,a5,a0
	ret
	.size	_Z13split_bb1_ptrPU11rvtt_l1_ptrPjPPib, .-_Z13split_bb1_ptrPU11rvtt_l1_ptrPjPPib
	.align	2
	.globl	_Z18split_bb1_baselineU11rvtt_l1_ptrPViS0_b
	.type	_Z18split_bb1_baselineU11rvtt_l1_ptrPViS0_b, @function
_Z18split_bb1_baselineU11rvtt_l1_ptrPViS0_b:
	beq	a2,zero,.L140
	lw	a4,0(a0)
	lw	a0,0(a1)
.L141:
	lw	a5,12(a1)
	lw	a3,16(a1)
	lw	a6,20(a1)
	lw	a2,24(a1)
	add	a5,a5,a3
	lw	a3,28(a1)
	add	a5,a5,a6
	add	a5,a5,a2
	add	a5,a5,a3
	add	a5,a5,a4
	add	a0,a5,a0
	ret
.L140:
	lw	a4,0(a1)
	lw	a0,0(a0)
	j	.L141
	.size	_Z18split_bb1_baselineU11rvtt_l1_ptrPViS0_b, .-_Z18split_bb1_baselineU11rvtt_l1_ptrPViS0_b
	.align	2
	.globl	_Z9split_bb2U11rvtt_l1_ptrPiS_b
	.type	_Z9split_bb2U11rvtt_l1_ptrPiS_b, @function
_Z9split_bb2U11rvtt_l1_ptrPiS_b:
	lw	a6,0(a0)
	lw	a3,0(a1)
	beq	a2,zero,.L144
	lw	a0,4(a0)
.L143:
	lw	a4,12(a1)
	lw	a5,16(a1)
	lw	t1,20(a1)
	lw	a7,24(a1)
	lw	a2,28(a1)
	add	a5,a4,a5
	add	a5,a5,t1
	add	a5,a5,a7
	add	a5,a5,a2
	add	a5,a5,a6
	add	a5,a5,a3
	add	a0,a5,a0
	ret
.L144:
	li	a0,0
	j	.L143
	.size	_Z9split_bb2U11rvtt_l1_ptrPiS_b, .-_Z9split_bb2U11rvtt_l1_ptrPiS_b
	.align	2
	.globl	_Z22volatile_type_mismatchU11rvtt_l1_ptrPVt
	.type	_Z22volatile_type_mismatchU11rvtt_l1_ptrPVt, @function
_Z22volatile_type_mismatchU11rvtt_l1_ptrPVt:
	lhu	a0,0(a0)
	ret
	.size	_Z22volatile_type_mismatchU11rvtt_l1_ptrPVt, .-_Z22volatile_type_mismatchU11rvtt_l1_ptrPVt
	.align	2
	.globl	_Z7warningU11rvtt_l1_ptrPiS_
	.type	_Z7warningU11rvtt_l1_ptrPiS_, @function
_Z7warningU11rvtt_l1_ptrPiS_:
	lw	a5,0(a0)
	lw	a4,4(a0)
	lw	a0,0(a1)
	mv	a5,a5	# GS L1 war
	add	a5,a5,a4
	add	a0,a5,a0
	ret
	.size	_Z7warningU11rvtt_l1_ptrPiS_, .-_Z7warningU11rvtt_l1_ptrPiS_
	.align	2
	.globl	_Z19fails_in_productionU11rvtt_l1_ptrP8longlongb
	.type	_Z19fails_in_productionU11rvtt_l1_ptrP8longlongb, @function
_Z19fails_in_productionU11rvtt_l1_ptrP8longlongb:
	addi	sp,sp,-16
	beq	a1,zero,.L148
	lw	a4,4(a0)
	lw	a5,0(a0)
	sw	a4,8(sp)
	sw	a5,12(sp)
	addi	sp,sp,16
	jr	ra
.L148:
	lw	a4,12(a0)
	lw	a5,8(a0)
	sw	a4,8(sp)
	sw	a5,12(sp)
	addi	sp,sp,16
	jr	ra
	.size	_Z19fails_in_productionU11rvtt_l1_ptrP8longlongb, .-_Z19fails_in_productionU11rvtt_l1_ptrP8longlongb
	.align	2
	.globl	_Z8hiloloadU11rvtt_l1_ptrP11tt_uint64_t
	.type	_Z8hiloloadU11rvtt_l1_ptrP11tt_uint64_t, @function
_Z8hiloloadU11rvtt_l1_ptrP11tt_uint64_t:
	mv	a5,a0
	lw	a1,4(a5)
	lw	a0,0(a0)
	ret
	.size	_Z8hiloloadU11rvtt_l1_ptrP11tt_uint64_t, .-_Z8hiloloadU11rvtt_l1_ptrP11tt_uint64_t
	.section	.text.startup,"ax",@progbits
	.align	2
	.globl	main
	.type	main, @function
main:
	li	a0,0
	ret
	.size	main, .-main
	.ident	"GCC: (GNU) 10.2.0"
