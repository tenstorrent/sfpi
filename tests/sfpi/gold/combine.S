	.file	"combine.cc"
	.option nopic
	.attribute arch, "rv32i2p0_y2p0"
	.attribute unaligned_access, 0
	.attribute stack_align, 16
	.text
	.align	1
	.globl	_Z11iadd_i_yes1v
	.type	_Z11iadd_i_yes1v, @function
_Z11iadd_i_yes1v:
	SFPLOADI	L0, 4, 3
	SFPIADD	-5, L0, L0, 1
	SFPNOP
	SFPNOP
	SFPNOP
	SFPSTORE	L10, 0, 0
	SFPENCC	3, 10
	SFPNOP
	ret
	.size	_Z11iadd_i_yes1v, .-_Z11iadd_i_yes1v
	.align	1
	.globl	_Z12iadd_i_yes3cv
	.type	_Z12iadd_i_yes3cv, @function
_Z12iadd_i_yes3cv:
	SFPLOADI	L1, 4, 3
	SFPLOADI	L0, 4, 4
	SFPIADD	-6, L0, L0, 1
	SFPNOP
	SFPNOP
	SFPNOP
	SFPSETCC	0, L0, 6
	SFPNOP
	SFPIADD	-5, L1, L1, 1
	SFPNOP
	SFPNOP
	SFPNOP
	SFPSTORE	L10, 0, 0
	SFPENCC	3, 10
	SFPNOP
	ret
	.size	_Z12iadd_i_yes3cv, .-_Z12iadd_i_yes3cv
	.align	1
	.globl	_Z12iadd_i_yes4av
	.type	_Z12iadd_i_yes4av, @function
_Z12iadd_i_yes4av:
	SFPLOADI	L0, 4, 3
	SFPLOADI	L1, 4, 4
	SFPIADD	-5, L0, L0, 1
	SFPNOP
	SFPNOP
	SFPNOP
	SFPIADD	-6, L1, L0, 1
	SFPNOP
	SFPNOP
	SFPNOP
	SFPSETCC	0, L0, 6
	SFPNOP
	SFPSTORE	L10, 0, 0
	SFPENCC	3, 10
	SFPNOP
	ret
	.size	_Z12iadd_i_yes4av, .-_Z12iadd_i_yes4av
	.align	1
	.globl	_Z12iadd_i_yes4bv
	.type	_Z12iadd_i_yes4bv, @function
_Z12iadd_i_yes4bv:
	SFPLOADI	L0, 4, 3
	SFPLOADI	L1, 4, 4
	SFPIADD	-5, L0, L0, 5
	SFPNOP
	SFPNOP
	SFPIADD	-6, L1, L1, 1
	SFPNOP
	SFPNOP
	SFPNOP
	SFPSETCC	0, L1, 6
	SFPNOP
	SFPIADD	0, L0, L0, 1
	SFPNOP
	SFPNOP
	SFPNOP
	SFPSTORE	L10, 0, 0
	SFPENCC	3, 10
	SFPNOP
	ret
	.size	_Z12iadd_i_yes4bv, .-_Z12iadd_i_yes4bv
	.align	1
	.globl	_Z11iadd_i_yes5v
	.type	_Z11iadd_i_yes5v, @function
_Z11iadd_i_yes5v:
	SFPLOADI	L0, 4, 3
	SFPIADD	5, L0, L0, 9
	SFPNOP
	SFPNOP
	SFPNOP
	SFPSTORE	L10, 0, 0
	SFPENCC	3, 10
	SFPNOP
	ret
	.size	_Z11iadd_i_yes5v, .-_Z11iadd_i_yes5v
	.align	1
	.globl	_Z11iadd_i_yes6v
	.type	_Z11iadd_i_yes6v, @function
_Z11iadd_i_yes6v:
	SFPLOADI	L0, 4, 3
	SFPLOADI	L1, 4, 4
	SFPIADD	6, L1, L0, 9
	SFPNOP
	SFPNOP
	SFPNOP
	SFPSTORE	L10, 0, 0
	SFPENCC	3, 10
	SFPNOP
	ret
	.size	_Z11iadd_i_yes6v, .-_Z11iadd_i_yes6v
	.align	1
	.globl	_Z11iadd_i_yes8v
	.type	_Z11iadd_i_yes8v, @function
_Z11iadd_i_yes8v:
	SFPLOADI	L1, 4, 3
	SFPIADD	5, L1, L0, 1
	SFPNOP
	SFPNOP
	SFPNOP
	SFPSTORE	L10, 0, 0
	SFPENCC	3, 10
	SFPNOP
	SFPSTORE	L1, 2, 0
	SFPSTORE	L0, 2, 0
	ret
	.size	_Z11iadd_i_yes8v, .-_Z11iadd_i_yes8v
	.align	1
	.globl	_Z10iadd_i_no1v
	.type	_Z10iadd_i_no1v, @function
_Z10iadd_i_no1v:
	SFPLOADI	L0, 4, 3
	SFPLOADI	L1, 4, 4
	SFPIADD	-6, L1, L1, 5
	SFPNOP
	SFPNOP
	SFPIADD	0, L0, L0, 1
	SFPNOP
	SFPNOP
	SFPNOP
	SFPSTORE	L10, 0, 0
	SFPENCC	3, 10
	SFPNOP
	ret
	.size	_Z10iadd_i_no1v, .-_Z10iadd_i_no1v
	.align	1
	.globl	_Z10iadd_i_no2v
	.type	_Z10iadd_i_no2v, @function
_Z10iadd_i_no2v:
	SFPLOADI	L0, 4, 3
	SFPIADD	-5, L0, L1, 5
	SFPNOP
	SFPNOP
	SFPIADD	0, L0, L0, 1
	SFPNOP
	SFPNOP
	SFPNOP
	SFPSTORE	L10, 0, 0
	SFPENCC	3, 10
	SFPNOP
	ret
	.size	_Z10iadd_i_no2v, .-_Z10iadd_i_no2v
	.align	1
	.globl	_Z10iadd_i_no4v
	.type	_Z10iadd_i_no4v, @function
_Z10iadd_i_no4v:
	SFPLOADI	L1, 4, 3
	SFPLOADI	L0, 4, 4
	SFPIADD	-5, L1, L1, 5
	SFPNOP
	SFPNOP
	SFPIADD	-7, L1, L1, 5
	SFPNOP
	SFPNOP
	SFPIADD	-6, L0, L0, 5
	SFPNOP
	SFPNOP
	SFPIADD	-9, L0, L0, 5
	SFPNOP
	SFPNOP
	SFPSTORE	L0, 2, 0
	ret
	.size	_Z10iadd_i_no4v, .-_Z10iadd_i_no4v
	.align	1
	.globl	_Z10iadd_i_no5v
	.type	_Z10iadd_i_no5v, @function
_Z10iadd_i_no5v:
	SFPLOADI	L1, 4, 3
	SFPLOADI	L0, 4, 4
	SFPIADD	0, L1, L1, 1
	SFPNOP
	SFPNOP
	SFPNOP
	SFPIADD	0, L0, L0, 1
	SFPNOP
	SFPNOP
	SFPNOP
	SFPSTORE	L10, 0, 0
	SFPENCC	3, 10
	SFPNOP
	ret
	.size	_Z10iadd_i_no5v, .-_Z10iadd_i_no5v
	.align	1
	.globl	_Z10iadd_i_no6v
	.type	_Z10iadd_i_no6v, @function
_Z10iadd_i_no6v:
	SFPLOADI	L0, 4, 3
	SFPIADD	5, L0, L0, 5
	SFPNOP
	SFPNOP
	SFPIADD	0, L0, L0, 5
	SFPNOP
	SFPNOP
	ret
	.size	_Z10iadd_i_no6v, .-_Z10iadd_i_no6v
	.align	1
	.globl	_Z11iadd_v_yes1v
	.type	_Z11iadd_v_yes1v, @function
_Z11iadd_v_yes1v:
	SFPLOADI	L0, 4, 3
	SFPLOADI	L1, 4, 4
	SFPIADD	0, L1, L0, 2
	SFPNOP
	SFPNOP
	SFPNOP
	SFPSTORE	L10, 0, 0
	SFPENCC	3, 10
	SFPNOP
	ret
	.size	_Z11iadd_v_yes1v, .-_Z11iadd_v_yes1v
	.align	1
	.globl	_Z11iadd_v_yes2v
	.type	_Z11iadd_v_yes2v, @function
_Z11iadd_v_yes2v:
	SFPLOADI	L1, 4, 3
	SFPLOADI	L0, 4, 4
	SFPIADD	0, L1, L0, 10
	SFPNOP
	SFPNOP
	SFPNOP
	SFPSTORE	L10, 0, 0
	SFPENCC	3, 10
	SFPNOP
	ret
	.size	_Z11iadd_v_yes2v, .-_Z11iadd_v_yes2v
	.align	1
	.globl	_Z11iadd_v_yes3v
	.type	_Z11iadd_v_yes3v, @function
_Z11iadd_v_yes3v:
	SFPLOADI	L1, 4, 3
	SFPLOADI	L0, 4, 4
	SFPIADD	0, L1, L0, 0
	SFPNOP
	SFPNOP
	SFPNOP
	SFPSTORE	L10, 0, 0
	SFPENCC	3, 10
	SFPNOP
	ret
	.size	_Z11iadd_v_yes3v, .-_Z11iadd_v_yes3v
	.align	1
	.globl	_Z12iadd_v_yes4av
	.type	_Z12iadd_v_yes4av, @function
_Z12iadd_v_yes4av:
	SFPLOADI	L1, 4, 3
	SFPLOADI	L0, 4, 4
	SFPIADD	0, L1, L0, 10
	SFPNOP
	SFPNOP
	SFPNOP
	SFPSTORE	L10, 0, 0
	SFPENCC	3, 10
	SFPNOP
	ret
	.size	_Z12iadd_v_yes4av, .-_Z12iadd_v_yes4av
	.align	1
	.globl	_Z12iadd_v_yes4bv
	.type	_Z12iadd_v_yes4bv, @function
_Z12iadd_v_yes4bv:
	SFPLOADI	L1, 4, 3
	SFPLOADI	L0, 4, 4
	SFPIADD	0, L1, L0, 0
	SFPNOP
	SFPNOP
	SFPNOP
	SFPSTORE	L10, 0, 0
	SFPENCC	3, 10
	SFPNOP
	ret
	.size	_Z12iadd_v_yes4bv, .-_Z12iadd_v_yes4bv
	.align	1
	.globl	_Z10iadd_v_no1v
	.type	_Z10iadd_v_no1v, @function
_Z10iadd_v_no1v:
	SFPLOADI	L0, 4, 3
	SFPLOADI	L1, 4, 4
	SFPIADD	0, L1, L0, 4
	SFPNOP
	SFPNOP
	SFPIADD	-5, L0, L0, 1
	SFPNOP
	SFPNOP
	SFPNOP
	SFPSTORE	L10, 0, 0
	SFPENCC	3, 10
	SFPNOP
	ret
	.size	_Z10iadd_v_no1v, .-_Z10iadd_v_no1v
	.align	1
	.globl	_Z10iadd_v_no2v
	.type	_Z10iadd_v_no2v, @function
_Z10iadd_v_no2v:
	SFPLOADI	L1, 4, 3
	SFPLOADI	L0, 4, 4
	SFPIADD	0, L1, L0, 6
	SFPNOP
	SFPNOP
	SFPIADD	-5, L0, L0, 1
	SFPNOP
	SFPNOP
	SFPNOP
	SFPSTORE	L10, 0, 0
	SFPENCC	3, 10
	SFPNOP
	ret
	.size	_Z10iadd_v_no2v, .-_Z10iadd_v_no2v
	.align	1
	.globl	_Z10iadd_v_no3v
	.type	_Z10iadd_v_no3v, @function
_Z10iadd_v_no3v:
	SFPLOADI	L1, 4, 3
	SFPLOADI	L0, 4, 4
	SFPIADD	0, L1, L0, 6
	SFPNOP
	SFPNOP
	SFPIADD	0, L0, L0, 5
	SFPNOP
	SFPNOP
	ret
	.size	_Z10iadd_v_no3v, .-_Z10iadd_v_no3v
	.align	1
	.globl	_Z10iadd_v_no4v
	.type	_Z10iadd_v_no4v, @function
_Z10iadd_v_no4v:
	SFPLOADI	L1, 4, 3
	SFPLOADI	L0, 4, 4
	SFPIADD	0, L1, L0, 2
	SFPNOP
	SFPNOP
	SFPNOP
	SFPIADD	-5, L1, L1, 5
	SFPNOP
	SFPNOP
	SFPSTORE	L10, 0, 0
	SFPENCC	3, 10
	SFPNOP
	ret
	.size	_Z10iadd_v_no4v, .-_Z10iadd_v_no4v
	.align	1
	.globl	_Z7lz_yes1v
	.type	_Z7lz_yes1v, @function
_Z7lz_yes1v:
	SFPLOADI	L0, 0, 16448
	SFPLZ	L0, L1, 10
	SFPNOP
	SFPNOP
	SFPNOP
	SFPSTORE	L10, 0, 0
	SFPENCC	3, 10
	SFPNOP
	SFPSTORE	L0, 0, 0
	SFPSTORE	L1, 2, 0
	ret
	.size	_Z7lz_yes1v, .-_Z7lz_yes1v
	.align	1
	.globl	_Z7lz_yes2v
	.type	_Z7lz_yes2v, @function
_Z7lz_yes2v:
	SFPLOADI	L0, 0, 16448
	SFPLZ	L0, L1, 2
	SFPNOP
	SFPNOP
	SFPNOP
	SFPSTORE	L10, 0, 0
	SFPENCC	3, 10
	SFPNOP
	SFPSTORE	L0, 0, 0
	SFPSTORE	L1, 2, 0
	ret
	.size	_Z7lz_yes2v, .-_Z7lz_yes2v
	.align	1
	.globl	_Z6lz_no1v
	.type	_Z6lz_no1v, @function
_Z6lz_no1v:
	SFPLOADI	L1, 0, 16448
	SFPLZ	L1, L0, 0
	SFPNOP
	SFPNOP
	SFPSETCC	0, L0, 2
	SFPNOP
	SFPSTORE	L10, 0, 0
	SFPENCC	3, 10
	SFPNOP
	SFPSTORE	L1, 0, 0
	SFPSTORE	L0, 2, 0
	ret
	.size	_Z6lz_no1v, .-_Z6lz_no1v
	.align	1
	.globl	_Z6lz_no2v
	.type	_Z6lz_no2v, @function
_Z6lz_no2v:
	SFPLOADI	L0, 0, 16448
	SFPLZ	L0, L1, 0
	SFPNOP
	SFPNOP
	SFPSETCC	0, L0, 4
	SFPNOP
	SFPSTORE	L10, 0, 0
	SFPENCC	3, 10
	SFPNOP
	SFPSTORE	L0, 0, 0
	SFPSTORE	L1, 2, 0
	ret
	.size	_Z6lz_no2v, .-_Z6lz_no2v
	.align	1
	.globl	_Z6lz_no3v
	.type	_Z6lz_no3v, @function
_Z6lz_no3v:
	SFPLOADI	L0, 0, 16448
	SFPLZ	L0, L1, 0
	SFPNOP
	SFPNOP
	SFPSETCC	0, L0, 0
	SFPNOP
	SFPSTORE	L10, 0, 0
	SFPENCC	3, 10
	SFPNOP
	SFPSTORE	L0, 0, 0
	SFPSTORE	L1, 2, 0
	ret
	.size	_Z6lz_no3v, .-_Z6lz_no3v
	.align	1
	.globl	_Z10exexp_yes1v
	.type	_Z10exexp_yes1v, @function
_Z10exexp_yes1v:
	SFPLOADI	L1, 0, 16448
	SFPEXEXP	L1, L0, 10
	SFPNOP
	SFPNOP
	SFPNOP
	SFPSTORE	L10, 0, 0
	SFPENCC	3, 10
	SFPNOP
	SFPSTORE	L1, 0, 0
	SFPSTORE	L0, 2, 0
	ret
	.size	_Z10exexp_yes1v, .-_Z10exexp_yes1v
	.align	1
	.globl	_Z10exexp_yes2v
	.type	_Z10exexp_yes2v, @function
_Z10exexp_yes2v:
	SFPLOADI	L1, 0, 16448
	SFPEXEXP	L1, L0, 2
	SFPNOP
	SFPNOP
	SFPNOP
	SFPSTORE	L10, 0, 0
	SFPENCC	3, 10
	SFPNOP
	SFPSTORE	L1, 0, 0
	SFPSTORE	L0, 2, 0
	ret
	.size	_Z10exexp_yes2v, .-_Z10exexp_yes2v
	.align	1
	.globl	_Z10exexp_yes3v
	.type	_Z10exexp_yes3v, @function
_Z10exexp_yes3v:
	SFPLOADI	L0, 0, 16448
	SFPEXEXP	L0, L0, 2
	SFPNOP
	SFPNOP
	SFPNOP
	SFPSTORE	L10, 0, 0
	SFPENCC	3, 10
	SFPNOP
	SFPSTORE	L0, 0, 0
	ret
	.size	_Z10exexp_yes3v, .-_Z10exexp_yes3v
	.align	1
	.globl	_Z9exexp_no1v
	.type	_Z9exexp_no1v, @function
_Z9exexp_no1v:
	SFPLOADI	L1, 0, 16448
	SFPEXEXP	L1, L0, 0
	SFPNOP
	SFPNOP
	SFPIADD	-5, L0, L2, 1
	SFPNOP
	SFPNOP
	SFPNOP
	SFPSTORE	L10, 0, 0
	SFPENCC	3, 10
	SFPNOP
	SFPSTORE	L1, 0, 0
	SFPSTORE	L0, 2, 0
	ret
	.size	_Z9exexp_no1v, .-_Z9exexp_no1v
	.align	1
	.globl	_Z9exexp_no2v
	.type	_Z9exexp_no2v, @function
_Z9exexp_no2v:
	SFPLOADI	L1, 0, 16448
	SFPEXEXP	L1, L0, 1
	SFPNOP
	SFPNOP
	SFPIADD	0, L0, L2, 9
	SFPNOP
	SFPNOP
	SFPNOP
	SFPSTORE	L10, 0, 0
	SFPENCC	3, 10
	SFPNOP
	SFPSTORE	L1, 0, 0
	SFPSTORE	L0, 2, 0
	ret
	.size	_Z9exexp_no2v, .-_Z9exexp_no2v
	.globl	_ZN7ckernel13instrn_bufferE
	.section	.sbss,"aw",@nobits
	.align	2
	.type	_ZN7ckernel13instrn_bufferE, @object
	.size	_ZN7ckernel13instrn_bufferE, 4
_ZN7ckernel13instrn_bufferE:
	.zero	4
	.ident	"GCC: (GNU) 10.2.0"
