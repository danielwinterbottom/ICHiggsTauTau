import os
import json

basedir = '%s/src/UserCode/ICHiggsTauTau/Analysis/HiggsTauTau' % os.environ[
    'CMSSW_BASE']

PROD = 'Mar19'
MAX_EVTS = 10000

SAMPLES = [
    'WJetsToLNuNLO',
    # 'WJetsToLNuLO'
]

for sa in SAMPLES:
    cfg = {
        'output': '%s.root' % sa,
        'filelist': '%s/filelists/%s_%s.dat' % (basedir, PROD, sa),
        'max_events': MAX_EVTS
    }
    os.system('%s/bin/WJetsStudy \'%s\' | tee %s.log' %
              (basedir, json.dumps(cfg), sa))

              #./bin/WJetsStudy '{"output":"WJetsNLO.root","filelist":"filelists/Mar19_WJetsToLNuNLO.dat","max_events":1000000}'
