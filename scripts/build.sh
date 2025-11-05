#! /bin/bash

set -eo pipefail
BIN="$(dirname "$0")"

if ! [[ "$BIN" -ef "scripts" ]]; then
    echo "run this script from repo's top directory" 1>&2
    exit 1
fi

NCPUS=$(nproc)

gcc_checking=release
dejagnu=false
enable_gdb=--disable-gdb
sim=false
test_binutils=false
test_gcc=false
test_tt=false
tt_built=false
tt_version=
BUILD=build
while [ "$#" -ne 0 ] ; do
    case "$1" in
	--checking) gcc_checking=all ;;
	--checking=*) gcc_checking="${1#*=}" ;;
	--dir=*) BUILD="${1#*=}" ;;
	--dejagnu) dejagnu=true ;;
	--gdb) enable_gdb=--enable-gdb ;;
	--infra) dejagnu=true sim=true ;;
	--serial) NCPUS=1 ;;
	--test) dejagnu=true sim=true test_gcc=true test_binutils=true ;;
	--test-binutils) dejagnu=true test_binutils=true ;;
	--test-gcc) dejagnu=true sim=true test_gcc=true ;;
	--test-tt) dejagnu=true test_tt=true ;;
	--tt-built) tt_built=true ;;
	--tt-version=*) tt_version="${1#*=}" ;;
	-*) echo "Unknown option '$1'" >&2 ; exit 2 ;;
	*) break ;;
    esac
    shift
done
if [ "$#" -ne 0 ] ; then
    echo "Unknown argument '$1'" >&2
    exit 2
fi

# figure version, now we know tt_built
if [[ -r $BUILD/version ]]; then
    tt_version=$(cat $BUILD/version)
elif [[ "$tt_version" == "" ]]; then
    tt_version=$($BIN/version.sh)
fi
echo "INFO: Version: $tt_version"

if ! [[ -d $BUILD ]]; then
    mkdir -p $BUILD/sfpi
    # extract git hashes for here and each submodule
    $BIN/git-hash.sh "$tt_version" >$BUILD/sfpi/README.txt
    echo $tt_version > $BUILD/version
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
if ! [[ -e $BUILD/Makefile ]]; then
    ident_options=()
    if $tt_built ; then
	# Building at tenstorrent, I guess we're on the hook for it :)
	ident_options=(--with-bugurl='https://github.com/tenstorrent/sfpi'
		       --with-pkgversion="tenstorrent/sfpi:$tt_version")
    else
	ident_options=(--with-bugurl='unsupported'
		       --with-pkgversion="tenstorrent/sfpi-DIY:$tt_version")
    fi
    (cd $BUILD
     set -x
     ../configure --prefix="$(pwd)/sfpi/compiler" "${ident_options[@]}" \
		  --with-mfc=tt \
		  --enable-gcc-checking="$gcc_checking" \
		  --without-system-zlib --without-zstd \
		  --enable-multilib \
		  --with-arch=rv32i --with-abi=ilp32 $enable_gdb)
fi

# build the toolchain
(set -x; nice make -C $BUILD -j$NCPUS)

# maybe make the test infra
if $dejagnu ; then
    (set -x; nice make -C $BUILD build-dejagnu -j$NCPUS)
fi
if $sim ; then
    if ! [[ -e qemu ]]; then
	git clone --depth 64 --branch v7.2.15 https://gitlab.com/qemu-project/qemu.git
    fi
    (set -x; nice make -C $BUILD build-sim -j$NCPUS)
fi

fails=0
unresolveds=0
errors=0
testing=false
TARGET_BOARDS='riscv-sim/'
tests=$BUILD/tests
if $test_binutils ; then
    testing=true
    (set -x; nice make -C $BUILD -j$NCPUS NEWLIB_TARGET_BOARDS="$TARGET_BOARDS" check-binutils)
    mkdir -p $tests
    for sum in $(find $BUILD/build-binutils-newlib -name '*.sum')
    do
	cp ${sum%sum}log $tests
	(set -x; nice $BIN/local-xfails.py --output $tests --xfails xfails $sum)
	fails=$((fails + $(grep -c '^FAIL:' $tests/$(basename $sum) || true)))
	unresolveds=$((unresolveds + $(grep -c '^UNRESOLVED:' $tests/$(basename $sum) || true)))
	errors=$((errors + $(grep -c '^ERROR:' $tests/$(basename $sum) || true)))
    done
fi

if $test_gcc ; then
    testing=true
    test_tt=false
    (set -x; SFPI=$(pwd) nice make -C $BUILD -j$NCPUS NEWLIB_TARGET_BOARDS="$TARGET_BOARDS" check-gcc)
    mkdir -p $tests
    for sum in $(find $BUILD/build-gcc-newlib-stage2 -name '*.sum')
    do
	cp ${sum%sum}log $tests
	(set -x; nice $BIN/local-xfails.py --output $tests --xfails xfails $sum)
	fails=$((fails + $(grep -c '^FAIL:' $tests/$(basename $sum) || true)))
	unresolveds=$((unresolveds + $(grep -c '^UNRESOLVED:' $tests/$(basename $sum) || true)))
	errors=$((errors + $(grep -c '^ERROR:' $tests/$(basename $sum) || true)))
    done
fi

if $test_tt; then
    testing=true
    (set -x; SFPI=$(pwd) nice make -C $BUILD -j$NCPUS NEWLIB_TARGET_BOARDS="$TARGET_BOARDS" check-gcc-tt)
    mkdir -p $tests
    for cc in gcc g++
    do
	cp $BUILD/build-gcc-newlib-stage2/gcc/testsuite/$cc/$cc.{sum,log} $tests
	fails=$((fails + $(grep -c '^FAIL:' $tests/$cc.sum || true)))
	unresolveds=$((unresolveds + $(grep -c '^UNRESOLVED:' $tests/$cc.sum || true)))
	errors=$((errors + $(grep -c '^ERROR:' $tests/$cc.sum || true)))
    done
fi

echo "INFO: Version: $tt_version"

if $testing ; then
    ok=true
    if [[ $errors != 0 ]]; then
	echo "ERROR: $errors tests are borked" >&2
	ok=false
    fi
    if [[ $unresolveds != 0 ]]; then
	echo "ERROR: $unresolveds tests are unresolved" >&2
	ok=false
    fi
    if [[ $fails != 0 ]]; then
	echo "ERROR: $fails tests failed" >&2
	ok=false
    fi
    if ! $ok; then
	exit 1
    fi
    echo "Tests passed. Yay!"
fi
