SFPI: Tenstorrent SFPU programming interface
=============================

This repo contains SFPI, its associated tests including and SFPU emulator and
a TT enhanced RISC-V gcc which exposes intrinsics that generate SFPU
instructions.

### Building
1) Clone the sfpi repo and get a branch:
  git clone https://yyz-gitlab.local.tenstorrent.com/tenstorrent/sfpi

  Work isn't done on master, check out whichever branch you need

2) Initialize the submodules:
  git submodule update --init --resursive

  The above will pull a bunch of submodules that aren't used (and takes a
  while), alternatively:
    git submodule update --init
    cd tt-gcc
    git submodule update --init

3) Configure the compiler:
    export SFPI_ROOT=<path to sfpi top level>
    cd tt-gcc
    ./configure --prefix=$SFPI_ROOT/.. -enable-multilib
    SFPI_ROOT must be an absolute path, I suggest the parent of sfpi

4) Build the compiler:
  The compiler is based on: https://github.com/riscv-collab/riscv-gnu-toolchain
  see documentation there for installing packages needed to build gcc.

  To build:
    make -j <n>

  Note: re-building gcc is squirrelly, if you aren't sure how to do it it is best
  to build from scratch w/ a "make clean".

5) Build the tests:
    cd ..
    make all

  If this builds, the compiler is at least exports the right intrinsics for use
  w/ the synced sfpi.
