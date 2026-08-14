#!/usr/bin/env bash

# Focused, deterministic validation for the opt-in SFPU pressure scheduler.

set -uo pipefail

if [[ $# -lt 1 || $# -gt 2 ]]; then
    echo "usage: $0 SFPI_INSTALL [OUTPUT_DIR]" >&2
    exit 2
fi

repo=$(cd "$(dirname "$0")/.." && pwd)
install=$(cd "$1" && pwd)
output=${2:-/tmp/sfpi-welford-validation}
cxx="$install/compiler/bin/riscv-tt-elf-g++"
include="$install/include"
source_dir="$repo/gcc/gcc/testsuite/g++.target/riscv/tt/sfpi"

if [[ ! -x "$cxx" || ! -d "$include" ]]; then
    echo "incomplete SFPI install: $install" >&2
    exit 2
fi

if [[ -e "$output" && ! -d "$output" ]]; then
    echo "validation output is not a directory: $output" >&2
    exit 2
fi
if [[ -d "$output" && -n $(find "$output" -mindepth 1 -print -quit) ]]; then
    echo "validation output must be empty: $output" >&2
    exit 2
fi

mkdir -p "$output"
failures=0

record_failure() {
    echo "FAIL: $*" >&2
    failures=$((failures + 1))
}

compile() {
    local directory=$1
    local cpu=$2
    local source=$3
    local assembly=$4
    shift 4
    mkdir -p "$directory"
    (
        cd "$directory" &&
        "$cxx" -mcpu="$cpu" -O2 -I"$include" \
            -fno-exceptions -fno-rtti "$@" -S "$source" -o "$assembly"
    )
}

for architecture in wormhole blackhole; do
    case "$architecture" in
        wormhole) cpu=tt-wh-tensix ;;
        blackhole) cpu=tt-bh-tensix ;;
    esac

    arch_output="$output/$architecture"
    minimal="$source_dir/welford-pressure-wh.C"
    rescue="$source_dir/welford-pressure-reorder-wh.C"
    predicated="$source_dir/lp-schedule-predicated-wh.C"
    cfg="$source_dir/lp-schedule-cfg-wh.C"
    live_across="$source_dir/lp-schedule-live-across-wh.C"

    compile "$arch_output/minimal-off" "$cpu" "$minimal" minimal.S \
        >"$arch_output-minimal-off.log" 2>&1 ||
        record_failure "$architecture minimal fixture failed with pass off"
    compile "$arch_output/minimal-on" "$cpu" "$minimal" minimal.S \
        -mtt-tensix-optimize-lp-schedule -fdump-tree-rvtt_lp_schedule \
        >"$arch_output-minimal-on.log" 2>&1 ||
        record_failure "$architecture minimal fixture failed with pass on"
    if [[ -f "$arch_output/minimal-off/minimal.S" && -f "$arch_output/minimal-on/minimal.S" ]]; then
        cmp "$arch_output/minimal-off/minimal.S" "$arch_output/minimal-on/minimal.S" ||
            record_failure "$architecture minimal pass-off/on assembly differs"
    fi

    if compile "$arch_output/rescue-off" "$cpu" "$rescue" rescue.S \
        >"$arch_output-rescue-off.log" 2>&1; then
        record_failure "$architecture late-fold fixture unexpectedly compiled with pass off"
    elif ! grep -q "cannot store sfpu register (register spill)" "$arch_output-rescue-off.log"; then
        record_failure "$architecture pass-off failure was not the expected SFPU spill"
    fi

    compile "$arch_output/rescue-manual" "$cpu" "$rescue" rescue.S \
        -DWELFORD_MANUAL_EARLY_FOLD \
        >"$arch_output-rescue-manual.log" 2>&1 ||
        record_failure "$architecture manual early-fold control failed"

    compile "$arch_output/rescue-on" "$cpu" "$rescue" rescue.S \
        -mtt-tensix-optimize-lp-schedule -fdump-tree-rvtt_lp_schedule \
        >"$arch_output-rescue-on.log" 2>&1 ||
        record_failure "$architecture scheduler did not rescue late-fold fixture"
    rescue_dump=$(find "$arch_output/rescue-on" -name '*rvtt_lp_schedule*' -type f -print -quit)
    if [[ -z "$rescue_dump" ]] ||
       ! grep -Eq 'old-peak=9.*new-peak=8.*applied=yes' "$rescue_dump"; then
        record_failure "$architecture rescue dump lacks the 9-to-8 certificate"
    fi

    compile "$arch_output/predicated-off" "$cpu" "$predicated" predicated.S \
        >"$arch_output-predicated-off.log" 2>&1 ||
        record_failure "$architecture predicated fixture failed with pass off"
    compile "$arch_output/predicated-on" "$cpu" "$predicated" predicated.S \
        -mtt-tensix-optimize-lp-schedule -fdump-tree-rvtt_lp_schedule \
        >"$arch_output-predicated-on.log" 2>&1 ||
        record_failure "$architecture predicated fixture failed with pass on"
    predicated_dump=$(find "$arch_output/predicated-on" -name '*rvtt_lp_schedule*' -type f -print -quit)
    if [[ -z "$predicated_dump" ]] || ! grep -q 'rejected=cc-epoch' "$predicated_dump"; then
        record_failure "$architecture predicated fixture was not explicitly rejected"
    fi
    if [[ -f "$arch_output/predicated-off/predicated.S" && -f "$arch_output/predicated-on/predicated.S" ]]; then
        cmp "$arch_output/predicated-off/predicated.S" "$arch_output/predicated-on/predicated.S" ||
            record_failure "$architecture predicated pass-off/on assembly differs"
    fi

    compile "$arch_output/cfg-off" "$cpu" "$cfg" cfg.S \
        >"$arch_output-cfg-off.log" 2>&1 ||
        record_failure "$architecture CFG fixture failed with pass off"
    compile "$arch_output/cfg-on" "$cpu" "$cfg" cfg.S \
        -mtt-tensix-optimize-lp-schedule -fdump-tree-rvtt_lp_schedule \
        >"$arch_output-cfg-on.log" 2>&1 ||
        record_failure "$architecture CFG fixture failed with pass on"
    cfg_dump=$(find "$arch_output/cfg-on" -name '*rvtt_lp_schedule*' -type f -print -quit)
    if [[ -z "$cfg_dump" ]] || ! grep -q 'rejected=cfg' "$cfg_dump"; then
        record_failure "$architecture CFG fixture was not explicitly rejected"
    fi
    if [[ -f "$arch_output/cfg-off/cfg.S" && -f "$arch_output/cfg-on/cfg.S" ]]; then
        cmp "$arch_output/cfg-off/cfg.S" "$arch_output/cfg-on/cfg.S" ||
            record_failure "$architecture CFG pass-off/on assembly differs"
    fi

    compile "$arch_output/live-across" "$cpu" "$live_across" live-across.S \
        -mtt-tensix-optimize-lp-schedule -fdump-tree-rvtt_lp_schedule \
        >"$arch_output-live-across.log" 2>&1 ||
        record_failure "$architecture live-across fixture failed"
    live_across_dump=$(find "$arch_output/live-across" -name '*rvtt_lp_schedule*' -type f -print -quit)
    if [[ -z "$live_across_dump" ]] ||
       ! grep -Eq 'ops=2.*live-in=4.*peak=4' "$live_across_dump"; then
        record_failure "$architecture oracle omitted an untouched live-across value"
    fi

    for repetition in 1 2 3; do
        compile "$arch_output/repeat-$repetition" "$cpu" "$rescue" rescue.S \
            -mtt-tensix-optimize-lp-schedule \
            >"$arch_output-repeat-$repetition.log" 2>&1 ||
            record_failure "$architecture deterministic repetition $repetition failed"
    done
    if [[ -f "$arch_output/repeat-1/rescue.S" ]]; then
        cmp "$arch_output/repeat-1/rescue.S" "$arch_output/repeat-2/rescue.S" ||
            record_failure "$architecture repetition 1/2 assembly differs"
        cmp "$arch_output/repeat-1/rescue.S" "$arch_output/repeat-3/rescue.S" ||
            record_failure "$architecture repetition 1/3 assembly differs"
    fi
done

if command -v sha256sum >/dev/null 2>&1; then
    find "$output" -name '*.S' -type f -print0 | sort -z | xargs -0 sha256sum >"$output/assembly.sha256"
else
    find "$output" -name '*.S' -type f -print0 | sort -z | xargs -0 shasum -a 256 >"$output/assembly.sha256"
fi

if [[ $failures -ne 0 ]]; then
    echo "$failures focused validation failure(s); see $output" >&2
    exit 1
fi

echo "PASS: Wormhole and Blackhole focused scheduler validation"
echo "artefacts: $output"
