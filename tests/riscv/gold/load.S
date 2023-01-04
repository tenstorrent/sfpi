	.file	"load.cc"
	.option nopic
	.attribute arch, "rv32i2p0"
	.attribute unaligned_access, 0
	.attribute stack_align, 16
	.text
	.align	2
	.globl	_Z12dual_use_bugPhPVjPVh
	.type	_Z12dual_use_bugPhPVjPVh, @function
_Z12dual_use_bugPhPVjPVh:
	lbu	a5,0(a0)	# L1 load
	sw	a5,0(a1)
	andi	a5,a5,0xff
	sb	a5,0(a2)
	ret
	.size	_Z12dual_use_bugPhPVjPVh, .-_Z12dual_use_bugPhPVjPVh
	.section	.text.startup,"ax",@progbits
	.align	2
	.globl	main
	.type	main, @function
main:
	li	a0,0
	ret
	.size	main, .-main
	.globl	array_udi
	.globl	array_usi
	.globl	array_uhi
	.globl	array_uqi
	.globl	array_di
	.globl	array_si
	.globl	array_hi
	.globl	array_qi
	.data
	.align	3
	.type	array_udi, @object
	.size	array_udi, 40
array_udi:
	.word	0
	.word	0
	.word	1
	.word	0
	.word	2
	.word	0
	.word	3
	.word	0
	.word	4
	.word	0
	.type	array_usi, @object
	.size	array_usi, 20
array_usi:
	.word	0
	.word	1
	.word	2
	.word	3
	.word	4
	.type	array_uhi, @object
	.size	array_uhi, 10
array_uhi:
	.half	0
	.half	1
	.half	2
	.half	3
	.half	4
	.zero	2
	.type	array_di, @object
	.size	array_di, 40
array_di:
	.word	0
	.word	0
	.word	-1
	.word	-1
	.word	-2
	.word	-1
	.word	-3
	.word	-1
	.word	-4
	.word	-1
	.type	array_si, @object
	.size	array_si, 20
array_si:
	.word	0
	.word	-1
	.word	-2
	.word	-3
	.word	-4
	.type	array_hi, @object
	.size	array_hi, 10
array_hi:
	.half	0
	.half	-1
	.half	-2
	.half	-3
	.half	-4
	.section	.sdata,"aw"
	.align	2
	.type	array_uqi, @object
	.size	array_uqi, 5
array_uqi:
	.string	""
	.ascii	"\001\002\003\004"
	.zero	3
	.type	array_qi, @object
	.size	array_qi, 5
array_qi:
	.string	""
	.ascii	"\377\376\375\374"
	.ident	"GCC: (GNU) 10.2.0"
