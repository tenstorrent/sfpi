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

IDENT=sfpi-$(uname -m)-$(uname -s)

(cd $BUILD ; tar czf $IDENT.tgz sfpi)
md5sum $BUILD/$IDENT.tgz > $BUILD/$IDENT.md5
