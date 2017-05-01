from UserCode.ICHiggsTauTau.jobs import Jobs
import argparse
import os

job_mgr = Jobs()
parser = argparse.ArgumentParser()
job_mgr.attach_job_args(parser)
parser.add_argument('--pythia', action='store_true')
args = parser.parse_args()
job_mgr.set_args(args)

basedir = '%s/src/UserCode/ICHiggsTauTau/Analysis/HiggsTauTau' % os.environ[
    'CMSSW_BASE']

MAX_EVTS = -1
FILES_PER_JOB = 50
PROD=''

DATA_SAMPLES = {
}


MC_SAMPLES = {
}

tanb_dict = {
        'A': '15',
        'H': '50',
        's': '15'
        }

for mass in ['80','90','100','110','120','130','140','160','180','200','250','300','350','400','450','500','600','700','800','900','1000','1200','1400','1500','1600','1800','2000','2300','2600','2900','3200']:
    for part in ['H']:
        key = '%s_%s_%s' % (part, mass, tanb_dict[part])
        MC_SAMPLES['%s_b_b' % key]    = ['%s_b_b' % key]
        MC_SAMPLES['%s_b_tb' % key]   = ['%s_b_tb' % key]
        MC_SAMPLES['%s_t_t' % key]    = ['%s_t_t' % key]
        MC_SAMPLES['%s_t_tb' % key]   = ['%s_t_tb' % key]
        MC_SAMPLES['%s_tb_tb' % key]  = ['%s_tb_tb' % key]

if args.pythia:
    PROD='Apr3_'
    MC_SAMPLES = {
            'SUSYGluGluToHToTauTau_M-80':     ['SUSYGluGluToHToTauTau_M-80'],
            'SUSYGluGluToHToTauTau_M-90':     ['SUSYGluGluToHToTauTau_M-90'],
            'SUSYGluGluToHToTauTau_M-100':    ['SUSYGluGluToHToTauTau_M-100'],
            'SUSYGluGluToHToTauTau_M-110':    ['SUSYGluGluToHToTauTau_M-110'],
            'SUSYGluGluToHToTauTau_M-120':    ['SUSYGluGluToHToTauTau_M-120'],
            'SUSYGluGluToHToTauTau_M-130':    ['SUSYGluGluToHToTauTau_M-130'],
            'SUSYGluGluToHToTauTau_M-140':    ['SUSYGluGluToHToTauTau_M-140'],
            'SUSYGluGluToHToTauTau_M-160':    ['SUSYGluGluToHToTauTau_M-160'],
            'SUSYGluGluToHToTauTau_M-180':    ['SUSYGluGluToHToTauTau_M-180'],
            'SUSYGluGluToHToTauTau_M-200':    ['SUSYGluGluToHToTauTau_M-200'],
            'SUSYGluGluToHToTauTau_M-250':    ['SUSYGluGluToHToTauTau_M-250'],
            'SUSYGluGluToHToTauTau_M-300':    ['SUSYGluGluToHToTauTau_M-300'],
            'SUSYGluGluToHToTauTau_M-350':    ['SUSYGluGluToHToTauTau_M-350'],
            'SUSYGluGluToHToTauTau_M-400':    ['SUSYGluGluToHToTauTau_M-400'],
            'SUSYGluGluToHToTauTau_M-450':    ['SUSYGluGluToHToTauTau_M-450'],
            'SUSYGluGluToHToTauTau_M-500':    ['SUSYGluGluToHToTauTau_M-500'],
            'SUSYGluGluToHToTauTau_M-600':    ['SUSYGluGluToHToTauTau_M-600'],
            'SUSYGluGluToHToTauTau_M-700':    ['SUSYGluGluToHToTauTau_M-700'],
            'SUSYGluGluToHToTauTau_M-800':    ['SUSYGluGluToHToTauTau_M-800'],
            'SUSYGluGluToHToTauTau_M-900':    ['SUSYGluGluToHToTauTau_M-900'],
            'SUSYGluGluToHToTauTau_M-1000':   ['SUSYGluGluToHToTauTau_M-1000'],
            'SUSYGluGluToHToTauTau_M-1200':   ['SUSYGluGluToHToTauTau_M-1200'],
            'SUSYGluGluToHToTauTau_M-1400':   ['SUSYGluGluToHToTauTau_M-1400'],
            'SUSYGluGluToHToTauTau_M-1500':   ['SUSYGluGluToHToTauTau_M-1500'],
            'SUSYGluGluToHToTauTau_M-1600':   ['SUSYGluGluToHToTauTau_M-1600'],
            'SUSYGluGluToHToTauTau_M-1800':   ['SUSYGluGluToHToTauTau_M-1800'],
            'SUSYGluGluToHToTauTau_M-2000':   ['SUSYGluGluToHToTauTau_M-2000'],
            'SUSYGluGluToHToTauTau_M-2300':   ['SUSYGluGluToHToTauTau_M-2300'],
            'SUSYGluGluToHToTauTau_M-2600':   ['SUSYGluGluToHToTauTau_M-2600'],
            'SUSYGluGluToHToTauTau_M-2900':   ['SUSYGluGluToHToTauTau_M-2900'],
            'SUSYGluGluToHToTauTau_M-3200':   ['SUSYGluGluToHToTauTau_M-3200'],
    }

SAMPLES = {}
SAMPLES.update(DATA_SAMPLES)
SAMPLES.update(MC_SAMPLES)
SEQUENCES = ['Theory']
#SEQUENCES = ['Zmm', 'ZmmTP', 'Zee', 'ZeeTP', 'ZmtTP', 'ZmtTP/scale_t_hi', 'ZmtTP/scale_t_lo', 'EffectiveEvents']
#SEQUENCES = ['HashMap']

if 'HashMap' in SEQUENCES:
  FILES_PER_JOB = 1E6

WHITELIST = {
    'Theory': list(MC_SAMPLES.keys())
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
        'file_prefix': '/nfs/dust/cms/user/agilbert/projects/mssm-nlo/CMSSW_7_1_20/src/test/my_gg_H_2HDM/MSSM-NLO/', 
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
        'ZmtTP_tauDM': 'decayModeFinding',
        'sample_mass': ''
        #'ZmtTP_tauDM': 'decayModeFindingNewDMs'
    }
    if args.pythia:
        cfg['file_prefix'] = ''
        cfg['sample_mass'] = sa.split('-')[1]
    job_mgr.add_filelist_split_jobs(
        prog=basedir+'/bin/HTT2016Studies',
        cfg=cfg,
        files_per_job=FILES_PER_JOB,
        output_cfgs=['output_name', 'lumi_out', 'trigger_info_output'])
    #job_mgr.task_name = task + '-' + sa
job_mgr.flush_queue()
