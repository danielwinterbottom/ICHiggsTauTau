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
                  help="Extra command line options")
parser.add_option("-s", "--scheme", dest="scheme_str", type='string', default='old_sm',
                  help="datacard scheme")
# parser.add_option("--central", dest="central", action='store_true', default=False,
#                   help="Only process the central energy scale inputs.")
# parser.add_option("--mvis", dest="mvis", action='store_true', default=False,
#                   help="Only make inputs for visible mass, no svfit.")
parser.add_option("--mssm", dest="mssm", action='store_true', default=False,
                  help="Make datacards for the MSSM analysis")
parser.add_option("-e", dest="energy", type='string', default='8',
                  help="The C.O.M. energy is written into the datacard name, default is 8")
# parser.add_option("--svfit_plot", dest="svfit_plot", type='string', default='',
#                   help="Override the default svfit plot")
# parser.add_option("--svfit_vbf_plot", dest="svfit_vbf_plot", type='string', default='',
#                   help="Override the default svfit vbf category plot")
# parser.add_option("--mvis_plot", dest="mvis_plot", type='string', default='',
#                   help="Override the default mvis plot")
# parser.add_option("--mvis_vbf_plot", dest="mvis_vbf_plot", type='string', default='',
#                   help="Override the default mvis vbf category plot")


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
    'mt' : 'mt_default',
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
    'mt' : 'mt_default',
    'em' : 'em_default'
  }
  sig_scheme = 'mssm_default'
  ANA = 'mssm'

  

for ch in channels:
  for x in scheme:
    cat_num = x[0]
    cat_str = x[1]
    dc      = x[2]
    bin     = x[3]
    bkg_scheme = bkg_schemes[ch]
    extra = options.extra

    if ch == 'et':
      extra += ' --fix_empty_bins="QCD,ZL,ZJ,ZLL,W"'
      extra += ' --syst_tau_scale="CMS_scale_t_etau_'+COM+'TeV"'
    
    if ch == 'mt': 
      extra += ' --fix_empty_bins="QCD,ZL,ZJ,ZLL,W"'
      extra += ' --syst_tau_scale="CMS_scale_t_mutau_'+COM+'TeV"'
      if cat_num == '2': 
        extra += ' --syst_qcd_shape="CMS_htt_QCDShape_mutau_boost_low_'+COM+'TeV"'
    
    if ch == 'em':
      extra += ' --fix_empty_bins="Fakes"'
      if COM=='8' and (cat_num in ['1', '3']): 
        extra += ' --syst_tau_scale="CMS_scale_e_highpt_'+COM+'TeV"'
      else:
        extra += ' --syst_tau_scale="CMS_scale_e_'+COM+'TeV"'
      if cat_num == '1':
        extra += ' --syst_fakes_shape="CMS_htt_FakeShape_em_0jet_high_'+COM+'TeV"'

    # if options.scheme_str == 'old_mssm':
    extra += ' --add_extra_binning="(150,0,1500):_fine_binning"'

    os.system('./bin/HiggsTauTauPlot4 --cfg=%(CFG)s --channel=%(ch)s'
      ' --method=%(cat_num)s --cat=%(cat_str)s --datacard=%(dc)s'
      ' --var="m_vis[%(bin)s]" --norm_bins=true '
      ' --background_scheme=%(bkg_scheme)s --signal_scheme=%(sig_scheme)s'
      ' --x_axis_label="m_{#tau#tau} [GeV]"'
      ' --blind=%(blind)s --x_blind_min=50 --x_blind_max=110 --verbose=false'
      ' --paramfile=%(PARAMS)s --folder=%(folder)s %(extra)s' % vars())
    print '\n'


# ANA = 'sm'
# svfit_plot="m_sv_sm_fine"
# svfit_vbf_plot="m_sv_sm"
# mvis_plot="m_vis_sm_fine"
# mvis_vbf_plot="m_vis_sm"
# if options.mssm:
#   svfit_plot="m_sv_mssm_fine"
#   svfit_vbf_plot="m_sv_mssm"
#   mvis_plot="m_vis_mssm_fine"
#   mvis_vbf_plot="m_vis_mssm"
#   ANA = 'mssm'
# if options.svfit_plot: svfit_plot=options.svfit_plot
# if options.svfit_vbf_plot: svfit_vbf_plot=options.svfit_vbf_plot
# if options.mvis_plot: mvis_plot=options.mvis_plot
# if options.mvis_vbf_plot: mvis_vbf_plot=options.mvis_vbf_plot

# blind = "false"
# if options.blind: blind = "true"



# for ch in channels:

#   if not options.mvis:
#     for sc in scales:
#       CATS=[ '8','3','2','1','0' ]
#       if options.mssm: CATS=[ '8','11' ]
#       for cat in CATS:
#         os.system('./bin/HiggsTauTauPlot3 --cfg=%(CFG)s  --tau_scale_mode=%(sc)s --channel=%(ch)s --rebin=1'
#           ' --method=%(cat)s --plot_name="%(svfit_plot)s"  --x_axis_label="m_{#tau#tau} [GeV]"'
#           ' --blind=%(blind)s --x_blind_min=100 --x_blind_max=160 --make_datacard=true --norm_bins=true --verbose=false'
#           ' --paramfile=%(PARAMS)s --folder=%(folder)s' % vars())
#       CATS=[ '5' ]
#       if options.mssm: CATS=[ '12' ]
#       for cat in CATS:
#         os.system('./bin/HiggsTauTauPlot3 --cfg=%(CFG)s  --tau_scale_mode=%(sc)s --channel=%(ch)s --rebin=1'
#           ' --method=%(cat)s --plot_name="%(svfit_vbf_plot)s"  --x_axis_label="m_{#tau#tau} [GeV]"'
#           ' --blind=%(blind)s --x_blind_min=100 --x_blind_max=160 --make_datacard=true --norm_bins=true --verbose=false'
#           ' --paramfile=%(PARAMS)s --folder=%(folder)s' % vars())
#     os.system('hadd -f htt_%(ch)s.inputs-%(ANA)s-%(COM)sTeV%(output)s.root datacard_*.root' % vars())
#     os.system('rm datacard_*.root')

#   for sc in scales:
#     CATS=[ '8','3','2','1','0' ]
#     if options.mssm: CATS=[ '8','11' ]
#     for cat in CATS:
#       os.system('./bin/HiggsTauTauPlot3 --cfg=%(CFG)s  --tau_scale_mode=%(sc)s --channel=%(ch)s --rebin=1'
#         ' --method=%(cat)s --plot_name="%(mvis_plot)s"  --x_axis_label="m_{#tau#tau} [GeV]"'
#         ' --blind=%(blind)s --x_blind_min=50 --x_blind_max=110 --make_datacard=true --norm_bins=true --verbose=false'
#         ' --paramfile=%(PARAMS)s --folder=%(folder)s' % vars())
#     CATS=[ '5' ]
#     if options.mssm: CATS=['12' ]
#     for cat in CATS:
#       os.system('./bin/HiggsTauTauPlot3 --cfg=%(CFG)s  --tau_scale_mode=%(sc)s --channel=%(ch)s --rebin=1'
#         ' --method=%(cat)s --plot_name="%(mvis_vbf_plot)s"  --x_axis_label="m_{#tau#tau} [GeV]"'
#         ' --blind=%(blind)s --x_blind_min=20 --x_blind_max=80 --make_datacard=true --norm_bins=true --verbose=false'
#         ' --paramfile=%(PARAMS)s --folder=%(folder)s' % vars())
#   os.system('hadd -f htt_%(ch)s.inputs-%(ANA)s-%(COM)sTeV-mvis%(output)s.root datacard_*.root' % vars())
#   os.system('rm datacard_*.root')







