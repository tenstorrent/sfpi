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
//   This type implements the variables stored in LREGs.  The type stores 32
//   bits in an unsigned int where the least signficant 19 bits contain the
//   LREG data.  Operations are performed in native form on the host hardware
//   (eg, fp32); conversions from the internal format to/from the native
//   formats are done on access.
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
#include "sfpi_internal.h"

namespace sfpu {

constexpr int SFPU_WIDTH_GRAYSKULL = 64;
constexpr int SFPU_SIZE_GRAYSKULL = 128;
constexpr int SFPU_CC_DEPTH_GRAYSKULL = 7;    // Grayskull is 8 deep w/ a bug limiting it to 7

constexpr int SFPU_WIDTH_WORMHOLE  = 32;
constexpr int SFPU_SIZE_WORMHOLE = 128;
constexpr int SFPU_CC_DEPTH_WORMHOLE = 16;

constexpr unsigned int BIT_15_MASK = 0x08000;
constexpr unsigned int BIT_18_MASK = 0x40000;
constexpr unsigned int BITS_0_TO_17_MASK = 0x3FFFF;
constexpr unsigned int BITS_0_TO_18_MASK = 0x7FFFF;

constexpr unsigned int TF32_BITS = 18;
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
 constexpr unsigned int FP16A_EXP_SHIFT = 10;
 constexpr unsigned int FP16A_EXP_BIAS = 15;

constexpr unsigned int FP32_SGN_SHIFT = 31;
constexpr unsigned int FP32_EXP_SHIFT = 23;
constexpr unsigned int FP32_MAN_SHIFT = 0;
constexpr unsigned int FP32_SGN_MASK = 0x80000000;
constexpr unsigned int FP32_MAN_WIDTH = 23;


#ifdef ARCH_GRAYSKULL
constexpr int SFPU_WIDTH = SFPU_WIDTH_GRAYSKULL;
constexpr int SFPU_SIZE = SFPU_SIZE_GRAYSKULL;
constexpr int SFPU_CC_DEPTH = SFPU_CC_DEPTH_GRAYSKULL;
#elif ARCH_WORMHOLE
constexpr int SFPU_WIDTH = SFPU_WIDTH_WORMHOLE;
constexpr int SFPU_SIZE = SFPU_SIZE_WORMHOLE;
constexpr int SFPU_CC_DEPTH = SFPU_CC_DEPTH_WORMHOLE;
#endif


///////////////////////////////////////////////////////////////////////////////
class SFPUDReg {
 private:
    unsigned short regs[SFPU_SIZE][SFPU_WIDTH];

 public:
    SFPUDReg();

    static inline float fp16b_to_float(unsigned int i);
    static inline int float_to_fp16b(float f);

    void load(unsigned int out[SFPU_WIDTH], const int addr) const;
    void store_int(const unsigned int data[SFPU_WIDTH], const int addr);
    void store_float(const unsigned int data[SFPU_WIDTH], const int addr);
    void store_float(const float data[SFPU_WIDTH], const int addr);
    void store_float(const float data, const int row, const int col);

    unsigned short get(const int row, const int col) const { return regs[row][col]; }
    float get_float(const int row, const int col) const { return fp16b_to_float(regs[row][col]); }
};


inline float SFPUDReg::fp16b_to_float(unsigned int i)
{
    union Converter {
        const float f;
        const uint32_t i;

        constexpr Converter(const unsigned int ini) : i(ini << 16) {}
    } tmp(i);

    return tmp.f;
}

inline int SFPUDReg::float_to_fp16b(float f)
{
    union Converter {
        const float f;
        const uint32_t i;

        constexpr Converter(const float inf) : f(inf) {}
    } tmp(f);

    return tmp.i >> 13;
}

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
// Data stored in values is in TF32 format
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

    inline float get_float(const int i) const { return tf32_to_float(values[i]); }
    inline unsigned int get_uint(const int i) const { return values[i]; }

    inline void set_float(const int i, const float f) { values[i] = float_to_tf32(f); }
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
extern __rvtt_vec_t sfpu_lreg[4];
};

///////////////////////////////////////////////////////////////////////////////
extern __rvtt_vec_t sfpu_rvtt_sfpload(unsigned int mod0, unsigned int addr);
extern __rvtt_vec_t sfpu_rvtt_sfpassignlr(unsigned int lr);
extern void sfpu_rvtt_sfpstore(const __rvtt_vec_t& v, unsigned int mod0, unsigned int addr);
extern __rvtt_vec_t sfpu_rvtt_sfploadi(unsigned int mod0, unsigned short value);
extern __rvtt_vec_t sfpu_rvtt_sfpmov(const __rvtt_vec_t& v, unsigned int mod1);
extern __rvtt_vec_t sfpu_rvtt_sfpadd(const __rvtt_vec_t& a, const __rvtt_vec_t& b, unsigned int mod1);
extern __rvtt_vec_t sfpu_rvtt_sfpmul(const __rvtt_vec_t& a, const __rvtt_vec_t& b, unsigned int mod1);
extern __rvtt_vec_t sfpu_rvtt_sfpmad(const __rvtt_vec_t& a, const __rvtt_vec_t& b, const __rvtt_vec_t& c, unsigned int mod1);
extern void sfpu_rvtt_sfpnop();
extern void sfpu_rvtt_sfpillegal();
extern void sfpu_rvtt_sfpencc(unsigned int imm12, unsigned int mod1);
extern void sfpu_rvtt_sfpcompc();
extern void sfpu_rvtt_sfppushc();
extern void sfpu_rvtt_sfppopc();
extern void sfpu_rvtt_sfpsetcc_i(unsigned int imm, unsigned int mod1);
extern void sfpu_rvtt_sfpsetcc_v(const __rvtt_vec_t& v, unsigned int mod1);
extern void sfpu_rvtt_sfpscmp_ex(const __rvtt_vec_t& v, unsigned int b, unsigned int mod1);
extern void sfpu_rvtt_sfpvcmp_ex(const __rvtt_vec_t& v1, const __rvtt_vec_t& v2, unsigned int mod1);
extern __rvtt_vec_t sfpu_rvtt_sfpexexp(const __rvtt_vec_t& v, unsigned int mod1);
extern __rvtt_vec_t sfpu_rvtt_sfpexman(const __rvtt_vec_t& v, unsigned int mod1);
extern __rvtt_vec_t sfpu_rvtt_sfpsetexp_i(unsigned int imm, const __rvtt_vec_t& v);
extern __rvtt_vec_t sfpu_rvtt_sfpsetexp_v(const __rvtt_vec_t& dst, const __rvtt_vec_t& src);
extern __rvtt_vec_t sfpu_rvtt_sfpsetman_i(unsigned int imm, const __rvtt_vec_t& v);
extern __rvtt_vec_t sfpu_rvtt_sfpsetman_v(const __rvtt_vec_t& dst, const __rvtt_vec_t& src);
extern __rvtt_vec_t sfpu_rvtt_sfpabs(const __rvtt_vec_t& v, unsigned int mod1);
extern __rvtt_vec_t sfpu_rvtt_sfpand(const __rvtt_vec_t& dst, const __rvtt_vec_t& src);
extern __rvtt_vec_t sfpu_rvtt_sfpor(const __rvtt_vec_t& dst, const __rvtt_vec_t& src);
extern __rvtt_vec_t sfpu_rvtt_sfpnot(const __rvtt_vec_t& v);
extern __rvtt_vec_t sfpu_rvtt_sfpmuli(const __rvtt_vec_t& v, unsigned short imm, unsigned int mod1);
extern __rvtt_vec_t sfpu_rvtt_sfpaddi(const __rvtt_vec_t& v, unsigned short imm, unsigned int mod1);
extern __rvtt_vec_t sfpu_rvtt_sfpdivp2(unsigned short imm, const __rvtt_vec_t& v, unsigned int mod1);
extern __rvtt_vec_t sfpu_rvtt_sfplz(const __rvtt_vec_t& v, unsigned int mod1);
extern __rvtt_vec_t sfpu_rvtt_sfpshft_i(const __rvtt_vec_t& dst, int shift);
extern __rvtt_vec_t sfpu_rvtt_sfpshft_v(const __rvtt_vec_t& dst, const __rvtt_vec_t&src);
extern __rvtt_vec_t sfpu_rvtt_sfpiadd_i(short imm, const __rvtt_vec_t& src, unsigned int mod1);
extern __rvtt_vec_t sfpu_rvtt_sfpiadd_i_ex(short imm, const __rvtt_vec_t& src, unsigned int mod1);
extern __rvtt_vec_t sfpu_rvtt_sfpiadd_v_ex(const __rvtt_vec_t& dst, const __rvtt_vec_t& src, unsigned int mod1);
extern __rvtt_vec_t sfpu_rvtt_sfpiadd_v(const __rvtt_vec_t& dst, const __rvtt_vec_t& src, unsigned int mod1);
extern __rvtt_vec_t sfpu_rvtt_sfpsetsgn_i(unsigned short imm, const __rvtt_vec_t& src);
extern __rvtt_vec_t sfpu_rvtt_sfpsetsgn_v(const __rvtt_vec_t& dst, const __rvtt_vec_t& src);
extern float lut_to_fp32(unsigned short v);
extern __rvtt_vec_t sfpu_rvtt_sfplut(const __rvtt_vec_t& l0, const __rvtt_vec_t& l1, const __rvtt_vec_t& l2, const __rvtt_vec_t& dst, unsigned short mod0);
