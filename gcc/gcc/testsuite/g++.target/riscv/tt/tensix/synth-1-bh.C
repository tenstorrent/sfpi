// { dg-options "-mcpu=tt-bh-tensix -fno-exceptions -fno-rtti -O2" }
// { dg-final { check-function-bodies "**" "" } }

extern volatile unsigned long iptr[];
unsigned frob ();

void zero ()
{
  unsigned i = frob ();
  // no renumbering
  auto id = __builtin_rvtt_synth_opcode (1, 0);

  auto val1 = __builtin_rvtt_sfpload (iptr, i, id + i, 1, 0, 0);
  auto val2 = __builtin_rvtt_sfpload (iptr, i, id + i, 1, 0, 0);

  __builtin_rvtt_sfpwritelreg (val1, 0);
  __builtin_rvtt_sfpwritelreg (val2, 1);
}
/*
**_Z4zerov:
**	addi	sp,sp,-16
**	sw	ra,12\(sp\)
**	call	_Z4frobv
**	li	a5, 1879048192	# 1:70000000
**	add	a0,a0,a5
**	lui	a5,%hi\(iptr\)
**	addi	a5,a5,%lo\(iptr\)
**	sw	a0, 0\(a5\)	# 1:SFPLOAD	L0, -, 0, 0
**	li	a4,1048576
**	xor	a4,a4,a0
**	sw	a4, 0\(a5\)	# 1:SFPLOAD	L1, -, 0, 0
**	# WRITE L0
**	# WRITE L1
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

  auto val1 = __builtin_rvtt_sfpload (iptr, i, id + i, 1, 0, 0);
  auto val2 = __builtin_rvtt_sfpload (iptr, j, id + j, 1, 0, 0);

  __builtin_rvtt_sfpwritelreg (val1, 0);
  __builtin_rvtt_sfpwritelreg (val2, 1);
}
/*
**_Z3onev:
**	addi	sp,sp,-16
**	sw	ra,12\(sp\)
**	sw	s0,8\(sp\)
**	call	_Z4frobv
**	mv	s0,a0
**	call	_Z4frobv
**	li	a4, 1879048192	# 3:70000000
**	lui	a5,%hi\(iptr\)
**	add	a4,a4,s0
**	addi	a5,a5,%lo\(iptr\)
**	sw	a4, 0\(a5\)	# 3:SFPLOAD	L0, -, 0, 0
**	li	a4, 1880096768	# 2:70100000
**	add	a4,a4,a0
**	sw	a4, 0\(a5\)	# 2:SFPLOAD	L1, -, 0, 0
**	# WRITE L0
**	# WRITE L1
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
  auto val1 = __builtin_rvtt_sfpload (iptr, i, id1 + i, 1, 0, 0);
  auto id2 = __builtin_rvtt_synth_opcode (1, 1);
  auto val2 = __builtin_rvtt_sfpload (iptr, j, id2 + j, 1, 0, 0);

  __builtin_rvtt_sfpwritelreg (val1, 0);
  __builtin_rvtt_sfpwritelreg (val2, 1);
}
/*
**_Z3twov:
**	addi	sp,sp,-16
**	sw	ra,12\(sp\)
**	sw	s0,8\(sp\)
**	call	_Z4frobv
**	mv	s0,a0
**	call	_Z4frobv
**	li	a4, 1879048192	# 2:70000000
**	lui	a5,%hi\(iptr\)
**	add	a4,a4,s0
**	addi	a5,a5,%lo\(iptr\)
**	sw	a4, 0\(a5\)	# 2:SFPLOAD	L0, -, 0, 0
**	li	a4, 1880096769	# 3:70100001
**	add	a4,a4,a0
**	sw	a4, 0\(a5\)	# 3:SFPLOAD	L1, -, 0, 0
**	# WRITE L0
**	# WRITE L1
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
      arg = id + i;
    }
  else
    {
      auto id = __builtin_rvtt_synth_opcode (1, 1);
      arg = id + i;
    }
  auto val1 = __builtin_rvtt_sfpload (iptr, i, arg, 1, 0, 0);
  __builtin_rvtt_sfpwritelreg (val1, 0);
}
/*
**_Z5threev:
**	addi	sp,sp,-16
**	sw	ra,12\(sp\)
**	call	_Z4frobv
**	li	a5, 1879048192	# 1:70000000
**	add	a5,a5,a0
**	lui	a4,%hi\(iptr\)
**	sw	a5, %lo\(iptr\)\(a4\)	# 1:SFPLOAD	L0, -, 0, 0
**	# WRITE L0
**	lw	ra,12\(sp\)
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
  auto val1 = __builtin_rvtt_sfpload (iptr, i, id + i, 1, 0, 0);
  __builtin_rvtt_sfpwritelreg (val1, 0);
}
/*
**_Z4fourv:
**	addi	sp,sp,-16
**	sw	ra,12\(sp\)
**	call	_Z4frobv
**	li	a5, 1879048192	# 1:70000000
**	add	a5,a5,a0
**	lui	a4,%hi\(iptr\)
**	sw	a5, %lo\(iptr\)\(a4\)	# 1:SFPLOAD	L0, -, 0, 0
**	# WRITE L0
**	lw	ra,12\(sp\)
**	addi	sp,sp,16
**	jr	ra
*/
