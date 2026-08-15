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

### 2.2 Baseline State vs. P0 Target Architecture

| Dimension | Checked-In Checkpoint | P0 Target Architecture |
| :--- | :--- | :--- |
| **Pressure Scheduler Flag** | `Init(0)` (Explicit opt-in required) | `Init(1)` for existing WH/BH gate + negative rollback option |
| **List Scheduler** | Implemented & independently validated | Primary fast-path rescue attempt (<0.1ms compile time) |
| **MILP Invocation** | Explicit second flag (invoked on all high-pressure) | Demand-driven escalation **only on list failure** (100k-node cap) |
| **Fallback Behavior** | Fails back to unmutated GIMPLE | Fails back to unmutated GIMPLE |
| **JIT Cache Integration** | Version-string hashed | Compiler flags & scheduler state hashed in build key |

```
Candidate Region (Peak > 8)
       │
       ├──► 1. Fast Path: Deterministic List Scheduler (<0.1ms compile time)
       │         │
       │         └──► [Valid Peak <= 8] ──► Commit GIMPLE Rewrite
       │
       ├──► 2. Exact Path: Bounded MILP Solver (Demand-driven on List-miss; hard 100k-node cap)
       │         │
       │         └──► [Valid Peak <= 8] ──► Commit GIMPLE Rewrite
       │
       └──► 3. Safe Fallback: Leave GIMPLE untouched (preserves legacy compilation state)
```

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
│ 11. Stage replacements via validate_change() with standalone 0 watermark│
│ 12. Run independent validator on fully staged patterns                  │
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

struct certified_destructive_tie {
    unsigned result_val;
    unsigned operand_val;
    unsigned op_index;
    int insn_pos;
    int operand_death_pos;
    tie_kind kind;
    int selected_alternative;
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

// Authoritative Destructive Tie Certification using GCC 15 Constraint APIs
bool certify_destructive_tie(rtx_insn *insn,
                             int insn_pos,
                             unsigned result_val,
                             unsigned operand_val,
                             unsigned op_index,
                             int operand_death_pos,
                             tie_kind kind,
                             int selected_alternative,
                             certified_destructive_tie &out_tie) {
    if (operand_death_pos != insn_pos) return false; // Must die exactly at issue boundary
    
    extract_insn(insn);
    int icode = INSN_CODE(insn);
    if (icode < 0) return false;

    int n_alts = insn_data[icode].n_alternatives;
    if (selected_alternative < 0 || selected_alternative >= n_alts) return false;
    if (op_index >= (unsigned)insn_data[icode].n_operands) return false;

    // Verify mode equality between destination and dying operand
    if (GET_MODE(recog_data.operand[0]) != GET_MODE(recog_data.operand[op_index])) {
        return false;
    }

    if (kind == tie_kind::MANDATORY_2ADDR || kind == tie_kind::LV_PREDICATION) {
        out_tie.result_val = result_val;
        out_tie.operand_val = operand_val;
        out_tie.op_index = op_index;
        out_tie.insn_pos = insn_pos;
        out_tie.operand_death_pos = operand_death_pos;
        out_tie.kind = kind;
        out_tie.selected_alternative = selected_alternative;
        return true;
    }
    return false;
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
            return false; // Duplicate pseudo entries forbidden
        }
        for (size_t j = 0; j < num_vals; ++j) {
            if (raw_interference[i][j] != raw_interference[j][i]) return false; // Symmetry
        }
    }

    std::unordered_set<unsigned> tied_results;
    for (const auto& tie : ties) {
        if (tie.result_val >= num_vals || tie.operand_val >= num_vals) return false;
        if (tie.insn_pos < 0 || tie.insn_pos >= (int)num_positions) return false;
        if (tie.operand_death_pos != tie.insn_pos) return false;
        if (!tied_results.insert(tie.result_val).second) {
            return false; // Duplicate tie for single result forbidden
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

    std::function<bool(size_t)> backtrack = [&](size_colored_count) -> bool {
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

// Global Function-Level Pseudo Closure Check (with Explicit Debug RTL Reset)
bool verify_global_pseudo_closure(function *fn,
                                  const std::vector<rtx_insn*>& island,
                                  const std::unordered_set<unsigned>& selected_pseudos) {
    std::unordered_set<rtx_insn*> island_insns(island.begin(), island.end());
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
                    // Reset stale debug use to avoid referencing substituted pseudo
                    INSN_VAR_LOCATION_LOC(use_insn) = gen_rtx_UNKNOWN_VAR_LOC();
                    df_insn_rescan(use_insn);
                } else {
                    return false; // Semantic use outside island!
                }
            }
        }
    }
    return true;
}

// Independent Precommit Staged Pattern Validator
bool verify_staged_island_patterns(const std::vector<rtx_insn*>& sfpu_island,
                                  const std::unordered_map<unsigned, unsigned>& regno_to_lreg,
                                  const std::vector<rtl_interval>& raw_intervals,
                                  const std::vector<std::vector<bool>>& raw_interference,
                                  const std::vector<certified_destructive_tie>& ties) {
    // 1. Verify every instruction in island remains recognizable
    for (rtx_insn *insn : sfpu_island) {
        if (recog(PATTERN(insn), insn, NULL) < 0) return false;
    }

    // 2. Verify no interfering intervals share a physical register
    size_t n = raw_intervals.size();
    for (size_t u = 0; u < n; ++u) {
        for (size_t v = u + 1; v < n; ++v) {
            if (raw_interference[u][v]) {
                unsigned r_u = regno_to_lreg.at(raw_intervals[u].pseudo_regno);
                unsigned r_v = regno_to_lreg.at(raw_intervals[v].pseudo_regno);
                if (r_u == r_v) return false;
            }
        }
    }

    // 3. Verify all certified destructive ties share the identical physical register
    for (const auto& tie : ties) {
        unsigned r_res = regno_to_lreg.at(raw_intervals[tie.result_val].pseudo_regno);
        unsigned r_op = regno_to_lreg.at(raw_intervals[tie.operand_val].pseudo_regno);
        if (r_res != r_op) return false;
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
            std::vector<certified_destructive_tie> ties;
            extract_rtl_constraint_model(sfpu_island, raw_intervals, raw_interference, ties);

            std::unordered_set<unsigned> selected_pseudos;
            for (const auto& iv : raw_intervals) {
                selected_pseudos.insert(iv.pseudo_regno);
            }

            // Pre-staging Global Function-Level Closure Verification
            if (!verify_global_pseudo_closure(fn, sfpu_island, selected_pseudos)) {
                telemetry.closure_reject_count++;
                continue;
            }

            std::vector<unsigned> final_reg_mapping;
            m2_solve_status status = solve_m2_exact_coloring(sfpu_island.size(), raw_intervals, raw_interference, ties, final_reg_mapping);
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

            for (rtx_insn *cur_insn : sfpu_island) {
                subrtx_ptr_iterator::array_type array;
                FOR_EACH_SUBRTX_PTR(iter, array, &PATTERN(cur_insn)) {
                    rtx *loc = *iter;
                    if (*loc && REG_P(*loc) && !HARD_REGISTER_P(*loc)) {
                        unsigned p_regno = REGNO(*loc);
                        auto it = regno_to_lreg.find(p_regno);
                        if (it != regno_to_lreg.end()) {
                            rtx hard_reg = gen_raw_REG(GET_MODE(*loc), SFPU_REG_FIRST + it->second);
                            if (!validate_change(cur_insn, loc, hard_reg, /*unique=*/true)) {
                                stage_ok = false;
                                break;
                            }
                        }
                    }
                }
                if (!stage_ok) break;
            }

            // Precommit Verification on Fully Staged Patterns
            if (stage_ok && verify_changes(0)) {
                if (verify_staged_island_patterns(sfpu_island, regno_to_lreg, raw_intervals, raw_interference, ties)) {
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
| **Welford (LayerNorm)** | 8 live values across 4 rows with zero register slack. | Recomputes delta ($\delta_2$) and hand-colors L0–L7. | **Demonstrated:** 9-to-8 rescue matches manual early fold; production replacement is P3 gate. |
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

---

## 9. Reconciled Phased Execution Plan (P0 – P5) & Validation Gates

```
┌──────────────────────────────────────────────────────────────────────────────────────────────────┐
│                                 RECONCILED EXECUTION ROADMAP                                     │
├───────┬──────────────────────────────────┬───────────┬───────────────────────────────────────────┤
│ Phase │ Milestone Name                   │ Timeline  │ Deliverable & Hard Gate                   │
├───────┼──────────────────────────────────┼───────────┼───────────────────────────────────────────┤
│ **P0**│ **Guarded Default-On Feasibility**│ Weeks 1-2 │ List-first + demand-driven MILP; rollback │
│       │                                  │           │ flag; whole-corpus assembly differential. │
├───────┼──────────────────────────────────┼───────────┼───────────────────────────────────────────┤
│ **P1**│ **Final-RTL Model & Checker**    │ Weeks 2-4 │ Dense layout positions 0..P-1; extract    │
│       │                                  │           │ constraint graph; independent validator.  │
├───────┼──────────────────────────────────┼───────────┼───────────────────────────────────────────┤
│ **P2**│ **Exact Closed-Island M2 Alloc** │ Weeks 4-8 │ DSATUR/backtracking allocator; atomic     │
│       │                                  │           │ substitution; pass 11->8 fixture.         │
├───────┼──────────────────────────────────┼───────────┼───────────────────────────────────────────┤
│ **P3**│ **Real Silicon Welford Benchmark**│ Parallel  │ Launch real producer/consumer on Blackhole│
│       │                                  │ (Weeks 2-3│ Export raw mean/M2 & cycle deltas.        │
├───────┼──────────────────────────────────┼───────────┼───────────────────────────────────────────┤
│ **P4**│ **Latency Mode (Dual-Horner)**   │ Weeks 8-12│ Goodman-Hsu mode switching; eliminate 40% │
│       │                                  │           │ of issue slots on Wormhole.               │
├───────┼──────────────────────────────────┼───────────┼───────────────────────────────────────────┤
│ **P5**│ **Coprocessor & MLIR Roadmaps**  │ Multi-Qtr │ Conflict-graph Replay, Macro event model, │
│       │                                  │           │ and TT-Vector MLIR Dialect.               │
└───────┴──────────────────────────────────┴───────────┴───────────────────────────────────────────┘
```

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

## 11. Rebuttal, Solution Proposals, and the Hardened Path

This section is the author's formal response to an external maturity-and-correctness critique of the preceding roadmap. The critique's central charge is that the title asserts "World SOTA" while the largest artifact in the document — the ~460-line §4.2 allocator (`SFPI_COMPILER_UPGRADE.md:195-656`) — is aspirational, and that a skimming reader will overestimate what is shipped. We accept the reader-legibility defect and concede the specific factual points below. We reject the stronger implication that the document *misrepresents* the tree: its speculative content is labeled structurally (`Target`, `Non-Normative Target`, `Candidate Opportunity`, `Mockup Evidence`, `Authoritative Architecture`) at exactly the lines a skimmer lands on. The defect is that this honesty is **distributed** across per-section adjectives rather than **consolidated** into one normative index. §11.1 consolidates it; §11.2 answers each axis; §11.3 specifies the engineering; §11.4 re-sequences delivery so that the single proof that matters — silicon — becomes a blocking gate rather than parallel-optional work.

### 11.1 Maturity Ledger

The following ledger is normative and is intended to be lifted to a new **§0** above §1, so that no reader can reach the §4 target block without first passing a table that classifies every capability against the fixed status vocabulary `{SHIPPED, STUB, ROADMAP}` with a checkable `file:line` anchor. Every `SHIPPED` anchor is CI-verifiable; every `ROADMAP`-tagged symbol (e.g. `assign_{v,r}`, DSATUR allocator, `run-corpus-differential.sh`) must fail the build if it is ever found *shipped* without promotion in this ledger.

| Capability | Status | Evidence |
| :--- | :--- | :--- |
| GIMPLE pressure list scheduler (`pass_rvtt_lp_schedule`, fires only when peak $>8$) | **SHIPPED** | `gimple-rvtt-lp-schedule.cc:651-769`; `rvtt-passes.def:41` |
| Independent transactional validator (re-derives liveness; gates peak $>8 \to \le 8$) | **SHIPPED** | `gimple-rvtt-lp-schedule.cc:352-579`, `:570-575` |
| Validator rejection self-test (three corrupted certificates rejected) | **SHIPPED** | `gimple-rvtt-lp-schedule.cc:585-645` |
| lp_solve **schedule-only** MILP (`x(op,slot)`, liveness cap 8, 100k-node abort) | **SHIPPED** (needs `HAVE_LPSOLVE`) | `rvtt-lpsolve.cc:71-74`, `:207-363`, `:94-99` |
| Both feature flags default **OFF** (opt-in) | **SHIPPED** | `riscv.opt:586`, `:593` |
| Pre-IRA alloc pass = **dump-only audit**, emits `colorability=unchecked` | **STUB** | `rtl-rvtt-lp-alloc.cc:55-58`, `:88`, `:120-124` |
| 11$\to$8 MILP-beats-list result surviving IRA (boundary; spills on purpose) | **STUB / boundary** | `scripts/lp-schedule-milp-beats-list.C:8-12`; `validate-welford-scheduler.sh:199-207` |
| Joint MILP with physical `assign_{v,r} \in \{0..7\}` register vars (§3.1) | **ROADMAP** | doc `:108-114`; contradicted by `rvtt-lpsolve.cc:71-74` |
| §4.2 14-step DSATUR / backtracking closed-island allocator | **ROADMAP** (P2, Wk 4-8) | doc `:196` (`// Target Implementation`), `:820` |
| Default-on (`Init(1)`) deployment | **ROADMAP** | doc `:30`, `:63` vs shipped `riscv.opt:586` |
| Whole-corpus differential driver `run-corpus-differential.sh` | **ROADMAP** (absent) | doc `:856`, `:863`; file not in `scripts/` |
| §6 replay-compression / `SFPLOADMACRO` speedups | **ROADMAP** (Mockup) | doc `:743`, `:757-766` |

**Retitle recommendation (`SFPI_COMPILER_UPGRADE.md:1`).** Separate what ships from what is planned so the title itself scopes to a roadmap:

> `# SFPI Compiler Upgrade — Phase 1: A Guarded, Opt-In GIMPLE Pressure Scheduler (Shipped), with a Roadmap to Exact MILP Allocation and World-SOTA Coprocessor Scheduling (Target)`

**Gate:** the retitle merges only if every noun in the title maps to a ledger row whose status is `SHIPPED`, or is immediately followed by the word *Target* / *Roadmap*.

### 11.2 Point-by-Point Rebuttal & Concessions

**1. Documentation-maturity honesty.**
*Concession:* the title says "World SOTA" and the §4.2 block is aspirational; a reader who stops mid-block before its one-line header will overestimate shipped scope.
*Rebuttal:* the honesty *exists* and sits where skimmers land — the §4.2 fence opens `// Target Implementation for …` (`:196`), §3.1 is headed "**Non-Normative Target**" (`:108`), §2.2 is a two-column *Checked-In* vs *P0 Target* table (`:58-66`), and §6 rows are each tagged "Candidate Opportunity / silicon verification required" (`:757-766`). The defect is *distributed labeling*, not misrepresentation; the "~20% shipped" figure is itself imprecise — two passes plus a real lp_solve adapter ship. Fix is consolidation (§11.1), not retraction.

**2. MILP scalability and solver-timeout risk.**
*Concession:* lp_solve is a weak B&B engine, a node-cap hit is functionally "give up," and the §3.1 model as written (occupancy binaries over horizon $T=\sum\text{latency}$, three lexicographic solves) would time out.
*Rebuttal:* that model is not in the tree. The shipped MILP is single-issue with horizon $N$, columns $= N^2 + 2VN$ (`rvtt-lpsolve.cc:81`), one solve against a deviation objective (`:405-411`), hard-capped at $2 \le N \le 24,\ V \le 32$ (`:145`), warm-started to the list order (`:190-194`), and demand-driven behind three guards (`gimple-rvtt-lp-schedule.cc:829`) plus `HAVE_LPSOLVE`. Worst case $24^2 + 2\cdot32\cdot24 \approx 2112$ columns (576 true binaries) — trivially inside lp_solve's competence. The critique lands only against the §3.1 *joint* model, which must never ship without the guards in §11.3-B.

**3. Cross-phase determinism across IRA (GIMPLE-schedule / RTL-observe).**
*Concession:* correct — the scheduler runs at GIMPLE before `pass_expand` (`rvtt-passes.def:41`) while observation runs before `pass_ira` (`:50`); a validated peak $\le 8$ GIMPLE order does not bind IRA's coloring, and IRA can still spill into `rvtt_mov_error` (`rvtt.cc:254`), exactly as the 11$\to$8 fixture shows.
*Rebuttal:* this is a "not-guaranteed-better," not a "can-be-worse-than-off," gap. Mutation is strictly conjunctive — `applied = validated && rejection_selftest && apply_schedule(...)` (`gimple-rvtt-lp-schedule.cc:881-882`) — `apply_schedule` early-returns on tie (`:775-779`), and a declined region emits zero `TODO` (`:1012-1013`), so a region we do not touch is byte-identical to flag-off. The residual (an *applied* reorder that does not help IRA) is real and is closed by M2 + the post-IRA guard in §11.3.

**4. M2 allocator: shipped stub vs. authoritative DSATUR pipeline.**
*Concession:* correct — `rtl-rvtt-lp-alloc.cc` is dump-only (`:55-58`), its `execute()` only calls `audit_function()` and returns `TODO_df_finish` (`:120-124`), none of `solve_m2_exact_coloring` / `certify_destructive_tie` exist as compiled code, and the §4.2 lambda `SFPI_COMPILER_UPGRADE.md:435` is malformed (`[&](size_colored_count)` — declared type `bool(size_t)` but nameless/typeless parameter, body uses `colored_count`).
*Rebuttal:* the stub is a *declared, load-bearing* milestone, not a Potemkin façade — it hardcodes `colorability=unchecked` (`:88`) precisely so it cannot be mistaken for an allocator, runs real `df_analyze` + forward liveness (`:60-91`), is correctly wired before `pass_ira`, and gates behind the same off-by-default flag. The roadmap already books it as P2/Weeks 4-8 (`:820`). We concede and fix the lambda signature to `[&](size_t colored_count)`.

**5. Silicon measurement (make-or-break).**
*Concession:* correct and decisive — there is not one silicon *or simulator* cycle number in the tree; every §7 row is "Candidate Opportunity" (`:757-766`), the headline figure is "Mockup Evidence" (`:743`), the Welford benchmark is booked P3/Parallel (`:823`), and `scripts/sfpu-perf-mockups.C` measures nothing.
*Rebuttal:* the document never asserts a silicon win, and the *compiler* bakes in no perf claim — the acceptance test is a pure pressure gate, $\text{old} > 8 \wedge \text{new} < \text{old} \wedge \text{new} \le 8$ (`gimple-rvtt-lp-schedule.cc:570-575`), never a cycle count. The error is *sequencing*: the only proof that matters is scheduled parallel/P3 instead of as a blocking front gate. §11.4 corrects this by promoting silicon to a P1 hard gate.

**6. Correctness / test-coverage.**
*Concession:* correct — `scripts/lp-schedule-milp-beats-list.C` is a driver-checked boundary case, not a DejaGNU xfail, and `scripts/run-corpus-differential.sh` does not exist.
*Rebuttal:* marking a known allocator boundary xfail is standard GCC practice, and the tree already ships the plumbing: `scripts/local-xfails.py` converts a matched FAIL to XFAIL and an unexpected success to XPASS (`local-xfails.py:133-134`, `:237-240`). The boundary is *positively pinned* today — `validate-welford-scheduler.sh:199-213` asserts the exact ICE string plus the GIMPLE `old-peak=11 … new-peak=8 … validated=yes` and MILP `selected=yes` dumps. The middle-end correctness story is tested; only the post-IRA coloring guarantee and two pieces of *harness plumbing* are missing.

### 11.3 Solution Proposals

**A. Reframe P0 as "advisory pressure relief, safe-by-fallback."** Retire the word *guarantee* from all pre-M2 doc prose and dump strings. The honest P0 **Contract** is: *for regions we touch, the modeled GIMPLE peak provably fell to $\le 8$; we do not yet bind IRA.* This is justified structurally, not rhetorically, by the conjunctive-gate / no-op-on-tie / zero-TODO-on-decline properties above.

**B. Harden the shipped MILP and pre-commit the §3.1 joint model behind identical controls.**

- **N-ceiling made explicit.** Promote the implicit `m_count > 24` cap (`rvtt-lpsolve.cc:145`, `:382`) to a named tunable `riscv_tt_milp_max_ops` (`Init(24)`), checked in `analyze_region` *before* `build_solver_problem` (`gimple-rvtt-lp-schedule.cc:829`). Above the ceiling: record `reason=milp-skipped-size`, keep the validated list schedule. *Gate:* a unit test proves $N=25$ never enters `rvtt-lpsolve.cc`.
- **Horizon invariant locked.** Assert `m_columns == N^2 + 2VN` as a regression check so no future edit reintroduces a $\sum\text{latency}$ per-cycle grid.
- **Wall-clock budget.** Add `riscv_tt_milp_timeout_ms` via `set_timeout`; map `SUBOPTIMAL`/timeout to `rvtt_solver_status::capped`, exactly like the existing 100k-node cap, so a capped incumbent can never contribute (`rvtt-lpsolve.cc:94-99`, `:182-185`).
- **Warm-start as a contract.** Keep `preferred_feasible` fixing `x(op,slot)` to the list order (`:190-194`); assert the returned order equals the list order when a $\le 8$ preferred schedule was supplied, else force `internal_error`.
- **Pre-commit rule.** The §3.1 `assign_{v,r}` joint model may only ever be *built* behind `riscv_tt_milp_max_ops` + timeout + capped-never-contributes. Code review blocks any `assign_{v,r}` binary lacking all three.

**C. Land M2 as compiled code behind the off flag.** Upgrade `pass_rvtt_lp_alloc` from `audit_function` (`rtl-rvtt-lp-alloc.cc:59-92`) to an atomic hard-register substituter over the L0–L7 class (`SFPU_REG_NUM = 8`): build the SFPU interference graph via `df_analyze` + `DF_LR_IN`; run bounded DSATUR + backtracking 8-coloring capped at `max_search_nodes = 100000` (matching the MILP B&B cap); on success atomically rewrite via grouped `validate_change` / `apply_change_group` with `recog_memoized(insn) >= 0` re-verification; on capped/failed coloring, no-op. This flips `colorability=unchecked` (`:88`) to `colorability=proved|infeasible`. An **independent** from-scratch interference validator — the RTL analogue of `validate_schedule` + `validator_rejection_selftest` — must reject a deliberately double-assigned coloring before any mutation, mirroring `gimple-rvtt-lp-schedule.cc:352,585`.

**D. Post-IRA spill-detection guard (the P0.5 safety net, before M2 lands).** Insert `pass_rvtt_lp_spillguard` via `INSERT_PASS_AFTER(pass_ira,1,...)`. After IRA/reload it scans for any `SFPU_REG_P` spill. On detection in a region the GIMPLE scheduler had reordered, the cheap shippable form masks `riscv_tt_opt_pressure_schedule` for that function and re-emits the baseline order — a one-shot self-fallback. The invariant is: **any function where the reorder led to a spill compiles identically to flag-off.**

```
                    POST-IRA SPILL-DETECTION GUARD  (pass_rvtt_lp_spillguard)
        ┌─────────────────────────────────────────────────────────────────────────┐
        │   GIMPLE reorder (validated peak<=8)  ──►  pass_expand  ──►  pass_ira     │
        └───────────────────────────────────┬─────────────────────────────────────┘
                                            ▼
                          ┌─────────────────────────────────┐
                          │  Scan reload output for a spill  │
                          │  of an SFPU_REG_P pseudo (L0-L7) │
                          └───────────────┬─────────────────┘
                              no spill     │      spill found in a stamped region
                         ┌────────────────┴────────────────┐
                         ▼                                  ▼
                 EMIT candidate code               MASK riscv_tt_opt_pressure_schedule
                 (reorder survived IRA)            for this function; RE-EMIT baseline
                         │                                  │
                         ▼                                  ▼
              ┌────────────────────┐            ┌──────────────────────────────────┐
              │  Non-regression:   │            │  INVARIANT: byte-identical to     │
              │  colored L0-L7 map │            │  flag-off  (no rvtt_mov_error)    │
              └────────────────────┘            └──────────────────────────────────┘
```

**E. Build the absent corpus driver.** Author `scripts/run-corpus-differential.sh --baseline=<dir> --candidate=<dir> --output=<dir>` (referenced at `SFPI_COMPILER_UPGRADE.md:856,863`). It compiles the whole TT-LLK allowlist corpus baseline-off vs candidate-default, archives per-kernel `.S` diffs, replays every *changed* binary through the existing `scripts/riscv-tt-elf-run` (`qemu-riscv32`) wrapper, and emits `summary.json` (`compiled`, `asm_changed`, `sim_run`, `sim_divergent`, `new_spills`) plus a histogram of region $(N, V)$ that sets the empirical value of `riscv_tt_milp_max_ops`. Because only `sfpadd`/`sfpmul`/`sfpmad` straight-line regions are ever touched (`schedulable_p`, `gimple-rvtt-lp-schedule.cc:81-115`), the untouched set is provably byte-identical and the baseline arm doubles as a continuously-tested kill switch.

**F. Promote the 11$\to$8 fixture to a self-flipping DejaGNU xfail.** Relocate to `gcc/gcc/testsuite/gcc.target/riscv/tt/lp-schedule-milp-beats-list.C` with a positive `scan-tree-dump` on `old-peak=11 … new-peak=8 … validated=yes reason=ok applied=yes` and the IRA boundary encoded as `{ dg-error "cannot store sfpu register \(register spill\)" "M2 boundary" { xfail *-*-* } }`. When M2 lands, DejaGNU reports **XPASS** — a loud CI signal to remove the marker. Register the same test in `local-xfails.py` (`:237-240`) so the checked-in `.sum` lane tracks the identical XFAIL$\to$XPASS transition. The profitability gate and self-tests are never weakened; the xfail is only on the final assembly step.

### 11.4 Hardened Execution Plan

This table re-sequences §9's roadmap so that (a) **silicon Welford measurement is a P0/P1 blocking gate**, not P3/parallel; (b) **M2 lands as compiled code behind the off flag** before any default-on flip; and (c) **P0 is reframed as advisory, safe-by-fallback**. Every row's Hard Gate is machine-checkable.

| Phase | Deliverable | Hard Gate | Closes Which Critique |
| :--- | :--- | :--- | :--- |
| **P0** | Insert §0 **Maturity Ledger** (§11.1) above §1; retitle line 1; ledger-consistency linter added to `validate-welford-scheduler.sh` | Every `SHIPPED` `file:line` anchor resolves in-tree; any `ROADMAP` symbol (`assign_{v,r}`, DSATUR, `run-corpus-differential.sh`) found shipped without promotion **fails the build** | Axis 1 (maturity honesty) |
| **P0** | Reframe P0 contract to **"advisory pressure relief, safe-by-fallback"** in doc §2 + dump strings (`gimple-rvtt-lp-schedule.cc:894-899`) | The word *guarantee* appears for **no** pre-M2 behavior; reviewer sign-off | Axis 3 (GIMPLE↔IRA gap) |
| **P0** | Author `scripts/run-corpus-differential.sh` (absent, `doc:856`); freeze A/B methodology (baseline = no-flag `Init(0)`) | Reproduces 11$\to$8 boundary deterministically over $\ge 3$ runs; zero `.S` diff outside `sfpadd`/`sfpmul`/`sfpmad` regions; emits $(N,V)$ histogram | Axes 5, 6, 2 |
| **P0** | Add `riscv_tt_milp_max_ops` (`Init 24`), checked in `analyze_region` before `build_solver_problem`; lock `m_columns == N^2+2VN` invariant | Unit test proves $N=25$ never enters `rvtt-lpsolve.cc` (`reason=milp-skipped-size`); column-count assertion passes for $N=24,V=32$ | Axis 2 (MILP scalability) |
| **P0.5** | Implement `pass_rvtt_lp_spillguard` (`INSERT_PASS_AFTER(pass_ira,1,…)`), recompile-fallback form | The 11$\to$8 fixture compiles to **baseline** (no `rvtt_mov_error` at `rvtt.cc:254`); `validate-welford-scheduler.sh:199-206` updated to assert clean fallback | Axis 3 (GIMPLE↔IRA gap) |
| **P1** | **Silicon Welford benchmark — MAKE-OR-BREAK.** Run flagship kernel on real Blackhole under the Tensix cycle profiler, $N \ge 30$, median + p95; simulator A/B on `riscv-tt-elf-run` as gating proxy | Candidate median cycles/tile $\le$ hand-tuned baseline with **non-overlapping p95**; zero net-new spills. **Failure keeps both flags `Init(0)` and blocks P2** | Axis 5 (silicon evidence) |
| **P1** | Promote 11$\to$8 fixture to DejaGNU xfail (§11.3-F); register in `local-xfails.py`; add wall-clock `riscv_tt_milp_timeout_ms` mapping to `capped` | DejaGNU reports **XFAIL** (not FAIL, not unmarked XPASS) on WH+BH; pathological region shows list schedule applied, MILP incumbent discarded | Axes 6, 2 |
| **P2** | Fix §4.2 lambda to `[&](size_t colored_count)` (`doc:435`); land M2 DSATUR/backtracking allocator with atomic `apply_change_group` substitution + independent coloring validator, **behind the off flag** | Extracted engine compiles under `-Werror`; coloring-corruption self-test rejected before mutation; dump emits `colorability=proved` (replacing `:88`); 11$\to$8 fixture **XFAIL→XPASS** with L-regs assigned | Axis 4 (M2 stub) |
| **P2** | Full-corpus silicon differential via `run-corpus-differential.sh`; **only then** flip `riscv.opt:586` `Init(0)→Init(1)` | Non-regression on $\ge 95\%$ of kernels, $\ge 1$ significant win, **zero net-new spill ICEs** across WH+BH; any failure keeps both flags `Init(0)` | Axes 5, 3 |
| **P3** | Convert remaining §7 "Candidate Opportunity" rows (Dual-Horner, Log, GELU/Erfinv, `doc:760-763`) from mock to measured | No §7 row keeps a numeric perf claim unless a committed silicon measurement backs it | Axis 5 |
| **P4/P5** | Defer latency scheduling and `SFPLOADMACRO` throughput modeling (`doc:877`) until the P1 silicon harness can score them | No throughput feature merges without a `run-corpus-differential.sh` silicon number attached | Axes 5, 2 |

---

## 12. Conclusion & Operational Contract

Adopting the **guarded default-on feasibility policy (P0)** makes validated SFPU pressure rescue automatic for the narrow WH/BH regions supported today while retaining deterministic fallback and an operational rollback. 

Completing **M2 Physical Allocation (P1/P2)** with an independently certified final-RTL DSATUR engine extends that rescue to logically feasible schedules that generic IRA still misses. 

Latency scheduling (P4), conflict-constrained replay (P5), and `SFPLOADMACRO` event modeling (P5) provide the disciplined engineering path to maximize hardware throughput and realize world-class vector compilation on Tenstorrent Tensix silicon.

---

## 13. Critical Rebuttal to the Hardened Plan

The maturity ledger is the strongest correction so far. It accurately distinguishes shipped GIMPLE scheduling from the dump-only RTL audit, labels the joint MILP and DSATUR allocator as roadmap work, identifies the absent corpus driver, and promotes silicon evidence. Those changes materially improve the document.

The hardened plan nevertheless introduces one infeasible recovery mechanism and still treats two incomplete checks as authoritative. These issues must be corrected before the ledger can serve as an execution contract.

### 13.1 A Post-IRA Pass Cannot Rewind and Recompile the Function

Section 11.3-D proposes a pass after IRA that detects an SFPU spill, masks the scheduler for the function, and "re-emits the baseline order." This is not a normal GCC pass-manager capability. By the time an after-IRA pass runs, GIMPLE scheduling, expansion, RTL scheduling, and IRA have already mutated or discarded the state needed to reproduce the flag-off compilation. Toggling a target flag at that point does not restore the original GIMPLE order or rerun the earlier pipeline.

More importantly, the SFPU spill path can call `rvtt_mov_error` during allocation/reload. A fatal diagnostic or ICE raised there prevents a later spillguard pass from executing at all. The proposed guard therefore cannot guarantee recovery from the failure it is intended to catch.

There are only three credible fallback designs:

1. **External compiler retry:** the build driver compiles with the candidate option, recognizes the specific spill failure, and launches a fresh compiler invocation with the rollback flag. This can preserve compile coverage but not a single-invocation byte-identity guarantee.
2. **Pre-IRA proof:** do not commit the GIMPLE reorder unless the authoritative final-RTL model proves colorability and the later allocation contract is enforced. This is the M2 direction.
3. **Retained pre-IRA alternative state:** preserve both candidate and baseline representations and choose before irreversible downstream passes. This is invasive and must be designed explicitly; a post-IRA pass cannot synthesize it retroactively.

Delete the in-compiler "recompile-fallback" claim unless a concrete GCC pass-manager mechanism and state-restoration implementation are supplied. For P0, use external retry plus corpus evidence, or accept that the current scheduler is advisory and cannot promise automatic in-process recovery.

### 13.2 The Global Closure Check Mutates RTL Outside the Transaction

`verify_global_pseudo_closure` is named as a verifier but resets out-of-island debug locations and rescans their DF records. It runs before model validation, coloring, and the standalone grouped-change transaction. If coloring returns `UNSAT`/`SEARCH_LIMIT`, recognition rejects a replacement, or the staged validator fails, those debug mutations remain committed even though the operational contract says failure leaves RTL untouched.

A read-only precondition must not mutate program state. Use one of these policies:

- reject an island containing external debug references;
- record affected debug instructions and stage their resets in the same grouped transaction; or
- perform a documented, non-fallible debug reset only after the semantic transaction commits, while weakening "byte-identical fallback" to exclude debug metadata.

The current implementation violates transactional fallback and can make `-g` output differ on a failed rescue. Add a negative test that forces coloring or recognition failure after discovering an external debug use and verifies identical RTL/debug dumps to baseline.

### 13.3 Constraint-Alternative Certification Still Does Not Inspect the Alternative

The revised certifier now obtains a real instruction code and bounds-checks `selected_alternative`, which fixes the prior `recog_memoized` category error. It still never runs constraint selection for the requested alternative and never inspects that alternative's operand constraints.

Specifically:

- `selected_alternative` is range-checked and copied, but never used to query or constrain operands;
- `insn_data[icode].n_alternatives` proves only that the index exists;
- mode equality does not prove a matching constraint or destructive tie;
- destination operand zero is assumed without proving the target pattern's output index;
- early-clobber, matching-operand numbers, allowed register classes, and `_lv` semantics are not checked; and
- the caller still supplies `kind` rather than the certifier deriving it from target semantics.

The implementation must use GCC's preprocessed `operand_alternative` data and/or an explicitly constrained enabled-alternative mask, then prove the result/operand match for `selected_alternative`. `recog_op_alt` is valid only after the required extraction/preprocessing/constraint setup. A merely recognizable instruction with equal operand modes is not a certified two-address tie.

### 13.4 The Staged Validator Checks the Solution Object, Not the Staged RTL

The new staged validator consumes the interference graph and ties, but it checks `regno_to_lreg` against those structures. That map is the solver's own output. Rechecking that the solver map has no conflicting colors and honors contracted ties is useful as a solver sanity check, but it does not prove the substitutions present in `PATTERN(insn)` equal the map.

The validator must traverse every fully staged instruction and establish an occurrence-level correspondence:

- every selected pseudo occurrence became the expected `SFPU_REG_FIRST + color` hard register;
- no selected pseudo remains;
- no unselected pseudo or hard register changed;
- modes and subregister structure remain legal;
- staged clobbers are included; and
- the actual recognized operand alternative realizes every certified tie.

Calling `recog(PATTERN(insn), ...)` proves recognizability, not mapping fidelity. Without an occurrence-level check, a traversal bug that omits or misrewrites one operand can pass the current validator.

### 13.5 The DSATUR Listing Still Does Not Compile

The maturity ledger correctly notices the malformed lambda:

```cpp
std::function<bool(size_t)> backtrack = [&](size_colored_count) -> bool {
```

The body uses `colored_count`, but the parameter has neither that name nor a valid declared type. Because this remains in the supposed target implementation, the document should fix it now to:

```cpp
std::function<bool(size_t)> backtrack = [&](size_t colored_count) -> bool {
```

A roadmap code block need not be production code, but a P2 implementation blueprint should at least compile in an isolated target test before its architecture is approved.

### 13.6 The Silicon Gate Uses Invalid Statistical Language

"Candidate median cycles/tile <= baseline with non-overlapping p95" is not a defined statistical test. A p95 is a quantile estimate, not an interval that can overlap or fail to overlap another p95. With only 30 samples, tail estimates are especially noisy.

Define the gate using raw paired or randomized A/B samples, a declared estimator, confidence interval or bootstrap procedure, practical non-inferiority margin, and noise controls. For example: candidate median is no worse than baseline by more than a predeclared percentage at a 95% confidence level, and any claimed win exceeds both that margin and observed run-to-run noise. Preserve raw samples and device metadata.

Correctness/compile coverage and performance should be separate gates. A scheduler that rescues formerly uncompilable code can merit default-on deployment without a silicon speedup, provided it creates no correctness or compile-success regression. Conversely, one significant speedup does not justify permitting regressions in 5% of kernels. Clarify whether the proposed 95% threshold concerns performance only; correctness and new spill regressions require 100% pass/fallback.

### 13.7 Revised Execution Decision

- **Keep the maturity ledger and shipped/roadmap separation.** They are accurate and valuable.
- **Proceed with P0 default-on engineering**, but use a real external retry or complete pre-IRA proof; do not rely on an impossible post-IRA rewind.
- **Require 100% correctness and compile-success non-regression** for the eligible corpus. Treat performance through a separately defined non-inferiority/win policy.
- **Keep P2 behind the off flag** until tie certification examines real GCC alternatives, debug cleanup is transactional, staged validation checks actual RTL occurrences, and the code compiles with negative rollback tests.
- **Do not block correctness work on a mandatory speedup.** Silicon evidence controls performance claims and profitability policy, not whether a sound compile-coverage rescue is worth implementing.

The project is now mature enough to stop cycling on terminology and implement the remaining gates. The two dangerous shortcuts are the fictional post-IRA rewind and validation functions that prove properties of metadata without proving the staged RTL actually realizes that metadata.
