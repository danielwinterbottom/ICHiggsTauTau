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

MAX_EVTS = 50
FILES_PER_JOB = 1000

# OUTPUT = 'output/HTT2016Studies_'+job_mgr.task_name
# os.system('mkdir -p %s' % OUTPUT)

task = job_mgr.task_name

joblist = [
   #('Pythia8PtGun_agilbert_JetPt15_20170710', ["filelists/Pythia8PtGun_agilbert_JetPt15_20170710.dat"]),
   # ('Pythia8PtGun_agilbert_JetPt30_20170710', ["filelists/Pythia8PtGun_agilbert_JetPt30_20170710.dat"]),
   # ('Pythia8PtGun_agilbert_TauPt45_201708', ["filelists/Pythia8PtGun_agilbert_TauPt45_201708.dat"]),
   ('Pythia8PtGun_agilbert_TauPt50_100_DM1_20170928', ["filelists/Pythia8PtGun_agilbert_TauPt50_100_DM1_20170928.dat"]),
   # ('Pythia8PtGun_agilbert_TauPt45_20170913', ["filelists/Pythia8PtGun_agilbert_TauPt45_20170913.dat"]),
   #('Pythia8PtGun_agilbert_JetPt50_20170808', ["filelists/Pythia8PtGun_agilbert_JetPt50_20170808.dat"]),
   #('Pythia8PtGun_agilbert_JetPt100_20170808', ["filelists/Pythia8PtGun_agilbert_JetPt100_20170808.dat"]),
   #('Pythia8PtGun_agilbert_JetPt200_20170808', ["filelists/Pythia8PtGun_agilbert_JetPt200_20170808.dat"]),
   #('Pythia8PtGun_agilbert_JetPt50_200PU_20170809', ["filelists/Pythia8PtGun_agilbert_JetPt50_200PU_20170809.dat"]),
   #('Dijet_agilbert_DiJet50_20170815', ["filelists/Dijet_agilbert_DiJet50_20170815.dat"]),
  # ('Dijet_test', ["filelists/filelist_dijet_test.dat"]),
]

for sample, filelist in joblist:

    cfg = {
        'output_name': '%s.root' % (sample),
        'filelists': filelist,
        'max_events': MAX_EVTS,
        'file_prefix': ''
    }

    job_mgr.add_filelist_split_jobs(
        prog=basedir + '/bin/HGCAL',
        cfg=cfg,
        files_per_job=FILES_PER_JOB,
        output_cfgs=['output_name'])
    #job_mgr.task_name = task + '-' + sa
job_mgr.flush_queue()
