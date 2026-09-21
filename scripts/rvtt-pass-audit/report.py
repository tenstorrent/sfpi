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


def render(doc: dict, an: dict, meas: dict | None) -> str:
    prov = doc["provenance"]
    by, meta = an["by"], an["meta"]
    mpass = (meas or {}).get("per_pass", {})
    mknob = (meas or {}).get("per_knob", {})
    mprov = (meas or {}).get("provenance", {})
    L: list[str] = []
    w = L.append

    w("# rvtt pass audit — soundness, correctness, performance potential")
    w("")
    w(f"- **Tree**: `sfpi-gcc` @ `{prov['gcc_head'][:12]}`")
    w(f"- **Passes scored**: {prov['scored']} of {prov['registered_passes']} registered "
      f"(`pass_dce` is generic GCC DCE, not an rvtt pass)")
    w(f"- **Model**: `{prov['model_served']}` — {prov['question_count']} independent judgments "
      f"per pass, one batched request each")
    w(f"- **Corpus context**: {prov['refusal_registry_size']} named refusals, "
      f"{len(prov['proof_artifacts'])} proof artifacts, {prov['tt_options']} "
      f"`-mtt-tensix-*` options, {prov.get('testsuite_size', '?')} TT testcases")
    if mprov:
        w(f"- **Silicon**: `{mprov['knob_file']}` + `{mprov['composition_file']}` — "
          f"{mprov['knob_rows']} measured rows, {mprov['knobs_resolved']}/"
          f"{mprov['knobs_seen']} knobs attributed to {mprov['passes_with_measurement']} passes")
    w("")
    w("> **How to read this.** Section 1 is *measurement* — real QB2 silicon, and the only part "
      "here that is evidence. Sections 2 onward are *model judgment*: calibrated probabilities "
      "over a rubric, which prove nothing and exist to rank where review time goes. Where the "
      "two disagree, the silicon wins. The ranked risk queue is **held** — see section 5.")
    w("")

    # ------------------------------------------------------------------
    # 1. MEASURED
    # ------------------------------------------------------------------
    w("## 1. Measured silicon — the authoritative perf column")
    w("")
    if not mpass:
        w("_No measured data joined. Run `./measured.py` first._")
        w("")
    else:
        w(f"Sign convention is the board's: **negative is a win** (fewer cycles vs hand). "
          f"Values are {mprov['value_used']}. A knob is attributed to a pass by "
          f"`knob → -mtt-tensix-* option → option Var → file reading it → pass owning that file`; "
          f"every link is mechanical.")
        w("")
        w("### 1a. Per knob")
        w("")
        w("| knob | rows | median | best | wins | regressions | pass |")
        w("|---|---|---|---|---|---|---|")
        for k in sorted(mknob.values(), key=lambda d: (d["median_vs_hand"] is None,
                                                       d["median_vs_hand"])):
            passes = ", ".join(f"`{fmt_pass(p)}`" for p in k["passes"])
            w(f"| `{k['knob']}` | {k['n_rows']} | **{k['median_vs_hand']:+.2f}** | "
              f"{k['best_vs_hand']:+.2f} | {k['n_wins']} | {k['n_regressions']} | {passes} |")
        w("")
        w("**Two caveats on attribution.** The chain resolves *which pass reads the flag*, which "
          "is not always *which pass implements the win*: `reassoc-mad-restructure` resolves to "
          "`combine`, because its Var is consulted in `gimple-rvtt-combine.cc` where the "
          "`mul+add -> mad` rule actually fires, not in `gimple-rvtt-reassoc.cc` which does the "
          "rebalancing that feeds it. Likewise `crossloop-cc-peel` resolves to `prgm_const` via "
          "`gimple-rvtt-prgm-residency.cc`, not to `pass_rvtt_crossloop`. Separately, "
          "`crossrow-2datum` is the one knob with **no option of that name** — it is hand-aliased "
          "to `-mtt-tensix-optimize-crossrow-pairing` in `measured.py`, so its 5 rows rest on my "
          "inference, not on a mechanical match. Check that alias before acting on its −9.64.")
        w("")
        w("**Row count and yield pull in opposite directions.** The two most widely attributed "
          "knobs — `stochrnd-store-fold` (9 rows) and `delivery-shape` (6) — have the weakest "
          "medians in the set. The strongest medians sit on knobs with two to five rows: "
          "`loop-prgm-reclaim`, `crossloop-cc-peel`, `reassoc-mad-restructure`, `crossrow-2datum`. "
          "Breadth of firing is not the same as size of win, and a median over two rows is a "
          "thin basis for a promotion decision either way.")
        w("")
        w("### 1b. Per pass")
        w("")
        w("| pass | knobs | rows | median | best | ships today |")
        w("|---|---|---|---|---|---|")
        for p in sorted(mpass.values(), key=lambda d: (d["median_vs_hand"] is None,
                                                       d["median_vs_hand"])):
            m = meta.get(p["pass"], {})
            w(f"| `{fmt_pass(p['pass'])}` | {', '.join(p['knobs'])} | {p['n_rows']} | "
              f"**{p['median_vs_hand']:+.2f}** | {p['best_vs_hand']:+.2f} | "
              f"{'yes' if m.get('always_on') else 'no'} |")
        w("")
        unmeasured = [p for p in by if p not in mpass]
        w(f"**{len(unmeasured)} of {prov['scored']} passes have no measured row at all.** "
          "For those, everything below is estimate, not evidence.")
        w("")

    # ------------------------------------------------------------------
    # 2. MODEL ESTIMATE, only where measurement is absent
    # ------------------------------------------------------------------
    w("## 2. Estimated opportunity where no measurement exists")
    w("")
    w("`upside x breadth`, both 0–3, from the model reading the source. This is a **prior for "
      "choosing what to measure next**, not a result. Passes with silicon rows are excluded — "
      "for those, section 1 is the answer.")
    w("")
    w("| pass | upside | breadth | mechanism | ships | est. opportunity |")
    w("|---|---|---|---|---|---|")
    shown = 0
    for o in an["opportunity"]:
        if o["pass"] in mpass or shown >= 15:
            continue
        shown += 1
        w(f"| `{fmt_pass(o['pass'])}` | {o['upside']:.2f} | {o['breadth']:.2f} | "
          f"{o['mechanism']} | {'yes' if o['always_on'] else 'no'} | **{o['opportunity']:.2f}** |")
    w("")

    # ------------------------------------------------------------------
    # 3. TEST COVERAGE — the verified structural finding
    # ------------------------------------------------------------------
    w("## 3. WITHDRAWN — the \"test-coverage inversion\"")
    w("")
    w("**An earlier version of this report claimed the always-on passes are the least tested, "
      "on the strength of `rvtt_expand`, `rvtt_live`, `rvtt_check` and `rvtt_synth_cse` having "
      "zero tests naming their dump. That finding is withdrawn. It was an artifact of the "
      "metric.**")
    w("")
    w("Counting tests by dump name measures one testing modality. It is not the one this suite "
      "mostly uses:")
    w("")
    w("| validation modality | testcases |")
    w("|---|---|")
    w("| `scan-assembler` against emitted instructions | 873 |")
    w("| `dg-error` diagnostics | 68 |")
    w("| `scan-tree-dump` / `scan-rtl-dump` naming a pass | 84 |")
    w("")
    w("A pass validated by assembly scanning scores zero on dump-name counting however well it "
      "is covered. 206 testcases scan the CC instruction sequences `rvtt_expand` emits, and 35 "
      "`dg-error` tests cover the spill diagnostic — none of which the metric could see. Three "
      "passes (`check_early`, `check_late`, `lreg_livein`) never touch `dump_file` at all, so the "
      "metric cannot reach them even in principle.")
    w("")
    w("**The instructive part is how this got through.** It was published as \"verified "
      "independently of the model\", and grep did verify it — the counts were accurate. What grep "
      "could not verify is that the counts measured what the sentence claimed. That is construct "
      "validity, not measurement error, and labelling it \"verified\" gave a bad metric more "
      "standing than any model output in this report has. A hand-check confirms a number; it does "
      "not confirm that the number means what you say it means.")
    w("")
    w("The extractor now records `emits_dump` per pass and splits targeted tests into `direct`, "
      "`by_refusal` and `by_diagnostic`, each carrying a caveat that none of them is a coverage "
      "measure on its own. Whether the always-on passes are in fact under-tested is **open**: "
      "answering it needs assembly-scan attribution, which nothing here does yet.")
    w("")

    # ------------------------------------------------------------------
    # 4. EVIDENCE CENSUS
    # ------------------------------------------------------------------
    w("## 4. Evidence census")
    w("")
    w("Counted two ways. *Settled* counts only answers the model committed to "
      f"(confidence >= {LOW_CONFIDENCE:.2f}); *unsettled* are shown separately rather than folded "
      "into a headline number, so a 0.18-confidence coin flip is not reported as a fact.")
    w("")
    for q, label in [("evidence_strength", "Strongest equivalence evidence"),
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
    w("The model's own `measurement_evidence` judgment is deliberately **not** reported here. "
      "Section 1 supersedes it: guessing from source comments whether a pass was ever measured "
      "is a proxy, and the board rows are the fact.")
    w("")

    # ------------------------------------------------------------------
    # 5. RISK QUEUE — HELD
    # ------------------------------------------------------------------
    w("## 5. Ranked risk queue — HELD, not published")
    w("")
    w("The soundness/correctness ranking is **withheld from this report on purpose.**")
    w("")
    w("Its risk questions assume an *optional transform with a precondition to check before "
      "acting*. That frame fits the opt-in optimizations and does not fit mandatory lowering: "
      "`rvtt_expand`'s job is to rewrite every condition tree it sees, so \"mutates IR before "
      "validation completes\" scores high for it as a restatement of its purpose, not as a "
      "defect. Hand-checking confirmed that reading — `expand` carries zero named refusals and "
      "three asserts, by design.")
    w("")
    w("That is the same class of error as two flags already caught and fixed during development "
      "(22 passes mislabeled default-off; `check_early/late` forced into an \"asserted "
      "equivalence\" bucket that had no outcome for deliberate error-recovery). Three instances "
      "of one failure mode is a reason to fix the instrument, not to ship its ranking.")
    w("")
    w("**Unblocking it needs a second question set written for lowering and enforcement passes** "
      "— one that asks about assert density, refusal-surface absence, and diagnostic reachability "
      "instead of precondition handling. The raw per-pass answers are in `results/scores.json` "
      "for anyone who wants them; they should not be read as a ranked defect list.")
    w("")

    # ------------------------------------------------------------------
    # 6. LIMITS
    # ------------------------------------------------------------------
    w("## 6. Limits of this run")
    w("")
    digested = [s for s in doc["scores"] if "digest" in (s.get("source_status") or "")]
    w(f"- **Source coverage.** A pass split across translation units behind a private `-int.h` "
      f"is sent with all its siblings. {len(digested)} of {prov['scored']} passes exceeded the "
      f"model's 32k-token state limit and had siblings reduced to a structural digest "
      f"(contract comment, signatures, and every IR-mutating call site with context). Those "
      f"passes' judgments saw less code; `source_status` in `scores.json` records which.")
    w(f"- **Unowned source.** ~25.5k lines of `tt/*.cc` belong to no single registered pass "
      f"(shared tables, cost models, generators) and are scored by nobody.")
    w(f"- **Run-to-run variance.** Scoring is not deterministic; the sub-0.50-confidence count "
      f"moved between 222 and 230 across repeated full runs. Do not read a 0.05 difference "
      f"between two passes as meaningful.")
    w(f"- **Unsettled answers.** {len(an['unresolved'])} of "
      f"{prov['scored'] * prov['question_count']} answers came back below "
      f"{LOW_CONFIDENCE:.2f} confidence and are excluded from every count above.")
    w("")

    w("---")
    w("")
    w("## Reproducing")
    w("")
    w("```sh")
    w("source <your-secrets-file>   # TYPESAFE_API_KEY")
    w("cd scripts/rvtt-pass-audit")
    w("./.venv/bin/python extract_cards.py       # tree    -> results/cards.json")
    w("./.venv/bin/python measured.py            # board   -> results/measured.json")
    w("./.venv/bin/python score_passes.py        # cards   -> results/scores.json")
    w("./.venv/bin/python report.py              # all     -> results/SCOREBOARD.md")
    w("```")
    w("")
    w(f"Fresh run cost: {prov['usage_totals_fresh']['input_tokens']:,} input / "
      f"{prov['usage_totals_fresh']['output_tokens']:,} output tokens across "
      f"{prov['scored']} requests. Scoring caches on a hash of (state, questions, model).")
    return "\n".join(L)


def main() -> int:
    ap = argparse.ArgumentParser()
    ap.add_argument("--scores", default="results/scores.json")
    ap.add_argument("--measured", default="results/measured.json")
    ap.add_argument("--out", default="results/SCOREBOARD.md")
    ap.add_argument("--json-out", default="results/analysis.json")
    args = ap.parse_args()

    doc = json.loads(Path(args.scores).read_text())
    an = build(doc["scores"])

    mp = Path(args.measured)
    meas = json.loads(mp.read_text()) if mp.exists() else None
    if meas is None:
        print(f"warning: {mp} not found — section 1 will be empty; run ./measured.py")

    Path(args.out).write_text(render(doc, an, meas))
    Path(args.json_out).write_text(json.dumps(
        {k: v for k, v in an.items() if k not in ("by", "meta")}, indent=2))

    print(f"measured passes : {len((meas or {}).get('per_pass', {}))}")
    print(f"risk queue      : HELD (computed, not rendered)")
    print(f"low-conf answers: {len(an['unresolved'])}")
    print(f"wrote {args.out} and {args.json_out}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
