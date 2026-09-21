#!/usr/bin/env python3
"""Join measured QB2 silicon results onto the pass cards.

The audit's `measurement_evidence` judgment is the model guessing, from a
pass's own source and comments, whether anyone ever measured it.  That is a
proxy.  This module replaces it with ground truth wherever ground truth
exists: the board's per-knob silicon rows, attributed to the pass that
implements the knob.

Chain:  knob name -> -mtt-tensix-* option -> option Var -> file(s) that read
        that Var (primary AND split siblings) -> the pass owning that file.

Every link is mechanical and every failure is reported rather than guessed.
Sign convention is the board's: NEGATIVE is a win (fewer cycles vs hand).

    ./measured.py --board ~/workspace/craq-sfpi/board --out results/measured.json
"""

from __future__ import annotations

import argparse
import csv
import json
import re
import statistics as st
from collections import defaultdict
from pathlib import Path

# Knobs whose board name is not a substring of any option name.  Kept
# explicit and tiny: an unmapped knob is reported, never guessed into a
# neighbouring option.
KNOB_ALIASES = {
    # The 2-datum crossrow shape is a mode of the crossrow pairing engine;
    # the board names the shape, riscv.opt names the engine.
    "crossrow-2datum": "mtt-tensix-optimize-crossrow-pairing",
}


def parse_options(opt_path: Path) -> dict[str, str]:
    """option name -> Var name, for every mtt-tensix-* option."""
    out = {}
    for m in re.finditer(
        r"^(mtt-tensix[\w=-]*)\n(.*Var\((\w+)\).*)$", opt_path.read_text(), re.M
    ):
        out[m.group(1)] = m.group(3)
    return out


def knob_to_option(knob: str, options: dict[str, str]) -> str | None:
    if knob in KNOB_ALIASES:
        return KNOB_ALIASES[knob]
    hits = [o for o in options if knob in o]
    if not hits:
        return None
    # Prefer the plain boolean over a `=`-joined tuning parameter, then the
    # shortest name, so "delivery-shape" picks the optimize flag rather than
    # "-min-benefit=".
    hits.sort(key=lambda o: (o.endswith("="), len(o)))
    return hits[0]


def var_to_files(var: str, tt_dir: Path) -> list[str]:
    hits = []
    for cc in sorted(tt_dir.glob("*.cc")):
        if re.search(rf"\b{re.escape(var)}\b", cc.read_text(errors="replace")):
            hits.append(cc.name)
    return hits


def build_file_owner(cards: list[dict]) -> dict[str, list[str]]:
    """file basename -> passes implemented in or split out of it."""
    owner: dict[str, list[str]] = defaultdict(list)
    for c in cards:
        if not c.get("file"):
            continue
        owner[Path(c["file"]).name].append(c["pass"])
        for s in c.get("split_siblings", []) or []:
            owner[Path(s["file"]).name].append(c["pass"])
    return owner


def load_board(board: Path) -> tuple[list[dict], dict[str, dict], str, str]:
    knob_f = sorted(board.glob("KNOB-SILICON-*.tsv"))[-1]
    comp_f = sorted(board.glob("COMPOSITION-SILICON-*.tsv"))[-1]

    def rows(p):
        return list(csv.DictReader(
            (l for l in p.open() if not l.startswith("#")), delimiter="\t"))

    knob_rows = rows(knob_f)
    comp = {r["op"]: r for r in rows(comp_f)}
    return knob_rows, comp, knob_f.name, comp_f.name


def fnum(x):
    try:
        return float(x)
    except (TypeError, ValueError):
        return None


def main() -> int:
    ap = argparse.ArgumentParser()
    ap.add_argument("--board", default=str(Path.home() / "workspace/craq-sfpi/board"))
    ap.add_argument("--cards", default="results/cards.json")
    ap.add_argument("--out", default="results/measured.json")
    args = ap.parse_args()

    doc = json.loads(Path(args.cards).read_text())
    gcc = Path(doc["provenance"]["gcc_path"])
    tt = gcc / "gcc" / "config" / "riscv" / "tt"
    cards = [c for c in doc["cards"] if c.get("resolution") == "ok"]

    options = parse_options(gcc / "gcc" / "config" / "riscv" / "riscv.opt")
    owner = build_file_owner(cards)
    knob_rows, comp, knob_file, comp_file = load_board(Path(args.board).expanduser())

    # knob -> measured rows
    per_knob: dict[str, list[dict]] = defaultdict(list)
    for r in knob_rows:
        k = r.get("best_knob")
        if not k:
            continue
        c = comp.get(r["op"], {})
        per_knob[k].append({
            "op": r["op"],
            "knob_vs_hand": fnum(r.get("knob_vs_hand")),
            "composition_best": fnum(c.get("best")),
            "best_shape": c.get("best_shape"),
            "board_class": r.get("board_class"),
            "classification": r.get("classification"),
            "confirms": r.get("confirms"),
        })

    resolved, unmapped = {}, []
    for knob, rows_ in sorted(per_knob.items()):
        opt = knob_to_option(knob, options)
        if not opt:
            unmapped.append({"knob": knob, "rows": len(rows_), "reason": "no matching option"})
            continue
        var = options[opt]
        files = var_to_files(var, tt)
        passes = sorted({p for f in files for p in owner.get(f, [])})
        if not passes:
            unmapped.append({"knob": knob, "rows": len(rows_), "option": opt,
                             "var": var, "files": files,
                             "reason": "no registered pass owns any file reading this Var"})
            continue

        # Prefer the composition best (the better of single-knob and
        # composition shapes, which is what the board reports as achieved);
        # fall back to the single-knob number when composition is blank.
        vals = [r["composition_best"] if r["composition_best"] is not None
                else r["knob_vs_hand"] for r in rows_]
        vals = [v for v in vals if v is not None]
        resolved[knob] = {
            "knob": knob, "option": opt, "var": var,
            "files": files, "passes": passes,
            "n_rows": len(rows_),
            "median_vs_hand": round(st.median(vals), 2) if vals else None,
            "best_vs_hand": round(min(vals), 2) if vals else None,
            "worst_vs_hand": round(max(vals), 2) if vals else None,
            "n_wins": sum(1 for v in vals if v < 0),
            "n_regressions": sum(1 for v in vals if v > 0),
            "rows": rows_,
        }

    # pass -> aggregate over every knob attributed to it
    per_pass: dict[str, dict] = {}
    for knob, info in resolved.items():
        for p in info["passes"]:
            e = per_pass.setdefault(p, {"pass": p, "knobs": [], "n_rows": 0,
                                        "vals": [], "n_wins": 0, "n_regressions": 0})
            e["knobs"].append(knob)
            e["n_rows"] += info["n_rows"]
            e["n_wins"] += info["n_wins"]
            e["n_regressions"] += info["n_regressions"]
            e["vals"] += [r["composition_best"] if r["composition_best"] is not None
                          else r["knob_vs_hand"] for r in info["rows"]]
    for e in per_pass.values():
        vals = [v for v in e["vals"] if v is not None]
        e["median_vs_hand"] = round(st.median(vals), 2) if vals else None
        e["best_vs_hand"] = round(min(vals), 2) if vals else None
        e.pop("vals")

    out = {
        "provenance": {
            "board": str(Path(args.board).expanduser()),
            "knob_file": knob_file,
            "composition_file": comp_file,
            "knob_rows": len(knob_rows),
            "knobs_seen": len(per_knob),
            "knobs_resolved": len(resolved),
            "knobs_unmapped": len(unmapped),
            "passes_with_measurement": len(per_pass),
            "sign_convention": "negative is a win (fewer cycles vs hand)",
            "value_used": "composition best where present, else single-knob knob_vs_hand",
        },
        "unmapped": unmapped,
        "per_knob": resolved,
        "per_pass": per_pass,
    }
    Path(args.out).parent.mkdir(parents=True, exist_ok=True)
    Path(args.out).write_text(json.dumps(out, indent=2))

    print(f"knob rows        : {len(knob_rows)}")
    print(f"knobs seen       : {len(per_knob)}")
    print(f"knobs resolved   : {len(resolved)}")
    print(f"passes measured  : {len(per_pass)}")
    if unmapped:
        print("unmapped knobs   :")
        for u in unmapped:
            print(f"  {u['knob']:28} ({u['rows']} rows) — {u['reason']}")
    print(f"wrote {args.out}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
