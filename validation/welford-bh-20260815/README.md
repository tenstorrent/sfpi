# Blackhole Welford validation — 2026-08-15

Verdict: **GO-BH-ONLY** for the raw-LREG correctness fix and full-literal
coalescing optimization. Wormhole compiled successfully but no Wormhole device
was available.

## Pinned source and compiler tuple

- SFPI superproject: `3264e3c3a15e9e6bcb782dee8730e574f34bd119`
- SFPI-GCC: `d9c39fbd15c3e3a1ccefdca3aa994029e07efb57`
- TT-Metal harness: `b6dd63708583a9762cce042a30d5e1ee10872a62`
- Installed `cc1plus` SHA-256:
  `284d266fd681a1d40f0e74c586b93eef1b862ca6b915823f4fd77e1c6782c54f`
- Device: Blackhole P100A, 800 MHz during capture. `device.json` records the
  driver, firmware, board identifier, clocks, and host software.

## Gates and results

- Solver-enabled checked compiler build: pass (`--with-lp-solve=yes`).
- Focused WH/BH scheduler validator with MILP required and three determinism
  runs: pass. The complete generated compiler artifacts and TT target DejaGNU
  output are preserved in `compiler-artifacts.tar.gz`.
- TT target DejaGNU suite: pass (`Tests passed. Yay!`).
- Physical Blackhole correctness: 15/15 pass — five implementations at
  N=1, N=2, and N=32. The test compares captured L4 mean and L5 M2 against a
  host sequential Welford reference. See `correctness/run.log`.
- Final N=32 vFloat math ELF SHA-256:
  `81ccafc96c30256758f07e70c547b1ecf0722cb16bf329fe592872877420cfdc`.
- Full-literal staging pattern `SFPLOADI; SFPMOV; SFPLOADI`: 0 occurrences in
  the final vFloat disassembly (seven before the optimization).

## Device timing

Each row below is a fresh pytest process/device session. `raw.csv` and
`post.csv` are copied immediately after each run into its unique directory.

| Implementation | WELFORD_BODY device cycles | Minimum | Mean | Range |
| --- | ---: | ---: | ---: | ---: |
| Handwritten replay | 326 / 326 / 326 | 326 | 326 | 0 |
| Compiler-generated vFloat direct | 323 / 323 / 323 | 323 | 323 | 0 |

The generated body is 3 cycles (0.92%) faster than handwritten replay in this
microbenchmark. This is a math-TRISC `WELFORD_BODY` profiler-zone delta from
device `read_wall_clock()` timestamps. It is not pytest time, a raw hardware
performance-counter event, or end-to-end kernel throughput. The zone wraps the
Welford recurrence and excludes initialization and destination completion.

The measurement does not establish a pressure-scheduler benefit. The Welford
body does not provide a pinned, identical-source, changed-binary flag-off/flag-on
A/B, and the historical `vFloat direct` and `vFloat rescue` selectors are
different source formulations.

## Reproduction

From the TT-Metal LLK test directory, point `tests/sfpi` at the pinned SFPI
install and run:

```bash
PYTHONPATH=/localdev/nkapre/sfpi-gcc-lreg-artifacts \
CHIP_ARCH=blackhole TT_METAL_DISABLE_SFPLOADMACRO=1 \
RUNNER_TEMP=/localdev/nkapre/welford-correctness \
/localdev/nkapre/tt-metal/python_env/bin/pytest -q -s -o addopts='' \
  -p pytest_workerid_plugin --timeout=600 \
  python_tests/test_sfpu_welford_prefix_snapshot.py::test_sfpu_welford_prefix_snapshot
```

For each timing selector/run, use a fresh `RUNNER_TEMP` and invoke
`test_sfpu_welford_device_profile[1-handwritten_replay]` or
`test_sfpu_welford_device_profile[2-vfloat_direct]`. Copy the fixture's shared
raw/post CSV immediately after each process, as done in `profiler/`.

`SHA256SUMS` covers every archived file. No Wormhole silicon result is implied.
