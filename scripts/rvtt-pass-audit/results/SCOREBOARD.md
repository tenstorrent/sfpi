# rvtt pass audit — soundness, correctness, performance potential

- **Tree**: `sfpi-gcc` @ `827598a71b72`
- **Passes scored**: 54 of 55 registered (`pass_dce` is generic GCC DCE, not an rvtt pass)
- **Model**: `jev-1.13.0` — 16 independent judgments per pass, one batched request each
- **Corpus context**: 540 named refusals, 11 proof artifacts, 97 `-mtt-tensix-*` options, 1542 TT testcases
- **Silicon**: `KNOB-SILICON-20260920.tsv` + `COMPOSITION-SILICON-20260921.tsv` — 43 measured rows, 17/17 knobs attributed to 15 passes

> **How to read this.** Section 1 is *measurement* — real QB2 silicon, and the only part here that is evidence. Sections 2 onward are *model judgment*: calibrated probabilities over a rubric, which prove nothing and exist to rank where review time goes. Where the two disagree, the silicon wins. The ranked risk queue is **held** — see section 5.

## 1. Measured silicon — the authoritative perf column

Sign convention is the board's: **negative is a win** (fewer cycles vs hand). Values are composition best where present, else single-knob knob_vs_hand. A knob is attributed to a pass by `knob → -mtt-tensix-* option → option Var → file reading it → pass owning that file`; every link is mechanical.

### 1a. Per knob

| knob | rows | median | best | wins | regressions | pass |
|---|---|---|---|---|---|---|
| `loop-prgm-reclaim` | 2 | **-26.90** | -48.51 | 2 | 0 | `prgm_const` |
| `crossloop-cc-peel` | 2 | **-17.77** | -17.77 | 2 | 0 | `prgm_const` |
| `dst-autoincr-load-carrier` | 1 | **-13.07** | -13.07 | 1 | 0 | `dst_autoincr` |
| `reassoc-mad-restructure` | 5 | **-9.93** | -47.87 | 4 | 1 | `combine` |
| `crossrow-2datum` | 5 | **-9.64** | -27.15 | 3 | 2 | `lp_schedule_prera`, `schedule` |
| `int-abs` | 1 | **-5.31** | -5.31 | 1 | 0 | `int_abs` |
| `list-schedule` | 1 | **-2.95** | -2.95 | 1 | 0 | `lp_schedule_prera`, `schedule` |
| `stochrnd-store-fold` | 9 | **-2.76** | -17.37 | 6 | 3 | `store_fold` |
| `post-autoincr-window` | 2 | **-2.18** | -20.92 | 1 | 1 | `replay`, `replay_reform` |
| `delivery-shape` | 6 | **-1.14** | -25.05 | 3 | 3 | `delivery_shape` |
| `crosscall-config-prefix` | 1 | **-0.86** | -0.86 | 1 | 0 | `crosscall` |
| `launch-flatten` | 1 | **-0.82** | -0.82 | 1 | 0 | `launch_flatten`, `replay_unroll`, `round_interleave` |
| `store-sink` | 2 | **-0.38** | -0.40 | 2 | 0 | `prgm_const`, `store_fold` |
| `crosscall-addrmod` | 1 | **+0.08** | +0.08 | 0 | 1 | `crosscall`, `dst_autoincr` |
| `crossrow-pairing-seed` | 2 | **+1.26** | +1.26 | 0 | 2 | `lp_schedule_prera`, `schedule` |
| `counted-capture-peel` | 1 | **+1.98** | +1.98 | 0 | 1 | `replay`, `replay_reform` |
| `lut-select-leaf-ext` | 1 | **+2.69** | +2.69 | 0 | 1 | `lut_select` |

**Two caveats on attribution.** The chain resolves *which pass reads the flag*, which is not always *which pass implements the win*: `reassoc-mad-restructure` resolves to `combine`, because its Var is consulted in `gimple-rvtt-combine.cc` where the `mul+add -> mad` rule actually fires, not in `gimple-rvtt-reassoc.cc` which does the rebalancing that feeds it. Likewise `crossloop-cc-peel` resolves to `prgm_const` via `gimple-rvtt-prgm-residency.cc`, not to `pass_rvtt_crossloop`. Separately, `crossrow-2datum` is the one knob with **no option of that name** — it is hand-aliased to `-mtt-tensix-optimize-crossrow-pairing` in `measured.py`, so its 5 rows rest on my inference, not on a mechanical match. Check that alias before acting on its −9.64.

**Row count and yield pull in opposite directions.** The two most widely attributed knobs — `stochrnd-store-fold` (9 rows) and `delivery-shape` (6) — have the weakest medians in the set. The strongest medians sit on knobs with two to five rows: `loop-prgm-reclaim`, `crossloop-cc-peel`, `reassoc-mad-restructure`, `crossrow-2datum`. Breadth of firing is not the same as size of win, and a median over two rows is a thin basis for a promotion decision either way.

### 1b. Per pass

| pass | knobs | rows | median | best | ships today |
|---|---|---|---|---|---|
| `prgm_const` | crossloop-cc-peel, loop-prgm-reclaim, store-sink | 6 | **-11.53** | -48.51 | no |
| `combine` | reassoc-mad-restructure | 5 | **-9.93** | -47.87 | yes |
| `dst_autoincr` | crosscall-addrmod, dst-autoincr-load-carrier | 2 | **-6.50** | -13.07 | no |
| `int_abs` | int-abs | 1 | **-5.31** | -5.31 | no |
| `store_fold` | stochrnd-store-fold, store-sink | 11 | **-2.36** | -17.37 | no |
| `delivery_shape` | delivery-shape | 6 | **-1.14** | -25.05 | no |
| `lp_schedule_prera` | crossrow-2datum, crossrow-pairing-seed, list-schedule | 8 | **-0.85** | -27.15 | no |
| `schedule` | crossrow-2datum, crossrow-pairing-seed, list-schedule | 8 | **-0.85** | -27.15 | yes |
| `launch_flatten` | launch-flatten | 1 | **-0.82** | -0.82 | no |
| `replay_unroll` | launch-flatten | 1 | **-0.82** | -0.82 | no |
| `round_interleave` | launch-flatten | 1 | **-0.82** | -0.82 | no |
| `crosscall` | crosscall-addrmod, crosscall-config-prefix | 2 | **-0.39** | -0.86 | no |
| `replay` | counted-capture-peel, post-autoincr-window | 3 | **+1.98** | -20.92 | yes |
| `replay_reform` | counted-capture-peel, post-autoincr-window | 3 | **+1.98** | -20.92 | yes |
| `lut_select` | lut-select-leaf-ext | 1 | **+2.69** | +2.69 | yes |

**39 of 54 passes have no measured row at all.** For those, everything below is estimate, not evidence.

## 2. Estimated opportunity where no measurement exists

`upside x breadth`, both 0–3, from the model reading the source. This is a **prior for choosing what to measure next**, not a result. Passes with silicon rows are excluded — for those, section 1 is the answer.

| pass | upside | breadth | mechanism | ships | est. opportunity |
|---|---|---|---|---|---|
| `immload_shorten` | 1.85 | 2.69 | fewer_delivered_words | yes | **4.98** |
| `live` | 1.77 | 2.76 | fewer_delivered_words | yes | **4.89** |
| `immvar_expand` | 1.62 | 2.62 | enables_downstream | yes | **4.24** |
| `synth_split` | 1.62 | 2.60 | fewer_delivered_words | yes | **4.21** |
| `synth_cse` | 1.83 | 2.19 | fewer_delivered_words | yes | **4.01** |
| `expand` | 1.53 | 2.61 | fewer_delivered_words | yes | **3.99** |
| `dce` | 1.83 | 2.12 | fewer_delivered_words | yes | **3.88** |
| `synth_opcode` | 1.51 | 2.42 | fewer_delivered_words | yes | **3.65** |
| `cc` | 1.29 | 2.71 | fewer_delivered_words | yes | **3.50** |
| `synth_renumber` | 1.25 | 2.43 | fewer_delivered_words | yes | **3.04** |
| `unspec_prop_rtl` | 1.28 | 2.23 | fewer_delivered_words | yes | **2.85** |
| `lp_alloc` | 2.17 | 1.30 | register_pressure | no | **2.82** |
| `immload_combine` | 1.36 | 1.94 | fewer_delivered_words | yes | **2.64** |
| `macro_planner` | 2.43 | 1.69 | replay_compression | no | **2.46** |
| `rmext` | 1.31 | 1.74 | fewer_delivered_words | yes | **2.28** |

## 3. Test-coverage inversion (verified independently of the model)

This one was confirmed by grep over the testsuite, not taken from a probability. Counting testcases that name a pass's dump:

| pass | ships on every compile | tests naming its dump |
|---|---|---|
| `rvtt_macro_planner` | no | 186 |
| `rvtt_replay` | yes | 160 |
| `rvtt_prgm_const` | no | 142 |
| `rvtt_dst_autoincr` | no | 103 |
| `rvtt_expand` | **yes** | 0 |
| `rvtt_live` | **yes** | 0 |
| `rvtt_check` | **yes** | 0 |
| `rvtt_synth_cse` | **yes** | 0 |

The passes that run on **every** Tensix compilation are the ones with no targeted tests, while the optional, off-by-default optimizations carry hundreds. That is exactly inverted from where coverage does the most good, and it stands on its own evidence.

## 4. Evidence census

Counted two ways. *Settled* counts only answers the model committed to (confidence >= 0.50); *unsettled* are shown separately rather than folded into a headline number, so a 0.18-confidence coin flip is not reported as a fact.

**Strongest equivalence evidence** — 43 settled, 11 unsettled

- `analytic_argument`: 30
- `exhaustive_proof_artifact`: 4
- `not_a_transform`: 4
- `deliberate_repair`: 3
- `test_only`: 1
- `asserted`: 1

**Primary performance mechanism** — 45 settled, 9 unsettled

- `fewer_delivered_words`: 30
- `correctness_only`: 6
- `enables_downstream`: 4
- `latency_hiding`: 2
- `replay_compression`: 2
- `register_pressure`: 1

The model's own `measurement_evidence` judgment is deliberately **not** reported here. Section 1 supersedes it: guessing from source comments whether a pass was ever measured is a proxy, and the board rows are the fact.

## 5. Ranked risk queue — HELD, not published

The soundness/correctness ranking is **withheld from this report on purpose.**

Its risk questions assume an *optional transform with a precondition to check before acting*. That frame fits the opt-in optimizations and does not fit mandatory lowering: `rvtt_expand`'s job is to rewrite every condition tree it sees, so "mutates IR before validation completes" scores high for it as a restatement of its purpose, not as a defect. Hand-checking confirmed that reading — `expand` carries zero named refusals and three asserts, by design.

That is the same class of error as two flags already caught and fixed during development (22 passes mislabeled default-off; `check_early/late` forced into an "asserted equivalence" bucket that had no outcome for deliberate error-recovery). Three instances of one failure mode is a reason to fix the instrument, not to ship its ranking.

**Unblocking it needs a second question set written for lowering and enforcement passes** — one that asks about assert density, refusal-surface absence, and diagnostic reachability instead of precondition handling. The raw per-pass answers are in `results/scores.json` for anyone who wants them; they should not be read as a ranked defect list.

## 6. Limits of this run

- **Source coverage.** A pass split across translation units behind a private `-int.h` is sent with all its siblings. 9 of 54 passes exceeded the model's 32k-token state limit and had siblings reduced to a structural digest (contract comment, signatures, and every IR-mutating call site with context). Those passes' judgments saw less code; `source_status` in `scores.json` records which.
- **Unowned source.** ~25.5k lines of `tt/*.cc` belong to no single registered pass (shared tables, cost models, generators) and are scored by nobody.
- **Run-to-run variance.** Scoring is not deterministic; the sub-0.50-confidence count moved between 222 and 230 across repeated full runs. Do not read a 0.05 difference between two passes as meaningful.
- **Unsettled answers.** 230 of 864 answers came back below 0.50 confidence and are excluded from every count above.

---

## Reproducing

```sh
source <your-secrets-file>   # TYPESAFE_API_KEY
cd scripts/rvtt-pass-audit
./.venv/bin/python extract_cards.py       # tree    -> results/cards.json
./.venv/bin/python measured.py            # board   -> results/measured.json
./.venv/bin/python score_passes.py        # cards   -> results/scores.json
./.venv/bin/python report.py              # all     -> results/SCOREBOARD.md
```

Fresh run cost: 871,589 input / 25,092 output tokens across 54 requests. Scoring caches on a hash of (state, questions, model).