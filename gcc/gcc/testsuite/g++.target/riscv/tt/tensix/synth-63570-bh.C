// { dg-options "-mcpu=tt-bh-tensix -fno-exceptions -fno-rtti -O2" }
// { dg-final { check-function-bodies "**" "" } }

extern volatile unsigned long iptr[];
unsigned frob ();

void zero ()
{
  unsigned i = frob ();
  // no renumbering
  auto id = __builtin_rvtt_synth_opcode (1, 0);
  auto sum = __builtin_rvtt_synth_add2 (id, 0, i);
  __builtin_rvtt_ttreplay (iptr, 0, i, sum, 1, 0, 0);
  __builtin_rvtt_ttreplay (iptr, 0, i, sum, 1, 0, 0);
}
/*
**_Z4zerov:
**	addi	sp,sp,-16
**	sw	ra,12\(sp\)
**	call	_Z4frobv
**	li	a5, 67108864	# 1:4000000
**	lui	a4,%hi\(iptr\)
**	add	a0,a5,a0
**	addi	a5,a4,%lo\(iptr\)
**	sw	a0, 0\(a5\)	# 1:TTREPLAY	0, -, 0, 0
**	sw	a0, 0\(a5\)	# 1:TTREPLAY	0, -, 0, 0
**	lw	ra,12\(sp\)
**	addi	sp,sp,16
**	jr	ra
*/

void one ()
{
  unsigned i = frob ();
  unsigned j = frob ();
  // renumbering
  auto id = __builtin_rvtt_synth_opcode (1, 0);
  auto sum1 = __builtin_rvtt_synth_add2 (id, i, 1);
  auto sum2 = __builtin_rvtt_synth_add2 (id, j, 2);

  __builtin_rvtt_ttreplay (iptr, i, 1, sum1, 1, 0, 0);
  __builtin_rvtt_ttreplay (iptr, j, 2, sum2, 1, 0, 0);
}
/*
**_Z3onev:
**	addi	sp,sp,-16
**	sw	ra,12\(sp\)
**	sw	s0,8\(sp\)
**	call	_Z4frobv
**	mv	s0,a0
**	call	_Z4frobv
**	li	a4, 67108880	# 3:4000010
**	li	a5, 67108896	# 2:4000020
**	lui	a3,%hi\(iptr\)
**	add	a4,a4,s0
**	add	a5,a5,a0
**	addi	a3,a3,%lo\(iptr\)
**	sw	a4, 0\(a3\)	# 3:TTREPLAY	-, 1, 0, 0
**	sw	a5, 0\(a3\)	# 2:TTREPLAY	-, 2, 0, 0
**	lw	ra,12\(sp\)
**	lw	s0,8\(sp\)
**	addi	sp,sp,16
**	jr	ra
*/

void oneb ()
{
  unsigned i = frob ();
  unsigned j = frob ();
  // renumbering
  auto id = __builtin_rvtt_synth_opcode (1, 0);
  auto sum1 = __builtin_rvtt_synth_add2 (id, i, i);
  auto sum2 = __builtin_rvtt_synth_add2 (id, j, j);

  __builtin_rvtt_ttreplay (iptr, i, i, sum1, 1, 0, 0);
  __builtin_rvtt_ttreplay (iptr, j, j, sum2, 1, 0, 0);
}
/*
**_Z4onebv:
**	addi	sp,sp,-16
**	sw	ra,12\(sp\)
**	sw	s0,8\(sp\)
**	call	_Z4frobv
**	mv	s0,a0
**	call	_Z4frobv
**	lui	a5,%hi\(iptr\)
**	li	a3, 67108864	# 2:4000000
**	li	a4, 67108864	# 3:4000000
**	sh1add	a0,a0,a3
**	addi	a5,a5,%lo\(iptr\)
**	sh1add	a4,s0,a4
**	sw	a4, 0\(a5\)	# 3:TTREPLAY	-, -, 0, 0
**	sw	a0, 0\(a5\)	# 2:TTREPLAY	-, -, 0, 0
**	lw	ra,12\(sp\)
**	lw	s0,8\(sp\)
**	addi	sp,sp,16
**	jr	ra
*/

void two ()
{
  unsigned i = frob ();
  unsigned j = frob ();
  // renumbering
  auto id1 = __builtin_rvtt_synth_opcode (1, 0);
  auto sum1 = __builtin_rvtt_synth_add2 (id1, i, 1);
  __builtin_rvtt_ttreplay (iptr, i, 1, sum1, 1, 0, 0);
  auto id2 = __builtin_rvtt_synth_opcode (1, 1);
  auto sum2 = __builtin_rvtt_synth_add2 (id2, j, 1);
  __builtin_rvtt_ttreplay (iptr, j, 1, sum2, 1, 0, 0);
}
/*
**_Z3twov:
**	addi	sp,sp,-16
**	sw	ra,12\(sp\)
**	sw	s0,8\(sp\)
**	call	_Z4frobv
**	mv	s0,a0
**	call	_Z4frobv
**	li	a5, 67108880	# 2:4000010
**	lui	a4,%hi\(iptr\)
**	add	a5,a5,s0
**	addi	a4,a4,%lo\(iptr\)
**	sw	a5, 0\(a4\)	# 2:TTREPLAY	-, 1, 0, 0
**	li	a5, 67108881	# 3:4000011
**	add	a5,a5,a0
**	sw	a5, 0\(a4\)	# 3:TTREPLAY	-, 1, 0, 0
**	lw	ra,12\(sp\)
**	lw	s0,8\(sp\)
**	addi	sp,sp,16
**	jr	ra
*/

void three ()
{
  // CSE
  unsigned i = frob ();
  unsigned arg;
  if (i & 1)
    {
      auto id = __builtin_rvtt_synth_opcode (1, 0);
      arg = __builtin_rvtt_synth_add2 (id, 0, i);
    }
  else
    {
      auto id = __builtin_rvtt_synth_opcode (1, 1);
      arg = __builtin_rvtt_synth_add2 (id, 0, i);
    }
  __builtin_rvtt_ttreplay (iptr, 0, i, arg, 1, 0, 0);
}
/*
**_Z5threev:
**	addi	sp,sp,-16
**	sw	ra,12\(sp\)
**	call	_Z4frobv
**	li	a5, 67108864	# 1:4000000
**	add	a5,a5,a0
**	lui	a4,%hi\(iptr\)
**	sw	a5, %lo\(iptr\)\(a4\)	# 1:TTREPLAY	0, -, 0, 0
**	lw	ra,12\(sp\)
**	addi	sp,sp,16
**	jr	ra
*/

void threeb ()
{
  // CSE
  unsigned i = frob ();
  unsigned arg;
  if (i & 1)
    {
      auto id = __builtin_rvtt_synth_opcode (1, 0);
      arg = __builtin_rvtt_synth_add2 (id, i, i);
    }
  else
    {
      auto id = __builtin_rvtt_synth_opcode (1, 1);
      arg = __builtin_rvtt_synth_add2 (id, i, i);
    }
  __builtin_rvtt_ttreplay (iptr, i, i, arg, 1, 0, 0);
}
/*
**_Z6threebv:
**	addi	sp,sp,-16
**	sw	ra,12\(sp\)
**	call	_Z4frobv
**	li	a5, 67108864	# 1:4000000
**	lui	a4,%hi\(iptr\)
**	sh1add	a0,a0,a5
**	sw	a0, %lo\(iptr\)\(a4\)	# 1:TTREPLAY	-, -, 0, 0
**	lw	ra,12\(sp\)
**	addi	sp,sp,16
**	jr	ra
*/

void threec ()
{
  // CSE
  unsigned i = frob ();
  unsigned j = frob ();
  unsigned arg;
  if (i & 1)
    {
      auto id = __builtin_rvtt_synth_opcode (1, 0);
      arg = __builtin_rvtt_synth_add2 (id, j, i);
    }
  else
    {
      auto id = __builtin_rvtt_synth_opcode (1, 1);
      arg = __builtin_rvtt_synth_add2 (id, j, i);
    }
  __builtin_rvtt_ttreplay (iptr, j, i, arg, 1, 0, 0);
}
/*
**_Z6threecv:
**	addi	sp,sp,-16
**	sw	ra,12\(sp\)
**	sw	s0,8\(sp\)
**	call	_Z4frobv
**	mv	s0,a0
**	call	_Z4frobv
**	li	a5, 67108864	# 1:4000000
**	add	a5,a5,a0
**	add	a0,a5,s0
**	lui	a5,%hi\(iptr\)
**	sw	a0, %lo\(iptr\)\(a5\)	# 1:TTREPLAY	-, -, 0, 0
**	lw	ra,12\(sp\)
**	lw	s0,8\(sp\)
**	addi	sp,sp,16
**	jr	ra
*/

void four ()
{
  // CSE
  unsigned i = frob ();
  unsigned id;
  if (i & 1)
    id = __builtin_rvtt_synth_opcode (1, 0);
  else
    id = __builtin_rvtt_synth_opcode (1, 1);
  auto sum = __builtin_rvtt_synth_add2 (id, 0, i);
  __builtin_rvtt_ttreplay (iptr, 0, i, sum, 1, 0, 0);
}
/*
**_Z4fourv:
**	addi	sp,sp,-16
**	sw	ra,12\(sp\)
**	call	_Z4frobv
**	li	a5, 67108864	# 1:4000000
**	add	a5,a5,a0
**	lui	a4,%hi\(iptr\)
**	sw	a5, %lo\(iptr\)\(a4\)	# 1:TTREPLAY	0, -, 0, 0
**	lw	ra,12\(sp\)
**	addi	sp,sp,16
**	jr	ra
*/
