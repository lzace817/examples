#!/bin/bash

set -e

CFLAGS="-Wall -Wextra -Wpedantic -Wstrict-prototypes -ggdb"
CFLAGS="${CFLAGS} -Wmissing-prototypes"
# CFLAGS="${CFLAGS} -Wno-unused-function -Wno-unused-parameter -Wno-unused-variable" # NOTE(proto): comment to look for unsused

gcc ${CFLAGS} -Idist/include client/main.c -o client/main -Ldist/lib -lgreetings

echo -e "\033[1;32mDONE!\033[0m"