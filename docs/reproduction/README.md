# Reproduce the recorded compiler/LLK run from a fresh machine

This is the first-mile recipe, not merely an evidence-auditing recipe. Use a
Linux build host with GitHub access, Git, a C/C++ toolchain, SFPI's build
prerequisites, Python 3.11+ with venv/pip and sufficient local disk. Measuring
requires an allocated Blackhole device with its driver/runtime already
installed. Setup does not allocate a device or install its driver. Build and
run on the same node; node-local home directories are not shared.

## 1. Obtain the source docs and the exact workflow

Use new destination directories. No preexisting four-repository checkout and
no `source_bundle.py export` are needed:

```sh
git clone --branch nkapre/sfpi git@github.com:tenstorrent/sfpi.git sfpi-handoff
export SFPI_HANDOFF="$(cd sfpi-handoff && pwd)"
git clone git@github.com:tenstorrent/craq-sfpi.git craq-workflow
cd craq-workflow
git checkout --detach 15ec9e7e92a92258d12956a75a802a1b705fe7f3
git apply --check "$SFPI_HANDOFF/docs/reproduction/workflow-portability.patch"
git apply "$SFPI_HANDOFF/docs/reproduction/workflow-portability.patch"
```

Why a patch: this campaign's workflow is a different tree from the SFPI-source
`nkapre/sfpi` mirror. The owner requested changes only on the source branch,
not workflow `main`. The patch fixes the original generator and its tests,
adds portable recorded-run pins and updates the workflow README and headline
run README. It does not duplicate the runner or modify any archived result.
Keep this applied patch when sharing the reproduction workspace; its paths are
relative and its base is the immutable workflow revision above.

## 2. Choose local storage, load pins, build and wire the harness

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

The expected target assumes default `--build-dir build`. The generated setup
state belongs to this machine; do not copy an old machine's SETUP-STATE.env.
`corpus/sweep.sh` checks `CRAQ_SETUP_STATE`; the lightweight `sweep_llks.py`
records tool identity but does not use that variable as a state gate.

For an unpinned development setup only, omit sourcing the pin file and run
`bash scripts/setup.sh`: WORK defaults to `$HOME/craq-build` and source refs
default to moving `nkapre/sfpi` branches. That is not historical reproduction.

## 3. Smoke, then full manifest-profile sweep

From this same patched workflow checkout and allocated node:

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
