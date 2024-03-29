#! /user/bin/env python
# MIT License

# Copyright (c) 2016 - 2021 Vasileios Kon. Pothos (terablade2001)
# https://github.com/terablade2001/vkpLibs

# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:

# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.

# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.

import sys

def vkpConfigRead(filename, flag):
  if (flag == 1):
    print ("vkpConfigRead - [%s]:"%(filename))
  try:
    res = {}
    with open(filename,"r") as rfp:
      Lines = rfp.readlines()
      for l in Lines:
        l = l.rstrip('\n');
        if len(l) == 0:
          continue
        if l[0] == "#":
          continue
        p = l.find(': ')
        if (p == -1):
          raise ValueError("Unexpected line without ':' in configuration file.\n * Error at config file LINE -> [%s]"%(l))
        if (flag == 1):
          print("  > [%s]"%(l))
        res[l[0:p]] = l[p+2:]
  except NameError:
    print("error:",NameError)
  return res

def vkpConfig_ConvertToFloats(str):
  return list(map(float,str.split(',')))

def vkpConfig_ConvertToInts(str):
  return list(map(int,str.split(',')))

def vkpConfig_ConvertToBools(str):
  return list(map(bool,str.split(',')))
