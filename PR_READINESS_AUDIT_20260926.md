# SFPI vector compiler: upstream-readiness audit

Date: 2026-09-26
Coordinate and extraction update: 2026-09-27

The findings below are the September 26 audit snapshot unless explicitly
updated.  The September 27 follow-up verifies repository coordinates, reviews
the intervening `tt-metal` changes, and restores the extracted pass's
correctness rationale.  It does not rerun the compiler suite or hardware
campaign; the 128 regression count and historical measurements remain dated
evidence, not fresh results.

This audit covers the `nkapre/sfpi` branches of `sfpi`, `sfpi-gcc`,
`tt-metal`, and `tt-blaze`.  It distinguishes code that exists, evidence that
has actually been produced, and gates that an upstream pull request can
reproduce.  Historical campaign records are useful evidence, but they are not
substitutes for a clean patch, a current build, or a checked-in test.

## Executive verdict

The project has a real compiler, real semantic A/B vehicles, and real silicon
results.  It is not a collection of placeholder passes.  It has also already
found semantic bugs that source review and disassembly comparison missed.

The September 26 audit found none of the four topic branches suitable for
submission as-is:

- `sfpi-gcc` has been rebased, but still reports 128 newly failing scan tests,
  has never completed the upstream maintainer's Development workflow, and
  contains 490 commits plus changes explicitly excluded from the proposed
  pass series.
- `tt-metal` contains the strongest validation machinery, but its topic branch
  is a roughly 590-commit, 98k-line campaign branch based far behind current
  `main`.
- `tt-blaze` contains the semantic lifts, but has no in-repository executable
  validation for them.  Its evidence currently lives in the `tt-metal` topic
  branch and in external campaign directories.
- The Z3, exhaustive, Galaxy, and ULP systems are individually substantial,
  but do not yet form one source-reproducible CI gate.  Their refusal domains
  are material and must remain visible.

The correct landing strategy is extraction from final source state into small
branches based on current upstream, followed by current CI and device evidence.
Replaying the campaign history, merging a mega-branch, or presenting static
instruction counts as performance evidence is not acceptable.

## Audited coordinates and September 27 refresh

Remote branch tips were checked with `git ls-remote` on September 27.  Counts
are `git rev-list --left-right --count <main>...<topic>` at the explicit tips
below; they count commits, including merges, rather than logical changes.

| Repository | Topic tip | Upstream relation on September 27 | Disposition |
|---|---|---:|---|
| `sfpi-gcc` | `1d02afa9728` | 1 behind, 491 ahead | rebased; tests not rerun here |
| `sfpi` | `f01cf4b` | 445 behind, 603 ahead | pins the rebased compiler |
| `tt-metal` | `d7021a945d3` | 1,741 behind, 599 ahead | intervening changes reviewed below |
| `tt-blaze` | `6b8b94295` | 412 behind, 5 ahead | semantic-source candidate only |

Main refs used for these counts: `sfpi-gcc` `ba48edbef33`, `sfpi`
`8075f72d2b9`, `tt-metal` `d7618ac9981`, `tt-blaze` `f7d9e8265`.
The previous table incorrectly
labelled local `tt-metal:nkapre/sfpi` `28efa259c4f` as the remote tip and
paired it with a count from another ref.  Withdraw that coordinate/count
pair; the refreshed table binds the count to the actual remote topic tip.

Nine commits separate `28efa259c4f` and `d7021a945d3`.  The diff consolidates
the sweep entrypoints into `sweep.sh`, adds the generic Galaxy shard and
setup checks, enables the three-way ULP leg, repairs the shadowed
`HEADLINE_ROWS` default, updates `prove_all` board discovery and configurable
artifact paths, changes pin-mismatch handling, consolidates per-op runners,
and renames `LANE*` identifiers to `SFPU_*`.

Two changes qualify the earlier verification assessment:

- `prove_all` now searches for `craq-sfpi/board/FINAL-BOARD.tsv` and accepts
  environment overrides for board and artifact locations.  It still has a
  fallback to the old board snapshot and fixed expected simulator hashes;
  source-reproducible instrumentation remains an open requirement.
- The configuration pin guard now reports environment overrides and
  continues.  Witness preflight likewise reports a different compiler and
  continues unless `--require-pin` is supplied.  New-compiler exploration is
  therefore possible, but rejection of an unreviewed pin is no longer the
  default.  Promotion must check the actual recorded pin explicitly; the
  old blanket description of these preflights as fail-closed is superseded.

These are source-diff findings.  Enabling or consolidating a runner does not
establish that its broader exhaustive or ULP campaign has completed.

The local `sfpi/gcc` checkout is the pre-rebase `5cc6de0f96d` history and is
not the canonical audit target.  Do not build PRs from that local ref.  The
primary `tt-metal` worktree is on an unrelated dirty matmul branch; do not
switch or rewrite it to perform this extraction.

## Gate 1: compiler-pass pull requests

### Blocking facts

1. The rebased compiler run has zero compile failures but 128 scan failures
   that were not failures before the rebase.  The largest families are
   crosscall, macro-planner, constant residency, LUT selection, and store
   source.  A pass whose own test family is red cannot be submitted.
2. The branch has never run the upstream maintainer's full superproject
   Development workflow (`--gdb --tt-built`, `--dejagnu`, `--test-tt`).  The
   historical short `pin-review-lint` runs do not establish build health.
3. The branch registers 31 new passes.  Twenty-seven are inert by default.
   `lut_select` is default-on, `dst_ownership` always runs and selects
   transform versus analysis internally, and `lreg_livein` and `spill_diag`
   always run.  These four require explicit treatment in the cover letter and
   cannot borrow the risk argument for opt-in passes.
4. Only seven branch-new tests use the maintainer's preferred
   `check-function-bodies` whole-function assembly form.  None belongs to the
   proposed Wave A passes.
5. Shared infrastructure remains large: about 9k lines across 18 files, with
   six translation units registering multiple passes.  Rough direct
   include/API closures are already about 5.2k lines for delivery shape,
   7.3k for store fold, 9.1k for invariant, and 6.5k for Dst auto-increment.
   These are not full transitive closures.  Delivery shape also calls a helper
   in the multi-pass replay-unroll translation unit.  The plan's pass-local
   line counts therefore understate the review unit until infrastructure is
   separated.  A mechanical pass-to-symbol dependency graph is required
   before extracting patches.
6. Several full-branch changes are deliberately outside the pass series:
   removal of an existing default-on combine option, intentional-miscompile
   test knobs, generic `gcc/system.h` additions, `lp_solve` configuration,
   and inherited-pass restructuring.  This makes wholesale cherry-picking
   unsafe.
7. Initial campaign commits are not PR boundaries.  For example, delivery
   shape was introduced across 27 files, while store fold was coupled to int
   not.  Reconstruct patches from final file state against upstream `main`.
8. Authorship policy is unresolved.  A large fraction of campaign commits
   carry model co-author trailers, while the destination repository uses
   neither those trailers nor `Signed-off-by`.  Decide the disclosure and DCO
   policy before opening the first PR; do not rewrite provenance silently.
9. Backend documentation is not authoritative at this tip.  The tree has 56
   RVTT registrations (25 inherited and 31 new), while the backend README
   still reports 54.  It also says a new pass lands off by default despite
   the `lut_select` and `dst_ownership` exceptions, and records that only two
   of eleven proof artifacts are mechanically enforced by the build.

### Compiler landing order

The following order supersedes an immediate four-pass Wave A submission:

1. **PR0: maintainer alignment.**  Ask whether the maintainer wants one pass
   per PR, subsystem series, or a vendor branch.  Agree on authorship and issue
   numbering.  This is a conversation, not a code patch.
2. **PR1: `lreg_livein` correctness fix.**  Extract the final pass and its
   minimal instruction-model dependencies.  Add a whole-function assembly
   test and a regression that demonstrates the silent wrong-code condition.
   Treat always-on status as part of the review, not an incidental default.
3. **Infrastructure series.**  Generate the pass dependency graph and land
   only infrastructure that is needed by the next pass and has independent
   tests.  Do not submit generic GCC changes as backend plumbing.
4. **First optional pass: delivery shape.**  It has broad measured reach and
   is the best opening optimization, but its dependency closure and a
   `check-function-bodies` test must travel with it.
5. **Store fold.**  Separate it from int-not.  Make every proof harness
   runnable through a checked-in target and add whole-function assembly
   coverage.
6. **Invariant.**  Submit only after its semantic contract is complete and
   its post-rebase test family is green.
7. **Dst auto-increment.**  Keep discovery/model and transform portions
   reviewable, and attach current identical-source A/B silicon evidence.
8. **Wave B/C/D.**  Start only after the 128 regression delta is zero and the
   Development workflow is green.

Each optimization PR must demonstrate all of the following:

- clean application to current `tenstorrent/sfpi-gcc:main`;
- flag-off executable `.text` identity where the pass is claimed inert;
- at least one whole-function assembly test, plus transform and refusal tests;
- full target-suite failure-set equality;
- source-reproducible semantic proof or an explicit bounded refusal;
- paired baseline/candidate correctness before timing; and
- no performance claim without drain-inclusive device-cycle measurements.

## Gate 2: LLK semantic equivalence and performance

### What is already credible

The `tt-metal` branch provides paired semantic/handwritten LLK vehicles,
passes-off/passes-on classification, executable `.text` comparison, compiler
and simulator provenance, CRAQ correctness gates, exclusive device locking,
and fresh profiler processes.  Device measurements use the drain-inclusive
`KERNEL` interval.

That machinery caught real errors:

- an SFPSWAP operand inversion returned bottom-8 instead of top-8;
- ENABLE_DEST_INDEX state was incorrectly moved across a store-visible
  window, corrupting packed index/score data; and
- the DeepSeek top32 sequence advanced 24 rather than the required 32 because
  one pass was missing.

The current records also show why issued-word counts are not a performance
oracle: a replay-walk experiment reduced issued words while making measured
end-to-end latency worse.

### Required extraction

1. Land the `tt-blaze` census and pure typed semantic sources separately from
   any production dispatch switch.
2. Extract a test-only `tt-metal` PR containing the vendored A/B vehicles.
   Add a machine-readable source manifest and a bidirectional SHA/drift check
   between `tt-blaze` and the vendored copies.
3. Submit each already-proven semantic fix with the regression vector that
   exposed it.
4. Keep known incomplete arms out of production dispatch: DeepSeek
   `sort_top4`, top16 ordering, partial topk-xl, sinkhorn, batched QK norm, and
   the blocked multi-tile single-face vehicle.
5. Land performance evidence separately as compact machine-readable records
   containing source SHA, compiler SHA, simulator SHA, ELF/text hash, chip,
   exact flags, correctness result, raw cycle samples, and statistic.

The minimum device campaign for a performance claim is same-chip alternating
baseline/candidate execution, correctness first, at least five repetitions,
multiple chips, and t1/t8/t32 where trip count changes compiler behavior.

## Gate 3: formal, exhaustive, Galaxy, and ULP verification

### Z3 scope

`formal_equiv.py` is a real QF_BV translation validator over final emitted
SFPU traces.  It replays each concrete trace snapshot before asking Z3, and
its extracted host self-tests pass with Z3 4.15.4, including 204/204 FMA
vectors.  It is not currently an `sfpi-gcc` CI gate.

The proof scope is deliberately narrower than the corpus:

- the 134-op `prove_all` manifest routes roughly 40 ops to formal equivalence,
  32 to bit-exact checking, and 62 to classification/refusal;
- cross-lane, stochastic, PRNG, and several configuration/opcode modes refuse;
- the Z3 semantics are manually transcribed and validated against observed
  snapshots, not generated from one authoritative ISA model; and
- the default flow pins prebuilt compiler and instrumented-simulator binaries
  whose source/build recipe is not fully in-tree.

Therefore “classified” must never be reported as “proved”, and Blaze
cross-lane LLKs must remain named `NOT-EXHAUSTIBLE` until an appropriate model
exists.

### Exhaustive and Galaxy scope

The 2^32 streaming driver has SHA commitments, identity gates, resumable
bands, and witness bisection.  The Galaxy kit uses same-chip paired execution,
correctness-first gating, and repeated measurements.  Today, however, only
the `sign` operation has a checked-in full 2^32 result; the device overlay is
mostly 2^16.  Integration into `prove_all` is still open.

Before calling this lane complete:

1. check in the instrumented simulator source/patch and reproducible build;
2. make formal, bit-exact, and classification results distinct schema states;
3. add the 2^32 streamer as a resumable `prove_all` backend;
4. store raw Galaxy manifests and hashes in-repository or in an immutable
   artifact store referenced by content hash; and
5. add property-specific reduced-domain suites for cross-lane LLKs: ties,
   permutations, NaNs, infinities, signed zero, subnormals, index association,
   tile-count boundaries, and layout transitions.

### ULP policy

The general ULP framework has already landed its first three upstream
`tt-metal` layers.  Later layers remain stacked topic branches and must be
rebased and submitted sequentially.  Float32 testing is strided rather than
exhaustive, architecture budgets do not transfer, and current tables do not
yet enforce per-operation budgets broadly.

The current exhaustive-workflow branch schedules only Wormhole n150 CI.  It
does not yet provide a Blackhole/p150 or Galaxy ULP matrix, so it cannot be
used as the acceptance gate for Blackhole compiler wins.

For every non-bit-exact optimization record both candidate and handwritten
error against the same high-precision oracle.  Acceptance must be explicit:

```text
candidate_ulp <= hand_ulp
```

or a reviewed degradation license with a numeric bound and a demonstrated
performance benefit.  Partition results by finite normal, subnormal, signed
zero, infinity, NaN, and out-of-domain inputs.  Aggregate percentile metrics
must not hide a worse exceptional class.

The existing 2,048-point accuracy sweep is report-only and must not be cited
as an enforced ULP gate.

ULP/PCC tolerance is for explicitly approximate algorithms or explicitly
licensed approximate-math flags.  A transform that claims to preserve C++ and
SFPI semantics remains bit-exact; ULP tolerance must never be used to excuse
ordinary compiler wrong-code.

## Gate 4: pass/knob selection automation

No property-driven chooser exists today.  `sweep_2x2.py` evaluates a static,
reviewed flag set and manually registered knob legs.  The pass-audit model is
triage only: it has silicon mappings for a minority of passes and deliberately
holds its risk queue because the rubric misclassifies mandatory lowering.

The first production-safe selector should be an offline recommendation engine,
not an online compiler heuristic.  Its unit of evidence is a tuple:

```text
(architecture, LLK semantic class, trip-count band, layout/state properties,
 compiler pin, ordered flag set)
```

For each tuple it may recommend only configurations that pass:

1. flag-off identity or an explicitly reviewed default-on contract;
2. compiler suite and refusal tests;
3. formal/bit-exact/ULP gate appropriate to the operation;
4. CRAQ and physical correctness for every changed binary;
5. paired silicon non-regression with confidence bounds; and
6. interaction testing against the already promoted ordered flag set.

Use monotone promotion states: `candidate -> measured -> licensed -> default`.
Absence of evidence, stale compiler/simulator pins, a changed binary outside
the measured census, or a property outside the licensed domain all force the
baseline configuration.  Never learn directly from static instruction count
or simulator cycles alone.

## Immediate stop/go checklist

- [ ] Maintainer agrees to the PR shape and provenance policy.
- [x] Compiler branch rebased to recent upstream.
- [ ] Rebase regression delta is zero (currently 128).
- [ ] Full Development workflow is green (currently never run).
- [ ] Per-pass dependency closure is generated and reviewed.
- [ ] First PR has maintainer-style whole-function assembly coverage.
- [ ] `tt-blaze` semantic sources have in-tree executable tests.
- [ ] Vendored LLK sources have an enforced SHA/drift manifest.
- [ ] Z3/instrumented-simulator flow builds from checked-in source.
- [ ] Proof, exhaustive equality, classification, and refusal are separate
      machine-readable states.
- [ ] Galaxy raw evidence is content-addressed and durable.
- [ ] ULP acceptance compares candidate to hand and oracle per input class.
- [ ] Selector remains fail-closed and advisory until the promotion matrix is
      complete.

Until the first four unchecked compiler items are closed, do not open an
optimization PR and do not advertise the branch as PR-ready.  Until the LLK
and evidence items are closed, describe performance results as campaign
evidence tied to their recorded pins, not as current upstream guarantees.

## First extracted compiler slice

The first correctness slice was reconstructed during this audit rather than
cherry-picked from the campaign history:

- worktree: `/Users/nkapre/workspace/sfpi-gcc-lreg-livein-pr`
- branch: `audit/lreg-livein-pr`
- base: upstream `main` at `ba48edbef33`
- implementation commit: `0df975b1823` (`Preserve raw LREG live-ins through
  IRA`), superseding the initial `1bcb74da1f6` after restoration of the full
  GPL boilerplate; 10 files, 632 insertions, with a 357-line pass
- documentation follow-up: `bcce3991aaf` (`Explain why raw LREG reservations
  are mandatory`)
- combined closure at `bcce3991aaf`: 10 files, 666 insertions; the 391-line
  pass, five target plumbing
  files, three existing BH/WH regressions, and one new
  `check-function-bodies` regression

The header now explains the raw SFPLOAD-to-L1/vFloat clobber example, the
absence of a diagnostic, the metadata contract, the local sentinel intervals,
and the reason the gate must include every Tensix compilation, including
`-O0`.  It states that sentinel instructions have zero length without
claiming that correcting register allocation leaves the object unchanged.
All bytes from the first `#include` to EOF are identical to `0df975b1823`;
the follow-up changes only the introductory comment.

The two commits are not pushed.  Their combined diff check is clean and the
unrelated case-insensitive `.C`/`.c` checkout collision is excluded from both
commits.  The September 26 configuration succeeded with Homebrew
GMP/MPFR/MPC, but the local macOS build stopped in bundled zlib before
compiling the target: the bundled `zutil.h`
defines `fdopen` as `NULL`, conflicting with Darwin's `_stdio.h` declaration.
The target tests, including the new exact-assembly expectation, therefore
remain unexecuted.  The comment-only follow-up required no new build; it
does not clear that blocker.  This branch is a reviewable candidate, not yet
a PR-ready claim; run it through the Linux superproject Development workflow
next.
