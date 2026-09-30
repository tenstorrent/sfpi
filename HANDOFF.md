# Handoff — 2026-09-29

## State at handoff

**Start here:** [setup and replay](#reproducing-a-recorded-run), then
[focused problem reproduction](#reproduce-and-diagnose-a-specific-problem).
All runnable campaign commands live here; archived paths are not local paths.

**Where the compiler work is.** The 177,787-line `nkapre/sfpi` branch is
partitioned into a **21-stage PR stack**: `nkapre/stack` in
`tenstorrent/craq-sfpi-gcc`, tip `8477d32d6ff`, on upstream base
`ba48edbef33`, cumulative diff 1,704 files / 166,106 insertions. Every stage
built standalone with `make all-gcc -j16` (21/21 rc=0, 0 compiler errors);
`rvtt.exp` was run at stage 21 only (6478/9 versus the source branch's 6478/8
in the same objdir, the one difference being `rv/zbkb.C`, which exists only
because the stack does not revert upstream `ba48edbef33`). Stage 22 of the
original partition was dropped deliberately: dead source plus a deletion that
would have reverted upstream. Per-stage commits, flags, coverage and
"not verified" lists are in `board/pr-packs/` in the workflow repository.
**One PR is open**: sfpi-gcc#22 (`nkapre/pr-lreg-livein`), mergeable, checks
green; the rest of the stack is not submitted.

**What is measured.** The current runtime measurement is the 2026-09-29
Blackhole sweep: 284 rows, **263 bounded-correctness PASS, 21 declared SKIP,
0 correctness failures**. Its counts are banded at ±0.5% and must always be
quoted with the band; see [verified results](#verified-results-and-limits).
263 is the *runnable* count — the committed `results.tsv` holds 259 PASS plus
the four outage rows that re-ran green, and over the 259 alone the same band
gives 188 / 69 / 2. Quote one denominator throughout: mixing a raw-sign count
from 164 rows with a banded count from 161 has already produced a wrong number.

Numerically, all three per-kernel legs have now run. Formal is reconstructed
and 8/8 VALIDATED; the exhaustive leg covered the **full 2^32** on a galaxy
(9 bit-exact, 5 divergent, 2 undecided); the stratified ULP leg reached 72 ops
and found **18 defective strata over 9 op rows, 15 of them with BOTH legs out
of contract** — `SEM-BUG` + `HAND-BUG` cells over the 369-cell corpus ledger,
graded at the **corrected** oracle `threeway_golden.py` (tt-metal `nkapre/sfpi`
`2107a950747`). That **supersedes "20 / 10 / 17"**, which was the same count
before six modelling defects were found in the golden itself; the `lgamma` S3/S7
"wrong sign" defects are withdrawn, the kernel was right
([re-grade](https://github.com/tenstorrent/craq-sfpi/blob/main/board/evidence/ulp-strata-regrade-20260930/NOTES.md)).
Quote the oracle revision with the count or it goes stale the same way. One of
the surviving defects was in production and is fixed (`f24d9515da4`). None of
this is an admission — see the boundaries column.

**What is not measured.** The sweep enabled **38 of the 92 options the stack
defines**; **53 appear in neither arm** and a 54th is pinned OFF in both.
**7 of 21 stages (03, 05, 12, 13, 15, 16, 18) have no runtime data at all.**
That is the *runtime* gap and it is still open. The *compile-time* gap is closed:
a 2026-09-29 compile-only census (263 rows x 27 knobs = 7,101 A/B verdicts,
`full_registry_coverage: true`) gives all 92 options a firing record —
**73 fire, 17 were measured and change no code on any of the 263 rows, 2 cannot
be A/B'd at all**. Cite the census for those 17; they are measured, not
unexamined. Firing means the option changes `.text`, never that it costs cycles.
`formal`, `exhaustive` and `ulp_admission` are NOT_RUN **in this sweep**; all
three have since been run separately — see
[verified results](#verified-results-and-limits). Four of the options outside the
profile (`-mtt-tensix-optimize-reassoc`,
`-mtt-tensix-optimize-reassoc-mad-restructure`,
`-mtt-tensix-optimize-stochrnd-store-fold`, `-mtt-tensix-optimize-store-sink`)
are `LICENSED_TARGET_FLAGS` in the workflow repository's
`scripts/select_llk_tuning.py`, which the ordinary per-op selector refuses and
routes to the separate ULP admission flow — the likely reason they are outside
the common profile.

**What is queued.** A measurement campaign to close the *runtime* half of that
knob-coverage gap is queued as a Slurm batch job: `board/evidence/incremental-knobs-20260929/` in
the workflow repository holds its plan, its group table and the job. It
measures each unmeasured option's **marginal** contribution on top of the
existing 39-token profile, not its standalone effect; results land under
`results/` there as each group completes. Read what is committed there rather
than assuming a scope for it from this document, and do not start a duplicate
run before checking. **Its raw run trees are deliberately not committed**:
`results/RAW-RUNS.md` records each group's node-local path and its
`CRAQ_GROUPS_ONLY=<group>` regeneration command instead. No checksum manifests,
no guard files, no provenance hashes.

Also queued, in priority order: a **post-fix 2^32 re-sweep** of `erf-fresh`,
`erfc-fresh` and `sigmoidlut-fresh`, whose current verdicts are a pre-fix
baseline; the two dead-slice ops `absint32` and `geluappx-fresh`, now that
`52fa68b6488` and `f7440593391` exist; the `hardshrink-fresh` discrepancy
between a full-space BIT-EXACT and a stratified SEM-BUG at S3; a `dest_acc=Yes`
row for the audit's `sigmoid(-89)` case; and the 16-bit band mode that would
unblock 75 of the 188 uncovered ULP rows, which is named and **not started**.

**Two owner decisions are outstanding and were deliberately not self-approved.**
Both come out of the `pow` fix (tt-metal `f24d9515da4`):

1. **Re-book six drifted anchors.** The 21f body gained two `v_if` guards, which
   costs a constant +40.00 (OFF) / +36.00 (ON) DIAGNOSTIC units on every row
   sharing it. Two booked cells cross the 10% `max_abs_drift_pct` tripwire —
   `unarypower:sem_off` and `unarypower-fresh:hand_off`, both **+10.409%** — and
   `rpow:hand_off` sits at +9.985%, 0.015 pp under. **They will read RED on the
   next weekly unless re-booked first**, and re-booking belongs at the
   conf-pinned cc1plus `b013967fffaa`, not at this lane's local `d1f90de7`.
2. **Add a reviewed `conf_lint` R7 exception.** R7 (LLK-pristine) needs a
   `_REVIEWED_LLK_API_EXCEPTIONS` entry in `sweep_2x2.conf` for the four pow
   headers. It holds one entry today (`ckernel_sfpu_quant.h`), and R7 was
   *already* RED on this branch for `ckernel_sfpu_sdpa_exp_unclamped.h`, so
   adding the four would not restore green on its own. An exception is reviewed
   like a pin; it is not self-added.

No hardware test from the previous round is running. Allocations `122075`
(sweep) and `122098` (fresh build) were released. Evidence is preserved in the
pinned workflow repository linked below; do not depend on a compute node or an
old terminal session remaining available.

## Reproducing a recorded run

This is the first-mile recipe, not merely an evidence-auditing recipe. Use a
Linux build host with GitHub SSH access, Git, a C/C++ toolchain, `wget`, `xz`,
Python 3.11+ with venv/pip, outbound HTTPS to ftp.gnu.org and github.com, and
about 20 GB of local disk. Setup builds its own bison and flex, so a host
without them is fine. Texinfo is not needed (setup passes `MAKEINFO=true`), and
neither are system GMP/MPFR/MPC (GCC builds its in-tree copies). Measuring
requires an allocated Blackhole device with its driver/runtime already
installed. Setup does not allocate a device or install its driver. Build and
run on the same node; node-local home directories are not shared.

Two host settings before any census or concurrent sweep:

```sh
ulimit -u 1024
export OPENBLAS_NUM_THREADS=1
```

Every pytest session imports numpy and OpenBLAS spawns one thread per core —
48 here — so N concurrent workers need 48N slots against a 512-process soft
cap, and the losers die with `posix_spawn: Operation not permitted`, recorded
misleadingly as `COMPILE_FAIL` (33 cells in one census trial). And compare
builds **within one tree**: `.text` embeds the checkout path on this target, so
two copies of one commit built with identical flags give `.text` of the same
size and different bytes — measured 9/9 identical inside one tree, 0/9 across
copies, with equal-length paths not helping and cycle counts unaffected. A
second path silently poisons `text_changed` and
`select_llk_tuning.py`'s frozen-baseline refusal.

### 1. Obtain the workflow

Use a new destination directory. No preexisting four-repository checkout, no
`source_bundle.py export` and no patch step are needed: the portability fixes
that used to live in `docs/reproduction/workflow-portability.patch` are now on
workflow `main`.

```sh
git clone git@github.com:tenstorrent/craq-sfpi.git craq-workflow
cd craq-workflow
git checkout --detach 5144c7e   # optional: the revision these steps were run on
```

Workflow `main` and this SFPI-source branch intentionally hold different trees;
you do not need an SFPI checkout to follow this recipe, because setup clones
SFPI itself.

### 2. Choose local storage, load pins, build and wire the harness

```sh
export WORK="$HOME/craq-repro-20260928"
source board/evidence/compiler-tuning-repairs-20260928/full-sweep/reproduce.env
bash scripts/setup.sh --work "$WORK"
export CRAQ_SETUP_STATE="$WORK/SETUP-STATE.env"
bash scripts/setup.sh --work "$WORK" --check
bash scripts/setup.sh --work "$WORK" --stage verify
```

The new `reproduce.env` exports only `SFPI_REF`, `METAL_REF`, `BLAZE_REF` and
`GCC_REF`: b375545 / b06bb841 / 50b341b / 566071b, respectively, as full SHAs.
The SFPI source docs commit is newer; b375545 is the measured source pin.
The generator no longer writes an absolute WORK. If using an **old** archived
environment file, override/export WORK **after** sourcing it; setting it before
sourcing does not protect it from that old assignment.

Setup clones the source repositories and GCC submodule, builds dependencies
and the toolchain, verifies it, creates the LLK Python environment, and wires:

```text
$WORK/tt-metal/tt_metal/tt-llk/tests/sfpi
    -> $WORK/sfpi/build/sfpi
```

That symlink, not the shell PATH, selects the headers and compiler used by the
LLK harness. Skipping the harness stage can select its stock SFPI release.
Check the actual path before any measurement:

```sh
TESTS="$WORK/tt-metal/tt_metal/tt-llk/tests"
test "$(readlink -f "$TESTS/sfpi")" = "$(readlink -f "$WORK/sfpi/build/sfpi")"
"$TESTS/sfpi/compiler/bin/riscv-tt-elf-g++" --version
"$TESTS/sfpi/compiler/bin/riscv-tt-elf-g++" -print-prog-name=cc1plus
```

The expected target assumes default `--build-dir build`. The published
incremental base is not downloadable today, so a clean host always does the full
binutils+gcc+newlib rebuild: measured at 12 minutes and 18 GB under `$WORK` on
an idle 16-core node. The generated setup
state belongs to this machine; do not copy an old machine's SETUP-STATE.env.
`corpus/sweep.sh` checks `CRAQ_SETUP_STATE`; the lightweight `sweep_llks.py`
records tool identity but does not use that variable as a state gate.

For an unpinned development setup only, omit sourcing the pin file and run
`bash scripts/setup.sh`: WORK defaults to `$HOME/craq-build` and source refs
default to moving `nkapre/sfpi` branches. That is not historical reproduction.

### 3. Smoke, then full manifest-profile sweep

From this same workflow checkout and allocated node:

```sh
python3 - "$TESTS" "$WORK/final-profile-smoke" exp <<'PY'
import json
from pathlib import Path
import subprocess
import sys
manifest = Path("board/evidence/compiler-tuning-repairs-20260928/full-sweep/manifest.json")
flags = json.loads(manifest.read_text())["flags"]
argv = [sys.executable, "scripts/sweep_llks.py", "--tests-root", sys.argv[1],
        "--out", sys.argv[2], "--repeats", "3",
        "--off-flags=" + flags["OFF_FLAGS"], "--on-flags=" + flags["ON_FLAGS"]]
if len(sys.argv) > 3:
    argv += ["--ops", sys.argv[3]]
subprocess.run(argv, check=True)
PY
```

For the full run, repeat that block with first line
`python3 - "$TESTS" "$WORK/final-profile-full" <<'PY'` (no `exp` argument).
Both output directories must be new. This uses the recorded record-hoist-OFF
profile, not corpus defaults. Then report the completed full run:

```sh
python3 scripts/report_llk_sweep.py \
  --current "$WORK/final-profile-full" --out "$WORK/final-profile-report.json"
```

The block above replays the **2026-09-28** manifest profile. To replay the
**2026-09-29** run instead, take `OFF_FLAGS` and `ON_FLAGS` from
`board/evidence/full-sweep-20260929/manifest.json` rather than retyping them;
that run also used a later tt-metal commit (`6acf1685f172d16d`), so the two are
not interchangeable.

Measured acceptance was 284 completed, 263 bounded PASS, 21 declared SKIP in
both runs. 2026-09-28 at ±1%: ON/hand 66/35/63, ON/OFF 189/74/0.
2026-09-29 at ±0.5%: ON/hand 71/25/68, ON/OFF 192/69/2 — always quote the
band. A rerun produces new evidence tied to its compiler and device, not a
guarantee of identical cycle counts or binary hashes. Formal, exhaustive and
ULP admission are separate.
Never execute the archived command.json's absolute node paths verbatim.

Copy new results off disposable nodes. Check setup logs, the resolved symlink,
manifest hashes and actual correctness results before claiming reproduction.
The workflow patch was validated with host tests and a relocated pin-file test;
this documentation update does not claim a new fresh-node hardware sweep.

## Reproduce and diagnose a specific problem

Run the setup above first. The following commands run from `craq-workflow`,
with WORK and TESTS set as above. Each output directory must be new. These
reproduce the test or comparison on the repaired compiler; they do not claim
to reproduce an old wrong-code witness merely by rerunning its row name.

### Header/compiler ABI failure or wrong compiler selected

```sh
readlink -f "$TESTS/sfpi"
CXX="$TESTS/sfpi/compiler/bin/riscv-tt-elf-g++"
"$CXX" -print-prog-name=cc1plus
"$CXX" -print-prog-name=as
sha256sum "$("$CXX" -print-prog-name=cc1plus)"
bash "$WORK/sfpi/scripts/validate-header-abi.sh" \
  "$WORK/sfpi/build/sfpi" "$WORK/header-abi-repro-new"
```

The probe instantiates dependent templates for WH/BH/QSR; it should pass on
the pinned pair. Too-few-builtin-arguments or target-specific overload failures
are compiler/header pairing failures, not reasons to loosen a numeric test.
If experimenting with a different `-B` prefix, add `-###` to the **exact failing
compile argv** and inspect its actual cc1plus path. A wrapper's earlier prefix
can override the compiler you intended to test. `--version` alone is insufficient.

### One LLK correctness/performance regression or tuning option

Choose one OP in the case statement. This freezes the final common profile
as OFF and changes only the named candidate option for ON:

```sh
export OP=geluappx-fresh
FROZEN_FLAGS=$(python3 - <<'PY'
import json
from pathlib import Path
p = Path("board/evidence/compiler-tuning-repairs-20260928/full-sweep/manifest.json")
print(json.loads(p.read_text())["flags"]["ON_FLAGS"])
PY
)
case "$OP" in
  geluappx-fresh)
    CANDIDATE_FLAGS="$FROZEN_FLAGS -mtt-tensix-optimize-crosscall-config-prefix" ;;
  mulint32-fresh)
    CANDIDATE_FLAGS="${FROZEN_FLAGS/-mtt-tensix-macro-planner /-mno-tt-tensix-macro-planner }" ;;
  absint32)
    CANDIDATE_FLAGS="$FROZEN_FLAGS -mtt-tensix-optimize-int-abs" ;;
  abs|tanhderivative-fitted|typecast|blaze-sdpareducerow-sum|blaze-sdpareducerow-sum-cl|blaze-sdpareducerow-sum-t8)
    CANDIDATE_FLAGS="${FROZEN_FLAGS/-mno-tt-tensix-optimize-replay-record-hoist/-mtt-tensix-optimize-replay-record-hoist}" ;;
  *) printf 'Choose an OP listed in this recipe\n' >&2; exit 2 ;;
esac
REPRO_OUT="$WORK/repro-$OP-new"
python3 scripts/sweep_llks.py --tests-root "$TESTS" --ops "$OP" \
  --repeats 3 --timeout 120 --out "$REPRO_OUT" \
  "--off-flags=$FROZEN_FLAGS" "--on-flags=$CANDIDATE_FLAGS"
python3 scripts/report_llk_sweep.py \
  --current "$REPRO_OUT" --out "$WORK/repro-$OP-report-new.json"
```

The final manifest already contains the LUT prerequisites for GELU. Retain
macro-planner child options for the multiply A/B; the disabled parent gates
them. Do not disable the SFPABS safety quarantine for these measurements.

Reference outcomes, not guaranteed cycle counts on another build/device:

| OP / option | Frozen → candidate cycles | What to inspect |
|---|---:|---|
| GELU / crosscall-config-prefix | 31033 → 28857 | Licensed source unchanged; hand parity, not a new ULP license |
| restricted multiply / macro-planner OFF | 38702 → 35117 | Operand domain below 2^23; unchanged output conversion |
| AbsInt32 / int-abs | 21815 → 17720 | Explicit safe SFPABS; old 16740 macro result not restored |
| float abs / record-hoist ON | 20538 → 20538 | Negative control: no recovered macro win |
| fitted tanh derivative / record-hoist ON | 578362 → 565331 | Must finish without timeout; still slower than hand |

For another failing row, repeat the full-profile Python block above with its
name in place of `exp`, rather than inventing new flags or changing tolerances.
For an old/new compiler comparison, prepare two separate roots from matching
source/header pins, inspect the real executable paths, and run the same row,
flags, seed and domain. Do not replace GCC inside an existing validated install.

### Re-check one kernel's ULP contract on silicon

The stratified leg from `tt_metal/tt-llk/tests/corpus/tools`. Produce the ELFs
with the **same** `LANEMK_TILE_DIM` the consumer will use, or the consumer looks
up a different source hash and refuses:

```sh
# producer, per op
cd "$TESTS/python_tests" && RUNNER_TEMP=$RT LANEMK_TILE_DIM=256,256 \
  TT_LLK_EXTRA_COMPILER_OPTIONS="$ON_FLAGS" CHIP_ARCH=blackhole SHORT_ARCH=bh \
  LLK_HOME=$LLK .venv/bin/python -m pytest -o addopts= -q \
  --compile-producer "$SEM" "$HAND"

# consumer + ULP fold, per op and per stratum.  Nine strata now; $S =
#   S0 0x00000000  +0.0 / subnormal     S5 0xC2B20000  -89.0
#   S1 0x3F700000  +0.9375              S6 0xFF700000  -3.1901e38
#   S2 0x41300000  +11.0                S7 0xC0A10000  -5.03125
#   S3 0xBF700000  -0.9375              S8 0x50150000  +9.99922e9
#   S4 0x7F780000  +3.2965e38
# S5-S8 were added because the inherited five held exactly one negative value,
# -0.9375, and no large negative at all.  run-ulp-strata.sh still defaults to
# S0-S4 only, so pass the rest explicitly.
python3 fp32_stream_sweep.py --op $OP --sem-node "$SEM" --hand-node "$HAND" \
  --farm "$TESTS/python_tests" --venv "$TESTS/.venv/bin/python" --llk-home $LLK \
  --runner-temp $RT --band-bits 16 --start-bit $S --total 65536 --chip 0 \
  --golden $OP --out $OUT/$OP-$name
```

One band is not a check: `[0, 2^18)` is entirely positive subnormals and zero,
and that is why the out-of-contract strata went unseen for so long. But the
band was only half the reason — the leg had also only ever run on 31 ops
because `threeway_golden.REGISTRY` held 31 entries, and `--golden <op>` is
inert without a spec. The registry now holds 72; adding an op means adding a
spec, not finding device time. The verdict will read
`BIT-EXACT-PARTIAL-65536-OF-2^32`; that is correct and must not be reported as
all-inputs. `selftest_threeway_golden.py` must run under
`tests/.venv/bin/python`, not system python.

For the full-space version of the same check, drive the galaxy shard
(`galaxy_shard.sh`, `galaxy_combine.py`) rather than looping bands by hand:
`SPACE`/`FULL_SPACE` and the divergence path only report correctly at
`7b8f32371c7` and `f7440593391` or later, and at `BAND_BITS=23` the rate is
~740k patterns/s/leg/chip rather than the ~65.5k a 2^18 band measures.

### Inspect a failure without destroying the evidence

```sh
python3 - "$REPRO_OUT" "$OP" <<'PY'
import json
from pathlib import Path
import sys
root = Path(sys.argv[1])
print((root / "summary.json").read_text())
row = root / "rows" / sys.argv[2]
if (row / "result.json").exists():
    print((row / "result.json").read_text())
for name in ("command.json", "status.json", "pytest.log", "pytest.xml"):
    for path in sorted(row.rglob(name)):
        print(path)
PY
```

Read the failed cell's command, status and pytest log together. Compile failure,
device timeout, assertion failure and a slower correct result are different
outcomes. A timeout stops the sweep: recover the device through the normal
cluster procedure, then rerun the pinned exp smoke before retrying the affected
row into a new directory. Do not continue timing on an unhealthy device, erase
the first failure, or count unrun rows as passes. The report requires a completed
run; use raw summary/row records for interrupted runs.

### Compiler tests and restricted-domain proof

```sh
python3 scripts/run_compiler_tests.py \
  --source "$WORK/sfpi/gcc" \
  --object-dir "$WORK/sfpi/build/build-gcc-newlib-stage2" \
  --sfpi-install "$WORK/sfpi/build/sfpi" \
  --out "$WORK/compiler-tests-repro-new" --timeout 3600
python3 -m venv "$WORK/limb2-proof-venv"
"$WORK/limb2-proof-venv/bin/python" -m pip install \
  -r "$TESTS/corpus/tools/requirements-formal.txt"
"$WORK/limb2-proof-venv/bin/python" \
  "$TESTS/corpus/selftest_mul_int32_limb2_contract.py"
```

The object path assumes the default full build. DejaGNU must be installed/on
PATH. Inspect the runner's result JSON, not merely make's exit code. For a
focused compiler regression, add for example
`--selector=rvtt.exp=macro-planner-derived-intmul-row-bh.C` with a new output
directory. The pinned full suite expectation is 8023 PASS / 2 XFAIL / zero
unexpected results. The Z3 script checks the restricted source model, not the
compiler's generated machine code. Full formal/Galaxy/ULP reproduction is
blocked as recorded under the remaining tasks; there is no honest substitute
command that turns the existing bounded sweep into those certificates.

Host-only checks, without allocating hardware. These need Python 3.11+: on
3.10 four source-bundle tests error on the missing `hashlib.file_digest`.

```sh
python3 -m unittest discover -s scripts -p 'test_*.py'
bash scripts/test_toolchain_provenance.sh
```

## Published source checkpoint

These are the source revisions validated in the completed round, not promises
that moving remote branches will remain there forever.

| Repository / branch | Commit |
|---|---|
| SFPI public and craq-sfpi source mirror, `nkapre/sfpi` | `b375545acf82d28029636b373ef35f61d23b5f23` |
| sfpi-gcc public and private mirrors, `nkapre/sfpi` | `566071bf728e07e56fba9a6eedcfb68a59213ed4` |
| tt-metal, `nkapre/sfpi` | `b06bb8410144eed620c95e8db2fe0c31802ae593` |
| tt-blaze, `nkapre/sfpi` (unchanged) | `50b341b95ea3a717e4e2a8501e021138c40b0237` |
| craq-sfpi workflow, `main`, completed implementation/evidence checkpoint | `15ec9e7e92a92258d12956a75a802a1b705fe7f3` |
| craq-sfpi workflow, `main`, revision these steps were last run end to end on | `5144c7e` |
| craq-sfpi-gcc, `nkapre/stack`, 21-stage PR stack tip | `8477d32d6ffcc53d7e9267d0d0428c37b1d08b65` |
| craq-sfpi-gcc, upstream base the stack is partitioned onto | `ba48edbef3341846177dd2852f00366055740df1` |
| tt-metal commit the 2026-09-29 sweep actually ran on | `6acf1685f172d16db110b412039f1645965a50b9` |

The 2026-09-29 sweep ran on tt-metal `6acf1685f172d16d`, **eight commits after
the pinned `b06bb841014`**, and its `manifest.json` records compiler binary
hashes but **no compiler source commit** — so that run cannot be tied to a GCC
revision from its own manifest. The `b06bb841014` row remains the validated
source pin. The table preserves the previously validated source checkpoint. Workflow `main` and the
SFPI-source `nkapre/sfpi` branch intentionally contain different trees. The
source mirror pair must agree with each other, not with workflow `main`.
SFPI pins binutils `7d192e0b6bb8e3bd3b7ec38d3316a59919051333` and newlib
`5e5e51f1dc56a99eb4648c28e00d73b6ea44a8b0`.

Local working repositories used in the round:

- `/Users/nkapre/workspace/craq-sfpi`: workflow and evidence.
- `/Users/nkapre/workspace/sfpi`: SFPI, with GCC under `gcc/`.
- `/Users/nkapre/workspace/tt-metal-sfpi-review`: reviewed tt-metal source.
- `/Users/nkapre/workspace/tt-blaze`: unchanged source.

Do not overwrite the separate user-owned `tt-metal` checkout or untracked
tt-blaze hydration scripts. Recheck worktree status before editing or pushing.

## Verified results and limits

Everything in the first block is from the **2026-09-29** sweep. Its
performance counts are banded at **±0.5%**; quoting one without the band is an
error, because the hand comparison changes verdict with the band.

| Check | Result | Boundary |
|---|---|---|
| Full Blackhole sweep, 2026-09-29 | 263 PASS / 21 SKIP / 0 correctness failures | Existing bounded pytest contracts, not all-input proof |
| Semantic ON vs OFF | 192 faster / 69 flat / 2 slower | 263 rows, ±0.5% band. Robust: 202/56/5 at raw sign, 188/74/1 at ±1% |
| Semantic ON vs hand ON | 71 beat / 25 tie / 68 hand wins | 164 rows with a hand arm, ±0.5% band. **Band-sensitive: 79/3/82 at raw sign, 66/35/63 at ±1%. A statistical tie, not a win.** |
| Four rows first recorded FAIL | Infrastructure outage, all four re-ran green | Status 3 = pytest `INTERNAL_ERROR` before collection; a numeric mismatch exits 1 |
| Option coverage in that sweep | 38 of 92 enabled | 53 options in neither arm, 1 pinned OFF in both; 7 of 21 stages have no runtime data |
| Compile-time option census, 2026-09-29 | 263 rows x 27 knobs = 7,101 A/B verdicts; all 92 options have a record — 73 fire, 17 measured and change no code, 2 unmeasurable | Compile-only, no device. A firing option has no cycle number; this does not reduce the runtime gap above |
| OFF arm construction | Valid, but not a literal negation | 16 of 38 enabled flags have no `-mno-` counterpart; valid only because all 16 are `Init(0)` |
| 21-stage PR stack build | 21 of 21 standalone `make all-gcc -j16` at rc=0, 0 errors | Build only; `rvtt.exp` run at stage 21 only, intermediate stages untested |
| `rvtt.exp` at stack tip | 6478 expected passes / 9 unexpected failures | Versus source `566071bf728` at 6478 / 8 in the same objdir; the one difference is `rv/zbkb.C`, an `all-gcc`-only objdir artefact |
| Upstream PR sfpi-gcc#22 | Open, mergeable, 683 added lines / 11 files, 4/4 Cycode green | Not merged, not reviewed; its builtin has no in-tree caller yet |
| Exhaustive bit-exactness, 2026-09-29 | Ran at 2^16 with silicon anchors: 32/32 rows, 16 BIT-EXACT-ALL-INPUTS, 13 DIVERGENT, 3 refused | 2^16 over 32 corpus rows, at cc1plus `d1f90de7`. Two refusals are device-anchor failures: craq-sim `1c47e9cd` is not bit-faithful to BH silicon on `mish-fitted` and `recip` |
| **Full 2^32 exhaustive, on a galaxy** | Ran on `bh-glx-120-b04u08`, 32 chips, `NPAR=32`, `BAND_BITS=23`: **9 ops `BIT-EXACT-ALL-INPUTS` at `covered=4294967296`, 5 `DIVERGENT`, 2 undecided** (dead slices). `rpow` bounded to `[0x70000000, 0x80000000)` | Coverage arithmetic checked at NPAR=4 and NPAR=32; 8 device-free selftests pass. **The earlier infeasibility claim was a bad measurement**: 65.5k patterns/s was a 2^18 band where per-band overhead dominates; at 2^23 it is ~740k/s/leg/chip, ~1.6 h/op on one chip and ~5-9 min observed on a galaxy. `erf-fresh`/`erfc-fresh`/`sigmoidlut-fresh` verdicts are a **pre-fix baseline** — `e75553f1c3f` changed those sem headers mid-campaign. **Do not report `hardshrink-fresh` as cleared**: the stratified leg called it SEM-BUG inside slice 23 the day before, unresolved. Snapshot taken with the job still running |
| Stratified ULP leg, 2026-09-29 | Ran on silicon, 31 ops x 5 exponent strata. **Six strata across four `fresh_cpp` kernels are outside the bf16 ULP contract where production is inside it** (`erf-fresh` S2/S4, `erfc-fresh` S2/S4, `hardshrink-fresh` S3, `sigmoidlut-fresh` S4), and `rpow` S4 is the opposite polarity | Open findings. 0.24% of 2^32 at five hand-picked exponents; every verdict is `BIT-EXACT-PARTIAL-65536-OF-2^32`. Probes refute; they do not certify. The `erf`/`erfc`/sigmoid-LUT sem side is fixed by `e75553f1c3f`; `rpow` by `f24d9515da4` |
| **Stratified ULP, corpus extension** (re-graded at the corrected oracle, `2107a950747`) | 41 more ops x 9 strata = **369 cells**: 302 CLEAN, 16 SEM-BUG, 11 OUT-OF-DOMAIN, 2 HAND-BUG, 2 OUT-OF-CLAIM, 36 NO-GOLDEN. **18 defective strata over 9 op rows; 26 ops clean across all 9 strata**. *Superseded:* 295/18/16/2/2/36 with 20 over 10 and 24 clean, at the pre-fix oracle | **15 of the 18 have BOTH legs out of contract** — 83% would read as agreement under an equivalence-only sweep. The `lgamma` S3/S7 rows are **withdrawn**: the node is `calculate_lgamma_stirling`, which returns the documented intermediate `lgamma((x<0.5)?1-x:x)`, and the golden graded a stage node against the composite's semantics; re-measured they are max ULP 2 and bit-exact. 7 of the 369 verdicts moved, all toward the kernel; **0 newly-exposed defects on this ledger** — that direction lands on the harness/exhaustive path, not here. Coverage **71 of 259 ops (27.4%)**; 173 of the 188 uncovered are harness-blocked, 15 need a two-operand stratification design. `erf`/`erfc` PRODUCTION are clean — that defect was `fresh_cpp`-only |
| **Static SFPU boundary audit** | **31 ranked defects** in three classes; 12 defect bodies over 13 of 21 `*_fitted.h`. Second overflow idiom `as<vFloat>((i<<23)+as<vInt>(w))` in 15 production + 7 `fresh_cpp` files, 5 invisible to a `setexp\|addexp` grep, and it spills into the **sign** bit | Static reading, not silicon. Its `sigmoid(-89)` NaN **did not reproduce** at `dest_acc=No` (the 16-bit DEST flushes as the golden does) — that needs a `dest_acc=Yes` row. Its "9 cleared" figure is not reproducible from its own TSVs; use the TSVs |
| **Production `pow` fix** | tt-metal `f24d9515da4`: one-sided clamp completed in both arches (5 files). Clamp **and** `+inf` substitution, verified as `0x7F800000`. `rpow` S4: 65535 ULP / 65536 out → **0 / 0, bit-exact with sem** | **Two owner actions outstanding** — six booked anchors drift +40.00 (OFF) / +36.00 (ON) DIAGNOSTIC units and two cross the 10% tripwire (`unarypower:sem_off`, `unarypower-fresh:hand_off`, both +10.409%; `rpow:hand_off` +9.985%, 0.015 pp under), needing re-booking at the conf-pinned cc1plus; and `conf_lint` R7 needs a reviewed `_REVIEWED_LLK_API_EXCEPTIONS` entry for the four pow headers |
| Formal equivalence | **Reconstructed and RUN: 8 of 8 rows VALIDATED, zero contradictions** (6 `PROVEN-EQUIV-ALL-INPUTS`, 2 `DIVERGENT`) | The instrument was in no commit — the pin was an uncommitted patch on craq-sim `1c47e9cd`, rewritten as `agent/laneJO-sfpu-trace-stream` tip `6de51ce0`. **Re-derivable from committed source, NOT reproduced from the recorded pin**: `ba23c3f1` is unrecoverable, so the provenance gate must be re-pinned. Only 7 of 8 rows have an overlay row to agree with. `clamp-fresh`'s z3 witness independently found the recorded 128-of-2^16 region at bf16 `0x8000`. Never infer formal status from correctness PASS |

Preceding round, retained for comparison and **not interchangeable** with the
above — different compiler binary, different tt-metal commit, ±1% band:

| Check | Result | Boundary |
|---|---|---|
| Full Blackhole sweep, 2026-09-28 | 263 PASS / 21 SKIP | Existing bounded pytest contracts |
| Semantic ON vs OFF | 189 faster / 74 parity / 0 slower | 263 rows, inclusive ±1% band |
| Semantic ON vs hand ON | 66 faster / 35 parity / 63 slower | 164 comparable rows, same band |
| Final vs preceding ON | 27 faster / 230 parity / 6 slower | Descriptive; source/compiler/profile differ |
| Fresh Linux compiler build | 8,023 PASS / 2 expected XFAIL / 0 unexpected | Includes 1,526 SFPI checks; not full Development CI |
| Header ABI | WH/BH/QSR PASS | Template instantiation, not hardware coverage on all three |
| Workflow tests | 57 PASS; setup/provenance regressions PASS | Host checks |
| Restricted multiply Z3 model | 11 checks PASS in fresh pinned environment | Source model on operands below 2^23, not machine-code proof |
| Formal / exhaustive / ULP in that sweep | NOT_RUN | Also not run in 2026-09-28; never infer these from correctness PASS |

The 2026-09-28 hardware compiler's `cc1plus` SHA-256 is
`65dfa31b0d76894ed46531aeb4964914e22931857293370261bbc07af9ce12be`.
The independent clean rebuild is
`be582de222aef1e7fe29c398e3027d1d960e60e1ddc641d9ea8eefbe014f82d6`.
The 2026-09-29 sweep's `cc1plus` is a third binary,
`1ade419280a690c2b8cefb0ab94f07251323cb3d390b7ab87c4f534c9f80dad7`; its
manifest records no source commit for it.
Source/build/test reproducibility is demonstrated; byte-identical rebuilds
and a hardware rerun using the second binary are not claimed.

## What to preserve

- Keep the BH SFPABS macro quarantine. Integer-abs macro execution produced
  wrong-code. The lost float-abs macro timing is a documented safety cost;
  a bounded historical float pass does not prove a safe mode-specific exception.
- Keep the narrow multiply contract: the limb-2 uplift is not generic Int32
  multiplication. Its source proof and existing tests have explicit domains.
- Keep numerical alternatives separate from ordinary compiler correctness.
  GELU's recovered timing uses the existing licensed source; it is not a new
  ULP admission or evidence of exact GELU equivalence.
- Do not globally enable record-hoist based on one winner. It helps five
  measured rows and hurts other measured rows. Select per operation.
- Use manifests for exact profiles. The final full sweep disables record-hoist;
  corpus defaults and historical board bookings are not interchangeable.
- Use `-###` when switching compiler prefixes. An earlier wrapper-provided
  `-B` took precedence over a later experimental `-B`; that control was invalid.
- Keep the coverage label attached to a bit-exactness verdict. A bounded sweep
  is `BIT-EXACT-PARTIAL-<n>-OF-2^32`, never `BIT-EXACT-ALL-INPUTS`; a reduced
  space refutes and never certifies. The tooling now says so itself.
- Describe the 17 census-silent options as *measured, changes no code*, citing
  the census — not as unmeasured gaps. The two genuinely unmeasurable ones are
  `-mtt-tensix-optimize-lp-schedule` (removed; the compiler names its
  replacement) and `-mtt-tensix-dst-layout-32b` (whole-TU declaration).
- Always state the band with a win/flat/loss count. Versus flags-off the result
  holds at every threshold; versus hand it does not, and dropping the band turns
  a tie into a claimed win.
- Keep the OFF-arm caveat attached to the numbers. 16 of the 38 enabled flags
  are absent from the OFF arm and off only because they are `Init(0)`. If any
  of those defaults is flipped, the OFF baseline is silently wrong and nothing
  in the harness will say so — recheck the list before reusing the profile.
- **Do not grade with an equivalence-only sweep.** 15 of the 18 defective
  strata have both arms out of contract, so 83% of them read as *agreement*
  when sem is compared to hand instead of to a golden. Grade each arm
  independently, which is what `threeway_golden` does. (Superseded phrasing:
  17 of 20, 85% — same argument, pre-fix oracle.)
- **Quote the oracle revision with any defect count.** The stratified ledger was
  re-graded once already because six modelling defects were found in the golden,
  and a bare "N defective strata" cannot be checked against anything. Say what it
  counts, at which `threeway_golden` revision, over which rows.
- **A wrong golden fails in both directions.** It fabricates defects (the `lgamma`
  stage-vs-composite error) *and* it licenses them (the deleted `ORACLE fp32
  OVERFLOW` class scored two infinities as 0 ULP, so an overflowing `i1` passed
  and the correct one failed). Re-grade for both when an oracle moves.
- **Keep "sem defect", "production defect" and "sem accuracy win" apart.** They
  are three different verdicts (`SEM-TOLERANCE-FAIL`,
  `SEM-TOLERANCE-PASS(hand fails tolerance)`, `TOLERANCE-BOTH-PASS` plus
  `candidate_not_worse`). An in-contract production arm that sem beats is a
  deliverable, not a bug report.
- **`formal` is re-derivable, not reproduced.** The recorded `ba23c3f1`
  simulator is gone for good; the reconstruction proves the verdicts from
  committed source on a re-pinned instrument. Do not write "reproduced from the
  pin", and do not let a passing leg imply the provenance gate is satisfied.
- **A clamp is not a saturation fix.** `setexp` with a full exponent field over
  a non-zero mantissa is a NaN, not `+inf`; check the output bit pattern, not
  that the guard fired.
- Run the host prerequisites before any census or parallel sweep: `ulimit -u
  1024` and `OPENBLAS_NUM_THREADS=1`. And build comparisons in **one** tree —
  `.text` embeds the checkout path on this target, so two copies of one commit
  give same-size different-byte `.text` and poison every `text_changed` fact.

## Resume in this order

1. Run the [setup and replay steps](#reproducing-a-recorded-run) in this handoff. The local [README.md](README.md)
   retains the SFPI source build instructions.
   Use new output directories and verify the actual installed toolchain.
2. Extend [per-LLK tuning](https://github.com/tenstorrent/craq-sfpi/blob/15ec9e7e92a92258d12956a75a802a1b705fe7f3/docs/LLK-TUNING.md) from known candidates, with the
   source/compiler frozen and correctness preceding three-repeat timing.
   Existing proposals are not deployed defaults or an exhaustive knob search.
3. Validate any selected dispatch configuration before promoting it. Do not
   relabel the fixed-profile sweep as a run of the combined tuned choices.
4. Follow [readiness](https://github.com/tenstorrent/craq-sfpi/blob/15ec9e7e92a92258d12956a75a802a1b705fe7f3/docs/READINESS.md) for upstream work. The isolated raw-LREG
   correctness patch is now open as
   [sfpi-gcc#22](https://github.com/tenstorrent/sfpi-gcc/pull/22) — mergeable,
   checks green, unreviewed, and dormant until the separate SFPI
   header/call-site change emits a marker. The remaining 20 stages of
   `nkapre/stack` are built but unsubmitted, and passing a build is not
   certification of upstream readiness.
5. **Clear the two outstanding owner decisions** in
   [State at handoff](#state-at-handoff) — the six drifted anchors and the
   `conf_lint` R7 exception — before the next weekly, or that weekly reads RED
   for reasons that have nothing to do with the compiler.
6. Work the remaining kernel defects. The `fresh_cpp` `erf`/`erfc`/sigmoid-LUT
   side is fixed (`e75553f1c3f`) and `rpow` is fixed (`f24d9515da4`); still open
   are the 18 defective strata over 9 op rows from the corpus extension
   (`softsign`, `i0`, `i1`, `i1-fresh`, `sqrtcustom`, `expm1cw`, `xielu`,
   `digamma`, `digamma-fresh`) and the 31 ranked defects from the static boundary
   audit. `lgamma` has left that list — its two strata were an oracle error and
   are withdrawn. Look at
   saturation and special-value handling first: 100% of a stratum out of
   contract at ~2^14-2^15 ULP is not accumulated inaccuracy, and the second
   overflow idiom `as<vFloat>((i<<23)+as<vInt>(w))` corrupts the **sign** bit.
   Re-run the stratified leg per fixed op, not a single band at 0.
7. Re-sweep what is only a baseline. `erf-fresh`, `erfc-fresh` and
   `sigmoidlut-fresh` were measured over 2^32 with **pre-fix** sem ELFs;
   `absint32` and `geluappx-fresh` lost slices to bugs now fixed; and
   `hardshrink-fresh` is bit-exact over the full space yet SEM-BUG at S3, which
   is unresolved.
8. Close formal/Galaxy/numeric gates separately. The formal leg now runs from a
   reconstructed instrument, but its recorded `ba23c3f1` pin is unrecoverable,
   so the **provenance gate has to be re-pinned** rather than satisfied — that
   is a decision, not a build. The 2^32 exhaustive leg is done for 14 ops on a
   galaxy. ULP coverage is 71 of 259 ops; 173 of the rest are harness-blocked
   and the 16-bit band mode is not started. Licensed-sigmoid infinity classes
   currently fail admission. Neither an override nor looser ordinary correctness
   tolerances closes any of this.

## Evidence map

- [2026-09-29 sweep](https://github.com/tenstorrent/craq-sfpi/blob/main/board/evidence/full-sweep-20260929/README.md): the current runtime measurement, its ±0.5% banding table, the outage diagnosis, and the flag-profile limits. Its `rows/` tree (1.6 GB) is not committed; the README says where the two node-local copies are.
- [Four-row re-run](https://github.com/tenstorrent/craq-sfpi/blob/main/board/evidence/full-sweep-20260929-rerun4/README.md): the four outage rows re-run green with the same compiler, tt-metal commit, seed and flags.
- [PR stack index](https://github.com/tenstorrent/craq-sfpi/blob/main/board/pr-packs/INDEX.md) and [per-stage packs](https://github.com/tenstorrent/craq-sfpi/tree/main/board/pr-packs): per-stage commit, flags with `Init()` defaults, coverage, runtime where any exists, and the per-stage "not verified" list.
- [Formal/ULP/exhaustive legs, 2026-09-29](https://github.com/tenstorrent/craq-sfpi/blob/main/board/evidence/formal-ulp-legs-20260929/NOTES.md): the first pass — the six SEM-BUG strata, the four machinery defects, and the 2^32 feasibility numbers that later proved to be a bad measurement.
- [Reconstructed formal instrument](https://github.com/tenstorrent/craq-sfpi/blob/main/board/evidence/formal-jo-instrument-recovered-20260929/NOTES.md): the rewritten `SFPUJO` hook, the 8 VALIDATED verdicts, the `clamp-fresh` z3 witness, and why re-derivable is not reproduced.
- [Full 2^32 on a galaxy](https://github.com/tenstorrent/craq-sfpi/blob/main/board/evidence/exhaustive-2p32-galaxy-20260930/RESULTS.md): per-op verdicts, diverging slice ids, the coverage arithmetic at NPAR=4 and NPAR=32, the corrected throughput, and the pre-fix-baseline caveat. Read `NOTES.md` §3b before quoting any of it.
- [Stratified ULP corpus extension](https://github.com/tenstorrent/craq-sfpi/blob/main/board/evidence/ulp-strata-corpus-20260930/NOTES.md): 369 cells, the both-legs-wrong argument, the four new strata and the coverage split. **Its counts are at the pre-fix oracle** — read it with [the re-grade](https://github.com/tenstorrent/craq-sfpi/blob/main/board/evidence/ulp-strata-regrade-20260930/NOTES.md) (18 / 9 / 15 at oracle `2107a950747`, every changed verdict in `RESULTS.tsv`) and [the six oracle modelling fixes](https://github.com/tenstorrent/craq-sfpi/blob/main/board/evidence/oracle-modelling-fixes-20260930/NOTES.md) that caused it.
- [ULP strata fixes](https://github.com/tenstorrent/craq-sfpi/blob/main/board/evidence/ulp-strata-fixes-20260929/NOTES.md): the `fresh_cpp/erf.h` diagnosis and before/after boards.
- [SFPU boundary audit](https://github.com/tenstorrent/craq-sfpi/blob/main/board/audit/sfpu-boundary-audit-20260929/README.md): 31 ranked defects, three classes, the second overflow idiom. Prefer its TSVs to its summary counts.
- [Production pow overflow clamp](https://github.com/tenstorrent/craq-sfpi/blob/main/board/evidence/binpow-overflow-clamp-20260929/NOTES.md): the fix, the `0x7F800000` bit-level check, and the two owner actions with their exact drift percentages.
- [Compile-time knob census, 2026-09-29](https://github.com/tenstorrent/craq-sfpi/blob/main/board/census/README-KNOB-CENSUS-20260929.md) and its [per-option table](https://github.com/tenstorrent/craq-sfpi/blob/main/board/census/OPTION-ATTRIBUTION-20260929.tsv): all 92 options, one row each, with the denominators kept apart.
- [Preceding 2026-09-28 sweep and generated report](https://github.com/tenstorrent/craq-sfpi/blob/15ec9e7e92a92258d12956a75a802a1b705fe7f3/board/evidence/compiler-tuning-repairs-20260928/full-sweep/README.md): exact manifests, counts, raw archive and checksums.
- [Fresh compiler build](https://github.com/tenstorrent/craq-sfpi/blob/15ec9e7e92a92258d12956a75a802a1b705fe7f3/board/evidence/compiler-tuning-repairs-20260928/fresh-build/README.md) and [network source restore](https://github.com/tenstorrent/craq-sfpi/blob/15ec9e7e92a92258d12956a75a802a1b705fe7f3/board/evidence/compiler-tuning-repairs-20260928/combined/SOURCE-RESTORE.md).
- [Performance reconciliation](https://github.com/tenstorrent/craq-sfpi/blob/15ec9e7e92a92258d12956a75a802a1b705fe7f3/docs/PERFORMANCE-DRIFT-20260928.md): all six final/prior slowdowns explained.
- [Five record-hoist A/Bs](https://github.com/tenstorrent/craq-sfpi/blob/15ec9e7e92a92258d12956a75a802a1b705fe7f3/board/evidence/performance-drift-20260928/final-hoist-controls.md): prior timings restored; proposals retained.
- [GELU and float-abs A/Bs](https://github.com/tenstorrent/craq-sfpi/blob/15ec9e7e92a92258d12956a75a802a1b705fe7f3/board/evidence/performance-drift-20260928/final-abs-gelu-tuning.md): GELU 31,033 → 28,857; abs unchanged at 20,538.
- [Float-abs archived route analysis](https://github.com/tenstorrent/craq-sfpi/blob/15ec9e7e92a92258d12956a75a802a1b705fe7f3/board/evidence/compiler-tuning-repairs-20260928/abs-route/README.md): actual old/new disassemblies and invalid-control correction.
- [Semantic uplift](https://github.com/tenstorrent/craq-sfpi/blob/15ec9e7e92a92258d12956a75a802a1b705fe7f3/docs/SEMANTIC-UPLIFT-REVIEW.md), [compiler repair boundaries](https://github.com/tenstorrent/craq-sfpi/blob/15ec9e7e92a92258d12956a75a802a1b705fe7f3/docs/COMPILER-TUNING-REPAIR-REVIEW.md), and [historical win contracts](https://github.com/tenstorrent/craq-sfpi/blob/15ec9e7e92a92258d12956a75a802a1b705fe7f3/docs/HISTORICAL-WIN-CONTRACTS.md).

Historical `board/FINAL-BOARD.tsv` remains a historical per-operation booking,
not the latest common-profile report. Prefer links to preserved evidence over
duplicate board snapshots or new parallel runners.
