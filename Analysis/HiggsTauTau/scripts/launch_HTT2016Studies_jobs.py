from UserCode.ICHiggsTauTau.jobs import Jobs
import argparse
import os

job_mgr = Jobs()
parser = argparse.ArgumentParser()
job_mgr.attach_job_args(parser)
args = parser.parse_args()
job_mgr.set_args(args)

basedir = '%s/src/UserCode/ICHiggsTauTau/Analysis/HiggsTauTau' % os.environ[
    'CMSSW_BASE']

MAX_EVTS = -1
FILES_PER_JOB = 100

SAMPLES = [
  'Tau-Run2016B-PromptReco-v1',
  'Tau-Run2016B-PromptReco-v2'
]

for sa in SAMPLES:
    filelist = '%s/filelists/%s.dat' % (basedir, sa)
    cfg = {
        'output': '%s.root' % sa,
        'filelists': [filelist],
        'max_events': MAX_EVTS,
        'sample_name': sa,
        'lumi_mask': 'Cert_271036-274443_13TeV_PromptReco_Collisions16_JSON.txt',
        'lumi_out': 'lumi',
        'has_map_mode': 0,
        'trigger_info_output': 'test_info.json'
    }
    job_mgr.add_filelist_split_jobs(prog=basedir+'/bin/HTT2016Studies', cfg=cfg, files_per_job=FILES_PER_JOB)

job_mgr.flush_queue()
