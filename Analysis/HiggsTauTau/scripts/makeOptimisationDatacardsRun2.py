#!/usr/bin/env python

import sys
from optparse import OptionParser
import os

CHANNELS= ['et', 'mt', 'em','tt']

def validate_channel(channel):
  assert channel in CHANNELS, 'Error, channel %(channel)s duplicated or unrecognised' % vars()
  CHANNELS.remove(channel)

def split_callback(option, opt, value, parser):
  setattr(parser.values, option.dest, value.split(','))

parser = OptionParser()
parser.add_option("-p","--parameterfile", dest="params", type='string',default='',
                  help="Specify the parameter file containing the luminosity and cross section information - can be used to override config file.")
parser.add_option("--cfg", dest="config", type='string',
                  help="The config file that will be passed to HiggsTauTauPlot5. Parameter file, input folder and signal scheme taken from this cfg file unless overriden by command line options")
parser.add_option("-i","--input", dest="folder", type='string', default='',
                  help="The input folder, containing the output of HTT - can be used to override config file")
parser.add_option("-o","--output", dest="output", type='string', default='',
                  help="The name that will be appended to the datacard inputs.")
parser.add_option("-c", "--channels", dest="channels", type='string', action='callback',callback=split_callback,
                  help="A comma separated list of channels to process.  Supported channels: %(CHANNELS)s" % vars())
parser.add_option("--blind", dest="blind", action='store_true', default=False,
                  help="blind data")
parser.add_option("--extra", dest="extra", type='string', default='',
                  help="Extra command line options, applied to every datacard")
parser.add_option("-s", "--scheme", dest="scheme", type='string', default='',
                  help="datacard scheme - can be used to override config file")
parser.add_option("--mvis", dest="mvis", action='store_true', default=False,
                  help="Only make inputs for visible mass, no svfit.")
parser.add_option("--svfit", dest="svfit", action='store_true', default=False,
                  help="Only make inputs for svfit mass.")
#parser.add_option("--dijet", dest="dijet", action='store_true', default=False,
#                  help="Make inputs for dijet mass.")
#parser.add_option("--mttbb", dest="mttbb", action='store_true', default=False,
#                  help="Make inputs for mttbb.")
#parser.add_option("--mH", dest="mH", action='store_true', default=False,
#                  help="Make inputs for mH using kinematic fitting.")
#parser.add_option("--masscuts", dest="masscuts", action='store_true', default=False,
#                  help="Apply mass cuts on m_bb and m_tautau to 4 body mass for H->hh analysis.")
parser.add_option("-e", dest="energy", type='string', default='13',
                  help="The C.O.M. energy is written into the datacard name, default is 13")

(options, args) = parser.parse_args()

output = options.output
if output: 
  output = '-'+output
  print 'Appending "%(output)s" to datacard outputs' % vars()

channels = options.channels

print 'Processing channels:      %(channels)s' % vars()

### Do some validation of the input
helpMsg = "Run 'datacard_inputs.py -h' for help."

if not channels:
  print 'Error, no channels specified. ' + helpMsg
  sys.exit(1)

for channel in channels:
  validate_channel(channel)


CFG=options.config
BLIND = options.blind
BLIND = "false"
if options.blind: BLIND = "true"
COM = options.energy
YEAR = '2015'

#Hacky config file parsing
with open("%(CFG)s"%vars(),"r") as cfgfile:
  lines = cfgfile.readlines()

configmap={}

for ind in range(0,len(lines)):
  configmap[lines[ind].split("=")[0]]=(lines[ind].split("=")[1])

SCHEME= configmap["signal_scheme"].rstrip('\n')
FOLDER=configmap["folder"].rstrip('\n')
PARAMS=configmap["paramfile"].rstrip('\n')

#Override config file params
if not options.scheme == "":
  SCHEME=options.scheme
if not options.folder ==  "":
  FOLDER=options.folder
if not options.params == "":
  PARAMS=options.params



########## Set up schemes and options

#### Always apply these options:
extra_global = ' --fix_empty_hists="ggH.*,ggA*,qqH.*,VH.*,WH.*,ZH.*,ttH.*,bbH.*"'

#### Apply these options for specific channels
extra_channel = {
  "et" : ' --fix_empty_bins="QCD,ZL,ZJ,ZLL,W" ',
  "mt" : ' --fix_empty_bins="QCD,ZL,ZJ,ZLL,W" ',
  "tt" : ' --fix_empty_bins="QCD,ZL,ZJ,ZLL,W" ',
  "em" : ' --fix_empty_bins="QCD"'
}

#################################################################
#### Old SM scheme
#################################################################
if SCHEME == 'run2_mssm':
#  extra_global += ' --syst_ggh_pt="QCDscale_ggH1in"'
  BINS_FINE="[0,10,20,30,40,50,60,70,80,90,100,110,120,130,140,150,160,170,180,190,200,225,250,275,300,325,350]"
  BINS="[0,20,40,60,80,100,120,140,160,180,200,250,300,350,400,500,700,900,1100,1300,1500,1700,1900,2100,2300,2500,2700,2900,3100,3300,3500,3700,3900]"
  scheme_et = [
  #  ("8",   "notwoprong",    "inclusivemtnotwoprong",  BINS, '--set_alias="sel:mt_1<40"')
    ("8",   "btagdb03iso0p07",    "btagdb03iso0p07",  BINS, '--set_alias="sel:mt_1<30"'),
    ("8",   "btagdb03iso0p08",    "btagdb03iso0p08",  BINS, '--set_alias="sel:mt_1<30"'),
    ("8",   "btagdb03iso0p09",    "btagdb03iso0p09",  BINS, '--set_alias="sel:mt_1<30"'),
    ("8",   "btagdb03iso0p1",    "btagdb03iso0p1",  BINS, '--set_alias="sel:mt_1<30"'),
    ("8",   "btagdb03iso0p11",    "btagdb03iso0p11",  BINS, '--set_alias="sel:mt_1<30"'),
    ("8",   "btagdb03iso0p12",    "btagdb03iso0p12",  BINS, '--set_alias="sel:mt_1<30"'),
    ("8",   "btagdb03iso0p13",    "btagdb03iso0p13",  BINS, '--set_alias="sel:mt_1<30"'),
    ("8",   "btagdb03iso0p14",    "btagdb03iso0p14",  BINS, '--set_alias="sel:mt_1<30"'),
    ("8",   "btagdb03iso0p15",    "btagdb03iso0p15",  BINS, '--set_alias="sel:mt_1<30"'),
    ("8",   "btagdb03iso0p16",    "btagdb03iso0p16",  BINS, '--set_alias="sel:mt_1<30"'),
    ("8",   "btagdb03iso0p17",    "btagdb03iso0p17",  BINS, '--set_alias="sel:mt_1<30"'),
    ("8",   "btagdb03iso0p18",    "btagdb03iso0p18",  BINS, '--set_alias="sel:mt_1<30"'),
    ("8",   "btagdb04iso0p07",    "btagdb04iso0p07",  BINS, '--set_alias="sel:mt_1<30"'),
    ("8",   "btagdb04iso0p08",    "btagdb04iso0p08",  BINS, '--set_alias="sel:mt_1<30"'),
    ("8",   "btagdb04iso0p09",    "btagdb04iso0p09",  BINS, '--set_alias="sel:mt_1<30"'),
    ("8",   "btagdb04iso0p1",    "btagdb04iso0p1",  BINS, '--set_alias="sel:mt_1<30"'),
    ("8",   "btagdb04iso0p11",    "btagdb04iso0p11",  BINS, '--set_alias="sel:mt_1<30"'),
    ("8",   "btagdb04iso0p12",    "btagdb04iso0p12",  BINS, '--set_alias="sel:mt_1<30"'),
    ("8",   "btagdb04iso0p13",    "btagdb04iso0p13",  BINS, '--set_alias="sel:mt_1<30"'),
    ("8",   "btagdb04iso0p14",    "btagdb04iso0p14",  BINS, '--set_alias="sel:mt_1<30"'),
    ("8",   "btagdb04iso0p15",    "btagdb04iso0p15",  BINS, '--set_alias="sel:mt_1<30"'),
    ("8",   "btagdb04iso0p16",    "btagdb04iso0p16",  BINS, '--set_alias="sel:mt_1<30"'),
    ("8",   "btagdb04iso0p17",    "btagdb04iso0p17",  BINS, '--set_alias="sel:mt_1<30"'),
    ("8",   "btagdb04iso0p18",    "btagdb04iso0p18",  BINS, '--set_alias="sel:mt_1<30"'),
    ("8",   "btagea03iso0p07",    "btagea03iso0p07",  BINS, '--set_alias="sel:mt_1<30"'),
    ("8",   "btagea03iso0p08",    "btagea03iso0p08",  BINS, '--set_alias="sel:mt_1<30"'),
    ("8",   "btagea03iso0p09",    "btagea03iso0p09",  BINS, '--set_alias="sel:mt_1<30"'),
    ("8",   "btagea03iso0p1",    "btagea03iso0p1",  BINS, '--set_alias="sel:mt_1<30"'),
    ("8",   "btagea03iso0p11",    "btagea03iso0p11",  BINS, '--set_alias="sel:mt_1<30"'),
    ("8",   "btagea03iso0p12",    "btagea03iso0p12",  BINS, '--set_alias="sel:mt_1<30"'),
    ("8",   "btagea03iso0p13",    "btagea03iso0p13",  BINS, '--set_alias="sel:mt_1<30"'),
    ("8",   "btagea03iso0p14",    "btagea03iso0p14",  BINS, '--set_alias="sel:mt_1<30"'),
    ("8",   "btagea03iso0p15",    "btagea03iso0p15",  BINS, '--set_alias="sel:mt_1<30"'),
    ("8",   "btagea03iso0p16",    "btagea03iso0p16",  BINS, '--set_alias="sel:mt_1<30"'),
    ("8",   "btagea03iso0p17",    "btagea03iso0p17",  BINS, '--set_alias="sel:mt_1<30"'),
    ("8",   "btagea03iso0p18",    "btagea03iso0p18",  BINS, '--set_alias="sel:mt_1<30"'),
    ("8",   "btagdb03allchiso0p07",    "btagdb03allchiso0p07",  BINS, '--set_alias="sel:mt_1<30"'),
    ("8",   "btagdb03allchiso0p08",    "btagdb03allchiso0p08",  BINS, '--set_alias="sel:mt_1<30"'),
    ("8",   "btagdb03allchiso0p09",    "btagdb03allchiso0p09",  BINS, '--set_alias="sel:mt_1<30"'),
    ("8",   "btagdb03allchiso0p1",    "btagdb03allchiso0p1",  BINS, '--set_alias="sel:mt_1<30"'),
    ("8",   "btagdb03allchiso0p11",    "btagdb03allchiso0p11",  BINS, '--set_alias="sel:mt_1<30"'),
    ("8",   "btagdb03allchiso0p12",    "btagdb03allchiso0p12",  BINS, '--set_alias="sel:mt_1<30"'),
    ("8",   "btagdb03allchiso0p13",    "btagdb03allchiso0p13",  BINS, '--set_alias="sel:mt_1<30"'),
    ("8",   "btagdb03allchiso0p14",    "btagdb03allchiso0p14",  BINS, '--set_alias="sel:mt_1<30"'),
    ("8",   "btagdb03allchiso0p15",    "btagdb03allchiso0p15",  BINS, '--set_alias="sel:mt_1<30"'),
    ("8",   "btagdb03allchiso0p16",    "btagdb03allchiso0p16",  BINS, '--set_alias="sel:mt_1<30"'),
    ("8",   "btagdb03allchiso0p17",    "btagdb03allchiso0p17",  BINS, '--set_alias="sel:mt_1<30"'),
    ("8",   "btagdb03allchiso0p18",    "btagdb03allchiso0p18",  BINS, '--set_alias="sel:mt_1<30"'),
    ("8",   "btagdb04allchiso0p07",    "btagdb04allchiso0p07",  BINS, '--set_alias="sel:mt_1<30"'),
    ("8",   "btagdb04allchiso0p08",    "btagdb04allchiso0p08",  BINS, '--set_alias="sel:mt_1<30"'),
    ("8",   "btagdb04allchiso0p09",    "btagdb04allchiso0p09",  BINS, '--set_alias="sel:mt_1<30"'),
    ("8",   "btagdb04allchiso0p1",    "btagdb04allchiso0p1",  BINS, '--set_alias="sel:mt_1<30"'),
    ("8",   "btagdb04allchiso0p11",    "btagdb04allchiso0p11",  BINS, '--set_alias="sel:mt_1<30"'),
    ("8",   "btagdb04allchiso0p12",    "btagdb04allchiso0p12",  BINS, '--set_alias="sel:mt_1<30"'),
    ("8",   "btagdb04allchiso0p13",    "btagdb04allchiso0p13",  BINS, '--set_alias="sel:mt_1<30"'),
    ("8",   "btagdb04allchiso0p14",    "btagdb04allchiso0p14",  BINS, '--set_alias="sel:mt_1<30"'),
    ("8",   "btagdb04allchiso0p15",    "btagdb04allchiso0p15",  BINS, '--set_alias="sel:mt_1<30"'),
    ("8",   "btagdb04allchiso0p16",    "btagdb04allchiso0p16",  BINS, '--set_alias="sel:mt_1<30"'),
    ("8",   "btagdb04allchiso0p17",    "btagdb04allchiso0p17",  BINS, '--set_alias="sel:mt_1<30"'),
    ("8",   "btagdb04allchiso0p18",    "btagdb04allchiso0p18",  BINS, '--set_alias="sel:mt_1<30"'),
    ("8",   "nobtagdb03iso0p07",    "nobtagdb03iso0p07",  BINS_FINE, '--set_alias="sel:mt_1<30"'),
    ("8",   "nobtagdb03iso0p08",    "nobtagdb03iso0p08",  BINS_FINE, '--set_alias="sel:mt_1<30"'),
    ("8",   "nobtagdb03iso0p09",    "nobtagdb03iso0p09",  BINS_FINE, '--set_alias="sel:mt_1<30"'),
    ("8",   "nobtagdb03iso0p1",    "nobtagdb03iso0p1",  BINS_FINE, '--set_alias="sel:mt_1<30"'),
    ("8",   "nobtagdb03iso0p11",    "nobtagdb03iso0p11",  BINS_FINE, '--set_alias="sel:mt_1<30"'),
    ("8",   "nobtagdb03iso0p12",    "nobtagdb03iso0p12",  BINS_FINE, '--set_alias="sel:mt_1<30"'),
    ("8",   "nobtagdb03iso0p13",    "nobtagdb03iso0p13",  BINS_FINE, '--set_alias="sel:mt_1<30"'),
    ("8",   "nobtagdb03iso0p14",    "nobtagdb03iso0p14",  BINS_FINE, '--set_alias="sel:mt_1<30"'),
    ("8",   "nobtagdb03iso0p15",    "nobtagdb03iso0p15",  BINS_FINE, '--set_alias="sel:mt_1<30"'),
    ("8",   "nobtagdb03iso0p16",    "nobtagdb03iso0p16",  BINS_FINE, '--set_alias="sel:mt_1<30"'),
    ("8",   "nobtagdb03iso0p17",    "nobtagdb03iso0p17",  BINS_FINE, '--set_alias="sel:mt_1<30"'),
    ("8",   "nobtagdb03iso0p18",    "nobtagdb03iso0p18",  BINS_FINE, '--set_alias="sel:mt_1<30"'),
    ("8",   "nobtagdb04iso0p07",    "nobtagdb04iso0p07",  BINS_FINE, '--set_alias="sel:mt_1<30"'),
    ("8",   "nobtagdb04iso0p08",    "nobtagdb04iso0p08",  BINS_FINE, '--set_alias="sel:mt_1<30"'),
    ("8",   "nobtagdb04iso0p09",    "nobtagdb04iso0p09",  BINS_FINE, '--set_alias="sel:mt_1<30"'),
    ("8",   "nobtagdb04iso0p1",    "nobtagdb04iso0p1",  BINS_FINE, '--set_alias="sel:mt_1<30"'),
    ("8",   "nobtagdb04iso0p11",    "nobtagdb04iso0p11",  BINS_FINE, '--set_alias="sel:mt_1<30"'),
    ("8",   "nobtagdb04iso0p12",    "nobtagdb04iso0p12",  BINS_FINE, '--set_alias="sel:mt_1<30"'),
    ("8",   "nobtagdb04iso0p13",    "nobtagdb04iso0p13",  BINS_FINE, '--set_alias="sel:mt_1<30"'),
    ("8",   "nobtagdb04iso0p14",    "nobtagdb04iso0p14",  BINS_FINE, '--set_alias="sel:mt_1<30"'),
    ("8",   "nobtagdb04iso0p15",    "nobtagdb04iso0p15",  BINS_FINE, '--set_alias="sel:mt_1<30"'),
    ("8",   "nobtagdb04iso0p16",    "nobtagdb04iso0p16",  BINS_FINE, '--set_alias="sel:mt_1<30"'),
    ("8",   "nobtagdb04iso0p17",    "nobtagdb04iso0p17",  BINS_FINE, '--set_alias="sel:mt_1<30"'),
    ("8",   "nobtagdb04iso0p18",    "nobtagdb04iso0p18",  BINS_FINE, '--set_alias="sel:mt_1<30"'),
    ("8",   "nobtagea03iso0p07",    "nobtagea03iso0p07",  BINS_FINE, '--set_alias="sel:mt_1<30"'),
    ("8",   "nobtagea03iso0p08",    "nobtagea03iso0p08",  BINS_FINE, '--set_alias="sel:mt_1<30"'),
    ("8",   "nobtagea03iso0p09",    "nobtagea03iso0p09",  BINS_FINE, '--set_alias="sel:mt_1<30"'),
    ("8",   "nobtagea03iso0p1",    "nobtagea03iso0p1",  BINS_FINE, '--set_alias="sel:mt_1<30"'),
    ("8",   "nobtagea03iso0p11",    "nobtagea03iso0p11",  BINS_FINE, '--set_alias="sel:mt_1<30"'),
    ("8",   "nobtagea03iso0p12",    "nobtagea03iso0p12",  BINS_FINE, '--set_alias="sel:mt_1<30"'),
    ("8",   "nobtagea03iso0p13",    "nobtagea03iso0p13",  BINS_FINE, '--set_alias="sel:mt_1<30"'),
    ("8",   "nobtagea03iso0p14",    "nobtagea03iso0p14",  BINS_FINE, '--set_alias="sel:mt_1<30"'),
    ("8",   "nobtagea03iso0p15",    "nobtagea03iso0p15",  BINS_FINE, '--set_alias="sel:mt_1<30"'),
    ("8",   "nobtagea03iso0p16",    "nobtagea03iso0p16",  BINS_FINE, '--set_alias="sel:mt_1<30"'),
    ("8",   "nobtagea03iso0p17",    "nobtagea03iso0p17",  BINS_FINE, '--set_alias="sel:mt_1<30"'),
    ("8",   "nobtagea03iso0p18",    "nobtagea03iso0p18",  BINS_FINE, '--set_alias="sel:mt_1<30"'),
    ("8",   "nobtagdb03allchiso0p07",    "nobtagdb03allchiso0p07",  BINS_FINE, '--set_alias="sel:mt_1<30"'),
    ("8",   "nobtagdb03allchiso0p08",    "nobtagdb03allchiso0p08",  BINS_FINE, '--set_alias="sel:mt_1<30"'),
    ("8",   "nobtagdb03allchiso0p09",    "nobtagdb03allchiso0p09",  BINS_FINE, '--set_alias="sel:mt_1<30"'),
    ("8",   "nobtagdb03allchiso0p1",    "nobtagdb03allchiso0p1",  BINS_FINE, '--set_alias="sel:mt_1<30"'),
    ("8",   "nobtagdb03allchiso0p11",    "nobtagdb03allchiso0p11",  BINS_FINE, '--set_alias="sel:mt_1<30"'),
    ("8",   "nobtagdb03allchiso0p12",    "nobtagdb03allchiso0p12",  BINS_FINE, '--set_alias="sel:mt_1<30"'),
    ("8",   "nobtagdb03allchiso0p13",    "nobtagdb03allchiso0p13",  BINS_FINE, '--set_alias="sel:mt_1<30"'),
    ("8",   "nobtagdb03allchiso0p14",    "nobtagdb03allchiso0p14",  BINS_FINE, '--set_alias="sel:mt_1<30"'),
    ("8",   "nobtagdb03allchiso0p15",    "nobtagdb03allchiso0p15",  BINS_FINE, '--set_alias="sel:mt_1<30"'),
    ("8",   "nobtagdb03allchiso0p16",    "nobtagdb03allchiso0p16",  BINS_FINE, '--set_alias="sel:mt_1<30"'),
    ("8",   "nobtagdb03allchiso0p17",    "nobtagdb03allchiso0p17",  BINS_FINE, '--set_alias="sel:mt_1<30"'),
    ("8",   "nobtagdb03allchiso0p18",    "nobtagdb03allchiso0p18",  BINS_FINE, '--set_alias="sel:mt_1<30"'),
    ("8",   "nobtagdb04allchiso0p07",    "nobtagdb04allchiso0p07",  BINS_FINE, '--set_alias="sel:mt_1<30"'),
    ("8",   "nobtagdb04allchiso0p08",    "nobtagdb04allchiso0p08",  BINS_FINE, '--set_alias="sel:mt_1<30"'),
    ("8",   "nobtagdb04allchiso0p09",    "nobtagdb04allchiso0p09",  BINS_FINE, '--set_alias="sel:mt_1<30"'),
    ("8",   "nobtagdb04allchiso0p1",    "nobtagdb04allchiso0p1",  BINS_FINE, '--set_alias="sel:mt_1<30"'),
    ("8",   "nobtagdb04allchiso0p11",    "nobtagdb04allchiso0p11",  BINS_FINE, '--set_alias="sel:mt_1<30"'),
    ("8",   "nobtagdb04allchiso0p12",    "nobtagdb04allchiso0p12",  BINS_FINE, '--set_alias="sel:mt_1<30"'),
    ("8",   "nobtagdb04allchiso0p13",    "nobtagdb04allchiso0p13",  BINS_FINE, '--set_alias="sel:mt_1<30"'),
    ("8",   "nobtagdb04allchiso0p14",    "nobtagdb04allchiso0p14",  BINS_FINE, '--set_alias="sel:mt_1<30"'),
    ("8",   "nobtagdb04allchiso0p15",    "nobtagdb04allchiso0p15",  BINS_FINE, '--set_alias="sel:mt_1<30"'),
    ("8",   "nobtagdb04allchiso0p16",    "nobtagdb04allchiso0p16",  BINS_FINE, '--set_alias="sel:mt_1<30"'),
    ("8",   "nobtagdb04allchiso0p17",    "nobtagdb04allchiso0p17",  BINS_FINE, '--set_alias="sel:mt_1<30"'),
    ("8",   "nobtagdb04allchiso0p18",    "nobtagdb04allchiso0p18",  BINS_FINE, '--set_alias="sel:mt_1<30"'),
    ("8",   "btagvlelm", "btagvlelm", BINS, '--set_alias="sel:mt_1<30"'),
    ("8",   "btagvletm", "btagvletm", BINS, '--set_alias="sel:mt_1<30"'),
    ("8",   "btaglelm", "btaglelm", BINS, '--set_alias="sel:mt_1<30"'),
    ("8",   "btagletm", "btagletm", BINS, '--set_alias="sel:mt_1<30"'),
    ("8",   "btagmelm", "btagmelm", BINS, '--set_alias="sel:mt_1<30"'),
    ("8",   "btagmetm", "btagmetm", BINS, '--set_alias="sel:mt_1<30"'),
    ("8",   "btagtelm", "btagtelm", BINS, '--set_alias="sel:mt_1<30"'),
    ("8",   "btagtetm", "btagtetm", BINS, '--set_alias="sel:mt_1<30"'),
    ("8",   "btagvtelm", "btagvtelm", BINS, '--set_alias="sel:mt_1<30"'),
    ("8",   "btagvtetm", "btagvtetm", BINS, '--set_alias="sel:mt_1<30"'),
    ("8",   "btagdbtight", "btagdbtight", BINS, '--set_alias="sel:mt_1<30"'),
    ("8",   "btagdbmedium", "btagdbmedium", BINS, '--set_alias="sel:mt_1<30"'),
    ("8",   "btagdbloose", "btagdbloose", BINS, '--set_alias="sel:mt_1<30"'),
    ("8",   "btagpwtight", "btagpwtight", BINS, '--set_alias="sel:mt_1<30"'),
    ("8",   "btagpwloose", "btagpwloose", BINS, '--set_alias="sel:mt_1<30"'),
    ("8",   "btagmvavtight","btagmvavtight",BINS, '--set_alias="sel:mt_1<30"'),
    ("8",   "btagmvatight","btagmvatight",BINS,'--set_alias="sel:mt_1<30"'),
    ("8",   "btagmvamedium","btagmvamedium", BINS, '--set_alias="sel:mt_1<30"'),
    ("8",   "btagmvaloose", "btagmvaloose", BINS, '--set_alias="sel:mt_1<30"'),
    ("8",   "btagmvavloose","btagmvavloose",BINS,'--set_alias="sel:mt_1<30"'),
    ("8",   "nobtagvlelm", "nobtagvlelm", BINS_FINE, '--set_alias="sel:mt_1<30"'),
    ("8",   "nobtagvletm", "nobtagvletm", BINS_FINE, '--set_alias="sel:mt_1<30"'),
    ("8",   "nobtaglelm", "nobtaglelm", BINS_FINE, '--set_alias="sel:mt_1<30"'),
    ("8",   "nobtagletm", "nobtagletm", BINS_FINE, '--set_alias="sel:mt_1<30"'),
    ("8",   "nobtagmelm", "nobtagmelm", BINS_FINE, '--set_alias="sel:mt_1<30"'),
    ("8",   "nobtagmetm", "nobtagmetm", BINS_FINE, '--set_alias="sel:mt_1<30"'),
    ("8",   "nobtagtelm", "nobtagtelm", BINS_FINE, '--set_alias="sel:mt_1<30"'),
    ("8",   "nobtagtetm", "nobtagtetm", BINS_FINE, '--set_alias="sel:mt_1<30"'),
    ("8",   "nobtagvtelm", "nobtagvtelm", BINS_FINE, '--set_alias="sel:mt_1<30"'),
    ("8",   "nobtagvtetm", "nobtagvtetm", BINS_FINE, '--set_alias="sel:mt_1<30"'),
    ("8",   "nobtagdbtight", "nobtagdbtight", BINS_FINE, '--set_alias="sel:mt_1<30"'),
    ("8",   "nobtagdbmedium", "nobtagdbmedium", BINS_FINE, '--set_alias="sel:mt_1<30"'),
    ("8",   "nobtagdbloose", "nobtagdbloose", BINS_FINE, '--set_alias="sel:mt_1<30"'),
    ("8",   "nobtagpwtight", "nobtagpwtight", BINS_FINE, '--set_alias="sel:mt_1<30"'),
    ("8",   "nobtagpwloose", "nobtagpwloose", BINS_FINE, '--set_alias="sel:mt_1<30"'),
    ("8",   "nobtagmvavtight","nobtagmvavtight",BINS_FINE, '--set_alias="sel:mt_1<30"'),
    ("8",   "nobtagmvatight","nobtagmvatight",BINS_FINE,'--set_alias="sel:mt_1<30"'),
    ("8",   "nobtagmvamedium","nobtagmvamedium", BINS_FINE, '--set_alias="sel:mt_1<30"'),
    ("8",   "nobtagmvaloose", "nobtagmvaloose", BINS_FINE, '--set_alias="sel:mt_1<30"'),
    ("8",   "nobtagmvavloose","nobtagmvavloose",BINS_FINE,'--set_alias="sel:mt_1<30"')

   # ("8",   "stdlv", "stdlv", BINS, ''),
   #  ("8",   "stdlvwdi", "stdlvwdi",BINS_FINE, ''),
   #  ("8",   "stdlvwloosedi","stdlvwloosedi",BINS_FINE, ''),
   #  ("8",   "stdlvwloosenoiddi","stdlvwloosenoiddi",BINS_FINE,''),

#    ("5",   "vbf",          "vbf",        BINS,      ' --set_alias="W_Shape_Sample:Special_5_WJetsToLNuSoup"'),
#    ("0",   "0jet_low",     "0jet_low",   BINS_FINE, ""),
#    ("1",   "0jet_high",    "0jet_high",  BINS_FINE, ""),
#    ("2",   "1jet_low",     "boost_low",  BINS_FINE, ""),
#    ("3",   "1jet_high",    "boost_high", BINS_FINE, ' --set_alias="w_shape_os:1"')
  ]
  scheme_mt = [
#    ("8",   "inclusive",    "inclusive",  BINS_FINE,  '')
     ("8",   "btagtrk03iso0p02", "btagtrk03iso0p02", BINS, '--set_alias="sel:mt_1<30"'),
     ("8",   "btagtrk03iso0p03", "btagtrk03iso0p03",BINS, '--set_alias="sel:mt_1<30"'),
     ("8",   "btagtrk03iso0p04","btagtrk03iso0p04",BINS, '--set_alias="sel:mt_1<30"'),
     ("8",   "btagtrk03iso0p05","btagtrk03iso0p05",BINS,'--set_alias="sel:mt_1<30"'),
     ("8",   "btagtrk03iso0p06","btagtrk03iso0p06",BINS,'--set_alias="sel:mt_1<30"'),
     ("8",   "btagpuw03iso0p05","btagpuw03iso0p05",BINS,'--set_alias="sel:mt_1<30"'),
     ("8",   "btagpuw03iso0p1","btagpuw03iso0p1",BINS,'--set_alias="sel:mt_1<30"'),
     ("8",   "btagpuw03iso0p15","btagpuw03iso0p15",BINS,'--set_alias="sel:mt_1<30"'),
     ("8",   "btagpuw04iso0p05","btagpuw04iso0p05",BINS,'--set_alias="sel:mt_1<30"'),
     ("8",   "btagpuw04iso0p1","btagpuw04iso0p1",BINS,'--set_alias="sel:mt_1<30"'),
     ("8",   "btagpuw04iso0p15","btagpuw04iso0p15",BINS,'--set_alias="sel:mt_1<30"'),
    ("8",   "btagdb03iso0p07",    "btagdb03iso0p07",  BINS, '--set_alias="sel:mt_1<30"'),
    ("8",   "btagdb03iso0p08",    "btagdb03iso0p08",  BINS, '--set_alias="sel:mt_1<30"'),
    ("8",   "btagdb03iso0p09",    "btagdb03iso0p09",  BINS, '--set_alias="sel:mt_1<30"'),
    ("8",   "btagdb03iso0p1",    "btagdb03iso0p1",  BINS, '--set_alias="sel:mt_1<30"'),
    ("8",   "btagdb03iso0p11",    "btagdb03iso0p11",  BINS, '--set_alias="sel:mt_1<30"'),
    ("8",   "btagdb03iso0p12",    "btagdb03iso0p12",  BINS, '--set_alias="sel:mt_1<30"'),
    ("8",   "btagdb03iso0p13",    "btagdb03iso0p13",  BINS, '--set_alias="sel:mt_1<30"'),
    ("8",   "btagdb03iso0p14",    "btagdb03iso0p14",  BINS, '--set_alias="sel:mt_1<30"'),
    ("8",   "btagdb03iso0p15",    "btagdb03iso0p15",  BINS, '--set_alias="sel:mt_1<30"'),
    ("8",   "btagdb03iso0p16",    "btagdb03iso0p16",  BINS, '--set_alias="sel:mt_1<30"'),
    ("8",   "btagdb03iso0p17",    "btagdb03iso0p17",  BINS, '--set_alias="sel:mt_1<30"'),
    ("8",   "btagdb03iso0p18",    "btagdb03iso0p18",  BINS, '--set_alias="sel:mt_1<30"'),
    #("8",   "ea03iso0p07",    "ea03iso0p07",  BINS, '--set_alias="sel:mt_1<30"'),
    #("8",   "ea03iso0p08",    "ea03iso0p08",  BINS, '--set_alias="sel:mt_1<30"'),
    #("8",   "ea03iso0p09",    "ea03iso0p09",  BINS, '--set_alias="sel:mt_1<30"'),
    #("8",   "ea03iso0p1",    "ea03iso0p1",  BINS, '--set_alias="sel:mt_1<30"'),
    #("8",   "ea03iso0p11",    "ea03iso0p11",  BINS, '--set_alias="sel:mt_1<30"'),
    #("8",   "ea03iso0p12",    "ea03iso0p12",  BINS, '--set_alias="sel:mt_1<30"'),
    #("8",   "ea03iso0p13",    "ea03iso0p13",  BINS, '--set_alias="sel:mt_1<30"'),
    ("8",   "btagdb03allchiso0p07",    "btagdb03allchiso0p07",  BINS, '--set_alias="sel:mt_1<30"'),
    ("8",   "btagdb03allchiso0p08",    "btagdb03allchiso0p08",  BINS, '--set_alias="sel:mt_1<30"'),
    ("8",   "btagdb03allchiso0p09",    "btagdb03allchiso0p09",  BINS, '--set_alias="sel:mt_1<30"'),
    ("8",   "btagdb03allchiso0p1",    "btagdb03allchiso0p1",  BINS, '--set_alias="sel:mt_1<30"'),
    ("8",   "btagdb03allchiso0p11",    "btagdb03allchiso0p11",  BINS, '--set_alias="sel:mt_1<30"'),
    ("8",   "btagdb03allchiso0p12",    "btagdb03allchiso0p12",  BINS, '--set_alias="sel:mt_1<30"'),
    ("8",   "btagdb03allchiso0p13",    "btagdb03allchiso0p13",  BINS, '--set_alias="sel:mt_1<30"'),
    ("8",   "btagdb03allchiso0p14",    "btagdb03allchiso0p14",  BINS, '--set_alias="sel:mt_1<30"'),
    ("8",   "btagdb03allchiso0p15",    "btagdb03allchiso0p15",  BINS, '--set_alias="sel:mt_1<30"'),
    ("8",   "btagdb03allchiso0p16",    "btagdb03allchiso0p16",  BINS, '--set_alias="sel:mt_1<30"'),
    ("8",   "btagdb03allchiso0p17",    "btagdb03allchiso0p17",  BINS, '--set_alias="sel:mt_1<30"'),
    ("8",   "btagdb03allchiso0p18",    "btagdb03allchiso0p18",  BINS, '--set_alias="sel:mt_1<30"'),
    ("8",   "btagdb04allchiso0p07",    "btagdb04allchiso0p07",  BINS, '--set_alias="sel:mt_1<30"'),
    ("8",   "btagdb04allchiso0p08",    "btagdb04allchiso0p08",  BINS, '--set_alias="sel:mt_1<30"'),
    ("8",   "btagdb04allchiso0p09",    "btagdb04allchiso0p09",  BINS, '--set_alias="sel:mt_1<30"'),
    ("8",   "btagdb04allchiso0p1",    "btagdb04allchiso0p1",  BINS, '--set_alias="sel:mt_1<30"'),
    ("8",   "btagdb04allchiso0p11",    "btagdb04allchiso0p11",  BINS, '--set_alias="sel:mt_1<30"'),
    ("8",   "btagdb04allchiso0p12",    "btagdb04allchiso0p12",  BINS, '--set_alias="sel:mt_1<30"'),
    ("8",   "btagdb04allchiso0p13",    "btagdb04allchiso0p13",  BINS, '--set_alias="sel:mt_1<30"'),
    ("8",   "btagdb04allchiso0p14",    "btagdb04allchiso0p14",  BINS, '--set_alias="sel:mt_1<30"'),
    ("8",   "btagdb04allchiso0p15",    "btagdb04allchiso0p15",  BINS, '--set_alias="sel:mt_1<30"'),
    ("8",   "btagdb04allchiso0p16",    "btagdb04allchiso0p16",  BINS, '--set_alias="sel:mt_1<30"'),
    ("8",   "btagdb04allchiso0p17",    "btagdb04allchiso0p17",  BINS, '--set_alias="sel:mt_1<30"'),
    ("8",   "btagdb04allchiso0p18",    "btagdb04allchiso0p18",  BINS, '--set_alias="sel:mt_1<30"'),
    ("8",   "btagdb04iso0p07",    "btagdb04iso0p07",  BINS, '--set_alias="sel:mt_1<30"'),
    ("8",   "btagdb04iso0p08",    "btagdb04iso0p08",  BINS, '--set_alias="sel:mt_1<30"'),
    ("8",   "btagdb04iso0p09",    "btagdb04iso0p09",  BINS, '--set_alias="sel:mt_1<30"'),
    ("8",   "btagdb04iso0p1",    "btagdb04iso0p1",  BINS, '--set_alias="sel:mt_1<30"'),
    ("8",   "btagdb04iso0p11",    "btagdb04iso0p11",  BINS, '--set_alias="sel:mt_1<30"'),
    ("8",   "btagdb04iso0p12",    "btagdb04iso0p12",  BINS, '--set_alias="sel:mt_1<30"'),
    ("8",   "btagdb04iso0p13",    "btagdb04iso0p13",  BINS, '--set_alias="sel:mt_1<30"'),
    ("8",   "btagdb04iso0p14",    "btagdb04iso0p14",  BINS, '--set_alias="sel:mt_1<30"'),
    ("8",   "btagdb04iso0p15",    "btagdb04iso0p15",  BINS, '--set_alias="sel:mt_1<30"'),
    ("8",   "btagdb04iso0p16",    "btagdb04iso0p16",  BINS, '--set_alias="sel:mt_1<30"'),
    ("8",   "btagdb04iso0p17",    "btagdb04iso0p17",  BINS, '--set_alias="sel:mt_1<30"'),
    ("8",   "btagdb04iso0p18",    "btagdb04iso0p18",  BINS, '--set_alias="sel:mt_1<30"'),
    ("8",   "btagtrk03iso0p07",    "btagtrk03iso0p07",  BINS, '--set_alias="sel:mt_1<30"'),
    ("8",   "btagtrk03iso0p08",    "btagtrk03iso0p08",  BINS, '--set_alias="sel:mt_1<30"'),
    ("8",   "btagtrk03iso0p09",    "btagtrk03iso0p09",  BINS, '--set_alias="sel:mt_1<30"'),
    ("8",   "btagtrk03iso0p1",    "btagtrk03iso0p1",  BINS, '--set_alias="sel:mt_1<30"'),
    ("8",   "btagtrk03iso0p11",    "btagtrk03iso0p11",  BINS, '--set_alias="sel:mt_1<30"'),
    ("8",   "btagtrk03iso0p12",    "btagtrk03iso0p12",  BINS, '--set_alias="sel:mt_1<30"'),
    ("8",   "btagtrk03iso0p13",    "btagtrk03iso0p13",  BINS, '--set_alias="sel:mt_1<30"'),
    ("8",   "btagtrk03iso0p14",    "btagtrk03iso0p14",  BINS, '--set_alias="sel:mt_1<30"'),
    ("8",   "btagtrk03iso0p15",    "btagtrk03iso0p15",  BINS, '--set_alias="sel:mt_1<30"'),
    ("8",   "btagtrk03iso0p16",    "btagtrk03iso0p16",  BINS, '--set_alias="sel:mt_1<30"'),
    ("8",   "btagtrk03iso0p17",    "btagtrk03iso0p17",  BINS, '--set_alias="sel:mt_1<30"'),
    ("8",   "btagtrk03iso0p18",    "btagtrk03iso0p18",  BINS, '--set_alias="sel:mt_1<30"'),
    ("8",   "btagvlelm", "btagvlelm", BINS, '--set_alias="sel:mt_1<30"'),
    ("8",   "btagvletm", "btagvletm", BINS, '--set_alias="sel:mt_1<30"'),
    ("8",   "btaglelm", "btaglelm", BINS, '--set_alias="sel:mt_1<30"'),
    ("8",   "btagletm", "btagletm", BINS, '--set_alias="sel:mt_1<30"'),
    ("8",   "btagmelm", "btagmelm", BINS, '--set_alias="sel:mt_1<30"'),
    ("8",   "btagmetm", "btagmetm", BINS, '--set_alias="sel:mt_1<30"'),
    ("8",   "btagtelm", "btagtelm", BINS, '--set_alias="sel:mt_1<30"'),
    ("8",   "btagtetm", "btagtetm", BINS, '--set_alias="sel:mt_1<30"'),
    ("8",   "btagvtelm", "btagvtelm", BINS, '--set_alias="sel:mt_1<30"'),
    ("8",   "btagvtetm", "btagvtetm", BINS, '--set_alias="sel:mt_1<30"'),
    ("8",   "btagdbtight", "btagdbtight", BINS, '--set_alias="sel:mt_1<30"'),
    ("8",   "btagdbmedium", "btagdbmedium", BINS, '--set_alias="sel:mt_1<30"'),
    ("8",   "btagdbloose", "btagdbloose", BINS, '--set_alias="sel:mt_1<30"'),
    ("8",   "btagpwtight", "btagpwtight", BINS, '--set_alias="sel:mt_1<30"'),
    ("8",   "btagpwloose", "btagpwloose", BINS, '--set_alias="sel:mt_1<30"'),
    ("8",   "btagmvavtight","btagmvavtight",BINS, '--set_alias="sel:mt_1<30"'),
    ("8",   "btagmvatight","btagmvatight",BINS,'--set_alias="sel:mt_1<30"'),
    ("8",   "btagmvamedium","btagmvamedium", BINS, '--set_alias="sel:mt_1<30"'),
    ("8",   "btagmvaloose", "btagmvaloose", BINS, '--set_alias="sel:mt_1<30"'),
    ("8",   "btagmvavloose","btagmvavloose",BINS,'--set_alias="sel:mt_1<30"'),
    ("8",   "nobtagvlelm", "nobtagvlelm", BINS_FINE, '--set_alias="sel:mt_1<30"'),
    ("8",   "nobtagvletm", "nobtagvletm", BINS_FINE, '--set_alias="sel:mt_1<30"'),
    ("8",   "nobtaglelm", "nobtaglelm", BINS_FINE, '--set_alias="sel:mt_1<30"'),
    ("8",   "nobtagletm", "nobtagletm", BINS_FINE, '--set_alias="sel:mt_1<30"'),
    ("8",   "nobtagmelm", "nobtagmelm", BINS_FINE, '--set_alias="sel:mt_1<30"'),
    ("8",   "nobtagmetm", "nobtagmetm", BINS_FINE, '--set_alias="sel:mt_1<30"'),
    ("8",   "nobtagtelm", "nobtagtelm", BINS_FINE, '--set_alias="sel:mt_1<30"'),
    ("8",   "nobtagtetm", "nobtagtetm", BINS_FINE, '--set_alias="sel:mt_1<30"'),
    ("8",   "nobtagvtelm", "nobtagvtelm", BINS_FINE, '--set_alias="sel:mt_1<30"'),
    ("8",   "nobtagvtetm", "nobtagvtetm", BINS_FINE, '--set_alias="sel:mt_1<30"'),
    ("8",   "nobtagdbtight", "nobtagdbtight", BINS_FINE, '--set_alias="sel:mt_1<30"'),
    ("8",   "nobtagdbmedium", "nobtagdbmedium", BINS_FINE, '--set_alias="sel:mt_1<30"'),
    ("8",   "nobtagdbloose", "nobtagdbloose", BINS_FINE, '--set_alias="sel:mt_1<30"'),
    ("8",   "nobtagpwtight", "nobtagpwtight", BINS_FINE, '--set_alias="sel:mt_1<30"'),
    ("8",   "nobtagpwloose", "nobtagpwloose", BINS_FINE, '--set_alias="sel:mt_1<30"'),
    ("8",   "nobtagmvavtight","nobtagmvavtight",BINS_FINE, '--set_alias="sel:mt_1<30"'),
    ("8",   "nobtagmvatight","nobtagmvatight",BINS_FINE,'--set_alias="sel:mt_1<30"'),
    ("8",   "nobtagmvamedium","nobtagmvamedium", BINS_FINE, '--set_alias="sel:mt_1<30"'),
    ("8",   "nobtagmvaloose", "nobtagmvaloose", BINS_FINE, '--set_alias="sel:mt_1<30"'),
    ("8",   "nobtagmvavloose","nobtagmvavloose",BINS_FINE,'--set_alias="sel:mt_1<30"')

#    ("5",   "vbf",          "vbf",        BINS,       ' --set_alias="W_Shape_Sample:Special_5_WJetsToLNuSoup"'),
#    ("0",   "0jet_low",     "0jet_low",   BINS_FINE,  ''),
#    ("1",   "0jet_high",    "0jet_high",  BINS_FINE,  ''),
#    ("2",   "1jet_low",     "boost_low",  BINS_FINE,  ' --syst_qcd_shape="CMS_htt_QCDShape_mutau_boost_low_'+COM+'TeV:50:1.15:0.15"'),
#    ("3",   "1jet_high",    "boost_high", BINS_FINE,  ' --set_alias="w_shape_os:1"')
  ]
  scheme_tt = [
    #("8",   "inclusive",    "inclusive",  BINS_FINE,  '')
    #("8",   "minlv",         "minlv"     ,  BINS_FINE, ''),
    ("8",   "btagincvlelm",    "btagincvlelm",  BINS, ''),
    ("8",   "btagincvletm",    "btagincvletm",  BINS, ''),
    ("8",   "btaginclelm",    "btaginclelm",  BINS, ''),
    ("8",   "btagincletm",    "btagincletm",  BINS, ''),
    ("8",   "btagincmelm",    "btagincmelm",  BINS, ''),
    ("8",   "btagincmetm",    "btagincmetm",  BINS, ''),
    ("8",   "btaginctelm",    "btaginctelm",  BINS, ''),
    ("8",   "btaginctetm",    "btaginctetm",  BINS, ''),
    ("8",   "btagincvtelm",    "btagincvtelm",  BINS, ''),
    ("8",   "btagincvtetm",    "btagincvtetm",  BINS, ''),
    ("8",   "btagdbloose",    "btagdbloose",  BINS, ''),
    ("8",   "btagdbmedium",    "btagdbmedium",  BINS, ''),
    ("8",   "btagdbtight",    "btagdbtight",  BINS, ''),
    ("8",   "btagpwloose",    "btagpwloose",  BINS, ''),
    ("8",   "btagpwmedium",    "btagpwmedium",  BINS, ''),
    ("8",   "btagpwtight",    "btagpwtight",  BINS, ''),
    ("8",   "btagmvavloose",    "btagmvavloose",  BINS, ''),
    ("8",   "btagmvaloose",    "btagmvaloose",  BINS, ''),
    ("8",   "btagmvamedium",    "btagmvamedium",  BINS, ''),
    ("8",   "btagmvatight",    "btagmvatight",  BINS, ''),
    ("8",   "btagmvavtight",    "btagmvavtight",  BINS, ''),
    ("8",   "nobtagincvlelm",    "nobtagincvlelm",  BINS, ''),
    ("8",   "nobtagincvletm",    "nobtagincvletm",  BINS, ''),
    ("8",   "nobtaginclelm",    "nobtaginclelm",  BINS, ''),
    ("8",   "nobtagincletm",    "nobtagincletm",  BINS, ''),
    ("8",   "nobtagincmelm",    "nobtagincmelm",  BINS, ''),
    ("8",   "nobtagincmetm",    "nobtagincmetm",  BINS, ''),
    ("8",   "nobtaginctelm",    "nobtaginctelm",  BINS, ''),
    ("8",   "nobtaginctetm",    "nobtaginctetm",  BINS, ''),
    ("8",   "nobtagincvtelm",    "nobtagincvtelm",  BINS, ''),
    ("8",   "nobtagincvtetm",    "nobtagincvtetm",  BINS, ''),
    ("8",   "nobtagdbloose",    "nobtagdbloose",  BINS, ''),
    ("8",   "nobtagdbmedium",    "nobtagdbmedium",  BINS, ''),
    ("8",   "nobtagdbtight",    "nobtagdbtight",  BINS, ''),
    ("8",   "nobtagpwloose",    "nobtagpwloose",  BINS, ''),
    ("8",   "nobtagpwmedium",    "nobtagpwmedium",  BINS, ''),
    ("8",   "nobtagpwtight",    "nobtagpwtight",  BINS, ''),
    ("8",   "nobtagmvavloose",    "nobtagmvavloose",  BINS, ''),
    ("8",   "nobtagmvaloose",    "nobtagmvaloose",  BINS, ''),
    ("8",   "nobtagmvamedium",    "nobtagmvamedium",  BINS, ''),
    ("8",   "nobtagmvatight",    "nobtagmvatight",  BINS, ''),
    ("8",   "nobtagmvavtight",    "nobtagmvavtight",  BINS, ''),


#    ("5",   "vbf",          "vbf",        BINS,       ' --set_alias="W_Shape_Sample:Special_5_WJetsToLNuSoup"'),
#    ("0",   "0jet_low",     "0jet_low",   BINS_FINE,  ''),
#    ("1",   "0jet_high",    "0jet_high",  BINS_FINE,  ''),
#    ("2",   "1jet_low",     "boost_low",  BINS_FINE,  ' --syst_qcd_shape="CMS_htt_QCDShape_mutau_boost_low_'+COM+'TeV:50:1.15:0.15"'),
#    ("3",   "1jet_high",    "boost_high", BINS_FINE,  ' --set_alias="w_shape_os:1"')
  ]
  scheme_em = [
    ("8", "btagmtrk03iso0p05", "btagmtrk03iso0p05", BINS, '--set_alias="sel:pzeta>-20"'),
    ("8", "btagmtrk03iso0p06", "btagmtrk03iso0p06", BINS, '--set_alias="sel:pzeta>-20"'),
    ("8", "btagmtrk03iso0p07", "btagmtrk03iso0p07", BINS, '--set_alias="sel:pzeta>-20"'),
    ("8", "btagmtrk03iso0p08", "btagmtrk03iso0p08", BINS, '--set_alias="sel:pzeta>-20"'),
    ("8", "btagmtrk03iso0p08", "btagmtrk03iso0p08", BINS, '--set_alias="sel:pzeta>-20"'),
    ("8", "btagmpuw03iso0p05", "btagmpuw03iso0p05", BINS, '--set_alias="sel:pzeta>-20"'),
    ("8", "btagmpuw03iso0p1", "btagmpuw03iso0p1", BINS, '--set_alias="sel:pzeta>-20"'),
    ("8", "btagmpuw03iso0p15", "btagmpuw03iso0p15", BINS, '--set_alias="sel:pzeta>-20"'),
    ("8", "btagmpuw04iso0p05", "btagmpuw04iso0p05", BINS, '--set_alias="sel:pzeta>-20"'),
    ("8", "btagmpuw04iso0p1", "btagmpuw04iso0p1", BINS, '--set_alias="sel:pzeta>-20"'),
    ("8", "btagmpuw04iso0p15", "btagmpuw04iso0p15", BINS, '--set_alias="sel:pzeta>-20"'),
#    ("8",   "inclusive",    "inclusive",  BINS, '--set_alias="sel:pzeta>-20"')
    ("8",   "btagedb03iso0p1",    "btagedb03iso0p1",  BINS, '--set_alias="sel:pzeta>-20"'),
    ("8",   "btagedb03iso0p11",    "btagedb03iso0p11",  BINS, '--set_alias="sel:pzeta>-20"'),
    ("8",   "btagedb03iso0p12",    "btagedb03iso0p12",  BINS, '--set_alias="sel:pzeta>-20"'),
    ("8",   "btagedb03iso0p13",    "btagedb03iso0p13",  BINS, '--set_alias="sel:pzeta>-20"'),
    ("8",   "btagedb03iso0p14",    "btagedb03iso0p14",  BINS, '--set_alias="sel:pzeta>-20"'),
    ("8",   "btagedb03iso0p15",    "btagedb03iso0p15",  BINS, '--set_alias="sel:pzeta>-20"'),
    ("8",   "btagedb03iso0p16",    "btagedb03iso0p16",  BINS, '--set_alias="sel:pzeta>-20"'),
    ("8",   "btagedb03iso0p17",    "btagedb03iso0p17",  BINS, '--set_alias="sel:pzeta>-20"'),
    ("8",   "btagedb03iso0p18",    "btagedb03iso0p18",  BINS, '--set_alias="sel:pzeta>-20"'),
    ("8",   "btagedb03iso0p19",    "btagedb03iso0p19",  BINS, '--set_alias="sel:pzeta>-20"'),
    ("8",   "btagedb03iso0p20",    "btagedb03iso0p20",  BINS, '--set_alias="sel:pzeta>-20"'),
    ("8",   "btagedb03iso0p21",    "btagedb03iso0p21",  BINS, '--set_alias="sel:pzeta>-20"'),
    ("8",   "btagedb04iso0p1",    "btagedb04iso0p1",  BINS, '--set_alias="sel:pzeta>-20"'),
    ("8",   "btagedb04iso0p11",    "btagedb04iso0p11",  BINS, '--set_alias="sel:pzeta>-20"'),
    ("8",   "btagedb04iso0p12",    "btagedb04iso0p12",  BINS, '--set_alias="sel:pzeta>-20"'),
    ("8",   "btagedb04iso0p13",    "btagedb04iso0p13",  BINS, '--set_alias="sel:pzeta>-20"'),
    ("8",   "btagedb04iso0p14",    "btagedb04iso0p14",  BINS, '--set_alias="sel:pzeta>-20"'),
    ("8",   "btagedb04iso0p15",    "btagedb04iso0p15",  BINS, '--set_alias="sel:pzeta>-20"'),
    ("8",   "btagedb04iso0p16",    "btagedb04iso0p16",  BINS, '--set_alias="sel:pzeta>-20"'),
    ("8",   "btagedb04iso0p17",    "btagedb04iso0p17",  BINS, '--set_alias="sel:pzeta>-20"'),
    ("8",   "btagedb04iso0p18",    "btagedb04iso0p18",  BINS, '--set_alias="sel:pzeta>-20"'),
    ("8",   "btagedb04iso0p19",    "btagedb04iso0p19",  BINS, '--set_alias="sel:pzeta>-20"'),
    ("8",   "btagedb04iso0p20",    "btagedb04iso0p20",  BINS, '--set_alias="sel:pzeta>-20"'),
    ("8",   "btagedb04iso0p21",    "btagedb04iso0p21",  BINS, '--set_alias="sel:pzeta>-20"'),
    ("8",   "btagmdb03iso0p1",    "btagmdb03iso0p1",  BINS, '--set_alias="sel:pzeta>-20"'),
    ("8",   "btagmdb03iso0p11",    "btagmdb03iso0p11",  BINS, '--set_alias="sel:pzeta>-20"'),
    ("8",   "btagmdb03iso0p12",    "btagmdb03iso0p12",  BINS, '--set_alias="sel:pzeta>-20"'),
    ("8",   "btagmdb03iso0p13",    "btagmdb03iso0p13",  BINS, '--set_alias="sel:pzeta>-20"'),
    ("8",   "btagmdb03iso0p14",    "btagmdb03iso0p14",  BINS, '--set_alias="sel:pzeta>-20"'),
    ("8",   "btagmdb03iso0p15",    "btagmdb03iso0p15",  BINS, '--set_alias="sel:pzeta>-20"'),
    ("8",   "btagmdb03iso0p16",    "btagmdb03iso0p16",  BINS, '--set_alias="sel:pzeta>-20"'),
    ("8",   "btagmdb03iso0p17",    "btagmdb03iso0p17",  BINS, '--set_alias="sel:pzeta>-20"'),
    ("8",   "btagmdb03iso0p18",    "btagmdb03iso0p18",  BINS, '--set_alias="sel:pzeta>-20"'),
    ("8",   "btagmdb03iso0p19",    "btagmdb03iso0p19",  BINS, '--set_alias="sel:pzeta>-20"'),
   ("8",   "btagmdb03iso0p20",    "btagmdb03iso0p20",  BINS, '--set_alias="sel:pzeta>-20"'),
    ("8",   "btagmdb03iso0p21",    "btagmdb03iso0p21",  BINS, '--set_alias="sel:pzeta>-20"'),
    ("8",   "btageea03iso0p1",    "btageea03iso0p1",  BINS, '--set_alias="sel:pzeta>-20"'),
    ("8",   "btageea03iso0p11",    "btageea03iso0p11",  BINS, '--set_alias="sel:pzeta>-20"'),
    ("8",   "btageea03iso0p12",    "btageea03iso0p12",  BINS, '--set_alias="sel:pzeta>-20"'),
    ("8",   "btageea03iso0p13",    "btageea03iso0p13",  BINS, '--set_alias="sel:pzeta>-20"'),
    ("8",   "btageea03iso0p14",    "btageea03iso0p14",  BINS, '--set_alias="sel:pzeta>-20"'),
    ("8",   "btageea03iso0p15",    "btageea03iso0p15",  BINS, '--set_alias="sel:pzeta>-20"'),
    ("8",   "btageea03iso0p16",    "btageea03iso0p16",  BINS, '--set_alias="sel:pzeta>-20"'),
    ("8",   "btageea03iso0p17",    "btageea03iso0p17",  BINS, '--set_alias="sel:pzeta>-20"'),
    ("8",   "btageea03iso0p18",    "btageea03iso0p18",  BINS, '--set_alias="sel:pzeta>-20"'),
    ("8",   "btageea03iso0p19",    "btageea03iso0p19",  BINS, '--set_alias="sel:pzeta>-20"'),
    ("8",   "btageea03iso0p20",    "btageea03iso0p20",  BINS, '--set_alias="sel:pzeta>-20"'),
    ("8",   "btageea03iso0p21",    "btageea03iso0p21",  BINS, '--set_alias="sel:pzeta>-20"'),
    ("8",   "btagmea03iso0p1",    "btagmea03iso0p1",  BINS, '--set_alias="sel:pzeta>-20"'),
    ("8",   "btagmea03iso0p11",    "btagmea03iso0p11",  BINS, '--set_alias="sel:pzeta>-20"'),
    ("8",   "btagmea03iso0p12",    "btagmea03iso0p12",  BINS, '--set_alias="sel:pzeta>-20"'),
    ("8",   "btagmea03iso0p13",    "btagmea03iso0p13",  BINS, '--set_alias="sel:pzeta>-20"'),
    ("8",   "btagmea03iso0p14",    "btagmea03iso0p14",  BINS, '--set_alias="sel:pzeta>-20"'),
    ("8",   "btagmea03iso0p15",    "btagmea03iso0p15",  BINS, '--set_alias="sel:pzeta>-20"'),
    ("8",   "btagmea03iso0p16",    "btagmea03iso0p16",  BINS, '--set_alias="sel:pzeta>-20"'),
    ("8",   "btagmea03iso0p17",    "btagmea03iso0p17",  BINS, '--set_alias="sel:pzeta>-20"'),
    ("8",   "btagmea03iso0p18",    "btagmea03iso0p18",  BINS, '--set_alias="sel:pzeta>-20"'),
    ("8",   "btagmea03iso0p19",    "btagmea03iso0p19",  BINS, '--set_alias="sel:pzeta>-20"'),
    ("8",   "btagmea03iso0p20",    "btagmea03iso0p20",  BINS, '--set_alias="sel:pzeta>-20"'),
    ("8",   "btagmea03iso0p21",    "btagmea03iso0p21",  BINS, '--set_alias="sel:pzeta>-20"'),
    ("8",   "btagedb03allchiso0p1",    "btagedb03allchiso0p1",  BINS, '--set_alias="sel:pzeta>-20"'),
    ("8",   "btagedb03allchiso0p11",    "btagedb03allchiso0p11",  BINS, '--set_alias="sel:pzeta>-20"'),
    ("8",   "btagedb03allchiso0p12",    "btagedb03allchiso0p12",  BINS, '--set_alias="sel:pzeta>-20"'),
    ("8",   "btagedb03allchiso0p13",    "btagedb03allchiso0p13",  BINS, '--set_alias="sel:pzeta>-20"'),
    ("8",   "btagedb03allchiso0p14",    "btagedb03allchiso0p14",  BINS, '--set_alias="sel:pzeta>-20"'),
    ("8",   "btagedb03allchiso0p15",    "btagedb03allchiso0p15",  BINS, '--set_alias="sel:pzeta>-20"'),
    ("8",   "btagedb03allchiso0p16",    "btagedb03allchiso0p16",  BINS, '--set_alias="sel:pzeta>-20"'),
    ("8",   "btagedb03allchiso0p17",    "btagedb03allchiso0p17",  BINS, '--set_alias="sel:pzeta>-20"'),
    ("8",   "btagedb03allchiso0p18",    "btagedb03allchiso0p18",  BINS, '--set_alias="sel:pzeta>-20"'),
    ("8",   "btagedb03allchiso0p19",    "btagedb03allchiso0p19",  BINS, '--set_alias="sel:pzeta>-20"'),
    ("8",   "btagedb03allchiso0p20",    "btagedb03allchiso0p20",  BINS, '--set_alias="sel:pzeta>-20"'),
    ("8",   "btagedb03allchiso0p21",    "btagedb03allchiso0p21",  BINS, '--set_alias="sel:pzeta>-20"'),
    ("8",   "btagmdb03allchiso0p1",    "btagmdb03allchiso0p1",  BINS, '--set_alias="sel:pzeta>-20"'),
    ("8",   "btagmdb03allchiso0p11",    "btagmdb03allchiso0p11",  BINS, '--set_alias="sel:pzeta>-20"'),
    ("8",   "btagmdb03allchiso0p12",    "btagmdb03allchiso0p12",  BINS, '--set_alias="sel:pzeta>-20"'),
    ("8",   "btagmdb03allchiso0p13",    "btagmdb03allchiso0p13",  BINS, '--set_alias="sel:pzeta>-20"'),
    ("8",   "btagmdb03allchiso0p14",    "btagmdb03allchiso0p14",  BINS, '--set_alias="sel:pzeta>-20"'),
    ("8",   "btagmdb03allchiso0p15",    "btagmdb03allchiso0p15",  BINS, '--set_alias="sel:pzeta>-20"'),
    ("8",   "btagmdb03allchiso0p16",    "btagmdb03allchiso0p16",  BINS, '--set_alias="sel:pzeta>-20"'),
    ("8",   "btagmdb03allchiso0p17",    "btagmdb03allchiso0p17",  BINS, '--set_alias="sel:pzeta>-20"'),
    ("8",   "btagmdb03allchiso0p18",    "btagmdb03allchiso0p18",  BINS, '--set_alias="sel:pzeta>-20"'),
    ("8",   "btagmdb03allchiso0p19",    "btagmdb03allchiso0p19",  BINS, '--set_alias="sel:pzeta>-20"'),
    ("8",   "btagmdb03allchiso0p20",    "btagmdb03allchiso0p20",  BINS, '--set_alias="sel:pzeta>-20"'),
    ("8",   "btagmdb03allchiso0p21",    "btagmdb03allchiso0p21",  BINS, '--set_alias="sel:pzeta>-20"'),
    ("8",   "btagedb04allchiso0p1",    "btagedb04allchiso0p1",  BINS, '--set_alias="sel:pzeta>-20"'),
    ("8",   "btagedb04allchiso0p11",    "btagedb04allchiso0p11",  BINS, '--set_alias="sel:pzeta>-20"'),
    ("8",   "btagedb04allchiso0p12",    "btagedb04allchiso0p12",  BINS, '--set_alias="sel:pzeta>-20"'),
    ("8",   "btagedb04allchiso0p13",    "btagedb04allchiso0p13",  BINS, '--set_alias="sel:pzeta>-20"'),
    ("8",   "btagedb04allchiso0p14",    "btagedb04allchiso0p14",  BINS, '--set_alias="sel:pzeta>-20"'),
    ("8",   "btagedb04allchiso0p15",    "btagedb04allchiso0p15",  BINS, '--set_alias="sel:pzeta>-20"'),
    ("8",   "btagedb04allchiso0p16",    "btagedb04allchiso0p16",  BINS, '--set_alias="sel:pzeta>-20"'),
    ("8",   "btagedb04allchiso0p17",    "btagedb04allchiso0p17",  BINS, '--set_alias="sel:pzeta>-20"'),
    ("8",   "btagedb04allchiso0p18",    "btagedb04allchiso0p18",  BINS, '--set_alias="sel:pzeta>-20"'),
   ("8",   "btagedb04allchiso0p19",    "btagedb04allchiso0p19",  BINS, '--set_alias="sel:pzeta>-20"'),
   ("8",   "btagedb04allchiso0p20",    "btagedb04allchiso0p20",  BINS, '--set_alias="sel:pzeta>-20"'),
   ("8",   "btagedb04allchiso0p21",    "btagedb04allchiso0p21",  BINS, '--set_alias="sel:pzeta>-20"'),
    ("8",   "btagmdb04allchiso0p1",    "btagmdb04allchiso0p1",  BINS, '--set_alias="sel:pzeta>-20"'),
    ("8",   "btagmdb04allchiso0p11",    "btagmdb04allchiso0p11",  BINS, '--set_alias="sel:pzeta>-20"'),
    ("8",   "btagmdb04allchiso0p12",    "btagmdb04allchiso0p12",  BINS, '--set_alias="sel:pzeta>-20"'),
    ("8",   "btagmdb04allchiso0p13",    "btagmdb04allchiso0p13",  BINS, '--set_alias="sel:pzeta>-20"'),
    ("8",   "btagmdb04allchiso0p14",    "btagmdb04allchiso0p14",  BINS, '--set_alias="sel:pzeta>-20"'),
    ("8",   "btagmdb04allchiso0p15",    "btagmdb04allchiso0p15",  BINS, '--set_alias="sel:pzeta>-20"'),
    ("8",   "btagmdb04allchiso0p16",    "btagmdb04allchiso0p16",  BINS, '--set_alias="sel:pzeta>-20"'),
    ("8",   "btagmdb04allchiso0p17",    "btagmdb04allchiso0p17",  BINS, '--set_alias="sel:pzeta>-20"'),
    ("8",   "btagmdb04allchiso0p18",    "btagmdb04allchiso0p18",  BINS, '--set_alias="sel:pzeta>-20"'),
    ("8",   "btagmdb04allchiso0p19",    "btagmdb04allchiso0p19",  BINS, '--set_alias="sel:pzeta>-20"'),
    ("8",   "btagmdb04allchiso0p20",    "btagmdb04allchiso0p20",  BINS, '--set_alias="sel:pzeta>-20"'),
    ("8",   "btagmdb04allchiso0p21",    "btagmdb04allchiso0p21",  BINS, '--set_alias="sel:pzeta>-20"'),
    ("8",   "btagmdb04iso0p1",    "btagmdb04iso0p1",  BINS, '--set_alias="sel:pzeta>-20"'),
    ("8",   "btagmdb04iso0p11",    "btagmdb04iso0p11",  BINS, '--set_alias="sel:pzeta>-20"'),
    ("8",   "btagmdb04iso0p12",    "btagmdb04iso0p12",  BINS, '--set_alias="sel:pzeta>-20"'),
    ("8",   "btagmdb04iso0p13",    "btagmdb04iso0p13",  BINS, '--set_alias="sel:pzeta>-20"'),
    ("8",   "btagmdb04iso0p14",    "btagmdb04iso0p14",  BINS, '--set_alias="sel:pzeta>-20"'),
    ("8",   "btagmdb04iso0p15",    "btagmdb04iso0p15",  BINS, '--set_alias="sel:pzeta>-20"'),
    ("8",   "btagmdb04iso0p16",    "btagmdb04iso0p16",  BINS, '--set_alias="sel:pzeta>-20"'),
    ("8",   "btagmdb04iso0p17",    "btagmdb04iso0p17",  BINS, '--set_alias="sel:pzeta>-20"'),
    ("8",   "btagmdb04iso0p18",    "btagmdb04iso0p18",  BINS, '--set_alias="sel:pzeta>-20"'),
    ("8",   "btagmdb04iso0p19",    "btagmdb04iso0p19",  BINS, '--set_alias="sel:pzeta>-20"'),
    ("8",   "btagmdb04iso0p20",    "btagmdb04iso0p20",  BINS, '--set_alias="sel:pzeta>-20"'),
    ("8",   "btagmdb04iso0p21",    "btagmdb04iso0p21",  BINS, '--set_alias="sel:pzeta>-20"'),
    ("8",   "btagmtrk03iso0p1",    "btagmtrk03iso0p1",  BINS, '--set_alias="sel:pzeta>-20"'),
    ("8",   "btagmtrk03iso0p11",    "btagmtrk03iso0p11",  BINS, '--set_alias="sel:pzeta>-20"'),
    ("8",   "btagmtrk03iso0p12",    "btagmtrk03iso0p12",  BINS, '--set_alias="sel:pzeta>-20"'),
    ("8",   "btagmtrk03iso0p13",    "btagmtrk03iso0p13",  BINS, '--set_alias="sel:pzeta>-20"'),
    ("8",   "btagmtrk03iso0p14",    "btagmtrk03iso0p14",  BINS, '--set_alias="sel:pzeta>-20"'),
    ("8",   "btagmtrk03iso0p15",    "btagmtrk03iso0p15",  BINS, '--set_alias="sel:pzeta>-20"'),
    ("8",   "btagmtrk03iso0p16",    "btagmtrk03iso0p16",  BINS, '--set_alias="sel:pzeta>-20"'),
    ("8",   "btagmtrk03iso0p17",    "btagmtrk03iso0p17",  BINS, '--set_alias="sel:pzeta>-20"'),
    ("8",   "btagmtrk03iso0p18",    "btagmtrk03iso0p18",  BINS, '--set_alias="sel:pzeta>-20"'),
    ("8",   "btagmtrk03iso0p19",    "btagmtrk03iso0p19",  BINS, '--set_alias="sel:pzeta>-20"'),
    ("8",   "btagmtrk03iso0p20",    "btagmtrk03iso0p20",  BINS, '--set_alias="sel:pzeta>-20"'),
    ("8",   "btagmtrk03iso0p21",    "btagmtrk03iso0p21",  BINS, '--set_alias="sel:pzeta>-20"'),
    ("8", "nobtagmtrk03iso0p05", "nobtagmtrk03iso0p05", BINS_FINE, '--set_alias="sel:pzeta>-20"'),
    ("8", "nobtagmtrk03iso0p06", "nobtagmtrk03iso0p06", BINS_FINE, '--set_alias="sel:pzeta>-20"'),
    ("8", "nobtagmtrk03iso0p07", "nobtagmtrk03iso0p07", BINS_FINE, '--set_alias="sel:pzeta>-20"'),
    ("8", "nobtagmtrk03iso0p08", "nobtagmtrk03iso0p08", BINS_FINE, '--set_alias="sel:pzeta>-20"'),
    ("8", "nobtagmtrk03iso0p08", "nobtagmtrk03iso0p08", BINS_FINE, '--set_alias="sel:pzeta>-20"'),
    ("8", "nobtagmpuw03iso0p05", "nobtagmpuw03iso0p05", BINS_FINE, '--set_alias="sel:pzeta>-20"'),
    ("8", "nobtagmpuw03iso0p1", "nobtagmpuw03iso0p1", BINS_FINE, '--set_alias="sel:pzeta>-20"'),
    ("8", "nobtagmpuw03iso0p15", "nobtagmpuw03iso0p15", BINS_FINE, '--set_alias="sel:pzeta>-20"'),
    ("8", "nobtagmpuw04iso0p05", "nobtagmpuw04iso0p05", BINS_FINE, '--set_alias="sel:pzeta>-20"'),
    ("8", "nobtagmpuw04iso0p1", "nobtagmpuw04iso0p1", BINS_FINE, '--set_alias="sel:pzeta>-20"'),
    ("8", "nobtagmpuw04iso0p15", "nobtagmpuw04iso0p15", BINS_FINE, '--set_alias="sel:pzeta>-20"'),
#    ("8",   "inclusive",    "inclusive",  BINS_FINE, '--set_alias="sel:pzeta>-20"')
    ("8",   "nobtagedb03iso0p1",    "nobtagedb03iso0p1",  BINS_FINE, '--set_alias="sel:pzeta>-20"'),
    ("8",   "nobtagedb03iso0p11",    "nobtagedb03iso0p11",  BINS_FINE, '--set_alias="sel:pzeta>-20"'),
    ("8",   "nobtagedb03iso0p12",    "nobtagedb03iso0p12",  BINS_FINE, '--set_alias="sel:pzeta>-20"'),
    ("8",   "nobtagedb03iso0p13",    "nobtagedb03iso0p13",  BINS_FINE, '--set_alias="sel:pzeta>-20"'),
    ("8",   "nobtagedb03iso0p14",    "nobtagedb03iso0p14",  BINS_FINE, '--set_alias="sel:pzeta>-20"'),
    ("8",   "nobtagedb03iso0p15",    "nobtagedb03iso0p15",  BINS_FINE, '--set_alias="sel:pzeta>-20"'),
    ("8",   "nobtagedb03iso0p16",    "nobtagedb03iso0p16",  BINS_FINE, '--set_alias="sel:pzeta>-20"'),
    ("8",   "nobtagedb03iso0p17",    "nobtagedb03iso0p17",  BINS_FINE, '--set_alias="sel:pzeta>-20"'),
    ("8",   "nobtagedb03iso0p18",    "nobtagedb03iso0p18",  BINS_FINE, '--set_alias="sel:pzeta>-20"'),
    ("8",   "nobtagedb03iso0p19",    "nobtagedb03iso0p19",  BINS_FINE, '--set_alias="sel:pzeta>-20"'),
    ("8",   "nobtagedb03iso0p20",    "nobtagedb03iso0p20",  BINS_FINE, '--set_alias="sel:pzeta>-20"'),
    ("8",   "nobtagedb03iso0p21",    "nobtagedb03iso0p21",  BINS_FINE, '--set_alias="sel:pzeta>-20"'),
    ("8",   "nobtagedb04iso0p1",    "nobtagedb04iso0p1",  BINS_FINE, '--set_alias="sel:pzeta>-20"'),
    ("8",   "nobtagedb04iso0p11",    "nobtagedb04iso0p11",  BINS_FINE, '--set_alias="sel:pzeta>-20"'),
    ("8",   "nobtagedb04iso0p12",    "nobtagedb04iso0p12",  BINS_FINE, '--set_alias="sel:pzeta>-20"'),
    ("8",   "nobtagedb04iso0p13",    "nobtagedb04iso0p13",  BINS_FINE, '--set_alias="sel:pzeta>-20"'),
    ("8",   "nobtagedb04iso0p14",    "nobtagedb04iso0p14",  BINS_FINE, '--set_alias="sel:pzeta>-20"'),
    ("8",   "nobtagedb04iso0p15",    "nobtagedb04iso0p15",  BINS_FINE, '--set_alias="sel:pzeta>-20"'),
    ("8",   "nobtagedb04iso0p16",    "nobtagedb04iso0p16",  BINS_FINE, '--set_alias="sel:pzeta>-20"'),
    ("8",   "nobtagedb04iso0p17",    "nobtagedb04iso0p17",  BINS_FINE, '--set_alias="sel:pzeta>-20"'),
    ("8",   "nobtagedb04iso0p18",    "nobtagedb04iso0p18",  BINS_FINE, '--set_alias="sel:pzeta>-20"'),
    ("8",   "nobtagedb04iso0p19",    "nobtagedb04iso0p19",  BINS_FINE, '--set_alias="sel:pzeta>-20"'),
    ("8",   "nobtagedb04iso0p20",    "nobtagedb04iso0p20",  BINS_FINE, '--set_alias="sel:pzeta>-20"'),
    ("8",   "nobtagedb04iso0p21",    "nobtagedb04iso0p21",  BINS_FINE, '--set_alias="sel:pzeta>-20"'),
    ("8",   "nobtagmdb03iso0p1",    "nobtagmdb03iso0p1",  BINS_FINE, '--set_alias="sel:pzeta>-20"'),
    ("8",   "nobtagmdb03iso0p11",    "nobtagmdb03iso0p11",  BINS_FINE, '--set_alias="sel:pzeta>-20"'),
    ("8",   "nobtagmdb03iso0p12",    "nobtagmdb03iso0p12",  BINS_FINE, '--set_alias="sel:pzeta>-20"'),
    ("8",   "nobtagmdb03iso0p13",    "nobtagmdb03iso0p13",  BINS_FINE, '--set_alias="sel:pzeta>-20"'),
    ("8",   "nobtagmdb03iso0p14",    "nobtagmdb03iso0p14",  BINS_FINE, '--set_alias="sel:pzeta>-20"'),
    ("8",   "nobtagmdb03iso0p15",    "nobtagmdb03iso0p15",  BINS_FINE, '--set_alias="sel:pzeta>-20"'),
    ("8",   "nobtagmdb03iso0p16",    "nobtagmdb03iso0p16",  BINS_FINE, '--set_alias="sel:pzeta>-20"'),
    ("8",   "nobtagmdb03iso0p17",    "nobtagmdb03iso0p17",  BINS_FINE, '--set_alias="sel:pzeta>-20"'),
    ("8",   "nobtagmdb03iso0p18",    "nobtagmdb03iso0p18",  BINS_FINE, '--set_alias="sel:pzeta>-20"'),
    ("8",   "nobtagmdb03iso0p19",    "nobtagmdb03iso0p19",  BINS_FINE, '--set_alias="sel:pzeta>-20"'),
   ("8",   "nobtagmdb03iso0p20",    "nobtagmdb03iso0p20",  BINS_FINE, '--set_alias="sel:pzeta>-20"'),
    ("8",   "nobtagmdb03iso0p21",    "nobtagmdb03iso0p21",  BINS_FINE, '--set_alias="sel:pzeta>-20"'),
    ("8",   "nobtageea03iso0p1",    "nobtageea03iso0p1",  BINS_FINE, '--set_alias="sel:pzeta>-20"'),
    ("8",   "nobtageea03iso0p11",    "nobtageea03iso0p11",  BINS_FINE, '--set_alias="sel:pzeta>-20"'),
    ("8",   "nobtageea03iso0p12",    "nobtageea03iso0p12",  BINS_FINE, '--set_alias="sel:pzeta>-20"'),
    ("8",   "nobtageea03iso0p13",    "nobtageea03iso0p13",  BINS_FINE, '--set_alias="sel:pzeta>-20"'),
    ("8",   "nobtageea03iso0p14",    "nobtageea03iso0p14",  BINS_FINE, '--set_alias="sel:pzeta>-20"'),
    ("8",   "nobtageea03iso0p15",    "nobtageea03iso0p15",  BINS_FINE, '--set_alias="sel:pzeta>-20"'),
    ("8",   "nobtageea03iso0p16",    "nobtageea03iso0p16",  BINS_FINE, '--set_alias="sel:pzeta>-20"'),
    ("8",   "nobtageea03iso0p17",    "nobtageea03iso0p17",  BINS_FINE, '--set_alias="sel:pzeta>-20"'),
    ("8",   "nobtageea03iso0p18",    "nobtageea03iso0p18",  BINS_FINE, '--set_alias="sel:pzeta>-20"'),
    ("8",   "nobtageea03iso0p19",    "nobtageea03iso0p19",  BINS_FINE, '--set_alias="sel:pzeta>-20"'),
    ("8",   "nobtageea03iso0p20",    "nobtageea03iso0p20",  BINS_FINE, '--set_alias="sel:pzeta>-20"'),
    ("8",   "nobtageea03iso0p21",    "nobtageea03iso0p21",  BINS_FINE, '--set_alias="sel:pzeta>-20"'),
    ("8",   "nobtagmea03iso0p1",    "nobtagmea03iso0p1",  BINS_FINE, '--set_alias="sel:pzeta>-20"'),
    ("8",   "nobtagmea03iso0p11",    "nobtagmea03iso0p11",  BINS_FINE, '--set_alias="sel:pzeta>-20"'),
    ("8",   "nobtagmea03iso0p12",    "nobtagmea03iso0p12",  BINS_FINE, '--set_alias="sel:pzeta>-20"'),
    ("8",   "nobtagmea03iso0p13",    "nobtagmea03iso0p13",  BINS_FINE, '--set_alias="sel:pzeta>-20"'),
    ("8",   "nobtagmea03iso0p14",    "nobtagmea03iso0p14",  BINS_FINE, '--set_alias="sel:pzeta>-20"'),
    ("8",   "nobtagmea03iso0p15",    "nobtagmea03iso0p15",  BINS_FINE, '--set_alias="sel:pzeta>-20"'),
    ("8",   "nobtagmea03iso0p16",    "nobtagmea03iso0p16",  BINS_FINE, '--set_alias="sel:pzeta>-20"'),
    ("8",   "nobtagmea03iso0p17",    "nobtagmea03iso0p17",  BINS_FINE, '--set_alias="sel:pzeta>-20"'),
    ("8",   "nobtagmea03iso0p18",    "nobtagmea03iso0p18",  BINS_FINE, '--set_alias="sel:pzeta>-20"'),
    ("8",   "nobtagmea03iso0p19",    "nobtagmea03iso0p19",  BINS_FINE, '--set_alias="sel:pzeta>-20"'),
    ("8",   "nobtagmea03iso0p20",    "nobtagmea03iso0p20",  BINS_FINE, '--set_alias="sel:pzeta>-20"'),
    ("8",   "nobtagmea03iso0p21",    "nobtagmea03iso0p21",  BINS_FINE, '--set_alias="sel:pzeta>-20"'),
    ("8",   "nobtagedb03allchiso0p1",    "nobtagedb03allchiso0p1",  BINS_FINE, '--set_alias="sel:pzeta>-20"'),
    ("8",   "nobtagedb03allchiso0p11",    "nobtagedb03allchiso0p11",  BINS_FINE, '--set_alias="sel:pzeta>-20"'),
    ("8",   "nobtagedb03allchiso0p12",    "nobtagedb03allchiso0p12",  BINS_FINE, '--set_alias="sel:pzeta>-20"'),
    ("8",   "nobtagedb03allchiso0p13",    "nobtagedb03allchiso0p13",  BINS_FINE, '--set_alias="sel:pzeta>-20"'),
    ("8",   "nobtagedb03allchiso0p14",    "nobtagedb03allchiso0p14",  BINS_FINE, '--set_alias="sel:pzeta>-20"'),
    ("8",   "nobtagedb03allchiso0p15",    "nobtagedb03allchiso0p15",  BINS_FINE, '--set_alias="sel:pzeta>-20"'),
    ("8",   "nobtagedb03allchiso0p16",    "nobtagedb03allchiso0p16",  BINS_FINE, '--set_alias="sel:pzeta>-20"'),
    ("8",   "nobtagedb03allchiso0p17",    "nobtagedb03allchiso0p17",  BINS_FINE, '--set_alias="sel:pzeta>-20"'),
    ("8",   "nobtagedb03allchiso0p18",    "nobtagedb03allchiso0p18",  BINS_FINE, '--set_alias="sel:pzeta>-20"'),
    ("8",   "nobtagedb03allchiso0p19",    "nobtagedb03allchiso0p19",  BINS_FINE, '--set_alias="sel:pzeta>-20"'),
    ("8",   "nobtagedb03allchiso0p20",    "nobtagedb03allchiso0p20",  BINS_FINE, '--set_alias="sel:pzeta>-20"'),
    ("8",   "nobtagedb03allchiso0p21",    "nobtagedb03allchiso0p21",  BINS_FINE, '--set_alias="sel:pzeta>-20"'),
    ("8",   "nobtagmdb03allchiso0p1",    "nobtagmdb03allchiso0p1",  BINS_FINE, '--set_alias="sel:pzeta>-20"'),
    ("8",   "nobtagmdb03allchiso0p11",    "nobtagmdb03allchiso0p11",  BINS_FINE, '--set_alias="sel:pzeta>-20"'),
    ("8",   "nobtagmdb03allchiso0p12",    "nobtagmdb03allchiso0p12",  BINS_FINE, '--set_alias="sel:pzeta>-20"'),
    ("8",   "nobtagmdb03allchiso0p13",    "nobtagmdb03allchiso0p13",  BINS_FINE, '--set_alias="sel:pzeta>-20"'),
    ("8",   "nobtagmdb03allchiso0p14",    "nobtagmdb03allchiso0p14",  BINS_FINE, '--set_alias="sel:pzeta>-20"'),
    ("8",   "nobtagmdb03allchiso0p15",    "nobtagmdb03allchiso0p15",  BINS_FINE, '--set_alias="sel:pzeta>-20"'),
    ("8",   "nobtagmdb03allchiso0p16",    "nobtagmdb03allchiso0p16",  BINS_FINE, '--set_alias="sel:pzeta>-20"'),
    ("8",   "nobtagmdb03allchiso0p17",    "nobtagmdb03allchiso0p17",  BINS_FINE, '--set_alias="sel:pzeta>-20"'),
    ("8",   "nobtagmdb03allchiso0p18",    "nobtagmdb03allchiso0p18",  BINS_FINE, '--set_alias="sel:pzeta>-20"'),
    ("8",   "nobtagmdb03allchiso0p19",    "nobtagmdb03allchiso0p19",  BINS_FINE, '--set_alias="sel:pzeta>-20"'),
    ("8",   "nobtagmdb03allchiso0p20",    "nobtagmdb03allchiso0p20",  BINS_FINE, '--set_alias="sel:pzeta>-20"'),
    ("8",   "nobtagmdb03allchiso0p21",    "nobtagmdb03allchiso0p21",  BINS_FINE, '--set_alias="sel:pzeta>-20"'),
    ("8",   "nobtagedb04allchiso0p1",    "nobtagedb04allchiso0p1",  BINS_FINE, '--set_alias="sel:pzeta>-20"'),
    ("8",   "nobtagedb04allchiso0p11",    "nobtagedb04allchiso0p11",  BINS_FINE, '--set_alias="sel:pzeta>-20"'),
    ("8",   "nobtagedb04allchiso0p12",    "nobtagedb04allchiso0p12",  BINS_FINE, '--set_alias="sel:pzeta>-20"'),
    ("8",   "nobtagedb04allchiso0p13",    "nobtagedb04allchiso0p13",  BINS_FINE, '--set_alias="sel:pzeta>-20"'),
    ("8",   "nobtagedb04allchiso0p14",    "nobtagedb04allchiso0p14",  BINS_FINE, '--set_alias="sel:pzeta>-20"'),
    ("8",   "nobtagedb04allchiso0p15",    "nobtagedb04allchiso0p15",  BINS_FINE, '--set_alias="sel:pzeta>-20"'),
    ("8",   "nobtagedb04allchiso0p16",    "nobtagedb04allchiso0p16",  BINS_FINE, '--set_alias="sel:pzeta>-20"'),
    ("8",   "nobtagedb04allchiso0p17",    "nobtagedb04allchiso0p17",  BINS_FINE, '--set_alias="sel:pzeta>-20"'),
    ("8",   "nobtagedb04allchiso0p18",    "nobtagedb04allchiso0p18",  BINS_FINE, '--set_alias="sel:pzeta>-20"'),
   ("8",   "nobtagedb04allchiso0p19",    "nobtagedb04allchiso0p19",  BINS_FINE, '--set_alias="sel:pzeta>-20"'),
   ("8",   "nobtagedb04allchiso0p20",    "nobtagedb04allchiso0p20",  BINS_FINE, '--set_alias="sel:pzeta>-20"'),
   ("8",   "nobtagedb04allchiso0p21",    "nobtagedb04allchiso0p21",  BINS_FINE, '--set_alias="sel:pzeta>-20"'),
    ("8",   "nobtagmdb04allchiso0p1",    "nobtagmdb04allchiso0p1",  BINS_FINE, '--set_alias="sel:pzeta>-20"'),
    ("8",   "nobtagmdb04allchiso0p11",    "nobtagmdb04allchiso0p11",  BINS_FINE, '--set_alias="sel:pzeta>-20"'),
    ("8",   "nobtagmdb04allchiso0p12",    "nobtagmdb04allchiso0p12",  BINS_FINE, '--set_alias="sel:pzeta>-20"'),
    ("8",   "nobtagmdb04allchiso0p13",    "nobtagmdb04allchiso0p13",  BINS_FINE, '--set_alias="sel:pzeta>-20"'),
    ("8",   "nobtagmdb04allchiso0p14",    "nobtagmdb04allchiso0p14",  BINS_FINE, '--set_alias="sel:pzeta>-20"'),
    ("8",   "nobtagmdb04allchiso0p15",    "nobtagmdb04allchiso0p15",  BINS_FINE, '--set_alias="sel:pzeta>-20"'),
    ("8",   "nobtagmdb04allchiso0p16",    "nobtagmdb04allchiso0p16",  BINS_FINE, '--set_alias="sel:pzeta>-20"'),
    ("8",   "nobtagmdb04allchiso0p17",    "nobtagmdb04allchiso0p17",  BINS_FINE, '--set_alias="sel:pzeta>-20"'),
    ("8",   "nobtagmdb04allchiso0p18",    "nobtagmdb04allchiso0p18",  BINS_FINE, '--set_alias="sel:pzeta>-20"'),
    ("8",   "nobtagmdb04allchiso0p19",    "nobtagmdb04allchiso0p19",  BINS_FINE, '--set_alias="sel:pzeta>-20"'),
    ("8",   "nobtagmdb04allchiso0p20",    "nobtagmdb04allchiso0p20",  BINS_FINE, '--set_alias="sel:pzeta>-20"'),
    ("8",   "nobtagmdb04allchiso0p21",    "nobtagmdb04allchiso0p21",  BINS_FINE, '--set_alias="sel:pzeta>-20"'),
    ("8",   "nobtagmdb04iso0p1",    "nobtagmdb04iso0p1",  BINS_FINE, '--set_alias="sel:pzeta>-20"'),
    ("8",   "nobtagmdb04iso0p11",    "nobtagmdb04iso0p11",  BINS_FINE, '--set_alias="sel:pzeta>-20"'),
    ("8",   "nobtagmdb04iso0p12",    "nobtagmdb04iso0p12",  BINS_FINE, '--set_alias="sel:pzeta>-20"'),
    ("8",   "nobtagmdb04iso0p13",    "nobtagmdb04iso0p13",  BINS_FINE, '--set_alias="sel:pzeta>-20"'),
    ("8",   "nobtagmdb04iso0p14",    "nobtagmdb04iso0p14",  BINS_FINE, '--set_alias="sel:pzeta>-20"'),
    ("8",   "nobtagmdb04iso0p15",    "nobtagmdb04iso0p15",  BINS_FINE, '--set_alias="sel:pzeta>-20"'),
    ("8",   "nobtagmdb04iso0p16",    "nobtagmdb04iso0p16",  BINS_FINE, '--set_alias="sel:pzeta>-20"'),
    ("8",   "nobtagmdb04iso0p17",    "nobtagmdb04iso0p17",  BINS_FINE, '--set_alias="sel:pzeta>-20"'),
    ("8",   "nobtagmdb04iso0p18",    "nobtagmdb04iso0p18",  BINS_FINE, '--set_alias="sel:pzeta>-20"'),
    ("8",   "nobtagmdb04iso0p19",    "nobtagmdb04iso0p19",  BINS_FINE, '--set_alias="sel:pzeta>-20"'),
    ("8",   "nobtagmdb04iso0p20",    "nobtagmdb04iso0p20",  BINS_FINE, '--set_alias="sel:pzeta>-20"'),
    ("8",   "nobtagmdb04iso0p21",    "nobtagmdb04iso0p21",  BINS_FINE, '--set_alias="sel:pzeta>-20"'),
    ("8",   "nobtagmtrk03iso0p1",    "nobtagmtrk03iso0p1",  BINS_FINE, '--set_alias="sel:pzeta>-20"'),
    ("8",   "nobtagmtrk03iso0p11",    "nobtagmtrk03iso0p11",  BINS_FINE, '--set_alias="sel:pzeta>-20"'),
    ("8",   "nobtagmtrk03iso0p12",    "nobtagmtrk03iso0p12",  BINS_FINE, '--set_alias="sel:pzeta>-20"'),
    ("8",   "nobtagmtrk03iso0p13",    "nobtagmtrk03iso0p13",  BINS_FINE, '--set_alias="sel:pzeta>-20"'),
    ("8",   "nobtagmtrk03iso0p14",    "nobtagmtrk03iso0p14",  BINS_FINE, '--set_alias="sel:pzeta>-20"'),
    ("8",   "nobtagmtrk03iso0p15",    "nobtagmtrk03iso0p15",  BINS_FINE, '--set_alias="sel:pzeta>-20"'),
    ("8",   "nobtagmtrk03iso0p16",    "nobtagmtrk03iso0p16",  BINS_FINE, '--set_alias="sel:pzeta>-20"'),
    ("8",   "nobtagmtrk03iso0p17",    "nobtagmtrk03iso0p17",  BINS_FINE, '--set_alias="sel:pzeta>-20"'),
    ("8",   "nobtagmtrk03iso0p18",    "nobtagmtrk03iso0p18",  BINS_FINE, '--set_alias="sel:pzeta>-20"'),
    ("8",   "nobtagmtrk03iso0p19",    "nobtagmtrk03iso0p19",  BINS_FINE, '--set_alias="sel:pzeta>-20"'),
    ("8",   "nobtagmtrk03iso0p20",    "nobtagmtrk03iso0p20",  BINS_FINE, '--set_alias="sel:pzeta>-20"'),
    ("8",   "nobtagmtrk03iso0p21",    "nobtagmtrk03iso0p21",  BINS_FINE, '--set_alias="sel:pzeta>-20"'),

#    ("5",   "vbf",          "vbf",        BINS,      ' --syst_tau_scale="CMS_scale_e_'+COM+'TeV"'),
#    ("0",   "0jet_low",     "0jet_low",   BINS_FINE, ' --syst_tau_scale="CMS_scale_e_'+COM+'TeV"'),
#    ("1",   "0jet_high",    "0jet_high",  BINS_FINE, (
#      (' --syst_tau_scale="CMS_scale_e_highpt_'+COM+'TeV"' if COM=='8' else ' --syst_tau_scale="CMS_scale_e_'+COM+'TeV"')+
#       ' --syst_fakes_shape="CMS_htt_FakeShape_em_0jet_high_'+COM+'TeV"')),
#    ("2",   "1jet_low",     "boost_low",  BINS_FINE, ' --syst_tau_scale="CMS_scale_e_'+COM+'TeV"'),
#    ("3",   "1jet_high",    "boost_high", BINS_FINE, ' --syst_tau_scale="CMS_scale_e_highpt_'+COM+'TeV"' if COM=='8' else ' --syst_tau_scale="CMS_scale_e_'+COM+'TeV"')
  ]
  bkg_schemes = {
    'et' : 'et_default',
    'mt' : 'mt_with_zmm',
    'em' : 'em_default',
    'tt' : 'tt_default'
  }
  sig_scheme = 'run2_mssm'
  ANA = 'mssm'

cat_schemes = {
  'et' : scheme_et,
  'mt' : scheme_mt,
  'em' : scheme_em,
  'tt' : scheme_tt
}



plots = [ 
  ('m_vis'  , 'M_{#tau#tau}^{vis} [GeV]'  , '-mvis' , "60", "2500" if ANA=='mssm' else "125"),
  ('m_sv'   , 'M_{#tau#tau} [GeV]'        , ''      , "100", "2500"  if ANA=='mssm' else "125")
 ]
if options.mvis: del plots[1]
if options.svfit: del plots[0]

for pl in plots:
  var     = pl[0]
  xlab    = pl[1]
  dc_app  = pl[2]
  blind_min = pl[3]
  blind_max = pl[4]
  for ch in channels:
    scheme = cat_schemes[ch]
    bkg_scheme = bkg_schemes[ch]
    for x in scheme:
      cat_num = x[0]
      cat_str = x[1]
      dc      = x[2]
      bin     = x[3]
      opts    = x[4]
      extra = options.extra + extra_global + extra_channel[ch] + opts

      os.system('$CMSSW_BASE/src/UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/bin/HiggsTauTauPlot5 --cfg=%(CFG)s --channel=%(ch)s'
        ' --method=%(cat_num)s --cat=%(cat_str)s --datacard=%(dc)s'
        ' --var="%(var)s%(bin)s" --norm_bins=true '
        ' --background_scheme=%(bkg_scheme)s --signal_scheme=%(sig_scheme)s'
        ' --x_axis_label="%(xlab)s" --y_axis_label="dN/dm_{#tau#tau} [1/GeV]"'
        ' --blind=%(BLIND)s --x_blind_min=%(blind_min)s --x_blind_max=%(blind_max)s --verbose=false'
        ' --paramfile=%(PARAMS)s --folder=%(FOLDER)s %(extra)s' % vars())
    os.system('hadd -f htt_%(ch)s.inputs-%(ANA)s-%(COM)sTeV%(dc_app)s%(output)s.root datacard_%(var)s_*_%(ch)s_%(YEAR)s.root' % vars())
    os.system('rm datacard_%(var)s_*_%(ch)s_%(YEAR)s.root' % vars())

