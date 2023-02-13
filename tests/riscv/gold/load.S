	.file	"load.cc"
	.option nopic
	.attribute arch, "rv32i2p0"
	.attribute unaligned_access, 0
	.attribute stack_align, 16
	.text
	.align	2
	.globl	_Z5hoistU11rvtt_l1_ptrPiS_
	.type	_Z5hoistU11rvtt_l1_ptrPiS_, @function
_Z5hoistU11rvtt_l1_ptrPiS_:
	lw	a0,0(a0)
	lw	a4,4(a1)
	lw	a5,0(a1)
	add	a5,a5,a4
	lw	a4,8(a1)
	add	a5,a5,a4
	add	a0,a5,a0
	ret
	.size	_Z5hoistU11rvtt_l1_ptrPiS_, .-_Z5hoistU11rvtt_l1_ptrPiS_
	.align	2
	.globl	_Z11hoist_storeU11rvtt_l1_ptrPiS_
	.type	_Z11hoist_storeU11rvtt_l1_ptrPiS_, @function
_Z11hoist_storeU11rvtt_l1_ptrPiS_:
	lw	a5,0(a1)
	lw	a4,4(a1)
	li	a3,3
	sw	a3,0(a1)
	lw	a0,0(a0)
	add	a5,a5,a4
	lw	a4,8(a1)
	add	a5,a5,a4
	add	a0,a5,a0
	ret
	.size	_Z11hoist_storeU11rvtt_l1_ptrPiS_, .-_Z11hoist_storeU11rvtt_l1_ptrPiS_
	.align	2
	.globl	_Z20hoist_restrict_storeU11rvtt_l1_ptrPiS_i
	.type	_Z20hoist_restrict_storeU11rvtt_l1_ptrPiS_i, @function
_Z20hoist_restrict_storeU11rvtt_l1_ptrPiS_i:
	lw	a0,0(a0)
	slli	a2,a2,2
	add	a2,a1,a2
	li	a4,3
	lw	a5,0(a2)
	sw	a4,0(a1)
	lw	a4,4(a2)
	add	a5,a5,a4
	lw	a4,8(a2)
	add	a5,a5,a4
	add	a0,a5,a0
	ret
	.size	_Z20hoist_restrict_storeU11rvtt_l1_ptrPiS_i, .-_Z20hoist_restrict_storeU11rvtt_l1_ptrPiS_i
	.align	2
	.globl	_Z20hoist_volatile_loadsU11rvtt_l1_ptrPiPVi
	.type	_Z20hoist_volatile_loadsU11rvtt_l1_ptrPiPVi, @function
_Z20hoist_volatile_loadsU11rvtt_l1_ptrPiPVi:
	lw	a5,0(a1)
	lw	a3,4(a1)
	add	a5,a5,a3
	lw	a4,8(a1)
	lw	a0,0(a0)
	add	a5,a5,a4
	add	a0,a5,a0
	ret
	.size	_Z20hoist_volatile_loadsU11rvtt_l1_ptrPiPVi, .-_Z20hoist_volatile_loadsU11rvtt_l1_ptrPiPVi
	.align	2
	.globl	_Z28hoist_volatile_restrict_loadU11rvtt_l1_ptrPiPVi
	.type	_Z28hoist_volatile_restrict_loadU11rvtt_l1_ptrPiPVi, @function
_Z28hoist_volatile_restrict_loadU11rvtt_l1_ptrPiPVi:
	li	a4,3
	lw	a5,0(a1)
	sw	a4,0(a1)
	lw	a3,4(a1)
	add	a5,a5,a3
	lw	a4,8(a1)
	lw	a0,0(a0)
	add	a5,a5,a4
	add	a0,a5,a0
	ret
	.size	_Z28hoist_volatile_restrict_loadU11rvtt_l1_ptrPiPVi, .-_Z28hoist_volatile_restrict_loadU11rvtt_l1_ptrPiPVi
	.align	2
	.globl	_Z21hoist_unspec_volatileU11rvtt_l1_ptrPiS_
	.type	_Z21hoist_unspec_volatileU11rvtt_l1_ptrPiS_, @function
_Z21hoist_unspec_volatileU11rvtt_l1_ptrPiS_:
	lw	a5,0(a1)
	SFPENCC	3, 10
	TTNOP
	lw	a0,0(a0)
	lw	a4,4(a1)
	add	a5,a5,a4
	lw	a4,8(a1)
	add	a5,a5,a4
	add	a0,a5,a0
	ret
	.size	_Z21hoist_unspec_volatileU11rvtt_l1_ptrPiS_, .-_Z21hoist_unspec_volatileU11rvtt_l1_ptrPiS_
	.align	2
	.globl	_Z5lowerU11rvtt_l1_ptrPiS_i
	.type	_Z5lowerU11rvtt_l1_ptrPiS_i, @function
_Z5lowerU11rvtt_l1_ptrPiS_i:
	lw	a0,0(a0)
	lw	a5,0(a1)
	lw	a3,4(a1)
	lw	a4,8(a1)
	slli	a2,a2,2
	add	a5,a5,a3
	add	a4,a4,a0
	add	a1,a1,a2
	add	a5,a5,a4
	lw	a0,0(a1)
	add	a0,a5,a0
	ret
	.size	_Z5lowerU11rvtt_l1_ptrPiS_i, .-_Z5lowerU11rvtt_l1_ptrPiS_i
	.align	2
	.globl	_Z6lower2U11rvtt_l1_ptrPiS_i
	.type	_Z6lower2U11rvtt_l1_ptrPiS_i, @function
_Z6lower2U11rvtt_l1_ptrPiS_i:
	lw	a0,0(a0)
	lw	a4,8(a1)
	lw	a5,0(a1)
	lw	a3,4(a1)
	slli	a2,a2,2
	add	a5,a5,a3
	add	a0,a4,a0
	add	a1,a1,a2
	add	a5,a5,a0
	lw	a4,0(a1)
	add	a5,a5,a4
	srai	a0,a0,1
	add	a0,a5,a0
	ret
	.size	_Z6lower2U11rvtt_l1_ptrPiS_i, .-_Z6lower2U11rvtt_l1_ptrPiS_i
	.globl	global_l1_const_ptr
	.globl	global_l1_xo_ptr
	.globl	global_l1_ptr
	.globl	global_ts
	.globl	global_l1
	.globl	global
	.bss
	.align	2
	.type	global_ts, @object
	.size	global_ts, 32
global_ts:
	.zero	32
	.type	global_l1, @object
	.size	global_l1, 16
global_l1:
	.zero	16
	.type	global, @object
	.size	global, 16
global:
	.zero	16
	.section	.sdata,"aw"
	.align	2
	.type	global_l1_const_ptr, @object
	.size	global_l1_const_ptr, 4
global_l1_const_ptr:
	.word	2048
	.type	global_l1_xo_ptr, @object
	.size	global_l1_xo_ptr, 4
global_l1_xo_ptr:
	.word	global_l1
	.type	global_l1_ptr, @object
	.size	global_l1_ptr, 4
global_l1_ptr:
	.word	global
	.ident	"GCC: (GNU) 10.2.0"
