#!/bin/bash

set -e
CFLAGS="-Wall -Wextra -ggdb"

gcc ${CFLAGS}  ov-test.c -o ov-test-no-oft
gcc ${CFLAGS} -ftrapv ov-test.c -o ov-test-oft

echo DONE!
echo

set +e

echo "no overflow check"
./ov-test-no-oft
echo test ended with code $?

echo ""

echo "overflow check"
./ov-test-oft
echo test ended with code $?