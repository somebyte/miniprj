# Author: somebyte
# e-mail: somebyte@yandex.ru
# date  : 31.08.2021
# desc. : the base script to make new scripts

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

