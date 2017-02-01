#!/usr/bin/env python

#./scripts/makeDatacardsRun2.py --cfg=scripts/new_plot_sm_2016.cfg -c 'et,mt,tt' -i output/Aug11_PreSel_sv_v4 --svfit scripts/Params_2016_spring16.dat -s 'run2_sm_run1cats' > datacard_outut.txt

import sys
from optparse import OptionParser
import os
import string

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
                  help="Make inputs for visible mass.")
parser.add_option("--svfit", dest="svfit", action='store_true', default=False,
                  help="Make inputs for svfit mass.")
parser.add_option("--mttot", dest="mttot", action='store_true', default=False,
                  help="Make inputs for total transverse mass.")
parser.add_option("--mtlep", dest="mtlep", action='store_true', default=False,
                  help="Make inputs for mt(lep1,lep2).")
parser.add_option("--mt1", dest="mt1", action='store_true', default=False,
                  help="Make inputs for mt(lep1,met).")
parser.add_option("--mt2", dest="mt2", action='store_true', default=False,
                  help="Make inputs for mt(lep2,met).")
parser.add_option("--mtsq", dest="mtsq", action='store_true', default=False,
                  help="Make inputs for sqrt(mt1*mt1+mt2*mt2).")
parser.add_option("--mtsv", dest="mtsv", action='store_true', default=False,
                  help="Make inputs for total transverse svfit mass.")
parser.add_option("--constant_binning",dest="const", action='store_true', default=False,
                  help="Use constant binning (196,0,3920)")
parser.add_option("--add_sm",dest="add_sm",action='store_true', default=False,
                  help="Add SM H as background")
parser.add_option("-e", dest="energy", type='string', default='13',
                  help="The C.O.M. energy is written into the datacard name, default is 13")
parser.add_option("--no_shape_systs", dest="no_shape_systs", action='store_true', default=False,
                  help="Do not add shape systematics")
parser.add_option("--norm_systs", dest="norm_systs", action='store_true', default=False,
                  help="Add shapes for evaluating normalisation uncerts")
parser.add_option("--year", dest="year", type='string', default='',
                  help="Output names are data-taking year dependent. This value is read from the config file if present")

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

#Hacky config file parsing
with open("%(CFG)s"%vars(),"r") as cfgfile:
  lines = cfgfile.readlines()

configmap={}

for ind in range(0,len(lines)):
  if len(lines[ind].split("="))>1:
    configmap[lines[ind].split("=")[0]]=(lines[ind].split("=")[1])
if "signal_scheme" in configmap:
  SCHEME= configmap["signal_scheme"].rstrip('\n')
YEAR=2015
if "year" in configmap:
  YEAR=configmap["year"].rstrip('\n')
FOLDER=configmap["folder"].rstrip('\n')
PARAMS=configmap["paramfile"].rstrip('\n')

#Override config file params
if not options.scheme == "":
  SCHEME=options.scheme
if not options.folder ==  "":
  FOLDER=options.folder
if not options.params == "":
  PARAMS=options.params
if not options.year == "":
  YEAR=options.year


########## Set up schemes and options

#### Always apply these options:

extra_global = ' '
if options.add_sm:
  extra_global = ' --add_sm_background '

#### Apply these options for specific channels

extra_channel = {
    "et" : ' --syst_tau_scale="CMS_scale_t_et_13TeV" --syst_eff_t="CMS_eff_t_mssmHigh_et_13TeV" --syst_tquark="CMS_htt_ttbarShape_13TeV" --syst_zwt="CMS_htt_dyShape_13TeV" --syst_w_fake_rate="CMS_htt_wFakeShape_13TeV" ', 
    "mt" : ' --syst_tau_scale="CMS_scale_t_mt_13TeV" --syst_eff_t="CMS_eff_t_mssmHigh_mt_13TeV" --syst_tquark="CMS_htt_ttbarShape_13TeV" --syst_zwt="CMS_htt_dyShape_13TeV" --syst_w_fake_rate="CMS_htt_wFakeShape_13TeV" ',
    "tt" : ' --syst_tau_scale="CMS_scale_t_tt_13TeV" --syst_eff_t="CMS_eff_t_mssmHigh_tt_13TeV" --syst_tquark="CMS_htt_ttbarShape_13TeV" --syst_zwt="CMS_htt_dyShape_13TeV" ',
    "em" : ' --syst_tau_scale="CMS_scale_e_em_13TeV" --syst_tquark="CMS_htt_ttbarShape_13TeV" --syst_zwt="CMS_htt_dyShape_13TeV" ',
}

if options.no_shape_systs:
  extra_channel = {
      "et" : ' ',
      "mt" : ' ',
      "tt" : ' ',
      "em" : ' '
  }

if options.norm_systs:
  extra_channel["et"] += ' --syst_scale_j="CMS_scale_j_13TeV"  --syst_eff_b="CMS_scale_b_13TeV" --syst_fake_b="CMS_fake_b_13TeV" '
  extra_channel["mt"] += ' --syst_scale_j="CMS_scale_j_13TeV"  --syst_eff_b="CMS_scale_b_13TeV" --syst_fake_b="CMS_fake_b_13TeV" '
  extra_channel["tt"] += ' --syst_scale_j="CMS_scale_j_13TeV"  --syst_eff_b="CMS_scale_b_13TeV" --syst_fake_b="CMS_fake_b_13TeV" '
  extra_channel["em"] += ' --syst_scale_j="CMS_scale_j_13TeV"  --syst_eff_b="CMS_scale_b_13TeV" --syst_fake_b="CMS_fake_b_13TeV" '


if SCHEME == 'run2_mssm_2016':
  BINS_FINE="[0,10,20,30,40,50,60,70,80,90,100,110,120,130,140,150,160,170,180,190,200,225,250,275,300,325,350,400,500,700,900,1100,1300,1500,1700,1900,2100,2300,2500,2700,2900,3100,3300,3500,3700,3900]"
  BINS="[0,20,40,60,80,100,120,140,160,180,200,250,300,350,400,500,700,900,1100,1300,1500,1700,1900,2100,2300,2500,2700,2900,3100,3300,3500,3700,3900]"
  if options.const:
    BINS_FINE="(98,0,3920)"
    BINS="(98,0,3920)"

  scheme_et = [
    ("12",   "inclusive",  "inclusive",  BINS_FINE, '--set_alias="sel:mt_1<50" '),
    ("12",   "nobtag",    "nobtag",  BINS_FINE, '--set_alias="sel:mt_1<50"  '),
    ("12",   "nobtag",    "nobtag_wjets_cr",  BINS_FINE, '--set_alias="sel:mt_1>70" '),
    ("12",   "nobtag",    "nobtag_wjets_ss_cr",  BINS_FINE, '--set_alias="sel:mt_1>70" --do_ss=true '),
    ("12",   "nobtag",    "nobtag_qcd_cr",  BINS_FINE, '--set_alias="sel:mt_1<50"  --do_ss=true '),
    ("16",   "btag",    "btag",  BINS, '--set_alias="sel:mt_1<50" '),
    ("16",   "btag",    "btag_wjets_cr",  BINS, '--set_alias="sel:mt_1>70" '),
    ("16",   "btag",    "btag_wjets_ss_cr",  BINS, '--set_alias="sel:mt_1>70" --do_ss=true '),
    ("16",   "btag",    "btag_qcd_cr",  BINS, '--set_alias="sel:mt_1<50" --do_ss=true '),
  ]
  scheme_mt = [
    ("12",   "inclusive",  "inclusive",  BINS_FINE, '--set_alias="sel:mt_1<40" '),
    ("12",   "nobtag",    "nobtag",  BINS_FINE, '--set_alias="sel:mt_1<40" '),
    ("12",   "nobtag",    "nobtag_wjets_cr",  BINS_FINE, '--set_alias="sel:mt_1>70" '),
    ("12",   "nobtag",    "nobtag_wjets_ss_cr",  BINS_FINE, '--set_alias="sel:mt_1>70"  --do_ss=true '),
    ("12",   "nobtag",    "nobtag_qcd_cr",  BINS_FINE, '--set_alias="sel:mt_1<40" --do_ss=true '),
    ("16",   "btag",    "btag",  BINS, '--set_alias="sel:mt_1<40" '),
    ("16",   "btag",    "btag_wjets_cr",  BINS, '--set_alias="sel:mt_1>70" '),
    ("16",   "btag",    "btag_wjets_ss_cr",  BINS, '--set_alias="sel:mt_1>70"  --do_ss=true '),
    ("16",   "btag",    "btag_qcd_cr",  BINS, '--set_alias="sel:mt_1<40" --do_ss=true '),
  ]
  scheme_tt = [
    ("8",   "inclusive",    "inclusive",  BINS_FINE,  ''),
    ("8",   "nobtag",    "nobtag",  BINS_FINE, ''),
    ("8",   "btag",    "btag",  BINS, '--set_alias="tt_qcd_norm:mva_olddm_tight_1>0.5&&mva_olddm_loose_2>0.5 &&mva_olddm_tight_2<0.5&&antiele_1 && antimu_1 && antiele_2 &&antimu_2 && leptonveto<0.5" '),

#    ("8",   "inclusive",    "inclusive_med_med",  BINS_FINE,  '--set_alias="tt_qcd_norm:mva_olddm_medium_1>0.5&&mva_olddm_medium_2>0.5&&mva_olddm_tight_2<0.5&&antiele_1&&antimu_1&&antiele_2&&antimu_2&&!leptonveto"'),
#    ("8",   "nobtag",    "nobtag_med_med",  BINS_FINE, '--set_alias="tt_qcd_norm:mva_olddm_medium_1>0.5&&mva_olddm_medium_2>0.5&&mva_olddm_tight_2<0.5&&antiele_1&&antimu_1&&antiele_2&&antimu_2&&!leptonveto"'),
#    ("8",   "btag",    "btag_med_med",  BINS, '--set_alias="tt_qcd_norm:mva_olddm_medium_1>0.5&&mva_olddm_medium_2>0.5&&mva_olddm_tight_2<0.5&&antiele_1&&antimu_1&&antiele_2&&antimu_2&&!leptonveto"'),
#    ("8",   "inclusive",    "inclusive_tight_med",  BINS_FINE,  '--set_alias="tt_qcd_norm:mva_olddm_tight_1>0.5&&mva_olddm_medium_2>0.5&&mva_olddm_tight_2<0.5&&antiele_1&&antimu_1&&antiele_2&&antimu_2&&!leptonveto"'),
#    ("8",   "nobtag",    "nobtag_tight_med",  BINS_FINE, '--set_alias="tt_qcd_norm:mva_olddm_tight_1>0.5&&mva_olddm_medium_2>0.5&&mva_olddm_tight_2<0.5&&antiele_1&&antimu_1&&antiele_2&&antimu_2&&!leptonveto"'),
#    ("8",   "btag",    "btag_tight_med",  BINS, '--set_alias="tt_qcd_norm:mva_olddm_tight_1>0.5&&mva_olddm_medium_2>0.5&&mva_olddm_tight_2<0.5&&antiele_1&&antimu_1&&antiele_2&&antimu_2&&!leptonveto"')
  ]
  scheme_em = [
    ("15",   "inclusive",    "inclusive",  BINS_FINE, '--set_alias="sel:pzeta>-20"'),
    ("15",   "nobtag",    "nobtag",  BINS_FINE, '--set_alias="sel:pzeta>-20"'),
    ("15",   "btag",    "btag",  BINS, '--set_alias="sel:pzeta>-20"')
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
  #'em' : scheme_em,
  'tt' : scheme_tt
}


plots = [ 
  ('m_vis'  , 'M_{#tau#tau}^{vis} [GeV]'  , '-mvis' , "60", "2500" if ANA=='mssm' else "120"),
  ('m_sv'   , 'M_{#tau#tau} [GeV]'        , ''      , "100", "2500"  if ANA=='mssm' else "160"),
  ('mt_tot' , "m_{T}^{tot} [GeV]"        , '-mttot'      , "100", "3000"),
  ('mt_sv' , "m_{T#tau#tau} [GeV]"        , '-mtsv'      , "100", "3000"),
  ('mt_lep' , "m_{T}^{lep} [GeV]"        , '-mtlep'      , "100", "3000"),
  ('sqrt(mt_1*mt_1+mt_2*mt_2)' , "m_{T}^{sq} [GeV]"        , '-mtsq'      , "100", "3000"),
  ('mt_1' , "m_{T,1} [GeV]"        , '-mt1'      , "100", "3000"),
  ('mt_2' , "m_{T,2} [GeV]"        , '-mt2'      , "100", "3000")
 ]
if options.mvis:
    plots = plots[0]
if options.svfit: 
    plots = plots[1]
if options.mttot: 
    plots = plots[2]
if options.mtsv: 
    plots = plots[3]
if options.mtlep: 
    plots = plots[4]
if options.mtsq: 
    plots = plots[5]
if options.mt1: 
    plots = plots[6]
if options.mt2: 
    plots = plots[7]

var     = plots[0]
xlab    = plots[1]
dc_app  = plots[2]
blind_min = plots[3]
blind_max = plots[4]
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
    
    os.system('python $CMSSW_BASE/src/UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/scripts/HiggsTauTauPlot.py --channel=%(ch)s'
              ' --method=%(cat_num)s --cat=%(cat_str)s --year=%(YEAR)s'
              '--paramfile=%(PARAMS)s --folder=%(FOLDER)s'
              ' --var="%(var)s%(bin)s" %(extra)s' % vars())

    #os.system('$CMSSW_BASE/src/UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/bin/HiggsTauTauPlot5 --cfg=%(CFG)s --channel=%(ch)s'
    #    ' --method=%(cat_num)s --cat=%(cat_str)s --datacard=%(dc)s'
    #    ' --var="%(var)s%(bin)s" --norm_bins=true '
    #    ' --background_scheme=%(bkg_scheme)s --signal_scheme=%(sig_scheme)s'
    #    ' --x_axis_label="%(xlab)s" --y_axis_label="dN/dm_{#tau#tau} [1/GeV]"'
    #    ' --blind=%(BLIND)s --x_blind_min=%(blind_min)s --x_blind_max=%(blind_max)s --verbosity=0'
    #    ' --paramfile=%(PARAMS)s --extra_pad=0.2 --folder=%(FOLDER)s %(extra)s' % vars())
    
  #varsplit = var.split('(')
  #varname=varsplit[0]
  #os.system('hadd -f htt_%(ch)s.inputs-%(ANA)s-%(COM)sTeV%(dc_app)s%(output)s.root datacard_%(varname)s_*_%(ch)s_%(YEAR)s.root' % vars())
  #os.system('rm datacard_%(varname)s_*_%(ch)s_%(YEAR)s.root' % vars())

