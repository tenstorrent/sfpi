#!/usr/bin/env bash

# Focused, deterministic validation for the opt-in SFPU pressure scheduler.

set -uo pipefail

if [[ $# -lt 1 || $# -gt 2 ]]; then
    echo "usage: $0 SFPI_INSTALL [OUTPUT_DIR]" >&2
    exit 2
fi

repo=$(cd "$(dirname "$0")/.." && pwd)
install=$(cd "$1" && pwd)
output=${2:-/tmp/sfpi-pressure-validation}
cxx="$install/compiler/bin/riscv-tt-elf-g++"
include="$install/include"
source_dir="$repo/gcc/gcc/testsuite/g++.target/riscv/tt/sfpi"
determinism_runs=${SCHEDULER_DETERMINISM_RUNS:-3}
require_milp=${SCHEDULER_REQUIRE_MILP:-0}
milp_args=()
if [[ "$require_milp" == 1 ]]; then
    milp_args=(-mtt-tensix-pressure-schedule-use-milp)
fi

if [[ ! -x "$cxx" || ! -d "$include" ]]; then
    echo "incomplete SFPI install: $install" >&2
    exit 2
fi
if ! [[ "$determinism_runs" =~ ^[1-9][0-9]*$ ]]; then
    echo "SCHEDULER_DETERMINISM_RUNS must be a positive integer" >&2
    exit 2
fi
if [[ "$require_milp" != 0 && "$require_milp" != 1 ]]; then
    echo "SCHEDULER_REQUIRE_MILP must be 0 or 1" >&2
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
    duplicate_use="$source_dir/lp-schedule-duplicate-use-wh.C"
    constant_lreg="$source_dir/lp-schedule-constant-lreg-wh.C"
    milp_cap="$source_dir/lp-schedule-milp-cap-wh.C"
    o0_gate="$source_dir/lp-schedule-o0-gate-wh.C"
    fused_dag="$source_dir/pressure-schedule-fused-dag-wh.C"
    milp_beats_list="$repo/scripts/lp-schedule-milp-beats-list.C"

    compile "$arch_output/minimal-off" "$cpu" "$minimal" minimal.S \
        >"$arch_output-minimal-off.log" 2>&1 ||
        record_failure "$architecture minimal fixture failed with pass off"
    compile "$arch_output/minimal-on" "$cpu" "$minimal" minimal.S \
        -mtt-tensix-optimize-pressure-schedule -fdump-tree-rvtt_lp_schedule \
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
        -mtt-tensix-optimize-pressure-schedule -fdump-tree-rvtt_lp_schedule \
        -fdump-rtl-rvtt_lp_alloc "${milp_args[@]}" \
        >"$arch_output-rescue-on.log" 2>&1 ||
        record_failure "$architecture scheduler did not rescue late-fold fixture"
    rescue_dump=$(find "$arch_output/rescue-on" -name '*rvtt_lp_schedule*' -type f -print -quit)
    if [[ -z "$rescue_dump" ]] ||
       ! grep -Eq 'old-peak=9.*new-peak=8.*validated=yes.*reason=ok.*applied=yes' "$rescue_dump"; then
        record_failure "$architecture rescue dump lacks the validated 9-to-8 certificate"
    fi
    if [[ -f "$arch_output/rescue-manual/rescue.S" && -f "$arch_output/rescue-on/rescue.S" ]]; then
        cmp "$arch_output/rescue-manual/rescue.S" "$arch_output/rescue-on/rescue.S" ||
            record_failure "$architecture automated rescue differs from the manual early-fold control"
    fi

    # This is a separate compiler-generated arithmetic DFG with no Welford
    # recurrence or source marker.  It proves the pass is generic rather than
    # recognizing the motivating algorithm.
    if compile "$arch_output/fused-dag-off" "$cpu" "$fused_dag" fused-dag.S \
        >"$arch_output-fused-dag-off.log" 2>&1; then
        record_failure "$architecture generic fused DAG unexpectedly compiled with pass off"
    elif ! grep -q "cannot store sfpu register (register spill)" \
        "$arch_output-fused-dag-off.log"; then
        record_failure "$architecture generic fused-DAG failure was not the expected SFPU spill"
    fi
    compile "$arch_output/fused-dag-list" "$cpu" "$fused_dag" fused-dag.S \
        -mtt-tensix-optimize-pressure-schedule -fdump-tree-rvtt_lp_schedule \
        >"$arch_output-fused-dag-list.log" 2>&1 ||
        record_failure "$architecture list scheduler did not rescue the generic fused DAG"
    fused_dump=$(find "$arch_output/fused-dag-list" -name '*rvtt_lp_schedule*' \
        -type f -print -quit)
    if [[ -z "$fused_dump" ]] ||
       ! grep -Eq 'old-peak=9.*new-peak=8.*validated=yes.*reason=ok.*rejection-selftest=passed.*applied=yes' \
        "$fused_dump"; then
        record_failure "$architecture generic fused DAG lacks a validated 9-to-8 certificate"
    fi
    if [[ "$require_milp" == 1 ]]; then
        compile "$arch_output/fused-dag-milp" "$cpu" "$fused_dag" fused-dag.S \
            -mtt-tensix-optimize-pressure-schedule \
            -mtt-tensix-pressure-schedule-use-milp \
            -fdump-tree-rvtt_lp_schedule \
            >"$arch_output-fused-dag-milp.log" 2>&1 ||
            record_failure "$architecture MILP did not rescue the generic fused DAG"
        if [[ -f "$arch_output/fused-dag-list/fused-dag.S" && \
              -f "$arch_output/fused-dag-milp/fused-dag.S" ]]; then
            cmp "$arch_output/fused-dag-list/fused-dag.S" \
                "$arch_output/fused-dag-milp/fused-dag.S" ||
                record_failure "$architecture generic fused-DAG list/MILP assembly differs"
        fi
    fi
    if [[ "$require_milp" == 1 ]] &&
       { [[ -z "$rescue_dump" ]] ||
         ! grep -Eq 'SFPU MILP: requested=yes available=yes status=optimal.*selected=yes' "$rescue_dump"; }; then
        record_failure "$architecture rescue did not use an optimal MILP certificate"
    fi
    if [[ "$require_milp" == 1 ]]; then
        compile "$arch_output/rescue-list" "$cpu" "$rescue" rescue.S \
            -mtt-tensix-optimize-pressure-schedule -fdump-tree-rvtt_lp_schedule \
            >"$arch_output-rescue-list.log" 2>&1 ||
            record_failure "$architecture list scheduler did not rescue the MILP fixture"
        list_dump=$(find "$arch_output/rescue-list" -name '*rvtt_lp_schedule*' -type f -print -quit)
        if [[ -z "$list_dump" ]] ||
           ! grep -Eq 'SFPU MILP: requested=no available=yes status=unavailable.*selected=no' "$list_dump"; then
            record_failure "$architecture list-only run unexpectedly entered the MILP backend"
        fi
        if [[ -f "$arch_output/rescue-list/rescue.S" && -f "$arch_output/rescue-on/rescue.S" ]]; then
            cmp "$arch_output/rescue-list/rescue.S" "$arch_output/rescue-on/rescue.S" ||
                record_failure "$architecture list/MILP rescue assembly differs"
        fi

        compile "$arch_output/milp-cap" "$cpu" "$milp_cap" milp-cap.S \
            -mtt-tensix-optimize-pressure-schedule \
            -mtt-tensix-pressure-schedule-use-milp \
            -fdump-tree-rvtt_lp_schedule \
            >"$arch_output-milp-cap.log" 2>&1 ||
            record_failure "$architecture capped MILP did not fall back to list scheduling"
        cap_dump=$(find "$arch_output/milp-cap" -name '*rvtt_lp_schedule*' -type f -print -quit)
        if [[ -z "$cap_dump" ]] ||
           ! grep -Eq 'SFPU MILP: requested=yes available=yes status=capped.*selected=no' "$cap_dump" ||
           ! grep -Eq 'old-peak=9.*new-peak=8.*validated=yes.*reason=ok.*applied=yes' "$cap_dump"; then
            record_failure "$architecture capped MILP fallback lacks a validated list certificate"
        fi

        # This deliberately stops at the current M2 boundary: the MILP finds
        # a schedule that the list heuristic misses, but GIMPLE cannot compel
        # IRA to realize its eight-coloring yet.  Capture both facts without
        # installing an expected compiler ICE in the DejaGNU suite.
        if compile "$arch_output/milp-beats-list" "$cpu" "$milp_beats_list" milp-beats-list.S \
            -mtt-tensix-optimize-pressure-schedule \
            -mtt-tensix-pressure-schedule-use-milp \
            -fdump-tree-rvtt_lp_schedule \
            >"$arch_output-milp-beats-list.log" 2>&1; then
            record_failure "$architecture MILP/list boundary unexpectedly compiled; update the M2 expectation"
        elif ! grep -q 'cannot store sfpu register (register spill)' "$arch_output-milp-beats-list.log"; then
            record_failure "$architecture MILP/list boundary did not reach the expected physical-allocation spill"
        fi
        beats_dump=$(find "$arch_output/milp-beats-list" -name '*rvtt_lp_schedule*' -type f -print -quit)
        if [[ -z "$beats_dump" ]] ||
           ! grep -Eq 'old-peak=11.*new-peak=8.*validated=yes.*reason=ok.*applied=yes' "$beats_dump" ||
           ! grep -Eq 'SFPU MILP: requested=yes available=yes status=optimal.*detail=ok.*selected=yes' "$beats_dump"; then
            record_failure "$architecture MILP/list boundary lacks the validated 11-to-8 solver certificate"
        fi
    fi
    rescue_rtl_dump=$(find "$arch_output/rescue-on" -name '*rvtt_lp_alloc*' -type f -print -quit)
    if [[ -z "$rescue_rtl_dump" ]] ||
       ! grep -Eq 'SFPU pre-IRA audit:.*capacity=8.*colorability=unchecked' "$rescue_rtl_dump"; then
        record_failure "$architecture rescue lacks the final pre-IRA reality audit"
    fi

    compile "$arch_output/predicated-off" "$cpu" "$predicated" predicated.S \
        >"$arch_output-predicated-off.log" 2>&1 ||
        record_failure "$architecture predicated fixture failed with pass off"
    compile "$arch_output/predicated-on" "$cpu" "$predicated" predicated.S \
        -mtt-tensix-optimize-pressure-schedule -fdump-tree-rvtt_lp_schedule \
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
        -mtt-tensix-optimize-pressure-schedule -fdump-tree-rvtt_lp_schedule \
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
        -mtt-tensix-optimize-pressure-schedule -fdump-tree-rvtt_lp_schedule \
        >"$arch_output-live-across.log" 2>&1 ||
        record_failure "$architecture live-across fixture failed"
    live_across_dump=$(find "$arch_output/live-across" -name '*rvtt_lp_schedule*' -type f -print -quit)
    if [[ -z "$live_across_dump" ]] ||
       ! grep -Eq 'ops=2.*live-in=4.*peak=4' "$live_across_dump"; then
        record_failure "$architecture oracle omitted an untouched live-across value"
    fi

    compile "$arch_output/duplicate-use" "$cpu" "$duplicate_use" duplicate-use.S \
        -mtt-tensix-optimize-pressure-schedule -fdump-tree-rvtt_lp_schedule \
        >"$arch_output-duplicate-use.log" 2>&1 ||
        record_failure "$architecture duplicate-use fixture failed"
    duplicate_dump=$(find "$arch_output/duplicate-use" -name '*rvtt_lp_schedule*' -type f -print -quit)
    if [[ -z "$duplicate_dump" ]] ||
       ! grep -Eq 'old-peak=9.*new-peak=8.*validated=yes.*reason=ok.*applied=yes' "$duplicate_dump"; then
        record_failure "$architecture duplicate-use fixture lacks an independent validation certificate"
    fi

    compile "$arch_output/constant-lreg" "$cpu" "$constant_lreg" constant-lreg.S \
        -mtt-tensix-optimize-pressure-schedule -fdump-tree-rvtt_lp_schedule \
        >"$arch_output-constant-lreg.log" 2>&1 ||
        record_failure "$architecture constant-LREG source-availability fixture failed"
    constant_dump=$(find "$arch_output/constant-lreg" -name '*rvtt_lp_schedule*' -type f -print -quit)
    if [[ -z "$constant_dump" ]] ||
       ! grep -Eq 'old-peak=9.*new-peak=8.*validated=yes.*reason=ok.*applied=yes' "$constant_dump"; then
        record_failure "$architecture constant-LREG fixture lacks a validation certificate"
    fi

    compile "$arch_output/o0-off" "$cpu" "$o0_gate" o0.S -O0 \
        >"$arch_output-o0-off.log" 2>&1 ||
        record_failure "$architecture O0 fixture failed with pass off"
    compile "$arch_output/o0-on" "$cpu" "$o0_gate" o0.S -O0 \
        -mtt-tensix-optimize-pressure-schedule -fdump-tree-rvtt_lp_schedule \
        >"$arch_output-o0-on.log" 2>&1 ||
        record_failure "$architecture O0 fixture failed with pass on"
    if find "$arch_output/o0-on" -name '*rvtt_lp_schedule*' -type f -print -quit |
       grep -q .; then
        record_failure "$architecture O0 gate unexpectedly ran the scheduler"
    fi
    if [[ -f "$arch_output/o0-off/o0.S" && -f "$arch_output/o0-on/o0.S" ]]; then
        cmp "$arch_output/o0-off/o0.S" "$arch_output/o0-on/o0.S" ||
            record_failure "$architecture O0 gate changed assembly"
    fi

    compile "$arch_output/debug" "$cpu" "$live_across" debug-off.S -g -gno-record-gcc-switches \
        >"$arch_output-debug-off.log" 2>&1 ||
        record_failure "$architecture debug fixture failed with pass off"
    compile "$arch_output/debug" "$cpu" "$live_across" debug-on.S -g -gno-record-gcc-switches \
        -mtt-tensix-optimize-pressure-schedule -fdump-tree-rvtt_lp_schedule \
        >"$arch_output-debug-on.log" 2>&1 ||
        record_failure "$architecture debug fixture failed with pass on"
    if find "$arch_output/debug" -name '*rvtt_lp_schedule*' -type f -print -quit |
       grep -q .; then
        record_failure "$architecture debug gate unexpectedly ran the scheduler"
    fi
    if [[ -f "$arch_output/debug/debug-off.S" && -f "$arch_output/debug/debug-on.S" ]]; then
        cmp "$arch_output/debug/debug-off.S" "$arch_output/debug/debug-on.S" ||
            record_failure "$architecture debug gate changed assembly"
    fi

    for repetition in $(seq 1 "$determinism_runs"); do
        compile "$arch_output/repeat-$repetition" "$cpu" "$rescue" rescue.S \
            -mtt-tensix-optimize-pressure-schedule "${milp_args[@]}" \
            >"$arch_output-repeat-$repetition.log" 2>&1 ||
            record_failure "$architecture deterministic repetition $repetition failed"
    done
    if [[ -f "$arch_output/repeat-1/rescue.S" ]]; then
        for repetition in $(seq 2 "$determinism_runs"); do
            cmp "$arch_output/repeat-1/rescue.S" "$arch_output/repeat-$repetition/rescue.S" ||
                record_failure "$architecture serial repetition 1/$repetition assembly differs"
        done
    fi

    parallel_pids=()
    for repetition in $(seq 1 "$determinism_runs"); do
        compile "$arch_output/parallel-$repetition" "$cpu" "$rescue" rescue.S \
            -mtt-tensix-optimize-pressure-schedule "${milp_args[@]}" \
            >"$arch_output-parallel-$repetition.log" 2>&1 &
        parallel_pids+=("$!")
    done
    for repetition in $(seq 1 "$determinism_runs"); do
        if ! wait "${parallel_pids[$((repetition - 1))]}"; then
            record_failure "$architecture parallel repetition $repetition failed"
        elif [[ -f "$arch_output/repeat-1/rescue.S" ]]; then
            cmp "$arch_output/repeat-1/rescue.S" "$arch_output/parallel-$repetition/rescue.S" ||
                record_failure "$architecture serial/parallel repetition $repetition differs"
        fi
    done
done

# QSR32 remains an explicit no-op until it has the same positive and negative
# test coverage as Wormhole and Blackhole.
qsr_output="$output/qsr32"
qsr_source="$source_dir/lp-schedule-live-across-wh.C"
mkdir -p "$qsr_output"
compile "$qsr_output/off" tt-qsr32-tensix "$qsr_source" qsr.S \
    >"$qsr_output-off.log" 2>&1 || record_failure "QSR32 gate fixture failed with pass off"
compile "$qsr_output/on" tt-qsr32-tensix "$qsr_source" qsr.S \
    -mtt-tensix-optimize-pressure-schedule -fdump-tree-rvtt_lp_schedule \
    >"$qsr_output-on.log" 2>&1 || record_failure "QSR32 gate fixture failed with pass on"
if find "$qsr_output/on" -name '*rvtt_lp_schedule*' -type f -print -quit |
   grep -q .; then
    record_failure "QSR32 gate unexpectedly ran the scheduler"
fi
if [[ -f "$qsr_output/off/qsr.S" && -f "$qsr_output/on/qsr.S" ]]; then
    cmp "$qsr_output/off/qsr.S" "$qsr_output/on/qsr.S" ||
        record_failure "QSR32 architecture gate changed assembly"
fi

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
