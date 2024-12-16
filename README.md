SFPI: Tenstorrent SFPU programming interface
============================================

This repo contains SFPI.

* sfpi header files in `include`
* TT-enhanced RISC-V `binutils` in binutils submodule
* TT-enhanced RISC-V `gcc` in gcc submodule
* standard newlib in `newlib` submodule
* tests in `tests`
* build and release scripts in `scripts`

GCC, Binutils and Newlib are (naturally) released under their own licenses.

The release versioning here is simply an integral version
numbering. The major version /does not/ indicate API breaking
changes. It will be incremented when updating the compiler to a new
upstream version. (There may be other reasons to increment.)

### User Documentation

https://docs.tenstorrent.com/tt-metalium/latest/tt_metal/apis/kernel_apis/sfpu/llk.html

### Building
1) Clone the sfpi repo, & initialize submodules:
```
  git clone git@github.com:tenstorrent-metal/sfpi.git
  git submodule update --init --recursive
```

2) Build the compiler:
```
  scripts/build.sh
```

  This will configure and build using the toplevel `configure` and
  `Makefile.in`, which originate from the RISC-V repo
  (https://github.com/riscv-collab/riscv-gnu-toolchain). The build is
  performed in a `build` subdirectory and a `sfpi/src-hashes` file is
  created there to record the source tree state at the start of a
  build. When making a release, you will want this to match upstream
  committed sources.

  If the build is interrupted, you can of course enter the appropriate
  subdirectory and manually resume after correcting the problem --
  such build would not be suitable for releasing though.

3) Build and running the sfpi tests:

```
    ln -s ../tests build
    CC_PATH=$(pwd)/build/sfpi/compiler make -C build/tests all
    CC_PATH=$(pwd)/build/sfpi/compiler make -C build/tests test
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

8) Running the toolchain test suites:
```
  scripts/build.sh --test
```

This will build qemu and the riscv dejagnu components, and then run the testsuites.

If you just want a binutils or gcc:
```
  scripts/build.sh --test-binutils
```
or
```
  scripts/build.sh --test-gcc
```

9) Running the gcc testsuite with specific options:
```
PATH=$(pwd)/build/sfpi/compiler/bin:$(pwd)/build/test-infra/bin:$PATH \
make -C build/build-gcc-newlib-stage2/gcc check-gcc "RUNTESTFLAGS=--target_board=riscv-sim/-march=rv32i/-mabi=ilp32/-mcmodel=medlow"
```

Alter the value passed to RUNTESTFLAGS as desired, for instance
`riscv-sim/mcpu=ttwh`.  Add `-v` options to get more logging to the
resulting dejagnu log file.
