typedef float __rvtt_vec_t __attribute__((vector_size(64*4)));

constexpr unsigned int SFPLOAD_MOD0_REBIAS_EXP = 1;
constexpr unsigned int SFPLOADI_MOD0_FLOATB = 0;
constexpr unsigned int SFPSTORE_MOD0_REBIAS_EXP = 0;

constexpr unsigned int CREG0 = 4;

constexpr float BIAS = 0.5F;


inline unsigned short f32_to_f16b(const float val)
{
    union {
        float vfloat;
        unsigned int vui;
    } tmp;

    tmp.vfloat = val;
    return tmp.vui >> 16;
}

void test_func(int count, bool calculate_bias)
{
    __builtin_riscv_sfpencc(3, 10);
    
    while (count-- > 0) {
        __rvtt_vec_t a = __builtin_riscv_sfpload(nullptr, SFPLOAD_MOD0_REBIAS_EXP, 0);
        __rvtt_vec_t b = __builtin_riscv_sfpload(nullptr, SFPLOAD_MOD0_REBIAS_EXP, 1);

        __rvtt_vec_t d = __builtin_riscv_sfpmul(a, b, 1);
        __rvtt_vec_t e = __builtin_riscv_sfpmul(b, a, 1);
        __rvtt_vec_t f = __builtin_riscv_sfpmul(b, d, 1);

        __builtin_riscv_sfpstore_v(nullptr, d, SFPSTORE_MOD0_REBIAS_EXP, 3);
        __builtin_riscv_sfpstore_v(nullptr, e, SFPSTORE_MOD0_REBIAS_EXP, 4);
        __builtin_riscv_sfpstore_v(nullptr, f, SFPSTORE_MOD0_REBIAS_EXP, 5);

        if (calculate_bias) {
            __rvtt_vec_t bias = __builtin_riscv_sfploadi(nullptr, SFPLOADI_MOD0_FLOATB, f32_to_f16b(BIAS));
            d = __builtin_riscv_sfpmul(d, bias, 1);
            __builtin_riscv_sfpstore_v(nullptr, d, SFPSTORE_MOD0_REBIAS_EXP, 4);
        }

        // Bump addressing base pointer (not in current project scope)
    }
}
