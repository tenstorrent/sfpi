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
 #APP
# 351 "ckernel.cc" 1
	.word (-536608768)
# 0 "" 2
 #NO_APP
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
 #APP
# 351 "ckernel.cc" 1
	.word (-536608768)
# 0 "" 2
 #NO_APP
	addi	a5,a5,-1
	bne	a5,zero,.L6
	ret
	.size	_ZN7ckernel4sfpu21calculate_exponentialILb1ELb0ELb0EEEvs.isra.0, .-_ZN7ckernel4sfpu21calculate_exponentialILb1ELb0ELb0EEEvs.isra.0
	.align	1
	.type	_ZN7ckernel4sfpu21calculate_exponentialILb0ELb1ELb0EEEvs.isra.0, @function
_ZN7ckernel4sfpu21calculate_exponentialILb0ELb1ELb0EEEvs.isra.0:
	li	a4,4
.L10:
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
.L9:
	SFPIADD	-1, L1, L1, 9
	SFPNOP
	SFPNOP
	SFPNOP
	SFPMUL	L0, L0, L4, L0, 0
	SFPNOP
	SFPNOP
	addi	a5,a5,-1
	bne	a5,zero,.L9
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
 #APP
# 351 "ckernel.cc" 1
	.word (-536608768)
# 0 "" 2
 #NO_APP
	addi	a4,a4,-1
	bne	a4,zero,.L10
	ret
	.size	_ZN7ckernel4sfpu21calculate_exponentialILb0ELb1ELb0EEEvs.isra.0, .-_ZN7ckernel4sfpu21calculate_exponentialILb0ELb1ELb0EEEvs.isra.0
	.align	1
	.type	_ZN7ckernel4sfpu21calculate_exponentialILb0ELb0ELb0EEEvs.isra.0, @function
_ZN7ckernel4sfpu21calculate_exponentialILb0ELb0ELb0EEEvs.isra.0:
	li	a4,4
.L15:
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
.L14:
	SFPIADD	-1, L1, L1, 9
	SFPNOP
	SFPNOP
	SFPNOP
	SFPMUL	L0, L0, L4, L0, 0
	SFPNOP
	SFPNOP
	addi	a5,a5,-1
	bne	a5,zero,.L14
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
 #APP
# 351 "ckernel.cc" 1
	.word (-536608768)
# 0 "" 2
 #NO_APP
	addi	a4,a4,-1
	bne	a4,zero,.L15
	ret
	.size	_ZN7ckernel4sfpu21calculate_exponentialILb0ELb0ELb0EEEvs.isra.0, .-_ZN7ckernel4sfpu21calculate_exponentialILb0ELb0ELb0EEEvs.isra.0
	.section	.text._ZN7ckernel4sfpu14calculate_compILb0EL8SfpuType8EEEvm,"axG",@progbits,_ZN7ckernel4sfpu14calculate_compILb0EL8SfpuType8EEEvm,comdat
	.align	1
	.weak	_ZN7ckernel4sfpu14calculate_compILb0EL8SfpuType8EEEvm
	.type	_ZN7ckernel4sfpu14calculate_compILb0EL8SfpuType8EEEvm, @function
_ZN7ckernel4sfpu14calculate_compILb0EL8SfpuType8EEEvm:
	beq	a0,zero,.L22
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
 #APP
# 904 "ckernel.cc" 1
	.word (-536608768)
# 0 "" 2
 #NO_APP
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
 #APP
# 904 "ckernel.cc" 1
	.word (-536608768)
# 0 "" 2
 #NO_APP
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
 #APP
# 904 "ckernel.cc" 1
	.word (-536608768)
# 0 "" 2
 #NO_APP
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
 #APP
# 904 "ckernel.cc" 1
	.word (-536608768)
# 0 "" 2
 #NO_APP
	ret
.L22:
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
 #APP
# 904 "ckernel.cc" 1
	.word (-536608768)
# 0 "" 2
 #NO_APP
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
 #APP
# 904 "ckernel.cc" 1
	.word (-536608768)
# 0 "" 2
 #NO_APP
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
 #APP
# 904 "ckernel.cc" 1
	.word (-536608768)
# 0 "" 2
 #NO_APP
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
 #APP
# 904 "ckernel.cc" 1
	.word (-536608768)
# 0 "" 2
 #NO_APP
	ret
	.size	_ZN7ckernel4sfpu14calculate_compILb0EL8SfpuType8EEEvm, .-_ZN7ckernel4sfpu14calculate_compILb0EL8SfpuType8EEEvm
	.section	.text._ZN7ckernel4sfpu14calculate_compILb0EL8SfpuType10EEEvm,"axG",@progbits,_ZN7ckernel4sfpu14calculate_compILb0EL8SfpuType10EEEvm,comdat
	.align	1
	.weak	_ZN7ckernel4sfpu14calculate_compILb0EL8SfpuType10EEEvm
	.type	_ZN7ckernel4sfpu14calculate_compILb0EL8SfpuType10EEEvm, @function
_ZN7ckernel4sfpu14calculate_compILb0EL8SfpuType10EEEvm:
	beq	a0,zero,.L27
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
 #APP
# 904 "ckernel.cc" 1
	.word (-536608768)
# 0 "" 2
 #NO_APP
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
 #APP
# 904 "ckernel.cc" 1
	.word (-536608768)
# 0 "" 2
 #NO_APP
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
 #APP
# 904 "ckernel.cc" 1
	.word (-536608768)
# 0 "" 2
 #NO_APP
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
 #APP
# 904 "ckernel.cc" 1
	.word (-536608768)
# 0 "" 2
 #NO_APP
	ret
.L27:
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
 #APP
# 904 "ckernel.cc" 1
	.word (-536608768)
# 0 "" 2
 #NO_APP
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
 #APP
# 904 "ckernel.cc" 1
	.word (-536608768)
# 0 "" 2
 #NO_APP
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
 #APP
# 904 "ckernel.cc" 1
	.word (-536608768)
# 0 "" 2
 #NO_APP
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
 #APP
# 904 "ckernel.cc" 1
	.word (-536608768)
# 0 "" 2
 #NO_APP
	ret
	.size	_ZN7ckernel4sfpu14calculate_compILb0EL8SfpuType10EEEvm, .-_ZN7ckernel4sfpu14calculate_compILb0EL8SfpuType10EEEvm
	.section	.text._ZN7ckernel4sfpu14calculate_signILb0EEEvm,"axG",@progbits,_ZN7ckernel4sfpu14calculate_signILb0EEEvm,comdat
	.align	1
	.weak	_ZN7ckernel4sfpu14calculate_signILb0EEEvm
	.type	_ZN7ckernel4sfpu14calculate_signILb0EEEvm, @function
_ZN7ckernel4sfpu14calculate_signILb0EEEvm:
	beq	a0,zero,.L32
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
 #APP
# 972 "ckernel.cc" 1
	.word (-536608768)
# 0 "" 2
 #NO_APP
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
 #APP
# 972 "ckernel.cc" 1
	.word (-536608768)
# 0 "" 2
 #NO_APP
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
 #APP
# 972 "ckernel.cc" 1
	.word (-536608768)
# 0 "" 2
 #NO_APP
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
 #APP
# 972 "ckernel.cc" 1
	.word (-536608768)
# 0 "" 2
 #NO_APP
	ret
.L32:
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
 #APP
# 972 "ckernel.cc" 1
	.word (-536608768)
# 0 "" 2
 #NO_APP
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
 #APP
# 972 "ckernel.cc" 1
	.word (-536608768)
# 0 "" 2
 #NO_APP
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
 #APP
# 972 "ckernel.cc" 1
	.word (-536608768)
# 0 "" 2
 #NO_APP
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
 #APP
# 972 "ckernel.cc" 1
	.word (-536608768)
# 0 "" 2
 #NO_APP
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
 #APP
# 451 "ckernel.cc" 1
	.word (-536608768)
# 0 "" 2
 #NO_APP
	SFPLOAD	L3, 1, 0
	SFPLUT	L3, 4
	SFPNOP
	SFPNOP
	SFPADDI	16128, L3, 0
	SFPNOP
	SFPNOP
	SFPSTORE	L3, 0, 0
 #APP
# 451 "ckernel.cc" 1
	.word (-536608768)
# 0 "" 2
 #NO_APP
	SFPLOAD	L3, 1, 0
	SFPLUT	L3, 4
	SFPNOP
	SFPNOP
	SFPADDI	16128, L3, 0
	SFPNOP
	SFPNOP
	SFPSTORE	L3, 0, 0
 #APP
# 451 "ckernel.cc" 1
	.word (-536608768)
# 0 "" 2
 #NO_APP
	SFPLOAD	L3, 1, 0
	SFPLUT	L3, 4
	SFPNOP
	SFPNOP
	SFPADDI	16128, L3, 0
	SFPNOP
	SFPNOP
	SFPSTORE	L3, 0, 0
 #APP
# 451 "ckernel.cc" 1
	.word (-536608768)
# 0 "" 2
 #NO_APP
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
 #APP
# 471 "ckernel.cc" 1
	.word (-536608768)
# 0 "" 2
 #NO_APP
	SFPLOAD	L3, 1, 0
	SFPLUT	L3, 4
	SFPNOP
	SFPNOP
	SFPSTORE	L3, 0, 0
 #APP
# 471 "ckernel.cc" 1
	.word (-536608768)
# 0 "" 2
 #NO_APP
	SFPLOAD	L3, 1, 0
	SFPLUT	L3, 4
	SFPNOP
	SFPNOP
	SFPSTORE	L3, 0, 0
 #APP
# 471 "ckernel.cc" 1
	.word (-536608768)
# 0 "" 2
 #NO_APP
	SFPLOAD	L3, 1, 0
	SFPLUT	L3, 4
	SFPNOP
	SFPNOP
	SFPSTORE	L3, 0, 0
 #APP
# 471 "ckernel.cc" 1
	.word (-536608768)
# 0 "" 2
 #NO_APP
	ret
	.size	_ZN7ckernel4sfpu14calculate_tanhILb1EEEvv, .-_ZN7ckernel4sfpu14calculate_tanhILb1EEEvv
	.section	.text._ZN7ckernel4sfpu18calculate_hardtanhILb0EEEvmmm,"axG",@progbits,_ZN7ckernel4sfpu18calculate_hardtanhILb0EEEvmmm,comdat
	.align	1
	.weak	_ZN7ckernel4sfpu18calculate_hardtanhILb0EEEvmmm
	.type	_ZN7ckernel4sfpu18calculate_hardtanhILb0EEEvmmm, @function
_ZN7ckernel4sfpu18calculate_hardtanhILb0EEEvmmm:
	lui	a5,%hi(_ZN7ckernel13instrn_bufferE)
	lw	a5,%lo(_ZN7ckernel13instrn_bufferE)(a5)
	slli	a0,a0,16
	srli	a0,a0,16
	li	a4,1898971136
	add	a4, a4, a0
# Op(71) - d(3)
	sw	a4,0(a5)
	slli	a1,a1,16
	srli	a1,a1,16
	li	a4,1897922560
	add	a4, a4, a1
# Op(71) - d(2)
	sw	a4,0(a5)
	slli	a2,a2,16
	srli	a2,a2,16
	li	a4,1896873984
	add	a4, a4, a2
# Op(71) - d(1)
	sw	a4,0(a5)
	li	a5,4
.L36:
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
 #APP
# 509 "ckernel.cc" 1
	.word (-536608768)
# 0 "" 2
 #NO_APP
	addi	a5,a5,-1
	bne	a5,zero,.L36
	ret
	.size	_ZN7ckernel4sfpu18calculate_hardtanhILb0EEEvmmm, .-_ZN7ckernel4sfpu18calculate_hardtanhILb0EEEvmmm
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
	SFPMOV	L3, L3, 1
	SFPNOP
	SFPNOP
	SFPADD	L10, L10, L3, L3, 0
	SFPNOP
	SFPNOP
	SFPSTORE	L3, 0, 0
 #APP
# 535 "ckernel.cc" 1
	.word (-536608768)
# 0 "" 2
 #NO_APP
	SFPLOAD	L3, 1, 0
	SFPLUT	L3, 4
	SFPNOP
	SFPNOP
	SFPMUL	L3, L3, L4, L3, 0
	SFPNOP
	SFPNOP
	SFPMOV	L3, L3, 1
	SFPNOP
	SFPNOP
	SFPADD	L10, L10, L3, L3, 0
	SFPNOP
	SFPNOP
	SFPSTORE	L3, 0, 0
 #APP
# 535 "ckernel.cc" 1
	.word (-536608768)
# 0 "" 2
 #NO_APP
	SFPLOAD	L3, 1, 0
	SFPLUT	L3, 4
	SFPNOP
	SFPNOP
	SFPMUL	L3, L3, L4, L3, 0
	SFPNOP
	SFPNOP
	SFPMOV	L3, L3, 1
	SFPNOP
	SFPNOP
	SFPADD	L10, L10, L3, L3, 0
	SFPNOP
	SFPNOP
	SFPSTORE	L3, 0, 0
 #APP
# 535 "ckernel.cc" 1
	.word (-536608768)
# 0 "" 2
 #NO_APP
	SFPLOAD	L3, 1, 0
	SFPLUT	L3, 4
	SFPNOP
	SFPNOP
	SFPMUL	L3, L3, L4, L3, 0
	SFPNOP
	SFPNOP
	SFPMOV	L3, L3, 1
	SFPNOP
	SFPNOP
	SFPADD	L10, L10, L3, L3, 0
	SFPNOP
	SFPNOP
	SFPSTORE	L3, 0, 0
 #APP
# 535 "ckernel.cc" 1
	.word (-536608768)
# 0 "" 2
 #NO_APP
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
	SFPMOV	L3, L3, 1
	SFPNOP
	SFPNOP
	SFPADD	L10, L10, L3, L3, 0
	SFPNOP
	SFPNOP
	SFPSTORE	L3, 0, 0
 #APP
# 535 "ckernel.cc" 1
	.word (-536608768)
# 0 "" 2
 #NO_APP
	SFPLOAD	L3, 1, 0
	SFPMUL	L3, L3, L4, L3, 0
	SFPNOP
	SFPNOP
	SFPMOV	L3, L3, 1
	SFPNOP
	SFPNOP
	SFPADD	L10, L10, L3, L3, 0
	SFPNOP
	SFPNOP
	SFPSTORE	L3, 0, 0
 #APP
# 535 "ckernel.cc" 1
	.word (-536608768)
# 0 "" 2
 #NO_APP
	SFPLOAD	L3, 1, 0
	SFPMUL	L3, L3, L4, L3, 0
	SFPNOP
	SFPNOP
	SFPMOV	L3, L3, 1
	SFPNOP
	SFPNOP
	SFPADD	L10, L10, L3, L3, 0
	SFPNOP
	SFPNOP
	SFPSTORE	L3, 0, 0
 #APP
# 535 "ckernel.cc" 1
	.word (-536608768)
# 0 "" 2
 #NO_APP
	SFPLOAD	L3, 1, 0
	SFPMUL	L3, L3, L4, L3, 0
	SFPNOP
	SFPNOP
	SFPMOV	L3, L3, 1
	SFPNOP
	SFPNOP
	SFPADD	L10, L10, L3, L3, 0
	SFPNOP
	SFPNOP
	SFPSTORE	L3, 0, 0
 #APP
# 535 "ckernel.cc" 1
	.word (-536608768)
# 0 "" 2
 #NO_APP
	ret
	.size	_ZN7ckernel4sfpu25calculate_tanh_derivativeILb0ELi1EEEvv, .-_ZN7ckernel4sfpu25calculate_tanh_derivativeILb0ELi1EEEvv
	.section	.text._ZN7ckernel4sfpu14calculate_geluILb0EEEvv,"axG",@progbits,_ZN7ckernel4sfpu14calculate_geluILb0EEEvv,comdat
	.align	1
	.weak	_ZN7ckernel4sfpu14calculate_geluILb0EEEvv
	.type	_ZN7ckernel4sfpu14calculate_geluILb0EEEvv, @function
_ZN7ckernel4sfpu14calculate_geluILb0EEEvv:
	li	a5,4
.L41:
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
	SFPLOADI	L2, 2, 65280
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
 #APP
# 429 "ckernel.cc" 1
	.word (-536608768)
# 0 "" 2
 #NO_APP
	addi	a5,a5,-1
	bne	a5,zero,.L41
	ret
	.size	_ZN7ckernel4sfpu14calculate_geluILb0EEEvv, .-_ZN7ckernel4sfpu14calculate_geluILb0EEEvv
	.section	.text._ZN7ckernel4sfpu14calculate_geluILb1EEEvv,"axG",@progbits,_ZN7ckernel4sfpu14calculate_geluILb1EEEvv,comdat
	.align	1
	.weak	_ZN7ckernel4sfpu14calculate_geluILb1EEEvv
	.type	_ZN7ckernel4sfpu14calculate_geluILb1EEEvv, @function
_ZN7ckernel4sfpu14calculate_geluILb1EEEvv:
	li	a5,4
.L44:
	SFPLOAD	L3, 1, 0
	SFPLOADI	L1, 2, 8492
	SFPLOADI	L2, 2, 65280
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
 #APP
# 429 "ckernel.cc" 1
	.word (-536608768)
# 0 "" 2
 #NO_APP
	addi	a5,a5,-1
	bne	a5,zero,.L44
	ret
	.size	_ZN7ckernel4sfpu14calculate_geluILb1EEEvv, .-_ZN7ckernel4sfpu14calculate_geluILb1EEEvv
	.section	.text._ZN7ckernel4sfpu25calculate_gelu_derivativeILb0EEEvv,"axG",@progbits,_ZN7ckernel4sfpu25calculate_gelu_derivativeILb0EEEvv,comdat
	.align	1
	.weak	_ZN7ckernel4sfpu25calculate_gelu_derivativeILb0EEEvv
	.type	_ZN7ckernel4sfpu25calculate_gelu_derivativeILb0EEEvv, @function
_ZN7ckernel4sfpu25calculate_gelu_derivativeILb0EEEvv:
	li	a4,4
.L48:
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
.L47:
	SFPIADD	-1, L1, L1, 9
	SFPNOP
	SFPNOP
	SFPNOP
	SFPMUL	L0, L0, L4, L0, 0
	SFPNOP
	SFPNOP
	addi	a5,a5,-1
	bne	a5,zero,.L47
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
	SFPLOADI	L2, 2, 65280
	SFPLUT	L3, 4
	SFPNOP
	SFPNOP
	SFPLOADI	L0, 0, 16128
	SFPLOAD	L1, 1, 0
	SFPMAD	L3, L0, L1, L3, 1
	SFPNOP
	SFPNOP
	SFPSTORE	L3, 0, 0
 #APP
# 563 "ckernel.cc" 1
	.word (-536608768)
# 0 "" 2
 #NO_APP
	addi	a4,a4,-1
	bne	a4,zero,.L48
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
 #APP
# 631 "ckernel.cc" 1
	.word (-536608768)
# 0 "" 2
 #NO_APP
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
 #APP
# 631 "ckernel.cc" 1
	.word (-536608768)
# 0 "" 2
 #NO_APP
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
 #APP
# 631 "ckernel.cc" 1
	.word (-536608768)
# 0 "" 2
 #NO_APP
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
 #APP
# 631 "ckernel.cc" 1
	.word (-536608768)
# 0 "" 2
 #NO_APP
	ret
	.size	_ZN7ckernel4sfpu14calculate_sqrtILb0ELi4ELi2EEEvv, .-_ZN7ckernel4sfpu14calculate_sqrtILb0ELi4ELi2EEEvv
	.section	.text._ZN7ckernel4sfpu21calculate_exponentialILb0ELb0ELb1EEEvs,"axG",@progbits,_ZN7ckernel4sfpu21calculate_exponentialILb0ELb0ELb1EEEvs,comdat
	.align	1
	.weak	_ZN7ckernel4sfpu21calculate_exponentialILb0ELb0ELb1EEEvs
	.type	_ZN7ckernel4sfpu21calculate_exponentialILb0ELb0ELb1EEEvs, @function
_ZN7ckernel4sfpu21calculate_exponentialILb0ELb0ELb1EEEvs:
	slli	a0,a0,16
	li	a4,4
	lui	a3,%hi(_ZN7ckernel13instrn_bufferE)
	srli	a0,a0,16
.L54:
	SFPLOAD	L0, 1, 0
	lw	a5,%lo(_ZN7ckernel13instrn_bufferE)(a3)
	li	a2,1896939520
	add	a2, a2, a0
# Op(71) - d(1)
	sw	a2,0(a5)
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
.L53:
	SFPIADD	-1, L1, L1, 9
	SFPNOP
	SFPNOP
	SFPNOP
	SFPMUL	L0, L0, L4, L0, 0
	SFPNOP
	SFPNOP
	addi	a5,a5,-1
	bne	a5,zero,.L53
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
 #APP
# 351 "ckernel.cc" 1
	.word (-536608768)
# 0 "" 2
 #NO_APP
	addi	a4,a4,-1
	bne	a4,zero,.L54
	ret
	.size	_ZN7ckernel4sfpu21calculate_exponentialILb0ELb0ELb1EEEvs, .-_ZN7ckernel4sfpu21calculate_exponentialILb0ELb0ELb1EEEvs
	.section	.text._ZN7ckernel4sfpu21calculate_exponentialILb0ELb1ELb1EEEvs,"axG",@progbits,_ZN7ckernel4sfpu21calculate_exponentialILb0ELb1ELb1EEEvs,comdat
	.align	1
	.weak	_ZN7ckernel4sfpu21calculate_exponentialILb0ELb1ELb1EEEvs
	.type	_ZN7ckernel4sfpu21calculate_exponentialILb0ELb1ELb1EEEvs, @function
_ZN7ckernel4sfpu21calculate_exponentialILb0ELb1ELb1EEEvs:
	slli	a0,a0,16
	li	a4,4
	lui	a3,%hi(_ZN7ckernel13instrn_bufferE)
	srli	a0,a0,16
.L59:
	SFPLOAD	L0, 1, 0
	lw	a5,%lo(_ZN7ckernel13instrn_bufferE)(a3)
	li	a2,1896939520
	add	a2, a2, a0
# Op(71) - d(1)
	sw	a2,0(a5)
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
.L58:
	SFPIADD	-1, L1, L1, 9
	SFPNOP
	SFPNOP
	SFPNOP
	SFPMUL	L0, L0, L4, L0, 0
	SFPNOP
	SFPNOP
	addi	a5,a5,-1
	bne	a5,zero,.L58
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
 #APP
# 351 "ckernel.cc" 1
	.word (-536608768)
# 0 "" 2
 #NO_APP
	addi	a4,a4,-1
	bne	a4,zero,.L59
	ret
	.size	_ZN7ckernel4sfpu21calculate_exponentialILb0ELb1ELb1EEEvs, .-_ZN7ckernel4sfpu21calculate_exponentialILb0ELb1ELb1EEEvs
	.section	.text._ZN7ckernel4sfpu21calculate_exponentialILb1ELb0ELb1EEEvs,"axG",@progbits,_ZN7ckernel4sfpu21calculate_exponentialILb1ELb0ELb1EEEvs,comdat
	.align	1
	.weak	_ZN7ckernel4sfpu21calculate_exponentialILb1ELb0ELb1EEEvs
	.type	_ZN7ckernel4sfpu21calculate_exponentialILb1ELb0ELb1EEEvs, @function
_ZN7ckernel4sfpu21calculate_exponentialILb1ELb0ELb1EEEvs:
	slli	a0,a0,16
	li	a5,4
	lui	a3,%hi(_ZN7ckernel13instrn_bufferE)
	srli	a0,a0,16
.L63:
	SFPLOAD	L1, 1, 0
	lw	a4,%lo(_ZN7ckernel13instrn_bufferE)(a3)
	li	a2,1899036672
	add	a2, a2, a0
# Op(71) - d(3)
	sw	a2,0(a4)
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
 #APP
# 351 "ckernel.cc" 1
	.word (-536608768)
# 0 "" 2
 #NO_APP
	addi	a5,a5,-1
	bne	a5,zero,.L63
	ret
	.size	_ZN7ckernel4sfpu21calculate_exponentialILb1ELb0ELb1EEEvs, .-_ZN7ckernel4sfpu21calculate_exponentialILb1ELb0ELb1EEEvs
	.section	.text._ZN7ckernel4sfpu21calculate_exponentialILb1ELb1ELb1EEEvs,"axG",@progbits,_ZN7ckernel4sfpu21calculate_exponentialILb1ELb1ELb1EEEvs,comdat
	.align	1
	.weak	_ZN7ckernel4sfpu21calculate_exponentialILb1ELb1ELb1EEEvs
	.type	_ZN7ckernel4sfpu21calculate_exponentialILb1ELb1ELb1EEEvs, @function
_ZN7ckernel4sfpu21calculate_exponentialILb1ELb1ELb1EEEvs:
	slli	a0,a0,16
	li	a5,4
	lui	a4,%hi(_ZN7ckernel13instrn_bufferE)
	srli	a0,a0,16
.L66:
	SFPLOAD	L1, 1, 0
	lw	a3,%lo(_ZN7ckernel13instrn_bufferE)(a4)
	li	a2,1899036672
	add	a2, a2, a0
# Op(71) - d(3)
	sw	a2,0(a3)
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
 #APP
# 351 "ckernel.cc" 1
	.word (-536608768)
# 0 "" 2
 #NO_APP
	addi	a5,a5,-1
	bne	a5,zero,.L66
	ret
	.size	_ZN7ckernel4sfpu21calculate_exponentialILb1ELb1ELb1EEEvs, .-_ZN7ckernel4sfpu21calculate_exponentialILb1ELb1ELb1EEEvs
	.section	.text._ZN7ckernel4sfpu13calculate_logILb1ELb1EEEvm,"axG",@progbits,_ZN7ckernel4sfpu13calculate_logILb1ELb1EEEvm,comdat
	.align	1
	.weak	_ZN7ckernel4sfpu13calculate_logILb1ELb1EEEvm
	.type	_ZN7ckernel4sfpu13calculate_logILb1ELb1EEEvm, @function
_ZN7ckernel4sfpu13calculate_logILb1ELb1EEEvm:
	slli	a0,a0,16
	li	a5,4
	lui	a3,%hi(_ZN7ckernel13instrn_bufferE)
	srli	a0,a0,16
.L69:
	SFPLOAD	L3, 1, 0
	SFPSETEXP	127, L3, L0, 1
	SFPNOP
	SFPNOP
	SFPLOADI	L1, 1, 11973
	SFPLOADI	L2, 1, 47539
	SFPMAD	L0, L1, L2, L1, 0
	SFPNOP
	SFPNOP
	SFPLOADI	L2, 1, 16428
	SFPMAD	L0, L1, L2, L1, 0
	SFPNOP
	SFPNOP
	SFPLOADI	L2, 1, 48615
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
	li	a2,1896939520
	add	a2, a2, a0
# Op(71) - d(1)
	sw	a2,0(a4)
	SFPMUL	L0, L1, L4, L0, 0
	SFPNOP
	SFPNOP
	SFPLOAD	L1, 1, 0
	SFPSETCC	0, L1, 6
	SFPNOP
	SFPLOADI	L0, 0, 65408
	SFPENCC	3, 10
	SFPNOP
	SFPSTORE	L0, 0, 0
 #APP
# 814 "ckernel.cc" 1
	.word (-536608768)
# 0 "" 2
 #NO_APP
	addi	a5,a5,-1
	bne	a5,zero,.L69
	ret
	.size	_ZN7ckernel4sfpu13calculate_logILb1ELb1EEEvm, .-_ZN7ckernel4sfpu13calculate_logILb1ELb1EEEvm
	.section	.text._ZN7ckernel4sfpu20calculate_reciprocalILb0ELi4EEEvv,"axG",@progbits,_ZN7ckernel4sfpu20calculate_reciprocalILb0ELi4EEEvv,comdat
	.align	1
	.weak	_ZN7ckernel4sfpu20calculate_reciprocalILb0ELi4EEEvv
	.type	_ZN7ckernel4sfpu20calculate_reciprocalILb0ELi4EEEvv, @function
_ZN7ckernel4sfpu20calculate_reciprocalILb0ELi4EEEvv:
	li	a5,4
.L72:
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
 #APP
# 585 "ckernel.cc" 1
	.word (-536608768)
# 0 "" 2
 #NO_APP
	addi	a5,a5,-1
	bne	a5,zero,.L72
	ret
	.size	_ZN7ckernel4sfpu20calculate_reciprocalILb0ELi4EEEvv, .-_ZN7ckernel4sfpu20calculate_reciprocalILb0ELi4EEEvv
	.section	.text._ZN7ckernel4sfpu20calculate_reciprocalILb1ELi4EEEvv,"axG",@progbits,_ZN7ckernel4sfpu20calculate_reciprocalILb1ELi4EEEvv,comdat
	.align	1
	.weak	_ZN7ckernel4sfpu20calculate_reciprocalILb1ELi4EEEvv
	.type	_ZN7ckernel4sfpu20calculate_reciprocalILb1ELi4EEEvv, @function
_ZN7ckernel4sfpu20calculate_reciprocalILb1ELi4EEEvv:
	li	a5,4
.L75:
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
 #APP
# 585 "ckernel.cc" 1
	.word (-536608768)
# 0 "" 2
 #NO_APP
	addi	a5,a5,-1
	bne	a5,zero,.L75
	ret
	.size	_ZN7ckernel4sfpu20calculate_reciprocalILb1ELi4EEEvv, .-_ZN7ckernel4sfpu20calculate_reciprocalILb1ELi4EEEvv
	.section	.text._ZN7ckernel4sfpu17calculate_dropoutILb0EEEvmm,"axG",@progbits,_ZN7ckernel4sfpu17calculate_dropoutILb0EEEvmm,comdat
	.align	1
	.weak	_ZN7ckernel4sfpu17calculate_dropoutILb0EEEvmm
	.type	_ZN7ckernel4sfpu17calculate_dropoutILb0EEEvmm, @function
_ZN7ckernel4sfpu17calculate_dropoutILb0EEEvmm:
	SFPLOADI	L1, 0, 43339
	slli	a1,a1,16
	slli	a0,a0,16
	li	a5,4
	lui	a3,%hi(_ZN7ckernel13instrn_bufferE)
	srli	a1,a1,8
	srli	a0,a0,16
.L78:
	lw	a4,%lo(_ZN7ckernel13instrn_bufferE)(a3)
	SFPLOAD	L0, 1, 0
	li	a2,1946157056
	add	a2, a2, a1
# Op(74) lv(0)  d(0)
	sw	a2,0(a4)
	SFPNOP
	SFPNOP
	SFPSTORE	L0, 0, 0
	SFPMOV	L3, L0, 0
	SFPNOP
	SFPNOP
	SFPSHFT	-3, L0, L0, 1
	SFPNOP
	SFPNOP
	li	a2,1898053632
	add	a2, a2, a0
# Op(71) - d(2)
	sw	a2,0(a4)
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
	SFPMOV	L1, L0, 0
	SFPNOP
	SFPNOP
	SFPAND	L3, L0
	SFPNOP
	SFPNOP
	SFPNOT	L0, L0
	SFPNOP
	SFPNOP
	SFPOR	L1, L3
	SFPNOP
	SFPNOP
	SFPAND	L0, L3
	SFPNOP
	SFPNOP
	SFPENCC	3, 10
	SFPNOP
 #APP
# 679 "ckernel.cc" 1
	.word (-536608768)
# 0 "" 2
 #NO_APP
	addi	a5,a5,-1
	bne	a5,zero,.L78
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
	sw	s1,4(sp)
	sw	s2,0(sp)
	SFPMOV	L15, L0, 0
	SFPNOP
	SFPNOP
	SFPSHFT	13, L0, L0, 1
	SFPNOP
	SFPNOP
	slli	a0,a0,16
	srli	a0,a0,16
	call	__floatunsisf
	lui	s2,%hi(_ZN7ckernel13instrn_bufferE)
	lw	a5,%lo(_ZN7ckernel13instrn_bufferE)(s2)
	srli	a0,a0,16
	li	a4,1896873984
	add	a4, a4, a0
# Op(71) - d(1)
	sw	a4,0(a5)
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
 #APP
# 904 "ckernel.cc" 1
	.word (-536608768)
# 0 "" 2
 #NO_APP
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
 #APP
# 904 "ckernel.cc" 1
	.word (-536608768)
# 0 "" 2
 #NO_APP
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
 #APP
# 904 "ckernel.cc" 1
	.word (-536608768)
# 0 "" 2
 #NO_APP
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
 #APP
# 904 "ckernel.cc" 1
	.word (-536608768)
# 0 "" 2
 #NO_APP
	mv	a0,s0
	call	_ZN7ckernel4sfpu14calculate_compILb0EL8SfpuType8EEEvm
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
 #APP
# 904 "ckernel.cc" 1
	.word (-536608768)
# 0 "" 2
 #NO_APP
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
 #APP
# 904 "ckernel.cc" 1
	.word (-536608768)
# 0 "" 2
 #NO_APP
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
 #APP
# 904 "ckernel.cc" 1
	.word (-536608768)
# 0 "" 2
 #NO_APP
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
 #APP
# 904 "ckernel.cc" 1
	.word (-536608768)
# 0 "" 2
 #NO_APP
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
 #APP
# 904 "ckernel.cc" 1
	.word (-536608768)
# 0 "" 2
 #NO_APP
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
 #APP
# 904 "ckernel.cc" 1
	.word (-536608768)
# 0 "" 2
 #NO_APP
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
 #APP
# 904 "ckernel.cc" 1
	.word (-536608768)
# 0 "" 2
 #NO_APP
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
 #APP
# 904 "ckernel.cc" 1
	.word (-536608768)
# 0 "" 2
 #NO_APP
	mv	a0,s0
	call	_ZN7ckernel4sfpu14calculate_compILb0EL8SfpuType10EEEvm
	lw	a4,%lo(_ZN7ckernel13instrn_bufferE)(s2)
	slli	a5,s0,16
	srli	a3,a5,16
	li	a2,1897988096
	add	a2, a2, a3
# Op(71) - d(2)
	sw	a2,0(a4)
	li	a2,1896939520
	add	a2, a2, a3
# Op(71) - d(1)
	sw	a2,0(a4)
	li	a3,4
	mv	s1,a5
	srli	a2,a5,8
.L81:
	SFPLOAD	L0, 1, 0
	SFPMAD	L2, L11, L0, L3, 0
	SFPNOP
	SFPNOP
	SFPSETCC	0, L3, 0
	SFPNOP
	lw	a4,%lo(_ZN7ckernel13instrn_bufferE)(s2)
	srli	a5,a5,16
	li	a1,1895890944
	add	a1, a1, a5
# Op(71) lv(0)  d(0)
	sw	a1,0(a4)
	SFPCOMPC
	SFPNOP
	SFPMAD	L1, L11, L0, L3, 0
	SFPNOP
	SFPNOP
	SFPSETCC	0, L3, 4
	SFPNOP
	li	a1,1895890944
	add	a1, a1, a5
# Op(71) lv(0)  d(0)
	sw	a1,0(a4)
	SFPENCC	3, 10
	SFPNOP
	mv	a5,s1
	li	a1,1962934272
	add	a1, a1, a2
# Op(75) lv(0)  d(0)
	sw	a1,0(a4)
	SFPNOP
	SFPNOP
	SFPSTORE	L0, 0, 0
 #APP
# 935 "ckernel.cc" 1
	.word (-536608768)
# 0 "" 2
 #NO_APP
	addi	a3,a3,-1
	bne	a3,zero,.L81
	SFPLOAD	L0, 1, 0
	SFPABS	L0, L0, 1
	SFPNOP
	SFPNOP
	SFPSTORE	L0, 0, 0
 #APP
# 947 "ckernel.cc" 1
	.word (-536608768)
# 0 "" 2
 #NO_APP
	SFPLOAD	L0, 1, 0
	SFPABS	L0, L0, 1
	SFPNOP
	SFPNOP
	SFPSTORE	L0, 0, 0
 #APP
# 947 "ckernel.cc" 1
	.word (-536608768)
# 0 "" 2
 #NO_APP
	SFPLOAD	L0, 1, 0
	SFPABS	L0, L0, 1
	SFPNOP
	SFPNOP
	SFPSTORE	L0, 0, 0
 #APP
# 947 "ckernel.cc" 1
	.word (-536608768)
# 0 "" 2
 #NO_APP
	SFPLOAD	L0, 1, 0
	SFPABS	L0, L0, 1
	SFPNOP
	SFPNOP
	SFPSTORE	L0, 0, 0
 #APP
# 947 "ckernel.cc" 1
	.word (-536608768)
# 0 "" 2
 #NO_APP
	li	a0,1
	call	_ZN7ckernel4sfpu14calculate_signILb0EEEvm
	call	_ZN7ckernel4sfpu17calculate_sigmoidILb0EEEvv
	call	_ZN7ckernel4sfpu14calculate_tanhILb1EEEvv
	mv	a2,s0
	mv	a1,s0
	mv	a0,s0
	call	_ZN7ckernel4sfpu18calculate_hardtanhILb0EEEvmmm
	lw	a5,%lo(_ZN7ckernel13instrn_bufferE)(s2)
	srli	s1,s1,16
	li	a4,1896873984
	add	a4, a4, s1
# Op(71) - d(1)
	sw	a4,0(a5)
	li	a5,4
.L82:
	SFPLOAD	L0, 1, 0
	SFPSETCC	0, L0, 0
	SFPNOP
	SFPMUL	L0, L1, L4, L0, 0
	SFPNOP
	SFPNOP
	SFPENCC	3, 10
	SFPNOP
	SFPSTORE	L0, 0, 0
 #APP
# 700 "ckernel.cc" 1
	.word (-536608768)
# 0 "" 2
 #NO_APP
	addi	a5,a5,-1
	bne	a5,zero,.L82
	call	_ZN7ckernel4sfpu25calculate_tanh_derivativeILb0ELi0EEEvv
	call	_ZN7ckernel4sfpu25calculate_tanh_derivativeILb0ELi1EEEvv
	li	a4,4
	li	a3,2
.L85:
	SFPLOAD	L1, 1, 0
	SFPMUL	L1, L1, L4, L0, 0
	SFPNOP
	SFPNOP
	bleu	s0,a3,.L83
	li	a5,2
.L84:
	SFPMUL	L0, L1, L4, L0, 0
	SFPNOP
	SFPNOP
	addi	a5,a5,1
	bne	s0,a5,.L84
.L83:
	SFPSTORE	L0, 0, 0
 #APP
# 717 "ckernel.cc" 1
	.word (-536608768)
# 0 "" 2
 #NO_APP
	addi	a4,a4,-1
	bne	a4,zero,.L85
	call	_ZN7ckernel4sfpu14calculate_geluILb0EEEvv
	call	_ZN7ckernel4sfpu14calculate_geluILb1EEEvv
	call	_ZN7ckernel4sfpu25calculate_gelu_derivativeILb0EEEvv
	call	_ZN7ckernel4sfpu14calculate_sqrtILb0ELi4ELi2EEEvv
	SFPLOAD	L0, 1, 0
	SFPIADD	0, L2, L0, 4
	SFPNOP
	SFPNOP
	SFPSHFT	-1, L0, L0, 1
	SFPNOP
	SFPNOP
	SFPSTORE	L0, 0, 0
 #APP
# 631 "ckernel.cc" 1
	.word (-536608768)
# 0 "" 2
 #NO_APP
	SFPLOAD	L0, 1, 0
	SFPIADD	0, L2, L0, 4
	SFPNOP
	SFPNOP
	SFPSHFT	-1, L0, L0, 1
	SFPNOP
	SFPNOP
	SFPSTORE	L0, 0, 0
 #APP
# 631 "ckernel.cc" 1
	.word (-536608768)
# 0 "" 2
 #NO_APP
	SFPLOAD	L0, 1, 0
	SFPIADD	0, L2, L0, 4
	SFPNOP
	SFPNOP
	SFPSHFT	-1, L0, L0, 1
	SFPNOP
	SFPNOP
	SFPSTORE	L0, 0, 0
 #APP
# 631 "ckernel.cc" 1
	.word (-536608768)
# 0 "" 2
 #NO_APP
	SFPLOAD	L0, 1, 0
	SFPIADD	0, L2, L0, 4
	SFPNOP
	SFPNOP
	SFPSHFT	-1, L0, L0, 1
	SFPNOP
	SFPNOP
	SFPSTORE	L0, 0, 0
 #APP
# 631 "ckernel.cc" 1
	.word (-536608768)
# 0 "" 2
 #NO_APP
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
	li	a5,4
.L86:
	SFPLOAD	L3, 1, 0
	SFPSETEXP	127, L3, L0, 1
	SFPNOP
	SFPNOP
	SFPLOADI	L1, 1, 11973
	SFPLOADI	L2, 1, 47539
	SFPMAD	L0, L1, L2, L1, 0
	SFPNOP
	SFPNOP
	SFPLOADI	L2, 1, 16428
	SFPMAD	L0, L1, L2, L1, 0
	SFPNOP
	SFPNOP
	SFPLOADI	L2, 1, 48615
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
	SFPLOADI	L0, 0, 65408
	SFPENCC	3, 10
	SFPNOP
	SFPSTORE	L0, 0, 0
 #APP
# 814 "ckernel.cc" 1
	.word (-536608768)
# 0 "" 2
 #NO_APP
	addi	a5,a5,-1
	bne	a5,zero,.L86
	call	_ZN7ckernel4sfpu20calculate_reciprocalILb0ELi4EEEvv
	call	_ZN7ckernel4sfpu20calculate_reciprocalILb1ELi4EEEvv
	mv	a0,s0
	mv	a1,s0
	call	_ZN7ckernel4sfpu17calculate_dropoutILb0EEEvmm
	lw	ra,12(sp)
	lw	s0,8(sp)
	lw	s1,4(sp)
	lw	s2,0(sp)
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
