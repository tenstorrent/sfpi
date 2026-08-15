#!/usr/bin/env bash
# SPDX-FileCopyrightText: © 2026 Tenstorrent AI ULC
# SPDX-License-Identifier: Apache-2.0

# F1.0 corpus scorer.  This is deliberately a measurement harness, not an
# instruction-count proxy: each corpus entry is compiled from its LLK source,
# assembled/disassembled, checked in CRAQ, and profiled on a physical device.
# The profiler's WELFORD_BODY rows are copied immediately from the LLK shared
# report directory, because the next pytest process overwrites that report.

set -euo pipefail

ROOT=$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)
CORPUS="$ROOT/scripts/corpus/welford-bh.tsv"
MODE=all
RUNS=3
OUT=""
ORIGINAL_ARGS=("$@")
TOOLCHAIN=${SFPI_TOOLCHAIN:-}
LLK_TESTS=${TT_LLK_TESTS:-}
PYTEST=${TT_PYTEST:-/localdev/nkapre/tt-metal/python_env/bin/pytest}
CRAQ_SIMULATOR=${CRAQ_SIMULATOR:-/localdev/nkapre/sfpi-welford-full-trace/craq-bh-debug/libttsim_bh.so}
CRAQ_SOURCE=${CRAQ_SOURCE:-/localdev/nkapre/craq-sim}
PYTHONPATH_VALUE=${SFPI_PYTHONPATH:-/localdev/nkapre/sfpi-gcc-lreg-artifacts}

usage() {
  cat <<'EOF'
Usage: scripts/run-corpus-score.sh --toolchain PATH --llk-tests PATH [options]

Compile each corpus source, archive its final linked math ELF and paired
objdump, run the simulator leg, then collect fresh device-profiler processes.

Required:
  --toolchain PATH       SFPI toolchain root containing compiler/bin
  --llk-tests PATH       isolated TT-Metal tt-llk/tests directory

Options:
  --corpus FILE          tab-separated corpus manifest (default: Welford BH)
  --out DIR              artifact directory (default: validation/corpus-score-UTC)
  --runs N               fresh profiler processes per selector (default: 3)
  --compile-only         compile/assemble only
  --sim-only             compile/assemble and CRAQ only
  --device-only          compile/assemble and device only
  --craq-simulator PATH  Blackhole libttsim.so for the CRAQ leg
  --craq-source PATH     CRAQ source worktree used to build that library
  --pytest PATH          pytest launcher
  --help

The LLK test directory is rewired only for the duration of this command and
must be an isolated worktree.  No static instruction count is treated as a
performance result.
EOF
}

die() { echo "run-corpus-score: $*" >&2; exit 2; }

while (($#)); do
  case "$1" in
    --toolchain) TOOLCHAIN=$2; shift 2 ;;
    --llk-tests) LLK_TESTS=$2; shift 2 ;;
    --corpus) CORPUS=$2; shift 2 ;;
    --out) OUT=$2; shift 2 ;;
    --runs) RUNS=$2; shift 2 ;;
    --compile-only) MODE=compile; shift ;;
    --sim-only) MODE=sim; shift ;;
    --device-only) MODE=device; shift ;;
    --craq-simulator) CRAQ_SIMULATOR=$2; shift 2 ;;
    --craq-source) CRAQ_SOURCE=$2; shift 2 ;;
    --pytest) PYTEST=$2; shift 2 ;;
    --help) usage; exit 0 ;;
    *) die "unknown option: $1" ;;
  esac
done

[[ -n "$TOOLCHAIN" ]] || die "--toolchain is required"
[[ -n "$LLK_TESTS" ]] || die "--llk-tests is required"
[[ -x "$PYTEST" ]] || die "pytest launcher is not executable: $PYTEST"
[[ -d "$LLK_TESTS" ]] || die "LLK test directory does not exist: $LLK_TESTS"
[[ -f "$CORPUS" ]] || die "corpus manifest does not exist: $CORPUS"
[[ -x "$TOOLCHAIN/compiler/bin/riscv-tt-elf-g++" ]] || die "missing toolchain compiler"
[[ "$RUNS" =~ ^[1-9][0-9]*$ ]] || die "--runs must be a positive integer"

if [[ -z "$OUT" ]]; then
  OUT="$ROOT/validation/corpus-score-$(date -u +%Y%m%dT%H%M%SZ)"
fi
mkdir -p "$OUT"
OUT=$(cd "$OUT" && pwd)
printf '%q ' "$0" "${ORIGINAL_ARGS[@]}" > "$OUT/command.sh"
printf '\n' >> "$OUT/command.sh"

ORIGINAL_SFPI=""
restore_toolchain() {
  rm -f "$LLK_TESTS/sfpi"
  if [[ -n "$ORIGINAL_SFPI" ]]; then
    ln -s "$ORIGINAL_SFPI" "$LLK_TESTS/sfpi"
  fi
}
trap restore_toolchain EXIT

if [[ -e "$LLK_TESTS/sfpi" ]]; then
  [[ -L "$LLK_TESTS/sfpi" ]] || die "$LLK_TESTS/sfpi is not a symlink"
  ORIGINAL_SFPI=$(readlink "$LLK_TESTS/sfpi")
  rm "$LLK_TESTS/sfpi"
fi
ln -s "$TOOLCHAIN" "$LLK_TESTS/sfpi"

COMPILER="$TOOLCHAIN/compiler/bin/riscv-tt-elf-g++"
OBJDUMP="$TOOLCHAIN/compiler/bin/riscv-tt-elf-objdump"
if [[ ! -x "$OBJDUMP" ]]; then
  die "toolchain lacks paired riscv-tt-elf-objdump: $OBJDUMP"
fi

{
  echo "sfpi_superproject=$(git -C "$ROOT" rev-parse HEAD)"
  echo "sfpi_gcc=$(git -C "$ROOT/gcc" rev-parse HEAD)"
  echo "compiler=$(readlink -f "$COMPILER")"
  sha256sum "$COMPILER"
  cc1plus=$(find "$TOOLCHAIN/compiler" -type f -name cc1plus -print -quit)
  [[ -n "$cc1plus" ]] && sha256sum "$cc1plus"
  echo "llk_tests=$(readlink -f "$LLK_TESTS")"
  git -C "$(cd "$LLK_TESTS/../../.." && pwd)" rev-parse HEAD 2>/dev/null || true
  echo "craq_library=$(readlink -f "$CRAQ_SIMULATOR")"
  [[ -f "$CRAQ_SIMULATOR" ]] && sha256sum "$CRAQ_SIMULATOR"
  if git -C "$CRAQ_SOURCE" rev-parse --is-inside-work-tree >/dev/null 2>&1; then
    echo "craq_source=$(readlink -f "$CRAQ_SOURCE")"
    echo "craq_git=$(git -C "$CRAQ_SOURCE" rev-parse HEAD)"
    echo "craq_dirty=$(git -C "$CRAQ_SOURCE" status --porcelain | wc -l)"
    git -C "$CRAQ_SOURCE" diff --binary | sha256sum | sed 's/^/craq_patch_sha256=/'
  fi
  echo "mode=$MODE"
  echo "runs=$RUNS"
} > "$OUT/manifest.txt"

pytest_base=("$PYTEST" -q -s -o addopts='' -p pytest_workerid_plugin --timeout=600)
run_pytest() {
  local run_dir=$1
  shift
  mkdir -p "$run_dir/temp"
  (
    cd "$LLK_TESTS"
    PYTHONPATH="$PYTHONPATH_VALUE" CHIP_ARCH=blackhole \
      TT_METAL_DISABLE_SFPLOADMACRO=1 RUNNER_TEMP="$run_dir/temp" \
      "${pytest_base[@]}" "$@"
  ) > "$run_dir/run.log" 2>&1
}

archive_math_elfs() {
  local from=$1 dest=$2
  mkdir -p "$dest"
  local found=0 elf hash base
  while IFS= read -r -d '' elf; do
    found=1
    hash=$(sha256sum "$elf" | awk '{print $1}')
    base=$(basename "$(dirname "$elf")")
    cp "$elf" "$dest/${hash}.math.elf"
    "$OBJDUMP" -D "$elf" > "$dest/${hash}.math.objdump"
    printf '%s  %s\n' "$hash" "$elf" >> "$dest/math-elf.sha256"
    printf '%s\n' "$base" >> "$dest/source-keys.txt"
  done < <(find "$from" -name math.elf -print0)
  ((found)) || die "no final linked math.elf emitted below $from"
}

copy_profile_rows() {
  local profile_key=$1 dest=$2
  local shared="$LLK_TESTS/../perf_data/$profile_key"
  [[ -f "$shared/$profile_key.csv" ]] || die "shared raw profile CSV missing: $shared/$profile_key.csv"
  [[ -f "$shared/$profile_key.post.csv" ]] || die "shared post profile CSV missing: $shared/$profile_key.post.csv"
  cp "$shared/$profile_key.csv" "$dest/raw.csv"
  cp "$shared/$profile_key.post.csv" "$dest/post.csv"
  {
    stat -c '%n %s' "$dest/raw.csv" "$dest/post.csv"
    sha256sum "$dest/raw.csv" "$dest/post.csv"
    rg 'WELFORD_DEVICE_PROFILE' "$dest/run.log"
  } > "$dest/profile-manifest.txt"
}

echo -e 'kernel\tbaseline_cycles\tcandidate_cycles\tbaseline_runs\tcandidate_runs' > "$OUT/score.tsv"
while IFS=$'\t' read -r kernel arch correctness_node sim_node baseline_profile candidate_profile profile_key; do
  [[ -z "$kernel" || "$kernel" == \#* ]] && continue
  [[ "$arch" == blackhole ]] || die "only Blackhole corpus entries are supported in F1.0: $kernel"
  kernel_out="$OUT/$kernel"
  mkdir -p "$kernel_out"

  # 1. Compile the source before simulator/device execution.  The producer
  # mode does not contact silicon; it gives us reproducible linked ELF inputs.
  compile_out="$kernel_out/compile"
  run_pytest "$compile_out" --compile-producer "$sim_node"
  archive_math_elfs "$compile_out" "$compile_out/elfs"

  if [[ "$MODE" == compile ]]; then
    continue
  fi

  if [[ "$MODE" == all || "$MODE" == sim ]]; then
    [[ -f "$CRAQ_SIMULATOR" ]] || die "CRAQ simulator missing: $CRAQ_SIMULATOR"
    sim_out="$kernel_out/craq"
    mkdir -p "$sim_out/temp"
    (
      cd "$LLK_TESTS"
      TT_METAL_SIMULATOR="$CRAQ_SIMULATOR" PYTHONPATH="$PYTHONPATH_VALUE" \
        CHIP_ARCH=blackhole TT_METAL_DISABLE_SFPLOADMACRO=1 \
        RUNNER_TEMP="$sim_out/temp" \
        "${pytest_base[@]}" --run-simulator "$sim_node"
    ) > "$sim_out/run.log" 2>&1
    archive_math_elfs "$sim_out" "$sim_out/elfs"
    printf '%s\n' \
      'CRAQ is a functional/simulation gate; its counters are not device-cycle results.' \
      > "$sim_out/measurement-scope.txt"
  fi

  if [[ "$MODE" == sim ]]; then
    continue
  fi

  # Correctness executes every selector/N in the corpus source, not only the
  # profiled pair.  It is intentionally a physical-device process.
  correctness_out="$kernel_out/correctness"
  run_pytest "$correctness_out" "$correctness_node"
  archive_math_elfs "$correctness_out" "$correctness_out/elfs"

  baseline_runs=()
  candidate_runs=()
  for run in $(seq 1 "$RUNS"); do
    run_out="$kernel_out/profile/baseline-r$run"
    run_pytest "$run_out" "$baseline_profile"
    copy_profile_rows "$profile_key" "$run_out"
    archive_math_elfs "$run_out" "$run_out/elfs"
    baseline_runs+=("$(rg -o 'math_cycles=[0-9]+' "$run_out/run.log" | tail -1 | cut -d= -f2)")
  done
  for run in $(seq 1 "$RUNS"); do
    run_out="$kernel_out/profile/candidate-r$run"
    run_pytest "$run_out" "$candidate_profile"
    copy_profile_rows "$profile_key" "$run_out"
    archive_math_elfs "$run_out" "$run_out/elfs"
    candidate_runs+=("$(rg -o 'math_cycles=[0-9]+' "$run_out/run.log" | tail -1 | cut -d= -f2)")
  done

  baseline_min=$(printf '%s\n' "${baseline_runs[@]}" | sort -n | head -1)
  candidate_min=$(printf '%s\n' "${candidate_runs[@]}" | sort -n | head -1)
  printf '%s\t%s\t%s\t%s\t%s\n' "$kernel" "$baseline_min" "$candidate_min" \
    "$(IFS=,; echo "${baseline_runs[*]}")" "$(IFS=,; echo "${candidate_runs[*]}")" >> "$OUT/score.tsv"
done < "$CORPUS"

echo "wrote scorer artifacts to $OUT"
cat "$OUT/score.tsv"
