#!/usr/bin/env python3
"""Build one deterministic "pass card" per registered rvtt pass.

The card is the state a System One judgment sees.  Everything here is
mechanical extraction from the tree -- no interpretation, no model.  If a
field cannot be established from the source it is recorded as null rather
than guessed, so a downstream judgment can distinguish "absent" from
"unknown".

Usage:
    ./extract_cards.py --gcc ../../gcc --out results/cards.json
"""

from __future__ import annotations

import argparse
import json
import os
import re
import subprocess
import sys
from pathlib import Path

# --------------------------------------------------------------------------
# rvtt-passes.def -- registration order, anchor, and the ordering rationale
# --------------------------------------------------------------------------

INSERT_RE = re.compile(
    r"^INSERT_PASS_(?P<dir>BEFORE|AFTER)\s*\(\s*(?P<anchor>\w+)\s*,\s*(?P<num>\d+)\s*,\s*(?P<pass>\w+)\s*\)"
)


def strip_comment(block: str) -> str:
    """Turn a run of /* ... */ and // lines into flowing prose."""
    block = re.sub(r"/\*|\*/", "", block)
    lines = []
    for line in block.splitlines():
        line = line.strip()
        line = re.sub(r"^\*\s?", "", line)
        line = re.sub(r"^//\s?", "", line)
        lines.append(line)
    text = "\n".join(lines).strip()
    # Collapse hard-wrapped prose but keep paragraph and indented-block breaks.
    return re.sub(r"\n{3,}", "\n\n", text)


def parse_passes_def(path: Path) -> list[dict]:
    """Return registered passes in file order with their preceding comment."""
    text = path.read_text()
    lines = text.splitlines()

    # Skip the GPL header: the first comment block that mentions "Copyright".
    passes: list[dict] = []
    pending: list[str] = []
    in_comment = False
    section = None

    for raw in lines:
        stripped = raw.strip()

        if stripped.startswith("// Gimple passes"):
            section = "gimple"
            pending = []
            continue
        if stripped.startswith("// RTL passes"):
            section = "rtl"
            pending = []
            continue

        m = INSERT_RE.match(stripped)
        if m:
            rationale = strip_comment("\n".join(pending)) if pending else None
            if rationale and "Copyright" in rationale:
                rationale = None
            passes.append(
                {
                    "pass": m.group("pass"),
                    "insert_direction": m.group("dir"),
                    "anchor": m.group("anchor"),
                    "anchor_instance": int(m.group("num")),
                    "tier": section,
                    "ordering_rationale": rationale,
                    "registration_index": len(passes),
                }
            )
            pending = []
            continue

        # Accumulate comment lines immediately preceding an INSERT line.
        if stripped.startswith("/*"):
            in_comment = True
            pending.append(raw)
            if "*/" in stripped:
                in_comment = False
            continue
        if in_comment:
            pending.append(raw)
            if "*/" in stripped:
                in_comment = False
            continue
        if stripped.startswith("//"):
            pending.append(raw)
            continue
        if not stripped:
            continue
        # Any other code line breaks the association.
        pending = []

    return passes


# --------------------------------------------------------------------------
# pass name -> implementing file
# --------------------------------------------------------------------------


def build_pass_file_index(tt_dir: Path) -> dict[str, Path]:
    """Map pass_rvtt_foo -> the .cc file defining make_pass_rvtt_foo."""
    index: dict[str, Path] = {}
    for cc in sorted(tt_dir.glob("*.cc")):
        text = cc.read_text(errors="replace")
        for m in re.finditer(r"^make_pass_(rvtt_\w+)\s*\(", text, re.M):
            index["pass_" + m.group(1)] = cc
    return index


def find_split_siblings(primary: Path, tt_dir: Path) -> list[Path]:
    """Files that are part of the same split pass (shared -int.h interface).

    Several passes were split into multiple translation units behind a
    private `-int.h` header (see the 2026-09 split commits).  The judgment
    should know those exist even when only the primary file is sent.
    """
    stem = primary.stem
    # rtl-rvtt-lp-alloc.cc -> rtl-rvtt-lp-alloc-int.h -> siblings
    int_h = tt_dir / f"{stem}-int.h"
    if not int_h.exists():
        return []
    siblings = []
    for cc in sorted(tt_dir.glob("*.cc")):
        if cc == primary:
            continue
        if f'"{int_h.name}"' in cc.read_text(errors="replace"):
            siblings.append(cc)
    return siblings


# --------------------------------------------------------------------------
# per-file extraction
# --------------------------------------------------------------------------


def leading_block_comment(text: str) -> str | None:
    """The contract comment: the first /* */ block after the GPL header."""
    blocks = re.findall(r"/\*.*?\*/", text, re.S)
    for b in blocks:
        if "Copyright" in b or "GNU General Public License" in b:
            continue
        return strip_comment(b)
    return None


def extract_gate(text: str, pass_name: str) -> str | None:
    """Body of the pass's gate() override, verbatim."""
    # Find the class, then the gate within it.
    cls = re.search(rf"class\s+{re.escape(pass_name)}\s*:.*?^\}};", text, re.S | re.M)
    scope = cls.group(0) if cls else text
    m = re.search(r"bool\s+gate\s*\([^)]*\)[^{]*\{", scope)
    if not m:
        return None
    start = m.end() - 1
    depth = 0
    for i in range(start, len(scope)):
        if scope[i] == "{":
            depth += 1
        elif scope[i] == "}":
            depth -= 1
            if depth == 0:
                return scope[start + 1 : i].strip()
    return None


def extract_dump_name(text: str, pass_name: str) -> str | None:
    data = re.search(
        rf"pass_data_{re.escape(pass_name[len('pass_'):])}\s*=\s*\{{(.*?)\}};", text, re.S
    )
    if not data:
        return None
    strings = re.findall(r'"([^"]+)"', data.group(1))
    return strings[0] if strings else None


def extract_pass_kind(text: str, pass_name: str) -> str | None:
    data = re.search(
        rf"pass_data_{re.escape(pass_name[len('pass_'):])}\s*=\s*\{{(.*?)\}};", text, re.S
    )
    if not data:
        return None
    m = re.search(r"\b(GIMPLE_PASS|RTL_PASS|SIMPLE_IPA_PASS|IPA_PASS)\b", data.group(1))
    return m.group(1) if m else None


def extract_flags(text: str, opts: dict[str, dict]) -> list[dict]:
    """Option flags this file reads, via their riscv_tt_* Var names."""
    found = []
    for var, meta in opts.items():
        if re.search(rf"\b{re.escape(var)}\b", text):
            found.append(meta)
    return sorted(found, key=lambda d: d["option"])


def parse_options(opt_path: Path) -> dict[str, dict]:
    """riscv.opt -> {var_name: {option, var, init, help}} for mtt-tensix-*."""
    text = opt_path.read_text(errors="replace")
    opts: dict[str, dict] = {}
    blocks = text.split("\n\n")
    for b in blocks:
        lines = [l for l in b.splitlines() if l.strip()]
        if not lines or not lines[0].startswith("mtt-tensix"):
            continue
        option = lines[0].strip()
        props = lines[1] if len(lines) > 1 else ""
        help_text = " ".join(lines[2:]).strip() or None
        var_m = re.search(r"Var\((\w+)\)", props)
        init_m = re.search(r"Init\((-?\d+)\)", props)
        if not var_m:
            continue
        opts[var_m.group(1)] = {
            "option": option,
            "var": var_m.group(1),
            "init": int(init_m.group(1)) if init_m else None,
            "undocumented": "Undocumented" in props,
            "help": help_text,
        }
    return opts


def extract_refusals(text: str, known: set[str]) -> list[str]:
    """Named refusals this file cites (matched against the frozen registry)."""
    cited = set()
    for name in known:
        if f'"{name}"' in text:
            cited.add(name)
    return sorted(cited)


def parse_refusal_registry(def_path: Path) -> list[dict]:
    rows = []
    for m in re.finditer(
        r'RVTT_REFUSAL\s*\(\s*(\w+)\s*,\s*"([^"]+)"\s*,\s*"((?:[^"\\]|\\.)*)"',
        def_path.read_text(errors="replace"),
    ):
        rows.append({"enum": m.group(1), "name": m.group(2), "note": m.group(3)})
    return rows


def extract_proof_citations(text: str, known_proofs: set[str]) -> list[str]:
    cited = set()
    for p in known_proofs:
        if p in text:
            cited.add(p)
    return sorted(cited)


def count_ir_mutations(text: str) -> dict[str, int]:
    """Rough census of IR-mutating API calls -- the risk surface."""
    patterns = {
        "gsi_replace": r"\bgsi_replace\s*\(",
        "gsi_remove": r"\bgsi_remove\s*\(",
        "gsi_insert": r"\bgsi_insert_\w+\s*\(",
        "emit_insn": r"\bemit_insn\w*\s*\(",
        "delete_insn": r"\bdelete_insn\w*\s*\(",
        "validate_change": r"\bvalidate_change\w*\s*\(",
        "df_insn_rescan": r"\bdf_insn_rescan\s*\(",
        "update_stmt": r"\bupdate_stmt\s*\(",
        "set_ssa_def": r"\bSSA_NAME_DEF_STMT\s*\(",
    }
    return {k: len(re.findall(v, text)) for k, v in patterns.items() if re.search(v, text)}


def classify_gating(gate: str | None, options: list[dict]) -> dict:
    """How is this pass turned on?

    A pass whose gate is just the target test runs on EVERY Tensix
    compilation.  Treating "no -mtt-tensix-* flag in the file" as
    "default off" gets the most always-on passes in the backend exactly
    backwards, so the distinction is explicit here.
    """
    if gate is None:
        return {"mode": "unknown", "always_on": None, "gating_flag": None}

    g = " ".join(gate.split())
    # Flags actually consulted by the gate, not merely present in the file.
    gating = [o for o in options if re.search(rf"\b{re.escape(o['var'])}\b", g)]

    if not gating:
        return {
            "mode": "unconditional",
            "always_on": True,
            "gating_flag": None,
            "note": "gate tests only the target/arch: this pass runs on every Tensix compilation",
        }

    # Init(-1) means "decided by a later heuristic", not off.
    inits = {o["option"]: o["init"] for o in gating}
    on = any(v not in (0, None) for v in inits.values())
    return {
        "mode": "flag_gated",
        "always_on": on,
        "gating_flag": sorted(inits),
        "inits": inits,
        "note": (
            "Init(-1) means the default is resolved later, not disabled"
            if any(v == -1 for v in inits.values())
            else None
        ),
    }


def find_tests(
    testsuite: Path,
    dump_name: str | None,
    options: list[dict],
    refusals_cited: list[str],
    gating: dict,
    total_tests: int,
) -> dict:
    """Testcases that exercise this pass.

    Three signals, strongest first:
      direct   -- the test names this pass's dump or one of its flags
      refusal  -- the test scans a dump for a refusal name only this pass
                  emits (the registry is frozen and dump-stable, so this
                  precisely identifies tests of this pass's decisions)
      implicit -- an unconditional pass is exercised by the whole suite
    """
    empty = {"direct": [], "by_refusal": [], "implicit_whole_suite": 0, "total": 0}
    if not testsuite.exists():
        return empty

    def grep_l(needles: list[str]) -> list[str]:
        if not needles:
            return []
        args = ["grep", "-rl", "--include=*.C"]
        for n in needles:
            args += ["-e", n]
        args.append(str(testsuite))
        try:
            out = subprocess.run(args, capture_output=True, text=True, timeout=180).stdout
        except (subprocess.TimeoutExpired, OSError):
            return []
        return sorted({Path(p).name for p in out.split() if p})

    direct = grep_l(([dump_name] if dump_name else []) + [o["option"] for o in options])
    by_refusal = [t for t in grep_l(refusals_cited) if t not in set(direct)]
    implicit = total_tests if gating.get("always_on") and gating["mode"] == "unconditional" else 0

    return {
        "direct": direct,
        "by_refusal": by_refusal,
        "implicit_whole_suite": implicit,
        "total": len(direct) + len(by_refusal),
    }


# --------------------------------------------------------------------------


def main() -> int:
    ap = argparse.ArgumentParser()
    ap.add_argument("--gcc", default=str(Path(__file__).resolve().parents[2] / "gcc"),
                    help="path to the sfpi-gcc checkout")
    ap.add_argument("--out", default="results/cards.json")
    args = ap.parse_args()

    gcc = Path(args.gcc).resolve()
    tt = gcc / "gcc" / "config" / "riscv" / "tt"
    if not tt.exists():
        print(f"error: {tt} does not exist", file=sys.stderr)
        return 1

    passes_def = tt / "rvtt-passes.def"
    opts = parse_options(gcc / "gcc" / "config" / "riscv" / "riscv.opt")
    refusals = parse_refusal_registry(tt / "rvtt-refusals.def")
    refusal_names = {r["name"] for r in refusals}
    proofs_dir = tt / "proofs"
    known_proofs = (
        {p.name for p in proofs_dir.iterdir() if p.is_dir()} if proofs_dir.exists() else set()
    )
    testsuite = gcc / "gcc" / "testsuite" / "g++.target" / "riscv" / "tt"
    total_tests = len(list(testsuite.rglob("*.C"))) if testsuite.exists() else 0

    registered = parse_passes_def(passes_def)
    file_index = build_pass_file_index(tt)

    head = subprocess.run(
        ["git", "-C", str(gcc), "rev-parse", "HEAD"], capture_output=True, text=True
    ).stdout.strip()

    cards = []
    for entry in registered:
        name = entry["pass"]
        src = file_index.get(name)
        card = dict(entry)

        if src is None:
            card.update(
                {
                    "file": None,
                    "resolution": "unresolved: no make_pass_%s definition found" % name[5:],
                }
            )
            cards.append(card)
            continue

        text = src.read_text(errors="replace")
        siblings = find_split_siblings(src, tt)
        options = extract_flags(text, opts)
        dump_name = extract_dump_name(text, name)
        gate = extract_gate(text, name)
        gating = classify_gating(gate, options)
        cited_refusals = extract_refusals(text, refusal_names)

        card.update(
            {
                "file": str(src.relative_to(gcc)),
                "loc": text.count("\n") + 1,
                "pass_kind": extract_pass_kind(text, name),
                "dump_name": dump_name,
                "split_siblings": [
                    {"file": str(s.relative_to(gcc)), "loc": s.read_text(errors="replace").count("\n") + 1}
                    for s in siblings
                ],
                "options": options,
                "gating": gating,
                "always_on": gating.get("always_on"),
                "gate": gate,
                "contract_comment": leading_block_comment(text),
                "refusals_cited": cited_refusals,
                "proofs_cited": extract_proof_citations(text, known_proofs),
                "ir_mutation_census": count_ir_mutations(text),
                "tests": find_tests(
                    testsuite, dump_name, options, cited_refusals, gating, total_tests
                ),
                "resolution": "ok",
            }
        )
        cards.append(card)

    out = Path(args.out)
    out.parent.mkdir(parents=True, exist_ok=True)
    out.write_text(
        json.dumps(
            {
                "provenance": {
                    "gcc_path": str(gcc),
                    "gcc_head": head,
                    "passes_def": str(passes_def.relative_to(gcc)),
                    "registered_passes": len(registered),
                    "resolved_passes": sum(1 for c in cards if c["resolution"] == "ok"),
                    "refusal_registry_size": len(refusals),
                    "proof_artifacts": sorted(known_proofs),
                    "tt_options": len(opts),
                    "testsuite_size": total_tests,
                    "unconditional_passes": sum(
                        1 for c in cards if c.get("gating", {}).get("mode") == "unconditional"
                    ),
                },
                "cards": cards,
            },
            indent=2,
        )
    )

    unresolved = [c["pass"] for c in cards if c["resolution"] != "ok"]
    print(f"registered passes : {len(registered)}")
    print(f"resolved to a file: {len(registered) - len(unresolved)}")
    if unresolved:
        print(f"unresolved        : {', '.join(unresolved)}")
    print(f"refusal registry  : {len(refusals)} names")
    print(f"proof artifacts   : {len(known_proofs)}")
    print(f"wrote {out}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
