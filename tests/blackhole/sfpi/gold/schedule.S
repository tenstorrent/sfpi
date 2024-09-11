	.file	"schedule.cc"
	.option nopic
	.attribute arch, "rv32i2p0_m2p0_l2p0"
	.attribute unaligned_access, 0
	.attribute stack_align, 16
	.text
	.align	2
	.globl	_Z13basic_add_nopv
	.type	_Z13basic_add_nopv, @function
_Z13basic_add_nopv:
	SFPLOAD	L1, 0, 0, 7
	SFPLOAD	L0, 0, 0, 7
	SFPADD	L0, L10, L0, L1, 0
	SFPNOP
	SFPSTORE	0, L0, 0, 7
	ret
	.size	_Z13basic_add_nopv, .-_Z13basic_add_nopv
	.align	2
	.globl	_Z13basic_mul_nopv
	.type	_Z13basic_mul_nopv, @function
_Z13basic_mul_nopv:
	SFPLOAD	L1, 0, 0, 7
	SFPLOAD	L0, 0, 0, 7
	SFPMUL	L0, L0, L1, L9, 0
	SFPNOP
	SFPSTORE	0, L0, 0, 7
	ret
	.size	_Z13basic_mul_nopv, .-_Z13basic_mul_nopv
	.align	2
	.globl	_Z14basic_addi_nopv
	.type	_Z14basic_addi_nopv, @function
_Z14basic_addi_nopv:
	SFPLOADI	L0, 16256, 0
	SFPADDI	L0, 16256, 0
	SFPNOP
	SFPSTORE	0, L0, 0, 7
	ret
	.size	_Z14basic_addi_nopv, .-_Z14basic_addi_nopv
	.align	2
	.globl	_Z14basic_muli_nopv
	.type	_Z14basic_muli_nopv, @function
_Z14basic_muli_nopv:
	SFPLOADI	L0, 16256, 0
	SFPMULI	L0, 16256, 0
	SFPNOP
	SFPSTORE	0, L0, 0, 7
	ret
	.size	_Z14basic_muli_nopv, .-_Z14basic_muli_nopv
	.align	2
	.globl	_Z18basic_addi_nop_immi
	.type	_Z18basic_addi_nop_immi, @function
_Z18basic_addi_nop_immi:
	SFPLOADI	L0, 16256, 0
	SFPADDI	L0, 16256, 0
	SFPNOP
	li	a4,16384
	slli	a5,a0,1
	addi	a4,a4,-1
	and	a5,a5,a4
	lui	a4,%hi(_ZN7ckernel13instrn_bufferE)
	li	a3, 1912717312	# 7201c000
	lw	a4,%lo(_ZN7ckernel13instrn_bufferE)(a4)
	add	a5,a5,a3
	sw	a5, 0(a4)	# Op(0x72) s(lr0)
	ret
	.size	_Z18basic_addi_nop_immi, .-_Z18basic_addi_nop_immi
	.align	2
	.globl	_Z8loop_nopv
	.type	_Z8loop_nopv, @function
_Z8loop_nopv:
	SFPLOAD	L0, 0, 0, 7
	SFPMULI	L0, 16384, 0
	SFPNOP
	li	a5,4
.L8:
	SFPSTORE	0, L0, 0, 7
	SFPMULI	L0, 16384, 0
	SFPNOP
	addi	a5,a5,-1
	bne	a5,zero,.L8
	ret
	.size	_Z8loop_nopv, .-_Z8loop_nopv
	.align	2
	.globl	_Z11loop_no_nopv
	.type	_Z11loop_no_nopv, @function
_Z11loop_no_nopv:
	SFPLOAD	L0, 0, 0, 7
	SFPMULI	L0, 16384, 0
	li	a5,4
.L11:
	SFPLOAD	L1, 2, 0, 7
	SFPSTORE	0, L1, 0, 7
	SFPMULI	L0, 16384, 0
	addi	a5,a5,-1
	bne	a5,zero,.L11
	ret
	.size	_Z11loop_no_nopv, .-_Z11loop_no_nopv
	.globl	_ZN7ckernel13instrn_bufferE
	.section	.sbss,"aw",@nobits
	.align	2
	.type	_ZN7ckernel13instrn_bufferE, @object
	.size	_ZN7ckernel13instrn_bufferE, 4
_ZN7ckernel13instrn_bufferE:
	.zero	4
	.ident	"GCC: (gcdc243316e1) 12.2.0"
