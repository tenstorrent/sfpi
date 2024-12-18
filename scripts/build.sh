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
if ! test -e build/Makefile ; then
    (cd build
     set -x
     ../configure --prefix="$(pwd)/sfpi/compiler" \
		  --with-multilib-generator='rv32i_xttgs-ilp32-- rv32im_xttwh-ilp32-- rv32im_xttbh-ilp32--' \
		  --with-arch=rv32i --with-abi=ilp32 --disable-gdb)
fi

# build the toolchain
(set -x; nice make -C build -j$NCPUS)

# maybe the test infra
if $infra ; then
    (set -x; nice make -C build infra -j$NCPUS)
fi

# maybe test
if $test ; then
    (set -x; nice make -C build check -j$NCPUS)
fi

if $test_binutils ; then
    (set -x; nice make -C build check-binutils -j$NCPUS)
fi

if $test_gcc ; then
   (set -x; nice make -C build check-gcc -j$NCPUS)
fi
