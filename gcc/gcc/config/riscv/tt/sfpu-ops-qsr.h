#pragma once

// Machine  generated from ckernel_ops.h
// sed -e '/define TT_OP.*[^\\]$/{s/TT_OP/TT_OP_QSR/g;p}' -e '/define TT_OP.*\\$/,/)$/{s/TT_OP/TT_OP_QSR/g;p}' -e d

#define TT_OP_QSR(opcode, params) ((opcode << 24) + params)
#define TT_OP_QSR_ADDGPR(OpB_is_Const, Result_GPR_Index, OpB_GPR_Index, OpA_GPR_Index) \
    TT_OP_QSR(0x58, (((OpB_is_Const) << 23) + ((Result_GPR_Index) << 12) + ((OpB_GPR_Index) << 6) + ((OpA_GPR_Index) << 0)))
#define TT_OP_QSR_ATCAS(SwapVal, CmpVal, Sel32b, Addr_GPR_Index) TT_OP_QSR(0x64, (((SwapVal) << 18) + ((CmpVal) << 14) + ((Sel32b) << 12) + ((Addr_GPR_Index) << 0)))
#define TT_OP_QSR_ATGETM(mutex_index)                            TT_OP_QSR(0xa0, (((mutex_index) << 0)))
#define TT_OP_QSR_ATINCGET(WrapVal, Sel32b, Data_GPR_Index, Addr_GPR_Index) \
    TT_OP_QSR(0x61, (((WrapVal) << 14) + ((Sel32b) << 12) + ((Data_GPR_Index) << 6) + ((Addr_GPR_Index) << 0)))
#define TT_OP_QSR_ATINCGETPTR(NoIncr, IncrVal, WrapVal, Sel32b, Data_GPR_Index, Addr_GPR_Index) \
    TT_OP_QSR(0x62, (((NoIncr) << 22) + ((IncrVal) << 18) + ((WrapVal) << 14) + ((Sel32b) << 12) + ((Data_GPR_Index) << 6) + ((Addr_GPR_Index) << 0)))
#define TT_OP_QSR_ATRELM(mutex_index)                              TT_OP_QSR(0xa1, (((mutex_index) << 0)))
#define TT_OP_QSR_ATSWAP(SwapMask, Data_GPR_Index, Addr_GPR_Index) TT_OP_QSR(0x63, (((SwapMask) << 14) + ((Data_GPR_Index) << 6) + ((Addr_GPR_Index) << 0)))
#define TT_OP_QSR_BITWOPGPR(OpB_is_Const, OpSel, Result_GPR_Index, OpB_GPR_Index, OpA_GPR_Index) \
    TT_OP_QSR(0x5b, (((OpB_is_Const) << 23) + ((OpSel) << 18) + ((Result_GPR_Index) << 12) + ((OpB_GPR_Index) << 6) + ((OpA_GPR_Index) << 0)))
#define TT_OP_QSR_CFGSHIFTMASK(CfgRegAddr, disable_mask_on_old_val, operation, mask_width, right_cshift_amt, scratch_sel)                       \
    TT_OP_QSR(                                                                                                                                  \
        0xba,                                                                                                                               \
        (((CfgRegAddr) << 16) + ((disable_mask_on_old_val) << 15) + ((operation) << 12) + ((mask_width) << 7) + ((right_cshift_amt) << 2) + \
         ((scratch_sel) << 0)))
#define TT_OP_QSR_CFGSHIFTMASK_BUT_ALIAS_BIT_8_OF_CFG_REG_ADDR_WITH_LSB_OF_OPCODE(                                                              \
    CfgRegAddr, disable_mask_on_old_val, operation, mask_width, right_cshift_amt, scratch_sel)                                              \
    TT_OP_QSR(                                                                                                                                  \
        0xbb,                                                                                                                               \
        (((CfgRegAddr) << 16) + ((disable_mask_on_old_val) << 15) + ((operation) << 12) + ((mask_width) << 7) + ((right_cshift_amt) << 2) + \
         ((scratch_sel) << 0)))
#define TT_OP_QSR_CLEARDVALID(cleardvalid, cleardvalid_S, dest_dvalid_reset, dest_dvalid_client_bank_reset, dest_pulse_last, reset)                              \
    TT_OP_QSR(                                                                                                                                                   \
        0x36,                                                                                                                                                \
        (((cleardvalid) << 22) + ((cleardvalid_S) << 20) + ((dest_dvalid_reset) << 10) + ((dest_dvalid_client_bank_reset) << 6) + ((dest_pulse_last) << 2) + \
         ((reset) << 0)))
#define TT_OP_QSR_CMPGPR(OpB_is_Const, OpSel, Result_GPR_Index, OpB_GPR_Index, OpA_GPR_Index) \
    TT_OP_QSR(0x5d, (((OpB_is_Const) << 23) + ((OpSel) << 18) + ((Result_GPR_Index) << 12) + ((OpB_GPR_Index) << 6) + ((OpA_GPR_Index) << 0)))
#define TT_OP_QSR_COMMIT_SHADOW(force_commit) TT_OP_QSR(0x41, (((force_commit) << 0)))
#define TT_OP_QSR_DMANOP                      TT_OP_QSR(0x60, 0)
#define TT_OP_QSR_ELWADD(clear_dvalid, dest_accum_en, instr_mod19, addr_mode, dst) \
    TT_OP_QSR(0x28, (((clear_dvalid) << 22) + ((dest_accum_en) << 21) + ((instr_mod19) << 19) + ((addr_mode) << 14) + ((dst) << 0)))
#define TT_OP_QSR_ELWADDDI(clear_dvalid, ins_mod, srcb_addr, srca_addr, addr_mode, dst) \
    TT_OP_QSR(0x31, (((clear_dvalid) << 22) + ((ins_mod) << 19) + ((srcb_addr) << 15) + ((srca_addr) << 11) + ((addr_mode) << 8) + ((dst) << 0)))
#define TT_OP_QSR_ELWMUL(clear_dvalid, dest_accum_en, instr_mod19, addr_mode, dst) \
    TT_OP_QSR(0x27, (((clear_dvalid) << 22) + ((dest_accum_en) << 21) + ((instr_mod19) << 19) + ((addr_mode) << 14) + ((dst) << 0)))
#define TT_OP_QSR_ELWMULDI(clear_dvalid, ins_mod, srcb_addr, srca_addr, addr_mode, dst) \
    TT_OP_QSR(0x3a, (((clear_dvalid) << 22) + ((ins_mod) << 19) + ((srcb_addr) << 15) + ((srca_addr) << 11) + ((addr_mode) << 8) + ((dst) << 0)))
#define TT_OP_QSR_ELWSUB(clear_dvalid, dest_accum_en, instr_mod19, addr_mode, dst) \
    TT_OP_QSR(0x30, (((clear_dvalid) << 22) + ((dest_accum_en) << 21) + ((instr_mod19) << 19) + ((addr_mode) << 14) + ((dst) << 0)))
#define TT_OP_QSR_ELWSUBDI(clear_dvalid, ins_mod, srcb_addr, srca_addr, addr_mode, dst) \
    TT_OP_QSR(0x32, (((clear_dvalid) << 22) + ((ins_mod) << 19) + ((srcb_addr) << 15) + ((srca_addr) << 11) + ((addr_mode) << 8) + ((dst) << 0)))
#define TT_OP_QSR_GAPOOL(clear_dvalid, instr_mod19, pool_addr_mode, rsvd, dst) \
    TT_OP_QSR(0x34, (((clear_dvalid) << 22) + ((instr_mod19) << 19) + ((pool_addr_mode) << 15) + ((rsvd) << 14) + ((dst) << 0)))
#define TT_OP_QSR_GMPOOL(clear_dvalid, instr_mod19, pool_addr_mode, rsvd, dst) \
    TT_OP_QSR(0x33, (((clear_dvalid) << 22) + ((instr_mod19) << 19) + ((pool_addr_mode) << 15) + ((rsvd) << 14) + ((dst) << 0)))
#define TT_OP_QSR_HALT                                                           TT_OP_QSR(0x23, 0)
#define TT_OP_QSR_INCRWC(rwc_cr, rwc_a, rwc_b, rwc_d)                            TT_OP_QSR(0x38, (((rwc_cr) << 18) + ((rwc_a) << 13) + ((rwc_b) << 8) + ((rwc_d) << 0)))
#define TT_OP_QSR_INC_DST_TILE_FACE_ROW_IDX(Tile_Face_Row_Sel, EngineSel, Value) TT_OP_QSR(0x0e, (((Tile_Face_Row_Sel) << 21) + ((EngineSel) << 18) + ((Value) << 0)))
#define TT_OP_QSR_INC_SRC_TILE_FACE_ROW_IDX(Tile_Face_Row_Sel, EngineSel, Value) TT_OP_QSR(0x07, (((Tile_Face_Row_Sel) << 21) + ((EngineSel) << 18) + ((Value) << 0)))
#define TT_OP_QSR_LOADIND(SizeSel, OffsetIndex, AutoIncSpec, Data_GPR_Index, Addr_GPR_Index) \
    TT_OP_QSR(0x49, (((SizeSel) << 22) + ((OffsetIndex) << 14) + ((AutoIncSpec) << 12) + ((Data_GPR_Index) << 6) + ((Addr_GPR_Index) << 0)))
#define TT_OP_QSR_LOADREG(Data_GPR_Index, RegAddr) TT_OP_QSR(0x68, (((Data_GPR_Index) << 18) + ((RegAddr) << 0)))
#define TT_OP_QSR_MOP(mop_type, done, loop_count, zmask_lo8_or_loop_count) \
    TT_OP_QSR(0x01, (((mop_type) << 23) + ((done) << 22) + ((loop_count) << 15) + ((zmask_lo8_or_loop_count) << 0)))
#define TT_OP_QSR_MOP_CFG(zmask_hi24) TT_OP_QSR(0x03, (((zmask_hi24) << 0)))
#define TT_OP_QSR_MOVA2D(dest_32b_lo, src, addr_mode, instr_mod, dst) \
    TT_OP_QSR(0x12, (((dest_32b_lo) << 23) + ((src) << 17) + ((addr_mode) << 14) + ((instr_mod) << 12) + ((dst) << 0)))
#define TT_OP_QSR_MOVB2A(srca, addr_mode, instr_mod, srcb)          TT_OP_QSR(0x0b, (((srca) << 17) + ((addr_mode) << 14) + ((instr_mod) << 12) + ((srcb) << 0)))
#define TT_OP_QSR_MOVB2D(dest_32b_lo, src, addr_mode, transfer_sz, bcast_datum0, dst) \
    TT_OP_QSR(0x13, (((dest_32b_lo) << 23) + ((src) << 17) + ((addr_mode) << 14) + ((transfer_sz) << 12) + ((bcast_datum0) << 11) + ((dst) << 0)))
#define TT_OP_QSR_MOVD2A(dest_32b_lo, src, addr_mode, instr_mod, dst) \
    TT_OP_QSR(0x08, (((dest_32b_lo) << 23) + ((src) << 17) + ((addr_mode) << 14) + ((instr_mod) << 12) + ((dst) << 0)))
#define TT_OP_QSR_MOVD2B(dest_32b_lo, src, addr_mode, instr_mod, transpose, dst) \
    TT_OP_QSR(0x0a, (((dest_32b_lo) << 23) + ((src) << 17) + ((addr_mode) << 14) + ((instr_mod) << 12) + ((transpose) << 11) + ((dst) << 0)))
#define TT_OP_QSR_MOVDBGA2D(dest_32b_lo, src, addr_mode, instr_mod, dst) \
    TT_OP_QSR(0x09, (((dest_32b_lo) << 23) + ((src) << 17) + ((addr_mode) << 14) + ((instr_mod) << 12) + ((dst) << 0)))
#define TT_OP_QSR_MOVDBGB2D(dest_32b_lo, src, addr_mode, transfer_sz, bcast_datum0, dst) \
    TT_OP_QSR(0x0c, (((dest_32b_lo) << 23) + ((src) << 17) + ((addr_mode) << 14) + ((transfer_sz) << 12) + ((bcast_datum0) << 11) + ((dst) << 0)))
#define TT_OP_QSR_MULGPR(OpB_is_Const, Result_GPR_Index, OpB_GPR_Index, OpA_GPR_Index) \
    TT_OP_QSR(0x5a, (((OpB_is_Const) << 23) + ((Result_GPR_Index) << 12) + ((OpB_GPR_Index) << 6) + ((OpA_GPR_Index) << 0)))
#define TT_OP_QSR_MVMUL(clear_dvalid, instr_mod19, addr_mode, dst) \
    TT_OP_QSR(0x26, (((clear_dvalid) << 22) + ((instr_mod19) << 19) + ((addr_mode) << 14) + ((dst) << 0)))
#define TT_OP_QSR_MVMULDI(clear_dvalid, ins_mod, srcb_addr, srca_addr, addr_mode, dst) \
    TT_OP_QSR(0x25, (((clear_dvalid) << 22) + ((ins_mod) << 19) + ((srcb_addr) << 15) + ((srca_addr) << 11) + ((addr_mode) << 8) + ((dst) << 0)))
#define TT_OP_QSR_NOP TT_OP_QSR(0x02, 0)
#define TT_OP_QSR_PACR0_FACE(Dst_Face_Idx, Src_Face_Idx, Dst_Tile_Offset_Idx_Inc, Src_Tile_Offset_Idx_Inc, Buffer_Descriptor_Table_Sel, ClrDatValid) \
    TT_OP_QSR(                                                                                                                                       \
        0x1f,                                                                                                                                    \
        (((Dst_Face_Idx) << 14) + ((Src_Face_Idx) << 12) + ((Dst_Tile_Offset_Idx_Inc) << 9) + ((Src_Tile_Offset_Idx_Inc) << 7) +                 \
         ((Buffer_Descriptor_Table_Sel) << 2) + ((ClrDatValid) << 1)))
#define TT_OP_QSR_PACR0_FACE_INC(Dst_Face_Idx_Inc, Src_Face_Idx_Inc, Dst_Tile_Offset_Idx_Inc, Src_Tile_Offset_Idx_Inc, Buffer_Descriptor_Table_Sel, ClrDatValid) \
    TT_OP_QSR(                                                                                                                                                   \
        0x20,                                                                                                                                                \
        (((Dst_Face_Idx_Inc) << 14) + ((Src_Face_Idx_Inc) << 12) + ((Dst_Tile_Offset_Idx_Inc) << 9) + ((Src_Tile_Offset_Idx_Inc) << 7) +                     \
         ((Buffer_Descriptor_Table_Sel) << 2) + ((ClrDatValid) << 1)))
#define TT_OP_QSR_PACR0_ROW(                                                                                                                              \
    Dst_Row_Idx, Src_Row_Idx, Dst_Face_Idx, Src_Face_Idx, Dst_Tile_Offset_Idx_Inc, Src_Tile_Offset_Idx_Inc, Buffer_Descriptor_Table_Sel, ClrDatValid) \
    TT_OP_QSR(                                                                                                                                            \
        0x2a,                                                                                                                                         \
        (((Dst_Row_Idx) << 20) + ((Src_Row_Idx) << 16) + ((Dst_Face_Idx) << 14) + ((Src_Face_Idx) << 12) + ((Dst_Tile_Offset_Idx_Inc) << 9) +         \
         ((Src_Tile_Offset_Idx_Inc) << 7) + ((Buffer_Descriptor_Table_Sel) << 2) + ((ClrDatValid) << 1)))
#define TT_OP_QSR_PACR0_ROW_INC(                                                                                                                                  \
    Dst_Row_Idx_Inc,                                                                                                                                          \
    Src_Row_Idx_Inc,                                                                                                                                          \
    Dst_Face_Idx_Inc,                                                                                                                                         \
    Src_Face_Idx_Inc,                                                                                                                                         \
    Dst_Tile_Offset_Idx_Inc,                                                                                                                                  \
    Src_Tile_Offset_Idx_Inc,                                                                                                                                  \
    Buffer_Descriptor_Table_Sel,                                                                                                                              \
    ClrDatValid)                                                                                                                                              \
    TT_OP_QSR(                                                                                                                                                    \
        0x2b,                                                                                                                                                 \
        (((Dst_Row_Idx_Inc) << 20) + ((Src_Row_Idx_Inc) << 16) + ((Dst_Face_Idx_Inc) << 14) + ((Src_Face_Idx_Inc) << 12) + ((Dst_Tile_Offset_Idx_Inc) << 9) + \
         ((Src_Tile_Offset_Idx_Inc) << 7) + ((Buffer_Descriptor_Table_Sel) << 2) + ((ClrDatValid) << 1)))
#define TT_OP_QSR_PACR0_TILE(Dst_Tile_Idx, Src_Tile_Idx, Buffer_Descriptor_Table_Sel, ClrDatValid) \
    TT_OP_QSR(0x0f, (((Dst_Tile_Idx) << 16) + ((Src_Tile_Idx) << 7) + ((Buffer_Descriptor_Table_Sel) << 2) + ((ClrDatValid) << 1)))
#define TT_OP_QSR_PACR0_TILE_INC(Dst_Tile_Idx_Inc, Src_Tile_Idx_Inc, Buffer_Descriptor_Table_Sel, ClrDatValid) \
    TT_OP_QSR(0x19, (((Dst_Tile_Idx_Inc) << 16) + ((Src_Tile_Idx_Inc) << 7) + ((Buffer_Descriptor_Table_Sel) << 2) + ((ClrDatValid) << 1)))
#define TT_OP_QSR_PACR1_FACE(Dst_Face_Idx, Src_Face_Idx, Dst_Tile_Offset_Idx_Inc, Src_Tile_Offset_Idx_Inc, Buffer_Descriptor_Table_Sel, ClrDatValid) \
    TT_OP_QSR(                                                                                                                                       \
        0x2e,                                                                                                                                    \
        (((Dst_Face_Idx) << 14) + ((Src_Face_Idx) << 12) + ((Dst_Tile_Offset_Idx_Inc) << 9) + ((Src_Tile_Offset_Idx_Inc) << 7) +                 \
         ((Buffer_Descriptor_Table_Sel) << 2) + ((ClrDatValid) << 1)))
#define TT_OP_QSR_PACR1_FACE_INC(Dst_Face_Idx_Inc, Src_Face_Idx_Inc, Dst_Tile_Offset_Idx_Inc, Src_Tile_Offset_Idx_Inc, Buffer_Descriptor_Table_Sel, ClrDatValid) \
    TT_OP_QSR(                                                                                                                                                   \
        0x2f,                                                                                                                                                \
        (((Dst_Face_Idx_Inc) << 14) + ((Src_Face_Idx_Inc) << 12) + ((Dst_Tile_Offset_Idx_Inc) << 9) + ((Src_Tile_Offset_Idx_Inc) << 7) +                     \
         ((Buffer_Descriptor_Table_Sel) << 2) + ((ClrDatValid) << 1)))
#define TT_OP_QSR_PACR1_ROW(                                                                                                                              \
    Dst_Row_Idx, Src_Row_Idx, Dst_Face_Idx, Src_Face_Idx, Dst_Tile_Offset_Idx_Inc, Src_Tile_Offset_Idx_Inc, Buffer_Descriptor_Table_Sel, ClrDatValid) \
    TT_OP_QSR(                                                                                                                                            \
        0x3b,                                                                                                                                         \
        (((Dst_Row_Idx) << 20) + ((Src_Row_Idx) << 16) + ((Dst_Face_Idx) << 14) + ((Src_Face_Idx) << 12) + ((Dst_Tile_Offset_Idx_Inc) << 9) +         \
         ((Src_Tile_Offset_Idx_Inc) << 7) + ((Buffer_Descriptor_Table_Sel) << 2) + ((ClrDatValid) << 1)))
#define TT_OP_QSR_PACR1_ROW_INC(                                                                                                                                  \
    Dst_Row_Idx_Inc,                                                                                                                                          \
    Src_Row_Idx_Inc,                                                                                                                                          \
    Dst_Face_Idx_Inc,                                                                                                                                         \
    Src_Face_Idx_Inc,                                                                                                                                         \
    Dst_Tile_Offset_Idx_Inc,                                                                                                                                  \
    Src_Tile_Offset_Idx_Inc,                                                                                                                                  \
    Buffer_Descriptor_Table_Sel,                                                                                                                              \
    ClrDatValid)                                                                                                                                              \
    TT_OP_QSR(                                                                                                                                                    \
        0x6e,                                                                                                                                                 \
        (((Dst_Row_Idx_Inc) << 20) + ((Src_Row_Idx_Inc) << 16) + ((Dst_Face_Idx_Inc) << 14) + ((Src_Face_Idx_Inc) << 12) + ((Dst_Tile_Offset_Idx_Inc) << 9) + \
         ((Src_Tile_Offset_Idx_Inc) << 7) + ((Buffer_Descriptor_Table_Sel) << 2) + ((ClrDatValid) << 1)))
#define TT_OP_QSR_PACR1_TILE(Dst_Tile_Idx, Src_Tile_Idx, Buffer_Descriptor_Table_Sel, ClrDatValid) \
    TT_OP_QSR(0x2c, (((Dst_Tile_Idx) << 16) + ((Src_Tile_Idx) << 7) + ((Buffer_Descriptor_Table_Sel) << 2) + ((ClrDatValid) << 1)))
#define TT_OP_QSR_PACR1_TILE_INC(Dst_Tile_Idx_Inc, Src_Tile_Idx_Inc, Buffer_Descriptor_Table_Sel, ClrDatValid) \
    TT_OP_QSR(0x2d, (((Dst_Tile_Idx_Inc) << 16) + ((Src_Tile_Idx_Inc) << 7) + ((Buffer_Descriptor_Table_Sel) << 2) + ((ClrDatValid) << 1)))
#define TT_OP_QSR_PACR_STRIDE(                                                                                                                \
    Src_Row_Idx_or_Inc_Mul4,                                                                                                              \
    Src_Row_Idx_Inc,                                                                                                                      \
    L1_Tile_Idx_or_Tile_Idx_Inc,                                                                                                          \
    Tile_Idx_Inc,                                                                                                                         \
    L1_16datums_Row_Index,                                                                                                                \
    Buffer_Descriptor_Table_Sel,                                                                                                          \
    PackerSel,                                                                                                                            \
    ClrDatValid)                                                                                                                          \
    TT_OP_QSR(                                                                                                                                \
        0x1d,                                                                                                                             \
        (((Src_Row_Idx_or_Inc_Mul4) << 18) + ((Src_Row_Idx_Inc) << 17) + ((L1_Tile_Idx_or_Tile_Idx_Inc) << 14) + ((Tile_Idx_Inc) << 13) + \
         ((L1_16datums_Row_Index) << 7) + ((Buffer_Descriptor_Table_Sel) << 2) + ((PackerSel) << 1) + ((ClrDatValid) << 0)))
#define TT_OP_QSR_PACR_UNTILIZE(Reserved, Cntr_Reset_mask, Dst_Z_Cntr_inc, Src_Z_Cntr_inc, Packer_Sel, Buffer_Descriptor_Table_Sel, ClrDatValid) \
    TT_OP_QSR(                                                                                                                                   \
        0x42,                                                                                                                                \
        (((Reserved) << 14) + ((Cntr_Reset_mask) << 12) + ((Dst_Z_Cntr_inc) << 10) + ((Src_Z_Cntr_inc) << 8) + ((Packer_Sel) << 7) +         \
         ((Buffer_Descriptor_Table_Sel) << 2) + ((ClrDatValid) << 1)))
#define TT_OP_QSR_POP_TILES(unpacker_rd_done_wait_mask, num_tiles, buffer_sel) \
    TT_OP_QSR(0x3e, (((unpacker_rd_done_wait_mask) << 15) + ((num_tiles) << 5) + ((buffer_sel) << 0)))
#define TT_OP_QSR_PUSH_TILES(packer_wr_done_wait_mask, num_tiles, buffer_sel) \
    TT_OP_QSR(0x3d, (((packer_wr_done_wait_mask) << 15) + ((num_tiles) << 5) + ((buffer_sel) << 0)))
#define TT_OP_QSR_RDCFG(GprAddress, CfgReg) TT_OP_QSR(0xb1, (((GprAddress) << 16) + ((CfgReg) << 0)))
#define TT_OP_QSR_REPLAY(start_idx, len, last, set_mutex, execute_while_loading, load_mode) \
    TT_OP_QSR(0x04, (((start_idx) << 14) + ((len) << 4) + ((last) << 3) + ((set_mutex) << 2) + ((execute_while_loading) << 1) + ((load_mode) << 0)))
#define TT_OP_QSR_RESOURCEDECL(linger_time, resources, op_class) TT_OP_QSR(0x05, (((linger_time) << 15) + ((resources) << 5) + ((op_class) << 0)))
#define TT_OP_QSR_RMWCIB0(CfgRegAddr, Mask, Data)                TT_OP_QSR(0xb2, (((CfgRegAddr) << 16) + ((Mask) << 8) + ((Data) << 0)))
#define TT_OP_QSR_RMWCIB0_BUT_ALIAS_BIT_8_OF_CFG_REG_ADDR_WITH_LSB_OF_OPCODE(CfgRegAddr, Mask, Data) \
    TT_OP_QSR(0xb3, (((CfgRegAddr) << 16) + ((Mask) << 8) + ((Data) << 0)))
#define TT_OP_QSR_RMWCIB1(CfgRegAddr, Mask, Data) TT_OP_QSR(0xb4, (((CfgRegAddr) << 16) + ((Mask) << 8) + ((Data) << 0)))
#define TT_OP_QSR_RMWCIB1_BUT_ALIAS_BIT_8_OF_CFG_REG_ADDR_WITH_LSB_OF_OPCODE(CfgRegAddr, Mask, Data) \
    TT_OP_QSR(0xb5, (((CfgRegAddr) << 16) + ((Mask) << 8) + ((Data) << 0)))
#define TT_OP_QSR_RMWCIB2(CfgRegAddr, Mask, Data) TT_OP_QSR(0xb6, (((CfgRegAddr) << 16) + ((Mask) << 8) + ((Data) << 0)))
#define TT_OP_QSR_RMWCIB2_BUT_ALIAS_BIT_8_OF_CFG_REG_ADDR_WITH_LSB_OF_OPCODE(CfgRegAddr, Mask, Data) \
    TT_OP_QSR(0xb7, (((CfgRegAddr) << 16) + ((Mask) << 8) + ((Data) << 0)))
#define TT_OP_QSR_RMWCIB3(CfgRegAddr, Mask, Data) TT_OP_QSR(0xb8, (((CfgRegAddr) << 16) + ((Mask) << 8) + ((Data) << 0)))
#define TT_OP_QSR_RMWCIB3_BUT_ALIAS_BIT_8_OF_CFG_REG_ADDR_WITH_LSB_OF_OPCODE(CfgRegAddr, Mask, Data) \
    TT_OP_QSR(0xb9, (((CfgRegAddr) << 16) + ((Mask) << 8) + ((Data) << 0)))
#define TT_OP_QSR_RV_PACR(reg_idx2, reg_idx1, reg_idx0)   TT_OP_QSR(0x35, (((reg_idx2) << 10) + ((reg_idx1) << 5) + ((reg_idx0) << 0)))
#define TT_OP_QSR_RV_UNPACR(reg_idx2, reg_idx1, reg_idx0) TT_OP_QSR(0x39, (((reg_idx2) << 10) + ((reg_idx1) << 5) + ((reg_idx0) << 0)))
#define TT_OP_QSR_RV_WRCFG(byte_mask, write_64b, index_of_reg_containing_cfg_index, index_of_reg_containing_wrdata_msbs, index_of_reg_containing_wrdata_lsbs) \
    TT_OP_QSR(                                                                                                                                                \
        0x54,                                                                                                                                             \
        (((byte_mask) << 16) + ((write_64b) << 15) + ((index_of_reg_containing_cfg_index) << 10) + ((index_of_reg_containing_wrdata_msbs) << 5) +         \
         ((index_of_reg_containing_wrdata_lsbs) << 0)))
#define TT_OP_QSR_SEMGET(sem_bank_sel, sem_sel) TT_OP_QSR(0xa5, (((sem_bank_sel) << 8) + ((sem_sel) << 0)))
#define TT_OP_QSR_SEMINIT(max_value, init_value, sem_bank_sel, sem_sel) \
    TT_OP_QSR(0xa3, (((max_value) << 20) + ((init_value) << 16) + ((sem_bank_sel) << 8) + ((sem_sel) << 0)))
#define TT_OP_QSR_SEMPOST(sem_bank_sel, sem_sel) TT_OP_QSR(0xa4, (((sem_bank_sel) << 8) + ((sem_sel) << 0)))
#define TT_OP_QSR_SEMWAIT(stall_res, wait_sem_cond, sem_bank_sel, sem_sel) \
    TT_OP_QSR(0xa6, (((stall_res) << 15) + ((wait_sem_cond) << 13) + ((sem_bank_sel) << 8) + ((sem_sel) << 0)))
#define TT_OP_QSR_SETDVALID(setvalid) TT_OP_QSR(0x57, (((setvalid) << 0)))
#define TT_OP_QSR_SETGPR(Payload_SigSelSize, Payload_SigSel, SetSignalsMode, GPR_Index16b) \
    TT_OP_QSR(0x45, (((Payload_SigSelSize) << 22) + ((Payload_SigSel) << 8) + ((SetSignalsMode) << 7) + ((GPR_Index16b) << 0)))
#define TT_OP_QSR_SETRWC(clear_ab_vld, rwc_cr, rwc_val, BitMask)                 TT_OP_QSR(0x37, (((clear_ab_vld) << 22) + ((rwc_cr) << 18) + ((rwc_val) << 6) + ((BitMask) << 0)))
#define TT_OP_QSR_SET_DST_TILE_FACE_ROW_IDX(Tile_Face_Row_Sel, EngineSel, Value) TT_OP_QSR(0x0d, (((Tile_Face_Row_Sel) << 21) + ((EngineSel) << 18) + ((Value) << 0)))
#define TT_OP_QSR_SET_SRC_TILE_FACE_ROW_IDX(Tile_Face_Row_Sel, EngineSel, Value) TT_OP_QSR(0x06, (((Tile_Face_Row_Sel) << 21) + ((EngineSel) << 18) + ((Value) << 0)))
#define TT_OP_QSR_SFPABS(lreg_c, lreg_dest, instr_mod1) TT_OP_QSR(0x7d, (((lreg_c) << 8) + ((lreg_dest) << 4) + ((instr_mod1) << 0)))
#define TT_OP_QSR_SFPADD(lreg_a, lreg_b, lreg_c, lreg_dest, instr_mod1) \
    TT_OP_QSR(0x85, (((lreg_a) << 16) + ((lreg_b) << 12) + ((lreg_c) << 8) + ((lreg_dest) << 4) + ((instr_mod1) << 0)))
#define TT_OP_QSR_SFPADDI(imm16_math, lreg_dest, instr_mod1)     TT_OP_QSR(0x75, (((imm16_math) << 8) + ((lreg_dest) << 4) + ((instr_mod1) << 0)))
#define TT_OP_QSR_SFPAND(lreg_c, lreg_dest)                      TT_OP_QSR(0x7e, (((lreg_c) << 8) + ((lreg_dest) << 4)))
#define TT_OP_QSR_SFPCAST(lreg_c, lreg_dest, instr_mod1)         TT_OP_QSR(0x90, (((lreg_c) << 8) + ((lreg_dest) << 4) + ((instr_mod1) << 0)))
#define TT_OP_QSR_SFPCOMPC                                       TT_OP_QSR(0x8b, 0)
#define TT_OP_QSR_SFPCONFIG(imm16_math, config_dest, instr_mod1) TT_OP_QSR(0x91, (((imm16_math) << 8) + ((config_dest) << 4) + ((instr_mod1) << 0)))
#define TT_OP_QSR_SFPDIVP2(imm12_math, lreg_c, lreg_dest, instr_mod1) \
    TT_OP_QSR(0x76, (((imm12_math) << 12) + ((lreg_c) << 8) + ((lreg_dest) << 4) + ((instr_mod1) << 0)))
#define TT_OP_QSR_SFPENCC(imm12_math, instr_mod1)                   TT_OP_QSR(0x8a, (((imm12_math) << 12) + ((instr_mod1) << 0)))
#define TT_OP_QSR_SFPEXEXP(lreg_c, lreg_dest, instr_mod1)           TT_OP_QSR(0x77, (((lreg_c) << 8) + ((lreg_dest) << 4) + ((instr_mod1) << 0)))
#define TT_OP_QSR_SFPEXMAN(lreg_c, lreg_dest, instr_mod1)           TT_OP_QSR(0x78, (((lreg_c) << 8) + ((lreg_dest) << 4) + ((instr_mod1) << 0)))
#define TT_OP_QSR_SFPGT(imm12_math, lreg_c, lreg_dest, instr_mod1)  TT_OP_QSR(0x97, (((imm12_math) << 12) + ((lreg_c) << 8) + ((lreg_dest) << 4) + ((instr_mod1) << 0)))
#define TT_OP_QSR_SFPIADD(imm12_math, lreg_c, lreg_dest, instr_mod1) \
    TT_OP_QSR(0x79, (((imm12_math) << 12) + ((lreg_c) << 8) + ((lreg_dest) << 4) + ((instr_mod1) << 0)))
#define TT_OP_QSR_SFPLE(imm12_math, lreg_c, lreg_dest, instr_mod1) TT_OP_QSR(0x96, (((imm12_math) << 12) + ((lreg_c) << 8) + ((lreg_dest) << 4) + ((instr_mod1) << 0)))
#define TT_OP_QSR_SFPLOAD(lreg_ind, instr_mod0, sfpu_addr_mode, done, dest_reg_addr) \
    TT_OP_QSR(0x70, (((lreg_ind) << 20) + ((instr_mod0) << 16) + ((sfpu_addr_mode) << 13) + ((done) << 11) + ((dest_reg_addr) << 0)))
#define TT_OP_QSR_SFPLOADI(lreg_ind, instr_mod0, imm16) TT_OP_QSR(0x71, (((lreg_ind) << 20) + ((instr_mod0) << 16) + ((imm16) << 0)))
#define TT_OP_QSR_SFPLOADMACRO(seq_id, lreg_ind_lo, instr_mod0, sfpu_addr_mode, done, dest_reg_addr, lreg_ind_hi)                                   \
    TT_OP_QSR(                                                                                                                                      \
        0x93,                                                                                                                                   \
        (((seq_id) << 22) + ((lreg_ind_lo) << 20) + ((instr_mod0) << 16) + ((sfpu_addr_mode) << 13) + ((done) << 11) + ((dest_reg_addr) << 1) + \
         ((lreg_ind_hi) << 0)))
#define TT_OP_QSR_SFPLUT(lreg_ind, instr_mod0)         TT_OP_QSR(0x73, (((lreg_ind) << 20) + ((instr_mod0) << 16)))
#define TT_OP_QSR_SFPLUTFP32(lreg_dest, instr_mod1)    TT_OP_QSR(0x95, (((lreg_dest) << 4) + ((instr_mod1) << 0)))
#define TT_OP_QSR_SFPLZ(lreg_c, lreg_dest, instr_mod1) TT_OP_QSR(0x81, (((lreg_c) << 8) + ((lreg_dest) << 4) + ((instr_mod1) << 0)))
#define TT_OP_QSR_SFPMAD(lreg_a, lreg_b, lreg_c, lreg_dest, instr_mod1) \
    TT_OP_QSR(0x84, (((lreg_a) << 16) + ((lreg_b) << 12) + ((lreg_c) << 8) + ((lreg_dest) << 4) + ((instr_mod1) << 0)))
#define TT_OP_QSR_SFPMOV(lreg_c, lreg_dest, instr_mod1) TT_OP_QSR(0x7c, (((lreg_c) << 8) + ((lreg_dest) << 4) + ((instr_mod1) << 0)))
#define TT_OP_QSR_SFPMUL(lreg_a, lreg_b, lreg_c, lreg_dest, instr_mod1) \
    TT_OP_QSR(0x86, (((lreg_a) << 16) + ((lreg_b) << 12) + ((lreg_c) << 8) + ((lreg_dest) << 4) + ((instr_mod1) << 0)))
#define TT_OP_QSR_SFPMUL24(lreg_a, lreg_b, lreg_dest, instr_mod1) TT_OP_QSR(0x98, (((lreg_a) << 16) + ((lreg_b) << 12) + ((lreg_dest) << 4) + ((instr_mod1) << 0)))
#define TT_OP_QSR_SFPMULI(imm16_math, lreg_dest, instr_mod1)      TT_OP_QSR(0x74, (((imm16_math) << 8) + ((lreg_dest) << 4) + ((instr_mod1) << 0)))
#define TT_OP_QSR_SFPNONLINEAR(lreg_c, lreg_dest, instr_mod1)     TT_OP_QSR(0x99, (((lreg_c) << 8) + ((lreg_dest) << 4) + ((instr_mod1) << 0)))
#define TT_OP_QSR_SFPNOP(srcs_wr_done, srcs_rd_done, dest_done)   TT_OP_QSR(0x8f, (((srcs_wr_done) << 2) + ((srcs_rd_done) << 1) + ((dest_done) << 0)))
#define TT_OP_QSR_SFPNOT(lreg_c, lreg_dest)                       TT_OP_QSR(0x80, (((lreg_c) << 8) + ((lreg_dest) << 4)))
#define TT_OP_QSR_SFPOR(lreg_c, lreg_dest)                        TT_OP_QSR(0x7f, (((lreg_c) << 8) + ((lreg_dest) << 4)))
#define TT_OP_QSR_SFPPOPC(instr_mod1)                             TT_OP_QSR(0x88, (((instr_mod1) << 0)))
#define TT_OP_QSR_SFPPUSHC(instr_mod1)                            TT_OP_QSR(0x87, (((instr_mod1) << 0)))
#define TT_OP_QSR_SFPSETCC(imm12_math, lreg_c, instr_mod1)        TT_OP_QSR(0x7b, (((imm12_math) << 12) + ((lreg_c) << 8) + ((instr_mod1) << 0)))
#define TT_OP_QSR_SFPSETEXP(imm12_math, lreg_c, lreg_dest, instr_mod1) \
    TT_OP_QSR(0x82, (((imm12_math) << 12) + ((lreg_c) << 8) + ((lreg_dest) << 4) + ((instr_mod1) << 0)))
#define TT_OP_QSR_SFPSETMAN(imm12_math, lreg_c, lreg_dest, instr_mod1) \
    TT_OP_QSR(0x83, (((imm12_math) << 12) + ((lreg_c) << 8) + ((lreg_dest) << 4) + ((instr_mod1) << 0)))
#define TT_OP_QSR_SFPSETSGN(imm12_math, lreg_c, lreg_dest, instr_mod1) \
    TT_OP_QSR(0x89, (((imm12_math) << 12) + ((lreg_c) << 8) + ((lreg_dest) << 4) + ((instr_mod1) << 0)))
#define TT_OP_QSR_SFPSHFT(imm12_math, lreg_c, lreg_dest, instr_mod1) \
    TT_OP_QSR(0x7a, (((imm12_math) << 12) + ((lreg_c) << 8) + ((lreg_dest) << 4) + ((instr_mod1) << 0)))
#define TT_OP_QSR_SFPSHFT2(imm12_math, lreg_c, lreg_dest, instr_mod1) \
    TT_OP_QSR(0x94, (((imm12_math) << 12) + ((lreg_c) << 8) + ((lreg_dest) << 4) + ((instr_mod1) << 0)))
#define TT_OP_QSR_SFPSTORE(lreg_ind, instr_mod0, sfpu_addr_mode, done, dest_reg_addr) \
    TT_OP_QSR(0x72, (((lreg_ind) << 20) + ((instr_mod0) << 16) + ((sfpu_addr_mode) << 13) + ((done) << 11) + ((dest_reg_addr) << 0)))
#define TT_OP_QSR_SFPSWAP(imm12_math, lreg_c, lreg_dest, instr_mod1) \
    TT_OP_QSR(0x92, (((imm12_math) << 12) + ((lreg_c) << 8) + ((lreg_dest) << 4) + ((instr_mod1) << 0)))
#define TT_OP_QSR_SFPTRANSP                                        TT_OP_QSR(0x8c, 0)
#define TT_OP_QSR_SFPXOR(lreg_c, lreg_dest)                        TT_OP_QSR(0x8d, (((lreg_c) << 8) + ((lreg_dest) << 4)))
#define TT_OP_QSR_SFP_STOCH_RND(rnd_mode, imm8_math, lreg_b, lreg_c, lreg_dest, instr_mod1) \
    TT_OP_QSR(0x8e, (((rnd_mode) << 21) + ((imm8_math) << 16) + ((lreg_b) << 12) + ((lreg_c) << 8) + ((lreg_dest) << 4) + ((instr_mod1) << 0)))
#define TT_OP_QSR_SHIFTGPR(OpB_is_Const, OpSel, Result_GPR_Index, OpB_GPR_Index, OpA_GPR_Index) \
    TT_OP_QSR(0x5c, (((OpB_is_Const) << 23) + ((OpSel) << 18) + ((Result_GPR_Index) << 12) + ((OpB_GPR_Index) << 6) + ((OpA_GPR_Index) << 0)))
#define TT_OP_QSR_SHIFTXB(addr_mode, rot_shift, shift_row) TT_OP_QSR(0x18, (((addr_mode) << 14) + ((rot_shift) << 10) + ((shift_row) << 0)))
#define TT_OP_QSR_STALLWAIT(stall_res, wait_res_idx_2, wait_res_idx_1, wait_res_idx_0) \
    TT_OP_QSR(0xa2, (((stall_res) << 15) + ((wait_res_idx_2) << 10) + ((wait_res_idx_1) << 5) + ((wait_res_idx_0) << 0)))
#define TT_OP_QSR_STOREIND(SizeSel, MemSel, OffsetIndex, AutoIncSpec, Data_GPR_Index, Addr_GPR_Index) \
    TT_OP_QSR(0x66, (((SizeSel) << 22) + ((MemSel) << 21) + ((OffsetIndex) << 14) + ((AutoIncSpec) << 12) + ((Data_GPR_Index) << 6) + ((Addr_GPR_Index) << 0)))
#define TT_OP_QSR_STOREREG(Data_GPR_Index, RegAddr) TT_OP_QSR(0x67, (((Data_GPR_Index) << 18) + ((RegAddr) << 0)))
#define TT_OP_QSR_SUBGPR(OpB_is_Const, Result_GPR_Index, OpB_GPR_Index, OpA_GPR_Index) \
    TT_OP_QSR(0x59, (((OpB_is_Const) << 23) + ((Result_GPR_Index) << 12) + ((OpB_GPR_Index) << 6) + ((OpA_GPR_Index) << 0)))
#define TT_OP_QSR_UNPACR0_FACE(Dst_Face_Idx, Src_Face_Idx, Dst_Tile_Offset_Idx_Inc, Src_Tile_Offset_Idx_Inc, Buffer_Descriptor_Table_Sel, SetDatValid) \
    TT_OP_QSR(                                                                                                                                         \
        0x47,                                                                                                                                      \
        (((Dst_Face_Idx) << 14) + ((Src_Face_Idx) << 12) + ((Dst_Tile_Offset_Idx_Inc) << 10) + ((Src_Tile_Offset_Idx_Inc) << 7) +                  \
         ((Buffer_Descriptor_Table_Sel) << 2) + ((SetDatValid) << 1)))
#define TT_OP_QSR_UNPACR0_FACE_INC(Dst_Face_Idx_Inc, Src_Face_Idx_Inc, Dst_Tile_Offset_Idx_Inc, Src_Tile_Offset_Idx_Inc, Buffer_Descriptor_Table_Sel, SetDatValid) \
    TT_OP_QSR(                                                                                                                                                     \
        0x3f,                                                                                                                                                  \
        (((Dst_Face_Idx_Inc) << 14) + ((Src_Face_Idx_Inc) << 12) + ((Dst_Tile_Offset_Idx_Inc) << 10) + ((Src_Tile_Offset_Idx_Inc) << 7) +                      \
         ((Buffer_Descriptor_Table_Sel) << 2) + ((SetDatValid) << 1)))
#define TT_OP_QSR_UNPACR0_ROW(                                                                                                                            \
    Dst_Row_Idx, Src_Row_Idx, Dst_Face_Idx, Src_Face_Idx, Dst_Tile_Offset_Idx_Inc, Src_Tile_Offset_Idx_Inc, Buffer_Descriptor_Table_Sel, SetDatValid) \
    TT_OP_QSR(                                                                                                                                            \
        0x4b,                                                                                                                                         \
        (((Dst_Row_Idx) << 20) + ((Src_Row_Idx) << 16) + ((Dst_Face_Idx) << 14) + ((Src_Face_Idx) << 12) + ((Dst_Tile_Offset_Idx_Inc) << 10) +        \
         ((Src_Tile_Offset_Idx_Inc) << 7) + ((Buffer_Descriptor_Table_Sel) << 2) + ((SetDatValid) << 1)))
#define TT_OP_QSR_UNPACR0_ROW_INC(                                                                                                                                 \
    Dst_Row_Idx_Inc,                                                                                                                                           \
    Src_Row_Idx_Inc,                                                                                                                                           \
    Dst_Face_Idx_Inc,                                                                                                                                          \
    Src_Face_Idx_Inc,                                                                                                                                          \
    Dst_Tile_Offset_Idx_Inc,                                                                                                                                   \
    Src_Tile_Offset_Idx_Inc,                                                                                                                                   \
    Buffer_Descriptor_Table_Sel,                                                                                                                               \
    SetDatValid)                                                                                                                                               \
    TT_OP_QSR(                                                                                                                                                     \
        0x4c,                                                                                                                                                  \
        (((Dst_Row_Idx_Inc) << 20) + ((Src_Row_Idx_Inc) << 16) + ((Dst_Face_Idx_Inc) << 14) + ((Src_Face_Idx_Inc) << 12) + ((Dst_Tile_Offset_Idx_Inc) << 10) + \
         ((Src_Tile_Offset_Idx_Inc) << 7) + ((Buffer_Descriptor_Table_Sel) << 2) + ((SetDatValid) << 1)))
#define TT_OP_QSR_UNPACR0_STRIDE(                                                                                                                          \
    Src_Reg_Y_Cntr_Incr, L1_Tile_Idx_or_Tile_Idx_Inc, Tile_Idx_Inc, Row_Mask_Reg_Sel, L1_16datums_Row_Index, Buffer_Descriptor_Table_Sel, SetDatValid) \
    TT_OP_QSR(                                                                                                                                             \
        0x6f,                                                                                                                                          \
        (((Src_Reg_Y_Cntr_Incr) << 20) + ((L1_Tile_Idx_or_Tile_Idx_Inc) << 17) + ((Tile_Idx_Inc) << 16) + ((Row_Mask_Reg_Sel) << 13) +                 \
         ((L1_16datums_Row_Index) << 7) + ((Buffer_Descriptor_Table_Sel) << 2) + ((SetDatValid) << 1)))
#define TT_OP_QSR_UNPACR0_TILE(Dst_Tile_Idx, Src_Tile_Idx, Buffer_Descriptor_Table_Sel, SetDatValid) \
    TT_OP_QSR(0x3c, (((Dst_Tile_Idx) << 15) + ((Src_Tile_Idx) << 7) + ((Buffer_Descriptor_Table_Sel) << 2) + ((SetDatValid) << 1)))
#define TT_OP_QSR_UNPACR0_TILE_INC(Dst_Tile_Idx_Inc, Src_Tile_Idx_Inc, Buffer_Descriptor_Table_Sel, SetDatValid) \
    TT_OP_QSR(0x44, (((Dst_Tile_Idx_Inc) << 15) + ((Src_Tile_Idx_Inc) << 7) + ((Buffer_Descriptor_Table_Sel) << 2) + ((SetDatValid) << 1)))
#define TT_OP_QSR_UNPACR1_FACE(Dst_Face_Idx, Src_Face_Idx, Dst_Tile_Offset_Idx_Inc, Src_Tile_Offset_Idx_Inc, Buffer_Descriptor_Table_Sel, SetDatValid) \
    TT_OP_QSR(                                                                                                                                         \
        0x6a,                                                                                                                                      \
        (((Dst_Face_Idx) << 14) + ((Src_Face_Idx) << 12) + ((Dst_Tile_Offset_Idx_Inc) << 10) + ((Src_Tile_Offset_Idx_Inc) << 7) +                  \
         ((Buffer_Descriptor_Table_Sel) << 2) + ((SetDatValid) << 1)))
#define TT_OP_QSR_UNPACR1_FACE_INC(Dst_Face_Idx_Inc, Src_Face_Idx_Inc, Dst_Tile_Offset_Idx_Inc, Src_Tile_Offset_Idx_Inc, Buffer_Descriptor_Table_Sel, SetDatValid) \
    TT_OP_QSR(                                                                                                                                                     \
        0x6b,                                                                                                                                                  \
        (((Dst_Face_Idx_Inc) << 14) + ((Src_Face_Idx_Inc) << 12) + ((Dst_Tile_Offset_Idx_Inc) << 10) + ((Src_Tile_Offset_Idx_Inc) << 7) +                      \
         ((Buffer_Descriptor_Table_Sel) << 2) + ((SetDatValid) << 1)))
#define TT_OP_QSR_UNPACR1_ROW(                                                                                                                            \
    Dst_Row_Idx, Src_Row_Idx, Dst_Face_Idx, Src_Face_Idx, Dst_Tile_Offset_Idx_Inc, Src_Tile_Offset_Idx_Inc, Buffer_Descriptor_Table_Sel, SetDatValid) \
    TT_OP_QSR(                                                                                                                                            \
        0x6c,                                                                                                                                         \
        (((Dst_Row_Idx) << 20) + ((Src_Row_Idx) << 16) + ((Dst_Face_Idx) << 14) + ((Src_Face_Idx) << 12) + ((Dst_Tile_Offset_Idx_Inc) << 10) +        \
         ((Src_Tile_Offset_Idx_Inc) << 7) + ((Buffer_Descriptor_Table_Sel) << 2) + ((SetDatValid) << 1)))
#define TT_OP_QSR_UNPACR1_ROW_INC(                                                                                                                                 \
    Dst_Row_Idx_Inc,                                                                                                                                           \
    Src_Row_Idx_Inc,                                                                                                                                           \
    Dst_Face_Idx_Inc,                                                                                                                                          \
    Src_Face_Idx_Inc,                                                                                                                                          \
    Dst_Tile_Offset_Idx_Inc,                                                                                                                                   \
    Src_Tile_Offset_Idx_Inc,                                                                                                                                   \
    Buffer_Descriptor_Table_Sel,                                                                                                                               \
    SetDatValid)                                                                                                                                               \
    TT_OP_QSR(                                                                                                                                                     \
        0x6d,                                                                                                                                                  \
        (((Dst_Row_Idx_Inc) << 20) + ((Src_Row_Idx_Inc) << 16) + ((Dst_Face_Idx_Inc) << 14) + ((Src_Face_Idx_Inc) << 12) + ((Dst_Tile_Offset_Idx_Inc) << 10) + \
         ((Src_Tile_Offset_Idx_Inc) << 7) + ((Buffer_Descriptor_Table_Sel) << 2) + ((SetDatValid) << 1)))
#define TT_OP_QSR_UNPACR1_STRIDE(                                                                                                                          \
    Src_Reg_Y_Cntr_Incr, L1_Tile_Idx_or_Tile_Idx_Inc, Tile_Idx_Inc, Row_Mask_Reg_Sel, L1_16datums_Row_Index, Buffer_Descriptor_Table_Sel, SetDatValid) \
    TT_OP_QSR(                                                                                                                                             \
        0xaa,                                                                                                                                          \
        (((Src_Reg_Y_Cntr_Incr) << 20) + ((L1_Tile_Idx_or_Tile_Idx_Inc) << 17) + ((Tile_Idx_Inc) << 16) + ((Row_Mask_Reg_Sel) << 13) +                 \
         ((L1_16datums_Row_Index) << 7) + ((Buffer_Descriptor_Table_Sel) << 2) + ((SetDatValid) << 1)))
#define TT_OP_QSR_UNPACR1_TILE(Dst_Tile_Idx, Src_Tile_Idx, Buffer_Descriptor_Table_Sel, SetDatValid) \
    TT_OP_QSR(0x5f, (((Dst_Tile_Idx) << 15) + ((Src_Tile_Idx) << 7) + ((Buffer_Descriptor_Table_Sel) << 2) + ((SetDatValid) << 1)))
#define TT_OP_QSR_UNPACR1_TILE_INC(Dst_Tile_Idx_Inc, Src_Tile_Idx_Inc, Buffer_Descriptor_Table_Sel, SetDatValid) \
    TT_OP_QSR(0x69, (((Dst_Tile_Idx_Inc) << 15) + ((Src_Tile_Idx_Inc) << 7) + ((Buffer_Descriptor_Table_Sel) << 2) + ((SetDatValid) << 1)))
#define TT_OP_QSR_UNPACR2_FACE(Dst_Face_Idx, Src_Face_Idx, Dst_Tile_Offset_Idx_Inc, Src_Tile_Offset_Idx_Inc, Buffer_Descriptor_Table_Sel, SetDatValid) \
    TT_OP_QSR(                                                                                                                                         \
        0x9d,                                                                                                                                      \
        (((Dst_Face_Idx) << 14) + ((Src_Face_Idx) << 12) + ((Dst_Tile_Offset_Idx_Inc) << 10) + ((Src_Tile_Offset_Idx_Inc) << 7) +                  \
         ((Buffer_Descriptor_Table_Sel) << 2) + ((SetDatValid) << 1)))
#define TT_OP_QSR_UNPACR2_FACE_INC(Dst_Face_Idx_Inc, Src_Face_Idx_Inc, Dst_Tile_Offset_Idx_Inc, Src_Tile_Offset_Idx_Inc, Buffer_Descriptor_Table_Sel, SetDatValid) \
    TT_OP_QSR(                                                                                                                                                     \
        0x9e,                                                                                                                                                  \
        (((Dst_Face_Idx_Inc) << 14) + ((Src_Face_Idx_Inc) << 12) + ((Dst_Tile_Offset_Idx_Inc) << 10) + ((Src_Tile_Offset_Idx_Inc) << 7) +                      \
         ((Buffer_Descriptor_Table_Sel) << 2) + ((SetDatValid) << 1)))
#define TT_OP_QSR_UNPACR2_ROW(                                                                                                                            \
    Dst_Row_Idx, Src_Row_Idx, Dst_Face_Idx, Src_Face_Idx, Dst_Tile_Offset_Idx_Inc, Src_Tile_Offset_Idx_Inc, Buffer_Descriptor_Table_Sel, SetDatValid) \
    TT_OP_QSR(                                                                                                                                            \
        0x9f,                                                                                                                                         \
        (((Dst_Row_Idx) << 20) + ((Src_Row_Idx) << 16) + ((Dst_Face_Idx) << 14) + ((Src_Face_Idx) << 12) + ((Dst_Tile_Offset_Idx_Inc) << 10) +        \
         ((Src_Tile_Offset_Idx_Inc) << 7) + ((Buffer_Descriptor_Table_Sel) << 2) + ((SetDatValid) << 1)))
#define TT_OP_QSR_UNPACR2_ROW_INC(                                                                                                                                 \
    Dst_Row_Idx_Inc,                                                                                                                                           \
    Src_Row_Idx_Inc,                                                                                                                                           \
    Dst_Face_Idx_Inc,                                                                                                                                          \
    Src_Face_Idx_Inc,                                                                                                                                          \
    Dst_Tile_Offset_Idx_Inc,                                                                                                                                   \
    Src_Tile_Offset_Idx_Inc,                                                                                                                                   \
    Buffer_Descriptor_Table_Sel,                                                                                                                               \
    SetDatValid)                                                                                                                                               \
    TT_OP_QSR(                                                                                                                                                     \
        0xa8,                                                                                                                                                  \
        (((Dst_Row_Idx_Inc) << 20) + ((Src_Row_Idx_Inc) << 16) + ((Dst_Face_Idx_Inc) << 14) + ((Src_Face_Idx_Inc) << 12) + ((Dst_Tile_Offset_Idx_Inc) << 10) + \
         ((Src_Tile_Offset_Idx_Inc) << 7) + ((Buffer_Descriptor_Table_Sel) << 2) + ((SetDatValid) << 1)))
#define TT_OP_QSR_UNPACR2_STRIDE(                                                                                                                          \
    Src_Reg_Y_Cntr_Incr, L1_Tile_Idx_or_Tile_Idx_Inc, Tile_Idx_Inc, Row_Mask_Reg_Sel, L1_16datums_Row_Index, Buffer_Descriptor_Table_Sel, SetDatValid) \
    TT_OP_QSR(                                                                                                                                             \
        0x4e,                                                                                                                                          \
        (((Src_Reg_Y_Cntr_Incr) << 20) + ((L1_Tile_Idx_or_Tile_Idx_Inc) << 17) + ((Tile_Idx_Inc) << 16) + ((Row_Mask_Reg_Sel) << 13) +                 \
         ((L1_16datums_Row_Index) << 7) + ((Buffer_Descriptor_Table_Sel) << 2) + ((SetDatValid) << 1)))
#define TT_OP_QSR_UNPACR2_TILE(Dst_Tile_Idx, Src_Tile_Idx, Buffer_Descriptor_Table_Sel, SetDatValid) \
    TT_OP_QSR(0x9b, (((Dst_Tile_Idx) << 15) + ((Src_Tile_Idx) << 7) + ((Buffer_Descriptor_Table_Sel) << 2) + ((SetDatValid) << 1)))
#define TT_OP_QSR_UNPACR2_TILE_INC(Dst_Tile_Idx_Inc, Src_Tile_Idx_Inc, Buffer_Descriptor_Table_Sel, SetDatValid) \
    TT_OP_QSR(0x9c, (((Dst_Tile_Idx_Inc) << 15) + ((Src_Tile_Idx_Inc) << 7) + ((Buffer_Descriptor_Table_Sel) << 2) + ((SetDatValid) << 1)))
#define TT_OP_QSR_UNPACR_DEST_FACE(Dst_Face_Idx, Src_Face_Idx, Dst_Tile_Offset_Idx_Inc, Src_Tile_Offset_Idx_Inc, Buffer_Descriptor_Table_Sel, SetDatValid) \
    TT_OP_QSR(                                                                                                                                             \
        0xae,                                                                                                                                          \
        (((Dst_Face_Idx) << 14) + ((Src_Face_Idx) << 12) + ((Dst_Tile_Offset_Idx_Inc) << 10) + ((Src_Tile_Offset_Idx_Inc) << 7) +                      \
         ((Buffer_Descriptor_Table_Sel) << 2) + ((SetDatValid) << 1)))
#define TT_OP_QSR_UNPACR_DEST_FACE_INC(                                                                                                       \
    Dst_Face_Idx_Inc, Src_Face_Idx_Inc, Dst_Tile_Offset_Idx_Inc, Src_Tile_Offset_Idx_Inc, Buffer_Descriptor_Table_Sel, SetDatValid)       \
    TT_OP_QSR(                                                                                                                                \
        0xaf,                                                                                                                             \
        (((Dst_Face_Idx_Inc) << 14) + ((Src_Face_Idx_Inc) << 12) + ((Dst_Tile_Offset_Idx_Inc) << 10) + ((Src_Tile_Offset_Idx_Inc) << 7) + \
         ((Buffer_Descriptor_Table_Sel) << 2) + ((SetDatValid) << 1)))
#define TT_OP_QSR_UNPACR_DEST_ROW(                                                                                                                        \
    Dst_Row_Idx, Src_Row_Idx, Dst_Face_Idx, Src_Face_Idx, Dst_Tile_Offset_Idx_Inc, Src_Tile_Offset_Idx_Inc, Buffer_Descriptor_Table_Sel, SetDatValid) \
    TT_OP_QSR(                                                                                                                                            \
        0xa7,                                                                                                                                         \
        (((Dst_Row_Idx) << 20) + ((Src_Row_Idx) << 16) + ((Dst_Face_Idx) << 14) + ((Src_Face_Idx) << 12) + ((Dst_Tile_Offset_Idx_Inc) << 10) +        \
         ((Src_Tile_Offset_Idx_Inc) << 7) + ((Buffer_Descriptor_Table_Sel) << 2) + ((SetDatValid) << 1)))
#define TT_OP_QSR_UNPACR_DEST_ROW_INC(                                                                                                                             \
    Dst_Row_Idx_Inc,                                                                                                                                           \
    Src_Row_Idx_Inc,                                                                                                                                           \
    Dst_Face_Idx_Inc,                                                                                                                                          \
    Src_Face_Idx_Inc,                                                                                                                                          \
    Dst_Tile_Offset_Idx_Inc,                                                                                                                                   \
    Src_Tile_Offset_Idx_Inc,                                                                                                                                   \
    Buffer_Descriptor_Table_Sel,                                                                                                                               \
    SetDatValid)                                                                                                                                               \
    TT_OP_QSR(                                                                                                                                                     \
        0x65,                                                                                                                                                  \
        (((Dst_Row_Idx_Inc) << 20) + ((Src_Row_Idx_Inc) << 16) + ((Dst_Face_Idx_Inc) << 14) + ((Src_Face_Idx_Inc) << 12) + ((Dst_Tile_Offset_Idx_Inc) << 10) + \
         ((Src_Tile_Offset_Idx_Inc) << 7) + ((Buffer_Descriptor_Table_Sel) << 2) + ((SetDatValid) << 1)))
#define TT_OP_QSR_UNPACR_DEST_STRIDE(                                                                                                                      \
    Src_Reg_Y_Cntr_Incr, L1_Tile_Idx_or_Tile_Idx_Inc, Tile_Idx_Inc, Row_Mask_Reg_Sel, L1_16datums_Row_Index, Buffer_Descriptor_Table_Sel, SetDatValid) \
    TT_OP_QSR(                                                                                                                                             \
        0xbd,                                                                                                                                          \
        (((Src_Reg_Y_Cntr_Incr) << 20) + ((L1_Tile_Idx_or_Tile_Idx_Inc) << 17) + ((Tile_Idx_Inc) << 16) + ((Row_Mask_Reg_Sel) << 13) +                 \
         ((L1_16datums_Row_Index) << 7) + ((Buffer_Descriptor_Table_Sel) << 2) + ((SetDatValid) << 1)))
#define TT_OP_QSR_UNPACR_DEST_TILE(Dst_Tile_Idx, Src_Tile_Idx, Buffer_Descriptor_Table_Sel, SetDatValid) \
    TT_OP_QSR(0xac, (((Dst_Tile_Idx) << 15) + ((Src_Tile_Idx) << 7) + ((Buffer_Descriptor_Table_Sel) << 2) + ((SetDatValid) << 1)))
#define TT_OP_QSR_UNPACR_DEST_TILE_INC(Dst_Tile_Idx_Inc, Src_Tile_Idx_Inc, Buffer_Descriptor_Table_Sel, SetDatValid) \
    TT_OP_QSR(0xad, (((Dst_Tile_Idx_Inc) << 15) + ((Src_Tile_Idx_Inc) << 7) + ((Buffer_Descriptor_Table_Sel) << 2) + ((SetDatValid) << 1)))
#define TT_OP_QSR_UNPACR_NOP(Unpacker_Select, Set_Dvalid, Stall_Cntrl, Bank_Clr_Ctrl, Src_ClrVal_Ctrl, Nop_type) \
    TT_OP_QSR(0x43, (((Unpacker_Select) << 8) + ((Set_Dvalid) << 7) + ((Stall_Cntrl) << 5) + ((Bank_Clr_Ctrl) << 4) + ((Src_ClrVal_Ctrl) << 2) + ((Nop_type) << 0)))
#define TT_OP_QSR_UNPACR_TILE_MISC(Unpack_Type, Row_Bcast_Row_Idx, Tile_Idx_Inc, Dst_Tile_Idx, Src_Tile_Idx, Buffer_Descriptor_Table_Sel, SetDatValid) \
    TT_OP_QSR(                                                                                                                                         \
        0xbf,                                                                                                                                      \
        (((Unpack_Type) << 21) + ((Row_Bcast_Row_Idx) << 15) + ((Tile_Idx_Inc) << 14) + ((Dst_Tile_Idx) << 12) + ((Src_Tile_Idx) << 7) +           \
         ((Buffer_Descriptor_Table_Sel) << 2) + ((SetDatValid) << 1)))
#define TT_OP_QSR_UNPACR_TILIZE(Reserved, Cntr_Reset_mask, Dst_Z_Cntr_inc, Src_Z_Cntr_inc, Unpack_Sel, Buffer_Descriptor_Table_Sel, SetDatValid) \
    TT_OP_QSR(                                                                                                                                   \
        0xbe,                                                                                                                                \
        (((Reserved) << 15) + ((Cntr_Reset_mask) << 13) + ((Dst_Z_Cntr_inc) << 11) + ((Src_Z_Cntr_inc) << 9) + ((Unpack_Sel) << 7) +         \
         ((Buffer_Descriptor_Table_Sel) << 2) + ((SetDatValid) << 1)))
#define TT_OP_QSR_WAIT_FREE(stall_res, num_tiles, buffer_sel)  TT_OP_QSR(0xab, (((stall_res) << 15) + ((num_tiles) << 5) + ((buffer_sel) << 0)))
#define TT_OP_QSR_WAIT_TILES(stall_res, num_tiles, buffer_sel) TT_OP_QSR(0xa9, (((stall_res) << 15) + ((num_tiles) << 5) + ((buffer_sel) << 0)))
#define TT_OP_QSR_WRCFG(GprAddress, wr128b, CfgReg)            TT_OP_QSR(0xb0, (((GprAddress) << 16) + ((wr128b) << 15) + ((CfgReg) << 0)))
#define TT_OP_QSR_ZEROACC(clear_mode, use_32_bit_mode, clear_zero_flags, addr_mode, where) \
    TT_OP_QSR(0x10, (((clear_mode) << 19) + ((use_32_bit_mode) << 18) + ((clear_zero_flags) << 17) + ((addr_mode) << 14) + ((where) << 0)))
#define TT_OP_QSR_ZEROSRC(packed_fmt, int_fmt, exp_bias, zero_val, write_mode, bank_mask, src_mask) \
    TT_OP_QSR(0x11, (((packed_fmt) << 7) + ((int_fmt) << 6) + ((exp_bias) << 5) + ((zero_val) << 4) + ((write_mode) << 3) + ((bank_mask) << 2) + ((src_mask) << 0)))
