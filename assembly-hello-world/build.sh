#!/bin/bash
# set -e

gcc -c hello.s
ld hello.o -o hello

./hello
echo exit code $?