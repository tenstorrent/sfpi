	.file	"schedule.cc"
	.option nopic
	.attribute arch, "rv32i2p0_y2p0"
	.attribute unaligned_access, 0
	.attribute stack_align, 16
	.text
	.align	1
	.globl	_Z9basic_nopi
	.type	_Z9basic_nopi, @function
_Z9basic_nopi:
	SFPLOADI	L0, 17136, 0
	SFPSTORE	48, L0, 0
	TTNOP
	SFPLOAD	L0, 48, 1
	SFPLOADI	L0, 17136, 0
	SFPSTORE	48, L0, 0
	TTNOP
	addi	a5,a0,-23
	lui	a4,%hi(_ZN7ckernel13instrn_bufferE)
	li	a3,65536
	addi	a3,a3,-1
	lw	a4,%lo(_ZN7ckernel13instrn_bufferE)(a4)
	slli	a5,a5,2
	and	a5,a5,a3
	li	a3, 1879113728	# 70010000
	add	a3,a5,a3
	sw	a3, 0(a4)	# Op(0x70) - d(lr0)
	SFPLOADI	L0, 17154, 0
	li	a3, 1912602624	# 72000000
	add	a3,a5,a3
	sw	a3, 0(a4)	# Op(0x72) s(lr0)
	TTNOP
	SFPLOAD	L0, 48, 1
	SFPLOADI	L0, 17164, 0
	li	a3, 1912602624	# 72000000
	add	a3,a5,a3
	sw	a3, 0(a4)	# Op(0x72) s(lr0)
	TTNOP
	li	a3, 1879113728	# 70010000
	add	a5,a5,a3
	sw	a5, 0(a4)	# Op(0x70) - d(lr0)
	ret
	.size	_Z9basic_nopi, .-_Z9basic_nopi
	.align	1
	.globl	_Z18basic_incrwc_nonopi
	.type	_Z18basic_incrwc_nonopi, @function
_Z18basic_incrwc_nonopi:
	SFPLOADI	L0, 17136, 0
	SFPSTORE	48, L0, 0
	TTINCRWC	0, 4, 0, 0
	SFPLOAD	L0, 48, 1
	SFPLOADI	L0, 17136, 0
	SFPSTORE	48, L0, 0
	TTINCRWC	0, 4, 0, 0
	addi	a5,a0,-23
	lui	a4,%hi(_ZN7ckernel13instrn_bufferE)
	li	a3,65536
	addi	a3,a3,-1
	lw	a4,%lo(_ZN7ckernel13instrn_bufferE)(a4)
	slli	a5,a5,2
	and	a5,a5,a3
	li	a3, 1879113728	# 70010000
	add	a3,a5,a3
	sw	a3, 0(a4)	# Op(0x70) - d(lr0)
	SFPLOADI	L0, 17154, 0
	li	a3, 1912602624	# 72000000
	add	a3,a5,a3
	sw	a3, 0(a4)	# Op(0x72) s(lr0)
	TTINCRWC	0, 4, 0, 0
	SFPLOAD	L0, 48, 1
	SFPLOADI	L0, 17164, 0
	li	a3, 1912602624	# 72000000
	add	a3,a5,a3
	sw	a3, 0(a4)	# Op(0x72) s(lr0)
	TTINCRWC	0, 4, 0, 0
	li	a3, 1879113728	# 70010000
	add	a5,a5,a3
	sw	a5, 0(a4)	# Op(0x70) - d(lr0)
	ret
	.size	_Z18basic_incrwc_nonopi, .-_Z18basic_incrwc_nonopi
	.align	1
	.globl	_Z8loop_nopv
	.type	_Z8loop_nopv, @function
_Z8loop_nopv:
	SFPLOADI	L0, 16256, 0
	SFPSTORE	0, L0, 0
	TTNOP
	li	a5,4
.L5:
	SFPLOAD	L0, 48, 1
	SFPSTORE	0, L0, 0
	TTNOP
	addi	a5,a5,-1
	bne	a5,zero,.L5
	ret
	.size	_Z8loop_nopv, .-_Z8loop_nopv
	.align	1
	.globl	_Z10loop_nonopv
	.type	_Z10loop_nonopv, @function
_Z10loop_nonopv:
	SFPLOADI	L0, 16256, 0
	SFPSTORE	0, L0, 0
	SFPLOADI	L0, 16256, 0
	SFPLOAD	L1, 0, 1
	SFPSTORE	0, L0, 0
	li	a5,3
.L8:
	SFPLOADI	L0, 16256, 0
	SFPLOAD	L1, 0, 1
	SFPSTORE	0, L0, 0
	addi	a5,a5,-1
	bne	a5,zero,.L8
	SFPSTORE	0, L1, 0
	ret
	.size	_Z10loop_nonopv, .-_Z10loop_nonopv
	.align	1
	.globl	_Z8loop_nopi
	.type	_Z8loop_nopi, @function
_Z8loop_nopi:
	SFPLOADI	L0, 16256, 0
	SFPSTORE	0, L0, 0
	TTNOP
	li	a4,65536
	addi	a4,a4,-1
	slli	a5,a0,2
	and	a5,a5,a4
	li	a3, 1879113728	# 70010000
	li	a4,4
	lui	a2,%hi(_ZN7ckernel13instrn_bufferE)
	add	a5,a5,a3
.L11:
	lw	a3,%lo(_ZN7ckernel13instrn_bufferE)(a2)
	sw	a5, 0(a3)	# Op(0x70) - d(lr0)
	SFPSTORE	0, L0, 0
	TTNOP
	addi	a4,a4,-1
	bne	a4,zero,.L11
	ret
	.size	_Z8loop_nopi, .-_Z8loop_nopi
	.globl	_ZN7ckernel13instrn_bufferE
	.section	.sbss,"aw",@nobits
	.align	2
	.type	_ZN7ckernel13instrn_bufferE, @object
	.size	_ZN7ckernel13instrn_bufferE, 4
_ZN7ckernel13instrn_bufferE:
	.zero	4
	.ident	"GCC: (GNU) 10.2.0"
