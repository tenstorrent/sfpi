# Welford vFloat Register Pressure and LP Scheduling Plan

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
- `-mtt-tensix-optimize-lp-schedule` is reported as `[disabled]` by the installed compiler unless explicitly enabled. This preserves the existing compiler by default.
- The focused validator produced 24 assembly files. On Wormhole and Blackhole, the late-fold fixture fails with the expected SFPU spill when the pass is off, while pass-on certifies `old-peak=9 new-peak=8 applied=yes` and emits exactly the same assembly as the manual early-fold control.
- The minimal, predicated and CFG rejection fixtures remain byte-identical pass-off/on. The live-across fixtures report `ops=2 live-in=4 peak=4`. Three repeated rescue compilations per architecture are deterministic: Wormhole SHA-256 `3cee70a65b29062489d790bcc30b23cc43ffe0741cc634e5106be8b0b14d8344`; Blackhole SHA-256 `502bff4f216e3ed5d90ba891cb6a0c6a636e765af6ad53ab1c94d04f0b0f3ed1`.
- The real TT-target DejaGNU run reports 1,070 expected passes, zero unexpected `FAIL`, zero `UNRESOLVED`, and zero `ERROR`. Two pre-existing expected failures remain expected. An initial invocation had falsely printed success because `runtest` was absent; installing `dejagnu` and rerunning the real suite exposed and eliminated that infrastructure false-green.
- The recurrent-SFPU EMA craq-sim smoke passes all three cases with the scheduler off and on: Wormhole `3 passed in 32.63s` off and `3 passed in 31.14s` on; Blackhole `3 passed in 31.74s` off and `3 passed in 30.55s` on. These are correctness smoke timings, not device-cycle measurements, and the test is handwritten EMA rather than a transformed eltwise Welford kernel.
- The pinned Blackhole simulator needed a VM-local compatibility patch before the LLK harness could run. It reads back retained TRISC/NCRISC reset-PC state for harness introspection and retains valid generated `THREAD_CFG23`/`THREAD_CFG25` `SETC16` writes. Hardware documents the reset-PC registers as write-only; this is a simulator accommodation, stored as `scripts/craq-sim-bh-llk-smoke.patch`, not an SFPI compiler semantic change.

The result is a strong compiler-feasibility checkpoint, not a performance sign-off. The pass currently contains an exact pressure oracle and deterministic list scheduler; it does **not** yet link or invoke `lp_solve`. Full `--test-gcc`, a transformed eltwise Welford numerical test, device execution, hardware cycle comparison against handwritten LLK, and the wider TT-Metal kernel corpus remain required before enabling it by default or claiming a latency win.

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

- Add the minimal `welford4` testcase to the verified TT DejaGNU harness. Audit found that `check-gcc-tt` selects `rvtt.exp`, whose original glob reached only `tt/*.C` and skipped `tt/sfpi/*.C`. The branch extends that driver with focused globs for only `sfpi/lp-schedule-*.C` and `sfpi/welford-pressure-*.C`; it does not silently enable the entire legacy directory.
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

- Add optional configure detection and a default-off target flag such as `-mtt-tensix-optimize-lp-schedule`.
- Add deterministic node/iteration limits and discard any non-optimal incumbent. A wall timeout is only an emergency abort.
- Fall back byte-for-byte to the original order on timeout, unsupported regions, infeasibility, validation failure or solver absence.
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

The local `sfpi-gcc` submodule was a three-commit shallow clone. Pushing that ancestry to a new empty repository failed because the shallow parent objects were absent; unshallowing would fetch roughly 2.15 million objects. The private branches are consequently explicit snapshot-root histories, while the normal local `nkapre/welford` branches retain upstream ancestry for later review. The SFPI snapshot's `.gitmodules` points `gcc` at the private GCC copy and its gitlink names the exact private GCC snapshot.

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

The first verified private tips were:

```text
nkapreTT/sfpi:     0afab83aadd767e43fb1d1c54a828043e31ef81c
nkapreTT/sfpi-gcc: 9dd3c45acf55f09b69cc9aabe505ddcd853bfefe
```

For later checkpoints, first commit normally on each local ancestry-preserving `nkapre/welford` branch. Then create a private snapshot commit whose tree is the normal branch's tree and whose parent is the prior private snapshot. Push GCC first. Rebuild the SFPI snapshot tree with the new private GCC gitlink and private `.gitmodules` URL, then push SFPI. Never stage the pre-existing local modification to `gcc/testsuite/g++.target/riscv/tt/sfpi/dataformat-bh.C`.

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
  -fno-exceptions -fno-rtti -mtt-tensix-optimize-lp-schedule \
  -fdump-tree-rvtt_lp_schedule -S "$SRC" -o /tmp/welford-on.S
```

The failing rescue fixture uses the same commands with `welford-pressure-reorder-wh.C`. Acceptance requires:

```sh
cmp /tmp/welford-off.S /tmp/welford-on.S       # low-pressure fixture
grep 'old-peak=9 new-peak=8 applied=yes' /tmp/*.rvtt_lp_schedule
! grep -q 'register spill' /tmp/welford-on.log
```

The current implementation is deliberately a default-off, positive-allowlist pressure oracle plus deterministic list scheduler. It is not yet an `lp_solve` integration. MILP comes only after the pressure/liveness model and materialization checker pass these tests.

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
```

The measured pass-on results are Wormhole `3 passed in 31.14s` and Blackhole `3 passed in 30.55s`, again with zero crashed tests. These elapsed times are not a useful performance comparison: the fixtures are handwritten EMA, simulator startup dominates, and the runs were intended only as numerical/non-crash regression gates.

### 9. Compiler and regression gates

Run the checked-in focused validator first. It exercises pass-off, minimal no-op, genuine 9-to-8 rescue, manual scheduling control, predication rejection, and three-run deterministic assembly on both architectures:

```sh
cd /home/nkapre.guest/sfpi-src
./scripts/validate-welford-scheduler.sh \
  /home/nkapre.guest/sfpi-lp-build/sfpi \
  /home/nkapre.guest/welford-validation
```

After the focused fixtures pass:

```sh
cd /home/nkapre.guest/sfpi-src
./scripts/build.sh --dir=/home/nkapre.guest/sfpi-lp-build --test-gcc
./scripts/build.sh --dir=/home/nkapre.guest/sfpi-lp-build --test-tt
```

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
