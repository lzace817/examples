#!/bin/bash

# build lib distribuition if needed. Simple auto increment patch version

set -e
# set -x

VERSION="1.0"
OUT_LIB_PATH=dist/lib
OUT_INC_PATH=dist/include
LIB_NAME=libgreetings.so
mkdir -p _build

mkdir -p $OUT_LIB_PATH

if [ ! -f "_build/.patch" ]; then
  echo "0" > "_build/.patch"
fi

function file_changed { # file_changed input output
    if [[ ( "$1" -nt "$2" ) ]]; then
        return 0
    else
        return 1
    fi
}

CHANGE=0
FILES=($(ls *.c *.h))

for FILE in "${FILES[@]}"
do
    if file_changed $FILE "$OUT_LIB_PATH/$LIB_NAME"; then
        CHANGE=1
    fi
done

if [[ ${CHANGE} -eq 1 ]]; then

PATCH=$(cat _build/.patch)

CFLAGS="-Wall -Wextra -Wpedantic -Wstrict-prototypes -ggdb"
CFLAGS="${CFLAGS} -Wmissing-prototypes"
# CFLAGS="${CFLAGS} -Wno-unused-function -Wno-unused-parameter -Wno-unused-variable" # NOTE(proto): comment to look for unsused

for file in *.c; do
  mkdir -p _build/"$(dirname "$file")"/
  gcc ${CFLAGS} -fPIC -I. -c $file -o _build/"$(dirname "$file")"/"$(basename "$file" .c).o"
done

gcc -shared -Wl,-soname,"$LIB_NAME.${VERSION}" -o "$OUT_LIB_PATH/$LIB_NAME.${VERSION}.$PATCH" _build/*.o

ln -s -f "$LIB_NAME.${VERSION}.$PATCH" $OUT_LIB_PATH/$LIB_NAME
ln -s -f "$LIB_NAME.${VERSION}.$PATCH" "$OUT_LIB_PATH/$LIB_NAME.${VERSION}"

PATCH=$(( PATCH+1 ))
echo $PATCH > _build/.patch

## include

mkdir -p $OUT_INC_PATH
cp *.h $OUT_INC_PATH

else
    echo "up to date"
fi