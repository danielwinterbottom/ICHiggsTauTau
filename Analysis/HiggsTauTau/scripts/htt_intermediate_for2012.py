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

SUPPORTED_ERAS_2012 = ['Paper', 'Moriond', 'HCP', 'DOnly']
SUPPORTED_ERAS_2011 = ['Paper', 'Total']
SUPPORTED_ERAS      = [ ] ## Will be assigned from 2011 or 2012

CHANNELS_2012 = ['et', 'mt', 'em', 'etmet', 'mtmet']
CHANNELS_2011 = ['et', 'mt', 'em']
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
                  help="A comma separted list of channels to process.  Supported channels: For 2102 %(CHANNELS_2012)s, for 2011 %(CHANNELS_2011)s" % vars())


(options, args) = parser.parse_args()
if options.wrapper: JOBWRAPPER=options.wrapper
if options.submit:  JOBSUBMIT=options.submit

channels = options.channels

if options.proc_data or options.proc_all:
  for ch in channels:
   JOB='Data_2012'
   FILELIST=('./filelists/June6_Data_53X_Data_Paper_%(ch)s_skim.dat' %vars())
   JSONPATCH= (r"'{\"job\":{\"channels\":[\"%(ch)s\"],\"filelist\":\"%(FILELIST)s\", \"file_prefix\":\"/vols/ssd00/cms/ajg06/June6/Data_53X/\"},\"sequence\":{\"output_name\":\"%(JOB)s\",\"is_data\":true}}' "%vars());
   nfiles = sum(1 for line in open('%(FILELIST)s' % vars()))
   nperjob = 10
   for i in range (0,int(math.ceil(float(nfiles)/float(nperjob)))) :
    os.system('%(JOBWRAPPER)s "./bin/HTT --cfg=scripts/config-ggH2012.json --json=%(JSONPATCH)s --offset=%(i)d --nlines=%(nperjob)d &> jobsnew/%(JOB)s-%(i)d.log" jobsnew/%(JOB)s-%(i)s.sh' %vars())
    os.system('%(JOBSUBMIT)s jobsnew/%(JOB)s-%(i)d.sh' % vars())

    if ch in ['et','mt']:
      JOB='Embedded_2012' 
      FILELIST=('./filelists/June6_Data_53X_Embedded_Paper_%(ch)s_skim.dat' %vars())
      JSONPATCH= (r"'{\"job\":{\"channels\":[\"%(ch)s\"],\"filelist\":\"%(FILELIST)s\", \"file_prefix\":\"/vols/ssd00/cms/ajg06/June6/Data_53X/\"},\"sequences\":{\"%(ch)s\":[\"scale_t_lo\", \"scale_t_hi\"]}, \"sequence\":{\"output_name\":\"%(JOB)s\",\"is_embedded\":true,\"is_data\":true, \"hadronic_tau_selector\":1}}' "%vars());
      nfiles = sum(1 for line in open('%(FILELIST)s' % vars()))
      nperjob = 10
      for i in range (0,int(math.ceil(float(nfiles)/float(nperjob)))) :
        os.system('%(JOBWRAPPER)s "./bin/HTT --cfg=scripts/config-ggH2012.json --json=%(JSONPATCH)s --offset=%(i)d --nlines=%(nperjob)d &> jobsnew/%(JOB)s-%(i)d.log" jobsnew/%(JOB)s-%(i)s.sh' %vars())
        os.system('%(JOBSUBMIT)s jobsnew/%(JOB)s-%(i)d.sh' % vars())

    if ch in ['et','mt']:
      JOB='Data_2012' 
      FILELIST=('./filelists/June6_Data_53X_Special_2_Data_Paper_%(ch)s_skim.dat' %vars())
      JSONPATCH= (r"'{\"job\":{\"channels\":[\"%(ch)s\"],\"filelist\":\"%(FILELIST)s\", \"file_prefix\":\"/vols/ssd00/cms/ajg06/June6/Data_53X/\"},\"sequence\":{\"output_name\":\"%(JOB)s\",\"is_data\":true, \"special_mode\":3}}' "%vars());
      nfiles = sum(1 for line in open('%(FILELIST)s' % vars()))
      nperjob = 10
      for i in range (0,int(math.ceil(float(nfiles)/float(nperjob)))) :
        os.system('%(JOBWRAPPER)s "./bin/HTT --cfg=scripts/config-ggH2012.json --json=%(JSONPATCH)s --offset=%(i)d --nlines=%(nperjob)d &> jobsnew/Special_3_%(JOB)s-%(i)d.log" jobsnew/Special_3_%(JOB)s-%(i)s.sh' %vars())
        os.system('%(JOBSUBMIT)s jobsnew/Special_3_%(JOB)s-%(i)d.sh' % vars())

    if ch in ['et','mt']:
      JOB='Data_2012' 
      FILELIST=('./filelists/June6_Data_53X_Special_4_Data_Paper_%(ch)s_skim.dat' %vars())
      JSONPATCH= (r"'{\"job\":{\"channels\":[\"%(ch)s\"],\"filelist\":\"%(FILELIST)s\", \"file_prefix\":\"/vols/ssd00/cms/ajg06/June6/Data_53X/\"},\"sequence\":{\"output_name\":\"%(JOB)s\",\"is_data\":true, \"special_mode\":4}}' "%vars());
      nfiles = sum(1 for line in open('%(FILELIST)s' % vars()))
      nperjob = 10
      for i in range (0,int(math.ceil(float(nfiles)/float(nperjob)))) :
        os.system('%(JOBWRAPPER)s "./bin/HTT --cfg=scripts/config-ggH2012.json --json=%(JSONPATCH)s --offset=%(i)d --nlines=%(nperjob)d &> jobsnew/Special_4_%(JOB)s-%(i)d.log" jobsnew/Special_4_%(JOB)s-%(i)s.sh' %vars())
        os.system('%(JOBSUBMIT)s jobsnew/Special_4_%(JOB)s-%(i)d.sh' % vars())
   
    if ch in ['em']:
      JOB='Data_2012' 
      FILELIST=('./filelists/June6_Data_53X_Special_20_Data_Paper_%(ch)s_skim.dat' %vars())
      JSONPATCH= (r"'{\"job\":{\"channels\":[\"%(ch)s\"],\"filelist\":\"%(FILELIST)s\", \"file_prefix\":\"/vols/ssd00/cms/ajg06/June6/Data_53X/\"},\"sequence\":{\"output_name\":\"%(JOB)s\",\"is_data\":true, \"special_mode\":20}}' "%vars());
      nfiles = sum(1 for line in open('%(FILELIST)s' % vars()))
      nperjob = 10
      for i in range (0,int(math.ceil(float(nfiles)/float(nperjob)))) :
        os.system('%(JOBWRAPPER)s "./bin/HTT --cfg=scripts/config-ggH2012.json --json=%(JSONPATCH)s --offset=%(i)d --nlines=%(nperjob)d &> jobsnew/Special_20_%(JOB)s-%(i)d.log" jobsnew/Special_20_%(JOB)s-%(i)s.sh' %vars())
        os.system('%(JOBSUBMIT)s jobsnew/Special_20_%(JOB)s-%(i)d.sh' % vars())

      JOB='Data_2012' 
      FILELIST=('./filelists/June6_Data_53X_Special_21_Data_Paper_%(ch)s_skim.dat' %vars())
      JSONPATCH= (r"'{\"job\":{\"channels\":[\"%(ch)s\"],\"filelist\":\"%(FILELIST)s\", \"file_prefix\":\"/vols/ssd00/cms/ajg06/June6/Data_53X/\"},\"sequence\":{\"output_name\":\"%(JOB)s\",\"is_data\":true, \"special_mode\":21}}' "%vars());
      nfiles = sum(1 for line in open('%(FILELIST)s' % vars()))
      nperjob = 10
      for i in range (0,int(math.ceil(float(nfiles)/float(nperjob)))) :
        os.system('%(JOBWRAPPER)s "./bin/HTT --cfg=scripts/config-ggH2012.json --json=%(JSONPATCH)s --offset=%(i)d --nlines=%(nperjob)d &> jobsnew/Special_21_%(JOB)s-%(i)d.log" jobsnew/Special_21_%(JOB)s-%(i)s.sh' %vars())
        os.system('%(JOBSUBMIT)s jobsnew/Special_21_%(JOB)s-%(i)d.sh' % vars())

      JOB='Data_2012' 
      FILELIST=('./filelists/June6_Data_53X_Special_22_Data_Paper_%(ch)s_skim.dat' %vars())
      JSONPATCH= (r"'{\"job\":{\"channels\":[\"%(ch)s\"],\"filelist\":\"%(FILELIST)s\", \"file_prefix\":\"/vols/ssd00/cms/ajg06/June6/Data_53X/\"},\"sequence\":{\"output_name\":\"%(JOB)s\",\"is_data\":true, \"special_mode\":22}}' "%vars());
      nfiles = sum(1 for line in open('%(FILELIST)s' % vars()))
      nperjob = 10
      for i in range (0,int(math.ceil(float(nfiles)/float(nperjob)))) :
        os.system('%(JOBWRAPPER)s "./bin/HTT --cfg=scripts/config-ggH2012.json --json=%(JSONPATCH)s --offset=%(i)d --nlines=%(nperjob)d &> jobsnew/Special_22_%(JOB)s-%(i)d.log" jobsnew/Special_22_%(JOB)s-%(i)s.sh' %vars())
        os.system('%(JOBSUBMIT)s jobsnew/Special_22_%(JOB)s-%(i)d.sh' % vars())

      JOB='Data_2012' 
      FILELIST=('./filelists/June6_Data_53X_Special_23_Data_Paper_%(ch)s_skim.dat' %vars())
      JSONPATCH= (r"'{\"job\":{\"channels\":[\"%(ch)s\"],\"filelist\":\"%(FILELIST)s\", \"file_prefix\":\"/vols/ssd00/cms/ajg06/June6/Data_53X/\"},\"sequence\":{\"output_name\":\"%(JOB)s\",\"is_data\":true, \"special_mode\":23}}' "%vars());
      nfiles = sum(1 for line in open('%(FILELIST)s' % vars()))
      nperjob = 10
      for i in range (0,int(math.ceil(float(nfiles)/float(nperjob)))) :
        os.system('%(JOBWRAPPER)s "./bin/HTT --cfg=scripts/config-ggH2012.json --json=%(JSONPATCH)s --offset=%(i)d --nlines=%(nperjob)d &> jobsnew/Special_23_%(JOB)s-%(i)d.log" jobsnew/Special_23_%(JOB)s-%(i)s.sh' %vars())
        os.system('%(JOBSUBMIT)s jobsnew/Special_23_%(JOB)s-%(i)d.sh' % vars())

      JOB='Data_2012' 
      FILELIST=('./filelists/June6_Data_53X_Special_24_Data_Paper_%(ch)s_skim.dat' %vars())
      JSONPATCH= (r"'{\"job\":{\"channels\":[\"%(ch)s\"],\"filelist\":\"%(FILELIST)s\", \"file_prefix\":\"/vols/ssd00/cms/ajg06/June6/Data_53X/\"},\"sequence\":{\"output_name\":\"%(JOB)s\",\"is_data\":true, \"special_mode\":24}}' "%vars());
      nfiles = sum(1 for line in open('%(FILELIST)s' % vars()))
      nperjob = 10
      for i in range (0,int(math.ceil(float(nfiles)/float(nperjob)))) :
        os.system('%(JOBWRAPPER)s "./bin/HTT --cfg=scripts/config-ggH2012.json --json=%(JSONPATCH)s --offset=%(i)d --nlines=%(nperjob)d &> jobsnew/Special_24_%(JOB)s-%(i)d.log" jobsnew/Special_24_%(JOB)s-%(i)s.sh' %vars())
        os.system('%(JOBSUBMIT)s jobsnew/Special_24_%(JOB)s-%(i)d.sh' % vars())

      JOB='Embedded_2012' 
      FILELIST=('./filelists/June6_Data_53X_Embedded_Paper_%(ch)s_skim.dat' %vars())
      JSONPATCH= (r"'{\"job\":{\"channels\":[\"%(ch)s\"], \"sequences\":{\"%(ch)s\":[\"scale_m_lo\",\"scale_m_hi\"]},\"filelist\":\"%(FILELIST)s\", \"file_prefix\":\"/vols/ssd00/cms/ajg06/June6/Data_53X/\"},\"sequence\":{\"output_name\":\"%(JOB)s\",\"is_data\":true, \"is_embedded\":true, \"baseline\":{\"mass_scale_mode\":true}}}' "%vars());
      nfiles = sum(1 for line in open('%(FILELIST)s' % vars()))
      nperjob = 10
      for i in range (0,int(math.ceil(float(nfiles)/float(nperjob)))) :
        os.system('%(JOBWRAPPER)s "./bin/HTT --cfg=scripts/config-ggH2012.json --json=%(JSONPATCH)s --offset=%(i)d --nlines=%(nperjob)d &> jobsnew/%(JOB)s-%(i)d.log" jobsnew/%(JOB)s-%(i)s.sh' %vars())
        os.system('%(JOBSUBMIT)s jobsnew/%(JOB)s-%(i)d.sh' % vars())

FILELIST='filelists/June6_MC_53X'

signal_mc = [ ]
signal_vh = [ ] 
signal_mc_ww = [ ]

if options.proc_sm or options.proc_all:
  masses = ['90','95','100','105','110','115','120','125','130','135','140','145','150','155','160']
  if options.short_signal: masses = ['125']
  for mass in masses :
    signal_mc += [
      'GluGluToHToTauTau_M-'+mass,
      'VBF_HToTauTau_M-'+mass, 
      'WH_ZH_TTH_HToTauTau_M-'+mass
    ]
    signal_vh += [
      'WH_ZH_TTH_HToTauTau_M-'+mass
    ]
  ww_masses = ['110','115','120','125','130','135','140','145','150','155','160']
  if options.short_signal: ww_masses = ['125']
  for ww_mass in ww_masses :
    signal_mc_ww += [
      'GluGluToHToWWTo2LAndTau2Nu_M-'+ww_mass,
      'VBF_HToWWTo2LAndTau2Nu_M-'+ww_mass, 
    ]
if options.proc_mssm or options.proc_all:
  masses = ['80','90','100','120','110','130','140','160','180','200','250','300','350','400','450','500','600','700','800','900','1000']
#  if not options.do_2011: masses += ['80','110']
  if options.short_signal: masses = ['160']
  for mass in masses :
    signal_mc += [
      'SUSYGluGluToHToTauTau_M-'+mass,
      'SUSYBBHToTauTau_M-'+mass
    ]

if options.proc_Hhh or options.proc_all:
  Hmasses = ['260','270','280','290','300','310','320','330','340','350']
  if options.short_signal: Hmasses = ['300']
  for Hmass in Hmasses : 
    signal_mc += [
      'GluGluToHTohhTo2Tau2B_mH-'+Hmass
    ]
  #Add SM 125 signal H->tautau/bb for H->hh analysis
  signal_mc += [
    'GluGluToHToTauTau_M-125',
    'VBF_HToTauTau_M-125', 
    'WH_ZH_TTH_HToTauTau_M-125',
    'WH_WToLNu_HToBB_M-125',
    'ZH_ZToLL_HToBB_M-125'
  ]
  signal_vh += [
    'WH_ZH_TTH_HToTauTau_M-125'
  ]
  if options.extra_signal_profile:
    Amasses = ['250','260','270','280','290','300','310','320','330','340','350']
    ATauTaumasses = ['260','270','280','290','300','310','320','330','340','350']
    bbHmasses = ['90','100','110','120','130','140','160','180','200','250','300','350','400']
    if options.short_signal: Amasses = ['300']
    if options.short_signal: ATauTaumasses = ['300']
    if options.short_signal: bbHmasses = ['120','300']
    for Hmass in Hmasses : 
      signal_mc += [
        'SUSYBBHTohhTo2Tau2B_mH-'+Hmass
      ]
    for Amass in Amasses :
      signal_mc += [
        'GluGluToAToZhToLLBB_mA-'+Amass
      ]
    for Amass in ATauTaumasses :
      signal_mc += [
        'GluGluToAToZhToLLTauTau_mA-'+Amass
        #'GluGluToAToZhToBBTauTau_mA-'+Amass
      ]
    for Hmass in bbHmasses : 
      signal_mc += [
        'SUSYBBHToTauTau_M-'+Hmass
      ]

if options.proc_bkg or options.proc_all:
    for ch in channels:
      if ch in ['et', 'mt', 'etmet', 'mtmet']:
        central_samples = [
          'WJetsToLNuSoup',
          'TTJetsFullLept',
          'TTJetsSemiLept',
          'TTJetsHadronicExt',
          'WWJetsTo2L2Nu',
          'WZJetsTo2L2Q',
          'WZJetsTo3LNu',
          'ZZJetsTo2L2Nu',
          'ZZJetsTo2L2Q',
          'ZZJetsTo4L',
          'T-tW',
          'Tbar-tW'
        ]

      if ch in ['em']:
        central_samples = [
          'TTJetsFullLept',
          'TTJetsSemiLept',
          'TTJetsHadronicExt',
          'WWJetsTo2L2Nu',
          'WZJetsTo2L2Q',
          'WZJetsTo3LNu',
          'ZZJetsTo2L2Nu',
          'ZZJetsTo2L2Q',
          'ZZJetsTo4L',
          'T-tW',
          'Tbar-tW'
        ]
      
#      if PRODUCTION=='June6' and (not options.do_2011):
#        central_samples.remove('TTJets')
#        central_samples += [
#          'TTJetsFullLept',
#          'TTJetsSemiLept',
#          'TTJetsHadronicExt'
#        ]
      if ch in ['et', 'mt']:
        soups = ['Soup']
        for sp in soups:  
          JOB='DYJetsToTauTau%s_2012' % (sp)
          JSONPATCH= (r"'{\"job\":{\"channels\":[\"%(ch)s\"], \"sequences\":{\"%(ch)s\":[\"scale_t_lo\",\"scale_t_hi\"]},\"filelist\":\"%(FILELIST)s_DYJetsToLL%(sp)s_%(ch)s_skim.dat\"}, \"sequence\":{\"output_name\":\"%(JOB)s\", \"hadronic_tau_selector\":1,\"faked_tau_selector\":2,\"ztautau_mode\":1}}' "%vars());
          nfiles = sum(1 for line in open('%(FILELIST)s_DYJetsToLL%(sp)s_%(ch)s_skim.dat' % vars()))
          nperjob = 10
          for i in range (0,int(math.ceil(float(nfiles)/float(nperjob)))) :
            os.system('%(JOBWRAPPER)s "./bin/HTT --cfg=scripts/config-ggH2012.json --json=%(JSONPATCH)s --offset=%(i)d --nlines=%(nperjob)d &> jobsnew/%(JOB)s-%(i)d.log" jobsnew/%(JOB)s-%(i)s.sh' %vars())
            os.system('%(JOBSUBMIT)s jobsnew/%(JOB)s-%(i)d.sh' % vars())


          JOB='DYJetsToLL%s_2012' % (sp)
          JSONPATCH= (r"'{\"job\":{\"channels\":[\"%(ch)s\"],\"filelist\":\"%(FILELIST)s_DYJetsToLL%(sp)s_%(ch)s_skim.dat\"}, \"sequence\":{\"output_name\":\"%(JOB)s\",\"ztautau_mode\":2}}' "%vars());
          nfiles = sum(1 for line in open('%(FILELIST)s_DYJetsToLL%(sp)s_%(ch)s_skim.dat' % vars()))
          nperjob = 10
          for i in range (0,int(math.ceil(float(nfiles)/float(nperjob)))) :
            os.system('%(JOBWRAPPER)s "./bin/HTT --cfg=scripts/config-ggH2012.json --json=%(JSONPATCH)s --offset=%(i)d --nlines=%(nperjob)d &> jobsnew/%(JOB)s-%(i)d.log" jobsnew/%(JOB)s-%(i)s.sh' %vars())
            os.system('%(JOBSUBMIT)s jobsnew/%(JOB)s-%(i)d.sh' % vars())

          JOB='DYJetsToLL-L%s_2012' % (sp)
          JSONPATCH= (r"'{\"job\":{\"channels\":[\"%(ch)s\"],\"filelist\":\"%(FILELIST)s_DYJetsToLL%(sp)s_%(ch)s_skim.dat\"}, \"sequence\":{\"output_name\":\"%(JOB)s\",\"ztautau_mode\":2,\"faked_tau_selector\":1}}' "%vars());
          nfiles = sum(1 for line in open('%(FILELIST)s_DYJetsToLL%(sp)s_%(ch)s_skim.dat' % vars()))
          nperjob = 10
          for i in range (0,int(math.ceil(float(nfiles)/float(nperjob)))) :
            os.system('%(JOBWRAPPER)s "./bin/HTT --cfg=scripts/config-ggH2012.json --json=%(JSONPATCH)s --offset=%(i)d --nlines=%(nperjob)d &> jobsnew/%(JOB)s-%(i)d.log" jobsnew/%(JOB)s-%(i)s.sh' %vars())
            os.system('%(JOBSUBMIT)s jobsnew/%(JOB)s-%(i)d.sh' % vars())


          JOB='DYJetsToLL-J%s_2012' % (sp)
          JSONPATCH= (r"'{\"job\":{\"channels\":[\"%(ch)s\"],\"filelist\":\"%(FILELIST)s_DYJetsToLL%(sp)s_%(ch)s_skim.dat\"}, \"sequence\":{\"output_name\":\"%(JOB)s\",\"ztautau_mode\":2,\"faked_tau_selector\":2}}' "%vars());
          nfiles = sum(1 for line in open('%(FILELIST)s_DYJetsToLL%(sp)s_%(ch)s_skim.dat' % vars()))
          nperjob = 10
          for i in range (0,int(math.ceil(float(nfiles)/float(nperjob)))) :
            os.system('%(JOBWRAPPER)s "./bin/HTT --cfg=scripts/config-ggH2012.json --json=%(JSONPATCH)s --offset=%(i)d --nlines=%(nperjob)d &> jobsnew/%(JOB)s-%(i)d.log" jobsnew/%(JOB)s-%(i)s.sh' %vars())
            os.system('%(JOBSUBMIT)s jobsnew/%(JOB)s-%(i)d.sh' % vars())


          JOB='DYJetsToTauTau-L%s_2012' % (sp)
          JSONPATCH= (r"'{\"job\":{\"channels\":[\"%(ch)s\"],\"filelist\":\"%(FILELIST)s_DYJetsToLL%(sp)s_%(ch)s_skim.dat\"}, \"sequence\":{\"output_name\":\"%(JOB)s\",\"ztautau_mode\":1,\"faked_tau_selector\":1}}' "%vars());
          nfiles = sum(1 for line in open('%(FILELIST)s_DYJetsToLL%(sp)s_%(ch)s_skim.dat' % vars()))
          nperjob = 10
          for i in range (0,int(math.ceil(float(nfiles)/float(nperjob)))) :
            os.system('%(JOBWRAPPER)s "./bin/HTT --cfg=scripts/config-ggH2012.json --json=%(JSONPATCH)s --offset=%(i)d --nlines=%(nperjob)d &> jobsnew/%(JOB)s-%(i)d.log" jobsnew/%(JOB)s-%(i)s.sh' %vars())
            os.system('%(JOBSUBMIT)s jobsnew/%(JOB)s-%(i)d.sh' % vars())

              
          JOB='DYJetsToTauTau-JJ%s_2012' % (sp)
          JSONPATCH= (r"'{\"job\":{\"channels\":[\"%(ch)s\"],\"filelist\":\"%(FILELIST)s_DYJetsToLL%(sp)s_%(ch)s_skim.dat\"}, \"sequence\":{\"output_name\":\"%(JOB)s\",\"hadronic_tau_selector\":2,\"faked_tau_selector\":2, \"ztautau_mode\":1}}' "%vars());
          nfiles = sum(1 for line in open('%(FILELIST)s_DYJetsToLL%(sp)s_%(ch)s_skim.dat' % vars()))
          nperjob = 10
          for i in range (0,int(math.ceil(float(nfiles)/float(nperjob)))) :
            os.system('%(JOBWRAPPER)s "./bin/HTT --cfg=scripts/config-ggH2012.json --json=%(JSONPATCH)s --offset=%(i)d --nlines=%(nperjob)d &> jobsnew/%(JOB)s-%(i)d.log" jobsnew/%(JOB)s-%(i)s.sh' %vars())
            os.system('%(JOBSUBMIT)s jobsnew/%(JOB)s-%(i)d.sh' % vars())
        
      if ch in ['em']:
        soups = ['', 'Soup']
        for sp in soups: 
          JOB='DYJetsToTauTau%s_2012' % (sp)
          JSONPATCH= (r"'{\"job\":{\"channels\":[\"%(ch)s\"],\"filelist\":\"%(FILELIST)s_DYJetsToLL%(sp)s_%(ch)s_skim.dat\"}, \"sequence\":{\"output_name\":\"%(JOB)s\", \"ztautau_mode\":1}}' "%vars());
          nfiles = sum(1 for line in open('%(FILELIST)s_DYJetsToLL%(sp)s_%(ch)s_skim.dat' % vars()))
          nperjob = 10
          for i in range (0,int(math.ceil(float(nfiles)/float(nperjob)))) :
            os.system('%(JOBWRAPPER)s "./bin/HTT --cfg=scripts/config-ggH2012.json --json=%(JSONPATCH)s --offset=%(i)d --nlines=%(nperjob)d &> jobsnew/%(JOB)s-%(i)d.log" jobsnew/%(JOB)s-%(i)s.sh' %vars())
            os.system('%(JOBSUBMIT)s jobsnew/%(JOB)s-%(i)d.sh' % vars())

          JOB='DYJetsToLL%s_2012' % (sp)
          JSONPATCH= (r"'{\"job\":{\"channels\":[\"%(ch)s\"],\"filelist\":\"%(FILELIST)s_DYJetsToLL%(sp)s_%(ch)s_skim.dat\"}, \"sequence\":{\"output_name\":\"%(JOB)s\", \"ztautau_mode\":2}}' "%vars());
          nfiles = sum(1 for line in open('%(FILELIST)s_DYJetsToLL%(sp)s_%(ch)s_skim.dat' % vars()))
          nperjob = 10
          for i in range (0,int(math.ceil(float(nfiles)/float(nperjob)))) :
            os.system('%(JOBWRAPPER)s "./bin/HTT --cfg=scripts/config-ggH2012.json --json=%(JSONPATCH)s --offset=%(i)d --nlines=%(nperjob)d &> jobsnew/%(JOB)s-%(i)d.log" jobsnew/%(JOB)s-%(i)s.sh' %vars())
            os.system('%(JOBSUBMIT)s jobsnew/%(JOB)s-%(i)d.sh' % vars())

      for sa in central_samples:
        JOB='%s_2012' % (sa)
        JSONPATCH= (r"'{\"job\":{\"channels\":[\"%(ch)s\"],\"filelist\":\"%(FILELIST)s_%(sa)s_%(ch)s_skim.dat\"}, \"sequence\":{\"output_name\":\"%(JOB)s\"}}' "%vars());
        nfiles = sum(1 for line in open('%(FILELIST)s_%(sa)s_%(ch)s_skim.dat' % vars()))
        nperjob = 10
        for i in range (0,int(math.ceil(float(nfiles)/float(nperjob)))) :
          os.system('%(JOBWRAPPER)s "./bin/HTT --cfg=scripts/config-ggH2012.json --json=%(JSONPATCH)s --offset=%(i)d --nlines=%(nperjob)d &> jobsnew/%(JOB)s-%(i)d.log" jobsnew/%(JOB)s-%(i)s.sh' %vars())
          os.system('%(JOBSUBMIT)s jobsnew/%(JOB)s-%(i)d.sh' % vars())

      if ch in ['et', 'mt']:
        JOB='WJetsToLNuSoup_2012' 
        JSONPATCH= (r"'{\"job\":{\"channels\":[\"%(ch)s\"],\"filelist\":\"%(FILELIST)s_Special_5_WJetsToLNuSoup_%(ch)s_skim.dat\"}, \"sequence\":{\"output_name\":\"%(JOB)s\",\"special_mode\":5}}' "%vars());
        nfiles = sum(1 for line in open('%(FILELIST)s_Special_5_WJetsToLNuSoup_%(ch)s_skim.dat' % vars()))
        nperjob = 10
        for i in range (0,int(math.ceil(float(nfiles)/float(nperjob)))) :
          os.system('%(JOBWRAPPER)s "./bin/HTT --cfg=scripts/config-ggH2012.json --json=%(JSONPATCH)s --offset=%(i)d --nlines=%(nperjob)d &> jobsnew/Special_5_%(JOB)s-%(i)d.log" jobsnew/Special_5_%(JOB)s-%(i)s.sh' %vars())
          os.system('%(JOBSUBMIT)s jobsnew/Special_5_%(JOB)s-%(i)d.sh' % vars())


if options.proc_sm or options.proc_mssm or options.proc_Hhh or options.proc_all:
    for ch in channels:
      for sa in signal_mc:
        JOB='%s_2012' % (sa)
        JSONPATCH= (r"'{\"job\":{\"channels\":[\"%(ch)s\"],\"filelist\":\"%(FILELIST)s_%(sa)s_%(ch)s_skim.dat\"}, \"sequences\":{\"%(ch)s\":[\"scale_t_lo\", \"scale_t_hi\"]}, \"sequence\":{\"output_name\":\"%(JOB)s\"}}' "%vars());
        nfiles = sum(1 for line in open('%(FILELIST)s_%(sa)s_%(ch)s_skim.dat' % vars()))
        nperjob = 10
        for i in range (0,int(math.ceil(float(nfiles)/float(nperjob)))) :
          os.system('%(JOBWRAPPER)s "./bin/HTT --cfg=scripts/config-ggH2012.json --json=%(JSONPATCH)s --offset=%(i)d --nlines=%(nperjob)d &> jobsnew/%(JOB)s-%(i)d.log" jobsnew/%(JOB)s-%(i)s.sh' %vars())
          os.system('%(JOBSUBMIT)s jobsnew/%(JOB)s-%(i)d.sh' % vars())

      for sa in signal_vh:
        name_wh = sa.replace('WH_ZH_TTH','WH')
        name_tth = sa.replace('WH_ZH_TTH','TTH')
        name_zh = sa.replace('WH_ZH_TTH','ZH')
        JOB='%s_2012' % (name_wh)
        JSONPATCH= (r"'{\"job\":{\"channels\":[\"%(ch)s\"],\"filelist\":\"%(FILELIST)s_%(sa)s_%(ch)s_skim.dat\"},\"sequences\":{\"%(ch)s\":[\"scale_t_lo\", \"scale_t_hi\"]}, \"sequence\":{\"output_name\":\"%(JOB)s\", \"vh_filter_mode\":1}}' "%vars());
        nfiles = sum(1 for line in open('%(FILELIST)s_%(sa)s_%(ch)s_skim.dat' % vars()))
        nperjob = 10
        for i in range (0,int(math.ceil(float(nfiles)/float(nperjob)))) :
          os.system('%(JOBWRAPPER)s "./bin/HTT --cfg=scripts/config-ggH2012.json --json=%(JSONPATCH)s --offset=%(i)d --nlines=%(nperjob)d &> jobsnew/%(JOB)s-%(i)d.log" jobsnew/%(JOB)s-%(i)s.sh' %vars())
          os.system('%(JOBSUBMIT)s jobsnew/%(JOB)s-%(i)d.sh' % vars())

        JOB='%s_2012' % (name_tth)
        JSONPATCH= (r"'{\"job\":{\"channels\":[\"%(ch)s\"],\"filelist\":\"%(FILELIST)s_%(sa)s_%(ch)s_skim.dat\"}, \"sequences\":{\"%(ch)s\":[\"scale_t_lo\", \"scale_t_hi\"]}, \"sequence\":{\"output_name\":\"%(JOB)s\", \"vh_filter_mode\":2}}' "%vars());
        nfiles = sum(1 for line in open('%(FILELIST)s_%(sa)s_%(ch)s_skim.dat' % vars()))
        nperjob = 10
        for i in range (0,int(math.ceil(float(nfiles)/float(nperjob)))) :
          os.system('%(JOBWRAPPER)s "./bin/HTT --cfg=scripts/config-ggH2012.json --json=%(JSONPATCH)s --offset=%(i)d --nlines=%(nperjob)d &> jobsnew/%(JOB)s-%(i)d.log" jobsnew/%(JOB)s-%(i)s.sh' %vars())
          os.system('%(JOBSUBMIT)s jobsnew/%(JOB)s-%(i)d.sh' % vars())

        JOB='%s_2012' % (name_zh)
        JSONPATCH= (r"'{\"job\":{\"channels\":[\"%(ch)s\"],\"filelist\":\"%(FILELIST)s_%(sa)s_%(ch)s_skim.dat\"},\"sequences\":{\"%(ch)s\":[\"scale_t_lo\", \"scale_t_hi\"]}, \"sequence\":{\"output_name\":\"%(JOB)s\", \"vh_filter_mode\":3}}' "%vars());
        nfiles = sum(1 for line in open('%(FILELIST)s_%(sa)s_%(ch)s_skim.dat' % vars()))
        nperjob = 10
        for i in range (0,int(math.ceil(float(nfiles)/float(nperjob)))) :
          os.system('%(JOBWRAPPER)s "./bin/HTT --cfg=scripts/config-ggH2012.json --json=%(JSONPATCH)s --offset=%(i)d --nlines=%(nperjob)d &> jobsnew/%(JOB)s-%(i)d.log" jobsnew/%(JOB)s-%(i)s.sh' %vars())
          os.system('%(JOBSUBMIT)s jobsnew/%(JOB)s-%(i)d.sh' % vars())


if options.proc_sm or options.proc_all:
    for ch in channels:
      for sa in signal_mc_ww:
        JOB='%s_2012' % (sa)
        JSONPATCH= (r"'{\"job\":{\"channels\":[\"%(ch)s\"],\"filelist\":\"%(FILELIST)s_%(sa)s_%(ch)s_skim.dat\"},\"sequences\":{\"%(ch)s\":[\"scale_t_lo\", \"scale_t_hi\"]}, \"sequence\":{\"output_name\":\"%(JOB)s\"}}' "%vars());
        nfiles = sum(1 for line in open('%(FILELIST)s_%(sa)s_%(ch)s_skim.dat' % vars()))
        nperjob = 10
        for i in range (0,int(math.ceil(float(nfiles)/float(nperjob)))) :
          os.system('%(JOBWRAPPER)s "./bin/HTT --cfg=scripts/config-ggH2012.json --json=%(JSONPATCH)s --offset=%(i)d --nlines=%(nperjob)d &> jobsnew/%(JOB)s-%(i)d.log" jobsnew/%(JOB)s-%(i)s.sh' %vars())
          os.system('%(JOBSUBMIT)s jobsnew/%(JOB)s-%(i)d.sh' % vars())


