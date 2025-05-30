#! /bin/bash

set -eo pipefail
BIN="$(dirname "$0")"

if ! test "$BIN" -ef "scripts"; then
    echo "run this script from repo's top directory" 1>&2
    exit 1
fi

NCPUS=$(nproc)

gcc_checking=release
dejagnu=false
sim=false
test_binutils=false
test_gcc=false
test_tt=false
tt_built=false
tt_version=
BUILD=build
if test $(hostname | cut -d- -f-3) = 'tt-metal-dev' ; then
    tt_built=true
fi
multilib='--with-multilib-generator=rv32im_xttwh-ilp32-- rv32im_xttbh-ilp32--'
while [ "$#" -ne 0 ] ; do
    case "$1" in
	--checking) gcc_checking=all ;;
	--checking=*) gcc_checking="${1#*=}" ;;
	--dir=*) BUILD="${1#*=}" ;;
	--dejagnu) dejagnu=true ;;
	--infra) dejagnu=true sim=true ;;
	--monolib) multilib=--disable-multilib ;;
	--serial) NCPUS=1 ;;
	--test) dejagnu=true sim=true test_gcc=true test_binutils=true ;;
	--test-binutils) dejagnu=true test_binutils=true ;;
	--test-gcc) dejagnu=true sim==true test_gcc=true ;;
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
if test -r $BUILD/version ; then
     tt_version=$(cat $BUILD/version)
elif test "$tt_version" ; then
    :
elif $tt_built ; then
    # tt-versioning
    tt_version=$(git describe --tags --match 'v*.*.*')
    tt_version="${tt_version#v}"
    tagged_head=true
    if echo "$tt_version" | grep -qe '-[0-9]\+-g[0-9a-f]\+$' ; then
	tagged_head=false
    fi
    head=$(git rev-parse --symbolic-full-name HEAD)
    branch=
    if test $head = "HEAD" ; then
	# detached head
	if ! $tagged_head ; then
	    # Not tagged, figure out a branch name to add
	    oIFS="$IFS"
	    local= origin=
	    # refs becomes a CSV
	    refs=$(git show -s --pretty=%D HEAD 2>/dev/null | sed 's/HEAD -> //' 's/, /s/g')
	    IFS=,
	    for ref in $refs
	    do
		case $label in
		    # shouldn't happen
		    'tag: *') ;;
		    'origin/*') origin=$ref ;;
		    '*') branch=$ref ;;
		esac
	    done
	    IFS="$oIFS"
	    if test -z "$branch" ; then
		branch="$origin"
	    fi
	fi
    elif ! $tagged_head || test $head != refs/heads/main ; then
	# not tagged or not main, use branch name
	branch=$head
    fi

    if test -n "$branch" ; then
	# just use the final component of a branch name
	tt_version="${tt_version%-*-g*}-${branch##*/}"
    fi
fi
echo "INFO: Version: $tt_version"

if ! test -d $BUILD ; then
    mkdir -p $BUILD/sfpi
    # extract git hashes for here and each submodule
    "$BIN/git-hash.sh" > $BUILD/sfpi/src-hashes
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
if ! test -e $BUILD/Makefile ; then
    ident_options=()
    if $tt_built ; then
	# Building at tenstorrent, I guess we're on the hook for it :)
	ident_options=(--with-bugurl='https://github.com/tenstorrent/sfpi'
		       --with-pkgversion="tenstorrent/sfpi:$tt_version")
    fi
    (cd $BUILD
     set -x
     ../configure --prefix="$(pwd)/sfpi/compiler" "${ident_options[@]}" \
		  --with-mfc=tt \
		  --enable-gcc-checking="$gcc_checking" \
		  --without-system-zlib --without-zstd \
		  "$multilib" \
		  --with-arch=rv32i --with-abi=ilp32 --enable-gdb)
fi

# build the toolchain
(set -x; nice make -C $BUILD -j$NCPUS)

# maybe the test infra
if $dejagnu ; then
    (set -x; nice make -C $BUILD build-dejagnu -j$NCPUS)
fi
if $sim ; then
    (set -x; nice make -C $BUILD build-sim -j$NCPUS)
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

