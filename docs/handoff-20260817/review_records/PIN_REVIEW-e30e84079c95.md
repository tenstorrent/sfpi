# PIN_REVIEW — gcc submodule → e30e84079c95 (pin 59 + documentation)

CATCH-UP RECORD: this bump advances the superproject gitlink across 40 pin cycles
(pins 20–59) in one step. The gate evidence below was produced by the per-pin
ceremonies on tt-quietbox-0 and recorded tt-metal-side; it was NOT re-run on the
machine that authored this commit (a macOS laptop with no device access, no
`/localdev`, and no toolchain build). This record closes a gitlink gap, it does
not certify a fresh measurement.

Date: 2026-09-17. Reviewed-by: superproject-side reconciliation against the
tt-metal PIN HISTORY (authoritative) and the sfpi-gcc branch tip.

Full gcc sha of the pinned submodule commit:
e30e84079c95a16b81fbe5a043a7ba1d168c5454; the pin-59 compiler commit it sits on is
ebeac6bb71b74205832158b3c958cae00ef02f52

## Why this bump exists

The superproject tip (5906b947, 2026-08-29) announced pin 47 while its committed
`gcc` gitlink still read 2b26eb61700ed787e10ad5e61c6c54b6ba4d7952 (2026-08-22,
pin 19) — a 233-commit, 28-pin divergence between what the HANDOFF described and
what a clone of this branch would actually build. This is the same failure class
the review waves logged twice before (d3311b82 "submodule 4 pins stale", closed
by b0c615da); it reopened at 7x the size. Anyone building `nkapre/sfpi` before
this commit got pin 19, not the pin the docs describe.

## Composition

2b26eb61700e..ebeac6bb71b7 = 233 commits, pins 20–59, spanning the lane campaign
from the laneE/F cohort through laneMA. Terminal commit `ebeac6bb71b`
("riscv: tt: retire two dead SFPU optimize knobs") is the pin-59 dead-flag
retirement. Structural delta measured in this checkout:

- registered passes (`rvtt-passes.def`): 56
- `mtt-` option entries (`riscv.opt`): 113
- `gcc/config/riscv/tt/` : 161 files, 4.9 MB
- `rvtt-refusals.def`: 78,337 bytes (named-refusal registry)

Subsystems that did not exist at pin 19 and are present here: DSATUR LREG
allocator with Dst-row spilling, DAG list scheduler, modulo scheduler, vendored
exact branch-and-bound MILP solver, the generic macro/MOP planner family
(`rtl-rvtt-macro-planner.cc` 131,093 B plus `rvtt-macro-{desc,sched,epoch,
tables,region,ownership,verify}.*`), `rvtt-effects.*`, and the replay
record-hoist framework including `-mtt-tensix-optimize-record-hoist-lift`.
The WP series ran past WP8 through WP15. The predecessor hardcoded
exact-calendar SFPLOADMACRO pass was deleted (5f31e00f0) in favour of the
generic planner, after byte-parity oracles proved the replacement matched.

## Gates (pin-59 ceremony, tt-metal d18e1c3af7, corpus/sweep_2x2.conf entry 59)

Quoted from the ceremony record; produced on tt-quietbox-0, not here:

- union gates: pinned-58 vs union ON-39 — 3300/3300 BYTE-IDENTICAL
  (retiring unused flags changes no codegen)
- chkon rc=0, ZERO ICEs, .text == ON
- DejaGnu: dg 7787 = 7783 + 4 EXACT; FAIL-16 line-identical; ERROR count 0
- installed-driver smoke: both retired flags error (both -m/-mno- forms),
  control compiles
- board UNCHANGED 88W/30P/16L @ FINAL-BOARD sha 4274cdc3 (post-pin-58
  galaxy-replication rebook; canon)
- ON set 39; KNOB_MODES 56
- installed driver sha bf439d4a8c91 (this pin installs the REBUILT DRIVER, not
  cc1plus alone — lp-schedule was a driver-resolved alias)

Paired installed-binary record: tt-metal
`corpus/REVIEW_RECORD-bf439d4a8c91.md` (sweep-side; answers the
installed-binary question this source-pin record does not).

## Known limitations shipped with this bump

- No gate was re-executed for this commit. The DejaGnu FAIL-set identity gate,
  the corpus byte-identity legs, and every silicon cell are inherited from the
  pin-20..59 ceremonies. If this branch is to be measured again, the gates must
  be re-run at this gitlink on a device host.
- `.github/workflows/sfpu-pressure-scheduler.yaml` has never executed (not
  registered with Actions; not on `main`; no PR has ever been opened from this
  branch). It cannot be counted as a gate for this pin.
- `tenstorrent/sfpi-gcc` has no CI at all, so nothing verified these 233
  commits automatically.
- This branch remains diverged from `main` (184 ahead / 37 behind at the time
  of writing); Sidwell's RAW-hazard and LREG-pressure work on `main` is not
  merged here.
- The authoritative board file (FINAL-BOARD.tsv), the adversarial-audit
  charter, and the raw evidence trees live only on tt-quietbox-0 under
  `~/sfpi-uplift` — not in any repo. This record cites them; it cannot
  reproduce them.

## Addendum — the two commits above ebeac6bb71b

95beefffb (README + campaign-note move) and 7c04488e6 (design headers on the
four largest passes) are comment- and file-move-only: no .cc logic, .h, .def,
.opt, machine description or Makefile fragment is touched, so generated code
is byte-identical to ebeac6bb71b by construction and every gate recorded above
applies to this gitlink unchanged.  No re-measurement is owed.

## Addendum 2 — d987ba4899e8

Advances past 7c04488e61b7 by one further commit, d987ba489 ("tt: correct a
false soundness-allowlist claim; unshare a dump name").  That commit changes
one comment and two pass dump-name strings; no transform logic, no option, no
machine description.  Codegen is byte-identical to the reviewed pin-59
compiler ebeac6bb71b, so every gate recorded above carries over unchanged and
no re-measurement is owed.

The dump-name change (rvtt_unspec_prop -> rvtt_unspec_prop_ssa / _rtl) was
checked against the testsuite first: no test scans the bare name.

## Addendum 3 — e30e84079c95

Adds e30e84079 ("tt: correct the flag counts in README and say how to enable
a pass").  README text only; no source, option, or machine description is
touched.  Codegen remains byte-identical to the reviewed pin-59 compiler
ebeac6bb71b and no gate is re-owed.

Note for future pins: documentation-only commits on the gcc branch still
move the branch head, and keeping the gitlink exactly on the head therefore
costs a pin bump per doc commit.  If that churn is unwanted, the alternative
is to let the pin lag deliberately and say so in the conf, rather than
letting it drift silently as it did for 28 pins.
