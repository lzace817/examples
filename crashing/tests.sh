#!/bin/bash

# ./assert
# echo $?
# ./segfault
# echo $?
# ./divz
# echo $?

tests=""
compile_app() {
    local base_name=$(basename "$1" .c)
    tests+="$base_name "
    gcc "$1" -o "${base_name}"
}

apps="*.c"

set -e
# set -x

for app in $apps; do
    compile_app "$app"
done

# disable stop on error
set +e

for t in $tests; do
  echo ""
  "./$t"
  echo "$t terminated with code $?"
done