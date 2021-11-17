#/bin/bash

SRC=`pwd`

DST="$1"
if [ -z $1 ]; then
    echo "Must specify the path to the destination (installation) directory"
    exit 0
fi
# Sanity test
if [ ! -d $DST/compiler/libexec ]; then
    echo "Did not find a libexec directory in destination $DST/compiler/libexec"
    exit 0
fi

if [ ! -d $SRC/compiler/libexec ]; then
    echo "Did not find a libexec directory in source $SRC/compiler/libexec"
    exit 0
fi

cp -r $SRC/include $DST
cp -r $SRC/compiler/bin $DST/compiler
cp -r $SRC/compiler/include $DST/compiler
cp -r $SRC/compiler/lib $DST/compiler
cp -r $SRC/compiler/libexec $DST/compiler
cp -r $SRC/compiler/riscv64-unknown-elf $DST/compiler
# skip $SRC/share, doesn't seem to be needed and is biggish

# Heavy hammer, will print tons of errors, eg,  "File format not recognized"
find $DST/compiler -type f -executable -exec strip {} \;
