#!/usr/bin/python3

import os

os.system ("./queuetest > queuetesting.py")

import queuetesting

from queuetesting import size00, size01, size02, size03, size04, a, b, c,\
size05,\
size06,\
size07,\
size08
from queuetesting import queue00, queue01, queue02, queue03, queue04, queue05,\
queue06, queue07, queue08

n = 0

if not size00:
    n += 1

if not queue00:
    n += 1

if size01 == 1 and queue01[0] == 0:
    n += 1

if size02 == 2 and queue02[1] == 1000:
    n += 1

if size03 == 3 and (queue03[1] + queue03[2]) == 3000:
    n += 1

if not size04 and (a + b + c) == 3000:
    n += 1

message = "testing push/pop:"
score   = "\tscore:"

if n < 6:
    print (message, "FAIL")
    print (score, n)
    quit()
else:
    print (message, "OK")

n = 0
sum = 0
message = "testing a limit:"

for i in queue05:
    sum += i

if size05 == 100 and sum == 4950:
    print (message, "OK")
else:
    print (message, "FAIL");
    print ("\tsize =", size05)
    print ("\tsum =", sum);
    quit();

message = "testing size() with pop():"

if not size06 and not queue06:
    print (message, "OK")
else:
    print (message, "FAIL")
    print ("\tsize =", size06)
    print ("\tcontent =", queue06);
    quit();

sum = 0
message = "testing re-filling queue:"

for i in queue07:
    sum += i

if size07 == 100 and sum == 4950:
    print (message, "OK")
else:
    print (message, "FAIL");
    print ("\tsize =", size07)
    print ("\tsum =", sum);
    quit();

message = "tesing clean:"

if not size08 and not queue08:
    print (message, "OK")
else:
    print (message, "FAIL")
    print ("\tsize =", size08)
    print ("\tcontent =", queue08);
    quit();


