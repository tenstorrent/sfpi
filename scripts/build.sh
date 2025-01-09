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
gcc_checking=release
multilib='--with-multilib-generator=rv32i_xttgs-ilp32-- rv32im_xttwh-ilp32-- rv32im_xttbh-ilp32--'
while [ "$#" -ne 0 ] ; do
    case "$1" in
	--serial) NCPUS=1 ;;
	--infra) infra=true ;;
	--test) infra=true test_gcc=true test_binutils=true ;;
	--test-gcc) infra=true test_gcc=true ;;
	--test-binutils) infra=true test_binutils=true ;;
	# There's an RTL abuse such that =all compiler breaks so bad :(
	--checking) gcc_checking=yes ;;
	--checking=*) gcc_checking="${1#*=}" ;;
	--monolib) multilib=--disable-multilib ;;
	-*) echo "Unknown option '$1'" >&2 ; exit 2 ;;
	*) break ;;
    esac
    shift
done

if [ "$#" -ne 0 ] ; then
    echo "Unknown argument '$1'" >&2
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
    bugurl_option=
    if test $(hostname | cut -d- -f-3) = 'tt-metal-dev' ; then
	# Building at tenstorrent, I guess we're on the hook for it :)
	bugurl_option=--with-bugurl='https://github.com/tenstorrent/tt-metal'
    fi
    (cd build
     set -x
     ../configure --prefix="$(pwd)/sfpi/compiler" $bugurl_option \
		  --enable-gcc-checking="$gcc_checking" \
		  "$multilib" \
		  --with-arch=rv32i --with-abi=ilp32 --disable-gdb)
fi

# build the toolchain
(set -x; nice make -C build -j$NCPUS)

# maybe the test infra
if $infra ; then
    (set -x; nice make -C build infra -j$NCPUS)
fi

if $test_binutils ; then
    (set -x; nice make -C build check-binutils -j$NCPUS)
    for sum in $(find build/build-binutils-newlib -name '*.sum')
    do
	(set -x; nice $BIN/local-xfails.py --output build --xfails xfails $sum)
    done
fi

if $test_gcc ; then
   (set -x; nice make -C build check-gcc -j$NCPUS)
    for sum in $(find build/build-gcc-newlib-stage2 -name '*.sum')
    do
	(set -x; nice $BIN/local-xfails.py --output build --xfails xfails $sum)
    done
fi

