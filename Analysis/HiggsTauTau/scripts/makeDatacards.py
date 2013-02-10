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
parser.add_option("-e", dest="energy", type='string', default='8',
                  help="The C.O.M. energy is written into the datacard name, default is 8")


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


for ch in channels:
  for sc in scales:
    CATS=[ '8','3','2','1','0' ]
    for cat in CATS:
      os.system('./bin/HiggsTauTauPlot3 --cfg=%(CFG)s  --tau_scale_mode=%(sc)s --channel=%(ch)s --rebin=1'
        ' --method=%(cat)s --plot_name="m_sv_sm_fine"  --x_axis_label="m_{#tau#tau} [GeV]"'
        ' --blind=false --x_blind_min=100 --x_blind_max=160 --make_datacard=true --norm_bins=true --verbose=false'
        ' --paramfile=%(PARAMS)s --folder=%(folder)s' % vars())
    CATS=[ '5' ]
    for cat in CATS:
      os.system('./bin/HiggsTauTauPlot3 --cfg=%(CFG)s  --tau_scale_mode=%(sc)s --channel=%(ch)s --rebin=1'
        ' --method=%(cat)s --plot_name="m_sv_sm"  --x_axis_label="m_{#tau#tau} [GeV]"'
        ' --blind=false --x_blind_min=100 --x_blind_max=160 --make_datacard=true --norm_bins=true --verbose=false'
        ' --paramfile=%(PARAMS)s --folder=%(folder)s' % vars())
  os.system('hadd -f htt_%(ch)s.inputs-sm-%(COM)sTeV%(output)s.root datacard_*.root' % vars())
  os.system('rm datacard_*.root')

  for sc in scales:
    CATS=[ '8','3','2','1','0' ]
    for cat in CATS:
      os.system('./bin/HiggsTauTauPlot3 --cfg=%(CFG)s  --tau_scale_mode=%(sc)s --channel=%(ch)s --rebin=1'
        ' --method=%(cat)s --plot_name="m_vis_sm_fine"  --x_axis_label="m_{#tau#tau} [GeV]"'
        ' --blind=false --x_blind_min=100 --x_blind_max=160 --make_datacard=true --norm_bins=true --verbose=false'
        ' --paramfile=%(PARAMS)s --folder=%(folder)s' % vars())
    CATS=[ '5' ]
    for cat in CATS:
      os.system('./bin/HiggsTauTauPlot3 --cfg=%(CFG)s  --tau_scale_mode=%(sc)s --channel=%(ch)s --rebin=1'
        ' --method=%(cat)s --plot_name="m_vis_sm"  --x_axis_label="m_{#tau#tau} [GeV]"'
        ' --blind=false --x_blind_min=100 --x_blind_max=160 --make_datacard=true --norm_bins=true --verbose=false'
        ' --paramfile=%(PARAMS)s --folder=%(folder)s' % vars())
  os.system('hadd -f htt_%(ch)s.inputs-sm-%(COM)sTeV-mvis%(output)s.root datacard_*.root' % vars())
  os.system('rm datacard_*.root')







