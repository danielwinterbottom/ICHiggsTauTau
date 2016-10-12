from UserCode.ICHiggsTauTau.jobs import Jobs
import argparse
import os
import json

job_mgr = Jobs()
parser = argparse.ArgumentParser()
job_mgr.attach_job_args(parser)
args = parser.parse_args()
job_mgr.set_args(args)

basedir = '%s/src/UserCode/ICHiggsTauTau/Analysis/HiggsTauTau' % os.environ[
    'CMSSW_BASE']

MAX_EVTS = -1
FILES_PER_JOB = 30
PROD='Aug16'

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
    'DYJetsToLL':               ['DYJetsToLL'],
    'DYJetsToLL-Old':               ['DYJetsToLL-Old'],
    'DYJetsToLLSoup':           ['DYJetsToLL', 'DY1JetsToLL', 'DY2JetsToLL', 'DY3JetsToLL', 'DY4JetsToLL'],
    'DYJetsToLL-NLO':           ['DYJetsToLL-NLO'],
    'DYJetsToLL_M-10to50':      ['DYJetsToLL_M-10to50'],
    'EWKWMinus2Jets_WToLNu':    ['EWKWMinus2Jets_WToLNu'],
    'EWKWPlus2Jets_WToLNu':     ['EWKWPlus2Jets_WToLNu'],
    'EWKZ2Jets_ZToLL':          ['EWKZ2Jets_ZToLL'],
    'TT':                       ['TT'],
    'ST_t-channel_antitop':     ['ST_t-channel_antitop'],
    'ST_t-channel_top':         ['ST_t-channel_top'],
    'ST_tW_antitop':            ['ST_tW_antitop'],
    'ST_tW_top':                ['ST_tW_top'],
    'VVTo2L2Nu':                ['VVTo2L2Nu'],
    'WWTo1L1Nu2Q':              ['WWTo1L1Nu2Q'],
    'WZJToLLLNu':               ['WZJToLLLNu'],
    'WZTo1L1Nu2Q':              ['WZTo1L1Nu2Q'],
    'WZTo1L3Nu':                ['WZTo1L3Nu'],
    'WZTo2L2Q':                 ['WZTo2L2Q'],
    'ZZTo2L2Q':                 ['ZZTo2L2Q'],
    'ZZTo4L':                   ['ZZTo4L'],
    'WJetsToLNu':               ['WJetsToLNu'],
    'WJetsToLNuSoup':           ['WJetsToLNu', 'W1JetsToLNu', 'W2JetsToLNu', 'W3JetsToLNu', 'W4JetsToLNu']
}

with open('scripts/params_%s.json' % PROD) as jsonfile:
    params = json.load(jsonfile)

SAMPLE_CFG = {
    'DYJetsToLL': {
        'do_zpt_reweighting': True
    },
    'TT': {
        'do_top_reweighting': True
    },
    'DYJetsToLLSoup': {
        'do_zpt_reweighting': True,
        'do_dyjets_stitching': True,
        'dyjets_stitching': {}
    },
    'WJetsToLNuSoup': {
        'do_wjets_stitching': True,
        'wjets_stitching': {}
    }
}
for x in ['', '1', '2', '3', '4']:
    SAMPLE_CFG['DYJetsToLLSoup']['dyjets_stitching']['evt_DY%sJetsToLL' % x] = params['DY%sJetsToLL' % x]['evt']
    SAMPLE_CFG['DYJetsToLLSoup']['dyjets_stitching']['xs_DY%sJetsToLL' % x] = params['DY%sJetsToLL' % x]['lo_xs']
    SAMPLE_CFG['WJetsToLNuSoup']['wjets_stitching']['evt_W%sJetsToLNu' % x] = params['W%sJetsToLNu' % x]['evt']
    SAMPLE_CFG['WJetsToLNuSoup']['wjets_stitching']['xs_W%sJetsToLNu' % x] = params['W%sJetsToLNu' % x]['lo_xs']

SAMPLES = {}
SAMPLES.update(DATA_SAMPLES)
SAMPLES.update(MC_SAMPLES)
#SEQUENCES = ['Zmm', 'Zmm/scale_m_hi', 'Zmm/scale_m_lo']
#SEQUENCES = ['SM_et', 'SM_mt', 'SM_em', 'SM_tt']
SEQUENCES = ['ZmtTP', 'ZmtTP/scale_t_hi', 'ZmtTP/scale_t_lo']
#SEQUENCES = ['Zmm', 'ZmmTP', 'Zee', 'ZeeTP', 'ZmtTP', 'ZmtTP/scale_t_hi', 'ZmtTP/scale_t_lo', 'EffectiveEvents']
#SEQUENCES = ['HashMap']

if 'HashMap' in SEQUENCES:
  FILES_PER_JOB = 1E6

if SEQUENCES == ['EffectiveEvents']:
  actual_samples = {}
  for subsamples in MC_SAMPLES.values():
    for subsample in subsamples:
      actual_samples[subsample] = [subsample]
  SAMPLES = actual_samples
  FILES_PER_JOB = 100

if SEQUENCES == ['DYDebug']:
    SAMPLES = {
        'DYJetsToLL':       ['DYJetsToLL'],
        'DYJetsToLL-Old':   ['DYJetsToLL-Old'],
        'DY1JetsToLL':      ['DY1JetsToLL'],
        'DY2JetsToLL':      ['DY2JetsToLL'],
        'DY3JetsToLL':      ['DY3JetsToLL'],
        'DY4JetsToLL':      ['DY4JetsToLL']
    }
    for key in SAMPLES:
        SAMPLE_CFG[key] = SAMPLE_CFG['DYJetsToLLSoup']


WHITELIST = {
    'Zmm': ['SingleMuon'] + list(MC_SAMPLES.keys()),
    'Zmm/scale_m_hi': list(MC_SAMPLES.keys()),
    'Zmm/scale_m_lo': list(MC_SAMPLES.keys()),
    'Zmm': ['SingleMuon'] + list(MC_SAMPLES.keys()),
    'ZmmTP': ['SingleMuon'] + list(MC_SAMPLES.keys()),
    'Zee': ['SingleElectron'] + list(MC_SAMPLES.keys()),
    'ZeeTP': ['SingleElectron'] + list(MC_SAMPLES.keys()),
    'ZmtTP': ['SingleMuon'] + list(MC_SAMPLES.keys()),
    'ZmtTP/scale_t_hi': list(MC_SAMPLES.keys()),
    'ZmtTP/scale_t_lo': list(MC_SAMPLES.keys()),
    'SM_et': ['SingleElectron'] + list(MC_SAMPLES.keys()),
    'SM_mt': ['SingleMuon'] + list(MC_SAMPLES.keys()),
    'SM_em': ['MuonEG'] + list(MC_SAMPLES.keys()),
    'SM_tt': ['Tau'] + list(MC_SAMPLES.keys()),
    'EffectiveEvents': list(SAMPLES.keys()),
    'HashMap': list(DATA_SAMPLES.keys()),
    'DYDebug': list(SAMPLES.keys())
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
    filelists = ['filelists/%s_%s.dat' % (PROD, X) for X in SAMPLES[sa]]
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
        'sf_wsp': 'input/scale_factors/htt_scalefactors_v6.root',
        # ZmtTP decay mode selection
        'ZmtTP_tauDM': 'decayModeFinding',
        #'ZmtTP_tauDM': 'decayModeFindingNewDMs',
        # Data/MC corrections - global flags
        'do_zpt_reweighting': False,
        'do_top_reweighting': False
    }
    if sa in SAMPLE_CFG:
        cfg.update(SAMPLE_CFG[sa])

    job_mgr.add_filelist_split_jobs(
        prog=basedir + '/bin/HTT2016Studies',
        cfg=cfg,
        files_per_job=FILES_PER_JOB,
        output_cfgs=['output_name', 'lumi_out', 'trigger_info_output'])
    job_mgr.task_name = task + '-' + sa
    job_mgr.flush_queue()
