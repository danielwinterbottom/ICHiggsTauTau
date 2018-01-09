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
                  help="Process data samples (including embedded)")

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

parser.add_option("--analysis", dest="analysis", type='string', default='mssm',
                  help="Specify whether trees are produced for mssm or sm analysis")


(options, args) = parser.parse_args()
if options.wrapper: JOBWRAPPER=options.wrapper
if options.submit:  JOBSUBMIT=options.submit

BACKUPNAME = options.slbackupname

#channels = options.channels
scales = options.scales
taues_study = options.taues_study
no_json = options.no_json
analysis = options.analysis


scale_list = scales.split(',')
flatjsonlist = []
flatjsonlistdysig = []
flatjsonlist.append("job:sequences:all:")
flatjsonlistdysig.append("job:sequences:all:")
for scale in scale_list: 
  if scale == "default":
    flatjsonlist.append("^%(scale)s"%vars())
    flatjsonlistdysig.append("^%(scale)s"%vars())
#  elif scale == "scale_t":
#    flatjsonlistdysig.append("^%(scale)s_hi^%(scale)s_lo"%vars()) 
  else:
    flatjsonlist.append("^%(scale)s_hi^%(scale)s_lo"%vars()) 
    flatjsonlistdysig.append("^%(scale)s_hi^%(scale)s_lo"%vars()) 

FLATJSONPATCHOTH = ''.join(flatjsonlist)
FLATJSONPATCHDYSIG = ''.join(flatjsonlistdysig)
 
if analysis == 'sm':
  CONFIG='scripts/configsm2016.json'
else:
  CONFIG='scripts/config2016.json'
  
FILELIST='filelists/Apr05_MC_80X'

signal_mc = [ ]
signal_vh = [ ] 
signal_mc_ww = [ ]

if os.path.isfile("./jobs/files_per_sample.txt"):
  os.system("mv ./jobs/files_per_sample.txt ./jobs/files_per_sample-%(BACKUPNAME)s.txt"%vars())

file_persamp = open("./jobs/files_per_sample.txt", "w")

if options.proc_sm or options.proc_all or options.proc_smbkg:
  masses = ['125']
  if options.short_signal or options.proc_smbkg: masses = ['125']
  for mass in masses :
    signal_mc += [
      'GluGluToHToTauTau_M-'+mass,
      'VBFHToTauTau_M-'+mass,
      'ZHToTauTau_M-'+mass,
      'WplusHToTauTau_M-'+mass,
      'WminusHToTauTau_M-'+mass,
      'TTHToTauTau_M-'+mass
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

if options.proc_data or options.proc_all or options.calc_lumi:
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
        


  DATAFILELIST="./filelists/Jun16_Data_80X"

  if options.calc_lumi:
    for sa in data_samples:
        JOB='%s_2016' % (sa)
        JSONPATCH= (r"'{\"job\":{\"filelist\":\"%(DATAFILELIST)s_%(sa)s.dat\",\"file_prefix\":\"root://gfe02.grid.hep.ph.ic.ac.uk:1097//store/user/dwinterb/Jun16_Data_80X/\",\"sequences\":{\"em\":[],\"et\":[],\"mt\":[],\"tt\":[]}}, \"sequence\":{\"output_name\":\"%(JOB)s\",\"is_data\":true,\"lumi_mask_only\":true}}' "%vars());
        nfiles = sum(1 for line in open('%(DATAFILELIST)s_%(sa)s.dat' % vars()))
        nperjob = 500 
        for i in range (0,int(math.ceil(float(nfiles)/float(nperjob)))) :
          os.system('%(JOBWRAPPER)s "./bin/HTT --cfg=%(CONFIG)s --json=%(JSONPATCH)s --offset=%(i)d --nlines=%(nperjob)d &> jobs/%(JOB)s-%(i)d.log" jobs/%(JOB)s-%(i)s.sh' %vars())
          os.system('%(JOBSUBMIT)s jobs/%(JOB)s-%(i)d.sh' % vars())
        file_persamp.write("%s %d\n" %(JOB, int(math.ceil(float(nfiles)/float(nperjob)))))

  else:
    for sa in data_samples:
        JOB='%s_2016' % (sa)
        JSONPATCH= (r"'{\"job\":{\"filelist\":\"%(DATAFILELIST)s_%(sa)s.dat\",\"file_prefix\":\"root://gfe02.grid.hep.ph.ic.ac.uk:1097//store/user/dwinterb/Jun16_Data_80X/\",\"sequences\":{\"em\":[],\"et\":[],\"mt\":[],\"tt\":[],\"zmm\":[],\"zee\":[]}}, \"sequence\":{\"output_name\":\"%(JOB)s\",\"is_data\":true}}' "%vars());
        nfiles = sum(1 for line in open('%(DATAFILELIST)s_%(sa)s.dat' % vars()))
        nperjob = 40
        
        for i in range (0,int(math.ceil(float(nfiles)/float(nperjob)))) :
          os.system('%(JOBWRAPPER)s "./bin/HTT --cfg=%(CONFIG)s --json=%(JSONPATCH)s --offset=%(i)d --nlines=%(nperjob)d &> jobs/%(JOB)s-%(i)d.log" jobs/%(JOB)s-%(i)s.sh' %vars())
          os.system('%(JOBSUBMIT)s jobs/%(JOB)s-%(i)d.sh' % vars())
        file_persamp.write("%s %d\n" %(JOB, int(math.ceil(float(nfiles)/float(nperjob)))))



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
    'TT',
    'WJetsToLNu',
    'WJetsToLNu-LO',
    'WJetsToLNu-LO-ext',
    'VVTo2L2Nu',
    'VVTo2L2Nu-ext1',
    'ZZTo2L2Q',
    'ZZTo4L',
    'ZZTo4L-amcat',
    'WWTo1L1Nu2Q',
    'WWToLNuQQ',
    'WWToLNuQQ-ext',
    'WZJToLLLNu',
    'WZTo1L3Nu',
    'WZTo2L2Q',
    'WZTo1L1Nu2Q',
    'T-t',
    'Tbar-t',
    'T-tW',
    'Tbar-tW',
    'DYJetsToLL',
    'DYJetsToLL-LO-ext1',
    'DYJetsToLL-LO-ext2',
    #'DYJetsToLL_M-10to50-ext',
    #'DYJetsToLL_M-10to50',
    'DYJetsToLL_M-10-50-LO',
    #'DY1JetsToLL_M-10-50-LO',
    #'DY2JetsToLL_M-10-50-LO',
    #'DY3JetsToLL_M-10-50-LO',
    #'DYJetsToLL_M-150-LO',
    'DY1JetsToLL-LO',
    'DY2JetsToLL-LO',
    'DY3JetsToLL-LO',
    'DY4JetsToLL-LO',
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

  if options.qcd_study:
    #FILELIST='filelists/Feb25_MC_76X'
    FILELIST='filelists/Apr05_MC_80X'
    central_samples = [
      'QCDMuEnrichedPt15'
  #    'QCDEMEnrichedPt15-20',
  #    'QCDEMEnrichedPt20-30',
  #    'QCDFlat',
  #    'QCDbcToEPt20-30'
       ]
  for sa in central_samples:
      JOB='%s_2016' % (sa)
      JSONPATCH= (r"'{\"job\":{\"filelist\":\"%(FILELIST)s_%(sa)s.dat\"}, \"sequence\":{\"output_name\":\"%(JOB)s\"}}' "%vars());
      FLATJSONPATCH=FLATJSONPATCHOTH
      nperjob = 30
      if 'scale' in FLATJSONPATCH:
        nperjob = 20
      if 'DY' in sa and 'JetsToLL' in sa:
        FLATJSONPATCH=FLATJSONPATCHDYSIG
        #nperjob = 30
      if 'TT' in sa:
        nperjob = 30
        if 'scale' in FLATJSONPATCH:
          nperjob = 15
#      if 'WJetsToLNu' in sa or 'W1JetsToLNu' in sa or 'W2JetsToLNu' in sa or 'W3JetsToLNu' in sa or 'W4JetsToLNu' in sa:
#        nperjob = 30
      if 'QCD' in sa:
          nperjob = 15
      if 'DY' in sa and 'JetsToLL' in sa and taues_study:
        FLATJSONPATCH=""
        CONFIG='scripts/taues_config.json'
        JSONPATCH= (r"'{\"job\":{\"filelist\":\"%(FILELIST)s_%(sa)s.dat\",\"sequences\":{\"em\":[\"scale_e_lo\",\"scale_e_hi\",\"scale_j_lo\",\"scale_j_hi\"],\"et\":[\"scale_t_lo_3\",\"scale_t_hi_3\",\"scale_t_lo_2\",\"scale_t_hi_2\",\"scale_t_lo_1\",\"scale_t_hi_1\",\"scale_t_lo_2.5\",\"scale_t_hi_2.5\",\"scale_t_lo_1.5\",\"scale_t_hi_1.5\",\"scale_t_lo_0.5\",\"scale_t_hi_0.5\",\"scale_j_lo\",\"scale_j_hi\"],\"mt\":[\"scale_t_lo_3\",\"scale_t_hi_3\",\"scale_t_lo_2\",\"scale_t_hi_2\",\"scale_t_lo_1\",\"scale_t_hi_1\",\"scale_t_lo_2.5\",\"scale_t_hi_2.5\",\"scale_t_lo_1.5\",\"scale_t_hi_1.5\",\"scale_t_lo_0.5\",\"scale_t_hi_0.5\",\"scale_j_lo\",\"scale_j_hi\"],\"tt\":[\"scale_t_lo_3\",\"scale_t_hi_3\",\"scale_t_lo_2\",\"scale_t_hi_2\",\"scale_t_lo_1\",\"scale_t_hi_1\",\"scale_t_lo_2.5\",\"scale_t_hi_2.5\",\"scale_t_lo_1.5\",\"scale_t_hi_1.5\",\"scale_t_lo_0.5\",\"scale_t_hi_0.5\",\"scale_j_hi\",\"scale_j_lo\"]}}, \"sequence\":{\"output_name\":\"%(JOB)s\"}}' "%vars());
        nperjob = 10


      nfiles = sum(1 for line in open('%(FILELIST)s_%(sa)s.dat' % vars()))
      for i in range (0,int(math.ceil(float(nfiles)/float(nperjob)))) :
        os.system('%(JOBWRAPPER)s "./bin/HTT --cfg=%(CONFIG)s --json=%(JSONPATCH)s --flatjson=%(FLATJSONPATCH)s --offset=%(i)d --nlines=%(nperjob)d &> jobs/%(JOB)s-%(i)d.log" jobs/%(JOB)s-%(i)s.sh' %vars())
        os.system('%(JOBSUBMIT)s jobs/%(JOB)s-%(i)d.sh' % vars())
      file_persamp.write("%s %d\n" %(JOB, int(math.ceil(float(nfiles)/float(nperjob)))))
  

if options.proc_sm or options.proc_smbkg or options.proc_mssm or options.proc_Hhh or options.proc_all:
  for sa in signal_mc:
    JOB='%s_2016' % (sa)
    JSONPATCH= (r"'{\"job\":{\"filelist\":\"%(FILELIST)s_%(sa)s.dat\"}, \"sequence\":{\"output_name\":\"%(JOB)s\"}}' "%vars());
    FLATJSONPATCH=FLATJSONPATCHDYSIG
    if os.path.exists('%(FILELIST)s_%(sa)s.dat' %vars()):
      nfiles = sum(1 for line in open('%(FILELIST)s_%(sa)s.dat' % vars()))
      nperjob = 50
      for i in range (0,int(math.ceil(float(nfiles)/float(nperjob)))) :
        os.system('%(JOBWRAPPER)s "./bin/HTT --cfg=%(CONFIG)s --json=%(JSONPATCH)s --flatjson=%(FLATJSONPATCH)s --offset=%(i)d --nlines=%(nperjob)d &> jobs/%(JOB)s-%(i)d.log" jobs/%(JOB)s-%(i)s.sh' %vars())
        os.system('%(JOBSUBMIT)s jobs/%(JOB)s-%(i)d.sh' % vars())
      file_persamp.write("%s %d\n" %(JOB, int(math.ceil(float(nfiles)/float(nperjob)))))
      
NLO_FILELIST='filelists/Jul22_MC_80X'
      
if options.proc_mssm_nlo or options.proc_mssm_nlo_qsh:
  for sa in nlo_signal_mc:
    JOB='%s_2016' % (sa)
    JSONPATCH= (r"'{\"job\":{\"filelist\":\"%(NLO_FILELIST)s_%(sa)s.dat\",\"file_prefix\":\"root://gfe02.grid.hep.ph.ic.ac.uk:1097//store/user/dwinterb/Jul22_MC_80X/\"}, \"sequence\":{\"output_name\":\"%(JOB)s\"}}' "%vars());
    FLATJSONPATCH=FLATJSONPATCHDYSIG
    if os.path.exists('%(NLO_FILELIST)s_%(sa)s.dat' %vars()):
      nfiles = sum(1 for line in open('%(NLO_FILELIST)s_%(sa)s.dat' % vars()))
      nperjob = 50
      for i in range (0,int(math.ceil(float(nfiles)/float(nperjob)))) :
        os.system('%(JOBWRAPPER)s "./bin/HTT --cfg=%(CONFIG)s --json=%(JSONPATCH)s --flatjson=%(FLATJSONPATCH)s --offset=%(i)d --nlines=%(nperjob)d &> jobs/%(JOB)s-%(i)d.log" jobs/%(JOB)s-%(i)s.sh' %vars())
        os.system('%(JOBSUBMIT)s jobs/%(JOB)s-%(i)d.sh' % vars())
      file_persamp.write("%s %d\n" %(JOB, int(math.ceil(float(nfiles)/float(nperjob)))))
      
NLO_QSH_FILELIST='filelists/May23_MC_80X'

if options.proc_mssm_nlo_qsh:
  for sa in nlo_qsh_signal_mc:
    JOB='%s_2016' % (sa)
    JSONPATCH= (r"'{\"job\":{\"filelist\":\"%(NLO_QSH_FILELIST)s_%(sa)s.dat\",\"file_prefix\":\"root://gfe02.grid.hep.ph.ic.ac.uk:1097//store/user/dwinterb/May23_MC_80X/\"}, \"sequence\":{\"output_name\":\"%(JOB)s\"}}' "%vars());
    FLATJSONPATCH=FLATJSONPATCHDYSIG
    if os.path.exists('%(NLO_QSH_FILELIST)s_%(sa)s.dat' %vars()):
      nfiles = sum(1 for line in open('%(NLO_QSH_FILELIST)s_%(sa)s.dat' % vars()))
      nperjob = 50
      for i in range (0,int(math.ceil(float(nfiles)/float(nperjob)))) :
        os.system('%(JOBWRAPPER)s "./bin/HTT --cfg=%(CONFIG)s --json=%(JSONPATCH)s --flatjson=%(FLATJSONPATCH)s --offset=%(i)d --nlines=%(nperjob)d &> jobs/%(JOB)s-%(i)d.log" jobs/%(JOB)s-%(i)s.sh' %vars())
        os.system('%(JOBSUBMIT)s jobs/%(JOB)s-%(i)d.sh' % vars())
      file_persamp.write("%s %d\n" %(JOB, int(math.ceil(float(nfiles)/float(nperjob)))))


