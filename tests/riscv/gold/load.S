	.file	"load.cc"
	.option nopic
	.attribute arch, "rv32i2p0"
	.attribute unaligned_access, 0
	.attribute stack_align, 16
	.text
	.align	2
	.globl	_Z23latency_chain_dependentPiiiiii
	.type	_Z23latency_chain_dependentPiiiiii, @function
_Z23latency_chain_dependentPiiiiii:
	lw	a0,0(a0)
	add	a1,a1,a0
	add	a1,a1,a2
	add	a1,a1,a3
	add	a1,a1,a4
	add	a0,a1,a5
	ret
	.size	_Z23latency_chain_dependentPiiiiii, .-_Z23latency_chain_dependentPiiiiii
	.align	2
	.globl	_Z19latency_chain_splitPiiiiii
	.type	_Z19latency_chain_splitPiiiiii, @function
_Z19latency_chain_splitPiiiiii:
	lw	a0,0(a0)
	add	a2,a2,a3
	add	a1,a1,a0
	add	a1,a1,a2
	add	a0,a4,a5
	add	a0,a1,a0
	ret
	.size	_Z19latency_chain_splitPiiiiii, .-_Z19latency_chain_splitPiiiiii
	.align	2
	.globl	_Z33latency_chain_dependent_decoratedU11rvtt_l1_ptrPiiiiii
	.type	_Z33latency_chain_dependent_decoratedU11rvtt_l1_ptrPiiiiii, @function
_Z33latency_chain_dependent_decoratedU11rvtt_l1_ptrPiiiiii:
	lw	a0,0(a0)
	add	a1,a1,a0
	add	a1,a1,a2
	add	a1,a1,a3
	add	a1,a1,a4
	add	a0,a1,a5
	ret
	.size	_Z33latency_chain_dependent_decoratedU11rvtt_l1_ptrPiiiiii, .-_Z33latency_chain_dependent_decoratedU11rvtt_l1_ptrPiiiiii
	.align	2
	.globl	_Z29latency_chain_split_decoratedU11rvtt_l1_ptrPiiiiii
	.type	_Z29latency_chain_split_decoratedU11rvtt_l1_ptrPiiiiii, @function
_Z29latency_chain_split_decoratedU11rvtt_l1_ptrPiiiiii:
	lw	a0,0(a0)
	add	a2,a2,a3
	add	a4,a4,a5
	add	a1,a1,a0
	add	a1,a1,a2
	add	a0,a1,a4
	ret
	.size	_Z29latency_chain_split_decoratedU11rvtt_l1_ptrPiiiiii, .-_Z29latency_chain_split_decoratedU11rvtt_l1_ptrPiiiiii
	.align	2
	.globl	_Z11load_globalv
	.type	_Z11load_globalv, @function
_Z11load_globalv:
	lui	a5,%hi(.LANCHOR0)
	lw	a0,%lo(.LANCHOR0)(a5)
	ret
	.size	_Z11load_globalv, .-_Z11load_globalv
	.align	2
	.globl	_Z14load_global_l1v
	.type	_Z14load_global_l1v, @function
_Z14load_global_l1v:
	lui	a5,%hi(.LANCHOR0+16)
	lw	a0,%lo(.LANCHOR0+16)(a5)
	mv	a0,a0	# GS L1 war
	ret
	.size	_Z14load_global_l1v, .-_Z14load_global_l1v
	.align	2
	.globl	_Z21load_global_l1_xo_ptrv
	.type	_Z21load_global_l1_xo_ptrv, @function
_Z21load_global_l1_xo_ptrv:
	lui	a5,%hi(global_l1_xo_ptr)
	lw	a5,%lo(global_l1_xo_ptr)(a5)
	lw	a0,0(a5)
	mv	a0,a0	# GS L1 war
	ret
	.size	_Z21load_global_l1_xo_ptrv, .-_Z21load_global_l1_xo_ptrv
	.align	2
	.globl	_Z18load_global_l1_ptrv
	.type	_Z18load_global_l1_ptrv, @function
_Z18load_global_l1_ptrv:
	lui	a5,%hi(global_l1_ptr)
	lw	a5,%lo(global_l1_ptr)(a5)
	lw	a0,0(a5)
	mv	a0,a0	# GS L1 war
	ret
	.size	_Z18load_global_l1_ptrv, .-_Z18load_global_l1_ptrv
	.align	2
	.globl	_Z21load_global_l1_ts_ptrv
	.type	_Z21load_global_l1_ts_ptrv, @function
_Z21load_global_l1_ts_ptrv:
	lui	a5,%hi(.LANCHOR0+44)
	lw	a0,%lo(.LANCHOR0+44)(a5)
	mv	a0,a0	# GS L1 war
	ret
	.size	_Z21load_global_l1_ts_ptrv, .-_Z21load_global_l1_ts_ptrv
	.align	2
	.globl	_Z13load_union_l14__tu
	.type	_Z13load_union_l14__tu, @function
_Z13load_union_l14__tu:
	lw	a0,0(a0)
	mv	a0,a0	# GS L1 war
	ret
	.size	_Z13load_union_l14__tu, .-_Z13load_union_l14__tu
	.align	2
	.globl	_Z13load_union_ll4__tu
	.type	_Z13load_union_ll4__tu, @function
_Z13load_union_ll4__tu:
	lw	a0,0(a0)
	ret
	.size	_Z13load_union_ll4__tu, .-_Z13load_union_ll4__tu
	.align	2
	.globl	_Z10load_arrayU11rvtt_l1_ptrP4__ar
	.type	_Z10load_arrayU11rvtt_l1_ptrP4__ar, @function
_Z10load_arrayU11rvtt_l1_ptrP4__ar:
	lw	a0,4(a0)
	mv	a0,a0	# GS L1 war
	ret
	.size	_Z10load_arrayU11rvtt_l1_ptrP4__ar, .-_Z10load_arrayU11rvtt_l1_ptrP4__ar
	.align	2
	.globl	_Z11load_array2U11rvtt_l1_ptrP4__ari
	.type	_Z11load_array2U11rvtt_l1_ptrP4__ari, @function
_Z11load_array2U11rvtt_l1_ptrP4__ari:
	slli	a1,a1,2
	add	a0,a0,a1
	lw	a0,0(a0)
	mv	a0,a0	# GS L1 war
	ret
	.size	_Z11load_array2U11rvtt_l1_ptrP4__ari, .-_Z11load_array2U11rvtt_l1_ptrP4__ari
	.align	2
	.globl	_Z11load_arraypU11rvtt_l1_ptrP5__arp
	.type	_Z11load_arraypU11rvtt_l1_ptrP5__arp, @function
_Z11load_arraypU11rvtt_l1_ptrP5__arp:
	lw	a5,0(a0)
	lw	a0,0(a5)
	mv	a0,a0	# GS L1 war
	ret
	.size	_Z11load_arraypU11rvtt_l1_ptrP5__arp, .-_Z11load_arraypU11rvtt_l1_ptrP5__arp
	.align	2
	.globl	_Z12load_arrayp2U11rvtt_l1_ptrP5__arp
	.type	_Z12load_arrayp2U11rvtt_l1_ptrP5__arp, @function
_Z12load_arrayp2U11rvtt_l1_ptrP5__arp:
	lw	a5,0(a0)
	lw	a2,4(a0)
	lw	a3,8(a0)
	lw	a4,12(a0)
	lw	a1,16(a0)
	lw	a5,0(a5)
	lw	a2,0(a2)
	lw	a3,0(a3)
	lw	a4,0(a4)
	lw	a0,0(a1)
	add	a5,a5,a2
	add	a5,a5,a3
	add	a5,a5,a4
	add	a0,a5,a0
	ret
	.size	_Z12load_arrayp2U11rvtt_l1_ptrP5__arp, .-_Z12load_arrayp2U11rvtt_l1_ptrP5__arp
	.align	2
	.globl	_Z9differentU11rvtt_l1_ptri
	.type	_Z9differentU11rvtt_l1_ptri, @function
_Z9differentU11rvtt_l1_ptri:
	ret
	.size	_Z9differentU11rvtt_l1_ptri, .-_Z9differentU11rvtt_l1_ptri
	.align	2
	.globl	_Z20load_global_l1_constv
	.type	_Z20load_global_l1_constv, @function
_Z20load_global_l1_constv:
	lui	a5,%hi(global_l1_const_ptr)
	lw	a5,%lo(global_l1_const_ptr)(a5)
	lw	a0,0(a5)
	mv	a0,a0	# GS L1 war
	ret
	.size	_Z20load_global_l1_constv, .-_Z20load_global_l1_constv
	.align	2
	.globl	_Z5cast1v
	.type	_Z5cast1v, @function
_Z5cast1v:
	lw	a0,800(zero)
	mv	a0,a0	# GS L1 war
	ret
	.size	_Z5cast1v, .-_Z5cast1v
	.align	2
	.globl	_Z5cast2ii
	.type	_Z5cast2ii, @function
_Z5cast2ii:
	addi	a0,a0,200
	slli	a0,a0,2
	add	a0,a0,a1
	lw	a0,0(a0)
	mv	a0,a0	# GS L1 war
	ret
	.size	_Z5cast2ii, .-_Z5cast2ii
	.align	2
	.globl	_Z5cast3i
	.type	_Z5cast3i, @function
_Z5cast3i:
	lw	a0,0(a0)
	mv	a0,a0	# GS L1 war
	ret
	.size	_Z5cast3i, .-_Z5cast3i
	.align	2
	.globl	_Z11lots_o_argsiiiiiiiiiiiiiiiiii
	.type	_Z11lots_o_argsiiiiiiiiiiiiiiiiii, @function
_Z11lots_o_argsiiiiiiiiiiiiiiiiii:
	add	a0,a0,a1
	add	a0,a0,a2
	add	a0,a0,a3
	add	a0,a0,a4
	add	a0,a0,a5
	lw	a5,0(sp)
	add	a0,a0,a6
	add	a0,a0,a7
	add	a5,a0,a5
	lw	a0,4(sp)
	lw	a4,12(sp)
	add	a5,a5,a0
	lw	a0,8(sp)
	add	a5,a5,a0
	add	a5,a5,a4
	lw	a4,16(sp)
	lw	a0,20(sp)
	add	a5,a5,a4
	add	a5,a5,a0
	lw	a0,24(sp)
	add	a5,a5,a0
	lw	a0,28(sp)
	add	a5,a5,a0
	lw	a0,32(sp)
	add	a5,a5,a0
	lw	a0,36(sp)
	add	a0,a5,a0
	ret
	.size	_Z11lots_o_argsiiiiiiiiiiiiiiiiii, .-_Z11lots_o_argsiiiiiiiiiiiiiiiiii
	.align	2
	.globl	_Z11lots_o_argsiiiiiiiiiiiiiiiiiU11rvtt_l1_ptrPi
	.type	_Z11lots_o_argsiiiiiiiiiiiiiiiiiU11rvtt_l1_ptrPi, @function
_Z11lots_o_argsiiiiiiiiiiiiiiiiiU11rvtt_l1_ptrPi:
	add	a0,a0,a1
	add	a0,a0,a2
	add	a0,a0,a3
	add	a0,a0,a4
	add	a0,a0,a5
	add	a0,a0,a6
	add	a5,a0,a7
	lw	a0,0(sp)
	lw	a4,36(sp)
	add	a5,a5,a0
	lw	a0,4(sp)
	lw	a3,0(a4)
	lw	a4,12(sp)
	add	a5,a5,a0
	lw	a0,8(sp)
	add	a5,a5,a0
	add	a5,a5,a4
	lw	a4,16(sp)
	lw	a0,20(sp)
	add	a5,a5,a4
	add	a5,a5,a0
	lw	a0,24(sp)
	add	a5,a5,a0
	lw	a0,28(sp)
	add	a5,a5,a0
	lw	a0,32(sp)
	add	a0,a5,a0
	add	a0,a0,a3
	ret
	.size	_Z11lots_o_argsiiiiiiiiiiiiiiiiiU11rvtt_l1_ptrPi, .-_Z11lots_o_argsiiiiiiiiiiiiiiiiiU11rvtt_l1_ptrPi
	.align	2
	.globl	_Z20addr_calc_decorated1U11rvtt_l1_ptrPi
	.type	_Z20addr_calc_decorated1U11rvtt_l1_ptrPi, @function
_Z20addr_calc_decorated1U11rvtt_l1_ptrPi:
	lw	a0,0(a0)
	mv	a0,a0	# GS L1 war
	ret
	.size	_Z20addr_calc_decorated1U11rvtt_l1_ptrPi, .-_Z20addr_calc_decorated1U11rvtt_l1_ptrPi
	.align	2
	.globl	_Z20addr_calc_decorated2U11rvtt_l1_ptrPi
	.type	_Z20addr_calc_decorated2U11rvtt_l1_ptrPi, @function
_Z20addr_calc_decorated2U11rvtt_l1_ptrPi:
	lw	a0,20(a0)
	mv	a0,a0	# GS L1 war
	ret
	.size	_Z20addr_calc_decorated2U11rvtt_l1_ptrPi, .-_Z20addr_calc_decorated2U11rvtt_l1_ptrPi
	.align	2
	.globl	_Z20addr_calc_decorated3U11rvtt_l1_ptrP4__ts
	.type	_Z20addr_calc_decorated3U11rvtt_l1_ptrP4__ts, @function
_Z20addr_calc_decorated3U11rvtt_l1_ptrP4__ts:
	lw	a0,0(a0)
	mv	a0,a0	# GS L1 war
	ret
	.size	_Z20addr_calc_decorated3U11rvtt_l1_ptrP4__ts, .-_Z20addr_calc_decorated3U11rvtt_l1_ptrP4__ts
	.align	2
	.globl	_Z21addr_calc_decorated4aU11rvtt_l1_ptrP4__ts
	.type	_Z21addr_calc_decorated4aU11rvtt_l1_ptrP4__ts, @function
_Z21addr_calc_decorated4aU11rvtt_l1_ptrP4__ts:
	lw	a0,4(a0)
	mv	a0,a0	# GS L1 war
	ret
	.size	_Z21addr_calc_decorated4aU11rvtt_l1_ptrP4__ts, .-_Z21addr_calc_decorated4aU11rvtt_l1_ptrP4__ts
	.align	2
	.globl	_Z21addr_calc_decorated4bU11rvtt_l1_ptrP4__ts
	.type	_Z21addr_calc_decorated4bU11rvtt_l1_ptrP4__ts, @function
_Z21addr_calc_decorated4bU11rvtt_l1_ptrP4__ts:
	lw	a0,44(a0)
	mv	a0,a0	# GS L1 war
	ret
	.size	_Z21addr_calc_decorated4bU11rvtt_l1_ptrP4__ts, .-_Z21addr_calc_decorated4bU11rvtt_l1_ptrP4__ts
	.align	2
	.globl	_Z20addr_calc_decorated5U11rvtt_l1_ptrPVi
	.type	_Z20addr_calc_decorated5U11rvtt_l1_ptrPVi, @function
_Z20addr_calc_decorated5U11rvtt_l1_ptrPVi:
	lw	a0,0(a0)
	mv	a0,a0	# GS L1 war
	ret
	.size	_Z20addr_calc_decorated5U11rvtt_l1_ptrPVi, .-_Z20addr_calc_decorated5U11rvtt_l1_ptrPVi
	.align	2
	.globl	_Z4plusU11rvtt_l1_ptrP4__ts
	.type	_Z4plusU11rvtt_l1_ptrP4__ts, @function
_Z4plusU11rvtt_l1_ptrP4__ts:
	lw	a0,4(a0)
	addi	a0,a0,5
	ret
	.size	_Z4plusU11rvtt_l1_ptrP4__ts, .-_Z4plusU11rvtt_l1_ptrP4__ts
	.align	2
	.globl	_Z3uqiU11rvtt_l1_ptrPh
	.type	_Z3uqiU11rvtt_l1_ptrPh, @function
_Z3uqiU11rvtt_l1_ptrPh:
	lbu	a0,0(a0)
	mv	a0,a0	# GS L1 war
	ret
	.size	_Z3uqiU11rvtt_l1_ptrPh, .-_Z3uqiU11rvtt_l1_ptrPh
	.align	2
	.globl	_Z2qiU11rvtt_l1_ptrPc
	.type	_Z2qiU11rvtt_l1_ptrPc, @function
_Z2qiU11rvtt_l1_ptrPc:
	lbu	a0,0(a0)
	mv	a0,a0	# GS L1 war
	ret
	.size	_Z2qiU11rvtt_l1_ptrPc, .-_Z2qiU11rvtt_l1_ptrPc
	.align	2
	.globl	_Z3uhiU11rvtt_l1_ptrPt
	.type	_Z3uhiU11rvtt_l1_ptrPt, @function
_Z3uhiU11rvtt_l1_ptrPt:
	lhu	a0,0(a0)
	mv	a0,a0	# GS L1 war
	ret
	.size	_Z3uhiU11rvtt_l1_ptrPt, .-_Z3uhiU11rvtt_l1_ptrPt
	.align	2
	.globl	_Z2hiU11rvtt_l1_ptrPs
	.type	_Z2hiU11rvtt_l1_ptrPs, @function
_Z2hiU11rvtt_l1_ptrPs:
	lh	a0,0(a0)
	mv	a0,a0	# GS L1 war
	ret
	.size	_Z2hiU11rvtt_l1_ptrPs, .-_Z2hiU11rvtt_l1_ptrPs
	.align	2
	.globl	_Z5up_qiU11rvtt_l1_ptrPc
	.type	_Z5up_qiU11rvtt_l1_ptrPc, @function
_Z5up_qiU11rvtt_l1_ptrPc:
	lbu	a0,0(a0)
	mv	a0,a0	# GS L1 war
	ret
	.size	_Z5up_qiU11rvtt_l1_ptrPc, .-_Z5up_qiU11rvtt_l1_ptrPc
	.align	2
	.globl	_Z6up_uqiU11rvtt_l1_ptrPh
	.type	_Z6up_uqiU11rvtt_l1_ptrPh, @function
_Z6up_uqiU11rvtt_l1_ptrPh:
	lbu	a0,0(a0)
	mv	a0,a0	# GS L1 war
	ret
	.size	_Z6up_uqiU11rvtt_l1_ptrPh, .-_Z6up_uqiU11rvtt_l1_ptrPh
	.align	2
	.globl	_Z5up_hiU11rvtt_l1_ptrPs
	.type	_Z5up_hiU11rvtt_l1_ptrPs, @function
_Z5up_hiU11rvtt_l1_ptrPs:
	lh	a0,0(a0)
	mv	a0,a0	# GS L1 war
	ret
	.size	_Z5up_hiU11rvtt_l1_ptrPs, .-_Z5up_hiU11rvtt_l1_ptrPs
	.align	2
	.globl	_Z6up_uhiU11rvtt_l1_ptrPt
	.type	_Z6up_uhiU11rvtt_l1_ptrPt, @function
_Z6up_uhiU11rvtt_l1_ptrPt:
	lhu	a0,0(a0)
	mv	a0,a0	# GS L1 war
	ret
	.size	_Z6up_uhiU11rvtt_l1_ptrPt, .-_Z6up_uhiU11rvtt_l1_ptrPt
	.align	2
	.globl	_Z7down_qiU11rvtt_l1_ptrPi
	.type	_Z7down_qiU11rvtt_l1_ptrPi, @function
_Z7down_qiU11rvtt_l1_ptrPi:
	lbu	a0,0(a0)
	mv	a0,a0	# GS L1 war
	ret
	.size	_Z7down_qiU11rvtt_l1_ptrPi, .-_Z7down_qiU11rvtt_l1_ptrPi
	.align	2
	.globl	_Z8down_uqiU11rvtt_l1_ptrPj
	.type	_Z8down_uqiU11rvtt_l1_ptrPj, @function
_Z8down_uqiU11rvtt_l1_ptrPj:
	lbu	a0,0(a0)
	mv	a0,a0	# GS L1 war
	ret
	.size	_Z8down_uqiU11rvtt_l1_ptrPj, .-_Z8down_uqiU11rvtt_l1_ptrPj
	.align	2
	.globl	_Z7down_hiU11rvtt_l1_ptrPi
	.type	_Z7down_hiU11rvtt_l1_ptrPi, @function
_Z7down_hiU11rvtt_l1_ptrPi:
	lh	a0,0(a0)
	mv	a0,a0	# GS L1 war
	ret
	.size	_Z7down_hiU11rvtt_l1_ptrPi, .-_Z7down_hiU11rvtt_l1_ptrPi
	.align	2
	.globl	_Z8down_uhiU11rvtt_l1_ptrPj
	.type	_Z8down_uhiU11rvtt_l1_ptrPj, @function
_Z8down_uhiU11rvtt_l1_ptrPj:
	lhu	a0,0(a0)
	mv	a0,a0	# GS L1 war
	ret
	.size	_Z8down_uhiU11rvtt_l1_ptrPj, .-_Z8down_uhiU11rvtt_l1_ptrPj
	.align	2
	.globl	_Z15l1ptrtestreturnU11rvtt_l1_ptrPVi
	.type	_Z15l1ptrtestreturnU11rvtt_l1_ptrPVi, @function
_Z15l1ptrtestreturnU11rvtt_l1_ptrPVi:
	ret
	.size	_Z15l1ptrtestreturnU11rvtt_l1_ptrPVi, .-_Z15l1ptrtestreturnU11rvtt_l1_ptrPVi
	.align	2
	.globl	_Z30error_drop_attribute_on_returnU11rvtt_l1_ptrPi
	.type	_Z30error_drop_attribute_on_returnU11rvtt_l1_ptrPi, @function
_Z30error_drop_attribute_on_returnU11rvtt_l1_ptrPi:
	ret
	.size	_Z30error_drop_attribute_on_returnU11rvtt_l1_ptrPi, .-_Z30error_drop_attribute_on_returnU11rvtt_l1_ptrPi
	.align	2
	.globl	_Z30error_drop_attribute_on_assignU11rvtt_l1_ptrPi
	.type	_Z30error_drop_attribute_on_assignU11rvtt_l1_ptrPi, @function
_Z30error_drop_attribute_on_assignU11rvtt_l1_ptrPi:
	lw	a0,0(a0)
	mv	a0,a0	# GS L1 war
	ret
	.size	_Z30error_drop_attribute_on_assignU11rvtt_l1_ptrPi, .-_Z30error_drop_attribute_on_assignU11rvtt_l1_ptrPi
	.align	2
	.globl	_Z4castPi
	.type	_Z4castPi, @function
_Z4castPi:
	ret
	.size	_Z4castPi, .-_Z4castPi
	.align	2
	.globl	_Z5storeU11rvtt_l1_ptrPi
	.type	_Z5storeU11rvtt_l1_ptrPi, @function
_Z5storeU11rvtt_l1_ptrPi:
	sw	zero,0(a0)
	ret
	.size	_Z5storeU11rvtt_l1_ptrPi, .-_Z5storeU11rvtt_l1_ptrPi
	.align	2
	.globl	_Z4copyU11rvtt_l1_ptrP4__ts
	.type	_Z4copyU11rvtt_l1_ptrP4__ts, @function
_Z4copyU11rvtt_l1_ptrP4__ts:
	lw	a5,0(a0)
	sw	a5,4(a0)
	ret
	.size	_Z4copyU11rvtt_l1_ptrP4__ts, .-_Z4copyU11rvtt_l1_ptrP4__ts
	.align	2
	.globl	_Z29error_add_attribute_on_returnPi
	.type	_Z29error_add_attribute_on_returnPi, @function
_Z29error_add_attribute_on_returnPi:
	ret
	.size	_Z29error_add_attribute_on_returnPi, .-_Z29error_add_attribute_on_returnPi
	.align	2
	.globl	_Z29error_add_attribute_on_assignPi
	.type	_Z29error_add_attribute_on_assignPi, @function
_Z29error_add_attribute_on_assignPi:
	ret
	.size	_Z29error_add_attribute_on_assignPi, .-_Z29error_add_attribute_on_assignPi
	.align	2
	.globl	_Z24error_l1_ptr_type_param1U11rvtt_l1_ptri
	.type	_Z24error_l1_ptr_type_param1U11rvtt_l1_ptri, @function
_Z24error_l1_ptr_type_param1U11rvtt_l1_ptri:
	ret
	.size	_Z24error_l1_ptr_type_param1U11rvtt_l1_ptri, .-_Z24error_l1_ptr_type_param1U11rvtt_l1_ptri
	.align	2
	.globl	_Z24error_l1_ptr_type_param2U11rvtt_l1_ptri
	.type	_Z24error_l1_ptr_type_param2U11rvtt_l1_ptri, @function
_Z24error_l1_ptr_type_param2U11rvtt_l1_ptri:
	addi	sp,sp,-16
	sw	a0,12(sp)
	lw	a0,12(sp)
	addi	sp,sp,16
	jr	ra
	.size	_Z24error_l1_ptr_type_param2U11rvtt_l1_ptri, .-_Z24error_l1_ptr_type_param2U11rvtt_l1_ptri
	.align	2
	.globl	_Z21error_l1_ptr_type_varv
	.type	_Z21error_l1_ptr_type_varv, @function
_Z21error_l1_ptr_type_varv:
	li	a0,1
	ret
	.size	_Z21error_l1_ptr_type_varv, .-_Z21error_l1_ptr_type_varv
	.align	2
	.globl	_Z19error_l1_ptr_returnU11rvtt_l1_ptrPVi
	.type	_Z19error_l1_ptr_returnU11rvtt_l1_ptrPVi, @function
_Z19error_l1_ptr_returnU11rvtt_l1_ptrPVi:
	lw	a0,0(a0)
	mv	a0,a0	# GS L1 war
	ret
	.size	_Z19error_l1_ptr_returnU11rvtt_l1_ptrPVi, .-_Z19error_l1_ptr_returnU11rvtt_l1_ptrPVi
	.align	2
	.globl	_Z16reg_read_latencyU12rvtt_reg_ptrPiiiiii
	.type	_Z16reg_read_latencyU12rvtt_reg_ptrPiiiiii, @function
_Z16reg_read_latencyU12rvtt_reg_ptrPiiiiii:
	lw	a0,0(a0)
	add	a2,a2,a3
	add	a1,a1,a0
	add	a1,a1,a2
	add	a0,a4,a5
	add	a0,a1,a0
	ret
	.size	_Z16reg_read_latencyU12rvtt_reg_ptrPiiiiii, .-_Z16reg_read_latencyU12rvtt_reg_ptrPiiiiii
	.align	2
	.globl	_Z14real_test_casejjiiiii
	.type	_Z14real_test_casejjiiiii, @function
_Z14real_test_casejjiiiii:
	slli	a0,a0,16
	add	a0,a0,a1
	lw	a0,0(a0)
	add	a5,a5,a6
	add	a5,a5,a4
	add	a5,a5,a3
	add	a5,a5,a2
	add	a0,a5,a0
	ret
	.size	_Z14real_test_casejjiiiii, .-_Z14real_test_casejjiiiii
	.section	.text.startup,"ax",@progbits
	.align	2
	.globl	main
	.type	main, @function
main:
	li	a0,0
	ret
	.size	main, .-main
	.globl	global_l1_const_ptr
	.globl	global_l1_xo_ptr
	.globl	global_l1_ptr
	.globl	global_ts
	.globl	global_l1
	.globl	global
	.bss
	.align	2
	.set	.LANCHOR0,. + 0
	.type	global, @object
	.size	global, 16
global:
	.zero	16
	.type	global_l1, @object
	.size	global_l1, 16
global_l1:
	.zero	16
	.type	global_ts, @object
	.size	global_ts, 32
global_ts:
	.zero	32
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
