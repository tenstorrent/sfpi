	.file	"nonimm.cc"
	.option nopic
	.attribute arch, "rv32i2p0_m2p0_w2p0"
	.attribute unaligned_access, 0
	.attribute stack_align, 16
	.text
	.align	1
	.globl	_Z6loadiffi
	.type	_Z6loadiffi, @function
_Z6loadiffi:
	li	a7,65536
	lui	a5,%hi(_ZN7ckernel13instrn_bufferE)
	addi	a7,a7,-1
	li	a2, 1897398272	# 71180000
	and	a3,a7,a0
	li	a6, 1897529344	# 711a0000
	srli	a0,a0,16
	lw	a5,%lo(_ZN7ckernel13instrn_bufferE)(a5)
	add	a2,a0,a2
	add	a6,a3,a6
	TTREPLAY	0, 6, 1, 1
	sw	a6, 0(a5)	# Op(0x71) - d(lr1)
	sw	a2, 0(a5)	# Op(0x71) lv(lr1)  d(lr1)
	li	a4, 1896480768	# 710a0000
	add	a3,a3,a4
	li	a4, 1896349696	# 71080000
	add	a4,a0,a4
	sw	a3, 0(a5)	# Op(0x71) - d(lr0)
	sw	a4, 0(a5)	# Op(0x71) lv(lr0)  d(lr0)
	SFPSTORE	0, L1, 0, 3
	SFPSTORE	2, L0, 0, 3
	TTREPLAY	0, 6, 0, 0
	TTREPLAY	0, 6, 0, 0
	TTREPLAY	0, 6, 0, 0
	li	a4, 1895825408	# 71000000
	and	a1,a1,a7
	add	a4,a1,a4
	sw	a4, 0(a5)	# Op(0x71) - d(lr0)
	SFPSTORE	0, L0, 0, 3
	sw	a4, 0(a5)	# Op(0x71) - d(lr0)
	SFPSTORE	0, L0, 0, 3
	sw	a4, 0(a5)	# Op(0x71) - d(lr0)
	SFPSTORE	0, L0, 0, 3
	sw	a4, 0(a5)	# Op(0x71) - d(lr0)
	SFPSTORE	0, L0, 0, 3
	li	a4, 1895890944	# 71010000
	add	a4,a4,a1
	sw	a4, 0(a5)	# Op(0x71) - d(lr0)
	SFPSTORE	0, L0, 0, 3
	sw	a4, 0(a5)	# Op(0x71) - d(lr0)
	SFPSTORE	0, L0, 0, 3
	sw	a4, 0(a5)	# Op(0x71) - d(lr0)
	SFPSTORE	0, L0, 0, 3
	sw	a4, 0(a5)	# Op(0x71) - d(lr0)
	SFPSTORE	0, L0, 0, 3
	li	a4, 1895825408	# 71000000
	add	a1,a4,a1
	sw	a1, 0(a5)	# Op(0x71) - d(lr0)
	SFPSTORE	0, L0, 0, 3
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
	SFPNOP
	SFPSETCC	L2, 0, 6
	li	a7,65536
	lui	a5,%hi(_ZN7ckernel13instrn_bufferE)
	addi	a7,a7,-1
	li	a2, 1896349696	# 71080000
	and	a3,a7,a0
	li	a6, 1896480768	# 710a0000
	srli	a0,a0,16
	lw	a5,%lo(_ZN7ckernel13instrn_bufferE)(a5)
	add	a2,a0,a2
	add	a6,a3,a6
	sw	a6, 0(a5)	# Op(0x71) lv(lr0)  d(lr0)
	sw	a2, 0(a5)	# Op(0x71) lv(lr0)  d(lr0)
	li	a4, 1897529344	# 711a0000
	add	a3,a3,a4
	li	a4, 1897398272	# 71180000
	add	a4,a0,a4
	sw	a3, 0(a5)	# Op(0x71) lv(lr1)  d(lr1)
	sw	a4, 0(a5)	# Op(0x71) lv(lr1)  d(lr1)
	SFPENCC	3, 10
	SFPSTORE	0, L0, 0, 3
	SFPSTORE	2, L1, 0, 3
	SFPLOADI	L0, 16256, 0
	SFPLOADI	L1, 16256, 0
	SFPLOADI	L2, 16384, 0
	SFPMAD	L2, L2, L11, L0, 0
	SFPNOP
	SFPSETCC	L2, 0, 6
	sw	a6, 0(a5)	# Op(0x71) lv(lr0)  d(lr0)
	sw	a2, 0(a5)	# Op(0x71) lv(lr0)  d(lr0)
	sw	a3, 0(a5)	# Op(0x71) lv(lr1)  d(lr1)
	sw	a4, 0(a5)	# Op(0x71) lv(lr1)  d(lr1)
	SFPENCC	3, 10
	SFPSTORE	0, L0, 0, 3
	SFPSTORE	2, L1, 0, 3
	SFPLOADI	L0, 16256, 0
	SFPLOADI	L1, 16256, 0
	SFPLOADI	L2, 16384, 0
	SFPMAD	L2, L2, L11, L0, 0
	SFPNOP
	SFPSETCC	L2, 0, 6
	sw	a6, 0(a5)	# Op(0x71) lv(lr0)  d(lr0)
	sw	a2, 0(a5)	# Op(0x71) lv(lr0)  d(lr0)
	sw	a3, 0(a5)	# Op(0x71) lv(lr1)  d(lr1)
	sw	a4, 0(a5)	# Op(0x71) lv(lr1)  d(lr1)
	SFPENCC	3, 10
	SFPSTORE	0, L0, 0, 3
	SFPSTORE	2, L1, 0, 3
	SFPLOADI	L0, 16256, 0
	SFPLOADI	L1, 16256, 0
	SFPLOADI	L2, 16384, 0
	SFPMAD	L2, L2, L11, L0, 0
	SFPNOP
	SFPSETCC	L2, 0, 6
	sw	a6, 0(a5)	# Op(0x71) lv(lr0)  d(lr0)
	sw	a2, 0(a5)	# Op(0x71) lv(lr0)  d(lr0)
	sw	a3, 0(a5)	# Op(0x71) lv(lr1)  d(lr1)
	sw	a4, 0(a5)	# Op(0x71) lv(lr1)  d(lr1)
	SFPENCC	3, 10
	SFPSTORE	0, L0, 0, 3
	SFPSTORE	2, L1, 0, 3
	SFPLOADI	L0, 16256, 0
	SFPLOADI	L1, 16384, 0
	SFPMAD	L1, L1, L11, L0, 0
	SFPNOP
	SFPSETCC	L1, 0, 6
	li	a4, 1895825408	# 71000000
	and	a1,a1,a7
	add	a4,a1,a4
	sw	a4, 0(a5)	# Op(0x71) lv(lr0)  d(lr0)
	TTREPLAY	0, 7, 1, 1
	SFPENCC	3, 10
	SFPSTORE	0, L0, 0, 3
	SFPLOADI	L0, 16256, 0
	SFPLOADI	L1, 16384, 0
	SFPMAD	L1, L1, L11, L0, 0
	SFPNOP
	SFPSETCC	L1, 0, 6
	sw	a4, 0(a5)	# Op(0x71) lv(lr0)  d(lr0)
	TTREPLAY	0, 7, 0, 0
	sw	a4, 0(a5)	# Op(0x71) lv(lr0)  d(lr0)
	TTREPLAY	0, 7, 0, 0
	sw	a4, 0(a5)	# Op(0x71) lv(lr0)  d(lr0)
	TTREPLAY	0, 7, 0, 0
	li	a4, 1895890944	# 71010000
	add	a4,a4,a1
	sw	a4, 0(a5)	# Op(0x71) lv(lr0)  d(lr0)
	TTREPLAY	0, 7, 0, 0
	sw	a4, 0(a5)	# Op(0x71) lv(lr0)  d(lr0)
	TTREPLAY	0, 7, 0, 0
	sw	a4, 0(a5)	# Op(0x71) lv(lr0)  d(lr0)
	TTREPLAY	0, 7, 0, 0
	sw	a4, 0(a5)	# Op(0x71) lv(lr0)  d(lr0)
	TTREPLAY	0, 7, 0, 0
	li	a4, 1895825408	# 71000000
	add	a1,a4,a1
	sw	a1, 0(a5)	# Op(0x71) lv(lr0)  d(lr0)
	SFPENCC	3, 10
	SFPLOADI	L1, 16384, 0
	SFPMAD	L1, L1, L11, L0, 0
	SFPNOP
	SFPSETCC	L1, 0, 6
	sw	a1, 0(a5)	# Op(0x71) lv(lr0)  d(lr0)
	SFPENCC	3, 10
	SFPLOADI	L1, 16384, 0
	SFPMAD	L1, L1, L11, L0, 0
	SFPNOP
	SFPSETCC	L1, 0, 6
	sw	a1, 0(a5)	# Op(0x71) lv(lr0)  d(lr0)
	SFPENCC	3, 10
	SFPLOADI	L1, 16384, 0
	SFPMAD	L1, L1, L11, L0, 0
	SFPNOP
	SFPSETCC	L1, 0, 6
	sw	a1, 0(a5)	# Op(0x71) lv(lr0)  d(lr0)
	TTREPLAY	0, 2, 0, 0
	ret
	.size	_Z9loadif_lvfi, .-_Z9loadif_lvfi
	.align	1
	.globl	_Z6loadiii
	.type	_Z6loadiii, @function
_Z6loadiii:
	slli	a6,a0,16
	lui	a5,%hi(_ZN7ckernel13instrn_bufferE)
	srli	a6,a6,16
	srli	a4,a0,16
	li	a2, 1896349696	# 71080000
	li	a1, 1896480768	# 710a0000
	lw	a5,%lo(_ZN7ckernel13instrn_bufferE)(a5)
	add	a2,a4,a2
	add	a1,a6,a1
	TTREPLAY	0, 3, 1, 1
	sw	a1, 0(a5)	# Op(0x71) - d(lr0)
	sw	a2, 0(a5)	# Op(0x71) lv(lr0)  d(lr0)
	SFPSTORE	0, L0, 0, 3
	TTREPLAY	0, 3, 0, 0
	TTREPLAY	0, 3, 0, 0
	TTREPLAY	0, 3, 0, 0
	li	a2, 1896349696	# 71080000
	add	a4,a4,a2
	sw	a1, 0(a5)	# Op(0x71) - d(lr0)
	sw	a4, 0(a5)	# Op(0x71) lv(lr0)  d(lr0)
	SFPSTORE	0, L0, 0, 3
	sw	a1, 0(a5)	# Op(0x71) - d(lr0)
	sw	a4, 0(a5)	# Op(0x71) lv(lr0)  d(lr0)
	SFPSTORE	0, L0, 0, 3
	sw	a1, 0(a5)	# Op(0x71) - d(lr0)
	sw	a4, 0(a5)	# Op(0x71) lv(lr0)  d(lr0)
	SFPSTORE	0, L0, 0, 3
	sw	a1, 0(a5)	# Op(0x71) - d(lr0)
	sw	a4, 0(a5)	# Op(0x71) lv(lr0)  d(lr0)
	SFPSTORE	0, L0, 0, 3
	ret
	.size	_Z6loadiii, .-_Z6loadiii
	.align	1
	.globl	_Z9loadii_lvi
	.type	_Z9loadii_lvi, @function
_Z9loadii_lvi:
	SFPLOADI	L0, 1, 4
	SFPIADD	L1, L0, -2, 5
	SFPSETCC	L1, 0, 6
	lui	a5,%hi(_ZN7ckernel13instrn_bufferE)
	slli	a6,a0,16
	srli	a4,a0,16
	srli	a6,a6,16
	li	a2, 1896349696	# 71080000
	li	a1, 1896480768	# 710a0000
	lw	a5,%lo(_ZN7ckernel13instrn_bufferE)(a5)
	add	a2,a4,a2
	add	a1,a6,a1
	sw	a1, 0(a5)	# Op(0x71) lv(lr0)  d(lr0)
	sw	a2, 0(a5)	# Op(0x71) lv(lr0)  d(lr0)
	TTREPLAY	0, 6, 1, 1
	SFPENCC	3, 10
	SFPSTORE	0, L0, 0, 3
	SFPLOADI	L0, 1, 4
	SFPIADD	L1, L0, -2, 5
	SFPSETCC	L1, 0, 6
	sw	a1, 0(a5)	# Op(0x71) lv(lr0)  d(lr0)
	sw	a2, 0(a5)	# Op(0x71) lv(lr0)  d(lr0)
	TTREPLAY	0, 6, 0, 0
	sw	a2, 0(a5)	# Op(0x71) lv(lr0)  d(lr0)
	TTREPLAY	0, 6, 0, 0
	sw	a2, 0(a5)	# Op(0x71) lv(lr0)  d(lr0)
	add	a4,a4,a2
	TTREPLAY	0, 6, 0, 0
	sw	a1, 0(a5)	# Op(0x71) lv(lr0)  d(lr0)
	sw	a4, 0(a5)	# Op(0x71) lv(lr0)  d(lr0)
	TTREPLAY	0, 6, 0, 0
	sw	a4, 0(a5)	# Op(0x71) lv(lr0)  d(lr0)
	TTREPLAY	0, 6, 0, 0
	sw	a4, 0(a5)	# Op(0x71) lv(lr0)  d(lr0)
	TTREPLAY	0, 6, 0, 0
	sw	a4, 0(a5)	# Op(0x71) lv(lr0)  d(lr0)
	TTREPLAY	0, 2, 0, 0
	ret
	.size	_Z9loadii_lvi, .-_Z9loadii_lvi
	.align	1
	.globl	_Z9loadstorei
	.type	_Z9loadstorei, @function
_Z9loadstorei:
	slli	a4,a0,1
	li	a0,16384
	lui	a5,%hi(_ZN7ckernel13instrn_bufferE)
	addi	a0,a0,-1
	li	a1, 1879097344	# 7000c000
	and	a4,a4,a0
	lw	a5,%lo(_ZN7ckernel13instrn_bufferE)(a5)
	add	a1,a4,a1
	TTREPLAY	0, 6, 1, 1
	sw	a1, 0(a5)	# Op(0x70) - d(lr0)
	li	a2, 1912651776	# 7200c000
	add	a2,a4,a2
	sw	a2, 0(a5)	# Op(0x72) s(lr0)
	SFPLOADI	L0, 1, 4
	mv	a3,a2
	sw	a2, 0(a5)	# Op(0x72) s(lr0)
	SFPLOADI	L0, 2, 4
	mv	a4,a2
	sw	a2, 0(a5)	# Op(0x72) s(lr0)
	TTREPLAY	0, 6, 0, 0
	TTREPLAY	0, 6, 0, 0
	TTREPLAY	0, 6, 0, 0
	ret
	.size	_Z9loadstorei, .-_Z9loadstorei
	.align	1
	.globl	_Z12loadstore_lvi
	.type	_Z12loadstore_lvi, @function
_Z12loadstore_lvi:
	TTREPLAY	0, 11, 1, 1
	SFPLOADI	L1, 16256, 0
	SFPLOADI	L0, 16384, 0
	SFPMAD	L0, L0, L11, L1, 0
	SFPNOP
	SFPSETCC	L0, 0, 6
	slli	a5,a0,1
	SFPENCC	3, 10
	li	a4,16384
	li	a2, 1913700352	# 7210c000
	addi	a4,a4,-1
	and	a5,a5,a4
	lui	a4,%hi(_ZN7ckernel13instrn_bufferE)
	add	a2,a5,a2
	lw	a4,%lo(_ZN7ckernel13instrn_bufferE)(a4)
	sw	a2, 0(a4)	# Op(0x72) s(lr1)
	SFPLOADI	L0, 1, 4
	li	a3, 1912651776	# 7200c000
	add	a3,a5,a3
	sw	a3, 0(a4)	# Op(0x72) s(lr0)
	SFPLOADI	L0, 2, 4
	mv	a5,a3
	sw	a3, 0(a4)	# Op(0x72) s(lr0)
	TTREPLAY	0, 11, 0, 0
	TTREPLAY	0, 11, 0, 0
	TTREPLAY	0, 11, 0, 0
	ret
	.size	_Z12loadstore_lvi, .-_Z12loadstore_lvi
	.align	1
	.globl	_Z17loadstorefallbacki
	.type	_Z17loadstorefallbacki, @function
_Z17loadstorefallbacki:
	slli	a5,a0,1
	li	a0,16384
	lui	a4,%hi(_ZN7ckernel13instrn_bufferE)
	addi	a0,a0,-1
	li	a3, 1879097344	# 7000c000
	and	a5,a5,a0
	lw	a4,%lo(_ZN7ckernel13instrn_bufferE)(a4)
	add	a3,a5,a3
	TTREPLAY	0, 2, 1, 1
	sw	a3, 0(a4)	# Op(0x70) - d(lr0)
	mv	a5,a3
	sw	a3, 0(a4)	# Op(0x70) - d(lr0)
	TTREPLAY	0, 2, 0, 0
	TTREPLAY	0, 2, 0, 0
	li	a2, 1048576
	xor	a2, a2, a3
	sw	a2, 0(a4)	# Op(0x70) - d(lr1)
	sw	a5, 0(a4)	# Op(0x70) - d(lr0)
	SFPSTORE	0, L1, 0, 3
	SFPSTORE	0, L0, 0, 3
	ret
	.size	_Z17loadstorefallbacki, .-_Z17loadstorefallbacki
	.align	1
	.globl	_Z20loadstorefallback_lvi
	.type	_Z20loadstorefallback_lvi, @function
_Z20loadstorefallback_lvi:
	SFPLOADI	L0, 16256, 0
	SFPLOADI	L1, 16256, 0
	TTREPLAY	0, 6, 1, 1
	SFPMAD	L2, L10, L11, L0, 0
	SFPNOP
	SFPSETCC	L2, 0, 6
	li	a4,16384
	slli	a5,a0,1
	addi	a4,a4,-1
	li	a3, 1879097344	# 7000c000
	and	a5,a5,a4
	lui	a4,%hi(_ZN7ckernel13instrn_bufferE)
	add	a3,a5,a3
	lw	a4,%lo(_ZN7ckernel13instrn_bufferE)(a4)
	sw	a3, 0(a4)	# Op(0x70) lv(lr0)  d(lr0)
	li	a2, 1880145920	# 7010c000
	add	a5,a5,a2
	sw	a5, 0(a4)	# Op(0x70) lv(lr1)  d(lr1)
	SFPENCC	3, 10
	TTREPLAY	0, 6, 0, 0
	TTREPLAY	0, 6, 0, 0
	TTREPLAY	0, 6, 0, 0
	SFPSTORE	0, L0, 0, 3
	SFPSTORE	0, L1, 0, 3
	ret
	.size	_Z20loadstorefallback_lvi, .-_Z20loadstorefallback_lvi
	.align	1
	.globl	_Z5fcmpsfi
	.type	_Z5fcmpsfi, @function
_Z5fcmpsfi:
	SFPLOADI	L0, 16256, 0
	li	a4,65536
	li	a3, 1897529344	# 711a0000
	addi	a4,a4,-1
	lui	a5,%hi(_ZN7ckernel13instrn_bufferE)
	and	a6,a4,a0
	srli	a0,a0,16
	add	a6,a6,a3
	li	a3, 1897398272	# 71180000
	lw	a5,%lo(_ZN7ckernel13instrn_bufferE)(a5)
	add	a3,a0,a3
	TTREPLAY	0, 25, 1, 1
	sw	a6, 0(a5)	# Op(0x71) - d(lr1)
	sw	a3, 0(a5)	# Op(0x71) lv(lr1)  d(lr1)
	SFPMAD	L1, L1, L11, L0, 0
	SFPNOP
	SFPSETCC	L1, 0, 6
	SFPSTORE	0, L0, 0, 3
	SFPENCC	3, 10
	li	a2, 1896873984	# 71100000
	and	a4,a1,a4
	add	a2,a4,a2
	sw	a2, 0(a5)	# Op(0x71) - d(lr1)
	SFPMAD	L1, L1, L11, L0, 0
	SFPNOP
	SFPSETCC	L1, 0, 6
	SFPSTORE	0, L0, 0, 3
	SFPENCC	3, 10
	li	a1, 1896939520	# 71110000
	add	a4,a4,a1
	sw	a4, 0(a5)	# Op(0x71) - d(lr1)
	SFPMAD	L1, L1, L11, L0, 0
	SFPNOP
	SFPSETCC	L1, 0, 6
	SFPSTORE	0, L0, 0, 3
	SFPENCC	3, 10
	SFPLOADI	L1, 16448, 0
	SFPMAD	L1, L1, L11, L0, 0
	SFPNOP
	SFPSETCC	L1, 0, 6
	SFPSTORE	0, L0, 0, 3
	SFPENCC	3, 10
	TTREPLAY	0, 25, 0, 0
	TTREPLAY	0, 25, 0, 0
	TTREPLAY	0, 25, 0, 0
	ret
	.size	_Z5fcmpsfi, .-_Z5fcmpsfi
	.align	1
	.globl	_Z7muladdffi
	.type	_Z7muladdffi, @function
_Z7muladdffi:
	li	a3,65536
	lui	a5,%hi(_ZN7ckernel13instrn_bufferE)
	addi	a3,a3,-1
	li	a4, 1896873984	# 71100000
	and	a1,a1,a3
	lw	a5,%lo(_ZN7ckernel13instrn_bufferE)(a5)
	add	a4,a1,a4
	sw	a4, 0(a5)	# Op(0x71) - d(lr1)
	SFPMULI	L1, 16256, 0
	li	a7, 1895890944	# 71010000
	add	a7,a1,a7
	sw	a7, 0(a5)	# Op(0x71) - d(lr0)
	SFPMUL	L0, L1, L0, L9, 0
	li	a6, 1897529344	# 711a0000
	and	a3,a3,a0
	srli	a4,a0,16
	li	a0, 1897398272	# 71180000
	add	a6,a3,a6
	add	a0,a4,a0
	TTREPLAY	0, 15, 1, 1
	sw	a6, 0(a5)	# Op(0x71) - d(lr1)
	sw	a0, 0(a5)	# Op(0x71) lv(lr1)  d(lr1)
	SFPMUL	L1, L0, L1, L9, 0
	SFPNOP
	li	a2, 1962934288	# 75000010
	slli	t1,a1,8
	add	a2,t1,a2
	sw	a2, 0(a5)	# Op(0x75) lv(lr1)  d(lr1)
	mv	a1,a7
	sw	a7, 0(a5)	# Op(0x71) - d(lr0)
	SFPADD	L1, L10, L1, L0, 0
	li	t3, 1896480768	# 710a0000
	add	a3,a3,t3
	li	t3, 1896349696	# 71080000
	add	a4,a4,t3
	sw	a3, 0(a5)	# Op(0x71) - d(lr0)
	sw	a4, 0(a5)	# Op(0x71) lv(lr0)  d(lr0)
	SFPADD	L0, L10, L1, L0, 0
	SFPNOP
	SFPSTORE	0, L0, 0, 3
	SFPSTORE	2, L1, 0, 3
	li	t3, 1946157056	# 74000000
	add	t1,t1,t3
	sw	t1, 0(a5)	# Op(0x74) lv(lr0)  d(lr0)
	li	t3, 1048576
	xor	t3, t3, a7
	sw	t3, 0(a5)	# Op(0x71) - d(lr1)
	SFPMUL	L0, L0, L1, L9, 0
	TTREPLAY	0, 15, 0, 0
	SFPMUL	L0, L0, L1, L9, 0
	TTREPLAY	0, 15, 0, 0
	SFPMUL	L1, L0, L1, L9, 0
	li	a7, 1048576
	xor	a7, a7, a6
	sw	a7, 0(a5)	# Op(0x71) - d(lr0)
	li	a6, 1048576
	xor	a6, a6, a0
	sw	a6, 0(a5)	# Op(0x71) lv(lr0)  d(lr0)
	SFPMUL	L0, L1, L0, L9, 0
	SFPNOP
	li	a0, 16
	xor	a0, a0, a2
	sw	a0, 0(a5)	# Op(0x75) lv(lr0)  d(lr0)
	li	a2, 1048576
	xor	a2, a2, a1
	sw	a2, 0(a5)	# Op(0x71) - d(lr1)
	SFPADD	L0, L10, L0, L1, 0
	li	a2, 1048576
	xor	a2, a2, a3
	sw	a2, 0(a5)	# Op(0x71) - d(lr1)
	li	a3, 1048576
	xor	a3, a3, a4
	sw	a3, 0(a5)	# Op(0x71) lv(lr1)  d(lr1)
	SFPADD	L1, L10, L0, L1, 0
	SFPNOP
	SFPSTORE	0, L1, 0, 3
	SFPSTORE	2, L0, 0, 3
	ret
	.size	_Z7muladdffi, .-_Z7muladdffi
	.align	1
	.globl	_Z10muladdf_lvfi
	.type	_Z10muladdf_lvfi, @function
_Z10muladdf_lvfi:
	SFPLOADI	L1, 16256, 0
	SFPLOADI	L0, 16256, 0
	TTREPLAY	0, 21, 1, 1
	SFPMAD	L2, L10, L11, L0, 0
	SFPNOP
	SFPSETCC	L2, 0, 6
	li	a2,65536
	lui	a5,%hi(_ZN7ckernel13instrn_bufferE)
	addi	a2,a2,-1
	li	t3, 1897922560	# 71200000
	and	a4,a1,a2
	lw	a5,%lo(_ZN7ckernel13instrn_bufferE)(a5)
	add	t3,a4,t3
	sw	t3, 0(a5)	# Op(0x71) - d(lr2)
	SFPMUL	L1, L0, L2, L9, 0
	li	t1, 1897988096	# 71210000
	add	t1,a4,t1
	sw	t1, 0(a5)	# Op(0x71) - d(lr2)
	SFPMUL	L0, L1, L2, L9, 0
	li	a7, 1898577920	# 712a0000
	and	a2,a2,a0
	li	a6, 1898446848	# 71280000
	srli	a0,a0,16
	add	a7,a2,a7
	add	a6,a0,a6
	sw	a7, 0(a5)	# Op(0x71) - d(lr2)
	sw	a6, 0(a5)	# Op(0x71) lv(lr2)  d(lr2)
	SFPMUL	L1, L0, L2, L9, 0
	sw	t3, 0(a5)	# Op(0x71) - d(lr2)
	SFPADD	L0, L10, L1, L2, 0
	sw	t1, 0(a5)	# Op(0x71) - d(lr2)
	SFPADD	L1, L10, L0, L2, 0
	sw	a7, 0(a5)	# Op(0x71) - d(lr2)
	sw	a6, 0(a5)	# Op(0x71) lv(lr2)  d(lr2)
	SFPADD	L0, L10, L1, L2, 0
	SFPNOP
	SFPSTORE	0, L0, 0, 3
	SFPSTORE	2, L1, 0, 3
	SFPENCC	3, 10
	TTREPLAY	0, 21, 0, 0
	TTREPLAY	0, 21, 0, 0
	TTREPLAY	0, 21, 0, 0
	ret
	.size	_Z10muladdf_lvfi, .-_Z10muladdf_lvfi
	.align	1
	.globl	_Z5icmpsi
	.type	_Z5icmpsi, @function
_Z5icmpsi:
	SFPLOADI	L2, 1, 4
	SFPLOADI	L0, 1, 4
	lui	a5,%hi(_ZN7ckernel13instrn_bufferE)
	slli	a3,a0,16
	lw	a4,%lo(_ZN7ckernel13instrn_bufferE)(a5)
	srli	a3,a3,16
	li	a5, 1897529344	# 711a0000
	srli	a0,a0,16
	add	a3,a3,a5
	li	a5, 1897398272	# 71180000
	add	a5,a0,a5
	TTREPLAY	0, 12, 1, 1
	sw	a3, 0(a4)	# Op(0x71) - d(lr1)
	sw	a5, 0(a4)	# Op(0x71) lv(lr1)  d(lr1)
	SFPIADD	L1, L2, 0, 6
	SFPSETCC	L1, 0, 6
	SFPLOADI	L1, 0, 0
	SFPSTORE	0, L1, 0, 3
	SFPENCC	3, 10
	SFPIADD	L1, L0, -1, 5
	SFPSETCC	L1, 0, 6
	SFPLOADI	L1, 0, 0
	SFPSTORE	0, L1, 0, 3
	SFPENCC	3, 10
	TTREPLAY	0, 12, 0, 0
	TTREPLAY	0, 12, 0, 0
	TTREPLAY	0, 7, 0, 0
	SFPIADD	L0, L0, -1, 5
	SFPSETCC	L0, 0, 6
	SFPLOADI	L0, 0, 0
	SFPSTORE	0, L0, 0, 3
	SFPENCC	3, 10
	ret
	.size	_Z5icmpsi, .-_Z5icmpsi
	.align	1
	.globl	_Z7muladdii
	.type	_Z7muladdii, @function
_Z7muladdii:
	SFPLOADI	L1, 1, 4
	lui	a5,%hi(_ZN7ckernel13instrn_bufferE)
	slli	a3,a0,16
	srli	a4,a0,16
	srli	a3,a3,16
	li	a1, 1896480768	# 710a0000
	li	a2, 1896349696	# 71080000
	lw	a5,%lo(_ZN7ckernel13instrn_bufferE)(a5)
	add	a1,a3,a1
	add	a2,a4,a2
	TTREPLAY	0, 8, 1, 1
	sw	a1, 0(a5)	# Op(0x71) - d(lr0)
	sw	a2, 0(a5)	# Op(0x71) lv(lr0)  d(lr0)
	SFPIADD	L0, L1, 0, 4
	SFPIADD	L0, L0, 1, 5
	li	a0, 1897529344	# 711a0000
	add	a3,a3,a0
	li	a0, 1897398272	# 71180000
	add	a4,a4,a0
	sw	a3, 0(a5)	# Op(0x71) - d(lr1)
	sw	a4, 0(a5)	# Op(0x71) lv(lr1)  d(lr1)
	SFPIADD	L1, L0, 0, 6
	SFPIADD	L1, L1, -1, 5
	TTREPLAY	0, 8, 0, 0
	TTREPLAY	0, 8, 0, 0
	TTREPLAY	0, 8, 0, 0
	ret
	.size	_Z7muladdii, .-_Z7muladdii
	.align	1
	.globl	_Z10muladdi_lvi
	.type	_Z10muladdi_lvi, @function
_Z10muladdi_lvi:
	SFPLOADI	L0, 1, 4
	SFPLOADI	L1, 1, 4
	TTREPLAY	0, 13, 1, 1
	SFPIADD	L2, L0, -2, 5
	SFPSETCC	L2, 0, 6
	lui	a5,%hi(_ZN7ckernel13instrn_bufferE)
	slli	a3,a0,16
	srli	a4,a0,16
	srli	a3,a3,16
	li	a1, 1898577920	# 712a0000
	li	a2, 1898446848	# 71280000
	lw	a5,%lo(_ZN7ckernel13instrn_bufferE)(a5)
	add	a1,a3,a1
	add	a2,a4,a2
	sw	a1, 0(a5)	# Op(0x71) - d(lr2)
	sw	a2, 0(a5)	# Op(0x71) lv(lr2)  d(lr2)
	SFPIADD	L2, L0, 0, 4
	SFPMOV	L1, L2, 0
	SFPIADD	L0, L1, 1, 5
	mv	a3,a1
	mv	a4,a2
	sw	a1, 0(a5)	# Op(0x71) - d(lr2)
	sw	a2, 0(a5)	# Op(0x71) lv(lr2)  d(lr2)
	SFPIADD	L2, L0, 0, 6
	SFPMOV	L1, L2, 0
	SFPIADD	L0, L1, -1, 5
	SFPENCC	3, 10
	TTREPLAY	0, 13, 0, 0
	TTREPLAY	0, 13, 0, 0
	TTREPLAY	0, 13, 0, 0
	ret
	.size	_Z10muladdi_lvi, .-_Z10muladdi_lvi
	.align	1
	.globl	_Z5otherif
	.type	_Z5otherif, @function
_Z5otherif:
	li	a5,16384
	slli	a3,a0,1
	addi	a5,a5,-1
	li	t4, 1879097344	# 7000c000
	and	a3,a3,a5
	lui	a5,%hi(_ZN7ckernel13instrn_bufferE)
	add	t4,a3,t4
	lw	a5,%lo(_ZN7ckernel13instrn_bufferE)(a5)
	TTREPLAY	0, 11, 1, 1
	sw	t4, 0(a5)	# Op(0x70) - d(lr0)
	li	t3, 1880145920	# 7010c000
	add	t3,a3,t3
	sw	t3, 0(a5)	# Op(0x70) - d(lr1)
	li	a4,16773120
	slli	a2,a0,12
	li	t1, 2298478849	# 89000101
	and	a2,a2,a4
	add	t1,a2,t1
	sw	t1, 0(a5)	# Op(0x89) - d(lr0) s(lr1)
	li	a4, 1896480768	# 710a0000
	slli	a7,a0,16
	srli	a0,a0,16
	srli	a7,a7,16
	add	a7,a7,a4
	li	a4, 1896349696	# 71080000
	add	a4,a0,a4
	sw	a7, 0(a5)	# Op(0x71) - d(lr0)
	sw	a4, 0(a5)	# Op(0x71) lv(lr0)  d(lr0)
	SFPSETMAN	L0, L1, 0, 0
	li	a6, 2181038097	# 82000011
	add	a6,a2,a6
	sw	a6, 0(a5)	# Op(0x82) - d(lr1) s(lr0)
	li	a1, 1979711777	# 76000121
	add	a2,a2,a1
	sw	a2, 0(a5)	# Op(0x76) - d(lr2) s(lr1)
	li	a0, 1914748928	# 7220c000
	add	a0,a3,a0
	sw	a0, 0(a5)	# Op(0x72) s(lr2)
	li	a1, 1913700352	# 7210c000
	add	a1,a3,a1
	sw	a1, 0(a5)	# Op(0x72) s(lr1)
	li	t5, 1912651776	# 7200c000
	add	a3,a3,t5
	sw	a3, 0(a5)	# Op(0x72) s(lr0)
	TTREPLAY	0, 11, 0, 0
	TTREPLAY	0, 11, 0, 0
	TTREPLAY	0, 11, 0, 0
	ret
	.size	_Z5otherif, .-_Z5otherif
	.align	1
	.globl	_Z8other_lvif
	.type	_Z8other_lvif, @function
_Z8other_lvif:
	SFPLOADI	L3, 16256, 0
	SFPMAD	L0, L10, L11, L3, 0
	SFPNOP
	SFPSETCC	L0, 0, 6
	li	a5,16384
	slli	a3,a0,1
	addi	a5,a5,-1
	li	t4, 1879097344	# 7000c000
	and	a3,a3,a5
	lui	a5,%hi(_ZN7ckernel13instrn_bufferE)
	add	t4,a3,t4
	lw	a5,%lo(_ZN7ckernel13instrn_bufferE)(a5)
	sw	t4, 0(a5)	# Op(0x70) - d(lr0)
	mv	t3,t4
	sw	t4, 0(a5)	# Op(0x70) - d(lr0)
	li	a4,16773120
	slli	a2,a0,12
	li	t1, 2298478609	# 89000011
	and	a2,a2,a4
	add	t1,a2,t1
	sw	t1, 0(a5)	# Op(0x89) - d(lr1) s(lr0)
	li	a4, 1898577920	# 712a0000
	slli	a7,a0,16
	srli	a0,a0,16
	srli	a7,a7,16
	add	a7,a7,a4
	li	a4, 1898446848	# 71280000
	add	a4,a0,a4
	sw	a7, 0(a5)	# Op(0x71) - d(lr2)
	sw	a4, 0(a5)	# Op(0x71) lv(lr2)  d(lr2)
	SFPSETMAN	L2, L0, 0, 0
	li	a6, 2181038593	# 82000201
	add	a6,a2,a6
	sw	a6, 0(a5)	# Op(0x82) - d(lr0) s(lr2)
	li	a1, 1979711505	# 76000011
	add	a2,a2,a1
	sw	a2, 0(a5)	# Op(0x76) - d(lr1) s(lr0)
	li	a0, 1913700352	# 7210c000
	add	a0,a3,a0
	TTREPLAY	0, 16, 1, 1
	sw	a0, 0(a5)	# Op(0x72) s(lr1)
	li	a1, 1912651776	# 7200c000
	add	a1,a3,a1
	sw	a1, 0(a5)	# Op(0x72) s(lr0)
	li	t5, 1914748928	# 7220c000
	add	a3,a3,t5
	sw	a3, 0(a5)	# Op(0x72) s(lr2)
	SFPENCC	3, 10
	SFPMAD	L4, L10, L11, L3, 0
	SFPNOP
	SFPSETCC	L4, 0, 6
	li	t5, 1048576
	xor	t5, t5, t4
	sw	t5, 0(a5)	# Op(0x70) lv(lr1)  d(lr1)
	sw	t4, 0(a5)	# Op(0x70) lv(lr0)  d(lr0)
	sw	t1, 0(a5)	# Op(0x89) lv(lr1)  d(lr1) s(lr0)
	li	t5, 6291456
	xor	t5, t5, a7
	sw	t5, 0(a5)	# Op(0x71) - d(lr4)
	li	t5, 6291456
	xor	t5, t5, a4
	sw	t5, 0(a5)	# Op(0x71) lv(lr4)  d(lr4)
	SFPSETMAN	L4, L0, 0, 0
	SFPMOV	L2, L4, 0
	sw	a6, 0(a5)	# Op(0x82) lv(lr0)  d(lr0) s(lr2)
	sw	a2, 0(a5)	# Op(0x76) lv(lr1)  d(lr1) s(lr0)
	TTREPLAY	0, 16, 0, 0
	TTREPLAY	0, 4, 0, 0
	SFPMAD	L3, L10, L11, L3, 0
	SFPNOP
	SFPSETCC	L3, 0, 6
	li	t5, 1048576
	xor	t5, t5, t4
	sw	t5, 0(a5)	# Op(0x70) lv(lr1)  d(lr1)
	sw	t3, 0(a5)	# Op(0x70) lv(lr0)  d(lr0)
	sw	t1, 0(a5)	# Op(0x89) lv(lr1)  d(lr1) s(lr0)
	li	t1, 1048576
	xor	t1, t1, a7
	sw	t1, 0(a5)	# Op(0x71) - d(lr3)
	li	a7, 1048576
	xor	a7, a7, a4
	sw	a7, 0(a5)	# Op(0x71) lv(lr3)  d(lr3)
	SFPSETMAN	L3, L0, 0, 0
	SFPMOV	L2, L3, 0
	sw	a6, 0(a5)	# Op(0x82) lv(lr0)  d(lr0) s(lr2)
	sw	a2, 0(a5)	# Op(0x76) lv(lr1)  d(lr1) s(lr0)
	TTREPLAY	0, 4, 0, 0
	ret
	.size	_Z8other_lvif, .-_Z8other_lvif
	.align	1
	.globl	_Z8stochrndi
	.type	_Z8stochrndi, @function
_Z8stochrndi:
	SFPLOADI	L0, 1, 4
	li	a4,65536
	slli	a5,a0,8
	addi	a4,a4,-1
	li	a3, 2382364700	# 8e00001c
	and	a5,a5,a4
	lui	a4,%hi(_ZN7ckernel13instrn_bufferE)
	add	a3,a5,a3
	lw	a4,%lo(_ZN7ckernel13instrn_bufferE)(a4)
	TTREPLAY	0, 3, 1, 1
	sw	a3, 0(a4)	# Op(0x8e) - d(lr1) s(lr0)
	li	a2, 2382364701	# 8e00001d
	add	a5,a5,a2
	sw	a5, 0(a4)	# Op(0x8e) - d(lr1) s(lr0)
	sw	a3, 0(a4)	# Op(0x8e) - d(lr1) s(lr0)
	sw	a5, 0(a4)	# Op(0x8e) - d(lr1) s(lr0)
	TTREPLAY	0, 3, 0, 0
	li	a3, 16
	xor	a3, a3, a5
	sw	a3, 0(a4)	# Op(0x8e) - d(lr0) s(lr0)
	SFPSTORE	0, L0, 0, 3
	ret
	.size	_Z8stochrndi, .-_Z8stochrndi
	.align	1
	.globl	_Z11stochrnd_lvi
	.type	_Z11stochrnd_lvi, @function
_Z11stochrnd_lvi:
	SFPLOADI	L1, 1, 4
	SFPLOADI	L0, 1, 4
	TTREPLAY	0, 5, 1, 1
	SFPIADD	L2, L1, -1, 5
	SFPSETCC	L2, 0, 6
	li	a4,65536
	slli	a5,a0,8
	addi	a4,a4,-1
	li	a3, 2382364940	# 8e00010c
	and	a5,a5,a4
	lui	a4,%hi(_ZN7ckernel13instrn_bufferE)
	add	a3,a5,a3
	lw	a4,%lo(_ZN7ckernel13instrn_bufferE)(a4)
	sw	a3, 0(a4)	# Op(0x8e) lv(lr0)  d(lr0) s(lr1)
	li	a2, 2382364941	# 8e00010d
	add	a5,a5,a2
	sw	a5, 0(a4)	# Op(0x8e) lv(lr0)  d(lr0) s(lr1)
	SFPENCC	3, 10
	TTREPLAY	0, 5, 0, 0
	TTREPLAY	0, 5, 0, 0
	TTREPLAY	0, 5, 0, 0
	SFPSTORE	0, L0, 0, 3
	ret
	.size	_Z11stochrnd_lvi, .-_Z11stochrnd_lvi
	.align	1
	.globl	_Z4shfti
	.type	_Z4shfti, @function
_Z4shfti:
	SFPLOADI	L0, 1, 4
	li	a2,16773120
	slli	a3,a0,12
	lui	a5,%hi(_ZN7ckernel13instrn_bufferE)
	and	a3,a3,a2
	lw	a4,%lo(_ZN7ckernel13instrn_bufferE)(a5)
	li	a5, 2046820353	# 7a000001
	add	a3,a3,a5
	TTREPLAY	0, 3, 1, 1
	sw	a3, 0(a4)	# Op(0x7a) lv(lr0)  d(lr0)
	neg	a5,a0
	slli	a5,a5,12
	and	a5,a5,a2
	li	a2, 2046820353	# 7a000001
	add	a5,a5,a2
	sw	a5, 0(a4)	# Op(0x7a) lv(lr0)  d(lr0)
	sw	a3, 0(a4)	# Op(0x7a) lv(lr0)  d(lr0)
	sw	a5, 0(a4)	# Op(0x7a) lv(lr0)  d(lr0)
	TTREPLAY	0, 3, 0, 0
	SFPMOV	L1, L0, 0
	li	a3, 16
	xor	a3, a3, a5
	sw	a3, 0(a4)	# Op(0x7a) lv(lr1)  d(lr1)
	SFPSTORE	0, L0, 0, 3
	SFPSTORE	0, L1, 0, 3
	ret
	.size	_Z4shfti, .-_Z4shfti
	.align	1
	.globl	_Z7shft_lvi
	.type	_Z7shft_lvi, @function
_Z7shft_lvi:
	SFPLOADI	L0, 1, 4
	SFPLOADI	L1, 1, 4
	TTREPLAY	0, 9, 1, 1
	SFPIADD	L2, L0, -1, 5
	SFPSETCC	L2, 0, 6
	lui	a5,%hi(_ZN7ckernel13instrn_bufferE)
	slli	a3,a0,12
	li	a2,16773120
	lw	a4,%lo(_ZN7ckernel13instrn_bufferE)(a5)
	and	a3,a3,a2
	SFPMOV	L2, L1, 0
	li	a5, 2046820385	# 7a000021
	add	a3,a3,a5
	sw	a3, 0(a4)	# Op(0x7a) lv(lr2)  d(lr2)
	SFPMOV	L0, L2, 0
	neg	a5,a0
	slli	a5,a5,12
	and	a5,a5,a2
	SFPMOV	L2, L0, 0
	li	a2, 2046820385	# 7a000021
	add	a5,a5,a2
	sw	a5, 0(a4)	# Op(0x7a) lv(lr2)  d(lr2)
	SFPMOV	L1, L2, 0
	SFPENCC	3, 10
	TTREPLAY	0, 9, 0, 0
	TTREPLAY	0, 9, 0, 0
	TTREPLAY	0, 9, 0, 0
	SFPSTORE	0, L0, 0, 3
	SFPSTORE	0, L1, 0, 3
	ret
	.size	_Z7shft_lvi, .-_Z7shft_lvi
	.align	1
	.globl	_Z4mulii
	.type	_Z4mulii, @function
_Z4mulii:
	slli	a0,a0,16
	li	a3, 1946157056	# 74000000
	srli	a0,a0,16
	lui	a4,%hi(_ZN7ckernel13instrn_bufferE)
	slli	a5,a0,8
	lw	a4,%lo(_ZN7ckernel13instrn_bufferE)(a4)
	add	a5,a5,a3
	li	a3, 1896873984	# 71100000
	add	a0,a0,a3
	sw	a0, 0(a4)	# Op(0x71) - d(lr1)
	SFPLOADI	L0, 16256, 0
	TTREPLAY	0, 4, 1, 1
	sw	a5, 0(a4)	# Op(0x74) lv(lr0)  d(lr0)
	SFPNOP
	sw	a5, 0(a4)	# Op(0x74) lv(lr0)  d(lr0)
	SFPNOP
	TTREPLAY	0, 4, 0, 0
	SFPSTORE	0, L0, 0, 3
	SFPSTORE	0, L1, 0, 3
	ret
	.size	_Z4mulii, .-_Z4mulii
	.align	1
	.globl	_Z13muli_nounrolli
	.type	_Z13muli_nounrolli, @function
_Z13muli_nounrolli:
	slli	a4,a0,16
	li	a0, 1895825408	# 71000000
	srli	a4,a4,16
	li	a5,4
	lui	a3,%hi(_ZN7ckernel13instrn_bufferE)
	add	a0,a0,a4
.L24:
	lw	a4,%lo(_ZN7ckernel13instrn_bufferE)(a3)
	sw	a0, 0(a4)	# Op(0x71) - d(lr0)
	SFPSTORE	0, L0, 0, 3
	addi	a5,a5,-1
	bne	a5,zero,.L24
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
