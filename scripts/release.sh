#! /bin/bash

set -eo pipefail
BIN="$(dirname "$0")"

if ! test "$BIN" -ef "scripts"; then
    echo "run this script from repo's top directory" 1>&2
    exit 1
fi

BUILD=build
force=false
while [ "$#" -ne 0 ] ; do
    case "$1" in
	--dir=*) BUILD="${1#*=}" ;;
	--force) force=true ;;
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

(cd $BUILD ; tar czf sfpi-release.tgz sfpi)
md5sum $BUILD/sfpi-release.tgz > $BUILD/sfpi.md5

# Create Debian package structure
PKGDIR="$BUILD/sfpi-deb"
DEBIAN="$PKGDIR/DEBIAN"
INSTALL_DIR="$PKGDIR/opt/tenstorrent/sfpi"

rm -rf "$PKGDIR"
mkdir -p "$DEBIAN" "$INSTALL_DIR"

# Extract the release tgz into the installation directory
tar -xzf "$BUILD/sfpi-release.tgz" -C "$PKGDIR/opt/tenstorrent"

# Create a control file for the package
cat > "$DEBIAN/control" <<EOF
Package: sfpi
Version: 1.0.0
Section: base
Priority: optional
Architecture: amd64
Maintainer: Tenstorrent <support@tenstorrent.com>
Depends: libgmp10 (>= 2:6.2.1), libmpfr6 (>= 4.1.0), libmpc3 (>= 1.2.1)
Description: Tenstorrent SFPI Release
 This package installs the sfpi release to /opt/tenstorrent/sfpi
EOF

# Build the .deb package
dpkg-deb --build "$PKGDIR" "$BUILD/sfpi.deb"

echo "Debian package created at: $BUILD/sfpi.deb"
