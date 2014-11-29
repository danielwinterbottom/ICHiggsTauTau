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
parser.add_option("--mttbb", dest="mttbb", action='store_true', default=False,
                  help="Make inputs for mttbb.")
parser.add_option("--mH", dest="mH", action='store_true', default=False,
                  help="Make inputs for mH using kinematic fitting.")
parser.add_option("--masscuts", dest="masscuts", action='store_true', default=False,
                  help="Apply mass cuts to final discriminant.")
parser.add_option("--btag_SF", dest="btag_SF", action='store_true', default=False,
                  help="Apply b-tag scale factor.")
parser.add_option("--mbbh", dest="mbbh", action='store_true', default=False,
                  help="Make inputs for mttbb using kinematic fit on jets")
parser.add_option("--chi2", dest="chi2", action='store_true', default=False,
                  help="Make inputs for chi2 from kinematic fitting.")
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

if not (options.mvis or options.svfit or options.dijet or options.mttbb or options.mH or options.chi2 or options.mbbh) :
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
  "et" : ' --fix_empty_bins="QCD,ZL,ZJ,ZLL,W"  --fix_negative_bins="QCD,QCD.*Up,QCD.*Down" --syst_tau_scale="CMS_scale_t_etau_'+COM+'TeV" --syst_scale_j="CMS_scale_j_'+COM+'TeV"',
  #"et" : ' --fix_empty_bins="QCD,ZL,ZJ,ZLL,W"  --fix_negative_bins="QCD,QCD.*Up,QCD.*Down"',
 #"et" : ' --fix_empty_bins="QCD,ZL,ZJ,ZLL,W"  --fix_negative_bins="QCD,QCD.*Up,QCD.*Down" --syst_eff_b="CMS_eff_b_'+COM+'TeV" --syst_fake_b="CMS_fake_b_'+COM+'TeV"',
  "mt" : ' --fix_empty_bins="QCD,ZL,ZJ,ZLL,W"  --fix_negative_bins="QCD,QCD.*Up,QCD.*Down" --syst_tau_scale="CMS_scale_t_mutau_'+COM+'TeV" --syst_scale_j="CMS_scale_j_'+COM+'TeV"',
  #"mt" : ' --fix_empty_bins="QCD,ZL,ZJ,ZLL,W"  --fix_negative_bins="QCD,QCD.*Up,QCD.*Down"',
  #"mt" : ' --fix_empty_bins="QCD,ZL,ZJ,ZLL,W"  --fix_negative_bins="QCD,QCD.*Up,QCD.*Down" --syst_eff_b="CMS_eff_b_'+COM+'TeV" --syst_fake_b="CMS_fake_b_'+COM+'TeV"',
  "em" : ' --fix_empty_bins="Fakes" --fix_negative_bins="Fakes,Fakes.*Up,Fakes.*Down"'
}

#### Set which distribution to include in datacards (note that because of different binnings it is only possible to run one at a time)
plots = [ 
  ('m_vis'  , 'M_{#tau#tau}^{vis} [GeV]'  , '-mvis' ,   "100"    ,"150"),
  ('m_sv'   , 'M_{#tau#tau} [GeV]'        , ''      ,   "100"   ,"150"),
  ('prebjet_mjj'   , 'M_{jj} [GeV]'        , ''      ,   "80"   ,"160"),
  ('mjj_tt'   , 'M_{#tau#tau+jj} [GeV]'        , ''      ,   "200"   ,"400"),
  ('m_H_hh'   , 'M_{H}^{kinfit} [GeV]'        , ''      ,   "200"   ,"400"),
  ('m_H_hh_chi2'   , '#chi^{2} from kin fit'        , ''      ,   "0"   ,"10"),
  ('mbb_h'   , 'M_{#tau#tau+jj}^{jet kinfit} [GeV]'        , ''      ,   "200"   ,"400"),
 ]
if options.mvis: 
    plots = [plots[0]]
if options.svfit:
    plots = [plots[1]]
if options.dijet: 
    plots = [plots[2]]
if options.mttbb: 
    plots = [plots[3]]
if options.mH: 
    plots = [plots[4]]
if options.chi2: 
    plots = [plots[5]]
if options.mbbh: 
    plots = [plots[6]]

#################################################################
#### New HTohh scheme
#################################################################

masscut_str=""
SF_str=""
if options.masscuts:
  masscut_str="MassCuts"
if options.btag_SF: 
  SF_str="SF"


if options.scheme == 'HTohh':
  if options.mvis or options.svfit:  
    BINS_FINE="[0,10,20,30,40,50,60,70,80,90,100,110,120,130,140,150,160,170,180,190,200,225,250,275,300,325,350]"
    BINS="[0,20,40,60,80,100,120,140,160,180,200,250,300,350]"
  elif options.dijet :
    BINS_FINE="(30,0,600)"
    BINS="(15,0,600)"
  elif options.mttbb or options.mbbh:
    BINS_FINE="[0,20,40,60,80,100,120,140,160,180,200,220,240,260,280,300,320,340,360,380,400,420,440,460,480,500,550,600,650,700,750,800,850,900,950,1000]"
    BINS="[0,20,40,60,80,100,120,140,160,180,200,220,240,260,280,300,320,340,360,380,400,420,440,460,480,500,550,600,650,700,750,800,850,900,950,1000]"
  elif options.mH :  
    BINS_FINE="[200,250,270,290,310,330,350,370,390,410,430,450,500,550,600,650,700]"
    BINS="[200,250,270,290,310,330,350,370,390,410,430,450,500,550,600,650,700]"
  elif options.chi2:
    BINS_FINE="(25,0,50)"
    BINS="(25,0,50)"
  scheme_et = [
    ("8",    "inclusive",   "inclusive",  BINS_FINE,  (
      ' --syst_w_fake_rate="CMS_htt_WShape_etau_inclusive_'+COM+'TeV"')),
    ("14",   "2jet0tag"+SF_str+masscut_str,      "2jet0tag",     BINS_FINE, ( 
      ' --syst_w_fake_rate="CMS_htt_WShape_etau_2jet0tag_'+COM+'TeV"'
      ' --syst_qcd_shape="CMS_htt_QCDShape_etau_2jet0tag_'+COM+'TeV:50:1.0:0.10"'
#      ' --syst_tquark="CMS_htt_TTbarShape_etau_2jet0tag_'+COM+'TeV"'
      ' --sub_ztt_top_shape=true')),
    ("15",   "2jet1tag"+SF_str+masscut_str,      "2jet1tag",     BINS_FINE,  (
      ' --syst_w_fake_rate="CMS_htt_WShape_etau_2jet1tag_'+COM+'TeV"'
      ' --syst_qcd_shape="CMS_htt_QCDShape_etau_2jet1tag_'+COM+'TeV:50:1.0:0.10"'
#      ' --syst_tquark="CMS_htt_TTbarShape_etau_2jet1tag_'+COM+'TeV"'
      ' --sub_ztt_top_shape=true')),
    ("16",   "2jet2tag"+SF_str+masscut_str,        "2jet2tag",       BINS,  (
      ' --syst_w_fake_rate="CMS_htt_WShape_etau_2jet2tag_'+COM+'TeV"'
      ' --syst_qcd_shape="CMS_htt_QCDShape_etau_2jet2tag_'+COM+'TeV:50:1.0:0.10"'
#      ' --syst_tquark="CMS_htt_TTbarShape_etau_2jet2tag_'+COM+'TeV"'
      ' --sub_ztt_top_shape=true')),
  #  ("3",   "1jet0tag",      "1jet0tag",     BINS_FINE, ( 
 #     ' --syst_w_fake_rate="CMS_htt_WShape_etau_1jet0tag_'+COM+'TeV"'
  #    ' --syst_qcd_shape="CMS_htt_QCDShape_etau_1jet0tag_'+COM+'TeV:50:1.0:0.10"')),
  #  ("4",   "1jet1tag",      "1jet1tag",     BINS,  (
  #    ' --syst_w_fake_rate="CMS_htt_WShape_etau_1jet1tag_'+COM+'TeV"'
   #   ' --syst_qcd_shape="CMS_htt_QCDShape_etau_1jet1tag_'+COM+'TeV:50:1.0:0.10"'
    #  ' --sub_ztt_top_shape=true'))
  ]
  scheme_mt = [
    ("8",    "inclusive",   "inclusive",  BINS_FINE, (
      ' --syst_w_fake_rate="CMS_htt_WShape_mutau_inclusive_'+COM+'TeV"')),
    ("14",   "2jet0tag"+SF_str+masscut_str,      "2jet0tag",     BINS_FINE,  (
      ' --syst_w_fake_rate="CMS_htt_WShape_mutau_2jet0tag_'+COM+'TeV"'
      ' --syst_qcd_shape="CMS_htt_QCDShape_mutau_2jet0tag_'+COM+'TeV:50:1.1:0.10"'
#      ' --syst_tquark="CMS_htt_TTbarShape_mutau_2jet0tag_'+COM+'TeV"'
      ' --sub_ztt_top_shape=true')),
    ("15",   "2jet1tag"+SF_str+masscut_str,      "2jet1tag",     BINS_FINE,  (
      ' --syst_w_fake_rate="CMS_htt_WShape_mutau_2jet1tag_'+COM+'TeV"'
      ' --syst_qcd_shape="CMS_htt_QCDShape_mutau_2jet1tag_'+COM+'TeV:50:1.0:0.10"'
#      ' --syst_tquark="CMS_htt_TTbarShape_mutau_2jet1tag_'+COM+'TeV"'
      ' --sub_ztt_top_shape=true')),
    ("16",   "2jet2tag"+SF_str+masscut_str,        "2jet2tag",       BINS,  ( 
      ' --syst_w_fake_rate="CMS_htt_WShape_mutau_2jet2tag_'+COM+'TeV"'
      ' --syst_qcd_shape="CMS_htt_QCDShape_mutau_2jet2tag_'+COM+'TeV:50:1.0:0.10"'
#      ' --syst_tquark="CMS_htt_TTbarShape_mutau_2jet2tag_'+COM+'TeV"'
      ' --sub_ztt_top_shape=true')),
#    ("3",   "1jet0tag",      "1jet0tag",     BINS_FINE,  (
 #     ' --syst_w_fake_rate="CMS_htt_WShape_mutau_1jet0tag_'+COM+'TeV"'
#      ' --syst_qcd_shape="CMS_htt_QCDShape_mutau_1jet0tag_'+COM+'TeV:50:1.1:0.10"')),
 #   ("4",   "1jet1tag",      "1jet1tag",     BINS,  (
#      ' --syst_w_fake_rate="CMS_htt_WShape_mutau_1jet1tag_'+COM+'TeV"'
#      ' --syst_qcd_shape="CMS_htt_QCDShape_mutau_1jet1tag_'+COM+'TeV:50:1.0:0.10"'
#      ' --sub_ztt_top_shape=true'))
  ]
  scheme_em = [
    ("8",    "inclusive",   "inclusive",  BINS_FINE,  (
      ' --syst_tquark="CMS_htt_TTbarShape_em_inclusive_'+COM+'TeV"'
      ' --syst_tau_scale="CMS_scale_e_'+COM+'TeV"')),
    ("0",   "2jet0tag",      "2jet0tag",     BINS_FINE, (
      ' --syst_tquark="CMS_htt_TTbarShape_em_2jet0tag_'+COM+'TeV"'
      ' --syst_fakes_os_ss_shape="CMS_htt_FakeShape_em_2jet0tag_'+COM+'TeV"'
      ' --syst_tau_scale="CMS_scale_e_'+COM+'TeV"'
      ' --sub_ztt_top_shape=true')),
    ("1",   "2jet1tag",      "2jet1tag",     BINS_FINE,  (
      ' --syst_tquark="CMS_htt_TTbarShape_em_2jet1tag_'+COM+'TeV"'
      ' --syst_qcd_shape="CMS_htt_FakeShape_em_2jet1tag_'+COM+'TeV:50:1.1:0.10"'
      ' --syst_tau_scale="CMS_scale_e_'+COM+'TeV"'
      ' --sub_ztt_top_shape=true')),
    ("2",   "2jet2tag",        "2jet2tag",       BINS, ( 
      ' --syst_tquark="CMS_htt_TTbarShape_em_2jet2tag_'+COM+'TeV"'
      ' --syst_qcd_shape="CMS_htt_FakeShape_em_2jet2tag_'+COM+'TeV:50:1.1:0.10"'
      ' --syst_tau_scale="CMS_scale_e_'+COM+'TeV"'
      ' --sub_ztt_top_shape=true')),
    ("3",   "1jet0tag",      "1jet0tag",     BINS_FINE, (
      ' --syst_tquark="CMS_htt_TTbarShape_em_1jet0tag_'+COM+'TeV"'
      ' --syst_fakes_os_ss_shape="CMS_htt_FakeShape_em_1jet0tag_'+COM+'TeV"'
      ' --syst_tau_scale="CMS_scale_e_'+COM+'TeV"')),
    ("4",   "1jet1tag",      "1jet1tag",     BINS,  (
      ' --syst_tquark="CMS_htt_TTbarShape_em_1jet1tag_'+COM+'TeV"'
      ' --syst_qcd_shape="CMS_htt_FakeShape_em_1jet1tag_'+COM+'TeV:50:1.1:0.10"'
      ' --syst_tau_scale="CMS_scale_e_'+COM+'TeV"'
      ' --sub_ztt_top_shape=true'))
  ]
  bkg_schemes = {
    'et' : 'et_default',
    'mt' : 'mt_with_zmm',
    'em' : 'em_default'
  }
  sig_scheme = 'mssm_nostack'
  ANA = 'Hhh'
  if options.svfit or options.dijet or options.mttbb or options.mvis: 
    extra_channel["et"] += ' --set_alias="sel:mt_1<30."'
    extra_channel["mt"] += ' --set_alias="sel:mt_1<30."'
    extra_channel["em"] += ' --set_alias="sel:pzeta>-30"'
  elif options.mH :
#    if options.masscuts:
#      extra_channel["et"] += ' --set_alias="sel:mt_1<30. && prebjet_mjj>70 && prebjet_mjj<150. && m_sv>90 && m_sv<150 && m_H_hh>0"'
#      extra_channel["mt"] += ' --set_alias="sel:mt_1<30. && prebjet_mjj>70 && prebjet_mjj<150. && m_sv>90 && m_sv<150 && m_H_hh>0"'
#    else:
    extra_channel["et"] += ' --set_alias="sel:mt_1<30. && m_H_hh>0"'
    extra_channel["mt"] += ' --set_alias="sel:mt_1<30. && m_H_hh>0"'
      
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
        ' --method=%(cat_num)s --cat=%(cat_str)s --datacard=%(dc)s'
        #' --method=8 --cat=%(cat_str)s --datacard=%(dc)s'
        ' --var="%(var)s%(bin)s" --norm_bins=true '
        ' --background_scheme=%(bkg_scheme)s --signal_scheme=%(sig_scheme)s'
        ' --x_axis_label="%(xlab)s" --y_axis_label="dN/dm_{#tau#tau} [1/GeV]"'
        ' --blind=%(BLIND)s --x_blind_min=%(blind_min)s --x_blind_max=%(blind_max)s --verbosity 0'
        ' --paramfile=%(PARAMS)s --folder=%(FOLDER)s %(extra)s' % vars())
    os.system('hadd -f htt_%(ch)s.inputs-%(ANA)s-%(COM)sTeV%(dc_app)s%(output)s.root datacard_%(var)s_*_%(ch)s_%(YEAR)s.root' % vars())
    os.system('rm datacard_%(var)s_*_%(ch)s_%(YEAR)s.root' % vars())

