import os
import json
import stat
from math import ceil

def file_len(fname):
  with open(fname) as f:
    for i, l in enumerate(f):
      pass
    return i + 1


JOB_PROTO="""#!/bin/bash
export X509_USER_PROXY=$HOME/.globus/user_proxy.pem
source /usr/share/Modules/init/bash
module use -a /afs/desy.de/group/cms/modulefiles/; module load cmssw
cd %(PWD)s
export SCRAM_ARCH=%(SCRAM_ARCH)s
eval `scramv1 runtime -sh`
%(INPUT)s
"""

basedir = '%s/src/UserCode/ICHiggsTauTau/Analysis/HiggsTauTau' % os.environ[
    'CMSSW_BASE']

PROD = 'Mar05_MC_76X'
MAX_EVTS = -1
FILES_PER_JOB = 200


SAMPLES = [
  'DY1JetsToLL_M-50-LO',
  'DY2JetsToLL_M-50-LO',
  'DY3JetsToLL_M-50-LO',
  'DY4JetsToLL_M-50-LO',
  'DYJetsToLL_M-150-LO',
  'DYJetsToLL_M-50-LO-ext'
]

for sa in SAMPLES:
    filelist = '%s/filelists/%s_%s.dat' % (basedir, PROD, sa)
    nfiles = file_len(filelist)
    njobs = int(ceil(float(nfiles)/float(FILES_PER_JOB)))
    for n in xrange(njobs):
        cfg = {
            'output': '%s_%i.root' % (sa, n),
            'filelist': filelist,
            'file_offset': n,
            'file_step': njobs,
            'max_events': MAX_EVTS,
            'sample_name': sa,
            "file_prefix": "root://xrootd.grid.hep.ph.ic.ac.uk//store/user/adewit/Mar05_MC_76X"
        }
        cmd = """%s/bin/GenWeightStudy '%s' | tee %s_%i.log""" % (basedir, json.dumps(cfg), sa, n)
        fname = 'job_%s_%i.sh' % (sa, n)
        with open(fname, "w") as outfile:
            print 'Creating job script %s' % fname 
            outfile.write(JOB_PROTO % ({
               'PWD': os.environ['PWD'],
               'SCRAM_ARCH': os.environ['SCRAM_ARCH'],
               'INPUT': cmd
              }))
        st = os.stat(fname)
        os.chmod(fname, st.st_mode | stat.S_IEXEC)
