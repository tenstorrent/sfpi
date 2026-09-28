# Worklog

## 2026-09-28 — close the fresh-machine handoff gap

The earlier handoff explained auditing better than bootstrapping. Added a
[fresh-clone reproduction path](docs/reproduction/README.md) with explicit WORK,
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
