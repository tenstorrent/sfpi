# Welford Silicon Validation Mission

This is the copy-paste handoff for an agent with real Wormhole and/or
Blackhole hardware. The objective is to decide whether ordinary vFloat C++,
compiled through the generic SFPU scheduler, can replace or complement the
handwritten Welford LLK without correctness or performance regression.

The pressure scheduler is currently an opt-in research pass. Passing this
mission does not make it default-on; it supplies the missing silicon evidence
for that decision.

## Non-negotiable rules

- Test the same row recurrence behind every implementation selector. Share
  loads, transpose, reciprocal lookup, state initialization, stores, and
  variance finalization so their costs are not misattributed to scheduling.
- Preserve the handwritten LLK as the performance golden and the sequential
  FP32 implementation as a numerical reference.
- Report device cycles, not pytest wall time or simulator elapsed time.
- Treat Wormhole and Blackhole as separate targets. Do not copy latency or NOP
  assumptions between them.
- Keep replay-direct and non-replay-direct measurements separate.
- Do not modify or commit the unrelated dirty
  `gcc/testsuite/g++.target/riscv/tt/sfpi/dataformat-bh.C` deletion.
- Do not claim success from compilation alone. Archive final assembly and ELF
  hashes for every measured binary.

## Acquire and pin the compiler

```bash
git clone --recursive \
  --branch nkapre/sfpi \
  git@github.com:tenstorrent/sfpi.git sfpi-scheduler
cd sfpi-scheduler
git submodule update --init --recursive

git rev-parse HEAD
git -C gcc rev-parse HEAD
git status --short
git -C gcc status --short
```

The matching GCC commit is published on `tenstorrent/sfpi-gcc` branch
`nkapre/sfpi`. The superproject pins
the exact required commit, so do not advance the submodule casually.

For the solver-enabled research build on Ubuntu:

```bash
sudo apt-get update
sudo apt-get install -y \
  build-essential flex bison texinfo dejagnu expect \
  libgmp-dev libmpfr-dev libmpc-dev zlib1g-dev \
  liblpsolve55-dev libsuitesparse-dev

SFPI_WITH_LP_SOLVE=yes \
  ./scripts/build.sh --dir="$PWD/../sfpi-silicon-build" --checking

./scripts/validate-sfpu-pressure-scheduler.sh \
  "$PWD/../sfpi-silicon-build/sfpi" \
  "$PWD/../sfpi-silicon-compiler-validation"

SFPI_WITH_LP_SOLVE=yes \
  ./scripts/build.sh --dir="$PWD/../sfpi-silicon-build" --test-tt
```

Stop if the focused validator or TT target suite has an unexpected failure,
error, unresolved test, zero executed tests, or nondeterministic assembly.

## Pin the machine and TT-Metal checkout

Use a clean TT-Metal worktree. Record all of the following in the result
bundle before testing:

```bash
git -C "$TT_METAL_HOME" rev-parse HEAD
git -C "$TT_METAL_HOME" status --short
uname -a
nvidia-smi 2>/dev/null || true
tt-smi --version 2>/dev/null || true
tt-smi 2>/dev/null || true
```

Also record board architecture and stepping, card serial, firmware versions,
device clock configuration, host CPU, kernel, compiler hashes, build type, and
all relevant environment variables. Disable unrelated adaptive clock or power
changes if the lab procedure permits it; otherwise record them.

Point only the validation worktree at the custom SFPI installation. Confirm
from the JIT log and compiler `--version` output that it is actually used.
Delete the validation JIT cache when switching compiler commit, scheduler
mode, architecture, or implementation selector. A stale kernel invalidates
the measurement.

### Use the custom compiler in a validation worktree

TT-Metal's production JIT checks `${TT_METAL_HOME}/runtime/sfpi` before
`/opt/tenstorrent/sfpi`. The standalone TT-LLK test harness separately checks
`tt_metal/tt-llk/tests/sfpi`. Point both at the same built installation in the
clean validation worktree:

```bash
export TT_METAL_HOME=/path/to/clean/tt-metal-validation
export CUSTOM_SFPI=/path/to/sfpi-silicon-build/sfpi

test -x "$CUSTOM_SFPI/compiler/bin/riscv-tt-elf-g++"
test -d "$CUSTOM_SFPI/include"

mkdir -p "$TT_METAL_HOME/runtime"
ln -sfn "$CUSTOM_SFPI" "$TT_METAL_HOME/runtime/sfpi"

LLK_SFPI="$TT_METAL_HOME/tt_metal/tt-llk/tests/sfpi"
if [[ -e "$LLK_SFPI" && ! -L "$LLK_SFPI" ]]; then
  mv "$LLK_SFPI" "${LLK_SFPI}.released-backup"
fi
ln -sfn "$CUSTOM_SFPI" "$LLK_SFPI"

readlink -f "$TT_METAL_HOME/runtime/sfpi"
readlink -f "$LLK_SFPI"
"$TT_METAL_HOME/runtime/sfpi/compiler/bin/riscv-tt-elf-g++" --version
```

The JIT hashes the selected compiler's version string into its build key in
ordinary mode. Archive that first version line and verify it changes from the
released compiler. Do not use build-map mode for performance runs because that
mode deliberately omits the compiler version from the cache key.

### Permanent TT-Metal pin for CI

The symlink override is appropriate for an isolated silicon experiment. A
reproducible TT-Metal branch or CI job needs an immutable SFPI package pin:

1. tag the exact `nkapre/sfpi` superproject commit with a
   slash-free validation version;
2. build x86_64 Debian SFPI `.txz`/`.deb` artifacts with `lp_solve` packaged or
   an explicit solver-absent configuration;
3. publish those artifacts at an immutable release URL;
4. create a dedicated TT-Metal branch such as
   `nkapre/sfpi`;
5. update both `tt_metal/sfpi-version` and
   `tt_metal/tt-llk/tests/sfpi-version` to the upstream repository, version,
   build identifier, and exact SHA-256 hashes; and
6. prove `setup_testing_env.sh`, Docker installation, CMake installation, and
   production JIT all resolve the same compiler version.

Do not put the Git branch name directly into today's `sfpi-version` fields.
The download and packaging machinery expects release filenames and hashes, and
a branch containing `/` is not a valid substitute for an immutable package.
Until the private artifacts exist, keep the TT-Metal pin change out of product
branches and use the explicit validation-worktree override above.

## Implement one controlled test driver

Add these files on a dedicated TT-Metal validation branch if they do not yet
exist:

```text
tt_metal/tt-llk/tests/python_tests/test_sfpu_welford.py
tt_metal/tt-llk/tests/sources/sfpu_welford_test.cpp
tt_metal/tt-llk/tests/python_tests/perf_sfpu_welford.py
tt_metal/tt-llk/tests/sources/sfpu_welford_perf.cpp
```

Compile-time selectors must provide at least:

```text
HANDWRITTEN_DIRECT
HANDWRITTEN_REPLAY
VFLOAT_DIRECT
VFLOAT_RESCUE
VFLOAT_MANUAL_EARLY_FOLD
```

Add `VFLOAT_REPLAY` only when generated row bodies are stable and the final
assembly proves that automatic replay was formed. The ordinary vFloat variants
must contain the source recurrence, not copied TTI instructions:

```cpp
vFloat delta = x - mean;
mean += delta * reciprocal;
vFloat delta2 = x - mean;
m2 += delta * delta2;
```

The perf binary must accept implementation, logical width, tile count, warmup
count, repetition count, accumulation mode, and reciprocal mode. Emit one
machine-readable result row per repetition.

## Correctness gate

Run every implementation on both supported architectures with:

- logical counts 1, 4, 31, 32, 33, 64, and 96;
- BF16 and FP32 accumulation;
- reciprocal-LUT and ordinary reciprocal modes;
- constant, zero, monotonic, alternating-magnitude, fixed-seed random, and
  high-offset near-constant inputs;
- NaN, positive/negative infinity, and signed zero;
- poisoned inactive rows and partial-tile padding;
- multi-tile state carry; and
- repeated invocations in one process to expose retained architectural state.

Compare raw mean and raw M2 before comparing final variance. Use all three
references:

1. handwritten implementation differential;
2. sequential FP32 Welford emulation; and
3. Float64 mean and population variance.

Report bitwise mismatches, max/p50/p95/p99 ULP distance, max absolute and
relative error, classification/sign mismatches, and unexpected negative or
nonfinite M2. PCC alone is not sufficient.

Any intermittent mismatch, poisoned-lane leak, architecture-state leak, or
unexpected NaN/Inf/sign behavior is an immediate stop. Save the smallest
reproducer and its kernel binaries.

## Static assembly gate

For every selector and architecture, archive the compiler command, ELF,
disassembly, scheduler dumps, and SHA-256 hashes. Tabulate per row/block/tile:

- physical LREG assignment and peak occupancy;
- SFPMAD, SFPADD, SFPMUL, SFPMOV, SFPNOP, load, and store counts;
- replay capture/playback count and buffer footprint;
- static text size;
- any `BADLOAD`, `BADSTORE`, spill/fill attempt, or compiler diagnostic; and
- whether list and MILP modes select the same final schedule.

Reject any result that merely compiles by introducing an unaccounted Dst
round-trip or changes floating-point association without an explicit numerical
policy.

## Performance protocol

Instrument only the repeated compute region with the TT-Metal device profiler,
for example `DeviceZoneScopedN("WELFORD-MATH")`. Keep profiler markers outside
the inner row recurrence unless their fixed overhead is measured and removed.

Run with the device profiler enabled:

```bash
cd "$TT_METAL_HOME"
export TT_METAL_DEVICE_PROFILER=1

# Substitute the actual built binary and its arguments.
./build/test/tt_metal/tt_llk_welford_perf \
  --implementation HANDWRITTEN_REPLAY \
  --logical-width 32 --tiles 256 --warmup 20 --repetitions 100

python tools/tracy/process_ops_logs.py
```

Use at least 20 warmups and 100 measured repetitions. Randomize or rotate the
implementation order within paired runs so thermal drift does not always favor
one variant. Repeat the full experiment in at least five fresh processes.

Measure direct recurrence and production replay throughput separately. Report
raw device cycles and normalized cycles per row, block, tile, and input element.
Also report median, p5, p95, median absolute deviation, and paired percentage
difference versus `HANDWRITTEN_REPLAY`. Capture scoreboard-stall counters when
the platform exposes them. Do not substitute host enqueue time.

The mandatory first matrix is:

| Architecture | Scheduler | Implementation |
|---|---|---|
| Wormhole | off, list, MILP | handwritten direct/replay and every vFloat selector |
| Blackhole | off, list, MILP | handwritten direct/replay and every vFloat selector |

The scheduler-off build may fail for the historical high-pressure vFloat
fixture; record that as a compilation result rather than silently dropping the
row. Unsupported/rejected low-pressure regions must remain assembly-identical
off/on.

## Acceptance and abort rules

The generated vFloat path is a candidate to replace handwritten Welford only
when all of the following hold independently on Wormhole and Blackhole:

- every correctness case passes with no intermittent failure;
- final physical LREG use is at most eight with no spill/fill attempt;
- scheduler dumps and final assembly agree about selected destructive reuse;
- static instruction counts explain the measured performance ordering;
- median cycles match or beat handwritten replay, with the paired 95%
  confidence interval excluding a regression greater than 1%;
- code size and replay footprint do not regress by more than 2% without an
  explicit owner-approved tradeoff; and
- repeated serial and parallel compilations produce identical artifacts.

If generated code is correct but slower, retain the scheduler as an opt-in
compilation-feasibility feature and do not claim LLK replacement. If one
architecture wins and the other loses, preserve architecture-specific
selection. A Blackhole result does not waive Wormhole validation or vice versa.

## Result bundle and morning report

Store the following under a timestamped directory and commit the small text
report; upload large ELF, profiler, and trace artifacts separately with hashes:

```text
manifest.txt
commands.log
environment.txt
git-status.txt
compiler-version.txt
correctness.csv
cycles.csv
static-instructions.csv
assembly/
scheduler-dumps/
profiler/
failures/
REPORT.md
```

`REPORT.md` must lead with one of: `GO`, `GO-WH-ONLY`, `GO-BH-ONLY`, or
`NO-GO`. Include the best generated selector, median cycle delta versus
handwritten replay, numerical worst case, static instruction explanation,
open blockers, and exact reproduction command. Do not lead with build or
simulator success; those are prerequisites, not silicon conclusions.
