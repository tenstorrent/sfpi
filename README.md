SFPI: Tenstorrent SFPU programming interface
============================================

This repo contains SFPI.

* sfpi header files in `include`
* TT-enhanced RISC-V `binutils` in binutils submodule
* TT-enahanced RISC-V `gcc` in gcc submodule
* standard newlib in `newlib` submodule
* tests in `tests`
* build and release scripts in `scripts`

GCC, Binutils and Newlib are (naturally) released under their own licenses.

The release versioning here is simply an integral version
numbering. The major version /does not/ indicate API breaking
changes. It will be incremented when updating the compiler to a new
upstream version. (There may be other reasons to increment.)

### User Documentation

https://tenstorrent-metal.github.io/tt-metal/latest/tt_metal/apis/kernel_apis/sfpu/llk.html

### Building
1) Clone the sfpi repo, & initialize submodules:
```
  git clone git@github.com:tenstorrent-metal/sfpi.git
  git submodule update --init --resursive
```

2) Build the compiler:
```
  scripts/build.sh
```

  This will configure and build using the toplevel `configure` and
  `Makefile.in`, which originate from the RISC-V repo. The build is
  performed in a `build` subdirectory and a `sfpi/src-hashes` file is
  created there to record the source tree state at the start of a
  build. When making a release, you will want this to match upstream
  committed sources.

  If the build is interrupted, you can of course enter the appropriate
  subdirectory and manually resume after correcting the problem --
  such build would not be suitable for releasing though.

3) Build and running the sfpi tests:

```
    mkdir -s ../tests build
    make -C build.tests all
    make -C build.tests test
```

  If the `all` target succeeds, the compiler at least exports the
  right intrinsics for use w/ the synced sfpi. If it fails to build,
  there is likely a sync error (submodule out of date) between what
  the compiler is exposing and what sfpi is calling.

  The `test` target builds kernels and verifies the resulting assembly
  is the same as a set of gold-standard files. This is unfortunately brittle.

4) Create a release
```
  scripts/release.sh
```

  This will verify the source hashes are unchanged from when the build
  started, and create both a tarball `sfpi-release.tgz` and an md5
  `sfpi.md5` in the build directory. The host binaries therein are stripped.

5) Making the release available (from github)

  Upload the tarball and md5 hash as a binary file added to a git
  hub. Users may automate downloading by augmenting their cmake `CMakeLists.txt` file with something like:
```
include(FetchContent)
FetchContent_Declare(
    sfpi
    URL https://github.com/$REPO/releases/download/$VERSION/sfpi-release.tgz
    URL_HASH MD5=$HASH
    SOURCE_DIR $INSTALL_LOCATION
)
FetchContent_MakeAvailable(sfpi)
```

where:
* $REPO is the repository containing the release (`tenstorrent/sfpi` for tenstorrent releases)
* $VERSION is the version to download
* $HASH is the md5 hash of the tarball
* $INSTALL_LOCATION is where to place the tarball's contents.

Refer to cmake documentation for further information about
`FetchContent`, `FetchContent_Declare` and
`FetchContent_MakeAvailable`.

8) **WARNING: This section is out of date:**

  Running the GCC test suite
  This is not required for typical build/release cycles, but should likely be
  done if the RISCV code paths are altered or any other signficant
  perturbation is made.

  The following recipe comes from HelpRack w/ some modifications for issues I
  ran into.

  Prerequisites
  The following programs should be installed on your system.
    * dejagnu, libglib2.0-dev, libfdt-dev, libpixman-1-dev, zlib1g-dev, libgtk-3-dev, expect, ninja
  (Note: I installed the above with conda)

8a) Build as above

8b) run
   make check build-sim -j12

This command also runs test suites which you can inspect in the "$SFPI_ROOT/tt-gcc/build-gcc-newlib-stage2/gcc/testsuite/gcc" directory. The files to inspect are "gcc.log" and "gcc.sum"

Preparing the system to support qemu emulation
8c) Create a file named "riscv32-unknown-elf-run" in the $SFPI_ROOT/compiler/bin
directory with the following script:

```
#!/bin/bash
RISC_V_SYSROOT=$SFPI_ROOT/compiler/riscv32-unknown-elf
qemu-args=()
while [[ "$1" != "" ]]
do
    case "$1" in
    -Wq,*) qemu_args+=("$(echo "$1" | cut -d, -f2-)");;
    *) break;;
    esac
    shift
done
xlen="$(readelf -h $1 | grep 'Class' | cut -d: -f 2 | xargs echo |
sed 's/^ELF//')"
qemu-riscv$xlen -r 5.10 "${qemu_args[@]}" -L ${RISC_V_SYSROOT} "$@"
```

8d) Mark this file as executable
  chmod +x riscv32-unknown-elf-run
The above will run some tests and put the results in gcc.log and gcc.sum under $SFPI_ROOT/build-gcc-newlib-stage2/gcc/testsuite/gcc

8e) Set PATH and LD_LIBRARY_PATH
  export PATH=$SFPI_ROOT/compiler/bin:$PATH
  export LD_LIBRARY_PATH=$SFPI_ROOT/compiler/lib:$LD_LIBRARY_PATH

8f) Create a test directory. You can place this directory wherever you want

8g) Copy the dejagnu configuration file from $SFPI_ROOT/tt-gcc/build-gcc-newlib-stage2/gcc/testsuite/gcc/site.exp to your "test" directory

8h) Edit "site.exp" and add the path to target_boards to this file. Append the following line to the
end of the file (expand $SFPI_ROOT manually):
  lappend boards_dir "$SFPI_ROOT/tt-gcc/riscv-dejagnu/baseboards"

8i) To run all the tests in the GCC test suite
  runtest -target_board="riscv-sim/-march=rv32iy/-mabi=ilp32/-mcmodel=medlow" -tool gcc
Or, eg, use "riscv-sim/-march=rv32iy/-mabi=ilp32/-mcmodel=medlow/-mgrayskull"
which runs the SFPU passes and found a couple bugs when first run.

8j) To run a particular test suite, e.g., compile.exp, execute.exp etc.
  runtest -target_board="riscv-sim/-march=rv32iy/-mabi=ilp32/-mcmodel=medlow" -tool gcc execute.exp

8k) To run a single test file eg. gcc.c-torture/execute/fprintf-1.c
  runtest -target_board="riscv-sim/-march=rv32iy/-mabi=ilp32/-mcmodel=medlow" -tool gcc execute.exp=fprintf-1*

Debug tips:
 - use -verbose to see what the heck is going on
 - change $SFPI_ROOT/compiler/share/dejagnu/dg.exp "set keep 0" to "set keep 1" to keep the executable around
 - look in gcc/testsuite/lib/gcc-dg.exp for the OPTIONS lists to whittle down
   and run fewer variations

8l) After running a single test as in 8k, the log file will list the options
to gcc that were used to run the test.  Add the path to the compiler to
compile just that single case.  Some tests are compilation tests and this will
be sufficient, other tests run filters on output or run the simulator to
generate a result.  These are more complicated and take some digging.

8m) check "gcc.log" and "gcc.sum" to view the log and summary of the executed tests, respectively.

8n) check "testrun.log" and "testrun.sum" to view the log and summary of the runtest command.
