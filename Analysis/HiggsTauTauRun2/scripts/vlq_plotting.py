#!/usr/bin/env python

# python scripts/vlq_plotting.py --channel='em,mt,et,tt' --year='2018' --output='vlq_distributions'


# Only 2018 signals at the moment

import os
import argparse
import math as math

parser = argparse.ArgumentParser()
parser.add_argument('--channel',help= 'Name of input channels', default='mt,et,tt')
parser.add_argument('--year', help= 'Name of input years', default='2016,2017,2018')
parser.add_argument('--output', help= 'Name of output folder to create', default='mssm_control_plots')
parser.add_argument('--add_systs', help= 'Add systematic', action='store_true')
parser.add_argument('--dry_run', help= 'Only create jobs, does not run them', action='store_true')
args = parser.parse_args()
      
# Things to loop over
channels = args.channel.split(',')
years = args.year.split(',')
output = args.output

def CreateBatchJob(name,cmssw_base,cmd_list):
  if os.path.exists(name): os.system('rm %(name)s' % vars())
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

def myround(x, base=5):
    return base * round(x/base)

def GetBinning(var,min_bin,max_bin,num_bins,log=False,round=1,final_bin_start=None):
  bins = []
  if not log:
    if final_bin_start == None:
      range_bins = max_bin - min_bin
    else:
      range_bins = final_bin_start - min_bin
    bin_interval = range_bins/num_bins
    for i in range(0,num_bins+1):
      bins.append(myround(min_bin+(i*bin_interval),base=round))
  else:
    if min_bin == 0: min_bin = 0.001
    min_bin_log = math.log(min_bin,10)
    if final_bin_start == None:
      max_bin_log = math.log(max_bin,10)
      range_bins_log = max_bin_log - min_bin_log
    else:
      final_bin_start_log = math.log(final_bin_start,10)
      range_bins_log = final_bin_start_log - min_bin_log
    bin_interval_log = range_bins_log/num_bins
    for i in range(0,num_bins+1):
      bins.append(myround(10**(min_bin_log+(i*bin_interval_log)),base=round))

  if final_bin_start != None: bins.append(max_bin)
  return var+str(bins).strip()

all_ch_variables = [
                    GetBinning('mt_tot',20,3200,50,log=True,round=1,final_bin_start=1000),
                    GetBinning('m_vis',20,3200,50,log=True,round=1,final_bin_start=1000),
#                    GetBinning('svfit_mass',20,3200,50,log=True,round=1,final_bin_start=1000),
                    GetBinning('met',10,800,40,log=True,round=1,final_bin_start=500),
                    #'n_jets[0,1,>=2]',
                    #'n_deepbjets[0,>=1]',
#                    'n_jets[0,1,2,3,4,5]',
#                    'n_deepbjets[0,1,2,3]',
#                    GetBinning('mt_1',0,500,40,log=False,round=1,final_bin_start=None),
#                    GetBinning('mt_2',0,500,40,log=False,round=1,final_bin_start=None),
#                    GetBinning('mt_lep',0,500,40,log=False,round=1,final_bin_start=None),
#                    GetBinning('eta_1',-2.2,2.2,40,log=False,round=0.05,final_bin_start=None),
#                    GetBinning('eta_2',-2.2,2.2,40,log=False,round=0.05,final_bin_start=None),
#                    GetBinning('met_dphi_1',0,3.2,40,log=False,round=0.05,final_bin_start=None),
#                    GetBinning('met_dphi_2',0,3.2,40,log=False,round=0.05,final_bin_start=None),
#                    GetBinning('dphi',0,3.2,40,log=False,round=0.05,final_bin_start=None),
#                    GetBinning('jpt_1',30,600,40,log=True,round=1,final_bin_start=None),
#                    GetBinning('jpt_2',30,600,40,log=True,round=1,final_bin_start=None),
#                    GetBinning('pt_vis',30,600,40,log=True,round=1,final_bin_start=None),
#                    GetBinning('pt_tt',30,600,40,log=True,round=1,final_bin_start=None),
#                    GetBinning('mjj',0,500,40,log=False,round=1,final_bin_start=None),
#                    GetBinning('dijetpt',30,600,40,log=True,round=1,final_bin_start=None),
#                    GetBinning('jdeta',0,6.0,40,log=False,round=0.05,final_bin_start=None),
                    ]

ch_dep_var = {"mt":[
                    GetBinning('pt_1',20,800,40,log=True,round=1,final_bin_start=500),
                    GetBinning('pt_2',30,800,40,log=True,round=1,final_bin_start=500),
#                    GetBinning('iso_1',0,0.15,40,log=False,round=0.001,final_bin_start=None),
                    #'tau_decay_mode_2[==0,==1,==10,==11]',
#                    'tau_decay_mode_2[0,1,10,11]',
                    ],
              "et":[
                    GetBinning('pt_1',20,800,40,log=True,round=1,final_bin_start=500),
                    GetBinning('pt_2',30,800,40,log=True,round=1,final_bin_start=500),
#                    GetBinning('iso_1',0,0.15,40,log=False,round=0.001,final_bin_start=None),
                    #'tau_decay_mode_2[==0,==1,==10,==11]',
#                    'tau_decay_mode_2[0,1,10,11]',
                    ],
              "tt":[
                    GetBinning('pt_1',40,800,40,log=True,round=1,final_bin_start=500),
                    GetBinning('pt_2',40,800,40,log=True,round=1,final_bin_start=500),
                    #'tau_decay_mode_1[==0,==1,==10,==11]',
                    #'tau_decay_mode_2[==0,==1,==10,==11]',
#                    'tau_decay_mode_1[0,1,10,11]',
#                    'tau_decay_mode_2[0,1,10,11]',
                    ],
               "em":[
                    GetBinning('pt_1',20,600,40,log=True,round=1,final_bin_start=None),
                    GetBinning('pt_2',30,600,40,log=True,round=1,final_bin_start=None),
                    GetBinning('iso_1',0,0.15,40,log=False,round=0.001,final_bin_start=None),
                    GetBinning('iso_2',0,0.15,40,log=False,round=0.001,final_bin_start=None),
                    ]
              }


config_files = {'2016':'scripts/plot_mssm_2016.cfg',
                '2017':'scripts/plot_mssm_2017.cfg',
                '2018':'scripts/plot_vlq_2018.cfg'
               }

categories = {
              "mt":[
                    #"Nbtag0_MTLt40",
                    #"Nbtag0_MT40To70",
                    #"NbtagGt1_MTLt40",
                    #"NbtagGt1_MT40To70",
                    #"Nbtag0_MTLt70",
                    #"NbtagGt1_MTLt70",
                    "MTLt70",
                    #"inclusive"
                    ],
              "et":[                    
                    #"Nbtag0_MTLt40",
                    #"Nbtag0_MT40To70",
                    #"NbtagGt1_MTLt40",
                    #"NbtagGt1_MT40To70",
                    #"Nbtag0_MTLt70",
                    #"NbtagGt1_MTLt70",
                    "MTLt70",
                    #"inclusive"
                    ],
              "tt":[
                    "inclusive",
#                    "Nbtag0",
                    #"NbtagGt1",
                    ],
              "em":[
                    "NbtagGt1_DZetaLtm35",
                    "Nbtag0_DZetaGt30",
                    "Nbtag0_DZetam10To30",
                    "Nbtag0_DZetam35Tom10",
                    "NbtagGt1_DZetaGt30",
                    "NbtagGt1_DZetam10To30",
                    "NbtagGt1_DZetam35Tom10",
                    "Nbtag0_DZetaGtm35",
                    "NbtagGt1_DZetaGtm35",
                    "DZetaGtm35",
                    ]
              }

methods = {
           "mt":17,
           "et":17,
           "tt":17,
           "em":19,
           }


add_options = '--ratio_range=\'auto\' --signal_scale=100 --log_y'
 
# Set up output directories

cmssw_base = os.getcwd().replace("src/UserCode/ICHiggsTauTau/Analysis/HiggsTauTauRun2","")

if not os.path.isdir('%(cmssw_base)s/src/UserCode/ICHiggsTauTau/Analysis/HiggsTauTauRun2/%(output)s' % vars()):
  os.system("mkdir %(cmssw_base)s/src/UserCode/ICHiggsTauTau/Analysis/HiggsTauTauRun2/%(output)s" % vars())

if not os.path.isdir('%(cmssw_base)s/src/UserCode/ICHiggsTauTau/Analysis/HiggsTauTauRun2/%(output)s/jobs' % vars()):
  os.system("mkdir %(cmssw_base)s/src/UserCode/ICHiggsTauTau/Analysis/HiggsTauTauRun2/%(output)s/jobs" % vars())

for channel in channels:
  if not os.path.isdir('%(cmssw_base)s/src/UserCode/ICHiggsTauTau/Analysis/HiggsTauTauRun2/%(output)s/%(channel)s' % vars()):
    os.system("mkdir %(cmssw_base)s/src/UserCode/ICHiggsTauTau/Analysis/HiggsTauTauRun2/%(output)s/%(channel)s" % vars())
  for year in years:
    if not os.path.isdir('%(cmssw_base)s/src/UserCode/ICHiggsTauTau/Analysis/HiggsTauTauRun2/%(output)s/%(channel)s/%(year)s' % vars()):
      os.system("mkdir %(cmssw_base)s/src/UserCode/ICHiggsTauTau/Analysis/HiggsTauTauRun2/%(output)s/%(channel)s/%(year)s" % vars())


# Job loop
for year in years:
  for channel in channels:
    variables = all_ch_variables + ch_dep_var[channel]
    categories_list = categories[channel]
    for cat in categories_list:
      for var in variables:
        if '[' in var: var_string = var.split('[')[0]
        elif '(' in var: var_string = var.split('(')[0]
    
        cfg = config_files[year]
        method = methods[channel]
        run_cmd_0_gU1 = 'python %(cmssw_base)s/src/UserCode/ICHiggsTauTau/Analysis/HiggsTauTauRun2/scripts/HiggsTauTauPlot.py --cfg=\'%(cfg)s\' --channel=%(channel)s --method=%(method)s --var=\'%(var)s\' --cat=%(cat)s %(add_options)s --plot_signals="vlq_betaRd33_0_mU2_gU1,vlq_betaRd33_0_mU3_gU1,vlq_betaRd33_0_mU4_gU1"' % vars()
        run_cmd_0_gU2 = 'python %(cmssw_base)s/src/UserCode/ICHiggsTauTau/Analysis/HiggsTauTauRun2/scripts/HiggsTauTauPlot.py --cfg=\'%(cfg)s\' --channel=%(channel)s --method=%(method)s --var=\'%(var)s\' --cat=%(cat)s %(add_options)s --plot_signals="vlq_betaRd33_0_mU2_gU2,vlq_betaRd33_0_mU3_gU2,vlq_betaRd33_0_mU4_gU2"' % vars()
        run_cmd_0_gU3 = 'python %(cmssw_base)s/src/UserCode/ICHiggsTauTau/Analysis/HiggsTauTauRun2/scripts/HiggsTauTauPlot.py --cfg=\'%(cfg)s\' --channel=%(channel)s --method=%(method)s --var=\'%(var)s\' --cat=%(cat)s %(add_options)s --plot_signals="vlq_betaRd33_0_mU2_gU3,vlq_betaRd33_0_mU3_gU3,vlq_betaRd33_0_mU4_gU3"' % vars()
        run_cmd_minus1_gU1 = 'python %(cmssw_base)s/src/UserCode/ICHiggsTauTau/Analysis/HiggsTauTauRun2/scripts/HiggsTauTauPlot.py --cfg=\'%(cfg)s\' --channel=%(channel)s --method=%(method)s --var=\'%(var)s\' --cat=%(cat)s %(add_options)s --plot_signals="vlq_betaRd33_minus1_mU2_gU1,vlq_betaRd33_minus1_mU3_gU1,vlq_betaRd33_minus1_mU4_gU1"' % vars()
        run_cmd_minus1_gU2 = 'python %(cmssw_base)s/src/UserCode/ICHiggsTauTau/Analysis/HiggsTauTauRun2/scripts/HiggsTauTauPlot.py --cfg=\'%(cfg)s\' --channel=%(channel)s --method=%(method)s --var=\'%(var)s\' --cat=%(cat)s %(add_options)s --plot_signals="vlq_betaRd33_minus1_mU2_gU2,vlq_betaRd33_minus1_mU3_gU2,vlq_betaRd33_minus1_mU4_gU2"' % vars()
        run_cmd_minus1_gU3 = 'python %(cmssw_base)s/src/UserCode/ICHiggsTauTau/Analysis/HiggsTauTauRun2/scripts/HiggsTauTauPlot.py --cfg=\'%(cfg)s\' --channel=%(channel)s --method=%(method)s --var=\'%(var)s\' --cat=%(cat)s %(add_options)s --plot_signals="vlq_betaRd33_minus1_mU2_gU3,vlq_betaRd33_minus1_mU3_gU3,vlq_betaRd33_minus1_mU4_gU3"' % vars()
        run_cmd_0_mU2 = 'python %(cmssw_base)s/src/UserCode/ICHiggsTauTau/Analysis/HiggsTauTauRun2/scripts/HiggsTauTauPlot.py --cfg=\'%(cfg)s\' --channel=%(channel)s --method=%(method)s --var=\'%(var)s\' --cat=%(cat)s %(add_options)s --plot_signals="vlq_betaRd33_0_mU2_gU1,vlq_betaRd33_0_mU2_gU2,vlq_betaRd33_0_mU2_gU3"' % vars()
        run_cmd_0_mU3 = 'python %(cmssw_base)s/src/UserCode/ICHiggsTauTau/Analysis/HiggsTauTauRun2/scripts/HiggsTauTauPlot.py --cfg=\'%(cfg)s\' --channel=%(channel)s --method=%(method)s --var=\'%(var)s\' --cat=%(cat)s %(add_options)s --plot_signals="vlq_betaRd33_0_mU3_gU1,vlq_betaRd33_0_mU3_gU2,vlq_betaRd33_0_mU3_gU3"' % vars()
        run_cmd_0_mU4 = 'python %(cmssw_base)s/src/UserCode/ICHiggsTauTau/Analysis/HiggsTauTauRun2/scripts/HiggsTauTauPlot.py --cfg=\'%(cfg)s\' --channel=%(channel)s --method=%(method)s --var=\'%(var)s\' --cat=%(cat)s %(add_options)s --plot_signals="vlq_betaRd33_0_mU4_gU1,vlq_betaRd33_0_mU4_gU2,vlq_betaRd33_0_mU4_gU3"' % vars()
        run_cmd_minus1_mU2 = 'python %(cmssw_base)s/src/UserCode/ICHiggsTauTau/Analysis/HiggsTauTauRun2/scripts/HiggsTauTauPlot.py --cfg=\'%(cfg)s\' --channel=%(channel)s --method=%(method)s --var=\'%(var)s\' --cat=%(cat)s %(add_options)s --plot_signals="vlq_betaRd33_minus1_mU2_gU1,vlq_betaRd33_minus1_mU2_gU2,vlq_betaRd33_minus1_mU2_gU3"' % vars()
        run_cmd_minus1_mU3 = 'python %(cmssw_base)s/src/UserCode/ICHiggsTauTau/Analysis/HiggsTauTauRun2/scripts/HiggsTauTauPlot.py --cfg=\'%(cfg)s\' --channel=%(channel)s --method=%(method)s --var=\'%(var)s\' --cat=%(cat)s %(add_options)s --plot_signals="vlq_betaRd33_minus1_mU3_gU1,vlq_betaRd33_minus1_mU3_gU2,vlq_betaRd33_minus1_mU3_gU3"' % vars()
        run_cmd_minus1_mU4 = 'python %(cmssw_base)s/src/UserCode/ICHiggsTauTau/Analysis/HiggsTauTauRun2/scripts/HiggsTauTauPlot.py --cfg=\'%(cfg)s\' --channel=%(channel)s --method=%(method)s --var=\'%(var)s\' --cat=%(cat)s %(add_options)s --plot_signals="vlq_betaRd33_minus1_mU4_gU1,vlq_betaRd33_minus1_mU4_gU2,vlq_betaRd33_minus1_mU4_gU3"' % vars()
        run_cmd_gu1_mU2 = 'python %(cmssw_base)s/src/UserCode/ICHiggsTauTau/Analysis/HiggsTauTauRun2/scripts/HiggsTauTauPlot.py --cfg=\'%(cfg)s\' --channel=%(channel)s --method=%(method)s --var=\'%(var)s\' --cat=%(cat)s %(add_options)s --plot_signals="vlq_betaRd33_0_mU2_gU1,vlq_betaRd33_minus1_mU2_gU1"' % vars()
        run_cmd_gu1_mU3 = 'python %(cmssw_base)s/src/UserCode/ICHiggsTauTau/Analysis/HiggsTauTauRun2/scripts/HiggsTauTauPlot.py --cfg=\'%(cfg)s\' --channel=%(channel)s --method=%(method)s --var=\'%(var)s\' --cat=%(cat)s %(add_options)s --plot_signals="vlq_betaRd33_0_mU3_gU1,vlq_betaRd33_minus1_mU3_gU1"' % vars()
        run_cmd_gu1_mU4 = 'python %(cmssw_base)s/src/UserCode/ICHiggsTauTau/Analysis/HiggsTauTauRun2/scripts/HiggsTauTauPlot.py --cfg=\'%(cfg)s\' --channel=%(channel)s --method=%(method)s --var=\'%(var)s\' --cat=%(cat)s %(add_options)s --plot_signals="vlq_betaRd33_0_mU4_gU1,vlq_betaRd33_minus1_mU4_gU1"' % vars()
 
  
  
        run_list = [run_cmd_0_gU1,run_cmd_0_gU2,run_cmd_0_gU3,run_cmd_minus1_gU1,run_cmd_minus1_gU2,run_cmd_minus1_gU3,run_cmd_0_mU2,run_cmd_0_mU3,run_cmd_0_mU4,run_cmd_minus1_mU2,run_cmd_minus1_mU3,run_cmd_minus1_mU4,run_cmd_gu1_mU2,run_cmd_gu1_mU3,run_cmd_gu1_mU4]
        add_name_list = ["_betaRd33_0_gU1","_betaRd33_0_gU2","_betaRd33_0_gU3","_betaRd33_minus1_gU1","_betaRd33_minus1_gU2","_betaRd33_minus1_gU3","_betaRd33_0_mU2","_betaRd33_0_mU3","_betaRd33_0_mU4","_betaRd33_minus1_mU2","_betaRd33_minus1_mU3","_betaRd33_minus1_mU4","_gU1_mU2","_gU1_mU3","_gU1_mU4"]
        i = 0
        for cmd in run_list:
          
          add_name = add_name_list[i]
          i += 1 

          add_syst = ''
          output_folder = '%(cmssw_base)s/src/UserCode/ICHiggsTauTau/Analysis/HiggsTauTauRun2/%(output)s/%(channel)s/%(year)s' % vars()
          if args.add_systs:
            if channel == "mt":
              add_syst = '--do_custom_uncerts --add_stat_to_syst --syst_tau_trg_diff="trg_syst_*" --syst_tau_id_diff="id_syst_*"  --do_ff_systs --syst_tquark="syst_ttbar_pt"  --syst_embedding_tt="syst_embed_tt"  --syst_tau_scale_grouped="CMS_scale_t_*group_%(year)s" --syst_res_j="CMS_res_j_%(year)s" --syst_scale_met_unclustered="CMS_scale_met_unclustered_%(year)s" --syst_scale_met="CMS_htt_boson_scale_met_%(year)s" --syst_res_met="CMS_htt_boson_reso_met_%(year)s" --syst_scale_j_regrouped="CMS_scale_j_*group" --syst_mufake_0pi_scale="CMS_ZLShape_mt_1prong_%(year)s" --syst_mufake_1pi_scale="CMS_ZLShape_mt_1prong1pizero_%(year)s" --syst_lep_trg_diff="CMS_eff_*_%(year)s"' % vars()
            elif channel in ["et"]:
              add_syst = '--do_custom_uncerts --add_stat_to_syst --syst_tau_trg_diff="trg_syst_*" --syst_tau_id_diff="id_syst_*"  --do_ff_systs --syst_tquark="syst_ttbar_pt"  --syst_embedding_tt="syst_embed_tt"  --syst_tau_scale_grouped="CMS_scale_t_*group_%(year)s" --syst_res_j="CMS_res_j_%(year)s" --syst_scale_met_unclustered="CMS_scale_met_unclustered_%(year)s" --syst_scale_met="CMS_htt_boson_scale_met_%(year)s" --syst_res_met="CMS_htt_boson_reso_met_%(year)s" --syst_scale_j_regrouped="CMS_scale_j_*group" --syst_efake_0pi_scale="CMS_ZLShape_et_1prong_%(year)s" --syst_efake_1pi_scale="CMS_ZLShape_et_1prong1pizero_%(year)s" --syst_e_scale="CMS_scale_e" --syst_lep_trg_diff="CMS_eff_*_%(year)s"' % vars()
            elif channel in ["tt"]:
              add_syst = '--do_custom_uncerts --add_stat_to_syst --syst_tau_trg_diff="trg_syst_*" --syst_tau_id_diff="id_syst_*"  --do_ff_systs --syst_tquark="syst_ttbar_pt"  --syst_embedding_tt="syst_embed_tt"  --syst_tau_scale_grouped="CMS_scale_t_*group_%(year)s" --syst_res_j="CMS_res_j_%(year)s" --syst_scale_met_unclustered="CMS_scale_met_unclustered_%(year)s" --syst_scale_met="CMS_htt_boson_scale_met_%(year)s" --syst_res_met="CMS_htt_boson_reso_met_%(year)s" --syst_scale_j_regrouped="CMS_scale_j_*group"' % vars()
            if year == "2016":
              add_syst += ' --syst_zwt="CMS_htt_dyShape_2016" --syst_prefire="CMS_prefiring"'
            elif year == "2017":
              add_syst += ' --syst_prefire="CMS_prefiring" --syst_zwt="CMS_htt_dyShape"'
            elif year == "2018":
              add_syst += ' --syst_zwt="CMS_htt_dyShape"'
   
          
          extra_name = '%(var_string)s%(add_name)s' % vars()
          cmd += ' --extra_name=\'%(extra_name)s\' --outputfolder=%(output_folder)s %(add_syst)s --paramfile=scripts/params_vlq_2018.json' % vars()
          if var_string in ["mt_tot","m_vis","pt_1","pt_2","met","svfit_mass","jpt_1","jpt_2","pt_vis","pt_tt","dijetpt"]: cmd += ' --log_x'
          job_file = '%(cmssw_base)s/src/UserCode/ICHiggsTauTau/Analysis/HiggsTauTauRun2/%(output)s/jobs/vlq_plot_%(cat)s_%(year)s_%(channel)s_%(extra_name)s.sh' % vars()
          CreateBatchJob(job_file,cmssw_base,[cmd])
          if not args.dry_run: SubmitBatchJob(job_file,time=180,memory=24,cores=1)

           
    


