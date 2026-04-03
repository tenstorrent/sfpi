/*
 * SPDX-FileCopyrightText: © 2023 Tenstorrent Inc.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

///////////////////////////////////////////////////////////////////////////////
// sfpi_fp16.h: fp16a/fp16b/int conversions
///////////////////////////////////////////////////////////////////////////////

#pragma once

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

  //    sfpi_inline s2vFloat16 negate() const { return s2vFloat16(value ^ 0x8000, format); }

    sfpi_inline uint32_t get() const { return value; }
    sfpi_inline uint32_t get_format() const { return format; }
};

//////////////////////////////////////////////////////////////////////////////
class s2vFloat16a : public s2vFloat16 {
 public:
    sfpi_inline s2vFloat16a(const int32_t in) : s2vFloat16(in, fp16a) {}
    sfpi_inline s2vFloat16a(const uint32_t in) : s2vFloat16(in, fp16a) {}
#if 0
    sfpi_inline s2vFloat16a(const int in) : s2vFloat16a(int32_t (in)) {}
    sfpi_inline s2vFloat16a(const unsigned int in) : s2vFloat16a(uint32_t (in)) {}
#endif
};

//////////////////////////////////////////////////////////////////////////////
class s2vFloat16b : public s2vFloat16 {
 public:
    sfpi_inline s2vFloat16b(const float in) : s2vFloat16(in, fp16b) {}
    sfpi_inline s2vFloat16b(const int32_t in) : s2vFloat16(in, fp16b) {}
    sfpi_inline s2vFloat16b(const uint32_t in) : s2vFloat16(in, fp16b) {}
#if 0
    sfpi_inline s2vFloat16b(const int in) : s2vFloat16b(int32_t (in)) {}
    sfpi_inline s2vFloat16b(const unsigned int in) : s2vFloat16b(uint32_t (in)) {}
#endif
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
