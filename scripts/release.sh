#! /bin/bash

set -eo pipefail
BIN="$(dirname "$0")"

if ! test "$BIN" -re "scripts"; then
    echo "run this script from repo's top directory" 1>&2
    exit 1
fi

# extract git hashes for here and each submodule
"$BIN/git-hash.sh" > build/src-hashes.post
if ! cmp -s build/sfpi/src-hashes build/src-hashes.post ; then
    echo "source tree has changed since build started" 1>&2
    exit 1
fi

# Copy include tree
tar cf - include | (cd build/sfpi && tar xf -)

cd build
find sfpi/compiler -type f -executable -exec file {} \; | grep '^[^ ]*:  *ELF 64-bit ' | cut -d: -f1 | xargs strip -g

tar czf sfpi-release.tgz sfpi
md5sum sfpi-release.tgz > sfpi.md5
