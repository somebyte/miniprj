#!/usr/bin/python3

import os
import importlib

fq0 = "/tmp/queue0"
fq1 = "/tmp/queue1"

if os.path.isfile (fq0):
  os.remove (fq0)

if os.path.isfile (fq1):
  os.remove (fq1)

def testqueueif (N, X, Z):
  os.system ("./queueif " + N + " info")
  os.system ("./queueif " + N + " push " + str (10 + X))
  os.system ("./queueif " + N + " push " + str (11 + X))
  os.system ("./queueif " + N + " push " + str (12 + X))
  os.system ("./queueif " + N + " push " + str (1010 + X))
  os.system ("./queueif " + N + " push " + str (2010 + X))
  os.system ("./queueif " + N + " push " + str (-100 + X))
  os.system ("./queueif " + N + " push " + str (-1000 + X))
  os.system ("./queueif " + N + " pop ")
  os.system ("./queueif " + N + " pop ")
  os.system ("./queueif " + N + " pop ")
  os.system ("./queueif " + N + " pop ")
  os.system ("./queueif " + N + " push " + str (1020 + X))
  os.system ("./queueif " + N + " push " + str (10 + X))
  os.system ("./queueif " + N + " push " + str (11 + X))
  os.system ("./queueif " + N + " push " + str (12 + X))
  os.system ("./queueif " + N + " push " + str (-300 + X))
  os.system ("./queueif " + N + " bit "  + str (6 + Z) + " > queue" + 'res' + ".py")
  os.system ("./queueif " + N + " info")
  
  result = 0
  if os.path.isfile ("queueres.py"):
    import queueres
    importlib.reload (queueres)
    from queueres import q
    print (q)
    for var in q: 
      result += var;
    print (result)
  return result

if testqueueif ('0', 0, 0) == 2730:
  print ("test queue 0: PASSED")

if os.path.isfile ("queue0.py"):
  os.remove ("queue0.py")

if testqueueif ('1', 100, 0) == 553:
  print ("test queue 0: PASSED")

