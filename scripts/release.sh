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

tar czf $BUILD/$NAME.tgz -C $BUILD sfpi
echo "INFO: Tarball: $BUILD/$NAME.tgz"

ARCH=$(dpkg --print-architecture)
VERSION="$tt_version"

echo "INFO: Creating Debian package for architecture: $ARCH with version: $VERSION"

# Create Debian package structure
PKGDIR="$BUILD/debian"
DEBIAN="$PKGDIR/DEBIAN"
INSTALL_DIR="$PKGDIR/opt/tenstorrent/sfpi"

rm -rf "$PKGDIR"
mkdir -p "$DEBIAN" "$INSTALL_DIR"

# Extract the release tgz into the installation directory
tar -xzf "$BUILD/$NAME.tgz" -C "$PKGDIR/opt/tenstorrent"

MAINTAINER="Tenstorrent <support@tenstorrent.com>"
if ! $tt_built ; then
    MAINTAINER="Unmaintained"
fi

# Create a control file for the package
cat > "$DEBIAN/control" <<EOF
Package: sfpi
Version: $VERSION
Section: base
Priority: optional
Architecture: $ARCH
Maintainer: $MAINTAINER
Homepage: https://github.com/tenstorrent/sfpi
Depends: 
Description: Tenstorrent SFPI Release
 This package installs the sfpi release to /opt/tenstorrent/sfpi
EOF

# Build the .deb package
dpkg-deb --build "$PKGDIR" "$BUILD/$NAME.deb"

echo "INFO: Debian package created at: $BUILD/$NAME.deb"

(cd $BUILD ; md5sum -b $NAME.{tgz,deb}) > $BUILD/$NAME.md5
echo "INFO: MD5: $BUILD/$NAME.md5"
