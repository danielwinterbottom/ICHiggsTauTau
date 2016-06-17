#!/usr/bin/env python

import sys
import shutil
import json
import os
import math


output_folder="/vols/cms/amd12/testdir/"
print os.environ['TMPDIR']

for filename in os.listdir(os.environ['TMPDIR']):
  if os.path.isdir(os.path.join(os.environ['TMPDIR'],filename)):
    for subfilename in os.listdir(os.path.join(os.environ['TMPDIR'],filename)):
      shutil.copy(os.environ['TMPDIR']+"/"+filename+"/"+subfilename,output_folder+"/"+filename+"/"+subfilename)
  else :
    shutil.copy(os.environ['TMPDIR']+"/"+filename,output_folder+"/"+filename)
