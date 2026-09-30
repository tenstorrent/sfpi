SFPI: Tenstorrent SFPU programming interface
============================================

This repo contains SFPI.

### Compiler campaign checkpoint — 2026-09-29

For a fresh machine, start with **[HANDOFF: setup, replay and diagnosis](HANDOFF.md#reproducing-a-recorded-run)**:
clone, choose WORK, load portable pins, build, verify the `tests/sfpi` link,
then run the exact manifest profile. No existing source bundle is required.

See [HANDOFF.md](HANDOFF.md) for tested source pins, exact-profile replay,
evidence links and remaining tasks; [WORKLOG.md](WORKLOG.md) records the repair
sequence, measured results and corrected hypotheses. These documents describe
the downstream `nkapre/sfpi` campaign, not a release-wide certification.

This branch is now also partitioned into a 21-stage PR stack on
`nkapre/stack` in `tenstorrent/craq-sfpi-gcc` (tip `8477d32d6ff`, upstream base
`ba48edbef33`); one of its review units is open as
[sfpi-gcc#22](https://github.com/tenstorrent/sfpi-gcc/pull/22). The rest is
built but unsubmitted.

Current validation totals and remaining gates are in the
[handoff status](HANDOFF.md#verified-results-and-limits), with raw evidence
linked there. Correctness PASS is not a performance win or formal certificate.
Performance counts from the 2026-09-29 sweep are banded at ±0.5% and must be
quoted with the band; versus hand-written LLKs the result is a statistical tie,
and roughly half the stack's options have no runtime measurement — though every
one of the 92 now has a compile-time firing record.

All three per-kernel numeric legs have since run. Formal was reconstructed onto
a re-pinned simulator and is 8/8 VALIDATED — re-derivable from committed source,
**not** reproduced from the recorded pin, which is unrecoverable. The exhaustive
leg covered the full 2^32 on a galaxy. The stratified ULP leg reached 72 ops and
found 20 defective strata over 10 op rows, **17 of them with both the compiled
and the hand-written arm out of contract** — so most of them would read as
agreement under an equivalence-only sweep. One of those defects was in
*production* and is fixed; two owner decisions from that fix are outstanding.
All of it, with its boundaries, is in [WORKLOG.md](WORKLOG.md) and
[HANDOFF.md](HANDOFF.md#verified-results-and-limits).

The source mirrors in public SFPI and craq-sfpi use `nkapre/sfpi`.
Campaign scripts and raw evidence live in the separate
[pinned workflow tree](https://github.com/tenstorrent/craq-sfpi/blob/15ec9e7e92a92258d12956a75a802a1b705fe7f3/README.md);
do not run its campaign commands from this source checkout. This documentation
update does not change compiler pins, headers, or measured configurations.

### Repository contents

* sfpi header files in `include`
* TT-enhanced RISC-V `binutils` in binutils submodule
* TT-enhanced RISC-V `gcc` in gcc submodule
* standard newlib in `newlib` submodule
* standard qemu cloned on demand
* build and release scripts in `scripts`

The Tensix backend that this fork exists for is `gcc/gcc/config/riscv/tt` --
the target passes and the `-mtt-` options that drive them. Read
`gcc/gcc/config/riscv/tt/README` first; it is the map of that directory. The
options themselves are declared in `gcc/gcc/config/riscv/riscv.opt` (113
`-mtt-` options, 107 of them `-mtt-tensix-*`); list them with:
```
  grep '^mtt-' gcc/gcc/config/riscv/riscv.opt
```
or, from a built compiler:
```
  path/to/install/sfpi/compiler/bin/riscv-tt-elf-g++ --help=target | grep mtt-
```
Only the two pressure-scheduler options are described in this README, under
`Building`; the rest are documented in the backend sources.

Which of them actually *ship* is a separate question, because tt-metal's
`jit_build/build.cpp` passes no `-mtt-tensix-*` flag at all — a production
kernel gets only the `Init(1)` options, of which there are six.
`gcc/docs/TENSIX-FLAG-DEFAULTS.md` is meant to keep that gap visible, and
`gcc/scripts/gen-flag-defaults.py` regenerates it. **That script is currently
inert**: it reads its reviewed-ON set from `craq-sfpi/dashboard/sweep_2x2.py`,
which does not exist, and a missing harness yields an empty set — so it prints
`reviewedON=0 promotion-backlog=0` and reports no gap, which is precisely the
failure its own docstring says it exists to catch. Point it at a real source
instead: a campaign sweep manifest's `flags.ON_FLAGS`, or tt-metal's
`tt_metal/tt-llk/tests/corpus/sweep_2x2.py`. Those sources agree at
`Init(1)=6 reviewedON=38 promotion-backlog=35 optin=60`, so the committed
table's "reviewed ON 39 / backlog 36" is one out — 39 is the ON arm's *token*
count (38 positive plus one `-mno-`), not its flag count. The repoint is a small
change and has been verified to run; it is not landed because the file lives in
the `gcc` submodule whose HEAD is the validated compiler pin, and committing
there would move that pin.

GCC, Binutils, Newlib and Qemu are (naturally) released under their
own licenses.

The release versioning here is simply an integral version
numbering. The major version /does not/ indicate API breaking
changes. It will be incremented when updating the compiler to a new
upstream version. (There may be other reasons to increment.)

### Reporting a Bug

**For any issues with this software please file an issue at
`https://github.com/tenstorrent/tt-metal`, and mark it with an `sfpi`
label. Do not try to file a report in this repo.**

If you are reporting when using `tt-metal`, please follow the following
procedure to obtain a reproducible test case:

* Enable some logging:
```
export TT_METAL_LOG_KERNELS_COMPILE_COMMANDS=1 TT_METAL_KERNEL_MAP=1 
export TT_METAL_LOGGER_LEVEL=info TT_METAL_LOGGER_TYPES=BuildKernels,LLRuntime 
export TT_METAL_LOGGER_FILE=$(pwd)/logger.log
```

* Run your program or test, capturing the output: `pytest ... |& tee bug.log`
* Copy the log files: `cp logger.log bug.log ~/.cache/tt-metal-cache`
* Create a tarball of `tt-metal-cache`: `tar czf bug.tgz -C ~/.cache tt-metal-cache`
* Attach that tarball to your bug report.
* Please describe what the bug is (in excrutiating detail).

If you're doing something different, add `-save-temps=obj
-fdump-tree-all -fdump-rtl-all` to the compilation line.  Tar up the
intermediate files so-produed and record the command line you used.

In either case, also determine the version of the compiler you are using:
```
path/to/install/sfpi/compiler/bin/riscv-tt-elf-g++ --version
```

**Remember, I am unlikely to be familiar with your problem domain. I do
not have your header files. It's probably difficult, if not impossible,
to reproduce your development environment.**

### User Documentation

https://docs.tenstorrent.com/tt-metal/latest/tt-metalium/tt_metal/apis/kernel_apis/sfpu/llk.html

### Obtaining Full Source

The Github-provided source tarballs (sfpi-$VERSION.tar) do not
contain the submodule source code. To obtain the full sources:

* Clone the sfpi repo: `git clone https://github.com/tenstorrent/sfpi.git`
* Enter the repo: `cd sfpi`
* Checkout the release using the tag: `git checkout $VERSION`
* Update the submodules: `git submodule update --depth 1 --init --recursive`

In the binary releases, you may examine `sfpi/README.md`, which lists
the submodules, their locations and hashes.

### Building

1) Clone the sfpi repo, & initialize submodules:
```
  git clone git@github.com:tenstorrent/sfpi.git
  git submodule update --init --recursive
```

2) Build the compiler:
```
  scripts/build.sh
```

  This will configure and build using the toplevel `configure` and
  `Makefile.in`, which originate from the RISC-V repo
  (`https://github.com/riscv-collab/riscv-gnu-toolchain`). The build is
  performed in a `build` subdirectory and a `hashes.pre` file is
  created there to record the source tree state at the start of a
  build. When making a release, you will want this to match upstream
  committed sources. If you want to build in a different subdirectory
  use the `--dir=$DIR` option.

  You may add a `--checking=VALUE` option to control gcc's checking --
  see gcc's documentation.  The default is `release`. Note this does
  not control how gcc itself is optimized (which is usually `-O2`).

  The experimental Tensix pressure scheduler can optionally link the host
  `lp_solve` library.  It is disabled by default and is not a target-runtime
  dependency.  On Ubuntu, install the host development packages and request
  it explicitly:

```
  sudo apt-get install liblpsolve55-dev libsuitesparse-dev
  SFPI_WITH_LP_SOLVE=yes scripts/build.sh
```

  `SFPI_WITH_LP_SOLVE` accepts `no` (the default), `auto`, `yes`, or an
  absolute installation prefix.  Use a fresh build directory when changing
  this setting; the build wrapper rejects an explicitly requested setting
  that does not match an existing configuration.  The resulting compiler
  still uses the list scheduler only when
  `-mtt-tensix-optimize-pressure-schedule` is passed; add
  `-mtt-tensix-pressure-schedule-use-milp` to invoke the solver backend.
  A compiler built without `lp_solve` still accepts both options and
  deterministically falls back to the independently validated list schedule.
  Solver-linked `cc1`/`cc1plus` binaries may depend on host `libcolamd` and
  `libsuitesparseconfig`, so release packaging must either provide those
  dependencies or keep `SFPI_WITH_LP_SOLVE=no`.

  A built compiler can be checked directly with
  `scripts/validate-sfpu-pressure-scheduler.sh build`.  The validation compiles
  ordinary vFloat C++ on Wormhole and Blackhole; handwritten Welford is used
  only as an assembly reference, and a separate fused arithmetic DFG proves
  that scheduling is not specialized to Welford.

  If the build is interrupted, you can of course enter the appropriate
  subdirectory and manually resume after correcting the problem --
  such build would not be suitable for releasing though.

  See below about the various `--test` options that may also be used.

3) Create a release

  * Build a release as described above.

  * Create the release artifacts:

```
  scripts/release.sh
```

  The same `--dir=$DIR` option as the build script is accepted. It
  will verify the source hashes are unchanged from when the build
  started. You may override this check with the `--force` option, but
  /be careful/.

  A `.txz` tarball will be created in a `release` directory, along
  with `.deb` or `.rpm` packages. Also a `.version` file is created.

4) Making the release available (from github)

  Create an `sfpi-version` file from the hash files generated during
  the release process (you may have several, by building and
  releasing on several hosts):

```
  scripts/sfpi-info.sh MERGE $VERSION_FILES
```

  Where `$VERSION_FILES` are the `.version` files created by one or
  more releases.  You will probably have to edit the created file to
  adjust the sfpi_url value.

  Upload the release files and sfpi-version to a github
  release. You'll want to set the version tag to be the same as the
  version string created during the build process (and mentioned in
  the sfpi-version file)

  Users may automate downloading by augmenting their cmake `CMakeLists.txt`
  file with something like:
```
# sfpi-info.sh generates a cmake script, which we include just below.
execute_process(
    COMMAND
        PATH_TO/sfpi-info.sh CMAKE txz
    OUTPUT_FILE ${SFPI_BASE}/sfpi-version.cmake
    COMMAND_ERROR_IS_FATAL ANY
)
# sfpi-info.sh sources sfpi-version, if either changes we should reconfigure
set_property(
    DIRECTORY
    APPEND
    PROPERTY
        CMAKE_CONFIGURE_DEPENDS
            "PATH_TO/sfpi-info.sh;../sfpi-version"
)
# this script sets a bunch of variables of the form SFPI_snake_case_name
include(${SFPI_BASE}/sfpi-version.cmake)
if(NOT "${SFPI_hash}" STREQUAL "")
    # download a toolchain
    include(FetchContent)
    FetchContent_Declare(
        sfpi
        URL
            "${SFPI_url}/${SFPI_filename}"
        URL_HASH "${SFPI_HASHTYPE}=${SFPI_hash}"
        SOURCE_DIR
        "${SFPI_BASE}/sfpi"
    )
    FetchContent_MakeAvailable(sfpi)
else()
    message(FATAL "No downloadable SFPI tarball for ${SFPI_arch} ${SFPI_dist}")
endif()
```

Refer to cmake documentation for further information about
`FetchContent`, `FetchContent_Declare` and
`FetchContent_MakeAvailable`.

A variant of this mechanism is used by Tenstorent's `tt-metal` repo
-- see `tt_metal/sfpi-info.sh` and its uses in
`tt_metal/hw/CMakeLists.txt` & `install_dependencies.sh`.

To download from a shell script use:
```
eval $(path/to/sfpi-info.sh SHELL [$pkg])
```

where `$pkg` is the desired package type (defaults to your system's
package format). This will set a bunch of `sfpi_foo` variables your
script may examine.

5) Running the toolchain test suites:
```
  scripts/build.sh --test
```

This will build qemu component, and then run the testsuites.

If you just want a binutils or gcc:
```
  scripts/build.sh --test-binutils
```
or
```
  scripts/build.sh --test-gcc
```

After the dejagnu tests have executed, the summary files (`$tool.sum`)
are post processed using local xfail files in the `xfails`
directory. This filters out additional fails that are due to
limitations of the test environment or deemed expected for some other
reason. The post processed files are placed in a `build/tests` directory,
the originals are left unchanged.

Note that these dejagnu test runs are idempotent. If you want to
repeat a test run you will need to delete the stamp file in
`build/stamps` (`check-binutils-newlib` or `check-gcc-newlib`). Note
that the post processing is run each time, and thus the processed
summary files will change if the xfail files are adjusted.

6) Running the TT-specific parts of the toolchain tests.
```
  scripts/build.sh --test-tt
```

This will run just the tt-specific subdirectories of the compiler
testsuites.  The summary files are copied to the build directory /but/
are not post processed as described above. Unlike running the full
testsuite, this operation is /not/ idempotent -- there is no need to
delete a stamp file to rerun them.

7) Running the gcc testsuite with specific options:
```
PATH=$(pwd)/build/sfpi/compiler/bin:$(pwd)/build/infra/bin:$PATH \
make -C build/build-gcc-newlib-stage2/gcc check-gcc \
    "RUNTESTFLAGS=--target_board=riscv-sim/mcpu=tt-bh"
```

Alter the value passed to RUNTESTFLAGS as desired, for instance
`riscv-sim/mcpu=tt-wh`.  Add `-v` options to get more logging to the
resulting dejagnu log file.
