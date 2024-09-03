	.file	"latency.cc"
	.option nopic
	.attribute arch, "rv32i2p0_y"
	.attribute unaligned_access, 0
	.attribute stack_align, 16
	.text
	.globl	__mulsi3
	.align	1
	.globl	_Z17mul_latency_chainiiiiii
	.type	_Z17mul_latency_chainiiiiii, @function
_Z17mul_latency_chainiiiiii:
	addi	sp,sp,-32
	sw	s1,20(sp)
	sw	s3,12(sp)
	mv	s1,a2
	mv	s3,a3
	sw	s0,24(sp)
	sw	s2,16(sp)
	add	s1,s1,s3
	mv	s2,a5
	mv	s0,a4
	sw	ra,28(sp)
	add	s0,s0,s2
	call	__mulsi3
	add	a0,a0,s1
	lw	ra,28(sp)
	add	a0,a0,s0
	lw	s0,24(sp)
	lw	s1,20(sp)
	lw	s2,16(sp)
	lw	s3,12(sp)
	addi	sp,sp,32
	jr	ra
	.size	_Z17mul_latency_chainiiiiii, .-_Z17mul_latency_chainiiiiii
	.globl	__divsi3
	.align	1
	.globl	_Z17div_latency_chainiiiiii
	.type	_Z17div_latency_chainiiiiii, @function
_Z17div_latency_chainiiiiii:
	addi	sp,sp,-32
	sw	s3,12(sp)
	sw	s4,8(sp)
	mv	s3,a3
	mv	s4,a2
	sw	s0,24(sp)
	sw	s1,20(sp)
	sw	s2,16(sp)
	sw	s5,4(sp)
	sw	ra,28(sp)
	mv	s0,a0
	mv	s2,a1
	mv	s1,a4
	mv	s5,a5
	add	s3,s4,s3
	call	__divsi3
	add	a1,s0,s2
	add	a3,a0,s3
	add	s0,s0,s4
	add	a3,a3,a1
	lw	ra,28(sp)
	add	a3,a3,s0
	lw	s0,24(sp)
	add	a0,s1,s5
	lw	s2,16(sp)
	lw	s1,20(sp)
	lw	s3,12(sp)
	lw	s4,8(sp)
	lw	s5,4(sp)
	add	a0,a3,a0
	addi	sp,sp,32
	jr	ra
	.size	_Z17div_latency_chainiiiiii, .-_Z17div_latency_chainiiiiii
	.globl	__modsi3
	.align	1
	.globl	_Z17mod_latency_chainiiiiii
	.type	_Z17mod_latency_chainiiiiii, @function
_Z17mod_latency_chainiiiiii:
	addi	sp,sp,-32
	sw	s3,12(sp)
	sw	s4,8(sp)
	mv	s3,a3
	mv	s4,a2
	sw	s0,24(sp)
	sw	s1,20(sp)
	sw	s2,16(sp)
	sw	s5,4(sp)
	sw	ra,28(sp)
	mv	s0,a0
	mv	s2,a1
	mv	s1,a4
	mv	s5,a5
	add	s3,s4,s3
	call	__modsi3
	add	a1,s0,s2
	add	a3,a0,s3
	add	s0,s0,s4
	add	a3,a3,a1
	lw	ra,28(sp)
	add	a3,a3,s0
	lw	s0,24(sp)
	add	a0,s1,s5
	lw	s2,16(sp)
	lw	s1,20(sp)
	lw	s3,12(sp)
	lw	s4,8(sp)
	lw	s5,4(sp)
	add	a0,a3,a0
	addi	sp,sp,32
	jr	ra
	.size	_Z17mod_latency_chainiiiiii, .-_Z17mod_latency_chainiiiiii
	.section	.text.startup,"ax",@progbits
	.align	1
	.globl	main
	.type	main, @function
main:
	li	a0,0
	ret
	.size	main, .-main
	.ident	"GCC: (gcdc243316e1) 12.2.0"
