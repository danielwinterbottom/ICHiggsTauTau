#!/usr/bin/env python

import sys
from optparse import OptionParser
import os
import math

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

parser.add_option("--list_backup", dest="slbackupname", type='string', default='prevlist',
                  help="Name you want to give to the previous files_per_samples file, in case you're resubmitting a subset of jobs")

parser.add_option("--scales", dest="scales", type='string', default='default',
                  help="List of systematic shifts to process")

parser.add_option("--taues_study", dest="taues_study", action='store_true', default=False,
                  help="Run lots of tau ES shifts jsut for DY")

parser.add_option("--qcd_study", dest="qcd_study", action='store_true', default=False,
                  help="Run QCD MC sample for QCD study")


(options, args) = parser.parse_args()
if options.wrapper: JOBWRAPPER=options.wrapper
if options.submit:  JOBSUBMIT=options.submit

BACKUPNAME = options.slbackupname

#channels = options.channels
scales = options.scales
taues_study = options.taues_study


scale_list = scales.split(',')
flatjsonlist = []
flatjsonlistdysig = []
flatjsonlist.append("job:sequences:all:")
flatjsonlistdysig.append("job:sequences:all:")
for scale in scale_list: 
  if scale == "default":
    flatjsonlist.append("^%(scale)s"%vars())
    flatjsonlistdysig.append("^%(scale)s"%vars())
  elif scale == "scale_t":
    flatjsonlistdysig.append("^%(scale)s_hi^%(scale)s_lo"%vars()) 
  else:
    flatjsonlist.append("^%(scale)s_hi^%(scale)s_lo"%vars()) 
    flatjsonlistdysig.append("^%(scale)s_hi^%(scale)s_lo"%vars()) 

FLATJSONPATCHOTH = ''.join(flatjsonlist)
FLATJSONPATCHDYSIG = ''.join(flatjsonlistdysig)


CONFIG='scripts/config.json'


FILELIST='filelists/Mar05_MC_76X'

signal_mc = [ ]
signal_vh = [ ] 
signal_mc_ww = [ ]

if os.path.isfile("./jobs/files_per_sample.txt"):
  os.system("mv ./jobs/files_per_sample.txt ./jobs/files_per_sample-%(BACKUPNAME)s.txt"%vars())

file_persamp = open("./jobs/files_per_sample.txt", "w")

if options.proc_sm or options.proc_all or options.proc_smbkg:
  masses = ['120','125','130']
  if options.short_signal or options.proc_smbkg: masses = ['125']
  for mass in masses :
    signal_mc += [
      'GluGluHToTauTau_M-'+mass,
      'VBFHToTauTau_M-'+mass,
      'ZHToTauTau_M-'+mass,
      'WplusHToTauTau_M-'+mass,
      'WminusHToTauTau_M-'+mass,
      'TTHToTauTau_M-'+mass
    ]
if options.proc_mssm or options.proc_all:
  masses = ['80','90','100','110','120','130','140','160','180','200','250','300','350','400','450','500','600','700','800','900','1000','1200','1400','1500','1600','1800','2000','2300','2600','2900','3200']
  if options.short_signal: masses = ['160']
  for mass in masses :
    signal_mc += [
      'SUSYGluGluToHToTauTau_M-'+mass,
      'SUSYGluGluToBBHToTauTau_M-'+mass
    ]

if options.proc_Hhh:
  masses = ['250','260','270','280','300','320','340','350','400','450','500','550','600','650','700','800','900']
  for mass in masses :
    signal_mc += [
       'GluGluToRadionToHHTo2B2Tau_M-'+mass
    ]

if options.proc_data or options.proc_all:
  data_samples = [
   'SingleMuon-2015D',
   'SingleElectron-2015D',
   'MuonEG-2015D',
   'Tau-2015D'
  ]


  DATAFILELIST="./filelists/Mar05_Data_76X"

  for sa in data_samples:
      JOB='%s_2015' % (sa)
      JSONPATCH= (r"'{\"job\":{\"filelist\":\"%(DATAFILELIST)s_%(sa)s.dat\",\"file_prefix\":\"root://xrootd.grid.hep.ph.ic.ac.uk//store/user/adewit/Mar05_Data_76X/\",\"sequences\":{\"em\":[],\"et\":[],\"mt\":[],\"tt\":[]}}, \"sequence\":{\"output_name\":\"%(JOB)s\",\"is_data\":true}}' "%vars());
      nfiles = sum(1 for line in open('%(DATAFILELIST)s_%(sa)s.dat' % vars()))
      nperjob = 40 
      for i in range (0,int(math.ceil(float(nfiles)/float(nperjob)))) :
        os.system('%(JOBWRAPPER)s "./bin/HTT --cfg=%(CONFIG)s --json=%(JSONPATCH)s --offset=%(i)d --nlines=%(nperjob)d &> jobs/%(JOB)s-%(i)d.log" jobs/%(JOB)s-%(i)s.sh' %vars())
        os.system('%(JOBSUBMIT)s jobs/%(JOB)s-%(i)d.sh' % vars())
      file_persamp.write("%s %d\n" %(JOB, int(math.ceil(float(nfiles)/float(nperjob)))))


if options.proc_qcd:
  qcd_samples = [
    'QCD_Ht100to200',
    'QCD_Ht200to300',
    'QCD_Ht300to500',
    'QCD_Ht500to700',
    'QCD_Ht700to1000',
    'QCD_Ht1000to1500',
    'QCD_Ht1500to2000',
    'QCD_Ht2000toInf'
  ]

  for sa in qcd_samples:
      JOB='%s_2015' % (sa)
      JSONPATCH= (r"'{\"job\":{\"filelist\":\"%(FILELIST)s_%(sa)s.dat\",\"sequences\":{\"em\":[],\"et\":[],\"mt\":[],\"tt\":[]}}, \"sequence\":{\"output_name\":\"%(JOB)s\"}}' "%vars());
      nfiles = sum(1 for line in open('%(FILELIST)s_%(sa)s.dat' % vars()))
      nperjob = 40 
      for i in range (0,int(math.ceil(float(nfiles)/float(nperjob)))) :
        os.system('%(JOBWRAPPER)s "./bin/HTT --cfg=%(CONFIG)s --json=%(JSONPATCH)s --offset=%(i)d --nlines=%(nperjob)d &> jobs/%(JOB)s-%(i)d.log" jobs/%(JOB)s-%(i)s.sh' %vars())
        os.system('%(JOBSUBMIT)s jobs/%(JOB)s-%(i)d.sh' % vars())
      file_persamp.write("%s %d\n" %(JOB, int(math.ceil(float(nfiles)/float(nperjob)))))


if options.proc_bkg or options.proc_all or options.qcd_study:
  central_samples = [
    'TT-ext',
    #'WJetsToLNu',
    'WJetsToLNu-LO',
    'W1JetsToLNu-LO',
    'W2JetsToLNu-LO',
    'W3JetsToLNu-LO',
    'W4JetsToLNu-LO',
    'ZZTo4L',
    'VVTo2L2Nu',
    'ZZTo2L2Q',
    'WWTo1L1Nu2Q',
    'WZJetsToLLLNu',
    'WZTo1L3Nu',
    'WZTo2L2Q',
    'WZTo1L1Nu2Q',
    'T-t',
    'Tbar-t',
    'T-tW',
    'Tbar-tW',
#   'DYJetsToLL',
#    'DYJetsToLL_M-50-LO',
 #   'DYJetsToLL_M-10',
    'DYJetsToLL_M-10-ext',
    #'DYJetsToLL_M-150-LO',
    'DYJetsToLL_M-50-LO-ext',
    'DY1JetsToLL_M-50-LO',
    'DY2JetsToLL_M-50-LO',
    'DY3JetsToLL_M-50-LO',
    'DY4JetsToLL_M-50-LO',
     ]

  if options.qcd_study:
    #FILELIST='filelists/Feb25_MC_76X'
    FILELIST='filelists/Mar05_MC_76X'
    central_samples = [
      'QCDMuEnriched',
      'QCDEMEnrichedPt15-20',
      'QCDEMEnrichedPt20-30',
      'QCDFlat',
      'QCDbcToEPt20-30'
       ]
  for sa in central_samples:
      JOB='%s_2015' % (sa)
      JSONPATCH= (r"'{\"job\":{\"filelist\":\"%(FILELIST)s_%(sa)s.dat\"}, \"sequence\":{\"output_name\":\"%(JOB)s\"}}' "%vars());
      FLATJSONPATCH=FLATJSONPATCHOTH
      nperjob = 50
      if 'scale' in FLATJSONPATCH:
        nperjob = 40
      if 'DY' in sa and 'JetsToLL' in sa:
        FLATJSONPATCH=FLATJSONPATCHDYSIG
        #nperjob = 30
        if 'scale' in FLATJSONPATCH:
          nperjob = 20
#      if 'WJetsToLNu' in sa or 'W1JetsToLNu' in sa or 'W2JetsToLNu' in sa or 'W3JetsToLNu' in sa or 'W4JetsToLNu' in sa:
#        nperjob = 30
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
    JOB='%s_2015' % (sa)
    JSONPATCH= (r"'{\"job\":{\"filelist\":\"%(FILELIST)s_%(sa)s.dat\"}, \"sequence\":{\"output_name\":\"%(JOB)s\"}}' "%vars());
    FLATJSONPATCH=FLATJSONPATCHDYSIG
    if os.path.exists('%(FILELIST)s_%(sa)s.dat' %vars()):
      nfiles = sum(1 for line in open('%(FILELIST)s_%(sa)s.dat' % vars()))
      nperjob = 50
      for i in range (0,int(math.ceil(float(nfiles)/float(nperjob)))) :
        os.system('%(JOBWRAPPER)s "./bin/HTT --cfg=%(CONFIG)s --json=%(JSONPATCH)s --flatjson=%(FLATJSONPATCH)s --offset=%(i)d --nlines=%(nperjob)d &> jobs/%(JOB)s-%(i)d.log" jobs/%(JOB)s-%(i)s.sh' %vars())
        os.system('%(JOBSUBMIT)s jobs/%(JOB)s-%(i)d.sh' % vars())
      file_persamp.write("%s %d\n" %(JOB, int(math.ceil(float(nfiles)/float(nperjob)))))

