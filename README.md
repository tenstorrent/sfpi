SFPI: Tenstorrent SFPU programming interface
============================================

This repo contains SFPI.

* sfpi header files in `include`
* TT-enhanced RISC-V `binutils` in binutils submodule
* TT-enhanced RISC-V `gcc` in gcc submodule
* standard newlib in `newlib` submodule
* standard qemu cloned on demand
* RiscV dejagnu harness in `riscv-dejagnu` submodule
* tests in `tests`
* build and release scripts in `scripts`

GCC, Binutils, Newlib, Qemu and Dejagnu are (naturally) released under
their own licenses.

The release versioning here is simply an integral version
numbering. The major version /does not/ indicate API breaking
changes. It will be incremented when updating the compiler to a new
upstream version. (There may be other reasons to increment.)

**For any issues with this software please file an issue at
`https://github.com/tenstorrent/tt-metal`, and mark it with an `sfpi`
label.**

If you are reporting an internal compiler (ICE) error when using
`tt-metal`, please follow this procedure to obtain a reproducible test
case:

* Enable map file generation: `export TT_METAL_KERNEL_MAP=1`
* Run your program or test, capturing the output: `pytest ... |& tee bug.log`
* Copy the log file: `cp bug.log ~/.cache/tt-metal-cache`
* Create a tarball of `tt-metal-cache`: `tar cf bug.tar -C ~/.cache tt-metal-cache`
* Attach that tarball to your bug report.
* Please describe what the bug is.

### User Documentation

https://docs.tenstorrent.com/tt-metalium/latest/tt_metal/apis/kernel_apis/sfpu/llk.html

### Obtaining Full Source

The Github-provided source tarballs (sfpi-$VERSION.tar.gz) do not
contain the submodule source code. To obtain the full sources:

* Clone the sfpi repo: `git clone https://github.com/tenstorrent/sfpi.git`
* Enter the repo: `cd sfpi`
* Checkout the release using the tag: `git checkout $VERSION`
* Update the submodules: `git submodule update --init --recursive`

In the binary releases, you may examing `sfpi/README.txt`, which lists
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
  (https://github.com/riscv-collab/riscv-gnu-toolchain). The build is
  performed in a `build` subdirectory and a `sfpi/src-hashes` file is
  created there to record the source tree state at the start of a
  build. When making a release, you will want this to match upstream
  committed sources. If you want to build in a different subdirectory
  use the `--dir=$DIR` option.

  You may add a `--checking=VALUE` option to control gcc's checking --
  see gcc's documentation.  The default is `release`. Note this does
  not control how gcc itself is optimized (which is usually `-O2`).

  If the build is interrupted, you can of course enter the appropriate
  subdirectory and manually resume after correcting the problem --
  such build would not be suitable for releasing though.

  See below about the various `--test` options that may also be used.

3) Build and running the local sfpi tests:

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

  * Update the `version` file to the new version number. That version
  will be augmented by the git branch name (the last component of a
  `/`-separated branch name), unless you're on `main`. This is
  recorded in a `version` file placed in the build directory.

  * Then build a release as described above.

  * To create the release artifacts:

```
  scripts/release.sh
```

  The same `--dir=$DIR` option as the build script is accepted. It
  will verify the source hashes are unchanged from when the build
  started. You may override this check with the `--force` option, but
  /be careful/.

  A `.txz` tarball will be created, along with `.deb` and/or `.rpm`
  packages, if the necessary packagers are available. (GEM utility FPM
  is used.) Also a `.md5` checksum file is created. The host binaries therein
  are stripped. The package dependencies are determined by looking at
  executable shared object requirements and inferring the packages
  from that.

5) Making the release available (from github)

  Upload the release files and md5 hash as a binary file added to a
  git hub. You'll want to set the version tag to be the same as the
  version string created during the build process.

  Users may automate downloading by augmenting their cmake `CMakeLists.txt`
  file with something like:
```
include(FetchContent)
FetchContent_Declare(
    sfpi
    URL https://github.com/$REPO/releases/download/$VERSION/sfpi-$VERSION-x86_64_Linux.txz
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

A more sophisticated mechanism is used by Tenstorent's `tt-metal` repo
-- see `tt_metal/sfpi-version.sh` and its uses in
`tt_metal/hw/CMakeLists.txt` & `install_dependencies.sh`.

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

After the dejagnu tests have executed, the summary files (`$tool.sum`)
are post processed using local xfail files in the `xfails`
directory. This filters out additional fails that are due to
limitations of the test environment or deemed expected for some other
reason. The post processed files are placed in the `build` directory,
the originals are left unchanged.

Note that these dejagnu test runs are idempotent. If you want to
repeat a test run you will need to delete the stamp file in
`build/stamps` (`check-binutils-newlib` or `check-gcc-newlib`). Note
that the post processing is run each time, and thus the processed
summary files will change if the xfail files are adjusted.

9) Running the TT-specific parts of the toolchain tests.
```
  scripts/build.sh --test-tt
```

This will run just the tt-specific subdirectories of the compiler
testsuites.  The summary files are copied to the build directory /but/
are not post processed as described above. Unlike running the full
testsuite, this operation is /not/ idempotent -- there is no need to
delete a stamp file to rerun them.

10) Running the gcc testsuite with specific options:
```
PATH=$(pwd)/build/sfpi/compiler/bin:$(pwd)/build/infra/bin:$PATH \
make -C build/build-gcc-newlib-stage2/gcc check-gcc \
    "RUNTESTFLAGS=--target_board=riscv-sim/mcpu=tt-bh"
```

Alter the value passed to RUNTESTFLAGS as desired, for instance
`riscv-sim/mcpu=tt-wh`.  Add `-v` options to get more logging to the
resulting dejagnu log file.
