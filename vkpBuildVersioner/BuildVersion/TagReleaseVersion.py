#! /user/bin/env python
# MIT License

# Copyright (c) 2017 - 2019 Vasileios Kon. Pothos (terablade2001)
# https://github.com/terablade2001

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

import subprocess
import sys
import struct
import math
import os

# Update this path to show to project's [BuildVersion] library
PyBuildVersionPath = "../python/BuildVersion/"
# Define the format of the tag.
PyFormat = 1
# Define Global Version File
VersionFile = "BuildVersion.hpp"

def TagBuildVersion(format, versionfile, buildversionpath):
  dbg = False

  ProjectStateFile = buildversionpath + "project.state"
  InternalVersion  = buildversionpath + "current.version"
  if not os.path.exists(ProjectStateFile):
    print("File is missing: [%s]. No version may exist! ... Aborting."%(ProjectStateFile))
    sys.exit(0)
  with open(ProjectStateFile,"w") as wfp:
    print("0",file=wfp,end='')

  # Use the global version file and update the version.
  Lines = [line.rstrip('\n') for line in open(versionfile)]
  version_number=[]
  for l in Lines:
    ps = l.find(" (")
    pe = l.find(".0)")
    BV = int(l[ps+2:pe])
    break

  BV = BV + 1
  with open(versionfile,"w") as wfp:
    print("#define VERSION_NUMBER (%i.0)"%(BV),file=wfp)
  with open(InternalVersion,"w") as wfp:
    print("(%i.0)"%(BV),file=wfp)

  
  Lines = [line.rstrip('\n') for line in open(InternalVersion)]
  version_number=[]
  for l in Lines:
    ps = l.find(" (")
    pe = l.find(".0)")
    version_number = l[ps+2:pe]
    break
  BV = int(version_number)
  BUILDFormatStr=[]
  if (format == 0):
    BUILDFormatStr = "%3.3i"%(BV)
  elif (format == 3):
    tr4 = math.floor(BV / (1000000000));
    tr3 = math.floor((BV-tr4*1000000000) / (1000000));
    tr2 = math.floor((BV-tr4*1000000000-tr3*1000000) / (1000));
    tr1 = math.floor(BV-tr4*1000000000-tr3*1000000-tr2*1000);
    BUILDFormatStr = "%i.%3.3i.%3.3i.%3.3i"%(tr4,tr3,tr2,tr1)
  elif (format == 2):
    tr3 = math.floor((BV) / (1000000));
    tr2 = math.floor((BV-tr3*1000000) / (1000));
    tr1 = math.floor((BV-tr3*1000000-tr2*1000));
    BUILDFormatStr = "%i.%3.3i.%3.3i"%(tr3,tr2,tr1)
  else:
    tr2 = math.floor((BV) / (1000));
    tr1 = math.floor((BV-tr2*1000));
    BUILDFormatStr = "%i.%3.3i"%(tr2,tr1)

  subprocess.call(["git", "add", "--all", ])
  subprocess.call(["git", "commit", "-m", '[%s]: RELEASED VERSION'%(BUILDFormatStr)])
  subprocess.call(["git", "tag", '%s'%(BUILDFormatStr)])
  # subprocess.call(["git", "push", "--tags"])
  

FF = (os.path.dirname(os.path.realpath(__file__))).replace("\\","/")+"/"
TagBuildVersion(PyFormat, FF+VersionFile, FF+PyBuildVersionPath)


