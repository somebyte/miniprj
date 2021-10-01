#!/bin/bash
# Author: somebyte
# e-mail: somebyte@yandex.ru
# date  : 28.09.2021
# editor: vim

SCRIPTPATH=$(readlink -e $0)
SCRIPTDIR=$(dirname ${SCRIPTPATH})
SCRIPTNAME=$(basename ${SCRIPTPATH})
SCRIPTNAME=${SCRIPTNAME%".sh"}

if [ -z "$SCRIPTDIR" ]; then
   printf "\n\tERROR: cannot define the script dir\n" >&2
   exit 1;
fi

. ${SCRIPTDIR}/output.sh
. ${SCRIPTDIR}/checks.sh

if [ -z "$SCRIPTPATH" ]; then
   error "cannot define the script path"
   exit 1;
fi

if [ -z "$SCRIPTNAME" ]; then
   error "cannot define the script name"
   exit 1;
fi

. ${SCRIPTDIR}/ctrl.fw.funcs

checkfwd

### 1 checking flash device file

checkx ${MOUNT} && checkx ${UMOUNT} && checkx ${MOUNTPOINT} && checkb ${FLASHDEV}

if [ $? -eq 1 ]; then
  fail
  exit 1
fi

###

### 2 checking flash mount point

checkd_mk ${FLASHMPT}

if [ $? -ne 0 ]; then
  fail
  exit 1
fi

checkd_mountpoint ${FLASHMPT}

if [ $? -eq 1 ]; then
  # 2.1 mount flash device
  ${MOUNT} ${FLASHDEV} ${FLASHMPT}
  checkd_mountpoint ${FLASHMPT}
  if [ $? -eq 1 ]; then
    fail
    exit 1
  fi
fi

###

### 3 searching for firmware

FIRMWARE=
for i in "${FWPATTERNS[@]}"; do
  VAR=($(ls ${FLASHMPT} | grep -E $i))
  if [ ! -z ${VAR[0]} ]; then
    FIRMWARE=${VAR[0]}
    break
  fi
done

checkf ${FLASHMPT}/${FIRMWARE}
if [ $? -ne 0 ]; then
  umount ${FLASHMPT}
  info "firmware not found"
  exit 2
fi

info "$FIRMWARE"

###

cp -v ${FLASHMPT}/${FIRMWARE} ${NEW_FIRMWARE_D}

if [ $? -ne 0 ]; then
  umount ${FLASHMPT}
  fail
  exit 3
fi

umount ${FLASHMPT}

exit 0

