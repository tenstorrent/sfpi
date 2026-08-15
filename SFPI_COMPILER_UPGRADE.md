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
| **Corpus Scorer / Differential Driver** | **Scorer bring-up is real but narrow:** `scripts/run-corpus-score.sh` (245 lines) has one `scripts/corpus/welford-bh.tsv` entry and compares handwritten replay with generated vFloat. The required identical-source scheduler off/on whole-corpus differential is absent. | **P0:** `scripts/run-corpus-differential.sh`, multi-target LLK manifest, changed-binary classification, simulator/correctness legs, and selected silicon A/B |
| **Hardware Silicon Baseline** | **GO-BH-ONLY**: 3 generated wins (Welford 323 vs 326, Reduce-SDPA 834 vs 840, Reciprocal 459 vs 467), 1 tie (Binary broadcast 608), 6 understood throughput gaps (§18.8.0). Primary archive in `validation/welford-bh-20260815/`. | **Open:** Wormhole silicon and an identical-source, changed-binary pressure-scheduler A/B (§14). |

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

1. **The whole-corpus differential driver is absent.** `scripts/run-corpus-score.sh` is a useful
   artifact/profiling harness, but its current one-entry Welford manifest and handwritten-vs-
   generated comparison do not implement the required identical-source flag-off/flag-on LLK
   differential. `scripts/run-corpus-differential.sh` remains a P0 deliverable.
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
│       │                                  │           │ wins, 1 tie; scheduler A/B + WH remain open.│
├───────┼──────────────────────────────────┼───────────┼───────────────────────────────────────────┤
│ **P4**│ **Latency Scheduling**           │ Planned   │ No reorder pass exists. Target: recover  │
│       │                                  │ Sprint 1  │ Addcmul's measured +21.9% loss via 2-row  │
│       │                                  │           │ exposure/interleaving.                    │
├───────┼──────────────────────────────────┼───────────┼───────────────────────────────────────────┤
│ **P5**│ **Coprocessor, LICM & Macros**   │ Mixed     │ Replay loop hoist shipped opt-in; LICM / │
│       │                                  │ Sprint 2-3│ constant pinning planned; SFPLOADMACRO    │
│       │                                  │           │ remains emit=no and simulator-blocked.    │
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

**P4 is an implementation target, not a landed pass.** The checked-in
`pass_rvtt_schedule` inserts at most one correctness NOP after examining the next Tensix dependency;
it never reorders instructions. `pass_rvtt_lp_schedule` reorders only pressure-rescue regions with
`old_peak > 8`. Consequently there is no post-RA peak-at-most-eight latency list scheduler or
two-row interleaver in the tree. The recorded Addcmul `+21.9%` is a loss caused by the missing
interleave, not a win. P4 becomes “Active” only with a compiler commit, discriminating assembly
test, and paired silicon result.

**P5 must be tracked per mechanism.** Fixed-encoding replay loop hoisting is real, conservative,
opt-in, and has a changed-binary Reduce-SDPA silicon win. LICM/constant pinning is still a proposed
response to SigmoidAppx. `pass_rvtt_loadmacro` remains a default-off discovery pass with `emit=no`,
and novel formation is blocked on a simulator event model plus silicon validation. A single
“Active” label obscures these materially different maturity levels.

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

**2. The machine model / cost oracle** (`/root/craq-sim`):
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
> (`gcc/config/riscv/tt/…`), not the superproject. Functional/timing reference: `/root/craq-sim` — `src/tensix.cpp`
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
greenfield wholesale.

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
| **A. Finish the SFPU story** | Full SFPU ISA scheduling/alloc beyond add/mul/mad (LUT/transcendental, int, casts); predication/masking under CC divergence; software pipelining across loops; M2 exact allocator **only if** raw-LREG+IRA proves insufficient on the corpus (still a stub, §4). | Partly shipped | Match/beat handwritten **non-replay** LLK across the SFPU corpus on silicon. |
| **B. DST tile register + RWC** | Model the DST accumulator (fp16/fp32 layout) and RWC (read/write-clear) hazard tokens between matrix engine, SFPU, and pack. Eliminate the hand-written `Dst` round-trips §7 kernels use (log/GELU/erfinv dump state to `Dst`). Annotated raw-LREG ownership enforcement already ships; layout/RWC aliasing and its post-IRA verifier remain. | Partial | Kernels that spill to `Dst` by hand keep values resident; correctness + non-inferiority. |
| **C. Cross-engine scheduling** | Model matrix engine (FPU/matmul) + pack/unpack pipelines; coordinate the three TRISCs (unpack/math/pack) via semaphores/wait-gates; schedule across engine boundaries. **This is where GCC's tile/dataflow abstractions may hit a ceiling — MLIR reconsideration checkpoint (§8).** | Not started | An end-to-end unpack→matmul→SFPU→pack kernel scheduled by the compiler, measured **whole-kernel** on silicon. |
| **D. Replay / MOP / `SFPLOADMACRO` emission** | Existing post-RA replay formation ships, and conservative loop-capture hoisting is opt-in with a changed-binary Reduce-SDPA silicon win. General cross-BB replay allocation, MOP, and `SFPLOADMACRO` emission remain. | Partial | Compiler-emitted replay matches handwritten replay cycle count across the corpus; every novel formation is correctness-checked and silicon-scored. |
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

**Consequences for the roadmap.** (1) F1 must be re-scoped: CRAQ is **not** a performance oracle for SFPU-vs-replay; calibrate its SFPU/replay timing to silicon micro-benchmarks first, or make silicon A/B the scoring authority (§18.7 M-F1.4). (2) The `rvtt-cost.md` DFA is sound as a **NOP-placement / correctness** mechanism but its latencies must not be read as a perf predictor until recalibrated to silicon (not to CRAQ). (3) **Track D performance claims scored only via CRAQ are untrustworthy** because replay is exactly the frontend effect CRAQ mis-models. Existing replay formation remains shipped, and the conservative opt-in loop-hoist has its own changed-binary Reduce-SDPA silicon result; neither should be described as dump-only. Novel `SFPLOADMACRO` emission, broader cross-BB replay discovery, and future replay allocation changes still require independent correctness plus silicon performance gates. Keep only the shipped `pass_rvtt_loadmacro` formation analysis at `emit=no` until its simulator execution model and silicon validation path exist.

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
- **Single-stream ceiling is inherited, not solved.** The `rvtt-cost.md` DFA models one in-order pipe. It correctly carries per-engine latency and same-class structural hazards, but it **cannot** price cross-TRISC semaphore rendezvous or the implicit SrcA/SrcB bank-valid handshake — true multi-stream dataflow tokens with no RTL analog. F1 must therefore mark those stalls as *modeled-as-barrier* (conservative, non-free), not schedule through them. That boundary is exactly the **Track C GCC-ceiling / MLIR-reconsideration trigger** (§18.4 C, §18.5, `SFPI_COMPILER_UPGRADE.md:1393-1395`): if the corpus shows F1's single-stream cost model systematically mispredicting on multi-engine kernels because the dominant stalls are cross-thread, that is the documented signal to reconsider an MLIR async-token representation (§8) rather than force a fused-stream fiction into GCC's DFA. F1's honest scope is the SFPU single-pipe cost table; it must not over-claim whole-kernel accuracy.
- **Attribute migration regression.** Annotating the generated `type=tensix` patterns in `rvtt.md`
  risks silently changing NOP placement on already-validated kernels. Keep `type=tensix` intact so
  existing pass membership does not change, and gate F1.2 on bit-identical scheduling of the shipped
  Welford binary before the new latencies are allowed to differ elsewhere.

### 18.8 Track D — Replay / MOP / `SFPLOADMACRO` Emission (closes the ~4% gap)

**Status: frontend REPLAY = PARTIAL (shipped, single-BB); MOP compression = GREENFIELD; `SFPLOADMACRO` = GREENFIELD (blocked on sim).**

#### 18.8.0 Silicon Scorecard — Compiler-Generated vs Hand-Tuned LLK

Consolidated from the per-kernel notes below (and §13.3 for Welford). **All figures are Blackhole
device profiler-zone `*_BODY` cycles — not whole-kernel throughput; CRAQ deltas are excluded per the
§18.7 calibration failure.** Lower is better; Δ is generated relative to hand-tuned.

| Kernel | Hand-tuned LLK | Generated | Δ | Correctness | Outcome |
| :--- | ---: | ---: | ---: | :--- | :--- |
| **Welford** body | 326 | 323 | **−0.9%** (−3) | 15/15 | Parity / marginal — §14 will not credit it as a scheduler win (bounded zone, different source bodies) |
| **Binary broadcast** | 608 | 608 | **0.0%** | 8/8 BH, 6/6 WH compile | **Exact tie** — zero-regression compiler-flow replacement |
| **Reduce-SDPA** body | 840 (8-slot replay) | 834 | **−0.7%** (−6) | paired full golden | **Generated win** — generic D1 preheader capture hoisting flips the prior +2.0% loss |
| **Reciprocal, accurate BF16** | 467 | 459 | **−1.7%** (−8) | paired canonical tolerance + PCC, 2/2 BH | **Generated win** — fresh semantic cubic becomes a generic 10-slot replay capture plus seven playbacks |
| **Binary Min/Max** | 140.93 | 198.76 | **+41.0%** (+57.83) | paired element tolerance + PCC | **Loss** — ordinary load/load/swap/store replay cannot match the handwritten SFPLOADMACRO pipeline |
| **Addcmul** | 292.92 | 357.03 | **+21.9%** (+64.11) | paired element tolerance + PCC | **Loss** — one-row dependency chain versus handwritten two-row interleave |
| **Typecast, Float16_b lane** | 265 | 317 | **+19.6%** (+52) | paired element tolerance + PCC, 2/2 BH | **Loss** — typed five-slot replay versus handwritten SFPLOADMACRO pipeline |
| **TTNN Where** | 159.25 | 312.50 | **+96.2%** (+153.25) | 2/2 BH | **Loss** — correct canonical SFPI; 7 replay slots versus handwritten SFPLOADMACRO's 3 |
| **TopK** | 5038 | 5310 | **+5.4%** (+272) | exact value/index pairing, 2/2 BH | **Loss** — multi-result typed model is sound; final delivery/schedule remains longer |
| **SigmoidAppx** | 222.85 | 446.85 | **+100.5%** (+224) | `atol=.13`, `rtol=.05`, PCC > .99, 2/2 BH | **Loss** — fresh cubic rematerializes constants per row and forms no replay |

**Read-out: three generated wins are now measured.** Generic D1 replay hoisting moves Reduce-SDPA
from a 2.0% generated loss to a repeatable 0.7% generated win; fresh semantic accurate-BF16
Reciprocal is 1.7% faster because the ordinary compiler replay pass compresses its ten-instruction
body; Welford remains a marginal generated win. Binary broadcast ties exactly. The loss rows split
cleanly into missing SFPLOADMACRO formation (Binary Min/Max, Typecast, Where), missing cross-iteration
latency scheduling (Addcmul), loop-invariant constant/replay formation (SigmoidAppx), and a smaller
TopK delivery/schedule gap. The dump-only formation pass records the missing macro proofs without
emitting speculative code.

#### 18.8.0.1 Perf-Loss Root Cause — Why Correct-But-Slower

Reported corpus run (2026-08-15; per-kernel silicon instruction-diff artifacts pending in-repo):
Binary Min/Max **+41.0%**, Addcmul **+21.9%**, Typecast **+19.6%**, TopK **+5.4%** — all correct.

**Unifying diagnosis.** Register allocation and correctness are solid (the shipped, silicon-validated
layer); **every loss is a pure throughput gap** from two mechanisms that are not built, and a cost
model blind to both. Confirmed structurally in `rvtt-passes.def`:

- The reordering scheduler `pass_rvtt_lp_schedule` **gates entirely on `old_peak > 8`**
  (`gimple-rvtt-lp-schedule.cc:824,829`) — for a peak-≤8 kernel it does nothing.
- `pass_rvtt_schedule` **only inserts NOPs, never reorders** (header *"schedule tensix insns (insert
  nops)"*; `emit_insn_after(gen_rvtt_sfpnop())`, `rtl-rvtt-schedule.cc:252`).

So **there is no latency-hiding scheduler** (§5 / P4 unbuilt): a dependent instruction either pays
an explicit correctness NOP where the target erratum requires one or an implicit hardware scoreboard
stall, and the compiler cannot fill that issue slot with independent work. Combined with no
`SFPLOADMACRO` formation (path B, GREENFIELD/sim-blocked), that is the dominant loss story.

| Kernel | Loss | Hand-tuned uses | Compiler's error (class) |
| :--- | ---: | :--- | :--- |
| **Binary Min/Max** | +41.0% | load+compute+store fused across `SFPLOADMACRO`'s 4 sub-units | **No `SFPLOADMACRO`** → serial `SFPLOAD→SFPMAX→SFPSTORE`, load latency exposed (most load-bound → worst) |
| **Addcmul** | +21.9% | manual `MUL_a,MUL_b,MAD_a,MAD_b` interleave to hide the 2-cycle latency across 2 rows | **No latency scheduler/unroll-and-jam** → emits a one-row `MUL→MAD` chain; the next independent row is not exposed for interleaving |
| **Typecast** | +19.6% | `SFPLOADMACRO` load-convert-store pipeline (§7 "memory bound") | **No `SFPLOADMACRO`** → serial convert loop |
| **TopK** | +5.4% | tuned sort network with statically expanded cases | Typed helper is smaller but retains runtime loop/control flow; dynamic-path attribution is still required |

**The compiler's errors, ranked.**
1. **No latency-hiding reorder (§5 / P4 unbuilt).** Only a pressure scheduler (fires `>8`) + a
   NOP-inserter. → Addcmul, and a latency tax on every body.
2. **No `SFPLOADMACRO` formation (path B, GREENFIELD + sim-blocked).** ttsim only whitelists known LLK
   macro signatures (`tensix.cpp:9928-9992`), so novel emission can't yet be validated. → Min/Max,
   Typecast, Where.
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
latency scheduling with cross-iteration exposure, not more register allocation — the allocator is
already at parity.

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

**Re-scoping note vs §18.4.** The master roadmap lists Track D as "Not started"; this design upgrades the REPLAY leg to PARTIAL because it genuinely reuses the shipped `pass_rvtt_replay` (834 lines) and the `ttreplay` builtin — the MOP and `SFPLOADMACRO` legs remain GREENFIELD.

The authoritative Welford result is generated vFloat 323 versus replay LLK 326 device cycles on
Blackhole: generated is already ~0.9% faster on that body. It therefore does not establish a
remaining Welford replay gap and does not motivate scheduler default-on. Track D is motivated by
other corpus kernels and by general frontend compression opportunity; path A replay and path B
`SFPLOADMACRO` are independent datapaths and must not be conflated.

#### 18.8.1 Hardware mechanism (simulator semantics; silicon performance authority)

**Path A — MOP + REPLAY share ONE 32-slot circular buffer.** Every RISC-pushed Tensix instruction enters `tensix_push_inst` (`tensix.cpp:2666`), whose opcode switch routes `0x01→MOP`, `0x03→MOP_CFG`, `0x04→REPLAY`, else passthrough — all funneling into the single choke point `replay_expander` (`tensix.cpp:2408`). State is per-pipe (per-TRISC): `replay_buf[TENSIX_INST_PIPES][32]`, `replay_index`, `replay_left`, `replay_execute_while_loading` (`sim.h:502-507`). `replay_expander` has three state-keyed modes:

| Mode | Condition | Behavior | cite |
|---|---|---|---|
| **Capture** | `replay_left>0` | write `replay_buf[index]=inst`; `index++`; `replay_left--`; if `execute_while_loading` also push to exec FIFO same cycle | `tensix.cpp:2411-2424` |
| **Replay-cmd** | `replay_left==0 & op==0x04` | decode `load_mode=bit0`, `exec_while_loading=bit1`, `len=bits<13,4>`, `start_idx=bits<23,14>`; `load_mode=1` arms capture (`replay_index=start_idx; replay_left=len`), `load_mode=0` playback loops `len` pushes of `replay_buf[start_idx+i]` tagged `replay_emit` | `tensix.cpp:2425-2461` |
| **Passthrough** | else | straight to exec FIFO | `tensix.cpp:2463-2470` |

Hard bounds the emitter MUST honor: `1<=len<=32` (`tensix.cpp:2444`), `start_idx<32` (`:2446`), and `start_idx+len<=32` — **overflow is `UndefinedBehavior`** (`:2447`). MOP (`tensix.cpp:2559`) is a hardware loop nest that calls `replay_expander` on its 9 template slots `mop_cfg[pipe][0..8]` (`sim.h:498-499`), so MOP and REPLAY *compose on the same buffer* (a MOP whose loop-op is a REPLAY playback is legal). Expansion is deferred (`defer=true`, `:2699/2712`) against the executable-FIFO watermark of 31 (`tensix.cpp:487-494`, drained one/cycle by `tensix_advance_frontend_stream`, `:2728`). On playback the wait-gate block mask is **recomputed per backend instruction** (`tensix.cpp:2464-2469`), so replayed-body hazards are still enforced individually — the compiler does not re-declare per-body sync.

**Path B — `SFPLOADMACRO` (opcode `0x93`) is a different datapath**, not the 32-slot buffer. It reads a 4-entry `load_macro_instruction_template[4]` / `load_macro_sequence[4]` / `load_macro_misc` (`sim.h:587-589`) populated by `SFPCONFIG` writes (`config_dest 0..3→template, 4..7→sequence, 8→misc`, `tensix.cpp:9740-9757`); each `SFPLOADMACRO` does an implicit `SFPLOAD` into `LReg[VD]` re-dispatched as `0x70` (`tensix.cpp:9911-9927`) then schedules templated work across 4 sub-units. **ttsim models it functionally by whitelisting known LLK signatures** (reduce max/min `0x1b8400de`, int-invert `0x6300005d`, etc., `tensix.cpp:9928-9992`); unknown shapes raise `UnsupportedFunctionality`. A compiler emitting *novel* templates cannot be validated until the sim is generalized to a real 4-sub-unit event model — this is why Welford is closed via path A.

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

**`SFPLOADMACRO` is GREENFIELD and sim-blocked.** The `SFPCONFIG` insn ships (`UNSPECV_SFPCONFIG`, `rvtt.md:89,1945-1971`) so template/sequence/misc writes are emittable, but there is no `SFPLOADMACRO` builtin and — critically — the sim whitelists LLK signatures only (`tensix.cpp:9928-9992`). Prerequisite: generalize ttsim's `SFPLOADMACRO` to a real 4-sub-unit event model (the `sfpu_macro_region` descriptor, §6.2) before *any* compiler emission can be validated. Then: `new: rvtt_ttloadmacro` builtin + the §6.2 target-internal collision/drain checker.

#### 18.8.3 Staged milestones

- **D0 (PARTIAL→ship gap fix):** Extend `pass_rvtt_replay` slot allocation to reuse disjoint-lifetime spans (limitation #2) and add prefix-span selection (#3). Single-BB only. The interval-reuse logic is net-new (the M2 allocator is a dump-only stub). Gate: no correctness regression on the 5 Welford selectors.
- **D1:** Cross-BB / dominator-scoped sequence discovery with per-generation live-value guarding (removes limitation #1). This is the milestone that targets the 3-cycle Welford body.
- **D2:** Sequence-through-non-Tensix hoisting (limitation #4 / PR 36496) to stop spurious sequence termination.
- **D3 (MOP, greenfield):** `new: rvtt_ttmop` + emit `MOP_CFG`/`MOP`; teach the allocator MOP∪REPLAY share the 32-slot arena; select MOP over REPLAY for nested loops by word-count cost.
- **D4 (`SFPLOADMACRO`, greenfield, sim-gated):** ttsim 4-sub-unit event model first; then `new: rvtt_ttloadmacro` + `sfpu_macro_region` collision/drain checker; scoped to Typecast/MulInt/Where (§7 row `:830`).

#### 18.8.4 Hard gate (measurable)

**D0 gate (shipped/measurable now):** auto-replay compression on the 8-row unrolled mockup holds **88→19 static Tensix insns on WH and 56→15 on BH** (§6.1, `:807`, *Mockup Evidence*), and `WELFORD_BODY` on Blackhole silicon holds the pinned **323/323/323 device cycles** for N=1/2/32 vs replay-LLK's 326 (`:823`) — with all 5 Welford selectors passing correctness. Any `replay_buf` `start_idx+len>32` at emit is a hard fail (`UndefinedBehavior`, `tensix.cpp:2447`); the pass must prove `S+L<=32` as an allocation invariant.
**D1 loop-hoist gate (cleared on Blackhole):** fixed-encoding replay capture hoisting changes
Reduce-SDPA from handwritten `840` versus generated `855.5` to handwritten `840` versus generated
`834` scoped device cycles, with paired correctness and three zero-spread processes per arm.  The
implementation remains opt-in and conservatively single-block-loop only.  Broader cross-BB span
discovery for Welford is still open and must independently satisfy the same changed-binary silicon
gate; the Reduce result does not imply that unimplemented transform exists.
**D4 gate (target, sim-blocked):** `SFPLOADMACRO`-lowered Typecast/MulInt/Where run on ttsim **without** `UnsupportedFunctionality` and hit a **≥1.33× steady-state issue rate** — where 1.33× is drawn from the §6.2/§7 "opportunity/potential" range (`:815,830`) and has not been measured; it is a target contingent on the D4 sim work landing first.

#### 18.8.5 Risks / ceiling

- **Emit-ordering invariant (correctness-critical):** an arm (`load_mode=1`) must be followed *immediately in program order* by exactly `len` pushes before any playback — `replay_expander` in capture mode swallows everything until `replay_left==0` (`tensix.cpp:2411-2424`). The post-reload pass must never let another REPLAY or a scheduler move interleave between arm and body. Running `pass_rvtt_replay` after `pass_postreload` (`:54`) is correct precisely because no reordering pass follows.
- **32-slot arena is a hard allocation constraint** shared across ALL live captured bodies per pipe (and, at D3, across MOP too). Over-allocation is silent `UndefinedBehavior` in the sim, not a diagnostic — the allocator carries the entire correctness burden, and that allocator is net-new (the shipped `rtl-rvtt-lp-alloc.cc` is a dump-only stub).
- **User-reservation contract:** LLK hand-kernels reserve slots and there is **no global sim registry** (`sim.h:502`); reservations must be a compiler-known descriptor (the §7 "explicit … ownership metadata … no global reservation" model, `:823`), mirrored on replay slots. Discovery-based reservation (the shipped "not used anywhere in the function" heuristic, `:48`) is sound only within a compilation unit — cross-TU LLK reservations need the metadata ABI.
- **Cross-BB live values (D1):** the pass author's own note (`:44-45`) flags that cross-BB replay needs better live-value computation for synthesized insns; getting generation-tracking wrong replays a stale-input body → silent numeric error, not a crash.
- **`SFPLOADMACRO` ceiling:** blocked entirely on ttsim being a pattern-matcher, not an executor (`:9928-9992`). Until D4's sim work lands, path B is unvalidatable — do **not** ship compiler-emitted novel macro templates. This is a sim-fidelity gate, not a GCC-IR ceiling (Track D fits GCC's post-RA peephole/allocation model cleanly; unlike Track C, there is no MLIR-reconsideration trigger here — replay packing is a single-stream, single-pipe problem the shipped pass already proves tractable in RTL).

### 18.9 Track B — DST Tile Register + RWC Hazard Model

**Status: PARTIAL.** The `_lv` live-value forwarding machinery, the LREG hard-register model, and the pattern combiner already ship in the sfpi rvtt backend and delete same-scope Dst round-trips today. What is GREENFIELD is the *format/layout-aware* and *RWC-index-aware* extension: proving a store→reload identity across the moving `dst_rwc` base and the CFG-state layout mode (Fp32 / 16b / int8), so cross-op and cross-region Dst spills in log/GELU/erfinv fold instead of surviving as `unspec_volatile` barriers.

**Re-scoping note vs §18.4.** The master roadmap lists Track B as "Not started"; this design upgrades it to PARTIAL because the `_lv` variants, enforcing `pass_rvtt_lreg_livein`, and the `rvtt.gc` combiner are genuinely shipped. The layout/RWC-alias extension and a post-IRA ownership verifier are net-new work. The M2 physical allocator remains a separate pressure/colorability project, not a prerequisite for the sentinel mechanism that already protects annotated raw-LREG ownership.

#### 18.9.1 Hardware mechanism (craq-sim ground truth)

DST is not flat storage — it is a physically-banked 16-bit tile register file with a parallel valid bitmap, addressed through a *moving* per-pipe counter, in a layout chosen by *global CFG state*. Four facts the compiler must model:

1. **Paired-row 32-bit layout.** Backing store is `uint16_t dst[1024][16]` + `bool dst_row_valid[1024]` (`sim.h:546-547`, `DST_ROWS=1024 ROW_SIZE=16` at `sim.h:217-219`). A 32-bit element is split across two rows 8 apart: `read_dst32b = (dst[adj][col]<<16)|dst[adj+8][col]`, `write_dst32b` writes `dst[adj]=data>>16; dst[adj+8]=data&0xFFFF` (`tensix.cpp:3503-3520`). Rows are bit-permuted by `dst32b_adjust_row`/`dst16b_adjust_row` (`tensix.cpp:3490-3501`) — a "Dst row index" is **not** a linear address.

2. **Layout is CFG-state, not value-carried.** `use_dst32b = ALU_ACC_CTRL_Fp32_enabled || ALU_ACC_CTRL_INT8_math_enabled || dst_32bit_addr_en` (`tensix.cpp:3822-3823, 3876-3877`; debug bit `sim.h:605` set at `tensix.cpp:1554-1556`). SFPU uses its own predicate `sfpu_dst32_layout = dst_32bit_addr_en || ALU_ACC_CTRL_Fp32_enabled`, and an SFPLOAD of a BF16 element pulls the top half `read_dst32b()>>16` in FP32-acc mode even with the debug bit clear (`tensix.cpp:8461-8484`). A store and its reload must agree on layout or the bits differ.

3. **RWC = the moving base + hazard token.** Per-pipe `dst_rwc[pipe]`/`dst_rwc_cr[pipe]` (`sim.h:569-570`) index DST. Math row = `dst_rwc + DEST_TARGET_REG_CFG_MATH_Offset + DEST_REGW_BASE_Base` (`tensix.cpp:3390-3391`); SFPLOAD adds `dest_reg_addr` on the same base (`tensix.cpp:8437-8442`); pack uses `DEST_TARGET_REG_CFG_PACK_SEC*_Offset` (`tensix.cpp:5970-5980`). ADDR_MOD mutates the counter via `math_update_rwc` (incr/clr/cr/c_to_cr, wrap=`DST_ROWS`, `tensix.cpp:3236-3249`, called at `3356`); `SETRWC`/`INCRWC` set/bump it (`tensix.cpp:5394-5401, 5443-5448`). **Program order + counter state, not a static register number, decides which physical rows alias.**

4. **Write→read enforced by the valid bitmap.** `read_dst32b`/`read_dst16b` return 0 (0xFFFF gmpool) when `!dst_row_valid[adj]` (`tensix.cpp:3506-3508, 3528-3530`); writes set it (optionally only on `col==15`, `3517-3519`); ZEROACC clears ranges (`tensix.cpp:3992-4057`). This is the matrix-writes-then-SFPU/pack-reads ordering edge that must survive as a true dependency.

**Why the round-trips exist (sfpi §7):** SFPU vFloat values live in 8 architectural LRegs; transcendentals (log/GELU/erfinv) that exceed LReg pressure SFPSTORE an intermediate to a Dst row and SFPLOAD it back. Physically a store-then-load of the same row with no intervening writer is identity — but because the row is chosen through the moving RWC base and the format depends on CFG state, the compiler cannot fold it without modeling both.

#### 18.9.2 What the GCC backend must model / emit, and where

The shipped rvtt backend already implements A and B; C and D are the Track-B delta.

**A. LReg live-value forwarding (SHIPPED).** Every SFPU op has a non-live and an `_lv` variant taking the prior Dst/LReg contents as an extra input (`rvtt-insn.def:152-163`; "`_lv` MUST follow the non-live version" `rvtt-insn.def:23`). `rvtt_sfpload` always expands into `rvtt_sfpload_lv` threading a `noval` placeholder for the live operand (`rvtt.md:575-591`; live operand class `reg_or_cstlreg_or_noval_operand`, `rvtt.md:561,616`). This makes a store's value and a later load's result the same pseudo, so a redundant store/reload becomes a copy. The combiner that cancels the chains is `pass_rvtt_combine` (gimple, in `rvtt-passes.def`) driven by `rvtt.gc` patterns (`sfpassign_lv`/`sfpnot_lv`/`sfpmul_lv` fusions, `rvtt.gc:25-230`); DCE is `pass_rvtt_dce`/`pass_rvtt_noval_elide`.

**B. LREG hard-register visibility to IRA (SHIPPED AND ENFORCING FOR ANNOTATED OWNERSHIP).** `pass_rvtt_lreg_livein` (`rtl-rvtt-lreg-livein.cc`) turns raw `sfpreadlregN`/`sfpwritelregN` (`rvtt.md:159-221`) and the `sfprawlreg_access` ownership marker (`release_mask`/`write_mask`, parsed `lreg-livein.cc:81-93`) into sentinel def/uses (`make_sentinel`/`emit_sentinel_*`, `lreg-livein.cc:95-119`) so IRA sees a normal live interval and will not reuse a raw-owned LREG as a temp. Its `execute` method calls `make_raw_lregs_live`; this is an RTL mutation, not a dump. The nearby `rvtt-passes.def` comment about dump-only coloring enforcement refers to the adjacent `pass_rvtt_lp_alloc` audit stub, although its placement is ambiguous and should be clarified in code. Scope remains explicit: annotated ownership is protected; arbitrary unannotated opaque asm is intentionally not decoded or inferred.

**C. Dst layout as an INSN-level mode (new).** The store and reload must agree on layout for the forward to be legal (mechanism pt 2). Today `sfpload`/`sfpstore` already carry opcode + src/dst shift + mod operands (`rvtt.md:553-573`); Track B adds a **layout attribute** `dst_layout ∈ {fp32,bf16,int8}` derived from the CFG-write reaching the op (`ALU_ACC_CTRL_*` / `dst_32bit_addr_en`). The combiner (`rvtt.gc`) must gain a guard that **refuses** to fuse a store/reload pair when an `ALU_FORMAT_SPEC`/`ALU_ACC_CTRL`/`dst_32bit_addr_en` CFG write lies between them and changes the layout — because layout is CFG-state, this is a reaching-definition check, `new: rvtt.gc` guard predicate `dst_layout_stable_p`, not a value comparison.

**D. RWC index + valid-bitmap as a precise alias model (new).** Currently `sfpload`/`sfpstore` are `unspec_volatile` (`rvtt.md:555,577`) precisely to pin them against reorder. Track B's job is to **relax** that volatility into a precise def/use keyed on the *symbolic RWC index* `(dst_rwc_base, math/pack offset, dest_reg_addr)` (mechanism pt 3), so (i) the scheduler may move Dst ops whose indices provably disjoint, and (ii) the combiner may cancel a store/reload only when the intervening `dst_rwc` base and offsets are provably unchanged — i.e. no `SETRWC`/`INCRWC`/ADDR_MOD-with-incr/clr and no ZEROACC over the row between them. The write→read valid-bitmap edge (pt 4) is *retained* as a true dependency (matrix/SFPU write before pack/SFPU read). Implementation: `new: rtl-rvtt-dst-alias.cc` (`new: pass_rvtt_dst_alias`, inserted before `pass_rvtt_schedule` in `rvtt-passes.def`) that attaches an RWC-index MEM-alias set to each Dst op and emits the fold; `adjust_cost` in the DFA (Track C/F1) reads the resulting deps.

#### 18.9.3 Staged milestones

| # | Milestone | Deliverable | Verify against |
|---|-----------|-------------|----------------|
| B0 | Baseline capture | Count surviving SFPSTORE/SFPLOAD Dst round-trips in log/GELU/erfinv on today's backend | craq-sim device-cycle + static insn count |
| B1 | Layout attribute | `dst_layout` attr on `sfpload`/`sfpstore`; `dst_layout_stable_p` guard in `rvtt.gc` | store/reload across a `ALU_ACC_CTRL` flip is **not** folded |
| B2 | RWC-index alias model | `new: rtl-rvtt-dst-alias.cc` attaches symbolic `(base,offset,addr)` index; relax `unspec_volatile`→precise def/use | fold only when RWC base + offsets provably unchanged; write→read valid edge preserved |
| B3 | Verify enforced LREG ownership | Add a post-IRA verifier for the already-enforcing `pass_rvtt_lreg_livein`; fail checking builds if an annotated raw-owned LREG is reused inside its sentinel interval. Clarify the ambiguous `rvtt-passes.def` comment. This is not blocked on M2. | no IRA reuse of a raw-owned LREG (mask from `sfprawlreg_access`), with a discriminating negative regression |
| B4 | Round-trip elimination | Combiner cancels proven-identity store/reload pairs in the three transcendentals | numeric identity on craq-sim; round-trip count → 0 where legal |

#### 18.9.4 Hard gate (measurable)

On craq-sim (Blackhole target), for the log, GELU, and erfinv SFPU kernels: **(1)** every store→reload of the same Dst row with no intervening RWC-base change, layout flip, or ZEROACC is eliminated (static SFPSTORE/SFPLOAD Dst round-trip count strictly lower than B0, and zero for the provably-identity cases); **(2)** bit-exact numeric output vs. the pre-optimization build across the tile; **(3)** no regression on the retained matrix-write→pack-read valid-bitmap ordering (no read of a stale/`!dst_row_valid` row); **(4)** device-cycle count ≤ B0 baseline. A fold that changes any output bit or violates a valid-bitmap edge fails the gate. Note B4 (the round-trip elimination that clears the gate) does not require B3; B1/B2/B4 are buildable ahead of the M2-blocked coloring-enforcement promotion.

#### 18.9.5 Risks

- **B3 verifies an enforcement mechanism that already ships.** Its risk is incomplete annotation or incorrect interval endpoints, especially across joins, loops, and multi-predecessor CFGs. The post-IRA verifier must diagnose overlap in checking builds, and the regression must be discriminating: deliberately remove or corrupt a sentinel and prove that the verifier catches the reuse. M2 may later replace or supplement baseline IRA for general pressure coloring, but B3 does not wait for it.
- **Layout inference is a reaching-definition problem across CFG writes.** `ALU_ACC_CTRL_*` and `dst_32bit_addr_en` are set by cfg writes possibly far from the SFPU op (`tensix.cpp:1554-1556`); if the analysis is imprecise the combiner must conservatively *refuse* the fold — safe but leaves round-trips. Mitigation: default-deny in `dst_layout_stable_p`.
- **RWC base is mutated as an ADDR_MOD side effect**, not an explicit operand (`math_update_rwc` at `tensix.cpp:3356`); missing one mutator (SETRWC/INCRWC/cr/c_to_cr/ZEROACC) between store and reload folds an aliasing pair and corrupts data. This is the primary correctness risk — the B2 alias model must treat *any* unmodeled RWC-touching op as a barrier.
- **Bit-permuted paired-row addressing** (`dst32b_adjust_row`, `tensix.cpp:3490-3501`) means the symbolic row index and the physical `adj_row` differ; disjointness must be proven on the *permuted* address, not the nominal row, or two "different" indices may alias the same bank. (CFG bitfield layout for `ALU_ACC_CTRL_*` is read directly from `tensix.cpp`, not a separate `tensix_regs.h` — repoint any anchor accordingly.)

### 18.10 Track C — Cross-Engine (matrix + pack/unpack + 3-TRISC) Scheduling

**Status: GREENFIELD.** Nothing in the shipped `sfpi` backend models cross-engine timing. Generated
Tensix commands live in `tt/rvtt.md`; the 87 `ttrocc.md` patterns are a separate QSR RISC-V RoCC
interface and must not be classified using Tensix FIFO opcode ranges. The existing rvtt passes
(`tt/rvtt-passes.def`: `pass_rvtt_check_early/immvar_expand/synth_split/noval_elide/synth_cse/dce/...`)
are all single-stream SFPU-lowering passes with no notion of engines or TRISC threads, while most
handwritten matrix/unpack/pack commands enter as opaque `.ttinsn` assembly. This track is the one
that tests whether GCC's IR can carry tile/dataflow scheduling at all, and it is the documented
MLIR-reconsideration checkpoint (SFPI_COMPILER_UPGRADE.md:1381, 1393–1395).

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

Modeling these forces one of two bad shapes: **(a)** three separately-compiled TRISC units with the compiler blind to the joint schedule (no cross-engine optimization — fails the whole-kernel gate at SFPI_COMPILER_UPGRADE.md:1381), or **(b)** a fictitious fused single stream with barrier pseudos, which serializes away the very concurrency the hardware exists to exploit. This is the documented §8 TT-Vector-dialect trigger (SFPI_COMPILER_UPGRADE.md:834–870, 1393–1395): async tokens / multiple concurrent value streams are the natural representation.

#### 18.10.3 Staged milestones

- **C0 — Issue-class model (GCC-clean, lands first).** Extend F1's orthogonal issue-class model to
  compiler-visible matrix/unpack/pack RVTT patterns as those patterns are introduced; do not change
  `type=tensix` membership and do not touch QSR `ttrocc.md`. Deliverable: the scheduler models the
  one-per-class-per-cycle structural hazard within a **single** pipe's represented stream. No
  cross-thread reasoning yet.
- **C1 — Single-stream fused block (requires Track B2).** With DST/RWC hazard tokens landed (§18.9), emit and schedule a **fused** unpack+matmul+SFPU+pack straight-line region (no semaphore rendezvous — one logical thread) and verify the bank-valid deps are honored. C1 needs Track B's alias model (B2); it is not transitively blocked on M2 because annotated raw-LREG enforcement already ships and B3 is now a verification backstop. This is the honest ceiling of what RTL carries.
- **C2 — Multi-TRISC rendezvous (the ceiling test).** Attempt whole-kernel scheduling across the 3 pipes with `SEMPOST`/`SEMWAIT` handoffs. Represent semaphores as **new: `__builtin_rvtt_sem_post/sem_wait`** intrinsics (new `unspecv` RVTT patterns, modeled `unspec_volatile` so they pin) and a **new: `rtl-rvtt-crossthread-sched` pass** that carries cross-stream happens-before edges. **This milestone is the go/no-go for MLIR** — if C2 needs a fictitious fused stream or blind per-thread compilation to build, the trigger fires.

#### 18.10.4 Hard gate

**An end-to-end unpack→matmul→SFPU→pack kernel scheduled by the compiler, measured whole-kernel on silicon, non-inferior to the handwritten LLK** (SFPI_COMPILER_UPGRADE.md:1381). Measured, not asserted: device cycle count on the real kernel, same corpus/oracle harness as §18.5 (F1). C0 alone does not clear the gate — it is validated only that the DFA schedule matches the arbiter's per-class issue order on a single pipe (compare against `tensix_rtl_issue_class_for_inst` traces). The gate is cleared only at C2 with the joint schedule beating (or tying) handwritten whole-kernel cycles. The prerequisite is Track B2's DST/RWC alias model plus verified ownership enforcement; M2 becomes a dependency only if an actual fused corpus case demonstrates a physical-coloring failure that baseline IRA cannot resolve.

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
└────────────────────────────┴─────────────────────────────┴─────────────────────────────────────────────────┘
```

These are meaningful proofs of viability, not corpus-wide dominance.  They are bounded body zones,
not whole-kernel throughput; Welford is also not an identical-source scheduler A/B.  Wormhole and
most of the 164-row corpus remain unmeasured against hand-tuned implementations on silicon.

### 19.2 M2 Is Not the Measured Blocker; It Is Not Universally Obsolete

The current measured losses do not show allocator spilling as their cause.  The evidence supports
keeping M2 on standby, with a concrete trigger rather than a rhetorical conclusion:

1. **GCC IRA is sufficient for the tested regions.** The `x<N>` constraints, multi-result RTL, and
   raw-LREG sentinels protect the live intervals exercised by the currently measured kernels.
2. **The sentinel mechanism is real but its provenance must be stated correctly.**
   `rtl-rvtt-lreg-livein.cc` models opaque LREG ownership; `8f943c2f8` is the predicate DEBUG-use
   fix, not the sentinel-pass commit.
3. **M2 remains a standby audit stub.** Activate it only when a reviewed corpus case proves that IRA
   spills, reloads, or fails to color a semantically valid region after raw-LREG modeling is correct.
   The present data do not prove that every future high-pressure or cross-engine kernel is spill-free.

### 19.3 Correcting the Throughput Diagnoses

The §18.8.0 scorecard contains six explicit loss rows (with Binary Min/Max combined), not five,
and additional measured semantic lanes such as Exp still need to be folded into that table.  The
current artifacts do not implicate allocation in these losses, but each proposed throughput fix
still needs changed-binary silicon acceptance.

```
┌──────────────────────────────┬───────────────┬─────────────────────────────────────────────────────────────┐
│ Identified Bottleneck        │ Impacted Ops  │ Concrete Architectural Remediation                          │
├──────────────────────────────┼───────────────┼─────────────────────────────────────────────────────────────┤
│ **1. Lack of Latency-Hiding  │ Addcmul       │ Form and interleave a two-row group before IRA.              │
│    Reorder Scheduling**      │ (+21.9%)      │ Interleave independent row dependency chains (MUL_a, MUL_b, │
│                              │               │ MAD_a, MAD_b) to hide the 2-cycle SFPU result latency.      │
├──────────────────────────────┼───────────────┼─────────────────────────────────────────────────────────────┤
│ **2. Missing SFPLOADMACRO    │ Min/Max (+41%)│ Use the audited CRAQ event model, then prove complete config │
│    Formation**               │ Typecast      │ ownership, hidden effects, and byte-identical fallback in   │
│                              │ (+19.6%)      │ active multi-op macro region emission.                      │
│                              │ Where (+96.2%)│                                                             │
├──────────────────────────────┼───────────────┼─────────────────────────────────────────────────────────────┤
│ **3. Loop-Invariant Constant │ SigmoidAppx   │ Hoist proven invariant constants and let IRA choose their   │
│    Rematerialization (LICM)**│ (+100.5%)     │ registers; do not hard-code L6/L7.                          │
├──────────────────────────────┼───────────────┼─────────────────────────────────────────────────────────────┤
│ **4. Cost Model Calibration**│ General       │ Model 1/cycle issue throughput separately from 2-cycle     │
│                              │ Schedule      │ result latency; calibrate with Blackhole silicon A/B.        │
└──────────────────────────────┴───────────────┴─────────────────────────────────────────────────────────────┘
```

### 19.4 Exact MILP as a First-Class Optimization Engine

The checked-in MILP is not merely an emergency fallback for cases where the deterministic list
scheduler fails. Its exact issue-position and liveness model, bounded region size, node cap, and
independently validated certificate make it the natural optimization oracle for the SFPU roadmap.
The 11-to-8 fixture already proves that exact search can recover a legal schedule outside the
heuristic's reach.

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

Run this exact lane on every bounded eligible corpus region in CI/oracle mode, including list hits,
and publish heuristic-versus-optimal gaps plus solver node/time distributions. The fast list
scheduler remains valuable for low compile latency; it does not define the quality ceiling.
Production may invoke MILP for all bounded regions, selected profitable shapes, or explicit
optimization modes depending on those measurements. Do not hard-code list-miss-only policy before
collecting the data.

This exact engine should also consume Sprint 1's pre-IRA two-row Addcmul groups. Once both independent
chains are visible, the MILP can jointly prove pressure legality and choose the minimum-latency
interleave instead of introducing another isolated greedy scheduler. M2 physical coloring remains
a distinct downstream mechanism and must not be conflated with the scheduling MILP.

### 19.5 GCC Is the Near-Term Path, with an Explicit Ceiling

Replay hoisting and typed multi-result operations show that valuable single-stream SFPU work fits
the GCC backend.  Continue GCC for Tracks A/D and the current interleave/LICM work.  This does not
prove that GCC is the right representation for multi-TRISC async dataflow; retain the Track-C/Track-E
ceiling trigger and reconsider MLIR when a concrete token schedule or scalar-to-vector lowering
requires disproportionate backend surgery.

### 19.6 Evidence-Gated Execution Directive

1. **Elevate the exact scheduler.** Preserve exact-on-request behavior, add the staged objectives in
   §19.4, run bounded list-versus-MILP corpus comparisons, and archive optimality and compile-cost
   distributions. Independent validation remains authoritative for every emitted schedule.
2. **Finish pre-IRA Dst-iteration interleaving.** Require Dst non-aliasing, SSA/cache integrity,
   exact final-ELF order, byte-identical fallback, CRAQ correctness, and three serialized Blackhole
   samples. Feed the exposed two-row DAG to both list and exact scheduling. Addcmul's `+21.9%` is a
   loss to close, not a delivered win.
3. **Finish the first sound macro-emission slice.** CRAQ `fd8ed6f` provides the admitted WH/BH
   transactional event model; compiler emission must additionally prove all config words, function-
   scoped scratch/slot ownership, opaque-owner exclusion, hidden effects, and ineligible identity.
   Pin the CRAQ repository commit and runner path in the result manifest so the cited model is
   reproducible outside the author's checkout.
4. **Build operation-independent invariant placement plus counted-loop replay.** Reject unproven
   MEM/GPR/config/CC/call/opaque-asm barriers and let IRA allocate coefficients.
5. **Name and extend the durable corpus runner.** The in-tree `scripts/run-corpus-score.sh` currently
   has one Welford manifest entry; the broader TT-Metal corpus evidence must name its repository,
   path, commit, manifest, and exact pytest nodes. Record compiler capability/pin provenance, use
   CRAQ for functional validation, and accept performance only from scoped device rows.
6. **Close architecture coverage.** Retain Blackhole as the current performance authority and add
   the corresponding Wormhole correctness and changed-binary silicon lane before making a
   cross-architecture default-on or superiority claim.

No transform is promoted on assembly aesthetics alone.  A sound but losing transform remains
opt-in or unshipped until its missing mechanism is implemented; broad superiority is claimed only
after the standing corpus and silicon gates demonstrate it.
