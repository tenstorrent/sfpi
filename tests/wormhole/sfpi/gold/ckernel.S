	.file	"ckernel.cc"
	.option nopic
	.attribute arch, "rv32i2p0_w2p0"
	.attribute unaligned_access, 0
	.attribute stack_align, 16
	.text
	.align	1
	.type	_Z13calculate_logILb1ELb0EEvm.isra.0, @function
_Z13calculate_logILb1ELb0EEvm.isra.0:
	li	a5,8
.L2:
	SFPLOAD	L2, 0, 3, 0
	SFPSETEXP	127, L2, L0, 1
	SFPMAD	L0, L13, L14, L1, 0
	SFPLOADI	L3, 8, 16389
	SFPLOADI	L3, 10, 37644
	SFPMAD	L0, L1, L3, L1, 0
	SFPLOADI	L3, 8, 49084
	SFPLOADI	L3, 10, 54945
	SFPMAD	L0, L1, L3, L1, 0
	SFPEXEXP	L2, L0, 2
	SFPNOT	L0, L3
	SFPIADD	1, L3, L3, 5
	SFPSETSGN	1, L3, L0, 1
	SFPENCC	3, 10
	SFPCAST L0, L0, 0
	SFPMAD	L0, L12, L1, L0, 0
	SFPSETCC	0, L2, 6
	SFPLOADI	L0, 0, 65408
	SFPENCC	3, 10
	SFPSTORE	L0, 0, 3, 0
 #APP
# 663 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	addi	a5,a5,-1
	bne	a5,zero,.L2
	ret
	.size	_Z13calculate_logILb1ELb0EEvm.isra.0, .-_Z13calculate_logILb1ELb0EEvm.isra.0
	.align	1
	.type	_Z21calculate_exponentialILb1ELb1ELb0EEvt.isra.0, @function
_Z21calculate_exponentialILb1ELb1ELb0EEvt.isra.0:
	SFPLOAD	L0, 0, 3, 0
	SFPMAD	L0, L12, L13, L0, 0
	SFPNOP
	SFPIADD	0, L14, L0, 4
	SFPSHFT	8, L0, L0, 1
	SFPSTORE	L0, 0, 3, 0
	SFPSETCC	0, L0, 0
	SFPSTORE	L9, 0, 3, 0
	SFPENCC	3, 10
 #APP
# 268 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	SFPLOAD	L0, 0, 3, 0
	SFPMAD	L0, L12, L13, L0, 0
	SFPNOP
	SFPIADD	0, L14, L0, 4
	SFPSHFT	8, L0, L0, 1
	SFPSTORE	L0, 0, 3, 0
	SFPSETCC	0, L0, 0
	SFPSTORE	L9, 0, 3, 0
	SFPENCC	3, 10
 #APP
# 268 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	SFPLOAD	L0, 0, 3, 0
	SFPMAD	L0, L12, L13, L0, 0
	SFPNOP
	SFPIADD	0, L14, L0, 4
	SFPSHFT	8, L0, L0, 1
	SFPSTORE	L0, 0, 3, 0
	SFPSETCC	0, L0, 0
	SFPSTORE	L9, 0, 3, 0
	SFPENCC	3, 10
 #APP
# 268 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	SFPLOAD	L0, 0, 3, 0
	SFPMAD	L0, L12, L13, L0, 0
	SFPNOP
	SFPIADD	0, L14, L0, 4
	SFPSHFT	8, L0, L0, 1
	SFPSTORE	L0, 0, 3, 0
	SFPSETCC	0, L0, 0
	SFPSTORE	L9, 0, 3, 0
	SFPENCC	3, 10
 #APP
# 268 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	SFPLOAD	L0, 0, 3, 0
	SFPMAD	L0, L12, L13, L0, 0
	SFPNOP
	SFPIADD	0, L14, L0, 4
	SFPSHFT	8, L0, L0, 1
	SFPSTORE	L0, 0, 3, 0
	SFPSETCC	0, L0, 0
	SFPSTORE	L9, 0, 3, 0
	SFPENCC	3, 10
 #APP
# 268 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	SFPLOAD	L0, 0, 3, 0
	SFPMAD	L0, L12, L13, L0, 0
	SFPNOP
	SFPIADD	0, L14, L0, 4
	SFPSHFT	8, L0, L0, 1
	SFPSTORE	L0, 0, 3, 0
	SFPSETCC	0, L0, 0
	SFPSTORE	L9, 0, 3, 0
	SFPENCC	3, 10
 #APP
# 268 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	SFPLOAD	L0, 0, 3, 0
	SFPMAD	L0, L12, L13, L0, 0
	SFPNOP
	SFPIADD	0, L14, L0, 4
	SFPSHFT	8, L0, L0, 1
	SFPSTORE	L0, 0, 3, 0
	SFPSETCC	0, L0, 0
	SFPSTORE	L9, 0, 3, 0
	SFPENCC	3, 10
 #APP
# 268 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	SFPLOAD	L0, 0, 3, 0
	SFPMAD	L0, L12, L13, L0, 0
	SFPNOP
	SFPIADD	0, L14, L0, 4
	SFPSHFT	8, L0, L0, 1
	SFPSTORE	L0, 0, 3, 0
	SFPSETCC	0, L0, 0
	SFPSTORE	L9, 0, 3, 0
	SFPENCC	3, 10
 #APP
# 268 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	ret
	.size	_Z21calculate_exponentialILb1ELb1ELb0EEvt.isra.0, .-_Z21calculate_exponentialILb1ELb1ELb0EEvt.isra.0
	.align	1
	.type	_Z21calculate_exponentialILb1ELb0ELb0EEvt.isra.0, @function
_Z21calculate_exponentialILb1ELb0ELb0EEvt.isra.0:
	SFPLOAD	L0, 0, 3, 0
	SFPMAD	L0, L12, L13, L0, 0
	SFPNOP
	SFPIADD	0, L14, L0, 4
	SFPSHFT	8, L0, L0, 1
	SFPSTORE	L0, 0, 3, 0
 #APP
# 268 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	SFPLOAD	L0, 0, 3, 0
	SFPMAD	L0, L12, L13, L0, 0
	SFPNOP
	SFPIADD	0, L14, L0, 4
	SFPSHFT	8, L0, L0, 1
	SFPSTORE	L0, 0, 3, 0
 #APP
# 268 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	SFPLOAD	L0, 0, 3, 0
	SFPMAD	L0, L12, L13, L0, 0
	SFPNOP
	SFPIADD	0, L14, L0, 4
	SFPSHFT	8, L0, L0, 1
	SFPSTORE	L0, 0, 3, 0
 #APP
# 268 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	SFPLOAD	L0, 0, 3, 0
	SFPMAD	L0, L12, L13, L0, 0
	SFPNOP
	SFPIADD	0, L14, L0, 4
	SFPSHFT	8, L0, L0, 1
	SFPSTORE	L0, 0, 3, 0
 #APP
# 268 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	SFPLOAD	L0, 0, 3, 0
	SFPMAD	L0, L12, L13, L0, 0
	SFPNOP
	SFPIADD	0, L14, L0, 4
	SFPSHFT	8, L0, L0, 1
	SFPSTORE	L0, 0, 3, 0
 #APP
# 268 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	SFPLOAD	L0, 0, 3, 0
	SFPMAD	L0, L12, L13, L0, 0
	SFPNOP
	SFPIADD	0, L14, L0, 4
	SFPSHFT	8, L0, L0, 1
	SFPSTORE	L0, 0, 3, 0
 #APP
# 268 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	SFPLOAD	L0, 0, 3, 0
	SFPMAD	L0, L12, L13, L0, 0
	SFPNOP
	SFPIADD	0, L14, L0, 4
	SFPSHFT	8, L0, L0, 1
	SFPSTORE	L0, 0, 3, 0
 #APP
# 268 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	SFPLOAD	L0, 0, 3, 0
	SFPMAD	L0, L12, L13, L0, 0
	SFPNOP
	SFPIADD	0, L14, L0, 4
	SFPSHFT	8, L0, L0, 1
	SFPSTORE	L0, 0, 3, 0
 #APP
# 268 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	ret
	.size	_Z21calculate_exponentialILb1ELb0ELb0EEvt.isra.0, .-_Z21calculate_exponentialILb1ELb0ELb0EEvt.isra.0
	.align	1
	.type	_Z21calculate_exponentialILb0ELb1ELb0EEvt.isra.0, @function
_Z21calculate_exponentialILb0ELb1ELb0EEvt.isra.0:
	li	a5,8
.L8:
	SFPLOAD	L1, 0, 3, 0
	SFPSETSGN	0, L1, L0, 1
	SFPEXEXP	L0, L2, 10
	SFPSETEXP	126, L0, L0, 1
	SFPENCC	3, 10
	SFPLOADI	L3, 0, 16220
	SFPMAD	L0, L8, L3, L3, 0
	SFPNOP
	SFPMAD	L0, L3, L10, L0, 0
	SFPSETCC	0, L2, 4
	SFPMUL	L0, L0, L9, L0, 0
	SFPIADD	-1, L2, L2, 9
	SFPMUL	L0, L0, L9, L0, 0
	SFPIADD	-1, L2, L2, 9
	SFPMUL	L0, L0, L9, L0, 0
	SFPIADD	-1, L2, L2, 9
	SFPMUL	L0, L0, L9, L0, 0
	SFPIADD	-1, L2, L2, 9
	SFPMUL	L0, L0, L9, L0, 0
	SFPIADD	-1, L2, L2, 9
	SFPMUL	L0, L0, L9, L0, 0
	SFPIADD	-1, L2, L2, 9
	SFPMUL	L0, L0, L9, L0, 0
	SFPIADD	-1, L2, L2, 9
	SFPMUL	L0, L0, L9, L0, 0
	SFPENCC	3, 10
	SFPSETCC	0, L1, 0
	SFPSETSGN	1, L0, L2, 1
	SFPSETEXP	126, L2, L2, 1
	SFPMAD	L2, L12, L13, L1, 0
	SFPNOP
	SFPMUL	L12, L1, L9, L1, 0
	SFPNOP
	SFPMAD	L2, L1, L13, L3, 0
	SFPNOP
	SFPMUL	L1, L3, L9, L1, 0
	SFPNOP
	SFPMAD	L2, L1, L13, L2, 0
	SFPNOP
	SFPMUL	L1, L2, L9, L1, 0
	SFPEXEXP	L0, L2, 0
	SFPEXEXP	L1, L3, 0
	SFPIADD	0, L3, L2, 6
	SFPPUSHC	0
	SFPIADD	126, L2, L2, 1
	SFPLOADI	L1, 0, 0
	SFPLOADI	L2, 4, 0
	SFPPOPC	0
	SFPSETEXP	0, L1, L2, 0
	SFPMOV	L2, L0, 0
	SFPENCC	3, 10
	SFPSTORE	L0, 0, 3, 0
 #APP
# 268 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	addi	a5,a5,-1
	bne	a5,zero,.L8
	ret
	.size	_Z21calculate_exponentialILb0ELb1ELb0EEvt.isra.0, .-_Z21calculate_exponentialILb0ELb1ELb0EEvt.isra.0
	.align	1
	.type	_Z21calculate_exponentialILb0ELb0ELb0EEvt.isra.0, @function
_Z21calculate_exponentialILb0ELb0ELb0EEvt.isra.0:
	li	a5,8
.L11:
	SFPLOAD	L1, 0, 3, 0
	SFPSETSGN	0, L1, L0, 1
	SFPEXEXP	L0, L2, 10
	SFPSETEXP	126, L0, L0, 1
	SFPENCC	3, 10
	SFPLOADI	L3, 0, 16220
	SFPMAD	L0, L8, L3, L3, 0
	SFPNOP
	SFPMAD	L0, L3, L10, L0, 0
	SFPSETCC	0, L2, 4
	SFPMUL	L0, L0, L9, L0, 0
	SFPIADD	-1, L2, L2, 9
	SFPMUL	L0, L0, L9, L0, 0
	SFPIADD	-1, L2, L2, 9
	SFPMUL	L0, L0, L9, L0, 0
	SFPIADD	-1, L2, L2, 9
	SFPMUL	L0, L0, L9, L0, 0
	SFPIADD	-1, L2, L2, 9
	SFPMUL	L0, L0, L9, L0, 0
	SFPIADD	-1, L2, L2, 9
	SFPMUL	L0, L0, L9, L0, 0
	SFPIADD	-1, L2, L2, 9
	SFPMUL	L0, L0, L9, L0, 0
	SFPIADD	-1, L2, L2, 9
	SFPMUL	L0, L0, L9, L0, 0
	SFPENCC	3, 10
	SFPSETCC	0, L1, 0
	SFPSETSGN	1, L0, L2, 1
	SFPSETEXP	126, L2, L2, 1
	SFPMAD	L2, L12, L13, L1, 0
	SFPNOP
	SFPMUL	L12, L1, L9, L1, 0
	SFPNOP
	SFPMAD	L2, L1, L13, L3, 0
	SFPNOP
	SFPMUL	L1, L3, L9, L1, 0
	SFPNOP
	SFPMAD	L2, L1, L13, L2, 0
	SFPNOP
	SFPMUL	L1, L2, L9, L1, 0
	SFPEXEXP	L0, L2, 0
	SFPEXEXP	L1, L3, 0
	SFPIADD	0, L3, L2, 6
	SFPPUSHC	0
	SFPIADD	126, L2, L2, 1
	SFPLOADI	L1, 0, 0
	SFPLOADI	L2, 4, 0
	SFPPOPC	0
	SFPSETEXP	0, L1, L2, 0
	SFPMOV	L2, L0, 0
	SFPENCC	3, 10
	SFPSTORE	L0, 0, 3, 0
 #APP
# 268 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	addi	a5,a5,-1
	bne	a5,zero,.L11
	ret
	.size	_Z21calculate_exponentialILb0ELb0ELb0EEvt.isra.0, .-_Z21calculate_exponentialILb0ELb0ELb0EEvt.isra.0
	.section	.text._Z14calculate_compILb0EL8SfpuType8EEvm,"axG",@progbits,_Z14calculate_compILb0EL8SfpuType8EEvm,comdat
	.align	1
	.weak	_Z14calculate_compILb0EL8SfpuType8EEvm
	.type	_Z14calculate_compILb0EL8SfpuType8EEvm, @function
_Z14calculate_compILb0EL8SfpuType8EEvm:
	bne	a0,zero,.L14
	SFPLOAD	L1, 0, 3, 0
	SFPLOADI	L0, 4, 14336
	SFPIADD	0, L1, L0, 4
	SFPSETCC	0, L0, 6
	SFPLOADI	L0, 0, 0
	SFPCOMPC
	SFPLOADI	L0, 0, 16256
	SFPENCC	3, 10
	SFPSTORE	L0, 0, 3, 0
 #APP
# 746 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	SFPLOAD	L1, 0, 3, 0
	SFPLOADI	L0, 4, 14336
	SFPIADD	0, L1, L0, 4
	SFPSETCC	0, L0, 6
	SFPLOADI	L0, 0, 0
	SFPCOMPC
	SFPLOADI	L0, 0, 16256
	SFPENCC	3, 10
	SFPSTORE	L0, 0, 3, 0
 #APP
# 746 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	SFPLOAD	L1, 0, 3, 0
	SFPLOADI	L0, 4, 14336
	SFPIADD	0, L1, L0, 4
	SFPSETCC	0, L0, 6
	SFPLOADI	L0, 0, 0
	SFPCOMPC
	SFPLOADI	L0, 0, 16256
	SFPENCC	3, 10
	SFPSTORE	L0, 0, 3, 0
 #APP
# 746 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	SFPLOAD	L1, 0, 3, 0
	SFPLOADI	L0, 4, 14336
	SFPIADD	0, L1, L0, 4
	SFPSETCC	0, L0, 6
	SFPLOADI	L0, 0, 0
	SFPCOMPC
	SFPLOADI	L0, 0, 16256
	SFPENCC	3, 10
	SFPSTORE	L0, 0, 3, 0
 #APP
# 746 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	SFPLOAD	L1, 0, 3, 0
	SFPLOADI	L0, 4, 14336
	SFPIADD	0, L1, L0, 4
	SFPSETCC	0, L0, 6
	SFPLOADI	L0, 0, 0
	SFPCOMPC
	SFPLOADI	L0, 0, 16256
	SFPENCC	3, 10
	SFPSTORE	L0, 0, 3, 0
 #APP
# 746 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	SFPLOAD	L1, 0, 3, 0
	SFPLOADI	L0, 4, 14336
	SFPIADD	0, L1, L0, 4
	SFPSETCC	0, L0, 6
	SFPLOADI	L0, 0, 0
	SFPCOMPC
	SFPLOADI	L0, 0, 16256
	SFPENCC	3, 10
	SFPSTORE	L0, 0, 3, 0
 #APP
# 746 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	SFPLOAD	L1, 0, 3, 0
	SFPLOADI	L0, 4, 14336
	SFPIADD	0, L1, L0, 4
	SFPSETCC	0, L0, 6
	SFPLOADI	L0, 0, 0
	SFPCOMPC
	SFPLOADI	L0, 0, 16256
	SFPENCC	3, 10
	SFPSTORE	L0, 0, 3, 0
 #APP
# 746 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	SFPLOAD	L1, 0, 3, 0
	SFPLOADI	L0, 4, 14336
	SFPIADD	0, L1, L0, 4
	SFPSETCC	0, L0, 6
	SFPLOADI	L0, 0, 0
	SFPCOMPC
	SFPLOADI	L0, 0, 16256
	SFPENCC	3, 10
	SFPSTORE	L0, 0, 3, 0
 #APP
# 746 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	ret
.L14:
	SFPLOAD	L0, 0, 3, 0
	SFPSETCC	0, L0, 6
	SFPLOADI	L0, 0, 0
	SFPCOMPC
	SFPLOADI	L0, 0, 16256
	SFPENCC	3, 10
	SFPSTORE	L0, 0, 3, 0
 #APP
# 746 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	SFPLOAD	L0, 0, 3, 0
	SFPSETCC	0, L0, 6
	SFPLOADI	L0, 0, 0
	SFPCOMPC
	SFPLOADI	L0, 0, 16256
	SFPENCC	3, 10
	SFPSTORE	L0, 0, 3, 0
 #APP
# 746 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	SFPLOAD	L0, 0, 3, 0
	SFPSETCC	0, L0, 6
	SFPLOADI	L0, 0, 0
	SFPCOMPC
	SFPLOADI	L0, 0, 16256
	SFPENCC	3, 10
	SFPSTORE	L0, 0, 3, 0
 #APP
# 746 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	SFPLOAD	L0, 0, 3, 0
	SFPSETCC	0, L0, 6
	SFPLOADI	L0, 0, 0
	SFPCOMPC
	SFPLOADI	L0, 0, 16256
	SFPENCC	3, 10
	SFPSTORE	L0, 0, 3, 0
 #APP
# 746 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	SFPLOAD	L0, 0, 3, 0
	SFPSETCC	0, L0, 6
	SFPLOADI	L0, 0, 0
	SFPCOMPC
	SFPLOADI	L0, 0, 16256
	SFPENCC	3, 10
	SFPSTORE	L0, 0, 3, 0
 #APP
# 746 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	SFPLOAD	L0, 0, 3, 0
	SFPSETCC	0, L0, 6
	SFPLOADI	L0, 0, 0
	SFPCOMPC
	SFPLOADI	L0, 0, 16256
	SFPENCC	3, 10
	SFPSTORE	L0, 0, 3, 0
 #APP
# 746 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	SFPLOAD	L0, 0, 3, 0
	SFPSETCC	0, L0, 6
	SFPLOADI	L0, 0, 0
	SFPCOMPC
	SFPLOADI	L0, 0, 16256
	SFPENCC	3, 10
	SFPSTORE	L0, 0, 3, 0
 #APP
# 746 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	SFPLOAD	L0, 0, 3, 0
	SFPSETCC	0, L0, 6
	SFPLOADI	L0, 0, 0
	SFPCOMPC
	SFPLOADI	L0, 0, 16256
	SFPENCC	3, 10
	SFPSTORE	L0, 0, 3, 0
 #APP
# 746 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	ret
	.size	_Z14calculate_compILb0EL8SfpuType8EEvm, .-_Z14calculate_compILb0EL8SfpuType8EEvm
	.section	.text._Z14calculate_compILb0EL8SfpuType10EEvm,"axG",@progbits,_Z14calculate_compILb0EL8SfpuType10EEvm,comdat
	.align	1
	.weak	_Z14calculate_compILb0EL8SfpuType10EEvm
	.type	_Z14calculate_compILb0EL8SfpuType10EEvm, @function
_Z14calculate_compILb0EL8SfpuType10EEvm:
	bne	a0,zero,.L18
	SFPLOAD	L1, 0, 3, 0
	SFPLOADI	L0, 4, 14336
	SFPIADD	0, L1, L0, 4
	SFPSETCC	0, L0, 6
	SFPLOADI	L0, 0, 16256
	SFPCOMPC
	SFPLOADI	L0, 0, 0
	SFPENCC	3, 10
	SFPSTORE	L0, 0, 3, 0
 #APP
# 746 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	SFPLOAD	L1, 0, 3, 0
	SFPLOADI	L0, 4, 14336
	SFPIADD	0, L1, L0, 4
	SFPSETCC	0, L0, 6
	SFPLOADI	L0, 0, 16256
	SFPCOMPC
	SFPLOADI	L0, 0, 0
	SFPENCC	3, 10
	SFPSTORE	L0, 0, 3, 0
 #APP
# 746 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	SFPLOAD	L1, 0, 3, 0
	SFPLOADI	L0, 4, 14336
	SFPIADD	0, L1, L0, 4
	SFPSETCC	0, L0, 6
	SFPLOADI	L0, 0, 16256
	SFPCOMPC
	SFPLOADI	L0, 0, 0
	SFPENCC	3, 10
	SFPSTORE	L0, 0, 3, 0
 #APP
# 746 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	SFPLOAD	L1, 0, 3, 0
	SFPLOADI	L0, 4, 14336
	SFPIADD	0, L1, L0, 4
	SFPSETCC	0, L0, 6
	SFPLOADI	L0, 0, 16256
	SFPCOMPC
	SFPLOADI	L0, 0, 0
	SFPENCC	3, 10
	SFPSTORE	L0, 0, 3, 0
 #APP
# 746 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	SFPLOAD	L1, 0, 3, 0
	SFPLOADI	L0, 4, 14336
	SFPIADD	0, L1, L0, 4
	SFPSETCC	0, L0, 6
	SFPLOADI	L0, 0, 16256
	SFPCOMPC
	SFPLOADI	L0, 0, 0
	SFPENCC	3, 10
	SFPSTORE	L0, 0, 3, 0
 #APP
# 746 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	SFPLOAD	L1, 0, 3, 0
	SFPLOADI	L0, 4, 14336
	SFPIADD	0, L1, L0, 4
	SFPSETCC	0, L0, 6
	SFPLOADI	L0, 0, 16256
	SFPCOMPC
	SFPLOADI	L0, 0, 0
	SFPENCC	3, 10
	SFPSTORE	L0, 0, 3, 0
 #APP
# 746 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	SFPLOAD	L1, 0, 3, 0
	SFPLOADI	L0, 4, 14336
	SFPIADD	0, L1, L0, 4
	SFPSETCC	0, L0, 6
	SFPLOADI	L0, 0, 16256
	SFPCOMPC
	SFPLOADI	L0, 0, 0
	SFPENCC	3, 10
	SFPSTORE	L0, 0, 3, 0
 #APP
# 746 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	SFPLOAD	L1, 0, 3, 0
	SFPLOADI	L0, 4, 14336
	SFPIADD	0, L1, L0, 4
	SFPSETCC	0, L0, 6
	SFPLOADI	L0, 0, 16256
	SFPCOMPC
	SFPLOADI	L0, 0, 0
	SFPENCC	3, 10
	SFPSTORE	L0, 0, 3, 0
 #APP
# 746 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	ret
.L18:
	SFPLOAD	L0, 0, 3, 0
	SFPSETCC	0, L0, 6
	SFPLOADI	L0, 0, 16256
	SFPCOMPC
	SFPLOADI	L0, 0, 0
	SFPENCC	3, 10
	SFPSTORE	L0, 0, 3, 0
 #APP
# 746 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	SFPLOAD	L0, 0, 3, 0
	SFPSETCC	0, L0, 6
	SFPLOADI	L0, 0, 16256
	SFPCOMPC
	SFPLOADI	L0, 0, 0
	SFPENCC	3, 10
	SFPSTORE	L0, 0, 3, 0
 #APP
# 746 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	SFPLOAD	L0, 0, 3, 0
	SFPSETCC	0, L0, 6
	SFPLOADI	L0, 0, 16256
	SFPCOMPC
	SFPLOADI	L0, 0, 0
	SFPENCC	3, 10
	SFPSTORE	L0, 0, 3, 0
 #APP
# 746 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	SFPLOAD	L0, 0, 3, 0
	SFPSETCC	0, L0, 6
	SFPLOADI	L0, 0, 16256
	SFPCOMPC
	SFPLOADI	L0, 0, 0
	SFPENCC	3, 10
	SFPSTORE	L0, 0, 3, 0
 #APP
# 746 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	SFPLOAD	L0, 0, 3, 0
	SFPSETCC	0, L0, 6
	SFPLOADI	L0, 0, 16256
	SFPCOMPC
	SFPLOADI	L0, 0, 0
	SFPENCC	3, 10
	SFPSTORE	L0, 0, 3, 0
 #APP
# 746 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	SFPLOAD	L0, 0, 3, 0
	SFPSETCC	0, L0, 6
	SFPLOADI	L0, 0, 16256
	SFPCOMPC
	SFPLOADI	L0, 0, 0
	SFPENCC	3, 10
	SFPSTORE	L0, 0, 3, 0
 #APP
# 746 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	SFPLOAD	L0, 0, 3, 0
	SFPSETCC	0, L0, 6
	SFPLOADI	L0, 0, 16256
	SFPCOMPC
	SFPLOADI	L0, 0, 0
	SFPENCC	3, 10
	SFPSTORE	L0, 0, 3, 0
 #APP
# 746 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	SFPLOAD	L0, 0, 3, 0
	SFPSETCC	0, L0, 6
	SFPLOADI	L0, 0, 16256
	SFPCOMPC
	SFPLOADI	L0, 0, 0
	SFPENCC	3, 10
	SFPSTORE	L0, 0, 3, 0
 #APP
# 746 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	ret
	.size	_Z14calculate_compILb0EL8SfpuType10EEvm, .-_Z14calculate_compILb0EL8SfpuType10EEvm
	.section	.text._Z15calculate_clampILb0EEvmmm,"axG",@progbits,_Z15calculate_clampILb0EEvmmm,comdat
	.align	1
	.weak	_Z15calculate_clampILb0EEvmmm
	.type	_Z15calculate_clampILb0EEvmmm, @function
_Z15calculate_clampILb0EEvmmm:
	lui	a6,%hi(_ZN7ckernel13instrn_bufferE)
	lw	a4,%lo(_ZN7ckernel13instrn_bufferE)(a6)
	slli	a5,a0,16
	srli	a5,a5,16
	li	a3,1899036672
	add	a3, a3, a5
# Op(0x71) - d(3)
	sw	a3,0(a4)
	slli	a3,a1,16
	srli	a3,a3,16
	li	a1,1897988096
	add	a1, a1, a3
# Op(0x71) - d(2)
	sw	a1,0(a4)
	slli	a2,a2,16
	li	a4,8
	mv	a1,a5
	srli	a2,a2,8
.L22:
	SFPLOAD	L0, 0, 3, 0
	SFPMAD	L3, L11, L0, L1, 0
	SFPNOP
	SFPSETCC	0, L1, 0
	lw	a5,%lo(_ZN7ckernel13instrn_bufferE)(a6)
	li	a0,1895890944
	add	a0, a0, a1
# Op(0x71) lv(0)  d(0)
	sw	a0,0(a5)
	SFPCOMPC
	SFPMAD	L2, L11, L0, L1, 0
	SFPNOP
	SFPSETCC	0, L1, 4
	li	a0,1895890944
	add	a0, a0, a3
# Op(0x71) lv(0)  d(0)
	sw	a0,0(a5)
	SFPENCC	3, 10
	li	a0,1962934272
	add	a0, a0, a2
# Op(0x75) lv(0)  d(0)
	sw	a0,0(a5)
	SFPNOP
	SFPNOP
	SFPNOP
	SFPSTORE	L0, 0, 3, 0
 #APP
# 777 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	addi	a4,a4,-1
	bne	a4,zero,.L22
	ret
	.size	_Z15calculate_clampILb0EEvmmm, .-_Z15calculate_clampILb0EEvmmm
	.section	.text._Z13calculate_absILb0EEvv,"axG",@progbits,_Z13calculate_absILb0EEvv,comdat
	.align	1
	.weak	_Z13calculate_absILb0EEvv
	.type	_Z13calculate_absILb0EEvv, @function
_Z13calculate_absILb0EEvv:
	SFPLOAD	L0, 0, 3, 0
	SFPABS	L0, L0, 1
	SFPSTORE	L0, 0, 3, 0
 #APP
# 789 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	SFPLOAD	L0, 0, 3, 0
	SFPABS	L0, L0, 1
	SFPSTORE	L0, 0, 3, 0
 #APP
# 789 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	SFPLOAD	L0, 0, 3, 0
	SFPABS	L0, L0, 1
	SFPSTORE	L0, 0, 3, 0
 #APP
# 789 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	SFPLOAD	L0, 0, 3, 0
	SFPABS	L0, L0, 1
	SFPSTORE	L0, 0, 3, 0
 #APP
# 789 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	SFPLOAD	L0, 0, 3, 0
	SFPABS	L0, L0, 1
	SFPSTORE	L0, 0, 3, 0
 #APP
# 789 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	SFPLOAD	L0, 0, 3, 0
	SFPABS	L0, L0, 1
	SFPSTORE	L0, 0, 3, 0
 #APP
# 789 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	SFPLOAD	L0, 0, 3, 0
	SFPABS	L0, L0, 1
	SFPSTORE	L0, 0, 3, 0
 #APP
# 789 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	SFPLOAD	L0, 0, 3, 0
	SFPABS	L0, L0, 1
	SFPSTORE	L0, 0, 3, 0
 #APP
# 789 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	ret
	.size	_Z13calculate_absILb0EEvv, .-_Z13calculate_absILb0EEvv
	.section	.text._Z14calculate_signILb0EEvm,"axG",@progbits,_Z14calculate_signILb0EEvm,comdat
	.align	1
	.weak	_Z14calculate_signILb0EEvm
	.type	_Z14calculate_signILb0EEvm, @function
_Z14calculate_signILb0EEvm:
	bne	a0,zero,.L26
	SFPLOAD	L1, 0, 3, 0
	SFPSTORE	L10, 0, 3, 0
	SFPSETCC	0, L1, 0
	SFPSTORE	L11, 0, 3, 0
	SFPENCC	3, 10
	SFPLOADI	L0, 4, 14336
	SFPIADD	0, L1, L0, 4
	SFPSETCC	0, L0, 6
	SFPSTORE	L9, 0, 3, 0
	SFPENCC	3, 10
 #APP
# 814 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	SFPLOAD	L1, 0, 3, 0
	SFPSTORE	L10, 0, 3, 0
	SFPSETCC	0, L1, 0
	SFPSTORE	L11, 0, 3, 0
	SFPENCC	3, 10
	SFPLOADI	L0, 4, 14336
	SFPIADD	0, L1, L0, 4
	SFPSETCC	0, L0, 6
	SFPSTORE	L9, 0, 3, 0
	SFPENCC	3, 10
 #APP
# 814 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	SFPLOAD	L1, 0, 3, 0
	SFPSTORE	L10, 0, 3, 0
	SFPSETCC	0, L1, 0
	SFPSTORE	L11, 0, 3, 0
	SFPENCC	3, 10
	SFPLOADI	L0, 4, 14336
	SFPIADD	0, L1, L0, 4
	SFPSETCC	0, L0, 6
	SFPSTORE	L9, 0, 3, 0
	SFPENCC	3, 10
 #APP
# 814 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	SFPLOAD	L1, 0, 3, 0
	SFPSTORE	L10, 0, 3, 0
	SFPSETCC	0, L1, 0
	SFPSTORE	L11, 0, 3, 0
	SFPENCC	3, 10
	SFPLOADI	L0, 4, 14336
	SFPIADD	0, L1, L0, 4
	SFPSETCC	0, L0, 6
	SFPSTORE	L9, 0, 3, 0
	SFPENCC	3, 10
 #APP
# 814 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	SFPLOAD	L1, 0, 3, 0
	SFPSTORE	L10, 0, 3, 0
	SFPSETCC	0, L1, 0
	SFPSTORE	L11, 0, 3, 0
	SFPENCC	3, 10
	SFPLOADI	L0, 4, 14336
	SFPIADD	0, L1, L0, 4
	SFPSETCC	0, L0, 6
	SFPSTORE	L9, 0, 3, 0
	SFPENCC	3, 10
 #APP
# 814 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	SFPLOAD	L1, 0, 3, 0
	SFPSTORE	L10, 0, 3, 0
	SFPSETCC	0, L1, 0
	SFPSTORE	L11, 0, 3, 0
	SFPENCC	3, 10
	SFPLOADI	L0, 4, 14336
	SFPIADD	0, L1, L0, 4
	SFPSETCC	0, L0, 6
	SFPSTORE	L9, 0, 3, 0
	SFPENCC	3, 10
 #APP
# 814 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	SFPLOAD	L1, 0, 3, 0
	SFPSTORE	L10, 0, 3, 0
	SFPSETCC	0, L1, 0
	SFPSTORE	L11, 0, 3, 0
	SFPENCC	3, 10
	SFPLOADI	L0, 4, 14336
	SFPIADD	0, L1, L0, 4
	SFPSETCC	0, L0, 6
	SFPSTORE	L9, 0, 3, 0
	SFPENCC	3, 10
 #APP
# 814 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	SFPLOAD	L1, 0, 3, 0
	SFPSTORE	L10, 0, 3, 0
	SFPSETCC	0, L1, 0
	SFPSTORE	L11, 0, 3, 0
	SFPENCC	3, 10
	SFPLOADI	L0, 4, 14336
	SFPIADD	0, L1, L0, 4
	SFPSETCC	0, L0, 6
	SFPSTORE	L9, 0, 3, 0
	SFPENCC	3, 10
 #APP
# 814 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	ret
.L26:
	SFPLOAD	L0, 0, 3, 0
	SFPSTORE	L10, 0, 3, 0
	SFPSETCC	0, L0, 0
	SFPSTORE	L11, 0, 3, 0
	SFPENCC	3, 10
	SFPSETCC	0, L0, 6
	SFPSTORE	L9, 0, 3, 0
	SFPENCC	3, 10
 #APP
# 814 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	SFPLOAD	L0, 0, 3, 0
	SFPSTORE	L10, 0, 3, 0
	SFPSETCC	0, L0, 0
	SFPSTORE	L11, 0, 3, 0
	SFPENCC	3, 10
	SFPSETCC	0, L0, 6
	SFPSTORE	L9, 0, 3, 0
	SFPENCC	3, 10
 #APP
# 814 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	SFPLOAD	L0, 0, 3, 0
	SFPSTORE	L10, 0, 3, 0
	SFPSETCC	0, L0, 0
	SFPSTORE	L11, 0, 3, 0
	SFPENCC	3, 10
	SFPSETCC	0, L0, 6
	SFPSTORE	L9, 0, 3, 0
	SFPENCC	3, 10
 #APP
# 814 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	SFPLOAD	L0, 0, 3, 0
	SFPSTORE	L10, 0, 3, 0
	SFPSETCC	0, L0, 0
	SFPSTORE	L11, 0, 3, 0
	SFPENCC	3, 10
	SFPSETCC	0, L0, 6
	SFPSTORE	L9, 0, 3, 0
	SFPENCC	3, 10
 #APP
# 814 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	SFPLOAD	L0, 0, 3, 0
	SFPSTORE	L10, 0, 3, 0
	SFPSETCC	0, L0, 0
	SFPSTORE	L11, 0, 3, 0
	SFPENCC	3, 10
	SFPSETCC	0, L0, 6
	SFPSTORE	L9, 0, 3, 0
	SFPENCC	3, 10
 #APP
# 814 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	SFPLOAD	L0, 0, 3, 0
	SFPSTORE	L10, 0, 3, 0
	SFPSETCC	0, L0, 0
	SFPSTORE	L11, 0, 3, 0
	SFPENCC	3, 10
	SFPSETCC	0, L0, 6
	SFPSTORE	L9, 0, 3, 0
	SFPENCC	3, 10
 #APP
# 814 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	SFPLOAD	L0, 0, 3, 0
	SFPSTORE	L10, 0, 3, 0
	SFPSETCC	0, L0, 0
	SFPSTORE	L11, 0, 3, 0
	SFPENCC	3, 10
	SFPSETCC	0, L0, 6
	SFPSTORE	L9, 0, 3, 0
	SFPENCC	3, 10
 #APP
# 814 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	SFPLOAD	L0, 0, 3, 0
	SFPSTORE	L10, 0, 3, 0
	SFPSETCC	0, L0, 0
	SFPSTORE	L11, 0, 3, 0
	SFPENCC	3, 10
	SFPSETCC	0, L0, 6
	SFPSTORE	L9, 0, 3, 0
	SFPENCC	3, 10
 #APP
# 814 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	ret
	.size	_Z14calculate_signILb0EEvm, .-_Z14calculate_signILb0EEvm
	.section	.text._Z17calculate_sigmoidILb0EEvv,"axG",@progbits,_Z17calculate_sigmoidILb0EEvv,comdat
	.align	1
	.weak	_Z17calculate_sigmoidILb0EEvv
	.type	_Z17calculate_sigmoidILb0EEvv, @function
_Z17calculate_sigmoidILb0EEvv:
	SFPLOAD	L3, 0, 3, 0
	SFPLUT	L3, 4
	SFPNOP
	SFPADDI	16128, L3, 0
	SFPNOP
	SFPSTORE	L3, 0, 3, 0
 #APP
# 333 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	SFPLOAD	L3, 0, 3, 0
	SFPLUT	L3, 4
	SFPNOP
	SFPADDI	16128, L3, 0
	SFPNOP
	SFPSTORE	L3, 0, 3, 0
 #APP
# 333 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	SFPLOAD	L3, 0, 3, 0
	SFPLUT	L3, 4
	SFPNOP
	SFPADDI	16128, L3, 0
	SFPNOP
	SFPSTORE	L3, 0, 3, 0
 #APP
# 333 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	SFPLOAD	L3, 0, 3, 0
	SFPLUT	L3, 4
	SFPNOP
	SFPADDI	16128, L3, 0
	SFPNOP
	SFPSTORE	L3, 0, 3, 0
 #APP
# 333 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	SFPLOAD	L3, 0, 3, 0
	SFPLUT	L3, 4
	SFPNOP
	SFPADDI	16128, L3, 0
	SFPNOP
	SFPSTORE	L3, 0, 3, 0
 #APP
# 333 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	SFPLOAD	L3, 0, 3, 0
	SFPLUT	L3, 4
	SFPNOP
	SFPADDI	16128, L3, 0
	SFPNOP
	SFPSTORE	L3, 0, 3, 0
 #APP
# 333 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	SFPLOAD	L3, 0, 3, 0
	SFPLUT	L3, 4
	SFPNOP
	SFPADDI	16128, L3, 0
	SFPNOP
	SFPSTORE	L3, 0, 3, 0
 #APP
# 333 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	SFPLOAD	L3, 0, 3, 0
	SFPLUT	L3, 4
	SFPNOP
	SFPADDI	16128, L3, 0
	SFPNOP
	SFPSTORE	L3, 0, 3, 0
 #APP
# 333 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	ret
	.size	_Z17calculate_sigmoidILb0EEvv, .-_Z17calculate_sigmoidILb0EEvv
	.section	.text._Z14calculate_tanhILb1EEvv,"axG",@progbits,_Z14calculate_tanhILb1EEvv,comdat
	.align	1
	.weak	_Z14calculate_tanhILb1EEvv
	.type	_Z14calculate_tanhILb1EEvv, @function
_Z14calculate_tanhILb1EEvv:
	SFPLOAD	L3, 0, 3, 0
	SFPLUT	L3, 4
	SFPNOP
	SFPSTORE	L3, 0, 3, 0
 #APP
# 354 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	SFPLOAD	L3, 0, 3, 0
	SFPLUT	L3, 4
	SFPNOP
	SFPSTORE	L3, 0, 3, 0
 #APP
# 354 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	SFPLOAD	L3, 0, 3, 0
	SFPLUT	L3, 4
	SFPNOP
	SFPSTORE	L3, 0, 3, 0
 #APP
# 354 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	SFPLOAD	L3, 0, 3, 0
	SFPLUT	L3, 4
	SFPNOP
	SFPSTORE	L3, 0, 3, 0
 #APP
# 354 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	SFPLOAD	L3, 0, 3, 0
	SFPLUT	L3, 4
	SFPNOP
	SFPSTORE	L3, 0, 3, 0
 #APP
# 354 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	SFPLOAD	L3, 0, 3, 0
	SFPLUT	L3, 4
	SFPNOP
	SFPSTORE	L3, 0, 3, 0
 #APP
# 354 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	SFPLOAD	L3, 0, 3, 0
	SFPLUT	L3, 4
	SFPNOP
	SFPSTORE	L3, 0, 3, 0
 #APP
# 354 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	SFPLOAD	L3, 0, 3, 0
	SFPLUT	L3, 4
	SFPNOP
	SFPSTORE	L3, 0, 3, 0
 #APP
# 354 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	ret
	.size	_Z14calculate_tanhILb1EEvv, .-_Z14calculate_tanhILb1EEvv
	.section	.text._Z18calculate_hardtanhILb0EEvmmm,"axG",@progbits,_Z18calculate_hardtanhILb0EEvmmm,comdat
	.align	1
	.weak	_Z18calculate_hardtanhILb0EEvmmm
	.type	_Z18calculate_hardtanhILb0EEvmmm, @function
_Z18calculate_hardtanhILb0EEvmmm:
	lui	a5,%hi(_ZN7ckernel13instrn_bufferE)
	lw	a5,%lo(_ZN7ckernel13instrn_bufferE)(a5)
	slli	a0,a0,16
	srli	a0,a0,16
	li	a4,1898971136
	add	a4, a4, a0
# Op(0x71) - d(3)
	sw	a4,0(a5)
	slli	a1,a1,16
	srli	a1,a1,16
	li	a4,1897922560
	add	a4, a4, a1
# Op(0x71) - d(2)
	sw	a4,0(a5)
	slli	a2,a2,16
	srli	a2,a2,16
	li	a4,1896873984
	add	a4, a4, a2
# Op(0x71) - d(1)
	sw	a4,0(a5)
	li	a5,8
.L32:
	SFPLOAD	L0, 0, 3, 0
	SFPADD	L10, L0, L3, L0, 0
	SFPNOP
	SFPSETCC	0, L0, 0
	SFPLOADI	L0, 0, 0
	SFPENCC	3, 10
	SFPADD	L10, L0, L2, L0, 0
	SFPNOP
	SFPSETCC	0, L0, 4
	SFPLOADI	L0, 0, 0
	SFPENCC	3, 10
	SFPADD	L10, L0, L1, L0, 0
	SFPNOP
	SFPSTORE	L0, 0, 3, 0
 #APP
# 392 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	addi	a5,a5,-1
	bne	a5,zero,.L32
	ret
	.size	_Z18calculate_hardtanhILb0EEvmmm, .-_Z18calculate_hardtanhILb0EEvmmm
	.section	.text._Z15calculate_lreluILb0EEvm,"axG",@progbits,_Z15calculate_lreluILb0EEvm,comdat
	.align	1
	.weak	_Z15calculate_lreluILb0EEvm
	.type	_Z15calculate_lreluILb0EEvm, @function
_Z15calculate_lreluILb0EEvm:
	lui	a5,%hi(_ZN7ckernel13instrn_bufferE)
	lw	a5,%lo(_ZN7ckernel13instrn_bufferE)(a5)
	slli	a0,a0,16
	srli	a0,a0,16
	li	a4,1896873984
	add	a4, a4, a0
# Op(0x71) - d(1)
	sw	a4,0(a5)
	li	a5,8
.L35:
	SFPLOAD	L0, 0, 3, 0
	SFPSETCC	0, L0, 0
	SFPMUL	L0, L1, L9, L0, 0
	SFPENCC	3, 10
	SFPSTORE	L0, 0, 3, 0
 #APP
# 582 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	addi	a5,a5,-1
	bne	a5,zero,.L35
	ret
	.size	_Z15calculate_lreluILb0EEvm, .-_Z15calculate_lreluILb0EEvm
	.section	.text._Z25calculate_tanh_derivativeILb0ELi0EEvv,"axG",@progbits,_Z25calculate_tanh_derivativeILb0ELi0EEvv,comdat
	.align	1
	.weak	_Z25calculate_tanh_derivativeILb0ELi0EEvv
	.type	_Z25calculate_tanh_derivativeILb0ELi0EEvv, @function
_Z25calculate_tanh_derivativeILb0ELi0EEvv:
	SFPLOAD	L3, 0, 3, 0
	SFPLUT	L3, 4
	SFPNOP
	SFPMOV	L3, L4, 1
	SFPMAD	L3, L4, L10, L3, 0
	SFPNOP
	SFPSTORE	L3, 0, 3, 0
 #APP
# 417 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	SFPLOAD	L3, 0, 3, 0
	SFPLUT	L3, 4
	SFPNOP
	SFPMOV	L3, L4, 1
	SFPMAD	L3, L4, L10, L3, 0
	SFPNOP
	SFPSTORE	L3, 0, 3, 0
 #APP
# 417 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	SFPLOAD	L3, 0, 3, 0
	SFPLUT	L3, 4
	SFPNOP
	SFPMOV	L3, L4, 1
	SFPMAD	L3, L4, L10, L3, 0
	SFPNOP
	SFPSTORE	L3, 0, 3, 0
 #APP
# 417 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	SFPLOAD	L3, 0, 3, 0
	SFPLUT	L3, 4
	SFPNOP
	SFPMOV	L3, L4, 1
	SFPMAD	L3, L4, L10, L3, 0
	SFPNOP
	SFPSTORE	L3, 0, 3, 0
 #APP
# 417 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	SFPLOAD	L3, 0, 3, 0
	SFPLUT	L3, 4
	SFPNOP
	SFPMOV	L3, L4, 1
	SFPMAD	L3, L4, L10, L3, 0
	SFPNOP
	SFPSTORE	L3, 0, 3, 0
 #APP
# 417 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	SFPLOAD	L3, 0, 3, 0
	SFPLUT	L3, 4
	SFPNOP
	SFPMOV	L3, L4, 1
	SFPMAD	L3, L4, L10, L3, 0
	SFPNOP
	SFPSTORE	L3, 0, 3, 0
 #APP
# 417 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	SFPLOAD	L3, 0, 3, 0
	SFPLUT	L3, 4
	SFPNOP
	SFPMOV	L3, L4, 1
	SFPMAD	L3, L4, L10, L3, 0
	SFPNOP
	SFPSTORE	L3, 0, 3, 0
 #APP
# 417 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	SFPLOAD	L3, 0, 3, 0
	SFPLUT	L3, 4
	SFPNOP
	SFPMOV	L3, L4, 1
	SFPMAD	L3, L4, L10, L3, 0
	SFPNOP
	SFPSTORE	L3, 0, 3, 0
 #APP
# 417 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	ret
	.size	_Z25calculate_tanh_derivativeILb0ELi0EEvv, .-_Z25calculate_tanh_derivativeILb0ELi0EEvv
	.section	.text._Z25calculate_tanh_derivativeILb0ELi1EEvv,"axG",@progbits,_Z25calculate_tanh_derivativeILb0ELi1EEvv,comdat
	.align	1
	.weak	_Z25calculate_tanh_derivativeILb0ELi1EEvv
	.type	_Z25calculate_tanh_derivativeILb0ELi1EEvv, @function
_Z25calculate_tanh_derivativeILb0ELi1EEvv:
	SFPLOAD	L3, 0, 3, 0
	SFPMOV	L3, L4, 1
	SFPMAD	L3, L4, L10, L3, 0
	SFPNOP
	SFPSTORE	L3, 0, 3, 0
 #APP
# 417 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	SFPLOAD	L3, 0, 3, 0
	SFPMOV	L3, L4, 1
	SFPMAD	L3, L4, L10, L3, 0
	SFPNOP
	SFPSTORE	L3, 0, 3, 0
 #APP
# 417 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	SFPLOAD	L3, 0, 3, 0
	SFPMOV	L3, L4, 1
	SFPMAD	L3, L4, L10, L3, 0
	SFPNOP
	SFPSTORE	L3, 0, 3, 0
 #APP
# 417 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	SFPLOAD	L3, 0, 3, 0
	SFPMOV	L3, L4, 1
	SFPMAD	L3, L4, L10, L3, 0
	SFPNOP
	SFPSTORE	L3, 0, 3, 0
 #APP
# 417 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	SFPLOAD	L3, 0, 3, 0
	SFPMOV	L3, L4, 1
	SFPMAD	L3, L4, L10, L3, 0
	SFPNOP
	SFPSTORE	L3, 0, 3, 0
 #APP
# 417 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	SFPLOAD	L3, 0, 3, 0
	SFPMOV	L3, L4, 1
	SFPMAD	L3, L4, L10, L3, 0
	SFPNOP
	SFPSTORE	L3, 0, 3, 0
 #APP
# 417 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	SFPLOAD	L3, 0, 3, 0
	SFPMOV	L3, L4, 1
	SFPMAD	L3, L4, L10, L3, 0
	SFPNOP
	SFPSTORE	L3, 0, 3, 0
 #APP
# 417 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	SFPLOAD	L3, 0, 3, 0
	SFPMOV	L3, L4, 1
	SFPMAD	L3, L4, L10, L3, 0
	SFPNOP
	SFPSTORE	L3, 0, 3, 0
 #APP
# 417 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	ret
	.size	_Z25calculate_tanh_derivativeILb0ELi1EEvv, .-_Z25calculate_tanh_derivativeILb0ELi1EEvv
	.section	.text._Z15calculate_powerILb1EEvm,"axG",@progbits,_Z15calculate_powerILb1EEvm,comdat
	.align	1
	.weak	_Z15calculate_powerILb1EEvm
	.type	_Z15calculate_powerILb1EEvm, @function
_Z15calculate_powerILb1EEvm:
	li	a4,8
	li	a3,2
.L42:
	SFPLOAD	L1, 0, 3, 0
	SFPMUL	L1, L1, L9, L0, 0
	SFPNOP
	bleu	a0,a3,.L40
	li	a5,2
.L41:
	SFPMUL	L0, L1, L9, L0, 0
	SFPNOP
	addi	a5,a5,1
	bne	a0,a5,.L41
.L40:
	SFPSTORE	L0, 0, 3, 0
 #APP
# 599 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	addi	a4,a4,-1
	bne	a4,zero,.L42
	ret
	.size	_Z15calculate_powerILb1EEvm, .-_Z15calculate_powerILb1EEvm
	.section	.text._Z14calculate_geluILb0EEvv,"axG",@progbits,_Z14calculate_geluILb0EEvv,comdat
	.align	1
	.weak	_Z14calculate_geluILb0EEvv
	.type	_Z14calculate_geluILb0EEvv, @function
_Z14calculate_geluILb0EEvv:
	SFPLOAD	L4, 0, 3, 0
	SFPMUL	L4, L4, L9, L3, 0
	SFPLOADI	L5, 0, 15671
	SFPMUL	L4, L5, L9, L5, 0
	SFPNOP
	SFPMAD	L3, L5, L4, L3, 0
	SFPNOP
	SFPMULI	16204, L3, 0
	SFPMULI	16128, L4, 0
	SFPLUT	L3, 4
	SFPNOP
	SFPMAD	L4, L3, L4, L4, 0
	SFPNOP
	SFPSTORE	L4, 0, 3, 0
 #APP
# 313 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	SFPLOAD	L4, 0, 3, 0
	SFPMUL	L4, L4, L9, L3, 0
	SFPLOADI	L5, 0, 15671
	SFPMUL	L4, L5, L9, L5, 0
	SFPNOP
	SFPMAD	L3, L5, L4, L3, 0
	SFPNOP
	SFPMULI	16204, L3, 0
	SFPMULI	16128, L4, 0
	SFPLUT	L3, 4
	SFPNOP
	SFPMAD	L4, L3, L4, L4, 0
	SFPNOP
	SFPSTORE	L4, 0, 3, 0
 #APP
# 313 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	SFPLOAD	L4, 0, 3, 0
	SFPMUL	L4, L4, L9, L3, 0
	SFPLOADI	L5, 0, 15671
	SFPMUL	L4, L5, L9, L5, 0
	SFPNOP
	SFPMAD	L3, L5, L4, L3, 0
	SFPNOP
	SFPMULI	16204, L3, 0
	SFPMULI	16128, L4, 0
	SFPLUT	L3, 4
	SFPNOP
	SFPMAD	L4, L3, L4, L4, 0
	SFPNOP
	SFPSTORE	L4, 0, 3, 0
 #APP
# 313 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	SFPLOAD	L4, 0, 3, 0
	SFPMUL	L4, L4, L9, L3, 0
	SFPLOADI	L5, 0, 15671
	SFPMUL	L4, L5, L9, L5, 0
	SFPNOP
	SFPMAD	L3, L5, L4, L3, 0
	SFPNOP
	SFPMULI	16204, L3, 0
	SFPMULI	16128, L4, 0
	SFPLUT	L3, 4
	SFPNOP
	SFPMAD	L4, L3, L4, L4, 0
	SFPNOP
	SFPSTORE	L4, 0, 3, 0
 #APP
# 313 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	SFPLOAD	L4, 0, 3, 0
	SFPMUL	L4, L4, L9, L3, 0
	SFPLOADI	L5, 0, 15671
	SFPMUL	L4, L5, L9, L5, 0
	SFPNOP
	SFPMAD	L3, L5, L4, L3, 0
	SFPNOP
	SFPMULI	16204, L3, 0
	SFPMULI	16128, L4, 0
	SFPLUT	L3, 4
	SFPNOP
	SFPMAD	L4, L3, L4, L4, 0
	SFPNOP
	SFPSTORE	L4, 0, 3, 0
 #APP
# 313 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	SFPLOAD	L4, 0, 3, 0
	SFPMUL	L4, L4, L9, L3, 0
	SFPLOADI	L5, 0, 15671
	SFPMUL	L4, L5, L9, L5, 0
	SFPNOP
	SFPMAD	L3, L5, L4, L3, 0
	SFPNOP
	SFPMULI	16204, L3, 0
	SFPMULI	16128, L4, 0
	SFPLUT	L3, 4
	SFPNOP
	SFPMAD	L4, L3, L4, L4, 0
	SFPNOP
	SFPSTORE	L4, 0, 3, 0
 #APP
# 313 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	SFPLOAD	L4, 0, 3, 0
	SFPMUL	L4, L4, L9, L3, 0
	SFPLOADI	L5, 0, 15671
	SFPMUL	L4, L5, L9, L5, 0
	SFPNOP
	SFPMAD	L3, L5, L4, L3, 0
	SFPNOP
	SFPMULI	16204, L3, 0
	SFPMULI	16128, L4, 0
	SFPLUT	L3, 4
	SFPNOP
	SFPMAD	L4, L3, L4, L4, 0
	SFPNOP
	SFPSTORE	L4, 0, 3, 0
 #APP
# 313 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	SFPLOAD	L4, 0, 3, 0
	SFPMUL	L4, L4, L9, L3, 0
	SFPLOADI	L5, 0, 15671
	SFPMUL	L4, L5, L9, L5, 0
	SFPNOP
	SFPMAD	L3, L5, L4, L3, 0
	SFPNOP
	SFPMULI	16204, L3, 0
	SFPMULI	16128, L4, 0
	SFPLUT	L3, 4
	SFPNOP
	SFPMAD	L4, L3, L4, L4, 0
	SFPNOP
	SFPSTORE	L4, 0, 3, 0
 #APP
# 313 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	ret
	.size	_Z14calculate_geluILb0EEvv, .-_Z14calculate_geluILb0EEvv
	.section	.text._Z14calculate_geluILb1EEvv,"axG",@progbits,_Z14calculate_geluILb1EEvv,comdat
	.align	1
	.weak	_Z14calculate_geluILb1EEvv
	.type	_Z14calculate_geluILb1EEvv, @function
_Z14calculate_geluILb1EEvv:
	SFPLOADI	L4, 0, 16128
	SFPLOAD	L3, 0, 3, 0
	SFPMUL	L3, L4, L9, L5, 0
	SFPLUT	L3, 4
	SFPNOP
	SFPMAD	L5, L3, L5, L5, 0
	SFPNOP
	SFPSTORE	L5, 0, 3, 0
 #APP
# 313 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	SFPLOAD	L3, 0, 3, 0
	SFPMUL	L3, L4, L9, L5, 0
	SFPLUT	L3, 4
	SFPNOP
	SFPMAD	L5, L3, L5, L5, 0
	SFPNOP
	SFPSTORE	L5, 0, 3, 0
 #APP
# 313 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	SFPLOAD	L3, 0, 3, 0
	SFPMUL	L3, L4, L9, L5, 0
	SFPLUT	L3, 4
	SFPNOP
	SFPMAD	L5, L3, L5, L5, 0
	SFPNOP
	SFPSTORE	L5, 0, 3, 0
 #APP
# 313 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	SFPLOAD	L3, 0, 3, 0
	SFPMUL	L3, L4, L9, L5, 0
	SFPLUT	L3, 4
	SFPNOP
	SFPMAD	L5, L3, L5, L5, 0
	SFPNOP
	SFPSTORE	L5, 0, 3, 0
 #APP
# 313 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	SFPLOAD	L3, 0, 3, 0
	SFPMUL	L3, L4, L9, L5, 0
	SFPLUT	L3, 4
	SFPNOP
	SFPMAD	L5, L3, L5, L5, 0
	SFPNOP
	SFPSTORE	L5, 0, 3, 0
 #APP
# 313 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	SFPLOAD	L3, 0, 3, 0
	SFPMUL	L3, L4, L9, L5, 0
	SFPLUT	L3, 4
	SFPNOP
	SFPMAD	L5, L3, L5, L5, 0
	SFPNOP
	SFPSTORE	L5, 0, 3, 0
 #APP
# 313 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	SFPLOAD	L3, 0, 3, 0
	SFPMUL	L3, L4, L9, L5, 0
	SFPLUT	L3, 4
	SFPNOP
	SFPMAD	L5, L3, L5, L5, 0
	SFPNOP
	SFPSTORE	L5, 0, 3, 0
 #APP
# 313 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	SFPLOAD	L3, 0, 3, 0
	SFPMUL	L3, L4, L9, L4, 0
	SFPLUT	L3, 4
	SFPNOP
	SFPMAD	L4, L3, L4, L4, 0
	SFPNOP
	SFPSTORE	L4, 0, 3, 0
 #APP
# 313 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	ret
	.size	_Z14calculate_geluILb1EEvv, .-_Z14calculate_geluILb1EEvv
	.section	.text._Z25calculate_gelu_derivativeILb0EEvv,"axG",@progbits,_Z25calculate_gelu_derivativeILb0EEvv,comdat
	.align	1
	.weak	_Z25calculate_gelu_derivativeILb0EEvv
	.type	_Z25calculate_gelu_derivativeILb0EEvv, @function
_Z25calculate_gelu_derivativeILb0EEvv:
	li	a5,8
.L48:
	SFPLOAD	L3, 0, 3, 0
	SFPMUL	L3, L3, L9, L4, 0
	SFPNOP
	SFPMULI	48896, L4, 0
	SFPNOP
	SFPSETSGN	0, L4, L2, 1
	SFPEXEXP	L2, L5, 10
	SFPSETEXP	126, L2, L2, 1
	SFPENCC	3, 10
	SFPLOADI	L6, 0, 16220
	SFPMAD	L2, L8, L6, L6, 0
	SFPNOP
	SFPMAD	L2, L6, L10, L2, 0
	SFPSETCC	0, L5, 4
	SFPMUL	L2, L2, L9, L2, 0
	SFPIADD	-1, L5, L5, 9
	SFPMUL	L2, L2, L9, L2, 0
	SFPIADD	-1, L5, L5, 9
	SFPMUL	L2, L2, L9, L2, 0
	SFPIADD	-1, L5, L5, 9
	SFPMUL	L2, L2, L9, L2, 0
	SFPIADD	-1, L5, L5, 9
	SFPMUL	L2, L2, L9, L2, 0
	SFPIADD	-1, L5, L5, 9
	SFPMUL	L2, L2, L9, L2, 0
	SFPIADD	-1, L5, L5, 9
	SFPMUL	L2, L2, L9, L2, 0
	SFPIADD	-1, L5, L5, 9
	SFPMUL	L2, L2, L9, L2, 0
	SFPENCC	3, 10
	SFPSETCC	0, L4, 0
	SFPSETSGN	1, L2, L5, 1
	SFPSETEXP	126, L5, L5, 1
	SFPMAD	L5, L12, L13, L4, 0
	SFPNOP
	SFPMUL	L12, L4, L9, L4, 0
	SFPNOP
	SFPMAD	L5, L4, L13, L6, 0
	SFPNOP
	SFPMUL	L4, L6, L9, L4, 0
	SFPNOP
	SFPMAD	L5, L4, L13, L5, 0
	SFPNOP
	SFPMUL	L4, L5, L9, L4, 0
	SFPEXEXP	L2, L5, 0
	SFPEXEXP	L4, L6, 0
	SFPIADD	0, L6, L5, 6
	SFPPUSHC	0
	SFPIADD	126, L5, L5, 1
	SFPLOADI	L4, 0, 0
	SFPLOADI	L5, 4, 0
	SFPPOPC	0
	SFPSETEXP	0, L4, L5, 0
	SFPMOV	L5, L2, 0
	SFPENCC	3, 10
	SFPMUL	L2, L3, L9, L4, 0
	SFPNOP
	SFPMULI	16076, L4, 0
	SFPLOADI	L2, 2, 65296
	SFPLUT	L3, 4
	SFPNOP
	SFPADD	L10, L4, L3, L4, 0
	SFPNOP
	SFPADDI	16128, L4, 0
	SFPNOP
	SFPSTORE	L4, 0, 3, 0
 #APP
# 450 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	addi	a5,a5,-1
	bne	a5,zero,.L48
	ret
	.size	_Z25calculate_gelu_derivativeILb0EEvv, .-_Z25calculate_gelu_derivativeILb0EEvv
	.section	.text._Z14calculate_sqrtILb0ELi4ELi2EEvv,"axG",@progbits,_Z14calculate_sqrtILb0ELi4ELi2EEvv,comdat
	.align	1
	.weak	_Z14calculate_sqrtILb0ELi4ELi2EEvv
	.type	_Z14calculate_sqrtILb0ELi4ELi2EEvv, @function
_Z14calculate_sqrtILb0ELi4ELi2EEvv:
	SFPLOAD	L3, 0, 3, 0
	SFPSETCC	0, L3, 2
	SFPMOV	L3, L1, 0
	SFPSHFT	-1, L0, L1, 1
	SFPIADD	0, L12, L1, 6
	SFPMUL	L1, L1, L9, L2, 0
	SFPLOADI	L0, 0, 48896
	SFPMUL	L3, L0, L9, L0, 0
	SFPLOADI	L4, 0, 16320
	SFPMAD	L2, L0, L4, L0, 0
	SFPNOP
	SFPMUL	L0, L1, L9, L1, 0
	SFPNOP
	SFPMUL	L1, L1, L9, L0, 0
	SFPLOADI	L2, 0, 48896
	SFPMUL	L3, L2, L9, L2, 0
	SFPLOADI	L4, 0, 16320
	SFPMAD	L0, L2, L4, L0, 0
	SFPNOP
	SFPMUL	L0, L1, L9, L0, 0
	SFPNOP
	SFPMUL	L0, L3, L9, L0, 0
	SFPNOP
	SFPSTORE	L0, 0, 3, 0
	SFPENCC	3, 10
 #APP
# 517 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	SFPLOAD	L3, 0, 3, 0
	SFPSETCC	0, L3, 2
	SFPMOV	L3, L1, 0
	SFPSHFT	-1, L0, L1, 1
	SFPIADD	0, L12, L1, 6
	SFPMUL	L1, L1, L9, L2, 0
	SFPLOADI	L0, 0, 48896
	SFPMUL	L3, L0, L9, L0, 0
	SFPLOADI	L4, 0, 16320
	SFPMAD	L2, L0, L4, L0, 0
	SFPNOP
	SFPMUL	L0, L1, L9, L1, 0
	SFPNOP
	SFPMUL	L1, L1, L9, L0, 0
	SFPLOADI	L2, 0, 48896
	SFPMUL	L3, L2, L9, L2, 0
	SFPLOADI	L4, 0, 16320
	SFPMAD	L0, L2, L4, L0, 0
	SFPNOP
	SFPMUL	L0, L1, L9, L0, 0
	SFPNOP
	SFPMUL	L0, L3, L9, L0, 0
	SFPNOP
	SFPSTORE	L0, 0, 3, 0
	SFPENCC	3, 10
 #APP
# 517 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	SFPLOAD	L3, 0, 3, 0
	SFPSETCC	0, L3, 2
	SFPMOV	L3, L1, 0
	SFPSHFT	-1, L0, L1, 1
	SFPIADD	0, L12, L1, 6
	SFPMUL	L1, L1, L9, L2, 0
	SFPLOADI	L0, 0, 48896
	SFPMUL	L3, L0, L9, L0, 0
	SFPLOADI	L4, 0, 16320
	SFPMAD	L2, L0, L4, L0, 0
	SFPNOP
	SFPMUL	L0, L1, L9, L1, 0
	SFPNOP
	SFPMUL	L1, L1, L9, L0, 0
	SFPLOADI	L2, 0, 48896
	SFPMUL	L3, L2, L9, L2, 0
	SFPLOADI	L4, 0, 16320
	SFPMAD	L0, L2, L4, L0, 0
	SFPNOP
	SFPMUL	L0, L1, L9, L0, 0
	SFPNOP
	SFPMUL	L0, L3, L9, L0, 0
	SFPNOP
	SFPSTORE	L0, 0, 3, 0
	SFPENCC	3, 10
 #APP
# 517 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	SFPLOAD	L3, 0, 3, 0
	SFPSETCC	0, L3, 2
	SFPMOV	L3, L1, 0
	SFPSHFT	-1, L0, L1, 1
	SFPIADD	0, L12, L1, 6
	SFPMUL	L1, L1, L9, L2, 0
	SFPLOADI	L0, 0, 48896
	SFPMUL	L3, L0, L9, L0, 0
	SFPLOADI	L4, 0, 16320
	SFPMAD	L2, L0, L4, L0, 0
	SFPNOP
	SFPMUL	L0, L1, L9, L1, 0
	SFPNOP
	SFPMUL	L1, L1, L9, L0, 0
	SFPLOADI	L2, 0, 48896
	SFPMUL	L3, L2, L9, L2, 0
	SFPLOADI	L4, 0, 16320
	SFPMAD	L0, L2, L4, L0, 0
	SFPNOP
	SFPMUL	L0, L1, L9, L0, 0
	SFPNOP
	SFPMUL	L0, L3, L9, L0, 0
	SFPNOP
	SFPSTORE	L0, 0, 3, 0
	SFPENCC	3, 10
 #APP
# 517 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	SFPLOAD	L3, 0, 3, 0
	SFPSETCC	0, L3, 2
	SFPMOV	L3, L1, 0
	SFPSHFT	-1, L0, L1, 1
	SFPIADD	0, L12, L1, 6
	SFPMUL	L1, L1, L9, L2, 0
	SFPLOADI	L0, 0, 48896
	SFPMUL	L3, L0, L9, L0, 0
	SFPLOADI	L4, 0, 16320
	SFPMAD	L2, L0, L4, L0, 0
	SFPNOP
	SFPMUL	L0, L1, L9, L1, 0
	SFPNOP
	SFPMUL	L1, L1, L9, L0, 0
	SFPLOADI	L2, 0, 48896
	SFPMUL	L3, L2, L9, L2, 0
	SFPLOADI	L4, 0, 16320
	SFPMAD	L0, L2, L4, L0, 0
	SFPNOP
	SFPMUL	L0, L1, L9, L0, 0
	SFPNOP
	SFPMUL	L0, L3, L9, L0, 0
	SFPNOP
	SFPSTORE	L0, 0, 3, 0
	SFPENCC	3, 10
 #APP
# 517 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	SFPLOAD	L3, 0, 3, 0
	SFPSETCC	0, L3, 2
	SFPMOV	L3, L1, 0
	SFPSHFT	-1, L0, L1, 1
	SFPIADD	0, L12, L1, 6
	SFPMUL	L1, L1, L9, L2, 0
	SFPLOADI	L0, 0, 48896
	SFPMUL	L3, L0, L9, L0, 0
	SFPLOADI	L4, 0, 16320
	SFPMAD	L2, L0, L4, L0, 0
	SFPNOP
	SFPMUL	L0, L1, L9, L1, 0
	SFPNOP
	SFPMUL	L1, L1, L9, L0, 0
	SFPLOADI	L2, 0, 48896
	SFPMUL	L3, L2, L9, L2, 0
	SFPLOADI	L4, 0, 16320
	SFPMAD	L0, L2, L4, L0, 0
	SFPNOP
	SFPMUL	L0, L1, L9, L0, 0
	SFPNOP
	SFPMUL	L0, L3, L9, L0, 0
	SFPNOP
	SFPSTORE	L0, 0, 3, 0
	SFPENCC	3, 10
 #APP
# 517 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	SFPLOAD	L3, 0, 3, 0
	SFPSETCC	0, L3, 2
	SFPMOV	L3, L1, 0
	SFPSHFT	-1, L0, L1, 1
	SFPIADD	0, L12, L1, 6
	SFPMUL	L1, L1, L9, L2, 0
	SFPLOADI	L0, 0, 48896
	SFPMUL	L3, L0, L9, L0, 0
	SFPLOADI	L4, 0, 16320
	SFPMAD	L2, L0, L4, L0, 0
	SFPNOP
	SFPMUL	L0, L1, L9, L1, 0
	SFPNOP
	SFPMUL	L1, L1, L9, L0, 0
	SFPLOADI	L2, 0, 48896
	SFPMUL	L3, L2, L9, L2, 0
	SFPLOADI	L4, 0, 16320
	SFPMAD	L0, L2, L4, L0, 0
	SFPNOP
	SFPMUL	L0, L1, L9, L0, 0
	SFPNOP
	SFPMUL	L0, L3, L9, L0, 0
	SFPNOP
	SFPSTORE	L0, 0, 3, 0
	SFPENCC	3, 10
 #APP
# 517 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	SFPLOAD	L3, 0, 3, 0
	SFPSETCC	0, L3, 2
	SFPMOV	L3, L1, 0
	SFPSHFT	-1, L0, L1, 1
	SFPIADD	0, L12, L1, 6
	SFPMOV	L1, L2, 2
	SFPMUL	L1, L1, L9, L1, 0
	SFPLOADI	L0, 0, 48896
	SFPMUL	L3, L0, L9, L0, 0
	SFPLOADI	L4, 0, 16320
	SFPMAD	L1, L0, L4, L0, 0
	SFPNOP
	SFPMUL	L0, L2, L9, L1, 0
	SFPNOP
	SFPMUL	L1, L1, L9, L0, 0
	SFPLOADI	L2, 0, 48896
	SFPMUL	L3, L2, L9, L2, 0
	SFPLOADI	L4, 0, 16320
	SFPMAD	L0, L2, L4, L0, 0
	SFPNOP
	SFPMUL	L0, L1, L9, L0, 0
	SFPNOP
	SFPMUL	L0, L3, L9, L0, 0
	SFPNOP
	SFPSTORE	L0, 0, 3, 0
	SFPENCC	3, 10
 #APP
# 517 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	ret
	.size	_Z14calculate_sqrtILb0ELi4ELi2EEvv, .-_Z14calculate_sqrtILb0ELi4ELi2EEvv
	.section	.text._Z14calculate_sqrtILb1ELi4ELi2EEvv,"axG",@progbits,_Z14calculate_sqrtILb1ELi4ELi2EEvv,comdat
	.align	1
	.weak	_Z14calculate_sqrtILb1ELi4ELi2EEvv
	.type	_Z14calculate_sqrtILb1ELi4ELi2EEvv, @function
_Z14calculate_sqrtILb1ELi4ELi2EEvv:
	SFPLOAD	L0, 0, 3, 0
	SFPIADD	0, L12, L0, 4
	SFPSHFT	-1, L0, L0, 1
	SFPSTORE	L0, 0, 3, 0
 #APP
# 517 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	SFPLOAD	L0, 0, 3, 0
	SFPIADD	0, L12, L0, 4
	SFPSHFT	-1, L0, L0, 1
	SFPSTORE	L0, 0, 3, 0
 #APP
# 517 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	SFPLOAD	L0, 0, 3, 0
	SFPIADD	0, L12, L0, 4
	SFPSHFT	-1, L0, L0, 1
	SFPSTORE	L0, 0, 3, 0
 #APP
# 517 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	SFPLOAD	L0, 0, 3, 0
	SFPIADD	0, L12, L0, 4
	SFPSHFT	-1, L0, L0, 1
	SFPSTORE	L0, 0, 3, 0
 #APP
# 517 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	SFPLOAD	L0, 0, 3, 0
	SFPIADD	0, L12, L0, 4
	SFPSHFT	-1, L0, L0, 1
	SFPSTORE	L0, 0, 3, 0
 #APP
# 517 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	SFPLOAD	L0, 0, 3, 0
	SFPIADD	0, L12, L0, 4
	SFPSHFT	-1, L0, L0, 1
	SFPSTORE	L0, 0, 3, 0
 #APP
# 517 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	SFPLOAD	L0, 0, 3, 0
	SFPIADD	0, L12, L0, 4
	SFPSHFT	-1, L0, L0, 1
	SFPSTORE	L0, 0, 3, 0
 #APP
# 517 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	SFPLOAD	L0, 0, 3, 0
	SFPIADD	0, L12, L0, 4
	SFPSHFT	-1, L0, L0, 1
	SFPSTORE	L0, 0, 3, 0
 #APP
# 517 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	ret
	.size	_Z14calculate_sqrtILb1ELi4ELi2EEvv, .-_Z14calculate_sqrtILb1ELi4ELi2EEvv
	.section	.text._Z21calculate_exponentialILb0ELb0ELb1EEvt,"axG",@progbits,_Z21calculate_exponentialILb0ELb0ELb1EEvt,comdat
	.align	1
	.weak	_Z21calculate_exponentialILb0ELb0ELb1EEvt
	.type	_Z21calculate_exponentialILb0ELb0ELb1EEvt, @function
_Z21calculate_exponentialILb0ELb0ELb1EEvt:
	li	a5,8
	lui	a4,%hi(_ZN7ckernel13instrn_bufferE)
.L53:
	SFPLOAD	L1, 0, 3, 0
	lw	a3,%lo(_ZN7ckernel13instrn_bufferE)(a4)
	li	a2,1895890944
	add	a2, a2, a0
# Op(0x71) - d(0)
	sw	a2,0(a3)
	SFPMUL	L1, L0, L9, L1, 0
	SFPNOP
	SFPSETSGN	0, L1, L0, 1
	SFPEXEXP	L0, L2, 10
	SFPSETEXP	126, L0, L0, 1
	SFPENCC	3, 10
	SFPLOADI	L3, 0, 16220
	SFPMAD	L0, L8, L3, L3, 0
	SFPNOP
	SFPMAD	L0, L3, L10, L0, 0
	SFPSETCC	0, L2, 4
	SFPMUL	L0, L0, L9, L0, 0
	SFPIADD	-1, L2, L2, 9
	SFPMUL	L0, L0, L9, L0, 0
	SFPIADD	-1, L2, L2, 9
	SFPMUL	L0, L0, L9, L0, 0
	SFPIADD	-1, L2, L2, 9
	SFPMUL	L0, L0, L9, L0, 0
	SFPIADD	-1, L2, L2, 9
	SFPMUL	L0, L0, L9, L0, 0
	SFPIADD	-1, L2, L2, 9
	SFPMUL	L0, L0, L9, L0, 0
	SFPIADD	-1, L2, L2, 9
	SFPMUL	L0, L0, L9, L0, 0
	SFPIADD	-1, L2, L2, 9
	SFPMUL	L0, L0, L9, L0, 0
	SFPENCC	3, 10
	SFPSETCC	0, L1, 0
	SFPSETSGN	1, L0, L2, 1
	SFPSETEXP	126, L2, L2, 1
	SFPMAD	L2, L12, L13, L1, 0
	SFPNOP
	SFPMUL	L12, L1, L9, L1, 0
	SFPNOP
	SFPMAD	L2, L1, L13, L3, 0
	SFPNOP
	SFPMUL	L1, L3, L9, L1, 0
	SFPNOP
	SFPMAD	L2, L1, L13, L2, 0
	SFPNOP
	SFPMUL	L1, L2, L9, L1, 0
	SFPEXEXP	L0, L2, 0
	SFPEXEXP	L1, L3, 0
	SFPIADD	0, L3, L2, 6
	SFPPUSHC	0
	SFPIADD	126, L2, L2, 1
	SFPLOADI	L1, 0, 0
	SFPLOADI	L2, 4, 0
	SFPPOPC	0
	SFPSETEXP	0, L1, L2, 0
	SFPMOV	L2, L0, 0
	SFPENCC	3, 10
	SFPSTORE	L0, 0, 3, 0
 #APP
# 268 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	addi	a5,a5,-1
	bne	a5,zero,.L53
	ret
	.size	_Z21calculate_exponentialILb0ELb0ELb1EEvt, .-_Z21calculate_exponentialILb0ELb0ELb1EEvt
	.section	.text._Z21calculate_exponentialILb0ELb1ELb1EEvt,"axG",@progbits,_Z21calculate_exponentialILb0ELb1ELb1EEvt,comdat
	.align	1
	.weak	_Z21calculate_exponentialILb0ELb1ELb1EEvt
	.type	_Z21calculate_exponentialILb0ELb1ELb1EEvt, @function
_Z21calculate_exponentialILb0ELb1ELb1EEvt:
	li	a5,8
	lui	a4,%hi(_ZN7ckernel13instrn_bufferE)
.L56:
	SFPLOAD	L1, 0, 3, 0
	lw	a3,%lo(_ZN7ckernel13instrn_bufferE)(a4)
	li	a2,1895890944
	add	a2, a2, a0
# Op(0x71) - d(0)
	sw	a2,0(a3)
	SFPMUL	L1, L0, L9, L1, 0
	SFPNOP
	SFPSETSGN	0, L1, L0, 1
	SFPEXEXP	L0, L2, 10
	SFPSETEXP	126, L0, L0, 1
	SFPENCC	3, 10
	SFPLOADI	L3, 0, 16220
	SFPMAD	L0, L8, L3, L3, 0
	SFPNOP
	SFPMAD	L0, L3, L10, L0, 0
	SFPSETCC	0, L2, 4
	SFPMUL	L0, L0, L9, L0, 0
	SFPIADD	-1, L2, L2, 9
	SFPMUL	L0, L0, L9, L0, 0
	SFPIADD	-1, L2, L2, 9
	SFPMUL	L0, L0, L9, L0, 0
	SFPIADD	-1, L2, L2, 9
	SFPMUL	L0, L0, L9, L0, 0
	SFPIADD	-1, L2, L2, 9
	SFPMUL	L0, L0, L9, L0, 0
	SFPIADD	-1, L2, L2, 9
	SFPMUL	L0, L0, L9, L0, 0
	SFPIADD	-1, L2, L2, 9
	SFPMUL	L0, L0, L9, L0, 0
	SFPIADD	-1, L2, L2, 9
	SFPMUL	L0, L0, L9, L0, 0
	SFPENCC	3, 10
	SFPSETCC	0, L1, 0
	SFPSETSGN	1, L0, L2, 1
	SFPSETEXP	126, L2, L2, 1
	SFPMAD	L2, L12, L13, L1, 0
	SFPNOP
	SFPMUL	L12, L1, L9, L1, 0
	SFPNOP
	SFPMAD	L2, L1, L13, L3, 0
	SFPNOP
	SFPMUL	L1, L3, L9, L1, 0
	SFPNOP
	SFPMAD	L2, L1, L13, L2, 0
	SFPNOP
	SFPMUL	L1, L2, L9, L1, 0
	SFPEXEXP	L0, L2, 0
	SFPEXEXP	L1, L3, 0
	SFPIADD	0, L3, L2, 6
	SFPPUSHC	0
	SFPIADD	126, L2, L2, 1
	SFPLOADI	L1, 0, 0
	SFPLOADI	L2, 4, 0
	SFPPOPC	0
	SFPSETEXP	0, L1, L2, 0
	SFPMOV	L2, L0, 0
	SFPENCC	3, 10
	SFPSTORE	L0, 0, 3, 0
 #APP
# 268 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	addi	a5,a5,-1
	bne	a5,zero,.L56
	ret
	.size	_Z21calculate_exponentialILb0ELb1ELb1EEvt, .-_Z21calculate_exponentialILb0ELb1ELb1EEvt
	.section	.text._Z21calculate_exponentialILb1ELb0ELb1EEvt,"axG",@progbits,_Z21calculate_exponentialILb1ELb0ELb1EEvt,comdat
	.align	1
	.weak	_Z21calculate_exponentialILb1ELb0ELb1EEvt
	.type	_Z21calculate_exponentialILb1ELb0ELb1EEvt, @function
_Z21calculate_exponentialILb1ELb0ELb1EEvt:
	SFPLOAD	L0, 0, 3, 0
	lui	a5,%hi(_ZN7ckernel13instrn_bufferE)
	lw	a5,%lo(_ZN7ckernel13instrn_bufferE)(a5)
	li	a4,1896939520
	add	a4, a4, a0
# Op(0x71) - d(1)
	sw	a4,0(a5)
	SFPMUL	L0, L1, L9, L0, 0
	SFPNOP
	SFPMAD	L0, L12, L13, L0, 0
	SFPNOP
	SFPIADD	0, L14, L0, 4
	SFPSHFT	8, L0, L0, 1
	SFPSTORE	L0, 0, 3, 0
 #APP
# 268 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	SFPLOAD	L0, 0, 3, 0
	li	a4,1896939520
	add	a4, a4, a0
# Op(0x71) - d(1)
	sw	a4,0(a5)
	SFPMUL	L0, L1, L9, L0, 0
	SFPNOP
	SFPMAD	L0, L12, L13, L0, 0
	SFPNOP
	SFPIADD	0, L14, L0, 4
	SFPSHFT	8, L0, L0, 1
	SFPSTORE	L0, 0, 3, 0
 #APP
# 268 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	SFPLOAD	L0, 0, 3, 0
	li	a4,1896939520
	add	a4, a4, a0
# Op(0x71) - d(1)
	sw	a4,0(a5)
	SFPMUL	L0, L1, L9, L0, 0
	SFPNOP
	SFPMAD	L0, L12, L13, L0, 0
	SFPNOP
	SFPIADD	0, L14, L0, 4
	SFPSHFT	8, L0, L0, 1
	SFPSTORE	L0, 0, 3, 0
 #APP
# 268 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	SFPLOAD	L0, 0, 3, 0
	li	a4,1896939520
	add	a4, a4, a0
# Op(0x71) - d(1)
	sw	a4,0(a5)
	SFPMUL	L0, L1, L9, L0, 0
	SFPNOP
	SFPMAD	L0, L12, L13, L0, 0
	SFPNOP
	SFPIADD	0, L14, L0, 4
	SFPSHFT	8, L0, L0, 1
	SFPSTORE	L0, 0, 3, 0
 #APP
# 268 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	SFPLOAD	L0, 0, 3, 0
	li	a4,1896939520
	add	a4, a4, a0
# Op(0x71) - d(1)
	sw	a4,0(a5)
	SFPMUL	L0, L1, L9, L0, 0
	SFPNOP
	SFPMAD	L0, L12, L13, L0, 0
	SFPNOP
	SFPIADD	0, L14, L0, 4
	SFPSHFT	8, L0, L0, 1
	SFPSTORE	L0, 0, 3, 0
 #APP
# 268 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	SFPLOAD	L0, 0, 3, 0
	li	a4,1896939520
	add	a4, a4, a0
# Op(0x71) - d(1)
	sw	a4,0(a5)
	SFPMUL	L0, L1, L9, L0, 0
	SFPNOP
	SFPMAD	L0, L12, L13, L0, 0
	SFPNOP
	SFPIADD	0, L14, L0, 4
	SFPSHFT	8, L0, L0, 1
	SFPSTORE	L0, 0, 3, 0
 #APP
# 268 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	SFPLOAD	L0, 0, 3, 0
	li	a4,1896939520
	add	a4, a4, a0
# Op(0x71) - d(1)
	sw	a4,0(a5)
	SFPMUL	L0, L1, L9, L0, 0
	SFPNOP
	SFPMAD	L0, L12, L13, L0, 0
	SFPNOP
	SFPIADD	0, L14, L0, 4
	SFPSHFT	8, L0, L0, 1
	SFPSTORE	L0, 0, 3, 0
 #APP
# 268 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	SFPLOAD	L0, 0, 3, 0
	li	a4,1896939520
	add	a4, a4, a0
# Op(0x71) - d(1)
	sw	a4,0(a5)
	SFPMUL	L0, L1, L9, L0, 0
	SFPNOP
	SFPMAD	L0, L12, L13, L0, 0
	SFPNOP
	SFPIADD	0, L14, L0, 4
	SFPSHFT	8, L0, L0, 1
	SFPSTORE	L0, 0, 3, 0
 #APP
# 268 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	ret
	.size	_Z21calculate_exponentialILb1ELb0ELb1EEvt, .-_Z21calculate_exponentialILb1ELb0ELb1EEvt
	.section	.text._Z21calculate_exponentialILb1ELb1ELb1EEvt,"axG",@progbits,_Z21calculate_exponentialILb1ELb1ELb1EEvt,comdat
	.align	1
	.weak	_Z21calculate_exponentialILb1ELb1ELb1EEvt
	.type	_Z21calculate_exponentialILb1ELb1ELb1EEvt, @function
_Z21calculate_exponentialILb1ELb1ELb1EEvt:
	SFPLOAD	L0, 0, 3, 0
	lui	a5,%hi(_ZN7ckernel13instrn_bufferE)
	lw	a5,%lo(_ZN7ckernel13instrn_bufferE)(a5)
	li	a4,1896939520
	add	a4, a4, a0
# Op(0x71) - d(1)
	sw	a4,0(a5)
	SFPMUL	L0, L1, L9, L0, 0
	SFPNOP
	SFPMAD	L0, L12, L13, L0, 0
	SFPNOP
	SFPIADD	0, L14, L0, 4
	SFPSHFT	8, L0, L0, 1
	SFPSTORE	L0, 0, 3, 0
	SFPSETCC	0, L0, 0
	SFPSTORE	L9, 0, 3, 0
	SFPENCC	3, 10
 #APP
# 268 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	SFPLOAD	L0, 0, 3, 0
	li	a4,1896939520
	add	a4, a4, a0
# Op(0x71) - d(1)
	sw	a4,0(a5)
	SFPMUL	L0, L1, L9, L0, 0
	SFPNOP
	SFPMAD	L0, L12, L13, L0, 0
	SFPNOP
	SFPIADD	0, L14, L0, 4
	SFPSHFT	8, L0, L0, 1
	SFPSTORE	L0, 0, 3, 0
	SFPSETCC	0, L0, 0
	SFPSTORE	L9, 0, 3, 0
	SFPENCC	3, 10
 #APP
# 268 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	SFPLOAD	L0, 0, 3, 0
	li	a4,1896939520
	add	a4, a4, a0
# Op(0x71) - d(1)
	sw	a4,0(a5)
	SFPMUL	L0, L1, L9, L0, 0
	SFPNOP
	SFPMAD	L0, L12, L13, L0, 0
	SFPNOP
	SFPIADD	0, L14, L0, 4
	SFPSHFT	8, L0, L0, 1
	SFPSTORE	L0, 0, 3, 0
	SFPSETCC	0, L0, 0
	SFPSTORE	L9, 0, 3, 0
	SFPENCC	3, 10
 #APP
# 268 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	SFPLOAD	L0, 0, 3, 0
	li	a4,1896939520
	add	a4, a4, a0
# Op(0x71) - d(1)
	sw	a4,0(a5)
	SFPMUL	L0, L1, L9, L0, 0
	SFPNOP
	SFPMAD	L0, L12, L13, L0, 0
	SFPNOP
	SFPIADD	0, L14, L0, 4
	SFPSHFT	8, L0, L0, 1
	SFPSTORE	L0, 0, 3, 0
	SFPSETCC	0, L0, 0
	SFPSTORE	L9, 0, 3, 0
	SFPENCC	3, 10
 #APP
# 268 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	SFPLOAD	L0, 0, 3, 0
	li	a4,1896939520
	add	a4, a4, a0
# Op(0x71) - d(1)
	sw	a4,0(a5)
	SFPMUL	L0, L1, L9, L0, 0
	SFPNOP
	SFPMAD	L0, L12, L13, L0, 0
	SFPNOP
	SFPIADD	0, L14, L0, 4
	SFPSHFT	8, L0, L0, 1
	SFPSTORE	L0, 0, 3, 0
	SFPSETCC	0, L0, 0
	SFPSTORE	L9, 0, 3, 0
	SFPENCC	3, 10
 #APP
# 268 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	SFPLOAD	L0, 0, 3, 0
	li	a4,1896939520
	add	a4, a4, a0
# Op(0x71) - d(1)
	sw	a4,0(a5)
	SFPMUL	L0, L1, L9, L0, 0
	SFPNOP
	SFPMAD	L0, L12, L13, L0, 0
	SFPNOP
	SFPIADD	0, L14, L0, 4
	SFPSHFT	8, L0, L0, 1
	SFPSTORE	L0, 0, 3, 0
	SFPSETCC	0, L0, 0
	SFPSTORE	L9, 0, 3, 0
	SFPENCC	3, 10
 #APP
# 268 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	SFPLOAD	L0, 0, 3, 0
	li	a4,1896939520
	add	a4, a4, a0
# Op(0x71) - d(1)
	sw	a4,0(a5)
	SFPMUL	L0, L1, L9, L0, 0
	SFPNOP
	SFPMAD	L0, L12, L13, L0, 0
	SFPNOP
	SFPIADD	0, L14, L0, 4
	SFPSHFT	8, L0, L0, 1
	SFPSTORE	L0, 0, 3, 0
	SFPSETCC	0, L0, 0
	SFPSTORE	L9, 0, 3, 0
	SFPENCC	3, 10
 #APP
# 268 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	SFPLOAD	L0, 0, 3, 0
	li	a4,1896939520
	add	a4, a4, a0
# Op(0x71) - d(1)
	sw	a4,0(a5)
	SFPMUL	L0, L1, L9, L0, 0
	SFPNOP
	SFPMAD	L0, L12, L13, L0, 0
	SFPNOP
	SFPIADD	0, L14, L0, 4
	SFPSHFT	8, L0, L0, 1
	SFPSTORE	L0, 0, 3, 0
	SFPSETCC	0, L0, 0
	SFPSTORE	L9, 0, 3, 0
	SFPENCC	3, 10
 #APP
# 268 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	ret
	.size	_Z21calculate_exponentialILb1ELb1ELb1EEvt, .-_Z21calculate_exponentialILb1ELb1ELb1EEvt
	.section	.text._Z13calculate_logILb1ELb1EEvm,"axG",@progbits,_Z13calculate_logILb1ELb1EEvm,comdat
	.align	1
	.weak	_Z13calculate_logILb1ELb1EEvm
	.type	_Z13calculate_logILb1ELb1EEvm, @function
_Z13calculate_logILb1ELb1EEvm:
	slli	a0,a0,16
	li	a5,8
	lui	a3,%hi(_ZN7ckernel13instrn_bufferE)
	srli	a0,a0,16
.L61:
	SFPLOAD	L2, 0, 3, 0
	SFPSETEXP	127, L2, L0, 1
	SFPMAD	L0, L13, L14, L1, 0
	SFPLOADI	L3, 8, 16389
	SFPLOADI	L3, 10, 37644
	SFPMAD	L0, L1, L3, L1, 0
	SFPLOADI	L3, 8, 49084
	SFPLOADI	L3, 10, 54945
	SFPMAD	L0, L1, L3, L1, 0
	SFPEXEXP	L2, L0, 2
	SFPNOT	L0, L3
	SFPIADD	1, L3, L3, 5
	SFPSETSGN	1, L3, L0, 1
	SFPENCC	3, 10
	SFPCAST L0, L0, 0
	SFPMAD	L0, L12, L1, L0, 0
	lw	a4,%lo(_ZN7ckernel13instrn_bufferE)(a3)
	li	a2,1896939520
	add	a2, a2, a0
# Op(0x71) - d(1)
	sw	a2,0(a4)
	SFPMUL	L0, L1, L9, L0, 0
	SFPSETCC	0, L2, 6
	SFPLOADI	L0, 0, 65408
	SFPENCC	3, 10
	SFPSTORE	L0, 0, 3, 0
 #APP
# 663 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	addi	a5,a5,-1
	bne	a5,zero,.L61
	ret
	.size	_Z13calculate_logILb1ELb1EEvm, .-_Z13calculate_logILb1ELb1EEvm
	.section	.text._Z20calculate_reciprocalILb0ELi4EEvv,"axG",@progbits,_Z20calculate_reciprocalILb0ELi4EEvv,comdat
	.align	1
	.weak	_Z20calculate_reciprocalILb0ELi4EEvv
	.type	_Z20calculate_reciprocalILb0ELi4EEvv, @function
_Z20calculate_reciprocalILb0ELi4EEvv:
	li	a5,4
.L64:
	SFPLOAD	L2, 0, 3, 0
	SFPSETSGN	1, L2, L0, 1
	SFPSETEXP	126, L0, L0, 1
	SFPMAD	L0, L12, L13, L1, 0
	SFPNOP
	SFPMUL	L12, L1, L9, L1, 0
	SFPNOP
	SFPMAD	L0, L1, L13, L3, 0
	SFPNOP
	SFPMUL	L1, L3, L9, L1, 0
	SFPNOP
	SFPMAD	L0, L1, L13, L0, 0
	SFPNOP
	SFPMUL	L1, L0, L9, L1, 0
	SFPEXEXP	L2, L0, 0
	SFPEXEXP	L1, L3, 0
	SFPIADD	0, L3, L0, 6
	SFPIADD	126, L0, L0, 1
	SFPLOADI	L1, 0, 0
	SFPLOADI	L0, 4, 0
	SFPENCC	3, 10
	SFPSETEXP	0, L1, L0, 0
	SFPSETCC	0, L2, 0
	SFPMOV	L0, L0, 1
	SFPENCC	3, 10
	SFPSTORE	L0, 0, 3, 0
 #APP
# 471 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	SFPLOAD	L2, 0, 3, 0
	SFPSETSGN	1, L2, L0, 1
	SFPSETEXP	126, L0, L0, 1
	SFPMAD	L0, L12, L13, L1, 0
	SFPNOP
	SFPMUL	L12, L1, L9, L1, 0
	SFPNOP
	SFPMAD	L0, L1, L13, L3, 0
	SFPNOP
	SFPMUL	L1, L3, L9, L1, 0
	SFPNOP
	SFPMAD	L0, L1, L13, L0, 0
	SFPNOP
	SFPMUL	L1, L0, L9, L1, 0
	SFPEXEXP	L2, L0, 0
	SFPEXEXP	L1, L3, 0
	SFPIADD	0, L3, L0, 6
	SFPIADD	126, L0, L0, 1
	SFPLOADI	L1, 0, 0
	SFPLOADI	L0, 4, 0
	SFPENCC	3, 10
	SFPSETEXP	0, L1, L0, 0
	SFPSETCC	0, L2, 0
	SFPMOV	L0, L0, 1
	SFPENCC	3, 10
	SFPSTORE	L0, 0, 3, 0
 #APP
# 471 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	addi	a5,a5,-2
	bne	a5,zero,.L64
	ret
	.size	_Z20calculate_reciprocalILb0ELi4EEvv, .-_Z20calculate_reciprocalILb0ELi4EEvv
	.section	.text._Z20calculate_reciprocalILb1ELi4EEvv,"axG",@progbits,_Z20calculate_reciprocalILb1ELi4EEvv,comdat
	.align	1
	.weak	_Z20calculate_reciprocalILb1ELi4EEvv
	.type	_Z20calculate_reciprocalILb1ELi4EEvv, @function
_Z20calculate_reciprocalILb1ELi4EEvv:
	li	a5,4
.L68:
	SFPLOAD	L2, 0, 3, 0
	SFPSETSGN	1, L2, L0, 1
	SFPSETEXP	126, L0, L0, 1
	SFPMAD	L0, L12, L13, L1, 0
	SFPNOP
	SFPMUL	L12, L1, L9, L1, 0
	SFPNOP
	SFPMAD	L0, L1, L13, L0, 0
	SFPNOP
	SFPMUL	L1, L0, L9, L1, 0
	SFPEXEXP	L2, L0, 0
	SFPEXEXP	L1, L3, 0
	SFPIADD	0, L3, L0, 6
	SFPIADD	126, L0, L0, 1
	SFPLOADI	L1, 0, 0
	SFPLOADI	L0, 4, 0
	SFPENCC	3, 10
	SFPSETEXP	0, L1, L0, 0
	SFPSETCC	0, L2, 0
	SFPMOV	L0, L0, 1
	SFPENCC	3, 10
	SFPSTORE	L0, 0, 3, 0
 #APP
# 471 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	SFPLOAD	L2, 0, 3, 0
	SFPSETSGN	1, L2, L0, 1
	SFPSETEXP	126, L0, L0, 1
	SFPMAD	L0, L12, L13, L1, 0
	SFPNOP
	SFPMUL	L12, L1, L9, L1, 0
	SFPNOP
	SFPMAD	L0, L1, L13, L0, 0
	SFPNOP
	SFPMUL	L1, L0, L9, L1, 0
	SFPEXEXP	L2, L0, 0
	SFPEXEXP	L1, L3, 0
	SFPIADD	0, L3, L0, 6
	SFPIADD	126, L0, L0, 1
	SFPLOADI	L1, 0, 0
	SFPLOADI	L0, 4, 0
	SFPENCC	3, 10
	SFPSETEXP	0, L1, L0, 0
	SFPSETCC	0, L2, 0
	SFPMOV	L0, L0, 1
	SFPENCC	3, 10
	SFPSTORE	L0, 0, 3, 0
 #APP
# 471 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	addi	a5,a5,-2
	bne	a5,zero,.L68
	ret
	.size	_Z20calculate_reciprocalILb1ELi4EEvv, .-_Z20calculate_reciprocalILb1ELi4EEvv
	.section	.text._Z17calculate_dropoutILb0EEvmm,"axG",@progbits,_Z17calculate_dropoutILb0EEvmm,comdat
	.align	1
	.weak	_Z17calculate_dropoutILb0EEvmm
	.type	_Z17calculate_dropoutILb0EEvmm, @function
_Z17calculate_dropoutILb0EEvmm:
	slli	a1,a1,16
	srli	a3,a0,16
	slli	a0,a0,16
	li	a5,8
	lui	a2,%hi(_ZN7ckernel13instrn_bufferE)
	srli	a1,a1,8
	srli	a0,a0,16
.L72:
	lw	a4,%lo(_ZN7ckernel13instrn_bufferE)(a2)
	SFPLOAD	L0, 0, 3, 0
	li	a6,1946157056
	add	a6, a6, a1
# Op(0x74) lv(0)  d(0)
	sw	a6,0(a4)
	SFPNOP
	SFPNOP
	SFPNOP
	SFPSTORE	L0, 0, 3, 0
	li	a6,1896480768
	add	a6, a6, a0
# Op(0x71) - d(0)
	sw	a6,0(a4)
	li	a6,1896349696
	add	a6, a6, a3
# Op(0x71) lv(0)  d(0)
	sw	a6,0(a4)
	SFPIADD	0, L3, L0, 2
	SFPSTORE	L9, 0, 3, 0
	SFPENCC	3, 10
	SFPMOV	L13, L0, 0
	SFPAND	L3, L0
	SFPSHFT	-1, L0, L3, 1
	SFPSETCC	0, L0, 2
	SFPXOR	L12, L3
	SFPENCC	3, 10
 #APP
# 561 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	addi	a5,a5,-1
	bne	a5,zero,.L72
	ret
	.size	_Z17calculate_dropoutILb0EEvmm, .-_Z17calculate_dropoutILb0EEvmm
	.section	.text.startup,"ax",@progbits
	.align	1
	.globl	main
	.type	main, @function
main:
	addi	sp,sp,-16
	sw	s0,8(sp)
	mv	s0,a0
	sw	ra,12(sp)
	SFPLOAD	L0, 0, 3, 0
	SFPSETCC	0, L0, 0
	SFPLOADI	L0, 0, 0
	SFPCOMPC
	SFPLOADI	L0, 0, 16256
	SFPENCC	3, 10
	SFPSTORE	L0, 0, 3, 0
 #APP
# 746 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	SFPLOAD	L0, 0, 3, 0
	SFPSETCC	0, L0, 0
	SFPLOADI	L0, 0, 0
	SFPCOMPC
	SFPLOADI	L0, 0, 16256
	SFPENCC	3, 10
	SFPSTORE	L0, 0, 3, 0
 #APP
# 746 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	SFPLOAD	L0, 0, 3, 0
	SFPSETCC	0, L0, 0
	SFPLOADI	L0, 0, 0
	SFPCOMPC
	SFPLOADI	L0, 0, 16256
	SFPENCC	3, 10
	SFPSTORE	L0, 0, 3, 0
 #APP
# 746 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	SFPLOAD	L0, 0, 3, 0
	SFPSETCC	0, L0, 0
	SFPLOADI	L0, 0, 0
	SFPCOMPC
	SFPLOADI	L0, 0, 16256
	SFPENCC	3, 10
	SFPSTORE	L0, 0, 3, 0
 #APP
# 746 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	SFPLOAD	L0, 0, 3, 0
	SFPSETCC	0, L0, 0
	SFPLOADI	L0, 0, 0
	SFPCOMPC
	SFPLOADI	L0, 0, 16256
	SFPENCC	3, 10
	SFPSTORE	L0, 0, 3, 0
 #APP
# 746 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	SFPLOAD	L0, 0, 3, 0
	SFPSETCC	0, L0, 0
	SFPLOADI	L0, 0, 0
	SFPCOMPC
	SFPLOADI	L0, 0, 16256
	SFPENCC	3, 10
	SFPSTORE	L0, 0, 3, 0
 #APP
# 746 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	SFPLOAD	L0, 0, 3, 0
	SFPSETCC	0, L0, 0
	SFPLOADI	L0, 0, 0
	SFPCOMPC
	SFPLOADI	L0, 0, 16256
	SFPENCC	3, 10
	SFPSTORE	L0, 0, 3, 0
 #APP
# 746 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	SFPLOAD	L0, 0, 3, 0
	SFPSETCC	0, L0, 0
	SFPLOADI	L0, 0, 0
	SFPCOMPC
	SFPLOADI	L0, 0, 16256
	SFPENCC	3, 10
	SFPSTORE	L0, 0, 3, 0
 #APP
# 746 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	call	_Z14calculate_compILb0EL8SfpuType8EEvm
	SFPLOAD	L0, 0, 3, 0
	SFPSETCC	0, L0, 0
	SFPLOADI	L0, 0, 0
	SFPLOADI	L1, 0, 0
	SFPCOMPC
	SFPLOADI	L0, 0, 16256
	SFPLOADI	L1, 0, 16256
	SFPENCC	3, 10
	SFPAND	L1, L0
	SFPSTORE	L0, 0, 3, 0
 #APP
# 746 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	SFPLOAD	L0, 0, 3, 0
	SFPSETCC	0, L0, 0
	SFPLOADI	L0, 0, 0
	SFPLOADI	L1, 0, 0
	SFPCOMPC
	SFPLOADI	L0, 0, 16256
	SFPLOADI	L1, 0, 16256
	SFPENCC	3, 10
	SFPAND	L1, L0
	SFPSTORE	L0, 0, 3, 0
 #APP
# 746 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	SFPLOAD	L0, 0, 3, 0
	SFPSETCC	0, L0, 0
	SFPLOADI	L0, 0, 0
	SFPLOADI	L1, 0, 0
	SFPCOMPC
	SFPLOADI	L0, 0, 16256
	SFPLOADI	L1, 0, 16256
	SFPENCC	3, 10
	SFPAND	L1, L0
	SFPSTORE	L0, 0, 3, 0
 #APP
# 746 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	SFPLOAD	L0, 0, 3, 0
	SFPSETCC	0, L0, 0
	SFPLOADI	L0, 0, 0
	SFPLOADI	L1, 0, 0
	SFPCOMPC
	SFPLOADI	L0, 0, 16256
	SFPLOADI	L1, 0, 16256
	SFPENCC	3, 10
	SFPAND	L1, L0
	SFPSTORE	L0, 0, 3, 0
 #APP
# 746 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	SFPLOAD	L0, 0, 3, 0
	SFPSETCC	0, L0, 0
	SFPLOADI	L0, 0, 0
	SFPLOADI	L1, 0, 0
	SFPCOMPC
	SFPLOADI	L0, 0, 16256
	SFPLOADI	L1, 0, 16256
	SFPENCC	3, 10
	SFPAND	L1, L0
	SFPSTORE	L0, 0, 3, 0
 #APP
# 746 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	SFPLOAD	L0, 0, 3, 0
	SFPSETCC	0, L0, 0
	SFPLOADI	L0, 0, 0
	SFPLOADI	L1, 0, 0
	SFPCOMPC
	SFPLOADI	L0, 0, 16256
	SFPLOADI	L1, 0, 16256
	SFPENCC	3, 10
	SFPAND	L1, L0
	SFPSTORE	L0, 0, 3, 0
 #APP
# 746 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	SFPLOAD	L0, 0, 3, 0
	SFPSETCC	0, L0, 0
	SFPLOADI	L0, 0, 0
	SFPLOADI	L1, 0, 0
	SFPCOMPC
	SFPLOADI	L0, 0, 16256
	SFPLOADI	L1, 0, 16256
	SFPENCC	3, 10
	SFPAND	L1, L0
	SFPSTORE	L0, 0, 3, 0
 #APP
# 746 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	SFPLOAD	L0, 0, 3, 0
	SFPSETCC	0, L0, 0
	SFPLOADI	L0, 0, 0
	SFPLOADI	L1, 0, 0
	SFPCOMPC
	SFPLOADI	L0, 0, 16256
	SFPLOADI	L1, 0, 16256
	SFPENCC	3, 10
	SFPAND	L1, L0
	SFPSTORE	L0, 0, 3, 0
 #APP
# 746 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	SFPLOAD	L0, 0, 3, 0
	SFPSETCC	0, L0, 0
	SFPLOADI	L0, 0, 16256
	SFPLOADI	L1, 0, 16256
	SFPCOMPC
	SFPLOADI	L0, 0, 0
	SFPLOADI	L1, 0, 0
	SFPENCC	3, 10
	SFPOR	L1, L0
	SFPSTORE	L0, 0, 3, 0
 #APP
# 746 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	SFPLOAD	L0, 0, 3, 0
	SFPSETCC	0, L0, 0
	SFPLOADI	L0, 0, 16256
	SFPLOADI	L1, 0, 16256
	SFPCOMPC
	SFPLOADI	L0, 0, 0
	SFPLOADI	L1, 0, 0
	SFPENCC	3, 10
	SFPOR	L1, L0
	SFPSTORE	L0, 0, 3, 0
 #APP
# 746 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	SFPLOAD	L0, 0, 3, 0
	SFPSETCC	0, L0, 0
	SFPLOADI	L0, 0, 16256
	SFPLOADI	L1, 0, 16256
	SFPCOMPC
	SFPLOADI	L0, 0, 0
	SFPLOADI	L1, 0, 0
	SFPENCC	3, 10
	SFPOR	L1, L0
	SFPSTORE	L0, 0, 3, 0
 #APP
# 746 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	SFPLOAD	L0, 0, 3, 0
	SFPSETCC	0, L0, 0
	SFPLOADI	L0, 0, 16256
	SFPLOADI	L1, 0, 16256
	SFPCOMPC
	SFPLOADI	L0, 0, 0
	SFPLOADI	L1, 0, 0
	SFPENCC	3, 10
	SFPOR	L1, L0
	SFPSTORE	L0, 0, 3, 0
 #APP
# 746 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	SFPLOAD	L0, 0, 3, 0
	SFPSETCC	0, L0, 0
	SFPLOADI	L0, 0, 16256
	SFPLOADI	L1, 0, 16256
	SFPCOMPC
	SFPLOADI	L0, 0, 0
	SFPLOADI	L1, 0, 0
	SFPENCC	3, 10
	SFPOR	L1, L0
	SFPSTORE	L0, 0, 3, 0
 #APP
# 746 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	SFPLOAD	L0, 0, 3, 0
	SFPSETCC	0, L0, 0
	SFPLOADI	L0, 0, 16256
	SFPLOADI	L1, 0, 16256
	SFPCOMPC
	SFPLOADI	L0, 0, 0
	SFPLOADI	L1, 0, 0
	SFPENCC	3, 10
	SFPOR	L1, L0
	SFPSTORE	L0, 0, 3, 0
 #APP
# 746 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	SFPLOAD	L0, 0, 3, 0
	SFPSETCC	0, L0, 0
	SFPLOADI	L0, 0, 16256
	SFPLOADI	L1, 0, 16256
	SFPCOMPC
	SFPLOADI	L0, 0, 0
	SFPLOADI	L1, 0, 0
	SFPENCC	3, 10
	SFPOR	L1, L0
	SFPSTORE	L0, 0, 3, 0
 #APP
# 746 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	SFPLOAD	L0, 0, 3, 0
	SFPSETCC	0, L0, 0
	SFPLOADI	L0, 0, 16256
	SFPLOADI	L1, 0, 16256
	SFPCOMPC
	SFPLOADI	L0, 0, 0
	SFPLOADI	L1, 0, 0
	SFPENCC	3, 10
	SFPOR	L1, L0
	SFPSTORE	L0, 0, 3, 0
 #APP
# 746 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	mv	a0,s0
	call	_Z14calculate_compILb0EL8SfpuType10EEvm
	mv	a2,s0
	mv	a1,s0
	mv	a0,s0
	call	_Z15calculate_clampILb0EEvmmm
	call	_Z13calculate_absILb0EEvv
	li	a0,1
	call	_Z14calculate_signILb0EEvm
	call	_Z17calculate_sigmoidILb0EEvv
	call	_Z14calculate_tanhILb1EEvv
	mv	a2,s0
	mv	a1,s0
	mv	a0,s0
	call	_Z18calculate_hardtanhILb0EEvmmm
	mv	a0,s0
	call	_Z15calculate_lreluILb0EEvm
	call	_Z25calculate_tanh_derivativeILb0ELi0EEvv
	call	_Z25calculate_tanh_derivativeILb0ELi1EEvv
	mv	a0,s0
	call	_Z15calculate_powerILb1EEvm
	call	_Z14calculate_geluILb0EEvv
	call	_Z14calculate_geluILb1EEvv
	call	_Z25calculate_gelu_derivativeILb0EEvv
	call	_Z14calculate_sqrtILb0ELi4ELi2EEvv
	call	_Z14calculate_sqrtILb1ELi4ELi2EEvv
	call	_Z21calculate_exponentialILb0ELb0ELb0EEvt.isra.0
	li	a0,0
	call	_Z21calculate_exponentialILb0ELb0ELb1EEvt
	call	_Z21calculate_exponentialILb0ELb1ELb0EEvt.isra.0
	li	a0,0
	call	_Z21calculate_exponentialILb0ELb1ELb1EEvt
	call	_Z21calculate_exponentialILb1ELb0ELb0EEvt.isra.0
	li	a0,0
	call	_Z21calculate_exponentialILb1ELb0ELb1EEvt
	call	_Z21calculate_exponentialILb1ELb1ELb0EEvt.isra.0
	li	a0,0
	call	_Z21calculate_exponentialILb1ELb1ELb1EEvt
	mv	a0,s0
	call	_Z13calculate_logILb1ELb1EEvm
	call	_Z13calculate_logILb1ELb0EEvm.isra.0
	call	_Z20calculate_reciprocalILb0ELi4EEvv
	call	_Z20calculate_reciprocalILb1ELi4EEvv
	mv	a0,s0
	mv	a1,s0
	call	_Z17calculate_dropoutILb0EEvmm
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
