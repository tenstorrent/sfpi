#!/usr/bin/env bash

set -eo pipefail
BIN="$(dirname "$0")"

if ! [[ "$BIN" -ef "scripts" ]]; then
    echo "run this script from repo's top directory" 1>&2
    exit 1
fi

NCPUS=$(nproc)

binutils=false
dejagnu=false
enable_gdb=--disable-gdb
gcc_checking=assert,df,extra,gimple,misc,rtl,rtlflag,tree
cross_arch=
full=
native=
sfpi_base=*
sfpi_build=
sfpi_version=
sim=false
small_build=
test_binutils=false
test_gcc=false
test_tt=false
tt_built=false

BUILD=
while [ "$#" -ne 0 ] ; do
    case "$1" in
	--binutils) binutils=true ;;
	--build-id=*) sfpi_build="${1#*=}"
		      if ! [[ $sfpi_build =~ ^[0-9]+$ ]]; then
			  echo "$1 is not a decimal number" >&2
			  exit 1
		      fi
		      ;;
	--checking=*) gcc_checking="${1#*=}" ;;
	--cross-arch=*) cross_arch="${1#*=}" ;;
	--dejagnu) dejagnu=true ;;
	--dir=*) BUILD="${1#*=}" ;;
	--full) sfpi_base= full=true ;;
	--gdb) enable_gdb=--enable-gdb ;;
	--infra) dejagnu=true sim=true ;;
	--native=*) native="${1#*=}" ;;
	--serial) NCPUS=1 ;;
	--small) small_build=SMALL_BUILD=1 ;;
	--test) dejagnu=true sim=true test_gcc=true test_binutils=true ;;
	--test-binutils) dejagnu=true test_binutils=true ;;
	--test-gcc) dejagnu=true sim=true test_gcc=true ;;
	--test-tt) dejagnu=true test_tt=true ;;
	--tt-built) tt_built=true ;;
	--version=*) sfpi_version="${1#*=}" ;;
	-*) echo "Unknown option '$1'" >&2 ; exit 2 ;;
	*) break ;;
    esac
    shift
done
if [[ "$#" -ne 0 ]]; then
    echo "Unknown argument '$1'" >&2
    exit 2
fi
BUILD=${BUILD:-build${cross_arch:+-$cross_arch}}

# figure version, now we know tt_built
if [[ -r $BUILD/version ]]; then
    source $BUILD/version
elif [[ "$sfpi_version" == "" ]]; then
    # match and exclude are globs, not regexes. This is close enough.
    if ! sfpi_version=$(git describe --tags --match '[0-9]*.[0-9]*.[0-9]*' \
			  --exclude '*-*' 2>/dev/null); then
	# legacy version numbering with a 'v' prefix.
	sfpi_version=$(git describe --tags --match 'v[0-9]*.[0-9]*.[0-9]*' \
			 --exclude 'v*-*' 2>/dev/null \
			 | sed 's/^v//' || true)
	if ! [[ $sfpi_version ]]; then
	    sfpi_version=0
	fi
    fi
    tagged_head=true
    if echo "$sfpi_version" | grep -qe '-[0-9]\+-g[0-9a-f]\+$'; then
	tagged_head=false
    fi
    head=$(git rev-parse --symbolic-full-name HEAD)
    branch=
    if [[ $head = "HEAD" ]]; then
	# detached head
	if ! $tagged_head; then
	    # Not tagged, figure out a branch name to add
	    origin=
	    refs=$(git show -s --pretty=%D HEAD 2>/dev/null \
		       | sed -e 's/^HEAD -> //' -e 's/ //g' -e 's/,/ /g' \
		    || true)
	    for ref in $refs
	    do
		case $ref in
		    # shouldn't happen
		    tag:*) ;;
		    HEAD) ;;
		    origin/*) origin=${ref#origin/} ;;
		    *) branch=$ref ;;
		esac
	    done
	    if [[ -z $branch ]]; then
		branch="$origin"
	    fi
	fi
    elif ! $tagged_head || [[ $head != refs/heads/main ]]; then
	# not tagged or not main, use branch name
	branch=${head#refs/heads/}
    fi

    if [[ -n "$branch" ]]; then
	# just use the final component of a branch name
	sfpi_version="${sfpi_version%-*-g*}-${branch##*/}"
    fi

    url=$(git config --get remote.origin.url \
	      | sed -e 's/[^:]*://' -e 's+//[^/]*/++' \
		    -e 's+/[^/]*\(\.git\)\?$++')
    if [[ $url != 'tenstorrent' ]]; then
	sfpi_version="$url-$sfpi_version"
    fi
fi
echo "INFO: Version: $sfpi_version"

if [[ -n $sfpi_base ]]; then
    sfpi_base=${sfpi_version%%-*}
    if [[ $sfpi_version = $sfpi_base ]]; then
	sfpi_base=
    fi
fi

# A toolchain for another arch (--cross-arch) is a Canadian cross:
# built here, it runs on $cross_arch Linux and targets the Tensix.
# That needs a toolchain of the same kind that runs here, the native
# toolchain, both to compile the target libraries and to supply
# newlib, as an incremental build takes it from a base release.
# --native names its .txz (or an unpacked sfpi directory). By default
# the most recent native release is used if it is the same gcc as
# these sources, else a native toolchain is built first.
if [[ -r $BUILD/cross ]]; then
    recorded=$(source $BUILD/cross; echo $sfpi_cross_arch)
    if [[ -n $cross_arch && $cross_arch != "$recorded" ]]; then
	echo "$BUILD was configured for $recorded, not $cross_arch" >&2
	exit 1
    fi
    source $BUILD/cross
    cross_arch=$sfpi_cross_arch
elif [[ -n $cross_arch && -d $BUILD ]]; then
    echo "$BUILD was configured for this machine, use another --dir for $cross_arch" >&2
    exit 1
fi
if [[ -n $native && -z $cross_arch ]]; then
    echo "--native is only needed with --cross-arch" >&2
    exit 2
fi
cross_host=
if [[ -n $cross_arch ]]; then
    cross_host=$cross_arch-linux-gnu
    if $dejagnu || $sim || $test_binutils || $test_gcc || $test_tt; then
	echo "A toolchain for $cross_arch cannot be tested on this machine" >&2
	exit 2
    fi
    if [[ $enable_gdb != --disable-gdb ]]; then
	echo "INFO: No gdb for $cross_arch"
	enable_gdb=--disable-gdb
    fi
    for tool in gcc g++ ar ranlib strip; do
	if ! command -v $cross_host-$tool >/dev/null; then
	    echo "No $cross_host-$tool: a cross compiler for $cross_arch is needed" >&2
	    exit 1
	fi
    done
    echo "INFO: Cross: $cross_arch ($cross_host)"
fi

# URL of release $1's tarball for this machine
release_url () {
    sed -e "/^sfpi_base=/s/=.*/='$1'/" $BUILD/version \
	| $BIN/sfpi-info.sh BASE \
	| (eval "$(cat)"; echo "$sfpi_url/$sfpi_filename.txz")
}

# Generate $BUILD/sfpi/README.md, noting $1 about how it was built
write_readme () {
    head -n 3 <README.md >$BUILD/sfpi/README.md
    if [[ -n $1 ]]; then
	echo "**$1**" >>$BUILD/sfpi/README.md
    fi
    cat $BUILD/hashes.pre >>$BUILD/sfpi/README.md
    echo >>$BUILD/sfpi/README.md
    sed '/Reporting/,/###/p;d' <README.md | head -n -1 >>$BUILD/sfpi/README.md
}

# Configure $BUILD, adding options "$@"
configure_build () {
    local ident_options=()
    if $tt_built; then
	# Building at tenstorrent, I guess we're on the hook for it :)
	ident_options=(--with-bugurl='https://github.com/tenstorrent/sfpi'
		       --with-pkgversion="tenstorrent/sfpi:$sfpi_version${sfpi_build:+[}$sfpi_build${sfpi_build:+]}")
    else
	ident_options=(--with-bugurl='unsupported'
		       --with-pkgversion="tenstorrent/sfpi-DIY:$sfpi_version${sfpi_build:+[}$sfpi_build${sfpi_build:+]}")
    fi
    (srcdir=$(pwd)
     cd $BUILD
     set -x
     $srcdir/configure --srcdir=$srcdir \
		       --prefix="$(pwd)/sfpi" "${ident_options[@]}" \
		       --with-mfc=tt \
		       --enable-gcc-checking="$gcc_checking" \
		       --without-system-zlib --without-zstd \
		       --enable-multilib \
		       --with-arch=rv32i --with-abi=ilp32 $enable_gdb "$@")
}

# Mark the build steps whose results are already in $BUILD/sfpi (from
# a base release) as done. gcc's stage2 is always built, and binutils
# too with --binutils.
stamp_prebuilt () {
    (set -x; make -C $BUILD stamps/check-write-permission)
    for file in $(sed -e '/^stamps\/[^c].*-newlib.*:/{s/: .*$//;p}' -e d $BUILD/Makefile)
    do
	if [[ -f $file ]] ; then
	    :
	elif $binutils && [[ $file =~ -binutils- ]]; then
	    (set -x; make -C $BUILD -j$NCPUS $small_build $file)
	elif ! [[ $file =~ -stage2$ ]]; then
	    mkdir -p $BUILD/$(dirname $file)
	    echo Inremental $file
	    echo Incremental >$BUILD/$file
	fi
    done
}

# Find or make the native toolchain for a cross build, setting
# sfpi_native and native_version
native_toolchain () {
    local dir=$BUILD/native
    local src=$native
    local want=$(cat gcc/gcc/BASE-VER)
    mkdir -p $dir
    if [[ -z $src ]]; then
	local release=$(git describe --tags --abbrev=0 \
			    --match '[0-9]*.[0-9]*.[0-9]*' --exclude '*-*' 2>/dev/null || true)
	if [[ -n $release ]]; then
	    local url=$(release_url $release)
	    echo "INFO: Native toolchain: $url"
	    if wget -nv -P $dir "$url"; then
		src=$dir/${url##*/}
	    fi
	fi
    elif [[ -d $src ]] && ! [[ -d $src/sfpi || -d $src/compiler ]]; then
	# A directory of release files, such as a downloaded CI artifact
	local txz=$(eval $($BIN/sfpi-info.sh DIST </dev/null); \
		    ls $src/sfpi_*_${sfpi_host_arch}_*.txz 2>/dev/null)
	if [[ $(echo "$txz" | wc -w) != 1 ]]; then
	    echo "Expected one native .txz in $src, found: ${txz:-none}" >&2
	    exit 1
	fi
	src=$txz
    fi
    sfpi_native=
    if [[ -n $src ]]; then
	if [[ -f $src ]]; then
	    tar xJf $src -C $dir
	    sfpi_native=$dir/sfpi
	elif [[ -d $src/sfpi/compiler ]]; then
	    sfpi_native=$src/sfpi
	elif [[ -d $src/compiler ]]; then
	    sfpi_native=$src
	else
	    echo "$src is not an sfpi toolchain" >&2
	    exit 1
	fi
    fi
    if [[ -n $sfpi_native ]]; then
	sfpi_native=$(realpath $sfpi_native)
	local have=$($sfpi_native/compiler/bin/riscv-tt-elf-gcc -dumpversion 2>/dev/null || true)
	if [[ -z $have ]]; then
	    echo "$sfpi_native does not run on this machine" >&2
	    exit 1
	elif [[ $have != "$want" ]]; then
	    # It cannot compile these sources' target libraries
	    if [[ -n $native ]]; then
		echo "$native is gcc $have, these sources are gcc $want" >&2
		exit 1
	    fi
	    echo "INFO: Native release is gcc $have, these sources are gcc $want"
	    sfpi_native=
	fi
    fi
    if [[ -z $sfpi_native ]]; then
	# No usable release: build the native toolchain from these sources
	echo "INFO: Building a native toolchain in $BUILD/native-build"
	(set -x
	 $BIN/build.sh --dir=$BUILD/native-build --checking=$gcc_checking \
		       ${small_build:+--small} ${full:+--full} \
		       $($tt_built && echo --tt-built) \
		       ${sfpi_build:+--build-id=$sfpi_build})
	sfpi_native=$(realpath $BUILD/native-build/sfpi)
    fi
    native_version=$(sed -e '/^Version: /!d' -e 's///' -e q $sfpi_native/README.md)
    echo "INFO: Native: $native_version ($sfpi_native)"
}

if ! [[ -d $BUILD ]]; then
    mkdir -p $BUILD/sfpi
    sfpi_repo=$(git config --get remote.origin.url)
    if [[ $sfpi_repo =~ ^git@ ]]; then
	sfpi_repo=${sfpi_repo/://}
	sfpi_repo=${sfpi_repo/git@/https://}
    fi
    cat <<EOF >$BUILD/version
sfpi_repo='${sfpi_repo%.git}'
sfpi_version='$sfpi_version'
sfpi_base='$sfpi_base'
sfpi_build='$sfpi_build'
sfpi_hashtype=sha256
EOF
    if [[ -n $cross_arch ]]; then
	native_toolchain
	cat <<EOF >$BUILD/cross
sfpi_cross_arch='$cross_arch'
sfpi_native='$sfpi_native'
EOF
	# Take newlib from the native toolchain, as from a base release.
	# Everything else is built for $cross_arch.
	mkdir -p $BUILD/sfpi/compiler
	cp -a $sfpi_native/compiler/riscv-tt-elf $BUILD/sfpi/compiler/
    elif [[ -n $sfpi_base ]]; then
	url=$(release_url $sfpi_base)
	if wget -P $BUILD "$url"; then
	    tar xJf $BUILD/${url##*/} -C $BUILD
	    # Make sure we renew the sfpi headers
	    rm -rf $BUILD/sfpi/include
	else
	    # No such base available
	    sfpi_base=
	    sed -i '/sfpi_base=/s/=.*/=/' $BUILD/version
	fi
    fi

    # extract git hashes for here and each submodule
    $BIN/git-hash.sh "$sfpi_version" >$BUILD/hashes.pre

    note=
    if [[ -n $cross_arch ]]; then
	note="Toolchain for $cross_arch Linux, newlib from $native_version"
    elif [[ -n $sfpi_base ]]; then
	note="Incremental build using $sfpi_base"
	echo $note
    fi
    write_readme "$note"
fi

# Clean the environment
for env in $(printenv) ; do
    var="${env%%=*}"
    case "$var" in
	LC_*) unset $var ;;
    esac
done
export LC_ALL=C

if [[ -n $cross_arch ]]; then
    # Build->target tools, to compile the target libraries. Its driver
    # insists on -mcpu= unless given a -B, as the in-tree one always is.
    export PATH="$sfpi_native/compiler/bin:$PATH"
    native_B="-B$sfpi_native/compiler/riscv-tt-elf/bin/"
    export CC_FOR_TARGET="riscv-tt-elf-gcc $native_B"
    export CXX_FOR_TARGET="riscv-tt-elf-g++ $native_B"
    export GCC_FOR_TARGET="$CC_FOR_TARGET"
    # gmp, mpfr & mpc for $cross_arch are built in-tree
    if ! [[ -e gcc/gmp && -e gcc/mpfr && -e gcc/mpc ]]; then
	(set -x; cd gcc && ./contrib/download_prerequisites --no-isl)
    fi
fi

# configure, if this is the first time
if ! [[ -e $BUILD/Makefile ]]; then
    if [[ -n $cross_arch ]]; then
	configure_build --with-host=$cross_host
	# binutils and gcc are built for $cross_arch, newlib is native's
	binutils=true
	stamp_prebuilt
    else
	configure_build
	if [[ -n $sfpi_base ]]; then
	    stamp_prebuilt
	fi
    fi
fi

# build the toolchain
(set -x; nice make -C $BUILD -j$NCPUS $small_build)

if [[ -n $cross_arch ]]; then
    # Every program must have been built for $cross_arch
    native_kind=$(file -L $sfpi_native/compiler/bin/riscv-tt-elf-gcc \
		      | sed -e 's/^[^,]*, \([^,]*\),.*/\1/')
    leftovers=$(find $BUILD/sfpi/compiler -type f -exec file {} + \
		    | grep -F ", $native_kind," | cut -d: -f1 || true)
    if [[ -n $leftovers ]]; then
	echo "ERROR: Not built for $cross_arch:" $leftovers >&2
	exit 1
    fi
fi

# maybe make the test infra
dejagnu=false
if $dejagnu; then
    (set -x; nice make -C $BUILD build-dejagnu -j$NCPUS $small_build)
fi
if $sim; then
    if ! [[ -e qemu ]]; then
	git clone --depth 64 --branch v7.2.15 https://gitlab.com/qemu-project/qemu.git
    fi
    (set -x; nice make -C $BUILD build-sim -j$NCPUS $small_build)
fi

fails=0
unresolveds=0
errors=0
testing=false
TARGET_BOARDS='riscv-sim/'
export DEJAGNU=$(realpath $BUILD)/dejagnu.exp
if ! [[ -f $BIN/dejagnu.exp ]]; then
    echo "lappend boards_dir \"$(realpath $BIN)\"" > $BUILD/dejagnu.exp
fi
tests=$BUILD/tests
eval $($BIN/sfpi-info.sh DIST <$BUILD/version)

if $test_binutils; then
    testing=true
    rm -f $BUILD/stamps/check-binutils-newlib
    (set -x; nice make -C $BUILD -j$NCPUS NEWLIB_TARGET_BOARDS="$TARGET_BOARDS" check-binutils)
    mkdir -p $tests
    for sum in $(find $BUILD/build-binutils-newlib -name '*.sum')
    do
	dst=$(basename -s .sum $sum)-${sfpi_arch}_${sfpi_dist}
	cp ${sum%sum}log $tests/$dst.log
	(set -x; nice $BIN/local-xfails.py --output $tests/$dst.sum --xfails xfails $sum)
	fails=$((fails + $(grep -c '^FAIL:' $tests/$dst.sum || true)))
	unresolveds=$((unresolveds + $(grep -c '^UNRESOLVED:' $tests/$dst.sum || true)))
	errors=$((errors + $(grep -c '^ERROR:' $tests/$dst.sum || true)))
    done
fi

if $test_gcc; then
    testing=true
    test_tt=false
    (set -x; SFPI=$(realpath $BUILD)/sfpi nice make -C $BUILD -j$NCPUS NEWLIB_TARGET_BOARDS="$TARGET_BOARDS" check-gcc)
    mkdir -p $tests
    for sum in $(find $BUILD/build-gcc-newlib-stage2 -name '*.sum')
    do
	dst=$(basename -s .sum $sum)-${sfpi_arch}_${sfpi_dist}
	cp ${sum%sum}log $tests/$dst.log
	(set -x; nice $BIN/local-xfails.py --output $tests/$dst.sum --xfails xfails $sum)
	fails=$((fails + $(grep -c '^FAIL:' $tests/$dst.sum || true)))
	unresolveds=$((unresolveds + $(grep -c '^UNRESOLVED:' $tests/$dst.sum || true)))
	errors=$((errors + $(grep -c '^ERROR:' $tests/$dst.sum || true)))
    done
fi

if $test_tt; then
    testing=true
    (set -x; SFPI=$(realpath $BUILD)/sfpi nice make -C $BUILD -j$NCPUS NEWLIB_TARGET_BOARDS="$TARGET_BOARDS" check-gcc-tt)
    mkdir -p $tests
    for cc in gcc g++
    do
	dst=$cc-${sfpi_arch}_${sfpi_dist}
	cp $BUILD/build-gcc-newlib-stage2/gcc/testsuite/$cc/$cc.sum $tests/$dst.sum
	cp $BUILD/build-gcc-newlib-stage2/gcc/testsuite/$cc/$cc.log $tests/$dst.log
	fails=$((fails + $(grep -c '^FAIL:' $tests/$dst.sum || true)))
	unresolveds=$((unresolveds + $(grep -c '^UNRESOLVED:' $tests/$dst.sum || true)))
	errors=$((errors + $(grep -c '^ERROR:' $tests/$dst.sum || true)))
    done
fi

echo "INFO: Version: $sfpi_version"
if [[ -n $sfpi_base ]]; then
    echo "INFO: Base: $sfpi_base"
fi
if [[ -n $sfpi_build ]]; then
    echo "INFO: Build: $sfpi_build"
fi

if $testing; then
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
