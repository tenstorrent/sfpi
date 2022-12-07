	.file	"ckernel.cc"
	.option nopic
	.attribute arch, "rv32i2p0_y"
	.attribute unaligned_access, 0
	.attribute stack_align, 16
	.text
	.align	1
	.type	_ZN7ckernel4sfpu13calculate_logILb1ELb0EEEvm.isra.0, @function
_ZN7ckernel4sfpu13calculate_logILb1ELb0EEEvm.isra.0:
	li	a5,4
.L2:
	SFPLOAD	L3, 0, 1
	SFPSETEXP	L2, L3, 127, 1
	TTNOP
	TTNOP
	SFPLOADI	L0, 11973, 1
	SFPLOADI	L1, -17997, 1
	SFPMAD	L0, L2, L0, L1, 0
	TTNOP
	TTNOP
	SFPLOADI	L1, 16428, 1
	SFPMAD	L0, L2, L0, L1, 0
	TTNOP
	TTNOP
	SFPLOADI	L1, -16921, 1
	SFPMAD	L2, L2, L0, L1, 0
	TTNOP
	TTNOP
	SFPLOADI	L0, 0, 4
	SFPSETCC	L3, 0, 2
	TTNOP
	SFPEXEXP	L0, L3, 2
	TTNOP
	TTNOP
	TTNOP
	SFPABS	L0, L0, 0
	TTNOP
	TTNOP
	SFPSETSGN	L3, L3, 1, 1
	TTNOP
	TTNOP
	SFPENCC	3, 10
	TTNOP
	SFPLOADI	L1, 0, 4
	SFPSETCC	L0, 0, 2
	TTNOP
	SFPLZ	L1, L0, 0
	TTNOP
	TTNOP
	SFPNOT	L1, L1
	TTNOP
	TTNOP
	SFPIADD	L1, L1, 19, 9
	TTNOP
	TTNOP
	TTNOP
	SFPIADD	L1, L1, 127, 5
	TTNOP
	TTNOP
	SFPENCC	3, 10
	TTNOP
	SFPSETEXP	L1, L3, 0, 0
	TTNOP
	TTNOP
	SFPLZ	L3, L0, 0
	TTNOP
	TTNOP
	SFPIADD	L3, L3, 1, 5
	TTNOP
	TTNOP
	SFPSHFT	L0, L3, 0, 0
	TTNOP
	TTNOP
	SFPSETMAN	L0, L1, 0, 0
	TTNOP
	TTNOP
	SFPMAD	L0, L0, L5, L2, 0
	TTNOP
	TTNOP
	SFPLOAD	L1, 0, 1
	SFPSETCC	L1, 0, 6
	TTNOP
	SFPLOADI	L0, -128, 0
	SFPENCC	3, 10
	TTNOP
	SFPSTORE	0, L0, 0
	TTINCRWC	0, 4, 0, 0
	addi	a5,a5,-1
	bne	a5,zero,.L2
	ret
	.size	_ZN7ckernel4sfpu13calculate_logILb1ELb0EEEvm.isra.0, .-_ZN7ckernel4sfpu13calculate_logILb1ELb0EEEvm.isra.0
	.align	1
	.type	_ZN7ckernel4sfpu21calculate_exponentialILb1ELb1ELb0EEEvs.isra.0, @function
_ZN7ckernel4sfpu21calculate_exponentialILb1ELb1ELb0EEEvs.isra.0:
	li	a5,4
.L6:
	SFPLOAD	L1, 0, 1
	SFPMAD	L1, L1, L7, L0, 0
	TTNOP
	TTNOP
	SFPIADD	L1, L2, 0, 4
	TTNOP
	TTNOP
	SFPSHFT	L1, L0, 8, 1
	TTNOP
	TTNOP
	SFPSTORE	0, L1, 0
	SFPSETCC	L1, 0, 0
	TTNOP
	SFPSTORE	0, L4, 0
	SFPENCC	3, 10
	TTNOP
	TTINCRWC	0, 4, 0, 0
	addi	a5,a5,-1
	bne	a5,zero,.L6
	ret
	.size	_ZN7ckernel4sfpu21calculate_exponentialILb1ELb1ELb0EEEvs.isra.0, .-_ZN7ckernel4sfpu21calculate_exponentialILb1ELb1ELb0EEEvs.isra.0
	.align	1
	.type	_ZN7ckernel4sfpu21calculate_exponentialILb1ELb0ELb0EEEvs.isra.0, @function
_ZN7ckernel4sfpu21calculate_exponentialILb1ELb0ELb0EEEvs.isra.0:
	li	a5,4
.L9:
	SFPLOAD	L1, 0, 1
	SFPMAD	L1, L1, L7, L0, 0
	TTNOP
	TTNOP
	SFPIADD	L1, L2, 0, 4
	TTNOP
	TTNOP
	SFPSHFT	L1, L0, 8, 1
	TTNOP
	TTNOP
	SFPSTORE	0, L1, 0
	TTINCRWC	0, 4, 0, 0
	addi	a5,a5,-1
	bne	a5,zero,.L9
	ret
	.size	_ZN7ckernel4sfpu21calculate_exponentialILb1ELb0ELb0EEEvs.isra.0, .-_ZN7ckernel4sfpu21calculate_exponentialILb1ELb0ELb0EEEvs.isra.0
	.align	1
	.type	_ZN7ckernel4sfpu21calculate_exponentialILb0ELb1ELb0EEEvs.isra.0, @function
_ZN7ckernel4sfpu21calculate_exponentialILb0ELb1ELb0EEEvs.isra.0:
	li	a4,4
.L13:
	SFPLOAD	L0, 0, 1
	SFPSETSGN	L0, L0, 0, 1
	TTNOP
	TTNOP
	SFPEXEXP	L1, L0, 10
	TTNOP
	TTNOP
	TTNOP
	SFPSETEXP	L0, L0, 126, 1
	TTNOP
	TTNOP
	SFPENCC	3, 10
	TTNOP
	SFPLOADI	L2, 16221, 0
	SFPMAD	L2, L0, L8, L2, 0
	TTNOP
	TTNOP
	SFPMAD	L0, L0, L2, L10, 0
	TTNOP
	TTNOP
	SFPSETCC	L1, 0, 4
	TTNOP
	SFPMUL	L0, L0, L0, L4, 0
	TTNOP
	TTNOP
	li	a5,7
.L12:
	SFPIADD	L1, L1, -1, 9
	TTNOP
	TTNOP
	TTNOP
	SFPMUL	L0, L0, L0, L4, 0
	TTNOP
	TTNOP
	addi	a5,a5,-1
	bne	a5,zero,.L12
	SFPENCC	3, 10
	TTNOP
	SFPLOAD	L1, 0, 1
	SFPSTORE	0, L0, 0
	SFPSETCC	L1, 0, 0
	TTNOP
	SFPSETSGN	L0, L0, 1, 1
	TTNOP
	TTNOP
	SFPSETEXP	L0, L0, 126, 1
	TTNOP
	TTNOP
	SFPLOADI	L2, 16384, 0
	SFPMAD	L1, L0, L7, L2, 0
	TTNOP
	TTNOP
	SFPMUL	L1, L7, L1, L4, 0
	TTNOP
	TTNOP
	SFPMAD	L3, L0, L1, L2, 0
	TTNOP
	TTNOP
	SFPMUL	L1, L1, L3, L4, 0
	TTNOP
	TTNOP
	SFPMAD	L0, L0, L1, L2, 0
	TTNOP
	TTNOP
	SFPMUL	L0, L1, L0, L4, 0
	TTNOP
	TTNOP
	SFPEXEXP	L2, L0, 0
	TTNOP
	TTNOP
	SFPLOAD	L1, 0, 1
	SFPEXEXP	L1, L1, 0
	TTNOP
	TTNOP
	SFPIADD	L1, L2, 0, 6
	TTNOP
	TTNOP
	SFPPUSHC
	SFPIADD	L1, L1, 126, 1
	TTNOP
	TTNOP
	TTNOP
	SFPLOADI	L0, 0, 0
	SFPLOADI	L1, 0, 4
	SFPPOPC
	TTNOP
	SFPSETEXP	L1, L0, 0, 0
	TTNOP
	TTNOP
	SFPSTORE	0, L1, 0
	SFPENCC	3, 10
	TTNOP
	TTINCRWC	0, 4, 0, 0
	addi	a4,a4,-1
	bne	a4,zero,.L13
	ret
	.size	_ZN7ckernel4sfpu21calculate_exponentialILb0ELb1ELb0EEEvs.isra.0, .-_ZN7ckernel4sfpu21calculate_exponentialILb0ELb1ELb0EEEvs.isra.0
	.align	1
	.type	_ZN7ckernel4sfpu21calculate_exponentialILb0ELb0ELb0EEEvs.isra.0, @function
_ZN7ckernel4sfpu21calculate_exponentialILb0ELb0ELb0EEEvs.isra.0:
	li	a4,4
.L18:
	SFPLOAD	L0, 0, 1
	SFPSETSGN	L0, L0, 0, 1
	TTNOP
	TTNOP
	SFPEXEXP	L1, L0, 10
	TTNOP
	TTNOP
	TTNOP
	SFPSETEXP	L0, L0, 126, 1
	TTNOP
	TTNOP
	SFPENCC	3, 10
	TTNOP
	SFPLOADI	L2, 16221, 0
	SFPMAD	L2, L0, L8, L2, 0
	TTNOP
	TTNOP
	SFPMAD	L0, L0, L2, L10, 0
	TTNOP
	TTNOP
	SFPSETCC	L1, 0, 4
	TTNOP
	SFPMUL	L0, L0, L0, L4, 0
	TTNOP
	TTNOP
	li	a5,7
.L17:
	SFPIADD	L1, L1, -1, 9
	TTNOP
	TTNOP
	TTNOP
	SFPMUL	L0, L0, L0, L4, 0
	TTNOP
	TTNOP
	addi	a5,a5,-1
	bne	a5,zero,.L17
	SFPENCC	3, 10
	TTNOP
	SFPLOAD	L1, 0, 1
	SFPSTORE	0, L0, 0
	SFPSETCC	L1, 0, 0
	TTNOP
	SFPSETSGN	L0, L0, 1, 1
	TTNOP
	TTNOP
	SFPSETEXP	L0, L0, 126, 1
	TTNOP
	TTNOP
	SFPLOADI	L2, 16384, 0
	SFPMAD	L1, L0, L7, L2, 0
	TTNOP
	TTNOP
	SFPMUL	L1, L7, L1, L4, 0
	TTNOP
	TTNOP
	SFPMAD	L3, L0, L1, L2, 0
	TTNOP
	TTNOP
	SFPMUL	L1, L1, L3, L4, 0
	TTNOP
	TTNOP
	SFPMAD	L0, L0, L1, L2, 0
	TTNOP
	TTNOP
	SFPMUL	L0, L1, L0, L4, 0
	TTNOP
	TTNOP
	SFPEXEXP	L2, L0, 0
	TTNOP
	TTNOP
	SFPLOAD	L1, 0, 1
	SFPEXEXP	L1, L1, 0
	TTNOP
	TTNOP
	SFPIADD	L1, L2, 0, 6
	TTNOP
	TTNOP
	SFPPUSHC
	SFPIADD	L1, L1, 126, 1
	TTNOP
	TTNOP
	TTNOP
	SFPLOADI	L0, 0, 0
	SFPLOADI	L1, 0, 4
	SFPPOPC
	TTNOP
	SFPSETEXP	L1, L0, 0, 0
	TTNOP
	TTNOP
	SFPSTORE	0, L1, 0
	SFPENCC	3, 10
	TTNOP
	TTINCRWC	0, 4, 0, 0
	addi	a4,a4,-1
	bne	a4,zero,.L18
	ret
	.size	_ZN7ckernel4sfpu21calculate_exponentialILb0ELb0ELb0EEEvs.isra.0, .-_ZN7ckernel4sfpu21calculate_exponentialILb0ELb0ELb0EEEvs.isra.0
	.align	1
	.type	_ZN7ckernel4sfpu14calculate_compILb0EL8SfpuType11EEEvm.isra.0, @function
_ZN7ckernel4sfpu14calculate_compILb0EL8SfpuType11EEEvm.isra.0:
	SFPLOAD	L0, 0, 1
	SFPSETCC	L0, 0, 0
	TTNOP
	SFPLOADI	L0, 16256, 0
	SFPLOADI	L1, 16256, 0
	SFPCOMPC
	TTNOP
	SFPLOADI	L0, 0, 0
	SFPLOADI	L1, 0, 0
	SFPENCC	3, 10
	TTNOP
	SFPOR	L0, L1
	TTNOP
	TTNOP
	SFPSTORE	0, L0, 0
	TTINCRWC	0, 4, 0, 0
	SFPLOAD	L0, 0, 1
	SFPSETCC	L0, 0, 0
	TTNOP
	SFPLOADI	L0, 16256, 0
	SFPLOADI	L1, 16256, 0
	SFPCOMPC
	TTNOP
	SFPLOADI	L0, 0, 0
	SFPLOADI	L1, 0, 0
	SFPENCC	3, 10
	TTNOP
	SFPOR	L0, L1
	TTNOP
	TTNOP
	SFPSTORE	0, L0, 0
	TTINCRWC	0, 4, 0, 0
	SFPLOAD	L0, 0, 1
	SFPSETCC	L0, 0, 0
	TTNOP
	SFPLOADI	L0, 16256, 0
	SFPLOADI	L1, 16256, 0
	SFPCOMPC
	TTNOP
	SFPLOADI	L0, 0, 0
	SFPLOADI	L1, 0, 0
	SFPENCC	3, 10
	TTNOP
	SFPOR	L0, L1
	TTNOP
	TTNOP
	SFPSTORE	0, L0, 0
	TTINCRWC	0, 4, 0, 0
	SFPLOAD	L0, 0, 1
	SFPSETCC	L0, 0, 0
	TTNOP
	SFPLOADI	L0, 16256, 0
	SFPLOADI	L1, 16256, 0
	SFPCOMPC
	TTNOP
	SFPLOADI	L0, 0, 0
	SFPLOADI	L1, 0, 0
	SFPENCC	3, 10
	TTNOP
	SFPOR	L0, L1
	TTNOP
	TTNOP
	SFPSTORE	0, L0, 0
	TTINCRWC	0, 4, 0, 0
	ret
	.size	_ZN7ckernel4sfpu14calculate_compILb0EL8SfpuType11EEEvm.isra.0, .-_ZN7ckernel4sfpu14calculate_compILb0EL8SfpuType11EEEvm.isra.0
	.align	1
	.type	_ZN7ckernel4sfpu14calculate_compILb0EL8SfpuType9EEEvm.isra.0, @function
_ZN7ckernel4sfpu14calculate_compILb0EL8SfpuType9EEEvm.isra.0:
	SFPLOAD	L0, 0, 1
	SFPSETCC	L0, 0, 0
	TTNOP
	SFPLOADI	L0, 0, 0
	SFPLOADI	L1, 0, 0
	SFPCOMPC
	TTNOP
	SFPLOADI	L0, 16256, 0
	SFPLOADI	L1, 16256, 0
	SFPENCC	3, 10
	TTNOP
	SFPAND	L0, L1
	TTNOP
	TTNOP
	SFPSTORE	0, L0, 0
	TTINCRWC	0, 4, 0, 0
	SFPLOAD	L0, 0, 1
	SFPSETCC	L0, 0, 0
	TTNOP
	SFPLOADI	L0, 0, 0
	SFPLOADI	L1, 0, 0
	SFPCOMPC
	TTNOP
	SFPLOADI	L0, 16256, 0
	SFPLOADI	L1, 16256, 0
	SFPENCC	3, 10
	TTNOP
	SFPAND	L0, L1
	TTNOP
	TTNOP
	SFPSTORE	0, L0, 0
	TTINCRWC	0, 4, 0, 0
	SFPLOAD	L0, 0, 1
	SFPSETCC	L0, 0, 0
	TTNOP
	SFPLOADI	L0, 0, 0
	SFPLOADI	L1, 0, 0
	SFPCOMPC
	TTNOP
	SFPLOADI	L0, 16256, 0
	SFPLOADI	L1, 16256, 0
	SFPENCC	3, 10
	TTNOP
	SFPAND	L0, L1
	TTNOP
	TTNOP
	SFPSTORE	0, L0, 0
	TTINCRWC	0, 4, 0, 0
	SFPLOAD	L0, 0, 1
	SFPSETCC	L0, 0, 0
	TTNOP
	SFPLOADI	L0, 0, 0
	SFPLOADI	L1, 0, 0
	SFPCOMPC
	TTNOP
	SFPLOADI	L0, 16256, 0
	SFPLOADI	L1, 16256, 0
	SFPENCC	3, 10
	TTNOP
	SFPAND	L0, L1
	TTNOP
	TTNOP
	SFPSTORE	0, L0, 0
	TTINCRWC	0, 4, 0, 0
	ret
	.size	_ZN7ckernel4sfpu14calculate_compILb0EL8SfpuType9EEEvm.isra.0, .-_ZN7ckernel4sfpu14calculate_compILb0EL8SfpuType9EEEvm.isra.0
	.align	1
	.type	_ZN7ckernel4sfpu14calculate_compILb0EL8SfpuType7EEEvm.isra.0, @function
_ZN7ckernel4sfpu14calculate_compILb0EL8SfpuType7EEEvm.isra.0:
	SFPLOAD	L0, 0, 1
	SFPSETCC	L0, 0, 0
	TTNOP
	SFPLOADI	L0, 0, 0
	SFPCOMPC
	TTNOP
	SFPLOADI	L0, 16256, 0
	SFPENCC	3, 10
	TTNOP
	SFPSTORE	0, L0, 0
	TTINCRWC	0, 4, 0, 0
	SFPLOAD	L0, 0, 1
	SFPSETCC	L0, 0, 0
	TTNOP
	SFPLOADI	L0, 0, 0
	SFPCOMPC
	TTNOP
	SFPLOADI	L0, 16256, 0
	SFPENCC	3, 10
	TTNOP
	SFPSTORE	0, L0, 0
	TTINCRWC	0, 4, 0, 0
	SFPLOAD	L0, 0, 1
	SFPSETCC	L0, 0, 0
	TTNOP
	SFPLOADI	L0, 0, 0
	SFPCOMPC
	TTNOP
	SFPLOADI	L0, 16256, 0
	SFPENCC	3, 10
	TTNOP
	SFPSTORE	0, L0, 0
	TTINCRWC	0, 4, 0, 0
	SFPLOAD	L0, 0, 1
	SFPSETCC	L0, 0, 0
	TTNOP
	SFPLOADI	L0, 0, 0
	SFPCOMPC
	TTNOP
	SFPLOADI	L0, 16256, 0
	SFPENCC	3, 10
	TTNOP
	SFPSTORE	0, L0, 0
	TTINCRWC	0, 4, 0, 0
	ret
	.size	_ZN7ckernel4sfpu14calculate_compILb0EL8SfpuType7EEEvm.isra.0, .-_ZN7ckernel4sfpu14calculate_compILb0EL8SfpuType7EEEvm.isra.0
	.section	.text._ZN7ckernel4sfpu14calculate_compILb0EL8SfpuType8EEEvm,"axG",@progbits,_ZN7ckernel4sfpu14calculate_compILb0EL8SfpuType8EEEvm,comdat
	.align	1
	.weak	_ZN7ckernel4sfpu14calculate_compILb0EL8SfpuType8EEEvm
	.type	_ZN7ckernel4sfpu14calculate_compILb0EL8SfpuType8EEEvm, @function
_ZN7ckernel4sfpu14calculate_compILb0EL8SfpuType8EEEvm:
	bne	a0,zero,.L25
	SFPLOAD	L1, 0, 1
	SFPLOADI	L0, 14336, 4
	SFPIADD	L0, L1, 0, 4
	TTNOP
	TTNOP
	SFPSETCC	L0, 0, 6
	TTNOP
	SFPLOADI	L0, 0, 0
	SFPCOMPC
	TTNOP
	SFPLOADI	L0, 16256, 0
	SFPENCC	3, 10
	TTNOP
	SFPSTORE	0, L0, 0
	TTINCRWC	0, 4, 0, 0
	SFPLOAD	L1, 0, 1
	SFPLOADI	L0, 14336, 4
	SFPIADD	L0, L1, 0, 4
	TTNOP
	TTNOP
	SFPSETCC	L0, 0, 6
	TTNOP
	SFPLOADI	L0, 0, 0
	SFPCOMPC
	TTNOP
	SFPLOADI	L0, 16256, 0
	SFPENCC	3, 10
	TTNOP
	SFPSTORE	0, L0, 0
	TTINCRWC	0, 4, 0, 0
	SFPLOAD	L1, 0, 1
	SFPLOADI	L0, 14336, 4
	SFPIADD	L0, L1, 0, 4
	TTNOP
	TTNOP
	SFPSETCC	L0, 0, 6
	TTNOP
	SFPLOADI	L0, 0, 0
	SFPCOMPC
	TTNOP
	SFPLOADI	L0, 16256, 0
	SFPENCC	3, 10
	TTNOP
	SFPSTORE	0, L0, 0
	TTINCRWC	0, 4, 0, 0
	SFPLOAD	L1, 0, 1
	SFPLOADI	L0, 14336, 4
	SFPIADD	L0, L1, 0, 4
	TTNOP
	TTNOP
	SFPSETCC	L0, 0, 6
	TTNOP
	SFPLOADI	L0, 0, 0
	SFPCOMPC
	TTNOP
	SFPLOADI	L0, 16256, 0
	SFPENCC	3, 10
	TTNOP
	SFPSTORE	0, L0, 0
	TTINCRWC	0, 4, 0, 0
	ret
.L25:
	SFPLOAD	L0, 0, 1
	SFPSETCC	L0, 0, 6
	TTNOP
	SFPLOADI	L0, 0, 0
	SFPCOMPC
	TTNOP
	SFPLOADI	L0, 16256, 0
	SFPENCC	3, 10
	TTNOP
	SFPSTORE	0, L0, 0
	TTINCRWC	0, 4, 0, 0
	SFPLOAD	L0, 0, 1
	SFPSETCC	L0, 0, 6
	TTNOP
	SFPLOADI	L0, 0, 0
	SFPCOMPC
	TTNOP
	SFPLOADI	L0, 16256, 0
	SFPENCC	3, 10
	TTNOP
	SFPSTORE	0, L0, 0
	TTINCRWC	0, 4, 0, 0
	SFPLOAD	L0, 0, 1
	SFPSETCC	L0, 0, 6
	TTNOP
	SFPLOADI	L0, 0, 0
	SFPCOMPC
	TTNOP
	SFPLOADI	L0, 16256, 0
	SFPENCC	3, 10
	TTNOP
	SFPSTORE	0, L0, 0
	TTINCRWC	0, 4, 0, 0
	SFPLOAD	L0, 0, 1
	SFPSETCC	L0, 0, 6
	TTNOP
	SFPLOADI	L0, 0, 0
	SFPCOMPC
	TTNOP
	SFPLOADI	L0, 16256, 0
	SFPENCC	3, 10
	TTNOP
	SFPSTORE	0, L0, 0
	TTINCRWC	0, 4, 0, 0
	ret
	.size	_ZN7ckernel4sfpu14calculate_compILb0EL8SfpuType8EEEvm, .-_ZN7ckernel4sfpu14calculate_compILb0EL8SfpuType8EEEvm
	.section	.text._ZN7ckernel4sfpu14calculate_compILb0EL8SfpuType10EEEvm,"axG",@progbits,_ZN7ckernel4sfpu14calculate_compILb0EL8SfpuType10EEEvm,comdat
	.align	1
	.weak	_ZN7ckernel4sfpu14calculate_compILb0EL8SfpuType10EEEvm
	.type	_ZN7ckernel4sfpu14calculate_compILb0EL8SfpuType10EEEvm, @function
_ZN7ckernel4sfpu14calculate_compILb0EL8SfpuType10EEEvm:
	bne	a0,zero,.L29
	SFPLOAD	L1, 0, 1
	SFPLOADI	L0, 14336, 4
	SFPIADD	L0, L1, 0, 4
	TTNOP
	TTNOP
	SFPSETCC	L0, 0, 6
	TTNOP
	SFPLOADI	L0, 16256, 0
	SFPCOMPC
	TTNOP
	SFPLOADI	L0, 0, 0
	SFPENCC	3, 10
	TTNOP
	SFPSTORE	0, L0, 0
	TTINCRWC	0, 4, 0, 0
	SFPLOAD	L1, 0, 1
	SFPLOADI	L0, 14336, 4
	SFPIADD	L0, L1, 0, 4
	TTNOP
	TTNOP
	SFPSETCC	L0, 0, 6
	TTNOP
	SFPLOADI	L0, 16256, 0
	SFPCOMPC
	TTNOP
	SFPLOADI	L0, 0, 0
	SFPENCC	3, 10
	TTNOP
	SFPSTORE	0, L0, 0
	TTINCRWC	0, 4, 0, 0
	SFPLOAD	L1, 0, 1
	SFPLOADI	L0, 14336, 4
	SFPIADD	L0, L1, 0, 4
	TTNOP
	TTNOP
	SFPSETCC	L0, 0, 6
	TTNOP
	SFPLOADI	L0, 16256, 0
	SFPCOMPC
	TTNOP
	SFPLOADI	L0, 0, 0
	SFPENCC	3, 10
	TTNOP
	SFPSTORE	0, L0, 0
	TTINCRWC	0, 4, 0, 0
	SFPLOAD	L1, 0, 1
	SFPLOADI	L0, 14336, 4
	SFPIADD	L0, L1, 0, 4
	TTNOP
	TTNOP
	SFPSETCC	L0, 0, 6
	TTNOP
	SFPLOADI	L0, 16256, 0
	SFPCOMPC
	TTNOP
	SFPLOADI	L0, 0, 0
	SFPENCC	3, 10
	TTNOP
	SFPSTORE	0, L0, 0
	TTINCRWC	0, 4, 0, 0
	ret
.L29:
	SFPLOAD	L0, 0, 1
	SFPSETCC	L0, 0, 6
	TTNOP
	SFPLOADI	L0, 16256, 0
	SFPCOMPC
	TTNOP
	SFPLOADI	L0, 0, 0
	SFPENCC	3, 10
	TTNOP
	SFPSTORE	0, L0, 0
	TTINCRWC	0, 4, 0, 0
	SFPLOAD	L0, 0, 1
	SFPSETCC	L0, 0, 6
	TTNOP
	SFPLOADI	L0, 16256, 0
	SFPCOMPC
	TTNOP
	SFPLOADI	L0, 0, 0
	SFPENCC	3, 10
	TTNOP
	SFPSTORE	0, L0, 0
	TTINCRWC	0, 4, 0, 0
	SFPLOAD	L0, 0, 1
	SFPSETCC	L0, 0, 6
	TTNOP
	SFPLOADI	L0, 16256, 0
	SFPCOMPC
	TTNOP
	SFPLOADI	L0, 0, 0
	SFPENCC	3, 10
	TTNOP
	SFPSTORE	0, L0, 0
	TTINCRWC	0, 4, 0, 0
	SFPLOAD	L0, 0, 1
	SFPSETCC	L0, 0, 6
	TTNOP
	SFPLOADI	L0, 16256, 0
	SFPCOMPC
	TTNOP
	SFPLOADI	L0, 0, 0
	SFPENCC	3, 10
	TTNOP
	SFPSTORE	0, L0, 0
	TTINCRWC	0, 4, 0, 0
	ret
	.size	_ZN7ckernel4sfpu14calculate_compILb0EL8SfpuType10EEEvm, .-_ZN7ckernel4sfpu14calculate_compILb0EL8SfpuType10EEEvm
	.section	.text._ZN7ckernel4sfpu15calculate_clampILb0EEEvmmm,"axG",@progbits,_ZN7ckernel4sfpu15calculate_clampILb0EEEvmmm,comdat
	.align	1
	.weak	_ZN7ckernel4sfpu15calculate_clampILb0EEEvmmm
	.type	_ZN7ckernel4sfpu15calculate_clampILb0EEEvmmm, @function
_ZN7ckernel4sfpu15calculate_clampILb0EEEvmmm:
	lui	a6,%hi(_ZN7ckernel13instrn_bufferE)
	li	a4,65536
	addi	a4,a4,-1
	lw	a7,%lo(_ZN7ckernel13instrn_bufferE)(a6)
	and	a0,a0,a4
	li	a5, 1897988096	# 71210000
	add	a5,a0,a5
	sw	a5, 0(a7)	# Op(0x71) - d(lr2)
	and	a5,a1,a4
	li	a3, 1896939520	# 71110000
	add	a3,a5,a3
	sw	a3, 0(a7)	# Op(0x71) - d(lr1)
	and	a3,a2,a4
	li	a2, 1895890944	# 71010000
	slli	a3,a3,8
	add	a1,a0,a2
	add	a2,a5,a2
	li	a5, 1962934272	# 75000000
	li	a4,4
	add	a3,a3,a5
.L33:
	SFPLOAD	L0, 0, 1
	SFPMAD	L3, L2, L11, L0, 0
	TTNOP
	TTNOP
	SFPSETCC	L3, 0, 0
	TTNOP
	lw	a5,%lo(_ZN7ckernel13instrn_bufferE)(a6)
	sw	a1, 0(a5)	# Op(0x71) lv(lr0)  d(lr0)
	SFPCOMPC
	TTNOP
	SFPMAD	L3, L1, L11, L0, 0
	TTNOP
	TTNOP
	SFPSETCC	L3, 0, 4
	TTNOP
	sw	a2, 0(a5)	# Op(0x71) lv(lr0)  d(lr0)
	SFPENCC	3, 10
	TTNOP
	sw	a3, 0(a5)	# Op(0x75) lv(lr0)  d(lr0)
	TTNOP
	TTNOP
	SFPSTORE	0, L0, 0
	TTINCRWC	0, 4, 0, 0
	addi	a4,a4,-1
	bne	a4,zero,.L33
	ret
	.size	_ZN7ckernel4sfpu15calculate_clampILb0EEEvmmm, .-_ZN7ckernel4sfpu15calculate_clampILb0EEEvmmm
	.section	.text._ZN7ckernel4sfpu13calculate_absILb0EEEvv,"axG",@progbits,_ZN7ckernel4sfpu13calculate_absILb0EEEvv,comdat
	.align	1
	.weak	_ZN7ckernel4sfpu13calculate_absILb0EEEvv
	.type	_ZN7ckernel4sfpu13calculate_absILb0EEEvv, @function
_ZN7ckernel4sfpu13calculate_absILb0EEEvv:
	SFPLOAD	L0, 0, 1
	SFPABS	L0, L0, 1
	TTNOP
	TTNOP
	SFPSTORE	0, L0, 0
	TTINCRWC	0, 4, 0, 0
	SFPLOAD	L0, 0, 1
	SFPABS	L0, L0, 1
	TTNOP
	TTNOP
	SFPSTORE	0, L0, 0
	TTINCRWC	0, 4, 0, 0
	SFPLOAD	L0, 0, 1
	SFPABS	L0, L0, 1
	TTNOP
	TTNOP
	SFPSTORE	0, L0, 0
	TTINCRWC	0, 4, 0, 0
	SFPLOAD	L0, 0, 1
	SFPABS	L0, L0, 1
	TTNOP
	TTNOP
	SFPSTORE	0, L0, 0
	TTINCRWC	0, 4, 0, 0
	ret
	.size	_ZN7ckernel4sfpu13calculate_absILb0EEEvv, .-_ZN7ckernel4sfpu13calculate_absILb0EEEvv
	.section	.text._ZN7ckernel4sfpu14calculate_signILb0EEEvm,"axG",@progbits,_ZN7ckernel4sfpu14calculate_signILb0EEEvm,comdat
	.align	1
	.weak	_ZN7ckernel4sfpu14calculate_signILb0EEEvm
	.type	_ZN7ckernel4sfpu14calculate_signILb0EEEvm, @function
_ZN7ckernel4sfpu14calculate_signILb0EEEvm:
	bne	a0,zero,.L37
	SFPLOAD	L1, 0, 1
	SFPSTORE	0, L10, 0
	SFPSETCC	L1, 0, 0
	TTNOP
	SFPSTORE	0, L11, 0
	SFPENCC	3, 10
	TTNOP
	SFPLOADI	L0, 14336, 4
	SFPIADD	L0, L1, 0, 4
	TTNOP
	TTNOP
	SFPSETCC	L0, 0, 6
	TTNOP
	SFPSTORE	0, L4, 0
	SFPENCC	3, 10
	TTNOP
	TTINCRWC	0, 4, 0, 0
	SFPLOAD	L1, 0, 1
	SFPSTORE	0, L10, 0
	SFPSETCC	L1, 0, 0
	TTNOP
	SFPSTORE	0, L11, 0
	SFPENCC	3, 10
	TTNOP
	SFPLOADI	L0, 14336, 4
	SFPIADD	L0, L1, 0, 4
	TTNOP
	TTNOP
	SFPSETCC	L0, 0, 6
	TTNOP
	SFPSTORE	0, L4, 0
	SFPENCC	3, 10
	TTNOP
	TTINCRWC	0, 4, 0, 0
	SFPLOAD	L1, 0, 1
	SFPSTORE	0, L10, 0
	SFPSETCC	L1, 0, 0
	TTNOP
	SFPSTORE	0, L11, 0
	SFPENCC	3, 10
	TTNOP
	SFPLOADI	L0, 14336, 4
	SFPIADD	L0, L1, 0, 4
	TTNOP
	TTNOP
	SFPSETCC	L0, 0, 6
	TTNOP
	SFPSTORE	0, L4, 0
	SFPENCC	3, 10
	TTNOP
	TTINCRWC	0, 4, 0, 0
	SFPLOAD	L1, 0, 1
	SFPSTORE	0, L10, 0
	SFPSETCC	L1, 0, 0
	TTNOP
	SFPSTORE	0, L11, 0
	SFPENCC	3, 10
	TTNOP
	SFPLOADI	L0, 14336, 4
	SFPIADD	L0, L1, 0, 4
	TTNOP
	TTNOP
	SFPSETCC	L0, 0, 6
	TTNOP
	SFPSTORE	0, L4, 0
	SFPENCC	3, 10
	TTNOP
	TTINCRWC	0, 4, 0, 0
	ret
.L37:
	SFPLOAD	L0, 0, 1
	SFPSTORE	0, L10, 0
	SFPSETCC	L0, 0, 0
	TTNOP
	SFPSTORE	0, L11, 0
	SFPENCC	3, 10
	TTNOP
	SFPSETCC	L0, 0, 6
	TTNOP
	SFPSTORE	0, L4, 0
	SFPENCC	3, 10
	TTNOP
	TTINCRWC	0, 4, 0, 0
	SFPLOAD	L0, 0, 1
	SFPSTORE	0, L10, 0
	SFPSETCC	L0, 0, 0
	TTNOP
	SFPSTORE	0, L11, 0
	SFPENCC	3, 10
	TTNOP
	SFPSETCC	L0, 0, 6
	TTNOP
	SFPSTORE	0, L4, 0
	SFPENCC	3, 10
	TTNOP
	TTINCRWC	0, 4, 0, 0
	SFPLOAD	L0, 0, 1
	SFPSTORE	0, L10, 0
	SFPSETCC	L0, 0, 0
	TTNOP
	SFPSTORE	0, L11, 0
	SFPENCC	3, 10
	TTNOP
	SFPSETCC	L0, 0, 6
	TTNOP
	SFPSTORE	0, L4, 0
	SFPENCC	3, 10
	TTNOP
	TTINCRWC	0, 4, 0, 0
	SFPLOAD	L0, 0, 1
	SFPSTORE	0, L10, 0
	SFPSETCC	L0, 0, 0
	TTNOP
	SFPSTORE	0, L11, 0
	SFPENCC	3, 10
	TTNOP
	SFPSETCC	L0, 0, 6
	TTNOP
	SFPSTORE	0, L4, 0
	SFPENCC	3, 10
	TTNOP
	TTINCRWC	0, 4, 0, 0
	ret
	.size	_ZN7ckernel4sfpu14calculate_signILb0EEEvm, .-_ZN7ckernel4sfpu14calculate_signILb0EEEvm
	.section	.text._ZN7ckernel4sfpu17calculate_sigmoidILb0EEEvv,"axG",@progbits,_ZN7ckernel4sfpu17calculate_sigmoidILb0EEEvv,comdat
	.align	1
	.weak	_ZN7ckernel4sfpu17calculate_sigmoidILb0EEEvv
	.type	_ZN7ckernel4sfpu17calculate_sigmoidILb0EEEvv, @function
_ZN7ckernel4sfpu17calculate_sigmoidILb0EEEvv:
	SFPLOAD	L3, 0, 1
	SFPLUT	L3, 4
	TTNOP
	TTNOP
	SFPADDI	L3, 16128, 0
	TTNOP
	TTNOP
	SFPSTORE	0, L3, 0
	TTINCRWC	0, 4, 0, 0
	SFPLOAD	L3, 0, 1
	SFPLUT	L3, 4
	TTNOP
	TTNOP
	SFPADDI	L3, 16128, 0
	TTNOP
	TTNOP
	SFPSTORE	0, L3, 0
	TTINCRWC	0, 4, 0, 0
	SFPLOAD	L3, 0, 1
	SFPLUT	L3, 4
	TTNOP
	TTNOP
	SFPADDI	L3, 16128, 0
	TTNOP
	TTNOP
	SFPSTORE	0, L3, 0
	TTINCRWC	0, 4, 0, 0
	SFPLOAD	L3, 0, 1
	SFPLUT	L3, 4
	TTNOP
	TTNOP
	SFPADDI	L3, 16128, 0
	TTNOP
	TTNOP
	SFPSTORE	0, L3, 0
	TTINCRWC	0, 4, 0, 0
	ret
	.size	_ZN7ckernel4sfpu17calculate_sigmoidILb0EEEvv, .-_ZN7ckernel4sfpu17calculate_sigmoidILb0EEEvv
	.section	.text._ZN7ckernel4sfpu14calculate_tanhILb1EEEvv,"axG",@progbits,_ZN7ckernel4sfpu14calculate_tanhILb1EEEvv,comdat
	.align	1
	.weak	_ZN7ckernel4sfpu14calculate_tanhILb1EEEvv
	.type	_ZN7ckernel4sfpu14calculate_tanhILb1EEEvv, @function
_ZN7ckernel4sfpu14calculate_tanhILb1EEEvv:
	SFPLOAD	L3, 0, 1
	SFPLUT	L3, 4
	TTNOP
	TTNOP
	SFPSTORE	0, L3, 0
	TTINCRWC	0, 4, 0, 0
	SFPLOAD	L3, 0, 1
	SFPLUT	L3, 4
	TTNOP
	TTNOP
	SFPSTORE	0, L3, 0
	TTINCRWC	0, 4, 0, 0
	SFPLOAD	L3, 0, 1
	SFPLUT	L3, 4
	TTNOP
	TTNOP
	SFPSTORE	0, L3, 0
	TTINCRWC	0, 4, 0, 0
	SFPLOAD	L3, 0, 1
	SFPLUT	L3, 4
	TTNOP
	TTNOP
	SFPSTORE	0, L3, 0
	TTINCRWC	0, 4, 0, 0
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
	and	a5,a2,a5
	li	a3, 1896873984	# 71100000
	add	a5,a5,a3
	sw	a5, 0(a4)	# Op(0x71) - d(lr1)
	li	a5,4
.L43:
	SFPLOAD	L0, 0, 1
	SFPADD	L0, L10, L0, L3, 0
	TTNOP
	TTNOP
	SFPSETCC	L0, 0, 0
	TTNOP
	SFPLOADI	L0, 0, 0
	SFPENCC	3, 10
	TTNOP
	SFPADD	L0, L10, L0, L2, 0
	TTNOP
	TTNOP
	SFPSETCC	L0, 0, 4
	TTNOP
	SFPLOADI	L0, 0, 0
	SFPENCC	3, 10
	TTNOP
	SFPADD	L0, L10, L0, L1, 0
	TTNOP
	TTNOP
	SFPSTORE	0, L0, 0
	TTINCRWC	0, 4, 0, 0
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
	lw	a4,%lo(_ZN7ckernel13instrn_bufferE)(a5)
	slli	a5,a0,16
	li	a3, 1896873984	# 71100000
	srli	a5,a5,16
	add	a5,a5,a3
	sw	a5, 0(a4)	# Op(0x71) - d(lr1)
	li	a5,4
.L46:
	SFPLOAD	L0, 0, 1
	SFPSETCC	L0, 0, 0
	TTNOP
	SFPMUL	L0, L0, L1, L4, 0
	TTNOP
	TTNOP
	SFPENCC	3, 10
	TTNOP
	SFPSTORE	0, L0, 0
	TTINCRWC	0, 4, 0, 0
	addi	a5,a5,-1
	bne	a5,zero,.L46
	ret
	.size	_ZN7ckernel4sfpu15calculate_lreluILb0EEEvm, .-_ZN7ckernel4sfpu15calculate_lreluILb0EEEvm
	.section	.text._ZN7ckernel4sfpu25calculate_tanh_derivativeILb0ELi0EEEvv,"axG",@progbits,_ZN7ckernel4sfpu25calculate_tanh_derivativeILb0ELi0EEEvv,comdat
	.align	1
	.weak	_ZN7ckernel4sfpu25calculate_tanh_derivativeILb0ELi0EEEvv
	.type	_ZN7ckernel4sfpu25calculate_tanh_derivativeILb0ELi0EEEvv, @function
_ZN7ckernel4sfpu25calculate_tanh_derivativeILb0ELi0EEEvv:
	SFPLOAD	L3, 0, 1
	SFPLUT	L3, 4
	TTNOP
	TTNOP
	SFPMUL	L3, L3, L3, L4, 0
	TTNOP
	TTNOP
	SFPMAD	L3, L11, L3, L10, 0
	TTNOP
	TTNOP
	SFPSTORE	0, L3, 0
	TTINCRWC	0, 4, 0, 0
	SFPLOAD	L3, 0, 1
	SFPLUT	L3, 4
	TTNOP
	TTNOP
	SFPMUL	L3, L3, L3, L4, 0
	TTNOP
	TTNOP
	SFPMAD	L3, L11, L3, L10, 0
	TTNOP
	TTNOP
	SFPSTORE	0, L3, 0
	TTINCRWC	0, 4, 0, 0
	SFPLOAD	L3, 0, 1
	SFPLUT	L3, 4
	TTNOP
	TTNOP
	SFPMUL	L3, L3, L3, L4, 0
	TTNOP
	TTNOP
	SFPMAD	L3, L11, L3, L10, 0
	TTNOP
	TTNOP
	SFPSTORE	0, L3, 0
	TTINCRWC	0, 4, 0, 0
	SFPLOAD	L3, 0, 1
	SFPLUT	L3, 4
	TTNOP
	TTNOP
	SFPMUL	L3, L3, L3, L4, 0
	TTNOP
	TTNOP
	SFPMAD	L3, L11, L3, L10, 0
	TTNOP
	TTNOP
	SFPSTORE	0, L3, 0
	TTINCRWC	0, 4, 0, 0
	ret
	.size	_ZN7ckernel4sfpu25calculate_tanh_derivativeILb0ELi0EEEvv, .-_ZN7ckernel4sfpu25calculate_tanh_derivativeILb0ELi0EEEvv
	.section	.text._ZN7ckernel4sfpu25calculate_tanh_derivativeILb0ELi1EEEvv,"axG",@progbits,_ZN7ckernel4sfpu25calculate_tanh_derivativeILb0ELi1EEEvv,comdat
	.align	1
	.weak	_ZN7ckernel4sfpu25calculate_tanh_derivativeILb0ELi1EEEvv
	.type	_ZN7ckernel4sfpu25calculate_tanh_derivativeILb0ELi1EEEvv, @function
_ZN7ckernel4sfpu25calculate_tanh_derivativeILb0ELi1EEEvv:
	SFPLOAD	L3, 0, 1
	SFPMUL	L3, L3, L3, L4, 0
	TTNOP
	TTNOP
	SFPMAD	L3, L11, L3, L10, 0
	TTNOP
	TTNOP
	SFPSTORE	0, L3, 0
	TTINCRWC	0, 4, 0, 0
	SFPLOAD	L3, 0, 1
	SFPMUL	L3, L3, L3, L4, 0
	TTNOP
	TTNOP
	SFPMAD	L3, L11, L3, L10, 0
	TTNOP
	TTNOP
	SFPSTORE	0, L3, 0
	TTINCRWC	0, 4, 0, 0
	SFPLOAD	L3, 0, 1
	SFPMUL	L3, L3, L3, L4, 0
	TTNOP
	TTNOP
	SFPMAD	L3, L11, L3, L10, 0
	TTNOP
	TTNOP
	SFPSTORE	0, L3, 0
	TTINCRWC	0, 4, 0, 0
	SFPLOAD	L3, 0, 1
	SFPMUL	L3, L3, L3, L4, 0
	TTNOP
	TTNOP
	SFPMAD	L3, L11, L3, L10, 0
	TTNOP
	TTNOP
	SFPSTORE	0, L3, 0
	TTINCRWC	0, 4, 0, 0
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
	SFPLOAD	L1, 0, 1
	SFPMUL	L0, L1, L1, L4, 0
	TTNOP
	TTNOP
	bleu	a0,a3,.L51
	li	a5,2
.L52:
	SFPMUL	L0, L0, L1, L4, 0
	TTNOP
	TTNOP
	addi	a5,a5,1
	bne	a0,a5,.L52
.L51:
	SFPSTORE	0, L0, 0
	TTINCRWC	0, 4, 0, 0
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
	SFPLOAD	L1, 0, 1
	SFPMUL	L3, L1, L1, L4, 0
	TTNOP
	TTNOP
	SFPMUL	L3, L3, L1, L4, 0
	TTNOP
	TTNOP
	SFPLOADI	L2, 15671, 0
	SFPMAD	L3, L3, L2, L1, 0
	TTNOP
	TTNOP
	SFPMULI	L3, 16204, 0
	TTNOP
	TTNOP
	SFPLOADI	L1, 8208, 2
	SFPLOADI	L2, -256, 2
	SFPLUT	L3, 4
	TTNOP
	TTNOP
	SFPLOAD	L1, 0, 1
	SFPMAD	L1, L1, L3, L1, 0
	TTNOP
	TTNOP
	SFPMULI	L1, 16128, 0
	TTNOP
	TTNOP
	SFPSTORE	0, L1, 0
	TTINCRWC	0, 4, 0, 0
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
	SFPLOAD	L3, 0, 1
	SFPLOADI	L1, 8492, 2
	SFPLOADI	L2, -256, 2
	SFPLUT	L3, 4
	TTNOP
	TTNOP
	SFPLOAD	L1, 0, 1
	SFPMAD	L1, L1, L3, L1, 0
	TTNOP
	TTNOP
	SFPMULI	L1, 16128, 0
	TTNOP
	TTNOP
	SFPSTORE	0, L1, 0
	TTINCRWC	0, 4, 0, 0
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
	SFPLOAD	L3, 0, 1
	SFPMUL	L0, L3, L3, L4, 0
	TTNOP
	TTNOP
	SFPMUL	L0, L0, L9, L4, 0
	TTNOP
	TTNOP
	SFPSTORE	0, L0, 0
	SFPSETSGN	L0, L0, 0, 1
	TTNOP
	TTNOP
	SFPEXEXP	L1, L0, 10
	TTNOP
	TTNOP
	TTNOP
	SFPSETEXP	L0, L0, 126, 1
	TTNOP
	TTNOP
	SFPENCC	3, 10
	TTNOP
	SFPLOADI	L2, 16221, 0
	SFPMAD	L2, L0, L8, L2, 0
	TTNOP
	TTNOP
	SFPMAD	L0, L0, L2, L10, 0
	TTNOP
	TTNOP
	SFPSETCC	L1, 0, 4
	TTNOP
	SFPMUL	L0, L0, L0, L4, 0
	TTNOP
	TTNOP
	li	a5,7
.L63:
	SFPIADD	L1, L1, -1, 9
	TTNOP
	TTNOP
	TTNOP
	SFPMUL	L0, L0, L0, L4, 0
	TTNOP
	TTNOP
	addi	a5,a5,-1
	bne	a5,zero,.L63
	SFPENCC	3, 10
	TTNOP
	SFPLOAD	L1, 0, 1
	SFPSTORE	0, L0, 0
	SFPSETCC	L1, 0, 0
	TTNOP
	SFPSETSGN	L0, L0, 1, 1
	TTNOP
	TTNOP
	SFPSETEXP	L0, L0, 126, 1
	TTNOP
	TTNOP
	SFPLOADI	L2, 16384, 0
	SFPMAD	L2, L0, L7, L2, 0
	TTNOP
	TTNOP
	SFPMUL	L2, L7, L2, L4, 0
	TTNOP
	TTNOP
	SFPLOADI	L1, 16384, 0
	SFPMAD	L1, L0, L2, L1, 0
	TTNOP
	TTNOP
	SFPMUL	L1, L2, L1, L4, 0
	TTNOP
	TTNOP
	SFPLOADI	L2, 16384, 0
	SFPMAD	L0, L0, L1, L2, 0
	TTNOP
	TTNOP
	SFPMUL	L1, L1, L0, L4, 0
	TTNOP
	TTNOP
	SFPEXEXP	L2, L1, 0
	TTNOP
	TTNOP
	SFPLOAD	L0, 0, 1
	SFPEXEXP	L0, L0, 0
	TTNOP
	TTNOP
	SFPIADD	L0, L2, 0, 6
	TTNOP
	TTNOP
	SFPPUSHC
	SFPIADD	L0, L0, 126, 1
	TTNOP
	TTNOP
	TTNOP
	SFPLOADI	L1, 0, 0
	SFPLOADI	L0, 0, 4
	SFPPOPC
	TTNOP
	SFPSETEXP	L0, L1, 0, 0
	TTNOP
	TTNOP
	SFPENCC	3, 10
	TTNOP
	SFPMULI	L0, 16076, 0
	TTNOP
	TTNOP
	SFPMUL	L0, L0, L3, L4, 0
	TTNOP
	TTNOP
	SFPSTORE	0, L0, 0
	SFPLOADI	L0, 6399, 2
	SFPLOADI	L1, 8492, 2
	SFPLOADI	L2, -256, 2
	SFPLUT	L3, 4
	TTNOP
	TTNOP
	SFPLOADI	L0, 16128, 0
	SFPLOAD	L1, 0, 1
	SFPMAD	L3, L3, L0, L1, 1
	TTNOP
	TTNOP
	SFPSTORE	0, L3, 0
	TTINCRWC	0, 4, 0, 0
	addi	a4,a4,-1
	bne	a4,zero,.L64
	ret
	.size	_ZN7ckernel4sfpu25calculate_gelu_derivativeILb0EEEvv, .-_ZN7ckernel4sfpu25calculate_gelu_derivativeILb0EEEvv
	.section	.text._ZN7ckernel4sfpu14calculate_sqrtILb0ELi4ELi2EEEvv,"axG",@progbits,_ZN7ckernel4sfpu14calculate_sqrtILb0ELi4ELi2EEEvv,comdat
	.align	1
	.weak	_ZN7ckernel4sfpu14calculate_sqrtILb0ELi4ELi2EEEvv
	.type	_ZN7ckernel4sfpu14calculate_sqrtILb0ELi4ELi2EEEvv, @function
_ZN7ckernel4sfpu14calculate_sqrtILb0ELi4ELi2EEEvv:
	SFPLOAD	L0, 0, 1
	SFPLOADI	L1, 24375, 0
	SFPSHFT	L0, L0, -1, 1
	TTNOP
	TTNOP
	SFPIADD	L0, L1, 0, 6
	TTNOP
	TTNOP
	SFPLOAD	L1, 0, 1
	SFPMUL	L2, L0, L0, L4, 0
	TTNOP
	TTNOP
	SFPMUL	L2, L2, L1, L4, 0
	TTNOP
	TTNOP
	SFPMAD	L2, L2, L9, L10, 1
	TTNOP
	TTNOP
	SFPMUL	L2, L2, L0, L4, 0
	TTNOP
	TTNOP
	SFPMUL	L0, L2, L2, L4, 0
	TTNOP
	TTNOP
	SFPMUL	L0, L0, L1, L4, 0
	TTNOP
	TTNOP
	SFPMAD	L0, L0, L9, L10, 1
	TTNOP
	TTNOP
	SFPMUL	L0, L0, L2, L4, 0
	TTNOP
	TTNOP
	SFPMUL	L0, L0, L1, L4, 0
	TTNOP
	TTNOP
	SFPSTORE	0, L0, 0
	TTINCRWC	0, 4, 0, 0
	SFPLOAD	L0, 0, 1
	SFPLOADI	L1, 24375, 0
	SFPSHFT	L0, L0, -1, 1
	TTNOP
	TTNOP
	SFPIADD	L0, L1, 0, 6
	TTNOP
	TTNOP
	SFPLOAD	L1, 0, 1
	SFPMUL	L2, L0, L0, L4, 0
	TTNOP
	TTNOP
	SFPMUL	L2, L2, L1, L4, 0
	TTNOP
	TTNOP
	SFPMAD	L2, L2, L9, L10, 1
	TTNOP
	TTNOP
	SFPMUL	L2, L2, L0, L4, 0
	TTNOP
	TTNOP
	SFPMUL	L0, L2, L2, L4, 0
	TTNOP
	TTNOP
	SFPMUL	L0, L0, L1, L4, 0
	TTNOP
	TTNOP
	SFPMAD	L0, L0, L9, L10, 1
	TTNOP
	TTNOP
	SFPMUL	L0, L0, L2, L4, 0
	TTNOP
	TTNOP
	SFPMUL	L0, L0, L1, L4, 0
	TTNOP
	TTNOP
	SFPSTORE	0, L0, 0
	TTINCRWC	0, 4, 0, 0
	SFPLOAD	L0, 0, 1
	SFPLOADI	L1, 24375, 0
	SFPSHFT	L0, L0, -1, 1
	TTNOP
	TTNOP
	SFPIADD	L0, L1, 0, 6
	TTNOP
	TTNOP
	SFPLOAD	L1, 0, 1
	SFPMUL	L2, L0, L0, L4, 0
	TTNOP
	TTNOP
	SFPMUL	L2, L2, L1, L4, 0
	TTNOP
	TTNOP
	SFPMAD	L2, L2, L9, L10, 1
	TTNOP
	TTNOP
	SFPMUL	L2, L2, L0, L4, 0
	TTNOP
	TTNOP
	SFPMUL	L0, L2, L2, L4, 0
	TTNOP
	TTNOP
	SFPMUL	L0, L0, L1, L4, 0
	TTNOP
	TTNOP
	SFPMAD	L0, L0, L9, L10, 1
	TTNOP
	TTNOP
	SFPMUL	L0, L0, L2, L4, 0
	TTNOP
	TTNOP
	SFPMUL	L0, L0, L1, L4, 0
	TTNOP
	TTNOP
	SFPSTORE	0, L0, 0
	TTINCRWC	0, 4, 0, 0
	SFPLOAD	L0, 0, 1
	SFPLOADI	L1, 24375, 0
	SFPSHFT	L0, L0, -1, 1
	TTNOP
	TTNOP
	SFPIADD	L0, L1, 0, 6
	TTNOP
	TTNOP
	SFPLOAD	L1, 0, 1
	SFPMUL	L2, L0, L0, L4, 0
	TTNOP
	TTNOP
	SFPMUL	L2, L2, L1, L4, 0
	TTNOP
	TTNOP
	SFPMAD	L2, L2, L9, L10, 1
	TTNOP
	TTNOP
	SFPMUL	L2, L2, L0, L4, 0
	TTNOP
	TTNOP
	SFPMUL	L0, L2, L2, L4, 0
	TTNOP
	TTNOP
	SFPMUL	L0, L0, L1, L4, 0
	TTNOP
	TTNOP
	SFPMAD	L0, L0, L9, L10, 1
	TTNOP
	TTNOP
	SFPMUL	L0, L0, L2, L4, 0
	TTNOP
	TTNOP
	SFPMUL	L0, L0, L1, L4, 0
	TTNOP
	TTNOP
	SFPSTORE	0, L0, 0
	TTINCRWC	0, 4, 0, 0
	ret
	.size	_ZN7ckernel4sfpu14calculate_sqrtILb0ELi4ELi2EEEvv, .-_ZN7ckernel4sfpu14calculate_sqrtILb0ELi4ELi2EEEvv
	.section	.text._ZN7ckernel4sfpu14calculate_sqrtILb1ELi4ELi2EEEvv,"axG",@progbits,_ZN7ckernel4sfpu14calculate_sqrtILb1ELi4ELi2EEEvv,comdat
	.align	1
	.weak	_ZN7ckernel4sfpu14calculate_sqrtILb1ELi4ELi2EEEvv
	.type	_ZN7ckernel4sfpu14calculate_sqrtILb1ELi4ELi2EEEvv, @function
_ZN7ckernel4sfpu14calculate_sqrtILb1ELi4ELi2EEEvv:
	SFPLOAD	L0, 0, 1
	SFPIADD	L0, L2, 0, 4
	TTNOP
	TTNOP
	SFPSHFT	L0, L0, -1, 1
	TTNOP
	TTNOP
	SFPSTORE	0, L0, 0
	TTINCRWC	0, 4, 0, 0
	SFPLOAD	L0, 0, 1
	SFPIADD	L0, L2, 0, 4
	TTNOP
	TTNOP
	SFPSHFT	L0, L0, -1, 1
	TTNOP
	TTNOP
	SFPSTORE	0, L0, 0
	TTINCRWC	0, 4, 0, 0
	SFPLOAD	L0, 0, 1
	SFPIADD	L0, L2, 0, 4
	TTNOP
	TTNOP
	SFPSHFT	L0, L0, -1, 1
	TTNOP
	TTNOP
	SFPSTORE	0, L0, 0
	TTINCRWC	0, 4, 0, 0
	SFPLOAD	L0, 0, 1
	SFPIADD	L0, L2, 0, 4
	TTNOP
	TTNOP
	SFPSHFT	L0, L0, -1, 1
	TTNOP
	TTNOP
	SFPSTORE	0, L0, 0
	TTINCRWC	0, 4, 0, 0
	ret
	.size	_ZN7ckernel4sfpu14calculate_sqrtILb1ELi4ELi2EEEvv, .-_ZN7ckernel4sfpu14calculate_sqrtILb1ELi4ELi2EEEvv
	.section	.text._ZN7ckernel4sfpu21calculate_exponentialILb0ELb0ELb1EEEvs,"axG",@progbits,_ZN7ckernel4sfpu21calculate_exponentialILb0ELb0ELb1EEEvs,comdat
	.align	1
	.weak	_ZN7ckernel4sfpu21calculate_exponentialILb0ELb0ELb1EEEvs
	.type	_ZN7ckernel4sfpu21calculate_exponentialILb0ELb0ELb1EEEvs, @function
_ZN7ckernel4sfpu21calculate_exponentialILb0ELb0ELb1EEEvs:
	slli	a3,a0,16
	srli	a3,a3,16
	li	a5, 1896939520	# 71110000
	li	a4,4
	lui	a2,%hi(_ZN7ckernel13instrn_bufferE)
	add	a3,a3,a5
.L71:
	SFPLOAD	L0, 0, 1
	lw	a5,%lo(_ZN7ckernel13instrn_bufferE)(a2)
	sw	a3, 0(a5)	# Op(0x71) - d(lr1)
	SFPMUL	L0, L0, L1, L4, 0
	TTNOP
	TTNOP
	SFPSTORE	0, L0, 0
	SFPSETSGN	L0, L0, 0, 1
	TTNOP
	TTNOP
	SFPEXEXP	L1, L0, 10
	TTNOP
	TTNOP
	TTNOP
	SFPSETEXP	L0, L0, 126, 1
	TTNOP
	TTNOP
	SFPENCC	3, 10
	TTNOP
	SFPLOADI	L2, 16221, 0
	SFPMAD	L2, L0, L8, L2, 0
	TTNOP
	TTNOP
	SFPMAD	L0, L0, L2, L10, 0
	TTNOP
	TTNOP
	SFPSETCC	L1, 0, 4
	TTNOP
	SFPMUL	L0, L0, L0, L4, 0
	TTNOP
	TTNOP
	li	a5,7
.L70:
	SFPIADD	L1, L1, -1, 9
	TTNOP
	TTNOP
	TTNOP
	SFPMUL	L0, L0, L0, L4, 0
	TTNOP
	TTNOP
	addi	a5,a5,-1
	bne	a5,zero,.L70
	SFPENCC	3, 10
	TTNOP
	SFPLOAD	L1, 0, 1
	SFPSTORE	0, L0, 0
	SFPSETCC	L1, 0, 0
	TTNOP
	SFPSETSGN	L0, L0, 1, 1
	TTNOP
	TTNOP
	SFPSETEXP	L0, L0, 126, 1
	TTNOP
	TTNOP
	SFPLOADI	L2, 16384, 0
	SFPMAD	L1, L0, L7, L2, 0
	TTNOP
	TTNOP
	SFPMUL	L1, L7, L1, L4, 0
	TTNOP
	TTNOP
	SFPMAD	L3, L0, L1, L2, 0
	TTNOP
	TTNOP
	SFPMUL	L1, L1, L3, L4, 0
	TTNOP
	TTNOP
	SFPMAD	L0, L0, L1, L2, 0
	TTNOP
	TTNOP
	SFPMUL	L0, L1, L0, L4, 0
	TTNOP
	TTNOP
	SFPEXEXP	L2, L0, 0
	TTNOP
	TTNOP
	SFPLOAD	L1, 0, 1
	SFPEXEXP	L1, L1, 0
	TTNOP
	TTNOP
	SFPIADD	L1, L2, 0, 6
	TTNOP
	TTNOP
	SFPPUSHC
	SFPIADD	L1, L1, 126, 1
	TTNOP
	TTNOP
	TTNOP
	SFPLOADI	L0, 0, 0
	SFPLOADI	L1, 0, 4
	SFPPOPC
	TTNOP
	SFPSETEXP	L1, L0, 0, 0
	TTNOP
	TTNOP
	SFPSTORE	0, L1, 0
	SFPENCC	3, 10
	TTNOP
	TTINCRWC	0, 4, 0, 0
	addi	a4,a4,-1
	bne	a4,zero,.L71
	ret
	.size	_ZN7ckernel4sfpu21calculate_exponentialILb0ELb0ELb1EEEvs, .-_ZN7ckernel4sfpu21calculate_exponentialILb0ELb0ELb1EEEvs
	.section	.text._ZN7ckernel4sfpu21calculate_exponentialILb0ELb1ELb1EEEvs,"axG",@progbits,_ZN7ckernel4sfpu21calculate_exponentialILb0ELb1ELb1EEEvs,comdat
	.align	1
	.weak	_ZN7ckernel4sfpu21calculate_exponentialILb0ELb1ELb1EEEvs
	.type	_ZN7ckernel4sfpu21calculate_exponentialILb0ELb1ELb1EEEvs, @function
_ZN7ckernel4sfpu21calculate_exponentialILb0ELb1ELb1EEEvs:
	slli	a3,a0,16
	srli	a3,a3,16
	li	a5, 1896939520	# 71110000
	li	a4,4
	lui	a2,%hi(_ZN7ckernel13instrn_bufferE)
	add	a3,a3,a5
.L76:
	SFPLOAD	L0, 0, 1
	lw	a5,%lo(_ZN7ckernel13instrn_bufferE)(a2)
	sw	a3, 0(a5)	# Op(0x71) - d(lr1)
	SFPMUL	L0, L0, L1, L4, 0
	TTNOP
	TTNOP
	SFPSTORE	0, L0, 0
	SFPSETSGN	L0, L0, 0, 1
	TTNOP
	TTNOP
	SFPEXEXP	L1, L0, 10
	TTNOP
	TTNOP
	TTNOP
	SFPSETEXP	L0, L0, 126, 1
	TTNOP
	TTNOP
	SFPENCC	3, 10
	TTNOP
	SFPLOADI	L2, 16221, 0
	SFPMAD	L2, L0, L8, L2, 0
	TTNOP
	TTNOP
	SFPMAD	L0, L0, L2, L10, 0
	TTNOP
	TTNOP
	SFPSETCC	L1, 0, 4
	TTNOP
	SFPMUL	L0, L0, L0, L4, 0
	TTNOP
	TTNOP
	li	a5,7
.L75:
	SFPIADD	L1, L1, -1, 9
	TTNOP
	TTNOP
	TTNOP
	SFPMUL	L0, L0, L0, L4, 0
	TTNOP
	TTNOP
	addi	a5,a5,-1
	bne	a5,zero,.L75
	SFPENCC	3, 10
	TTNOP
	SFPLOAD	L1, 0, 1
	SFPSTORE	0, L0, 0
	SFPSETCC	L1, 0, 0
	TTNOP
	SFPSETSGN	L0, L0, 1, 1
	TTNOP
	TTNOP
	SFPSETEXP	L0, L0, 126, 1
	TTNOP
	TTNOP
	SFPLOADI	L2, 16384, 0
	SFPMAD	L1, L0, L7, L2, 0
	TTNOP
	TTNOP
	SFPMUL	L1, L7, L1, L4, 0
	TTNOP
	TTNOP
	SFPMAD	L3, L0, L1, L2, 0
	TTNOP
	TTNOP
	SFPMUL	L1, L1, L3, L4, 0
	TTNOP
	TTNOP
	SFPMAD	L0, L0, L1, L2, 0
	TTNOP
	TTNOP
	SFPMUL	L0, L1, L0, L4, 0
	TTNOP
	TTNOP
	SFPEXEXP	L2, L0, 0
	TTNOP
	TTNOP
	SFPLOAD	L1, 0, 1
	SFPEXEXP	L1, L1, 0
	TTNOP
	TTNOP
	SFPIADD	L1, L2, 0, 6
	TTNOP
	TTNOP
	SFPPUSHC
	SFPIADD	L1, L1, 126, 1
	TTNOP
	TTNOP
	TTNOP
	SFPLOADI	L0, 0, 0
	SFPLOADI	L1, 0, 4
	SFPPOPC
	TTNOP
	SFPSETEXP	L1, L0, 0, 0
	TTNOP
	TTNOP
	SFPSTORE	0, L1, 0
	SFPENCC	3, 10
	TTNOP
	TTINCRWC	0, 4, 0, 0
	addi	a4,a4,-1
	bne	a4,zero,.L76
	ret
	.size	_ZN7ckernel4sfpu21calculate_exponentialILb0ELb1ELb1EEEvs, .-_ZN7ckernel4sfpu21calculate_exponentialILb0ELb1ELb1EEEvs
	.section	.text._ZN7ckernel4sfpu21calculate_exponentialILb1ELb0ELb1EEEvs,"axG",@progbits,_ZN7ckernel4sfpu21calculate_exponentialILb1ELb0ELb1EEEvs,comdat
	.align	1
	.weak	_ZN7ckernel4sfpu21calculate_exponentialILb1ELb0ELb1EEEvs
	.type	_ZN7ckernel4sfpu21calculate_exponentialILb1ELb0ELb1EEEvs, @function
_ZN7ckernel4sfpu21calculate_exponentialILb1ELb0ELb1EEEvs:
	slli	a4,a0,16
	srli	a4,a4,16
	li	a3, 1899036672	# 71310000
	li	a5,4
	lui	a2,%hi(_ZN7ckernel13instrn_bufferE)
	add	a4,a4,a3
.L80:
	SFPLOAD	L1, 0, 1
	lw	a3,%lo(_ZN7ckernel13instrn_bufferE)(a2)
	sw	a4, 0(a3)	# Op(0x71) - d(lr3)
	SFPMUL	L1, L1, L3, L4, 0
	TTNOP
	TTNOP
	SFPSTORE	0, L1, 0
	SFPMAD	L1, L1, L7, L0, 0
	TTNOP
	TTNOP
	SFPIADD	L1, L2, 0, 4
	TTNOP
	TTNOP
	SFPSHFT	L1, L0, 8, 1
	TTNOP
	TTNOP
	SFPSTORE	0, L1, 0
	TTINCRWC	0, 4, 0, 0
	addi	a5,a5,-1
	bne	a5,zero,.L80
	ret
	.size	_ZN7ckernel4sfpu21calculate_exponentialILb1ELb0ELb1EEEvs, .-_ZN7ckernel4sfpu21calculate_exponentialILb1ELb0ELb1EEEvs
	.section	.text._ZN7ckernel4sfpu21calculate_exponentialILb1ELb1ELb1EEEvs,"axG",@progbits,_ZN7ckernel4sfpu21calculate_exponentialILb1ELb1ELb1EEEvs,comdat
	.align	1
	.weak	_ZN7ckernel4sfpu21calculate_exponentialILb1ELb1ELb1EEEvs
	.type	_ZN7ckernel4sfpu21calculate_exponentialILb1ELb1ELb1EEEvs, @function
_ZN7ckernel4sfpu21calculate_exponentialILb1ELb1ELb1EEEvs:
	slli	a4,a0,16
	srli	a4,a4,16
	li	a3, 1899036672	# 71310000
	li	a5,4
	lui	a2,%hi(_ZN7ckernel13instrn_bufferE)
	add	a4,a4,a3
.L83:
	SFPLOAD	L1, 0, 1
	lw	a3,%lo(_ZN7ckernel13instrn_bufferE)(a2)
	sw	a4, 0(a3)	# Op(0x71) - d(lr3)
	SFPMUL	L1, L1, L3, L4, 0
	TTNOP
	TTNOP
	SFPSTORE	0, L1, 0
	SFPMAD	L1, L1, L7, L0, 0
	TTNOP
	TTNOP
	SFPIADD	L1, L2, 0, 4
	TTNOP
	TTNOP
	SFPSHFT	L1, L0, 8, 1
	TTNOP
	TTNOP
	SFPSTORE	0, L1, 0
	SFPSETCC	L1, 0, 0
	TTNOP
	SFPSTORE	0, L4, 0
	SFPENCC	3, 10
	TTNOP
	TTINCRWC	0, 4, 0, 0
	addi	a5,a5,-1
	bne	a5,zero,.L83
	ret
	.size	_ZN7ckernel4sfpu21calculate_exponentialILb1ELb1ELb1EEEvs, .-_ZN7ckernel4sfpu21calculate_exponentialILb1ELb1ELb1EEEvs
	.section	.text._ZN7ckernel4sfpu13calculate_logILb1ELb1EEEvm,"axG",@progbits,_ZN7ckernel4sfpu13calculate_logILb1ELb1EEEvm,comdat
	.align	1
	.weak	_ZN7ckernel4sfpu13calculate_logILb1ELb1EEEvm
	.type	_ZN7ckernel4sfpu13calculate_logILb1ELb1EEEvm, @function
_ZN7ckernel4sfpu13calculate_logILb1ELb1EEEvm:
	slli	a4,a0,16
	srli	a4,a4,16
	li	a3, 1896939520	# 71110000
	li	a5,4
	lui	a2,%hi(_ZN7ckernel13instrn_bufferE)
	add	a4,a4,a3
.L86:
	SFPLOAD	L3, 0, 1
	SFPSETEXP	L2, L3, 127, 1
	TTNOP
	TTNOP
	SFPLOADI	L0, 11973, 1
	SFPLOADI	L1, -17997, 1
	SFPMAD	L0, L2, L0, L1, 0
	TTNOP
	TTNOP
	SFPLOADI	L1, 16428, 1
	SFPMAD	L0, L2, L0, L1, 0
	TTNOP
	TTNOP
	SFPLOADI	L1, -16921, 1
	SFPMAD	L2, L2, L0, L1, 0
	TTNOP
	TTNOP
	SFPLOADI	L0, 0, 4
	SFPSETCC	L3, 0, 2
	TTNOP
	SFPEXEXP	L0, L3, 2
	TTNOP
	TTNOP
	TTNOP
	SFPABS	L0, L0, 0
	TTNOP
	TTNOP
	SFPSETSGN	L3, L3, 1, 1
	TTNOP
	TTNOP
	SFPENCC	3, 10
	TTNOP
	SFPLOADI	L1, 0, 4
	SFPSETCC	L0, 0, 2
	TTNOP
	SFPLZ	L1, L0, 0
	TTNOP
	TTNOP
	SFPNOT	L1, L1
	TTNOP
	TTNOP
	SFPIADD	L1, L1, 19, 9
	TTNOP
	TTNOP
	TTNOP
	SFPIADD	L1, L1, 127, 5
	TTNOP
	TTNOP
	SFPENCC	3, 10
	TTNOP
	SFPSETEXP	L1, L3, 0, 0
	TTNOP
	TTNOP
	SFPLZ	L3, L0, 0
	TTNOP
	TTNOP
	SFPIADD	L3, L3, 1, 5
	TTNOP
	TTNOP
	SFPSHFT	L0, L3, 0, 0
	TTNOP
	TTNOP
	SFPSETMAN	L0, L1, 0, 0
	TTNOP
	TTNOP
	SFPMAD	L0, L0, L5, L2, 0
	TTNOP
	TTNOP
	lw	a3,%lo(_ZN7ckernel13instrn_bufferE)(a2)
	sw	a4, 0(a3)	# Op(0x71) - d(lr1)
	SFPMUL	L0, L0, L1, L4, 0
	TTNOP
	TTNOP
	SFPLOAD	L1, 0, 1
	SFPSETCC	L1, 0, 6
	TTNOP
	SFPLOADI	L0, -128, 0
	SFPENCC	3, 10
	TTNOP
	SFPSTORE	0, L0, 0
	TTINCRWC	0, 4, 0, 0
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
	SFPLOAD	L0, 0, 1
	SFPSETSGN	L0, L0, 1, 1
	TTNOP
	TTNOP
	SFPSETEXP	L0, L0, 126, 1
	TTNOP
	TTNOP
	SFPLOADI	L2, 16384, 0
	SFPMAD	L1, L0, L7, L2, 0
	TTNOP
	TTNOP
	SFPMUL	L1, L7, L1, L4, 0
	TTNOP
	TTNOP
	SFPMAD	L3, L0, L1, L2, 0
	TTNOP
	TTNOP
	SFPMUL	L1, L1, L3, L4, 0
	TTNOP
	TTNOP
	SFPMAD	L0, L0, L1, L2, 0
	TTNOP
	TTNOP
	SFPMUL	L1, L1, L0, L4, 0
	TTNOP
	TTNOP
	SFPEXEXP	L2, L1, 0
	TTNOP
	TTNOP
	SFPLOAD	L0, 0, 1
	SFPEXEXP	L0, L0, 0
	TTNOP
	TTNOP
	SFPIADD	L0, L2, 0, 6
	TTNOP
	TTNOP
	SFPIADD	L0, L0, 126, 1
	TTNOP
	TTNOP
	TTNOP
	SFPLOADI	L1, 0, 0
	SFPLOADI	L0, 0, 4
	SFPENCC	3, 10
	TTNOP
	SFPSETEXP	L0, L1, 0, 0
	TTNOP
	TTNOP
	SFPLOAD	L1, 0, 1
	SFPSETCC	L1, 0, 0
	TTNOP
	SFPMOV	L0, L0, 1
	TTNOP
	TTNOP
	SFPENCC	3, 10
	TTNOP
	SFPSTORE	0, L0, 0
	TTINCRWC	0, 4, 0, 0
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
	SFPLOAD	L0, 0, 1
	SFPSETSGN	L0, L0, 1, 1
	TTNOP
	TTNOP
	SFPSETEXP	L0, L0, 126, 1
	TTNOP
	TTNOP
	SFPLOADI	L2, 16384, 0
	SFPMAD	L1, L0, L7, L2, 0
	TTNOP
	TTNOP
	SFPMUL	L1, L7, L1, L4, 0
	TTNOP
	TTNOP
	SFPMAD	L0, L0, L1, L2, 0
	TTNOP
	TTNOP
	SFPMUL	L1, L1, L0, L4, 0
	TTNOP
	TTNOP
	SFPEXEXP	L2, L1, 0
	TTNOP
	TTNOP
	SFPLOAD	L0, 0, 1
	SFPEXEXP	L0, L0, 0
	TTNOP
	TTNOP
	SFPIADD	L0, L2, 0, 6
	TTNOP
	TTNOP
	SFPIADD	L0, L0, 126, 1
	TTNOP
	TTNOP
	TTNOP
	SFPLOADI	L1, 0, 0
	SFPLOADI	L0, 0, 4
	SFPENCC	3, 10
	TTNOP
	SFPSETEXP	L0, L1, 0, 0
	TTNOP
	TTNOP
	SFPLOAD	L1, 0, 1
	SFPSETCC	L1, 0, 0
	TTNOP
	SFPMOV	L0, L0, 1
	TTNOP
	TTNOP
	SFPENCC	3, 10
	TTNOP
	SFPSTORE	0, L0, 0
	TTINCRWC	0, 4, 0, 0
	addi	a5,a5,-1
	bne	a5,zero,.L92
	ret
	.size	_ZN7ckernel4sfpu20calculate_reciprocalILb1ELi4EEEvv, .-_ZN7ckernel4sfpu20calculate_reciprocalILb1ELi4EEEvv
	.section	.text._ZN7ckernel4sfpu17calculate_dropoutILb0EEEvmm,"axG",@progbits,_ZN7ckernel4sfpu17calculate_dropoutILb0EEEvmm,comdat
	.align	1
	.weak	_ZN7ckernel4sfpu17calculate_dropoutILb0EEEvmm
	.type	_ZN7ckernel4sfpu17calculate_dropoutILb0EEEvmm, @function
_ZN7ckernel4sfpu17calculate_dropoutILb0EEEvmm:
	SFPLOADI	L1, -22197, 0
	li	a5,65536
	addi	a5,a5,-1
	and	a4,a1,a5
	li	a3, 1946157056	# 74000000
	slli	a4,a4,8
	and	a0,a0,a5
	add	a4,a4,a3
	li	a3, 1898053632	# 71220000
	li	a5,4
	lui	a1,%hi(_ZN7ckernel13instrn_bufferE)
	add	a2,a0,a3
.L95:
	lw	a3,%lo(_ZN7ckernel13instrn_bufferE)(a1)
	SFPLOAD	L0, 0, 1
	sw	a4, 0(a3)	# Op(0x74) lv(lr0)  d(lr0)
	TTNOP
	TTNOP
	SFPSTORE	0, L0, 0
	SFPMOV	L0, L3, 0
	TTNOP
	TTNOP
	SFPSHFT	L0, L0, -3, 1
	TTNOP
	TTNOP
	sw	a2, 0(a3)	# Op(0x71) - d(lr2)
	SFPIADD	L2, L0, 0, 2
	TTNOP
	TTNOP
	TTNOP
	SFPSTORE	0, L4, 0
	SFPENCC	3, 10
	TTNOP
	SFPMOV	L0, L3, 0
	TTNOP
	TTNOP
	SFPSHFT	L0, L0, 1, 1
	TTNOP
	TTNOP
	SFPSETCC	L0, 0, 0
	TTNOP
	SFPMOV	L0, L3, 0
	TTNOP
	TTNOP
	SFPOR	L0, L1
	TTNOP
	TTNOP
	SFPAND	L3, L1
	TTNOP
	TTNOP
	SFPNOT	L3, L3
	TTNOP
	TTNOP
	SFPAND	L3, L0
	TTNOP
	TTNOP
	SFPENCC	3, 10
	TTNOP
	TTINCRWC	0, 4, 0, 0
	addi	a5,a5,-1
	bne	a5,zero,.L95
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
	SFPMOV	L0, L15, 0
	TTNOP
	TTNOP
	SFPSHFT	L0, L0, 13, 1
	TTNOP
	TTNOP
	slli	a0,a0,16
	srli	a0,a0,16
	call	__floatunsisf
	lui	a5,%hi(_ZN7ckernel13instrn_bufferE)
	lw	a5,%lo(_ZN7ckernel13instrn_bufferE)(a5)
	li	a4, 1896873984	# 71100000
	srli	a0,a0,16
	add	a0,a0,a4
	sw	a0, 0(a5)	# Op(0x71) - d(lr1)
	SFPMOV	L3, L0, 0
	TTNOP
	TTNOP
	SFPAND	L3, L1
	TTNOP
	TTNOP
	SFPNOT	L3, L3
	TTNOP
	TTNOP
	SFPOR	L0, L1
	TTNOP
	TTNOP
	SFPAND	L3, L0
	TTNOP
	TTNOP
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
	.ident	"GCC: (GNU) 12.2.0"
