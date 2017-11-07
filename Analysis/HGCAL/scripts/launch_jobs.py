from UserCode.ICHiggsTauTau.jobs import Jobs
import argparse
import os

job_mgr = Jobs()
parser = argparse.ArgumentParser()
job_mgr.attach_job_args(parser)
args = parser.parse_args()
job_mgr.set_args(args)

basedir = '%s/src/UserCode/ICHiggsTauTau/Analysis/HGCAL' % os.environ[
    'CMSSW_BASE']

MAX_EVTS = -1
FILES_PER_JOB = 10

# OUTPUT = 'output/HTT2016Studies_'+job_mgr.task_name
# os.system('mkdir -p %s' % OUTPUT)

task = job_mgr.task_name

joblist = [
   # ('Pythia8PtGun_agilbert_TauPt50_100_DM1_20170928_test', ["filelists/Pythia8PtGun_agilbert_TauPt50_100_DM1_20170928.dat"]),
   # ('Pythia8PtGun_agilbert_TauPt50_100_DM1_20170928', ["filelists/Pythia8PtGun_agilbert_TauPt50_100_DM1_20170928_full.dat"]),
   #('Pythia8PtGun_agilbert_TauPt50_100_DM1_PU140_20171013', ["filelists/Pythia8PtGun_agilbert_TauPt50_100_DM1_PU140_20171013_full.dat"]),
   #('Pythia8PtGun_agilbert_TauPt50_100_DM1_PU140_20171013_pusub', ["filelists/Pythia8PtGun_agilbert_TauPt50_100_DM1_PU140_20171013_full.dat"]),
   #('Pythia8PtGun_agilbert_TauPt50_100_DM1_PU200_20171030', ["filelists/Pythia8PtGun_agilbert_TauPt50_100_DM1_PU200_20171030_full.dat"]),
   #('Pythia8PtGun_agilbert_TauPt50_100_DM1_PU200_20171030_pusub', ["filelists/Pythia8PtGun_agilbert_TauPt50_100_DM1_PU200_20171030_full.dat"]),
   #('Dijet_agilbert_DiJetFlat20_200_20171015', ["filelists/Dijet_agilbert_DiJetFlat20_200_20171015.dat"]),
   #('Dijet_agilbert_DiJetFlat20_200_PU140_20171102', ["filelists/Dijet_agilbert_DiJetFlat20_200_PU140_20171102.dat"]),
   #('Dijet_agilbert_DiJetFlat20_200_PU140_20171102_pusub', ["filelists/Dijet_agilbert_DiJetFlat20_200_PU140_20171102.dat"]),
   ('Dijet_agilbert_DiJetFlatQuarks20_200_20171106', ["filelists/Dijet_agilbert_DiJetFlatQuarks20_200_20171106.dat"]),
]
OPTS = {
  'Pythia8PtGun_agilbert_TauPt50_100_DM1_PU140_20171013_pusub': {
    's1_pu_strategy': 1
  },
  'Pythia8PtGun_agilbert_TauPt50_100_DM1_PU200_20171030_pusub': {
    's1_pu_strategy': 1
  },
  'Dijet_agilbert_DiJetFlat20_200_PU140_20171102_pusub': {
    's1_pu_strategy': 1
  }
}

for sample, filelist in joblist:

    cfg = {
        'output_name': '%s.root' % (sample),
        'filelists': filelist,
        'max_events': MAX_EVTS,
        'file_prefix': '',
        'do_fakes': False,
        's1_rechit_threshold': 5.,
        's1_pu_strategy': 0,
        's2_jet_distance': 0.2,
        's2_min_jet_pt': 15.,
        's3_min_surrounding_hits': 5,
        's3_min_lower_energy_hits': 5,
        's3_use_hcal_dxy': True,
        's3_hcal_dxy': 10.,
        's3_use_single_hit': False,
        's3_single_hit_min': 0.6,
        's4_hit_merge_dr': 0.01,
        's4_min_hits_for_prong': 3,
        's5_merge_strategy': 0,
        's5_exp_merge_scale': 0.01
    }
    if 'DiJet' in sample:
        cfg['do_fakes'] = True
    if sample in OPTS:
        cfg.update(OPTS[sample])

    job_mgr.add_filelist_split_jobs(
        prog=basedir + '/bin/HGCAL',
        cfg=cfg,
        files_per_job=FILES_PER_JOB,
        output_cfgs=['output_name'])
    #job_mgr.task_name = task + '-' + sa
job_mgr.flush_queue()
