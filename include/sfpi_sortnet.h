/* -*- C++ -*-
 * SPDX-FileCopyrightText: © 2026 Tenstorrent Inc.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

// Sort-network library (lane FG, X5; design: lane EY-R
// CROSSLANE-DESIGN-INPUT.md P20; correctness spec: lane FB's
// crosslane arsenal per-stage bitonic goldens).
//
// Not for standalone use: included from sfpi.h after sfpi_crosslane.h.
//
// NO sort primitive exists here or anywhere (no production vector ISA
// or library has one): networks are GENERATED from the compare-exchange
// stages and fixed permutes of sfpi_crosslane.h -- sort2 / sort2_rows /
// sort2_kv / sort2_kv_rows / transp8 -- following the VQSort pattern
// (order and key-value pairing are TRAITS of the stage, never new
// instructions).  The network is the standard bitonic network: the
// stage list is exactly bitonic_network_stages(n) of the acceptance
// arsenal's oracle (tt-metal tests/python_tests/helpers/
// crosslane_oracle.py), and the per-stage results are checkable against
// its recorded traces (crosslane_fixtures/bitonic_stages.json).
//
// == Machine geometry (who sorts what) ==
//
// The register file is 8 x 32 lanes; every network sorts many
// independent "machines" at once:
//
//   bitonic_sort8<Order>  (v[8])   element i = register i; every LANE
//                                  is an independent 8-element machine
//                                  (32 machines).  Pure sort2 stages.
//   bitonic_sort32<Order> (v[8])   element (r*8 + g) = subvector row r
//                                  of register g; every COLUMN is an
//                                  independent 32-element machine
//                                  (8 machines).  Register-pair stages
//                                  lower to sort2/sort2_rows (the
//                                  per-row-group SFPSWAP direction
//                                  modes ARE the bitonic inner-stage
//                                  directions); row-pair stages ride a
//                                  transp8 sandwich (SFPTRANSP maps the
//                                  row axis onto the register axis).
//   bitonic_sort16_kv<Order> (k[4], p[4])
//                                  element (r*4 + g) = row r of key
//                                  register g, key registers L0..L3,
//                                  companions L4..L7 (the compiler pins
//                                  companion == key + 4); every COLUMN
//                                  is an independent 16-element
//                                  key-value machine (8 machines).
//                                  Every exchange is ONE
//                                  ENABLE_DEST_INDEX SFPSWAP; row-pair
//                                  stages ride transp8 sandwiches
//                                  (SFPTRANSP is TEN-2932-exempt and
//                                  transposes BOTH banks, so key and
//                                  companion stay paired).  The CALLER
//                                  owns the window: open
//                                  set_dest_index_window<true>() before
//                                  and close after (window contract in
//                                  sfpi_crosslane.h).
//
// == Why there is no bitonic_sort32_kv ==
//
// A 32-element key-value machine needs 32 keys + 32 companions = 16
// live registers per machine -- twice the 8-register LReg file -- and
// every reduced-machine layout (e.g. column-pair machines) forces
// column-axis exchanges and per-column directions that cannot be
// spelled without a 9th live register (compile-proven
// lreg-pressure-exceeded class; the SFPCONFIG lane-mask direction form
// stages its value through LReg0, and the shuffle-based column
// exchange needs partner temporaries).  The hand kernels do not do it
// either: the top-k family PACKS the index into the score's low bits
// (add_lsb / remove_msb, P19 packed-KV traffic) and runs the VALUE
// network.  bitonic_sort32_kv therefore refuses by name at compile
// time and points at that spelling.
//
// == Tie caveat (carried from the arsenal) ==
//
// SFPSWAP's equal-value behavior is an UNADJUDICATED doc-vs-sim
// divergence (visible through ENABLE_DEST_INDEX companions).  Value
// networks are unaffected (equal values swap to equal values); KV
// companion selection between EQUAL keys must not be relied on until
// silicon adjudicates.  Tie-free claims only.

#pragma once

#ifndef SFPI_SORTNET_FROM_SFPI_H
#error "include sfpi.h instead of sfpi_sortnet.h"
#endif

namespace sfpi {

namespace impl_ {

template <SortOrder Order>
constexpr SortOrder sort_flip
  = Order == SortOrder::Ascending ? SortOrder::Descending
				  : SortOrder::Ascending;

// Direction-resolved compare-exchange: Asc when the bitonic direction
// bit agrees with the requested order.
template <SortOrder Order, bool DirAsc, typename V>
sfpi_inline void ce (V &a, V &b)
{
  if constexpr (DirAsc)
    sort2<Order> (a, b);
  else
    sort2<sort_flip<Order>> (a, b);
}

// Row-group-direction compare-exchange.  PatternIfAsc names the rows
// where `a' receives the minimum under an ascending request; the
// descending request swaps the OPERAND ROLES (VQSort Order trait ->
// SFPSWAP roles; the mod set has no complemented patterns).
template <SortOrder Order, RowPattern PatternIfAsc, typename V>
sfpi_inline void ce_rows (V &a, V &b)
{
  if constexpr (Order == SortOrder::Ascending)
    sort2_rows<PatternIfAsc> (a, b);
  else
    sort2_rows<PatternIfAsc> (b, a);
}

template <SortOrder Order, bool DirAsc>
sfpi_inline void ce_kv (vFloat &ka, vFloat &kb, vUInt &pa, vUInt &pb)
{
  if constexpr (DirAsc)
    sort2_kv<Order> (ka, kb, pa, pb);
  else
    sort2_kv<sort_flip<Order>> (ka, kb, pa, pb);
}

template <SortOrder Order, RowPattern PatternIfAsc>
sfpi_inline void ce_kv_rows (vFloat &ka, vFloat &kb, vUInt &pa, vUInt &pb)
{
  if constexpr (Order == SortOrder::Ascending)
    sort2_kv_rows<PatternIfAsc> (ka, kb, pa, pb);
  else
    sort2_kv_rows<PatternIfAsc> (kb, ka, pb, pa);
}

// Dual-bank transpose of eight value registers (both banks carry sort
// data; the vUInt views are bit-pattern transparent).
sfpi_inline void sortnet_transp8 (vFloat (&v)[8])
{
  vUInt c0 = as<vUInt> (v[4]), c1 = as<vUInt> (v[5]);
  vUInt c2 = as<vUInt> (v[6]), c3 = as<vUInt> (v[7]);
  transp8 (v[0], v[1], v[2], v[3], c0, c1, c2, c3);
  v[4] = as<vFloat> (c0);
  v[5] = as<vFloat> (c1);
  v[6] = as<vFloat> (c2);
  v[7] = as<vFloat> (c3);
}

} // namespace impl_

//////////////////////////////////////////////////////////////////////////////
// bitonic_sort8: 32 independent 8-element machines (one per lane),
// element i = register i.  Stage list == bitonic_network_stages(8):
// six stages of four register-pair exchanges; direction of pair
// (i, i^j) is ascending iff (i & k) == 0.
//
// STATIC COST: 24 SFPSWAP (48 issue slots with the next-slot stalls).

// The optional Stages parameter truncates the network after that many
// stages (per-stage sim validation against the arsenal's recorded
// traces); the default runs the full network.
template <SortOrder Order, unsigned Stages = 6>
sfpi_inline void bitonic_sort8 (vFloat (&v)[8])
{
  using namespace impl_;
  static_assert (Stages <= 6, "bitonic_sort8: 6 stages");
  if constexpr (Stages > 0)
    {
      // k = 2, j = 1: dir(i) = (i & 2) == 0.
      ce<Order, true> (v[0], v[1]);
      ce<Order, false> (v[2], v[3]);
      ce<Order, true> (v[4], v[5]);
      ce<Order, false> (v[6], v[7]);
    }
  if constexpr (Stages > 1)
    {
      // k = 4, j = 2: dir = (i & 4) == 0.
      ce<Order, true> (v[0], v[2]);
      ce<Order, true> (v[1], v[3]);
      ce<Order, false> (v[4], v[6]);
      ce<Order, false> (v[5], v[7]);
    }
  if constexpr (Stages > 2)
    {
      // k = 4, j = 1.
      ce<Order, true> (v[0], v[1]);
      ce<Order, true> (v[2], v[3]);
      ce<Order, false> (v[4], v[5]);
      ce<Order, false> (v[6], v[7]);
    }
  if constexpr (Stages > 3)
    {
      // k = 8, j = 4: all ascending.
      ce<Order, true> (v[0], v[4]);
      ce<Order, true> (v[1], v[5]);
      ce<Order, true> (v[2], v[6]);
      ce<Order, true> (v[3], v[7]);
    }
  if constexpr (Stages > 4)
    {
      // k = 8, j = 2.
      ce<Order, true> (v[0], v[2]);
      ce<Order, true> (v[1], v[3]);
      ce<Order, true> (v[4], v[6]);
      ce<Order, true> (v[5], v[7]);
    }
  if constexpr (Stages > 5)
    {
      // k = 8, j = 1.
      ce<Order, true> (v[0], v[1]);
      ce<Order, true> (v[2], v[3]);
      ce<Order, true> (v[4], v[5]);
      ce<Order, true> (v[6], v[7]);
    }
}

//////////////////////////////////////////////////////////////////////////////
// bitonic_sort32: 8 independent 32-element machines (one per column),
// element e = r*8 + g (subvector row r of register g).  Direction of
// pair (e, e^j) is ascending iff (e & k) == 0, so:
//   k <= 4   direction depends on g only  -> per-pair sort2
//   k = 8    direction = row bit 0        -> SFPSWAP Min02Max13
//   k = 16   direction = row bit 1        -> SFPSWAP Min01Max23
//   k = 32   ascending                    -> sort2
// and the exchange axis:
//   j <= 4   register pairs (g, g^j)
//   j >= 8   row pairs (r, r^(j/8)) -> transp8 sandwich: SFPTRANSP
//            maps element (reg B+g', row r) to (reg B+r, row g') per
//            bank, turning row pairs into register pairs.
//
// STATIC COST: 60 SFPSWAP + 4 SFPTRANSP (124 issue slots): 15 stages
// of 4 register-pair exchanges, two transp8 sandwiches (the second is
// shared by the k=32 row stages).

template <SortOrder Order, unsigned Stages = 15>
sfpi_inline void bitonic_sort32 (vFloat (&v)[8])
{
  using namespace impl_;
  static_assert (Stages <= 15, "bitonic_sort32: 15 stages");
  if constexpr (Stages > 0)
    {
      // k = 2, j = 1: dir = (g & 2) == 0 (row-uniform).
      ce<Order, true> (v[0], v[1]);
      ce<Order, false> (v[2], v[3]);
      ce<Order, true> (v[4], v[5]);
      ce<Order, false> (v[6], v[7]);
    }
  if constexpr (Stages > 1)
    {
      // k = 4, j = 2: dir = (g & 4) == 0.
      ce<Order, true> (v[0], v[2]);
      ce<Order, true> (v[1], v[3]);
      ce<Order, false> (v[4], v[6]);
      ce<Order, false> (v[5], v[7]);
    }
  if constexpr (Stages > 2)
    {
      // k = 4, j = 1.
      ce<Order, true> (v[0], v[1]);
      ce<Order, true> (v[2], v[3]);
      ce<Order, false> (v[4], v[5]);
      ce<Order, false> (v[6], v[7]);
    }
  if constexpr (Stages > 3)
    {
      // k = 8, j = 4: dir = (e & 8) == 0 = row bit 0 -> rows 0,2 give
      // the lower register the minimum.
      ce_rows<Order, RowPattern::Min02Max13> (v[0], v[4]);
      ce_rows<Order, RowPattern::Min02Max13> (v[1], v[5]);
      ce_rows<Order, RowPattern::Min02Max13> (v[2], v[6]);
      ce_rows<Order, RowPattern::Min02Max13> (v[3], v[7]);
    }
  if constexpr (Stages > 4)
    {
      // k = 8, j = 2.
      ce_rows<Order, RowPattern::Min02Max13> (v[0], v[2]);
      ce_rows<Order, RowPattern::Min02Max13> (v[1], v[3]);
      ce_rows<Order, RowPattern::Min02Max13> (v[4], v[6]);
      ce_rows<Order, RowPattern::Min02Max13> (v[5], v[7]);
    }
  if constexpr (Stages > 5)
    {
      // k = 8, j = 1.
      ce_rows<Order, RowPattern::Min02Max13> (v[0], v[1]);
      ce_rows<Order, RowPattern::Min02Max13> (v[2], v[3]);
      ce_rows<Order, RowPattern::Min02Max13> (v[4], v[5]);
      ce_rows<Order, RowPattern::Min02Max13> (v[6], v[7]);
    }
  if constexpr (Stages > 6)
    {
      // k = 16, j = 8: row pairs (r, r^1), dir = row bit 1 ->
      // transposed register pairs (T0,T1) asc, (T2,T3) desc, per bank.
      sortnet_transp8 (v);
      ce<Order, true> (v[0], v[1]);
      ce<Order, false> (v[2], v[3]);
      ce<Order, true> (v[4], v[5]);
      ce<Order, false> (v[6], v[7]);
      sortnet_transp8 (v);
    }
  if constexpr (Stages > 7)
    {
      // k = 16, j = 4: register pairs, dir = row bit 1.
      ce_rows<Order, RowPattern::Min01Max23> (v[0], v[4]);
      ce_rows<Order, RowPattern::Min01Max23> (v[1], v[5]);
      ce_rows<Order, RowPattern::Min01Max23> (v[2], v[6]);
      ce_rows<Order, RowPattern::Min01Max23> (v[3], v[7]);
    }
  if constexpr (Stages > 8)
    {
      // k = 16, j = 2.
      ce_rows<Order, RowPattern::Min01Max23> (v[0], v[2]);
      ce_rows<Order, RowPattern::Min01Max23> (v[1], v[3]);
      ce_rows<Order, RowPattern::Min01Max23> (v[4], v[6]);
      ce_rows<Order, RowPattern::Min01Max23> (v[5], v[7]);
    }
  if constexpr (Stages > 9)
    {
      // k = 16, j = 1.
      ce_rows<Order, RowPattern::Min01Max23> (v[0], v[1]);
      ce_rows<Order, RowPattern::Min01Max23> (v[2], v[3]);
      ce_rows<Order, RowPattern::Min01Max23> (v[4], v[5]);
      ce_rows<Order, RowPattern::Min01Max23> (v[6], v[7]);
    }
  if constexpr (Stages > 10)
    {
      // k = 32, j = 16 and j = 8: row pairs (r, r^2) then (r, r^1),
      // all ascending -- one shared sandwich (a truncation between the
      // two still closes it).
      sortnet_transp8 (v);
      ce<Order, true> (v[0], v[2]);
      ce<Order, true> (v[1], v[3]);
      ce<Order, true> (v[4], v[6]);
      ce<Order, true> (v[5], v[7]);
      if constexpr (Stages > 11)
	{
	  ce<Order, true> (v[0], v[1]);
	  ce<Order, true> (v[2], v[3]);
	  ce<Order, true> (v[4], v[5]);
	  ce<Order, true> (v[6], v[7]);
	}
      sortnet_transp8 (v);
    }
  if constexpr (Stages > 12)
    {
      // k = 32, j = 4.
      ce<Order, true> (v[0], v[4]);
      ce<Order, true> (v[1], v[5]);
      ce<Order, true> (v[2], v[6]);
      ce<Order, true> (v[3], v[7]);
    }
  if constexpr (Stages > 13)
    {
      // k = 32, j = 2.
      ce<Order, true> (v[0], v[2]);
      ce<Order, true> (v[1], v[3]);
      ce<Order, true> (v[4], v[6]);
      ce<Order, true> (v[5], v[7]);
    }
  if constexpr (Stages > 14)
    {
      // k = 32, j = 1.
      ce<Order, true> (v[0], v[1]);
      ce<Order, true> (v[2], v[3]);
      ce<Order, true> (v[4], v[5]);
      ce<Order, true> (v[6], v[7]);
    }
}

//////////////////////////////////////////////////////////////////////////////
// bitonic_sort16_kv: 8 independent 16-element KEY-VALUE machines (one
// per column), element e = r*4 + g (row r of key register g); keys in
// k[0..3] (allocated L0..L3), companions in p[0..3] (pinned L4..L7 by
// the indexed-swap register alternatives).  Direction of (e, e^j):
//   k = 2   (e & 2) = g bit 1  -> per-pair
//   k = 4   (e & 4) = row bit 0 -> Min02Max13
//   k = 8   (e & 8) = row bit 1 -> Min01Max23
//   k = 16  ascending
// Exchange axis: j <= 2 register pairs; j >= 4 row pairs via the
// transp8 sandwich -- SFPTRANSP is TEN-2932-exempt and transposes BOTH
// banks, so each key's companion rides the identical in-bank
// permutation and the pairing survives.
//
// CALLER CONTRACT: the ENABLE_DEST_INDEX window must be OPEN
// (set_dest_index_window<true>()) around the call, and the caller
// closes it; window content here is exactly SFPSWAP/SFPTRANSP (the
// TEN-2932-exempt set), which the -mtt-tensix-optimize-crosslane
// window checker verifies on the final stream.
//
// STATIC COST: 20 indexed SFPSWAP + 4 SFPTRANSP (44 issue slots).

template <SortOrder Order, unsigned Stages = 10>
sfpi_inline void bitonic_sort16_kv (vFloat (&k)[4], vUInt (&p)[4])
{
  using namespace impl_;
  static_assert (Stages <= 10, "bitonic_sort16_kv: 10 stages");
  if constexpr (Stages > 0)
    {
      // k = 2, j = 1: dir = (g & 2) == 0.
      ce_kv<Order, true> (k[0], k[1], p[0], p[1]);
      ce_kv<Order, false> (k[2], k[3], p[2], p[3]);
    }
  if constexpr (Stages > 1)
    {
      // k = 4, j = 2: dir = row bit 0.
      ce_kv_rows<Order, RowPattern::Min02Max13> (k[0], k[2], p[0], p[2]);
      ce_kv_rows<Order, RowPattern::Min02Max13> (k[1], k[3], p[1], p[3]);
    }
  if constexpr (Stages > 2)
    {
      // k = 4, j = 1.
      ce_kv_rows<Order, RowPattern::Min02Max13> (k[0], k[1], p[0], p[1]);
      ce_kv_rows<Order, RowPattern::Min02Max13> (k[2], k[3], p[2], p[3]);
    }
  if constexpr (Stages > 3)
    {
      // k = 8, j = 4: row pairs (r, r^1), dir = row bit 1 ->
      // transposed pairs (T0,T1) asc, (T2,T3) desc.
      transp8 (k[0], k[1], k[2], k[3], p[0], p[1], p[2], p[3]);
      ce_kv<Order, true> (k[0], k[1], p[0], p[1]);
      ce_kv<Order, false> (k[2], k[3], p[2], p[3]);
      transp8 (k[0], k[1], k[2], k[3], p[0], p[1], p[2], p[3]);
    }
  if constexpr (Stages > 4)
    {
      // k = 8, j = 2: register pairs, dir = row bit 1.
      ce_kv_rows<Order, RowPattern::Min01Max23> (k[0], k[2], p[0], p[2]);
      ce_kv_rows<Order, RowPattern::Min01Max23> (k[1], k[3], p[1], p[3]);
    }
  if constexpr (Stages > 5)
    {
      // k = 8, j = 1.
      ce_kv_rows<Order, RowPattern::Min01Max23> (k[0], k[1], p[0], p[1]);
      ce_kv_rows<Order, RowPattern::Min01Max23> (k[2], k[3], p[2], p[3]);
    }
  if constexpr (Stages > 6)
    {
      // k = 16, j = 8 and j = 4: row pairs (r, r^2) then (r, r^1),
      // ascending -- one shared sandwich (a truncation between the two
      // still closes it).
      transp8 (k[0], k[1], k[2], k[3], p[0], p[1], p[2], p[3]);
      ce_kv<Order, true> (k[0], k[2], p[0], p[2]);
      ce_kv<Order, true> (k[1], k[3], p[1], p[3]);
      if constexpr (Stages > 7)
	{
	  ce_kv<Order, true> (k[0], k[1], p[0], p[1]);
	  ce_kv<Order, true> (k[2], k[3], p[2], p[3]);
	}
      transp8 (k[0], k[1], k[2], k[3], p[0], p[1], p[2], p[3]);
    }
  if constexpr (Stages > 8)
    {
      // k = 16, j = 2: register pairs, ascending.
      ce_kv<Order, true> (k[0], k[2], p[0], p[2]);
      ce_kv<Order, true> (k[1], k[3], p[1], p[3]);
    }
  if constexpr (Stages > 9)
    {
      // k = 16, j = 1.
      ce_kv<Order, true> (k[0], k[1], p[0], p[1]);
      ce_kv<Order, true> (k[2], k[3], p[2], p[3]);
    }
}

// Named refusal: see the header comment for the register-file
// arithmetic.  The supported spellings are bitonic_sort16_kv (true
// companions) or bitonic_sort32 over P19-packed (index | score)
// values, the hand kernels' add_lsb/remove_msb discipline.
template <SortOrder Order, typename... T>
sfpi_inline void bitonic_sort32_kv (T &...)
{
  static_assert (impl_::crosslane_dependent_false<vFloat>::value,
		 "crosslane-kv32-register-file: a 32-element key-value "
		 "machine needs 32 keys + 32 companions = 16 live registers "
		 "-- twice the LReg file -- and every reduced-machine "
		 "layout forces column exchanges/directions that need a 9th "
		 "live register.  Use bitonic_sort16_kv, or pack the index "
		 "into the value's low bits (P19) and run bitonic_sort32");
}

} // namespace sfpi
