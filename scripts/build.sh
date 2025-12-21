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
small_build=
incremental=true
BUILD=build
while [ "$#" -ne 0 ] ; do
    case "$1" in
	--checking) gcc_checking=all ;;
	--checking=*) gcc_checking="${1#*=}" ;;
	--dir=*) BUILD="${1#*=}" ;;
	--dejagnu) dejagnu=true ;;
	--full) incremental=false ;;
	--gdb) enable_gdb=--enable-gdb ;;
	--infra) dejagnu=true sim=true ;;
	--serial) NCPUS=1 ;;
	--small) small_build=SMALL_BUILD=1 ;;
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
if [[ "$#" -ne 0 ]]; then
    echo "Unknown argument '$1'" >&2
    exit 2
fi

# figure version, now we know tt_built
if [[ -r $BUILD/version ]]; then
    tt_version=$(cat $BUILD/version)
elif [[ "$tt_version" == "" ]]; then
    # match and exclude are globs, not regexes. This is close enough.
    if ! tt_version=$(git describe --tags --match '[0-9]*.[0-9]*.[0-9]*' \
			  --exclude '*-*' 2>/dev/null); then
	# legacy version numbering with a 'v' prefix.
	tt_version=$(git describe --tags --match 'v[0-9]*.[0-9]*.[0-9]*' \
			 --exclude 'v*-*' 2>/dev/null \
			 | sed 's/^v//' || true)
	if ! [[ $tt_version ]]; then
	    tt_version=0
	fi
    fi
    tagged_head=true
    if echo "$tt_version" | grep -qe '-[0-9]\+-g[0-9a-f]\+$'; then
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
	tt_version="${tt_version%-*-g*}-${branch##*/}"
    fi

    url=$(git config --get remote.origin.url \
	      | sed -e 's/[^:]*://' -e 's+//[^/]*/++' \
		    -e 's+/[^/]*\(\.git\)\?$++')
    if [[ $url != 'tenstorrent' ]]; then
	tt_version="$url-$tt_version"
    fi
fi
echo "INFO: Version: $tt_version"

base_version=${tt_version%%-*}
if [[ $tt_version = $base_version ]]; then
    incremental=false
fi

if [[ -d $BUILD ]]; then
    incremental=false
else
    mkdir -p $BUILD/sfpi
    if $incremental; then
	eval $($BIN/sfpi-info.sh VERSION $base_version )
	if wget -P $BUILD "$sfpi_url/$sfpi_filename.txz"; then
	    tar xJf $BUILD/$sfpi_filename.txz -C $BUILD
	else
	    incremental=false
	fi
    fi

    # extract git hashes for here and each submodule
    $BIN/git-hash.sh "$tt_version" >$BUILD/hashes.pre
    echo $tt_version > $BUILD/version

    # Generate a README.md
    head -n 3 <README.md >$BUILD/sfpi/README.md
    if $incremental; then
	echo Incremental build using $base_version
	echo "**Incremental build using $base_version**" >>$BUILD/sfpi/README.md
    else
	echo Full build
	base_version=
    fi
    echo $base_version >>$BUILD/version
    cat $BUILD/hashes.pre >>$BUILD/sfpi/README.md
    echo >>$BUILD/sfpi/README.md
    sed '/Reporting/,/###/p;d' <README.md | head -n -1 >>$BUILD/sfpi/README.md
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
    if $tt_built; then
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
    if $incremental; then
	(set -x; make -C $BUILD stamps/check-write-permission)
	for file in $(sed -e '/^stamps\/[^c].*-newlib.*:/{s/: .*$//;p}' -e d $BUILD/Makefile)
	do
	    if ! [[ $file =~ -stage2$ ]]; then
		mkdir -p $BUILD/$(dirname $file)
		echo Inremental $file
		echo Incremental >$BUILD/$file
	    fi
	done
    fi
fi

# build the toolchain
(set -x; nice make -C $BUILD -j$NCPUS $small_build)

# maybe make the test infra
if $dejagnu; then
    (set -x; nice make -C $BUILD build-dejagnu -j$NCPUS $small_build)
fi
if $sim; then
    if ! [[ -e qemu ]]; then
	git clone --depth 64 --branch v7.2.15 https://gitlab.com/qemu-project/qemu.git
    fi
    (set -x; nice make -C $BUILD build-sim -j$NCPUS $small_build)
fi

eval $($BIN/sfpi-info.sh VERSION $tt_version)

fails=0
unresolveds=0
errors=0
testing=false
TARGET_BOARDS='riscv-sim/'
tests=$BUILD/tests
if $test_binutils; then
    testing=true
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
    (set -x; SFPI=$(pwd) nice make -C $BUILD -j$NCPUS NEWLIB_TARGET_BOARDS="$TARGET_BOARDS" check-gcc)
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
    (set -x; SFPI=$(pwd) nice make -C $BUILD -j$NCPUS NEWLIB_TARGET_BOARDS="$TARGET_BOARDS" check-gcc-tt)
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

echo "INFO: Version: $tt_version"
if $incremental; then
    echo "INFO: Base: $base_version"
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
