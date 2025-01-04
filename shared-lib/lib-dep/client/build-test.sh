#!/bin/bash

export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$PWD/libraries/lib

echo Compiling client

gcc client/client.c -o client/client -Ilibraries/include -Llibraries/lib -lfoo

echo "============= TESTING ================"

echo "for patch 0"
ln -sf libfoo.so.1.0.0 libraries/lib/libfoo.so.1.0
ldd client/client

echo "for patch 1"
ln -sf libfoo.so.1.0.1 libraries/lib/libfoo.so.1.0
ldd client/client