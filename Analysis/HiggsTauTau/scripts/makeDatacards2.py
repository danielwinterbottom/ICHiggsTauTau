#!/usr/bin/env python

import sys
from optparse import OptionParser
import os

CHANNELS= ['et', 'mt', 'em','mtmet']
SCHEMES = ['old_sm','old_mssm']

def validate_channel(channel):
  assert channel in CHANNELS, 'Error, channel %(channel)s duplicated or unrecognised' % vars()
  CHANNELS.remove(channel)

def split_callback(option, opt, value, parser):
  setattr(parser.values, option.dest, value.split(','))

parser = OptionParser()
parser.add_option("-p","--parameterfile", dest="params", type='string',
                  help="Specify the paramter file containing the luminosity and cross section information.")
parser.add_option("--cfg", dest="config", type='string',
                  help="The config file that will be passed to HiggsTauTauPlot3.")
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
parser.add_option("-s", "--scheme", dest="scheme_str", type='string', default='old_sm',
                  help="datacard scheme")
parser.add_option("--mvis", dest="mvis", action='store_true', default=False,
                  help="Only make inputs for visible mass, no svfit.")
parser.add_option("-e", dest="energy", type='string', default='8',
                  help="The C.O.M. energy is written into the datacard name, default is 8")

(options, args) = parser.parse_args()

output = options.output
if output: 
  output = '-'+output
  print 'Appending "%(output)s" to datacard outputs' % vars()

channels = options.channels
# scales = SCALES
# if options.central: scales = ['0']

# print 'Processing energy scales: %(scales)s' % vars()
print 'Processing channels:      %(channels)s' % vars()

### Do some validation of the input
helpMsg = "Run 'datacard_inputs.py -h' for help."

if not channels:
  print 'Error, no channels specified. ' + helpMsg
  sys.exit(1)

for channel in channels:
  validate_channel(channel)


PARAMS=options.params
CFG=options.config
folder = options.folder
blind = options.blind
blind = "false"
if options.blind: blind = "true"
COM = options.energy

if options.scheme_str == 'old_sm':
  BINS_FINE="0,10,20,30,40,50,60,70,80,90,100,110,120,130,140,150,160,170,180,190,200,225,250,275,300,325,350"
  BINS="0,20,40,60,80,100,120,140,160,180,200,250,300,350"
  scheme = [
    ("8",   "inclusive",    "inclusive",  BINS_FINE, ""),
    ("5",   "vbf",          "vbf",        BINS, ""),
    ("0",   "0jet_low",     "0jet_low",   BINS_FINE, ""),
    ("1",   "0jet_high",    "0jet_high",  BINS_FINE, ""),
    ("2",   "1jet_low",     "boost_low",  BINS_FINE, ""),
    ("3",   "1jet_high",    "boost_high", BINS_FINE, "")
  ]
  bkg_schemes = {
    'et' : 'et_default',
    'mt' : 'mt_with_zmm',
    'em' : 'em_default'
  }
  sig_scheme = 'sm_default'
  ANA = 'sm'

if options.scheme_str == 'new_sm_et_mt':
  BINS_FINE="0,10,20,30,40,50,60,70,80,90,100,110,120,130,140,150,160,170,180,190,200,225,250,275,300,325,350"
  BINS="0,20,40,60,80,100,120,140,160,180,200,250,300,350"
  scheme = [
    ("8",   "inclusive",        "inclusive",    BINS_FINE, ""),
    ("5",   "new_vbf",          "vbf",          BINS, ""),
    ("5",   "new_vbf_tight",    "vbf_tight",    BINS, ""),
    ("0",   "new_0jet_low",     "0jet_low",     BINS_FINE, ""),
    ("1",   "new_0jet_medium",  "0jet_medium",  BINS_FINE, ""),
    ("1",   "new_0jet_high",    "0jet_high",    BINS_FINE, ""),
    ("2",   "new_1jet_medium",  "boost_medium", BINS_FINE, ""),
    ("3",   "new_1jet_high",    "boost_high",   BINS_FINE, ""),
    ("3",   "new_v1jet_high",   "vboost_high",  BINS_FINE, "")
  ]
  bkg_schemes = {
    'et' : 'et_default',
    'mt' : 'mt_with_zmm',
    'em' : 'em_default'
  }
  sig_scheme = 'sm_default'
  ANA = 'sm'

if options.scheme_str == 'old_mssm' :
  BINS_FINE="0,10,20,30,40,50,60,70,80,90,100,110,120,130,140,150,160,170,180,190,200,225,250,275,300,325,350,400,500,700,1000,1500"
  BINS="0,20,40,60,80,100,120,140,160,180,200,250,300,350,400,500,700,1000,1500"
  scheme = [
    ("8",    "inclusive",   "inclusive",  BINS_FINE, ""),
    ("11",   "nobtag",      "nobtag",     BINS_FINE, ""),
    ("12",   "btag",        "btag",       BINS, "")
  ]
  bkg_schemes = {
    'et' : 'et_default',
    'mt' : 'mt_with_zmm',
    'em' : 'em_default'
  }
  sig_scheme = 'mssm_default'
  ANA = 'mssm'


plots = [ ('m_vis','M_{#tau#tau}^{vis} [GeV]', '-mvis') ]
if not options.mvis: plots += ('m_sv','M_{#tau#tau} [GeV]', ''),

for pl in plots:
  dc_app  = pl[2]
  for ch in channels:
    for x in scheme:
      cat_num = x[0]
      cat_str = x[1]
      dc      = x[2]
      bin     = x[3]
      var     = pl[0]
      xlab    = pl[1]
      bkg_scheme = bkg_schemes[ch]
      extra = options.extra

      # Always fix signal shapes with no entries
      extra += ' --fix_empty_hists="ggH.*,qqH.*,VH.*,bbH.*"'

      # For the btag method subtract embedded contamination
      # from ttbar
      if cat_num == '12':
        extra += ' --sub_ztt_top_frac=0.015'

      if ch == 'et':
        extra += ' --fix_empty_bins="QCD,ZL,ZJ,ZLL,W"'
        extra += ' --syst_tau_scale="CMS_scale_t_etau_'+COM+'TeV"'
        # To increase statistics, don't apply the os selection for the
        # W shape when using the boost_high method
        if cat_num == '3' and options.scheme_str == 'old_sm':
          extra += ' --set_alias="w_shape_sel:mt_1<20."'

      if ch == 'mt': 
        extra += ' --fix_empty_bins="QCD,ZL,ZJ,ZLL,W"'
        extra += ' --syst_tau_scale="CMS_scale_t_mutau_'+COM+'TeV"'
        # For the boost_low method do the QCD low-mass shape uncertainty
        if cat_num == '2': 
          extra += ' --syst_qcd_shape="CMS_htt_QCDShape_mutau_'+dc+'_'+COM+'TeV"'
        # To increase statistics, don't apply the os selection for the
        # W shape when using the boost_high method
        if cat_num == '3' and options.scheme_str == 'old_sm':
          extra += ' --set_alias="w_shape_sel:mt_1<20."'

      if ch == 'em':
        extra += ' --fix_empty_bins="Fakes"'
        if COM=='8' and (cat_num in ['1', '3']): 
          extra += ' --syst_tau_scale="CMS_scale_e_highpt_'+COM+'TeV"'
        else:
          extra += ' --syst_tau_scale="CMS_scale_e_'+COM+'TeV"'
        if cat_num == '1':
          extra += ' --syst_fakes_shape="CMS_htt_FakeShape_em_0jet_high_'+COM+'TeV"'

      if options.scheme_str == 'old_mssm':
        extra += ' --add_extra_binning="(150,0,1500):_fine_binning"'
        extra += ' --add_sm_background="125"'

      if options.scheme_str == 'new_sm_et_mt' and ch in ['et','mt']:
        extra += ' --set_alias="w_extrp_os_sig_sel:os && mt_1<30.,os_sel:os && mt_1<30.,vbf_w_extrp_os_sig_sel:os && mt_1<30.,w_shape_sel:os && mt_1<30.,qcd_sdb_sel:!os && mt_1<30.,w_extrp_ss_sig_sel:!os && mt_1<30."'
        if cat_num == '3':
          extra = extra.replace('w_shape_sel:os && mt_1<30.','w_shape_sel:mt_1<30.')

      os.system('./bin/HiggsTauTauPlot4 --cfg=%(CFG)s --channel=%(ch)s'
        ' --method=%(cat_num)s --cat=%(cat_str)s --datacard=%(dc)s'
        ' --var="%(var)s[%(bin)s]" --norm_bins=true '
        ' --background_scheme=%(bkg_scheme)s --signal_scheme=%(sig_scheme)s'
        ' --x_axis_label="%(xlab)s" --y_axis_label="dN/dm_{#tau#tau} [1/GeV]"'
        ' --blind=%(blind)s --x_blind_min=50 --x_blind_max=110 --verbose=false'
        ' --paramfile=%(PARAMS)s --folder=%(folder)s %(extra)s' % vars())
    os.system('hadd -f htt_%(ch)s.inputs-%(ANA)s-%(COM)sTeV%(dc_app)s%(output)s.root datacard_*.root' % vars())
    os.system('rm datacard_*.root')







