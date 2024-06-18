#!/bin/bash

SCRIPT_DIR="$(dirname "$(realpath "$BASH_SOURCE")")"

echo $SCRIPT_DIR

mkdir -p /mnt/c/Users/redri/Desktop/SAR
rsync -a --delete $SCRIPT_DIR/out/Debug/* /mnt/c/Users/redri/Desktop/SAR/
cd /mnt/c/Users/redri

/mnt/c/Windows/System32/cmd.exe /c start cmd /k $(wslpath -w /mnt/c/Users/redri/Desktop/SAR/SAR.exe)
