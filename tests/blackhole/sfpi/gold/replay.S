	.file	"replay.cc"
	.option nopic
	.attribute arch, "rv32i2p0_m2p0_l2p0"
	.attribute unaligned_access, 0
	.attribute stack_align, 16
	.text
	.align	2
	.globl	_Z9no_replayv
	.type	_Z9no_replayv, @function
_Z9no_replayv:
	SFPLOAD	L0, 0, 0, 7
	SFPSTORE	0, L0, 0, 7
	ret
	.size	_Z9no_replayv, .-_Z9no_replayv
	.align	2
	.globl	_Z7replay4v
	.type	_Z7replay4v, @function
_Z7replay4v:
	TTREPLAY	0, 3, 1, 1
	SFPLOAD	L0, 0, 0, 7
	SFPDIVP2	L0, L0, 3, 1
	SFPSTORE	0, L0, 0, 7
	TTREPLAY	0, 3, 0, 0
	TTREPLAY	0, 3, 0, 0
	TTREPLAY	0, 3, 0, 0
	ret
	.size	_Z7replay4v, .-_Z7replay4v
	.align	2
	.globl	_Z7replay6v
	.type	_Z7replay6v, @function
_Z7replay6v:
	TTREPLAY	0, 6, 1, 1
	SFPLOAD	L0, 0, 0, 7
	SFPDIVP2	L0, L0, 3, 1
	SFPSTORE	0, L0, 0, 7
	SFPLOAD	L0, 0, 0, 7
	SFPDIVP2	L0, L0, 3, 1
	SFPSTORE	0, L0, 0, 7
	TTREPLAY	0, 6, 0, 0
	TTREPLAY	0, 6, 0, 0
	TTREPLAY	0, 6, 0, 0
	TTREPLAY	0, 6, 0, 0
	TTREPLAY	0, 6, 0, 0
	ret
	.size	_Z7replay6v, .-_Z7replay6v
	.align	2
	.globl	_Z25replay_load_in_the_middlev
	.type	_Z25replay_load_in_the_middlev, @function
_Z25replay_load_in_the_middlev:
	SFPLOADI	L0, 1, 4
	TTREPLAY	0, 9, 1, 1
	SFPIADD	L0, L0, 1, 5
	SFPIADD	L1, L0, -4, 5
	SFPSETCC	L1, 0, 6
	SFPLOADI	L0, 0, 4
	SFPENCC	3, 10
	SFPLOAD	L2, 0, 0, 7
	SFPMOV	L1, L0, 0
	SFPSETEXP	L1, L2, 0, 0
	SFPSTORE	0, L1, 0, 7
	TTREPLAY	0, 9, 0, 0
	TTREPLAY	0, 9, 0, 0
	TTREPLAY	0, 9, 0, 0
	TTREPLAY	0, 9, 0, 0
	TTREPLAY	0, 9, 0, 0
	TTREPLAY	0, 9, 0, 0
	TTREPLAY	0, 5, 0, 0
	SFPLOAD	L1, 0, 0, 7
	SFPSETEXP	L0, L1, 0, 0
	SFPSTORE	0, L0, 0, 7
	ret
	.size	_Z25replay_load_in_the_middlev, .-_Z25replay_load_in_the_middlev
	.align	2
	.globl	_Z27replay_assignlreg_const_regv
	.type	_Z27replay_assignlreg_const_regv, @function
_Z27replay_assignlreg_const_regv:
	TTREPLAY	0, 8, 1, 1
	SFPLOAD	L1, 0, 0, 7
	SFPMOV	L0, L3, 0
	SFPSETMAN	L0, L1, 0, 0
	SFPSTORE	0, L0, 0, 7
	SFPLOAD	L1, 0, 0, 7
	SFPMOV	L0, L3, 0
	SFPSETMAN	L0, L1, 0, 0
	SFPSTORE	0, L0, 0, 7
	TTREPLAY	0, 8, 0, 0
	TTREPLAY	0, 8, 0, 0
	TTREPLAY	0, 8, 0, 0
	ret
	.size	_Z27replay_assignlreg_const_regv, .-_Z27replay_assignlreg_const_regv
	.align	2
	.globl	_Z18replay_partial_topv
	.type	_Z18replay_partial_topv, @function
_Z18replay_partial_topv:
	TTREPLAY	0, 7, 1, 1
	SFPLOAD	L0, 0, 0, 7
	SFPMULI	L0, 16544, 0
	SFPNOP
	SFPSTORE	0, L0, 0, 7
	SFPLOAD	L0, 0, 0, 7
	SFPMULI	L0, 16544, 0
	SFPNOP
	SFPSTORE	0, L0, 0, 7
	TTREPLAY	0, 7, 0, 0
	SFPABS	L0, L0, 1
	SFPSTORE	0, L0, 0, 7
	ret
	.size	_Z18replay_partial_topv, .-_Z18replay_partial_topv
	.align	2
	.globl	_Z18replay_partial_botv
	.type	_Z18replay_partial_botv, @function
_Z18replay_partial_botv:
	TTREPLAY	0, 4, 1, 1
	SFPLOAD	L0, 0, 0, 7
	SFPMULI	L0, 16544, 0
	SFPNOP
	SFPSTORE	0, L0, 0, 7
	TTREPLAY	0, 4, 0, 0
	TTREPLAY	0, 4, 0, 0
	SFPLOADI	L0, 16460, 8
	SFPLOADI	L0, 52429, 10
	SFPMULI	L0, 16544, 0
	SFPNOP
	SFPSTORE	0, L0, 0, 7
	ret
	.size	_Z18replay_partial_botv, .-_Z18replay_partial_botv
	.align	2
	.globl	_Z13replay_nonimmi
	.type	_Z13replay_nonimmi, @function
_Z13replay_nonimmi:
	slli	a3,a0,16
	srli	a3,a3,16
	TTREPLAY	0, 5, 1, 1
	SFPLOAD	L1, 0, 0, 7
	lui	a5,%hi(_ZN7ckernel13instrn_bufferE)
	srli	a0,a0,16
	li	a2, 1896480768	# 710a0000
	lw	a5,%lo(_ZN7ckernel13instrn_bufferE)(a5)
	li	a4, 1896349696	# 71080000
	add	a4,a0,a4
	add	a3,a3,a2
	sw	a3, 0(a5)	# Op(0x71) - d(lr0)
	sw	a4, 0(a5)	# Op(0x71) lv(lr0)  d(lr0)
	SFPSETMAN	L0, L1, 0, 0
	SFPSTORE	0, L0, 0, 7
	TTREPLAY	0, 5, 0, 0
	TTREPLAY	0, 5, 0, 0
	TTREPLAY	0, 5, 0, 0
	TTREPLAY	0, 5, 0, 0
	TTREPLAY	0, 5, 0, 0
	TTREPLAY	0, 5, 0, 0
	TTREPLAY	0, 5, 0, 0
	ret
	.size	_Z13replay_nonimmi, .-_Z13replay_nonimmi
	.align	2
	.globl	_Z14replay_nonimm2i
	.type	_Z14replay_nonimm2i, @function
_Z14replay_nonimm2i:
	SFPLOAD	L1, 0, 0, 7
	li	a1,65536
	addi	a1,a1,-1
	lui	a5,%hi(_ZN7ckernel13instrn_bufferE)
	li	a3, 1896480768	# 710a0000
	srli	a4,a0,16
	and	a2,a0,a1
	li	a6, 1896349696	# 71080000
	lw	a5,%lo(_ZN7ckernel13instrn_bufferE)(a5)
	add	a4,a4,a6
	add	a2,a2,a3
	sw	a2, 0(a5)	# Op(0x71) - d(lr0)
	TTREPLAY	0, 4, 1, 1
	sw	a4, 0(a5)	# Op(0x71) lv(lr0)  d(lr0)
	SFPSETMAN	L0, L1, 0, 0
	SFPSTORE	0, L0, 0, 7
	SFPLOAD	L1, 0, 0, 7
	addi	a2,a0,1
	and	a2,a2,a1
	add	a2,a2,a3
	sw	a2, 0(a5)	# Op(0x71) - d(lr0)
	TTREPLAY	0, 4, 0, 0
	addi	a2,a0,2
	and	a2,a2,a1
	add	a2,a2,a3
	sw	a2, 0(a5)	# Op(0x71) - d(lr0)
	TTREPLAY	0, 4, 0, 0
	addi	a2,a0,3
	and	a2,a2,a1
	add	a2,a2,a3
	sw	a2, 0(a5)	# Op(0x71) - d(lr0)
	TTREPLAY	0, 4, 0, 0
	addi	a2,a0,4
	and	a2,a2,a1
	add	a2,a2,a3
	sw	a2, 0(a5)	# Op(0x71) - d(lr0)
	TTREPLAY	0, 4, 0, 0
	addi	a2,a0,5
	and	a2,a2,a1
	add	a2,a2,a3
	sw	a2, 0(a5)	# Op(0x71) - d(lr0)
	TTREPLAY	0, 4, 0, 0
	addi	a2,a0,6
	and	a2,a2,a1
	add	a2,a2,a3
	sw	a2, 0(a5)	# Op(0x71) - d(lr0)
	TTREPLAY	0, 4, 0, 0
	addi	a0,a0,7
	and	a0,a0,a1
	add	a0,a0,a3
	sw	a0, 0(a5)	# Op(0x71) - d(lr0)
	TTREPLAY	0, 3, 0, 0
	ret
	.size	_Z14replay_nonimm2i, .-_Z14replay_nonimm2i
	.align	2
	.globl	_Z14replay_nonimm3i
	.type	_Z14replay_nonimm3i, @function
_Z14replay_nonimm3i:
	li	a3,16384
	slli	a0,a0,1
	addi	a3,a3,-1
	lui	a5,%hi(_ZN7ckernel13instrn_bufferE)
	and	a2,a0,a3
	lw	a5,%lo(_ZN7ckernel13instrn_bufferE)(a5)
	li	a4, 1879162880	# 7001c000
	add	a4,a2,a4
	TTREPLAY	0, 4, 1, 1
	sw	a4, 0(a5)	# Op(0x70) - d(lr0)
	SFPSTORE	0, L0, 0, 7
	sw	a4, 0(a5)	# Op(0x70) - d(lr0)
	SFPSTORE	0, L0, 0, 7
	TTREPLAY	0, 4, 0, 0
	TTREPLAY	0, 4, 0, 0
	TTREPLAY	0, 4, 0, 0
	addi	a0,a0,2
	and	a0,a0,a3
	li	a4, 1879162880	# 7001c000
	add	a0,a0,a4
	sw	a0, 0(a5)	# Op(0x70) - d(lr0)
	SFPSTORE	0, L0, 0, 7
	li	a4, 1879162880	# 7001c000
	add	a4,a4,a2
	sw	a4, 0(a5)	# Op(0x70) - d(lr0)
	SFPSTORE	0, L0, 0, 7
	sw	a4, 0(a5)	# Op(0x70) - d(lr0)
	SFPSTORE	0, L0, 0, 7
	sw	a4, 0(a5)	# Op(0x70) - d(lr0)
	SFPSTORE	0, L0, 0, 7
	sw	a4, 0(a5)	# Op(0x70) - d(lr0)
	SFPSTORE	0, L0, 0, 7
	sw	a4, 0(a5)	# Op(0x70) - d(lr0)
	SFPSTORE	0, L0, 0, 7
	sw	a4, 0(a5)	# Op(0x70) - d(lr0)
	SFPSTORE	0, L0, 0, 7
	sw	a4, 0(a5)	# Op(0x70) - d(lr0)
	SFPSTORE	0, L0, 0, 7
	sw	a4, 0(a5)	# Op(0x70) - d(lr0)
	SFPSTORE	0, L0, 0, 7
	ret
	.size	_Z14replay_nonimm3i, .-_Z14replay_nonimm3i
	.align	2
	.globl	_Z21replay_jump_in_middleii
	.type	_Z21replay_jump_in_middleii, @function
_Z21replay_jump_in_middleii:
	SFPLOAD	L0, 0, 0, 7
	SFPSETMAN	L0, L0, 5, 1
	SFPSTORE	0, L0, 0, 7
	SFPLOAD	L0, 0, 0, 7
	SFPSETMAN	L0, L0, 5, 1
	andi	a5,a0,1
	beq	a5,zero,.L13
	xor	a0,a0,a1
.L13:
	SFPSTORE	0, L0, 0, 7
	SFPLOAD	L0, 0, 0, 7
	SFPSETMAN	L0, L0, 5, 1
	andi	a5,a0,2
	beq	a5,zero,.L14
	xor	a0,a0,a1
.L14:
	SFPSTORE	0, L0, 0, 7
	SFPLOAD	L0, 0, 0, 7
	SFPSETMAN	L0, L0, 5, 1
	andi	a5,a0,3
	beq	a5,zero,.L15
	xor	a0,a0,a1
.L15:
	SFPSTORE	0, L0, 0, 7
	SFPLOAD	L0, 0, 0, 7
	SFPSETMAN	L0, L0, 5, 1
	andi	a5,a0,4
	beq	a5,zero,.L16
	xor	a0,a0,a1
.L16:
	SFPSTORE	0, L0, 0, 7
	SFPLOAD	L0, 0, 0, 7
	SFPSETMAN	L0, L0, 5, 1
	andi	a5,a0,5
	beq	a5,zero,.L17
	xor	a0,a0,a1
.L17:
	SFPSTORE	0, L0, 0, 7
	SFPLOAD	L0, 0, 0, 7
	SFPSETMAN	L0, L0, 5, 1
	andi	a5,a0,6
	beq	a5,zero,.L18
	xor	a0,a0,a1
.L18:
	SFPSTORE	0, L0, 0, 7
	SFPLOAD	L0, 0, 0, 7
	SFPSETMAN	L0, L0, 5, 1
	andi	a5,a0,7
	beq	a5,zero,.L19
	xor	a0,a0,a1
.L19:
	SFPSTORE	0, L0, 0, 7
	ret
	.size	_Z21replay_jump_in_middleii, .-_Z21replay_jump_in_middleii
	.align	2
	.globl	_Z14replay_tti_asmv
	.type	_Z14replay_tti_asmv, @function
_Z14replay_tti_asmv:
	SFPLOAD	L0, 0, 0, 7
	SFPMULI	L0, 16544, 0
	SFPNOP
 #APP
# 174 "replay.cc" 1
	.word (402653186)
# 0 "" 2
 #NO_APP
	SFPSTORE	0, L0, 0, 7
	SFPLOAD	L0, 0, 0, 7
	SFPMULI	L0, 16544, 0
	SFPNOP
 #APP
# 174 "replay.cc" 1
	.word (402653186)
# 0 "" 2
 #NO_APP
	SFPSTORE	0, L0, 0, 7
	SFPLOAD	L0, 0, 0, 7
	SFPMULI	L0, 16544, 0
	SFPNOP
 #APP
# 174 "replay.cc" 1
	.word (402653186)
# 0 "" 2
 #NO_APP
	SFPSTORE	0, L0, 0, 7
	SFPLOAD	L0, 0, 0, 7
	SFPMULI	L0, 16544, 0
	SFPNOP
 #APP
# 174 "replay.cc" 1
	.word (402653186)
# 0 "" 2
 #NO_APP
	SFPSTORE	0, L0, 0, 7
	SFPLOAD	L0, 0, 0, 7
	SFPMULI	L0, 16544, 0
	SFPNOP
 #APP
# 174 "replay.cc" 1
	.word (402653186)
# 0 "" 2
 #NO_APP
	SFPSTORE	0, L0, 0, 7
	SFPLOAD	L0, 0, 0, 7
	SFPMULI	L0, 16544, 0
	SFPNOP
 #APP
# 174 "replay.cc" 1
	.word (402653186)
# 0 "" 2
 #NO_APP
	SFPSTORE	0, L0, 0, 7
	SFPLOAD	L0, 0, 0, 7
	SFPMULI	L0, 16544, 0
	SFPNOP
 #APP
# 174 "replay.cc" 1
	.word (402653186)
# 0 "" 2
 #NO_APP
	SFPSTORE	0, L0, 0, 7
	SFPLOAD	L0, 0, 0, 7
	SFPMULI	L0, 16544, 0
	SFPNOP
 #APP
# 174 "replay.cc" 1
	.word (402653186)
# 0 "" 2
 #NO_APP
	SFPSTORE	0, L0, 0, 7
	ret
	.size	_Z14replay_tti_asmv, .-_Z14replay_tti_asmv
	.globl	_ZN7ckernel13instrn_bufferE
	.section	.sbss,"aw",@nobits
	.align	2
	.type	_ZN7ckernel13instrn_bufferE, @object
	.size	_ZN7ckernel13instrn_bufferE, 4
_ZN7ckernel13instrn_bufferE:
	.zero	4
	.ident	"GCC: (gcdc243316e1) 12.2.0"
