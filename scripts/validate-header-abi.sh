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

# ---------------------------------------------------------------------------
# Templates, on every target.
#
# The TU above catches an arity break only where the call site is NOT inside a
# template: a builtin called with type-dependent arguments is not checked until
# something instantiates it.  Re-injecting the two-argument sfpwriteconfig_v
# bug proves the gap -- it fires at sfpi_crosslane.h (non-template) but passes
# silently at sfpi_classes.h vCReg::operator=, which is a class template.
#
# It also compiled Blackhole only, while the per-ISA RVTT_OVR rows in
# rvtt-insn.def give Quasar different arities for the same builtin.  That is a
# real break: sfpswap takes three operands on WH/BH and four on QSR.
#
# So: instantiate the dependent-argument templates, and build them for every
# target this compiler supports.  Compilation is the whole assertion here --
# the encoding contract stays with the Blackhole TU above.
cat >"$output/header-abi-templates.C" <<'EOF'
namespace ckernel {
volatile unsigned long *instrn_buffer;
}

#include <sfpi.h>
#include <sfpi_crosslane.h>

using namespace sfpi;

__attribute__((noinline))
void sfpi_header_abi_templates()
{
    // vCReg::operator= -- a class template; the original sfpwriteconfig_v
    // arity break lived here and was invisible until instantiated.
    vInt seed = 1;
    vConstIntPrgm1 = seed;

    // sort2 / sort2_rows -- sfpswap with a type-dependent V, four operands on
    // QSR and three elsewhere.
    vFloat fa = 0.0f, fb = 1.0f;
    sort2<SortOrder::Ascending>(fa, fb);
    sort2<SortOrder::Descending>(fa, fb);
    sort2_rows<RowPattern::MinAll>(fa, fb);
    sort2_rows<RowPattern::Min01Max23>(fa, fb);

    vSMag sa = as<vSMag>(fa), sb = as<vSMag>(fb);
    sort2<SortOrder::Ascending>(sa, sb);
    sort2_rows<RowPattern::Min02Max13>(sa, sb);

    dst_reg[0] = fa + as<vFloat>(sa);
}
EOF

template_targets=()
for probe in "-mcpu=tt-wh-tensix -DARCH_WORMHOLE"              "-mcpu=tt-bh-tensix -DARCH_BLACKHOLE"              "-march=rv32im_xtttensixqsr -mabi=ilp32 -DARCH_QUASAR"; do
    # shellcheck disable=SC2086
    if "$cxx" $probe -E -x c++ /dev/null -o /dev/null 2>/dev/null; then
        template_targets+=("$probe")
    fi
done
if [[ ${#template_targets[@]} -eq 0 ]]; then
    echo "no tensix target accepted by this compiler" >&2
    exit 1
fi
for target in "${template_targets[@]}"; do
    # shellcheck disable=SC2086
    "$cxx" $target -O2 \
        -I"$install/include" -fno-exceptions -fno-rtti -Werror \
        -Wno-error=deprecated-declarations \
        -S "$output/header-abi-templates.C" -o "$output/header-abi-templates.S" \
        || { echo "template ABI probe failed for: $target" >&2; exit 1; }
    if grep -q '__builtin_rvtt_' "$output/header-abi-templates.S"; then
        echo "template ABI probe left an rvtt builtin unresolved: $target" >&2
        exit 1
    fi
done

compiler_version=$("$cxx" --version)
compiler_version=${compiler_version%%$'\n'*}
echo "PASS: SFPI header/compiler ABI ($compiler_version)"
echo "targets: ${#template_targets[@]} (templates instantiated)"
echo "cc1plus: $cc1plus"
if [[ $temporary -eq 0 ]]; then
    echo "assembly: $output/header-abi.S"
    echo "object: $output/header-abi.o"
fi
