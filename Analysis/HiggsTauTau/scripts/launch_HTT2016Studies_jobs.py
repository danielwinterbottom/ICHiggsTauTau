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
FILES_PER_JOB = 40
PROD='Aug16_'

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
    'DYJetsToLL':           ['DYJetsToLL'],
    'DYJetsToLL_M-10to50':  ['DYJetsToLL_M-10to50'],
    'TT':                   ['TT'],
    'VVTo2L2Nu':            ['VVTo2L2Nu'],
    'WWTo1L1Nu2Q':          ['WWTo1L1Nu2Q'],
    'WZJToLLLNu':           ['WZJToLLLNu'],
    'WZTo1L1Nu2Q':          ['WZTo1L1Nu2Q'],
    'WZTo1L3Nu':            ['WZTo1L3Nu'],
    'WZTo2L2Q':             ['WZTo2L2Q'],
    'ZZTo2L2Q':             ['ZZTo2L2Q'],
    'ZZTo4L':               ['ZZTo4L'],
    'WJetsToLNu':           ['WJetsToLNu']
}

SAMPLES = {}
SAMPLES.update(DATA_SAMPLES)
SAMPLES.update(MC_SAMPLES)
SEQUENCES = ['ZmtTP', 'ZmtTP/scale_t_hi', 'ZmtTP/scale_t_lo']
#SEQUENCES = ['Zmm', 'ZmmTP', 'Zee', 'ZeeTP', 'ZmtTP', 'ZmtTP/scale_t_hi', 'ZmtTP/scale_t_lo', 'EffectiveEvents']
#SEQUENCES = ['HashMap']

if 'HashMap' in SEQUENCES:
  FILES_PER_JOB = 1E6

WHITELIST = {
    'Zmm': ['SingleMuon'] + list(MC_SAMPLES.keys()),
    'ZmmTP': ['SingleMuon'] + list(MC_SAMPLES.keys()),
    'Zee': ['SingleElectron'] + list(MC_SAMPLES.keys()),
    'ZeeTP': ['SingleElectron'] + list(MC_SAMPLES.keys()),
    'ZmtTP': ['SingleMuon'] + list(MC_SAMPLES.keys()),
    'ZmtTP/scale_t_hi': list(MC_SAMPLES.keys()),
    'ZmtTP/scale_t_lo': list(MC_SAMPLES.keys()),
    'EffectiveEvents': list(MC_SAMPLES.keys()),
    'HashMap': list(DATA_SAMPLES.keys())
}

OUTPUT = 'output/HTT2016Studies_'+job_mgr.task_name
os.system('mkdir -p %s' % OUTPUT)
for seq in SEQUENCES:
    os.system('mkdir -p %s/%s' % (OUTPUT, seq))

task = job_mgr.task_name

for sa in SAMPLES:
    seqs = []
    doSample = False
    for seq in SEQUENCES:
        if seq in WHITELIST:
            if sa in WHITELIST[seq]:
                seqs.append(seq)
                doSample = True
        else:
            seqs.append(seq)
            doSample = True
    if not doSample:
        continue
    filelists = ['filelists/%s%s.dat' % (PROD, X) for X in SAMPLES[sa]]
    cfg = {
        # General settings
        'output_dir': '%s' % (OUTPUT),
        'output_name': '%s.root' % (sa),
        'filelists': filelists,
        'max_events': MAX_EVTS,
        'is_data': sa in DATA_SAMPLES.keys(),
        'sequences': seqs,
        # Lumi settings
        'lumi_mask': 'input/json/Cert_271036-276811_13TeV_PromptReco_Collisions16_JSON.txt',
        'lumi_out': '%s/lumi_%s' % (OUTPUT, sa),
        # Pileup weights
        'data_pu': 'input/pileup/Data_Pileup_2016_69p2mb_Cert_271036-276811.root:pileup',
        'data_pu_hi': 'input/pileup/Data_Pileup_2016_80mb_Cert_271036-276811.root:pileup',
        'mc_pu': 'input/pileup/MC_Spring16_PU25ns_V1.root:pileup',
        # Hash map settings
        'hash_map_mode': 0,
        'hash_map_input': 'input/string_hash/hash_map.json',
        'hash_map_output': 'hash_map_%s.json' % sa,
        # Trigger info settings
        'trigger_info_output': '%s/trigger_info_%s.json' % (OUTPUT, sa),
        # Scale factors workspace
        'sf_wsp': 'input/scale_factors/htt_scalefactors_v3.root',
        # ZmtTP decay mode selection
        'ZmtTP_tauDM': 'decayModeFinding'
    }
    job_mgr.add_filelist_split_jobs(
        prog=basedir+'/bin/HTT2016Studies',
        cfg=cfg,
        files_per_job=FILES_PER_JOB,
        output_cfgs=['output_name', 'lumi_out', 'trigger_info_output'])
    job_mgr.task_name = task + '-' + sa
    job_mgr.flush_queue()
