#!/bin/bash
# Author: somebyte
# e-mail: somebyte@yandex.ru
# date  : 18.10.2021
# desc. : the script to start VM

SCRIPTPATH=$(readlink -e $0)
SCRIPTDIR=$(dirname ${SCRIPTPATH})
SCRIPTNAME=$(basename ${SCRIPTPATH})
SCRIPTNAME=${SCRIPTNAME%".sh"}

QEMU="/usr/bin/qemu-system-x86_64"

export CPU MEMORY VIDEO CDROM SDCARD SMB HDB HDC

if [ -z "${CPU}" ]; then
  CPU="4"
fi

if [ -z "${MEMORY}" ]; then
  MEMORY="8G"
fi

if [ -z "${VIDEO}" ]; then
  VIDEO="std"
fi

if [ -n "${CDROM}" ]; then
  CDROM="-cdrom ${CDROM}"
fi

if [ -n "${SMB}" ]; then
  SMB="smb=${SMB}"
fi

if [ -n "${SDCARD}" ]; then
  SDCARD="-drive file=${SDCARD}"
fi

if [ -n "${HDB}" ]; then
  HDB="-hdb ${HDB}"
fi

if [ -n "${HDC}" ]; then
  HDC="-hdc ${HDC}"
fi

${QEMU} ${SCRIPTNAME}.qcow2 ${HDB} ${HDC} -net nic -net user,${SMB} -cpu kvm64 -enable-kvm -smp ${CPU} -m ${MEMORY} -vga ${VIDEO} ${CDROM} ${SDCARD}

