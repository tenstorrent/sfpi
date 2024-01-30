SFPI: Tenstorrent SFPU programming interface
============================================

This repo contains SFPI, its associated tests including an SFPU emulator (complete
for GS, partial for WH and not supported) and a TT enhanced RISC-V gcc which exposes
intrinsics that generate SFPU instructions.

### User Documentation

https://tenstorrent-metal.github.io/tt-metal/latest/tt_metal/apis/kernel_apis/sfpu/llk.html

### Building
1) Clone the sfpi repo and get a branch:
  git clone git@github.com:tenstorrent-metal/sfpi.git

2) Initialize the submodules:
  git submodule update --init --resursive

  The above will pull a bunch of submodules that aren't used (and takes a
  while), alternatively:
    git submodule update --init
    cd tt-gcc
    git submodule update --init

  If you are working on gcc (probably in a submodule such as riscv-gcc), be
  sure to check out an appropriate branch.  The gcc work isn't done on master,
  typically it is done on tt-rel/riscv-gcc-10.2.0.  If you are just compiling,
  then don't bother with this, the --init above should put you at the right
  commit.

3) Configure the compiler:
```
    export SFPI_ROOT=<path to sfpi top level>
    cd tt-gcc
    ./configure --prefix=$SFPI_ROOT/compiler -disable-multilib -with-abi=ilp32 -with-arch=rv32i
```

  SFPI_ROOT must be an absolute path.  Note: the install script and the
  steps below assume the path used above

4) Build the compiler:

  The compiler is based on: https://github.com/riscv-collab/riscv-gnu-toolchain
  see documentation there for installing packages needed to build gcc.

  To build:

```
    make -j <n>
```

  Note: incremental gcc builds are squirrelly, if you aren't sure how to do it
  it is best to build from scratch w/ a "make clean" at the tt-gcc level.

5) Build the tests:

```
    cd $SFPI_ROOT/sfpi/tests
    make all
```

  If this builds, the compiler at least exports the right intrinsics for use
  w/ the synced sfpi.  If it fails to build, there is likely a sync error
  (submodule out of date) between what the compiler is exposing and what sfpi
  is calling.

  The tests in the gcc directory just build some code to be sure gcc is sane.
  The tests in sfpi use the SFPI wrapper and are more complex.

6) Run the tests
```
    cd sfpi
    make test
```

   This will run the built x86/kernels and diff the results against a file in
   gold.  That tests the simulator and sfpi.  It will also build a few files
   and diff the assembly (.S files) against those in gold.  This is likely
   brittle, but provides some sanity that the compiler and sfpi are doing at
   least what they used to do.

7) Create a release
```
    cd $SFPI_ROOT
    bin/release.sh <path to release compiler>
```

  Note: the release script assumes you are overwriting a release (it looks
  for a directory as a sanity check)

  The above will copy lots of files and strip the results which spews tons of
  errors.  This could be cleaned up, but seems to work (strip fails when it
  doesn't recognized the file type).

8) Running the GCC test suite
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
