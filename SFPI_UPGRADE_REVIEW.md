# SFPI Compiler Upgrade — Standing Review & Findings

*Single living review of `SFPI_COMPILER_UPGRADE.md` and the Welford silicon investigation.
Checked against the actual tree at `nkapre/sfpi`. Round 1 of this review was consolidated into the
document's §12 by `f6cc69c` — good; the insights were absorbed. This file is the one place my
opinions and findings live; earlier `WELFORD_LREG_FINDINGS.md` is merged in below.*

## Verdict (current)

The document is **materially more honest** than early revisions and its self-critique (§12) is
genuinely sharp. But it keeps improving *prose about code that does not run* while the tree stands
still: across many commits, **no compiler code has changed** — `rtl-rvtt-lp-alloc.cc` is still a
133-line dump-only stub, both flags are still `Init(0)`, `run-corpus-differential.sh` is still
absent. And there is a hard external contradiction: §2.2 asserts *"Hardware Silicon Baseline:
Functional verification complete"* — but **Welford fails on silicon.** Separately, a verified
investigation (below) shows the *hypothesized* compiler root cause does **not** reproduce through
the `sfpi::l_reg[]` path; the real bug is on the raw-LLK path.

---

## Part A — Document review

### Real progress (credit)
- **§2.2 is now a Ground-Truth-vs-Target table** — labels the pre-IRA pass a "Dump-only stub (133
  lines)", the corpus driver "Absent", both flags honestly `Init(0)`. This is the maturity ledger
  that was missing; the single best change in the document.
- **`tie_kind` drift fixed** — the `LV_PREDICATION`/certifier inconsistency is unified.
- **§12 self-critique is excellent** — "the tie certifier is defined but not invoked," "a
  constraint that exists is not an enabled/satisfied alternative," and the occurrence-ledger
  argument in §12.3 are all correct and deep. Right *kind* of scrutiny.

### The pattern
The Rebut/Harmonize loop now reviews its own aspirational code: §12 applies rigorous compiler-review
standards to the §4.2 target pseudocode — while `rtl-rvtt-lp-alloc.cc` remains a stub. The document
gets more careful about code that does not run; the code that *does* run just failed silicon.

### The silicon row is false, not merely "overstated"
§12.5 softly calls the "Functional verification complete" row *overstated*. Given the observed
Welford failure it is **false** and should read: *"Welford FAILS on silicon."* A ground-truth table
carrying a known-false row is worse than no row. (Nuance from Part B: the failure is real, but its
compiler root cause is not the one the document/model would predict.)

### The model-boundary point (holds, with a correction from Part B)
The §3.1 MILP, §4.2 DSATUR interference, and §4.1 extraction all assume every LREG-resident value
is an ordinary allocatable SSA temporary. That framing is incomplete for LREGs that originate
outside C++ SSA. But note the correction below: for the **`sfpi::l_reg[]`** path the compiler
*already* models the constraint correctly (single-register-class pinning). The genuine hole is the
**raw-LLK** path, which has no pseudo at all.

---

## Part B — Welford L-register clobber: verified investigation

*Recon + fix-design + adversarial verification that **built `/root/sfpi/build/sfpi/compiler` and
ran fixtures.** Conclusions are grounded in compiled output.*

### Headline: the hypothesized gap does not reproduce via the sfpi-builtin path
`sfpi::l_reg[Lx]` lowers `builtin → GIMPLE gcall(index=INTEGER_CST) → RTL unspec_volatile on an
allocatable pseudo → hard reg 80+idx`, with the final binding **forced** by the single-register
class `x<N>` (`rvtt.md:186-205` → `rvtt-constraints.md:28-50` → `riscv.h:546-627`). That pseudo
carries a normal live interval, so **baseline IRA already refuses to color a temporary onto L1
while L1 is live.** Compiled evidence:
- `lp.C`: `peak=6` → scheduler never runs (gate `old_peak > 8`, `gimple-rvtt-lp-schedule.cc:824,829`);
  SFP output **byte-identical** on/off.
- `hp.C` (6 explicit reads + wide fan): `peak=10 → new-peak=7, applied=yes` (scheduler genuinely
  runs) — and **L1 stays clean.**

### Corrected root cause: the raw-LLK path
Welford's `L0–L3` arrive from **raw LLK code** (inline asm / direct ckernel SFPU emission) outside
C++ SSA — **no GCC pseudo, no live interval.** With nothing to pin, IRA doesn't know L1 holds a
live raw value, so a surrounding sfpi temporary can be colored onto L1 and clobber it. The
reproducer used an all-`l_reg[]` fixture and therefore tested the already-safe path.

### What survives verification, what to drop
- **Drop (wrong IR):** adding `fixed_color` to `rvtt_sched_value` + reducing MILP
  `register_capacity`. The scheduler only **reorders** gcalls (`:807`); the MILP is **count-only**
  (`sum live ≤ capacity`, `rvtt-lpsolve.cc:351-363`), no color dimension. It cannot keep a temp off
  physical L1 — it enforces a count property the model already has. Prevention belongs in **IRA
  conflict edges**, not the GIMPLE pressure model.
- **Keep (detector only):** a post-IRA verifier wired at exactly `INSERT_PASS_AFTER(pass_ira, 1)`
  (before `pass_rvtt_synth_opcode`, which encodes `REGNO − SFPU_REG_FIRST` and would break DF).
  Anchor the interval on the **readlreg-produced hard regno** with **source-use** endpoints
  (`[readlreg insn, last insn with a source operand REGNO == L(i)]`), excluding the `writelreg`
  index; guard const-regs with `regno < SFPU_REG_FIRST + SFPU_CREG_IDX_LWM`.

### Note for whoever is steering the live patch
Do **not** ship the GIMPLE `fixed_color` + MILP-capacity change as the Welford fix — verification
shows it does not enforce the no-clobber invariant and is byte-identical on the reproducible cases.
The real bug is the raw-LLK path; the `sfpi::l_reg[]` path is already handled correctly.

---

## Recommendations (consolidated)

1. **Correct the silicon row** to reflect the real Welford failure.
2. **Get the real reproducer** — the actual failing Welford shape (raw-LLK `L0–L3` loads + sfpi
   consumers), not an all-`l_reg[]` fixture. Confirm the L1 clobber on this branch before fixing.
3. **Model raw-asm LREG defs/uses as fixed live ranges** so IRA sees them (precise interval
   `[raw def, last use]`, not blanket region reservation).
4. **Land the post-IRA verifier** as the backstop (above).
5. **Make the regression discriminating** — push `old_peak > 8` with an explicit/raw LREG live
   across, checked against a deliberately-regressed allocator or a checking-assert in the verifier.
   A byte-identical-on-this-branch fixture proves nothing.
6. **Stop improving the description; land one real thing** behind the off flag — the verifier, or
   the first raw-LLK reproducer — rather than another pass over §4.2 or §12.
