#!/usr/bin/env bash
# Check all public header declarations before building the candidate compiler.
# Uses generic RV32 for the target integer ABI, with generated fallback builtin
# declarations. This does not exercise Tensix lowering, allocation or assembly.
set -euo pipefail

if [[ $# != 2 ]]; then
    echo "usage: $0 GCC_SOURCE RISCV_CXX" >&2
    exit 2
fi
source_dir=$(cd "$(dirname "$0")/.." && pwd)
definitions="$1/gcc/config/riscv/tt/rvtt-insn.def"
cxx=$2
if [[ ! -r "$definitions" || ! -x "$cxx" ]]; then
    echo "need GCC source rvtt-insn.def and an executable RV32 C++ compiler" >&2
    exit 2
fi
fixture=$(mktemp -d "${TMPDIR:-/tmp}/sfpi-header-declarations.XXXXXX")
trap 'rm -rf "$fixture"' EXIT
"$source_dir/scripts/gen-decls.py" "$definitions" >"$fixture/tensix_builtins.def"
for architecture in WORMHOLE BLACKHOLE QUASAR; do
    "$cxx" -march=rv32im -std=c++17 -D"ARCH_$architecture" \
        -I"$source_dir/include" -I"$fixture" \
        -Wno-attributes -Wno-psabi -Wno-invalid-constexpr \
        -fsyntax-only "$source_dir/scripts/predicate-header-abi.C" \
        "$source_dir/scripts/alignment-header-abi.C"
done
echo "PASS: WH/BH/QSR header declarations (no Tensix backend execution)"
