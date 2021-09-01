# Author: somebyte
# e-mail: somebyte@yandex.ru
# date  : 31.08.2021
# desc. : the script for including functions into a script to check files

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

