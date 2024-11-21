	.file	"nonimm.cc"
	.option nopic
	.attribute arch, "rv32i2p0_xttgs1p0"
	.attribute unaligned_access, 0
	.attribute stack_align, 16
	.text
	.align	2
	.globl	_Z5macroi
	.type	_Z5macroi, @function
_Z5macroi:
	lui	a5,%hi(_ZN7ckernel13instrn_bufferE)
	lw	a5,%lo(_ZN7ckernel13instrn_bufferE)(a5)
	li	a4,1881538560
	add	a4,a0,a4
	sw	a4,0(a5)
	li	a4,1898315776
	add	a4,a0,a4
	li	a3,1915092992
	sw	a4,0(a5)
	add	a3,a0,a3
	sw	a3,0(a5)
	li	a3,2030043136
	slli	a4,a0,12
	addi	a3,a3,565
	add	a3,a4,a3
	sw	a3,0(a5)
	li	a3,1962934272
	addi	a3,a3,42
	slli	a0,a0,8
	add	a0,a0,a3
	li	a3,2046820352
	addi	a3,a3,561
	sw	a0,0(a5)
	add	a3,a4,a3
	sw	a3,0(a5)
	li	a3,1979711488
	addi	a3,a3,561
	add	a3,a4,a3
	sw	a3,0(a5)
	li	a3,-2113929216
	addi	a3,a3,561
	add	a3,a4,a3
	sw	a3,0(a5)
	li	a3,-2097152000
	addi	a3,a3,561
	add	a3,a4,a3
	sw	a3,0(a5)
	li	a3,-1996488704
	addi	a3,a3,561
	add	a4,a4,a3
	sw	a4,0(a5)
	ret
	.size	_Z5macroi, .-_Z5macroi
	.align	2
	.globl	_Z13macro_checkedi
	.type	_Z13macro_checkedi, @function
_Z13macro_checkedi:
	slli	a3,a0,16
	lui	a5,%hi(_ZN7ckernel13instrn_bufferE)
	lw	a5,%lo(_ZN7ckernel13instrn_bufferE)(a5)
	srli	a3,a3,16
	li	a4,1881538560
	add	a4,a3,a4
	li	a2,1898315776
	sw	a4,0(a5)
	add	a2,a3,a2
	sw	a2,0(a5)
	slli	a4,a0,12
	li	a2,16773120
	and	a4,a4,a2
	li	a2,1915092992
	add	a3,a3,a2
	sw	a3,0(a5)
	li	a3,2030043136
	addi	a3,a3,565
	add	a3,a4,a3
	li	a2,16777216
	sw	a3,0(a5)
	slli	a0,a0,8
	addi	a2,a2,-256
	li	a3,1962934272
	addi	a3,a3,42
	and	a0,a0,a2
	add	a0,a0,a3
	li	a3,2046820352
	addi	a3,a3,561
	sw	a0,0(a5)
	add	a3,a4,a3
	sw	a3,0(a5)
	li	a3,1979711488
	addi	a3,a3,561
	add	a3,a4,a3
	sw	a3,0(a5)
	li	a3,-2113929216
	addi	a3,a3,561
	add	a3,a4,a3
	sw	a3,0(a5)
	li	a3,-2097152000
	addi	a3,a3,561
	add	a3,a4,a3
	sw	a3,0(a5)
	li	a3,-1996488704
	addi	a3,a3,561
	add	a4,a4,a3
	sw	a4,0(a5)
	ret
	.size	_Z13macro_checkedi, .-_Z13macro_checkedi
	.align	2
	.globl	_Z19macro_reuse_checkedi
	.type	_Z19macro_reuse_checkedi, @function
_Z19macro_reuse_checkedi:
	li	a1,65536
	addi	a1,a1,-1
	lui	a5,%hi(_ZN7ckernel13instrn_bufferE)
	lw	a5,%lo(_ZN7ckernel13instrn_bufferE)(a5)
	li	a2,1881538560
	and	a6,a0,a1
	addi	a3,a0,1
	add	a7,a6,a2
	and	a3,a3,a1
	addi	a4,a0,2
	sw	a7,0(a5)
	and	a4,a4,a1
	add	a7,a3,a2
	addi	a0,a0,3
	sw	a7,0(a5)
	and	a0,a0,a1
	add	a1,a4,a2
	sw	a1,0(a5)
	add	a2,a0,a2
	li	a1,1915092992
	sw	a2,0(a5)
	add	a6,a6,a1
	sw	a6,0(a5)
	add	a3,a3,a1
	sw	a3,0(a5)
	add	a4,a4,a1
	sw	a4,0(a5)
	add	a0,a0,a1
	sw	a0,0(a5)
	ret
	.size	_Z19macro_reuse_checkedi, .-_Z19macro_reuse_checkedi
	.align	2
	.globl	_Z8load_imms
	.type	_Z8load_imms, @function
_Z8load_imms:
	SFPLOAD	L3, 10, 1
	SFPLOAD	L2, 20, 1
	SFPLOAD	L1, 30, 1
	SFPLOAD	L0, 40, 1
	SFPSTORE	1, L3, 2
	SFPSTORE	10, L2, 2
	SFPSTORE	20, L1, 2
	SFPSTORE	30, L0, 2
	ret
	.size	_Z8load_imms, .-_Z8load_imms
	.align	2
	.globl	_Z8load_regs
	.type	_Z8load_regs, @function
_Z8load_regs:
	li	a5,65536
	addi	a5,a5,-1
	lui	a3,%hi(_ZN7ckernel13instrn_bufferE)
	and	a4,a0,a5
	li	a2, 1882259456	# 70310000
	lw	a3,%lo(_ZN7ckernel13instrn_bufferE)(a3)
	add	a4,a4,a2
	sw	a4, 0(a3)	# Op(0x70) - d(lr3)
	addi	a4,a0,10
	and	a4,a4,a5
	li	a2, 1881210880	# 70210000
	add	a4,a4,a2
	sw	a4, 0(a3)	# Op(0x70) - d(lr2)
	addi	a4,a0,20
	and	a4,a4,a5
	li	a2, 1880162304	# 70110000
	add	a4,a4,a2
	sw	a4, 0(a3)	# Op(0x70) - d(lr1)
	addi	a0,a0,30
	and	a0,a0,a5
	li	a5, 1879113728	# 70010000
	add	a0,a0,a5
	sw	a0, 0(a3)	# Op(0x70) - d(lr0)
	SFPSTORE	1, L3, 2
	SFPSTORE	10, L2, 2
	SFPSTORE	20, L1, 2
	SFPSTORE	30, L0, 2
	ret
	.size	_Z8load_regs, .-_Z8load_regs
	.align	2
	.globl	_Z9load_boths
	.type	_Z9load_boths, @function
_Z9load_boths:
	SFPLOAD	L3, 10, 1
	SFPLOAD	L2, 20, 1
	li	a5,65536
	addi	a5,a5,-1
	lui	a3,%hi(_ZN7ckernel13instrn_bufferE)
	and	a4,a0,a5
	li	a2, 1880162304	# 70110000
	lw	a3,%lo(_ZN7ckernel13instrn_bufferE)(a3)
	add	a4,a4,a2
	sw	a4, 0(a3)	# Op(0x70) - d(lr1)
	addi	a0,a0,10
	and	a0,a0,a5
	li	a5, 1879113728	# 70010000
	add	a0,a0,a5
	sw	a0, 0(a3)	# Op(0x70) - d(lr0)
	SFPSTORE	1, L3, 2
	SFPSTORE	10, L2, 2
	SFPSTORE	20, L1, 2
	SFPSTORE	30, L0, 2
	ret
	.size	_Z9load_boths, .-_Z9load_boths
	.align	2
	.globl	_Z9loadi_imms
	.type	_Z9loadi_imms, @function
_Z9loadi_imms:
	SFPLOADI	L3, 10, 0
	SFPLOADI	L2, 10, 4
	SFPLOADI	L1, 20, 8
	SFPLOADI	L0, 30, 12
	SFPSTORE	1, L3, 2
	SFPSTORE	10, L2, 2
	SFPSTORE	20, L1, 2
	SFPSTORE	30, L0, 2
	ret
	.size	_Z9loadi_imms, .-_Z9loadi_imms
	.align	2
	.globl	_Z9loadi_regs
	.type	_Z9loadi_regs, @function
_Z9loadi_regs:
	li	a5,65536
	addi	a5,a5,-1
	lui	a3,%hi(_ZN7ckernel13instrn_bufferE)
	and	a4,a0,a5
	li	a2, 1898971136	# 71300000
	lw	a3,%lo(_ZN7ckernel13instrn_bufferE)(a3)
	add	a4,a4,a2
	sw	a4, 0(a3)	# Op(0x71) - d(lr3)
	addi	a4,a0,1
	and	a4,a4,a5
	li	a2, 1898184704	# 71240000
	add	a4,a4,a2
	sw	a4, 0(a3)	# Op(0x71) - d(lr2)
	addi	a4,a0,2
	and	a4,a4,a5
	li	a2, 1897398272	# 71180000
	add	a4,a4,a2
	sw	a4, 0(a3)	# Op(0x71) - d(lr1)
	addi	a0,a0,3
	and	a0,a0,a5
	li	a5, 1896611840	# 710c0000
	add	a0,a0,a5
	sw	a0, 0(a3)	# Op(0x71) - d(lr0)
	SFPSTORE	1, L3, 2
	SFPSTORE	10, L2, 2
	SFPSTORE	20, L1, 2
	SFPSTORE	30, L0, 2
	ret
	.size	_Z9loadi_regs, .-_Z9loadi_regs
	.align	2
	.globl	_Z10loadi_boths
	.type	_Z10loadi_boths, @function
_Z10loadi_boths:
	SFPLOADI	L3, 10, 0
	SFPLOADI	L2, 20, 4
	li	a5,65536
	addi	a5,a5,-1
	lui	a3,%hi(_ZN7ckernel13instrn_bufferE)
	and	a4,a0,a5
	li	a2, 1897398272	# 71180000
	lw	a3,%lo(_ZN7ckernel13instrn_bufferE)(a3)
	add	a4,a4,a2
	sw	a4, 0(a3)	# Op(0x71) - d(lr1)
	addi	a0,a0,1
	and	a0,a0,a5
	li	a5, 1896611840	# 710c0000
	add	a0,a0,a5
	sw	a0, 0(a3)	# Op(0x71) - d(lr0)
	SFPSTORE	1, L3, 2
	SFPSTORE	10, L2, 2
	SFPSTORE	20, L1, 2
	SFPSTORE	30, L0, 2
	ret
	.size	_Z10loadi_boths, .-_Z10loadi_boths
	.align	2
	.globl	_Z9store_imms
	.type	_Z9store_imms, @function
_Z9store_imms:
	SFPLOAD	L0, 10, 1
	SFPSTORE	10, L0, 2
	SFPSTORE	20, L0, 2
	SFPSTORE	30, L0, 2
	SFPSTORE	40, L0, 2
	ret
	.size	_Z9store_imms, .-_Z9store_imms
	.align	2
	.globl	_Z9store_regs
	.type	_Z9store_regs, @function
_Z9store_regs:
	SFPLOAD	L0, 10, 1
	li	a5,65536
	addi	a5,a5,-1
	lui	a3,%hi(_ZN7ckernel13instrn_bufferE)
	and	a4,a0,a5
	li	a2, 1912733696	# 72020000
	lw	a3,%lo(_ZN7ckernel13instrn_bufferE)(a3)
	add	a4,a4,a2
	sw	a4, 0(a3)	# Op(0x72) s(lr0)
	addi	a4,a0,1
	and	a4,a4,a5
	li	a2, 1912733696	# 72020000
	add	a4,a4,a2
	sw	a4, 0(a3)	# Op(0x72) s(lr0)
	addi	a4,a0,2
	and	a4,a4,a5
	li	a2, 1912733696	# 72020000
	add	a4,a4,a2
	sw	a4, 0(a3)	# Op(0x72) s(lr0)
	addi	a0,a0,3
	and	a0,a0,a5
	li	a5, 1912733696	# 72020000
	add	a0,a0,a5
	sw	a0, 0(a3)	# Op(0x72) s(lr0)
	ret
	.size	_Z9store_regs, .-_Z9store_regs
	.align	2
	.globl	_Z10store_boths
	.type	_Z10store_boths, @function
_Z10store_boths:
	SFPLOAD	L0, 10, 1
	SFPSTORE	10, L0, 2
	SFPSTORE	20, L0, 2
	li	a4,65536
	addi	a5,a0,1
	addi	a4,a4,-1
	lui	a3,%hi(_ZN7ckernel13instrn_bufferE)
	and	a5,a5,a4
	li	a2, 1912733696	# 72020000
	lw	a3,%lo(_ZN7ckernel13instrn_bufferE)(a3)
	add	a5,a5,a2
	sw	a5, 0(a3)	# Op(0x72) s(lr0)
	addi	a0,a0,2
	and	a0,a0,a4
	li	a5, 1912733696	# 72020000
	add	a0,a0,a5
	sw	a0, 0(a3)	# Op(0x72) s(lr0)
	ret
	.size	_Z10store_boths, .-_Z10store_boths
	.align	2
	.globl	_Z11store_r_imms
	.type	_Z11store_r_imms, @function
_Z11store_r_imms:
	SFPSTORE	10, L13, 2
	SFPSTORE	20, L13, 2
	SFPSTORE	10, L13, 2
	SFPSTORE	20, L13, 2
	ret
	.size	_Z11store_r_imms, .-_Z11store_r_imms
	.align	2
	.globl	_Z11store_r_regs
	.type	_Z11store_r_regs, @function
_Z11store_r_regs:
	li	a5,65536
	addi	a5,a5,-1
	lui	a3,%hi(_ZN7ckernel13instrn_bufferE)
	and	a4,a0,a5
	li	a2, 1926365184	# 72d20000
	lw	a3,%lo(_ZN7ckernel13instrn_bufferE)(a3)
	add	a4,a4,a2
	sw	a4, 0(a3)	# Op(0x72) s(lr13)
	addi	a4,a0,1
	and	a4,a4,a5
	li	a2, 1926365184	# 72d20000
	add	a4,a4,a2
	sw	a4, 0(a3)	# Op(0x72) s(lr13)
	addi	a4,a0,2
	and	a4,a4,a5
	li	a2, 1926365184	# 72d20000
	add	a4,a4,a2
	sw	a4, 0(a3)	# Op(0x72) s(lr13)
	addi	a0,a0,3
	and	a0,a0,a5
	li	a5, 1926365184	# 72d20000
	add	a0,a0,a5
	sw	a0, 0(a3)	# Op(0x72) s(lr13)
	ret
	.size	_Z11store_r_regs, .-_Z11store_r_regs
	.align	2
	.globl	_Z12store_r_boths
	.type	_Z12store_r_boths, @function
_Z12store_r_boths:
	SFPSTORE	10, L13, 2
	SFPSTORE	20, L13, 2
	li	a5,65536
	addi	a5,a5,-1
	lui	a3,%hi(_ZN7ckernel13instrn_bufferE)
	and	a4,a0,a5
	li	a2, 1926365184	# 72d20000
	lw	a3,%lo(_ZN7ckernel13instrn_bufferE)(a3)
	add	a4,a4,a2
	sw	a4, 0(a3)	# Op(0x72) s(lr13)
	addi	a0,a0,1
	and	a0,a0,a5
	li	a5, 1926365184	# 72d20000
	add	a0,a0,a5
	sw	a0, 0(a3)	# Op(0x72) s(lr13)
	ret
	.size	_Z12store_r_boths, .-_Z12store_r_boths
	.align	2
	.globl	_Z8addi_imms
	.type	_Z8addi_imms, @function
_Z8addi_imms:
	SFPLOAD	L3, 10, 1
	SFPLOAD	L2, 20, 1
	SFPLOAD	L1, 30, 1
	SFPLOAD	L0, 40, 1
	SFPADDI	L3, 1, 1
	TTNOP
	TTNOP
	SFPADDI	L2, 2, 1
	TTNOP
	TTNOP
	SFPADDI	L1, 3, 3
	TTNOP
	TTNOP
	SFPADDI	L0, 4, 3
	TTNOP
	TTNOP
	SFPSTORE	1, L3, 2
	SFPSTORE	10, L2, 2
	SFPSTORE	20, L1, 2
	SFPSTORE	30, L0, 2
	ret
	.size	_Z8addi_imms, .-_Z8addi_imms
	.align	2
	.globl	_Z8addi_regs
	.type	_Z8addi_regs, @function
_Z8addi_regs:
	SFPLOAD	L3, 10, 1
	SFPLOAD	L2, 20, 1
	SFPLOAD	L1, 30, 1
	SFPLOAD	L0, 40, 1
	li	a4,16777216
	slli	a5,a0,8
	addi	a4,a4,-256
	lui	a3,%hi(_ZN7ckernel13instrn_bufferE)
	and	a5,a5,a4
	li	a2, 1962934321	# 75000031
	lw	a3,%lo(_ZN7ckernel13instrn_bufferE)(a3)
	add	a5,a5,a2
	sw	a5, 0(a3)	# Op(0x75) lv(lr3)  d(lr3)
	TTNOP
	TTNOP
	addi	a5,a0,1
	slli	a5,a5,8
	and	a5,a5,a4
	li	a2, 1962934314	# 7500002a
	add	a5,a5,a2
	sw	a5, 0(a3)	# Op(0x75) lv(lr2)  d(lr2)
	TTNOP
	TTNOP
	addi	a5,a0,2
	slli	a5,a5,8
	and	a5,a5,a4
	li	a2, 1962934293	# 75000015
	add	a5,a5,a2
	sw	a5, 0(a3)	# Op(0x75) lv(lr1)  d(lr1)
	TTNOP
	TTNOP
	addi	a0,a0,3
	slli	a0,a0,8
	and	a0,a0,a4
	li	a5, 1962934279	# 75000007
	add	a0,a0,a5
	sw	a0, 0(a3)	# Op(0x75) lv(lr0)  d(lr0)
	TTNOP
	TTNOP
	SFPSTORE	1, L3, 2
	SFPSTORE	10, L2, 2
	SFPSTORE	20, L1, 2
	SFPSTORE	30, L0, 2
	ret
	.size	_Z8addi_regs, .-_Z8addi_regs
	.align	2
	.globl	_Z9addi_boths
	.type	_Z9addi_boths, @function
_Z9addi_boths:
	SFPLOAD	L3, 10, 1
	SFPLOAD	L2, 20, 1
	SFPLOAD	L1, 30, 1
	SFPLOAD	L0, 40, 1
	SFPADDI	L3, 1, 1
	TTNOP
	TTNOP
	SFPADDI	L2, 2, 3
	TTNOP
	TTNOP
	li	a4,16777216
	slli	a5,a0,8
	addi	a4,a4,-256
	lui	a3,%hi(_ZN7ckernel13instrn_bufferE)
	and	a5,a5,a4
	li	a2, 1962934293	# 75000015
	lw	a3,%lo(_ZN7ckernel13instrn_bufferE)(a3)
	add	a5,a5,a2
	sw	a5, 0(a3)	# Op(0x75) lv(lr1)  d(lr1)
	TTNOP
	TTNOP
	addi	a0,a0,1
	slli	a0,a0,8
	and	a0,a0,a4
	li	a5, 1962934279	# 75000007
	add	a0,a0,a5
	sw	a0, 0(a3)	# Op(0x75) lv(lr0)  d(lr0)
	TTNOP
	TTNOP
	SFPSTORE	1, L3, 2
	SFPSTORE	10, L2, 2
	SFPSTORE	20, L1, 2
	SFPSTORE	30, L0, 2
	ret
	.size	_Z9addi_boths, .-_Z9addi_boths
	.align	2
	.globl	_Z11xiadd_i_imms
	.type	_Z11xiadd_i_imms, @function
_Z11xiadd_i_imms:
	SFPLOAD	L3, 10, 1
	SFPLOAD	L2, 20, 1
	SFPLOAD	L1, 30, 1
	SFPLOAD	L0, 40, 1
	SFPIADD	L3, L3, 1, 5
	TTNOP
	TTNOP
	SFPIADD	L2, L2, 2, 5
	TTNOP
	TTNOP
	SFPIADD	L1, L1, 3, 5
	TTNOP
	TTNOP
	SFPIADD	L0, L0, 4, 5
	TTNOP
	TTNOP
	SFPSTORE	1, L3, 2
	SFPSTORE	10, L2, 2
	SFPSTORE	20, L1, 2
	SFPSTORE	30, L0, 2
	ret
	.size	_Z11xiadd_i_imms, .-_Z11xiadd_i_imms
	.align	2
	.globl	_Z11xiadd_i_regs
	.type	_Z11xiadd_i_regs, @function
_Z11xiadd_i_regs:
	SFPLOAD	L1, 10, 1
	SFPLOAD	L0, 20, 1
	SFPLOAD	L3, 30, 1
	li	a5,65536
	addi	a5,a5,-1
	lui	a3,%hi(_ZN7ckernel13instrn_bufferE)
	and	a4,a0,a5
	li	a2, 1898053632	# 71220000
	lw	a3,%lo(_ZN7ckernel13instrn_bufferE)(a3)
	add	a4,a4,a2
	sw	a4, 0(a3)	# Op(0x71) - d(lr2)
	SFPIADD	L2, L1, 0, 4
	TTNOP
	TTNOP
	addi	a4,a0,1
	and	a4,a4,a5
	li	a2, 1897005056	# 71120000
	add	a4,a4,a2
	sw	a4, 0(a3)	# Op(0x71) - d(lr1)
	SFPIADD	L1, L0, 0, 4
	TTNOP
	TTNOP
	addi	a0,a0,2
	and	a0,a0,a5
	li	a5, 1895956480	# 71020000
	add	a0,a0,a5
	sw	a0, 0(a3)	# Op(0x71) - d(lr0)
	SFPIADD	L0, L3, 0, 4
	TTNOP
	TTNOP
	SFPSTORE	1, L2, 2
	SFPSTORE	10, L1, 2
	SFPSTORE	20, L0, 2
	ret
	.size	_Z11xiadd_i_regs, .-_Z11xiadd_i_regs
	.align	2
	.globl	_Z12xiadd_i_boths
	.type	_Z12xiadd_i_boths, @function
_Z12xiadd_i_boths:
	SFPLOAD	L2, 10, 1
	SFPLOAD	L1, 20, 1
	SFPLOAD	L3, 30, 1
	SFPIADD	L2, L2, 1, 5
	TTNOP
	TTNOP
	SFPIADD	L1, L1, 2, 5
	TTNOP
	TTNOP
	slli	a0,a0,16
	lui	a5,%hi(_ZN7ckernel13instrn_bufferE)
	li	a4, 1895956480	# 71020000
	lw	a5,%lo(_ZN7ckernel13instrn_bufferE)(a5)
	srli	a0,a0,16
	add	a0,a0,a4
	sw	a0, 0(a5)	# Op(0x71) - d(lr0)
	SFPIADD	L0, L3, 0, 4
	TTNOP
	TTNOP
	SFPSTORE	1, L2, 2
	SFPSTORE	10, L1, 2
	SFPSTORE	20, L0, 2
	ret
	.size	_Z12xiadd_i_boths, .-_Z12xiadd_i_boths
	.align	2
	.globl	_Z10shft_i_imms
	.type	_Z10shft_i_imms, @function
_Z10shft_i_imms:
	SFPLOAD	L3, 10, 1
	SFPLOAD	L2, 20, 1
	SFPLOAD	L1, 30, 1
	SFPLOAD	L0, 40, 1
	SFPSHFT	L3, L0, 1, 1
	TTNOP
	TTNOP
	SFPSHFT	L2, L0, 2, 1
	TTNOP
	TTNOP
	SFPSHFT	L1, L0, 3, 1
	TTNOP
	TTNOP
	SFPSHFT	L0, L0, 4, 1
	TTNOP
	TTNOP
	SFPSTORE	1, L3, 2
	SFPSTORE	10, L2, 2
	SFPSTORE	20, L1, 2
	SFPSTORE	30, L0, 2
	ret
	.size	_Z10shft_i_imms, .-_Z10shft_i_imms
	.align	2
	.globl	_Z10shft_i_regs
	.type	_Z10shft_i_regs, @function
_Z10shft_i_regs:
	SFPLOAD	L3, 10, 1
	SFPLOAD	L2, 20, 1
	SFPLOAD	L1, 30, 1
	SFPLOAD	L0, 40, 1
	slli	a5,a0,20
	lui	a4,%hi(_ZN7ckernel13instrn_bufferE)
	li	a3, 2046820401	# 7a000031
	lw	a4,%lo(_ZN7ckernel13instrn_bufferE)(a4)
	srli	a5,a5,8
	add	a5,a5,a3
	sw	a5, 0(a4)	# Op(0x7a) lv(lr3)  d(lr3)
	TTNOP
	TTNOP
	addi	a5,a0,1
	li	a3,16773120
	slli	a5,a5,12
	and	a5,a5,a3
	li	a2, 2046820385	# 7a000021
	add	a5,a5,a2
	sw	a5, 0(a4)	# Op(0x7a) lv(lr2)  d(lr2)
	TTNOP
	TTNOP
	addi	a5,a0,2
	slli	a5,a5,12
	and	a5,a5,a3
	li	a2, 2046820369	# 7a000011
	add	a5,a5,a2
	sw	a5, 0(a4)	# Op(0x7a) lv(lr1)  d(lr1)
	TTNOP
	TTNOP
	addi	a0,a0,3
	slli	a0,a0,12
	and	a0,a0,a3
	li	a5, 2046820353	# 7a000001
	add	a0,a0,a5
	sw	a0, 0(a4)	# Op(0x7a) lv(lr0)  d(lr0)
	TTNOP
	TTNOP
	SFPSTORE	1, L3, 2
	SFPSTORE	10, L2, 2
	SFPSTORE	20, L1, 2
	SFPSTORE	30, L0, 2
	ret
	.size	_Z10shft_i_regs, .-_Z10shft_i_regs
	.align	2
	.globl	_Z11shft_i_boths
	.type	_Z11shft_i_boths, @function
_Z11shft_i_boths:
	SFPLOAD	L3, 10, 1
	SFPLOAD	L2, 20, 1
	SFPLOAD	L1, 30, 1
	SFPLOAD	L0, 40, 1
	SFPSHFT	L3, L0, 1, 1
	TTNOP
	TTNOP
	SFPSHFT	L2, L0, 2, 1
	TTNOP
	TTNOP
	slli	a5,a0,20
	lui	a4,%hi(_ZN7ckernel13instrn_bufferE)
	li	a3, 2046820369	# 7a000011
	lw	a4,%lo(_ZN7ckernel13instrn_bufferE)(a4)
	srli	a5,a5,8
	add	a5,a5,a3
	sw	a5, 0(a4)	# Op(0x7a) lv(lr1)  d(lr1)
	TTNOP
	TTNOP
	addi	a0,a0,1
	li	a5,16773120
	slli	a0,a0,12
	and	a0,a0,a5
	li	a5, 2046820353	# 7a000001
	add	a0,a0,a5
	sw	a0, 0(a4)	# Op(0x7a) lv(lr0)  d(lr0)
	TTNOP
	TTNOP
	SFPSTORE	1, L3, 2
	SFPSTORE	10, L2, 2
	SFPSTORE	20, L1, 2
	SFPSTORE	30, L0, 2
	ret
	.size	_Z11shft_i_boths, .-_Z11shft_i_boths
	.align	2
	.globl	_Z9divp2_imms
	.type	_Z9divp2_imms, @function
_Z9divp2_imms:
	SFPLOAD	L0, 10, 1
	SFPLOAD	L2, 20, 1
	SFPLOAD	L0, 30, 1
	SFPLOAD	L1, 40, 1
	SFPDIVP2	L3, L0, 26, 0
	TTNOP
	TTNOP
	SFPDIVP2	L2, L2, 42, 1
	TTNOP
	TTNOP
	SFPDIVP2	L0, L0, 58, 0
	TTNOP
	TTNOP
	SFPDIVP2	L1, L1, 74, 1
	TTNOP
	TTNOP
	SFPSTORE	1, L3, 2
	SFPSTORE	10, L2, 2
	SFPSTORE	20, L0, 2
	SFPSTORE	30, L1, 2
	ret
	.size	_Z9divp2_imms, .-_Z9divp2_imms
	.align	2
	.globl	_Z9divp2_regs
	.type	_Z9divp2_regs, @function
_Z9divp2_regs:
	SFPLOAD	L0, 10, 1
	SFPLOAD	L2, 20, 1
	SFPLOAD	L1, 30, 1
	SFPLOAD	L0, 40, 1
	slli	a5,a0,20
	lui	a4,%hi(_ZN7ckernel13instrn_bufferE)
	li	a3, 1979711536	# 76000030
	lw	a4,%lo(_ZN7ckernel13instrn_bufferE)(a4)
	srli	a5,a5,8
	add	a5,a5,a3
	sw	a5, 0(a4)	# Op(0x76) - d(lr3) s(lr0)
	TTNOP
	TTNOP
	addi	a5,a0,1
	li	a3,16773120
	slli	a5,a5,12
	and	a5,a5,a3
	li	a2, 1979712033	# 76000221
	add	a5,a5,a2
	sw	a5, 0(a4)	# Op(0x76) - d(lr2) s(lr2)
	TTNOP
	TTNOP
	addi	a5,a0,2
	slli	a5,a5,12
	and	a5,a5,a3
	li	a2, 1979711760	# 76000110
	add	a5,a5,a2
	sw	a5, 0(a4)	# Op(0x76) - d(lr1) s(lr1)
	TTNOP
	TTNOP
	addi	a0,a0,3
	slli	a0,a0,12
	and	a0,a0,a3
	li	a5, 1979711489	# 76000001
	add	a0,a0,a5
	sw	a0, 0(a4)	# Op(0x76) - d(lr0) s(lr0)
	TTNOP
	TTNOP
	SFPSTORE	1, L3, 2
	SFPSTORE	10, L2, 2
	SFPSTORE	20, L1, 2
	SFPSTORE	30, L0, 2
	ret
	.size	_Z9divp2_regs, .-_Z9divp2_regs
	.align	2
	.globl	_Z10divp2_boths
	.type	_Z10divp2_boths, @function
_Z10divp2_boths:
	SFPLOAD	L0, 10, 1
	SFPLOAD	L2, 20, 1
	SFPLOAD	L1, 30, 1
	SFPLOAD	L0, 40, 1
	SFPDIVP2	L3, L0, 27, 0
	TTNOP
	TTNOP
	SFPDIVP2	L2, L2, 27, 1
	TTNOP
	TTNOP
	slli	a5,a0,20
	lui	a4,%hi(_ZN7ckernel13instrn_bufferE)
	li	a3, 1979711760	# 76000110
	lw	a4,%lo(_ZN7ckernel13instrn_bufferE)(a4)
	srli	a5,a5,8
	add	a5,a5,a3
	sw	a5, 0(a4)	# Op(0x76) - d(lr1) s(lr1)
	TTNOP
	TTNOP
	addi	a0,a0,1
	li	a5,16773120
	slli	a0,a0,12
	and	a0,a0,a5
	li	a5, 1979711489	# 76000001
	add	a0,a0,a5
	sw	a0, 0(a4)	# Op(0x76) - d(lr0) s(lr0)
	TTNOP
	TTNOP
	SFPSTORE	1, L3, 2
	SFPSTORE	10, L2, 2
	SFPSTORE	20, L1, 2
	SFPSTORE	30, L0, 2
	ret
	.size	_Z10divp2_boths, .-_Z10divp2_boths
	.align	2
	.globl	_Z12setexp_i_imms
	.type	_Z12setexp_i_imms, @function
_Z12setexp_i_imms:
	SFPLOAD	L3, 10, 1
	SFPLOAD	L2, 20, 1
	SFPLOAD	L1, 30, 1
	SFPLOAD	L0, 40, 1
	SFPSETEXP	L3, L3, 170, 1
	TTNOP
	TTNOP
	SFPSETEXP	L2, L2, 170, 1
	TTNOP
	TTNOP
	SFPSETEXP	L1, L1, 170, 1
	TTNOP
	TTNOP
	SFPSETEXP	L0, L0, 170, 1
	TTNOP
	TTNOP
	SFPSTORE	1, L3, 2
	SFPSTORE	10, L2, 2
	SFPSTORE	20, L1, 2
	SFPSTORE	30, L0, 2
	ret
	.size	_Z12setexp_i_imms, .-_Z12setexp_i_imms
	.align	2
	.globl	_Z12setexp_i_regs
	.type	_Z12setexp_i_regs, @function
_Z12setexp_i_regs:
	SFPLOAD	L3, 10, 1
	SFPLOAD	L2, 20, 1
	SFPLOAD	L1, 30, 1
	SFPLOAD	L0, 40, 1
	slli	a5,a0,20
	lui	a4,%hi(_ZN7ckernel13instrn_bufferE)
	li	a3, 2181038897	# 82000331
	lw	a4,%lo(_ZN7ckernel13instrn_bufferE)(a4)
	srli	a5,a5,8
	add	a5,a5,a3
	sw	a5, 0(a4)	# Op(0x82) - d(lr3) s(lr3)
	TTNOP
	TTNOP
	addi	a5,a0,1
	li	a3,16773120
	slli	a5,a5,12
	and	a5,a5,a3
	li	a2, 2181038625	# 82000221
	add	a5,a5,a2
	sw	a5, 0(a4)	# Op(0x82) - d(lr2) s(lr2)
	TTNOP
	TTNOP
	addi	a5,a0,2
	slli	a5,a5,12
	and	a5,a5,a3
	li	a2, 2181038353	# 82000111
	add	a5,a5,a2
	sw	a5, 0(a4)	# Op(0x82) - d(lr1) s(lr1)
	TTNOP
	TTNOP
	addi	a0,a0,3
	slli	a0,a0,12
	and	a0,a0,a3
	li	a5, 2181038081	# 82000001
	add	a0,a0,a5
	sw	a0, 0(a4)	# Op(0x82) - d(lr0) s(lr0)
	TTNOP
	TTNOP
	SFPSTORE	1, L3, 2
	SFPSTORE	10, L2, 2
	SFPSTORE	20, L1, 2
	SFPSTORE	30, L0, 2
	ret
	.size	_Z12setexp_i_regs, .-_Z12setexp_i_regs
	.align	2
	.globl	_Z13setexp_i_boths
	.type	_Z13setexp_i_boths, @function
_Z13setexp_i_boths:
	SFPLOAD	L3, 10, 1
	SFPLOAD	L2, 20, 1
	SFPLOAD	L1, 30, 1
	SFPLOAD	L0, 40, 1
	SFPSETEXP	L3, L3, 187, 1
	TTNOP
	TTNOP
	SFPSETEXP	L2, L2, 187, 1
	TTNOP
	TTNOP
	slli	a5,a0,20
	lui	a4,%hi(_ZN7ckernel13instrn_bufferE)
	li	a3, 2181038353	# 82000111
	lw	a4,%lo(_ZN7ckernel13instrn_bufferE)(a4)
	srli	a5,a5,8
	add	a5,a5,a3
	sw	a5, 0(a4)	# Op(0x82) - d(lr1) s(lr1)
	TTNOP
	TTNOP
	addi	a0,a0,1
	li	a5,16773120
	slli	a0,a0,12
	and	a0,a0,a5
	li	a5, 2181038081	# 82000001
	add	a0,a0,a5
	sw	a0, 0(a4)	# Op(0x82) - d(lr0) s(lr0)
	TTNOP
	TTNOP
	SFPSTORE	1, L3, 2
	SFPSTORE	10, L2, 2
	SFPSTORE	20, L1, 2
	SFPSTORE	30, L0, 2
	ret
	.size	_Z13setexp_i_boths, .-_Z13setexp_i_boths
	.align	2
	.globl	_Z12setman_i_imms
	.type	_Z12setman_i_imms, @function
_Z12setman_i_imms:
	SFPLOAD	L3, 10, 1
	SFPLOAD	L2, 20, 1
	SFPLOAD	L1, 30, 1
	SFPLOAD	L0, 40, 1
	SFPSETMAN	L3, L3, 170, 1
	TTNOP
	TTNOP
	SFPSETMAN	L2, L2, 170, 1
	TTNOP
	TTNOP
	SFPSETMAN	L1, L1, 170, 1
	TTNOP
	TTNOP
	SFPSETMAN	L0, L0, 170, 1
	TTNOP
	TTNOP
	SFPSTORE	1, L3, 2
	SFPSTORE	10, L2, 2
	SFPSTORE	20, L1, 2
	SFPSTORE	30, L0, 2
	ret
	.size	_Z12setman_i_imms, .-_Z12setman_i_imms
	.align	2
	.globl	_Z12setman_i_regs
	.type	_Z12setman_i_regs, @function
_Z12setman_i_regs:
	SFPLOAD	L2, 10, 1
	SFPLOAD	L1, 20, 1
	SFPLOAD	L0, 30, 1
	slli	a5,a0,20
	lui	a4,%hi(_ZN7ckernel13instrn_bufferE)
	li	a3, 2197815841	# 83000221
	lw	a4,%lo(_ZN7ckernel13instrn_bufferE)(a4)
	srli	a5,a5,8
	add	a5,a5,a3
	sw	a5, 0(a4)	# Op(0x83) - d(lr2) s(lr2)
	TTNOP
	TTNOP
	addi	a5,a0,1
	li	a3,16773120
	slli	a5,a5,12
	and	a5,a5,a3
	li	a2, 2197815569	# 83000111
	add	a5,a5,a2
	sw	a5, 0(a4)	# Op(0x83) - d(lr1) s(lr1)
	TTNOP
	TTNOP
	addi	a0,a0,2
	slli	a0,a0,12
	and	a0,a0,a3
	li	a5, 2197815297	# 83000001
	add	a0,a0,a5
	sw	a0, 0(a4)	# Op(0x83) - d(lr0) s(lr0)
	TTNOP
	TTNOP
	SFPSTORE	1, L2, 2
	SFPSTORE	10, L1, 2
	SFPSTORE	20, L0, 2
	ret
	.size	_Z12setman_i_regs, .-_Z12setman_i_regs
	.align	2
	.globl	_Z13setman_i_boths
	.type	_Z13setman_i_boths, @function
_Z13setman_i_boths:
	SFPLOAD	L2, 10, 1
	SFPLOAD	L1, 20, 1
	SFPLOAD	L0, 30, 1
	SFPSETMAN	L2, L2, 187, 1
	TTNOP
	TTNOP
	SFPSETMAN	L1, L1, 187, 1
	TTNOP
	TTNOP
	slli	a5,a0,20
	lui	a4,%hi(_ZN7ckernel13instrn_bufferE)
	li	a3, 2197815297	# 83000001
	lw	a4,%lo(_ZN7ckernel13instrn_bufferE)(a4)
	srli	a5,a5,8
	add	a5,a5,a3
	sw	a5, 0(a4)	# Op(0x83) - d(lr0) s(lr0)
	TTNOP
	TTNOP
	SFPSTORE	1, L2, 2
	SFPSTORE	10, L1, 2
	SFPSTORE	20, L0, 2
	ret
	.size	_Z13setman_i_boths, .-_Z13setman_i_boths
	.align	2
	.globl	_Z12setsgn_i_imms
	.type	_Z12setsgn_i_imms, @function
_Z12setsgn_i_imms:
	SFPLOAD	L3, 10, 1
	SFPLOAD	L2, 20, 1
	SFPLOAD	L1, 30, 1
	SFPLOAD	L0, 40, 1
	SFPSETSGN	L3, L3, 1962, 1
	TTNOP
	TTNOP
	SFPSETSGN	L2, L2, 1962, 1
	TTNOP
	TTNOP
	SFPSETSGN	L1, L1, 1962, 1
	TTNOP
	TTNOP
	SFPSETSGN	L0, L0, 1962, 1
	TTNOP
	TTNOP
	SFPSTORE	1, L3, 2
	SFPSTORE	10, L2, 2
	SFPSTORE	20, L1, 2
	SFPSTORE	30, L0, 2
	ret
	.size	_Z12setsgn_i_imms, .-_Z12setsgn_i_imms
	.align	2
	.globl	_Z12setsgn_i_regj
	.type	_Z12setsgn_i_regj, @function
_Z12setsgn_i_regj:
	SFPLOAD	L3, 10, 1
	SFPLOAD	L2, 20, 1
	SFPLOAD	L1, 30, 1
	SFPLOAD	L0, 40, 1
	slli	a5,a0,12
	li	a3,16773120
	lui	a4,%hi(_ZN7ckernel13instrn_bufferE)
	and	a5,a5,a3
	li	a2, 2298479409	# 89000331
	lw	a4,%lo(_ZN7ckernel13instrn_bufferE)(a4)
	add	a5,a5,a2
	sw	a5, 0(a4)	# Op(0x89) - d(lr3) s(lr3)
	TTNOP
	TTNOP
	addi	a5,a0,1
	slli	a5,a5,12
	and	a5,a5,a3
	li	a2, 2298479137	# 89000221
	add	a5,a5,a2
	sw	a5, 0(a4)	# Op(0x89) - d(lr2) s(lr2)
	TTNOP
	TTNOP
	addi	a5,a0,2
	slli	a5,a5,12
	and	a5,a5,a3
	li	a2, 2298478865	# 89000111
	add	a5,a5,a2
	sw	a5, 0(a4)	# Op(0x89) - d(lr1) s(lr1)
	TTNOP
	TTNOP
	addi	a0,a0,3
	slli	a0,a0,12
	and	a0,a0,a3
	li	a5, 2298478593	# 89000001
	add	a0,a0,a5
	sw	a0, 0(a4)	# Op(0x89) - d(lr0) s(lr0)
	TTNOP
	TTNOP
	SFPSTORE	1, L3, 2
	SFPSTORE	10, L2, 2
	SFPSTORE	20, L1, 2
	SFPSTORE	30, L0, 2
	ret
	.size	_Z12setsgn_i_regj, .-_Z12setsgn_i_regj
	.align	2
	.globl	_Z13setsgn_i_boths
	.type	_Z13setsgn_i_boths, @function
_Z13setsgn_i_boths:
	SFPLOAD	L3, 10, 1
	SFPLOAD	L2, 20, 1
	SFPLOAD	L1, 30, 1
	SFPLOAD	L0, 40, 1
	SFPSETSGN	L3, L3, 1979, 1
	TTNOP
	TTNOP
	SFPSETSGN	L2, L2, 1979, 1
	TTNOP
	TTNOP
	slli	a5,a0,20
	lui	a4,%hi(_ZN7ckernel13instrn_bufferE)
	li	a3, 2298478865	# 89000111
	lw	a4,%lo(_ZN7ckernel13instrn_bufferE)(a4)
	srli	a5,a5,8
	add	a5,a5,a3
	sw	a5, 0(a4)	# Op(0x89) - d(lr1) s(lr1)
	TTNOP
	TTNOP
	addi	a0,a0,1
	li	a5,16773120
	slli	a0,a0,12
	and	a0,a0,a5
	li	a5, 2298478593	# 89000001
	add	a0,a0,a5
	sw	a0, 0(a4)	# Op(0x89) - d(lr0) s(lr0)
	TTNOP
	TTNOP
	SFPSTORE	1, L3, 2
	SFPSTORE	10, L2, 2
	SFPSTORE	20, L1, 2
	SFPSTORE	30, L0, 2
	ret
	.size	_Z13setsgn_i_boths, .-_Z13setsgn_i_boths
	.align	2
	.globl	_Z11multi_reusei
	.type	_Z11multi_reusei, @function
_Z11multi_reusei:
	SFPLOAD	L0, 0, 0
	li	a5,16773120
	slli	a0,a0,12
	and	a0,a0,a5
	li	a5, 2298478593	# 89000001
	add	a5,a0,a5
	li	a3,0
	sw	a5, 0(a3)	# Op(0x89) - d(lr0) s(lr0)
	TTNOP
	TTNOP
	li	a5, 2298478593	# 89000001
	add	a5,a0,a5
	sw	a5, 0(a3)	# Op(0x89) - d(lr0) s(lr0)
	TTNOP
	TTNOP
	li	a5, 2298478593	# 89000001
	add	a5,a0,a5
	sw	a5, 0(a3)	# Op(0x89) - d(lr0) s(lr0)
	TTNOP
	TTNOP
	li	a5, 2298478593	# 89000001
	add	a0,a0,a5
	sw	a0, 0(a3)	# Op(0x89) - d(lr0) s(lr0)
	TTNOP
	TTNOP
	SFPSTORE	0, L0, 2
	ret
	.size	_Z11multi_reusei, .-_Z11multi_reusei
	.align	2
	.globl	_Z10loop_macroi
	.type	_Z10loop_macroi, @function
_Z10loop_macroi:
	lui	a5,%hi(_ZN7ckernel13instrn_bufferE)
	lw	a5,%lo(_ZN7ckernel13instrn_bufferE)(a5)
	li	t4,1881538560
	add	t4,a0,t4
	li	t3,1898315776
	sw	t4,0(a5)
	add	t3,a0,t3
	li	t1,1915092992
	li	a7,2030043136
	sw	t3,0(a5)
	slli	a4,a0,12
	add	t1,a0,t1
	addi	a7,a7,565
	li	a3,1962934272
	sw	t1,0(a5)
	add	a7,a4,a7
	addi	a3,a3,42
	slli	a0,a0,8
	li	a6,2046820352
	sw	a7,0(a5)
	add	a0,a0,a3
	addi	a6,a6,561
	li	a1,1979711488
	sw	a0,0(a5)
	add	a6,a4,a6
	addi	a1,a1,561
	li	a2,-2113929216
	sw	a6,0(a5)
	add	a1,a4,a1
	addi	a2,a2,561
	li	a3,-2097152000
	sw	a1,0(a5)
	add	a2,a4,a2
	addi	a3,a3,561
	li	t5,-1996488704
	sw	a2,0(a5)
	add	a3,a4,a3
	addi	t5,t5,561
	sw	a3,0(a5)
	add	a4,a4,t5
	sw	a4,0(a5)
	sw	t4,0(a5)
	sw	t3,0(a5)
	sw	t1,0(a5)
	sw	a7,0(a5)
	sw	a0,0(a5)
	sw	a6,0(a5)
	sw	a1,0(a5)
	sw	a2,0(a5)
	sw	a3,0(a5)
	sw	a4,0(a5)
	sw	t4,0(a5)
	sw	t3,0(a5)
	sw	t1,0(a5)
	sw	a7,0(a5)
	sw	a0,0(a5)
	sw	a6,0(a5)
	sw	a1,0(a5)
	sw	a2,0(a5)
	sw	a3,0(a5)
	sw	a4,0(a5)
	sw	t4,0(a5)
	sw	t3,0(a5)
	sw	t1,0(a5)
	sw	a7,0(a5)
	sw	a0,0(a5)
	sw	a6,0(a5)
	sw	a1,0(a5)
	sw	a2,0(a5)
	sw	a3,0(a5)
	sw	a4,0(a5)
	ret
	.size	_Z10loop_macroi, .-_Z10loop_macroi
	.align	2
	.globl	_Z13loop_compileri
	.type	_Z13loop_compileri, @function
_Z13loop_compileri:
	slli	a0,a0,16
	srai	a0,a0,16
	li	a3,65536
	addi	a3,a3,-1
	addi	a6,a0,10
	addi	a2,a0,20
	addi	a4,a0,30
	lui	a5,%hi(_ZN7ckernel13instrn_bufferE)
	and	a1,a0,a3
	and	a6,a6,a3
	and	a0,a2,a3
	and	a4,a4,a3
	lw	a5,%lo(_ZN7ckernel13instrn_bufferE)(a5)
	li	a3, 1882259456	# 70310000
	add	a1,a1,a3
	sw	a1, 0(a5)	# Op(0x70) - d(lr3)
	li	a2, 1881210880	# 70210000
	add	a2,a2,a6
	sw	a2, 0(a5)	# Op(0x70) - d(lr2)
	li	a3, 1880162304	# 70110000
	add	a3,a3,a0
	sw	a3, 0(a5)	# Op(0x70) - d(lr1)
	li	a0, 1879113728	# 70010000
	add	a4,a4,a0
	sw	a4, 0(a5)	# Op(0x70) - d(lr0)
	SFPSTORE	1, L3, 2
	SFPSTORE	10, L2, 2
	SFPSTORE	20, L1, 2
	SFPSTORE	30, L0, 2
	TTNOP
	sw	a1, 0(a5)	# Op(0x70) - d(lr3)
	sw	a2, 0(a5)	# Op(0x70) - d(lr2)
	sw	a3, 0(a5)	# Op(0x70) - d(lr1)
	sw	a4, 0(a5)	# Op(0x70) - d(lr0)
	SFPSTORE	1, L3, 2
	SFPSTORE	10, L2, 2
	SFPSTORE	20, L1, 2
	SFPSTORE	30, L0, 2
	TTNOP
	sw	a1, 0(a5)	# Op(0x70) - d(lr3)
	sw	a2, 0(a5)	# Op(0x70) - d(lr2)
	sw	a3, 0(a5)	# Op(0x70) - d(lr1)
	sw	a4, 0(a5)	# Op(0x70) - d(lr0)
	SFPSTORE	1, L3, 2
	SFPSTORE	10, L2, 2
	SFPSTORE	20, L1, 2
	SFPSTORE	30, L0, 2
	TTNOP
	sw	a1, 0(a5)	# Op(0x70) - d(lr3)
	sw	a2, 0(a5)	# Op(0x70) - d(lr2)
	sw	a3, 0(a5)	# Op(0x70) - d(lr1)
	sw	a4, 0(a5)	# Op(0x70) - d(lr0)
	SFPSTORE	1, L3, 2
	SFPSTORE	10, L2, 2
	SFPSTORE	20, L1, 2
	SFPSTORE	30, L0, 2
	ret
	.size	_Z13loop_compileri, .-_Z13loop_compileri
	.section	.rodata.str1.4,"aMS",@progbits,1
	.align	2
.LC0:
	.string	"%p\n"
	.section	.text.startup,"ax",@progbits
	.align	2
	.globl	main
	.type	main, @function
main:
	addi	sp,sp,-32
	sw	s1,20(sp)
	mv	s1,a0
	sw	ra,28(sp)
	sw	s0,24(sp)
	sw	s2,16(sp)
	sw	s3,12(sp)
	sw	s4,8(sp)
	slli	s0,s1,16
	call	_Z5macroi
	mv	a0,s1
	srai	s0,s0,16
	call	_Z13macro_checkedi
	SFPLOAD	L3, 10, 1
	SFPLOAD	L2, 20, 1
	SFPLOAD	L1, 30, 1
	SFPLOAD	L0, 40, 1
	SFPSTORE	1, L3, 2
	SFPSTORE	10, L2, 2
	SFPSTORE	20, L1, 2
	SFPSTORE	30, L0, 2
	mv	a0,s0
	call	_Z8load_regs
	mv	a0,s0
	call	_Z9load_boths
	SFPLOADI	L3, 10, 0
	SFPLOADI	L2, 10, 4
	SFPLOADI	L1, 20, 8
	SFPLOADI	L0, 30, 12
	SFPSTORE	1, L3, 2
	SFPSTORE	10, L2, 2
	SFPSTORE	20, L1, 2
	SFPSTORE	30, L0, 2
	TTNOP
	mv	a0,s0
	call	_Z9loadi_regs
	mv	a0,s0
	call	_Z10loadi_boths
	SFPLOAD	L0, 10, 1
	SFPSTORE	10, L0, 2
	SFPSTORE	20, L0, 2
	SFPSTORE	30, L0, 2
	SFPSTORE	40, L0, 2
	TTNOP
	SFPLOAD	L0, 10, 1
	li	s3,65536
	addi	s3,s3,-1
	lui	s4,%hi(_ZN7ckernel13instrn_bufferE)
	and	s2,s0,s3
	lw	a5,%lo(_ZN7ckernel13instrn_bufferE)(s4)
	li	a4, 1912733696	# 72020000
	add	a4,s2,a4
	sw	a4, 0(a5)	# Op(0x72) s(lr0)
	addi	a4,s0,1
	and	a4,a4,s3
	li	a3, 1912733696	# 72020000
	add	a3,a4,a3
	sw	a3, 0(a5)	# Op(0x72) s(lr0)
	addi	a3,s0,2
	and	a3,a3,s3
	li	a2, 1912733696	# 72020000
	add	a2,a3,a2
	sw	a2, 0(a5)	# Op(0x72) s(lr0)
	addi	a2,s0,3
	and	a2,a2,s3
	li	a1, 1912733696	# 72020000
	add	a1,a2,a1
	sw	a1, 0(a5)	# Op(0x72) s(lr0)
	TTNOP
	SFPLOAD	L0, 10, 1
	SFPSTORE	10, L0, 2
	SFPSTORE	20, L0, 2
	li	a1, 1912733696	# 72020000
	add	a1,a4,a1
	sw	a1, 0(a5)	# Op(0x72) s(lr0)
	li	a1, 1912733696	# 72020000
	add	a1,a3,a1
	sw	a1, 0(a5)	# Op(0x72) s(lr0)
	SFPSTORE	10, L13, 2
	SFPSTORE	20, L13, 2
	SFPSTORE	10, L13, 2
	SFPSTORE	20, L13, 2
	li	a1, 1926365184	# 72d20000
	add	a1,s2,a1
	sw	a1, 0(a5)	# Op(0x72) s(lr13)
	li	a1, 1926365184	# 72d20000
	add	a1,a4,a1
	sw	a1, 0(a5)	# Op(0x72) s(lr13)
	li	a1, 1926365184	# 72d20000
	add	a3,a3,a1
	sw	a3, 0(a5)	# Op(0x72) s(lr13)
	li	a3, 1926365184	# 72d20000
	add	a2,a2,a3
	sw	a2, 0(a5)	# Op(0x72) s(lr13)
	SFPSTORE	10, L13, 2
	SFPSTORE	20, L13, 2
	li	a3, 1926365184	# 72d20000
	add	a3,s2,a3
	sw	a3, 0(a5)	# Op(0x72) s(lr13)
	li	a3, 1926365184	# 72d20000
	add	a4,a4,a3
	sw	a4, 0(a5)	# Op(0x72) s(lr13)
	TTNOP
	mv	a0,s0
	call	_Z8addi_imms
	mv	a0,s0
	call	_Z8addi_regs
	mv	a0,s0
	call	_Z9addi_boths
	mv	a0,s0
	call	_Z11xiadd_i_imms
	mv	a0,s0
	call	_Z11xiadd_i_regs
	mv	a0,s0
	call	_Z12xiadd_i_boths
	mv	a0,s0
	call	_Z10shft_i_imms
	mv	a0,s0
	call	_Z10shft_i_regs
	mv	a0,s0
	call	_Z11shft_i_boths
	mv	a0,s0
	call	_Z9divp2_imms
	mv	a0,s0
	call	_Z9divp2_regs
	mv	a0,s0
	call	_Z10divp2_boths
	mv	a0,s0
	call	_Z12setexp_i_imms
	mv	a0,s0
	call	_Z12setexp_i_regs
	mv	a0,s0
	call	_Z13setexp_i_boths
	mv	a0,s0
	call	_Z12setman_i_imms
	mv	a0,s0
	call	_Z12setman_i_regs
	mv	a0,s0
	call	_Z13setman_i_boths
	mv	a0,s0
	call	_Z12setsgn_i_imms
	mv	a0,s1
	call	_Z12setsgn_i_regj
	mv	a0,s0
	call	_Z13setsgn_i_boths
	SFPLOAD	L0, 0, 0
	slli	a4,s1,12
	li	a5,16773120
	and	a5,a4,a5
	li	a3, 2298478593	# 89000001
	add	a3,a5,a3
	li	a1,0
	sw	a3, 0(a1)	# Op(0x89) - d(lr0) s(lr0)
	TTNOP
	TTNOP
	li	a3, 2298478593	# 89000001
	add	a3,a5,a3
	sw	a3, 0(a1)	# Op(0x89) - d(lr0) s(lr0)
	TTNOP
	TTNOP
	li	a3, 2298478593	# 89000001
	add	a3,a5,a3
	sw	a3, 0(a1)	# Op(0x89) - d(lr0) s(lr0)
	TTNOP
	TTNOP
	li	a3, 2298478593	# 89000001
	add	a5,a5,a3
	sw	a5, 0(a1)	# Op(0x89) - d(lr0) s(lr0)
	TTNOP
	TTNOP
	SFPSTORE	0, L0, 2
	TTNOP
	lw	a5,%lo(_ZN7ckernel13instrn_bufferE)(s4)
	li	t3,1881538560
	add	t3,s1,t3
	li	t1,1898315776
	sw	t3,0(a5)
	add	t1,s1,t1
	li	a7,1915092992
	li	a6,2030043136
	sw	t1,0(a5)
	add	a7,s1,a7
	addi	a6,a6,565
	li	a3,1962934272
	sw	a7,0(a5)
	add	a6,a4,a6
	addi	a3,a3,42
	slli	s1,s1,8
	li	a0,2046820352
	sw	a6,0(a5)
	add	s1,s1,a3
	addi	a0,a0,561
	li	a1,1979711488
	sw	s1,0(a5)
	add	a0,a4,a0
	addi	a1,a1,561
	li	a3,-2113929216
	sw	a0,0(a5)
	add	a1,a4,a1
	addi	a3,a3,561
	li	a2,-2097152000
	sw	a1,0(a5)
	add	a3,a4,a3
	addi	a2,a2,561
	li	t4,-1996488704
	sw	a3,0(a5)
	add	a2,a4,a2
	addi	t4,t4,561
	sw	a2,0(a5)
	add	a4,a4,t4
	sw	a4,0(a5)
	sw	t3,0(a5)
	sw	t1,0(a5)
	sw	a7,0(a5)
	sw	a6,0(a5)
	sw	s1,0(a5)
	sw	a0,0(a5)
	sw	a1,0(a5)
	sw	a3,0(a5)
	sw	a2,0(a5)
	sw	a4,0(a5)
	sw	t3,0(a5)
	sw	t1,0(a5)
	sw	a7,0(a5)
	sw	a6,0(a5)
	sw	s1,0(a5)
	sw	a0,0(a5)
	sw	a1,0(a5)
	sw	a3,0(a5)
	sw	a2,0(a5)
	sw	a4,0(a5)
	sw	t3,0(a5)
	sw	t1,0(a5)
	sw	a7,0(a5)
	sw	a6,0(a5)
	sw	s1,0(a5)
	sw	a0,0(a5)
	sw	a1,0(a5)
	sw	a3,0(a5)
	sw	a2,0(a5)
	addi	a0,s0,10
	addi	a3,s0,20
	li	a2, 1882259456	# 70310000
	addi	s0,s0,30
	and	a0,a0,s3
	and	a3,a3,s3
	and	s0,s0,s3
	add	s2,s2,a2
	sw	a4,0(a5)
	sw	s2, 0(a5)	# Op(0x70) - d(lr3)
	lw	a1,%lo(_ZN7ckernel13instrn_bufferE)(s4)
	li	a2, 1881210880	# 70210000
	add	a2,a2,a0
	sw	a2, 0(a1)	# Op(0x70) - d(lr2)
	li	a5, 1880162304	# 70110000
	add	a5,a3,a5
	sw	a5, 0(a1)	# Op(0x70) - d(lr1)
	li	a4, 1879113728	# 70010000
	add	a4,a4,s0
	sw	a4, 0(a1)	# Op(0x70) - d(lr0)
	SFPSTORE	1, L3, 2
	SFPSTORE	10, L2, 2
	SFPSTORE	20, L1, 2
	SFPSTORE	30, L0, 2
	TTNOP
	sw	s2, 0(a1)	# Op(0x70) - d(lr3)
	sw	a2, 0(a1)	# Op(0x70) - d(lr2)
	sw	a5, 0(a1)	# Op(0x70) - d(lr1)
	sw	a4, 0(a1)	# Op(0x70) - d(lr0)
	SFPSTORE	1, L3, 2
	SFPSTORE	10, L2, 2
	SFPSTORE	20, L1, 2
	SFPSTORE	30, L0, 2
	TTNOP
	sw	s2, 0(a1)	# Op(0x70) - d(lr3)
	sw	a2, 0(a1)	# Op(0x70) - d(lr2)
	sw	a5, 0(a1)	# Op(0x70) - d(lr1)
	sw	a4, 0(a1)	# Op(0x70) - d(lr0)
	SFPSTORE	1, L3, 2
	SFPSTORE	10, L2, 2
	SFPSTORE	20, L1, 2
	SFPSTORE	30, L0, 2
	TTNOP
	sw	s2, 0(a1)	# Op(0x70) - d(lr3)
	sw	a2, 0(a1)	# Op(0x70) - d(lr2)
	sw	a5, 0(a1)	# Op(0x70) - d(lr1)
	sw	a4, 0(a1)	# Op(0x70) - d(lr0)
	SFPSTORE	1, L3, 2
	SFPSTORE	10, L2, 2
	SFPSTORE	20, L1, 2
	SFPSTORE	30, L0, 2
	lui	a0,%hi(.LC0)
	addi	a0,a0,%lo(.LC0)
	call	printf
	lw	ra,28(sp)
	lw	s0,24(sp)
	lw	s1,20(sp)
	lw	s2,16(sp)
	lw	s3,12(sp)
	lw	s4,8(sp)
	li	a0,0
	addi	sp,sp,32
	jr	ra
	.size	main, .-main
	.globl	_ZN7ckernel13instrn_bufferE
	.section	.sbss,"aw",@nobits
	.align	2
	.type	_ZN7ckernel13instrn_bufferE, @object
	.size	_ZN7ckernel13instrn_bufferE, 4
_ZN7ckernel13instrn_bufferE:
	.zero	4
	.ident	"GCC: (g323f2891276) 12.2.0"
