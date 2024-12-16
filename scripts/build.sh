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
test=false
test_gcc=false
test_binutils=false
while [ "$#" -ne 0 ] ; do
    case "$1" in
	--serial) NCPUS=1 ;;
	--infra) infra=true ;;
	--test) infra=true ; test=true ;;
	--test-gcc) infra=true ; test=false ; test_gcc=true ;;
	--test-binutils) infra=true ; test=false ; test_binutils=true ;;
	-*) echo "Unknown option '$1'" >2 ; exit 2 ;;
	*) break ;;
    esac
    shift
done

if [ "$#" -ne 0 ] ; then
    echo "Unknown argument '$1'" >2
    exit 2
fi

if ! test -d build ; then
    mkdir -p build/sfpi
    # extract git hashes for here and each submodule
    "$BIN/git-hash.sh" > build/sfpi/src-hashes
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
test -e build/Makefile ||
    (cd build ; ../configure --prefix="$(pwd)/sfpi/compiler" --disable-multilib --with-arch=rv32i --with-abi=ilp32 --disable-gdb)

# build the toolchain
nice make -C build -j$NCPUS

# maybe the test infra
$infra && nice make -C build infra -j$NCPUS

# maybe test
$test && nice make -C build check -j$NCPUS

$test_binutils && nice make -C build check-binutils -j$NCPUS

$test_gcc && nice make -C build check-gcc -j$NCPUS
