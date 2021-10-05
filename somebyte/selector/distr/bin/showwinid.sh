#!/bin/sh

wmctrl -ir $1 -b remove,shaded
wmctrl -ia $1
