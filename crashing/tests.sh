#!/bin/bash

set -e

CFLAGS="-Wall -Wextra -Wpedantic -Wstrict-prototypes -ggdb"
CFLAGS="${CFLAGS} -Wmissing-prototypes"
CFLAGS="${CFLAGS} -Wno-unused-function -Wno-unused-parameter -Wno-unused-variable" # NOTE(proto): comment to look for unsused

SRCS=`find -iname '*.c'`
for file in ${SRCS}; do
  mkdir -p build/"$(dirname "$file")"/
  gcc ${CFLAGS} $file -o build/"$(dirname "$file")"/"$(basename "$file" .c)"
done

echo -e "\033[1;32mDONE!\033[0m"

set +e
echo "Testing"
. tests.list