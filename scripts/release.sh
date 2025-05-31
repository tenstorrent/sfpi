#! /bin/bash

set -eo pipefail
BIN="$(dirname "$0")"

if ! test "$BIN" -ef "scripts"; then
    echo "run this script from repo's top directory" 1>&2
    exit 1
fi

BUILD=build
force=false
tt_built=false
if test $(hostname | cut -d- -f-3) = 'tt-metal-dev' ; then
    tt_built=true
fi
while [ "$#" -ne 0 ] ; do
    case "$1" in
	--dir=*) BUILD="${1#*=}" ;;
	--force) force=true ;;
	--tt-built) tt_built=true ;;
	-*) echo "Unknown option '$1'" >&2 ; exit 2 ;;
	*) break ;;
    esac
    shift
done

# extract git hashes for here and each submodule
"$BIN/git-hash.sh" > $BUILD/src-hashes.post
if ! cmp -s $BUILD/sfpi/src-hashes $BUILD/src-hashes.post ; then
    echo "*** WARNING: Source tree has changed since build started ***" 1>&2
    if ! $force ; then
	exit 1
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

tar cJf $BUILD/$NAME.txz -C $BUILD sfpi
echo "INFO: Tarball: $BUILD/$NAME.txz"
md5=($NAME.txz)

if type dpkg-deb >/dev/null 2>&1 && type fpm >/dev/null 2>&1 ; then
    fpm -s dir -t deb \
        -n sfpi \
        -v $tt_version \
        -C ./build/sfpi \
        --prefix /opt/tenstorrent/sfpi \
        --depends "libgmp >= 6.2.1" \
        --depends "libmpfr >= 4.1.0" \
        --depends "libmpc >= 1.2.1" \
        --depends "libisl >= 0.24" \
        --architecture native \
	--license "Apache-2.0" \
        --url "https://github.com/tenstorrent/sfpi" \
	--description "Tenstorrent SFPI compiler tools" \
        --maintainer "Tenstorrent <support@tenstorrent.com>" \
	--vendor "Tenstorrent" \
        --rpm-auto-add-directories \
        --rpm-rpmbuild-define "_build_id_links none" \
        -p build/$NAME.deb

    echo "INFO: Debian package created at: $BUILD/$NAME.deb"
    md5+=($NAME.deb)
fi

if type rpmbuild >/dev/null 2>&1 && type fpm >/dev/null 2>&1 ; then
    fpm -s dir -t rpm \
        -n sfpi \
        -v $tt_version \
        -C ./build/sfpi \
        --prefix /opt/tenstorrent/sfpi \
        --depends "gmp >= 6.2.0" \
        --depends "mpfr >= 4.1.0" \
        --depends "libmpc >= 1.2.1" \
        --architecture native \
	--license "Apache-2.0" \
        --url "https://github.com/tenstorrent/sfpi" \
	--description "Tenstorrent SFPI compiler tools" \
        --maintainer "Tenstorrent <support@tenstorrent.com>" \
	--vendor "Tenstorrent" \
        --rpm-auto-add-directories \
        --rpm-rpmbuild-define "_build_id_links none" \
        -p build/$NAME.rpm

    echo "INFO: RPM package created at: $BUILD/$NAME.rpm"
    md5+=($NAME.rpm)
fi

(cd $BUILD ; md5sum -b ${md5[@]}) > $BUILD/$NAME.md5
echo "INFO: MD5: $BUILD/$NAME.md5"
