// { dg-options "-mcpu=tt-wh -fno-exceptions -fno-rtti -O2" }
// { dg-final { check-function-bodies "**" "" } }

extern "C" {
int load_relaxed (int *a) { return __atomic_load_n (a, __ATOMIC_RELAXED); }
/*
**load_relaxed:
**	lw	a0,0\(a0\)
**	ret
*/
int load_seq_cst (int *a) { return __atomic_load_n (a, __ATOMIC_SEQ_CST); }
/*
**load_seq_cst:
**	lui a5,%hi\(0xFFB11014\); addi a5,a5,%lo\(0xFFB11014\)  # RISCV_TDMA_REG_STATUS
**	sw zero,0\(a5\); sw zero,0\(a5\); sw zero,0\(a5\)
**	nop; nop
**	lw	a0,0\(a0\)
**	lui a5,%hi\(0xFFB11014\); addi a5,a5,%lo\(0xFFB11014\)  # RISCV_TDMA_REG_STATUS
**	sw zero,0\(a5\); sw zero,0\(a5\); sw zero,0\(a5\)
**	nop; nop
**	ret
*/
int load_acquire (int *a) { return __atomic_load_n (a, __ATOMIC_ACQUIRE); }
/*
**load_acquire:
**	lw	a0,0\(a0\)
**	lui a5,%hi\(0xFFB11014\); addi a5,a5,%lo\(0xFFB11014\)  # RISCV_TDMA_REG_STATUS
**	sw zero,0\(a5\); sw zero,0\(a5\); sw zero,0\(a5\)
**	nop; nop
**	ret
*/
int load_consume (int *a) { return __atomic_load_n (a, __ATOMIC_CONSUME); }
/*
**load_consume:
**	lw	a0,0\(a0\)
**	lui a5,%hi\(0xFFB11014\); addi a5,a5,%lo\(0xFFB11014\)  # RISCV_TDMA_REG_STATUS
**	sw zero,0\(a5\); sw zero,0\(a5\); sw zero,0\(a5\)
**	nop; nop
**	ret
*/

void store_relaxed (int *a) { __atomic_store_n (a, 1, __ATOMIC_RELAXED); }
/*
**store_relaxed:
**	li	a5,1
**	sw	a5,0\(a0\)
**	ret
*/
void store_seq_cst (int *a) { __atomic_store_n (a, 2, __ATOMIC_SEQ_CST); }
/*
**store_seq_cst:
**	lui a5,%hi\(0xFFB11014\); addi a5,a5,%lo\(0xFFB11014\)  # RISCV_TDMA_REG_STATUS
**	sw zero,0\(a5\); sw zero,0\(a5\); sw zero,0\(a5\)
**	nop; nop
**	li	a5,2
**	sw	a5,0\(a0\)
**	lui a5,%hi\(0xFFB11014\); addi a5,a5,%lo\(0xFFB11014\)  # RISCV_TDMA_REG_STATUS
**	sw zero,0\(a5\); sw zero,0\(a5\); sw zero,0\(a5\)
**	nop; nop
**	ret
*/
void store_release (int *a) { __atomic_store_n (a, 3, __ATOMIC_RELEASE); }
/*
**store_release:
**	lui a5,%hi\(0xFFB11014\); addi a5,a5,%lo\(0xFFB11014\)  # RISCV_TDMA_REG_STATUS
**	sw zero,0\(a5\); sw zero,0\(a5\); sw zero,0\(a5\)
**	nop; nop
**	li	a5,3
**	sw	a5,0\(a0\)
**	ret
*/
}
