#!/bin/bash
# Author: somebyte
# e-mail: somebyte@yandex.ru
# 09.08.2021, "KB KORUND-M"

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

. ${SCRIPTDIR}/${SCRIPTNAME}-vars

println

checkx ${TAR}       && checkx ${SYNC}     && checkx ${RMCMD}  && \
checkx ${DD}        && checkx ${CPCMD}    && checkx ${SFDISK} && \
checkx ${MKFS_FAT}  && checkx ${MKFS_EXT} && checkb ${MMCDEV} && \
checkx ${MMCU}      && checkx ${MOUNT}    && checkx ${UMOUNT}

if [ $? -eq 1 ]; then
  fail
  exit 1
fi

BLOCK="Install u-boot on mmc"
block "${BLOCK}" "..." "***"

checkf ${SPL} && checkf ${UBOOT}

if [ $? -eq 1 ]; then
  fail
  exit 1
fi

checkb ${MMCBOOTDEV} && checkf ${MMCBOOTCTL}

if [ $? -eq 0 ]; then
  echo 0 > ${MMCBOOTCTL}
  ${DD} if=/dev/zero of=${MMCBOOTDEV} bs=1K count=33 && sync
  ${DD} if=${SPL}    of=${MMCBOOTDEV} bs=1K seek=1   && sync
  ${DD} if=${UBOOT}  of=${MMCBOOTDEV} bs=1K seek=69  && sync
  echo 1 > ${MMCBOOTCTL} 
  ${MMCU} bootpart enable 1 1 ${MMCDEV}
  sleep 1
  block "${BLOCK}" "DONE" "***"
else
  block "${BLOCK}" "FAIL" "***"
  exit 1
fi

BLOCK="Creating mmc partions table"
block "${BLOCK}" "..." "***"

checkb ${MMCDEV}p1 && checkb ${MMCDEV}p2
if [ $? -eq 1 ]; then
${SFDISK} ${MMCDEV} << EOF
20480 1048576 L -
1069056 + L -
EOF
fi

${SYNC} && checkb ${MMCDEV}p1 && checkb ${MMCDEV}p2
if [ $? -eq 0 ]; then
  ${MKFS_FAT} ${MMCDEV}p1  
  ${MKFS_EXT} ${MMCDEV}p2
  checkd_mk ${MOUNTPOINT}/${MMC}p1
  checkd_mk ${MOUNTPOINT}/${MMC}p2
  ${MOUNT} ${MMCDEV}p1 ${MOUNTPOINT}/${MMC}p1
  ${MOUNT} ${MMCDEV}p2 ${MOUNTPOINT}/${MMC}p2
  block "${BLOCK}" "DONE" "***"
else
  block "${BLOCK}" "FAIL" "***"
  exit 1;
fi

BLOCK="Copy Kernel & DTS to ${MMC}p1"
block "${BLOCK}" "..." "***"

checkd ${MOUNTPOINT}/${MMC}p1 && checkf ${DISTR}/linux/imx6qp-sabresd.dtb &&\
checkf ${DISTR}/linux/zImage
if [ $? -eq 0 ]; then
  ${RMCMD} -rfv ${MOUNTPOINT}/${MMC}p1/*
  ${CPCMD} -v ${DISTR}/linux/* ${MOUNTPOINT}/${MMC}p1/
  block "${BLOCK}" "DONE" "***"
else
  block "${BLOCK}" "FAIL" "***"
  exit 1
fi

BLOCK="Install rootfs to ${MMC}p2"
block "${BLOCK}" "..." "***"

checkd ${MOUNTPOINT}/${MMC}p2 && \
checkd ${ROOTFS}${MODULES}    && \
checkf ${ROOTFS}${WMTYPE}/${ROOTFSARCH}
if [ $? -eq 0 ]; then
  printf "Wait ${ROOTFS}${WMTYPE}/${ROOTFSARCH} unpacking..."
  ${RMCMD} -rf ${MOUNTPOINT}/${MMC}p2/*
  ${TAR} -jxf ${ROOTFS}${WMTYPE}/${ROOTFSARCH} -C ${MOUNTPOINT}/${MMC}p2
  ${CPCMD} -Rav ${ROOTFS}${MODULES}/* ${MOUNTPOINT}/${MMC}p2/
  ${SYNC}
  ${UMOUNT} ${MMCDEV}* -v
  block "${BLOCK}" "DONE" "***"
else
  block "${BLOCK}" "FAIL" "***"
  exit 1
fi

println

exit 0

