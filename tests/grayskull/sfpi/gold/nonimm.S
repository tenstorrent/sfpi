	.file	"nonimm.cc"
	.option nopic
	.attribute arch, "rv32i2p0_y2p0"
	.attribute unaligned_access, 0
	.attribute stack_align, 16
	.text
	.align	1
	.globl	_Z6loadiffi
	.type	_Z6loadiffi, @function
_Z6loadiffi:
	lui	a5,%hi(_ZN7ckernel13instrn_bufferE)
	lw	a5,%lo(_ZN7ckernel13instrn_bufferE)(a5)
	srli	a0,a0,16
	li	a4, 1896873984	# 71100000
	add	a4,a0,a4
	sw	a4, 0(a5)	# Op(0x71) - d(lr1)
	li	a3, 1895825408	# 71000000
	add	a0,a0,a3
	sw	a0, 0(a5)	# Op(0x71) - d(lr0)
	SFPSTORE	0, L1, 0
	SFPSTORE	4, L0, 0
	sw	a4, 0(a5)	# Op(0x71) - d(lr1)
	sw	a0, 0(a5)	# Op(0x71) - d(lr0)
	SFPSTORE	0, L1, 0
	SFPSTORE	4, L0, 0
	sw	a4, 0(a5)	# Op(0x71) - d(lr1)
	sw	a0, 0(a5)	# Op(0x71) - d(lr0)
	SFPSTORE	0, L1, 0
	SFPSTORE	4, L0, 0
	sw	a4, 0(a5)	# Op(0x71) - d(lr1)
	sw	a0, 0(a5)	# Op(0x71) - d(lr0)
	SFPSTORE	0, L1, 0
	SFPSTORE	4, L0, 0
	slli	a1,a1,16
	srli	a1,a1,16
	li	a4, 1895825408	# 71000000
	add	a4,a1,a4
	sw	a4, 0(a5)	# Op(0x71) - d(lr0)
	SFPSTORE	0, L0, 0
	sw	a4, 0(a5)	# Op(0x71) - d(lr0)
	SFPSTORE	0, L0, 0
	sw	a4, 0(a5)	# Op(0x71) - d(lr0)
	SFPSTORE	0, L0, 0
	sw	a4, 0(a5)	# Op(0x71) - d(lr0)
	SFPSTORE	0, L0, 0
	li	a4, 1895890944	# 71010000
	add	a4,a4,a1
	sw	a4, 0(a5)	# Op(0x71) - d(lr0)
	SFPSTORE	0, L0, 0
	sw	a4, 0(a5)	# Op(0x71) - d(lr0)
	SFPSTORE	0, L0, 0
	sw	a4, 0(a5)	# Op(0x71) - d(lr0)
	SFPSTORE	0, L0, 0
	sw	a4, 0(a5)	# Op(0x71) - d(lr0)
	SFPSTORE	0, L0, 0
	li	a4, 1895825408	# 71000000
	add	a1,a4,a1
	sw	a1, 0(a5)	# Op(0x71) - d(lr0)
	SFPSTORE	0, L0, 0
	ret
	.size	_Z6loadiffi, .-_Z6loadiffi
	.align	1
	.globl	_Z9loadif_lvfi
	.type	_Z9loadif_lvfi, @function
_Z9loadif_lvfi:
	SFPLOADI	L0, 16256, 0
	SFPLOADI	L1, 16256, 0
	SFPLOADI	L2, 16384, 0
	SFPMAD	L2, L2, L11, L0, 0
	TTNOP
	TTNOP
	SFPSETCC	L2, 0, 6
	TTNOP
	lui	a5,%hi(_ZN7ckernel13instrn_bufferE)
	lw	a5,%lo(_ZN7ckernel13instrn_bufferE)(a5)
	srli	a4,a0,16
	li	a3, 1895825408	# 71000000
	add	a3,a4,a3
	sw	a3, 0(a5)	# Op(0x71) lv(lr0)  d(lr0)
	li	a2, 1896873984	# 71100000
	add	a4,a4,a2
	sw	a4, 0(a5)	# Op(0x71) lv(lr1)  d(lr1)
	SFPENCC	3, 10
	TTNOP
	SFPSTORE	0, L0, 0
	SFPSTORE	4, L1, 0
	SFPLOADI	L0, 16256, 0
	SFPLOADI	L1, 16256, 0
	SFPLOADI	L2, 16384, 0
	SFPMAD	L2, L2, L11, L0, 0
	TTNOP
	TTNOP
	SFPSETCC	L2, 0, 6
	TTNOP
	sw	a3, 0(a5)	# Op(0x71) lv(lr0)  d(lr0)
	sw	a4, 0(a5)	# Op(0x71) lv(lr1)  d(lr1)
	SFPENCC	3, 10
	TTNOP
	SFPSTORE	0, L0, 0
	SFPSTORE	4, L1, 0
	SFPLOADI	L0, 16256, 0
	SFPLOADI	L1, 16256, 0
	SFPLOADI	L2, 16384, 0
	SFPMAD	L2, L2, L11, L0, 0
	TTNOP
	TTNOP
	SFPSETCC	L2, 0, 6
	TTNOP
	sw	a3, 0(a5)	# Op(0x71) lv(lr0)  d(lr0)
	sw	a4, 0(a5)	# Op(0x71) lv(lr1)  d(lr1)
	SFPENCC	3, 10
	TTNOP
	SFPSTORE	0, L0, 0
	SFPSTORE	4, L1, 0
	SFPLOADI	L0, 16256, 0
	SFPLOADI	L1, 16256, 0
	SFPLOADI	L2, 16384, 0
	SFPMAD	L2, L2, L11, L0, 0
	TTNOP
	TTNOP
	SFPSETCC	L2, 0, 6
	TTNOP
	sw	a3, 0(a5)	# Op(0x71) lv(lr0)  d(lr0)
	sw	a4, 0(a5)	# Op(0x71) lv(lr1)  d(lr1)
	SFPENCC	3, 10
	TTNOP
	SFPSTORE	0, L0, 0
	SFPSTORE	4, L1, 0
	SFPLOADI	L0, 16256, 0
	SFPLOADI	L1, 16384, 0
	SFPMAD	L1, L1, L11, L0, 0
	TTNOP
	TTNOP
	SFPSETCC	L1, 0, 6
	TTNOP
	slli	a1,a1,16
	srli	a1,a1,16
	li	a4, 1895825408	# 71000000
	add	a4,a1,a4
	sw	a4, 0(a5)	# Op(0x71) lv(lr0)  d(lr0)
	SFPENCC	3, 10
	TTNOP
	SFPSTORE	0, L0, 0
	SFPLOADI	L0, 16256, 0
	SFPLOADI	L1, 16384, 0
	SFPMAD	L1, L1, L11, L0, 0
	TTNOP
	TTNOP
	SFPSETCC	L1, 0, 6
	TTNOP
	sw	a4, 0(a5)	# Op(0x71) lv(lr0)  d(lr0)
	SFPENCC	3, 10
	TTNOP
	SFPSTORE	0, L0, 0
	SFPLOADI	L0, 16256, 0
	SFPLOADI	L1, 16384, 0
	SFPMAD	L1, L1, L11, L0, 0
	TTNOP
	TTNOP
	SFPSETCC	L1, 0, 6
	TTNOP
	sw	a4, 0(a5)	# Op(0x71) lv(lr0)  d(lr0)
	SFPENCC	3, 10
	TTNOP
	SFPSTORE	0, L0, 0
	SFPLOADI	L0, 16256, 0
	SFPLOADI	L1, 16384, 0
	SFPMAD	L1, L1, L11, L0, 0
	TTNOP
	TTNOP
	SFPSETCC	L1, 0, 6
	TTNOP
	sw	a4, 0(a5)	# Op(0x71) lv(lr0)  d(lr0)
	SFPENCC	3, 10
	TTNOP
	SFPSTORE	0, L0, 0
	SFPLOADI	L0, 16256, 0
	SFPLOADI	L1, 16384, 0
	SFPMAD	L1, L1, L11, L0, 0
	TTNOP
	TTNOP
	SFPSETCC	L1, 0, 6
	TTNOP
	li	a4, 1895890944	# 71010000
	add	a4,a4,a1
	sw	a4, 0(a5)	# Op(0x71) lv(lr0)  d(lr0)
	SFPENCC	3, 10
	TTNOP
	SFPSTORE	0, L0, 0
	SFPLOADI	L0, 16256, 0
	SFPLOADI	L1, 16384, 0
	SFPMAD	L1, L1, L11, L0, 0
	TTNOP
	TTNOP
	SFPSETCC	L1, 0, 6
	TTNOP
	sw	a4, 0(a5)	# Op(0x71) lv(lr0)  d(lr0)
	SFPENCC	3, 10
	TTNOP
	SFPSTORE	0, L0, 0
	SFPLOADI	L0, 16256, 0
	SFPLOADI	L1, 16384, 0
	SFPMAD	L1, L1, L11, L0, 0
	TTNOP
	TTNOP
	SFPSETCC	L1, 0, 6
	TTNOP
	sw	a4, 0(a5)	# Op(0x71) lv(lr0)  d(lr0)
	SFPENCC	3, 10
	TTNOP
	SFPSTORE	0, L0, 0
	SFPLOADI	L0, 16256, 0
	SFPLOADI	L1, 16384, 0
	SFPMAD	L1, L1, L11, L0, 0
	TTNOP
	TTNOP
	SFPSETCC	L1, 0, 6
	TTNOP
	sw	a4, 0(a5)	# Op(0x71) lv(lr0)  d(lr0)
	SFPENCC	3, 10
	TTNOP
	SFPSTORE	0, L0, 0
	SFPLOADI	L0, 16256, 0
	SFPLOADI	L1, 16384, 0
	SFPMAD	L1, L1, L11, L0, 0
	TTNOP
	TTNOP
	SFPSETCC	L1, 0, 6
	TTNOP
	li	a4, 1895825408	# 71000000
	add	a1,a4,a1
	sw	a1, 0(a5)	# Op(0x71) lv(lr0)  d(lr0)
	SFPENCC	3, 10
	TTNOP
	SFPLOADI	L1, 16384, 0
	SFPMAD	L1, L1, L11, L0, 0
	TTNOP
	TTNOP
	SFPSETCC	L1, 0, 6
	TTNOP
	sw	a1, 0(a5)	# Op(0x71) lv(lr0)  d(lr0)
	SFPENCC	3, 10
	TTNOP
	SFPLOADI	L1, 16384, 0
	SFPMAD	L1, L1, L11, L0, 0
	TTNOP
	TTNOP
	SFPSETCC	L1, 0, 6
	TTNOP
	sw	a1, 0(a5)	# Op(0x71) lv(lr0)  d(lr0)
	SFPENCC	3, 10
	TTNOP
	SFPLOADI	L1, 16384, 0
	SFPMAD	L1, L1, L11, L0, 0
	TTNOP
	TTNOP
	SFPSETCC	L1, 0, 6
	TTNOP
	sw	a1, 0(a5)	# Op(0x71) lv(lr0)  d(lr0)
	SFPENCC	3, 10
	TTNOP
	SFPSTORE	0, L0, 0
	ret
	.size	_Z9loadif_lvfi, .-_Z9loadif_lvfi
	.align	1
	.globl	_Z6loadiii
	.type	_Z6loadiii, @function
_Z6loadiii:
	lui	a5,%hi(_ZN7ckernel13instrn_bufferE)
	slli	a3,a0,16
	lw	a5,%lo(_ZN7ckernel13instrn_bufferE)(a5)
	srli	a3,a3,16
	li	a4, 1896087552	# 71040000
	add	a4,a3,a4
	sw	a4, 0(a5)	# Op(0x71) - d(lr0)
	SFPSTORE	0, L0, 2
	sw	a4, 0(a5)	# Op(0x71) - d(lr0)
	SFPSTORE	0, L0, 2
	sw	a4, 0(a5)	# Op(0x71) - d(lr0)
	SFPSTORE	0, L0, 2
	sw	a4, 0(a5)	# Op(0x71) - d(lr0)
	SFPSTORE	0, L0, 2
	mv	a0,a4
	sw	a0, 0(a5)	# Op(0x71) - d(lr0)
	SFPSTORE	0, L0, 2
	sw	a0, 0(a5)	# Op(0x71) - d(lr0)
	SFPSTORE	0, L0, 2
	sw	a0, 0(a5)	# Op(0x71) - d(lr0)
	SFPSTORE	0, L0, 2
	sw	a0, 0(a5)	# Op(0x71) - d(lr0)
	SFPSTORE	0, L0, 2
	ret
	.size	_Z6loadiii, .-_Z6loadiii
	.align	1
	.globl	_Z9loadii_lvi
	.type	_Z9loadii_lvi, @function
_Z9loadii_lvi:
	SFPLOADI	L0, 1, 4
	SFPIADD	L1, L0, -2, 5
	TTNOP
	TTNOP
	SFPSETCC	L1, 0, 6
	TTNOP
	lui	a5,%hi(_ZN7ckernel13instrn_bufferE)
	lw	a5,%lo(_ZN7ckernel13instrn_bufferE)(a5)
	slli	a0,a0,16
	srli	a0,a0,16
	li	a4, 1896087552	# 71040000
	add	a4,a0,a4
	sw	a4, 0(a5)	# Op(0x71) lv(lr0)  d(lr0)
	SFPENCC	3, 10
	TTNOP
	SFPSTORE	0, L0, 2
	SFPLOADI	L0, 1, 4
	SFPIADD	L1, L0, -2, 5
	TTNOP
	TTNOP
	SFPSETCC	L1, 0, 6
	TTNOP
	sw	a4, 0(a5)	# Op(0x71) lv(lr0)  d(lr0)
	SFPENCC	3, 10
	TTNOP
	SFPSTORE	0, L0, 2
	SFPLOADI	L0, 1, 4
	SFPIADD	L1, L0, -2, 5
	TTNOP
	TTNOP
	SFPSETCC	L1, 0, 6
	TTNOP
	sw	a4, 0(a5)	# Op(0x71) lv(lr0)  d(lr0)
	SFPENCC	3, 10
	TTNOP
	SFPSTORE	0, L0, 2
	SFPLOADI	L0, 1, 4
	SFPIADD	L1, L0, -2, 5
	TTNOP
	TTNOP
	SFPSETCC	L1, 0, 6
	TTNOP
	sw	a4, 0(a5)	# Op(0x71) lv(lr0)  d(lr0)
	SFPENCC	3, 10
	TTNOP
	SFPSTORE	0, L0, 2
	SFPLOADI	L0, 1, 4
	SFPIADD	L1, L0, -2, 5
	TTNOP
	TTNOP
	SFPSETCC	L1, 0, 6
	TTNOP
	li	a4, 1896087552	# 71040000
	add	a4,a4,a0
	sw	a4, 0(a5)	# Op(0x71) lv(lr0)  d(lr0)
	SFPENCC	3, 10
	TTNOP
	SFPSTORE	0, L0, 2
	SFPLOADI	L0, 1, 4
	SFPIADD	L1, L0, -2, 5
	TTNOP
	TTNOP
	SFPSETCC	L1, 0, 6
	TTNOP
	sw	a4, 0(a5)	# Op(0x71) lv(lr0)  d(lr0)
	SFPENCC	3, 10
	TTNOP
	SFPSTORE	0, L0, 2
	SFPLOADI	L0, 1, 4
	SFPIADD	L1, L0, -2, 5
	TTNOP
	TTNOP
	SFPSETCC	L1, 0, 6
	TTNOP
	sw	a4, 0(a5)	# Op(0x71) lv(lr0)  d(lr0)
	SFPENCC	3, 10
	TTNOP
	SFPSTORE	0, L0, 2
	SFPLOADI	L0, 1, 4
	SFPIADD	L1, L0, -2, 5
	TTNOP
	TTNOP
	SFPSETCC	L1, 0, 6
	TTNOP
	sw	a4, 0(a5)	# Op(0x71) lv(lr0)  d(lr0)
	SFPENCC	3, 10
	TTNOP
	SFPSTORE	0, L0, 2
	ret
	.size	_Z9loadii_lvi, .-_Z9loadii_lvi
	.align	1
	.globl	_Z9loadstorei
	.type	_Z9loadstorei, @function
_Z9loadstorei:
	slli	a4,a0,2
	lui	a5,%hi(_ZN7ckernel13instrn_bufferE)
	li	a0,65536
	addi	a0,a0,-1
	lw	a5,%lo(_ZN7ckernel13instrn_bufferE)(a5)
	and	a4,a4,a0
	li	a1, 1879113728	# 70010000
	add	a1,a4,a1
	sw	a1, 0(a5)	# Op(0x70) - d(lr0)
	li	a2, 1912602624	# 72000000
	add	a2,a4,a2
	sw	a2, 0(a5)	# Op(0x72) s(lr0)
	SFPLOADI	L0, 1, 4
	li	a3, 1912733696	# 72020000
	add	a3,a4,a3
	sw	a3, 0(a5)	# Op(0x72) s(lr0)
	SFPLOADI	L0, 2, 4
	mv	a4,a3
	sw	a3, 0(a5)	# Op(0x72) s(lr0)
	sw	a1, 0(a5)	# Op(0x70) - d(lr0)
	sw	a2, 0(a5)	# Op(0x72) s(lr0)
	SFPLOADI	L0, 1, 4
	sw	a3, 0(a5)	# Op(0x72) s(lr0)
	SFPLOADI	L0, 2, 4
	sw	a3, 0(a5)	# Op(0x72) s(lr0)
	sw	a1, 0(a5)	# Op(0x70) - d(lr0)
	sw	a2, 0(a5)	# Op(0x72) s(lr0)
	SFPLOADI	L0, 1, 4
	sw	a3, 0(a5)	# Op(0x72) s(lr0)
	SFPLOADI	L0, 2, 4
	sw	a3, 0(a5)	# Op(0x72) s(lr0)
	sw	a1, 0(a5)	# Op(0x70) - d(lr0)
	sw	a2, 0(a5)	# Op(0x72) s(lr0)
	SFPLOADI	L0, 1, 4
	sw	a3, 0(a5)	# Op(0x72) s(lr0)
	SFPLOADI	L0, 2, 4
	sw	a4, 0(a5)	# Op(0x72) s(lr0)
	ret
	.size	_Z9loadstorei, .-_Z9loadstorei
	.align	1
	.globl	_Z12loadstore_lvi
	.type	_Z12loadstore_lvi, @function
_Z12loadstore_lvi:
	SFPLOADI	L1, 16256, 0
	SFPLOADI	L0, 16384, 0
	SFPMAD	L0, L0, L11, L1, 0
	TTNOP
	TTNOP
	SFPSETCC	L0, 0, 6
	TTNOP
	slli	a5,a0,2
	SFPENCC	3, 10
	TTNOP
	lui	a4,%hi(_ZN7ckernel13instrn_bufferE)
	li	a3,65536
	addi	a3,a3,-1
	lw	a4,%lo(_ZN7ckernel13instrn_bufferE)(a4)
	and	a5,a5,a3
	li	a2, 1913651200	# 72100000
	add	a2,a5,a2
	sw	a2, 0(a4)	# Op(0x72) s(lr1)
	SFPLOADI	L0, 1, 4
	li	a3, 1912733696	# 72020000
	add	a3,a5,a3
	sw	a3, 0(a4)	# Op(0x72) s(lr0)
	SFPLOADI	L0, 2, 4
	mv	a5,a3
	sw	a3, 0(a4)	# Op(0x72) s(lr0)
	SFPLOADI	L1, 16256, 0
	SFPLOADI	L0, 16384, 0
	SFPMAD	L0, L0, L11, L1, 0
	TTNOP
	TTNOP
	SFPSETCC	L0, 0, 6
	TTNOP
	SFPENCC	3, 10
	TTNOP
	sw	a2, 0(a4)	# Op(0x72) s(lr1)
	SFPLOADI	L0, 1, 4
	sw	a3, 0(a4)	# Op(0x72) s(lr0)
	SFPLOADI	L0, 2, 4
	sw	a3, 0(a4)	# Op(0x72) s(lr0)
	SFPLOADI	L1, 16256, 0
	SFPLOADI	L0, 16384, 0
	SFPMAD	L0, L0, L11, L1, 0
	TTNOP
	TTNOP
	SFPSETCC	L0, 0, 6
	TTNOP
	SFPENCC	3, 10
	TTNOP
	sw	a2, 0(a4)	# Op(0x72) s(lr1)
	SFPLOADI	L0, 1, 4
	sw	a3, 0(a4)	# Op(0x72) s(lr0)
	SFPLOADI	L0, 2, 4
	sw	a3, 0(a4)	# Op(0x72) s(lr0)
	SFPLOADI	L1, 16256, 0
	SFPLOADI	L0, 16384, 0
	SFPMAD	L0, L0, L11, L1, 0
	TTNOP
	TTNOP
	SFPSETCC	L0, 0, 6
	TTNOP
	SFPENCC	3, 10
	TTNOP
	sw	a2, 0(a4)	# Op(0x72) s(lr1)
	SFPLOADI	L0, 1, 4
	sw	a3, 0(a4)	# Op(0x72) s(lr0)
	SFPLOADI	L0, 2, 4
	sw	a5, 0(a4)	# Op(0x72) s(lr0)
	ret
	.size	_Z12loadstore_lvi, .-_Z12loadstore_lvi
	.align	1
	.globl	_Z17loadstorefallbacki
	.type	_Z17loadstorefallbacki, @function
_Z17loadstorefallbacki:
	slli	a5,a0,2
	lui	a4,%hi(_ZN7ckernel13instrn_bufferE)
	li	a0,65536
	addi	a0,a0,-1
	lw	a4,%lo(_ZN7ckernel13instrn_bufferE)(a4)
	and	a5,a5,a0
	li	a3, 1879113728	# 70010000
	add	a3,a5,a3
	sw	a3, 0(a4)	# Op(0x70) - d(lr0)
	mv	a5,a3
	sw	a3, 0(a4)	# Op(0x70) - d(lr0)
	sw	a3, 0(a4)	# Op(0x70) - d(lr0)
	sw	a3, 0(a4)	# Op(0x70) - d(lr0)
	sw	a3, 0(a4)	# Op(0x70) - d(lr0)
	sw	a3, 0(a4)	# Op(0x70) - d(lr0)
	li	a2, 1048576
	xor	a2, a2, a3
	sw	a2, 0(a4)	# Op(0x70) - d(lr1)
	sw	a5, 0(a4)	# Op(0x70) - d(lr0)
	SFPSTORE	0, L1, 0
	SFPSTORE	0, L0, 0
	ret
	.size	_Z17loadstorefallbacki, .-_Z17loadstorefallbacki
	.align	1
	.globl	_Z20loadstorefallback_lvi
	.type	_Z20loadstorefallback_lvi, @function
_Z20loadstorefallback_lvi:
	SFPLOADI	L0, 16256, 0
	SFPLOADI	L1, 16256, 0
	SFPMAD	L2, L10, L11, L0, 0
	TTNOP
	TTNOP
	SFPSETCC	L2, 0, 6
	TTNOP
	lui	a4,%hi(_ZN7ckernel13instrn_bufferE)
	li	a3,65536
	slli	a5,a0,2
	addi	a3,a3,-1
	lw	a4,%lo(_ZN7ckernel13instrn_bufferE)(a4)
	and	a5,a5,a3
	li	a3, 1879113728	# 70010000
	add	a3,a5,a3
	sw	a3, 0(a4)	# Op(0x70) lv(lr0)  d(lr0)
	li	a2, 1880162304	# 70110000
	add	a5,a5,a2
	sw	a5, 0(a4)	# Op(0x70) lv(lr1)  d(lr1)
	SFPENCC	3, 10
	TTNOP
	SFPMAD	L2, L10, L11, L0, 0
	TTNOP
	TTNOP
	SFPSETCC	L2, 0, 6
	TTNOP
	sw	a3, 0(a4)	# Op(0x70) lv(lr0)  d(lr0)
	sw	a5, 0(a4)	# Op(0x70) lv(lr1)  d(lr1)
	SFPENCC	3, 10
	TTNOP
	SFPMAD	L2, L10, L11, L0, 0
	TTNOP
	TTNOP
	SFPSETCC	L2, 0, 6
	TTNOP
	sw	a3, 0(a4)	# Op(0x70) lv(lr0)  d(lr0)
	sw	a5, 0(a4)	# Op(0x70) lv(lr1)  d(lr1)
	SFPENCC	3, 10
	TTNOP
	SFPMAD	L2, L10, L11, L0, 0
	TTNOP
	TTNOP
	SFPSETCC	L2, 0, 6
	TTNOP
	sw	a3, 0(a4)	# Op(0x70) lv(lr0)  d(lr0)
	sw	a5, 0(a4)	# Op(0x70) lv(lr1)  d(lr1)
	SFPENCC	3, 10
	TTNOP
	SFPSTORE	0, L0, 0
	SFPSTORE	0, L1, 0
	ret
	.size	_Z20loadstorefallback_lvi, .-_Z20loadstorefallback_lvi
	.align	1
	.globl	_Z5fcmpsfi
	.type	_Z5fcmpsfi, @function
_Z5fcmpsfi:
	SFPLOADI	L0, 16256, 0
	li	a3,65536
	lui	a5,%hi(_ZN7ckernel13instrn_bufferE)
	srli	a0,a0,16
	addi	a3,a3,-1
	lw	a5,%lo(_ZN7ckernel13instrn_bufferE)(a5)
	and	a4,a0,a3
	li	a2, 1896873984	# 71100000
	add	a4,a4,a2
	sw	a4, 0(a5)	# Op(0x71) - d(lr1)
	SFPMAD	L1, L1, L11, L0, 0
	TTNOP
	TTNOP
	SFPSETCC	L1, 0, 6
	TTNOP
	SFPSTORE	0, L0, 0
	SFPENCC	3, 10
	TTNOP
	and	a3,a1,a3
	li	a2, 1896873984	# 71100000
	add	a2,a3,a2
	sw	a2, 0(a5)	# Op(0x71) - d(lr1)
	SFPMAD	L1, L1, L11, L0, 0
	TTNOP
	TTNOP
	SFPSETCC	L1, 0, 6
	TTNOP
	SFPSTORE	0, L0, 0
	SFPENCC	3, 10
	TTNOP
	li	a1, 1896939520	# 71110000
	add	a3,a3,a1
	sw	a3, 0(a5)	# Op(0x71) - d(lr1)
	SFPMAD	L1, L1, L11, L0, 0
	TTNOP
	TTNOP
	SFPSETCC	L1, 0, 6
	TTNOP
	SFPSTORE	0, L0, 0
	SFPENCC	3, 10
	TTNOP
	SFPLOADI	L1, 16448, 0
	SFPMAD	L1, L1, L11, L0, 0
	TTNOP
	TTNOP
	SFPSETCC	L1, 0, 6
	TTNOP
	SFPSTORE	0, L0, 0
	SFPENCC	3, 10
	TTNOP
	sw	a4, 0(a5)	# Op(0x71) - d(lr1)
	SFPMAD	L1, L1, L11, L0, 0
	TTNOP
	TTNOP
	SFPSETCC	L1, 0, 6
	TTNOP
	SFPSTORE	0, L0, 0
	SFPENCC	3, 10
	TTNOP
	sw	a2, 0(a5)	# Op(0x71) - d(lr1)
	SFPMAD	L1, L1, L11, L0, 0
	TTNOP
	TTNOP
	SFPSETCC	L1, 0, 6
	TTNOP
	SFPSTORE	0, L0, 0
	SFPENCC	3, 10
	TTNOP
	sw	a3, 0(a5)	# Op(0x71) - d(lr1)
	SFPMAD	L1, L1, L11, L0, 0
	TTNOP
	TTNOP
	SFPSETCC	L1, 0, 6
	TTNOP
	SFPSTORE	0, L0, 0
	SFPENCC	3, 10
	TTNOP
	SFPLOADI	L1, 16448, 0
	SFPMAD	L1, L1, L11, L0, 0
	TTNOP
	TTNOP
	SFPSETCC	L1, 0, 6
	TTNOP
	SFPSTORE	0, L0, 0
	SFPENCC	3, 10
	TTNOP
	sw	a4, 0(a5)	# Op(0x71) - d(lr1)
	SFPMAD	L1, L1, L11, L0, 0
	TTNOP
	TTNOP
	SFPSETCC	L1, 0, 6
	TTNOP
	SFPSTORE	0, L0, 0
	SFPENCC	3, 10
	TTNOP
	sw	a2, 0(a5)	# Op(0x71) - d(lr1)
	SFPMAD	L1, L1, L11, L0, 0
	TTNOP
	TTNOP
	SFPSETCC	L1, 0, 6
	TTNOP
	SFPSTORE	0, L0, 0
	SFPENCC	3, 10
	TTNOP
	sw	a3, 0(a5)	# Op(0x71) - d(lr1)
	SFPMAD	L1, L1, L11, L0, 0
	TTNOP
	TTNOP
	SFPSETCC	L1, 0, 6
	TTNOP
	SFPSTORE	0, L0, 0
	SFPENCC	3, 10
	TTNOP
	SFPLOADI	L1, 16448, 0
	SFPMAD	L1, L1, L11, L0, 0
	TTNOP
	TTNOP
	SFPSETCC	L1, 0, 6
	TTNOP
	SFPSTORE	0, L0, 0
	SFPENCC	3, 10
	TTNOP
	sw	a4, 0(a5)	# Op(0x71) - d(lr1)
	SFPMAD	L1, L1, L11, L0, 0
	TTNOP
	TTNOP
	SFPSETCC	L1, 0, 6
	TTNOP
	SFPSTORE	0, L0, 0
	SFPENCC	3, 10
	TTNOP
	sw	a2, 0(a5)	# Op(0x71) - d(lr1)
	SFPMAD	L1, L1, L11, L0, 0
	TTNOP
	TTNOP
	SFPSETCC	L1, 0, 6
	TTNOP
	SFPSTORE	0, L0, 0
	SFPENCC	3, 10
	TTNOP
	sw	a3, 0(a5)	# Op(0x71) - d(lr1)
	SFPMAD	L1, L1, L11, L0, 0
	TTNOP
	TTNOP
	SFPSETCC	L1, 0, 6
	TTNOP
	SFPSTORE	0, L0, 0
	SFPENCC	3, 10
	TTNOP
	SFPLOADI	L1, 16448, 0
	SFPMAD	L1, L1, L11, L0, 0
	TTNOP
	TTNOP
	SFPSETCC	L1, 0, 6
	TTNOP
	SFPSTORE	0, L0, 0
	SFPENCC	3, 10
	TTNOP
	ret
	.size	_Z5fcmpsfi, .-_Z5fcmpsfi
	.align	1
	.globl	_Z7muladdffi
	.type	_Z7muladdffi, @function
_Z7muladdffi:
	li	a4,65536
	lui	a5,%hi(_ZN7ckernel13instrn_bufferE)
	addi	a4,a4,-1
	lw	a5,%lo(_ZN7ckernel13instrn_bufferE)(a5)
	and	a1,a1,a4
	li	a3, 1896873984	# 71100000
	add	a3,a1,a3
	sw	a3, 0(a5)	# Op(0x71) - d(lr1)
	SFPMULI	L1, 16256, 0
	TTNOP
	TTNOP
	li	a6, 1895890944	# 71010000
	add	a6,a1,a6
	sw	a6, 0(a5)	# Op(0x71) - d(lr0)
	SFPMUL	L1, L1, L0, L4, 0
	TTNOP
	TTNOP
	srli	a3,a0,16
	and	a4,a3,a4
	li	a2, 1946157072	# 74000010
	slli	a4,a4,8
	add	a4,a4,a2
	sw	a4, 0(a5)	# Op(0x74) lv(lr1)  d(lr1)
	TTNOP
	TTNOP
	slli	a0,a1,8
	li	a2, 1962934288	# 75000010
	add	a2,a0,a2
	sw	a2, 0(a5)	# Op(0x75) lv(lr1)  d(lr1)
	TTNOP
	TTNOP
	mv	a1,a6
	sw	a6, 0(a5)	# Op(0x71) - d(lr0)
	SFPADD	L1, L10, L1, L0, 0
	TTNOP
	TTNOP
	li	a7, 1895825408	# 71000000
	add	a3,a3,a7
	sw	a3, 0(a5)	# Op(0x71) - d(lr0)
	SFPADD	L0, L10, L1, L0, 0
	TTNOP
	TTNOP
	SFPSTORE	0, L0, 0
	SFPSTORE	4, L1, 0
	li	a7, 1946157056	# 74000000
	add	a0,a0,a7
	sw	a0, 0(a5)	# Op(0x74) lv(lr0)  d(lr0)
	TTNOP
	TTNOP
	li	a7, 1048576
	xor	a7, a7, a6
	sw	a7, 0(a5)	# Op(0x71) - d(lr1)
	SFPMUL	L1, L0, L1, L4, 0
	TTNOP
	TTNOP
	sw	a4, 0(a5)	# Op(0x74) lv(lr1)  d(lr1)
	TTNOP
	TTNOP
	sw	a2, 0(a5)	# Op(0x75) lv(lr1)  d(lr1)
	TTNOP
	TTNOP
	sw	a6, 0(a5)	# Op(0x71) - d(lr0)
	SFPADD	L1, L10, L1, L0, 0
	TTNOP
	TTNOP
	sw	a3, 0(a5)	# Op(0x71) - d(lr0)
	SFPADD	L0, L10, L1, L0, 0
	TTNOP
	TTNOP
	SFPSTORE	0, L0, 0
	SFPSTORE	4, L1, 0
	sw	a0, 0(a5)	# Op(0x74) lv(lr0)  d(lr0)
	TTNOP
	TTNOP
	li	a7, 1048576
	xor	a7, a7, a6
	sw	a7, 0(a5)	# Op(0x71) - d(lr1)
	SFPMUL	L1, L0, L1, L4, 0
	TTNOP
	TTNOP
	sw	a4, 0(a5)	# Op(0x74) lv(lr1)  d(lr1)
	TTNOP
	TTNOP
	sw	a2, 0(a5)	# Op(0x75) lv(lr1)  d(lr1)
	TTNOP
	TTNOP
	sw	a6, 0(a5)	# Op(0x71) - d(lr0)
	SFPADD	L1, L10, L1, L0, 0
	TTNOP
	TTNOP
	sw	a3, 0(a5)	# Op(0x71) - d(lr0)
	SFPADD	L0, L10, L1, L0, 0
	TTNOP
	TTNOP
	SFPSTORE	0, L0, 0
	SFPSTORE	4, L1, 0
	sw	a0, 0(a5)	# Op(0x74) lv(lr0)  d(lr0)
	TTNOP
	TTNOP
	li	a0, 1048576
	xor	a0, a0, a6
	sw	a0, 0(a5)	# Op(0x71) - d(lr1)
	SFPMUL	L0, L0, L1, L4, 0
	TTNOP
	TTNOP
	li	a0, 16
	xor	a0, a0, a4
	sw	a0, 0(a5)	# Op(0x74) lv(lr0)  d(lr0)
	TTNOP
	TTNOP
	li	a4, 16
	xor	a4, a4, a2
	sw	a4, 0(a5)	# Op(0x75) lv(lr0)  d(lr0)
	TTNOP
	TTNOP
	li	a4, 1048576
	xor	a4, a4, a1
	sw	a4, 0(a5)	# Op(0x71) - d(lr1)
	SFPADD	L0, L10, L0, L1, 0
	TTNOP
	TTNOP
	li	a4, 1048576
	xor	a4, a4, a3
	sw	a4, 0(a5)	# Op(0x71) - d(lr1)
	SFPADD	L1, L10, L0, L1, 0
	TTNOP
	TTNOP
	SFPSTORE	0, L1, 0
	SFPSTORE	4, L0, 0
	ret
	.size	_Z7muladdffi, .-_Z7muladdffi
	.align	1
	.globl	_Z10muladdf_lvfi
	.type	_Z10muladdf_lvfi, @function
_Z10muladdf_lvfi:
	SFPLOADI	L1, 16256, 0
	SFPLOADI	L0, 16256, 0
	SFPMAD	L2, L10, L11, L0, 0
	TTNOP
	TTNOP
	SFPSETCC	L2, 0, 6
	TTNOP
	lui	a5,%hi(_ZN7ckernel13instrn_bufferE)
	slli	a4,a1,16
	lw	a5,%lo(_ZN7ckernel13instrn_bufferE)(a5)
	srli	a4,a4,16
	li	a7, 1897922560	# 71200000
	add	a7,a4,a7
	sw	a7, 0(a5)	# Op(0x71) - d(lr2)
	SFPMUL	L1, L0, L2, L4, 0
	TTNOP
	TTNOP
	li	a6, 1897988096	# 71210000
	add	a6,a4,a6
	sw	a6, 0(a5)	# Op(0x71) - d(lr2)
	SFPMUL	L0, L1, L2, L4, 0
	TTNOP
	TTNOP
	srli	a3,a0,16
	li	a1, 1897922560	# 71200000
	add	a1,a3,a1
	sw	a1, 0(a5)	# Op(0x71) - d(lr2)
	SFPMUL	L1, L0, L2, L4, 0
	TTNOP
	TTNOP
	sw	a7, 0(a5)	# Op(0x71) - d(lr2)
	SFPADD	L0, L10, L1, L2, 0
	TTNOP
	TTNOP
	sw	a6, 0(a5)	# Op(0x71) - d(lr2)
	SFPADD	L1, L10, L0, L2, 0
	TTNOP
	TTNOP
	mv	a3,a1
	sw	a1, 0(a5)	# Op(0x71) - d(lr2)
	SFPADD	L0, L10, L1, L2, 0
	TTNOP
	TTNOP
	SFPSTORE	0, L0, 0
	SFPSTORE	4, L1, 0
	SFPENCC	3, 10
	TTNOP
	SFPMAD	L2, L10, L11, L0, 0
	TTNOP
	TTNOP
	SFPSETCC	L2, 0, 6
	TTNOP
	sw	a7, 0(a5)	# Op(0x71) - d(lr2)
	SFPMUL	L1, L0, L2, L4, 0
	TTNOP
	TTNOP
	sw	a6, 0(a5)	# Op(0x71) - d(lr2)
	SFPMUL	L0, L1, L2, L4, 0
	TTNOP
	TTNOP
	sw	a1, 0(a5)	# Op(0x71) - d(lr2)
	SFPMUL	L1, L0, L2, L4, 0
	TTNOP
	TTNOP
	sw	a7, 0(a5)	# Op(0x71) - d(lr2)
	SFPADD	L0, L10, L1, L2, 0
	TTNOP
	TTNOP
	sw	a6, 0(a5)	# Op(0x71) - d(lr2)
	SFPADD	L1, L10, L0, L2, 0
	TTNOP
	TTNOP
	sw	a1, 0(a5)	# Op(0x71) - d(lr2)
	SFPADD	L0, L10, L1, L2, 0
	TTNOP
	TTNOP
	SFPSTORE	0, L0, 0
	SFPSTORE	4, L1, 0
	SFPENCC	3, 10
	TTNOP
	SFPMAD	L2, L10, L11, L0, 0
	TTNOP
	TTNOP
	SFPSETCC	L2, 0, 6
	TTNOP
	sw	a7, 0(a5)	# Op(0x71) - d(lr2)
	SFPMUL	L1, L0, L2, L4, 0
	TTNOP
	TTNOP
	sw	a6, 0(a5)	# Op(0x71) - d(lr2)
	SFPMUL	L0, L1, L2, L4, 0
	TTNOP
	TTNOP
	sw	a1, 0(a5)	# Op(0x71) - d(lr2)
	SFPMUL	L1, L0, L2, L4, 0
	TTNOP
	TTNOP
	sw	a7, 0(a5)	# Op(0x71) - d(lr2)
	SFPADD	L0, L10, L1, L2, 0
	TTNOP
	TTNOP
	sw	a6, 0(a5)	# Op(0x71) - d(lr2)
	SFPADD	L1, L10, L0, L2, 0
	TTNOP
	TTNOP
	sw	a1, 0(a5)	# Op(0x71) - d(lr2)
	SFPADD	L0, L10, L1, L2, 0
	TTNOP
	TTNOP
	SFPSTORE	0, L0, 0
	SFPSTORE	4, L1, 0
	SFPENCC	3, 10
	TTNOP
	SFPMAD	L2, L10, L11, L0, 0
	TTNOP
	TTNOP
	SFPSETCC	L2, 0, 6
	TTNOP
	sw	a7, 0(a5)	# Op(0x71) - d(lr2)
	SFPMUL	L1, L0, L2, L4, 0
	TTNOP
	TTNOP
	sw	a6, 0(a5)	# Op(0x71) - d(lr2)
	SFPMUL	L0, L1, L2, L4, 0
	TTNOP
	TTNOP
	sw	a1, 0(a5)	# Op(0x71) - d(lr2)
	SFPMUL	L1, L0, L2, L4, 0
	TTNOP
	TTNOP
	sw	a7, 0(a5)	# Op(0x71) - d(lr2)
	SFPADD	L0, L10, L1, L2, 0
	TTNOP
	TTNOP
	sw	a6, 0(a5)	# Op(0x71) - d(lr2)
	SFPADD	L1, L10, L0, L2, 0
	TTNOP
	TTNOP
	sw	a3, 0(a5)	# Op(0x71) - d(lr2)
	SFPADD	L0, L10, L1, L2, 0
	TTNOP
	TTNOP
	SFPSTORE	0, L0, 0
	SFPSTORE	4, L1, 0
	SFPENCC	3, 10
	TTNOP
	ret
	.size	_Z10muladdf_lvfi, .-_Z10muladdf_lvfi
	.align	1
	.globl	_Z5icmpsi
	.type	_Z5icmpsi, @function
_Z5icmpsi:
	SFPLOADI	L1, 1, 4
	SFPLOADI	L0, 1, 4
	lui	a5,%hi(_ZN7ckernel13instrn_bufferE)
	lw	a4,%lo(_ZN7ckernel13instrn_bufferE)(a5)
	slli	a5,a0,16
	li	a3, 1898184704	# 71240000
	srli	a5,a5,16
	add	a5,a5,a3
	sw	a5, 0(a4)	# Op(0x71) - d(lr2)
	SFPIADD	L2, L1, 0, 6
	TTNOP
	TTNOP
	SFPSETCC	L2, 0, 6
	TTNOP
	SFPLOADI	L2, 0, 0
	SFPSTORE	0, L2, 0
	SFPENCC	3, 10
	TTNOP
	SFPIADD	L2, L0, -1, 5
	TTNOP
	TTNOP
	SFPSETCC	L2, 0, 6
	TTNOP
	SFPLOADI	L2, 0, 0
	SFPSTORE	0, L2, 0
	SFPENCC	3, 10
	TTNOP
	sw	a5, 0(a4)	# Op(0x71) - d(lr2)
	SFPIADD	L2, L1, 0, 6
	TTNOP
	TTNOP
	SFPSETCC	L2, 0, 6
	TTNOP
	SFPLOADI	L2, 0, 0
	SFPSTORE	0, L2, 0
	SFPENCC	3, 10
	TTNOP
	SFPIADD	L2, L0, -1, 5
	TTNOP
	TTNOP
	SFPSETCC	L2, 0, 6
	TTNOP
	SFPLOADI	L2, 0, 0
	SFPSTORE	0, L2, 0
	SFPENCC	3, 10
	TTNOP
	sw	a5, 0(a4)	# Op(0x71) - d(lr2)
	SFPIADD	L2, L1, 0, 6
	TTNOP
	TTNOP
	SFPSETCC	L2, 0, 6
	TTNOP
	SFPLOADI	L2, 0, 0
	SFPSTORE	0, L2, 0
	SFPENCC	3, 10
	TTNOP
	SFPIADD	L2, L0, -1, 5
	TTNOP
	TTNOP
	SFPSETCC	L2, 0, 6
	TTNOP
	SFPLOADI	L2, 0, 0
	SFPSTORE	0, L2, 0
	SFPENCC	3, 10
	TTNOP
	sw	a5, 0(a4)	# Op(0x71) - d(lr2)
	SFPIADD	L2, L1, 0, 6
	TTNOP
	TTNOP
	SFPSETCC	L2, 0, 6
	TTNOP
	SFPLOADI	L1, 0, 0
	SFPSTORE	0, L1, 0
	SFPENCC	3, 10
	TTNOP
	SFPIADD	L0, L0, -1, 5
	TTNOP
	TTNOP
	SFPSETCC	L0, 0, 6
	TTNOP
	SFPLOADI	L0, 0, 0
	SFPSTORE	0, L0, 0
	SFPENCC	3, 10
	TTNOP
	ret
	.size	_Z5icmpsi, .-_Z5icmpsi
	.align	1
	.globl	_Z7muladdii
	.type	_Z7muladdii, @function
_Z7muladdii:
	SFPLOADI	L1, 1, 4
	lui	a5,%hi(_ZN7ckernel13instrn_bufferE)
	lw	a4,%lo(_ZN7ckernel13instrn_bufferE)(a5)
	slli	a5,a0,16
	srli	a5,a5,16
	li	a3, 1896087552	# 71040000
	add	a3,a5,a3
	sw	a3, 0(a4)	# Op(0x71) - d(lr0)
	SFPIADD	L0, L1, 0, 4
	TTNOP
	TTNOP
	SFPIADD	L0, L0, 1, 5
	TTNOP
	TTNOP
	li	a2, 1897136128	# 71140000
	add	a5,a5,a2
	sw	a5, 0(a4)	# Op(0x71) - d(lr1)
	SFPIADD	L1, L0, 0, 6
	TTNOP
	TTNOP
	SFPIADD	L1, L1, -1, 5
	TTNOP
	TTNOP
	sw	a3, 0(a4)	# Op(0x71) - d(lr0)
	SFPIADD	L0, L1, 0, 4
	TTNOP
	TTNOP
	SFPIADD	L0, L0, 1, 5
	TTNOP
	TTNOP
	sw	a5, 0(a4)	# Op(0x71) - d(lr1)
	SFPIADD	L1, L0, 0, 6
	TTNOP
	TTNOP
	SFPIADD	L1, L1, -1, 5
	TTNOP
	TTNOP
	sw	a3, 0(a4)	# Op(0x71) - d(lr0)
	SFPIADD	L0, L1, 0, 4
	TTNOP
	TTNOP
	SFPIADD	L0, L0, 1, 5
	TTNOP
	TTNOP
	sw	a5, 0(a4)	# Op(0x71) - d(lr1)
	SFPIADD	L1, L0, 0, 6
	TTNOP
	TTNOP
	SFPIADD	L1, L1, -1, 5
	TTNOP
	TTNOP
	sw	a3, 0(a4)	# Op(0x71) - d(lr0)
	SFPIADD	L0, L1, 0, 4
	TTNOP
	TTNOP
	SFPIADD	L0, L0, 1, 5
	TTNOP
	TTNOP
	sw	a5, 0(a4)	# Op(0x71) - d(lr1)
	SFPIADD	L1, L0, 0, 6
	TTNOP
	TTNOP
	SFPIADD	L1, L1, -1, 5
	TTNOP
	TTNOP
	ret
	.size	_Z7muladdii, .-_Z7muladdii
	.align	1
	.globl	_Z10muladdi_lvi
	.type	_Z10muladdi_lvi, @function
_Z10muladdi_lvi:
	SFPLOADI	L0, 1, 4
	SFPLOADI	L1, 1, 4
	SFPIADD	L2, L0, -2, 5
	TTNOP
	TTNOP
	SFPSETCC	L2, 0, 6
	TTNOP
	lui	a5,%hi(_ZN7ckernel13instrn_bufferE)
	lw	a4,%lo(_ZN7ckernel13instrn_bufferE)(a5)
	slli	a5,a0,16
	srli	a5,a5,16
	li	a3, 1898184704	# 71240000
	add	a3,a5,a3
	sw	a3, 0(a4)	# Op(0x71) - d(lr2)
	SFPIADD	L2, L0, 0, 4
	TTNOP
	TTNOP
	SFPMOV	L1, L2, 0
	TTNOP
	TTNOP
	SFPIADD	L0, L1, 1, 5
	TTNOP
	TTNOP
	mv	a5,a3
	sw	a3, 0(a4)	# Op(0x71) - d(lr2)
	SFPIADD	L2, L0, 0, 6
	TTNOP
	TTNOP
	SFPMOV	L1, L2, 0
	TTNOP
	TTNOP
	SFPIADD	L0, L1, -1, 5
	TTNOP
	TTNOP
	SFPENCC	3, 10
	TTNOP
	SFPIADD	L2, L0, -2, 5
	TTNOP
	TTNOP
	SFPSETCC	L2, 0, 6
	TTNOP
	sw	a3, 0(a4)	# Op(0x71) - d(lr2)
	SFPIADD	L2, L0, 0, 4
	TTNOP
	TTNOP
	SFPMOV	L1, L2, 0
	TTNOP
	TTNOP
	SFPIADD	L0, L1, 1, 5
	TTNOP
	TTNOP
	sw	a3, 0(a4)	# Op(0x71) - d(lr2)
	SFPIADD	L2, L0, 0, 6
	TTNOP
	TTNOP
	SFPMOV	L1, L2, 0
	TTNOP
	TTNOP
	SFPIADD	L0, L1, -1, 5
	TTNOP
	TTNOP
	SFPENCC	3, 10
	TTNOP
	SFPIADD	L2, L0, -2, 5
	TTNOP
	TTNOP
	SFPSETCC	L2, 0, 6
	TTNOP
	sw	a3, 0(a4)	# Op(0x71) - d(lr2)
	SFPIADD	L2, L0, 0, 4
	TTNOP
	TTNOP
	SFPMOV	L1, L2, 0
	TTNOP
	TTNOP
	SFPIADD	L0, L1, 1, 5
	TTNOP
	TTNOP
	sw	a3, 0(a4)	# Op(0x71) - d(lr2)
	SFPIADD	L2, L0, 0, 6
	TTNOP
	TTNOP
	SFPMOV	L1, L2, 0
	TTNOP
	TTNOP
	SFPIADD	L0, L1, -1, 5
	TTNOP
	TTNOP
	SFPENCC	3, 10
	TTNOP
	SFPIADD	L2, L0, -2, 5
	TTNOP
	TTNOP
	SFPSETCC	L2, 0, 6
	TTNOP
	sw	a3, 0(a4)	# Op(0x71) - d(lr2)
	SFPIADD	L2, L0, 0, 4
	TTNOP
	TTNOP
	SFPMOV	L1, L2, 0
	TTNOP
	TTNOP
	SFPIADD	L0, L1, 1, 5
	TTNOP
	TTNOP
	sw	a5, 0(a4)	# Op(0x71) - d(lr2)
	SFPIADD	L2, L0, 0, 6
	TTNOP
	TTNOP
	SFPMOV	L1, L2, 0
	TTNOP
	TTNOP
	SFPIADD	L0, L1, -1, 5
	TTNOP
	TTNOP
	SFPENCC	3, 10
	TTNOP
	ret
	.size	_Z10muladdi_lvi, .-_Z10muladdi_lvi
	.align	1
	.globl	_Z5otherif
	.type	_Z5otherif, @function
_Z5otherif:
	lui	a5,%hi(_ZN7ckernel13instrn_bufferE)
	li	a4,65536
	slli	a3,a0,2
	addi	a4,a4,-1
	lw	a5,%lo(_ZN7ckernel13instrn_bufferE)(a5)
	and	a3,a3,a4
	li	t3, 1879113728	# 70010000
	add	t3,a3,t3
	sw	t3, 0(a5)	# Op(0x70) - d(lr0)
	mv	t1,t3
	sw	t3, 0(a5)	# Op(0x70) - d(lr0)
	slli	a4,a0,12
	li	a2,16773120
	and	a4,a4,a2
	li	a7, 2298478609	# 89000011
	add	a7,a4,a7
	sw	a7, 0(a5)	# Op(0x89) - d(lr1) s(lr0)
	TTNOP
	TTNOP
	li	a6, 2197815297	# 83000001
	add	a6,a4,a6
	sw	a6, 0(a5)	# Op(0x83) - d(lr0) s(lr0)
	TTNOP
	TTNOP
	li	a0, 2181038097	# 82000011
	add	a0,a4,a0
	sw	a0, 0(a5)	# Op(0x82) - d(lr1) s(lr0)
	TTNOP
	TTNOP
	li	a2, 1979711777	# 76000121
	add	a4,a4,a2
	sw	a4, 0(a5)	# Op(0x76) - d(lr2) s(lr1)
	TTNOP
	TTNOP
	li	a1, 1914699776	# 72200000
	add	a1,a3,a1
	sw	a1, 0(a5)	# Op(0x72) s(lr2)
	li	a2, 1913651200	# 72100000
	add	a2,a3,a2
	sw	a2, 0(a5)	# Op(0x72) s(lr1)
	li	t4, 1912602624	# 72000000
	add	a3,a3,t4
	sw	a3, 0(a5)	# Op(0x72) s(lr0)
	sw	t3, 0(a5)	# Op(0x70) - d(lr0)
	sw	t3, 0(a5)	# Op(0x70) - d(lr0)
	sw	a7, 0(a5)	# Op(0x89) - d(lr1) s(lr0)
	TTNOP
	TTNOP
	sw	a6, 0(a5)	# Op(0x83) - d(lr0) s(lr0)
	TTNOP
	TTNOP
	sw	a0, 0(a5)	# Op(0x82) - d(lr1) s(lr0)
	TTNOP
	TTNOP
	sw	a4, 0(a5)	# Op(0x76) - d(lr2) s(lr1)
	TTNOP
	TTNOP
	sw	a1, 0(a5)	# Op(0x72) s(lr2)
	sw	a2, 0(a5)	# Op(0x72) s(lr1)
	sw	a3, 0(a5)	# Op(0x72) s(lr0)
	sw	t3, 0(a5)	# Op(0x70) - d(lr0)
	sw	t3, 0(a5)	# Op(0x70) - d(lr0)
	sw	a7, 0(a5)	# Op(0x89) - d(lr1) s(lr0)
	TTNOP
	TTNOP
	sw	a6, 0(a5)	# Op(0x83) - d(lr0) s(lr0)
	TTNOP
	TTNOP
	sw	a0, 0(a5)	# Op(0x82) - d(lr1) s(lr0)
	TTNOP
	TTNOP
	sw	a4, 0(a5)	# Op(0x76) - d(lr2) s(lr1)
	TTNOP
	TTNOP
	sw	a1, 0(a5)	# Op(0x72) s(lr2)
	sw	a2, 0(a5)	# Op(0x72) s(lr1)
	sw	a3, 0(a5)	# Op(0x72) s(lr0)
	sw	t3, 0(a5)	# Op(0x70) - d(lr0)
	sw	t1, 0(a5)	# Op(0x70) - d(lr0)
	sw	a7, 0(a5)	# Op(0x89) - d(lr1) s(lr0)
	TTNOP
	TTNOP
	sw	a6, 0(a5)	# Op(0x83) - d(lr0) s(lr0)
	TTNOP
	TTNOP
	sw	a0, 0(a5)	# Op(0x82) - d(lr1) s(lr0)
	TTNOP
	TTNOP
	sw	a4, 0(a5)	# Op(0x76) - d(lr2) s(lr1)
	TTNOP
	TTNOP
	sw	a1, 0(a5)	# Op(0x72) s(lr2)
	sw	a2, 0(a5)	# Op(0x72) s(lr1)
	sw	a3, 0(a5)	# Op(0x72) s(lr0)
	ret
	.size	_Z5otherif, .-_Z5otherif
	.align	1
	.globl	_Z8other_lvif
	.type	_Z8other_lvif, @function
_Z8other_lvif:
	SFPLOADI	L1, 16256, 0
	SFPMAD	L0, L10, L11, L1, 0
	TTNOP
	TTNOP
	SFPSETCC	L0, 0, 6
	TTNOP
	lui	a5,%hi(_ZN7ckernel13instrn_bufferE)
	li	a4,65536
	slli	a3,a0,2
	addi	a4,a4,-1
	lw	a5,%lo(_ZN7ckernel13instrn_bufferE)(a5)
	and	a3,a3,a4
	li	a6, 1879113728	# 70010000
	add	a6,a3,a6
	sw	a6, 0(a5)	# Op(0x70) - d(lr0)
	li	a1, 1881210880	# 70210000
	add	a1,a3,a1
	sw	a1, 0(a5)	# Op(0x70) - d(lr2)
	slli	a4,a0,12
	li	a0,16773120
	and	a4,a4,a0
	li	a2, 1979712001	# 76000201
	add	a4,a4,a2
	sw	a4, 0(a5)	# Op(0x76) - d(lr0) s(lr2)
	TTNOP
	TTNOP
	li	a2, 1912602624	# 72000000
	add	a2,a3,a2
	sw	a2, 0(a5)	# Op(0x72) s(lr0)
	li	a0, 1914699776	# 72200000
	add	a3,a3,a0
	sw	a3, 0(a5)	# Op(0x72) s(lr2)
	SFPENCC	3, 10
	TTNOP
	SFPMAD	L3, L10, L11, L1, 0
	TTNOP
	TTNOP
	SFPSETCC	L3, 0, 6
	TTNOP
	sw	a6, 0(a5)	# Op(0x70) lv(lr0)  d(lr0)
	sw	a1, 0(a5)	# Op(0x70) lv(lr2)  d(lr2)
	sw	a4, 0(a5)	# Op(0x76) lv(lr0)  d(lr0) s(lr2)
	TTNOP
	TTNOP
	sw	a2, 0(a5)	# Op(0x72) s(lr0)
	sw	a3, 0(a5)	# Op(0x72) s(lr2)
	SFPENCC	3, 10
	TTNOP
	SFPMAD	L3, L10, L11, L1, 0
	TTNOP
	TTNOP
	SFPSETCC	L3, 0, 6
	TTNOP
	sw	a6, 0(a5)	# Op(0x70) lv(lr0)  d(lr0)
	sw	a1, 0(a5)	# Op(0x70) lv(lr2)  d(lr2)
	sw	a4, 0(a5)	# Op(0x76) lv(lr0)  d(lr0) s(lr2)
	TTNOP
	TTNOP
	sw	a2, 0(a5)	# Op(0x72) s(lr0)
	sw	a3, 0(a5)	# Op(0x72) s(lr2)
	SFPENCC	3, 10
	TTNOP
	SFPMAD	L1, L10, L11, L1, 0
	TTNOP
	TTNOP
	SFPSETCC	L1, 0, 6
	TTNOP
	sw	a6, 0(a5)	# Op(0x70) lv(lr0)  d(lr0)
	sw	a1, 0(a5)	# Op(0x70) lv(lr2)  d(lr2)
	sw	a4, 0(a5)	# Op(0x76) lv(lr0)  d(lr0) s(lr2)
	TTNOP
	TTNOP
	sw	a2, 0(a5)	# Op(0x72) s(lr0)
	sw	a3, 0(a5)	# Op(0x72) s(lr2)
	SFPENCC	3, 10
	TTNOP
	ret
	.size	_Z8other_lvif, .-_Z8other_lvif
	.align	1
	.globl	_Z4shfti
	.type	_Z4shfti, @function
_Z4shfti:
	SFPLOADI	L0, 1, 4
	lui	a5,%hi(_ZN7ckernel13instrn_bufferE)
	lw	a4,%lo(_ZN7ckernel13instrn_bufferE)(a5)
	slli	a3,a0,12
	li	a2,16773120
	and	a3,a3,a2
	li	a5, 2046820353	# 7a000001
	add	a3,a3,a5
	sw	a3, 0(a4)	# Op(0x7a) lv(lr0)  d(lr0)
	TTNOP
	TTNOP
	neg	a5,a0
	slli	a5,a5,12
	and	a5,a5,a2
	li	a2, 2046820353	# 7a000001
	add	a5,a5,a2
	sw	a5, 0(a4)	# Op(0x7a) lv(lr0)  d(lr0)
	TTNOP
	TTNOP
	sw	a3, 0(a4)	# Op(0x7a) lv(lr0)  d(lr0)
	TTNOP
	TTNOP
	sw	a5, 0(a4)	# Op(0x7a) lv(lr0)  d(lr0)
	TTNOP
	TTNOP
	sw	a3, 0(a4)	# Op(0x7a) lv(lr0)  d(lr0)
	TTNOP
	TTNOP
	sw	a5, 0(a4)	# Op(0x7a) lv(lr0)  d(lr0)
	TTNOP
	TTNOP
	sw	a3, 0(a4)	# Op(0x7a) lv(lr0)  d(lr0)
	TTNOP
	TTNOP
	SFPMOV	L1, L0, 0
	TTNOP
	TTNOP
	li	a3, 16
	xor	a3, a3, a5
	sw	a3, 0(a4)	# Op(0x7a) lv(lr1)  d(lr1)
	TTNOP
	TTNOP
	SFPSTORE	0, L0, 2
	SFPSTORE	0, L1, 2
	ret
	.size	_Z4shfti, .-_Z4shfti
	.align	1
	.globl	_Z7shft_lvi
	.type	_Z7shft_lvi, @function
_Z7shft_lvi:
	SFPLOADI	L0, 1, 4
	SFPLOADI	L1, 1, 4
	SFPIADD	L2, L0, -1, 5
	TTNOP
	TTNOP
	SFPSETCC	L2, 0, 6
	TTNOP
	lui	a5,%hi(_ZN7ckernel13instrn_bufferE)
	lw	a4,%lo(_ZN7ckernel13instrn_bufferE)(a5)
	slli	a3,a0,12
	li	a2,16773120
	and	a3,a3,a2
	SFPMOV	L2, L1, 0
	TTNOP
	TTNOP
	li	a5, 2046820385	# 7a000021
	add	a3,a3,a5
	sw	a3, 0(a4)	# Op(0x7a) lv(lr2)  d(lr2)
	TTNOP
	TTNOP
	SFPMOV	L0, L2, 0
	TTNOP
	TTNOP
	neg	a5,a0
	slli	a5,a5,12
	and	a5,a5,a2
	SFPMOV	L2, L0, 0
	TTNOP
	TTNOP
	li	a2, 2046820385	# 7a000021
	add	a5,a5,a2
	sw	a5, 0(a4)	# Op(0x7a) lv(lr2)  d(lr2)
	TTNOP
	TTNOP
	SFPMOV	L1, L2, 0
	TTNOP
	TTNOP
	SFPENCC	3, 10
	TTNOP
	SFPIADD	L2, L0, -1, 5
	TTNOP
	TTNOP
	SFPSETCC	L2, 0, 6
	TTNOP
	SFPMOV	L2, L1, 0
	TTNOP
	TTNOP
	sw	a3, 0(a4)	# Op(0x7a) lv(lr2)  d(lr2)
	TTNOP
	TTNOP
	SFPMOV	L0, L2, 0
	TTNOP
	TTNOP
	SFPMOV	L2, L0, 0
	TTNOP
	TTNOP
	sw	a5, 0(a4)	# Op(0x7a) lv(lr2)  d(lr2)
	TTNOP
	TTNOP
	SFPMOV	L1, L2, 0
	TTNOP
	TTNOP
	SFPENCC	3, 10
	TTNOP
	SFPIADD	L2, L0, -1, 5
	TTNOP
	TTNOP
	SFPSETCC	L2, 0, 6
	TTNOP
	SFPMOV	L2, L1, 0
	TTNOP
	TTNOP
	sw	a3, 0(a4)	# Op(0x7a) lv(lr2)  d(lr2)
	TTNOP
	TTNOP
	SFPMOV	L0, L2, 0
	TTNOP
	TTNOP
	SFPMOV	L2, L0, 0
	TTNOP
	TTNOP
	sw	a5, 0(a4)	# Op(0x7a) lv(lr2)  d(lr2)
	TTNOP
	TTNOP
	SFPMOV	L1, L2, 0
	TTNOP
	TTNOP
	SFPENCC	3, 10
	TTNOP
	SFPIADD	L2, L0, -1, 5
	TTNOP
	TTNOP
	SFPSETCC	L2, 0, 6
	TTNOP
	SFPMOV	L2, L1, 0
	TTNOP
	TTNOP
	sw	a3, 0(a4)	# Op(0x7a) lv(lr2)  d(lr2)
	TTNOP
	TTNOP
	SFPMOV	L0, L2, 0
	TTNOP
	TTNOP
	SFPMOV	L2, L0, 0
	TTNOP
	TTNOP
	sw	a5, 0(a4)	# Op(0x7a) lv(lr2)  d(lr2)
	TTNOP
	TTNOP
	SFPMOV	L1, L2, 0
	TTNOP
	TTNOP
	SFPENCC	3, 10
	TTNOP
	SFPSTORE	0, L0, 2
	SFPSTORE	0, L1, 2
	ret
	.size	_Z7shft_lvi, .-_Z7shft_lvi
	.align	1
	.globl	_Z4mulii
	.type	_Z4mulii, @function
_Z4mulii:
	slli	a0,a0,16
	srli	a0,a0,16
	lui	a5,%hi(_ZN7ckernel13instrn_bufferE)
	lw	a4,%lo(_ZN7ckernel13instrn_bufferE)(a5)
	li	a3, 1946157056	# 74000000
	slli	a5,a0,8
	add	a5,a5,a3
	li	a3, 1896873984	# 71100000
	add	a0,a0,a3
	sw	a0, 0(a4)	# Op(0x71) - d(lr1)
	SFPLOADI	L0, 16256, 0
	sw	a5, 0(a4)	# Op(0x74) lv(lr0)  d(lr0)
	TTNOP
	TTNOP
	sw	a5, 0(a4)	# Op(0x74) lv(lr0)  d(lr0)
	TTNOP
	TTNOP
	sw	a5, 0(a4)	# Op(0x74) lv(lr0)  d(lr0)
	TTNOP
	TTNOP
	sw	a5, 0(a4)	# Op(0x74) lv(lr0)  d(lr0)
	TTNOP
	TTNOP
	SFPSTORE	0, L0, 0
	SFPSTORE	0, L1, 0
	ret
	.size	_Z4mulii, .-_Z4mulii
	.align	1
	.globl	_Z13muli_nounrolli
	.type	_Z13muli_nounrolli, @function
_Z13muli_nounrolli:
	slli	a4,a0,16
	srli	a4,a4,16
	li	a0, 1895825408	# 71000000
	li	a5,4
	lui	a3,%hi(_ZN7ckernel13instrn_bufferE)
	add	a0,a0,a4
.L22:
	lw	a4,%lo(_ZN7ckernel13instrn_bufferE)(a3)
	sw	a0, 0(a4)	# Op(0x71) - d(lr0)
	SFPSTORE	0, L0, 0
	addi	a5,a5,-1
	bne	a5,zero,.L22
	ret
	.size	_Z13muli_nounrolli, .-_Z13muli_nounrolli
	.globl	_ZN7ckernel13instrn_bufferE
	.section	.sbss,"aw",@nobits
	.align	2
	.type	_ZN7ckernel13instrn_bufferE, @object
	.size	_ZN7ckernel13instrn_bufferE, 4
_ZN7ckernel13instrn_bufferE:
	.zero	4
	.ident	"GCC: (GNU) 10.2.0"
