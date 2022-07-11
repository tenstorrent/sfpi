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
    __builtin_rvtt_gs_sfpencc(3, 10);
    
    while (count-- > 0) {
        __rvtt_vec_t a = __builtin_rvtt_gs_sfpload(nullptr, SFPLOAD_MOD0_REBIAS_EXP, 0);
        __rvtt_vec_t b = __builtin_rvtt_gs_sfpload(nullptr, SFPLOAD_MOD0_REBIAS_EXP, 1);

        __rvtt_vec_t zero = __builtin_rvtt_sfpassignlr(4);
        __rvtt_vec_t d = __builtin_rvtt_gs_sfpmad(a, b, zero, 1);
        __rvtt_vec_t e = __builtin_rvtt_gs_sfpmad(b, a, zero, 1);
        __rvtt_vec_t f = __builtin_rvtt_gs_sfpmad(b, d, zero, 1);

        __builtin_rvtt_gs_sfpstore(nullptr, d, SFPSTORE_MOD0_REBIAS_EXP, 3);
        __builtin_rvtt_gs_sfpstore(nullptr, e, SFPSTORE_MOD0_REBIAS_EXP, 4);
        __builtin_rvtt_gs_sfpstore(nullptr, f, SFPSTORE_MOD0_REBIAS_EXP, 5);

        if (calculate_bias) {
            __rvtt_vec_t bias = __builtin_rvtt_gs_sfpxloadi(nullptr, SFPLOADI_MOD0_FLOATB, f32_to_f16b(BIAS));
            d = __builtin_rvtt_gs_sfpmad(d, bias, zero, 1);
            __builtin_rvtt_gs_sfpstore(nullptr, d, SFPSTORE_MOD0_REBIAS_EXP, 4);
        }

        // Bump addressing base pointer (not in current project scope)
    }
}

int main(int argc, char* argv[])
{
    test_func(argc, argc);
}
