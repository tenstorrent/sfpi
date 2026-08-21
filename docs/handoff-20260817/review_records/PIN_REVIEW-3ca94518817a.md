# PIN_REVIEW — gcc submodule → 3ca94518817a (pin 14)

Date: 2026-08-20. Reviewed-by: swarm orchestrator on owner order ("cut pin 14"); adversarial
verification per lane, union gates per merger.

## Composition
staging/pin14 = pin-13 union 8ae4a2d6b01 + census-rooting fix (CG 8dee5e84029, wave-8/9
wrong-code closure) + CJ production twins (e0754714a5b) + 9-lane overnight union in order
CT(docs/proofs) CI CU CP-witness CK CN CV CY CZ — all default-off flags, zero exclusions.

## Gates (pin-14 gcc batch merger, evidence ~/sfpi-uplift/pin14-evidence-20260820/)
- Full rvtt.exp: 4540 PASS / 16 FAIL, FAIL set byte-identical to same-recipe base
  (frozen-9 + 7 documented sfpi-env rows reproduced on base).
- Focused twins: every lane family 0 FAIL.
- Corpus flags-off: 3211/3211 .text identical vs base. Reviewed-ON-22: 3211/3211 identical.
- CRAQ pinned sim: 17/17 witness legs.

## Install method (owner-ratified fast path, 2026-08-20)
Gate-proven union binaries installed directly via scripts/pin-install-fast.sh (no rebuild;
sha gate = trust anchor): cc1plus 01aed0d8d58dc79459d32eaaba7e1ad3fa02dede5552c1be224c663705c14bb3,
driver (xg++) 5d3742f5847279f59f155b6cdad907f4c2e561b529d60ea2265bffb5ba21d290, cc1/lto1/xgcc
together (driver embeds option tables). All 25 ON flags + 4 new default-off flags accepted;
default compile verified; pin-13 binaries backed up in-install.

## Quarantine lift
crosscall/crossloop/init-hoist return to the ON set at this pin per wave-9 lift conditions:
census fix (CG) + coverage twins (CJ, adversarially audited SUFFICIENT) + wave-10 clean
verification. Conf-side lift in tt-metal 0d2a35a4ca + ceremony commit.


## Amendment (2026-08-21, auditor side — ledger 18(a)/wave-13/14 mandate)

Full gcc sha of the pinned submodule commit: 3ca94518817a2558474fe9d0d09cedbb296f32cb
(Amendment only; the original record's content is unchanged. Mandated by both the
external audit (master ledger 18(a)) and the independent pin-14 audit, decision 3.)
