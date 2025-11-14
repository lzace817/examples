#!/bin/bash

set -e
mkdir -p build
gcc fd-hello.c -o fd-hello

./fd-hello

echo -e "\033[1;32mDONE!\033[0m"
