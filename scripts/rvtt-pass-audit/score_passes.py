#!/usr/bin/env python3
"""Score every rvtt pass card with one batched System One request per pass.

All 16 questions are independent over the same state, so they ride in a
single request per pass -- the fan-out pattern.  Results are cached by a
hash of (state, questions, model) so a re-run after editing one pass only
re-scores that pass.

    export TYPESAFE_API_KEY=...            # or: source <your-secrets-file>
    ./score_passes.py --cards results/cards.json --out results/scores.json

    ./score_passes.py --only pass_rvtt_ccmask --no-cache     # single pass
    ./score_passes.py --dry-run                              # cost estimate, no calls
"""

from __future__ import annotations

import argparse
import concurrent.futures as cf
import hashlib
import json
import os
import re
import sys
import time
from pathlib import Path

from typesafe_sdk import TypeSafeClient
from typesafe_sdk import constants as ts_constants

from questions import ALL_QUESTIONS, GROUPS, build_state

# jev-1.13.0 allows 64k tokens per request and, the binding one, 32k for
# `state` plus the longest question.  Largest request that actually landed
# was 34,901 total input tokens, so the source has to fit in roughly 95 KB
# once the card metadata and sixteen questions are accounted for.
MAX_SOURCE_CHARS = 82_000


def digest_file(text: str, name: str) -> str:
    """A structural digest of a sibling too large to send whole.

    Keeps what the judgments actually read -- the contract comment, the
    shape of the interface, and every IR-mutating call site with a little
    context -- and drops the bodies in between.  Blind truncation would be
    worse: the risky code is as likely to sit at the end of a 2700-line
    file as at the start.
    """
    lines = text.splitlines()
    keep: set[int] = set()

    # Leading contract comment: everything up to the first #include.
    for i, l in enumerate(lines[:400]):
        if l.startswith("#include"):
            break
        keep.add(i)

    sig = re.compile(r"^(static\s+|const\s+)?[\w:<>,\s\*&]+\s+[\w:]+\s*\([^;]*$|^(class|struct|namespace)\s+\w+")
    mutate = re.compile(
        r"\b(gsi_replace|gsi_remove|gsi_insert_\w+|emit_insn\w*|delete_insn\w*|"
        r"validate_change\w*|df_insn_rescan|update_stmt|unlink_stmt_vdef|"
        r"rvtt_refuse\w*|gcc_assert|gcc_unreachable)\s*\(")

    for i, l in enumerate(lines):
        if sig.match(l):
            keep.add(i)
        if mutate.search(l):
            keep |= {j for j in range(max(0, i - 3), min(len(lines), i + 4))}

    out, prev = [f"/* ===== {name} — STRUCTURAL DIGEST ({len(lines)} lines) ===== */"], None
    for i in sorted(keep):
        if prev is not None and i > prev + 1:
            out.append(f"    /* ... {i - prev - 1} lines elided ... */")
        out.append(lines[i])
        prev = i
    return "\n".join(out) + "\n"


def load_source(gcc_root: Path, card: dict) -> tuple[str, str]:
    """Return (source_text, status).

    A split pass lives in several translation units behind a private
    `-int.h` (rvtt_schedule is 309 lines of primary and 9871 more across
    seven siblings).  Sending only the primary file would ask the model to
    judge a transform whose implementation it cannot see, so every sibling
    is concatenated in, each under its own banner.
    """
    if not card.get("file"):
        return "", "absent"
    primary = gcc_root / card["file"]
    if not primary.exists():
        return "", "absent"
    sibs = [gcc_root / s["file"] for s in (card.get("split_siblings") or [])]
    sibs = [p for p in sibs if p.exists()]

    ptext = primary.read_text(errors="replace")
    texts = {p: p.read_text(errors="replace") for p in sibs}

    # Everything whole, if it fits.
    if len(ptext) + sum(len(t) for t in texts.values()) <= MAX_SOURCE_CHARS:
        parts = [f"\n/* ========== {primary.name} ========== */\n" + ptext]
        parts += [f"\n/* ========== {p.name} ========== */\n" + t for p, t in texts.items()]
        return "".join(parts), ("complete" if not sibs
                                else f"complete ({1 + len(sibs)} files)")

    # Otherwise: primary whole, siblings digested.  The primary defines the
    # pass, so it is the last thing to give up.
    # A primary that alone blows the budget gets digested too; nothing else
    # would fit beside it anyway.
    pstatus = "complete"
    if len(ptext) > MAX_SOURCE_CHARS:
        ptext = digest_file(ptext, primary.name)
        pstatus = "digested"
    parts = [f"\n/* ========== {primary.name} ========== */\n" + ptext]
    budget = MAX_SOURCE_CHARS - len(ptext)
    digested, dropped = 0, 0
    for p, t in sorted(texts.items(), key=lambda kv: len(kv[1])):
        d = digest_file(t, p.name)
        if len(d) <= budget:
            parts.append("\n" + d)
            budget -= len(d)
            digested += 1
        else:
            dropped += 1

    status = f"primary {pstatus}; {digested} of {len(sibs)} siblings digested"
    if dropped:
        status += f"; {dropped} omitted for length"
    return "".join(parts), status


def state_digest(state: dict, model: str) -> str:
    payload = {
        "state": state,
        "questions": {
            k: q.model_dump() if hasattr(q, "model_dump") else str(q)
            for k, q in ALL_QUESTIONS.items()
        },
        "model": model,
    }
    return hashlib.sha256(
        json.dumps(payload, sort_keys=True, default=str).encode()
    ).hexdigest()


def score_one(client: TypeSafeClient, card: dict, gcc_root: Path, model: str,
              cache: dict, use_cache: bool) -> dict:
    source, status = load_source(gcc_root, card)
    state = build_state(card, source, status)
    digest = state_digest(state, model)

    if use_cache and digest in cache:
        rec = dict(cache[digest])
        rec["cached"] = True
        return rec

    t0 = time.time()
    resp = client.system_one(state=state, questions=ALL_QUESTIONS, model=model)
    elapsed = time.time() - t0

    answers = {k: a.model_dump() for k, a in resp.answers.items()}
    rec = {
        "pass": card["pass"],
        "file": card.get("file"),
        "tier": card.get("tier"),
        "always_on": card.get("always_on"),
        "gating_mode": card.get("gating", {}).get("mode"),
        "loc": card.get("loc"),
        "source_status": status,
        "state_digest": digest,
        "model": resp.model,
        "usage": resp.usage.model_dump() if hasattr(resp.usage, "model_dump") else dict(resp.usage),
        "elapsed_s": round(elapsed, 2),
        "answers": answers,
        "cached": False,
    }
    return rec


def main() -> int:
    ap = argparse.ArgumentParser()
    ap.add_argument("--cards", default="results/cards.json")
    ap.add_argument("--out", default="results/scores.json")
    ap.add_argument("--cache", default="results/.score-cache.json")
    ap.add_argument("--model", default=None, help="default: jev-latest")
    ap.add_argument("--only", action="append", help="score only these passes")
    ap.add_argument("--workers", type=int, default=6)
    ap.add_argument("--no-cache", action="store_true")
    ap.add_argument("--dry-run", action="store_true", help="estimate size, make no calls")
    args = ap.parse_args()

    doc = json.loads(Path(args.cards).read_text())
    gcc_root = Path(doc["provenance"]["gcc_path"])
    cards = [c for c in doc["cards"] if c.get("resolution") == "ok"]
    if args.only:
        cards = [c for c in cards if c["pass"] in set(args.only)]
    if not cards:
        print("no cards to score", file=sys.stderr)
        return 1

    model = args.model or ts_constants.DEFAULT_MODEL

    if args.dry_run:
        total = 0
        for c in cards:
            src, status = load_source(gcc_root, c)
            st = build_state(c, src, status)
            total += len(json.dumps(st))
        print(f"passes            : {len(cards)}")
        print(f"questions per pass: {len(ALL_QUESTIONS)}")
        print(f"requests          : {len(cards)}")
        print(f"state chars total : {total:,}  (~{int(total/3.5):,} input tokens)")
        print(f"model             : {model}")
        return 0

    if not os.environ.get(ts_constants.API_KEY_ENV):
        print(
            f"error: {ts_constants.API_KEY_ENV} is not set.\n"
            f"       source <your-secrets-file>",
            file=sys.stderr,
        )
        return 2

    cache_path = Path(args.cache)
    cache = {}
    if cache_path.exists() and not args.no_cache:
        cache = json.loads(cache_path.read_text())

    results: list[dict] = []
    failures: list[dict] = []

    with TypeSafeClient() as client:
        with cf.ThreadPoolExecutor(max_workers=args.workers) as pool:
            futs = {
                pool.submit(score_one, client, c, gcc_root, model, cache, not args.no_cache): c
                for c in cards
            }
            done = 0
            for fut in cf.as_completed(futs):
                card = futs[fut]
                done += 1
                try:
                    rec = fut.result()
                    results.append(rec)
                    tag = "cache" if rec["cached"] else f"{rec['usage']['input_tokens']:>6}tok"
                    print(f"[{done:>2}/{len(cards)}] {tag}  {rec['pass']}")
                except Exception as e:  # noqa: BLE001 - report, do not abort the run
                    failures.append({"pass": card["pass"], "error": f"{type(e).__name__}: {e}"})
                    print(f"[{done:>2}/{len(cards)}] FAILED  {card['pass']}: {type(e).__name__}: {e}")

    results.sort(key=lambda r: r["pass"])

    for r in results:
        if not r["cached"]:
            cache[r["state_digest"]] = r
    cache_path.parent.mkdir(parents=True, exist_ok=True)
    cache_path.write_text(json.dumps(cache, indent=2))

    fresh = [r for r in results if not r["cached"]]
    totals = {
        "input_tokens": sum(r["usage"]["input_tokens"] for r in fresh),
        "output_tokens": sum(r["usage"]["output_tokens"] for r in fresh),
    }

    Path(args.out).parent.mkdir(parents=True, exist_ok=True)
    Path(args.out).write_text(
        json.dumps(
            {
                "provenance": {
                    **doc["provenance"],
                    "model_requested": model,
                    "model_served": results[0]["model"] if results else None,
                    "questions": {g: qs for g, qs in GROUPS.items()},
                    "question_count": len(ALL_QUESTIONS),
                    "scored": len(results),
                    "failed": len(failures),
                    "fresh_requests": len(fresh),
                    "usage_totals_fresh": totals,
                },
                "failures": failures,
                "scores": results,
            },
            indent=2,
        )
    )

    print(f"\nscored {len(results)} passes ({len(fresh)} fresh, {len(results)-len(fresh)} cached)")
    if failures:
        print(f"FAILED {len(failures)}: {', '.join(f['pass'] for f in failures)}")
    print(f"tokens (fresh): {totals['input_tokens']:,} in / {totals['output_tokens']:,} out")
    print(f"wrote {args.out}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
