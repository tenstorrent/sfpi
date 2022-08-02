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
    __builtin_rvtt_wh_sfpencc(3, 10);
    
    while (count-- > 0) {
        __rvtt_vec_t a = __builtin_rvtt_wh_sfpload(nullptr, SFPLOAD_MOD0_REBIAS_EXP, 0, 0, 0, 0);
        __rvtt_vec_t b = __builtin_rvtt_wh_sfpload(nullptr, SFPLOAD_MOD0_REBIAS_EXP, 0, 1, 0, 0);

        __rvtt_vec_t zero = __builtin_rvtt_sfpassignlreg(4);
        __rvtt_vec_t d = __builtin_rvtt_wh_sfpmad(a, b, zero, 0);
        __rvtt_vec_t e = __builtin_rvtt_wh_sfpmad(b, a, zero, 0);
        __rvtt_vec_t f = __builtin_rvtt_wh_sfpmad(b, d, zero, 0);

        __builtin_rvtt_wh_sfpstore(nullptr, d, 0, 3, 3, 0, 0);
        __builtin_rvtt_wh_sfpstore(nullptr, e, 0, 3, 4, 0, 0);
        __builtin_rvtt_wh_sfpstore(nullptr, f, 0, 3, 5, 0, 0);

        if (calculate_bias) {
            __rvtt_vec_t bias = __builtin_rvtt_wh_sfpxloadi(nullptr, SFPLOADI_MOD0_FLOATB, f32_to_f16b(BIAS), 0, 0);
            d = __builtin_rvtt_wh_sfpmad(d, bias, zero, 0);
            __builtin_rvtt_wh_sfpstore(nullptr, d, 0, 3, 4, 0, 0);
        }

        // Bump addressing base pointer (not in current project scope)
    }
}

int main(int argc, char* argv[])
{
    test_func(argc, argc);
}
