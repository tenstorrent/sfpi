	.file	"replay.cc"
	.option nopic
	.attribute arch, "rv32i2p0_m2p0_w2p0"
	.attribute unaligned_access, 0
	.attribute stack_align, 16
	.text
	.align	1
	.globl	_Z9no_replayv
	.type	_Z9no_replayv, @function
_Z9no_replayv:
	SFPLOAD	L0, 0, 3, 0
	SFPSTORE	L0, 0, 3, 0
	ret
	.size	_Z9no_replayv, .-_Z9no_replayv
	.align	1
	.globl	_Z7replay4v
	.type	_Z7replay4v, @function
_Z7replay4v:
	SFPREPLAY	0, 3, 1, 1
	SFPLOAD	L0, 0, 3, 0
	SFPDIVP2	3, L0, L0, 1
	SFPSTORE	L0, 0, 3, 0
	SFPREPLAY	0, 3, 0, 0
	SFPREPLAY	0, 3, 0, 0
	SFPREPLAY	0, 3, 0, 0
	ret
	.size	_Z7replay4v, .-_Z7replay4v
	.align	1
	.globl	_Z7replay6v
	.type	_Z7replay6v, @function
_Z7replay6v:
	SFPREPLAY	0, 6, 1, 1
	SFPLOAD	L0, 0, 3, 0
	SFPDIVP2	3, L0, L0, 1
	SFPSTORE	L0, 0, 3, 0
	SFPLOAD	L0, 0, 3, 0
	SFPDIVP2	3, L0, L0, 1
	SFPSTORE	L0, 0, 3, 0
	SFPREPLAY	0, 6, 0, 0
	SFPREPLAY	0, 6, 0, 0
	SFPREPLAY	0, 6, 0, 0
	SFPREPLAY	0, 6, 0, 0
	SFPREPLAY	0, 6, 0, 0
	ret
	.size	_Z7replay6v, .-_Z7replay6v
	.align	1
	.globl	_Z25replay_load_in_the_middlev
	.type	_Z25replay_load_in_the_middlev, @function
_Z25replay_load_in_the_middlev:
	SFPLOADI	L0, 4, 1
	SFPIADD	1, L0, L0, 5
	SFPIADD	-4, L0, L1, 5
	SFPSETCC	0, L1, 6
	SFPLOADI	L0, 4, 0
	SFPENCC	3, 10
	SFPREPLAY	0, 9, 1, 1
	SFPLOAD	L2, 0, 3, 0
	SFPMOV	L0, L1, 0
	SFPSETEXP	0, L2, L1, 0
	SFPSTORE	L1, 0, 3, 0
	SFPIADD	1, L0, L0, 5
	SFPIADD	-4, L0, L1, 5
	SFPSETCC	0, L1, 6
	SFPLOADI	L0, 4, 0
	SFPENCC	3, 10
	SFPREPLAY	0, 9, 0, 0
	SFPREPLAY	0, 9, 0, 0
	SFPREPLAY	0, 9, 0, 0
	SFPREPLAY	0, 9, 0, 0
	SFPREPLAY	0, 9, 0, 0
	SFPREPLAY	0, 9, 0, 0
	SFPLOAD	L1, 0, 3, 0
	SFPSETEXP	0, L1, L0, 0
	SFPSTORE	L0, 0, 3, 0
	ret
	.size	_Z25replay_load_in_the_middlev, .-_Z25replay_load_in_the_middlev
	.align	1
	.globl	_Z27replay_assignlreg_const_regv
	.type	_Z27replay_assignlreg_const_regv, @function
_Z27replay_assignlreg_const_regv:
	SFPREPLAY	0, 8, 1, 1
	SFPLOAD	L1, 0, 3, 0
	SFPMOV	L3, L0, 0
	SFPSETMAN	0, L1, L0, 0
	SFPSTORE	L0, 0, 3, 0
	SFPLOAD	L1, 0, 3, 0
	SFPMOV	L3, L0, 0
	SFPSETMAN	0, L1, L0, 0
	SFPSTORE	L0, 0, 3, 0
	SFPREPLAY	0, 8, 0, 0
	SFPREPLAY	0, 8, 0, 0
	SFPREPLAY	0, 8, 0, 0
	ret
	.size	_Z27replay_assignlreg_const_regv, .-_Z27replay_assignlreg_const_regv
	.align	1
	.globl	_Z18replay_partial_topv
	.type	_Z18replay_partial_topv, @function
_Z18replay_partial_topv:
	SFPREPLAY	0, 4, 1, 1
	SFPLOAD	L0, 0, 3, 0
	SFPMULI	16544, L0, 0
	SFPWNOP
	SFPSTORE	L0, 0, 3, 0
	SFPREPLAY	0, 4, 0, 0
	SFPREPLAY	0, 4, 0, 0
	SFPREPLAY	0, 3, 0, 0
	SFPABS	L0, L0, 1
	SFPSTORE	L0, 0, 3, 0
	ret
	.size	_Z18replay_partial_topv, .-_Z18replay_partial_topv
	.align	1
	.globl	_Z18replay_partial_botv
	.type	_Z18replay_partial_botv, @function
_Z18replay_partial_botv:
	SFPREPLAY	0, 4, 1, 1
	SFPLOAD	L0, 0, 3, 0
	SFPMULI	16544, L0, 0
	SFPWNOP
	SFPSTORE	L0, 0, 3, 0
	SFPREPLAY	0, 4, 0, 0
	SFPREPLAY	0, 4, 0, 0
	SFPLOADI	L0, 8, 16460
	SFPLOADI	L0, 10, 52429
	SFPMULI	16544, L0, 0
	SFPWNOP
	SFPSTORE	L0, 0, 3, 0
	ret
	.size	_Z18replay_partial_botv, .-_Z18replay_partial_botv
	.align	1
	.globl	_Z13replay_nonimmi
	.type	_Z13replay_nonimmi, @function
_Z13replay_nonimmi:
	slli	a3,a0,16
	srli	a3,a3,16
	SFPREPLAY	0, 5, 1, 1
	SFPLOAD	L1, 0, 3, 0
	lui	a5,%hi(_ZN7ckernel13instrn_bufferE)
	lw	a5,%lo(_ZN7ckernel13instrn_bufferE)(a5)
	srli	a0,a0,16
	li	a2, 1896480768	# 710a0000
	li	a4, 1896349696	# 71080000
	add	a4,a0,a4
	add	a3,a3,a2
	sw	a3, 0(a5)	# Op(0x71) - d(lr0)
	sw	a4, 0(a5)	# Op(0x71) lv(lr0)  d(lr0)
	SFPSETMAN	0, L1, L0, 0
	SFPSTORE	L0, 0, 3, 0
	SFPREPLAY	0, 5, 0, 0
	SFPREPLAY	0, 5, 0, 0
	SFPREPLAY	0, 5, 0, 0
	SFPREPLAY	0, 5, 0, 0
	SFPREPLAY	0, 5, 0, 0
	SFPREPLAY	0, 5, 0, 0
	SFPREPLAY	0, 5, 0, 0
	ret
	.size	_Z13replay_nonimmi, .-_Z13replay_nonimmi
	.align	1
	.globl	_Z14replay_nonimm2i
	.type	_Z14replay_nonimm2i, @function
_Z14replay_nonimm2i:
	SFPLOAD	L1, 0, 3, 0
	li	a2,65536
	lui	a5,%hi(_ZN7ckernel13instrn_bufferE)
	addi	a2,a2,-1
	lw	a5,%lo(_ZN7ckernel13instrn_bufferE)(a5)
	li	a3, 1896480768	# 710a0000
	srli	a6,a0,16
	and	a1,a0,a2
	li	a4, 1896349696	# 71080000
	add	a4,a6,a4
	add	a1,a1,a3
	sw	a1, 0(a5)	# Op(0x71) - d(lr0)
	sw	a4, 0(a5)	# Op(0x71) lv(lr0)  d(lr0)
	SFPSETMAN	0, L1, L0, 0
	SFPSTORE	L0, 0, 3, 0
	SFPLOAD	L1, 0, 3, 0
	addi	a1,a0,1
	and	a1,a1,a2
	add	a1,a1,a3
	sw	a1, 0(a5)	# Op(0x71) - d(lr0)
	sw	a4, 0(a5)	# Op(0x71) lv(lr0)  d(lr0)
	SFPSETMAN	0, L1, L0, 0
	SFPSTORE	L0, 0, 3, 0
	SFPLOAD	L1, 0, 3, 0
	addi	a1,a0,2
	and	a1,a1,a2
	add	a1,a1,a3
	sw	a1, 0(a5)	# Op(0x71) - d(lr0)
	sw	a4, 0(a5)	# Op(0x71) lv(lr0)  d(lr0)
	SFPSETMAN	0, L1, L0, 0
	SFPSTORE	L0, 0, 3, 0
	SFPLOAD	L1, 0, 3, 0
	addi	a1,a0,3
	and	a1,a1,a2
	add	a1,a1,a3
	sw	a1, 0(a5)	# Op(0x71) - d(lr0)
	sw	a4, 0(a5)	# Op(0x71) lv(lr0)  d(lr0)
	SFPSETMAN	0, L1, L0, 0
	SFPSTORE	L0, 0, 3, 0
	SFPLOAD	L1, 0, 3, 0
	addi	a1,a0,4
	and	a1,a1,a2
	add	a1,a1,a3
	sw	a1, 0(a5)	# Op(0x71) - d(lr0)
	sw	a4, 0(a5)	# Op(0x71) lv(lr0)  d(lr0)
	SFPSETMAN	0, L1, L0, 0
	SFPSTORE	L0, 0, 3, 0
	SFPLOAD	L1, 0, 3, 0
	addi	a1,a0,5
	and	a1,a1,a2
	add	a1,a1,a3
	sw	a1, 0(a5)	# Op(0x71) - d(lr0)
	sw	a4, 0(a5)	# Op(0x71) lv(lr0)  d(lr0)
	SFPSETMAN	0, L1, L0, 0
	SFPSTORE	L0, 0, 3, 0
	SFPLOAD	L1, 0, 3, 0
	addi	a1,a0,6
	and	a1,a1,a2
	add	a1,a1,a3
	sw	a1, 0(a5)	# Op(0x71) - d(lr0)
	sw	a4, 0(a5)	# Op(0x71) lv(lr0)  d(lr0)
	SFPSETMAN	0, L1, L0, 0
	SFPSTORE	L0, 0, 3, 0
	SFPLOAD	L1, 0, 3, 0
	addi	a0,a0,7
	and	a0,a0,a2
	add	a0,a0,a3
	sw	a0, 0(a5)	# Op(0x71) - d(lr0)
	sw	a4, 0(a5)	# Op(0x71) lv(lr0)  d(lr0)
	SFPSETMAN	0, L1, L0, 0
	SFPSTORE	L0, 0, 3, 0
	ret
	.size	_Z14replay_nonimm2i, .-_Z14replay_nonimm2i
	.align	1
	.globl	_Z14replay_nonimm3i
	.type	_Z14replay_nonimm3i, @function
_Z14replay_nonimm3i:
	li	a2,16384
	lui	a5,%hi(_ZN7ckernel13instrn_bufferE)
	slli	a0,a0,1
	addi	a2,a2,-1
	lw	a5,%lo(_ZN7ckernel13instrn_bufferE)(a5)
	and	a1,a0,a2
	li	a3, 1879097344	# 7000c000
	add	a3,a1,a3
	SFPREPLAY	0, 8, 1, 1
	sw	a3, 0(a5)	# Op(0x70) - d(lr0)
	SFPSTORE	L0, 0, 3, 0
	sw	a3, 0(a5)	# Op(0x70) - d(lr0)
	SFPSTORE	L0, 0, 3, 0
	sw	a3, 0(a5)	# Op(0x70) - d(lr0)
	SFPSTORE	L0, 0, 3, 0
	sw	a3, 0(a5)	# Op(0x70) - d(lr0)
	SFPSTORE	L0, 0, 3, 0
	SFPREPLAY	0, 8, 0, 0
	addi	a0,a0,2
	and	a0,a0,a2
	li	a4, 1879097344	# 7000c000
	add	a0,a0,a4
	sw	a0, 0(a5)	# Op(0x70) - d(lr0)
	SFPSTORE	L0, 0, 3, 0
	mv	a4,a3
	SFPREPLAY	0, 8, 0, 0
	SFPREPLAY	0, 8, 0, 0
	ret
	.size	_Z14replay_nonimm3i, .-_Z14replay_nonimm3i
	.align	1
	.globl	_Z21replay_jump_in_middleii
	.type	_Z21replay_jump_in_middleii, @function
_Z21replay_jump_in_middleii:
	SFPLOAD	L0, 0, 3, 0
	SFPSETMAN	5, L0, L0, 1
	SFPSTORE	L0, 0, 3, 0
	SFPLOAD	L0, 0, 3, 0
	SFPSETMAN	5, L0, L0, 1
	andi	a5,a0,1
	beq	a5,zero,.L13
	xor	a0,a0,a1
.L13:
	SFPSTORE	L0, 0, 3, 0
	SFPLOAD	L0, 0, 3, 0
	SFPSETMAN	5, L0, L0, 1
	andi	a5,a0,2
	beq	a5,zero,.L14
	xor	a0,a0,a1
.L14:
	SFPSTORE	L0, 0, 3, 0
	SFPLOAD	L0, 0, 3, 0
	SFPSETMAN	5, L0, L0, 1
	andi	a5,a0,3
	beq	a5,zero,.L15
	xor	a0,a0,a1
.L15:
	SFPSTORE	L0, 0, 3, 0
	SFPLOAD	L0, 0, 3, 0
	SFPSETMAN	5, L0, L0, 1
	andi	a5,a0,4
	beq	a5,zero,.L16
	xor	a0,a0,a1
.L16:
	SFPSTORE	L0, 0, 3, 0
	SFPLOAD	L0, 0, 3, 0
	SFPSETMAN	5, L0, L0, 1
	andi	a5,a0,5
	beq	a5,zero,.L17
	xor	a0,a0,a1
.L17:
	SFPSTORE	L0, 0, 3, 0
	SFPLOAD	L0, 0, 3, 0
	SFPSETMAN	5, L0, L0, 1
	andi	a5,a0,6
	beq	a5,zero,.L18
	xor	a0,a0,a1
.L18:
	SFPSTORE	L0, 0, 3, 0
	SFPLOAD	L0, 0, 3, 0
	SFPSETMAN	5, L0, L0, 1
	andi	a5,a0,7
	beq	a5,zero,.L19
	xor	a0,a0,a1
.L19:
	SFPSTORE	L0, 0, 3, 0
	ret
	.size	_Z21replay_jump_in_middleii, .-_Z21replay_jump_in_middleii
	.align	1
	.globl	_Z14replay_tti_asmv
	.type	_Z14replay_tti_asmv, @function
_Z14replay_tti_asmv:
	SFPREPLAY	0, 3, 1, 1
	SFPLOAD	L0, 0, 3, 0
	SFPMULI	16544, L0, 0
	SFPWNOP
 #APP
# 168 "replay.cc" 1
	.word (402653186)
# 0 "" 2
 #NO_APP
	SFPSTORE	L0, 0, 3, 0
	SFPREPLAY	0, 3, 0, 0
 #APP
# 168 "replay.cc" 1
	.word (402653186)
# 0 "" 2
 #NO_APP
	SFPSTORE	L0, 0, 3, 0
	SFPREPLAY	0, 3, 0, 0
 #APP
# 168 "replay.cc" 1
	.word (402653186)
# 0 "" 2
 #NO_APP
	SFPSTORE	L0, 0, 3, 0
	SFPREPLAY	0, 3, 0, 0
 #APP
# 168 "replay.cc" 1
	.word (402653186)
# 0 "" 2
 #NO_APP
	SFPSTORE	L0, 0, 3, 0
	SFPREPLAY	0, 3, 0, 0
 #APP
# 168 "replay.cc" 1
	.word (402653186)
# 0 "" 2
 #NO_APP
	SFPSTORE	L0, 0, 3, 0
	SFPREPLAY	0, 3, 0, 0
 #APP
# 168 "replay.cc" 1
	.word (402653186)
# 0 "" 2
 #NO_APP
	SFPSTORE	L0, 0, 3, 0
	SFPREPLAY	0, 3, 0, 0
 #APP
# 168 "replay.cc" 1
	.word (402653186)
# 0 "" 2
 #NO_APP
	SFPSTORE	L0, 0, 3, 0
	SFPREPLAY	0, 3, 0, 0
 #APP
# 168 "replay.cc" 1
	.word (402653186)
# 0 "" 2
 #NO_APP
	SFPSTORE	L0, 0, 3, 0
	ret
	.size	_Z14replay_tti_asmv, .-_Z14replay_tti_asmv
	.globl	_ZN7ckernel13instrn_bufferE
	.section	.sbss,"aw",@nobits
	.align	2
	.type	_ZN7ckernel13instrn_bufferE, @object
	.size	_ZN7ckernel13instrn_bufferE, 4
_ZN7ckernel13instrn_bufferE:
	.zero	4
	.ident	"GCC: (GNU) 10.2.0"
