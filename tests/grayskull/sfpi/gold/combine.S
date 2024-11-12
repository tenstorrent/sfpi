	.file	"combine.cc"
	.option nopic
	.attribute arch, "rv32i2p0_y"
	.attribute unaligned_access, 0
	.attribute stack_align, 16
	.text
	.align	2
	.globl	_Z11iadd_i_yes1v
	.type	_Z11iadd_i_yes1v, @function
_Z11iadd_i_yes1v:
	SFPLOADI	L0, 3, 4
	SFPIADD	L0, L0, -5, 1
	TTNOP
	TTNOP
	TTNOP
	SFPSTORE	0, L10, 0
	SFPENCC	3, 10
	TTNOP
	ret
	.size	_Z11iadd_i_yes1v, .-_Z11iadd_i_yes1v
	.align	2
	.globl	_Z16iadd_i_yes1_livev
	.type	_Z16iadd_i_yes1_livev, @function
_Z16iadd_i_yes1_livev:
	SFPLOADI	L0, 3, 4
	SFPSETCC	L0, 0, 0
	TTNOP
	SFPIADD	L0, L0, -5, 1
	TTNOP
	TTNOP
	TTNOP
	SFPSTORE	0, L10, 0
	SFPENCC	3, 10
	TTNOP
	ret
	.size	_Z16iadd_i_yes1_livev, .-_Z16iadd_i_yes1_livev
	.align	2
	.globl	_Z11iadd_i_yes2v
	.type	_Z11iadd_i_yes2v, @function
_Z11iadd_i_yes2v:
	SFPLOADI	L0, 3, 4
	SFPIADD	L0, L0, -7, 1
	TTNOP
	TTNOP
	TTNOP
	SFPSTORE	0, L10, 0
	SFPENCC	3, 10
	TTNOP
	ret
	.size	_Z11iadd_i_yes2v, .-_Z11iadd_i_yes2v
	.align	2
	.globl	_Z12iadd_i_yes3av
	.type	_Z12iadd_i_yes3av, @function
_Z12iadd_i_yes3av:
	SFPLOADI	L0, 3, 4
	SFPLOADI	L1, 4, 4
	SFPIADD	L0, L0, -5, 1
	TTNOP
	TTNOP
	TTNOP
	SFPIADD	L2, L1, -6, 5
	TTNOP
	TTNOP
	SFPSETCC	L2, 0, 6
	TTNOP
	SFPSTORE	0, L10, 0
	SFPENCC	3, 10
	TTNOP
	SFPSTORE	0, L0, 2
	SFPSTORE	0, L1, 2
	ret
	.size	_Z12iadd_i_yes3av, .-_Z12iadd_i_yes3av
	.align	2
	.globl	_Z12iadd_i_yes3bv
	.type	_Z12iadd_i_yes3bv, @function
_Z12iadd_i_yes3bv:
	SFPLOADI	L0, 3, 4
	SFPLOADI	L1, 4, 4
	SFPIADD	L1, L1, -6, 5
	TTNOP
	TTNOP
	SFPSETCC	L1, 0, 6
	TTNOP
	SFPIADD	L0, L0, -5, 1
	TTNOP
	TTNOP
	TTNOP
	SFPSTORE	0, L10, 0
	SFPENCC	3, 10
	TTNOP
	ret
	.size	_Z12iadd_i_yes3bv, .-_Z12iadd_i_yes3bv
	.align	2
	.globl	_Z12iadd_i_yes3cv
	.type	_Z12iadd_i_yes3cv, @function
_Z12iadd_i_yes3cv:
	SFPLOADI	L0, 3, 4
	SFPLOADI	L1, 4, 4
	SFPIADD	L1, L1, -6, 5
	TTNOP
	TTNOP
	SFPSETCC	L1, 0, 6
	TTNOP
	SFPIADD	L0, L0, -5, 1
	TTNOP
	TTNOP
	TTNOP
	SFPSTORE	0, L10, 0
	SFPENCC	3, 10
	TTNOP
	ret
	.size	_Z12iadd_i_yes3cv, .-_Z12iadd_i_yes3cv
	.align	2
	.globl	_Z12iadd_i_yes3dv
	.type	_Z12iadd_i_yes3dv, @function
_Z12iadd_i_yes3dv:
	SFPLOADI	L0, 3, 4
	SFPIADD	L0, L0, -5, 5
	TTNOP
	TTNOP
	SFPIADD	L1, L0, 0, 5
	TTNOP
	TTNOP
	SFPSETCC	L0, 0, 0
	TTNOP
	SFPSTORE	0, L10, 0
	SFPENCC	3, 10
	TTNOP
	ret
	.size	_Z12iadd_i_yes3dv, .-_Z12iadd_i_yes3dv
	.align	2
	.globl	_Z12iadd_i_yes4av
	.type	_Z12iadd_i_yes4av, @function
_Z12iadd_i_yes4av:
	SFPLOADI	L1, 3, 4
	SFPLOADI	L0, 4, 4
	SFPIADD	L1, L1, -5, 1
	TTNOP
	TTNOP
	TTNOP
	SFPIADD	L0, L0, -6, 5
	TTNOP
	TTNOP
	SFPSETCC	L0, 0, 6
	TTNOP
	SFPSTORE	0, L10, 0
	SFPENCC	3, 10
	TTNOP
	ret
	.size	_Z12iadd_i_yes4av, .-_Z12iadd_i_yes4av
	.align	2
	.globl	_Z11iadd_i_yes5v
	.type	_Z11iadd_i_yes5v, @function
_Z11iadd_i_yes5v:
	SFPLOADI	L0, 3, 4
	SFPIADD	L0, L0, 5, 9
	TTNOP
	TTNOP
	TTNOP
	SFPSTORE	0, L10, 0
	SFPENCC	3, 10
	TTNOP
	ret
	.size	_Z11iadd_i_yes5v, .-_Z11iadd_i_yes5v
	.align	2
	.globl	_Z11iadd_i_yes6v
	.type	_Z11iadd_i_yes6v, @function
_Z11iadd_i_yes6v:
	SFPLOADI	L0, 3, 4
	SFPLOADI	L1, 4, 4
	SFPIADD	L2, L0, -5, 9
	TTNOP
	TTNOP
	TTNOP
	SFPIADD	L0, L1, 6, 9
	TTNOP
	TTNOP
	TTNOP
	SFPSTORE	0, L10, 0
	SFPENCC	3, 10
	TTNOP
	ret
	.size	_Z11iadd_i_yes6v, .-_Z11iadd_i_yes6v
	.align	2
	.globl	_Z11iadd_i_yes7v
	.type	_Z11iadd_i_yes7v, @function
_Z11iadd_i_yes7v:
	SFPLOADI	L0, 4, 4
	SFPIADD	L1, L0, 6, 9
	TTNOP
	TTNOP
	TTNOP
	SFPSTORE	0, L10, 0
	SFPENCC	3, 10
	TTNOP
	SFPSTORE	0, L1, 2
	SFPSTORE	0, L0, 2
	ret
	.size	_Z11iadd_i_yes7v, .-_Z11iadd_i_yes7v
	.align	2
	.globl	_Z11iadd_i_yes8v
	.type	_Z11iadd_i_yes8v, @function
_Z11iadd_i_yes8v:
	SFPLOADI	L0, 3, 4
	SFPIADD	L1, L0, 5, 1
	TTNOP
	TTNOP
	TTNOP
	SFPSTORE	0, L10, 0
	SFPENCC	3, 10
	TTNOP
	SFPSTORE	4, L0, 2
	SFPSTORE	8, L1, 2
	ret
	.size	_Z11iadd_i_yes8v, .-_Z11iadd_i_yes8v
	.align	2
	.globl	_Z11iadd_i_yes9v
	.type	_Z11iadd_i_yes9v, @function
_Z11iadd_i_yes9v:
	SFPLOADI	L1, 3, 4
	SFPLOADI	L0, 5, 4
	SFPIADD	L1, L1, -7, 1
	TTNOP
	TTNOP
	TTNOP
	SFPIADD	L0, L0, -9, 1
	TTNOP
	TTNOP
	TTNOP
	SFPSTORE	0, L10, 0
	SFPENCC	3, 10
	TTNOP
	ret
	.size	_Z11iadd_i_yes9v, .-_Z11iadd_i_yes9v
	.align	2
	.globl	_Z10iadd_i_no1v
	.type	_Z10iadd_i_no1v, @function
_Z10iadd_i_no1v:
	SFPLOADI	L1, 3, 4
	SFPLOADI	L0, 4, 4
	SFPIADD	L0, L0, -6, 5
	TTNOP
	TTNOP
	SFPSETCC	L1, 0, 0
	TTNOP
	SFPSTORE	0, L10, 0
	SFPENCC	3, 10
	TTNOP
	ret
	.size	_Z10iadd_i_no1v, .-_Z10iadd_i_no1v
	.align	2
	.globl	_Z10iadd_i_no2v
	.type	_Z10iadd_i_no2v, @function
_Z10iadd_i_no2v:
	SFPLOADI	L0, 3, 4
	SFPIADD	L1, L0, -5, 5
	TTNOP
	TTNOP
	SFPSETCC	L0, 0, 0
	TTNOP
	SFPSTORE	0, L10, 0
	SFPENCC	3, 10
	TTNOP
	ret
	.size	_Z10iadd_i_no2v, .-_Z10iadd_i_no2v
	.align	2
	.globl	_Z10iadd_i_no3v
	.type	_Z10iadd_i_no3v, @function
_Z10iadd_i_no3v:
	SFPLOADI	L0, 3, 4
	SFPIADD	L0, L0, -5, 5
	TTNOP
	TTNOP
	SFPIADD	L0, L0, -7, 1
	TTNOP
	TTNOP
	TTNOP
	SFPSTORE	0, L10, 0
	SFPENCC	3, 10
	TTNOP
	SFPSTORE	0, L0, 2
	ret
	.size	_Z10iadd_i_no3v, .-_Z10iadd_i_no3v
	.align	2
	.globl	_Z10iadd_i_no4v
	.type	_Z10iadd_i_no4v, @function
_Z10iadd_i_no4v:
	SFPLOADI	L1, 3, 4
	SFPLOADI	L0, 4, 4
	SFPIADD	L1, L1, -5, 5
	TTNOP
	TTNOP
	SFPIADD	L1, L1, -7, 5
	TTNOP
	TTNOP
	SFPIADD	L0, L0, -6, 5
	TTNOP
	TTNOP
	SFPIADD	L0, L0, -9, 5
	TTNOP
	TTNOP
	SFPSTORE	0, L0, 2
	ret
	.size	_Z10iadd_i_no4v, .-_Z10iadd_i_no4v
	.align	2
	.globl	_Z11iadd_i_no4bv
	.type	_Z11iadd_i_no4bv, @function
_Z11iadd_i_no4bv:
	SFPLOADI	L0, 3, 4
	SFPLOADI	L1, 4, 4
	SFPIADD	L0, L0, -5, 5
	TTNOP
	TTNOP
	SFPIADD	L1, L1, -6, 9
	TTNOP
	TTNOP
	TTNOP
	SFPSETCC	L0, 0, 0
	TTNOP
	SFPSTORE	0, L10, 0
	SFPENCC	3, 10
	TTNOP
	ret
	.size	_Z11iadd_i_no4bv, .-_Z11iadd_i_no4bv
	.align	2
	.globl	_Z10iadd_i_no5v
	.type	_Z10iadd_i_no5v, @function
_Z10iadd_i_no5v:
	SFPLOADI	L1, 3, 4
	SFPLOADI	L0, 4, 4
	SFPSETCC	L1, 0, 0
	TTNOP
	SFPSETCC	L0, 0, 0
	TTNOP
	SFPSTORE	0, L10, 0
	SFPENCC	3, 10
	TTNOP
	ret
	.size	_Z10iadd_i_no5v, .-_Z10iadd_i_no5v
	.align	2
	.globl	_Z10iadd_i_no6v
	.type	_Z10iadd_i_no6v, @function
_Z10iadd_i_no6v:
	SFPLOADI	L0, 3, 4
	SFPIADD	L0, L0, 5, 5
	TTNOP
	TTNOP
	SFPIADD	L0, L0, 0, 5
	TTNOP
	TTNOP
	ret
	.size	_Z10iadd_i_no6v, .-_Z10iadd_i_no6v
	.align	2
	.globl	_Z10iadd_i_no7v
	.type	_Z10iadd_i_no7v, @function
_Z10iadd_i_no7v:
	SFPLOADI	L0, 3, 4
	SFPIADD	L0, L0, -5, 5
	TTNOP
	TTNOP
	SFPIADD	L1, L0, 7, 5
	TTNOP
	TTNOP
	SFPSETCC	L0, 0, 0
	TTNOP
	SFPSTORE	0, L10, 0
	SFPENCC	3, 10
	TTNOP
	ret
	.size	_Z10iadd_i_no7v, .-_Z10iadd_i_no7v
	.align	2
	.globl	_Z10iadd_i_no8v
	.type	_Z10iadd_i_no8v, @function
_Z10iadd_i_no8v:
	SFPLOADI	L0, 3, 4
	SFPLOADI	L1, 5, 4
	SFPIADD	L0, L0, -7, 5
	TTNOP
	TTNOP
	SFPIADD	L1, L1, -9, 1
	TTNOP
	TTNOP
	TTNOP
	SFPSETCC	L0, 0, 0
	TTNOP
	SFPSTORE	0, L10, 0
	SFPENCC	3, 10
	TTNOP
	ret
	.size	_Z10iadd_i_no8v, .-_Z10iadd_i_no8v
	.align	2
	.globl	_Z10iadd_i_no9v
	.type	_Z10iadd_i_no9v, @function
_Z10iadd_i_no9v:
	SFPLOADI	L0, 3, 4
	SFPIADD	L0, L0, -3, 5
	TTNOP
	TTNOP
	SFPIADD	L1, L0, 0, 5
	TTNOP
	TTNOP
	SFPSETCC	L0, 0, 0
	TTNOP
	SFPSTORE	0, L10, 0
	SFPENCC	3, 10
	TTNOP
	SFPSTORE	0, L1, 2
	ret
	.size	_Z10iadd_i_no9v, .-_Z10iadd_i_no9v
	.align	2
	.globl	_Z11iadd_i_no10v
	.type	_Z11iadd_i_no10v, @function
_Z11iadd_i_no10v:
	addi	sp,sp,-16
	sw	ra,12(sp)
	SFPLOADI	L0, 3, 4
	call	rand
	SFPIADD	L0, L0, -5, 5
	TTNOP
	TTNOP
	bne	a0,zero,.L26
	SFPSETCC	L0, 0, 0
	TTNOP
	SFPSTORE	0, L10, 0
	SFPENCC	3, 10
	TTNOP
.L25:
	lw	ra,12(sp)
	addi	sp,sp,16
	jr	ra
.L26:
	li	a5,1
	bne	a0,a5,.L25
	SFPSETCC	L0, 0, 0
	TTNOP
	SFPSTORE	0, L9, 0
	SFPENCC	3, 10
	TTNOP
	lw	ra,12(sp)
	addi	sp,sp,16
	jr	ra
	.size	_Z11iadd_i_no10v, .-_Z11iadd_i_no10v
	.align	2
	.globl	_Z11iadd_i_no11v
	.type	_Z11iadd_i_no11v, @function
_Z11iadd_i_no11v:
	SFPLOADI	L0, 3, 4
	SFPIADD	L0, L0, -5, 1
	TTNOP
	TTNOP
	TTNOP
	SFPIADD	L0, L0, 2, 5
	TTNOP
	TTNOP
	SFPCOMPC
	TTNOP
	SFPSETCC	L0, 0, 0
	TTNOP
	SFPSTORE	0, L10, 0
	SFPENCC	3, 10
	TTNOP
	ret
	.size	_Z11iadd_i_no11v, .-_Z11iadd_i_no11v
	.align	2
	.globl	_Z11iadd_i_no12j
	.type	_Z11iadd_i_no12j, @function
_Z11iadd_i_no12j:
	SFPLOADI	L0, 3, 4
	SFPIADD	L0, L0, -5, 5
	TTNOP
	TTNOP
	slli	a5,a0,16
	lui	a4,%hi(_ZN7ckernel13instrn_bufferE)
	li	a3, 1897136128	# 71140000
	lw	a4,%lo(_ZN7ckernel13instrn_bufferE)(a4)
	srli	a5,a5,16
	add	a5,a5,a3
	sw	a5, 0(a4)	# Op(0x71) - d(lr1)
	SFPIADD	L1, L0, 0, 2
	TTNOP
	TTNOP
	TTNOP
	SFPIADD	L0, L0, 2, 5
	TTNOP
	TTNOP
	SFPENCC	3, 10
	TTNOP
	SFPSTORE	0, L0, 2
	ret
	.size	_Z11iadd_i_no12j, .-_Z11iadd_i_no12j
	.align	2
	.globl	_Z13iadd_i_no_bb1i
	.type	_Z13iadd_i_no_bb1i, @function
_Z13iadd_i_no_bb1i:
	SFPLOADI	L0, 1, 4
	SFPIADD	L0, L0, -5, 5
	TTNOP
	TTNOP
	ble	a0,zero,.L32
	li	a5,0
.L33:
	SFPSETCC	L0, 0, 0
	TTNOP
	SFPIADD	L0, L0, 1, 5
	TTNOP
	TTNOP
	SFPENCC	3, 10
	TTNOP
	addi	a5,a5,1
	bne	a0,a5,.L33
.L32:
	SFPSTORE	0, L0, 2
	ret
	.size	_Z13iadd_i_no_bb1i, .-_Z13iadd_i_no_bb1i
	.align	2
	.globl	_Z11iadd_v_yes1v
	.type	_Z11iadd_v_yes1v, @function
_Z11iadd_v_yes1v:
	SFPLOADI	L0, 3, 4
	SFPLOADI	L1, 4, 4
	SFPIADD	L0, L1, 0, 2
	TTNOP
	TTNOP
	TTNOP
	SFPSTORE	0, L10, 0
	SFPENCC	3, 10
	TTNOP
	ret
	.size	_Z11iadd_v_yes1v, .-_Z11iadd_v_yes1v
	.align	2
	.globl	_Z11iadd_v_yes2v
	.type	_Z11iadd_v_yes2v, @function
_Z11iadd_v_yes2v:
	SFPLOADI	L1, 3, 4
	SFPLOADI	L0, 4, 4
	SFPIADD	L0, L1, 0, 10
	TTNOP
	TTNOP
	TTNOP
	SFPSTORE	0, L10, 0
	SFPENCC	3, 10
	TTNOP
	ret
	.size	_Z11iadd_v_yes2v, .-_Z11iadd_v_yes2v
	.align	2
	.globl	_Z11iadd_v_yes3v
	.type	_Z11iadd_v_yes3v, @function
_Z11iadd_v_yes3v:
	SFPLOADI	L1, 3, 4
	SFPLOADI	L0, 4, 4
	SFPIADD	L0, L1, 0, 0
	TTNOP
	TTNOP
	TTNOP
	SFPSTORE	0, L10, 0
	SFPENCC	3, 10
	TTNOP
	ret
	.size	_Z11iadd_v_yes3v, .-_Z11iadd_v_yes3v
	.align	2
	.globl	_Z12iadd_v_yes4av
	.type	_Z12iadd_v_yes4av, @function
_Z12iadd_v_yes4av:
	SFPLOADI	L1, 3, 4
	SFPLOADI	L0, 4, 4
	SFPIADD	L0, L1, 0, 10
	TTNOP
	TTNOP
	TTNOP
	SFPSTORE	0, L10, 0
	SFPENCC	3, 10
	TTNOP
	ret
	.size	_Z12iadd_v_yes4av, .-_Z12iadd_v_yes4av
	.align	2
	.globl	_Z12iadd_v_yes4bv
	.type	_Z12iadd_v_yes4bv, @function
_Z12iadd_v_yes4bv:
	SFPLOADI	L1, 3, 4
	SFPLOADI	L0, 4, 4
	SFPIADD	L1, L0, 0, 0
	TTNOP
	TTNOP
	TTNOP
	SFPSTORE	0, L10, 0
	SFPENCC	3, 10
	TTNOP
	SFPSTORE	0, L0, 2
	ret
	.size	_Z12iadd_v_yes4bv, .-_Z12iadd_v_yes4bv
	.align	2
	.globl	_Z10iadd_v_no1v
	.type	_Z10iadd_v_no1v, @function
_Z10iadd_v_no1v:
	SFPLOADI	L0, 3, 4
	SFPLOADI	L1, 4, 4
	SFPIADD	L0, L1, 0, 4
	TTNOP
	TTNOP
	SFPIADD	L0, L0, -5, 1
	TTNOP
	TTNOP
	TTNOP
	SFPSTORE	0, L10, 0
	SFPENCC	3, 10
	TTNOP
	ret
	.size	_Z10iadd_v_no1v, .-_Z10iadd_v_no1v
	.align	2
	.globl	_Z10iadd_v_no2v
	.type	_Z10iadd_v_no2v, @function
_Z10iadd_v_no2v:
	SFPLOADI	L1, 3, 4
	SFPLOADI	L0, 4, 4
	SFPIADD	L0, L1, 0, 6
	TTNOP
	TTNOP
	SFPIADD	L0, L0, -5, 1
	TTNOP
	TTNOP
	TTNOP
	SFPSTORE	0, L10, 0
	SFPENCC	3, 10
	TTNOP
	ret
	.size	_Z10iadd_v_no2v, .-_Z10iadd_v_no2v
	.align	2
	.globl	_Z10iadd_v_no3v
	.type	_Z10iadd_v_no3v, @function
_Z10iadd_v_no3v:
	SFPLOADI	L1, 3, 4
	SFPLOADI	L0, 4, 4
	SFPIADD	L0, L1, 0, 6
	TTNOP
	TTNOP
	SFPIADD	L0, L0, 0, 5
	TTNOP
	TTNOP
	ret
	.size	_Z10iadd_v_no3v, .-_Z10iadd_v_no3v
	.align	2
	.globl	_Z10iadd_v_no4v
	.type	_Z10iadd_v_no4v, @function
_Z10iadd_v_no4v:
	SFPLOADI	L0, 3, 4
	SFPLOADI	L1, 4, 4
	SFPIADD	L1, L0, 0, 2
	TTNOP
	TTNOP
	TTNOP
	SFPIADD	L0, L0, -5, 5
	TTNOP
	TTNOP
	SFPSTORE	0, L10, 0
	SFPENCC	3, 10
	TTNOP
	ret
	.size	_Z10iadd_v_no4v, .-_Z10iadd_v_no4v
	.align	2
	.globl	_Z13iadd_v_no_bb1i
	.type	_Z13iadd_v_no_bb1i, @function
_Z13iadd_v_no_bb1i:
	SFPLOADI	L0, 1, 4
	SFPLOADI	L1, 2, 4
	SFPIADD	L0, L1, 0, 6
	TTNOP
	TTNOP
	ble	a0,zero,.L45
	li	a5,0
.L46:
	SFPSETCC	L0, 0, 0
	TTNOP
	SFPIADD	L0, L0, 1, 5
	TTNOP
	TTNOP
	SFPENCC	3, 10
	TTNOP
	addi	a5,a5,1
	bne	a0,a5,.L46
.L45:
	SFPSTORE	0, L0, 2
	ret
	.size	_Z13iadd_v_no_bb1i, .-_Z13iadd_v_no_bb1i
	.align	2
	.globl	_Z7lz_yes1v
	.type	_Z7lz_yes1v, @function
_Z7lz_yes1v:
	SFPLOADI	L0, 16448, 0
	SFPLZ	L1, L0, 10
	TTNOP
	TTNOP
	TTNOP
	SFPSTORE	0, L10, 0
	SFPENCC	3, 10
	TTNOP
	SFPSTORE	0, L0, 0
	SFPSTORE	0, L1, 2
	ret
	.size	_Z7lz_yes1v, .-_Z7lz_yes1v
	.align	2
	.globl	_Z7lz_yes2v
	.type	_Z7lz_yes2v, @function
_Z7lz_yes2v:
	SFPLOADI	L0, 16448, 0
	SFPLZ	L1, L0, 2
	TTNOP
	TTNOP
	TTNOP
	SFPSTORE	0, L10, 0
	SFPENCC	3, 10
	TTNOP
	SFPSTORE	0, L0, 0
	SFPSTORE	0, L1, 2
	ret
	.size	_Z7lz_yes2v, .-_Z7lz_yes2v
	.align	2
	.globl	_Z6lz_no1v
	.type	_Z6lz_no1v, @function
_Z6lz_no1v:
	SFPLOADI	L1, 16448, 0
	SFPLZ	L0, L1, 0
	TTNOP
	TTNOP
	SFPSETCC	L0, 0, 2
	TTNOP
	SFPSTORE	0, L10, 0
	SFPENCC	3, 10
	TTNOP
	SFPSTORE	0, L1, 0
	SFPSTORE	0, L0, 2
	ret
	.size	_Z6lz_no1v, .-_Z6lz_no1v
	.align	2
	.globl	_Z6lz_no2v
	.type	_Z6lz_no2v, @function
_Z6lz_no2v:
	SFPLOADI	L0, 16448, 0
	SFPLZ	L1, L0, 0
	TTNOP
	TTNOP
	SFPSETCC	L0, 0, 4
	TTNOP
	SFPSTORE	0, L10, 0
	SFPENCC	3, 10
	TTNOP
	SFPSTORE	0, L0, 0
	SFPSTORE	0, L1, 2
	ret
	.size	_Z6lz_no2v, .-_Z6lz_no2v
	.align	2
	.globl	_Z6lz_no3v
	.type	_Z6lz_no3v, @function
_Z6lz_no3v:
	SFPLOADI	L0, 16448, 0
	SFPLZ	L1, L0, 0
	TTNOP
	TTNOP
	SFPSETCC	L0, 0, 0
	TTNOP
	SFPSTORE	0, L10, 0
	SFPENCC	3, 10
	TTNOP
	SFPSTORE	0, L0, 0
	SFPSTORE	0, L1, 2
	ret
	.size	_Z6lz_no3v, .-_Z6lz_no3v
	.align	2
	.globl	_Z9lz_no_bb1i
	.type	_Z9lz_no_bb1i, @function
_Z9lz_no_bb1i:
	SFPLOADI	L0, 16448, 0
	SFPLZ	L1, L0, 0
	TTNOP
	TTNOP
	ble	a0,zero,.L54
	li	a5,0
.L55:
	SFPSETCC	L0, 0, 0
	TTNOP
	SFPADDI	L0, 16256, 0
	TTNOP
	TTNOP
	SFPENCC	3, 10
	TTNOP
	addi	a5,a5,1
	bne	a0,a5,.L55
.L54:
	SFPSTORE	0, L0, 0
	ret
	.size	_Z9lz_no_bb1i, .-_Z9lz_no_bb1i
	.align	2
	.globl	_Z10exexp_yes1v
	.type	_Z10exexp_yes1v, @function
_Z10exexp_yes1v:
	SFPLOADI	L0, 16448, 0
	SFPEXEXP	L1, L0, 10
	TTNOP
	TTNOP
	TTNOP
	SFPSTORE	0, L10, 0
	SFPENCC	3, 10
	TTNOP
	SFPSTORE	0, L0, 0
	SFPSTORE	0, L1, 2
	ret
	.size	_Z10exexp_yes1v, .-_Z10exexp_yes1v
	.align	2
	.globl	_Z15exexp_yes1_livev
	.type	_Z15exexp_yes1_livev, @function
_Z15exexp_yes1_livev:
	SFPLOADI	L1, 16448, 0
	SFPLOADI	L0, 1, 4
	SFPSETCC	L1, 0, 0
	TTNOP
	SFPEXEXP	L0, L1, 10
	TTNOP
	TTNOP
	TTNOP
	SFPSTORE	0, L10, 0
	SFPENCC	3, 10
	TTNOP
	SFPSTORE	0, L1, 0
	SFPSTORE	0, L0, 2
	ret
	.size	_Z15exexp_yes1_livev, .-_Z15exexp_yes1_livev
	.align	2
	.globl	_Z10exexp_yes2v
	.type	_Z10exexp_yes2v, @function
_Z10exexp_yes2v:
	SFPLOADI	L0, 16448, 0
	SFPEXEXP	L1, L0, 2
	TTNOP
	TTNOP
	TTNOP
	SFPSTORE	0, L10, 0
	SFPENCC	3, 10
	TTNOP
	SFPSTORE	0, L0, 0
	SFPSTORE	0, L1, 2
	ret
	.size	_Z10exexp_yes2v, .-_Z10exexp_yes2v
	.align	2
	.globl	_Z10exexp_yes3v
	.type	_Z10exexp_yes3v, @function
_Z10exexp_yes3v:
	SFPLOADI	L0, 16448, 0
	SFPEXEXP	L0, L0, 2
	TTNOP
	TTNOP
	TTNOP
	SFPSTORE	0, L10, 0
	SFPENCC	3, 10
	TTNOP
	SFPSTORE	0, L0, 0
	ret
	.size	_Z10exexp_yes3v, .-_Z10exexp_yes3v
	.align	2
	.globl	_Z9exexp_no1v
	.type	_Z9exexp_no1v, @function
_Z9exexp_no1v:
	SFPLOADI	L1, 16448, 0
	SFPEXEXP	L0, L1, 0
	TTNOP
	TTNOP
	SFPIADD	L2, L0, -5, 1
	TTNOP
	TTNOP
	TTNOP
	SFPSTORE	0, L10, 0
	SFPENCC	3, 10
	TTNOP
	SFPSTORE	0, L1, 0
	SFPSTORE	0, L0, 2
	ret
	.size	_Z9exexp_no1v, .-_Z9exexp_no1v
	.align	2
	.globl	_Z9exexp_no2v
	.type	_Z9exexp_no2v, @function
_Z9exexp_no2v:
	SFPLOADI	L1, 16448, 0
	SFPEXEXP	L0, L1, 1
	TTNOP
	TTNOP
	SFPSETCC	L0, 0, 4
	TTNOP
	SFPSTORE	0, L10, 0
	SFPENCC	3, 10
	TTNOP
	SFPSTORE	0, L1, 0
	SFPSTORE	0, L0, 2
	ret
	.size	_Z9exexp_no2v, .-_Z9exexp_no2v
	.align	2
	.globl	_Z12exexp_no_bb1i
	.type	_Z12exexp_no_bb1i, @function
_Z12exexp_no_bb1i:
	SFPLOADI	L1, 16448, 0
	SFPEXEXP	L0, L1, 0
	TTNOP
	TTNOP
	ble	a0,zero,.L64
	li	a5,0
.L65:
	SFPSETCC	L0, 0, 4
	TTNOP
	SFPSTORE	0, L10, 0
	SFPENCC	3, 10
	TTNOP
	addi	a5,a5,1
	bne	a0,a5,.L65
.L64:
	SFPSTORE	0, L1, 0
	SFPSTORE	0, L0, 2
	ret
	.size	_Z12exexp_no_bb1i, .-_Z12exexp_no_bb1i
	.align	2
	.globl	_Z8mad_yes1v
	.type	_Z8mad_yes1v, @function
_Z8mad_yes1v:
	SFPLOAD	L0, 0, 1
	SFPLOAD	L1, 4, 1
	SFPLOAD	L2, 8, 1
	SFPMAD	L0, L0, L1, L2, 0
	TTNOP
	TTNOP
	SFPSTORE	0, L0, 0
	ret
	.size	_Z8mad_yes1v, .-_Z8mad_yes1v
	.align	2
	.globl	_Z8mad_yes2v
	.type	_Z8mad_yes2v, @function
_Z8mad_yes2v:
	SFPLOAD	L0, 0, 1
	SFPLOAD	L1, 4, 1
	SFPLOAD	L2, 8, 1
	SFPMAD	L3, L0, L1, L2, 0
	TTNOP
	TTNOP
	SFPSTORE	0, L3, 0
	SFPMAD	L0, L0, L1, L2, 0
	TTNOP
	TTNOP
	SFPSTORE	0, L0, 0
	ret
	.size	_Z8mad_yes2v, .-_Z8mad_yes2v
	.align	2
	.globl	_Z8mad_yes3v
	.type	_Z8mad_yes3v, @function
_Z8mad_yes3v:
	SFPLOAD	L1, 0, 1
	SFPLOAD	L0, 4, 1
	SFPLOAD	L2, 8, 1
	SFPMAD	L0, L1, L0, L2, 0
	TTNOP
	TTNOP
	SFPMAD	L0, L10, L0, L10, 0
	TTNOP
	TTNOP
	SFPMAD	L1, L1, L0, L2, 0
	TTNOP
	TTNOP
	SFPSTORE	0, L1, 0
	ret
	.size	_Z8mad_yes3v, .-_Z8mad_yes3v
	.align	2
	.globl	_Z17mad_maybe_future1v
	.type	_Z17mad_maybe_future1v, @function
_Z17mad_maybe_future1v:
	SFPLOAD	L1, 0, 1
	SFPLOAD	L3, 4, 1
	SFPLOAD	L2, 8, 1
	SFPLOADI	L0, 16512, 0
	SFPMUL	L1, L1, L3, L4, 0
	TTNOP
	TTNOP
	SFPMUL	L0, L2, L0, L4, 0
	TTNOP
	TTNOP
	SFPADD	L1, L10, L1, L2, 0
	TTNOP
	TTNOP
	SFPSTORE	0, L1, 0
	SFPADD	L0, L10, L0, L2, 0
	TTNOP
	TTNOP
	SFPSTORE	0, L0, 0
	ret
	.size	_Z17mad_maybe_future1v, .-_Z17mad_maybe_future1v
	.align	2
	.globl	_Z17mad_maybe_future2v
	.type	_Z17mad_maybe_future2v, @function
_Z17mad_maybe_future2v:
	SFPLOAD	L0, 0, 1
	SFPLOAD	L1, 4, 1
	SFPMUL	L0, L0, L1, L4, 0
	TTNOP
	TTNOP
	SFPLOAD	L1, 8, 1
	SFPADD	L1, L10, L0, L1, 0
	TTNOP
	TTNOP
	SFPLOAD	L2, 12, 1
	SFPADD	L0, L10, L0, L2, 0
	TTNOP
	TTNOP
	SFPSTORE	0, L1, 0
	SFPSTORE	0, L0, 0
	ret
	.size	_Z17mad_maybe_future2v, .-_Z17mad_maybe_future2v
	.align	2
	.globl	_Z13mad_live_yes1v
	.type	_Z13mad_live_yes1v, @function
_Z13mad_live_yes1v:
	SFPLOAD	L1, 0, 1
	SFPLOAD	L2, 4, 1
	SFPLOAD	L3, 8, 1
	SFPLOAD	L0, 12, 1
	SFPSETCC	L1, 0, 6
	TTNOP
	SFPMAD	L0, L1, L2, L3, 0
	TTNOP
	TTNOP
	SFPENCC	3, 10
	TTNOP
	SFPSTORE	0, L0, 0
	ret
	.size	_Z13mad_live_yes1v, .-_Z13mad_live_yes1v
	.align	2
	.globl	_Z20mad_reg_pressure_no4v
	.type	_Z20mad_reg_pressure_no4v, @function
_Z20mad_reg_pressure_no4v:
	SFPLOAD	L1, 0, 1
	SFPLOAD	L0, 4, 1
	SFPLOAD	L3, 8, 1
	SFPMUL	L2, L1, L0, L4, 0
	TTNOP
	TTNOP
	SFPEXEXP	L1, L1, 0
	TTNOP
	TTNOP
	SFPEXEXP	L0, L0, 0
	TTNOP
	TTNOP
	SFPADD	L2, L10, L2, L3, 0
	TTNOP
	TTNOP
	SFPSTORE	0, L3, 0
	SFPSTORE	0, L2, 0
	SFPSTORE	4, L1, 2
	SFPSTORE	4, L0, 2
	ret
	.size	_Z20mad_reg_pressure_no4v, .-_Z20mad_reg_pressure_no4v
	.align	2
	.globl	_Z7mad_no1v
	.type	_Z7mad_no1v, @function
_Z7mad_no1v:
	SFPLOAD	L0, 0, 1
	SFPLOAD	L2, 4, 1
	SFPLOAD	L1, 8, 1
	SFPMUL	L0, L0, L2, L4, 0
	TTNOP
	TTNOP
	SFPADD	L1, L10, L1, L0, 0
	TTNOP
	TTNOP
	SFPSTORE	0, L0, 0
	SFPSTORE	0, L1, 0
	ret
	.size	_Z7mad_no1v, .-_Z7mad_no1v
	.align	2
	.globl	_Z7mad_no2v
	.type	_Z7mad_no2v, @function
_Z7mad_no2v:
	SFPLOAD	L0, 0, 1
	SFPLOAD	L2, 4, 1
	SFPLOAD	L1, 8, 1
	SFPMUL	L0, L0, L2, L4, 0
	TTNOP
	TTNOP
	SFPSTORE	0, L0, 0
	SFPADD	L0, L10, L1, L0, 0
	TTNOP
	TTNOP
	SFPSTORE	0, L0, 0
	ret
	.size	_Z7mad_no2v, .-_Z7mad_no2v
	.align	2
	.globl	_Z7mad_no3v
	.type	_Z7mad_no3v, @function
_Z7mad_no3v:
	SFPLOAD	L2, 0, 1
	SFPLOAD	L1, 4, 1
	SFPLOAD	L0, 8, 1
	SFPMUL	L1, L2, L1, L4, 0
	TTNOP
	TTNOP
	SFPSETCC	L2, 0, 4
	TTNOP
	SFPENCC	3, 10
	TTNOP
	SFPADD	L0, L10, L0, L1, 0
	TTNOP
	TTNOP
	SFPSTORE	0, L0, 0
	ret
	.size	_Z7mad_no3v, .-_Z7mad_no3v
	.align	2
	.globl	_Z12mad_live_no1v
	.type	_Z12mad_live_no1v, @function
_Z12mad_live_no1v:
	SFPLOAD	L3, 0, 1
	SFPLOAD	L1, 4, 1
	SFPLOAD	L2, 8, 1
	SFPLOAD	L0, 12, 1
	SFPMUL	L1, L3, L1, L4, 0
	TTNOP
	TTNOP
	SFPSETCC	L3, 0, 6
	TTNOP
	SFPADD	L0, L10, L1, L2, 0
	TTNOP
	TTNOP
	SFPENCC	3, 10
	TTNOP
	SFPSTORE	0, L0, 0
	ret
	.size	_Z12mad_live_no1v, .-_Z12mad_live_no1v
	.align	2
	.globl	_Z12mad_live_no2v
	.type	_Z12mad_live_no2v, @function
_Z12mad_live_no2v:
	SFPLOAD	L3, 0, 1
	SFPLOAD	L0, 4, 1
	SFPLOAD	L2, 8, 1
	SFPLOAD	L1, 12, 1
	SFPMUL	L0, L3, L0, L4, 0
	TTNOP
	TTNOP
	SFPSETCC	L3, 0, 6
	TTNOP
	SFPADD	L1, L10, L0, L2, 0
	TTNOP
	TTNOP
	SFPENCC	3, 10
	TTNOP
	SFPSTORE	0, L1, 0
	SFPSTORE	0, L0, 0
	ret
	.size	_Z12mad_live_no2v, .-_Z12mad_live_no2v
	.align	2
	.globl	_Z10mad_no_bb1i
	.type	_Z10mad_no_bb1i, @function
_Z10mad_no_bb1i:
	SFPLOAD	L0, 0, 1
	SFPLOAD	L2, 4, 1
	SFPLOAD	L1, 8, 1
	SFPMUL	L0, L0, L2, L4, 0
	TTNOP
	TTNOP
	ble	a0,zero,.L80
	li	a5,0
.L81:
	SFPADD	L0, L10, L0, L1, 0
	TTNOP
	TTNOP
	addi	a5,a5,1
	bne	a0,a5,.L81
.L80:
	SFPSTORE	0, L0, 0
	ret
	.size	_Z10mad_no_bb1i, .-_Z10mad_no_bb1i
	.align	2
	.globl	_Z9addi_yes1v
	.type	_Z9addi_yes1v, @function
_Z9addi_yes1v:
	SFPLOADI	L0, 16384, 0
	SFPADDI	L0, 16256, 0
	TTNOP
	TTNOP
	SFPSTORE	0, L0, 0
	ret
	.size	_Z9addi_yes1v, .-_Z9addi_yes1v
	.align	2
	.globl	_Z15addi_yes_caveatv
	.type	_Z15addi_yes_caveatv, @function
_Z15addi_yes_caveatv:
	SFPLOADI	L0, 16256, 0
	SFPLOADI	L1, 16384, 0
	SFPADD	L1, L10, L0, L1, 0
	TTNOP
	TTNOP
	SFPSTORE	0, L1, 0
	SFPADDI	L0, 16384, 0
	TTNOP
	TTNOP
	SFPSTORE	4, L0, 0
	ret
	.size	_Z15addi_yes_caveatv, .-_Z15addi_yes_caveatv
	.align	2
	.globl	_Z9addi_yes2v
	.type	_Z9addi_yes2v, @function
_Z9addi_yes2v:
	SFPLOADI	L0, 16256, 0
	SFPADDI	L0, 16384, 0
	TTNOP
	TTNOP
	SFPADDI	L0, 16384, 0
	TTNOP
	TTNOP
	SFPADDI	L0, 16384, 0
	TTNOP
	TTNOP
	SFPSTORE	0, L0, 0
	ret
	.size	_Z9addi_yes2v, .-_Z9addi_yes2v
	.align	2
	.globl	_Z9addi_yes3v
	.type	_Z9addi_yes3v, @function
_Z9addi_yes3v:
	SFPLOAD	L0, 0, 1
	SFPLOAD	L1, 4, 1
	SFPLOADI	L2, 16256, 0
	SFPMAD	L0, L0, L1, L2, 0
	TTNOP
	TTNOP
	SFPADDI	L0, 16384, 0
	TTNOP
	TTNOP
	SFPSTORE	0, L0, 0
	ret
	.size	_Z9addi_yes3v, .-_Z9addi_yes3v
	.align	2
	.globl	_Z9addi_yes4v
	.type	_Z9addi_yes4v, @function
_Z9addi_yes4v:
	SFPLOADI	L1, 16256, 0
	SFPLOADI	L0, 16384, 0
	SFPADDI	L0, 16256, 0
	TTNOP
	TTNOP
	SFPSTORE	0, L0, 0
	SFPSTORE	0, L1, 0
	SFPLOADI	L0, 16256, 0
	SFPLOADI	L1, 16384, 0
	SFPADDI	L0, 16384, 0
	TTNOP
	TTNOP
	SFPSTORE	0, L0, 0
	SFPSTORE	0, L1, 0
	ret
	.size	_Z9addi_yes4v, .-_Z9addi_yes4v
	.align	2
	.globl	_Z13addi_yes_imm1j
	.type	_Z13addi_yes_imm1j, @function
_Z13addi_yes_imm1j:
	slli	a0,a0,16
	lui	a5,%hi(_ZN7ckernel13instrn_bufferE)
	li	a4, 1895825408	# 71000000
	lw	a5,%lo(_ZN7ckernel13instrn_bufferE)(a5)
	srli	a0,a0,16
	add	a0,a0,a4
	sw	a0, 0(a5)	# Op(0x71) - d(lr0)
	SFPADDI	L0, 16256, 0
	TTNOP
	TTNOP
	SFPSTORE	0, L0, 0
	ret
	.size	_Z13addi_yes_imm1j, .-_Z13addi_yes_imm1j
	.align	2
	.globl	_Z13addi_yes_imm2j
	.type	_Z13addi_yes_imm2j, @function
_Z13addi_yes_imm2j:
	lui	a5,%hi(_ZN7ckernel13instrn_bufferE)
	lw	a4,%lo(_ZN7ckernel13instrn_bufferE)(a5)
	SFPLOADI	L0, 16256, 0
	slli	a5,a0,16
	li	a3, 1962934272	# 75000000
	srli	a5,a5,8
	add	a5,a5,a3
	sw	a5, 0(a4)	# Op(0x75) lv(lr0)  d(lr0)
	TTNOP
	TTNOP
	SFPSTORE	0, L0, 0
	ret
	.size	_Z13addi_yes_imm2j, .-_Z13addi_yes_imm2j
	.align	2
	.globl	_Z13addi_yes_imm3j
	.type	_Z13addi_yes_imm3j, @function
_Z13addi_yes_imm3j:
	lui	a5,%hi(_ZN7ckernel13instrn_bufferE)
	lw	a4,%lo(_ZN7ckernel13instrn_bufferE)(a5)
	SFPLOAD	L0, 0, 1
	slli	a5,a0,16
	li	a3, 1962934272	# 75000000
	srli	a5,a5,8
	add	a5,a5,a3
	sw	a5, 0(a4)	# Op(0x75) lv(lr0)  d(lr0)
	TTNOP
	TTNOP
	SFPSTORE	0, L0, 0
	ret
	.size	_Z13addi_yes_imm3j, .-_Z13addi_yes_imm3j
	.align	2
	.globl	_Z13addi_yes_imm4j
	.type	_Z13addi_yes_imm4j, @function
_Z13addi_yes_imm4j:
	SFPLOAD	L0, 0, 1
	slli	a5,a0,16
	lui	a4,%hi(_ZN7ckernel13instrn_bufferE)
	li	a3, 1962934272	# 75000000
	lw	a4,%lo(_ZN7ckernel13instrn_bufferE)(a4)
	srli	a5,a5,8
	add	a5,a5,a3
	sw	a5, 0(a4)	# Op(0x75) lv(lr0)  d(lr0)
	TTNOP
	TTNOP
	SFPSTORE	0, L0, 0
	ret
	.size	_Z13addi_yes_imm4j, .-_Z13addi_yes_imm4j
	.align	2
	.globl	_Z14addi_live_yes1v
	.type	_Z14addi_live_yes1v, @function
_Z14addi_live_yes1v:
	SFPLOADI	L0, 16384, 0
	SFPLOAD	L1, 0, 1
	SFPSETCC	L1, 0, 6
	TTNOP
	SFPADDI	L0, 16448, 0
	TTNOP
	TTNOP
	SFPENCC	3, 10
	TTNOP
	SFPSTORE	0, L0, 0
	ret
	.size	_Z14addi_live_yes1v, .-_Z14addi_live_yes1v
	.align	2
	.globl	_Z14addi_live_yes2v
	.type	_Z14addi_live_yes2v, @function
_Z14addi_live_yes2v:
	SFPLOADI	L0, 16384, 0
	SFPLOAD	L1, 0, 1
	SFPSETCC	L1, 0, 6
	TTNOP
	SFPADDI	L0, 16448, 0
	TTNOP
	TTNOP
	SFPENCC	3, 10
	TTNOP
	SFPSTORE	0, L0, 0
	ret
	.size	_Z14addi_live_yes2v, .-_Z14addi_live_yes2v
	.align	2
	.globl	_Z21addi_live_yes3_futurev
	.type	_Z21addi_live_yes3_futurev, @function
_Z21addi_live_yes3_futurev:
	SFPLOADI	L0, 16384, 0
	SFPLOADI	L1, 16448, 0
	SFPLOAD	L2, 0, 1
	SFPSETCC	L2, 0, 6
	TTNOP
	SFPADD	L0, L10, L0, L1, 0
	TTNOP
	TTNOP
	SFPENCC	3, 10
	TTNOP
	SFPSTORE	0, L0, 0
	ret
	.size	_Z21addi_live_yes3_futurev, .-_Z21addi_live_yes3_futurev
	.align	2
	.globl	_Z21addi_live_yes4_futurev
	.type	_Z21addi_live_yes4_futurev, @function
_Z21addi_live_yes4_futurev:
	SFPLOADI	L1, 16448, 0
	SFPLOADI	L0, 16384, 0
	SFPLOAD	L2, 0, 1
	SFPSETCC	L2, 0, 6
	TTNOP
	SFPLOADI	L1, 16512, 0
	SFPADD	L0, L10, L0, L1, 0
	TTNOP
	TTNOP
	SFPENCC	3, 10
	TTNOP
	SFPSTORE	0, L0, 0
	ret
	.size	_Z21addi_live_yes4_futurev, .-_Z21addi_live_yes4_futurev
	.align	2
	.globl	_Z13addi_live_no2v
	.type	_Z13addi_live_no2v, @function
_Z13addi_live_no2v:
	SFPLOADI	L2, 16384, 0
	SFPLOADI	L0, 16256, 0
	SFPLOAD	L1, 0, 1
	SFPSETCC	L1, 0, 6
	TTNOP
	SFPLOADI	L1, 16448, 0
	SFPADD	L0, L10, L1, L2, 0
	TTNOP
	TTNOP
	SFPENCC	3, 10
	TTNOP
	SFPSTORE	0, L0, 0
	ret
	.size	_Z13addi_live_no2v, .-_Z13addi_live_no2v
	.align	2
	.globl	_Z13addi_live_no3v
	.type	_Z13addi_live_no3v, @function
_Z13addi_live_no3v:
	SFPLOADI	L1, 16256, 0
	SFPLOADI	L2, 16384, 0
	SFPLOADI	L0, 16256, 0
	SFPLOAD	L3, 0, 1
	SFPSETCC	L3, 0, 6
	TTNOP
	SFPLOADI	L1, 16448, 0
	SFPADD	L0, L10, L1, L2, 0
	TTNOP
	TTNOP
	SFPENCC	3, 10
	TTNOP
	SFPSTORE	0, L0, 0
	ret
	.size	_Z13addi_live_no3v, .-_Z13addi_live_no3v
	.align	2
	.globl	_Z13addi_live_no4v
	.type	_Z13addi_live_no4v, @function
_Z13addi_live_no4v:
	SFPLOADI	L1, 16256, 0
	SFPLOAD	L2, 0, 1
	SFPLOADI	L0, 16256, 0
	SFPLOAD	L3, 0, 1
	SFPSETCC	L3, 0, 6
	TTNOP
	SFPLOADI	L1, 16448, 0
	SFPADD	L0, L10, L1, L2, 0
	TTNOP
	TTNOP
	SFPENCC	3, 10
	TTNOP
	SFPSTORE	0, L0, 0
	ret
	.size	_Z13addi_live_no4v, .-_Z13addi_live_no4v
	.align	2
	.globl	_Z8addi_no1v
	.type	_Z8addi_no1v, @function
_Z8addi_no1v:
	SFPLOADI	L0, 16256, 0
	SFPLOAD	L1, 0, 1
	SFPLOAD	L2, 0, 1
	SFPSETCC	L2, 0, 6
	TTNOP
	SFPLOADI	L0, 16448, 0
	SFPENCC	3, 10
	TTNOP
	SFPADD	L0, L10, L0, L1, 0
	TTNOP
	TTNOP
	SFPSTORE	0, L0, 0
	ret
	.size	_Z8addi_no1v, .-_Z8addi_no1v
	.align	2
	.globl	_Z8addi_no2v
	.type	_Z8addi_no2v, @function
_Z8addi_no2v:
	SFPLOADI	L1, 16256, 0
	SFPLOADI	L0, 16384, 0
	SFPADD	L2, L10, L1, L0, 0
	TTNOP
	TTNOP
	SFPSTORE	0, L1, 0
	SFPSTORE	0, L0, 0
	SFPSTORE	0, L2, 0
	ret
	.size	_Z8addi_no2v, .-_Z8addi_no2v
	.align	2
	.globl	_Z11addi_no_bb1i
	.type	_Z11addi_no_bb1i, @function
_Z11addi_no_bb1i:
	SFPLOADI	L1, 16256, 0
	SFPLOADI	L2, 16384, 0
	ble	a0,zero,.L102
	SFPADD	L0, L10, L1, L2, 0
	TTNOP
	TTNOP
	li	a5,0
	j	.L103
.L104:
	SFPADD	L0, L10, L1, L2, 0
	TTNOP
	TTNOP
.L103:
	addi	a5,a5,1
	bne	a0,a5,.L104
.L102:
	SFPSTORE	0, L0, 0
	ret
	.size	_Z11addi_no_bb1i, .-_Z11addi_no_bb1i
	.align	2
	.globl	_Z9muli_yes1v
	.type	_Z9muli_yes1v, @function
_Z9muli_yes1v:
	SFPLOADI	L0, 16384, 0
	SFPMULI	L0, 16256, 0
	TTNOP
	TTNOP
	SFPSTORE	0, L0, 0
	ret
	.size	_Z9muli_yes1v, .-_Z9muli_yes1v
	.align	2
	.globl	_Z15muli_yes_caveatv
	.type	_Z15muli_yes_caveatv, @function
_Z15muli_yes_caveatv:
	SFPLOADI	L0, 16256, 0
	SFPLOADI	L1, 16384, 0
	SFPMUL	L1, L0, L1, L4, 0
	TTNOP
	TTNOP
	SFPSTORE	0, L1, 0
	SFPMULI	L0, 16384, 0
	TTNOP
	TTNOP
	SFPSTORE	4, L0, 0
	ret
	.size	_Z15muli_yes_caveatv, .-_Z15muli_yes_caveatv
	.align	2
	.globl	_Z18add_plus_half_yes1v
	.type	_Z18add_plus_half_yes1v, @function
_Z18add_plus_half_yes1v:
	SFPLOAD	L0, 0, 1
	SFPLOAD	L1, 4, 1
	SFPLOAD	L2, 8, 1
	SFPADD	L3, L10, L0, L1, 1
	TTNOP
	TTNOP
	SFPSTORE	0, L3, 0
	SFPADD	L3, L10, L0, L1, 3
	TTNOP
	TTNOP
	SFPSTORE	4, L3, 0
	SFPMUL	L3, L0, L1, L4, 1
	TTNOP
	TTNOP
	SFPSTORE	8, L3, 0
	SFPMUL	L3, L0, L1, L4, 3
	TTNOP
	TTNOP
	SFPSTORE	12, L3, 0
	SFPMAD	L3, L0, L1, L2, 1
	TTNOP
	TTNOP
	SFPSTORE	16, L3, 0
	SFPMAD	L0, L0, L1, L2, 3
	TTNOP
	TTNOP
	SFPSTORE	20, L0, 0
	TTNOP
	SFPLOAD	L0, 0, 1
	SFPADDI	L0, 16256, 1
	TTNOP
	TTNOP
	SFPSTORE	24, L0, 0
	TTNOP
	SFPLOAD	L0, 0, 1
	SFPADDI	L0, 16256, 3
	TTNOP
	TTNOP
	SFPSTORE	28, L0, 0
	TTNOP
	SFPLOAD	L0, 0, 1
	SFPMULI	L0, 16256, 1
	TTNOP
	TTNOP
	SFPSTORE	32, L0, 0
	TTNOP
	SFPLOAD	L0, 0, 1
	SFPMULI	L0, 16256, 3
	TTNOP
	TTNOP
	SFPSTORE	36, L0, 0
	ret
	.size	_Z18add_plus_half_yes1v, .-_Z18add_plus_half_yes1v
	.align	2
	.globl	_Z18add_plus_half_yes2v
	.type	_Z18add_plus_half_yes2v, @function
_Z18add_plus_half_yes2v:
	SFPLOAD	L0, 0, 1
	SFPLOAD	L1, 4, 1
	SFPLOAD	L2, 8, 1
	SFPMAD	L0, L0, L1, L2, 1
	TTNOP
	TTNOP
	SFPSTORE	4, L0, 0
	ret
	.size	_Z18add_plus_half_yes2v, .-_Z18add_plus_half_yes2v
	.align	2
	.globl	_Z18add_plus_half_yes3v
	.type	_Z18add_plus_half_yes3v, @function
_Z18add_plus_half_yes3v:
	SFPLOAD	L0, 0, 1
	SFPLOAD	L2, 4, 1
	SFPLOAD	L3, 8, 1
	SFPLOADI	L1, 16128, 0
	SFPMAD	L0, L0, L2, L3, 1
	TTNOP
	TTNOP
	SFPSTORE	4, L0, 0
	SFPSTORE	8, L1, 0
	ret
	.size	_Z18add_plus_half_yes3v, .-_Z18add_plus_half_yes3v
	.align	2
	.globl	_Z30add_plus_half_live_yes1_futurev
	.type	_Z30add_plus_half_live_yes1_futurev, @function
_Z30add_plus_half_live_yes1_futurev:
	SFPLOAD	L0, 0, 1
	SFPLOAD	L1, 4, 1
	SFPSETCC	L0, 0, 0
	TTNOP
	SFPADD	L1, L10, L0, L1, 0
	TTNOP
	TTNOP
	SFPLOADI	L2, 16128, 0
	SFPADD	L0, L10, L1, L2, 0
	TTNOP
	TTNOP
	SFPENCC	3, 10
	TTNOP
	SFPSTORE	8, L0, 0
	ret
	.size	_Z30add_plus_half_live_yes1_futurev, .-_Z30add_plus_half_live_yes1_futurev
	.align	2
	.globl	_Z30add_plus_half_live_yes2_futurev
	.type	_Z30add_plus_half_live_yes2_futurev, @function
_Z30add_plus_half_live_yes2_futurev:
	SFPLOAD	L0, 0, 1
	SFPLOAD	L1, 4, 1
	SFPSETCC	L0, 0, 0
	TTNOP
	SFPADD	L0, L10, L0, L1, 0
	TTNOP
	TTNOP
	SFPADDI	L0, 16128, 0
	TTNOP
	TTNOP
	SFPENCC	3, 10
	TTNOP
	SFPSTORE	8, L0, 0
	ret
	.size	_Z30add_plus_half_live_yes2_futurev, .-_Z30add_plus_half_live_yes2_futurev
	.align	2
	.globl	_Z30add_plus_half_live_yes3_futurev
	.type	_Z30add_plus_half_live_yes3_futurev, @function
_Z30add_plus_half_live_yes3_futurev:
	SFPLOAD	L0, 0, 1
	SFPLOAD	L2, 4, 1
	SFPLOAD	L1, 8, 1
	SFPSETCC	L0, 0, 0
	TTNOP
	SFPADD	L1, L10, L0, L2, 0
	TTNOP
	TTNOP
	SFPLOADI	L2, 16128, 0
	SFPADD	L1, L10, L0, L2, 0
	TTNOP
	TTNOP
	SFPENCC	3, 10
	TTNOP
	SFPSTORE	12, L0, 0
	ret
	.size	_Z30add_plus_half_live_yes3_futurev, .-_Z30add_plus_half_live_yes3_futurev
	.align	2
	.globl	_Z22add_plus_half_live_no1v
	.type	_Z22add_plus_half_live_no1v, @function
_Z22add_plus_half_live_no1v:
	SFPLOAD	L0, 0, 1
	SFPLOAD	L1, 4, 1
	SFPADD	L0, L10, L0, L1, 0
	TTNOP
	TTNOP
	SFPSETCC	L0, 0, 0
	TTNOP
	SFPADDI	L0, 16128, 0
	TTNOP
	TTNOP
	SFPENCC	3, 10
	TTNOP
	SFPSTORE	8, L0, 0
	ret
	.size	_Z22add_plus_half_live_no1v, .-_Z22add_plus_half_live_no1v
	.align	2
	.globl	_Z22add_plus_half_live_no2v
	.type	_Z22add_plus_half_live_no2v, @function
_Z22add_plus_half_live_no2v:
	SFPLOAD	L0, 0, 1
	SFPLOAD	L1, 4, 1
	SFPSETCC	L0, 0, 0
	TTNOP
	SFPADD	L0, L10, L0, L1, 0
	TTNOP
	TTNOP
	SFPENCC	3, 10
	TTNOP
	SFPADDI	L0, 16128, 0
	TTNOP
	TTNOP
	SFPSTORE	8, L0, 0
	ret
	.size	_Z22add_plus_half_live_no2v, .-_Z22add_plus_half_live_no2v
	.align	2
	.globl	_Z17add_plus_half_no1j
	.type	_Z17add_plus_half_no1j, @function
_Z17add_plus_half_no1j:
	SFPLOAD	L0, 0, 1
	SFPLOAD	L1, 4, 1
	SFPLOAD	L2, 8, 1
	SFPMAD	L0, L0, L1, L2, 0
	TTNOP
	TTNOP
	slli	a5,a0,16
	lui	a4,%hi(_ZN7ckernel13instrn_bufferE)
	li	a3, 1962934272	# 75000000
	lw	a4,%lo(_ZN7ckernel13instrn_bufferE)(a4)
	srli	a5,a5,8
	add	a5,a5,a3
	sw	a5, 0(a4)	# Op(0x75) lv(lr0)  d(lr0)
	TTNOP
	TTNOP
	SFPSTORE	0, L0, 0
	ret
	.size	_Z17add_plus_half_no1j, .-_Z17add_plus_half_no1j
	.align	2
	.globl	_Z17add_plus_half_no2v
	.type	_Z17add_plus_half_no2v, @function
_Z17add_plus_half_no2v:
	SFPLOAD	L0, 0, 1
	SFPLOAD	L2, 4, 1
	SFPLOAD	L1, 8, 1
	SFPMUL	L0, L0, L2, L4, 1
	TTNOP
	TTNOP
	SFPADD	L0, L10, L0, L1, 1
	TTNOP
	TTNOP
	SFPSTORE	4, L0, 0
	ret
	.size	_Z17add_plus_half_no2v, .-_Z17add_plus_half_no2v
	.align	2
	.globl	_Z20add_plus_half_no_bb1i
	.type	_Z20add_plus_half_no_bb1i, @function
_Z20add_plus_half_no_bb1i:
	SFPLOAD	L0, 0, 1
	SFPLOAD	L2, 4, 1
	SFPLOADI	L1, 16128, 0
	SFPADD	L0, L10, L0, L2, 0
	TTNOP
	TTNOP
	ble	a0,zero,.L118
	li	a5,0
.L119:
	SFPADD	L0, L10, L0, L1, 0
	TTNOP
	TTNOP
	addi	a5,a5,1
	bne	a0,a5,.L119
.L118:
	SFPSTORE	0, L0, 0
	ret
	.size	_Z20add_plus_half_no_bb1i, .-_Z20add_plus_half_no_bb1i
	.align	2
	.globl	_Z10complexishv
	.type	_Z10complexishv, @function
_Z10complexishv:
	SFPLOAD	L0, 0, 1
	SFPLOAD	L2, 4, 1
	SFPLOADI	L1, 16736, 0
	SFPLOADI	L3, 16448, 0
	SFPMAD	L0, L0, L2, L3, 1
	TTNOP
	TTNOP
	SFPLOAD	L2, 8, 1
	SFPLOAD	L3, 12, 1
	SFPMAD	L0, L0, L2, L3, 0
	TTNOP
	TTNOP
	SFPLOADI	L2, 16640, 0
	SFPLOADI	L3, 16672, 0
	SFPMAD	L0, L0, L2, L3, 1
	TTNOP
	TTNOP
	SFPMULI	L0, 16704, 3
	TTNOP
	TTNOP
	SFPMULI	L0, 16736, 0
	TTNOP
	TTNOP
	SFPMULI	L0, 0, 0
	TTNOP
	TTNOP
	SFPSTORE	16, L0, 0
	TTNOP
	SFPLOAD	L2, 20, 1
	SFPLOAD	L0, 24, 1
	SFPLOADI	L3, -16320, 0
	SFPMAD	L2, L2, L0, L3, 1
	TTNOP
	TTNOP
	SFPLOAD	L0, 28, 1
	SFPMOV	L0, L0, 1
	TTNOP
	TTNOP
	SFPMUL	L2, L2, L0, L4, 0
	TTNOP
	TTNOP
	SFPLOAD	L0, 32, 1
	SFPMAD	L0, L11, L0, L2, 0
	TTNOP
	TTNOP
	SFPLOADI	L2, 16640, 0
	SFPLOADI	L3, -16096, 0
	SFPMAD	L0, L0, L2, L3, 3
	TTNOP
	TTNOP
	SFPMULI	L0, 16704, 3
	TTNOP
	TTNOP
	SFPMOV	L1, L1, 1
	TTNOP
	TTNOP
	SFPMUL	L0, L0, L1, L4, 0
	TTNOP
	TTNOP
	SFPMULI	L0, 0, 0
	TTNOP
	TTNOP
	SFPSTORE	36, L0, 0
	ret
	.size	_Z10complexishv, .-_Z10complexishv
	.globl	_ZN7ckernel13instrn_bufferE
	.section	.sbss,"aw",@nobits
	.align	2
	.type	_ZN7ckernel13instrn_bufferE, @object
	.size	_ZN7ckernel13instrn_bufferE, 4
_ZN7ckernel13instrn_bufferE:
	.zero	4
	.ident	"GCC: (g96bd106687d) 12.2.0"
