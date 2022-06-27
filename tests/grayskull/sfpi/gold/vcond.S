	.file	"vcond.cc"
	.option nopic
	.attribute arch, "rv32i2p0_y2p0"
	.attribute unaligned_access, 0
	.attribute stack_align, 16
	.text
	.align	1
	.globl	_Z19vcond_inside_inlinev
	.type	_Z19vcond_inside_inlinev, @function
_Z19vcond_inside_inlinev:
	SFPLOADI	L0, 0, 16256
	SFPSETCC	0, L0, 4
	SFPNOP
	SFPLOADI	L0, 0, 16384
	SFPENCC	3, 10
	SFPNOP
	SFPSTORE	L0, 0, 0
	ret
	.size	_Z19vcond_inside_inlinev, .-_Z19vcond_inside_inlinev
	.align	1
	.globl	_Z22vcond_inside_noninlinei
	.type	_Z22vcond_inside_noninlinei, @function
_Z22vcond_inside_noninlinei:
	SFPLOADI	L0, 0, 16256
	bne	a0,zero,.L4
	SFPPUSHC
	SFPLOADI	L1, 4, 0
	SFPSETCC	0, L0, 0
	SFPNOP
	SFPLOADI	L1, 4, 1
	SFPPOPC
	SFPNOP
	SFPSETCC	0, L1, 2
	SFPNOP
	SFPLOADI	L0, 0, 16384
	SFPENCC	3, 10
	SFPNOP
	SFPSTORE	L0, 0, 0
	ret
.L4:
	SFPPUSHC
	SFPLOADI	L1, 4, 0
	SFPSETCC	0, L0, 4
	SFPNOP
	SFPLOADI	L1, 4, 1
	SFPPOPC
	SFPNOP
	SFPSETCC	0, L1, 2
	SFPNOP
	SFPLOADI	L0, 0, 16384
	SFPENCC	3, 10
	SFPNOP
	SFPSTORE	L0, 0, 0
	ret
	.size	_Z22vcond_inside_noninlinei, .-_Z22vcond_inside_noninlinei
	.align	1
	.globl	_Z20vcond_outside_inlinev
	.type	_Z20vcond_outside_inlinev, @function
_Z20vcond_outside_inlinev:
	SFPLOADI	L0, 0, 16256
	SFPLOADI	L1, 4, 0
	SFPSETCC	0, L0, 4
	SFPNOP
	SFPLOADI	L1, 4, 1
	SFPENCC	3, 10
	SFPNOP
	SFPSETCC	0, L1, 6
	SFPNOP
	SFPLOADI	L0, 0, 16384
	SFPENCC	3, 10
	SFPNOP
	SFPSTORE	L0, 0, 0
	ret
	.size	_Z20vcond_outside_inlinev, .-_Z20vcond_outside_inlinev
	.align	1
	.globl	_Z23vcond_outside_noninlinei
	.type	_Z23vcond_outside_noninlinei, @function
_Z23vcond_outside_noninlinei:
	SFPLOADI	L0, 0, 16256
	bne	a0,zero,.L8
	SFPLOADI	L1, 4, 0
	SFPSETCC	0, L0, 0
	SFPNOP
	SFPLOADI	L1, 4, 1
	SFPENCC	3, 10
	SFPNOP
	SFPSETCC	0, L1, 6
	SFPNOP
	SFPLOADI	L0, 0, 16384
	SFPENCC	3, 10
	SFPNOP
	SFPSTORE	L0, 0, 0
	ret
.L8:
	SFPLOADI	L1, 4, 0
	SFPSETCC	0, L0, 4
	SFPNOP
	SFPLOADI	L1, 4, 1
	SFPENCC	3, 10
	SFPNOP
	SFPSETCC	0, L1, 6
	SFPNOP
	SFPLOADI	L0, 0, 16384
	SFPENCC	3, 10
	SFPNOP
	SFPSTORE	L0, 0, 0
	ret
	.size	_Z23vcond_outside_noninlinei, .-_Z23vcond_outside_noninlinei
	.align	1
	.globl	_Z19vcond_orphan_inlinei
	.type	_Z19vcond_orphan_inlinei, @function
_Z19vcond_orphan_inlinei:
	SFPLOADI	L1, 0, 16256
	SFPLOADI	L0, 4, 0
	SFPSETCC	0, L1, 4
	SFPNOP
	SFPLOADI	L0, 4, 1
	SFPENCC	3, 10
	SFPNOP
	ret
	.size	_Z19vcond_orphan_inlinei, .-_Z19vcond_orphan_inlinei
	.align	1
	.globl	_Z22vcond_orphan_noninlinei
	.type	_Z22vcond_orphan_noninlinei, @function
_Z22vcond_orphan_noninlinei:
	SFPLOADI	L0, 0, 16256
	bne	a0,zero,.L12
	SFPLOADI	L1, 4, 0
	SFPSETCC	0, L0, 0
	SFPNOP
	SFPLOADI	L1, 4, 1
	SFPENCC	3, 10
	SFPNOP
	ret
.L12:
	SFPLOADI	L1, 4, 0
	SFPSETCC	0, L0, 4
	SFPNOP
	SFPLOADI	L1, 4, 1
	SFPENCC	3, 10
	SFPNOP
	ret
	.size	_Z22vcond_orphan_noninlinei, .-_Z22vcond_orphan_noninlinei
	.align	1
	.globl	_Z19vcond_asbool_inlinei
	.type	_Z19vcond_asbool_inlinei, @function
_Z19vcond_asbool_inlinei:
	SFPLOADI	L1, 0, 16256
	SFPLOADI	L0, 4, 0
	SFPSETCC	0, L1, 4
	SFPNOP
	SFPLOADI	L0, 4, 1
	SFPENCC	3, 10
	SFPNOP
	SFPSTORE	L0, 2, 0
	ret
	.size	_Z19vcond_asbool_inlinei, .-_Z19vcond_asbool_inlinei
	.align	1
	.globl	_Z22vcond_asbool_noninlinei
	.type	_Z22vcond_asbool_noninlinei, @function
_Z22vcond_asbool_noninlinei:
	SFPLOADI	L1, 0, 16256
	bne	a0,zero,.L16
	SFPLOADI	L0, 4, 0
	SFPSETCC	0, L1, 0
	SFPNOP
	SFPLOADI	L0, 4, 1
	SFPENCC	3, 10
	SFPNOP
	SFPSTORE	L0, 2, 0
	ret
.L16:
	SFPLOADI	L0, 4, 0
	SFPSETCC	0, L1, 4
	SFPNOP
	SFPLOADI	L0, 4, 1
	SFPENCC	3, 10
	SFPNOP
	SFPSTORE	L0, 2, 0
	ret
	.size	_Z22vcond_asbool_noninlinei, .-_Z22vcond_asbool_noninlinei
	.align	1
	.globl	_Z20vcond_inside_inline2v
	.type	_Z20vcond_inside_inline2v, @function
_Z20vcond_inside_inline2v:
	SFPLOADI	L0, 0, 16256
	SFPSETCC	0, L0, 4
	SFPNOP
	SFPSETCC	0, L0, 2
	SFPNOP
	SFPLOADI	L0, 0, 16384
	SFPENCC	3, 10
	SFPNOP
	SFPSTORE	L0, 0, 0
	ret
	.size	_Z20vcond_inside_inline2v, .-_Z20vcond_inside_inline2v
	.align	1
	.globl	_Z23vcond_inside_noninline2i
	.type	_Z23vcond_inside_noninline2i, @function
_Z23vcond_inside_noninline2i:
	SFPLOADI	L0, 0, 16256
	bne	a0,zero,.L20
	SFPPUSHC
	SFPLOADI	L1, 4, 0
	SFPSETCC	0, L0, 0
	SFPNOP
	SFPSETCC	0, L0, 2
	SFPNOP
	SFPLOADI	L1, 4, 1
	SFPPOPC
	SFPNOP
	SFPSETCC	0, L1, 2
	SFPNOP
	SFPLOADI	L0, 0, 16384
	SFPENCC	3, 10
	SFPNOP
	SFPSTORE	L0, 0, 0
	ret
.L20:
	SFPPUSHC
	SFPLOADI	L1, 4, 0
	SFPSETCC	0, L0, 4
	SFPNOP
	SFPSETCC	0, L0, 2
	SFPNOP
	SFPLOADI	L1, 4, 1
	SFPPOPC
	SFPNOP
	SFPSETCC	0, L1, 2
	SFPNOP
	SFPLOADI	L0, 0, 16384
	SFPENCC	3, 10
	SFPNOP
	SFPSTORE	L0, 0, 0
	ret
	.size	_Z23vcond_inside_noninline2i, .-_Z23vcond_inside_noninline2i
	.align	1
	.globl	_Z21vcond_outside_inline2v
	.type	_Z21vcond_outside_inline2v, @function
_Z21vcond_outside_inline2v:
	SFPLOADI	L0, 0, 16256
	SFPLOADI	L1, 4, 0
	SFPSETCC	0, L0, 4
	SFPNOP
	SFPSETCC	0, L0, 2
	SFPNOP
	SFPLOADI	L1, 4, 1
	SFPENCC	3, 10
	SFPNOP
	SFPSETCC	0, L1, 6
	SFPNOP
	SFPLOADI	L0, 0, 16384
	SFPENCC	3, 10
	SFPNOP
	SFPSTORE	L0, 0, 0
	ret
	.size	_Z21vcond_outside_inline2v, .-_Z21vcond_outside_inline2v
	.align	1
	.globl	_Z24vcond_outside_noninline2i
	.type	_Z24vcond_outside_noninline2i, @function
_Z24vcond_outside_noninline2i:
	SFPLOADI	L0, 0, 16256
	bne	a0,zero,.L24
	SFPLOADI	L1, 4, 0
	SFPSETCC	0, L0, 0
	SFPNOP
	SFPSETCC	0, L0, 2
	SFPNOP
	SFPLOADI	L1, 4, 1
	SFPENCC	3, 10
	SFPNOP
	SFPSETCC	0, L1, 6
	SFPNOP
	SFPLOADI	L0, 0, 16384
	SFPENCC	3, 10
	SFPNOP
	SFPSTORE	L0, 0, 0
	ret
.L24:
	SFPLOADI	L1, 4, 0
	SFPSETCC	0, L0, 4
	SFPNOP
	SFPSETCC	0, L0, 2
	SFPNOP
	SFPLOADI	L1, 4, 1
	SFPENCC	3, 10
	SFPNOP
	SFPSETCC	0, L1, 6
	SFPNOP
	SFPLOADI	L0, 0, 16384
	SFPENCC	3, 10
	SFPNOP
	SFPSTORE	L0, 0, 0
	ret
	.size	_Z24vcond_outside_noninline2i, .-_Z24vcond_outside_noninline2i
	.align	1
	.globl	_Z20vcond_orphan_inline2i
	.type	_Z20vcond_orphan_inline2i, @function
_Z20vcond_orphan_inline2i:
	SFPLOADI	L1, 0, 16256
	SFPLOADI	L0, 4, 0
	SFPSETCC	0, L1, 4
	SFPNOP
	SFPSETCC	0, L1, 2
	SFPNOP
	SFPLOADI	L0, 4, 1
	SFPENCC	3, 10
	SFPNOP
	ret
	.size	_Z20vcond_orphan_inline2i, .-_Z20vcond_orphan_inline2i
	.align	1
	.globl	_Z23vcond_orphan_noninline2i
	.type	_Z23vcond_orphan_noninline2i, @function
_Z23vcond_orphan_noninline2i:
	SFPLOADI	L0, 0, 16256
	bne	a0,zero,.L28
	SFPLOADI	L1, 4, 0
	SFPSETCC	0, L0, 0
	SFPNOP
	SFPSETCC	0, L0, 2
	SFPNOP
	SFPLOADI	L1, 4, 1
	SFPENCC	3, 10
	SFPNOP
	ret
.L28:
	SFPLOADI	L1, 4, 0
	SFPSETCC	0, L0, 4
	SFPNOP
	SFPSETCC	0, L0, 2
	SFPNOP
	SFPLOADI	L1, 4, 1
	SFPENCC	3, 10
	SFPNOP
	ret
	.size	_Z23vcond_orphan_noninline2i, .-_Z23vcond_orphan_noninline2i
	.align	1
	.globl	_Z20vcond_asbool_inline2i
	.type	_Z20vcond_asbool_inline2i, @function
_Z20vcond_asbool_inline2i:
	SFPLOADI	L1, 0, 16256
	SFPLOADI	L0, 4, 0
	SFPSETCC	0, L1, 4
	SFPNOP
	SFPSETCC	0, L1, 2
	SFPNOP
	SFPLOADI	L0, 4, 1
	SFPENCC	3, 10
	SFPNOP
	SFPSTORE	L0, 2, 0
	ret
	.size	_Z20vcond_asbool_inline2i, .-_Z20vcond_asbool_inline2i
	.align	1
	.globl	_Z23vcond_asbool_noninline2i
	.type	_Z23vcond_asbool_noninline2i, @function
_Z23vcond_asbool_noninline2i:
	SFPLOADI	L1, 0, 16256
	bne	a0,zero,.L32
	SFPLOADI	L0, 4, 0
	SFPSETCC	0, L1, 0
	SFPNOP
	SFPSETCC	0, L1, 2
	SFPNOP
	SFPLOADI	L0, 4, 1
	SFPENCC	3, 10
	SFPNOP
	SFPSTORE	L0, 2, 0
	ret
.L32:
	SFPLOADI	L0, 4, 0
	SFPSETCC	0, L1, 4
	SFPNOP
	SFPSETCC	0, L1, 2
	SFPNOP
	SFPLOADI	L0, 4, 1
	SFPENCC	3, 10
	SFPNOP
	SFPSTORE	L0, 2, 0
	ret
	.size	_Z23vcond_asbool_noninline2i, .-_Z23vcond_asbool_noninline2i
	.align	1
	.globl	_Z20vcond_inside_inline3v
	.type	_Z20vcond_inside_inline3v, @function
_Z20vcond_inside_inline3v:
	SFPLOADI	L0, 0, 16256
	SFPSETCC	0, L0, 6
	SFPNOP
	SFPSETCC	0, L0, 4
	SFPNOP
	SFPSETCC	0, L0, 2
	SFPNOP
	SFPCOMPC
	SFPNOP
	SFPLOADI	L0, 0, 16384
	SFPENCC	3, 10
	SFPNOP
	SFPSTORE	L0, 0, 0
	ret
	.size	_Z20vcond_inside_inline3v, .-_Z20vcond_inside_inline3v
	.align	1
	.globl	_Z23vcond_inside_noninline3i
	.type	_Z23vcond_inside_noninline3i, @function
_Z23vcond_inside_noninline3i:
	SFPLOADI	L0, 0, 16256
	SFPSETCC	0, L0, 6
	SFPNOP
	SFPSETCC	0, L0, 4
	SFPNOP
	SFPSETCC	0, L0, 2
	SFPNOP
	SFPCOMPC
	SFPNOP
	SFPLOADI	L0, 0, 16384
	SFPENCC	3, 10
	SFPNOP
	SFPSTORE	L0, 0, 0
	ret
	.size	_Z23vcond_inside_noninline3i, .-_Z23vcond_inside_noninline3i
	.align	1
	.globl	_Z21vcond_outside_inline3v
	.type	_Z21vcond_outside_inline3v, @function
_Z21vcond_outside_inline3v:
	SFPLOADI	L0, 0, 16256
	SFPLOADI	L1, 4, 0
	SFPSETCC	0, L0, 6
	SFPNOP
	SFPSETCC	0, L0, 4
	SFPNOP
	SFPSETCC	0, L0, 2
	SFPNOP
	SFPCOMPC
	SFPNOP
	SFPLOADI	L1, 4, 1
	SFPENCC	3, 10
	SFPNOP
	SFPSETCC	0, L1, 6
	SFPNOP
	SFPLOADI	L0, 0, 16384
	SFPENCC	3, 10
	SFPNOP
	SFPSTORE	L0, 0, 0
	ret
	.size	_Z21vcond_outside_inline3v, .-_Z21vcond_outside_inline3v
	.align	1
	.globl	_Z24vcond_outside_noninline3i
	.type	_Z24vcond_outside_noninline3i, @function
_Z24vcond_outside_noninline3i:
	SFPLOADI	L0, 0, 16256
	SFPLOADI	L1, 4, 0
	SFPSETCC	0, L0, 6
	SFPNOP
	SFPSETCC	0, L0, 4
	SFPNOP
	SFPSETCC	0, L0, 2
	SFPNOP
	SFPCOMPC
	SFPNOP
	SFPLOADI	L1, 4, 1
	SFPENCC	3, 10
	SFPNOP
	SFPSETCC	0, L1, 6
	SFPNOP
	SFPLOADI	L0, 0, 16384
	SFPENCC	3, 10
	SFPNOP
	SFPSTORE	L0, 0, 0
	ret
	.size	_Z24vcond_outside_noninline3i, .-_Z24vcond_outside_noninline3i
	.align	1
	.globl	_Z27vcond_inside_elseif_inline3v
	.type	_Z27vcond_inside_elseif_inline3v, @function
_Z27vcond_inside_elseif_inline3v:
	SFPLOADI	L0, 0, 16256
	SFPSETCC	0, L0, 6
	SFPNOP
	SFPLOADI	L0, 0, 16384
	SFPCOMPC
	SFPNOP
	SFPPUSHC
	SFPSETCC	0, L0, 6
	SFPNOP
	SFPSETCC	0, L0, 4
	SFPNOP
	SFPSETCC	0, L0, 2
	SFPNOP
	SFPCOMPC
	SFPNOP
	SFPLOADI	L0, 0, 16448
	SFPPOPC
	SFPNOP
	SFPENCC	3, 10
	SFPNOP
	SFPSTORE	L0, 0, 0
	ret
	.size	_Z27vcond_inside_elseif_inline3v, .-_Z27vcond_inside_elseif_inline3v
	.align	1
	.globl	_Z30vcond_inside_elseif_noninline3i
	.type	_Z30vcond_inside_elseif_noninline3i, @function
_Z30vcond_inside_elseif_noninline3i:
	SFPLOADI	L0, 0, 16256
	SFPSETCC	0, L0, 6
	SFPNOP
	SFPLOADI	L0, 0, 16384
	SFPCOMPC
	SFPNOP
	SFPPUSHC
	SFPSETCC	0, L0, 6
	SFPNOP
	SFPSETCC	0, L0, 4
	SFPNOP
	SFPSETCC	0, L0, 2
	SFPNOP
	SFPCOMPC
	SFPNOP
	SFPLOADI	L0, 0, 16448
	SFPPOPC
	SFPNOP
	SFPENCC	3, 10
	SFPNOP
	SFPSTORE	L0, 0, 0
	ret
	.size	_Z30vcond_inside_elseif_noninline3i, .-_Z30vcond_inside_elseif_noninline3i
	.align	1
	.globl	_Z28vcond_outside_elseif_inline3v
	.type	_Z28vcond_outside_elseif_inline3v, @function
_Z28vcond_outside_elseif_inline3v:
	SFPLOADI	L0, 0, 16256
	SFPLOADI	L1, 4, 0
	SFPSETCC	0, L0, 6
	SFPNOP
	SFPSETCC	0, L0, 4
	SFPNOP
	SFPSETCC	0, L0, 2
	SFPNOP
	SFPCOMPC
	SFPNOP
	SFPLOADI	L1, 4, 1
	SFPENCC	3, 10
	SFPNOP
	SFPSETCC	0, L0, 6
	SFPNOP
	SFPLOADI	L0, 0, 16384
	SFPCOMPC
	SFPNOP
	SFPSETCC	0, L1, 6
	SFPNOP
	SFPLOADI	L0, 0, 16448
	SFPENCC	3, 10
	SFPNOP
	SFPSTORE	L0, 0, 0
	ret
	.size	_Z28vcond_outside_elseif_inline3v, .-_Z28vcond_outside_elseif_inline3v
	.align	1
	.globl	_Z31vcond_outside_elseif_noninline3i
	.type	_Z31vcond_outside_elseif_noninline3i, @function
_Z31vcond_outside_elseif_noninline3i:
	SFPLOADI	L0, 0, 16256
	SFPLOADI	L1, 4, 0
	SFPSETCC	0, L0, 6
	SFPNOP
	SFPSETCC	0, L0, 4
	SFPNOP
	SFPSETCC	0, L0, 2
	SFPNOP
	SFPCOMPC
	SFPNOP
	SFPLOADI	L1, 4, 1
	SFPENCC	3, 10
	SFPNOP
	SFPSETCC	0, L0, 6
	SFPNOP
	SFPLOADI	L0, 0, 16384
	SFPCOMPC
	SFPNOP
	SFPSETCC	0, L1, 6
	SFPNOP
	SFPLOADI	L0, 0, 16448
	SFPENCC	3, 10
	SFPNOP
	SFPSTORE	L0, 0, 0
	ret
	.size	_Z31vcond_outside_elseif_noninline3i, .-_Z31vcond_outside_elseif_noninline3i
	.align	1
	.globl	_Z20vcond_inside_inline4v
	.type	_Z20vcond_inside_inline4v, @function
_Z20vcond_inside_inline4v:
	SFPLOADI	L0, 0, 16256
	SFPPUSHC
	SFPLOADI	L1, 0, 16448
	SFPMAD	L1, L11, L0, L1, 0
	SFPNOP
	SFPNOP
	SFPSETCC	0, L1, 0
	SFPNOP
	SFPLOADI	L1, 0, 16256
	SFPPUSHC
	SFPENCC	3, 2
	SFPNOP
	SFPMOV	L0, L2, 0
	SFPNOP
	SFPNOP
	SFPPOPC
	SFPNOP
	SFPMAD	L11, L1, L0, L2, 0
	SFPNOP
	SFPNOP
	SFPPOPC
	SFPNOP
	SFPPUSHC
	SFPLOADI	L1, 0, 16448
	SFPMAD	L1, L11, L2, L1, 0
	SFPNOP
	SFPNOP
	SFPSETCC	0, L1, 0
	SFPNOP
	SFPPOPC
	SFPNOP
	SFPSETCC	0, L2, 6
	SFPNOP
	SFPLOADI	L0, 0, 16384
	SFPENCC	3, 10
	SFPNOP
	SFPSTORE	L0, 0, 0
	ret
	.size	_Z20vcond_inside_inline4v, .-_Z20vcond_inside_inline4v
	.align	1
	.globl	_Z23vcond_inside_noninline4i
	.type	_Z23vcond_inside_noninline4i, @function
_Z23vcond_inside_noninline4i:
	SFPLOADI	L2, 0, 16256
	addi	a5,a0,-1
	beq	a0,zero,.L48
	SFPPUSHC
	SFPLOADI	L0, 0, 16448
	SFPMAD	L0, L11, L2, L0, 0
	SFPNOP
	SFPNOP
	SFPSETCC	0, L0, 0
	SFPNOP
	li	a4,3
	SFPPUSHC
	SFPENCC	3, 2
	SFPNOP
	SFPMOV	L2, L0, 0
	SFPNOP
	SFPNOP
	SFPPOPC
	SFPNOP
	beq	a5,a4,.L45
.L46:
	SFPLOADI	L1, 0, 16256
	SFPMAD	L11, L1, L0, L0, 0
	SFPNOP
	SFPNOP
	SFPPOPC
	SFPNOP
	beq	a5,zero,.L44
	SFPPUSHC
	SFPLOADI	L1, 0, 16448
	SFPMAD	L1, L11, L0, L1, 0
	SFPNOP
	SFPNOP
	SFPSETCC	0, L1, 0
	SFPNOP
	addi	a5,a5,-1
	bne	a5,a4,.L46
.L45:
	SFPPOPC
	SFPNOP
.L44:
	SFPPUSHC
	SFPLOADI	L1, 4, 0
	SFPSETCC	0, L0, 6
	SFPNOP
	SFPPUSHC
	SFPENCC	3, 2
	SFPNOP
	SFPMOV	L1, L0, 0
	SFPNOP
	SFPNOP
	SFPPOPC
	SFPNOP
	SFPLOADI	L0, 4, 1
	SFPPOPC
	SFPNOP
	SFPSETCC	0, L0, 2
	SFPNOP
	SFPLOADI	L2, 0, 16384
	SFPENCC	3, 10
	SFPNOP
	SFPSTORE	L2, 0, 0
	ret
.L48:
	SFPPUSHC
	SFPENCC	3, 2
	SFPNOP
	SFPMOV	L2, L0, 0
	SFPNOP
	SFPNOP
	SFPPOPC
	SFPNOP
	j	.L44
	.size	_Z23vcond_inside_noninline4i, .-_Z23vcond_inside_noninline4i
	.align	1
	.globl	_Z21vcond_outside_inline4v
	.type	_Z21vcond_outside_inline4v, @function
_Z21vcond_outside_inline4v:
	SFPLOADI	L0, 0, 16256
	SFPLOADI	L1, 0, 16448
	SFPMAD	L1, L11, L0, L1, 0
	SFPNOP
	SFPNOP
	SFPSETCC	0, L1, 0
	SFPNOP
	SFPLOADI	L1, 0, 16256
	SFPPUSHC
	SFPENCC	3, 2
	SFPNOP
	SFPMOV	L0, L2, 0
	SFPNOP
	SFPNOP
	SFPPOPC
	SFPNOP
	SFPMAD	L11, L1, L0, L2, 0
	SFPNOP
	SFPNOP
	SFPENCC	3, 10
	SFPNOP
	SFPLOADI	L1, 0, 16448
	SFPMAD	L1, L11, L2, L1, 0
	SFPNOP
	SFPNOP
	SFPSETCC	0, L1, 0
	SFPNOP
	SFPENCC	3, 10
	SFPNOP
	SFPLOADI	L1, 4, 0
	SFPSETCC	0, L2, 6
	SFPNOP
	SFPLOADI	L1, 4, 1
	SFPENCC	3, 10
	SFPNOP
	SFPSETCC	0, L1, 6
	SFPNOP
	SFPLOADI	L0, 0, 16384
	SFPENCC	3, 10
	SFPNOP
	SFPSTORE	L0, 0, 0
	ret
	.size	_Z21vcond_outside_inline4v, .-_Z21vcond_outside_inline4v
	.align	1
	.globl	_Z24vcond_outside_noninline4i
	.type	_Z24vcond_outside_noninline4i, @function
_Z24vcond_outside_noninline4i:
	SFPLOADI	L2, 0, 16256
	addi	a5,a0,-1
	beq	a0,zero,.L56
	SFPLOADI	L0, 0, 16448
	SFPMAD	L0, L11, L2, L0, 0
	SFPNOP
	SFPNOP
	SFPSETCC	0, L0, 0
	SFPNOP
	li	a4,3
	SFPPUSHC
	SFPENCC	3, 2
	SFPNOP
	SFPMOV	L2, L0, 0
	SFPNOP
	SFPNOP
	SFPPOPC
	SFPNOP
	beq	a5,a4,.L53
.L54:
	SFPLOADI	L1, 0, 16256
	SFPMAD	L11, L1, L0, L0, 0
	SFPNOP
	SFPNOP
	SFPENCC	3, 10
	SFPNOP
	beq	a5,zero,.L52
	SFPLOADI	L1, 0, 16448
	SFPMAD	L1, L11, L0, L1, 0
	SFPNOP
	SFPNOP
	SFPSETCC	0, L1, 0
	SFPNOP
	addi	a5,a5,-1
	bne	a5,a4,.L54
.L53:
	SFPENCC	3, 10
	SFPNOP
.L52:
	SFPLOADI	L1, 4, 0
	SFPSETCC	0, L0, 6
	SFPNOP
	SFPPUSHC
	SFPENCC	3, 2
	SFPNOP
	SFPMOV	L1, L0, 0
	SFPNOP
	SFPNOP
	SFPPOPC
	SFPNOP
	SFPLOADI	L0, 4, 1
	SFPENCC	3, 10
	SFPNOP
	SFPSETCC	0, L0, 6
	SFPNOP
	SFPLOADI	L2, 0, 16384
	SFPENCC	3, 10
	SFPNOP
	SFPSTORE	L2, 0, 0
	ret
.L56:
	SFPPUSHC
	SFPENCC	3, 2
	SFPNOP
	SFPMOV	L2, L0, 0
	SFPNOP
	SFPNOP
	SFPPOPC
	SFPNOP
	j	.L52
	.size	_Z24vcond_outside_noninline4i, .-_Z24vcond_outside_noninline4i
	.globl	_ZN7ckernel13instrn_bufferE
	.section	.sbss,"aw",@nobits
	.align	2
	.type	_ZN7ckernel13instrn_bufferE, @object
	.size	_ZN7ckernel13instrn_bufferE, 4
_ZN7ckernel13instrn_bufferE:
	.zero	4
	.ident	"GCC: (GNU) 10.2.0"
