# Worklog

## 2026-10-09 — Exabox boundary validation and expm1 recovery

- Reproduced the FP32 `expm1cw` defect on Blackhole and moved the cutoff from
  the bf16-only 88.5 point to the actual adjacent-FP32 transition:
  `0x42b17217` finite, `0x42b17218` infinity. Production and fresh boundary
  tests pass 2/2; six broader configurations pass 6/6.
- Replaced the initial correct but costly k=128 reconstruction with a k=127
  cap. Three-repeat matched silicon improved hand OFF 315439 → 262193, hand ON
  303279 → 254130, generated OFF 299065 → 245817 and generated ON 268986 →
  203834 cycles (16.2–24.2%). Correctness preceded timing; no anchor was moved.
- Tightened TopK validation without asserting reverted stable-order support:
  explicit and hand outputs remain bit-identical, value multisets and
  index/value association are gated, and the FP16 infinity XFAIL now admits
  only the measured infinity/max-normal swap. Rerun: 82 PASS / 2 XFAIL.
- Wormhole compile-only: six selected production/fresh `expm1cw` cases pass;
  no Wormhole silicon, formal, exhaustive or ULP run was claimed.
- Committed tt-metal `d9d7bdc4906` and pushed `nkapre/sfpi` to both the
  Tenstorrent and nkapreTT mirrors. Evidence is under
  `/data/nkapre/sfpi-boundary-topk-20261009/` (Slurm 129256, 129268, 129269,
  129292 and 129294). The matched four-op baseline keeps I0's absent hand arm
  as N/A and does not admit the semantically stale fast Softplus body.
- Tested, but did not adopt, I0's licensed reassociation mode. Fast-math
  preconditions alone are neutral at 463033 cycles; adding the existing
  reassociation option gives 426169 cycles in three exact repeats (−8.0%).
  Two ordinary correctness nodes pass, followed by nine 65536-pattern FP32
  strata with zero out-of-tolerance results and max 1 bf16 ULP (Slurm 129302,
  129306). The mode is value-changing and has no approved production per-LLK
  rollout, so no defaults, source or compiler rules changed.

## 2026-10-09 — reconcile audit and prepare another-machine takeover

- Preserved the other agent's `19d401fd717` corrections: committed TopK tie,
  two-knob Welford recovery, identified installed-compiler mismatch and the
  reported 8,092/0/2 correctly configured rvtt.exp result. The preceding audit
  described an earlier checkpoint; its stale findings are not current blockers.
- Corrected the chat's loss-count interpretation: two identified historical
  win-to-loss transitions are not an exact new corpus loss count. The broader
  fix round has additional slowdowns; weekly RED includes non-performance
  failures. A matched per-row loss roster remains required.
- Carried forward scoped audit follow-ups: FP32 expm1 premature overflow
  above 88.5, overly broad TopK infinity XFAIL and non-gating stable ordering.
  These require checking against subsequent commits before changing anything.
- Added portable setup/smoke commands, explicit checkpoint refs, evidence
  preservation and performance-recovery priorities to HANDOFF. No new kernel,
  compiler, hardware or proof run in this documentation task; no new worktrees,
  branches or workflow-main writes.

## 2026-10-08 — parallel fix round across kernels, compiler and anchors

- Eight lanes ran on QB0 with one device lock per run and private trees under
  `~/craq-build/lanes/`. Full table and open items:
  [handoff](HANDOFF.md#2026-10-08-parallel-fix-round-read-first).
- Welford: typed 350 → 321 cycles versus hand replay 325 by enabling the
  existing transp-involution and interlock-schedule options; 26/26 PASS.
- TopK: fixed a pack/unpack race that corrupted every width above 128
  (`ae3b0c53`), added 156 exactness cases. Corrected the recorded win: at
  `ccff48c6363` hand and explicit tie at 4918 cycles.
- Kernels: expm1cw overflow band and NaN handling, i0/i1 NaN and ±inf golden,
  `fresh_cpp/expm1cw.h`, sigmoidlut-fresh NaN, `log1p_fitted` domain guard and
  digamma negative-x reflection fixed and pushed; golden now models bf16 NaN →
  ±inf at `dest_acc=Yes`. hardshrink-fresh cleared (dropped dispatch);
  absint32/geluappx-fresh 2^32 records committed.
- Compiler: GCC `ba4b9b13a52` admits structured condition markers in the
  CC-canonical peel proof; pinned in SFPI `95dd2b3`. Its full rvtt.exp is
  8,092 PASS / 0 unexpected / 2 XFAIL in a correctly configured objdir; the
  8,087-with-zaamo-failures figure in the pin commit came from an objdir
  without an assembler.
- Toolchain identity: the installed QB0 compiler was `b6b32a51d1a`, not
  `064ef4565ea`; no conclusion depended on it. Rebuilt and installed at
  `95dd2b3`, old install kept alongside.
- Owner actions: 30 reviewed R7 exceptions; anchors re-booked at pin 59 for
  rows whose body changed. Full weekly afterwards: 39 RED (17 never anchored,
  18 compile/correctness failures at pin 59, 2 missing `issue_slot_lb`,
  2 stale anchors). Several correctness fixes carry large booked slowdowns
  (i0, softplus, relu, sigmoidappx-tree); they are open, not accepted.

## 2026-10-08 — extend explicit-state validation beyond TopK

- tt-metal `0616c8eb300` exposed the tested TopK merge as an opt-in production
  entry using public `sfpi::l_reg`; existing defaults remain unchanged.
  Production-entry runs passed 72 exact cases each at O2/pass-disabled,
  O3-scheduled/pass-disabled and O3-scheduled/pass-enabled. With the fixed
  launch-flatten option, five-run timing was hand 5038 / explicit 4929 cycles.
- tt-metal `ccff48c6363` moved the existing typed EMA tile body into shared
  `common/ckernel_sfpu_ema_explicit.h`, with corpus compatibility forwarding
  rather than a duplicate body. Caller-owned `EmaState` carries state across
  calls. Contract 1 is tested; contract 2 is a different ordering, not admitted
  as a bit-exact replacement. No default was switched.
- Fresh Blackhole EMA module: 18 PASS, including exact hand/typed output
  comparisons for 1/2/4/32 tiles. Seed 0, finite BF16 [-4,4], alpha=.25,
  beta=.75. Five-run body timing: hand/typed 335/329 cycles for one tile;
  212.21875/209.125 cycles per tile across 32 tiles. Initialization is untimed.
- Fresh Welford module: 26 PASS, including exact captured BF16 mean/M2
  comparisons at prefixes 1/2/4/8/16/32, plus independent tolerance checks.
  Seed 20260814, finite BF16 [-4,4]. This is not internal FP32 equivalence.
  Five-run timing: handwritten replay 325 / typed-direct 350 cycles (+7.69%).
  Hand-direct is 464.8 but is not the appropriate winning baseline. The typed
  candidate was NOT promoted.
- Both modules used compiler `064ef4565ea`, O3 with scheduling, launch-flatten
  on, and the raw-LREG live-in pass disabled. Fixed combined-run profiler row
  selection and EMA parameter-schema mismatch; an initial profiling attempt
  failed in the harness, and reported numbers come from successful reruns.
- Added a source-only raw-SFPU inventory: 39 headers, 2004 sites in three
  architecture SFPU trees. The preliminary 44-header grep included comments.
  This is not instantiated coverage or a transitive all-helper audit.
- Committed and pushed `ccff48c6363` to both tt-metal `nkapre/sfpi` mirrors;
  no new branch/worktree. Documentation and evidence locations are in the
  [current handoff](HANDOFF.md#2026-10-08-explicit-state-rollout). No hardware
  job from these runs was left running. Remaining regions and non-Blackhole
  validation are unfinished; no new formal/exhaustive/ULP run was performed.

## 2026-10-08 — fix explicit-LREG TopK unroll regression

- GCC `064ef4565ea`: existing opt-in launch-flatten now admits raw delivery
  with explicit LREG read/write lifetimes, retaining its structural and size
  bounds. No new pass, global unroll-limit change, or kernel-specific rule.
- Fresh Linux baseline/patched builds. Focused tests: baseline 50 PASS / 2
  FAIL in the new positive regression; patched 52 PASS / 0 FAIL. Full patched
  `rvtt.exp`: 8,085 PASS, 2 expected XFAIL, no unexpected failures; 1,537 SFPI
  checks actually ran.
- Fresh Blackhole public-LRegFile TopK: same flags on both arms, O3 scheduled,
  live-in pass disabled. Baseline/on and patched/off: hand 5038/threaded 5186;
  patched/on: hand 5038/threaded 4929 cycles, five runs/arm. No global unroll
  threshold override. All 72 BF16/FP16 exact value/index cases pass in each
  configuration; patched O2/on additionally passes all 72.
- The option is `-mtt-tensix-optimize-launch-flatten`; default remains off.
  The region remains a test adaptation, not a wholesale production rewrite.
  See tt-metal `tests/corpus/RAW_LREG_EXPERIMENT.md` for scope and commands.

## 2026-10-04 — complete and archive the all-LLK knob search

- Completed the current-compiler bounded search across all 263 runnable LLKs:
  2,159 measured configurations, 77 candidate selections over the frozen
  same-source baseline, 157 baseline retentions, 28 rows with no eligible
  ordinary candidate, and one quarantined row.
- The run stopped twice on the same `gcd-fresh` pair (`dst-autoincr` disabled,
  `replay-loop-unroll` enabled), wedging two different Blackholes. That pair is
  recorded as `QUARANTINED_DEVICE_TIMEOUT`, not retried on a third device and
  not eligible for selection. A separate `sigmoidlut-fresh` pair failed
  compilation; the row completed around it and selected a passing candidate.
- Continued only missing rows on devices 1 and 2, preserving all three segment
  records. Added a deterministic merger that rejects incompatible settings,
  duplicate completed rows, coverage gaps and unnamed stopped rows. Added the
  77-row selected roster to the matrix renderer. Workflow tests: 75 passed and
  11 subtests passed.
- Committed the complete merged `search.json`, 263 × 91 wide/long matrices,
  selected roster, census completion record and human audit at workflow commit
  [`be5e52d`](https://github.com/tenstorrent/craq-sfpi/tree/be5e52da1d283cc6cd323f5584e1898a0501fb9d/board/evidence/llk-knob-search-20261004).
  The bounded search did not run formal, exhaustive, ULP or deployment gates.

## 2026-10-02 — current-compiler all-LLK knob campaign started (historical checkpoint)

- Rebuilt and verified the SFPI/tt-metal harness on `tt-quietbox-0.local`
  under one existing `~/craq-build` tree. Source pins: SFPI `9f89f13`,
  sfpi-gcc `80153da1594`, tt-metal `1edfd7f8ede`.
- A one-LLK current-compiler pilot (`absint32`) found an interaction:
  `int-abs` alone −18.77% versus the frozen compiler baseline; paired with
  `replay-loop-unroll` disabled, −37.52% (21,815 → 13,630 cycles, three
  identical samples). That remains 0.32% slower than handwritten. Existing
  bounded correctness passed; formal/exhaustive/ULP were NOT_RUN.
- Completed the full compile-only 263 × 91 census: 23,933/23,933 verdicts,
  1,278 changed, 22,648 identical, seven explicit `reassoc` compile refusals.
  It is complete with no missing or invalid verdicts. These are firing
  observations, not performance claims.
- Started the full correctness-gated silicon search in persistent tmux. At
  05:59 EDT, 13/263 LLKs and 120 candidates had finished with zero candidate
  failures. Three LLKs selected improvements versus the frozen baseline:
  `abs` −19.33%, `absint32` −37.52%, `acosh-fitted` −16.48%. The remaining
  completed LLKs retained baseline; no corpus-wide winner count is available.
- Added a deterministic wide grid and per-cell TSV renderer in workflow commit
  `acd3546`; refreshed the live matrix. Fixed an unrelated pytest collection
  mistake in `720d922`; workflow script suite was 73 passed, 11 subtests.
  The jobs, evidence, exact status/recovery commands, and limits are in the
  [handoff](HANDOFF.md#2026-10-02-live-search-handoff). No Git worktree or branch
  clutter was created.

## 2026-09-30 — re-grade the stratified ledger after the oracle itself was corrected

Six modelling defects were found in the verification oracle, not in the kernels
(`oracle-modelling-fixes-20260930`). That re-graded the exhaustive bf16 board —
40 of 192 (op, leg) verdicts, 21 ops — but left the **stratified** ledger, whose
"20 defective strata across 10 op rows, 17 of them with both arms out of
contract" was still being published as current fact in three documents. It is
now re-graded in full.

- **The count, with its scope, because a bare count is what went stale.**
  **18 defective strata over 9 op rows (7 base kernels), 15 of them with both
  arms out of contract.** That is a count of `SEM-BUG` + `HAND-BUG` *cells* over
  the 369 cells of `ulp-strata-corpus-20260930` (41 ops x 9 exponent strata,
  one bf16 operand per stratum), graded by the leg's own `classify-board.py`,
  unmodified, against `threeway_golden.py` at tt-metal `nkapre/sfpi`
  **`2107a950747`**, on the device bytes the original leg measured. Composition
  302 CLEAN / 16 SEM-BUG / 2 HAND-BUG / 11 OUT-OF-DOMAIN / 2 OUT-OF-CLAIM /
  36 NO-GOLDEN, against 295 / 18 / 2 / 16 / 2 / 36 before. The old figure is
  **superseded, not retracted**: it is the same cells at the pre-fix oracle.
- **The re-grade is almost entirely analytic, and that is a property of the
  change, not a shortcut.** The oracle change is deterministic and each stratum
  is one bf16 operand, so recomputing all 369 goldens settles which cells can
  move: **365 goldens are bit-identical**, 4 move (all `lgamma`), and the one
  comparator change reaches 9 more. **13 cells reached, 7 verdicts changed, 356
  unchanged by construction.** Four fixes reach nothing here — input FTZ needs a
  subnormal operand, the SFPSWAP order and `SFPABS` changes need a NaN, and
  `gelu(-inf)` needs `-inf`; no stratum is any of those.
- **Withdrawn (2).** `lgamma` S3 and S7. The LLK node is
  `calculate_lgamma_stirling`, which returns the documented intermediate
  `lgamma(z)`, `z = (x < 0.5) ? 1-x : x`; the golden was grading a stage node
  against the composite's semantics. Re-measured on silicon at the corrected
  oracle: max ULP **2** at S3 and **bit-exact** at S7, 0/65536 out. `lgamma` is
  now clean on all nine strata and leaves the defect list.
- **Newly exposed (0) — and the reason is specific, not an omission.** The
  correction's other direction is real: it deleted the `ORACLE fp32 OVERFLOW`
  escape class, which scored two infinities as 0 ULP and so passed an
  overflowing `i1` while failing the correct one. It moves no cell *here*
  because this leg's golden already evaluated in fp64 — which is exactly why
  `i0` S5 at `x = -89`, inside that band, was **already** booked SEM-BUG, and
  `i1`/`i1-fresh` S5 already booked OUT-OF-CLAIM. The defect was hidden on the
  harness's own `passed_test` reference and on the exhaustive path, never on
  this one. On the exhaustive board all 40 changed verdicts also move downward.
  Say that, rather than manufacture a symmetric table.
- **What the ledger does gain in the strict direction is coverage.** Removing
  `lgamma` from `GAMMA_POLE_OPS` makes the non-positive integers graded where
  they were excluded as poles. Two strata sit there — S0 at `0.0` and S5 at
  `-89.0` — and both **pass** (`314.0` bit-exact at S5). Five cells move from a
  licensed category to in-contract: those two plus `erfinv` S5/S6/S7, where the
  golden's infinity is a packer conversion of a NaN and the device returns the
  same conversion with the other sign bit. No golden was weakened: D2 moves the
  golden **toward the hardware** because the packer's sign behaviour is what it
  should model, and it still refuses a computed overflow — `expm1cw` S4
  returning `-inf` where `+inf` is correct remains a defect on the same board.
- **Analytic vs re-measured vs ungraded: 367 / 2 / 0.** The two re-measured are
  `lgamma` S3 and S7 (full 65536-pattern bands, chip 0). `lgamma` S0 and S5 are
  re-graded from the band's recorded `.corr` witness; the re-measured siblings
  bound the band spread at 2 bf16 ULP, which at S5 is `314.0 +- ~4` against a
  tolerance of 15.75, so a re-measure would confirm rather than decide. No
  silicon was run for this re-grade: tt-quietbox-0 carries no tt-metal checkout
  or build, and another lane held the box with two toolchain builds. Recorded as
  a choice.
- **A correction to the correction.** `ulp-strata-fixes-20260930/RESULTS.tsv`
  has **four** `lgamma` `STILL-OUT` rows, not two — S0 (83), S3 (86), S5 (88),
  S7 (90). All four are in contract at the corrected oracle, so `STILL-OUT`
  there is 12 -> 8. The oracle-fix notes named only 86 and 90; the pole
  exclusion hid the other two.
- **`hardshrink-fresh` reconciles, and the corrected oracle is not what settles
  it.** `x = -0.9375` is a normal, non-NaN, non-integer operand through a
  compare-and-passthrough body, so no fix reaches it and the S3 golden is
  bit-identical before and after. The withdrawal stands on its own evidence: the
  band hashes to 262144 bytes of the `0xA5` clear sentinel, so the dispatch
  wrote nothing and 6602 ULP is `0xA5A5` read as data; eight stress repeats in
  the same ELF are 0 ULP; the galaxy sweep is `BIT-EXACT-ALL-INPUTS` at
  `covered=4294967296`. Independently, the production `hardshrink` arm is CLEAN
  on all nine strata at the corrected oracle. **Not a kernel defect; do not
  re-count it.**
- **The oracle was not touched**, so its 193-check selftest is unchanged. No
  parallel grader was written: the re-grade recomputes goldens and compares with
  the oracle's own `numeric_comparison`, and attributes with the leg's own
  `classify-board.py`. Record:
  [ulp-strata-regrade-20260930](https://github.com/tenstorrent/craq-sfpi/blob/main/board/evidence/ulp-strata-regrade-20260930/NOTES.md)
  (`RESULTS.tsv` every changed verdict, `COUNTS.tsv` both revisions).

## 2026-09-29 — finish all three legs, and fix what they found

The formal instrument was rebuilt, the exhaustive leg ran over the whole 2^32,
the stratified ULP leg went from 31 ops to 72, and one of the defects it found
turned out to be in **production**, not in the compiled kernel. Several
statements from earlier today are retracted below.

- **Formal: reconstructed, and it runs.** The instrument was never in any
  commit — the pinned simulator was an uncommitted patch on top of one, which
  `prove_all_domain_overlay.tsv` records in its own header
  (`# sim ba23c3f16912 = craq-sim 1c47e9cd + JO value-observation patch`). The
  `SFPUJO I/V/M/C` hook was rewritten from that description onto pinned
  craq-sim `1c47e9cd` as branch `agent/laneJO-sfpu-trace-stream`, tip
  `6de51ce0`. **8 of 8 rows VALIDATED, zero contradictions** — 6
  `PROVEN-EQUIV-ALL-INPUTS`, 2 `DIVERGENT` (`clamp-fresh`, `mulint32-fresh`).
  `clamp-fresh`'s z3 witness independently rediscovered the region the domain
  overlay had already recorded: bf16 `0x8000` upward, exactly **128 of 2^16**
  inputs. State the boundary precisely — **the verdicts are re-derivable from
  committed source, not reproduced from the recorded pin.** `prove_all.py` still
  expects `ba23c3f1`, that binary is gone, and nothing will hash to it again, so
  the provenance gate has to be re-pinned rather than satisfied. Only 7 of the 8
  rows have an overlay row to agree with; `where` has none and is merely
  consistent.
- **Retract "the trace hooks were removed over time."** They were never on
  `main`. `TTSIM_TRACE_LOADMACRO` was introduced by exactly one commit,
  `6c072b57`, on the `nkapre/sfpi` line. The fresh build "losing" it was the
  fresh build never having had it. Do not re-run that `git log -S` in a
  main-only clone expecting confirmation: it returns zero hits either way.
- **Full 2^32 ran on a galaxy, and yesterday's infeasibility claim was a bad
  measurement.** On `bh-glx-120-b04u08`, 32 chips, `NPAR=32`, `BAND_BITS=23`:
  **9 ops `BIT-EXACT-ALL-INPUTS` at `covered=4294967296`, 5 `DIVERGENT`, 2
  undecided** on dead slices. Diverging slices out of 32: `erf-fresh` 32,
  `erfinv-fresh` 31, `erfc-fresh` 27, `sigmoidlut-fresh` 18, `rpow` 2 — and
  **`rpow` is now bounded** to `[0x70000000, 0x80000000)` and nowhere else in
  2^32. Coverage arithmetic checked end to end at NPAR=4 and NPAR=32, with 8
  device-free selftests. The 65.5k patterns/s figure was taken at a 2^18 band
  where per-band process overhead dominates; at 2^23 it is **~740k/s/leg/chip**,
  so ~1.6 h for one op on one chip and ~5-9 min observed on a galaxy. **The
  throughput measurement, not the machine, was the error** — both figures are
  Blackhole silicon. Two caveats to carry: `e75553f1c3f` changed the `erf`,
  `erfc` and sigmoid-LUT sem headers mid-campaign, so those three verdicts are a
  pre-fix baseline rather than a test of the fix; and **`hardshrink-fresh` must
  not be reported as cleared** over the full space, because the stratified leg
  called it SEM-BUG inside slice 23 the day before and that is unresolved.
- **Three more machinery defects, all fixed on tt-metal `nkapre/sfpi`.**
  (1) `7b8f32371c7`: the earlier self-certification fix reached the stream
  sweepers but **not `galaxy_combine.py`**, which compared coverage to the run's
  own `SPACE` — a 32-slice 2^18 shard printed
  `BIT-EXACT-ALL-INPUTS covered=262144`. `SPACE` and `FULL_SPACE` are now
  separate. (2) `f7440593391`: **the galaxy driver could not report `DIVERGENT`
  at all.** Streamers exit 1 on divergence and the driver read any non-zero exit
  as a dead chip, so the combiner's divergence branch was unreachable; it now
  reads each slice's own verdict file. (3) `52fa68b6488`: a TOCTOU on
  `pytest_errors.log` in `conftest.py` killed 3 of 32 slices (73 of 400 trials
  before, 0 of 400 after). It is pushed, but **deliberately not applied to the
  in-flight staged farm** — `conftest.py` sits under the directory hashed into
  `farm_python_sha256`, so editing it invalidates every band cache and destroys
  a running campaign's resume state.
- **Stratified ULP over 41 more ops: 9 strata, 369 cells, 20 defective strata
  across 10 op rows.** [**SUPERSEDED 2026-09-30 — the count is now 18 across 9,
  15 both-arms, and the `lgamma` S3/S7 rows below are withdrawn; see the
  2026-09-30 entry.** Everything else in this bullet stands.] The headline is not
  the count. **17 of the 20 have BOTH
  legs out of contract**, so 85% of these defects would read as *agreement*
  under a sem-versus-hand equivalence sweep; only `digamma-fresh` splits (S3/S7
  hand-only, S4 sem-only). Examples: `softsign` S4/S6 returns exactly `0.0` for
  a function bounded in (-1, 1) — the reciprocal `3.0335e-39` flushes below the
  16-bit DEST's `2^-126`; `lgamma` S3/S7 sign flip; `i1` returns one constant
  `1.15477e37` at three unrelated inputs; `i0` S5 1.55e16x low; `sqrtcustom` S4
  2.159x; plus `expm1cw`, `xielu`, `digamma`. **24 ops are clean across all nine
  strata**, and `erf`/`erfc` PRODUCTION are among them — that defect was
  `fresh_cpp`-only. Four strata were added because the inherited five held
  exactly one negative value (`-0.9375`) and no large negative at all; S6 is
  `-3.1901e38`, and three of the six defects above live in the new strata.
  Coverage is **71 of 259 ops (27.4%)**; of the 188 uncovered, **173** are
  blocked on harness work (75 `Float16_b` rows refused by
  `StimuliConfig.write`, 98 with no `SFPU_STREAM` hook) and the remaining 15
  binary rows need a two-operand stratification *design*, since a stratum is a
  one-operand partition. The 16-bit band mode is named as future work and has
  not been started.
- **Correction to the "band choice" explanation.** The standing `[0, 2^18)`
  probe covered the **same** 31 ops, not different ones; the other **228 had no
  band probe at all**. And the 31-op limit was never device time — it was
  `threeway_golden.REGISTRY`, which held exactly 31 entries, and `--golden <op>`
  is inert without a spec. The registry now holds 72.
- **A static boundary audit of every LLK: 31 ranked defects**, in three classes
  (exponent-field writes that wrap, polynomials evaluated outside their fit
  domain, unbounded final segments on analytically bounded functions). 12 defect
  bodies over 13 of the 21 `*_fitted.h` files. The structural finding is a
  **second overflow idiom**, `as<vFloat>((i << 23) + as<vInt>(w))`, in 15
  production files across three arches and 7 `fresh_cpp` files, 5 of them
  invisible to a `setexp|addexp` grep — and worse than a wrapped exponent,
  because a shift whose field reaches 256 sets **bit 31**, the sign. Measured
  instance: `quasar`'s `ckernel_sfpu_exp.h` gives `exp(90) = -1.175e-38`. Two
  cautions: the audit's `sigmoid(-89)` NaN **did not reproduce** at
  `dest_acc=No` — the 16-bit DEST flushes exactly as the golden does, so that
  case needs a `dest_acc=Yes` row rather than a stratum — and the audit's
  "9 cleared" figure is not reproducible from any tally in its own TSVs. Use the
  TSVs.
- **A production `pow` fix, tt-metal `f24d9515da4`.**
  `_sfpu_binary_power_21f_` and `_sfpu_unary_power_21f_` clamped only the low
  side of the exponent, in **both arches** (5 files, 4 of them the blackhole and
  wormhole_b0 copies of two headers). The threshold `z_f32 >= 128` was derived
  independently and then found to match the already-correct fp32 siblings.
  **Clamping alone is insufficient**: at the ceiling `setexp` writes exponent
  field 255 over the non-zero mantissa the Horner step leaves (`zif == 14447`),
  which is a NaN — so the fix clamps *and* substitutes `+inf`, verified at bit
  level as `0x7F800000`. `rpow` S4 went from 65535 ULP with 65536/65536 out of
  contract, returning a constant `1.0` where the answer is `+inf`, to **0 ULP,
  0 out of contract, bit-exact with sem**. **Two owner actions are outstanding
  and were not self-approved:** six booked anchors move by a constant +40.00
  (OFF) / +36.00 (ON) DIAGNOSTIC units, and two cross the 10% tripwire —
  `unarypower:sem_off` and `unarypower-fresh:hand_off` at **+10.409%**, with
  `rpow:hand_off` at +9.985%, 0.015 pp under — so they need re-booking at the
  conf-pinned cc1plus before the next weekly; and `conf_lint` R7 (LLK-pristine)
  needs a reviewed `_REVIEWED_LLK_API_EXCEPTIONS` entry for the four pow
  headers.
- **Sem sometimes beats production, and that is a deliverable.**
  `threeway_golden` grades each arm independently against a host-computed
  golden — the accumulator is one per leg and the other arm's output never
  enters it — so three outcomes are separable: `SEM-TOLERANCE-FAIL` (compiled
  kernel out of contract), `SEM-TOLERANCE-PASS(hand fails tolerance)`
  (production out of contract, e.g. `rpow` S4), and `TOLERANCE-BOTH-PASS` with
  `ulp_admission.candidate_not_worse` (both in contract, sem closer, e.g.
  `erfc-fresh` at 0 ULP against production's in-contract 11343). Keep the last
  two distinct: an in-contract production arm that sem beats is an accuracy
  win, not a production defect.
- **The honest perf denominator question is still open.** The idea that wins
  should be counted only over rows whose two arms compile differently is sound,
  but **this sweep commits no per-row `.text` identity fact** — the manifest
  rows carry no ELF hash and the 1.6 GB `rows/` tree is not committed — so that
  denominator cannot be reconstructed. The nearest measured proxy is the 20 ops
  with no firing knob in either attribution set: **0 wins / 20 flat / 0
  regressions** at ±0.5%, 19 of them at exactly 0.0000% and `softmaxk` at
  0.458%. Quote **192 / 69 / 2 over 263 rows**, or 188 / 69 / 2 over this
  file's 259; and quote the hand comparison at one denominator (164 rows:
  79/3/82 raw, 71/25/68 at ±0.5%), never a raw count from one against a banded
  count from another.
- **Raw run trees are no longer committed.** Ten tarballs totalling 14.7 MB had
  landed; removing them took the results tree from ~14.9 MB to 284 KB.
  `results/RAW-RUNS.md` records each group's node-local path and its
  `CRAQ_GROUPS_ONLY=<group>` regeneration command instead. No checksum
  manifests, no guard files, no provenance hashes — a path and a command.
- **`gen-flag-defaults.py` is currently inert.** It reads its reviewed-ON set
  from `craq-sfpi/dashboard/sweep_2x2.py`, which does not exist, and a missing
  harness returns an empty set — so it prints `reviewedON=0
  promotion-backlog=0` and reports no gap, which is the exact failure its
  docstring says it exists to catch. Repointing it at a real source (a sweep
  manifest's `flags.ON_FLAGS`, or tt-metal's `tests/corpus/sweep_2x2.py`) is a
  ~40-line change, verified to run: all three real sources agree at
  `Init(1)=6 reviewedON=38 promotion-backlog=35 optin=60`. That also makes
  `docs/TENSIX-FLAG-DEFAULTS.md` one out — its "reviewed ON 39 / backlog 36"
  counts the ON arm's 39 *tokens* (38 positive plus one `-mno-`), not 38 flags.
  The change is not landed here because the file lives in the `gcc` submodule
  whose HEAD **is** the validated compiler pin `566071bf728`, and committing
  there would move that pin as a side effect of a documentation change.

Evidence: [reconstructed formal instrument](https://github.com/tenstorrent/craq-sfpi/blob/main/board/evidence/formal-jo-instrument-recovered-20260929/NOTES.md),
[2^32 on a galaxy](https://github.com/tenstorrent/craq-sfpi/blob/main/board/evidence/exhaustive-2p32-galaxy-20260930/RESULTS.md),
[stratified ULP corpus](https://github.com/tenstorrent/craq-sfpi/blob/main/board/evidence/ulp-strata-corpus-20260930/NOTES.md),
[ULP strata fixes](https://github.com/tenstorrent/craq-sfpi/blob/main/board/evidence/ulp-strata-fixes-20260929/NOTES.md),
[SFPU boundary audit](https://github.com/tenstorrent/craq-sfpi/blob/main/board/audit/sfpu-boundary-audit-20260929/README.md),
[pow overflow clamp](https://github.com/tenstorrent/craq-sfpi/blob/main/board/evidence/binpow-overflow-clamp-20260929/NOTES.md),
[raw-run policy](https://github.com/tenstorrent/craq-sfpi/blob/main/board/evidence/incremental-knobs-20260929/results/RAW-RUNS.md).

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
  **Wrong, and corrected in the entry above**: that rate was measured at a 2^18
  band where per-band overhead dominates, and 2^32 subsequently ran on a
  galaxy. The "a probe is not a substitute" half stands.
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
  **Superseded by the entry above**: the instrument was reconstructed onto
  pinned craq-sim `1c47e9cd` and the leg now runs 8/8 VALIDATED. The narrower
  claim survives — the verdicts are re-derivable from committed source, not
  reproduced from the recorded `ba23c3f1` pin, which stays unrecoverable. The
  "further away, having lost `TTSIM_TRACE_LOADMACRO`" reasoning was wrong: that
  string was never on `main` at all.
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
