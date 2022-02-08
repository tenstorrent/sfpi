#include "sfpu.h"

using namespace sfpu;
using namespace sfpi;

namespace sfpu {

SFPUDReg sfpu_dreg;
SFPUCC sfpu_cc;
__rvtt_vec_t sfpu_lreg[4];

};

static unsigned int cmp_ex_to_setcc_mod1_map[] = {
  0,
  SFPSETCC_MOD1_LREG_LT0,
  SFPSETCC_MOD1_LREG_EQ0,
  SFPSETCC_MOD1_LREG_GTE0,
  SFPSETCC_MOD1_LREG_NE0,
};

///////////////////////////////////////////////////////////////////////////////
SFPUDReg::SFPUDReg()
{ 
    for (int j = 0; j < SFPU_SIZE; j++) {
        for (int i = 0; i < SFPU_WIDTH; i++) {
            regs[j][i] = 0xDEAD;
        }
    }
}

void SFPUDReg::load(unsigned int out[SFPU_WIDTH], const int addr) const
{
    for (int i = 0; i < SFPU_WIDTH; i++) {
        if (sfpu_cc.enabled(i)) {
            // XXXXX FIXME this is for fp16 only
            out[i] = regs[addr][i] << 3;
        }
    }
}

void SFPUDReg::store_int(const unsigned int data[SFPU_WIDTH], const int addr)
{
    for (int i = 0; i < SFPU_WIDTH; i++) {
        if (sfpu_cc.enabled(i)) {
            regs[addr][i] = data[i];
        }
    }
}

void SFPUDReg::store_float(const unsigned int data[SFPU_WIDTH], const int addr)
{
    // XXXXXFIXME handle rebias on format A
    for (int i = 0; i < SFPU_WIDTH; i++) {
        if (sfpu_cc.enabled(i)) {
            regs[addr][i] = data[i] >> 3;
        }
    }
}

void SFPUDReg::store_float(const float data[SFPU_WIDTH], const int addr)
{
    for (int i = 0; i < SFPU_WIDTH; i++) {
        if (sfpu_cc.enabled(i)) {
            regs[addr][i] = float_to_fp16b(data[i]) >> 3;
        }
    }
}

void SFPUDReg::store_float(const float data, const int row, const int col)
{
    if (sfpu_cc.enabled(col)) {
        regs[row][col] = float_to_fp16b(data) >> 3;
    }
}

///////////////////////////////////////////////////////////////////////////////
SFPUCC::SFPUCC() : stack_ptr(0), deferred_result_pending(false)
{
    for (int i = 0; i < SFPU_WIDTH; i++) {
        enable[i] = false;
        result[i] = true;
        for (int j = 0; j < SFPU_CC_DEPTH; j++) {
            enable_stack[j][i] = false;
            result_stack[j][i] = false;
        }
    }
}

void SFPUCC::set_enable(const bool value)
{
    for (int i = 0; i < SFPU_WIDTH; i++) {
        enable[i] = value;
    }
}

void SFPUCC::comp_enable()
{
    for (int i = 0; i < SFPU_WIDTH; i++) {
        enable[i] = !enable[i];
    }
}

void SFPUCC::set_result(const bool use_enable, const bool value)
{
    for (int i = 0; i < SFPU_WIDTH; i++) {
        result[i] = value && (!use_enable || enable[i]);
    }
}

void SFPUCC::deferred_init()
{
    deferred_result_pending = true;
    for (int i = 0; i < SFPU_WIDTH; i++) {
        deferred_result[i] = result[i];
    }
}

void SFPUCC::deferred_commit()
{
    if (deferred_result_pending) {
        for (int i = 0; i < SFPU_WIDTH; i++) {
            result[i] = deferred_result[i];
        }
        deferred_result_pending = false;
    }
}


void SFPUCC::push()
{
    if (stack_ptr == SFPU_CC_DEPTH) {
        fprintf(stderr, "CC stack overflow on push, aborting");
        exit(-1);
   }
    for (int i = 0; i < SFPU_WIDTH; i++) {
        enable_stack[stack_ptr][i] = enable[i];
        result_stack[stack_ptr][i] = result[i];
    }

    stack_ptr++;
}

void SFPUCC::pop()
{
    if (stack_ptr == 0) {
        fprintf(stderr, "CC stack underflow on pop, aborting");
        exit(-1);
    }
    stack_ptr--;
    for (int i = 0; i < SFPU_WIDTH; i++) {
        enable[i] = enable_stack[stack_ptr][i];
        result[i] = result_stack[stack_ptr][i];
    }
}

void SFPUCC::comp()
{
    for (int i = 0; i < SFPU_WIDTH; i++) {
        if (stack_ptr == 0 || (enable_stack[stack_ptr - 1][i] && result_stack[stack_ptr - 1][i])) {
            set_result(true, i, !enabled(i));
        } else {
            set_result(false, i, false);
        }
    }
}

void SFPUCC::deferred_comp()
{
    for (int i = 0; i < SFPU_WIDTH; i++) {
        if (stack_ptr == 0 || (enable_stack[stack_ptr - 1][i] && result_stack[stack_ptr - 1][i])) {
            deferred_set_result(true, i, !enabled(i));
        } else {
            deferred_set_result(false, i, false);
        }
    }
}

void SFPUCC::dump()
{
    char enabled_string[2] = { 'd', 'E' };
    if (deferred_result_pending) {
        printf("cc(d): ");
    } else {
        printf("cc: ");
    }

    for (int i = 0; i < SFPU_WIDTH; i++) {
        if (sfpu_cc.deferred_result_pending) {
            printf("[%d]%c%c  ", i, enabled_string[!sfpu_cc.enable[i]], enabled_string[sfpu_cc.deferred_result[i]]);
        } else {
            printf("[%d]%c%c  ", i, enabled_string[!sfpu_cc.enable[i]], enabled_string[sfpu_cc.result[i]]);
        }
    }
    printf("\n");
}

void SFPUCC::dump(int i)
{
    char enabled_string[2] = { 'd', 'E' };
    if (sfpu_cc.deferred_result_pending) {
        printf("[%d]d%c%c  ", i, enabled_string[!sfpu_cc.enable[i]], enabled_string[sfpu_cc.deferred_result[i]]);
    } else {
        printf("[%d]%c%c  ", i, enabled_string[!sfpu_cc.enable[i]], enabled_string[sfpu_cc.result[i]]);
    }
}

///////////////////////////////////////////////////////////////////////////////
constexpr __rvtt_vec_t::__rvtt_vec_t(ConstructType unused_for_now) : values()
{
    // Default constructor is called to initialize Tile-ID for CRegs
    for (int i = 0; i < SFPU_WIDTH; i++) {
        values[i] = i;
    }
}

constexpr __rvtt_vec_t::__rvtt_vec_t(const unsigned int v) : values()
{
    for (int i = 0; i < SFPU_WIDTH; i++) {
        values[i] = v;
    }
}

void __rvtt_vec_t::dump(int start, int stop) const
{
    char enabled_string[2] = { 'd', 'E' };
    for (int i = start; i < stop; i++) {
        printf("reg[%d]: (%c) 0x%x %f\n", i, enabled_string[sfpu_cc.enabled(i)], get_uint(i), get_float(i));
    }
}

void __rvtt_vec_t::dump(int i) const
{
    char enabled_string[2] = { 'd', 'E' };
    printf("reg[%d]: (%c) 0x%x %f\n", i, enabled_string[sfpu_cc.enabled(i)], get_uint(i), get_float(i));
}

///////////////////////////////////////////////////////////////////////////////
class CRegInternal {
 private:
    static constexpr __rvtt_vec_t cregs[16] {
        0xDEADBEEF >> 13, 0xDEADBEEF >> 13, 0xDEADBEEF >> 13, 0xDEADBEEF >> 13,
        0x00000000 >> 13, 0x3F316000 >> 13, 0xBF80E000 >> 13, 0x3FB8A000 >> 13,
        0x3F564000 >> 13, 0xBF000000 >> 13, 0x3F800000 >> 13, 0xBF800000 >> 13,
        0x3B000000 >> 13, 0xBF2CC000 >> 13, 0xBEB08000 >> 13, __rvtt_vec_t::ConstructType::TileId
    };

 public:
    constexpr const __rvtt_vec_t& operator[](const int i) const { return cregs[i]; }
};

///////////////////////////////////////////////////////////////////////////////
constexpr CRegInternal kCRegInternal;

///////////////////////////////////////////////////////////////////////////////
__rvtt_vec_t sfpu_rvtt_sfpload(unsigned int mod0, unsigned int addr)
{
    __rvtt_vec_t tmp;

    sfpu_cc.deferred_commit();

    // XXXXXFIXME handle rebias on format A
    sfpu_dreg.load(tmp.get_data_write(), addr);

    return tmp;
}

///////////////////////////////////////////////////////////////////////////////
__rvtt_vec_t sfpu_rvtt_sfpassignlr(unsigned int lr)
{
    return (lr < 4) ? sfpu_lreg[lr] : kCRegInternal[lr];
}

void sfpu_rvtt_sfpstore(const __rvtt_vec_t& v, unsigned int mod0, unsigned int addr)
{
    sfpu_cc.deferred_commit();
    if (mod0 & SFPSTORE_MOD0_INT) {
        sfpu_dreg.store_int(v.get_data_read(), addr);
    } else {
        sfpu_dreg.store_float(v.get_data_read(), addr);
    }
}

__rvtt_vec_t sfpu_rvtt_sfploadi(unsigned int mod0, unsigned short value)
{
    unsigned int converted;

    switch (mod0) {

    case SFPLOADI_MOD0_FLOATB:
        converted = static_cast<unsigned int>(value) << 3;
        break;

    case SFPLOADI_MOD0_FLOATA:
        {
            unsigned int val32 = static_cast<unsigned int>(value);
            converted =
                ((val32 & FP16A_SGN_MASK) << 3) |
                (((val32 & FP16A_EXP_MASK) - ((FP16A_EXP_BIAS - TF32_EXP_BIAS) << FP16A_EXP_SHIFT)) << (TF32_EXP_SHIFT - FP16A_EXP_SHIFT)) |
                (val32 & FP16A_MAN_MASK);

        }
        break;

    case SFPLOADI_MOD0_USHORT:
        // unsigned int
        converted = value;
        break;

    case SFPLOADI_MOD0_SHORT:
        // signed int
        converted = value;
        if (value & BIT_15_MASK) {
            converted |= 0xFFFF0000;
        }
        break;

    default:
        throw;
    }

    sfpu_cc.deferred_commit();
    __rvtt_vec_t tmp;
    for (int i = 0; i < SFPU_WIDTH; i++) {
        if (sfpu_cc.enabled(i)) {
            tmp.set_uint(i, converted);
        }
    }

    return tmp;
}

__rvtt_vec_t sfpu_rvtt_sfpmov(const __rvtt_vec_t& v, unsigned int mod1)
{
    unsigned int mask = 0;

    if (mod1 == SFPMOV_MOD1_COMPSIGN) {
        mask = BIT_18_MASK;
    }

    sfpu_cc.deferred_commit();
    __rvtt_vec_t t;
    for (int i = 0; i < SFPU_WIDTH; i++) {
        if (sfpu_cc.enabled(i)) {
            t.set_uint(i, v.get_uint(i) ^ mask);
        }
    }

    return t;
}

__rvtt_vec_t sfpu_rvtt_sfpadd(const __rvtt_vec_t& a, const __rvtt_vec_t& b, unsigned int mod1)
{
    __rvtt_vec_t tmp;
    float offset = 0.0F;

    if (mod1 == SFPMAD_MOD1_OFFSET_POSH) {
        offset = 0.5F;
    } else if (mod1 == SFPMAD_MOD1_OFFSET_NEGH) {
        offset = -0.5F;
    }

    sfpu_cc.deferred_commit();
    for (int i = 0; i < SFPU_WIDTH; i++) {
        if (sfpu_cc.enabled(i)) {
            tmp.set_float(i, a.get_float(i) + b.get_float(i) + offset);
        }
    }

    return tmp;
}

__rvtt_vec_t sfpu_rvtt_sfpmul(const __rvtt_vec_t& a, const __rvtt_vec_t& b, unsigned int mod1)
{
    __rvtt_vec_t tmp;
    float offset = 0.0F;

    if (mod1 == SFPMAD_MOD1_OFFSET_POSH) {
        offset = 0.5F;
    } else if (mod1 == SFPMAD_MOD1_OFFSET_NEGH) {
        offset = -0.5F;
    }

    sfpu_cc.deferred_commit();
    for (int i = 0; i < SFPU_WIDTH; i++) {
        if (sfpu_cc.enabled(i)) {
            tmp.set_float(i, a.get_float(i) * b.get_float(i) + offset);
        }
    }

    return tmp;
}

__rvtt_vec_t sfpu_rvtt_sfpmad(const __rvtt_vec_t& a, const __rvtt_vec_t& b, const __rvtt_vec_t& c, unsigned int mod1)
{
    __rvtt_vec_t tmp;
    float offset = 0.0F;

    if (mod1 == SFPMAD_MOD1_OFFSET_POSH) {
        offset = 0.5F;
    } else if (mod1 == SFPMAD_MOD1_OFFSET_NEGH) {
        offset = -0.5F;
    }

    sfpu_cc.deferred_commit();
    for (int i = 0; i < SFPU_WIDTH; i++) {
        if (sfpu_cc.enabled(i)) {
            tmp.set_float(i, a.get_float(i) * b.get_float(i) + c.get_float(i) + offset);
        }
    }

    return tmp;
}

void sfpu_rvtt_sfpnop()
{
}

void sfpu_rvtt_sfpillegal()
{
    fprintf(stderr, "Too many SFP conditionals in an if, aborting\n");
    exit(-1);
}

void sfpu_rvtt_sfpencc(unsigned int imm12, unsigned int mod1)
{
    sfpu_cc.deferred_commit();
    switch (mod1) {
    case SFPENCC_MOD1_EU_R1:
        sfpu_cc.set_result(false, true);
        break;
    case SFPENCC_MOD1_EC_R1:
        sfpu_cc.comp_enable();
        sfpu_cc.set_result(false, true);
        break;
    case SFPENCC_MOD1_EI_R1:
        sfpu_cc.set_enable((imm12 & 1) == 1);
        sfpu_cc.set_result(false, true);
        break;
    case SFPENCC_MOD1_EU_RI:
        sfpu_cc.set_result(false, (imm12 & 2) == 2);
        break;
    case SFPENCC_MOD1_EC_RI:
        sfpu_cc.comp_enable();
        sfpu_cc.set_result(false, (imm12 & 2) == 2);
        break;
    case SFPENCC_MOD1_EI_RI:
        sfpu_cc.set_enable((imm12 & 1) == 1);
        sfpu_cc.set_result(false, (imm12 & 2) == 2);
        break;
    default:
        throw;
    };
}

void sfpu_rvtt_sfpcompc()
{
    sfpu_cc.deferred_commit();
    sfpu_cc.comp();
}

void sfpu_rvtt_sfppushc()
{
    sfpu_cc.deferred_commit();
    sfpu_cc.push();
}

void sfpu_rvtt_sfppopc()
{
    sfpu_cc.deferred_commit();
    sfpu_cc.pop();
}

void sfpu_rvtt_sfpsetcc_i(unsigned int imm, unsigned int mod1)
{
    sfpu_cc.deferred_commit();
    for (int i = 0; i < SFPU_WIDTH; i++) {
        if (mod1 == SFPEXEXP_MOD1_NODEBIAS) {
            sfpu_cc.and_result(i, (imm & 1) != 0);
        } else if (mod1 == SFPEXEXP_MOD1_SET_CC_COMP_EXP) {
            sfpu_cc.and_result(i, !sfpu_cc.enabled(i));
        }
    }
}

void sfpu_rvtt_sfpsetcc_v(const __rvtt_vec_t& v, unsigned int mod1)
{
    sfpu_cc.deferred_commit();
    for (int i = 0; i < SFPU_WIDTH; i++) {
        switch (mod1) {
        case SFPSETCC_MOD1_LREG_LT0:
            sfpu_cc.and_result(i, (v.get_uint(i) & BIT_18_MASK) != 0);
            break;
        case SFPSETCC_MOD1_LREG_NE0:
            sfpu_cc.and_result(i, (v.get_uint(i) & BITS_0_TO_18_MASK) != 0);
            break;
        case SFPSETCC_MOD1_LREG_GTE0:
            sfpu_cc.and_result(i, (v.get_uint(i) & BIT_18_MASK) == 0);
            break;
        case SFPSETCC_MOD1_LREG_EQ0:
            sfpu_cc.and_result(i, (v.get_uint(i) & BITS_0_TO_18_MASK) == 0);
            break;
        case SFPSETCC_MOD1_COMP:
            sfpu_cc.and_result(i, !sfpu_cc.enabled(i));
            break;
        default:
            throw;
        }
    }
}

void sfpu_rvtt_sfpscmp_ex(const __rvtt_vec_t& a, unsigned int b, unsigned int mod1)
{
    unsigned int cmp = mod1 & SFPCMP_EX_MOD1_CC_MASK;
    
    if (cmp == SFPCMP_EX_MOD1_CC_LTE || cmp == SFPCMP_EX_MOD1_CC_GT) {
        unsigned int new_mod = mod1 & ~SFPCMP_EX_MOD1_CC_MASK;
        // Inefficient
        sfpu_rvtt_sfpscmp_ex(a, b, new_mod | SFPCMP_EX_MOD1_CC_GTE);
        sfpu_rvtt_sfpscmp_ex(a, b, new_mod | SFPCMP_EX_MOD1_CC_NE);
        if (cmp == SFPCMP_EX_MOD1_CC_LTE) {
            sfpu_rvtt_sfpcompc();
        }
        return;
    }

    if (b != 0) {
        __rvtt_vec_t tmp;
        int loadi_mod = ((mod1 & SFPSCMP_EX_MOD1_FMT_A) == SFPSCMP_EX_MOD1_FMT_A) ? SFPLOADI_MOD0_FLOATA : SFPLOADI_MOD0_FLOATB;
        __rvtt_vec_t op_b = __builtin_rvtt_sfploadi(loadi_mod, b);
        tmp = __builtin_rvtt_sfpmad(op_b, __builtin_rvtt_sfpassignlr(CREG_IDX_NEG_1), a, 0);

        __builtin_rvtt_sfpsetcc_v(tmp, cmp_ex_to_setcc_mod1_map[mod1]);
    } else {
        __builtin_rvtt_sfpsetcc_v(a, cmp_ex_to_setcc_mod1_map[mod1]);
    }
}

void sfpu_rvtt_sfpvcmp_ex(const __rvtt_vec_t& a, const __rvtt_vec_t& b, unsigned int mod1)
{
    unsigned int cmp = mod1 & SFPCMP_EX_MOD1_CC_MASK;

    __rvtt_vec_t tmp = __builtin_rvtt_sfpmad(b,
                                             __builtin_rvtt_sfpassignlr(CREG_IDX_NEG_1),
                                             a,
                                             0);

    if (cmp == SFPCMP_EX_MOD1_CC_LTE || cmp == SFPCMP_EX_MOD1_CC_GT) {
        __builtin_rvtt_sfpsetcc_v(tmp, SFPSETCC_MOD1_LREG_GTE0);
        __builtin_rvtt_sfpsetcc_v(tmp, SFPSETCC_MOD1_LREG_NE0);
        if (cmp == SFPCMP_EX_MOD1_CC_LTE) {
            sfpu_rvtt_sfpcompc();
        }
    } else {
        __builtin_rvtt_sfpsetcc_v(tmp, cmp_ex_to_setcc_mod1_map[mod1]);
    }
}

__rvtt_vec_t sfpu_rvtt_sfpexexp(const __rvtt_vec_t& v, unsigned int mod1)
{
    __rvtt_vec_t tmp;

    sfpu_cc.deferred_commit();
    sfpu_cc.deferred_init();
    for (int i = 0; i < SFPU_WIDTH; i++) {
        if (sfpu_cc.enabled(i)) {
            int exp = (v.get_uint(i) >> TF32_EXP_SHIFT) & 0xFF;
            if ((mod1 & SFPEXEXP_MOD1_NODEBIAS) == 0) {
                exp -= TF32_EXP_BIAS;
            }

            tmp.set_uint(i, exp);

            if ((mod1 & (SFPEXEXP_MOD1_SET_CC_COMP_EXP | SFPEXEXP_MOD1_SET_CC_SGN_EXP)) == SFPEXEXP_MOD1_SET_CC_SGN_EXP) {
                sfpu_cc.deferred_and_result(i, exp < 0);
            } else if ((mod1 & 0xA) == 10) {
                sfpu_cc.deferred_and_result(i, exp >= 0);
            }
        }
    }

    return tmp;
}

__rvtt_vec_t sfpu_rvtt_sfpexman(const __rvtt_vec_t& v, unsigned int mod1)
{
    __rvtt_vec_t tmp;

    sfpu_cc.deferred_commit();
    for (int i = 0; i < SFPU_WIDTH; i++) {
        if (sfpu_cc.enabled(i)) {
            unsigned int man = v.get_uint(i) & TF32_MAN_MASK;
            if (mod1 == SFPEXMAN_MOD1_PAD9) {
                tmp.set_uint(i, man);
            } else if (mod1 == SFPEXMAN_MOD1_PAD8) {
                tmp.set_uint(i, man | (TF32_MAN_MASK + 1));
            }
        }
    }

    return tmp;
}

__rvtt_vec_t sfpu_rvtt_sfpsetexp_i(unsigned int imm, const __rvtt_vec_t& v)
{
    __rvtt_vec_t tmp;

    sfpu_cc.deferred_commit();
    for (int i = 0; i < SFPU_WIDTH; i++) {
        if (sfpu_cc.enabled(i)) {
            // Keep sign bit and mantissa
            tmp.set_uint(i, (v.get_uint(i) & TF32_SGN_MAN_MASK) | ((imm & 0xFF) << TF32_EXP_SHIFT));
        }
    }

    return tmp;
}

__rvtt_vec_t sfpu_rvtt_sfpsetexp_v(const __rvtt_vec_t& dst, const __rvtt_vec_t& src)
{
    __rvtt_vec_t tmp;

    sfpu_cc.deferred_commit();
    for (int i = 0; i < SFPU_WIDTH; i++) {
        if (sfpu_cc.enabled(i)) {
            // Keep sign bit and mantissa
            tmp.set_uint(i, (src.get_uint(i) & TF32_SGN_MAN_MASK) | ((dst.get_uint(i) & 0xFF) << TF32_EXP_SHIFT));
        }
    }

    return tmp;
}

__rvtt_vec_t sfpu_rvtt_sfpsetman_i(unsigned int imm, const __rvtt_vec_t& v)
{
    __rvtt_vec_t tmp;

    sfpu_cc.deferred_commit();
    for (int i = 0; i < SFPU_WIDTH; i++) {
        if (sfpu_cc.enabled(i)) {
            // Keep sign bit and exponent
            tmp.set_uint(i, (v.get_uint(i) & TF32_SGN_EXP_MASK) | (imm & TF32_MAN_MASK));
        }
    }

    return tmp;
}

__rvtt_vec_t sfpu_rvtt_sfpsetman_v(const __rvtt_vec_t& dst, const __rvtt_vec_t& src)
{
    __rvtt_vec_t tmp;

    sfpu_cc.deferred_commit();
    for (int i = 0; i < SFPU_WIDTH; i++) {
        if (sfpu_cc.enabled(i)) {
            // Keep sign bit and exponent
#ifdef ARCH_GRAYSKULL
            // Grayskull bug, bits come from the exponent section
            // wonder if this instruction is even used
            tmp.set_uint(i, (src.get_uint(i) & TF32_SGN_EXP_MASK) | ((dst.get_uint(i) >> 9) & 0x1FF));
#else
            tmp.set_uint(i, (src.get_uint(i) & TF32_SGN_EXP_MASK) | (dst.get_uint(i) & TF32_MAN_MASK));
#endif
        }
    }

    return tmp;
}

__rvtt_vec_t sfpu_rvtt_sfpabs(const __rvtt_vec_t& v, unsigned int mod1)
{
    __rvtt_vec_t tmp;

    sfpu_cc.deferred_commit();
    for (int i = 0; i < SFPU_WIDTH; i++) {
        if (sfpu_cc.enabled(i)) {
            if (mod1 == SFPABS_MOD1_INT) {
                // 19 bit int...relies on upper bits being the sign extension
                int32_t n = v.get_uint(i);
                if (n < 0) n = -n;
                tmp.set_uint(i, n);
            } else {
                // TF32, clear sign bit - 18
                tmp.set_uint(i, v.get_uint(i) & BITS_0_TO_17_MASK);
            }
        }
    }

    return tmp;
}

__rvtt_vec_t sfpu_rvtt_sfpand(const __rvtt_vec_t& dst, const __rvtt_vec_t& src)
{
    __rvtt_vec_t tmp;

    sfpu_cc.deferred_commit();
    for (int i = 0; i < SFPU_WIDTH; i++) {
        if (sfpu_cc.enabled(i)) {
            tmp.set_uint(i, src.get_uint(i) & dst.get_uint(i));
        }
    }

    return tmp;
}

__rvtt_vec_t sfpu_rvtt_sfpor(const __rvtt_vec_t& dst, const __rvtt_vec_t& src)
{
    __rvtt_vec_t tmp;

    sfpu_cc.deferred_commit();
    for (int i = 0; i < SFPU_WIDTH; i++) {
        if (sfpu_cc.enabled(i)) {
            tmp.set_uint(i, src.get_uint(i) | dst.get_uint(i));
        }
    }

    return tmp;
}

__rvtt_vec_t sfpu_rvtt_sfpnot(const __rvtt_vec_t& v)
{
    __rvtt_vec_t tmp;

    sfpu_cc.deferred_commit();
    for (int i = 0; i < SFPU_WIDTH; i++) {
        if (sfpu_cc.enabled(i)) {
            // XXXXX FIXME  - sets high bits above 18, does it matter? think
            // through whether high bits should always be 0
            // pretty sure this is a bug...
            tmp.set_uint(i, ~v.get_uint(i));
        }
    }

    return tmp;
}

__rvtt_vec_t sfpu_rvtt_sfpmuli(const __rvtt_vec_t& v, unsigned short imm, unsigned int mod1)
{
    __rvtt_vec_t tmp;

    float f = SFPUDReg::fp16b_to_float(imm);
    sfpu_cc.deferred_commit();
    for (int i = 0; i < SFPU_WIDTH; i++) {
        if (sfpu_cc.enabled(i)) {
            tmp.set_float(i, v.get_float(i) * f);
        }
    }

    return tmp;
}

__rvtt_vec_t sfpu_rvtt_sfpaddi(const __rvtt_vec_t& v, unsigned short imm, unsigned int mod1)
{
    __rvtt_vec_t tmp;

    float f = SFPUDReg::fp16b_to_float(imm);
    sfpu_cc.deferred_commit();
    for (int i = 0; i < SFPU_WIDTH; i++) {
        if (sfpu_cc.enabled(i)) {
            tmp.set_float(i, v.get_float(i) + f);
        }
    }

    return tmp;
}

__rvtt_vec_t sfpu_rvtt_sfpdivp2(unsigned short imm, const __rvtt_vec_t& v, unsigned int mod1)
{
    __rvtt_vec_t tmp;

    unsigned int exp = (imm & 0xFF) << TF32_EXP_SHIFT;
    sfpu_cc.deferred_commit();
    for (int i = 0; i < SFPU_WIDTH; i++) {
        if (sfpu_cc.enabled(i)) {
            unsigned int old = v.get_uint(i);

            unsigned int setexp;
            if (mod1 == SFPSDIVP2_MOD1_ADD) {
                setexp = ((old & TF32_EXP_MASK) + exp) & TF32_EXP_MASK;
            } else {
                setexp = exp;
            }
            tmp.set_uint(i, ((old & (TF32_SGN_MASK | TF32_MAN_MASK)) | setexp));
        }
    }

    return tmp;
}

__rvtt_vec_t sfpu_rvtt_sfplz(const __rvtt_vec_t& v, unsigned int mod1)
{
    __rvtt_vec_t tmp;

    sfpu_cc.deferred_commit();
    sfpu_cc.deferred_init();
    for (int i = 0; i < SFPU_WIDTH; i++) {
        if (sfpu_cc.enabled(i)) {
            unsigned int val = v.get_uint(i);

            int b;
            for (b = TF32_BITS; b >= 0; b--) {
                if ((val & (1 << b)) != 0) {
                    break;
                }
            }

            tmp.set_uint(i, TF32_BITS - b);

            switch (mod1) {
            case SFPLZ_MOD1_CC_NONE:
                break;
            case SFPLZ_MOD1_CC_NE0:
                sfpu_cc.deferred_and_result(i, val != 0);
                break;
            case SFPLZ_MOD1_CC_COMP:
                sfpu_cc.deferred_comp();
                break;
            case SFPLZ_MOD1_CC_EQ0:
                sfpu_cc.deferred_and_result(i, val == 0);
                break;
            default:
                throw;
            }
        }
    }

    return tmp;
}

__rvtt_vec_t sfpu_rvtt_sfpshft_i(const __rvtt_vec_t& dst, int shift)
{
    __rvtt_vec_t tmp;

    sfpu_cc.deferred_commit();
    for (int i = 0; i < SFPU_WIDTH; i++) {
        if (sfpu_cc.enabled(i)) {
            if (shift < 0) {
                tmp.set_uint(i, dst.get_uint(i) >> -shift);
            } else {
                tmp.set_uint(i, dst.get_uint(i) << shift);
            }
        }
    }

    return tmp;
}

__rvtt_vec_t sfpu_rvtt_sfpshft_v(const __rvtt_vec_t& dst, const __rvtt_vec_t&src)
{
    __rvtt_vec_t tmp;

    sfpu_cc.deferred_commit();
    for (int i = 0; i < SFPU_WIDTH; i++) {
        if (sfpu_cc.enabled(i)) {
            int shift = src.get_uint(i);
            if (shift < 0) {
                tmp.set_uint(i, dst.get_uint(i) >> -shift);
            } else {
                tmp.set_uint(i, dst.get_uint(i) << shift);
            }
        }
    }

    return tmp;
}

__rvtt_vec_t sfpu_rvtt_sfpiadd_i(short imm, const __rvtt_vec_t& src, unsigned int mod1)
{
    __rvtt_vec_t tmp;

    // Note: the instruction for iadd uses a 12 bit imm, but the builtin
    // supports 16 bit imms using loadi
    sfpu_cc.deferred_commit();
    sfpu_cc.deferred_init();
    for (int i = 0; i < SFPU_WIDTH; i++) {
        if (sfpu_cc.enabled(i)) {
            int val = src.get_uint(i) + imm;
            tmp.set_uint(i, val);

            if (mod1 & SFPIADD_MOD1_CC_GTE0) {
                sfpu_cc.deferred_and_result(i, val >= 0);
            } else if ((mod1 & SFPIADD_MOD1_CC_NONE) == 0) {
                // LT0
                sfpu_cc.deferred_and_result(i, val < 0);
            }
        }
    }

    return tmp;
}

__rvtt_vec_t sfpu_rvtt_sfpiadd_i_ex(short imm, const __rvtt_vec_t& src, unsigned int mod1)
{
    __rvtt_vec_t tmp;

    // Note: the instruction for iadd uses a 12 bit imm, but the builtin
    // supports 16 bit imms using loadi
    sfpu_cc.deferred_commit();
    sfpu_cc.deferred_init();
    for (int i = 0; i < SFPU_WIDTH; i++) {
        if (sfpu_cc.enabled(i)) {
            int val = src.get_uint(i);
            val += (mod1 & SFPIADD_EX_MOD1_IS_SUB) ? -imm : imm;
            tmp.set_uint(i, val);

            switch (mod1 & SFPCMP_EX_MOD1_CC_MASK) {
            case SFPCMP_EX_MOD1_CC_GTE:
                sfpu_cc.deferred_and_result(i, val >= 0);
                break;
            case SFPCMP_EX_MOD1_CC_LT:
                sfpu_cc.deferred_and_result(i, val < 0);
                break;
            case SFPCMP_EX_MOD1_CC_EQ:
                sfpu_cc.deferred_and_result(i, val == 0);
                break;
            case SFPCMP_EX_MOD1_CC_NE:
                sfpu_cc.deferred_and_result(i, val != 0);
                break;
            case SFPCMP_EX_MOD1_CC_LTE:
                sfpu_cc.deferred_and_result(i, val <= 0);
                break;
            case SFPCMP_EX_MOD1_CC_GT:
                sfpu_cc.deferred_and_result(i, val > 0);
                break;
            }
        }
    }

    return tmp;
}

__rvtt_vec_t sfpu_rvtt_sfpiadd_v(const __rvtt_vec_t& dst, const __rvtt_vec_t& src, unsigned int mod1)
{
    __rvtt_vec_t tmp;

    sfpu_cc.deferred_commit();
    sfpu_cc.deferred_init();
    for (int i = 0; i < SFPU_WIDTH; i++) {
        if (sfpu_cc.enabled(i)) {
            int val;
            if (mod1 & SFPIADD_MOD1_ARG_2SCOMP_LREG_DST) {
                val = src.get_uint(i) - dst.get_uint(i);
            } else {
                // Positive LREG_DST
                val = src.get_uint(i) + dst.get_uint(i);
            }

            tmp.set_uint(i, val);

            if (mod1 & SFPIADD_MOD1_CC_GTE0) {
                sfpu_cc.deferred_and_result(i, val >= 0);
            } else if ((mod1 & SFPIADD_MOD1_CC_NONE) == 0) {
                // LT0
                sfpu_cc.deferred_and_result(i, val < 0);
            }
        }
    }

    return tmp;
}

__rvtt_vec_t sfpu_rvtt_sfpiadd_v_ex(const __rvtt_vec_t& dst, const __rvtt_vec_t& src, unsigned int mod1)
{
    __rvtt_vec_t tmp;

    unsigned int cmp = mod1 & SFPCMP_EX_MOD1_CC_MASK;

    bool is_sub = ((mod1 & SFPIADD_EX_MOD1_IS_SUB) != 0);
    unsigned int mod = is_sub ? SFPIADD_MOD1_ARG_2SCOMP_LREG_DST : SFPIADD_MOD1_ARG_LREG_DST;
    if (cmp == SFPCMP_EX_MOD1_CC_LT || cmp == SFPCMP_EX_MOD1_CC_GTE) {
        // Perform op w/ compare
        mod |= (cmp == SFPCMP_EX_MOD1_CC_LT) ? SFPIADD_MOD1_CC_LT0 : SFPIADD_MOD1_CC_GTE0;
        tmp = sfpu_rvtt_sfpiadd_v(dst, src, mod);
    } else if (cmp == SFPCMP_EX_MOD1_CC_LTE || cmp == SFPCMP_EX_MOD1_CC_GT) {
        // Perform op w/o compare, compare w/ IADDI
        tmp = sfpu_rvtt_sfpiadd_v_ex(dst, src, (mod1 & ~SFPCMP_EX_MOD1_CC_MASK) | SFPCMP_EX_MOD1_CC_GTE);
        sfpu_rvtt_sfpsetcc_v(tmp, SFPSETCC_MOD1_LREG_NE0);
        if (cmp == SFPCMP_EX_MOD1_CC_LTE) {
            sfpu_rvtt_sfpcompc();
        }
    } else {
        // Perform op w/o compare, compare with SETCC
        mod |= SFPIADD_MOD1_CC_NONE;
        tmp = sfpu_rvtt_sfpiadd_v(dst, src, mod);
        if (cmp != 0) {
            sfpu_rvtt_sfpsetcc_v(tmp, cmp_ex_to_setcc_mod1_map[cmp]);
        }
    }

    return tmp;
}

__rvtt_vec_t sfpu_rvtt_sfpsetsgn_i(unsigned short imm, const __rvtt_vec_t& src)
{
    __rvtt_vec_t tmp;

    unsigned int sign = (static_cast<unsigned int>(imm) & 1) << TF32_SGN_SHIFT;
    sfpu_cc.deferred_commit();
    for (int i = 0; i < SFPU_WIDTH; i++) {
        if (sfpu_cc.enabled(i)) {
            tmp.set_uint(i, (src.get_uint(i) & (TF32_EXP_MASK | TF32_MAN_MASK)) | sign);
        }
    }

    return tmp;
}

__rvtt_vec_t sfpu_rvtt_sfpsetsgn_v(const __rvtt_vec_t& dst, const __rvtt_vec_t& src)
{
    __rvtt_vec_t tmp;

    sfpu_cc.deferred_commit();
    for (int i = 0; i < SFPU_WIDTH; i++) {
        if (sfpu_cc.enabled(i)) {
            unsigned int sign = dst.get_uint(i) & TF32_SGN_MASK;
            tmp.set_uint(i, (src.get_uint(i) & (TF32_EXP_MASK | TF32_MAN_MASK)) | sign);
        }
    }

    return tmp;
}

inline float lut_to_fp32(unsigned short v)
{
    union Converter {
        float f;
        uint32_t i;
    } conv;

    if (v == 0xFF) {
        conv.i = 0;
    } else {
        uint32_t sgn = (v & 0x80) >> 7;
        uint32_t exp = 127 - ((v & 0x70) >> 4); // Note: exponent extender is always positive!
        uint32_t man = (v & 0x0F);
        conv.i = (sgn << FP32_SGN_SHIFT) | (exp << FP32_EXP_SHIFT) | (man << (FP32_MAN_WIDTH - 4));
    }

    return conv.f;
}


__rvtt_vec_t sfpu_rvtt_sfplut(const __rvtt_vec_t& l0,
                              const __rvtt_vec_t& l1,
                              const __rvtt_vec_t& l2,
                              const __rvtt_vec_t& dst,
                               unsigned short mod0)
{
    unsigned short bias_flag = (mod0 & SFPLUT_MOD0_BIAS_MASK);
    float bias = (bias_flag == SFPLUT_MOD0_BIAS_NEG) ? -0.5F : (bias_flag == SFPLUT_MOD0_BIAS_POS) ? 0.5F : 0.0F;
    bool retain_sgn = ((mod0 & SFPLUT_MOD0_SGN_RETAIN) != 0);

    __rvtt_vec_t tmp;
    sfpu_cc.deferred_commit();
    for (int i = 0; i < SFPU_WIDTH; i++) {
        if (sfpu_cc.enabled(i)) {
            int exp = ((dst.get_uint(i) & TF32_EXP_MASK) >> TF32_EXP_SHIFT) - TF32_EXP_BIAS;

            const __rvtt_vec_t& which = (exp < 0) ? l0 : (exp == 0) ? l1 : l2;
            unsigned int in = which.get_uint(i);
            unsigned short a_in = (in >> 8) & 0xFF;
            unsigned short b_in = in & 0xFF;

            float a = lut_to_fp32(a_in);
            float b = lut_to_fp32(b_in);
            float lr3 = dst.get_float(i);

            float result = a * fabs(lr3) + b + bias;

            if (retain_sgn) {
                union {
                    float fval;
                    uint32_t ival;
                };
                fval = lr3;
                uint32_t sgn = ival & FP32_SGN_MASK;
                fval = result;
                uint32_t rest = ival & ~FP32_SGN_MASK;
                ival = rest | sgn;
                result = fval;
            }

            tmp.set_float(i, result);
        }
    }

    return tmp;
}
