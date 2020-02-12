import glob
import os

CONDOR_TEMPLATE = """executable = ./jobs/%(EXE)s
Proxy_path =/afs/cern.ch/user/a/adow/private/x509up
arguments = $(ProcId) $(Proxy_path)
output                = ./jobs/%(TASK)s.$(ClusterId).$(ProcId).out
error                 = ./jobs/%(TASK)s.$(ClusterId).$(ProcId).err
log                   = ./jobs/%(TASK)s.$(ClusterId).log
requirements = (OpSysAndVer =?= "SLCern6")
+JobFlavour     = "longlunch"
queue
"""

jobs=glob.glob('jobs/*.sh')
n_failed=0
n_total=0
for j in jobs:
  failed=True
  if 'parajob' in j: continue
  n_total+=1
  log = j.replace('.sh','.log')
  exists = len(glob.glob('%(log)s' % vars()))>0
  finished = False
  if exists:
    with open(log) as f:
      finished = 'Finished Processing' in f.read()

  if not exists or not finished:
    n_failed+=1
    # resubmit
    print j
    outscriptname = j.replace('jobs/','')
    subfilename = j.replace('.sh','.sub').replace('jobs/','')
    subfile = open("jobs/{}".format(subfilename), "w")
    condor_settings = CONDOR_TEMPLATE % {
      'EXE': outscriptname,
      'TASK': j.replace('.sh','').replace('jobs/','')
    }
    subfile.write(condor_settings)
    subfile.close()
    os.system('condor_submit jobs/{}'.format(subfilename))


print '%(n_failed)i jobs out of %(n_total)i failed' % vars()
   
