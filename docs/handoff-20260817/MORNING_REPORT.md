# Morning Report — SFPI Overnight Autonomous Session (2026-08-16 ~00:30 → ~05:15 UTC)

All results below are same-source Blackhole A/B (identical source/input/golden, only compiler flags differ), paired CRAQ before silicon, paired physical correctness before profiling, 3 fresh profiler processes per selector under both locks, every run deterministic ×3. Same-source claims and vs-handwritten claims are kept separate throughout. **Nothing was pushed to any remote.**

## 1. Headline: SDPA Exp Unclamped — −31.8% same-source, gap to hand cut from +49.5% to +13.1%

Four generic compiler mechanisms landed and were silicon-verified tonight on the **original unchanged source**, each independently reviewed before its silicon run:

| Stack (cumulative) | ON cycles | same-source Δ vs OFF 1048 | vs handwritten 632 |
|---|---:|---:|---:|
| replay-hoist only (pre-existing) | 945×3 | −9.83% | +49.5% |
| + region-scoped invariant ownership (1b32104bc) | 745×3 | **−28.91%** | +17.9% |
| + typed Dst auto-increment (a5ad58956) | 725×3 | **−30.82%** | +14.7% |
| + final-row→launch conversion (75c34aba2) | **715×3** | **−31.77% (1.466x)** | **+13.13% (+83 cycles)** |

**SDPA 2x2** (the only op tonight with a distinct handwritten variant): semantic OFF `1048` / semantic ON `715`; handwritten OFF = handwritten ON (`.text` byte-identical `45accd4b…`/1392 B, one physical baseline reused by identity) = `632`. Passes do not touch hand code; the compiler claim is −31.77%; the competitiveness claim is +13.13%.

The falsifiable pre-silicon target was **fully met**: payload 16 slots, zero constant SFPLOADI in the capture, zero per-row TTINCRWC, no scalar inner backedge. `.text`: OFF `c9ac3da4…`/1028 B (byte-identical across every stack — flag-gating proven), final ON `1f44d8fa…`/1056 B.

Branch `agent/sdpa-partial-invariant`: `d7fbeedff → b313c147f (unroll + ICE fix) → 1b32104bc (scoped ownership) → a5ad58956 (dst-autoincr) → 5a90f5d45 (pragma-unroll guard) → 0cf27c64c (M2a SETC16 dominating placement + distance guard) → 75c34aba2 (M1 launch conversion)`. Three independent reviews, all APPROVE_WITH_NOTES, all notes recorded. Evidence: `/localdev/nkapre/sdpa-gap-{,scoped-,incrwc-,m1m2a-}evidence-20260816` (manifests 46/211/200/236 entries).

**Model correction for next steps:** M1 delivered structurally (8th row is now an 8th launch) but only −10 of the modeled −68 cycles — the RISC-push:replay cost ratio is ~1.23:1, not 2.2:1. Re-fit the delta model before trusting the M2b prediction (578–595, i.e. beating hand) — directionally right, magnitude optimistic. M2a correctly **refused** on this kernel (opaque face-advance asm inside the face loop); it fires as-is once the typed face-advance migration (already built, see §3) reaches the SDPA kernel path.

## 2. Replay-hoist is shape-dependent — measured, then gated

| Op | OFF | ON | Δ | Verdict | Hand variant |
|---|---:|---:|---:|---|---|
| SDPA Exp (hot-loop shape) | 1048 | 945 | **−9.83%** | win | 632 |
| Log (17-slot, 3-trip) | 608.83×3 | 619.84×3 | **+1.81%** | REGRESSION | none — 2x2 collapses |
| Log1p (31-slot, 3-trip) | 1088.84×3 | 1113.84×3 | **+2.30%** | REGRESSION | none — 2x2 collapses |

Note: the incoming claim that Log1p was "already CRAQ-green" was **false** — builds were staged but never executed. CRAQ was run properly first (PASS/PASS) before silicon.

**Response, landed tonight:** generic profitability gate `c8ed82248` on `agent/replay-hoist-profitability` — benefit = (trips−1)×re-record(len) − record-only(len) − trips×launch, all constants in the target cost table, proven trips only. Under the gated compiler Log and Log1p ON become **byte-identical refusals** (CRAQ PASS/PASS) while the SDPA hoist still fires byte-identical to the measured 945-class binary. Evidence `/localdev/nkapre/replay-profit-evidence-20260816`. Needs independent review before push.

**Latency-schedule first data point:** Expm1 — genuinely different `.text` per selector, measured **exactly 0.000%** (identical raw counter 159467.0 in all six processes): Blackhole's dynamic stalls absorb single-slot static reorders. No hand variant.

## 3. SFPLOADMACRO re-engineering: hardcoding → automatic planner (the charter item)

Branch `agent/generic-macro-planner` (from e4b974208), chain `WP0 ce8b04ea1 → WP1 30d3c6207 → WP2 08930f61e → WP3 13e600d88 → WP4 fa79564e3 → tables de1542810/efd47e6f3 → WP5 52d258292 → WP6 6604fedf9 → WP7 9e1aae005 → label e40af0014 → in-place fix 9609b7dd4`:

- **Magic word `0x37120004` deleted from the compiler** (WP1); replaced by a typed `rvtt_ttdstface` builtin/insn that assembles byte-identically (`0xdc480010`). Companion SFPI wrapper `ded6e4e9d` and LLK migration `df504b3b2` landed on `agent/typed-face-advance` branches; **8/8 minmax CRAQ with the typed trio produces ELFs byte-identical to the frozen raw-word oracle.** WP6-prep even proved the word decodes exactly as `SETRWC(0,4,8,0,0,4)`.
- Full 7-layer planner implemented: generated effect attributes (refusing defaults), effect-set API, shared path-sensitive ownership, dataflow region discovery, DAG scheduler, capability tables (raw words live ONLY there, 490/0 round-trip checks), descriptor synthesis + adversarial verifier (11/0).
- **WP7 parity gate green and the Min/Max exact calendar DELETED:** 19 in-tree byte-parities; the mod-1/9 routing, SETC16 words, per-row calendar and break-evens all **derived**, no numeric threshold anywhere.
- **Honest catch by the CRAQ validation:** the planner initially *refused the real production kernel* (in-place-store rows — a shape no test covered; refusal was byte-identical so nothing unsound shipped). Fixed generically (`9609b7dd4`, store-demoted candidate fallback in the scheduling search, no address special-case): **8/8 CRAQ digests now EXACTLY match the frozen oracle** (bh `3418c903`/`2432a558`, wh `bb5ace1e`/`ff5b8d41`). Min/Max regression target **met by derivation**. 1431 rvtt passes, frozen 15-FAIL set throughout.
- **WP8 honestly deferred** (Signbit needs single-row + loop-preheader discovery beyond v1 scope); ordered 5-step worklist in `NOTES-wp0-20260816.md`. Min/Max silicon remains blocked until WP8/WP9 per the handoff rule.
- 7 constants flagged needing an independent architectural reference — most critical: **BH launch addr-mode shift `<<13` (frozen pass, adopted) vs `<<14` (sfpu-ops-bh.h, suspected stale)**.

## 4. Scout sweep (14 ops) — 2x2 coverage expansion

Changed-binary + CRAQ-green: **Expm1** (latency-schedule; measured 0.000%), **Mish/Selu/Cbrt** (replay-hoist record-only shapes — do-not-promote; the profitability gate now refuses this class). Byte-identical OFF/ON: quant/requant/dequant (QSR — consistent with QSR refusal), Erfinv (skip mystery solved: only Float32→Float32 builds on BH), SnakeBeta (recheck), Erf, Erfc, Softplus, I0. Gelu/GeluTanh/Tanh: CRAQ PASS/PASS, silicon correctly withheld (losing shape class). None of the swept eltwise ops has a distinct handwritten variant — their 2x2 collapses to semantic OFF/ON, like Lerp.

## 5. Carry-forwards / decisions needed from you

1. **Push queue (nothing pushed per protocol):** sdpa branch (7 commits, 3 reviews), replay-profit gate (needs review), macro-planner chain (WP0–WP7 + fix), typed-face-advance trio (3 repos, lockstep landing), plus the pre-existing `sdpa-exp-unclamped-win` branch still awaiting its rebase/push.
2. **Beat-hand path for SDPA (+83 to parity, then win):** M2b — typed effects for LLK setup/sync asm → invariant + SETC16 hoist above the face loop (M2a is implemented and waiting on it). Prediction optimistic; re-fit the delta model first (`residual-93-analysis.md` + m1m2a silicon actuals).
3. **Review carry-forwards:** M1 exit-block non-consumer exemption signed off as accepted-risk (revisit if call-saved LREG semantics ever appear); refuse launch-conversion across non-replay-neutral asm once typed effects land; unify M2a's loop-iteration estimate with the proven-trips discipline; SETC16-to-consume distance guard is conservative but needs hardware confirmation.
4. **Architectural reference needed** for the 7 flagged constants (BH shift conflict first).
5. Ops-level notes: session-limit outage at ~02:30 killed one wave (recovered fully after your re-login); scout toolchain driver is `df1a3ae5…`, same source stack but not byte-identical to the Lerp-reviewed `a6fe054d` binary — A/Bs internally valid, provenance note only; a leftover Tracy viewer (PID 3450946, port 8080) from the previous swarm is still running, harmless.

Authoritative live state: `/home/nkapre/HANDOFF.md`. Every claim above has a SHA256-manifested evidence directory under `/localdev/nkapre/`.
