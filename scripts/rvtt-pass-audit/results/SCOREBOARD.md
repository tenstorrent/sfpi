# rvtt pass audit — soundness, correctness, performance potential

- **Tree**: `sfpi-gcc` @ `827598a71b72`
- **Passes scored**: 54 of 55 registered (`pass_dce` is generic GCC DCE, not an rvtt pass)
- **Model**: `jev-1.13.0` — 16 independent judgments per pass, one batched request each
- **Corpus context**: 540 named refusals, 11 proof artifacts, 97 `-mtt-tensix-*` options

> **What this is.** A calibrated triage ranking, not a verifier. System One returns probabilities over a rubric; it does not prove soundness. Soundness here is established by `tt/proofs/`, the testsuite, and silicon A/B. Read every number below as *where to spend review time*, and confirm each flag against the source before acting on it.

## 1. Review queue

Flags are **relative to this corpus**, not absolute verdicts. A pass appears because it sits in the worst fifth of the 54 on some dimension. Percentile cuts are quoted inline so you can see how far from its peers it actually is.

**Read the two queues differently.** The risk questions assume an *optional transform with a precondition to check before acting*. That frame fits the flag-gated optimizations. It does not fit mandatory lowering: `expand`'s job is to rewrite every condition tree it sees, so "mutates before validating" is tautological for it, not a defect. Hand-checking confirmed this — `expand` carries zero named refusals and three asserts by design. The two populations are therefore ranked separately, and 1b should be read as *where the backend carries unevidenced risk by construction*, not as a defect list.

### 1a. Opt-in optimizations (off by default) — the risk questions apply directly — 12 flagged, 4 with an ALARM

These opt in to a transform under a precondition, so a flag here means the precondition handling looks weaker than its peers. **This is the queue to work first**: these are the passes whose promotion is still a live decision.

#### `attrib` — flag-gated, default-off, 266 loc, gimple
`gcc/config/riscv/tt/gimple-rvtt-attrib.cc`

- **CONCERN** — fail-closed discipline weakest in the corpus (p=0.08, corpus p20=0.12)
- **ALARM** — failed checks may be silently dropped (1.95/3, corpus p80=1.86)
- **ALARM** — most fragile to pass reordering (2.42/3, corpus p80=2.40)
- **CONCERN** — thinnest test coverage in the corpus (0.00/3, p20=0.02)

#### `lp_alloc` — flag-gated, default-off, 1610 loc, rtl
`gcc/config/riscv/tt/rtl-rvtt-lp-alloc.cc`

- **CONCERN** — largest apparent gap between contract and code (p=0.57, corpus p20=0.58)
- **ALARM** — may mutate IR before validation completes (1.91/3, corpus p80=1.52)

#### `lreg_rename_chains` — flag-gated, default-off, 1966 loc, rtl
`gcc/config/riscv/tt/rtl-rvtt-lreg-rename.cc`

- **ALARM** — may mutate IR before validation completes (1.89/3, corpus p80=1.52)

#### `lp_schedule_prera` — flag-gated, default-off, 1350 loc, rtl
`gcc/config/riscv/tt/rtl-rvtt-lp-schedule-prera.cc`

- **ALARM** — may mutate IR before validation completes (1.84/3, corpus p80=1.52)

#### `reassoc` — flag-gated, default-off, 1537 loc, gimple
`gcc/config/riscv/tt/gimple-rvtt-reassoc.cc`

- **CONCERN** — largest apparent gap between contract and code (p=0.40, corpus p20=0.58)
- **CONCERN** — WH/BH gating least established (p=0.54, corpus p20=0.54)

#### `delivery_shape` — flag-gated, default-off, 597 loc, gimple
`gcc/config/riscv/tt/gimple-rvtt-delivery-shape.cc`

- **CONCERN** — largest apparent gap between contract and code (p=0.49, corpus p20=0.58)
- **CONCERN** — WH/BH gating least established (p=0.52, corpus p20=0.54)

#### `macro_planner` — flag-gated, default-off, 1913 loc, rtl
`gcc/config/riscv/tt/rtl-rvtt-macro-planner.cc`

- **CONCERN** — may mutate IR before validation completes (1.65/3, corpus p80=1.52)

#### `lp_schedule` — flag-gated, default-off, 1111 loc, gimple
`gcc/config/riscv/tt/gimple-rvtt-lp-schedule.cc`

- **CONCERN** — largest apparent gap between contract and code (p=0.48, corpus p20=0.58)

#### `mop_form` — flag-gated, default-off, 1353 loc, rtl
`gcc/config/riscv/tt/rtl-rvtt-mop-form.cc`

- **CONCERN** — largest apparent gap between contract and code (p=0.49, corpus p20=0.58)

#### `int_abs` — flag-gated, default-off, 578 loc, gimple
`gcc/config/riscv/tt/gimple-rvtt-int-abs.cc`

- **CONCERN** — largest apparent gap between contract and code (p=0.51, corpus p20=0.58)

#### `crossloop` — flag-gated, default-off, 533 loc, gimple
`gcc/config/riscv/tt/gimple-rvtt-crossloop.cc`

- **CONCERN** — WH/BH gating least established (p=0.48, corpus p20=0.54)

#### `prgm_const` — flag-gated, default-off, 949 loc, gimple
`gcc/config/riscv/tt/gimple-rvtt-prgm-const.cc`

- **CONCERN** — WH/BH gating least established (p=0.51, corpus p20=0.54)

### 1b. Passes that ship today — unconditional, or flag-gated with `Init(1)` — 12 flagged, 12 with an ALARM

Mandatory lowering, diagnosis, and enforcement, plus the few flags that default on (`dce`, `cc`). Discount `mutation_before_validation` and `ordering_fragility` here — they restate the pass's job. The transferable signals are **missing targeted tests** and **an absent named-refusal surface**.

#### `check_early` — **RUNS UNCONDITIONALLY**, 491 loc, gimple
`gcc/config/riscv/tt/gimple-rvtt-check.cc`

- **CONCERN** — fail-closed discipline weakest in the corpus (p=0.04, corpus p20=0.12)
- **CONCERN** — largest apparent gap between contract and code (p=0.43, corpus p20=0.58)
- **CONCERN** — WH/BH gating least established (p=0.36, corpus p20=0.54)
- **CONCERN** — may mutate IR before validation completes (1.60/3, corpus p80=1.52)
- **ALARM** — failed checks may be silently dropped (2.25/3, corpus p80=1.86)
- **ALARM** — most fragile to pass reordering (2.46/3, corpus p80=2.40)
- **CONCERN** — thinnest test coverage in the corpus (0.01/3, p20=0.02)
- **ALARM** — the above ships on every compile: this pass is NOT behind an off-by-default flag

#### `check_late` — **RUNS UNCONDITIONALLY**, 491 loc, gimple
`gcc/config/riscv/tt/gimple-rvtt-check.cc`

- **CONCERN** — fail-closed discipline weakest in the corpus (p=0.04, corpus p20=0.12)
- **CONCERN** — largest apparent gap between contract and code (p=0.50, corpus p20=0.58)
- **CONCERN** — WH/BH gating least established (p=0.36, corpus p20=0.54)
- **CONCERN** — may mutate IR before validation completes (1.56/3, corpus p80=1.52)
- **ALARM** — failed checks may be silently dropped (2.37/3, corpus p80=1.86)
- **CONCERN** — thinnest test coverage in the corpus (0.02/3, p20=0.02)
- **ALARM** — the above ships on every compile: this pass is NOT behind an off-by-default flag

#### `expand` — **RUNS UNCONDITIONALLY**, 854 loc, gimple
`gcc/config/riscv/tt/gimple-rvtt-expand.cc`

- **CONCERN** — fail-closed discipline weakest in the corpus (p=0.10, corpus p20=0.12)
- **CONCERN** — WH/BH gating least established (p=0.52, corpus p20=0.54)
- **ALARM** — may mutate IR before validation completes (2.17/3, corpus p80=1.52)
- **ALARM** — most fragile to pass reordering (2.74/3, corpus p80=2.40)
- **ALARM** — the above ships on every compile: this pass is NOT behind an off-by-default flag

#### `synth_renumber` — **RUNS UNCONDITIONALLY**, 844 loc, gimple
`gcc/config/riscv/tt/gimple-rvtt-synth.cc`

- **CONCERN** — fail-closed discipline weakest in the corpus (p=0.12, corpus p20=0.12)
- **CONCERN** — may mutate IR before validation completes (1.65/3, corpus p80=1.52)
- **ALARM** — failed checks may be silently dropped (2.03/3, corpus p80=1.86)
- **ALARM** — most fragile to pass reordering (2.61/3, corpus p80=2.40)
- **CONCERN** — thinnest test coverage in the corpus (0.02/3, p20=0.02)
- **ALARM** — the above ships on every compile: this pass is NOT behind an off-by-default flag

#### `unspec_prop_ssa` — **RUNS UNCONDITIONALLY**, 300 loc, gimple
`gcc/config/riscv/tt/gimple-rvtt-unspec.cc`

- **CONCERN** — fail-closed discipline weakest in the corpus (p=0.11, corpus p20=0.12)
- **CONCERN** — may mutate IR before validation completes (1.52/3, corpus p80=1.52)
- **ALARM** — failed checks may be silently dropped (1.92/3, corpus p80=1.86)
- **ALARM** — most fragile to pass reordering (2.68/3, corpus p80=2.40)
- **CONCERN** — thinnest test coverage in the corpus (0.02/3, p20=0.02)
- **ALARM** — the above ships on every compile: this pass is NOT behind an off-by-default flag

#### `synth_split` — **RUNS UNCONDITIONALLY**, 844 loc, gimple
`gcc/config/riscv/tt/gimple-rvtt-synth.cc`

- **CONCERN** — fail-closed discipline weakest in the corpus (p=0.09, corpus p20=0.12)
- **CONCERN** — largest apparent gap between contract and code (p=0.58, corpus p20=0.58)
- **ALARM** — failed checks may be silently dropped (2.08/3, corpus p80=1.86)
- **ALARM** — most fragile to pass reordering (2.54/3, corpus p80=2.40)
- **ALARM** — the above ships on every compile: this pass is NOT behind an off-by-default flag

#### `fix_ebreak` — **RUNS UNCONDITIONALLY**, 152 loc, rtl
`gcc/config/riscv/tt/rtl-rvtt-fix-ebreak.cc`

- **CONCERN** — fail-closed discipline weakest in the corpus (p=0.12, corpus p20=0.12)
- **CONCERN** — largest apparent gap between contract and code (p=0.43, corpus p20=0.58)
- **CONCERN** — WH/BH gating least established (p=0.42, corpus p20=0.54)
- **ALARM** — rewrites IR with equivalence merely asserted — no argument, no proof artifact (conf 0.69)
- **ALARM** — the above ships on every compile: this pass is NOT behind an off-by-default flag

#### `immload_shorten` — **RUNS UNCONDITIONALLY**, 948 loc, gimple
`gcc/config/riscv/tt/gimple-rvtt-immvar.cc`

- **ALARM** — failed checks may be silently dropped (1.96/3, corpus p80=1.86)
- **ALARM** — most fragile to pass reordering (2.40/3, corpus p80=2.40)
- **CONCERN** — thinnest test coverage in the corpus (0.02/3, p20=0.02)
- **NOTE** — possibly asserted-without-argument, but the model did not settle (conf 0.33) — treat as unresolved
- **ALARM** — the above ships on every compile: this pass is NOT behind an off-by-default flag

#### `noval_elide` — **RUNS UNCONDITIONALLY**, 154 loc, gimple
`gcc/config/riscv/tt/gimple-rvtt-noval.cc`

- **CONCERN** — largest apparent gap between contract and code (p=0.58, corpus p20=0.58)
- **CONCERN** — WH/BH gating least established (p=0.52, corpus p20=0.54)
- **ALARM** — most fragile to pass reordering (2.44/3, corpus p80=2.40)
- **CONCERN** — thinnest test coverage in the corpus (0.02/3, p20=0.02)
- **ALARM** — the above ships on every compile: this pass is NOT behind an off-by-default flag

#### `synth_opcode` — **RUNS UNCONDITIONALLY**, 284 loc, rtl
`gcc/config/riscv/tt/rtl-rvtt-synth.cc`

- **CONCERN** — fail-closed discipline weakest in the corpus (p=0.10, corpus p20=0.12)
- **CONCERN** — WH/BH gating least established (p=0.52, corpus p20=0.54)
- **ALARM** — most fragile to pass reordering (2.79/3, corpus p80=2.40)
- **ALARM** — the above ships on every compile: this pass is NOT behind an off-by-default flag

#### `lreg_livein` — **RUNS UNCONDITIONALLY**, 350 loc, rtl
`gcc/config/riscv/tt/rtl-rvtt-lreg-livein.cc`

- **CONCERN** — fail-closed discipline weakest in the corpus (p=0.07, corpus p20=0.12)
- **ALARM** — most fragile to pass reordering (2.68/3, corpus p80=2.40)
- **CONCERN** — thinnest test coverage in the corpus (0.02/3, p20=0.02)
- **ALARM** — the above ships on every compile: this pass is NOT behind an off-by-default flag

#### `fix_raw` — **RUNS UNCONDITIONALLY**, 249 loc, rtl
`gcc/config/riscv/tt/rtl-rvtt-fix-raw.cc`

- **CONCERN** — fail-closed discipline weakest in the corpus (p=0.07, corpus p20=0.12)
- **CONCERN** — largest apparent gap between contract and code (p=0.55, corpus p20=0.58)
- **CONCERN** — WH/BH gating least established (p=0.40, corpus p20=0.54)
- **CONCERN** — thinnest test coverage in the corpus (0.02/3, p20=0.02)
- **ALARM** — the above ships on every compile: this pass is NOT behind an off-by-default flag

## 2. Performance opportunity

`opportunity = upside x breadth x evidence_discount` — a pass already backed by silicon A/B is not an opportunity, it is done. Both factors are 0–3.

| pass | upside | breadth | mechanism | evidence | default | opportunity |
|---|---|---|---|---|---|---|
| `replay` | 2.93 | 2.16 | replay_compression | structural (0.22) | UNCOND | **5.70** |
| `immload_shorten` | 1.83 | 2.68 | fewer_delivered_words | none (0.63) | UNCOND | **4.90** |
| `immvar_expand` | 1.85 | 2.65 | fewer_delivered_words | none (0.68) | UNCOND | **4.90** |
| `live` | 1.71 | 2.74 | fewer_delivered_words | none (0.66) | UNCOND | **4.69** |
| `replay_reform` | 2.93 | 1.78 | replay_compression | structural (0.30) | UNCOND | **4.69** |
| `synth_split` | 1.68 | 2.63 | fewer_delivered_words | none (0.89) | UNCOND | **4.42** |
| `synth_cse` | 1.86 | 2.35 | fewer_delivered_words | none (0.91) | UNCOND | **4.37** |
| `dce` | 1.83 | 2.10 | fewer_delivered_words | none (0.85) | UNCOND | **3.84** |
| `schedule` | 2.36 | 2.67 | latency_hiding | simulator (0.66) | UNCOND | **3.78** |
| `expand` | 1.46 | 2.55 | fewer_delivered_words | none (0.90) | UNCOND | **3.72** |
| `synth_opcode` | 1.44 | 2.47 | fewer_delivered_words | none (0.79) | UNCOND | **3.56** |
| `cc` | 1.28 | 2.68 | fewer_delivered_words | none (0.54) | UNCOND | **3.43** |
| `combine` | 1.72 | 2.08 | fewer_delivered_words | structural (0.39) | UNCOND | **3.22** |
| `synth_renumber` | 1.24 | 2.43 | enables_downstream | none (0.94) | UNCOND | **3.01** |
| `replay_unroll` | 2.95 | 1.08 | enables_downstream | structural (0.37) | off | **2.87** |
| `unspec_prop_rtl` | 1.26 | 2.23 | fewer_delivered_words | none (0.87) | UNCOND | **2.81** |
| `lp_alloc` | 2.10 | 1.32 | register_pressure | none (0.45) | off | **2.77** |
| `launch_flatten` | 2.82 | 1.06 | enables_downstream | structural (0.47) | off | **2.69** |
| `immload_combine` | 1.34 | 1.91 | fewer_delivered_words | none (0.62) | UNCOND | **2.56** |
| `rmext` | 1.35 | 1.77 | fewer_delivered_words | none (0.48) | UNCOND | **2.39** |

## 3. Evidence against the Rescue Contract

`default_on_readiness` scores a pass against the Evidence-Based Rescue Contract: `0` = envelope unsettled, `3` = proven, differentialed, and measured. It means two different things depending on how the pass is gated, so the two populations are split.

### 3a. Promotion candidates (flag-gated)

For these, readiness is the actual promotion question: should the flag flip?

| pass | readiness | evidence | equivalence argument |
|---|---|---|---|
| `invariant` | 2.04 | silicon_ab | analytic_argument |
| `dst_autoincr` | 2.02 | silicon_ab | analytic_argument |
| `ccmask` | 2.00 | silicon_ab | exhaustive_proof_artifact |
| `lreg_rename_chains` | 1.93 | silicon_ab | analytic_argument |
| `replay` | 1.93 | structural | analytic_argument |
| `lut_select` | 1.90 | silicon_ab | analytic_argument |
| `lp_alloc` | 1.81 | none | analytic_argument |
| `mop_form` | 1.80 | simulator | analytic_argument |
| `store_fold` | 1.75 | silicon_ab | exhaustive_proof_artifact |
| `int_not` | 1.74 | structural | exhaustive_proof_artifact |
| `macro_planner` | 1.74 | simulator | analytic_argument |
| `replay_reform` | 1.74 | structural | analytic_argument |

### 3b. Already shipping, evidence short of the contract (unconditional)

These are not promotion candidates — they are mandatory lowering and enforcement that already run on **every** Tensix compilation. For them a low readiness score is the *inverse* observation: the contract's evidence bar is not met, and the code ships anyway. That is expected for mandatory lowering, which cannot be gated behind evidence. It is listed because it locates where the backend carries unevidenced risk by construction.

| pass | readiness | evidence | equivalence argument | targeted tests |
|---|---|---|---|---|
| `fix_ebreak` | 0.41 | none | asserted | 0.04/3 |
| `rmext` | 0.77 | none | analytic_argument | 1.01/3 |
| `check_late` | 0.82 | none | deliberate_repair | 0.02/3 |
| `check_early` | 0.84 | none | deliberate_repair | 0.01/3 |
| `noval_elide` | 0.84 | none | analytic_argument | 0.02/3 |
| `lreg_livein` | 0.86 | none | analytic_argument | 0.02/3 |
| `unspec_prop_ssa` | 1.03 | none | analytic_argument | 0.02/3 |
| `synth_opcode` | 1.04 | none | test_only | 0.98/3 |
| `expand` | 1.09 | none | analytic_argument | 0.03/3 |
| `synth_cse` | 1.10 | none | analytic_argument | 0.02/3 |
| `synth_renumber` | 1.10 | none | analytic_argument | 0.02/3 |
| `synth_split` | 1.14 | none | analytic_argument | 0.03/3 |

## 4. Suggested review order

| # | pass | priority | default | loc |
|---|---|---|---|---|
| 1 | `check_early` | 2.48 | UNCOND | 491 |
| 2 | `check_late` | 2.44 | UNCOND | 491 |
| 3 | `fix_ebreak` | 2.43 | UNCOND | 152 |
| 4 | `rmext` | 2.33 | UNCOND | 610 |
| 5 | `live` | 2.26 | UNCOND | 714 |
| 6 | `noval_elide` | 2.26 | UNCOND | 154 |
| 7 | `unspec_prop_ssa` | 2.21 | UNCOND | 300 |
| 8 | `synth_renumber` | 2.20 | UNCOND | 844 |
| 9 | `expand` | 2.19 | UNCOND | 854 |
| 10 | `fix_raw` | 2.18 | UNCOND | 249 |
| 11 | `synth_split` | 2.17 | UNCOND | 844 |
| 12 | `cc` | 2.15 | UNCOND | 390 |
| 13 | `synth_cse` | 2.14 | UNCOND | 844 |
| 14 | `unspec_prop_rtl` | 2.14 | UNCOND | 303 |
| 15 | `immvar_expand` | 2.13 | UNCOND | 948 |

## 5. Evidence census

Counted two ways. *Settled* counts only answers the model actually committed to (confidence >= 0.50); *unsettled* answers are shown separately rather than folded into a headline number. A raw count alone would report a 0.18-confidence coin flip as a fact.

**Strongest equivalence evidence** — 44 settled, 10 unsettled

- `analytic_argument`: 30
- `exhaustive_proof_artifact`: 4
- `not_a_transform`: 4
- `deliberate_repair`: 3
- `asserted`: 2
- `test_only`: 1

**Performance evidence** — 35 settled, 19 unsettled

- `none`: 28
- `silicon_ab`: 3
- `structural`: 2
- `simulator`: 2

**Primary performance mechanism** — 47 settled, 7 unsettled

- `fewer_delivered_words`: 31
- `correctness_only`: 6
- `enables_downstream`: 4
- `replay_compression`: 3
- `latency_hiding`: 2
- `register_pressure`: 1

## 6. Low-confidence answers (do not treat as findings)

222 of 864 answers came back below 0.50 confidence. The model is reporting that the state did not settle the question — usually because the evidence genuinely is not in the source.

| pass | question | answer | conf |
|---|---|---|---|
| `cc` | mutation_before_validation | 1.57 | 0.00 |
| `check_early` | mutation_before_validation | 1.6 | 0.00 |
| `check_late` | mutation_before_validation | 1.56 | 0.00 |
| `crosslane_window` | contract_precision | 1.18 | 0.00 |
| `dce` | silent_drop_risk | 1.38 | 0.00 |
| `dst_interleave` | mutation_before_validation | 1.05 | 0.00 |
| `fix_ebreak` | mutation_before_validation | 1.24 | 0.00 |
| `fix_ebreak` | ordering_fragility | 1.99 | 0.00 |
| `fix_raw` | mutation_before_validation | 1.08 | 0.00 |
| `fix_raw` | silent_drop_risk | 1.29 | 0.00 |
| `immload_combine` | mutation_before_validation | 1.16 | 0.00 |
| `immvar_expand` | mutation_before_validation | 1.54 | 0.00 |
| `int_abs` | silent_drop_risk | 1.01 | 0.00 |
| `live` | mutation_before_validation | 1.6 | 0.00 |
| `lp_schedule` | perf_upside | 1.85 | 0.00 |
| `lp_schedule_prera` | silent_drop_risk | 1.0 | 0.00 |
| `lreg_livein` | mutation_before_validation | 1.02 | 0.00 |
| `noval_elide` | silent_drop_risk | 1.41 | 0.00 |
| `replay_reform` | mutation_before_validation | 1.03 | 0.00 |
| `replay_reform` | ordering_fragility | 1.97 | 0.00 |
| `store_fold` | silent_drop_risk | 1.04 | 0.00 |
| `synth_cse` | mutation_before_validation | 1.43 | 0.00 |
| `synth_opcode` | mutation_before_validation | 1.11 | 0.00 |
| `synth_renumber` | mutation_before_validation | 1.65 | 0.00 |
| `synth_split` | mutation_before_validation | 1.49 | 0.00 |

---

## Reproducing

```sh
source <your-secrets-file>   # TYPESAFE_API_KEY
cd scripts/rvtt-pass-audit
./.venv/bin/python extract_cards.py        # tree -> results/cards.json
./.venv/bin/python score_passes.py         # cards -> results/scores.json
./.venv/bin/python report.py               # scores -> results/SCOREBOARD.md
```

Scoring is cached on a hash of (state, questions, model), so a re-run after editing one pass re-scores only that pass. Fresh run cost: 812,412 input / 25,095 output tokens.