# Worklog

## 2026-09-29 — partition the branch into a reviewable PR stack

- Split the 177,787-line `nkapre/sfpi` compiler branch into **21
  dependency-ordered commits** on `nkapre/stack` in `tenstorrent/craq-sfpi-gcc`,
  tip `8477d32d6ff`, onto upstream base `ba48edbef33`. Cumulative diff versus
  that base: 1,704 files / 166,106 insertions.
- Built every stage on its own with `make all-gcc -j16`: **21 of 21 at rc=0,
  0 compiler errors**, no content deviation from the partition manifest.
  `rvtt.exp` ran at stage 21 only: 6478 expected passes / 9 unexpected
  failures, against source `566071bf728` rebuilt in the same objdir at
  6478 / 8. The one differing test is `rv/zbkb.C`: the stack deliberately does
  not revert upstream `ba48edbef33` (the xttzbkb commit), whose test the source
  branch deletes. It fails for a missing target assembler in an `all-gcc`-only
  objdir, the same cause as the `shft2-chained-copy4-assemble-bh.C` failure
  present in both runs. Intermediate stages are proven by build, not testsuite.
- Dropped stage 22 of the original partition deliberately: 7,440 lines of dead
  source (`rtl-rvtt-replay-{hoist,crf,discover}.cc`, `rtl-rvtt-replay-int.h`
  — none in `RVTT_OBJS`, included by nothing but each other, duplicating
  symbols `rtl-rvtt-replay.cc` defines) plus the `rv/zbkb.C` deletion that
  would have reverted upstream.
- Wrote one evidence pack per stage recording its commit, the `-mtt-tensix-*`
  flags it adds with their `Init()` defaults, LLK coverage, runtime where any
  exists, correctness, reproduction, and an explicit "not verified" list.
- Opened the first upstream PR of the campaign:
  [sfpi-gcc#22](https://github.com/tenstorrent/sfpi-gcc/pull/22),
  `tt: preserve raw LREG live-ins through IRA`, branch `nkapre/pr-lreg-livein`,
  683 added lines over 11 files, mergeable, 4/4 Cycode checks green. Its
  metadata builtin has no in-tree caller yet; the pass is dormant until the
  separate SFPI header/call-site change emits a marker.

Evidence: [PR stack index](https://github.com/tenstorrent/craq-sfpi/blob/main/board/pr-packs/INDEX.md),
[per-stage packs](https://github.com/tenstorrent/craq-sfpi/tree/main/board/pr-packs).

## 2026-09-29 — re-measure the full corpus, and state what it does not cover

A fresh Blackhole sweep completed all 284 registered rows: **263
bounded-correctness PASS, 21 declared SKIP, 0 correctness failures**, seed 42,
three paired repetitions, on tt-metal `6acf1685f172d16d`.

Four rows first recorded FAIL. That was a host-side outage, not a compiler
result: all eight failing legs exited status 3 (pytest's `INTERNAL_ERROR`; a
numeric mismatch exits 1) with an identical `UmdBaseException: Query mappings
failed on device 0` raised from `conftest.py` `pytest_configure`, before
collection — no test ran, no kernel loaded. `lgamma-fitted`'s ON arm rebuilt
byte-identically and passed 30/30, and all four re-ran green.

**Counts are banded at ±0.5% and the band must be quoted with them.** Over
the 263 PASS rows:

| Comparison | Faster/beat | Flat/tie | Slower/hand wins |
|---|---:|---:|---:|
| ON versus flags-off, 263 rows | 192 | 69 | 2 |
| ON versus handwritten ON, 164 rows | 71 | 25 | 68 |

The first is robust at every threshold. The second is not: at raw sign it is
79 / 3 / 82 and at ±1% it is 66 / 35 / 63. **Compiler versus hand is a
statistical tie, not a win.** The two regressions versus flags-off are
`reduce-sdpa` (+1.02%) and `fill-fresh` (+0.68%).

What the sweep does not cover, stated plainly:

- It enabled **38 of the 92 options the stack defines**. **53 appear in neither
  arm**; a 54th, `-mtt-tensix-optimize-replay-record-hoist`, is named
  negatively and so is pinned OFF in both arms and not differentially measured.
- **7 of 21 stages have no runtime data at all**: 03, 05, 12, 13, 15, 16, 18.
- `formal`, `exhaustive` and `ulp_admission` are all **NOT_RUN**; they need a
  pinned `libttsim.so` host that was unavailable.
- Four unmeasured options — `-mtt-tensix-optimize-reassoc`,
  `-mtt-tensix-optimize-reassoc-mad-restructure`,
  `-mtt-tensix-optimize-stochrnd-store-fold`, `-mtt-tensix-optimize-store-sink`
  — are `LICENSED_TARGET_FLAGS` in the workflow repository's
  `scripts/select_llk_tuning.py`, which the
  ordinary per-op selector refuses and routes to the separate ULP admission
  flow. That is the likely reason they are outside the common profile.

Methodology caveat worth keeping: the A/B is **not a literal negation**. ON is
39 tokens (38 positive + 1 explicit `-mno-`), OFF is 22, and 16 of the 38
enabled flags have no `-mno-` counterpart in OFF. All 16 are `Init(0)`, so the
comparison is valid — by virtue of defaults, not by construction. A future
default flip would leave one enabled in both arms and silently invalidate the
OFF baseline, with no error from the harness.

A per-knob campaign to fill in the missing runtime coverage is planned and
queued as a Slurm batch job; it measures each unmeasured option's marginal
contribution on top of the existing 39-token profile, not its standalone
effect. Nothing is measured there yet.

Evidence: [2026-09-29 sweep](https://github.com/tenstorrent/craq-sfpi/blob/main/board/evidence/full-sweep-20260929/README.md),
[four-row re-run](https://github.com/tenstorrent/craq-sfpi/blob/main/board/evidence/full-sweep-20260929-rerun4/README.md),
[banding table](https://github.com/tenstorrent/craq-sfpi/blob/main/board/pr-packs/INDEX.md#sign-convention-and-banding),
[queued per-knob campaign](https://github.com/tenstorrent/craq-sfpi/blob/main/board/evidence/incremental-knobs-20260929/PLAN.md).

## 2026-09-28 — consolidate operator commands into HANDOFF

Moved the fresh-machine recipe into HANDOFF.md and removed the duplicate
reproduction README. HANDOFF now includes exact-profile replay, targeted knob
A/Bs, compiler/header diagnosis, failure-log inspection, compiler tests and
the restricted multiply proof command. README is an entry point; workflow
and evidence READMEs link to the same command guide. No new device results
are claimed by this documentation-only consolidation.
Removed the obsolete pin-47 operator handoff at
`docs/handoff-20260817/HANDOFF.md`; its historical contents remain recoverable
from Git at `639baeb:docs/handoff-20260817/HANDOFF.md`. Root HANDOFF.md is the
only current operator handoff.

## 2026-09-28 — close the fresh-machine handoff gap

The earlier handoff explained auditing better than bootstrapping. Added a
[fresh-clone reproduction path](HANDOFF.md#reproducing-a-recorded-run) with explicit WORK,
portable four-repository pins, the `tests/sfpi` selector, setup-state scope,
and exact-profile smoke/full commands. Corrected the source-bundle generator
to omit WORK and tested sourcing a relocated pin file with two chosen roots.
Because workflow main is out of scope, the tested fix is carried as an
applicable patch on source `nkapre/sfpi`, not as a duplicate runner or a main
branch commit. No new silicon run is claimed for this documentation change.

This log records completed work and corrections, not a release certificate.
See [HANDOFF.md](HANDOFF.md) for current source pins and remaining tasks,
the [workflow README](https://github.com/tenstorrent/craq-sfpi/blob/15ec9e7e92a92258d12956a75a802a1b705fe7f3/README.md) for campaign commands, and the linked evidence for raw results.
The entries below summarize the September 27–28 records; they are not an
exhaustive transcript of the earlier campaign.

## 2026-09-27 — establish a working baseline

- Repaired compiler and harness integration, paired headers/toolchain, and
  hardened ABI checks against dependent-template and target-specific errors.
- Ran compiler-only and then SFPI-enabled suites; the latter at GCC
  `aeafe77cdabd` recorded 8,003 PASS, two expected XFAILs, zero unexpected failures.
- Preserved a complete 284-row Blackhole sweep: 263 bounded PASS, 21 SKIP.
  At a 1% band its ON/hand counts were 68/34/62 and ON/OFF counts 186/76/1.
- Kept the raw-LREG upstream correctness patch distinct from the downstream
  optimization stack. Passing the target suite did not establish full
  Development-workflow health or readiness of all optimization PRs.

Evidence: [compiler repairs](https://github.com/tenstorrent/craq-sfpi/blob/15ec9e7e92a92258d12956a75a802a1b705fe7f3/board/evidence/compiler-repairs-20260927/README.md),
[replay timeout investigation](https://github.com/tenstorrent/craq-sfpi/blob/15ec9e7e92a92258d12956a75a802a1b705fe7f3/board/evidence/replay-timeout-20260927/README.md).

## 2026-09-28 — reconcile what the wins meant

- Separated correctness PASS, ON/OFF improvement, and performance versus hand.
  The historical board booked per-operation flags and sometimes licensed or
  restricted-domain source arms; common ON was not a replay of those bookings.
- Distinguished the 22 preceding-run ON slowdowns from the older large gaps
  against hand. Same-compiler controls showed record-hoist hurting tanhshrink,
  scaled SiLU and polygamma; this did not establish its effect on every LLK.
- Audited source contracts and compiler dispatch: fixes match dataflow,
  ownership, CPU/opcode/mode properties, not kernel-name special cases.

Evidence: [reconciliation](https://github.com/tenstorrent/craq-sfpi/blob/15ec9e7e92a92258d12956a75a802a1b705fe7f3/docs/PERFORMANCE-DRIFT-20260928.md),
[historical contracts](https://github.com/tenstorrent/craq-sfpi/blob/15ec9e7e92a92258d12956a75a802a1b705fe7f3/docs/HISTORICAL-WIN-CONTRACTS.md).

## 2026-09-28 — repair compiler and semantic source

| Commit | Repair |
|---|---|
| GCC `2cb75df9ce8` | Remove dead boundary materializations after LUT conversion |
| GCC `eeb0ad2b514` | Quarantine incorrect BH SFPABS macro execution; retain explicit lowering |
| GCC `0ef7438be79` | Remove converted LUT coefficient roots while preserving shared uses |
| GCC `566071bf728` | Route BH SFPCAST mod3 through VD rather than unsupported LReg16 delivery |
| tt-metal `b06bb841014` | Read restricted limb-2 multiply inputs as vSMag; retain output conversion and add pinned source-model proof |
| SFPI `b375545` | Pin the validated final GCC revision |

The LUT diagnosis was corrected: coefficient placement already succeeded;
unnecessary materializations survived conversion. The abs value identity was
not the wrong-code cause; macro execution was. The multiply uplift is valid
only for its declared operand domain, not a generic full-domain replacement.

Evidence: [compiler review](https://github.com/tenstorrent/craq-sfpi/blob/15ec9e7e92a92258d12956a75a802a1b705fe7f3/docs/COMPILER-TUNING-REPAIR-REVIEW.md),
[semantic review](https://github.com/tenstorrent/craq-sfpi/blob/15ec9e7e92a92258d12956a75a802a1b705fe7f3/docs/SEMANTIC-UPLIFT-REVIEW.md),
[combined validation](https://github.com/tenstorrent/craq-sfpi/blob/15ec9e7e92a92258d12956a75a802a1b705fe7f3/board/evidence/compiler-tuning-repairs-20260928/combined/README.md).

## 2026-09-28 — prove source/build reproduction

- Exported and restored the published source pins through fresh network clones.
  Checked all SFPI submodule gitlinks and their remote fetchability.
- Built the full toolchain from a clean detached Linux checkout in a separate
  object/install tree, without reusing the sweep compiler's objects.
- Fresh suite: 8,023 PASS, two expected XFAILs, zero unexpected results,
  including 1,526 SFPI checks. WH/BH/QSR template ABI probes passed.
- Fresh pinned Z3 environment: all 11 restricted-multiply source-model checks
  passed. This is not a proof of generated machine code.
- Recorded distinct old/new compiler hashes; did not claim byte identity or
  transfer the original binary's hardware result onto the rebuilt binary.

Evidence: [fresh build](https://github.com/tenstorrent/craq-sfpi/blob/15ec9e7e92a92258d12956a75a802a1b705fe7f3/board/evidence/compiler-tuning-repairs-20260928/fresh-build/README.md),
[source restore](https://github.com/tenstorrent/craq-sfpi/blob/15ec9e7e92a92258d12956a75a802a1b705fe7f3/board/evidence/compiler-tuning-repairs-20260928/combined/SOURCE-RESTORE.md).

## 2026-09-28 — finish the full sweep and isolate remaining drift

The frozen final run completed 284/284 rows: **263 PASS, 21 SKIP, zero failures
or timeouts**. Each of 854 passing cells retained three timing samples and ELF
inventories. The archived report uses the same inclusive 1% band throughout:

| Comparison | Faster | Parity | Slower |
|---|---:|---:|---:|
| ON versus OFF, 263 rows | 189 | 74 | 0 |
| ON versus handwritten ON, 164 rows | 66 | 35 | 63 |
| Final versus prior ON, descriptive | 27 | 230 | 6 |

Formal, exhaustive and ULP admission were NOT_RUN in this sweep.

Follow-up controls explained all six final/prior slowdowns:

- Record-hoist ON restored all five prior medians for three reduction rows,
  fitted tanh derivative and typecast. These are per-row proposals, not a
  recommendation to enable the option globally.
- Float `abs` stayed at 20,538 with record-hoist OFF or ON, disproving that
  hypothesis. Actual archived old ON code used macro delivery at 17,851;
  the repaired code uses explicit instructions under the SFPABS quarantine.
- An attempted old-compiler `-B` control accidentally invoked the new compiler
  because its wrapper's prefix took precedence. `-###` exposed the error;
  the invalid control is retained and excluded from attribution.
- GELU's crosscall-config-prefix option restored exactly 28,857 cycles from
  31,033, at parity with hand. Restricted multiply separately measured 35,117
  with macro-planner disabled, versus the 38,702 same-source baseline.
- Historical tuned AbsInt32 at 16,740 was not restored; the safe explicit
  int-abs result is 17,720. Correctness quarantine was not weakened for speed.

Evidence: [full sweep](https://github.com/tenstorrent/craq-sfpi/blob/15ec9e7e92a92258d12956a75a802a1b705fe7f3/board/evidence/compiler-tuning-repairs-20260928/full-sweep/README.md),
[hoist controls](https://github.com/tenstorrent/craq-sfpi/blob/15ec9e7e92a92258d12956a75a802a1b705fe7f3/board/evidence/performance-drift-20260928/final-hoist-controls.md),
[GELU/abs controls](https://github.com/tenstorrent/craq-sfpi/blob/15ec9e7e92a92258d12956a75a802a1b705fe7f3/board/evidence/performance-drift-20260928/final-abs-gelu-tuning.md),
[abs routes](https://github.com/tenstorrent/craq-sfpi/blob/15ec9e7e92a92258d12956a75a802a1b705fe7f3/board/evidence/compiler-tuning-repairs-20260928/abs-route/README.md).

## 2026-09-28 — close the workflow and hand off

- Added a deterministic completed-sweep reporter with explicit denominators
  and separate verification statuses; prior-run comparisons remain descriptive.
- Added/tested tuning proposals with fixed baselines, actual sample medians,
  text-alias deduplication, and correctness-failure exclusion from selection.
- Recorded tracked source patches for new runs without a blanket dirty-tree
  refusal; legacy dirty records remain explicitly unbound to source contents.
- Final host suite: 57 PASS; shell setup/provenance regressions PASS.
- Pushed and checked both source mirror pairs, tt-metal, and workflow `main`.
  Implementation/evidence checkpoint: `15ec9e7`. Released both allocations.
- Prepared documentation, then placed the README update, this worklog, and the
  handoff on the SFPI-source `nkapre/sfpi` branch as requested. No documentation
  commit is directed to workflow `main`. The handoff records exact-profile replay
  rather than implying corpus defaults reproduce the measured final run.

Remaining work is explicit: per-operation deployment validation and broader
tuning, upstream PR extraction/CI, the missing instrumented formal simulator,
full Galaxy coverage, and complete numeric admission. The licensed-sigmoid
infinity rejection is still a blocker, not an exception to suppress.
