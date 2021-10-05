#!/bin/sh

echo $(wmctrl -l|grep -i $1|awk '{print $1}')

