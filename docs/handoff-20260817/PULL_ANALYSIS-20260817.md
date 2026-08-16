# PULL ANALYSIS — 2026-08-17 (adversarial synthesis of four independent reviews)

Scope: everything landed on the four `nkapre/sfpi` branches since the HANDOFF baselines —
**sfpi-gcc e4b974208 → bb56f1d77**, **tt-metal 55ce75be → 69d61d66**, **craq-sim be8e8597 → f80a8d6**, **sfpi → 9a555cb89f** (gcc pin ddf44ed64 → cd0af49be → bb56f1d77).
Inputs: four adversarial reviews (GCC, CRAQ-sim, tt-metal, cross-repo), each run with independent scratch builds, no silicon, existing checkouts read-only. This document separates **CONFIRMED** (reproduced/verified with evidence) from **PLAUSIBLE/SUSPECT** (credible but not locally verifiable). Same-source (OFF→ON causal) and vs-hand (competitiveness) claims are kept distinct throughout.

---

## 1. Headline verdict

**The newly-landed state is trustworthy in its core claims and clean of hardcoding, but NOT yet trustworthy as an unattended pipeline: 6 CONFIRMED defects, all fixable, none requiring a rollback.**

- Every load-bearing engineering claim survived attack: the profitability recalibration is a real plane, not a curve-fit notch (76 unseen probe points); default codegen is byte-identical to e4b974208; the WP8 oracle-mint→delete chain verifies byte-for-byte with an independent compiler build; all three unification merges are mechanically faithful; craq-sim's recognizers are genuinely gone and Min/Max executes 8/8 bit-exact through the generic path; the pin chain is exact.
- **Zero hardcoding findings across all four reviews** — the non-negotiable rule and its sim extension both hold at the tips.
- The 6 confirmed defects are: one compiler soundness gap (all-lanes-enable proof weakened to "any CC write"), three tt-metal sweep-automation gate defects (DejaGnu RED-on-clean, knob-silicon CRAQ bypass, win→refusal GREEN hole), one refuted commit-message claim (65d2c873 "neither instruction stream changes" is false), and one cross-repo version-skew trap (stale `PINNED_COMPILER_SHA256` vs the rebooked bb56f1d77 Reduce-SDPA baseline).
- The silicon numbers themselves look genuine (honest STOP/regression handling, internally consistent arithmetic), but the recalibration A/B evidence and all Lane D/E raw records live only on tt-quietbox-0 — unauditable from this box, and the device class silently moved p100a→p150 for the recalibration run.

Merge-worthiness: sfpi-gcc bb56f1d77, craq-sim f80a8d6, and the sfpi pin are sound to keep. The compiler all-lanes gap must be fixed **before any lane-predicated shape class (typecast faces, Where successors) reaches silicon**. The tt-metal sweep gates must be fixed **before the next scheduled nightly/weekly is trusted**.

---

## 2. Per-repo findings

### 2a. sfpi-gcc (e4b974208 → bb56f1d77) — verdict SOUND_WITH_FINDINGS

**What landed:** the three-branch unification (planner → sdpa → profit, b3c031380 / b5e07e458 / 6724f48c6), all of WP8 (oracle mint 32e20f9fd BEFORE quarantined-pass deletion 5f31e00f0; raw-word table migration; typecast four-region descriptor sharing 50cad63fa; Where→named refusals; `-mtt-tensix-{analyze,emit}-loadmacro` now hard-error), the mechanism/threshold test separation 12e1dc0b4, and the silicon-driven profitability recalibration bb56f1d77.

**CONFIRMED (all verified with a from-scratch cc1plus build at bb56f1d77):**
- Profitability model recalibration is calibration, not fingerprinting. Decision surface = `benefit = trips*((1+len)*123 − max(123, 100*len)) − (1+len)*123`, MIN_BENEFIT=60 centislots. 76 unseen (trips,len) probes with aperiodic payloads match exactly; surface is piecewise-linear with a single kink at len≈1.23; **no notch near Reduce-SDPA (4,8)=121**, which sits 2 slots inside the trips=4 fire boundary. The 123/100 constants trace to the pre-existing silicon-refit 1.23× delivery model; only threshold 60 is calibration, and any threshold in (0,121] gives identical decisions on all measured points. The commit's central arithmetic is true: the old model ordered the silicon winner (ReduceSDPA modeled 16) below both silicon losers (Log 34, Log1p 62) — no threshold could separate them; the new model is sign-correct on all measured points.
- Default codegen byte-identity vs e4b974208: tip `.text` byte-identical on the pin corpus (3/3 recorded hashes match) plus 238 tensix testsuite files at default flags (238 identical, 0 differ).
- WP8 oracle chain end-to-end: mint precedes delete; re-running mint-wp8-oracles.sh with an independent tip build reproduces every committed hash; staged-loop and cast-round planner output byte-identical to frozen quarantined-pass oracles; all refusal shapes byte-identical in every column.
- Minmax parities: 6/6 in-place `.frozen` parities and 26/26 surviving periodic-minmax oracle-store entries reproduce exactly at the tip.
- No transplanted signbit calendar: formation is structural (subunit + TT_OP-derived opcode byte + store placement); shift rides the imm12 packer with encodability refusal; zero `==-31`/`0xfe1` checks anywhere; probed shifts −31…2047 all form identically.
- Merges are faithful semantic unions (mechanically recreated: trees byte-identical to committed merges).
- 12e1dc0b4 is legitimate: default-gate fire coverage exists for both the counted high-trip class and the exact Reduce-class (4,8) shape.
- Suites green: focused families 1107 passes / 0 fails; full rvtt.exp FAIL set equals the frozen environmental set (only delta: the deleted quarantined-pass's own test).
- Archived Reduce-SDPA D1 baseline corroborated (post.csv shows 834.0 generated vs 840.0 hand).

**REFUTED suspicions:** the recalibration is NOT a curve-fit notch; 12e1dc0b4 does NOT hide a default-gate regression.

**CONFIRMED DEFECT (D1):** the all-lanes-enable proof was weakened without a consumer-side replacement. The planner accepts **any pure CC write** as the ambient enable (`rtl-rvtt-macro-planner.cc:216-266`, both as `rows[0].enable` and via `preheader_trailing_enable`). Behaviorally reproduced on the tip build: a minmax-shaped region whose enable is `__builtin_rvtt_sfpencc(0,10)` (lanes OFF, imm12=0 ≠ SFPENCC_IMM12_BOTH=3) **forms the full frozen macro calendar** with explicit SFPSWAP/SFPSTORE rows deleted. The deleted quarantined pass refused this (required exactly mod1=10, imm12=3). Store/misc semantics under partial lanes are PARTIAL/unproven per NOTES-wp6-prep. `sfpencc_all_lanes_word()` is dead code outside its unit test, and the `rvtt-macro-ownership.cc:98-102` comment overstates the implemented guarantee ("the proof itself is the consumer's" — no consumer implements it; docs/MACRO_PLANNER.md discloses the weaker behavior). **Formation-outside-proven-envelope is CONFIRMED; hardware misbehavior is PLAUSIBLE.**

**SUSPECT / unverifiable:** bb56f1d77's fresh A/B numbers (855.50 refused / 832.75 re-enabled / 839.00 hand, ×3, "BH p150") have no discoverable evidence archive, SHA256SUMS, or lock logs on this machine, and the device class changed p100a→p150 without a recorded cross-device control. The archived-era 834/840 IS corroborated; the recalibration run's protocol items (4) and (7) are unauditable.

**Hardcoding:** none. The `desc_programs[]` three-entry whitelist in rvtt-macro-desc.cc was the closest gray-zone item and passed: keyed by derived structure, never reads op names/coefficients/raw words, unproven values refuse, genericity proven by renamed/varied/near-miss tests and content-independent probes.

**Protocol notes:** WP7 minmax parity manifests (19 distinct-address + 6 in-place) + refusal-oracle store exist only in `/localdev/nkapre/minmax-refusal-identity/` — verified at tip but NOT on the remote (HANDOFF §6b said to commit them; only the WP8 manifest was).

### 2b. craq-sim (be8e8597 → f80a8d6) — verdict SOUND_WITH_FINDINGS

**What landed:** 6c072b5 (delete every §6a recognizer + all recognizer state in sim.h) + f80a8d6; the range touches exactly 5 files.

**CONFIRMED:**
- Recognizer deletion complete: zero hits in src/ for the entire deletion inventory (0x1b8400de, 0x770, 0x312, all template/sequence constants); no renamed re-introduction (no known_shapes/whitelist/admissible tables anywhere). Remaining opcode-value comparisons are architectural encoding facts (0x92 SWAP occupies MAD comparator; 0x94 SFPSHFT2 VB-aliases-immediate), not shape fingerprints.
- Generic path fidelity: SFPLOADMACRO decodes templates/sequence/misc exclusively from SFPCONFIG-written state (tensix.cpp:9685-9703, :9808); events retire at issue+1+Delay via field-rewrite into the ordinary per-instruction executors.
- Behavioral equivalence proven by execution: 8/8 Min/Max CRAQ matrix at f80a8d6 against tt-metal 69d61d66 — all pytest PASS, all 8 math.elf digests EXACT vs the frozen minmax-final-craq-v2 oracle. TTSIM_TRACE_LOADMACRO shows 1024 launches/test through the generic decode on ON legs, 0 on OFF; the ON binaries program seq 0x00dd008c / misc 0x330 — exactly the values the deleted binary_fp_compare recognizer special-cased, now executed purely from programmed state.
- Adversarial never-whitelisted descriptor passes bit-exactly vs its explicit decomposition (wh + bh). Diff-fuzz strengthened, not weakened (12→13 directed tests; old recognizer-path oracles replaced by independent explicit decompositions); 1000/1000 PASS on both arches. aabbd10c (SFPSTORE mod0=9 LO16) retained. Sim CI green; non-macro default behavior unchanged (event machinery gated on pending macro events).

**SUSPECT (tracked, no fix required before merge):**
- `execute_load_macro_template_direct` (tensix.cpp:11250–~11660) is a ~450-line parallel reimplementation of 12 opcode classes used for the two non-encodable overrides (LReg16 target; SFPSHFT2 VB←VD). Opcode-generic — no directive violation — but a permanent semantic-divergence risk surface, pinned only by the fuzz harness. Its coverage gap (e.g., SFPSWAP into LReg[16] → precise UnsupportedFunctionality) is a fidelity ceiling.
- Randomized fuzz trials derive the expected rewritten SWAP word via the same `build_dispatch` under test (pre-existing partial self-reference; directed tests are independent).

**Hardcoding:** none. Raw words survive only in tests/diff-fuzz expectations (permitted).

**Protocol:** no violations. Two deliberate documented deviations: absolute scheduling instead of the suggested optional 4-sub-unit FIFO (justified: mul_int32 relies on event passing), and the delayed-event model going always-on by design (the old default IS the thing §6a ordered deleted). State note: f80a8d6 exists only on ref pull/ns from the review box's view; confirm origin/nkapre/sfpi actually points at f80a8d6.

### 2c. tt-metal (55ce75be → 69d61d66) — verdict DEFECTS_FOUND

**What landed:** 10 commits, all under tt-llk tests/corpus + one perf source: sweep_2x2 automation (sweep_2x2.py/.conf/_ops.tsv, weekly/nightly scripts), p150 chip-class baselines (3102fdbe, sfpu_device_baseline_p150_v1.tsv), Min/Max perf-harness fixes (65d2c873 clamp, 4ff5c848 compile fix), typecast audit move (722d7a8c), Reduce-SDPA baseline pair update (69d61d66). No SDPA kernel / LLK header / harness-python changes — SDPA compile inputs unchanged from verified 55ce75be.

**CONFIRMED sound:**
- SDPA sem-corr node compiles OFF/default deterministically (2 runs, identical .text sha).
- sweep_2x2.py's main path encodes §1: classify-before-device, hardened BH CRAQ gate (SKIP_NO_SIMULATOR cannot open it), both flocks, 3 fresh procs/leg alternating OFF/ON, in-lock CSV copies, evidence SHA256SUMS, baselines never auto-updated.
- The 834/840 Reduce-SDPA pair is presented as a fresh p150 paired A/B with the exact-equality-to-p100a explicitly labeled a reproduction; the update followed the documented STOP-then-reviewed-manual-update flow (3102fdbe seeded 839/855.5 as measured_known_regression STOP first).
- Chip-class separation is structural on the data side (separate per-class baseline files, chip_class column, p100a file immutable, no cross-class arithmetic).
- Committed numbers internally consistent (TSV×tile_cnt reproductions, causal/vs-hand arithmetic, corpus --validate clean); no op names in sweep_2x2.py.

**CONFIRMED DEFECTS:**
- **(D2) weekly_bh_sweep.sh DejaGnu gate is broken — RED precisely when clean.** `FAIL=$(grep -c '^FAIL' g++.sum || echo 0)` yields the two-line string `0\n0` on zero fails (grep -c prints 0 AND exits 1), `[ "$FAIL" -eq 0 ]` errors, `||` branch sets RC=1. Reproduced in bash. The intended zero-FAIL enforcement never functions.
- **(D3) knob_silicon() bypasses the CRAQ gate and paired correctness.** In sweep_2x2.py `run()`, weekly per-knob device jobs execute BEFORE the BH-CRAQ gate is evaluated, consult no craq verdicts, and never run correctness for the single-knob flag sets (which were also never CRAQ'd). Violates §1(3)/(6) ordering for every weekly headline row.
- **(D4) win→refusal regressions pass GREEN.** If a previously winning row goes OFF/ON byte-identical (planner stops firing), `report()` emits "refusal byte-identical: GREEN" and skips the flip check; measured baseline cells are never consulted for refusal rows. Total-refusal regressions are invisible.
- **(D5) 65d2c873's claim "neither instruction stream changes" is REFUTED.** Recompiled at 65d2c873~1 vs tip, same toolchain/flags: math.elf `.text` changes for BOTH impls (impl0 2064B/8bbfd337→2040B/b5fefe7b; impl1 2364B/bafafbaa→2252B/2bd37e73); the clamp is inside the timed TILE_LOOP zone. Both "harness" fixes change the MEASURED KERNEL. Consequence: p150 Min/Max cells are NOT same-source with the p100a 226.65/156.99/144.02 records (which also predate 4ff5c848 — measured via shims on a kernel that cannot compile at 55ce75be). The p150 2×2 is internally same-source and valid; only the "p100a record CONFIRMED" annotations ride on a changed kernel + changed chip and must be reworded to ratio-level reproduction.

**SUSPECT:**
- Typecast annotation stale/contradictory: "WP8 step 4 targets it" — but step 4 (50cad63fa) landed BEFORE the measuring compiler 4633999c was built. Either the shared descriptor fired and the annotation misattributes the residual +22.64%, or it refused and the row should record the refusal. No fired/refused dump archived for the real node's ON leg.
- TYPECAST_BODY metric validity: the ON leg is a macro-launch shape measured with a BODY-family marker, no math-drain barrier, and no recorded issue-slot lower-bound check — the TSV's own header says BODY is invalid for fire-and-forget launch shapes (§1 metric caveat + §5.6 unaddressed).
- Provenance completeness: no per-cell .text hashes; most cells lack raw 3-sample values; actual run records live only in ~/sfpi-uplift on tt-quietbox-0.
- Aggregation inconsistency: sweep_2x2.py aggregates by MEAN, but the committed minmax-max hand_off cell 18.2236 is the MIN of its own provenance samples (18.2266/18.2236/18.2236; mean 18.2246…) — that baseline cell was picked outside the tool's aggregator, so the tool cannot reproduce its own baseline.
- p150 TSV header asserts compiler 4633999c for the whole file while the two reduce rows were measured with a bb56f1d77-built compiler (see D6). sdpa hand cells have no paired physical correctness leg at this pin (honestly recorded in the TSV).

**Hardcoding:** none in decision logic. Config-declared-not-discovered risks: sweep_2x2_ops.tsv duplicates corpus node info without cross-validation (722d7a8c is exactly the failure class that would have caught); HEADLINE_ROWS is a hardcoded op list (acceptable budget knob, drift risk); **chip-class is asserted by config, never probed against the attached device** — run on a p100a box and cells are silently recorded as p150; scoreboard.tsv lacks a chip_class column.

### 2d. sfpi superproject (→ 9a555cb89f)

**CONFIRMED:** pin chain exact (submodule pin = bb56f1d77…896, byte-equal to sfpi-gcc origin/nkapre/sfpi; both pin-advance commits describe what actually landed); the gcc tree at the pin builds (cc1plus/xg++ 15.1.0, no dangling references to deleted rtl-rvtt-loadmacro.cc, removed flags error on use); scripts/build.sh unchanged from the HANDOFF-verified state; the §18.9/B0 greenfield reconciliation IS fixed (SFPI_COMPILER_UPGRADE.md:2032 supersession block).
**Residual:** the B0 block still cites "pinned gcc ddf44ed64" (stale by two advances) and the superseded GREENFIELD body text below the banner is unedited. Minor: gcc capability-table comments cite line numbers of the deleted rtl-rvtt-loadmacro.cc (documentation, acceptable).

---

## 3. Cross-repo coherence

**Coherent:** unification landed in the §10-required order; WP8 oracle-before-deletion ordering honored; the WP8 typecast shape emitted by the tip compiler decodes cleanly in the new generic sim decoder (templates 0x900000C0/0x8E0000D1, sequence 0x534D0004, misc 0x100 — the same words the deleted cast-round whitelist matched, now derived compiler-side and decoded generically sim-side); 50cad63fa's blocked-on-sim-coverage class was discharged by Lane E gating silicon through the NEW sim (f80a8d64 per sweep conf); the recalibrated gate fires (4,8)→121≥60 at default and still refuses Log/Log1p.

**The p150-vs-p100a chip split:** structurally handled on the data side (per-class files, chip_class column, no cross-class arithmetic), but three seams remain: (a) the recalibration A/B silently moved device class p100a→p150 with no cross-device control recorded; (b) chip class is config-asserted, never device-probed; (c) the HANDOFF §3 scoreboard header still says p100a while p150 is now the authoritative class. Key p150 re-measurements: SDPA sem 1289→1036 (−19.63% KERNEL) / hand 1009; Min/Max −30.72% / +8.26% vs hand; Signbit planner-fired −22.98% causal, beats hand −5.81%; Expm1 −2.49%; Lerp −13.43%; Exp +37.61%; SigmoidAppx +63.68%; Typecast worsened to +22.64% vs hand.

**Compiler-sha keying — CONFIRMED DEFECT (D6), the version-skew trap:** tt-metal 69d61d66 rebooked the Reduce-SDPA baseline pair from a **bb56f1d77-built** compiler but `sweep_2x2.conf` `PINNED_COMPILER_SHA256` still names the pre-recalibration 4633999c build, and the p150 TSV header claims 4633999c for the whole file. The next scheduled nightly either refuses to sweep (sha mismatch) or runs the OLD compiler — whose Reduce-SDPA regression (+1.97% vs hand) sits WITHIN the 5% MAX_DRIFT_PCT of the new 834 baseline, so the sweep could silently bless the regressed compiler as GREEN. Compounding: no per-row compiler_sha in the baseline/scoreboard schema, so mixed-era cells are not machine-distinguishable; the bb56f1d rows record only "pack 9ca4a98c", not a binary sha256.

**Unauditable from this box (not asserted as violations):** all Lane D/E evidence dirs and the recalibration A/B records (tt-quietbox-0 only); whether Lane E used `--skip-craq-gate` (escape hatch exists); whether the p150 SDPA ON bytes literally equal b0d9e72e.

---

## 4. Required fixes, ranked by severity

**P0 — soundness (before any lane-predicated shape reaches silicon):**
1. **Wire the all-lanes proof** (sfpi-gcc `rtl-rvtt-macro-planner.cc:216-266`): consume `sfpencc_all_lanes_word()`/an operand check as the consumer-side proof, or CRAQ-prove the partial-lane envelope. Add tests both directions (lanes-off must refuse or be proven). Fix the overstated comment at `rvtt-macro-ownership.cc:98-102`.

**P1 — gate integrity (before the next scheduled sweep is trusted):**
2. **Fix weekly_bh_sweep.sh FAIL counting** (e.g. `FAIL=$(grep -c '^FAIL' g++.sum); [ -z "$FAIL" ] && FAIL=0` — grep -c already prints 0) so zero-FAIL gates GREEN and nonzero gates RED.
3. **Move knob_silicon() behind the BH CRAQ gate** in sweep_2x2.py `run()` and add paired correctness (minimum: classify+CRAQ) per OFF+single-knob flag set before its device legs.
4. **Close the win→refusal hole in report()**: REFUSAL_BYTE_IDENTICAL on a row whose chip-class baseline has measured sem_off/sem_on cells = RED, not GREEN.
5. **Promote PINNED_COMPILER_SHA256** in sweep_2x2.conf to the bb56f1d77-built compiler's sha256 (note the change in the baseline header) BEFORE the next nightly; add a compiler_sha (ideally craq_sim_sha) column to the baseline/scoreboard row schema; record the bb56f1d rows' full binary sha256.

**P2 — measurement honesty / provenance:**
6. Reword 65d2c873's comparability claim and the p100a "CONFIRMED" annotations: the clamp changes the measured kernel's .text for both impls (verified); these are ratio-level reproductions across a changed kernel and chip, not same-source identity.
7. Run the planner dump on the real `metal__ckernel_sfpu_typecast` BH node with the bb56f1d77 compiler; record whether the WP8 four-region shared descriptor forms; re-book the +22.64% cause (formation refusal vs fired-but-insufficient) and fix the "step 4 targets it" phrasing in corpus notes and 52d4723a's record.
8. Implement the §1 issue-slot lower-bound sanity check in the automation; add a drain barrier or KERNEL-marker leg for BODY-marker macro-launch rows (typecast, minmax); record the check for the existing p150 cells.
9. Add a device chip-class probe (tt-smi/board id) to sweep_2x2.py preflight matching CHIP_CLASS; emit chip_class in scoreboard.tsv. Make baseline-cell aggregation match the tool's aggregator (mean vs min) or record the aggregator per row.
10. Archive/link the bb56f1d77 p150 recalibration A/B evidence (with the p100a→p150 device-change note and independent-review record); push the WP7 minmax parity manifests + refusal-oracle store (`/localdev/nkapre/minmax-refusal-identity/`) into the repo as testsuite support data per §6b.

**P3 — documentation / hygiene:**
11. Re-record the no-silicon profitability band: acceptance region [60,121) modeled benefit — including the test-pinned (4,9)=90 fire — has zero silicon points; restore rvtt-cost.md's dropped sentence about non-itemized dynamic pipeline costs. Silicon A/B stays the promotion backstop.
12. Add `loop_trip_weight` (0ee353061) to the carry-forward list (profitability-only, perf risk not soundness) rather than silently widening the M2a exception.
13. Have sweep_2x2.py cross-validate sweep_2x2_ops.tsv against sfpu_corpus_v2.tsv (corpus_id exists; nodes ⊆ mapped modules).
14. SFPI_COMPILER_UPGRADE.md: refresh the B0 block's "pinned gcc ddf44ed64" citation to bb56f1d77; edit or clearly strike the superseded GREENFIELD status lines in §18.9.2/§18.9.6.
15. Rewrite the stale HANDOFF sections (list in §5 below — applied to /home/nkapre/HANDOFF.md alongside this report).

---

## 5. HANDOFF correction list (stale statements as of these tips)

- **§2 sfpi row:** "tip c0906f19 … VERIFY the pin is still ddf44ed64" — FALSE. Tip 9a555cb89f, pin bb56f1d77 (advanced twice via cd0af49be).
- **§2 sfpi-gcc row:** "unification IN FLIGHT … merge them yourself" — DONE, plus all of WP8 and the recalibration; tip bb56f1d77.
- **§2 tt-metal row:** tip 55ce75be — now 69d61d66 (+10 commits: sweep automation, p150 baselines, Min/Max harness fixes, typecast audit move, Reduce-SDPA baseline update).
- **§2 craq-sim row / §9 bootstrap:** "be8e8597 (cleanup authorized)" / "DO NOT MODIFY" — cleanup DONE at f80a8d6; checkout f80a8d6; hands-off note obsolete.
- **§2 branch-contents profitability formula:** superseded — new model `benefit = trips*(deliver − max(123, execute)) − deliver` in centislots, MIN_BENEFIT=60; `-mtt-tensix-replay-hoist-min-benefit=` CHANGED UNITS (slots→centislots) and default (64→60).
- **§3 NOTE "SDPA b0d9e72e ON bytes NOT yet silicon-measured":** superseded — Lane D measured the SDPA 2×2 on p150 (sem 1289→1036, −19.63% KERNEL; hand 1009). Caveat: whether those ON bytes are literally b0d9e72e is not recorded in-repo.
- **§3 "Signbit must be reproduced by the planner before it counts":** DONE — p150 planner-fired sem ON −22.98% causal, beats hand −5.81%.
- **§3 open losses:** Expm1 0.000%→p150 −2.49%; Lerp −2.75%→−13.43%; Min/Max re-confirmed −30.72%/+8.26%; Exp +70.7%→+37.61%; SigmoidAppx →+63.68%; Typecast worsened to +22.64% and the "needs §6b step 4" attribution is now SUSPECT (step 4 already in the measuring compiler). Scoreboard header "p100a" no longer authoritative — p150 is.
- **§5 item 5:** "band ~64..148 has no silicon point" — falsified in both directions: four silicon points now exist (ReduceSDPA WIN, SDPA-exp WIN, Log/Log1p LOSS); old model refuted and replaced; NEW no-silicon band is [60,121).
- **§6a and §6b worklists:** both DONE (oracle-before-deletion ordering honored), pending this review's fixes (all-lanes proof; WP7 manifests not yet committed).
- **§9:** "gcc lands at ddf44ed64" — wrong; also tt-metal checkout → 69d61d66.
- **§10 OFF flag set:** still lists `-mno-tt-tensix-emit-loadmacro`, which now ERRORS on the tip compiler (sweep_2x2.py already dropped it).
- **§18.9 residual (sfpi repo):** B0 supersession block cites stale pin; GREENFIELD body text unedited.

---

## 6. Review provenance

- GCC review: scratch worktree /localdev/nkapre/adv-review-gcc-tip @ bb56f1d77, build /localdev/nkapre/adv-review-build-tip; probe artifacts in session scratchpad (adv-probe.C, adv-encc.C/.s, adv-oracles/, adv-dejagnu/, adv-sweep/).
- CRAQ review: scratch worktree /localdev/nkapre/adv-review-craq-f80a8d6 (libttsim bh/wh/qsr) + /localdev/nkapre/adv-review-ttmetal-69d61d66; matrix evidence in scratchpad craq-matrix2/.
- tt-metal review: scratch worktree from tt-metal-fresh-cpp-attack @ 69d61d66 (since removed), macro-planner-craq2-toolchain.
- Cross-repo review: read-only over the pulled checkouts; empirical legs: tip gcc build, removed-flag checks, typecast/lowtrip dg-final verification, diff-fuzz re-run 1000/1000 bh+wh, static descriptor decode.
- Not verified anywhere: silicon numbers (no device access), tt-quietbox-0 evidence trees, Lane E `--skip-craq-gate` non-use, p150 SDPA ON bytes == b0d9e72e.

**CONFIRMED defect tally: 6** (D1 all-lanes formation gap; D2 DejaGnu RED-on-clean; D3 knob-silicon CRAQ bypass; D4 win→refusal GREEN hole; D5 refuted 65d2c873 comparability claim; D6 pinned-compiler-sha skew). Everything else is SUSPECT/PLAUSIBLE or a documentation/hygiene gap.
