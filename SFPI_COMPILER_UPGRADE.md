# SFPI Compiler Upgrade: Scheduling, Allocation, Replay, and LLK Lowering

Welford is the motivating zero-slack fixture, not a pattern recognized or
hard-coded by the compiler.  The implementation extracts any eligible SFPU
SSA dataflow graph, runs the same list or MILP machinery, and validates the
result without knowing which source-level algorithm produced it.

“Pressure scheduler” is the umbrella term: the list heuristic and optional
MILP are two engines for choosing a dependency-respecting issue order whose
maximum live SFPU value count fits the eight LREGs.  GCC already has generic
RTL scheduling and IRA/LRA allocation, but ordinary GCC assumes spilling is a
valid escape hatch.  Before this work, SFPI had no target pass that extracted
the vFloat SSA graph and scheduled it against a non-spillable eight-register
capacity; its target-specific postreload pass could only insert hazard NOPs
after allocation had already succeeded or failed.

The underlying risk is a zero-slack register-allocation problem. Vanilla Welford can fit in eight LREGs, but only if several destructive coalesces succeed perfectly. SFPU values cannot spill, and the existing “scheduler” runs after allocation and only inserts NOPs. A current SFPI 7.69 compiler does successfully coalesce the minimal four-row recurrence, so the historical failure still needs to be pinned to its exact compiler revision or surrounding eltwise state before it can be called reproduced.

## Revised decision after the second agent storm

**Go**, but not as a global LP rewrite and not with the first formulation below left uncorrected. The safe project is a staged, opt-in pressure scheduler:

1. Build an exact DFG/pressure oracle and emit diagnostics only.
2. Add a deterministic pressure-first list scheduler.
3. Use MILP only for small, positively allowlisted regions where the heuristic cannot find a legal eight-register schedule or where there is a measured latency opportunity.
4. Accept a solver result only when an independent checker proves its schedule, live ranges and materializable destructive ties.
5. Keep the pass off by default until compiler, TT-Metal and hardware differential testing is clean.

The opportunity extends well beyond Welford. TT-Metal already contains explicit reloads, recomputations and hand-interleaved instruction streams that work around the same eight-register and RAW-latency constraints. However, `_lv` is predication semantics as well as an RTL register tie, and a GIMPLE-selected physical coloring is not automatically an allocation guarantee. Those two facts define the hard safety boundary.

## Execution log: measured baseline

The first executable fixture loads L0–L3 as four inputs, L4 as mean, L5 as M2 and L7 as reciprocal, then applies the source-shaped recurrence four times. It was compiled in an x86_64 Linux VM with the released SFPI 7.69.0 GCC 15.1.0 toolchain.

- Wormhole (`-mcpu=tt-wh-tensix -O2`) compiles successfully with all eight LREGs occupied and no spill/fill. Each row lowers to four SFPMADs separated by three hazard NOPs, with an additional final hazard NOP before the state write.
- Blackhole (`-mcpu=tt-bh-tensix -O2`) also compiles successfully with no spill/fill. Each row lowers to two SFPADDs and two SFPMADs with no inserted NOPs in this compiler revision.
- The late GIMPLE graph contains 16 arithmetic operations. The boundary has seven live vector inputs and the measured source-order peak is eight.
- The handwritten LLK's recomputation/MOV schedule is therefore not automatically faster than current compiler output. On Wormhole the natural compiler sequence is seven issued slots per non-final row versus eight for the handwritten sequence; hardware/replay measurements are still required before making a performance claim.

This result does not remove the compiler hazard: the allocation has zero slack, spills remain fatal, and extra live eltwise state can still make the graph impossible. It changes the immediate milestone from “make this minimal source compile” to “identify the exact failing context, build a trustworthy pressure oracle, and only transform graphs where final assembly or device measurements prove a win.”

## Overnight prototype result

The checking-enabled x86_64 stage-1 compiler executed the opt-in prototype on the real late-fold fixture on both requested architectures. The measured dump was identical on Wormhole and Blackhole:

```text
SFPU pressure region: bb=2 ops=17 live-in=8 peak=9
SFPU pressure schedule: old-peak=9 new-peak=8 applied=yes
```

With the flag off, both compilers reached reload and failed with the expected `cannot store sfpu register (register spill)` ICE. With the flag on, both emitted assembly successfully. That assembly was byte-for-byte identical to compiling the fixture with `WELFORD_MANUAL_EARLY_FOLD`, so the automated rescue recovered the source-level golden schedule rather than merely finding a different allocation by accident.

The minimal zero-slack recurrence remained unchanged: pass-off and pass-on assembly compared equal on both architectures, and its dump stayed at `ops=16 live-in=7 peak=8`. A predicated fixture was rejected as `cc-epoch` and remained byte-identical pass-off/on on both architectures. The live-across oracle fixture reported `ops=2 live-in=4 peak=4`, confirming that an untouched value spanning a hard region boundary is counted.

These are compiler feasibility and code-generation results, not a hardware latency claim. The final installed stage-2 compiler, focused DejaGNU run, and simulator smoke results are recorded in the morning report below.

## Morning report: 2026-08-14

The overnight prototype completed its Linux compiler and focused simulator gates on both requested architectures.

- A checking-enabled GCC 15.1.0/SFPI 7.69.0 toolchain built and installed successfully in the x86_64 Lima VM. The roughly six-hour build produced the base toolchain plus Wormhole ILP32, Blackhole ILP32, QSR32 ILP32 and QSR64 LP64 runtime variants. GCC's build-time C and C++ selftests reported 8,449,969 and 8,449,993 checks respectively.
- The prototype's compatibility spelling `-mtt-tensix-optimize-lp-schedule` is reported as `[disabled]` by the installed compiler unless explicitly enabled. The canonical spelling is now `-mtt-tensix-optimize-pressure-schedule`; both remain off by default.
- The focused validator produced 24 assembly files. On Wormhole and Blackhole, the late-fold fixture fails with the expected SFPU spill when the pass is off, while pass-on certifies `old-peak=9 new-peak=8 applied=yes` and emits exactly the same assembly as the manual early-fold control.
- The minimal, predicated and CFG rejection fixtures remain byte-identical pass-off/on. The live-across fixtures report `ops=2 live-in=4 peak=4`. Three repeated rescue compilations per architecture are deterministic: Wormhole SHA-256 `3cee70a65b29062489d790bcc30b23cc43ffe0741cc634e5106be8b0b14d8344`; Blackhole SHA-256 `502bff4f216e3ed5d90ba891cb6a0c6a636e765af6ad53ab1c94d04f0b0f3ed1`.
- The real TT-target DejaGNU run reports 1,070 expected passes, zero unexpected `FAIL`, zero `UNRESOLVED`, and zero `ERROR`. Two pre-existing expected failures remain expected. An initial invocation had falsely printed success because `runtest` was absent; installing `dejagnu` and rerunning the real suite exposed and eliminated that infrastructure false-green.
- The recurrent-SFPU EMA craq-sim smoke passes all three cases with the scheduler off and on: Wormhole `3 passed in 32.63s` off and `3 passed in 31.14s` on; Blackhole `3 passed in 31.74s` off and `3 passed in 30.55s` on. These are correctness smoke timings, not device-cycle measurements, and the test is handwritten EMA rather than a transformed eltwise Welford kernel.
- The pinned Blackhole simulator needed a VM-local compatibility patch before the LLK harness could run. It reads back retained TRISC/NCRISC reset-PC state for harness introspection and retains valid generated `THREAD_CFG23`/`THREAD_CFG25` `SETC16` writes. Hardware documents the reset-PC registers as write-only; this is a simulator accommodation, stored as `scripts/craq-sim-bh-llk-smoke.patch`, not an SFPI compiler semantic change.

That overnight result was a strong compiler-feasibility checkpoint, not a performance sign-off. At that checkpoint the pass contained an exact pressure oracle and deterministic list scheduler but did **not** yet link or invoke `lp_solve`. Full `--test-gcc`, a transformed eltwise Welford numerical test, device execution, hardware cycle comparison against handwritten LLK, and the wider TT-Metal kernel corpus remain required before enabling it by default or claiming a latency win.

## Autonomous M0 hardening and optional MILP implementation

The next branch iteration closes the prototype's immediate legality gaps and adds a deliberately narrow `lp_solve` research path. This does not change the production/default-on decision: both transforms remain off unless explicitly requested, and physical allocation is still not enforced.

- `schedule_solution` and an independent `validate_schedule` now run before any GIMPLE move. The checker reconstructs the exact operation permutation, vector and scalar source availability, every SSA def-before-use edge, duplicate operands, live-in/live-out/live-through values, old and new peaks, the eight-register limit and strict profitability. A failed certificate never mutates GIMPLE.
- Every accepted dependent schedule is immediately cloned into deliberately
  invalid duplicate-operation, use-before-definition and false-peak
  certificates.  The validator must reject all three before the real schedule
  is applied, and the focused DejaGNU golden requires
  `rejection-selftest=passed` in the dump.
- Constant-LREG reads remain zero-pressure values, but the checker proves that their definitions stay before every moved consumer. Dedicated Wormhole and Blackhole fixtures exercise this ordering. Debug compilation is conservatively gated off, `-O0` is a no-op, QSR32 remains a no-op, and only unconditional one-basic-block XTT32 `sfpadd`/`sfpmul`/`sfpmad` regions are eligible.
- The canonical user-facing list-scheduler flag is `-mtt-tensix-optimize-pressure-schedule`; the old private LP spelling is only an undocumented compatibility alias. A second flag, `-mtt-tensix-pressure-schedule-use-milp`, is required to invoke the solver. Therefore installing a solver-linked compiler cannot silently replace list scheduling.
- `--with-lp-solve={no,auto,yes,PATH}` is a host configure option. The default is `no`; explicit requests require `lpsolve/lp_lib.h` plus a linkable host `liblpsolve55` and SuiteSparse dependencies, and an explicit prefix is checked rather than falling through to the target sysroot or an unrelated system installation.
- The first MILP is schedule-only. It uses stable integer op/value IDs, one binary issue choice per op/slot, exact def-use precedence, exact after-slot liveness, and an eight-register capacity constraint. Same-slot death followed by a result definition models ordinary destructive reuse without inventing `_lv` predication. It handles at most 24 operations and 32 values, uses a deterministic 100,000-node cap, and accepts only `OPTIMAL`. An initial sequential lexicographic objective proved far too expensive on the 17-op fixture. M3a now performs one solve with an exact 0/1 objective that prefers the deterministic list schedule: when the list result already fits, its issue choices are fixed and `lp_solve` independently certifies the full liveness/capacity model; when it does not fit, the MILP may deviate to find the fewest changed issue slots. Liveness linearization columns are bounded continuous variables implied exactly by binary issue choices, substantially reducing the branch-and-bound model. Stable model construction plus repeated serial/parallel hashes are its determinism gate. Capped, unavailable, infeasible, aborted or invalid results fall back to the list scheduler; no incumbent is accepted.
- A dump-only RTL pass now runs immediately before IRA. It reports actual XTT32 pseudo/hard-register liveness after expansion, sched1 and early rematerialization. Its output deliberately says `colorability=unchecked`: this is a GIMPLE-to-RTL reality audit, not the physical-allocation guarantee required for production.
- The focused validator now checks manual-control equality, list-versus-MILP equality, solver cap fallback, independent certificates, the final RTL audit, constant-LREG source availability, duplicate uses, O0/debug/QSR gates, and serial/parallel determinism on both Wormhole and Blackhole. The general test wrapper also fails early when `runtest` is absent or when a purported test run reports zero expected passes.

The remaining hard boundary is unchanged: a GIMPLE schedule can shorten interference but cannot force IRA to realize an arbitrary physical coloring. The first production-grade guarantee must solve and independently validate the actual pre-IRA RTL island, atomically substitute certified hard LREGs, re-run recognition/dataflow checks, and enter IRA with no SFPU pseudos in that island. Until that milestone and real transformed-Welford hardware testing land, the MILP flag is a default-off research facility rather than a product optimization.

The first measured solver runs sharpen that boundary. A sequential lexicographic formulation did not finish the 17-op Welford rescue after roughly five minutes and was removed. The bounded one-shot model compiles the same checking-build fixture in 14.75 seconds versus 13.89 seconds for the list scheduler, about 0.86 seconds or 6% overhead in this VM. That is acceptable telemetry for an explicit research opt-in but remains above the proposed 5% p95 budget for broad enablement.

More importantly, exhaustive enumeration produced a ten-op arithmetic DFG where source order peaks at eleven, the deterministic list heuristic remains above eight, and MILP finds an independently validated `11 -> 8` schedule. Final IRA nevertheless emits the fatal SFPU spill. The checked-in `scripts/lp-schedule-milp-beats-list.C` fixture intentionally records this as an expected M2 research boundary rather than a passing DejaGNU test. It proves both that MILP can add value beyond the list heuristic and that GIMPLE feasibility alone is not the production allocation guarantee.

The benefit is not hard-coded to Welford.  A separate fused arithmetic fixture
loads eight independent inputs and computes add/multiply/MAD chains.  Its
source order keeps two inputs live while creating a ninth value and fails with
the ordinary SFPU spill on both WH and BH.  The generic list scheduler and
MILP both validate `9 -> 8`, compile successfully, and emit identical assembly
to each other.  Conversely, the existing Horner/polynomial test reports peak
two and is byte-identical off/list/MILP.  The WH fused-DAG schedule contains
two more hazard NOPs than a manually latency-aware early fold, which is direct
evidence that this checkpoint is a feasibility scheduler, not yet a latency
optimizer; the pass remains opt-in and the unscheduled source emits no code at
all because allocation fails.

### Final local M0 checkpoint

The final checking compiler was rebuilt after adding the independent
certificate-rejection self-test and after fixing the DejaGNU driver to include
the algorithm-agnostic `pressure-schedule-*.C` fixtures. The real TT/SFPI
driver then completed with **1,106 expected passes, two expected failures, and
zero unexpected FAIL, ERROR, or UNRESOLVED results**. The sum file is
`/home/nkapre.guest/sfpi-lp-build/evidence/tt-g++-final.sum` with SHA-256
`fffeb5ca85218fdf49421b57bbdc8da0b5571e8e0b5564864e31b4679756c272`.
The final focused harness also passed and recorded 52 assembly artifacts. The
generated Welford-shaped rescue hashes are `3cee70a65b29062489d790bcc30b23cc43ffe0741cc634e5106be8b0b14d8344`
on Wormhole and `502bff4f216e3ed5d90ba891cb6a0c6a636e765f6ad53ab1c94d04f0b0f3ed1`
on Blackhole; the unrelated fused-DAG hashes are
`5ddaca11f72fe93daa31150c8c56f4dc056a1df0d679bd126fad9b212e74e4ce`
and `11e952af9e49f1d5bde9f57973a08a33856295af961f97de621c1f9a970c98b6`
respectively. Every transformed dump reports `old-peak=9 new-peak=8`,
`validated=yes`, and `rejection-selftest=passed`.

This is the same target-specific test lane used by SFPI's checked-in
`build-sfpi.yaml` (`scripts/build.sh --test-tt`), executed locally in the
x86_64 Linux VM with `lp_solve` linked. It is not an authoritative
Tenstorrent-org CI result: the private `nkapreTT` repositories can reproduce
the job, but product CI remains pending until these commits run on a
Tenstorrent-owned branch or pull request.

An attempted broad GCC `check-c/check-c++` diagnostic was stopped after it
produced hundreds of unrelated RISC-V simulator/newlib execution failures in
ordinary libc and torture tests. The pressure pass was disabled in those
tests, and there was no matching prepatch baseline, so that output cannot be
used either as a regression or as a green gate. A future full-GCC comparison
must run base and patch from the same environment and compare `.sum` files.

## Remaining execution plan after the prototype

This section is the reconciled result of a second compiler, allocator, regression and TT-Metal validation review. It supersedes any interpretation that the overnight prototype is already an LP scheduler or a production-ready optimization.

### Honest status

| Capability | Status | Current evidence or gap |
|---|---|---|
| XTT32 GIMPLE pressure oracle | Implemented for one straight-line basic block | WH/BH minimal Welford reports peak 8; rescue reports 9 |
| Deterministic pressure-first list schedule | Implemented | WH/BH rescue reaches 8 and matches the manual early-fold assembly |
| Conservative arithmetic allowlist | Implemented | Non-live `sfpadd`, `sfpmul` and `sfpmad` only |
| CFG and CC/predication rejection | Implemented | Negative fixtures remain pass-off/on identical |
| Independent transactional legality checker | Implemented | Rebuilds the op permutation, all SSA def-use/source-availability constraints and exact liveness before mutation |
| Latency/resource model | **Not implemented** | No makespan or NOP non-regression claim is currently valid |
| Enforceable physical LREG assignment | **Not implemented** | The list-missed 11-to-8 fixture still spills after a valid MILP/GIMPLE certificate, directly demonstrating the gap |
| Optional `lp_solve` integration | Implemented, default off | Host-only configure probe/linkage and a capped schedule-only MILP; Welford is certified 9-to-8 and an exhaustive synthetic graph demonstrates a list-missed 11-to-8 schedule |
| Final pre-IRA RTL pressure audit | Implemented, dump-only | Observes actual XTT32 hard/pseudo liveness after sched1/remat; does not yet prove colorability or assign LREGs |
| Non-Welford positive fixture | Implemented | A generic fused add/multiply/MAD DAG fails at peak 9 with the pass off and compiles at validated peak 8 with list and MILP schedules on WH/BH |
| Low-pressure non-Welford no-op | Implemented | The existing Horner/polynomial fixture reports peak 2 and has identical off/list/MILP assembly |
| Vanilla eltwise Welford functional fixture | **Not implemented** | The late-fold rescue is a focused compiler fixture, not the production kernel |
| WH/BH craq-sim smoke | Implemented for handwritten EMA | Useful non-crash/numerical smoke; not transformed Welford validation |
| WH/BH hardware comparison with handwritten Welford | **Not run** | No authoritative latency, NOP-safety or replay-throughput result yet |

The first landing decision, the MILP decision and a future default-on decision are separate. Passing one must not be presented as satisfying the next.

### Critical path

```text
Research path (implemented, default off):
M0 harden current list scheduler
  → M1 canonical model and dump-only final-RTL audit
  → M3a optional schedule-only lp_solve experiment

Production path (mandatory before support/default-on):
M1 final-RTL audit
  → M2 enforce physical allocation on final pre-IRA RTL
  → M3b prove solver value, packaging and failure containment
  → M4 transformed Welford functional and hardware sign-off
  → M5 broader kernels, soak and rollout
```

The real Welford test harness can be developed in parallel with M0–M2. A solver-produced GIMPLE schedule may be used as a default-off research experiment once the independent checker accepts it, as this branch now does. It must not be described as production-safe, merged as a supported optimization, or enabled by default until the final-RTL allocation-enforcement path exists.

### M0 — harden the current scheduler

This is the smallest technically honest next patch.

1. Replace the current pressure-only recheck with a separate `schedule_solution` and `validate_schedule` path that does not share the scheduler's bookkeeping. Before mutation it must independently verify:

   - the result is an exact permutation of the original operation multiset;
   - every vector and scalar SSA definition precedes every use, including duplicate operands;
   - every source is available at the proposed issue point;
   - region membership, allowlist, CC state, CFG and barrier rules still hold;
   - live-in, live-out and live-through values produce the claimed old/new peaks;
   - the new peak is at most eight and strictly lower than the old peak;
   - the opcode/operand fingerprint is unchanged.

2. Make constant-LREG reads explicit in the legality model. Either treat a constant `sfpreadlreg` as a boundary, retain it as a zero-pressure dependence node, or prove that every scheduled consumer remains after its definition. Add a regression that would fail if a consumer crossed the read.
3. Define debug and optimization-level policy. Initially make `-O0` a no-op even when the flag is explicit, and either treat debug binds as boundaries or repair/reset them with checking tests under `-g`.
4. Rebuild and verify real and virtual SSA after an accepted move. Emit stable dump lines such as `validated=yes` or `validated=no reason=...`; validation failure must leave GIMPLE untouched.
5. Rename the experimental interface before it becomes public baggage. Prefer `rvtt_pressure_schedule` and `-mtt-tensix-optimize-pressure-schedule`, or an enum such as `-mtt-tensix-scheduler={off,list,milp}`. Preserve the old private spelling only if existing automation needs a temporary alias.
6. Either gate the transform to Wormhole and Blackhole or add full QSR32 positive/negative parity. QSR64 is a runtime build variant, not a Tensix scheduling target.

M0 exits only when the deliberate invalid-solution tests are rejected, the WH/BH rescue still equals the manual assembly, all unsupported fixtures remain byte-identical, 20 serial and 20 parallel builds are deterministic, full `--test-gcc` and `--test-tt` have no new unexpected results, and the test wrappers assert that `runtest` exists and a nonzero expected test count actually ran.

Until M0 exits, this branch is **no-go even for a generally supported default-off merge**. The current private prototype remains useful for research and fixtures.

### M1 — canonical scheduling model and final-RTL reality audit

Extract solver-independent data structures into target scheduling files rather than teaching each heuristic or solver to rediscover semantics:

```text
gcc/gcc/config/riscv/tt/rvtt-schedule.h
gcc/gcc/config/riscv/tt/rvtt-schedule.cc
```

The model should contain stable operation/value IDs, original order, all SSA dependencies, live-in/out boundaries, operation fingerprints, and per-architecture descriptors for movability, resource class, latency, semantic LV operands and legal ordinary destination/source overlap. Reject any opcode without a positive descriptor.

Add a dump-only pre-IRA RTL pass, for example:

```text
gcc/gcc/config/riscv/tt/rtl-rvtt-lp-alloc.cc
```

Register it immediately before IRA, after generic scheduling and early rematerialization. It must inspect the actual expanded SFPU pseudos, hard-register reads/writes, existing semantic `_lv` ties, recognizer alternatives, clobbers and fixed registers. Compare final-RTL pressure/colorability with the GIMPLE certificate and report mismatches without changing RTL.

M1 exits only when the WH/BH rescue is demonstrably eight-colorable at this final boundary, minimal Welford is correctly reported as zero-slack, every SFPU operand/constraint is modeled, randomized DAGs up to roughly ten nodes agree with exhaustive enumeration, and architecture metadata predicts the hazards repaired by the existing postreload scheduler. Any unexplained GIMPLE-to-RTL mismatch is a hard stop.

### M2 — enforce the allocation certificate

This is the production correctness boundary. A GIMPLE-selected color is only a feasibility suggestion; IRA is not required to reproduce it.

For a closed straight-line SFPU island immediately before IRA:

1. Build the actual RTL interference and constraint graph after all earlier expansion, scheduling and rematerialization.
2. Enumerate legal XTT32 placements L0–L7, including fixed/precolored operands, semantic `_lv` matching constraints, clobbers and ordinary overlaps accepted by the machine description.
3. Express destructive reuse as the result and a dying operand receiving the same hard register. Do not synthesize an `_lv` operand: `_lv` carries inactive-lane predication semantics.
4. Independently validate the coloring, then atomically substitute the certified island's SFPU pseudos with hard registers using GCC grouped changes.
5. Re-run recognition and dataflow scans. Commit only if every instruction remains recognized and no SFPU pseudo remains inside the certified island before IRA.

Existing non-LV arithmetic patterns generally accept equal destination/source hard registers, so new tied MD patterns are not automatically required. Add one only when the recognizer rejects a hardware-legal overlap. If safe pre-IRA hard-register substitution is incompatible with GCC invariants, stop and design explicit target IRA integration; do not weaken the claim to “IRA will probably follow the coloring.”

M2 exits when final assembly matches the allocation certificate on WH and BH across supported optimization modes, no certified island can attempt `BADLOAD`, `BADSTORE` or an SFPU spill, fixed-LREG boundary tests pass, and failure before the grouped commit leaves RTL byte-identical.

XTT64/XTT128 remain out of scope. Later support must enumerate legal aligned/contiguous hard-register tuples from target rules rather than counting anonymous two- or four-register units.

### M3 — optional `lp_solve`, initially schedule-only

The external solver earns its complexity only if the list scheduler misses a real feasible graph or loses measurable cycles on at least two kernels. Welford plus a second independent-chain or polynomial graph should provide that entry evidence.

Add a small solver adapter and guarded host-build integration. Expected touch points include GCC/SFPI `configure.ac`, generated configure files, GCC `Makefile.in`, `t-riscv-tt`, and new `rvtt-lpsolve.{h,cc}` files.

Requirements:

- explicit `--with-lp-solve={no,auto,PATH}` behavior;
- solver-absent builds and bootstraps remain supported;
- an explicitly requested but missing header/library fails configure clearly;
- link the host library into `cc1`/`cc1plus`; never search the RISC-V target sysroot;
- validate x86_64 and aarch64 Linux host packaging and missing-runtime-library behavior;
- pin the solver version and complete LGPL, SBOM, security and distribution review.

The implemented M3a research model is deliberately smaller than the eventual production model: one unconditional XTT32 basic block, at most 24 operations and 32 values, one issue slot per operation, exact def-to-last-use liveness, precedence and an eight-register capacity constraint. It does not choose physical colors, model architectural latency/resources, or promise a makespan improvement. It performs one solve over a stably constructed model, minimizing the number of issue slots that differ from the deterministic list result; the independent M0 checker remains authoritative before mutation and serial/parallel assembly hashing detects nondeterminism.

The later M3b production model should retain similarly strict region limits while adding fixed assignment, live-and-assigned capacity, architecture resource/latency rules and only machine-representable destructive overlaps on the actual pre-IRA RTL graph.

For M3b, solve lexicographically:

1. find a feasible schedule/coloring with peak at most eight;
2. minimize architecture-specific makespan;
3. minimize live area and copies;
4. apply a canonical source-ID tie-break.

M3a accepts only a proven `OPTIMAL` schedule that passes the independent GIMPLE checker. Solver unavailable, infeasible, non-optimal or capped falls back to the independently checked list schedule; a malformed purported optimum leaves the region untouched. M3b additionally requires M2 allocation materialization. Discard timeout incumbents in both phases. In-process solver crashes or hangs cannot be recovered by semantic fallback, so a supported/default-on build requires a pinned trusted library and an explicit watchdog/release-risk decision.

M3 exits only after exhaustive small-DAG oracle agreement, forced failure-path tests, ASan/UBSan model/checker fuzzing, repeated and parallel deterministic builds, acceptable compile-time telemetry, and a measured incremental win over list scheduling.

### M4 — real eltwise Welford comparison

Add a TT-LLK functional driver modeled on the EMA test:

```text
tt_metal/tt-llk/tests/python_tests/test_sfpu_welford.py
tt_metal/tt-llk/tests/sources/sfpu_welford_test.cpp
tt_metal/tt-llk/tests/python_tests/perf_sfpu_welford.py
tt_metal/tt-llk/tests/sources/sfpu_welford_perf.cpp
```

Compile the same non-recurrence machinery with selectable row implementations:

- `HANDWRITTEN_DIRECT`;
- production `HANDWRITTEN_REPLAY`;
- exact vanilla `VFLOAT_DIRECT`;
- `VFLOAT_RESCUE`;
- `VFLOAT_MANUAL_EARLY_FOLD`;
- optionally `VFLOAT_REPLAY` once generated row lengths are stable.

Share block loads, reciprocal lookup, state clear, raw mean/M2 stores and variance finalization. Only the row recurrence may differ; otherwise transpose, reciprocal or pack effects would be misattributed to scheduling.

The functional matrix covers logical counts `1, 4, 31, 32, 33, 64, 96`, BF16 and FP32 accumulation, reciprocal-LUT and ordinary reciprocal modes, constant/zero, monotonic, alternating magnitude, fixed-seed random, high-offset near-constant, NaN/Inf/signed-zero, poisoned inactive rows, multi-tile state carry and repeated invocations in one process. Compare raw mean and M2 as well as final variance against:

- the manual implementation differential;
- sequential FP32 Welford emulation;
- Float64 mean/population variance.

Record bitwise mismatch count, maximum and percentile ULP distance, absolute/relative error, classification/sign mismatches and unexpected negative/nonfinite M2. PCC alone is insufficient for raw statistics.

Craq-sim can establish compilation, raw numerical equivalence, partial-row handling, state carry, replay interaction and gross architecture correctness. It cannot sign off latency or NOP safety.

On real Wormhole and Blackhole hardware, archive compiler/TT-Metal commits, board stepping, firmware and clock, assembly/ELF hashes, hard-register map, MAD/ADD/MOV/NOP/load/store/replay counts, replay footprint, text size and cycles per row/block/tile. Measure direct primitive and production replay throughput separately, with warmups and paired randomized runs. Report median, p5/p95 and median absolute deviation; never use pytest or simulator wall time as a performance number.

M4 exits when all functional cases pass on both architectures, rescue output and assembly match the manual control, no intermittent hazard failure appears, and static instruction counts explain measured cycle ordering. Replacing handwritten LLK requires generated vFloat to match or beat its median hardware cycles without numerical degradation. If it is slower, retain the scheduler as a compilation-feasibility feature and make no LLK-replacement claim. Architecture disagreement permits architecture-specific implementations.

Then add a benchmark-only implementation selector to real LayerNorm Welford and run the existing LayerNorm/state-leak suites across partial and multi-tile widths before changing any production default.

### M5 — regression firewall, expansion and rollout

Every off/on matrix should cover WH/BH and QSR32 only when QSR scheduling support is explicitly enabled:

- true and anti dependencies, duplicate operands, scalar SSA, constant-LREG use, boundary live-in/out, debug binds and live-out stores;
- peaks 7/8/9, irreducible pressure, size caps and multiple separated regions;
- `_lv` and nested CC, memory, LREG/config access, stochastic operations, synth/runtime immediates, explicit NOP/replay, LUT/transpose/swap, asm/calls/EH, CFG/PHI/loop and composite XTT modes;
- `-O0/-O1/-O2/-O3/-Os`, `-g`, exceptions, LTO where supported, different working directories, serial and parallel builds;
- solver absent/present/wrong path/missing runtime library, optimal/infeasible/non-optimal/timeout/invalid certificate;
- full GCC/TT suites, complete TT-Metal kernel compilation, numerical device tests, MAD/MOV/NOP/code-size/replay deltas, and compile-time p50/p95/RSS.

The current `test_sfpi.cpp` skips WH and BH, so resolve those skips or add an enabled scheduler-specific device fixture before treating it as a primary gate.

After Welford, expand one family at a time:

1. dual-Horner rational evaluation for independent-chain latency hiding;
2. addcmul for cross-row scheduling;
3. log for a second compact pressure case;
4. GELU-tanh and erfinv for long-lived input/reload pressure;
5. remainder and asinh only after separately gated rematerialization;
6. atanh and binary power last because they deliberately cut graphs through DST or use delicate numerical compensation.

Each family repeats compiler certificate, craq-sim differential, static instruction audit and WH/BH hardware cycles. Welford success does not authorize new opcodes, CFG shapes or predication semantics.

Rematerialization is a later independent phase, not part of the first MILP. It may duplicate only positively whitelisted deterministic, state-free, non-trapping operations and must prove a net register/cycle benefit. Stochastic, CC, config, LUT, memory and architectural-state operations are never rematerialized.

### Landing gates

| Decision | Minimum go criteria |
|---|---|
| Merge list scheduler default-off | M0 complete; pre-patch versus patched-flag-off corpus identical; full suites show no new unexpected results; WH/BH tests and QSR gate/parity reviewed |
| Shadow CI/canary | Whole TT-Metal corpus compiles off/on; zero unsupported-region diffs; compile p50 ≤2%, p95 ≤5%, RSS ≤5%; transformed Welford plus two non-Welford kernels numerically clean |
| Named product-kernel opt-in | WH/BH hardware correctness; affected cycles no worse than 1% at 95% confidence; code/replay regression below 2% or explicitly signed off; JIT cache key includes scheduler/toolchain state |
| Global default-on | At least 14 consecutive green nightlies and 10,000 distinct kernel-TU compilations; historical production failure captured; rollback flag propagated through TT-Metal; architecture/compiler/op/release owners sign off |
| MILP merge/default-off | M0–M3 complete; real incremental value over list scheduling; host-build, legal/security, determinism, fuzzing and failure-injection gates pass |

Keep MILP default-off through its own soak even if list scheduling is later enabled. Expanding eligibility is always a separate review. The existing postreload hazard scheduler remains the final verifier/repair, and any unexpected NOP beyond the modeled schedule is a test failure.

Ownership is explicit: SFPI compiler owners own SSA/RTL legality, allocation enforcement, determinism and rollback; architecture/LLK owners own overlap and latency tables plus handwritten goldens; TT-Metal operation owners own Welford/LayerNorm numerical and device-performance acceptance; toolchain/release owners own host packaging, cache invalidation and reproducibility; legal/security owners approve the solver distribution; CI/simulator owners enforce real test counts and keep the craq-sim compatibility patch separate from hardware sign-off.

Planning estimate, assuming one compiler engineer with architecture and hardware support: M0 takes roughly 2–4 focused days; an end-to-end solver plus enforceable allocation prototype is roughly 2–3 weeks; production sign-off is approximately 4–7 engineer-weeks plus hardware queue time and the 14-night soak. The allocation-enforcement spike is the largest uncertainty.

## What happens today

The compiler path is:

```text
vFloat C++
  → __builtin_rvtt_* operations
  → late GIMPLE SSA transforms
  → RTL expansion
  → generic GCC scheduling
  → IRA/LRA register allocation
  → rvtt_schedule hazard/NOP repair
  → assembly
```

Important points:

- `vFloat` is a wrapper over `__xtt_vector`; assignment emits `sfpassign_lv` in `include/sfpi_classes.h`.
- Late GIMPLE SSA is already the DFG. There is no separate SFPI graph extractor.
- `rvtt_get_insn_data(gcall*)` classifies SFPU nodes in `gcc/gcc/config/riscv/tt/rvtt.cc`.
- Late target GIMPLE cleanup, combine and liveness passes are registered in `gcc/gcc/config/riscv/tt/rvtt-passes.def`.
- GCC exposes exactly eight variable SFPU registers, L0–L7, in `gcc/gcc/config/riscv/riscv.h`.
- An attempted spill intentionally aborts with `cannot store sfpu register (register spill)` in `gcc/gcc/config/riscv/tt/rvtt.cc`.
- The current `gcc/gcc/config/riscv/tt/rtl-rvtt-schedule.cc` does not reorder operations. It runs after reload and inserts hazard NOPs. By then it is too late to rescue any graph for which allocation attempted an SFPU spill.

## Why vanilla Welford is fragile

The minimal source-shaped recurrence is:

```cpp
vFloat delta = x - mean;
mean += delta * recip;
vFloat delta2 = x - mean;
m2 += delta * delta2;
```

With four input rows prefetched, the first update starts with seven live values:

```text
L0–L3: x0, x1, x2, x3
L4:    mean
L5:    M2
L7:    reciprocal
```

`delta` consumes the final free register, L6. From there, compilation succeeds only if allocation chooses:

```text
new_mean → overwrite dying old_mean
delta2   → overwrite the current input x
new_M2   → overwrite old M2
```

A feasible conceptual allocation is:

```text
L6 = x  - L4
L4 = L6 * L7 + L4
x  = x  - L4
L5 = L6 * x  + L5
```

That is exactly eight registers with no slack. Any extra temporary, missed coalescing, literal load, or legitimate predication `_lv` lifetime demands a ninth register. Because spills are unsupported, reload aborts when that ninth simultaneous value is real or the required aliases are not realized. The current 7.69.0 compiler realizes the aliases for the minimal fixture; older releases or a larger eltwise wrapper may not.

This is why scheduling the unchanged DFG is insufficient. The compiler must also choose and enforce destructive operand/result aliases.

## What the handwritten LLK does

The primary implementations are:

- Wormhole: `~/workspace/tt-metal/tt_metal/tt-llk/tt_llk_wormhole_b0/common/inc/sfpu/ckernel_sfpu_welfords.h`
- Blackhole: `~/workspace/tt-metal/tt_metal/tt-llk/tt_llk_blackhole/common/inc/sfpu/ckernel_sfpu_welfords.h`

They explicitly fix the register layout:

| Registers | Value |
|---|---|
| L0–L3 | Four prefetched inputs |
| L4 | Mean, later reused for recomputed delta |
| L5 | M2, updated in place |
| L6 | Only scratch; delta then new mean |
| L7 | Reciprocal |

The per-row schedule is:

```text
A: L6    = x - old_mean
   NOP                       # Wormhole
B: L6    = L6 * recip + old_mean
C: L4    = x - old_mean      # recompute delta; destroy old mean
D: x     = x - L6            # destroy input
   NOP                       # Wormhole
E: L5    = L4 * x + L5
F: L4    = L6
```

The recomputation is important for two reasons:

- It structurally guarantees destructive reuse instead of trusting IRA.
- On Wormhole, `C` fills the result-latency gap between `B` and `D`, trading an extra MAD for fewer exposed stalls.

The handwritten golden is five MADs plus one move per row, with two Wormhole NOPs. A solver should be allowed to discover either this recomputation schedule or the retained-delta four-MAD schedule, depending on architecture latency and actual makespan.

The clean full-kernel benchmark is:

```text
~/workspace/tt-metal/ttnn/cpp/ttnn/operations/normalization/layernorm/device/kernels/compute/layernorm_welford.cpp
```

It keeps mean and M2 resident across the entire width. GroupNorm is a later benchmark because it deliberately saves multiple group states to DST; those are algorithmic state stores, not compiler spills.

## Does the benefit extend beyond Welford?

Yes. There are three reusable benefit classes:

1. **Pressure rescue:** shorten live ranges, choose legal destructive updates, or deliberately recompute a cheap value so an otherwise unallocatable graph fits L0–L7.
2. **Latency hiding:** interleave independent chains in the two-cycle SFPMAD RAW gap instead of emitting `SFPNOP`.
3. **Move/reload avoidance:** keep an input or accumulator resident through a helper instead of cutting the graph through DST solely because allocation failed.

Concrete evidence in the Wormhole TT-Metal tree:

| Kernel | Existing manual workaround | What it tests |
|---|---|---|
| `ckernel_sfpu_log.h:25-67` | Explicit `Reload for register pressure` at line 62 | Small polynomial plus exponent correction; best second pressure test |
| `ckernel_sfpu_gelu.h:355-380` | Reloads the original input from DST due to register pressure | Keeping a long-lived input across an inlined tanh helper |
| `ckernel_sfpu_erfinv.h:48-56` | Reloads input after nested log/sqrt work | Pressure across nested fully-inlined helpers |
| `ckernel_sfpu_binary_pow.h:215-252` | Avoids keeping `s` live across exp to prevent a reload-insn ICE | Large-region pressure and boundary-lifetime stress |
| `ckernel_sfpu_binary_remainder.h:90-121` | Recomputes divisor chunks | Bounded rematerialization for integer-view vector values |
| `ckernel_sfpu_remainder.h:114-145` | Reloads `a` from DST | Reload elimination |
| `ckernel_sfpu_trigonometry.h:1120-1168` | Recomputes `abs(inp)` and `inp*inp` because caching triggers reload ICE | Rematerialization versus retention |
| `ckernel_sfpu_trigonometry.h:1190-1217` | Uses DST to break an atanh expression that exceeds the budget | Larger graph partitioning, later phase |
| `ckernel_sfpu_piecewise_rational.h:95-132` | Interleaves numerator/denominator Horner chains to hide SFPMAD latency | Best independent-chain latency benchmark |
| `ckernel_sfpu_addcmul.h:35-61` | Processes two rows as `MUL_a, MUL_b, MAD_a, MAD_b` | Cross-row latency hiding |
| `ckernel_sfpu_rounding_ops.h:137-151` | Schedules exponent work in explicit “Hide SFPNOP” slots | Later predication-aware latency test |

`ckernel_sfpu_expm1_cw.h:30-54` is another natural straight-line candidate: range reduction keeps `x`, `tmp`, `k_f` and `r` around a polynomial evaluation before constructing `two_k`. Reciprocal/Newton kernels and fused eltwise kernels with multiple live streams have the same shape.

The initial benchmark ladder should be:

1. Welford: feasibility plus destructive aliasing.
2. Log: straightforward pressure reduction.
3. Dual-Horner rational evaluation: independent-chain latency hiding.
4. Addcmul: two-row interleaving.
5. GELU-tanh and erfinv: remove explicit pressure reloads.
6. Remainder and asinh/atanh: bounded rematerialization versus explicit DST cuts.
7. Binary power: larger-region stress test.

### Cases that should not be optimized initially

- Any region containing `v_if`, `v_else`, `v_and`, CC push/pop, or another nontrivial predication epoch.
- DST, LREG or memory loads/stores inside the candidate region.
- Explicit `TTI_*` or physical-LREG code, including handwritten Welford/EMA or MoE/top-k microcode. These are goldens, not transformation inputs.
- Transpose, swap, shuffle, `SFPCONFIG`, `SETRWC`, replay/loadmacro, address-counter changes, explicit NOPs, or unknown builtins.
- Stochastic rounding, RNG, configuration reads, architectural-state reads, dynamic synth/runtime-immediate operations until each is audited.
- Calls unless fully inlined and every resulting operation is positively classified.
- XTT64/XTT128 until legal contiguous/aligned register tuples are modeled and tested.
- LUT kernels that deliberately pin most of L0–L7.
- Multi-block CFG, PHIs, loops and EH regions.

Low-pressure unary kernels are legal future candidates but are unlikely to repay solver cost. Control-, memory-, LUT- and transpose-heavy regions are fundamentally poor fits for this pass.

## Proposed pass

Add a final target-specific GIMPLE pass immediately before RTL expansion:

```cpp
INSERT_PASS_BEFORE(pass_expand, 1, pass_rvtt_lp_schedule);
```

Place it after `pass_rvtt_attrib` in `rvtt-passes.def`.

GIMPLE is the right MVP boundary because:

- SSA directly exposes def-use edges and live ranges.
- RVTT combine, FMA formation, predication liveness, CC cleanup and immediate lowering have already run.
- Scheduling here changes lifetimes before IRA.
- Once expanded, most SFPU instructions become conservative `UNSPEC_VOLATILE` RTL.
- Post-reload is too late.

The initial region should be a straight-line sequence inside one basic block, in a CC epoch proven all-lanes-enabled. It contains only positively allowlisted, deterministic XTT32 arithmetic. All nonconstant scalar SSA operands must be defined before the region. The first unsupported statement terminates the region; the pass does not partially reorder around something it merely assumes is harmless.

### DFG extraction

For each region:

- Node: a `gcall` accepted by `rvtt_get_insn_data`.
- True edge: XTT SSA operand definition to use.
- Boundary values: PHIs, live-ins and live-outs.
- Program-order edges: every ordering constraint implied by operands or audited target semantics. Every non-allowlisted statement is a region boundary, not just an edge.
- Destructive candidates: a result may reuse an operand whose final use occurs at that operation.
- Mandatory `_lv` tie: the output must reuse the semantically required inactive-lane value. This is not an optional coalescing hint.
- Constant LREGs such as L11 do not count against the eight variable registers.
- Include all operands, including scalar SSA operands to target builtins; considering only XTT operands can legalize an invalid reorder.
- Phase one rejects XTT64/XTT128. A later implementation must enumerate legal contiguous/aligned hard-register tuples according to `HARD_REGNO_MODE_OK`, rather than merely charging two/four anonymous units.

Do not use `rvtt_insn_data::has_side_effects == false` as a purity test. It only describes part of the target semantics, while most generated arithmetic becomes `UNSPEC_VOLATILE` RTL. Maintain a small positive table that states, per opcode and architecture, whether it may move, whether it may duplicate, its resource use, latency, live operand, and legal destructive operands.

## MILP formulation

Despite the name, this needs `lp_solve` mixed-integer support, not continuous LP.

Use time-indexed issue/liveness variables plus a time-invariant register assignment:

```text
issue[i,t]      operation i is issued in slot t
live[v,t]       value v is live during slot t
assign[v,r]     value v is assigned to physical LREG r for its lifetime
occupy[v,r,t]   logical AND of live[v,t] and assign[v,r]
alias[i,v]      result of i destructively reuses value v
```

Without `assign[v,r]`, `occupy[v,r,t]` lets a value move between registers for free. Alternatively the model would need explicit move and continuity constraints. A static assignment is smaller and matches the no-spill/no-move MVP.

Constraints:

- Every operation issues exactly once.
- At most one SFPU operation issues per cycle.
- SSA dependency: consumer slot is at least producer slot plus architecture latency.
- `live[v,t]` is exactly linearized from the scheduled definition through the latest scheduled use, including boundary live-outs. It is not a free capacity variable.
- Operands are read in the issue slot; a produced value becomes live after that slot. This permits a result to overwrite an operand that dies at the operation without falsely counting both simultaneously.
- Fixed live-ins occupy their required physical registers.
- Each value has one static legal register assignment.
- `occupy[v,r,t] = live[v,t] AND assign[v,r]` using the standard three linear inequalities.
- No two live values occupy the same LREG.
- A result can share an operand register only when that operand dies at the operation, or when `_lv` semantics require the tie.
- Register capacity is always at most eight.
- Alias choices create anti/output dependency edges so overwrites occur only after all required reads.
- Architecture-specific latency/hazard rules determine whether an independent operation can fill a gap or a NOP is required.

Solve lexicographically and sequentially:

1. Establish feasibility with peak occupancy at most eight.
2. Minimize makespan at that feasible peak.
3. Minimize copies and live-range area with makespan fixed.

Do not collapse these into one weighted objective: no latency win is worth accepting nine registers.

Start with regions of roughly 16–24 operations, at most 24 SSA values, and a 48–64 cycle horizon. Cap candidate regions per function. Use stable node ordering, integer coefficients and a deterministic node/iteration limit. A wall timeout may be an emergency escape, but its incumbent must be discarded: accepting whatever happened to be found near a time boundary makes compiler output nondeterministic. Only a proven `OPTIMAL` solution is eligible for application.

### Scheduling certificate versus allocation guarantee

The solver can prove that a coloring and tie plan exists, but GIMPLE cannot force IRA/LRA to honor an arbitrary physical coloring. Existing `_lv` machine patterns can enforce an output/input tie through matching constraint `0`; they cannot encode a complete physical assignment.

More importantly, `_lv` preserves inactive lanes under predication. Introducing it as a generic allocation hint may silently change values. Only in a CC epoch proven all-lanes-enabled may an exactly equivalent `_lv` form encode its semantic live-value tie.

Welford also needs ordinary destination/source overlap with a dying arithmetic operand. That is a different constraint. Add internal tied RTL patterns or alternatives for each hardware-legal operand position, or attach an internal annotation that expansion turns into such a pattern. Solver alias variables may range only over ties the machine description can represent and the hardware implements. Never synthesize a fake LV operand merely to request coalescing.

For the MVP:

- Contract every selected destructive tie that can be represented semantically and mechanically.
- Recompute the contracted interference graph and require a valid eight-color certificate.
- Assert after expansion that every promised tie appears in recognized RTL.
- Validate post-reload assembly in tests.

This materially raises the odds that IRA succeeds, but it is not an honest hard guarantee for arbitrary colorings. A complete guarantee requires either target integration with IRA or a small pre-reload local hard-register allocator/substitution mechanism for certified regions. Do not claim Welford is fixed until final RTL/assembly demonstrates the promised ties and no spill attempt.

## Regression firewall

The pass must be transactional: extract, solve and independently validate without mutating GIMPLE; only then apply the complete rewrite. Timeout, infeasibility, an unsupported operation, validation failure, or inability to materialize a tie leaves the original statements untouched and emits at most a dump note.

Required invariants:

- The flag is off by default during development and soak. With the flag off, compiler output is byte-for-byte unchanged.
- A build without `lp_solve` remains supported and produces the unchanged compiler behavior.
- Eligibility uses the positive opcode/architecture allowlist above.
- Preserve the exact floating-point operation multiset and operand association. Do not reassociate, newly contract an FMA, or change rounding mode.
- Phase one performs no rematerialization.
- Later rematerialization uses a tiny whitelist of deterministic, nontrapping, state-independent operations. Never duplicate stochastic, CC, config, state-read or memory operations.
- Require `gimple_call_nothrow_p`; reject EH edges and potentially throwing calls.
- Preserve each statement's original `location_t` and lexical block. A clone inherits its source statement's location rather than a region-wide location.
- Rebuild SSA and virtual SSA after mutation; checking builds run `verify_ssa` and `verify_gimple_in_cfg`.
- An independent in-compiler checker verifies the operation multiset, topological order, all scalar/vector dependencies, barriers, CC epoch, exact live intervals, fixed registers, alias last-use conditions, legal tuples, opcode availability and architecture hazards.
- Initially accept a transformation only if it strictly decreases peak pressure **or** turns a graph that lacks a materializable eight-register allocation into one with a verified tie/color certificate. Peak must never exceed eight and predicted makespan must not regress. After soak, allow otherwise-equal pressure for a strict latency win.
- Keep the existing post-reload hazard scheduler as final verification/repair; track NOP count, code size and automatic replay footprint as regression metrics.

Use a deterministic pressure-first heuristic before invoking MILP. Its ready-list priority should prefer staying at or below eight, killing the most operands, reducing pressure, then critical-path length, with source UID as a stable final tie-break. Skip the solver for comfortably low-pressure regions. Invoke it at seven/eight registers, after heuristic failure, or for a measured latency opportunity.

## Implementation sequence

### 1. Baseline and diagnostics

- Add the focused fixtures to the existing `tt/sfpi/rvtt.exp` harness.  That
  driver already executes the complete legacy `sfpi/*.C` directory; the
  test-count guard therefore verifies the full directory ran rather than
  relying on a new, potentially empty test glob.
- Capture late GIMPLE, IRA/reload dumps, live count and the current spill diagnostic.
- Add a dump-only DFG extractor and exact pressure oracle before introducing any scheduler.

Example testcase:

```cpp
namespace ckernel {
unsigned* instrn_buffer;
}

#include <sfpi.h>
using namespace sfpi;

sfpi_inline void welford_update(
    vFloat x, vFloat recip, vFloat& mean, vFloat& m2) {
    vFloat delta = x - mean;
    mean += delta * recip;
    vFloat delta2 = x - mean;
    m2 += delta * delta2;
}

void welford4() {
    vFloat x0 = l_reg[LRegs::LReg0];
    vFloat x1 = l_reg[LRegs::LReg1];
    vFloat x2 = l_reg[LRegs::LReg2];
    vFloat x3 = l_reg[LRegs::LReg3];
    vFloat mean = l_reg[LRegs::LReg4];
    vFloat m2 = l_reg[LRegs::LReg5];
    vFloat recip = l_reg[LRegs::LReg7];

    welford_update(x0, recip, mean, m2);
    welford_update(x1, recip, mean, m2);
    welford_update(x2, recip, mean, m2);
    welford_update(x3, recip, mean, m2);

    l_reg[LRegs::LReg4] = mean;
    l_reg[LRegs::LReg5] = m2;
}
```

Expected diagnostic/dump command:

```bash
SFPI_ROOT=/path/to/sfpi
CXX="$SFPI_ROOT/compiler/bin/riscv-tt-elf-g++"

"$CXX" \
  -mcpu=tt-wh-tensix -O2 \
  -I"$SFPI_ROOT/include" \
  -fno-exceptions -fno-rtti \
  -S welford-pressure-wh.C \
  -o /tmp/welford-pressure-wh.S \
  -fdump-tree-all -fdump-rtl-all \
  -fira-verbose=10
```

### 2. Solver-independent scheduler

- Create `gcc/gcc/config/riscv/tt/gimple-rvtt-lp-schedule.cc`.
- Separate region extraction, architecture metadata, model construction and solution application.
- Dump nodes, edges, live-ins/outs, legal aliases and original peak pressure.
- Implement and test the deterministic pressure-first list scheduler.
- Leave GIMPLE unchanged unless the independent checker accepts a strictly better result.

### 3. Integrate `lp_solve`

- Add optional configure detection, the default-off `-mtt-tensix-optimize-pressure-schedule` flag, and the separately gated `-mtt-tensix-pressure-schedule-use-milp` backend selection.
- Add deterministic node/iteration limits and discard any non-optimal incumbent. A wall timeout is only an emergency abort.
- On solver timeout, infeasibility, cap or absence, fall back to the independently checked deterministic list result. An unsupported or invalid region remains untouched, and disabling the pressure scheduler remains byte-identical to the unmodified compiler.
- Review LGPL and static/dynamic linkage requirements before release packaging.

Concrete integration points:

- New `gcc/gcc/config/riscv/tt/gimple-rvtt-lp-schedule.cc`.
- Add its object in `gcc/gcc/config/riscv/tt/t-riscv-tt`.
- Add `make_pass_rvtt_lp_schedule` in `gcc/gcc/config/riscv/tt/rvtt-protos.h`.
- Register it after `pass_rvtt_attrib` in `gcc/gcc/config/riscv/tt/rvtt-passes.def`.
- Add the opt-in target flag in `gcc/gcc/config/riscv/riscv.opt`.

`lp_solve` is a host library linked into `cc1`/`cc1plus`, not a target library. Adding an object to `t-riscv-tt` is insufficient. GCC also needs a guarded `configure.ac` option/check for `lp_lib.h` and a symbol, a config define/substitution, and host-side link injection into the appropriate backend/host libraries. Test bootstrap, cross-host packaging and solver-absent builds, and get legal review for LGPL dynamic/static distribution obligations. Never accidentally link a library from the target sysroot.

### 4. Joint scheduling and representable destructive ties

- Solve only capped, straight-line, unconditional allowlisted regions.
- Rewrite only selected ties that have exact target semantics and explicit machine-description support.
- Add ordinary destructive tied patterns rather than abusing `_lv`.
- Recompute the contracted interference graph and independently validate the certificate before mutation.
- Verify after expansion that all selected ties are present and after reload that no spill/fill was attempted.
- Keep the existing post-reload scheduler as a hazard verifier and repair pass.

If IRA still cannot reliably realize valid certified graphs, stop here and implement target IRA integration or a pre-reload local allocator. Do not paper over an allocation failure with solver fallback, because the original Welford path is itself the failing path.

### 5. Rematerialization

- Only after scheduling/ties are stable, add bounded duplication from a tiny positive whitelist of proven pure arithmetic.
- Let Welford choose between retaining `delta` and recomputing it.
- Penalize duplicated instructions while optimizing final architecture-specific makespan.

### 6. Compiler regression matrix

Positive tests:

- Welford success, independent-chain reorder, last-use tie, boundary live-in/live-out, and eight-versus-nine pressure.
- Forced timeout, infeasible model and solver-unavailable paths.
- Compile the same input repeatedly and in parallel; assembly and dumps must be identical.
- `-O1`, `-O2`, `-O3`, `-Os` and `-g` on Wormhole, Blackhole and QSR configurations.
- Randomized small DFGs checked against exhaustive scheduling/coloring where feasible.

Negative tests must prove pass-on assembly is identical to pass-off for:

- Nested `v_if`, CC push/pop/compare/encc, existing `_lv` and `sfpassign_lv`.
- SFPU/DST loads and stores, LREG/config reads and writes, dynamic immediate/synth, stochastic rounding and explicit NOP/replay.
- Inline asm, unknown calls, branches, PHIs, loops, EH, XTT64 and XTT128.
- Transpose, swap, LUT/config and hand-allocated TTI code.

Run both `scripts/build.sh --test-gcc` and `scripts/build.sh --test-tt` with a checking-enabled compiler. Diff final assembly across the SFPI/TT-LLK kernel corpus on all supported architectures. Track compile median/p95, solver invocations, model sizes, optimal/infeasible/fallback counts, code size, MAD/MOV/NOP counts and replay-buffer footprint.

### 7. TT-Metal and hardware comparison

Compare pass-off, pass-on and handwritten LLK on:

- Peak LREG occupancy.
- Spill/fill attempts.
- MAD/MOV/NOP count per row.
- Replay-buffer footprint.
- Device cycles per row and tile.
- Mean/M2 numerical agreement.
- Widths 1, 4, 32, 33 and multi-tile.
- Skewed inputs such as `randn() + 100`.
- Poisoned partial-tile padding.
- Reciprocal-LUT mode first.

Existing correctness coverage to extend:

- `~/workspace/tt-metal/tests/ttnn/unit_tests/operations/fused/test_layer_norm.py`
- `~/workspace/tt-metal/tests/ttnn/unit_tests/operations/fused/test_group_norm_DRAM.py`
- `~/workspace/tt-metal/models/demos/stable_diffusion_xl_base/vae/tests/pcc/test_welford_state_leak_regression.py`

Also cover log, rational Horner, addcmul, GELU, erfinv, remainder, sin/cos, power and EMA with pass-off/pass-on assembly and numerical comparisons. Poison inactive lanes so an incorrect `_lv` transformation cannot hide behind active-lane answers.

`~/workspace/tt-metal/tests/tt_metal/tt_metal/sfpi/test_sfpi.cpp` recursively exercises useful kernels, but currently skips both Blackhole and Wormhole. Resolve those skips or add a separate enabled fixture before treating it as the primary device regression gate. Include `tt_metal/tt-llk/tests/python_tests/test_sfpu_{unary,binary,ternary}.py`, reduction/softmax/stochastic-rounding suites, and `tests/tt_metal/tt_metal/jit_build/{test_compile_stress.cpp,compile_stress_ci.py}`.

## Success criteria

- The exact historical/production failing Welford context is captured as a regression, while the minimal `welford4` fixture continues to compile without SFPU spill/fill.
- Peak physical LREG use is at most eight.
- Solver-selected destructive ties are present in final RTL/assembly.
- Mean and M2 remain resident across row updates.
- No copies are introduced merely to canonicalize L0–L3 inputs.
- Floating-point association and FMA selection remain numerically valid.
- Generated device cycles are competitive with or better than the handwritten LLK.
- Flag-off compiler and assembly output are unchanged.
- Unsupported regions, solver absence, infeasibility, non-optimal termination and validation failures retain byte-identical GIMPLE.
- Negative legality tests remain byte-identical with the pass enabled.
- No compiler/TT-Metal regression on Wormhole, Blackhole or QSR, including numerical output, hazard checks, replay/code size and compile-time budgets.
- At least log and one independent-chain latency benchmark demonstrate that the benefit is not Welford-specific.

## Reproducible Linux VM workflow

The host workspace is macOS/arm64. SFPI's released compiler and the TT-Metal simulator test stack are Linux workflows, and a macOS craq-sim build produces Mach-O libraries that Linux cannot load. The working lane is the x86_64 Lima VM named `ttmetal-x86`; the older aarch64 VM named `ttmetal` currently has a virtual-disk I/O failure and is not used.

Host-side discovery and startup:

```sh
limactl list
limactl start ttmetal-x86
limactl shell ttmetal-x86 -- uname -a
```

The host workspace is mounted read/write at the same absolute path inside the VM:

```text
/Users/nkapre/workspace/sfpi
/Users/nkapre/workspace/tt-metal
/Users/nkapre/workspace/craq-sim
```

Do builds in VM-local storage under `/home/nkapre.guest`, not on the shared mount. This avoids slow 9p metadata traffic and keeps Linux build outputs separate from macOS outputs.

### 1. Install VM build prerequisites

```sh
limactl shell ttmetal-x86 -- bash -lc '
  sudo apt-get update &&
  sudo apt-get install -y \
    texinfo bison flex dejagnu libgmp-dev libmpfr-dev libmpc-dev
'
```

`texinfo` is required: without `makeinfo`, the binutils stage fails while generating `doc/bfd.info`. `dejagnu` supplies `runtest`; without it, the TT test wrapper can print a misleading success without executing the intended tests.

### 2. Stage and build the modified SFPI compiler

Initialize the source submodules once from the host checkout:

```sh
cd /Users/nkapre/workspace/sfpi
git submodule update --init gcc binutils newlib
```

Stage an isolated Linux source copy and build directory:

```sh
limactl shell ttmetal-x86 -- bash -lc '
  mkdir -p /home/nkapre.guest/sfpi-src &&
  rsync -a --delete \
    /Users/nkapre/workspace/sfpi/ \
    /home/nkapre.guest/sfpi-src/ &&
  cd /home/nkapre.guest/sfpi-src &&
  ./scripts/build.sh \
    --dir=/home/nkapre.guest/sfpi-lp-build \
    --checking
'
```

For an incremental retry after changing GCC target files:

```sh
limactl shell ttmetal-x86 -- bash -lc '
  cp /Users/nkapre/workspace/sfpi/gcc/gcc/config/riscv/tt/gimple-rvtt-lp-schedule.cc \
     /home/nkapre.guest/sfpi-src/gcc/gcc/config/riscv/tt/ &&
  make -C /home/nkapre.guest/sfpi-lp-build -j6 \
    2>&1 | tee -a /home/nkapre.guest/sfpi-lp-build/build-retry.log
'
```

The built toolchain is expected at:

```text
/home/nkapre.guest/sfpi-lp-build/sfpi/compiler/bin/riscv-tt-elf-g++
```

### Private checkpoint copies

The authenticated GitHub account has read-only access to the Tenstorrent upstream repositories. Private copies were therefore created for overnight checkpoints:

```text
git@github.com:nkapreTT/sfpi.git
git@github.com:nkapreTT/sfpi-gcc.git
branch: nkapre/welford
```

The local `sfpi-gcc` checkout remains shallow. The normal local
`nkapre/welford` branches retain their upstream-facing history, while the
private branches are a linear chain of explicit snapshot commits containing
the same trees. This avoids fetching millions of unrelated GCC objects merely
to bank an experimental branch. The SFPI snapshot replaces its GCC gitlink
with the matching private GCC snapshot. Its relative submodule URL
`../sfpi-gcc.git` lets an SSH clone under `nkapreTT` resolve that gitlink from
the sibling private repository.

Populate a clone made while the repositories were still empty with:

```sh
cd ~/sfpi
git fetch origin
git switch --track origin/nkapre/welford
git submodule update --init --recursive

cd ~/sfpi-gcc
git fetch origin
git switch --track origin/nkapre/welford
```

For later checkpoints, commit normally on each local `nkapre/welford` branch.
Create a private GCC snapshot whose tree is the normal GCC commit and whose
parent is the previous private snapshot, then push it first. Create the SFPI
snapshot from the normal root tree after replacing only the `gcc` gitlink with
that private GCC snapshot. Never stage the pre-existing local modification to
`gcc/testsuite/g++.target/riscv/tt/sfpi/dataformat-bh.C`.

### 3. Reproduce the current register spill

The minimal four-row recurrence is `welford-pressure-wh.C`; it succeeds on released SFPI 7.69.0. The current failing fixture is `welford-pressure-reorder-wh.C`: it adds one independent `x3 + bias` operation late in source order. That keeps eight boundary values live when the first Welford delta is created, so the released compiler attempts a ninth LREG and aborts.

```sh
limactl shell ttmetal-x86 -- bash -lc '
  CXX=/Users/nkapre/workspace/tt-metal/tt_metal/tt-llk/tests/sfpi/compiler/bin/riscv-tt-elf-g++
  INC=/Users/nkapre/workspace/tt-metal/tt_metal/tt-llk/tests/sfpi/include
  mkdir -p /tmp/welford-pressure-reorder
  "$CXX" \
    -mcpu=tt-wh-tensix -O2 -I"$INC" \
    -fno-exceptions -fno-rtti -S \
    /Users/nkapre/workspace/sfpi/gcc/gcc/testsuite/g++.target/riscv/tt/sfpi/welford-pressure-reorder-wh.C \
    -o /tmp/welford-pressure-reorder/release.S
'
```

Measured with SFPI 7.69.0/GCC 15.1.0, this reaches final RTL and fails with:

```text
internal compiler error: cannot store sfpu register (register spill)
```

This is a valid scheduling rescue: moving `folded = x3 + bias` before the recurrence kills two live inputs and creates one result, reducing peak pressure from nine to eight without reassociation or rematerialization.

The same released compiler succeeds when the fixture is compiled with `-DWELFORD_MANUAL_EARLY_FOLD`, which places only that add before the recurrence. The measured pair is therefore: late fold fails with the SFPU spill ICE; early fold exits zero and emits a 61-line assembly file. This isolates source scheduling as the deciding variable.

### 4. Compile pass-off and pass-on

Pass-off must preserve the compiler's prior behavior byte-for-byte:

```sh
MOD_CXX=/home/nkapre.guest/sfpi-lp-build/sfpi/compiler/bin/riscv-tt-elf-g++
SFPI_INC=/home/nkapre.guest/sfpi-lp-build/sfpi/include
SRC=/home/nkapre.guest/sfpi-src/gcc/gcc/testsuite/g++.target/riscv/tt/sfpi/welford-pressure-wh.C

"$MOD_CXX" -mcpu=tt-wh-tensix -O2 -I"$SFPI_INC" \
  -fno-exceptions -fno-rtti -S "$SRC" -o /tmp/welford-off.S
"$MOD_CXX" -mcpu=tt-wh-tensix -O2 -I"$SFPI_INC" \
  -fno-exceptions -fno-rtti -mtt-tensix-optimize-pressure-schedule \
  -fdump-tree-rvtt_lp_schedule -S "$SRC" -o /tmp/welford-on.S

# Invoke the optional MILP backend as a separate A/B:
"$MOD_CXX" -mcpu=tt-wh-tensix -O2 -I"$SFPI_INC" \
  -fno-exceptions -fno-rtti -mtt-tensix-optimize-pressure-schedule \
  -mtt-tensix-pressure-schedule-use-milp \
  -fdump-tree-rvtt_lp_schedule -fdump-rtl-rvtt_lp_alloc \
  -S "$SRC" -o /tmp/welford-milp.S
```

The failing rescue fixture uses the same commands with `welford-pressure-reorder-wh.C`. Acceptance requires:

```sh
cmp /tmp/welford-off.S /tmp/welford-on.S       # low-pressure fixture
grep 'old-peak=9 new-peak=8 applied=yes' /tmp/*.rvtt_lp_schedule
! grep -q 'register spill' /tmp/welford-on.log
```

The overnight checkpoint used only the default-off pressure oracle and deterministic list scheduler. The current branch additionally contains the separately gated, schedule-only `lp_solve` M3a experiment described above. Both paths must pass the independent pressure/liveness certificate; neither is yet the M2 physical-allocation guarantee.

### 5. Build Linux craq-sim libraries

Do not reuse the host `src/_out/release_{wh,bh}/libttsim.so`; those are Mach-O arm64 libraries. Stage a VM-local source copy while excluding large unrelated artifacts, then build native ELF x86_64 libraries:

```sh
limactl shell ttmetal-x86 -- bash -lc '
  mkdir -p /home/nkapre.guest/craq-sim-linux &&
  rsync -a \
    --exclude=.git --exclude="**/_out" --exclude=.venv \
    --exclude=formal --exclude=docs --exclude=.rtl2ttsim \
    --exclude="*.xml" --exclude=plots --exclude=runs \
    --exclude=blaze_llk_perf \
    /Users/nkapre/workspace/craq-sim/ \
    /home/nkapre.guest/craq-sim-linux/ &&
  cd /home/nkapre.guest/craq-sim-linux &&
  patch -p1 < /Users/nkapre/workspace/sfpi/scripts/craq-sim-bh-llk-smoke.patch &&
  ./make.py --env TTSIM_LTO=0 \
    src/_out/release_wh/libttsim.so \
    src/_out/release_bh/libttsim.so
'
```

Verify the result:

```sh
limactl shell ttmetal-x86 -- file \
  /home/nkapre.guest/craq-sim-linux/src/_out/release_wh/libttsim.so \
  /home/nkapre.guest/craq-sim-linux/src/_out/release_bh/libttsim.so
```

Both must report `ELF 64-bit LSB shared object, x86-64`.

### 6. Create the TT-LLK Python environment

The checked-in setup wrapper insists on Python 3.10, while this VM has Python 3.12. The pinned dependencies work under 3.12 when installed directly:

```sh
limactl shell ttmetal-x86 -- bash -lc '
  python3 -m venv /home/nkapre.guest/llk-venv &&
  /home/nkapre.guest/llk-venv/bin/pip install -q --upgrade pip uv &&
  /home/nkapre.guest/llk-venv/bin/uv pip install \
    --python /home/nkapre.guest/llk-venv/bin/python \
    --index-strategy unsafe-best-match --no-cache-dir \
    -r /Users/nkapre/workspace/tt-metal/tt_metal/tt-llk/tests/requirements.txt
'
```

### 7. Stage simulator descriptors

TT-Metal requires `soc_descriptor.yaml` next to each simulator library:

```sh
limactl shell ttmetal-x86 -- bash -lc '
  mkdir -p /home/nkapre.guest/sim-stage/wh /home/nkapre.guest/sim-stage/bh
  cp /home/nkapre.guest/craq-sim-linux/src/_out/release_wh/libttsim.so \
     /home/nkapre.guest/sim-stage/wh/libttsim.so
  cp /Users/nkapre/workspace/tt-metal/tt_metal/soc_descriptors/wormhole_b0_80_arch.yaml \
     /home/nkapre.guest/sim-stage/wh/soc_descriptor.yaml
  cp /home/nkapre.guest/craq-sim-linux/src/_out/release_bh/libttsim.so \
     /home/nkapre.guest/sim-stage/bh/libttsim.so
  cp /Users/nkapre/workspace/tt-metal/tt_metal/soc_descriptors/blackhole_140_arch.yaml \
     /home/nkapre.guest/sim-stage/bh/soc_descriptor.yaml
'
```

### 8. Run TT-Metal LLK tests on craq-sim

The simulator maps a large virtual address range and `pytest-forked` clones the process for each test. On this no-swap VM, Linux's default overcommit heuristic rejects the fork even when about 10 GiB is available. Enable overcommit in the disposable VM:

```sh
limactl shell ttmetal-x86 -- sudo sysctl -w vm.overcommit_memory=1
```

Run a focused recurrent-SFPU correctness smoke test on Wormhole:

```sh
limactl shell ttmetal-x86 -- bash -lc '
  export PATH=/home/nkapre.guest/llk-venv/bin:$PATH
  export LLK_HOME=/home/nkapre.guest/tt-metal-lp/tt_metal/tt-llk
  export TT_METAL_SIMULATOR=/home/nkapre.guest/sim-stage/wh/libttsim.so
  export TT_METAL_DISABLE_SFPLOADMACRO=1
  cd /home/nkapre.guest/tt-metal-lp/tt_metal/tt-llk/tests
  ./run_ttsim_regression.sh \
    --architecture wormhole --workers 0 --timeout 180 \
    test_sfpu_ema.py
'
```

For the pinned craq-sim checkout, apply `scripts/craq-sim-bh-llk-smoke.patch` before building the Blackhole library. The unpatched simulator first stops on debug offset `0x228`, the BH TRISC0 reset-PC register, and then on valid `SETC16` registers 23/25. The patch exposes state already retained by the simulator and adds the generated thread-register union members and write cases. It is only a test-harness compatibility shim; do not infer that hardware permits reset-PC reads.

The measured pass-off results are Wormhole `3 passed in 32.63s` and Blackhole `3 passed in 31.74s`, each with zero crashed tests.

For pass-on end-to-end testing, create a tiny isolated SFPI overlay so the shared TT-Metal checkout and installed compiler remain unchanged:

```sh
limactl shell ttmetal-x86 -- bash -lc '
  mkdir -p /home/nkapre.guest/sfpi-lp-on/compiler/bin
  ln -sfn /home/nkapre.guest/sfpi-lp-build/sfpi/include \
    /home/nkapre.guest/sfpi-lp-on/include
  cp /Users/nkapre/workspace/sfpi/scripts/riscv-tt-elf-g++-lp-wrapper \
    /home/nkapre.guest/sfpi-lp-on/compiler/bin/riscv-tt-elf-g++
  chmod +x /home/nkapre.guest/sfpi-lp-on/compiler/bin/riscv-tt-elf-g++
  ln -sfn /home/nkapre.guest/sfpi-lp-on \
    /home/nkapre.guest/tt-metal-lp/tt_metal/tt-llk/tests/sfpi
'
```

Run the same simulator command with the compiler wrapper enabled:

```sh
export SFPI_REAL_CXX=/home/nkapre.guest/sfpi-lp-build/sfpi/compiler/bin/riscv-tt-elf-g++
export SFPI_USE_MILP=0    # list scheduler
# Repeat with SFPI_USE_MILP=1 for the solver-linked MILP A/B.
```

The measured pass-on results are Wormhole `3 passed in 31.14s` and Blackhole `3 passed in 30.55s`, again with zero crashed tests. These elapsed times are not a useful performance comparison: the fixtures are handwritten EMA, simulator startup dominates, and the runs were intended only as numerical/non-crash regression gates.

### 9. Compiler and regression gates

Run the checked-in focused validator first. It exercises ordinary vFloat C++
for pass-off, minimal no-op, genuine 9-to-8 Welford-shaped rescue, a separate
generic fused-DAG rescue, manual scheduling control, predication rejection,
and deterministic assembly on both architectures:

```sh
cd /home/nkapre.guest/sfpi-src
./scripts/validate-sfpu-pressure-scheduler.sh \
  /home/nkapre.guest/sfpi-lp-build/sfpi \
  /home/nkapre.guest/sfpu-pressure-validation

SCHEDULER_REQUIRE_MILP=1 SCHEDULER_DETERMINISM_RUNS=20 \
  ./scripts/validate-sfpu-pressure-scheduler.sh \
  /home/nkapre.guest/sfpi-lp-build/sfpi \
  /home/nkapre.guest/sfpu-pressure-milp-validation
```

After the focused fixtures pass:

```sh
cd /home/nkapre.guest/sfpi-src
SFPI_WITH_LP_SOLVE=yes \
  ./scripts/build.sh --dir=/home/nkapre.guest/sfpi-lp-build --test-tt
```

That is the checked-in SFPI CI test lane. A broad `--test-gcc` invocation is a
separate base-versus-patch differential: run both revisions in identical
containers and compare their `.sum` files rather than requiring an absolute
zero-failure simulator result.

For a clean Ubuntu 22.04 Runpod, clone with SSH so the relative private GCC
submodule resolves correctly, then build and run both gates:

```sh
SUDO=
if [[ $(id -u) -ne 0 ]]; then SUDO=sudo; fi
$SUDO apt-get update
$SUDO apt-get install -y \
  autoconf automake bison dejagnu expect flex gawk python3 texinfo \
  patchutils ruby wget libexpat1-dev libgmp-dev libmpc-dev libmpfr-dev \
  gcc g++ liblpsolve55-dev libsuitesparse-dev

git clone --branch nkapre/welford --recurse-submodules \
  git@github.com:nkapreTT/sfpi.git
cd sfpi

SFPI_WITH_LP_SOLVE=yes scripts/build.sh --tt-built --checking --small
SFPI_WITH_LP_SOLVE=yes scripts/build.sh --dejagnu --small
SFPI_WITH_LP_SOLVE=yes scripts/build.sh --test-tt

SCHEDULER_REQUIRE_MILP=1 SCHEDULER_DETERMINISM_RUNS=20 \
  scripts/validate-sfpu-pressure-scheduler.sh \
  build build/sfpu-pressure-validation
```

Archive `build/tests`, `build/sfpu-pressure-validation`, the final assembly,
and both scheduler dumps. This Runpod result is reproducibility evidence; the
authoritative CI label still requires a Tenstorrent-org branch or pull
request.

Then run at least the following focused LLK files with pass off and on, on both Wormhole and Blackhole:

```text
test_sfpu_ema.py
test_sfpu_unary.py
test_sfpu_binary.py
test_sfpu_ternary.py
test_sfpu_reduce.py
test_sfpu_softmax_k.py
```

Record JUnit XML/HTML, final assembly hashes, compiler dump pressure, MAD/MOV/NOP counts and any simulator numerical differences. Keep the scheduler default off until unsupported/predicated kernels are proven byte-identical and the complete corpus is clean.

## Architecture caveat

Current Wormhole and Blackhole handwritten schedules disagree on hazard NOPs, and an unmerged change reportedly argues that Blackhole’s six-instruction sequence is unsafe. Latencies must therefore be architecture-configured and validated on hardware. The current Blackhole source should not be treated as unquestionable truth.

## Linux handoff: a separate Tensix/TTI replay optimization pass

This is a standalone assignment for another compiler agent. It must not be folded into the Welford pressure pass: replay formation runs after register allocation and optimizes repeated final instruction streams, while the pressure pass runs in GIMPLE before expansion to keep allocation below eight LREGs. Keeping separate flags and dumps makes regressions bisectable.

### Agent brief

> On an x86_64 Linux machine, build the SFPI toolchain with checking enabled and investigate an opt-in Tensix replay optimization pass. First characterize the existing `rtl-rvtt-replay.cc` pass and create dump-only tests; do not begin by replacing it. The current pass already finds repeated post-reload Tensix RTL sequences, respects explicit replay-buffer reservations, greedily chooses profitable candidates, and emits `rvtt_ttreplay_int`. The useful new work is either (a) conservative replay formation that exposes identical sequences separated by safely movable scalar RTL, as described by FIXME PR 36496, or (b) an exact small-instance selector that improves the existing greedy replay-buffer/overlap choice. Implement one measured improvement behind a default-off flag, independently validate every rewrite, preserve pass-off assembly byte-for-byte, and test Wormhole, Blackhole, and Quasar-specific replay behavior. Do not change predication semantics, instruction hazards, explicit replay, or architectural state ordering.

### Relevant source map

```text
gcc/gcc/config/riscv/tt/rtl-rvtt-replay.cc
    existing discovery, hashing, overlap triage, greedy selection, replacement
gcc/gcc/config/riscv/tt/rtl-rvtt-schedule.cc
    post-allocation Tensix RAW-hazard/NOP repair
gcc/gcc/config/riscv/tt/rvtt-passes.def
    rvtt_schedule before pass_postreload; rvtt_replay after pass_postreload
gcc/gcc/config/riscv/tt/rvtt.md
    rvtt_ttreplay expansion and rvtt_ttreplay_int instruction
gcc/gcc/config/riscv/tt/rvtt-insn.def
    ttreplay builtin metadata
gcc/gcc/config/riscv/riscv.opt
    -mtt-tensix-optimize-replay (currently default on), buffer size, QSR fix
gcc/gcc/config/riscv/tt/t-riscv-tt
gcc/gcc/config/riscv/tt/rvtt-protos.h
```

The existing replay implementation has several facts that the new work must preserve:

- Minimum automatic sequence length is four non-empty Tensix instructions.
- Discovery is per basic block and currently stops a candidate at non-Tensix instructions.
- Sequence identity is final recognized RTL plus the generation of synthesized scalar inputs; matching opcodes with different live scalar generations is unsafe.
- Explicit fixed/variable replay captures reserve buffer spans globally. Automatic replay may use only the remaining spans.
- Candidate occurrences may overlap; selecting one sequence invalidates conflicting occurrences.
- Wormhole/Blackhole can execute the first sequence while capturing. The Quasar workaround cannot, so its saving and replacement rules differ.
- The hazard pass currently precedes automatic replay. Any new instruction-moving pass must run before hazard repair, or a final hazard verifier/repair must run after it. Never create a new RAW hazard after the last repair point.

### Linux setup and baseline

```sh
git clone --recurse-submodules git@github.com:tenstorrent/sfpi.git
cd sfpi
sudo apt-get update
sudo apt-get install -y \
  build-essential git texinfo bison flex \
  libgmp-dev libmpfr-dev libmpc-dev

./scripts/build.sh --dir="$PWD/../sfpi-replay-build" --checking
REPLAY_CXX="$PWD/../sfpi-replay-build/sfpi/compiler/bin/riscv-tt-elf-g++"
REPLAY_INC="$PWD/../sfpi-replay-build/sfpi/include"
```

Before editing, compile replay-bearing kernels for each architecture both with automatic replay enabled and disabled. Capture assembly and the existing RTL replay dump; verify deterministic output across at least three runs.

```sh
"$REPLAY_CXX" -mcpu=tt-wh-tensix -O2 -I"$REPLAY_INC" \
  -S replay_fixture.C -fdump-rtl-rvtt_replay -o wh-replay-on.S
"$REPLAY_CXX" -mcpu=tt-wh-tensix -O2 -I"$REPLAY_INC" \
  -mno-tt-tensix-optimize-replay -S replay_fixture.C -o wh-replay-off.S

# Repeat with -mcpu=tt-bh-tensix and the supported Quasar spelling in this tree.
```

If `-fdump-rtl-rvtt_replay` differs in this GCC build, use `-fdump-rtl-all` once and take the exact generated dump suffix from the output. Record the compiler commit, command line, assembly hashes, replay captures/playbacks, instruction count, and code size.

### Stage 1: analyzer and fixtures

Add focused compiler fixtures before transformation code. Each fixture should use enough repeated operations to cross the four-instruction profitability threshold and should scan final assembly plus the replay dump.

Required positive cases:

1. Two and four identical unrolled SFPU sequences that already replay today.
2. A profitable candidate larger than one buffer slot choice, exposing the greedy selector's loss if selection is the target.
3. Identical Tensix sequences separated by a scalar instruction that is provably movable, exposing PR 36496 if formation is the target.
4. Explicit replay occupying a subrange, with automatic replay correctly allocated around it.
5. Architecture fixtures for Wormhole, Blackhole, and Quasar capture semantics.

Required negative cases:

- Same opcode sequence with different scalar-value generations.
- Predication/CC push-pop/compare epochs, stochastic rounding, config/LREG reads and writes, load/store, transpose, explicit NOP, inline asm, unknown calls, branches, and variable replay captures.
- A scalar instruction whose destination feeds the candidate, whose source is overwritten by it, which can trap, or whose memory/volatile effects prevent movement.
- Overlapping occurrences and candidates that exactly fill or exceed the available buffer.
- A schedule where moving a scalar instruction would remove a latency gap and require a new SFPU NOP.

The first patch should be dump-only. Emit stable, machine-readable lines for candidate span, length, occurrence count, buffer demand, predicted saving, conflicts, rejection reason, and selected/not-selected status. Put deterministic caps on basic-block length and candidate count.

### Stage 2A: conservative replay formation

Choose this only when the missed replay is demonstrably caused by intervening scalar RTL. Work on final RTL where sequence identity is real, but place formation before `rvtt_schedule` so the existing hazard repair sees the final order. The move is legal only when an independent RTL dataflow check proves all of the following:

- The instruction is non-Tensix, non-memory, non-volatile, non-throwing, and has no implicit architectural state effects.
- Its complete hard/pseudo register use/def sets have no true, anti, or output dependence with every crossed instruction, including condition codes and synthesized-opcode state.
- It does not cross labels, notes that delimit EH/CFI semantics, calls, asm, barriers, explicit replay, basic-block boundaries, or debug-location boundaries that cannot be preserved.
- Moving it cannot change a user-visible exception, memory order, or predicated-lane behavior.

Construct and validate the complete new order before mutating RTL. After mutation call the appropriate DF rescan/verification hooks, preserve locations, and let `rvtt_schedule` repair hazards. Reject the region on any uncertainty. Do not move Tensix operations in the first version.

### Stage 2B: exact small-instance replay selection

Choose this instead when a fixture proves the existing `pick_replay` greedy choice loses code-size/cycle savings. Reuse the existing candidate discovery; do not create a second incompatible hasher. Model each candidate occurrence and buffer placement with integer/binary variables or a deterministic dynamic program:

- selected occurrences cannot overlap deleted RTL;
- each captured sequence consumes one contiguous available replay-buffer span;
- different selected sequences cannot overlap buffer storage unless lifetime reuse is explicitly modeled and proven;
- explicit replay spans are unavailable;
- savings use the architecture-specific capture rule already in `replace_sequence`;
- candidate selection and placement have a canonical source-order tie-break.

Use exact integer coefficients and deterministic node/iteration caps. Accept only a proven optimum; timeout, infeasible, solver unavailable, or validator failure must fall back to the existing greedy pass unchanged. Independently recompute overlap, storage, saving, capture location, and all emitted replay operands before replacing RTL.

### Flags and pass plumbing

Do not repurpose the existing default-on `-mtt-tensix-optimize-replay`. Add a separate default-off experimental flag, for example:

```text
-mtt-tensix-optimize-replay-form       # Stage 2A
-mtt-tensix-optimize-replay-select     # Stage 2B
```

The new pass/dump should be independently switchable. Add its object to `t-riscv-tt`, declaration to `rvtt-protos.h`, and insertion to `rvtt-passes.def`. If using an external solver, make it a host build dependency detected by `configure`, support solver-absent builds, and do not accidentally link a target-sysroot library into `cc1`/`cc1plus`.

### Acceptance gates

- With the new flag off, compiler assembly and existing replay dumps are byte-identical to baseline.
- With the flag on but no proven opportunity, RTL and assembly are byte-identical.
- Every accepted transform has a dump certificate and passes an independent legality/profitability validator.
- Existing automatic replay results never lose predicted savings; code size, capture/playback count, and hazard NOP count do not regress.
- Compile each positive and negative fixture three times and in parallel; hashes must match.
- Run `./scripts/build.sh --dir=... --test-gcc` and `--test-tt` with checking enabled.
- Run TT-LLK replay/SFPU kernels on craq-sim for Wormhole and Blackhole, plus Quasar when supported; then run device cycle and numerical tests because simulator coverage is not a hardware-latency proof.
- Test explicit replay mixed with automatic replay, buffer sizes 1 through 32 at boundary values, `-O1/-O2/-O3/-Os`, LTO if supported, and debug builds.
- Report compile-time median/p95, candidate count, optimizer time, replay-buffer occupancy, static instructions saved, NOP delta, simulator cycles, and device cycles.

The deliverable is a small reviewable branch with fixtures first, analyzer second, one default-off transformation third, and a Markdown report containing exact commands and measured before/after results. A patch that merely makes more sequences look textually identical without proving scalar generations, hazards, explicit-buffer reservations, and architecture capture semantics is not acceptable.

## SFPU architecture and LLK productization audit

This section records the 2026-08-14 architecture and TT-Metal agent storm. It
extends the Welford pressure work into a general compiler roadmap. The audit
used the official `tenstorrent/tt-isa-documentation` repository at commit
[`860a84abf7cd68098c444656798bd79a261f627e`](https://github.com/tenstorrent/tt-isa-documentation/tree/860a84abf7cd68098c444656798bd79a261f627e)
and the local TT-Metal checkout at `c65666dcef0`. The ISA repository currently
documents Wormhole B0 and Blackhole A0 only and explicitly warns that behavior
must not be transferred between them without validation. It identifies
`ttsim` as the golden ISA reference. Quasar/QSR therefore remains separately
gated until an authoritative model and hardware tests exist.

### Architectural model

The SFPU is a 32-lane SIMD engine with only eight ordinary allocatable
registers, LREG0 through LREG7. LREG8 is a fixed 0.8373 constant, LREG9 is
zero, LREG10 is one, LREG11 through LREG14 are configuration-written
constants, and LREG15 contains twice the lane index. SFPI reserves LREG11 for
negative one. LREG16 is a special transient register that can be written and
read only by operations launched through `SFPLOADMACRO`. See the official
[LREG description](https://github.com/tenstorrent/tt-isa-documentation/blob/860a84abf7cd68098c444656798bd79a261f627e/WormholeB0/TensixTile/TensixCoprocessor/LReg.md#L3-L16).

LREGs and `Dst` are shared backend state rather than private state for each
Tensix thread. SFPU loads and stores also consume implicit per-thread RWCs and
address-modifier state. A correct scheduler therefore needs more than an SSA
graph of vFloat values: it must model physical LREGs, condition state,
configuration state, `Dst` aliases, RWCs, address modifiers, replay slots,
load-macro slots, and cross-thread ownership barriers.

The frontend order is:

```text
RISC-V Tensix instruction pushes
        -> MOP expander
        -> Replay expander
        -> Wait Gate / synchronization
        -> parallel Tensix backend units
        -> SFPU load | simple | MAD | round | store sub-units
```

Replay and MOP reduce the number of instructions RISC-V must push. Their
expanders still emit at most one Tensix instruction per cycle, so they mainly
improve code size and frontend issue availability. `SFPLOADMACRO` is
fundamentally different: it is the only mechanism that can drive multiple
SFPU sub-units in one cycle, potentially issuing a load plus one Simple, MAD,
Round, and Store operation. See the official
[Vector Unit](https://github.com/tenstorrent/tt-isa-documentation/blob/860a84abf7cd68098c444656798bd79a261f627e/WormholeB0/TensixTile/TensixCoprocessor/VectorUnit.md#L97-L99)
and
[`SFPLOADMACRO`](https://github.com/tenstorrent/tt-isa-documentation/blob/860a84abf7cd68098c444656798bd79a261f627e/WormholeB0/TensixTile/TensixCoprocessor/SFPLOADMACRO.md#L1-L13)
documentation.

FP add, multiply, MAD, and LUT-family operations have two-cycle result
latency. On Wormhole, software must put an unrelated instruction or a NOP in
the producer/consumer gap. Blackhole normally scoreboards these dependencies,
but the official ISA lists missed and false dependencies for several AND, OR,
integer-add, shift, config, swap, and shuffle modes. Blackhole automatic
stalling also does not apply to operations launched by `SFPLOADMACRO`. A
single shared latency table is therefore not valid across architectures. See
[Wormhole SFPMAD scheduling](https://github.com/tenstorrent/tt-isa-documentation/blob/860a84abf7cd68098c444656798bd79a261f627e/WormholeB0/TensixTile/TensixCoprocessor/SFPMAD.md#L66-L72)
and
[Blackhole SFPMAD scheduling](https://github.com/tenstorrent/tt-isa-documentation/blob/860a84abf7cd68098c444656798bd79a261f627e/BlackholeA0/TensixTile/TensixCoprocessor/SFPMAD.md#L68-L86).

### Corpus evidence beyond Welford

The modern Wormhole, Blackhole, and Quasar SFPU trees contain dozens of
handwritten instances of the same compiler problems:

- 26 files record replay sequences and 31 files execute replay sequences.
- 21 files contain 326 references involving `SFPLOADMACRO`.
- 36 files contain 296 explicit `TTI_SFPNOP` references.
- 25 files contain 246 `SFPTRANSP` references.
- 21 files explicitly discuss pressure, spills, reloads, or reload-budget
  compiler failures.
- The non-SFPU LLK library contains 61 files using `ckernel_template`, showing
  that MOP/replay programming is pervasive pack/unpack/math infrastructure,
  not a Welford special case.

Representative examples include:

- Wormhole reduction interleaves independent `SFPSHFT2` and `SFPSWAP`
  chains to fill two-cycle latency slots:
  `tt-metal/tt_metal/hw/ckernels/wormhole_b0/metal/llk_api/llk_sfpu/ckernel_sfpu_reduce.h`.
- `addcmul` processes two rows together, issuing both multiplies before both
  MADs to obtain a NOP-free pipeline:
  `tt-metal/tt_metal/hw/ckernels/wormhole_b0/metal/llk_api/llk_sfpu/ckernel_sfpu_addcmul.h`.
- Tanh evaluates two datums in lockstep and explicitly notes that one more
  live vector spills:
  `tt-metal/tt_metal/hw/ckernels/wormhole_b0/metal/llk_api/llk_sfpu/ckernel_sfpu_tanh.h`.
- Integer remainder recomputes divisor chunks to reduce pressure; erfinv and
  integer division reload values from `Dst`; trigonometry deliberately
  recomputes `abs(x)` and `x*x` to avoid the fatal reload budget.
- Binary broadcast fills four serial shuffle latency slots with four
  independent `Dst` loads, then pipelines four independent arithmetic
  operations before draining through stores:
  `tt-metal/tt_metal/tt-llk/tt_llk_wormhole_b0/common/inc/sfpu/ckernel_sfpu_binary_bcast.h`.
- Typecast, signbit, max/min, reciprocal, integer multiply, reduction, GELU,
  and exp contain handwritten `SFPLOADMACRO` pipelines with plain-loop
  fallbacks. These fallbacks are a ready-made differential corpus for a future
  compiler lowering.

These examples prove that the opportunity is broader than Welford. The same
shared optimizer can help online reductions, log/exp/tanh, Horner chains,
addcmul, integer division and remainder, typecasts, reductions, and fused
eltwise graphs.

### Productization matrix

| Priority | Optimization | Compiler layer | Current state | Decision |
|---:|---|---|---|---|
| 1 | Physical LREG allocation and destructive coalescing | Final pre-IRA RTL | Audit only | Build now |
| 1 | Latency-aware scheduling and useful NOP filling | Post-allocation RTL | NOP insertion only | Build now |
| 2 | Replay verification and exact slot packing | Postreload | Generic pass already default on | Harden now |
| 2 | Narrow rematerialization and recomputation | Late GIMPLE or pre-IRA RTL | Not target-directed | Build narrowly |
| 2 | Bounded cross-row software pipelining | Loop GIMPLE plus machine validation | Handwritten only | Build after explicit `Dst`/RWC state |
| 2 | Config, constant, LUT, and address-modifier state tracking | Shared target model | Fragmented implicit state | Prerequisite |
| 3 | `SFPLOADMACRO` formation | Post-allocation, before hazard repair | Raw encodings only | Annotated MVP |
| 3 | Transpose/layout lowering | Layout IR and post-allocation lowering | Handwritten only | Prototype |
| 4 | MOP synthesis | Separate LLK/kernel schedule IR | Explicit template library | Separate project |
| -- | Implicit `Dst` spilling | Pre-IRA | No scratch ABI | No-go without an ownership contract |

### Shared machine model

The central product should be one architecture-specific operation and state
descriptor consumed by allocation, scheduling, replay, and macro formation.
For each selected Tensix/SFPU operation it should record:

- explicit and implicit input/output LREGs;
- legal destructive destination/source overlaps;
- fixed-register and constant-register requirements;
- result latency and sub-unit occupancy per architecture;
- Blackhole scoreboard coverage and known scoreboard bugs;
- condition-code, flag-stack, PRNG, config, LUT, RWC, address-modifier, and
  `Dst` state effects;
- whether the operation is movable, duplicable, replayable, or macro-eligible;
- whether it can execute under predication and how inactive lanes are
  preserved;
- the final exact instruction fingerprint used by replay validation.

Unknown operations and unknown state ownership must be hard barriers. Every
transform should build and validate a complete certificate before mutating
GIMPLE or RTL.

### Physical LREG allocation

The current pressure scheduler can shorten SSA live ranges, but it cannot
force IRA to implement a chosen physical coloring. The pre-IRA audit still
reports `colorability=unchecked`, and the checked-in list-missed MILP fixture
demonstrates the consequence: a valid eight-live GIMPLE schedule can still
spill after expansion.

The production allocator must operate on the actual final pre-IRA RTL island:

1. Build the physical interference and recognizer-constraint graph.
2. Include fixed LREGs, existing semantic `_lv` ties, ordinary legal
   destructive overlaps, and boundary precoloring.
3. Solve and independently validate eight-colorability.
4. Atomically substitute the certified island's pseudos with hard LREGs.
5. Rescan dataflow and require every resulting instruction to remain
   recognized.
6. Enter IRA with no SFPU pseudos remaining in the certified island.
7. Leave the entire island untouched on any failure.

This is the first point at which the compiler can honestly guarantee that an
accepted region cannot produce `BADLOAD`/`BADSTORE` or a fatal SFPU spill.

### Latency-aware scheduling and bounded row pipelining

The current `rtl-rvtt-schedule` pass does not reorder work. It scans the final
stream and conditionally inserts one hazard NOP. A new target scheduler should
instead fill exposed latency with independent loads, constants, comparisons,
arithmetic from another chain, or work from an adjacent row. The old pass
should remain as a final verifier and conservative repair.

The scheduler should optimize lexicographically:

1. remain physically colorable within eight LREGs;
2. preserve exact semantics and architectural state;
3. minimize critical path and exposed stall cycles;
4. minimize moves and live area;
5. prefer identical physical encodings across unrolled rows for replay;
6. consider code size and replay-buffer occupancy.

A bounded loop transformation can search row unroll factors 1, 2, 4, and 8,
using the same physical-register and latency validator. It must not move
today's volatile `sfpload`/`sfpstore` builtins until `Dst`, RWC, address-mode,
and alias state are explicit tokens in the IR.

### Replay: extend the existing compiler pass

Replay is already a real, default-enabled generic compiler optimization.
`rtl-rvtt-replay.cc` runs after register allocation and hazard repair, finds
identical final Tensix sequences, reserves explicit user spans, greedily packs
the remaining 32-entry buffer, emits capture/playback, and deletes repeated
copies. The ordinary unrolled builtin loop in
`gcc/gcc/testsuite/g++.target/riscv/tt/tensix/replay-34602-wh.C` already proves
compiler-generated replay.

The hardware replay buffer is per thread and circular. It can record and
optionally execute a sequence or expand an earlier recording at one
instruction per cycle with no mode-transition penalty. See the official
[`REPLAY` model](https://github.com/tenstorrent/tt-isa-documentation/blob/860a84abf7cd68098c444656798bd79a261f627e/WormholeB0/TensixTile/TensixCoprocessor/REPLAY.md#L17-L50).

The next replay work is therefore hardening rather than a new pass:

- independently verify the captured and expanded instruction traces;
- strengthen scalar-generation fingerprints;
- add exact dynamic programming or MILP for the fixed 32-slot packing problem;
- reuse buffer slots whose live ranges do not overlap;
- make physical allocation give isomorphic unrolled regions identical LREG
  assignments when feasibility and latency are unchanged;
- add generated-vFloat full-tile Welford, exp, quant, rand, typecast, and
  reduction tests;
- measure static code size and RISC-V issue availability separately from SFPU
  backend cycles.

Replay formation must remain a final-machine-stream transformation. GIMPLE
does not yet know exact instruction words, final physical LREGs, synthesized
scalar generations, address-counter effects, or final synchronization pushes.

#### Why `tt-polynomial-fitter` still needs manual replay

The existing pass is useful, but it solves a much narrower problem than the
manual TTI paths in `tt-polynomial-fitter`. It is an exact post-register-
allocation repeated-sequence compressor, not a loop recognizer, software
pipeline, register allocator, or symbolic replay scheduler.

In particular, `rtl-rvtt-replay.cc`:

- sees only the final physical Tensix instruction stream in one basic block;
- requires at least four identical replayable instructions to occur more than
  once;
- compares the allocated LREG operands and the generation of synthesized
  scalar operands;
- ends a candidate at a non-Tensix instruction, inline assembly, or another
  state boundary; and
- runs after register allocation and hazard-NOP insertion.

Consequently, it works when unrolling has already produced byte-identical
copies. `replay-34602-wh.C` is the positive proof: a fully unrolled ordinary
builtin sequence becomes one capture and seven playbacks. It does not turn a
remaining RISC-V loop backedge into SFPU replay, make partially unrolled
iterations identical, or canonicalize iterations modulo LREG renaming. An
address increment, synthesized opcode, scalar loop update, different physical
register assignment, constant materialization, or predication/setup operation
can prevent or shorten a match. The compiler option is default-on, and neither
`tt-polynomial-fitter` nor the inspected `tt-metal` sources explicitly disable
it; lack of a useful match is therefore generally a stream-shape limitation,
not an omitted `-mtt-tensix-optimize-replay` flag.

More importantly, automatic replay only compresses the body GCC already
emitted. If that body reloads constants, contains exposed MAD hazards, walks
`Dst` using scalar instructions, or uses an unnecessarily expensive register
layout, replay faithfully repeats all of that overhead. It can produce replay
instructions without producing the performance of a hand-designed replay
kernel.

The polynomial fitter's manual lowering is a compound optimization:

1. select a legal algorithm-specific body shape;
2. choose one- or two-row interleaving from the eight-LREG pressure budget;
3. assign exact physical LREGs and pin coefficients outside the repeated body;
4. schedule independent MAD chains to fill result-latency gaps;
5. use `ADDR_MOD` to advance `Dst` without scalar loop machinery;
6. fit the result into the 32 replay slots, then explicitly record and play it;
7. restore shared D-RWC state with `SETRWC SET_D` before the next tile.

The three polynomial shapes make the distinction concrete in
`deployment/generic_lut_activation/kernels/compute/piecewise_generic.cpp` in
the fitter repository. Plain Horner uses a two-element interleave so every MAD
gap is useful work. Signed-absolute polynomials require three live values per
lane, so two-way interleaving cannot fit in eight LREGs and the one-element
body uses explicit gaps. The even-parity body reloads coefficients inside the
capture when a pinned LREG would be overwritten by later iterations. Their
bodies are checked against the 32-slot capacity before selection.

The shared `tti_replay.h` also encodes obligations that the current replay
matcher neither invents nor verifies: silicon-derived producer/consumer
separations, a CC-balanced predicated body, replay-buffer bounds, exact LREG
ownership, and the mandatory D-RWC repair. Those are reasons to build the
shared physical allocation, state-token, latency, and final-stream validator
described here—not reasons to add a second duplicate-sequence matcher.

Some fitter notes say the compiler replay path was "dormant", while an exp
audit says the compiled stream was already saturated with replay. These are
compatible observations at different layers: exact repeated streams can and
do trigger GCC's pass, but GCC does not currently reshape a generic vFloat
loop into the high-quality, replay-friendly body above. In that exp case the
remaining gap was constant reloads versus native pinned constants. The
productization target is therefore to make scheduling, physical allocation,
constant placement, address-state modeling, and replay selection cooperate so
generic SFPI produces the same body quality without handwritten TTI.

### Rematerialization and `Dst` cuts

The first safe rematerialization pass should duplicate only positively
allowlisted deterministic operations such as direct immediate loads, abs,
simple masks, squares, and cheap address calculations. It should require a
strict pressure rescue or a verified latency win.

It must never duplicate stochastic rounding, PRNG reads, CC operations,
configuration access, LUT/stateful operations, memory operations, replay, or
macro operations. General algebraic reassociation and even/odd Horner
rewriting also change floating-point rounding and belong behind explicit
numerics/fast-math policy.

Using `Dst` as an automatic spill store is not equivalent to an ordinary
stack spill. It may overwrite user output, change RWCs, race another backend
unit, or expose stale values on architectures with `Dst` visibility hazards.
Automatic cuts require a caller ABI reserving exact scratch rows, a layout and
alias proof, architecture-specific store/reload visibility, and proof that no
packer or user observes the temporary values. Until that contract exists,
implicit `Dst` spilling is a hard no-go.

### `SFPLOADMACRO`: checked region before automatic inference

The correct mnemonic is `SFPLOADMACRO`. It is not merely a fused load. Four
instruction templates and four sequence words are programmed through
`SFPCONFIG`; each macro launch rewrites physical operands and schedules
operations onto Simple, MAD, Round, and Store sub-units with three-bit delays.
Delays can count elapsed instructions or elapsed cycles, and outstanding
operations interact across sub-units. The macro can use transient LREG16.

The dangerous cases are architectural, not stylistic:

- a scheduled macro operation wins a same-cycle sub-unit collision and the
  ordinary operation is silently discarded;
- macro-scheduled stores use different address/RWC semantics from ordinary
  stores;
- Blackhole automatic RAW stalling does not cover macro-issued operations;
- template and delay state survives outside the local source expression;
- Wormhole LLKs explicitly disarm macro state and drain it because leaving the
  state armed can hang the SFPU.

The first product should therefore be an explicitly owned region:

```text
sfpu_macro_region begin
    configure templates and sequences
    execute a fixed load/compute/round/store pipeline
    drain all pending sub-unit operations
    perform architecture-specific teardown
sfpu_macro_region end
```

GCC first needs recognized config and `SFPLOADMACRO` builtins/RTL rather than
raw encoded-word macros. Formation should run after physical allocation but
before final hazard repair, and should use a cycle-accurate pending-event
validator. Start with one WH/BH typecast, signbit, max/min, or int32-multiply
loop whose init, use, cleanup, and plain-loop fallback are all visible. Keep
Quasar separate.

### MOP: a separate declarative LLK compiler

MOP is a per-thread frontend microprogram expander, not an SFPU arithmetic
peephole. It supports a masked A/B template and a nested start/loop/end
template, with special last-inner and last-outer instructions. Wormhole can
expand one MOP to 32,639 instructions; Blackhole has wider counts. The nine
configuration words are write-only and must not change during expansion. MOP
can emit replay instructions because the MOP expander precedes replay in the
frontend, but replay expansion cannot meaningfully contain MOP. See the
official
[MOP Expander model](https://github.com/tenstorrent/tt-isa-documentation/blob/860a84abf7cd68098c444656798bd79a261f627e/WormholeB0/TensixTile/TensixCoprocessor/MOPExpander.md#L1-L137).

MOP programs in TT-Metal combine math, pack, unpack, PACR/UNPACR, address
counters, context selection, replay fragments, last-iteration substitutions,
MMIO configuration, and sometimes cross-thread semaphore synchronization.
Inferring them from arbitrary vFloat GIMPLE would omit most of the semantics.

The product direction should be a declarative LLK schedule IR containing:

- backend engine and instruction template;
- outer/inner loop geometry and masks;
- start, loop, last-inner, last-outer, and end operations;
- replay fragments and buffer ownership;
- address-counter and context transformations;
- synchronization and resource declarations;
- init, kill, restore, and configuration ownership.

That IR can lower to the existing `ckernel_template` and
`ckernel_unpack_template` machinery. GCC should provide recognized MOP/config
builtins and validation, but initial template selection should remain in this
LLK planner or an explicitly annotated whole-kernel region.

### Transpose and layout lowering

`SFPTRANSP` is frequently used to compensate for the four-row physical
granularity of SFPU load/store rather than as an arbitrary arithmetic
operation. Welford loads four physical `Dst` rows into LREG0-LREG3, transposes
them into logical row vectors, computes, then restores layout. Reshuffle and
reduction kernels use related all-eight-LREG layouts.

A future layout-aware lowering should represent logical row/lane operations
before physical LREG assignment, then select paired
load/transpose/compute/transpose/store schedules. The verifier must account
for every fixed LREG clobber and prove that the final layout and `Dst` contents
match the abstract operation.

### Recommended implementation sequence

1. Introduce the shared WH/BH operation, latency, resource, and state-effect
   descriptor.
2. Finish certified final-RTL physical LREG allocation and destructive
   coalescing.
3. Add a post-allocation latency scheduler; retain NOP insertion as the final
   verifier/repair.
4. Harden replay and add exact buffer packing plus replay-friendly coloring.
5. Add narrow rematerialization.
6. Add bounded row software pipelining after explicit `Dst`/RWC state exists.
7. Add a checked `SFPLOADMACRO` region and validate one WH/BH kernel family
   against its handwritten and plain-loop versions.
8. Add transpose/layout lowering.
9. Develop MOP as a separate declarative LLK schedule compiler.

### Validation boundary

Every late transformation should have an independent symbolic trace and cycle
validator. It must reject unknown configuration ownership, indirect LREG
indices it cannot resolve, unmatched predication state, implicit RWC or `Dst`
dependencies, unsupported instruction modes, and possible same-cycle macro
collisions.

For each supported optimization, compile the same ordinary source with the
feature off and on for Wormhole and Blackhole, compare final expanded traces,
run ttsim/craq-sim numerical tests, and then run hardware cycle tests. Use the
existing handwritten LLK as a performance golden and the plain-loop fallback
as a semantic golden. Quasar/QSR remains disabled until it has an authoritative
ISA model, focused compiler fixtures, simulator coverage, and hardware
sign-off.
