///////////////////////////////////////////////////////////////////////////////
// sfpi_fp16.h: fp16a/fp16b/int conversions
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include <cstdint>

#if defined(ARCH_GRAYSKULL)
#include <grayskull/sfpi_hw.h>
#elif defined(ARCH_WORMHOLE)
#include <wormhole/sfpi_hw.h>
#endif

namespace sfpi {

//////////////////////////////////////////////////////////////////////////////
class s2vFloat16 {
 public:
    enum Format {
        fp16a = SFPLOADI_MOD0_FLOATA,
        fp16b = SFPLOADI_MOD0_FLOATB
    };

 private:
    uint32_t value;
    Format format;

    static sfpi_inline uint32_t fp32_to_fp16a(const float val);
    static sfpi_inline uint32_t fp32_to_fp16b(const float val);

 public:
    sfpi_inline s2vFloat16(const float in, const Format f = fp16b);
    sfpi_inline s2vFloat16(const int32_t in, const Format f = fp16b);
    sfpi_inline s2vFloat16(const uint32_t in, const Format f = fp16b);

    sfpi_inline s2vFloat16 negate() const { return s2vFloat16(value ^ 0x8000, format); }

    sfpi_inline uint32_t get() const { return value; }
    sfpi_inline uint32_t get_format() const { return format; }
};

//////////////////////////////////////////////////////////////////////////////
class s2vFloat16a : public s2vFloat16 {
 public:
    sfpi_inline s2vFloat16a(const float in) : s2vFloat16(in, fp16a) {}
    sfpi_inline s2vFloat16a(const double in) : s2vFloat16(static_cast<float>(in), fp16a) {}
#ifndef __clang__
    sfpi_inline s2vFloat16a(const int in) : s2vFloat16(static_cast<int32_t>(in), fp16a) {}
#endif
    sfpi_inline s2vFloat16a(const int32_t in) : s2vFloat16(in, fp16a) {}
#ifndef __clang__
    sfpi_inline s2vFloat16a(const unsigned int in) : s2vFloat16(static_cast<uint32_t>(in), fp16a) {}
#endif
    sfpi_inline s2vFloat16a(const uint32_t in) : s2vFloat16(in, fp16a) {}
};

//////////////////////////////////////////////////////////////////////////////
class s2vFloat16b : public s2vFloat16 {
 public:
    sfpi_inline s2vFloat16b(const float in) : s2vFloat16(in, fp16b) {}
    sfpi_inline s2vFloat16b(const double in) : s2vFloat16(static_cast<float>(in), fp16b) {}
#ifndef __clang__
    sfpi_inline s2vFloat16b(const int in) : s2vFloat16(static_cast<int32_t>(in), fp16b) {}
#endif
    sfpi_inline s2vFloat16b(const int32_t in) : s2vFloat16(in, fp16b) {}
#ifndef __clang__
    sfpi_inline s2vFloat16b(const unsigned int in) : s2vFloat16(static_cast<uint32_t>(in), fp16b) {}
#endif
    sfpi_inline s2vFloat16b(const uint32_t in) : s2vFloat16(in, fp16b) {}
};

//////////////////////////////////////////////////////////////////////////////
sfpi_inline s2vFloat16::s2vFloat16(const float in, const Format f)
{
    value = (f == fp16a) ? fp32_to_fp16a(in) : fp32_to_fp16b(in);
    format = f;
}

sfpi_inline s2vFloat16::s2vFloat16(const int32_t in, const Format f)
{
    value = in;
    format = f;
}

sfpi_inline s2vFloat16::s2vFloat16(const uint32_t in, const Format f)
{
    value = in;
    format = f;
}

sfpi_inline uint32_t s2vFloat16::fp32_to_fp16a(const float val)
{
    union {
        float vfloat;
        uint32_t vui;
    } tmp;

    tmp.vfloat = val;

    // https://stackoverflow.com/questions/1659440/32-bit-to-16-bit-floating-point-conversion
    // Handles denorms.  May be costly w/ non-immediate values
    const unsigned int b = tmp.vui + 0x00001000;
    const unsigned int e = (b & 0x7F800000) >> 23;
    const unsigned int m = b & 0x007FFFFF;
    const unsigned int result =
        (b & 0x80000000) >> 16 |
        (e > 112) * ((((e - 112) << 10) &0x7C00) | m >> 13) |
        ((e < 113) & (e > 101)) * ((((0x007FF000 + m) >> (125 -e )) + 1) >> 1) |
        (e > 143) * 0x7FFF;
#if 0
    // Simple/faster but less complete
    const unsigned int result =
        ((tmp.vui >> 16) & 0x8000) |
        ((((tmp.vui & 0x7F800000) - 0x38000000) >> 13) & 0x7c00) |
        ((tmp.vui >> 13) & 0x03FF);
#endif

    return result;
}

sfpi_inline uint32_t s2vFloat16::fp32_to_fp16b(const float val)
{
    union {
        float vfloat;
        uint32_t vui;
    } tmp;

    tmp.vfloat = val;

    return tmp.vui >> 16;
}

}; // namespace sfpi
