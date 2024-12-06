#! /bin/bash

set -eo pipefail
BIN="$(dirname "$0")"

NCPUS=$(grep -c '^processor' /proc/cpuinfo)
if ! test "$BIN" -ef "scripts"; then
    echo "run this script from repo's top directory" 1>&2
    exit 1
fi

if ! test -d build ; then
    mkdir -p build/sfpi
    # extract git hashes for here and each submodule
    "$BIN/git-hash.sh" > build/sfpi/src-hashes
fi

# Clean the environment
for env in $(printenv) ; do
    var="${env%%=*}"
    case "$var" in
	LC_*) unset $var ;;
    esac
done
export LC_ALL=C

cd build
test -e Makefile ||
    ../configure --prefix="$(pwd)/sfpi/compiler" --disable-multilib --with-arch=rv32i --with-abi=ilp32 --disable-gdb
nice make -j${NCPUS:-1}
cd ~-

