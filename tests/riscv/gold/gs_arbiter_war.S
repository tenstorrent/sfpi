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
	.globl	_Z6simplePiS_
	.type	_Z6simplePiS_, @function
_Z6simplePiS_:
	lw	a5,0(a0)	# L1 load
	lw	a6,0(a1)
	lw	a2,8(a1)
	lw	a3,12(a1)
	lw	a4,16(a1)
	mv	a5,a5	# GS L1 war
	lw	a0,20(a1)
	add	a5,a5,a6
	add	a5,a5,a2
	add	a5,a5,a3
	add	a5,a5,a4
	add	a0,a5,a0
	ret
	.size	_Z6simplePiS_, .-_Z6simplePiS_
	.align	2
	.globl	_Z4copyPiS_
	.type	_Z4copyPiS_, @function
_Z4copyPiS_:
	lw	a4,0(a0)	# L1 load
	lw	a5,0(a1)
	lw	t5,4(a1)
	lw	t4,8(a1)
	lw	t3,12(a1)
	mv	a4,a4	# GS L1 war
	lw	t1,16(a1)
	lw	a7,20(a1)
	lw	a6,24(a1)
	add	a5,a5,a4
	sw	a5,0(a1)
	lw	t6,4(a0)	# L1 load
	lw	a2,28(a1)
	lw	a3,32(a1)
	lw	a4,36(a1)
	lw	a5,40(a1)
	add	t5,t5,t6
	sw	t5,4(a1)
	lw	t5,8(a0)	# L1 load
	add	t4,t4,t5
	sw	t4,8(a1)
	lw	t4,12(a0)	# L1 load
	add	t3,t3,t4
	sw	t3,12(a1)
	lw	t3,16(a0)	# L1 load
	add	t1,t1,t3
	sw	t1,16(a1)
	lw	t1,20(a0)	# L1 load
	add	a7,a7,t1
	sw	a7,20(a1)
	lw	a7,24(a0)	# L1 load
	add	a6,a6,a7
	sw	a6,24(a1)
	lw	a6,28(a0)	# L1 load
	add	a2,a2,a6
	sw	a2,28(a1)
	lw	a2,32(a0)	# L1 load
	add	a3,a3,a2
	sw	a3,32(a1)
	lw	a3,36(a0)	# L1 load
	add	a4,a4,a3
	sw	a4,36(a1)
	lw	a4,40(a0)	# L1 load
	add	a5,a5,a4
	sw	a5,40(a1)
	lw	a6,44(a0)	# L1 load
	lw	a2,44(a1)
	lw	a3,48(a1)
	lw	a4,52(a1)
	lw	a5,56(a1)
	add	a2,a2,a6
	sw	a2,44(a1)
	lw	a2,48(a0)	# L1 load
	add	a3,a3,a2
	sw	a3,48(a1)
	lw	a3,52(a0)	# L1 load
	add	a4,a4,a3
	sw	a4,52(a1)
	lw	a4,56(a0)	# L1 load
	add	a5,a5,a4
	sw	a5,56(a1)
	ret
	.size	_Z4copyPiS_, .-_Z4copyPiS_
	.align	2
	.globl	_Z11double_loadPiS_
	.type	_Z11double_loadPiS_, @function
_Z11double_loadPiS_:
	lw	a5,0(a0)	# L1 load
	lw	a2,4(a0)	# L1 load
	lw	a6,0(a1)
	lw	a0,8(a1)
	lw	a3,12(a1)
	lw	a4,16(a1)
	add	a5,a5,a6
	add	a5,a5,a0
	add	a5,a5,a2
	lw	a0,20(a1)
	add	a5,a5,a3
	add	a5,a5,a4
	add	a0,a5,a0
	ret
	.size	_Z11double_loadPiS_, .-_Z11double_loadPiS_
	.align	2
	.globl	_Z5call1PiS_
	.type	_Z5call1PiS_, @function
_Z5call1PiS_:
	addi	sp,sp,-16
	sw	s0,8(sp)
	lw	s0,0(a0)	# L1 load
	sw	ra,12(sp)
	mv	s0,s0	# GS L1 war
	call	_Z6simplePiS_
	lw	ra,12(sp)
	add	a0,a0,s0
	lw	s0,8(sp)
	addi	sp,sp,16
	jr	ra
	.size	_Z5call1PiS_, .-_Z5call1PiS_
	.align	2
	.globl	_Z5call2PiS_
	.type	_Z5call2PiS_, @function
_Z5call2PiS_:
	lw	a0,0(a0)	# L1 load
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
	mv	a0,a0	# GS L1 war
	call	_Z11lotsoparamsiiiiiiiiiii
	lw	ra,28(sp)
	addi	sp,sp,32
	jr	ra
	.size	_Z5call2PiS_, .-_Z5call2PiS_
	.align	2
	.globl	_Z5call3Piiiiiiiiiii
	.type	_Z5call3Piiiiiiiiiii, @function
_Z5call3Piiiiiiiiiii:
	lw	a0,0(a0)	# L1 load
	addi	sp,sp,-16
	lw	t4,16(sp)
	lw	t3,20(sp)
	lw	t1,24(sp)
	sw	a2,20(sp)
	sw	a1,24(sp)
	sw	a3,16(sp)
	sw	a0,12(sp)
	lw	a0,12(sp)
	mv	t6,a4
	mv	t5,a5
	mv	a4,a7
	mv	a5,a6
	mv	a7,t6
	mv	a6,t5
	mv	a3,t4
	mv	a2,t3
	mv	a1,t1
	addi	sp,sp,16
	tail	_Z11lotsoparamsiiiiiiiiiii
	.size	_Z5call3Piiiiiiiiiii, .-_Z5call3Piiiiiiiiiii
	.align	2
	.globl	_Z7at_exitPiS_
	.type	_Z7at_exitPiS_, @function
_Z7at_exitPiS_:
	lw	a0,0(a0)	# L1 load
	mv	a0,a0	# GS L1 war
	ret
	.size	_Z7at_exitPiS_, .-_Z7at_exitPiS_
	.align	2
	.globl	_Z5loop1PiS_
	.type	_Z5loop1PiS_, @function
_Z5loop1PiS_:
	lw	a0,0(a0)	# L1 load
	addi	a3,a1,16
	li	a5,0
.L14:
	lw	a4,0(a1)
	addi	a1,a1,4
	add	a5,a5,a4
	mv	a0,a0	# GS L1 war
	bne	a3,a1,.L14
	add	a0,a0,a5
	ret
	.size	_Z5loop1PiS_, .-_Z5loop1PiS_
	.align	2
	.globl	_Z13loop1_preemptPiS_
	.type	_Z13loop1_preemptPiS_, @function
_Z13loop1_preemptPiS_:
	lw	a0,0(a0)	# L1 load
	mv	a0,a0	# GS L1 war
	addi	a3,a1,16
	li	a5,0
.L17:
	lw	a4,0(a1)
	addi	a1,a1,4
	add	a5,a5,a4
	bne	a3,a1,.L17
	add	a0,a0,a5
	ret
	.size	_Z13loop1_preemptPiS_, .-_Z13loop1_preemptPiS_
	.align	2
	.globl	_Z5loop2PiS_
	.type	_Z5loop2PiS_, @function
_Z5loop2PiS_:
	addi	a6,a0,16
	li	a4,0
	li	a5,0
.L20:
	lw	a3,0(a0)	# L1 load
	lw	a2,0(a1)
	addi	a0,a0,4
	addi	a1,a1,4
	add	a5,a5,a2
	add	a4,a4,a3
	bne	a0,a6,.L20
	add	a0,a4,a5
	ret
	.size	_Z5loop2PiS_, .-_Z5loop2PiS_
	.align	2
	.globl	_Z5loop3PiS_
	.type	_Z5loop3PiS_, @function
_Z5loop3PiS_:
	lw	a0,0(a0)	# L1 load
	addi	a2,a1,16
	li	a5,0
.L25:
	lw	a4,0(a1)
	lw	a3,40(a1)
	beq	a4,a3,.L27
	lw	a4,4(a1)
.L27:
	addi	a1,a1,4
	add	a5,a5,a4
	mv	a0,a0	# GS L1 war
	bne	a2,a1,.L25
	add	a0,a5,a0
	ret
	.size	_Z5loop3PiS_, .-_Z5loop3PiS_
	.align	2
	.globl	_Z13loop3_preemptPiS_
	.type	_Z13loop3_preemptPiS_, @function
_Z13loop3_preemptPiS_:
	lw	a0,0(a0)	# L1 load
	mv	a0,a0	# GS L1 war
	addi	a2,a1,16
	li	a5,0
.L31:
	lw	a4,0(a1)
	lw	a3,40(a1)
	beq	a4,a3,.L33
	lw	a4,4(a1)
.L33:
	addi	a1,a1,4
	add	a5,a5,a4
	bne	a2,a1,.L31
	add	a0,a5,a0
	ret
	.size	_Z13loop3_preemptPiS_, .-_Z13loop3_preemptPiS_
	.align	2
	.globl	_Z5loop4PiS_
	.type	_Z5loop4PiS_, @function
_Z5loop4PiS_:
	addi	a2,a1,16
	li	a6,0
	li	a3,0
.L37:
	lw	a5,0(a1)
	lw	a4,40(a1)
	beq	a5,a4,.L39
	add	a3,a3,a5
.L36:
	addi	a1,a1,4
	bne	a1,a2,.L37
	add	a0,a3,a6
	ret
.L39:
	lw	a5,0(a0)	# L1 load
	addi	a0,a0,4
	add	a6,a6,a5
	j	.L36
	.size	_Z5loop4PiS_, .-_Z5loop4PiS_
	.align	2
	.globl	_Z5loop5PiS_
	.type	_Z5loop5PiS_, @function
_Z5loop5PiS_:
	mv	a4,a0
	addi	a2,a1,16
	li	a0,0
.L44:
	lw	a5,0(a1)
	lw	a3,40(a1)
	beq	a5,a3,.L46
	addi	a1,a1,4
	add	a0,a0,a5
	bne	a1,a2,.L44
	ret
.L46:
	lw	a5,0(a4)	# L1 load
	addi	a1,a1,4
	addi	a4,a4,4
	add	a0,a0,a5
	bne	a1,a2,.L44
	ret
	.size	_Z5loop5PiS_, .-_Z5loop5PiS_
	.align	2
	.globl	_Z5cond1PiS_b
	.type	_Z5cond1PiS_b, @function
_Z5cond1PiS_b:
	beq	a2,zero,.L48
	lw	a0,0(a0)	# L1 load
	lw	a5,4(a1)
	add	a0,a0,a5
	ret
.L48:
	lw	a0,0(a1)
	lw	a5,4(a1)
	add	a0,a0,a5
	ret
	.size	_Z5cond1PiS_b, .-_Z5cond1PiS_b
	.align	2
	.globl	_Z5cond2PiS_b
	.type	_Z5cond2PiS_b, @function
_Z5cond2PiS_b:
	lw	a0,0(a0)	# L1 load
	beq	a2,zero,.L51
	lw	a5,0(a1)
	add	a0,a0,a5
	ret
.L51:
	lw	a5,4(a1)
	lw	a2,8(a1)
	lw	a3,12(a1)
	lw	a4,16(a1)
	add	a5,a5,a2
	add	a5,a5,a3
	add	a5,a5,a4
	add	a0,a5,a0
	ret
	.size	_Z5cond2PiS_b, .-_Z5cond2PiS_b
	.align	2
	.globl	_Z5cond3PiS_b
	.type	_Z5cond3PiS_b, @function
_Z5cond3PiS_b:
	lw	a0,0(a0)	# L1 load
	beq	a2,zero,.L54
	lw	a5,0(a1)
	add	a0,a0,a5
	ret
.L54:
	lw	a5,4(a1)
	lw	a6,8(a1)
	lw	a2,12(a1)
	lw	a3,16(a1)
	mv	a0,a0	# GS L1 war
	lw	a4,20(a1)
	add	a5,a5,a6
	add	a5,a5,a2
	add	a5,a5,a3
	add	a5,a5,a4
	add	a0,a5,a0
	ret
	.size	_Z5cond3PiS_b, .-_Z5cond3PiS_b
	.align	2
	.globl	_Z7complexPiS_b
	.type	_Z7complexPiS_b, @function
_Z7complexPiS_b:
	mv	a5,a0
	lw	a0,0(a0)	# L1 load
	beq	a2,zero,.L57
	lw	a4,0(a1)
	add	a0,a0,a4
.L57:
	li	a4,5
	ble	a0,a4,.L58
	lw	a4,4(a1)
	add	a0,a0,a4
.L58:
	ble	a0,zero,.L56
	lw	a3,4(a1)
	li	a4,33
	add	a0,a0,a3
	ble	a0,a4,.L56
	addi	a1,a1,8
	li	a4,0
	li	a2,33
	j	.L60
.L61:
	lw	a3,0(a1)
	addi	a1,a1,4
	add	a0,a0,a3
	ble	a0,a2,.L56
.L60:
	lw	a3,0(a5)	# L1 load
	addi	a4,a4,1
	addi	a5,a5,4
	add	a0,a0,a3
	bgt	a0,a4,.L61
.L56:
	mv	a0,a0	# GS L1 war
	ret
	.size	_Z7complexPiS_b, .-_Z7complexPiS_b
	.align	2
	.globl	_Z8complex2PiS_ii
	.type	_Z8complex2PiS_ii, @function
_Z8complex2PiS_ii:
	mv	a4,a0
	lw	a0,0(a0)	# L1 load
	beq	a2,zero,.L66
	lw	a5,0(a1)
	add	a0,a0,a5
.L66:
	li	a5,5
	ble	a0,a5,.L67
	lw	a5,4(a1)
	add	a0,a0,a5
.L67:
	ble	a0,zero,.L65
	lw	a5,4(a1)
	add	a0,a0,a5
	and	a5,a2,a0
	beq	a5,zero,.L65
	addi	a1,a1,8
	li	a6,0
.L69:
	and	a5,a3,a0
	lw	a7,0(a4)	# L1 load
	addi	a6,a6,1
	beq	a5,zero,.L70
	add	a0,a0,a7
.L70:
	mv	a7,a7	# GS L1 war
	ble	a0,a6,.L65
	lw	a5,0(a1)
	addi	a4,a4,4
	addi	a1,a1,4
	add	a0,a0,a5
	and	a5,a2,a0
	bne	a5,zero,.L69
.L65:
	mv	a0,a0	# GS L1 war
	ret
	.size	_Z8complex2PiS_ii, .-_Z8complex2PiS_ii
	.align	2
	.globl	_Z11remove_war1v
	.type	_Z11remove_war1v, @function
_Z11remove_war1v:
	ret
	.size	_Z11remove_war1v, .-_Z11remove_war1v
	.align	2
	.globl	_Z11remove_war2v
	.type	_Z11remove_war2v, @function
_Z11remove_war2v:
	ret
	.size	_Z11remove_war2v, .-_Z11remove_war2v
	.align	2
	.globl	_Z11remove_war3v
	.type	_Z11remove_war3v, @function
_Z11remove_war3v:
	ret
	.size	_Z11remove_war3v, .-_Z11remove_war3v
	.align	2
	.globl	_Z11remove_war4PiS_
	.type	_Z11remove_war4PiS_, @function
_Z11remove_war4PiS_:
	lw	a5,0(a0)	# L1 load
	lw	a6,8(a1)
	lw	a0,0(a1)
	lw	a2,12(a1)
	lw	a3,16(a1)
	mv	a5,a5	# GS L1 war
	lw	a4,20(a1)
	add	a0,a0,a6
	add	a0,a0,a2
	add	a0,a0,a3
	add	a0,a0,a4
	add	a0,a0,a5
	ret
	.size	_Z11remove_war4PiS_, .-_Z11remove_war4PiS_
	.align	2
	.globl	_Z7cancel1PiS_
	.type	_Z7cancel1PiS_, @function
_Z7cancel1PiS_:
	lw	a5,0(a0)	# L1 load
	lw	a2,0(a1)
	lw	a3,8(a1)
	lw	a4,12(a1)
	lw	a0,16(a1)
	add	a5,a5,a2
	add	a5,a5,a3
	add	a5,a5,a4
	add	a0,a5,a0
	ret
	.size	_Z7cancel1PiS_, .-_Z7cancel1PiS_
	.align	2
	.globl	_Z7cancel2PiS_S_
	.type	_Z7cancel2PiS_S_, @function
_Z7cancel2PiS_S_:
	lw	a3,0(a0)	# L1 load
	lw	a4,0(a1)
	mv	a5,a0
	li	a0,0
	add	a4,a4,a3
	sw	a4,0(a2)
	lw	a3,4(a5)	# L1 load
	lw	a4,4(a1)
	add	a4,a4,a3
	sw	a4,4(a2)
	lw	a3,8(a5)	# L1 load
	lw	a4,8(a1)
	add	a4,a4,a3
	sw	a4,8(a2)
	lw	a3,12(a5)	# L1 load
	lw	a4,12(a1)
	add	a4,a4,a3
	sw	a4,12(a2)
	lw	a3,16(a5)	# L1 load
	lw	a4,16(a1)
	add	a4,a4,a3
	sw	a4,16(a2)
	lw	a4,20(a5)	# L1 load
	lw	a5,20(a1)
	add	a5,a5,a4
	sw	a5,20(a2)
	ret
	.size	_Z7cancel2PiS_S_, .-_Z7cancel2PiS_S_
	.align	2
	.globl	_Z7cancel3PiS_S_
	.type	_Z7cancel3PiS_S_, @function
_Z7cancel3PiS_S_:
	lw	a5,0(a0)	# L1 load
	lw	a2,4(a0)	# L1 load
	lw	t3,8(a0)	# L1 load
	addi	sp,sp,-16
	sw	s0,12(sp)
	lw	s0,12(a0)	# L1 load
	lw	t4,16(a0)	# L1 load
	lw	t5,24(a1)
	lw	t1,52(a1)
	lw	a3,48(a1)
	lw	a6,20(a0)	# L1 load
	lw	a7,76(a1)
	mv	a6,a6	# GS L1 war
	lw	a4,72(a1)
	add	a2,a5,a2
	lw	t2,80(a1)
	lw	a0,96(a1)
	lw	t0,100(a1)
	add	a2,a2,t5
	add	a2,a2,t3
	lw	t6,104(a1)
	lw	a5,120(a1)
	lw	t5,124(a1)
	add	a3,a3,t1
	lw	t3,108(a1)
	lw	t1,128(a1)
	add	a3,a3,a2
	add	a4,a4,a7
	add	a3,a3,s0
	lw	a7,132(a1)
	add	a4,a4,t2
	add	a0,a0,t0
	lw	a2,136(a1)
	add	a4,a4,a3
	add	a0,a0,t6
	add	a5,a5,t5
	lw	s0,12(sp)
	add	a4,a4,t4
	add	a0,a0,t3
	add	a5,a5,t1
	add	a0,a0,a4
	add	a5,a5,a7
	add	a0,a6,a0
	add	a5,a5,a2
	add	a0,a5,a0
	addi	sp,sp,16
	jr	ra
	.size	_Z7cancel3PiS_S_, .-_Z7cancel3PiS_S_
	.align	2
	.globl	_Z11war_at_exitPi
	.type	_Z11war_at_exitPi, @function
_Z11war_at_exitPi:
	lw	a0,0(a0)	# L1 load
	mv	a0,a0	# GS L1 war
	ret
	.size	_Z11war_at_exitPi, .-_Z11war_at_exitPi
	.align	2
	.globl	_Z12split_bb1_siPiS_b
	.type	_Z12split_bb1_siPiS_b, @function
_Z12split_bb1_siPiS_b:
	lw	a5,0(a0)	# L1 load
	addi	sp,sp,-16
	sw	a5,12(sp)
	beq	a2,zero,.L94
	lw	a0,0(a1)
	mv	a4,a5
.L95:
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
	addi	sp,sp,16
	jr	ra
.L94:
	lw	a4,0(a1)
	mv	a0,a5
	j	.L95
	.size	_Z12split_bb1_siPiS_b, .-_Z12split_bb1_siPiS_b
	.align	2
	.globl	_Z13split_bb1_usiPjPib
	.type	_Z13split_bb1_usiPjPib, @function
_Z13split_bb1_usiPjPib:
	lw	a5,0(a0)	# L1 load
	addi	sp,sp,-16
	sw	a5,12(sp)
	beq	a2,zero,.L98
	lw	a0,0(a1)
	mv	a4,a5
.L99:
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
	addi	sp,sp,16
	jr	ra
.L98:
	lw	a4,0(a1)
	mv	a0,a5
	j	.L99
	.size	_Z13split_bb1_usiPjPib, .-_Z13split_bb1_usiPjPib
	.align	2
	.globl	_Z12split_bb1_hiPsPib
	.type	_Z12split_bb1_hiPsPib, @function
_Z12split_bb1_hiPsPib:
	lh	a5,0(a0)	# L1 load
	addi	sp,sp,-16
	sw	a5,12(sp)
	beq	a2,zero,.L102
	lw	a0,0(a1)
	mv	a4,a5
.L103:
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
	addi	sp,sp,16
	jr	ra
.L102:
	lw	a4,0(a1)
	mv	a0,a5
	j	.L103
	.size	_Z12split_bb1_hiPsPib, .-_Z12split_bb1_hiPsPib
	.align	2
	.globl	_Z13split_bb1_uhiPtPib
	.type	_Z13split_bb1_uhiPtPib, @function
_Z13split_bb1_uhiPtPib:
	lhu	a5,0(a0)	# L1 load
	addi	sp,sp,-16
	sw	a5,12(sp)
	beq	a2,zero,.L106
	lw	a0,0(a1)
	mv	a4,a5
.L107:
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
	addi	sp,sp,16
	jr	ra
.L106:
	lw	a4,0(a1)
	mv	a0,a5
	j	.L107
	.size	_Z13split_bb1_uhiPtPib, .-_Z13split_bb1_uhiPtPib
	.align	2
	.globl	_Z12split_bb1_qiPcPib
	.type	_Z12split_bb1_qiPcPib, @function
_Z12split_bb1_qiPcPib:
	lb	a5,0(a0)	# L1 load
	addi	sp,sp,-16
	sw	a5,12(sp)
	beq	a2,zero,.L110
	lw	a0,0(a1)
	mv	a4,a5
.L111:
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
	addi	sp,sp,16
	jr	ra
.L110:
	lw	a4,0(a1)
	mv	a0,a5
	j	.L111
	.size	_Z12split_bb1_qiPcPib, .-_Z12split_bb1_qiPcPib
	.align	2
	.globl	_Z13split_bb1_uqiPhPib
	.type	_Z13split_bb1_uqiPhPib, @function
_Z13split_bb1_uqiPhPib:
	lbu	a5,0(a0)	# L1 load
	addi	sp,sp,-16
	sw	a5,12(sp)
	beq	a2,zero,.L114
	lw	a0,0(a1)
	mv	a4,a5
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
	addi	sp,sp,16
	jr	ra
.L114:
	lw	a4,0(a1)
	mv	a0,a5
	j	.L115
	.size	_Z13split_bb1_uqiPhPib, .-_Z13split_bb1_uqiPhPib
	.align	2
	.globl	_Z13split_bb1_ptrPPjPPib
	.type	_Z13split_bb1_ptrPPjPPib, @function
_Z13split_bb1_ptrPPjPPib:
	lw	a5,0(a0)	# L1 load
	addi	sp,sp,-16
	sw	a5,12(sp)
	beq	a2,zero,.L118
	lw	a0,0(a1)
	mv	a4,a5
.L119:
	lw	a3,16(a1)
	lw	a5,12(a1)
	lw	a7,20(a1)
	lw	a2,24(a1)
	lw	a6,0(a3)
	lw	a5,0(a5)
	lw	a3,28(a1)
	lw	a1,0(a7)
	lw	a2,0(a2)
	lw	a3,0(a3)
	add	a5,a5,a6
	lw	a4,0(a4)
	add	a5,a5,a1
	lw	a0,0(a0)
	add	a5,a5,a2
	add	a5,a5,a3
	add	a5,a5,a4
	add	a0,a5,a0
	addi	sp,sp,16
	jr	ra
.L118:
	lw	a4,0(a1)
	mv	a0,a5
	j	.L119
	.size	_Z13split_bb1_ptrPPjPPib, .-_Z13split_bb1_ptrPPjPPib
	.align	2
	.globl	_Z18split_bb1_baselinePViS0_b
	.type	_Z18split_bb1_baselinePViS0_b, @function
_Z18split_bb1_baselinePViS0_b:
	beq	a2,zero,.L122
	lw	a4,0(a0)
	lw	a0,0(a1)
.L123:
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
.L122:
	lw	a4,0(a1)
	lw	a0,0(a0)
	j	.L123
	.size	_Z18split_bb1_baselinePViS0_b, .-_Z18split_bb1_baselinePViS0_b
	.align	2
	.globl	_Z9split_bb2PiS_b
	.type	_Z9split_bb2PiS_b, @function
_Z9split_bb2PiS_b:
	lw	a6,0(a0)	# L1 load
	lw	a3,0(a1)
	beq	a2,zero,.L126
	lw	a0,4(a0)	# L1 load
.L125:
	lw	a4,12(a1)
	lw	a5,16(a1)
	lw	t1,20(a1)
	mv	a0,a0	# GS L1 war
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
.L126:
	li	a0,0
	mv	a6,a6	# GS L1 war
	j	.L125
	.size	_Z9split_bb2PiS_b, .-_Z9split_bb2PiS_b
	.align	2
	.globl	_Z7warningPiS_
	.type	_Z7warningPiS_, @function
_Z7warningPiS_:
	lw	a5,0(a0)	# L1 load
	lw	a4,4(a0)	# L1 load
	lw	a0,0(a1)
	add	a5,a5,a4
	add	a0,a5,a0
	ret
	.size	_Z7warningPiS_, .-_Z7warningPiS_
	.section	.text.startup,"ax",@progbits
	.align	2
	.globl	main
	.type	main, @function
main:
	li	a0,0
	ret
	.size	main, .-main
	.ident	"GCC: (GNU) 10.2.0"
