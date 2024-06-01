#!/bin/bash

set -e
# set -x

CFLAGS="-Wall -Wextra -Wpedantic -Wstrict-prototypes -ggdb"
# CFLAGS="${CFLAGS} -Wno-unused-function -Wno-unused-parameter -Wno-unused-variable" # NOTE(proto): comment to look for unsused

gcc ${CFLAGS} -c arena1.c
gcc ${CFLAGS} -c arena2.c
gcc ${CFLAGS} -c test-arena.c

gcc ${CFLAGS} arena1.o test-arena.o -o test1
gcc ${CFLAGS} arena2.o test-arena.o -o test2

echo build DONE!
echo

set +e

./test1
echo test1 ended with code $?

echo ""

./test2
echo test2 ended with code $?