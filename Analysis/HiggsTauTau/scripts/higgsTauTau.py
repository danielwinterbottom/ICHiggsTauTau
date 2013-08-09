#!/usr/bin/env python

import sys
from optparse import OptionParser
import os

## Set up the job wrapper and job submit options, default first then
## override if the environment variable is set
## Set up in the input path prefix, i.e. eos, dcache, ssd, local
JOBWRAPPER      = './scripts/generate_job.sh'
JOBSUBMIT       = 'true'
PREFIXOVERRIDE  = 'root://eoscms//eos/cms/store/user/agilbert/Skims/'
if "JOBWRAPPER" in os.environ:      JOBWRAPPER      = os.environ["JOBWRAPPER"]
if "JOBSUBMIT"  in os.environ:      JOBSUBMIT       = os.environ["JOBSUBMIT"]
if "PREFIXOVERRIDE" in os.environ:  PREFIXOVERRIDE  = os.environ["PREFIXOVERRIDE"]  
print "Using job-wrapper:    " + JOBWRAPPER
print "Using job-submission: " + JOBSUBMIT
print "Using input prefix:   " + PREFIXOVERRIDE


SUPPORTED_ERAS_2012 = ['Paper', 'Moriond', 'HCP', 'DOnly']
SUPPORTED_ERAS_2011 = ['Paper', 'Total']
SUPPORTED_ERAS      = [ ] ## Will be assigned from 2011 or 2012

CHANNELS_2012 = ['et', 'mt', 'em', 'etmet', 'mtmet']
CHANNELS_2011 = ['et', 'mt', 'em']
CHANNELS      = [ ] ## will be assigned from 2011 or 2012

SCALES = ['0', '1', '2']

PRODUCTION_2012 = 'Dec30'
PRODUCTION_2011 = 'Sept11'
PRODUCTION = ''


def validate_channel(channel):
  assert channel in CHANNELS, 'Error, channel %(channel)s duplicated or unrecognised' % vars()
  CHANNELS.remove(channel)

def validate_scales(scale):
  assert scale in SCALES, 'Error, scale %(scale)s duplicated or unrecognised' % vars()
  SCALES.remove(scale)

def split_callback(option, opt, value, parser):
  setattr(parser.values, option.dest, value.split(','))


parser = OptionParser()
parser.add_option("--wrapper", dest="wrapper",
                  help="Specify the job-wrapper script. The current wrapper is '%(JOBWRAPPER)s'."
                  " Using the --wrapper option overrides both the default and the environment variable. " % vars())

parser.add_option("--submit", dest="submit",
                  help="Specify the job-submission method. The current method is '%(JOBSUBMIT)s'"
                  " Using the --submit option overrides both the default and the environment variable. " % vars())

parser.add_option("-p", "--production", dest="production",
                  help="Specify the ntuple production to use.  The defaults are '%(PRODUCTION_2012)s' for 2012 and '%(PRODUCTION_2011)s' for 2011" % vars())

parser.add_option("-i","--input_prefix", dest="input",
                  help="Specify the path prefix for the skimmed ntuple input. The current prefix is '%(PREFIXOVERRIDE)s'"
                  " Using this option overrides both the default and the environment variable. " % vars())

parser.add_option("-e", "--era", dest="era",
                  help="Specify a data-taking era to process. This option both sets the config files that will be used, "
                  "e.g. scripts/ERA_2012.cfg, and selects the appropriate data filelists. "
                  "The following options are supported by default: For 2012 %(SUPPORTED_ERAS_2012)s, for 2011 %(SUPPORTED_ERAS_2011)s" % vars(), metavar="ERA")

parser.add_option("--data", dest="proc_data", action='store_true', default=False,
                  help="Process data samples (including embedded)")

parser.add_option("--bkg", dest="proc_bkg", action='store_true', default=False,
                  help="Process background mc samples")

parser.add_option("--sm", dest="proc_sm", action='store_true', default=False,
                  help="Process signal SM mc samples")

parser.add_option("--mssm", dest="proc_mssm", action='store_true', default=False,
                  help="Process signal MSSM mc samples")

parser.add_option("--all", dest="proc_all", action='store_true', default=False,
                  help="Process all samples")

parser.add_option("--2011", dest="do_2011", action='store_true', default=False,
                  help="Process the 2011 analysis, otherwise the 2012 analysis is processed by default.")

parser.add_option("--short_signal", dest="short_signal", action='store_true', default=False,
                  help="Only process the 125/160 signal samples")

parser.add_option("-c", "--channels", dest="channels", type='string', action='callback',callback=split_callback,
                  help="A comma separted list of channels to process.  Supported channels: For 2102 %(CHANNELS_2012)s, for 2011 %(CHANNELS_2011)s" % vars())

parser.add_option("-s", "--energyscales", dest="scales", type='string', action='callback',callback=split_callback,
                  help="A comma separted list of energy/mass scale shifts.  Supports %(SCALES)s where 0 is central, 1 is down and 2 is up." % vars())

(options, args) = parser.parse_args()


if options.wrapper: JOBWRAPPER=options.wrapper
if options.submit:  JOBSUBMIT=options.submit
if options.input:   PREFIXOVERRIDE=options.input
channels = options.channels
scales =options.scales
ERA = options.era

SUPPORTED_ERAS = SUPPORTED_ERAS_2012
CHANNELS = CHANNELS_2012
PRODUCTION = PRODUCTION_2012 if options.production == None else options.production
PREFIXDATA="--is_data=1 --input_prefix="+PREFIXOVERRIDE+"/"+PRODUCTION+"/Data_53X/"
PREFIXMC="--is_data=0 --input_prefix="+PREFIXOVERRIDE+"/"+PRODUCTION+"/MC_53X/"
YR='2012'
if options.do_2011:
  SUPPORTED_ERAS = SUPPORTED_ERAS_2011
  CHANNELS = CHANNELS_2011
  PRODUCTION = PRODUCTION_2011 if options.production == None else options.production
  PREFIXDATA="--is_data=1 --input_prefix="+PREFIXOVERRIDE+"/"+PRODUCTION+"/Data_42X/"
  PREFIXMC="--is_data=0 --input_prefix="+PREFIXOVERRIDE+"/"+PRODUCTION+"/MC_42X/"
  YR='2011'

### Do some validation of the input
helpMsg = "Run 'higgsTauTau.py -h' for help."
if not (options.proc_data or options.proc_bkg or options.proc_sm or options.proc_mssm or options.proc_all):
  print 'Error, must run script with a least one of --data, --bkg, --sm, --mssm, --all. ' + helpMsg
  sys.exit(1)
if not channels:
  print 'Error, no channels specified. ' + helpMsg
  sys.exit(1)
if not scales:
  print 'Error, no energy scales specified. ' + helpMsg
  sys.exit(1)
if not ERA:
  print 'Error, no data-taking era specified. ' + helpMsg
  sys.exit(1)
if not (ERA in SUPPORTED_ERAS):
  print 'Warning, specified era not supported by default. ' + helpMsg
for channel in channels:
  validate_channel(channel)
for scale in scales:
  validate_scales(scale)


CONFIG = 'scripts/%s_%s.cfg' % (ERA, YR)
print 'Using config: %s'% CONFIG

FILELIST='filelists/'+PRODUCTION+'_Data_53X'
if options.do_2011: FILELIST='filelists/'+PRODUCTION+'_Data_42X'

if options.proc_data or options.proc_all:
  for ch in channels:
    if '0' in scales:
      JOB='Data_%s_%s' % (ch,YR)
      os.system('%(JOBWRAPPER)s "./bin/HiggsTauTau --cfg=%(CONFIG)s %(PREFIXDATA)s --filelist=%(FILELIST)s_Data_%(ERA)s_%(ch)s_skim.dat --channel=%(ch)s --output_name=%(JOB)s.root &> jobs/%(JOB)s.log" jobs/%(JOB)s.sh' % vars())
      os.system('%(JOBSUBMIT)s jobs/%(JOB)s.sh' % vars())

    if ch in ['et', 'mt', 'mtmet']:
      for sc in scales:
        JOB='Embedded_%s_%s' % (ch,YR)
        os.system('%(JOBWRAPPER)s "./bin/HiggsTauTau --cfg=%(CONFIG)s %(PREFIXDATA)s --tau_scale_mode=%(sc)s --filelist=%(FILELIST)s_Embedded_%(ERA)s_%(ch)s_skim.dat --channel=%(ch)s '
          ' --is_embedded=true --output_name=%(JOB)s.root &> jobs/%(JOB)s-%(sc)s.log" jobs/%(JOB)s-%(sc)s.sh' % vars())
        os.system('%(JOBSUBMIT)s jobs/%(JOB)s-%(sc)s.sh' % vars())
    #if ch in ['et', 'mt'] and not options.do_2011:
    #  for sc in scales:
    #    JOB='RecHit_%s_%s' % (ch,YR)
    #    os.system('%(JOBWRAPPER)s "./bin/HiggsTauTau --cfg=%(CONFIG)s %(PREFIXDATA)s --tau_scale_mode=%(sc)s --filelist=%(FILELIST)s_RecHit_%(ERA)s_%(ch)s_skim.dat --channel=%(ch)s '
    #      ' --is_embedded=true --output_name=%(JOB)s.root &> jobs/%(JOB)s-%(sc)s.log" jobs/%(JOB)s-%(sc)s.sh' % vars())
    #    os.system('%(JOBSUBMIT)s jobs/%(JOB)s-%(sc)s.sh' % vars())

    if ch in ['et', 'mt', 'etmet', 'mtmet']:
      if '0' in scales:
        JOB='Data_%s_%s' % (ch,YR)
        os.system('%(JOBWRAPPER)s "./bin/HiggsTauTau --cfg=%(CONFIG)s %(PREFIXDATA)s --filelist=%(FILELIST)s_Special_2_Data_%(ERA)s_%(ch)s_skim.dat --channel=%(ch)s'
          ' --special_mode=3 --output_name=%(JOB)s.root &> jobs/Special_3_%(JOB)s.log" jobs/Special_3_%(JOB)s.sh' % vars())
        os.system('%(JOBSUBMIT)s jobs/Special_3_%(JOB)s.sh' % vars())

    if ch in ['et', 'mt', 'mtmet']  and (PRODUCTION == 'June6' or PRODUCTION == 'Apr11'):
      if '0' in scales:
        JOB='Data_%s_%s' % (ch,YR)
        os.system('%(JOBWRAPPER)s "./bin/HiggsTauTau --cfg=%(CONFIG)s %(PREFIXDATA)s --filelist=%(FILELIST)s_Special_4_Data_%(ERA)s_%(ch)s_skim.dat --channel=%(ch)s'
          ' --special_mode=4 --output_name=%(JOB)s.root &> jobs/Special_4_%(JOB)s.log" jobs/Special_4_%(JOB)s.sh' % vars())
        os.system('%(JOBSUBMIT)s jobs/Special_4_%(JOB)s.sh' % vars())


    if ch in ['em']:
      if '0' in scales:
        JOB='Data_%s_%s' % (ch,YR)
        os.system('%(JOBWRAPPER)s "./bin/HiggsTauTau --cfg=%(CONFIG)s %(PREFIXDATA)s --filelist=%(FILELIST)s_Special_20_Data_%(ERA)s_%(ch)s_skim.dat --channel=%(ch)s \
        --special_mode=20 --output_name=%(JOB)s.root &> jobs/Special_20_%(JOB)s.log" jobs/Special_20_%(JOB)s.sh' % vars())
        os.system('%(JOBSUBMIT)s jobs/Special_20_%(JOB)s.sh' % vars())

        JOB='Data_%s_%s' % (ch,YR)
        os.system('%(JOBWRAPPER)s "./bin/HiggsTauTau --cfg=%(CONFIG)s %(PREFIXDATA)s --filelist=%(FILELIST)s_Special_21_Data_%(ERA)s_%(ch)s_skim.dat --channel=%(ch)s \
        --special_mode=21 --output_name=%(JOB)s.root &> jobs/Special_21_%(JOB)s.log" jobs/Special_21_%(JOB)s.sh' % vars())
        os.system('%(JOBSUBMIT)s jobs/Special_21_%(JOB)s.sh' % vars())

        JOB='Data_%s_%s' % (ch,YR)
        os.system('%(JOBWRAPPER)s "./bin/HiggsTauTau --cfg=%(CONFIG)s %(PREFIXDATA)s --filelist=%(FILELIST)s_Special_22_Data_%(ERA)s_%(ch)s_skim.dat --channel=%(ch)s \
        --special_mode=22 --output_name=%(JOB)s.root &> jobs/Special_22_%(JOB)s.log" jobs/Special_22_%(JOB)s.sh' % vars())
        os.system('%(JOBSUBMIT)s jobs/Special_22_%(JOB)s.sh' % vars())

        if PRODUCTION == 'Feb20' or PRODUCTION == 'Apr11' or PRODUCTION == 'June6':
          JOB='Data_%s_%s' % (ch,YR)
          os.system('%(JOBWRAPPER)s "./bin/HiggsTauTau --cfg=%(CONFIG)s %(PREFIXDATA)s --filelist=%(FILELIST)s_Special_23_Data_%(ERA)s_%(ch)s_skim.dat --channel=%(ch)s \
          --special_mode=23 --output_name=%(JOB)s.root &> jobs/Special_23_%(JOB)s.log" jobs/Special_23_%(JOB)s.sh' % vars())
          os.system('%(JOBSUBMIT)s jobs/Special_23_%(JOB)s.sh' % vars())

          JOB='Data_%s_%s' % (ch,YR)
          os.system('%(JOBWRAPPER)s "./bin/HiggsTauTau --cfg=%(CONFIG)s %(PREFIXDATA)s --filelist=%(FILELIST)s_Special_24_Data_%(ERA)s_%(ch)s_skim.dat --channel=%(ch)s \
          --special_mode=24 --output_name=%(JOB)s.root &> jobs/Special_24_%(JOB)s.log" jobs/Special_24_%(JOB)s.sh' % vars())
          os.system('%(JOBSUBMIT)s jobs/Special_24_%(JOB)s.sh' % vars())
        else:
          JOB='Data_%s_%s' % (ch,YR)
          os.system('%(JOBWRAPPER)s "./bin/HiggsTauTau --cfg=%(CONFIG)s %(PREFIXDATA)s --filelist=%(FILELIST)s_Special_25_Data_%(ERA)s_%(ch)s_skim.dat --channel=%(ch)s \
          --special_mode=23 --output_name=%(JOB)s.root &> jobs/Special_23_%(JOB)s.log" jobs/Special_23_%(JOB)s.sh' % vars())
          os.system('%(JOBSUBMIT)s jobs/Special_23_%(JOB)s.sh' % vars())

          JOB='Data_%s_%s' % (ch,YR)
          os.system('%(JOBWRAPPER)s "./bin/HiggsTauTau --cfg=%(CONFIG)s %(PREFIXDATA)s --filelist=%(FILELIST)s_Special_25_Data_%(ERA)s_%(ch)s_skim.dat --channel=%(ch)s \
          --special_mode=24 --output_name=%(JOB)s.root &> jobs/Special_24_%(JOB)s.log" jobs/Special_24_%(JOB)s.sh' % vars())
          os.system('%(JOBSUBMIT)s jobs/Special_24_%(JOB)s.sh' % vars())

      if '0' in scales:
        JOB='Embedded_%s_%s' % (ch,YR)
        os.system('%(JOBWRAPPER)s "./bin/HiggsTauTau --cfg=%(CONFIG)s %(PREFIXDATA)s --mass_scale_mode=2 --filelist=%(FILELIST)s_Embedded_%(ERA)s_%(ch)s_skim.dat --channel=%(ch)s \
          --is_embedded=true --output_name=%(JOB)s.root &> jobs/%(JOB)s-2.log" jobs/%(JOB)s-2.sh' % vars())
        os.system('%(JOBSUBMIT)s jobs/%(JOB)s-2.sh' % vars())
#      if '0' in scales and not options.do_2011:
#        JOB='RecHit_%s_%s' % (ch,YR)
#        os.system('%(JOBWRAPPER)s "./bin/HiggsTauTau --cfg=%(CONFIG)s %(PREFIXDATA)s --mass_scale_mode=2 --filelist=%(FILELIST)s_RecHit_%(ERA)s_%(ch)s_skim.dat --channel=%(ch)s \
#          --is_embedded=true --output_name=%(JOB)s.root &> jobs/%(JOB)s-2.log" jobs/%(JOB)s-2.sh' % vars())
#        os.system('%(JOBSUBMIT)s jobs/%(JOB)s-2.sh' % vars())

      if '1' in scales:
        JOB='Embedded_%s_%s' % (ch,YR)
        os.system('%(JOBWRAPPER)s "./bin/HiggsTauTau --cfg=%(CONFIG)s %(PREFIXDATA)s --mass_scale_mode=1 --filelist=%(FILELIST)s_Embedded_%(ERA)s_%(ch)s_skim.dat --channel=%(ch)s \
          --is_embedded=true --output_name=%(JOB)s.root &> jobs/%(JOB)s-1.log" jobs/%(JOB)s-1.sh' % vars())
        os.system('%(JOBSUBMIT)s jobs/%(JOB)s-1.sh' % vars())
#      if '1' in scales and not options.do_2011:
#        JOB='RecHit_%s_%s' % (ch,YR)
#        os.system('%(JOBWRAPPER)s "./bin/HiggsTauTau --cfg=%(CONFIG)s %(PREFIXDATA)s --mass_scale_mode=1 --filelist=%(FILELIST)s_RecHit_%(ERA)s_%(ch)s_skim.dat --channel=%(ch)s \
#          --is_embedded=true --output_name=%(JOB)s.root &> jobs/%(JOB)s-1.log" jobs/%(JOB)s-1.sh' % vars())
#        os.system('%(JOBSUBMIT)s jobs/%(JOB)s-1.sh' % vars())

      if '2' in scales:
        JOB='Embedded_%s_%s' % (ch,YR)
        os.system('%(JOBWRAPPER)s "./bin/HiggsTauTau --cfg=%(CONFIG)s %(PREFIXDATA)s --mass_scale_mode=3 --filelist=%(FILELIST)s_Embedded_%(ERA)s_%(ch)s_skim.dat --channel=%(ch)s \
          --is_embedded=true --output_name=%(JOB)s.root &> jobs/%(JOB)s-3.log" jobs/%(JOB)s-3.sh' % vars())
        os.system('%(JOBSUBMIT)s jobs/%(JOB)s-3.sh' % vars())
#      if '2' in scales and not options.do_2011:
#        JOB='RecHit_%s_%s' % (ch,YR)
#        os.system('%(JOBWRAPPER)s "./bin/HiggsTauTau --cfg=%(CONFIG)s %(PREFIXDATA)s --mass_scale_mode=3 --filelist=%(FILELIST)s_RecHit_%(ERA)s_%(ch)s_skim.dat --channel=%(ch)s \
#          --is_embedded=true --output_name=%(JOB)s.root &> jobs/%(JOB)s-3.log" jobs/%(JOB)s-3.sh' % vars())
#        os.system('%(JOBSUBMIT)s jobs/%(JOB)s-3.sh' % vars())


FILELIST='filelists/'+PRODUCTION+'_MC_53X'
if options.do_2011: FILELIST='filelists/'+PRODUCTION+'_MC_42X'

signal_mc = [ ]
signal_mc_ww = [ ]

if options.proc_sm or options.proc_all:
  masses = ['110','115','120','125','130','135','140','145']
  if not options.do_2011: masses += ['90','95','100','105','150','155','160']
  if options.do_2011 and ERA=='Paper' : masses += ['90','95','100','105','150','155','160']
  if options.short_signal: masses = ['125']
  for mass in masses :
    signal_mc += [
      'GluGluToHToTauTau_M-'+mass,
      'VBF_HToTauTau_M-'+mass, 
      'WH_ZH_TTH_HToTauTau_M-'+mass
    ]
  if  not options.do_2011:
    ww_masses = ['110','115','120','125','130','135','140','145','150','155','160']
    if options.short_signal: ww_masses = ['125']
    for ww_mass in ww_masses :
      signal_mc_ww += [
        'GluGluToHToWWTo2LAndTau2Nu_M-'+ww_mass,
        'VBF_HToWWTo2LAndTau2Nu_M-'+ww_mass, 
      ]
  if options.do_2011:
    ww_masses = ['110','115','125','135']
    if options.short_signal: ww_masses = ['125']
    for ww_mass in ww_masses :
      signal_mc_ww += [
        'GluGluToHToWWTo2LAndTau2Nu_M-'+ww_mass,
        'VBF_HToWWTo2LAndTau2Nu_M-'+ww_mass, 
      ]
    ww_masses = ['120','130','140','150','160']
    if options.short_signal: ww_masses = [ ]
    for ww_mass in ww_masses :
      signal_mc_ww += [
        'GluGluToHToWWTo2L2Nu_M-'+ww_mass,
        'GluGluToHToWWTo2Tau2Nu_M-'+ww_mass,
        'GluGluToHToWWToLNuTauNu_M-'+ww_mass,
        'VBF_HToWWTo2L2Nu_M-'+ww_mass,
        'VBF_HToWWTo2Tau2Nu_M-'+ww_mass,
        'VBF_HToWWToLNuTauNu_M-'+ww_mass
      ]
if options.proc_mssm or options.proc_all:
  masses = ['90','100','120','130','140','160','180','200','250','300','350','400','450','500','600','700','800','900','1000']
  if not options.do_2011: masses += ['80','110']
  if options.short_signal: masses = ['160']
  for mass in masses :
    signal_mc += [
      'SUSYGluGluToHToTauTau_M-'+mass,
      'SUSYBBHToTauTau_M-'+mass
    ]



if options.proc_bkg or options.proc_all:
    for ch in channels:
      if ch in ['et', 'mt', 'etmet', 'mtmet']:
        central_samples = [
          'WJetsToLNuSoup',
          'TTJets',
          'TT',
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
          'TTJets',
          'TT',
          'WWJetsTo2L2Nu',
          'WZJetsTo2L2Q',
          'WZJetsTo3LNu',
          'ZZJetsTo2L2Nu',
          'ZZJetsTo2L2Q',
          'ZZJetsTo4L',
          'T-tW',
          'Tbar-tW'
        ]
      
      if options.do_2011:
        central_samples.remove('TT')
      if PRODUCTION=='June6' and (not options.do_2011):
        central_samples += [
          'TTJetsFullLept',
          'TTJetsSemiLept',
          'TTJetsHadronicExt'
        ]
      for sc in scales:
        if ch in ['et', 'mt', 'etmet', 'mtmet']:
          soups = ['', 'Soup']
          if options.do_2011: soups.remove('Soup')
          for sp in soups:  
            if options.do_2011 and ERA=='Total':
              JOB='DYJetsToTauTau%s_%s_%s' % (sp,ch,YR)
              os.system('%(JOBWRAPPER)s "./bin/HiggsTauTau --cfg=%(CONFIG)s %(PREFIXMC)s --tau_scale_mode=%(sc)s --filelist=%(FILELIST)s_DYJetsToTauTau%(sp)s_%(ch)s_skim.dat --channel=%(ch)s'
                ' --ztautau_mode=1 --output_name=%(JOB)s.root &> jobs/%(JOB)s-%(sc)s.log" jobs/%(JOB)s-%(sc)s.sh' % vars())
              os.system('%(JOBSUBMIT)s jobs/%(JOB)s-%(sc)s.sh' % vars())
            else:
              JOB='DYJetsToTauTau%s_%s_%s' % (sp,ch,YR)
              os.system('%(JOBWRAPPER)s "./bin/HiggsTauTau --cfg=%(CONFIG)s %(PREFIXMC)s --tau_scale_mode=%(sc)s --filelist=%(FILELIST)s_DYJetsToLL%(sp)s_%(ch)s_skim.dat --channel=%(ch)s'
                ' --ztautau_mode=1 --faked_tau_selector=2 --output_name=%(JOB)s.root &> jobs/%(JOB)s-%(sc)s.log" jobs/%(JOB)s-%(sc)s.sh' % vars())
              os.system('%(JOBSUBMIT)s jobs/%(JOB)s-%(sc)s.sh' % vars())

            if sc in ['0']:
              JOB='DYJetsToLL%s_%s_%s' % (sp,ch,YR)
              os.system('%(JOBWRAPPER)s "./bin/HiggsTauTau --cfg=%(CONFIG)s %(PREFIXMC)s --tau_scale_mode=%(sc)s --filelist=%(FILELIST)s_DYJetsToLL%(sp)s_%(ch)s_skim.dat --channel=%(ch)s'
                ' --ztautau_mode=2 --output_name=%(JOB)s.root &> jobs/%(JOB)s-%(sc)s.log" jobs/%(JOB)s-%(sc)s.sh' % vars())
              os.system('%(JOBSUBMIT)s jobs/%(JOB)s-%(sc)s.sh' % vars())

              JOB='DYJetsToLL-L%s_%s_%s' % (sp,ch,YR)
              os.system('%(JOBWRAPPER)s "./bin/HiggsTauTau --cfg=%(CONFIG)s %(PREFIXMC)s --tau_scale_mode=%(sc)s --filelist=%(FILELIST)s_DYJetsToLL%(sp)s_%(ch)s_skim.dat --channel=%(ch)s'
                ' --svfit_override=DYJetsToLL%(sp)s_%(ch)s_%(YR)s.root --faked_tau_selector=1 --ztautau_mode=2 --output_name=%(JOB)s.root &> jobs/%(JOB)s-%(sc)s.log" jobs/%(JOB)s-%(sc)s.sh' % vars())
              os.system('%(JOBSUBMIT)s jobs/%(JOB)s-%(sc)s.sh' % vars())

              JOB='DYJetsToLL-J%s_%s_%s' % (sp,ch,YR)
              os.system('%(JOBWRAPPER)s "./bin/HiggsTauTau --cfg=%(CONFIG)s %(PREFIXMC)s --tau_scale_mode=%(sc)s --filelist=%(FILELIST)s_DYJetsToLL%(sp)s_%(ch)s_skim.dat --channel=%(ch)s'
                ' --svfit_override=DYJetsToLL%(sp)s_%(ch)s_%(YR)s.root --faked_tau_selector=2 --ztautau_mode=2 --output_name=%(JOB)s.root &> jobs/%(JOB)s-%(sc)s.log" jobs/%(JOB)s-%(sc)s.sh' % vars())
              os.system('%(JOBSUBMIT)s jobs/%(JOB)s-%(sc)s.sh' % vars())

              JOB='DYJetsToTauTau-L%s_%s_%s' % (sp,ch,YR)
              os.system('%(JOBWRAPPER)s "./bin/HiggsTauTau --cfg=%(CONFIG)s %(PREFIXMC)s --tau_scale_mode=%(sc)s --filelist=%(FILELIST)s_DYJetsToLL%(sp)s_%(ch)s_skim.dat --channel=%(ch)s'
                ' --faked_tau_selector=1 --ztautau_mode=1 --output_name=%(JOB)s.root &> jobs/%(JOB)s-%(sc)s.log" jobs/%(JOB)s-%(sc)s.sh' % vars())
              os.system('%(JOBSUBMIT)s jobs/%(JOB)s-%(sc)s.sh' % vars())

          """
          if sc in ['0']:
            # Special Mode 18 DYJetsToLL-L
            JOB='DYJetsToLL-L_%s_%s' % (ch,YR)
            os.system('%(JOBWRAPPER)s "./bin/HiggsTauTau --cfg=%(CONFIG)s %(PREFIXMC)s --tau_scale_mode=%(sc)s --filelist=%(FILELIST)s_DYJetsToLL_%(ch)s_skim.dat --channel=%(ch)s'
              ' --special_mode=18 --faked_tau_selector=1 --ztautau_mode=2 --output_name=%(JOB)s.root &> jobs/Special_18_%(JOB)s-%(sc)s.log" jobs/Special_18_%(JOB)s-%(sc)s.sh' % vars())
            os.system('%(JOBSUBMIT)s jobs/Special_18_%(JOB)s-%(sc)s.sh' % vars())
          """

        if ch in ['em'] and sc in ['0']:
          soups = ['', 'Soup']
          if options.do_2011: soups.remove('Soup')
          for sp in soups: 
            JOB='DYJetsToTauTau%s_%s_%s' % (sp,ch,YR)
            os.system('%(JOBWRAPPER)s "./bin/HiggsTauTau --cfg=%(CONFIG)s %(PREFIXMC)s --tau_scale_mode=0 --filelist=%(FILELIST)s_DYJetsToLL%(sp)s_%(ch)s_skim.dat --channel=%(ch)s'
              ' --ztautau_mode=1 --output_name=%(JOB)s.root &> jobs/%(JOB)s-%(sc)s.log" jobs/%(JOB)s-%(sc)s.sh' % vars())
            os.system('%(JOBSUBMIT)s jobs/%(JOB)s-%(sc)s.sh' % vars())
            if sc in ['0']:
              JOB='DYJetsToLL%s_%s_%s' % (sp,ch,YR)
              os.system('%(JOBWRAPPER)s "./bin/HiggsTauTau --cfg=%(CONFIG)s %(PREFIXMC)s --tau_scale_mode=%(sc)s --filelist=%(FILELIST)s_DYJetsToLL%(sp)s_%(ch)s_skim.dat --channel=%(ch)s'
                ' --ztautau_mode=2 --output_name=%(JOB)s.root &> jobs/%(JOB)s-%(sc)s.log" jobs/%(JOB)s-%(sc)s.sh' % vars())
              os.system('%(JOBSUBMIT)s jobs/%(JOB)s-%(sc)s.sh' % vars())

        for sa in central_samples:
          if sc in ['0']:
            JOB='%s_%s_%s' % (sa,ch,YR)
            os.system('%(JOBWRAPPER)s "./bin/HiggsTauTau --cfg=%(CONFIG)s %(PREFIXMC)s --tau_scale_mode=%(sc)s --filelist=%(FILELIST)s_%(sa)s_%(ch)s_skim.dat --channel=%(ch)s'
              ' --output_name=%(JOB)s.root &> jobs/%(JOB)s-%(sc)s.log" jobs/%(JOB)s-%(sc)s.sh' % vars())
            os.system('%(JOBSUBMIT)s jobs/%(JOB)s-%(sc)s.sh' % vars())


        if ch in ['et', 'mt', 'mtmet']  and (PRODUCTION == 'June6' or PRODUCTION == 'Apr11'):
          if sc in ['0']:
            JOB='WJetsToLNuSoup_%s_%s' % (ch,YR)
            os.system('%(JOBWRAPPER)s "./bin/HiggsTauTau --cfg=%(CONFIG)s %(PREFIXMC)s --tau_scale_mode=%(sc)s --filelist=%(FILELIST)s_Special_5_WJetsToLNuSoup_%(ch)s_skim.dat --channel=%(ch)s'
              ' --special_mode=5 --output_name=%(JOB)s.root &> jobs/Special_5_%(JOB)s.log" jobs/Special_5_%(JOB)s.sh' % vars())
            os.system('%(JOBSUBMIT)s jobs/Special_5_%(JOB)s.sh' % vars())




if options.proc_sm or options.proc_mssm or options.proc_all:
    for ch in channels:
      for sc in scales:
        for sa in signal_mc:
          JOB='%s_%s_%s' % (sa,ch,YR)
          os.system('%(JOBWRAPPER)s "./bin/HiggsTauTau --cfg=%(CONFIG)s %(PREFIXMC)s --tau_scale_mode=%(sc)s --filelist=%(FILELIST)s_%(sa)s_%(ch)s_skim.dat --channel=%(ch)s'
              ' --output_name=%(JOB)s.root &> jobs/%(JOB)s-%(sc)s.log" jobs/%(JOB)s-%(sc)s.sh' % vars())
          os.system('%(JOBSUBMIT)s jobs/%(JOB)s-%(sc)s.sh' % vars())


if options.proc_sm or options.proc_all:
    for ch in channels:
      #if ch == 'em':
        for sc in scales:
          for sa in signal_mc_ww:
            JOB='%s_%s_%s' % (sa,ch,YR)
            os.system('%(JOBWRAPPER)s "./bin/HiggsTauTau --cfg=%(CONFIG)s %(PREFIXMC)s --tau_scale_mode=%(sc)s --filelist=%(FILELIST)s_%(sa)s_%(ch)s_skim.dat --channel=%(ch)s'
                ' --output_name=%(JOB)s.root &> jobs/%(JOB)s-%(sc)s.log" jobs/%(JOB)s-%(sc)s.sh' % vars())
            os.system('%(JOBSUBMIT)s jobs/%(JOB)s-%(sc)s.sh' % vars())






