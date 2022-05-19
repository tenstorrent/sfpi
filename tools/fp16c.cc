#include <cstdio>
#include <cmath>
#include <string>
#include <sfpi_fp16.h>

using namespace sfpi;
using namespace std;

inline float fp16b_to_fp32(const uint32_t val)
{
    union {
        float vfloat;
        uint32_t vui;
    } tmp;

    tmp.vui = val << 16;
    return tmp.vfloat;
}

inline float fp16a_to_fp32(const uint32_t val)
{
    union {
        float vfloat;
        uint32_t vui;
    } tmp;

    tmp.vui = ((val & 0x8000) << 16) | (((val & 0x7c00) << 13) + 0x38000000) | ((val & 0x03FF) << 13);

    //((tmp.vui >> 16) & 0x8000) | ((((tmp.vui & 0x7F800000) - 0x38000000) >> 13) & 0x7c00) | ((tmp.vui >> 13) & 0x03FF);
    return tmp.vfloat;
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

inline int float_to_fp8(float f)
{
    union Converter {
        const float f;
        const uint32_t i;

        constexpr Converter(const float inf) : f(inf) {}
    } tmp(f);
    
    if (f == 0.0) return 0xff;

    unsigned int exp = (tmp.i & 0x7f800000) >> 23;
    if (exp > 127 || exp < 127 - 7) return 0x0BAD;
    
    return ((tmp.i >> 24) & 0x80) | ((127 - exp) << 4) | ((tmp.i >> 19) & 0xf);
}

inline float fp8_to_float(unsigned int x)
{
    if (x > 0xff) return NAN;
    if (x == 0xff) return 0.0;

    unsigned int exp = 127 - ((x & 0x70) >> 4);
    unsigned int y = ((x & 0x80) << 24) | (exp << 23) | ((x & 0xf) << 19);

    union Converter {
        const float f;
        const uint32_t i;

        constexpr Converter(const unsigned int ini) : i(ini) {}
    } tmp(y);
    
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

// Inputs:
//    float value
//    hex int value
//    dec int value
int main(int argc, char* argv[])
{
    if (argc != 2) {
        fprintf(stderr, "Usage: fp16c <val>\n");
        fprintf(stderr, "  where <val> is a float, hex or dec value of the form\n");
        fprintf(stderr, "  X.Y OR 0xX or X\n");
        exit(-1);
    }

    string s = argv[1];

    if (s.find(".") != string::npos ||
        s.find("INF") != string::npos ||
        s.find("inf") != string::npos ||
        s.find("nan") != string::npos ||
        s.find("NAN") != string::npos) {
        float val = stof(s);
        printf("fp16a: 0x%x (%d)\n", (unsigned int)s2vFloat16a(val).get(), (int)s2vFloat16a(val).get());
        printf("fp16b: 0x%x (%d)\n", (unsigned int)s2vFloat16b(val).get(), (int)s2vFloat16b(val).get());
        printf("fp8  : 0x%x\n", (unsigned int)float_to_fp8(val));
        printf("fp32 : 0x%x (%d, %dhi %dlo)\n", (unsigned int)float_to_int(val), (int)float_to_int(val), (unsigned int)float_to_int(val) >> 16, (unsigned int)float_to_int(val) & 0xFFFF);
    } else if (s[0] == '0' && (s[1] == 'x' || s[1] == 'X') && s.length() > 6) {
        int val = stoi(s, nullptr, 0);
        // Must be fp32
        printf("fp32: %12.12f\n", int_to_float(val));
    } else {
        int val = stoi(s, nullptr, 0);
        printf("fp32 (from fp16a): %12.12f\n", fp16a_to_fp32(val));
        printf("fp32 (from fp16b): %12.12f\n", fp16b_to_fp32(val));
        printf("fp32 (from 8)    : %12.12f\n", fp8_to_float(val));
    }
}
