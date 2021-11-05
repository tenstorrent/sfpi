#include <cstdio>
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

sfpi_inline float fp16a_to_fp32(const uint32_t val)
{
    union {
        float vfloat;
        uint32_t vui;
    } tmp;

    tmp.vui = ((val & 0x8000) << 16) | (((val & 0x7c00) << 13) + 0x38000000) | ((val & 0x03FF) << 13);

    //((tmp.vui >> 16) & 0x8000) | ((((tmp.vui & 0x7F800000) - 0x38000000) >> 13) & 0x7c00) | ((tmp.vui >> 13) & 0x03FF);
    return tmp.vfloat;
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
        printf("fp16a: 0x%x (%d)\n", (unsigned int)ScalarFP16a(val).get(), (int)ScalarFP16a(val).get());
        printf("fp16b: 0x%x (%d)\n", (unsigned int)ScalarFP16b(val).get(), (int)ScalarFP16b(val).get());
    } else {
        int val = stoi(s, nullptr, 0);
        printf("fp32 (from fp16a): %f\n", fp16a_to_fp32(val));
        printf("fp32 (from fp16b): %f\n", fp16b_to_fp32(val));
    }
}
