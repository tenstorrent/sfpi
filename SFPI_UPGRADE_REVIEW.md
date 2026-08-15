# Review: SFPI_COMPILER_UPGRADE.md — Intelligent Opinion

*Reviewer pass against commit `44a6a70` (branch `nkapre/sfpi`). Every claim below is checked
against the actual tree, not the document's self-description.*

## Verdict

The **engineering thesis is sound and the shipped scheduler is real**; the document's honesty
problem is not that it lies — it labels aspirational content — but that it keeps getting *more
elaborate about what is planned* while *nothing ships*. The last **12 commits touched only
`SFPI_COMPILER_UPGRADE.md`** (`git log --name-only`); not one line of the compiler moved. The
§4.2 "Target Implementation" is now a polished ~490-line exact-DSATUR allocator — backing a
`rtl-rvtt-lp-alloc.cc` that remains a **133-line dump-only stub** emitting `colorability=unchecked`.
The doc-vs-tree gap is widening, not closing.

## Ground truth (verified against the tree)

| Capability | Doc presents as | Actually in tree |
| :--- | :--- | :--- |
| GIMPLE pressure scheduler + independent validator | Shipped | **Real** — `gimple-rvtt-lp-schedule.cc` (1025 lines), unchanged |
| lp_solve MILP adapter | Shipped | **Real** — `rvtt-lpsolve.cc` (482 lines, real `<lpsolve/lp_lib.h>`), unchanged |
| §4.2 14-step DSATUR M2 allocator | "Authoritative Architecture" | **Stub** — `rtl-rvtt-lp-alloc.cc` is 133 lines, `audit_function()` only, `colorability=unchecked`; zero of `solve_m2_exact_coloring` / `execute_rvtt_pre_ira_alloc` / `certify_destructive_tie` exist as compiled code |
| Default-on (`Init(1)`) | "P0 Target" | Both flags still **`Init(0)`** (`riscv.opt:586,593`) |
| `run-corpus-differential.sh` | "required P0 deliverable" (§10.2) | **Absent** from `scripts/` |
| §7 kernel wins / §6 replay figures | "Candidate Opportunity" / "Mockup" | **No silicon or simulator number exists anywhere in the tree** |

## What genuinely improved this revision (credit where due)

The Rebut/Harmonize loop did absorb real correctness fixes into the §4.2 *target* code:

1. **The compile bug is fixed.** The DSATUR lambda is now `[&](size_t colored_count)` (was
   `[&](size_colored_count)`, which could never compile).
2. **Legitimate GCC-15 constraint inspection.** `certify_destructive_tie` now calls
   `preprocess_constraints` and checks `recog_op_alt[...].matches == 0` — i.e. it actually
   verifies the dying operand is tied to destination operand 0 in the selected alternative,
   instead of assuming it. That is the correct API-level way to certify a 2-address tie.
3. **Debug locations handled correctly.** External `DEBUG_INSN` uses are now collected and
   reset *inside* the same grouped-change transaction (`gen_rtx_UNKNOWN_VAR_LOC`), rather than
   left dangling at a substituted pseudo — a real soundness improvement.
4. **Occurrence-level validator.** The precommit check now proves *zero selected pseudos remain*
   in staged patterns, not just that intervals don't collide.
5. **P3 is now a "paired A/B non-inferiority gate"** instead of a vague parallel task — better.

These are good changes. They are also all changes *to prose describing code that does not exist.*

## The core problem

**Motion without progress.** Each round polishes the aspirational allocator (§4.2) or tightens a
gate sentence, and reverts the structural-honesty edits (a maturity ledger; an honest retitle;
promoting silicon to a blocking gate). The net effect is a document that reads as *more done* every
week while the tree stands still. A 490-line exact-coloring engine with telemetry counters and
transactional staging, sitting above a file that literally prints `colorability=unchecked`, is the
single most misleading artifact here — precisely because the code is now good enough to look shipped.

Two smaller technical notes on the new §4.2:
- **Dead-code drift.** `certify_destructive_tie` now hard-sets `kind = MANDATORY_2ADDR` and no
  longer produces `LV_PREDICATION`, yet `solve_m2_exact_coloring` still branches on
  `LV_PREDICATION` when contracting ties. Harmless today, but the enum value is now unreachable
  from the certifier — a sign the two halves are being edited independently.
- **The GIMPLE→IRA gap is still unaddressed in code.** The scheduler runs at GIMPLE; IRA runs
  after. A validated peak≤8 GIMPLE order still does not bind IRA — which is exactly why the
  `11→8` fixture still spills. Only the (unbuilt) M2 pass closes this. Until M2 is real, P0's
  "rescue" is advisory, not guaranteed. This is safe (touched regions fall back byte-identically),
  but the doc's "Guarded Default-On" framing continues to imply more than the two-pass reality.

## The one recommendation

Stop improving the description. **Land one real thing, behind the existing off-by-default flag:**

- **Either** replace the `audit_function` stub with a *minimal* real coloring on the simplest
  closed island (even greedy, no backtracking) so `rtl-rvtt-lp-alloc.cc` produces one committed
  substitution and the `11→8` fixture flips xfail→xpass — proving the GIMPLE→IRA gap is closable;
- **or** produce the *first measured number* — one paired A/B Welford cycle count on Blackhole (or
  even the simulator) — so §7 has one row that is `Demonstrated` rather than `Candidate Opportunity`.

Either is worth more than another Harmonize pass. The plan is not blocked on design; it is blocked
on shipping its first line of M2 code or its first data point. Everything needed to do that is
already understood in this document.
