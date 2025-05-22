# Cross compiling sysroot

## setup

set `SBC` to ssh your board address inside `mksysroot.sh` script:
``` sh
cd cross
nano mksysroot.sh
```
`mksysroot.sh`:
``` sh
# set -x
set -e

# ssh to target here:
SBC="john@10.1.1.1"
...
```

run the script:
``` sh
./mksysroot.sh
```
> [!NOTE]
> you will need some ssh credential here!
