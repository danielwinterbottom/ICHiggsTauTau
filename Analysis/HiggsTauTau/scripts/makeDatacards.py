#!/usr/bin/env python

import sys
from optparse import OptionParser
import os


CHANNELS= ['et', 'mt', 'em']
SCALES= ['1', '2', '0']


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
parser.add_option("-i","--input", dest="input", type='string',
                  help="The input folder, containing the output of HiggsTauTau")
parser.add_option("-o","--output", dest="output", type='string', default='',
                  help="The name that will be appended to the datacard inputs.")
parser.add_option("-c", "--channels", dest="channels", type='string', action='callback',callback=split_callback,
                  help="A comma separted list of channels to process.  Supported channels: %(CHANNELS)s" % vars())
parser.add_option("--central", dest="central", action='store_true', default=False,
                  help="Only process the central energy scale inputs.")
parser.add_option("--mvis", dest="mvis", action='store_true', default=False,
                  help="Only make inputs for visible mass, no svfit.")
parser.add_option("--mssm", dest="mssm", action='store_true', default=False,
                  help="Make datacards for the MSSM analysis")
parser.add_option("-e", dest="energy", type='string', default='8',
                  help="The C.O.M. energy is written into the datacard name, default is 8")
parser.add_option("--svfit_plot", dest="svfit_plot", type='string', default='m_sv_sm_fine',
                  help="Override the default svfit plot")
parser.add_option("--svfit_vbf_plot", dest="svfit_vbf_plot", type='string', default='m_sv_sm',
                  help="Override the default svfit vbf category plot")
parser.add_option("--mvis_plot", dest="mvis_plot", type='string', default='m_vis_sm_fine',
                  help="Override the default mvis plot")
parser.add_option("--mvis_vbf_plot", dest="mvis_vbf_plot", type='string', default='m_vis_sm',
                  help="Override the default mvis vbf category plot")


(options, args) = parser.parse_args()

output = options.output
if output: 
  output = '-'+output
  print 'Appending "%(output)s" to datacard outputs' % vars()

channels = options.channels
scales = SCALES
if options.central: scales = ['0']

print 'Processing energy scales: %(scales)s' % vars()
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
folder = options.input
COM = options.energy

ANA = 'sm'
if options.svfit_plot: svfit_plot=options.svfit_plot
if options.svfit_vbf_plot: svfit_vbf_plot=options.svfit_vbf_plot
if options.mvis_plot: mvis_plot=options.mvis_plot
if options.mvis_vbf_plot: mvis_vbf_plot=options.mvis_vbf_plot
if options.mssm:
  svfit_plot="m_sv_mssm_fine"
  svfit_vbf_plot="m_sv_mssm"
  mvis_plot="m_vis_mssm_fine"
  mvis_vbf_plot="m_vis_mssm"
  ANA = 'mssm'






for ch in channels:

  if not options.mvis:
    for sc in scales:
      CATS=[ '8','3','2','1','0' ]
      if options.mssm: CATS=[ '8','3','2','1','0','11' ]
      for cat in CATS:
        os.system('./bin/HiggsTauTauPlot3 --cfg=%(CFG)s  --tau_scale_mode=%(sc)s --channel=%(ch)s --rebin=1'
          ' --method=%(cat)s --plot_name="%(svfit_plot)s"  --x_axis_label="m_{#tau#tau} [GeV]"'
          ' --blind=false --x_blind_min=100 --x_blind_max=160 --make_datacard=true --norm_bins=true --verbose=false'
          ' --paramfile=%(PARAMS)s --folder=%(folder)s' % vars())
      CATS=[ '5' ]
      if options.mssm: CATS=[ '6','7','12' ]
      for cat in CATS:
        os.system('./bin/HiggsTauTauPlot3 --cfg=%(CFG)s  --tau_scale_mode=%(sc)s --channel=%(ch)s --rebin=1'
          ' --method=%(cat)s --plot_name="%(svfit_vbf_plot)s"  --x_axis_label="m_{#tau#tau} [GeV]"'
          ' --blind=false --x_blind_min=100 --x_blind_max=160 --make_datacard=true --norm_bins=true --verbose=false'
          ' --paramfile=%(PARAMS)s --folder=%(folder)s' % vars())
    os.system('hadd -f htt_%(ch)s.inputs-%(ANA)s-%(COM)sTeV%(output)s.root datacard_*.root' % vars())
    os.system('rm datacard_*.root')

  for sc in scales:
    CATS=[ '8','3','2','1','0' ]
    if options.mssm: CATS=[ '8','3','2','1','0','11' ]
    for cat in CATS:
      os.system('./bin/HiggsTauTauPlot3 --cfg=%(CFG)s  --tau_scale_mode=%(sc)s --channel=%(ch)s --rebin=1'
        ' --method=%(cat)s --plot_name="%(mvis_plot)s"  --x_axis_label="m_{#tau#tau} [GeV]"'
        ' --blind=false --x_blind_min=100 --x_blind_max=160 --make_datacard=true --norm_bins=true --verbose=false'
        ' --paramfile=%(PARAMS)s --folder=%(folder)s' % vars())
    CATS=[ '5' ]
    if options.mssm: CATS=[ '6','7','12' ]
    for cat in CATS:
      os.system('./bin/HiggsTauTauPlot3 --cfg=%(CFG)s  --tau_scale_mode=%(sc)s --channel=%(ch)s --rebin=1'
        ' --method=%(cat)s --plot_name="%(mvis_vbf_plot)s"  --x_axis_label="m_{#tau#tau} [GeV]"'
        ' --blind=false --x_blind_min=100 --x_blind_max=160 --make_datacard=true --norm_bins=true --verbose=false'
        ' --paramfile=%(PARAMS)s --folder=%(folder)s' % vars())
  os.system('hadd -f htt_%(ch)s.inputs-%(ANA)s-%(COM)sTeV-mvis%(output)s.root datacard_*.root' % vars())
  os.system('rm datacard_*.root')







