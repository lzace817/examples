#!/bin/bash

set -e
mkdir -p build
gcc hello.c -o build/hello

echo -e "\033[1;32mDONE!\033[0m"
