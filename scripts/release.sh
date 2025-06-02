#! /bin/bash

set -eo pipefail
BIN="$(dirname "$0")"

if ! test "$BIN" -ef "scripts"; then
    echo "run this script from repo's top directory" 1>&2
    exit 1
fi

BUILD=build
ci=false
force=false
tt_built=false
if test $(hostname | cut -d- -f-3) = 'tt-metal-dev' ; then
    tt_built=true
fi
while [ "$#" -ne 0 ] ; do
    case "$1" in
	--ci) ci=true ;;
	--dir=*) BUILD="${1#*=}" ;;
	--force) force=true ;;
	--tt-built) tt_built=true ;;
	-*) echo "Unknown option '$1'" >&2 ; exit 2 ;;
	*) break ;;
    esac
    shift
done

if ! $ci ; then
    # extract git hashes for here and each submodule
    "$BIN/git-hash.sh" > $BUILD/src-hashes.post
    if ! cmp -s $BUILD/sfpi/src-hashes $BUILD/src-hashes.post ; then
	echo "*** WARNING: Source tree has changed since build started ***" 1>&2
	if ! $force ; then
	    exit 1
	fi
    fi
fi

if test -r $BUILD/version ; then
     tt_version=$(cat $BUILD/version)
else
    echo "No $BUILD/version file present" 1>&2
    exit 1
fi
echo "INFO: Version: $tt_version"

# Copy include tree
tar cf - include | tar xf - -C $BUILD/sfpi

find $BUILD/sfpi/compiler -type f -executable -exec file {} \; | \
    grep '^[^ ]*:  *ELF 64-bit ' | cut -d: -f1 | xargs strip -g

NAME=sfpi-$(uname -m)_$(uname -s)

# whether we can build a particular package type
mkdeb=true
mkrpm=false
type rpmbuild >/dev/null 2>&1 && mkrpm=true

# whether we're on a particular package type
ondeb=false
onrpm=false
dpkg-query -f '${Version}' -W libc-bin >/dev/null 2>&1 && ondeb=true
! $ondeb && rpm -q --qf '%{VERSION}' glibc >/dev/null 2>&1 && onrpm=true

fpm_options=(-s dir -n sfpi -C $BUILD/sfpi --prefix /opt/tenstorrent/sfpi)
fpm_options+=(--architecture native)
fpm_options+=(--license "Apache 2.0/GPL 2,3/LGPL 2.1")
fpm_options+=(--url "https://github.com/tenstorrent/sfpi")
fpm_options+=(--description "Tenstorrent SFPI compiler tools")
if $tt_built ; then
    fpm_options+=(--maintainer "Tenstorrent <support@tenstorrent.com>" --vendor "Tenstorrent")
else
    fpm_options+=(--maintainer Unmaintained --vendor Unknown)
fi

# Required libs
deb_libs=(libmpc3 libmpfr6 libgmp10)
rpm_libs=(libmpc mpfr gmp)
# optional isl lib
if ldd $BUILD/sfpi/compiler/libexec/gcc/riscv32-tt-elf/*/cc1plus | grep -q 'libisl\.' ; then
    deb_libs+=(libisl23)
    rpm_libs+=(isl)
fi

deb_deps=()
rpm_dep=()

for ((ix=0; ix!=${#deb_libs[@]}; ix++))
do
    deb_lib=${deb_libs[ix]}
    rpm_lib=${rpm_libs[ix]}
    if $ondeb ; then
	lib=$deb_lib
	if version=$(dpkg-query -f '${Version}' -W $deb_lib 2>/dev/null) ; then
	    if [[ $version =~ ^([0-9]+:)?([0-9.]*) ]] ; then
		version=${BASH_REMATCH[2]}
	    else
		version=
	    fi
	fi
    elif $onrpm ; then
	lib=$rpm_lib
	if version=$(rpm -q --qf '%{VERSION}' $rpm_lib 2>/dev/null) ; then
	    # sometimes version is prefixed by the package name
	    : # version=${version/#$rpm_lib-/}
	fi
    else
	lib=$deb_lib/$rpm_lib
	version=
    fi
    if test -z "$version" ; then
	echo "WARNING: Cannot determine $lib version used" 1>&2
    else
	echo "INFO: $lib >= $version"
	deb_deps+=(--depends "$deb_lib >= $version")
	rpm_deps+=(--depends "$rpm_lib >= $version")
    fi
done

if false ; then
# determine versions we used
for dev in ${libs[@]} ; do
    lib=$dev
    if $ondeb ; then
	# mapping from lib$dev-dev to user lib is complex, getting dependencies is simple
	if depends=$(dpkg-query -f '${Depends}' -W lib$dev-dev 2>/dev/null) ; then
	    # libgmp-dev, libmpfr6 (= 4.1.0-3build3)
	    # libgmp10 (= 2:6.2.1+dfsg-3ubuntu1), libgmpxx4ldbl (= 2:6.2.1+dfsg-3ubuntu1)
	    IFSsave="$IFS"
	    IFS=,
	    for dep in $depends
	    do
		if [[ $dep =~ ^(lib$dev[0-9]*)\ \(=\ ([0-9]+:)?([0-9.]*).*\)$ ]] ; then
		    lib=${BASH_REMATCH[1]}
		    version=${BASH_REMATCH[3]}
		    break;
		fi
	    done
	    IFS="$IFSsave"
	    echo "depend is $dep"
	    #lib="${dep%% *}"
	    #version=$(echo "$dep" | sed -e 's/^.* (= //' -e 's/[^0-9.:].*//')
	else
	    version=
	fi
    elif $onrpm ; then
	# mapping from $foo-devel to user lib is simple, getting dependencies is hard
	if version=$(rpm -q --qf '%[VERSION}' lib$dev 2>/dev/null) ; then
	    lib=lib$dev
	elif version=$(rpm -q --qf '%[VERSION}' $dev 2>/dev/null) ; then
	    lib=$dev
	else
	    version=
	fi
    else
	version=
    fi
    # 2:6.2.1+dfsg-3ubuntu1
    # version=$(echo "$version" | sed -e 's/^[0-9]*://' -e 's/[^0-9.].*//')
    if test -z "$version" ; then
	echo "WARNING: Cannot determine $dev version used" 1>&2
    else
	echo "INFO: $lib >= $version"
	fpm_options+=(--depends "$lib >= $version")
    fi
done
fi

rm -f $BUILD/$NAME.txz
tar cJf $BUILD/$NAME.txz -C $BUILD sfpi
echo "INFO: Tarball: $BUILD/$NAME.txz"
md5=txz

deb=true
if $mkdeb ; then
    rm -f $BUILD/$NAME.deb
    # _ in version is verboten
    fpm -t deb -v "${tt_version//_/-}" "${fpm_options[@]}" "${deb_deps[@]}" -p $BUILD/$NAME.deb
    echo "INFO: Debian: $BUILD/$NAME.deb"
    md5+=,deb
fi

if $mkrpm ; then
    rm -f $BUILD/$NAME.rpm
    # - in version is verboten
    fpm -t rpm -v "${tt_version//-/_}" "${fpm_options[@]}" "${rpm_deps[@]}" -p $BUILD/$NAME.rpm \
	--rpm-auto-add-directories --rpm-rpmbuild-define "_build_id_links none"
    echo "INFO: RPM: $BUILD/$NAME.rpm"
    md5+=,rpm
fi
(cd $BUILD ; md5sum -b $(eval echo $NAME.{$md5})) > $BUILD/$NAME.md5
echo "INFO: MD5: $BUILD/$NAME.md5"
