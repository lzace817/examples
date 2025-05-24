#!/bin/bash

set -e

CFLAGS="-Wall -Wextra -Wpedantic -Wstrict-prototypes -ggdb"
CFLAGS="${CFLAGS} -Wmissing-prototypes"
CFLAGS="${CFLAGS} -Wno-unused-function -Wno-unused-parameter -Wno-unused-variable" # NOTE(proto): comment to look for unsused

gcc ${CFLAGS} main.c -o opt-args

echo -e "\033[1;32mDONE!\033[0m"


./opt-args