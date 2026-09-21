#!/usr/bin/env python3
"""Turn scores.json into a reviewable scoreboard.

Policy lives here, not in the judgments.  The raw probabilities stay
reusable: changing a threshold or a weight below re-renders the report
without re-running inference.

    ./report.py --scores results/scores.json --out results/SCOREBOARD.md
"""

from __future__ import annotations

import argparse
import json
from pathlib import Path

# Policy lives here, so it can be tuned without new inference.
#
# Calibration note (2026-09-21, jev-1.13.0, n=54): absolute cutoffs do not
# work on this corpus.  On the deep questions the model's probability mass
# compresses toward the middle -- `rationale_matches_code` never exceeds
# 0.81 and `arch_gating_correct` never exceeds 0.87 across all 54 passes.
# An absolute 0.80 cut therefore flags essentially everything and ranks
# nothing.  What carries signal is a pass's position RELATIVE TO ITS PEERS
# on the same question, so flagging is percentile-based.  A flag means
# "unusual among the 54", not "defective".
OUTLIER_PCTL = 0.20        # worst fifth of the distribution on a given question
ABS_RISK_FLOOR = 1.75      # a risk score must also clear this to raise an ALARM
LOW_CONFIDENCE = 0.50      # answers below this are unresolved, not findings
MAX_FLAGGED = 12           # a triage list longer than this is not a triage list


def noul(a):  return a["noul"]
def score(a): return a["score"]
def conf(a):  return a.get("confidence")


def pctl_threshold(values: list[float], q: float) -> float:
    v = sorted(values)
    if not v:
        return 0.0
    i = max(0, min(len(v) - 1, int(round(q * (len(v) - 1)))))
    return v[i]


def fmt_pass(name: str) -> str:
    return name.replace("pass_rvtt_", "")


def build(scores: list[dict]) -> dict:
    by = {s["pass"]: s["answers"] for s in scores}
    meta = {s["pass"]: s for s in scores}

    # Per-question outlier cuts, computed from this corpus.
    # "lower is worse" Nouls, and "higher is worse" risk scores.
    noul_qs = {
        "fail_closed": "fail-closed discipline weakest in the corpus",
        "rationale_matches_code": "largest apparent gap between contract and code",
        "arch_gating_correct": "WH/BH gating least established",
    }
    risk_qs = {
        "mutation_before_validation": "may mutate IR before validation completes",
        "silent_drop_risk": "failed checks may be silently dropped",
        "ordering_fragility": "most fragile to pass reordering",
    }

    cuts = {}
    for q in noul_qs:
        cuts[q] = pctl_threshold([noul(by[p][q]) for p in by], OUTLIER_PCTL)
    for q in risk_qs:
        cuts[q] = pctl_threshold([score(by[p][q]) for p in by], 1 - OUTLIER_PCTL)
    cuts["test_adequacy"] = pctl_threshold([score(by[p]["test_adequacy"]) for p in by],
                                           OUTLIER_PCTL)

    flags = []
    for p, a in by.items():
        m = meta[p]
        row = {"pass": p, "file": m["file"], "always_on": m["always_on"],
               "tier": m["tier"], "loc": m["loc"], "reasons": [], "weight": 0.0}

        for q, label in noul_qs.items():
            v = noul(a[q])
            if v <= cuts[q]:
                row["reasons"].append(("CONCERN", f"{label} (p={v:.2f}, corpus p20={cuts[q]:.2f})"))
                row["weight"] += (cuts[q] - v) + 0.5

        for q, label in risk_qs.items():
            v = score(a[q])
            if v >= cuts[q]:
                sev = "ALARM" if v >= ABS_RISK_FLOOR else "CONCERN"
                row["reasons"].append(
                    (sev, f"{label} ({v:.2f}/3, corpus p80={cuts[q]:.2f})"))
                row["weight"] += (v - cuts[q]) + (1.0 if sev == "ALARM" else 0.5)

        if score(a["test_adequacy"]) <= cuts["test_adequacy"]:
            v = score(a["test_adequacy"])
            row["reasons"].append(
                ("CONCERN", f"thinnest test coverage in the corpus ({v:.2f}/3, p20={cuts['test_adequacy']:.2f})"))
            row["weight"] += 0.5

        # Categorical facts, not relative positions: these stand on their own --
        # but only when the model actually settled on them.  Raising an ALARM off
        # a 0.24-confidence choice would contradict the low-confidence floor this
        # same report applies everywhere else.
        if a["evidence_strength"]["choice"] == "asserted":
            c = conf(a["evidence_strength"])
            if c >= LOW_CONFIDENCE:
                row["reasons"].append(
                    ("ALARM", f"rewrites IR with equivalence merely asserted — no argument, no proof artifact (conf {c:.2f})"))
                row["weight"] += 1.5
            else:
                row["reasons"].append(
                    ("NOTE", f"possibly asserted-without-argument, but the model did not settle (conf {c:.2f}) — treat as unresolved"))

        # Running unconditionally amplifies everything: a defect actually ships.
        if any(s != "NOTE" for s, _ in row["reasons"]) and m["always_on"]:
            row["weight"] += 1.0
            row["reasons"].append(("ALARM", "the above ships on every compile: this pass is NOT behind an off-by-default flag"))

        if row["reasons"]:
            # The axis that matters is "does this ship", not "does it have a
            # flag".  A flag-gated pass with Init(1) -- rvtt_dce, rvtt_cc --
            # ships exactly as surely as one with no flag at all.
            row["population"] = "ships" if m["always_on"] else "opt_in"
            flags.append(row)

    flags.sort(key=lambda r: (-r["weight"], r["pass"]))

    # The risk questions are calibrated for OPTIONAL transforms that have a
    # precondition to check before acting.  Mandatory lowering has no such
    # precondition -- "mutates before validating" is tautological for a pass
    # whose entire job is to rewrite every tree it sees.  Ranking the two
    # populations against each other would bury the opt-in optimizations,
    # which are where these questions actually bite, so they are separated.
    flags_gated = [f for f in flags if f["population"] == "opt_in"][:MAX_FLAGGED]
    flags_uncond = [f for f in flags if f["population"] == "ships"][:MAX_FLAGGED]

    # Performance opportunity = upside x breadth, discounted by how much
    # evidence already exists (already-measured work is not an opportunity).
    opp = []
    for p, a in by.items():
        up, br = score(a["perf_upside"]), score(a["applicability_breadth"])
        ev = a["measurement_evidence"]["choice"]
        discount = {"silicon_ab": 0.25, "simulator": 0.6, "structural": 0.9, "none": 1.0}[ev]
        opp.append({
            "pass": p, "upside": up, "breadth": br,
            "mechanism": a["perf_mechanism"]["choice"],
            "mechanism_conf": conf(a["perf_mechanism"]),
            "evidence": ev, "evidence_conf": conf(a["measurement_evidence"]),
            "readiness": score(a["default_on_readiness"]),
            "always_on": meta[p]["always_on"],
            "opportunity": round(up * br * discount, 2),
        })
    opp.sort(key=lambda d: -d["opportunity"])

    prio = sorted(by, key=lambda p: -score(by[p]["review_priority"]))

    unresolved = []
    for p, a in by.items():
        for q, ans in a.items():
            c = conf(ans)
            if c is not None and c < LOW_CONFIDENCE:
                unresolved.append({"pass": p, "question": q, "confidence": c,
                                   "answer": ans.get("choice", ans.get("score"))})
    unresolved.sort(key=lambda d: d["confidence"])

    return {"flags": flags, "flags_gated": flags_gated, "flags_uncond": flags_uncond,
            "opportunity": opp, "priority": prio,
            "unresolved": unresolved, "by": by, "meta": meta}


def render(doc: dict, an: dict) -> str:
    prov = doc["provenance"]
    by, meta = an["by"], an["meta"]
    L: list[str] = []
    w = L.append

    w("# rvtt pass audit — soundness, correctness, performance potential")
    w("")
    w(f"- **Tree**: `sfpi-gcc` @ `{prov['gcc_head'][:12]}`")
    w(f"- **Passes scored**: {prov['scored']} of {prov['registered_passes']} registered "
      f"(`pass_dce` is generic GCC DCE, not an rvtt pass)")
    w(f"- **Model**: `{prov['model_served']}` — {prov['question_count']} independent judgments per pass, "
      f"one batched request each")
    w(f"- **Corpus context**: {prov['refusal_registry_size']} named refusals, "
      f"{len(prov['proof_artifacts'])} proof artifacts, {prov['tt_options']} `-mtt-tensix-*` options")
    w("")
    w("> **What this is.** A calibrated triage ranking, not a verifier. System One returns "
      "probabilities over a rubric; it does not prove soundness. Soundness here is established by "
      "`tt/proofs/`, the testsuite, and silicon A/B. Read every number below as *where to spend "
      "review time*, and confirm each flag against the source before acting on it.")
    w("")

    # ---- flags
    w("## 1. Review queue")
    w("")
    w("Flags are **relative to this corpus**, not absolute verdicts. A pass appears because it "
      "sits in the worst fifth of the 54 on some dimension. Percentile cuts are quoted inline so "
      "you can see how far from its peers it actually is.")
    w("")
    w("**Read the two queues differently.** The risk questions assume an *optional transform with "
      "a precondition to check before acting*. That frame fits the flag-gated optimizations. It "
      "does not fit mandatory lowering: `expand`'s job is to rewrite every condition tree it sees, "
      "so \"mutates before validating\" is tautological for it, not a defect. Hand-checking "
      "confirmed this — `expand` carries zero named refusals and three asserts by design. The two "
      "populations are therefore ranked separately, and 1b should be read as *where the backend "
      "carries unevidenced risk by construction*, not as a defect list.")
    w("")

    for title, rows, note in [
        ("### 1a. Opt-in optimizations (off by default) — the risk questions apply directly",
         an["flags_gated"],
         "These opt in to a transform under a precondition, so a flag here means the precondition "
         "handling looks weaker than its peers. **This is the queue to work first**: these are "
         "the passes whose promotion is still a live decision."),
        ("### 1b. Passes that ship today — unconditional, or flag-gated with `Init(1)`",
         an["flags_uncond"],
         "Mandatory lowering, diagnosis, and enforcement, plus the few flags that default on "
         "(`dce`, `cc`). Discount `mutation_before_validation` and `ordering_fragility` here — "
         "they restate the pass's job. The transferable signals are **missing targeted tests** "
         "and **an absent named-refusal surface**."),
    ]:
        alarms = [f for f in rows if any(s == "ALARM" for s, _ in f["reasons"])]
        w(f"{title} — {len(rows)} flagged, {len(alarms)} with an ALARM")
        w("")
        w(note)
        w("")
        for f in rows:
            on = "**RUNS UNCONDITIONALLY**" if f["always_on"] else "flag-gated, default-off"
            w(f"#### `{fmt_pass(f['pass'])}` — {on}, {f['loc']} loc, {f['tier']}")
            w(f"`{f['file']}`")
            w("")
            for sev, why in f["reasons"]:
                w(f"- **{sev}** — {why}")
            w("")

    # ---- perf
    w("## 2. Performance opportunity")
    w("")
    w("`opportunity = upside x breadth x evidence_discount` — a pass already backed by silicon A/B "
      "is not an opportunity, it is done. Both factors are 0–3.")
    w("")
    w("| pass | upside | breadth | mechanism | evidence | default | opportunity |")
    w("|---|---|---|---|---|---|---|")
    for o in an["opportunity"][:20]:
        ec = f" ({o['evidence_conf']:.2f})" if o["evidence_conf"] is not None else ""
        w(f"| `{fmt_pass(o['pass'])}` | {o['upside']:.2f} | {o['breadth']:.2f} | "
          f"{o['mechanism']} | {o['evidence']}{ec} | {'UNCOND' if o['always_on'] else 'off'} | "
          f"**{o['opportunity']:.2f}** |")
    w("")

    # ---- readiness
    w("## 3. Evidence against the Rescue Contract")
    w("")
    w("`default_on_readiness` scores a pass against the Evidence-Based Rescue Contract: "
      "`0` = envelope unsettled, `3` = proven, differentialed, and measured. It means two "
      "different things depending on how the pass is gated, so the two populations are split.")
    w("")

    gated = [p for p in by if meta[p]["gating_mode"] == "flag_gated"]
    uncond = [p for p in by if meta[p]["gating_mode"] == "unconditional"]

    w("### 3a. Promotion candidates (flag-gated)")
    w("")
    w("For these, readiness is the actual promotion question: should the flag flip?")
    w("")
    w("| pass | readiness | evidence | equivalence argument |")
    w("|---|---|---|---|")
    for p in sorted(gated, key=lambda p: -score(by[p]["default_on_readiness"]))[:12]:
        a = by[p]
        w(f"| `{fmt_pass(p)}` | {score(a['default_on_readiness']):.2f} | "
          f"{a['measurement_evidence']['choice']} | {a['evidence_strength']['choice']} |")
    w("")

    w("### 3b. Already shipping, evidence short of the contract (unconditional)")
    w("")
    w("These are not promotion candidates — they are mandatory lowering and enforcement that "
      "already run on **every** Tensix compilation. For them a low readiness score is the "
      "*inverse* observation: the contract's evidence bar is not met, and the code ships anyway. "
      "That is expected for mandatory lowering, which cannot be gated behind evidence. It is "
      "listed because it locates where the backend carries unevidenced risk by construction.")
    w("")
    w("| pass | readiness | evidence | equivalence argument | targeted tests |")
    w("|---|---|---|---|---|")
    for p in sorted(uncond, key=lambda p: score(by[p]["default_on_readiness"]))[:12]:
        a = by[p]
        w(f"| `{fmt_pass(p)}` | {score(a['default_on_readiness']):.2f} | "
          f"{a['measurement_evidence']['choice']} | {a['evidence_strength']['choice']} | "
          f"{score(a['test_adequacy']):.2f}/3 |")
    w("")

    # ---- review order
    w("## 4. Suggested review order")
    w("")
    w("| # | pass | priority | default | loc |")
    w("|---|---|---|---|---|")
    for i, p in enumerate(an["priority"][:15], 1):
        w(f"| {i} | `{fmt_pass(p)}` | {score(by[p]['review_priority']):.2f} | "
          f"{'UNCOND' if meta[p]['always_on'] else 'off'} | {meta[p]['loc']} |")
    w("")

    # ---- evidence census
    w("## 5. Evidence census")
    w("")
    w("Counted two ways. *Settled* counts only answers the model actually committed to "
      f"(confidence >= {LOW_CONFIDENCE:.2f}); *unsettled* answers are shown separately rather than "
      "folded into a headline number. A raw count alone would report a 0.18-confidence coin flip "
      "as a fact.")
    w("")
    for q, label in [("evidence_strength", "Strongest equivalence evidence"),
                     ("measurement_evidence", "Performance evidence"),
                     ("perf_mechanism", "Primary performance mechanism")]:
        settled: dict[str, int] = {}
        unsettled = 0
        for p in by:
            a = by[p][q]
            if conf(a) is not None and conf(a) < LOW_CONFIDENCE:
                unsettled += 1
                continue
            settled[a["choice"]] = settled.get(a["choice"], 0) + 1
        w(f"**{label}** — {sum(settled.values())} settled, {unsettled} unsettled")
        w("")
        for k, v in sorted(settled.items(), key=lambda kv: -kv[1]):
            w(f"- `{k}`: {v}")
        w("")

    # ---- honesty section
    w("## 6. Low-confidence answers (do not treat as findings)")
    w("")
    w(f"{len(an['unresolved'])} of {prov['scored'] * prov['question_count']} answers came back below "
      f"{LOW_CONFIDENCE:.2f} confidence. The model is reporting that the state did not settle the "
      "question — usually because the evidence genuinely is not in the source.")
    w("")
    w("| pass | question | answer | conf |")
    w("|---|---|---|---|")
    for u in an["unresolved"][:25]:
        w(f"| `{fmt_pass(u['pass'])}` | {u['question']} | {u['answer']} | {u['confidence']:.2f} |")
    w("")

    w("---")
    w("")
    w("## Reproducing")
    w("")
    w("```sh")
    w("source <your-secrets-file>   # TYPESAFE_API_KEY")
    w("cd scripts/rvtt-pass-audit")
    w("./.venv/bin/python extract_cards.py        # tree -> results/cards.json")
    w("./.venv/bin/python score_passes.py         # cards -> results/scores.json")
    w("./.venv/bin/python report.py               # scores -> results/SCOREBOARD.md")
    w("```")
    w("")
    w(f"Scoring is cached on a hash of (state, questions, model), so a re-run after editing one "
      f"pass re-scores only that pass. Fresh run cost: "
      f"{prov['usage_totals_fresh']['input_tokens']:,} input / "
      f"{prov['usage_totals_fresh']['output_tokens']:,} output tokens.")
    return "\n".join(L)


def main() -> int:
    ap = argparse.ArgumentParser()
    ap.add_argument("--scores", default="results/scores.json")
    ap.add_argument("--out", default="results/SCOREBOARD.md")
    ap.add_argument("--json-out", default="results/analysis.json")
    args = ap.parse_args()

    doc = json.loads(Path(args.scores).read_text())
    an = build(doc["scores"])

    Path(args.out).write_text(render(doc, an))
    Path(args.json_out).write_text(json.dumps(
        {k: v for k, v in an.items() if k not in ("by", "meta")}, indent=2))

    print(f"opt-in flagged : {len(an['flags_gated'])} passes")
    print(f"shipping flagged: {len(an['flags_uncond'])} passes")
    print(f"low-conf answers: {len(an['unresolved'])}")
    print(f"wrote {args.out} and {args.json_out}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
