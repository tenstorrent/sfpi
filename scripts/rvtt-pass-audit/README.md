# rvtt pass audit

A re-runnable triage instrument over the registered `rvtt` compiler passes.
It extracts a deterministic "pass card" per pass, asks a fixed set of typed
judgments about it via TypeSafe System One (`jev`), and renders a scoreboard
ranking the passes on soundness, correctness, and performance potential.

## What this is, and what it is not

**It is** a consistent ranking across all 54 passes, so scarce human review
time goes where it pays off, and so a claim made in one pass's header comment
can be compared against the same claim in another's.

**It is not** a verifier. System One returns calibrated probabilities over a
rubric. It does not prove that a transform is semantics-preserving.
Soundness in this backend is established by `gcc/config/riscv/tt/proofs/`,
the 1542-test suite, and paired silicon A/B — not by this tool. Every flag
here is a prior, to be confirmed against the source before anyone acts on it.

Two findings from the first run were hand-checked and turned out to be
artifacts of *this tool*, not defects in the compiler. Both are now fixed,
and both are worth knowing about before trusting a future run:

- `expand`, `live`, and `check_early/late` were initially reported as
  "default-off" because no `-mtt-tensix-*` flag appears in their source. In
  fact their gate is a bare `return TARGET_XTT_TENSIX;` — they run on *every*
  Tensix compilation. The extractor now classifies gating explicitly.
- `check_early/late` were flagged for "equivalence merely asserted". They are
  diagnostic passes that deliberately repair what they diagnose; preserving
  the original semantics is explicitly not their goal. The
  `evidence_strength` question had no outcome for that, so the model was
  forced into a wrong bucket. It now has `deliberate_repair`.

## Known limitation

The risk questions (`mutation_before_validation`, `ordering_fragility`) assume
an **optional transform with a precondition to check before acting**. That
frame fits the opt-in optimizations. It does not fit mandatory lowering:
`expand`'s job is to rewrite every condition tree it sees, so "mutates before
validating" is tautological for it. The report therefore splits the review
queue by whether a pass ships today, and says which signals to discount in
which half. A second question set written specifically for lowering and
enforcement passes is the obvious next improvement.

## Layout

| file | role |
|---|---|
| `extract_cards.py` | tree → `results/cards.json`. Pure mechanical extraction: no model, no interpretation. Unknown fields are recorded as null rather than guessed. |
| `questions.py` | the 16 judgments, plus the shared Tensix/SFPU backend context every judgment sees |
| `score_passes.py` | one batched request per pass; caches on a hash of (state, questions, model) |
| `report.py` | scores → `results/SCOREBOARD.md`. All policy (thresholds, weights) lives here, so tuning re-renders without new inference. |

## Running

```sh
source <your-secrets-file>     # provides TYPESAFE_API_KEY
cd scripts/rvtt-pass-audit

./.venv/bin/python extract_cards.py         # ~20s, no network
./.venv/bin/python score_passes.py          # 54 requests, ~16s at 6 workers
./.venv/bin/python report.py                # instant
```

Useful flags:

```sh
./score_passes.py --dry-run                 # size/cost estimate, no calls
./score_passes.py --only pass_rvtt_ccmask   # one pass
./score_passes.py --no-cache                # force re-scoring
```

Scoring is cached on the state digest, so after editing one pass only that
pass is re-scored. Editing `questions.py` invalidates everything, by design.

## Cost

One full run: 54 requests, ~812k input / ~25k output tokens, about 16 seconds
wall clock at 6 workers. The largest single pass (`lreg_rename_chains`,
1966 loc) is ~31k input tokens.

## Design notes

**Why one request per pass.** All 16 questions are independent over the same
state, so they fan out in a single request and cannot see one another's
answers. That is both cheaper and the documented pattern.

**Why percentile flagging, not absolute thresholds.** On this corpus the
model's probability mass compresses toward the middle for the deep questions:
`rationale_matches_code` never exceeds 0.81 and `arch_gating_correct` never
exceeds 0.87 across all 54 passes. An absolute 0.80 cut flags 54 of 54 and
ranks nothing. Position relative to peers on the same question is what
carries signal, so a flag means "unusual among the 54", not "defective".

**Why confidence is reported, not hidden.** Roughly a quarter of the 864
answers come back below 0.50 confidence. Those are listed in their own
section and excluded from the census counts rather than folded into headline
numbers — otherwise a 0.18-confidence coin flip gets reported as a fact.

**Run-to-run variance is real.** Scoring is not deterministic: across repeated
full runs in one session the count of sub-0.50-confidence answers moved
between 222 and 230, and individual scores drift by a few hundredths. The
high-confidence categorical answers were stable; the marginal ones were not.
Do not read a 0.05 difference between two passes as meaningful, and re-run
before concluding that an edit changed a score. The cache exists partly so a
baseline stays fixed while you work.
