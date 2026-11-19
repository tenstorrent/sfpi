;; Machine description for Tenstorrent ROCC Intrinsics.
;; Copyright (C) 2025 Tenstorrent Inc.

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

;; TT rocc extension

(define_c_enum "unspecv" [
  UNSPECV_TTROCC_NOP
  UNSPECV_TTROCC_NOC_FENCE
  UNSPECV_TTROCC_DBG_POSTCODE
  UNSPECV_TTROCC_LLK_INTF_WRITE  
  UNSPECV_TTROCC_LLK_INTF_READ  
  UNSPECV_TTROCC_FDS_INTF_WRITE  
  UNSPECV_TTROCC_FDS_INTF_READ
  UNSPECV_TTROCC_WR_TILES_TO_PROCESS_THRES_TR_ACK
  UNSPECV_TTROCC_WR_TILES_TO_PROCESS_THRES_WR_SENT
  UNSPECV_TTROCC_WR_TILES_TO_PROCESS_THRES_IDMA_TR_ACK
  UNSPECV_TTROCC_RD_TILES_TO_PROCESS_THRES_TR_ACK
  UNSPECV_TTROCC_RD_TILES_TO_PROCESS_THRES_WR_SENT
  UNSPECV_TTROCC_RD_TILES_TO_PROCESS_THRES_IDMA_TR_ACK
  UNSPECV_TTROCC_CS_ALLOC
  UNSPECV_TTROCC_CS_DEALLOC
  UNSPECV_TTROCC_CS_SAVE
  UNSPECV_TTROCC_CS_RESTORE
  UNSPECV_TTROCC_ADDRGEN_WR_REG
  UNSPECV_TTROCC_ADDRGEN_RD_REG
  UNSPECV_TTROCC_ADDRGEN_RESET
  UNSPECV_TTROCC_ADDRGEN_RESET_COUNTERS
  UNSPECV_TTROCC_ADDRGEN_PEEK_SRC
  UNSPECV_TTROCC_ADDRGEN_POP_SRC
  UNSPECV_TTROCC_ADDRGEN_POP_X_SRC
  UNSPECV_TTROCC_ADDRGEN_PEEK_DEST
  UNSPECV_TTROCC_ADDRGEN_POP_DEST
  UNSPECV_TTROCC_ADDRGEN_POP_X_DEST
  UNSPECV_TTROCC_ADDRGEN_POP_BOTH
  UNSPECV_TTROCC_ADDRGEN_PUSH_SRC
  UNSPECV_TTROCC_ADDRGEN_PUSH_SRC_POP_X
  UNSPECV_TTROCC_ADDRGEN_PUSH_DEST
  UNSPECV_TTROCC_ADDRGEN_PUSH_DEST_POP_X
  UNSPECV_TTROCC_ADDRGEN_PUSH_BOTH
  UNSPECV_TTROCC_ADDRGEN_PUSH_BOTH_POP_X
  UNSPECV_TTROCC_CMDBUF_WR_REG  
  UNSPECV_TTROCC_CMDBUF_RD_REG  
  UNSPECV_TTROCC_CMDBUF_GET_VC_SPACE  
  UNSPECV_TTROCC_CMDBUF_GET_VC_SPACE_VC  
  UNSPECV_TTROCC_CMDBUF_WR_SENT  
  UNSPECV_TTROCC_CMDBUF_WR_SENT_TRID  
  UNSPECV_TTROCC_CMDBUF_TR_ACK  
  UNSPECV_TTROCC_CMDBUF_TR_ACK_TRID  
  UNSPECV_TTROCC_CMDBUF_RESET  
  UNSPECV_TTROCC_CMDBUF_IDMA_GET_VC_SPACE  
  UNSPECV_TTROCC_CMDBUF_IDMA_GET_VC_SPACE_VC  
  UNSPECV_TTROCC_CMDBUF_IDMA_TR_ACK  
  UNSPECV_TTROCC_CMDBUF_IDMA_TR_ACK_TRID  
  UNSPECV_TTROCC_CMDBUF_ISSUE_TRANS  
  UNSPECV_TTROCC_CMDBUF_ISSUE_INLINE_TRANS  
  UNSPECV_TTROCC_CMDBUF_ISSUE_INLINE_ADDR_TRANS  
  UNSPECV_TTROCC_CMDBUF_ISSUE_READ1_TRANS  
  UNSPECV_TTROCC_CMDBUF_ISSUE_READ2_TRANS  
  UNSPECV_TTROCC_CMDBUF_ISSUE_WRITE1_TRANS  
  UNSPECV_TTROCC_CMDBUF_ISSUE_WRITE2_TRANS  
  UNSPECV_TTROCC_CMDBUF_READ_TILES_TO_PROCESS_TR_ACK
  UNSPECV_TTROCC_CMDBUF_READ_TILES_TO_PROCESS_TR_ACK_TR_ID
  UNSPECV_TTROCC_CMDBUF_READ_TILES_TO_PROCESS_WR_SENT
  UNSPECV_TTROCC_CMDBUF_READ_TILES_TO_PROCESS_WR_SENT_TR_ID
  UNSPECV_TTROCC_CMDBUF_READ_TILES_TO_PROCESS_IDMA_TR_ACK
  UNSPECV_TTROCC_CMDBUF_READ_TILES_TO_PROCESS_IDMA_TR_ACK_TR_ID
  UNSPECV_TTROCC_CMDBUF_CLEAR_TILES_TO_PROCESS_TR_ACK
  UNSPECV_TTROCC_CMDBUF_CLEAR_TILES_TO_PROCESS_WR_SENT
  UNSPECV_TTROCC_CMDBUF_CLEAR_TILES_TO_PROCESS_IDMA_TR_ACK
  UNSPECV_TTROCC_SCMDBUF_WR_REG  
  UNSPECV_TTROCC_SCMDBUF_RD_REG  
  UNSPECV_TTROCC_SCMDBUF_GET_VC_SPACE  
  UNSPECV_TTROCC_SCMDBUF_GET_VC_SPACE_VC  
  UNSPECV_TTROCC_SCMDBUF_WR_SENT  
  UNSPECV_TTROCC_SCMDBUF_WR_SENT_TRID  
  UNSPECV_TTROCC_SCMDBUF_TR_ACK  
  UNSPECV_TTROCC_SCMDBUF_TR_ACK_TRID  
  UNSPECV_TTROCC_SCMDBUF_RESET  
  UNSPECV_TTROCC_SCMDBUF_ISSUE_TRANS  
  UNSPECV_TTROCC_SCMDBUF_ISSUE_INLINE_TRANS  
  UNSPECV_TTROCC_SCMDBUF_ISSUE_INLINE_ADDR_TRANS  
  UNSPECV_TTROCC_SCMDBUF_ISSUE_READ1_TRANS  
  UNSPECV_TTROCC_SCMDBUF_ISSUE_READ2_TRANS  
  UNSPECV_TTROCC_SCMDBUF_ISSUE_WRITE1_TRANS  
  UNSPECV_TTROCC_SCMDBUF_ISSUE_WRITE2_TRANS  
  UNSPECV_TTROCC_SCMDBUF_READ_TILES_TO_PROCESS_TR_ACK
  UNSPECV_TTROCC_SCMDBUF_READ_TILES_TO_PROCESS_TR_ACK_TR_ID
  UNSPECV_TTROCC_SCMDBUF_READ_TILES_TO_PROCESS_WR_SENT
  UNSPECV_TTROCC_SCMDBUF_READ_TILES_TO_PROCESS_WR_SENT_TR_ID
  UNSPECV_TTROCC_SCMDBUF_READ_TILES_TO_PROCESS_IDMA_TR_ACK
  UNSPECV_TTROCC_SCMDBUF_READ_TILES_TO_PROCESS_IDMA_TR_ACK_TR_ID
  UNSPECV_TTROCC_SCMDBUF_CLEAR_TILES_TO_PROCESS_TR_ACK
  UNSPECV_TTROCC_SCMDBUF_CLEAR_TILES_TO_PROCESS_WR_SENT
  UNSPECV_TTROCC_SCMDBUF_CLEAR_TILES_TO_PROCESS_IDMA_TR_ACK
])

(define_enum "xtt_rocc_addrgen" [
  no  ; not an addrgem
  yes  ; an addrgen, if a set, requires barrier before next addrgen, unless value used
  push ; requires barrier before next addrgen to same generator
])

(define_enum_attr "xtt_rocc_addrgen" "xtt_rocc_addrgen" (const_string "no"))

(define_insn "riscv_ttrocc_nop"
  [(unspec_volatile:DI [(const_int 0)] UNSPECV_TTROCC_NOP)]
  "TARGET_XTT_ROCC_QSR"
  "tt.rocc.nop"
  [(set_attr "type" "ttrocc")])

(define_insn "riscv_ttrocc_noc_fence"
  [(unspec_volatile:DI [(const_int 0)] UNSPECV_TTROCC_NOC_FENCE)]
  "TARGET_XTT_ROCC_QSR"
  "tt.rocc.noc_fence"
  [(set_attr "type" "ttrocc")])

(define_insn "riscv_ttrocc_dbg_postcode"
  [(unspec_volatile:DI [(match_operand:DI 0 "register_operand" "r")] UNSPECV_TTROCC_DBG_POSTCODE)]
  "TARGET_XTT_ROCC_QSR"
  "tt.rocc.dbg_postcode\t%0"
  [(set_attr "type" "ttrocc")])

(define_insn "riscv_ttrocc_llk_intf_write"
  [(unspec_volatile:DI [(match_operand:DI 0 "register_operand" "r")
                        (match_operand:DI 1 "register_operand" "r")] UNSPECV_TTROCC_LLK_INTF_WRITE)]
  "TARGET_XTT_ROCC_QSR"
  "tt.rocc.llk_intf_write\t%0,%1"
  [(set_attr "type" "ttrocc")])

(define_insn "riscv_ttrocc_llk_intf_read"
  [(set (match_operand:DI 0 "register_operand" "=r")
        (unspec_volatile:DI [(match_operand:DI 1 "register_operand" "r")] UNSPECV_TTROCC_LLK_INTF_READ))]
  "TARGET_XTT_ROCC_QSR"
  "tt.rocc.llk_intf_read\t%0,%1"
  [(set_attr "type" "ttrocc")])

(define_insn "riscv_ttrocc_fds_intf_write"
  [(unspec_volatile:DI [(match_operand:DI 0 "register_operand" "r")
                        (match_operand:DI 1 "register_operand" "r")] UNSPECV_TTROCC_FDS_INTF_WRITE)]
  "TARGET_XTT_ROCC_QSR"
  "tt.rocc.fds_intf_write\t%0,%1"
  [(set_attr "type" "ttrocc")])

(define_insn "riscv_ttrocc_fds_intf_read"
  [(set (match_operand:DI 0 "register_operand" "=r")
        (unspec_volatile:DI [(match_operand:DI 1 "register_operand" "r")] UNSPECV_TTROCC_FDS_INTF_READ))]
  "TARGET_XTT_ROCC_QSR"
  "tt.rocc.fds_intf_read\t%0,%1"
  [(set_attr "type" "ttrocc")])

(define_insn "riscv_ttrocc_wr_tiles_to_process_thres_tr_ack"
  [(unspec_volatile:DI [(match_operand:DI 0 "register_operand" "r")
                        (match_operand:DI 1 "register_operand" "r")] UNSPECV_TTROCC_WR_TILES_TO_PROCESS_THRES_TR_ACK)]
  "TARGET_XTT_ROCC_QSR"
  "tt.rocc.wr_tiles_to_process_thres_tr_ack\t%0,%1"
  [(set_attr "type" "ttrocc")])

(define_insn "riscv_ttrocc_wr_tiles_to_process_thres_wr_sent"
  [(unspec_volatile:DI [(match_operand:DI 0 "register_operand" "r")
                        (match_operand:DI 1 "register_operand" "r")] UNSPECV_TTROCC_WR_TILES_TO_PROCESS_THRES_WR_SENT)]
  "TARGET_XTT_ROCC_QSR"
  "tt.rocc.wr_tiles_to_process_thres_wr_sent\t%0,%1"
  [(set_attr "type" "ttrocc")])

(define_insn "riscv_ttrocc_wr_tiles_to_process_thres_idma_tr_ack"
  [(unspec_volatile:DI [(match_operand:DI 0 "register_operand" "r")
                        (match_operand:DI 1 "register_operand" "r")] UNSPECV_TTROCC_WR_TILES_TO_PROCESS_THRES_IDMA_TR_ACK)]
  "TARGET_XTT_ROCC_QSR"
  "tt.rocc.wr_tiles_to_process_thres_idma_tr_ack\t%0,%1"
  [(set_attr "type" "ttrocc")])

(define_insn "riscv_ttrocc_rd_tiles_to_process_thres_tr_ack"
  [(set (match_operand:DI 0 "register_operand" "=r")
        (unspec_volatile:DI [(match_operand:DI 1 "register_operand" "r")] UNSPECV_TTROCC_RD_TILES_TO_PROCESS_THRES_TR_ACK))]
  "TARGET_XTT_ROCC_QSR"
  "tt.rocc.rd_tiles_to_process_thres_tr_ack\t%0,%1"
  [(set_attr "type" "ttrocc")])

(define_insn "riscv_ttrocc_rd_tiles_to_process_thres_wr_sent"
  [(set (match_operand:DI 0 "register_operand" "=r")
        (unspec_volatile:DI [(match_operand:DI 1 "register_operand" "r")] UNSPECV_TTROCC_RD_TILES_TO_PROCESS_THRES_WR_SENT))]
  "TARGET_XTT_ROCC_QSR"
  "tt.rocc.rd_tiles_to_process_thres_wr_sent\t%0,%1"
  [(set_attr "type" "ttrocc")])

(define_insn "riscv_ttrocc_rd_tiles_to_process_thres_idma_tr_ack"
  [(set (match_operand:DI 0 "register_operand" "=r")
        (unspec_volatile:DI [(match_operand:DI 1 "register_operand" "r")] UNSPECV_TTROCC_RD_TILES_TO_PROCESS_THRES_IDMA_TR_ACK))]
  "TARGET_XTT_ROCC_QSR"
  "tt.rocc.rd_tiles_to_process_thres_idma_tr_ack\t%0,%1"
  [(set_attr "type" "ttrocc")])

(define_insn "riscv_ttrocc_cs_alloc"
  [(set (match_operand:DI 0 "register_operand" "=r")
        (unspec_volatile:DI [(const_int 0)] UNSPECV_TTROCC_CS_ALLOC))]
  "TARGET_XTT_ROCC_QSR"
  "tt.rocc.cs_alloc\t%0"
  [(set_attr "type" "ttrocc")])

(define_insn "riscv_ttrocc_cs_dealloc"
  [(unspec_volatile:DI [(match_operand:DI 0 "register_operand" "r")] UNSPECV_TTROCC_CS_DEALLOC)]
  "TARGET_XTT_ROCC_QSR"
  "tt.rocc.cs_dealloc\t%0"
  [(set_attr "type" "ttrocc")])

(define_insn "riscv_ttrocc_cs_save"
  [(unspec_volatile:DI [(match_operand:DI 0 "register_operand" "r")] UNSPECV_TTROCC_CS_SAVE)]
  "TARGET_XTT_ROCC_QSR"
  "tt.rocc.cs_save\t%0"
  [(set_attr "type" "ttrocc")])

(define_insn "riscv_ttrocc_cs_restore"
  [(unspec_volatile:DI [(match_operand:DI 0 "register_operand" "r")] UNSPECV_TTROCC_CS_RESTORE)]
  "TARGET_XTT_ROCC_QSR"
  "tt.rocc.cs_restore\t%0"
  [(set_attr "type" "ttrocc")])

(define_insn "riscv_ttrocc_addrgen_wr_reg"
  [(unspec_volatile:DI [(match_operand:DI 0 "const_int_operand" "n")
                        (match_operand:DI 1 "const_int_operand" "n")
                        (match_operand:DI 2 "register_operand" "r")] UNSPECV_TTROCC_ADDRGEN_WR_REG)]
  "TARGET_XTT_ROCC_QSR"
  ;; We hardcode two extra unused registers per the HW engineers' request
  "tt.rocc.addrgen_wr_reg\tx0,%0,%1,%2,x0"
  [(set_attr "type" "ttrocc")
   (set_attr "xtt_rocc_addrgen" "yes")])

(define_insn "riscv_ttrocc_addrgen_rd_reg"
  [(set (match_operand:DI 0 "register_operand" "=r")
        (unspec_volatile:DI [(match_operand:DI 1 "const_int_operand" "n")
                             (match_operand:DI 2 "const_int_operand" "n")] UNSPECV_TTROCC_ADDRGEN_RD_REG))]
  "TARGET_XTT_ROCC_QSR"
  ;; We hardcode two extra unused registers per the HW engineers' request
  "tt.rocc.addrgen_rd_reg\t%0,%1,%2,x0,x0"
  [(set_attr "type" "ttrocc")
   (set_attr "xtt_rocc_addrgen" "yes")])

(define_insn "riscv_ttrocc_addrgen_reset"
  [(unspec_volatile:DI [(match_operand:DI 0 "const_int_operand" "n")] UNSPECV_TTROCC_ADDRGEN_RESET)]
  "TARGET_XTT_ROCC_QSR"
  "tt.rocc.addrgen_reset\t%0"
  [(set_attr "type" "ttrocc")
   (set_attr "xtt_rocc_addrgen" "yes")])

(define_insn "riscv_ttrocc_addrgen_reset_counters"
  [(unspec_volatile:DI [(match_operand:DI 0 "const_int_operand" "n")] UNSPECV_TTROCC_ADDRGEN_RESET_COUNTERS)]
  "TARGET_XTT_ROCC_QSR"
  ;; We use an extra register here per the spec, but hardcode it to zero
  "tt.rocc.addrgen_reset_counters\t%0,x0"
  [(set_attr "type" "ttrocc")
   (set_attr "xtt_rocc_addrgen" "yes")])

(define_insn "riscv_ttrocc_addrgen_peek_src"
  [(set (match_operand:DI 0 "register_operand" "=r")
        (unspec_volatile:DI [(match_operand:DI 1 "const_int_operand" "n")] UNSPECV_TTROCC_ADDRGEN_PEEK_SRC))]
  "TARGET_XTT_ROCC_QSR"
  "tt.rocc.addrgen_peek_src\t%0,%1"
  [(set_attr "type" "ttrocc")
   (set_attr "xtt_rocc_addrgen" "yes")])

(define_expand "riscv_ttrocc_addrgen_pop_src"
  [(set (match_operand:DI 0 "register_operand")
        (unspec_volatile:DI [(match_operand:DI 1 "const_int_operand")] UNSPECV_TTROCC_ADDRGEN_POP_SRC))]
  "TARGET_XTT_ROCC_QSR"
  {
    if (riscv_tt_fix_qsr_rocc > 0)
      {
        rtx one = gen_reg_rtx (DImode);
	emit_move_insn (one, GEN_INT (1));
	emit_insn (gen_riscv_ttrocc_addrgen_pop_x_src
	  (operands[0], operands[1], one));
        DONE;
      }
  })

(define_insn "*riscv_ttrocc_addrgen_pop_src"
  [(set (match_operand:DI 0 "register_operand" "=r")
        (unspec_volatile:DI [(match_operand:DI 1 "const_int_operand" "n")] UNSPECV_TTROCC_ADDRGEN_POP_SRC))]
  "TARGET_XTT_ROCC_QSR && riscv_tt_fix_qsr_rocc <= 0"
  "tt.rocc.addrgen_pop_src\t%0,%1"
  [(set_attr "type" "ttrocc")
   (set_attr "xtt_rocc_addrgen" "yes")])

(define_insn "riscv_ttrocc_addrgen_pop_x_src"
  [(set (match_operand:DI 0 "register_operand" "=r")
        (unspec_volatile:DI [(match_operand:DI 1 "const_int_operand" "n")
                             (match_operand:DI 2 "register_operand" "r")] UNSPECV_TTROCC_ADDRGEN_POP_X_SRC))]
  "TARGET_XTT_ROCC_QSR"
  "tt.rocc.addrgen_pop_x_src\t%0,%1,%2"
  [(set_attr "type" "ttrocc")
   (set_attr "xtt_rocc_addrgen" "yes")])

(define_insn "riscv_ttrocc_addrgen_peek_dest"
  [(set (match_operand:DI 0 "register_operand" "=r")
        (unspec_volatile:DI [(match_operand:DI 1 "const_int_operand" "n")] UNSPECV_TTROCC_ADDRGEN_PEEK_DEST))]
  "TARGET_XTT_ROCC_QSR"
  "tt.rocc.addrgen_peek_dest\t%0,%1"
  [(set_attr "type" "ttrocc")
   (set_attr "xtt_rocc_addrgen" "yes")])

(define_expand "riscv_ttrocc_addrgen_pop_dest"
  [(set (match_operand:DI 0 "register_operand")
        (unspec_volatile:DI [(match_operand:DI 1 "const_int_operand")] UNSPECV_TTROCC_ADDRGEN_POP_DEST))]
  "TARGET_XTT_ROCC_QSR"
  {
    if (riscv_tt_fix_qsr_rocc > 0)
      {
        rtx one = gen_reg_rtx (DImode);
	emit_move_insn (one, GEN_INT (1));
	emit_insn (gen_riscv_ttrocc_addrgen_pop_x_dest
	  (operands[0], operands[1], one));
        DONE;
      }
  })

(define_insn "*riscv_ttrocc_addrgen_pop_dest"
  [(set (match_operand:DI 0 "register_operand" "=r")
        (unspec_volatile:DI [(match_operand:DI 1 "const_int_operand" "n")] UNSPECV_TTROCC_ADDRGEN_POP_DEST))]
  "TARGET_XTT_ROCC_QSR && riscv_tt_fix_qsr_rocc <= 0"
  "tt.rocc.addrgen_pop_dest\t%0,%1"
  [(set_attr "type" "ttrocc")
   (set_attr "xtt_rocc_addrgen" "yes")])

(define_insn "riscv_ttrocc_addrgen_pop_x_dest"
  [(set (match_operand:DI 0 "register_operand" "=r")
        (unspec_volatile:DI [(match_operand:DI 1 "const_int_operand" "n")
                          (match_operand:DI 2 "register_operand" "r")] UNSPECV_TTROCC_ADDRGEN_POP_X_DEST))]
  "TARGET_XTT_ROCC_QSR"
  "tt.rocc.addrgen_pop_x_dest\t%0,%1,%2"
  [(set_attr "type" "ttrocc")
   (set_attr "xtt_rocc_addrgen" "yes")])

(define_insn "riscv_ttrocc_addrgen_pop_both"
  [(set (match_operand:DI 0 "register_operand" "=r")
        (unspec_volatile:DI [(match_operand:DI 1 "const_int_operand" "n")
                             (match_operand:DI 2 "register_operand" "r")
                             (match_operand:DI 3 "register_operand" "r")] UNSPECV_TTROCC_ADDRGEN_POP_BOTH))]
  "TARGET_XTT_ROCC_QSR"
  "tt.rocc.addrgen_pop_both\t%0,%1,%2,%3"
  [(set_attr "type" "ttrocc")
   (set_attr "xtt_rocc_addrgen" "yes")])

(define_insn "riscv_ttrocc_addrgen_push_src"
  [(unspec_volatile:DI [(match_operand:DI 0 "const_int_operand" "n")] UNSPECV_TTROCC_ADDRGEN_PUSH_SRC)]
  "TARGET_XTT_ROCC_QSR"
  "tt.rocc.addrgen_push_src\t%0"
  [(set_attr "type" "ttrocc")
   (set_attr "xtt_rocc_addrgen" "push")])

(define_insn "riscv_ttrocc_addrgen_push_src_pop_x"
  [(unspec_volatile:DI [(match_operand:DI 0 "const_int_operand" "n")
                     (match_operand:DI 1 "register_operand" "r")] UNSPECV_TTROCC_ADDRGEN_PUSH_SRC_POP_X)]
  "TARGET_XTT_ROCC_QSR"
  "tt.rocc.addrgen_push_src_pop_x\t%0,%1"
  [(set_attr "type" "ttrocc")
   (set_attr "xtt_rocc_addrgen" "push")])

(define_insn "riscv_ttrocc_addrgen_push_dest"
  [(unspec_volatile:DI [(match_operand:DI 0 "const_int_operand" "n")] UNSPECV_TTROCC_ADDRGEN_PUSH_DEST)]
  "TARGET_XTT_ROCC_QSR"
  "tt.rocc.addrgen_push_dest\t%0"
  [(set_attr "type" "ttrocc")
   (set_attr "xtt_rocc_addrgen" "push")])

(define_insn "riscv_ttrocc_addrgen_push_dest_pop_x"
  [(unspec_volatile:DI [(match_operand:DI 0 "const_int_operand" "n")
                     (match_operand:DI 1 "register_operand" "r")] UNSPECV_TTROCC_ADDRGEN_PUSH_DEST_POP_X)]
  "TARGET_XTT_ROCC_QSR"
  "tt.rocc.addrgen_push_dest_pop_x\t%0,%1"
  [(set_attr "type" "ttrocc")
   (set_attr "xtt_rocc_addrgen" "push")])

(define_insn "riscv_ttrocc_addrgen_push_both"
  [(unspec_volatile:DI [(match_operand:DI 0 "const_int_operand" "n")] UNSPECV_TTROCC_ADDRGEN_PUSH_BOTH)]
  "TARGET_XTT_ROCC_QSR"
  "tt.rocc.addrgen_push_both\t%0"
  [(set_attr "type" "ttrocc")
   (set_attr "xtt_rocc_addrgen" "push")])

(define_insn "riscv_ttrocc_addrgen_push_both_pop_x"
  [(unspec_volatile:DI [(match_operand:DI 0 "const_int_operand" "n")
                     (match_operand:DI 1 "register_operand" "r")
                     (match_operand:DI 2 "register_operand" "r")] UNSPECV_TTROCC_ADDRGEN_PUSH_BOTH_POP_X)]
  "TARGET_XTT_ROCC_QSR"
  "tt.rocc.addrgen_push_both_pop_x\t%0,%1,%2"
  [(set_attr "type" "ttrocc")
   (set_attr "xtt_rocc_addrgen" "push")])

(define_insn "riscv_ttrocc_cmdbuf_wr_reg"
  [(unspec_volatile:DI [(match_operand:DI 0 "const_int_operand" "n")
                     (match_operand:DI 1 "const_int_operand" "n")
                     (match_operand:DI 2 "register_operand" "r")] UNSPECV_TTROCC_CMDBUF_WR_REG)]
  "TARGET_XTT_ROCC_QSR"
  ;; We hardcode two extra unused registers per the HW engineers' request
  "tt.rocc.cmdbuf_wr_reg\tx0,%0,%1,%2,x0"
  [(set_attr "type" "ttrocc")])
    
(define_insn "riscv_ttrocc_cmdbuf_rd_reg"
  [(set (match_operand:DI 0 "register_operand" "=r")
        (unspec_volatile:DI [(match_operand:DI 1 "const_int_operand" "n")
                             (match_operand:DI 2 "const_int_operand" "n")] UNSPECV_TTROCC_CMDBUF_RD_REG))]
  "TARGET_XTT_ROCC_QSR"
  ;; We hardcode two extra unused registers per the HW engineers' request
  "tt.rocc.cmdbuf_rd_reg\t%0,%1,%2,x0,x0"
  [(set_attr "type" "ttrocc")])

(define_insn "riscv_ttrocc_cmdbuf_get_vc_space"
  [(set (match_operand:DI 0 "register_operand" "=r")
        (unspec_volatile:DI [(match_operand:DI 1 "const_int_operand" "n")] UNSPECV_TTROCC_CMDBUF_GET_VC_SPACE))]
  "TARGET_XTT_ROCC_QSR"
  "tt.rocc.cmdbuf_get_vc_space\t%0,%1"
  [(set_attr "type" "ttrocc")])
   
(define_insn "riscv_ttrocc_cmdbuf_get_vc_space_vc"
  [(set (match_operand:DI 0 "register_operand" "=r")
        (unspec_volatile:DI [(match_operand:DI 1 "const_int_operand" "n")
                             (match_operand:DI 2 "register_operand" "r")] UNSPECV_TTROCC_CMDBUF_GET_VC_SPACE_VC))]
  "TARGET_XTT_ROCC_QSR"
  "tt.rocc.cmdbuf_get_vc_space_vc\t%0,%1,%2"
  [(set_attr "type" "ttrocc")])
   
(define_insn "riscv_ttrocc_cmdbuf_wr_sent"
  [(set (match_operand:DI 0 "register_operand" "=r")
        (unspec_volatile:DI [(match_operand:DI 1 "const_int_operand" "n")] UNSPECV_TTROCC_CMDBUF_WR_SENT))]
  "TARGET_XTT_ROCC_QSR"
  "tt.rocc.cmdbuf_wr_sent\t%0,%1"
  [(set_attr "type" "ttrocc")])
   
(define_insn "riscv_ttrocc_cmdbuf_wr_sent_trid"
  [(set (match_operand:DI 0 "register_operand" "=r")
        (unspec_volatile:DI [(match_operand:DI 1 "const_int_operand" "n")
                             (match_operand:DI 2 "register_operand" "r")] UNSPECV_TTROCC_CMDBUF_WR_SENT_TRID))]
  "TARGET_XTT_ROCC_QSR"
  "tt.rocc.cmdbuf_wr_sent_trid\t%0,%1,%2"
  [(set_attr "type" "ttrocc")])
   
(define_insn "riscv_ttrocc_cmdbuf_tr_ack"
  [(set (match_operand:DI 0 "register_operand" "=r")
        (unspec_volatile:DI [(match_operand:DI 1 "const_int_operand" "n")] UNSPECV_TTROCC_CMDBUF_TR_ACK))]
  "TARGET_XTT_ROCC_QSR"
  "tt.rocc.cmdbuf_tr_ack\t%0,%1"
  [(set_attr "type" "ttrocc")])
   
(define_insn "riscv_ttrocc_cmdbuf_tr_ack_trid"
  [(set (match_operand:DI 0 "register_operand" "=r")
        (unspec_volatile:DI [(match_operand:DI 1 "const_int_operand" "n")
                             (match_operand:DI 2 "register_operand" "r")] UNSPECV_TTROCC_CMDBUF_TR_ACK_TRID))]
  "TARGET_XTT_ROCC_QSR"
  "tt.rocc.cmdbuf_tr_ack_trid\t%0,%1,%2"
  [(set_attr "type" "ttrocc")])

(define_insn "riscv_ttrocc_cmdbuf_reset"
  [(unspec_volatile:DI [(match_operand:DI 0 "const_int_operand" "n")] UNSPECV_TTROCC_CMDBUF_RESET)]
  "TARGET_XTT_ROCC_QSR"
  "tt.rocc.cmdbuf_reset\t%0"
  [(set_attr "type" "ttrocc")])

(define_insn "riscv_ttrocc_cmdbuf_idma_get_vc_space"
  [(set (match_operand:DI 0 "register_operand" "=r")
        (unspec_volatile:DI [(match_operand:DI 1 "const_int_operand" "n")] UNSPECV_TTROCC_CMDBUF_IDMA_GET_VC_SPACE))]
  "TARGET_XTT_ROCC_QSR"
  "tt.rocc.cmdbuf_idma_get_vc_space\t%0,%1"
  [(set_attr "type" "ttrocc")])
   
(define_insn "riscv_ttrocc_cmdbuf_idma_get_vc_space_vc"
  [(set (match_operand:DI 0 "register_operand" "=r")
        (unspec_volatile:DI [(match_operand:DI 1 "const_int_operand" "n")
                             (match_operand:DI 2 "register_operand" "r")] UNSPECV_TTROCC_CMDBUF_IDMA_GET_VC_SPACE_VC))]
  "TARGET_XTT_ROCC_QSR"
  "tt.rocc.cmdbuf_idma_get_vc_space_vc\t%0,%1,%2"
  [(set_attr "type" "ttrocc")])
   
(define_insn "riscv_ttrocc_cmdbuf_idma_tr_ack"
  [(set (match_operand:DI 0 "register_operand" "=r")
        (unspec_volatile:DI [(match_operand:DI 1 "const_int_operand" "n")] UNSPECV_TTROCC_CMDBUF_IDMA_TR_ACK))]
  "TARGET_XTT_ROCC_QSR"
  "tt.rocc.cmdbuf_idma_tr_ack\t%0,%1"
  [(set_attr "type" "ttrocc")])
   
(define_insn "riscv_ttrocc_cmdbuf_idma_tr_ack_trid"
  [(set (match_operand:DI 0 "register_operand" "=r")
        (unspec_volatile:DI [(match_operand:DI 1 "const_int_operand" "n")
                             (match_operand:DI 2 "register_operand" "r")] UNSPECV_TTROCC_CMDBUF_IDMA_TR_ACK_TRID))]
  "TARGET_XTT_ROCC_QSR"
  "tt.rocc.cmdbuf_idma_tr_ack_trid\t%0,%1,%2"
  [(set_attr "type" "ttrocc")])

(define_insn "riscv_ttrocc_cmdbuf_issue_trans"
  [(unspec_volatile:DI [(match_operand:DI 0 "const_int_operand" "n")] UNSPECV_TTROCC_CMDBUF_ISSUE_TRANS)]
  "TARGET_XTT_ROCC_QSR"
  "tt.rocc.cmdbuf_issue_trans\t%0"
  [(set_attr "type" "ttrocc")])

(define_insn "riscv_ttrocc_cmdbuf_issue_inline_trans"
  [(unspec_volatile:DI [(match_operand:DI 0 "const_int_operand" "n")
                     (match_operand:DI 1 "register_operand" "r")] UNSPECV_TTROCC_CMDBUF_ISSUE_INLINE_TRANS)]
  "TARGET_XTT_ROCC_QSR"
  "tt.rocc.cmdbuf_issue_inline_trans\t%0,%1"
  [(set_attr "type" "ttrocc")])

(define_insn "riscv_ttrocc_cmdbuf_issue_inline_addr_trans"
  [(unspec_volatile:DI [(match_operand:DI 0 "const_int_operand" "n")
                     (match_operand:DI 1 "register_operand" "r")
                     (match_operand:DI 2 "register_operand" "r")] UNSPECV_TTROCC_CMDBUF_ISSUE_INLINE_ADDR_TRANS)]
  "TARGET_XTT_ROCC_QSR"
  "tt.rocc.cmdbuf_issue_inline_addr_trans\t%0,%1,%2"
  [(set_attr "type" "ttrocc")])

(define_insn "riscv_ttrocc_cmdbuf_issue_read1_trans"
  [(unspec_volatile:DI [(match_operand:DI 0 "const_int_operand" "n")
                     (match_operand:DI 1 "register_operand" "r")] UNSPECV_TTROCC_CMDBUF_ISSUE_READ1_TRANS)]
  "TARGET_XTT_ROCC_QSR"
  "tt.rocc.cmdbuf_issue_read1_trans\t%0,%1"
  [(set_attr "type" "ttrocc")])

(define_insn "riscv_ttrocc_cmdbuf_issue_read2_trans"
  [(unspec_volatile:DI [(match_operand:DI 0 "const_int_operand" "n")
                     (match_operand:DI 1 "register_operand" "r")
                     (match_operand:DI 2 "register_operand" "r")] UNSPECV_TTROCC_CMDBUF_ISSUE_READ2_TRANS)]
  "TARGET_XTT_ROCC_QSR"
  "tt.rocc.cmdbuf_issue_read2_trans\t%0,%1,%2"
  [(set_attr "type" "ttrocc")])

(define_insn "riscv_ttrocc_cmdbuf_issue_write1_trans"
  [(unspec_volatile:DI [(match_operand:DI 0 "const_int_operand" "n")
                     (match_operand:DI 1 "register_operand" "r")] UNSPECV_TTROCC_CMDBUF_ISSUE_WRITE1_TRANS)]
  "TARGET_XTT_ROCC_QSR"
  "tt.rocc.cmdbuf_issue_write1_trans\t%0,%1"
  [(set_attr "type" "ttrocc")])

(define_insn "riscv_ttrocc_cmdbuf_issue_write2_trans"
  [(unspec_volatile:DI [(match_operand:DI 0 "const_int_operand" "n")
                     (match_operand:DI 1 "register_operand" "r")
                     (match_operand:DI 2 "register_operand" "r")] UNSPECV_TTROCC_CMDBUF_ISSUE_WRITE2_TRANS)]
  "TARGET_XTT_ROCC_QSR"
  "tt.rocc.cmdbuf_issue_write2_trans\t%0,%1,%2"
  [(set_attr "type" "ttrocc")])

(define_insn "riscv_ttrocc_cmdbuf_read_tiles_to_process_tr_ack"
  [(set (match_operand:DI 0 "register_operand" "=r")
        (unspec_volatile:DI [(match_operand:DI 1 "const_int_operand" "n")] UNSPECV_TTROCC_CMDBUF_READ_TILES_TO_PROCESS_TR_ACK))]
  "TARGET_XTT_ROCC_QSR"
  "tt.rocc.cmdbuf_read_tiles_to_process_tr_ack\t%0,%1"
  [(set_attr "type" "ttrocc")])

(define_insn "riscv_ttrocc_cmdbuf_read_tiles_to_process_tr_ack_tr_id"
  [(set (match_operand:DI 0 "register_operand" "=r")
        (unspec_volatile:DI [(match_operand:DI 1 "const_int_operand" "n")
                             (match_operand:DI 2 "register_operand" "r")] UNSPECV_TTROCC_CMDBUF_READ_TILES_TO_PROCESS_TR_ACK_TR_ID))]
  "TARGET_XTT_ROCC_QSR"
  "tt.rocc.cmdbuf_read_tiles_to_process_tr_ack_tr_id\t%0,%1,%2"
  [(set_attr "type" "ttrocc")])

(define_insn "riscv_ttrocc_cmdbuf_read_tiles_to_process_wr_sent"
  [(set (match_operand:DI 0 "register_operand" "=r")
        (unspec_volatile:DI [(match_operand:DI 1 "const_int_operand" "n")] UNSPECV_TTROCC_CMDBUF_READ_TILES_TO_PROCESS_WR_SENT))]
  "TARGET_XTT_ROCC_QSR"
  "tt.rocc.cmdbuf_read_tiles_to_process_wr_sent\t%0,%1"
  [(set_attr "type" "ttrocc")])

(define_insn "riscv_ttrocc_cmdbuf_read_tiles_to_process_wr_sent_tr_id"
  [(set (match_operand:DI 0 "register_operand" "=r")
        (unspec_volatile:DI [(match_operand:DI 1 "const_int_operand" "n")
                             (match_operand:DI 2 "register_operand" "r")] UNSPECV_TTROCC_CMDBUF_READ_TILES_TO_PROCESS_WR_SENT_TR_ID))]
  "TARGET_XTT_ROCC_QSR"
  "tt.rocc.cmdbuf_read_tiles_to_process_wr_sent_tr_id\t%0,%1,%2"
  [(set_attr "type" "ttrocc")])

(define_insn "riscv_ttrocc_cmdbuf_read_tiles_to_process_idma_tr_ack"
  [(set (match_operand:DI 0 "register_operand" "=r")
        (unspec_volatile:DI [(match_operand:DI 1 "const_int_operand" "n")] UNSPECV_TTROCC_CMDBUF_READ_TILES_TO_PROCESS_IDMA_TR_ACK))]
  "TARGET_XTT_ROCC_QSR"
  "tt.rocc.cmdbuf_read_tiles_to_process_idma_tr_ack\t%0,%1"
  [(set_attr "type" "ttrocc")])

(define_insn "riscv_ttrocc_cmdbuf_read_tiles_to_process_idma_tr_ack_tr_id"
  [(set (match_operand:DI 0 "register_operand" "=r")
        (unspec_volatile:DI [(match_operand:DI 1 "const_int_operand" "n")
                             (match_operand:DI 2 "register_operand" "r")] UNSPECV_TTROCC_CMDBUF_READ_TILES_TO_PROCESS_IDMA_TR_ACK_TR_ID))]
  "TARGET_XTT_ROCC_QSR"
  "tt.rocc.cmdbuf_read_tiles_to_process_idma_tr_ack_tr_id\t%0,%1,%2"
  [(set_attr "type" "ttrocc")])

(define_insn "riscv_ttrocc_cmdbuf_clear_tiles_to_process_tr_ack"
  [(unspec_volatile:DI [(match_operand:DI 0 "const_int_operand" "n")
                     (match_operand:DI 1 "register_operand" "r")] UNSPECV_TTROCC_CMDBUF_CLEAR_TILES_TO_PROCESS_TR_ACK)]
  "TARGET_XTT_ROCC_QSR"
  "tt.rocc.cmdbuf_clear_tiles_to_process_tr_ack\t%0,%1"
  [(set_attr "type" "ttrocc")])

(define_insn "riscv_ttrocc_cmdbuf_clear_tiles_to_process_wr_sent"
  [(unspec_volatile:DI [(match_operand:DI 0 "const_int_operand" "n")
                     (match_operand:DI 1 "register_operand" "r")] UNSPECV_TTROCC_CMDBUF_CLEAR_TILES_TO_PROCESS_WR_SENT)]
  "TARGET_XTT_ROCC_QSR"
  "tt.rocc.cmdbuf_clear_tiles_to_process_wr_sent\t%0,%1"
  [(set_attr "type" "ttrocc")])

(define_insn "riscv_ttrocc_cmdbuf_clear_tiles_to_process_idma_tr_ack"
  [(unspec_volatile:DI [(match_operand:DI 0 "const_int_operand" "n")
                     (match_operand:DI 1 "register_operand" "r")] UNSPECV_TTROCC_CMDBUF_CLEAR_TILES_TO_PROCESS_IDMA_TR_ACK)]
  "TARGET_XTT_ROCC_QSR"
  "tt.rocc.cmdbuf_clear_tiles_to_process_idma_tr_ack\t%0,%1"
  [(set_attr "type" "ttrocc")])

(define_insn "riscv_ttrocc_scmdbuf_wr_reg"
  [(unspec_volatile:DI [(match_operand:DI 0 "const_int_operand" "n")
                     (match_operand:DI 1 "register_operand" "r")] UNSPECV_TTROCC_SCMDBUF_WR_REG)]
  "TARGET_XTT_ROCC_QSR"
  ;; We hardcode two extra unused registers per the HW engineers' request
  "tt.rocc.scmdbuf_wr_reg\tx0,%0,%1,x0"
  [(set_attr "type" "ttrocc")])
    
(define_insn "riscv_ttrocc_scmdbuf_rd_reg"
  [(set (match_operand:DI 0 "register_operand" "=r")
        (unspec_volatile:DI [(match_operand:DI 1 "const_int_operand" "n")] UNSPECV_TTROCC_SCMDBUF_RD_REG))]
  "TARGET_XTT_ROCC_QSR"
  ;; We hardcode two extra unused registers per the HW engineers' request
  "tt.rocc.scmdbuf_rd_reg\t%0,%1,x0,x0"
  [(set_attr "type" "ttrocc")])

(define_insn "riscv_ttrocc_scmdbuf_get_vc_space"
  [(set (match_operand:DI 0 "register_operand" "=r")
        (unspec_volatile:DI [(const_int 0)] UNSPECV_TTROCC_SCMDBUF_GET_VC_SPACE))]
  "TARGET_XTT_ROCC_QSR"
  "tt.rocc.scmdbuf_get_vc_space\t%0"
  [(set_attr "type" "ttrocc")])
   
(define_insn "riscv_ttrocc_scmdbuf_get_vc_space_vc"
  [(set (match_operand:DI 0 "register_operand" "=r")
        (unspec_volatile:DI [(match_operand:DI 1 "register_operand" "r")] UNSPECV_TTROCC_SCMDBUF_GET_VC_SPACE_VC))]
  "TARGET_XTT_ROCC_QSR"
  "tt.rocc.scmdbuf_get_vc_space_vc\t%0,%1"
  [(set_attr "type" "ttrocc")])
   
(define_insn "riscv_ttrocc_scmdbuf_wr_sent"
  [(set (match_operand:DI 0 "register_operand" "=r")
        (unspec_volatile:DI [(const_int 0)] UNSPECV_TTROCC_SCMDBUF_WR_SENT))]
  "TARGET_XTT_ROCC_QSR"
  "tt.rocc.scmdbuf_wr_sent\t%0"
  [(set_attr "type" "ttrocc")])
   
(define_insn "riscv_ttrocc_scmdbuf_wr_sent_trid"
  [(set (match_operand:DI 0 "register_operand" "=r")
        (unspec_volatile:DI [(match_operand:DI 1 "register_operand" "r")] UNSPECV_TTROCC_SCMDBUF_WR_SENT_TRID))]
  "TARGET_XTT_ROCC_QSR"
  "tt.rocc.scmdbuf_wr_sent_trid\t%0,%1"
  [(set_attr "type" "ttrocc")])
   
(define_insn "riscv_ttrocc_scmdbuf_tr_ack"
  [(set (match_operand:DI 0 "register_operand" "=r")
        (unspec_volatile:DI [(const_int 0)] UNSPECV_TTROCC_SCMDBUF_TR_ACK))]
  "TARGET_XTT_ROCC_QSR"
  "tt.rocc.scmdbuf_tr_ack\t%0"
  [(set_attr "type" "ttrocc")])
   
(define_insn "riscv_ttrocc_scmdbuf_tr_ack_trid"
  [(set (match_operand:DI 0 "register_operand" "=r")
        (unspec_volatile:DI [(match_operand:DI 1 "register_operand" "r")] UNSPECV_TTROCC_SCMDBUF_TR_ACK_TRID))]
  "TARGET_XTT_ROCC_QSR"
  "tt.rocc.scmdbuf_tr_ack_trid\t%0,%1"
  [(set_attr "type" "ttrocc")])

(define_insn "riscv_ttrocc_scmdbuf_reset"
  [(unspec_volatile:DI [(const_int 0)] UNSPECV_TTROCC_SCMDBUF_RESET)]
  "TARGET_XTT_ROCC_QSR"
  "tt.rocc.scmdbuf_reset"
  [(set_attr "type" "ttrocc")])

(define_insn "riscv_ttrocc_scmdbuf_issue_trans"
  [(unspec_volatile:DI [(const_int 0)] UNSPECV_TTROCC_SCMDBUF_ISSUE_TRANS)]
  "TARGET_XTT_ROCC_QSR"
  "tt.rocc.scmdbuf_issue_trans"
  [(set_attr "type" "ttrocc")])

(define_insn "riscv_ttrocc_scmdbuf_issue_inline_trans"
  [(unspec_volatile:DI [(match_operand:DI 0 "register_operand" "r")] UNSPECV_TTROCC_SCMDBUF_ISSUE_INLINE_TRANS)]
  "TARGET_XTT_ROCC_QSR"
  "tt.rocc.scmdbuf_issue_inline_trans\t%0"
  [(set_attr "type" "ttrocc")])

(define_insn "riscv_ttrocc_scmdbuf_issue_inline_addr_trans"
  [(unspec_volatile:DI [(match_operand:DI 0 "register_operand" "r")
                     (match_operand:DI 1 "register_operand" "r")] UNSPECV_TTROCC_SCMDBUF_ISSUE_INLINE_ADDR_TRANS)]
  "TARGET_XTT_ROCC_QSR"
  "tt.rocc.scmdbuf_issue_inline_addr_trans\t%0,%1"
  [(set_attr "type" "ttrocc")])

(define_insn "riscv_ttrocc_scmdbuf_issue_read1_trans"
  [(unspec_volatile:DI [(match_operand:DI 0 "register_operand" "r")] UNSPECV_TTROCC_SCMDBUF_ISSUE_READ1_TRANS)]
  "TARGET_XTT_ROCC_QSR"
  "tt.rocc.scmdbuf_issue_read1_trans\t%0"
  [(set_attr "type" "ttrocc")])

(define_insn "riscv_ttrocc_scmdbuf_issue_read2_trans"
  [(unspec_volatile:DI [(match_operand:DI 0 "register_operand" "r")
                     (match_operand:DI 1 "register_operand" "r")] UNSPECV_TTROCC_SCMDBUF_ISSUE_READ2_TRANS)]
  "TARGET_XTT_ROCC_QSR"
  "tt.rocc.scmdbuf_issue_read2_trans\t%0,%1"
  [(set_attr "type" "ttrocc")])

(define_insn "riscv_ttrocc_scmdbuf_issue_write1_trans"
  [(unspec_volatile:DI [(match_operand:DI 0 "register_operand" "r")] UNSPECV_TTROCC_SCMDBUF_ISSUE_WRITE1_TRANS)]
  "TARGET_XTT_ROCC_QSR"
  "tt.rocc.scmdbuf_issue_write1_trans\t%0"
  [(set_attr "type" "ttrocc")])

(define_insn "riscv_ttrocc_scmdbuf_issue_write2_trans"
  [(unspec_volatile:DI [(match_operand:DI 0 "register_operand" "r")
                     (match_operand:DI 1 "register_operand" "r")] UNSPECV_TTROCC_SCMDBUF_ISSUE_WRITE2_TRANS)]
  "TARGET_XTT_ROCC_QSR"
  "tt.rocc.scmdbuf_issue_write2_trans\t%0,%1"
  [(set_attr "type" "ttrocc")])

(define_insn "riscv_ttrocc_scmdbuf_read_tiles_to_process_tr_ack"
  [(set (match_operand:DI 0 "register_operand" "=r")
        (unspec_volatile:DI [(const_int 0)] UNSPECV_TTROCC_SCMDBUF_READ_TILES_TO_PROCESS_TR_ACK))]
  "TARGET_XTT_ROCC_QSR"
  "tt.rocc.scmdbuf_read_tiles_to_process_tr_ack\t%0"
  [(set_attr "type" "ttrocc")])

(define_insn "riscv_ttrocc_scmdbuf_read_tiles_to_process_tr_ack_tr_id"
  [(set (match_operand:DI 0 "register_operand" "=r")
        (unspec_volatile:DI [(match_operand:DI 1 "register_operand" "r")] UNSPECV_TTROCC_SCMDBUF_READ_TILES_TO_PROCESS_TR_ACK_TR_ID))]
  "TARGET_XTT_ROCC_QSR"
  "tt.rocc.scmdbuf_read_tiles_to_process_tr_ack_tr_id\t%0,%1"
  [(set_attr "type" "ttrocc")])

(define_insn "riscv_ttrocc_scmdbuf_read_tiles_to_process_wr_sent"
  [(set (match_operand:DI 0 "register_operand" "=r")
        (unspec_volatile:DI [(const_int 0)]
                    UNSPECV_TTROCC_SCMDBUF_READ_TILES_TO_PROCESS_WR_SENT))]
  "TARGET_XTT_ROCC_QSR"
  "tt.rocc.scmdbuf_read_tiles_to_process_wr_sent\t%0"
  [(set_attr "type" "ttrocc")])

(define_insn "riscv_ttrocc_scmdbuf_read_tiles_to_process_wr_sent_tr_id"
  [(set (match_operand:DI 0 "register_operand" "=r")
        (unspec_volatile:DI [
                     (match_operand:DI 1 "register_operand" "r")] UNSPECV_TTROCC_SCMDBUF_READ_TILES_TO_PROCESS_WR_SENT_TR_ID))]
  "TARGET_XTT_ROCC_QSR"
  "tt.rocc.scmdbuf_read_tiles_to_process_wr_sent_tr_id\t%0,%1"
  [(set_attr "type" "ttrocc")])

(define_insn "riscv_ttrocc_scmdbuf_read_tiles_to_process_idma_tr_ack"
  [(set (match_operand:DI 0 "register_operand" "=r")
        (unspec_volatile:DI [(const_int 0)]
                    UNSPECV_TTROCC_SCMDBUF_READ_TILES_TO_PROCESS_IDMA_TR_ACK))]
  "TARGET_XTT_ROCC_QSR"
  "tt.rocc.scmdbuf_read_tiles_to_process_idma_tr_ack\t%0"
  [(set_attr "type" "ttrocc")])

(define_insn "riscv_ttrocc_scmdbuf_read_tiles_to_process_idma_tr_ack_tr_id"
  [(set (match_operand:DI 0 "register_operand" "=r")
        (unspec_volatile:DI [(match_operand:DI 1 "register_operand" "r")] UNSPECV_TTROCC_SCMDBUF_READ_TILES_TO_PROCESS_IDMA_TR_ACK_TR_ID))]
  "TARGET_XTT_ROCC_QSR"
  "tt.rocc.scmdbuf_read_tiles_to_process_idma_tr_ack_tr_id\t%0,%1"
  [(set_attr "type" "ttrocc")])

(define_insn "riscv_ttrocc_scmdbuf_clear_tiles_to_process_tr_ack"
  [(unspec_volatile:DI [(match_operand:DI 0 "register_operand" "r")] UNSPECV_TTROCC_SCMDBUF_CLEAR_TILES_TO_PROCESS_TR_ACK)]
  "TARGET_XTT_ROCC_QSR"
  "tt.rocc.scmdbuf_clear_tiles_to_process_tr_ack\t%0"
  [(set_attr "type" "ttrocc")])

(define_insn "riscv_ttrocc_scmdbuf_clear_tiles_to_process_wr_sent"
  [(unspec_volatile:DI [(match_operand:DI 0 "register_operand" "r")] UNSPECV_TTROCC_SCMDBUF_CLEAR_TILES_TO_PROCESS_WR_SENT)]
  "TARGET_XTT_ROCC_QSR"
  "tt.rocc.scmdbuf_clear_tiles_to_process_wr_sent\t%0"
  [(set_attr "type" "ttrocc")])

(define_insn "riscv_ttrocc_scmdbuf_clear_tiles_to_process_idma_tr_ack"
  [(unspec_volatile:DI [(match_operand:DI 0 "register_operand" "r")] UNSPECV_TTROCC_SCMDBUF_CLEAR_TILES_TO_PROCESS_IDMA_TR_ACK)]
  "TARGET_XTT_ROCC_QSR"
  "tt.rocc.scmdbuf_clear_tiles_to_process_idma_tr_ack\t%0"
  [(set_attr "type" "ttrocc")])

