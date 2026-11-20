;; Machine description for Tenstorrent SFPU Intrinsics.
;; Copyright (C) 2022-2026 Tenstorrent Inc.
;; Originated by Paul Keller (pkeller@tenstorrent.com)
;; Rewritten by Nathan Sidwell (nsidwell@tenstorrent.com, nathan@acm.org).

;; This file is part of GCC.

;; GCC is free software; you can redistribute it and/or modify
;; it under the terms of the GNU General Public License as published by
;; the Free Software Foundation; either version 3, or (at your option)
;; any later version.

;; GCC is distributed in the hope that it will be useful,
;; but WITHOUT ANY WARRANTY; without even the implied warranty of
;; MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
;; GNU General Public License for more details.

;; You should have received a copy of the GNU General Public License
;; along with GCC; see the file COPYING3.  If not see
;; <http://www.gnu.org/licenses/>.

;; WH  - ISA 2.0
;; BH  - ISA 3.0
;; QSR - ISA 4.0 (NEO)
;; TRI - ISA 4.1 (Not Yet Supported)

(include "tt/rvtt-predicates.md")
(include "tt/rvtt-tune.md")

(define_c_enum "unspec" [
  UNSPEC_SYNTH_OPCODE
  UNSPEC_SFPCSTLREG
  UNSPEC_SFPNOVAL
  UNSPEC_SFPOMIT
  UNSPEC_SFPCLEAVE ; cleave together and cleave apart, yay auto-antonyms!
])

(define_c_enum "unspecv" [
  ;; Tenstorrent SFPU unspecs.
  ;; INT for internal
  ;; IMM for immediate
  ;; LV for keep dst reg alive as input for predicated liveness

  UNSPECV_SFPVARLREG

  UNSPECV_SFPNOP
  UNSPECV_SFPBANKDONE
  UNSPECV_SFPASSIGN

  UNSPECV_SFPLOADI
  UNSPECV_SFPLOAD
  UNSPECV_SFPLOADSRCS
  UNSPECV_SFPSTORE
  UNSPECV_SFPSTORESRCS

  UNSPECV_SFPSETCC
  UNSPECV_SFPENCC
  UNSPECV_SFPCOMPC
  UNSPECV_SFPPUSHC
  UNSPECV_SFPPOPC

  UNSPECV_SFPMUL
  UNSPECV_SFPMULI
  UNSPECV_SFPADD
  UNSPECV_SFPADDI
  UNSPECV_SFPMAD
  UNSPECV_SFPIADD

  UNSPECV_SFPMOV
  UNSPECV_SFPEXEXP
  UNSPECV_SFPEXMAN
  UNSPECV_SFPABS
  UNSPECV_SFPLZ
  UNSPECV_SFPSETEXP
  UNSPECV_SFPSETMAN
  UNSPECV_SFPSETSGN
  UNSPECV_SFPAND
  UNSPECV_SFPOR
  UNSPECV_SFPXOR
  UNSPECV_SFPNOT

  UNSPECV_SFPSHFT
  UNSPECV_SFPCAST

  UNSPECV_SFPDIVP2
  UNSPECV_SFPSTOCHRND

  UNSPECV_SFPCONFIG

  UNSPECV_SFPLUT
  UNSPECV_SFPLUTFP32

  UNSPECV_SFPSWAP
  UNSPECV_SFPTRANSP
  UNSPECV_SFPSHFT2_COPY4
  UNSPECV_SFPSHFT2_SUBVEC_COPY4
  UNSPECV_SFPSHFT2_SUBVEC_SHFL1_COPY4
  UNSPECV_SFPSHFT2_SUBVEC_SHFL1

  UNSPECV_SFPGT
  UNSPECV_SFPLE

  UNSPECV_SFPMUL24
  UNSPECV_SFPARECIP
  UNSPECV_SFPNONLINEAR

  UNSPECV_TTINSN
  UNSPECV_TTREPLAY
  UNSPECV_TTINCRWC
])

(define_enum "xtt_delay" [
  none
  static
  dynamic
])
(define_enum_attr "xtt_delay" "xtt_delay"
  (const_string "none"))
;; BH & QSR eratta covers problems with detecting dependent insns in dynamic delay slots
;; this is a bit mask BH:bit0, QSR:bit1
(define_enum "xtt_dynamic_bug" [
  none
  (bh 1)
  (qsr 2)
])
(define_enum_attr "xtt_dynamic_bug" "xtt_dynamic_bug" (const_string "none"))

;; rvtt_synth_opcode is used to synthesize sfp/tt instructions that
;; are injected into the instruction stream.  rvtt_synth_opcode is
;; tied to 1 or more insns (unrolling can do that). The ID does that
;; (SSA DEP-USE chains are insufficient as we need to prevent CSE
;; merging unrelated synth_opcode builtin calls). The first src
;; operand is later replaced with the constant parts of the
;; instruction encoding (once register allocation has
;; happened). Because different insns might have different register
;; uses (but mostly don't) we need to check that the registers are
;; still consistent and fixup if not at code emission time.
(define_insn "rvtt_synth_opcode"
  [(set (match_operand:SI 0 "register_operand" "=r")
        (unspec:SI [
	  (match_operand:SI 1 "const_int_operand" "n")
          (match_operand:SI 2 "const_int_operand" "n")
	  ] UNSPEC_SYNTH_OPCODE))]
  "TARGET_XTT_TENSIX"
{
  static char pattern[32];
  unsigned pos = 0;

  pos += snprintf (&pattern[pos], sizeof (pattern) - pos,
		   "li\t%%0, %%2\t# %d:%x", unsigned (UINTVAL (operands[1])),
		   unsigned (UINTVAL (operands[2])));
  gcc_assert (pos < sizeof (pattern));

  return pattern;
}
  [(set_attr "type" "const")])

(define_expand "rvtt_synth_add2"
  [(set (match_operand:SI 0 "register_operand")
        (unspec:SI [
	  (match_operand:SI 1 "register_operand") ;; synth result
	  (match_operand:SI 2 "arith_operand") ;; immediate 0
          (match_operand:SI 3 "arith_operand") ;; immediate 1
	  ] 0))]
  "TARGET_XTT_TENSIX"
{
  // expands to up to 2 adds
  rtx sum = operands[1];
  for (unsigned ix = 2; ix != 4; ix++)
    if (operands[ix] != CONST0_RTX(SImode))
      {
        rtx tmp = gen_reg_rtx (SImode);
        emit_insn (gen_addsi3 (tmp, sum, operands[ix]));
        sum = tmp;
      }
  emit_move_insn (operands[0], sum);
  DONE;
})

(define_expand "rvtt_lreg_pressure"
  [(unspec:SI [
     (match_operand:SI 0 "const_int_operand")]
     0)]
  "TARGET_XTT_TENSIX"
  {
    // We have to emit something.  DCE will throw this away
    emit_move_insn (gen_reg_rtx (SImode), operands[0]);
    DONE;
  })

(define_expand "rvtt_sfpreadlreg"
  [(set (match_operand:XTT32SI 0 "register_operand")
        (unspec_volatile:XTT32SI [
	  (match_operand:SI 1 "const_int_operand")
	  ] UNSPECV_SFPVARLREG))]
  "TARGET_XTT_TENSIX"
  {
    unsigned regno = INTVAL (operands[1]);
    if (regno >= SFPU_CREG_IDX_LWM)
      {
        rtx src_op = rvtt_gen_rtx_creg (GET_MODE (operands[0]), regno);
        emit_insn (gen_rtx_SET (operands[0], src_op));
        DONE;
      }
  })

(define_expand "rvtt_sfpwritelreg"
  [(unspec_volatile:XTT32SI [
     (match_operand:XTT32SI 0 "register_operand")
     (match_operand:SI      1 "const_int_operand")
     ] UNSPECV_SFPVARLREG)]
  "TARGET_XTT_TENSIX")

(define_int_iterator rvtt_lregs [0 1 2 3 4 5 6 7])
;; We have to map the number to a string.
(define_int_attr rvtt_lregs_value
  [(0 "0") (1 "1") (2 "2") (3 "3") (4 "4") (5 "5") (6 "6") (7 "7")])

(define_insn "rvtt_sfpwritelreg<rvtt_lregs_value>"
  [(unspec_volatile:XTT32SI [
     (match_operand:XTT32SI 0 "register_operand" "x<rvtt_lregs_value>")
     (const_int rvtt_lregs)
     ] UNSPECV_SFPVARLREG)]
  "TARGET_XTT_TENSIX"
  "# WRITE %0"
  [(set_attr "type" "tensix")
   (set_attr "length" "0")])

(define_insn "rvtt_sfpreadlreg<rvtt_lregs_value>"
  [(set (match_operand:XTT32SI 0 "register_operand" "=x<rvtt_lregs_value>")
        (unspec_volatile:XTT32SI [
	  (const_int rvtt_lregs)
	  ] UNSPECV_SFPVARLREG))]
  "TARGET_XTT_TENSIX"
  "# READ %0"
;; not a xtt_dynamic_bug consumer, it is for the user to get this right.
  [(set_attr "type" "tensix")
   (set_attr "length" "0")])

(define_insn "rvtt_sfpnovalue"
  [(set (match_operand:XTT32SI 0 "register_operand")
        (unspec:XTT32SI [
	  (const_int 0)
	  ] UNSPEC_SFPNOVAL))]
  "TARGET_XTT_TENSIX"
  "# NOVALUE %0"
  [(set_attr "type" "tensix")
   (set_attr "length" "0")])

;; We need the predicate markers until here.  Turn them into something DCE will remove.
(define_expand "rvtt_sfpxpred"
  [(set:SI (match_operand:SI 0 "register_operand")
     (unspec:SI [
        (match_operand:SI 1 "const_int_operand")
        (match_operand:SI 2 "reg_or_const_int_operand")]
	0))]
  "TARGET_XTT_TENSIX"
  {
    emit_move_insn (operands[0], operands[2]);
    DONE;
  })

(define_expand "rvtt_sfpxcond"
  [(set:SI (match_operand:SI 0 "register_operand")
     (unspec:SI [
        (match_operand:SI 1 "const_int_operand")
        (match_operand:SI 2 "reg_or_const_int_operand")
        (match_operand:SI 3 "reg_or_const_int_operand")]
	0))]
  "TARGET_XTT_TENSIX"
  {
    emit_move_insn (operands[0], operands[2]);
    DONE;
  })

;; These builtins are converted by gimple passes, but the insns are still
;; needed due to the way we expand them.

(define_expand "rvtt_sfpxlogic"
  [(set (match_operand:SI 0 "register_operand")
        (unspec:SI [
          (match_operand:SI 1 "const_int_operand")
          (match_operand:SI 2 "reg_or_const_int_operand")
          (match_operand:SI 3 "reg_or_const_int_operand")
          ] 0))]
  "TARGET_XTT_TENSIX"
  "FAIL;")

(define_expand "rvtt_sfpxcondiX"
  [(set (match_operand:XTT32SI 0 "register_operand")
        (unspec:SI [
	  (match_operand:SI 1 "register_operand")
	  ] 0))]
  "TARGET_XTT_TENSIX"
  "FAIL;")

(define_expand "rvtt_sfpxcmp"
  [(set (match_operand:SI 0 "register_operand")
        (unspec_volatile:SI [
	  (match_operand:XTT32SI 1 "reg_or_cstlreg_operand")
          (match_operand:XTT32SI 2 "reg_or_cstlreg_operand")
          (match_operand:SI    3 "const_int_operand")
	  ] 0))]
  "TARGET_XTT_TENSIX"
{
  FAIL;
})

(define_expand "rvtt_sfpxloadi"
  [(set (match_operand:XTT32SI 0 "register_operand")
        (unspec:XTT32SI [
	  (match_operand:SI 1 "reg_or_0_operand")
          (match_operand:SI 2 "reg_or_const_int_operand")
          (match_operand:SI 3 "reg_or_0_operand")
          (match_operand:SI 4 "const_int_operand")
          (match_operand:SI 5 "const_int_operand")
	  ] 0))]
  "TARGET_XTT_TENSIX"
{
  FAIL;
})

(define_insn "rvtt_sfpconcat2"
  [(set (match_operand:XTT64SI 0 "register_operand" "=xr")
     (unspec:XTT64SI [
       (match_operand:XTT32SI 1 "register_operand" "xr")
       (match_operand:XTT32SI 2 "register_operand" "xr")
       ] UNSPEC_SFPCLEAVE))]
  "TARGET_XTT_TENSIX"
  "SFPBAD.CONCAT %0, %1, %2"
  [(set_attr "type" "tensix")])

(define_insn "rvtt_sfpselect2"
  [(set (match_operand:XTT32SI 0 "register_operand" "=xr")
     (unspec:XTT32SI [
       (match_operand:XTT64SI 1 "register_operand" "xr")
       (match_operand:SI 2 "const_int_operand" "n")
       ] UNSPEC_SFPCLEAVE))]
  "TARGET_XTT_TENSIX"
  "SFPBAD.SELECT %0, %1, %2"
  [(set_attr "type" "tensix")])

(define_insn "rvtt_sfpconcat4"
  [(set (match_operand:XTT128SI 0 "register_operand" "=xr")
     (unspec:XTT128SI [
       (match_operand:XTT32SI 1 "register_operand" "xr")
       (match_operand:XTT32SI 2 "register_operand" "xr")
       (match_operand:XTT32SI 3 "register_operand" "xr")
       (match_operand:XTT32SI 4 "register_operand" "xr")
       ] UNSPEC_SFPCLEAVE))]
  "TARGET_XTT_TENSIX"
  "SFPBAD.CONCAT %0, %1, %2, %3, %4"
  [(set_attr "type" "tensix")])

(define_insn "rvtt_sfpselect4"
  [(set (match_operand:XTT32SI 0 "register_operand" "=xr")
     (unspec:XTT32SI [
       (match_operand:XTT128SI 1 "register_operand" "xr")
       (match_operand:SI 2 "const_int_operand" "n")
       ] UNSPEC_SFPCLEAVE))]
  "TARGET_XTT_TENSIX"
  "SFPBAD.SELECT %0, %1, %2"
  [(set_attr "type" "tensix")])

(define_insn "rvtt_sfpconcat8"
  [(set (match_operand:XTT256SI 0 "register_operand" "=xr")
     (unspec:XTT256SI [
       (match_operand:XTT32SI 1 "register_operand" "xr")
       (match_operand:XTT32SI 2 "register_operand" "xr")
       (match_operand:XTT32SI 3 "register_operand" "xr")
       (match_operand:XTT32SI 4 "register_operand" "xr")
       (match_operand:XTT32SI 5 "register_operand" "xr")
       (match_operand:XTT32SI 6 "register_operand" "xr")
       (match_operand:XTT32SI 7 "register_operand" "xr")
       (match_operand:XTT32SI 8 "register_operand" "xr")
       ] UNSPEC_SFPCLEAVE))]
  "TARGET_XTT_TENSIX"
  "SFPBAD.CONCAT %0, %1, %2, %3, %4, %5, %6, %7, %8"
  [(set_attr "type" "tensix")])

(define_insn "rvtt_sfpselect8"
  [(set (match_operand:XTT32SI 0 "register_operand" "=xr")
     (unspec:XTT32SI [
       (match_operand:XTT256SI 1 "register_operand" "xr")
       (match_operand:SI 2 "const_int_operand" "n")
       ] UNSPEC_SFPCLEAVE))]
  "TARGET_XTT_TENSIX"
  "SFPBAD.SELECT %0, %1, %2"
  [(set_attr "type" "tensix")])

(define_insn "rvtt_sfpnop"
  [(unspec_volatile:XTT32SI [
     (const_int 0)
     ] UNSPECV_SFPNOP)]
  "TARGET_XTT_TENSIX"
  "SFPNOP"
  [(set_attr "type" "tensix")])

(define_insn "rvtt_sfpbankdone"
  [(unspec_volatile:XTT32SI [
     (match_operand:SI   0 "const_int_operand" "n")
     (match_operand:SI   1 "const_int_operand" "n")
     (match_operand:SI   2 "const_int_operand" "n")
     ] UNSPECV_SFPBANKDONE)]
  "TARGET_XTT_TENSIX_QSR"
  "SFPBANKDONE\t%0, %1, %2"
  [(set_attr "type" "tensix")])

(define_expand "movxtt32si"
  [(set (match_operand:XTT32SI 0 "")
	(match_operand:XTT32SI 1 ""))]
  "TARGET_XTT_TENSIX"
{
  if (riscv_legitimize_move (GET_MODE (operands[0]), operands[0], operands[1]))
    DONE;
})

;; the simple set must accept reg-movs, loads and stores. You can't
;; break this apart otherwise reload blows up when trying to spill/fill

(define_insn "rvtt_sfpassign"
  [(set (match_operand:XTT32SI 0 "nonimmediate_operand" "=xr,xr,m")
        (match_operand:XTT32SI 1 "nonimmediate_or_cstlreg_operand" "xrxc,m,xrxc"))]
  "TARGET_XTT_TENSIX
   && (register_operand (operands[0], XTT32SImode)
       || reg_or_cstlreg_operand (operands[1], XTT32SImode))"
  {
    static const char *const assembly[] = {
      "SFPMOV\t%0, %1, 2",
      "SFPBAD.LOAD\t%0, %1",
      "SFPBAD.STORE\t%1, %0",
    };
    if (which_alternative)
      rvtt_mov_error (insn);
    return assembly[which_alternative];
  }
  [(set_attr "type" "tensix")])

(define_expand "rvtt_sfpassign_lv"
  [(set (match_operand:XTT32SI 0 "register_operand")
        (unspec_volatile:XTT32SI [
	  (match_operand:XTT32SI 1 "reg_or_cstlreg_or_noval_operand")
          (match_operand:XTT32SI 2 "reg_or_cstlreg_operand")
	  ] UNSPECV_SFPASSIGN))]
  "TARGET_XTT_TENSIX"
  {
  })

(define_insn_and_split "*rvtt_sfpassign_lv_int"
  [(set (match_operand:XTT32SI 0 "register_operand" "=xr,xr")
        (unspec_volatile:XTT32SI [
	  (match_operand:XTT32SI 1 "reg_or_cstlreg_or_noval_operand" "xn,0")
          (match_operand:XTT32SI 2 "reg_or_cstlreg_operand" "xrxc,xrxc")
	  ] UNSPECV_SFPASSIGN))]
  "TARGET_XTT_TENSIX"
  "@
   #
   SFPMOV\t%0, %2, 0\t# LV:%1"
  "&& (noval_operand (operands[1], GET_MODE (operands[1]))
       || rtx_equal_p (operands[1], operands[2])
       || find_reg_note (insn, REG_UNUSED, operands[0]))"
  [(set (match_dup 0) (match_dup 2))]
  {
    // Setting it to a normal mov will leave DCE to deal with
    // the REG_UNUSED case, that's simpler than redetecting here.
  }
  [(set_attr "type" "tensix")])

(define_expand "rvtt_sfploadi"
  [(set (match_operand:XTT32SI 0 "register_operand")
        (unspec:XTT32SI [
	  (match_operand:SI 1 "reg_or_0_operand")
          (match_operand:SI 2 "reg_or_const_int_operand")
          (match_operand:SI 3 "reg_or_0_operand")
          (match_operand:SI 4 "const_int_operand")
          (match_operand:SI 5 "const_int_operand")
	  ] UNSPECV_SFPLOADI))]
  "TARGET_XTT_TENSIX"
{
  emit_insn (gen_rvtt_sfploadi_lv
    (operands[0], operands[1], rvtt_gen_rtx_noval (XTT32SImode),
     operands[2], operands[3], operands[4], operands[5]));
  DONE;
})

(define_expand "rvtt_sfploadi_lv"
  [(set (match_operand:XTT32SI 0 "register_operand")
        (unspec:XTT32SI [
	  (match_operand:SI    1 "reg_or_0_operand")
          (match_operand:XTT32SI 2 "register_operand")
          (match_operand:SI    3 "reg_or_const_int_operand")
          (match_operand:SI    4 "reg_or_0_operand")
          (match_operand:SI    5 "const_int_operand")
          (match_operand:SI    6 "const_int_operand")
	  ] UNSPECV_SFPLOADI))]
  "TARGET_XTT_TENSIX"
{
  auto mem = CONST0_RTX (SImode);
  auto opc = CONST0_RTX (SImode);
  auto enc = CONST0_RTX (SImode);
  auto insn = CONST0_RTX (SImode);
  auto imm = operands[3];
  if (!CONST_INT_P (imm))
    {
      mem = gen_rtx_MEM (SImode, operands[1]);
      int op
        = TARGET_XTT_TENSIX_WH  ? TT_OP_WH_SFPLOADI (0, INTVAL (operands[6]), 0)
        : TARGET_XTT_TENSIX_BH  ? TT_OP_BH_SFPLOADI (0, INTVAL (operands[6]), 0)
        : TARGET_XTT_TENSIX_QSR ? TT_OP_QSR_SFPLOADI (0, INTVAL (operands[6]), 0)
        : (gcc_unreachable (), 0);
      opc = GEN_INT (op);
      enc = GEN_INT (rvtt_synth (UINTVAL (operands[5])).src_shift (0).dst_shift (20));
      imm = rvtt_gen_rtx_noval (GET_MODE (imm));
      insn = operands[4];
    }

  emit_insn (gen_rvtt_sfploadi_lv_int
    (operands[0], mem, opc, enc, insn,
     rvtt_gen_rtx_noval (XTT32SImode),
     operands[2], imm, operands[6]));
  DONE;
})

(define_insn "rvtt_sfploadi_lv_int"
  [(set (match_operand:XTT32SI 0 "register_operand" "=xr,xr,xr,xr")
        (unspec_volatile:XTT32SI [
          (match_operand:SI    1 "mem_or_0_operand"  "J,J,m,m")
          (match_operand:SI    2 "const_int_operand" "J,J,n,n") ;; opcode
          (match_operand:SI    3 "const_int_operand" "J,J,n,n") ;; id, src & dst shifts
          (match_operand:SI    4 "reg_or_0_operand"  "J,J,r,r") ;; insn
          (match_operand:XTT32SI 5 "noval_operand"   "xn,xn,xn,xn") ;; src (none)
          (match_operand:XTT32SI 6 "reg_or_cstlreg_or_noval_operand" "xn,0,xn,0") ;; lv
          (match_operand:SI    7 "const_int_or_noval_operand" "n,n,xn,xn") ;; imm
          (match_operand:SI    8 "const_int_operand" "n,n,n,n")
	  ] UNSPECV_SFPLOADI))
   (clobber (match_scratch:SI  9 "=X,X,&r,&r"))]
  "TARGET_XTT_TENSIX"
  {
    return rvtt_synth::pattern (which_alternative >> 1,
       which_alternative & 1
       ? "SFPLOADI\t%0, %7, %8\t# LV:%6"
       : "SFPLOADI\t%0, %7, %8",
      operands, true, 9);
  }
  [(set_attr "type" "tensix")])

(define_expand "rvtt_sfpload"
  [(set (match_operand:XTT32SI 0 "register_operand")
        (unspec_volatile:XTT32SI [
          (match_operand:SI 1 "reg_or_0_operand")
          (match_operand:SI 2 "reg_or_const_int_operand")
          (match_operand:SI 3 "reg_or_0_operand")
          (match_operand:SI 4 "const_int_operand")
          (match_operand:SI 5 "const_int_operand")
          (match_operand:SI 6 "const_int_operand")
          ] UNSPECV_SFPLOAD))]
  "TARGET_XTT_TENSIX"
{
  emit_insn (gen_rvtt_sfpload_lv
    (operands[0], operands[1], rvtt_gen_rtx_noval (XTT32SImode), operands[2],
     operands[3], operands[4], operands[5], operands[6]));
  DONE;
})

(define_expand "rvtt_sfploadsrcs"
  [(set (match_operand:XTT32SI 0 "register_operand")
        (unspec_volatile:XTT32SI [
          (match_operand:SI 1 "reg_or_0_operand")
          (match_operand:SI 2 "reg_or_const_int_operand")
          (match_operand:SI 3 "reg_or_0_operand")
          (match_operand:SI 4 "const_int_operand")
          (match_operand:SI 5 "const_int_operand")
          (match_operand:SI 6 "const_int_operand")
          (match_operand:SI 7 "const_int_operand")
          ] UNSPECV_SFPLOADSRCS))]
  "TARGET_XTT_TENSIX_QSR"
{
  emit_insn (gen_rvtt_sfploadsrcs_lv
    (operands[0], operands[1], rvtt_gen_rtx_noval (XTT32SImode), operands[2],
     operands[3], operands[4], operands[5], operands[6], operands[7]));
  DONE;
})

(define_expand "rvtt_sfpload_lv"
  [(set (match_operand:XTT32SI 0 "register_operand")
        (unspec_volatile:XTT32SI [
          (match_operand:SI    1 "reg_or_0_operand")
          (match_operand:XTT32SI 2 "reg_or_cstlreg_or_noval_operand")
          (match_operand:SI    3 "reg_or_const_int_operand")
          (match_operand:SI    4 "reg_or_0_operand")
          (match_operand:SI    5 "const_int_operand")
          (match_operand:SI    6 "const_int_operand")
          (match_operand:SI    7 "const_int_operand")
	  ] UNSPECV_SFPLOAD))]
  "TARGET_XTT_TENSIX"
{
  auto mem = CONST0_RTX (SImode);
  auto opc = CONST0_RTX (SImode);
  auto enc = CONST0_RTX (SImode);
  auto insn = CONST0_RTX (SImode);
  auto imm = operands[3];
  if (!CONST_INT_P (imm))
    {
      mem = gen_rtx_MEM (SImode, operands[1]);
      int op
        = TARGET_XTT_TENSIX_WH 	? TT_OP_WH_SFPLOAD (0, INTVAL (operands[6]), INTVAL (operands[7]), 0)
	: TARGET_XTT_TENSIX_BH 	? TT_OP_BH_SFPLOAD (0, INTVAL (operands[6]), INTVAL (operands[7]), 0)
	: TARGET_XTT_TENSIX_QSR	? TT_OP_QSR_SFPLOAD (0, INTVAL (operands[6]), INTVAL (operands[7]), 0, 0)
        : (gcc_unreachable (), 0);
      opc = GEN_INT (op);
      enc = GEN_INT (rvtt_synth (UINTVAL (operands[5])).dst_shift (20));
      imm = rvtt_gen_rtx_noval (GET_MODE (imm));
      insn = operands[4];
    }

  emit_insn (gen_rvtt_sfpload_lv_int
    (operands[0], mem, opc, enc, insn,
     rvtt_gen_rtx_noval (XTT32SImode),
     operands[2], imm, operands[6], operands[7]));
  DONE;
})

(define_expand "rvtt_sfploadsrcs_lv"
  [(set (match_operand:XTT32SI 0 "register_operand")
        (unspec_volatile:XTT32SI [
          (match_operand:SI    1 "reg_or_0_operand")
          (match_operand:XTT32SI 2 "reg_or_cstlreg_or_noval_operand")
          (match_operand:SI    3 "reg_or_const_int_operand")
          (match_operand:SI    4 "reg_or_0_operand")
          (match_operand:SI    5 "const_int_operand")
          (match_operand:SI    6 "const_int_operand")
          (match_operand:SI    7 "const_int_operand")
          (match_operand:SI    8 "const_int_operand")
	  ] UNSPECV_SFPLOADSRCS))]
  "TARGET_XTT_TENSIX_QSR"
{
  auto mem = CONST0_RTX (SImode);
  auto opc = CONST0_RTX (SImode);
  auto enc = CONST0_RTX (SImode);
  auto insn = CONST0_RTX (SImode);
  auto imm = operands[3];
  if (!CONST_INT_P (imm))
    {
      mem = gen_rtx_MEM (SImode, operands[1]);
      int op = TT_OP_QSR_SFPLOAD (0, INTVAL (operands[6]), INTVAL (operands[7]), INTVAL (operands[8]), 1 << 10);
      opc = GEN_INT (op);
      enc = GEN_INT (rvtt_synth (UINTVAL (operands[5])).dst_shift (20));
      imm = rvtt_gen_rtx_noval (GET_MODE (imm));
      insn = operands[4];
    }

  emit_insn (gen_rvtt_sfploadsrcs_lv_int
    (operands[0], mem, opc, enc, insn,
     rvtt_gen_rtx_noval (XTT32SImode),
     operands[2], imm, operands[6], operands[7], operands[8]));
  DONE;
})

(define_insn "rvtt_sfpload_lv_int"
  [(set (match_operand:XTT32SI 0 "register_operand" "=xr,xr,xr,xr")
        (unspec_volatile:XTT32SI [
          (match_operand:SI    1 "mem_or_0_operand"  "J,J,m,m")
          (match_operand:SI    2 "const_int_operand" "J,J,n,n") ;; opcode
          (match_operand:SI    3 "const_int_operand" "J,J,n,n") ;; id, src & dst shifts
          (match_operand:SI    4 "reg_or_0_operand"  "J,J,r,r") ;; insn
          (match_operand:XTT32SI 5 "noval_operand"   "xn,xn,xn,xn") ;; src (none)
          (match_operand:XTT32SI 6 "reg_or_cstlreg_or_noval_operand" "xn,0,xn,0") ;; lv
          (match_operand:SI    7 "const_int_or_noval_operand" "n,n,xn,xn") ;; imm
          (match_operand:SI    8 "const_int_operand" "n,n,n,n")
          (match_operand:SI    9 "const_int_operand" "n,n,n,n")
          ] UNSPECV_SFPLOAD))
   (clobber (match_scratch:SI 10 "=X,X,&r,&r"))]
  "TARGET_XTT_TENSIX"
  {
    return rvtt_synth::pattern (which_alternative >> 1,
      which_alternative & 1
      ? (TARGET_XTT_TENSIX_QSR ? "SFPLOAD\t%0, %7, %8, %9, 0, 0\t# LV:%6"
         : "SFPLOAD\t%0, %7, %8, %9\t# LV:%6")
      : (TARGET_XTT_TENSIX_QSR ? "SFPLOAD\t%0, %7, %8, %9, 0, 0"
         : "SFPLOAD\t%0, %7, %8, %9"),
      operands, true, 10);
  }
  [(set_attr "type" "tensix")])

(define_insn "rvtt_sfploadsrcs_lv_int"
  [(set (match_operand:XTT32SI 0 "register_operand" "=xr,xr,xr,xr")
        (unspec_volatile:XTT32SI [
          (match_operand:SI    1 "mem_or_0_operand"  "J,J,m,m")
          (match_operand:SI    2 "const_int_operand" "J,J,n,n") ;; opcode
          (match_operand:SI    3 "const_int_operand" "J,J,n,n") ;; id, src & dst shifts
          (match_operand:SI    4 "reg_or_0_operand"  "J,J,r,r") ;; insn
          (match_operand:XTT32SI 5 "noval_operand"   "xn,xn,xn,xn") ;; src (none)
          (match_operand:XTT32SI 6 "reg_or_cstlreg_or_noval_operand" "xn,0,xn,0") ;; lv
          (match_operand:SI    7 "const_int_or_noval_operand" "n,n,xn,xn") ;; imm
          (match_operand:SI    8 "const_int_operand" "n,n,n,n")
          (match_operand:SI    9 "const_int_operand" "n,n,n,n")
          (match_operand:SI   10 "const_int_operand" "n,n,n,n")
          ] UNSPECV_SFPLOADSRCS))
   (clobber (match_scratch:SI 11 "=X,X,&r,&r"))]
  "TARGET_XTT_TENSIX_QSR"
  {
    return rvtt_synth::pattern (which_alternative >> 1,
      which_alternative & 1
      ? "SFPLOAD\t%0, %7, %8, %9, 1, %10\t# LV:%6"
      : "SFPLOAD\t%0, %7, %8, %9, 1, %10",
      operands, true, 11);
  }
  [(set_attr "type" "tensix")])

(define_expand "rvtt_sfpstore"
  [(unspec_volatile:XTT32SI [
     (match_operand:SI    0 "reg_or_0_operand")
     (match_operand:XTT32SI 1 "reg_or_cstlreg_operand")
     (match_operand:SI    2 "reg_or_const_int_operand")
     (match_operand:SI    3 "reg_or_0_operand")
     (match_operand:SI    4 "const_int_operand")
     (match_operand:SI    5 "const_int_operand")
     (match_operand:SI    6 "const_int_operand")
     ] UNSPECV_SFPSTORE)]
  "TARGET_XTT_TENSIX"
{
  auto mem = CONST0_RTX (SImode);
  auto opc = CONST0_RTX (SImode);
  auto enc = CONST0_RTX (SImode);
  auto insn = CONST0_RTX (SImode);
  auto imm = operands[2];
  if (!CONST_INT_P (imm))
    {
      mem = gen_rtx_MEM (SImode, operands[0]);
      int op
        = TARGET_XTT_TENSIX_WH 	? TT_OP_WH_SFPSTORE (0, INTVAL (operands[5]), INTVAL (operands[6]), 0)
        : TARGET_XTT_TENSIX_BH  ? TT_OP_BH_SFPSTORE (0, INTVAL (operands[5]), INTVAL (operands[6]), 0)
        : TARGET_XTT_TENSIX_QSR ? TT_OP_QSR_SFPSTORE (0, INTVAL (operands[5]), INTVAL (operands[6]), 0, 0)
        : (gcc_unreachable (), 0);
      opc = GEN_INT (op);
      enc = GEN_INT (rvtt_synth (UINTVAL (operands[4])).src_shift (20));
      imm = rvtt_gen_rtx_noval (GET_MODE (imm));
      insn = operands[3];
    }

  emit_insn (gen_rvtt_sfpstore_int
    (mem, opc, enc, insn,
     operands[1], imm, operands[5], operands[6]));
  DONE;
})

;; stores cannot write from L12..L15 due to load macro side loading possibility
(define_insn "rvtt_sfpstore_int"
  [(unspec_volatile:XTT32SI [
    (match_operand:SI    0 "mem_or_0_operand"  "J,m")
    (match_operand:SI    1 "const_int_operand" "J,n") ;; opcode
    (match_operand:SI    2 "const_int_operand" "J,n") ;; id, src & dst shifts
    (match_operand:SI    3 "reg_or_0_operand"  "J,r") ;; insn
    (match_operand:XTT32SI 4 "reg_or_cstlreg_operand" "xrxs,xrxs") ;; src
    (match_operand:SI    5 "const_int_or_noval_operand" "n,xn") ;; imm
    (match_operand:SI    6 "const_int_operand" "n,n")
    (match_operand:SI    7 "const_int_operand" "n,n")
    ] UNSPECV_SFPSTORE)
   (clobber (match_scratch:SI  8 "=X,&r"))]
  "TARGET_XTT_TENSIX"
  {
    return rvtt_synth::pattern (which_alternative,
      TARGET_XTT_TENSIX_QSR ? "SFPSTORE\t%4, %5, %6, %7, 0, 0"
      : "SFPSTORE\t%4, %5, %6, %7",
      operands, false, 8);
  }
  [(set_attr "type" "tensix")])

(define_expand "rvtt_sfpstoresrcs"
  [(unspec_volatile:XTT32SI [
     (match_operand:SI    0 "reg_or_0_operand")
     (match_operand:XTT32SI 1 "reg_or_cstlreg_operand")
     (match_operand:SI    2 "reg_or_const_int_operand")
     (match_operand:SI    3 "reg_or_0_operand")
     (match_operand:SI    4 "const_int_operand")
     (match_operand:SI    5 "const_int_operand")
     (match_operand:SI    6 "const_int_operand")
     (match_operand:SI    7 "const_int_operand")
     ] UNSPECV_SFPSTORESRCS)]
  "TARGET_XTT_TENSIX_QSR"
{
  auto mem = CONST0_RTX (SImode);
  auto opc = CONST0_RTX (SImode);
  auto enc = CONST0_RTX (SImode);
  auto insn = CONST0_RTX (SImode);
  auto imm = operands[2];
  if (!CONST_INT_P (imm))
    {
      mem = gen_rtx_MEM (SImode, operands[0]);
      int op
        = TT_OP_QSR_SFPSTORE (0, INTVAL (operands[5]), INTVAL (operands[6]), INTVAL (operands[7]), 1 << 10);
      opc = GEN_INT (op);
      enc = GEN_INT (rvtt_synth (UINTVAL (operands[4])).src_shift (20));
      imm = rvtt_gen_rtx_noval (GET_MODE (imm));
      insn = operands[3];
    }

  emit_insn (gen_rvtt_sfpstoresrcs_int
    (mem, opc, enc, insn,
     operands[1], imm, operands[5], operands[6], operands[7]));
  DONE;
})

(define_insn "rvtt_sfpstoresrcs_int"
  [(unspec_volatile:XTT32SI [
    (match_operand:SI    0 "mem_or_0_operand"  "J,m")
    (match_operand:SI    1 "const_int_operand" "J,n") ;; opcode
    (match_operand:SI    2 "const_int_operand" "J,n") ;; id, src & dst shifts
    (match_operand:SI    3 "reg_or_0_operand"  "J,r") ;; insn
    (match_operand:XTT32SI 4 "reg_or_cstlreg_operand" "xrxs,xrxs") ;; src
    (match_operand:SI    5 "const_int_or_noval_operand" "n,xn") ;; imm
    (match_operand:SI    6 "const_int_operand" "n,n")
    (match_operand:SI    7 "const_int_operand" "n,n")
    (match_operand:SI    8 "const_int_operand" "n,n")
    ] UNSPECV_SFPSTORESRCS)
   (clobber (match_scratch:SI  9 "=X,&r"))]
  "TARGET_XTT_TENSIX_QSR"
  {
    return rvtt_synth::pattern (which_alternative,
      "SFPSTORE\t%4, %5, %6, %7, 1, %8",
      operands, false, 9);
  }
  [(set_attr "type" "tensix")])

(define_expand "rvtt_sfpsetcc_wh"
  [(unspec_volatile:XTT32SI [
     (match_operand:XTT32SI 0 "reg_or_cstlreg_operand"  "xrxc")
     (match_operand:SI   1 "const_int_operand" "n")
     ] UNSPECV_SFPSETCC)]
  "TARGET_XTT_TENSIX_WH_BH"
  {
    emit_insn (gen_rvtt_sfpsetcc
      (operands[0], operands[1], CONST0_RTX (SImode)));
    DONE;
  })

(define_expand "rvtt_sfpsetcc"
  [(unspec_volatile:XTT32SI [
     (match_operand:XTT32SI 0 "reg_or_cstlreg_operand"  "xrxc")
     (match_operand:SI   1 "const_int_operand" "n")
     (match_operand:SI   2 "const_int_operand" "n")
     ] UNSPECV_SFPSETCC)]
  "TARGET_XTT_TENSIX"
  {
    operands[2] = GEN_INT (INTVAL (operands[2]) << 11);
  })

(define_insn "*rvtt_sfpsetcc"
  [(unspec_volatile:XTT32SI [
     (match_operand:XTT32SI 0 "reg_or_cstlreg_operand"  "xrxc")
     (match_operand:SI   1 "const_int_operand" "n")
     (match_operand:SI   2 "const_int_operand" "n")
     ] UNSPECV_SFPSETCC)]
  "TARGET_XTT_TENSIX"
  "SFPSETCC\t%0, %2, %1"
  [(set_attr "type" "tensix")])

(define_insn "rvtt_sfpencc"
  [(unspec_volatile:XTT32SI [
     (match_operand:SI    0 "const_int_operand" "n")
     (match_operand:SI    1 "const_int_operand" "n")
     ] UNSPECV_SFPENCC)]
  "TARGET_XTT_TENSIX"
  "SFPENCC\t%1, %0"
  [(set_attr "type" "tensix")])

(define_insn "rvtt_sfpcompc"
  [(unspec_volatile:XTT32SI [
     (const_int 0)
     ] UNSPECV_SFPCOMPC)]
  "TARGET_XTT_TENSIX"
  "SFPCOMPC"
  [(set_attr "type" "tensix")])

(define_insn "rvtt_sfppushc"
  [(unspec_volatile:XTT32SI [
     (match_operand:SI    0 "const_int_operand" "n")
     ] UNSPECV_SFPPUSHC)]
  "TARGET_XTT_TENSIX"
  "SFPPUSHC\t%0"
  [(set_attr "type" "tensix")])

(define_insn "rvtt_sfppopc"
  [(unspec_volatile:XTT32SI [
     (match_operand:SI    0 "const_int_operand" "n")
     ] UNSPECV_SFPPOPC)]
  "TARGET_XTT_TENSIX"
  "SFPPOPC\t%0"
  [(set_attr "type" "tensix")])

(define_int_iterator rvtt_muladd_op [
  UNSPECV_SFPMUL
  UNSPECV_SFPADD
  ])
(define_int_attr rvtt_muladd_name [
  (UNSPECV_SFPMUL "mul")
  (UNSPECV_SFPADD "add")
  ])
(define_int_attr rvtt_muladd_insn [
  (UNSPECV_SFPMUL "MUL")
  (UNSPECV_SFPADD "ADD")
  ])
(define_expand "rvtt_sfp<rvtt_muladd_name>"
  [(set (match_operand:XTT32SI 0 "register_operand" "=xr")
        (unspec_volatile:XTT32SI [
	  (match_operand:XTT32SI 1 "reg_or_cstlreg_operand"  "xrxc")
          (match_operand:XTT32SI 2 "reg_or_cstlreg_operand"  "xrxc")
          (match_operand:SI    3 "const_int_operand" "n")
	  ] rvtt_muladd_op))]
  "TARGET_XTT_TENSIX"
  {
    emit_insn (gen_rvtt_sfp<rvtt_muladd_name>_lv
      (operands[0], rvtt_gen_rtx_noval (XTT32SImode),
       operands[1], operands[2], operands[3]));
    DONE;
  })

(define_insn "rvtt_sfp<rvtt_muladd_name>_lv"
  [(set (match_operand:XTT32SI 0 "register_operand" "=xr,xr")
        (unspec_volatile:XTT32SI [
	  (match_operand:XTT32SI 1 "reg_or_cstlreg_or_noval_operand" "xn,0")
          (match_operand:XTT32SI 2 "reg_or_cstlreg_operand"  "xrxc,xrxc")
          (match_operand:XTT32SI 3 "reg_or_cstlreg_operand"  "xrxc,xrxc")
          (match_operand:SI    4 "const_int_operand" "n,n")
	  ] rvtt_muladd_op))]
  "TARGET_XTT_TENSIX"
  "@
   SFP<rvtt_muladd_insn>\t%0, %2, %3, %4
   SFP<rvtt_muladd_insn>\t%0, %2, %3, %4\t# LV:%1"
  [(set_attr "type" "tensix")
   (set_attr "xtt_delay" "dynamic")])

(define_expand "rvtt_sfpmad"
  [(set (match_operand:XTT32SI 0 "register_operand")
        (unspec_volatile:XTT32SI [
	  (match_operand:XTT32SI 1 "reg_or_cstlreg_operand")
          (match_operand:XTT32SI 2 "reg_or_cstlreg_operand")
          (match_operand:XTT32SI 3 "reg_or_cstlreg_operand")
          (match_operand:SI    4 "const_int_operand")
	  ] UNSPECV_SFPMAD))]
  "TARGET_XTT_TENSIX"
{
  emit_insn (gen_rvtt_sfpmad_lv
    (operands[0], rvtt_gen_rtx_noval (XTT32SImode),
     operands[1], operands[2], operands[3], operands[4]));
  DONE;
})

(define_insn "rvtt_sfpmad_lv"
  [(set (match_operand:XTT32SI 0 "register_operand" "=xr,xr")
        (unspec_volatile:XTT32SI [
	  (match_operand:XTT32SI 1 "reg_or_cstlreg_or_noval_operand" "xn,0")
          (match_operand:XTT32SI 2 "reg_or_cstlreg_operand"  "xrxc,xrxc")
          (match_operand:XTT32SI 3 "reg_or_cstlreg_operand"  "xrxc,xrxc")
          (match_operand:XTT32SI 4 "reg_or_cstlreg_operand"  "xrxc,xrxc")
          (match_operand:SI    5 "const_int_operand" "n,n")
	  ] UNSPECV_SFPMAD))]
  "TARGET_XTT_TENSIX"
  "@
   SFPMAD\t%0, %2, %3, %4, %5
   SFPMAD\t%0, %2, %3, %4, %5\t# LV:%1"
  [(set_attr "type" "tensix")
   (set_attr "xtt_delay" "dynamic")])

(define_int_iterator rvtt_muliaddi_op [
  UNSPECV_SFPMULI
  UNSPECV_SFPADDI
  ])
(define_int_attr rvtt_muliaddi_name [
  (UNSPECV_SFPMULI "muli")
  (UNSPECV_SFPADDI "addi")
  ])
(define_int_attr rvtt_muliaddi_insn [
  (UNSPECV_SFPMULI "MULI")
  (UNSPECV_SFPADDI "ADDI")
  ])

(define_expand "rvtt_sfp<rvtt_muliaddi_name>"
  [(set (match_operand:XTT32SI 0 "register_operand")
        (unspec_volatile:XTT32SI [
	  (match_operand:SI    1 "reg_or_0_operand")
          (match_operand:XTT32SI 2 "reg_or_cstlreg_operand")
          (match_operand:SI    3 "reg_or_const_int_operand")
          (match_operand:SI    4 "reg_or_0_operand")
          (match_operand:SI    5 "const_int_operand")
          (match_operand:SI    6 "const_int_operand")
	  ] rvtt_muliaddi_op))]
  "TARGET_XTT_TENSIX"
{
  emit_insn (gen_rvtt_sfp<rvtt_muliaddi_name>_lv
    (operands[0], operands[1], rvtt_gen_rtx_noval (XTT32SImode),
     operands[2], operands[3], operands[4], operands[5], operands[6]));
  DONE;
})

(define_expand "rvtt_sfp<rvtt_muliaddi_name>_lv"
  [(set (match_operand:XTT32SI 0 "register_operand")
        (unspec_volatile:XTT32SI [
	  (match_operand:SI    1 "reg_or_0_operand")
          (match_operand:XTT32SI 2 "reg_or_cstlreg_or_noval_operand")
          (match_operand:XTT32SI 3 "reg_or_cstlreg_operand")
          (match_operand:SI    4 "reg_or_const_int_operand")
          (match_operand:SI    5 "reg_or_0_operand")
          (match_operand:SI    6 "const_int_operand")
          (match_operand:SI    7 "const_int_operand")
	  ] rvtt_muliaddi_op))]
  "TARGET_XTT_TENSIX"
{
  auto mem = CONST0_RTX (SImode);
  auto opc = CONST0_RTX (SImode);
  auto enc = CONST0_RTX (SImode);
  auto insn = CONST0_RTX (SImode);
  auto imm = operands[4];
  if (!CONST_INT_P (imm))
    {
      mem = gen_rtx_MEM (SImode, operands[1]);
      int op
        = TARGET_XTT_TENSIX_WH 	? TT_OP_WH_SFP<rvtt_muliaddi_insn> (0, 0, INTVAL (operands[7]))
	: TARGET_XTT_TENSIX_BH 	? TT_OP_BH_SFP<rvtt_muliaddi_insn> (0, 0, INTVAL (operands[7]))
	: TARGET_XTT_TENSIX_QSR	? TT_OP_QSR_SFP<rvtt_muliaddi_insn> (0, 0, INTVAL (operands[7]))
        : (gcc_unreachable (), 0);
      opc = GEN_INT (op);
      enc = GEN_INT (rvtt_synth (UINTVAL (operands[6])).src_shift (4).dst_shift (4));
      imm = rvtt_gen_rtx_noval (GET_MODE (imm));
      insn = operands[5];
    }

  emit_insn (gen_rvtt_sfp<rvtt_muliaddi_name>_int_lv
    (operands[0], mem, opc, enc, insn,
     operands[3], operands[2], imm, operands[7]));
  DONE;
})

(define_insn_and_rewrite "rvtt_sfp<rvtt_muliaddi_name>_int_lv"
  [(set (match_operand:XTT32SI 0 "register_operand" "=xr,xr,xr,xr,xr,xr")
        (unspec_volatile:XTT32SI [
          (match_operand:SI    1 "mem_or_0_operand"  "J,J,J,m,m,m")
          (match_operand:SI    2 "const_int_operand" "J,J,J,n,n,n") ;; opcode
          (match_operand:SI    3 "const_int_operand" "J,J,J,n,n,n") ;; id, src & dst shifts
          (match_operand:SI    4 "reg_or_0_operand"  "J,J,J,r,r,r") ;; insn
          (match_operand:XTT32SI 5 "reg_or_cstlreg_operand" "0,0,0,0,0,0") ;; src
          (match_operand:XTT32SI 6 "reg_or_cstlreg_or_noval_or_omit_operand" "xn,xo,xrxc,xn,xo,xrxc") ;; lv
          (match_operand:SI    7 "const_int_or_noval_operand" "n,n,n,xn,xn,xn") ;; imm
          (match_operand:SI    8 "const_int_operand" "n,n,n,n,n,n")
          ] rvtt_muliaddi_op))
   (clobber (match_scratch:SI  9 "=X,X,X,&r,&r,&r"))]
  "TARGET_XTT_TENSIX"
  {
    int which = which_alternative;
    bool dyn = which >= 3;
    if (dyn)
      which -= 3;
    return rvtt_synth::pattern (dyn,
        which == 0 ? "SFP<rvtt_muliaddi_insn>\t%0, %7, %8" :
        which == 1 ? "SFP<rvtt_muliaddi_insn>\t%0, %7, %8\t# LV:%5" :
        "#",
        operands, true, 9);
  }
  "&& !noval_or_omit_operand (operands[6], GET_MODE (operands[6]))"
  {
    rvtt_merge_lv_src (&operands[6], &operands[5]);
  }
  [(set_attr "type" "tensix")
   (set_attr "xtt_delay" "dynamic")])

(define_expand "rvtt_sfpiadd_v"
  [(set (match_operand:XTT32SI 0 "register_operand")
        (unspec_volatile:XTT32SI [
	(match_operand:XTT32SI 1 "register_operand")
        (match_operand:XTT32SI 2 "reg_or_cstlreg_operand")
        (match_operand:SI      3 "const_int_operand")
	] UNSPECV_SFPIADD))]
  "TARGET_XTT_TENSIX"
  {
    emit_insn (gen_rvtt_sfpiadd_v_lv
      (operands[0], rvtt_gen_rtx_noval (XTT32SImode),
       operands[1], operands[2], operands[3]));
    DONE;
  })

(define_insn_and_rewrite "rvtt_sfpiadd_v_lv"
  [(set (match_operand:XTT32SI 0 "register_operand" "=xr,xr,xr")
        (unspec_volatile:XTT32SI [
	  (match_operand:XTT32SI 1 "reg_or_cstlreg_or_noval_or_omit_operand" "xn,xo,xrxc")
	  (match_operand:XTT32SI 2 "reg_or_cstlreg_operand" "0,0,0")
          (match_operand:XTT32SI 3 "reg_or_cstlreg_operand" "xrxc,xrxc,xrxc")
          (match_operand:SI      4 "const_int_operand" "n,n,n")
	  ] UNSPECV_SFPIADD))]
  "TARGET_XTT_TENSIX"
  "@
   SFPIADD\t%0, %3, 0, %4
   SFPIADD\t%0, %3, 0, %4\t# LV:%2
   #"
  "&& true"
  {
    if (TARGET_XTT_TENSIX_QSR
        && find_reg_note (curr_insn, REG_UNUSED, operands[0])
        && cstlreg_operand (operands[2], GET_MODE (operands[2])))
      {
        emit_insn (gen_rvtt_sfpiadd_v_nv
	  (operands[2], operands[3], operands[4]));
        DONE;
      }

    if (noval_or_omit_operand (operands[1], GET_MODE (operands[1])))
      FAIL; // No need to do anything

    rvtt_merge_lv_src (&operands[1], &operands[2],
        INTVAL (operands[4]) & SFPIADD_MOD1_ARG_2SCOMP_LREG_DST
	? nullptr : &operands[3]);
  }
  [(set_attr "type" "tensix")
   (set (attr "xtt_dynamic_bug") (symbol_ref "xtt_dynamic_bug (XTT_DYNAMIC_BUG_BH | XTT_DYNAMIC_BUG_QSR)"))])

(define_insn "rvtt_sfpiadd_v_nv"
  [(unspec_volatile:XTT32SI [
     (match_operand:XTT32SI 0 "cstlreg_operand" "xc")
     (match_operand:XTT32SI 1 "reg_or_cstlreg_operand" "xrxc")
     (match_operand:SI      2 "const_int_operand" "n")
     ] UNSPECV_SFPIADD)]
  "TARGET_XTT_TENSIX_QSR"
  "SFPIADD\t%0, %1, 0, %2"
  [(set_attr "type" "tensix")
   (set (attr "xtt_dynamic_bug") (symbol_ref "xtt_dynamic_bug (XTT_DYNAMIC_BUG_BH | XTT_DYNAMIC_BUG_QSR)"))])

(define_expand "rvtt_sfpiadd_i"
  [(set (match_operand:XTT32SI 0 "register_operand")
        (unspec_volatile:XTT32SI [
          (match_operand:SI 1 "reg_or_0_operand")
          (match_operand:XTT32SI 2 "reg_or_cstlreg_operand")
          (match_operand:SI 3 "reg_or_const_int_operand")
          (match_operand:SI 4 "reg_or_0_operand")
          (match_operand:SI 5 "const_int_operand")
          (match_operand:SI 6 "const_int_operand")
          ] UNSPECV_SFPIADD))]
  "TARGET_XTT_TENSIX"
{
  emit_insn (gen_rvtt_sfpiadd_i_lv
    (operands[0], operands[1], rvtt_gen_rtx_noval (XTT32SImode), operands[2],
     operands[3], operands[4], operands[5], operands[6]));
  DONE;
})

(define_expand "rvtt_sfpiadd_i_lv"
  [(set (match_operand:XTT32SI 0 "register_operand")
        (unspec_volatile:XTT32SI [
          (match_operand:SI 1 "reg_or_0_operand")
          (match_operand:XTT32SI 2 "reg_or_cstlreg_or_noval_operand")
          (match_operand:XTT32SI 3 "reg_or_cstlreg_operand")
          (match_operand:SI 4 "reg_or_const_int_operand")
          (match_operand:SI 5 "reg_or_0_operand")
          (match_operand:SI 6 "const_int_operand")
          (match_operand:SI 7 "const_int_operand")
          ] UNSPECV_SFPIADD))]
  "TARGET_XTT_TENSIX"
{
  operands[7] = GEN_INT (INTVAL (operands[7]) | SFPIADD_MOD1_ARG_IMM);
  auto mem = CONST0_RTX (SImode);
  auto opc = CONST0_RTX (SImode);
  auto enc = CONST0_RTX (SImode);
  auto insn = CONST0_RTX (SImode);
  auto imm = operands[4];
  if (!CONST_INT_P (imm))
    {
      mem = gen_rtx_MEM (SImode, operands[1]);
      int op
        = TARGET_XTT_TENSIX_WH 	? TT_OP_WH_SFPIADD (0, 0, 0, INTVAL (operands[7]))
	: TARGET_XTT_TENSIX_BH 	? TT_OP_BH_SFPIADD (0, 0, 0, INTVAL (operands[7]))
	: TARGET_XTT_TENSIX_QSR	? TT_OP_QSR_SFPIADD (0, 0, 0, INTVAL (operands[7]))
        : (gcc_unreachable (), 0);
      opc = GEN_INT (op);
      enc = GEN_INT (rvtt_synth (UINTVAL (operands[6])).src_shift (8).dst_shift (4));
      imm = rvtt_gen_rtx_noval (GET_MODE (imm));
      insn = operands[5];
    }

  emit_insn (gen_rvtt_sfpiadd_i_lv_int
    (operands[0], mem, opc, enc, insn,
     operands[3], operands[2], imm, operands[7]));
  DONE;
})

(define_insn_and_split "rvtt_sfpiadd_i_lv_int"
  [(set (match_operand:XTT32SI 0 "register_operand" "=xr,xr,xr,xr")
        (unspec_volatile:XTT32SI [
          (match_operand:SI    1 "mem_or_0_operand"  "J,J,m,m")
          (match_operand:SI    2 "const_int_operand" "J,J,n,n") ;; opcode
          (match_operand:SI    3 "const_int_operand" "J,J,n,n") ;; id, src & dst shifts
          (match_operand:SI    4 "reg_or_0_operand"  "J,J,r,r") ;; insn
          (match_operand:XTT32SI 5 "reg_or_cstlreg_operand" "xrxc,xrxc,xrxc,xrxc") ;; src
          (match_operand:XTT32SI 6 "reg_or_cstlreg_or_noval_operand" "xn,0,xn,0") ;; lv
          (match_operand:SI    7 "const_int_or_noval_operand" "n,n,xn,xn") ;; imm
          (match_operand:SI    8 "const_int_operand" "n,n,n,n")
          ] UNSPECV_SFPIADD))
   (clobber (match_scratch:SI  9 "=X,X,&r,&r"))]
  "TARGET_XTT_TENSIX"
  {
    return rvtt_synth::pattern (which_alternative >> 1,
      which_alternative & 1
      ? "SFPIADD\t%0, %5, %7, %8\t# LV:%6"
      : "SFPIADD\t%0, %5, %7, %8",
      operands, true, 9);
  }
  "&& TARGET_XTT_TENSIX_QSR
   && find_reg_note (insn, REG_UNUSED, operands[0])"
  [(const_int 0)]
  {
    rtx opcode = operands[2];
    if (operands[1] != CONST0_RTX (SImode))
      {
        // Bake L8 as the dst reg here
        auto enc = rvtt_synth (INTVAL (operands[3]));
        opcode = GEN_INT (INTVAL (opcode) | (0x8 << enc.dst_shift ()));
      }
    emit_insn (gen_rvtt_sfpiadd_i_nv
      (operands[1], opcode, operands[3], operands[4], operands[5],
       operands[7], operands[8]));
    DONE;
  }
  [(set_attr "type" "tensix")])

(define_insn "rvtt_sfpiadd_i_nv"
  [(unspec_volatile:XTT32SI [
     (match_operand:SI    0 "mem_or_0_operand"  "J,m")
     (match_operand:SI    1 "const_int_operand" "J,n") ;; opcode
     (match_operand:SI    2 "const_int_operand" "J,n") ;; id, src & dst shifts
     (match_operand:SI    3 "reg_or_0_operand"  "J,r") ;; insn
     (match_operand:XTT32SI 4 "reg_or_cstlreg_operand" "xrxc,xrxc") ;; src
     (match_operand:SI    5 "const_int_or_noval_operand" "n,xn") ;; imm
     (match_operand:SI    6 "const_int_operand" "n,n")
     ] UNSPECV_SFPIADD)
   (clobber (match_scratch:SI  7 "=X,&r"))]
  "TARGET_XTT_TENSIX_QSR"
  {
    return rvtt_synth::pattern (which_alternative,
      "SFPIADD\tL8, %4, %5, %6",
      operands, false, 7);
  }
  [(set_attr "type" "tensix")])

(define_int_iterator rvtt_unary_op [
  UNSPECV_SFPMOV
  UNSPECV_SFPEXEXP
  UNSPECV_SFPEXMAN
  UNSPECV_SFPABS
  UNSPECV_SFPLZ
  ])
(define_int_attr rvtt_unary_name [
  (UNSPECV_SFPMOV "mov")
  (UNSPECV_SFPEXEXP "exexp")
  (UNSPECV_SFPEXMAN "exman")
  (UNSPECV_SFPABS "abs")
  (UNSPECV_SFPLZ "lz")
  ])
(define_int_attr rvtt_unary_insn [
  (UNSPECV_SFPMOV "MOV")
  (UNSPECV_SFPEXEXP "EXEXP")
  (UNSPECV_SFPEXMAN "EXMAN")
  (UNSPECV_SFPABS "ABS")
  (UNSPECV_SFPLZ "LZ")
  ])

(define_expand "rvtt_sfp<rvtt_unary_name>"
  [(set (match_operand:XTT32SI 0 "register_operand")
        (unspec_volatile:XTT32SI [
	  (match_operand:XTT32SI 1 "register_operand")
          (match_operand:SI    2 "const_int_operand")
	  ] rvtt_unary_op))]
  "TARGET_XTT_TENSIX"
{
  emit_insn (gen_rvtt_sfp<rvtt_unary_name>_lv (
      operands[0], rvtt_gen_rtx_noval (XTT32SImode), operands[1], operands[2]));
  DONE;
})

(define_insn_and_split "rvtt_sfp<rvtt_unary_name>_lv"
  [(set (match_operand:XTT32SI 0 "register_operand" "=xr,xr")
        (unspec_volatile:XTT32SI [
	  (match_operand:XTT32SI 1 "reg_or_cstlreg_or_noval_operand" "xn,0")
          (match_operand:XTT32SI 2 "reg_or_cstlreg_operand" "xrxc,xrxc")
          (match_operand:SI    3 "const_int_operand" "n,n")
	  ] rvtt_unary_op))]
  "TARGET_XTT_TENSIX"
  "@
   SFP<rvtt_unary_insn>\t%0, %2, %3
   SFP<rvtt_unary_insn>\t%0, %2, %3\t# LV:%1"
  "&& TARGET_XTT_TENSIX_QSR
   && <rvtt_unary_op> == UNSPECV_SFPLZ
   && find_reg_note (insn, REG_UNUSED, operands[0])"
  [(const_int 0)]
  {
    emit_insn (gen_rvtt_sfp<rvtt_unary_name>_nv
      (operands[2], operands[3]));
    DONE;
  }
  [(set_attr "type" "tensix")])

(define_insn "rvtt_sfp<rvtt_unary_name>_nv"
  [(unspec_volatile:XTT32SI [
     (match_operand:XTT32SI 0 "reg_or_cstlreg_operand" "xrxc")
     (match_operand:SI    1 "const_int_operand" "n")
     ] rvtt_unary_op)]
  "TARGET_XTT_TENSIX_QSR && <rvtt_unary_op> == UNSPECV_SFPLZ"
  "SFP<rvtt_unary_insn>\tL8, %0, %1"
  [(set_attr "type" "tensix")])

(define_int_iterator rvtt_set_op [
  UNSPECV_SFPSETEXP
  UNSPECV_SFPSETMAN
  UNSPECV_SFPSETSGN
  ])
(define_int_attr rvtt_set_name [
  (UNSPECV_SFPSETEXP "exp")
  (UNSPECV_SFPSETMAN "man")
  (UNSPECV_SFPSETSGN "sgn")
  ])
(define_int_attr rvtt_set_insn [
  (UNSPECV_SFPSETEXP "EXP")
  (UNSPECV_SFPSETMAN "MAN")
  (UNSPECV_SFPSETSGN "SGN")
  ])

(define_expand "rvtt_sfpset<rvtt_set_name>_v"
  [(set (match_operand:XTT32SI 0 "register_operand")
        (unspec_volatile:XTT32SI [
          (match_operand:XTT32SI 1 "reg_or_cstlreg_operand")
	  (match_operand:XTT32SI 2 "reg_or_cstlreg_operand")
          (match_operand:SI      3 "const_int_operand")
	  ] rvtt_set_op))]
  "TARGET_XTT_TENSIX"
  {
    emit_insn (gen_rvtt_sfpset<rvtt_set_name>_v_lv
      (operands[0], rvtt_gen_rtx_noval (XTT32SImode),
       operands[1], operands[2], operands[3]));
    DONE;
  })

(define_insn_and_rewrite "rvtt_sfpset<rvtt_set_name>_v_lv"
  [(set (match_operand:XTT32SI 0 "register_operand" "=xr,xr,xr")
        (unspec_volatile:XTT32SI [
          (match_operand:XTT32SI 1 "reg_or_cstlreg_or_noval_or_omit_operand" "xn,xo,xrxc")
          (match_operand:XTT32SI 2 "reg_or_cstlreg_operand" "xrxc,xrxc,xrxc")
	  (match_operand:XTT32SI 3 "reg_or_cstlreg_operand" "0,0,0")
          (match_operand:SI      4 "const_int_operand" "n,n,n")
	  ] rvtt_set_op))]
  "TARGET_XTT_TENSIX"
  "@
   SFPSET<rvtt_set_insn>\t%0, %2, 0, %4
   SFPSET<rvtt_set_insn>\t%0, %2, 0, %4\t# LV:%3
   #"
  "&& !noval_or_omit_operand (operands[1], GET_MODE (operands[1]))"
  {
    rvtt_merge_lv_src (&operands[1], &operands[3]);
  }
  [(set_attr "type" "tensix")])

(define_expand "rvtt_sfpset<rvtt_set_name>_i"
  [(set (match_operand:XTT32SI 0 "register_operand")
        (unspec_volatile:XTT32SI [
	  (match_operand:SI    1 "reg_or_0_operand")
          (match_operand:XTT32SI 2 "reg_or_cstlreg_operand")
          (match_operand:SI    3 "reg_or_const_int_operand")
          (match_operand:SI    4 "reg_or_0_operand")
          (match_operand:SI    5 "const_int_operand")
          (match_operand:SI    6 "const_int_operand")
	  ] rvtt_set_op))]
  "TARGET_XTT_TENSIX"
{
  emit_insn (gen_rvtt_sfpset<rvtt_set_name>_i_lv
    (operands[0], operands[1], rvtt_gen_rtx_noval (XTT32SImode),
     operands[2], operands[3], operands[4], operands[5], operands[6]));
  DONE;
})

(define_expand "rvtt_sfpset<rvtt_set_name>_i_lv"
  [(set (match_operand:XTT32SI 0 "register_operand")
        (unspec_volatile:XTT32SI [
	  (match_operand:SI    1 "reg_or_0_operand")
          (match_operand:XTT32SI 2 "reg_or_cstlreg_or_noval_operand")
          (match_operand:XTT32SI 3 "reg_or_cstlreg_operand")
          (match_operand:SI    4 "reg_or_const_int_operand")
          (match_operand:SI    5 "reg_or_0_operand")
          (match_operand:SI    6 "const_int_operand")
          (match_operand:SI    7 "const_int_operand")
	  ] rvtt_set_op))]
  "TARGET_XTT_TENSIX"
{
  operands[7] = GEN_INT (INTVAL (operands[7]) | SFPSET<rvtt_set_insn>_MOD1_IMM);
  auto mem = CONST0_RTX (SImode);
  auto opc = CONST0_RTX (SImode);
  auto enc = CONST0_RTX (SImode);
  auto insn = CONST0_RTX (SImode);
  auto imm = operands[4];
  if (!CONST_INT_P (imm))
    {
      // This should have been expanded at by synth-expand
      gcc_assert (<rvtt_set_op> != UNSPECV_SFPSETMAN);

      mem = gen_rtx_MEM (SImode, operands[1]);
      int op
        = TARGET_XTT_TENSIX_WH  ? TT_OP_WH_SFPSET<rvtt_set_insn> (0, 0, 0, INTVAL (operands[7]))
        : TARGET_XTT_TENSIX_BH  ? TT_OP_BH_SFPSET<rvtt_set_insn> (0, 0, 0, INTVAL (operands[7]))
        : TARGET_XTT_TENSIX_QSR ? TT_OP_QSR_SFPSET<rvtt_set_insn> (0, 0, 0, INTVAL (operands[7]))
        : (gcc_unreachable (), 0);
      opc = GEN_INT (op);
      enc = GEN_INT (rvtt_synth (UINTVAL (operands[6])).src_shift (8).dst_shift (4));
      imm = rvtt_gen_rtx_noval (GET_MODE (imm));
      insn = operands[5];
    }

  emit_insn (gen_rvtt_sfpset<rvtt_set_name>_i_lv_int
    (operands[0], mem, opc, enc, insn,
     operands[3], operands[2], imm, operands[7]));
  DONE;
})

(define_insn "rvtt_sfpset<rvtt_set_name>_i_lv_int"
  [(set (match_operand:XTT32SI 0 "register_operand" "=xr,xr,xr,xr")
        (unspec_volatile:XTT32SI [
          (match_operand:SI    1 "mem_or_0_operand"  "J,J,m,m")
          (match_operand:SI    2 "const_int_operand" "J,J,n,n") ;; opcode
          (match_operand:SI    3 "const_int_operand" "J,J,n,n") ;; id, src & dst shifts
          (match_operand:SI    4 "reg_or_0_operand"  "J,J,r,r") ;; insn
          (match_operand:XTT32SI 5 "reg_or_cstlreg_operand" "xrxc,xrxc,xrxc,xrxc") ;; src
          (match_operand:XTT32SI 6 "reg_or_cstlreg_or_noval_operand" "xn,0,xn,0") ;; lv
          (match_operand:SI    7 "const_int_or_noval_operand" "n,n,xn,xn") ;; imm
          (match_operand:SI    8 "const_int_operand" "n,n,n,n")
          ] rvtt_set_op))
   (clobber (match_scratch:SI  9 "=X,X,&r,&r"))]
  "TARGET_XTT_TENSIX"
  {
    return rvtt_synth::pattern (which_alternative >> 1,
      which_alternative & 1
      ? "SFPSET<rvtt_set_insn>\t%0, %5, %7, %8\t# LV:%6"
      : "SFPSET<rvtt_set_insn>\t%0, %5, %7, %8",
      operands, true, 9);
  }
  [(set_attr "type" "tensix")])

(define_int_iterator rvtt_logical_op [
  UNSPECV_SFPAND
  UNSPECV_SFPOR
  UNSPECV_SFPXOR
  ])
(define_int_attr rvtt_logical_name [
  (UNSPECV_SFPAND "and")
  (UNSPECV_SFPOR "or")
  (UNSPECV_SFPXOR "xor")
  ])
(define_int_attr rvtt_logical_insn [
  (UNSPECV_SFPAND "AND")
  (UNSPECV_SFPOR "OR")
  (UNSPECV_SFPXOR "XOR")
  ])

(define_expand "rvtt_sfp<rvtt_logical_name>"
  [(set (match_operand:XTT32SI 0 "register_operand")
        (unspec_volatile:XTT32SI [
	  (match_operand:XTT32SI 1 "reg_or_cstlreg_operand")
          (match_operand:XTT32SI 2 "reg_or_cstlreg_operand")
	  ] rvtt_logical_op))]
  "TARGET_XTT_TENSIX"
  {
    emit_insn (gen_rvtt_sfp<rvtt_logical_name>_lv
      (operands[0], rvtt_gen_rtx_noval (XTT32SImode),
       operands[1], operands[2]));
    DONE;
  })

(define_expand "rvtt_sfp<rvtt_logical_name>_lv"
  [(set (match_operand:XTT32SI 0 "register_operand")
        (unspec_volatile:XTT32SI [
	  (match_operand:XTT32SI 1 "reg_or_cstlreg_or_noval_operand")
	  (match_operand:XTT32SI 2 "reg_or_cstlreg_operand")
          (match_operand:XTT32SI 3 "reg_or_cstlreg_operand")
	  ] rvtt_logical_op))]
  "TARGET_XTT_TENSIX"
  {
    rtx insn = nullptr;
    if (<rvtt_logical_op> == UNSPECV_SFPXOR
        || TARGET_XTT_TENSIX_WH || TARGET_XTT_TENSIX_QSR)
      insn = gen_rvtt_sfp<rvtt_logical_name>_lv_2op
        (operands[0], operands[1], operands[2], operands[3]);
    else
      {
        // There is no XOR use of VB, but this needs to be compilable
	constexpr unsigned SFPXOR_MOD1_USE_VB __attribute__((unused)) = ~0u;
        insn = gen_rvtt_sfp<rvtt_logical_name>_lv_bh
          (operands[0], operands[1], operands[2], operands[3],
           GEN_INT (SFP<rvtt_logical_insn>_MOD1_USE_VB));
      }	   
    emit_insn (insn);
    DONE;
  })

(define_insn_and_rewrite "rvtt_sfp<rvtt_logical_name>_lv_2op"
  [(set (match_operand:XTT32SI 0 "register_operand" "=xr,xr,xr")
        (unspec_volatile:XTT32SI [
	  (match_operand:XTT32SI 1 "reg_or_cstlreg_or_noval_or_omit_operand" "xn,xo,xrxc")
	  (match_operand:XTT32SI 2 "reg_or_cstlreg_operand" "0,0,0")
          (match_operand:XTT32SI 3 "reg_or_cstlreg_operand" "xrxc,xrxc,xrxc")
	  ] rvtt_logical_op))]
  "<rvtt_logical_op> == UNSPECV_SFPXOR
   || TARGET_XTT_TENSIX_WH || TARGET_XTT_TENSIX_QSR"
  "@
   SFP<rvtt_logical_insn>\t%0, %3
   SFP<rvtt_logical_insn>\t%0, %3\t# LV:%2
   #"
  "&& !noval_or_omit_operand (operands[1], GET_MODE (operands[1]))"
  {
    rvtt_merge_lv_src (&operands[1], &operands[2], &operands[3]);
  }
  [(set_attr "type" "tensix")])

(define_insn "rvtt_sfp<rvtt_logical_name>_lv_bh"
  [(set (match_operand:XTT32SI 0 "register_operand" "=xr,xr")
        (unspec_volatile:XTT32SI [
	  (match_operand:XTT32SI 1 "reg_or_cstlreg_or_noval_operand" "xn,0")
	  (match_operand:XTT32SI 2 "reg_or_cstlreg_operand" "xrxc,xrxc")
          (match_operand:XTT32SI 3 "reg_or_cstlreg_operand" "xrxc,xrxc")
          (match_operand:SI      4 "const_int_operand"  "n,n")
	  ] rvtt_logical_op))]
  "<rvtt_logical_op> != UNSPECV_SFPXOR && TARGET_XTT_TENSIX_BH"
  "@
   SFP<rvtt_logical_insn>\t%0, %2, %3, %4
   SFP<rvtt_logical_insn>\t%0, %2, %3, %4\t# LV:%1"
  [(set_attr "type" "tensix")
   (set (attr "xtt_dynamic_bug") (symbol_ref "xtt_dynamic_bug (XTT_DYNAMIC_BUG_BH)"))])

(define_expand "rvtt_sfpnot"
  [(set (match_operand:XTT32SI 0 "register_operand" "=xr")
        (unspec_volatile:XTT32SI [
	  (match_operand:XTT32SI 1 "reg_or_cstlreg_operand"  "xrxc")
	  ] UNSPECV_SFPNOT))]
  "TARGET_XTT_TENSIX"
  {
    emit_insn (gen_rvtt_sfpnot_lv (operands[0], rvtt_gen_rtx_noval (XTT32SImode), operands[1]));
    DONE;
  })

(define_insn "rvtt_sfpnot_lv"
  [(set (match_operand:XTT32SI 0 "register_operand" "=xr,xr")
        (unspec_volatile:XTT32SI [
	  (match_operand:XTT32SI 1 "reg_or_cstlreg_or_noval_operand"  "xn,0")
          (match_operand:XTT32SI 2 "reg_or_cstlreg_operand"  "xrxc,xrxc")
	  ] UNSPECV_SFPNOT))]
  "TARGET_XTT_TENSIX"
  "@
   SFPNOT\t%0, %2
   SFPNOT\t%0, %2\t# LV:%1"
  [(set_attr "type" "tensix")])

(define_expand "rvtt_sfpshft_v"
  [(set (match_operand:XTT32SI 0 "register_operand")
        (unspec_volatile:XTT32SI [
	  (match_operand:XTT32SI 1 "reg_or_cstlreg_operand")
          (match_operand:XTT32SI 2 "reg_or_cstlreg_operand")
          (match_operand:SI    3 "const_int_operand")
	  ] UNSPECV_SFPSHFT))]
  "TARGET_XTT_TENSIX"
  {
    emit_insn (gen_rvtt_sfpshft_v_lv
      (operands[0], rvtt_gen_rtx_noval (XTT32SImode),
       operands[1], operands[2], operands[3]));
    DONE;
  })

(define_expand "rvtt_sfpshft_v_lv"
  [(set (match_operand:XTT32SI 0 "register_operand")
        (unspec_volatile:XTT32SI [
	  (match_operand:XTT32SI 1 "reg_or_cstlreg_or_noval_operand")
	  (match_operand:XTT32SI 2 "reg_or_cstlreg_operand")
          (match_operand:XTT32SI 3 "reg_or_cstlreg_operand")
          (match_operand:SI    4 "const_int_operand")
	  ] UNSPECV_SFPSHFT))]
  "TARGET_XTT_TENSIX"
  {
    emit_insn (gen_rvtt_sfpshft_v_lv_int
        (operands[0], operands[1], operands[2], operands[3], operands[4]));
    DONE;
  })

(define_insn_and_rewrite "rvtt_sfpshft_v_lv_int"
  [(set (match_operand:XTT32SI 0 "register_operand" "=xr,xr,xr")
        (unspec_volatile:XTT32SI [
	  (match_operand:XTT32SI 1 "reg_or_cstlreg_or_noval_or_omit_operand"  "xn,xo,xrxc")
	  (match_operand:XTT32SI 2 "reg_or_cstlreg_operand"  "0,0,0")
          (match_operand:XTT32SI 3 "reg_or_cstlreg_operand"  "xrxc,xrxc,xrxc")
          (match_operand:SI    4 "const_int_operand"  "n,n,n")
	  ] UNSPECV_SFPSHFT))]
  "TARGET_XTT_TENSIX"
  "@
   SFPSHFT\t%0, %3, 0, %4
   SFPSHFT\t%0, %3, 0, %4\t# LV:%2
   #"
  "&& !noval_or_omit_operand (operands[1], GET_MODE (operands[1]))"
  {
    rvtt_merge_lv_src (&operands[1], &operands[2]);
  }
  [(set_attr "type" "tensix")
   (set (attr "xtt_dynamic_bug") (symbol_ref "xtt_dynamic_bug (XTT_DYNAMIC_BUG_BH | XTT_DYNAMIC_BUG_QSR)"))])

(define_expand "rvtt_sfpshft_i"
  [(set (match_operand:XTT32SI 0 "register_operand")
        (unspec_volatile:XTT32SI [
	  (match_operand:SI    1 "reg_or_0_operand")
          (match_operand:XTT32SI 2 "reg_or_cstlreg_operand")
          (match_operand:SI    3 "reg_or_const_int_operand")
          (match_operand:SI    4 "reg_or_0_operand")
          (match_operand:SI    5 "const_int_operand")
          (match_operand:SI    6 "const_int_operand")
	  ] UNSPECV_SFPSHFT))]
  "TARGET_XTT_TENSIX"
  {
    emit_insn (gen_rvtt_sfpshft_i_lv
        (operands[0], operands[1], rvtt_gen_rtx_noval (XTT32SImode),
         operands[2], operands[3], operands[4], operands[5], operands[6]));
    DONE;
  })

(define_expand "rvtt_sfpshft_i_lv"
  [(set (match_operand:XTT32SI 0 "register_operand")
        (unspec_volatile:XTT32SI [
	  (match_operand:SI    1 "reg_or_0_operand")
          (match_operand:XTT32SI 2 "reg_or_cstlreg_or_noval_operand")
          (match_operand:XTT32SI 3 "reg_or_cstlreg_operand")
          (match_operand:SI    4 "reg_or_const_int_operand")
          (match_operand:SI    5 "reg_or_0_operand")
          (match_operand:SI    6 "const_int_operand")
          (match_operand:SI    7 "const_int_operand")
	  ] UNSPECV_SFPSHFT))]
  "TARGET_XTT_TENSIX"
{
  unsigned mod = SFPSHFT_MOD1_SHFT_IMM
      | (TARGET_XTT_TENSIX_BH_QSR ? SFPSHFT_MOD1_SRC_LREG_C : 0);
  operands[7] = GEN_INT (INTVAL (operands[7]) | mod);

  auto mem = CONST0_RTX (SImode);
  auto opc = CONST0_RTX (SImode);
  auto enc = CONST0_RTX (SImode);
  auto insn = CONST0_RTX (SImode);
  auto imm = operands[4];
  if (!CONST_INT_P (imm))
    {
      mem = gen_rtx_MEM (SImode, operands[1]);
      int op
        = TARGET_XTT_TENSIX_WH 	? TT_OP_WH_SFPSHFT (0, 0, 0, INTVAL (operands[7]))
        : TARGET_XTT_TENSIX_BH  ? TT_OP_BH_SFPSHFT (0, 0, 0, INTVAL (operands[7]))
        : TARGET_XTT_TENSIX_QSR ? TT_OP_QSR_SFPSHFT (0, 0, 0, INTVAL (operands[7]))
        : (gcc_unreachable (), 0);
      opc = GEN_INT (op);
      enc = GEN_INT (rvtt_synth (UINTVAL (operands[6]))
                    .src_shift (TARGET_XTT_TENSIX_WH ? 4 : TARGET_XTT_TENSIX_BH_QSR ? 8 : 0)
		    .dst_shift (4));
      imm = rvtt_gen_rtx_noval (GET_MODE (imm));
      insn = operands[5];
    }

  emit_insn (gen_rvtt_sfpshft_i_lv_int
    (operands[0], mem, opc, enc, insn,
     operands[3], operands[2], imm,
     operands[7]));
  DONE;
})

(define_expand "rvtt_sfpshft_i_lv_int"
  [(parallel [
     (set (match_operand:XTT32SI 0 "register_operand")
          (unspec_volatile:XTT32SI [
            (match_operand:SI    1 "mem_or_0_operand")
            (match_operand:SI    2 "const_int_operand") ;; opcode
            (match_operand:SI    3 "const_int_operand") ;; id, src & dst shifts
            (match_operand:SI    4 "reg_or_0_operand")  ;; insn
            (match_operand:XTT32SI 5 "reg_or_cstlreg_operand") ;; src
            (match_operand:XTT32SI 6 "reg_or_cstlreg_or_noval_operand") ;; lv
            (match_operand:SI    7 "const_int_or_noval_operand") ;; imm
            (match_operand:SI    8 "const_int_operand")
            ] UNSPECV_SFPSHFT))
       (clobber (match_scratch:SI  9))])]
  "TARGET_XTT_TENSIX")

(define_insn "*rvtt_sfpshft_i_lv_3op"
  [(set (match_operand:XTT32SI 0 "register_operand" "=xr,xr,xr,xr")
        (unspec_volatile:XTT32SI [
          (match_operand:SI    1 "mem_or_0_operand" "J,J,m,m")
          (match_operand:SI    2 "const_int_operand" "J,J,n,n") ;; opcode
          (match_operand:SI    3 "const_int_operand" "J,J,n,n") ;; id, src & dst shifts
          (match_operand:SI    4 "reg_or_0_operand" "J,J,r,r")  ;; insn
          (match_operand:XTT32SI 5 "reg_or_cstlreg_operand" "xrxc,xrxc,xrxc,xrxc") ;; src
          (match_operand:XTT32SI 6 "reg_or_cstlreg_or_noval_operand" "xn,0,xn,0") ;; lv
          (match_operand:SI    7 "const_int_or_noval_operand" "n,n,xn,xn") ;; imm
          (match_operand:SI    8 "const_int_operand" "n,n,n,n")
	  ] UNSPECV_SFPSHFT))
   (clobber (match_scratch:SI  9 "=X,X,&r,&r"))]
  "TARGET_XTT_TENSIX_BH_QSR"
  {
    return rvtt_synth::pattern (which_alternative >> 1,
      which_alternative & 1
      ? "SFPSHFT\t%0, %5, %7, %8\t# LV:%6"
      : "SFPSHFT\t%0, %5, %7, %8",
      operands, true, 9);
  }
  [(set_attr "type" "tensix")])

(define_insn_and_rewrite "*rvtt_sfpshft_i_lv_2op"
  [(set (match_operand:XTT32SI 0 "register_operand" "=xr,xr,xr,xr,xr,xr")
        (unspec_volatile:XTT32SI [
          (match_operand:SI    1 "mem_or_0_operand"  "J,J,J,m,m,m")
          (match_operand:SI    2 "const_int_operand" "J,J,J,n,n,n") ;; opcode
          (match_operand:SI    3 "const_int_operand" "J,J,J,n,n,n") ;; id, src & dst shifts
          (match_operand:SI    4 "reg_or_0_operand"  "J,J,J,r,r,r") ;; insn
          (match_operand:XTT32SI 5 "reg_or_cstlreg_operand" "0,0,0,0,0,0") ;; src
          (match_operand:XTT32SI 6 "reg_or_cstlreg_or_noval_or_omit_operand" "xn,xo,xrxc,xn,xo,xrxc") ;; lv
          (match_operand:SI    7 "const_int_or_noval_operand" "n,n,n,xn,xn,xn") ;; imm
          (match_operand:SI    8 "const_int_operand" "n,n,n,n,n,n")
	  ] UNSPECV_SFPSHFT))
   (clobber (match_scratch:SI  9 "=X,X,X,&r,&r,&r"))]
  "TARGET_XTT_TENSIX_WH"
  {
    int which = which_alternative;
    bool dyn = which >= 3;
    if (dyn)
      which -= 3;
    return rvtt_synth::pattern (dyn,
      which == 0 ? "SFPSHFT\t%0, L0, %7, %8" :
      which == 1 ? "SFPSHFT\t%0, L0, %7, %8\t# LV:%5" :
      "#",
      operands, true, 9);
  }
  "&& !noval_or_omit_operand (operands[6], GET_MODE (operands[6]))"
  {
    rvtt_merge_lv_src (&operands[6], &operands[5]);
  }
  [(set_attr "type" "tensix")])

(define_expand "rvtt_sfpcast"
  [(set (match_operand:XTT32SI 0 "register_operand" "=xr")
        (unspec_volatile:XTT32SI [
	  (match_operand:XTT32SI 1 "reg_or_cstlreg_operand"  "xrxc")
          (match_operand:SI    2 "const_int_operand" "n")
	  ] UNSPECV_SFPCAST))]
  "TARGET_XTT_TENSIX"
  {
    emit_insn (gen_rvtt_sfpcast_lv (operands[0],
      rvtt_gen_rtx_noval (XTT32SImode), operands[1], operands[2]));
    DONE;
  })

(define_insn "rvtt_sfpcast_lv"
  [(set (match_operand:XTT32SI 0 "register_operand" "=xr,xr")
        (unspec_volatile:XTT32SI [
	  (match_operand:XTT32SI 1 "reg_or_cstlreg_or_noval_operand"  "xn,0")
          (match_operand:XTT32SI 2 "reg_or_cstlreg_operand"  "xrxc,xrxc")
          (match_operand:SI    3 "const_int_operand" "n,n")
	  ] UNSPECV_SFPCAST))]
  "TARGET_XTT_TENSIX"
  "@
   SFPCAST\t%0, %2, %3
   SFPCAST\t%0, %2, %3\t# LV:%1"
  [(set_attr "type" "tensix")])

(define_expand "rvtt_sfpdivp2"
  [(set (match_operand:XTT32SI 0 "register_operand")
        (unspec_volatile:XTT32SI [
	  (match_operand:SI    1 "reg_or_0_operand")
          (match_operand:XTT32SI 2 "reg_or_cstlreg_operand")
          (match_operand:SI    3 "reg_or_const_int_operand")
          (match_operand:SI    4 "reg_or_0_operand")
          (match_operand:SI    5 "const_int_operand")
          (match_operand:SI    6 "const_int_operand")
	  ] UNSPECV_SFPDIVP2))]
  "TARGET_XTT_TENSIX"
{
  emit_insn (gen_rvtt_sfpdivp2_lv
    (operands[0], operands[1], rvtt_gen_rtx_noval (XTT32SImode),
     operands[2], operands[3], operands[4], operands[5], operands[6]));
  DONE;
})

(define_expand "rvtt_sfpdivp2_lv"
  [(set (match_operand:XTT32SI 0 "register_operand")
        (unspec_volatile:XTT32SI [
	  (match_operand:SI    1 "reg_or_0_operand")
          (match_operand:XTT32SI 2 "reg_or_cstlreg_or_noval_operand")
          (match_operand:XTT32SI 3 "reg_or_cstlreg_operand")
          (match_operand:SI    4 "reg_or_const_int_operand")
          (match_operand:SI    5 "reg_or_0_operand")
          (match_operand:SI    6 "const_int_operand")
          (match_operand:SI    7 "const_int_operand")
	  ] UNSPECV_SFPDIVP2))]
  "TARGET_XTT_TENSIX"
{
  auto mem = CONST0_RTX (SImode);
  auto opc = CONST0_RTX (SImode);
  auto enc = CONST0_RTX (SImode);
  auto insn = CONST0_RTX (SImode);
  auto imm = operands[4];
  if (!CONST_INT_P (imm))
    {
      mem = gen_rtx_MEM (SImode, operands[1]);
      int op
        = TARGET_XTT_TENSIX_WH 	? TT_OP_BH_SFPDIVP2 (0, 0, 0, INTVAL (operands[7]))
        : TARGET_XTT_TENSIX_BH  ? TT_OP_BH_SFPDIVP2 (0, 0, 0, INTVAL (operands[7]))
        : TARGET_XTT_TENSIX_QSR ? TT_OP_QSR_SFPDIVP2 (0, 0, 0, INTVAL (operands[7]))
        : (gcc_unreachable (), 0);
      opc = GEN_INT (op);
      enc = GEN_INT (rvtt_synth (UINTVAL (operands[6])).src_shift (8).dst_shift (4));
      imm = rvtt_gen_rtx_noval (GET_MODE (imm));
      insn = operands[5];
    }

  emit_insn (gen_rvtt_sfpdivp2_lv_int
    (operands[0], mem, opc, enc, insn,
     operands[3], operands[2], imm, operands[7]));
  DONE;
})

(define_insn "rvtt_sfpdivp2_lv_int"
  [(set (match_operand:XTT32SI 0 "register_operand" "=xr,xr,xr,xr")
        (unspec_volatile:XTT32SI [
          (match_operand:SI    1 "mem_or_0_operand"  "J,J,m,m")
          (match_operand:SI    2 "const_int_operand" "J,J,n,n") ;; opcode
          (match_operand:SI    3 "const_int_operand" "J,J,n,n") ;; id, src & dst shifts
          (match_operand:SI    4 "reg_or_0_operand"  "J,J,r,r") ;; insn
          (match_operand:XTT32SI 5 "reg_or_cstlreg_operand" "xrxc,xrxc,xrxc,xrxc") ;; src
          (match_operand:XTT32SI 6 "reg_or_cstlreg_or_noval_operand" "xn,0,xn,0") ;; lv
          (match_operand:SI    7 "const_int_or_noval_operand" "n,n,xn,xn") ;; imm
          (match_operand:SI    8 "const_int_operand" "n,n,n,n")
	  ] UNSPECV_SFPDIVP2))
   (clobber (match_scratch:SI  9 "=X,X,&r,&r"))]
  "TARGET_XTT_TENSIX"
  {
    return rvtt_synth::pattern (which_alternative >> 1,
      which_alternative & 1
      ? "SFPDIVP2\t%0, %5, %7, %8\t# LV:%6"
      : "SFPDIVP2\t%0, %5, %7, %8",
      operands, true, 9);
  }
  [(set_attr "type" "tensix")])

(define_expand "rvtt_sfpstochrnd"
  [(set (match_operand:XTT32SI 0 "register_operand")
        (unspec_volatile:XTT32SI [
          (match_operand:XTT32SI 1 "register_operand")
	  (match_operand:SI    2 "const_int_operand")
          (match_operand:SI    3 "const_int_operand")
	  ] UNSPECV_SFPSTOCHRND))]
  "TARGET_XTT_TENSIX"
{
  emit_insn (gen_rvtt_sfpstochrnd_lv
    (operands[0], rvtt_gen_rtx_noval (XTT32SImode),
     operands[1], operands[2], operands[3]));
  DONE;
})

(define_insn "rvtt_sfpstochrnd_lv"
  [(set (match_operand:XTT32SI 0 "register_operand" "=xr,xr")
        (unspec_volatile:XTT32SI [
	  (match_operand:XTT32SI 1 "reg_or_cstlreg_or_noval_operand" "xn,0")
          (match_operand:XTT32SI 2 "reg_or_cstlreg_operand"  "xrxc,xrxc")
          (match_operand:SI    3 "const_int_operand" "n,n")
          (match_operand:SI    4 "const_int_operand" "n,n")
	  ] UNSPECV_SFPSTOCHRND))]
  "TARGET_XTT_TENSIX"
  "@
   SFPSTOCHRND\t%0, L9, %2, 0, %3, %4
   SFPSTOCHRND\t%0, L9, %2, 0, %3, %4\t# LV:%1"
  [(set_attr "type" "tensix")])

(define_expand "rvtt_sfpstochrnd_descale_v"
  [(set (match_operand:XTT32SI 0 "register_operand")
        (unspec_volatile:XTT32SI [
          (match_operand:XTT32SI 1 "register_operand")
          (match_operand:XTT32SI 2 "reg_or_cstlreg_operand")
	  (match_operand:SI    3 "const_int_operand")
          (match_operand:SI    4 "const_int_operand")
	  ] UNSPECV_SFPSTOCHRND))]
  "TARGET_XTT_TENSIX"
{
  emit_insn (gen_rvtt_sfpstochrnd_descale_v_lv
    (operands[0], rvtt_gen_rtx_noval (XTT32SImode),
     operands[1], operands[2], operands[3], operands[4]));
  DONE;
})

(define_insn "rvtt_sfpstochrnd_descale_v_lv"
  [(set (match_operand:XTT32SI 0 "register_operand" "=xr,xr")
        (unspec_volatile:XTT32SI [
	  (match_operand:XTT32SI 1 "reg_or_cstlreg_or_noval_operand" "xn,0")
          (match_operand:XTT32SI 2 "reg_or_cstlreg_operand"  "xrxc,xrxc")
          (match_operand:XTT32SI 3 "reg_or_cstlreg_operand"  "xrxc,xrxc")
          (match_operand:SI    4 "const_int_operand" "n,n")
          (match_operand:SI    5 "const_int_operand" "n,n")
	  ] UNSPECV_SFPSTOCHRND))]
  "TARGET_XTT_TENSIX"
  "@
   SFPSTOCHRND\t%0, %3, %2, 0, %4, %5
   SFPSTOCHRND\t%0, %3, %2, 0, %4, %5\t# LV:%1"
  [(set_attr "type" "tensix")])

(define_expand "rvtt_sfpstochrnd_descale_i"
  [(set (match_operand:XTT32SI 0 "register_operand")
        (unspec_volatile:XTT32SI [
	  (match_operand:SI    1 "reg_or_0_operand")
          (match_operand:XTT32SI 2 "reg_or_cstlreg_operand")
          (match_operand:SI    3 "reg_or_const_int_operand")
          (match_operand:SI    4 "reg_or_0_operand")
          (match_operand:SI    5 "const_int_operand")
          (match_operand:SI    6 "const_int_operand")
          (match_operand:SI    7 "const_int_operand")
	  ] UNSPECV_SFPSTOCHRND))]
  "TARGET_XTT_TENSIX"
{
  emit_insn (gen_rvtt_sfpstochrnd_descale_i_lv
    (operands[0], operands[1], rvtt_gen_rtx_noval (XTT32SImode),
     operands[2], operands[3],
     operands[4], operands[5], operands[6], operands[7]));
  DONE;
})

(define_expand "rvtt_sfpstochrnd_descale_i_lv"
  [(set (match_operand:XTT32SI 0 "register_operand")
        (unspec_volatile:XTT32SI [
	  (match_operand:SI    1 "reg_or_0_operand")
          (match_operand:XTT32SI 2 "reg_or_cstlreg_or_noval_operand")
          (match_operand:XTT32SI 3 "reg_or_cstlreg_operand")
          (match_operand:SI    4 "reg_or_const_int_operand")
          (match_operand:SI    5 "reg_or_0_operand")
          (match_operand:SI    6 "const_int_operand")
          (match_operand:SI    7 "const_int_operand")
          (match_operand:SI    8 "const_int_operand")
	  ] UNSPECV_SFPSTOCHRND))]
  "TARGET_XTT_TENSIX"
{
  operands[7] = GEN_INT (INTVAL (operands[7]) | SFPSTOCHRND_MOD1_IMM8);

  auto mem = CONST0_RTX (SImode);
  auto opc = CONST0_RTX (SImode);
  auto enc = CONST0_RTX (SImode);
  auto insn = CONST0_RTX (SImode);
  auto imm = operands[4];
  if (!CONST_INT_P (imm))
    {
      mem = gen_rtx_MEM (SImode, operands[1]);
      int op
        = TARGET_XTT_TENSIX_WH  ? TT_OP_WH_SFP_STOCH_RND (INTVAL (operands[8]),
	             0, CREG_IDX_0, 0, 0, INTVAL (operands[7]))
        : TARGET_XTT_TENSIX_BH  ? TT_OP_BH_SFP_STOCH_RND (INTVAL (operands[8]),
	             0, CREG_IDX_0, 0, 0, INTVAL (operands[7]))
        : TARGET_XTT_TENSIX_QSR ? TT_OP_QSR_SFP_STOCH_RND (INTVAL (operands[8]),
	             0, CREG_IDX_0, 0, 0, INTVAL (operands[7]))
        : (gcc_unreachable (), 0);
      opc = GEN_INT (op);
      enc = GEN_INT (rvtt_synth (UINTVAL (operands[6])).src_shift (8).dst_shift (4));
      imm = rvtt_gen_rtx_noval (GET_MODE (imm));
      insn = operands[5];
    }

  emit_insn (gen_rvtt_sfpstochrnd_descale_i_lv_int
    (operands[0], mem, opc, enc, insn,
     operands[3], operands[2], imm, operands[7], operands[8]));
  DONE;
})

(define_insn "rvtt_sfpstochrnd_descale_i_lv_int"
  [(set (match_operand:XTT32SI 0 "register_operand" "=xr,xr,xr,xr")
        (unspec_volatile:XTT32SI [
          (match_operand:SI    1 "mem_or_0_operand"  "J,J,m,m")
          (match_operand:SI    2 "const_int_operand" "J,J,n,n") ;; opcode
          (match_operand:SI    3 "const_int_operand" "J,J,n,n") ;; id, src & dst shifts
          (match_operand:SI    4 "reg_or_0_operand"  "J,J,r,r") ;; insn
          (match_operand:XTT32SI 5 "reg_or_cstlreg_operand" "xrxc,xrxc,xrxc,xrxc") ;; src
          (match_operand:XTT32SI 6 "reg_or_cstlreg_or_noval_operand" "xn,0,xn,0") ;; lv
          (match_operand:SI    7 "const_int_or_noval_operand" "n,n,xn,xn") ;; imm
          (match_operand:SI    8 "const_int_operand" "n,n,n,n")
          (match_operand:SI    9 "const_int_operand" "n,n,n,n")
	  ] UNSPECV_SFPSTOCHRND))
   (clobber (match_scratch:SI 10 "=X,X,&r,&r"))]
  "TARGET_XTT_TENSIX"
  {
    return rvtt_synth::pattern (which_alternative >> 1,
      which_alternative & 1
      ? "SFPSTOCHRND\t%0, L9, %5, %7, %8, %9\t# LV:%6"
      : "SFPSTOCHRND\t%0, L9, %5, %7, %8, %9",
      operands, true, 10);
  }
  [(set_attr "type" "tensix")])

(define_expand "rvtt_sfpreadconfig"
  [(set (match_operand:XTT32SI 0 "register_operand" "=xr")
        (unspec_volatile:XTT32SI [
	  (match_operand:SI    1 "const_int_operand" "n")
	  ] UNSPECV_SFPCONFIG))]
  "TARGET_XTT_TENSIX_BH_QSR"
  {
    emit_insn (gen_rvtt_sfpreadconfig_lv
      (operands[0], rvtt_gen_rtx_noval (XTT32SImode), operands[1]));
    DONE;
  })

(define_insn "rvtt_sfpreadconfig_lv"
  [(set (match_operand:XTT32SI 0 "register_operand" "=xr,xr")
        (unspec_volatile:XTT32SI [
	  (match_operand:XTT32SI 1 "reg_or_cstlreg_or_noval_operand" "xn,0")
          (match_operand:SI 2 "const_int_operand" "n,n")
	  ] UNSPECV_SFPCONFIG))]
  "TARGET_XTT_TENSIX_BH_QSR"
  "@
   SFPMOV\t%0, L%2, 8\t# CFG:%2
   SFPMOV\t%0, L%2, 8\t# LV:%1 CFG:%2"
  [(set_attr "type" "tensix")])

(define_insn "rvtt_sfpwriteconfig_v"
  [(unspec_volatile:XTT32SI [
     (match_operand:XTT32SI 0 "register_operand" "x0")
     (match_operand:SI   1 "const_int_operand" "n")
     (match_operand:SI   2 "const_int_operand" "n")
     ] UNSPECV_SFPCONFIG)]
  "TARGET_XTT_TENSIX"
  "SFPCONFIG\t%2, 0, %1\t# R:%0 CFG:%2"
  [(set_attr "type" "tensix")
   (set (attr "xtt_dynamic_bug") (symbol_ref "xtt_dynamic_bug (XTT_DYNAMIC_BUG_BH | XTT_DYNAMIC_BUG_QSR)"))])

(define_expand "rvtt_sfpwriteconfig_i"
  [(unspec_volatile:XTT32SI [
     (match_operand:SI 0 "mem_or_0_operand")
     (match_operand:SI 1 "reg_or_const_int_operand")
     (match_operand:SI 2 "reg_or_0_operand")
     (match_operand:SI 3 "const_int_operand")
     (match_operand:SI 4 "const_int_operand")
     (match_operand:SI 5 "const_int_operand")
     ] UNSPECV_SFPCONFIG)]
  "TARGET_XTT_TENSIX"
{
  operands[4] = GEN_INT (INTVAL (operands[4]) | SFPCONFIG_MOD1_ARG_IMM);
  auto mem = CONST0_RTX (SImode);
  auto opc = CONST0_RTX (SImode);
  auto enc = CONST0_RTX (SImode);
  auto insn = CONST0_RTX (SImode);
  auto imm = operands[1];
  if (!CONST_INT_P (imm))
    {
      mem = gen_rtx_MEM (SImode, operands[0]);
      int op
        = TARGET_XTT_TENSIX_WH 	? TT_OP_WH_SFPCONFIG (0, INTVAL (operands[5]), INTVAL (operands[4]))
	: TARGET_XTT_TENSIX_BH 	? TT_OP_BH_SFPCONFIG (0, INTVAL (operands[5]), INTVAL (operands[4]))
	: TARGET_XTT_TENSIX_QSR	? TT_OP_QSR_SFPCONFIG (0, INTVAL (operands[5]), INTVAL (operands[4]))
        : (gcc_unreachable (), 0);
      opc = GEN_INT (op);
      enc = GEN_INT (rvtt_synth (UINTVAL (operands[3])));
      imm = rvtt_gen_rtx_noval (GET_MODE (imm));
      insn = operands[2];
    }

  emit_insn (gen_rvtt_sfpwriteconfig_i_int
    (mem, opc, enc, insn, rvtt_gen_rtx_noval (XTT32SImode), imm,
    operands[4], operands[5]));
  DONE;
})

(define_insn "rvtt_sfpwriteconfig_i_int"
  [(unspec_volatile:XTT32SI [
     (match_operand:SI 0 "mem_or_0_operand"  "J,m")
     (match_operand:SI 1 "const_int_operand" "J,n") ;; opcode
     (match_operand:SI 2 "const_int_operand" "J,n") ;; id
     (match_operand:SI 3 "reg_or_0_operand"  "J,r") ;; insn
     (match_operand:XTT32SI 4 "noval_operand" "xn,xn") ;; src (none)
     (match_operand:SI 5 "const_int_or_noval_operand" "n,xn") ;; imm
     (match_operand:SI 6 "const_int_operand" "n,n")
     (match_operand:SI 7 "const_int_operand" "n,n")
     ] UNSPECV_SFPCONFIG)]
  "TARGET_XTT_TENSIX"
  {
    return rvtt_synth::pattern (which_alternative,
      "SFPCONFIG\t%7, %5, %6\t# CFG:%7", operands, false, -1);
  }
  [(set_attr "type" "tensix")
   (set (attr "xtt_dynamic_bug") (symbol_ref "xtt_dynamic_bug (XTT_DYNAMIC_BUG_BH | XTT_DYNAMIC_BUG_QSR)"))])

(define_insn "rvtt_sfplut"
  [(set (match_operand:XTT32SI 0 "register_operand" "=x3")
        (unspec_volatile:XTT32SI [
	  (match_operand:XTT32SI 1 "reg_or_cstlreg_operand"  "x0")
          (match_operand:XTT32SI 2 "reg_or_cstlreg_operand"  "x1")
          (match_operand:XTT32SI 3 "reg_or_cstlreg_operand"  "x2")
          (match_operand:XTT32SI 4 "reg_or_cstlreg_operand"  "0")
          (match_operand:SI    5 "const_int_operand" "n")
	  ] UNSPECV_SFPLUT))]
  "TARGET_XTT_TENSIX_WH_BH"
  "SFPLUT\t%0, %5\t# R:%1,%2,%3,%4"
  [(set_attr "type" "tensix")
   (set_attr "xtt_delay" "dynamic")])

(define_insn "rvtt_sfplut_qsr"
  [(set (match_operand:XTT32SI 0 "register_operand" "=xr")
        (unspec_volatile:XTT32SI [
          (match_operand:XTT32SI 1 "reg_or_cstlreg_operand"  "x3")
          (match_operand:SI    2 "const_int_operand" "n")
	  ] UNSPECV_SFPLUT))]
  "TARGET_XTT_TENSIX_QSR"
  "SFPLUT\t%0, %2\t# R:%1"
  [(set_attr "type" "tensix")
   (set_attr "xtt_delay" "dynamic")])

(define_insn "rvtt_sfplutfp32"
  [(set (match_operand:XTT32SI 0 "register_operand" "=xr")
        (unspec_volatile:XTT32SI [
          (match_operand:XTT32SI 1 "register_operand"  "x3")
          (match_operand:SI    2 "const_int_operand" "n")
	  ] UNSPECV_SFPLUTFP32))]
  "TARGET_XTT_TENSIX_QSR"
  "SFPLUTFP32\t%0, %2\t# R:%1"
  [(set_attr "type" "tensix")
   (set_attr "xtt_delay" "dynamic")])

(define_insn_and_split "rvtt_sfplutfp32_3r"
  [(set (match_operand:XTT32SI 0 "register_operand" "=xr")
        (unspec_volatile:XTT32SI [
	  (match_operand:XTT32SI 1 "register_operand"  "x0")
          (match_operand:XTT32SI 2 "register_operand"  "x1")
          (match_operand:XTT32SI 3 "register_operand"  "x2")
          (match_operand:XTT32SI 4 "register_operand"  "x3")
          (match_operand:SI    5 "const_int_operand" "n")
	  ] UNSPECV_SFPLUTFP32))
        (clobber (match_scratch:XTT32SI 6 "=x7"))]
  "TARGET_XTT_TENSIX_WH_BH"
  "#"
  "&& reload_completed"
  [(const_int 0)]
{
  // The dst register is determined by the value in L7,
  // so we need to emit a loadi to L7 first. How pleasant.
  emit_insn (gen_rvtt_sfploadi_lv_int
    (operands[6], CONST0_RTX (SImode), CONST0_RTX (SImode), CONST0_RTX (SImode), CONST0_RTX (SImode),
    rvtt_gen_rtx_noval (XTT32SImode),
    rvtt_gen_rtx_noval (XTT32SImode),
    GEN_INT (REGNO (operands[0]) - SFPU_REG_FIRST),
    GEN_INT (SFPLOADI_MOD0_USHORT)));
  emit_insn (gen_rvtt_sfplutfp32_3r_split
    (operands[0], operands[1], operands[2], operands[3],
     operands[4], operands[5], operands[6]));
  DONE;
}
  [(set_attr "type" "tensix")
   (set_attr "xtt_delay" "dynamic")])

(define_insn "rvtt_sfplutfp32_3r_split"
  [(set (match_operand:XTT32SI 0 "register_operand" "=xr")
        (unspec_volatile:XTT32SI [
	  (match_operand:XTT32SI 1 "register_operand"  "x0")
          (match_operand:XTT32SI 2 "register_operand"  "x1")
          (match_operand:XTT32SI 3 "register_operand"  "x2")
          (match_operand:XTT32SI 4 "register_operand"  "x3")
          (match_operand:SI    5 "const_int_operand" "n")
          (match_operand:XTT32SI 6 "register_operand"  "x7")
	  ] UNSPECV_SFPLUTFP32))]
  "TARGET_XTT_TENSIX_WH_BH && reload_completed"
  "SFPLUTFP32\t%0, %5\t# R:%1,%2,%3,%4,%6"
  [(set_attr "type" "tensix")
   (set_attr "xtt_delay" "dynamic")])

(define_insn "rvtt_sfplutfp32_6r"
  [(set (match_operand:XTT32SI 0 "register_operand" "=xr")
        (unspec_volatile:XTT32SI [
	  (match_operand:XTT32SI 1 "reg_or_cstlreg_operand"  "x0")
          (match_operand:XTT32SI 2 "reg_or_cstlreg_operand"  "x1")
          (match_operand:XTT32SI 3 "reg_or_cstlreg_operand"  "x2")
          (match_operand:XTT32SI 4 "reg_or_cstlreg_operand"  "x4")
          (match_operand:XTT32SI 5 "reg_or_cstlreg_operand"  "x5")
          (match_operand:XTT32SI 6 "reg_or_cstlreg_operand"  "x6")
          (match_operand:XTT32SI 7 "reg_or_cstlreg_operand"  "x3")
          (match_operand:SI    8 "const_int_operand" "n")
	  ] UNSPECV_SFPLUTFP32))]
  "TARGET_XTT_TENSIX_WH_BH"
  "SFPLUTFP32\t%0, %8\t# R:%1,%2,%3,%4,%5,%6,%7"
  [(set_attr "type" "tensix")
   (set_attr "xtt_delay" "dynamic")])

(define_insn "rvtt_sfpswap_int"
  [(set (match_operand:XTT32SI 0 "register_operand" "=xr")
        (unspec_volatile:XTT32SI [
	  (match_operand:XTT32SI 2 "reg_or_cstlreg_operand" "0")
	  (match_operand:XTT32SI 3 "reg_or_cstlreg_operand" "1")
          (match_operand:SI    4 "const_int_operand"  "n")
          (match_operand:SI    5 "const_int_operand"  "n")
	  ] UNSPECV_SFPSWAP))
   (set (match_operand:XTT32SI 1 "register_operand" "=xr")
        (unspec_volatile:XTT32SI [
	  (match_dup 2)
	  (match_dup 3)
          (match_dup 4)
          (match_dup 5)
	  ] UNSPECV_SFPSWAP))]
  "TARGET_XTT_TENSIX"
  {
    return TARGET_XTT_TENSIX_WH_BH
      ? "SFPSWAP\t%2, %3, %4"
      : "SFPSWAP\t%2, %3, %5, %4";
  }
  [(set_attr "type" "tensix")
   (set (attr "xtt_dynamic_bug") (symbol_ref "xtt_dynamic_bug (XTT_DYNAMIC_BUG_BH | XTT_DYNAMIC_BUG_QSR)"))])

(define_insn "*rvtt_sfpswap_cst1"
  [(set (match_operand:XTT32SI 0 "register_operand" "=xr")
        (unspec_volatile:XTT32SI [
	  (match_operand:XTT32SI 1 "reg_or_cstlreg_operand" "0")
	  (match_operand:XTT32SI 2 "cstlreg_operand" "xc")
          (match_operand:SI    3 "const_int_operand"  "n")
          (match_operand:SI    4 "const_int_operand"  "n")
	  (const_int 1)
	  ] UNSPECV_SFPSWAP))]
  "TARGET_XTT_TENSIX"
  {
    return TARGET_XTT_TENSIX_WH_BH
      ? "SFPSWAP\t%1, %2, %3"
      : "SFPSWAP\t%1, %2, %4, %3";
  }
  [(set_attr "type" "tensix")
   (set (attr "xtt_dynamic_bug") (symbol_ref "xtt_dynamic_bug (XTT_DYNAMIC_BUG_BH | XTT_DYNAMIC_BUG_QSR)"))])

(define_split
  [(set (match_operand:XTT32SI 0 "register_operand")
        (unspec_volatile:XTT32SI [
	  (match_operand:XTT32SI 2 "reg_or_cstlreg_operand")
	  (match_operand:XTT32SI 3 "cstlreg_operand")
          (match_operand:SI    4 "const_int_operand")
          (match_operand:SI    5 "const_int_operand")
	  ] UNSPECV_SFPSWAP))
   (set (match_operand:XTT32SI 1 "register_operand")
        (unspec_volatile:XTT32SI [
	  (match_dup 2)
	  (match_dup 3)
          (match_dup 4)
          (match_dup 5)
	  ] UNSPECV_SFPSWAP))]
  "bool (find_reg_note (insn, REG_UNUSED, operands[1]))
   && !(cstlreg_operand (operands[2], XTT32SImode)
        && find_reg_note (insn, REG_UNUSED, operands[0]))"
  [(set (match_dup 0)
        (unspec_volatile:XTT32SI [
 	  (match_dup 2)
	  (match_dup 3)
          (match_dup 4)
          (match_dup 5)
	  (const_int 1)
	  ] UNSPECV_SFPSWAP))])

(define_insn "*rvtt_sfpswap_cst2"
  [(set (match_operand:XTT32SI 0 "register_operand" "=xr")
        (unspec_volatile:XTT32SI [
	  (match_operand:XTT32SI 1 "cstlreg_operand" "xs")
	  (match_operand:XTT32SI 2 "reg_or_cstlreg_operand" "0")
          (match_operand:SI    3 "const_int_operand"  "n")
          (match_operand:SI    4 "const_int_operand"  "n")
	  (const_int 2)
	  ] UNSPECV_SFPSWAP))]
  "TARGET_XTT_TENSIX"
  {
    return TARGET_XTT_TENSIX_WH_BH
      ? "SFPSWAP\t%1, %2, %3"
      : "SFPSWAP\t%1, %2, %4, %3";
  }
  [(set_attr "type" "tensix")
   (set (attr "xtt_dynamic_bug") (symbol_ref "xtt_dynamic_bug (XTT_DYNAMIC_BUG_BH | XTT_DYNAMIC_BUG_QSR)"))])

(define_split
  [(set (match_operand:XTT32SI 0 "register_operand")
        (unspec_volatile:XTT32SI [
	  (match_operand:XTT32SI 2 "cstlreg_operand")
	  (match_operand:XTT32SI 3 "reg_or_cstlreg_operand")
          (match_operand:SI    4 "const_int_operand")
          (match_operand:SI    5 "const_int_operand")
	  ] UNSPECV_SFPSWAP))
   (set (match_operand:XTT32SI 1 "register_operand")
        (unspec_volatile:XTT32SI [
	  (match_dup 2)
	  (match_dup 3)
          (match_dup 4)
          (match_dup 5)
	  ] UNSPECV_SFPSWAP))]
  "find_reg_note (insn, REG_UNUSED, operands[0])
   && !(cstlreg_operand (operands[3], XTT32SImode)
        && find_reg_note (insn, REG_UNUSED, operands[1]))"
  [(set (match_dup 1)
        (unspec_volatile:XTT32SI [
 	  (match_dup 2)
	  (match_dup 3)
          (match_dup 4)
          (match_dup 5)
	  (const_int 2)
	  ] UNSPECV_SFPSWAP))])

(define_insn "*rvtt_sfpswap_cst3"
  [(unspec_volatile:XTT32SI [
     (match_operand:XTT32SI 0 "cstlreg_operand" "xs")
     (match_operand:XTT32SI 1 "cstlreg_operand" "xc")
     (match_operand:SI    2 "const_int_operand"  "n")
     (match_operand:SI    3 "const_int_operand"  "n")
     (const_int 3)
  ] UNSPECV_SFPSWAP)]
  "TARGET_XTT_TENSIX"
  {
    return TARGET_XTT_TENSIX_WH_BH
      ? "SFPSWAP\t%0, %1, %2"
      : "SFPSWAP\t%0, %1, %3, %2";
  }
  [(set_attr "type" "tensix")
   (set (attr "xtt_dynamic_bug") (symbol_ref "xtt_dynamic_bug (XTT_DYNAMIC_BUG_BH | XTT_DYNAMIC_BUG_QSR)"))])

(define_split
  [(set (match_operand:XTT32SI 0 "register_operand")
        (unspec_volatile:XTT32SI [
	  (match_operand:XTT32SI 2 "cstlreg_operand")
	  (match_operand:XTT32SI 3 "cstlreg_operand")
          (match_operand:SI    4 "const_int_operand")
          (match_operand:SI    5 "const_int_operand")
	  ] UNSPECV_SFPSWAP))
   (set (match_operand:XTT32SI 1 "register_operand")
        (unspec_volatile:XTT32SI [
	  (match_dup 2)
	  (match_dup 3)
          (match_dup 4)
          (match_dup 5)
	  ] UNSPECV_SFPSWAP))]
  "bool (find_reg_note (insn, REG_UNUSED, operands[0]))
   && bool (find_reg_note (insn, REG_UNUSED, operands[1]))"
  [(unspec_volatile:XTT32SI [
     (match_dup 2)
     (match_dup 3)
     (match_dup 4)
     (match_dup 5)
     (const_int 3)
     ] UNSPECV_SFPSWAP)])

(define_expand "rvtt_sfpswap_wh"
  [(set (match_operand:XTT64SI 0 "register_operand")
        (unspec_volatile:XTT32SI [
	  (match_operand:XTT32SI 1 "reg_or_cstlreg_operand")
	  (match_operand:XTT32SI 2 "reg_or_cstlreg_operand")
          (match_operand:SI 3 "const_int_operand")
	  ] UNSPECV_SFPSWAP))]
  "TARGET_XTT_TENSIX_WH_BH"
{
  emit_insn (gen_rvtt_sfpswap
    (operands[0], operands[1], operands[2], operands[3], CONST0_RTX (SImode)));
  DONE;
})

(define_expand "rvtt_sfpswap"
  [(set (match_operand:XTT64SI 0 "register_operand")
        (unspec_volatile:XTT32SI [
	  (match_operand:XTT32SI 1 "reg_or_cstlreg_operand")
	  (match_operand:XTT32SI 2 "reg_or_cstlreg_operand")
          (match_operand:SI 3 "const_int_operand")
          (match_operand:SI 4 "const_int_operand")
	  ] UNSPECV_SFPSWAP))]
  "TARGET_XTT_TENSIX"
{
  rtx a = gen_reg_rtx (XTT32SImode);
  rtx b = gen_reg_rtx (XTT32SImode);
  emit_insn (gen_rvtt_sfpswap_int
    (a, b, operands[1], operands[2], operands[3], operands[4]));
  emit_insn (gen_rvtt_sfpconcat2
    (operands[0], a, b));
  DONE;
})

(define_insn "rvtt_sfptransp_int"
  [(set (match_operand:XTT32SI 0 "register_operand" "=x0")
        (unspec_volatile:XTT32SI [
	  (match_operand:XTT32SI 8 "reg_or_cstlreg_operand" "0")
	  (match_operand:XTT32SI 9 "reg_or_cstlreg_operand" "1")
	  (match_operand:XTT32SI 10 "reg_or_cstlreg_operand" "2")
	  (match_operand:XTT32SI 11 "reg_or_cstlreg_operand" "3")
	  ] UNSPECV_SFPTRANSP))
   (set (match_operand:XTT32SI 1 "register_operand" "=x1")
        (unspec_volatile:XTT32SI [
	  (match_dup 8)
	  (match_dup 9)
          (match_dup 10)
          (match_dup 11)
	  ] UNSPECV_SFPTRANSP))
   (set (match_operand:XTT32SI 2 "register_operand" "=x2")
        (unspec_volatile:XTT32SI [
	  (match_dup 8)
	  (match_dup 9)
          (match_dup 10)
          (match_dup 11)
	  ] UNSPECV_SFPTRANSP))
   (set (match_operand:XTT32SI 3 "register_operand" "=x3")
        (unspec_volatile:XTT32SI [
	  (match_dup 8)
	  (match_dup 9)
          (match_dup 10)
          (match_dup 11)
	  ] UNSPECV_SFPTRANSP))
   (set (match_operand:XTT32SI 4 "register_operand" "=x4")
        (unspec_volatile:XTT32SI [
	  (match_operand:XTT32SI 12 "reg_or_cstlreg_operand" "4")
	  (match_operand:XTT32SI 13 "reg_or_cstlreg_operand" "5")
	  (match_operand:XTT32SI 14 "reg_or_cstlreg_operand" "6")
	  (match_operand:XTT32SI 15 "reg_or_cstlreg_operand" "7")
	  ] UNSPECV_SFPTRANSP))
   (set (match_operand:XTT32SI 5 "register_operand" "=x5")
        (unspec_volatile:XTT32SI [
	  (match_dup 12)
	  (match_dup 13)
          (match_dup 14)
          (match_dup 15)
	  ] UNSPECV_SFPTRANSP))
   (set (match_operand:XTT32SI 6 "register_operand" "=x6")
        (unspec_volatile:XTT32SI [
	  (match_dup 12)
	  (match_dup 13)
          (match_dup 14)
          (match_dup 15)
	  ] UNSPECV_SFPTRANSP))
   (set (match_operand:XTT32SI 7 "register_operand" "=x7")
        (unspec_volatile:XTT32SI [
	  (match_dup 12)
	  (match_dup 13)
          (match_dup 14)
          (match_dup 15)
	  ] UNSPECV_SFPTRANSP))]
  "TARGET_XTT_TENSIX"
  "SFPTRANSP"
  [(set_attr "type" "tensix")])

(define_expand "rvtt_sfptransp"
  [(set (match_operand:XTT256SI 0 "register_operand")
        (unspec_volatile:XTT32SI [
	  (match_operand:XTT32SI 1 "reg_or_cstlreg_operand")
	  (match_operand:XTT32SI 2 "reg_or_cstlreg_operand")
	  (match_operand:XTT32SI 3 "reg_or_cstlreg_operand")
	  (match_operand:XTT32SI 4 "reg_or_cstlreg_operand")
	  (match_operand:XTT32SI 5 "reg_or_cstlreg_operand")
	  (match_operand:XTT32SI 6 "reg_or_cstlreg_operand")
	  (match_operand:XTT32SI 7 "reg_or_cstlreg_operand")
	  (match_operand:XTT32SI 8 "reg_or_cstlreg_operand")
	  ] UNSPECV_SFPTRANSP))]
  "TARGET_XTT_TENSIX"
{
  rtx a = gen_reg_rtx (XTT32SImode);
  rtx b = gen_reg_rtx (XTT32SImode);
  rtx c = gen_reg_rtx (XTT32SImode);
  rtx d = gen_reg_rtx (XTT32SImode);
  rtx e = gen_reg_rtx (XTT32SImode);
  rtx f = gen_reg_rtx (XTT32SImode);
  rtx g = gen_reg_rtx (XTT32SImode);
  rtx h = gen_reg_rtx (XTT32SImode);

  emit_insn (gen_rvtt_sfptransp_int
    (a, b, c, d, e, f, g, h,
    operands[1], operands[2], operands[3], operands[4], operands[5], operands[6], operands[7], operands[8]));
  emit_insn (gen_rvtt_sfpconcat8
    (operands[0], a, b, c, d, e, f, g, h));
  DONE;
})

(define_insn "rvtt_sfpshft2_copy4_int"
  [(set (match_operand:XTT32SI 0 "register_operand" "=x0")
        (unspec_volatile:XTT32SI [
	  (match_operand:XTT32SI 4 "reg_or_cstlreg_operand" "1")
	  (match_operand:SI     7 "const_int_operand" "n")
	  ] UNSPECV_SFPSHFT2_COPY4))
   (set (match_operand:XTT32SI 1 "register_operand" "=x1")
        (unspec_volatile:XTT32SI [
	  (match_operand:XTT32SI 5 "reg_or_cstlreg_operand" "2")
	  (match_dup 7)
	  ] UNSPECV_SFPSHFT2_COPY4))
   (set (match_operand:XTT32SI 2 "register_operand" "=x2")
        (unspec_volatile:XTT32SI [
	  (match_operand:XTT32SI 6 "reg_or_cstlreg_operand" "3")
	  (match_dup 7)
	  ] UNSPECV_SFPSHFT2_COPY4))
   (set (match_operand:XTT32SI 3 "register_operand" "=x3")
        (unspec_volatile:XTT32SI [
	  (match_dup 7)
	  ] UNSPECV_SFPSHFT2_COPY4))]
  "TARGET_XTT_TENSIX"
  "SFPSHFT2\t%0, %0, 0, %7"
  [(set_attr "type" "tensix")
   (set (attr "xtt_dynamic_bug") (symbol_ref "xtt_dynamic_bug (XTT_DYNAMIC_BUG_BH | XTT_DYNAMIC_BUG_QSR)"))])

(define_expand "rvtt_sfpshft2_copy4"
  [(set (match_operand:XTT128SI 0 "register_operand")
        (unspec_volatile:XTT32SI [
	  (match_operand:XTT32SI 1 "reg_or_cstlreg_operand")
	  (match_operand:XTT32SI 2 "reg_or_cstlreg_operand")
	  (match_operand:XTT32SI 3 "reg_or_cstlreg_operand")
	  (match_operand:SI    4 "const_int_operand")
	  ] UNSPECV_SFPSHFT2_COPY4))]
  "TARGET_XTT_TENSIX"
{
  rtx a = gen_reg_rtx (XTT32SImode);
  rtx b = gen_reg_rtx (XTT32SImode);
  rtx c = gen_reg_rtx (XTT32SImode);
  rtx d = gen_reg_rtx (XTT32SImode);

  emit_insn (gen_rvtt_sfpshft2_copy4_int
    (a, b, c, d, operands[1], operands[2], operands[3],
     operands[4]));
  emit_insn (gen_rvtt_sfpconcat4
    (operands[0], a, b, c, d));
  DONE;
})

(define_insn "rvtt_sfpshft2_subvec_copy4_int"
  [(set (match_operand:XTT32SI 0 "register_operand" "=x0")
        (unspec_volatile:XTT32SI [
	  (match_operand:XTT32SI 4 "reg_or_cstlreg_operand" "1")
	  (match_operand:SI    8 "const_int_operand" "n")
	  ] UNSPECV_SFPSHFT2_SUBVEC_COPY4))
   (set (match_operand:XTT32SI 1 "register_operand" "=x1")
        (unspec_volatile:XTT32SI [
	  (match_operand:XTT32SI 5 "reg_or_cstlreg_operand" "2")
	  (match_dup 8)
	  ] UNSPECV_SFPSHFT2_SUBVEC_COPY4))
   (set (match_operand:XTT32SI 2 "register_operand" "=x2")
        (unspec_volatile:XTT32SI [
	  (match_operand:XTT32SI 6 "reg_or_cstlreg_operand" "3")
	  (match_dup 8)
	  ] UNSPECV_SFPSHFT2_SUBVEC_COPY4))
   (set (match_operand:XTT32SI 3 "register_operand" "=x3")
        (unspec_volatile:XTT32SI [
	  (match_operand:XTT32SI 7 "reg_or_cstlreg_operand" "0")
	  (match_dup 8)
	  ] UNSPECV_SFPSHFT2_SUBVEC_COPY4))]
  "TARGET_XTT_TENSIX"
  "SFPSHFT2\t%0, %0, 0, %8"
  [(set_attr "type" "tensix")
   (set (attr "xtt_dynamic_bug") (symbol_ref "xtt_dynamic_bug (XTT_DYNAMIC_BUG_BH | XTT_DYNAMIC_BUG_QSR)"))])

(define_expand "rvtt_sfpshft2_subvec_copy4"
  [(set (match_operand:XTT128SI 0 "register_operand")
        (unspec_volatile:XTT32SI [
	  (match_operand:XTT32SI 1 "reg_or_cstlreg_operand")
	  (match_operand:XTT32SI 2 "reg_or_cstlreg_operand")
	  (match_operand:XTT32SI 3 "reg_or_cstlreg_operand")
	  (match_operand:XTT32SI 4 "reg_or_cstlreg_operand")
	  (match_operand:SI     5 "const_int_operand")
	  ] UNSPECV_SFPSHFT2_SUBVEC_COPY4))]
  "TARGET_XTT_TENSIX"
{
  rtx a = gen_reg_rtx (XTT32SImode);
  rtx b = gen_reg_rtx (XTT32SImode);
  rtx c = gen_reg_rtx (XTT32SImode);
  rtx d = gen_reg_rtx (XTT32SImode);

  emit_insn (gen_rvtt_sfpshft2_subvec_copy4_int
    (a, b, c, d, operands[1], operands[2], operands[3], operands[4],
     operands[5]));
  emit_insn (gen_rvtt_sfpconcat4
    (operands[0], a, b, c, d));
  DONE;
})

(define_insn "rvtt_sfpshft2_subvec_shfl1_copy4_int"
  [(set (match_operand:XTT32SI 0 "register_operand" "=x0")
        (unspec_volatile:XTT32SI [
	  (match_operand:XTT32SI 4 "reg_or_cstlreg_operand" "1")
	  (match_operand:SI    8 "const_int_operand" "n")
	  ] UNSPECV_SFPSHFT2_SUBVEC_SHFL1_COPY4))
   (set (match_operand:XTT32SI 1 "register_operand" "=x1")
        (unspec_volatile:XTT32SI [
	  (match_operand:XTT32SI 5 "reg_or_cstlreg_operand" "2")
	  (match_dup 8)
	  ] UNSPECV_SFPSHFT2_SUBVEC_SHFL1_COPY4))
   (set (match_operand:XTT32SI 2 "register_operand" "=x2")
        (unspec_volatile:XTT32SI [
	  (match_operand:XTT32SI 6 "reg_or_cstlreg_operand" "3")
	  (match_dup 8)
	  ] UNSPECV_SFPSHFT2_SUBVEC_SHFL1_COPY4))
   (set (match_operand:XTT32SI 3 "register_operand" "=x3")
        (unspec_volatile:XTT32SI [
	  (match_operand:XTT32SI 7 "reg_or_cstlreg_operand" "xrxc")
	  (match_dup 8)
	  ] UNSPECV_SFPSHFT2_SUBVEC_SHFL1_COPY4))]
  "TARGET_XTT_TENSIX"
  "SFPSHFT2\t%0, %7, 0, %8"
  [(set_attr "type" "tensix")
   (set_attr "xtt_delay" "static")
   (set (attr "xtt_dynamic_bug") (symbol_ref "xtt_dynamic_bug (XTT_DYNAMIC_BUG_BH | XTT_DYNAMIC_BUG_QSR)"))])

(define_expand "rvtt_sfpshft2_subvec_shfl1_copy4"
  [(set (match_operand:XTT128SI 0 "register_operand")
        (unspec_volatile:XTT32SI [
	  (match_operand:XTT32SI 1 "reg_or_cstlreg_operand")
	  (match_operand:XTT32SI 2 "reg_or_cstlreg_operand")
	  (match_operand:XTT32SI 3 "reg_or_cstlreg_operand")
	  (match_operand:XTT32SI 4 "reg_or_cstlreg_operand")
	  (match_operand:SI     5 "const_int_operand")
	  ] UNSPECV_SFPSHFT2_SUBVEC_SHFL1_COPY4))]
  "TARGET_XTT_TENSIX"
{
  rtx a = gen_reg_rtx (XTT32SImode);
  rtx b = gen_reg_rtx (XTT32SImode);
  rtx c = gen_reg_rtx (XTT32SImode);
  rtx d = gen_reg_rtx (XTT32SImode);

  emit_insn (gen_rvtt_sfpshft2_subvec_shfl1_copy4_int
    (a, b, c, d, operands[1], operands[2], operands[3], operands[4],
     operands[5]));
  emit_insn (gen_rvtt_sfpconcat4
    (operands[0], a, b, c, d));
  DONE;
})

(define_insn "rvtt_sfpshft2_subvec_shfl1_int"
  [(set (match_operand:XTT32SI 0 "register_operand" "=xr,xr")
        (unspec_volatile:XTT32SI [
	  (match_operand:XTT32SI 1 "reg_or_cstlreg_or_noval_operand" "xn,0")
	  (match_operand:XTT32SI 2 "reg_or_cstlreg_operand" "xrxc,xrxc")
	  (match_operand:SI    3 "const_int_operand" "n,n")
	  ] UNSPECV_SFPSHFT2_SUBVEC_SHFL1))]
  "TARGET_XTT_TENSIX"
  "SFPSHFT2\t%0, %2, 0, %3"
  [(set_attr "type" "tensix")
   (set_attr "xtt_delay" "static")
   (set (attr "xtt_dynamic_bug") (symbol_ref "xtt_dynamic_bug (XTT_DYNAMIC_BUG_BH | XTT_DYNAMIC_BUG_QSR)"))])

(define_insn "rvtt_sfpshft2_subvec_shfl1_dead"
  [(unspec_volatile:XTT32SI [
     (match_operand:XTT32SI 0 "reg_or_cstlreg_or_noval_operand" "xn,xrxc")
     (match_operand:XTT32SI 1 "reg_or_cstlreg_operand" "xrxc,xrxc")
     (match_operand:SI 2 "const_int_operand" "n,n")
     ] UNSPECV_SFPSHFT2_SUBVEC_SHFL1)]
  "TARGET_XTT_TENSIX"
  "@
   SFPSHFT2\tL8, %1, 0, %2
   SFPSHFT2\tL8, %1, 0, %2\t# LV:%1"
  [(set_attr "type" "tensix")
   (set_attr "xtt_delay" "static")
   (set (attr "xtt_dynamic_bug") (symbol_ref "xtt_dynamic_bug (XTT_DYNAMIC_BUG_BH | XTT_DYNAMIC_BUG_QSR)"))])

(define_split
  [(set (match_operand:XTT32SI 0 "register_operand")
        (unspec_volatile:XTT32SI [
	  (match_operand:XTT32SI 1 "reg_or_cstlreg_or_noval_operand")
	  (match_operand:XTT32SI 2 "reg_or_cstlreg_operand")
	  (match_operand:SI    3 "const_int_operand")
	  ] UNSPECV_SFPSHFT2_SUBVEC_SHFL1))]
  "bool (find_reg_note (insn, REG_UNUSED, operands[0]))"
  [(unspec_volatile:XTT32SI [
     (match_dup 1)
     (match_dup 2)
     (match_dup 3)
     ] UNSPECV_SFPSHFT2_SUBVEC_SHFL1)])

(define_expand "rvtt_sfpshft2_subvec_shfl1"
  [(set (match_operand:XTT32SI 0 "register_operand")
        (unspec_volatile:XTT32SI [
	  (match_operand:XTT32SI 1 "reg_or_cstlreg_operand")
	  (match_operand:SI    2 "const_int_operand")
	  ] UNSPECV_SFPSHFT2_SUBVEC_SHFL1))]
  "TARGET_XTT_TENSIX"
{
  emit_insn (gen_rvtt_sfpshft2_subvec_shfl1_lv
    (operands[0], rvtt_gen_rtx_noval (XTT32SImode), operands[1],
     operands[2]));
  DONE;
})

(define_expand "rvtt_sfpshft2_subvec_shfl1_lv"
  [(set (match_operand:XTT32SI 0 "register_operand")
        (unspec_volatile:XTT32SI [
	  (match_operand:XTT32SI 1 "reg_or_cstlreg_operand")
	  (match_operand:XTT32SI 2 "reg_or_cstlreg_operand")
	  (match_operand:SI    3 "const_int_operand")
	  ] UNSPECV_SFPSHFT2_SUBVEC_SHFL1))]
  "TARGET_XTT_TENSIX"
{
  if (TARGET_XTT_TENSIX_WH && INTVAL (operands[3]) == SFPSHFT2_MOD1_SUBVEC_SHFLSHR1)
    {
      // WH_B0 HW bug (issue #3240): the shftr version of the insn doesn't set the
      // value shifted into place to 0 but instead uses the previous value (eg,
      // from a ror) Here we clear that value to 0 by rotating in the 0 register

      emit_insn (gen_rvtt_sfpshft2_subvec_shfl1_dead
        (rvtt_gen_rtx_noval (XTT32SImode),
	 rvtt_gen_rtx_creg (XTT32SImode, CREG_IDX_0),
	 GEN_INT (SFPSHFT2_MOD1_SUBVEC_SHFLROR1)));
    }
  emit_insn (gen_rvtt_sfpshft2_subvec_shfl1_int
    (operands[0], operands[1], operands[2],
     operands[3]));
  DONE;
})

(define_int_iterator rvtt_gtle_op [
  UNSPECV_SFPGT
  UNSPECV_SFPLE
  ])
(define_int_attr rvtt_gtle_name [
  (UNSPECV_SFPGT "gt")
  (UNSPECV_SFPLE "le")
  ])
(define_int_attr rvtt_gtle_insn [
  (UNSPECV_SFPGT "GT")
  (UNSPECV_SFPLE "LE")
  ])

(define_expand "rvtt_sfp<rvtt_gtle_name>_bh"
  [(set (match_operand:XTT32SI 0 "register_operand")
        (unspec_volatile:XTT32SI [
          (match_operand:XTT32SI 1 "reg_or_cstlreg_operand")
          (match_operand:XTT32SI 2 "reg_or_cstlreg_operand")
          (match_operand:SI    3 "const_int_operand")
          ] rvtt_gtle_op))]
  "TARGET_XTT_TENSIX_BH"
  {
    emit_insn (gen_rvtt_sfp<rvtt_gtle_name>_lv
      (operands[0], rvtt_gen_rtx_noval (XTT32SImode),
       operands[1], operands[2], operands[3], CONST0_RTX (SImode)));
    DONE;
  })

(define_expand "rvtt_sfp<rvtt_gtle_name>_lv_bh"
  [(set (match_operand:XTT32SI 0 "register_operand")
        (unspec_volatile:XTT32SI [
          (match_operand:XTT32SI 1 "reg_or_cstlreg_or_noval_operand")
          (match_operand:XTT32SI 2 "reg_or_cstlreg_operand")
          (match_operand:XTT32SI 3 "reg_or_cstlreg_operand")
          (match_operand:SI    4 "const_int_operand")
          ] rvtt_gtle_op))]
  "TARGET_XTT_TENSIX_BH"
  {
    emit_insn (gen_rvtt_sfp<rvtt_gtle_name>_lv
      (operands[0], operands[1],
       operands[2], operands[3], operands[4], CONST0_RTX (SImode)));
    DONE;
  })

(define_expand "rvtt_sfp<rvtt_gtle_name>"
  [(set (match_operand:XTT32SI 0 "register_operand")
        (unspec_volatile:XTT32SI [
          (match_operand:XTT32SI 1 "reg_or_cstlreg_operand")
          (match_operand:XTT32SI 2 "reg_or_cstlreg_operand")
          (match_operand:SI    3 "const_int_operand")
          (match_operand:SI    4 "const_int_operand")
          ] rvtt_gtle_op))]
  "TARGET_XTT_TENSIX_QSR"
  {
    emit_insn (gen_rvtt_sfp<rvtt_gtle_name>_lv
      (operands[0], rvtt_gen_rtx_noval (XTT32SImode),
       operands[1], operands[2], operands[3], operands[4]));
    DONE;
  })

(define_expand "rvtt_sfp<rvtt_gtle_name>_lv"
  [(set (match_operand:XTT32SI 0 "register_operand")
        (unspec_volatile:XTT32SI [
          (match_operand:XTT32SI 1 "reg_or_cstlreg_or_noval_operand")
          (match_operand:XTT32SI 2 "reg_or_cstlreg_operand")
          (match_operand:XTT32SI 3 "reg_or_cstlreg_operand")
          (match_operand:SI    4 "const_int_operand")
          (match_operand:SI    5 "const_int_operand")
          ] rvtt_gtle_op))]
  "TARGET_XTT_TENSIX_BH_QSR"
  {
    // We clear this later if result is unused
    operands[4] = GEN_INT (INTVAL (operands[4]) | SFPGTLE_MOD1_SET_DEST);
  })

(define_insn_and_rewrite "*rvtt_sfp<rvtt_gtle_name>_int"
  [(set (match_operand:XTT32SI 0 "register_operand" "=xr,xr,xr")
        (unspec_volatile:XTT32SI [
          (match_operand:XTT32SI 1 "reg_or_cstlreg_or_noval_or_omit_operand"  "xn,xo,xrxc")
          (match_operand:XTT32SI 2 "reg_or_cstlreg_operand"  "0,0,0")
          (match_operand:XTT32SI 3 "reg_or_cstlreg_operand"  "xrxc,xrxc,xrxc")
          (match_operand:SI    4 "const_int_operand" "n,n,n")
          (match_operand:SI    5 "const_int_operand" "n,n,n")
          ] rvtt_gtle_op))]
  "TARGET_XTT_TENSIX_BH_QSR"
  "@
   SFP<rvtt_gtle_insn>\t%0, %3, %5, %4
   SFP<rvtt_gtle_insn>\t%0, %3, %5, %4\t# LV:%2
   #"
  "&& true"
  {
    if (find_reg_note (curr_insn, REG_UNUSED, operands[0]))
      {
        emit_insn (gen_rvtt_sfp<rvtt_gtle_name>_nv
          (operands[2], operands[3],
           GEN_INT (INTVAL (operands[4]) & ~SFPGTLE_MOD1_SET_DEST),
	   operands[5]));
        DONE;
      }

    if (noval_or_omit_operand (operands[1], GET_MODE (operands[1])))
      FAIL; // nothing to change

    rvtt_merge_lv_src (&operands[1], &operands[2]);
  }
  [(set_attr "type" "tensix")])

(define_insn "rvtt_sfp<rvtt_gtle_name>_nv"
  [(unspec_volatile:XTT32SI [
     (match_operand:XTT32SI 0 "reg_or_cstlreg_operand"  "xrxs")
     (match_operand:XTT32SI 1 "reg_or_cstlreg_operand"  "xrxc")
     (match_operand:SI    2 "const_int_operand" "n")
     (match_operand:SI    3 "const_int_operand" "n")
     ] rvtt_gtle_op)]
  "TARGET_XTT_TENSIX_BH_QSR"
  "SFP<rvtt_gtle_insn>\t%0, %1, %3, %2"
  [(set_attr "type" "tensix")])

(define_expand "rvtt_sfpmul24"
  [(set (match_operand:XTT32SI 0 "register_operand")
        (unspec_volatile:XTT32SI [
	  (match_operand:XTT32SI 1 "reg_or_cstlreg_operand")
          (match_operand:XTT32SI 2 "reg_or_cstlreg_operand")
          (match_operand:SI    3 "const_int_operand")
	  ] UNSPECV_SFPMUL24))]
  "TARGET_XTT_TENSIX_BH_QSR"
{
  emit_insn (gen_rvtt_sfpmul24_lv
    (operands[0], rvtt_gen_rtx_noval (XTT32SImode),
     operands[1], operands[2], operands[3]));
  DONE;
})

(define_insn "rvtt_sfpmul24_lv"
  [(set (match_operand:XTT32SI 0 "register_operand" "=xr,xr")
        (unspec_volatile:XTT32SI [
	  (match_operand:XTT32SI 1 "reg_or_cstlreg_or_noval_operand" "xn,0")
          (match_operand:XTT32SI 2 "reg_or_cstlreg_operand"  "xrxc,xrxc")
          (match_operand:XTT32SI 3 "reg_or_cstlreg_operand"  "xrxc,xrxc")
          (match_operand:SI    4 "const_int_operand" "n,n")
	  ] UNSPECV_SFPMUL24))]
  "TARGET_XTT_TENSIX_BH_QSR"
  "@
   SFPMUL24\t%0, %2, %3, %4
   SFPMUL24\t%0, %2, %3, %4\t# LV:%1"
  [(set_attr "type" "tensix")
   (set_attr "xtt_delay" "dynamic")])

(define_expand "rvtt_sfparecip"
  [(set (match_operand:XTT32SI 0 "register_operand")
        (unspec_volatile:XTT32SI [
	  (match_operand:XTT32SI 1 "reg_or_cstlreg_operand")
          (match_operand:SI    2 "const_int_operand")
	  ] UNSPECV_SFPARECIP))]
  "TARGET_XTT_TENSIX_BH"
{
  emit_insn (gen_rvtt_sfparecip_lv
    (operands[0], rvtt_gen_rtx_noval (XTT32SImode),
     operands[1], operands[2]));
  DONE;
})

(define_insn "rvtt_sfparecip_lv"
  [(set (match_operand:XTT32SI 0 "register_operand" "=xr,xr")
        (unspec_volatile:XTT32SI [
	  (match_operand:XTT32SI 1 "reg_or_cstlreg_or_noval_operand"  "xn,0")
          (match_operand:XTT32SI 2 "reg_or_cstlreg_operand"  "xrxc,xrxc")
          (match_operand:SI    3 "const_int_operand" "n,n")
	  ] UNSPECV_SFPARECIP))]
  "TARGET_XTT_TENSIX_BH"
  "@
   SFPARECIP\t%0, %2, %3
   SFPARECIP\t%0, %2, %3\t# LV:%1"
  [(set_attr "type" "tensix")])

(define_expand "rvtt_sfpnonlinear"
  [(set (match_operand:XTT32SI 0 "register_operand")
        (unspec_volatile:XTT32SI [
	  (match_operand:XTT32SI 1 "reg_or_cstlreg_operand")
          (match_operand:SI    2 "const_int_operand")
	  ] UNSPECV_SFPNONLINEAR))]
  "TARGET_XTT_TENSIX_QSR"
{
  emit_insn (gen_rvtt_sfpnonlinear_lv
    (operands[0], rvtt_gen_rtx_noval (XTT32SImode),
     operands[1], operands[2]));
  DONE;
})

(define_insn "rvtt_sfpnonlinear_lv"
  [(set (match_operand:XTT32SI 0 "register_operand" "=xr,xr")
        (unspec_volatile:XTT32SI [
	  (match_operand:XTT32SI 1 "reg_or_cstlreg_or_noval_operand"  "xn,0")
          (match_operand:XTT32SI 2 "reg_or_cstlreg_operand"  "xrxc,xrxc")
          (match_operand:SI    3 "const_int_operand" "n,n")
	  ] UNSPECV_SFPNONLINEAR))]
  "TARGET_XTT_TENSIX_QSR"
  "@
   SFPNONLINEAR\t%0, %2, %3
   SFPNONLINEAR\t%0, %2, %3\t# LV:%1"
  [(set_attr "type" "tensix")
   (set (attr "xtt_dynamic_bug") (symbol_ref "xtt_dynamic_bug (XTT_DYNAMIC_BUG_QSR)"))])

(define_expand "rvtt_ttinsn"
  [(unspec_volatile
    [(match_operand:SI    0 "reg_or_0_operand")
     (match_operand:SI    1 "reg_or_const_int_operand")
     (match_operand:SI    2 "const_int_operand")
     (match_operand:SI    3 "const_int_operand")
     (match_operand:SI    4 "const_int_operand")
     ] UNSPECV_TTINSN)]
  "TARGET_XTT_TENSIX"
{
  auto mem = CONST0_RTX (SImode);
  auto val = operands[1];

  if (!CONST_INT_P (val))
    mem = gen_rtx_MEM (SImode, operands[0]);
  emit_insn (gen_rvtt_ttinsn_int (mem, val));
  DONE;
})

(define_insn "rvtt_ttinsn_int"
  [(unspec_volatile:XTT32SI [
     (match_operand:SI    0 "mem_or_0_operand"  "J,m")
     (match_operand:SI    1 "reg_or_const_int_operand" "n,r")
     ] UNSPECV_TTINSN)]
  "TARGET_XTT_TENSIX"
  "@
   .ttinsn\t%1
   sw\t%1,%0"
  [(set_attr "type" "tensix")])

(define_expand "rvtt_ttreplay"
  [(unspec_volatile:XTT32SI [
     (match_operand:SI    0 "reg_or_0_operand")
     (match_operand:SI    1 "reg_or_const_int_operand")
     (match_operand:SI    2 "reg_or_const_int_operand")
     (match_operand:SI    3 "reg_or_0_operand")
     (match_operand:SI    4 "const_int_operand")
     (match_operand:SI    5 "const_int_operand")
     (match_operand:SI    6 "const_int_operand")
     ] UNSPECV_TTREPLAY)]
  "TARGET_XTT_TENSIX_WH_BH"
{
  auto mem = CONST0_RTX (SImode);
  auto opc = CONST0_RTX (SImode);
  auto enc = CONST0_RTX (SImode);
  auto insn = CONST0_RTX (SImode);
  unsigned csts[2] = {0, 0};
  bool all_const = true;
  for (unsigned ix = 2; ix--;)
    if (CONST_INT_P (operands[1 + ix]))
      csts[ix] = UINTVAL (operands[1 + ix]);
    else
      {
        operands[1 + ix] = rvtt_gen_rtx_noval (GET_MODE (operands[1 + ix]));
        all_const = false;
      }
  if (!all_const)
    {
      mem = gen_rtx_MEM (SImode, operands[0]);
      int op
        = TARGET_XTT_TENSIX_WH  ? TT_OP_WH_REPLAY (csts[0], csts[1],
                         INTVAL (operands[5]), INTVAL (operands[6]))
        : TARGET_XTT_TENSIX_BH  ? TT_OP_BH_REPLAY (csts[0], csts[1],
	                 INTVAL (operands[5]), INTVAL (operands[6]))
        : (gcc_unreachable (), 0);
      opc = GEN_INT (op);
      enc = GEN_INT (rvtt_synth (UINTVAL (operands[4])));
      insn = operands[3];
    }

  emit_insn (gen_rvtt_ttreplay_int
    (mem, opc, enc, insn, rvtt_gen_rtx_noval (XTT32SImode),
     operands[1], operands[2],
     operands[5], operands[6], CONST0_RTX (SImode), CONST0_RTX (SImode)));
  DONE;
})

(define_expand "rvtt_ttreplay_qsr"
  [(unspec_volatile:XTT32SI [
     (match_operand:SI    0 "reg_or_0_operand")
     (match_operand:SI    1 "reg_or_const_int_operand")
     (match_operand:SI    2 "reg_or_const_int_operand")
     (match_operand:SI    3 "reg_or_0_operand")
     (match_operand:SI    4 "const_int_operand")
     (match_operand:SI    5 "const_int_operand")
     (match_operand:SI    6 "const_int_operand")
     (match_operand:SI    7 "const_int_operand")
     (match_operand:SI    8 "const_int_operand")
     ] UNSPECV_TTREPLAY)]
  "TARGET_XTT_TENSIX_QSR"
{
  auto mem = CONST0_RTX (SImode);
  auto opc = CONST0_RTX (SImode);
  auto enc = CONST0_RTX (SImode);
  auto insn = CONST0_RTX (SImode);
  unsigned csts[2] = {0, 0};
  bool all_const = true;
  for (unsigned ix = 2; ix--;)
    if (CONST_INT_P (operands[1 + ix]))
      csts[ix] = UINTVAL (operands[1 + ix]);
    else
      {
        operands[1 + ix] = rvtt_gen_rtx_noval (GET_MODE (operands[1 + ix]));
        all_const = false;
      }
  if (!all_const)
    {
      mem = gen_rtx_MEM (SImode, operands[0]);
      int op
        = TARGET_XTT_TENSIX_QSR ? TT_OP_QSR_REPLAY (csts[0], csts[1],
	                 INTVAL (operands[7]), INTVAL (operands[8]),
			 INTVAL (operands[5]), INTVAL (operands[6]))
        : (gcc_unreachable (), 0);
      opc = GEN_INT (op);
      enc = GEN_INT (rvtt_synth (UINTVAL (operands[4])));
      insn = operands[3];
    }

  emit_insn (gen_rvtt_ttreplay_int
    (mem, opc, enc, insn, rvtt_gen_rtx_noval (XTT32SImode),
     operands[1], operands[2],
     operands[5], operands[6], operands[7], operands[8]));
  DONE;
})

(define_insn "rvtt_ttreplay_int"
  [(unspec_volatile:XTT32SI [
     (match_operand:SI    0 "mem_or_0_operand"  "J,m")
     (match_operand:SI    1 "const_int_operand" "J,n") ;; opcode
     (match_operand:SI    2 "const_int_operand" "J,n") ;; id, src & dst shifts
     (match_operand:SI    3 "reg_or_0_operand"  "J,r") ;; insn
     (match_operand:XTT32SI 4 "noval_operand"   "xn,xn") ;; src (none)
     (match_operand:SI    5 "const_int_or_noval_operand" "n,nxn") ;; imm1, start
     (match_operand:SI    6 "const_int_or_noval_operand" "n,nxn") ;; imm2, len
     (match_operand:SI    7 "const_int_operand" "n,n") ;; exec-while-load
     (match_operand:SI    8 "const_int_operand" "n,n") ;; load
     (match_operand:SI    9 "const_int_operand" "n,n") ;; last
     (match_operand:SI   10 "const_int_operand" "n,n") ;; set mutex
     ] UNSPECV_TTREPLAY)]
  "TARGET_XTT_TENSIX"
  {
    return rvtt_synth::pattern (which_alternative,
      TARGET_XTT_TENSIX_QSR ? "TTREPLAY\t%5, %6, %9, %10, %7, %8"
      : "TTREPLAY\t%5, %6, %7, %8",
      operands, false, -1);
  }
  [(set_attr "type" "tensix")])

(define_insn "rvtt_ttincrwc"
  [(unspec_volatile:XTT32SI [
     (match_operand:SI    0 "const_int_operand" "n")
     (match_operand:SI    1 "const_int_operand" "n")
     (match_operand:SI    2 "const_int_operand" "n")
     (match_operand:SI    3 "const_int_operand" "n")
     ] UNSPECV_TTINCRWC)]
  "TARGET_XTT_TENSIX"
  "TTINCRWC\t%0, %1, %2, %3"
  [(set_attr "type" "tensix")])
