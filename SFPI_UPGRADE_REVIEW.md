# Review: SFPI_COMPILER_UPGRADE.md — Intelligent Opinion (Round 2)

*Reviewer pass against commit `69561bc` (branch `nkapre/sfpi`). Every claim below is checked
against the actual tree. (Round 1 of this review was consolidated into the document's §12 by
`f6cc69c` — good; the insights were absorbed.)*

## Verdict

The document is now **materially more honest** and its self-critique (§12) is genuinely sharp.
But the loop has turned inward: §12 is now a rigorous code review *of §4.2 pseudocode that is not
in the tree*, and across another ~5 commits **not one line of compiler code has changed** (last
commits touch only the two `.md` files; `rtl-rvtt-lp-alloc.cc` is still the 133-line dump-only
stub, both flags still `Init(0)`, `run-corpus-differential.sh` still absent). And there is now a
**hard external contradiction**: §2.2 asserts *"Hardware Silicon Baseline: Functional verification
complete"* — but **Welford fails on silicon.** That row is not merely "overstated" (as §12.5
softly says); it is **false**, and the failure exposes a correctness hole the whole document misses.

## Real progress since Round 1 (credit)

- **§2.2 is now a Ground-Truth-vs-Target table** — labels the pre-IRA pass "Dump-only stub (133
  lines)", the corpus driver "Absent", both flags honestly `Init(0)`. This is exactly the maturity
  ledger that was missing; it is the single best change in the document.
- **`tie_kind` drift fixed** — the `LV_PREDICATION`/certifier inconsistency I flagged is unified.
- **§12 self-critique is excellent** — "the tie certifier is defined but not invoked," "a
  constraint that exists is not an enabled/satisfied alternative," and the occurrence-ledger
  argument in §12.3 are all correct and deep. This is the right *kind* of scrutiny.

These should stay. The problem is what §12 is scrutinizing, and what it doesn't.

## The decisive finding: the silicon row is false, and it reveals a model-level hole

**§2.2 "Functional verification complete" is contradicted by an observed silicon failure: Welford
clobbers `L1`.** The mechanism (confirmed against the lowering in `include/sfpi_classes.h:187-260`):
an explicit `sfpi::l_reg[Lx]` lowers via `__builtin_rvtt_sfpreadlreg` / `sfpwritelreg` /
`sfpassign_lv` to a reference to a **fixed** architectural L-register. Welford's inputs arrive in
`L0–L3` from raw LLK code *outside* ordinary C++ SSA. The generated code consumes `L0`, then later
reads `L1` — but the allocator assigns a vFloat temporary to `L1` *before* that read, because
nothing in the model marks `L1` as holding a live, fixed-register value across the raw-LLK barrier.

**This is upstream of everything §12.1–12.4 discuss.** Those sections are about correctly *wiring*
the certifier and occurrence-validator for the coloring machinery. But the Welford clobber does not
require any of that machinery to be wrong. You can certify every destructive tie, prove perfect
occurrence-to-color correspondence, and *still* clobber `L1` — because the interference graph never
contained a node or edge for `L1`'s explicit lifetime in the first place. The §3.1 MILP, the §4.2
DSATUR interference model, and the §4.1 extraction all implicitly assume every LREG-resident value
is an ordinary allocatable SSA temporary. **An explicit `l_reg[Lx]` sourced from raw LLK is a
fixed-register lifetime, and the document's model has no representation for it.** That missing
invariant is the actual silicon bug — not a wiring gap, a *model-boundary* gap.

## The pattern, restated

The Rebut/Harmonize loop is now reviewing its own aspirational code. §12 applies genuinely rigorous
compiler-review standards to the §4.2 target block — while `rtl-rvtt-lp-alloc.cc` remains a stub and
the one thing that would have caught the Welford bug (a fixed-lifetime model for explicit LREGs) is
absent from both the prose and the tree. The document keeps getting more careful about code that
does not run, and the code that *does* run just failed silicon on a case the model cannot express.

## Recommendations

1. **Correct the silicon row now.** Per ground truth it should read: *"Welford FAILS on silicon —
   explicit-LREG (`L1`) clobber; root cause: explicit `l_reg[Lx]` not modeled as a fixed-register
   lifetime."* A ground-truth table that carries a known-false row is worse than no row.
2. **Add the missing invariant to the model, not just the wiring.** State it explicitly in §4.1
   extraction and §3.1 constraints: *an explicit `sfpi::l_reg[Lx]` is a hard-register contract* — a
   fixed def/use with a live interval and interference edges, whose liveness propagates across
   raw-LLK / SFPU barriers. The fix is **precise liveness** (interval ends at the last explicit
   use, so the LREG is reusable after), not global reservation of the LREG for the region. Add the
   forbidden-color/precolor constraint in *both* the MILP and the list allocator, and a
   post-allocation verifier that rejects any temporary overlapping a live explicit LREG.
3. **Land the minimal regression.** Raw code loads `L0–L3`; generated code consumes `L0`, then
   `L1`; assert no temporary is written to `L1` before its explicit consumer. This must fail on
   today's tree and pass after the fix — it is the first test that would have caught the silicon
   failure at compile time.

A dedicated design pass (recon → fix design → adversarial verification) is already producing the
per-file patch plan and this regression for the explicit-LREG invariant. That work — plus fixing
the false silicon row — is worth more than any further pass over §4.2.
