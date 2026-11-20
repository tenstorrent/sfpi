// { dg-options "-mcpu=tt-bh-tensix -O2 -I [SFPI]/include -fno-exceptions -fno-rtti" }
// { dg-final { check-function-bodies "**" "" } }

#include <lltt.h>

void record (unsigned length) {
  lltt::record(16, length);
  lltt::record<lltt::Exec>(20, length);
}
/*
**_Z6recordj:
**	slli	a0,a0,4
**	andi	a0,a0,1008
**	li	a5, 67371009	# 1:4040001
**	lui	a4,%hi\(__instrn_buffer\)
**	add	a5,(a0,a5|a5,a0)
**	addi	a4,a4,%lo\(__instrn_buffer\)
**	sw	a5, 0\(a4\)	# 1:TTREPLAY	16, -, 0, 1
**	li	a5, 67436547	# 2:4050003
**	add	a5,a5,a0
**	sw	a5, 0\(a4\)	# 2:TTREPLAY	20, -, 1, 1
**	ret
*/

std::uint32_t replay (unsigned length) {
  lltt::replay(16, length);
  return lltt::replay_insn(20, length);
}
/*
**_Z6replayj:
**	slli	a0,a0,4
**	andi	a4,a0,1008
**	li	a5, 67371008	# 1:4040000
**	add	a5,a5,a4
**	lui	a4,%hi\(__instrn_buffer\)
**	sw	a5, %lo\(__instrn_buffer\)\(a4\)	# 1:TTREPLAY	16, -, 0, 0
**	li	a5,67436544
**	or	a0,a0,a5
**	ret
*/
