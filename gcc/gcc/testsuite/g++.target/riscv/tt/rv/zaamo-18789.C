/* { dg-do compile } */
/* { dg-options "-mcpu=tt-bh -O2" } */
/* { dg-final { check-function-bodies "**" "" } } */
// Taken from gcc.target/riscv/amo/zaamo-rvwmo-amo-add-int.c


/*
** atomic_add_fetch_int_relaxed:
**	amoadd\.w\tzero,a1,0\(a0\)
**	ret
*/
extern "C" void atomic_add_fetch_int_relaxed (int* bar, int baz)
{
  __atomic_add_fetch(bar, baz, __ATOMIC_RELAXED);
}

/*
** atomic_add_fetch_int_acquire:
**	amoadd\.w\.aq\tzero,a1,0\(a0\)
**	ret
*/
extern "C" void atomic_add_fetch_int_acquire (int* bar, int baz)
{
  __atomic_add_fetch(bar, baz, __ATOMIC_ACQUIRE);
}

/*
** atomic_add_fetch_int_release:
**	amoadd\.w\.rl\tzero,a1,0\(a0\)
**	ret
*/
extern "C" void atomic_add_fetch_int_release (int* bar, int baz)
{
  __atomic_add_fetch(bar, baz, __ATOMIC_RELEASE);
}

/*
** atomic_add_fetch_int_acq_rel:
**	amoadd\.w\.aqrl\tzero,a1,0\(a0\)
**	ret
*/
extern "C" void atomic_add_fetch_int_acq_rel (int* bar, int baz)
{
  __atomic_add_fetch(bar, baz, __ATOMIC_ACQ_REL);
}

/*
** atomic_add_fetch_int_seq_cst:
**	amoadd\.w\.aqrl\tzero,a1,0\(a0\)
**	ret
*/
extern "C" void atomic_add_fetch_int_seq_cst (int* bar, int baz)
{
  __atomic_add_fetch(bar, baz, __ATOMIC_SEQ_CST);
}
