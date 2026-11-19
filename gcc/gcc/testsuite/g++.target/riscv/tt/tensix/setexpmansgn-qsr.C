// { dg-options "-mcpu=tt-qsr32-tensix -fno-exceptions -fno-rtti -O2 -fno-shrink-wrap" }
// { dg-final { check-function-bodies "**" "" } }

namespace setexp {
  void set_v () {
    auto a = __builtin_rvtt_sfpreadlreg (0);
    auto b = __builtin_rvtt_sfpreadlreg (1);
    auto r = __builtin_rvtt_sfpsetexp_v (a, b, 0);
    __builtin_rvtt_sfpwritelreg (r, 0);
  }
/*
**_ZN6setexp5set_vEv:
**	# READ L0
**	# READ L1
** 	SFPSETEXP	L1, L0, 0, 0
**	SFPMOV	L0, L1, 2
**	# WRITE L0
**	ret
*/

    void cpy_v () {
    auto a = __builtin_rvtt_sfpreadlreg (0);
    auto b = __builtin_rvtt_sfpreadlreg (1);
    auto r = __builtin_rvtt_sfpsetexp_v (a, b, 2);
    __builtin_rvtt_sfpwritelreg (r, 0);
  }
/*
**_ZN6setexp5cpy_vEv:
**	# READ L0
**	# READ L1
** 	SFPSETEXP	L1, L0, 0, 2
**	SFPMOV	L0, L1, 2
**	# WRITE L0
**	ret
*/

  void set_i () {
    auto a = __builtin_rvtt_sfpreadlreg (0);
    auto r = __builtin_rvtt_sfpsetexp_i (nullptr, a, 12, 0, 0, 0);
    __builtin_rvtt_sfpwritelreg (r, 0);
  }
/*
**_ZN6setexp5set_iEv:
**	# READ L0
**	SFPSETEXP	L0, L0, 12, 1
**	# WRITE L0
**	ret
*/
}

namespace setman {
  void set_v () {
    auto a = __builtin_rvtt_sfpreadlreg (0);
    auto b = __builtin_rvtt_sfpreadlreg (1);
    auto r = __builtin_rvtt_sfpsetman_v (a, b, 0);
    __builtin_rvtt_sfpwritelreg (r, 0);
  }
/*
**_ZN6setman5set_vEv:
**	# READ L0
**	# READ L1
** 	SFPSETMAN	L1, L0, 0, 0
**	SFPMOV	L0, L1, 2
**	# WRITE L0
**	ret
*/

  void set_i () {
    auto a = __builtin_rvtt_sfpreadlreg (0);
    auto r = __builtin_rvtt_sfpsetman_i (nullptr, a, 12, 0, 0, 0);
    __builtin_rvtt_sfpwritelreg (r, 0);
  }
/*
**_ZN6setman5set_iEv:
**	# READ L0
**	SFPSETMAN	L0, L0, 12, 1
**	# WRITE L0
**	ret
*/
}

namespace setsgn {
  void set_v () {
    auto a = __builtin_rvtt_sfpreadlreg (0);
    auto b = __builtin_rvtt_sfpreadlreg (1);
    auto r = __builtin_rvtt_sfpsetsgn_v (a, b, 0);
    __builtin_rvtt_sfpwritelreg (r, 0);
  }
/*
**_ZN6setsgn5set_vEv:
**	# READ L0
**	# READ L1
** 	SFPSETSGN	L1, L0, 0, 0
**	SFPMOV	L0, L1, 2
**	# WRITE L0
**	ret
*/

  void set_i () {
    auto a = __builtin_rvtt_sfpreadlreg (0);
    auto r = __builtin_rvtt_sfpsetsgn_i (nullptr, a, 13, 0, 0, 0);
    __builtin_rvtt_sfpwritelreg (r, 0);
  }
/*
**_ZN6setsgn5set_iEv:
**	# READ L0
**	SFPSETSGN	L0, L0, 1, 1
**	# WRITE L0
**	ret
*/
}
