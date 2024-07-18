#!/bin/bash
set -xe
gcc -Wall -Wextra -ggdb ncmem.c -o ncmem
echo DONE!