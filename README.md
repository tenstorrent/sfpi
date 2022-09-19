SFPI: Tenstorrent SFPU programming interface
=============================

This repo contains SFPI, its associated tests including and SFPU emulator and
a TT enhanced RISC-V gcc which exposes intrinsics that generate SFPU
instructions.

### User Documentation

http://yyz-webservice-02.local.tenstorrent.com/docs/pybuda-docs/index.html

### Internals (implementation) Documentation

https://yyz-gitlab.local.tenstorrent.com/tenstorrent/sfpi/-/wikis/SFPI

### Building
1) Clone the sfpi repo and get a branch:
  git clone https://yyz-gitlab.local.tenstorrent.com/tenstorrent/sfpi

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
