#!/usr/bin/env python

# ./scripts/makeDatacards_cpdecay_2018_tauID_SF.py --cfg=scripts/plot_cpdecays_2018.cfg -c 'mt,zmm' scripts/params_2018.json -s 'cpdecay' --embedding 

import sys
from optparse import OptionParser
import os
import string
import shlex
from subprocess import Popen, PIPE

CHANNELS= [ 'mt', 'zmm']

def validate_channel(channel):
  assert channel in CHANNELS, 'Error, channel %(channel)s duplicated or unrecognised' % vars()
  CHANNELS.remove(channel)

def split_callback(option, opt, value, parser):
  setattr(parser.values, option.dest, value.split(','))

def run_command(command):
    print command
    p = Popen(shlex.split(command), stdout = PIPE, stderr = PIPE)
    out, err = p.communicate()
    print out, err
    return out, err

parser = OptionParser()
parser.add_option("-p","--parameterfile", dest="params", type='string',default='',
                  help="Specify the parameter file containing the luminosity and cross section information - can be used to override config file.")
parser.add_option("--cfg", dest="config", type='string',
                  help="The config file that will be passed to HiggsTauTauPlot.py. Parameter file, input folder and signal scheme taken from this cfg file unless overriden by command line options")
parser.add_option("-i","--input", dest="folder", type='string', default='',
                  help="The input folder, containing the output of HTT - can be used to override config file")
parser.add_option("-o","--output", dest="output", type='string', default='',
                  help="The name that will be appended to the datacard inputs.")
parser.add_option("--output_folder", dest="output_folder", type='string', default='./',
                  help="Output folder where plots/datacards will be saved to.")
parser.add_option("-c", "--channels", dest="channels", type='string', action='callback',callback=split_callback,
                  help="A comma separated list of channels to process.  Supported channels: %(CHANNELS)s" % vars())
parser.add_option("--blind", dest="blind", action='store_true', default=False,
                  help="blind data")
parser.add_option("--extra", dest="extra", type='string', default='',
                  help="Extra command line options, applied to every datacard")
parser.add_option("-s", "--scheme", dest="scheme", type='string', default='',
                  help="datacard scheme - can be used to override config file")
parser.add_option("-e", dest="energy", type='string', default='13',
                  help="The C.O.M. energy is written into the datacard name, default is 13")
parser.add_option("--no_shape_systs", dest="no_shape_systs", action='store_true', default=False,
                  help="Do not add shape systematics")
parser.add_option("--no_shift_systs", dest="no_shift_systs", action='store_true', default=False,
                  help="Do not add shape systematics if they require trees from deperate directory (but the rest are run as usual)")
parser.add_option("--total_jes", dest="total_jes", action='store_true', default=False,
                  help="Do total JES uncertainties.")
parser.add_option("--split_jes", dest="split_jes", action='store_true', default=False,
                  help="If set then the JES uncertainties split by source are added")
parser.add_option("--regional_jes", dest="regional_jes", action='store_true', default=False,
                  help="Split JES by sources grouped regionally")
parser.add_option("--norm_systs", dest="norm_systs", action='store_true', default=False,
                  help="Add shapes for evaluating normalisation uncerts")
parser.add_option("--year", dest="year", type='string', default='',
                  help="Output names are data-taking year dependent. This value is read from the config file if present")
parser.add_option("--embedding", dest="embedding", action='store_true', default=False,
                  help="Add shapes are embedded samples.")
parser.add_option("--batch", dest="batch", action='store_true', default=False,
                  help="Submit on batch.")
parser.add_option("--hadd", dest="hadd", action='store_true', default=False,
                  help="Hadd output files (post batch).")

(options, args) = parser.parse_args()
output_folder = options.output_folder
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
BLIND = " "
if options.blind: BLIND = "--blind"
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
YEAR=2018
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
no_shift_systs = options.no_shift_systs

########## Set up schemes and options

#### Always apply these options:

extra_global = ' '


#### Apply these options for specific channels

# update these to latest shape systematics
extra_channel = {
      "mt" : ' ',
      "zmm" : ' ',
  }
jes_systematics=''
common_shape_systematics=' --syst_zwt="CMS_htt_dyShape_13TeV" --syst_tquark="CMS_htt_ttbarShape_13TeV" --syst_mu_scale="CMS_scale_mu_13TeV" '

mt_shape_systematics=''
mt_shape_systematics+=' --syst_tau_scale_0pi="CMS_scale_t_1prong_13TeV" --syst_tau_scale_1pi="CMS_scale_t_1prong1pizero_13TeV" --syst_tau_scale_3prong="CMS_scale_t_3prong_13TeV" --syst_scale_met="CMS_htt_boson_scale_met_13TeV" --syst_res_met="CMS_htt_boson_reso_met_13TeV" '


if options.embedding:
  common_shape_systematics+=' --syst_embedding_tt="CMS_ttbar_embeded_13TeV" '

extra_channel = {
      "mt" : ' '+common_shape_systematics+ ' '+mt_shape_systematics,
      "zmm" : ' '+common_shape_systematics,#+ ' '+em_shape_systematics,
  }

if options.no_shape_systs:
  extra_channel = {
      "mt" : ' ',
      "zmm" : ' '
  }

if SCHEME == 'cpdecay':
  

  # MT variables
  VAR_MT       = "m_vis(10,50,90)"
  ADD_STRING_MT_GENERAL = '--add_wt=single_l_sf/idisoweight_2  --set_alias "sel:(mt_1<50)" --set_alias "inclusive:(n_bjets==0 && trg_singlemuon&&pt_1>25 && '
  ADD_STRING_MT_MVADM0_Pt20to40 = ' mva_dm_2==0 && pt_2>20 && pt_2<40)"'
  ADD_STRING_MT_MVADM1_Pt20to40 = ' mva_dm_2==1 && pt_2>20 && pt_2<40)"'
  ADD_STRING_MT_MVADM2_Pt20to40 = ' mva_dm_2==2 && pt_2>20 && pt_2<40)"'
  ADD_STRING_MT_MVADM10_Pt20to40 = ' mva_dm_2==10 && pt_2>20 && pt_2<40)"'
  ADD_STRING_MT_MVADM11_Pt20to40 = ' mva_dm_2==11 && pt_2>20 && pt_2<40)"'
  ADD_STRING_MT_MVADM0_PtMoreThan40 = ' mva_dm_2==0 && pt_2>40)"'
  ADD_STRING_MT_MVADM1_PtMoreThan40 = ' mva_dm_2==1 && pt_2>40)"'
  ADD_STRING_MT_MVADM2_PtMoreThan40 = ' mva_dm_2==2 && pt_2>40)"'
  ADD_STRING_MT_MVADM10_PtMoreThan40 = ' mva_dm_2==10 && pt_2>40)"'
  ADD_STRING_MT_MVADM11_PtMoreThan40 = ' mva_dm_2==11 && pt_2>40)"'
  
  ADD_STRING_MT_HPSDM0_Pt20to40 =       ' tau_decay_mode_2==0 && pt_2>20 && pt_2<40)"'
  ADD_STRING_MT_HPSDM1_Pt20to40 =       ' tau_decay_mode_2==1 && pt_2>20 && pt_2<40)"'
  ADD_STRING_MT_HPSDM10_Pt20to40 =      ' tau_decay_mode_2==10 && pt_2>20 && pt_2<40)"'
  ADD_STRING_MT_HPSDM11_Pt20to40 =      ' tau_decay_mode_2==11 && pt_2>20 && pt_2<40)"'
  ADD_STRING_MT_HPSDM0_PtMoreThan40 =   ' tau_decay_mode_2==0 && pt_2>40)"'
  ADD_STRING_MT_HPSDM1_PtMoreThan40 =   ' tau_decay_mode_2==1 && pt_2>40)"'
  ADD_STRING_MT_HPSDM10_PtMoreThan40 =  ' tau_decay_mode_2==10 && pt_2>40)"'
  ADD_STRING_MT_HPSDM11_PtMoreThan40 =  ' tau_decay_mode_2==11 && pt_2>40)"'

  # ZMM variables
  VAR_ZMM       = "m_vis(1,70,120)"
  ADD_STRING_ZMM_GENERAL = '--add_wt=single_l_sf --set_alias "inclusive:(n_bjets==0 && trg_singlemuon&&pt_1>25)" ' 
  
  scheme_zmm = [
   # ("8",    "inclusive",      "2018_ZMM_inclusive",         VAR_ZMM, ADD_STRING_ZMM_GENERAL ),
  ]
  
  scheme_mt = [
    ("12",   "inclusive",      "2018_MVADM0_Pt20to40",       VAR_MT,  ADD_STRING_MT_GENERAL + ADD_STRING_MT_MVADM0_Pt20to40),
   # ("12",   "inclusive",      "2018_MVADM1_Pt20to40",       VAR_MT,  ADD_STRING_MT_GENERAL + ADD_STRING_MT_MVADM1_Pt20to40),
   # ("12",   "inclusive",      "2018_MVADM2_Pt20to40",       VAR_MT,  ADD_STRING_MT_GENERAL + ADD_STRING_MT_MVADM2_Pt20to40),
   # ("12",   "inclusive",      "2018_MVADM10_Pt20to40",      VAR_MT,  ADD_STRING_MT_GENERAL + ADD_STRING_MT_MVADM10_Pt20to40),
   # ("12",   "inclusive",      "2018_MVADM11_Pt20to40",      VAR_MT,  ADD_STRING_MT_GENERAL + ADD_STRING_MT_MVADM11_Pt20to40),
   # ("12",   "inclusive",      "2018_MVADM0_PtMoreThan40",   VAR_MT,  ADD_STRING_MT_GENERAL + ADD_STRING_MT_MVADM0_PtMoreThan40),
   # ("12",   "inclusive",      "2018_MVADM1_PtMoreThan40",   VAR_MT,  ADD_STRING_MT_GENERAL + ADD_STRING_MT_MVADM1_PtMoreThan40),
   # ("12",   "inclusive",      "2018_MVADM2_PtMoreThan40",   VAR_MT,  ADD_STRING_MT_GENERAL + ADD_STRING_MT_MVADM2_PtMoreThan40),
   # ("12",   "inclusive",      "2018_MVADM10_PtMoreThan40",  VAR_MT,  ADD_STRING_MT_GENERAL + ADD_STRING_MT_MVADM10_PtMoreThan40),
   # ("12",   "inclusive",      "2018_MVADM11_PtMoreThan40",  VAR_MT,  ADD_STRING_MT_GENERAL + ADD_STRING_MT_MVADM11_PtMoreThan40),

   # ("12",   "inclusive",      "2018_HPSDM0_Pt20to40",       VAR_MT,  ADD_STRING_MT_GENERAL + ADD_STRING_MT_HPSDM0_Pt20to40),
   # ("12",   "inclusive",      "2018_HPSDM1_Pt20to40",       VAR_MT,  ADD_STRING_MT_GENERAL + ADD_STRING_MT_HPSDM1_Pt20to40),
   # ("12",   "inclusive",      "2018_HPSDM10_Pt20to40",      VAR_MT,  ADD_STRING_MT_GENERAL + ADD_STRING_MT_HPSDM10_Pt20to40),
   # ("12",   "inclusive",      "2018_HPSDM11_Pt20to40",      VAR_MT,  ADD_STRING_MT_GENERAL + ADD_STRING_MT_HPSDM11_Pt20to40),
   # ("12",   "inclusive",      "2018_HPSDM0_PtMoreThan40",   VAR_MT,  ADD_STRING_MT_GENERAL + ADD_STRING_MT_HPSDM0_PtMoreThan40),
   # ("12",   "inclusive",      "2018_HPSDM1_PtMoreThan40",   VAR_MT,  ADD_STRING_MT_GENERAL + ADD_STRING_MT_HPSDM1_PtMoreThan40),
   # ("12",   "inclusive",      "2018_HPSDM10_PtMoreThan40",  VAR_MT,  ADD_STRING_MT_GENERAL + ADD_STRING_MT_HPSDM10_PtMoreThan40),
   # ("12",   "inclusive",      "2018_HPSDM11_PtMoreThan40",  VAR_MT,  ADD_STRING_MT_GENERAL + ADD_STRING_MT_HPSDM11_PtMoreThan40),
  ]
  scheme_tt = [
  ]
  scheme_em = [
  ]
  bkg_schemes = {
    'et' : 'et_default',
    'mt' : 'mt_with_zmm',
    'em' : 'em_default',
    'tt' : 'tt_default',
    'zmm' : 'zmm_default'
  }
  ANA = 'sm'


cat_schemes = {
  'mt' : scheme_mt,
  'zmm' : scheme_zmm
}

qsub_command = 'qsub -e ./err -o /dev/null -cwd -V -q hep.q -v CFG="{}",ch="{}",cat_num="{}",cat_str="{}",YEAR="{}",output_folder="{}",dc="{}",PARAMS="{}",FOLDER="{}",BLIND="{}"'

dc_app='-2D'
for ch in channels:
    scheme = cat_schemes[ch]
    bkg_scheme = bkg_schemes[ch]
    for x in scheme:
        cat_num = x[0]
        cat_str = x[1]
        dc      = x[2]
        var     = x[3]
        opts    = x[4]
        extra = options.extra + ' ' + extra_global + ' ' + extra_channel[ch] + ' ' + opts
        if options.embedding: extra+=' --embedding'
        extra_jes = options.extra + ' ' + extra_global + ' ' + jes_systematics + ' ' + opts + ' --no_default '

        if not options.hadd:
            if not options.batch:
                print 'python $CMSSW_BASE/src/UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/scripts/HiggsTauTauPlot.py --cfg=%(CFG)s --channel=%(ch)s --method=%(cat_num)s --cat=%(cat_str)s --year=%(YEAR)s --outputfolder=%(output_folder)s/ --datacard=%(dc)s --paramfile=%(PARAMS)s --folder=%(FOLDER)s %(BLIND)s --var="%(var)s" %(extra)s ' % vars()
                os.system('python $CMSSW_BASE/src/UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/scripts/HiggsTauTauPlot.py --cfg=%(CFG)s --channel=%(ch)s'
                    ' --method=%(cat_num)s --cat=%(cat_str)s --year=%(YEAR)s --outputfolder=%(output_folder)s/ --datacard=%(dc)s'
                    ' --paramfile=%(PARAMS)s --folder=%(FOLDER)s %(BLIND)s'
                    ' --var="%(var)s" %(extra)s --ratio_range 0,2 --log_y ' % vars())

            else:
                run_command(qsub_command
                        .format(CFG,ch,cat_num,cat_str,YEAR,output_folder,dc,PARAMS,FOLDER,BLIND)
                        + ' -v var="\'{}\'"'.format(var)
                        + ' -v extra="{}"'.format(extra)
                        + ' ./scripts/batch_datacards.sh'
                        )

            if jes_systematics and not options.no_shape_systs and not options.batch:
              # have to do this to avoid using too much memory...
                os.system('python $CMSSW_BASE/src/UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/scripts/HiggsTauTauPlot.py --cfg=%(CFG)s --channel=%(ch)s'
                    ' --method=%(cat_num)s --cat=%(cat_str)s --year=%(YEAR)s --outputfolder=%(output_folder)s/ --datacard=%(dc)s --extra_name=jes1'
                    ' --paramfile=%(PARAMS)s --folder=%(FOLDER)s %(BLIND)s'
                    ' --var="%(var)s" %(extra_jes)s --no_plot --jes_sources=1:9' % vars())
                os.system('python $CMSSW_BASE/src/UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/scripts/HiggsTauTauPlot.py --cfg=%(CFG)s --channel=%(ch)s'
                    ' --method=%(cat_num)s --cat=%(cat_str)s --year=%(YEAR)s --outputfolder=%(output_folder)s/ --datacard=%(dc)s --extra_name=jes2'
                    ' --paramfile=%(PARAMS)s --folder=%(FOLDER)s %(BLIND)s'
                    ' --var="%(var)s" %(extra_jes)s --no_plot --jes_sources=10:18' % vars())
                os.system('python $CMSSW_BASE/src/UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/scripts/HiggsTauTauPlot.py --cfg=%(CFG)s --channel=%(ch)s'
                    ' --method=%(cat_num)s --cat=%(cat_str)s --year=%(YEAR)s --outputfolder=%(output_folder)s/ --datacard=%(dc)s --extra_name=jes3'
                    ' --paramfile=%(PARAMS)s --folder=%(FOLDER)s %(BLIND)s'
                    ' --var="%(var)s" %(extra_jes)s --no_plot --jes_sources=19:27' % vars())

            elif jes_systematics and not options.no_shape_systs and options.batch:
                run_command(qsub_command
                        .format(CFG,ch,cat_num,cat_str,YEAR,output_folder,dc,PARAMS,FOLDER,BLIND)
                        + ' -v var="\'{}\'"'.format(var)
                        + ' -v extra_jes="{}"'.format(extra_jes)
                        + ' -v extra_name=jes1,jes_sources=1:9 ./scripts/batch_datacards_jes.sh'
                        )
                run_command(qsub_command
                        .format(CFG,ch,cat_num,cat_str,YEAR,output_folder,dc,PARAMS,FOLDER,BLIND)
                        + ' -v var="\'{}\'"'.format(var)
                        + ' -v extra_jes="{}"'.format(extra_jes)
                        + ' -v extra_name=jes2,jes_sources=10:18 ./scripts/batch_datacards_jes.sh'
                        )
                run_command(qsub_command
                        .format(CFG,ch,cat_num,cat_str,YEAR,output_folder,dc,PARAMS,FOLDER,BLIND)
                        + ' -v var="\'{}\'"'.format(var)
                        + ' -v extra_jes="{}"'.format(extra_jes)
                        + ' -v extra_name=jes3,jes_sources=19:27 ./scripts/batch_datacards_jes.sh'
                        )

    if not options.batch:
        os.system('hadd -f %(output_folder)s/htt_%(ch)s.inputs-%(ANA)s-%(COM)sTeV%(dc_app)s%(output)s.root %(output_folder)s/datacard_*_%(ch)s_%(YEAR)s.root' % vars())
        os.system('rm %(output_folder)s/datacard_*_%(ch)s_%(YEAR)s.root' % vars())

    if options.hadd:
        os.system('hadd -f %(output_folder)s/htt_%(ch)s.inputs-%(ANA)s-%(COM)sTeV%(dc_app)s%(output)s.root %(output_folder)s/datacard_*_%(ch)s_%(YEAR)s.root' % vars())
        os.system('rm %(output_folder)s/datacard_*_%(ch)s_%(YEAR)s.root ' % vars())

