# Welford L-register clobber — investigation findings (multi-agent, verified against a built compiler)

*Scope: the reported Welford silicon failure attributed to explicit `sfpi::l_reg[Lx]` not being
treated as a hard-register lifetime. Recon + fix-design + **adversarial verification that actually
built `/root/sfpi/build/sfpi/compiler` and ran fixtures.** Conclusions below are grounded in
compiled output, not prose.*

## Headline: the hypothesized gap does not reproduce through the sfpi-builtin path on this branch

An `sfpi::l_reg[Lx]` access lowers `builtin → GIMPLE gcall(index=INTEGER_CST) → RTL
unspec_volatile on an allocatable pseudo → hard reg 80+idx`, where the final binding is **forced**
by the single-register class `x<N>` (`rvtt.md:186-205` → `rvtt-constraints.md:28-50` →
`riscv.h:546-627`). Because that pseudo is pinned to exactly `SFPU_REGS_L<N>` and carries a normal
live interval, **baseline IRA already refuses to color a temporary onto L1 while L1 is live.**
The verifier compiled the exact proposed reproducer and a high-pressure variant:

- `lp.C`: pre-IRA audit reports `peak=6` → scheduler/MILP path never entered (gate is
  `old_peak > 8`, `gimple-rvtt-lp-schedule.cc:824,829`); SFP output **byte-identical** flag-on vs
  flag-off. Proves nothing about the fix.
- `hp.C` (6 explicit reads + wide fan): `peak=10 → new-peak=7, applied=yes` — genuinely drives the
  scheduler — and **L1 stays clean**. No clobber.

**On this branch, through `l_reg[]`, both peak≤8 and peak>8 are correct.** This is therefore a
*hardening/verification* task via the builtin path, not a reproduced bug.

## So where is the real Welford failure? The raw-LLK path, not the sfpi path

The reproducer used `sfpi::l_reg[Lx]`, which **creates a pinned pseudo** IRA understands. The actual
Welford inputs `L0–L3` arrive from **raw LLK code** (inline asm / direct ckernel SFPU emission)
that is *outside* ordinary C++ SSA — i.e. there is **no GCC pseudo and no live interval** for those
L-registers. That is precisely the case the builtin path does not model and the reproducer did not
exercise. With no pseudo to pin, nothing tells IRA that L1 holds a live raw value, so an sfpi
temporary in the surrounding region can be colored onto L1 and clobber it.

**Corrected root cause:** the gap is not "explicit `l_reg[Lx]` is unmodeled" (it is modeled, via
single-register-class pinning). The gap is **raw-asm/LLK-defined L-registers have no live interval
visible to IRA.** Design invariant #2 ("propagate liveness across raw-LLK barriers") was the right
instinct; the reproducer just tested the wrong (already-safe) path.

## What survives adversarial verification, and what must be dropped

**Drop (mislabeled — wrong IR):** adding `fixed_color` to `rvtt_sched_value` and reducing MILP
`register_capacity`. The pressure scheduler only **reorders** gcalls (`gsi_move_before`,
`:807`); it never assigns a physical register. The MILP's sole interference row is
`sum_value live(value,slot) <= register_capacity` (`rvtt-lpsolve.cc:351-363`) — a **count**, with
no color dimension. These changes cannot say "keep a temp off physical L1"; they enforce a count
property the model already has. Prevention, if wanted, belongs in **RTL register allocation (IRA
conflict edges)**, not the GIMPLE pressure model.

**Keep (the one enforcing element) — as a detector:** the post-IRA verifier. Wire it at exactly
`INSERT_PASS_AFTER(pass_ira, 1)` (before `pass_rvtt_synth_opcode`, which encodes
`REGNO − SFPU_REG_FIRST` into opcodes and would break DF). Anchor the explicit-LREG interval on the
**readlreg-produced hard regno** with **source-use** endpoints — `[readlreg insn, last insn with a
source operand whose REGNO == L(i)]` — explicitly excluding the `writelreg` index operand. Guard
against const-regs with `regno < SFPU_REG_FIRST + SFPU_CREG_IDX_LWM` (creg ≥ 8 are
`UNSPEC_SFPCSTLREG` immediates, not registers). This turns a silent miscompile into an ICE/`sorry`,
which is worth having regardless.

## Next steps (in order)

1. **Get the real reproducer.** Extract the actual failing Welford shape from the silicon run
   (raw-LLK L0–L3 loads + sfpi consumers), not an all-`l_reg[]` fixture. Compile it on this branch
   and confirm the L1 clobber appears. Until it does, we are fixing a hypothesis, not the failure.
2. **Model raw-asm LREG defs/uses as fixed-register live ranges** so IRA sees them (e.g. surface the
   raw L0–L3 as clobbers/uses on the region boundary, or reserve via IRA conflicts across the
   raw-asm barrier — precise interval `[raw def, last use]`, not blanket region reservation).
3. **Land the post-IRA verifier** as the backstop (above).
4. **Make the regression discriminating.** It must push `old_peak > 8` with an explicit/raw LREG
   live across, and be checked against a deliberately-regressed allocator **or** a checking-assert
   in the verifier — a byte-identical-on-this-branch fixture proves nothing.

## Note for whoever is steering the live patch

Do **not** ship the GIMPLE `fixed_color` + MILP-capacity change as the Welford fix — verification
shows it does not enforce the no-clobber invariant and is byte-identical on the reproducible cases.
The real bug is on the raw-LLK path; the compiler already handles the `sfpi::l_reg[]` path
correctly.
