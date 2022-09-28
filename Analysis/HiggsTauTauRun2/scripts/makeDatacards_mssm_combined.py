#!/usr/bin/env python

# python scripts/makeDatacards_mssm_combined.py --years='2016,2017,2018' --channels='tt,mt,et' --batch
# python scripts/makeDatacards_mssm_combined.py --years='2016,2017,2018' --channels='tt,mt,et' --output_folder='mssm_dc' --batch
# python scripts/makeDatacards_mssm_combined.py --years='2016,2017,2018' --channels='tt,mt,et' --output_folder='mssm_dc_v4' --no_syst --batch

# after running all jobs hadd them using this commnd inside the output folder:
#for var in mt_tot_puppi stmet; do for ch in tt et mt; do for year in 2016 2017 2018; do eval "hadd -f ${year}/${ch}/htt_all.inputs-mssm-vs-sm-Run${year}-${var}.root ${year}/${ch}/*${var}.root"; done; done; done

#for var in mt_tot_puppi stmet n_bjets n_jets jpt_1 jpt_2 met pt_1 pt_2 ip_sig_1 ip_sig_2 ip_mag_1 ip_mag_2; do for ch in tt et mt; do for year in 2016 2017 2018; do eval "hadd -f ${year}/${ch}/htt_all.inputs-mssm-vs-sm-Run${year}-${var}.root ${year}/${ch}/*${var}.root"; done; done; done

# to copy them to the right combine directory
#for ch in tt et mt em; do for year in 2016 2017 2018; do cp ${year}/${ch}/htt_all.inputs-mssm-vs-sm-Run${year}-*.root /vols/cms/gu18/CH_unblinding/CMSSW_10_2_25/src/CombineHarvester/MSSMvsSMRun2Legacy/shapes/${year}/${ch}/; done; done

import sys
from optparse import OptionParser
import os

CHANNELS = ['et', 'mt','tt']

config_files = {'2016':'scripts/plot_mssm_2016.cfg',
                '2017':'scripts/plot_mssm_2017.cfg',
                '2018':'scripts/plot_mssm_2018.cfg'
               }
param_files = {'2016':'scripts/params_mssm_2016.json',
               '2017':'scripts/params_mssm_2017.json',
               '2018':'scripts/params_mssm_2018.json',
              }

def split_callback(option, opt, value, parser):
  setattr(parser.values, option.dest, value.split(','))

def CreateBatchJob(name,cmssw_base,cmd_list):
  if os.path.exists(job_file): os.system('rm %(name)s' % vars())
  os.system('echo "#!/bin/bash" >> %(name)s' % vars())
  os.system('echo "cd %(cmssw_base)s/src/UserCode/ICHiggsTauTau/Analysis/HiggsTauTauRun2" >> %(name)s' % vars())
  os.system('echo "source /vols/grid/cms/setup.sh" >> %(name)s' % vars())
  os.system('echo "export SCRAM_ARCH=slc6_amd64_gcc481" >> %(name)s' % vars())
  os.system('echo "eval \'scramv1 runtime -sh\'" >> %(name)s' % vars())
  os.system('echo "source %(cmssw_base)s/src/UserCode/ICHiggsTauTau/Analysis/HiggsTauTauRun2/scripts/setup_libs.sh" >> %(name)s' % vars())
  os.system('echo "ulimit -c 0" >> %(name)s' % vars())
  for cmd in cmd_list:
    os.system('echo "%(cmd)s" >> %(name)s' % vars())
  os.system('chmod +x %(name)s' % vars())
  print "Created job:",name

def SubmitBatchJob(name,time=180,memory=24,cores=1):
  error_log = name.replace('.sh','_error.log')
  output_log = name.replace('.sh','_output.log')
  if os.path.exists(error_log): os.system('rm %(error_log)s' % vars())
  if os.path.exists(output_log): os.system('rm %(output_log)s' % vars())
  if cores>1: os.system('qsub -e %(error_log)s -o %(output_log)s -V -q hep.q -pe hep.pe %(cores)s -l h_rt=0:%(time)s:0 -l h_vmem=%(memory)sG -cwd %(name)s' % vars())
  else: os.system('qsub -e %(error_log)s -o %(output_log)s -V -q hep.q -l h_rt=0:%(time)s:0 -l h_vmem=%(memory)sG -cwd %(name)s' % vars())


parser = OptionParser()
parser.add_option("--output_folder", dest="output_folder", type='string', default='mssm_datacards',
                  help="Output folder where plots/datacards will be saved to.")
parser.add_option("-c", "--channels", dest="channels", type='string', action='callback',callback=split_callback,
                  help="A comma separated list of channels to process.  Supported channels: %(CHANNELS)s" % vars())
parser.add_option("--batch", dest="batch", action='store_true', default=False,
                  help="Submit on batch.")
parser.add_option("--dry_run",dest="dry_run", action='store_true', default=False,
                  help="Just displays code it will run")
parser.add_option("--years", dest="years", type='string', default='2016,2017,2018',
                  help="Year input")
parser.add_option("--no_syst",dest="no_syst", action='store_true', default=False,
                  help="Run without systematics")
parser.add_option("--old_sig",dest="old_sig", action='store_true', default=False,
                  help="Will make datacards with old signal")


(options, args) = parser.parse_args()
output_folder = options.output_folder
channels = options.channels
years = options.years
old_sig = options.old_sig
no_syst = options.no_syst

print 'Processing channels:      %(channels)s' % vars()
print 'Processing years:         %(years)s' % vars()

years = options.years.split(',')

cmssw_base = os.getcwd().replace('src/UserCode/ICHiggsTauTau/Analysis/HiggsTauTauRun2','')


if not os.path.isdir('%(output_folder)s' % vars()):
  os.system("mkdir %(output_folder)s" % vars())

if not os.path.isdir('%(output_folder)s/jobs' % vars()):
  os.system("mkdir %(output_folder)s/jobs" % vars())

for year in years:

  if not os.path.isdir('%(output_folder)s/%(year)s' % vars()):
    os.system("mkdir %(output_folder)s/%(year)s" % vars())
   
  CFG = config_files[year]

  with open("%(CFG)s"%vars(),"r") as cfgfile:
    lines = cfgfile.readlines()

  configmap={}

  for ind in range(0,len(lines)):
    if len(lines[ind].split("="))>1:
      configmap[lines[ind].split("=")[0]]=(lines[ind].split("=")[1])
  YEAR=year
  FOLDER=configmap["folder"].rstrip('\n')
  PARAMS=configmap["paramfile"].rstrip('\n')

  PARAMS = param_files[year]

  ########## Set up schemes and options ############

  # my attempt at a compromise with the binning
  # merge mt_tot<50 bins since this region was tricking the past and seems to only impact the lowest mass point  (M=60)
  # use slightly finer bins for mt_tot between 350 and 1000
  BINS_FINE="[0,50,60,70,80,90,100,110,120,130,140,150,160,170,180,190,200,225,250,275,300,325,350,400,450,500,600,700,800,900,1100,1300,1500,1700,1900,2100,2300,2500,2700,2900,3100,3300,3500,3700,3900,4100,4300,4500,4700,5000]"
  BINS_STMET="[0,50,60,70,80,90,100,110,120,130,140,150,160,170,180,190,200,225,250,275,300,325,350,400,450,500,600,700,800,900,1100,1300,1500,1700,1900,2100,2300,2500,2700,2900,3100,3300,3500,3700,3900,4100,4300,4500,4700,5000]"
  BINS_VFINE="[0,10,20,30,40,50,60,70,80,90,100,110,120,130,140,150,160,170,180,190,200,225,250,275,300,325,350,400,450,500,600,700,800,900,1100,1300,1500,1700,1900,2100,2300,2500,2700,2900,3100,3300,3500,3700,3900,4100,4300,4500,4700,5000]"
  BINS_SVFIT="[0,5,10,15,20,25,30,35,40,45,50,55,60,65,70,75,80,85,90,95,100,105,110,115,120,125,130,135,140,145,150,155,160,165,170,175,180,185,190,195,200,205,210,215,220,225,230,235,240,245,250,255,260,265,270,275,280,285,290,295,300]"
  BINS_MVIS="[0,5,10,15,20,25,30,35,40,45,50,55,60,65,70,75,80,85,90,95,100,105,110,115,120,125,130,135,140,145,150,155,160,165,170,175,180,185,190,195,200]"
  BINS_2D='[0,50,100,200],[0,10,20,30,40,50,60,70,80,90,100,110,120,130,140,150,160,170,180,190,200,210,220,230,240,250,260,270,280,290,300]'

  # for btag category bins merge mt_tot<60 for same reason as above
  # add a slightly finer binning between 500 and 1000
  BINS="[0,60,80,100,120,140,160,180,200,250,300,350,400,500,600,700,800,900,1100,1300,1500,1700,1900,2100,2300,2500,2700,2900,3100,3300,3500,3700,3900,4100,4300,4500,4700,5000]"

  categories_et = [
                   "Nbtag0_MTLt40",
                   "Nbtag0_MT40To70",
                   "NbtagGt1_MTLt40",
                   "NbtagGt1_MT40To70",
                   "Nbtag0_MTLt40_MHGt250",
                   "Nbtag0_MT40To70_MHGt250",
                   #"Nbtag0_MTLt40_MHGt200",
                   #"Nbtag0_MT40To70_MHGt200",
                   "Nbtag0_Njets0_MTLt40",
                   "Nbtag0_Njets0_MT40To70",
                   "Nbtag0_NjetsGt1_MTLt40",
                   "Nbtag0_NjetsGt1_MT40To70",
                   ]

  categories_mt = [
                   "Nbtag0_MTLt40",
                   "Nbtag0_MT40To70",
                   "NbtagGt1_MTLt40",
                   "NbtagGt1_MT40To70",
                   "Nbtag0_MTLt40_MHGt250",
                   "Nbtag0_MT40To70_MHGt250",
                   #"Nbtag0_MTLt40_MHGt200",
                   #"Nbtag0_MT40To70_MHGt200",
                   "Nbtag0_Njets0_MTLt40",
                   "Nbtag0_Njets0_MT40To70",
                   "Nbtag0_NjetsGt1_MTLt40",
                   "Nbtag0_NjetsGt1_MT40To70",
                   ]

  categories_tt = [
                   "Nbtag0",
                   "NbtagGt1",
                   "Nbtag0_MHGt250",
                   "Nbtag0_Njets0",
                   "Nbtag0_NjetsGt1",
                   #"Nbtag0_MHGt200",
                   ]

  categories_em = [
                   #"inclusive",
                   "Nbtag0_DZetaGt30",
                   "Nbtag0_DZetam10To30",
                   "Nbtag0_DZetam35Tom10",
                   "NbtagGt1_DZetaGt30",
                   "NbtagGt1_DZetam10To30",
                   "NbtagGt1_DZetam35Tom10",
                   "NbtagGt1_DZetaLtm35",

                   "Nbtag0_DZetaGt30_MHGt250",
                   "Nbtag0_DZetam10To30_MHGt250",
                   "Nbtag0_DZetam35Tom10_MHGt250",
                   ]
 
  ANA = 'mssm-vs-sm-Run%(year)s' % vars()

  cat_schemes = {
                 'et' : categories_et,
                 'mt' : categories_mt,
                 'em' : categories_em,
                 'tt' : categories_tt
                 }

  ### Systematics ###

  common_shape_systematics = [
      ' --syst_tau_id_diff="CMS_eff_t_*_%(year)s"' % vars(), # Tau ID efficiency
      ' --syst_tau_trg_diff="CMS_eff_*_%(year)s"' % vars(), # Tau Trigger efficiency
      ' --syst_tquark="CMS_htt_ttbarShape"', # Top pT re-weighting
      ' --syst_embedding_tt="CMS_htt_emb_ttbar_%(year)s"' % vars(), # ttbar contamination in embedding
      ' --syst_embed_pt="embed_zpt_mass_shape"' % vars(), # ttbar contamination in embedding
      ' --syst_mssm_ggh ' # ggH theory uncertainties,
#      ' --syst_eff_b=CMS_htt_eff_b_%(year)s' % vars(),
#      ' --syst_fake_b=CMS_htt_mistag_b_%(year)s'% vars(),
    ]

  common_sep_shape_systematics = [
      ' --syst_tau_scale_grouped="CMS_scale_t_*group_%(year)s"' % vars(), # Tau energy scale
      ' --syst_res_j="CMS_res_j_%(year)s"' % vars(), # Jet energy resolution
      ' --syst_scale_met_unclustered="CMS_scale_met_unclustered_%(year)s"' % vars(), # MET unclustered energy uncertainty
      ' --syst_scale_met="CMS_htt_boson_scale_met_%(year)s"' % vars(), # MET recoil correction uncertainty
      ' --syst_res_met="CMS_htt_boson_res_met_%(year)s"' % vars(),  # MET recoil correction uncertainty
      ' --syst_scale_j_regrouped="CMS_scale_j_*group"', # Jet energy scale (grouped)
    ]

  if year != '2018': 
    common_shape_systematics += [
      ' --syst_prefire="CMS_prefiring"'
      ]
  if year == '2016':  
    common_shape_systematics += [
      ' --syst_zwt="CMS_htt_dyShape_2016"'
      ]
  else:
    common_shape_systematics += [
      ' --syst_zwt="CMS_htt_dyShape"'
      ]


  et_shape_systematics = [
    ' --syst_lep_trg_diff="CMS_eff_*_%(year)s"' % vars(), # Lepton trigger efficiency 
  ]

 
  et_sep_shape_systematics = [
    ' --syst_efake_0pi_scale="CMS_ZLShape_et_1prong_%(year)s"' % vars(), # l to tau h fake energy scale
    ' --syst_efake_1pi_scale="CMS_ZLShape_et_1prong1pizero_%(year)s"' % vars(), # l to tau h fake energy scale
    ' --syst_e_scale="CMS_scale_e"', # Election energy scale
    ' --do_ff_systs',
    #'--syst_e_res="CMS_res_e"',
    #'--syst_tau_res="CMS_res_t"'
  ]

  mt_shape_systematics = [
    ' --syst_lep_trg_diff="CMS_eff_*_%(year)s"' % vars(), # Lepton trigger efficiency 
  ]

  mt_sep_shape_systematics = [
    ' --syst_mufake_0pi_scale="CMS_ZLShape_mt_1prong_%(year)s"' % vars(), # l to tau h fake energy scale
    ' --syst_mufake_1pi_scale="CMS_ZLShape_mt_1prong1pizero_%(year)s"' % vars(), # l to tau h fake energy scale
    ' --do_ff_systs',
    #'--syst_mu_res="CMS_res_mu"',
   # '--syst_tau_res="CMS_res_t"',
    #'--syst_mu_scale="CMS_scale_mu"',
  ]

  tt_shape_systematics = [
  ]
  
  tt_sep_shape_systematics = [
    ' --do_ff_systs'
    #'--syst_tau_res="CMS_res_t"',
  ]
 
  em_shape_systematics = ['']
  em_sep_shape_systematics = [
    #'--syst_mu_res="CMS_res_mu"',
    #'--syst_e_res="CMS_res_e"',
    #'--syst_mu_scale="CMS_scale_mu"',
  ]

  extra_channel = {
      "em" : common_shape_systematics+em_shape_systematics,
      "et" : common_shape_systematics+et_shape_systematics,
      "mt" : common_shape_systematics+mt_shape_systematics,
      "tt" : common_shape_systematics+tt_shape_systematics,
  }

  sep_systs_channel = {
      "em" : common_sep_shape_systematics+em_sep_shape_systematics,
      "et" : common_sep_shape_systematics+et_sep_shape_systematics,
      "mt" : common_sep_shape_systematics+mt_sep_shape_systematics,
      "tt" : common_sep_shape_systematics+tt_sep_shape_systematics,
  }


  var     = 'mt_tot'
  dc_app  = '-mt_tot_puppi'
  #var     = 'm_vis'
  #dc_app  = '-m_vis'


  for ch in channels:

    if not os.path.isdir('%(output_folder)s/%(year)s/%(ch)s' % vars()):
      os.system("mkdir %(output_folder)s/%(year)s/%(ch)s" % vars())

    categories = cat_schemes[ch]

    for cat in categories:

      add_cond = '--add_wt=\'wt_tau_trg_mssm*wt_tau_id_mssm*wt_prefire\''
      if ch == "em": method = '19'
      else: method = '17'

   
      if var == 'm_vis': bins = BINS_MVIS
      elif cat.startswith("btag") or cat.startswith("NbtagGt1"): bins = BINS
      else: bins = BINS_FINE

      if old_sig:
        add_cond += " --bbh_masses_powheg='' --ggh_masses_powheg=''"
      else:
        add_cond += " --bbh_nlo_masses='' --ggh_masses=''"

      add_cond_nosysts = add_cond

      if not no_syst:
        for i in extra_channel[ch]:  
          add_cond += i

      run_cmd = 'python %(cmssw_base)s/src/UserCode/ICHiggsTauTau/Analysis/HiggsTauTauRun2/scripts/HiggsTauTauPlot.py --cfg=%(CFG)s --channel=%(ch)s --method=%(method)s --cat=%(cat)s --year=%(YEAR)s --outputfolder=%(output_folder)s/%(year)s/%(ch)s --datacard=%(cat)s --paramfile=%(PARAMS)s --folder=%(FOLDER)s --var="%(var)s%(bins)s" --embedding --doMSSMReWeighting --add_sm_background=125 --no_plot %(add_cond)s' % vars()
      rename_cmd = 'mv %(output_folder)s/%(year)s/%(ch)s/datacard_%(var)s_%(cat)s_%(ch)s_%(YEAR)s.root %(output_folder)s/%(year)s/%(ch)s/htt_%(ch)s_%(cat)s.inputs-%(ANA)s-mt_tot_puppi.root' % vars()

      run_cmd_alt1 = ''
      rename_cmd_alt1 = ''
      run_cmd_alt2 = ''
      rename_cmd_alt2 = ''
      run_cmd_alt3 = ''
      rename_cmd_alt3 = ''
      run_cmd_alt4 = ''
      rename_cmd_alt4 = ''

      # do seperate svfit and pt_tt vs svfit, and more bins at low mt_tot 
      #bins = BINS_VFINE
      #run_cmd_alt1 = 'python %(cmssw_base)s/src/UserCode/ICHiggsTauTau/Analysis/HiggsTauTauRun2/scripts/HiggsTauTauPlot.py --cfg=%(CFG)s --channel=%(ch)s --method=%(method)s --cat=%(cat)s --year=%(YEAR)s --outputfolder=%(output_folder)s/%(year)s/%(ch)s --datacard=%(cat)s_vfine --paramfile=%(PARAMS)s --folder=%(FOLDER)s --var="%(var)s%(bins)s" --embedding --doMSSMReWeighting --add_sm_background=125 --no_plot %(add_cond)s' % vars()
      #rename_cmd_alt1 = 'mv %(output_folder)s/%(year)s/%(ch)s/datacard_%(var)s_%(cat)s_vfine_%(ch)s_%(YEAR)s.root %(output_folder)s/%(year)s/%(ch)s/htt_%(ch)s_%(cat)s_vfine.inputs-%(ANA)s%(dc_app)s.root' % vars()

      #bins = BINS_SVFIT
      #var_alt = 'svfit_mass'
      #run_cmd_alt2 = 'python %(cmssw_base)s/src/UserCode/ICHiggsTauTau/Analysis/HiggsTauTauRun2/scripts/HiggsTauTauPlot.py --cfg=%(CFG)s --channel=%(ch)s --method=%(method)s --cat=%(cat)s --year=%(YEAR)s --outputfolder=%(output_folder)s/%(year)s/%(ch)s --datacard=%(cat)s --paramfile=%(PARAMS)s --folder=%(FOLDER)s --var="%(var_alt)s%(bins)s" --embedding --doMSSMReWeighting --add_sm_background=125 --no_plot %(add_cond)s' % vars()
      #rename_cmd_alt2 = 'mv %(output_folder)s/%(year)s/%(ch)s/datacard_%(var_alt)s_%(cat)s_svfit_%(ch)s_%(YEAR)s.root %(output_folder)s/%(year)s/%(ch)s/htt_%(ch)s_%(cat)s.inputs-%(ANA)s-m_sv_puppi.root' % vars()

      #bins = BINS_2D
      #var_alt = 'pt_tt,svfit_mass'
      #run_cmd_alt3 = 'python %(cmssw_base)s/src/UserCode/ICHiggsTauTau/Analysis/HiggsTauTauRun2/scripts/HiggsTauTauPlot.py --cfg=%(CFG)s --channel=%(ch)s --method=%(method)s --cat=%(cat)s --year=%(YEAR)s --outputfolder=%(output_folder)s/%(year)s/%(ch)s --datacard=%(cat)s --paramfile=%(PARAMS)s --folder=%(FOLDER)s --var="%(var_alt)s%(bins)s" --embedding --doMSSMReWeighting --add_sm_background=125 --no_plot %(add_cond)s' % vars()
      #rename_cmd_alt3 = 'mv %(output_folder)s/%(year)s/%(ch)s/datacard_pt_tt_vs_svfit_%(cat)s_pt_tt_vs_svfit_%(ch)s_%(YEAR)s.root %(output_folder)s/%(year)s/%(ch)s/htt_%(ch)s_%(cat)s.inputs-%(ANA)s-m_sv_VS_pt_tt_splitpT.root' % vars()

      if "NjetsGt1" in cat:

        alt_var = [
                   ["stmet","pt_1+pt_2+jpt_1+met","[0,50,60,70,80,90,100,110,120,130,140,150,160,170,180,190,200,225,250,275,300,325,350,400,450,500,600,700,800,900,1100,1300,1500,1700,1900,2100,2300,2500,2700,2900,3100,3300,3500,3700,3900,4100,4300,4500,4700,5000]"],
                   ["dphi","fabs\\(dphi\\)","[0.0,0.31416,0.62832,0.94248,1.25664,1.5708,1.88496,2.19912,2.51328,2.82744,3.1416]"],
                   ["n_bjets","n_deepbjets","[0,1,2,3,4,5]"],
                   ["n_jets","n_jets","[0,1,2,3,4,5,6,7]"],
                   ["jpt_1","jpt_1","[0,60,120,180,240,300,360,420,480,540,600,660,720,780,840,900,1000,1400]"],
                   ["jpt_2","jpt_2","[0,60,120,180,240,300,360,420,480,540,600,660,720,900]"],
                   ["met","met","[0,60,120,180,240,300,360,420,480,540,600,660,780,900,1400]"],
                   ["pt_1","pt_1","[0,60,120,180,240,300,360,420,480,540,600,900,1400]"],
                   ["pt_2","pt_2","[0,60,120,180,240,300,360,420,480,540,600,900,1400]"],
                   ["ip_sig_1","ip_sig_1","[0,0.5,1.0,1.5,2.0,3,4,5,10]"],
                   ["ip_sig_2","ip_sig_2","[0,0.5,1.0,1.5,2.0,3,4,5,10]"],
                   ["ip_mag_1","ip_mag_1","[0.001,0.002,0.003,0.004,0.005,0.006,0.008,0.010,0.014,0.020]"],
                   ["ip_mag_2","ip_mag_2","[0.001,0.002,0.003,0.004,0.005,0.006,0.008,0.010,0.014,0.020]"],
                  ]
      
        run_cmd_alt4 = []
        rename_cmd_alt4 = []
        for var_name, var_alt, bins in alt_var:

          run_cmd_alt4.append('python %(cmssw_base)s/src/UserCode/ICHiggsTauTau/Analysis/HiggsTauTauRun2/scripts/HiggsTauTauPlot.py --cfg=%(CFG)s --channel=%(ch)s --method=%(method)s --cat=%(cat)s --year=%(YEAR)s --outputfolder=%(output_folder)s/%(year)s/%(ch)s --datacard=%(cat)s --paramfile=%(PARAMS)s --folder=%(FOLDER)s --var="%(var_alt)s%(bins)s" --embedding --doMSSMReWeighting --add_sm_background=125 --no_plot %(add_cond)s' % vars())
          rename_cmd_alt4.append('mv %(output_folder)s/%(year)s/%(ch)s/datacard_%(var_alt)s_%(cat)s_%(ch)s_%(YEAR)s.root %(output_folder)s/%(year)s/%(ch)s/htt_%(ch)s_%(cat)s.inputs-%(ANA)s-%(var_name)s.root' % vars())

      #commands = [(run_cmd,rename_cmd)]
      commands = []
      if not var == 'm_vis':
        if run_cmd_alt1 != '': commands.append((run_cmd_alt1, rename_cmd_alt1)) 
        if run_cmd_alt2 != '': commands.append((run_cmd_alt2, rename_cmd_alt2)) 
        if run_cmd_alt3 != '': commands.append((run_cmd_alt3, rename_cmd_alt3)) 
        for ind, val in enumerate(run_cmd_alt4):
          commands.append((run_cmd_alt4[ind], rename_cmd_alt4[ind]))

      num = 0
      for run_cmd_, rename_cmd_ in commands:

        if not options.batch:
          print run_cmd_
          if not options.dry_run:
            os.system(run_cmd_)
            os.system(rename_cmd)
        elif options.batch:
          job_file = '%(output_folder)s/jobs/mssm_datacard_%(cat)s_%(ch)s_%(YEAR)s_%(num)s.sh' % vars()
          CreateBatchJob(job_file,cmssw_base,[run_cmd_,rename_cmd_])
          if not options.dry_run:
            #if (ch in ["mt","et"] or (YEAR in "2018" and ch in "tt")) and not options.no_syst and False:
            if ch in ["mt","et"] and not options.no_syst:
              #SubmitBatchJob(job_file,time=600,memory=24,cores=1) 
              SubmitBatchJob(job_file,time=600,memory=12,cores=1) 
            else:
              SubmitBatchJob(job_file,time=180,memory=24,cores=1)
    
        
        # run systematics that involve drawing from different trees in parallel      
        for syst in sep_systs_channel[ch]:
          syst_name=syst.split('=')[0].split('--')[1]
          dc='%(cat)s_%(syst_name)s' % vars()
          run_cmd_syst_ = run_cmd_.replace(add_cond, add_cond_nosysts)+' --extra_name=%(syst_name)s --no_default %(syst)s ' % vars()
          rename_cmd_syst_ = rename_cmd_.replace('%(ch)s_%(YEAR)s.root' % vars(), '%(syst_name)s_%(ch)s_%(YEAR)s.root' % vars() ).replace('.inputs' % vars(), '_%(syst_name)s.inputs' % vars())
    
          if not options.batch:
            print run_cmd_syst_
            if not options.dry_run:
              os.system(run_cmd_syst_)
              os.system(rename_cmd_syst_)
          elif options.batch:
            job_file = '%(output_folder)s/jobs/mssm_datacard_%(dc)s_%(ch)s_%(YEAR)s_%(num)s.sh' % vars()
            CreateBatchJob(job_file,cmssw_base,[run_cmd_syst_,rename_cmd_syst_])
            if not options.dry_run:
              #if (ch in ["mt","et"] or (YEAR in "2018" and ch in "tt")) and not options.no_syst and False:
              if ch in ["mt","et"] and 'do_ff_syst' in syst:
                SubmitBatchJob(job_file,time=600,memory=12,cores=1)
              else:
                SubmitBatchJob(job_file,time=180,memory=24,cores=1)    
        num+=1


