# PIN_REVIEW — gcc 8ae4a2d6b01d53c8d646be759c1685b298fe9f9f (pin 13)

RETROACTIVE RECORD: created 2026-08-19 by lane CP (wave-9 item 13(d)
remediation). The bump itself — sfpi commit `5a1cfe6`, "Advance gcc
submodule to pin-13 union 8ae4a2d6b01" — landed BARE (5th consecutive
wave of the bare-bump chronic). This record collects the review evidence
that existed at bump time; the pin-review-lint gate added alongside it
makes a sixth recurrence structurally impossible on CI-covered pushes.

## Pin contents

Pin-12 base `7b4e4d96fb6` + six independently reviewed lanes, merged in
order by the batch merger onto `staging/pin13`:

1. BV `3d8c1fc1aef` — reduce-sdpa re-record pricing split (binding-resource
   pricing; restores the archived 832.75 formation byte-exact).
2. BW `f071f551eb5` — counted-row multidef NAMED refusal + replay-owner
   word-exact sequence barrier (fixes all 11 pin-12 withheld rows).
3. CC `67830bb5896` — ims-carrier former (flag stays OUT of the ON set).
4. CD `03549943022` — crossloop-hoist (exp +4.31→+1.90 vs hand, measured).
5. CA `3af885d073c` — drain-backedge elision + D2 init-hoist (minmax/where
   measured wins).
6. CF `66332ec60dd` — const-residency CC-canonical peel + TU value reuse +
   SFPIADD-imm/SFPDIVP2 latency audits (incl. its own repeat-raw-claim
   soundness fix).

## Review basis (existed at bump time)

- Per-lane independent review entries in the swarm ledger (each lane
  reviewed before batch entry; see session ledger 2026-08-19).
- Union gates, evidence dir `~/sfpi-uplift/pin13-evidence-20260819/`
  (tt-quietbox-0): flags-off byte-identity 5462/5462; ON-set 163 changed
  shapes ALL attributed; DejaGnu full rvtt.exp with unexpected-FAIL set
  byte-identical to the frozen-9 reference (`dejagnu/fail-set.txt`);
  CRAQ 53/53; witness_preflight ALL GREEN on the INSTALLED binary
  (cc1plus sha256 `8e87fba0e35f2a2b4a80981310afc3601a1ce34131518dea1b62c6afc1b030d5`).
- Installed-binary review record (tt-metal side, in-repo):
  `tt_metal/tt-llk/tests/corpus/review_records/REVIEW_RECORD-8e87fba0e35f.md`
  at tt-metal `cae6b25869` (work/nkapre-sfpi).

## Post-hoc adversarial review of this pin

Wave-9 verdict (sfpi `4adac11aac1`, HANDOFF item 13): BV pricing split
VERIFIED GOOD (resolves ledger 9(i)); defaults identity 471/471; suites
frozen; 5/6 merges clean automerges; CF consumes sfpencc_all_lanes_word.
FINDINGS AGAINST the pin: census rooting hole unfixed with two new
consumers (crossloop, init-hoist), init-hoist ICE on non-trivially rooted
census, zero-trip prose still false → the three census-dependent flags are
QUARANTINED (tt-metal `e6375987677f`, ON set 25→22) pending lane CG's
census-rooting fix; BW's replay-owner barrier lacked an in-tree witness —
closed by lane CP on sfpi-gcc branch `agent/bw-witness` (synthetic
interposed-owner witness, red on pre-fix pin-12 base / green on pin-13,
full rvtt.exp FAIL set byte-identical to frozen-9).
