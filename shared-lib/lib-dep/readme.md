# dependencies within dynamic libraries
withtout recompiling the client code, a shared library can update its dependencies.

``` bash
============= TESTING ================
for patch 0
        linux-vdso.so.1 (0x00007ffe42952000)
        libfoo.so.1.0 => .../lib/libfoo.so.1.0 (0x000072ed33370000)
        libc.so.6 => .../libc.so.6 (0x000072ed33000000)
        libbar.so.1.0 => .../lib/libbar.so.1.0 (0x000072ed33351000)
        /lib64/ld-linux-x86-64.so.2 (0x000072ed3337c000)
for patch 1
        linux-vdso.so.1 (0x00007fffa7be9000)
        libfoo.so.1.0 => .../lib/libfoo.so.1.0 (0x000077a2995d4000)
        libc.so.6 => .../libc.so.6 (0x000077a299200000)
        libcawabunga.so.1.0 => .../lib/libcawabunga.so.1.0 (0x000077a2995b5000)
        /lib64/ld-linux-x86-64.so.2 (0x000077a2995e0000)
DONE!
```

as we can see, the client can detect a change on it's dependencies with no client recompilation. The library foo is the only one who needs to be recompiled.

## structure
item | patch | depends on
---|---|---
client | - | libfoo.so
libfoo.so | 0 | libbar.so
libfoo.so | 1 | libcawabunga.so

## How it works

when compiling the `.so`, links to the desired dependencies:

``` bash
gcc -shared -fPIC ... -lbar
```

This will embed a reference to `.so` within the library itself.

TODO: insert command to view dependencies within  dynamic library.

## Quick start

``` bash
$ ./build.sh
```