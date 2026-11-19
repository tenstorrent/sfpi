// { dg-options "-mcpu=tt-bh-tensix -fno-exceptions -fno-rtti -O2 -mno-tt-tensix-optimize-dce" }
// { dg-final { check-function-bodies "**" "" } }

void copy ()
{
  auto v1 = __builtin_rvtt_sfpload (nullptr, 0, 0, 0, 0, 0);
  auto v2 = __builtin_rvtt_sfpload (nullptr, 0, 0, 0, 0, 0);
  auto v3 = __builtin_rvtt_sfpload (nullptr, 0, 0, 0, 0, 0);

  auto r = __builtin_rvtt_sfpshft2_copy4 (v1, v2, v3, 0);
  auto v0 = __builtin_rvtt_sfpselect4 (r, 0);
  v1 = __builtin_rvtt_sfpselect4 (r, 1);
  v2 = __builtin_rvtt_sfpselect4 (r, 2);
  v3 = __builtin_rvtt_sfpselect4 (r, 3);

  __builtin_rvtt_sfpstore (nullptr, v0, 0, 0, 0, 0, 0);
  __builtin_rvtt_sfpstore (nullptr, v1, 0, 0, 0, 0, 0);
  __builtin_rvtt_sfpstore (nullptr, v2, 0, 0, 0, 0, 0);
  __builtin_rvtt_sfpstore (nullptr, v3, 0, 0, 0, 0, 0);
}
/*
**_Z4copyv:
**	SFPLOAD	L1, 0, 0, 0
**	SFPLOAD	L2, 0, 0, 0
**	SFPLOAD	L3, 0, 0, 0
**	SFPSHFT2	L0, L0, 0, 0
**	SFPSTORE	L0, 0, 0, 0
**	SFPSTORE	L1, 0, 0, 0
**	SFPSTORE	L2, 0, 0, 0
**	SFPSTORE	L3, 0, 0, 0
**	ret
*/

void subvec_copy ()
{
  auto v0 = __builtin_rvtt_sfpload (nullptr, 0, 0, 0, 0, 0);
  auto v1 = __builtin_rvtt_sfpload (nullptr, 0, 0, 0, 0, 0);
  auto v2 = __builtin_rvtt_sfpload (nullptr, 0, 0, 0, 0, 0);
  auto v3 = __builtin_rvtt_sfpload (nullptr, 0, 0, 0, 0, 0);

  auto r = __builtin_rvtt_sfpshft2_subvec_copy4 (v0, v1, v2, v3, 1);
  v0 = __builtin_rvtt_sfpselect4 (r, 0);
  v1 = __builtin_rvtt_sfpselect4 (r, 1);
  v2 = __builtin_rvtt_sfpselect4 (r, 2);
  v3 = __builtin_rvtt_sfpselect4 (r, 3);

  __builtin_rvtt_sfpstore (nullptr, v0, 0, 0, 0, 0, 0);
  __builtin_rvtt_sfpstore (nullptr, v1, 0, 0, 0, 0, 0);
  __builtin_rvtt_sfpstore (nullptr, v2, 0, 0, 0, 0, 0);
  __builtin_rvtt_sfpstore (nullptr, v3, 0, 0, 0, 0, 0);
}
/*
**_Z11subvec_copyv:
**	SFPLOAD	L1, 0, 0, 0
**	SFPLOAD	L2, 0, 0, 0
**	SFPLOAD	L3, 0, 0, 0
**	SFPLOAD	L0, 0, 0, 0
**	SFPSHFT2	L0, L0, 0, 1
**	SFPSTORE	L0, 0, 0, 0
**	SFPSTORE	L1, 0, 0, 0
**	SFPSTORE	L2, 0, 0, 0
**	SFPSTORE	L3, 0, 0, 0
**	ret
*/

void subvec_shfl_copy ()
{
  auto v1 = __builtin_rvtt_sfpload (nullptr, 0, 0, 0, 0, 0);
  auto v2 = __builtin_rvtt_sfpload (nullptr, 0, 0, 0, 0, 0);
  auto v3 = __builtin_rvtt_sfpload (nullptr, 0, 0, 0, 0, 0);
  auto src = __builtin_rvtt_sfpload (nullptr, 0, 0, 0, 0, 0);

  auto r = __builtin_rvtt_sfpshft2_subvec_shfl1_copy4 (v1, v2, v3, src, 2);
  auto v0 = __builtin_rvtt_sfpselect4 (r, 0);
  v1 = __builtin_rvtt_sfpselect4 (r, 1);
  v2 = __builtin_rvtt_sfpselect4 (r, 2);
  v3 = __builtin_rvtt_sfpselect4 (r, 3);

  __builtin_rvtt_sfpstore (nullptr, v0, 0, 0, 0, 0, 0);
  __builtin_rvtt_sfpstore (nullptr, v1, 0, 0, 0, 0, 0);
  __builtin_rvtt_sfpstore (nullptr, v2, 0, 0, 0, 0, 0);
  __builtin_rvtt_sfpstore (nullptr, v3, 0, 0, 0, 0, 0);
}
/*
**_Z16subvec_shfl_copyv:
**	SFPLOAD	L1, 0, 0, 0
**	SFPLOAD	L2, 0, 0, 0
**	SFPLOAD	L3, 0, 0, 0
**	SFPLOAD	L0, 0, 0, 0
**	SFPSHFT2	L0, L0, 0, 2
**	SFPNOP
**	SFPSTORE	L0, 0, 0, 0
**	SFPSTORE	L1, 0, 0, 0
**	SFPSTORE	L2, 0, 0, 0
**	SFPSTORE	L3, 0, 0, 0
**	ret
*/

void subvec_shfl_rot ()
{
  auto src = __builtin_rvtt_sfpload (nullptr, 0, 0, 0, 0, 0);

  auto dst = __builtin_rvtt_sfpshft2_subvec_shfl1 (src, 3);

  __builtin_rvtt_sfpstore (nullptr, dst, 0, 0, 0, 0, 0);
}
/*
**_Z15subvec_shfl_rotv:
**	SFPLOAD	L0, 0, 0, 0
**	SFPSHFT2	L0, L0, 0, 3
**	SFPNOP
**	SFPSTORE	L0, 0, 0, 0
**	ret
*/

void subvec_shfl_shift ()
{
  auto src = __builtin_rvtt_sfpload (nullptr, 0, 0, 0, 0, 0);

  // There's an erratum about this, we need to emit an extra insn
  auto dst = __builtin_rvtt_sfpshft2_subvec_shfl1 (src, 4);

  __builtin_rvtt_sfpstore (nullptr, dst, 0, 0, 0, 0, 0);
}
/*
**_Z17subvec_shfl_shiftv:
**	SFPLOAD	L0, 0, 0, 0
**	SFPSHFT2	L0, L0, 0, 4
**	SFPNOP
**	SFPSTORE	L0, 0, 0, 0
**	ret
*/

void subvec_shfl_dead ()
{
  auto src = __builtin_rvtt_sfpload (nullptr, 0, 0, 0, 0, 0);

  auto dst = __builtin_rvtt_sfpshft2_subvec_shfl1 (src, 3);
}
/*
**_Z16subvec_shfl_deadv:
**	SFPLOAD	L0, 0, 0, 0
**	SFPSHFT2	L8, L0, 0, 3
**	ret
*/
