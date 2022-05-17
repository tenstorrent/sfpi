	.file	"nonimm.cc"
	.option nopic
	.attribute arch, "rv32i2p0_w2p0"
	.attribute unaligned_access, 0
	.attribute stack_align, 16
	.text
	.align	1
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
	.align	1
	.globl	_Z13macro_checkedi
	.type	_Z13macro_checkedi, @function
_Z13macro_checkedi:
	lui	a5,%hi(_ZN7ckernel13instrn_bufferE)
	lw	a5,%lo(_ZN7ckernel13instrn_bufferE)(a5)
	slli	a3,a0,16
	srli	a3,a3,16
	li	a4,1881538560
	add	a4,a3,a4
	li	a2,1898315776
	sw	a4,0(a5)
	add	a2,a3,a2
	sw	a2,0(a5)
	li	a2,1915092992
	add	a3,a3,a2
	sw	a3,0(a5)
	slli	a1,a0,12
	li	a4,16773120
	li	a3,2030043136
	and	a4,a1,a4
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
	.align	1
	.globl	_Z19macro_reuse_checkedi
	.type	_Z19macro_reuse_checkedi, @function
_Z19macro_reuse_checkedi:
	lui	a5,%hi(_ZN7ckernel13instrn_bufferE)
	li	a1,65536
	lw	a5,%lo(_ZN7ckernel13instrn_bufferE)(a5)
	addi	a1,a1,-1
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
	.align	1
	.globl	_Z8load_imms
	.type	_Z8load_imms, @function
_Z8load_imms:
	SFPLOAD	L3, 1, 0, 10
	SFPLOAD	L2, 1, 0, 20
	SFPLOAD	L1, 1, 0, 30
	SFPLOAD	L0, 1, 0, 40
	SFPSTORE	L3, 0, 2, 1
	SFPSTORE	L2, 0, 2, 10
	SFPSTORE	L1, 0, 2, 20
	SFPSTORE	L0, 0, 2, 30
	ret
	.size	_Z8load_imms, .-_Z8load_imms
	.align	1
	.globl	_Z8load_regs
	.type	_Z8load_regs, @function
_Z8load_regs:
	lui	a5,%hi(_ZN7ckernel13instrn_bufferE)
	lw	a5,%lo(_ZN7ckernel13instrn_bufferE)(a5)
	slli	a4,a0,16
	srli	a4,a4,16
	li	a3,1882259456
	add	a3, a3, a4
# Op(0x70) - d(3)
	sw	a3,0(a5)
	addi	a4,a0,10
	slli	a4,a4,16
	srli	a4,a4,16
	li	a3,1881210880
	add	a3, a3, a4
# Op(0x70) - d(2)
	sw	a3,0(a5)
	addi	a4,a0,20
	slli	a4,a4,16
	srli	a4,a4,16
	li	a3,1880162304
	add	a3, a3, a4
# Op(0x70) - d(1)
	sw	a3,0(a5)
	addi	a0,a0,30
	slli	a0,a0,16
	srli	a0,a0,16
	li	a4,1879113728
	add	a4, a4, a0
# Op(0x70) - d(0)
	sw	a4,0(a5)
	SFPSTORE	L3, 0, 2, 1
	SFPSTORE	L2, 0, 2, 10
	SFPSTORE	L1, 0, 2, 20
	SFPSTORE	L0, 0, 2, 30
	ret
	.size	_Z8load_regs, .-_Z8load_regs
	.align	1
	.globl	_Z9load_boths
	.type	_Z9load_boths, @function
_Z9load_boths:
	SFPLOAD	L3, 1, 0, 10
	SFPLOAD	L2, 1, 0, 20
	lui	a5,%hi(_ZN7ckernel13instrn_bufferE)
	lw	a5,%lo(_ZN7ckernel13instrn_bufferE)(a5)
	slli	a4,a0,16
	srli	a4,a4,16
	li	a3,1880162304
	add	a3, a3, a4
# Op(0x70) - d(1)
	sw	a3,0(a5)
	addi	a0,a0,10
	slli	a0,a0,16
	srli	a0,a0,16
	li	a4,1879113728
	add	a4, a4, a0
# Op(0x70) - d(0)
	sw	a4,0(a5)
	SFPSTORE	L3, 0, 2, 1
	SFPSTORE	L2, 0, 2, 10
	SFPSTORE	L1, 0, 2, 20
	SFPSTORE	L0, 0, 2, 30
	ret
	.size	_Z9load_boths, .-_Z9load_boths
	.align	1
	.globl	_Z9loadi_imms
	.type	_Z9loadi_imms, @function
_Z9loadi_imms:
	SFPLOADI	L3, 0, 10
	SFPLOADI	L2, 4, 10
	SFPLOADI	L1, 8, 20
	SFPLOADI	L0, 10, 30
	SFPSTORE	L3, 0, 2, 1
	SFPSTORE	L2, 0, 2, 10
	SFPSTORE	L1, 0, 2, 20
	SFPSTORE	L0, 0, 2, 30
	ret
	.size	_Z9loadi_imms, .-_Z9loadi_imms
	.align	1
	.globl	_Z9loadi_regs
	.type	_Z9loadi_regs, @function
_Z9loadi_regs:
	lui	a5,%hi(_ZN7ckernel13instrn_bufferE)
	lw	a5,%lo(_ZN7ckernel13instrn_bufferE)(a5)
	slli	a4,a0,16
	srli	a4,a4,16
	li	a3,1898971136
	add	a3, a3, a4
# Op(0x71) - d(3)
	sw	a3,0(a5)
	addi	a4,a0,1
	slli	a4,a4,16
	srli	a4,a4,16
	li	a3,1898184704
	add	a3, a3, a4
# Op(0x71) - d(2)
	sw	a3,0(a5)
	addi	a4,a0,2
	slli	a4,a4,16
	srli	a4,a4,16
	li	a3,1897398272
	add	a3, a3, a4
# Op(0x71) - d(1)
	sw	a3,0(a5)
	addi	a0,a0,3
	slli	a0,a0,16
	srli	a0,a0,16
	li	a4,1896611840
	add	a4, a4, a0
# Op(0x71) - d(0)
	sw	a4,0(a5)
	SFPSTORE	L3, 0, 2, 1
	SFPSTORE	L2, 0, 2, 10
	SFPSTORE	L1, 0, 2, 20
	SFPSTORE	L0, 0, 2, 30
	ret
	.size	_Z9loadi_regs, .-_Z9loadi_regs
	.align	1
	.globl	_Z10loadi_boths
	.type	_Z10loadi_boths, @function
_Z10loadi_boths:
	SFPLOADI	L3, 0, 10
	SFPLOADI	L2, 4, 20
	lui	a5,%hi(_ZN7ckernel13instrn_bufferE)
	lw	a5,%lo(_ZN7ckernel13instrn_bufferE)(a5)
	slli	a4,a0,16
	srli	a4,a4,16
	li	a3,1897398272
	add	a3, a3, a4
# Op(0x71) - d(1)
	sw	a3,0(a5)
	addi	a0,a0,1
	slli	a0,a0,16
	srli	a0,a0,16
	li	a4,1896611840
	add	a4, a4, a0
# Op(0x71) - d(0)
	sw	a4,0(a5)
	SFPSTORE	L3, 0, 2, 1
	SFPSTORE	L2, 0, 2, 10
	SFPSTORE	L1, 0, 2, 20
	SFPSTORE	L0, 0, 2, 30
	ret
	.size	_Z10loadi_boths, .-_Z10loadi_boths
	.align	1
	.globl	_Z9store_imms
	.type	_Z9store_imms, @function
_Z9store_imms:
	SFPLOAD	L0, 1, 0, 10
	SFPSTORE	L0, 0, 2, 10
	SFPSTORE	L0, 0, 2, 20
	SFPSTORE	L0, 0, 2, 30
	SFPSTORE	L0, 0, 2, 40
	ret
	.size	_Z9store_imms, .-_Z9store_imms
	.align	1
	.globl	_Z9store_regs
	.type	_Z9store_regs, @function
_Z9store_regs:
	SFPLOAD	L0, 1, 0, 10
	lui	a5,%hi(_ZN7ckernel13instrn_bufferE)
	lw	a5,%lo(_ZN7ckernel13instrn_bufferE)(a5)
	slli	a4,a0,16
	srli	a4,a4,16
	li	a3,1912635392
	add	a3, a3, a4
# Op(0x72) s(0)
	sw	a3,0(a5)
	addi	a4,a0,1
	slli	a4,a4,16
	srli	a4,a4,16
	li	a3,1912733696
	add	a3, a3, a4
# Op(0x72) s(0)
	sw	a3,0(a5)
	addi	a4,a0,2
	slli	a4,a4,16
	srli	a4,a4,16
	li	a3,1912733696
	add	a3, a3, a4
# Op(0x72) s(0)
	sw	a3,0(a5)
	addi	a0,a0,3
	slli	a0,a0,16
	srli	a0,a0,16
	li	a4,1912733696
	add	a4, a4, a0
# Op(0x72) s(0)
	sw	a4,0(a5)
	ret
	.size	_Z9store_regs, .-_Z9store_regs
	.align	1
	.globl	_Z10store_boths
	.type	_Z10store_boths, @function
_Z10store_boths:
	SFPLOAD	L0, 1, 0, 10
	SFPSTORE	L0, 0, 2, 10
	SFPSTORE	L0, 0, 2, 20
	lui	a5,%hi(_ZN7ckernel13instrn_bufferE)
	lw	a4,%lo(_ZN7ckernel13instrn_bufferE)(a5)
	addi	a5,a0,1
	slli	a5,a5,16
	srli	a5,a5,16
	li	a3,1912733696
	add	a3, a3, a5
# Op(0x72) s(0)
	sw	a3,0(a4)
	addi	a0,a0,2
	slli	a0,a0,16
	srli	a0,a0,16
	li	a5,1912733696
	add	a5, a5, a0
# Op(0x72) s(0)
	sw	a5,0(a4)
	ret
	.size	_Z10store_boths, .-_Z10store_boths
	.align	1
	.globl	_Z11store_r_imms
	.type	_Z11store_r_imms, @function
_Z11store_r_imms:
	SFPSTORE	L13, 0, 2, 10
	SFPSTORE	L13, 0, 2, 20
	SFPSTORE	L13, 0, 2, 10
	SFPSTORE	L13, 0, 2, 20
	ret
	.size	_Z11store_r_imms, .-_Z11store_r_imms
	.align	1
	.globl	_Z11store_r_regs
	.type	_Z11store_r_regs, @function
_Z11store_r_regs:
	lui	a5,%hi(_ZN7ckernel13instrn_bufferE)
	lw	a5,%lo(_ZN7ckernel13instrn_bufferE)(a5)
	slli	a4,a0,16
	srli	a4,a4,16
	li	a3,1926266880
	add	a3, a3, a4
# Op(0x72) s(13)
	sw	a3,0(a5)
	addi	a4,a0,1
	slli	a4,a4,16
	srli	a4,a4,16
	li	a3,1926365184
	add	a3, a3, a4
# Op(0x72) s(13)
	sw	a3,0(a5)
	addi	a4,a0,2
	slli	a4,a4,16
	srli	a4,a4,16
	li	a3,1926365184
	add	a3, a3, a4
# Op(0x72) s(13)
	sw	a3,0(a5)
	addi	a0,a0,3
	slli	a0,a0,16
	srli	a0,a0,16
	li	a4,1926365184
	add	a4, a4, a0
# Op(0x72) s(13)
	sw	a4,0(a5)
	ret
	.size	_Z11store_r_regs, .-_Z11store_r_regs
	.align	1
	.globl	_Z12store_r_boths
	.type	_Z12store_r_boths, @function
_Z12store_r_boths:
	SFPSTORE	L13, 0, 2, 10
	SFPSTORE	L13, 0, 2, 20
	lui	a5,%hi(_ZN7ckernel13instrn_bufferE)
	lw	a5,%lo(_ZN7ckernel13instrn_bufferE)(a5)
	slli	a4,a0,16
	srli	a4,a4,16
	li	a3,1926266880
	add	a3, a3, a4
# Op(0x72) s(13)
	sw	a3,0(a5)
	addi	a0,a0,1
	slli	a0,a0,16
	srli	a0,a0,16
	li	a4,1926365184
	add	a4, a4, a0
# Op(0x72) s(13)
	sw	a4,0(a5)
	ret
	.size	_Z12store_r_boths, .-_Z12store_r_boths
	.align	1
	.globl	_Z8addi_imms
	.type	_Z8addi_imms, @function
_Z8addi_imms:
	SFPLOAD	L3, 1, 0, 10
	SFPLOAD	L2, 1, 0, 20
	SFPLOAD	L1, 1, 0, 30
	SFPLOAD	L0, 1, 0, 40
	SFPADDI	1, L3, 0
	SFPADDI	2, L2, 0
	SFPADDI	3, L1, 0
	SFPADDI	4, L0, 0
	SFPSTORE	L3, 0, 2, 1
	SFPSTORE	L2, 0, 2, 10
	SFPSTORE	L1, 0, 2, 20
	SFPSTORE	L0, 0, 2, 30
	ret
	.size	_Z8addi_imms, .-_Z8addi_imms
	.align	1
	.globl	_Z8addi_regs
	.type	_Z8addi_regs, @function
_Z8addi_regs:
	SFPLOAD	L3, 1, 0, 10
	SFPLOAD	L2, 1, 0, 20
	SFPLOAD	L1, 1, 0, 30
	SFPLOAD	L0, 1, 0, 40
	lui	a5,%hi(_ZN7ckernel13instrn_bufferE)
	lw	a4,%lo(_ZN7ckernel13instrn_bufferE)(a5)
	slli	a5,a0,16
	srli	a5,a5,8
	li	a3,1962934320
	add	a3, a3, a5
# Op(0x75) lv(3)  d(3)
	sw	a3,0(a4)
	SFPNOP
	SFPNOP
	addi	a5,a0,1
	slli	a5,a5,16
	srli	a5,a5,8
	li	a3,1962934304
	add	a3, a3, a5
# Op(0x75) lv(2)  d(2)
	sw	a3,0(a4)
	SFPNOP
	SFPNOP
	addi	a5,a0,2
	slli	a5,a5,16
	srli	a5,a5,8
	li	a3,1962934288
	add	a3, a3, a5
# Op(0x75) lv(1)  d(1)
	sw	a3,0(a4)
	SFPNOP
	SFPNOP
	addi	a0,a0,3
	slli	a0,a0,16
	srli	a0,a0,8
	li	a5,1962934272
	add	a5, a5, a0
# Op(0x75) lv(0)  d(0)
	sw	a5,0(a4)
	SFPNOP
	SFPNOP
	SFPSTORE	L3, 0, 2, 1
	SFPSTORE	L2, 0, 2, 10
	SFPSTORE	L1, 0, 2, 20
	SFPSTORE	L0, 0, 2, 30
	ret
	.size	_Z8addi_regs, .-_Z8addi_regs
	.align	1
	.globl	_Z9addi_boths
	.type	_Z9addi_boths, @function
_Z9addi_boths:
	SFPLOAD	L3, 1, 0, 10
	SFPLOAD	L2, 1, 0, 20
	SFPLOAD	L1, 1, 0, 30
	SFPLOAD	L0, 1, 0, 40
	SFPADDI	1, L3, 0
	SFPADDI	2, L2, 0
	lui	a5,%hi(_ZN7ckernel13instrn_bufferE)
	lw	a4,%lo(_ZN7ckernel13instrn_bufferE)(a5)
	slli	a5,a0,16
	srli	a5,a5,8
	li	a3,1962934288
	add	a3, a3, a5
# Op(0x75) lv(1)  d(1)
	sw	a3,0(a4)
	SFPNOP
	SFPNOP
	addi	a0,a0,1
	slli	a0,a0,16
	srli	a0,a0,8
	li	a5,1962934272
	add	a5, a5, a0
# Op(0x75) lv(0)  d(0)
	sw	a5,0(a4)
	SFPNOP
	SFPNOP
	SFPSTORE	L3, 0, 2, 1
	SFPSTORE	L2, 0, 2, 10
	SFPSTORE	L1, 0, 2, 20
	SFPSTORE	L0, 0, 2, 30
	ret
	.size	_Z9addi_boths, .-_Z9addi_boths
	.align	1
	.globl	_Z10iadd_i_imms
	.type	_Z10iadd_i_imms, @function
_Z10iadd_i_imms:
	SFPLOAD	L3, 1, 0, 10
	SFPLOAD	L2, 1, 0, 20
	SFPLOAD	L1, 1, 0, 30
	SFPLOAD	L0, 1, 0, 40
	SFPIADD	1, L3, L3, 5
	SFPIADD	2, L2, L2, 5
	SFPIADD	3, L1, L1, 5
	SFPIADD	4, L0, L0, 5
	SFPSTORE	L3, 0, 2, 1
	SFPSTORE	L2, 0, 2, 10
	SFPSTORE	L1, 0, 2, 20
	SFPSTORE	L0, 0, 2, 30
	ret
	.size	_Z10iadd_i_imms, .-_Z10iadd_i_imms
	.align	1
	.globl	_Z10iadd_i_regs
	.type	_Z10iadd_i_regs, @function
_Z10iadd_i_regs:
	SFPLOAD	L3, 1, 0, 10
	SFPLOAD	L2, 1, 0, 20
	SFPLOAD	L1, 1, 0, 30
	SFPLOAD	L0, 1, 0, 40
	lui	a5,%hi(_ZN7ckernel13instrn_bufferE)
	lw	a4,%lo(_ZN7ckernel13instrn_bufferE)(a5)
	slli	a5,a0,20
	srli	a5,a5,8
	li	a3,2030043957
	add	a3, a3, a5
# Op(0x79) - d(3) s(3)
	sw	a3,0(a4)
	SFPNOP
	SFPNOP
	addi	a5,a0,1
	slli	a5,a5,20
	srli	a5,a5,8
	li	a3,2030043685
	add	a3, a3, a5
# Op(0x79) - d(2) s(2)
	sw	a3,0(a4)
	SFPNOP
	SFPNOP
	addi	a5,a0,2
	slli	a5,a5,20
	srli	a5,a5,8
	li	a3,2030043413
	add	a3, a3, a5
# Op(0x79) - d(1) s(1)
	sw	a3,0(a4)
	SFPNOP
	SFPNOP
	addi	a0,a0,3
	slli	a0,a0,20
	srli	a0,a0,8
	li	a5,2030043141
	add	a5, a5, a0
# Op(0x79) - d(0) s(0)
	sw	a5,0(a4)
	SFPNOP
	SFPNOP
	SFPSTORE	L3, 0, 2, 1
	SFPSTORE	L2, 0, 2, 10
	SFPSTORE	L1, 0, 2, 20
	SFPSTORE	L0, 0, 2, 30
	ret
	.size	_Z10iadd_i_regs, .-_Z10iadd_i_regs
	.align	1
	.globl	_Z11iadd_i_boths
	.type	_Z11iadd_i_boths, @function
_Z11iadd_i_boths:
	SFPLOAD	L3, 1, 0, 10
	SFPLOAD	L2, 1, 0, 20
	SFPLOAD	L1, 1, 0, 30
	SFPLOAD	L0, 1, 0, 40
	SFPIADD	1, L3, L3, 5
	SFPIADD	2, L2, L2, 5
	lui	a5,%hi(_ZN7ckernel13instrn_bufferE)
	lw	a4,%lo(_ZN7ckernel13instrn_bufferE)(a5)
	slli	a5,a0,20
	srli	a5,a5,8
	li	a3,2030043413
	add	a3, a3, a5
# Op(0x79) - d(1) s(1)
	sw	a3,0(a4)
	SFPNOP
	SFPNOP
	addi	a0,a0,1
	slli	a0,a0,20
	srli	a0,a0,8
	li	a5,2030043141
	add	a5, a5, a0
# Op(0x79) - d(0) s(0)
	sw	a5,0(a4)
	SFPNOP
	SFPNOP
	SFPSTORE	L3, 0, 2, 1
	SFPSTORE	L2, 0, 2, 10
	SFPSTORE	L1, 0, 2, 20
	SFPSTORE	L0, 0, 2, 30
	ret
	.size	_Z11iadd_i_boths, .-_Z11iadd_i_boths
	.align	1
	.globl	_Z13iadd_i_imm_exs
	.type	_Z13iadd_i_imm_exs, @function
_Z13iadd_i_imm_exs:
	SFPLOAD	L3, 1, 0, 10
	SFPLOAD	L2, 1, 0, 20
	SFPLOAD	L1, 1, 0, 30
	SFPLOAD	L0, 1, 0, 40
	SFPIADD	1, L3, L3, 5
	SFPIADD	2, L2, L2, 5
	SFPIADD	3, L1, L1, 5
	SFPIADD	4, L0, L0, 5
	SFPSTORE	L3, 0, 2, 1
	SFPSTORE	L2, 0, 2, 10
	SFPSTORE	L1, 0, 2, 20
	SFPSTORE	L0, 0, 2, 30
	ret
	.size	_Z13iadd_i_imm_exs, .-_Z13iadd_i_imm_exs
	.align	1
	.globl	_Z13iadd_i_reg_exs
	.type	_Z13iadd_i_reg_exs, @function
_Z13iadd_i_reg_exs:
	SFPLOAD	L1, 1, 0, 10
	SFPLOAD	L0, 1, 0, 20
	SFPLOAD	L3, 1, 0, 30
	lui	a5,%hi(_ZN7ckernel13instrn_bufferE)
	lw	a5,%lo(_ZN7ckernel13instrn_bufferE)(a5)
	slli	a4,a0,16
	srli	a4,a4,16
	li	a3,1898577920
	add	a3, a3, a4
# Op(0x71) - d(2)
	sw	a3,0(a5)
	srli	a4,a0,16
	li	a3,1898446848
	add	a3, a3, a4
# Op(0x71) lv(2)  d(2)
	sw	a3,0(a5)
	SFPIADD	0, L1, L2, 4
	addi	a4,a0,1
	slli	a3,a4,16
	srli	a3,a3,16
	li	a2,1897529344
	add	a2, a2, a3
# Op(0x71) - d(1)
	sw	a2,0(a5)
	srli	a4,a4,16
	li	a3,1897398272
	add	a3, a3, a4
# Op(0x71) lv(1)  d(1)
	sw	a3,0(a5)
	SFPIADD	0, L0, L1, 4
	addi	a0,a0,2
	slli	a4,a0,16
	srli	a4,a4,16
	li	a3,1896480768
	add	a3, a3, a4
# Op(0x71) - d(0)
	sw	a3,0(a5)
	srli	a0,a0,16
	li	a4,1896349696
	add	a4, a4, a0
# Op(0x71) lv(0)  d(0)
	sw	a4,0(a5)
	SFPIADD	0, L3, L0, 4
	SFPSTORE	L2, 0, 2, 1
	SFPSTORE	L1, 0, 2, 10
	SFPSTORE	L0, 0, 2, 20
	ret
	.size	_Z13iadd_i_reg_exs, .-_Z13iadd_i_reg_exs
	.align	1
	.globl	_Z14iadd_i_both_exs
	.type	_Z14iadd_i_both_exs, @function
_Z14iadd_i_both_exs:
	SFPLOAD	L2, 1, 0, 10
	SFPLOAD	L1, 1, 0, 20
	SFPLOAD	L3, 1, 0, 30
	SFPIADD	1, L2, L2, 5
	SFPIADD	2, L1, L1, 5
	lui	a5,%hi(_ZN7ckernel13instrn_bufferE)
	lw	a5,%lo(_ZN7ckernel13instrn_bufferE)(a5)
	slli	a4,a0,16
	srli	a4,a4,16
	li	a3,1896480768
	add	a3, a3, a4
# Op(0x71) - d(0)
	sw	a3,0(a5)
	srli	a0,a0,16
	li	a4,1896349696
	add	a4, a4, a0
# Op(0x71) lv(0)  d(0)
	sw	a4,0(a5)
	SFPIADD	0, L3, L0, 4
	SFPSTORE	L2, 0, 2, 1
	SFPSTORE	L1, 0, 2, 10
	SFPSTORE	L0, 0, 2, 20
	ret
	.size	_Z14iadd_i_both_exs, .-_Z14iadd_i_both_exs
	.align	1
	.globl	_Z10shft_i_imms
	.type	_Z10shft_i_imms, @function
_Z10shft_i_imms:
	SFPLOAD	L3, 1, 0, 10
	SFPLOAD	L2, 1, 0, 20
	SFPLOAD	L1, 1, 0, 30
	SFPLOAD	L0, 1, 0, 40
	SFPSHFT	1, L0, L3, 1
	SFPSHFT	2, L0, L2, 1
	SFPSHFT	3, L0, L1, 1
	SFPSHFT	4, L0, L0, 1
	SFPSTORE	L3, 0, 2, 1
	SFPSTORE	L2, 0, 2, 10
	SFPSTORE	L1, 0, 2, 20
	SFPSTORE	L0, 0, 2, 30
	ret
	.size	_Z10shft_i_imms, .-_Z10shft_i_imms
	.align	1
	.globl	_Z10shft_i_regs
	.type	_Z10shft_i_regs, @function
_Z10shft_i_regs:
	SFPLOAD	L3, 1, 0, 10
	SFPLOAD	L2, 1, 0, 20
	SFPLOAD	L1, 1, 0, 30
	SFPLOAD	L0, 1, 0, 40
	lui	a5,%hi(_ZN7ckernel13instrn_bufferE)
	lw	a4,%lo(_ZN7ckernel13instrn_bufferE)(a5)
	slli	a5,a0,20
	srli	a5,a5,8
	li	a3,2046820401
	add	a3, a3, a5
# Op(0x7a) lv(3)  d(3)
	sw	a3,0(a4)
	SFPNOP
	SFPNOP
	addi	a5,a0,1
	slli	a5,a5,20
	srli	a5,a5,8
	li	a3,2046820385
	add	a3, a3, a5
# Op(0x7a) lv(2)  d(2)
	sw	a3,0(a4)
	SFPNOP
	SFPNOP
	addi	a5,a0,2
	slli	a5,a5,20
	srli	a5,a5,8
	li	a3,2046820369
	add	a3, a3, a5
# Op(0x7a) lv(1)  d(1)
	sw	a3,0(a4)
	SFPNOP
	SFPNOP
	addi	a0,a0,3
	slli	a0,a0,20
	srli	a0,a0,8
	li	a5,2046820353
	add	a5, a5, a0
# Op(0x7a) lv(0)  d(0)
	sw	a5,0(a4)
	SFPNOP
	SFPNOP
	SFPSTORE	L3, 0, 2, 1
	SFPSTORE	L2, 0, 2, 10
	SFPSTORE	L1, 0, 2, 20
	SFPSTORE	L0, 0, 2, 30
	ret
	.size	_Z10shft_i_regs, .-_Z10shft_i_regs
	.align	1
	.globl	_Z11shft_i_boths
	.type	_Z11shft_i_boths, @function
_Z11shft_i_boths:
	SFPLOAD	L3, 1, 0, 10
	SFPLOAD	L2, 1, 0, 20
	SFPLOAD	L1, 1, 0, 30
	SFPLOAD	L0, 1, 0, 40
	SFPSHFT	1, L0, L3, 1
	SFPSHFT	2, L0, L2, 1
	lui	a5,%hi(_ZN7ckernel13instrn_bufferE)
	lw	a4,%lo(_ZN7ckernel13instrn_bufferE)(a5)
	slli	a5,a0,20
	srli	a5,a5,8
	li	a3,2046820369
	add	a3, a3, a5
# Op(0x7a) lv(1)  d(1)
	sw	a3,0(a4)
	SFPNOP
	SFPNOP
	addi	a0,a0,1
	slli	a0,a0,20
	srli	a0,a0,8
	li	a5,2046820353
	add	a5, a5, a0
# Op(0x7a) lv(0)  d(0)
	sw	a5,0(a4)
	SFPNOP
	SFPNOP
	SFPSTORE	L3, 0, 2, 1
	SFPSTORE	L2, 0, 2, 10
	SFPSTORE	L1, 0, 2, 20
	SFPSTORE	L0, 0, 2, 30
	ret
	.size	_Z11shft_i_boths, .-_Z11shft_i_boths
	.align	1
	.globl	_Z9divp2_imms
	.type	_Z9divp2_imms, @function
_Z9divp2_imms:
	SFPLOAD	L0, 1, 0, 10
	SFPLOAD	L2, 1, 0, 20
	SFPLOAD	L0, 1, 0, 30
	SFPLOAD	L1, 1, 0, 40
	SFPDIVP2	26, L0, L3, 0
	SFPDIVP2	42, L2, L2, 1
	SFPDIVP2	58, L0, L0, 0
	SFPDIVP2	74, L1, L1, 1
	SFPSTORE	L3, 0, 2, 1
	SFPSTORE	L2, 0, 2, 10
	SFPSTORE	L0, 0, 2, 20
	SFPSTORE	L1, 0, 2, 30
	ret
	.size	_Z9divp2_imms, .-_Z9divp2_imms
	.align	1
	.globl	_Z9divp2_regs
	.type	_Z9divp2_regs, @function
_Z9divp2_regs:
	SFPLOAD	L0, 1, 0, 10
	SFPLOAD	L2, 1, 0, 20
	SFPLOAD	L1, 1, 0, 30
	SFPLOAD	L0, 1, 0, 40
	lui	a5,%hi(_ZN7ckernel13instrn_bufferE)
	lw	a4,%lo(_ZN7ckernel13instrn_bufferE)(a5)
	slli	a5,a0,20
	srli	a5,a5,8
	li	a3,1979711536
	add	a3, a3, a5
# Op(0x76) - d(3) s(0)
	sw	a3,0(a4)
	SFPNOP
	SFPNOP
	addi	a5,a0,1
	slli	a5,a5,20
	srli	a5,a5,8
	li	a3,1979712033
	add	a3, a3, a5
# Op(0x76) - d(2) s(2)
	sw	a3,0(a4)
	SFPNOP
	SFPNOP
	addi	a5,a0,2
	slli	a5,a5,20
	srli	a5,a5,8
	li	a3,1979711760
	add	a3, a3, a5
# Op(0x76) - d(1) s(1)
	sw	a3,0(a4)
	SFPNOP
	SFPNOP
	addi	a0,a0,3
	slli	a0,a0,20
	srli	a0,a0,8
	li	a5,1979711489
	add	a5, a5, a0
# Op(0x76) - d(0) s(0)
	sw	a5,0(a4)
	SFPNOP
	SFPNOP
	SFPSTORE	L3, 0, 2, 1
	SFPSTORE	L2, 0, 2, 10
	SFPSTORE	L1, 0, 2, 20
	SFPSTORE	L0, 0, 2, 30
	ret
	.size	_Z9divp2_regs, .-_Z9divp2_regs
	.align	1
	.globl	_Z10divp2_boths
	.type	_Z10divp2_boths, @function
_Z10divp2_boths:
	SFPLOAD	L0, 1, 0, 10
	SFPLOAD	L2, 1, 0, 20
	SFPLOAD	L1, 1, 0, 30
	SFPLOAD	L0, 1, 0, 40
	SFPDIVP2	27, L0, L3, 0
	SFPDIVP2	27, L2, L2, 1
	lui	a5,%hi(_ZN7ckernel13instrn_bufferE)
	lw	a4,%lo(_ZN7ckernel13instrn_bufferE)(a5)
	slli	a5,a0,20
	srli	a5,a5,8
	li	a3,1979711760
	add	a3, a3, a5
# Op(0x76) - d(1) s(1)
	sw	a3,0(a4)
	SFPNOP
	SFPNOP
	addi	a0,a0,1
	slli	a0,a0,20
	srli	a0,a0,8
	li	a5,1979711489
	add	a5, a5, a0
# Op(0x76) - d(0) s(0)
	sw	a5,0(a4)
	SFPNOP
	SFPNOP
	SFPSTORE	L3, 0, 2, 1
	SFPSTORE	L2, 0, 2, 10
	SFPSTORE	L1, 0, 2, 20
	SFPSTORE	L0, 0, 2, 30
	ret
	.size	_Z10divp2_boths, .-_Z10divp2_boths
	.align	1
	.globl	_Z12setexp_i_imms
	.type	_Z12setexp_i_imms, @function
_Z12setexp_i_imms:
	SFPLOAD	L3, 1, 0, 10
	SFPLOAD	L2, 1, 0, 20
	SFPLOAD	L1, 1, 0, 30
	SFPLOAD	L0, 1, 0, 40
	SFPSETEXP	170, L3, L3, 1
	SFPSETEXP	170, L2, L2, 1
	SFPSETEXP	170, L1, L1, 1
	SFPSETEXP	170, L0, L0, 1
	SFPSTORE	L3, 0, 2, 1
	SFPSTORE	L2, 0, 2, 10
	SFPSTORE	L1, 0, 2, 20
	SFPSTORE	L0, 0, 2, 30
	ret
	.size	_Z12setexp_i_imms, .-_Z12setexp_i_imms
	.align	1
	.globl	_Z12setexp_i_regs
	.type	_Z12setexp_i_regs, @function
_Z12setexp_i_regs:
	SFPLOAD	L3, 1, 0, 10
	SFPLOAD	L2, 1, 0, 20
	SFPLOAD	L1, 1, 0, 30
	SFPLOAD	L0, 1, 0, 40
	lui	a5,%hi(_ZN7ckernel13instrn_bufferE)
	lw	a4,%lo(_ZN7ckernel13instrn_bufferE)(a5)
	slli	a5,a0,20
	srli	a5,a5,8
	li	a3,-2113928399
	add	a3, a3, a5
# Op(0xffffff82) - d(3) s(3)
	sw	a3,0(a4)
	SFPNOP
	SFPNOP
	addi	a5,a0,1
	slli	a5,a5,20
	srli	a5,a5,8
	li	a3,-2113928671
	add	a3, a3, a5
# Op(0xffffff82) - d(2) s(2)
	sw	a3,0(a4)
	SFPNOP
	SFPNOP
	addi	a5,a0,2
	slli	a5,a5,20
	srli	a5,a5,8
	li	a3,-2113928943
	add	a3, a3, a5
# Op(0xffffff82) - d(1) s(1)
	sw	a3,0(a4)
	SFPNOP
	SFPNOP
	addi	a0,a0,3
	slli	a0,a0,20
	srli	a0,a0,8
	li	a5,-2113929215
	add	a5, a5, a0
# Op(0xffffff82) - d(0) s(0)
	sw	a5,0(a4)
	SFPNOP
	SFPNOP
	SFPSTORE	L3, 0, 2, 1
	SFPSTORE	L2, 0, 2, 10
	SFPSTORE	L1, 0, 2, 20
	SFPSTORE	L0, 0, 2, 30
	ret
	.size	_Z12setexp_i_regs, .-_Z12setexp_i_regs
	.align	1
	.globl	_Z13setexp_i_boths
	.type	_Z13setexp_i_boths, @function
_Z13setexp_i_boths:
	SFPLOAD	L3, 1, 0, 10
	SFPLOAD	L2, 1, 0, 20
	SFPLOAD	L1, 1, 0, 30
	SFPLOAD	L0, 1, 0, 40
	SFPSETEXP	187, L3, L3, 1
	SFPSETEXP	187, L2, L2, 1
	lui	a5,%hi(_ZN7ckernel13instrn_bufferE)
	lw	a4,%lo(_ZN7ckernel13instrn_bufferE)(a5)
	slli	a5,a0,20
	srli	a5,a5,8
	li	a3,-2113928943
	add	a3, a3, a5
# Op(0xffffff82) - d(1) s(1)
	sw	a3,0(a4)
	SFPNOP
	SFPNOP
	addi	a0,a0,1
	slli	a0,a0,20
	srli	a0,a0,8
	li	a5,-2113929215
	add	a5, a5, a0
# Op(0xffffff82) - d(0) s(0)
	sw	a5,0(a4)
	SFPNOP
	SFPNOP
	SFPSTORE	L3, 0, 2, 1
	SFPSTORE	L2, 0, 2, 10
	SFPSTORE	L1, 0, 2, 20
	SFPSTORE	L0, 0, 2, 30
	ret
	.size	_Z13setexp_i_boths, .-_Z13setexp_i_boths
	.align	1
	.globl	_Z12setman_i_imms
	.type	_Z12setman_i_imms, @function
_Z12setman_i_imms:
	SFPLOAD	L3, 1, 0, 10
	SFPLOAD	L2, 1, 0, 20
	SFPLOAD	L1, 1, 0, 30
	SFPLOAD	L0, 1, 0, 40
	SFPSETMAN	170, L3, L3, 1
	SFPSETMAN	170, L2, L2, 1
	SFPSETMAN	170, L1, L1, 1
	SFPSETMAN	170, L0, L0, 1
	SFPSTORE	L3, 0, 2, 1
	SFPSTORE	L2, 0, 2, 10
	SFPSTORE	L1, 0, 2, 20
	SFPSTORE	L0, 0, 2, 30
	ret
	.size	_Z12setman_i_imms, .-_Z12setman_i_imms
	.align	1
	.globl	_Z12setman_i_regs
	.type	_Z12setman_i_regs, @function
_Z12setman_i_regs:
	SFPLOAD	L3, 1, 0, 10
	SFPLOAD	L2, 1, 0, 20
	SFPLOAD	L1, 1, 0, 30
	SFPLOAD	L0, 1, 0, 40
	lui	a5,%hi(_ZN7ckernel13instrn_bufferE)
	lw	a4,%lo(_ZN7ckernel13instrn_bufferE)(a5)
	slli	a5,a0,20
	srli	a5,a5,8
	li	a3,-2097151183
	add	a3, a3, a5
# Op(0xffffff83) - d(3) s(3)
	sw	a3,0(a4)
	SFPNOP
	SFPNOP
	addi	a5,a0,1
	slli	a5,a5,20
	srli	a5,a5,8
	li	a3,-2097151455
	add	a3, a3, a5
# Op(0xffffff83) - d(2) s(2)
	sw	a3,0(a4)
	SFPNOP
	SFPNOP
	addi	a5,a0,2
	slli	a5,a5,20
	srli	a5,a5,8
	li	a3,-2097151727
	add	a3, a3, a5
# Op(0xffffff83) - d(1) s(1)
	sw	a3,0(a4)
	SFPNOP
	SFPNOP
	addi	a0,a0,3
	slli	a0,a0,20
	srli	a0,a0,8
	li	a5,-2097151999
	add	a5, a5, a0
# Op(0xffffff83) - d(0) s(0)
	sw	a5,0(a4)
	SFPNOP
	SFPNOP
	SFPSTORE	L3, 0, 2, 1
	SFPSTORE	L2, 0, 2, 10
	SFPSTORE	L1, 0, 2, 20
	SFPSTORE	L0, 0, 2, 30
	ret
	.size	_Z12setman_i_regs, .-_Z12setman_i_regs
	.align	1
	.globl	_Z13setman_i_boths
	.type	_Z13setman_i_boths, @function
_Z13setman_i_boths:
	SFPLOAD	L3, 1, 0, 10
	SFPLOAD	L2, 1, 0, 20
	SFPLOAD	L1, 1, 0, 30
	SFPLOAD	L0, 1, 0, 40
	SFPSETMAN	187, L3, L3, 1
	SFPSETMAN	187, L2, L2, 1
	lui	a5,%hi(_ZN7ckernel13instrn_bufferE)
	lw	a4,%lo(_ZN7ckernel13instrn_bufferE)(a5)
	slli	a5,a0,20
	srli	a5,a5,8
	li	a3,-2097151727
	add	a3, a3, a5
# Op(0xffffff83) - d(1) s(1)
	sw	a3,0(a4)
	SFPNOP
	SFPNOP
	addi	a0,a0,1
	slli	a0,a0,20
	srli	a0,a0,8
	li	a5,-2097151999
	add	a5, a5, a0
# Op(0xffffff83) - d(0) s(0)
	sw	a5,0(a4)
	SFPNOP
	SFPNOP
	SFPSTORE	L3, 0, 2, 1
	SFPSTORE	L2, 0, 2, 10
	SFPSTORE	L1, 0, 2, 20
	SFPSTORE	L0, 0, 2, 30
	ret
	.size	_Z13setman_i_boths, .-_Z13setman_i_boths
	.align	1
	.globl	_Z12setsgn_i_imms
	.type	_Z12setsgn_i_imms, @function
_Z12setsgn_i_imms:
	SFPLOAD	L3, 1, 0, 10
	SFPLOAD	L2, 1, 0, 20
	SFPLOAD	L1, 1, 0, 30
	SFPLOAD	L0, 1, 0, 40
	SFPSETSGN	1962, L3, L3, 1
	SFPSETSGN	1962, L2, L2, 1
	SFPSETSGN	1962, L1, L1, 1
	SFPSETSGN	1962, L0, L0, 1
	SFPSTORE	L3, 0, 2, 1
	SFPSTORE	L2, 0, 2, 10
	SFPSTORE	L1, 0, 2, 20
	SFPSTORE	L0, 0, 2, 30
	ret
	.size	_Z12setsgn_i_imms, .-_Z12setsgn_i_imms
	.align	1
	.globl	_Z12setsgn_i_regj
	.type	_Z12setsgn_i_regj, @function
_Z12setsgn_i_regj:
	SFPLOAD	L3, 1, 0, 10
	SFPLOAD	L2, 1, 0, 20
	SFPLOAD	L1, 1, 0, 30
	SFPLOAD	L0, 1, 0, 40
	lui	a5,%hi(_ZN7ckernel13instrn_bufferE)
	lw	a4,%lo(_ZN7ckernel13instrn_bufferE)(a5)
	slli	a5,a0,20
	srli	a5,a5,8
	li	a3,-1996487887
	add	a3, a3, a5
# Op(0xffffff89) - d(3) s(3)
	sw	a3,0(a4)
	SFPNOP
	SFPNOP
	addi	a5,a0,1
	slli	a5,a5,20
	srli	a5,a5,8
	li	a3,-1996488159
	add	a3, a3, a5
# Op(0xffffff89) - d(2) s(2)
	sw	a3,0(a4)
	SFPNOP
	SFPNOP
	addi	a5,a0,2
	slli	a5,a5,20
	srli	a5,a5,8
	li	a3,-1996488431
	add	a3, a3, a5
# Op(0xffffff89) - d(1) s(1)
	sw	a3,0(a4)
	SFPNOP
	SFPNOP
	addi	a0,a0,3
	slli	a0,a0,20
	srli	a0,a0,8
	li	a5,-1996488703
	add	a5, a5, a0
# Op(0xffffff89) - d(0) s(0)
	sw	a5,0(a4)
	SFPNOP
	SFPNOP
	SFPSTORE	L3, 0, 2, 1
	SFPSTORE	L2, 0, 2, 10
	SFPSTORE	L1, 0, 2, 20
	SFPSTORE	L0, 0, 2, 30
	ret
	.size	_Z12setsgn_i_regj, .-_Z12setsgn_i_regj
	.align	1
	.globl	_Z13setsgn_i_boths
	.type	_Z13setsgn_i_boths, @function
_Z13setsgn_i_boths:
	SFPLOAD	L3, 1, 0, 10
	SFPLOAD	L2, 1, 0, 20
	SFPLOAD	L1, 1, 0, 30
	SFPLOAD	L0, 1, 0, 40
	SFPSETSGN	1979, L3, L3, 1
	SFPSETSGN	1979, L2, L2, 1
	lui	a5,%hi(_ZN7ckernel13instrn_bufferE)
	lw	a4,%lo(_ZN7ckernel13instrn_bufferE)(a5)
	slli	a5,a0,20
	srli	a5,a5,8
	li	a3,-1996488431
	add	a3, a3, a5
# Op(0xffffff89) - d(1) s(1)
	sw	a3,0(a4)
	SFPNOP
	SFPNOP
	addi	a0,a0,1
	slli	a0,a0,20
	srli	a0,a0,8
	li	a5,-1996488703
	add	a5, a5, a0
# Op(0xffffff89) - d(0) s(0)
	sw	a5,0(a4)
	SFPNOP
	SFPNOP
	SFPSTORE	L3, 0, 2, 1
	SFPSTORE	L2, 0, 2, 10
	SFPSTORE	L1, 0, 2, 20
	SFPSTORE	L0, 0, 2, 30
	ret
	.size	_Z13setsgn_i_boths, .-_Z13setsgn_i_boths
	.align	1
	.globl	_Z11multi_reusei
	.type	_Z11multi_reusei, @function
_Z11multi_reusei:
	SFPLOAD	L0, 0, 0, 0
	slli	a5,a0,20
	srli	a5,a5,8
	li	a4,0
	li	a3,-1996488703
	add	a3, a3, a5
# Op(0xffffff89) - d(0) s(0)
	sw	a3,0(a4)
	SFPNOP
	SFPNOP
	slli	a5,a0,20
	srli	a5,a5,8
	li	a3,-1996488703
	add	a3, a3, a5
# Op(0xffffff89) - d(0) s(0)
	sw	a3,0(a4)
	SFPNOP
	SFPNOP
	slli	a5,a0,20
	srli	a5,a5,8
	li	a3,-1996488703
	add	a3, a3, a5
# Op(0xffffff89) - d(0) s(0)
	sw	a3,0(a4)
	SFPNOP
	SFPNOP
	mv	a0,a5
	li	a5,-1996488703
	add	a5, a5, a0
# Op(0xffffff89) - d(0) s(0)
	sw	a5,0(a4)
	SFPNOP
	SFPNOP
	SFPSTORE	L0, 0, 2, 0
	ret
	.size	_Z11multi_reusei, .-_Z11multi_reusei
	.align	1
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
	.align	1
	.globl	_Z13loop_compileri
	.type	_Z13loop_compileri, @function
_Z13loop_compileri:
	lui	a5,%hi(_ZN7ckernel13instrn_bufferE)
	slli	a0,a0,16
	srai	a0,a0,16
	lw	a5,%lo(_ZN7ckernel13instrn_bufferE)(a5)
	slli	a1,a0,16
	addi	a2,a0,10
	addi	a3,a0,20
	addi	a4,a0,30
	srli	a1,a1,16
	li	a6,1882259456
	add	a6, a6, a1
# Op(0x70) - d(3)
	sw	a6,0(a5)
	slli	a1,a2,16
	srli	a1,a1,16
	li	a6,1881210880
	add	a6, a6, a1
# Op(0x70) - d(2)
	sw	a6,0(a5)
	slli	a1,a3,16
	srli	a1,a1,16
	li	a6,1880162304
	add	a6, a6, a1
# Op(0x70) - d(1)
	sw	a6,0(a5)
	slli	a1,a4,16
	srli	a1,a1,16
	li	a6,1879113728
	add	a6, a6, a1
# Op(0x70) - d(0)
	sw	a6,0(a5)
	SFPSTORE	L3, 0, 2, 1
	SFPSTORE	L2, 0, 2, 10
	SFPSTORE	L1, 0, 2, 20
	SFPSTORE	L0, 0, 2, 30
	slli	a1,a0,16
	srli	a1,a1,16
	li	a6,1882259456
	add	a6, a6, a1
# Op(0x70) - d(3)
	sw	a6,0(a5)
	slli	a1,a2,16
	srli	a1,a1,16
	li	a6,1881210880
	add	a6, a6, a1
# Op(0x70) - d(2)
	sw	a6,0(a5)
	slli	a1,a3,16
	srli	a1,a1,16
	li	a6,1880162304
	add	a6, a6, a1
# Op(0x70) - d(1)
	sw	a6,0(a5)
	slli	a1,a4,16
	srli	a1,a1,16
	li	a6,1879113728
	add	a6, a6, a1
# Op(0x70) - d(0)
	sw	a6,0(a5)
	SFPSTORE	L3, 0, 2, 1
	SFPSTORE	L2, 0, 2, 10
	SFPSTORE	L1, 0, 2, 20
	SFPSTORE	L0, 0, 2, 30
	slli	a1,a0,16
	srli	a1,a1,16
	li	a6,1882259456
	add	a6, a6, a1
# Op(0x70) - d(3)
	sw	a6,0(a5)
	slli	a1,a2,16
	srli	a1,a1,16
	li	a6,1881210880
	add	a6, a6, a1
# Op(0x70) - d(2)
	sw	a6,0(a5)
	slli	a1,a3,16
	srli	a1,a1,16
	li	a6,1880162304
	add	a6, a6, a1
# Op(0x70) - d(1)
	sw	a6,0(a5)
	slli	a1,a4,16
	srli	a1,a1,16
	li	a6,1879113728
	add	a6, a6, a1
# Op(0x70) - d(0)
	sw	a6,0(a5)
	SFPSTORE	L3, 0, 2, 1
	SFPSTORE	L2, 0, 2, 10
	SFPSTORE	L1, 0, 2, 20
	SFPSTORE	L0, 0, 2, 30
	slli	a0,a0,16
	srli	a0,a0,16
	li	a1,1882259456
	add	a1, a1, a0
# Op(0x70) - d(3)
	sw	a1,0(a5)
	slli	a2,a2,16
	srli	a2,a2,16
	li	a1,1881210880
	add	a1, a1, a2
# Op(0x70) - d(2)
	sw	a1,0(a5)
	slli	a3,a3,16
	srli	a3,a3,16
	li	a2,1880162304
	add	a2, a2, a3
# Op(0x70) - d(1)
	sw	a2,0(a5)
	slli	a4,a4,16
	srli	a4,a4,16
	li	a3,1879113728
	add	a3, a3, a4
# Op(0x70) - d(0)
	sw	a3,0(a5)
	SFPSTORE	L3, 0, 2, 1
	SFPSTORE	L2, 0, 2, 10
	SFPSTORE	L1, 0, 2, 20
	SFPSTORE	L0, 0, 2, 30
	ret
	.size	_Z13loop_compileri, .-_Z13loop_compileri
	.section	.rodata.str1.4,"aMS",@progbits,1
	.align	2
.LC0:
	.string	"%p\n"
	.section	.text.startup,"ax",@progbits
	.align	1
	.globl	main
	.type	main, @function
main:
	addi	sp,sp,-16
	sw	ra,12(sp)
	sw	s0,8(sp)
	sw	s1,4(sp)
	sw	s2,0(sp)
	mv	s1,a0
	call	_Z5macroi
	mv	a0,s1
	slli	s0,s1,16
	call	_Z13macro_checkedi
	srai	s0,s0,16
	SFPLOAD	L3, 1, 0, 10
	SFPLOAD	L2, 1, 0, 20
	SFPLOAD	L1, 1, 0, 30
	SFPLOAD	L0, 1, 0, 40
	SFPSTORE	L3, 0, 2, 1
	SFPSTORE	L2, 0, 2, 10
	SFPSTORE	L1, 0, 2, 20
	SFPSTORE	L0, 0, 2, 30
	mv	a0,s0
	call	_Z8load_regs
	mv	a0,s0
	call	_Z9load_boths
	SFPLOADI	L3, 0, 10
	SFPLOADI	L2, 4, 10
	SFPLOADI	L1, 8, 20
	SFPLOADI	L0, 10, 30
	SFPSTORE	L3, 0, 2, 1
	SFPSTORE	L2, 0, 2, 10
	SFPSTORE	L1, 0, 2, 20
	SFPSTORE	L0, 0, 2, 30
	mv	a0,s0
	call	_Z9loadi_regs
	mv	a0,s0
	call	_Z10loadi_boths
	SFPLOAD	L0, 1, 0, 10
	SFPSTORE	L0, 0, 2, 10
	SFPSTORE	L0, 0, 2, 20
	SFPSTORE	L0, 0, 2, 30
	SFPSTORE	L0, 0, 2, 40
	SFPLOAD	L0, 1, 0, 10
	lui	s2,%hi(_ZN7ckernel13instrn_bufferE)
	lw	a5,%lo(_ZN7ckernel13instrn_bufferE)(s2)
	slli	a4,s0,16
	srli	a4,a4,16
	li	a3,1912635392
	add	a3, a3, a4
# Op(0x72) s(0)
	sw	a3,0(a5)
	addi	a4,s0,1
	slli	a3,a4,16
	srli	a3,a3,16
	li	a2,1912733696
	add	a2, a2, a3
# Op(0x72) s(0)
	sw	a2,0(a5)
	addi	a3,s0,2
	slli	a2,a3,16
	srli	a2,a2,16
	li	a1,1912733696
	add	a1, a1, a2
# Op(0x72) s(0)
	sw	a1,0(a5)
	addi	a2,s0,3
	slli	a1,a2,16
	srli	a1,a1,16
	li	a0,1912733696
	add	a0, a0, a1
# Op(0x72) s(0)
	sw	a0,0(a5)
	SFPLOAD	L0, 1, 0, 10
	SFPSTORE	L0, 0, 2, 10
	SFPSTORE	L0, 0, 2, 20
	slli	a1,a4,16
	srli	a1,a1,16
	li	a0,1912733696
	add	a0, a0, a1
# Op(0x72) s(0)
	sw	a0,0(a5)
	slli	a1,a3,16
	srli	a1,a1,16
	li	a0,1912733696
	add	a0, a0, a1
# Op(0x72) s(0)
	sw	a0,0(a5)
	SFPSTORE	L13, 0, 2, 10
	SFPSTORE	L13, 0, 2, 20
	SFPSTORE	L13, 0, 2, 10
	SFPSTORE	L13, 0, 2, 20
	slli	a1,s0,16
	srli	a1,a1,16
	li	a0,1926266880
	add	a0, a0, a1
# Op(0x72) s(13)
	sw	a0,0(a5)
	slli	a1,a4,16
	srli	a1,a1,16
	li	a0,1926365184
	add	a0, a0, a1
# Op(0x72) s(13)
	sw	a0,0(a5)
	slli	a3,a3,16
	srli	a3,a3,16
	li	a1,1926365184
	add	a1, a1, a3
# Op(0x72) s(13)
	sw	a1,0(a5)
	slli	a2,a2,16
	srli	a2,a2,16
	li	a3,1926365184
	add	a3, a3, a2
# Op(0x72) s(13)
	sw	a3,0(a5)
	SFPSTORE	L13, 0, 2, 10
	SFPSTORE	L13, 0, 2, 20
	slli	a3,s0,16
	srli	a3,a3,16
	li	a2,1926266880
	add	a2, a2, a3
# Op(0x72) s(13)
	sw	a2,0(a5)
	slli	a4,a4,16
	srli	a4,a4,16
	li	a3,1926365184
	add	a3, a3, a4
# Op(0x72) s(13)
	sw	a3,0(a5)
	mv	a0,s0
	call	_Z8addi_imms
	mv	a0,s0
	call	_Z8addi_regs
	mv	a0,s0
	call	_Z9addi_boths
	mv	a0,s0
	call	_Z10iadd_i_imms
	mv	a0,s0
	call	_Z10iadd_i_regs
	mv	a0,s0
	call	_Z11iadd_i_boths
	mv	a0,s0
	call	_Z13iadd_i_imm_exs
	mv	a0,s0
	call	_Z13iadd_i_reg_exs
	mv	a0,s0
	call	_Z14iadd_i_both_exs
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
	SFPLOAD	L0, 0, 0, 0
	slli	a5,s1,20
	srli	a5,a5,8
	li	a4,0
	li	a3,-1996488703
	add	a3, a3, a5
# Op(0xffffff89) - d(0) s(0)
	sw	a3,0(a4)
	SFPNOP
	SFPNOP
	slli	a5,s1,20
	srli	a5,a5,8
	li	a3,-1996488703
	add	a3, a3, a5
# Op(0xffffff89) - d(0) s(0)
	sw	a3,0(a4)
	SFPNOP
	SFPNOP
	slli	a5,s1,20
	srli	a5,a5,8
	li	a3,-1996488703
	add	a3, a3, a5
# Op(0xffffff89) - d(0) s(0)
	sw	a3,0(a4)
	SFPNOP
	SFPNOP
	slli	a5,s1,20
	srli	a5,a5,8
	li	a3,-1996488703
	add	a3, a3, a5
# Op(0xffffff89) - d(0) s(0)
	sw	a3,0(a4)
	SFPNOP
	SFPNOP
	SFPSTORE	L0, 0, 2, 0
	lw	a5,%lo(_ZN7ckernel13instrn_bufferE)(s2)
	li	t3,1881538560
	add	t3,s1,t3
	li	t1,1898315776
	sw	t3,0(a5)
	add	t1,s1,t1
	li	a7,1915092992
	li	a6,2030043136
	sw	t1,0(a5)
	slli	a4,s1,12
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
	li	a2,-2113929216
	sw	a0,0(a5)
	add	a1,a4,a1
	addi	a2,a2,561
	li	a3,-2097152000
	sw	a1,0(a5)
	add	a2,a4,a2
	addi	a3,a3,561
	li	t4,-1996488704
	sw	a2,0(a5)
	add	a3,a4,a3
	addi	t4,t4,561
	sw	a3,0(a5)
	add	a4,a4,t4
	sw	a4,0(a5)
	sw	t3,0(a5)
	sw	t1,0(a5)
	sw	a7,0(a5)
	sw	a6,0(a5)
	sw	s1,0(a5)
	sw	a0,0(a5)
	sw	a1,0(a5)
	sw	a2,0(a5)
	sw	a3,0(a5)
	sw	a4,0(a5)
	sw	t3,0(a5)
	sw	t1,0(a5)
	sw	a7,0(a5)
	sw	a6,0(a5)
	sw	s1,0(a5)
	sw	a0,0(a5)
	sw	a1,0(a5)
	sw	a2,0(a5)
	sw	a3,0(a5)
	sw	a4,0(a5)
	sw	t3,0(a5)
	sw	t1,0(a5)
	sw	a7,0(a5)
	sw	a6,0(a5)
	sw	s1,0(a5)
	sw	a0,0(a5)
	sw	a1,0(a5)
	sw	a2,0(a5)
	slli	a1,s0,16
	sw	a3,0(a5)
	addi	a0,s0,10
	addi	a2,s0,20
	addi	a3,s0,30
	srli	a1,a1,16
	sw	a4,0(a5)
	li	a4,1882259456
	add	a4, a4, a1
# Op(0x70) - d(3)
	sw	a4,0(a5)
	lw	a1,%lo(_ZN7ckernel13instrn_bufferE)(s2)
	slli	a5,a0,16
	srli	a5,a5,16
	li	a4,1881210880
	add	a4, a4, a5
# Op(0x70) - d(2)
	sw	a4,0(a1)
	slli	a5,a2,16
	srli	a5,a5,16
	li	a4,1880162304
	add	a4, a4, a5
# Op(0x70) - d(1)
	sw	a4,0(a1)
	slli	a5,a3,16
	srli	a5,a5,16
	li	a4,1879113728
	add	a4, a4, a5
# Op(0x70) - d(0)
	sw	a4,0(a1)
	SFPSTORE	L3, 0, 2, 1
	SFPSTORE	L2, 0, 2, 10
	SFPSTORE	L1, 0, 2, 20
	SFPSTORE	L0, 0, 2, 30
	slli	a5,s0,16
	srli	a5,a5,16
	li	a4,1882259456
	add	a4, a4, a5
# Op(0x70) - d(3)
	sw	a4,0(a1)
	slli	a5,a0,16
	srli	a5,a5,16
	li	a4,1881210880
	add	a4, a4, a5
# Op(0x70) - d(2)
	sw	a4,0(a1)
	slli	a5,a2,16
	srli	a5,a5,16
	li	a4,1880162304
	add	a4, a4, a5
# Op(0x70) - d(1)
	sw	a4,0(a1)
	slli	a5,a3,16
	srli	a5,a5,16
	li	a4,1879113728
	add	a4, a4, a5
# Op(0x70) - d(0)
	sw	a4,0(a1)
	SFPSTORE	L3, 0, 2, 1
	SFPSTORE	L2, 0, 2, 10
	SFPSTORE	L1, 0, 2, 20
	SFPSTORE	L0, 0, 2, 30
	slli	a5,s0,16
	srli	a5,a5,16
	li	a4,1882259456
	add	a4, a4, a5
# Op(0x70) - d(3)
	sw	a4,0(a1)
	slli	a5,a0,16
	srli	a5,a5,16
	li	a4,1881210880
	add	a4, a4, a5
# Op(0x70) - d(2)
	sw	a4,0(a1)
	slli	a5,a2,16
	srli	a5,a5,16
	li	a4,1880162304
	add	a4, a4, a5
# Op(0x70) - d(1)
	sw	a4,0(a1)
	slli	a5,a3,16
	srli	a5,a5,16
	li	a4,1879113728
	add	a4, a4, a5
# Op(0x70) - d(0)
	sw	a4,0(a1)
	SFPSTORE	L3, 0, 2, 1
	SFPSTORE	L2, 0, 2, 10
	SFPSTORE	L1, 0, 2, 20
	SFPSTORE	L0, 0, 2, 30
	slli	s0,s0,16
	srli	s0,s0,16
	li	a5,1882259456
	add	a5, a5, s0
# Op(0x70) - d(3)
	sw	a5,0(a1)
	slli	a0,a0,16
	srli	a0,a0,16
	li	a5,1881210880
	add	a5, a5, a0
# Op(0x70) - d(2)
	sw	a5,0(a1)
	slli	a2,a2,16
	srli	a2,a2,16
	li	a5,1880162304
	add	a5, a5, a2
# Op(0x70) - d(1)
	sw	a5,0(a1)
	slli	a3,a3,16
	srli	a3,a3,16
	li	a5,1879113728
	add	a5, a5, a3
# Op(0x70) - d(0)
	sw	a5,0(a1)
	SFPSTORE	L3, 0, 2, 1
	SFPSTORE	L2, 0, 2, 10
	SFPSTORE	L1, 0, 2, 20
	SFPSTORE	L0, 0, 2, 30
	lui	a0,%hi(.LC0)
	addi	a0,a0,%lo(.LC0)
	call	printf
	lw	ra,12(sp)
	lw	s0,8(sp)
	lw	s1,4(sp)
	lw	s2,0(sp)
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
