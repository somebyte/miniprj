# Author: somebyte
# e-mail: somebyte@yandex.ru
# date  : 31.08.2021
# desc. : the script for including functions into a script to check files

function checkz {
  if [ -z $1 ]; then
    error "empty string"
    return 1
  fi
  info "string \"$1\" \t[OK]"
  return 0
}

function checkx {
  if [ ! -x $1 ]; then
    error "$1 \t[NOT FOUND]"
    return 1
  fi
  info "$1 \t[OK]"
  return 0
}

function checkb {
  if [ ! -b $1 ]; then
    error "$1 \t[NOT FOUND]"
    return 1
  fi
  info "$1 \t[OK]"
  return 0
}

function checkc {
  if [ ! -c $1 ]; then
    error "$1 \t[NOT FOUND]"
    return 1
  fi
  info "$1 \t[OK]"
  return 0
}

function checkf {
  if [ ! -f $1 ]; then
    error "$1 \t[NOT FOUND]"
    return 1
  fi
  info "$1 \t[OK]"
  return 0
}

function checkd {
  if [ ! -d $1 ]; then
    error "$1 \t[NOT FOUND]"
    return 1
  fi
  info "$1 \t[OK]"
  return 0
}

function checkd_mk {
  if [ ! -d $1 ]; then
    info "$1 \t[NOT FOUND]"
    info "$1 \t[MAKING...]"
    mkdir -p $1
    checkd $1
    return $?
  fi
  info "$1 \t[OK]"
  return 0
}

function sized {
  sized=($(ls $1))
  return ${#sized[@]}
}

