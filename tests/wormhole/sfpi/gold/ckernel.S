	.file	"ckernel.cc"
	.option nopic
	.attribute arch, "rv32i2p0_y2p0"
	.attribute unaligned_access, 0
	.attribute stack_align, 16
	.text
	.align	1
	.type	_ZN7ckernel4sfpu21calculate_exponentialILb1ELb1ELb0ELm0EEEvt.isra.0, @function
_ZN7ckernel4sfpu21calculate_exponentialILb1ELb1ELb0ELm0EEEvt.isra.0:
	li	a5,8
.L2:
	SFPLOAD	L1, 0, 0
	SFPMAD	L1, L13, L0, L1, 0
	SFPNOP
	SFPIADD	0, L2, L1, 4
	SFPSHFT	8, L0, L1, 1
	SFPSTORE	L1, 0, 0
	SFPSETCC	0, L1, 0
	SFPSTORE	L9, 0, 0
	SFPENCC	3, 10
 #APP
# 296 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	addi	a5,a5,-1
	bne	a5,zero,.L2
	ret
	.size	_ZN7ckernel4sfpu21calculate_exponentialILb1ELb1ELb0ELm0EEEvt.isra.0, .-_ZN7ckernel4sfpu21calculate_exponentialILb1ELb1ELb0ELm0EEEvt.isra.0
	.align	1
	.type	_ZN7ckernel4sfpu21calculate_exponentialILb1ELb0ELb0ELm0EEEvt.isra.0, @function
_ZN7ckernel4sfpu21calculate_exponentialILb1ELb0ELb0ELm0EEEvt.isra.0:
	li	a5,8
.L6:
	SFPLOAD	L1, 0, 0
	SFPMAD	L1, L13, L0, L1, 0
	SFPNOP
	SFPIADD	0, L2, L1, 4
	SFPSHFT	8, L0, L1, 1
	SFPSTORE	L1, 0, 0
 #APP
# 296 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	addi	a5,a5,-1
	bne	a5,zero,.L6
	ret
	.size	_ZN7ckernel4sfpu21calculate_exponentialILb1ELb0ELb0ELm0EEEvt.isra.0, .-_ZN7ckernel4sfpu21calculate_exponentialILb1ELb0ELb0ELm0EEEvt.isra.0
	.align	1
	.type	_ZN7ckernel4sfpu21calculate_exponentialILb0ELb1ELb0ELm0EEEvt.isra.0, @function
_ZN7ckernel4sfpu21calculate_exponentialILb0ELb1ELb0ELm0EEEvt.isra.0:
	li	a5,8
.L9:
	SFPLOAD	L1, 0, 0
	SFPSETSGN	0, L1, L0, 1
	SFPEXEXP	L0, L2, 10
	SFPSETEXP	126, L0, L0, 1
	SFPENCC	3, 10
	SFPLOADI	L3, 0, 16221
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
	SFPLOADI	L3, 0, 16384
	SFPMAD	L2, L13, L3, L1, 0
	SFPNOP
	SFPMUL	L13, L1, L9, L1, 0
	SFPNOP
	SFPMAD	L2, L1, L3, L4, 0
	SFPNOP
	SFPMUL	L1, L4, L9, L1, 0
	SFPNOP
	SFPMAD	L2, L1, L3, L2, 0
	SFPNOP
	SFPMUL	L1, L2, L9, L1, 0
	SFPEXEXP	L0, L2, 0
	SFPEXEXP	L1, L3, 0
	SFPIADD	0, L3, L2, 6
	SFPPUSHC
	SFPIADD	126, L2, L2, 1
	SFPLOADI	L1, 0, 0
	SFPLOADI	L2, 4, 0
	SFPPOPC
	SFPSETEXP	0, L1, L2, 0
	SFPMOV	L2, L0, 0
	SFPENCC	3, 10
	SFPSTORE	L0, 0, 0
 #APP
# 296 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	addi	a5,a5,-1
	bne	a5,zero,.L9
	ret
	.size	_ZN7ckernel4sfpu21calculate_exponentialILb0ELb1ELb0ELm0EEEvt.isra.0, .-_ZN7ckernel4sfpu21calculate_exponentialILb0ELb1ELb0ELm0EEEvt.isra.0
	.align	1
	.type	_ZN7ckernel4sfpu21calculate_exponentialILb0ELb0ELb0ELm0EEEvt.isra.0, @function
_ZN7ckernel4sfpu21calculate_exponentialILb0ELb0ELb0ELm0EEEvt.isra.0:
	li	a5,8
.L12:
	SFPLOAD	L1, 0, 0
	SFPSETSGN	0, L1, L0, 1
	SFPEXEXP	L0, L2, 10
	SFPSETEXP	126, L0, L0, 1
	SFPENCC	3, 10
	SFPLOADI	L3, 0, 16221
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
	SFPLOADI	L3, 0, 16384
	SFPMAD	L2, L13, L3, L1, 0
	SFPNOP
	SFPMUL	L13, L1, L9, L1, 0
	SFPNOP
	SFPMAD	L2, L1, L3, L4, 0
	SFPNOP
	SFPMUL	L1, L4, L9, L1, 0
	SFPNOP
	SFPMAD	L2, L1, L3, L2, 0
	SFPNOP
	SFPMUL	L1, L2, L9, L1, 0
	SFPEXEXP	L0, L2, 0
	SFPEXEXP	L1, L3, 0
	SFPIADD	0, L3, L2, 6
	SFPPUSHC
	SFPIADD	126, L2, L2, 1
	SFPLOADI	L1, 0, 0
	SFPLOADI	L2, 4, 0
	SFPPOPC
	SFPSETEXP	0, L1, L2, 0
	SFPMOV	L2, L0, 0
	SFPENCC	3, 10
	SFPSTORE	L0, 0, 0
 #APP
# 296 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	addi	a5,a5,-1
	bne	a5,zero,.L12
	ret
	.size	_ZN7ckernel4sfpu21calculate_exponentialILb0ELb0ELb0ELm0EEEvt.isra.0, .-_ZN7ckernel4sfpu21calculate_exponentialILb0ELb0ELb0ELm0EEEvt.isra.0
	.align	1
	.type	_ZN7ckernel4sfpu18calculate_log_bodyILb0ELm0EEEvm.isra.0, @function
_ZN7ckernel4sfpu18calculate_log_bodyILb0ELm0EEEvm.isra.0:
	SFPLOAD	L2, 0, 0
	SFPSETEXP	127, L2, L0, 1
	SFPLOADI	L1, 8, 15832
	SFPLOADI	L1, 10, 44460
	SFPLOADI	L3, 8, 48950
	SFPLOADI	L3, 10, 11115
	SFPMAD	L0, L1, L3, L1, 0
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
	SFPSTORE	L0, 0, 0
	ret
	.size	_ZN7ckernel4sfpu18calculate_log_bodyILb0ELm0EEEvm.isra.0, .-_ZN7ckernel4sfpu18calculate_log_bodyILb0ELm0EEEvm.isra.0
	.align	1
	.type	_ZN7ckernel4sfpu13calculate_logILb1ELb0ELm0EEEvm.isra.0, @function
_ZN7ckernel4sfpu13calculate_logILb1ELb0ELm0EEEvm.isra.0:
	addi	sp,sp,-16
	sw	s0,8(sp)
	sw	ra,12(sp)
	li	s0,8
.L16:
	call	_ZN7ckernel4sfpu18calculate_log_bodyILb0ELm0EEEvm.isra.0
 #APP
# 713 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	addi	s0,s0,-1
	bne	s0,zero,.L16
	lw	ra,12(sp)
	lw	s0,8(sp)
	addi	sp,sp,16
	jr	ra
	.size	_ZN7ckernel4sfpu13calculate_logILb1ELb0ELm0EEEvm.isra.0, .-_ZN7ckernel4sfpu13calculate_logILb1ELb0ELm0EEEvm.isra.0
	.section	.text._ZN7ckernel4sfpu14calculate_compILb0EL8SfpuType8ELm0EEEvm,"axG",@progbits,_ZN7ckernel4sfpu14calculate_compILb0EL8SfpuType8ELm0EEEvm,comdat
	.align	1
	.weak	_ZN7ckernel4sfpu14calculate_compILb0EL8SfpuType8ELm0EEEvm
	.type	_ZN7ckernel4sfpu14calculate_compILb0EL8SfpuType8ELm0EEEvm, @function
_ZN7ckernel4sfpu14calculate_compILb0EL8SfpuType8ELm0EEEvm:
	bne	a0,zero,.L20
	SFPLOAD	L1, 0, 0
	SFPLOADI	L0, 4, 14336
	SFPIADD	0, L1, L0, 4
	SFPSETCC	0, L0, 6
	SFPLOADI	L0, 0, 0
	SFPCOMPC
	SFPLOADI	L0, 0, 16256
	SFPENCC	3, 10
	SFPSTORE	L0, 0, 0
 #APP
# 796 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	SFPLOAD	L1, 0, 0
	SFPLOADI	L0, 4, 14336
	SFPIADD	0, L1, L0, 4
	SFPSETCC	0, L0, 6
	SFPLOADI	L0, 0, 0
	SFPCOMPC
	SFPLOADI	L0, 0, 16256
	SFPENCC	3, 10
	SFPSTORE	L0, 0, 0
 #APP
# 796 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	SFPLOAD	L1, 0, 0
	SFPLOADI	L0, 4, 14336
	SFPIADD	0, L1, L0, 4
	SFPSETCC	0, L0, 6
	SFPLOADI	L0, 0, 0
	SFPCOMPC
	SFPLOADI	L0, 0, 16256
	SFPENCC	3, 10
	SFPSTORE	L0, 0, 0
 #APP
# 796 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	SFPLOAD	L1, 0, 0
	SFPLOADI	L0, 4, 14336
	SFPIADD	0, L1, L0, 4
	SFPSETCC	0, L0, 6
	SFPLOADI	L0, 0, 0
	SFPCOMPC
	SFPLOADI	L0, 0, 16256
	SFPENCC	3, 10
	SFPSTORE	L0, 0, 0
 #APP
# 796 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	SFPLOAD	L1, 0, 0
	SFPLOADI	L0, 4, 14336
	SFPIADD	0, L1, L0, 4
	SFPSETCC	0, L0, 6
	SFPLOADI	L0, 0, 0
	SFPCOMPC
	SFPLOADI	L0, 0, 16256
	SFPENCC	3, 10
	SFPSTORE	L0, 0, 0
 #APP
# 796 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	SFPLOAD	L1, 0, 0
	SFPLOADI	L0, 4, 14336
	SFPIADD	0, L1, L0, 4
	SFPSETCC	0, L0, 6
	SFPLOADI	L0, 0, 0
	SFPCOMPC
	SFPLOADI	L0, 0, 16256
	SFPENCC	3, 10
	SFPSTORE	L0, 0, 0
 #APP
# 796 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	SFPLOAD	L1, 0, 0
	SFPLOADI	L0, 4, 14336
	SFPIADD	0, L1, L0, 4
	SFPSETCC	0, L0, 6
	SFPLOADI	L0, 0, 0
	SFPCOMPC
	SFPLOADI	L0, 0, 16256
	SFPENCC	3, 10
	SFPSTORE	L0, 0, 0
 #APP
# 796 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	SFPLOAD	L1, 0, 0
	SFPLOADI	L0, 4, 14336
	SFPIADD	0, L1, L0, 4
	SFPSETCC	0, L0, 6
	SFPLOADI	L0, 0, 0
	SFPCOMPC
	SFPLOADI	L0, 0, 16256
	SFPENCC	3, 10
	SFPSTORE	L0, 0, 0
 #APP
# 796 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	ret
.L20:
	SFPLOAD	L0, 0, 0
	SFPSETCC	0, L0, 6
	SFPLOADI	L0, 0, 0
	SFPCOMPC
	SFPLOADI	L0, 0, 16256
	SFPENCC	3, 10
	SFPSTORE	L0, 0, 0
 #APP
# 796 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	SFPLOAD	L0, 0, 0
	SFPSETCC	0, L0, 6
	SFPLOADI	L0, 0, 0
	SFPCOMPC
	SFPLOADI	L0, 0, 16256
	SFPENCC	3, 10
	SFPSTORE	L0, 0, 0
 #APP
# 796 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	SFPLOAD	L0, 0, 0
	SFPSETCC	0, L0, 6
	SFPLOADI	L0, 0, 0
	SFPCOMPC
	SFPLOADI	L0, 0, 16256
	SFPENCC	3, 10
	SFPSTORE	L0, 0, 0
 #APP
# 796 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	SFPLOAD	L0, 0, 0
	SFPSETCC	0, L0, 6
	SFPLOADI	L0, 0, 0
	SFPCOMPC
	SFPLOADI	L0, 0, 16256
	SFPENCC	3, 10
	SFPSTORE	L0, 0, 0
 #APP
# 796 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	SFPLOAD	L0, 0, 0
	SFPSETCC	0, L0, 6
	SFPLOADI	L0, 0, 0
	SFPCOMPC
	SFPLOADI	L0, 0, 16256
	SFPENCC	3, 10
	SFPSTORE	L0, 0, 0
 #APP
# 796 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	SFPLOAD	L0, 0, 0
	SFPSETCC	0, L0, 6
	SFPLOADI	L0, 0, 0
	SFPCOMPC
	SFPLOADI	L0, 0, 16256
	SFPENCC	3, 10
	SFPSTORE	L0, 0, 0
 #APP
# 796 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	SFPLOAD	L0, 0, 0
	SFPSETCC	0, L0, 6
	SFPLOADI	L0, 0, 0
	SFPCOMPC
	SFPLOADI	L0, 0, 16256
	SFPENCC	3, 10
	SFPSTORE	L0, 0, 0
 #APP
# 796 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	SFPLOAD	L0, 0, 0
	SFPSETCC	0, L0, 6
	SFPLOADI	L0, 0, 0
	SFPCOMPC
	SFPLOADI	L0, 0, 16256
	SFPENCC	3, 10
	SFPSTORE	L0, 0, 0
 #APP
# 796 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	ret
	.size	_ZN7ckernel4sfpu14calculate_compILb0EL8SfpuType8ELm0EEEvm, .-_ZN7ckernel4sfpu14calculate_compILb0EL8SfpuType8ELm0EEEvm
	.section	.text._ZN7ckernel4sfpu14calculate_compILb0EL8SfpuType10ELm0EEEvm,"axG",@progbits,_ZN7ckernel4sfpu14calculate_compILb0EL8SfpuType10ELm0EEEvm,comdat
	.align	1
	.weak	_ZN7ckernel4sfpu14calculate_compILb0EL8SfpuType10ELm0EEEvm
	.type	_ZN7ckernel4sfpu14calculate_compILb0EL8SfpuType10ELm0EEEvm, @function
_ZN7ckernel4sfpu14calculate_compILb0EL8SfpuType10ELm0EEEvm:
	bne	a0,zero,.L24
	SFPLOAD	L1, 0, 0
	SFPLOADI	L0, 4, 14336
	SFPIADD	0, L1, L0, 4
	SFPSETCC	0, L0, 6
	SFPLOADI	L0, 0, 16256
	SFPCOMPC
	SFPLOADI	L0, 0, 0
	SFPENCC	3, 10
	SFPSTORE	L0, 0, 0
 #APP
# 796 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	SFPLOAD	L1, 0, 0
	SFPLOADI	L0, 4, 14336
	SFPIADD	0, L1, L0, 4
	SFPSETCC	0, L0, 6
	SFPLOADI	L0, 0, 16256
	SFPCOMPC
	SFPLOADI	L0, 0, 0
	SFPENCC	3, 10
	SFPSTORE	L0, 0, 0
 #APP
# 796 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	SFPLOAD	L1, 0, 0
	SFPLOADI	L0, 4, 14336
	SFPIADD	0, L1, L0, 4
	SFPSETCC	0, L0, 6
	SFPLOADI	L0, 0, 16256
	SFPCOMPC
	SFPLOADI	L0, 0, 0
	SFPENCC	3, 10
	SFPSTORE	L0, 0, 0
 #APP
# 796 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	SFPLOAD	L1, 0, 0
	SFPLOADI	L0, 4, 14336
	SFPIADD	0, L1, L0, 4
	SFPSETCC	0, L0, 6
	SFPLOADI	L0, 0, 16256
	SFPCOMPC
	SFPLOADI	L0, 0, 0
	SFPENCC	3, 10
	SFPSTORE	L0, 0, 0
 #APP
# 796 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	SFPLOAD	L1, 0, 0
	SFPLOADI	L0, 4, 14336
	SFPIADD	0, L1, L0, 4
	SFPSETCC	0, L0, 6
	SFPLOADI	L0, 0, 16256
	SFPCOMPC
	SFPLOADI	L0, 0, 0
	SFPENCC	3, 10
	SFPSTORE	L0, 0, 0
 #APP
# 796 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	SFPLOAD	L1, 0, 0
	SFPLOADI	L0, 4, 14336
	SFPIADD	0, L1, L0, 4
	SFPSETCC	0, L0, 6
	SFPLOADI	L0, 0, 16256
	SFPCOMPC
	SFPLOADI	L0, 0, 0
	SFPENCC	3, 10
	SFPSTORE	L0, 0, 0
 #APP
# 796 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	SFPLOAD	L1, 0, 0
	SFPLOADI	L0, 4, 14336
	SFPIADD	0, L1, L0, 4
	SFPSETCC	0, L0, 6
	SFPLOADI	L0, 0, 16256
	SFPCOMPC
	SFPLOADI	L0, 0, 0
	SFPENCC	3, 10
	SFPSTORE	L0, 0, 0
 #APP
# 796 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	SFPLOAD	L1, 0, 0
	SFPLOADI	L0, 4, 14336
	SFPIADD	0, L1, L0, 4
	SFPSETCC	0, L0, 6
	SFPLOADI	L0, 0, 16256
	SFPCOMPC
	SFPLOADI	L0, 0, 0
	SFPENCC	3, 10
	SFPSTORE	L0, 0, 0
 #APP
# 796 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	ret
.L24:
	SFPLOAD	L0, 0, 0
	SFPSETCC	0, L0, 6
	SFPLOADI	L0, 0, 16256
	SFPCOMPC
	SFPLOADI	L0, 0, 0
	SFPENCC	3, 10
	SFPSTORE	L0, 0, 0
 #APP
# 796 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	SFPLOAD	L0, 0, 0
	SFPSETCC	0, L0, 6
	SFPLOADI	L0, 0, 16256
	SFPCOMPC
	SFPLOADI	L0, 0, 0
	SFPENCC	3, 10
	SFPSTORE	L0, 0, 0
 #APP
# 796 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	SFPLOAD	L0, 0, 0
	SFPSETCC	0, L0, 6
	SFPLOADI	L0, 0, 16256
	SFPCOMPC
	SFPLOADI	L0, 0, 0
	SFPENCC	3, 10
	SFPSTORE	L0, 0, 0
 #APP
# 796 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	SFPLOAD	L0, 0, 0
	SFPSETCC	0, L0, 6
	SFPLOADI	L0, 0, 16256
	SFPCOMPC
	SFPLOADI	L0, 0, 0
	SFPENCC	3, 10
	SFPSTORE	L0, 0, 0
 #APP
# 796 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	SFPLOAD	L0, 0, 0
	SFPSETCC	0, L0, 6
	SFPLOADI	L0, 0, 16256
	SFPCOMPC
	SFPLOADI	L0, 0, 0
	SFPENCC	3, 10
	SFPSTORE	L0, 0, 0
 #APP
# 796 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	SFPLOAD	L0, 0, 0
	SFPSETCC	0, L0, 6
	SFPLOADI	L0, 0, 16256
	SFPCOMPC
	SFPLOADI	L0, 0, 0
	SFPENCC	3, 10
	SFPSTORE	L0, 0, 0
 #APP
# 796 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	SFPLOAD	L0, 0, 0
	SFPSETCC	0, L0, 6
	SFPLOADI	L0, 0, 16256
	SFPCOMPC
	SFPLOADI	L0, 0, 0
	SFPENCC	3, 10
	SFPSTORE	L0, 0, 0
 #APP
# 796 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	SFPLOAD	L0, 0, 0
	SFPSETCC	0, L0, 6
	SFPLOADI	L0, 0, 16256
	SFPCOMPC
	SFPLOADI	L0, 0, 0
	SFPENCC	3, 10
	SFPSTORE	L0, 0, 0
 #APP
# 796 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	ret
	.size	_ZN7ckernel4sfpu14calculate_compILb0EL8SfpuType10ELm0EEEvm, .-_ZN7ckernel4sfpu14calculate_compILb0EL8SfpuType10ELm0EEEvm
	.section	.text._ZN7ckernel4sfpu15calculate_clampILb0ELm0EEEvmmm,"axG",@progbits,_ZN7ckernel4sfpu15calculate_clampILb0ELm0EEEvmmm,comdat
	.align	1
	.weak	_ZN7ckernel4sfpu15calculate_clampILb0ELm0EEEvmmm
	.type	_ZN7ckernel4sfpu15calculate_clampILb0ELm0EEEvmmm, @function
_ZN7ckernel4sfpu15calculate_clampILb0ELm0EEEvmmm:
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
.L28:
	SFPLOAD	L0, 0, 0
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
	SFPSTORE	L0, 0, 0
 #APP
# 827 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	addi	a4,a4,-1
	bne	a4,zero,.L28
	ret
	.size	_ZN7ckernel4sfpu15calculate_clampILb0ELm0EEEvmmm, .-_ZN7ckernel4sfpu15calculate_clampILb0ELm0EEEvmmm
	.section	.text._ZN7ckernel4sfpu13calculate_absILb0ELm0EEEvv,"axG",@progbits,_ZN7ckernel4sfpu13calculate_absILb0ELm0EEEvv,comdat
	.align	1
	.weak	_ZN7ckernel4sfpu13calculate_absILb0ELm0EEEvv
	.type	_ZN7ckernel4sfpu13calculate_absILb0ELm0EEEvv, @function
_ZN7ckernel4sfpu13calculate_absILb0ELm0EEEvv:
	SFPLOAD	L0, 0, 0
	SFPABS	L0, L0, 1
	SFPSTORE	L0, 0, 0
 #APP
# 839 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	SFPLOAD	L0, 0, 0
	SFPABS	L0, L0, 1
	SFPSTORE	L0, 0, 0
 #APP
# 839 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	SFPLOAD	L0, 0, 0
	SFPABS	L0, L0, 1
	SFPSTORE	L0, 0, 0
 #APP
# 839 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	SFPLOAD	L0, 0, 0
	SFPABS	L0, L0, 1
	SFPSTORE	L0, 0, 0
 #APP
# 839 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	SFPLOAD	L0, 0, 0
	SFPABS	L0, L0, 1
	SFPSTORE	L0, 0, 0
 #APP
# 839 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	SFPLOAD	L0, 0, 0
	SFPABS	L0, L0, 1
	SFPSTORE	L0, 0, 0
 #APP
# 839 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	SFPLOAD	L0, 0, 0
	SFPABS	L0, L0, 1
	SFPSTORE	L0, 0, 0
 #APP
# 839 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	SFPLOAD	L0, 0, 0
	SFPABS	L0, L0, 1
	SFPSTORE	L0, 0, 0
 #APP
# 839 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	ret
	.size	_ZN7ckernel4sfpu13calculate_absILb0ELm0EEEvv, .-_ZN7ckernel4sfpu13calculate_absILb0ELm0EEEvv
	.section	.text._ZN7ckernel4sfpu14calculate_signILb0ELm0EEEvm,"axG",@progbits,_ZN7ckernel4sfpu14calculate_signILb0ELm0EEEvm,comdat
	.align	1
	.weak	_ZN7ckernel4sfpu14calculate_signILb0ELm0EEEvm
	.type	_ZN7ckernel4sfpu14calculate_signILb0ELm0EEEvm, @function
_ZN7ckernel4sfpu14calculate_signILb0ELm0EEEvm:
	bne	a0,zero,.L32
	SFPLOAD	L1, 0, 0
	SFPSTORE	L10, 0, 0
	SFPSETCC	0, L1, 0
	SFPSTORE	L11, 0, 0
	SFPENCC	3, 10
	SFPLOADI	L0, 4, 14336
	SFPIADD	0, L1, L0, 4
	SFPSETCC	0, L0, 6
	SFPSTORE	L9, 0, 0
	SFPENCC	3, 10
 #APP
# 864 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	SFPLOAD	L1, 0, 0
	SFPSTORE	L10, 0, 0
	SFPSETCC	0, L1, 0
	SFPSTORE	L11, 0, 0
	SFPENCC	3, 10
	SFPLOADI	L0, 4, 14336
	SFPIADD	0, L1, L0, 4
	SFPSETCC	0, L0, 6
	SFPSTORE	L9, 0, 0
	SFPENCC	3, 10
 #APP
# 864 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	SFPLOAD	L1, 0, 0
	SFPSTORE	L10, 0, 0
	SFPSETCC	0, L1, 0
	SFPSTORE	L11, 0, 0
	SFPENCC	3, 10
	SFPLOADI	L0, 4, 14336
	SFPIADD	0, L1, L0, 4
	SFPSETCC	0, L0, 6
	SFPSTORE	L9, 0, 0
	SFPENCC	3, 10
 #APP
# 864 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	SFPLOAD	L1, 0, 0
	SFPSTORE	L10, 0, 0
	SFPSETCC	0, L1, 0
	SFPSTORE	L11, 0, 0
	SFPENCC	3, 10
	SFPLOADI	L0, 4, 14336
	SFPIADD	0, L1, L0, 4
	SFPSETCC	0, L0, 6
	SFPSTORE	L9, 0, 0
	SFPENCC	3, 10
 #APP
# 864 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	SFPLOAD	L1, 0, 0
	SFPSTORE	L10, 0, 0
	SFPSETCC	0, L1, 0
	SFPSTORE	L11, 0, 0
	SFPENCC	3, 10
	SFPLOADI	L0, 4, 14336
	SFPIADD	0, L1, L0, 4
	SFPSETCC	0, L0, 6
	SFPSTORE	L9, 0, 0
	SFPENCC	3, 10
 #APP
# 864 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	SFPLOAD	L1, 0, 0
	SFPSTORE	L10, 0, 0
	SFPSETCC	0, L1, 0
	SFPSTORE	L11, 0, 0
	SFPENCC	3, 10
	SFPLOADI	L0, 4, 14336
	SFPIADD	0, L1, L0, 4
	SFPSETCC	0, L0, 6
	SFPSTORE	L9, 0, 0
	SFPENCC	3, 10
 #APP
# 864 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	SFPLOAD	L1, 0, 0
	SFPSTORE	L10, 0, 0
	SFPSETCC	0, L1, 0
	SFPSTORE	L11, 0, 0
	SFPENCC	3, 10
	SFPLOADI	L0, 4, 14336
	SFPIADD	0, L1, L0, 4
	SFPSETCC	0, L0, 6
	SFPSTORE	L9, 0, 0
	SFPENCC	3, 10
 #APP
# 864 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	SFPLOAD	L1, 0, 0
	SFPSTORE	L10, 0, 0
	SFPSETCC	0, L1, 0
	SFPSTORE	L11, 0, 0
	SFPENCC	3, 10
	SFPLOADI	L0, 4, 14336
	SFPIADD	0, L1, L0, 4
	SFPSETCC	0, L0, 6
	SFPSTORE	L9, 0, 0
	SFPENCC	3, 10
 #APP
# 864 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	ret
.L32:
	SFPLOAD	L0, 0, 0
	SFPSTORE	L10, 0, 0
	SFPSETCC	0, L0, 0
	SFPSTORE	L11, 0, 0
	SFPENCC	3, 10
	SFPSETCC	0, L0, 6
	SFPSTORE	L9, 0, 0
	SFPENCC	3, 10
 #APP
# 864 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	SFPLOAD	L0, 0, 0
	SFPSTORE	L10, 0, 0
	SFPSETCC	0, L0, 0
	SFPSTORE	L11, 0, 0
	SFPENCC	3, 10
	SFPSETCC	0, L0, 6
	SFPSTORE	L9, 0, 0
	SFPENCC	3, 10
 #APP
# 864 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	SFPLOAD	L0, 0, 0
	SFPSTORE	L10, 0, 0
	SFPSETCC	0, L0, 0
	SFPSTORE	L11, 0, 0
	SFPENCC	3, 10
	SFPSETCC	0, L0, 6
	SFPSTORE	L9, 0, 0
	SFPENCC	3, 10
 #APP
# 864 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	SFPLOAD	L0, 0, 0
	SFPSTORE	L10, 0, 0
	SFPSETCC	0, L0, 0
	SFPSTORE	L11, 0, 0
	SFPENCC	3, 10
	SFPSETCC	0, L0, 6
	SFPSTORE	L9, 0, 0
	SFPENCC	3, 10
 #APP
# 864 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	SFPLOAD	L0, 0, 0
	SFPSTORE	L10, 0, 0
	SFPSETCC	0, L0, 0
	SFPSTORE	L11, 0, 0
	SFPENCC	3, 10
	SFPSETCC	0, L0, 6
	SFPSTORE	L9, 0, 0
	SFPENCC	3, 10
 #APP
# 864 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	SFPLOAD	L0, 0, 0
	SFPSTORE	L10, 0, 0
	SFPSETCC	0, L0, 0
	SFPSTORE	L11, 0, 0
	SFPENCC	3, 10
	SFPSETCC	0, L0, 6
	SFPSTORE	L9, 0, 0
	SFPENCC	3, 10
 #APP
# 864 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	SFPLOAD	L0, 0, 0
	SFPSTORE	L10, 0, 0
	SFPSETCC	0, L0, 0
	SFPSTORE	L11, 0, 0
	SFPENCC	3, 10
	SFPSETCC	0, L0, 6
	SFPSTORE	L9, 0, 0
	SFPENCC	3, 10
 #APP
# 864 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	SFPLOAD	L0, 0, 0
	SFPSTORE	L10, 0, 0
	SFPSETCC	0, L0, 0
	SFPSTORE	L11, 0, 0
	SFPENCC	3, 10
	SFPSETCC	0, L0, 6
	SFPSTORE	L9, 0, 0
	SFPENCC	3, 10
 #APP
# 864 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	ret
	.size	_ZN7ckernel4sfpu14calculate_signILb0ELm0EEEvm, .-_ZN7ckernel4sfpu14calculate_signILb0ELm0EEEvm
	.section	.text._ZN7ckernel4sfpu17calculate_sigmoidILb0ELm0EEEvv,"axG",@progbits,_ZN7ckernel4sfpu17calculate_sigmoidILb0ELm0EEEvv,comdat
	.align	1
	.weak	_ZN7ckernel4sfpu17calculate_sigmoidILb0ELm0EEEvv
	.type	_ZN7ckernel4sfpu17calculate_sigmoidILb0ELm0EEEvv, @function
_ZN7ckernel4sfpu17calculate_sigmoidILb0ELm0EEEvv:
	SFPLOAD	L3, 0, 0
	SFPLUT	L3, 4
	SFPNOP
	SFPADDI	16128, L3, 0
	SFPNOP
	SFPSTORE	L3, 0, 0
 #APP
# 391 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	SFPLOAD	L3, 0, 0
	SFPLUT	L3, 4
	SFPNOP
	SFPADDI	16128, L3, 0
	SFPNOP
	SFPSTORE	L3, 0, 0
 #APP
# 391 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	SFPLOAD	L3, 0, 0
	SFPLUT	L3, 4
	SFPNOP
	SFPADDI	16128, L3, 0
	SFPNOP
	SFPSTORE	L3, 0, 0
 #APP
# 391 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	SFPLOAD	L3, 0, 0
	SFPLUT	L3, 4
	SFPNOP
	SFPADDI	16128, L3, 0
	SFPNOP
	SFPSTORE	L3, 0, 0
 #APP
# 391 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	SFPLOAD	L3, 0, 0
	SFPLUT	L3, 4
	SFPNOP
	SFPADDI	16128, L3, 0
	SFPNOP
	SFPSTORE	L3, 0, 0
 #APP
# 391 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	SFPLOAD	L3, 0, 0
	SFPLUT	L3, 4
	SFPNOP
	SFPADDI	16128, L3, 0
	SFPNOP
	SFPSTORE	L3, 0, 0
 #APP
# 391 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	SFPLOAD	L3, 0, 0
	SFPLUT	L3, 4
	SFPNOP
	SFPADDI	16128, L3, 0
	SFPNOP
	SFPSTORE	L3, 0, 0
 #APP
# 391 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	SFPLOAD	L3, 0, 0
	SFPLUT	L3, 4
	SFPNOP
	SFPADDI	16128, L3, 0
	SFPNOP
	SFPSTORE	L3, 0, 0
 #APP
# 391 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	ret
	.size	_ZN7ckernel4sfpu17calculate_sigmoidILb0ELm0EEEvv, .-_ZN7ckernel4sfpu17calculate_sigmoidILb0ELm0EEEvv
	.section	.text._ZN7ckernel4sfpu14calculate_tanhILb1ELm0EEEvv,"axG",@progbits,_ZN7ckernel4sfpu14calculate_tanhILb1ELm0EEEvv,comdat
	.align	1
	.weak	_ZN7ckernel4sfpu14calculate_tanhILb1ELm0EEEvv
	.type	_ZN7ckernel4sfpu14calculate_tanhILb1ELm0EEEvv, @function
_ZN7ckernel4sfpu14calculate_tanhILb1ELm0EEEvv:
	SFPLOAD	L3, 0, 0
	SFPLUT	L3, 4
	SFPNOP
	SFPSTORE	L3, 0, 0
 #APP
# 411 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	SFPLOAD	L3, 0, 0
	SFPLUT	L3, 4
	SFPNOP
	SFPSTORE	L3, 0, 0
 #APP
# 411 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	SFPLOAD	L3, 0, 0
	SFPLUT	L3, 4
	SFPNOP
	SFPSTORE	L3, 0, 0
 #APP
# 411 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	SFPLOAD	L3, 0, 0
	SFPLUT	L3, 4
	SFPNOP
	SFPSTORE	L3, 0, 0
 #APP
# 411 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	SFPLOAD	L3, 0, 0
	SFPLUT	L3, 4
	SFPNOP
	SFPSTORE	L3, 0, 0
 #APP
# 411 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	SFPLOAD	L3, 0, 0
	SFPLUT	L3, 4
	SFPNOP
	SFPSTORE	L3, 0, 0
 #APP
# 411 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	SFPLOAD	L3, 0, 0
	SFPLUT	L3, 4
	SFPNOP
	SFPSTORE	L3, 0, 0
 #APP
# 411 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	SFPLOAD	L3, 0, 0
	SFPLUT	L3, 4
	SFPNOP
	SFPSTORE	L3, 0, 0
 #APP
# 411 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	ret
	.size	_ZN7ckernel4sfpu14calculate_tanhILb1ELm0EEEvv, .-_ZN7ckernel4sfpu14calculate_tanhILb1ELm0EEEvv
	.section	.text._ZN7ckernel4sfpu18calculate_hardtanhILb0ELm0EEEvmmm,"axG",@progbits,_ZN7ckernel4sfpu18calculate_hardtanhILb0ELm0EEEvmmm,comdat
	.align	1
	.weak	_ZN7ckernel4sfpu18calculate_hardtanhILb0ELm0EEEvmmm
	.type	_ZN7ckernel4sfpu18calculate_hardtanhILb0ELm0EEEvmmm, @function
_ZN7ckernel4sfpu18calculate_hardtanhILb0ELm0EEEvmmm:
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
.L38:
	SFPLOAD	L0, 0, 0
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
	SFPSTORE	L0, 0, 0
 #APP
# 449 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	addi	a5,a5,-1
	bne	a5,zero,.L38
	ret
	.size	_ZN7ckernel4sfpu18calculate_hardtanhILb0ELm0EEEvmmm, .-_ZN7ckernel4sfpu18calculate_hardtanhILb0ELm0EEEvmmm
	.section	.text._ZN7ckernel4sfpu15calculate_lreluILb0ELm0EEEvm,"axG",@progbits,_ZN7ckernel4sfpu15calculate_lreluILb0ELm0EEEvm,comdat
	.align	1
	.weak	_ZN7ckernel4sfpu15calculate_lreluILb0ELm0EEEvm
	.type	_ZN7ckernel4sfpu15calculate_lreluILb0ELm0EEEvm, @function
_ZN7ckernel4sfpu15calculate_lreluILb0ELm0EEEvm:
	lui	a5,%hi(_ZN7ckernel13instrn_bufferE)
	lw	a5,%lo(_ZN7ckernel13instrn_bufferE)(a5)
	slli	a0,a0,16
	srli	a0,a0,16
	li	a4,1896873984
	add	a4, a4, a0
# Op(0x71) - d(1)
	sw	a4,0(a5)
	li	a5,8
.L41:
	SFPLOAD	L0, 0, 0
	SFPSETCC	0, L0, 0
	SFPMUL	L0, L1, L9, L0, 0
	SFPENCC	3, 10
	SFPSTORE	L0, 0, 0
 #APP
# 633 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	addi	a5,a5,-1
	bne	a5,zero,.L41
	ret
	.size	_ZN7ckernel4sfpu15calculate_lreluILb0ELm0EEEvm, .-_ZN7ckernel4sfpu15calculate_lreluILb0ELm0EEEvm
	.section	.text._ZN7ckernel4sfpu25calculate_tanh_derivativeILb0ELi0ELm0EEEvv,"axG",@progbits,_ZN7ckernel4sfpu25calculate_tanh_derivativeILb0ELi0ELm0EEEvv,comdat
	.align	1
	.weak	_ZN7ckernel4sfpu25calculate_tanh_derivativeILb0ELi0ELm0EEEvv
	.type	_ZN7ckernel4sfpu25calculate_tanh_derivativeILb0ELi0ELm0EEEvv, @function
_ZN7ckernel4sfpu25calculate_tanh_derivativeILb0ELi0ELm0EEEvv:
	SFPLOAD	L3, 0, 0
	SFPLUT	L3, 4
	SFPNOP
	SFPMUL	L3, L3, L9, L3, 0
	SFPNOP
	SFPMAD	L11, L3, L10, L3, 0
	SFPNOP
	SFPSTORE	L3, 0, 0
 #APP
# 475 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	SFPLOAD	L3, 0, 0
	SFPLUT	L3, 4
	SFPNOP
	SFPMUL	L3, L3, L9, L3, 0
	SFPNOP
	SFPMAD	L11, L3, L10, L3, 0
	SFPNOP
	SFPSTORE	L3, 0, 0
 #APP
# 475 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	SFPLOAD	L3, 0, 0
	SFPLUT	L3, 4
	SFPNOP
	SFPMUL	L3, L3, L9, L3, 0
	SFPNOP
	SFPMAD	L11, L3, L10, L3, 0
	SFPNOP
	SFPSTORE	L3, 0, 0
 #APP
# 475 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	SFPLOAD	L3, 0, 0
	SFPLUT	L3, 4
	SFPNOP
	SFPMUL	L3, L3, L9, L3, 0
	SFPNOP
	SFPMAD	L11, L3, L10, L3, 0
	SFPNOP
	SFPSTORE	L3, 0, 0
 #APP
# 475 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	SFPLOAD	L3, 0, 0
	SFPLUT	L3, 4
	SFPNOP
	SFPMUL	L3, L3, L9, L3, 0
	SFPNOP
	SFPMAD	L11, L3, L10, L3, 0
	SFPNOP
	SFPSTORE	L3, 0, 0
 #APP
# 475 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	SFPLOAD	L3, 0, 0
	SFPLUT	L3, 4
	SFPNOP
	SFPMUL	L3, L3, L9, L3, 0
	SFPNOP
	SFPMAD	L11, L3, L10, L3, 0
	SFPNOP
	SFPSTORE	L3, 0, 0
 #APP
# 475 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	SFPLOAD	L3, 0, 0
	SFPLUT	L3, 4
	SFPNOP
	SFPMUL	L3, L3, L9, L3, 0
	SFPNOP
	SFPMAD	L11, L3, L10, L3, 0
	SFPNOP
	SFPSTORE	L3, 0, 0
 #APP
# 475 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	SFPLOAD	L3, 0, 0
	SFPLUT	L3, 4
	SFPNOP
	SFPMUL	L3, L3, L9, L3, 0
	SFPNOP
	SFPMAD	L11, L3, L10, L3, 0
	SFPNOP
	SFPSTORE	L3, 0, 0
 #APP
# 475 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	ret
	.size	_ZN7ckernel4sfpu25calculate_tanh_derivativeILb0ELi0ELm0EEEvv, .-_ZN7ckernel4sfpu25calculate_tanh_derivativeILb0ELi0ELm0EEEvv
	.section	.text._ZN7ckernel4sfpu25calculate_tanh_derivativeILb0ELi1ELm0EEEvv,"axG",@progbits,_ZN7ckernel4sfpu25calculate_tanh_derivativeILb0ELi1ELm0EEEvv,comdat
	.align	1
	.weak	_ZN7ckernel4sfpu25calculate_tanh_derivativeILb0ELi1ELm0EEEvv
	.type	_ZN7ckernel4sfpu25calculate_tanh_derivativeILb0ELi1ELm0EEEvv, @function
_ZN7ckernel4sfpu25calculate_tanh_derivativeILb0ELi1ELm0EEEvv:
	SFPLOAD	L3, 0, 0
	SFPMUL	L3, L3, L9, L3, 0
	SFPNOP
	SFPMAD	L11, L3, L10, L3, 0
	SFPNOP
	SFPSTORE	L3, 0, 0
 #APP
# 475 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	SFPLOAD	L3, 0, 0
	SFPMUL	L3, L3, L9, L3, 0
	SFPNOP
	SFPMAD	L11, L3, L10, L3, 0
	SFPNOP
	SFPSTORE	L3, 0, 0
 #APP
# 475 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	SFPLOAD	L3, 0, 0
	SFPMUL	L3, L3, L9, L3, 0
	SFPNOP
	SFPMAD	L11, L3, L10, L3, 0
	SFPNOP
	SFPSTORE	L3, 0, 0
 #APP
# 475 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	SFPLOAD	L3, 0, 0
	SFPMUL	L3, L3, L9, L3, 0
	SFPNOP
	SFPMAD	L11, L3, L10, L3, 0
	SFPNOP
	SFPSTORE	L3, 0, 0
 #APP
# 475 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	SFPLOAD	L3, 0, 0
	SFPMUL	L3, L3, L9, L3, 0
	SFPNOP
	SFPMAD	L11, L3, L10, L3, 0
	SFPNOP
	SFPSTORE	L3, 0, 0
 #APP
# 475 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	SFPLOAD	L3, 0, 0
	SFPMUL	L3, L3, L9, L3, 0
	SFPNOP
	SFPMAD	L11, L3, L10, L3, 0
	SFPNOP
	SFPSTORE	L3, 0, 0
 #APP
# 475 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	SFPLOAD	L3, 0, 0
	SFPMUL	L3, L3, L9, L3, 0
	SFPNOP
	SFPMAD	L11, L3, L10, L3, 0
	SFPNOP
	SFPSTORE	L3, 0, 0
 #APP
# 475 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	SFPLOAD	L3, 0, 0
	SFPMUL	L3, L3, L9, L3, 0
	SFPNOP
	SFPMAD	L11, L3, L10, L3, 0
	SFPNOP
	SFPSTORE	L3, 0, 0
 #APP
# 475 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	ret
	.size	_ZN7ckernel4sfpu25calculate_tanh_derivativeILb0ELi1ELm0EEEvv, .-_ZN7ckernel4sfpu25calculate_tanh_derivativeILb0ELi1ELm0EEEvv
	.section	.text._ZN7ckernel4sfpu15calculate_powerILb1ELm0EEEvm,"axG",@progbits,_ZN7ckernel4sfpu15calculate_powerILb1ELm0EEEvm,comdat
	.align	1
	.weak	_ZN7ckernel4sfpu15calculate_powerILb1ELm0EEEvm
	.type	_ZN7ckernel4sfpu15calculate_powerILb1ELm0EEEvm, @function
_ZN7ckernel4sfpu15calculate_powerILb1ELm0EEEvm:
	li	a4,8
	li	a3,2
.L48:
	SFPLOAD	L1, 0, 0
	SFPMUL	L1, L1, L9, L0, 0
	SFPNOP
	bleu	a0,a3,.L46
	li	a5,2
.L47:
	SFPMUL	L0, L1, L9, L0, 0
	SFPNOP
	addi	a5,a5,1
	bne	a0,a5,.L47
.L46:
	SFPSTORE	L0, 0, 0
 #APP
# 650 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	addi	a4,a4,-1
	bne	a4,zero,.L48
	ret
	.size	_ZN7ckernel4sfpu15calculate_powerILb1ELm0EEEvm, .-_ZN7ckernel4sfpu15calculate_powerILb1ELm0EEEvm
	.section	.text._ZN7ckernel4sfpu14calculate_geluILb0ELm0EEEvv,"axG",@progbits,_ZN7ckernel4sfpu14calculate_geluILb0ELm0EEEvv,comdat
	.align	1
	.weak	_ZN7ckernel4sfpu14calculate_geluILb0ELm0EEEvv
	.type	_ZN7ckernel4sfpu14calculate_geluILb0ELm0EEEvv, @function
_ZN7ckernel4sfpu14calculate_geluILb0ELm0EEEvv:
	SFPLOAD	L4, 0, 0
	SFPMUL	L4, L4, L9, L3, 0
	SFPNOP
	SFPMUL	L3, L4, L9, L3, 0
	SFPLOADI	L1, 8, 15671
	SFPLOADI	L1, 10, 10003
	SFPMAD	L3, L1, L4, L3, 0
	SFPLOADI	L1, 8, 16204
	SFPLOADI	L1, 10, 419
	SFPMUL	L3, L1, L9, L3, 0
	SFPLOADI	L1, 2, 8208
	SFPLOADI	L2, 2, 65280
	SFPLUT	L3, 4
	SFPNOP
	SFPMAD	L4, L3, L4, L4, 0
	SFPNOP
	SFPMULI	16128, L4, 0
	SFPNOP
	SFPSTORE	L4, 0, 0
 #APP
# 370 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	SFPLOAD	L4, 0, 0
	SFPMUL	L4, L4, L9, L3, 0
	SFPNOP
	SFPMUL	L3, L4, L9, L3, 0
	SFPLOADI	L1, 8, 15671
	SFPLOADI	L1, 10, 10003
	SFPMAD	L3, L1, L4, L3, 0
	SFPLOADI	L1, 8, 16204
	SFPLOADI	L1, 10, 419
	SFPMUL	L3, L1, L9, L3, 0
	SFPLOADI	L1, 2, 8208
	SFPLOADI	L2, 2, 65280
	SFPLUT	L3, 4
	SFPNOP
	SFPMAD	L4, L3, L4, L4, 0
	SFPNOP
	SFPMULI	16128, L4, 0
	SFPNOP
	SFPSTORE	L4, 0, 0
 #APP
# 370 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	SFPLOAD	L4, 0, 0
	SFPMUL	L4, L4, L9, L3, 0
	SFPNOP
	SFPMUL	L3, L4, L9, L3, 0
	SFPLOADI	L1, 8, 15671
	SFPLOADI	L1, 10, 10003
	SFPMAD	L3, L1, L4, L3, 0
	SFPLOADI	L1, 8, 16204
	SFPLOADI	L1, 10, 419
	SFPMUL	L3, L1, L9, L3, 0
	SFPLOADI	L1, 2, 8208
	SFPLOADI	L2, 2, 65280
	SFPLUT	L3, 4
	SFPNOP
	SFPMAD	L4, L3, L4, L4, 0
	SFPNOP
	SFPMULI	16128, L4, 0
	SFPNOP
	SFPSTORE	L4, 0, 0
 #APP
# 370 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	SFPLOAD	L4, 0, 0
	SFPMUL	L4, L4, L9, L3, 0
	SFPNOP
	SFPMUL	L3, L4, L9, L3, 0
	SFPLOADI	L1, 8, 15671
	SFPLOADI	L1, 10, 10003
	SFPMAD	L3, L1, L4, L3, 0
	SFPLOADI	L1, 8, 16204
	SFPLOADI	L1, 10, 419
	SFPMUL	L3, L1, L9, L3, 0
	SFPLOADI	L1, 2, 8208
	SFPLOADI	L2, 2, 65280
	SFPLUT	L3, 4
	SFPNOP
	SFPMAD	L4, L3, L4, L4, 0
	SFPNOP
	SFPMULI	16128, L4, 0
	SFPNOP
	SFPSTORE	L4, 0, 0
 #APP
# 370 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	SFPLOAD	L4, 0, 0
	SFPMUL	L4, L4, L9, L3, 0
	SFPNOP
	SFPMUL	L3, L4, L9, L3, 0
	SFPLOADI	L1, 8, 15671
	SFPLOADI	L1, 10, 10003
	SFPMAD	L3, L1, L4, L3, 0
	SFPLOADI	L1, 8, 16204
	SFPLOADI	L1, 10, 419
	SFPMUL	L3, L1, L9, L3, 0
	SFPLOADI	L1, 2, 8208
	SFPLOADI	L2, 2, 65280
	SFPLUT	L3, 4
	SFPNOP
	SFPMAD	L4, L3, L4, L4, 0
	SFPNOP
	SFPMULI	16128, L4, 0
	SFPNOP
	SFPSTORE	L4, 0, 0
 #APP
# 370 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	SFPLOAD	L4, 0, 0
	SFPMUL	L4, L4, L9, L3, 0
	SFPNOP
	SFPMUL	L3, L4, L9, L3, 0
	SFPLOADI	L1, 8, 15671
	SFPLOADI	L1, 10, 10003
	SFPMAD	L3, L1, L4, L3, 0
	SFPLOADI	L1, 8, 16204
	SFPLOADI	L1, 10, 419
	SFPMUL	L3, L1, L9, L3, 0
	SFPLOADI	L1, 2, 8208
	SFPLOADI	L2, 2, 65280
	SFPLUT	L3, 4
	SFPNOP
	SFPMAD	L4, L3, L4, L4, 0
	SFPNOP
	SFPMULI	16128, L4, 0
	SFPNOP
	SFPSTORE	L4, 0, 0
 #APP
# 370 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	SFPLOAD	L4, 0, 0
	SFPMUL	L4, L4, L9, L3, 0
	SFPNOP
	SFPMUL	L3, L4, L9, L3, 0
	SFPLOADI	L1, 8, 15671
	SFPLOADI	L1, 10, 10003
	SFPMAD	L3, L1, L4, L3, 0
	SFPLOADI	L1, 8, 16204
	SFPLOADI	L1, 10, 419
	SFPMUL	L3, L1, L9, L3, 0
	SFPLOADI	L1, 2, 8208
	SFPLOADI	L2, 2, 65280
	SFPLUT	L3, 4
	SFPNOP
	SFPMAD	L4, L3, L4, L4, 0
	SFPNOP
	SFPMULI	16128, L4, 0
	SFPNOP
	SFPSTORE	L4, 0, 0
 #APP
# 370 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	SFPLOAD	L4, 0, 0
	SFPMUL	L4, L4, L9, L3, 0
	SFPNOP
	SFPMUL	L3, L4, L9, L3, 0
	SFPLOADI	L1, 8, 15671
	SFPLOADI	L1, 10, 10003
	SFPMAD	L3, L1, L4, L3, 0
	SFPLOADI	L1, 8, 16204
	SFPLOADI	L1, 10, 419
	SFPMUL	L3, L1, L9, L3, 0
	SFPLOADI	L1, 2, 8208
	SFPLOADI	L2, 2, 65280
	SFPLUT	L3, 4
	SFPNOP
	SFPMAD	L4, L3, L4, L4, 0
	SFPNOP
	SFPMULI	16128, L4, 0
	SFPNOP
	SFPSTORE	L4, 0, 0
 #APP
# 370 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	ret
	.size	_ZN7ckernel4sfpu14calculate_geluILb0ELm0EEEvv, .-_ZN7ckernel4sfpu14calculate_geluILb0ELm0EEEvv
	.section	.text._ZN7ckernel4sfpu14calculate_geluILb1ELm0EEEvv,"axG",@progbits,_ZN7ckernel4sfpu14calculate_geluILb1ELm0EEEvv,comdat
	.align	1
	.weak	_ZN7ckernel4sfpu14calculate_geluILb1ELm0EEEvv
	.type	_ZN7ckernel4sfpu14calculate_geluILb1ELm0EEEvv, @function
_ZN7ckernel4sfpu14calculate_geluILb1ELm0EEEvv:
	SFPLOAD	L4, 0, 0
	SFPLOADI	L1, 2, 8492
	SFPLOADI	L2, 2, 65280
	SFPMOV	L4, L3, 0
	SFPLUT	L3, 4
	SFPNOP
	SFPMAD	L4, L3, L4, L4, 0
	SFPNOP
	SFPMULI	16128, L4, 0
	SFPNOP
	SFPSTORE	L4, 0, 0
 #APP
# 370 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	SFPLOAD	L4, 0, 0
	SFPLOADI	L1, 2, 8492
	SFPLOADI	L2, 2, 65280
	SFPMOV	L4, L3, 0
	SFPLUT	L3, 4
	SFPNOP
	SFPMAD	L4, L3, L4, L4, 0
	SFPNOP
	SFPMULI	16128, L4, 0
	SFPNOP
	SFPSTORE	L4, 0, 0
 #APP
# 370 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	SFPLOAD	L4, 0, 0
	SFPLOADI	L1, 2, 8492
	SFPLOADI	L2, 2, 65280
	SFPMOV	L4, L3, 0
	SFPLUT	L3, 4
	SFPNOP
	SFPMAD	L4, L3, L4, L4, 0
	SFPNOP
	SFPMULI	16128, L4, 0
	SFPNOP
	SFPSTORE	L4, 0, 0
 #APP
# 370 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	SFPLOAD	L4, 0, 0
	SFPLOADI	L1, 2, 8492
	SFPLOADI	L2, 2, 65280
	SFPMOV	L4, L3, 0
	SFPLUT	L3, 4
	SFPNOP
	SFPMAD	L4, L3, L4, L4, 0
	SFPNOP
	SFPMULI	16128, L4, 0
	SFPNOP
	SFPSTORE	L4, 0, 0
 #APP
# 370 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	SFPLOAD	L4, 0, 0
	SFPLOADI	L1, 2, 8492
	SFPLOADI	L2, 2, 65280
	SFPMOV	L4, L3, 0
	SFPLUT	L3, 4
	SFPNOP
	SFPMAD	L4, L3, L4, L4, 0
	SFPNOP
	SFPMULI	16128, L4, 0
	SFPNOP
	SFPSTORE	L4, 0, 0
 #APP
# 370 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	SFPLOAD	L4, 0, 0
	SFPLOADI	L1, 2, 8492
	SFPLOADI	L2, 2, 65280
	SFPMOV	L4, L3, 0
	SFPLUT	L3, 4
	SFPNOP
	SFPMAD	L4, L3, L4, L4, 0
	SFPNOP
	SFPMULI	16128, L4, 0
	SFPNOP
	SFPSTORE	L4, 0, 0
 #APP
# 370 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	SFPLOAD	L4, 0, 0
	SFPLOADI	L1, 2, 8492
	SFPLOADI	L2, 2, 65280
	SFPMOV	L4, L3, 0
	SFPLUT	L3, 4
	SFPNOP
	SFPMAD	L4, L3, L4, L4, 0
	SFPNOP
	SFPMULI	16128, L4, 0
	SFPNOP
	SFPSTORE	L4, 0, 0
 #APP
# 370 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	SFPLOAD	L4, 0, 0
	SFPLOADI	L1, 2, 8492
	SFPLOADI	L2, 2, 65280
	SFPMOV	L4, L3, 0
	SFPLUT	L3, 4
	SFPNOP
	SFPMAD	L4, L3, L4, L4, 0
	SFPNOP
	SFPMULI	16128, L4, 0
	SFPNOP
	SFPSTORE	L4, 0, 0
 #APP
# 370 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	ret
	.size	_ZN7ckernel4sfpu14calculate_geluILb1ELm0EEEvv, .-_ZN7ckernel4sfpu14calculate_geluILb1ELm0EEEvv
	.section	.text._ZN7ckernel4sfpu25calculate_gelu_derivativeILb0ELm0EEEvv,"axG",@progbits,_ZN7ckernel4sfpu25calculate_gelu_derivativeILb0ELm0EEEvv,comdat
	.align	1
	.weak	_ZN7ckernel4sfpu25calculate_gelu_derivativeILb0ELm0EEEvv
	.type	_ZN7ckernel4sfpu25calculate_gelu_derivativeILb0ELm0EEEvv, @function
_ZN7ckernel4sfpu25calculate_gelu_derivativeILb0ELm0EEEvv:
	li	a5,8
.L54:
	SFPLOAD	L3, 0, 0
	SFPMUL	L3, L3, L9, L1, 0
	SFPNOP
	SFPMULI	48896, L1, 0
	SFPNOP
	SFPSETSGN	0, L1, L0, 1
	SFPEXEXP	L0, L2, 10
	SFPSETEXP	126, L0, L0, 1
	SFPENCC	3, 10
	SFPLOADI	L4, 0, 16221
	SFPMAD	L0, L8, L4, L4, 0
	SFPNOP
	SFPMAD	L0, L4, L10, L0, 0
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
	SFPLOADI	L4, 0, 16384
	SFPMAD	L2, L13, L4, L1, 0
	SFPNOP
	SFPMUL	L13, L1, L9, L1, 0
	SFPNOP
	SFPMAD	L2, L1, L4, L5, 0
	SFPNOP
	SFPMUL	L1, L5, L9, L1, 0
	SFPNOP
	SFPMAD	L2, L1, L4, L2, 0
	SFPNOP
	SFPMUL	L1, L2, L9, L1, 0
	SFPEXEXP	L0, L2, 0
	SFPEXEXP	L1, L4, 0
	SFPIADD	0, L4, L2, 6
	SFPPUSHC
	SFPIADD	126, L2, L2, 1
	SFPLOADI	L1, 0, 0
	SFPLOADI	L2, 4, 0
	SFPPOPC
	SFPSETEXP	0, L1, L2, 0
	SFPMOV	L2, L0, 0
	SFPENCC	3, 10
	SFPMULI	16076, L0, 0
	SFPNOP
	SFPMUL	L0, L3, L9, L4, 0
	SFPLOADI	L0, 2, 6399
	SFPLOADI	L1, 2, 8492
	SFPLOADI	L2, 2, 65280
	SFPLUT	L3, 4
	SFPLOADI	L0, 0, 16128
	SFPLOADI	L1, 0, 16128
	SFPMAD	L3, L0, L1, L3, 0
	SFPNOP
	SFPADD	L10, L4, L3, L3, 0
	SFPNOP
	SFPSTORE	L3, 0, 0
 #APP
# 499 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	addi	a5,a5,-1
	bne	a5,zero,.L54
	ret
	.size	_ZN7ckernel4sfpu25calculate_gelu_derivativeILb0ELm0EEEvv, .-_ZN7ckernel4sfpu25calculate_gelu_derivativeILb0ELm0EEEvv
	.section	.text._ZN7ckernel4sfpu14calculate_sqrtILb0ELi4ELi2ELm0EEEvv,"axG",@progbits,_ZN7ckernel4sfpu14calculate_sqrtILb0ELi4ELi2ELm0EEEvv,comdat
	.align	1
	.weak	_ZN7ckernel4sfpu14calculate_sqrtILb0ELi4ELi2ELm0EEEvv
	.type	_ZN7ckernel4sfpu14calculate_sqrtILb0ELi4ELi2ELm0EEEvv, @function
_ZN7ckernel4sfpu14calculate_sqrtILb0ELi4ELi2ELm0EEEvv:
	SFPLOAD	L3, 0, 0
	SFPSETCC	0, L3, 2
	SFPLOADI	L0, 0, 24375
	SFPMOV	L3, L1, 0
	SFPSHFT	-1, L0, L1, 1
	SFPIADD	0, L0, L1, 6
	SFPPUSHC
	SFPENCC	3, 2
	SFPMOV	L1, L2, 0
	SFPPOPC
	SFPMUL	L1, L1, L9, L1, 0
	SFPMUL	L3, L14, L9, L0, 0
	SFPLOADI	L4, 0, 16320
	SFPMAD	L1, L0, L4, L0, 0
	SFPNOP
	SFPMUL	L0, L2, L9, L1, 0
	SFPNOP
	SFPMUL	L1, L1, L9, L0, 0
	SFPMUL	L3, L14, L9, L2, 0
	SFPLOADI	L4, 0, 16320
	SFPMAD	L0, L2, L4, L0, 0
	SFPNOP
	SFPMUL	L0, L1, L9, L0, 0
	SFPNOP
	SFPMUL	L0, L3, L9, L0, 0
	SFPNOP
	SFPSTORE	L0, 0, 0
	SFPENCC	3, 10
 #APP
# 567 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	SFPLOAD	L3, 0, 0
	SFPSETCC	0, L3, 2
	SFPLOADI	L0, 0, 24375
	SFPMOV	L3, L1, 0
	SFPSHFT	-1, L0, L1, 1
	SFPIADD	0, L0, L1, 6
	SFPPUSHC
	SFPENCC	3, 2
	SFPMOV	L1, L2, 0
	SFPPOPC
	SFPMUL	L1, L1, L9, L1, 0
	SFPMUL	L3, L14, L9, L0, 0
	SFPLOADI	L4, 0, 16320
	SFPMAD	L1, L0, L4, L0, 0
	SFPNOP
	SFPMUL	L0, L2, L9, L1, 0
	SFPNOP
	SFPMUL	L1, L1, L9, L0, 0
	SFPMUL	L3, L14, L9, L2, 0
	SFPLOADI	L4, 0, 16320
	SFPMAD	L0, L2, L4, L0, 0
	SFPNOP
	SFPMUL	L0, L1, L9, L0, 0
	SFPNOP
	SFPMUL	L0, L3, L9, L0, 0
	SFPNOP
	SFPSTORE	L0, 0, 0
	SFPENCC	3, 10
 #APP
# 567 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	SFPLOAD	L3, 0, 0
	SFPSETCC	0, L3, 2
	SFPLOADI	L0, 0, 24375
	SFPMOV	L3, L1, 0
	SFPSHFT	-1, L0, L1, 1
	SFPIADD	0, L0, L1, 6
	SFPPUSHC
	SFPENCC	3, 2
	SFPMOV	L1, L2, 0
	SFPPOPC
	SFPMUL	L1, L1, L9, L1, 0
	SFPMUL	L3, L14, L9, L0, 0
	SFPLOADI	L4, 0, 16320
	SFPMAD	L1, L0, L4, L0, 0
	SFPNOP
	SFPMUL	L0, L2, L9, L1, 0
	SFPNOP
	SFPMUL	L1, L1, L9, L0, 0
	SFPMUL	L3, L14, L9, L2, 0
	SFPLOADI	L4, 0, 16320
	SFPMAD	L0, L2, L4, L0, 0
	SFPNOP
	SFPMUL	L0, L1, L9, L0, 0
	SFPNOP
	SFPMUL	L0, L3, L9, L0, 0
	SFPNOP
	SFPSTORE	L0, 0, 0
	SFPENCC	3, 10
 #APP
# 567 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	SFPLOAD	L3, 0, 0
	SFPSETCC	0, L3, 2
	SFPLOADI	L0, 0, 24375
	SFPMOV	L3, L1, 0
	SFPSHFT	-1, L0, L1, 1
	SFPIADD	0, L0, L1, 6
	SFPPUSHC
	SFPENCC	3, 2
	SFPMOV	L1, L2, 0
	SFPPOPC
	SFPMUL	L1, L1, L9, L1, 0
	SFPMUL	L3, L14, L9, L0, 0
	SFPLOADI	L4, 0, 16320
	SFPMAD	L1, L0, L4, L0, 0
	SFPNOP
	SFPMUL	L0, L2, L9, L1, 0
	SFPNOP
	SFPMUL	L1, L1, L9, L0, 0
	SFPMUL	L3, L14, L9, L2, 0
	SFPLOADI	L4, 0, 16320
	SFPMAD	L0, L2, L4, L0, 0
	SFPNOP
	SFPMUL	L0, L1, L9, L0, 0
	SFPNOP
	SFPMUL	L0, L3, L9, L0, 0
	SFPNOP
	SFPSTORE	L0, 0, 0
	SFPENCC	3, 10
 #APP
# 567 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	SFPLOAD	L3, 0, 0
	SFPSETCC	0, L3, 2
	SFPLOADI	L0, 0, 24375
	SFPMOV	L3, L1, 0
	SFPSHFT	-1, L0, L1, 1
	SFPIADD	0, L0, L1, 6
	SFPPUSHC
	SFPENCC	3, 2
	SFPMOV	L1, L2, 0
	SFPPOPC
	SFPMUL	L1, L1, L9, L1, 0
	SFPMUL	L3, L14, L9, L0, 0
	SFPLOADI	L4, 0, 16320
	SFPMAD	L1, L0, L4, L0, 0
	SFPNOP
	SFPMUL	L0, L2, L9, L1, 0
	SFPNOP
	SFPMUL	L1, L1, L9, L0, 0
	SFPMUL	L3, L14, L9, L2, 0
	SFPLOADI	L4, 0, 16320
	SFPMAD	L0, L2, L4, L0, 0
	SFPNOP
	SFPMUL	L0, L1, L9, L0, 0
	SFPNOP
	SFPMUL	L0, L3, L9, L0, 0
	SFPNOP
	SFPSTORE	L0, 0, 0
	SFPENCC	3, 10
 #APP
# 567 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	SFPLOAD	L3, 0, 0
	SFPSETCC	0, L3, 2
	SFPLOADI	L0, 0, 24375
	SFPMOV	L3, L1, 0
	SFPSHFT	-1, L0, L1, 1
	SFPIADD	0, L0, L1, 6
	SFPPUSHC
	SFPENCC	3, 2
	SFPMOV	L1, L2, 0
	SFPPOPC
	SFPMUL	L1, L1, L9, L1, 0
	SFPMUL	L3, L14, L9, L0, 0
	SFPLOADI	L4, 0, 16320
	SFPMAD	L1, L0, L4, L0, 0
	SFPNOP
	SFPMUL	L0, L2, L9, L1, 0
	SFPNOP
	SFPMUL	L1, L1, L9, L0, 0
	SFPMUL	L3, L14, L9, L2, 0
	SFPLOADI	L4, 0, 16320
	SFPMAD	L0, L2, L4, L0, 0
	SFPNOP
	SFPMUL	L0, L1, L9, L0, 0
	SFPNOP
	SFPMUL	L0, L3, L9, L0, 0
	SFPNOP
	SFPSTORE	L0, 0, 0
	SFPENCC	3, 10
 #APP
# 567 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	SFPLOAD	L3, 0, 0
	SFPSETCC	0, L3, 2
	SFPLOADI	L0, 0, 24375
	SFPMOV	L3, L1, 0
	SFPSHFT	-1, L0, L1, 1
	SFPIADD	0, L0, L1, 6
	SFPPUSHC
	SFPENCC	3, 2
	SFPMOV	L1, L2, 0
	SFPPOPC
	SFPMUL	L1, L1, L9, L1, 0
	SFPMUL	L3, L14, L9, L0, 0
	SFPLOADI	L4, 0, 16320
	SFPMAD	L1, L0, L4, L0, 0
	SFPNOP
	SFPMUL	L0, L2, L9, L1, 0
	SFPNOP
	SFPMUL	L1, L1, L9, L0, 0
	SFPMUL	L3, L14, L9, L2, 0
	SFPLOADI	L4, 0, 16320
	SFPMAD	L0, L2, L4, L0, 0
	SFPNOP
	SFPMUL	L0, L1, L9, L0, 0
	SFPNOP
	SFPMUL	L0, L3, L9, L0, 0
	SFPNOP
	SFPSTORE	L0, 0, 0
	SFPENCC	3, 10
 #APP
# 567 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	SFPLOAD	L3, 0, 0
	SFPSETCC	0, L3, 2
	SFPLOADI	L0, 0, 24375
	SFPMOV	L3, L1, 0
	SFPSHFT	-1, L0, L1, 1
	SFPIADD	0, L0, L1, 6
	SFPPUSHC
	SFPENCC	3, 2
	SFPMOV	L1, L2, 0
	SFPPOPC
	SFPMUL	L1, L1, L9, L1, 0
	SFPMUL	L3, L14, L9, L0, 0
	SFPLOADI	L4, 0, 16320
	SFPMAD	L1, L0, L4, L0, 0
	SFPNOP
	SFPMUL	L0, L2, L9, L1, 0
	SFPNOP
	SFPMUL	L1, L1, L9, L0, 0
	SFPMUL	L3, L14, L9, L2, 0
	SFPLOADI	L4, 0, 16320
	SFPMAD	L0, L2, L4, L0, 0
	SFPNOP
	SFPMUL	L0, L1, L9, L0, 0
	SFPNOP
	SFPMUL	L0, L3, L9, L0, 0
	SFPNOP
	SFPSTORE	L0, 0, 0
	SFPENCC	3, 10
 #APP
# 567 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	ret
	.size	_ZN7ckernel4sfpu14calculate_sqrtILb0ELi4ELi2ELm0EEEvv, .-_ZN7ckernel4sfpu14calculate_sqrtILb0ELi4ELi2ELm0EEEvv
	.section	.text._ZN7ckernel4sfpu14calculate_sqrtILb1ELi4ELi2ELm0EEEvv,"axG",@progbits,_ZN7ckernel4sfpu14calculate_sqrtILb1ELi4ELi2ELm0EEEvv,comdat
	.align	1
	.weak	_ZN7ckernel4sfpu14calculate_sqrtILb1ELi4ELi2ELm0EEEvv
	.type	_ZN7ckernel4sfpu14calculate_sqrtILb1ELi4ELi2ELm0EEEvv, @function
_ZN7ckernel4sfpu14calculate_sqrtILb1ELi4ELi2ELm0EEEvv:
	SFPLOAD	L0, 0, 0
	SFPIADD	0, L2, L0, 4
	SFPSHFT	-1, L0, L0, 1
	SFPSTORE	L0, 0, 0
 #APP
# 567 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	SFPLOAD	L0, 0, 0
	SFPIADD	0, L2, L0, 4
	SFPSHFT	-1, L0, L0, 1
	SFPSTORE	L0, 0, 0
 #APP
# 567 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	SFPLOAD	L0, 0, 0
	SFPIADD	0, L2, L0, 4
	SFPSHFT	-1, L0, L0, 1
	SFPSTORE	L0, 0, 0
 #APP
# 567 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	SFPLOAD	L0, 0, 0
	SFPIADD	0, L2, L0, 4
	SFPSHFT	-1, L0, L0, 1
	SFPSTORE	L0, 0, 0
 #APP
# 567 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	SFPLOAD	L0, 0, 0
	SFPIADD	0, L2, L0, 4
	SFPSHFT	-1, L0, L0, 1
	SFPSTORE	L0, 0, 0
 #APP
# 567 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	SFPLOAD	L0, 0, 0
	SFPIADD	0, L2, L0, 4
	SFPSHFT	-1, L0, L0, 1
	SFPSTORE	L0, 0, 0
 #APP
# 567 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	SFPLOAD	L0, 0, 0
	SFPIADD	0, L2, L0, 4
	SFPSHFT	-1, L0, L0, 1
	SFPSTORE	L0, 0, 0
 #APP
# 567 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	SFPLOAD	L0, 0, 0
	SFPIADD	0, L2, L0, 4
	SFPSHFT	-1, L0, L0, 1
	SFPSTORE	L0, 0, 0
 #APP
# 567 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	ret
	.size	_ZN7ckernel4sfpu14calculate_sqrtILb1ELi4ELi2ELm0EEEvv, .-_ZN7ckernel4sfpu14calculate_sqrtILb1ELi4ELi2ELm0EEEvv
	.section	.text._ZN7ckernel4sfpu21calculate_exponentialILb0ELb0ELb1ELm0EEEvt,"axG",@progbits,_ZN7ckernel4sfpu21calculate_exponentialILb0ELb0ELb1ELm0EEEvt,comdat
	.align	1
	.weak	_ZN7ckernel4sfpu21calculate_exponentialILb0ELb0ELb1ELm0EEEvt
	.type	_ZN7ckernel4sfpu21calculate_exponentialILb0ELb0ELb1ELm0EEEvt, @function
_ZN7ckernel4sfpu21calculate_exponentialILb0ELb0ELb1ELm0EEEvt:
	li	a5,8
	lui	a4,%hi(_ZN7ckernel13instrn_bufferE)
.L59:
	SFPLOAD	L1, 0, 0
	lw	a3,%lo(_ZN7ckernel13instrn_bufferE)(a4)
	li	a2,1895890944
	add	a2, a2, a0
# Op(0x71) - d(0)
	sw	a2,0(a3)
	SFPMUL	L1, L0, L9, L1, 0
	SFPNOP
	SFPSTORE	L1, 0, 0
	SFPSETSGN	0, L1, L0, 1
	SFPEXEXP	L0, L2, 10
	SFPSETEXP	126, L0, L0, 1
	SFPENCC	3, 10
	SFPLOADI	L3, 0, 16221
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
	SFPLOADI	L3, 0, 16384
	SFPMAD	L2, L13, L3, L1, 0
	SFPNOP
	SFPMUL	L13, L1, L9, L1, 0
	SFPNOP
	SFPMAD	L2, L1, L3, L4, 0
	SFPNOP
	SFPMUL	L1, L4, L9, L1, 0
	SFPNOP
	SFPMAD	L2, L1, L3, L2, 0
	SFPNOP
	SFPMUL	L1, L2, L9, L1, 0
	SFPEXEXP	L0, L2, 0
	SFPEXEXP	L1, L3, 0
	SFPIADD	0, L3, L2, 6
	SFPPUSHC
	SFPIADD	126, L2, L2, 1
	SFPLOADI	L1, 0, 0
	SFPLOADI	L2, 4, 0
	SFPPOPC
	SFPSETEXP	0, L1, L2, 0
	SFPMOV	L2, L0, 0
	SFPENCC	3, 10
	SFPSTORE	L0, 0, 0
 #APP
# 296 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	addi	a5,a5,-1
	bne	a5,zero,.L59
	ret
	.size	_ZN7ckernel4sfpu21calculate_exponentialILb0ELb0ELb1ELm0EEEvt, .-_ZN7ckernel4sfpu21calculate_exponentialILb0ELb0ELb1ELm0EEEvt
	.section	.text._ZN7ckernel4sfpu21calculate_exponentialILb0ELb1ELb1ELm0EEEvt,"axG",@progbits,_ZN7ckernel4sfpu21calculate_exponentialILb0ELb1ELb1ELm0EEEvt,comdat
	.align	1
	.weak	_ZN7ckernel4sfpu21calculate_exponentialILb0ELb1ELb1ELm0EEEvt
	.type	_ZN7ckernel4sfpu21calculate_exponentialILb0ELb1ELb1ELm0EEEvt, @function
_ZN7ckernel4sfpu21calculate_exponentialILb0ELb1ELb1ELm0EEEvt:
	li	a5,8
	lui	a4,%hi(_ZN7ckernel13instrn_bufferE)
.L62:
	SFPLOAD	L1, 0, 0
	lw	a3,%lo(_ZN7ckernel13instrn_bufferE)(a4)
	li	a2,1895890944
	add	a2, a2, a0
# Op(0x71) - d(0)
	sw	a2,0(a3)
	SFPMUL	L1, L0, L9, L1, 0
	SFPNOP
	SFPSTORE	L1, 0, 0
	SFPSETSGN	0, L1, L0, 1
	SFPEXEXP	L0, L2, 10
	SFPSETEXP	126, L0, L0, 1
	SFPENCC	3, 10
	SFPLOADI	L3, 0, 16221
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
	SFPLOADI	L3, 0, 16384
	SFPMAD	L2, L13, L3, L1, 0
	SFPNOP
	SFPMUL	L13, L1, L9, L1, 0
	SFPNOP
	SFPMAD	L2, L1, L3, L4, 0
	SFPNOP
	SFPMUL	L1, L4, L9, L1, 0
	SFPNOP
	SFPMAD	L2, L1, L3, L2, 0
	SFPNOP
	SFPMUL	L1, L2, L9, L1, 0
	SFPEXEXP	L0, L2, 0
	SFPEXEXP	L1, L3, 0
	SFPIADD	0, L3, L2, 6
	SFPPUSHC
	SFPIADD	126, L2, L2, 1
	SFPLOADI	L1, 0, 0
	SFPLOADI	L2, 4, 0
	SFPPOPC
	SFPSETEXP	0, L1, L2, 0
	SFPMOV	L2, L0, 0
	SFPENCC	3, 10
	SFPSTORE	L0, 0, 0
 #APP
# 296 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	addi	a5,a5,-1
	bne	a5,zero,.L62
	ret
	.size	_ZN7ckernel4sfpu21calculate_exponentialILb0ELb1ELb1ELm0EEEvt, .-_ZN7ckernel4sfpu21calculate_exponentialILb0ELb1ELb1ELm0EEEvt
	.section	.text._ZN7ckernel4sfpu21calculate_exponentialILb1ELb0ELb1ELm0EEEvt,"axG",@progbits,_ZN7ckernel4sfpu21calculate_exponentialILb1ELb0ELb1ELm0EEEvt,comdat
	.align	1
	.weak	_ZN7ckernel4sfpu21calculate_exponentialILb1ELb0ELb1ELm0EEEvt
	.type	_ZN7ckernel4sfpu21calculate_exponentialILb1ELb0ELb1ELm0EEEvt, @function
_ZN7ckernel4sfpu21calculate_exponentialILb1ELb0ELb1ELm0EEEvt:
	li	a5,8
	lui	a3,%hi(_ZN7ckernel13instrn_bufferE)
.L65:
	SFPLOAD	L1, 0, 0
	lw	a4,%lo(_ZN7ckernel13instrn_bufferE)(a3)
	li	a2,1899036672
	add	a2, a2, a0
# Op(0x71) - d(3)
	sw	a2,0(a4)
	SFPMUL	L1, L3, L9, L1, 0
	SFPNOP
	SFPSTORE	L1, 0, 0
	SFPMAD	L1, L13, L0, L1, 0
	SFPNOP
	SFPIADD	0, L2, L1, 4
	SFPSHFT	8, L0, L1, 1
	SFPSTORE	L1, 0, 0
 #APP
# 296 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	addi	a5,a5,-1
	bne	a5,zero,.L65
	ret
	.size	_ZN7ckernel4sfpu21calculate_exponentialILb1ELb0ELb1ELm0EEEvt, .-_ZN7ckernel4sfpu21calculate_exponentialILb1ELb0ELb1ELm0EEEvt
	.section	.text._ZN7ckernel4sfpu21calculate_exponentialILb1ELb1ELb1ELm0EEEvt,"axG",@progbits,_ZN7ckernel4sfpu21calculate_exponentialILb1ELb1ELb1ELm0EEEvt,comdat
	.align	1
	.weak	_ZN7ckernel4sfpu21calculate_exponentialILb1ELb1ELb1ELm0EEEvt
	.type	_ZN7ckernel4sfpu21calculate_exponentialILb1ELb1ELb1ELm0EEEvt, @function
_ZN7ckernel4sfpu21calculate_exponentialILb1ELb1ELb1ELm0EEEvt:
	li	a5,8
	lui	a4,%hi(_ZN7ckernel13instrn_bufferE)
.L68:
	SFPLOAD	L1, 0, 0
	lw	a3,%lo(_ZN7ckernel13instrn_bufferE)(a4)
	li	a2,1899036672
	add	a2, a2, a0
# Op(0x71) - d(3)
	sw	a2,0(a3)
	SFPMUL	L1, L3, L9, L1, 0
	SFPNOP
	SFPSTORE	L1, 0, 0
	SFPMAD	L1, L13, L0, L1, 0
	SFPNOP
	SFPIADD	0, L2, L1, 4
	SFPSHFT	8, L0, L1, 1
	SFPSTORE	L1, 0, 0
	SFPSETCC	0, L1, 0
	SFPSTORE	L9, 0, 0
	SFPENCC	3, 10
 #APP
# 296 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	addi	a5,a5,-1
	bne	a5,zero,.L68
	ret
	.size	_ZN7ckernel4sfpu21calculate_exponentialILb1ELb1ELb1ELm0EEEvt, .-_ZN7ckernel4sfpu21calculate_exponentialILb1ELb1ELb1ELm0EEEvt
	.section	.text._ZN7ckernel4sfpu20calculate_reciprocalILb0ELi4ELm0EEEvv,"axG",@progbits,_ZN7ckernel4sfpu20calculate_reciprocalILb0ELi4ELm0EEEvv,comdat
	.align	1
	.weak	_ZN7ckernel4sfpu20calculate_reciprocalILb0ELi4ELm0EEEvv
	.type	_ZN7ckernel4sfpu20calculate_reciprocalILb0ELi4ELm0EEEvv, @function
_ZN7ckernel4sfpu20calculate_reciprocalILb0ELi4ELm0EEEvv:
	li	a5,4
.L71:
	SFPLOAD	L2, 0, 0
	SFPSETSGN	1, L2, L0, 1
	SFPSETEXP	126, L0, L0, 1
	SFPLOADI	L3, 0, 16384
	SFPMAD	L0, L13, L3, L1, 0
	SFPNOP
	SFPMUL	L13, L1, L9, L1, 0
	SFPNOP
	SFPMAD	L0, L1, L3, L4, 0
	SFPNOP
	SFPMUL	L1, L4, L9, L1, 0
	SFPNOP
	SFPMAD	L0, L1, L3, L0, 0
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
	SFPSTORE	L0, 0, 0
 #APP
# 520 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	addi	a5,a5,-1
	bne	a5,zero,.L71
	ret
	.size	_ZN7ckernel4sfpu20calculate_reciprocalILb0ELi4ELm0EEEvv, .-_ZN7ckernel4sfpu20calculate_reciprocalILb0ELi4ELm0EEEvv
	.section	.text._ZN7ckernel4sfpu20calculate_reciprocalILb1ELi4ELm0EEEvv,"axG",@progbits,_ZN7ckernel4sfpu20calculate_reciprocalILb1ELi4ELm0EEEvv,comdat
	.align	1
	.weak	_ZN7ckernel4sfpu20calculate_reciprocalILb1ELi4ELm0EEEvv
	.type	_ZN7ckernel4sfpu20calculate_reciprocalILb1ELi4ELm0EEEvv, @function
_ZN7ckernel4sfpu20calculate_reciprocalILb1ELi4ELm0EEEvv:
	li	a5,4
.L74:
	SFPLOAD	L2, 0, 0
	SFPSETSGN	1, L2, L0, 1
	SFPSETEXP	126, L0, L0, 1
	SFPLOADI	L3, 0, 16384
	SFPMAD	L0, L13, L3, L1, 0
	SFPNOP
	SFPMUL	L13, L1, L9, L1, 0
	SFPNOP
	SFPMAD	L0, L1, L3, L0, 0
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
	SFPSTORE	L0, 0, 0
 #APP
# 520 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	addi	a5,a5,-1
	bne	a5,zero,.L74
	ret
	.size	_ZN7ckernel4sfpu20calculate_reciprocalILb1ELi4ELm0EEEvv, .-_ZN7ckernel4sfpu20calculate_reciprocalILb1ELi4ELm0EEEvv
	.section	.text._ZN7ckernel4sfpu17calculate_dropoutILb0ELm0EEEvmm,"axG",@progbits,_ZN7ckernel4sfpu17calculate_dropoutILb0ELm0EEEvmm,comdat
	.align	1
	.weak	_ZN7ckernel4sfpu17calculate_dropoutILb0ELm0EEEvmm
	.type	_ZN7ckernel4sfpu17calculate_dropoutILb0ELm0EEEvmm, @function
_ZN7ckernel4sfpu17calculate_dropoutILb0ELm0EEEvmm:
	SFPLOADI	L1, 2, 46080
	slli	a1,a1,16
	srli	a3,a0,16
	slli	a0,a0,16
	li	a5,8
	lui	a2,%hi(_ZN7ckernel13instrn_bufferE)
	srli	a1,a1,8
	srli	a0,a0,16
.L77:
	lw	a4,%lo(_ZN7ckernel13instrn_bufferE)(a2)
	SFPLOAD	L0, 0, 0
	li	a6,1946157056
	add	a6, a6, a1
# Op(0x74) lv(0)  d(0)
	sw	a6,0(a4)
	SFPNOP
	SFPNOP
	SFPNOP
	SFPSTORE	L0, 0, 0
	li	a6,1896480768
	add	a6, a6, a0
# Op(0x71) - d(0)
	sw	a6,0(a4)
	li	a6,1896349696
	add	a6, a6, a3
# Op(0x71) lv(0)  d(0)
	sw	a6,0(a4)
	SFPIADD	0, L3, L0, 2
	SFPSTORE	L9, 0, 0
	SFPENCC	3, 10
	SFPMOV	L7, L0, 0
	SFPAND	L3, L0
	SFPSHFT	-1, L0, L3, 1
	SFPSETCC	0, L0, 2
	SFPMOV	L1, L0, 0
	SFPAND	L3, L0
	SFPNOT	L0, L0
	SFPPUSHC
	SFPENCC	3, 2
	SFPMOV	L1, L2, 0
	SFPPOPC
	SFPOR	L3, L2
	SFPMOV	L2, L3, 0
	SFPAND	L3, L0
	SFPMOV	L0, L3, 0
	SFPENCC	3, 10
 #APP
# 612 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	addi	a5,a5,-1
	bne	a5,zero,.L77
	ret
	.size	_ZN7ckernel4sfpu17calculate_dropoutILb0ELm0EEEvmm, .-_ZN7ckernel4sfpu17calculate_dropoutILb0ELm0EEEvmm
	.section	.text._ZN7ckernel4sfpu18calculate_log_bodyILb1ELm0EEEvm,"axG",@progbits,_ZN7ckernel4sfpu18calculate_log_bodyILb1ELm0EEEvm,comdat
	.align	1
	.weak	_ZN7ckernel4sfpu18calculate_log_bodyILb1ELm0EEEvm
	.type	_ZN7ckernel4sfpu18calculate_log_bodyILb1ELm0EEEvm, @function
_ZN7ckernel4sfpu18calculate_log_bodyILb1ELm0EEEvm:
	SFPLOAD	L2, 0, 0
	SFPSETEXP	127, L2, L0, 1
	SFPLOADI	L1, 8, 15832
	SFPLOADI	L1, 10, 44460
	SFPLOADI	L3, 8, 48950
	SFPLOADI	L3, 10, 11115
	SFPMAD	L0, L1, L3, L1, 0
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
	lui	a5,%hi(_ZN7ckernel13instrn_bufferE)
	lw	a5,%lo(_ZN7ckernel13instrn_bufferE)(a5)
	slli	a0,a0,16
	srli	a0,a0,16
	li	a4,1896939520
	add	a4, a4, a0
# Op(0x71) - d(1)
	sw	a4,0(a5)
	SFPMUL	L0, L1, L9, L0, 0
	SFPSETCC	0, L2, 6
	SFPLOADI	L0, 0, 65408
	SFPENCC	3, 10
	SFPSTORE	L0, 0, 0
	ret
	.size	_ZN7ckernel4sfpu18calculate_log_bodyILb1ELm0EEEvm, .-_ZN7ckernel4sfpu18calculate_log_bodyILb1ELm0EEEvm
	.section	.text._ZN7ckernel4sfpu13calculate_logILb1ELb1ELm0EEEvm,"axG",@progbits,_ZN7ckernel4sfpu13calculate_logILb1ELb1ELm0EEEvm,comdat
	.align	1
	.weak	_ZN7ckernel4sfpu13calculate_logILb1ELb1ELm0EEEvm
	.type	_ZN7ckernel4sfpu13calculate_logILb1ELb1ELm0EEEvm, @function
_ZN7ckernel4sfpu13calculate_logILb1ELb1ELm0EEEvm:
	addi	sp,sp,-16
	sw	s0,8(sp)
	sw	s1,4(sp)
	sw	ra,12(sp)
	mv	s1,a0
	li	s0,8
.L81:
	mv	a0,s1
	call	_ZN7ckernel4sfpu18calculate_log_bodyILb1ELm0EEEvm
 #APP
# 713 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	addi	s0,s0,-1
	bne	s0,zero,.L81
	lw	ra,12(sp)
	lw	s0,8(sp)
	lw	s1,4(sp)
	addi	sp,sp,16
	jr	ra
	.size	_ZN7ckernel4sfpu13calculate_logILb1ELb1ELm0EEEvm, .-_ZN7ckernel4sfpu13calculate_logILb1ELb1ELm0EEEvm
	.section	.text.startup,"ax",@progbits
	.align	1
	.globl	_ZN7ckernel4sfpu4mainEiPPc
	.type	_ZN7ckernel4sfpu4mainEiPPc, @function
_ZN7ckernel4sfpu4mainEiPPc:
	addi	sp,sp,-16
	sw	s0,8(sp)
	mv	s0,a0
	sw	ra,12(sp)
	SFPLOAD	L0, 0, 0
	SFPSETCC	0, L0, 0
	SFPLOADI	L0, 0, 0
	SFPCOMPC
	SFPLOADI	L0, 0, 16256
	SFPENCC	3, 10
	SFPSTORE	L0, 0, 0
 #APP
# 796 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	SFPLOAD	L0, 0, 0
	SFPSETCC	0, L0, 0
	SFPLOADI	L0, 0, 0
	SFPCOMPC
	SFPLOADI	L0, 0, 16256
	SFPENCC	3, 10
	SFPSTORE	L0, 0, 0
 #APP
# 796 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	SFPLOAD	L0, 0, 0
	SFPSETCC	0, L0, 0
	SFPLOADI	L0, 0, 0
	SFPCOMPC
	SFPLOADI	L0, 0, 16256
	SFPENCC	3, 10
	SFPSTORE	L0, 0, 0
 #APP
# 796 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	SFPLOAD	L0, 0, 0
	SFPSETCC	0, L0, 0
	SFPLOADI	L0, 0, 0
	SFPCOMPC
	SFPLOADI	L0, 0, 16256
	SFPENCC	3, 10
	SFPSTORE	L0, 0, 0
 #APP
# 796 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	SFPLOAD	L0, 0, 0
	SFPSETCC	0, L0, 0
	SFPLOADI	L0, 0, 0
	SFPCOMPC
	SFPLOADI	L0, 0, 16256
	SFPENCC	3, 10
	SFPSTORE	L0, 0, 0
 #APP
# 796 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	SFPLOAD	L0, 0, 0
	SFPSETCC	0, L0, 0
	SFPLOADI	L0, 0, 0
	SFPCOMPC
	SFPLOADI	L0, 0, 16256
	SFPENCC	3, 10
	SFPSTORE	L0, 0, 0
 #APP
# 796 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	SFPLOAD	L0, 0, 0
	SFPSETCC	0, L0, 0
	SFPLOADI	L0, 0, 0
	SFPCOMPC
	SFPLOADI	L0, 0, 16256
	SFPENCC	3, 10
	SFPSTORE	L0, 0, 0
 #APP
# 796 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	SFPLOAD	L0, 0, 0
	SFPSETCC	0, L0, 0
	SFPLOADI	L0, 0, 0
	SFPCOMPC
	SFPLOADI	L0, 0, 16256
	SFPENCC	3, 10
	SFPSTORE	L0, 0, 0
 #APP
# 796 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	call	_ZN7ckernel4sfpu14calculate_compILb0EL8SfpuType8ELm0EEEvm
	SFPLOAD	L0, 0, 0
	SFPSETCC	0, L0, 0
	SFPLOADI	L0, 0, 0
	SFPLOADI	L1, 0, 0
	SFPCOMPC
	SFPLOADI	L0, 0, 16256
	SFPLOADI	L1, 0, 16256
	SFPENCC	3, 10
	SFPAND	L1, L0
	SFPSTORE	L0, 0, 0
 #APP
# 796 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	SFPLOAD	L0, 0, 0
	SFPSETCC	0, L0, 0
	SFPLOADI	L0, 0, 0
	SFPLOADI	L1, 0, 0
	SFPCOMPC
	SFPLOADI	L0, 0, 16256
	SFPLOADI	L1, 0, 16256
	SFPENCC	3, 10
	SFPAND	L1, L0
	SFPSTORE	L0, 0, 0
 #APP
# 796 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	SFPLOAD	L0, 0, 0
	SFPSETCC	0, L0, 0
	SFPLOADI	L0, 0, 0
	SFPLOADI	L1, 0, 0
	SFPCOMPC
	SFPLOADI	L0, 0, 16256
	SFPLOADI	L1, 0, 16256
	SFPENCC	3, 10
	SFPAND	L1, L0
	SFPSTORE	L0, 0, 0
 #APP
# 796 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	SFPLOAD	L0, 0, 0
	SFPSETCC	0, L0, 0
	SFPLOADI	L0, 0, 0
	SFPLOADI	L1, 0, 0
	SFPCOMPC
	SFPLOADI	L0, 0, 16256
	SFPLOADI	L1, 0, 16256
	SFPENCC	3, 10
	SFPAND	L1, L0
	SFPSTORE	L0, 0, 0
 #APP
# 796 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	SFPLOAD	L0, 0, 0
	SFPSETCC	0, L0, 0
	SFPLOADI	L0, 0, 0
	SFPLOADI	L1, 0, 0
	SFPCOMPC
	SFPLOADI	L0, 0, 16256
	SFPLOADI	L1, 0, 16256
	SFPENCC	3, 10
	SFPAND	L1, L0
	SFPSTORE	L0, 0, 0
 #APP
# 796 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	SFPLOAD	L0, 0, 0
	SFPSETCC	0, L0, 0
	SFPLOADI	L0, 0, 0
	SFPLOADI	L1, 0, 0
	SFPCOMPC
	SFPLOADI	L0, 0, 16256
	SFPLOADI	L1, 0, 16256
	SFPENCC	3, 10
	SFPAND	L1, L0
	SFPSTORE	L0, 0, 0
 #APP
# 796 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	SFPLOAD	L0, 0, 0
	SFPSETCC	0, L0, 0
	SFPLOADI	L0, 0, 0
	SFPLOADI	L1, 0, 0
	SFPCOMPC
	SFPLOADI	L0, 0, 16256
	SFPLOADI	L1, 0, 16256
	SFPENCC	3, 10
	SFPAND	L1, L0
	SFPSTORE	L0, 0, 0
 #APP
# 796 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	SFPLOAD	L0, 0, 0
	SFPSETCC	0, L0, 0
	SFPLOADI	L0, 0, 0
	SFPLOADI	L1, 0, 0
	SFPCOMPC
	SFPLOADI	L0, 0, 16256
	SFPLOADI	L1, 0, 16256
	SFPENCC	3, 10
	SFPAND	L1, L0
	SFPSTORE	L0, 0, 0
 #APP
# 796 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	SFPLOAD	L0, 0, 0
	SFPSETCC	0, L0, 0
	SFPLOADI	L0, 0, 16256
	SFPLOADI	L1, 0, 16256
	SFPCOMPC
	SFPLOADI	L0, 0, 0
	SFPLOADI	L1, 0, 0
	SFPENCC	3, 10
	SFPOR	L1, L0
	SFPSTORE	L0, 0, 0
 #APP
# 796 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	SFPLOAD	L0, 0, 0
	SFPSETCC	0, L0, 0
	SFPLOADI	L0, 0, 16256
	SFPLOADI	L1, 0, 16256
	SFPCOMPC
	SFPLOADI	L0, 0, 0
	SFPLOADI	L1, 0, 0
	SFPENCC	3, 10
	SFPOR	L1, L0
	SFPSTORE	L0, 0, 0
 #APP
# 796 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	SFPLOAD	L0, 0, 0
	SFPSETCC	0, L0, 0
	SFPLOADI	L0, 0, 16256
	SFPLOADI	L1, 0, 16256
	SFPCOMPC
	SFPLOADI	L0, 0, 0
	SFPLOADI	L1, 0, 0
	SFPENCC	3, 10
	SFPOR	L1, L0
	SFPSTORE	L0, 0, 0
 #APP
# 796 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	SFPLOAD	L0, 0, 0
	SFPSETCC	0, L0, 0
	SFPLOADI	L0, 0, 16256
	SFPLOADI	L1, 0, 16256
	SFPCOMPC
	SFPLOADI	L0, 0, 0
	SFPLOADI	L1, 0, 0
	SFPENCC	3, 10
	SFPOR	L1, L0
	SFPSTORE	L0, 0, 0
 #APP
# 796 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	SFPLOAD	L0, 0, 0
	SFPSETCC	0, L0, 0
	SFPLOADI	L0, 0, 16256
	SFPLOADI	L1, 0, 16256
	SFPCOMPC
	SFPLOADI	L0, 0, 0
	SFPLOADI	L1, 0, 0
	SFPENCC	3, 10
	SFPOR	L1, L0
	SFPSTORE	L0, 0, 0
 #APP
# 796 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	SFPLOAD	L0, 0, 0
	SFPSETCC	0, L0, 0
	SFPLOADI	L0, 0, 16256
	SFPLOADI	L1, 0, 16256
	SFPCOMPC
	SFPLOADI	L0, 0, 0
	SFPLOADI	L1, 0, 0
	SFPENCC	3, 10
	SFPOR	L1, L0
	SFPSTORE	L0, 0, 0
 #APP
# 796 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	SFPLOAD	L0, 0, 0
	SFPSETCC	0, L0, 0
	SFPLOADI	L0, 0, 16256
	SFPLOADI	L1, 0, 16256
	SFPCOMPC
	SFPLOADI	L0, 0, 0
	SFPLOADI	L1, 0, 0
	SFPENCC	3, 10
	SFPOR	L1, L0
	SFPSTORE	L0, 0, 0
 #APP
# 796 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	SFPLOAD	L0, 0, 0
	SFPSETCC	0, L0, 0
	SFPLOADI	L0, 0, 16256
	SFPLOADI	L1, 0, 16256
	SFPCOMPC
	SFPLOADI	L0, 0, 0
	SFPLOADI	L1, 0, 0
	SFPENCC	3, 10
	SFPOR	L1, L0
	SFPSTORE	L0, 0, 0
 #APP
# 796 "ckernel.cc" 1
	.word (-536739840)
# 0 "" 2
 #NO_APP
	mv	a0,s0
	call	_ZN7ckernel4sfpu14calculate_compILb0EL8SfpuType10ELm0EEEvm
	mv	a2,s0
	mv	a1,s0
	mv	a0,s0
	call	_ZN7ckernel4sfpu15calculate_clampILb0ELm0EEEvmmm
	call	_ZN7ckernel4sfpu13calculate_absILb0ELm0EEEvv
	li	a0,1
	call	_ZN7ckernel4sfpu14calculate_signILb0ELm0EEEvm
	call	_ZN7ckernel4sfpu17calculate_sigmoidILb0ELm0EEEvv
	call	_ZN7ckernel4sfpu14calculate_tanhILb1ELm0EEEvv
	mv	a2,s0
	mv	a1,s0
	mv	a0,s0
	call	_ZN7ckernel4sfpu18calculate_hardtanhILb0ELm0EEEvmmm
	mv	a0,s0
	call	_ZN7ckernel4sfpu15calculate_lreluILb0ELm0EEEvm
	call	_ZN7ckernel4sfpu25calculate_tanh_derivativeILb0ELi0ELm0EEEvv
	call	_ZN7ckernel4sfpu25calculate_tanh_derivativeILb0ELi1ELm0EEEvv
	mv	a0,s0
	call	_ZN7ckernel4sfpu15calculate_powerILb1ELm0EEEvm
	call	_ZN7ckernel4sfpu14calculate_geluILb0ELm0EEEvv
	call	_ZN7ckernel4sfpu14calculate_geluILb1ELm0EEEvv
	call	_ZN7ckernel4sfpu25calculate_gelu_derivativeILb0ELm0EEEvv
	call	_ZN7ckernel4sfpu14calculate_sqrtILb0ELi4ELi2ELm0EEEvv
	call	_ZN7ckernel4sfpu14calculate_sqrtILb1ELi4ELi2ELm0EEEvv
	call	_ZN7ckernel4sfpu21calculate_exponentialILb0ELb0ELb0ELm0EEEvt.isra.0
	li	a0,0
	call	_ZN7ckernel4sfpu21calculate_exponentialILb0ELb0ELb1ELm0EEEvt
	call	_ZN7ckernel4sfpu21calculate_exponentialILb0ELb1ELb0ELm0EEEvt.isra.0
	li	a0,0
	call	_ZN7ckernel4sfpu21calculate_exponentialILb0ELb1ELb1ELm0EEEvt
	call	_ZN7ckernel4sfpu21calculate_exponentialILb1ELb0ELb0ELm0EEEvt.isra.0
	li	a0,0
	call	_ZN7ckernel4sfpu21calculate_exponentialILb1ELb0ELb1ELm0EEEvt
	call	_ZN7ckernel4sfpu21calculate_exponentialILb1ELb1ELb0ELm0EEEvt.isra.0
	li	a0,0
	call	_ZN7ckernel4sfpu21calculate_exponentialILb1ELb1ELb1ELm0EEEvt
	mv	a0,s0
	call	_ZN7ckernel4sfpu13calculate_logILb1ELb1ELm0EEEvm
	call	_ZN7ckernel4sfpu13calculate_logILb1ELb0ELm0EEEvm.isra.0
	call	_ZN7ckernel4sfpu20calculate_reciprocalILb0ELi4ELm0EEEvv
	call	_ZN7ckernel4sfpu20calculate_reciprocalILb1ELi4ELm0EEEvv
	mv	a0,s0
	mv	a1,s0
	call	_ZN7ckernel4sfpu17calculate_dropoutILb0ELm0EEEvmm
	lw	ra,12(sp)
	lw	s0,8(sp)
	li	a0,0
	addi	sp,sp,16
	jr	ra
	.size	_ZN7ckernel4sfpu4mainEiPPc, .-_ZN7ckernel4sfpu4mainEiPPc
	.globl	_ZN7ckernel13instrn_bufferE
	.section	.sbss,"aw",@nobits
	.align	2
	.type	_ZN7ckernel13instrn_bufferE, @object
	.size	_ZN7ckernel13instrn_bufferE, 4
_ZN7ckernel13instrn_bufferE:
	.zero	4
	.ident	"GCC: (GNU) 10.2.0"
