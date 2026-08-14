# SFPI Compiler Upgrade: Pressure Scheduling, Exact MILP Optimization, Allocation Enforcement, and Tensix Coprocessor Lowering

## 1. Executive Summary & Architectural Overview

The **SFPI Compiler Upgrade** addresses the fundamental vector register allocation and instruction scheduling challenge of the Tenstorrent Tensix coprocessor: **the SFPU is a 32-lane SIMD engine with only eight variable vector registers (L0–L7) and no hardware or software stack spilling mechanism.**

In standard GCC, register allocation (IRA/LRA) assumes memory spilling is a valid fallback. On the SFPU, any register pressure exceeding eight simultaneous live values causes an immediate, fatal internal compiler error (`cannot store sfpu register (register spill)`). Furthermore, tight expressions like online Welford updates require exact **destructive operand reuse** (e.g., overwriting a dying input with a result) with zero slack.

```
       ┌─────────────────────────────────────────────────────────────┐
       │              The Zero-Slack Allocation Triad                │
       └─────────────────────────────────────────────────────────────┘
                                      ▲
                                     / \
                                    /   \
                 Strict 8-LREG     /     \     No Spill / No Stack
                 Hardware Limit   /_______\    Abort on 9th Live Value
                                      │
                                      ▼
                        Destructive 2-Address Overlap
                        (Result must overwrite dying input)
```

### Core Invariants & Architecture Decisions

1. **Dual-Engine Scheduling (Exact MILP & List Heuristic):**
   - **MILP Optimization (`lp_solve`):** Essential for finding globally optimal schedules on complex dataflow DAGs where greedy heuristics fail (such as the proven 11-to-8 register reduction case), and for joint multi-objective optimization (minimizing register pressure, exposed pipeline stalls, and copy overhead).
   - **Deterministic List Scheduler:** High-speed pressure-first ready-list scheduler prioritizing operand kills and critical paths for standard compilation.
2. **Scheduling Feasibility vs. Physical Allocation Enforcement (M2):**
   - A schedule with peak liveness $\le 8$ at the GIMPLE level is a necessary feasibility certificate, but not an allocation guarantee. Standard GCC IRA can still fail to realize the required destructive coalesces.
   - Production safety requires an explicit **M2 Allocation Enforcement mechanism** before reload so that certified islands enter register allocation with guaranteed hard LREG assignments.
3. **Strict Predication Semantics (`_lv` Safety):**
   - `_lv` forms (e.g., `sfpassign_lv`) carry inactive-lane preservation semantics under condition-code (`v_if`/`v_else`) masking. `_lv` is never synthesized or treated merely as a generic register coalescing hint, preventing silent vector corruption.
4. **Feasibility Scheduling vs. Latency Scheduling:**
   - **Feasibility Mode:** Minimize peak register pressure to fit within 8 LREGs and rescue uncompilable graphs.
   - **Latency Mode:** Minimize exposed pipeline stalls (e.g., interleaving independent chains in Wormhole 2-cycle RAW gaps) when pressure is already within capacity ($\le 8$).
5. **Separation of Coprocessor Concerns:**
   - Pressure/latency scheduling, replay buffer compression, `SFPLOADMACRO` multi-unit dispatch, MOP planning, layout lowering, and rematerialization are structured as distinct compiler passes with clear boundaries and independent verification.

---

## 2. Dual-Engine Pressure Scheduling (Exact MILP & List Heuristic)

The compiler pass operates on straight-line vector arithmetic basic blocks at the late GIMPLE SSA level (immediately before RTL expansion).

```
vFloat C++ Source
       │
       ▼
GIMPLE SSA Dataflow Extraction
       │
       ▼
Transactional Legality & Pressure Oracle
       │
       ├──► Deterministic List Scheduler (Fast Heuristic Path)
       │
       └──► Exact MILP Optimizer (0/1 Integer Linear Program via lp_solve)
       │
       ▼
Independent Schedule & Pressure Certificate Validator
       │  (Rebuilds def-use, sources, peaks, destructive ties)
       ▼
Commit GIMPLE Rewrite  ──(On any validation failure)──► Leave GIMPLE Untouched
```

### 2.1 Mathematical MILP Formulation

The MILP optimizer models simultaneous instruction scheduling, exact liveness linearization, register capacity constraints, and destructive operand reuse using Mixed Integer Linear Programming:

#### Variables:
- $\text{issue}_{i,t} \in \{0, 1\}$: Binary indicator that operation $i \in \{1 \dots N\}$ is issued in time slot $t \in \{1 \dots T\}$.
- $\text{live}_{v,t} \in [0, 1]$: Continuous variable (linearized to $0$ or $1$) indicating SSA value $v$ is live at slot $t$.
- $\text{assign}_{v,r} \in \{0, 1\}$: Binary assignment of value $v$ to physical register $r \in \{0 \dots 7\}$.
- $\text{occupy}_{v,r,t} \in [0, 1]$: Linearized conjunction $\text{live}_{v,t} \land \text{assign}_{v,r}$.
- $\text{alias}_{i,v} \in \{0, 1\}$: Result of operation $i$ destructively overwrites operand value $v$.

#### Constraints:
1. **Single Issue:** Every operation issues exactly once:
   $$\sum_{t=1}^T \text{issue}_{i,t} = 1 \quad \forall i$$
2. **Resource Capacity:** At most one SFPU operation issues per cycle:
   $$\sum_{i=1}^N \text{issue}_{i,t} \le 1 \quad \forall t$$
3. **Dataflow Precedence & Latency:** For every true dependency $(u, v) \in E$:
   $$\sum_{t=1}^T t \cdot \text{issue}_{v,t} \ge \sum_{t=1}^T t \cdot \text{issue}_{u,t} + \text{latency}(u)$$
4. **Exact Liveness Linearization:** Value $v = \text{def}(i)$ becomes live immediately after slot $t(i)$ and remains live until the latest consuming slot $\max_{w \in \text{uses}(v)} t(w)$.
5. **Destructive Overlap / Same-Slot Reuse:** If operation $i$ reuses operand $v$ via $\text{alias}_{i,v} = 1$, the result register may equal $v$'s register because $v$ dies at slot $t(i)$.
6. **Strict Capacity Limit:** Total active registers at any cycle $t$ must satisfy:
   $$\sum_{v} \text{live}_{v,t} \le 8 \quad \forall t$$

#### Multi-Tier Lexicographic Objective:
The MILP solves sequentially or via a weighted primary formulation:
1. **Primary:** Minimize peak register occupancy: $\min \max_t \sum_v \text{live}_{v,t}$.
2. **Secondary:** Minimize schedule makespan: $\min \sum_t t \cdot \text{issue}_{\text{last}, t}$.
3. **Tertiary:** Minimize register copy / live-range area and deviation from the deterministic list schedule.

### 2.2 Why MILP is Essential: The 11-to-8 Optimality Proof

While greedy list heuristics work for simple expressions, register-constrained DAG scheduling is strongly NP-hard. The checked-in fixture `scripts/lp-schedule-milp-beats-list.C` provides concrete mathematical proof of MILP's necessity:

- **Graph Structure:** A 10-operation arithmetic DAG with 11 initial live values.
- **List Heuristic Result:** Trapped in a local pressure minimum; fails to reduce peak below 9.
- **MILP Result:** Explores the full combinatorial search space, finds an exact sequence of destructive reuses, and produces a validated **11 $\to$ 8** schedule.

MILP serves as both an exact solver for complex kernels and an authoritative oracle against which heuristic improvements are measured.

### 2.3 Deterministic List Scheduler

For rapid compilation, the list scheduler uses a pressure-aware ready list:
- **Priority 1:** Operations that immediately kill one or more live operands (reducing net register pressure).
- **Priority 2:** Operations on the critical latency path.
- **Priority 3:** Operations that keep total live values $\le 8$.
- **Tie-Break:** Canonical source statement UID for 100% build determinism.

### 2.4 Independent Transactional Legality & Certificate Validator

Before any GIMPLE mutation occurs, an independent validator (`validate_schedule`) verifies:
1. The schedule is an exact permutation of the original operation multiset.
2. Every SSA operand definition strictly precedes all uses.
3. Live-in, live-out, and live-through values confirm the claimed peak $\le 8$.
4. CC epoch state, constant LREG reads (L8–L15), and scalar SSA inputs remain legal.
5. Deliberate invalid-certificate self-tests (duplicate operations, use-before-def, false peaks) must be rejected before committing changes.
6. On any validation failure, timeout, or infeasibility, GIMPLE remains byte-for-byte untouched.

---

## 3. The M2 Physical Register Allocation Problem & Architecture Spike

### 3.1 The Pre-IRA Boundary Gap

A GIMPLE schedule with peak liveness $\le 8$ guarantees that an 8-coloring *theoretically exists*. However, GCC's Integrated Register Allocator (IRA) and reload pass operate on RTL and may:
- Choose an alternative coloring that fails to coalesce destructive ties.
- Introduce intermediate pseudo-registers during expansion that inflate pressure.
- Abort with an internal compiler error (`cannot store sfpu register`) when reload fails.

To bridge this gap, Milestone M2 must guarantee that certified schedules reach assembly without relying on IRA heuristics.

```
       ┌─────────────────────────────────────────────────────────────┐
       │             Candidate Allocation Architectures (M2)         │
       └─────────────────────────────────────────────────────────────┘
                                      │
   ┌──────────────────────────────────┼──────────────────────────────────┐
   ▼                                  ▼                                  ▼
Option A: Atomic Pre-IRA           Option B: Target IRA Hooks         Option C: Explicit Tied
Hard-Register Island Substitution         & Priority Directives              Machine Constraints
(Replace pseudos w/ L0-L7)         (Influence IRA coloring)           (Target MD constraints)
```

### 3.2 Architectural Spike: Evaluating 5 Implementation Candidates

| Candidate Architecture | Implementation Mechanism | Strengths | Risks / Trade-offs |
| :--- | :--- | :--- | :--- |
| **Option A: Atomic Pre-IRA Hard-Register Island Substitution** | Enumerate interval coloring in pre-IRA RTL pass (`rtl-rvtt-lp-alloc.cc`), atomically replace pseudos with hard L0–L7 using `apply_change_group()`. | Completely eliminates IRA uncertainty; mathematically impossible for IRA to spill the island. | Requires closed island boundaries; fixed register and scalar address interactions must be audited. |
| **Option B: Target-Specific IRA Allocation Hooks** | Use `TARGET_IRA_CHANGE_PSEUDO_ALLOCNO_CLASS` and allocation priority hooks to force IRA to follow the coloring order. | Works within standard GCC allocation pipeline. | GCC IRA priority heuristics do not offer hard guarantees under zero-slack constraints. |
| **Option C: Explicit Destructive Tied RTL Constraints** | Add machine description patterns in `rvtt.md` matching destination and dying operand via constraint `"0"` or `"1"`. | Native GCC mechanism for 2-address destructive reuse. | Expanding all permutations of tied operands increases machine description pattern complexity. |
| **Option D: Pre-Reload / Post-IRA Local Repair Pass** | Allow IRA to run, but inspect the result before reload; if uncolored or spilled, apply local graph recoloring. | Traps failures at the exact point of occurrence. | Complex interaction with LRA live-range splitting. |
| **Option E: Spill-Failure Exception & Rescheduling Loop** | Catch the reload spill trigger and invoke a constrained rescheduling pass with adjusted weights. | Dynamic recovery from compiler allocation misses. | GCC reload is not architected for backtracking recovery loops. |

### 3.3 M2 Exit Criteria
Milestone M2 is considered complete when:
1. The synthetic `11 -> 8` fixture (`lp-schedule-milp-beats-list.C`) compiles cleanly to final assembly without register spill.
2. Certified straight-line arithmetic islands enter reload with zero unallocated SFPU pseudos.
3. Every destructive tie selected by the solver is physically present in the emitted assembly.

---

## 4. Feasibility vs. Latency Scheduling

### 4.1 Functional Separation of Passes

```
                          CANDIDATE BASIC BLOCK
                                    │
                    ┌───────────────┴───────────────┐
                    ▼                               ▼
         Peak Register Pressure > 8       Peak Register Pressure <= 8
                    │                               │
                    ▼                               ▼
          FEASIBILITY SCHEDULER             LATENCY SCHEDULER
       (Minimize Register Pressure)      (Minimize Exposed Stalls)
                    │                               │
                    ├── Prioritize operand kills     ├── Interleave independent chains
                    ├── Minimize active live ranges  ├── Fill 2-cycle MAD RAW gaps
                    └── Target: Fit in 8 LREGs       └── Target: Minimize makespan
```

### 4.2 Latency Hiding & Independent Chain Interleaving

Wormhole B0 hardware requires a **2-cycle result latency** for SFPU floating-point operations (`SFPADD`, `SFPMUL`, `SFPMAD`). If a consumer immediately follows its producer, software must insert `SFPNOP`.

#### The Dual-Horner Polynomial Benchmark:
Evaluating rational approximations $P(x)/Q(x)$ presents two independent arithmetic chains.
- **Serial Baseline (Wormhole):** 8 MADs + 7 exposed hazard NOPs = **15 issue slots**.
- **Interleaved Latency Schedule:** 8 MADs + 1 trailing NOP = **9 issue slots** (**40% reduction in issue slots**).

```
Serial Issue Stream (15 Slots):
Slot:  0      1      2      3      4      5      6      7      8      9     10     11     12     13     14
Op:   P0 ──► NOP ──► P1 ──► NOP ──► P2 ──► NOP ──► P3 ──► Q0 ──► NOP ──► Q1 ──► NOP ──► Q2 ──► NOP ──► Q3

Interleaved Issue Stream (9 Slots):
Slot:  0      1      2      3      4      5      6      7      8
Op:   P0 ──► Q0 ──► P1 ──► Q1 ──► P2 ──► Q2 ──► P3 ──► Q3 ──► NOP
```

### 4.3 Architectural Differences: Wormhole vs. Blackhole vs. Quasar

- **Wormhole B0:** Relies entirely on software-scheduled instruction separation or explicit `SFPNOP` insertion.
- **Blackhole A0:** Features hardware scoreboarding to stall dependent instructions automatically. However, official ISA documentation records specific hardware scoreboard omissions (e.g., bitwise logic, integer adds, shuffles, and `SFPLOADMACRO` executions). The compiler latency model must account for these target differences.
- **Quasar (QSR):** Separate coprocessor architecture; maintains independent scheduling flags and pipeline profiles.

---

## 5. Tensix Coprocessor Optimization Architecture

```
                                  TENSIX COPACCEL FRONTEND
                ┌─────────────────────────────────────────────────────────┐
                │          RISC-V Tensix Instruction Push Stream          │
                └────────────────────────────┬────────────────────────────┘
                                             │
                       ┌─────────────────────┴─────────────────────┐
                       ▼                                           ▼
             ┌───────────────────┐                       ┌───────────────────┐
             │   MOP Expander    │                       │  Replay Expander  │
             │ (Up to 32k insns) │                       │ (32-slot circular)│
             └─────────┬─────────┘                       └─────────┬─────────┘
                       │                                           │
                       └─────────────────────┬─────────────────────┘
                                             │
                                ┌─────────────────────────┐
                                │ Wait Gate / Sync Engine │
                                └────────────┬────────────┘
                                             │
                ┌────────────────────────────┴────────────────────────────┐
                ▼                            ▼                            ▼
      ┌──────────────────┐         ┌──────────────────┐         ┌──────────────────┐
      │  Matrix Engine   │         │  Vector (SFPU)   │         │ Pack / Unpack    │
      │  (FPU / MatMul)  │         │   32-Lane SIMD   │         │ (Data Formatting)│
      └──────────────────┘         └─────────┬────────┘         └──────────────────┘
                                             │
                 ┌───────────────────────────┴───────────────────────────┐
                 ▼               ▼                   ▼                   ▼
           ┌───────────┐   ┌───────────┐       ┌───────────┐       ┌───────────┐
           │ SFPU Load │   │ Simple AL │       │ SFPU MAD  │       │ SFPU Store│
           └───────────┘   └───────────┘       └───────────┘       └───────────┘
```

### 5.1 Register & State Architecture (from `tt-isa-documentation`)

- **General Allocatable Registers:** $L_0 \dots L_7$ (8 variable 32-lane vector registers).
- **Constant / Fixed Registers:**
  - $L_8 = 0.8373$, $L_9 = 0.0$, $L_{10} = 1.0$, $L_{11} = -1.0$, $L_{15} = 2 \times \text{lane\_id}$.
- **Transient Register $L_{16}$:**
  - Accessible **only** during operations launched via `SFPLOADMACRO`.
- **Implicit State:** Per-thread Read-Write Counters (RWCs), Destination ($Dst$) tile registers, address modifiers (`ADDR_MOD`), and condition-code stacks.

### 5.2 Replay Buffer Hardening (`rtl-rvtt-replay.cc`)

The Tensix coprocessor features a **32-slot per-thread circular instruction replay buffer**. Recording an unrolled sequence and executing it via `REPLAY` eliminates RISC-V frontend instruction push overhead.

- **Mockup Evidence:** On an 8-row unrolled loop, automatic replay compression reduces static Tensix instructions from **88 down to 19 on Wormhole (-78.4%)** and **56 down to 15 on Blackhole (-73.2%)**.
- **Roadmap:** Harden the post-reload replay pass with exact dynamic programming for optimal 32-slot packing and inter-iteration register-renaming canonicalization.

### 5.3 `SFPLOADMACRO` Multi-Unit Concurrent Dispatch

`SFPLOADMACRO` is the only hardware mechanism capable of multi-unit issue in the SFPU. It programs up to four template instructions launched with 3-bit delay counters across:
- **Load Sub-Unit**
- **Simple ALU Sub-Unit**
- **MAD Sub-Unit**
- **Store Sub-Unit**

```
Standard Serial Issue (4 Cycles / Element):
Cycle 0: [SFPLOAD  ]
Cycle 1: [SFPLOAD  ]
Cycle 2: [SFPMUL   ]
Cycle 3: [SFPSTORE ]

SFPLOADMACRO Pipelined Dispatch (1 Cycle / Element Steady State):
Cycle t: [Load(i+2)] | [Simple/Mul(i+1)] | [Store(i)]  <-- 3 sub-units active simultaneously
```

- **Target Kernels:** Typecast, integer multiply (`mul_int`), signbit, and conditional `where` achieve **1.33x to 4.0x steady-state throughput increases**.
- **Implementation:** Introduce structured `sfpu_macro_region` blocks with explicit lifecycle management (arm, execute, drain, disarm) to avoid hanging coprocessor state.

### 5.4 Rematerialization Policy

Rematerialization duplicates cheap computations to break long live ranges without memory spilling.
- **Whitelist (Pure, Deterministic):** Direct constants, `sfpabs`, bitwise masks, simple squares, base address arithmetic.
- **Strictly Prohibited:** Condition-code operations, PRNG/stochastic rounding, configuration reads, LUT accesses, memory loads/stores.

### 5.5 Layout & Transpose Lowering (`SFPTRANSP`)

The SFPU operates on $Dst$ rows with 4-row physical load/store granularity. Welford, transpose, and matrix reductions use `SFPTRANSP` to transform physical tile formats into logical lane vectors. Layout-aware lowering will optimize paired load/transpose/compute/store sequences globally.

### 5.6 Declarative MOP IR

Micro-Operation (MOP) expanders generate tens of thousands of frontend operations from compact templates. Rather than reverse-engineering MOPs from raw C++, a declarative schedule IR will allow TT-Metal kernels to express nested looping and unpack/math/pack synchronization directly.

---

## 6. TT-LLK Kernel Corpus Analysis & Performance Potential

| Kernel | Architecture Challenge | Existing Manual Workaround | Compiler Solution & Expected Win |
| :--- | :--- | :--- | :--- |
| **Welford (LayerNorm)** | 8 live values across 4 rows with zero register slack. | Recomputes delta ($\delta_2$) and hand-colors L0–L7. | List & MILP schedulers find valid 8-LREG schedule; eliminates ICE without manual microcode. |
| **Dual-Horner Rational** | 7 exposed NOP stalls in serial $P(x)/Q(x)$ evaluation. | Manual instruction interleaving in TTI. | Automatic latency scheduler interleaves chains, eliminating **40% of issue slots**. |
| **Piecewise Generic / LUT** | Interleaved MADs, pinned coefficients, D-RWC updates. | 3 distinct hand-written polynomial replay bodies. | Compiler-managed coefficient pinning + exact replay packing. |
| **Log (`ckernel_sfpu_log.h`)** | Peak pressure 9 during polynomial + exponent correction. | Explicit reload from $Dst$ at line 62. | Pressure scheduling keeps inputs resident; saves 2 $Dst$ memory cuts. |
| **GELU / Erfinv** | High register pressure across nested inlined tanh/log/sqrt. | Intermediate state dumped to $Dst$. | Continuous 8-LREG allocation eliminates $Dst$ round-trip overhead. |
| **Addcmul (`ckernel_sfpu_addcmul.h`)** | Inter-row RAW dependencies across 2 rows. | Manual `MUL_a, MUL_b, MAD_a, MAD_b` ordering. | Latency scheduler automatically pipelines adjacent rows. |
| **Integer Remainder / Div** | Divisor chunk pressure. | Recomputes divisor expressions. | Target-directed rematerialization. |
| **Typecast / MulInt / Where** | Serial load-compute-store memory bound. | Plain loop fallback. | `SFPLOADMACRO` multi-unit pipeline drives **2.0x–4.0x throughput**. |

---

## 7. Reconciled Execution Roadmap & Milestones

```
M0: Harden List Scheduler & Validator (Default Off)
 │
 ├──► M1: Unified Target Machine Model & Pre-IRA RTL Audit
 │     │
 │     └──► M2: M2 Allocation Enforcement Spike & Implementation
 │           │
 │           └──► M3: Production MILP Integration & Lexicographic Solver
 │                 │
 │                 └──► M4: Silicon Hardware Sign-off & TT-Metal CI Pin
 │                       │
 │                       └──► M5: Latency Scheduling, Replay & Macro Rollout
```

### Milestone Specifications

- **Milestone M0 (Hardened Baseline):**
  - Transactional GIMPLE list scheduler with independent validator.
  - Passes DejaGNU testsuite with zero regressions.
  - Deterministic builds certified across 20 serial and 20 parallel runs.
- **Milestone M1 (Canonical Machine Model & RTL Audit):**
  - Unified opcode descriptors for Wormhole, Blackhole, and Quasar.
  - Pre-IRA RTL audit pass (`rtl-rvtt-lp-alloc.cc`) reporting real pseudo/hard register liveness.
- **Milestone M2 (Allocation Enforcement Spike):**
  - Evaluate pre-IRA hard-register island substitution vs. target IRA constraints.
  - Prove that the 11-to-8 fixture (`lp-schedule-milp-beats-list.C`) reaches assembly without reload spilling.
- **Milestone M3 (Production MILP Optimizer):**
  - Host-side configure detection for `--with-lp-solve`.
  - Multi-objective lexicographic optimization (pressure $\to$ makespan $\to$ ties).
  - Deterministic branch-and-bound node limits with graceful fallback to list scheduling.
- **Milestone M4 (Silicon Hardware Sign-off):**
  - Execute [`WELFORD_SILICON_VALIDATION.md`](WELFORD_SILICON_VALIDATION.md) on real Wormhole and Blackhole devices.
  - Measure hardware cycles, static instruction counts, replay buffer usage, and numerical parity against hand-optimized LLKs.
- **Milestone M5 (Latency Scheduling & Coprocessor Expansion):**
  - Implement makespan-minimizing latency scheduling for Dual-Horner and `addcmul`.
  - Harden 32-slot replay buffer packing.
  - Deploy structured `SFPLOADMACRO` pipelines for typecast and integer math.

---

## 8. Testing, Build & Verification Workflow

### 8.1 Compiler Build & Flags

```bash
# Build compiler with checking and MILP solver support:
SFPI_WITH_LP_SOLVE=yes ./scripts/build.sh --dir=build --checking

# Compiler invocation flags:
riscv-tt-elf-g++ -mcpu=tt-wh-tensix -O2 \
  -mtt-tensix-optimize-pressure-schedule \      # Enable pressure scheduling (list)
  -mtt-tensix-pressure-schedule-use-milp \      # Enable exact MILP solver
  -fdump-tree-rvtt_lp_schedule \                # Dump GIMPLE scheduling decisions
  -fdump-rtl-rvtt_lp_alloc \                    # Dump pre-IRA RTL liveness audit
  -S kernel.C -o kernel.S
```

### 8.2 Focused Validation Harness

Run the automated test validation suite covering positive rescues, negative predication rejections, constant LREG ordering, and determinism:

```bash
./scripts/validate-sfpu-pressure-scheduler.sh build build/validation-output
```

---

## 9. Conclusion

The unified SFPI compiler upgrade provides a mathematically grounded, robust solution to the zero-slack 8-LREG allocation challenge. By combining the **exact optimality of MILP** with the **speed of deterministic list scheduling**, and backing them with **enforceable pre-IRA physical allocation (M2)**, Tenstorrent establishes a reliable compiler foundation that eliminates register spill crashes and paves the way for automated, world-class vector code generation.
