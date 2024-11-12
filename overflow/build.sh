#!/bin/bash

set -e
CFLAGS="-Wall -Wextra -Wpedantic -Wstrict-prototypes -ggdb"
CFLAGS="${CFLAGS} -Wmissing-prototypes"
CFLAGS="${CFLAGS} -Wno-unused-function -Wno-unused-parameter -Wno-unused-variable" # NOTE(proto): comment to look for unsused


gcc ${CFLAGS}  ov-test.c -o ov-test-no-oft
gcc ${CFLAGS} -ftrapv ov-test.c -o ov-test-oft

echo -e "\033[1;32mDONE!\033[0m"
echo

set +e

echo "no overflow check"
./ov-test-no-oft
echo test ended with code $?

echo ""

echo "overflow check"
./ov-test-oft
echo test ended with code $?