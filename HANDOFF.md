# Handoff — 2026-09-28

## State at handoff

**Start here:** [setup and replay](#reproducing-a-recorded-run), then
[focused problem reproduction](#reproduce-and-diagnose-a-specific-problem).
All runnable campaign commands live here; archived paths are not local paths.

The compiler-repair, drift-diagnosis, and reproducibility round is complete.
The final Blackhole sweep completed all 284 rows: **263 bounded-correctness
PASS, 21 declared SKIP, zero failures or timeouts**. This is not completion of
the entire upstreaming or formal/numeric certification program.

No hardware test is running. Allocations `122075` (sweep) and `122098`
(fresh build) were released. Evidence is preserved in the pinned workflow repository linked below; do not depend on a
compute node or an old terminal session remaining available.

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

The prior measured acceptance was 284 completed, 263 bounded PASS, 21 declared
SKIP; at ±1%, ON/hand was 66/35/63 and ON/OFF 189/74/0. A rerun produces new
evidence tied to its compiler and device, not a guarantee of identical cycle
counts or binary hashes. Formal, exhaustive and ULP admission are separate.
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

This document is a subsequent documentation-only update on the SFPI source
branch; the table preserves the previously validated source checkpoint. Workflow `main` and the
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

| Check | Result | Boundary |
|---|---|---|
| Full Blackhole sweep | 263 PASS / 21 SKIP | Existing bounded pytest contracts, not all-input proof |
| Semantic ON vs OFF | 189 faster / 74 parity / 0 slower | 263 rows, inclusive ±1% band |
| Semantic ON vs hand ON | 66 faster / 35 parity / 63 slower | 164 comparable rows, same band |
| Final vs preceding ON | 27 faster / 230 parity / 6 slower | Descriptive; source/compiler/profile differ |
| Fresh Linux compiler build | 8,023 PASS / 2 expected XFAIL / 0 unexpected | Includes 1,526 SFPI checks; not full Development CI |
| Header ABI | WH/BH/QSR PASS | Template instantiation, not hardware coverage on all three |
| Workflow tests | 57 PASS; setup/provenance regressions PASS | Host checks |
| Restricted multiply Z3 model | 11 checks PASS in fresh pinned environment | Source model on operands below 2^23, not machine-code proof |
| Formal / exhaustive / ULP in full sweep | NOT_RUN | Never infer these from correctness PASS |

The hardware compiler's `cc1plus` SHA-256 is
`65dfa31b0d76894ed46531aeb4964914e22931857293370261bbc07af9ce12be`.
The independent clean rebuild is
`be582de222aef1e7fe29c398e3027d1d960e60e1ddc641d9ea8eefbe014f82d6`.
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

## Resume in this order

1. Run the [setup and replay steps](#reproducing-a-recorded-run) in this handoff. The local [README.md](README.md)
   retains the SFPI source build instructions.
   Use new output directories and verify the actual installed toolchain.
2. Extend [per-LLK tuning](https://github.com/tenstorrent/craq-sfpi/blob/15ec9e7e92a92258d12956a75a802a1b705fe7f3/docs/LLK-TUNING.md) from known candidates, with the
   source/compiler frozen and correctness preceding three-repeat timing.
   Existing proposals are not deployed defaults or an exhaustive knob search.
3. Validate any selected dispatch configuration before promoting it. Do not
   relabel the fixed-profile sweep as a run of the combined tuned choices.
4. Follow [readiness](https://github.com/tenstorrent/craq-sfpi/blob/15ec9e7e92a92258d12956a75a802a1b705fe7f3/docs/READINESS.md) for upstream work: maintainer alignment,
   isolated raw-LREG correctness patch, infrastructure extraction, then small
   independently tested pass PRs. No PR was opened by this round; all 31 passes
   are not certified upstream-ready.
5. Close formal/Galaxy/numeric gates separately. The required instrumented JO
   simulator and observation patch were not found; full exhaustive coverage is
   unmeasured. Licensed-sigmoid infinity classes currently fail admission.
   Neither an override nor looser ordinary correctness tolerances closes this.

## Evidence map

- [Final sweep and generated report](https://github.com/tenstorrent/craq-sfpi/blob/15ec9e7e92a92258d12956a75a802a1b705fe7f3/board/evidence/compiler-tuning-repairs-20260928/full-sweep/README.md): exact manifests, counts, raw archive and checksums.
- [Fresh compiler build](https://github.com/tenstorrent/craq-sfpi/blob/15ec9e7e92a92258d12956a75a802a1b705fe7f3/board/evidence/compiler-tuning-repairs-20260928/fresh-build/README.md) and [network source restore](https://github.com/tenstorrent/craq-sfpi/blob/15ec9e7e92a92258d12956a75a802a1b705fe7f3/board/evidence/compiler-tuning-repairs-20260928/combined/SOURCE-RESTORE.md).
- [Performance reconciliation](https://github.com/tenstorrent/craq-sfpi/blob/15ec9e7e92a92258d12956a75a802a1b705fe7f3/docs/PERFORMANCE-DRIFT-20260928.md): all six final/prior slowdowns explained.
- [Five record-hoist A/Bs](https://github.com/tenstorrent/craq-sfpi/blob/15ec9e7e92a92258d12956a75a802a1b705fe7f3/board/evidence/performance-drift-20260928/final-hoist-controls.md): prior timings restored; proposals retained.
- [GELU and float-abs A/Bs](https://github.com/tenstorrent/craq-sfpi/blob/15ec9e7e92a92258d12956a75a802a1b705fe7f3/board/evidence/performance-drift-20260928/final-abs-gelu-tuning.md): GELU 31,033 → 28,857; abs unchanged at 20,538.
- [Float-abs archived route analysis](https://github.com/tenstorrent/craq-sfpi/blob/15ec9e7e92a92258d12956a75a802a1b705fe7f3/board/evidence/compiler-tuning-repairs-20260928/abs-route/README.md): actual old/new disassemblies and invalid-control correction.
- [Semantic uplift](https://github.com/tenstorrent/craq-sfpi/blob/15ec9e7e92a92258d12956a75a802a1b705fe7f3/docs/SEMANTIC-UPLIFT-REVIEW.md), [compiler repair boundaries](https://github.com/tenstorrent/craq-sfpi/blob/15ec9e7e92a92258d12956a75a802a1b705fe7f3/docs/COMPILER-TUNING-REPAIR-REVIEW.md), and [historical win contracts](https://github.com/tenstorrent/craq-sfpi/blob/15ec9e7e92a92258d12956a75a802a1b705fe7f3/docs/HISTORICAL-WIN-CONTRACTS.md).

Historical `board/FINAL-BOARD.tsv` remains a historical per-operation booking,
not the latest common-profile report. Prefer links to preserved evidence over
duplicate board snapshots or new parallel runners.
