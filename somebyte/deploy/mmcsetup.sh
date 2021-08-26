#!/bin/bash
# Author: somebyte
# e-mail: somebyte@yandex.ru
# 09.08.2021, "KB KORUND-M"

RAM=2GB
DISTR="/distr"
CARD="mmc"
SPL="${DISTR}/uboot-${RAM}-${CARD}/SPL"
UBOOT="${DISTR}/uboot-${RAM}-${CARD}/u-boot.img"
ROOTFS="${DISTR}/rootfs-"
MODULES="modules"
WMTYPE="X11"
MOUNTPOINT="/run/media"
ROOTFSARCH="rootfs.tar.bz2"

# CMD
SYNC="/bin/sync"
TAR="/bin/tar"
DD="/bin/dd"
CPCMD="/bin/cp"
RMCMD="/bin/rm"
UMOUNT="/bin/umount"
SFDISK="/usr/sbin/sfdisk"
MKFS_FAT="/usr/sbin/mkfs.fat"
MKFS_EXT="/sbin/mkfs.ext4"
MMCU="/usr/bin/mmc"

# MMC
MMC="mmcblk3"
MMCBOOT="${MMC}boot0"
MMCDEV="/dev/${MMC}"
MMCBOOTDEV="/dev/${MMCBOOT}"
MMCBOOTCTL="/sys/block/${MMCBOOT}/force_ro"

printf "\n"

function checkx {
  if [ ! -x $1 ]; then
    printf "$1 \t[NOT FOUND]\n" >&2
    return 1
  fi
  printf "$1 \t[OK]\n"
  return 0
}

function checkb {
  if [ ! -b $1 ]; then
    printf "$1 \t[NOT FOUND]\n" >&2
    return 1
  fi
  printf "$1 \t[OK]\n"
  return 0
}

function checkf {
  if [ ! -f $1 ]; then
    printf "$1 \t[NOT FOUND]\n" >&2
    return 1
  fi
  printf "$1 \t[OK]\n"
  return 0
}

function checkd {
  if [ ! -d $1 ]; then
    printf "$1 \t[NOT FOUND]\n" >&2
    return 1
  fi
  printf "$1 \t[OK]\n"
  return 0
}

checkx ${TAR}       && checkx ${SYNC}     && checkx ${RMCMD}  && \
checkx ${DD}        && checkx ${CPCMD}    && checkx ${SFDISK} && \
checkx ${MKFS_FAT}  && checkx ${MKFS_EXT} && checkb ${MMCDEV} && \
checkx ${MMCU}      && checkx ${UMOUNT}

if [ $? -eq 1 ]; then
  printf "\n\nFAIL\n\n"
  exit 1
fi

printf "\n\nInstall u-boot on mmc \t[...]\n***\n"
checkf ${SPL} && checkf ${UBOOT}

if [ $? -eq 1 ]; then
  printf "FAIL\n\n"
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
  printf "***\nInstall u-boot on mmc \t[DONE]\n"
else
  printf "***\nInstall u-boot on mmc \t[FAIL]\n"
  exit 1
fi

printf "\n\nCreating mmc partions table\t[...]\n***\n"

checkb ${MMCDEV}p1 && checkb ${MMCDEV}p2
if [ $? -eq 1 ]; then
#  ${UMOUNT} ${MMCDEV}p*
#  ${SFDISK} --delete ${MMCDEV}
${SFDISK} ${MMCDEV} << EOF
20480 1048576 L -
1069056 + L -
EOF
fi

checkb ${MMCDEV}p1 && checkb ${MMCDEV}p2
if [ $? -eq 0 ]; then
  ${MKFS_FAT} ${MMCDEV}p1  
  ${MKFS_EXT} ${MMCDEV}p2  
  printf "***\nCreating mmc partions table\t[DONE]\n"
else
  printf "***\nCreating mmc partions table\t[FAIL]\n"
  exit 1;
fi

printf "\n\nCopy Kernel & DTS to ${MMC}p1\t[...]\n***\n"
checkd ${MOUNTPOINT}/${MMC}p1 && checkf ${DISTR}/linux/imx6qp-sabresd.dtb &&\
checkf ${DISTR}/linux/zImage
if [ $? -eq 0 ]; then
  ${RMCMD} -rfv ${MOUNTPOINT}/${MMC}p1/*
  ${CPCMD} -v ${DISTR}/linux/* ${MOUNTPOINT}/${MMC}p1/
  printf "***\nCopy Kernel & DTS to ${MMC}p1\t[DONE]\n"
else
  printf "***\nCopy Kernel & DTS to ${MMC}p1\t[FAIL]\n"
  exit 1
fi

printf "\n\nInstall rootfs to ${MMC}p2\t[...]\n***\n"
checkd ${MOUNTPOINT}/${MMC}p2 && \
checkd ${ROOTFS}${MODULES}    && \
checkf ${ROOTFS}${WMTYPE}/${ROOTFSARCH}
if [ $? -eq 0 ]; then
  printf "Wait ${ROOTFS}${WMTYPE}/${ROOTFSARCH} unpacking..."
  ${RMCMD} -rf ${MOUNTPOINT}/${MMC}p2/*
  ${TAR} -jxf ${ROOTFS}${WMTYPE}/${ROOTFSARCH} -C ${MOUNTPOINT}/${MMC}p2
  ${CPCMD} -Rav ${ROOTFS}${MODULES}/* ${MOUNTPOINT}/${MMC}p2/
  ${SYNC}
  printf "***\nInstall rootfs to ${MMC}p2\t[DONE]\n"
else
  printf "***\nInstall rootfs to ${MMC}p2\t[FAIL]\n"
fi

printf "\n"

exit 0

