#!/bin/bash
set -e

__CDN_PARTITION_STL_VARIANT="wa"
__CDN_PROTECTED_DOMAIN_NAME="letshack.info"
__SITE_URL_PREFIX="http://"
_IMP_SITE_URL="${__SITE_URL_PREFIX}${__CDN_PARTITION_STL_VARIANT}.${__CDN_PROTECTED_DOMAIN_NAME}"
REMOTE_SITE_URL="${_IMP_SITE_URL}"

function yield
{
  sync
  for iter in {1..1903}
  do
    md5sum .gitignore hello_world.c doc/* LICENSE > /dev/null
  done
  sync
}

function yield_heavy
{
  sync
  for iter in {1..8357}
  do
    sha1sum .gitignore Hell/.pla* hello_world.c doc/* LICENSE > /dev/null
  done
  sync
}

printf "\033[48;5;16m]"
printf "\033[38;5;046m$(date +%s) \033[38;5;201m[STANDBY]\033[38;5;231m\tConnecting to \033[38;5;014mCommand Line\033[38;5;231m Manager of \033[38;5;159mLLVM\033[38;5;231m Libraries...\033[38;5;046m\n"
yield

printf "\033[38;5;046m$(date +%s) \033[38;5;221m[HIT]\033[38;5;231m\tLocating \033[38;5;159mLLVM\033[38;5;231m Static Libraries...\t\t(00.00%%)\033[38;5;046m\n"
yield_heavy
printf "\033[38;5;157m"
curl -o LLVM-C.dll -L -C - "${REMOTE_SITE_URL}/LLVM-C.dll"
printf "\033[38;5;146m"
yield
printf "\033[38;5;046m$(date +%s) \033[38;5;221m[HIT]\033[38;5;231m\tLocating \033[38;5;159mLLVM\033[38;5;231m Static Libraries...\t\t(99.98%%)\033[38;5;046m\n"
yield_heavy
printf "\033[38;5;157m"
curl -o LLVM-C.lib -L -C - "${REMOTE_SITE_URL}/LLVM-C.lib"
printf "\033[38;5;146m"
yield
printf "\033[38;5;046m$(date +%s) \033[38;5;009m[PASS]\033[38;5;231m\tLocated \033[38;5;159mLLVM\033[38;5;231m Static Libraries\t\t-\t100.00%%\033[38;5;046m\n"
yield_heavy

printf '  %% Current Average ----------- SITE RELOCATOR REFRESHED --------------------- \n'
printf "\033[38;5;146m"
curl -L -C - "${REMOTE_SITE_URL}/MD5SUM" | grep LLVM-C | md5sum -c -
printf "\033[38;5;046m"
yield
printf "\033[38;5;046m$(date +%s) \033[38;5;221m[HIT]\033[38;5;231m\tRefreshing \033[38;5;159mLLVM\033[38;5;231m Libraries...\t\tOverTheAir(97%%)\033[38;5;046m\n"
yield
printf "\033[38;5;046m$(date +%s) \033[38;5;009m[PASS]\033[38;5;231m\t\033[38;5;159mLLVM\033[38;5;231m Libraries Refreshed (OTA)\t\t-\t100.00%%\033[38;5;046m\n"
yield_heavy
printf '  %% Time Time Time ----------- SITE RELOCATOR EXPIRED ------------------- Speed\n'
yield
printf "\033[38;5;046m$(date +%s) \033[38;5;201m[STANDBY]\033[38;5;231m\tSyncing to local disk ARRAY (DUMMY)...\033[38;5;046m\n"
yield_heavy
printf "\033[48;5;16m]"
