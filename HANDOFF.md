# Handoff — 2026-09-28

## State at handoff

The compiler-repair, drift-diagnosis, and reproducibility round is complete.
The final Blackhole sweep completed all 284 rows: **263 bounded-correctness
PASS, 21 declared SKIP, zero failures or timeouts**. This is not completion of
the entire upstreaming or formal/numeric certification program.

No hardware test is running. Allocations `122075` (sweep) and `122098`
(fresh build) were released. Evidence is preserved in the pinned workflow repository linked below; do not depend on a
compute node or an old terminal session remaining available.

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

1. Read the [workflow README](https://github.com/tenstorrent/craq-sfpi/blob/15ec9e7e92a92258d12956a75a802a1b705fe7f3/README.md) for setup and reporting,
   and the exact-profile replay command below. The local [README.md](README.md)
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

## Replay the measured profile

Run this from the separate craq-sfpi **workflow checkout** at `15ec9e7e92a92258d12956a75a802a1b705fe7f3`,
not from this SFPI source checkout. Set `WORK` to the prepared source/build
root and use a newly allocated Blackhole device. Verify the installed chain
first using the linked workflow instructions. Corpus defaults are not the
recorded final profile.

```sh
python3 - "$WORK/tt-metal/tt_metal/tt-llk/tests" "$WORK/llk-final-profile-new" <<'PY'
import json
import subprocess
import sys
from pathlib import Path

manifest = Path("board/evidence/compiler-tuning-repairs-20260928/full-sweep/manifest.json")
flags = json.loads(manifest.read_text())["flags"]
subprocess.run([
    sys.executable, "scripts/sweep_llks.py", "--tests-root", sys.argv[1],
    "--out", sys.argv[2], "--repeats", "3",
    "--off-flags=" + flags["OFF_FLAGS"],
    "--on-flags=" + flags["ON_FLAGS"],
], check=True)
PY
```

This preserves record-hoist OFF. A rebuilt compiler has its own binary identity
and needs new evidence; it does not inherit the archived binary's measurement.
For a completed run, use `scripts/report_llk_sweep.py --current RUN --out NEW_JSON`
from that same workflow checkout. Use `scripts/select_llk_tuning.py` for
measured per-operation proposals; it neither launches a search nor changes
production defaults.
