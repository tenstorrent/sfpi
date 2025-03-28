#! /bin/bash

set -eo pipefail
BIN="$(dirname "$0")"

if ! test "$BIN" -ef "scripts"; then
    echo "run this script from repo's top directory" 1>&2
    exit 1
fi

NCPUS=$(grep -c '^processor' /proc/cpuinfo)
if ! test "$NCPUS" ; then
    NCPUS=1
fi

infra=false
test_gcc=false
test_binutils=false
test_tt=false
gcc_checking=release
BUILD=build
multilib='--with-multilib-generator=rv32i_xttgs-ilp32-- rv32im_xttwh-ilp32-- rv32im_xttbh-ilp32--'
while [ "$#" -ne 0 ] ; do
    case "$1" in
	--serial) NCPUS=1 ;;
	--infra) infra=true ;;
	--test) infra=true test_gcc=true test_binutils=true ;;
	--test-gcc) infra=true test_gcc=true ;;
	--test-tt) infra=true test_tt=true ;;
	--test-binutils) infra=true test_binutils=true ;;
	--checking) gcc_checking=all ;;
	--checking=*) gcc_checking="${1#*=}" ;;
	--monolib) multilib=--disable-multilib ;;
	--dir=*) BUILD="${1#*=}" ;;
	-*) echo "Unknown option '$1'" >&2 ; exit 2 ;;
	*) break ;;
    esac
    shift
done

if [ "$#" -ne 0 ] ; then
    echo "Unknown argument '$1'" >&2
    exit 2
fi

if ! test -d $BUILD ; then
    mkdir -p $BUILD/sfpi
    # extract git hashes for here and each submodule
    "$BIN/git-hash.sh" > $BUILD/sfpi/src-hashes
fi

# Clean the environment
for env in $(printenv) ; do
    var="${env%%=*}"
    case "$var" in
	LC_*) unset $var ;;
    esac
done
export LC_ALL=C

# configure, if this is the first time
if ! test -e $BUILD/Makefile ; then
    bugurl_option=
    if test $(hostname | cut -d- -f-3) = 'tt-metal-dev' ; then
	# Building at tenstorrent, I guess we're on the hook for it :)
	bugurl_option=--with-bugurl='https://github.com/tenstorrent/tt-metal'
    fi
    (cd $BUILD
     set -x
     ../configure --prefix="$(pwd)/sfpi/compiler" $bugurl_option \
		  --enable-gcc-checking="$gcc_checking" \
		  "$multilib" \
		  --with-arch=rv32i --with-abi=ilp32 --enable-gdb)
fi

# build the toolchain
(set -x; nice make -C $BUILD -j$NCPUS)

# maybe the test infra
if $infra ; then
    (set -x; nice make -C $BUILD infra -j$NCPUS)
fi

if $test_binutils ; then
    (set -x; nice make -C $BUILD -j$NCPUS check-binutils)
    for sum in $(find $BUILD/build-binutils-newlib -name '*.sum')
    do
	(set -x; nice $BIN/local-xfails.py --output $BUILD --xfails xfails $sum)
    done
fi

TARGET_BOARDS='riscv-sim/ riscv-sim/cpu=tt-wh riscv-sim/cpu=tt-bh'
if $test_gcc ; then
    test_tt=false
   (set -x; SFPI=$(pwd) nice make -C $BUILD -j$NCPUS NEWLIB_TARGET_BOARDS="$TARGET_BOARDS" check-gcc)
    for sum in $(find $BUILD/build-gcc-newlib-stage2 -name '*.sum')
    do
	(set -x; nice $BIN/local-xfails.py --output $BUILD --xfails xfails $sum)
    done
fi

if $test_tt; then
    (set -x; SFPI=$(pwd) nice make -C $BUILD -j$NCPUS NEWLIB_TARGET_BOARDS="$TARGET_BOARDS" check-gcc-tt)
    (set -x; cp $BUILD/build-gcc-newlib-stage2/gcc/testsuite/gcc/gcc.sum $BUILD)
    (set -x; cp $BUILD/build-gcc-newlib-stage2/gcc/testsuite/g++/g++.sum $BUILD)
fi

