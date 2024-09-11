	.file	"smoke.cc"
	.option nopic
	.attribute arch, "rv32i2p0_l2p0"
	.attribute unaligned_access, 0
	.attribute stack_align, 16
	.text
	.align	2
	.globl	_Z5smokev
	.type	_Z5smokev, @function
_Z5smokev:
	SFPLOAD	L3, 0, 0, 1
	SFPLOAD	L0, 16, 0, 1
	SFPLOAD	L2, 32, 0, 1
	SFPMAD	L1, L3, L0, L2, 0
	SFPMAD	L1, L3, L0, L6, 0
	SFPMAD	L1, L3, L7, L2, 0
	SFPMAD	L1, L3, L7, L6, 0
	SFPMAD	L1, L8, L0, L2, 0
	SFPMAD	L1, L8, L0, L6, 0
	SFPMAD	L1, L8, L7, L2, 0
	SFPMAD	L6, L8, L7, L6, 0
	SFPMUL	L1, L8, L7, L9, 0
	SFPADD	L1, L10, L8, L7, 0
	SFPENCC	2, 8
	SFPPUSHC	0
	SFPSETCC	L3, 0, 12
	SFPSETCC	L0, 1, 12
	SFPSETCC	L3, 0, 2
	SFPMOV	L4, L3, 0
	SFPIADD	L4, L1, 0, 6
	SFPSETCC	L4, 0, 2
	SFPSETCC	L3, 0, 2
	SFPMAD	L3, L1, L11, L3, 0
	SFPNOP
	SFPSETCC	L3, 0, 2
	SFPPOPC	0
	SFPCOMPC
	SFPENCC	3, 10
	SFPLOADI	L3, -12, 0
	SFPSTORE	0, L3, 0, 2
	SFPSTORE	4, L3, 0, 2
	SFPSTORE	4, L13, 0, 2
	SFPMAD	L3, L3, L0, L2, 0
	SFPNOP
	SFPSTORE	4, L3, 0, 2
	SFPNOP
	SFPSTORE	1, L0, 0, 2
	SFPLOAD	L3, 0, 0, 2
	SFPMOV	L0, L3, 1
	SFPSTORE	2, L2, 0, 2
	SFPSTORE	3, L1, 0, 2
	SFPSTORE	10, L0, 0, 2
	SFPEXEXP	L0, L3, 0
	SFPEXMAN	L0, L3, 1
	SFPSETEXP	L0, L3, 23, 1
	SFPSETEXP	L0, L3, 0, 0
	SFPSETMAN	L0, L3, 23, 1
	SFPSETMAN	L0, L3, 0, 0
	SFPABS	L0, L3, 1
	SFPAND	L0, L3
	SFPOR	L0, L3
	SFPNOT	L0, L3
	SFPMULI	L0, 32, 0
	SFPNOP
	SFPADDI	L0, 32, 0
	SFPDIVP2	L0, L3, 32, 1
	SFPLZ	L0, L3, 4
	SFPSHFT	L0, L0, 10, 1
	SFPSHFT	L0, L3, 0, 0
	SFPIADD	L3, L3, 10, 5
	SFPSETSGN	L0, L3, 10, 1
	SFPSETSGN	L0, L3, 0, 0
	SFPSTORE	6, L3, 0, 2
	SFPSTORE	8, L0, 0, 2
	SFPXOR	L0, L3
	SFPCAST L0, L3, 1
	SFPSHFT2	L0, L3, 0, 3
	SFPNOP
	SFPSTOCHRND	L0, L0, L3, 1, 1, 3
	SFPSTOCHRND	L0, L0, L3, 1, 1, 0
	SFPCONFIG	0, 0, 0
	SFPLUT	L3, 0
	.size	_Z5smokev, .-_Z5smokev
	.align	2
	.globl	_Z10smoke_livev
	.type	_Z10smoke_livev, @function
_Z10smoke_livev:
	SFPLOAD	L0, 0, 0, 1
	SFPLOAD	L0, 0, 0, 1
	SFPLOAD	L1, 0, 0, 1
	SFPMAD	L1, L0, L0, L0, 0
	SFPNOP
	SFPMUL	L1, L0, L0, L9, 0
	SFPNOP
	SFPADD	L1, L10, L0, L0, 0
	SFPLOADI	L0, 12, 0
	SFPMOV	L0, L1, 1
	SFPEXMAN	L0, L1, 1
	SFPSETEXP	L0, L1, 23, 1
	SFPSETMAN	L0, L1, 23, 1
	SFPABS	L0, L1, 1
	SFPNOT	L0, L1
	SFPDIVP2	L0, L1, 32, 1
	SFPLZ	L0, L1, 2
	SFPEXEXP	L0, L1, 0
	SFPIADD	L0, L1, 10, 5
	SFPSETCC	L0, 0, 2
	SFPENCC	3, 10
	SFPSETSGN	L0, L1, 10, 1
	SFPMOV	L0, L1, 2
	SFPCAST L0, L1, 1
	SFPSHFT2	L0, L1, 0, 3
	SFPNOP
	SFPSTOCHRND	L0, L0, L1, 1, 1, 3
	SFPSTOCHRND	L0, L0, L1, 1, 1, 0
	ret
	.size	_Z10smoke_livev, .-_Z10smoke_livev
	.align	2
	.globl	_Z7smokierv
	.type	_Z7smokierv, @function
_Z7smokierv:
	SFPLOAD	L3, 20, 0, 1
	SFPMAD	L3, L2, L0, L1, 0
	SFPNOP
	SFPLUT	L3, 4
	SFPSTORE	20, L2, 0, 2
	SFPSTORE	20, L0, 0, 2
	SFPSTORE	20, L1, 0, 2
	SFPSTORE	20, L3, 0, 2
	SFPLOAD	L2, 20, 0, 1
	SFPLOAD	L0, 20, 0, 1
	SFPLOAD	L3, 20, 0, 1
	SFPSTORE	20, L2, 0, 2
	SFPSTORE	20, L0, 0, 2
	SFPSTORE	20, L3, 0, 2
	SFPMOV	L2, L1, 2
	.size	_Z7smokierv, .-_Z7smokierv
	.align	2
	.globl	_Z8smokiestv
	.type	_Z8smokiestv, @function
_Z8smokiestv:
	SFPLOAD	L1, 0, 0, 2
	SFPLOAD	L0, 1, 0, 2
	SFPSETEXP	L0, L1, 0, 0
	SFPSTORE	6, L0, 0, 2
	ret
	.size	_Z8smokiestv, .-_Z8smokiestv
	.align	2
	.globl	_Z8assignlrv
	.type	_Z8assignlrv, @function
_Z8assignlrv:
	SFPSTORE	0, L0, 0, 0
	SFPSTORE	0, L1, 0, 0
	SFPSTORE	0, L2, 0, 0
	SFPSTORE	0, L3, 0, 0
	SFPSTORE	0, L4, 0, 0
	SFPSTORE	0, L5, 0, 0
	SFPSTORE	0, L6, 0, 0
	SFPSTORE	0, L7, 0, 0
	SFPSTORE	0, L8, 0, 0
	SFPSTORE	0, L9, 0, 0
	SFPSTORE	0, L10, 0, 0
	SFPSTORE	0, L11, 0, 0
	SFPSTORE	0, L12, 0, 0
	SFPSTORE	0, L13, 0, 0
	SFPSTORE	0, L14, 0, 0
	SFPSTORE	0, L15, 0, 0
	ret
	.size	_Z8assignlrv, .-_Z8assignlrv
	.align	2
	.globl	_Z11bh_specificv
	.type	_Z11bh_specificv, @function
_Z11bh_specificv:
	SFPMUL24	L0, L1, L2, L0, 0
	SFPNOP
	SFPMUL24	L0, L1, L2, L0, 0
	SFPNOP
	SFPARECIP	L1, L0, 0, 1
	SFPARECIP	L0, L1, 0, 2
	SFPSTORE	20, L0, 0, 2
	SFPGT	L0, L0, 0, 1
	SFPLE	L0, L0, 0, 1
	SFPSTORE	20, L0, 0, 2
	SFPENCC	3, 10
	ret
	.size	_Z11bh_specificv, .-_Z11bh_specificv
	.section	.text.startup,"ax",@progbits
	.align	2
	.globl	main
	.type	main, @function
main:
	addi	sp,sp,-16
	sw	ra,12(sp)
	call	_Z8assignlrv
	call	_Z5smokev
	call	_Z10smoke_livev
	call	_Z7smokierv
	SFPLOAD	L1, 0, 0, 2
	SFPLOAD	L0, 1, 0, 2
	SFPSETEXP	L0, L1, 0, 0
	SFPSTORE	6, L0, 0, 2
	SFPMUL24	L0, L1, L2, L0, 0
	SFPNOP
	SFPMUL24	L0, L1, L2, L0, 0
	SFPNOP
	SFPARECIP	L1, L0, 0, 1
	SFPARECIP	L0, L1, 0, 2
	SFPSTORE	20, L0, 0, 2
	SFPGT	L0, L0, 0, 1
	SFPLE	L0, L0, 0, 1
	SFPSTORE	20, L0, 0, 2
	SFPENCC	3, 10
	lw	ra,12(sp)
	li	a0,0
	addi	sp,sp,16
	jr	ra
	.size	main, .-main
	.ident	"GCC: (gcdc243316e1) 12.2.0"
