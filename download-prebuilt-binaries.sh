#!/bin/sh
set -e

REMOTE_SITE_URL=http://wa.letshack.info

curl -o LLVM-C.dll -L -C - "${REMOTE_SITE_URL}/LLVM-C.dll"
curl -o LLVM-C.lib -L -C - "${REMOTE_SITE_URL}/LLVM-C.lib"

curl -L -C - "${REMOTE_SITE_URL}/MD5SUM" | grep LLVM-C | md5sum -c -
