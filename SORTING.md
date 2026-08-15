# High-Performance Vectorized Sorting & Top-K Architecture on Tenstorrent Tensix (Wormhole & Blackhole)

## 1. Executive Summary & Mission Objectives

Sorting and Top-$K$ selection are critical computational bottlenecks across modern deep learning workloads:
- **MoE Expert Routing:** Mixtral 8x7B/8x22B ($K=2, N=8$), DeepSeek-V3 / DeepSeek-R1 ($K=8, N=256$), Qwen2-MoE ($K=4, N=60$).
- **Autoregressive LLM Sampling:** Top-$K$ decoding ($K \in \{32, 50, 64\}$, $N = 32\text{k} \dots 128\text{k}$).
- **Vector Search & RAG:** Approximate Nearest Neighbor ranking ($K \in \{64, 128, 256, 1024\}$, $N = 10\text{k} \dots 1\text{M}$).
- **Vision & Multimodal:** Point cloud nearest neighbors, Top-$K$ RoI pooling, Non-Maximum Suppression (NMS).

This document establishes a **rigorous architectural attack plan** to deliver optimal, hardware-specialized sorting and Top-$K$ kernels on Tenstorrent Tensix Vector Processing Units (SFPU) on **Wormhole** and **Blackhole**.

---

## 2. Tensix SFPU Hardware & ISA Foundations

### 2.1 Vector Register File & Tile Geometry

A Tensix tile consists of **$32 \times 32$ elements** (1024 elements), arranged as **4 faces of $16 \times 16$ elements**:
- The SFPU is a **32-lane SIMD vector coprocessor**.
- Vector registers ($L_0 \dots L_7$) hold **32 elements** across physical SIMD lanes.
- A single $16 \times 16$ face contains 256 elements, represented across 8 vector registers (or 8 rows of 32 elements across dual-row configurations).

```
┌─────────────────────────────────────────────────────────────┐
│                   TENSIX TILE (32 x 32)                     │
│  ┌───────────────────────────┬───────────────────────────┐  │
│  │   Face 0 (16x16 elements) │   Face 1 (16x16 elements) │  │
│  │   Rows 0..15, Cols 0..15  │   Rows 0..15, Cols 16..31 │  │
│  ├───────────────────────────┼───────────────────────────┤  │
│  │   Face 2 (16x16 elements) │   Face 3 (16x16 elements) │  │
│  │   Rows 16..31, Cols 0..15 │   Rows 16..31, Cols 16..31│  │
│  └───────────────────────────┴───────────────────────────┘  │
└─────────────────────────────────────────────────────────────┘
```

---

### 2.2 `SFPSWAP`: The Dual Compare-and-Swap Engine

The Tenstorrent SFPU features a specialized hardware instruction designed specifically for sorting networks: `SFPSWAP`.

#### Mechanics & Execution Modes:
1. **Unconditional Swap (`Mod1 = 0`):** Swaps vector registers $V_D$ and $V_C$.
2. **Vector Min/Max (`Mod1 = 1`):** Computes lanewise minimum in $V_D$ and lanewise maximum in $V_C$ simultaneously:
   $$V_D[i] = \min(V_D[i], V_C[i]), \quad V_C[i] = \max(V_D[i], V_C[i])$$
3. **Subvector Polarity Masking (`Mod1 \in \{2 \dots 8\}`):** Inverts comparison direction across subvector slices to implement bitonic network stages in hardware without branch or shuffle overhead:
   - `SUBVEC_MIN01_MAX23 (Mod1 = 2)`: First 16 lanes $V_D = \min, V_C = \max$; last 16 lanes $V_D = \max, V_C = \min$.
   - `SUBVEC_MIN02_MAX13 (Mod1 = 3)`: Lanes 0..7 and 16..23 $V_D = \min$; lanes 8..15 and 24..31 $V_D = \max$.
   - `SUBVEC_MIN03_MAX12 (Mod1 = 4)`: Lanes 0..7 and 24..31 $V_D = \min$; lanes 8..23 $V_D = \max$.

#### Simultaneous Value & Index Tracking (`ENABLE_DEST_INDEX`):
When `ENABLE_DEST_INDEX` is set in `SFPCONFIG`:
- $V_C, V_D \in \{L_0 \dots L_3\}$ hold data values (scores/keys).
- Companion registers $V_{CA}, V_{DA} \in \{L_4 \dots L_7\}$ (where $V_A = 4 + (V \ \& \ 3)$) hold companion indices.
- **In a single 2-cycle instruction**, `SFPSWAP` compares keys in $L_0 \dots L_3$, swaps the keys, **and simultaneously swaps the corresponding indices in $L_4 \dots L_7$**!

```
               SFPSWAP SIMULTANEOUS ARGMIN/ARGMAX PIPELINE
┌───────────────────────────┐             ┌───────────────────────────┐
│ Value Reg VC (L0..L3)     │             │ Value Reg VD (L0..L3)     │
└─────────────┬─────────────┘             └─────────────┬─────────────┘
              │                                         │
              ▼                                         ▼
       ┌──────────────┐                          ┌──────────────┐
       │ Key Compare  ├─────────────────────────►│ Conditional  │
       │ (Sign-Mag)   │                          │ Value Swap   │
       └──────┬───────┘                          └──────┬───────┘
              │                                         │
              │ Swap Trigger (if VC < VD)               ▼
              │                          ┌─────────────────────────────┐
              │                          │ Updated L0..L3 (Min in VD)  │
              │                          └─────────────────────────────┘
              ▼
       ┌──────────────┐
       │ Index Swap   ├─────────────────────────►┌─────────────────────────────┐
       │ (Companion)  │                          │ Updated L4..L7 (Argmin in   │
       └──────────────┘                          │  Companion of VD)           │
                                                 └─────────────────────────────┘
```

---

### 2.3 `SFPTRANSP`: Subvector Intra-Tile Matrix Permutation

`SFPTRANSP` transposes a $4 \times 4$ subgrid of lanes within every 8-lane column across 4 stacked vector registers ($L_0 \dots L_3$ and $L_4 \dots L_7$):
- Viewing 4 vector registers as a $16 \times 8$ grid of lanes, it transposes the 16 elements in each column.
- Eliminates expensive scalar cross-lane permute instructions.
- Converts row-wise sorted sequences into column-wise sorted sequences, enabling multi-stage bitonic sorting across register arrays.

---

## 3. Analytical Roofline Model for Sorting & Top-$K$

Sorting throughput on Tensix is governed by three primary hardware subsystems:
1. **Unpacker Bandwidth ($B_{\text{unpack}}$):** Transferring tiles from Circular Buffers (L1 SRAM) to SrcA/SrcB registers ($16 \text{ bytes/cycle}$).
2. **SFPU Compute Throughput ($T_{\text{sfpu}}$):** 32 SIMD lanes @ 1 instruction/cycle (or 0.5 instructions/cycle for 2-cycle `SFPSWAP`).
3. **Packer Bandwidth ($B_{\text{pack}}$):** Transferring filtered/sorted tiles from Dst Accumulator back to L1 Circular Buffers ($16 \text{ bytes/cycle}$).

```
┌────────────────────────────────────────────────────────────────────────────────┐
│                       TENSIX SFPU ROOFLINE REGIMES                             │
│                                                                                │
│  Throughput                                                                    │
│  (Elements/cyc)                                                                │
│       ▲                                                                        │
│       │                                                                        │
│   32  │                       ┌──────────────────────┐  Regime 1: MoE Micro    │
│       │                      ╱│ SFPU Compute Bound   │  (K<=8, N<=256)         │
│   16  │                     ╱ │ (Register-Resident)  │                         │
│       │                    ╱  └──────────────────────┘                         │
│    8  │                   ╱                             Regime 2: Bitonic Tile │
│       │                  ╱                              (K<=64, N<=2048)       │
│    4  │  ┌──────────────┘                                                      │
│       │  │ Dst / Memory                                 Regime 3: Streaming    │
│    2  │  │ Bandwidth                                    (K<=32, N>=4096)       │
│       │  │ Bound                                                               │
│    0  └──┴──────────────────────────────────────────► K / Problem Size         │
│         K=2       K=8       K=32      K=64     K=256    K=1024                 │
└────────────────────────────────────────────────────────────────────────────────┘
```

### 3.1 Algorithmic Regimes & Theoretical Bounds

| Regime | Problem Size | Primary Bottleneck | Optimal Algorithm | Theoretical Floor (Cycles/Tile) |
| :--- | :--- | :--- | :--- | :--- |
| **Regime 1: MoE Micro** | $K \le 8, N \le 256$ | SFPU `SFPSWAP` issue rate | Bitonic Face Reducer (Single-Pass, Zero Spill) | **$120 \dots 160$ cycles** |
| **Regime 2: Bitonic Tile** | $K \le 64, N \le 2048$ | Dst Accumulator Ping-Pong | Replay-Compressed Bitonic Merge Network | **$280 \dots 340$ cycles** |
| **Regime 3: Streaming Filter** | $K \le 32, N \ge 4096$ | Unpacker Stream Bandwidth | 1-Compare Vector Threshold Rejection Filter | **$64 \dots 96$ cycles / tile** |
| **Regime 4: Global Sort** | Full $N \log N, N > 4096$ | NOC Bandwidth + DRAM | Hierarchical Multi-Core Radix / Bitonic Merge | **Bandwidth Bound** |

---

## 4. Sorting Algorithm Taxonomy for Tensix SIMD

```
┌─────────────────────────────────────────────────────────────────────────────────────────────────┐
│                                 TENSIX SORTING ALGORITHM TAXONOMY                               │
├───────────────────────────────┬───────────────────────────────┬─────────────────────────────────┤
│ Algorithm Class               │ Tensix Hardware Primitives    │ Best Suited ML Workloads        │
├───────────────────────────────┼───────────────────────────────┼─────────────────────────────────┤
│ **1. Bitonic Sorting Network**│ `SFPSWAP` (Subvector modes),  │ MoE Routing (DeepSeek, Mixtral) │
│                               │ `SFPTRANSP`, Replay Buffer    │ Standard Top-K (K <= 64)        │
├───────────────────────────────┼───────────────────────────────┼─────────────────────────────────┤
│ **2. Vectorized Threshold     │ `SFPMAX`, `SFPSETCC`,         │ LLM Long-Context Generation,    │
│    Streaming Filter**         │ Predicated Discard            │ RAG Vector Search (N >= 4096)   │
├───────────────────────────────┼───────────────────────────────┼─────────────────────────────────┤
│ **3. Vector Radix Binning**   │ `SFPSHFT`, `SFPAND`, `SFPMOV`,│ Full Tile Sort,                 │
│                               │ Dst-Indexed Histogramming     │ Large K Ranking (K >= 256)      │
├───────────────────────────────┼───────────────────────────────┼─────────────────────────────────┤
│ **4. Multi-Core Tournament    │ NOC Multicast / Semaphores,   │ Distributed MoE (Multi-Device), │
│    Hierarchical Merge**       │ Pairwise Inter-Tile Bitonic   │ Megatron-LM Top-K Sampling      │
└───────────────────────────────┴───────────────────────────────┴─────────────────────────────────┘
```

---

## 5. Architectural Specifications for the 4 Top-$K$ Kernel Variants

### 5.1 Variant A: `TopK_MoE_Micro` (Optimized for DeepSeek & Mixtral)
- **Target Workload:** $N \le 256$ expert scores, $K \in \{2, 4, 6, 8\}$.
- **Core Architecture:**
  1. Load 16 expert scores per lane into $L_0 \dots L_3$; indices in $L_4 \dots L_7$.
  2. Execute a 16-element bitonic sorting network using 3 stages of `SFPSWAP` with subvector masks (`MOD1_VEC_MIN_MAX`, `MOD1_SUBVEC_MIN01_MAX23`).
  3. `SFPTRANSP` transposes the results across 4 registers.
  4. Truncate to top-$K$ with zero memory spills; directly pipe output into Softmax normalization.
- **Target Cycles:** **$\le 140$ cycles per face** (45% faster than baseline).

---

### 5.2 Variant B: `TopK_Bitonic_Tile` ($K \le 64$, $N \le 2048$)
- **Target Workload:** Standard LLM Top-32/64 sampling across vocabulary tiles.
- **Core Architecture:**
  1. Load pairs of 16-element vectors into $L_0 \dots L_3$ and $L_4 \dots L_7$.
  2. Use Preheader Replay Hoisting (`5a849606f`) to capture the bitonic compare-swap network into Tensix `replay_buf`.
  3. Perform iterative pairwise merge passes across 16-element faces using Dst ping-pong addressing (`ADDR_MOD_3` with `dist` increments).
- **Target Cycles:** **$\le 310$ cycles per tile**.

---

### 5.3 Variant C: `TopK_Streaming_Heap` ($K \le 32$, $N \ge 4096$)
- **Target Workload:** Large-scale sequence search (RAG, long-context beam search).
- **Core Architecture:**
  1. Maintain the running Top-$K$ candidates resident in vector registers $L_0 \dots L_3$ (values) and $L_4 \dots L_7$ (indices).
  2. Maintain a broadcast threshold register $L_{\text{thresh}} = \min(\text{Current Top-}K)$.
  3. Unpack input tiles continuously:
     - Vector compare `SFPMAX(L_in, L_thresh)`.
     - If all lanes in tile $\le L_{\text{thresh}}$, **immediately discard tile with zero Dst writes** (skips 95%+ of elements).
     - If candidate elements exceed threshold, insert into vector priority queue and update threshold.
- **Speedup Potential:** **$3.5\times \dots 6.0\times$ faster** than full sorting networks on large $N$.

---

### 5.4 Variant D: `TopK_Hierarchical_Merge` (Distributed Multi-Core)
- **Target Workload:** Full tile sorting and cross-Tensix core distributed Top-$K$.
- **Core Architecture:**
  1. Each Tensix core executes local `TopK_MoE_Micro` or `TopK_Bitonic_Tile` on its slice of input tiles.
  2. Cores exchange local top-$K$ candidates over the Tensix NoC using async semaphore handshakes (`SEMPOST`/`SEMWAIT`).
  3. Final receiver core executes a 2-way bitonic merge to produce the globally sorted Top-$K$.

---

## 6. Implementation & Verification Plan

### 6.1 Verification Contract
1. **Numerical Reference Parity:** 100% bitwise parity against sequential FP64/FP32 host references.
2. **Strict Index Stability:** When scores are identical ($A[i] == A[j]$ where $i < j$), verify index order is deterministic.
3. **Adversarial Input Matrix:**
   - Uniform random floats.
   - Reverse-sorted and fully sorted inputs.
   - All-equal keys (testing comparator degenerate modes).
   - Infs, NaNs, and Signed Zeros ($-0.0$ vs $+0.0$ sign-magnitude ordering).

---

### 6.2 Phased Performance Execution Roadmap

```
┌─────────┬──────────────────────────────────────┬───────────┬───────────────────────────────────────────┐
│ Sprint  │ Deliverable                          │ Target    │ Measurable Gate                           │
├─────────┼──────────────────────────────────────┼───────────┼───────────────────────────────────────────┤
│ **S1**  │ `TopK_MoE_Micro` (K in {2,4,6,8})    │ Weeks 1-2 │ <= 140 cycles on Blackhole silicon        │
├─────────┼──────────────────────────────────────┼───────────┼───────────────────────────────────────────┤
│ **S2**  │ Replay-Compressed `TopK_Bitonic_Tile`│ Weeks 3-4 │ <= 310 cycles on Blackhole silicon        │
├─────────┼──────────────────────────────────────┼───────────┼───────────────────────────────────────────┤
│ **S3**  │ `TopK_Streaming_Heap` (Large N)      │ Weeks 5-6 │ > 3.0x speedup over full sort for N=8192  │
├─────────┼──────────────────────────────────────┼───────────┼───────────────────────────────────────────┤
│ **S4**  │ Hierarchical Multi-Core Top-K        │ Weeks 7-8 │ Linear scaling across 8x8 Tensix mesh     │
└─────────┴──────────────────────────────────────┴───────────┴───────────────────────────────────────────┘
```
