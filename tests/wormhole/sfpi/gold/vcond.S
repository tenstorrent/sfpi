	.file	"vcond.cc"
	.option nopic
	.attribute arch, "rv32i2p0_m2p0_w"
	.attribute unaligned_access, 0
	.attribute stack_align, 16
	.text
	.align	1
	.globl	_Z19vcond_inside_inlinev
	.type	_Z19vcond_inside_inlinev, @function
_Z19vcond_inside_inlinev:
	SFPLOADI	L0, 16256, 0
	SFPSETCC	L0, 0, 4
	SFPLOADI	L0, 16384, 0
	SFPENCC	3, 10
	SFPSTORE	0, L0, 0, 3
	ret
	.size	_Z19vcond_inside_inlinev, .-_Z19vcond_inside_inlinev
	.align	1
	.globl	_Z22vcond_inside_noninlinei
	.type	_Z22vcond_inside_noninlinei, @function
_Z22vcond_inside_noninlinei:
	SFPLOADI	L0, 16256, 0
	bne	a0,zero,.L4
	SFPPUSHC	0
	SFPLOADI	L1, 0, 4
	SFPSETCC	L0, 0, 0
	SFPLOADI	L1, 1, 4
	SFPPOPC	0
	SFPSETCC	L1, 0, 2
	SFPLOADI	L0, 16384, 0
	SFPENCC	3, 10
	SFPSTORE	0, L0, 0, 3
	ret
.L4:
	SFPPUSHC	0
	SFPLOADI	L1, 0, 4
	SFPSETCC	L0, 0, 4
	SFPLOADI	L1, 1, 4
	SFPPOPC	0
	SFPSETCC	L1, 0, 2
	SFPLOADI	L0, 16384, 0
	SFPENCC	3, 10
	SFPSTORE	0, L0, 0, 3
	ret
	.size	_Z22vcond_inside_noninlinei, .-_Z22vcond_inside_noninlinei
	.align	1
	.globl	_Z20vcond_outside_inlinev
	.type	_Z20vcond_outside_inlinev, @function
_Z20vcond_outside_inlinev:
	SFPLOADI	L0, 16256, 0
	SFPLOADI	L1, 0, 4
	SFPSETCC	L0, 0, 4
	SFPLOADI	L1, 1, 4
	SFPENCC	3, 10
	SFPSETCC	L1, 0, 6
	SFPLOADI	L0, 16384, 0
	SFPENCC	3, 10
	SFPSTORE	0, L0, 0, 3
	ret
	.size	_Z20vcond_outside_inlinev, .-_Z20vcond_outside_inlinev
	.align	1
	.globl	_Z23vcond_outside_noninlinei
	.type	_Z23vcond_outside_noninlinei, @function
_Z23vcond_outside_noninlinei:
	SFPLOADI	L0, 16256, 0
	bne	a0,zero,.L8
	SFPLOADI	L1, 0, 4
	SFPSETCC	L0, 0, 0
	SFPLOADI	L1, 1, 4
	SFPENCC	3, 10
	SFPSETCC	L1, 0, 6
	SFPLOADI	L0, 16384, 0
	SFPENCC	3, 10
	SFPSTORE	0, L0, 0, 3
	ret
.L8:
	SFPLOADI	L1, 0, 4
	SFPSETCC	L0, 0, 4
	SFPLOADI	L1, 1, 4
	SFPENCC	3, 10
	SFPSETCC	L1, 0, 6
	SFPLOADI	L0, 16384, 0
	SFPENCC	3, 10
	SFPSTORE	0, L0, 0, 3
	ret
	.size	_Z23vcond_outside_noninlinei, .-_Z23vcond_outside_noninlinei
	.align	1
	.globl	_Z19vcond_orphan_inlinei
	.type	_Z19vcond_orphan_inlinei, @function
_Z19vcond_orphan_inlinei:
	SFPLOADI	L1, 16256, 0
	SFPLOADI	L0, 0, 4
	SFPSETCC	L1, 0, 4
	SFPLOADI	L0, 1, 4
	SFPENCC	3, 10
	ret
	.size	_Z19vcond_orphan_inlinei, .-_Z19vcond_orphan_inlinei
	.align	1
	.globl	_Z22vcond_orphan_noninlinei
	.type	_Z22vcond_orphan_noninlinei, @function
_Z22vcond_orphan_noninlinei:
	SFPLOADI	L0, 16256, 0
	bne	a0,zero,.L12
	SFPLOADI	L1, 0, 4
	SFPSETCC	L0, 0, 0
	SFPLOADI	L1, 1, 4
	SFPENCC	3, 10
	ret
.L12:
	SFPLOADI	L1, 0, 4
	SFPSETCC	L0, 0, 4
	SFPLOADI	L1, 1, 4
	SFPENCC	3, 10
	ret
	.size	_Z22vcond_orphan_noninlinei, .-_Z22vcond_orphan_noninlinei
	.align	1
	.globl	_Z19vcond_asbool_inlinei
	.type	_Z19vcond_asbool_inlinei, @function
_Z19vcond_asbool_inlinei:
	SFPLOADI	L1, 16256, 0
	SFPLOADI	L0, 0, 4
	SFPSETCC	L1, 0, 4
	SFPLOADI	L0, 1, 4
	SFPENCC	3, 10
	SFPSTORE	0, L0, 12, 3
	ret
	.size	_Z19vcond_asbool_inlinei, .-_Z19vcond_asbool_inlinei
	.align	1
	.globl	_Z22vcond_asbool_noninlinei
	.type	_Z22vcond_asbool_noninlinei, @function
_Z22vcond_asbool_noninlinei:
	SFPLOADI	L1, 16256, 0
	bne	a0,zero,.L16
	SFPLOADI	L0, 0, 4
	SFPSETCC	L1, 0, 0
	SFPLOADI	L0, 1, 4
	SFPENCC	3, 10
	SFPSTORE	0, L0, 12, 3
	ret
.L16:
	SFPLOADI	L0, 0, 4
	SFPSETCC	L1, 0, 4
	SFPLOADI	L0, 1, 4
	SFPENCC	3, 10
	SFPSTORE	0, L0, 12, 3
	ret
	.size	_Z22vcond_asbool_noninlinei, .-_Z22vcond_asbool_noninlinei
	.align	1
	.globl	_Z20vcond_inside_inline2v
	.type	_Z20vcond_inside_inline2v, @function
_Z20vcond_inside_inline2v:
	SFPLOADI	L0, 16256, 0
	SFPSETCC	L0, 0, 4
	SFPSETCC	L0, 0, 2
	SFPLOADI	L0, 16384, 0
	SFPENCC	3, 10
	SFPSTORE	0, L0, 0, 3
	ret
	.size	_Z20vcond_inside_inline2v, .-_Z20vcond_inside_inline2v
	.align	1
	.globl	_Z23vcond_inside_noninline2i
	.type	_Z23vcond_inside_noninline2i, @function
_Z23vcond_inside_noninline2i:
	SFPLOADI	L0, 16256, 0
	bne	a0,zero,.L20
	SFPPUSHC	0
	SFPLOADI	L1, 0, 4
	SFPSETCC	L0, 0, 0
	SFPSETCC	L0, 0, 2
	SFPLOADI	L1, 1, 4
	SFPPOPC	0
	SFPSETCC	L1, 0, 2
	SFPLOADI	L0, 16384, 0
	SFPENCC	3, 10
	SFPSTORE	0, L0, 0, 3
	ret
.L20:
	SFPPUSHC	0
	SFPLOADI	L1, 0, 4
	SFPSETCC	L0, 0, 4
	SFPSETCC	L0, 0, 2
	SFPLOADI	L1, 1, 4
	SFPPOPC	0
	SFPSETCC	L1, 0, 2
	SFPLOADI	L0, 16384, 0
	SFPENCC	3, 10
	SFPSTORE	0, L0, 0, 3
	ret
	.size	_Z23vcond_inside_noninline2i, .-_Z23vcond_inside_noninline2i
	.align	1
	.globl	_Z21vcond_outside_inline2v
	.type	_Z21vcond_outside_inline2v, @function
_Z21vcond_outside_inline2v:
	SFPLOADI	L0, 16256, 0
	SFPLOADI	L1, 0, 4
	SFPSETCC	L0, 0, 4
	SFPSETCC	L0, 0, 2
	SFPLOADI	L1, 1, 4
	SFPENCC	3, 10
	SFPSETCC	L1, 0, 6
	SFPLOADI	L0, 16384, 0
	SFPENCC	3, 10
	SFPSTORE	0, L0, 0, 3
	ret
	.size	_Z21vcond_outside_inline2v, .-_Z21vcond_outside_inline2v
	.align	1
	.globl	_Z24vcond_outside_noninline2i
	.type	_Z24vcond_outside_noninline2i, @function
_Z24vcond_outside_noninline2i:
	SFPLOADI	L0, 16256, 0
	bne	a0,zero,.L24
	SFPLOADI	L1, 0, 4
	SFPSETCC	L0, 0, 0
	SFPSETCC	L0, 0, 2
	SFPLOADI	L1, 1, 4
	SFPENCC	3, 10
	SFPSETCC	L1, 0, 6
	SFPLOADI	L0, 16384, 0
	SFPENCC	3, 10
	SFPSTORE	0, L0, 0, 3
	ret
.L24:
	SFPLOADI	L1, 0, 4
	SFPSETCC	L0, 0, 4
	SFPSETCC	L0, 0, 2
	SFPLOADI	L1, 1, 4
	SFPENCC	3, 10
	SFPSETCC	L1, 0, 6
	SFPLOADI	L0, 16384, 0
	SFPENCC	3, 10
	SFPSTORE	0, L0, 0, 3
	ret
	.size	_Z24vcond_outside_noninline2i, .-_Z24vcond_outside_noninline2i
	.align	1
	.globl	_Z20vcond_orphan_inline2i
	.type	_Z20vcond_orphan_inline2i, @function
_Z20vcond_orphan_inline2i:
	SFPLOADI	L1, 16256, 0
	SFPLOADI	L0, 0, 4
	SFPSETCC	L1, 0, 4
	SFPSETCC	L1, 0, 2
	SFPLOADI	L0, 1, 4
	SFPENCC	3, 10
	ret
	.size	_Z20vcond_orphan_inline2i, .-_Z20vcond_orphan_inline2i
	.align	1
	.globl	_Z23vcond_orphan_noninline2i
	.type	_Z23vcond_orphan_noninline2i, @function
_Z23vcond_orphan_noninline2i:
	SFPLOADI	L0, 16256, 0
	bne	a0,zero,.L28
	SFPLOADI	L1, 0, 4
	SFPSETCC	L0, 0, 0
	SFPSETCC	L0, 0, 2
	SFPLOADI	L1, 1, 4
	SFPENCC	3, 10
	ret
.L28:
	SFPLOADI	L1, 0, 4
	SFPSETCC	L0, 0, 4
	SFPSETCC	L0, 0, 2
	SFPLOADI	L1, 1, 4
	SFPENCC	3, 10
	ret
	.size	_Z23vcond_orphan_noninline2i, .-_Z23vcond_orphan_noninline2i
	.align	1
	.globl	_Z20vcond_asbool_inline2i
	.type	_Z20vcond_asbool_inline2i, @function
_Z20vcond_asbool_inline2i:
	SFPLOADI	L1, 16256, 0
	SFPLOADI	L0, 0, 4
	SFPSETCC	L1, 0, 4
	SFPSETCC	L1, 0, 2
	SFPLOADI	L0, 1, 4
	SFPENCC	3, 10
	SFPSTORE	0, L0, 12, 3
	ret
	.size	_Z20vcond_asbool_inline2i, .-_Z20vcond_asbool_inline2i
	.align	1
	.globl	_Z23vcond_asbool_noninline2i
	.type	_Z23vcond_asbool_noninline2i, @function
_Z23vcond_asbool_noninline2i:
	SFPLOADI	L1, 16256, 0
	bne	a0,zero,.L32
	SFPLOADI	L0, 0, 4
	SFPSETCC	L1, 0, 0
	SFPSETCC	L1, 0, 2
	SFPLOADI	L0, 1, 4
	SFPENCC	3, 10
	SFPSTORE	0, L0, 12, 3
	ret
.L32:
	SFPLOADI	L0, 0, 4
	SFPSETCC	L1, 0, 4
	SFPSETCC	L1, 0, 2
	SFPLOADI	L0, 1, 4
	SFPENCC	3, 10
	SFPSTORE	0, L0, 12, 3
	ret
	.size	_Z23vcond_asbool_noninline2i, .-_Z23vcond_asbool_noninline2i
	.align	1
	.globl	_Z20vcond_inside_inline3v
	.type	_Z20vcond_inside_inline3v, @function
_Z20vcond_inside_inline3v:
	SFPLOADI	L0, 16256, 0
	SFPSETCC	L0, 0, 6
	SFPSETCC	L0, 0, 4
	SFPSETCC	L0, 0, 2
	SFPCOMPC
	SFPLOADI	L0, 16384, 0
	SFPENCC	3, 10
	SFPSTORE	0, L0, 0, 3
	ret
	.size	_Z20vcond_inside_inline3v, .-_Z20vcond_inside_inline3v
	.align	1
	.globl	_Z23vcond_inside_noninline3i
	.type	_Z23vcond_inside_noninline3i, @function
_Z23vcond_inside_noninline3i:
	SFPLOADI	L0, 16256, 0
	SFPPUSHC	0
	SFPLOADI	L1, 0, 4
	SFPSETCC	L0, 0, 6
	SFPSETCC	L0, 0, 4
	SFPSETCC	L0, 0, 2
	SFPCOMPC
	SFPLOADI	L1, 1, 4
	SFPPOPC	0
	SFPSETCC	L1, 0, 2
	SFPLOADI	L0, 16384, 0
	SFPENCC	3, 10
	SFPSTORE	0, L0, 0, 3
	ret
	.size	_Z23vcond_inside_noninline3i, .-_Z23vcond_inside_noninline3i
	.align	1
	.globl	_Z21vcond_outside_inline3v
	.type	_Z21vcond_outside_inline3v, @function
_Z21vcond_outside_inline3v:
	SFPLOADI	L0, 16256, 0
	SFPLOADI	L1, 0, 4
	SFPSETCC	L0, 0, 6
	SFPSETCC	L0, 0, 4
	SFPSETCC	L0, 0, 2
	SFPCOMPC
	SFPLOADI	L1, 1, 4
	SFPENCC	3, 10
	SFPSETCC	L1, 0, 6
	SFPLOADI	L0, 16384, 0
	SFPENCC	3, 10
	SFPSTORE	0, L0, 0, 3
	ret
	.size	_Z21vcond_outside_inline3v, .-_Z21vcond_outside_inline3v
	.align	1
	.globl	_Z24vcond_outside_noninline3i
	.type	_Z24vcond_outside_noninline3i, @function
_Z24vcond_outside_noninline3i:
	SFPLOADI	L0, 16256, 0
	SFPLOADI	L1, 0, 4
	SFPSETCC	L0, 0, 6
	SFPSETCC	L0, 0, 4
	SFPSETCC	L0, 0, 2
	SFPCOMPC
	SFPLOADI	L1, 1, 4
	SFPENCC	3, 10
	SFPSETCC	L1, 0, 6
	SFPLOADI	L0, 16384, 0
	SFPENCC	3, 10
	SFPSTORE	0, L0, 0, 3
	ret
	.size	_Z24vcond_outside_noninline3i, .-_Z24vcond_outside_noninline3i
	.align	1
	.globl	_Z27vcond_inside_elseif_inline3v
	.type	_Z27vcond_inside_elseif_inline3v, @function
_Z27vcond_inside_elseif_inline3v:
	SFPLOADI	L0, 16256, 0
	SFPSETCC	L0, 0, 6
	SFPLOADI	L0, 16384, 0
	SFPCOMPC
	SFPPUSHC	0
	SFPSETCC	L0, 0, 6
	SFPSETCC	L0, 0, 4
	SFPSETCC	L0, 0, 2
	SFPCOMPC
	SFPLOADI	L0, 16448, 0
	SFPPOPC	0
	SFPENCC	3, 10
	SFPSTORE	0, L0, 0, 3
	ret
	.size	_Z27vcond_inside_elseif_inline3v, .-_Z27vcond_inside_elseif_inline3v
	.align	1
	.globl	_Z30vcond_inside_elseif_noninline3i
	.type	_Z30vcond_inside_elseif_noninline3i, @function
_Z30vcond_inside_elseif_noninline3i:
	SFPLOADI	L0, 16256, 0
	SFPSETCC	L0, 0, 6
	SFPLOADI	L0, 16384, 0
	SFPCOMPC
	SFPPUSHC	0
	SFPPUSHC	0
	SFPLOADI	L1, 0, 4
	SFPSETCC	L0, 0, 6
	SFPSETCC	L0, 0, 4
	SFPSETCC	L0, 0, 2
	SFPCOMPC
	SFPLOADI	L1, 1, 4
	SFPPOPC	0
	SFPSETCC	L1, 0, 2
	SFPLOADI	L0, 16448, 0
	SFPPOPC	0
	SFPENCC	3, 10
	SFPSTORE	0, L0, 0, 3
	ret
	.size	_Z30vcond_inside_elseif_noninline3i, .-_Z30vcond_inside_elseif_noninline3i
	.align	1
	.globl	_Z28vcond_outside_elseif_inline3v
	.type	_Z28vcond_outside_elseif_inline3v, @function
_Z28vcond_outside_elseif_inline3v:
	SFPLOADI	L0, 16256, 0
	SFPLOADI	L1, 0, 4
	SFPSETCC	L0, 0, 6
	SFPSETCC	L0, 0, 4
	SFPSETCC	L0, 0, 2
	SFPCOMPC
	SFPLOADI	L1, 1, 4
	SFPENCC	3, 10
	SFPSETCC	L0, 0, 6
	SFPLOADI	L0, 16384, 0
	SFPCOMPC
	SFPSETCC	L1, 0, 6
	SFPLOADI	L0, 16448, 0
	SFPENCC	3, 10
	SFPSTORE	0, L0, 0, 3
	ret
	.size	_Z28vcond_outside_elseif_inline3v, .-_Z28vcond_outside_elseif_inline3v
	.align	1
	.globl	_Z31vcond_outside_elseif_noninline3i
	.type	_Z31vcond_outside_elseif_noninline3i, @function
_Z31vcond_outside_elseif_noninline3i:
	SFPLOADI	L0, 16256, 0
	SFPLOADI	L1, 0, 4
	SFPSETCC	L0, 0, 6
	SFPSETCC	L0, 0, 4
	SFPSETCC	L0, 0, 2
	SFPCOMPC
	SFPLOADI	L1, 1, 4
	SFPENCC	3, 10
	SFPSETCC	L0, 0, 6
	SFPLOADI	L0, 16384, 0
	SFPCOMPC
	SFPSETCC	L1, 0, 6
	SFPLOADI	L0, 16448, 0
	SFPENCC	3, 10
	SFPSTORE	0, L0, 0, 3
	ret
	.size	_Z31vcond_outside_elseif_noninline3i, .-_Z31vcond_outside_elseif_noninline3i
	.align	1
	.globl	_Z20vcond_inside_inline4v
	.type	_Z20vcond_inside_inline4v, @function
_Z20vcond_inside_inline4v:
	SFPLOADI	L0, 16256, 0
	SFPPUSHC	0
	SFPLOADI	L1, 16448, 0
	SFPMAD	L1, L1, L11, L0, 0
	SFPNOP
	SFPSETCC	L1, 0, 0
	SFPLOADI	L1, 16256, 0
	SFPMOV	L2, L0, 2
	SFPMAD	L2, L11, L1, L0, 0
	SFPPOPC	0
	SFPPUSHC	0
	SFPLOADI	L1, 16448, 0
	SFPMAD	L1, L1, L11, L2, 0
	SFPNOP
	SFPSETCC	L1, 0, 0
	SFPPOPC	0
	SFPSETCC	L2, 0, 6
	SFPLOADI	L0, 16384, 0
	SFPENCC	3, 10
	SFPSTORE	0, L0, 0, 3
	ret
	.size	_Z20vcond_inside_inline4v, .-_Z20vcond_inside_inline4v
	.align	1
	.globl	_Z23vcond_inside_noninline4i
	.type	_Z23vcond_inside_noninline4i, @function
_Z23vcond_inside_noninline4i:
	SFPLOADI	L2, 16256, 0
	addi	a5,a0,-1
	SFPMOV	L0, L2, 2
	beq	a0,zero,.L53
	SFPPUSHC	0
	SFPLOADI	L0, 16448, 0
	SFPMAD	L0, L0, L11, L2, 0
	SFPNOP
	SFPSETCC	L0, 0, 0
	li	a4,3
	SFPMOV	L0, L2, 2
	bne	a5,a4,.L56
	j	.L54
.L55:
	SFPPUSHC	0
	SFPLOADI	L1, 16448, 0
	SFPMAD	L1, L1, L11, L0, 0
	SFPNOP
	SFPSETCC	L1, 0, 0
	addi	a5,a5,-1
	beq	a5,a4,.L54
.L56:
	SFPLOADI	L1, 16256, 0
	SFPMAD	L0, L11, L1, L0, 0
	SFPPOPC	0
	bne	a5,zero,.L55
.L53:
	SFPPUSHC	0
	SFPLOADI	L1, 0, 4
	SFPSETCC	L0, 0, 6
	SFPMOV	L0, L1, 2
	SFPLOADI	L0, 1, 4
	SFPPOPC	0
	SFPSETCC	L0, 0, 2
	SFPLOADI	L2, 16384, 0
	SFPENCC	3, 10
	SFPSTORE	0, L2, 0, 3
	ret
.L54:
	SFPPOPC	0
	j	.L53
	.size	_Z23vcond_inside_noninline4i, .-_Z23vcond_inside_noninline4i
	.align	1
	.globl	_Z21vcond_outside_inline4v
	.type	_Z21vcond_outside_inline4v, @function
_Z21vcond_outside_inline4v:
	SFPLOADI	L0, 16256, 0
	SFPLOADI	L1, 16448, 0
	SFPMAD	L1, L1, L11, L0, 0
	SFPNOP
	SFPSETCC	L1, 0, 0
	SFPLOADI	L1, 16256, 0
	SFPMOV	L2, L0, 2
	SFPMAD	L2, L11, L1, L0, 0
	SFPENCC	3, 10
	SFPLOADI	L1, 16448, 0
	SFPMAD	L1, L1, L11, L2, 0
	SFPNOP
	SFPSETCC	L1, 0, 0
	SFPENCC	3, 10
	SFPLOADI	L1, 0, 4
	SFPSETCC	L2, 0, 6
	SFPLOADI	L1, 1, 4
	SFPENCC	3, 10
	SFPSETCC	L1, 0, 6
	SFPLOADI	L0, 16384, 0
	SFPENCC	3, 10
	SFPSTORE	0, L0, 0, 3
	ret
	.size	_Z21vcond_outside_inline4v, .-_Z21vcond_outside_inline4v
	.align	1
	.globl	_Z24vcond_outside_noninline4i
	.type	_Z24vcond_outside_noninline4i, @function
_Z24vcond_outside_noninline4i:
	SFPLOADI	L2, 16256, 0
	addi	a5,a0,-1
	SFPMOV	L0, L2, 2
	beq	a0,zero,.L61
	SFPLOADI	L0, 16448, 0
	SFPMAD	L0, L0, L11, L2, 0
	SFPNOP
	SFPSETCC	L0, 0, 0
	li	a4,3
	SFPMOV	L0, L2, 2
	bne	a5,a4,.L64
	j	.L62
.L63:
	SFPLOADI	L1, 16448, 0
	SFPMAD	L1, L1, L11, L0, 0
	SFPNOP
	SFPSETCC	L1, 0, 0
	addi	a5,a5,-1
	beq	a5,a4,.L62
.L64:
	SFPLOADI	L1, 16256, 0
	SFPMAD	L0, L11, L1, L0, 0
	SFPENCC	3, 10
	bne	a5,zero,.L63
.L61:
	SFPLOADI	L1, 0, 4
	SFPSETCC	L0, 0, 6
	SFPMOV	L0, L1, 2
	SFPLOADI	L0, 1, 4
	SFPENCC	3, 10
	SFPSETCC	L0, 0, 6
	SFPLOADI	L2, 16384, 0
	SFPENCC	3, 10
	SFPSTORE	0, L2, 0, 3
	ret
.L62:
	SFPENCC	3, 10
	j	.L61
	.size	_Z24vcond_outside_noninline4i, .-_Z24vcond_outside_noninline4i
	.globl	_ZN7ckernel13instrn_bufferE
	.section	.sbss,"aw",@nobits
	.align	2
	.type	_ZN7ckernel13instrn_bufferE, @object
	.size	_ZN7ckernel13instrn_bufferE, 4
_ZN7ckernel13instrn_bufferE:
	.zero	4
	.ident	"GCC: (gcdc243316e1) 12.2.0"
