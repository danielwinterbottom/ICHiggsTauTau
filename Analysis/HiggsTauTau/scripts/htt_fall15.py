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


CHANNELS_2015 = ['et', 'mt', 'em', 'tt']
#CHANNELS_2011 = ['et', 'mt', 'em']
CHANNELS      = [ ] ## will be assigned from 2011 or 2012

def split_callback(option, opt, value, parser):
  setattr(parser.values, option.dest, value.split(','))


parser = OptionParser()
parser.add_option("--wrapper", dest="wrapper",
                  help="Specify the job-wrapper script. The current wrapper is '%(JOBWRAPPER)s'."
                  " Using the --wrapper option overrides both the default and the environment variable. " % vars())

parser.add_option("--submit", dest="submit",
                  help="Specify the job-submission method. The current method is '%(JOBSUBMIT)s'"
                  " Using the --submit option overrides both the default and the environment variable. " % vars())


#parser.add_option("-e", "--era", dest="era",
#                  help="Specify a data-taking era to process. This option both sets the config files that will be used, "
#                  "e.g. scripts/ERA_2012.cfg, and selects the appropriate data filelists. "
#                  "The following options are supported by default: For 2012 %(SUPPORTED_ERAS_2012)s, for 2011 %(SUPPORTED_ERAS_2011)s" % vars(), metavar="ERA")

parser.add_option("--data", dest="proc_data", action='store_true', default=False,
                  help="Process data samples (including embedded)")

parser.add_option("--bkg", dest="proc_bkg", action='store_true', default=False,
                  help="Process background mc samples")

parser.add_option("--qcd", dest="proc_qcd", action='store_true', default=False,
                  help="Process QCD mc samples")

parser.add_option("--sm", dest="proc_sm", action='store_true', default=False,
                  help="Process signal SM mc samples")

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

parser.add_option("-c", "--channels", dest="channels", type='string', action='callback',callback=split_callback,
                  help="A comma separated list of channels to ignore.  Supported channels: For data_2015 %(CHANNELS_2015)s" % vars())

parser.add_option("--list_backup", dest="slbackupname", type='string', default='prevlist',
                  help="Name you want to give to the previous files_per_samples file, in case you're resubmitting a subset of jobs")

parser.add_option("--scales", dest="scales", action='store_true', default=False,
                  help="Process tau ES up/down")

parser.add_option("--taues_study", dest="taues_study", action='store_true', default=False,
                  help="Run lots of tau ES shifts jsut for DY")


(options, args) = parser.parse_args()
if options.wrapper: JOBWRAPPER=options.wrapper
if options.submit:  JOBSUBMIT=options.submit

BACKUPNAME = options.slbackupname

channels = options.channels
scales = options.scales
taues_study = options.taues_study







CONFIG='scripts/config.json'
#if options.proc_data or options.proc_all:
#  for ch in channels:
#    if '0' in scales:
#      JOB='Data_%s_%s' % (ch,YR)
#      os.system('%(JOBWRAPPER)s "./bin/HiggsTauTau --cfg=%(CONFIG)s %(PREFIXDATA)s --filelist=%(FILELIST)s_Data_%(ERA)s_%(ch)s_skim.dat --channel=%(ch)s --output_name=%(JOB)s.root &> jobs/%(JOB)s.log" jobs/%(JOB)s.sh' % vars())
#      os.system('%(JOBSUBMIT)s jobs/%(JOB)s.sh' % vars())
#
#    JOB='DYJetsToTauTau%s_2015' % (sp)
#    JSONPATCH= (r"'{\"job\":{ \"sequences\":{\"em\":[]},\"filelist\":\"%(FILELIST)s_DYJetsToLL%(sp)s_M-50.dat\"}, \"sequence\":{\"output_name\":\"%(JOB)s\", \"hadronic_tau_selector\":1,\"faked_tau_selector\":2,\"ztautau_mode\":1}}' "%vars());
 

FILELIST='filelists/Feb12_MC_76X'
#FILELIST13='filelists/July13_MC_76X'

signal_mc = [ ]
signal_vh = [ ] 
signal_mc_ww = [ ]

if os.path.isfile("./jobs/files_per_sample.txt"):
  os.system("mv ./jobs/files_per_sample.txt ./jobs/files_per_sample-%(BACKUPNAME)s.txt"%vars())

file_persamp = open("./jobs/files_per_sample.txt", "w")

if options.proc_sm or options.proc_all:
#  masses = ['90','95','100','105','110','115','120','125','130','135','140','145','150','155','160']
  masses = ['120','125','130']
  #masses = ['125','130']
  if options.short_signal: masses = ['125']
  for mass in masses :
    signal_mc += [
#      'SUSYGluGluToHToTauTau_M-'+mass,
#      'VBFHToTauTau_M-'+mass, 
      'GluGluHToTauTau_M-'+mass,
#      'WH_ZH_TTH_HToTauTau_M-'+mass
    ]
    signal_vh += [
#      'WH_ZH_TTH_HToTauTau_M-'+mass
    ]
#  ww_masses = ['110','115','120','125','130','135','140','145','150','155','160']
#  if options.short_signal: ww_masses = ['125']
#  for ww_mass in ww_masses :
#    signal_mc_ww += [
#      'GluGluToHToWWTo2LAndTau2Nu_M-'+ww_mass,
#      'VBF_HToWWTo2LAndTau2Nu_M-'+ww_mass, 
#    ]
if options.proc_mssm or options.proc_all:
  masses = ['80','90','100','110','120','130','140','160','180','200','250','300','350','400','450','500','600','700','800','900','1000','1200','1400','1500','1600','1800','2000','2300','2600','2900','3200']
#  if not options.do_2011: masses += ['80','110']
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

#if options.proc_Hhh or options.proc_all:
#  Hmasses = ['260','270','280','290','300','310','320','330','340','350']
#  if options.short_signal: Hmasses = ['300']
#  for Hmass in Hmasses : 
#    signal_mc += [
#      'GluGluToHTohhTo2Tau2B_mH-'+Hmass
#    ]
  #Add SM 125 signal H->tautau/bb for H->hh analysis
#  signal_mc += [
#    'GluGluToHToTauTau_M-125',
#    'VBF_HToTauTau_M-125', 
#    'WH_ZH_TTH_HToTauTau_M-125',
#    'WH_WToLNu_HToBB_M-125',
#    'ZH_ZToLL_HToBB_M-125'
#  ]
#  signal_vh += [
#    'WH_ZH_TTH_HToTauTau_M-125'
#  ]
#  if options.extra_signal_profile:
#    Amasses = ['250','260','270','280','290','300','310','320','330','340','350']
#    ATauTaumasses = ['260','270','280','290','300','310','320','330','340','350']
#    bbHmasses = ['90','100','110','120','130','140','160','180','200','250','300','350','400']
#    if options.short_signal: Amasses = ['300']
#    if options.short_signal: ATauTaumasses = ['300']
#    if options.short_signal: bbHmasses = ['120','300']
#    for Hmass in Hmasses : 
#      signal_mc += [
#        'SUSYBBHTohhTo2Tau2B_mH-'+Hmass
#      ]
#    for Amass in Amasses :
#      signal_mc += [
#        'GluGluToAToZhToLLBB_mA-'+Amass
#      ]
#    for Amass in ATauTaumasses :
#      signal_mc += [
#        'GluGluToAToZhToLLTauTau_mA-'+Amass
#        #'GluGluToAToZhToBBTauTau_mA-'+Amass
#      ]
#    for Hmass in bbHmasses : 
#      signal_mc += [
#        'SUSYBBHToTauTau_M-'+Hmass
#      ]
#
if options.proc_data or options.proc_all:
  data_samples = [
#   'SingleMuon-2015B-17Jul',
#   'MuonEG-2015B-17Jul',
#   'SingleElectron-2015B-17Jul',
#   'Tau-2015B-17Jul',
   'SingleMuon-2015D',
   'SingleElectron-2015D',
   'MuonEG-2015D',
   'Tau-2015D'
  ]


  DATAFILELIST="./filelists/Feb12_Data_76X"

  for sa in data_samples:
      JOB='%s_2015' % (sa)
      JSONPATCH= (r"'{\"job\":{\"filelist\":\"%(DATAFILELIST)s_%(sa)s.dat\",\"file_prefix\":\"root://xrootd.grid.hep.ph.ic.ac.uk//store/user/adewit/Feb12_Data_76X/\",\"sequences\":{\"em\":[],\"et\":[],\"mt\":[],\"tt\":[]}}, \"sequence\":{\"output_name\":\"%(JOB)s\",\"is_data\":true}}' "%vars());
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


if options.proc_bkg or options.proc_all:
  central_samples = [
    #'TTJets',
    'TT-ext',
    'WJetsToLNu',
    'WJetsToLNu-LO',
		#'WJetsToLNu',
    #'WWinclusive',
    #'ZZinclusive',
    #'WZinclusive',
    'ZZTo4L',
    #'VVTo2L2Nu',
    'ZZTo2L2Q',
    'WWTo1L1Nu2Q',
    'WZJetsTo3LNu',
    'WZTo1L3Nu',
    #'WZTo2L2Q',
    'WZTo1L1Nu2Q',
    'T-t',
    'Tbar-t',
    'T-tW',
    'Tbar-tW',
    #'DYJetsToLL',
    'DYJetsToLL_M-50-LO',
    'DY1JetsToLL_M-50-LO',
    'DY2JetsToLL_M-50-LO',
    'DY3JetsToLL_M-50-LO',
    'DY4JetsToLL_M-50-LO',
    #'DYJetsToLL_M-50_HT100-200',
    #'DYJetsToLL_M-50_HT200-400',
    #'DYJetsToLL_M-50_HT400-600',
    #'DYJetsToLL_M-50_HT600-Inf',
    #'WJetsToLNu_HT100-200',
    #'WJetsToLNu_HT200-400',
    #'WJetsToLNu_HT400-600',
    #'WJetsToLNu_HT600-Inf',
     ]

  for sa in central_samples:
      JOB='%s_2015' % (sa)
      JSONPATCH= (r"'{\"job\":{\"filelist\":\"%(FILELIST)s_%(sa)s.dat\",\"sequences\":{\"em\":[],\"et\":[],\"mt\":[],\"tt\":[]}}, \"sequence\":{\"output_name\":\"%(JOB)s\"}}' "%vars());
      nperjob = 60
      if scales:
        JSONPATCH= (r"'{\"job\":{\"filelist\":\"%(FILELIST)s_%(sa)s.dat\",\"sequences\":{\"em\":[\"scale_j_lo\",\"scale_j_hi\"],\"et\":[\"scale_j_lo\",\"scale_j_hi\"],\"mt\":[\"scale_j_lo\",\"scale_j_hi\"],\"tt\":[\"scale_j_lo\",\"scale_j_hi\"]}}, \"sequence\":{\"output_name\":\"%(JOB)s\"}}' "%vars());
        if 'TT-ext' in sa:
          nperjob = 40
      if 'DY' in sa and 'JetsToLL' in sa and scales:
        JSONPATCH= (r"'{\"job\":{\"filelist\":\"%(FILELIST)s_%(sa)s.dat\",\"sequences\":{\"em\":[\"scale_e_lo\",\"scale_e_hi\",\"scale_j_lo\",\"scale_j_hi\"],\"et\":[\"scale_t_lo\",\"scale_t_hi\",\"scale_j_lo\",\"scale_j_hi\"],\"mt\":[\"scale_t_lo\",\"scale_t_hi\",\"scale_j_lo\",\"scale_j_hi\"],\"tt\":[\"scale_t_lo\",\"scale_t_hi\",\"scale_j_hi\",\"scale_j_lo\"]}}, \"sequence\":{\"output_name\":\"%(JOB)s\"}}' "%vars());
        nperjob = 40
      if 'DY' in sa and 'JetsToLL' in sa and taues_study:
        CONFIG='scripts/taues_config.json'
        JSONPATCH= (r"'{\"job\":{\"filelist\":\"%(FILELIST)s_%(sa)s.dat\",\"sequences\":{\"em\":[\"scale_e_lo\",\"scale_e_hi\",\"scale_j_lo\",\"scale_j_hi\"],\"et\":[\"scale_t_lo_3\",\"scale_t_hi_3\",\"scale_t_lo_2\",\"scale_t_hi_2\",\"scale_t_lo_1\",\"scale_t_hi_1\",\"scale_t_lo_2.5\",\"scale_t_hi_2.5\",\"scale_t_lo_1.5\",\"scale_t_hi_1.5\",\"scale_t_lo_0.5\",\"scale_t_hi_0.5\",\"scale_j_lo\",\"scale_j_hi\"],\"mt\":[\"scale_t_lo_3\",\"scale_t_hi_3\",\"scale_t_lo_2\",\"scale_t_hi_2\",\"scale_t_lo_1\",\"scale_t_hi_1\",\"scale_t_lo_2.5\",\"scale_t_hi_2.5\",\"scale_t_lo_1.5\",\"scale_t_hi_1.5\",\"scale_t_lo_0.5\",\"scale_t_hi_0.5\",\"scale_j_lo\",\"scale_j_hi\"],\"tt\":[\"scale_t_lo_3\",\"scale_t_hi_3\",\"scale_t_lo_2\",\"scale_t_hi_2\",\"scale_t_lo_1\",\"scale_t_hi_1\",\"scale_t_lo_2.5\",\"scale_t_hi_2.5\",\"scale_t_lo_1.5\",\"scale_t_hi_1.5\",\"scale_t_lo_0.5\",\"scale_t_hi_0.5\",\"scale_j_hi\",\"scale_j_lo\"]}}, \"sequence\":{\"output_name\":\"%(JOB)s\"}}' "%vars());
        nperjob = 10


      nfiles = sum(1 for line in open('%(FILELIST)s_%(sa)s.dat' % vars()))
      for i in range (0,int(math.ceil(float(nfiles)/float(nperjob)))) :
        os.system('%(JOBWRAPPER)s "./bin/HTT --cfg=%(CONFIG)s --json=%(JSONPATCH)s --offset=%(i)d --nlines=%(nperjob)d &> jobs/%(JOB)s-%(i)d.log" jobs/%(JOB)s-%(i)s.sh' %vars())
        os.system('%(JOBSUBMIT)s jobs/%(JOB)s-%(i)d.sh' % vars())
      file_persamp.write("%s %d\n" %(JOB, int(math.ceil(float(nfiles)/float(nperjob)))))

        
#      if PRODUCTION=='June6' and (not options.do_2011):
#        central_samples.remove('TTJets')
#        central_samples += [
#          'TTJetsFullLept',
#          'TTJetsSemiLept',
#          'TTJetsHadronicExt'
#        ]

'''
  soups = ['']
  nfiles = sum(1 for line in open('%(FILELIST)s_DYJetsToLL.dat' % vars()))
  nperjob = 30 
  for sp in soups:  
    JOB='DYJetsToTauTau%s_2015' % (sp)
    JSONPATCH= (r"'{\"job\":{ \"sequences\":{\"em\":[]},\"filelist\":\"%(FILELIST)s_DYJetsToLL%(sp)s.dat\"}, \"sequence\":{\"output_name\":\"%(JOB)s\", \"hadronic_tau_selector\":1,\"faked_tau_selector\":2,\"ztautau_mode\":1}}' "%vars());
    for i in range (0,int(math.ceil(float(nfiles)/float(nperjob)))) :
      os.system('%(JOBWRAPPER)s "./bin/HTT --cfg=%(CONFIG)s --json=%(JSONPATCH)s --offset=%(i)d --nlines=%(nperjob)d &> jobs/%(JOB)s-%(i)d.log" jobs/%(JOB)s-%(i)s.sh' %vars())
      os.system('%(JOBSUBMIT)s jobs/%(JOB)s-%(i)d.sh' % vars())
    file_persamp.write("%s %d\n" %(JOB, int(math.ceil(float(nfiles)/float(nperjob)))))


    JOB='DYJetsToLL%s_2015' % (sp)
    JSONPATCH= (r"'{\"job\":{\"sequences\":{\"em\":[],\"et\":[],\"mt\":[],\"tt\":[]},\"filelist\":\"%(FILELIST)s_DYJetsToLL%(sp)s.dat\"}, \"sequence\":{\"output_name\":\"%(JOB)s\",\"ztautau_mode\":2}}' "%vars());
    for i in range (0,int(math.ceil(float(nfiles)/float(nperjob)))) :
      os.system('%(JOBWRAPPER)s "./bin/HTT --cfg=%(CONFIG)s --json=%(JSONPATCH)s --offset=%(i)d --nlines=%(nperjob)d &> jobs/%(JOB)s-%(i)d.log" jobs/%(JOB)s-%(i)s.sh' %vars())
      os.system('%(JOBSUBMIT)s jobs/%(JOB)s-%(i)d.sh' % vars())
    file_persamp.write("%s %d\n" %(JOB, int(math.ceil(float(nfiles)/float(nperjob)))))

#				#Don't run these for emu channel:

    JOB='DYJetsToLL-L%s_2015' % (sp)
    JSONPATCH= (r"'{\"job\":{\"channels\":[\"et\",\"mt\",\"tt\"],\"sequences\":{\"em\":[],\"et\":[],\"mt\":[],\"tt\":[]},\"filelist\":\"%(FILELIST)s_DYJetsToLL%(sp)s.dat\"}, \"sequence\":{\"output_name\":\"%(JOB)s\",\"ztautau_mode\":2,\"faked_tau_selector\":1}}' "%vars());
    for i in range (0,int(math.ceil(float(nfiles)/float(nperjob)))) :
      os.system('%(JOBWRAPPER)s "./bin/HTT --cfg=%(CONFIG)s --json=%(JSONPATCH)s --offset=%(i)d --nlines=%(nperjob)d &> jobs/%(JOB)s-%(i)d.log" jobs/%(JOB)s-%(i)s.sh' %vars())
      os.system('%(JOBSUBMIT)s jobs/%(JOB)s-%(i)d.sh' % vars())
    file_persamp.write("%s %d\n" %(JOB, int(math.ceil(float(nfiles)/float(nperjob)))))


    JOB='DYJetsToLL-J%s_2015' % (sp)
    JSONPATCH= (r"'{\"job\":{\"channels\":[\"et\",\"mt\",\"tt\"],\"sequences\":{\"em\":[],\"et\":[],\"mt\":[],\"tt\":[]},\"filelist\":\"%(FILELIST)s_DYJetsToLL%(sp)s.dat\"}, \"sequence\":{\"output_name\":\"%(JOB)s\",\"ztautau_mode\":2,\"faked_tau_selector\":2}}' "%vars());
    for i in range (0,int(math.ceil(float(nfiles)/float(nperjob)))) :
      os.system('%(JOBWRAPPER)s "./bin/HTT --cfg=%(CONFIG)s --json=%(JSONPATCH)s --offset=%(i)d --nlines=%(nperjob)d &> jobs/%(JOB)s-%(i)d.log" jobs/%(JOB)s-%(i)s.sh' %vars())
      os.system('%(JOBSUBMIT)s jobs/%(JOB)s-%(i)d.sh' % vars())
    file_persamp.write("%s %d\n" %(JOB, int(math.ceil(float(nfiles)/float(nperjob)))))


    JOB='DYJetsToTauTau-L%s_2015' % (sp)
    JSONPATCH= (r"'{\"job\":{\"channels\":[\"et\",\"mt\",\"tt\"],\"sequences\":{\"em\":[],\"et\":[],\"mt\":[],\"tt\":[]},\"filelist\":\"%(FILELIST)s_DYJetsToLL%(sp)s.dat\"}, \"sequence\":{\"output_name\":\"%(JOB)s\",\"ztautau_mode\":1,\"faked_tau_selector\":1}}' "%vars());
    for i in range (0,int(math.ceil(float(nfiles)/float(nperjob)))) :
      os.system('%(JOBWRAPPER)s "./bin/HTT --cfg=%(CONFIG)s --json=%(JSONPATCH)s --offset=%(i)d --nlines=%(nperjob)d &> jobs/%(JOB)s-%(i)d.log" jobs/%(JOB)s-%(i)s.sh' %vars())
      os.system('%(JOBSUBMIT)s jobs/%(JOB)s-%(i)d.sh' % vars())
    file_persamp.write("%s %d\n" %(JOB, int(math.ceil(float(nfiles)/float(nperjob)))))
#
#    #JOB='DYJetsToTauTau10-50%s_2015' % (sp)
#    #JSONPATCH= (r"'{\"job\":{ \"sequences\":{\"em\":[]},\"filelist\":\"%(FILELIST)s_DYJetsToLL10-50%(sp)s.dat\"}, \"sequence\":{\"output_name\":\"%(JOB)s\", \"hadronic_tau_selector\":1,\"faked_tau_selector\":2,\"ztautau_mode\":1}}' "%vars());
#    #for i in range (0,int(math.ceil(float(nfiles)/float(nperjob)))) :
#    #  os.system('%(JOBWRAPPER)s "./bin/HTT --cfg=%(CONFIG)s --json=%(JSONPATCH)s --offset=%(i)d --nlines=%(nperjob)d &> jobs/%(JOB)s-%(i)d.log" jobs/%(JOB)s-%(i)s.sh' %vars())
#    #  os.system('%(JOBSUBMIT)s jobs/%(JOB)s-%(i)d.sh' % vars())
    #file_persamp.write("%s %d\n" %(JOB, int(math.ceil(float(nfiles)/float(nperjob)))))


#    JOB='DYJetsToLL10-50%s_2015' % (sp)
#    JSONPATCH= (r"'{\"job\":{\"sequences\":{\"em\":[],\"et\":[],\"mt\":[],\"tt\":[]},\"filelist\":\"%(FILELIST)s_DYJetsToLL10-50%(sp)s.dat\"}, \"sequence\":{\"output_name\":\"%(JOB)s\",\"ztautau_mode\":2}}' "%vars());
#    for i in range (0,int(math.ceil(float(nfiles)/float(nperjob)))) :
#      os.system('%(JOBWRAPPER)s "./bin/HTT --cfg=%(CONFIG)s --json=%(JSONPATCH)s --offset=%(i)d --nlines=%(nperjob)d &> jobs/%(JOB)s-%(i)d.log" jobs/%(JOB)s-%(i)s.sh' %vars())
#      os.system('%(JOBSUBMIT)s jobs/%(JOB)s-%(i)d.sh' % vars())
#    file_persamp.write("%s %d\n" %(JOB, int(math.ceil(float(nfiles)/float(nperjob)))))

				#Don't run these for emu channel:

#    JOB='DYJetsToLL10-50-L%s_2015' % (sp)
#    JSONPATCH= (r"'{\"job\":{\"channels\":[\"et\",\"mt\",\"tt\"],\"sequences\":{\"em\":[],\"et\":[],\"mt\":[],\"tt\":[]},\"filelist\":\"%(FILELIST)s_DYJetsToLL10-50%(sp)s.dat\"}, \"sequence\":{\"output_name\":\"%(JOB)s\",\"ztautau_mode\":2,\"faked_tau_selector\":1}}' "%vars());
#    for i in range (0,int(math.ceil(float(nfiles)/float(nperjob)))) :
#      os.system('%(JOBWRAPPER)s "./bin/HTT --cfg=%(CONFIG)s --json=%(JSONPATCH)s --offset=%(i)d --nlines=%(nperjob)d &> jobs/%(JOB)s-%(i)d.log" jobs/%(JOB)s-%(i)s.sh' %vars())
#      os.system('%(JOBSUBMIT)s jobs/%(JOB)s-%(i)d.sh' % vars())
#    file_persamp.write("%s %d\n" %(JOB, int(math.ceil(float(nfiles)/float(nperjob)))))


#    JOB='DYJetsToLL10-50-J%s_2015' % (sp)
#    JSONPATCH= (r"'{\"job\":{\"channels\":[\"et\",\"mt\",\"tt\"],\"sequences\":{\"em\":[],\"et\":[],\"mt\":[],\"tt\":[]},\"filelist\":\"%(FILELIST)s_DYJetsToLL10-50%(sp)s.dat\"}, \"sequence\":{\"output_name\":\"%(JOB)s\",\"ztautau_mode\":2,\"faked_tau_selector\":2}}' "%vars());
#    for i in range (0,int(math.ceil(float(nfiles)/float(nperjob)))) :
#      os.system('%(JOBWRAPPER)s "./bin/HTT --cfg=%(CONFIG)s --json=%(JSONPATCH)s --offset=%(i)d --nlines=%(nperjob)d &> jobs/%(JOB)s-%(i)d.log" jobs/%(JOB)s-%(i)s.sh' %vars())
#      os.system('%(JOBSUBMIT)s jobs/%(JOB)s-%(i)d.sh' % vars())
#    file_persamp.write("%s %d\n" %(JOB, int(math.ceil(float(nfiles)/float(nperjob)))))


#    JOB='DYJetsToTauTau10-50-L%s_2015' % (sp)
#    JSONPATCH= (r"'{\"job\":{\"channels\":[\"et\",\"mt\",\"tt\"],\"sequences\":{\"em\":[],\"et\":[],\"mt\":[],\"tt\":[]},\"filelist\":\"%(FILELIST)s_DYJetsToLL10-50%(sp)s.dat\"}, \"sequence\":{\"output_name\":\"%(JOB)s\",\"ztautau_mode\":1,\"faked_tau_selector\":1}}' "%vars());
#    for i in range (0,int(math.ceil(float(nfiles)/float(nperjob)))) :
#      os.system('%(JOBWRAPPER)s "./bin/HTT --cfg=%(CONFIG)s --json=%(JSONPATCH)s --offset=%(i)d --nlines=%(nperjob)d &> jobs/%(JOB)s-%(i)d.log" jobs/%(JOB)s-%(i)s.sh' %vars())
#      os.system('%(JOBSUBMIT)s jobs/%(JOB)s-%(i)d.sh' % vars())
#    file_persamp.write("%s %d\n" %(JOB, int(math.ceil(float(nfiles)/float(nperjob)))))

              
    JOB='DYJetsToTauTau-JJ%s_2015' % (sp)
    JSONPATCH= (r"'{\"job\":{\"channels\":[\"et\",\"mt\",\"tt\"],\"sequences\":{\"em\":[],\"et\":[],\"mt\":[],\"tt\":[]},\"filelist\":\"%(FILELIST)s_DYJetsToLL%(sp)s.dat\"}, \"sequence\":{\"output_name\":\"%(JOB)s\",\"hadronic_tau_selector\":2,\"faked_tau_selector\":2, \"ztautau_mode\":1}}' "%vars());
    for i in range (0,int(math.ceil(float(nfiles)/float(nperjob)))) :
      os.system('%(JOBWRAPPER)s "./bin/HTT --cfg=%(CONFIG)s --json=%(JSONPATCH)s --offset=%(i)d --nlines=%(nperjob)d &> jobs/%(JOB)s-%(i)d.log" jobs/%(JOB)s-%(i)s.sh' %vars())
      os.system('%(JOBSUBMIT)s jobs/%(JOB)s-%(i)d.sh' % vars())
    file_persamp.write("%s %d\n" %(JOB, int(math.ceil(float(nfiles)/float(nperjob)))))
#
#
#             
#    JOB='DYJetsToTauTau10-50-JJ%s_2015' % (sp)
#    JSONPATCH= (r"'{\"job\":{\"channels\":[\"et\",\"mt\",\"tt\"],\"sequences\":{\"em\":[],\"et\":[],\"mt\":[],\"tt\":[]},\"filelist\":\"%(FILELIST)s_DYJetsToLL10-50%(sp)s.dat\"}, \"sequence\":{\"output_name\":\"%(JOB)s\",\"hadronic_tau_selector\":2,\"faked_tau_selector\":2, \"ztautau_mode\":1}}' "%vars());
#    for i in range (0,int(math.ceil(float(nfiles)/float(nperjob)))) :
#      os.system('%(JOBWRAPPER)s "./bin/HTT --cfg=%(CONFIG)s --json=%(JSONPATCH)s --offset=%(i)d --nlines=%(nperjob)d &> jobs/%(JOB)s-%(i)d.log" jobs/%(JOB)s-%(i)s.sh' %vars())
#      os.system('%(JOBSUBMIT)s jobs/%(JOB)s-%(i)d.sh' % vars())
#    file_persamp.write("%s %d\n" %(JOB, int(math.ceil(float(nfiles)/float(nperjob)))))
        
#
'''



'''

      if ch in ['et', 'mt']:
        JOB='WJetsToLNuSoup_%s_2012' % (ch)
        JSONPATCH= (r"'{\"job\":{\"channels\":[\"%(ch)s\"],\"filelist\":\"%(FILELIST)s_Special_5_WJetsToLNuSoup_%(ch)s_skim.dat\", \"file_prefix\":\"/vols/ssd00/cms/amd12/June6/MC_53X/\"}, \"sequence\":{\"output_name\":\"%(JOB)s\",\"special_mode\":5}}' "%vars());
        nfiles = sum(1 for line in open('%(FILELIST)s_Special_5_WJetsToLNuSoup_%(ch)s_skim.dat' % vars()))
        nperjob = 10
        for i in range (0,int(math.ceil(float(nfiles)/float(nperjob)))) :
          os.system('%(JOBWRAPPER)s "./bin/HTT --cfg=%(CONFIG)s --json=%(JSONPATCH)s --offset=%(i)d --nlines=%(nperjob)d &> jobs/Special_5_%(JOB)s-%(i)d.log" jobs/Special_5_%(JOB)s-%(i)s.sh' %vars())
          os.system('%(JOBSUBMIT)s jobs/Special_5_%(JOB)s-%(i)d.sh' % vars())
'''

if options.proc_sm or options.proc_mssm or options.proc_Hhh or options.proc_all:
  for sa in signal_mc:
    JOB='%s_2015' % (sa)
    JSONPATCH= (r"'{\"job\":{\"filelist\":\"%(FILELIST)s_%(sa)s.dat\"}, \"sequence\":{\"output_name\":\"%(JOB)s\"}}' "%vars());
    if scales:
      JSONPATCH= (r"'{\"job\":{\"filelist\":\"%(FILELIST)s_%(sa)s.dat\",\"sequences\":{\"em\":[\"scale_e_lo\",\"scale_e_hi\",\"scale_j_lo\",\"scale_j_hi\"],\"et\":[\"scale_t_lo\",\"scale_t_hi\",\"scale_j_lo\",\"scale_j_hi\"],\"mt\":[\"scale_t_lo\",\"scale_t_hi\",\"scale_j_lo\",\"scale_j_hi\"],\"tt\":[\"scale_t_lo\",\"scale_t_hi\",\"scale_j_lo\",\"scale_j_hi\"]}}, \"sequence\":{\"output_name\":\"%(JOB)s\"}}' "%vars());
    if os.path.exists('%(FILELIST)s_%(sa)s.dat' %vars()):
      nfiles = sum(1 for line in open('%(FILELIST)s_%(sa)s.dat' % vars()))
      nperjob = 50
      for i in range (0,int(math.ceil(float(nfiles)/float(nperjob)))) :
        os.system('%(JOBWRAPPER)s "./bin/HTT --cfg=%(CONFIG)s --json=%(JSONPATCH)s --offset=%(i)d --nlines=%(nperjob)d &> jobs/%(JOB)s-%(i)d.log" jobs/%(JOB)s-%(i)s.sh' %vars())
        os.system('%(JOBSUBMIT)s jobs/%(JOB)s-%(i)d.sh' % vars())
      file_persamp.write("%s %d\n" %(JOB, int(math.ceil(float(nfiles)/float(nperjob)))))
'''
      for sa in signal_vh:
        name_wh = sa.replace('WH_ZH_TTH','WH')
        name_tth = sa.replace('WH_ZH_TTH','TTH')
        name_zh = sa.replace('WH_ZH_TTH','ZH')
        JOB='%s_%s_2012' % (name_wh,ch)
        JSONPATCH= (r"'{\"job\":{\"channels\":[\"%(ch)s\"],\"filelist\":\"%(FILELIST)s_%(sa)s_%(ch)s_skim.dat\", \"file_prefix\":\"/vols/ssd00/cms/amd12/June6/MC_53X/\",\"sequences\":{\"%(ch)s\":[\"scale_t_lo\", \"scale_t_hi\"]}}, \"sequence\":{\"output_name\":\"%(JOB)s\", \"vh_filter_mode\":1}}' "%vars());
        nfiles = sum(1 for line in open('%(FILELIST)s_%(sa)s_%(ch)s_skim.dat' % vars()))
        nperjob = 10
        for i in range (0,int(math.ceil(float(nfiles)/float(nperjob)))) :
          os.system('%(JOBWRAPPER)s "./bin/HTT --cfg=%(CONFIG)s --json=%(JSONPATCH)s --offset=%(i)d --nlines=%(nperjob)d &> jobs/%(JOB)s-%(i)d.log" jobs/%(JOB)s-%(i)s.sh' %vars())
          os.system('%(JOBSUBMIT)s jobs/%(JOB)s-%(i)d.sh' % vars())

        JOB='%s_%s_2012' % (name_tth,ch)
        JSONPATCH= (r"'{\"job\":{\"channels\":[\"%(ch)s\"],\"filelist\":\"%(FILELIST)s_%(sa)s_%(ch)s_skim.dat\", \"file_prefix\":\"/vols/ssd00/cms/amd12/June6/MC_53X/\",\"sequences\":{\"%(ch)s\":[\"scale_t_lo\", \"scale_t_hi\"]}}, \"sequence\":{\"output_name\":\"%(JOB)s\", \"vh_filter_mode\":2}}' "%vars());
        nfiles = sum(1 for line in open('%(FILELIST)s_%(sa)s_%(ch)s_skim.dat' % vars()))
        nperjob = 10
        for i in range (0,int(math.ceil(float(nfiles)/float(nperjob)))) :
          os.system('%(JOBWRAPPER)s "./bin/HTT --cfg=scripts/config-ggH2012.json --json=%(JSONPATCH)s --offset=%(i)d --nlines=%(nperjob)d &> jobs/%(JOB)s-%(i)d.log" jobs/%(JOB)s-%(i)s.sh' %vars())
          os.system('%(JOBSUBMIT)s jobs/%(JOB)s-%(i)d.sh' % vars())

        JOB='%s_%s_2012' % (name_zh,ch)
        JSONPATCH= (r"'{\"job\":{\"channels\":[\"%(ch)s\"],\"filelist\":\"%(FILELIST)s_%(sa)s_%(ch)s_skim.dat\", \"file_prefix\":\"/vols/ssd00/cms/amd12/June6/MC_53X/\",\"sequences\":{\"%(ch)s\":[\"scale_t_lo\", \"scale_t_hi\"]}}, \"sequence\":{\"output_name\":\"%(JOB)s\", \"vh_filter_mode\":3}}' "%vars());
        nfiles = sum(1 for line in open('%(FILELIST)s_%(sa)s_%(ch)s_skim.dat' % vars()))
        nperjob = 10
        for i in range (0,int(math.ceil(float(nfiles)/float(nperjob)))) :
          os.system('%(JOBWRAPPER)s "./bin/HTT --cfg=scripts/config-ggH2012.json --json=%(JSONPATCH)s --offset=%(i)d --nlines=%(nperjob)d &> jobs/%(JOB)s-%(i)d.log" jobs/%(JOB)s-%(i)s.sh' %vars())
          os.system('%(JOBSUBMIT)s jobs/%(JOB)s-%(i)d.sh' % vars())


if options.proc_sm or options.proc_all:
    for ch in channels:
      for sa in signal_mc_ww:
        JOB='%s_%s_2012' % (sa,ch)
        JSONPATCH= (r"'{\"job\":{\"channels\":[\"%(ch)s\"],\"filelist\":\"%(FILELIST)s_%(sa)s_%(ch)s_skim.dat\", \"file_prefix\":\"/vols/ssd00/cms/amd12/June6/MC_53X/\",\"sequences\":{\"%(ch)s\":[\"scale_t_lo\", \"scale_t_hi\"]}}, \"sequence\":{\"output_name\":\"%(JOB)s\"}}' "%vars());
        nfiles = sum(1 for line in open('%(FILELIST)s_%(sa)s_%(ch)s_skim.dat' % vars()))
        nperjob = 10
        for i in range (0,int(math.ceil(float(nfiles)/float(nperjob)))) :
          os.system('%(JOBWRAPPER)s "./bin/HTT --cfg=scripts/config-ggH2012.json --json=%(JSONPATCH)s --offset=%(i)d --nlines=%(nperjob)d &> jobs/%(JOB)s-%(i)d.log" jobs/%(JOB)s-%(i)s.sh' %vars())
          os.system('%(JOBSUBMIT)s jobs/%(JOB)s-%(i)d.sh' % vars())
'''


