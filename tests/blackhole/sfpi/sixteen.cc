/*
 * SPDX-FileCopyrightText: © 2023 Tenstorrent Inc.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <type_traits>
#include <cstdint>


class foo {
    int x;

 public:
    foo(int xx) : x(xx) {}
    int operator+(int32_t a) { __builtin_rvtt_wh_sfpnop(); return a + x; }

    template<typename T, std::enable_if_t<std::is_same_v<T, std::int16_t>, int> = 0>
    int operator+(T a) { return a * x; }
};



int moo(int32_t q, int16_t w)
{
    foo x = 5;

    int y = x + q;
    int z = x + w;
    int a = x + 3;

    return y + z + a;
}
