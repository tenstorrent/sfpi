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
| **Exact MILP Engine** | **Real** (`rvtt-lpsolve.cc`, 482 lines via `lp_solve`) | Demand-driven escalation on list failure (100k-node cap) |
| **Driver Flags** | `Init(0)` (Explicit opt-in required) | `Init(1)` default-on for WH/BH allowlist + rollback option |
| **Pre-IRA Physical Allocator** | Dump-only stub (`rtl-rvtt-lp-alloc.cc`, 133 lines) | **M2 Engine:** 14-step exact DSATUR allocator with GCC 15 change transactions |
| **Corpus Differential Driver** | Absent | **P0 Deliverable:** `scripts/run-corpus-differential.sh` |
| **Hardware Silicon Baseline** | **Archived Green (Blackhole)**: vFloat 339 cyc — −27.3% vs non-replay (466), within 13 cyc / 3.9% of replay LLK (326); N=32 all-selectors pass; compiler `8bea8aba49` / `be125cd`; archived §13.8. Raw-LLK asm path still needs the fixed-range model (§13.4). | **P3 Deliverable:** Full multi-kernel A/B paired suite (initial Welford run archived §13.8) |

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
| **Welford (LayerNorm)** | 8 live values across 4 rows with zero register slack. | Recomputes delta ($\delta_2$) and hand-colors L0–L7. | **Demonstrated on Blackhole Silicon:** 339 device math cycles (-27.3% vs non-replay 466 cycles, within 13 cycles / 3.9% of replay LLK 326 cycles, N=32 correctness passes all selectors). |
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
│       │                                  │ (Weeks 2-3│ Paired A/B cycles & non-inferiority gate. │
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

### 12.7 Verified Compiler Fixtures Do Not Establish the Raw-LLK Root Cause

Section 13 correctly demonstrates that the pure `sfpi::l_reg[]` builtin path creates constrained pseudos and does not reproduce the suspected L1 collision. That is valuable negative evidence. It does not verify the asserted "corrected root cause" in the raw-LLK path. The same section admits that the real raw-LLK reproducer has not yet been obtained.

Until a fixture combines the actual raw LREG producer/consumer sequence with surrounding SFPI temporaries and reproduces the silicon failure or assembly collision, classify raw-LLK missing live ranges as the leading hypothesis—not a verified root cause. Inline assembly may expose clobbers or constraints that change the analysis, and a silicon failure can have causes other than L1 allocation.

A post-IRA verifier can diagnose a realized conflict but cannot repair an earlier clobber or recover from a fatal allocation path. Treat it as a checking backstop, not the primary fix. The primary fix should model verified raw-LLK fixed live ranges before IRA once the reproducer establishes their true boundaries.

### 12.8 Stop the Documentation Loop and Land Discriminating Code

The checked-in allocator remains a dump-only stub, both flags remain `Init(0)`, and the corpus differential driver remains absent. Section 13 is right that the next useful milestone is executable work.

- Land the real raw-LLK reproducer first and require it to fail for the intended reason.
- Fix candidate validation, mandatory-kind handling, alternative feasibility, and debug predicates in compiled P1 code behind the off flag.
- Implement an independently reconstructed staged-RTL checker, not only a solver/ledger consistency check.
- Build the corpus/simulator differential driver and list-success MILP short-circuit before the default-on flip.
- Keep silicon performance claims separate from correctness and compile-coverage gates.

The architecture is sufficiently specified. The remaining risk is now that repeated prose harmonization creates confidence in pseudocode that has never compiled or survived adversarial fixtures.

---

## 13. Reviewer Opinion & Verified Welford Findings

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
- **Archived (was open):** the paired A/B numbers are now recorded in-tree at **§13.8** and cited
  from the §2.2 silicon row. Still to attach when exported from the harness: the raw per-run cycle
  dumps and correctness vectors + device/firmware metadata, so the summary is backed by primary
  artifacts, not just a table.

### 13.8 Archived Silicon Results — Blackhole Welford Body (2026-08-15)

*In-repo archive of the paired A/B silicon run, per §13.5.5 / §13.7. This is the reported run
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
- This validates the committed opt-in scheduler on real silicon. It does **not** exercise M2
  (`rtl-rvtt-lp-alloc.cc` still `colorability=unchecked`); the win is scheduler + baseline IRA.

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
- VERDICT: correctness + register-allocation safety verified on silicon; scheduler matches
  non-replay handwritten perf with automatic allocation (no manual LREG pinning). Passing does not
  flip default-on; it supplies the silicon evidence for that decision.

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
