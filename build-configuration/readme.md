# build configuration

There are two configuration flags on `main.c` that controls witch message is printed. This can be configured during compilation using gcc's command line argument `-D` without no modificaon on code:

``` sh
gcc hello.c -o hello -D CONFIG_PLAIN
```

Build systems can parameterize command line arguments and make this configuration as abstract as needed, to a point where even non developers can customize it.

```
               ___________
Message type: |_________|v|
                |__plain__|
                |__fancy__|
```

This example ilustrates how a build configuration works.

## quick start

``` bash
./configure
make
./hello
```
