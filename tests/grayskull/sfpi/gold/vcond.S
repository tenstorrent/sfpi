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
	SFPLOADI	L0, 16256, 0
	SFPSETCC	L0, 0, 4
	TTNOP
	SFPLOADI	L0, 16384, 0
	SFPENCC	3, 10
	TTNOP
	SFPSTORE	0, L0, 0
	ret
	.size	_Z19vcond_inside_inlinev, .-_Z19vcond_inside_inlinev
	.align	1
	.globl	_Z22vcond_inside_noninlinei
	.type	_Z22vcond_inside_noninlinei, @function
_Z22vcond_inside_noninlinei:
	SFPLOADI	L0, 16256, 0
	bne	a0,zero,.L4
	SFPPUSHC
	SFPLOADI	L1, 0, 4
	SFPSETCC	L0, 0, 0
	TTNOP
	SFPLOADI	L1, 1, 4
	SFPPOPC
	TTNOP
	SFPSETCC	L1, 0, 2
	TTNOP
	SFPLOADI	L0, 16384, 0
	SFPENCC	3, 10
	TTNOP
	SFPSTORE	0, L0, 0
	ret
.L4:
	SFPPUSHC
	SFPLOADI	L1, 0, 4
	SFPSETCC	L0, 0, 4
	TTNOP
	SFPLOADI	L1, 1, 4
	SFPPOPC
	TTNOP
	SFPSETCC	L1, 0, 2
	TTNOP
	SFPLOADI	L0, 16384, 0
	SFPENCC	3, 10
	TTNOP
	SFPSTORE	0, L0, 0
	ret
	.size	_Z22vcond_inside_noninlinei, .-_Z22vcond_inside_noninlinei
	.align	1
	.globl	_Z20vcond_outside_inlinev
	.type	_Z20vcond_outside_inlinev, @function
_Z20vcond_outside_inlinev:
	SFPLOADI	L0, 16256, 0
	SFPLOADI	L1, 0, 4
	SFPSETCC	L0, 0, 4
	TTNOP
	SFPLOADI	L1, 1, 4
	SFPENCC	3, 10
	TTNOP
	SFPSETCC	L1, 0, 6
	TTNOP
	SFPLOADI	L0, 16384, 0
	SFPENCC	3, 10
	TTNOP
	SFPSTORE	0, L0, 0
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
	TTNOP
	SFPLOADI	L1, 1, 4
	SFPENCC	3, 10
	TTNOP
	SFPSETCC	L1, 0, 6
	TTNOP
	SFPLOADI	L0, 16384, 0
	SFPENCC	3, 10
	TTNOP
	SFPSTORE	0, L0, 0
	ret
.L8:
	SFPLOADI	L1, 0, 4
	SFPSETCC	L0, 0, 4
	TTNOP
	SFPLOADI	L1, 1, 4
	SFPENCC	3, 10
	TTNOP
	SFPSETCC	L1, 0, 6
	TTNOP
	SFPLOADI	L0, 16384, 0
	SFPENCC	3, 10
	TTNOP
	SFPSTORE	0, L0, 0
	ret
	.size	_Z23vcond_outside_noninlinei, .-_Z23vcond_outside_noninlinei
	.align	1
	.globl	_Z19vcond_orphan_inlinei
	.type	_Z19vcond_orphan_inlinei, @function
_Z19vcond_orphan_inlinei:
	SFPLOADI	L1, 16256, 0
	SFPLOADI	L0, 0, 4
	SFPSETCC	L1, 0, 4
	TTNOP
	SFPLOADI	L0, 1, 4
	SFPENCC	3, 10
	TTNOP
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
	TTNOP
	SFPLOADI	L1, 1, 4
	SFPENCC	3, 10
	TTNOP
	ret
.L12:
	SFPLOADI	L1, 0, 4
	SFPSETCC	L0, 0, 4
	TTNOP
	SFPLOADI	L1, 1, 4
	SFPENCC	3, 10
	TTNOP
	ret
	.size	_Z22vcond_orphan_noninlinei, .-_Z22vcond_orphan_noninlinei
	.align	1
	.globl	_Z19vcond_asbool_inlinei
	.type	_Z19vcond_asbool_inlinei, @function
_Z19vcond_asbool_inlinei:
	SFPLOADI	L1, 16256, 0
	SFPLOADI	L0, 0, 4
	SFPSETCC	L1, 0, 4
	TTNOP
	SFPLOADI	L0, 1, 4
	SFPENCC	3, 10
	TTNOP
	SFPSTORE	0, L0, 2
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
	TTNOP
	SFPLOADI	L0, 1, 4
	SFPENCC	3, 10
	TTNOP
	SFPSTORE	0, L0, 2
	ret
.L16:
	SFPLOADI	L0, 0, 4
	SFPSETCC	L1, 0, 4
	TTNOP
	SFPLOADI	L0, 1, 4
	SFPENCC	3, 10
	TTNOP
	SFPSTORE	0, L0, 2
	ret
	.size	_Z22vcond_asbool_noninlinei, .-_Z22vcond_asbool_noninlinei
	.align	1
	.globl	_Z20vcond_inside_inline2v
	.type	_Z20vcond_inside_inline2v, @function
_Z20vcond_inside_inline2v:
	SFPLOADI	L0, 16256, 0
	SFPSETCC	L0, 0, 4
	TTNOP
	SFPSETCC	L0, 0, 2
	TTNOP
	SFPLOADI	L0, 16384, 0
	SFPENCC	3, 10
	TTNOP
	SFPSTORE	0, L0, 0
	ret
	.size	_Z20vcond_inside_inline2v, .-_Z20vcond_inside_inline2v
	.align	1
	.globl	_Z23vcond_inside_noninline2i
	.type	_Z23vcond_inside_noninline2i, @function
_Z23vcond_inside_noninline2i:
	SFPLOADI	L0, 16256, 0
	bne	a0,zero,.L20
	SFPPUSHC
	SFPLOADI	L1, 0, 4
	SFPSETCC	L0, 0, 0
	TTNOP
	SFPSETCC	L0, 0, 2
	TTNOP
	SFPLOADI	L1, 1, 4
	SFPPOPC
	TTNOP
	SFPSETCC	L1, 0, 2
	TTNOP
	SFPLOADI	L0, 16384, 0
	SFPENCC	3, 10
	TTNOP
	SFPSTORE	0, L0, 0
	ret
.L20:
	SFPPUSHC
	SFPLOADI	L1, 0, 4
	SFPSETCC	L0, 0, 4
	TTNOP
	SFPSETCC	L0, 0, 2
	TTNOP
	SFPLOADI	L1, 1, 4
	SFPPOPC
	TTNOP
	SFPSETCC	L1, 0, 2
	TTNOP
	SFPLOADI	L0, 16384, 0
	SFPENCC	3, 10
	TTNOP
	SFPSTORE	0, L0, 0
	ret
	.size	_Z23vcond_inside_noninline2i, .-_Z23vcond_inside_noninline2i
	.align	1
	.globl	_Z21vcond_outside_inline2v
	.type	_Z21vcond_outside_inline2v, @function
_Z21vcond_outside_inline2v:
	SFPLOADI	L0, 16256, 0
	SFPLOADI	L1, 0, 4
	SFPSETCC	L0, 0, 4
	TTNOP
	SFPSETCC	L0, 0, 2
	TTNOP
	SFPLOADI	L1, 1, 4
	SFPENCC	3, 10
	TTNOP
	SFPSETCC	L1, 0, 6
	TTNOP
	SFPLOADI	L0, 16384, 0
	SFPENCC	3, 10
	TTNOP
	SFPSTORE	0, L0, 0
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
	TTNOP
	SFPSETCC	L0, 0, 2
	TTNOP
	SFPLOADI	L1, 1, 4
	SFPENCC	3, 10
	TTNOP
	SFPSETCC	L1, 0, 6
	TTNOP
	SFPLOADI	L0, 16384, 0
	SFPENCC	3, 10
	TTNOP
	SFPSTORE	0, L0, 0
	ret
.L24:
	SFPLOADI	L1, 0, 4
	SFPSETCC	L0, 0, 4
	TTNOP
	SFPSETCC	L0, 0, 2
	TTNOP
	SFPLOADI	L1, 1, 4
	SFPENCC	3, 10
	TTNOP
	SFPSETCC	L1, 0, 6
	TTNOP
	SFPLOADI	L0, 16384, 0
	SFPENCC	3, 10
	TTNOP
	SFPSTORE	0, L0, 0
	ret
	.size	_Z24vcond_outside_noninline2i, .-_Z24vcond_outside_noninline2i
	.align	1
	.globl	_Z20vcond_orphan_inline2i
	.type	_Z20vcond_orphan_inline2i, @function
_Z20vcond_orphan_inline2i:
	SFPLOADI	L1, 16256, 0
	SFPLOADI	L0, 0, 4
	SFPSETCC	L1, 0, 4
	TTNOP
	SFPSETCC	L1, 0, 2
	TTNOP
	SFPLOADI	L0, 1, 4
	SFPENCC	3, 10
	TTNOP
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
	TTNOP
	SFPSETCC	L0, 0, 2
	TTNOP
	SFPLOADI	L1, 1, 4
	SFPENCC	3, 10
	TTNOP
	ret
.L28:
	SFPLOADI	L1, 0, 4
	SFPSETCC	L0, 0, 4
	TTNOP
	SFPSETCC	L0, 0, 2
	TTNOP
	SFPLOADI	L1, 1, 4
	SFPENCC	3, 10
	TTNOP
	ret
	.size	_Z23vcond_orphan_noninline2i, .-_Z23vcond_orphan_noninline2i
	.align	1
	.globl	_Z20vcond_asbool_inline2i
	.type	_Z20vcond_asbool_inline2i, @function
_Z20vcond_asbool_inline2i:
	SFPLOADI	L1, 16256, 0
	SFPLOADI	L0, 0, 4
	SFPSETCC	L1, 0, 4
	TTNOP
	SFPSETCC	L1, 0, 2
	TTNOP
	SFPLOADI	L0, 1, 4
	SFPENCC	3, 10
	TTNOP
	SFPSTORE	0, L0, 2
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
	TTNOP
	SFPSETCC	L1, 0, 2
	TTNOP
	SFPLOADI	L0, 1, 4
	SFPENCC	3, 10
	TTNOP
	SFPSTORE	0, L0, 2
	ret
.L32:
	SFPLOADI	L0, 0, 4
	SFPSETCC	L1, 0, 4
	TTNOP
	SFPSETCC	L1, 0, 2
	TTNOP
	SFPLOADI	L0, 1, 4
	SFPENCC	3, 10
	TTNOP
	SFPSTORE	0, L0, 2
	ret
	.size	_Z23vcond_asbool_noninline2i, .-_Z23vcond_asbool_noninline2i
	.align	1
	.globl	_Z20vcond_inside_inline3v
	.type	_Z20vcond_inside_inline3v, @function
_Z20vcond_inside_inline3v:
	SFPLOADI	L0, 16256, 0
	SFPSETCC	L0, 0, 6
	TTNOP
	SFPSETCC	L0, 0, 4
	TTNOP
	SFPSETCC	L0, 0, 2
	TTNOP
	SFPCOMPC
	TTNOP
	SFPLOADI	L0, 16384, 0
	SFPENCC	3, 10
	TTNOP
	SFPSTORE	0, L0, 0
	ret
	.size	_Z20vcond_inside_inline3v, .-_Z20vcond_inside_inline3v
	.align	1
	.globl	_Z23vcond_inside_noninline3i
	.type	_Z23vcond_inside_noninline3i, @function
_Z23vcond_inside_noninline3i:
	SFPLOADI	L0, 16256, 0
	SFPSETCC	L0, 0, 6
	TTNOP
	SFPSETCC	L0, 0, 4
	TTNOP
	SFPSETCC	L0, 0, 2
	TTNOP
	SFPCOMPC
	TTNOP
	SFPLOADI	L0, 16384, 0
	SFPENCC	3, 10
	TTNOP
	SFPSTORE	0, L0, 0
	ret
	.size	_Z23vcond_inside_noninline3i, .-_Z23vcond_inside_noninline3i
	.align	1
	.globl	_Z21vcond_outside_inline3v
	.type	_Z21vcond_outside_inline3v, @function
_Z21vcond_outside_inline3v:
	SFPLOADI	L0, 16256, 0
	SFPLOADI	L1, 0, 4
	SFPSETCC	L0, 0, 6
	TTNOP
	SFPSETCC	L0, 0, 4
	TTNOP
	SFPSETCC	L0, 0, 2
	TTNOP
	SFPCOMPC
	TTNOP
	SFPLOADI	L1, 1, 4
	SFPENCC	3, 10
	TTNOP
	SFPSETCC	L1, 0, 6
	TTNOP
	SFPLOADI	L0, 16384, 0
	SFPENCC	3, 10
	TTNOP
	SFPSTORE	0, L0, 0
	ret
	.size	_Z21vcond_outside_inline3v, .-_Z21vcond_outside_inline3v
	.align	1
	.globl	_Z24vcond_outside_noninline3i
	.type	_Z24vcond_outside_noninline3i, @function
_Z24vcond_outside_noninline3i:
	SFPLOADI	L0, 16256, 0
	SFPLOADI	L1, 0, 4
	SFPSETCC	L0, 0, 6
	TTNOP
	SFPSETCC	L0, 0, 4
	TTNOP
	SFPSETCC	L0, 0, 2
	TTNOP
	SFPCOMPC
	TTNOP
	SFPLOADI	L1, 1, 4
	SFPENCC	3, 10
	TTNOP
	SFPSETCC	L1, 0, 6
	TTNOP
	SFPLOADI	L0, 16384, 0
	SFPENCC	3, 10
	TTNOP
	SFPSTORE	0, L0, 0
	ret
	.size	_Z24vcond_outside_noninline3i, .-_Z24vcond_outside_noninline3i
	.align	1
	.globl	_Z27vcond_inside_elseif_inline3v
	.type	_Z27vcond_inside_elseif_inline3v, @function
_Z27vcond_inside_elseif_inline3v:
	SFPLOADI	L0, 16256, 0
	SFPSETCC	L0, 0, 6
	TTNOP
	SFPLOADI	L0, 16384, 0
	SFPCOMPC
	TTNOP
	SFPPUSHC
	SFPSETCC	L0, 0, 6
	TTNOP
	SFPSETCC	L0, 0, 4
	TTNOP
	SFPSETCC	L0, 0, 2
	TTNOP
	SFPCOMPC
	TTNOP
	SFPLOADI	L0, 16448, 0
	SFPPOPC
	TTNOP
	SFPENCC	3, 10
	TTNOP
	SFPSTORE	0, L0, 0
	ret
	.size	_Z27vcond_inside_elseif_inline3v, .-_Z27vcond_inside_elseif_inline3v
	.align	1
	.globl	_Z30vcond_inside_elseif_noninline3i
	.type	_Z30vcond_inside_elseif_noninline3i, @function
_Z30vcond_inside_elseif_noninline3i:
	SFPLOADI	L0, 16256, 0
	SFPSETCC	L0, 0, 6
	TTNOP
	SFPLOADI	L0, 16384, 0
	SFPCOMPC
	TTNOP
	SFPPUSHC
	SFPSETCC	L0, 0, 6
	TTNOP
	SFPSETCC	L0, 0, 4
	TTNOP
	SFPSETCC	L0, 0, 2
	TTNOP
	SFPCOMPC
	TTNOP
	SFPLOADI	L0, 16448, 0
	SFPPOPC
	TTNOP
	SFPENCC	3, 10
	TTNOP
	SFPSTORE	0, L0, 0
	ret
	.size	_Z30vcond_inside_elseif_noninline3i, .-_Z30vcond_inside_elseif_noninline3i
	.align	1
	.globl	_Z28vcond_outside_elseif_inline3v
	.type	_Z28vcond_outside_elseif_inline3v, @function
_Z28vcond_outside_elseif_inline3v:
	SFPLOADI	L0, 16256, 0
	SFPLOADI	L1, 0, 4
	SFPSETCC	L0, 0, 6
	TTNOP
	SFPSETCC	L0, 0, 4
	TTNOP
	SFPSETCC	L0, 0, 2
	TTNOP
	SFPCOMPC
	TTNOP
	SFPLOADI	L1, 1, 4
	SFPENCC	3, 10
	TTNOP
	SFPSETCC	L0, 0, 6
	TTNOP
	SFPLOADI	L0, 16384, 0
	SFPCOMPC
	TTNOP
	SFPSETCC	L1, 0, 6
	TTNOP
	SFPLOADI	L0, 16448, 0
	SFPENCC	3, 10
	TTNOP
	SFPSTORE	0, L0, 0
	ret
	.size	_Z28vcond_outside_elseif_inline3v, .-_Z28vcond_outside_elseif_inline3v
	.align	1
	.globl	_Z31vcond_outside_elseif_noninline3i
	.type	_Z31vcond_outside_elseif_noninline3i, @function
_Z31vcond_outside_elseif_noninline3i:
	SFPLOADI	L0, 16256, 0
	SFPLOADI	L1, 0, 4
	SFPSETCC	L0, 0, 6
	TTNOP
	SFPSETCC	L0, 0, 4
	TTNOP
	SFPSETCC	L0, 0, 2
	TTNOP
	SFPCOMPC
	TTNOP
	SFPLOADI	L1, 1, 4
	SFPENCC	3, 10
	TTNOP
	SFPSETCC	L0, 0, 6
	TTNOP
	SFPLOADI	L0, 16384, 0
	SFPCOMPC
	TTNOP
	SFPSETCC	L1, 0, 6
	TTNOP
	SFPLOADI	L0, 16448, 0
	SFPENCC	3, 10
	TTNOP
	SFPSTORE	0, L0, 0
	ret
	.size	_Z31vcond_outside_elseif_noninline3i, .-_Z31vcond_outside_elseif_noninline3i
	.align	1
	.globl	_Z20vcond_inside_inline4v
	.type	_Z20vcond_inside_inline4v, @function
_Z20vcond_inside_inline4v:
	SFPLOADI	L0, 16256, 0
	SFPPUSHC
	SFPLOADI	L1, 16448, 0
	SFPMAD	L1, L1, L11, L0, 0
	TTNOP
	TTNOP
	SFPSETCC	L1, 0, 0
	TTNOP
	SFPLOADI	L1, 16256, 0
	SFPPUSHC
	SFPENCC	3, 2
	TTNOP
	SFPMOV	L2, L0, 0
	TTNOP
	TTNOP
	SFPPOPC
	TTNOP
	SFPMAD	L2, L11, L1, L0, 0
	TTNOP
	TTNOP
	SFPPOPC
	TTNOP
	SFPPUSHC
	SFPLOADI	L1, 16448, 0
	SFPMAD	L1, L1, L11, L2, 0
	TTNOP
	TTNOP
	SFPSETCC	L1, 0, 0
	TTNOP
	SFPPOPC
	TTNOP
	SFPSETCC	L2, 0, 6
	TTNOP
	SFPLOADI	L0, 16384, 0
	SFPENCC	3, 10
	TTNOP
	SFPSTORE	0, L0, 0
	ret
	.size	_Z20vcond_inside_inline4v, .-_Z20vcond_inside_inline4v
	.align	1
	.globl	_Z23vcond_inside_noninline4i
	.type	_Z23vcond_inside_noninline4i, @function
_Z23vcond_inside_noninline4i:
	SFPLOADI	L2, 16256, 0
	addi	a5,a0,-1
	beq	a0,zero,.L48
	SFPPUSHC
	SFPLOADI	L0, 16448, 0
	SFPMAD	L0, L0, L11, L2, 0
	TTNOP
	TTNOP
	SFPSETCC	L0, 0, 0
	TTNOP
	li	a4,3
	SFPPUSHC
	SFPENCC	3, 2
	TTNOP
	SFPMOV	L0, L2, 0
	TTNOP
	TTNOP
	SFPPOPC
	TTNOP
	beq	a5,a4,.L45
.L46:
	SFPLOADI	L1, 16256, 0
	SFPMAD	L0, L11, L1, L0, 0
	TTNOP
	TTNOP
	SFPPOPC
	TTNOP
	beq	a5,zero,.L44
	SFPPUSHC
	SFPLOADI	L1, 16448, 0
	SFPMAD	L1, L1, L11, L0, 0
	TTNOP
	TTNOP
	SFPSETCC	L1, 0, 0
	TTNOP
	addi	a5,a5,-1
	bne	a5,a4,.L46
.L45:
	SFPPOPC
	TTNOP
.L44:
	SFPPUSHC
	SFPLOADI	L1, 0, 4
	SFPSETCC	L0, 0, 6
	TTNOP
	SFPPUSHC
	SFPENCC	3, 2
	TTNOP
	SFPMOV	L0, L1, 0
	TTNOP
	TTNOP
	SFPPOPC
	TTNOP
	SFPLOADI	L0, 1, 4
	SFPPOPC
	TTNOP
	SFPSETCC	L0, 0, 2
	TTNOP
	SFPLOADI	L2, 16384, 0
	SFPENCC	3, 10
	TTNOP
	SFPSTORE	0, L2, 0
	ret
.L48:
	SFPPUSHC
	SFPENCC	3, 2
	TTNOP
	SFPMOV	L0, L2, 0
	TTNOP
	TTNOP
	SFPPOPC
	TTNOP
	j	.L44
	.size	_Z23vcond_inside_noninline4i, .-_Z23vcond_inside_noninline4i
	.align	1
	.globl	_Z21vcond_outside_inline4v
	.type	_Z21vcond_outside_inline4v, @function
_Z21vcond_outside_inline4v:
	SFPLOADI	L0, 16256, 0
	SFPLOADI	L1, 16448, 0
	SFPMAD	L1, L1, L11, L0, 0
	TTNOP
	TTNOP
	SFPSETCC	L1, 0, 0
	TTNOP
	SFPLOADI	L1, 16256, 0
	SFPPUSHC
	SFPENCC	3, 2
	TTNOP
	SFPMOV	L2, L0, 0
	TTNOP
	TTNOP
	SFPPOPC
	TTNOP
	SFPMAD	L2, L11, L1, L0, 0
	TTNOP
	TTNOP
	SFPENCC	3, 10
	TTNOP
	SFPLOADI	L1, 16448, 0
	SFPMAD	L1, L1, L11, L2, 0
	TTNOP
	TTNOP
	SFPSETCC	L1, 0, 0
	TTNOP
	SFPENCC	3, 10
	TTNOP
	SFPLOADI	L1, 0, 4
	SFPSETCC	L2, 0, 6
	TTNOP
	SFPLOADI	L1, 1, 4
	SFPENCC	3, 10
	TTNOP
	SFPSETCC	L1, 0, 6
	TTNOP
	SFPLOADI	L0, 16384, 0
	SFPENCC	3, 10
	TTNOP
	SFPSTORE	0, L0, 0
	ret
	.size	_Z21vcond_outside_inline4v, .-_Z21vcond_outside_inline4v
	.align	1
	.globl	_Z24vcond_outside_noninline4i
	.type	_Z24vcond_outside_noninline4i, @function
_Z24vcond_outside_noninline4i:
	SFPLOADI	L2, 16256, 0
	addi	a5,a0,-1
	beq	a0,zero,.L56
	SFPLOADI	L0, 16448, 0
	SFPMAD	L0, L0, L11, L2, 0
	TTNOP
	TTNOP
	SFPSETCC	L0, 0, 0
	TTNOP
	li	a4,3
	SFPPUSHC
	SFPENCC	3, 2
	TTNOP
	SFPMOV	L0, L2, 0
	TTNOP
	TTNOP
	SFPPOPC
	TTNOP
	beq	a5,a4,.L53
.L54:
	SFPLOADI	L1, 16256, 0
	SFPMAD	L0, L11, L1, L0, 0
	TTNOP
	TTNOP
	SFPENCC	3, 10
	TTNOP
	beq	a5,zero,.L52
	SFPLOADI	L1, 16448, 0
	SFPMAD	L1, L1, L11, L0, 0
	TTNOP
	TTNOP
	SFPSETCC	L1, 0, 0
	TTNOP
	addi	a5,a5,-1
	bne	a5,a4,.L54
.L53:
	SFPENCC	3, 10
	TTNOP
.L52:
	SFPLOADI	L1, 0, 4
	SFPSETCC	L0, 0, 6
	TTNOP
	SFPPUSHC
	SFPENCC	3, 2
	TTNOP
	SFPMOV	L0, L1, 0
	TTNOP
	TTNOP
	SFPPOPC
	TTNOP
	SFPLOADI	L0, 1, 4
	SFPENCC	3, 10
	TTNOP
	SFPSETCC	L0, 0, 6
	TTNOP
	SFPLOADI	L2, 16384, 0
	SFPENCC	3, 10
	TTNOP
	SFPSTORE	0, L2, 0
	ret
.L56:
	SFPPUSHC
	SFPENCC	3, 2
	TTNOP
	SFPMOV	L0, L2, 0
	TTNOP
	TTNOP
	SFPPOPC
	TTNOP
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
