/*
 * SPDX-FileCopyrightText: © 2023 Tenstorrent Inc.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

// Test LUT
// Early version had out of registers problem triggered by this

typedef float v64sf __attribute__((vector_size(64*4)));

int main(int argc, char* argv[])
{
    v64sf l0, l1, l2, l3;

    l0 = __builtin_rvtt_wh_sfpxloadi(nullptr, 0, 12, 0, 0);
    l1 = __builtin_rvtt_wh_sfpxloadi(nullptr, 0, 13, 0, 0);
    l2 = __builtin_rvtt_wh_sfpxloadi(nullptr, 0, 14, 0, 0);
    l3 = __builtin_rvtt_wh_sfpxloadi(nullptr, 0, 6, 0, 0);
    l3 = __builtin_rvtt_wh_sfplut(l1, l2, l0, l3, 0);
    l3 = __builtin_rvtt_wh_sfplut(l1, l2, l0, l3, 4);
}
