#!/bin/bash

# set -x
set -e

# ssh to target here:
SBC="user@hostname"

###############################################################################

# set paths
SBC_PATH="${SBC}:/"
TMP_PATH="test/sysroot-mirror/"
SYSROOT_PATH="test/sysroot/"

# copy to local mirror
echo "Creating local mirror"
mkdir -p "${TMP_PATH}usr/"
rsync -a --info=progress2 "${SBC_PATH}usr/" "${TMP_PATH}usr/"
mkdir -p "${TMP_PATH}etc/alternatives/"
rsync -a --info=progress2 "${SBC_PATH}etc/alternatives/" "${TMP_PATH}etc/alternatives/"

# create links
mkdir -p "${SYSROOT_PATH}etc"
ln -sf "usr/lib" "${SYSROOT_PATH}lib"
ln -sf "usr/bin" "${SYSROOT_PATH}bin"

# create linker script
# TODO: fix for other architetures
cat << EOF > "${SYSROOT_PATH}etc/ld.so.conf"
/lib/aarch64-linux-gnu
/usr/lib/aarch64-linux-gnu
/usr/local/lib
EOF

SOURCE_PATH=$(realpath "${TMP_PATH}")
DESTINATION_PATH=$(realpath "${SYSROOT_PATH}")

# Copy files from source to destination
echo "Updating sysroot files"
rsync -a --info=progress2 "$SOURCE_PATH/" "$DESTINATION_PATH/"

# Change to the destination directory
cd "$DESTINATION_PATH" || exit

# Fix the symbolic links
echo "Fixing symbolic links"
find . -type l | while read -r link; do
    target=$(readlink "$link")
    if [[ "$target" == /* ]]; then
        # If the target is an absolute path, replace it with the new path
        new_target="${DESTINATION_PATH}${target}"
        ln -sf "$new_target" "$link" || true
    fi
done

echo -e "\033[1;32mDONE!\033[0m"

#   compile with:
#   ``` sh
#   ${CROSS_COMPILE}gcc main.c -o cross-hello --sysroot=./test/sysroot
#   ```