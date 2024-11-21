	.file	"rv32_and_sfpu.cc"
	.option nopic
	.attribute arch, "rv32i2p0_xttgs1p0"
	.attribute unaligned_access, 0
	.attribute stack_align, 16
	.text
	.align	2
	.globl	_Z9test_funcib
	.type	_Z9test_funcib, @function
_Z9test_funcib:
	SFPENCC	3, 10
	TTNOP
	addi	a5,a0,-1
	ble	a0,zero,.L1
	li	a4,-1
	bne	a1,zero,.L3
.L4:
	SFPLOAD	L1, 0, 1
	SFPLOAD	L0, 1, 1
	SFPMAD	L2, L1, L0, L4, 1
	TTNOP
	TTNOP
	SFPMAD	L1, L0, L1, L4, 1
	TTNOP
	TTNOP
	SFPMAD	L0, L0, L2, L4, 1
	TTNOP
	TTNOP
	SFPSTORE	3, L2, 0
	SFPSTORE	4, L1, 0
	SFPSTORE	5, L0, 0
	TTNOP
	addi	a5,a5,-1
	bne	a5,a4,.L4
	ret
.L3:
	SFPLOAD	L2, 0, 1
	SFPLOAD	L1, 1, 1
	SFPMAD	L0, L2, L1, L4, 1
	TTNOP
	TTNOP
	SFPMAD	L2, L1, L2, L4, 1
	TTNOP
	TTNOP
	SFPMAD	L1, L1, L0, L4, 1
	TTNOP
	TTNOP
	SFPSTORE	3, L0, 0
	SFPSTORE	4, L2, 0
	SFPSTORE	5, L1, 0
	SFPLOADI	L1, 16128, 0
	SFPMAD	L0, L0, L1, L4, 1
	TTNOP
	TTNOP
	SFPSTORE	4, L0, 0
	TTNOP
	addi	a5,a5,-1
	bne	a5,a4,.L3
.L1:
	ret
	.size	_Z9test_funcib, .-_Z9test_funcib
	.section	.text.startup,"ax",@progbits
	.align	2
	.globl	main
	.type	main, @function
main:
	addi	sp,sp,-16
	snez	a1,a0
	sw	ra,12(sp)
	call	_Z9test_funcib
	lw	ra,12(sp)
	li	a0,0
	addi	sp,sp,16
	jr	ra
	.size	main, .-main
	.ident	"GCC: (g323f2891276) 12.2.0"
