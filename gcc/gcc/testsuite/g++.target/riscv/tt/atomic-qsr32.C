// { dg-options "-mcpu=tt-qsr32 -fno-exceptions -fno-rtti -O2" }
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
**	fence	rw,rw
**	lw	a0,0\(a0\)
**	fence	r,rw
**	ret
*/
int load_acquire (int *a) { return __atomic_load_n (a, __ATOMIC_ACQUIRE); }
/*
**load_acquire:
**	lw	a0,0\(a0\)
**	fence	r,rw
**	ret
*/
int load_consume (int *a) { return __atomic_load_n (a, __ATOMIC_CONSUME); }
/*
**load_consume:
**	lw	a0,0\(a0\)
**	fence	r,rw
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
**	li	a5,2
**	fence	rw,w
**	sw	a5,0\(a0\)
**	fence	rw,rw
**	ret
*/
void store_release (int *a) { __atomic_store_n (a, 3, __ATOMIC_RELEASE); }
/*
**store_release:
**	li	a5,3
**	fence	rw,w
**	sw	a5,0\(a0\)
**	ret
*/
}
