#! /bin/bash

set -eo pipefail
BIN="$(dirname "$0")"

if ! test "$BIN" -ef "scripts"; then
    echo "run this script from repo's top directory" 1>&2
    exit 1
fi

BUILD=build
force=false
debian=false
while [ "$#" -ne 0 ] ; do
    case "$1" in
	--dir=*) BUILD="${1#*=}" ;;
	--force) force=true ;;
        --debian) debian=true ;;
	-*) echo "Unknown option '$1'" >&2 ; exit 2 ;;
	*) break ;;
    esac
    shift
done

# extract git hashes for here and each submodule
"$BIN/git-hash.sh" > $BUILD/src-hashes.post
if ! cmp -s $BUILD/sfpi/src-hashes $BUILD/src-hashes.post ; then
    echo "source tree has changed since build started" 1>&2
    if ! $force ; then
	exit 1
    fi
fi

# Copy include tree
tar cf - include | (cd $BUILD/sfpi && tar xf -)

find $BUILD/sfpi/compiler -type f -executable -exec file {} \; | \
    grep '^[^ ]*:  *ELF 64-bit ' | cut -d: -f1 | xargs strip -g

IDENT=sfpi-$(uname -m)-$(uname -s)

(cd $BUILD ; tar czf $IDENT.tgz sfpi)
md5sum $BUILD/$IDENT.tgz > $BUILD/$IDENT.md5

if $debian; then

ARCH=$(dpkg --print-architecture)
VERSION=$(git describe --tags --always | sed 's/^v//; s/-/~/g')

echo "INFO: Creating Debian package for architecture: $ARCH with version: $VERSION"

# Create Debian package structure
PKGDIR="$BUILD/sfpi-deb"
DEBIAN="$PKGDIR/DEBIAN"
INSTALL_DIR="$PKGDIR/opt/tenstorrent/sfpi"

rm -rf "$PKGDIR"
mkdir -p "$DEBIAN" "$INSTALL_DIR"

# Extract the release tgz into the installation directory
tar -xzf "$BUILD/$IDENT.tgz" -C "$PKGDIR/opt/tenstorrent"

# Create a control file for the package
cat > "$DEBIAN/control" <<EOF
Package: sfpi
Version: $VERSION
Section: base
Priority: optional
Architecture: $ARCH
Maintainer: Tenstorrent <support@tenstorrent.com>
Homepage: https://github.com/tenstorrent/sfpi
Depends: libgmp10 (>= 2:6.2.1), libmpfr6 (>= 4.1.0), libmpc3 (>= 1.2.1)
Description: Tenstorrent SFPI Release
 This package installs the sfpi release to /opt/tenstorrent/sfpi
EOF

# Build the .deb package
dpkg-deb --build "$PKGDIR" "$BUILD/$IDENT.deb"

echo "INFO: Debian package created at: $BUILD/$IDENT.deb"
fi
