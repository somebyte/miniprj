#!/bin/bash
# Author: somebyte
# e-mail: somebyte@yandex.ru
# date  : 26.08.2021
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

if [ -z "${TTYDEV}" ]; then
  TTYDEV=$DEFAULT_TTYDEV
fi

if [ -z "${TTYSPEED}" ]; then
  TTYSPEED=$DEFAULT_TTYSPEED
fi

REPEAT=0
while [ $REPEAT -eq 0 ]; do

  FIRMWARE=
  for i in "${FWPATTERNS[@]}"; do
     VAR=($(ls ${NEW_FIRMWARE_D} | grep -E $i))
     if [ ! -z ${VAR[0]} ]; then
       FIRMWARE=${VAR[0]}
       break
     fi
  done

  if [ -z $FIRMWARE ]; then
    info "New firmware not found!"
    exit 1
  fi

  info "New firmware found: ${FIRMWARE}"
  STARTADDR=${FIRMWARE#"firmware_"}
  STARTADDR=${STARTADDR%".srec"}
  FIRMWARE=${NEW_FIRMWARE_D}/${FIRMWARE}
  info "Start firmware address: ${STARTADDR}"
  echo ${STARTADDR} > ${STARTADDR_FL}

  checkx ${UPLOADER} && checkz ${TTYDEV} && checkc ${TTYDEV} && checkf ${FIRMWARE}

  if [ $? -eq 1 ]; then
    fail
    exit 2
  fi

  ${UPLOADER} ${TTYDEV} ${TTYSPEED} ${FIRMWARE} 2>${ERRFILE} 1>${NULLDEV} 

  RETCODE=$?
  ERROR=()
  i=0
  while read ERROR[$i]; do
    i=$(($i+1))
  done < ${ERRFILE}

  if [ -f ${ERRFILE} ]; then
    rm ${ERRFILE}
  fi

  if [ ${RETCODE} -ne 0 ]; then
    for i in "${ERROR[@]}"; do
      if [ ! -z "${i}" ]; then
        error "$i"
      fi
    done
    error_processing ${RETCODE}
    REPEAT=$?
  fi

  if [ ${RETCODE} -eq 0 ]; then
    mv_new_to_cur
    info "UPLOAD SUCCESSFUL"
    exit ${RETCODE}
  else
    fail
  fi

done

exit ${RETCODE}

