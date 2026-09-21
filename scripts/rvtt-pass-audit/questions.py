#!/usr/bin/env python3
"""The judgment set applied to every rvtt pass.

Design rules followed here (from the TypeSafe System One guidance):

  * One narrow judgment per question.  Nothing that conflates two signals.
  * All questions are independent over the same state, so they go in one
    batched request per pass and cannot see one another's answers.
  * Criteria describe concrete situations and stand on their own.
  * Every Choice carries a no-match outcome, so the model is never forced
    into a wrong bucket.
  * Questions reference state by backticked path.

What this instrument is: a triage ranking over 54 passes on consistent
dimensions.  What it is not: a proof.  Soundness of these transforms is
established by `tt/proofs/`, the testsuite, and silicon A/B -- not here.
A judgment is a prior on where scarce human review time pays off.
"""

from __future__ import annotations

from typesafe_sdk import Choice, Noul, Score

# --------------------------------------------------------------------------
# Shared backend context.  Identical for every pass, so a judgment about
# performance is grounded in the machine rather than in generic compiler
# folklore.  Facts only; sourced from rvtt-passes.def, riscv.opt, and
# SFPI_COMPILER_UPGRADE.md.
# --------------------------------------------------------------------------

BACKEND_CONTEXT = {
    "target": "Tenstorrent Tensix coprocessor, SFPU vector unit (Wormhole/Blackhole)",
    "register_file": (
        "The SFPU is a 32-lane SIMD engine with only eight variable vector "
        "registers L0-L7 and NO hardware or software spill mechanism. Exceeding "
        "eight simultaneously-live values is a fatal internal compiler error, "
        "not a slowdown. Register pressure is a correctness constraint here, "
        "not a performance one."
    ),
    "cost_model": (
        "Cost is dominated by DELIVERED WORDS: each SFPU instruction word issued "
        "from the RISC-V core to the Tensix coprocessor occupies issue bandwidth. "
        "Reducing the number of delivered words is the primary performance lever. "
        "Secondary levers: filling instruction shadows (latency hiding), replay "
        "buffer / SFPLOADMACRO compression (one launch word replays many recorded "
        "words), and avoiding serial CC dependence spines."
    ),
    "correctness_regime": (
        "Kernels are bit-exactness sensitive. A transform that changes floating "
        "point results, NaN sign classes, denormal handling, or lane-mask "
        "semantics is a correctness defect even when numerically 'close'. "
        "Several passes cite exhaustive 2^32 per-lane equivalence sweeps in "
        "tt/proofs/ as their soundness evidence."
    ),
    "refusal_discipline": (
        "The backend has a frozen registry of 540 named refusals "
        "(rvtt-refusals.def). The intended discipline is fail-closed: when a "
        "precondition cannot be proven, the pass must refuse BY NAME and leave "
        "the IR byte-identical, rather than proceeding on a weaker assumption."
    ),
    "deployment_contract": (
        "Default-on promotion is governed by an Evidence-Based Rescue Contract: "
        "a strict region allowlist, a bypass that leaves ineligible regions "
        "byte-identical, independent validation of any proposed rewrite, "
        "whole-corpus differential testing, and a tested rollback flag."
    ),
}


# --------------------------------------------------------------------------
# SOUNDNESS -- does the pass's own safety argument hold together?
# --------------------------------------------------------------------------

SOUNDNESS = {
    "contract_precision": Score(
        instructions=(
            "Judge how precisely `pass.contract_comment` states the condition "
            "under which this transform preserves program semantics. Consider "
            "only the stated contract, not whether the code implements it."
        ),
        criteria=[
            "No semantic contract is stated; the comment describes only what the pass does mechanically or why it is fast.",
            "A contract is gestured at in prose but the preconditions are vague enough that two engineers could disagree about whether a given region qualifies.",
            "Preconditions are enumerated concretely and an equivalence argument is sketched, but some step is asserted rather than shown.",
            "Preconditions are enumerated concretely and the equivalence argument is complete and checkable, including the edge encodings (zeros, NaN sign classes, infinities, denormals) where those are relevant.",
        ],
    ),
    "evidence_strength": Choice(
        instructions=(
            "What is the STRONGEST form of semantic-equivalence evidence this pass "
            "actually offers, considering `pass.contract_comment`, "
            "`pass.proofs_cited`, and `pass.source`?"
        ),
        criteria={
            "exhaustive_proof_artifact": "Cites a checked-in exhaustive or machine-checked equivalence artifact (e.g. a sweep over all 2^32 lane encodings) that covers this transform.",
            "analytic_argument": "Gives a complete written equivalence argument in the source, but no checked-in proof artifact backs it.",
            "test_only": "Offers no equivalence argument; correctness rests on the cited testcases.",
            "asserted": "The transform is asserted to be semantics-preserving with neither argument nor meaningful tests.",
            "deliberate_repair": "The pass edits IR as ERROR RECOVERY after diagnosing a user error -- substituting an in-range value, deleting an offending statement -- so preserving the original semantics is explicitly not the goal. Equivalence evidence does not apply.",
            "not_a_transform": "This pass does not rewrite IR (it is analysis, annotation, diagnosis, or dump-only), so equivalence evidence does not apply.",
        },
    ),
    "fail_closed": Noul(
        instructions=(
            "When a precondition cannot be established, does this pass leave the IR "
            "unchanged and refuse -- rather than proceeding under a weaker assumption? "
            "Judge from `pass.source` and `pass.refusals_cited`."
        ),
        criteria={
            "true": "Every path that cannot prove its precondition bails out leaving the IR as it found it, typically recording a named refusal.",
            "false": "At least one path proceeds with a rewrite, a default, or a fallback value when its precondition was not established.",
        },
    ),
    "rationale_matches_code": Noul(
        instructions=(
            "Does `pass.source` actually implement the contract described in "
            "`pass.contract_comment` and the ordering claim in "
            "`pass.ordering_rationale`? Answer no if the code is materially "
            "narrower, broader, or different from what the comments describe."
        ),
        criteria={
            "true": "The implemented conditions and rewrite match the documented contract; any divergence is immaterial.",
            "false": "The code admits cases the contract excludes, excludes cases the contract admits, or performs a different rewrite than described.",
        },
    ),
    "mutation_before_validation": Score(
        instructions=(
            "Assess the risk that `pass.source` mutates IR, or dereferences a "
            "candidate's position, BEFORE every validity condition for that "
            "mutation has been checked. This backend has a history of exactly "
            "this defect class."
        ),
        criteria=[
            "All validity conditions are established first; mutation happens in a single committed step after the decision is final.",
            "Mutation follows validation, but some state is captured or a pointer dereferenced before the last check, with no observed way for that to be invalid.",
            "There is a path where IR is edited and then conditionally undone, so correctness depends on the undo being exact.",
            "There is a path that edits IR or dereferences a candidate position before a condition that can genuinely fail is checked.",
        ],
    ),
    "ordering_fragility": Score(
        instructions=(
            "How fragile is this pass to changes in pass ordering? Consider what "
            "`pass.ordering_rationale` says it depends on, and whether "
            "`pass.source` would silently misbehave -- rather than refuse or "
            "assert -- if an upstream pass stopped producing the expected form."
        ),
        criteria=[
            "Order-independent, or it re-derives everything it needs and refuses on an unexpected form.",
            "Depends on an upstream form but detects violations and refuses by name.",
            "Depends on an upstream form and would produce a missed optimization, but not wrong code, if that form changed.",
            "Depends on an upstream invariant that is documented only in a comment; if it were violated the pass would silently produce wrong code.",
        ],
    ),
}


# --------------------------------------------------------------------------
# CORRECTNESS -- the concrete defect surface
# --------------------------------------------------------------------------

CORRECTNESS = {
    "arch_gating_correct": Noul(
        instructions=(
            "This transform's validity may differ between Wormhole and Blackhole "
            "(different SFPU instruction availability and semantics). Considering "
            "`pass.gate` and `pass.source`, is the pass correctly restricted to "
            "the architectures on which its rewrite is actually valid?"
        ),
        criteria={
            "true": "Either the transform is architecture-neutral, or every architecture-specific rewrite is guarded by the corresponding target test.",
            "false": "An architecture-specific rewrite can fire on an architecture where its instruction or semantics differ, with no guard.",
        },
    ),
    "silent_drop_risk": Score(
        instructions=(
            "Assess the risk that a FAILED check inside `pass.source` is silently "
            "swallowed -- the pass continues, or reports success, without the "
            "failure reaching a refusal record, a diagnostic, or an abort."
        ),
        criteria=[
            "Every failed check reaches a named refusal or a hard error; nothing is dropped.",
            "Failed checks are recorded, though a few take an unnamed generic path that is still observable in the dump.",
            "At least one failed check causes a quiet bail-out that leaves no trace in any dump or diagnostic.",
            "At least one failed check is discarded while the pass continues on a path that still commits a change.",
        ],
    ),
    "transactional_commit": Choice(
        instructions=(
            "Does this pass commit its changes transactionally -- either the whole "
            "proposed rewrite lands, or the function is left in its original state? "
            "Judge from `pass.source`."
        ),
        criteria={
            "transactional": "Changes are staged and committed as a unit, or each individual edit is independently valid on its own.",
            "partial_possible": "A multi-step rewrite can fail partway and leave the function in a state that is neither the original nor the intended result.",
            "no_ir_rewrite": "The pass does not rewrite IR, so the question does not apply.",
        },
    ),
    "test_adequacy": Score(
        instructions=(
            "Given the transform implemented in `pass.source`, how well is the "
            "transform's decision surface covered? Weigh refusal paths and edge "
            "conditions, not just the happy path. `pass.tests.direct` lists tests "
            "naming this pass's dump or flags; `pass.tests.by_refusal` lists tests "
            "scanning for refusal names this pass emits; "
            "`pass.tests.implicit_whole_suite` is nonzero when this pass is "
            "unconditional and therefore runs in every test in the suite -- broad "
            "exercise, but not targeted coverage of its decisions."
        ),
        criteria=[
            "No targeted tests; nothing exercises this pass's specific decisions.",
            "The main positive transform is tested; refusal paths and edge conditions are not.",
            "Positive transform plus several refusal paths are tested, with visible gaps in the edge conditions the contract calls out.",
            "Positive transform, the flag-off byte-identical case, and the named refusal paths are each covered, including the edge conditions the contract calls out.",
        ],
    ),
}


# --------------------------------------------------------------------------
# PERFORMANCE POTENTIAL on Tenstorrent silicon
# --------------------------------------------------------------------------

PERFORMANCE = {
    "perf_mechanism": Choice(
        instructions=(
            "What is this pass's PRIMARY mechanism for improving performance on "
            "the SFPU, given `backend.cost_model`? Pick the dominant one."
        ),
        criteria={
            "fewer_delivered_words": "Directly reduces the count of SFPU instruction words issued for the same computation.",
            "register_pressure": "Reduces simultaneously-live vector values, whose first-order effect is avoiding a fatal spill rather than saving cycles.",
            "latency_hiding": "Reorders or interleaves so that instruction shadows are filled and stalls are absorbed.",
            "replay_compression": "Enables replay-buffer or SFPLOADMACRO compression, so one launch word stands in for many recorded words.",
            "enables_downstream": "Produces no direct win itself; its value is canonicalizing the stream so a later pass can act.",
            "correctness_only": "Exists for correctness, diagnosis, or enforcement, with no performance intent.",
        },
    ),
    "perf_upside": Score(
        instructions=(
            "Estimate the magnitude of the silicon win this pass can deliver on a "
            "representative SFPU kernel when it fires, judging the transform in "
            "`pass.source` against `backend.cost_model`. Judge the ceiling of the "
            "mechanism, not how often it fires."
        ),
        criteria=[
            "No direct performance effect; the pass is for correctness, diagnosis, or enforcement.",
            "Removes a small constant number of words or stalls from a region; single-digit percent on an affected kernel at best.",
            "Removes a meaningful fraction of a hot region's delivered words or stalls; a clear double-digit percent win on kernels where it fires.",
            "Restructures the delivery of a whole loop body -- replay or MOP compression, complete unroll and re-roll, or rescuing a kernel that otherwise cannot compile at all.",
        ],
    ),
    "applicability_breadth": Score(
        instructions=(
            "How broadly across a realistic TT-LLK SFPU kernel corpus will this "
            "pass's preconditions actually be met? Judge firing frequency, not win "
            "size. Weigh how narrow the pattern in `pass.source` is and how many "
            "refusals stand between a candidate and a commit."
        ),
        criteria=[
            "A single hand-written shape; expect it to fire on roughly one kernel family.",
            "A recognizable but narrow idiom; expect a handful of kernels.",
            "A common idiom in vector kernel code; expect it to fire across many kernels.",
            "Structural and near-universal; expect it on essentially every SFPU kernel.",
        ],
    ),
    "measurement_evidence": Choice(
        instructions=(
            "What performance evidence does this pass actually have, judging from "
            "`pass.contract_comment`, `pass.source`, and `pass.tests`? Do not credit "
            "evidence that is merely planned or described as future work."
        ),
        criteria={
            "silicon_ab": "Cites a paired A/B measurement on real silicon with this transform as the only variable.",
            "simulator": "Cites simulator or cost-model measurement, but not silicon.",
            "structural": "Shows only that the output stream is structurally better (fewer words, lower peak), with no timing measurement.",
            "none": "Offers no performance evidence of any kind.",
        },
    ),
    "default_on_readiness": Score(
        instructions=(
            "Judge this pass's readiness for default-on promotion against "
            "`backend.deployment_contract`. `pass.gating` records how it is turned "
            "on today: mode `unconditional` means it already runs on every Tensix "
            "compilation. Judge the evidence and the safety envelope, not the "
            "current flag value."
        ),
        criteria=[
            "Not ready: the transform's safety envelope itself is unsettled, or it can produce wrong code on an input it accepts.",
            "Not ready: the transform is locally well defended, but the corpus differential and any measurement are both absent.",
            "Close: local safety is well defended and there is structural or simulator evidence, but no paired silicon A/B on a changed binary.",
            "Ready: safety envelope is proven and enforced, a whole-corpus differential exists, and a paired measurement on a changed binary supports the promotion.",
        ],
    ),
}


# --------------------------------------------------------------------------
# CROSS-CUTTING
# --------------------------------------------------------------------------

TRIAGE = {
    "review_priority": Score(
        instructions=(
            "If a senior compiler engineer had time to deeply review only a few of "
            "these 54 passes before shipping, how high should THIS pass sit on that "
            "list? Weigh the blast radius of a defect against how well defended the "
            "pass already is. Consult `pass.gating`: a pass behind an off-by-default "
            "flag that refuses aggressively is lower priority than one that runs "
            "unconditionally on every Tensix compilation and rewrites broadly."
        ),
        criteria=[
            "Low: analysis-only or diagnostic, or so narrowly gated that a defect could barely escape.",
            "Moderate: a real transform, but behind an off-by-default flag with a well-defended envelope and good refusal coverage.",
            "High: runs unconditionally or by default, or is broad enough that a defect would reach many kernels, and some part of its argument is unproven.",
            "Urgent: there is a specific reason from the source to believe it can produce wrong code or an ICE on an input it currently accepts.",
        ],
    ),
}


ALL_QUESTIONS: dict[str, object] = {**SOUNDNESS, **CORRECTNESS, **PERFORMANCE, **TRIAGE}

GROUPS = {
    "soundness": list(SOUNDNESS),
    "correctness": list(CORRECTNESS),
    "performance": list(PERFORMANCE),
    "triage": list(TRIAGE),
}


def build_state(card: dict, source: str, source_status: str) -> dict:
    """Shape one pass card plus its source into the judgment state."""
    return {
        "backend": BACKEND_CONTEXT,
        "pass": {
            "name": card["pass"],
            "tier": card.get("tier"),
            "pipeline_anchor": f"{card.get('insert_direction')} {card.get('anchor')}",
            "pass_kind": card.get("pass_kind"),
            "dump_name": card.get("dump_name"),
            "file": card.get("file"),
            "loc": card.get("loc"),
            "ordering_rationale": card.get("ordering_rationale"),
            "options": card.get("options"),
            "gating": card.get("gating"),
            "always_on": card.get("always_on"),
            "gate": card.get("gate"),
            "contract_comment": card.get("contract_comment"),
            "refusals_cited": card.get("refusals_cited"),
            "proofs_cited": card.get("proofs_cited"),
            "ir_mutation_census": card.get("ir_mutation_census"),
            "tests": card.get("tests"),
            "split_siblings": [s["file"] for s in card.get("split_siblings", [])],
            "source_status": source_status,
            "source": source,
        },
    }
