# Author: somebyte
# e-mail: somebyte@yandex.ru
# date  : 01.09.2021
# desc. : the script for including functions into a script to output info

function error {
  printf "ERROR: $1\n" >&2
}

function warning {
  printf "WARNING: $1\n"
}

function info {
  printf "INFO: $1\n"
}

function println {
  printf "$1\n"
}

function block {
  printf "\n$3\n$1 \t[$2]\n$3\n"
}

function fail {
  println "\n" 
  println "FAIL"$1
  println
}

