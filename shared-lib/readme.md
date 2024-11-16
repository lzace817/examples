# on shared libs

playground with shared libraries.

## how to use

test how to patch a shared library without recompile the client code.

- compile the lib with `build.sh`
- clear libraries with `clean-lib.sh`
- compile the client with `client/build-client.sh`
- run client with `client/run.sh`
- edit the library source code and change version number inside `build.sh`

TODO: add a simple way to manage versions

