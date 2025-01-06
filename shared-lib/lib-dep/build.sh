#!/bin/bash
set -e
# set -x

CFLAGS="-Wall -Wextra -Wpedantic -Wstrict-prototypes -Wswitch-enum -ggdb"
CFLAGS="${CFLAGS} -Wmissing-prototypes"
CFLAGS="${CFLAGS} -Wno-unused-function -Wno-unused-parameter -Wno-unused-variable" # NOTE: comment to look for unsused

CROSS_COMPILE=""
PREFIX="libraries"

CC="${CROSS_COMPILE}gcc"
mkdir -p $PREFIX/lib
mkdir -p $PREFIX/include

so_compile(){
    file=$1
    ${CC} ${CFLAGS} -c -fPIC -I. $file -o $PREFIX/"$(dirname "$file")"/"$(basename "$file" .c).o"
}

VERSION=1.0
PATCH=0

make_lib() {
    base=$1
    dep=$2
    so_compile $base.c
    LIB_NAME=lib$base.so
    ${CC} ${CFLAGS} -shared -Wl,-soname,"$LIB_NAME.${VERSION}" -fPIC $PREFIX/$base.o -L$PREFIX/lib/ $dep -o $PREFIX/lib/${LIB_NAME}.${VERSION}.${PATCH}
    ln -s -f "$LIB_NAME.${VERSION}.$PATCH" $PREFIX/lib/$LIB_NAME
    ln -s -f "$LIB_NAME.${VERSION}.$PATCH" "$PREFIX/lib/$LIB_NAME.${VERSION}"
}

echo Compiling libraries

make_lib bar
make_lib cawabunga
cp bar.h $PREFIX/include/


# first version
ln -sf foo-v1.c foo.c
make_lib foo -lbar
cp foo.h $PREFIX/include/

# second version
PATCH=1
ln -sf foo-v2.c foo.c
make_lib foo -lcawabunga # now we depend on cawabunga
cp cawabunga.h $PREFIX/include/

./client/build-test.sh

echo -e "\033[1;32mDONE!\033[0m"