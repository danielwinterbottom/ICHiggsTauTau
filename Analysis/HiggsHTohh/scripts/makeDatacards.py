#!/usr/bin/env python

import sys
from optparse import OptionParser
import os

CHANNELS= ['et', 'mt', 'em']

def validate_channel(channel):
  assert channel in CHANNELS, 'Error, channel %(channel)s duplicated or unrecognised' % vars()
  CHANNELS.remove(channel)

def split_callback(option, opt, value, parser):
  setattr(parser.values, option.dest, value.split(','))

parser = OptionParser()
parser.add_option("-p","--parameterfile", dest="params", type='string',
                  help="Specify the paramter file containing the luminosity and cross section information.")
parser.add_option("--cfg", dest="config", type='string',
                  help="The config file that will be passed to HiggsHTohhPlot.")
parser.add_option("-i","--input", dest="folder", type='string',
                  help="The input folder, containing the output of HiggsTauTau")
parser.add_option("-o","--output", dest="output", type='string', default='',
                  help="The name that will be appended to the datacard inputs.")
parser.add_option("-c", "--channels", dest="channels", type='string', action='callback',callback=split_callback,
                  help="A comma separted list of channels to process.  Supported channels: %(CHANNELS)s" % vars())
parser.add_option("--blind", dest="blind", action='store_true', default=False,
                  help="blind data")
parser.add_option("--extra", dest="extra", type='string', default='',
                  help="Extra command line options, applied to every datacard")
parser.add_option("-s", "--scheme", dest="scheme", type='string', default='HTohh',
                  help="datacard scheme")
parser.add_option("--mvis", dest="mvis", action='store_true', default=False,
                  help="Make inputs for visible mass.")
parser.add_option("--svfit", dest="svfit", action='store_true', default=False,
                  help="Make inputs for svfit mass.")
parser.add_option("--dijet", dest="dijet", action='store_true', default=False,
                  help="Make inputs for dijet mass.")
parser.add_option("--fullH", dest="fullH", action='store_true', default=False,
                  help="Make inputs for reconstructed H mass.")
parser.add_option("-e", dest="energy", type='string', default='8',
                  help="The C.O.M. energy is written into the datacard name, default is 8")

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

if not (options.mvis or options.svfit or options.dijet or options.fullH) :
  print 'Error, please specify distribution to plot.'
  sys.exit(1)


PARAMS=options.params
CFG=options.config
FOLDER = options.folder
BLIND = options.blind
BLIND = "false"
if options.blind: BLIND = "true"
COM = options.energy
YEAR = ('2012' if COM=='8' else '2011')

########## Set up schemes and options

#### Always apply these options:
extra_global = ' --fix_empty_hists="ggHTohh.*,ggAToZh.*"'

#### Apply these options for specific channels
extra_channel = {
  "et" : ' --fix_empty_bins="QCD,ZL,ZJ,ZLL,W"  --fix_negative_bins="QCD,QCD.*Up,QCD.*Down" --syst_tau_scale="CMS_scale_t_etau_'+COM+'TeV"',
  "mt" : ' --fix_empty_bins="QCD,ZL,ZJ,ZLL,W"  --fix_negative_bins="QCD,QCD.*Up,QCD.*Down" --syst_tau_scale="CMS_scale_t_mutau_'+COM+'TeV"',
  "em" : ' --fix_empty_bins="Fakes" --fix_negative_bins="Fakes,Fakes.*Up,Fakes.*Down"'
}

#### Set which distribution to include in datacards (note that because of different binnings it is only possible to run one at a time)
plots = [ 
  ('m_vis'  , 'M_{#tau#tau}^{vis} [GeV]'  , '-mvis' ,   "100"    ,"150"),
  ('m_sv'   , 'M_{#tau#tau} [GeV]'        , ''      ,   "100"   ,"150"),
  ('prebjet_mjj'   , 'M_{jj} [GeV]'        , ''      ,   "80"   ,"160"),
  ('mjj_tt'   , 'M_{#tau#tau+jj} [GeV]'        , ''      ,   "200"   ,"600")
 ]
if options.mvis: 
    plots = [plots[0]]
if options.svfit:
    plots = [plots[1]]
if options.dijet: 
    plots = [plots[2]]
if options.fullH: 
    plots = [plots[3]]

#################################################################
#### New HTohh scheme
#################################################################

if options.scheme == 'HTohh':
  if options.mvis or options.svfit:  
    BINS_FINE="[0,10,20,30,40,50,60,70,80,90,100,110,120,130,140,150,160,170,180,190,200,225,250,275,300,325,350]"
    BINS="[0,20,40,60,80,100,120,140,160,180,200,250,300,350]"
  elif options.dijet :
    BINS_FINE="(30,0,600)"
    BINS="(15,0,600)"
  elif options.fullH:
    BINS_FINE="(20,0,1000)"
    BINS="(20,0,1000)"
  scheme_et = [
    ("8",    "inclusive",   "inclusive",  BINS_FINE,  (
      ' --syst_w_fake_rate="CMS_htt_WShape_etau_inclusive_'+COM+'TeV"')),
    ("0",   "0tag",      "0tag",     BINS_FINE, ( 
      ' --syst_w_fake_rate="CMS_htt_WShape_etau_0tag_'+COM+'TeV"'
      ' --syst_qcd_shape="CMS_htt_QCDShape_etau_0tag_'+COM+'TeV:50:1.0:0.10"')),
    ("1",   "1tag",      "1tag",     BINS_FINE,  (
      ' --syst_w_fake_rate="CMS_htt_WShape_etau_1tag_'+COM+'TeV"'
      ' --syst_qcd_shape="CMS_htt_QCDShape_etau_1tag_'+COM+'TeV:50:1.0:0.10"'
      ' --sub_ztt_top_frac=0.015')),
    ("2",   "2tag",        "2tag",       BINS,  (
      ' --syst_w_fake_rate="CMS_htt_WShape_etau_2tag_'+COM+'TeV"'
      ' --syst_qcd_shape="CMS_htt_QCDShape_etau_2tag_'+COM+'TeV:50:1.0:0.10"'
      ' --sub_ztt_top_frac=0.015'))
  ]
  scheme_mt = [
    ("8",    "inclusive",   "inclusive",  BINS_FINE, (
      ' --syst_w_fake_rate="CMS_htt_WShape_mutau_inclusive_'+COM+'TeV"')),
    ("0",   "0tag",      "0tag",     BINS_FINE,  (
      ' --syst_w_fake_rate="CMS_htt_WShape_mutau_0tag_'+COM+'TeV"'
      ' --syst_qcd_shape="CMS_htt_QCDShape_mutau_0tag_'+COM+'TeV:50:1.1:0.10"')),
    ("1",   "1tag",      "1tag",     BINS_FINE,  (
      ' --syst_w_fake_rate="CMS_htt_WShape_mutau_1tag_'+COM+'TeV"'
      ' --syst_qcd_shape="CMS_htt_QCDShape_mutau_1tag_'+COM+'TeV:50:1.0:0.10"'
      ' --sub_ztt_top_frac=0.015')),
    ("2",   "2tag",        "2tag",       BINS,  ( 
      ' --syst_w_fake_rate="CMS_htt_WShape_mutau_2tag_'+COM+'TeV"'
      ' --syst_qcd_shape="CMS_htt_QCDShape_mutau_2tag_'+COM+'TeV:50:1.0:0.10"'
      ' --sub_ztt_top_frac=0.015'))
  ]
  scheme_em = [
    ("8",    "inclusive",   "inclusive",  BINS_FINE,  (
      ' --syst_tquark="CMS_htt_TTbarShape_em_inclusive_'+COM+'TeV"'
      ' --syst_tau_scale="CMS_scale_e_'+COM+'TeV"')),
    ("0",   "0tag",      "0tag",     BINS_FINE, (
      ' --syst_tquark="CMS_htt_TTbarShape_em_0tag_'+COM+'TeV"'
      ' --syst_fakes_os_ss_shape="CMS_htt_FakeShape_em_0tag_'+COM+'TeV"'
      ' --syst_tau_scale="CMS_scale_e_'+COM+'TeV"')),
    ("1",   "1tag",      "1tag",     BINS_FINE,  (
      ' --syst_tquark="CMS_htt_TTbarShape_em_1tag_'+COM+'TeV"'
      ' --syst_qcd_shape="CMS_htt_FakeShape_em_1tag_'+COM+'TeV:50:1.1:0.10"'
      ' --syst_tau_scale="CMS_scale_e_'+COM+'TeV"'
      ' --sub_ztt_top_frac=0.015')),
    ("2",   "2tag",        "2tag",       BINS, ( 
      ' --syst_tquark="CMS_htt_TTbarShape_em_2tag_'+COM+'TeV"'
      ' --syst_qcd_shape="CMS_htt_FakeShape_em_2tag_'+COM+'TeV:50:1.1:0.10"'
      ' --syst_tau_scale="CMS_scale_e_'+COM+'TeV"'
      ' --sub_ztt_top_frac=0.015'))
  ]
  bkg_schemes = {
    'et' : 'et_default',
    'mt' : 'mt_with_zmm',
    'em' : 'em_default'
  }
  sig_scheme = 'mssm_nostack'
  ANA = 'Hhh'
  extra_channel["et"] += ' --set_alias="sel:mt_1<30."'
  extra_channel["mt"] += ' --set_alias="sel:mt_1<30."'
  extra_channel["et"] += ' --syst_zl_shift="CMS_htt_ZLScale_etau_'+COM+'TeV:1.02:0.98"'
  extra_channel["mt"] += ' --syst_zl_shift="CMS_htt_ZLScale_mutau_'+COM+'TeV:1.02:0.98"'

#################################################################

cat_schemes = {
   'et' : scheme_et,
   'mt' : scheme_mt,
   'em' : scheme_em
}


   

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

      os.system('$CMSSW_BASE/src/UserCode/ICHiggsTauTau/Analysis/HiggsHTohh/bin/HiggsHTohhPlot --cfg=%(CFG)s --channel=%(ch)s'
        #' --method=%(cat_num)s --cat=%(cat_str)s --datacard=%(dc)s'
        ' --method=8 --cat=%(cat_str)s --datacard=%(dc)s'
        ' --var="%(var)s%(bin)s" --norm_bins=true '
        ' --background_scheme=%(bkg_scheme)s --signal_scheme=%(sig_scheme)s'
        ' --x_axis_label="%(xlab)s" --y_axis_label="dN/dm_{#tau#tau} [1/GeV]"'
        ' --blind=%(BLIND)s --x_blind_min=%(blind_min)s --x_blind_max=%(blind_max)s --verbosity 0'
        ' --paramfile=%(PARAMS)s --folder=%(FOLDER)s %(extra)s' % vars())
    os.system('hadd -f htt_%(ch)s.inputs-%(ANA)s-%(COM)sTeV%(dc_app)s%(output)s.root datacard_%(var)s_*_%(ch)s_%(YEAR)s.root' % vars())
    os.system('rm datacard_%(var)s_*_%(ch)s_%(YEAR)s.root' % vars())

