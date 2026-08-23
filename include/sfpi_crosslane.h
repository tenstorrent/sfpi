/* -*- C++ -*-
 * SPDX-FileCopyrightText: © 2026 Tenstorrent Inc.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

// Typed cross-lane vocabulary (lane FA, 2026-08-21; design: lane EY-R
// CROSSLANE-DESIGN-INPUT.md; graduated from the lane EW/EX builtin-bridge
// kit, tt-blaze blaze/kernels/sfpu/semantic/sfpu_bridge.hpp).
//
// Not for standalone use: included from sfpi.h after sfpi_lib.h.
//
// == Lane geometry (the honest 2-D model) ==
//
// The 32 SFPU lanes are 4 subvector rows x 8 columns
// (tt-isa-documentation */TensixTile/TensixCoprocessor/VectorUnit.md).
// Two cluster sizes are first-class, mirroring SPIR-V ClusterSize / CUDA
// width: the hardware row of 8 (subvec_* ops, SFPSHFT2/SFPSWAP territory)
// and the full vector of 32 (reduce/broadcast compositions through
// SFPTRANSP).  The 4-row axis is exposed as its own dimension (RowPattern
// on sorts, transp8), never as a fake 4-lane cluster.
//
// == Type-level rules (each from a verified cross-ecosystem invariant) ==
//
//  1. Patterns, distances, and orders are TEMPLATE PARAMETERS, never
//     runtime values (LLVM shufflevector constant-mask rule; MLIR
//     attribute masks; ISPC uniform control operands; GCC VEC_PERM_EXPR
//     const selector) -- matching the hardware truth that every Tensix
//     cross-lane pattern is a fixed Mod1 encoding.  Runtime-pattern
//     requests refuse by name at compile time
//     (crosslane-dynamic-shuffle-unsupported): the hardware has no
//     vrgather/tbl-class instruction.
//  2. Sort order and key-value pairing are TRAITS of the compare-exchange
//     stage (VQSort Order/IsKV pattern), mapped to SFPSWAP operand roles
//     and the ENABLE_DEST_INDEX companion mechanism.  There is
//     deliberately NO sort primitive here (no production vector ISA or
//     library has one): networks are generated over sort2/sort2_rows/
//     sort2_kv + the fixed permutes (lane X5).
//  3. There is deliberately NO scan primitive (no ISA in the survey has
//     one; MLIR decomposes vector.scan unconditionally): scans decompose
//     into these ops at the call site or in a later lowering lane.
//  4. Every op's semantics are defined UNDER the current lane-enable
//     state (all lowered instructions are lanewise-predicated, per the
//     functional models); the CC window is part of the contract, exactly
//     like CUDA membermask / LLVM convergencectrl.
//
// == Lowering contracts and the X4 interception seam ==
//
// Every op below documents its exact lowering (instruction sequence +
// tt-isa-documentation cite).  Tier-1 ops are one builtin; composite ops
// (reduce/broadcast/zip/unzip/butterfly) are single always_inline frames
// whose bodies are the PINNED canonical sequences -- the future
// rvtt-crosslane-lower pass (lane X4) intercepts by recognizing these
// canonical forms (LLVM is*Mask lesson: recognizers over a small
// canonical form) and may re-lower them per-arch behind the same
// semantics.  Window-model constraint carried on every op that opens
// LaneConfig.ENABLE_DEST_INDEX (TEN-2932): while the bit is set only
// SFPLOAD/SFPLOADI/SFPSWAP/SFPTRANSP may write LReg[4..7]; the window
// open/close points lower to the DISTINGUISHABLE marker word
// SFPCONFIG 15, {4|0}, 1 (imm-form, __builtin_rvtt_sfpconfig_i) so X4
// can scope the window and keep allocator-inserted register moves out of
// it (lane EX's disasm gate caught a real in-window SFPMOV L5,L4
// violation -- that check is a per-kernel gate until X4 owns it).
//
// == Hazard facts (priced by the compiler, restated for humans) ==
//
// SFPSWAP and the SFPSHFT2 shuffle modes accept only SFPNOP on the next
// cycle (auto-stall: each costs 2 issue slots; inside SFPLOADMACRO
// sequences the stall is ABSENT and misuse is undefined behavior --
// SFPSWAP.md / SFPSHFT2.md CAUTION blocks).

#pragma once

#ifndef SFPI_CROSSLANE_FROM_SFPI_H
#error "include sfpi.h instead of sfpi_crosslane.h"
#endif

namespace sfpi {

//////////////////////////////////////////////////////////////////////////////
// Lane identity helpers (P17).
//
// vConstTileId holds 2*lane_id per lane (CREG_IDX_TILEID; lane-EX proof in
// tt-blaze softmax_k.hpp header).  These seed COMPUTED lane predicates,
// which replace lane-masked SFPCONFIG writes in the public surface (the
// lane-EX softmax_k dissolution precedent): a predicate from lane_col()
// plus v_if is the typed spelling of a per-column config mask.

sfpi_inline vInt lane_id ()
{
  return as<vInt> (as<vUInt> (vInt (vConstTileId)) >> 1);
}

// Column within the row of 8 (0..7).
sfpi_inline vInt lane_col ()
{
  return as<vInt> (as<vUInt> (vInt (vConstTileId)) >> 1 & 7u);
}

// Subvector row (0..3).
sfpi_inline vInt lane_row ()
{
  return as<vInt> (as<vUInt> (vInt (vConstTileId)) >> 4);
}

namespace impl_ {
template <typename> struct crosslane_dependent_false : public std::false_type {};

// Cross-lane data movement ops are bit-pattern transparent: any 32-bit
// element type rides along.
template <typename V>
constexpr bool is_crosslane_vec_v
  = std::disjunction<std::is_base_of<vFloat, V>, std::is_base_of<vInt, V>,
		     std::is_base_of<vUInt, V>, std::is_base_of<vSMag, V>>::value;

// SFPSWAP's comparison is the sign-magnitude total order; only vFloat and
// vSMag order correctly under it (same rule as sfpi::min_max).
template <typename V>
constexpr bool is_sortable_vec_v
  = std::disjunction<std::is_base_of<vFloat, V>, std::is_base_of<vSMag, V>>::value;

// Bit-pattern zero of any element type (slide fill value; 0.0f and integer
// 0 share the all-zero pattern).
template <typename V>
sfpi_inline V crosslane_zero ()
{
  if constexpr (std::is_base_of<vFloat, V>::value)
    return V (0.0f);
  else
    return V (0);
}
} // namespace impl_

//////////////////////////////////////////////////////////////////////////////
// Rotate / slide within the row of 8 (P3/P4).
//
// subvec_rotr<K>: every row independently rotates its 8 values toward
// higher columns by K: result[col] = v[(col - K) & 7].
// Lowering: K x SFPSHFT2 Mod1=3 SUBVEC_SHFLROR1 (SFPSHFT2.md; the ONLY
// intra-row rotation the hardware has is by one, so K rotations compose).
// Cost: 2K issue slots (next-slot stall per SFPSHFT2 shuffle).
// X4 seam: a chain of K SUBVEC_SHFLROR1 on one value is the canonical
// rotate form (ror1^8 == identity is already algebra the compiler knows
// how to hold: gimple-rvtt-transp-involution.cc precedent).

template <unsigned K, typename V>
sfpi_inline V subvec_rotr (const V &v)
{
  static_assert (impl_::is_crosslane_vec_v<V>,
		 "subvec_rotr: 32-bit sfpi vector types only");
  static_assert (K < 8, "subvec_rotr: rotate distance is mod 8");
  if constexpr (K == 0)
    return v;
  else
    return subvec_rotr<K - 1> (V (subvec_shflror1 (v)));
}

// In-place rotate-by-1 keeping destination == source (the live-value
// builtin ties them): the register shape of the hand kernels'
// SFPSHFT2(0, Ln, Ln, 3), and what keeps an 8-live-vector kernel inside
// the LREG file (the plain form needs a transient 9th register and the
// pressure check rightly refuses).  Graduated from lane EX's bridge kit.
template <typename V>
sfpi_inline void subvec_rotr1_ip (V &v)
{
  static_assert (impl_::is_crosslane_vec_v<V>,
		 "subvec_rotr1_ip: 32-bit sfpi vector types only");
  v = V (__builtin_rvtt_sfpshft2_subvec_shfl1_lv (v.get (), v.get (),
						  SFPSHFT2_MOD1_SUBVEC_SHFLROR1));
}

// subvec_slideup<K>: values move toward higher columns, zero-filling from
// column 0 (RVV vslideup direction): result[col] = col >= K ? v[col-K] : 0.
//
// Lowering contract is PER-ARCH (the first capability-table split):
//  - Blackhole/Quasar: K x SFPSHFT2 Mod1=4 SUBVEC_SHFLSHR1 (SFPSHFT2.md;
//    Blackhole fixed the Wormhole bug).  Cost 2K slots.
//  - Wormhole: SUBVEC_SHFLSHR1 is a documented hardware bug (WormholeB0
//    SFPSHFT2.md: lane 0 receives an UnpredictableValue; "should not be
//    used on Wormhole").  Lowered instead as subvec_rotr<K> plus a
//    lane-predicated zero of columns < K.  Cost 2K + ~4 slots.
// X4 seam: same canonical chains; the arch split moves into the pass's
// per-arch capability tables.

template <unsigned K, typename V>
sfpi_inline V subvec_slideup (const V &v)
{
  static_assert (impl_::is_crosslane_vec_v<V>,
		 "subvec_slideup: 32-bit sfpi vector types only");
  static_assert (K <= 8, "subvec_slideup: slide distance is at most the row");
  if constexpr (K == 8)
    return impl_::crosslane_zero<V> ();
  else if constexpr (K == 0)
    return v;
  else
    {
#if __riscv_xtttensixwh
      V r = subvec_rotr<K> (v);
      v_if (lane_col () < (int) K) {
	r = impl_::crosslane_zero<V> ();
      } v_endif;
      return r;
#else
      return subvec_slideup<K - 1> (V (subvec_shflshr1 (v)));
#endif
    }
}

// subvec_slidedown<K>: values move toward lower columns, zero-filling from
// column 7 (RVV vslidedown direction): result[col] = col < 8-K ? v[col+K] : 0.
// Lowering: subvec_rotr<8-K> (== rotate by -K) plus a lane-predicated zero
// of columns >= 8-K; the hardware has no left-shift form (SFPSHFT2.md has
// only ROR1/SHR1).  Cost 2*(8-K) + ~4 slots -- an honest DERIVED price;
// prefer subvec_rotr when wrap-in values are ignored.

template <unsigned K, typename V>
sfpi_inline V subvec_slidedown (const V &v)
{
  static_assert (impl_::is_crosslane_vec_v<V>,
		 "subvec_slidedown: 32-bit sfpi vector types only");
  static_assert (K <= 8, "subvec_slidedown: slide distance is at most the row");
  if constexpr (K == 8)
    return impl_::crosslane_zero<V> ();
  else if constexpr (K == 0)
    return v;
  else
    {
      V r = subvec_rotr<8 - K> (v);
      v_if (lane_col () >= (int) (8 - K)) {
	r = impl_::crosslane_zero<V> ();
      } v_endif;
      return r;
    }
}

//////////////////////////////////////////////////////////////////////////////
// Butterfly (XOR) exchange within the row of 8.
//
// butterfly_xor<K>: result[col] = v[col ^ K], K in {1, 2, 4} (the classic
// warp shfl.bfly / OpGroupNonUniformShuffleXor pattern, cluster 8).
// Composite K (3,5,6,7) decomposes into its bit factors.
//
// Lowering contract (all from SFPSHFT2.md ROR1 + lane predicates; the
// hardware has no xor-shuffle instruction):
//  - K=4: identical to subvec_rotr<4> (col^4 == (col+4) mod 8).  8 slots.
//  - K=2: select((col & 2) != 0, rotr<2>(v), rotr<6>(v)).  ~16 slots.
//  - K=1: select((col & 1) != 0, rotr<1>(v), rotr<7>(v)).  ~20 slots.
// DERIVED-EXPENSIVE: costs are honest v1 prices; X4 may re-lower (e.g.
// through SFPSHFT2 COPY4 queue forms) behind the same semantics.
// The payoff is the contract it enables: a butterfly fold tree produces
// BITWISE-IDENTICAL results in every lane even for non-associative FP ops
// (each step pairs lanes symmetrically), which is what reduce<Add> pins.

template <unsigned K, typename V>
sfpi_inline V butterfly_xor (const V &v)
{
  static_assert (impl_::is_crosslane_vec_v<V>,
		 "butterfly_xor: 32-bit sfpi vector types only");
  static_assert (K >= 1 && K < 8, "butterfly_xor: distance in 1..7");
  if constexpr (K == 4)
    return subvec_rotr<4> (v);
  else if constexpr (K == 2 || K == 1)
    {
      V hi = subvec_rotr<K> (v);
      V lo = subvec_rotr<8 - K> (v);
      V r = lo;
      v_if ((lane_col () & (int) K) != 0) {
	r = hi;
      } v_endif;
      return r;
    }
  else
    {
      // Composite distance: apply the bit factors (a fixed, commuting set).
      V r = v;
      if constexpr (K & 1)
	r = butterfly_xor<1> (r);
      if constexpr (K & 2)
	r = butterfly_xor<2> (r);
      if constexpr (K & 4)
	r = butterfly_xor<4> (r);
      return r;
    }
}

//////////////////////////////////////////////////////////////////////////////
// shuffle<P0..P7>: compile-time-pattern shuffle within each row of 8
// (the one portable spelling; GCC VEC_PERM_EXPR / LLVM shufflevector
// analogue at cluster 8).  result[col] = v[P_col].
//
// Lowering: a recognizer CHAIN, cheapest first (the universal backend
// architecture: aarch64 evpc chain, riscv shuffle_* chain) -- over the
// STRUCTURAL patterns the hardware has:
//   identity -> nothing
//   broadcast (all P equal) -> subvec_broadcast<P0>
//   rotation (P_col == (col-K) & 7) -> subvec_rotr<K>
//   xor (P_col == col ^ K) -> butterfly_xor<K>
// Everything else refuses BY NAME at compile time: the hardware has no
// general permute (no vrgather/tbl class instruction; SFPSHFT2.md /
// SFPTRANSP.md patterns are fixed by Mod1).  This refusal is the honest
// spelling of gap P14, not a missing feature of this header.
//
// Dynamic (runtime) patterns refuse by name via the overload below.

template <unsigned Col, typename V>
sfpi_inline V subvec_broadcast (const V &v);

namespace impl_ {
constexpr bool crosslane_perm_is_identity (unsigned p0, unsigned p1, unsigned p2,
					   unsigned p3, unsigned p4, unsigned p5,
					   unsigned p6, unsigned p7)
{
  return p0 == 0 && p1 == 1 && p2 == 2 && p3 == 3
    && p4 == 4 && p5 == 5 && p6 == 6 && p7 == 7;
}
constexpr bool crosslane_perm_is_broadcast (unsigned p0, unsigned p1, unsigned p2,
					    unsigned p3, unsigned p4, unsigned p5,
					    unsigned p6, unsigned p7)
{
  return p1 == p0 && p2 == p0 && p3 == p0 && p4 == p0 && p5 == p0
    && p6 == p0 && p7 == p0 && p0 < 8;
}
constexpr int crosslane_perm_rot (unsigned p0, unsigned p1, unsigned p2,
				  unsigned p3, unsigned p4, unsigned p5,
				  unsigned p6, unsigned p7)
{
  // result[col] = v[(col - K) & 7]  ->  P_col == (col - K) & 7.
  unsigned k = (0 - p0) & 7;
  unsigned p[8] = { p0, p1, p2, p3, p4, p5, p6, p7 };
  for (unsigned col = 0; col < 8; col++)
    if (p[col] != ((col - k) & 7))
      return -1;
  return (int) k;
}
constexpr int crosslane_perm_xor (unsigned p0, unsigned p1, unsigned p2,
				  unsigned p3, unsigned p4, unsigned p5,
				  unsigned p6, unsigned p7)
{
  unsigned k = p0;			// col 0: P_0 == 0 ^ K
  unsigned p[8] = { p0, p1, p2, p3, p4, p5, p6, p7 };
  if (k == 0 || k >= 8)
    return -1;
  for (unsigned col = 0; col < 8; col++)
    if (p[col] != (col ^ k))
      return -1;
  return (int) k;
}
} // namespace impl_

template <unsigned P0, unsigned P1, unsigned P2, unsigned P3,
	  unsigned P4, unsigned P5, unsigned P6, unsigned P7, typename V>
sfpi_inline V shuffle (const V &v)
{
  static_assert (impl_::is_crosslane_vec_v<V>,
		 "shuffle: 32-bit sfpi vector types only");
  static_assert (P0 < 8 && P1 < 8 && P2 < 8 && P3 < 8
		 && P4 < 8 && P5 < 8 && P6 < 8 && P7 < 8,
		 "shuffle: pattern indices are columns 0..7");
  if constexpr (impl_::crosslane_perm_is_identity (P0, P1, P2, P3, P4, P5, P6, P7))
    return v;
  else if constexpr (impl_::crosslane_perm_is_broadcast (P0, P1, P2, P3, P4, P5, P6, P7))
    return subvec_broadcast<P0> (v);
  else if constexpr (impl_::crosslane_perm_rot (P0, P1, P2, P3, P4, P5, P6, P7) >= 0)
    return subvec_rotr<(unsigned) impl_::crosslane_perm_rot (P0, P1, P2, P3,
							     P4, P5, P6, P7)> (v);
  else if constexpr (impl_::crosslane_perm_xor (P0, P1, P2, P3, P4, P5, P6, P7) >= 0)
    return butterfly_xor<(unsigned) impl_::crosslane_perm_xor (P0, P1, P2, P3,
							       P4, P5, P6, P7)> (v);
  else
    static_assert (impl_::crosslane_dependent_false<V>::value,
		   "crosslane-general-permute-unsupported: Tensix has no general "
		   "per-lane permute; only identity/broadcast/rotation/xor row "
		   "patterns lower (SFPSHFT2.md/SFPTRANSP.md: patterns are fixed "
		   "by Mod1)");
}

// Named compile-time refusal of dynamic shuffles (the P14 hardware gap:
// no vrgather/TableLookupLanes/tbl equivalent exists).  Row-granular
// dynamic movement goes through Dst instead (store + reload at computed
// row addresses).
template <typename V, typename Index>
sfpi_inline V shuffle (const V &, const Index &)
{
  static_assert (impl_::crosslane_dependent_false<V>::value,
		 "crosslane-dynamic-shuffle-unsupported: Tensix has no dynamic "
		 "per-lane permute instruction; shuffle patterns must be "
		 "compile-time template parameters "
		 "(sfpi::shuffle<P0..P7>(v))");
}

//////////////////////////////////////////////////////////////////////////////
// Broadcast (P1).
//
// subvec_broadcast<Col>: within each row of 8, every column receives that
// row's value at column Col.  result[col] = v[Col] (per row).
// Lowering (DERIVED, log-fill): d = (col - Col) & 7 computed from
// lane_col(); then three predicated rotate-merge steps (distances 1, 2, 4)
// fill the row: after step k, columns at distance < 2^k hold the value.
// ~25 slots.  NOTE: a reduction fold already leaves its result in ALL
// lanes (see reduce<Op>) -- reach for broadcast only when a specific
// column's value is wanted.
// X4 seam: the canonical form is this exact rotate/merge ladder from one
// inline frame; X4 may substitute the SFPCONFIG vertical-broadcast path
// (SFPCONFIG.md: input from the first 8 lanes broadcast to 32) where a
// programmable constant register is free.

template <unsigned Col, typename V>
sfpi_inline V subvec_broadcast (const V &v)
{
  static_assert (impl_::is_crosslane_vec_v<V>,
		 "subvec_broadcast: 32-bit sfpi vector types only");
  static_assert (Col < 8, "subvec_broadcast: source column is 0..7");
  vInt d = (lane_col () - (int) Col) & 7;
  V r = v;
  V t = subvec_rotr<1> (r);
  v_if (d == 1) {
    r = t;
  } v_endif;
  t = subvec_rotr<2> (r);
  v_if (d == 2 || d == 3) {
    r = t;
  } v_endif;
  t = subvec_rotr<4> (r);
  v_if (d >= 4) {
    r = t;
  } v_endif;
  return r;
}

//////////////////////////////////////////////////////////////////////////////
// Compare-exchange: the sort-network building block (P7/P8/P9).
//
// NO sort or top-k primitive exists here, by design: every production
// system builds sorting from exactly {compare-exchange, fixed permute,
// odd/even blend} (VQSort, Bramas, CUB; the one hardware sort engine
// found, IBM z15 SORTL, is a memory-to-memory coprocessor, not a lane
// primitive).  Networks are generated over these stages in lane X5.
//
// ORDER is a trait mapped to SFPSWAP OPERAND ROLES (VQSort
// OrderAscending/OrderDescending -> First/Last): under Mod1=1 the VD
// operand receives min and VC receives max (SFPSWAP.md), so Descending
// simply swaps the roles.  Mod1=9 (VD=max) is deliberately not used: the
// compiler refuses it (no architectural enum; silicon behavior above
// mod 9 is non-contractual).  Per-column direction flips keep their
// hand-kernel spelling (lane-masked SFPCONFIG with the value staged in
// LReg[0]) until X4/X5 own them.
//
// DOCUMENTED COMPARISON CONTRACT (part of the op's semantics, not a
// footnote): SFPSWAP orders by the sign-magnitude total order
// -NaN < -Inf < ... < -0 < +0 < ... < +Inf < +NaN, and equal values are
// not always left alone: max lanes swap equal positive values, min lanes
// swap equal negative values (SFPSWAP.md).  Types: vFloat/vSMag only
// (same rule as sfpi::min_max; two's-complement vInt/vUInt order
// incorrectly under sign-magnitude comparison).
//
// TIE CONTRACT CAVEAT (lane FB finding, 2026-08-21): the documented
// equal-value swap rule above DISAGREES with the pinned simulator's
// behavior, observably via ENABLE_DEST_INDEX argmin/argmax (which of two
// equal keys' companions is selected).  Tie behavior is UNADJUDICATED
// pending a silicon probe: the value results of sort2/sort2_rows are
// unaffected (equal values swap to equal values), but sort2_kv's
// companion selection between EQUAL keys must not be relied on --
// keep fixtures tie-free (the FB arsenal oracle carries tie="doc"|"sim"
// dual models until silicon decides).
//
// Cost: 2 issue slots each (next-slot stall).

enum class SortOrder
{
  Ascending,	// a <- min, b <- max
  Descending,	// a <- max, b <- min
};

template <SortOrder Order, typename V>
sfpi_inline void sort2 (V &a, V &b)
{
  static_assert (impl_::is_sortable_vec_v<V>,
		 "sort2: SFPSWAP's sign-magnitude total order sorts vFloat/vSMag "
		 "only (wrap vInt via sfpi::as<vSMag> conversions)");
  if constexpr (Order == SortOrder::Ascending)
    {
      auto r = __builtin_rvtt_sfpswap (a.get (), b.get (), SFPSWAP_MOD1_VEC_MIN_MAX);
      a = V (__builtin_rvtt_sfpselect2 (r, 0));
      b = V (__builtin_rvtt_sfpselect2 (r, 1));
    }
  else
    {
      auto r = __builtin_rvtt_sfpswap (b.get (), a.get (), SFPSWAP_MOD1_VEC_MIN_MAX);
      b = V (__builtin_rvtt_sfpselect2 (r, 0));
      a = V (__builtin_rvtt_sfpselect2 (r, 1));
    }
}

// Per-row-group direction: the bitonic inner stage (P8).  One SFPSWAP
// executes a DIFFERENT min/max direction per subvector row (SFPSWAP.md
// Mod1=2..8 VDGetsMin masks).  RowPattern names which rows give `a' the
// minimum (the rest give `a' the maximum).
enum class RowPattern : unsigned
{
  MinAll      = SFPSWAP_MOD1_VEC_MIN_MAX,	// rows 0123: a <- min
  Min01Max23  = SFPSWAP_MOD1_SUBVEC_MIN01_MAX23,
  Min02Max13  = SFPSWAP_MOD1_SUBVEC_MIN02_MAX13,
  Min03Max12  = SFPSWAP_MOD1_SUBVEC_MIN03_MAX12,
  Min0Max123  = SFPSWAP_MOD1_SUBVEC_MIN0_MAX123,
  Min1Max023  = SFPSWAP_MOD1_SUBVEC_MIN1_MAX023,
  Min2Max013  = SFPSWAP_MOD1_SUBVEC_MIN2_MAX013,
  Min3Max012  = SFPSWAP_MOD1_SUBVEC_MIN3_MAX012,
};

template <RowPattern Pattern, typename V>
sfpi_inline void sort2_rows (V &a, V &b)
{
  static_assert (impl_::is_sortable_vec_v<V>,
		 "sort2_rows: SFPSWAP's sign-magnitude total order sorts "
		 "vFloat/vSMag only");
  auto r = __builtin_rvtt_sfpswap (a.get (), b.get (), (unsigned) Pattern);
  a = V (__builtin_rvtt_sfpselect2 (r, 0));
  b = V (__builtin_rvtt_sfpselect2 (r, 1));
}

//////////////////////////////////////////////////////////////////////////////
// Key-value compare-exchange (P9) and its configuration window.
//
// set_dest_index_window<Enable> toggles LaneConfig.ENABLE_DEST_INDEX
// (bit 2).  Lowering: the imm-form SFPCONFIG builtin
// __builtin_rvtt_sfpconfig_i(0x4|0x0, 15, 1) -- byte-identical to the
// hand kernels' TTI_SFPCONFIG(0x4|0x0, 0xF, 1) words and REGISTER-FREE
// (the value-form builtin stages through LReg[0]: a 9th live vector,
// pressure-infeasible exactly where the hand kernels toggle the window --
// lane EX's compile-proven lreg-pressure-exceeded refusals).  On a
// toolchain without the builtin the value form is the fallback, with its
// pressure cost.
//
// WINDOW CONTRACT (TEN-2932, Wormhole/Blackhole erratum; SFPCONFIG.md
// LaneConfig table): while ENABLE_DEST_INDEX is set, instructions other
// than SFPLOAD/SFPLOADI/SFPSWAP/SFPTRANSP that write LReg[4..7] are
// UnsupportedFunctionality.  Callers keep window content to sort2_kv /
// transp8 / loads; until the X4 window model lands, kernels using this
// gate their emitted window by disassembly inspection (bridge-kit
// discipline; EX caught a real allocator-inserted SFPMOV L5,L4 inside a
// window).  The imm-form open/close words are the DISTINGUISHABLE MARKERS
// X4 scopes the window by.

template <bool Enable>
sfpi_inline void set_dest_index_window ()
{
#if defined (__has_builtin) && __has_builtin (__builtin_rvtt_sfpconfig_i)
  __builtin_rvtt_sfpconfig_i (Enable ? 0x4 : 0x0, SFPCONFIG_DEST_SFPU_CTRL, 1);
#else
  vInt cfg = Enable ? 4 : 0;
  __builtin_rvtt_sfpwriteconfig_v (cfg.get (), SFPCONFIG_DEST_SFPU_CTRL);
#endif
}

// sort2_kv<Order>: ONE SFPSWAP under an open ENABLE_DEST_INDEX window
// compares the keys AND swaps the companion payloads on the same per-lane
// decision (SFPSWAP.md ENABLE_DEST_INDEX leg) -- key+payload
// compare-exchange in one instruction (the argsort/top-k stage).  The
// compiler's register alternatives pin companion == value + 4
// (rvtt_sfpswap_indexed), so the emitted word is exactly the hand
// kernels' TTI_SFPSWAP(0, ...).  Caller MUST hold the window open
// (set_dest_index_window<true>()); see the window contract above.
// Graduated from lane EX's indexed_swap.

template <SortOrder Order>
sfpi_inline void sort2_kv (vFloat &ka, vFloat &kb, vUInt &pa, vUInt &pb)
{
  if constexpr (Order == SortOrder::Ascending)
    {
      auto r = __builtin_rvtt_sfpswap_indexed (ka.get (), kb.get (),
					       pa.get (), pb.get (),
					       SFPSWAP_MOD1_VEC_MIN_MAX);
      ka = vFloat (__builtin_rvtt_sfpselect4 (r, 0));
      kb = vFloat (__builtin_rvtt_sfpselect4 (r, 1));
      pa = vUInt (__builtin_rvtt_sfpselect4 (r, 2));
      pb = vUInt (__builtin_rvtt_sfpselect4 (r, 3));
    }
  else
    {
      auto r = __builtin_rvtt_sfpswap_indexed (kb.get (), ka.get (),
					       pb.get (), pa.get (),
					       SFPSWAP_MOD1_VEC_MIN_MAX);
      kb = vFloat (__builtin_rvtt_sfpselect4 (r, 0));
      ka = vFloat (__builtin_rvtt_sfpselect4 (r, 1));
      pb = vUInt (__builtin_rvtt_sfpselect4 (r, 2));
      pa = vUInt (__builtin_rvtt_sfpselect4 (r, 3));
    }
}

// Row-group-direction key-value stage (bitonic inner stages over KV data).
template <RowPattern Pattern>
sfpi_inline void sort2_kv_rows (vFloat &ka, vFloat &kb, vUInt &pa, vUInt &pb)
{
  auto r = __builtin_rvtt_sfpswap_indexed (ka.get (), kb.get (),
					   pa.get (), pb.get (),
					   (unsigned) Pattern);
  ka = vFloat (__builtin_rvtt_sfpselect4 (r, 0));
  kb = vFloat (__builtin_rvtt_sfpselect4 (r, 1));
  pa = vUInt (__builtin_rvtt_sfpselect4 (r, 2));
  pb = vUInt (__builtin_rvtt_sfpselect4 (r, 3));
}

//////////////////////////////////////////////////////////////////////////////
// Transpose (P5).
//
// transp8: THE hardware inter-row primitive.  One SFPTRANSP stacks four
// registers as a (register x row) 4x4 matrix per column and transposes
// it -- on BOTH banks at once (SFPTRANSP.md: the instruction always
// permutes LReg[0..3] AND LReg[4..7]).  This 8-operand form is the
// AUDITED one: its RTL PARALLEL carries the complete architectural write
// set, so the allocator sees the companion-bank definitions (the legacy
// 4-operand sfpi::subvec_transp deliberately keeps refusing effect
// defaults because it under-states the write set -- see rvtt.md; prefer
// transp8 in new code).  Graduated from lane EX's bridge kit.
// Cost: 1 slot (+ fixed-LReg companion reads).
// TEN-2932: SFPTRANSP is one of the four opcodes ALLOWED to write
// LReg[4..7] inside an open ENABLE_DEST_INDEX window.

sfpi_inline void transp8 (vFloat &v0, vFloat &v1, vFloat &v2, vFloat &v3,
			  vUInt &c0, vUInt &c1, vUInt &c2, vUInt &c3)
{
  auto r = __builtin_rvtt_sfptransp8 (v0.get (), v1.get (), v2.get (), v3.get (),
				      c0.get (), c1.get (), c2.get (), c3.get ());
  v0 = vFloat (__builtin_rvtt_sfpselect4 (r, 0));
  v1 = vFloat (__builtin_rvtt_sfpselect4 (r, 1));
  v2 = vFloat (__builtin_rvtt_sfpselect4 (r, 2));
  v3 = vFloat (__builtin_rvtt_sfpselect4 (r, 3));
  c0 = vUInt (l_reg[LRegs::LReg4]);
  c1 = vUInt (l_reg[LRegs::LReg5]);
  c2 = vUInt (l_reg[LRegs::LReg6]);
  c3 = vUInt (l_reg[LRegs::LReg7]);
}

sfpi_inline void transp8 (vUInt &v0, vUInt &v1, vUInt &v2, vUInt &v3,
			  vUInt &c0, vUInt &c1, vUInt &c2, vUInt &c3)
{
  auto r = __builtin_rvtt_sfptransp8 (v0.get (), v1.get (), v2.get (), v3.get (),
				      c0.get (), c1.get (), c2.get (), c3.get ());
  v0 = vUInt (__builtin_rvtt_sfpselect4 (r, 0));
  v1 = vUInt (__builtin_rvtt_sfpselect4 (r, 1));
  v2 = vUInt (__builtin_rvtt_sfpselect4 (r, 2));
  v3 = vUInt (__builtin_rvtt_sfpselect4 (r, 3));
  c0 = vUInt (l_reg[LRegs::LReg4]);
  c1 = vUInt (l_reg[LRegs::LReg5]);
  c2 = vUInt (l_reg[LRegs::LReg6]);
  c3 = vUInt (l_reg[LRegs::LReg7]);
}

//////////////////////////////////////////////////////////////////////////////
// Zip / unzip across the row axis (P6).
//
// rowvec_zip(a, b): interleave the subvector rows of two vectors.
// Viewing (a0..a3, b0..b3) as an 8-row sequence, on return
// a = (a0, b0, a1, b1) and b = (a2, b2, a3, b3).
// Lowering (DERIVED; SFPTRANSP.md + SFPSWAP.md Mod1=0 lanewise
// unconditional swap under lane predication -- the "SFPTRANSP + SFPMOV/
// SFPSWAP mod-0 composition" of the design input):
//   stage the bank as (a, b, a, b)          [2 SFPMOV]
//   SFPTRANSP: T_i = (a_i, b_i, a_i, b_i)   [1]
//   rows {2,3} predicated swap T0<->T1 and T2<->T3
//     -> T0 = (a0, b0, a1, b1), T2 = (a2, b2, a3, b3)
// ~14 slots + 4 companion zero-inits (the audited dual-bank transpose
// requires defined companions; the second bank could zip another pair for
// free -- an X4/X5 improvement).
//
// rowvec_unzip(a, b): the inverse (deinterleave).  On return
// a = (a0, a2, b0, b2), b = (a1, a3, b1, b3) (even rows then odd rows of
// the 8-row sequence).  Lowering: zip APPLIED TWICE -- the 8-element
// out-shuffle (riffle) has order 3 (2^3 == 1 mod 7), so unzip == zip o zip.
// ~2x the zip cost (2 SFPTRANSP + staging), DERIVED-EXPENSIVE but
// allocator-clean.  (A single-extra-transpose spelling exists on paper --
// evens quad in the value bank, odds quad in the companion bank of one
// dual-bank SFPTRANSP -- but its duplicate-source staging trips the known
// IRA dual-bank coloring gap, lane EX's repro-top16-ira: colorable 8-live
// graphs spill when exact-register x0-x7 quartet webs chain through free
// copy webs.  Re-lower through that spelling when the allocator fix
// lands.)
//
// Both verified against the SFPTRANSP.md functional model by hand
// (T_i row_j == input_j row_i) -- the X3 permutation-identity harness
// re-proves them on the pinned sims with lane-distinguishing seeds.

sfpi_inline void rowvec_zip (vFloat &a, vFloat &b)
{
  vFloat v0 = a, v1 = b, v2 = a, v3 = b;
  vUInt c0 = 0, c1 = 0, c2 = 0, c3 = 0;
  transp8 (v0, v1, v2, v3, c0, c1, c2, c3);
  v_if (lane_row () >= 2) {
    swap (v0, v1);
    swap (v2, v3);
  } v_endif;
  a = v0;
  b = v2;
}

sfpi_inline void rowvec_unzip (vFloat &a, vFloat &b)
{
  rowvec_zip (a, b);
  rowvec_zip (a, b);
}

//////////////////////////////////////////////////////////////////////////////
// Reduce-across (P10/P11): mission name reduce_across == these.
//
// subvec_reduce<Op>(v): every lane receives the reduction of its row of 8.
// reduce<Op>(v): every lane receives the reduction of all 32 lanes.
// Result-in-all-lanes is the GPU convention (HLSL wave ops "broadcast the
// final result to all active lanes"); no separate broadcast needed.
//
// EXPLICIT REASSOCIATION CONTRACT (the reduce invariant; llvm.vector.
// reduce.* reassoc-vs-ordered precedent).  The fold TREE is part of the
// semantics:
//  - Max/Min are total-order idempotent-associative under SFPSWAP's
//    sign-magnitude order: ANY tree yields the identical bit pattern, and
//    the cheap rotation ladder is used: a = op(a, rotr<1>(a)); a = op(a,
//    rotr<2>(a)); a = op(a, rotr<4>(a)).  7 SFPSHFT2 + 3 ops (~20 slots).
//    All 8 (32) lanes hold the bit-identical result.
//  - Add (FP) is NOT associative, so the tree is pinned to the BUTTERFLY
//    ladder s = s + butterfly_xor<1>(s); s = s + butterfly_xor<2>(s);
//    s = s + butterfly_xor<4>(s): every step pairs lanes symmetrically,
//    which makes all lanes bitwise identical (a rotation ladder would
//    give each lane a differently-associated sum).  The per-lane value is
//    the fixed pairwise tree ((v_j+v_{j^1}) + (v_{j^2}+v_{j^3})) +
//    ((v_{j^4}+v_{j^5}) + (v_{j^6}+v_{j^7})) -- same shape as LLVM's
//    guaranteed log2 shuffle expansion (getShuffleReduction SplitHalf).
//    ~55 slots: the honest v1 price of the bit-stable contract; callers
//    licensed to reassociate can build the cheap rotation ladder from
//    subvec_rotr directly.
//  - Cross-row step (cluster 32): transp8(a, a, a, a) makes T_i =
//    row-i-of-a broadcast to all rows (SFPTRANSP.md functional model with
//    four equal inputs); the pinned combine is op(op(T0, T1), op(T2, T3)).
//    Exact for Max/Min; for Add all lanes stay identical by construction
//    (each T_i is row-uniform per column).
//
// X4 seam: each reduce is ONE inline frame whose body is the pinned
// canonical ladder; the structured-op interception point
// (.RVTT_CROSSLANE_REDUCE class) replaces these bodies in lane X4.

enum class ReduceOp
{
  Max,
  Min,
  Add,
};

namespace impl_ {
template <ReduceOp Op>
sfpi_inline vFloat crosslane_apply (const vFloat &x, const vFloat &y)
{
  if constexpr (Op == ReduceOp::Max)
    return max (x, y);
  else if constexpr (Op == ReduceOp::Min)
    return min (x, y);
  else
    return x + y;
}
} // namespace impl_

template <ReduceOp Op>
sfpi_inline vFloat subvec_reduce (const vFloat &v)
{
  vFloat a = v;
  if constexpr (Op == ReduceOp::Add)
    {
      a = impl_::crosslane_apply<Op> (a, butterfly_xor<1> (a));
      a = impl_::crosslane_apply<Op> (a, butterfly_xor<2> (a));
      a = impl_::crosslane_apply<Op> (a, butterfly_xor<4> (a));
    }
  else
    {
      a = impl_::crosslane_apply<Op> (a, subvec_rotr<1> (a));
      a = impl_::crosslane_apply<Op> (a, subvec_rotr<2> (a));
      a = impl_::crosslane_apply<Op> (a, subvec_rotr<4> (a));
    }
  return a;
}

template <ReduceOp Op>
sfpi_inline vFloat reduce (const vFloat &v)
{
  vFloat a = subvec_reduce<Op> (v);
  // Cross-row: four equal inputs make T_i the broadcast of row i.
  vFloat t0 = a, t1 = a, t2 = a, t3 = a;
  vUInt c0 = 0, c1 = 0, c2 = 0, c3 = 0;
  transp8 (t0, t1, t2, t3, c0, c1, c2, c3);
  return impl_::crosslane_apply<Op> (impl_::crosslane_apply<Op> (t0, t1),
				     impl_::crosslane_apply<Op> (t2, t3));
}

// Full-vector broadcast: every one of the 32 lanes receives the value at
// (row Idx/8, column Idx%8).  Lowering: subvec_broadcast within rows,
// then the transp8 row-broadcast trick selects the source row.  ~35
// slots, DERIVED-EXPENSIVE.
template <unsigned Idx>
sfpi_inline vFloat broadcast_lane (const vFloat &v)
{
  static_assert (Idx < 32, "broadcast_lane: lane index is 0..31");
  vFloat b = subvec_broadcast<Idx & 7> (v);
  vFloat t0 = b, t1 = b, t2 = b, t3 = b;
  vUInt c0 = 0, c1 = 0, c2 = 0, c3 = 0;
  transp8 (t0, t1, t2, t3, c0, c1, c2, c3);
  if constexpr ((Idx >> 3) == 0)
    return t0;
  else if constexpr ((Idx >> 3) == 1)
    return t1;
  else if constexpr ((Idx >> 3) == 2)
    return t2;
  else
    return t3;
}

//////////////////////////////////////////////////////////////////////////////
// Packed-halfword Dst access (P19).  Graduated from lane EX's bridge kit
// (load_companion/store_companion/store_uint16): the top-k kernels pack
// (index LO16 | score HI16) into one register with MERGING partial loads.
// The merge is PRESSURE-MANDATORY, not style: a zero-fill + OR spelling
// needs a 9th live vector (compile-proven lreg-pressure-exceeded).
//
// Address units are raw SFPLOAD/SFPSTORE addresses: dst_reg[i] == address
// 2*i (SFP_DESTREG_STRIDE).  Semantics (SFPLOAD.md/SFPSTORE.md, WH+BH):
//   load  LO16_ONLY (14): LReg = (old & 0xffff0000) | Dst16b  [merging]
//   load  HI16_ONLY (15): LReg = (Dst16b << 16) | (old & 0xffff)
//   store LO16_ONLY (14): Dst16b = Datum & 0xffff
//   store HI16_ONLY (15): Dst16b = Datum >> 16
//   store UINT16     (6): Dst16b = Datum & 0xffff
// The first (LO16_ONLY) load of dst_load_packed leaves the high half at
// the stale register value, exactly as the hand kernels' first load into
// a stale LREG does; the second (HI16_ONLY) load defines it.
// TEN-2932: SFPLOAD/SFPLOADI are window-exempt opcodes.
//
// LOW-HALF CONTRACT CAVEAT (lane FB finding, 2026-08-21): for 16b Dst
// datums the PAIRED LOW HALF diverges three ways -- the ISA doc says
// preserve, the pinned sim zeroes, and silicon canonicalizes BF16 on
// FP16B RMW stores (tt-blaze #2475).  These helpers are verified only in
// FB's Adj16 fp32-accumulate Dst configuration (32-bit datums, no paired
// half); do not lower onto 16b partial Dst modes outside that config
// without adjudicating the low-half behavior.

sfpi_inline vUInt dst_load_packed (unsigned lo16_addr, unsigned hi16_addr)
{
  vUInt c { __builtin_rvtt_sfpload (lo16_addr, SFPLOAD_MOD0_FMT_LO16_ONLY,
				    SFPLOAD_ADDR_MODE_NOINC) };
  c = vUInt (__builtin_rvtt_sfpload_lv (ckernel::instrn_buffer, c.get (),
					hi16_addr, 0, 0,
					SFPLOAD_MOD0_FMT_HI16_ONLY,
					SFPLOAD_ADDR_MODE_NOINC));
  return c;
}

sfpi_inline void dst_store_packed (const vUInt &c,
				   unsigned lo16_addr, unsigned hi16_addr)
{
  __builtin_rvtt_sfpstore (c.get (), lo16_addr, SFPSTORE_MOD0_FMT_LO16_ONLY,
			   SFPSTORE_ADDR_MODE_NOINC);
  __builtin_rvtt_sfpstore (c.get (), hi16_addr, SFPSTORE_MOD0_FMT_HI16_ONLY,
			   SFPSTORE_ADDR_MODE_NOINC);
}

sfpi_inline void dst_store_uint16 (const vUInt &v, unsigned addr)
{
  __builtin_rvtt_sfpstore (v.get (), addr, SFPSTORE_MOD0_FMT_UINT16,
			   SFPSTORE_ADDR_MODE_NOINC);
}

//////////////////////////////////////////////////////////////////////////////
// X6: FPU FACE TRANSPOSE (Dst 16x16 faces, 32-bit datums) -- Blackhole.
//
// The SFPU shuffle vocabulary above (X1..X5) permutes LANES within the
// 4x8 vector geometry; transposing a 16x16 FACE of Dst rows is Matrix-Unit
// (FPU) territory: MOVD2B parks Dst rows in SrcB[16:32), TRNSPSRCB
// transposes that 16x16 block in place, MOVB2A/MOVB2D/MOVA2D move the
// transposed rows back.  This section is the typed spelling of the hand
// choreography in blaze ckernel_sfpu_topk_xl.h transpose_dest_face_32b<>
// (straight-line, MOP-free, replay-free; the in-tree LLK
// llk_math_transpose_dest.h is the same instruction algebra driven by
// MOP + replay + its own ADDR_MOD program -- an LLK-API shape, NOT safe
// inside an SFPU kernel; this surface is).
//
// Semantics source: tt-isa-documentation WormholeB0
// {MOVD2B,MOVB2A,MOVB2D,MOVA2D,TRNSPSRCB,RMWCIB}.md functional models
// (they carry the Blackhole arms; the BlackholeA0 tree is a doc gap and
// the pinned sim is the BH oracle -- lane FV X6-RESEARCH.md).
//
// CONTRACT (all bit-exact for arbitrary 32-bit Dst datums -- fp32 values
// and int32/packed indices alike):
//   face_transpose_dst_32b<FaceRow>():
//     Dst32b[FaceRow + i][j] <- Dst32b[FaceRow + j][i]   (i, j in 0..15)
// under a caller-opened face_transpose_cfg_enter()/_leave() block, in a
// 32-bit Dst configuration (ALU_ACC_CTRL_Fp32_enabled == 1 on entry --
// the dest-accumulate SFPU kernel state), under the standard SFPU LLK
// math state where instruction address-mod 7 applies ZERO increments
// (cmath_common SFPU program; every move below rides mod 7 so no RWC
// drifts), with the SrcA AND SrcB banks owned by the math thread
// (AllowedClient == MatrixUnit).  Bank ownership is CROSS-THREAD state:
// the unpack thread grants it once per math epoch
// (_llk_unpack_set_srcb_dummy_valid_ after its SrcA feeds -- the
// transpose_dest_test.cpp protocol), and the math thread returns the
// banks when its section completes (ttsetrwc CLR_AB; see
// face_transpose_release_banks()).  The KV-window precedent: the caller
// owns the window; this surface owns the words inside it.
//
// WHY THE THREE-PASS FORMAT DANCE IS EXACT (X6-RESEARCH.md section 4):
// lo16 halves ride ShuffleBF16 (injective, inverted by the Float32-format
// MOVA2D write-back into Dst bits 15..0); hi16 halves ride ShuffleTF32
// (bits 31..13 preserved; MOVB2D under SrcA-format Tf32 reconstructs bits
// 31..16 and zeroes 15..0, which the lo16 pass then rewrites).
// ALU_ACC_CTRL_Zero_Flag_disabled_src MUST be 1 across the block: the
// SrcB->SrcA and SrcB->Dst moves flush any datum whose shuffled image has
// a zero low byte (MOVB2A.md FlushDenormals) -- silent data loss on
// ~1/256 of bit patterns without it.  DISABLE_IMPLIED_SRCA_FMT MUST be 1:
// on Blackhole the implied-format path reads ImpliedSrcBFmt of a
// never-unpacked bank, NonContractualBehavior per MOVD2B.md.
// SRCB-FORMAT EDGE (host-proven, X6 oracle theorem): MOVB2D's masking
// arm keys on the effective SrcBFmt, which this block does NOT own; the
// composition is exact for every 8b-exponent-class SrcBFmt (the &0x7F8FF
// mask only drops bits pass 3 rewrites) but an FP16-class SrcBFmt would
// mask the relocated exponent bits and corrupt Dst bits 31..16.  The
// hand kernels carry the same reliance on the ambient non-FP16 ALU
// state; kernels running FP16-class ALU B formats must not call this.
//
// NAMED REFUSALS:
//   crosslane-facetranspose-unsupported-target: only the Blackhole
//     choreography is proven (WH lacks the implied-format contract audit
//     and a hand vehicle; QSR encodes the family differently -- the
//     builtins themselves refuse there too).
//   crosslane-facetranspose-row-unaligned: FaceRow must be a multiple of
//     16 (faces), within Dst (FaceRow + 16 <= 1024).
//   crosslane-facetranspose-16b-unproven: 16-bit Dst datum faces (the
//     deepseek single-face MOP variants) are NOT provided -- their MOP/
//     replay scheduling and even/odd Dst16b row interleave are a separate
//     audit.  32-bit only.
//
// HAZARD LEDGER (X6-RESEARCH.md section 6): enter() issues
// TTSTALLWAIT(STALL_CFG, WAIT_SFPU|SRCA_VLD|SRCB_VLD) so the first config
// write -- and everything after it in the in-order math stream -- waits
// for SFPU drain and bank ownership.  The FPU's own read-after-MOV
// windows are hardware-interlocked between FPU instructions (MOVD2B.md /
// MOVB2D.md scheduling notes); SFPU loads issued right after the
// choreography follow the hand kernels' proven pattern (topk_xl phase 6+)
// and are re-proven by the X6 sim gate.

namespace facetranspose_impl_ {

template <unsigned> struct dependent_false_u : public std::false_type {};

// Blackhole backend-config field constants (hw/inc .../blackhole/
// cfg_defines.h; the X6 arsenal probe static_asserts every one of these
// against the production headers -- the drift belt).  ADDR32 values index
// 32-bit config words; RMWCIB rewrites one byte of such a word.
constexpr unsigned bh_alu_format_srca_addr32 = 1;   // ALU_FORMAT_SPEC_REG0_SrcA
constexpr unsigned bh_alu_format_srca_shamt = 17;
constexpr unsigned bh_alu_format_srca_mask = 0x1e0000;
constexpr unsigned bh_alu_fp32_enabled_addr32 = 1;  // ALU_ACC_CTRL_Fp32_enabled
constexpr unsigned bh_alu_fp32_enabled_shamt = 29;
constexpr unsigned bh_alu_fp32_enabled_mask = 0x20000000;
constexpr unsigned bh_alu_zero_flag_dis_src_addr32 = 2; // ..Zero_Flag_disabled_src
constexpr unsigned bh_alu_zero_flag_dis_src_shamt = 0;
constexpr unsigned bh_alu_zero_flag_dis_src_mask = 0x1;
constexpr unsigned bh_disable_implied_srca_fmt_setc16 = 2; // SETC16 space
// DataFormat codes (blackhole tensix_types.h).
constexpr unsigned bh_fmt_float32 = 0;
constexpr unsigned bh_fmt_tf32 = 4;
constexpr unsigned bh_fmt_float16_b = 5;
// STALLWAIT resources (blackhole ckernel_instr_params.h p_stall).
constexpr unsigned bh_stall_cfg = 0x80;
constexpr unsigned bh_wait_sfpu = 0x800;
constexpr unsigned bh_srca_vld = 0x80;
constexpr unsigned bh_srcb_vld = 0x100;
// MOV instruction-mod fields (blackhole ckernel_instr_params.h p_mov*).
constexpr unsigned bh_mov_dest_norm = 0;
constexpr unsigned bh_mov_dest_32b_low = 1;
constexpr unsigned bh_movd2b_mov_4_rows = 2;
constexpr unsigned bh_movb2a_mov_4_rows = 2;
constexpr unsigned bh_movb2d_mov_4_rows = 4;
constexpr unsigned bh_mova2d_mov_8_rows = 2;
// The SFPU-invariant instruction address-mod (zero increments under the
// SFPU LLK math program; rvtt-effects no_increment_address_mode is the
// same capability fact for SFPLOAD/SFPSTORE).
constexpr unsigned bh_addr_mod_sfpu = 7;
// TRNSPSRCB operates on SrcB rows [16, 32).
constexpr unsigned bh_srcb_transpose_row_base = 16;

} // namespace facetranspose_impl_

// TOOLCHAIN DEGRADATION (the lane FA __has_builtin discipline): on a
// toolchain without the X6 builtin family the surface parses cleanly and
// every USE refuses by name at instantiation -- required so sfpi.h keeps
// compiling at earlier compiler pins (merge coordination owns the
// submodule bump).  The guard keys on ONE family member; the family lands
// atomically.
#if defined (__has_builtin) && __has_builtin (__builtin_rvtt_ttmovd2b)

namespace facetranspose_impl_ {

// Byte-granular config RMW of one field: the constexpr mirror of
// ckernel::cfg_reg_rmw_tensix<> (ckernel.h), emitting one TTRMWCIBk per
// mask-covered byte.
template <unsigned Addr32, unsigned Shamt, unsigned Mask, unsigned Val>
sfpi_inline void cfg_field_rmw ()
{
  constexpr unsigned wrdata = Val << Shamt;
  static_assert ((wrdata & ~Mask) == 0,
		 "facetranspose cfg_field_rmw: value exceeds field mask");
  if constexpr ((Mask & 0xffu) != 0)
    __builtin_rvtt_ttrmwcib (0, Mask & 0xffu, wrdata & 0xffu, Addr32);
  if constexpr ((Mask & 0xff00u) != 0)
    __builtin_rvtt_ttrmwcib (1, (Mask >> 8) & 0xffu, (wrdata >> 8) & 0xffu,
			     Addr32);
  if constexpr ((Mask & 0xff0000u) != 0)
    __builtin_rvtt_ttrmwcib (2, (Mask >> 16) & 0xffu, (wrdata >> 16) & 0xffu,
			     Addr32);
  if constexpr ((Mask & 0xff000000u) != 0)
    __builtin_rvtt_ttrmwcib (3, (Mask >> 24) & 0xffu, (wrdata >> 24) & 0xffu,
			     Addr32);
}

template <unsigned Fmt>
sfpi_inline void set_srca_format ()
{
  cfg_field_rmw<bh_alu_format_srca_addr32, bh_alu_format_srca_shamt,
		bh_alu_format_srca_mask, Fmt> ();
}

template <unsigned En>
sfpi_inline void set_fp32_enabled ()
{
  cfg_field_rmw<bh_alu_fp32_enabled_addr32, bh_alu_fp32_enabled_shamt,
		bh_alu_fp32_enabled_mask, En> ();
}

} // namespace facetranspose_impl_

// Open the face-transpose configuration block: implied-SrcA-format
// inference OFF, source zero-flag (denormal flush) OFF, then stall the
// following stream until the SFPU has drained and both Src banks are
// owned.  Mirrors blaze enter_transpose_cfg_block() + the phase-entry
// TTI_STALLWAIT.  Entry invariant: ALU_ACC_CTRL_Fp32_enabled == 1 (the
// 32-bit dest-accumulate kernel state); the block leaves it 1.
template <bool Proven = true>
sfpi_inline void face_transpose_cfg_enter ()
{
#if __riscv_xtttensixbh
  namespace fi = facetranspose_impl_;
  __builtin_rvtt_ttsetc16 (fi::bh_disable_implied_srca_fmt_setc16, 1);
  fi::cfg_field_rmw<fi::bh_alu_zero_flag_dis_src_addr32,
		    fi::bh_alu_zero_flag_dis_src_shamt,
		    fi::bh_alu_zero_flag_dis_src_mask, 1> ();
  __builtin_rvtt_ttstallwait (fi::bh_stall_cfg,
			      fi::bh_wait_sfpu | fi::bh_srca_vld
			      | fi::bh_srcb_vld);
#else
  static_assert (facetranspose_impl_::dependent_false_u<Proven ? 1u : 0u>::value,
		 "crosslane-facetranspose-unsupported-target: the FPU face "
		 "transpose choreography is Blackhole-proven only");
#endif
}

// Close the block: restore implied-format inference and the source
// zero-flag.  Mirrors blaze leave_transpose_cfg_block().
template <bool Proven = true>
sfpi_inline void face_transpose_cfg_leave ()
{
#if __riscv_xtttensixbh
  namespace fi = facetranspose_impl_;
  __builtin_rvtt_ttsetc16 (fi::bh_disable_implied_srca_fmt_setc16, 0);
  fi::cfg_field_rmw<fi::bh_alu_zero_flag_dis_src_addr32,
		    fi::bh_alu_zero_flag_dis_src_shamt,
		    fi::bh_alu_zero_flag_dis_src_mask, 0> ();
#else
  static_assert (facetranspose_impl_::dependent_false_u<Proven ? 1u : 0u>::value,
		 "crosslane-facetranspose-unsupported-target: the FPU face "
		 "transpose choreography is Blackhole-proven only");
#endif
}

// Return the Src banks to the unpacker at the end of the math epoch
// (TTSETRWC CLR_AB + counter reset -- the hand kernels' section exit).
// Call after the LAST face_transpose of the epoch, not per face.
sfpi_inline void face_transpose_release_banks ()
{
  // p_setrwc::CLR_AB = 3, SET_ABD = 7 (blackhole ckernel_instr_params.h);
  // operand order = TTI_SETRWC (clear_ab_vld, cr, d, b, a, mask).
  __builtin_rvtt_ttsetrwc (3, 0, 0, 0, 0, 7);
}

// Transpose one 16x16 face of 32-bit Dst datums in place at Dst row
// FaceRow.  Caller holds a face_transpose_cfg block open.  18 FPU words +
// 10 config-byte words, straight-line; bit-exact for every 32-bit datum.
template <unsigned FaceRow>
sfpi_inline void face_transpose_dst_32b ()
{
#if __riscv_xtttensixbh
  namespace fi = facetranspose_impl_;
  static_assert (FaceRow % 16 == 0 && FaceRow + 16 <= 1024,
		 "crosslane-facetranspose-row-unaligned: FaceRow must be a "
		 "16-aligned Dst face row below 1024");
  constexpr unsigned b = fi::bh_srcb_transpose_row_base;
  constexpr unsigned am = fi::bh_addr_mod_sfpu;

  // Pass 1: lo16 halves -> SrcB (BF16-shuffled), transpose, park in SrcA.
  fi::set_srca_format<fi::bh_fmt_float16_b> ();
  __builtin_rvtt_ttmovd2b (fi::bh_mov_dest_32b_low, b + 0, am,
			   fi::bh_movd2b_mov_4_rows, FaceRow + 0);
  __builtin_rvtt_ttmovd2b (fi::bh_mov_dest_32b_low, b + 4, am,
			   fi::bh_movd2b_mov_4_rows, FaceRow + 4);
  __builtin_rvtt_ttmovd2b (fi::bh_mov_dest_32b_low, b + 8, am,
			   fi::bh_movd2b_mov_4_rows, FaceRow + 8);
  __builtin_rvtt_ttmovd2b (fi::bh_mov_dest_32b_low, b + 12, am,
			   fi::bh_movd2b_mov_4_rows, FaceRow + 12);
  __builtin_rvtt_tttrnspsrcb ();
  __builtin_rvtt_ttmovb2a (0, am, fi::bh_movb2a_mov_4_rows, b + 0);
  __builtin_rvtt_ttmovb2a (4, am, fi::bh_movb2a_mov_4_rows, b + 4);
  __builtin_rvtt_ttmovb2a (8, am, fi::bh_movb2a_mov_4_rows, b + 8);
  __builtin_rvtt_ttmovb2a (12, am, fi::bh_movb2a_mov_4_rows, b + 12);

  // Pass 2: hi16 halves -> SrcB (TF32-shuffled), transpose, write back to
  // Dst bits 31..16 (bits 15..0 zeroed, rewritten by pass 3).
  fi::set_srca_format<fi::bh_fmt_tf32> ();
  __builtin_rvtt_ttmovd2b (fi::bh_mov_dest_norm, b + 0, am,
			   fi::bh_movd2b_mov_4_rows, FaceRow + 0);
  __builtin_rvtt_ttmovd2b (fi::bh_mov_dest_norm, b + 4, am,
			   fi::bh_movd2b_mov_4_rows, FaceRow + 4);
  __builtin_rvtt_ttmovd2b (fi::bh_mov_dest_norm, b + 8, am,
			   fi::bh_movd2b_mov_4_rows, FaceRow + 8);
  __builtin_rvtt_ttmovd2b (fi::bh_mov_dest_norm, b + 12, am,
			   fi::bh_movd2b_mov_4_rows, FaceRow + 12);
  __builtin_rvtt_tttrnspsrcb ();
  __builtin_rvtt_ttmovb2d (fi::bh_mov_dest_norm, b + 0, am,
			   fi::bh_movb2d_mov_4_rows, FaceRow + 0);
  __builtin_rvtt_ttmovb2d (fi::bh_mov_dest_norm, b + 4, am,
			   fi::bh_movb2d_mov_4_rows, FaceRow + 4);
  __builtin_rvtt_ttmovb2d (fi::bh_mov_dest_norm, b + 8, am,
			   fi::bh_movb2d_mov_4_rows, FaceRow + 8);
  __builtin_rvtt_ttmovb2d (fi::bh_mov_dest_norm, b + 12, am,
			   fi::bh_movb2d_mov_4_rows, FaceRow + 12);

  // Pass 3: parked lo16 halves from SrcA -> Dst bits 15..0 (hi16
  // preserved).  Fp32_enabled must be 0 across these two words (the
  // UseDst32bLo write path) and back to 1 after -- the entry invariant.
  fi::set_fp32_enabled<0> ();
  fi::set_srca_format<fi::bh_fmt_float32> ();
  __builtin_rvtt_ttmova2d (fi::bh_mov_dest_32b_low, 0, am,
			   fi::bh_mova2d_mov_8_rows, FaceRow + 0);
  __builtin_rvtt_ttmova2d (fi::bh_mov_dest_32b_low, 8, am,
			   fi::bh_mova2d_mov_8_rows, FaceRow + 8);
  fi::set_fp32_enabled<1> ();
#else
  static_assert (facetranspose_impl_::dependent_false_u<FaceRow>::value,
		 "crosslane-facetranspose-unsupported-target: the FPU face "
		 "transpose choreography is Blackhole-proven only");
#endif
}

// Batched form: N consecutive faces starting at Dst row Base (the topk_xl
// transpose_N_faces shape), one cfg block around the whole batch.  The
// caller still owns bank grant/release (epoch scope, not batch scope).
template <unsigned N, unsigned Base = 0, bool OuterCfg = true>
sfpi_inline void face_transpose_dst_32b_batch ()
{
  static_assert (N >= 1 && Base % 16 == 0 && Base + 16 * N <= 1024,
		 "crosslane-facetranspose-row-unaligned: batch must be "
		 "16-aligned consecutive Dst faces below 1024");
  if constexpr (OuterCfg)
    face_transpose_cfg_enter ();
  face_transpose_dst_32b<Base> ();
  if constexpr (N > 1)
    face_transpose_dst_32b_batch<N - 1, Base + 16, false> ();
  if constexpr (OuterCfg)
    face_transpose_cfg_leave ();
}

#else // !__has_builtin (__builtin_rvtt_ttmovd2b)

// Degradation stubs: parse everywhere, refuse by name on USE.
template <bool Proven = true>
sfpi_inline void face_transpose_cfg_enter ()
{
  static_assert (facetranspose_impl_::dependent_false_u<Proven ? 1u : 0u>::value,
		 "crosslane-facetranspose-toolchain-missing-builtins: this "
		 "toolchain lacks the X6 FPU face-transpose builtin family");
}

template <bool Proven = true>
sfpi_inline void face_transpose_cfg_leave ()
{
  static_assert (facetranspose_impl_::dependent_false_u<Proven ? 1u : 0u>::value,
		 "crosslane-facetranspose-toolchain-missing-builtins: this "
		 "toolchain lacks the X6 FPU face-transpose builtin family");
}

template <bool Proven = true>
sfpi_inline void face_transpose_release_banks ()
{
  static_assert (facetranspose_impl_::dependent_false_u<Proven ? 1u : 0u>::value,
		 "crosslane-facetranspose-toolchain-missing-builtins: this "
		 "toolchain lacks the X6 FPU face-transpose builtin family");
}

template <unsigned FaceRow>
sfpi_inline void face_transpose_dst_32b ()
{
  static_assert (facetranspose_impl_::dependent_false_u<FaceRow>::value,
		 "crosslane-facetranspose-toolchain-missing-builtins: this "
		 "toolchain lacks the X6 FPU face-transpose builtin family");
}

template <unsigned N, unsigned Base = 0, bool OuterCfg = true>
sfpi_inline void face_transpose_dst_32b_batch ()
{
  static_assert (facetranspose_impl_::dependent_false_u<N>::value,
		 "crosslane-facetranspose-toolchain-missing-builtins: this "
		 "toolchain lacks the X6 FPU face-transpose builtin family");
}

#endif // __has_builtin (__builtin_rvtt_ttmovd2b)

} // namespace sfpi
