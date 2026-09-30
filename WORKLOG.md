# Worklog

## 2026-09-29 — run the validation legs, and correct why the formal one cannot

The reason this campaign gave for `formal`, `exhaustive` and `ulp_admission`
being NOT_RUN — *"a pinned `libttsim.so` host that was unavailable"* — was
**wrong**, and it had been propagated into the workflow README, this worklog,
HANDOFF and all 21 per-stage evidence packs. `libttsim.so` builds from craq-sim
exactly as that repository's README documents
(`TT_VERSION=1 ./make.py src/_out/release_bh/libttsim.so`); all three targets
rebuilt at craq-sim `6db1103a` with zero errors. Two of the three legs then ran.

- **Exhaustive: ran**, at 2^16, 32/32 manifest rows, with real silicon anchors:
  **16 BIT-EXACT-ALL-INPUTS, 13 DIVERGENT, 3 refused**, zero class
  contradictions with the recorded silicon overlay, and produced at cc1plus
  `d1f90de7` rather than pin-59 — so that equivalence partition holds across two
  compiler builds. Two of the three refusals are a simulator-fidelity finding:
  **craq-sim `1c47e9cd` is not bit-faithful to Blackhole silicon on
  `mish-fitted` and `recip`**, their device anchors failed, and the machinery
  fail-closed rather than counting the sim sweep.
- **Full 2^32 is not feasible on this hardware.** Measured 65.5k
  patterns/s/chip: one op is ~36.4 h on one chip, ~9.1 h on four, and the 31
  single-2^32 ops are ~12 days of the whole box. A reduced space is a probe, not
  a substitute — 2^20 is 0.024% of 2^32 — and must not be reported as one.
- **ULP: ran, on silicon, and found real kernel bugs.** The claim that this leg
  was "library only, no `--golden` wiring" was stale: `--golden` is wired
  through the harness's `SFPU_GOLDEN` variable into `test_sfpu_unary.py:95`. The
  leg ran 31 ops x 5 exponent strata. **Six strata across four `fresh_cpp`
  kernels come back SEM-BUG** — the compiled kernel outside the documented bf16
  ULP contract where the hand-written production kernel is inside it, and
  bitwise divergent from it on silicon: `erf-fresh` S2 and S4 (32513 ULP, 100%
  of the stratum out of contract, hand 0), `erfc-fresh` S2 and S4,
  `hardshrink-fresh` S3, `sigmoidlut-fresh` S4. These are open findings;
  **nothing is fixed here.** `rpow` S4 is the opposite polarity — production at
  16384 ULP, compiled leg exact — so it is a finding against production.
  None of it is visible from the standing unstratified probe, because
  `[0, 2^18)` read as u32 bit patterns is entirely positive subnormals and zero.
  The strata are themselves probes: 0.24% of 2^32 at five hand-picked exponents,
  every verdict labelled `BIT-EXACT-PARTIAL-65536-OF-2^32`. They refute; they
  certify nothing.
- **Formal: did not run, for a different and more serious reason.** The leg does
  not want a plain `libttsim.so`; it wants one emitting the `SFPUJO I/V/M/C`
  stream under `TTSIM_TRACE_SFPU_STREAM`, and **that build exists in no
  committed source.** Verified four ways: `strings` on all three `libttsim.so`
  on the box including the fresh build; `git grep` over every craq-sim remote
  branch; no saved diff anywhere; nothing hashing to the pinned `ba23c3f1`. The
  fresh build is further away still, having lost `TTSIM_TRACE_LOADMACRO` that
  the two pinned checkouts keep. Pin-59 cc1plus `b013967fffaa` is also gone and
  there are no cached traces, so it cannot be re-proven offline either. All 40
  formal-routed ops fail closed as UNSWEPT / LEG-FAILED with
  **`SMT-PROVEN-ALL-INPUTS = 0`**, and the prover itself is sound
  (`selftest_formal_equiv.py`, 204/204 golden vectors, device-free). **Formal
  results have never been reproducible from committed source.**
- **Four defects in the measuring machinery, all of which inflated confidence.**
  (1) `fp32_stream_sweep.py` / `binary_stream_sweep.py` let a reduced sweep
  certify itself: `covered == args.total` only proves the bands tile the range
  *requested*, so a `--total` of 2^18 stamped `BIT-EXACT-ALL-INPUTS` while the
  summary beside it recorded `full 2^32=False`. Fixed on tt-metal `nkapre/sfpi`
  as `3f1979755f3` — partial runs now say `BIT-EXACT-PARTIAL-<n>-OF-2^32`, exit
  status follows the comparison rather than the label, and consumers gate on a
  `BIT-EXACT` prefix. (2) `prove_all.py` computed its certified count by
  subtraction, counting `UNSWEPT`, `SCOPE-REFUSED`, `INFEASIBLE-2^32` and
  `SIM-BIT-EXACT-16` as certified: a run with no successful proof printed
  "certified-or-domain in fast set = 31". It now counts positively. (3) Two
  knobs (`macro-planner-replay`, `optimize-mop-form`) were classified `solo`
  while the pipelines they depend on were off in the solo baseline — a
  structural A/A; `planner-replay`'s long-standing NO-FIRE measured the leg
  shape, not the pass, and under drop-one it fires on 10 ops. (4) `classify()`
  keyed its cache on the work directory, recompiling the identical baseline leg
  once per knob per row; memoised. (3) and (4) landed as `ede53dea74f`, and
  together they took the census from ~5 to ~150 verdicts a minute, which is what
  made full-corpus coverage affordable rather than a sample.
- **Every compile-time option is now accounted for.** A compile-only census —
  263 rows x 27 knobs = 7,101 A/B verdicts, `full_registry_coverage: true` —
  closes the firing-record gap for all 92 options the stack adds: **73 fire, 17
  were measured and change no code on any of the 263 rows, and 2 are genuinely
  unmeasurable** (`-mtt-tensix-optimize-lp-schedule`, which the compiler rejects
  with *"was removed; use `-mtt-tensix-optimize-pressure-schedule`"*, and
  `-mtt-tensix-dst-layout-32b`, a whole-TU declaration for which a knob would be
  an A/A). Ten are newly measured as firing: `lreg-rename-chains` 74 ops,
  `reassoc-loop-carried` 47, `hoisted-prgm-reuse` 26,
  `delivery-shape-min-benefit=0` 24, `macro-planner-replay` 10,
  `crossrow-pairing-stall-words` 8, `ims-budget=1` 6, `macro-ims` 4,
  `replay-hoist-completion-guard` 3, `crosscall-addrmod` 1. `FIRE-BREADTH.tsv`
  was regenerated (66 -> 91 rows, 50 `ops` counts corrected, 3 verdicts flipped,
  `--self-check` 91/91). **The 17 silent options are "measured, changes no code",
  not unmeasured gaps** — they should be cited to the census. Firing is a
  compile-time fact and still not a cycle measurement, so the runtime gap below
  stays open.
- **33 phantom `COMPILE_FAIL`s were host exhaustion, not compilation.** Every
  pytest session spawns 48 BLAS threads, so N concurrent workers exceed a
  512-process cap and the losers die with
  `posix_spawn: Operation not permitted`. Run with `ulimit -u 1024` and
  `OPENBLAS_NUM_THREADS=1`.

Evidence: [formal/ULP legs](https://github.com/tenstorrent/craq-sfpi/blob/main/board/evidence/formal-ulp-legs-20260929/NOTES.md),
[knob census](https://github.com/tenstorrent/craq-sfpi/blob/main/board/census/README-KNOB-CENSUS-20260929.md),
[per-option table](https://github.com/tenstorrent/craq-sfpi/blob/main/board/census/OPTION-ATTRIBUTION-20260929.tsv),
[fire-breadth ledger](https://github.com/tenstorrent/craq-sfpi/blob/main/board/ledgers/FIRE-BREADTH.tsv).

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
  That runtime gap is still open; the *compile-time* gap was closed later the
  same day by the knob census in the entry above.
- **7 of 21 stages have no runtime data at all**: 03, 05, 12, 13, 15, 16, 18.
- `formal`, `exhaustive` and `ulp_admission` are all **NOT_RUN** in this sweep.
  The reason recorded here at the time — a pinned `libttsim.so` host that was
  unavailable — was wrong; see the entry above for what actually ran and what
  cannot.
- Four options outside the measured profile — `-mtt-tensix-optimize-reassoc`,
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
