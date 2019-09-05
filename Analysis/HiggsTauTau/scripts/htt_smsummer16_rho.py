#!/usr/bin/env python

import sys
from optparse import OptionParser
import os
import math
import json

JOBWRAPPER      = './scripts/generate_job.sh'
JOBSUBMIT       = 'true'
if "JOBWRAPPER" in os.environ:      JOBWRAPPER      = os.environ["JOBWRAPPER"]
if "JOBSUBMIT"  in os.environ:      JOBSUBMIT       = os.environ["JOBSUBMIT"]
print "Using job-wrapper:    " + JOBWRAPPER
print "Using job-submission: " + JOBSUBMIT


#CHANNELS_2015 = ['et', 'mt', 'em', 'tt']

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
parser.add_option("--embed", dest="proc_embed", action='store_true', default=False,
                  help="Process embedded sampes")
parser.add_option("--calc_lumi", dest="calc_lumi", action='store_true', default=False,
                  help="Run on data and only write out lumi mask jsons")

parser.add_option("--bkg", dest="proc_bkg", action='store_true', default=False,
                  help="Process background mc samples")

parser.add_option("--qcd", dest="proc_qcd", action='store_true', default=False,
                  help="Process QCD mc samples")

parser.add_option("--sm", dest="proc_sm", action='store_true', default=False,
                  help="Process signal SM mc samples")

parser.add_option("--smbkg", dest="proc_smbkg", action='store_true', default=False,
                  help="Process signal SM mc samples @ 125 GeV")

parser.add_option("--mssm", dest="proc_mssm", action='store_true', default=False,
                  help="Process signal MSSM mc samples")

parser.add_option("--mssm_nlo", dest="proc_mssm_nlo", action='store_true', default=False,
                  help="Process NLO signal MSSM mc samples")

parser.add_option("--mssm_nlo_qsh", dest="proc_mssm_nlo_qsh", action='store_true', default=False,
                  help="Process NLO signal Qsh Up/Down MSSM mc samples")


parser.add_option("--Hhh", dest="proc_Hhh", action='store_true', default=False,
                  help="Process signal H->hh mc samples")

parser.add_option("--extra_signal_profile", dest="extra_signal_profile", action='store_true', default=False,
                  help="Process extra signal H->hh mc samples for profiling")

parser.add_option("--all", dest="proc_all", action='store_true', default=False,
                  help="Process all samples")

parser.add_option("--short_signal", dest="short_signal", action='store_true', default=False,
                  help="Only process the 125/160 signal samples")
parser.add_option("--cp_signal", dest="cp_signal", action='store_true', default=False,
                  help="Process the samples needed for CP studies")

#parser.add_option("-c", "--channels", dest="channels", type='string', action='callback',callback=split_callback,
#                  help="A comma separated list of channels to ignore.  Supported channels: For data_2015 %(CHANNELS_2015)s" % vars())
parser.add_option("--no_json", dest="no_json", action='store_true', default=False,
                  help="Do not read the channels to process from the json to decide which datasets to run on")

parser.add_option("--list_backup", dest="slbackupname", type='string', default='prevlist',
                  help="Name you want to give to the previous files_per_samples file, in case you're resubmitting a subset of jobs")

parser.add_option("--scales", dest="scales", type='string', default='default',
                  help="List of systematic shifts to process")

parser.add_option("--taues_study", dest="taues_study", action='store_true', default=False,
                  help="Run lots of tau ES shifts jsut for DY")

parser.add_option("--qcd_study", dest="qcd_study", action='store_true', default=False,
                  help="Run QCD MC sample for QCD study")

parser.add_option("--analysis", dest="analysis", type='string', default='sm',
                  help="Specify whether trees are produced for mssm or sm analysis")

parser.add_option("--parajobs", dest="parajobs", action='store_true', default=False,
                  help="Submit jobs parametrically")
parser.add_option("--config", dest="config", type='string', default='',
                  help="Config file")
parser.add_option("--mg_signal", dest="mg_signal", action='store_true', default=False,
                  help="Process the MG signal samples")
parser.add_option("--cp_decay", dest="cp_decay", action='store_true', default=False,
                  help="Process the sample for CP in decay measurments")

(options, args) = parser.parse_args()
if options.wrapper: JOBWRAPPER=options.wrapper
if options.submit:  JOBSUBMIT=options.submit

def getParaJobSubmit(N):
  if not options.submit: return 'true'
  sub_opts=JOBSUBMIT.split(' ',1)[1]
  if '\"' in sub_opts: sub_opts = sub_opts.replace('\"','')
  sub_opts = '%s \"%s -t 1-%i:1\"' %(JOBSUBMIT.split(' ',1)[0], sub_opts, N)
  print sub_opts
  return sub_opts

BACKUPNAME = options.slbackupname

#channels = options.channels
scales = options.scales
taues_study = options.taues_study
no_json = options.no_json
analysis = options.analysis
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
#  elif scale == "scale_t":
#    flatjsonlistdysig.append("^%(scale)s_hi^%(scale)s_lo"%vars()) 
  else:
    n_scales+=1
    flatjsonlist.append("^%(scale)s_hi^%(scale)s_lo"%vars()) 
    flatjsonlistdysig.append("^%(scale)s_hi^%(scale)s_lo"%vars()) 
 
if analysis == 'sm':
  CONFIG='scripts/configsm2016cpdecay.json'
else:
  CONFIG='scripts/config2016.json'
if options.config != '': CONFIG = options.config
 
n_channels=1
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
  
scale = int(math.ceil(float(n_scales*n_channels)/50))
if scale < 1: scale = 1

total = float(len(flatjsonlistdysig))
flatjsons = []
# this makes sure the JES's are submitted as seperate jobs (solves memory issues)
#for i in flatjsonlistdysig:
#  if 'scale_j' in i and 'hf' not in i and 'cent' not in i and 'full' not in i and 'relbal' not in i:
#    flatjsons.append('job:sequences:all:'+i)
#    flatjsonlistdysig.remove(i)
#    scale = int(math.ceil(float((n_scales-2)*n_channels)/50))
#    if scale < 1: scale = 1
# split into seperate jobs if number of scales is over a value
for i in range(0,scale):
   first = i*int(math.ceil(total/scale))
   last = (i+1)*int(math.ceil(total/scale))
   temp=''.join(flatjsonlistdysig[first:last]) 
   if temp == '': continue
   temp='job:sequences:all:'+temp
   flatjsons.append(temp)
 
FILELIST='filelists/Jan31_MC_80X'

signal_mc = [ ]
signal_vh = [ ] 
signal_mc_ww = [ ]

if os.path.isfile("./jobs/files_per_sample.txt"):
  os.system("mv ./jobs/files_per_sample.txt ./jobs/files_per_sample-%(BACKUPNAME)s.txt"%vars())

file_persamp = open("./jobs/files_per_sample.txt", "w")

if options.mg_signal:
  signal_mc += [
  'GluGluToPseudoscalarHToTauTau_M125_amcatnloFXFX',
  'GluGluToMaxmixHToTauTau_M125_amcatnloFXFX',
  'GluGluToHToTauTau_M125_amcatnloFXFX',
  'GluGluToPseudoscalarHToTauTauPlusTwoJets_M125_amcatnloFXFX',
  'GluGluToMaxmixHToTauTauPlusTwoJets_M125_amcatnloFXFX',
  'GluGluToHToTauTauPlusTwoJets_M125_amcatnloFXFX',

  ]

if options.cp_decay:
  signal_mc += [
   'GluGluToHToTauTau_M-125-nospinner',
   'VBFHToTauTau_M-125-nospinner',
   'GluGluToHToTauTau_M-125-nospinner-filter',
   'VBFHToTauTau_M-125-nospinner-filter',
   'VBFHToTauTau_M-125-nospinner-filter-ext' 
  ]

if options.proc_sm or options.proc_all or options.proc_smbkg:
  if options.analysis == 'sm': masses = ['125']
  else: masses = ['125']
  if options.short_signal or options.proc_smbkg: masses = ['125']
  for mass in masses :
    signal_mc += [
      'GluGluToHToTauTau_M-'+mass,
      'VBFHToTauTau_M-'+mass,
      'ZHToTauTau_M-'+mass,
      'WplusHToTauTau_M-'+mass,
      'WminusHToTauTau_M-'+mass#,
    ]
  if options.proc_sm:  
    signal_mc += [
        'GluGluHToWWTo2L2Nu_M-125',
        'VBFHToWWTo2L2Nu_M-125'
        ]
  # add cp samples
  if options.cp_signal:
    signal_mc += [
        'GluGluH2JetsToTauTau_M125_CPmixing_pseudoscalar',
        'GluGluH2JetsToTauTau_M125_CPmixing_maxmix',
        'GluGluH2JetsToTauTau_M125_CPmixing_sm',
        'VBFHiggs0M_M-125',
        'VBFHiggs0Mf05ph0_M-125',
        'VBFHiggs0PM_M-125',
        'ZHiggs0M_M-125',
        'ZHiggs0Mf05ph0_M-125',
        'ZHiggs0PM_M-125',
        'WHiggs0M_M-125',
        'WHiggs0Mf05ph0_M-125',
        'WHiggs0PM_M-125'
        #'GluGluToHToTauTau_amcNLO_M-125',
        #'VBFHToTauTau_amcNLO_M-125'
    ]  
    
if options.proc_mssm or options.proc_all:
  gghmasses = ['80','90','100','110','120','130', '140', '160','180','200','250','350','400','450','500', '600','700','800','900','1000','1200','1400','1600','1800','2000','2300','2600','2900','3200'] # 
  bbhmasses = ['80','90','100','110','120','130','140','160','180','200','250','350','400','450','500','600','700','800','900','1000','1200','1400','1600','1800','2000','2300','2600','2900','3200']
  if options.short_signal: 
    gghmasses = ['500']
    bbhmasses = ['500']
  for mass in gghmasses :
    signal_mc += [
      'SUSYGluGluToHToTauTau_M-'+mass,
    ]
  for mass in bbhmasses :
    signal_mc += [
      'SUSYGluGluToBBHToTauTau_M-'+mass
    ]
nlo_signal_mc = [ ]    
if options.proc_mssm_nlo:
  bbhmasses = ['100','1000','110','120','1200','130','140','1400','160','1600','180','1800','200','2000','2300','250','2600','2900','3200','350','400','450','500','600','700','80','800','90','900']
  for mass in bbhmasses :
    nlo_signal_mc += [
      'SUSYGluGluToBBHToTauTau_M-'+mass+'-NLO'
    ]
nlo_qsh_signal_mc = [ ]
if options.proc_mssm_nlo_qsh:
  bbhmasses = ['80','130','200','350','700','1200','1800','3200']
  for mass in bbhmasses :
    nlo_qsh_signal_mc += [
      'SUSYGluGluToBBHToTauTau_M-'+mass+'-NLO-QshUp', 'SUSYGluGluToBBHToTauTau_M-'+mass+'-NLO-QshDown'
    ]

#if options.proc_Hhh:
#  masses = ['250','260','270','280','300','320','340','350','400','450','500','550','600','650','700','800','900']
#  for mass in masses :
#    signal_mc += [
#       'GluGluToRadionToHHTo2B2Tau_M-'+mass
#    ]

if options.proc_data or options.proc_all or options.calc_lumi or options.proc_embed:
  if not no_json:
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
  else:
    channels=['mt','et','tt','em','zmm','zee']

if options.proc_data or options.proc_all or options.calc_lumi:  

  data_samples = []
  data_eras = ['B','C','D','E','F','G','H']
  for chn in channels:
    for era in data_eras: 
         if 'mt' in chn or 'zmm' in chn:
           if not era == 'H':  
               data_samples+=[   
                'SingleMuon'+era]
           else:
               data_samples+=[   
                'SingleMuon'+era+'v2']
               data_samples+=[   
                'SingleMuon'+era+'v3']
         if 'et' in chn or 'zee' in chn:
           if not era == 'H':  
               data_samples+=[   
                'SingleElectron'+era]
           else:
               data_samples+=[   
                'SingleElectron'+era+'v2']
               data_samples+=[   
                'SingleElectron'+era+'v3']
         if 'em' in chn:
           if not era == 'H':  
               data_samples+=[   
                'MuonEG'+era]
           else:
               data_samples+=[   
                'MuonEG'+era+'v2']
               data_samples+=[   
                'MuonEG'+era+'v3']
         if 'tt' in chn:
           if not era == 'H':  
               data_samples+=[   
                'Tau'+era]
           else:
               data_samples+=[   
                'Tau'+era+'v2']
               data_samples+=[   
                'Tau'+era+'v3'] 
        

  DATAFILELIST="./filelists/Jan31_Data_80X"

  if options.calc_lumi:
    for sa in data_samples:
        JOB='%s_2016' % (sa)
        JSONPATCH= (r"'{\"job\":{\"filelist\":\"%(DATAFILELIST)s_%(sa)s.dat\",\"file_prefix\":\"root://gfe02.grid.hep.ph.ic.ac.uk:1097//store/user/dwinterb/Jan31_Data_80X/\",\"sequences\":{\"em\":[],\"et\":[],\"mt\":[],\"tt\":[]}}, \"sequence\":{\"output_name\":\"%(JOB)s\",\"is_data\":true,\"lumi_mask_only\":true}}' "%vars());
        nfiles = sum(1 for line in open('%(DATAFILELIST)s_%(sa)s.dat' % vars()))
        nperjob = 500 
        for i in range (0,int(math.ceil(float(nfiles)/float(nperjob)))) :
          os.system('%(JOBWRAPPER)s "./bin/HTT --cfg=%(CONFIG)s --json=%(JSONPATCH)s --offset=%(i)d --nlines=%(nperjob)d &> jobs/%(JOB)s-%(i)d.log" jobs/%(JOB)s-%(i)s.sh' %vars())
          if not parajobs: os.system('%(JOBSUBMIT)s jobs/%(JOB)s-%(i)d.sh' % vars())
        if parajobs: 
          os.system('%(JOBWRAPPER)s ./jobs/%(JOB)s-\$\(\(SGE_TASK_ID-1\)\).sh  jobs/parajob_%(JOB)s.sh' %vars())
          PARAJOBSUBMIT = getParaJobSubmit(int(math.ceil(float(nfiles)/float(nperjob))))
          os.system('%(PARAJOBSUBMIT)s jobs/parajob_%(JOB)s.sh' % vars())  
        file_persamp.write("%s %d\n" %(JOB, int(math.ceil(float(nfiles)/float(nperjob)))))

  else:
    for sa in data_samples:
        JOB='%s_2016' % (sa)
        JSONPATCH= (r"'{\"job\":{\"filelist\":\"%(DATAFILELIST)s_%(sa)s.dat\",\"file_prefix\":\"root://gfe02.grid.hep.ph.ic.ac.uk:1097//store/user/dwinterb/Jan31_Data_80X/\",\"sequences\":{\"em\":[],\"et\":[],\"mt\":[],\"tt\":[],\"zmm\":[],\"zee\":[]}}, \"sequence\":{\"output_name\":\"%(JOB)s\",\"is_data\":true}}' "%vars());
        nfiles = sum(1 for line in open('%(DATAFILELIST)s_%(sa)s.dat' % vars()))
        nperjob = 40
        
        for i in range (0,int(math.ceil(float(nfiles)/float(nperjob)))) :  
          os.system('%(JOBWRAPPER)s "./bin/HTT --cfg=%(CONFIG)s --json=%(JSONPATCH)s --offset=%(i)d --nlines=%(nperjob)d &> jobs/%(JOB)s-%(i)d.log" jobs/%(JOB)s-%(i)s.sh' %vars())
          if not parajobs: os.system('%(JOBSUBMIT)s jobs/%(JOB)s-%(i)d.sh' % vars())
        if parajobs: 
          os.system('%(JOBWRAPPER)s ./jobs/%(JOB)s-\$\(\(SGE_TASK_ID-1\)\).sh  jobs/parajob_%(JOB)s.sh' %vars())
          PARAJOBSUBMIT = getParaJobSubmit(int(math.ceil(float(nfiles)/float(nperjob))))
          os.system('%(PARAJOBSUBMIT)s jobs/parajob_%(JOB)s.sh' % vars())  
        file_persamp.write("%s %d\n" %(JOB, int(math.ceil(float(nfiles)/float(nperjob)))))
        
if options.proc_embed or options.proc_all:        
        
  embed_samples = []
  data_eras = ['B','C','D','E','F','G','H']
  for chn in channels:
    for era in data_eras:
      if 'em' in chn:
        embed_samples+=['EmbeddingElMu'+era]
      if 'et' in chn:
        embed_samples+=['EmbeddingElTau'+era]
      if 'mt' in chn:
        embed_samples+=['EmbeddingMuTau'+era]
      if 'tt' in chn:
        embed_samples+=['EmbeddingTauTau'+era]
      if 'zmm' in chn:
        embed_samples+=['EmbeddingMuMu'+era]
      if 'zee' in chn:
        embed_samples+=['EmbeddingElEl'+era]


        
  EMBEDFILELISTZMM="./filelists/Jan31_MC_80X"

  EMBEDFILELIST="./filelists/Jan31_MC_80X"
  
  for sa in embed_samples:
    job_num=0  
    JOB='%s_2016' % (sa)
    JSONPATCH= (r"'{\"job\":{\"filelist\":\"%(EMBEDFILELIST)s_%(sa)s.dat\",\"file_prefix\":\"root://gfe02.grid.hep.ph.ic.ac.uk:1097//store/user/dwinterb/Jan31_MC_80X/\",\"sequences\":{\"em\":[],\"et\":[],\"mt\":[],\"tt\":[],\"zmm\":[],\"zee\":[]}}, \"sequence\":{\"output_name\":\"%(JOB)s\",\"is_embedded\":true}}' "%vars());
    if 'EmbeddingMuMu' in sa:
      JSONPATCH= (r"'{\"job\":{\"filelist\":\"%(EMBEDFILELISTZMM)s_%(sa)s.dat\",\"file_prefix\":\"root://gfe02.grid.hep.ph.ic.ac.uk:1097//store/user/dwinterb/Jan31_MC_80X/\",\"sequences\":{\"em\":[],\"et\":[],\"mt\":[],\"tt\":[],\"zmm\":[],\"zee\":[]}}, \"sequence\":{\"output_name\":\"%(JOB)s\",\"is_embedded\":true}}' "%vars());
    for FLATJSONPATCH in flatjsons: 
      nperjob = 4
      FLATJSONPATCH = FLATJSONPATCH.replace('^scale_j_hi^scale_j_lo','').replace('^scale_j_hf_hi^scale_j_hf_lo','').replace('^scale_j_cent_hi^scale_j_cent_lo','').replace('^scale_j_full_hi^scale_j_full_lo','').replace('^scale_j_relbal_hi^scale_j_relbal_lo','')

      FLATJSONPATCH = FLATJSONPATCH.replace('^scale_efake_0pi_hi^scale_efake_0pi_lo','').replace('^scale_efake_1pi_hi^scale_efake_1pi_lo','').replace('^scale_mufake_0pi_hi^scale_mufake_0pi_lo','').replace('^scale_mufake_1pi_hi^scale_mufake_1pi_lo','').replace('^met_cl_hi^met_cl_lo','').replace('^met_uncl_hi^met_uncl_lo','').replace('^scale_met_hi^scale_met_lo','').replace('^res_met_hi^res_met_lo','').replace('^scale_met_njets0_hi^scale_met_njets0_lo','').replace('^res_met_njets0_hi^res_met_njets0_lo','').replace('^scale_met_njets1_hi^scale_met_njets1_lo','').replace('^res_met_njets1_hi^res_met_njets1_lo','').replace('^scale_met_njets2_hi^scale_met_njets2_lo','').replace('^res_met_njets2_hi^res_met_njets2_lo','')  
      if 'TauTau' in  sa: FLATJSONPATCH = FLATJSONPATCH.replace('^scale_e_hi^scale_e_lo','').replace('^scale_mu_hi^scale_mu_lo','').replace('^scale_t_hi^scale_t_lo','')
      if 'ElMu' in  sa: FLATJSONPATCH = FLATJSONPATCH.replace('^scale_e_hi^scale_e_lo','').replace('^scale_t_0pi_hi^scale_t_0pi_lo','').replace('^scale_t_1pi_hi^scale_t_1pi_lo','').replace('^scale_t_3prong_hi^scale_t_3prong_lo','')
      if 'MuTau' in  sa: FLATJSONPATCH = FLATJSONPATCH.replace('^scale_e_hi^scale_e_lo','').replace('^scale_t_hi^scale_t_lo','')
      if 'ElTau' in  sa: FLATJSONPATCH = FLATJSONPATCH.replace('^scale_mu_hi^scale_mu_lo','').replace('^scale_t_hi^scale_t_lo','')
      nperjob = int(math.ceil(float(nperjob)/max(1.,float(n_scales-8)*float(n_channels)/10.)))
      if 'EmbeddingMuMu' in sa: nfiles = sum(1 for line in open('%(EMBEDFILELISTZMM)s_%(sa)s.dat' % vars()))
      else: nfiles = sum(1 for line in open('%(EMBEDFILELIST)s_%(sa)s.dat' % vars()))
      for i in range (0,int(math.ceil(float(nfiles)/float(nperjob)))) :
        os.system('%(JOBWRAPPER)s "./bin/HTT --cfg=%(CONFIG)s --json=%(JSONPATCH)s --flatjson=%(FLATJSONPATCH)s --offset=%(i)d --nlines=%(nperjob)d &> jobs/%(JOB)s-%(job_num)d.log" jobs/%(JOB)s-%(job_num)s.sh' %vars())
        if not parajobs: os.system('%(JOBSUBMIT)s jobs/%(JOB)s-%(job_num)d.sh' % vars())
        job_num+=1
      file_persamp.write("%s %d\n" %(JOB, int(math.ceil(float(nfiles)/float(nperjob)))))
      if parajobs: 
        os.system('%(JOBWRAPPER)s ./jobs/%(JOB)s-\$\(\(SGE_TASK_ID-1\)\).sh  jobs/parajob_%(JOB)s.sh' %vars())
        PARAJOBSUBMIT = getParaJobSubmit(job_num)
        os.system('%(PARAJOBSUBMIT)s jobs/parajob_%(JOB)s.sh' % vars()) 



#if options.proc_qcd:
#  qcd_samples = [
#    'QCD_Ht100to200',
#    'QCD_Ht200to300',
#    'QCD_Ht300to500',
#    'QCD_Ht500to700',
#    'QCD_Ht700to1000',
#    'QCD_Ht1000to1500',
#    'QCD_Ht1500to2000',
#    'QCD_Ht2000toInf'
#  ]

#  for sa in qcd_samples:
#      JOB='%s_2015' % (sa)
#      JSONPATCH= (r"'{\"job\":{\"filelist\":\"%(FILELIST)s_%(sa)s.dat\",\"sequences\":{\"em\":[],\"et\":[],\"mt\":[],\"tt\":[]}}, \"sequence\":{\"output_name\":\"%(JOB)s\"}}' "%vars());
#      nfiles = sum(1 for line in open('%(FILELIST)s_%(sa)s.dat' % vars()))
#      nperjob = 40 
#      for i in range (0,int(math.ceil(float(nfiles)/float(nperjob)))) :
#        os.system('%(JOBWRAPPER)s "./bin/HTT --cfg=%(CONFIG)s --json=%(JSONPATCH)s --offset=%(i)d --nlines=%(nperjob)d &> jobs/%(JOB)s-%(i)d.log" jobs/%(JOB)s-%(i)s.sh' %vars())
#        os.system('%(JOBSUBMIT)s jobs/%(JOB)s-%(i)d.sh' % vars())
#      file_persamp.write("%s %d\n" %(JOB, int(math.ceil(float(nfiles)/float(nperjob)))))
#

if options.proc_bkg or options.proc_all or options.qcd_study:
  central_samples = [
     'DYJetsToLL',
     'TT',
     'VVTo2L2Nu',
     'VVTo2L2Nu-ext1',
     'ZZTo2L2Q',
     'ZZTo4L-amcat',
     'WWTo1L1Nu2Q',
     'WZJToLLLNu',
     'WZTo1L3Nu',
     'WZTo2L2Q',
     'WZTo1L1Nu2Q',
     'T-t',
     'Tbar-t',
     'T-tW',
     'Tbar-tW',
     'DYJetsToLL-LO-ext1',
     'DYJetsToLL-LO-ext2',
     'DYJetsToLL_M-10-50-LO',
     'DY1JetsToLL-LO',
     'DY2JetsToLL-LO',
     'DY3JetsToLL-LO',
     'DY4JetsToLL-LO',
     'WJetsToLNu-LO',
     'WJetsToLNu-LO-ext',
     'W1JetsToLNu-LO',
     'W2JetsToLNu-LO',
     'W2JetsToLNu-LO-ext',
     'W3JetsToLNu-LO',
     'W3JetsToLNu-LO-ext',
     'W4JetsToLNu-LO',
     'W4JetsToLNu-LO-ext1',
     'W4JetsToLNu-LO-ext2',
     'WGToLNuG',
     'WGToLNuG-ext',
     'WGstarToLNuEE',
     'WGstarToLNuMuMu'
     ]
  
  if options.analysis == 'sm':
    extra_samples = [
      'EWKWMinus2Jets_WToLNu-ext1',
      'EWKWMinus2Jets_WToLNu-ext2',
      'EWKWMinus2Jets_WToLNu',
      'EWKWPlus2Jets_WToLNu-ext1',
      'EWKWPlus2Jets_WToLNu-ext2',
      'EWKWPlus2Jets_WToLNu',
      'EWKZ2Jets_ZToLL-ext',
      'EWKZ2Jets_ZToLL'
    ]
    central_samples.extend(extra_samples)

  if options.qcd_study:
    FILELIST='filelists/Jan31_MC_80X'
    central_samples = [
      #'QCDEM_Pt120to170',
      #'QCDEM_Pt170to300',
      #'QCDEM_Pt20to30',
      #'QCDEM_Pt300toInf',
      #'QCDEM_Pt30to50',
      #'QCDEM_Pt50to80',
      #'QCDEM_Pt80to120',
      #'QCDMu5_Pt1000toInf',
      #'QCDMu5_Pt120to170',
      #'QCDMu5_Pt15to20',
      #'QCDMu5_Pt170to130',
      #'QCDMu5_Pt20to30',
      #'QCDMu5_Pt300to470',
      #'QCDMu5_Pt30to50',
      #'QCDMu5_Pt470to600',
      #'QCDMu5_Pt50to80',
      #'QCDMu5_Pt600to800',
      #'QCDMu5_Pt800to1000',
      #'QCDMu5_Pt80to120',
      'QCDMuEnrichedPt15'
       ]
  for sa in central_samples:
      JOB='%s_2016' % (sa)
      JSONPATCH= (r"'{\"job\":{\"filelist\":\"%(FILELIST)s_%(sa)s.dat\"}, \"sequence\":{\"output_name\":\"%(JOB)s\"}}' "%vars());
      job_num=0
      for FLATJSONPATCH in flatjsons: 
        nperjob = 5 #20
        if 'scale' in FLATJSONPATCH:
          nperjob = 5
        if 'DY' in sa and 'JetsToLL' in sa:
          nperjob = 5
        if 'TT' in sa:
          nperjob = 5
          if 'scale' in FLATJSONPATCH:
            nperjob = 2
#        if 'WJetsToLNu' in sa or 'W1JetsToLNu' in sa or 'W2JetsToLNu' in sa or 'W3JetsToLNu' in sa or 'W4JetsToLNu' in sa:
#          nperjob = 30
        if 'QCD' in sa:
            nperjob = 15
        if 'scale' in FLATJSONPATCH:
          nperjob = 2
        if 'DY' in sa and 'JetsToLL' in sa and taues_study:
          FLATJSONPATCH=""
          CONFIG='scripts/taues_config.json'
          JSONPATCH= (r"'{\"job\":{\"filelist\":\"%(FILELIST)s_%(sa)s.dat\",\"sequences\":{\"em\":[\"scale_e_lo\",\"scale_e_hi\",\"scale_j_lo\",\"scale_j_hi\"],\"et\":[\"scale_t_lo_3\",\"scale_t_hi_3\",\"scale_t_lo_2\",\"scale_t_hi_2\",\"scale_t_lo_1\",\"scale_t_hi_1\",\"scale_t_lo_2.5\",\"scale_t_hi_2.5\",\"scale_t_lo_1.5\",\"scale_t_hi_1.5\",\"scale_t_lo_0.5\",\"scale_t_hi_0.5\",\"scale_j_lo\",\"scale_j_hi\"],\"mt\":[\"scale_t_lo_3\",\"scale_t_hi_3\",\"scale_t_lo_2\",\"scale_t_hi_2\",\"scale_t_lo_1\",\"scale_t_hi_1\",\"scale_t_lo_2.5\",\"scale_t_hi_2.5\",\"scale_t_lo_1.5\",\"scale_t_hi_1.5\",\"scale_t_lo_0.5\",\"scale_t_hi_0.5\",\"scale_j_lo\",\"scale_j_hi\"],\"tt\":[\"scale_t_lo_3\",\"scale_t_hi_3\",\"scale_t_lo_2\",\"scale_t_hi_2\",\"scale_t_lo_1\",\"scale_t_hi_1\",\"scale_t_lo_2.5\",\"scale_t_hi_2.5\",\"scale_t_lo_1.5\",\"scale_t_hi_1.5\",\"scale_t_lo_0.5\",\"scale_t_hi_0.5\",\"scale_j_hi\",\"scale_j_lo\"]}}, \"sequence\":{\"output_name\":\"%(JOB)s\"}}' "%vars());
          nperjob = 10
        #FLATJSONPATCH = FLATJSONPATCH.replace('^scale_e_hi^scale_e_lo','').replace('^scale_mu_hi^scale_mu_lo','')
        FLATJSONPATCH = FLATJSONPATCH.replace('^scale_mu_hi^scale_mu_lo','')
        if 'DY' not in sa and 'EWKZ' not in sa:
          FLATJSONPATCH = FLATJSONPATCH.replace('^scale_efake_0pi_hi^scale_efake_0pi_lo','').replace('^scale_efake_1pi_hi^scale_efake_1pi_lo','').replace('^scale_mufake_0pi_hi^scale_mufake_0pi_lo','').replace('^scale_mufake_1pi_hi^scale_mufake_1pi_lo','')
        if 'DY' not in sa and 'JetsToLNu' not in sa and 'WG' not in sa and 'EWKZ' not in sa and 'EWKW' not in sa:
          FLATJSONPATCH = FLATJSONPATCH.replace('^scale_met_hi^scale_met_lo','').replace('^res_met_hi^res_met_lo','').replace('^scale_met_njets0_hi^scale_met_njets0_lo','').replace('^res_met_njets0_hi^res_met_njets0_lo','').replace('^scale_met_njets1_hi^scale_met_njets1_lo','').replace('^res_met_njets1_hi^res_met_njets1_lo','').replace('^scale_met_njets2_hi^scale_met_njets2_lo','').replace('^res_met_njets2_hi^res_met_njets2_lo','')
        n_scales = FLATJSONPATCH.count('_lo') + FLATJSONPATCH.count('default')
        nperjob = int(math.ceil(float(nperjob)/max(1.,float(n_scales)*float(n_channels)/10.)))
        nfiles = sum(1 for line in open('%(FILELIST)s_%(sa)s.dat' % vars()))
        for i in range (0,int(math.ceil(float(nfiles)/float(nperjob)))) :
          os.system('%(JOBWRAPPER)s "./bin/HTT --cfg=%(CONFIG)s --json=%(JSONPATCH)s --flatjson=%(FLATJSONPATCH)s --offset=%(i)d --nlines=%(nperjob)d &> jobs/%(JOB)s-%(job_num)d.log" jobs/%(JOB)s-%(job_num)s.sh' %vars())
          if not parajobs: os.system('%(JOBSUBMIT)s jobs/%(JOB)s-%(job_num)d.sh' % vars())
          job_num+=1
        file_persamp.write("%s %d\n" %(JOB, int(math.ceil(float(nfiles)/float(nperjob)))))
      if parajobs: 
        os.system('%(JOBWRAPPER)s ./jobs/%(JOB)s-\$\(\(SGE_TASK_ID-1\)\).sh  jobs/parajob_%(JOB)s.sh' %vars())
        PARAJOBSUBMIT = getParaJobSubmit(job_num)
        os.system('%(PARAJOBSUBMIT)s jobs/parajob_%(JOB)s.sh' % vars()) 
#if float(n_scales*n_channels)/100 > 1: nperjob = int(math.ceil(nperjob/(float(n_scales*n_channels)/100)))  

if options.proc_sm or options.proc_smbkg or options.proc_mssm or options.proc_Hhh or options.proc_all or options.cp_decay:
  if options.analysis == 'sm': SIG_FILELIST='filelists/Jan31_MC_80X'
  else: SIG_FILELIST = FILELIST
  for sa in signal_mc:
    SIG_FILELIST='filelists/Jan31_MC_80X'
    print sa
    if 'nospinner' in sa: SIG_FILELIST='filelists/Jan31_MC_80X'
    if 'Higgs0' in sa or 'GluGluH2JetsToTauTau' in sa: SIG_FILELIST='filelists/Apr02_MC_80X'
    JOB='%s_2016' % (sa)
    SIG_DIR = SIG_FILELIST.split('/')[1]
    JSONPATCH= (r"'{\"job\":{\"filelist\":\"%(SIG_FILELIST)s_%(sa)s.dat\",\"file_prefix\":\"root://gfe02.grid.hep.ph.ic.ac.uk:1097//store/user/dwinterb/%(SIG_DIR)s/\"}, \"sequence\":{\"output_name\":\"%(JOB)s\"}}' "%vars());
    job_num=0
    for FLATJSONPATCH in flatjsons:
      FLATJSONPATCH = FLATJSONPATCH.replace('^scale_efake_0pi_hi^scale_efake_0pi_lo','').replace('^scale_efake_1pi_hi^scale_efake_1pi_lo','').replace('^scale_mufake_0pi_hi^scale_mufake_0pi_lo','').replace('^scale_mufake_1pi_hi^scale_mufake_1pi_lo','')
      #FLATJSONPATCH = FLATJSONPATCH.replace('^scale_e_hi^scale_e_lo','').replace('^scale_mu_hi^scale_mu_lo','')
      FLATJSONPATCH = FLATJSONPATCH.replace('^scale_mu_hi^scale_mu_lo','')
      if os.path.exists('%(SIG_FILELIST)s_%(sa)s.dat' %vars()):
        nfiles = sum(1 for line in open('%(SIG_FILELIST)s_%(sa)s.dat' % vars()))
        nperjob = 5
        if 'filter' in sa: nperjob = 2
        for i in range (0,int(math.ceil(float(nfiles)/float(nperjob)))) :
          os.system('%(JOBWRAPPER)s "./bin/HTT --cfg=%(CONFIG)s --json=%(JSONPATCH)s --flatjson=%(FLATJSONPATCH)s --offset=%(i)d --nlines=%(nperjob)d &> jobs/%(JOB)s-%(job_num)d.log" jobs/%(JOB)s-%(job_num)s.sh' %vars())
          if not parajobs: os.system('%(JOBSUBMIT)s jobs/%(JOB)s-%(job_num)d.sh' % vars())
          job_num+=1 
        file_persamp.write("%s %d\n" %(JOB, int(math.ceil(float(nfiles)/float(nperjob)))))
    if parajobs: 
      os.system('%(JOBWRAPPER)s ./jobs/%(JOB)s-\$\(\(SGE_TASK_ID-1\)\).sh  jobs/parajob_%(JOB)s.sh' %vars())
      PARAJOBSUBMIT = getParaJobSubmit(job_num)
      os.system('%(PARAJOBSUBMIT)s jobs/parajob_%(JOB)s.sh' % vars()) 
      

if options.mg_signal:
  for sa in signal_mc:
    if 'amcatnloFXFX' in sa: SIG_FILELIST='filelists/Jan31_MC_80X'
    else: SIG_FILELIST='filelists/Jan31_MC_80X'
    JOB='%s_2016' % (sa)
    SIG_DIR = SIG_FILELIST.split('/')[1]
    JSONPATCH= (r"'{\"job\":{\"filelist\":\"%(SIG_FILELIST)s_%(sa)s.dat\",\"file_prefix\":\"root://gfe02.grid.hep.ph.ic.ac.uk:1097//store/user/dwinterb/%(SIG_DIR)s/\"}, \"sequence\":{\"output_name\":\"%(JOB)s\"}}' "%vars());
    job_num=0
    for FLATJSONPATCH in flatjsons:
      FLATJSONPATCH = FLATJSONPATCH.replace('^scale_efake_0pi_hi^scale_efake_0pi_lo','').replace('^scale_efake_1pi_hi^scale_efake_1pi_lo','').replace('^scale_mufake_0pi_hi^scale_mufake_0pi_lo','').replace('^scale_mufake_1pi_hi^scale_mufake_1pi_lo','')
      #FLATJSONPATCH = FLATJSONPATCH.replace('^scale_e_hi^scale_e_lo','').replace('^scale_mu_hi^scale_mu_lo','')
      FLATJSONPATCH = FLATJSONPATCH.replace('^scale_mu_hi^scale_mu_lo','')

      if os.path.exists('%(SIG_FILELIST)s_%(sa)s.dat' %vars()):
        nfiles = sum(1 for line in open('%(SIG_FILELIST)s_%(sa)s.dat' % vars()))
        nperjob = 1
        for i in range (0,int(math.ceil(float(nfiles)/float(nperjob)))) :
          os.system('%(JOBWRAPPER)s "./bin/HTT --cfg=%(CONFIG)s --json=%(JSONPATCH)s --flatjson=%(FLATJSONPATCH)s --offset=%(i)d --nlines=%(nperjob)d &> jobs/%(JOB)s-%(job_num)d.log" jobs/%(JOB)s-%(job_num)s.sh' %vars())
          if not parajobs: os.system('%(JOBSUBMIT)s jobs/%(JOB)s-%(job_num)d.sh' % vars())
          job_num+=1
        file_persamp.write("%s %d\n" %(JOB, int(math.ceil(float(nfiles)/float(nperjob)))))
    if parajobs:
      os.system('%(JOBWRAPPER)s ./jobs/%(JOB)s-\$\(\(SGE_TASK_ID-1\)\).sh  jobs/parajob_%(JOB)s.sh' %vars())
      PARAJOBSUBMIT = getParaJobSubmit(job_num)
      os.system('%(PARAJOBSUBMIT)s jobs/parajob_%(JOB)s.sh' % vars())



