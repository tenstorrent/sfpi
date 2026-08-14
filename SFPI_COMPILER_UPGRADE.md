# SFPI Compiler Upgrade: Guarded Default-On Scheduling, Exact MILP Optimization, Allocation Enforcement, and the Roadmap to World SOTA

## 1. Executive Summary & Architectural Overview

The **SFPI Compiler Upgrade** addresses the fundamental vector register allocation and instruction scheduling challenge of the Tenstorrent Tensix coprocessor: **the SFPU is a 32-lane SIMD engine with only eight variable vector registers (L0–L7) and no hardware or software stack spilling mechanism.**

In standard GCC, register allocation (IRA/LRA) assumes memory spilling is a valid escape hatch. On the SFPU, any register pressure exceeding eight simultaneous live values causes an immediate, fatal internal compiler error (`cannot store sfpu register (register spill)`). Furthermore, tight mathematical expressions like online Welford updates require exact **destructive operand reuse** (e.g., overwriting a dying input with a result) with zero slack.

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

---

## 2. Guarded Default-On Policy & Rebuttal Analysis

### 2.1 The Non-Negative Delta Principle

The core justification for enabling the pressure scheduler by default for eligible regions is the **Non-Negative Delta Principle**:

1. **Zero Impact on Passing Code (Peak $\le 8$):** Any basic block whose natural source order fits within 8 LREGs completely bypasses pressure mutation, remaining **100% byte-identical** to baseline GCC.
2. **Strict Non-Regression on Failing Code (Peak $> 8$):** Basic blocks exceeding 8 LREGs currently abort with a fatal spill ICE. An automated rescue attempt that succeeds turns an uncompilable program into a valid binary; an attempt that fails leaves the program uncompilable. The delta is strictly non-negative ($\ge 0$).
3. **Incomplete Rescue Is Not a Regression:** The synthetic $11 \to 8$ fixture spills in IRA after GIMPLE scheduling, but because it also spilled without the scheduler, no working program is regressed.

```
                             REBUTTAL LOGIC FLOW
                   ┌──────────────────────────────────────┐
                   │    Input Region Register Pressure    │
                   └──────────────────┬───────────────────┘
                                      │
                      ┌───────────────┴───────────────┐
                      ▼                               ▼
               Peak <= 8 LREGs                  Peak > 8 LREGs
                      │                               │
                      ▼                               ▼
               BYPASS SCHEDULER               AUTOMATIC RESCUE
             (100% Byte-Identical)            (Strictly Non-Negative)
                                                      │
                                   ┌──────────────────┴──────────────────┐
                                   ▼                                     ▼
                            Rescue Succeeds                       Rescue Fails
                           (Compiles Cleanly)                 (Fails as it did before)
```

### 2.2 Production Guardrails & Fallback Chain

To eliminate JIT latency spikes and package distribution risks, the default-on scheduler executes as a strict fallback chain:

```
Candidate Region (Peak > 8)
       │
       ├──► 1. Fast Path: Deterministic List Scheduler (<0.1ms compile time)
       │         │
       │         └──► [Valid Peak <= 8] ──► Commit GIMPLE Rewrite
       │
       ├──► 2. Exact Path: Bounded MILP Solver (Demand-driven on List-miss; hard 20ms / 50k-node cap)
       │         │
       │         └──► [Valid Peak <= 8] ──► Commit GIMPLE Rewrite
       │
       └──► 3. Safe Fallback: Leave GIMPLE untouched (preserves legacy compilation state)
```

- **Default Flag:** `-mtt-tensix-optimize-pressure-schedule` (**Enabled by default** for $>8$ regions).
- **Rollback Override:** `-mno-tt-tensix-optimize-pressure-schedule` (Restores 100% legacy GCC behavior).
- **MILP Enablement:** `-mtt-tensix-pressure-schedule-use-milp` (**Auto-selected on list failure** when host solver is present).

---

## 3. Dual-Engine Scheduling Core (List Heuristic & Exact MILP)

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

### 3.1 Mathematical MILP Formulation

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
$$\min \quad 10^4 \cdot \underbrace{\left(\max_{t} \sum_{v=1}^V \text{live}_{v,t}\right)}_{\text{Peak Register Pressure}} + 10^2 \cdot \underbrace{\left(\sum_{t=1}^T t \cdot \text{issue}_{\text{last}, t}\right)}_{\text{Makespan (Stall Minimization)}} + \sum_{i=1}^N \sum_{v \in \text{deps}(i)} (1 - \text{alias}_{i,v})$$

### 3.2 Classical Prior Art: Goodman-Hsu Dual-Mode Scheduling
* **Prior Art:** Goodman & Hsu (1988) *Code Scheduling to Reduce Register Pressure (IPS)*.
* **Operational Modes:**
  1. **Pressure-Reduction Mode (P-Mode):** When live count $\ge K - \delta$ (e.g., $\ge 7$), prioritize nodes that kill the most live values.
  2. **Latency-Minimization Mode (L-Mode):** When live count $< K - \delta$ (e.g., $\le 6$), prioritize critical-path latency and independent chain interleaving.

```
                     READY LIST EVALUATION
                               │
               ┌───────────────┴───────────────┐
               ▼                               ▼
    Current Liveness >= 7            Current Liveness <= 6
               │                               │
               ▼                               ▼
     P-MODE (Pressure Mode)          L-MODE (Latency Mode)
  Priority 1: Max Operands Killed   Priority 1: Longest Critical Path
  Priority 2: Critical Path         Priority 2: Alternating Chains (Dual-Horner)
  Priority 3: Canonical UID         Priority 3: Canonical UID
```

### 3.3 Why MILP is Essential: The 11-to-8 Optimality Proof

While greedy list heuristics work for simple expressions, register-constrained DAG scheduling is strongly NP-hard. The checked-in fixture `scripts/lp-schedule-milp-beats-list.C` provides concrete mathematical proof of MILP's necessity:

- **Graph Structure:** A 10-operation arithmetic DAG with 11 initial live values.
- **List Heuristic Result:** Trapped in a local pressure minimum; fails to reduce peak below 9.
- **MILP Result:** Explores the full combinatorial search space, finds an exact sequence of destructive reuses, and produces a validated **11 $\to$ 8** schedule.

---

## 4. Milestone M2: Physical Register Allocation Enforcement (Concrete Implementation)

### 4.1 The Core Problem & Prior Art
* **Prior Art:** Appel & George (2001) *Iterated Register Coalescing*, Chaitin (1982) *Register Allocation via Coloring*, and LLVM's *Partitioned Boolean Quadratic Programming (PBQP)*.
* **The Failure Mode:** Standard GCC IRA creates virtual allocation units (`allocnos`) for every pseudo-register. In a zero-slack 8-register target without stack spilling, even one uncoalesced destructive tie causes IRA's priority heuristic to fail, pushing the pseudo into reload where it triggers `cannot store sfpu register`.
* **The Solution (Pre-IRA Closed Island Substitution):** By substituting pseudos with physical hard registers ($L_0 \dots L_7$) *before* IRA runs, IRA treats the entire island as pre-colored/fixed machine registers. IRA skips allocno generation, and reload is structurally bypassed.

```
       GIMPLE Certified Schedule (Peak <= 8)
                         │
                         ▼
       RTL Expansion (`pass_expand`)
                         │
                         ▼
       Early Rematerialization & Sched1
                         │
                         ▼
┌─────────────────────────────────────────────────────────────┐
│   `pass_rvtt_lp_alloc` (Pre-IRA Physical Island Allocator)   │
├─────────────────────────────────────────────────────────────┤
│ 1. Extract straight-line SFPU RTL insn sequence             │
│ 2. Build local live intervals for XTT32 pseudo-registers    │
│ 3. Apply Greedy Left-Edge / Interval 8-Coloring             │
│ 4. Atomic substitution: pseudo_reg -> gen_raw_REG(L0..L7)   │
│ 5. Validate recog_memoized(insn) >= 0 for all MD patterns   │
│ 6. Commit via apply_change_group() + df_insn_rescan_all()   │
└────────────────────────┬────────────────────────────────────┘
                         │
                         ▼
       GCC IRA / LRA (Zero SFPU pseudos remain; cannot spill!)
```

### 4.2 Concrete C++ Implementation (`gcc/gcc/config/riscv/tt/rtl-rvtt-lp-alloc.cc`)

```cpp
#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "backend.h"
#include "rtl.h"
#include "df.h"
#include "insn-config.h"
#include "recog.h"
#include "rvtt.h"

namespace {

struct live_interval {
    unsigned pseudo_regno;
    int first_def_uid;
    int last_use_uid;
    unsigned assigned_lreg;
};

// Interval Graph 8-Coloring (Left-Edge Algorithm on Straight-Line RTL)
bool allocate_interval_colors(std::vector<live_interval>& intervals, 
                              const std::unordered_map<unsigned, unsigned>& destructive_ties) {
    std::sort(intervals.begin(), intervals.end(), 
              [](const live_interval& a, const live_interval& b) {
                  return a.first_def_uid < b.first_def_uid;
              });

    bool lreg_busy[8] = {false};
    
    for (auto& iv : intervals) {
        for (const auto& past : intervals) {
            if (&past == &iv) break;
            if (past.last_use_uid <= iv.first_def_uid && past.assigned_lreg < 8) {
                lreg_busy[past.assigned_lreg] = false;
            }
        }

        // Check destructive tie to dying operand
        auto tie_it = destructive_ties.find(iv.pseudo_regno);
        if (tie_it != destructive_ties.end()) {
            unsigned tied_pseudo = tie_it->second;
            for (const auto& cand : intervals) {
                if (cand.pseudo_regno == tied_pseudo && cand.assigned_lreg < 8) {
                    iv.assigned_lreg = cand.assigned_lreg;
                    lreg_busy[iv.assigned_lreg] = true;
                    break;
                }
            }
        }

        if (iv.assigned_lreg >= 8) {
            for (unsigned r = 0; r < 8; ++r) {
                if (!lreg_busy[r]) {
                    iv.assigned_lreg = r;
                    lreg_busy[r] = true;
                    break;
                }
            }
        }

        if (iv.assigned_lreg >= 8) {
            return false; // Peak exceeds 8 physical LREGs
        }
    }
    return true;
}

// Atomic RTL Hard Register Substitution Pass
unsigned int execute_rvtt_pre_ira_alloc(function *fn) {
    basic_block bb;
    FOR_EACH_BB_FN(bb, fn) {
        rtx_insn *insn;
        std::vector<rtx_insn*> sfpu_island;
        
        FOR_BB_INSNS(bb, insn) {
            if (NONDEBUG_INSN_P(insn) && rvtt_sfpu_insn_p(insn)) {
                sfpu_island.push_back(insn);
            }
        }
        
        if (sfpu_island.empty()) continue;

        std::vector<live_interval> intervals;
        std::unordered_map<unsigned, unsigned> destructive_ties;
        extract_rtl_intervals(sfpu_island, intervals, destructive_ties);

        if (!allocate_interval_colors(intervals, destructive_ties)) {
            continue; // Fall back to IRA if colorability fails
        }

        std::unordered_map<unsigned, unsigned> regno_to_lreg;
        for (const auto& iv : intervals) {
            regno_to_lreg[iv.pseudo_regno] = iv.assigned_lreg;
        }

        validate_change_start();
        bool rewrite_ok = true;

        for (rtx_insn *cur_insn : sfpu_island) {
            subrtx_ptr_iterator::array_type array;
            FOR_EACH_SUBRTX_PTR(iter, array, &PATTERN(cur_insn)) {
                rtx *loc = *iter;
                if (*loc && REG_P(*loc) && !HARD_REGISTER_P(*loc)) {
                    unsigned p_regno = REGNO(*loc);
                    auto it = regno_to_lreg.find(p_regno);
                    if (it != regno_to_lreg.end()) {
                        rtx hard_reg = gen_raw_REG(GET_MODE(*loc), LREG_0 + it->second);
                        validate_change(cur_insn, loc, hard_reg, /*unique=*/true);
                    }
                }
            }
            if (recog_memoized(cur_insn) < 0) {
                rewrite_ok = false;
                break;
            }
        }

        if (rewrite_ok && verify_changes(0)) {
            confirm_change_group();
            df_insn_rescan_all();
        } else {
            cancel_changes(0);
        }
    }
    return 0;
}

} // namespace
```

---

## 5. Latency Scheduling & Hazard Elimination (The 40% Issue Win)

### 5.1 Separation of Modes

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

### 5.2 Latency Hiding & Independent Chain Interleaving

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

---

## 6. Tensix Coprocessor Lowering (Replay DP & `SFPLOADMACRO`)

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

### 6.1 Replay Buffer Packing via 0/1 Knapsack DP (`rtl-rvtt-replay.cc`)

* **Problem:** Given $M$ repeated candidate sequences $\{S_1, \dots, S_M\}$ in unrolled basic blocks, where candidate $S_i$ has instruction length $L_i$ and occurrence count $K_i$, select a non-overlapping subset to fit in the 32-slot hardware replay buffer maximizing instruction push savings:

$$\max \sum_{i \in \text{Selected}} (K_i - 1) \cdot L_i \quad \text{s.t.} \quad \sum_{i \in \text{Selected}} L_i \le 32$$

* **Algorithm (Bounded Knapsack DP in $O(M \cdot 32)$):**
  Since $W = 32$ and $M \le 64$, the optimal subset is computed in $<10\mu\text{s}$:

```cpp
int dp[33] = {0};
int parent[64][33];

for (int i = 0; i < M; ++i) {
    int weight = candidates[i].length;
    int profit = (candidates[i].occurrences - 1) * candidates[i].length;
    for (int w = 32; w >= weight; --w) {
        if (dp[w - weight] + profit > dp[w]) {
            dp[w] = dp[w - weight] + profit;
            parent[i][w] = 1;
        }
    }
}
```

- **Mockup Evidence:** On an 8-row unrolled loop, automatic replay compression reduces static Tensix instructions from **88 down to 19 on Wormhole (-78.4%)** and **56 down to 15 on Blackhole (-73.2%)**.

### 6.2 `SFPLOADMACRO` 4-Way Multi-Unit Concurrency

`SFPLOADMACRO` achieves **1 cycle/element steady-state throughput** by programming the 4 internal SFPU sub-units to execute in parallel across transient register $L_{16}$:

```
Cycle t:
┌──────────────────┐  ┌──────────────────┐  ┌──────────────────┐  ┌──────────────────┐
│  LOAD SUB-UNIT   │  │ SIMPLE ALU UNIT  │  │   MAD SUB-UNIT   │  │ STORE SUB-UNIT   │
│ Load Dst[i+2]->L0│  │ Mov L16 -> L1    │  │ MAD L0*L1->L16   │  │ Store L16->Dst[i]│
└──────────────────┘  └──────────────────┘  └──────────────────┘  └──────────────────┘
```

#### Structured GCC C++ Macro Region (`include/sfpi_macro.h`):

```cpp
#define SFPU_MACRO_BEGIN(template_id) \
    __builtin_rvtt_sfpconfig_macro_arm(template_id);

#define SFPU_MACRO_EXECUTE(dst_row, count) \
    __builtin_rvtt_sfploadmacro(dst_row, count);

#define SFPU_MACRO_END() \
    __builtin_rvtt_sfpconfig_macro_drain(); \
    __builtin_rvtt_sfpconfig_macro_disarm();
```

---

## 7. TT-LLK Kernel Corpus Analysis & Performance Potential

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

## 8. SOTA Vectorization: The MLIR / Triton Decoupled Compiler Architecture

```
┌─────────────────────────────────────────────────────────────┐
│                 High-Level Kernel Definition                │
│             (PyTorch / Triton / MLIR Linalg)                │
└──────────────────────────────┬──────────────────────────────┘
                               │
                               ▼
┌─────────────────────────────────────────────────────────────┐
│                  TT-Vector MLIR Dialect                     │
│  - First-class 32-lane vector types: `!tt.vfloat<32>`       │
│  - Explicit tile registers: `!tt.dst_tile<32, 32>`          │
│  - RWC tokens & Predication Masks                           │
└──────────────────────────────┬──────────────────────────────┘
                               │
             ┌─────────────────┴─────────────────┐
             ▼                                   ▼
┌─────────────────────────────┐     ┌─────────────────────────┐
│ Polyhedral Loop Tiling Pass │     │ MLIR Vector Unroll &    │
│ (Multi-Tile Welford / Norm) │     │ Register Tiling (L0..L7)│
└────────────┬────────────────┘     └────────────┬────────────┘
             │                                   │
             └─────────────────┬─────────────────┘
                               │
                               ▼
┌─────────────────────────────────────────────────────────────┐
│                 Target Machine Lowering                     │
│    (Auto Replay 32-Slot + SFPLOADMACRO Template Emit)       │
└─────────────────────────────────────────────────────────────┘
```

### MLIR TT-Vector Dialect Schema:

```mlir
// MLIR Representation of a Welford Online Accumulation Step:
%delta = tt_vector.sub %x, %mean : !tt.vfloat<32>
%new_mean = tt_vector.fma %delta, %recip, %mean {destructive_tie = 0} : !tt.vfloat<32>
%delta2 = tt_vector.sub %x, %new_mean {destructive_tie = 1} : !tt.vfloat<32>
%new_m2 = tt_vector.fma %delta, %delta2, %m2 {destructive_tie = 0} : !tt.vfloat<32>
```

---

## 9. Comprehensive Engineering Timeline & Validation Matrix

```
┌──────────────────────────────────────────────────────────────────────────────────────────────────┐
│                                 ENGINEERING EXECUTION TIMELINE                                   │
├─────────────┬────────────────────────────────────────────┬───────────┬───────────────────────────┤
│ Milestone   │ Key Deliverable                            │ LOC Delta │ Target Completion         │
├─────────────┼────────────────────────────────────────────┼───────────┼───────────────────────────┤
│ **M0 / M1** │ Guarded Default-On + RTL Liveness Audit    │ ~150 LOC  │ **Week 1 (Immediate)**    │
│ **M2**      │ Pre-IRA Hard LREG Island Allocator         │ ~500 LOC  │ **Weeks 2 - 4**           │
│ **M3**      │ Latency Mode & Dual-Horner Interleaving    │ ~300 LOC  │ **Weeks 4 - 6**           │
│ **M4**      │ Replay DP Solver + SFPLOADMACRO Regions    │ ~450 LOC  │ **Weeks 6 - 9**           │
│ **M5**      │ Blackhole Silicon Performance Benchmark   │ ~200 LOC  │ **Weeks 9 - 11**          │
│ **SOTA**    │ MLIR TT-Vector Dialect & Triton Lowering   │ ~3,500 LOC│ **Weeks 12 - 20**         │
└─────────────┴────────────────────────────────────────────┴───────────┴───────────────────────────┘
```

---

## 10. Counter-Rebuttal: Default-On Is an Engineering Decision, Not a Claim That the Roadmap Is Finished

A review that rejects default-on because every later roadmap component is not already implemented applies the wrong standard to this change. The decision at hand is narrower: whether the existing, allowlisted feasibility transform should automatically attempt to rescue high-pressure SFPU arithmetic.

The present implementation and the intended production policy:

| Component | Current checkpoint | Default-on target |
| :--- | :--- | :--- |
| Pressure scheduler flag | `Init(0)`; explicit opt-in | `Init(1)` for the existing WH/BH eligibility gate, with an explicit negative rollback flag |
| List scheduler | Implemented and independently validated | First automatic rescue attempt for peak-above-eight regions |
| MILP invocation | Explicit second flag | Demand-driven escalation only when list cannot produce a validated peak $\le 8$ order |
| MILP model | Exact bounded schedule-order model; 50k-node cap | Extend after M2 with physical assignment and latency objectives |
| Pre-IRA allocation pass | Dump-only liveness audit | Enforce and independently verify certified physical LREG assignment |
| Silicon harness | Validation specification and scaffold | Real producer/consumer tests comparing cycles against handwritten LLKs |

---

## 11. Critical Engineering Rebuttal and Corrected Execution Contract

The revised plan has the right strategic center: guarded default-on pressure
scheduling, exact escalation when heuristics miss, and final-RTL allocation
enforcement for a machine that cannot spill.  However, naming a classical
algorithm and presenting C++-shaped pseudocode do not by themselves make the
allocation design safe.  This section records the objections that must be
resolved before the roadmap is treated as an implementation specification.

The rebuttal is deliberately pro-execution.  It does **not** recommend
returning to indefinite default-off development.  It separates the small,
defensible default-on change from the harder M2, latency, replay, macro, and
MLIR projects so each can advance behind an honest contract.

### 11.1 Preserve the Default-On Decision, Narrow Its Claim

The immediate product decision is whether the existing WH/BH feasibility
scheduler should automatically attempt a rescue in the narrow regions it
already recognizes.  It is not a simultaneous decision to enable latency
reordering, replay restructuring, macro formation, rematerialization, or QSR.

The default-on implementation should be exactly:

```text
region fails eligibility
    -> unchanged

eligible region with source-order peak <= 8
    -> unchanged

eligible region with source-order peak > 8
    -> run deterministic list scheduler
       -> independently valid peak <= 8: commit list order
       -> otherwise, if a solver-enabled build is available:
            run bounded MILP
            -> OPTIMAL plus independent valid peak <= 8: commit MILP order
            -> anything else: unchanged
```

This differs from the current checkpoint in three concrete ways:

1. `riscv_tt_opt_pressure_schedule` is currently `Init(0)` and must be changed
   deliberately, with a test proving the generated negative option restores
   legacy behavior.
2. MILP is currently controlled by a second default-off option and is invoked
   for every eligible high-pressure region when requested, including regions
   for which list scheduling already supplied a feasible incumbent.  The
   production dispatch should return after a validated list solution and
   invoke MILP only on a list miss.
3. The current solver uses a deterministic 100,000-node cap.  The documented
   50,000-node and 20 ms limits are proposed values, not implementation facts.
   A wall-clock cutoff must not decide whether compiler output changes because
   it makes builds host-load dependent.  Use a deterministic work/node cap and
   measure its wall-time distribution instead.

The "non-negative delta" remains a useful operational intuition, but it is not
an unqualified theorem.  A late-GIMPLE source-order peak above eight does not
prove that legacy IRA would fail; fortunate coalescing can make some such
programs compile.  Default-on therefore requires a whole-corpus legacy versus
new-default differential.  This is a bounded and testable rollout obligation,
not a reason to disable the feature permanently.

The existing evidence is strong enough to justify that rollout:

- 1,106 expected compiler passes, two expected failures, and no unexpected
  failures, errors, or unresolved tests;
- deterministic serial/parallel builds and validator rejection self-tests;
- WH/BH Welford-shaped and unrelated fused-DAG 9-to-8 rescues;
- 18 of 18 recurrent LLK EMA off/list/MILP correctness executions; and
- broad candidate-toolchain LLK runs with 34,776 Blackhole passes and 35,714
  Wormhole passes.

Those broad runs are meaningful compatibility evidence, not a clean
whole-corpus scheduler A/B and not silicon performance evidence.  The next
gate is to compile the entire eligible corpus off/default, archive every
changed assembly hash, and execute the changed set through simulator and
available silicon correctness suites.

### 11.2 Correct the Goodman-Hsu Citation and Use It as Policy, Not Proof

The relevant prior art is James R. Goodman and Wei-Chung Hsu, *Code Scheduling
and Register Allocation in Large Basic Blocks*, ICS 1988, pages 442--452,
DOI [`10.1145/55364.55407`](https://doi.org/10.1145/55364.55407).  Later work
often calls the technique integrated prepass scheduling (IPS), but "Code
Scheduling to Reduce Register Pressure (IPS)" is not the paper title.

Its central idea fits SFPU well: use latency-oriented scheduling while
registers are available, then switch emphasis toward freeing registers as the
budget becomes scarce.  It supports the proposed dual-mode policy.  It does
not establish that seven is the correct switch threshold, that the shown
ready-list priorities are optimal, or that a pressure-safe order is physically
allocatable on this target.  Those are target-specific hypotheses requiring
corpus and silicon measurement.

The first implementation should parameterize and dump the switching decision:

```text
mode=P reason=live-count>=threshold live=7 threshold=7
mode=L reason=capacity-slack live=5 threshold=7
```

Run thresholds 6, 7, and 8 over the same WH/BH corpus.  Select the default from
compile success, final NOPs, code size, and device cycles rather than from the
historical algorithm's authority.

### 11.3 The Current MILP and the Target Joint Model Must Remain Distinct

Today's `rvtt_sched_problem` contains stable operation IDs, dependencies,
logical value definitions/uses, live-out markers, a register-capacity scalar,
and preferred issue slots.  The implemented solver chooses an operation order
and validates logical liveness.  It does **not** yet contain physical
`assign[v,r]`, `occupy[v,r,t]`, destructive `alias[i,v]`, target latency,
resource occupancy, copy cost, or a machine makespan objective.

Therefore Section 3.1 describes the target M2/M3 joint model, not the current
solver.  That distinction should remain explicit in code review and release
notes.  "Exact" means exact relative to the bounded model being solved; it
does not currently mean machine-optimal schedule and allocation.

The proposed weighted objective is also not automatically lexicographic:

```text
10000 * peak + 100 * makespan + copies
```

It is lexicographic only if proven upper bounds guarantee that the maximum
possible sum of all lower-priority terms is less than one unit of the next
priority.  Prefer sequential solves with the previous optimum fixed, or
derive and assert safe weights from region bounds.

Makespan must be represented by an explicit variable:

```text
finish >= issue_time[i] + latency[i]    for every operation i
minimize finish
```

A term involving `issue[last,t]` is valid only if a unique distinguished sink
is guaranteed to complete last, which is not true for a general arithmetic
DAG.

The 11-to-8 fixture must also be worded precisely.  Its **source-order peak**
is eleven; it does not have eleven simultaneous region live-ins.  Eleven true
live-ins cannot be reduced by scheduling inside the region.  The fixture
proves that exact schedule search can beat the current list heuristic and that
logical feasibility alone does not force IRA to realize the coloring.  The
remaining spill is a precise M2 reproducer, not evidence against MILP.

### 11.4 The Proposed Left-Edge Allocator Is Not Correct as Written

Classical left-edge coloring is optimal for ordinary interval graphs.  The
M2 problem is an interval graph plus fixed colors, forbidden colors,
instruction alternatives, hard-register clobbers, boundary values, and
destructive equality constraints.  A naive left-edge extension is not a
complete allocator for that problem.

The pseudocode has several immediate correctness defects:

1. `assigned_lreg` has no initializer, so testing it against eight reads an
   indeterminate value.
2. RTL instruction UIDs are used as time positions.  UIDs are identities, not
   guaranteed final-layout order after instruction insertion and movement.
3. `lreg_busy` is cleared by scanning every historical interval.  If an
   expired interval and a currently active interval reused the same color,
   processing the expired interval can falsely mark the active color free.
4. A destructive tie copies the operand's color without proving that the
   operand dies at that instruction, that the machine alternative allows the
   overlap, or that another active interval does not own the color.
5. Every SFPU instruction in a basic block is appended to one island while
   intervening non-SFPU instructions are skipped.  This can reorder allocation
   reasoning across scalar dependencies, asm, calls, CC/configuration changes,
   RWC/Dst state, synchronization, replay, or other barriers.
6. Only pseudo occurrences inside selected SFPU instructions are replaced.  A
   pseudo defined before, used after, or referenced by an intervening
   instruction would be partially converted without a boundary copy or
   equivalence proof.
7. Existing hard LREG uses, live-across hard registers, clobbers, subregs,
   composite modes, and recognized alternative constraints are absent from
   the color model.
8. `LREG_0`, `rvtt_sfpu_insn_p`, `extract_rtl_intervals`, and
   `validate_change_start` are not checked-in GCC/SFPI interfaces.  The actual
   hard-register base is `SFPU_REG_FIRST`.
9. Pending grouped changes, recognition caches, dataflow rescans, register
   notes, and the final no-pseudo condition require a more complete
   transaction than the shown loop provides.

The active-set bug alone is sufficient to reject the pseudocode as an
implementation template.  Consider intervals A, B, and C where A uses L0 and
expires, B later reuses L0 and remains live, and C begins.  Scanning expired A
clears L0; scanning active B does not restore it; C can then be assigned L0
while overlapping B.

### 11.5 Correct M2 Architecture: Final-RTL Constraints Plus Exact Local Coloring

M2 should treat final pre-IRA RTL as authoritative.  The GIMPLE certificate is
a schedule seed and diagnostic correlation point; expansion, sched1, and
early rematerialization may change the graph.

The authoritative pipeline should be:

```text
1. Discover one contiguous final-RTL island.
2. Terminate at every unmodeled instruction or architectural-state boundary.
3. Assign dense layout positions 0..N-1; never use instruction UID as time.
4. Extract every XTT32 pseudo def/use, hard-register use/clobber, mode,
   recognizer alternative, live-in, live-out, and fixed/precolored value.
5. Prove which ordinary dying-operand overlaps are machine-legal.
6. Keep `_lv` semantic ties separate from ordinary destructive coalescing.
7. Build equality classes only for mandatory, independently proven ties.
8. Reject an equality class whose members overlap or have incompatible
   precolors.
9. Build interference, allowed-color, and fixed-color constraints.
10. Solve the eight-color problem deterministically.
11. Independently validate every position, color, tie, clobber, and boundary.
12. Apply all substitutions as one grouped RTL transaction.
13. Verify every changed instruction remains recognized, rebuild dataflow,
    require zero island SFPU pseudos, and revalidate the final stream.
14. On any failure, cancel the complete group and leave RTL unchanged.
```

Because regions are intentionally small and there are only eight colors, the
authoritative solver should be exact bounded DSATUR/backtracking or an
equivalent constraint solver.  Left-edge is useful as a fast incumbent but
need not be trusted as the completeness boundary.

A deterministic coloring search can prioritize:

1. precolored/equality-contracted classes;
2. smallest allowed-color set;
3. highest saturation degree;
4. highest interference degree;
5. stable pseudo/position ID.

The checker must be algorithmically independent of the search.  It receives a
mapping and proves:

- each value has one legal color;
- all precolors are honored;
- every interfering pair differs;
- every mandatory tie agrees;
- every tied operand dies at the allowed issue boundary;
- every physical hard-register use/clobber is respected;
- every rewritten instruction is recognized; and
- no selected SFPU pseudo remains.

Initially accept only fully closed islands: every colored pseudo is defined
and used exclusively inside the island, with fixed hard-register boundary
operands modeled explicitly.  Boundary-copy insertion can be a later feature.

The 11-to-8 fixture is the first positive exit test, but M2 also needs:

- exhaustive agreement with brute force on small random constrained interval
  graphs;
- deliberate overlapping-tie, incompatible-precolor, clobber, subreg,
  noncontiguous-island, and live-across rejection fixtures;
- transactional failure tests proving pass-off/failure RTL identity;
- final assembly matching the accepted certificate on WH and BH; and
- ASan/UBSan checking builds plus serial/parallel determinism.

### 11.6 Replay Selection Is Conflict-Constrained Packing, Not Plain Knapsack

The current replay pass correctly observes that buffer selection has a
knapsack component.  The proposed `O(M * 32)` 0/1 DP is insufficient because
candidate sequences are not independent items.

Candidates can overlap in source occurrences, contain one another, compete
for the same repeated instructions, require one of several contiguous free
buffer spans after explicit user reservations, and have non-overlapping
lifetimes that could permit slot reuse.  Profit must also subtract capture and
playback overhead rather than count only `(occurrences - 1) * length`.

The exact formulation needs:

- a candidate conflict graph over overlapping/incompatible occurrences;
- available contiguous replay spans after all explicit reservations;
- a placement choice assigning each selected candidate to a fitting span;
- capture/playback and code-size cost in the objective;
- deterministic tie-breaking; and
- independent validation of the final capture/playback stream.

For the current small 32-slot buffer, bounded branch-and-bound or DP over
conflict components and span occupancy is practical.  Plain capacity-only
knapsack can select mutually incompatible candidates and is not a safe
replacement for the existing greedy logic.  The displayed `parent[64][33]`
table also lacks complete initialization and reconstruction and should remain
illustrative rather than implementation pseudocode.

Replay's measured 73--78% reduction is static frontend stream size in the
mockup, not an equivalent backend-cycle reduction.  Device value depends on
frontend starvation and must be measured separately.

### 11.7 `SFPLOADMACRO` Requires an Event Model Before a Public Macro API

The proposed begin/execute/end surface is attractive but hides the hard
semantics: template words, sequence words, cycle-versus-instruction delays,
sub-unit collisions, outstanding-event drain, L16 lifetime, Dst/RWC effects,
scoreboarding omissions, and architecture-specific teardown.

The named arm, drain, and disarm builtins do not exist today.  More
importantly, a generic `drain()` cannot be specified until the exact WH/BH
pending-event behavior is modeled and validated.

Implement one target-internal `sfpu_macro_region` descriptor first.  It must
contain explicit configuration ownership, scheduled sub-unit events,
architectural state inputs/outputs, maximum outstanding latency, collision
proof, teardown, and a plain-loop fallback.  Lower and validate one kernel
family on both architectures before exposing a stable source API.

The 1.33--4.0x figures are theoretical steady-state issue-rate opportunities
from known schedules, not measured application speedups.  Preserve that label
until silicon counters establish the realized gain.

### 11.8 MLIR/Triton Is a Separate Multi-Quarter Program

A TT vector dialect can eventually make vector semantics, Dst layout,
predication, RWC state, replay ownership, and macro regions explicit.  That is
a valuable direction, but it is not a 3,500-line extension that predictably
becomes a competitive Triton backend in eight weeks.

The high-level dialect should express semantics, not prematurely force a
physical tie.  A `destructive_tie` attribute must at least define the operand
index, whether the tie is mandatory or preferred, its inactive-lane behavior,
legal fallback, and target-specific alternatives.  Prefer semantic Welford
operations at the high level, liveness/bufferization in a lower vector
dialect, and physical tie alternatives only in the machine-level dialect.

Claims about outperforming CUDA/PTX or TPU/XLA require defined workloads,
hardware, numerical policy, compiler baselines, and measurements.  They do
not belong in the acceptance criteria for the SFPI pressure scheduler.

### 11.9 Revised Execution Plan and Credible Gates

#### P0 — Guarded default-on feasibility scheduling

- Flip the existing narrow WH/BH pressure-scheduler default.
- Verify and test the negative rollback option.
- Return immediately after a validated list solution.
- Invoke MILP automatically only on list failure in solver-enabled builds.
- Retain solver-free builds and deterministic work caps.
- Add compiler/JIT cache-key coverage.
- Run whole-corpus off/default assembly differential and execute every changed
  LLK through correctness gates.

Expected scope: one to two focused weeks, excluding hardware queue time.

#### P1 — Final-RTL model and independent checker

- Define contiguous island boundaries and dense positions.
- Extract full pseudo/hard-register constraints and state barriers.
- Implement the independent final-RTL allocation checker first.
- Fuzz the checker against exhaustive small instances.

Expected scope: two to four weeks.

#### P2 — Exact local M2 allocation

- Add deterministic exact eight-color search with left-edge/DSATUR incumbent.
- Apply atomic hard-register substitution only to closed islands.
- Verify recognition, dataflow, final coloring, and zero pseudos.
- Make the 11-to-8 fixture compile and add adversarial rejection tests.

Expected scope: three to eight additional weeks depending on GCC invariants.

#### P3 — Real silicon Welford validation

- Replace `assert True` with an actual producer/consumer launch.
- Export raw mean and M2 and compare numerical classifications and ULPs.
- Use the correct observation-dependent reciprocal sequence.
- Export real device cycle deltas.
- Compare off, list, MILP, manual early-fold, handwritten direct, and
  handwritten replay using identical surrounding work.

This runs in parallel with P1/P2 and gates Welford replacement/performance
claims, not compilation-rescue rollout.

#### P4 — Latency scheduling

- Add audited WH/BH latency and resource descriptors.
- Implement Goodman-Hsu-style mode switching with measured thresholds.
- Preserve final hazard verification/repair.
- Validate Dual-Horner and addcmul with static traces and silicon cycles.

Expected scope: four to eight weeks plus hardware validation.

#### P5 — Separate follow-on proposals

Replay selection, `SFPLOADMACRO` formation, and MLIR/Triton lowering require
separate owners, design documents, risk reviews, and acceptance matrices.
Bundling their schedules hides rather than reduces their complexity.

### 11.10 Final Position

Proceed aggressively with guarded default-on feasibility scheduling.  Make
MILP the automatic exact escalation on list misses.  Treat the remaining IRA
spill as the best available M2 reproducer, not as a reason to retreat.

At the same time, do not implement the shown Left-Edge allocator literally,
do not describe the target joint MILP as today's solver, and do not convert
static issue/code-size opportunities into silicon speedup claims.  A correct
final-RTL constraint model and exact small-region coloring search are the
shortest credible route from the current 9-to-8 rescues to a general
allocation guarantee.

This is the engineering-aware compromise: aggressive deployment where the
existing validator contains risk, exactness where eight non-spillable
registers leave no margin for heuristic allocation, and measured claims at the
hardware boundary.

---

## 12. Conclusion

Adopting the **guarded default-on feasibility policy** makes validated SFPU
pressure rescue automatic for the narrow WH/BH regions supported today while
retaining deterministic fallback and an operational rollback.  Completing M2
with an independently certified final-RTL allocation will extend that rescue
to logically feasible schedules that generic IRA still misses.

Latency scheduling, replay selection, and `SFPLOADMACRO` formation remain
separate opportunities whose static models justify implementation and silicon
experiments, not completed performance wins.  Pursued in that order—with exact
allocation at the zero-spill boundary and hardware measurements at the
performance boundary—the program provides a credible path from fragile manual
LREG scheduling toward reliable compiler-generated SFPU code.
