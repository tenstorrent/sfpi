#!/usr/bin/env bash

# Compile a real SFPI translation unit against the selected compiler.  This
# catches header/compiler ABI skew that a header-only syntax check misses.

set -euo pipefail

if [[ $# -lt 1 || $# -gt 2 ]]; then
    echo "usage: $0 SFPI_INSTALL [OUTPUT_DIR]" >&2
    exit 2
fi

if [[ ! -d "$1" ]]; then
    echo "incomplete SFPI install: $1" >&2
    exit 2
fi
install=$(cd "$1" && pwd)
cxx="$install/compiler/bin/riscv-tt-elf-g++"
temporary=0

if [[ ! -x "$cxx" || ! -d "$install/include" ]]; then
    echo "incomplete SFPI install: $install" >&2
    exit 2
fi

if [[ $# -eq 2 ]]; then
    output=$2
    if [[ -e "$output" && ! -d "$output" ]]; then
        echo "validation output is not a directory: $output" >&2
        exit 2
    fi
    mkdir -p "$output"
    output=$(cd "$output" && pwd)
    if [[ -n $(find "$output" -mindepth 1 -print -quit) ]]; then
        echo "validation output must be empty: $output" >&2
        exit 2
    fi
else
    output=$(mktemp -d "${TMPDIR:-/tmp}/sfpi-header-abi.XXXXXX")
    temporary=1
fi

cleanup() {
    if [[ $temporary -eq 1 ]]; then
        rm -rf "$output"
    fi
}
trap cleanup EXIT

cc1plus=$("$cxx" -print-prog-name=cc1plus)
if [[ "$cc1plus" != /* ]]; then
    cc1plus=$(PATH="$(dirname "$cxx"):$PATH" command -v "$cc1plus" || true)
fi
if [[ -z "$cc1plus" || ! -x "$cc1plus" ]]; then
    echo "compiler driver did not resolve an executable cc1plus" >&2
    exit 1
fi
cc1plus=$(realpath "$cc1plus")
compiler_root=$(realpath "$install/compiler")
case "$cc1plus" in
    "$compiler_root"/*) ;;
    *)
        echo "compiler driver resolved cc1plus outside its install: $cc1plus" >&2
        exit 1
        ;;
esac

cat >"$output/header-abi.C" <<'EOF'
namespace ckernel {
volatile unsigned long *instrn_buffer;
}

#include <sfpi.h>

using namespace sfpi;

__attribute__((noinline))
void sfpi_header_abi()
{
    // Keep all SFPU values local: SFPU-valued function parameters are not a
    // supported target ABI and would test an unrelated refusal.
    vUInt input = 1u;

    // This implicit conversion requires the out-of-class definition in
    // sfpi_funcs.h; a declaration alone fails under always_inline.
    vInt converted = input;

    // vCReg assignment must call the compiler's three-argument value-form
    // SFPCONFIG builtin: value, modifier, destination.
    vConstIntPrgm0 = converted;
    dst_reg[0] = converted;

    // Exercise the compiler/assembler contract for SFPGT constant registers.
    // The compiler may use L10 for 1.0f here; assemblers predating the paired
    // SFPGT/SFPLE update incorrectly reject first operands above L7.
    vFloat val = l_reg[LRegs::LReg3];
    vFloat result = 0.0f;
    v_if (val < 1.0f) {
        result = 1.0f;
    }
    v_elseif (val <= 2.0f) {
        result = 2.0f;
    }
    v_endif;
    l_reg[LRegs::LReg3] = result;
}
EOF

"$cxx" -mcpu=tt-bh-tensix -DARCH_BLACKHOLE -O2 \
    -I"$install/include" -fno-exceptions -fno-rtti -Werror \
    -Wno-error=deprecated-declarations \
    -S "$output/header-abi.C" -o "$output/header-abi.S"

if ! grep -Eq '(^|[[:space:]])SFPCONFIG([[:space:]]|$)' "$output/header-abi.S"; then
    echo "header ABI smoke emitted no SFPCONFIG" >&2
    exit 1
fi
if ! grep -Eq '(^|[[:space:]])SFPSTORE([[:space:]]|$)' "$output/header-abi.S"; then
    echo "header ABI smoke emitted no SFPSTORE" >&2
    exit 1
fi
if grep -q '__builtin_rvtt_' "$output/header-abi.S"; then
    echo "header ABI smoke left an rvtt builtin unresolved" >&2
    exit 1
fi
if ! grep -Eq '(^|[[:space:]])SFPGT[[:space:]]+L10,[[:space:]]*L3,' "$output/header-abi.S"; then
    echo "header ABI smoke did not exercise SFPGT with constant register L10" >&2
    exit 1
fi

# Unlike the assembly inspection above, this invokes the paired assembler and
# rejects compiler/binutils combinations that disagree about SFPU operands.
"$cxx" -mcpu=tt-bh-tensix -DARCH_BLACKHOLE -O2 \
    -I"$install/include" -fno-exceptions -fno-rtti -Werror \
    -Wno-error=deprecated-declarations \
    -c "$output/header-abi.C" -o "$output/header-abi.o"

compiler_version=$("$cxx" --version)
compiler_version=${compiler_version%%$'\n'*}
echo "PASS: SFPI header/compiler ABI ($compiler_version)"
echo "cc1plus: $cc1plus"
if [[ $temporary -eq 0 ]]; then
    echo "assembly: $output/header-abi.S"
    echo "object: $output/header-abi.o"
fi
