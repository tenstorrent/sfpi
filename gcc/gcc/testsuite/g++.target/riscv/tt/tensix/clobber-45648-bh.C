// { dg-options "-mcpu=tt-bh-tensix -O2 -fno-exceptions -fno-rtti" }
// { dg-final { check-function-bodies "**" "" } }

void foo () {
  auto a = __builtin_rvtt_sfpload (nullptr, 0, 0, 0, 0, 0);
  __builtin_rvtt_sfppushc (0);
  auto b = __builtin_rvtt_sfpnovalue ();

  a = __builtin_rvtt_sfpiadd_v_lv (b, a, a, 0);

  __builtin_rvtt_sfppopc (0);
  __builtin_rvtt_sfpstore (nullptr, a, 0, 0, 0, 0, 0);
}

/*
**_Z3foov:
**	SFPLOAD	L0, 0, 0, 0
**	SFPIADD	L0, L0, 0, 0
**	SFPENCC	3, 10
**	SFPSTORE	L0, 0, 0, 0
**	ret
*/
