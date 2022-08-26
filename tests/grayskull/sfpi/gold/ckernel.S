	.file	"ckernel.cc"
	.option nopic
	.attribute arch, "rv32i2p0_y2p0"
	.attribute unaligned_access, 0
	.attribute stack_align, 16
	.text
	.align	1
	.type	_ZN7ckernel4sfpu21calculate_exponentialILb1ELb1ELb0EEEvs.isra.0, @function
_ZN7ckernel4sfpu21calculate_exponentialILb1ELb1ELb0EEEvs.isra.0:
	li	a5,4
.L2:
	SFPLOAD	L1, 1, 0
	SFPMAD	L1, L7, L0, L1, 0
	SFPNOP
	SFPNOP
	SFPIADD	0, L2, L1, 4
	SFPNOP
	SFPNOP
	SFPSHFT	8, L0, L1, 1
	SFPNOP
	SFPNOP
	SFPSTORE	L1, 0, 0
	SFPSETCC	0, L1, 0
	SFPNOP
	SFPSTORE	L4, 0, 0
	SFPENCC	3, 10
	SFPNOP
	SFPINCRWC	0, 4, 0, 0
	addi	a5,a5,-1
	bne	a5,zero,.L2
	ret
	.size	_ZN7ckernel4sfpu21calculate_exponentialILb1ELb1ELb0EEEvs.isra.0, .-_ZN7ckernel4sfpu21calculate_exponentialILb1ELb1ELb0EEEvs.isra.0
	.align	1
	.type	_ZN7ckernel4sfpu21calculate_exponentialILb1ELb0ELb0EEEvs.isra.0, @function
_ZN7ckernel4sfpu21calculate_exponentialILb1ELb0ELb0EEEvs.isra.0:
	li	a5,4
.L6:
	SFPLOAD	L1, 1, 0
	SFPMAD	L1, L7, L0, L1, 0
	SFPNOP
	SFPNOP
	SFPIADD	0, L2, L1, 4
	SFPNOP
	SFPNOP
	SFPSHFT	8, L0, L1, 1
	SFPNOP
	SFPNOP
	SFPSTORE	L1, 0, 0
	SFPINCRWC	0, 4, 0, 0
	addi	a5,a5,-1
	bne	a5,zero,.L6
	ret
	.size	_ZN7ckernel4sfpu21calculate_exponentialILb1ELb0ELb0EEEvs.isra.0, .-_ZN7ckernel4sfpu21calculate_exponentialILb1ELb0ELb0EEEvs.isra.0
	.align	1
	.type	_ZN7ckernel4sfpu14calculate_compILb0EL8SfpuType11EEEvm.isra.0, @function
_ZN7ckernel4sfpu14calculate_compILb0EL8SfpuType11EEEvm.isra.0:
	SFPLOAD	L0, 1, 0
	SFPSETCC	0, L0, 0
	SFPNOP
	SFPLOADI	L0, 0, 16256
	SFPLOADI	L1, 0, 16256
	SFPCOMPC
	SFPNOP
	SFPLOADI	L0, 0, 0
	SFPLOADI	L1, 0, 0
	SFPENCC	3, 10
	SFPNOP
	SFPOR	L1, L0
	SFPNOP
	SFPNOP
	SFPSTORE	L0, 0, 0
	SFPINCRWC	0, 4, 0, 0
	SFPLOAD	L0, 1, 0
	SFPSETCC	0, L0, 0
	SFPNOP
	SFPLOADI	L0, 0, 16256
	SFPLOADI	L1, 0, 16256
	SFPCOMPC
	SFPNOP
	SFPLOADI	L0, 0, 0
	SFPLOADI	L1, 0, 0
	SFPENCC	3, 10
	SFPNOP
	SFPOR	L1, L0
	SFPNOP
	SFPNOP
	SFPSTORE	L0, 0, 0
	SFPINCRWC	0, 4, 0, 0
	SFPLOAD	L0, 1, 0
	SFPSETCC	0, L0, 0
	SFPNOP
	SFPLOADI	L0, 0, 16256
	SFPLOADI	L1, 0, 16256
	SFPCOMPC
	SFPNOP
	SFPLOADI	L0, 0, 0
	SFPLOADI	L1, 0, 0
	SFPENCC	3, 10
	SFPNOP
	SFPOR	L1, L0
	SFPNOP
	SFPNOP
	SFPSTORE	L0, 0, 0
	SFPINCRWC	0, 4, 0, 0
	SFPLOAD	L0, 1, 0
	SFPSETCC	0, L0, 0
	SFPNOP
	SFPLOADI	L0, 0, 16256
	SFPLOADI	L1, 0, 16256
	SFPCOMPC
	SFPNOP
	SFPLOADI	L0, 0, 0
	SFPLOADI	L1, 0, 0
	SFPENCC	3, 10
	SFPNOP
	SFPOR	L1, L0
	SFPNOP
	SFPNOP
	SFPSTORE	L0, 0, 0
	SFPINCRWC	0, 4, 0, 0
	ret
	.size	_ZN7ckernel4sfpu14calculate_compILb0EL8SfpuType11EEEvm.isra.0, .-_ZN7ckernel4sfpu14calculate_compILb0EL8SfpuType11EEEvm.isra.0
	.align	1
	.type	_ZN7ckernel4sfpu14calculate_compILb0EL8SfpuType9EEEvm.isra.0, @function
_ZN7ckernel4sfpu14calculate_compILb0EL8SfpuType9EEEvm.isra.0:
	SFPLOAD	L0, 1, 0
	SFPSETCC	0, L0, 0
	SFPNOP
	SFPLOADI	L0, 0, 0
	SFPLOADI	L1, 0, 0
	SFPCOMPC
	SFPNOP
	SFPLOADI	L0, 0, 16256
	SFPLOADI	L1, 0, 16256
	SFPENCC	3, 10
	SFPNOP
	SFPAND	L1, L0
	SFPNOP
	SFPNOP
	SFPSTORE	L0, 0, 0
	SFPINCRWC	0, 4, 0, 0
	SFPLOAD	L0, 1, 0
	SFPSETCC	0, L0, 0
	SFPNOP
	SFPLOADI	L0, 0, 0
	SFPLOADI	L1, 0, 0
	SFPCOMPC
	SFPNOP
	SFPLOADI	L0, 0, 16256
	SFPLOADI	L1, 0, 16256
	SFPENCC	3, 10
	SFPNOP
	SFPAND	L1, L0
	SFPNOP
	SFPNOP
	SFPSTORE	L0, 0, 0
	SFPINCRWC	0, 4, 0, 0
	SFPLOAD	L0, 1, 0
	SFPSETCC	0, L0, 0
	SFPNOP
	SFPLOADI	L0, 0, 0
	SFPLOADI	L1, 0, 0
	SFPCOMPC
	SFPNOP
	SFPLOADI	L0, 0, 16256
	SFPLOADI	L1, 0, 16256
	SFPENCC	3, 10
	SFPNOP
	SFPAND	L1, L0
	SFPNOP
	SFPNOP
	SFPSTORE	L0, 0, 0
	SFPINCRWC	0, 4, 0, 0
	SFPLOAD	L0, 1, 0
	SFPSETCC	0, L0, 0
	SFPNOP
	SFPLOADI	L0, 0, 0
	SFPLOADI	L1, 0, 0
	SFPCOMPC
	SFPNOP
	SFPLOADI	L0, 0, 16256
	SFPLOADI	L1, 0, 16256
	SFPENCC	3, 10
	SFPNOP
	SFPAND	L1, L0
	SFPNOP
	SFPNOP
	SFPSTORE	L0, 0, 0
	SFPINCRWC	0, 4, 0, 0
	ret
	.size	_ZN7ckernel4sfpu14calculate_compILb0EL8SfpuType9EEEvm.isra.0, .-_ZN7ckernel4sfpu14calculate_compILb0EL8SfpuType9EEEvm.isra.0
	.align	1
	.type	_ZN7ckernel4sfpu14calculate_compILb0EL8SfpuType7EEEvm.isra.0, @function
_ZN7ckernel4sfpu14calculate_compILb0EL8SfpuType7EEEvm.isra.0:
	SFPLOAD	L0, 1, 0
	SFPSETCC	0, L0, 0
	SFPNOP
	SFPLOADI	L0, 0, 0
	SFPCOMPC
	SFPNOP
	SFPLOADI	L0, 0, 16256
	SFPENCC	3, 10
	SFPNOP
	SFPSTORE	L0, 0, 0
	SFPINCRWC	0, 4, 0, 0
	SFPLOAD	L0, 1, 0
	SFPSETCC	0, L0, 0
	SFPNOP
	SFPLOADI	L0, 0, 0
	SFPCOMPC
	SFPNOP
	SFPLOADI	L0, 0, 16256
	SFPENCC	3, 10
	SFPNOP
	SFPSTORE	L0, 0, 0
	SFPINCRWC	0, 4, 0, 0
	SFPLOAD	L0, 1, 0
	SFPSETCC	0, L0, 0
	SFPNOP
	SFPLOADI	L0, 0, 0
	SFPCOMPC
	SFPNOP
	SFPLOADI	L0, 0, 16256
	SFPENCC	3, 10
	SFPNOP
	SFPSTORE	L0, 0, 0
	SFPINCRWC	0, 4, 0, 0
	SFPLOAD	L0, 1, 0
	SFPSETCC	0, L0, 0
	SFPNOP
	SFPLOADI	L0, 0, 0
	SFPCOMPC
	SFPNOP
	SFPLOADI	L0, 0, 16256
	SFPENCC	3, 10
	SFPNOP
	SFPSTORE	L0, 0, 0
	SFPINCRWC	0, 4, 0, 0
	ret
	.size	_ZN7ckernel4sfpu14calculate_compILb0EL8SfpuType7EEEvm.isra.0, .-_ZN7ckernel4sfpu14calculate_compILb0EL8SfpuType7EEEvm.isra.0
	.align	1
	.type	_ZN7ckernel4sfpu21calculate_exponentialILb0ELb1ELb0EEEvs.isra.0, @function
_ZN7ckernel4sfpu21calculate_exponentialILb0ELb1ELb0EEEvs.isra.0:
	li	a4,4
.L13:
	SFPLOAD	L0, 1, 0
	SFPSETSGN	0, L0, L0, 1
	SFPNOP
	SFPNOP
	SFPEXEXP	L0, L1, 10
	SFPNOP
	SFPNOP
	SFPNOP
	SFPSETEXP	126, L0, L0, 1
	SFPNOP
	SFPNOP
	SFPENCC	3, 10
	SFPNOP
	SFPLOADI	L2, 0, 16221
	SFPMAD	L0, L8, L2, L2, 0
	SFPNOP
	SFPNOP
	SFPMAD	L0, L2, L10, L0, 0
	SFPNOP
	SFPNOP
	SFPSETCC	0, L1, 4
	SFPNOP
	SFPMUL	L0, L0, L4, L0, 0
	SFPNOP
	SFPNOP
	li	a5,7
.L12:
	SFPIADD	-1, L1, L1, 9
	SFPNOP
	SFPNOP
	SFPNOP
	SFPMUL	L0, L0, L4, L0, 0
	SFPNOP
	SFPNOP
	addi	a5,a5,-1
	bne	a5,zero,.L12
	SFPENCC	3, 10
	SFPNOP
	SFPLOAD	L1, 1, 0
	SFPSTORE	L0, 0, 0
	SFPSETCC	0, L1, 0
	SFPNOP
	SFPSETSGN	1, L0, L0, 1
	SFPNOP
	SFPNOP
	SFPSETEXP	126, L0, L0, 1
	SFPNOP
	SFPNOP
	SFPLOADI	L2, 0, 16384
	SFPMAD	L0, L7, L2, L1, 0
	SFPNOP
	SFPNOP
	SFPMUL	L7, L1, L4, L1, 0
	SFPNOP
	SFPNOP
	SFPMAD	L0, L1, L2, L3, 0
	SFPNOP
	SFPNOP
	SFPMUL	L1, L3, L4, L1, 0
	SFPNOP
	SFPNOP
	SFPMAD	L0, L1, L2, L0, 0
	SFPNOP
	SFPNOP
	SFPMUL	L1, L0, L4, L0, 0
	SFPNOP
	SFPNOP
	SFPEXEXP	L0, L2, 0
	SFPNOP
	SFPNOP
	SFPLOAD	L1, 1, 0
	SFPEXEXP	L1, L1, 0
	SFPNOP
	SFPNOP
	SFPIADD	0, L2, L1, 6
	SFPNOP
	SFPNOP
	SFPPUSHC
	SFPIADD	126, L1, L1, 1
	SFPNOP
	SFPNOP
	SFPNOP
	SFPLOADI	L0, 0, 0
	SFPLOADI	L1, 4, 0
	SFPPOPC
	SFPNOP
	SFPSETEXP	0, L0, L1, 0
	SFPNOP
	SFPNOP
	SFPSTORE	L1, 0, 0
	SFPENCC	3, 10
	SFPNOP
	SFPINCRWC	0, 4, 0, 0
	addi	a4,a4,-1
	bne	a4,zero,.L13
	ret
	.size	_ZN7ckernel4sfpu21calculate_exponentialILb0ELb1ELb0EEEvs.isra.0, .-_ZN7ckernel4sfpu21calculate_exponentialILb0ELb1ELb0EEEvs.isra.0
	.align	1
	.type	_ZN7ckernel4sfpu21calculate_exponentialILb0ELb0ELb0EEEvs.isra.0, @function
_ZN7ckernel4sfpu21calculate_exponentialILb0ELb0ELb0EEEvs.isra.0:
	li	a4,4
.L18:
	SFPLOAD	L0, 1, 0
	SFPSETSGN	0, L0, L0, 1
	SFPNOP
	SFPNOP
	SFPEXEXP	L0, L1, 10
	SFPNOP
	SFPNOP
	SFPNOP
	SFPSETEXP	126, L0, L0, 1
	SFPNOP
	SFPNOP
	SFPENCC	3, 10
	SFPNOP
	SFPLOADI	L2, 0, 16221
	SFPMAD	L0, L8, L2, L2, 0
	SFPNOP
	SFPNOP
	SFPMAD	L0, L2, L10, L0, 0
	SFPNOP
	SFPNOP
	SFPSETCC	0, L1, 4
	SFPNOP
	SFPMUL	L0, L0, L4, L0, 0
	SFPNOP
	SFPNOP
	li	a5,7
.L17:
	SFPIADD	-1, L1, L1, 9
	SFPNOP
	SFPNOP
	SFPNOP
	SFPMUL	L0, L0, L4, L0, 0
	SFPNOP
	SFPNOP
	addi	a5,a5,-1
	bne	a5,zero,.L17
	SFPENCC	3, 10
	SFPNOP
	SFPLOAD	L1, 1, 0
	SFPSTORE	L0, 0, 0
	SFPSETCC	0, L1, 0
	SFPNOP
	SFPSETSGN	1, L0, L0, 1
	SFPNOP
	SFPNOP
	SFPSETEXP	126, L0, L0, 1
	SFPNOP
	SFPNOP
	SFPLOADI	L2, 0, 16384
	SFPMAD	L0, L7, L2, L1, 0
	SFPNOP
	SFPNOP
	SFPMUL	L7, L1, L4, L1, 0
	SFPNOP
	SFPNOP
	SFPMAD	L0, L1, L2, L3, 0
	SFPNOP
	SFPNOP
	SFPMUL	L1, L3, L4, L1, 0
	SFPNOP
	SFPNOP
	SFPMAD	L0, L1, L2, L0, 0
	SFPNOP
	SFPNOP
	SFPMUL	L1, L0, L4, L0, 0
	SFPNOP
	SFPNOP
	SFPEXEXP	L0, L2, 0
	SFPNOP
	SFPNOP
	SFPLOAD	L1, 1, 0
	SFPEXEXP	L1, L1, 0
	SFPNOP
	SFPNOP
	SFPIADD	0, L2, L1, 6
	SFPNOP
	SFPNOP
	SFPPUSHC
	SFPIADD	126, L1, L1, 1
	SFPNOP
	SFPNOP
	SFPNOP
	SFPLOADI	L0, 0, 0
	SFPLOADI	L1, 4, 0
	SFPPOPC
	SFPNOP
	SFPSETEXP	0, L0, L1, 0
	SFPNOP
	SFPNOP
	SFPSTORE	L1, 0, 0
	SFPENCC	3, 10
	SFPNOP
	SFPINCRWC	0, 4, 0, 0
	addi	a4,a4,-1
	bne	a4,zero,.L18
	ret
	.size	_ZN7ckernel4sfpu21calculate_exponentialILb0ELb0ELb0EEEvs.isra.0, .-_ZN7ckernel4sfpu21calculate_exponentialILb0ELb0ELb0EEEvs.isra.0
	.align	1
	.type	_ZN7ckernel4sfpu13calculate_logILb1ELb0EEEvm.isra.0, @function
_ZN7ckernel4sfpu13calculate_logILb1ELb0EEEvm.isra.0:
	li	a5,4
.L22:
	SFPLOAD	L3, 1, 0
	SFPSETEXP	127, L3, L0, 1
	SFPNOP
	SFPNOP
	SFPLOADI	L1, 1, 11973
	SFPLOADI	L2, 1, -17997
	SFPMAD	L0, L1, L2, L1, 0
	SFPNOP
	SFPNOP
	SFPLOADI	L2, 1, 16428
	SFPMAD	L0, L1, L2, L1, 0
	SFPNOP
	SFPNOP
	SFPLOADI	L2, 1, -16921
	SFPMAD	L0, L1, L2, L2, 0
	SFPNOP
	SFPNOP
	SFPLOADI	L0, 4, 0
	SFPSETCC	0, L3, 2
	SFPNOP
	SFPEXEXP	L3, L0, 2
	SFPNOP
	SFPNOP
	SFPNOP
	SFPABS	L0, L0, 0
	SFPNOP
	SFPNOP
	SFPSETSGN	1, L3, L3, 1
	SFPNOP
	SFPNOP
	SFPENCC	3, 10
	SFPNOP
	SFPLOADI	L1, 4, 0
	SFPSETCC	0, L0, 2
	SFPNOP
	SFPLZ	L0, L1, 0
	SFPNOP
	SFPNOP
	SFPNOT	L1, L1
	SFPNOP
	SFPNOP
	SFPIADD	19, L1, L1, 9
	SFPNOP
	SFPNOP
	SFPNOP
	SFPIADD	127, L1, L1, 5
	SFPNOP
	SFPNOP
	SFPENCC	3, 10
	SFPNOP
	SFPSETEXP	0, L3, L1, 0
	SFPNOP
	SFPNOP
	SFPLZ	L0, L3, 0
	SFPNOP
	SFPNOP
	SFPIADD	1, L3, L3, 5
	SFPNOP
	SFPNOP
	SFPSHFT	0, L3, L0, 0
	SFPNOP
	SFPNOP
	SFPSETMAN	0, L1, L0, 0
	SFPNOP
	SFPNOP
	SFPMAD	L0, L5, L2, L0, 0
	SFPNOP
	SFPNOP
	SFPLOAD	L1, 1, 0
	SFPSETCC	0, L1, 6
	SFPNOP
	SFPLOADI	L0, 0, -128
	SFPENCC	3, 10
	SFPNOP
	SFPSTORE	L0, 0, 0
	SFPINCRWC	0, 4, 0, 0
	addi	a5,a5,-1
	bne	a5,zero,.L22
	ret
	.size	_ZN7ckernel4sfpu13calculate_logILb1ELb0EEEvm.isra.0, .-_ZN7ckernel4sfpu13calculate_logILb1ELb0EEEvm.isra.0
	.section	.text._ZN7ckernel4sfpu14calculate_compILb0EL8SfpuType8EEEvm,"axG",@progbits,_ZN7ckernel4sfpu14calculate_compILb0EL8SfpuType8EEEvm,comdat
	.align	1
	.weak	_ZN7ckernel4sfpu14calculate_compILb0EL8SfpuType8EEEvm
	.type	_ZN7ckernel4sfpu14calculate_compILb0EL8SfpuType8EEEvm, @function
_ZN7ckernel4sfpu14calculate_compILb0EL8SfpuType8EEEvm:
	bne	a0,zero,.L25
	SFPLOAD	L1, 1, 0
	SFPLOADI	L0, 4, 14336
	SFPIADD	0, L1, L0, 4
	SFPNOP
	SFPNOP
	SFPSETCC	0, L0, 6
	SFPNOP
	SFPLOADI	L0, 0, 0
	SFPCOMPC
	SFPNOP
	SFPLOADI	L0, 0, 16256
	SFPENCC	3, 10
	SFPNOP
	SFPSTORE	L0, 0, 0
	SFPINCRWC	0, 4, 0, 0
	SFPLOAD	L1, 1, 0
	SFPLOADI	L0, 4, 14336
	SFPIADD	0, L1, L0, 4
	SFPNOP
	SFPNOP
	SFPSETCC	0, L0, 6
	SFPNOP
	SFPLOADI	L0, 0, 0
	SFPCOMPC
	SFPNOP
	SFPLOADI	L0, 0, 16256
	SFPENCC	3, 10
	SFPNOP
	SFPSTORE	L0, 0, 0
	SFPINCRWC	0, 4, 0, 0
	SFPLOAD	L1, 1, 0
	SFPLOADI	L0, 4, 14336
	SFPIADD	0, L1, L0, 4
	SFPNOP
	SFPNOP
	SFPSETCC	0, L0, 6
	SFPNOP
	SFPLOADI	L0, 0, 0
	SFPCOMPC
	SFPNOP
	SFPLOADI	L0, 0, 16256
	SFPENCC	3, 10
	SFPNOP
	SFPSTORE	L0, 0, 0
	SFPINCRWC	0, 4, 0, 0
	SFPLOAD	L1, 1, 0
	SFPLOADI	L0, 4, 14336
	SFPIADD	0, L1, L0, 4
	SFPNOP
	SFPNOP
	SFPSETCC	0, L0, 6
	SFPNOP
	SFPLOADI	L0, 0, 0
	SFPCOMPC
	SFPNOP
	SFPLOADI	L0, 0, 16256
	SFPENCC	3, 10
	SFPNOP
	SFPSTORE	L0, 0, 0
	SFPINCRWC	0, 4, 0, 0
	ret
.L25:
	SFPLOAD	L0, 1, 0
	SFPSETCC	0, L0, 6
	SFPNOP
	SFPLOADI	L0, 0, 0
	SFPCOMPC
	SFPNOP
	SFPLOADI	L0, 0, 16256
	SFPENCC	3, 10
	SFPNOP
	SFPSTORE	L0, 0, 0
	SFPINCRWC	0, 4, 0, 0
	SFPLOAD	L0, 1, 0
	SFPSETCC	0, L0, 6
	SFPNOP
	SFPLOADI	L0, 0, 0
	SFPCOMPC
	SFPNOP
	SFPLOADI	L0, 0, 16256
	SFPENCC	3, 10
	SFPNOP
	SFPSTORE	L0, 0, 0
	SFPINCRWC	0, 4, 0, 0
	SFPLOAD	L0, 1, 0
	SFPSETCC	0, L0, 6
	SFPNOP
	SFPLOADI	L0, 0, 0
	SFPCOMPC
	SFPNOP
	SFPLOADI	L0, 0, 16256
	SFPENCC	3, 10
	SFPNOP
	SFPSTORE	L0, 0, 0
	SFPINCRWC	0, 4, 0, 0
	SFPLOAD	L0, 1, 0
	SFPSETCC	0, L0, 6
	SFPNOP
	SFPLOADI	L0, 0, 0
	SFPCOMPC
	SFPNOP
	SFPLOADI	L0, 0, 16256
	SFPENCC	3, 10
	SFPNOP
	SFPSTORE	L0, 0, 0
	SFPINCRWC	0, 4, 0, 0
	ret
	.size	_ZN7ckernel4sfpu14calculate_compILb0EL8SfpuType8EEEvm, .-_ZN7ckernel4sfpu14calculate_compILb0EL8SfpuType8EEEvm
	.section	.text._ZN7ckernel4sfpu14calculate_compILb0EL8SfpuType10EEEvm,"axG",@progbits,_ZN7ckernel4sfpu14calculate_compILb0EL8SfpuType10EEEvm,comdat
	.align	1
	.weak	_ZN7ckernel4sfpu14calculate_compILb0EL8SfpuType10EEEvm
	.type	_ZN7ckernel4sfpu14calculate_compILb0EL8SfpuType10EEEvm, @function
_ZN7ckernel4sfpu14calculate_compILb0EL8SfpuType10EEEvm:
	bne	a0,zero,.L29
	SFPLOAD	L1, 1, 0
	SFPLOADI	L0, 4, 14336
	SFPIADD	0, L1, L0, 4
	SFPNOP
	SFPNOP
	SFPSETCC	0, L0, 6
	SFPNOP
	SFPLOADI	L0, 0, 16256
	SFPCOMPC
	SFPNOP
	SFPLOADI	L0, 0, 0
	SFPENCC	3, 10
	SFPNOP
	SFPSTORE	L0, 0, 0
	SFPINCRWC	0, 4, 0, 0
	SFPLOAD	L1, 1, 0
	SFPLOADI	L0, 4, 14336
	SFPIADD	0, L1, L0, 4
	SFPNOP
	SFPNOP
	SFPSETCC	0, L0, 6
	SFPNOP
	SFPLOADI	L0, 0, 16256
	SFPCOMPC
	SFPNOP
	SFPLOADI	L0, 0, 0
	SFPENCC	3, 10
	SFPNOP
	SFPSTORE	L0, 0, 0
	SFPINCRWC	0, 4, 0, 0
	SFPLOAD	L1, 1, 0
	SFPLOADI	L0, 4, 14336
	SFPIADD	0, L1, L0, 4
	SFPNOP
	SFPNOP
	SFPSETCC	0, L0, 6
	SFPNOP
	SFPLOADI	L0, 0, 16256
	SFPCOMPC
	SFPNOP
	SFPLOADI	L0, 0, 0
	SFPENCC	3, 10
	SFPNOP
	SFPSTORE	L0, 0, 0
	SFPINCRWC	0, 4, 0, 0
	SFPLOAD	L1, 1, 0
	SFPLOADI	L0, 4, 14336
	SFPIADD	0, L1, L0, 4
	SFPNOP
	SFPNOP
	SFPSETCC	0, L0, 6
	SFPNOP
	SFPLOADI	L0, 0, 16256
	SFPCOMPC
	SFPNOP
	SFPLOADI	L0, 0, 0
	SFPENCC	3, 10
	SFPNOP
	SFPSTORE	L0, 0, 0
	SFPINCRWC	0, 4, 0, 0
	ret
.L29:
	SFPLOAD	L0, 1, 0
	SFPSETCC	0, L0, 6
	SFPNOP
	SFPLOADI	L0, 0, 16256
	SFPCOMPC
	SFPNOP
	SFPLOADI	L0, 0, 0
	SFPENCC	3, 10
	SFPNOP
	SFPSTORE	L0, 0, 0
	SFPINCRWC	0, 4, 0, 0
	SFPLOAD	L0, 1, 0
	SFPSETCC	0, L0, 6
	SFPNOP
	SFPLOADI	L0, 0, 16256
	SFPCOMPC
	SFPNOP
	SFPLOADI	L0, 0, 0
	SFPENCC	3, 10
	SFPNOP
	SFPSTORE	L0, 0, 0
	SFPINCRWC	0, 4, 0, 0
	SFPLOAD	L0, 1, 0
	SFPSETCC	0, L0, 6
	SFPNOP
	SFPLOADI	L0, 0, 16256
	SFPCOMPC
	SFPNOP
	SFPLOADI	L0, 0, 0
	SFPENCC	3, 10
	SFPNOP
	SFPSTORE	L0, 0, 0
	SFPINCRWC	0, 4, 0, 0
	SFPLOAD	L0, 1, 0
	SFPSETCC	0, L0, 6
	SFPNOP
	SFPLOADI	L0, 0, 16256
	SFPCOMPC
	SFPNOP
	SFPLOADI	L0, 0, 0
	SFPENCC	3, 10
	SFPNOP
	SFPSTORE	L0, 0, 0
	SFPINCRWC	0, 4, 0, 0
	ret
	.size	_ZN7ckernel4sfpu14calculate_compILb0EL8SfpuType10EEEvm, .-_ZN7ckernel4sfpu14calculate_compILb0EL8SfpuType10EEEvm
	.section	.text._ZN7ckernel4sfpu15calculate_clampILb0EEEvmmm,"axG",@progbits,_ZN7ckernel4sfpu15calculate_clampILb0EEEvmmm,comdat
	.align	1
	.weak	_ZN7ckernel4sfpu15calculate_clampILb0EEEvmmm
	.type	_ZN7ckernel4sfpu15calculate_clampILb0EEEvmmm, @function
_ZN7ckernel4sfpu15calculate_clampILb0EEEvmmm:
	lui	a6,%hi(_ZN7ckernel13instrn_bufferE)
	li	a3,65536
	addi	a3,a3,-1
	lw	a7,%lo(_ZN7ckernel13instrn_bufferE)(a6)
	and	a0,a0,a3
	li	a5, 1897988096	# 71210000
	add	a5,a0,a5
	sw	a5, 0(a7)	# Op(0x71) - d(lr2)
	and	a5,a1,a3
	li	a4, 1896939520	# 71110000
	add	a4,a5,a4
	sw	a4, 0(a7)	# Op(0x71) - d(lr1)
	and	a3,a2,a3
	li	a2, 1895890944	# 71010000
	slli	a3,a3,8
	li	a1, 1895890944	# 71010000
	add	a2,a5,a2
	li	a5, 1962934272	# 75000000
	li	a4,4
	add	a1,a0,a1
	add	a3,a3,a5
.L33:
	SFPLOAD	L0, 1, 0
	SFPMAD	L2, L11, L0, L3, 0
	SFPNOP
	SFPNOP
	SFPSETCC	0, L3, 0
	SFPNOP
	lw	a5,%lo(_ZN7ckernel13instrn_bufferE)(a6)
	sw	a1, 0(a5)	# Op(0x71) lv(lr0)  d(lr0)
	SFPCOMPC
	SFPNOP
	SFPMAD	L1, L11, L0, L3, 0
	SFPNOP
	SFPNOP
	SFPSETCC	0, L3, 4
	SFPNOP
	sw	a2, 0(a5)	# Op(0x71) lv(lr0)  d(lr0)
	SFPENCC	3, 10
	SFPNOP
	sw	a3, 0(a5)	# Op(0x75) lv(lr0)  d(lr0)
	SFPNOP
	SFPNOP
	SFPSTORE	L0, 0, 0
	SFPINCRWC	0, 4, 0, 0
	addi	a4,a4,-1
	bne	a4,zero,.L33
	ret
	.size	_ZN7ckernel4sfpu15calculate_clampILb0EEEvmmm, .-_ZN7ckernel4sfpu15calculate_clampILb0EEEvmmm
	.section	.text._ZN7ckernel4sfpu13calculate_absILb0EEEvv,"axG",@progbits,_ZN7ckernel4sfpu13calculate_absILb0EEEvv,comdat
	.align	1
	.weak	_ZN7ckernel4sfpu13calculate_absILb0EEEvv
	.type	_ZN7ckernel4sfpu13calculate_absILb0EEEvv, @function
_ZN7ckernel4sfpu13calculate_absILb0EEEvv:
	SFPLOAD	L0, 1, 0
	SFPABS	L0, L0, 1
	SFPNOP
	SFPNOP
	SFPSTORE	L0, 0, 0
	SFPINCRWC	0, 4, 0, 0
	SFPLOAD	L0, 1, 0
	SFPABS	L0, L0, 1
	SFPNOP
	SFPNOP
	SFPSTORE	L0, 0, 0
	SFPINCRWC	0, 4, 0, 0
	SFPLOAD	L0, 1, 0
	SFPABS	L0, L0, 1
	SFPNOP
	SFPNOP
	SFPSTORE	L0, 0, 0
	SFPINCRWC	0, 4, 0, 0
	SFPLOAD	L0, 1, 0
	SFPABS	L0, L0, 1
	SFPNOP
	SFPNOP
	SFPSTORE	L0, 0, 0
	SFPINCRWC	0, 4, 0, 0
	ret
	.size	_ZN7ckernel4sfpu13calculate_absILb0EEEvv, .-_ZN7ckernel4sfpu13calculate_absILb0EEEvv
	.section	.text._ZN7ckernel4sfpu14calculate_signILb0EEEvm,"axG",@progbits,_ZN7ckernel4sfpu14calculate_signILb0EEEvm,comdat
	.align	1
	.weak	_ZN7ckernel4sfpu14calculate_signILb0EEEvm
	.type	_ZN7ckernel4sfpu14calculate_signILb0EEEvm, @function
_ZN7ckernel4sfpu14calculate_signILb0EEEvm:
	bne	a0,zero,.L37
	SFPLOAD	L1, 1, 0
	SFPSTORE	L10, 0, 0
	SFPSETCC	0, L1, 0
	SFPNOP
	SFPSTORE	L11, 0, 0
	SFPENCC	3, 10
	SFPNOP
	SFPLOADI	L0, 4, 14336
	SFPIADD	0, L1, L0, 4
	SFPNOP
	SFPNOP
	SFPSETCC	0, L0, 6
	SFPNOP
	SFPSTORE	L4, 0, 0
	SFPENCC	3, 10
	SFPNOP
	SFPINCRWC	0, 4, 0, 0
	SFPLOAD	L1, 1, 0
	SFPSTORE	L10, 0, 0
	SFPSETCC	0, L1, 0
	SFPNOP
	SFPSTORE	L11, 0, 0
	SFPENCC	3, 10
	SFPNOP
	SFPLOADI	L0, 4, 14336
	SFPIADD	0, L1, L0, 4
	SFPNOP
	SFPNOP
	SFPSETCC	0, L0, 6
	SFPNOP
	SFPSTORE	L4, 0, 0
	SFPENCC	3, 10
	SFPNOP
	SFPINCRWC	0, 4, 0, 0
	SFPLOAD	L1, 1, 0
	SFPSTORE	L10, 0, 0
	SFPSETCC	0, L1, 0
	SFPNOP
	SFPSTORE	L11, 0, 0
	SFPENCC	3, 10
	SFPNOP
	SFPLOADI	L0, 4, 14336
	SFPIADD	0, L1, L0, 4
	SFPNOP
	SFPNOP
	SFPSETCC	0, L0, 6
	SFPNOP
	SFPSTORE	L4, 0, 0
	SFPENCC	3, 10
	SFPNOP
	SFPINCRWC	0, 4, 0, 0
	SFPLOAD	L1, 1, 0
	SFPSTORE	L10, 0, 0
	SFPSETCC	0, L1, 0
	SFPNOP
	SFPSTORE	L11, 0, 0
	SFPENCC	3, 10
	SFPNOP
	SFPLOADI	L0, 4, 14336
	SFPIADD	0, L1, L0, 4
	SFPNOP
	SFPNOP
	SFPSETCC	0, L0, 6
	SFPNOP
	SFPSTORE	L4, 0, 0
	SFPENCC	3, 10
	SFPNOP
	SFPINCRWC	0, 4, 0, 0
	ret
.L37:
	SFPLOAD	L0, 1, 0
	SFPSTORE	L10, 0, 0
	SFPSETCC	0, L0, 0
	SFPNOP
	SFPSTORE	L11, 0, 0
	SFPENCC	3, 10
	SFPNOP
	SFPSETCC	0, L0, 6
	SFPNOP
	SFPSTORE	L4, 0, 0
	SFPENCC	3, 10
	SFPNOP
	SFPINCRWC	0, 4, 0, 0
	SFPLOAD	L0, 1, 0
	SFPSTORE	L10, 0, 0
	SFPSETCC	0, L0, 0
	SFPNOP
	SFPSTORE	L11, 0, 0
	SFPENCC	3, 10
	SFPNOP
	SFPSETCC	0, L0, 6
	SFPNOP
	SFPSTORE	L4, 0, 0
	SFPENCC	3, 10
	SFPNOP
	SFPINCRWC	0, 4, 0, 0
	SFPLOAD	L0, 1, 0
	SFPSTORE	L10, 0, 0
	SFPSETCC	0, L0, 0
	SFPNOP
	SFPSTORE	L11, 0, 0
	SFPENCC	3, 10
	SFPNOP
	SFPSETCC	0, L0, 6
	SFPNOP
	SFPSTORE	L4, 0, 0
	SFPENCC	3, 10
	SFPNOP
	SFPINCRWC	0, 4, 0, 0
	SFPLOAD	L0, 1, 0
	SFPSTORE	L10, 0, 0
	SFPSETCC	0, L0, 0
	SFPNOP
	SFPSTORE	L11, 0, 0
	SFPENCC	3, 10
	SFPNOP
	SFPSETCC	0, L0, 6
	SFPNOP
	SFPSTORE	L4, 0, 0
	SFPENCC	3, 10
	SFPNOP
	SFPINCRWC	0, 4, 0, 0
	ret
	.size	_ZN7ckernel4sfpu14calculate_signILb0EEEvm, .-_ZN7ckernel4sfpu14calculate_signILb0EEEvm
	.section	.text._ZN7ckernel4sfpu17calculate_sigmoidILb0EEEvv,"axG",@progbits,_ZN7ckernel4sfpu17calculate_sigmoidILb0EEEvv,comdat
	.align	1
	.weak	_ZN7ckernel4sfpu17calculate_sigmoidILb0EEEvv
	.type	_ZN7ckernel4sfpu17calculate_sigmoidILb0EEEvv, @function
_ZN7ckernel4sfpu17calculate_sigmoidILb0EEEvv:
	SFPLOAD	L3, 1, 0
	SFPLUT	L3, 4
	SFPNOP
	SFPNOP
	SFPADDI	16128, L3, 0
	SFPNOP
	SFPNOP
	SFPSTORE	L3, 0, 0
	SFPINCRWC	0, 4, 0, 0
	SFPLOAD	L3, 1, 0
	SFPLUT	L3, 4
	SFPNOP
	SFPNOP
	SFPADDI	16128, L3, 0
	SFPNOP
	SFPNOP
	SFPSTORE	L3, 0, 0
	SFPINCRWC	0, 4, 0, 0
	SFPLOAD	L3, 1, 0
	SFPLUT	L3, 4
	SFPNOP
	SFPNOP
	SFPADDI	16128, L3, 0
	SFPNOP
	SFPNOP
	SFPSTORE	L3, 0, 0
	SFPINCRWC	0, 4, 0, 0
	SFPLOAD	L3, 1, 0
	SFPLUT	L3, 4
	SFPNOP
	SFPNOP
	SFPADDI	16128, L3, 0
	SFPNOP
	SFPNOP
	SFPSTORE	L3, 0, 0
	SFPINCRWC	0, 4, 0, 0
	ret
	.size	_ZN7ckernel4sfpu17calculate_sigmoidILb0EEEvv, .-_ZN7ckernel4sfpu17calculate_sigmoidILb0EEEvv
	.section	.text._ZN7ckernel4sfpu14calculate_tanhILb1EEEvv,"axG",@progbits,_ZN7ckernel4sfpu14calculate_tanhILb1EEEvv,comdat
	.align	1
	.weak	_ZN7ckernel4sfpu14calculate_tanhILb1EEEvv
	.type	_ZN7ckernel4sfpu14calculate_tanhILb1EEEvv, @function
_ZN7ckernel4sfpu14calculate_tanhILb1EEEvv:
	SFPLOAD	L3, 1, 0
	SFPLUT	L3, 4
	SFPNOP
	SFPNOP
	SFPSTORE	L3, 0, 0
	SFPINCRWC	0, 4, 0, 0
	SFPLOAD	L3, 1, 0
	SFPLUT	L3, 4
	SFPNOP
	SFPNOP
	SFPSTORE	L3, 0, 0
	SFPINCRWC	0, 4, 0, 0
	SFPLOAD	L3, 1, 0
	SFPLUT	L3, 4
	SFPNOP
	SFPNOP
	SFPSTORE	L3, 0, 0
	SFPINCRWC	0, 4, 0, 0
	SFPLOAD	L3, 1, 0
	SFPLUT	L3, 4
	SFPNOP
	SFPNOP
	SFPSTORE	L3, 0, 0
	SFPINCRWC	0, 4, 0, 0
	ret
	.size	_ZN7ckernel4sfpu14calculate_tanhILb1EEEvv, .-_ZN7ckernel4sfpu14calculate_tanhILb1EEEvv
	.section	.text._ZN7ckernel4sfpu18calculate_hardtanhILb0EEEvmmm,"axG",@progbits,_ZN7ckernel4sfpu18calculate_hardtanhILb0EEEvmmm,comdat
	.align	1
	.weak	_ZN7ckernel4sfpu18calculate_hardtanhILb0EEEvmmm
	.type	_ZN7ckernel4sfpu18calculate_hardtanhILb0EEEvmmm, @function
_ZN7ckernel4sfpu18calculate_hardtanhILb0EEEvmmm:
	li	a5,65536
	lui	a4,%hi(_ZN7ckernel13instrn_bufferE)
	addi	a5,a5,-1
	lw	a4,%lo(_ZN7ckernel13instrn_bufferE)(a4)
	and	a0,a0,a5
	li	a3, 1898971136	# 71300000
	add	a0,a0,a3
	sw	a0, 0(a4)	# Op(0x71) - d(lr3)
	and	a1,a1,a5
	li	a3, 1897922560	# 71200000
	add	a1,a1,a3
	sw	a1, 0(a4)	# Op(0x71) - d(lr2)
	and	a2,a2,a5
	li	a5, 1896873984	# 71100000
	add	a2,a2,a5
	sw	a2, 0(a4)	# Op(0x71) - d(lr1)
	li	a5,4
.L43:
	SFPLOAD	L0, 1, 0
	SFPADD	L10, L0, L3, L0, 0
	SFPNOP
	SFPNOP
	SFPSETCC	0, L0, 0
	SFPNOP
	SFPLOADI	L0, 0, 0
	SFPENCC	3, 10
	SFPNOP
	SFPADD	L10, L0, L2, L0, 0
	SFPNOP
	SFPNOP
	SFPSETCC	0, L0, 4
	SFPNOP
	SFPLOADI	L0, 0, 0
	SFPENCC	3, 10
	SFPNOP
	SFPADD	L10, L0, L1, L0, 0
	SFPNOP
	SFPNOP
	SFPSTORE	L0, 0, 0
	SFPINCRWC	0, 4, 0, 0
	addi	a5,a5,-1
	bne	a5,zero,.L43
	ret
	.size	_ZN7ckernel4sfpu18calculate_hardtanhILb0EEEvmmm, .-_ZN7ckernel4sfpu18calculate_hardtanhILb0EEEvmmm
	.section	.text._ZN7ckernel4sfpu15calculate_lreluILb0EEEvm,"axG",@progbits,_ZN7ckernel4sfpu15calculate_lreluILb0EEEvm,comdat
	.align	1
	.weak	_ZN7ckernel4sfpu15calculate_lreluILb0EEEvm
	.type	_ZN7ckernel4sfpu15calculate_lreluILb0EEEvm, @function
_ZN7ckernel4sfpu15calculate_lreluILb0EEEvm:
	lui	a5,%hi(_ZN7ckernel13instrn_bufferE)
	lw	a5,%lo(_ZN7ckernel13instrn_bufferE)(a5)
	slli	a0,a0,16
	li	a4, 1896873984	# 71100000
	srli	a0,a0,16
	add	a0,a0,a4
	sw	a0, 0(a5)	# Op(0x71) - d(lr1)
	li	a5,4
.L46:
	SFPLOAD	L0, 1, 0
	SFPSETCC	0, L0, 0
	SFPNOP
	SFPMUL	L0, L1, L4, L0, 0
	SFPNOP
	SFPNOP
	SFPENCC	3, 10
	SFPNOP
	SFPSTORE	L0, 0, 0
	SFPINCRWC	0, 4, 0, 0
	addi	a5,a5,-1
	bne	a5,zero,.L46
	ret
	.size	_ZN7ckernel4sfpu15calculate_lreluILb0EEEvm, .-_ZN7ckernel4sfpu15calculate_lreluILb0EEEvm
	.section	.text._ZN7ckernel4sfpu25calculate_tanh_derivativeILb0ELi0EEEvv,"axG",@progbits,_ZN7ckernel4sfpu25calculate_tanh_derivativeILb0ELi0EEEvv,comdat
	.align	1
	.weak	_ZN7ckernel4sfpu25calculate_tanh_derivativeILb0ELi0EEEvv
	.type	_ZN7ckernel4sfpu25calculate_tanh_derivativeILb0ELi0EEEvv, @function
_ZN7ckernel4sfpu25calculate_tanh_derivativeILb0ELi0EEEvv:
	SFPLOAD	L3, 1, 0
	SFPLUT	L3, 4
	SFPNOP
	SFPNOP
	SFPMUL	L3, L3, L4, L3, 0
	SFPNOP
	SFPNOP
	SFPMAD	L11, L3, L10, L3, 0
	SFPNOP
	SFPNOP
	SFPSTORE	L3, 0, 0
	SFPINCRWC	0, 4, 0, 0
	SFPLOAD	L3, 1, 0
	SFPLUT	L3, 4
	SFPNOP
	SFPNOP
	SFPMUL	L3, L3, L4, L3, 0
	SFPNOP
	SFPNOP
	SFPMAD	L11, L3, L10, L3, 0
	SFPNOP
	SFPNOP
	SFPSTORE	L3, 0, 0
	SFPINCRWC	0, 4, 0, 0
	SFPLOAD	L3, 1, 0
	SFPLUT	L3, 4
	SFPNOP
	SFPNOP
	SFPMUL	L3, L3, L4, L3, 0
	SFPNOP
	SFPNOP
	SFPMAD	L11, L3, L10, L3, 0
	SFPNOP
	SFPNOP
	SFPSTORE	L3, 0, 0
	SFPINCRWC	0, 4, 0, 0
	SFPLOAD	L3, 1, 0
	SFPLUT	L3, 4
	SFPNOP
	SFPNOP
	SFPMUL	L3, L3, L4, L3, 0
	SFPNOP
	SFPNOP
	SFPMAD	L11, L3, L10, L3, 0
	SFPNOP
	SFPNOP
	SFPSTORE	L3, 0, 0
	SFPINCRWC	0, 4, 0, 0
	ret
	.size	_ZN7ckernel4sfpu25calculate_tanh_derivativeILb0ELi0EEEvv, .-_ZN7ckernel4sfpu25calculate_tanh_derivativeILb0ELi0EEEvv
	.section	.text._ZN7ckernel4sfpu25calculate_tanh_derivativeILb0ELi1EEEvv,"axG",@progbits,_ZN7ckernel4sfpu25calculate_tanh_derivativeILb0ELi1EEEvv,comdat
	.align	1
	.weak	_ZN7ckernel4sfpu25calculate_tanh_derivativeILb0ELi1EEEvv
	.type	_ZN7ckernel4sfpu25calculate_tanh_derivativeILb0ELi1EEEvv, @function
_ZN7ckernel4sfpu25calculate_tanh_derivativeILb0ELi1EEEvv:
	SFPLOAD	L3, 1, 0
	SFPMUL	L3, L3, L4, L3, 0
	SFPNOP
	SFPNOP
	SFPMAD	L11, L3, L10, L3, 0
	SFPNOP
	SFPNOP
	SFPSTORE	L3, 0, 0
	SFPINCRWC	0, 4, 0, 0
	SFPLOAD	L3, 1, 0
	SFPMUL	L3, L3, L4, L3, 0
	SFPNOP
	SFPNOP
	SFPMAD	L11, L3, L10, L3, 0
	SFPNOP
	SFPNOP
	SFPSTORE	L3, 0, 0
	SFPINCRWC	0, 4, 0, 0
	SFPLOAD	L3, 1, 0
	SFPMUL	L3, L3, L4, L3, 0
	SFPNOP
	SFPNOP
	SFPMAD	L11, L3, L10, L3, 0
	SFPNOP
	SFPNOP
	SFPSTORE	L3, 0, 0
	SFPINCRWC	0, 4, 0, 0
	SFPLOAD	L3, 1, 0
	SFPMUL	L3, L3, L4, L3, 0
	SFPNOP
	SFPNOP
	SFPMAD	L11, L3, L10, L3, 0
	SFPNOP
	SFPNOP
	SFPSTORE	L3, 0, 0
	SFPINCRWC	0, 4, 0, 0
	ret
	.size	_ZN7ckernel4sfpu25calculate_tanh_derivativeILb0ELi1EEEvv, .-_ZN7ckernel4sfpu25calculate_tanh_derivativeILb0ELi1EEEvv
	.section	.text._ZN7ckernel4sfpu15calculate_powerILb1EEEvm,"axG",@progbits,_ZN7ckernel4sfpu15calculate_powerILb1EEEvm,comdat
	.align	1
	.weak	_ZN7ckernel4sfpu15calculate_powerILb1EEEvm
	.type	_ZN7ckernel4sfpu15calculate_powerILb1EEEvm, @function
_ZN7ckernel4sfpu15calculate_powerILb1EEEvm:
	li	a4,4
	li	a3,2
.L53:
	SFPLOAD	L1, 1, 0
	SFPMUL	L1, L1, L4, L0, 0
	SFPNOP
	SFPNOP
	bleu	a0,a3,.L51
	li	a5,2
.L52:
	SFPMUL	L0, L1, L4, L0, 0
	SFPNOP
	SFPNOP
	addi	a5,a5,1
	bne	a0,a5,.L52
.L51:
	SFPSTORE	L0, 0, 0
	SFPINCRWC	0, 4, 0, 0
	addi	a4,a4,-1
	bne	a4,zero,.L53
	ret
	.size	_ZN7ckernel4sfpu15calculate_powerILb1EEEvm, .-_ZN7ckernel4sfpu15calculate_powerILb1EEEvm
	.section	.text._ZN7ckernel4sfpu14calculate_geluILb0EEEvv,"axG",@progbits,_ZN7ckernel4sfpu14calculate_geluILb0EEEvv,comdat
	.align	1
	.weak	_ZN7ckernel4sfpu14calculate_geluILb0EEEvv
	.type	_ZN7ckernel4sfpu14calculate_geluILb0EEEvv, @function
_ZN7ckernel4sfpu14calculate_geluILb0EEEvv:
	li	a5,4
.L57:
	SFPLOAD	L1, 1, 0
	SFPMUL	L1, L1, L4, L3, 0
	SFPNOP
	SFPNOP
	SFPMUL	L3, L1, L4, L3, 0
	SFPNOP
	SFPNOP
	SFPLOADI	L2, 0, 15671
	SFPMAD	L3, L2, L1, L3, 0
	SFPNOP
	SFPNOP
	SFPMULI	16204, L3, 0
	SFPNOP
	SFPNOP
	SFPLOADI	L1, 2, 8208
	SFPLOADI	L2, 2, -256
	SFPLUT	L3, 4
	SFPNOP
	SFPNOP
	SFPLOAD	L1, 1, 0
	SFPMAD	L1, L3, L1, L3, 0
	SFPNOP
	SFPNOP
	SFPMULI	16128, L3, 0
	SFPNOP
	SFPNOP
	SFPSTORE	L3, 0, 0
	SFPINCRWC	0, 4, 0, 0
	addi	a5,a5,-1
	bne	a5,zero,.L57
	ret
	.size	_ZN7ckernel4sfpu14calculate_geluILb0EEEvv, .-_ZN7ckernel4sfpu14calculate_geluILb0EEEvv
	.section	.text._ZN7ckernel4sfpu14calculate_geluILb1EEEvv,"axG",@progbits,_ZN7ckernel4sfpu14calculate_geluILb1EEEvv,comdat
	.align	1
	.weak	_ZN7ckernel4sfpu14calculate_geluILb1EEEvv
	.type	_ZN7ckernel4sfpu14calculate_geluILb1EEEvv, @function
_ZN7ckernel4sfpu14calculate_geluILb1EEEvv:
	li	a5,4
.L60:
	SFPLOAD	L3, 1, 0
	SFPLOADI	L1, 2, 8492
	SFPLOADI	L2, 2, -256
	SFPLUT	L3, 4
	SFPNOP
	SFPNOP
	SFPLOAD	L1, 1, 0
	SFPMAD	L1, L3, L1, L1, 0
	SFPNOP
	SFPNOP
	SFPMULI	16128, L1, 0
	SFPNOP
	SFPNOP
	SFPSTORE	L1, 0, 0
	SFPINCRWC	0, 4, 0, 0
	addi	a5,a5,-1
	bne	a5,zero,.L60
	ret
	.size	_ZN7ckernel4sfpu14calculate_geluILb1EEEvv, .-_ZN7ckernel4sfpu14calculate_geluILb1EEEvv
	.section	.text._ZN7ckernel4sfpu25calculate_gelu_derivativeILb0EEEvv,"axG",@progbits,_ZN7ckernel4sfpu25calculate_gelu_derivativeILb0EEEvv,comdat
	.align	1
	.weak	_ZN7ckernel4sfpu25calculate_gelu_derivativeILb0EEEvv
	.type	_ZN7ckernel4sfpu25calculate_gelu_derivativeILb0EEEvv, @function
_ZN7ckernel4sfpu25calculate_gelu_derivativeILb0EEEvv:
	li	a4,4
.L64:
	SFPLOAD	L3, 1, 0
	SFPMUL	L3, L3, L4, L0, 0
	SFPNOP
	SFPNOP
	SFPMUL	L0, L9, L4, L0, 0
	SFPNOP
	SFPNOP
	SFPSTORE	L0, 0, 0
	SFPSETSGN	0, L0, L0, 1
	SFPNOP
	SFPNOP
	SFPEXEXP	L0, L1, 10
	SFPNOP
	SFPNOP
	SFPNOP
	SFPSETEXP	126, L0, L0, 1
	SFPNOP
	SFPNOP
	SFPENCC	3, 10
	SFPNOP
	SFPLOADI	L2, 0, 16221
	SFPMAD	L0, L8, L2, L2, 0
	SFPNOP
	SFPNOP
	SFPMAD	L0, L2, L10, L0, 0
	SFPNOP
	SFPNOP
	SFPSETCC	0, L1, 4
	SFPNOP
	SFPMUL	L0, L0, L4, L0, 0
	SFPNOP
	SFPNOP
	li	a5,7
.L63:
	SFPIADD	-1, L1, L1, 9
	SFPNOP
	SFPNOP
	SFPNOP
	SFPMUL	L0, L0, L4, L0, 0
	SFPNOP
	SFPNOP
	addi	a5,a5,-1
	bne	a5,zero,.L63
	SFPENCC	3, 10
	SFPNOP
	SFPLOAD	L1, 1, 0
	SFPSTORE	L0, 0, 0
	SFPSETCC	0, L1, 0
	SFPNOP
	SFPSETSGN	1, L0, L0, 1
	SFPNOP
	SFPNOP
	SFPSETEXP	126, L0, L0, 1
	SFPNOP
	SFPNOP
	SFPLOADI	L1, 0, 16384
	SFPMAD	L0, L7, L1, L1, 0
	SFPNOP
	SFPNOP
	SFPMUL	L7, L1, L4, L2, 0
	SFPNOP
	SFPNOP
	SFPLOADI	L1, 0, 16384
	SFPMAD	L0, L2, L1, L1, 0
	SFPNOP
	SFPNOP
	SFPMUL	L2, L1, L4, L1, 0
	SFPNOP
	SFPNOP
	SFPLOADI	L2, 0, 16384
	SFPMAD	L0, L1, L2, L0, 0
	SFPNOP
	SFPNOP
	SFPMUL	L1, L0, L4, L1, 0
	SFPNOP
	SFPNOP
	SFPEXEXP	L1, L2, 0
	SFPNOP
	SFPNOP
	SFPLOAD	L0, 1, 0
	SFPEXEXP	L0, L0, 0
	SFPNOP
	SFPNOP
	SFPIADD	0, L2, L0, 6
	SFPNOP
	SFPNOP
	SFPPUSHC
	SFPIADD	126, L0, L0, 1
	SFPNOP
	SFPNOP
	SFPNOP
	SFPLOADI	L1, 0, 0
	SFPLOADI	L0, 4, 0
	SFPPOPC
	SFPNOP
	SFPSETEXP	0, L1, L0, 0
	SFPNOP
	SFPNOP
	SFPENCC	3, 10
	SFPNOP
	SFPMULI	16076, L0, 0
	SFPNOP
	SFPNOP
	SFPMUL	L0, L3, L4, L0, 0
	SFPNOP
	SFPNOP
	SFPSTORE	L0, 0, 0
	SFPLOADI	L0, 2, 6399
	SFPLOADI	L1, 2, 8492
	SFPLOADI	L2, 2, -256
	SFPLUT	L3, 4
	SFPNOP
	SFPNOP
	SFPLOADI	L0, 0, 16128
	SFPLOAD	L1, 1, 0
	SFPMAD	L3, L0, L1, L3, 1
	SFPNOP
	SFPNOP
	SFPSTORE	L3, 0, 0
	SFPINCRWC	0, 4, 0, 0
	addi	a4,a4,-1
	bne	a4,zero,.L64
	ret
	.size	_ZN7ckernel4sfpu25calculate_gelu_derivativeILb0EEEvv, .-_ZN7ckernel4sfpu25calculate_gelu_derivativeILb0EEEvv
	.section	.text._ZN7ckernel4sfpu14calculate_sqrtILb0ELi4ELi2EEEvv,"axG",@progbits,_ZN7ckernel4sfpu14calculate_sqrtILb0ELi4ELi2EEEvv,comdat
	.align	1
	.weak	_ZN7ckernel4sfpu14calculate_sqrtILb0ELi4ELi2EEEvv
	.type	_ZN7ckernel4sfpu14calculate_sqrtILb0ELi4ELi2EEEvv, @function
_ZN7ckernel4sfpu14calculate_sqrtILb0ELi4ELi2EEEvv:
	SFPLOAD	L0, 1, 0
	SFPLOADI	L1, 0, 24375
	SFPSHFT	-1, L0, L0, 1
	SFPNOP
	SFPNOP
	SFPIADD	0, L1, L0, 6
	SFPNOP
	SFPNOP
	SFPLOAD	L1, 1, 0
	SFPMUL	L0, L0, L4, L2, 0
	SFPNOP
	SFPNOP
	SFPMUL	L2, L1, L4, L2, 0
	SFPNOP
	SFPNOP
	SFPMAD	L2, L9, L10, L2, 1
	SFPNOP
	SFPNOP
	SFPMUL	L2, L0, L4, L2, 0
	SFPNOP
	SFPNOP
	SFPMUL	L2, L2, L4, L0, 0
	SFPNOP
	SFPNOP
	SFPMUL	L0, L1, L4, L0, 0
	SFPNOP
	SFPNOP
	SFPMAD	L0, L9, L10, L0, 1
	SFPNOP
	SFPNOP
	SFPMUL	L0, L2, L4, L0, 0
	SFPNOP
	SFPNOP
	SFPMUL	L0, L1, L4, L0, 0
	SFPNOP
	SFPNOP
	SFPSTORE	L0, 0, 0
	SFPINCRWC	0, 4, 0, 0
	SFPLOAD	L0, 1, 0
	SFPLOADI	L1, 0, 24375
	SFPSHFT	-1, L0, L0, 1
	SFPNOP
	SFPNOP
	SFPIADD	0, L1, L0, 6
	SFPNOP
	SFPNOP
	SFPLOAD	L1, 1, 0
	SFPMUL	L0, L0, L4, L2, 0
	SFPNOP
	SFPNOP
	SFPMUL	L2, L1, L4, L2, 0
	SFPNOP
	SFPNOP
	SFPMAD	L2, L9, L10, L2, 1
	SFPNOP
	SFPNOP
	SFPMUL	L2, L0, L4, L2, 0
	SFPNOP
	SFPNOP
	SFPMUL	L2, L2, L4, L0, 0
	SFPNOP
	SFPNOP
	SFPMUL	L0, L1, L4, L0, 0
	SFPNOP
	SFPNOP
	SFPMAD	L0, L9, L10, L0, 1
	SFPNOP
	SFPNOP
	SFPMUL	L0, L2, L4, L0, 0
	SFPNOP
	SFPNOP
	SFPMUL	L0, L1, L4, L0, 0
	SFPNOP
	SFPNOP
	SFPSTORE	L0, 0, 0
	SFPINCRWC	0, 4, 0, 0
	SFPLOAD	L0, 1, 0
	SFPLOADI	L1, 0, 24375
	SFPSHFT	-1, L0, L0, 1
	SFPNOP
	SFPNOP
	SFPIADD	0, L1, L0, 6
	SFPNOP
	SFPNOP
	SFPLOAD	L1, 1, 0
	SFPMUL	L0, L0, L4, L2, 0
	SFPNOP
	SFPNOP
	SFPMUL	L2, L1, L4, L2, 0
	SFPNOP
	SFPNOP
	SFPMAD	L2, L9, L10, L2, 1
	SFPNOP
	SFPNOP
	SFPMUL	L2, L0, L4, L2, 0
	SFPNOP
	SFPNOP
	SFPMUL	L2, L2, L4, L0, 0
	SFPNOP
	SFPNOP
	SFPMUL	L0, L1, L4, L0, 0
	SFPNOP
	SFPNOP
	SFPMAD	L0, L9, L10, L0, 1
	SFPNOP
	SFPNOP
	SFPMUL	L0, L2, L4, L0, 0
	SFPNOP
	SFPNOP
	SFPMUL	L0, L1, L4, L0, 0
	SFPNOP
	SFPNOP
	SFPSTORE	L0, 0, 0
	SFPINCRWC	0, 4, 0, 0
	SFPLOAD	L0, 1, 0
	SFPLOADI	L1, 0, 24375
	SFPSHFT	-1, L0, L0, 1
	SFPNOP
	SFPNOP
	SFPIADD	0, L1, L0, 6
	SFPNOP
	SFPNOP
	SFPLOAD	L1, 1, 0
	SFPMUL	L0, L0, L4, L2, 0
	SFPNOP
	SFPNOP
	SFPMUL	L2, L1, L4, L2, 0
	SFPNOP
	SFPNOP
	SFPMAD	L2, L9, L10, L2, 1
	SFPNOP
	SFPNOP
	SFPMUL	L2, L0, L4, L2, 0
	SFPNOP
	SFPNOP
	SFPMUL	L2, L2, L4, L0, 0
	SFPNOP
	SFPNOP
	SFPMUL	L0, L1, L4, L0, 0
	SFPNOP
	SFPNOP
	SFPMAD	L0, L9, L10, L0, 1
	SFPNOP
	SFPNOP
	SFPMUL	L0, L2, L4, L0, 0
	SFPNOP
	SFPNOP
	SFPMUL	L0, L1, L4, L0, 0
	SFPNOP
	SFPNOP
	SFPSTORE	L0, 0, 0
	SFPINCRWC	0, 4, 0, 0
	ret
	.size	_ZN7ckernel4sfpu14calculate_sqrtILb0ELi4ELi2EEEvv, .-_ZN7ckernel4sfpu14calculate_sqrtILb0ELi4ELi2EEEvv
	.section	.text._ZN7ckernel4sfpu14calculate_sqrtILb1ELi4ELi2EEEvv,"axG",@progbits,_ZN7ckernel4sfpu14calculate_sqrtILb1ELi4ELi2EEEvv,comdat
	.align	1
	.weak	_ZN7ckernel4sfpu14calculate_sqrtILb1ELi4ELi2EEEvv
	.type	_ZN7ckernel4sfpu14calculate_sqrtILb1ELi4ELi2EEEvv, @function
_ZN7ckernel4sfpu14calculate_sqrtILb1ELi4ELi2EEEvv:
	SFPLOAD	L0, 1, 0
	SFPIADD	0, L2, L0, 4
	SFPNOP
	SFPNOP
	SFPSHFT	-1, L0, L0, 1
	SFPNOP
	SFPNOP
	SFPSTORE	L0, 0, 0
	SFPINCRWC	0, 4, 0, 0
	SFPLOAD	L0, 1, 0
	SFPIADD	0, L2, L0, 4
	SFPNOP
	SFPNOP
	SFPSHFT	-1, L0, L0, 1
	SFPNOP
	SFPNOP
	SFPSTORE	L0, 0, 0
	SFPINCRWC	0, 4, 0, 0
	SFPLOAD	L0, 1, 0
	SFPIADD	0, L2, L0, 4
	SFPNOP
	SFPNOP
	SFPSHFT	-1, L0, L0, 1
	SFPNOP
	SFPNOP
	SFPSTORE	L0, 0, 0
	SFPINCRWC	0, 4, 0, 0
	SFPLOAD	L0, 1, 0
	SFPIADD	0, L2, L0, 4
	SFPNOP
	SFPNOP
	SFPSHFT	-1, L0, L0, 1
	SFPNOP
	SFPNOP
	SFPSTORE	L0, 0, 0
	SFPINCRWC	0, 4, 0, 0
	ret
	.size	_ZN7ckernel4sfpu14calculate_sqrtILb1ELi4ELi2EEEvv, .-_ZN7ckernel4sfpu14calculate_sqrtILb1ELi4ELi2EEEvv
	.section	.text._ZN7ckernel4sfpu21calculate_exponentialILb0ELb0ELb1EEEvs,"axG",@progbits,_ZN7ckernel4sfpu21calculate_exponentialILb0ELb0ELb1EEEvs,comdat
	.align	1
	.weak	_ZN7ckernel4sfpu21calculate_exponentialILb0ELb0ELb1EEEvs
	.type	_ZN7ckernel4sfpu21calculate_exponentialILb0ELb0ELb1EEEvs, @function
_ZN7ckernel4sfpu21calculate_exponentialILb0ELb0ELb1EEEvs:
	slli	a0,a0,16
	srli	a0,a0,16
	li	a5, 1896939520	# 71110000
	li	a4,4
	lui	a3,%hi(_ZN7ckernel13instrn_bufferE)
	add	a0,a0,a5
.L71:
	SFPLOAD	L0, 1, 0
	lw	a5,%lo(_ZN7ckernel13instrn_bufferE)(a3)
	sw	a0, 0(a5)	# Op(0x71) - d(lr1)
	SFPMUL	L0, L1, L4, L0, 0
	SFPNOP
	SFPNOP
	SFPSTORE	L0, 0, 0
	SFPSETSGN	0, L0, L0, 1
	SFPNOP
	SFPNOP
	SFPEXEXP	L0, L1, 10
	SFPNOP
	SFPNOP
	SFPNOP
	SFPSETEXP	126, L0, L0, 1
	SFPNOP
	SFPNOP
	SFPENCC	3, 10
	SFPNOP
	SFPLOADI	L2, 0, 16221
	SFPMAD	L0, L8, L2, L2, 0
	SFPNOP
	SFPNOP
	SFPMAD	L0, L2, L10, L0, 0
	SFPNOP
	SFPNOP
	SFPSETCC	0, L1, 4
	SFPNOP
	SFPMUL	L0, L0, L4, L0, 0
	SFPNOP
	SFPNOP
	li	a5,7
.L70:
	SFPIADD	-1, L1, L1, 9
	SFPNOP
	SFPNOP
	SFPNOP
	SFPMUL	L0, L0, L4, L0, 0
	SFPNOP
	SFPNOP
	addi	a5,a5,-1
	bne	a5,zero,.L70
	SFPENCC	3, 10
	SFPNOP
	SFPLOAD	L1, 1, 0
	SFPSTORE	L0, 0, 0
	SFPSETCC	0, L1, 0
	SFPNOP
	SFPSETSGN	1, L0, L0, 1
	SFPNOP
	SFPNOP
	SFPSETEXP	126, L0, L0, 1
	SFPNOP
	SFPNOP
	SFPLOADI	L2, 0, 16384
	SFPMAD	L0, L7, L2, L1, 0
	SFPNOP
	SFPNOP
	SFPMUL	L7, L1, L4, L1, 0
	SFPNOP
	SFPNOP
	SFPMAD	L0, L1, L2, L3, 0
	SFPNOP
	SFPNOP
	SFPMUL	L1, L3, L4, L1, 0
	SFPNOP
	SFPNOP
	SFPMAD	L0, L1, L2, L0, 0
	SFPNOP
	SFPNOP
	SFPMUL	L1, L0, L4, L0, 0
	SFPNOP
	SFPNOP
	SFPEXEXP	L0, L2, 0
	SFPNOP
	SFPNOP
	SFPLOAD	L1, 1, 0
	SFPEXEXP	L1, L1, 0
	SFPNOP
	SFPNOP
	SFPIADD	0, L2, L1, 6
	SFPNOP
	SFPNOP
	SFPPUSHC
	SFPIADD	126, L1, L1, 1
	SFPNOP
	SFPNOP
	SFPNOP
	SFPLOADI	L0, 0, 0
	SFPLOADI	L1, 4, 0
	SFPPOPC
	SFPNOP
	SFPSETEXP	0, L0, L1, 0
	SFPNOP
	SFPNOP
	SFPSTORE	L1, 0, 0
	SFPENCC	3, 10
	SFPNOP
	SFPINCRWC	0, 4, 0, 0
	addi	a4,a4,-1
	bne	a4,zero,.L71
	ret
	.size	_ZN7ckernel4sfpu21calculate_exponentialILb0ELb0ELb1EEEvs, .-_ZN7ckernel4sfpu21calculate_exponentialILb0ELb0ELb1EEEvs
	.section	.text._ZN7ckernel4sfpu21calculate_exponentialILb0ELb1ELb1EEEvs,"axG",@progbits,_ZN7ckernel4sfpu21calculate_exponentialILb0ELb1ELb1EEEvs,comdat
	.align	1
	.weak	_ZN7ckernel4sfpu21calculate_exponentialILb0ELb1ELb1EEEvs
	.type	_ZN7ckernel4sfpu21calculate_exponentialILb0ELb1ELb1EEEvs, @function
_ZN7ckernel4sfpu21calculate_exponentialILb0ELb1ELb1EEEvs:
	slli	a0,a0,16
	srli	a0,a0,16
	li	a5, 1896939520	# 71110000
	li	a4,4
	lui	a3,%hi(_ZN7ckernel13instrn_bufferE)
	add	a0,a0,a5
.L76:
	SFPLOAD	L0, 1, 0
	lw	a5,%lo(_ZN7ckernel13instrn_bufferE)(a3)
	sw	a0, 0(a5)	# Op(0x71) - d(lr1)
	SFPMUL	L0, L1, L4, L0, 0
	SFPNOP
	SFPNOP
	SFPSTORE	L0, 0, 0
	SFPSETSGN	0, L0, L0, 1
	SFPNOP
	SFPNOP
	SFPEXEXP	L0, L1, 10
	SFPNOP
	SFPNOP
	SFPNOP
	SFPSETEXP	126, L0, L0, 1
	SFPNOP
	SFPNOP
	SFPENCC	3, 10
	SFPNOP
	SFPLOADI	L2, 0, 16221
	SFPMAD	L0, L8, L2, L2, 0
	SFPNOP
	SFPNOP
	SFPMAD	L0, L2, L10, L0, 0
	SFPNOP
	SFPNOP
	SFPSETCC	0, L1, 4
	SFPNOP
	SFPMUL	L0, L0, L4, L0, 0
	SFPNOP
	SFPNOP
	li	a5,7
.L75:
	SFPIADD	-1, L1, L1, 9
	SFPNOP
	SFPNOP
	SFPNOP
	SFPMUL	L0, L0, L4, L0, 0
	SFPNOP
	SFPNOP
	addi	a5,a5,-1
	bne	a5,zero,.L75
	SFPENCC	3, 10
	SFPNOP
	SFPLOAD	L1, 1, 0
	SFPSTORE	L0, 0, 0
	SFPSETCC	0, L1, 0
	SFPNOP
	SFPSETSGN	1, L0, L0, 1
	SFPNOP
	SFPNOP
	SFPSETEXP	126, L0, L0, 1
	SFPNOP
	SFPNOP
	SFPLOADI	L2, 0, 16384
	SFPMAD	L0, L7, L2, L1, 0
	SFPNOP
	SFPNOP
	SFPMUL	L7, L1, L4, L1, 0
	SFPNOP
	SFPNOP
	SFPMAD	L0, L1, L2, L3, 0
	SFPNOP
	SFPNOP
	SFPMUL	L1, L3, L4, L1, 0
	SFPNOP
	SFPNOP
	SFPMAD	L0, L1, L2, L0, 0
	SFPNOP
	SFPNOP
	SFPMUL	L1, L0, L4, L0, 0
	SFPNOP
	SFPNOP
	SFPEXEXP	L0, L2, 0
	SFPNOP
	SFPNOP
	SFPLOAD	L1, 1, 0
	SFPEXEXP	L1, L1, 0
	SFPNOP
	SFPNOP
	SFPIADD	0, L2, L1, 6
	SFPNOP
	SFPNOP
	SFPPUSHC
	SFPIADD	126, L1, L1, 1
	SFPNOP
	SFPNOP
	SFPNOP
	SFPLOADI	L0, 0, 0
	SFPLOADI	L1, 4, 0
	SFPPOPC
	SFPNOP
	SFPSETEXP	0, L0, L1, 0
	SFPNOP
	SFPNOP
	SFPSTORE	L1, 0, 0
	SFPENCC	3, 10
	SFPNOP
	SFPINCRWC	0, 4, 0, 0
	addi	a4,a4,-1
	bne	a4,zero,.L76
	ret
	.size	_ZN7ckernel4sfpu21calculate_exponentialILb0ELb1ELb1EEEvs, .-_ZN7ckernel4sfpu21calculate_exponentialILb0ELb1ELb1EEEvs
	.section	.text._ZN7ckernel4sfpu21calculate_exponentialILb1ELb0ELb1EEEvs,"axG",@progbits,_ZN7ckernel4sfpu21calculate_exponentialILb1ELb0ELb1EEEvs,comdat
	.align	1
	.weak	_ZN7ckernel4sfpu21calculate_exponentialILb1ELb0ELb1EEEvs
	.type	_ZN7ckernel4sfpu21calculate_exponentialILb1ELb0ELb1EEEvs, @function
_ZN7ckernel4sfpu21calculate_exponentialILb1ELb0ELb1EEEvs:
	slli	a0,a0,16
	srli	a0,a0,16
	li	a3, 1899036672	# 71310000
	li	a5,4
	lui	a2,%hi(_ZN7ckernel13instrn_bufferE)
	add	a3,a3,a0
.L80:
	SFPLOAD	L1, 1, 0
	lw	a4,%lo(_ZN7ckernel13instrn_bufferE)(a2)
	sw	a3, 0(a4)	# Op(0x71) - d(lr3)
	SFPMUL	L1, L3, L4, L1, 0
	SFPNOP
	SFPNOP
	SFPSTORE	L1, 0, 0
	SFPMAD	L1, L7, L0, L1, 0
	SFPNOP
	SFPNOP
	SFPIADD	0, L2, L1, 4
	SFPNOP
	SFPNOP
	SFPSHFT	8, L0, L1, 1
	SFPNOP
	SFPNOP
	SFPSTORE	L1, 0, 0
	SFPINCRWC	0, 4, 0, 0
	addi	a5,a5,-1
	bne	a5,zero,.L80
	ret
	.size	_ZN7ckernel4sfpu21calculate_exponentialILb1ELb0ELb1EEEvs, .-_ZN7ckernel4sfpu21calculate_exponentialILb1ELb0ELb1EEEvs
	.section	.text._ZN7ckernel4sfpu21calculate_exponentialILb1ELb1ELb1EEEvs,"axG",@progbits,_ZN7ckernel4sfpu21calculate_exponentialILb1ELb1ELb1EEEvs,comdat
	.align	1
	.weak	_ZN7ckernel4sfpu21calculate_exponentialILb1ELb1ELb1EEEvs
	.type	_ZN7ckernel4sfpu21calculate_exponentialILb1ELb1ELb1EEEvs, @function
_ZN7ckernel4sfpu21calculate_exponentialILb1ELb1ELb1EEEvs:
	slli	a5,a0,16
	srli	a5,a5,16
	li	a3, 1899036672	# 71310000
	li	a4,4
	lui	a2,%hi(_ZN7ckernel13instrn_bufferE)
	add	a5,a5,a3
.L83:
	SFPLOAD	L1, 1, 0
	lw	a3,%lo(_ZN7ckernel13instrn_bufferE)(a2)
	sw	a5, 0(a3)	# Op(0x71) - d(lr3)
	SFPMUL	L1, L3, L4, L1, 0
	SFPNOP
	SFPNOP
	SFPSTORE	L1, 0, 0
	SFPMAD	L1, L7, L0, L1, 0
	SFPNOP
	SFPNOP
	SFPIADD	0, L2, L1, 4
	SFPNOP
	SFPNOP
	SFPSHFT	8, L0, L1, 1
	SFPNOP
	SFPNOP
	SFPSTORE	L1, 0, 0
	SFPSETCC	0, L1, 0
	SFPNOP
	SFPSTORE	L4, 0, 0
	SFPENCC	3, 10
	SFPNOP
	SFPINCRWC	0, 4, 0, 0
	addi	a4,a4,-1
	bne	a4,zero,.L83
	ret
	.size	_ZN7ckernel4sfpu21calculate_exponentialILb1ELb1ELb1EEEvs, .-_ZN7ckernel4sfpu21calculate_exponentialILb1ELb1ELb1EEEvs
	.section	.text._ZN7ckernel4sfpu13calculate_logILb1ELb1EEEvm,"axG",@progbits,_ZN7ckernel4sfpu13calculate_logILb1ELb1EEEvm,comdat
	.align	1
	.weak	_ZN7ckernel4sfpu13calculate_logILb1ELb1EEEvm
	.type	_ZN7ckernel4sfpu13calculate_logILb1ELb1EEEvm, @function
_ZN7ckernel4sfpu13calculate_logILb1ELb1EEEvm:
	slli	a4,a0,16
	srli	a4,a4,16
	li	a0, 1896939520	# 71110000
	li	a5,4
	lui	a3,%hi(_ZN7ckernel13instrn_bufferE)
	add	a0,a0,a4
.L86:
	SFPLOAD	L3, 1, 0
	SFPSETEXP	127, L3, L0, 1
	SFPNOP
	SFPNOP
	SFPLOADI	L1, 1, 11973
	SFPLOADI	L2, 1, -17997
	SFPMAD	L0, L1, L2, L1, 0
	SFPNOP
	SFPNOP
	SFPLOADI	L2, 1, 16428
	SFPMAD	L0, L1, L2, L1, 0
	SFPNOP
	SFPNOP
	SFPLOADI	L2, 1, -16921
	SFPMAD	L0, L1, L2, L2, 0
	SFPNOP
	SFPNOP
	SFPLOADI	L0, 4, 0
	SFPSETCC	0, L3, 2
	SFPNOP
	SFPEXEXP	L3, L0, 2
	SFPNOP
	SFPNOP
	SFPNOP
	SFPABS	L0, L0, 0
	SFPNOP
	SFPNOP
	SFPSETSGN	1, L3, L3, 1
	SFPNOP
	SFPNOP
	SFPENCC	3, 10
	SFPNOP
	SFPLOADI	L1, 4, 0
	SFPSETCC	0, L0, 2
	SFPNOP
	SFPLZ	L0, L1, 0
	SFPNOP
	SFPNOP
	SFPNOT	L1, L1
	SFPNOP
	SFPNOP
	SFPIADD	19, L1, L1, 9
	SFPNOP
	SFPNOP
	SFPNOP
	SFPIADD	127, L1, L1, 5
	SFPNOP
	SFPNOP
	SFPENCC	3, 10
	SFPNOP
	SFPSETEXP	0, L3, L1, 0
	SFPNOP
	SFPNOP
	SFPLZ	L0, L3, 0
	SFPNOP
	SFPNOP
	SFPIADD	1, L3, L3, 5
	SFPNOP
	SFPNOP
	SFPSHFT	0, L3, L0, 0
	SFPNOP
	SFPNOP
	SFPSETMAN	0, L1, L0, 0
	SFPNOP
	SFPNOP
	SFPMAD	L0, L5, L2, L0, 0
	SFPNOP
	SFPNOP
	lw	a4,%lo(_ZN7ckernel13instrn_bufferE)(a3)
	sw	a0, 0(a4)	# Op(0x71) - d(lr1)
	SFPMUL	L0, L1, L4, L0, 0
	SFPNOP
	SFPNOP
	SFPLOAD	L1, 1, 0
	SFPSETCC	0, L1, 6
	SFPNOP
	SFPLOADI	L0, 0, -128
	SFPENCC	3, 10
	SFPNOP
	SFPSTORE	L0, 0, 0
	SFPINCRWC	0, 4, 0, 0
	addi	a5,a5,-1
	bne	a5,zero,.L86
	ret
	.size	_ZN7ckernel4sfpu13calculate_logILb1ELb1EEEvm, .-_ZN7ckernel4sfpu13calculate_logILb1ELb1EEEvm
	.section	.text._ZN7ckernel4sfpu20calculate_reciprocalILb0ELi4EEEvv,"axG",@progbits,_ZN7ckernel4sfpu20calculate_reciprocalILb0ELi4EEEvv,comdat
	.align	1
	.weak	_ZN7ckernel4sfpu20calculate_reciprocalILb0ELi4EEEvv
	.type	_ZN7ckernel4sfpu20calculate_reciprocalILb0ELi4EEEvv, @function
_ZN7ckernel4sfpu20calculate_reciprocalILb0ELi4EEEvv:
	li	a5,4
.L89:
	SFPLOAD	L0, 1, 0
	SFPSETSGN	1, L0, L0, 1
	SFPNOP
	SFPNOP
	SFPSETEXP	126, L0, L0, 1
	SFPNOP
	SFPNOP
	SFPLOADI	L2, 0, 16384
	SFPMAD	L0, L7, L2, L1, 0
	SFPNOP
	SFPNOP
	SFPMUL	L7, L1, L4, L1, 0
	SFPNOP
	SFPNOP
	SFPMAD	L0, L1, L2, L3, 0
	SFPNOP
	SFPNOP
	SFPMUL	L1, L3, L4, L1, 0
	SFPNOP
	SFPNOP
	SFPMAD	L0, L1, L2, L0, 0
	SFPNOP
	SFPNOP
	SFPMUL	L1, L0, L4, L1, 0
	SFPNOP
	SFPNOP
	SFPEXEXP	L1, L2, 0
	SFPNOP
	SFPNOP
	SFPLOAD	L0, 1, 0
	SFPEXEXP	L0, L0, 0
	SFPNOP
	SFPNOP
	SFPIADD	0, L2, L0, 6
	SFPNOP
	SFPNOP
	SFPIADD	126, L0, L0, 1
	SFPNOP
	SFPNOP
	SFPNOP
	SFPLOADI	L1, 0, 0
	SFPLOADI	L0, 4, 0
	SFPENCC	3, 10
	SFPNOP
	SFPSETEXP	0, L1, L0, 0
	SFPNOP
	SFPNOP
	SFPLOAD	L1, 1, 0
	SFPSETCC	0, L1, 0
	SFPNOP
	SFPMOV	L0, L0, 1
	SFPNOP
	SFPNOP
	SFPENCC	3, 10
	SFPNOP
	SFPSTORE	L0, 0, 0
	SFPINCRWC	0, 4, 0, 0
	addi	a5,a5,-1
	bne	a5,zero,.L89
	ret
	.size	_ZN7ckernel4sfpu20calculate_reciprocalILb0ELi4EEEvv, .-_ZN7ckernel4sfpu20calculate_reciprocalILb0ELi4EEEvv
	.section	.text._ZN7ckernel4sfpu20calculate_reciprocalILb1ELi4EEEvv,"axG",@progbits,_ZN7ckernel4sfpu20calculate_reciprocalILb1ELi4EEEvv,comdat
	.align	1
	.weak	_ZN7ckernel4sfpu20calculate_reciprocalILb1ELi4EEEvv
	.type	_ZN7ckernel4sfpu20calculate_reciprocalILb1ELi4EEEvv, @function
_ZN7ckernel4sfpu20calculate_reciprocalILb1ELi4EEEvv:
	li	a5,4
.L92:
	SFPLOAD	L0, 1, 0
	SFPSETSGN	1, L0, L0, 1
	SFPNOP
	SFPNOP
	SFPSETEXP	126, L0, L0, 1
	SFPNOP
	SFPNOP
	SFPLOADI	L2, 0, 16384
	SFPMAD	L0, L7, L2, L1, 0
	SFPNOP
	SFPNOP
	SFPMUL	L7, L1, L4, L1, 0
	SFPNOP
	SFPNOP
	SFPMAD	L0, L1, L2, L0, 0
	SFPNOP
	SFPNOP
	SFPMUL	L1, L0, L4, L1, 0
	SFPNOP
	SFPNOP
	SFPEXEXP	L1, L2, 0
	SFPNOP
	SFPNOP
	SFPLOAD	L0, 1, 0
	SFPEXEXP	L0, L0, 0
	SFPNOP
	SFPNOP
	SFPIADD	0, L2, L0, 6
	SFPNOP
	SFPNOP
	SFPIADD	126, L0, L0, 1
	SFPNOP
	SFPNOP
	SFPNOP
	SFPLOADI	L1, 0, 0
	SFPLOADI	L0, 4, 0
	SFPENCC	3, 10
	SFPNOP
	SFPSETEXP	0, L1, L0, 0
	SFPNOP
	SFPNOP
	SFPLOAD	L1, 1, 0
	SFPSETCC	0, L1, 0
	SFPNOP
	SFPMOV	L0, L0, 1
	SFPNOP
	SFPNOP
	SFPENCC	3, 10
	SFPNOP
	SFPSTORE	L0, 0, 0
	SFPINCRWC	0, 4, 0, 0
	addi	a5,a5,-1
	bne	a5,zero,.L92
	ret
	.size	_ZN7ckernel4sfpu20calculate_reciprocalILb1ELi4EEEvv, .-_ZN7ckernel4sfpu20calculate_reciprocalILb1ELi4EEEvv
	.section	.text._ZN7ckernel4sfpu17calculate_dropoutILb0EEEvmm,"axG",@progbits,_ZN7ckernel4sfpu17calculate_dropoutILb0EEEvmm,comdat
	.align	1
	.weak	_ZN7ckernel4sfpu17calculate_dropoutILb0EEEvmm
	.type	_ZN7ckernel4sfpu17calculate_dropoutILb0EEEvmm, @function
_ZN7ckernel4sfpu17calculate_dropoutILb0EEEvmm:
	SFPLOADI	L1, 0, -22197
	li	a4,65536
	addi	a4,a4,-1
	and	a5,a1,a4
	li	a3, 1946157056	# 74000000
	slli	a5,a5,8
	and	a0,a0,a4
	add	a5,a5,a3
	li	a3, 1898053632	# 71220000
	li	a4,4
	lui	a2,%hi(_ZN7ckernel13instrn_bufferE)
	add	a0,a0,a3
.L95:
	lw	a3,%lo(_ZN7ckernel13instrn_bufferE)(a2)
	SFPLOAD	L0, 1, 0
	sw	a5, 0(a3)	# Op(0x74) lv(lr0)  d(lr0)
	SFPNOP
	SFPNOP
	SFPSTORE	L0, 0, 0
	SFPMOV	L3, L0, 0
	SFPNOP
	SFPNOP
	SFPSHFT	-3, L0, L0, 1
	SFPNOP
	SFPNOP
	sw	a0, 0(a3)	# Op(0x71) - d(lr2)
	SFPIADD	0, L0, L2, 2
	SFPNOP
	SFPNOP
	SFPNOP
	SFPSTORE	L4, 0, 0
	SFPENCC	3, 10
	SFPNOP
	SFPMOV	L3, L0, 0
	SFPNOP
	SFPNOP
	SFPSHFT	1, L0, L0, 1
	SFPNOP
	SFPNOP
	SFPSETCC	0, L0, 0
	SFPNOP
	SFPMOV	L3, L0, 0
	SFPNOP
	SFPNOP
	SFPOR	L1, L0
	SFPNOP
	SFPNOP
	SFPAND	L1, L3
	SFPNOP
	SFPNOP
	SFPNOT	L3, L3
	SFPNOP
	SFPNOP
	SFPAND	L0, L3
	SFPNOP
	SFPNOP
	SFPENCC	3, 10
	SFPNOP
	SFPINCRWC	0, 4, 0, 0
	addi	a4,a4,-1
	bne	a4,zero,.L95
	ret
	.size	_ZN7ckernel4sfpu17calculate_dropoutILb0EEEvmm, .-_ZN7ckernel4sfpu17calculate_dropoutILb0EEEvmm
	.globl	__floatunsisf
	.section	.text.startup,"ax",@progbits
	.align	1
	.globl	main
	.type	main, @function
main:
	addi	sp,sp,-16
	sw	s0,8(sp)
	mv	s0,a0
	sw	ra,12(sp)
	SFPMOV	L15, L0, 0
	SFPNOP
	SFPNOP
	SFPSHFT	13, L0, L0, 1
	SFPNOP
	SFPNOP
	slli	a0,a0,16
	srli	a0,a0,16
	call	__floatunsisf
	lui	a5,%hi(_ZN7ckernel13instrn_bufferE)
	lw	a5,%lo(_ZN7ckernel13instrn_bufferE)(a5)
	li	a4, 1896873984	# 71100000
	srli	a0,a0,16
	add	a0,a0,a4
	sw	a0, 0(a5)	# Op(0x71) - d(lr1)
	SFPMOV	L0, L3, 0
	SFPNOP
	SFPNOP
	SFPAND	L1, L3
	SFPNOP
	SFPNOP
	SFPNOT	L3, L3
	SFPNOP
	SFPNOP
	SFPOR	L1, L0
	SFPNOP
	SFPNOP
	SFPAND	L0, L3
	SFPNOP
	SFPNOP
	call	_ZN7ckernel4sfpu14calculate_compILb0EL8SfpuType7EEEvm.isra.0
	mv	a0,s0
	call	_ZN7ckernel4sfpu14calculate_compILb0EL8SfpuType8EEEvm
	call	_ZN7ckernel4sfpu14calculate_compILb0EL8SfpuType9EEEvm.isra.0
	call	_ZN7ckernel4sfpu14calculate_compILb0EL8SfpuType11EEEvm.isra.0
	mv	a0,s0
	call	_ZN7ckernel4sfpu14calculate_compILb0EL8SfpuType10EEEvm
	mv	a2,s0
	mv	a1,s0
	mv	a0,s0
	call	_ZN7ckernel4sfpu15calculate_clampILb0EEEvmmm
	call	_ZN7ckernel4sfpu13calculate_absILb0EEEvv
	li	a0,1
	call	_ZN7ckernel4sfpu14calculate_signILb0EEEvm
	call	_ZN7ckernel4sfpu17calculate_sigmoidILb0EEEvv
	call	_ZN7ckernel4sfpu14calculate_tanhILb1EEEvv
	mv	a2,s0
	mv	a1,s0
	mv	a0,s0
	call	_ZN7ckernel4sfpu18calculate_hardtanhILb0EEEvmmm
	mv	a0,s0
	call	_ZN7ckernel4sfpu15calculate_lreluILb0EEEvm
	call	_ZN7ckernel4sfpu25calculate_tanh_derivativeILb0ELi0EEEvv
	call	_ZN7ckernel4sfpu25calculate_tanh_derivativeILb0ELi1EEEvv
	mv	a0,s0
	call	_ZN7ckernel4sfpu15calculate_powerILb1EEEvm
	call	_ZN7ckernel4sfpu14calculate_geluILb0EEEvv
	call	_ZN7ckernel4sfpu14calculate_geluILb1EEEvv
	call	_ZN7ckernel4sfpu25calculate_gelu_derivativeILb0EEEvv
	call	_ZN7ckernel4sfpu14calculate_sqrtILb0ELi4ELi2EEEvv
	call	_ZN7ckernel4sfpu14calculate_sqrtILb1ELi4ELi2EEEvv
	call	_ZN7ckernel4sfpu21calculate_exponentialILb0ELb0ELb0EEEvs.isra.0
	li	a0,0
	call	_ZN7ckernel4sfpu21calculate_exponentialILb0ELb0ELb1EEEvs
	call	_ZN7ckernel4sfpu21calculate_exponentialILb0ELb1ELb0EEEvs.isra.0
	li	a0,0
	call	_ZN7ckernel4sfpu21calculate_exponentialILb0ELb1ELb1EEEvs
	call	_ZN7ckernel4sfpu21calculate_exponentialILb1ELb0ELb0EEEvs.isra.0
	li	a0,0
	call	_ZN7ckernel4sfpu21calculate_exponentialILb1ELb0ELb1EEEvs
	call	_ZN7ckernel4sfpu21calculate_exponentialILb1ELb1ELb0EEEvs.isra.0
	li	a0,0
	call	_ZN7ckernel4sfpu21calculate_exponentialILb1ELb1ELb1EEEvs
	mv	a0,s0
	call	_ZN7ckernel4sfpu13calculate_logILb1ELb1EEEvm
	call	_ZN7ckernel4sfpu13calculate_logILb1ELb0EEEvm.isra.0
	call	_ZN7ckernel4sfpu20calculate_reciprocalILb0ELi4EEEvv
	call	_ZN7ckernel4sfpu20calculate_reciprocalILb1ELi4EEEvv
	mv	a0,s0
	mv	a1,s0
	call	_ZN7ckernel4sfpu17calculate_dropoutILb0EEEvmm
	lw	ra,12(sp)
	lw	s0,8(sp)
	li	a0,0
	addi	sp,sp,16
	jr	ra
	.size	main, .-main
	.globl	_ZN7ckernel13instrn_bufferE
	.section	.sbss,"aw",@nobits
	.align	2
	.type	_ZN7ckernel13instrn_bufferE, @object
	.size	_ZN7ckernel13instrn_bufferE, 4
_ZN7ckernel13instrn_bufferE:
	.zero	4
	.ident	"GCC: (GNU) 10.2.0"
