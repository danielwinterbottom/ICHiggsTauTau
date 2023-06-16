#!/usr/bin/env python

# python scripts/htt_mssm_2016.py --bkg --data --embed --sm --mssm --jetmetuncerts --scales="default,scale_t_0pi,scale_t_1pi,scale_t_3prong,scale_t_3prong1pi0,scale_efake_0pi,scale_efake_1pi,scale_mufake_0pi,scale_mufake_1pi,scale_e" --submit='./scripts/submit_ic_batch_job.sh "hep.q -l h_rt=0:180:0 -l h_vmem=24G"'

# importing libraries
import sys
from optparse import OptionParser
import os
import math
import json

# setting up job stuff and condor template
# ----------------------------------------
os.system("voms-proxy-init --voms cms --valid 192:00 --out ~/cms.proxy")

JOBWRAPPER      = './scripts/generate_job.sh'
JOBSUBMIT       = 'true'
if "JOBWRAPPER" in os.environ:      JOBWRAPPER      = os.environ["JOBWRAPPER"]
if "JOBSUBMIT"  in os.environ:      JOBSUBMIT       = os.environ["JOBSUBMIT"]
print "Using job-wrapper:    " + JOBWRAPPER
print "Using job-submission: " + JOBSUBMIT

CONDOR_TEMPLATE = """executable = ./jobs/%(EXE)s
Proxy_path =/afs/cern.ch/user/a/adow/private/x509up
arguments = $(Proxy_path)
output                = /dev/null
error                 = /dev/null
log                   = ./jobs/%(TASK)s.$(ClusterId).log
requirements = (OpSysAndVer =?= "SLCern6")
+JobFlavour     = "longlunch"
queue
"""
# ----------------------------------------

# parsing options
# ----------------------------------------
def split_callback(option, opt, value, parser):
  setattr(parser.values, option.dest, value.split(','))

parser = OptionParser()
parser.add_option("--wrapper", dest="wrapper",
                  help="Specify the job-wrapper script. The current wrapper is '%(JOBWRAPPER)s'."
                  " Using the --wrapper option overrides both the default and the environment variable. " % vars())

parser.add_option("--submit", dest="submit",
                  help="Specify the job-submission method. The current method is '%(JOBSUBMIT)s'"
                  " Using the --submit option overrides both the default and the environment variable. " % vars())

parser.add_option("--data", dest="proc_data", action='store_true', default=False,
                  help="Process data samples")

parser.add_option("--calc_lumi", dest="calc_lumi", action='store_true', default=False,
                  help="Run on data and only write out lumi mask jsons")

parser.add_option("--bkg", dest="proc_bkg", action='store_true', default=False,
                  help="Process background mc samples")

parser.add_option("--all", dest="proc_all", action='store_true', default=False,
                  help="Process all samples")

parser.add_option("--no_json", dest="no_json", action='store_true', default=False,
                  help="Do not read the channels to process from the json to decide which datasets to run on")

parser.add_option("--scales", dest="scales", type='string', default='default',
                  help="List of systematic shifts to process")

parser.add_option("--parajobs", dest="parajobs", action='store_true', default=False,
                  help="Submit jobs parametrically")

parser.add_option("--config", dest="config", type='string', default='scripts/config_UL_2016_postVFP.json',
                  help="Config file")

parser.add_option("--list_backup", dest="slbackupname", type='string', default='prevlist',
                  help="Name you want to give to the previous files_per_samples file, in case you're resubmitting a subset of jobs")

parser.add_option("--condor", action='store_true', default=False,
                  help="Submit jobs to condor (for lxplus)")

parser.add_option("--effective_events", action='store_true', default=False,
                  help="Generating effective event numbers for processing all samples")

parser.add_option("--jetmetuncerts", dest="jetmetuncerts", action='store_true', default=False,
                  help="Do JES, JER, and MET uncertainties")

parser.add_option("--embed", dest="proc_embed", action='store_true', default=False,
                  help="Process embedded sampes")
# ----------------------------------------
(options,args) = parser.parse_args()
if options.wrapper: JOBWRAPPER=options.wrapper
if options.submit:  JOBSUBMIT=options.submit
if options.condor: JOBWRAPPER = "./scripts/generate_condor_job.sh"

jetuncert_string=''
if options.jetmetuncerts:
  jetuncert_string='\\"do_jetmet_uncerts\\":true'
else:
  jetuncert_string='\\"do_jetmet_uncerts\\":false'

def getParaJobSubmit(N):
  if not options.submit: return 'true'
  sub_opts=JOBSUBMIT.split(' ',1)[1]
  if '\"' in sub_opts: sub_opts = sub_opts.replace('\"','')
  sub_opts = '%s \"%s -t 1-%i:1\"' %(JOBSUBMIT.split(' ',1)[0], sub_opts, N)
  print sub_opts
  return sub_opts

BACKUPNAME = options.slbackupname
scales = options.scales
parajobs = options.parajobs


scale_list = scales.split(',')
flatjsonlist = []
flatjsonlistdysig = []
flatjsonlist.append("job:sequences:all:")
n_scales=0
for scale in scale_list: 
  n_scales+=1  
  if scale == "default":
    flatjsonlist.append("^%(scale)s"%vars())
    flatjsonlistdysig.append("^%(scale)s"%vars())
  else:
    n_scales+=1
    flatjsonlist.append("^%(scale)s_hi^%(scale)s_lo"%vars()) 
    flatjsonlistdysig.append("^%(scale)s_hi^%(scale)s_lo"%vars()) 
 
CONFIG='scripts/config_UL_2016_postVFP.json'
if options.config != '': CONFIG = options.config

n_channels=1
output_folder = ""
svfit_folder = ""
svfit_mode = 0
with open(CONFIG,"r") as input:
  with open ("config_for_python_channels.json","w") as output:
    for line in input:
      if not '//' in line:
        output.write(line)
    output.close()
  input.close()

with open("config_for_python_channels.json") as config_file:
  cfg = json.load(config_file)
  n_channels=len(cfg["job"]["channels"])
  output_folder = cfg["sequence"]["output_folder"]
  svfit_mode = cfg["sequence"]["new_svfit_mode"]
  svfit_folder = cfg["sequence"]["svfit_folder"]

# makes sure output folder(s) (and svfit folder(s) if needed) is always created
os.system("bash scripts/make_output_folder.sh {}".format(output_folder))
if svfit_mode == 1:
    os.system("bash scripts/make_output_folder.sh {}".format(svfit_folder))
  
#scale = int(math.ceil(float(n_scales*n_channels)/50))
scale = int(math.ceil(float(n_scales*n_channels)/30))

if scale < 1: scale = 1

total = float(len(flatjsonlistdysig))
flatjsons = []
if options.jetmetuncerts:
# when we do the jet met uncertainties we do not want to run additional systematics in the same job 
  for i in flatjsonlistdysig:
    if 'default' in i:
      flatjsons.append('job:sequences:all:'+i)
      flatjsonlistdysig.remove(i)
# split into seperate jobs if number of scales is over a value
for i in range(0,scale):
   first = i*int(math.ceil(total/scale))
   last = (i+1)*int(math.ceil(total/scale))
   temp=''.join(flatjsonlistdysig[first:last]) 
   if temp == '': continue
   temp='job:sequences:all:'+temp
   flatjsons.append(temp)
 
FILELIST="filelists/Mar31_2016-postVFP_MC_124X"

signal_mc = [ ]
signal_vh = [ ] 
signal_mc_ww = [ ]

if os.path.isfile("./jobs/files_per_sample_2016_postVFP.txt"):
  os.system("mv ./jobs/files_per_sample_2016_postVFP.txt ./jobs/files_per_sample_2016_postVFP-%(BACKUPNAME)s.txt"%vars())

file_persamp = open("./jobs/files_per_sample_2016_postVFP.txt", "w")



if options.proc_data or options.proc_all or options.calc_lumi or options.proc_embed:
    with open(CONFIG,"r") as input:
      with open ("config_for_python.json","w") as output:
        for line in input:
          if not '//' in line:
            output.write(line)
        output.close()
      input.close()

    with open("config_for_python.json") as config_file:
      cfg = json.load(config_file)

    channels=cfg["job"]["channels"]

if options.proc_data or options.proc_all or options.calc_lumi:  

  data_samples = []
  data_eras = ['F','G','H']
  for chn in channels:
    for era in data_eras:
      if 'mt' in chn or 'zmm' in chn:
           data_samples+=['SingleMuon'+era] 
      if 'et' in chn or 'zee' in chn:
           data_samples+=['SingleElectron'+era]
      if 'em' in chn:
           data_samples+=['MuonEG'+era]
      if 'tt' in chn:
           data_samples+=['Tau'+era]
    if ('mt' in channels or 'et' in channels) and 'tt' not in channels:
      for era in data_eras:
        data_samples+=['Tau'+era]        

    if options.effective_events: # this is just used for the effective events case
        for era in data_eras:
          if 'SingleMuon'+era not in data_samples: data_samples+=['SingleMuon'+era]
          if 'EGamma'+era not in data_samples: data_samples+=['EGamma'+era]
          if 'Tau'+era not in data_samples: data_samples+=['Tau'+era]
          if 'MuonEG'+era not in data_samples: data_samples+=['MuonEG'+era]
          if 'DoubleMuon'+era not in data_samples: data_samples+=['DoubleMuon'+era]

  DATAFILELIST="./filelists/Mar31_2016-postVFP_Data_124X"

  for sa in data_samples:
      JOB='%s_2016_postVFP' % (sa)
      DATAFILELIST_ = DATAFILELIST
      user='dwinterb'
      prefix='Mar31_Data_124X_2016-postVFP'
      JSONPATCH= (r"'{\"job\":{\"filelist\":\"%(DATAFILELIST_)s_%(sa)s.dat\",\"file_prefix\":\"root://gfe02.grid.hep.ph.ic.ac.uk:1097//store/user/%(user)s/%(prefix)s/\",\"sequences\":{\"em\":[],\"et\":[],\"mt\":[],\"tt\":[],\"zmm\":[],\"zee\":[]}}, \"sequence\":{\"output_name\":\"%(JOB)s\",\"is_data\":true}}' "%vars());
      nfiles = sum(1 for line in open('%(DATAFILELIST_)s_%(sa)s.dat' % vars()))
      nperjob = 40
      for i in range (0,int(math.ceil(float(nfiles)/float(nperjob)))) :  
        os.system('%(JOBWRAPPER)s "./bin/HTT --cfg=%(CONFIG)s --json=%(JSONPATCH)s --offset=%(i)d --nlines=%(nperjob)d &> jobs/%(JOB)s-%(i)d.log" jobs/%(JOB)s-%(i)s.sh' %vars())
        if not parajobs and not options.condor:
            os.system('%(JOBSUBMIT)s jobs/%(JOB)s-%(i)d.sh' % vars())
        elif not parajobs and options.condor:
            outscriptname = '{}-{}.sh'.format(JOB, i)
            subfilename = '{}_{}.sub'.format(JOB, i)
            subfile = open("jobs/{}".format(subfilename), "w")
            condor_settings = CONDOR_TEMPLATE % {
              'EXE': outscriptname,
              'TASK': "{}-{}".format(JOB, i)
            }
            subfile.write(condor_settings)
            subfile.close()
            os.system('condor_submit jobs/{}'.format(subfilename))

      if parajobs: 
        os.system('%(JOBWRAPPER)s ./jobs/%(JOB)s-\$\(\(SGE_TASK_ID-1\)\).sh  jobs/parajob_%(JOB)s.sh' %vars())
        PARAJOBSUBMIT = getParaJobSubmit(int(math.ceil(float(nfiles)/float(nperjob))))
        os.system('%(PARAJOBSUBMIT)s jobs/parajob_%(JOB)s.sh' % vars())  
      file_persamp.write("%s %d\n" %(JOB, int(math.ceil(float(nfiles)/float(nperjob)))))

if options.proc_embed or options.proc_all:

    embed_samples = []
    data_eras = ['F','G','H']
    for chn in channels:
        for era in data_eras:
            if 'em' in chn:
                embed_samples+=['EmbeddingElMu2016'+era]
            if 'et' in chn:
                embed_samples+=['EmbeddingElTau2016'+era]
            if 'mt' in chn:
                embed_samples+=['EmbeddingMuTau2016'+era]
            if 'tt' in chn:
                embed_samples+=['EmbeddingTauTau2016'+era]
            if 'zmm' in chn:
                embed_samples+=['EmbeddingMuMu2016'+era]
            if 'zee' in chn:
                embed_samples+=['EmbeddingElEl2016'+era]

    EMBEDFILELIST="./filelists/May25_2016-postVFP_MC_124X"

    for sa in embed_samples:
        job_num=0
        JOB='%s_2016_postVFP' % (sa)
        JSONPATCH= (r"'{\"job\":{\"filelist\":\"%(EMBEDFILELIST)s_%(sa)s.dat\",\"file_prefix\":\"root://gfe02.grid.hep.ph.ic.ac.uk:1097//store/user/dwinterb/May25_EMB_124X_2016-postVFP/\",\"sequences\":{\"em\":[],\"et\":[],\"mt\":[],\"tt\":[],\"zmm\":[],\"zee\":[]}}, \"sequence\":{\"output_name\":\"%(JOB)s\",\"is_embedded\":true,%(jetuncert_string)s}}' "%vars());
        for FLATJSONPATCH in flatjsons:
            nperjob = 20
            FLATJSONPATCH = FLATJSONPATCH.replace('^scale_j_hi^scale_j_lo','').replace('^scale_j_hf_hi^scale_j_hf_lo','').replace('^scale_j_cent_hi^scale_j_cent_lo','').replace('^scale_j_full_hi^scale_j_full_lo','').replace('^scale_j_relbal_hi^scale_j_relbal_lo','').replace('^scale_j_relsamp_hi^scale_j_relsamp_lo','').replace('^scale_j_relbal_hi^scale_j_relbal_lo','').replace('^scale_j_abs_hi^scale_j_abs_lo','').replace('^scale_j_abs_year_hi^scale_j_abs_year_lo','').replace('^scale_j_flav_hi^scale_j_flav_lo','').replace('^scale_j_bbec1_hi^scale_j_bbec1_lo','').replace('^scale_j_bbec1_year_hi^scale_j_bbec1_year_lo','').replace('^scale_j_ec2_hi^scale_j_ec2_lo','').replace('^scale_j_ec2_year_hi^scale_j_ec2_year_lo','').replace('^scale_j_hf_hi^scale_j_hf_lo','').replace('^scale_j_hf_year_hi^scale_j_hf_year_lo','').replace('^scale_j_relsamp_year_hi^scale_j_relsamp_year_lo','').replace('^res_j_hi^res_j_lo','')

            FLATJSONPATCH = FLATJSONPATCH.replace('^scale_efake_0pi_hi^scale_efake_0pi_lo','').replace('^scale_efake_1pi_hi^scale_efake_1pi_lo','').replace('^scale_mufake_0pi_hi^scale_mufake_0pi_lo','').replace('^scale_mufake_1pi_hi^scale_mufake_1pi_lo','').replace('^met_cl_hi^met_cl_lo','').replace('^met_uncl_hi^met_uncl_lo','').replace('^scale_met_hi^scale_met_lo','').replace('^res_met_hi^res_met_lo','').replace('^scale_met_njets0_hi^scale_met_njets0_lo','').replace('^res_met_njets0_hi^res_met_njets0_lo','').replace('^scale_met_njets1_hi^scale_met_njets1_lo','').replace('^res_met_njets1_hi^res_met_njets1_lo','').replace('^scale_met_njets2_hi^scale_met_njets2_lo','').replace('^res_met_njets2_hi^res_met_njets2_lo','')
            if 'TauTau' in  sa: FLATJSONPATCH = FLATJSONPATCH.replace('^scale_e_hi^scale_e_lo','').replace('^scale_mu_hi^scale_mu_lo','').replace('^scale_t_hi^scale_t_lo','')
            if 'ElMu' in  sa: FLATJSONPATCH = FLATJSONPATCH.replace('^scale_t_0pi_hi^scale_t_0pi_lo','').replace('^scale_t_1pi_hi^scale_t_1pi_lo','').replace('^scale_t_3prong_hi^scale_t_3prong_lo','').replace('^scale_t_3prong1pi0_hi^scale_t_3prong1pi0_lo','')
            if 'MuTau' in  sa: FLATJSONPATCH = FLATJSONPATCH.replace('^scale_e_hi^scale_e_lo','').replace('^scale_t_hi^scale_t_lo','')
            if 'ElTau' in  sa: FLATJSONPATCH = FLATJSONPATCH.replace('^scale_mu_hi^scale_mu_lo','').replace('^scale_t_hi^scale_t_lo','')
            if FLATJSONPATCH == 'job:sequences:all:^^' or FLATJSONPATCH == 'job:sequences:all:': continue

            n_scales = FLATJSONPATCH.count('_lo')*2 + FLATJSONPATCH.count('default')

            if options.jetmetuncerts and 'default' in FLATJSONPATCH: n_scales +=2

            if n_scales*n_channels>=24: nperjob = 10
            if n_scales*n_channels>=48: nperjob=5

            nfiles = sum(1 for line in open('%(EMBEDFILELIST)s_%(sa)s.dat' % vars()))
            for i in range (0,int(math.ceil(float(nfiles)/float(nperjob)))) :
                os.system('%(JOBWRAPPER)s "./bin/HTT --cfg=%(CONFIG)s --json=%(JSONPATCH)s --flatjson=%(FLATJSONPATCH)s --offset=%(i)d --nlines=%(nperjob)d &> jobs/%(JOB)s-%(job_num)d.log" jobs/%(JOB)s-%(job_num)s.sh' %vars())
                if not parajobs and not options.condor:
                    os.system('%(JOBSUBMIT)s jobs/%(JOB)s-%(job_num)d.sh' % vars())
                elif not parajobs and options.condor:
                    outscriptname = '{}-{}.sh'.format(JOB, job_num)
                    subfilename = '{}_{}.sub'.format(JOB, job_num)
                    subfile = open("jobs/{}".format(subfilename), "w")
                    condor_settings = CONDOR_TEMPLATE % {
                      'EXE': outscriptname,
                      'TASK': "{}-{}".format(JOB, job_num)
                    }
                    subfile.write(condor_settings)
                    subfile.close()
                    os.system('condor_submit jobs/{}'.format(subfilename))

                job_num+=1
            file_persamp.write("%s %d\n" %(JOB, int(math.ceil(float(nfiles)/float(nperjob)))))
        if parajobs:
            os.system('%(JOBWRAPPER)s ./jobs/%(JOB)s-\$\(\(SGE_TASK_ID-1\)\).sh  jobs/parajob_%(JOB)s.sh' %vars())
            PARAJOBSUBMIT = getParaJobSubmit(job_num)
            os.system('%(PARAJOBSUBMIT)s jobs/parajob_%(JOB)s.sh' % vars())

if options.proc_bkg or options.proc_all:
  central_samples = [
    # Drell-Yan LO
    'DY1JetsToLL-LO',
    'DY2JetsToLL-LO',
    'DY3JetsToLL-LO',
    'DY4JetsToLL-LO',
    'DYJetsToLL-LO',

    # Low mass Drell Yan LO
    'DYJetsToLL_M-10to50-LO',

    # Drell-Yan NLO
    'DYJetsToLL_0J-NLO',
    'DYJetsToLL_1J-NLO',
    'DYJetsToLL_2J-NLO',
    'DYJetsToLL-NLO',

    # Electroweak W and Z
    'EWKWMinus2Jets_WToLNu',
    'EWKWPlus2Jets_WToLNu',
    'EWKZ2Jets_ZToLL',

    # W + Jets L0
    'WJetsToLNu-LO',
    'W1JetsToLNu-LO',
    'W2JetsToLNu-LO',
    'W3JetsToLNu-LO',
    'W4JetsToLNu-LO',

#    # W + Jets NLO
#    'WJetsToLNu_0J-NLO',
#    'WJetsToLNu_1J-NLO',
#    'WJetsToLNu_2J-NLO',
#    'WJetsToLNu-NLO',

    # ttbar
    'TTTo2L2Nu',
    'TTToHadronic',
    'TTToSemiLeptonic',

    # Split diboson (Missing Files: WZTo1L3Nu, WZTo1L1Nu2Q, WWTo1L1Nu2Q)
    'WZTo2Q2L',
    'WZTo3LNu',
    'WWTo2L2Nu',
    'ZZTo2L2Nu',
    'ZZTo4L', 
    'WZTo1L3Nu',
    'WZTo1L1Nu2Q',
    'WWTo1L1Nu2Q',

#    # Triboson
#    'WWZ',
#    'WWZ-ext1',
#    'WZZ',
#    'WZZ-ext1',
#    'WWW',
#    'WWW-ext1',
#    'ZZZ',
#    'ZZZ-ext1',

    # Other backgrounds
    'WGToLNuG',
    'Tbar-t',
    'Tbar-tW',
    'T-t',
    'T-tW',

#    # SM Higgs
#    'GluGluHToTauTau_M125',
#    'VBFHToTauTau_M125',
#    'WminusHToTauTau_M125',
#    'WplusHToTauTau_M125',
#    'ZHToTauTau_M125',
#    'ttHToTauTau_M125',

#     'QCD_Pt-20_MuEnrichedPt15',
#
#     'QCD_Pt-1000_MuEnrichedPt5',
#     'QCD_Pt-120To170_MuEnrichedPt5',
#     'QCD_Pt-15To20_MuEnrichedPt5',
#     'QCD_Pt-170To300_MuEnrichedPt5',
#     'QCD_Pt-20To30_MuEnrichedPt5',
#     'QCD_Pt-300To470_MuEnrichedPt5',
#     'QCD_Pt-30To50_MuEnrichedPt5',
#     'QCD_Pt-470To600_MuEnrichedPt5',
#     'QCD_Pt-50To80_MuEnrichedPt5',
#     'QCD_Pt-600To800_MuEnrichedPt5',
#     'QCD_Pt-800To1000_MuEnrichedPt5',
#     'QCD_Pt-80To120_MuEnrichedPt5',
  ] 

  if options.effective_events: # this is just used for the effective events case
    central_samples += [
                        'VBF_HToZZTo4L_M125',
                        'GluGlu_HToZZTo4L_M125',
                        'GluGluToContinToZZTo2e2mu',
                        'GluGluToContinToZZTo2e2tau',
                        'GluGluToContinToZZTo2mu2tau',
                        'GluGluToContinToZZTo4e',
                        'GluGluToContinToZZTo4mu',
                        'GluGluToContinToZZTo4tau',
                        ]


  for sa in central_samples:
      JOB='%s_2016_postVFP' % (sa)
      user='dwinterb'
      PREFIX='Mar31_MC_124X_2016-postVFP'
      if 'MuEnrichedPt5' in sa:
        PREFIX='May25_MC_124X_2016-postVFP'
        FILELIST='filelists/May25_2016-postVFP_MC_124X'
      elif 'MuEnrichedPt15' in sa:
        PREFIX='Apr20_MC_124X_2016-postVFP'
        FILELIST='filelists/Apr20_2016-postVFP_MC_124X'
      else:
        PREFIX='Mar31_MC_124X_2016-postVFP'
        FILELIST='filelists/Mar31_2016-postVFP_MC_124X'

      JSONPATCH= (r"'{\"job\":{\"filelist\":\"%(FILELIST)s_%(sa)s.dat\", \"file_prefix\":\"root://gfe02.grid.hep.ph.ic.ac.uk:1097//store/user/%(user)s/%(PREFIX)s/\"}, \"sequence\":{\"output_name\":\"%(JOB)s\",%(jetuncert_string)s}}' "%vars());

      job_num=0
      for FLATJSONPATCH in flatjsons: 
        nperjob = 20
        if 'TT' in sa or 'VVTo2L2Nu' in sa: nperjob = 10
        if 'DY' not in sa and 'EWKZ' not in sa:
          FLATJSONPATCH = FLATJSONPATCH.replace('^scale_efake_0pi_hi^scale_efake_0pi_lo','').replace('^scale_efake_1pi_hi^scale_efake_1pi_lo','').replace('^scale_mufake_0pi_hi^scale_mufake_0pi_lo','').replace('^scale_mufake_1pi_hi^scale_mufake_1pi_lo','')
        if 'DY' not in sa and 'JetsToLNu' not in sa and 'WG' not in sa and 'EWKZ' not in sa and 'EWKW' not in sa:
          FLATJSONPATCH = FLATJSONPATCH.replace('^scale_met_hi^scale_met_lo','').replace('^res_met_hi^res_met_lo','').replace('^scale_met_njets0_hi^scale_met_njets0_lo','').replace('^res_met_njets0_hi^res_met_njets0_lo','').replace('^scale_met_njets1_hi^scale_met_njets1_lo','').replace('^res_met_njets1_hi^res_met_njets1_lo','').replace('^scale_met_njets2_hi^scale_met_njets2_lo','').replace('^res_met_njets2_hi^res_met_njets2_lo','')
        n_scales = FLATJSONPATCH.count('_lo') + FLATJSONPATCH.count('default')
        if n_scales*n_channels>=24: nperjob = 10
        if n_scales*n_channels>=48: nperjob=5
        if 'TT' in sa or 'VVTo2L2Nu' in sa:
          nperjob = 10 
          if n_scales*n_channels>24: nperjob = 5
          if n_scales*n_channels>48: nperjob = 2
        if options.jetmetuncerts and 'default' in FLATJSONPATCH: nperjob = int(math.ceil(float(nperjob)/2))

        #nperjob = int(math.ceil(float(nperjob)/max(1.,float(n_scales)*float(n_channels)/10.)))
        nfiles = sum(1 for line in open('%(FILELIST)s_%(sa)s.dat' % vars()))
        for i in range (0,int(math.ceil(float(nfiles)/float(nperjob)))) :
          os.system('%(JOBWRAPPER)s "./bin/HTT --cfg=%(CONFIG)s --json=%(JSONPATCH)s --flatjson=%(FLATJSONPATCH)s --offset=%(i)d --nlines=%(nperjob)d &> jobs/%(JOB)s-%(job_num)d.log" jobs/%(JOB)s-%(job_num)s.sh' %vars())
          if not parajobs and not options.condor:
              os.system('%(JOBSUBMIT)s jobs/%(JOB)s-%(job_num)d.sh' % vars())
          elif not parajobs and options.condor:
              outscriptname = '{}-{}.sh'.format(JOB, job_num)
              subfilename = '{}_{}.sub'.format(JOB, job_num)
              subfile = open("jobs/{}".format(subfilename), "w")
              condor_settings = CONDOR_TEMPLATE % {
                'EXE': outscriptname,
                'TASK': "{}-{}".format(JOB, job_num)
              }
              subfile.write(condor_settings)
              subfile.close()
              os.system('condor_submit jobs/{}'.format(subfilename))
          job_num+=1
        file_persamp.write("%s %d\n" %(JOB, int(math.ceil(float(nfiles)/float(nperjob)))))
      if parajobs: 
        os.system('%(JOBWRAPPER)s ./jobs/%(JOB)s-\$\(\(SGE_TASK_ID-1\)\).sh  jobs/parajob_%(JOB)s.sh' %vars())
        PARAJOBSUBMIT = getParaJobSubmit(job_num)
        os.system('%(PARAJOBSUBMIT)s jobs/parajob_%(JOB)s.sh' % vars()) 

