	.file	"smoke.cc"
	.option nopic
	.attribute arch, "rv32i2p0_m2p0_w2p0"
	.attribute unaligned_access, 0
	.attribute stack_align, 16
	.text
	.align	1
	.globl	_Z5smokev
	.type	_Z5smokev, @function
_Z5smokev:
	SFPLOAD	L3, 0, 1, 0
	SFPLOAD	L0, 0, 1, 16
	SFPLOAD	L2, 0, 1, 32
	SFPMAD	L3, L0, L2, L1, 0
	SFPMAD	L3, L0, L6, L1, 0
	SFPMAD	L3, L7, L2, L1, 0
	SFPMAD	L3, L7, L6, L1, 0
	SFPMAD	L8, L0, L2, L1, 0
	SFPMAD	L8, L0, L6, L1, 0
	SFPMAD	L8, L7, L2, L1, 0
	SFPMAD	L8, L7, L6, L6, 0
	SFPMUL	L8, L7, L9, L1, 0
	SFPADD	L10, L8, L7, L1, 0
	SFPENCC	2, 8
	SFPPUSHC	0
	SFPSETCC	0, L3, 12
	SFPSETCC	1, L0, 12
	SFPSETCC	0, L3, 2
	SFPMOV	L3, L4, 0
	SFPIADD	0, L1, L4, 6
	SFPSETCC	0, L4, 2
	SFPSETCC	0, L3, 2
	SFPMAD	L1, L11, L3, L3, 0
	SFPWNOP
	SFPSETCC	0, L3, 2
	SFPPOPC	0
	SFPCOMPC
	SFPENCC	3, 10
	SFPLOADI	L3, 0, -12
	SFPSTORE	L3, 0, 2, 0
	SFPSTORE	L3, 0, 2, 4
	SFPSTORE	L13, 0, 2, 4
	SFPMAD	L3, L0, L2, L3, 0
	SFPWNOP
	SFPSTORE	L3, 0, 2, 4
	SFPWNOP
	SFPSTORE	L0, 0, 2, 1
	SFPLOAD	L3, 0, 2, 0
	SFPMOV	L3, L0, 1
	SFPSTORE	L2, 0, 2, 2
	SFPSTORE	L1, 0, 2, 3
	SFPSTORE	L0, 0, 2, 10
	SFPEXEXP	L3, L0, 0
	SFPEXMAN	L3, L0, 1
	SFPSETEXP	23, L3, L0, 1
	SFPSETEXP	0, L3, L0, 0
	SFPSETMAN	23, L3, L0, 1
	SFPSETMAN	0, L3, L0, 0
	SFPABS	L3, L0, 1
	SFPAND	L3, L0
	SFPOR	L3, L0
	SFPNOT	L3, L0
	SFPMULI	32, L0, 0
	SFPWNOP
	SFPADDI	32, L0, 0
	SFPDIVP2	32, L3, L0, 1
	SFPLZ	L3, L0, 2
	SFPSHFT	10, L0, L0, 1
	SFPSHFT	0, L3, L0, 0
	SFPIADD	10, L3, L3, 5
	SFPSETSGN	10, L3, L0, 1
	SFPSETSGN	0, L3, L0, 0
	SFPSTORE	L3, 0, 2, 6
	SFPSTORE	L0, 0, 2, 8
	SFPXOR	L3, L0
	SFPCAST L3, L0, 1
	SFPSHFT2	0, L3, L0, 3
	SFPWNOP
	SFPSTOCHRND	1, 3, L0, L3, L0, 1
	SFPSTOCHRND	1, 0, L0, L3, L0, 1
	SFPCONFIG	0, 0, 0
	SFPLUT	L3, 0
	SFPWNOP
	.size	_Z5smokev, .-_Z5smokev
	.align	1
	.globl	_Z10smoke_livev
	.type	_Z10smoke_livev, @function
_Z10smoke_livev:
	SFPLOAD	L0, 0, 1, 0
	SFPLOAD	L0, 0, 1, 0
	SFPLOAD	L1, 0, 1, 0
	SFPMAD	L0, L0, L0, L1, 0
	SFPWNOP
	SFPMUL	L0, L0, L9, L1, 0
	SFPWNOP
	SFPADD	L10, L0, L0, L1, 0
	SFPLOADI	L0, 0, 12
	SFPMOV	L1, L0, 1
	SFPEXMAN	L1, L0, 1
	SFPSETEXP	23, L1, L0, 1
	SFPSETMAN	23, L1, L0, 1
	SFPABS	L1, L0, 1
	SFPNOT	L1, L0
	SFPDIVP2	32, L1, L0, 1
	SFPLZ	L1, L0, 2
	SFPEXEXP	L1, L0, 0
	SFPIADD	10, L1, L0, 5
	SFPSETCC	0, L0, 2
	SFPENCC	3, 10
	SFPSETSGN	10, L1, L0, 1
	SFPCAST L1, L1, 1
	ret
	.size	_Z10smoke_livev, .-_Z10smoke_livev
	.align	1
	.globl	_Z7smokierv
	.type	_Z7smokierv, @function
_Z7smokierv:
	SFPLOAD	L3, 0, 1, 20
	SFPMAD	L2, L0, L1, L3, 0
	SFPWNOP
	SFPLUT	L3, 4
	SFPSTORE	L2, 0, 2, 20
	SFPSTORE	L0, 0, 2, 20
	SFPSTORE	L1, 0, 2, 20
	SFPSTORE	L3, 0, 2, 20
	SFPLOAD	L2, 0, 1, 20
	SFPLOAD	L0, 0, 1, 20
	SFPLOAD	L3, 0, 1, 20
	SFPSTORE	L2, 0, 2, 20
	SFPSTORE	L0, 0, 2, 20
	SFPSTORE	L3, 0, 2, 20
	SFPMOV	L1, L2, 2
	.size	_Z7smokierv, .-_Z7smokierv
	.align	1
	.globl	_Z8smokiestv
	.type	_Z8smokiestv, @function
_Z8smokiestv:
	SFPLOAD	L1, 0, 2, 0
	SFPLOAD	L0, 0, 2, 1
	SFPSETEXP	0, L1, L0, 0
	SFPSTORE	L0, 0, 2, 6
	ret
	.size	_Z8smokiestv, .-_Z8smokiestv
	.align	1
	.globl	_Z8assignlrv
	.type	_Z8assignlrv, @function
_Z8assignlrv:
	SFPSTORE	L0, 0, 0, 0
	SFPSTORE	L1, 0, 0, 0
	SFPSTORE	L2, 0, 0, 0
	SFPSTORE	L3, 0, 0, 0
	SFPSTORE	L4, 0, 0, 0
	SFPSTORE	L5, 0, 0, 0
	SFPSTORE	L6, 0, 0, 0
	SFPSTORE	L7, 0, 0, 0
	SFPSTORE	L8, 0, 0, 0
	SFPSTORE	L9, 0, 0, 0
	SFPSTORE	L10, 0, 0, 0
	SFPSTORE	L11, 0, 0, 0
	SFPSTORE	L12, 0, 0, 0
	SFPSTORE	L13, 0, 0, 0
	SFPSTORE	L14, 0, 0, 0
	SFPSTORE	L15, 0, 0, 0
	ret
	.size	_Z8assignlrv, .-_Z8assignlrv
	.section	.text.startup,"ax",@progbits
	.align	1
	.globl	main
	.type	main, @function
main:
	addi	sp,sp,-16
	sw	ra,12(sp)
	call	_Z8assignlrv
	call	_Z5smokev
	call	_Z10smoke_livev
	call	_Z7smokierv
	SFPLOAD	L1, 0, 2, 0
	SFPLOAD	L0, 0, 2, 1
	SFPSETEXP	0, L1, L0, 0
	SFPSTORE	L0, 0, 2, 6
	lw	ra,12(sp)
	li	a0,0
	addi	sp,sp,16
	jr	ra
	.size	main, .-main
	.ident	"GCC: (GNU) 10.2.0"
