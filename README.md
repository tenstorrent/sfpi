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

### Reporting a Bug

**For any issues with this software please file an issue at
`https://github.com/tenstorrent/tt-metal`, and mark it with an `sfpi`
label. Do not try an dfile a report in this repo.**

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

https://docs.tenstorrent.com/tt-metalium/latest/tt_metal/apis/kernel_apis/sfpu/llk.html

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
  with `.deb` or `.rpm` packages. Also a `.hash` file is created.

5) Making the release available (from github)

  Create an `sfpi-version` file from the hash files generated during
  the release process (you may have several, by building and
  releasing on several hosts):

```
  scripts/sfpi-info.sh CREATE [$DIRS]
```

  Where `$DIRS` are the directories containing the `.hash` files.  You
  will probably have to edit the created file to adjust the sfpi_url
  value.

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
reason. The post processed files are placed in a `build/tests` directory,
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
