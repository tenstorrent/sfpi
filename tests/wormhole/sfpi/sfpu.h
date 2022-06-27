//////////////////////////////////////////////////////////////////////////////
// sfpu.h: Tensix SFPU emulator for use w/ sfpi
//   This file provides interface to the sfpu emulator for testing sfpi
//
// The HW implementation will utilize a compiler extended with a type
// (__rvtt_vec_t) and many __builtin_* functions that map to TT assembly
// instructions.  The emulated version fakes this out by supplying a class for
// __rvtt_vec_t and soft implementations of the functionality of the
// __builtin_*s.
//
// __rvtt_vec_t:
//   This type implements the variables stored in LREGs.  On wormhole, the
//   type stores 32 bits in an unsigned int/float which is determined by
//   context at use time.
//
// External dependencies:
//   This code will plug into the emulation environment which needs to model
//   the destination register (for loads/stores), the condition code stack and
//   condition code registers (latter can be implemented here if needed) XXXXX
//   FIXME
///////////////////////////////////////////////////////////////////////////////
#pragma once

#include <cstdint>
#include <stdexcept>
#include <math.h>
#include <sfpi_fp16.h>
#include "wormhole/sfpi_hw.h"

namespace sfpu {

constexpr int SFPU_COLS = 8;
constexpr int SFPU_ROWS = 4;
constexpr int SFPU_WIDTH = SFPU_ROWS * SFPU_COLS;
constexpr int SFPU_DREG_SIZE = 128;
constexpr int SFPU_CC_DEPTH = 16;
constexpr int SFPU_LREGS = 8;

constexpr unsigned int BIT_15_MASK = 0x08000;
constexpr unsigned int BIT_31_MASK = 0x80000000;
constexpr unsigned int BITS_0_TO_31_MASK = 0x7FFFFFFF;

constexpr unsigned int TF32_MAX_BIT = 18;
constexpr unsigned int TF32_SGN_MASK = 0x40000;
constexpr unsigned int TF32_EXP_MASK = 0x3FC00;
constexpr unsigned int TF32_MAN_MASK = 0x003FF;
constexpr unsigned int TF32_SGN_EXP_MASK = TF32_SGN_MASK | TF32_EXP_MASK;
constexpr unsigned int TF32_SGN_MAN_MASK = TF32_SGN_MASK | TF32_MAN_MASK;
constexpr unsigned int TF32_SGN_SHIFT = 18;
constexpr unsigned int TF32_EXP_SHIFT = 10;
constexpr unsigned int TF32_EXP_BIAS = 127;

constexpr unsigned int FP16B_SGN_MASK = 0x8000;
constexpr unsigned int FP16B_EXP_MASK = 0x7F80;
constexpr unsigned int FP16B_MAN_MASK = 0x007F;
constexpr unsigned int FP16B_EXP_SHIFT = 7;

constexpr unsigned int FP16A_SGN_MASK = 0x8000;
constexpr unsigned int FP16A_EXP_MASK = 0x7C00;
constexpr unsigned int FP16A_MAN_MASK = 0x03FF;
constexpr unsigned int FP16A_SGN_SHIFT = 15;
constexpr unsigned int FP16A_EXP_SHIFT = 10;
constexpr unsigned int FP16A_MAN_WIDTH = 10;
constexpr unsigned int FP16A_EXP_BIAS = 15;

constexpr unsigned int FP32_MAX_BIT = 31;
constexpr unsigned int FP32_SGN_MASK = 0x80000000;
constexpr unsigned int FP32_EXP_MASK = 0x7F800000;
constexpr unsigned int FP32_MAN_MASK = 0x007FFFFF;
constexpr unsigned int FP32_SGN_EXP_MASK = FP32_SGN_MASK | FP32_EXP_MASK;
constexpr unsigned int FP32_SGN_MAN_MASK = FP32_SGN_MASK | FP32_MAN_MASK;
constexpr unsigned int FP32_SGN_SHIFT = 31;
constexpr unsigned int FP32_EXP_SHIFT = 23;
constexpr unsigned int FP32_MAN_SHIFT = 0;
constexpr unsigned int FP32_MAN_WIDTH = 23;
constexpr unsigned int FP32_EXP_BIAS = 127;

///////////////////////////////////////////////////////////////////////////////
inline float fp16b_to_float(unsigned int i)
{
    union Converter {
        const float f;
        const uint32_t i;

        constexpr Converter(const unsigned int ini) : i(ini << 16) {}
    } tmp(i);

    return tmp.f;
}

// XXXXX Quick and dirty, doesn't handle denorms, etc.
inline float fp16a_to_float(unsigned int i)
{
    const unsigned int s = (i & FP16A_SGN_MASK) >> FP16A_SGN_SHIFT;
    const unsigned int e = ((i & FP16A_EXP_MASK) >> FP16A_EXP_SHIFT) - FP16A_EXP_BIAS;
    const unsigned int m = (i & FP16A_MAN_MASK);

    const unsigned int fpval =
        (s << FP32_SGN_SHIFT) |
        ((e + FP32_EXP_BIAS) << FP32_EXP_SHIFT) |
        (m << (FP32_MAN_WIDTH - FP16A_MAN_WIDTH));

    union Converter {
        const float f;
        const uint32_t i;

        constexpr Converter(const unsigned int ini) : i(ini) {}
    } tmp(fpval);

    return tmp.f;
}

inline int float_to_fp16b(float f)
{
    union Converter {
        const float f;
        const uint32_t i;

        constexpr Converter(const float inf) : f(inf) {}
    } tmp(f);

    return tmp.i >> 13;
}

inline float int_to_float(unsigned int i)
{
    union Converter {
        const float f;
        const uint32_t i;

        constexpr Converter(const unsigned int ini) : i(ini) {}
    } tmp(i);

    return tmp.f;
}

inline int float_to_int(float f)
{
    union Converter {
        const float f;
        const uint32_t i;

        constexpr Converter(const float inf) : f(inf) {}
    } tmp(f);

    return tmp.i;
}

///////////////////////////////////////////////////////////////////////////////
class SFPUDReg {
 private:
    unsigned int addr_offset;
    unsigned int regs[SFPU_DREG_SIZE][SFPU_WIDTH];

 public:
    SFPUDReg();

    void load(unsigned int out[SFPU_WIDTH], const int addr) const;
    void store_int(const unsigned int data[SFPU_WIDTH], const int addr);
    void store_float(const unsigned int data[SFPU_WIDTH], const int addr);
    void store_float(const float data[SFPU_WIDTH], const int addr);
    void store_float(const float data, const int row, const int col);

    unsigned int get(const int row, const int col) const { return regs[row][col]; }
    float get_float(const int row, const int col) const { return int_to_float(regs[row][col]); }

    void add_offset(int o) { addr_offset += o; }
};

///////////////////////////////////////////////////////////////////////////////
class SFPUCC {
 private:
    int stack_ptr;
    bool enable[SFPU_WIDTH];  // Careful: false means all reads/writes go through
    bool result[SFPU_WIDTH];
    bool enable_stack[SFPU_CC_DEPTH][SFPU_WIDTH];
    bool result_stack[SFPU_CC_DEPTH][SFPU_WIDTH];

    bool deferred_result_pending;
    bool deferred_result[SFPU_WIDTH];

 public:
    SFPUCC();

    void set_enable(const bool value);
    void comp_enable();

    void set_result(const bool use_enable, const bool value);
    void set_result(const bool use_enable, const int i, const bool value) { result[i] = value && (!use_enable || enable[i]); }
    void and_result(const int i, const bool value) { result[i] = result[i] && value && enable[i]; }

    void deferred_init();
    void deferred_and_result(const int i, const bool value) { deferred_result[i] = deferred_result[i] && value && enable[i]; }
    void deferred_set_result(const bool use_enable, const int i, const bool value) { deferred_result[i] = value && (!use_enable || enable[i]); }
    void deferred_comp();
    void deferred_commit();

    bool enabled(const int which) { return !enable[which] || result[which]; }

    void comp();

    void push();
    void replace();
    void pop();

    void dump();
    void dump(int i);
};

///////////////////////////////////////////////////////////////////////////////
extern sfpu::SFPUDReg sfpu_dreg;
extern SFPUCC sfpu_cc;

} // namespace sfpu


///////////////////////////////////////////////////////////////////////////////
// Emulates an LREG
// Data stored in values is in FP32 format
///////////////////////////////////////////////////////////////////////////////
class __rvtt_vec_t {
 private:
    unsigned int values[sfpu::SFPU_WIDTH];

    static inline unsigned int float_to_tf32(const float f);
    static inline float tf32_to_float(const unsigned int i);

 public:
    enum class ConstructType {
        TileId
    };

    __rvtt_vec_t() = default;
    __rvtt_vec_t(const __rvtt_vec_t& in) = default;
    constexpr __rvtt_vec_t(ConstructType t);
    constexpr __rvtt_vec_t(const unsigned int v);

    inline unsigned int* get_data_write() { return values; }
    inline const unsigned int* get_data_read() const { return values; }

    inline float get_float(const int i) const { return sfpu::int_to_float(values[i]); }
    inline unsigned int get_uint(const int i) const { return values[i]; }

    inline void set_float(const int i, const float f) { values[i] = sfpu::float_to_int(f); }
    inline void set_uint(const int i, const int ini) { values[i] = ini; }

    inline void operator=(const __rvtt_vec_t& v);

    void dump(int start, int stop) const;
    void dump(int i) const;
};

///////////////////////////////////////////////////////////////////////////////
inline void __rvtt_vec_t::operator=(const __rvtt_vec_t& v)
{
    for (int i = 0; i < sfpu::SFPU_WIDTH; i++) {
        if (sfpu::sfpu_cc.enabled(i)) {
            values[i] = v.values[i];
        }
    }
}

inline unsigned int __rvtt_vec_t::float_to_tf32(const float f)
{
    union Converter {
        const float f;
        const uint32_t i;

        constexpr Converter(const float inf) : f(inf) {}
    } tmp(f);

    return tmp.i >> 13;
}

inline float __rvtt_vec_t::tf32_to_float(const unsigned int i)
{
    union Converter {
        const float f;
        const uint32_t i;

        constexpr Converter(const unsigned int ini) : i(ini << 13) {}
    } tmp(i);

    return tmp.f;
}

namespace sfpu {
extern __rvtt_vec_t sfpu_lreg[SFPU_LREGS];
};

///////////////////////////////////////////////////////////////////////////////
extern void sfpu_rvtt_sfpincrwc(int cr, int d, int b, int a);
extern __rvtt_vec_t sfpu_rvtt_sfpload(unsigned int mod0, unsigned int mode, unsigned int addr);
extern __rvtt_vec_t sfpu_rvtt_sfpassignlr(unsigned int lr);
extern void sfpu_rvtt_sfpstore(const __rvtt_vec_t& v, unsigned int mod0, unsigned int addr);
extern __rvtt_vec_t sfpu_rvtt_sfploadi(unsigned int mod0, unsigned int value);
extern __rvtt_vec_t sfpu_rvtt_sfpmov(const __rvtt_vec_t& v, unsigned int mod1);
extern __rvtt_vec_t sfpu_rvtt_sfpadd(const __rvtt_vec_t& a, const __rvtt_vec_t& b, unsigned int mod1);
extern __rvtt_vec_t sfpu_rvtt_sfpmul(const __rvtt_vec_t& a, const __rvtt_vec_t& b, unsigned int mod1);
extern __rvtt_vec_t sfpu_rvtt_sfpmad(const __rvtt_vec_t& a, const __rvtt_vec_t& b, const __rvtt_vec_t& c, unsigned int mod1);
extern void sfpu_rvtt_sfpnop();
extern void sfpu_rvtt_sfpillegal();
extern void sfpu_rvtt_sfpencc(unsigned int imm12, unsigned int mod1);
extern void sfpu_rvtt_sfpcompc();
extern void sfpu_rvtt_sfppushc(unsigned int mod1);
extern void sfpu_rvtt_sfppopc();
extern void sfpu_rvtt_sfpsetcc_i(unsigned int imm, unsigned int mod1);
extern void sfpu_rvtt_sfpsetcc_v(const __rvtt_vec_t& v, unsigned int mod1);
extern void sfpu_rvtt_sfpxscmp(const __rvtt_vec_t& v, unsigned int b, unsigned int mod1);
extern void sfpu_rvtt_sfpxvcmp(const __rvtt_vec_t& v1, const __rvtt_vec_t& v2, unsigned int mod1);
extern __rvtt_vec_t sfpu_rvtt_sfpexexp(const __rvtt_vec_t& v, unsigned int mod1);
extern __rvtt_vec_t sfpu_rvtt_sfpexman(const __rvtt_vec_t& v, unsigned int mod1);
extern __rvtt_vec_t sfpu_rvtt_sfpsetexp_i(unsigned int imm, const __rvtt_vec_t& v);
extern __rvtt_vec_t sfpu_rvtt_sfpsetexp_v(const __rvtt_vec_t& dst, const __rvtt_vec_t& src);
extern __rvtt_vec_t sfpu_rvtt_sfpsetman_i(unsigned int imm, const __rvtt_vec_t& v, unsigned int mod1);
extern __rvtt_vec_t sfpu_rvtt_sfpsetman_v(const __rvtt_vec_t& dst, const __rvtt_vec_t& src);
extern __rvtt_vec_t sfpu_rvtt_sfpabs(const __rvtt_vec_t& v, unsigned int mod1);
extern __rvtt_vec_t sfpu_rvtt_sfpand(const __rvtt_vec_t& dst, const __rvtt_vec_t& src);
extern __rvtt_vec_t sfpu_rvtt_sfpor(const __rvtt_vec_t& dst, const __rvtt_vec_t& src);
extern __rvtt_vec_t sfpu_rvtt_sfpxor(const __rvtt_vec_t& dst, const __rvtt_vec_t& src);
extern __rvtt_vec_t sfpu_rvtt_sfpnot(const __rvtt_vec_t& v);
extern __rvtt_vec_t sfpu_rvtt_sfpmuli(const __rvtt_vec_t& v, unsigned short imm, unsigned int mod1);
extern __rvtt_vec_t sfpu_rvtt_sfpaddi(const __rvtt_vec_t& v, unsigned short imm, unsigned int mod1);
extern __rvtt_vec_t sfpu_rvtt_sfpdivp2(unsigned short imm, const __rvtt_vec_t& v, unsigned int mod1);
extern __rvtt_vec_t sfpu_rvtt_sfplz(const __rvtt_vec_t& v, unsigned int mod1);
extern __rvtt_vec_t sfpu_rvtt_sfpshft_i(const __rvtt_vec_t& dst, int shift);
extern __rvtt_vec_t sfpu_rvtt_sfpshft_v(const __rvtt_vec_t& dst, const __rvtt_vec_t&src);
extern __rvtt_vec_t sfpu_rvtt_sfpiadd_i(short imm, const __rvtt_vec_t& src, unsigned int mod1);
extern __rvtt_vec_t sfpu_rvtt_sfpxiadd_i(int imm, const __rvtt_vec_t& src, unsigned int mod1);
extern __rvtt_vec_t sfpu_rvtt_sfpxiadd_v(const __rvtt_vec_t& dst, const __rvtt_vec_t& src, unsigned int mod1);
extern __rvtt_vec_t sfpu_rvtt_sfpiadd_v(const __rvtt_vec_t& dst, const __rvtt_vec_t& src, unsigned int mod1);
extern __rvtt_vec_t sfpu_rvtt_sfpsetsgn_i(unsigned short imm, const __rvtt_vec_t& src);
extern __rvtt_vec_t sfpu_rvtt_sfpsetsgn_v(const __rvtt_vec_t& dst, const __rvtt_vec_t& src);
extern __rvtt_vec_t sfpu_rvtt_sfplut(const __rvtt_vec_t& l0, const __rvtt_vec_t& l1, const __rvtt_vec_t& l2, const __rvtt_vec_t& dst, unsigned short mod0);
extern __rvtt_vec_t sfpu_rvtt_sfplutfp32_3r(const __rvtt_vec_t& l0,
                                            const __rvtt_vec_t& l1,
                                            const __rvtt_vec_t& l2,
                                            const __rvtt_vec_t& dst,
                                            unsigned short mod0);
extern __rvtt_vec_t sfpu_rvtt_sfplutfp32_6r(const __rvtt_vec_t& l0,
                                            const __rvtt_vec_t& l1,
                                            const __rvtt_vec_t& l2,
                                            const __rvtt_vec_t& l4,
                                            const __rvtt_vec_t& l5,
                                            const __rvtt_vec_t& l6,
                                            const __rvtt_vec_t& dst,
                                            unsigned short mod0);
extern __rvtt_vec_t sfpu_rvtt_sfpcast(const __rvtt_vec_t& src, unsigned int mod1);
extern __rvtt_vec_t sfpu_rvtt_sfpstochrnd_i(const unsigned int mode,
                                            const unsigned int imm8, const __rvtt_vec_t& srcc,
                                            unsigned int mod1);
extern __rvtt_vec_t sfpu_rvtt_sfpstochrnd_v(const unsigned int mode,
                                            const __rvtt_vec_t& srcb, const __rvtt_vec_t& srcc,
                                            unsigned int mod1);
extern void sfpu_rvtt_sfptransp(__rvtt_vec_t& l0, __rvtt_vec_t& l1, __rvtt_vec_t& l2, __rvtt_vec_t& l3);
extern __rvtt_vec_t sfpu_rvtt_sfpshft2_i(const __rvtt_vec_t& dst, int shift);
extern __rvtt_vec_t sfpu_rvtt_sfpshft2_v(const __rvtt_vec_t& dst, const __rvtt_vec_t&src);
extern void sfpu_rvtt_sfpshft2_g(__rvtt_vec_t& l0, __rvtt_vec_t& l1,
                                 __rvtt_vec_t& l2, __rvtt_vec_t& l3,
                                 int mod);

extern void sfpu_rvtt_sfpshft2_ge(const __rvtt_vec_t& src,
                                  __rvtt_vec_t& l0, __rvtt_vec_t& l1,
                                  __rvtt_vec_t& l2, __rvtt_vec_t& l3);
extern __rvtt_vec_t sfpu_rvtt_sfpshft2_e(const __rvtt_vec_t& src, int mod);
extern void sfpu_rvtt_sfpswap(__rvtt_vec_t& dst, __rvtt_vec_t& src, int mod);
extern void sfpu_rvtt_sfpconfig_v(const __rvtt_vec_t& l0, unsigned int config_dest);

extern int sfpu_rvtt_sfpxfcmps(const __rvtt_vec_t& v, unsigned int f, int mod);
extern int sfpu_rvtt_sfpxfcmpv(const __rvtt_vec_t& v1, const __rvtt_vec_t& v2, int mod1);
extern int sfpu_rvtt_sfpxicmps(const __rvtt_vec_t& v, unsigned int i, int mod1);
extern int sfpu_rvtt_sfpxicmpv(const __rvtt_vec_t& v1, const __rvtt_vec_t& v2, int mod1);
extern int sfpu_rvtt_sfpxbool(int t, int a, int b);
extern void __builtin_rvtt_sfpxcondb(int tree, int start);
extern __rvtt_vec_t __builtin_rvtt_sfpxcondi(int i);
