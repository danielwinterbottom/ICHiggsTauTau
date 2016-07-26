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
FILES_PER_JOB = 50
PROD='July19_'

DATA_SAMPLES = {
  'Tau': [
        'Tau-Run2016B-PromptReco-v1',
        'Tau-Run2016B-PromptReco-v2',
        'Tau-Run2016C-PromptReco-v2',
        'Tau-Run2016D-PromptReco-v2'
        ],
  'SingleMuon': [
        'SingleMuon-Run2016B-PromptReco-v1',
        'SingleMuon-Run2016B-PromptReco-v2',
        'SingleMuon-Run2016C-PromptReco-v2',
        'SingleMuon-Run2016D-PromptReco-v2'
        ],
  'SingleElectron': [
        'SingleElectron-Run2016B-PromptReco-v1',
        'SingleElectron-Run2016B-PromptReco-v2',
        'SingleElectron-Run2016C-PromptReco-v2',
        'SingleElectron-Run2016D-PromptReco-v2'
        ],
  'MuonEG': [
        'MuonEG-Run2016B-PromptReco-v1',
        'MuonEG-Run2016B-PromptReco-v2',
        'MuonEG-Run2016C-PromptReco-v2',
        'MuonEG-Run2016D-PromptReco-v2'
        ]
}

MC_SAMPLES = {
    'DYJetsToLL': [
        'DYJetsToLL'
    ]
}

SAMPLES = {}
SAMPLES.update(DATA_SAMPLES)
SAMPLES.update(MC_SAMPLES)
SEQUENCES = ['Zmm', 'ZmmTP', 'EffectiveEvents']

OUTPUT = 'output/HTT2016Studies_July19'
os.system('mkdir -p %s' % OUTPUT)

task = job_mgr.task_name

for sa in SAMPLES:
    filelists = ['filelists/%s%s.dat' % (PROD, X) for X in SAMPLES[sa]]
    cfg = {
        # General settings
        'output': '%s/%s.root' % (OUTPUT, sa),
        'filelists': filelists,
        'max_events': MAX_EVTS,
        'is_data': sa in DATA_SAMPLES.keys(),
        'sequences': SEQUENCES,
        # Lumi settings
        'lumi_mask': 'input/json/Cert_271036-276811_13TeV_PromptReco_Collisions16_JSON_NoL1T.txt',
        'lumi_out': '%s/lumi_%s' % (OUTPUT, sa),
        # Pileup weights
        'data_pu': 'input/pileup/Data_Pileup_2016_63mb_Cert_271036-276811_NoL1T.root:pileup',
        'mc_pu': 'input/pileup/MC_Spring16_PU25ns_V1.root:pileup',
        # Hash map settings
        'hash_map_mode': 0,
        'hash_map_input': 'input/string_hash/hash_map.json',
        'hash_map_output': 'hash_map_%s.json' % sa,
        # Trigger info settings
        'trigger_info_output': '%s/trigger_info_%s.json' % (OUTPUT, sa)
    }
    job_mgr.add_filelist_split_jobs(
        prog=basedir+'/bin/HTT2016Studies',
        cfg=cfg,
        files_per_job=FILES_PER_JOB,
        output_cfgs=['output', 'lumi_out', 'trigger_info_output'])
    job_mgr.task_name = task + '-' + sa
    job_mgr.flush_queue()
