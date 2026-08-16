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

## 2. Guarded Default-On Policy: The Testable Rescue Contract

### 2.1 The Evidence-Based Rescue Contract

A source-order GIMPLE peak above eight does not guarantee that baseline GCC will spill or abort, because fortunate downstream coalescing in IRA can occasionally allocate tight graphs. Therefore, rather than claiming an unqualified non-negative delta theorem, default-on deployment is governed by an **Evidence-Based Rescue Contract**:

1. **Strict Region Allowlist:** Regions outside the positively classified arithmetic allowlist (`sfpadd`, `sfpmul`, `sfpmad` on Wormhole/Blackhole) are left **100% untouched**.
2. **Threshold Invariant:** Regions with source-order peak at or below eight are left **100% untouched** (byte-identical assembly).
3. **Transactional Independent Validation:** A proposed schedule is committed if and only if an independent validator proves def-before-use precedence, source availability, exact liveness, and peak $\le 8$.
4. **Whole-Corpus Differential Testing:** The whole eligible corpus is compiled legacy-off versus proposed-default to classify every changed binary through simulator and silicon suites.
5. **Tested Operational Rollback:** The explicit rollback flag (`-mno-tt-tensix-optimize-pressure-schedule`) remains supported and verified in CI.

```
                             RESCUE CONTRACT FLOW
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
             (100% Byte-Identical)          (Transactional Fallback)
                                                      │
                                   ┌──────────────────┴──────────────────┐
                                   ▼                                     ▼
                            Rescue Succeeds                       Rescue Fails
                           (Compiles Cleanly)                 (Preserves Legacy State)
```

### 2.2 Checked-In Tree Ground Truth vs. Target Implementation

| Dimension | Checked-In Checkpoint (`tree`) | Target Production Architecture |
| :--- | :--- | :--- |
| **GIMPLE Pressure Scheduler** | **Real** (`gimple-rvtt-lp-schedule.cc`, 1025 lines) | Fast-path list + independent schedule validator |
| **Exact MILP Engine** | **Real** and intentionally available whenever explicitly requested for an over-pressure region, including when list scheduling succeeds (`rvtt-lpsolve.cc`, 482 lines via `lp_solve`) | First-class bounded exact lane: pressure rescue, optimality oracle, and multi-objective latency/reuse/replay optimization under deterministic node/time caps |
| **Driver Flags** | `Init(0)` (Explicit opt-in required) | `Init(1)` default-on for WH/BH allowlist + rollback option |
| **Pre-IRA Physical Allocator** | Dump-only stub (`rtl-rvtt-lp-alloc.cc`, 133 lines) | **Conditional M2:** build exact transactional coloring only when corpus evidence demonstrates recurring baseline-IRA failures after ownership modeling |
| **Corpus Scorer / Differential Driver** | **Two real layers:** this repository's `scripts/run-corpus-score.sh` has one Welford entry; TT-Metal `da3832b31d` provides `tt_metal/tt-llk/tests/corpus/sfpu_corpus.py` with 164 logical implementations / 332 architecture paths, exact pytest-node attribution, compiler capability/pin provenance, CRAQ and serialized-silicon modes, plus identical-source flag-off/flag-on executable-`.text` classification in isolated build roots. | **P0:** run that changed-binary lane on selected scheduler rows, then require paired scoped silicon A/B; structure alone is not performance evidence |
| **Hardware Silicon Baseline** | **GO-BH-ONLY** (Blackhole **p100a**-era record, pre-planner compiler lineage — per the §18 reading discipline every number carries chip class + compiler era): 3 generated wins (Welford 323 vs 326, Reduce-SDPA 834 vs 840, Reciprocal 459 vs 467), 1 tie (Binary broadcast 608), 1 parity (Addcmul +0.02%), and understood throughput gaps. Authoritative current scoreboard: **§18.8.0.4** (p150). Primary archive in `validation/welford-bh-20260815/`. | **Open:** Wormhole silicon and an identical-source, changed-binary pressure-scheduler A/B (§14). |

```
Candidate Region (Peak > 8)
       │
       ├──► 1. Fast Path: Deterministic List Scheduler (<0.1ms compile time)
       │         │
       │         └──► [Valid Peak <= 8] ──► Commit GIMPLE Rewrite
       │
       ├──► 2. Exact Path: Bounded MILP Solver (Explicit/CI/oracle mode; hard node/time caps)
       │         │
       │         └──► [Valid Peak <= 8] ──► Commit GIMPLE Rewrite
       │
       └──► 3. Safe Fallback: Leave GIMPLE untouched (preserves legacy compilation state)
```

### 2.3 Implementation Audit and Default-On Decision (2026-08-15)

**Decision: keep `-mtt-tensix-optimize-pressure-schedule` default-off until the P0 evidence gates
below are satisfied.** This is not a claim that one IRA spill is catastrophic, nor a demand that
M2 exist before any useful scheduler can ship. It is the narrower conclusion required by the
Evidence-Based Rescue Contract: the local transformation is well defended, but the promised
deployment evidence does not yet exist.

What is implemented and credible today:

- The WH/BH `sfpadd`/`sfpmul`/`sfpmad` allowlist, `old_peak > 8` bypass, transactional rewrite,
  independent certificate validation, deterministic scheduling, and explicit rollback flag are
  real. Focused WH/BH/QSR, debug, predication, CFG, constant-LREG, malformed-certificate, and
  deterministic-build tests cover the local safety envelope.
- The scheduler commits only a strictly lower independently recomputed peak that is at most eight;
  otherwise it preserves the original GIMPLE order.
- The raw-LREG ownership pass is a separate, enforcing pre-IRA mechanism: it emits sentinel
  definitions/uses so IRA sees annotated raw-LREG lifetimes. It must not be confused with the
  dump-only `rtl-rvtt-lp-alloc.cc` pressure auditor.

What prevents default-on promotion:

1. **The identical-source pressure-scheduler differential is absent.** The broad TT-Metal corpus
   runner exists at `tt_metal/tt-llk/tests/corpus/sfpu_corpus.py` (TT-Metal `da3832b31d`) and tracks
   164 logical implementations / 332 architecture paths with per-node outcomes.  This repository's
   `run-corpus-score.sh` remains a useful Welford bring-up harness.  Neither currently performs the
   required scheduler flag-off/flag-on changed-binary classification, which remains the P0 delta.
2. **No changed-binary scheduler silicon A/B exists.** The Welford scheduler-off/on arms tied
   because the measured body bypassed the scheduler. That establishes no regression on that input,
   not a silicon scheduler win. At least one `old_peak > 8` corpus kernel must produce different
   assembly and pass paired correctness/cycle measurement with the flag as the only variable.
3. **The exact MILP's optimization potential is not yet productized.** Invoking it on request even
   when list scheduling succeeds is intentional and useful: it enables exact optimality studies
   rather than assuming a feasible heuristic result is best. The current objective, however,
   minimizes deviation from the preferred list order once that order already fits, so it proves
   feasibility without exploring latency, destructive reuse, rematerialization, or replay value.
   Preserve the bounded exact-on-request path; add an oracle/CI mode and lexicographic objectives
   for pressure, physical makespan, copies/reuse, constant placement, and replay opportunity. Use
   measured solver distributions—not a blanket list-miss rule—to choose the eventual production
   invocation policy.
4. **Physical colorability is not enforced by M2.** `rtl-rvtt-lp-alloc.cc` still reports
   `colorability=unchecked`. This does not automatically block a scheduler win—baseline IRA has
   allocated several real cases—but any corpus case that reaches peak eight and then spills must
   be classified, not dismissed. Build M2 only if the corpus demonstrates that pressure rescue
   repeatedly fails at physical coloring; until then, treat M2 as an independent P1 gap rather
   than a prerequisite invented for already-safe cases.

**Promotion gate:** switch the WH/BH allowlist to `Init(1)` only after (a) the differential driver
classifies every changed eligible binary, (b) all correctness suites are green, (c) at least one
changed-binary identical-source silicon A/B is archived, (d) rollback is exercised in CI, and
(e) every new spill is either eliminated or explicitly accepted with measured impact. Performance
need not improve on every changed kernel, but the default-on set must be non-regressing under the
published acceptance threshold; transformations outside that set stay behind the rollback flag.

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

### 3.1 Non-Normative Target M2/M3 Joint MILP Model Outline

The target joint MILP optimizer models simultaneous instruction scheduling, exact liveness linearization, physical register assignment, destructive operand reuse, and critical path makespan over a bounded horizon $T = \sum_{i=1}^N \max(1, \text{latency}(i))$:

#### Variables:
- $\text{issue}_{i,t} \in \{0, 1\}$: Binary indicator that operation $i \in \{1 \dots N\}$ is issued in time slot $t \in \{1 \dots T\}$.
- $\text{live}_{v,t} \in [0, 1]$: Continuous variable (linearized to $0$ or $1$) indicating SSA value $v$ is live at slot $t$.
- $\text{assign}_{v,r} \in \{0, 1\}$: Binary assignment of value $v$ to physical register $r \in \{0 \dots 7\}$.
- $\text{occupy}_{v,r,t} \in [0, 1]$: Linearized conjunction $\text{live}_{v,t} \land \text{assign}_{v,r}$.
- $\text{alias}_{i,v} \in \{0, 1\}$: Result of operation $i$ destructively overwrites operand value $v$.
- $\text{finish} \in \mathbb{R}^+$: Maximum completion time across all sinks in the DAG.

#### Constraints & Linear Implications:
1. **Single Issue:** Every operation issues exactly once:
   $$\sum_{t=1}^T \text{issue}_{i,t} = 1 \quad \forall i$$
2. **Resource Capacity:** At most one SFPU operation issues per cycle:
   $$\sum_{i=1}^N \text{issue}_{i,t} \le 1 \quad \forall t$$
3. **Dataflow Precedence & Latency:** For every true dependency $(u, v) \in E$:
   $$\sum_{t=1}^T t \cdot \text{issue}_{v,t} \ge \sum_{t=1}^T t \cdot \text{issue}_{u,t} + \text{latency}(u)$$
4. **General Makespan Formulation:**
   $$\text{finish} \ge \sum_{t=1}^T t \cdot \text{issue}_{i,t} + \text{latency}(i) \quad \forall i \in \{1 \dots N\}$$
5. **Exact Liveness Linearization:** Value $v = \text{def}(i)$ becomes live immediately after slot $t(i)$ and remains live until the latest consuming slot $\max_{w \in \text{uses}(v)} t(w)$.
6. **Single Physical Register Assignment:** Each value $v$ is assigned exactly one physical register:
   $$\sum_{r=0}^7 \text{assign}_{v,r} = 1 \quad \forall v$$
7. **Linearized Register Occupancy:**
   $$\text{occupy}_{v,r,t} \ge \text{live}_{v,t} + \text{assign}_{v,r} - 1, \quad \text{occupy}_{v,r,t} \le \text{live}_{v,t}, \quad \text{occupy}_{v,r,t} \le \text{assign}_{v,r}$$
8. **Physical Register Mutual Exclusion:** At most one live value occupies physical register $r$ at cycle $t$:
   $$\sum_{v=1}^V \text{occupy}_{v,r,t} \le 1 \quad \forall r \in \{0 \dots 7\}, \forall t$$
9. **Mandatory Destructive Selection for 2-Address Operations:**
   $$\sum_{v \in \text{valid\_ops}(i)} \text{alias}_{i,v} = 1$$
10. **Linearized Destructive Implications:**
    $$\text{assign}_{\text{def}(i), r} - \text{assign}_{v, r} \le 1 - \text{alias}_{i,v} \quad \forall r \in \{0 \dots 7\}$$
    $$\text{assign}_{v, r} - \text{assign}_{\text{def}(i), r} \le 1 - \text{alias}_{i,v} \quad \forall r \in \{0 \dots 7\}$$
11. **Schedule-Dependent Final Use Ordering:** When $\text{alias}_{i,v} = 1$, every other consumer $u \in \text{uses}(v) \setminus \{i\}$ must issue before or at the same cycle as $i$:
    $$\sum_{t=1}^T t \cdot \text{issue}_{i, t} \ge \sum_{t=1}^T t \cdot \text{issue}_{u, t} - T \cdot (1 - \text{alias}_{i, v}) \quad \forall u \in \text{uses}(v) \setminus \{i\}$$

#### Multi-Tier Lexicographic Objective (Sequential Solves):
1. **Solve 1 (Feasibility):** Minimize peak register occupancy: $\min \max_t \sum_v \text{live}_{v,t} \le 8$.
2. **Solve 2 (Makespan):** With peak fixed $\le 8$, minimize makespan: $\min \text{finish}$.
3. **Solve 3 (Coalescing):** With makespan fixed, minimize copies and deviation from list schedule.

### 3.2 Classical Prior Art: Goodman-Hsu Dual-Mode Scheduling
* **Prior Art:** James R. Goodman and Wei-Chung Hsu, *Code Scheduling and Register Allocation in Large Basic Blocks*, ICS 1988, pp. 442–452, DOI: [`10.1145/55364.55407`](https://doi.org/10.1145/55364.55407).
* **Operational Modes:**
  1. **Pressure-Reduction Mode (P-Mode):** When live count $\ge K - \delta$ (parameterized across thresholds 6, 7, and 8), prioritize nodes that kill the most live values.
  2. **Latency-Minimization Mode (L-Mode):** When live count $< K - \delta$, prioritize critical-path latency and independent chain interleaving.
* Telemetry dumps (`mode=P`/`mode=L`) record switching decisions for corpus calibration.

### 3.3 Why MILP is Essential: The 11-to-8 Optimality Proof

While greedy list heuristics work for simple expressions, register-constrained DAG scheduling is strongly NP-hard. The checked-in fixture `scripts/lp-schedule-milp-beats-list.C` provides concrete mathematical proof of MILP's necessity:

- **Graph Structure:** A 10-operation arithmetic DAG with 11 source-order peak live values.
- **List Heuristic Result:** Trapped in a local pressure minimum; fails to reduce peak below 9.
- **MILP Result:** Explores the full combinatorial search space, finds an exact sequence of destructive reuses, and produces a validated **11 $\to$ 8** schedule.

---

## 4. Milestone M2: Physical Register Allocation Enforcement (Authoritative Architecture)

### 4.1 The Final-RTL Constraint Architecture

Milestone M2 treats final pre-IRA RTL as authoritative. To eliminate the IRA spill hazard permanently, M2 executes the following 14-step pipeline on strict, contiguous closed islands across the entire basic block:

```
┌─────────────────────────────────────────────────────────────────────────┐
│              Authoritative Pre-IRA M2 Allocation Pipeline               │
├─────────────────────────────────────────────────────────────────────────┤
│ 1. Iterate basic blocks to discover all contiguous final-RTL islands    │
│ 2. Terminate at any unmodeled instruction, call, branch, or barrier     │
│ 3. Assign dense layout positions 0..P-1 across island instructions      │
│ 4. Extract all XTT32 pseudo def/use, hard-register clobbers & modes     │
│ 5. Validate input model shape, symmetry, mask bounds & normalized colors│
│ 6. Certify machine-legal dying operand overlaps via certify_destructive │
│ 7. Build equality classes only for certified ties via Union-Find        │
│ 8. Intersect allowed color masks, reconcile precolors, build conflict G │
│ 9. Solve exact 8-coloring via bounded DSATUR with explicit status enums │
│ 10. Precommit verification: prove global function-level pseudo closure  │
│ 11. Stage replacements & debug resets via validate_change() (0 watermark)│
│ 12. Run occurrence-level independent validator on fully staged patterns │
│ 13. Confirm change group, rebuild DF, assert ZERO SFPU pseudos in island│
│ 14. On any precommit failure, cancel group (0) and leave RTL untouched  │
└─────────────────────────────────────────────────────────────────────────┘
```

### 4.2 Exact Bounded DSATUR / Backtracking Coloring Engine

```cpp
// Target Implementation for gcc/gcc/config/riscv/tt/rtl-rvtt-lp-alloc.cc:
#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "backend.h"
#include "rtl.h"
#include "df.h"
#include "insn-config.h"
#include "recog.h"
#include "rvtt.h"

#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <functional>

namespace {

enum class tie_kind { NONE, PERMITTED, MANDATORY_2ADDR, LV_PREDICATION };

enum class m2_solve_status {
    SOLVED,
    INVALID_MODEL,
    UNSAT,
    SEARCH_LIMIT
};

struct m2_pass_telemetry {
    unsigned extraction_reject_count = 0;
    unsigned tie_cert_reject_count = 0;
    unsigned invalid_model_count = 0;
    unsigned unsat_count = 0;
    unsigned search_limit_count = 0;
    unsigned closure_reject_count = 0;
    unsigned recog_reject_count = 0;
    unsigned validator_reject_count = 0;
    unsigned committed_count = 0;
};

struct destructive_tie_candidate {
    unsigned result_val;
    unsigned operand_val;
    unsigned op_index;
    int insn_pos;
    int operand_death_pos;
    int selected_alternative;
    tie_kind requested_kind;
};

struct certified_destructive_tie {
    unsigned result_val;
    unsigned operand_val;
    unsigned op_index;
    int insn_pos;
    int operand_death_pos;
    tie_kind kind;
    int selected_alternative;
};

struct staged_occurrence_record {
    rtx_insn *insn;
    rtx *loc;
    unsigned original_pseudo;
    unsigned expected_hard_reg;
    machine_mode mode;
};

struct rtl_interval {
    unsigned pseudo_regno;
    int start_pos;              // Dense layout index 0..P-1 [start, end)
    int end_pos;                // Dense layout index 0..P-1
    uint8_t allowed_color_mask; // Bitmask of legal LREGs (must be subset of 0xFF)
    int fixed_color;            // Normalized integer: -1 or 0..7
};

struct m2_color_node {
    unsigned stable_id;
    uint8_t allowed_color_mask;
    int fixed_color;
    std::vector<unsigned> member_values;
};

// Disjoint Set Union (Union-Find) for Mandatory Equality Ties
struct union_find {
    std::vector<unsigned> parent;
    union_find(size_t n) : parent(n) { for (size_t i = 0; i < n; ++i) parent[i] = i; }
    unsigned find(unsigned i) { return parent[i] == i ? i : parent[i] = find(parent[i]); }
    void unite(unsigned i, unsigned j) { parent[find(i)] = find(j); }
};

// Authoritative Destructive Tie Certification using GCC 15 Constraint Alternatives
bool certify_destructive_tie(rtx_insn *insn,
                             const destructive_tie_candidate &cand,
                             certified_destructive_tie &out_tie) {
    if (cand.operand_death_pos != cand.insn_pos) return false; // Must die exactly at issue boundary
    
    extract_insn(insn);
    preprocess_constraints(insn);
    int icode = INSN_CODE(insn);
    if (icode < 0) return false;

    int n_alts = insn_data[icode].n_alternatives;
    if (cand.selected_alternative < 0 || cand.selected_alternative >= n_alts) return false;
    if (cand.op_index >= (unsigned)insn_data[icode].n_operands) return false;

    // Verify destination and operand modes match
    if (GET_MODE(recog_data.operand[0]) != GET_MODE(recog_data.operand[cand.op_index])) {
        return false;
    }

    // Inspect preprocessed operand alternative to verify matching constraint (e.g. '0')
    const operand_alternative *op_alt = &recog_op_alt[cand.selected_alternative * recog_data.n_operands];
    if (op_alt[cand.op_index].matches != 0) {
        return false; // Operand does not match result register 0 in this alternative!
    }

    out_tie.result_val = cand.result_val;
    out_tie.operand_val = cand.operand_val;
    out_tie.op_index = cand.op_index;
    out_tie.insn_pos = cand.insn_pos;
    out_tie.operand_death_pos = cand.operand_death_pos;
    out_tie.kind = cand.requested_kind;
    out_tie.selected_alternative = cand.selected_alternative;
    return true;
}

// Dimensional Model Validation Before Graph Search
bool validate_extracted_model(size_t num_positions,
                              const std::vector<rtl_interval>& raw_intervals,
                              const std::vector<std::vector<bool>>& raw_interference,
                              const std::vector<certified_destructive_tie>& ties) {
    size_t num_vals = raw_intervals.size();
    if (num_vals == 0 || num_positions == 0 || raw_interference.size() != num_vals) return false;

    std::unordered_set<unsigned> seen_pseudos;
    for (size_t i = 0; i < num_vals; ++i) {
        if (raw_interference[i].size() != num_vals) return false;
        if (raw_interference[i][i]) return false; // Diagonal self-interference forbidden
        if (raw_intervals[i].start_pos < 0 || raw_intervals[i].start_pos >= (int)num_positions) return false;
        if (raw_intervals[i].end_pos <= raw_intervals[i].start_pos || raw_intervals[i].end_pos > (int)num_positions) return false;
        if (raw_intervals[i].allowed_color_mask == 0) return false;
        if (raw_intervals[i].fixed_color < -1 || raw_intervals[i].fixed_color > 7) return false;
        if (raw_intervals[i].fixed_color != -1 && 
            !(raw_intervals[i].allowed_color_mask & (1 << raw_intervals[i].fixed_color))) {
            return false;
        }
        if (!seen_pseudos.insert(raw_intervals[i].pseudo_regno).second) {
            return false;
        }
        for (size_t j = 0; j < num_vals; ++j) {
            if (raw_interference[i][j] != raw_interference[j][i]) return false;
        }
    }

    std::unordered_set<unsigned> tied_results;
    for (const auto& tie : ties) {
        if (tie.result_val >= num_vals || tie.operand_val >= num_vals) return false;
        if (tie.insn_pos < 0 || tie.insn_pos >= (int)num_positions) return false;
        if (tie.operand_death_pos != tie.insn_pos) return false;
        if (!tied_results.insert(tie.result_val).second) {
            return false;
        }
    }
    return true;
}

// Exact Bounded DSATUR 8-Coloring with Color-Mask Intersections
m2_solve_status solve_m2_exact_coloring(size_t num_positions,
                                        const std::vector<rtl_interval>& raw_intervals,
                                        const std::vector<std::vector<bool>>& raw_interference,
                                        const std::vector<certified_destructive_tie>& ties,
                                        std::vector<unsigned>& final_reg_mapping,
                                        unsigned max_search_nodes = 50000) {
    if (!validate_extracted_model(num_positions, raw_intervals, raw_interference, ties)) {
        return m2_solve_status::INVALID_MODEL;
    }

    size_t num_vals = raw_intervals.size();
    union_find uf(num_vals);

    // 1. Contract certified mandatory equality ties
    for (const auto& tie : ties) {
        if (tie.kind == tie_kind::MANDATORY_2ADDR || tie.kind == tie_kind::LV_PREDICATION) {
            uf.unite(tie.result_val, tie.operand_val);
        }
    }

    // 2. Build contracted node list with strict color mask intersection
    std::unordered_map<unsigned, unsigned> root_to_node_idx;
    std::vector<m2_color_node> contracted_nodes;

    for (size_t i = 0; i < num_vals; ++i) {
        unsigned root = uf.find(i);
        if (root_to_node_idx.find(root) == root_to_node_idx.end()) {
            root_to_node_idx[root] = contracted_nodes.size();
            m2_color_node node;
            node.stable_id = contracted_nodes.size();
            node.allowed_color_mask = 0xFF;
            node.fixed_color = -1;
            contracted_nodes.push_back(node);
        }
        
        m2_color_node& c_node = contracted_nodes[root_to_node_idx[root]];
        c_node.member_values.push_back(i);
        c_node.allowed_color_mask &= raw_intervals[i].allowed_color_mask;

        // Reconcile normalized fixed precolors (0..7)
        if (raw_intervals[i].fixed_color != -1) {
            if (c_node.fixed_color != -1 && c_node.fixed_color != raw_intervals[i].fixed_color) {
                return m2_solve_status::INVALID_MODEL; // Conflicting precolors!
            }
            c_node.fixed_color = raw_intervals[i].fixed_color;
        }
    }

    for (const auto& c_node : contracted_nodes) {
        if (c_node.allowed_color_mask == 0) {
            return m2_solve_status::INVALID_MODEL;
        }
        if (c_node.fixed_color != -1 && !(c_node.allowed_color_mask & (1 << c_node.fixed_color))) {
            return m2_solve_status::INVALID_MODEL;
        }
    }

    size_t num_nodes = contracted_nodes.size();
    std::vector<std::vector<bool>> contracted_interference(num_nodes, std::vector<bool>(num_nodes, false));

    for (size_t u = 0; u < num_vals; ++u) {
        for (size_t v = 0; v < num_vals; ++v) {
            if (raw_interference[u][v]) {
                unsigned node_u = root_to_node_idx[uf.find(u)];
                unsigned node_v = root_to_node_idx[uf.find(v)];
                if (node_u == node_v) {
                    return m2_solve_status::INVALID_MODEL; // Self-interference in class!
                }
                contracted_interference[node_u][node_v] = true;
            }
        }
    }

    // 3. Exact Bounded DSATUR Backtracking Search
    std::vector<unsigned> node_colors(num_nodes, 8);
    unsigned search_steps = 0;
    bool search_capped = false;

    auto get_saturation_degree = [&](size_t u) {
        std::unordered_set<unsigned> neighbor_colors;
        for (size_t v = 0; v < num_nodes; ++v) {
            if (contracted_interference[u][v] && node_colors[v] < 8) {
                neighbor_colors.insert(node_colors[v]);
            }
        }
        return neighbor_colors.size();
    };

    std::function<bool(size_t)> backtrack = [&](size_t colored_count) -> bool {
        if (colored_count == num_nodes) return true;
        if (++search_steps > max_search_nodes) {
            search_capped = true;
            return false;
        }

        size_t best_u = num_nodes;
        size_t max_sat = 0;
        for (size_t u = 0; u < num_nodes; ++u) {
            if (node_colors[u] >= 8) {
                size_t sat = get_saturation_degree(u);
                if (best_u == num_nodes || sat > max_sat) {
                    best_u = u;
                    max_sat = sat;
                }
            }
        }

        for (unsigned color = 0; color < 8; ++color) {
            if (!(contracted_nodes[best_u].allowed_color_mask & (1 << color))) continue;
            if (contracted_nodes[best_u].fixed_color != -1 && 
                contracted_nodes[best_u].fixed_color != (int)color) continue;

            bool color_ok = true;
            for (size_t v = 0; v < num_nodes; ++v) {
                if (contracted_interference[best_u][v] && node_colors[v] == color) {
                    color_ok = false;
                    break;
                }
            }
            if (color_ok) {
                node_colors[best_u] = color;
                if (backtrack(colored_count + 1)) return true;
                node_colors[best_u] = 8;
            }
        }
        return false;
    };

    if (!backtrack(0)) {
        return search_capped ? m2_solve_status::SEARCH_LIMIT : m2_solve_status::UNSAT;
    }

    // 4. Expand contracted colors back to all raw values
    final_reg_mapping.assign(num_vals, 8);
    for (size_t node_idx = 0; node_idx < num_nodes; ++node_idx) {
        for (unsigned val_idx : contracted_nodes[node_idx].member_values) {
            final_reg_mapping[val_idx] = node_colors[node_idx];
        }
    }
    return m2_solve_status::SOLVED;
}

// Global Function-Level Pseudo Closure Check (Read-Only Precondition)
bool verify_global_pseudo_closure(function *fn,
                                  const std::vector<rtx_insn*>& island,
                                  const std::unordered_set<unsigned>& selected_pseudos,
                                  std::vector<rtx_insn*>& out_debug_resets) {
    std::unordered_set<rtx_insn*> island_insns(island.begin(), island.end());
    out_debug_resets.clear();

    for (unsigned regno : selected_pseudos) {
        for (df_ref ref = DF_REG_DEF_CHAIN(regno); ref; ref = DF_REF_NEXT_REG(ref)) {
            rtx_insn *def_insn = DF_REF_INSN(ref);
            if (!def_insn || island_insns.find(def_insn) == island_insns.end()) {
                return false; // Semantic definition outside island!
            }
        }
        for (df_ref ref = DF_REG_USE_CHAIN(regno); ref; ref = DF_REF_NEXT_REG(ref)) {
            rtx_insn *use_insn = DF_REF_INSN(ref);
            if (!use_insn) continue;
            if (island_insns.find(use_insn) == island_insns.end()) {
                if (DEBUG_INSN_P(use_insn)) {
                    out_debug_resets.push_back(use_insn); // Collect for grouped change staging
                } else {
                    return false; // Semantic use outside island!
                }
            }
        }
    }
    return true;
}

// Occurrence-Level Independent Precommit Staged Pattern Validator
bool verify_staged_island_patterns(const std::vector<rtx_insn*>& sfpu_island,
                                  const std::vector<staged_occurrence_record>& ledger,
                                  const std::vector<rtx_insn*>& debug_resets,
                                  const std::vector<rtl_interval>& raw_intervals,
                                  const std::vector<std::vector<bool>>& raw_interference,
                                  const std::vector<certified_destructive_tie>& ties) {
    // 1. Verify every instruction in island remains recognizable
    for (rtx_insn *insn : sfpu_island) {
        if (recog(PATTERN(insn), insn, NULL) < 0) return false;
    }

    // 2. Ledger verification: prove every staged location contains expected hard register
    for (const auto& rec : ledger) {
        if (!*rec.loc || !REG_P(*rec.loc) || !HARD_REGISTER_P(*rec.loc)) return false;
        if (REGNO(*rec.loc) != rec.expected_hard_reg || GET_MODE(*rec.loc) != rec.mode) {
            return false;
        }
    }

    // 3. Prove zero selected pseudos remain in staged island patterns
    std::unordered_set<unsigned> selected_pseudos;
    for (const auto& iv : raw_intervals) selected_pseudos.insert(iv.pseudo_regno);

    for (rtx_insn *insn : sfpu_island) {
        subrtx_ptr_iterator::array_type array;
        FOR_EACH_SUBRTX_PTR(iter, array, &PATTERN(insn)) {
            rtx *loc = *iter;
            if (*loc && REG_P(*loc) && !HARD_REGISTER_P(*loc)) {
                if (selected_pseudos.find(REGNO(*loc)) != selected_pseudos.end()) {
                    return false; // Staged replacement missed a pseudo occurrence!
                }
            }
        }
    }

    // 4. Verify all collected debug locations are now reset
    for (rtx_insn *dbg : debug_resets) {
        if (INSN_VAR_LOCATION_LOC(dbg) != gen_rtx_UNKNOWN_VAR_LOC()) return false;
    }

    return true;
}

// Atomic RTL Hard Register Substitution using GCC 15 Grouped-Change APIs
unsigned int execute_rvtt_pre_ira_alloc(function *fn) {
    m2_pass_telemetry telemetry;
    basic_block bb;

    FOR_EACH_BB_FN(bb, fn) {
        rtx_insn *insn = BB_HEAD(bb);
        
        // Outer cursor loop: discover and process ALL contiguous islands in BB
        while (insn && insn != NEXT_INSN(BB_END(bb))) {
            std::vector<rtx_insn*> sfpu_island;
            
            // 1. Scan to find next contiguous SFPU island
            while (insn && insn != NEXT_INSN(BB_END(bb))) {
                if (NONDEBUG_INSN_P(insn)) {
                    if (rvtt_sfpu_insn_p(insn)) {
                        sfpu_island.push_back(insn);
                    } else if (!sfpu_island.empty()) {
                        break; // Island boundary reached
                    }
                }
                insn = NEXT_INSN(insn);
            }
            
            if (sfpu_island.empty()) continue;

            std::vector<rtl_interval> raw_intervals;
            std::vector<std::vector<bool>> raw_interference;
            std::vector<destructive_tie_candidate> tie_candidates;
            extract_rtl_constraint_model(sfpu_island, raw_intervals, raw_interference, tie_candidates);

            // 2. Authoritative Tie Certification Loop
            std::vector<certified_destructive_tie> certified_ties;
            bool cert_failed = false;
            for (const auto& cand : tie_candidates) {
                certified_destructive_tie cert_tie;
                rtx_insn *cand_insn = sfpu_island[cand.insn_pos];
                if (certify_destructive_tie(cand_insn, cand, cert_tie)) {
                    certified_ties.push_back(cert_tie);
                } else if (cand.requested_kind == tie_kind::MANDATORY_2ADDR) {
                    cert_failed = true;
                    break;
                }
            }

            if (cert_failed) {
                telemetry.tie_cert_reject_count++;
                continue;
            }

            std::unordered_set<unsigned> selected_pseudos;
            for (const auto& iv : raw_intervals) {
                selected_pseudos.insert(iv.pseudo_regno);
            }

            // Pre-staging Read-Only Global Function-Level Closure Verification
            std::vector<rtx_insn*> debug_resets;
            if (!verify_global_pseudo_closure(fn, sfpu_island, selected_pseudos, debug_resets)) {
                telemetry.closure_reject_count++;
                continue;
            }

            std::vector<unsigned> final_reg_mapping;
            m2_solve_status status = solve_m2_exact_coloring(sfpu_island.size(), raw_intervals, raw_interference, certified_ties, final_reg_mapping);
            if (status != m2_solve_status::SOLVED) {
                if (status == m2_solve_status::INVALID_MODEL) telemetry.invalid_model_count++;
                else if (status == m2_solve_status::UNSAT) telemetry.unsat_count++;
                else if (status == m2_solve_status::SEARCH_LIMIT) telemetry.search_limit_count++;
                continue;
            }

            std::unordered_map<unsigned, unsigned> regno_to_lreg;
            for (size_t i = 0; i < raw_intervals.size(); ++i) {
                regno_to_lreg[raw_intervals[i].pseudo_regno] = final_reg_mapping[i];
            }

            // Standalone GCC 15 Grouped Change Transaction (Assert 0 changes pending)
            gcc_assert(num_validated_changes() == 0);
            bool stage_ok = true;
            std::vector<staged_occurrence_record> ledger;

            // 3. Stage semantic hard-register substitutions & build occurrence ledger
            for (rtx_insn *cur_insn : sfpu_island) {
                subrtx_ptr_iterator::array_type array;
                FOR_EACH_SUBRTX_PTR(iter, array, &PATTERN(cur_insn)) {
                    rtx *loc = *iter;
                    if (*loc && REG_P(*loc) && !HARD_REGISTER_P(*loc)) {
                        unsigned p_regno = REGNO(*loc);
                        auto it = regno_to_lreg.find(p_regno);
                        if (it != regno_to_lreg.end()) {
                            unsigned hreg = SFPU_REG_FIRST + it->second;
                            rtx hard_reg = gen_raw_REG(GET_MODE(*loc), hreg);
                            ledger.push_back({cur_insn, loc, p_regno, hreg, GET_MODE(*loc)});
                            if (!validate_change(cur_insn, loc, hard_reg, /*unique=*/true)) {
                                stage_ok = false;
                                break;
                            }
                        }
                    }
                }
                if (!stage_ok) break;
            }

            // 4. Stage external debug location resets within the same transaction
            if (stage_ok) {
                for (rtx_insn *debug_insn : debug_resets) {
                    if (!validate_change(debug_insn, &INSN_VAR_LOCATION_LOC(debug_insn), gen_rtx_UNKNOWN_VAR_LOC(), /*unique=*/true)) {
                        stage_ok = false;
                        break;
                    }
                }
            }

            // Precommit Verification on Fully Staged Patterns
            if (stage_ok && verify_changes(0)) {
                if (verify_staged_island_patterns(sfpu_island, ledger, debug_resets, raw_intervals, raw_interference, certified_ties)) {
                    confirm_change_group();
                    df_insn_rescan_all();
                    telemetry.committed_count++;
                } else {
                    cancel_changes(0);
                    telemetry.validator_reject_count++;
                }
            } else {
                cancel_changes(0);
                telemetry.recog_reject_count++;
            }
        }
    }

    if (dump_file) {
        fprintf(dump_file, "\n--- RVTT Pre-IRA Allocation Telemetry ---\n");
        fprintf(dump_file, "Committed: %u, Recog Rejects: %u, Validator Rejects: %u, Closure Rejects: %u, UNSAT: %u, Search Limit: %u, Invalid Model: %u, Extract Rejects: %u, Tie Cert Rejects: %u\n",
                telemetry.committed_count, telemetry.recog_reject_count, telemetry.validator_reject_count, telemetry.closure_reject_count,
                telemetry.unsat_count, telemetry.search_limit_count, telemetry.invalid_model_count, telemetry.extraction_reject_count, telemetry.tie_cert_reject_count);
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
- **Interleaved Latency Schedule:** 8 MADs + 1 trailing NOP = **9 issue slots** (**40% reduction in static issue slots**).

```
Serial Issue Stream (15 Slots):
Slot:  0      1      2      3      4      5      6      7      8      9     10     11     12     13     14
Op:   P0 ──► NOP ──► P1 ──► NOP ──► P2 ──► NOP ──► P3 ──► Q0 ──► NOP ──► Q1 ──► NOP ──► Q2 ──► NOP ──► Q3

Interleaved Issue Stream (9 Slots):
Slot:  0      1      2      3      4      5      6      7      8
Op:   P0 ──► Q0 ──► P1 ──► Q1 ──► P2 ──► Q2 ──► P3 ──► Q3 ──► NOP
```

---

## 6. Tensix Coprocessor Lowering (Replay & `SFPLOADMACRO`)

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

### 6.1 Replay Selection as Conflict-Constrained Span Placement

Replay buffer optimization is a **conflict-constrained placement problem** over the 32-entry circular buffer:
- **Candidate Conflict Graph:** $G_C = (V_C, E_C)$ models overlapping/incompatible instruction occurrences.
- **Span Placement:** Selected candidates must fit contiguous available spans $[S_{\text{start}}, S_{\text{end}}] \subseteq [0, 31]$ after explicit user reservations.
- **Objective Function:** Maximize net instruction words saved minus capture/playback overhead:
  $$\max \sum_{i \in \text{Selected}} \left( (K_i - 1) \cdot L_i - \text{Overhead}_i \right)$$
- **Mockup Evidence:** On an 8-row unrolled loop, automatic replay compression reduces static Tensix instructions from **88 down to 19 on Wormhole (-78.4%)** and **56 down to 15 on Blackhole (-73.2%)** (frontend stream reduction).

### 6.2 `SFPLOADMACRO` Target-Internal Event Model

Rather than premature public macros, `SFPLOADMACRO` is governed by a compiler-internal `sfpu_macro_region` descriptor:
- Models concurrent execution across 4 sub-units: **Load, Simple ALU, MAD, Store**.
- Tracks 3-bit delay counters, sub-unit queue latencies, and transient $L_{16}$ lifetime.
- Proves zero sub-unit collisions and ensures safe teardown/drain before exiting the region.
- **Target Kernels:** Typecast, integer multiply (`mul_int`), signbit, and conditional `where` present **1.33x to 4.0x steady-state issue rate opportunities**.

---

## 7. TT-LLK Kernel Corpus Analysis & Performance Potential

| Kernel | Architecture Challenge | Existing Manual Workaround | Demonstrated vs. Candidate Opportunity |
| :--- | :--- | :--- | :--- |
| **Welford (LayerNorm)** | Raw LLK produces L0–L3; generated SFPI consumes them across the recurrence. | Explicit raw-LREG ownership metadata plus late full-literal coalescing; no global LREG reservation. | **Demonstrated on Blackhole silicon:** 323/323/323 `WELFORD_BODY` device cycles versus replay LLK 326/326/326; N=1/2/32 correctness passes all five selectors. |
| **Dual-Horner Rational** | 7 exposed NOP stalls in serial $P(x)/Q(x)$ evaluation. | Manual instruction interleaving in TTI. | **Candidate Opportunity:** 40% static issue-slot reduction; silicon verification required. |
| **Piecewise Generic / LUT** | Interleaved MADs, pinned coefficients, D-RWC updates. | 3 distinct hand-written polynomial replay bodies. | **Candidate Opportunity:** Compiler-managed coefficient pinning + exact replay packing. |
| **Log (`ckernel_sfpu_log.h`)** | Peak pressure 9 during polynomial + exponent correction. | Explicit reload from $Dst$ at line 62. | **Candidate Opportunity:** Pressure scheduling keeps inputs resident; eliminates $Dst$ cuts once demonstrated on compiler diffs. |
| **GELU / Erfinv** | High register pressure across nested inlined tanh/log/sqrt. | Intermediate state dumped to $Dst$. | **Candidate Opportunity:** Continuous 8-LREG allocation eliminates $Dst$ round-trip overhead. |
| **Addcmul (`ckernel_sfpu_addcmul.h`)** | Inter-row RAW dependencies across 2 rows. | Manual `MUL_a, MUL_b, MAD_a, MAD_b` ordering. | **Candidate Opportunity:** Latency scheduler automatically pipelines adjacent rows. |
| **Integer Remainder / Div** | Divisor chunk pressure. | Recomputes divisor expressions. | **Candidate Opportunity:** Target-directed rematerialization. |
| **Typecast / MulInt / Where** | Serial load-compute-store memory bound. | Plain loop fallback. | **Candidate Opportunity:** `SFPLOADMACRO` pipeline represents 1.33x–4.0x issue rate potential. |

---

## 8. SOTA Vectorization: Decoupled MLIR / Triton Architecture

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

The multi-quarter MLIR roadmap separates mathematical semantics at the high level, vector bufferization at the mid level, and physical destructive ties only during machine-level lowering.

> **Status (deferred).** Per the current strategic decision the SOTA path is a **GCC-extension**, not
> an MLIR rewrite — see **§18**. This MLIR/Triton architecture is retained as an *optional later
> layer*, to be reconsidered only if GCC hits the tile/dataflow ceiling called out at §18 (Track C /
> Track E checkpoint). It is not on the near-term critical path.

---

## 9. Reconciled Phased Execution Plan (P0 – P5) & Validation Gates

```
┌──────────────────────────────────────────────────────────────────────────────────────────────────┐
│                                 RECONCILED EXECUTION ROADMAP                                     │
├───────┬──────────────────────────────────┬───────────┬───────────────────────────────────────────┤
│ Phase │ Milestone Name                   │ Timeline  │ Deliverable & Hard Gate                   │
├───────┼──────────────────────────────────┼───────────┼───────────────────────────────────────────┤
│ **P0**│ **Default-On & Exact Sched**     │ BLOCKED   │ List + exact MILP + scorer/DFA shipped; │
│       │                                  │           │ whole-LLK differential + silicon open.   │
├───────┼──────────────────────────────────┼───────────┼───────────────────────────────────────────┤
│ **P1**│ **Raw-LREG Ownership & Liveness**│ Partial   │ Annotated sentinel enforcement shipped;  │
│       │                                  │           │ post-IRA verifier remains open.           │
├───────┼──────────────────────────────────┼───────────┼───────────────────────────────────────────┤
│ **P2**│ **Exact Closed-Island M2 Alloc** │Conditional│ M2 remains a dump-only audit stub; build │
│       │                                  │           │ if corpus exposes recurring IRA failures.│
├───────┼──────────────────────────────────┼───────────┼───────────────────────────────────────────┤
│ **P3**│ **Silicon Flow Scorecard**       │ Partial   │ **GO-BH-ONLY**: 3 generated-vs-handwritten│
│       │                                  │           │ wins, 1 tie, 1 parity (Addcmul; §18.8.0.2)│
│       │                                  │           │ scheduler A/B + WH remain open.           │
├───────┼──────────────────────────────────┼───────────┼───────────────────────────────────────────┤
│ **P4**│ **Latency Scheduling**           │ Partial   │ Generic proven 2-row Dst fusion/interleave│
│       │                                  │ landed    │ closes Addcmul +21.9%→+0.02% parity; broad│
│       │                                  │           │ modulo/cross-BB scheduling remains open.  │
├───────┼──────────────────────────────────┼───────────┼───────────────────────────────────────────┤
│ **P5**│ **Coprocessor, LICM & Macros**   │ Mixed     │ Counted replay + pressure-safe invariant │
│       │                                  │ Sprint 2-3│ constant hoist shipped opt-in; generic    │
│       │                                  │           │ macro planner LANDED default-off (§18.8.0.4).│
└───────┴──────────────────────────────────┴───────────┴───────────────────────────────────────────┘
```

### 9.1 Engineering Rebuttal to “Delivered” Status

The previous table collapsed foundations, active experiments, and completed production gates into
the same status vocabulary. That was not an editorial nit: it produced conclusions contradicted by
the compiler and by later sections of this document.

**P0 is not delivered.** The pressure scheduler remains `Init(0)`, and the Evidence-Based Rescue
Contract's whole-corpus differential and changed-binary scheduler silicon A/B are absent.
`run-corpus-score.sh` is useful F1.0 plumbing, but
one Welford manifest entry comparing handwritten replay against generated vFloat is neither a
whole-LLK corpus nor an identical-source flag differential. Renaming that scorer a “Corpus
Differential Driver” does not satisfy §2.1 item 4 or §10.2.

**P1 is delivered only for its annotated boundary.** `pass_rvtt_lreg_livein` already mutates RTL to
enforce sentinel lifetimes before IRA; it is not the dump-only `pass_rvtt_lp_alloc` audit stub.
That is real progress. The remaining hardening item is a discriminating post-IRA verifier and
continued CFG/loop/partial-mask coverage. Arbitrary unannotated opaque asm remains outside the
contract by design.

**P3 is a compiler-flow scorecard, not scheduler certification.** The three Blackhole wins and one
tie are valuable generated-versus-handwritten kernel results. They exercise different compiler
features—literal folding, typed replay formation/hoisting, and ordinary code generation—and should
remain in the scorecard. They do not replace an identical-source pressure-scheduler off/on test.
Welford's flag-off/flag-on body bypassed the pressure rewrite; Reduce-SDPA's changed-binary win
validates replay hoisting; neither proves default-on pressure scheduling.

**P4 is now landed for one conservative class, not complete.** The legacy `pass_rvtt_schedule`
still only inserts correctness NOPs, and `pass_rvtt_lp_schedule` remains a pressure-rescue pass for
`old_peak > 8`. New P4 phase 2/3 instead recognizes same-BB adjacent Dst iterations before IRA,
proves typed-address range and mod-4 cross-row non-aliasing, fuses the RWC step, and interleaves two
independent dynamic chains before replay capture. Its compiler tests, byte-identical fallback,
CRAQ functional gate, and paired Blackhole run close Addcmul from `+21.9%` to `+0.02%` parity. It is
not a general post-RA list/modulo scheduler and has not produced a silicon win.

**P5 must be tracked per mechanism.** Fixed-encoding replay loop hoisting is real, conservative,
opt-in, and has a changed-binary Reduce-SDPA silicon win. Counted-loop replay (`6422dbd9e3`) and
operation-independent invariant SFPU constant hoisting (`2bfa165348`) are now also shipped opt-in.
The latter's conservative eight-LREG pressure preflight covers entry live-through values,
loop-defined live-outs, and function-global opaque ownership; the prior nine-constant reload ICE
now refuses before mutation. Their first semantic Sigmoid silicon result remains open.
*[Superseded 2026-08-17 — the struck text below was true of the `fd8ed6f`-era tree:]*
~~`pass_rvtt_loadmacro` remains a default-off discovery pass with `emit=no` in the checked-in
compiler, but the simulator prerequisite is no longer wholly absent: CRAQ `fd8ed6f` provides the
audited transactional evaluator for its admitted WH/BH shapes. Compiler configuration ownership,
fallback identity, unsupported shapes, and silicon validation remain open.~~ Current state:
`rtl-rvtt-loadmacro.cc` was deleted entirely at sfpi-gcc `5f31e00f0` (oracle-minted first at
`32e20f9fd`) and its `-mtt-tensix-{analyze,emit}-loadmacro` flags hard-error on use; macro
emission is now the generic 7-layer planner (`-mtt-tensix-macro-planner`, default-off) at gcc
`bb56f1d77`, executed generically by craq-sim `f80a8d6` and silicon-proven on Min/Max
(§18.8.0.4). A single “Active” label obscures these materially different maturity levels.

**Execution recommendation:** finish the smallest discriminating evidence loop while expanding the
exact engine deliberately: (1) preserve bounded MILP execution on request and run it as a corpus
optimality oracle; (2) add latency/reuse/replay-aware objectives after exact pressure feasibility;
(3) ship the real whole-corpus scheduler differential; (4) archive one `old_peak > 8`,
changed-assembly, identical-source silicon off/on result; (5) classify every new spill and build M2
only if failures recur; then (6) decide the default production policy from measured quality and
compile-time distributions. In parallel, P4 should expose two-row Addcmul groups to both the list
and exact schedulers, but it must not borrow the pressure scheduler's validation status or describe
a measured deficit as a completed win.

---

## 10. Testing, Build & Verification Workflow

### 10.1 Compiler Build & Flags

```bash
# Validated checked-in build lane:
SFPI_WITH_LP_SOLVE=yes scripts/build.sh --tt-built --checking --small
SFPI_WITH_LP_SOLVE=yes scripts/build.sh --test-tt

# Compiler invocation:
riscv-tt-elf-g++ -mcpu=tt-wh-tensix -O2 \
  -mtt-tensix-optimize-pressure-schedule \
  -mtt-tensix-pressure-schedule-use-milp \
  -fdump-tree-rvtt_lp_schedule \
  -fdump-rtl-rvtt_lp_alloc \
  -S kernel.C -o kernel.S
```

### 10.2 Whole-Corpus A/B Differential Testing & Telemetry (P0 Implementation Deliverable)

The whole-corpus differential validation driver (`scripts/run-corpus-differential.sh`) is a required P0 deliverable designed to execute against the TT-LLK kernel corpus:

```bash
# Required P0 Deliverable: Whole-Corpus Assembly Differential & Simulator Suite
# Compiles corpus under baseline (-mno-tt-tensix-optimize-pressure-schedule) and 
# candidate default (-mtt-tensix-optimize-pressure-schedule), archives diffs, and
# executes every changed binary through simulator verification.
./scripts/run-corpus-differential.sh --baseline=build-off --candidate=build-default --output=diffs/

# Focused validation harness:
./scripts/validate-sfpu-pressure-scheduler.sh build build/validation-output
```

---

## 11. Conclusion & Operational Contract

Adopting the **guarded default-on feasibility policy (P0)** makes validated SFPU pressure rescue automatic for the narrow WH/BH regions supported today while retaining deterministic fallback and an operational rollback. 

Completing **M2 Physical Allocation (P1/P2)** with an independently certified final-RTL DSATUR engine extends that rescue to logically feasible schedules that generic IRA still misses. 

Latency scheduling (P4), conflict-constrained replay (P5), and `SFPLOADMACRO` event modeling (P5) provide the disciplined engineering path to maximize hardware throughput and realize world-class vector compilation on Tenstorrent Tensix silicon.

---

## 12. Critical Reanalysis: Wiring Improved, New Edge-Case Failures Exposed

The latest revision resolves several prior objections. Extraction now emits raw tie candidates, a visible loop invokes the certifier, semantic substitutions have an occurrence ledger, debug resets are staged transactionally, the DSATUR lambda is valid, and the silicon row no longer claims completed hardware validation. These are substantive corrections.

The target pseudocode still contains concrete safety bugs. They should be fixed before another architectural rewrite of the document.

### 12.1 Candidate Instruction Position Is Dereferenced Before Validation

The certification loop executes:

```cpp
rtx_insn *cand_insn = sfpu_island[cand.insn_pos];
```

before any bounds check on `cand.insn_pos`. A negative `int` converts to a very large `size_t`; an oversized positive value also indexes out of bounds. Model validation happens later, after the unsafe access. A malformed extractor result therefore causes undefined behavior rather than an `INVALID_MODEL` fallback.

Validate every candidate field before dereferencing anything: instruction position against `sfpu_island.size()`, value indices against `raw_intervals.size()`, operand index against the recognized instruction, death position against the dense domain, and alternative/kind against target semantics. Prefer an extraction result object with an explicit success status so `extraction_reject_count` can actually be incremented; the displayed `void` extractor provides no rejection path.

### 12.2 Failed Mandatory `_lv` Certification Is Silently Dropped

The solver contracts both `MANDATORY_2ADDR` and `LV_PREDICATION` ties as mandatory equality classes. The certification loop rejects the island only when a failed candidate has kind `MANDATORY_2ADDR`. A failed `LV_PREDICATION` candidate is silently omitted, after which coloring proceeds without the equality required by the solver's own semantics.

Every required tie kind must reject the entire island on certification failure. Only explicitly optional/permitted candidates may be dropped. Better, replace open-coded enum comparisons with a single `tie_requires_equality(kind)` predicate shared by certification, validation, contraction, and negative tests.

### 12.3 The Requested Alternative Still Is Not Proven Selectable

Preprocessing constraints and checking `matches == 0` proves that the constraint text for the indexed alternative refers to operand zero. It does not prove that this alternative is enabled and satisfied by the actual operands. The certifier still assumes operand zero is the destination, copies `requested_kind`, and does not validate early-clobber, register filters/classes, or `_lv` semantics.

Constrain against a mask containing the requested enabled alternative, verify the selected result, and inspect the corresponding `operand_alternative` records. Derive the output operand and tie kind from target instruction semantics. Preserve recognition globals with the appropriate saver/discipline so certification cannot corrupt surrounding extraction state.

### 12.4 Debug Reset Validation Uses RTX Pointer Identity Incorrectly

The validator checks:

```cpp
INSN_VAR_LOCATION_LOC(dbg) != gen_rtx_UNKNOWN_VAR_LOC()
```

`gen_rtx_UNKNOWN_VAR_LOC()` constructs a `CLOBBER(VOIDmode, const0_rtx)`. Calling it again need not return the same RTX pointer as the staged value, so pointer inequality is not a semantic unknown-location test and can reject every nonempty debug-reset transaction.

GCC already provides the correct predicate:

```cpp
VAR_LOC_UNKNOWN_P(INSN_VAR_LOCATION_LOC(dbg))
```

Use it, deduplicate `debug_resets`, and add `-g` success plus forced-recognition/validator-failure tests proving grouped commit and cancellation of every debug reset.

### 12.5 The "Independent" Validator No Longer Checks Interference or Ties

The staged validator receives `raw_interference` and `ties` but does not use them. The previous map-level interference/tie checks were removed when the ledger was added. The ledger proves that recorded locations contain the expected solver-selected hard registers and that selected pseudos disappeared; it does not independently prove that the coloring itself respects reconstructed liveness, masks, precolors, clobbers, and certified ties.

Keep both layers:

1. occurrence-level proof that staged RTL realizes the proposed mapping; and
2. independent reconstruction of final staged RTL liveness/constraints proving that realized mapping is legal.

The second layer must rebuild the interference and tie facts from staged RTL rather than trusting the extractor matrix. Otherwise a shared extractor bug can produce an incorrect graph, a self-consistent coloring, and a perfectly matching occurrence ledger.

### 12.6 Raw Ledger Pointers Need a Stability Contract

`staged_occurrence_record` stores raw `rtx *loc` pointers and dereferences them after `verify_changes(0)`. Group verification can add clobbers or replace an instruction pattern. The plan must prove those stored sub-RTX location pointers remain valid across every grouped canonicalization/pattern replacement, or use stable occurrence identities and re-find each occurrence in the staged pattern.

A robust ledger records instruction identity plus a stable operand/path descriptor and independently traverses the post-verification pattern. Merely rereading the same pointer used to stage a change risks validating the mutation mechanism against itself.

### 12.7 Raw-LLK Root Cause Is Reproduced and Fixed

The earlier pure-`sfpi::l_reg[]` fixture was correctly rejected as
non-discriminating: constrained builtin pseudos already carried allocator-visible
lifetimes. The actual Welford boundary was subsequently traced in CRAQ and on
Blackhole. Opaque raw `.ttinsn` loads/transposes produced L0-L3 without an RTL
definition, so IRA assigned a generated row-1 delta temporary to still-live L1;
the row-2 input was destroyed before its first generated use.

The integrated solution is pre-IRA prevention, not post-IRA recovery. Raw LLK
emits explicit no-code ownership metadata and `rtl-rvtt-lreg-livein` constructs
fixed-LREG intervals across the CFG. The Welford producer marker, discriminating
old/new allocation test, loop/edge/join regressions, CRAQ N=2 trace, and physical
N=1/N=2/N=32 results establish the cause and repair. A future post-IRA verifier
would be defense in depth, not the primary fix.

### 12.8 Executable Status and Remaining Allocator Work

The raw-LREG reproducer, preventative pass, CFG regressions, executable LLK
harness, pinned build, and silicon archive are now landed. The pressure scheduler
remains opt-in and the proposed M2 coloring allocator remains a dump/audit stub;
those facts are separate from the completed Welford interop fix.

Before any default-on scheduler or M2 claim, retain the earlier requirements for
independent staged-RTL validation, corpus/simulator differential coverage, and a
changed-binary high-pressure silicon A/B. Keep those performance claims separate
from the correctness and literal-coalescing evidence in §§13-15.

---

## 13. Integrated Welford Result (Authoritative)

This section supersedes the historical review material retained in Appendices A-C.

### 13.1 Final source tuple

- SFPI build tuple: superproject `3264e3c3a15e9e6bcb782dee8730e574f34bd119`,
  SFPI-GCC `d9c39fbd15c3e3a1ccefdca3aa994029e07efb57`.
- TT-Metal producer marker and executable LLK harness:
  `b6dd63708583a9762cce042a30d5e1ee10872a62`.
- Installed checked compiler `cc1plus` SHA-256:
  `284d266fd681a1d40f0e74c586b93eef1b862ca6b915823f4fd77e1c6782c54f`.
- Primary evidence: `validation/welford-bh-20260815/`.

SFPI-GCC now models opaque raw-LLK LREG ownership explicitly. The no-code
`sfprawlreg_access(release_mask, write_mask)` marker starts and ends fixed-LREG
ownership intervals, and the pre-IRA CFG pass keeps those registers unavailable
to generated temporaries. `0c9adf7d2` fixes live-out placement and documents the
first operand as a last-use/ownership-release mask. `3b5d6a43d` adds Blackhole
and Wormhole tests covering straight-line, fallthrough, conditional, loop,
empty-block, multi-predecessor, repeated-use, and mixed release/write cases.

The generic literal optimization is also integrated. `30b86b491` first removed
the reload move between low and upper `SFPLOADI` halves, but its early bypass was
too broad and caused 69 TT assembly-test failures by skipping established
constant folding. `d9c39fbd1` fixes the design: all immediate shortening and
constant-register/scalar combines run first; only a remaining single-use,
otherwise-unsimplifiable low/upper pair is recombined for one-result RTL
expansion. The complete TT gate then passes with no golden-file churn.

### 13.2 Validation gates

- Solver-enabled checked compiler build: pass (`--with-lp-solve=yes`).
- Focused WH/BH scheduler validation, MILP required, three determinism runs:
  pass.
- TT target DejaGNU gate: pass (`Tests passed. Yay!`).
- Blackhole P100A physical correctness: 15/15 pass — five selectors at N=1,
  N=2, and N=32, checking captured L4 mean and L5 M2 against the host Welford
  reference.
- Final vFloat N=32 math ELF SHA-256:
  `81ccafc96c30256758f07e70c547b1ecf0722cb16bf329fe592872877420cfdc`.
- Final linked disassembly contains zero
  `SFPLOADI; SFPMOV; SFPLOADI` staging triples (seven before coalescing).

Wormhole produced all compile artifacts but no Wormhole device was available;
the silicon verdict is therefore **GO-BH-ONLY**, not cross-architecture GO.

### 13.3 Archived Blackhole device result

Each timing sample used a fresh pytest process and device session. Raw and post
CSV rows were copied immediately after each run and are archived separately.

| Implementation | WELFORD_BODY cycles | Minimum | Mean | Range |
| :--- | :--- | ---: | ---: | ---: |
| Handwritten replay | 326 / 326 / 326 | 326 | 326 | 0 |
| Compiler-generated vFloat direct | 323 / 323 / 323 | 323 | 323 | 0 |

The generated recurrence is 3 cycles (0.92%) faster than handwritten replay in
this bounded microbenchmark. That is a measured result, not a static instruction
count estimate. This is *not* a scheduler- or compiler-superiority claim: per
§14.2 it compares two different source bodies after two generic QoI fixes
(raw-LREG ownership modeling and late literal coalescing), and makes no
pressure-scheduler performance claim.

## 14. Silicon Measurement Reanalysis and Rebuttal (Authoritative)

The earlier rebuttal was right about the central category error: the Welford
numbers do not demonstrate a pressure-scheduler speedup. It was wrong or stale
about the exact measurement mechanism, the source relationship between the
selectors, and the integration state.

### 14.1 What was actually measured

The fixture records a math-TRISC `WELFORD_BODY` profiler-zone delta using device
`read_wall_clock()` timestamps. It is not pytest wall time and not a raw hardware
performance-counter event. `TRACE_N=0` removes diagnostic stores, while the zone
wraps only the recurrence; initialization, wait-for-destination, destination
completion, and host/runtime overhead are excluded. These are therefore valid,
repeatable device-body cycles, not end-to-end kernel throughput.

### 14.2 What the selector comparison does not prove

Historical `vFloat direct` and `vFloat rescue` are different source bodies in
the fixture. The harness did not record an identical-source compiler-flag
off/on pair with pass dumps and different ELF hashes. Their historical equal
timings therefore did **not** establish a 0.0% scheduler delta, prove that the
pass bypassed, or validate a pressure rescue. Likewise, comparing handwritten
direct with generated vFloat changes implementations and cannot credit the
pressure scheduler.

The current 323-versus-326 result is instead evidence for the generated vFloat
body after two generic correctness/QoI changes: raw-LREG ownership modeling and
late full-literal coalescing. It makes no scheduler-performance claim.

### 14.3 What is now reproducible

The previous criticism that the fixture and primary rows existed only in local
scratch space is resolved:

- TT-Metal `b6dd6370` commits the executable C++ and Python LLK harness.
- `validation/welford-bh-20260815/` commits correctness output, per-run raw/post
  profiler CSVs, compiler-gate artifacts, device/firmware metadata, final
  disassembly, ELF hashes, commands, and a complete SHA-256 manifest.
- SFPI pins the exact SFPI-GCC source used for the final rebuild.

The archive intentionally calls the metric a device-body profiler-zone result.
It does not relabel it as whole-kernel timing.

### 14.4 Corrected engineering conclusion

- Accept the raw-LREG fix as integrated and positively validated for the marked
  Welford boundary on Blackhole, with broad BH/WH compiler CFG regressions.
- Accept the literal-coalescing optimization: it preserves the mature immediate
  simplification pipeline, passes the complete TT gate, removes all seven
  Welford staging triples, and retains silicon correctness.
- Accept 323/323/323 versus 326/326/326 as the final pinned Blackhole
  `WELFORD_BODY` result.
- Do not infer pressure-scheduler benefit, replay causality, whole-kernel
  throughput, or Wormhole silicon behavior from this measurement.

## 15. Remaining Follow-Up

The Welford correctness fix, compiler optimization, branch integration, and
Blackhole microbenchmark archive are complete. They do not require another
rebuttal cycle.

The remaining work is separate in scope:

1. Run an identical-source, changed-binary `old-peak > 8` pressure-scheduler
   off/on case on silicon, retaining pass dumps, flags, ELF/disassembly hashes,
   and three paired device samples. Until then, scheduler silicon benefit is
   unproven.
2. Repeat the correctness and timing campaign on Wormhole hardware when one is
   available.
3. Add full TRISC/kernel or TT-Metal device-profiler timing if an end-to-end
   product-throughput claim is required; the archived body metric must not be
   substituted for it.
4. Continue annotating other opaque raw-LLK producer/consumer boundaries as
   they adopt the explicit ownership API. The pass is general, but unmarked raw
   assembly is intentionally not decoded or guessed by GCC.
5. The proposed M2 coloring allocator remains audit-only
   (`colorability=unchecked`) and is not exercised or required by this Welford
   result.

## 16. Silicon-Archive Rebuttal: Compiler Work Accepted; Historical Binary Binding Is Incomplete

The latest revision resolves the major objections from Appendix C. The SFPI
superproject now pins SFPI-GCC `d9c39fbd1`; `0c9adf7d2` keeps ownership live
through a non-jump `BB_END` and names operand zero as a release/last-use mask;
`3b5d6a43d` adds WH/BH loop and join coverage; the TT gate is green; and the
top-level `SHA256SUMS` verifies every file currently in
`validation/welford-bh-20260815/`. These are sufficient to accept the raw-LREG
fix as integrated and the reported Blackhole correctness campaign as strong
positive evidence for the marked Welford boundary.

The archive is nevertheless not a complete historical binary-provenance chain.
This is a narrower defect than the earlier “local-only” objection, but it is
real and directly relevant to the new 323-versus-326 performance conclusion.

### 16.1 The Timed ELFs Are Not Hashed or Archived

Each profiler run directory contains only `raw.csv`, `post.csv`, `run.log`, and
an `archive-manifest.txt`. Those manifests hash the two CSV files and repeat the
console result; they do not record:

- the timed `math.elf` SHA-256;
- a disassembly or text-section hash;
- the TT-Metal build-cache key;
- the compiler executable hash used for that build; or
- the complete compile command and environment.

`TEXT_SIZE(MATH_ISOLATE)` is useful metadata, but 2891 or 3019 bytes is not a
binary identity. The six profiler rows therefore prove that the harness
reported 326 and 323 cycles, while the archive does not cryptographically bind
those rows to the exact replay and vFloat executables that ran.

### 16.2 The Archived Disassembly Is for a Different Compile-Time Variant

The sole disassembly is
`vfloat-direct-n32.math.objdump`, sourced from the correctness build with
`TRACE_IMPL=2` and `TRACE_N=32`. The performance test explicitly builds with
`TRACE_IMPL=2` and `TRACE_N=0` so diagnostic captures disappear. Because
`TRACE_N` controls `if constexpr` code generation, those are different ELFs.

Likewise, the highlighted final ELF hash
`81ccafc96c30256758f07e70c547b1ecf0722cb16bf329fe592872877420cfdc`
is the N=32 correctness ELF, not the binary that produced the 323-cycle rows.
The N=32 disassembly is valid evidence for correctness-build code quality, but
it cannot serve as the disassembly or hash for the capture-free timing binary.

Archive the exact `TRACE_N=0` replay and vFloat `math.elf` files—or at minimum
their hashes plus complete disassemblies—from every fresh performance build.
Record those hashes in the corresponding run manifest before the build tree is
reused or removed.

### 16.3 The Compiler Hash Is Recorded but Not Independently Recomputable

The archive records the installed `cc1plus` hash
`284d266fd681a1d40f0e74c586b93eef1b862ca6b915823f4fd77e1c6782c54f`,
but `compiler-artifacts.tar.gz` does not contain that executable. Its
`test-tt.log` reports the generic package version `0-sfpi`, not commit
`d9c39fbd1`, and the profiler logs do not repeat the compiler hash.

Pinning the source submodule makes a future rebuild possible; it does not prove
that a historical ELF was produced by a particular installed binary. Preserve
the compiler executable or a build manifest generated by that executable and
make each ELF manifest reference its hash. This is especially important here
because an earlier installed compiler retained stale configure-version text.

### 16.4 Correctness ELF Hashes Need an Explicit Selector Map

`correctness/math-elf.sha256` lists 15 hashes against now-unavailable absolute
build paths. It does not map each entry to `(N, TRACE_IMPL, selector)` and does
not archive the ELFs. The correctness log identifies all 15 passing test cases,
but there is no durable join key between a test-case line and one of the 15 ELF
hashes.

Emit a machine-readable manifest such as:

```text
selector=vfloat_direct n=32 trace_impl=2 trace_n=32 \
compiler_sha256=... math_elf_sha256=... build_key=...
```

for every correctness and performance binary. The harness should write this
record as part of the run, not reconstruct it afterward from directory order.

### 16.5 The Published Reproduction Command Still Depends on Local-Only Paths

The committed TT-Metal harness at `b6dd6370` is real and executable, resolving
the former placeholder-harness problem. The archive README's exact command,
however, still depends on `/localdev/nkapre/...` for `PYTHONPATH`, the Python
environment, runner storage, and the SFPI install symlink. It describes timing
invocations in prose rather than preserving the complete commands used for all
six runs.

Replace those paths with a repository script accepting explicit SFPI install,
TT-Metal checkout, output directory, selector, and repetition arguments. The
script should verify both source commits and the compiler hash, create a unique
build/output directory, run the test, copy the ELF/disassembly/CSV/log, emit the
selector manifest, and finally generate `SHA256SUMS` from the repository root.

### 16.6 Corrected Acceptance Decision

- **Accept:** the raw-LREG Welford root cause, integrated preventative fix,
  release-mask contract, compiler CFG regressions, and 15/15 Blackhole
  correctness result.
- **Accept as a measured report:** the archived profiler rows are internally
  consistent at replay 326/326/326 and generated vFloat 323/323/323 device-body
  cycles.
- **Do not yet call the performance rows binary-reproducible:** neither timed
  ELF is archived or hashed, and the available N=32 vFloat disassembly belongs
  to a different compile-time variant.
- **Do not infer scheduler benefit:** the revised authoritative sections
  correctly leave the identical-source changed-binary scheduler A/B open.
- **Close the remaining archive gap mechanically:** rerun or recover the two
  `TRACE_N=0` ELFs, bind each run to compiler/ELF hashes, and commit a portable
  reproduction script. No new compiler architecture is required for this step.

The engineering fix has crossed the acceptance threshold. The remaining
rebuttal is against calling the performance archive self-contained before it
contains the exact binaries—or durable identities for the exact binaries—that
produced its headline numbers.

## 17. Reviewer Sign-Off — Resolution Confirmed

*Independent check against the tree at superproject `3264e3c` / SFPI-GCC `d9c39fbd1`. This closes the
review thread carried in Appendix A.*

**The fix landed exactly as the prior analysis prescribed.** Appendix A §13.4 argued the Welford
clobber was the raw-LLK path (raw LREG has no IRA live interval) and that prevention *"belongs in
RTL register allocation, not the GIMPLE pressure model,"* with M2 not required. Confirmed in-tree:

- New pass `rtl-rvtt-lreg-livein.cc`, wired `INSERT_PASS_BEFORE (pass_ira, 1, pass_rvtt_lreg_livein)`
  — RTL, pre-IRA, as specified; the GIMPLE `fixed_color` / MILP-capacity patch was correctly **not**
  taken.
- `__builtin_rvtt_sfprawlreg_access(release_mask, write_mask)` creates fixed-LREG intervals with
  **precise release at last use** and CFG-join handling — the "precise liveness, not blanket
  reservation" requirement, not a whole-region reserve.
- Discriminating regressions (`raw-lreg-livein-cfg-bh/wh.C`) use the raw builtins + raw `sfpload`,
  not an all-`l_reg[]` fixture — the reproducer shape Appendix A said was missing.
- `rtl-rvtt-lp-alloc.cc` is still the 133-line `colorability=unchecked` stub: the win required
  **no M2**, as predicted.

**Independent confirmation of the authoritative claims.** Blackhole P100A 15/15 correctness, a green
TT DejaGNU gate, zero `SFPLOADI;SFPMOV;SFPLOADI` triples, and the committed
`validation/welford-bh-20260815/` manifest are the reproducible evidence earlier rounds asked for.
§14 is the right call: 323-vs-326 is a bounded device-body zone attributable to the two QoI fixes
(raw-LREG ownership + literal coalescing), **not** a pressure-scheduler speedup.

**Bottom line.** The correctness defect is resolved, correctly scoped, and reproducibly archived —
the strongest state this document has been in, and the point at which the long doc-only churn finally
converted into shipped, tested compiler code. Two items remain honestly open and are already booked
in §15: (1) the **scheduler's own** silicon benefit is still unproven (no identical-source flag
off/on pair), so P0 default-on (`Init(1)`) is not yet earned; (2) Wormhole is compile-only
(GO-BH-ONLY). One editorial nit: §13.3 leads with 323 < 326 — add an inline pointer to §14.2 so a
skimmer does not read it as a scheduler- or compiler-superiority headline.

---

## 18. Roadmap to a SOTA Vector Compiler (GCC-Extension Path)

*Added in response to "what is the roadmap to a SOTA vector compiler, and does this doc have enough
detail?" — the honest prior answer was no: §8 was a one-diagram MLIR vision and §9's P5 collapsed the
entire future into one cell. This section is the actual roadmap under the chosen strategy: **extend
the GCC/sfpi backend; defer MLIR (§8) to an optional later layer.***

> **Current coordinates (2026-08-17) — read before any track status below.** The authoritative state
> of all four repos is branch `nkapre/sfpi`:
>
> | Repo | Tip | Carries |
> | :--- | :--- | :--- |
> | **sfpi** (superproject) | `9a555cb89f` | gcc pin `bb56f1d77` (chain `ddf44ed64 → cd0af49be → bb56f1d77`, verified exact); wrappers incl. `dst_face_advance` (`ded6e4e9`); this document |
> | **sfpi-gcc** | `bb56f1d77` | three-branch unification (planner → sdpa → profit: `b3c031380` / `b5e07e458` / `6724f48c6`, mechanically faithful unions); all of WP8 (quarantined pass deleted `5f31e00f0` after oracle mint `32e20f9fd`); silicon-recalibrated profitability gate; `docs/MACRO_PLANNER.md` |
> | **tt-metal** | `69d61d66` | sweep_2x2 automation + weekly/nightly scripts; p150 chip-class baselines; Min/Max perf-harness fixes; Reduce-SDPA 834/840 p150 baseline pair; typed-LLK migration |
> | **craq-sim** | `f80a8d6` | recognizer deletion (machine-local HANDOFF §6a worklist) complete — generic descriptor-driven macro execution only |
>
> Bootstrap knowledge that used to live only in the machine-local HANDOFF now lives in-repo: this §18
> (track statuses; the reconciled state in §18.8.0.4 and the pull-analysis in §18.8.0.5),
> `docs/MACRO_PLANNER.md` in sfpi-gcc, and the sweep/harness scripts under tt-metal
> `tt_metal/tt-llk/tests/`. Reading discipline for every number below: it must carry its **chip class**
> (p100a vs p150 — never mixed arithmetically) and its **compiler era**, and same-source (OFF→ON
> causal) claims are never mixed with vs-hand (competitiveness) claims.

### 18.0 Implementation Onboarding (read this first)

Curated reading list for an agent about to *implement* the roadmap. Ordered; honest about what is
real vs. stub vs. superseded. Paths under `gcc/config/riscv/tt/` are in the **`sfpi-gcc` submodule**
(from the superproject root that is `gcc/gcc/config/riscv/tt/`).

**One-line brief.** The immediate deployment priority is **F2 plus the §2.3 scheduler evidence
gate**: ship the whole-corpus identical-source differential and obtain a changed-binary silicon
off/on result. In parallel, continue **F1.3–F1.4 (§18.7)** by calibrating the shipped DFA/cost-source
foundation to silicon instead of the currently mismatched simulator timing. **The M2 allocator and
the MLIR stack do not exist — do not depend on them without a corpus-demonstrated need.**

**0. Orient — only the live parts of this doc.** §2.2 (Ground Truth vs Target — read first), §13+§14
(authoritative Welford result + measurement hygiene), §18 (this roadmap; §18.7 F1 is the first task).
**Skip / do not build from:** §3.1 ("Non-Normative Target"), §4.2 (the "Target Implementation" C++ —
*not in the tree*), §8 (MLIR, deferred), Appendices A/B/C (superseded history).

**1. The shipped backend you are extending** (`gcc/config/riscv/tt/`):
- `gimple-rvtt-lp-schedule.cc` + `rvtt-lpsolve.cc` + `rvtt-schedule.h` — the real pressure scheduler,
  lp_solve model, and its data structures. The pass is opt-in and locally validated; the archived
  Welford silicon off/on pair bypassed the transformation and therefore is not evidence of a
  changed-binary scheduler win.
- `rtl-rvtt-lreg-livein.cc` — the recent raw-LREG-liveness fix; **read as the template** for adding a
  correct pre-IRA pass (sentinels, CFG dataflow, `INSERT_PASS_BEFORE(pass_ira, …)`).
- `rtl-rvtt-schedule.cc` — the `xtt_delay` STATIC/DYNAMIC NOP rule (+ the `XTT_DYNAMIC_BUG` scoreboard
  exception). **F1 swaps its cost source — read closely.**
- `rvtt.md` (`type=tensix`) and `rvtt-tune.md` (exists, but **no Tensix reservations** today) — F1
  keeps `type=tensix` as the pass-membership attribute and adds an orthogonal five-value issue-class
  attribute plus a real DFA / cost table. `ttrocc.md` is the separate QSR RISC-V RoCC interface and
  is not the Tensix FIFO command stream.
- `rvtt-insn.def`, `rvtt-constraints.md` — builtins and the `x<N>` single-register-class LREG pinning.
- `rvtt-passes.def` — pass ordering; `riscv.opt` — the two flags, both `Init(0)`.
- `rtl-rvtt-replay.cc` — **existing** replay codegen; Track D (§18.8) extends this, it is not a blank
  slate.
- `include/sfpi_classes.h` (superproject) — how `l_reg[Lx]` / `vFloat` lower to builtins.
- ⚠️ `rtl-rvtt-lp-alloc.cc` — a 133-line **dump-only stub** (`colorability=unchecked`). **Do NOT build
  on it or assume the M2 allocator exists.**

**2. The machine model / cost oracle** (your `craq-sim` checkout at the §18 current-coordinates tip; paths below are repo-relative):
- `src/tensix.cpp` — issue gaps + `busy_until` timers (`tensix_sfpu_issue_gap`, `math_busy_until`,
  ~L2251–2283, L478–494); SFPU/DST/`l_regs`/`load_macro_*` execution (Tracks B/D).
- `src/libttsim.cpp` — per-cycle clock loop, the 5 issue classes + priority (~L2265–2280), and the
  profile counters that emit device cycles (~L260–277).
- `src/sim.h` — `TensixState` (l_regs, dst, rwc). `scripts/perf/` — existing perf extraction to mirror
  in `run-corpus-score.sh`.

**3. Build, validate, reproduce:**
- `scripts/build.sh` (with `SFPI_WITH_LP_SOLVE=yes`) and `scripts/validate-sfpu-pressure-scheduler.sh`
  — the pinned build + validation lane (§10.1).
- `gcc/.../testsuite/g++.target/riscv/tt/tensix/raw-lreg-livein*.C` — the raw-LREG regressions; the
  pattern for a *discriminating* test.
- `validation/welford-bh-20260815/` — the archived silicon run. Note there is **no from-source
  reproduction path yet**, only this archive — building that path is F1's first milestone (§18.7 M-F1.0).

#### 18.0.1 F1 Kickoff Brief (copy-paste to the implementing agent)

> **Mission: complete Track F1 and F2 from this roadmap.**
>
> Read §18.0 (this section) first — your ordered reading list and honest map of real vs. stub. Then
> §2.3, §10.2, and §18.7 are your specs. Ignore §3.1, §4.2, §8, Appendices A/B/C.
>
> **What you're doing (and not doing).** You are *not* writing a new compiler. You are replacing the
> near-binary latency rule in the shipped backend with a per-architecture cost table calibrated to
> silicon microbenchmarks, using `craq-sim` as a functional/debugging reference until its timing
> error is repaired. Milestones (§18.7.3):
> - **F1.0 (shipped, narrow corpus)** — extend `run-corpus-score.sh` beyond its current Welford
>   manifest while preserving compiler hashes, ELF/objdump artifacts, CRAQ functional results,
>   physical correctness, and copied device profiles.
> - **F1.1** — new `rvtt-cost.md`: retain `type=tensix` and add an orthogonal five-value craq-sim
>   issue-class attribute; add a `define_automaton` + per-class `define_insn_reservation` with
>   latencies from craq-sim's issue gaps.
> - **F1.2** — make `rtl-rvtt-schedule.cc` table-driven, replacing the `xtt_delay` STATIC/DYNAMIC rule
>   (incl. the `XTT_DYNAMIC_BUG` path). Same insertion point / same NOP mechanism — swap the cost
>   *source*, not a new pass.
> - **F1.3** — extend the existing `rvtt-tune.md` with per-arch (WH/BH/QSR) tensix reservations;
>   scorer error vs craq-sim ≤ target on the corpus.
> - **F1.4/F2** — stand up the identical-source, changed-binary, paired off/on **silicon A/B** and
>   `run-corpus-differential.sh`. Existing handwritten-vs-generated scorer plumbing is reusable,
>   but no scheduler-specific whole-corpus A/B exists to "wire".
>
> **Hard constraints:**
> - Do **NOT** touch or depend on `rtl-rvtt-lp-alloc.cc` (dump-only stub, `colorability=unchecked`).
>   No MLIR. Leave both scheduler flags at `Init(0)`.
> - Preserve **byte-identical** output for ineligible / already-≤8-peak regions
>   (`validate-sfpu-pressure-scheduler.sh`).
> - F1.2 must reproduce **identical NOP placement on the Welford body** before you trust the new cost
>   source.
>
> **Definition of done:** scorer (F1.0) reproduces 323/326 from source; the cost table (F1.1–1.3)
> drives NOP insertion with committed simulator and silicon error bounds across the LLK corpus; TT
> DejaGNU stays green; changed binaries are measured on silicon.
>
> **Ground truth / where to build:** compiler code is in the **`sfpi-gcc` submodule**
> (`gcc/config/riscv/tt/…`), not the superproject. Functional/timing reference: your `craq-sim` checkout (§18 current-coordinates block) — `src/tensix.cpp`
> (issue gaps/`busy_until`), `src/libttsim.cpp` (cycle loop, 5 issue classes, profile counters); your
> latency numbers must explain these and then fit silicon. Build: `SFPI_WITH_LP_SOLVE=yes scripts/build.sh …`. Test pattern:
> `testsuite/g++.target/riscv/tt/tensix/raw-lreg-livein*.C`.
>
> **Guardrails:** never `git add -A` (untracked `build/` + the gcc submodule live in the tree — stage
> files explicitly). Commit only the compiler files + the new script + tests. Score every codegen
> change against craq-sim before claiming an improvement — no static instruction-count claims.
>
> **Report back with:** the scorer table (baseline vs candidate cycles per kernel), the DejaGNU
> result, and confirmation Welford is byte-identical / still 323 on BH.

### 18.1 What "SOTA" means here (definition + baseline)

SOTA is a claim relative to a baseline, so fix one: **match or beat the hand-tuned TT-LLK
(handwritten replay) across the kernel corpus on silicon, and additionally compile kernels that are
infeasible to hand-write** (higher register pressure, cross-engine pipelines). "Faster than a diagram"
is not a metric; a committed corpus A/B on Blackhole/Wormhole is.

### 18.2 Honest scope today

Shipped is **one register file (8 LREGs) of one engine (SFPU) for one op class (add/mul/mad)**:
pressure scheduling (opt-in) + raw-LREG liveness through IRA + literal coalescing, with Welford
compiler-flow correctness validated on silicon (§13). The Welford off/on body did not exercise the
pressure rewrite, so scheduler-specific silicon validation remains open. That is the *easiest* engine. The hard 90% of a Tensix vector compiler —
the DST/RWC hazard model, the matrix + pack/unpack pipeline, cross-TRISC coordination, general
replay allocation/MOP, `SFPLOADMACRO` emission, and autovectorization — is not complete. Replay
formation and conservative loop capture hoisting are already real; do not describe Track D as
greenfield wholesale. *[Update 2026-08-17: this paragraph's shipped-scope snapshot is the
2026-08-15-era view and predates the landed unified sdpa stack and the generic `SFPLOADMACRO`
macro planner (both default-off at gcc `bb56f1d77` — §18.4 rows A/D, §18.8.0.4). `SFPLOADMACRO`
emission has therefore moved from the not-complete list to LANDED, and "one op class
(add/mul/mad)" understates the current coverage; the DST/RWC hazard model, cross-TRISC
coordination, MOP, and autovectorization remain the open hard 90%.]*

### 18.3 Foundation (cross-cutting — must precede the tracks)

- **F1. Cost model calibrated to silicon, with `craq-sim` functional coverage + silicon A/B.** A SOTA scheduler needs an accurate perf
  oracle. The current simulator is useful for functional coverage but fails the recorded
  Welford magnitude calibration; silicon is the performance authority until that error is bounded.
  **Gate:** every codegen change is scored
  against craq-sim and a silicon A/B, not by static instruction counts.
- **F2. Ship the corpus differential driver** (`run-corpus-differential.sh`, §10.2 — still absent).
  **Gate:** whole-LLK-corpus baseline-vs-candidate, asm diff + simulator + silicon, is the standing
  regression before any "SOTA" claim.

### 18.4 Tracks, deliverables, hard gates

| Track | Scope (all GCC-internal) | Status | Hard gate |
| :--- | :--- | :--- | :--- |
| **A. Finish the SFPU story** | Full SFPU ISA scheduling/alloc beyond add/mul/mad (LUT/transcendental, int, casts); predication/masking under CC divergence; software pipelining across loops; M2 exact allocator **only if** raw-LREG+IRA proves insufficient on the corpus (still a stub, §4). | Partly shipped — the unified sdpa stack (pressure-aware invariant hoist, replay-aware unroll, dst-autoincr, launch conversion) is LANDED default-off at gcc `bb56f1d77` and silicon-proven (SDPA −19.63% causal on p150); LUT lowering (SigmoidAppx) and Exp remain open (§18.8.0.4) | Match/beat handwritten **non-replay** LLK across the SFPU corpus on silicon. |
| **B. DST tile register + RWC** | Model the DST accumulator (fp16/fp32 layout) and RWC (read/write-clear) hazard tokens between matrix engine, SFPU, and pack. Eliminate the hand-written `Dst` round-trips §7 kernels use (log/GELU/erfinv dump state to `Dst`). Annotated raw-LREG ownership enforcement already ships; layout/RWC aliasing and its post-IRA verifier remain. | B0 enabler LANDED (typed `ttdstface`/`ttsetrwc` at gcc `bb56f1d77`; wrappers `ded6e4e9`; LLK migrated on tt-metal `nkapre/sfpi`); B1–B5 pass unbuilt; lane PAUSED (§18.9) | Kernels that spill to `Dst` by hand keep values resident; correctness + non-inferiority. |
| **C. Cross-engine scheduling** | Model matrix engine (FPU/matmul) + pack/unpack pipelines; coordinate the three TRISCs (unpack/math/pack) via semaphores/wait-gates; schedule across engine boundaries. **This is where GCC's tile/dataflow abstractions may hit a ceiling — MLIR reconsideration checkpoint (§8).** | Not started | An end-to-end unpack→matmul→SFPU→pack kernel scheduled by the compiler, measured **whole-kernel** on silicon. |
| **D. Replay / MOP / `SFPLOADMACRO` emission** | Existing post-RA replay formation ships, and conservative loop-capture hoisting is opt-in with a changed-binary Reduce-SDPA silicon win. General cross-BB replay allocation and MOP remain. | Planner LANDED — the generic 7-layer `SFPLOADMACRO` planner is at gcc `bb56f1d77` (default-off; Min/Max exact calendar deleted and planner-derived byte-identically; silicon-proven −30.73% same-source p100a, −30.72% p150), executed generically by craq-sim `f80a8d6`; replay-hoist + recalibrated profitability gate landed; MOP still greenfield (§18.8.0.4) | Compiler-emitted replay matches handwritten replay cycle count across the corpus; every novel formation is correctness-checked and silicon-scored. |
| **E. Front-end / autovectorization** | Auto-vectorize scalar SFPU loops within GCC (today `sfpi` is *explicit* `vFloat` intrinsics, not a vectorizing compiler). Higher-level entry (Triton/linalg) stays deferred with MLIR. **Second GCC-ceiling checkpoint.** | Not started | A scalar-source kernel auto-vectorizes to within a set % of hand-written `vFloat`. |
| **F. Precision & numerics** | bf16/fp32 mixed-precision paths; parity gates against the LLK numeric corpus. | Not started | No precision regression vs handwritten across the corpus. |

### 18.5 Sequencing and the GCC-ceiling checkpoint

Order: **F1–F2 first** (you cannot claim SOTA without the oracle and the corpus harness), then **D**
(closes the residual replay-buffer-compression gap and is high-ROI), then **B** (DST/RWC unblocks the pressure-bound
kernels), then **C** (the hardest, and the one that tests whether GCC can carry tile/dataflow
scheduling). **A** proceeds in parallel as incremental SFPU coverage. **E/F** follow.

**Reconsider MLIR (§8) only at a concrete trigger:** if Track C (cross-engine tile scheduling) or
Track E (autovectorization) cannot be expressed cleanly in GCC's IR without disproportionate
backend surgery. Make that a documented decision point, not a drift.

### 18.6 Does this doc now have enough detail?

For the shipped slice (P0 + raw-LREG fix): yes, thoroughly. For "SOTA vector compiler": this section
is the *skeleton* — it names the tracks, gates, and sequencing that were previously a single diagram.
Each track (B, C, D especially) still needs its own design at the depth §3/§4 gave SFPU scheduling
before it is executable. Treat §18 as the roadmap; the per-track designs are the next writing task.

---

### 18.7–18.10 Detailed Track Designs

These four subsections give the per-track hardware mechanism, GCC backend delta (grounded in the shipped `rvtt` backend), staged milestones, and a measurable hard gate for the foundation cost model (F1) and the three optimization tracks (D, B, C), in the sequencing order §18.5 mandates.

### 18.7 Track F1 — Cost Model on craq-sim + Silicon A/B (foundation)

**Status: F1.0 and F1.1 SHIPPED; F1.2 compatibility phase SHIPPED; F1.3–F1.4 IN PROGRESS.** Commit `88598f4` adds the source-to-artifact scorer and its Blackhole Welford corpus entry. The scorer compiles and archives paired ELF/objdump artifacts, requires CRAQ as a functional gate, runs physical correctness, and copies each device-profiler row before the LLK harness overwrites it. Its pinned bring-up reproduced handwritten replay at `326,326,326` device cycles and generated vFloat at `323,325,323` (15/15 correctness). SFPI-GCC `daa4eb84e` adds the orthogonal Tensix issue-class DFA without changing the existing hazard scheduler; `e19762c51` sources the legacy zero/one bubble count from a generated numeric attribute while preserving the STATIC/DYNAMIC dependency probe and `XTT_DYNAMIC_BUG` semantics. The WH/BH/QSR compatibility matrix at `67ab3afa3` passes 18/18 focused checks and emits the same instruction/NOP streams as the pre-F1 compiler. This is deliberately a byte-compatible cost-source foundation, not a claim that calibrated multi-cycle scheduling has landed. CRAQ's modeled cycles are explicitly not substituted for the `WELFORD_BODY` silicon interval. Per-architecture calibration and identical-source compiler A/B remain open.

**F1.0 oracle audit (2026-08-15): FAILED CALIBRATION GATE.** CRAQ commit `f2d145ea` restores the `simulated_cycles` field that rollback `747e0c95` removed while leaving the extractor schema behind. The repaired model reports the scoped `WELFORD_BODY` at **376 cycles for handwritten replay and 284 for generated vFloat**; silicon (archived raw CSVs, `validation/welford-bh-20260815/`) reports **326 for handwritten replay and 323 for vFloat**. Until the SFPU timing model is calibrated, CRAQ is a functional/debugging oracle and silicon is the performance acceptance authority; compiler scheduling decisions must not be trained to this mismatched result.

**Root-cause analysis (corrected 2026-08-15).** An earlier draft of this note inverted the silicon assignment and concluded CRAQ "predicts the wrong winner." Against the primary CSV artifact that is wrong on both counts: silicon is `replay=326, vFloat=323`, so **both CRAQ and silicon agree vFloat is (marginally) faster** — the failure is *magnitude*, not direction. CRAQ inflates the margin ~27× (CRAQ `−24.5%` vs silicon `−0.9%` for vFloat relative to replay), which still disqualifies it for scoring: a scheduler trained on a 24.5% signal that is really 0.9% will make bad trade-offs. The error is two-sided and diagnostic:

- **CRAQ makes replay look ~15% too slow** (376 vs 326) — it under-credits the **replay-buffer / MOP frontend compression** that is the whole point of the handwritten replay path (§6). CRAQ models backend issue, not frontend throughput.
- **CRAQ makes the vFloat stream ~12% too fast** (284 vs 323) — it under-models **SFPU backend hazards**: the shipped `rvtt-cost.md` DFA sets `rvtt_issue_sfpu = 1`, but §5.2 states SFPU floating-point ops have a **2-cycle result latency**. The DFA mirrors CRAQ's *issue gap* (1/cycle throughput), not the *result latency* (2), and has no notion of concurrent-pipe overlap. Aggregate instruction/stall totals cannot repair this because Tensix pipes progress concurrently.

**Consequences for the roadmap.** (1) F1 must be re-scoped: CRAQ is **not** a performance oracle for SFPU-vs-replay; calibrate its SFPU/replay timing to silicon micro-benchmarks first, or make silicon A/B the scoring authority (§18.7 M-F1.4). (2) The `rvtt-cost.md` DFA is sound as a **NOP-placement / correctness** mechanism but its latencies must not be read as a perf predictor until recalibrated to silicon (not to CRAQ). (3) **Track D performance claims scored only via CRAQ are untrustworthy** because replay is exactly the frontend effect CRAQ mis-models. Existing replay formation remains shipped, and the conservative opt-in loop-hoist has its own changed-binary Reduce-SDPA silicon result; neither should be described as dump-only. Novel `SFPLOADMACRO` emission, broader cross-BB replay discovery, and future replay allocation changes still require independent correctness plus silicon performance gates. ~~Keep only the shipped `pass_rvtt_loadmacro` formation analysis at `emit=no` until its simulator execution model and silicon validation path exist.~~ *[Directive superseded 2026-08-17: that pass is deleted (`5f31e00f0`; its flags hard-error on use) and the prerequisites now exist — generic sim execution at craq-sim `f80a8d6` and Min/Max silicon validation (§18.8.0.4). The surviving rule is the preceding sentence's per-emission correctness + silicon gate.]*

**Re-scoping note vs §18.4.** The master roadmap lists F1's dependents (B/C/D) as "Not started". F1.0 is now shipped; F1.1–F1.4 reuse the shipped NOP-insertion mechanism rather than rebuilding it — the delta is the cost source, not the pass.

#### 18.7.1 Hardware mechanism (what the cost table must reproduce)

craq-sim is the functional reference and candidate timing model; silicon is the current performance
authority until the failed calibration above is repaired. Its timing is driven by a small,
enumerable set of knobs that a static cost table can be fit against and then checked against
silicon:

- **Per-engine issue gap.** After an instruction issues, `tensix_note_execution_resource_issue` sets a per-pipe `busy_until` timer: math ops (wait-gate bit6) get `clock + (MATH_ISSUE_GAP ? gap : 1)`, SFPU ops (bit8) get `clock + (SFPU_ISSUE_GAP ? gap : 1)`, CFG is a fixed 2-stage pipe (`clock+2`), everything else `clock+1` (craq-sim `tensix.cpp:2251-2283`). A math op re-blocks while `clock < math_busy_until` (`tensix.cpp:12068-12075`). The gaps are env-tunable and default 0→1: `TT_METAL_SIMULATOR_TENSIX_MATH_ISSUE_GAP` / `..._SFPU_ISSUE_GAP` (`tensix.cpp:478,483`). **These four numbers (math gap, sfpu gap, cfg=2, default=1) ARE the per-type latency table F1 must encode.**
- **Structural issue hazard (the 5 RTL classes).** At most one instruction per issue class (Math/Sfpu/Tdma/Cfg/Sync) retires per cycle, in fixed priority Math>Sfpu>Tdma>Cfg>Sync with per-class round-robin across pipes (`libttsim.cpp:2265-2280`, `2311-2394`). Within a single pipe (the only stream a GCC compilation owns) this is a same-class back-to-back stall.
- **Frontend FIFO watermark.** The executable FIFO drains one entry/cycle while `size > threshold=31` (`TT_METAL_SIMULATOR_TENSIX_FRONTEND_FIFO_THRESHOLD`, `tensix.cpp:487-494`, `2728-2747`) — a second-order effect that bounds burst issue and interacts with Track D replay expansion.
- **True-dependency stalls** through the SrcA/SrcB bank-valid handshake and DST valid bitmap (Track B/C territory) — out of scope for the *single-stream SFPU* cost table F1 delivers, but F1 must not model them as free.

Device cycle counts are surfaced by the per-cycle clock loop (`clock_tensix_tile_one_cycle_rtl_aware`, `libttsim.cpp:2282`; profile counters `libttsim.cpp:209,260-277`). This is the number the §2.2 Welford row already reports (323 vFloat vs 326 replay-LLK device cycles) — i.e. the oracle output F1 formalizes.

#### 18.7.2 What the GCC backend must model/emit, and where

The shipped scheduler proves the *insertion mechanism* but has no calibrated cost:

- `rtl-rvtt-schedule.cc` (`pass_rvtt_schedule`, registered `INSERT_PASS_BEFORE (pass_postreload, 1, ...)` in `rvtt-passes.def`) walks each insn and, for `get_attr_type == TYPE_TENSIX`, reads a **three-valued** attribute `xtt_delay ∈ {NONE, STATIC, DYNAMIC}` (`rtl-rvtt-schedule.cc:187-249`). STATIC → always emit one `rvtt_sfpnop` after; DYNAMIC → emit a NOP only if the next reader of the written SFPU reg is adjacent; NONE → nothing. This is **latency-1 with a BH/QSR scoreboard-bug exception**, not a pure hardcoded 1: BH/QSR carry a (buggy) scoreboard via `XTT_DYNAMIC_BUG_{BH,QSR}` that can clear `is_dependent` (`rtl-rvtt-schedule.cc:132-141`). The model is therefore *almost* a hardcoded single-cycle latency — enough to prove insertion, not enough to be a cost oracle.
- Generated Tensix commands in `tt/rvtt.md` carry `(set_attr "type" "tensix")`; that membership
  attribute is consumed by `rtl-rvtt-schedule.cc`, `rtl-rvtt-replay.cc`, and related passes. There
  is no orthogonal issue-class attribute, `define_automaton`, per-class `define_insn_reservation`, or
  per-engine `define_cpu_unit`, so GCC's DFA scheduler is effectively unused for these commands.
  `ttrocc.md`'s 87 `type=ttrocc` patterns are QSR RISC-V RoCC interface operations, not the Tensix
  FIFO words classified by craq-sim's opcode ranges.

F1 concretely delivers:

1. **new: `rvtt-cost.md`** — a real DFA. Keep `type=tensix` and add an orthogonal issue-class
   attribute (`math`, `sfpu`, `tdma`, `cfg`, `sync`) to the generated Tensix patterns in `rvtt.md`;
   add one `define_automaton` with a `define_cpu_unit` per class, and a
   `define_insn_reservation` per class whose latency is the craq-sim gap for that class (sfpu-gap,
   math-gap, cfg=2, else=1). The class boundaries are the exact opcode ranges craq-sim uses
   (`libttsim.cpp:2265-2280`; `tensix_wait_gate_block_mask_for_inst` bit map,
   `tensix.cpp:1723-1897`) — mechanically portable, not guessed.
2. **Stage the near-binary `xtt_delay` replacement without changing legacy codegen.** The shipped F1.2 compatibility phase adds generated `xtt_delay_bubbles` values (`NONE=0`, `STATIC/DYNAMIC=1`) and leaves the existing dependency probe plus `XTT_DYNAMIC_BUG` mask intact. A later calibrated phase may generalize the NOP count to `max(0, latency-distance)`, but only after defining emitted-Tensix distance across CFG paths and proving identical output for all one-bubble cases. The issue-class DFA is structural metadata; it must not be conflated with the correctness NOP contract.
3. **extend the SHIPPED `rvtt-tune.md`** (per `-mcpu`: `tt-wh-tensix`, `tt-bh-tensix`, QSR). The file already exists (`tt/rvtt-tune.md`, 66 lines) but carries **only** standard RISC-V `load/fpload/store/imul/idiv/alu` reservations — there is **no** tensix/sfpu reservation in it today. F1 adds one latency row per issue class per arch, each a fitted constant, because craq-sim's own gaps are per-build env constants (`tensix.cpp:478-494`). The `sfpu-ops-{wh,bh,qsr}.h` split already establishes the per-arch table pattern to mirror.
4. **new: `run-corpus-score.sh` (the oracle harness)** — compile → assemble → run under craq-sim → read device cycles from the profile counters (`libttsim.cpp:260-277`), producing a `(kernel, baseline_cycles, candidate_cycles)` table. This is the offline scorer every codegen change is graded by, and the input to the silicon A/B. It builds on the pinned-build + per-selector setup behind the §13.8 Welford archive (`validation/welford-bh-20260815/`), which is today an *archive of numbers*, not a from-source reproduction path — F1.0 builds that path.

Explicitly **not** in F1: no MLIR, no new engine model, no autovectorizer, no exact allocator. F1 is the cost table + the scorer.

#### 18.7.3 Staged milestones

| M | Deliverable | Anchor |
| :-- | :--- | :--- |
| F1.0 | **SHIPPED AS BRING-UP, NOT WHOLE-CORPUS COVERAGE.** `run-corpus-score.sh` archives compiler hashes, ELF/objdump, CRAQ functional results, physical correctness, and device profiles, but its current manifest is Welford-only and its arms are not the required scheduler flag-off/flag-on differential. | `scripts/run-corpus-score.sh`; §2.3; §13.8 archive |
| F1.1 | **SHIPPED (`daa4eb84e`).** `rvtt-cost.md`: orthogonal issue attribute on `type=tensix` patterns + DFA reservations. Real Tensix defaults SFPU, TTREPLAY is TDMA, SFPCONFIG remains SFPU; zero-length ghosts, ordinary RISC-V and `ttrocc` are `none`. | `tt/rvtt.md`; `tt/rvtt-cost.md` |
| F1.2 | **Compatibility phase SHIPPED (`e19762c51`, matrix `67ab3afa3`).** Generated numeric bubble count feeds the unchanged STATIC/DYNAMIC dependency probe; 18/18 WH/BH/QSR checks preserve legacy instruction/NOP placement. Arbitrary calibrated latency-distance remains open. | `rtl-rvtt-schedule.cc`; `cost-schedule-{common,qsr}.C` |
| F1.3 | **extend** `rvtt-tune.md` with per-arch tensix/sfpu reservations (WH/BH/QSR); bound prediction error against silicon microbenchmarks and record simulator residuals rather than fitting blindly to the current mismatched model. | `tt/rvtt-tune.md` (existing); `sfpu-ops-{wh,bh,qsr}.h` pattern |
| F1.4 | Silicon A/B **brought up** (net-new): identical-source, changed-binary, paired off/on device runs feed the same scorer table. No shipped A/B harness exists to "wire" (§14.2/§14.4 mark it Open). | §14; §2.2 GO-BH-ONLY row |

#### 18.7.4 Hard gate (measurable)

Two-part, and both must hold:

1. **Oracle fidelity.** For every calibrated microbenchmark and scored LLK kernel, record compiler-model, simulator, and silicon results separately. The compiler cost model's candidate-vs-baseline ordering must agree with silicon, and its error must remain within a committed per-architecture tolerance. Simulator agreement is reported but is not an acceptance gate until the known replay/SFPU calibration residual is within that tolerance. Concretely: reproduce the Welford body's 323/326 silicon split from `-mcpu=tt-bh-tensix` source through `run-corpus-score.sh`, while also retaining the mismatched simulator result as a visible calibration failure rather than tuning it away.
2. **Every codegen change is scored, never by static instruction count.** The standing regression is the whole-LLK-corpus baseline-vs-candidate run — asm diff + craq-sim + a paired silicon A/B (§18.3 F1 gate, §18.4 gate row). A change that improves static op count but regresses craq-sim/silicon cycles **fails**. This is the gate that retires the "0.0% delta on a bypassed peak-at-most-eight fixture" hole in §14.1: F1 is not accepted until the scorer shows a non-zero, correctly-signed delta on a case that actually exercises the scheduler (`old-peak > 8`, `applied=yes`).

#### 18.7.5 Risks

- **Silicon A/B is net-new bring-up, not wiring.** §14.2/§14.4 and the §2.2 baseline row list the identical-source, changed-binary silicon A/B as **Open**, and §14.1 records the only silicon run as a *bypassed* 0.0%-delta case. F1.4 must stand up that harness from scratch; until it exists, the silicon leg of the gate is unmet.
- **Calibration drift.** craq-sim gaps are build/env constants (`tensix.cpp:478-494`), not silicon-measured; a `rvtt-tune.md` fit to craq-sim can diverge from silicon. Mitigation: the silicon A/B leg (F1.4) is a *required* half of the gate — craq-sim is the fast oracle, silicon is the acceptance authority (GO-BH-ONLY posture, §2.2). Wormhole silicon remains an open validation surface.
- **Single-stream ceiling is inherited, not solved.** The `rvtt-cost.md` DFA models one in-order pipe. It correctly carries per-engine latency and same-class structural hazards, but it **cannot** price cross-TRISC semaphore rendezvous or the implicit SrcA/SrcB bank-valid handshake — true multi-stream dataflow tokens with no RTL analog. F1 must therefore mark those stalls as *modeled-as-barrier* (conservative, non-free), not schedule through them. That boundary is exactly the **Track C GCC-ceiling / MLIR-reconsideration trigger** (§18.4 Track C, §18.5): if the corpus shows F1's single-stream cost model systematically mispredicting on multi-engine kernels because the dominant stalls are cross-thread, that is the documented signal to reconsider an MLIR async-token representation (§8) rather than force a fused-stream fiction into GCC's DFA. F1's honest scope is the SFPU single-pipe cost table; it must not over-claim whole-kernel accuracy.
- **Attribute migration regression.** Annotating the generated `type=tensix` patterns in `rvtt.md`
  risks silently changing NOP placement on already-validated kernels. Keep `type=tensix` intact so
  existing pass membership does not change, and gate F1.2 on bit-identical scheduling of the shipped
  Welford binary before the new latencies are allowed to differ elsewhere.

### 18.8 Track D — Replay / MOP / `SFPLOADMACRO` Emission

*(The heading's former "closes the ~4% gap" tagline was the superseded 339-era Welford figure; the authoritative 323-vs-326 result establishes no remaining Welford replay gap — see the end of §18.8.0.3.)*

**Status (2026-08-17): frontend REPLAY = LANDED with the silicon-recalibrated profitability gate (default-off); MOP compression = GREENFIELD; `SFPLOADMACRO` = LANDED as the generic 7-layer macro planner at gcc `bb56f1d77` (`-mtt-tensix-macro-planner`, default-off; Min/Max exact calendar deleted and planner-derived byte-identically; silicon-proven −30.73% same-source on Blackhole **p100a**, re-confirmed −30.72% on p150), executed generically by craq-sim `f80a8d6` (all shape recognizers deleted). See §18.8.0.4 for the reconciled current state and §18.8.0.5 for its adversarial verification; earlier status lines below are retained as lineage.**

*[Historical status line, SUPERSEDED by the above: "frontend REPLAY = PARTIAL (shipped, single-BB); MOP compression = GREENFIELD; `SFPLOADMACRO` = PARTIAL FOUNDATION (CRAQ `fd8ed6f` executes admitted WH/BH transactional shapes; compiler emission is unlanded)."]*

#### 18.8.0 Silicon Scorecard — Compiler vs Hand-Tuned LLK (p100a-era record; SUPERSEDED as the authoritative scoreboard by §18.8.0.4's p150 table — retained for lineage)

> **[SUPERSESSION NOTE 2026-08-17.]** The table below is the p100a record. The authoritative chip
> class is now **p150** (tt-metal `69d61d66` chip-class-separated baselines); several rows moved
> (Signbit's planner-reproduction condition is DISCHARGED; Typecast worsened to +22.64%; Exp improved
> to +37.61%; Lerp to −13.43%; Expm1 to −2.49%). Do not mix p100a and p150 cells arithmetically.
> Current numbers + comparability rules: **§18.8.0.4**.

**Framing (mandatory 2×2):** `{semantic source, handwritten source} × {passes OFF, ON}`. **Causal** =
semantic OFF→ON (the compiler's own improvement, a causal compiler claim). **Competitive** = semantic-ON
vs hand. *Never mix them.* Blackhole p100a, 3 fresh procs/selector, deterministic ×3. **CRAQ is never a
perf oracle.** Numbers are records — a fresh machine re-measures (machine-local HANDOFF §9–§10 bootstrap/harness recipes) to re-verify.

**Ops with a distinct hand LLK (full 2×2):**

| Op | sem OFF | sem ON | causal (OFF→ON) | hand | vs hand | note |
| :--- | ---: | ---: | ---: | ---: | ---: | :--- |
| **SDPA Exp Unclamped** (BODY marker) | 1048 | 715 | **−31.77%** | 632 | +13.13% | strongest result; large causal win from generic replay-hoist → scoped ownership → dst-autoincr → launch conversion |
| — same, KERNEL marker (migrated infra) | 1299 | 1057 | **−18.63%** | 1018 | +3.83% | drain-inclusive marker (BODY marker is invalid for fire-and-forget replay shapes) |
| **Binary Min/Max** | 226.65 | 156.99 | **−30.73%** | 144.02 | +9.0% | **first physical proof of *derived* (non-hardcoded) SFPLOADMACRO**; was booked as a +41% open loss |
| **Addcmul** | — | 292.99 | — | 292.93 | +0.02% | parity (single paired run); generic pre-IRA Dst-iteration fusion/interleave |
| **Typecast** | — | 313 / 317 | — | 267 / 265 | +17.2% macro / +19.6% sem | open — needs 4-region descriptor sharing (HANDOFF §6b step 4) |
| **Exp semantic** | — | 989.75 | — | 579.74 | +70.7% | biggest open loss (HANDOFF §7b lane) |
| **SigmoidAppx** | — | 361.80 | — | 222.88 | +62.3% | open — needs semantic LUT lowering (HANDOFF §7c lane); improved from the earlier +100.5% |
| **TTNN Where** | — | — | — | — | +96.2% | misc `0x706` ≠ sim `0x770` (different protocols) — closes via the planner's CC-template extension, **not** a patch |
| **TopK** | — | — | — | — | +5.4% | runtime loop/control vs static expansion (zero `SFPMOV` in both) |
| **Signbit** | — | — | — | — | −7.48% | win, but via the **old exact calendar** — must be planner-reproduced (WP8) before it counts |

**Semantic-only ops (no distinct hand — causal only):** **Lerp −2.75%** (landed win); **Expm1 0.000%**
(latency reorder absorbed by BH dynamic stalls); **Log +1.81% / Log1p +2.30%** regressions — now
**byte-identical refusals** under the profitability gate (correctly not shipping a regression).

**Read-out.** Two genuine *causal* wins on hand-having ops — **SDPA Exp Unclamped −31.77%** (only +13.13%
vs hand) and **Min/Max −30.73%** (only +9% vs hand, flipping a +41% loss) — both from **generic** mechanisms
(replay hoisting / scoped ownership / launch conversion; derived non-hardcoded macro emission), not per-kernel
hacks. The open losses (Where, Exp, Sigmoid, Typecast, TopK) each map to a named generic mechanism still to
land. Caveats: delivery cost re-fit to **1.23× per replayed slot** (old 2.2:1 over-predicted launch gains
~7×); all planner/macro flags are **default-off**; the craq-sim magic-number recognizers are now **DELETED**
(the HANDOFF-§6a authorization was executed at craq-sim `f80a8d6` — see §18.8.0.4). **Lineage note:** the earlier F1-track figures (Welford −0.9%, Reduce-SDPA −0.7%,
Reciprocal −1.7%, Binary broadcast tie) are a **different, pre-planner lineage** (HANDOFF §A, "reconcile/retire")
and are retained in the notes below — do **not** sum them with the planner-lineage rows above.

#### 18.8.0.1 Perf-Loss Root Cause — Why Correct-But-Slower

> **[SUPERSEDED 2026-08-17 — retained as the 2026-08-15-era diagnosis.]** The "No `SFPLOADMACRO`"
> error class below is CLOSED generically: the planner now forms derived macros and Min/Max is a
> silicon-proven **−30.73% same-source win** (was +41.0% here). SigmoidAppx's +100.5% and Typecast's
> +19.6% are also stale (now +63.68% / +22.64% on p150). Current table: **§18.8.0.4**.

Reported corpus run (2026-08-15; per-kernel silicon instruction-diff artifacts pending in-repo):
Binary Min/Max **+41.0%**, Typecast **+19.6%**, TopK **+5.4%** — all correct. Addcmul's original
**+21.9%** result is retained below as the fail-before diagnosis; the landed P4 phase-2/phase-3
compiler flow now measures **292.99 vs 292.93 cycles (+0.02%)**, i.e. parity.

**Unifying diagnosis, updated after the Addcmul fix.** Register allocation and correctness are solid
for the measured regions. The original losses were throughput gaps from mechanisms that were absent
at the time, plus a cost model blind to them. The first latency mechanism is now built and validated:
generic pre-IRA adjacent-Dst fusion exposes two independent rows and a late GIMPLE interleaver orders
their dynamic chains before replay formation. The remaining macro and invariant-placement gaps are
still open. Historical baseline structure was:

- The reordering scheduler `pass_rvtt_lp_schedule` **gates entirely on `old_peak > 8`**
  (`gimple-rvtt-lp-schedule.cc:824,829`) — for a peak-≤8 kernel it does nothing.
- `pass_rvtt_schedule` **only inserts NOPs, never reorders** (header *"schedule tensix insns (insert
  nops)"*; `emit_insn_after(gen_rvtt_sfpnop())`, `rtl-rvtt-schedule.cc:252`).

At that baseline there was no latency-hiding scheduler. P4 now fills this gap for a deliberately
conservative class: same-BB, non-aliasing adjacent Dst iterations whose SSA/effect graph proves two
independent chains. A dependent instruction outside that admitted class still pays an explicit
correctness NOP or implicit hardware scoreboard stall. Missing checked-in `SFPLOADMACRO` formation
(path B) remains the dominant loss story for Min/Max, Typecast, and Where.

| Kernel | Loss | Hand-tuned uses | Compiler's error (class) |
| :--- | ---: | :--- | :--- |
| **Binary Min/Max** | +41.0% | load+compute+store fused across `SFPLOADMACRO`'s 4 sub-units | **No `SFPLOADMACRO`** → serial `SFPLOAD→SFPMAX→SFPSTORE`, load latency exposed (most load-bound → worst) |
| **Addcmul** | +21.9% fail-before; +0.02% after P4 | manual `MUL_a,MUL_b,MAD_a,MAD_b` interleave to hide the 2-cycle latency across 2 rows | **Closed to parity:** generic pre-IRA Dst fusion exposes row B, proves mod-4 non-aliasing, and interleaves the two chains before replay capture |
| **Typecast** | +19.6% | `SFPLOADMACRO` load-convert-store pipeline (§7 "memory bound") | **No `SFPLOADMACRO`** → serial convert loop |
| **TopK** | +5.4% | tuned sort network with statically expanded cases | Typed helper is smaller but retains runtime loop/control flow; dynamic-path attribution is still required |

**The compiler's errors, ranked.**
1. **Latency-hiding reorder is partial, not absent.** P4 phase 2/3 is landed for proven adjacent-Dst
   pairs and closes Addcmul from +21.9% to parity. It is not a general modulo scheduler: unrelated
   loops, cross-BB groups, and shapes without a proven independent second chain still fall back.
2. **No checked-in `SFPLOADMACRO` formation (path B).** The original simulator path only
   whitelists known LLK signatures, but CRAQ `fd8ed6f` now provides an audited persistent,
   transactional evaluator for explicitly admitted WH/BH shapes. Compiler config/slot ownership,
   unsupported-shape fallback, and silicon performance remain open. → Min/Max, Typecast, Where.
3. **Cost model blind to the deciding effects (F1 calibration failure, §18.7).** `rvtt-cost.md` models
   `sfpu=1` not the real 2-cycle result latency, and has zero model of replay/`SFPLOADMACRO` frontend
   throughput — so even the scheduling that exists optimizes the wrong objective. Sits under #1/#2.
4. **Residual control-flow/schedule overhead.** TopK's four-result `SFPSWAP` is one RTL `PARALLEL`
   and one emitted instruction, not four moves; the measured typed ELF has zero `SFPMOV`, only one
   additional scalar `mv` in the helper, and substantially fewer static instructions than the
   handwritten ELF. Its remaining +5.4% must therefore be attributed from the executed loop/control
   path before assigning a compiler transform.
5. **Loop-invariant constant rematerialization (no coefficient pinning / LICM).** SigmoidAppx
   (**+100.5%**, the worst measured loss) rematerializes the cubic's constants *per row* and forms no
   replay, instead of pinning the coefficients once (§7 "compiler-managed coefficient pinning"). This
   is neither a latency nor a `SFPLOADMACRO` problem — it is loop-invariant-code-motion / constant
   pinning, and its absence also blocks replay formation on the body. → SigmoidAppx and polynomial
   approximations (log, gelu, erf).

**Diagnostic path (CRAQ can't be used — it mis-models these; §18.7).** For each loss, take the silicon
instruction-class and dependency-distance diff (handwritten vs generated: `SFPLOADMACRO / TTREPLAY /
SFPNOP / SFPLOAD / SFPMOV` plus producer→consumer slot distance), as the Reduce-SDPA and TTNN Where
notes already do. **Confirmed:** both measured Addcmul ELFs contain zero literal `SFPNOP`; generated
captures one seven-slot row with adjacent `SFPMULI→SFPMAD`, while handwritten captures fourteen
slots with `MUL_a,MUL_b,MAD_a,MAD_b`, exposing the independent chain and avoiding implicit scoreboard
delay. Min/Max and Typecast show no generated `SFPLOADMACRO`. The fixes are Track-D (§18.8) plus P4
latency scheduling with cross-iteration exposure, not more register allocation. The first P4 slice
is now validated at parity; broadening must preserve the same alias, SSA, byte-identity, and silicon
gates rather than treating parity as a corpus-wide scheduler win.

### 18.8.0.2 Current-State Audit (2026-08-16)

> **[SUPERSEDED 2026-08-17 by §18.8.0.4.]** This audit's "**0 classes Closed, 4 Partial, 1 Open**"
> roll-up and its kernel accounting were true against tip `e4b974208cc` / HEAD `8f943c2f84a` and are
> obsolete at pin `bb56f1d77`: the Min/Max exact calendar is deleted and planner-derived (silicon
> −30.73%), WP8 is complete (quarantined pass deleted), the sim recognizers are gone (`f80a8d6`),
> and the profitability gate is landed + recalibrated. Retained unedited as the audit-methodology
> record.

Scope: reconciles the five error classes and the §18.8.0 kernel scorecard against what is actually checked in on `origin/nkapre/sfpi` (tip `e4b974208cc`) vs the checked-out HEAD `8f943c2f84a`. Rule applied throughout: **no win and no closed loss is claimed without a fresh Blackhole silicon number.** Where a fix has landed but not been re-measured, the status is stated as *landed; silicon remeasure pending* and the old loss figure is treated as **stale-pending**, not superseded.

#### (1) Per-error-class status

| # | Error class | Mechanism landed | Status | Silicon-proven vs landed-unmeasured | Default state |
| :-- | :--- | :--- | :--- | :--- | :--- |
| 1 | **Latency reorder / interleave** | `fill_latency_bubbles` (1-slot muladd-only, 3-insn window) + adjacent-Dst-**pair** fusion/interleave (`gimple-rvtt-dst-iteration.cc`) | **Partial** — narrow, structurally-gated peepholes; not a general list/modulo scheduler | **Silicon-proven for exactly one kernel** (Addcmul parity, single paired BH run — not a corpus A/B). Everything else landed; silicon remeasure pending. Not on HEAD (branch-only). | Init(0), opt-in |
| 2 | **SFPLOADMACRO formation** | Real emitters for 3 shapes: sign-bit configured-region, U16→BF16 cast/round, predicated 3-load select (`rtl-rvtt-loadmacro.cc`, rvtt.md `_int`/`_select_int`) | **Partial** — 3 shapes emit launch words; of these, the select/Where shape is not yet sim-recognized (misc mismatch, see §3). Min/Max still **dump-only** (never pushed to emit vector) | **Landed; silicon remeasure pending.** All coverage is compile-only `scan-assembler` (.ttinsn/SFPCONFIG counts); "3 shapes emit" ≠ "3 shapes sim-execute". No silicon, no CRAQ-run assertion. Branch-only, not on HEAD. | Init(0), `-mtt-tensix-emit-loadmacro` |
| 3 | **Cost-model calibration** | Silicon-corpus-derived loadmacro pipeline (`653244e26ab`); §18.7 explicitly excludes all modeled-cycle CRAQ deltas from the scorecard | **Open** — CRAQ modeled cycles are functional-only and disqualified as a perf authority (§18.7 calibration failure) | **Not silicon-authoritative by construction.** Only real silicon numbers admitted to the scorecard; MulInt32 562.6/283.9 are CRAQ math-cycles, excluded. | n/a |
| 4 | **Control-flow / schedule (counted-loop replay)** | `counted_loop_payload` + `hoist_counted_loops` (preheader capture, one in-loop playback) + one-trailing-`TTINCRWC` relaxation | **Partial** — real, test-covered (TTREPLAY 2); single-BB (`header==latch`) only, no cross-BB | **Landed; silicon remeasure pending for the counted path** (Reduce-SDPA / Reciprocal kernel names not in-tree; wins credited under §18.8.0 come from the *generic D1* hoist, which is silicon-measured). Counted-hoist commits branch-only, not on HEAD. Baseline single-BB unrolled replay is default-on. | Counted: Init(0), `-mtt-tensix-optimize-replay-hoist`. Baseline: Init(1) |
| 5 | **LICM / constant-pinning** | `canonical_insn_buffer_p` matcher fix (accepts `__instrn_buffer` ADDR_EXPR, not just `integer_zerop`) → invariant SFPU immediate hoist now fires on real typed SFPI bodies | **Partial** — matcher fix real and correctly scoped (impostors rejected); hoist provably fires on typed form | **Landed; silicon remeasure pending.** Compile-only `scan-tree-dump` ("Hoisted invariant SFPU immediate" ×2). No dg-do run, no A/B, no device cycles. Branch-only, not on HEAD. | Init(0), `-mtt-tensix-optimize-invariant-loadi` |

Summary: **0 classes Closed, 4 Partial, 1 Open.** Only one kernel-level number across all five classes is silicon-proven (Addcmul — single paired BH run, not corpus A/B — and it is parity, not a win).

#### (2) Kernel scorecard with landed-fix / measured accounting

All figures are Blackhole `*_BODY` profiler-zone cycles (lower = better); Δ is generated vs hand-tuned LLK. Wormhole silicon is entirely unmeasured (GO-BH-ONLY, no device).

| Kernel | LLK | Gen | Δ | Silicon? | Landed fix + measured? | Standing status |
| :--- | ---: | ---: | ---: | :--- | :--- | :--- |
| **Welford** | 326 | 323 | −0.9% | Yes, current | n/a (bypasses pressure rewrite) | **Win** (marginal; not a scheduler A/B) |
| **Binary broadcast** | 608 | 608 | 0.0% | Yes, current | n/a | **Tie** (zero-regression flow swap) |
| **Reduce-SDPA** | 840 | 834 | −0.7% | Yes, current | **Fix landed + measured** (generic D1 preheader hoist) | **Win** — doc-recorded prior +2.0% loss, now −0.7% on current silicon |
| **Reciprocal (acc. BF16)** | 467 | 459 | −1.7% | Yes, current, 2/2 BH | **Fix landed + measured** (10-slot capture + 7 playbacks) | **Win** |
| **Addcmul** | 292.93 | 292.99 | +0.02% | Yes, current, 2/2 BH | **Fix landed + measured** (pre-IRA Dst fusion/interleave) | **Parity** — supersedes prior +21.9% loss; explicitly NOT a win |
| **Binary Min/Max** | 140.93 | 198.76 | +41.0% | Yes, current | **No fix landed** (Min/Max is dump-only; SFPLOADMACRO path-B not emitted) | **Open loss** — number current |
| **Typecast Float16_b** | 265 | 317 | +19.6% | Yes, current | **Fix landed; silicon remeasure pending** (U16→BF16 cast/round emitter landed on branch; unmeasured, off HEAD) | **Loss, stale-pending** — no fresh silicon on emitted macro |
| **TTNN Where** | 159.25 | 312.50 | +96.2% | Yes, current, 2/2 BH | **Fix landed on branch but not confirmed sim-executable as emitted** (misc 0x706 vs sim-required 0x770); silicon remeasure blocked pending sim-recognition. ICE fix on HEAD is correctness-only, byte-identical non-debug asm | **Loss, stale-pending** — not sim-recognized as a where shape; perf unmeasured (see §3) |
| **TopK** | 5038 | 5310 | +5.4% | Yes, current, 2/2 BH | **Partial fix landed; silicon remeasure pending** (multi-result typed-IR safety model cleared blocker; perf A/B not re-run, no win claimed) | **Loss, stale-pending** (safety unblocked, perf unmeasured) |
| **SigmoidAppx** | 222.85 | 446.85 | +100.5% | Yes, current, 2/2 BH | **Fix landed; silicon remeasure pending** (counted-loop replay hoist + pressure-safe invariant-const hoist; this is their first active discriminator) | **Loss, stale-pending** — first Sigmoid silicon result OPEN; the +100.5% is the standing, not-yet-superseded number |

Reading key:
- **Superseded (fix landed AND re-measured):** Reduce-SDPA (+2.0% → −0.7% win), Addcmul (+21.9% → +0.02% parity). These two are the only old losses fully closed by fresh silicon.
- **Stale-pending (fix landed, NO fresh silicon):** SigmoidAppx (strongest case — two mechanisms landed, explicitly its first discriminator), Typecast, TTNN Where (also not sim-recognized as emitted), TopK (partial: safety only). Their tabled loss figures are **still the current standing numbers** and must not be reported as closed.
- **Open, no fix landed:** Binary Min/Max (+41.0%) — Min/Max recognized but never emitted.
- **CRAQ-only, off-scorecard:** MulInt32 562.625 vs 283.93 are Blackhole MATH cycles from the dump-only analyzer (emit=no), excluded per §18.7; no win claimed.

Net: **3 silicon wins, 1 tie, 1 silicon parity (Addcmul), 1 open silicon loss (Min/Max), 4 stale-pending losses.** No previously-losing kernel is reported closed unless it carries a fresh Blackhole number.

#### (3) Remaining correctness risks (explicit)

**Address-width / launch-word packing (SFPLOADMACRO).** The prior address-mode bug is worked around, not eliminated. `emit_select_launch` force-sets `macro_address_mode=0` with an in-code comment that copying the proven load address-mode would overlap InstrMod0 and silently turn an opening F16b mode-2 load into mode-3 on Blackhole. Packers are inconsistent: the select path shifts `address_mode << 14` unconditionally, while the configured/cast paths shift `<< (BH?13:14)`. Select bounds addresses to 8-bit (≤0xff); configured/cast bound to 10-bit (≤0x3ff). This is a **latent width/overlap hazard** if the select path is ever enabled on WH with a nonzero address_mode.

**Misc-word mismatch (Where path) — confirmed sim-executability failure.** This is not indeterminate. The compiler's `emit_select_config` emits `emit_config_word(lreg0, 0x00000706u, 8)` (`rtl-rvtt-loadmacro.cc:1164`); config index 8 writes `load_macro_misc` (`craq-sim/src/tensix.cpp:9756`); the sim's `where_loadmacro()` gate requires `load_macro_misc == 0x770u` (`tensix.cpp:10106`), while templates `0x7B0000C6`/`0x8A0000D0` do match. The emitted Where macro programs misc `0x706`, which does not satisfy the sim's `where_loadmacro()` requirement of `0x770`; **as checked in, the emitted Where macro is not sim-recognized as a where shape** (0x706 ≠ 0x770) and falls through to `UnsupportedFunctionality` (or the generic loop). Combined with the stale-pending perf status, the Where emitter is neither measured nor confirmed sim-executable as emitted.

**CC transactional / sim model is signature-matched, not event-modeled.** `TENSIX_EXECUTE_SFPLOADMACRO` dispatches by exact template/sequence/misc constant match; unmatched shapes hit `UnsupportedFunctionality`. A generic 4-sub-unit `SequenceBits` decode loop exists but (a) is reached only after the leading signature special-cases return, and (b) is **functional-immediate** — the in-code comment states the delay field is ignored and a real caller needing cycle timing would require a sub-unit FIFO model that does not exist (`tensix.cpp:9878`). So the calendar is **signature-recognized, not timing-simulated**; CRAQ cycle deltas are functional-only and are correctly excluded from the silicon scorecard (§18.7). SFPSWAP-to-LREG16 collision and the "3 NOP drain vs greatest-programmed-delay" question remain called-out-unproven.

**Sim-gating / default-off across the board.** Every landed optimization in classes 1, 2, 4-counted, and 5 is `Init(0)` and exercised only under explicit non-default flags; a stock `-O2` build gets none of them. All emitter/hoist/matcher coverage is **static** (`scan-assembler` / `scan-tree-dump`) with **no `dg-do run`, no A/B, no device cycles**. The `canonical_insn_buffer_p` matcher keys purely on the assembler name `__instrn_buffer` + external/public linkage (it does not check the `rvtt_reg_ptr` attribute) — benign under ABI reservation and impostor-rejected in tests, but noted.

**Provenance caveat.** The SFPLOADMACRO emitters, counted-loop replay hoist, invariant-hoist matcher fix, and the Dst-iteration passes all live on `origin/nkapre/sfpi` and are **not ancestors of the checked-out HEAD `8f943c2f84a`** (all 13 commits verified non-ancestors); `gimple-rvtt-dst-iteration.cc` and the counted-loop payload code do not exist on-disk at HEAD. The on-HEAD `rtl-rvtt-replay.cc` is the pre-extension version. Any default build of HEAD reflects none of the class 1/2/4-counted/5 mechanisms.

#### 18.8.0.3 Update (2026-08-16): generic macro planner pinned — provenance closed, sim gap now the wall

> **[PARTIALLY SUPERSEDED 2026-08-17 — see §18.8.0.4.]** The compiler-side account below stands, but
> two of its findings are now CLOSED: (a) the "sim side untouched" wall fell — craq-sim `f80a8d6`
> deleted every recognizer (`0x1b8400de`, `0x770`, all template constants) and executes macros
> generically from SFPCONFIG-programmed state, so planner shapes are no longer whitelist-bound; (b)
> "unvalidatable, unscorable, unmeasured" no longer holds — the 8/8 Min/Max CRAQ matrix is bit-exact
> through the generic path and fresh p150 silicon exists for Min/Max and Typecast; lane-predicated
> shapes (typecast faces, Where successors) remain gated on the D1 all-lanes fix (§18.8.0.5). The pin
> reference `ddf44ed64` is stale by two advances (now `bb56f1d77`).

Supersedes the two stale findings above. The `agent/generic-macro-planner` + `agent/toolchain-pin`
merges advanced the submodule pin `8f943c2f84a → ddf44ed64` (`nkapre/sfpi` `02bf3e1`), so the
"a default build reflects none of the landings" finding in §18.8.0.2 and the "enabler absent /
`30d3c6207` not a valid object" status in §18.9 are **now closed** — the enabler and planner are in
the pinned toolchain (all `Init(0)`, default-off).

**Compiler side (real, pinned).** A WP0–WP8 refactor replaces the hardcoded magic-number emitters with
a table-driven planner: per-arch capability tables (`rvtt-macro-tables-{bh,wh}.def`), descriptor /
ownership / region / scheduler / verifier subsystems (`rtl-rvtt-macro-planner.cc`,
`rvtt-macro-{desc,ownership,region,sched,verify}.*`), typed RWC barriers (`TTSETRWC`, Dst-face-advance
= WP1, the Track B enabler §18.9 designed), and a path-sensitive ownership analysis (WP3). WP7 **deletes
the Min/Max hardcoded calendar**; WP8 **quarantines the old loadmacro pass for deletion**. Flags:
`-mtt-tensix-{analyze,emit}-loadmacro`, `-mtt-tensix-macro-planner`, all `Init(0)`. The planner
"emits only structurally proven SFPLOADMACRO calendars" — a real resource model, not constant-matching.

**Sim side (untouched — now the binding constraint).** Zero craq-sim commits. It still recognizes
macros by hardcoded constants (`0x1b8400de`, `0x770`, `tensix.cpp:9928,10106`) and still ignores the
delay field (functional-immediate, `tensix.cpp:9877`). Consequence: the planner can now form *general*
macro shapes, but the sim only accepts whitelisted templates → `UnsupportedFunctionality` on novel
shapes (the compiler out-generalized its own oracle), and no sim path yields SFPLOADMACRO *timing*.
So every macro kernel (Where +96%, Min/Max +41%, Typecast +20%) remains **unvalidatable beyond the
whitelist, unscorable on CRAQ, and unmeasured** (all planner flags default-off). The next gate is the
4-sub-unit timing model in craq-sim (or a silicon-only harness), not more compiler passes.

**Reduce-SDPA discriminator (2026-08-15).** TT-Metal `6d7c0fdb` adds a test-only identical-math handwritten-replay/generated-SFPI selector and a serialized Blackhole profiler archive without changing the production LLK. Both paths pass the full 512x64 four-subblock golden. The handwritten 8-slot replay body measures `839,839,839` `REDUCE_SDPA_BODY` device cycles; the first generated SFPI form measures `914,914,914` (`+75`, `+8.94%`). Its raw `TTI_SFPLOAD` operations are opaque `.ttinsn` barriers to GCC even though the linked ELF looks replayable. TT-Metal `f46e98b5` expresses the same loads through the typed compiler API; the existing post-RA pass then forms two 8-slot captures and fourteen static playbacks, and silicon improves to `855.5,855.5,855.5`, recovering 58.5 cycles (78% of the deficit) without a compiler change. The remaining `+16.5` cycles (`+1.97%`) were then closed by generic D1 preheader capture hoisting: the current pinned result is handwritten `840` vs generated `834` — a **−0.7% generated win**, the corpus's first outright flip (§18.8.0). This note is retained for the recovery history (opaque `.ttinsn` → typed API → hoisting). Arbitrary raw-asm decoding is rejected; opaque asm remains a barrier. Artifacts and hashes are recorded in TT-Metal `tt_metal/tt-llk/tests/corpus/REDUCE_SDPA_SILICON_AB.md`.

**Broader LLK conversion checkpoint (2026-08-15).** TT-Metal `c1471817` carries two additional test-only corpus lanes. Binary broadcast passes 8/8 representative Blackhole correctness cases, 6/6 Wormhole generated compiles, and CRAQ A/B; physical `BINARY_BCAST_BODY` is exactly tied at handwritten `608,608,608` versus generated SFPI `608,608,608`. This is a zero-regression compiler-flow replacement, not a speedup.  SFPI-GCC `8f943c2f8` fixes the canonical TTNNWhere `v_if` debug-build ICE by resetting stale `DEBUG_BIND` uses when RVTT removes scalar predicate definitions; WH/BH/QSR focused checks pass and non-debug assembly is byte-identical.  The corrected U16 selector passes CRAQ and Blackhole correctness, but measures handwritten `159.25,159.25,159.25` versus generated `312.50,312.50,312.50` `TTNN_WHERE_BODY` cycles.  The executed caller already receives the generic outermost-CC combine; the remaining 3-slot-versus-7-slot gap is SFPLOADMACRO formation, not PUSH/POP lowering. Evidence is in TT-Metal `tt_metal/tt-llk/tests/corpus/{BINARY_BCAST_SILICON_AB,TTNN_WHERE_COMPILER_AB}.md`.

**Replay legality and TopK checkpoint (2026-08-15).** SFPI-GCC `32fe8cd23` makes replay payload legality explicit: all 67 emitted Tensix patterns are classified (64 safe, two barriers, one explicit owner), opaque asm defaults to a boundary, and explicit TTREPLAY slot ownership is processed before candidate formation. Its replay corpus is 41/41 and the full target-suite failure set is unchanged from baseline. TT-Metal `02de2580` records why TopK cannot yet be converted safely: typed `SFPSWAP` omits the simultaneous L4–L7 index-pair results when index tracking is enabled, and typed `SFPTRANSP` omits the live L4–L7 transpose group. The required fix is a general multi-result architectural model plus post-RA pair verification; no accidental-allocation silicon result is accepted.

SFPI-GCC `c4e4e809a` implements that architectural model without a TopK-specific
pattern.  Indexed SFPSWAP is one four-SET RTL operation whose allocation alternatives enforce
value registers in L0–L3 and exact companion outputs at value+4; the eight-register transpose is
one PARALLEL with explicit L0–L7 uses and definitions.  WH/BH/QSR allocator and encoding checks
pass 15/15, including unconstrained adversarial allocation, and the existing CRAQ SFPSWAP
differential passes 100/100 on both WH and BH.  This clears the typed-IR safety blocker; a generated
TopK functional/performance A/B remains follow-up work, so no TopK silicon win is claimed yet.

**D1 replay-hoist result (2026-08-15).** SFPI-GCC `5a849606f` adds a default-off,
post-RA loop optimization for fixed-encoding, compiler-visible Tensix replay payloads.  It records
with no execution in a dedicated preheader, replaces the in-loop clones with playback, reserves
the selected slots for the remaining function, and rejects MEM/GPR-dependent payloads, opaque
assembly, calls, explicit replay owners, abnormal entries, and non-single-block loops.  A typed
architectural-L8 discard load closes the final Reduce-SDPA raw-instruction boundary without
manufacturing an allocatable result.  The compiler gates are 52/52 replay tests and 713 target
passes with the baseline 15 failures and two expected failures unchanged; ineligible output is
byte-identical on/off.  On Blackhole, the paired full-golden Reduce-SDPA test passes both selectors
and measures handwritten replay at `840,840,840` versus generated typed SFPI at `834,834,834`
scoped `REDUCE_SDPA_BODY` device cycles: a repeatable 6-cycle (`0.714%`) generated-code win.
CRAQ is a functional gate here, not the performance authority.

**Counted-loop replay + invariant placement follow-up (2026-08-15).** SFPI-GCC `6422dbd9e3`
extends the opt-in replay-hoist lane to one-BB counted loops containing one uninterrupted,
fixed-encoding replay-safe SFPU run. It requires an existing dedicated preheader, rejects
MEM/calls/asm/dynamic words/config/counters/replay owners, preserves WH scheduler NOP payloads,
and allocates persistent slots around explicit owners. SFPI-GCC `2bfa165348` separately hoists
loop-invariant SFPU immediate materialization before IRA, but only after a transactional pressure
preflight proves at most eight LREGs including hoisted constants, PHIs, external live-through values,
and loop-defined live-outs; any opaque function asm or unmodeled call refuses. The focused suites are
63/63 replay and 47/47 invariant with WH/BH/QSR byte-identical ineligible fallback. These commits
are compiler mechanisms, not silicon wins yet; the Sigmoid semantic lane is their first active
changed-binary performance discriminator.

**Durable corpus checkpoint (2026-08-15).** TT-Metal `164a10f2` replaces the original 11-row
prioritization list as the coverage authority with 164 surface-qualified implementations and 332
WH/BH/QSR paths (152 BH, 138 WH, 42 QSR).  The versioned manifest hard-fails on inventory drift;
compile CI covers all three architectures; pinned CRAQ runs publish modeled-cycle artifacts; and
serialized silicon results compare against a checked-in baseline keyed by operation, architecture,
metric, scope, and selector.  CRAQ `aabbd10` adds spec-correct SFPSTORE LO16 mode-9 execution and
WH/BH regressions.  Simulator modeled cycles remain distinct from physical device cycles.

**SFPLOADMACRO formation checkpoint (2026-08-15).** SFPI-GCC `a1c5665f0` pins the
MulInt32 compiler-flow gap and adds a default-off, dump-only post-RA discovery pass before replay
formation.  The measured typed path remains `562.625` versus handwritten `283.9296875` Blackhole
math cycles: the handwritten implementation preprograms delayed templates with hidden LREG, CC,
Dst/RWC, subunit-calendar, write-port, drain, and replay-lockstep effects, so a local opcode
peephole or naked public builtin would be unsound.  The analyzer emits no RTL, reports stable
per-candidate rejection reasons, and is byte-identical off/on across WH/BH/QSR, including when D1
replay hoisting is enabled.  Its 21 analyzer, 11 replay-hoist, six discard, and six MulInt baseline
checks pass.  Actual formation remains gated on a compiler-owned macro descriptor and a simulator
event model for arbitrary delayed sequences; no MulInt performance win is claimed yet.

**Re-scoping note vs §18.4.** The master roadmap lists Track D as "Not started"; this design upgrades the REPLAY leg to PARTIAL because it genuinely reuses the shipped `pass_rvtt_replay` (834 lines) and the `ttreplay` builtin. MOP remains GREENFIELD. `SFPLOADMACRO` has a partial CRAQ execution foundation for admitted shapes, while compiler emission remains unlanded.

The authoritative Welford result is generated vFloat 323 versus replay LLK 326 device cycles on
Blackhole: generated is already ~0.9% faster on that body. It therefore does not establish a
remaining Welford replay gap and does not motivate scheduler default-on. Track D is motivated by
other corpus kernels and by general frontend compression opportunity; path A replay and path B
`SFPLOADMACRO` are independent datapaths and must not be conflated.

#### 18.8.0.4 Landed-State Reconciliation (2026-08-17) — current authoritative status

Supersedes the §18.8.0.2 "0 closed / 4 partial / 1 open" framing, the §18.8.0 p100a scorecard as
the authoritative table, and §18.8.0.3's "sim gap is the wall" finding. State is at the tips in
the §18 current-coordinates block (sfpi-gcc `bb56f1d77` / tt-metal `69d61d66` / craq-sim `f80a8d6`
/ sfpi `9a555cb89f`), adversarially verified per §18.8.0.5.

**What landed since the 2026-08-16 audits:**

- **Min/Max exact calendar DELETED and planner-derived.** WP7 deleted the hardcoded calendar; the
  generic planner re-derives the emission byte-identically (including the real in-place-store kernel
  via a generic store-demoted scheduling fallback). Evidence: byte-parity oracles, **8/8 CRAQ
  digests EXACT** vs the frozen oracle, and silicon — p100a **−30.73% same-source**, **+9.0% vs
  hand** (was a booked +41% open loss); re-confirmed on p150 at −30.72% / +8.26% (ratio-level, see
  table). First physical proof of *derived* (non-hardcoded) `SFPLOADMACRO` emission.
- **WP8 complete.** `rtl-rvtt-loadmacro.cc` (the quarantined exact-calendar pass) deleted entirely
  (`5f31e00f0`) *after* oracle mint (`32e20f9fd`; re-minting with an independent tip build reproduces
  every committed hash); typecast four-region descriptor sharing landed (`50cad63fa`); Where →
  named `cc-template-unsupported` refusals; `-mtt-tensix-{analyze,emit}-loadmacro` now hard-error
  on use. Signbit formation is structural (no transplanted calendar; shifts form-or-refuse by
  encodability).
- **Recognizers deleted from the sim** (craq-sim `f80a8d6`): every HANDOFF-§6a-listed recognizer and all recognizer
  state removed, no renamed re-introduction; `SFPLOADMACRO` decodes templates/sequence/misc purely
  from SFPCONFIG-written state; 8/8 Min/Max matrix bit-exact through the generic path (1024
  launches/test on ON legs, 0 on OFF); diff-fuzz strengthened 12→13 directed tests, 1000/1000 PASS
  bh+wh. The compiler no longer out-generalizes its own oracle.
- **Profitability gate landed + RECALIBRATED** at `bb56f1d77` from 4 silicon points (the old formula
  ordered the silicon winner below both losers): `benefit = trips×(deliver − max(123, execute)) −
  deliver` in **centislots** (deliver = (1+len)×123, execute = 100×len), MIN_BENEFIT=60;
  `-mtt-tensix-replay-hoist-min-benefit=` changed units slots→centislots and default 64→60.
  Verified a *plane, not a curve-fit notch* (76 unseen probes match exactly; no notch near
  ReduceSDPA (4,8)=121). Log/Log1p remain byte-identical refusals; ReduceSDPA (4,8)=121 and
  SDPA-exp (8,24)=2325 fire. The no-silicon band is now modeled benefit **[60,121)** centislots.
- **Default codegen byte-identical** vs `e4b974208` (pin corpus 3/3 recorded hashes + 238 tensix
  testsuite files at default flags).

**Per-error-class roll-up (supersedes the §18.8.0.2 table):** class 1 (latency reorder) partial,
silicon-proven for Addcmul and the sdpa stack; class 2 (`SFPLOADMACRO` formation) **CLOSED
generically** — planner-derived, Min/Max silicon win — with the residual losses attributed
elsewhere *(caveat: the Typecast +22.64% attribution is SUSPECT pending the fired-vs-refused
planner dump on the real node — if the shared descriptor refused, that residual IS a formation
gap; see the loss table below and §18.8.0.5)*; class 3 (cost model) — the *profitability* model is now silicon-recalibrated (the F1
Welford-timing oracle calibration remains a separate open item, §18.7); class 4 (counted replay)
landed + silicon-measured (Reduce-SDPA 834 vs 840); class 5 (LICM/invariant hoist) landed and
silicon-exercised inside the SDPA stack. The "0 classes Closed" framing is obsolete.

**Current loss table — chip-class split. AUTHORITATIVE CLASS = p150** (tt-metal `69d61d66`,
chip-class-separated baseline files; the p100a file is immutable and retained for lineage; never
mix classes arithmetically):

| Op | p150 status (at `69d61d66`) | Comparability to the p100a record |
| :--- | :--- | :--- |
| **SDPA Exp Unclamped** | sem 1289→1036 (**−19.63% causal**, KERNEL marker) / hand 1009 (+2.7% vs hand) | ratio-level only (chip changed; whether the measured ON bytes equal `b0d9e72e` is not recorded in-repo) |
| **Binary Min/Max** | **−30.72% causal** / +8.26% vs hand | **NOT same-source** with the p100a 226.65/156.99/144.02 records — `65d2c873`/`4ff5c848` changed the measured kernel's `.text` for both impls (verified); ratio-level reproduction only |
| **Signbit** | planner-fired sem ON **−22.98% causal**; **beats hand −5.81%** | the §18.8.0 "must be planner-reproduced before it counts" condition is **DISCHARGED** |
| **Reduce-SDPA** | generated **834** vs hand **840** (`bb56f1d77`-built compiler; gate-fix promotion pair) | equals the archived p100a-era 834/840 (corroborated by post.csv); labeled a reproduction in-repo |
| **Expm1** | −2.49% | was exactly 0.000% on p100a |
| **Lerp** | −13.43% | was −2.75% on p100a |
| **Exp** | **+37.61%** (open loss, HANDOFF §7b lane) | was +70.7% |
| **SigmoidAppx** | **+63.68%** (open loss, HANDOFF §7c LUT lane) | was +62.33% |
| **Typecast** | **+22.64%** (WORSENED; the "WP8 step 4 targets it" attribution is SUSPECT — see §18.8.0.5) | was +17.2%/+19.6%; the ON leg used a BODY-family marker on a macro-launch shape with no issue-slot lower-bound check recorded |
| **Log / Log1p** | byte-identical refusals under the recalibrated gate | unchanged (correctly not shipping a regression) |

Comparability rules: the p150 2×2 cells are internally same-source and valid; p100a "CONFIRMED"
annotations on Min/Max must be read as **ratio-level reproductions across a changed kernel and a
changed chip**, not same-source identity; per-class baseline files + `chip_class` column, no
cross-class arithmetic. Raw Lane D/E run records live on tt-quietbox-0 (`~/sfpi-uplift`), not
in-repo.

#### 18.8.0.5 Adversarial Pull-Analysis (2026-08-17) — four independent reviews of the landed state

Condensation of `PULL_ANALYSIS-20260817.md` (kept alongside the HANDOFF): four adversarial reviews
(GCC, CRAQ-sim, tt-metal, cross-repo), each with independent scratch builds, no silicon, covering
sfpi-gcc `e4b974208 → bb56f1d77`, tt-metal `55ce75be → 69d61d66`, craq-sim `be8e8597 → f80a8d6`,
sfpi `→ 9a555cb89f`.

**Headline verdict.** *The newly-landed state is trustworthy in its core claims and clean of
hardcoding, but NOT yet trustworthy as an unattended pipeline: 6 CONFIRMED defects, all fixable,
none requiring a rollback.* sfpi-gcc `bb56f1d77`, craq-sim `f80a8d6`, and the sfpi pin are sound to
keep. The compiler all-lanes gap (D1) must be fixed **before any lane-predicated shape class
(typecast faces, Where successors) reaches silicon**; the tt-metal sweep gates (D2–D4, D6) must be
fixed **before the next scheduled nightly/weekly is trusted**.

**Refuted suspicions (every load-bearing claim survived attack):**

- The profitability recalibration is a **verified plane, not a curve-fit notch**: 76 unseen
  (trips,len) probes with aperiodic payloads match the decision surface exactly; single kink at
  len≈1.23; no notch near Reduce-SDPA (4,8)=121; only threshold 60 is calibration, and any
  threshold in (0,121] gives identical decisions on all measured points.
- The **WP8 oracle chain is byte-verified end-to-end**: mint precedes delete; re-running
  `mint-wp8-oracles.sh` with an independent tip build reproduces every committed hash; planner
  output byte-identical to the frozen quarantined-pass oracles; all refusal shapes byte-identical.
- The three **unification merges are mechanically faithful** semantic unions (recreated trees
  byte-identical to the committed merges).
- The **sim recognizers are genuinely gone**: zero hits for the deletion inventory, no renamed
  re-introduction; Min/Max executes 8/8 bit-exact purely from programmed state — the very words
  (seq `0x00dd008c` / misc `0x330`) the deleted recognizer special-cased now run generically.
- **Zero hardcoding findings across all four reviews** — the non-negotiable rule and its sim
  extension both hold at the tips (the `desc_programs[]` three-entry whitelist passed: keyed by
  derived structure, unproven values refuse, genericity proven by renamed/varied/near-miss tests).
- Default codegen byte-identity vs `e4b974208` confirmed; `12e1dc0b4` (test separation) does NOT
  hide a default-gate regression.

**The 6 CONFIRMED defects (file anchors):**

- **D1 — all-lanes-enable proof gap (compiler soundness, P0).** The planner accepts *any pure CC
  write* as the ambient enable (`rtl-rvtt-macro-planner.cc:216-266`, both as `rows[0].enable` and
  via `preheader_trailing_enable`). Reproduced on a tip build: a lanes-OFF `SFPENCC(0,10)`
  (imm12=0 ≠ SFPENCC_IMM12_BOTH=3) **forms the full frozen macro calendar**; the deleted quarantined
  pass refused this. `sfpencc_all_lanes_word()` is dead code; the comment at
  `rvtt-macro-ownership.cc:98-102` overstates the implemented guarantee. Formation outside the
  proven envelope is CONFIRMED; hardware misbehavior is PLAUSIBLE. Fix before lane-predicated
  shapes hit silicon.
- **D2 — weekly DejaGnu gate inversion (gate integrity, P1).** In `weekly_bh_sweep.sh`,
  `FAIL=$(grep -c '^FAIL' g++.sum || echo 0)` yields the two-line string `0\n0` on a clean run
  (`grep -c` prints 0 AND exits 1), the `-eq` test errors, and the `||` branch sets RC=1 — **RED
  precisely when clean**; the intended zero-FAIL enforcement never functions.
- **D3 — `knob_silicon()` bypasses CRAQ + correctness (P1).** In `sweep_2x2.py run()`, weekly
  per-knob device jobs execute BEFORE the BH-CRAQ gate is evaluated, consult no craq verdicts, and
  never run correctness for the single-knob flag sets — violating the silicon-protocol items (3)/(6) ordering (machine-local HANDOFF §1) for every
  weekly headline row.
- **D4 — win→refusal regressions pass GREEN (P1).** If a previously winning row goes OFF/ON
  byte-identical (planner stops firing), `report()` emits "refusal byte-identical: GREEN" and never
  consults the measured baseline cells — total-refusal regressions are invisible.
- **D5 — `65d2c873`'s "neither instruction stream changes" claim REFUTED (P2).** Recompiled at
  `65d2c873~1` vs tip, same toolchain/flags: `math.elf` `.text` changes for BOTH Min/Max impls, and
  the clamp is inside the timed TILE_LOOP zone. Consequence: the p150 Min/Max cells are NOT
  same-source with the p100a records (which also predate compile-fix `4ff5c848`); the p150 2×2 is
  internally valid, but "p100a record CONFIRMED" annotations must be reworded to ratio-level
  reproduction.
- **D6 — `PINNED_COMPILER_SHA256` skew (version-skew trap, P1).** tt-metal `69d61d66` rebooked the
  Reduce-SDPA baseline pair from a **`bb56f1d77`-built** compiler, but `sweep_2x2.conf` still pins
  the pre-recalibration `4633999c` build (and the p150 TSV header claims it file-wide). The next
  scheduled nightly either refuses (sha mismatch) or runs the OLD compiler — whose +1.97% Reduce-SDPA
  regression sits WITHIN the 5% MAX_DRIFT of the new 834 baseline and could be silently blessed
  GREEN. No per-row `compiler_sha` exists in the baseline/scoreboard schema.

**SUSPECT (credible, not locally verifiable — tracked):**

- The recalibration's fresh A/B numbers (855.50 refused / 832.75 re-enabled / 839.00 hand, "BH
  p150") have **no discoverable evidence archive** on the dev box, and the device class silently
  moved p100a→p150 with no recorded cross-device control (the archived-era 834/840 IS corroborated).
- The Typecast **"WP8 step 4 targets it" attribution is a misattribution risk**: step 4
  (`50cad63fa`) landed BEFORE the measuring compiler `4633999c` was built — either the shared
  descriptor fired and the annotation mislabels the residual +22.64%, or it refused and the row
  should record the refusal. No fired/refused dump archived for the real node's ON leg.
- The Typecast **BODY-marker cells lack the issue-slot lower-bound check**: the ON leg is a
  macro-launch shape measured with a BODY-family marker, no math-drain barrier — the metric class
  the §1 caveat declares invalid for fire-and-forget launch shapes.
- Also tracked: chip class is config-asserted, never device-probed; `scoreboard.tsv` lacks a
  `chip_class` column; the minmax-max hand baseline cell was aggregated by MIN while the tool
  aggregates by MEAN; craq-sim's `execute_load_macro_template_direct` (~450-line parallel evaluator
  for the two non-encodable overrides) is an opcode-generic but permanent divergence-risk surface
  pinned only by fuzz.

**Required fixes, ranked** (full list with anchors in `PULL_ANALYSIS-20260817.md` §4):

1. **P0:** wire the all-lanes proof (consume `sfpencc_all_lanes_word()` or CRAQ-prove the
   partial-lane envelope; tests both directions; fix the ownership comment) — before any
   lane-predicated shape reaches silicon.
2. **P1:** fix the `weekly_bh_sweep.sh` FAIL counting; move `knob_silicon()` behind the BH CRAQ
   gate + paired correctness; make win→refusal RED in `report()`; promote `PINNED_COMPILER_SHA256`
   to the `bb56f1d77`-built compiler and add per-row `compiler_sha` to the baseline/scoreboard
   schema — before the next scheduled sweep is trusted.
3. **P2:** reword the `65d2c873` comparability claims to ratio-level; planner-dump the real
   `metal__ckernel_sfpu_typecast` BH node to attribute the +22.64% (fired vs refused); implement
   the issue-slot lower-bound check + drain barrier/KERNEL leg for BODY-marker macro-launch rows;
   add a device chip-class probe + `chip_class` in scoreboard; archive the recalibration A/B
   evidence and commit the WP7 minmax parity manifests + refusal-oracle store.
4. **P3:** re-record the [60,121) no-silicon profitability band + restore rvtt-cost.md's
   non-itemized-dynamic-costs sentence; add `loop_trip_weight` to the carry-forward list;
   cross-validate `sweep_2x2_ops.tsv` against the corpus; this document's own reconciliation
   (done in this revision: §18.9 B0/GREENFIELD supersessions, this section).

#### 18.8.1 Hardware mechanism (simulator semantics; silicon performance authority)

**Path A — MOP + REPLAY share ONE 32-slot circular buffer.** Every RISC-pushed Tensix instruction enters `tensix_push_inst` (`tensix.cpp:2666`), whose opcode switch routes `0x01→MOP`, `0x03→MOP_CFG`, `0x04→REPLAY`, else passthrough — all funneling into the single choke point `replay_expander` (`tensix.cpp:2408`). State is per-pipe (per-TRISC): `replay_buf[TENSIX_INST_PIPES][32]`, `replay_index`, `replay_left`, `replay_execute_while_loading` (`sim.h:502-507`). `replay_expander` has three state-keyed modes:

| Mode | Condition | Behavior | cite |
|---|---|---|---|
| **Capture** | `replay_left>0` | write `replay_buf[index]=inst`; `index++`; `replay_left--`; if `execute_while_loading` also push to exec FIFO same cycle | `tensix.cpp:2411-2424` |
| **Replay-cmd** | `replay_left==0 & op==0x04` | decode `load_mode=bit0`, `exec_while_loading=bit1`, `len=bits<13,4>`, `start_idx=bits<23,14>`; `load_mode=1` arms capture (`replay_index=start_idx; replay_left=len`), `load_mode=0` playback loops `len` pushes of `replay_buf[start_idx+i]` tagged `replay_emit` | `tensix.cpp:2425-2461` |
| **Passthrough** | else | straight to exec FIFO | `tensix.cpp:2463-2470` |

Hard bounds the emitter MUST honor: `1<=len<=32` (`tensix.cpp:2444`), `start_idx<32` (`:2446`), and `start_idx+len<=32` — **overflow is `UndefinedBehavior`** (`:2447`). MOP (`tensix.cpp:2559`) is a hardware loop nest that calls `replay_expander` on its 9 template slots `mop_cfg[pipe][0..8]` (`sim.h:498-499`), so MOP and REPLAY *compose on the same buffer* (a MOP whose loop-op is a REPLAY playback is legal). Expansion is deferred (`defer=true`, `:2699/2712`) against the executable-FIFO watermark of 31 (`tensix.cpp:487-494`, drained one/cycle by `tensix_advance_frontend_stream`, `:2728`). On playback the wait-gate block mask is **recomputed per backend instruction** (`tensix.cpp:2464-2469`), so replayed-body hazards are still enforced individually — the compiler does not re-declare per-body sync.

**Path B — `SFPLOADMACRO` (opcode `0x93`) is a different datapath**, not the 32-slot buffer. It reads a 4-entry `load_macro_instruction_template[4]` / `load_macro_sequence[4]` / `load_macro_misc` (`sim.h:587-589`) populated by `SFPCONFIG` writes (`config_dest 0..3→template, 4..7→sequence, 8→misc`, `tensix.cpp:9740-9757`); each `SFPLOADMACRO` does an implicit `SFPLOAD` into `LReg[VD]` re-dispatched as `0x70` (`tensix.cpp:9911-9927`) then schedules templated work across 4 sub-units. *[SUPERSEDED 2026-08-17: at craq-sim `f80a8d6` every recognizer is deleted and generic descriptor decode with delayed events (retiring at issue+1+Delay) is the ONLY path — there is no signature whitelist and no admitted-shape fallback (§18.8.0.4). The `fd8ed6f`-era description below is retained as lineage.]* ~~The original path functionally whitelists known LLK signatures. CRAQ `fd8ed6f` adds persistent delayed-event queues, transactional same-cycle evaluation, resource/write arbitration, issue-time store snapshots, and pure evaluators for a conservative WH/BH subset. Unsupported and conflicting shapes still fall back; QSR is not claimed.~~

#### 18.8.2 What the GCC backend models/emits, and where — grounded in the shipped `rvtt` backend

**Path A is already SHIPPED, partially.** The builtin `ttreplay` exists (`rvtt-insn.def:239`, `VOID_FTYPE_XTT_IPTR_USI_USI_USI_USI_USI_USI`) and lowers through the expander `rvtt_ttreplay` → insn `rvtt_ttreplay_int` (`rvtt.md:2664-2725`), whose operands are exactly the sim encoding fields: `%3`=insn/imm, `%5`=`len`/start, `%6`=`exec-while-load`, `%7`=`load` — emitting `TT_OP_{WH,BH,QSR}_REPLAY(...)`. The auto-compression pass **ships**: `pass_rvtt_replay` runs `INSERT_PASS_AFTER (pass_postreload, 1, ...)` (`rvtt-passes.def:54`), 834 lines in `rtl-rvtt-replay.cc`. It already implements §6.1's model:

- **Candidate conflict graph / sequence discovery**: O(N²) repeated-subsequence finder over `replay_info{hash, generation, must_end, empty}` (`rtl-rvtt-replay.cc:62-118`), `MIN_SEQUENCE=4` (`:60`), CRC32 insn hashing including `generation` (oldest SI value) so only same-generation synth insns match (`:219-293`).
- **Span placement over 32 slots** as the knapsack it names (`:40-41`), with `REPLAY_{playback,fixed_capture,variable_capture}` (`:123`) mapping directly onto the sim's capture-arm vs playback modes.
- **User-reservation avoidance**: "if the user has explicitly used replay, we use the parts of the replay buffer that have not [been] used anywhere in the function" (`:48-49`) — satisfying §6.1's "contiguous available spans after explicit user reservations" without a global sim registry (there is none — `sim.h:502`).

**The PARTIAL gap (the ~4%) is documented in the pass's own limitations:**
1. **Single-BB scope only** (`:43`): "Only consider single BBs … Looking across BBs would require … the dominator graph and better live-value computation for synthesized insns." Welford's unrolled recurrence body that yields the 3-cycle gap needs cross-BB span discovery.
2. **No slot-reuse when lifetimes are disjoint** (`:46-47`): "If sequence A's occurrences are all before sequence B's, B could reuse the replay buffer locations. We do not consider this." This under-packs the 32 slots → fewer spans compressed.
3. **All-or-nothing spans** (`:50-51`): "We use all of a discovered sequence (or none). We could … use the first N insns." Prevents partial fits.
4. **Non-Tensix insns terminate sequences** (`:53-56`, PR 36496): address/opcode computation mid-sequence is not hoisted, so it splits an otherwise-replayable body.

Closing the gap is **finishing this shipped pass**, not building one: extend `replay_block`/`replay_map` to a dominator-scoped region, add interval-based slot reuse (disjoint-lifetime spans coalesce onto `[0,31]`), and add prefix-span selection. No new builtin needed; the `ttreplay_int` encoding path is complete.

**Interval slot-reuse is net-new — it is NOT reusing a shipped allocator.** The §4 DSATUR/backtracking allocator is *pseudocode in the markdown*; the actual `rtl-rvtt-lp-alloc.cc` is a 133-line **intentionally dump-only stub** (`return TODO_df_finish`, comment "intentionally dump-only," `:57,123`). D0/D1's disjoint-lifetime coalescing must therefore be built fresh (or blocked on the unbuilt M2 allocator), not framed as wiring an existing DSATUR engine.

**MOP compression is GREENFIELD**: there is **no** MOP builtin — `grep ttmop|loadmacro|mop rvtt-insn.def` is empty (verified). For doubly-nested loops, one MOP word (9-slot `mop_cfg` template, up to ~32k expansion) beats N REPLAY playbacks. This needs `new: rvtt_ttmop` builtin + `new: pass_rvtt_mop` (or a mode in `rtl-rvtt-replay.cc`) emitting `MOP`/`MOP_CFG` (`0x01`/`0x03`) with the two MOP types (zmask `:2567`, nested outer/inner `:2594`). Compose-on-same-buffer means the allocator must treat MOP-captured and REPLAY-captured spans as one 32-slot arena.

**`SFPLOADMACRO` compiler emission — *[SUPERSEDED 2026-08-17: emission is now LANDED as the generic 7-layer macro planner at gcc `bb56f1d77`, and the sim executes descriptors generically at `f80a8d6`; see §18.8.0.4. The paragraph below is the pre-planner design context.]* —** was unlanded; its simulator foundation was partial. `SFPCONFIG` ships (`UNSPECV_SFPCONFIG`, `rvtt.md:89,1945-1971`), and CRAQ `fd8ed6f` executes a conservative set of structurally validated WH/BH macro events. The compiler still needs a target-internal launch/config descriptor that owns and materializes every template/sequence/misc field, models hidden LREG/LREG16/CC/Dst effects, rejects opaque owners, and preserves byte-identical fallback. Extend CRAQ and the compiler together for each additional admitted shape; do not infer arbitrary template safety from opcode names.

#### 18.8.3 Staged milestones

- **D0 (PARTIAL→ship gap fix):** Extend `pass_rvtt_replay` slot allocation to reuse disjoint-lifetime spans (limitation #2) and add prefix-span selection (#3). Single-BB only. The interval-reuse logic is net-new (the M2 allocator is a dump-only stub). Gate: no correctness regression on the 5 Welford selectors.
- **D1:** Cross-BB / dominator-scoped sequence discovery with per-generation live-value guarding (removes limitation #1). This is the milestone that targets the 3-cycle Welford body.
- **D2:** Sequence-through-non-Tensix hoisting (limitation #4 / PR 36496) to stop spurious sequence termination.
- **D3 (MOP, greenfield):** `new: rvtt_ttmop` + emit `MOP_CFG`/`MOP`; teach the allocator MOP∪REPLAY share the 32-slot arena; select MOP over REPLAY for nested loops by word-count cost.
- **D4 (`SFPLOADMACRO`) — LANDED 2026-08-17 (§18.8.0.4):** ~~use CRAQ `fd8ed6f` as the admitted-shape functional gate; land a compiler-internal launch/config descriptor with byte-identical fallback; then expand pure event evaluators and compiler legality shape by shape~~ — the generic 7-layer planner (capability tables → descriptor synthesis → emission-gating verifier, byte-identical refusals) is landed at gcc `bb56f1d77`, and craq-sim `f80a8d6` decodes any programmed descriptor generically (the admitted-shape sim model is gone). Remaining D4 work: attribute/close the residual Typecast loss (fired-vs-refused, §18.8.0.5), the Where CC-template extension, and MulInt silicon — each still per-shape correctness + silicon gated (the §7 Typecast/MulInt/Where row).

#### 18.8.4 Hard gate (measurable)

**D0 gate (shipped/measurable now):** auto-replay compression on the 8-row unrolled mockup holds **88→19 static Tensix insns on WH and 56→15 on BH** (§6.1 *Mockup Evidence*), and `WELFORD_BODY` on Blackhole silicon holds the pinned **323/323/323 device cycles** for N=1/2/32 vs replay-LLK's 326 (§7 Welford row, §13.3) — with all 5 Welford selectors passing correctness. Any `replay_buf` `start_idx+len>32` at emit is a hard fail (`UndefinedBehavior`, `tensix.cpp:2447`); the pass must prove `S+L<=32` as an allocation invariant.
**D1 loop-hoist gate (cleared on Blackhole):** fixed-encoding replay capture hoisting changes
Reduce-SDPA from handwritten `840` versus generated `855.5` to handwritten `840` versus generated
`834` scoped device cycles, with paired correctness and three zero-spread processes per arm.  The
implementation remains opt-in and conservatively single-block-loop only.  Broader cross-BB span
discovery for Welford is still open and must independently satisfy the same changed-binary silicon
gate; the Reduce result does not imply that unimplemented transform exists.
**D4 gate (per-shape; restated 2026-08-17 for the generic sim):** each `SFPLOADMACRO`-lowered Typecast/MulInt/Where shape must first execute bit-exactly under craq-sim `f80a8d6`'s generic descriptor decode against an independent explicit decomposition (the `fd8ed6f`-era "admitted by the transactional model without fallback" whitelist framing is obsolete — the sim now decodes any programmed descriptor; compiler-side refusals must remain byte-identical), then pass paired hardware correctness and repeated silicon A/B. The prior **≥1.33×** figure is an unmeasured opportunity target, not an acceptance result; CRAQ modeled cycles are not the performance authority.

#### 18.8.5 Risks / ceiling

- **Emit-ordering invariant (correctness-critical):** an arm (`load_mode=1`) must be followed *immediately in program order* by exactly `len` pushes before any playback — `replay_expander` in capture mode swallows everything until `replay_left==0` (`tensix.cpp:2411-2424`). The post-reload pass must never let another REPLAY or a scheduler move interleave between arm and body. Running `pass_rvtt_replay` after `pass_postreload` (`:54`) is correct precisely because no reordering pass follows.
- **32-slot arena is a hard allocation constraint** shared across ALL live captured bodies per pipe (and, at D3, across MOP too). Over-allocation is silent `UndefinedBehavior` in the sim, not a diagnostic — the allocator carries the entire correctness burden, and that allocator is net-new (the shipped `rtl-rvtt-lp-alloc.cc` is a dump-only stub).
- **User-reservation contract:** LLK hand-kernels reserve slots and there is **no global sim registry** (`sim.h:502`); reservations must be a compiler-known descriptor (the §7 "explicit … ownership metadata … no global reservation" model — Welford row), mirrored on replay slots. Discovery-based reservation (the shipped "not used anywhere in the function" heuristic, `:48`) is sound only within a compilation unit — cross-TU LLK reservations need the metadata ABI.
- **Cross-BB live values (D1):** the pass author's own note (`:44-45`) flags that cross-BB replay needs better live-value computation for synthesized insns; getting generation-tracking wrong replays a stale-input body → silent numeric error, not a crash.
- **`SFPLOADMACRO` ceiling — *[risk retired 2026-08-17]*:** ~~CRAQ `fd8ed6f` removes the blanket pattern-matcher blocker only for its admitted WH/BH shapes. Novel or conflicting templates remain unvalidated until both a pure transactional evaluator and matching compiler legality proof exist.~~ At `f80a8d6` the recognizers are deleted and an adversarial never-whitelisted descriptor executes bit-exactly through the generic decode (§18.8.0.5); the residual fidelity surface is the ~450-line `execute_load_macro_template_direct` parallel evaluator for the two non-encodable overrides (opcode-generic, fuzz-pinned — tracked in §18.8.0.5). Track D remains a single-stream problem, unlike Track C.

### 18.9 Track B — DST Tile Register + RWC Hazard Model (executable design)

> **B0 reconciliation (updated 2026-08-17 post pull-analysis) — supersedes the GREENFIELD/"enabler absent" status in the §18.9 status line and §18.9.2/§18.9.6 below.** The B0 prerequisite is **SATISFIED and fully landed on `nkapre/sfpi`**: the typed `ttdstface`/`ttsetrwc` builtins (the `85151036f`/`30d3c6207` lineage, carried into mainline by the merged planner chain) are in the pinned gcc **`bb56f1d77`** (the `0x37120004` magic word was deleted; typed `rvtt_ttdstface` assembles byte-identically to `0xdc480010`); the wrapper commit `ded6e4e9` (`dst_face_advance` / `setrwc<>`) is merged on sfpi; the LLK migration is landed on tt-metal `nkapre/sfpi` (`69d61d66`). **B1–B5 remain OPEN** — `pass_rvtt_dst_ownership` does not exist and no Track B silicon exists; this is the **paused Track B lane** (resume only on user go-ahead). Note: an earlier revision of this block cited the stale pin `ddf44ed64`; the pin has since advanced `ddf44ed64 → cd0af49be → bb56f1d77`.

**Status (2026-08-17): B0 SATISFIED (see the reconciliation block above); the pass (B1–B5) and silicon remain OPEN; lane PAUSED.** *The original status line — "GREENFIELD (enabler is branch-only/absent; no pass; no silicon)" — and this paragraph's availability claims are retained below SUPERSEDED: they were verified against the old pin `8f943c2f84a` and are FALSE at the current pin `bb56f1d77`, where the builtins, wrappers, and LLK migration are all landed.* The strategy is *extend GCC, defer MLIR*: model the DST accumulator and its read-write-clear (RWC) address counters as first-class compiler-visible resources so the hand-written `Dst` round-trips in the log/GELU/erfinv transcendentals (§7) fold into resident LRegs instead of surviving as opaque `.ttinsn` barriers. The mechanism is fully specified by craq-sim; the compiler side is a mirror of the shipped raw-LREG-livein solution. What blocks a first line of code is that the two typed boundary builtins this design depends on — `__builtin_rvtt_ttdstface` and `__builtin_rvtt_ttsetrwc` — **do not exist in the pinned gcc tree**, and even their thin header wrappers are not resident here: the `setrwc<>` wrapper (`lltt.h`) exists only on an unmerged branch commit (`c010af4a28`, "sfpi: add typed SETRWC boundary wrapper") that is **not reachable from HEAD** (`63516cc`) — at HEAD `include/lltt.h` is 41 lines and lines 35-42 are `replay_insn`, with no `setrwc`; and the `dst_face_advance` wrapper cited to `sfpi.h` at commit `ded6e4e9dc` has **no on-disk source at all** — that commit is not reachable/materialized in the working tree, and `include/sfpi.h:671-679` at HEAD is unrelated (`l_reg`/`dst_reg`/SrcS aliases). The compiler commit the branch cites as the required backend, sfpi-gcc `30d3c6207`, is *not a valid object* in the pinned submodule (`git cat-file -t 30d3c6207` fails; submodule HEAD is `8f943c2f84a`). A `grep` for `ttsetrwc|ttdstface` across `gcc/` and `include/` returns only unrelated `TT_OP_*_SETRWC` assembler macros — no `RVTT_FN`, no expander, no insn. So Track B is greenfield on both halves: the enabler builtins must be landed in the backend first, then the ownership pass built on top.

**Re-scoping note vs §18.4.** The master roadmap lists Track B as "Partial", crediting the shipped `_lv` live-value forwarding, the enforcing `pass_rvtt_lreg_livein`, and the `rvtt.gc` combiner. That machinery is real and it deletes *same-scope, same-layout* LReg round-trips — but it is Track A's SFPU-value plumbing, not a DST/RWC model. It cannot fold the §7 spills, because those cross the moving `dst_rwc` base and the CFG-state layout mode, and the loads reach the compiler as opaque `.ttinsn` words with no def/use edge for the Dst rows or RWC counters they touch. The honest status for the DST/RWC *hazard model* — the thing this subsection specs — is greenfield: the typed boundary that would make those effects visible is branch-only/absent, and no ownership pass, no post-IRA verifier, and no silicon result exist yet. This design does **not** depend on the M2 physical allocator (a dump-only stub, §18.8) or on any MLIR reconsideration; it is entirely a GCC-backend extension.

#### 18.9.1 Hardware mechanism (craq-sim ground truth)

DST is not flat storage — it is a physically-banked 16-bit tile register file with a parallel valid bitmap, addressed through a *moving* per-pipe counter, in a layout chosen by *global CFG state*. There is no per-tile object: a "Dst face" is an 8-row-aligned window into one flat SoA array (matrix ops require `DstRow%8==0`, `tensix.cpp:4940,4993`). Five facts the compiler must model:

1. **Paired-row 32-bit layout.** Backing store is `uint16_t dst[1024][16]` + `bool dst_row_valid[1024]` (`sim.h:546-547`; `DST_ROWS=1024 ROW_SIZE=16` at `sim.h:217-219`). In 16b mode one datum is one cell, tracked at `dst[row]` (`write_dst16b`/`read_dst16b`, `tensix.cpp:3523-3545`). In 32b mode one FP32/INT32 datum is split hi/lo across two cells 8 rows apart — `hi=dst[adj]`, `lo=dst[adj+8]` (`write_dst32b`/`read_dst32b`, `tensix.cpp:3504-3520`) — and **only the HI row's `dst_row_valid[adj]` is the liveness bit**; lo is implied. Rows are permuted by `dst32b_adjust_row` (`tensix.cpp:3491-3501`): the base `((row&0x1F8)<<1)|(row&0x207)` shuffle applies on all targets, plus a **Blackhole-only (`TT_VERSION==1`)** extra `dst_remap_row` + `DEST_ACCESS_CFG_swizzle_32b` XOR-permute; on WH (`TT_VERSION==0`) that remap is `(void)p_tensix` and only the `0x1F8/0x207` shuffle applies. A "Dst row index" is **not** a linear address, and in 32b mode owning face `f` means owning rows `{adj(f), adj(f)+8}`.

2. **Layout is CFG-state, not value-carried.** `use_dst32b = ALU_ACC_CTRL_Fp32_enabled || ALU_ACC_CTRL_INT8_math_enabled || dst_32bit_addr_en` (`tensix.cpp:3822-3823, 3994, 4983`). The first two are per-cfg-state config-register fields read via `p_config`; `dst_32bit_addr_en` is a global bool poked by the RISC debug bus (`DBG_FEATURE_DISABLE` bit 11, `tile.cpp:1556`). SFPU has an extra wrinkle: with Fp32 dest-acc on, BF16 intermediates still live in the HI half of the 32b layout even when the debug bit is clear (`sfpu_dst32_layout = dst_32bit_addr_en || ALU_ACC_CTRL_Fp32_enabled`, `tensix.cpp:8461-8463`). A store and its reload must agree on layout or the bits differ.

3. **RWC = the moving base + hazard token.** Per-pipe `dst_rwc[pipe]`/`dst_rwc_cr[pipe]` (`sim.h:569-570`) index DST; `_cr` is the checkpoint base that CR-mode ops restore to. The effective row is *symbolic*: `math_dst_row_base` (`tensix.cpp:3390-3391`) returns `dst_rwc[pipe] + DEST_TARGET_REG_CFG_MATH_Offset + math_dest_regw_base(p_config)`, and the `<dst field>` is added by the **caller** (`4988, 8437-8441`), not inside the function. The RWC is added at **every** access, so program order + counter state — not a static register number — decides which physical rows alias. Counter transitions:
   - **ADDR_MOD per-instruction advance.** Every matrix op ends with `math_update_counters` (`tensix.cpp:5072`) reading `ADDR_MOD_DST_SECi {DestIncr,DestCR,DestClear,DestCToCR}` (`tensix.cpp:3329-3332`) → `math_update_rwc` (`tensix.cpp:3236-3249`): plain `DestIncr=8` walks to the next face; `DestClear` resets `rwc=cr=0` (the RWC **boundary**); `cr` walks from base; `c_to_cr` advances then checkpoints. This is the "Dst += 8 per face" stepping — it is *not* in `SETRWC`.
   - **SETRWC (`0x37`) / INCRWC (`0x38`).** `SETRWC` sets counters from immediate fields under `bit_mask`/`rwc_cr` (dst leg `tensix.cpp:5394-5401`; `rwc_d` multiple-of-4, ≤12); it also fires `math_clear_src_valid` (`:5424`), so it doubles as a dvalid-clear boundary. `INCRWC` bumps (`:5443-5448`). ZEROACC (`tensix.cpp:3976-4044`) clears/sets the valid bitmap over 1/16/face rows and is likewise a boundary.

4. **Write→read enforced by the valid bitmap.** `read_dst32b`/`read_dst16b` return 0 (0xFFFF for gmpool) when `!dst_row_valid[adj]` (`tensix.cpp:3506-3508, 3528-3530`); matrix/MOV/SFPU writes set it (optionally only on `col==15`, `3517-3519, 3543`). The packer reads Dst through the same `read_dst*` (`tensix.cpp:6204,6253`), so it observes the bitmap. **Matrix accumulation is itself a per-face Dst read-modify-write** (`read_dst32b → mvmul_row16 → write_dst32b`, `tensix.cpp:5039-5070`) — the accumulator reads its own prior face value.

5. **Cross-engine hazard gating.** Two implicit data hazards sit under the explicit `STALLWAIT`/`SEMWAIT` waits (which map math→bit6, PACR→bit2, UNPACR→bit3, `tensix.cpp:1723-1777`): (a) the SrcA/SrcB dvalid handshake — an `MVMUL` stalls until both src valids are set by the unpacker, then retires them through a one-cycle pipeline and flips `src_a_matrix_bank` (`tensix.cpp:4951-4966, 3395-3437`); (b) the `dst_row_valid` bitmap gating matrix/SFPU→pack.

**Why the round-trips exist (sfpi §7).** SFPU vFloat values live in 8 architectural LRegs; transcendentals that exceed LReg pressure SFPSTORE an intermediate to a Dst row and SFPLOAD it back. The concrete sites this track targets: log reloads the original input at the zero/inf/nan special case (`v_if(in == 0.0F) { // Reload for register pressure`, `ckernel_sfpu_log.h:53/54`, peak pressure 9, §7 `:880`); erfinv reloads `in` from `sfpi::dst_reg[0]` to reattach the sign after nested inlined log + two `sqrt_custom` (`ckernel_sfpu_erfinv.h:54`, §7 `:881`); GELU reloads `x` from `dst_reg[0]` after the accurate FP32 tanh to finish `0.5*x*(1+tanh(...))` (`ckernel_sfpu_gelu.h:373`). Physically a store-then-load of the same row with no intervening writer is identity — but because the row is chosen through the moving RWC base and the format depends on CFG state, the compiler cannot fold it without modeling both. (Era note: on the original dev box the working tt-metal checkout carried the *eliminated* log form while the CI checkout carried the old hand-spilled bodies — that diff is the target; reproduce it from any tt-metal `nkapre/sfpi` checkout against a mainline checkout.)

#### 18.9.2 The enabler — typed `ttdstface` / `ttsetrwc` boundaries, and why they mirror raw-LREG-livein

> *[SUPERSEDED 2026-08-17: the "branch-only/absent"/"not a valid object" availability claims in this
> subsection were true against the old pin `8f943c2f84a` and are FALSE at pin `bb56f1d77` — the
> builtins (`85151036f`/`30d3c6207` lineage), the sfpi wrappers (`ded6e4e9`), and the tt-metal LLK
> migration are all landed on `nkapre/sfpi`. The **contract description** below remains the accurate
> design reference. See the B0 block at the top of §18.9.]*

The problem is exactly the one raw-LREG-livein already solved for L-registers, one level up. Today a raw `.ttinsn` L-register access is emitted as `rvtt_sfprawlreg_access`, a length-0 `UNSPEC_VOLATILE` whose only operands are `release_mask`/`write_mask` const_ints (`rvtt.md:222-236`); it has **no** RTL def/use of any SFPU pseudo, so IRA sees the architectural LREGs it touches as free (`rtl-rvtt-lreg-livein.cc:40-45`). `pass_rvtt_lreg_livein` makes those opaque lifetimes visible *without inventing real dataflow*, by pinning single-hard-register sentinel pseudos (`rvtt_sfpreadlreg<N>`/`rvtt_sfpwritelreg<N>`, `=x<N>` bound to single-register class `SFPU_REGS_L<N>`, `rvtt.md:199-220`, `rvtt-constraints.md:28-49`) across each interval, running a forward union-join dataflow over an 8-bit mask, and closing joins with a fresh local token per BB rather than a cross-CFG phi.

DST/RWC is the mirror image, and it is *harder* because SFPLOAD/SFPSTORE (`rvtt-insn.def:157-171`) and TTINCRWC (`rvtt-insn.def:246`) read/write the Dst accumulator and the RWC counters but the RTL exposes none of it — loads are just `VOL` `UNSPEC_VOLATILE` producing an LREG, and "loads can change rwc depending on unobserved state" (`rvtt-insn.def:156`). The `SFPLOADMACRO` formation pass can only *reject* such a region today with `dst-rwc-effect-unproved` (`rtl-rvtt-loadmacro.cc:49,75-76,177`; contract `SFPLOADMACRO_FORMATION.md:56,80-83`) precisely because there are no def/use edges for Dst rows or RWC counters.

The two typed builtins are the missing edge — **but neither is resident in the pinned tree; both are branch-only or absent (see §18.9 status), so the citations below describe the intended contract, not on-disk source.** They carry the architectural fields as first-class `const_int` operands in a recognizable insn:

- **`__builtin_rvtt_ttdstface()`** (nullary; intended wrapper `sfpi::dst_face_advance()`). Its intended contract: "one face is two architectural CR-mode Dst += 8 counter steps with **no LREG, CC, or configuration effect**." That is exactly the typed fact an ownership pass needs to advance its Dst-face state by one face while treating LREG liveness as transparent through it. (No on-disk source exists here: the `ded6e4e9dc` commit is not materialized in the working tree and `sfpi.h:671-679` at HEAD is unrelated aliases.)
- **`template<unsigned Clear,Cr,D,B,A,Set> __builtin_rvtt_ttsetrwc(...)`** (intended wrapper `lltt::setrwc()`) — all six architectural SETRWC fields as immediate-only template args, letting the pass update RWC ownership exactly from operands. (The wrapper lives only on unmerged commit `c010af4a28`, not reachable from HEAD; at HEAD `lltt.h:35-42` is `replay_insn`.)

By analogy to the nearest existing RWC builtin `ttincrwc` (`rvtt-insn.def:250` `RVTT_FN(ttincrwc,,,VOID_FTYPE_SI_SI_SI_SI,VOL,...)`; insn `rvtt_ttincrwc` at `rvtt.md:2898-2907`, a volatile unspec of const_int fields emitting `"TTINCRWC %0,%1,%2,%3"` with `(set_attr "xtt_replay" "barrier")`), the two new builtins should each become **a `define_expand` doing per-target opcode selection** (the `TT_OP_{WH,BH,QSR}_*` ladder switched on `TARGET_XTT_TENSIX_{WH,BH,QSR}`, the same shape as `rvtt_ttreplay` at `rvtt.md:2910-2948`) feeding **a `define_insn` whose RTL is a volatile unspec with typed const_int operands and an `xtt_replay` owner/barrier attribute**. This inverts the opaque-region protocol: instead of decoding a 32-bit `.ttinsn` word (`rvtt.md:649`, `rvtt_sfploaddiscard_int` emitting `.ttinsn %0`) and separately trusting an author-supplied `sfprawlreg_access` mask, the ownership/liveness passes read the Dst-counter and RWC effect directly off the pattern. `dst_face_advance`'s "no LREG, CC, or configuration effect" is precisely the typed contract those analyses consume.

#### 18.9.3 The proposed pass — `new: pass_rvtt_dst_ownership` (GREENFIELD; does not exist)

A new pre-IRA `rtl_opt_pass`, `pass_rvtt_dst_ownership` in `new: rtl-rvtt-dst-ownership.cc`, modeled almost 1:1 on `rtl-rvtt-lreg-livein.cc`, gated on `TARGET_XTT_TENSIX`, returning `TODO_df_finish`, and wired `INSERT_PASS_BEFORE (pass_ira, 1, ...)` in `rvtt-passes.def` alongside the existing lreg-livein entry (`rvtt-passes.def:50`).

**Reused nearly verbatim from the template:**

- **A decode helper** mirroring `raw_access_p`/`read_lregno` (`lreg-livein.cc:47-93`) that classifies each SFPLOAD/SFPSTORE/`ttsetrwc`/`ttdstface`/`ttincrwc` into `{Dst face/bank touched, RWC counter deltas, release/write intent}`. Because the typed builtins expose their fields as `const_int` operands, this decodes the pattern — no opaque-word parse.
- **Single-hard-register-class sentinel pseudos** (mirror of `make_sentinel`/`emit_sentinel_*`/`end_sentinel`, `lreg-livein.cc:95-129`), one modeled resource per Dst face/bank and per RWC counter (`dst_rwc`/`dst_rwc_cr`), with length-0 `read/write` pseudo-insns analogous to `rvtt_sfp{read,write}lreg<N>` so IRA reserves the resource across an interval and emits nothing.
- **The exact transfer function** `live = (live & ~releases) | writes` (`lreg-livein.cc:158`) and the forward union-join fixpoint over `in[]`/`out[]` (`lreg-livein.cc:172-201`).
- **The CFG-join-token idiom:** a fresh local pseudo minted at each live-in BB head, closed by a `USE` at the tail (`lreg-livein.cc:222-228, 271-276`), so no cross-CFG pseudo or phi is invented for a resource that has no representable SSA value — perfect for Dst/RWC, which likewise have none.

**What it tracks:** (i) **live Dst faces** — a face is LIVE (readable/packable) only after a writer set `dst_row_valid[dst32b_adjust_row(face)]` on its HI rows and no intervening ZEROACC-clear; conservatively from the first writer to the next ZEROACC/reuse. (ii) **RWC boundaries** — the points where the affine `dst_rwc` mapping resets or jumps and the face-identity chain must be split: any op whose ADDR_MOD carries `DestClear`/`DestCR`/`DestCToCR`, any `SETRWC`/`INCRWC` touching the dst leg, and any ZEROACC (`tensix.cpp:3242-3245, 5394-5401, 5443-5448`). Between two boundaries the target face is a single SSA-like def/use chain. (iii) **layout tag** per face — `fp32`/`bf16`/`int8` from the reaching `ALU_ACC_CTRL_*`/`dst_32bit_addr_en` write — so a 32b face's two aliasing rows are never treated as independent, and a store→reload is only foldable when its layout is provably unchanged.

**Where it diverges from the template (Dst is an accumulator with counters, not an 8-entry file):**

1. **State is not a flat 8-bit membership mask.** The abstract state is a tuple `(face/bank ID + row + format + RWC counter values)`, and RWC counters are *monotonic-increment* (TTINCRWC/ADDR_MOD), not kill/def. The transfer function must carry per-counter interval/affine state — or, conservatively, a single "Dst region owned" token plus a proof that the RWC deltas net to the launch address (invariant 7, `SFPLOADMACRO_FORMATION.md:80-82`).
2. **Join is may-alias / conservative-conflict.** Reserving an extra LREG is always safe, so the template's union is sound; over-reserving Dst is also safe, but the counter identity that keeps a row/format invariant is a *stronger* fact a lossy join destroys. A join of two different counter states must therefore **fall to `dst-rwc-effect-unproved`** (feeding the existing loadmacro reject vocabulary, `rtl-rvtt-loadmacro.cc:49,177`), not pick one.
3. **Region-scoped bank exclusion, not per-insn coloring.** Invariant 8 (`SFPLOADMACRO_FORMATION.md:83`) requires excluding concurrent clients of a Dst/SrcS bank for the whole region — a region-scoped ownership lock, which lreg-livein never needs.
4. **Live-out closes at a dominator-computed drain**, not `BB_END`. lreg-livein closes at the block tail (`end_sentinel_at_block_end`, `lreg-livein.cc:136-147`); Dst ownership must close at a post-dominating drain point (invariants 6–7, `SFPLOADMACRO_FORMATION.md:58,78`).
5. **The sentinels are memory-clobbering `UNSPEC_VOLATILE`**, not pure register defs/uses: loads' RWC effect depends on unobserved state and macro stores inherit the launch address (`rvtt-insn.def:156`, `SFPLOADMACRO_FORMATION.md:81-82`), so the Dst/RWC sentinels double as alias/order barriers for the tile memory.

**Consumers.** The ownership token + counter-invariant fact is the live-in/live-out edge the `SFPLOADMACRO` descriptor demands (`SFPLOADMACRO_FORMATION.md:55-62,80-83`), promoting the standing `dst-rwc-effect-unproved` reject to a proof; and it is the true/anti/output dependency Track C's Haifa scheduler needs once all engines are emitted in one stream (§18.10). The fold itself — cancelling a proven-identity store/reload — is applied once the interval carries no boundary and a stable layout tag between the store and reload.

#### 18.9.4 Staged milestones

| # | Milestone | Deliverable | Verify against |
|---|-----------|-------------|----------------|
| B0 | **SATISFIED (2026-08-17)** — enabler builtins landed | **DONE:** typed `ttdstface`/`ttsetrwc` builtins in pinned gcc `bb56f1d77` (`85151036f`/`30d3c6207` lineage; the `0x37120004` magic word is deleted and typed `rvtt_ttdstface` assembles byte-identically to `0xdc480010`); wrappers merged on sfpi (`ded6e4e9`); LLK migration on tt-metal `nkapre/sfpi`. *(Historical deliverable text: add `RVTT_FN(ttsetrwc/ttdstface)` to `rvtt-insn.def`; `define_expand`+`define_insn` shaped on `rvtt_ttincrwc`/`rvtt_ttreplay`; merge the branch wrappers.)* | header calls compile; WH/BH emit the mnemonics; QSR arm refuses — all verified in the landed state (8/8 minmax CRAQ with the typed trio byte-identical to the frozen raw-word oracle) |
| B1 | Baseline capture | Count surviving SFPSTORE/SFPLOAD Dst round-trips + device cycles in log/GELU/erfinv (`ckernel_sfpu_{log,erfinv,gelu}.h`) on today's backend | craq-sim device-cycle + static insn count |
| B2 | Decode + sentinel skeleton | `new: rtl-rvtt-dst-ownership.cc`: decode helper for SFPLOAD/SFPSTORE/`ttsetrwc`/`ttdstface`/`ttincrwc`; single-register-class Dst-face/RWC sentinel pseudos; forward union-join fixpoint (transfer `live=(live&~rel)\|wr`) | reserves the modeled resource across an interval; identical instruction stream (length-0 sentinels emit nothing) |
| B3 | Face-liveness + RWC-boundary + layout tag | Track live Dst faces via `dst_row_valid`(HI-row) with ZEROACC as death; split the chain at every ADDR_MOD-clr/cr/c_to_cr, SETRWC/INCRWC dst leg, ZEROACC; attach `fp32/bf16/int8` layout tag from reaching `ALU_ACC_CTRL_*`/`dst_32bit_addr_en` | boundaries land at the sim's exact reset/jump points; 32b face owns `{adj,adj+8}` on the permuted address; lossy join → `dst-rwc-effect-unproved` |
| B4 | Round-trip elimination | Cancel proven-identity store/reload pairs in log/GELU/erfinv (no boundary, stable layout, no ZEROACC between store and reload); retain matrix-write→pack-read valid edge | numeric identity on craq-sim; the `:53/:54/:373` author reloads disappear; round-trip count → 0 where legal |
| B5 | Silicon non-inferiority A/B (no silicon exists yet for this track) | Paired off/on device runs (flag as only variable, §2.1) on log/GELU/erfinv Blackhole binaries | measured device cycles ≤ handwritten Dst-spilling baseline; LLK numeric suites green |

#### 18.9.5 Hard gate (measurable)

Grounded in the §18.4 Track B gate row ("Kernels that spill to Dst by hand keep values resident; correctness + non-inferiority"). For log, GELU, and erfinv:

1. **Round-trips eliminated.** The compiler-produced code contains **no author reload of the input from `dst_reg`** — the `ckernel_sfpu_log.h:53/54`, `ckernel_sfpu_erfinv.h:54`, and `ckernel_sfpu_gelu.h:373` reloads are replaced by LReg-resident values. Static SFPSTORE/SFPLOAD Dst round-trip count strictly below B1, and zero for the provably-identity cases.
2. **Correctness (bit-exact).** Bit-exact numeric output vs. the pre-optimization build across the tile, and the LLK numeric-correctness suites stay green (parity vs handwritten; cf. the §7 Welford 15/15 five-selector precedent).
3. **Ordering preserved.** No regression on the retained matrix-write→pack-read `dst_row_valid` edge — no read of a stale / `!dst_row_valid` row.
4. **Silicon non-inferiority.** Measured Blackhole device cycles **no worse than** the handwritten Dst-spilling baseline, run as a paired off/on flag-as-only-variable measurement (the §7/§13 WELFORD_BODY 323-vs-326 discipline).

A fold that changes any output bit, violates a valid-bitmap edge, or crosses an unmodeled RWC/layout boundary fails the gate. craq-sim is the functional/correctness authority; silicon is the performance authority (§18.7 calibration failure — CRAQ cycle deltas are not admitted as a perf oracle). B4 clears gate parts 1–3; B5 clears part 4.

#### 18.9.6 Honest status and risks

- **[SUPERSEDED 2026-08-17 — B0 is SATISFIED at pin `bb56f1d77` (builtins, wrappers `ded6e4e9`, and LLK migration all landed); only the pass (B1–B5) and silicon remain greenfield, and the lane is PAUSED. The bullet below is the historical 2026-08-16 finding against pin `8f943c2f84a`.]** ~~GREENFIELD on both halves — no code exists yet.~~ The enabler builtins are wrapped only on an unmerged branch / not materialized at all, over compiler builtins that are **absent from the pinned gcc tree**: `grep ttsetrwc|ttdstface` over `gcc/`+`include/` finds only `TT_OP_*_SETRWC` assembler macros, no `RVTT_FN`/expander/insn. The `setrwc<>` wrapper is on unmerged commit `c010af4a28`, **not reachable from HEAD** (`63516cc`); at HEAD `lltt.h:35-42` is `replay_insn`, no `setrwc`. The `dst_face_advance` wrapper cited to commit `ded6e4e9dc`/`sfpi.h:671-679` has **no on-disk source in the working tree** (that content is not materialized here; `sfpi.h:671-679` at HEAD is `l_reg`/`dst_reg`/SrcS aliases). The cited backend commit sfpi-gcc `30d3c6207` is *not a valid object* (submodule HEAD `8f943c2f84a`). Code that includes those wrappers and calls `dst_face_advance()`/`setrwc<>()` **cannot be built here today** ("unknown builtin"/"cannot convert", or the header itself is missing). `pass_rvtt_dst_ownership` does not exist, and there is no silicon number for this track. B0 (materialize/merge the wrappers, land the builtins, re-pin sfpi-gcc) is a hard prerequisite for everything after it. This design deliberately does **not** depend on the M2 physical allocator (a dump-only stub, §18.8) or on any MLIR path.
- **QSR refuses at expansion (by design, must be enforced).** QSR's SETRWC has a *different field shape*: `TT_OP_QSR_SETRWC(clear_ab_vld,rwc_cr,rwc_val,BitMask)` fuses D/B/A into one `rwc_val` (`sfpu-ops-qsr.h:205`), whereas WH/BH keep the six separate fields the typed wrapper exposes (`sfpu-ops-wh.h:211`, `sfpu-ops-bh.h:224`). The six-field typed boundary therefore has **no faithful QSR encoding**. craq-sim itself reinforces this: its QSR SETRWC path (`TT_VERSION>1`, `tensix.cpp:5404-5418`) is `TTSIM_ERROR(UntestedFunctionality)` and only accepts `rwc_cr ∈ {0,4}`. So the per-target expander must drop the QSR arm (`: (gcc_unreachable(),0)`, the mechanism at `rvtt.md:487,564,639,696,817,1093`) or hard-`error_at` in the early gimple check (precedent: the QSR replay-erratum diagnostic `gimple-rvtt-check.cc:255-261`) rather than silently mis-encode. Ownership is modeled precisely on WH/BH and hard-rejected on QSR — never wrong.
- **fp32/int8 Dst layout is CFG-state, a reaching-definition problem.** `ALU_ACC_CTRL_Fp32_enabled`/`ALU_ACC_CTRL_INT8_math_enabled` are cfg-register fields and `dst_32bit_addr_en` is a debug-bus global set far from the SFPU op (`tensix.cpp:3822-3823`, `tile.cpp:1556`). If the layout tag is imprecise the fold must **default-deny** (safe, leaves the round-trip). The FP32-acc BF16-in-HI-half special case (`sfpu_dst32_layout = dst_32bit_addr_en || ALU_ACC_CTRL_Fp32_enabled`, `tensix.cpp:8461-8463`) means an SFPLOAD's layout is not always readable off the opcode alone — the reaching-definition analysis, not the mnemonic, decides.
- **RWC base is mutated as an ADDR_MOD side effect**, not an explicit operand (`math_update_rwc` at `tensix.cpp:3356`). Missing one mutator (SETRWC/INCRWC/cr/c_to_cr/ZEROACC) between a store and reload folds an aliasing pair and silently corrupts data. This is the primary correctness risk: the pass must treat *any* unmodeled RWC-touching op as a barrier, and a lossy CFG join must fall to `dst-rwc-effect-unproved` rather than merge.
- **Permuted paired-row addressing** (`dst32b_adjust_row`, `tensix.cpp:3491-3501`) means the symbolic row index and the physical `adj_row` differ — the base `0x1F8/0x207` shuffle on all targets, plus a **Blackhole-only** `dst_remap_row`/`DEST_ACCESS_CFG_swizzle_32b` XOR-permute (a no-op on WH). Disjointness must be proven on the *permuted* address, or two "different" indices may alias the same bank. In 32b mode a face owns `{adj(f), adj(f)+8}` and the two rows must never be treated as independent.
- **Cross-engine dvalid subtlety.** An `MVMUL` both consumes the current Src face and read-modify-writes its own Dst face (`tensix.cpp:5039-5070`); an explicit author reload of a face an accumulating `MVMUL` already round-trips is redundant, but proving that requires the matrix-bank/dvalid token model (`src_a_matrix_bank` flip, `tensix.cpp:3395-3437`) as well as the Dst model — getting the generation/bank parity wrong replays a stale face and yields a numeric error, not a crash.

### 18.10 Track C — Cross-Engine (matrix + pack/unpack + 3-TRISC) Scheduling

**Status: GREENFIELD.** Nothing in the shipped `sfpi` backend models cross-engine timing. Generated
Tensix commands live in `tt/rvtt.md`; the 87 `ttrocc.md` patterns are a separate QSR RISC-V RoCC
interface and must not be classified using Tensix FIFO opcode ranges. The existing rvtt passes
(`tt/rvtt-passes.def`: `pass_rvtt_check_early/immvar_expand/synth_split/noval_elide/synth_cse/dce/...`)
are all single-stream SFPU-lowering passes with no notion of engines or TRISC threads, while most
handwritten matrix/unpack/pack commands enter as opaque `.ttinsn` assembly. This track is the one
that tests whether GCC's IR can carry tile/dataflow scheduling at all, and it is the documented
MLIR-reconsideration checkpoint (§18.4 Track C gate row, §18.5).

#### 18.10.1 Hardware mechanism (craq-sim ground truth)

The Tensix "kernel" is not one instruction stream — it is 3 (WH/BH) or 4 (QSR) concurrent in-order TRISC pipes that rendezvous through shared state. Six mechanisms the compiler must reason about:

1. **Three/four private in-order pipes.** `TENSIX_INST_PIPES` = 3 for `TT_VERSION<=1`, 4 for `>=2` (sim.h:211–215). Each pipe is a private FIFO with its own wait-gate mask, ttsync bits, and per-engine `busy_until` timers, all `[TENSIX_INST_PIPES]`-indexed (sim.h:481–541). Pipes share **only** the semaphore array, the mailbox, and the physical SrcA/SrcB/Dst datapath.

2. **5-class one-per-class-per-cycle issue arbiter.** Every cycle `clock_tensix_tile_one_cycle_rtl_aware` classifies each pipe head into one of 5 RTL issue classes via `tensix_rtl_issue_class_for_inst` (libttsim.cpp:2265–2280): `0xA0–0xA7→Sync`, `0xB0–0xB8/0x05→Cfg`, `0x70–0x99→Sfpu`, `0x08–0x18 || 0x21–0x3A→Math` (libttsim.cpp:2277), else `Tdma`. It issues **at most one instruction per class per cycle** in fixed priority `Math, Sfpu, Tdma, Cfg, Sync`, with per-class round-robin across pipes (`rtl_issue_next_pipe[]`, libttsim.cpp:2311–2394). This is a hard structural hazard: two pipes whose heads are the same class cannot both retire in one cycle.

3. **8 shared semaphores = the only explicit cross-TRISC channel.** `sem[8]`/`sem_max[8]` (sim.h:517–518), init `0xF` (tensix.cpp:88–90). `SEMPOST` increments, `SEMGET`/`SEMWAIT`-consume decrements (saturating at 15), `SEMINIT` sets value+max (tensix.cpp:11685–11731). Per-Tensix-global, so a producer pipe's `SEMPOST` is visible to a consumer pipe's `SEMWAIT` — the entire unpack→math→pack handoff substrate.

4. **Per-pipe wait-gate latch.** `SEMWAIT (0xA6)` and `STALLWAIT (0xA2)` latch a per-pipe gate (`semwait_active`/`stallwait_active`, sim.h:523,529). `tensix_wait_gate_blocks` (tensix.cpp:2067–2163) blocks a head iff its resource bit (`tensix_wait_gate_block_mask_for_inst`, tensix.cpp:1723–1897: MVMUL `0x26`→bit6, PACR `0x41`→bit2, UNPACR `0x42`→bit3, SFPU→bit8, Sync→bit1, CFG→bit7) intersects the latched `stall_res`. Critical subtlety: **a met predicate is forgotten and cannot be re-armed** (tensix.cpp:2075–2088, 12054).

5. **Implicit SrcA/SrcB/Dst bank-valid handshake (below the ISA).** The FPU stalls until the unpacker-filled source bank is valid — MOVB2A returns false ("stall until SrcB valid") on `!src_sync_data_ready(...)` (tensix.cpp:3941–3944; same gate 1629/1635/1685/1691). This is double-buffered `TensixSrcSyncState` (valid_man/read_id/write_id, sim.h:462–475) — a producer/consumer FIFO **no instruction names**.

6. **Per-engine issue latency.** `tensix_note_execution_resource_issue` (tensix.cpp:2251–2283) sets `math_busy_until`/`pack_busy_until`/etc. after issue (math/sfpu tunable issue gap, CFG fixed clock+2 two-stage pipe, others clock+1); a math op re-blocks while `clock < math_busy_until` (tensix.cpp:12068–12075). Decode gate order is `wait_gate_blocks → ttsync_lsq_blocks → math_issue_gap` (tensix.cpp:12044–12075).

#### 18.10.2 What GCC can model, and where it breaks

Split cleanly into what the single-stream Haifa/DFA scheduler carries versus what it cannot express.

**Modelable in GCC (the part worth building):**

- **The 5 issue classes + structural "one per class per cycle" hazard** map onto GCC function units
  for commands represented in RTL. Reuse F1's `rvtt-cost.md`: keep `type=tensix`, attach the
  orthogonal `math/sfpu/tdma/cfg/sync` attribute to compiler-visible `rvtt.md` patterns, and use its
  `define_automaton` / per-class reservations. Track C must first introduce compiler-visible RVTT
  patterns for any matrix/unpack/pack `.ttinsn` commands it intends to schedule; it cannot classify
  opaque inline assembly or infer classes by rewriting `ttrocc.md`.
- **Per-engine issue latency** is a standard `adjust_cost`/reservation-latency entry — direct port of the `*_busy_until` deltas.
- **SrcA/SrcB/Dst bank-valid producer→consumer edges** are expressible as true/anti/output dependencies **once the compiler emits all engines' code in one stream** — this is exactly Track B (DST + RWC hazard tokens, §18.9); the Haifa scheduler can order a single fused stream against those deps. Track B is a hard prerequisite for any single-stream fusion attempt here.

**Where GCC's IR hits the ceiling (the MLIR trigger):**

GCC schedules **one in-order stream per compilation.** The three cross-cutting behaviors below have no RTL analog:

| Hardware behavior | craq-sim anchor | Why RTL can't carry it |
| :--- | :--- | :--- |
| Cross-thread happens-before via `sem[8]` | tensix.cpp:11685–11731 | "math-pipe `SEMWAIT(sem3>=1)` must be dominated by pack-pipe `SEMPOST(sem3)`" is an inter-**stream** acquire/release match. A single-thread DAG has no edge type for it. |
| Wait-gate latch (met predicate forgotten, not re-armable) | tensix.cpp:2075–2088, 12054 | Not a monotone dependency; it is stateful dataflow-token semantics. |
| Implicit SrcA/SrcB bank-valid handshake | tensix.cpp:3941–3944 | No IR for "this op stalls until a bank owned by another thread goes valid." |

Modeling these forces one of two bad shapes: **(a)** three separately-compiled TRISC units with the compiler blind to the joint schedule (no cross-engine optimization — fails the §18.4 Track C whole-kernel gate), or **(b)** a fictitious fused single stream with barrier pseudos, which serializes away the very concurrency the hardware exists to exploit. This is the documented §8 TT-Vector-dialect trigger (§8, §18.5): async tokens / multiple concurrent value streams are the natural representation.

#### 18.10.3 Staged milestones

- **C0 — Issue-class model (GCC-clean, lands first).** Extend F1's orthogonal issue-class model to
  compiler-visible matrix/unpack/pack RVTT patterns as those patterns are introduced; do not change
  `type=tensix` membership and do not touch QSR `ttrocc.md`. Deliverable: the scheduler models the
  one-per-class-per-cycle structural hazard within a **single** pipe's represented stream. No
  cross-thread reasoning yet.
- **C1 — Single-stream fused block (requires Track B2).** With DST/RWC hazard tokens landed (§18.9), emit and schedule a **fused** unpack+matmul+SFPU+pack straight-line region (no semaphore rendezvous — one logical thread) and verify the bank-valid deps are honored. C1 needs Track B's alias model (B2); it is not transitively blocked on M2 because annotated raw-LREG enforcement already ships and B3 is now a verification backstop. This is the honest ceiling of what RTL carries.
- **C2 — Multi-TRISC rendezvous (the ceiling test).** Attempt whole-kernel scheduling across the 3 pipes with `SEMPOST`/`SEMWAIT` handoffs. Represent semaphores as **new: `__builtin_rvtt_sem_post/sem_wait`** intrinsics (new `unspecv` RVTT patterns, modeled `unspec_volatile` so they pin) and a **new: `rtl-rvtt-crossthread-sched` pass** that carries cross-stream happens-before edges. **This milestone is the go/no-go for MLIR** — if C2 needs a fictitious fused stream or blind per-thread compilation to build, the trigger fires.

#### 18.10.4 Hard gate

**An end-to-end unpack→matmul→SFPU→pack kernel scheduled by the compiler, measured whole-kernel on silicon, non-inferior to the handwritten LLK** (§18.4 Track C gate row). Measured, not asserted: device cycle count on the real kernel, same corpus/oracle harness as §18.5 (F1). C0 alone does not clear the gate — it is validated only that the DFA schedule matches the arbiter's per-class issue order on a single pipe (compare against `tensix_rtl_issue_class_for_inst` traces). The gate is cleared only at C2 with the joint schedule beating (or tying) handwritten whole-kernel cycles. The prerequisite is Track B2's DST/RWC alias model plus verified ownership enforcement; M2 becomes a dependency only if an actual fused corpus case demonstrates a physical-coloring failure that baseline IRA cannot resolve.

#### 18.10.5 Risks and the explicit GCC-ceiling / MLIR trigger

- **Primary risk — the ceiling is real and structural, not a coding gap.** The three behaviors in §18.10.2's table are the exact "cannot be expressed cleanly in GCC's IR without disproportionate backend surgery" condition (SFPI_COMPILER_UPGRADE.md:1393–1395). **Trigger, stated concretely:** if milestone **C2** cannot represent cross-TRISC `sem[8]` acquire/release matching (tensix.cpp:11685–11731) and the wait-gate latch (tensix.cpp:2075–2088) as first-class scheduler constraints — i.e. it degenerates to blind per-thread compilation (a) or barrier-pseudo serialization (b) — **stop and open the §8 MLIR TT-Vector dialect decision** (SFPI_COMPILER_UPGRADE.md:834–870). Make it a documented decision point, not drift (SFPI_COMPILER_UPGRADE.md:1395).
- **Dependency risk:** C1/C2 require Track B's DST/RWC tokens (§18.9); without resident Dst values and RWC-indexed aliasing, the single-stream dependencies do not exist. M2 is conditional rather than transitive: require it only if corpus evidence shows baseline IRA cannot color a qualifying fused region.
- **Fidelity risk:** the `busy_until` gaps and CFG clock+2 (tensix.cpp:2251–2283) are tunable in the sim; the reservation latencies in `ttrocc-sched.md` must be re-derived per silicon target (WH/BH/QSR), not hardcoded — the same F1 calibration surface.
- **Scope risk:** C0 is genuinely GCC-clean and high-value on its own (it fixes the flat-`type` scheduling blindness that also helps Tracks A/B/D). Land C0 regardless of the C2 verdict.

---

## Appendix A. Superseded Reviewer Opinion & Welford Findings

The following material is retained as investigation history only. Its status,
provenance, cycle values, and open-item claims are superseded by §§13-15 above.
NOTE ON NUMBERING: the §13.x subsection labels below are this appendix's own
historical numbering and do NOT refer to the authoritative main-body §13; cross-
references to them are always prefixed "Appendix A §13.x".

*Standing external review, folded in per single-document policy. Checked against the actual tree;
supersedes and replaces the former standalone review file.*

### 13.1 Verdict

The document is materially more honest than early revisions and §12 is genuinely sharp. As of this
revision the loop has absorbed §13 well — §2.2 no longer claims completed hardware validation (it
now reads *"Pure `sfpi::l_reg[]` passes; raw-LLK asm path requires fixed range model"*) and §12
correctly demotes the raw-LLK cause to a **leading hypothesis, not a verified root cause**. That is
real convergence toward accuracy. But it is convergence toward an *honest description of an
unreproduced bug*, not toward a fix: across the recent doc window the `gcc` submodule pointer is frozen (the real, committed scheduler
code lives in the submodule, not the superproject — see §13.7), so the churn is all superproject
prose while the compiler itself stands still. `rtl-rvtt-lp-alloc.cc` is still a 133-line dump-only
stub, both flags still `Init(0)`, and `run-corpus-differential.sh` is still absent.

### 13.2 The silicon row is now corrected (credit) — keep the document self-consistent

§2.2 previously claimed *"Functional verification complete"*; it now correctly scopes the status to
*"Pure `sfpi::l_reg[]` passes; raw-LLK asm path requires fixed range model."* This is exactly the
right correction and it supersedes the earlier "the row is false" objection. One housekeeping
consequence of folding review into the same document: keep §13's quotes in sync with §2.2 — the row
no longer contains the old text, so no section should still cite it. The failure is real; per §13.3
its compiler root cause is a **leading hypothesis, not yet reproduced**.

### 13.3 Verified Welford investigation (compiler was built and fixtures were run)

A recon + fix-design + adversarial-verification pass **built `build/sfpi/compiler` and compiled
fixtures.** Result: the hypothesized "explicit `l_reg[Lx]` is unmodeled" gap **does not reproduce**
through the sfpi-builtin path.

- `sfpi::l_reg[Lx]` lowers `builtin → GIMPLE gcall(index=INTEGER_CST) → RTL unspec_volatile on an
  allocatable pseudo → hard reg 80+idx`, with the final binding **forced** by the single-register
  class `x<N>` (`rvtt.md:186-205` → `rvtt-constraints.md:28-50` → `riscv.h:546-627`). That pseudo
  has a real live interval, so **baseline IRA already refuses to color a temporary onto L1 while L1
  is live.**
- Evidence: a `peak=6` fixture is byte-identical flag-on/off (scheduler never runs, gate
  `old_peak > 8`); a `peak=10 → new-peak=7` fixture drives the scheduler and **keeps L1 clean**.

**Corrected root cause — the raw-LLK path.** Welford's `L0–L3` arrive from raw LLK code (inline
asm / direct SFPU emission) outside C++ SSA — **no GCC pseudo, no live interval.** With nothing to
pin, IRA cannot know L1 holds a live raw value, so a surrounding sfpi temporary can be colored onto
L1 and clobber it. The reproducer used an all-`l_reg[]` fixture and therefore tested the
already-safe path.

### 13.4 What this means for §3.1 / §4 and the live patch

- **Drop (wrong IR):** any fix that adds `fixed_color` to the sched value or reduces the MILP
  `register_capacity`. The pressure scheduler only **reorders** gcalls; the MILP is **count-only**
  (`sum live ≤ capacity`, `rvtt-lpsolve.cc:351-363`), with no color dimension. It cannot keep a
  temporary off physical L1. Prevention belongs in **RTL register allocation (IRA conflict
  edges)**, not the GIMPLE pressure model.
- **Keep (as a detector):** a post-IRA verifier wired at exactly `INSERT_PASS_AFTER(pass_ira, 1)`
  (before `pass_rvtt_synth_opcode`), anchored on the readlreg-produced hard regno with source-use
  interval endpoints, const-reg-guarded (`regno < SFPU_REG_FIRST + SFPU_CREG_IDX_LWM`).

### 13.5 Recommendations

1. ~~Correct the §2.2 silicon row~~ **(done — §2.2 now scopes silicon status accurately).**
2. Obtain the **real raw-LLK reproducer** (raw `L0–L3` loads + sfpi consumers) and confirm the L1
   clobber on this branch before writing a fix.
3. Model raw-asm LREG defs/uses as **fixed live ranges** for IRA (precise interval `[raw def, last
   use]`, not blanket region reservation).
4. Land the post-IRA verifier as the backstop.
5. Make the regression **discriminating** — push `old_peak > 8` with an explicit/raw LREG live
   across, checked against a deliberately-regressed allocator or a checking-assert; a
   byte-identical-on-this-branch fixture proves nothing.
6. Stop improving the description; land one real thing behind the off flag rather than another pass
   over §4.2 or §12.

### 13.6 The loop has converged on accuracy, not resolution

The document is now a precise, well-hedged description of a bug that has **not been reproduced** and
a fix that has **not been written**. That epistemic state — "raw-LLK missing live ranges is the
leading hypothesis; a silicon failure may have other causes" (§12) — is where this investigation
should have *started*, not where it lands after eight revisions. Each further Rebut/Harmonize pass
refines the prose; none produces the raw-LLK reproducer or the IRA-conflict/verifier change the
analysis already specifies. That is the whole finding this round: the plan is no longer blocked on
understanding or on honesty. It is blocked on someone running the real kernel and writing the one
change §13.3–13.4 already pin down.

### 13.7 Silicon is green — commit-history confirmation (update)

Blackhole Welford-body results are in and **green**: `vFloat direct` and `vFloat rescue` both run
at **339 device math cycles** (N=32 correctness passes all five selectors), **27.3% faster** than
non-replay handwritten code (466), and **~4% / 13 cycles slower** than the production **replay** LLK
(326). This is the first real data point in the whole saga, and the commit history confirms exactly
what it validates:

- **The win comes from committed code, not the doc churn.** The scheduler, the `lp_solve` MILP
  adapter, and the alloc stub were all added in a single `gcc`-submodule commit — `8bea8aba49`,
  *"riscv: add opt-in SFPU pressure scheduling"* (2026-08-14) — integrated into the superproject at
  `be125cd` *"toolchain: integrate generic SFPU pressure scheduler"*, after a real ramp
  (`074123f` WIP Welford → `86dadf8`/`7a12652` Blackhole checkpoints → `a549386` validation
  workflow). The `vFloat rescue` variant **is** that opt-in GIMPLE pressure pass.
- **Provenance is clean and frozen.** All 40 superproject commits since `be125cd` — the entire
  Rebut/Harmonize doc saga — touched the `gcc` pointer **zero** times. The silicon result therefore
  measures `8bea8aba49` exactly, unchanged by any subsequent prose.
- **Correction to earlier rounds (§13.1/§13.6):** my "no compiler code changed" observations were
  measuring the *superproject*, which by design only tracks docs + the submodule pointer. Real,
  substantial compiler code **does** exist and is committed — in the `gcc` submodule. The accurate
  statement is: the code landed *before* the doc window and has been frozen through it.
- **The win is scheduler-only; M2 is still a stub.** `rtl-rvtt-lp-alloc.cc` in `8bea8aba49` still
  prints `colorability=unchecked`. Silicon-green is produced entirely by the GIMPLE pressure
  scheduler with baseline IRA — **not** by the §4 M2 DSATUR allocator, which remains unbuilt. This
  confirms §13.3–13.4: the pure-`vFloat`/`l_reg[]` path is IRA-safe on real silicon, and the
  earlier correctness concern was resolved by expressing Welford fully in `vFloat` (sidestepping the
  raw-LLK-interop path that §13.3 identified as the actual clobber risk).
- **"Derisked" is fair.** Correctness is proven on silicon; the residual ~4% (13 cycles) is the
  **replay-buffer compression** gap (§6 / P5) — a throughput feature the compiler does not emit yet
  — not a correctness or allocation risk. Closing those cycles is perf tuning, not de-risking.
- **Archived (was open):** the paired A/B numbers are recorded in-tree at **Appendix A §13.8** (this is the superseded 339-cycle run; §2.2 now carries the authoritative 323/326 result, not these numbers). Still to attach when exported from the harness: the raw per-run cycle
  dumps and correctness vectors + device/firmware metadata, so the summary is backed by primary
  artifacts, not just a table.

### 13.8 Archived Silicon Results — Blackhole Welford Body (2026-08-15)

*In-repo archive of the paired A/B silicon run, per Appendix A §13.5 item 5 / §13.7. This is the reported run
summary; raw device logs (per-run cycle dumps, correctness vectors) should be attached alongside
when exported from the harness.*

**Device / provenance**
- Device: Blackhole (`bh-33`), device **math** cycles.
- Compiler under test: `gcc` submodule `8bea8aba49` ("riscv: add opt-in SFPU pressure scheduling"),
  integrated at superproject `be125cd`; `gcc` pointer unchanged since (verified: 0 bumps in the
  40 commits after `be125cd`). Superproject review commit: this branch (`nkapre/sfpi`).
- Kernel: Welford (LayerNorm) body. Flags: `vFloat rescue` = `-mtt-tensix-optimize-pressure-schedule`
  (opt-in pressure scheduler); `vFloat direct` = same source, scheduler off.
- Correctness: separate **N=32** pass, **all five selectors** pass. Cycle runs are deterministic
  (three identical samples per variant).

**Results (device math cycles, 3 runs each)**

| Variant | Cycles (run1/2/3) | vs production replay LLK |
| :--- | :--- | :--- |
| Handwritten **replay** LLK (production baseline) | 326 / 326 / 326 | baseline |
| **vFloat direct** (scheduler off) | 339 / 339 / 339 | +13 cyc, **+3.99%** |
| **vFloat rescue** (pressure scheduler on) | 339 / 339 / 339 | +13 cyc, **+3.99%** |
| Handwritten **direct** (non-replay) | 466 / 466 / 466 | +140 cyc, **+42.9%** |

**Conclusions**
- The compiler-generated `vFloat` path is **27.3% faster** than non-replay handwritten code
  (466 → 339) and within **~4% (13 cycles)** of the production **replay** LLK (326). All runs passed.
- `direct` and `rescue` tie at 339 here: the Welford body fits without pressure rescue on this
  input, so the scheduler is correctly a no-op (consistent with the peak-≤8 bypass), not a
  regression. Rescue's value shows on higher-pressure bodies, not this one.
- The residual gap to 326 is entirely the **replay-buffer compression** the production LLK uses and
  the compiler does not yet emit (§6 / P5) — a throughput feature, not a correctness or allocation
  gap. "Manual early-fold" was in flight at capture time; append its row when it lands.
- This validates the generated compiler flow and establishes scheduler-flag non-regression on this
  bypassed body. It does **not** validate a pressure rewrite and does **not** exercise M2
  (`rtl-rvtt-lp-alloc.cc` still `colorability=unchecked`).

### 13.9 Reproducer, Runbook & Archived Report Card (folded from WELFORD_SILICON_VALIDATION.md)

Consolidated here per single-document policy; the standalone file is removed. Full build-runbook and
per-file test drivers remain recoverable from git history (commit `e0057ae`).

**Archived report card (Blackhole)**
- STATUS: `GO-BH-ONLY` (green on silicon). TESTED_ARCH: Blackhole (`-mcpu=tt-bh-tensix`).
- COMPILER_COMMIT: `8bea8aba49` (gcc submodule), integrated `be125cd`.
- FUNCTIONAL: PASSED — N=32 across all 5 selectors, 100% parity vs FP64 reference Welford.
- DEVICE MATH CYCLES: non-replay handwritten 466; vFloat direct 339 (−27.3%); vFloat rescue 339
  (−27.3%); production replay LLK 326 (vFloat +13 cyc / +3.9%, attributable to replay-buffer
  frontend compression).
- VERDICT: correctness + register-allocation safety for this compiler-generated Welford flow were
  verified on silicon, and enabling the scheduler flag was non-regressing because the measured
  body bypassed the pass. This is not the changed-binary scheduler evidence required for
  default-on; that remains open under §2.3.

**Selectors under one shared wrapper** (loads, transpose, reciprocal, init, stores shared so cost is
not misattributed): `HANDWRITTEN_DIRECT`, `HANDWRITTEN_REPLAY` (perf golden), `VFLOAT_RESCUE`
(pressure scheduler on), `VFLOAT_MANUAL_EARLY_FOLD` (control), and baseline direct. Numerical golden
is sequential FP64 Welford; cycles are real `read_wall_clock()` device counters.

**Reproducer shape** (`sfpu_welford_test.cpp`; note the explicit-LREG loads are the sfpi `l_reg[]`
path — the IRA-safe one per §13.3, not raw-LLK inline asm):
```cpp
// inputs prefetched into L0-L3; state/reciprocal in L4/L5/L7
vFloat x0=l_reg[LRegs::LReg0], x1=l_reg[LRegs::LReg1], x2=l_reg[LRegs::LReg2], x3=l_reg[LRegs::LReg3];
vFloat mean=l_reg[LRegs::LReg4], m2=l_reg[LRegs::LReg5], recip=l_reg[LRegs::LReg7];
// VFLOAT_RESCUE body, per row:
vFloat delta = x - mean;  mean += delta * recip;
vFloat delta2 = x - mean; m2   += delta * delta2;
// ... 4 rows ... then store: l_reg[LRegs::LReg4]=mean; l_reg[LRegs::LReg5]=m2;
```

**Pinned build / validate**
```bash
git clone --recursive --branch nkapre/sfpi git@github.com:tenstorrent/sfpi.git sfpi-scheduler
git -C sfpi-scheduler submodule update --init --recursive
SFPI_WITH_LP_SOLVE=yes ./scripts/build.sh --dir="$PWD/../sfpi-silicon-build" --checking
./scripts/validate-sfpu-pressure-scheduler.sh "$PWD/../sfpi-silicon-build/sfpi" out/
# Zero-setup: point CUSTOM_SFPI at the build and run existing TT-Metal Blackhole suites
#   pytest .../operations/fused/test_layer_norm.py ; .../vae/tests/pcc/test_welford_state_leak_regression.py
```

**Still to attach** (raw primary artifacts, per §13.7): per-run cycle dumps, per-selector
disassembly + LREG occupancy + SFPMAD/NOP counts + SHA-256 ELF hashes, and device stepping/firmware.

---

## Appendix B. Superseded Silicon Reanalysis

This appendix records the pre-integration critique. See authoritative §14. The
§14.x subsection labels below are this appendix's own historical numbering and do
NOT refer to the authoritative main-body §14; cite them as "Appendix B §14.x".

The Blackhole result is real and useful, but three distinct conclusions must not be conflated. The
clean, capture-free Welford-body measurements were 466 cycles for handwritten direct, 326 cycles
for handwritten replay, and 339 cycles for generated `vFloat` direct and rescue. Each value was
repeated in three fresh processes with zero observed spread. A separate matched correctness suite
validated both mean and M2 for all five selectors at N=1, N=2, and N=32. However, the pressure
scheduler bypassed the measured body, so these data do **not** establish a scheduler performance
win or exercise a pressure rescue. The primary logs, binaries, and disassemblies also remain local
rather than committed as a self-contained in-tree silicon archive.

### 14.1 The Only Scheduler A/B Delta Reported Is Exactly Zero

The controlled compiler-feature comparison is `vFloat direct` with the scheduler off versus the
same source with the scheduler on. Both are 339 cycles. Section 13.8 also states that the pass takes
the peak-at-most-eight bypass and is a no-op. Therefore the measured scheduler effect is:

```text
scheduler off: 339 cycles
scheduler on:  339 cycles
delta:           0 cycles (0.0%)
```

The 466-to-339 comparison changes the implementation from handwritten non-replay LLK to generated
`vFloat`; it is not an on/off test of `-mtt-tensix-optimize-pressure-schedule`. Attributing that
27.3% delta to the scheduler is a category error. The result proves that compiling with the flag is
non-regressing on this bypassed case, which is valuable, but it cannot demonstrate the pass's
benefit or justify calling this Welford result “scheduler-only.”

To measure the scheduler, the silicon fixture must show `old-peak > 8`, `applied=yes`, and different
off/on assembly, then run both binaries under the same wrapper. The existing compiler-only rescue
fixture supplies the first part but is not the Welford silicon binary reported here.

### 14.2 Primary Evidence Exists Locally but Is Not Yet In-Tree

The silicon campaign produced real local primary evidence: clean per-selector device-cycle logs,
separate N=1/N=2/N=32 correctness logs, final ELF images, hashes, and disassembly. The measurement
uses a hardware math-thread counter around `WELFORD_BODY`; it is not pytest wall time. The local
runner now also persists its profiler rows. Thus it is accurate to call the values **measured on
Blackhole**, rather than merely inferred or attested.

That evidence is not yet independently reproducible from this branch. The checked-in
`sfpu-pressure-results.tar.gz` is a host compiler/assembly bundle and does not contain the silicon
logs, measured TT-Metal ELF images, device record, or the current executable LLK fixture. Local
absolute paths are useful provenance for the ongoing investigation, but they are not a portable
archive. Reserve **in-tree archived/reproducible** for a committed bundle containing at least:

- raw output for every cycle and correctness run;
- exact TT-Metal commit, device ID/stepping, firmware and runtime configuration;
- full compile commands and selector definitions;
- ELF and disassembly hashes proving which binary produced each row; and
- an executable driver that parses results and fails on correctness/performance thresholds.

### 14.3 The Folded Template Is Not the Measurement Harness

The full driver preserved in commit `e0057ae` is explicitly a handoff template. Its functional test
ends with `assert True`; it never launches a device kernel or compares device output with the FP64
reference. Its performance runner returns only whether a pytest command exited successfully; it
does not read, parse, or report a hardware cycle counter. The C++ timing sketch comments “Record
end_cycles - start_cycles” but contains no implementation that exports that value.

Those snippets cannot substantiate “100% parity,” 339 cycles, or deterministic three-run results.
They should not be confused with the current local LLK fixture, which launches real selector ELFs,
compares the captured L4 mean and L5 M2 against a host Welford reference in correctness mode, and
uses a capture-free `TRACE_N=0` hardware profiler interval in performance mode. The current fixture
is the evidence-producing harness, but it remains uncommitted. It and its exact invocation must be
pinned before the branch is self-contained.

### 14.4 Correctness Now Exercises the Raw-LLK Boundary, but Scope Remains Bounded

The original failure was reproduced at the actual raw-LLK boundary. Blackhole Welford loads and
transposes L0-L3 through opaque raw `.ttinsn` operations; without compiler-visible metadata, IRA
could assign a generated temporary to still-live L1, corrupting the next row's input. CRAQ tracing
localized the first clobber, and physical Blackhole reproduced the resulting N=2 divergence.

SFPI-GCC commit `97df2fddd5f7485235a08f26c6325a82cdd1e824` adds explicit raw-LREG access metadata and a
pre-IRA live-in pass. TT-Metal commit `f9bc067285f104df709075f0839f80425ded459d` marks the Welford
raw producer after its final transpose. With those changes, CRAQ N=1/N=2 and physical Blackhole
N=1/N=2/N=32 runs pass for handwritten direct, handwritten replay, `vFloat` direct, `vFloat`
rescue, and manual early fold. At N=32, all five selectors report mean maximum absolute error
0.001953125 and M2 maximum absolute error 0.9746094, within the prescribed BF16 tolerances.

This validates the marked Welford raw-producer-to-generated-consumer path and the specific raw-LREG
fix. It does not validate every opaque LLK macro, all CFG shapes, the general allocator, or
Wormhole silicon. Because the pressure scheduler bypassed this body, it also does not exercise the
transformed schedule, validator acceptance, list/MILP choice, rollback, or high-pressure register
allocation on silicon.

### 14.5 Replay Structure Is Demonstrated; Cycle Attribution Is Still Incomplete

The clean Blackhole triplets are deterministic: handwritten replay is 326/326/326 cycles and
generated `vFloat` direct is 339/339/339 cycles, a 13-cycle or 3.99% gap. Local final-ELF
disassembly also demonstrates the structural difference: handwritten replay issues 32 `TTREPLAY`
commands over fixed six-operation row images, while the generated body has no body replay and
roughly fifty more static RISC/Tensix words.

That evidence makes replay compression the leading explanation, but it does not prove that the
entire 13-cycle wall-clock difference is caused by replay. Static command counts do not model
scoreboard stalls, asynchronous issue, or overlap. A matched replay-disabled control or an
instruction-issue trace is still required before using “entirely attributable.” Report the exact
observed gap and the demonstrated structural correlation separately from the causal claim.

### 14.6 Revised Engineering Verdict

- Accept the 339-cycle value as a clean, capture-free `WELFORD_BODY` Blackhole device-math result;
  handwritten replay remains the 326-cycle golden implementation, so generated `vFloat` is 3.99%
  slower in this comparison.
- Accept mean and M2 correctness for all five selectors on the separately matched N=1/N=2/N=32
  Blackhole correctness suite for this marked raw-LREG path. The performance invocation itself is
  capture-free and does not repeat those output comparisons inline.
- Record the scheduler A/B conclusion accurately: **0.0% delta on a bypassed peak-at-most-eight
  Welford body; non-regression shown, rescue benefit not exercised.**
- Do not use 466-to-339 to credit the pressure scheduler; it compares different implementations.
- Treat the raw-LREG fix as positively validated for the Welford boundary, while retaining broader
  raw-LLK and CFG coverage as open work.
- Primary evidence exists locally, but do not call the run independently reproducible from the
  branch until the artifact bundle and real harness are committed.
- Run the actual 9-to-8 Welford rescue and a non-Welford changed-binary case on silicon, paired
  off/on, before treating this result as scheduler silicon validation.
- Keep the 13-cycle replay attribution qualified until a matched replay-disabled control or
  instruction-issue trace establishes causality.

This preserves the valid scheduler rebuttal without erasing the separately validated raw-LREG
correctness fix or conflating uncommitted evidence with nonexistent evidence.

---

## Appendix C. Superseded Integration Rebuttal

The integration and CFG objections below were resolved by `0c9adf7d2`,
`3b5d6a43d`, `d9c39fbd1`, and the committed archive. See authoritative §15. The
§15.x subsection labels below are this appendix's own historical numbering and do
NOT refer to the authoritative main-body §15; cite them as "Appendix C §15.x".

The silicon validator's correction materially advances the investigation. Both referenced external
commits are real and inspectable: SFPI-GCC `97df2fddd5f7485235a08f26c6325a82cdd1e824` adds raw-LREG
metadata plus a pre-IRA live-in pass, and TT-Metal
`f9bc067285f104df709075f0839f80425ded459d` emits a write marker for Welford's raw L0-L3 producer.
That is executable engineering, not another pseudocode proposal.

The remaining disagreement is about integration and proof. The branch describes local artifacts
that reviewers cannot inspect and calls a one-path fix validated before its CFG lifetime
construction has a complete regression matrix. The available local provenance is consistent with
the corrected compiler being used for both correctness and performance, so a stale submodule pin
must not be promoted into a claim that the silicon binary necessarily used the old compiler.

### 15.1 The Pinned Superproject Does Not Contain the Validated Raw-LREG Fix

This SFPI tree still pins its `gcc` submodule to
`8bea8aba4945485f32307212d28ca7dc6107f18d`. The raw-LREG fix is the later descendant
`97df2fddd5f7485235a08f26c6325a82cdd1e824`, but the superproject submodule pointer has not moved.
Consequently, the pinned build commands in §13.9 cannot build the compiler described in §14.4.

The document also continues to identify `8bea8aba49` as the compiler under test in §2.2, §13.7,
§13.8, and §13.9. That metadata is stale, but it does not prove that the 339-cycle executable was
built by the old compiler. The clean performance runner resolved to the raw-LREG candidate compiler
installation. Its installed `cc1plus` has SHA-256
`26af0f1ec56ff992c17acd1e95d6849bfc6659f6fa5e1b4ffa5414484d3ed271` and contains the
`rvtt_lreg_livein` pass and `RAWLREG` pattern. The old `g8bea8aba4` package string was inherited
from the original configure metadata, so it is not reliable binary provenance.

The accurate conclusion is narrower: local evidence is consistent with the fixed compiler, but
the branch and run logs do not bind every measured ELF to a reproducible source tuple. This is an
integration and archival defect, not evidence that the measured 339 cycles or matched correctness
results are invalid.

The integration gate is straightforward:

1. bump the SFPI submodule to the reviewed SFPI-GCC fix;
2. record the exact TT-Metal commit and SFPI superproject commit for each run;
3. rebuild every selector from that pinned tuple;
4. archive binary hashes proving both correctness and performance used that tuple; and
5. rerun host compiler, CRAQ, and silicon matrices after the pointer bump.

Until then, §14.4 is credible local field evidence for the fixed compiler and TT-Metal marker, but
not validation of the checkout produced by this document's own runbook.

### 15.2 The New Pass Has a Live-Out Endpoint Hole at Basic-Block Boundaries

The pre-IRA pass computes a conservative union dataflow and creates a fresh fixed-register token in
each block. That is a reasonable approach. Its live-out endpoint is currently emitted with:

```cpp
rtx_insn *last = BB_END (bb);
if (live[regno])
  end_sentinel (live[regno], last);  // emits USE before `last`
```

For a raw LREG live out of the block, placing its final `USE` **before** `BB_END` does not keep it
live through the final instruction. If `BB_END` is an SFPU definition, IRA may legally reuse that
fixed LREG for the definition after the sentinel dies, clobbering the architectural value before a
successor consumes it. The successor's fresh entry token does not repair a clobber already emitted
in the predecessor; independent tokens deliberately carry no value-preserving edge relationship.

This needs a structurally correct end-of-block representation and a discriminating test where:

- a raw marker makes L1 live across an edge;
- the predecessor's final instruction defines an allocatable SFPU pseudo;
- the successor consumes L1; and
- compilation without correct through-edge protection assigns the final predecessor definition to
  L1 or triggers an explicit checking failure.

Add fallthrough, conditional, loop-backedge, critical-edge, empty-block, and multi-predecessor
variants. The current `raw-lreg-livein-bh.C` is a useful straight-line assembly test, but it does not
exercise this live-out boundary.

### 15.3 “Read Mask” Currently Means “Lifetime Kill,” Which Must Be an Explicit Contract

`raw_access_p` names its operands `read_mask` and `write_mask`, but `transfer_block` implements:

```cpp
live = (live & ~reads) | writes;
```

and the rewrite ends a token on every marked read. That behavior is correct only if a read bit means
**last raw use / ownership transfer to a compiler-visible pseudo**, not merely “this raw instruction
reads the LREG.” A non-final raw read followed by another raw use would release the reservation too
early and recreate the clobber class this pass is intended to prevent.

Define the marker semantics in the API. Either rename the first operand to `kill_mask`/`last_use_mask`
or distinguish ordinary reads from lifetime-ending reads. Add negative tests with multiple raw reads
and intervening generated temporaries; a metadata API whose ordinary-sounding “read” operation
silently kills liveness is too easy for LLK call sites to misuse.

This ambiguity does not invalidate the measured Welford case: its marker is `(read_mask=0,
write_mask=0x0f)`, so it does not exercise the questionable read-mask behavior. It does prevent the
current API from being accepted as a universal raw-LLK contract.

### 15.4 The Committed Regression Proves One Straight-Line Blackhole Allocation

SFPI-GCC `97df2fddd5` adds one Blackhole assembly-scan test. It checks that temporaries following a
raw L0-L3 producer do not occupy L1-L3 before their explicit reads. It does not compare behavior
with the marker omitted, exercise CFG propagation, test repeated producer/consumer epochs, cover
mixed raw and builtin reads, or validate Wormhole.

The reported CRAQ and Blackhole tests may cover the real Welford failure, but they remain local. A
positive silicon result is not a substitute for compiler regressions that force every dataflow
edge case. Before treating the pass as a general raw-LREG contract, add at least:

- a deliberately failing no-marker or disabled-pass control proving the test is discriminating;
- write-after-write, simultaneous kill/write, and multiple-LREG partial-mask cases;
- the CFG cases from §15.2 on both WH and BH compiler targets;
- assembly checks proving the metadata emits no hardware instruction; and
- deterministic fallback/diagnostics for malformed or unsupported marker use.

### 15.5 Resolved Canonical-Text Contradictions

Section 14 correctly says the scheduler was bypassed, the raw-LREG boundary was separately fixed,
and replay causality is not fully proven. The current revision removes or scopes the earlier
normative-looking claims that said:

- the 339-cycle result “validates the committed opt-in scheduler on real silicon”;
- the win is “scheduler-only” even though scheduler off and on are identical;
- the earlier correctness concern was resolved by sidestepping raw LLK, although §14.4 now says the
  raw boundary was reproduced and fixed;
- the 13-cycle gap is “entirely” replay compression; and
- the archived report uses compiler `8bea8aba49` while the fix under discussion is `97df2fddd5`.

Those claims are retained here only as the historical review finding. The canonical §2.2–§2.3
tables and the archived report-card conclusion now state that Welford established compiler-flow
correctness and flag non-regression on a bypassed input, not pressure-scheduler silicon benefit.

### 15.6 Revised Decision

- **Accept** that the silicon validator found and reproduced a concrete raw-LLK/SFPI boundary bug.
- **Accept** the clean 339-cycle Blackhole device-math measurement and the separate matched
  N=1/N=2/N=32 mean/M2 correctness results as valid local evidence.
- **Accept provisionally** that the two external commits fix the measured Welford case on Blackhole;
  the CFG and API gaps prevent a universal-safety claim, not acceptance of this straight-line case.
- **Do not yet accept** that this SFPI branch contains that fix; its submodule pointer proves it does
  not.
- **Do not yet accept** the live-in pass as CFG-safe until the pre-`BB_END` lifetime hole is fixed
  and covered by discriminating edge tests.
- **Keep the scheduler conclusion unchanged:** the reported silicon scheduler A/B is 339 versus
  339 on a bypassed body, so it establishes non-regression, not rescue benefit.
- **Require one pinned artifact tuple**—SFPI, SFPI-GCC, TT-Metal, device/firmware, harness, logs,
  ELFs, hashes, and disassemblies—before promoting local field results to reproducible branch
  evidence.

The validator has supplied the first plausible real fix in this loop. The correct response is to
integrate and harden it, not to invalidate valid local measurements because their source tuple was
not pinned or stretch one corrected path into a broader certification than it supports.

### 15.7 Required Follow-Up Before Integration GO

The silicon measurement itself is good; the compiler integration is not finished. Complete these
items before declaring the raw-LREG mechanism generally safe or the branch reproducible:

1. fix the basic-block live-out endpoint so a protected LREG remains unavailable through the final
   predecessor instruction, and add fallthrough, conditional, loop, critical-edge, empty-block,
   and multi-predecessor regressions;
2. define the first marker operand as either ordinary read, last use, or ownership transfer, encode
   that contract unambiguously, and test repeated raw reads and mixed read/write masks;
3. bump the SFPI `gcc` submodule to the reviewed fix, rebuild from that pinned checkout, and rerun
   focused compiler, CRAQ, N=1/N=2/N=32 correctness, and clean three-process profiler gates;
4. commit the real LLK fixture, raw logs, profiler rows, ELF/text hashes, disassembly, device record,
   and exact commands, then correct the earlier canonical tables that still attribute 466-to-339
   to the scheduler or call the replay explanation complete; and
5. run a changed-binary `old-peak > 8` scheduler off/on silicon case before claiming scheduler
   performance benefit.

Items 1-4 are required to promote the raw-LREG work from a successful Welford fix to an integrated,
reproducible compiler solution. Item 5 is a separate scheduler-performance gate and does not affect
the validity of the existing Welford device-cycle measurement.

---

## 19. Strategic Assessment: Evidence, Corrections, and the Next Compiler Sprints

*Evidence-bounded synthesis of what the GCC-extension results prove, what they do not prove, and
which compiler mechanisms have falsifiable silicon gates.*

> **[CURRENCY NOTE 2026-08-17.]** This assessment predates the planner/WP8/recalibration landings
> and the p150 re-measurements. Its stale figures (Min/Max +41%, Typecast +19.6%, Where +96.2%,
> SigmoidAppx +100.5%) and its `fd8ed6f`-era sim framing are SUPERSEDED by **§18.8.0.4** (current
> loss table, chip-class split) and **§18.8.0.5** (adversarial verification of the landed state).
> Retained for the strategy reasoning, which stands.

### 19.1 From Theoretical Debate to Measured Silicon Viability

Physical Blackhole results establish that compiler-generated SFPI can match or narrowly beat
hand-tuned Tensix assembly in several correctness-gated profiler zones
(`validation/welford-bh-20260815/`, `REDUCE_SDPA_SILICON_AB.md`):

```
                       EMPIRICAL SILICON VALIDATION RECORD
┌────────────────────────────┬─────────────────────────────┬─────────────────────────────────────────────────┐
│ Kernel Benchmark           │ Silicon Outcome (Blackhole) │ Technical Driver                                │
├────────────────────────────┼─────────────────────────────┼─────────────────────────────────────────────────┤
│ **Reduce-SDPA Body**       │ **834 vs 840 cycles (-0.7%)**│ Generic D1 preheader replay capture hoisting    │
│ **Reciprocal BF16**        │ **459 vs 467 cycles (-1.7%)**│ 10-slot compiler replay capture + 7 playbacks   │
│ **Welford Body**           │ **323 vs 326 cycles (-0.9%)**│ Raw-LREG ownership + literal coalescing         │
│ **Binary Broadcast**       │ **608 vs 608 cycles (0.0%)** │ Exact cycle parity; 100% functional match       │
│ **Addcmul**                │ **292.99 vs 292.93 (+0.02%)**│ Generic two-row fusion/interleave; parity, no win│
└────────────────────────────┴─────────────────────────────┴─────────────────────────────────────────────────┘
```

These are meaningful proofs of viability, not corpus-wide dominance.  They are bounded body zones,
not whole-kernel throughput; Welford is also not an identical-source scheduler A/B.  Wormhole and
most of the 164-row corpus remain unmeasured against hand-tuned implementations on silicon.

### 19.2 M2 Is Not the Measured Blocker; It Is Not Universally Obsolete

The current measured silicon losses do not show allocator spilling as their cause.  The evidence
supports keeping M2 on standby, with a concrete trigger rather than a rhetorical conclusion. A new
compile-time adversarial result also sharpens that trigger: hoisting nine invariant SFPU constants
without a pressure budget extends their simultaneous LREG lifetimes and ICEs in reload, while the
option-off source compiles. This blocks that LICM prototype; it does not retroactively explain the
measured kernel losses.

1. **GCC IRA is sufficient for the tested regions.** The `x<N>` constraints, multi-result RTL, and
   raw-LREG sentinels protect the live intervals exercised by the currently measured kernels.
2. **The sentinel mechanism is real but its provenance must be stated correctly.**
   `rtl-rvtt-lreg-livein.cc` models opaque LREG ownership; `8f943c2f8` is the predicate DEBUG-use
   fix, not the sentinel-pass commit.
3. **M2 remains a standby audit stub, but pressure accounting is immediately required.** The
   invariant-hoist pass must conservatively budget existing loop-live values plus every proposed
   hoist and refuse before mutation when the eight allocatable LREGs may be exceeded. Activate the
   exact allocator only if a reviewed, semantically valid corpus region remains profitable after that
   guard yet IRA cannot color it. The present data do not prove every future region spill-free.

### 19.3 Correcting the Throughput Diagnoses

The §18.8.0 scorecard contains six explicit loss rows (with Binary Min/Max combined), not five,
and additional measured semantic lanes such as Exp still need to be folded into that table.  The
current artifacts do not implicate allocation in these losses, but each proposed throughput fix
still needs changed-binary silicon acceptance.

```
┌──────────────────────────────┬───────────────┬─────────────────────────────────────────────────────────────┐
│ Identified Bottleneck        │ Impacted Ops  │ Concrete Architectural Remediation                          │
├──────────────────────────────┼───────────────┼─────────────────────────────────────────────────────────────┤
│ **1. Partial Latency-Hiding │ Addcmul       │ Landed: form/interleave a proven two-row group before IRA;  │
│    Reorder Scheduling**      │ +21.9%→+0.02% │ now silicon parity. Generalize only from measured residuals │
│                              │               │ and retain alias/SSA/byte-identity fallback proofs.         │
├──────────────────────────────┼───────────────┼─────────────────────────────────────────────────────────────┤
│ **2. Missing SFPLOADMACRO    │ Min/Max (+41%)│ Use the audited CRAQ event model, then prove complete config │
│    Formation**               │ Typecast      │ ownership, hidden effects, and byte-identical fallback in   │
│                              │ (+19.6%)      │ active multi-op macro region emission.                      │
│                              │ Where (+96.2%)│                                                             │
├──────────────────────────────┼───────────────┼─────────────────────────────────────────────────────────────┤
│ **3. Loop-Invariant Constant │ SigmoidAppx   │ Hoist proven invariants only under an explicit peak-LREG    │
│    Rematerialization (LICM)**│ (+100.5%)     │ budget; refuse transactionally before IRA spill/reload ICE. │
├──────────────────────────────┼───────────────┼─────────────────────────────────────────────────────────────┤
│ **4. Cost Model Calibration**│ General       │ Model 1/cycle issue throughput separately from 2-cycle     │
│                              │ Schedule      │ result latency; calibrate with Blackhole silicon A/B.        │
└──────────────────────────────┴───────────────┴─────────────────────────────────────────────────────────────┘
```

### 19.4 Exact MILP as a Bounded Offline Oracle, Then a Candidate Engine

The checked-in MILP is useful beyond emergency fallback: its exact issue-position and liveness
model, bounded region size, node cap, and independently validated certificate make it a credible
**offline oracle for that abstract pressure model**.  The 11-to-8 fixture proves that exact search
can recover a legal pressure schedule outside the heuristic's reach.  It does not yet make the
current model an oracle for physical makespan, replay throughput, macro calendars, Dst aliasing, or
constant placement; those semantics and calibrated costs must be added and independently tested.

The present objective is deliberately narrow: if the preferred list schedule already satisfies
capacity, it becomes the unique zero-deviation optimum. That is a useful fast proof, but it leaves
the engine's larger potential unused. Evolve the bounded model in stages:

1. **Pressure feasibility and optimality:** retain the exact peak-at-most-eight constraint and
   distinguish proven infeasibility, cap exhaustion, and optimal solutions.
2. **Physical makespan:** with feasibility fixed, minimize the calibrated dependency schedule using
   separate issue-throughput and result-latency inputs.
3. **Reuse and movement:** with makespan fixed, minimize copies, destructive-reuse misses, and
   harmful rematerialization.
4. **Frontend opportunity:** score legal replay bodies, invariant placement, and macro-compatible
   groups without weakening ownership or barrier constraints.
5. **Deterministic tie-break:** only after the architectural objectives are fixed, minimize
   deviation from the stable list/source order.

After the corresponding architectural semantics are present, run this exact lane on bounded corpus
regions in CI/oracle mode, including list hits, and publish heuristic-versus-optimal gaps plus solver
node/time distributions.  Start with a sampled/size-capped lane rather than making every bounded
region a blocking CI solve before compile-cost distributions exist.  The fast list scheduler remains
valuable for low compile latency; production policy follows the measurements.

The exact engine may later consume Sprint 1's pre-IRA two-row Addcmul groups, but it is not on the
critical path to the first changed-binary silicon discriminator.  First prove the group legality and
calibrated dependency model with the deterministic interleaver; then use that reviewed DAG as the
MILP/list comparison input. M2 physical coloring remains a distinct downstream mechanism and must
not be conflated with the scheduling MILP.

### 19.5 GCC Is the Near-Term Path, with an Explicit Ceiling

Replay hoisting and typed multi-result operations show that valuable single-stream SFPU work fits
the GCC backend.  Continue GCC for Tracks A/D and the current interleave/LICM work.  This does not
prove that GCC is the right representation for multi-TRISC async dataflow; retain the Track-C/Track-E
ceiling trigger and reconsider MLIR when a concrete token schedule or scalar-to-vector lowering
requires disproportionate backend surgery.

### 19.6 Evidence-Gated Execution Directive

1. **Keep the landed pre-IRA Dst-iteration interleaver evidence-bounded.** Addcmul is closed from
   `+21.9%` to `+0.02%` parity with Dst non-aliasing, SSA/cache integrity, exact final-ELF order,
   byte-identical fallback, CRAQ correctness, and repeated Blackhole samples. Generalize only when
   another corpus row presents the same proven shape; do not call parity a win.
2. **Finish the first sound macro-emission slice.** *[DONE 2026-08-17 — this directive is
   executed: the generic planner at gcc `bb56f1d77` proves config-word ownership, hidden effects,
   opaque-owner exclusion, and byte-identical refusals, and craq-sim `f80a8d6` replaced the
   `fd8ed6f` admitted-shape model with generic descriptor decode; see §18.8.0.4.]*
   ~~CRAQ `fd8ed6f` provides the admitted WH/BH transactional event model; compiler emission must
   additionally prove all config words, function-scoped scratch/slot ownership, opaque-owner
   exclusion, hidden effects, and ineligible identity.~~
   Still good practice: pin the CRAQ repository commit and runner path in every result manifest so
   the cited model is reproducible outside the author's checkout.
3. **Silicon-score the landed invariant-placement + counted-replay pair.** Commits `2bfa165348` and
   `6422dbd9e3` now include the conservative eight-LREG budget, live-through/live-out accounting,
   opaque-owner refusal, counted-loop capture legality, and byte-identical fallback. Run the existing
   semantic Sigmoid lane with both flags, require changed executable `.text` plus CRAQ correctness,
   then accept or reject it from repeated scoped device cycles.
4. **Execute the named durable compiler A/B lane.** TT-Metal `da3832b31d`,
   `tt_metal/tt-llk/tests/corpus/sfpu_corpus.py`, is the 164-row / 332-path authority with exact
   pytest-node attribution, compiler capability/pin provenance, and identical-source flag-off/on
   executable-`.text` classification. Use `--require-changed-binary` only on eligible rows, retain
   byte identity as the expected ineligible fallback, use CRAQ for functional validation, and accept
   performance only from scoped device rows.
5. **Use MILP as a sampled bounded oracle.** Preserve exact-on-request behavior; after the latency,
   Dst, replay, or macro semantics being scored are actually modeled, compare list versus MILP and
   archive optimality plus compile-cost distributions before changing production invocation policy.
6. **Close architecture coverage.** Retain Blackhole as the current performance authority and add
   the corresponding Wormhole correctness and changed-binary silicon lane before making a
   cross-architecture default-on or superiority claim.

No transform is promoted on assembly aesthetics alone.  A sound but losing transform remains
opt-in or unshipped until its missing mechanism is implemented; broad superiority is claimed only
after the standing corpus and silicon gates demonstrate it.
