#!/usr/bin/env bash

# Build the SFPI toolchain for a different *host* (Canadian cross), e.g.
# riscv64 Linux, on a build machine that already has a native SFPI of
# the same version.
#
#   build  = this machine (e.g. x86_64-linux-gnu)
#   host   = --host triplet, the machine the toolchain will run on
#   target = riscv-tt-elf, as for every SFPI
#
# Only the host programs are compiled: binutils, gcc's driver and
# compilers (all-gcc) and the LTO plugin. Everything that runs on the
# Tensix (newlib, libgcc, libstdc++, crt files, sfpi headers) is
# target code and does not depend on the host, so it is copied from
# the native toolchain given with --native. The native toolchain's
# bin directory is also used as the build->target compiler, as a
# Canadian cross requires.
#
# The gcc configuration is taken verbatim from the native compiler's
# 'Configured with:' line, so both toolchains are configured alike.
#
# Usage:
#   scripts/cross-host.sh --host=riscv64-linux-gnu [--native=build]
#                         [--dir=build-riscv64] [--checking=release]
#                         [--jobs=N] [--force] [--no-release]
#
# --native is a directory containing sfpi/ (a native build directory,
# or an unpacked sfpi_*.txz release). The source tree must be at the
# same commits the native toolchain was built from (checked against
# its README.md unless --force).
#
# Requirements on the build machine: a cross compiler for the host
# (${host}-gcc, ${host}-g++ on PATH), plus the usual build tools
# (bison, flex, m4, gawk). gmp, mpfr and mpc are built in-tree; run
# gcc/contrib/download_prerequisites --no-isl first if gcc/gmp is
# missing.

set -eo pipefail
BIN="$(dirname "$0")"

if ! [[ "$BIN" -ef "scripts" ]]; then
    echo "run this script from repo's top directory" >&2
    exit 1
fi

host=
native=build
BUILD=
checking=
jobs=$(nproc)
force=false
release=true
while [ "$#" -ne 0 ] ; do
    case "$1" in
	--host=*) host="${1#*=}" ;;
	--native=*) native="${1#*=}" ;;
	--dir=*) BUILD="${1#*=}" ;;
	--checking=*) checking="${1#*=}" ;;
	--jobs=*) jobs="${1#*=}" ;;
	--force) force=true ;;
	--no-release) release=false ;;
	-*) echo "Unknown option '$1'" >&2 ; exit 2 ;;
	*) echo "Unknown argument '$1'" >&2 ; exit 2 ;;
    esac
    shift
done

if [[ -z $host ]]; then
    echo "--host=TRIPLET is required (e.g. riscv64-linux-gnu)" >&2
    exit 2
fi
host_arch=${host%%-*}
BUILD=${BUILD:-build-$host_arch}
native=$(realpath "$native")
srcdir=$(pwd)
build_triplet=$(gcc -dumpmachine)

ncc=$native/sfpi/compiler/bin/riscv-tt-elf-gcc
if ! [[ -x $ncc ]]; then
    echo "No native toolchain at $native/sfpi (expected $ncc)" >&2
    exit 1
fi
for tool in ${host}-gcc ${host}-g++ ${host}-ar ${host}-strip; do
    if ! command -v $tool >/dev/null; then
	echo "Host cross tool $tool not found on PATH" >&2
	exit 1
    fi
done
if ! [[ -d gcc/gmp && -d gcc/mpfr && -d gcc/mpc ]]; then
    echo "gcc/gmp, gcc/mpfr or gcc/mpc missing:" \
	 "run (cd gcc && ./contrib/download_prerequisites --no-isl)" >&2
    exit 1
fi

# Same compiler sources as the native toolchain? Its README.md carries
# the git-hash.sh table it was built from: compare the submodule
# commits (the top level only holds scripts and headers, which come
# from the native tree).
mismatch=false
while read -r dir hash; do
    if ! grep -qE "^\| *$dir +\|.*\| *$hash *\|" "$native/sfpi/README.md"; then
	echo "Source $dir at $hash does not match $native/sfpi/README.md" >&2
	mismatch=true
    fi
done < <($BIN/git-hash.sh x | sed -e '/^| \.\/[^ ]* /!d' \
	     -e 's/^| *\([^ ]*\) *|[^|]*|[^|]*| *\([0-9a-f]*\) *|.*/\1 \2/')
if $mismatch && ! $force; then
    echo "Check out the commits the native toolchain was built from, or use --force" >&2
    exit 1
fi

# gcc configure arguments: the native compiler's, minus what we set.
cfg=$($ncc -v 2>&1 | sed -e '/^Configured with: /!d' -e 's/^Configured with: //')
eval "set -- $cfg"
shift # the configure script itself
gcc_args=()
for arg in "$@"; do
    case "$arg" in
	--prefix=*|--src=*|--with-sysroot=*|--build=*|--host=*) ;;
	--enable-checking=*) gcc_args+=("--enable-checking=${checking:-${arg#*=}}") ;;
	*) gcc_args+=("$arg") ;;
    esac
done
pkgversion=
isa_spec=
bugurl=
for arg in "${gcc_args[@]}"; do
    case "$arg" in
	--with-pkgversion=*) pkgversion="$arg" ;;
	--with-isa-spec=*) isa_spec="$arg" ;;
	--with-bugurl=*) bugurl="$arg" ;;
    esac
done

prefix=/sfpi/compiler
stage=$(realpath -m $BUILD/stage)
export LC_ALL=C
export PATH="$native/sfpi/compiler/bin:$PATH"

rm -rf $BUILD
mkdir -p $BUILD/build-binutils $BUILD/build-gcc

if [[ -r $native/version ]]; then
    cp $native/version $BUILD/version
else
    # An unpacked release has no version file: describe this checkout,
    # as build.sh does for a tagged release.
    sfpi_version=$(sed -e '/^Version: /!d' -e 's/^Version: //' -e q $native/sfpi/README.md)
    sfpi_repo=$(git config --get remote.origin.url)
    if [[ $sfpi_repo =~ ^git@ ]]; then
	sfpi_repo=${sfpi_repo/://}
	sfpi_repo=${sfpi_repo/git@/https://}
    fi
    cat <<EOF >$BUILD/version
sfpi_repo='${sfpi_repo%.git}'
sfpi_version='$sfpi_version'
sfpi_base=''
sfpi_build=''
sfpi_hashtype=sha256
EOF
fi
$BIN/git-hash.sh "$(source $BUILD/version; echo $sfpi_version)" >$BUILD/hashes.pre
source $BUILD/version
echo "INFO: Version: $sfpi_version, host $host (build $build_triplet)"

# binutils: flags as in Makefile.in's stamps/build-binutils-newlib.
(cd $BUILD/build-binutils
 set -x
 $srcdir/binutils/configure --build=$build_triplet --host=$host \
     --target=riscv-tt-elf --prefix=$prefix \
     --disable-werror --disable-gdb --disable-gdbserver --disable-sim \
     --disable-libdecnumber --disable-readline --disable-gprofng \
     "$bugurl" "$pkgversion" $isa_spec) >$BUILD/binutils.log 2>&1
(set -x
 nice make -C $BUILD/build-binutils -j$jobs MAKEINFO=true >>$BUILD/binutils.log 2>&1
 make -C $BUILD/build-binutils MAKEINFO=true DESTDIR=$stage install >>$BUILD/binutils.log 2>&1)

# gcc: host programs only. Target libraries come from the native build.
(cd $BUILD/build-gcc
 set -x
 $srcdir/gcc/configure --build=$build_triplet --host=$host \
     --prefix=$prefix --with-sysroot=$prefix/riscv-tt-elf \
     "${gcc_args[@]}") >$BUILD/gcc.log 2>&1
# gcc's selftests run the new compiler on the build machine, which a
# Canadian cross cannot do (and the top-level Makefile does not pass a
# SELFTEST_TARGETS override down), so empty them in gcc/Makefile.
(set -x
 nice make -C $BUILD/build-gcc -j$jobs MAKEINFO=true configure-gcc >>$BUILD/gcc.log 2>&1
 sed -i -e 's/^SELFTEST_TARGETS = .*/SELFTEST_TARGETS =/' $BUILD/build-gcc/gcc/Makefile
 nice make -C $BUILD/build-gcc -j$jobs MAKEINFO=true \
      all-gcc all-lto-plugin >>$BUILD/gcc.log 2>&1
 make -C $BUILD/build-gcc MAKEINFO=true DESTDIR=$stage \
      install-gcc install-lto-plugin >>$BUILD/gcc.log 2>&1)

# Assemble: the native tree (target code, headers), host programs on top.
cp -a $native/sfpi $BUILD/sfpi
cp -a $stage$prefix/. $BUILD/sfpi/compiler/

# Remaining build-machine executables are native extras we did not
# rebuild (simulator, gdb, module mapper): drop them. Anything else is
# a bug in this script.
build_machine=$(file -L "$ncc" | sed -e 's/^[^,]*, \([^,]*\),.*/\1/')
leftovers=$(find $BUILD/sfpi/compiler -type f -exec file {} + \
		| grep -F ", $build_machine," | cut -d: -f1 || true)
for file in $leftovers; do
    case $file in
	*/bin/riscv-tt-elf-run|*/bin/riscv-tt-elf-gdb*|*/g++-mapper-server)
	    echo "INFO: dropping $build_machine-only ${file#$BUILD/sfpi/}"
	    rm -f $file ;;
	*)
	    echo "ERROR: $file is still a $build_machine binary" >&2
	    exit 1 ;;
    esac
done

if $release; then
    (set -x
     SFPI_HOST_ARCH=$host_arch STRIP=${host}-strip \
	 $BIN/release.sh --dir=$BUILD --txz-only)
fi
echo "INFO: Host $host_arch toolchain in $BUILD/sfpi"
