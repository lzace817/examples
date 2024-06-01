#!/bin/bash

set -e

gcc -Wall -Wextra -ggdb -c arena1.c
gcc -Wall -Wextra -ggdb -c arena2.c
# gcc -Wall -Wextra -ggdb arena1.o test-arena.c -o test
gcc -Wall -Wextra -ggdb arena2.o test-arena.c -o test

echo DONE!
echo

./test
echo test ended with code $?