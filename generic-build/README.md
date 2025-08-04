# generic build
Generic makefile that automatically compiles a C project following some simple rules.

__Rules__:
- all C files on the root of the project should be compiled in their own binary.
- libraries are located on `libs`.
- A binary can depend on any subset of the libraries.

desired features:
- [ ] support for out of tree build.

# how to use
just set the files in the correct location, copy the [Makefile](Makefile) to the root and compile.

``` bash
make
```
