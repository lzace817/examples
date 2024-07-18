#!/bin/bash

set -e

CFLAGS="-Wall -Wextra -Wpedantic -Wstrict-prototypes -ggdb"
CFLAGS="${CFLAGS} -Wno-unused-function -Wno-unused-parameter -Wno-unused-variable" # NOTE(proto): comment to look for unsused

for file in *.c; do
  mkdir -p build/"$(dirname "$file")"/
  gcc ${CFLAGS} $file -c -o build/"$(dirname "$file")"/"$(basename "$file" .c).o"
done

gcc ${CFLAGS} build/test-arena.o build/arena4.o -o build/test-arena

echo -e "\033[1;32mDONE!\033[0m"
