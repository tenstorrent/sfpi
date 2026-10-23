#!/usr/bin/env python3
"""Check lltt's public capability gate and exact mask forwarding on the host.

This tests the header contract; target allocation and marker erasure belong to
the compiler's rvtt.exp tests and require a compiler containing the builtin.
"""

import os
from pathlib import Path
import subprocess
import tempfile
import unittest


class LregEffectHeader(unittest.TestCase):
    def compile_and_run(self, available):
        include = Path(__file__).resolve().parents[1] / "include"
        with tempfile.TemporaryDirectory(prefix="sfpi-lreg-effect-") as directory:
            fixture = Path(directory)
            (fixture / "tensix_builtins.h").write_text("""
#pragma once
inline void __builtin_rvtt_ttreplay(volatile unsigned long *, unsigned,
                                   unsigned, unsigned, unsigned, bool, bool) {}
inline void __builtin_rvtt_ttsetrwc(unsigned, unsigned, unsigned,
                                  unsigned, unsigned, unsigned) {}
extern unsigned seen_reads, seen_writes;
inline void __builtin_rvtt_sfprawlreg_effect(unsigned reads, unsigned writes) {
    seen_reads = reads;
    seen_writes = writes;
}
""")
            # lltt includes tensix_builtins.h with quotes, so copy lltt only
            # into the fixture to select these declarations without touching
            # the installed target compiler or generated builtin declarations.
            (fixture / "lltt.h").write_text((include / "lltt.h").read_text())
            (fixture / "test.cc").write_text("""
#include <cstdint>
#undef __has_builtin
#define __has_builtin(NAME) sfpi_test_has_##NAME
#define sfpi_test_has___builtin_rvtt_sfprawlreg_effect AVAILABLE
#include "lltt.h"
unsigned seen_reads, seen_writes;
volatile unsigned long __instrn_buffer[1];
#if AVAILABLE
#ifndef LLTT_HAS_LREG_EFFECT
#error Missing capability macro
#endif
int main(int argc, char **) {
    lltt::lreg_effect(0x81u, 0x42u);
    if (seen_reads != 0x81u || seen_writes != 0x42u) return 1;
    lltt::lreg_effect(unsigned(argc), unsigned(argc + 1));
    return seen_reads != unsigned(argc) || seen_writes != unsigned(argc + 1);
}
#else
#ifdef LLTT_HAS_LREG_EFFECT
#error Capability advertised without compiler support
#endif
// Defining this name proves no unsupported wrapper was declared.
namespace lltt { constexpr unsigned lreg_effect = 7; }
int main() { return lltt::lreg_effect != 7; }
#endif
""")
            command = [os.environ.get("CXX", "c++"), "-std=c++17",
                       "-Wno-attributes", f"-DAVAILABLE={int(available)}",
                       str(fixture / "test.cc"), "-o", str(fixture / "test")]
            subprocess.run(command, check=True, capture_output=True, text=True)
            subprocess.run([str(fixture / "test")], check=True)

    def test_supported(self):
        self.compile_and_run(True)

    def test_unsupported(self):
        self.compile_and_run(False)


if __name__ == "__main__":
    unittest.main()
