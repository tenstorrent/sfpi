	.file	"combine.cc"
	.option nopic
	.attribute arch, "rv32i2p0_m2p0_w2p0"
	.attribute unaligned_access, 0
	.attribute stack_align, 16
	.text
	.align	1
	.globl	_Z11iadd_i_yes1v
	.type	_Z11iadd_i_yes1v, @function
_Z11iadd_i_yes1v:
	SFPLOADI	L0, 4, 3
	SFPIADD	-5, L0, L0, 1
	SFPSTORE	L10, 0, 3, 0
	SFPENCC	3, 10
	ret
	.size	_Z11iadd_i_yes1v, .-_Z11iadd_i_yes1v
	.align	1
	.globl	_Z16iadd_i_yes1_livev
	.type	_Z16iadd_i_yes1_livev, @function
_Z16iadd_i_yes1_livev:
	SFPLOADI	L0, 4, 3
	SFPSETCC	0, L0, 0
	SFPIADD	-5, L0, L0, 1
	SFPSTORE	L10, 0, 3, 0
	SFPENCC	3, 10
	ret
	.size	_Z16iadd_i_yes1_livev, .-_Z16iadd_i_yes1_livev
	.align	1
	.globl	_Z11iadd_i_yes2v
	.type	_Z11iadd_i_yes2v, @function
_Z11iadd_i_yes2v:
	SFPLOADI	L0, 4, 3
	SFPIADD	-7, L0, L0, 1
	SFPSTORE	L10, 0, 3, 0
	SFPENCC	3, 10
	ret
	.size	_Z11iadd_i_yes2v, .-_Z11iadd_i_yes2v
	.align	1
	.globl	_Z12iadd_i_yes3av
	.type	_Z12iadd_i_yes3av, @function
_Z12iadd_i_yes3av:
	SFPLOADI	L0, 4, 3
	SFPLOADI	L1, 4, 4
	SFPIADD	-5, L0, L0, 1
	SFPIADD	-6, L1, L2, 5
	SFPSETCC	0, L2, 6
	SFPSTORE	L10, 0, 3, 0
	SFPENCC	3, 10
	SFPSTORE	L0, 0, 3, 0
	SFPSTORE	L1, 0, 3, 0
	ret
	.size	_Z12iadd_i_yes3av, .-_Z12iadd_i_yes3av
	.align	1
	.globl	_Z12iadd_i_yes3bv
	.type	_Z12iadd_i_yes3bv, @function
_Z12iadd_i_yes3bv:
	SFPLOADI	L0, 4, 3
	SFPLOADI	L1, 4, 4
	SFPIADD	-6, L1, L1, 5
	SFPSETCC	0, L1, 6
	SFPIADD	-5, L0, L0, 1
	SFPSTORE	L10, 0, 3, 0
	SFPENCC	3, 10
	ret
	.size	_Z12iadd_i_yes3bv, .-_Z12iadd_i_yes3bv
	.align	1
	.globl	_Z12iadd_i_yes3cv
	.type	_Z12iadd_i_yes3cv, @function
_Z12iadd_i_yes3cv:
	SFPLOADI	L0, 4, 3
	SFPLOADI	L1, 4, 4
	SFPIADD	-6, L1, L1, 5
	SFPSETCC	0, L1, 6
	SFPIADD	-5, L0, L0, 1
	SFPSTORE	L10, 0, 3, 0
	SFPENCC	3, 10
	ret
	.size	_Z12iadd_i_yes3cv, .-_Z12iadd_i_yes3cv
	.align	1
	.globl	_Z12iadd_i_yes3dv
	.type	_Z12iadd_i_yes3dv, @function
_Z12iadd_i_yes3dv:
	SFPLOADI	L0, 4, 3
	SFPIADD	-5, L0, L0, 5
	SFPIADD	0, L0, L1, 5
	SFPSETCC	0, L0, 0
	SFPSTORE	L10, 0, 3, 0
	SFPENCC	3, 10
	ret
	.size	_Z12iadd_i_yes3dv, .-_Z12iadd_i_yes3dv
	.align	1
	.globl	_Z12iadd_i_yes4av
	.type	_Z12iadd_i_yes4av, @function
_Z12iadd_i_yes4av:
	SFPLOADI	L1, 4, 3
	SFPLOADI	L0, 4, 4
	SFPIADD	-5, L1, L1, 1
	SFPIADD	-6, L0, L0, 5
	SFPSETCC	0, L0, 6
	SFPSTORE	L10, 0, 3, 0
	SFPENCC	3, 10
	ret
	.size	_Z12iadd_i_yes4av, .-_Z12iadd_i_yes4av
	.align	1
	.globl	_Z11iadd_i_yes5v
	.type	_Z11iadd_i_yes5v, @function
_Z11iadd_i_yes5v:
	SFPLOADI	L0, 4, 3
	SFPIADD	5, L0, L0, 9
	SFPSTORE	L10, 0, 3, 0
	SFPENCC	3, 10
	ret
	.size	_Z11iadd_i_yes5v, .-_Z11iadd_i_yes5v
	.align	1
	.globl	_Z11iadd_i_yes6v
	.type	_Z11iadd_i_yes6v, @function
_Z11iadd_i_yes6v:
	SFPLOADI	L0, 4, 3
	SFPLOADI	L1, 4, 4
	SFPIADD	-5, L0, L2, 9
	SFPIADD	6, L1, L0, 9
	SFPSTORE	L10, 0, 3, 0
	SFPENCC	3, 10
	ret
	.size	_Z11iadd_i_yes6v, .-_Z11iadd_i_yes6v
	.align	1
	.globl	_Z11iadd_i_yes7v
	.type	_Z11iadd_i_yes7v, @function
_Z11iadd_i_yes7v:
	SFPLOADI	L0, 4, 4
	SFPIADD	6, L0, L1, 9
	SFPSTORE	L10, 0, 3, 0
	SFPENCC	3, 10
	SFPSTORE	L1, 0, 3, 0
	SFPSTORE	L0, 0, 3, 0
	ret
	.size	_Z11iadd_i_yes7v, .-_Z11iadd_i_yes7v
	.align	1
	.globl	_Z11iadd_i_yes8v
	.type	_Z11iadd_i_yes8v, @function
_Z11iadd_i_yes8v:
	SFPLOADI	L0, 4, 3
	SFPIADD	5, L0, L1, 1
	SFPSTORE	L10, 0, 3, 0
	SFPENCC	3, 10
	SFPSTORE	L0, 0, 3, 2
	SFPSTORE	L1, 0, 3, 4
	ret
	.size	_Z11iadd_i_yes8v, .-_Z11iadd_i_yes8v
	.align	1
	.globl	_Z11iadd_i_yes9v
	.type	_Z11iadd_i_yes9v, @function
_Z11iadd_i_yes9v:
	SFPLOADI	L1, 4, 3
	SFPLOADI	L0, 4, 5
	SFPIADD	-7, L1, L1, 1
	SFPIADD	-9, L0, L0, 1
	SFPSTORE	L10, 0, 3, 0
	SFPENCC	3, 10
	ret
	.size	_Z11iadd_i_yes9v, .-_Z11iadd_i_yes9v
	.align	1
	.globl	_Z10iadd_i_no1v
	.type	_Z10iadd_i_no1v, @function
_Z10iadd_i_no1v:
	SFPLOADI	L1, 4, 3
	SFPLOADI	L0, 4, 4
	SFPIADD	-6, L0, L0, 5
	SFPSETCC	0, L1, 0
	SFPSTORE	L10, 0, 3, 0
	SFPENCC	3, 10
	ret
	.size	_Z10iadd_i_no1v, .-_Z10iadd_i_no1v
	.align	1
	.globl	_Z10iadd_i_no2v
	.type	_Z10iadd_i_no2v, @function
_Z10iadd_i_no2v:
	SFPLOADI	L0, 4, 3
	SFPIADD	-5, L0, L1, 5
	SFPSETCC	0, L0, 0
	SFPSTORE	L10, 0, 3, 0
	SFPENCC	3, 10
	ret
	.size	_Z10iadd_i_no2v, .-_Z10iadd_i_no2v
	.align	1
	.globl	_Z10iadd_i_no3v
	.type	_Z10iadd_i_no3v, @function
_Z10iadd_i_no3v:
	SFPLOADI	L0, 4, 3
	SFPIADD	-5, L0, L0, 5
	SFPIADD	-7, L0, L0, 1
	SFPSTORE	L10, 0, 3, 0
	SFPENCC	3, 10
	SFPSTORE	L0, 0, 3, 0
	ret
	.size	_Z10iadd_i_no3v, .-_Z10iadd_i_no3v
	.align	1
	.globl	_Z10iadd_i_no4v
	.type	_Z10iadd_i_no4v, @function
_Z10iadd_i_no4v:
	SFPLOADI	L1, 4, 3
	SFPLOADI	L0, 4, 4
	SFPIADD	-5, L1, L1, 5
	SFPIADD	-7, L1, L1, 5
	SFPIADD	-6, L0, L0, 5
	SFPIADD	-9, L0, L0, 5
	SFPSTORE	L0, 0, 3, 0
	ret
	.size	_Z10iadd_i_no4v, .-_Z10iadd_i_no4v
	.align	1
	.globl	_Z11iadd_i_no4bv
	.type	_Z11iadd_i_no4bv, @function
_Z11iadd_i_no4bv:
	SFPLOADI	L0, 4, 3
	SFPLOADI	L1, 4, 4
	SFPIADD	-5, L0, L0, 5
	SFPIADD	-6, L1, L1, 9
	SFPSETCC	0, L0, 0
	SFPSTORE	L10, 0, 3, 0
	SFPENCC	3, 10
	ret
	.size	_Z11iadd_i_no4bv, .-_Z11iadd_i_no4bv
	.align	1
	.globl	_Z10iadd_i_no5v
	.type	_Z10iadd_i_no5v, @function
_Z10iadd_i_no5v:
	SFPLOADI	L1, 4, 3
	SFPLOADI	L0, 4, 4
	SFPSETCC	0, L1, 0
	SFPSETCC	0, L0, 0
	SFPSTORE	L10, 0, 3, 0
	SFPENCC	3, 10
	ret
	.size	_Z10iadd_i_no5v, .-_Z10iadd_i_no5v
	.align	1
	.globl	_Z10iadd_i_no6v
	.type	_Z10iadd_i_no6v, @function
_Z10iadd_i_no6v:
	SFPLOADI	L0, 4, 3
	SFPIADD	5, L0, L0, 5
	SFPIADD	0, L0, L0, 5
	ret
	.size	_Z10iadd_i_no6v, .-_Z10iadd_i_no6v
	.align	1
	.globl	_Z10iadd_i_no7v
	.type	_Z10iadd_i_no7v, @function
_Z10iadd_i_no7v:
	SFPLOADI	L0, 4, 3
	SFPIADD	-5, L0, L0, 5
	SFPIADD	7, L0, L1, 5
	SFPSETCC	0, L0, 0
	SFPSTORE	L10, 0, 3, 0
	SFPENCC	3, 10
	ret
	.size	_Z10iadd_i_no7v, .-_Z10iadd_i_no7v
	.align	1
	.globl	_Z10iadd_i_no8v
	.type	_Z10iadd_i_no8v, @function
_Z10iadd_i_no8v:
	SFPLOADI	L0, 4, 3
	SFPLOADI	L1, 4, 5
	SFPIADD	-7, L0, L0, 5
	SFPIADD	-9, L1, L1, 1
	SFPSETCC	0, L0, 0
	SFPSTORE	L10, 0, 3, 0
	SFPENCC	3, 10
	ret
	.size	_Z10iadd_i_no8v, .-_Z10iadd_i_no8v
	.align	1
	.globl	_Z10iadd_i_no9v
	.type	_Z10iadd_i_no9v, @function
_Z10iadd_i_no9v:
	SFPLOADI	L0, 4, 3
	SFPIADD	-3, L0, L0, 5
	SFPIADD	0, L0, L1, 5
	SFPSETCC	0, L0, 0
	SFPSTORE	L10, 0, 3, 0
	SFPENCC	3, 10
	SFPSTORE	L1, 0, 3, 0
	ret
	.size	_Z10iadd_i_no9v, .-_Z10iadd_i_no9v
	.align	1
	.globl	_Z11iadd_i_no10v
	.type	_Z11iadd_i_no10v, @function
_Z11iadd_i_no10v:
	addi	sp,sp,-16
	sw	ra,12(sp)
	SFPLOADI	L0, 4, 3
	call	rand
	SFPIADD	-5, L0, L0, 5
	beq	a0,zero,.L29
	li	a5,1
	beq	a0,a5,.L30
	lw	ra,12(sp)
	addi	sp,sp,16
	jr	ra
.L30:
	SFPSETCC	0, L0, 0
	SFPSTORE	L9, 0, 3, 0
	SFPENCC	3, 10
	lw	ra,12(sp)
	addi	sp,sp,16
	jr	ra
.L29:
	SFPSETCC	0, L0, 0
	SFPSTORE	L10, 0, 3, 0
	SFPENCC	3, 10
	lw	ra,12(sp)
	addi	sp,sp,16
	jr	ra
	.size	_Z11iadd_i_no10v, .-_Z11iadd_i_no10v
	.align	1
	.globl	_Z11iadd_i_no11v
	.type	_Z11iadd_i_no11v, @function
_Z11iadd_i_no11v:
	SFPLOADI	L0, 4, 3
	SFPIADD	-5, L0, L0, 1
	SFPIADD	2, L0, L0, 5
	SFPCOMPC
	SFPSETCC	0, L0, 0
	SFPSTORE	L10, 0, 3, 0
	SFPENCC	3, 10
	ret
	.size	_Z11iadd_i_no11v, .-_Z11iadd_i_no11v
	.align	1
	.globl	_Z11iadd_i_no12j
	.type	_Z11iadd_i_no12j, @function
_Z11iadd_i_no12j:
	SFPLOADI	L0, 4, 3
	SFPIADD	-5, L0, L0, 5
	lui	a5,%hi(_ZN7ckernel13instrn_bufferE)
	lw	a4,%lo(_ZN7ckernel13instrn_bufferE)(a5)
	slli	a5,a0,16
	li	a3, 1897529344	# 711a0000
	srli	a5,a5,16
	add	a5,a5,a3
	sw	a5, 0(a4)	# Op(0x71) - d(lr1)
	li	a5, 1897398272	# 71180000
	srli	a0,a0,16
	add	a0,a0,a5
	sw	a0, 0(a4)	# Op(0x71) lv(lr1)  d(lr1)
	SFPIADD	0, L0, L1, 2
	SFPIADD	2, L0, L0, 5
	SFPENCC	3, 10
	SFPSTORE	L0, 0, 3, 0
	ret
	.size	_Z11iadd_i_no12j, .-_Z11iadd_i_no12j
	.align	1
	.globl	_Z13iadd_i_no_bb1i
	.type	_Z13iadd_i_no_bb1i, @function
_Z13iadd_i_no_bb1i:
	SFPLOADI	L0, 4, 1
	SFPIADD	-5, L0, L0, 5
	bgt	a0,zero,.L37
	SFPSTORE	L0, 0, 3, 0
	ret
.L37:
	li	a5,0
.L35:
	SFPSETCC	0, L0, 0
	SFPIADD	1, L0, L0, 5
	SFPENCC	3, 10
	addi	a5,a5,1
	bne	a0,a5,.L35
	SFPSTORE	L0, 0, 3, 0
	ret
	.size	_Z13iadd_i_no_bb1i, .-_Z13iadd_i_no_bb1i
	.align	1
	.globl	_Z11iadd_v_yes1v
	.type	_Z11iadd_v_yes1v, @function
_Z11iadd_v_yes1v:
	SFPLOADI	L0, 4, 3
	SFPLOADI	L1, 4, 4
	SFPIADD	0, L1, L0, 2
	SFPSTORE	L10, 0, 3, 0
	SFPENCC	3, 10
	ret
	.size	_Z11iadd_v_yes1v, .-_Z11iadd_v_yes1v
	.align	1
	.globl	_Z11iadd_v_yes2v
	.type	_Z11iadd_v_yes2v, @function
_Z11iadd_v_yes2v:
	SFPLOADI	L1, 4, 3
	SFPLOADI	L0, 4, 4
	SFPIADD	0, L1, L0, 10
	SFPSTORE	L10, 0, 3, 0
	SFPENCC	3, 10
	ret
	.size	_Z11iadd_v_yes2v, .-_Z11iadd_v_yes2v
	.align	1
	.globl	_Z11iadd_v_yes3v
	.type	_Z11iadd_v_yes3v, @function
_Z11iadd_v_yes3v:
	SFPLOADI	L1, 4, 3
	SFPLOADI	L0, 4, 4
	SFPIADD	0, L1, L0, 0
	SFPSTORE	L10, 0, 3, 0
	SFPENCC	3, 10
	ret
	.size	_Z11iadd_v_yes3v, .-_Z11iadd_v_yes3v
	.align	1
	.globl	_Z12iadd_v_yes4av
	.type	_Z12iadd_v_yes4av, @function
_Z12iadd_v_yes4av:
	SFPLOADI	L1, 4, 3
	SFPLOADI	L0, 4, 4
	SFPIADD	0, L1, L0, 10
	SFPSTORE	L10, 0, 3, 0
	SFPENCC	3, 10
	ret
	.size	_Z12iadd_v_yes4av, .-_Z12iadd_v_yes4av
	.align	1
	.globl	_Z12iadd_v_yes4bv
	.type	_Z12iadd_v_yes4bv, @function
_Z12iadd_v_yes4bv:
	SFPLOADI	L1, 4, 3
	SFPLOADI	L0, 4, 4
	SFPIADD	0, L0, L1, 0
	SFPSTORE	L10, 0, 3, 0
	SFPENCC	3, 10
	SFPSTORE	L0, 0, 3, 0
	ret
	.size	_Z12iadd_v_yes4bv, .-_Z12iadd_v_yes4bv
	.align	1
	.globl	_Z10iadd_v_no1v
	.type	_Z10iadd_v_no1v, @function
_Z10iadd_v_no1v:
	SFPLOADI	L0, 4, 3
	SFPLOADI	L1, 4, 4
	SFPIADD	0, L1, L0, 4
	SFPIADD	-5, L0, L0, 1
	SFPSTORE	L10, 0, 3, 0
	SFPENCC	3, 10
	ret
	.size	_Z10iadd_v_no1v, .-_Z10iadd_v_no1v
	.align	1
	.globl	_Z10iadd_v_no2v
	.type	_Z10iadd_v_no2v, @function
_Z10iadd_v_no2v:
	SFPLOADI	L1, 4, 3
	SFPLOADI	L0, 4, 4
	SFPIADD	0, L1, L0, 6
	SFPIADD	-5, L0, L0, 1
	SFPSTORE	L10, 0, 3, 0
	SFPENCC	3, 10
	ret
	.size	_Z10iadd_v_no2v, .-_Z10iadd_v_no2v
	.align	1
	.globl	_Z10iadd_v_no3v
	.type	_Z10iadd_v_no3v, @function
_Z10iadd_v_no3v:
	SFPLOADI	L1, 4, 3
	SFPLOADI	L0, 4, 4
	SFPIADD	0, L1, L0, 6
	SFPIADD	0, L0, L0, 5
	ret
	.size	_Z10iadd_v_no3v, .-_Z10iadd_v_no3v
	.align	1
	.globl	_Z10iadd_v_no4v
	.type	_Z10iadd_v_no4v, @function
_Z10iadd_v_no4v:
	SFPLOADI	L0, 4, 3
	SFPLOADI	L1, 4, 4
	SFPIADD	0, L0, L1, 2
	SFPIADD	-5, L0, L0, 5
	SFPSTORE	L10, 0, 3, 0
	SFPENCC	3, 10
	ret
	.size	_Z10iadd_v_no4v, .-_Z10iadd_v_no4v
	.align	1
	.globl	_Z13iadd_v_no_bb1i
	.type	_Z13iadd_v_no_bb1i, @function
_Z13iadd_v_no_bb1i:
	SFPLOADI	L0, 4, 1
	SFPLOADI	L1, 4, 2
	SFPIADD	0, L1, L0, 6
	bgt	a0,zero,.L51
	SFPSTORE	L0, 0, 3, 0
	ret
.L51:
	li	a5,0
.L49:
	SFPSETCC	0, L0, 0
	SFPIADD	1, L0, L0, 5
	SFPENCC	3, 10
	addi	a5,a5,1
	bne	a0,a5,.L49
	SFPSTORE	L0, 0, 3, 0
	ret
	.size	_Z13iadd_v_no_bb1i, .-_Z13iadd_v_no_bb1i
	.align	1
	.globl	_Z7lz_yes1v
	.type	_Z7lz_yes1v, @function
_Z7lz_yes1v:
	SFPLOADI	L0, 0, 16448
	SFPLZ	L0, L1, 10
	SFPSTORE	L10, 0, 3, 0
	SFPENCC	3, 10
	SFPSTORE	L0, 0, 3, 0
	SFPSTORE	L1, 0, 3, 0
	ret
	.size	_Z7lz_yes1v, .-_Z7lz_yes1v
	.align	1
	.globl	_Z7lz_yes2v
	.type	_Z7lz_yes2v, @function
_Z7lz_yes2v:
	SFPLOADI	L0, 0, 16448
	SFPLZ	L0, L1, 2
	SFPSTORE	L10, 0, 3, 0
	SFPENCC	3, 10
	SFPSTORE	L0, 0, 3, 0
	SFPSTORE	L1, 0, 3, 0
	ret
	.size	_Z7lz_yes2v, .-_Z7lz_yes2v
	.align	1
	.globl	_Z6lz_no1v
	.type	_Z6lz_no1v, @function
_Z6lz_no1v:
	SFPLOADI	L1, 0, 16448
	SFPLZ	L1, L0, 0
	SFPSETCC	0, L0, 2
	SFPSTORE	L10, 0, 3, 0
	SFPENCC	3, 10
	SFPSTORE	L1, 0, 3, 0
	SFPSTORE	L0, 0, 3, 0
	ret
	.size	_Z6lz_no1v, .-_Z6lz_no1v
	.align	1
	.globl	_Z6lz_no2v
	.type	_Z6lz_no2v, @function
_Z6lz_no2v:
	SFPLOADI	L0, 0, 16448
	SFPLZ	L0, L1, 0
	SFPSETCC	0, L0, 4
	SFPSTORE	L10, 0, 3, 0
	SFPENCC	3, 10
	SFPSTORE	L0, 0, 3, 0
	SFPSTORE	L1, 0, 3, 0
	ret
	.size	_Z6lz_no2v, .-_Z6lz_no2v
	.align	1
	.globl	_Z6lz_no3v
	.type	_Z6lz_no3v, @function
_Z6lz_no3v:
	SFPLOADI	L0, 0, 16448
	SFPLZ	L0, L1, 0
	SFPSETCC	0, L0, 0
	SFPSTORE	L10, 0, 3, 0
	SFPENCC	3, 10
	SFPSTORE	L0, 0, 3, 0
	SFPSTORE	L1, 0, 3, 0
	ret
	.size	_Z6lz_no3v, .-_Z6lz_no3v
	.align	1
	.globl	_Z9lz_no_bb1i
	.type	_Z9lz_no_bb1i, @function
_Z9lz_no_bb1i:
	SFPLOADI	L0, 0, 16448
	SFPLZ	L0, L1, 0
	bgt	a0,zero,.L61
	SFPSTORE	L0, 0, 3, 0
	ret
.L61:
	li	a5,0
.L59:
	SFPSETCC	0, L0, 0
	SFPADDI	16256, L0, 0
	SFPENCC	3, 10
	addi	a5,a5,1
	bne	a0,a5,.L59
	SFPSTORE	L0, 0, 3, 0
	ret
	.size	_Z9lz_no_bb1i, .-_Z9lz_no_bb1i
	.align	1
	.globl	_Z10exexp_yes1v
	.type	_Z10exexp_yes1v, @function
_Z10exexp_yes1v:
	SFPLOADI	L0, 0, 16448
	SFPEXEXP	L0, L1, 10
	SFPSTORE	L10, 0, 3, 0
	SFPENCC	3, 10
	SFPSTORE	L0, 0, 3, 0
	SFPSTORE	L1, 0, 3, 0
	ret
	.size	_Z10exexp_yes1v, .-_Z10exexp_yes1v
	.align	1
	.globl	_Z15exexp_yes1_livev
	.type	_Z15exexp_yes1_livev, @function
_Z15exexp_yes1_livev:
	SFPLOADI	L1, 0, 16448
	SFPLOADI	L0, 4, 1
	SFPSETCC	0, L1, 0
	SFPEXEXP	L1, L0, 10
	SFPSTORE	L10, 0, 3, 0
	SFPENCC	3, 10
	SFPSTORE	L1, 0, 3, 0
	SFPSTORE	L0, 0, 3, 0
	ret
	.size	_Z15exexp_yes1_livev, .-_Z15exexp_yes1_livev
	.align	1
	.globl	_Z10exexp_yes2v
	.type	_Z10exexp_yes2v, @function
_Z10exexp_yes2v:
	SFPLOADI	L0, 0, 16448
	SFPEXEXP	L0, L1, 2
	SFPSTORE	L10, 0, 3, 0
	SFPENCC	3, 10
	SFPSTORE	L0, 0, 3, 0
	SFPSTORE	L1, 0, 3, 0
	ret
	.size	_Z10exexp_yes2v, .-_Z10exexp_yes2v
	.align	1
	.globl	_Z10exexp_yes3v
	.type	_Z10exexp_yes3v, @function
_Z10exexp_yes3v:
	SFPLOADI	L0, 0, 16448
	SFPEXEXP	L0, L0, 2
	SFPSTORE	L10, 0, 3, 0
	SFPENCC	3, 10
	SFPSTORE	L0, 0, 3, 0
	ret
	.size	_Z10exexp_yes3v, .-_Z10exexp_yes3v
	.align	1
	.globl	_Z9exexp_no1v
	.type	_Z9exexp_no1v, @function
_Z9exexp_no1v:
	SFPLOADI	L1, 0, 16448
	SFPEXEXP	L1, L0, 0
	SFPIADD	-5, L0, L2, 1
	SFPSTORE	L10, 0, 3, 0
	SFPENCC	3, 10
	SFPSTORE	L1, 0, 3, 0
	SFPSTORE	L0, 0, 3, 0
	ret
	.size	_Z9exexp_no1v, .-_Z9exexp_no1v
	.align	1
	.globl	_Z9exexp_no2v
	.type	_Z9exexp_no2v, @function
_Z9exexp_no2v:
	SFPLOADI	L1, 0, 16448
	SFPEXEXP	L1, L0, 1
	SFPSETCC	0, L0, 4
	SFPSTORE	L10, 0, 3, 0
	SFPENCC	3, 10
	SFPSTORE	L1, 0, 3, 0
	SFPSTORE	L0, 0, 3, 0
	ret
	.size	_Z9exexp_no2v, .-_Z9exexp_no2v
	.align	1
	.globl	_Z12exexp_no_bb1i
	.type	_Z12exexp_no_bb1i, @function
_Z12exexp_no_bb1i:
	SFPLOADI	L0, 0, 16448
	SFPEXEXP	L0, L1, 0
	bgt	a0,zero,.L72
	SFPSTORE	L0, 0, 3, 0
	SFPSTORE	L1, 0, 3, 0
	ret
.L72:
	li	a5,0
.L70:
	SFPSETCC	0, L1, 4
	SFPSTORE	L10, 0, 3, 0
	SFPENCC	3, 10
	addi	a5,a5,1
	bne	a0,a5,.L70
	SFPSTORE	L0, 0, 3, 0
	SFPSTORE	L1, 0, 3, 0
	ret
	.size	_Z12exexp_no_bb1i, .-_Z12exexp_no_bb1i
	.align	1
	.globl	_Z8mad_yes1v
	.type	_Z8mad_yes1v, @function
_Z8mad_yes1v:
	SFPLOAD	L0, 0, 3, 0
	SFPLOAD	L1, 0, 3, 2
	SFPLOAD	L2, 0, 3, 4
	SFPMAD	L0, L1, L2, L0, 0
	SFPWNOP
	SFPSTORE	L0, 0, 3, 0
	ret
	.size	_Z8mad_yes1v, .-_Z8mad_yes1v
	.align	1
	.globl	_Z8mad_yes2v
	.type	_Z8mad_yes2v, @function
_Z8mad_yes2v:
	SFPLOAD	L0, 0, 3, 0
	SFPLOAD	L1, 0, 3, 2
	SFPLOAD	L2, 0, 3, 4
	SFPMAD	L0, L1, L2, L3, 0
	SFPWNOP
	SFPSTORE	L3, 0, 3, 0
	SFPMAD	L0, L1, L2, L0, 0
	SFPWNOP
	SFPSTORE	L0, 0, 3, 0
	ret
	.size	_Z8mad_yes2v, .-_Z8mad_yes2v
	.align	1
	.globl	_Z8mad_yes3v
	.type	_Z8mad_yes3v, @function
_Z8mad_yes3v:
	SFPLOAD	L1, 0, 3, 0
	SFPLOAD	L0, 0, 3, 2
	SFPLOAD	L2, 0, 3, 4
	SFPMAD	L1, L0, L2, L0, 0
	SFPWNOP
	SFPMAD	L10, L0, L10, L0, 0
	SFPWNOP
	SFPMAD	L1, L0, L2, L0, 0
	SFPWNOP
	SFPSTORE	L0, 0, 3, 0
	ret
	.size	_Z8mad_yes3v, .-_Z8mad_yes3v
	.align	1
	.globl	_Z17mad_maybe_future1v
	.type	_Z17mad_maybe_future1v, @function
_Z17mad_maybe_future1v:
	SFPLOAD	L1, 0, 3, 0
	SFPLOAD	L3, 0, 3, 2
	SFPLOAD	L2, 0, 3, 4
	SFPLOADI	L0, 0, 16512
	SFPMUL	L1, L3, L9, L1, 0
	SFPMUL	L2, L0, L9, L0, 0
	SFPADD	L10, L1, L2, L1, 0
	SFPWNOP
	SFPSTORE	L1, 0, 3, 0
	SFPADD	L10, L0, L2, L0, 0
	SFPWNOP
	SFPSTORE	L0, 0, 3, 0
	ret
	.size	_Z17mad_maybe_future1v, .-_Z17mad_maybe_future1v
	.align	1
	.globl	_Z17mad_maybe_future2v
	.type	_Z17mad_maybe_future2v, @function
_Z17mad_maybe_future2v:
	SFPLOAD	L0, 0, 3, 0
	SFPLOAD	L1, 0, 3, 2
	SFPMUL	L0, L1, L9, L0, 0
	SFPLOAD	L1, 0, 3, 4
	SFPADD	L10, L0, L1, L1, 0
	SFPLOAD	L2, 0, 3, 6
	SFPADD	L10, L0, L2, L0, 0
	SFPSTORE	L1, 0, 3, 0
	SFPSTORE	L0, 0, 3, 0
	ret
	.size	_Z17mad_maybe_future2v, .-_Z17mad_maybe_future2v
	.align	1
	.globl	_Z13mad_live_yes1v
	.type	_Z13mad_live_yes1v, @function
_Z13mad_live_yes1v:
	SFPLOAD	L1, 0, 3, 0
	SFPLOAD	L2, 0, 3, 2
	SFPLOAD	L3, 0, 3, 4
	SFPLOAD	L0, 0, 3, 6
	SFPSETCC	0, L1, 6
	SFPMAD	L1, L2, L3, L0, 0
	SFPENCC	3, 10
	SFPSTORE	L0, 0, 3, 0
	ret
	.size	_Z13mad_live_yes1v, .-_Z13mad_live_yes1v
	.align	1
	.globl	_Z20mad_reg_pressure_no4v
	.type	_Z20mad_reg_pressure_no4v, @function
_Z20mad_reg_pressure_no4v:
	SFPLOAD	L1, 0, 3, 0
	SFPLOAD	L0, 0, 3, 2
	SFPLOAD	L3, 0, 3, 4
	SFPMUL	L1, L0, L9, L2, 0
	SFPEXEXP	L1, L1, 0
	SFPEXEXP	L0, L0, 0
	SFPADD	L10, L2, L3, L2, 0
	SFPSTORE	L3, 0, 3, 0
	SFPSTORE	L2, 0, 3, 0
	SFPSTORE	L1, 0, 3, 2
	SFPSTORE	L0, 0, 3, 2
	ret
	.size	_Z20mad_reg_pressure_no4v, .-_Z20mad_reg_pressure_no4v
	.align	1
	.globl	_Z7mad_no1v
	.type	_Z7mad_no1v, @function
_Z7mad_no1v:
	SFPLOAD	L0, 0, 3, 0
	SFPLOAD	L2, 0, 3, 2
	SFPLOAD	L1, 0, 3, 4
	SFPMUL	L0, L2, L9, L0, 0
	SFPWNOP
	SFPADD	L10, L1, L0, L1, 0
	SFPSTORE	L0, 0, 3, 0
	SFPSTORE	L1, 0, 3, 0
	ret
	.size	_Z7mad_no1v, .-_Z7mad_no1v
	.align	1
	.globl	_Z7mad_no2v
	.type	_Z7mad_no2v, @function
_Z7mad_no2v:
	SFPLOAD	L0, 0, 3, 0
	SFPLOAD	L2, 0, 3, 2
	SFPLOAD	L1, 0, 3, 4
	SFPMUL	L0, L2, L9, L0, 0
	SFPWNOP
	SFPSTORE	L0, 0, 3, 0
	SFPADD	L10, L1, L0, L0, 0
	SFPWNOP
	SFPSTORE	L0, 0, 3, 0
	ret
	.size	_Z7mad_no2v, .-_Z7mad_no2v
	.align	1
	.globl	_Z7mad_no3v
	.type	_Z7mad_no3v, @function
_Z7mad_no3v:
	SFPLOAD	L2, 0, 3, 0
	SFPLOAD	L1, 0, 3, 2
	SFPLOAD	L0, 0, 3, 4
	SFPMUL	L2, L1, L9, L1, 0
	SFPSETCC	0, L2, 4
	SFPENCC	3, 10
	SFPADD	L10, L0, L1, L0, 0
	SFPWNOP
	SFPSTORE	L0, 0, 3, 0
	ret
	.size	_Z7mad_no3v, .-_Z7mad_no3v
	.align	1
	.globl	_Z12mad_live_no1v
	.type	_Z12mad_live_no1v, @function
_Z12mad_live_no1v:
	SFPLOAD	L2, 0, 3, 0
	SFPLOAD	L1, 0, 3, 2
	SFPLOAD	L3, 0, 3, 4
	SFPLOAD	L0, 0, 3, 6
	SFPMUL	L2, L1, L9, L1, 0
	SFPSETCC	0, L2, 6
	SFPADD	L10, L1, L3, L0, 0
	SFPENCC	3, 10
	SFPSTORE	L0, 0, 3, 0
	ret
	.size	_Z12mad_live_no1v, .-_Z12mad_live_no1v
	.align	1
	.globl	_Z12mad_live_no2v
	.type	_Z12mad_live_no2v, @function
_Z12mad_live_no2v:
	SFPLOAD	L2, 0, 3, 0
	SFPLOAD	L0, 0, 3, 2
	SFPLOAD	L3, 0, 3, 4
	SFPLOAD	L1, 0, 3, 6
	SFPMUL	L2, L0, L9, L0, 0
	SFPSETCC	0, L2, 6
	SFPADD	L10, L0, L3, L1, 0
	SFPENCC	3, 10
	SFPSTORE	L1, 0, 3, 0
	SFPSTORE	L0, 0, 3, 0
	ret
	.size	_Z12mad_live_no2v, .-_Z12mad_live_no2v
	.align	1
	.globl	_Z10mad_no_bb1i
	.type	_Z10mad_no_bb1i, @function
_Z10mad_no_bb1i:
	SFPLOAD	L0, 0, 3, 0
	SFPLOAD	L2, 0, 3, 2
	SFPLOAD	L1, 0, 3, 4
	SFPMUL	L0, L2, L9, L0, 0
	SFPWNOP
	ble	a0,zero,.L86
	li	a5,0
.L87:
	SFPADD	L10, L0, L1, L0, 0
	SFPWNOP
	addi	a5,a5,1
	bne	a0,a5,.L87
.L86:
	SFPSTORE	L0, 0, 3, 0
	ret
	.size	_Z10mad_no_bb1i, .-_Z10mad_no_bb1i
	.align	1
	.globl	_Z9addi_yes1v
	.type	_Z9addi_yes1v, @function
_Z9addi_yes1v:
	SFPLOADI	L0, 0, 16384
	SFPADDI	16256, L0, 0
	SFPWNOP
	SFPSTORE	L0, 0, 3, 0
	ret
	.size	_Z9addi_yes1v, .-_Z9addi_yes1v
	.align	1
	.globl	_Z15addi_yes_caveatv
	.type	_Z15addi_yes_caveatv, @function
_Z15addi_yes_caveatv:
	SFPLOADI	L0, 0, 16256
	SFPLOADI	L1, 0, 16384
	SFPADD	L10, L0, L1, L1, 0
	SFPWNOP
	SFPSTORE	L1, 0, 3, 0
	SFPADDI	16384, L0, 0
	SFPWNOP
	SFPSTORE	L0, 0, 3, 2
	ret
	.size	_Z15addi_yes_caveatv, .-_Z15addi_yes_caveatv
	.align	1
	.globl	_Z9addi_yes2v
	.type	_Z9addi_yes2v, @function
_Z9addi_yes2v:
	SFPLOADI	L0, 0, 16256
	SFPADDI	16384, L0, 0
	SFPWNOP
	SFPADDI	16384, L0, 0
	SFPWNOP
	SFPADDI	16384, L0, 0
	SFPWNOP
	SFPSTORE	L0, 0, 3, 0
	ret
	.size	_Z9addi_yes2v, .-_Z9addi_yes2v
	.align	1
	.globl	_Z9addi_yes3v
	.type	_Z9addi_yes3v, @function
_Z9addi_yes3v:
	SFPLOAD	L0, 0, 3, 0
	SFPLOAD	L1, 0, 3, 2
	SFPLOADI	L2, 0, 16256
	SFPMAD	L0, L1, L2, L0, 0
	SFPWNOP
	SFPADDI	16384, L0, 0
	SFPWNOP
	SFPSTORE	L0, 0, 3, 0
	ret
	.size	_Z9addi_yes3v, .-_Z9addi_yes3v
	.align	1
	.globl	_Z9addi_yes4v
	.type	_Z9addi_yes4v, @function
_Z9addi_yes4v:
	SFPLOADI	L1, 0, 16256
	SFPLOADI	L0, 0, 16384
	SFPADDI	16256, L0, 0
	SFPWNOP
	SFPSTORE	L0, 0, 3, 0
	SFPSTORE	L1, 0, 3, 0
	SFPLOADI	L0, 0, 16256
	SFPLOADI	L1, 0, 16384
	SFPADDI	16384, L0, 0
	SFPWNOP
	SFPSTORE	L0, 0, 3, 0
	SFPSTORE	L1, 0, 3, 0
	ret
	.size	_Z9addi_yes4v, .-_Z9addi_yes4v
	.align	1
	.globl	_Z13addi_yes_imm1j
	.type	_Z13addi_yes_imm1j, @function
_Z13addi_yes_imm1j:
	lui	a5,%hi(_ZN7ckernel13instrn_bufferE)
	lw	a5,%lo(_ZN7ckernel13instrn_bufferE)(a5)
	slli	a0,a0,16
	li	a4, 1895825408	# 71000000
	srli	a0,a0,16
	add	a0,a0,a4
	sw	a0, 0(a5)	# Op(0x71) - d(lr0)
	SFPADDI	16256, L0, 0
	SFPWNOP
	SFPSTORE	L0, 0, 3, 0
	ret
	.size	_Z13addi_yes_imm1j, .-_Z13addi_yes_imm1j
	.align	1
	.globl	_Z13addi_yes_imm2j
	.type	_Z13addi_yes_imm2j, @function
_Z13addi_yes_imm2j:
	lui	a5,%hi(_ZN7ckernel13instrn_bufferE)
	lw	a4,%lo(_ZN7ckernel13instrn_bufferE)(a5)
	SFPLOADI	L0, 0, 16256
	slli	a5,a0,16
	srli	a5,a5,16
	li	a3, 1962934272	# 75000000
	slli	a5,a5,8
	add	a5,a5,a3
	sw	a5, 0(a4)	# Op(0x75) lv(lr0)  d(lr0)
	SFPWNOP
	SFPSTORE	L0, 0, 3, 0
	ret
	.size	_Z13addi_yes_imm2j, .-_Z13addi_yes_imm2j
	.align	1
	.globl	_Z14addi_live_yes1v
	.type	_Z14addi_live_yes1v, @function
_Z14addi_live_yes1v:
	SFPLOADI	L0, 0, 16384
	SFPLOAD	L1, 0, 3, 0
	SFPSETCC	0, L1, 6
	SFPADDI	16448, L0, 0
	SFPENCC	3, 10
	SFPSTORE	L0, 0, 3, 0
	ret
	.size	_Z14addi_live_yes1v, .-_Z14addi_live_yes1v
	.align	1
	.globl	_Z14addi_live_yes2v
	.type	_Z14addi_live_yes2v, @function
_Z14addi_live_yes2v:
	SFPLOADI	L0, 0, 16384
	SFPLOAD	L1, 0, 3, 0
	SFPSETCC	0, L1, 6
	SFPADDI	16448, L0, 0
	SFPENCC	3, 10
	SFPSTORE	L0, 0, 3, 0
	ret
	.size	_Z14addi_live_yes2v, .-_Z14addi_live_yes2v
	.align	1
	.globl	_Z21addi_live_yes3_futurev
	.type	_Z21addi_live_yes3_futurev, @function
_Z21addi_live_yes3_futurev:
	SFPLOADI	L0, 0, 16384
	SFPLOADI	L1, 0, 16448
	SFPLOAD	L2, 0, 3, 0
	SFPSETCC	0, L2, 6
	SFPADD	L10, L0, L1, L0, 0
	SFPENCC	3, 10
	SFPSTORE	L0, 0, 3, 0
	ret
	.size	_Z21addi_live_yes3_futurev, .-_Z21addi_live_yes3_futurev
	.align	1
	.globl	_Z21addi_live_yes4_futurev
	.type	_Z21addi_live_yes4_futurev, @function
_Z21addi_live_yes4_futurev:
	SFPLOADI	L1, 0, 16448
	SFPLOADI	L0, 0, 16384
	SFPLOAD	L2, 0, 3, 0
	SFPSETCC	0, L2, 6
	SFPLOADI	L1, 0, 16512
	SFPADD	L10, L0, L1, L0, 0
	SFPENCC	3, 10
	SFPSTORE	L0, 0, 3, 0
	ret
	.size	_Z21addi_live_yes4_futurev, .-_Z21addi_live_yes4_futurev
	.align	1
	.globl	_Z13addi_live_no2v
	.type	_Z13addi_live_no2v, @function
_Z13addi_live_no2v:
	SFPLOADI	L2, 0, 16384
	SFPLOADI	L0, 0, 16256
	SFPLOAD	L1, 0, 3, 0
	SFPSETCC	0, L1, 6
	SFPLOADI	L1, 0, 16448
	SFPADD	L10, L1, L2, L0, 0
	SFPENCC	3, 10
	SFPSTORE	L0, 0, 3, 0
	ret
	.size	_Z13addi_live_no2v, .-_Z13addi_live_no2v
	.align	1
	.globl	_Z13addi_live_no3v
	.type	_Z13addi_live_no3v, @function
_Z13addi_live_no3v:
	SFPLOADI	L1, 0, 16256
	SFPLOADI	L2, 0, 16384
	SFPLOADI	L0, 0, 16256
	SFPLOAD	L3, 0, 3, 0
	SFPSETCC	0, L3, 6
	SFPLOADI	L1, 0, 16448
	SFPADD	L10, L1, L2, L0, 0
	SFPENCC	3, 10
	SFPSTORE	L0, 0, 3, 0
	ret
	.size	_Z13addi_live_no3v, .-_Z13addi_live_no3v
	.align	1
	.globl	_Z13addi_live_no4v
	.type	_Z13addi_live_no4v, @function
_Z13addi_live_no4v:
	SFPLOADI	L1, 0, 16256
	SFPLOAD	L2, 0, 3, 0
	SFPLOADI	L0, 0, 16256
	SFPLOAD	L3, 0, 3, 0
	SFPSETCC	0, L3, 6
	SFPLOADI	L1, 0, 16448
	SFPADD	L10, L1, L2, L0, 0
	SFPENCC	3, 10
	SFPSTORE	L0, 0, 3, 0
	ret
	.size	_Z13addi_live_no4v, .-_Z13addi_live_no4v
	.align	1
	.globl	_Z8addi_no1v
	.type	_Z8addi_no1v, @function
_Z8addi_no1v:
	SFPLOADI	L0, 0, 16256
	SFPLOAD	L1, 0, 3, 0
	SFPLOAD	L2, 0, 3, 0
	SFPSETCC	0, L2, 6
	SFPLOADI	L0, 0, 16448
	SFPENCC	3, 10
	SFPADD	L10, L0, L1, L0, 0
	SFPWNOP
	SFPSTORE	L0, 0, 3, 0
	ret
	.size	_Z8addi_no1v, .-_Z8addi_no1v
	.align	1
	.globl	_Z8addi_no2v
	.type	_Z8addi_no2v, @function
_Z8addi_no2v:
	SFPLOADI	L1, 0, 16256
	SFPLOADI	L0, 0, 16384
	SFPADD	L10, L1, L0, L2, 0
	SFPSTORE	L1, 0, 3, 0
	SFPSTORE	L0, 0, 3, 0
	SFPSTORE	L2, 0, 3, 0
	ret
	.size	_Z8addi_no2v, .-_Z8addi_no2v
	.align	1
	.globl	_Z11addi_no_bb1i
	.type	_Z11addi_no_bb1i, @function
_Z11addi_no_bb1i:
	SFPLOADI	L1, 0, 16256
	SFPLOADI	L2, 0, 16384
	ble	a0,zero,.L107
	SFPADD	L10, L1, L2, L0, 0
	SFPWNOP
	li	a5,1
	beq	a0,a5,.L107
.L108:
	SFPADD	L10, L1, L2, L0, 0
	SFPWNOP
	addi	a5,a5,1
	bne	a0,a5,.L108
.L107:
	SFPSTORE	L0, 0, 3, 0
	ret
	.size	_Z11addi_no_bb1i, .-_Z11addi_no_bb1i
	.align	1
	.globl	_Z9muli_yes1v
	.type	_Z9muli_yes1v, @function
_Z9muli_yes1v:
	SFPLOADI	L0, 0, 16384
	SFPMULI	16256, L0, 0
	SFPWNOP
	SFPSTORE	L0, 0, 3, 0
	ret
	.size	_Z9muli_yes1v, .-_Z9muli_yes1v
	.align	1
	.globl	_Z15muli_yes_caveatv
	.type	_Z15muli_yes_caveatv, @function
_Z15muli_yes_caveatv:
	SFPLOADI	L0, 0, 16256
	SFPLOADI	L1, 0, 16384
	SFPMUL	L0, L1, L9, L1, 0
	SFPWNOP
	SFPSTORE	L1, 0, 3, 0
	SFPMULI	16384, L0, 0
	SFPWNOP
	SFPSTORE	L0, 0, 3, 2
	ret
	.size	_Z15muli_yes_caveatv, .-_Z15muli_yes_caveatv
	.align	1
	.globl	_Z18add_plus_half_yes1v
	.type	_Z18add_plus_half_yes1v, @function
_Z18add_plus_half_yes1v:
	ret
	.size	_Z18add_plus_half_yes1v, .-_Z18add_plus_half_yes1v
	.align	1
	.globl	_Z18add_plus_half_yes2v
	.type	_Z18add_plus_half_yes2v, @function
_Z18add_plus_half_yes2v:
	SFPLOAD	L0, 0, 3, 0
	SFPLOAD	L2, 0, 3, 2
	SFPLOAD	L1, 0, 3, 4
	SFPLOADI	L3, 0, 16128
	SFPMAD	L0, L2, L3, L0, 0
	SFPWNOP
	SFPADD	L10, L0, L1, L0, 0
	SFPWNOP
	SFPSTORE	L0, 0, 3, 2
	ret
	.size	_Z18add_plus_half_yes2v, .-_Z18add_plus_half_yes2v
	.align	1
	.globl	_Z18add_plus_half_yes3v
	.type	_Z18add_plus_half_yes3v, @function
_Z18add_plus_half_yes3v:
	SFPLOAD	L0, 0, 3, 0
	SFPLOAD	L3, 0, 3, 2
	SFPLOAD	L2, 0, 3, 4
	SFPLOADI	L1, 0, 16128
	SFPMAD	L0, L3, L1, L0, 0
	SFPWNOP
	SFPADD	L10, L0, L2, L0, 0
	SFPWNOP
	SFPSTORE	L0, 0, 3, 2
	SFPSTORE	L1, 0, 3, 4
	ret
	.size	_Z18add_plus_half_yes3v, .-_Z18add_plus_half_yes3v
	.align	1
	.globl	_Z30add_plus_half_live_yes1_futurev
	.type	_Z30add_plus_half_live_yes1_futurev, @function
_Z30add_plus_half_live_yes1_futurev:
	SFPLOAD	L0, 0, 3, 0
	SFPLOAD	L1, 0, 3, 2
	SFPSETCC	0, L0, 0
	SFPADD	L10, L0, L1, L1, 0
	SFPLOADI	L2, 0, 16128
	SFPADD	L10, L1, L2, L0, 0
	SFPENCC	3, 10
	SFPSTORE	L0, 0, 3, 4
	ret
	.size	_Z30add_plus_half_live_yes1_futurev, .-_Z30add_plus_half_live_yes1_futurev
	.align	1
	.globl	_Z30add_plus_half_live_yes2_futurev
	.type	_Z30add_plus_half_live_yes2_futurev, @function
_Z30add_plus_half_live_yes2_futurev:
	SFPLOAD	L0, 0, 3, 0
	SFPLOAD	L1, 0, 3, 2
	SFPSETCC	0, L0, 0
	SFPADD	L10, L0, L1, L0, 0
	SFPWNOP
	SFPADDI	16128, L0, 0
	SFPENCC	3, 10
	SFPSTORE	L0, 0, 3, 4
	ret
	.size	_Z30add_plus_half_live_yes2_futurev, .-_Z30add_plus_half_live_yes2_futurev
	.align	1
	.globl	_Z30add_plus_half_live_yes3_futurev
	.type	_Z30add_plus_half_live_yes3_futurev, @function
_Z30add_plus_half_live_yes3_futurev:
	SFPLOAD	L0, 0, 3, 0
	SFPLOAD	L2, 0, 3, 2
	SFPLOAD	L1, 0, 3, 4
	SFPSETCC	0, L0, 0
	SFPADD	L10, L0, L2, L1, 0
	SFPLOADI	L2, 0, 16128
	SFPADD	L10, L0, L2, L1, 0
	SFPENCC	3, 10
	SFPSTORE	L0, 0, 3, 6
	ret
	.size	_Z30add_plus_half_live_yes3_futurev, .-_Z30add_plus_half_live_yes3_futurev
	.align	1
	.globl	_Z22add_plus_half_live_no1v
	.type	_Z22add_plus_half_live_no1v, @function
_Z22add_plus_half_live_no1v:
	SFPLOAD	L0, 0, 3, 0
	SFPLOAD	L1, 0, 3, 2
	SFPADD	L10, L0, L1, L0, 0
	SFPWNOP
	SFPSETCC	0, L0, 0
	SFPADDI	16128, L0, 0
	SFPENCC	3, 10
	SFPSTORE	L0, 0, 3, 4
	ret
	.size	_Z22add_plus_half_live_no1v, .-_Z22add_plus_half_live_no1v
	.align	1
	.globl	_Z22add_plus_half_live_no2v
	.type	_Z22add_plus_half_live_no2v, @function
_Z22add_plus_half_live_no2v:
	SFPLOAD	L0, 0, 3, 0
	SFPLOAD	L1, 0, 3, 2
	SFPSETCC	0, L0, 0
	SFPADD	L10, L0, L1, L0, 0
	SFPENCC	3, 10
	SFPADDI	16128, L0, 0
	SFPWNOP
	SFPSTORE	L0, 0, 3, 4
	ret
	.size	_Z22add_plus_half_live_no2v, .-_Z22add_plus_half_live_no2v
	.align	1
	.globl	_Z17add_plus_half_no1j
	.type	_Z17add_plus_half_no1j, @function
_Z17add_plus_half_no1j:
	SFPLOAD	L0, 0, 3, 0
	SFPLOAD	L1, 0, 3, 2
	SFPLOAD	L2, 0, 3, 4
	SFPMAD	L0, L1, L2, L0, 0
	SFPWNOP
	slli	a5,a0,16
	lui	a4,%hi(_ZN7ckernel13instrn_bufferE)
	lw	a4,%lo(_ZN7ckernel13instrn_bufferE)(a4)
	srli	a5,a5,16
	li	a3, 1962934272	# 75000000
	slli	a5,a5,8
	add	a5,a5,a3
	sw	a5, 0(a4)	# Op(0x75) lv(lr0)  d(lr0)
	SFPWNOP
	SFPSTORE	L0, 0, 3, 0
	ret
	.size	_Z17add_plus_half_no1j, .-_Z17add_plus_half_no1j
	.align	1
	.globl	_Z17add_plus_half_no2v
	.type	_Z17add_plus_half_no2v, @function
_Z17add_plus_half_no2v:
	SFPLOAD	L0, 0, 3, 0
	SFPLOAD	L2, 0, 3, 2
	SFPLOAD	L1, 0, 3, 4
	SFPLOADI	L3, 0, 16128
	SFPMAD	L0, L2, L3, L0, 0
	SFPWNOP
	SFPADD	L10, L0, L1, L0, 0
	SFPWNOP
	SFPADDI	16128, L0, 0
	SFPWNOP
	SFPSTORE	L0, 0, 3, 2
	ret
	.size	_Z17add_plus_half_no2v, .-_Z17add_plus_half_no2v
	.align	1
	.globl	_Z20add_plus_half_no_bb1i
	.type	_Z20add_plus_half_no_bb1i, @function
_Z20add_plus_half_no_bb1i:
	SFPLOAD	L0, 0, 3, 0
	SFPLOAD	L2, 0, 3, 2
	SFPLOADI	L1, 0, 16128
	SFPADD	L10, L0, L2, L0, 0
	SFPWNOP
	ble	a0,zero,.L127
	li	a5,0
.L128:
	SFPADD	L10, L0, L1, L0, 0
	SFPWNOP
	addi	a5,a5,1
	bne	a0,a5,.L128
.L127:
	SFPSTORE	L0, 0, 3, 0
	ret
	.size	_Z20add_plus_half_no_bb1i, .-_Z20add_plus_half_no_bb1i
	.align	1
	.globl	_Z10complexishv
	.type	_Z10complexishv, @function
_Z10complexishv:
	SFPLOAD	L0, 0, 3, 0
	SFPLOAD	L2, 0, 3, 2
	SFPLOADI	L1, 0, 16736
	SFPLOADI	L3, 0, 16448
	SFPMAD	L0, L2, L3, L0, 0
	SFPWNOP
	SFPADDI	16128, L0, 0
	SFPLOAD	L2, 0, 3, 4
	SFPLOAD	L3, 0, 3, 6
	SFPMAD	L0, L2, L3, L0, 0
	SFPLOADI	L2, 0, 16640
	SFPLOADI	L3, 0, 16128
	SFPMAD	L0, L2, L3, L0, 0
	SFPWNOP
	SFPADDI	16672, L0, 0
	SFPLOADI	L2, 0, 16704
	SFPLOADI	L3, 0, 48896
	SFPMAD	L0, L2, L3, L0, 0
	SFPWNOP
	SFPMULI	16736, L0, 0
	SFPWNOP
	SFPMULI	0, L0, 0
	SFPWNOP
	SFPSTORE	L0, 0, 3, 8
	SFPLOAD	L0, 0, 3, 10
	SFPLOAD	L2, 0, 3, 12
	SFPLOADI	L3, 0, 49216
	SFPMAD	L0, L2, L3, L0, 0
	SFPWNOP
	SFPADDI	16128, L0, 0
	SFPLOAD	L2, 0, 3, 14
	SFPMOV	L2, L2, 1
	SFPMUL	L0, L2, L9, L2, 0
	SFPLOAD	L0, 0, 3, 16
	SFPMAD	L11, L0, L2, L0, 0
	SFPLOADI	L2, 0, 16640
	SFPLOADI	L3, 0, 48896
	SFPMAD	L0, L2, L3, L0, 0
	SFPWNOP
	SFPADDI	49440, L0, 0
	SFPLOADI	L2, 0, 16704
	SFPLOADI	L3, 0, 48896
	SFPMAD	L0, L2, L3, L0, 0
	SFPMOV	L1, L1, 1
	SFPMUL	L0, L1, L9, L1, 0
	SFPWNOP
	SFPMULI	0, L1, 0
	SFPWNOP
	SFPSTORE	L1, 0, 3, 18
	ret
	.size	_Z10complexishv, .-_Z10complexishv
	.globl	_ZN7ckernel13instrn_bufferE
	.section	.sbss,"aw",@nobits
	.align	2
	.type	_ZN7ckernel13instrn_bufferE, @object
	.size	_ZN7ckernel13instrn_bufferE, 4
_ZN7ckernel13instrn_bufferE:
	.zero	4
	.ident	"GCC: (GNU) 10.2.0"
