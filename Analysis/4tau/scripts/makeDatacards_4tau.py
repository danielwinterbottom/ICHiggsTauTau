#!/usr/bin/env python

# python scripts/makeDatacards_4tau.py --channel=mmtt --output=0512
# python scripts/makeDatacards_4tau.py --channel=mmtt --output=0512 --only_var=mvis_os --only_option=signal

import os
import argparse
import math as math
import copy

parser = argparse.ArgumentParser()
parser.add_argument('--channel',help= 'Name of input channels', default='mttt,ettt,tttt,eett,mmtt,emtt,ttt')
parser.add_argument('--output', help= 'Name of output folder to create', default='4tau_plots')
parser.add_argument('--only_var', help= 'Only run for this variable', default='')
parser.add_argument('--only_option', help= 'Only run for this option', default='')
parser.add_argument("--no_syst",dest="no_syst", action='store_true', default=False,help="Run without systematics")
args = parser.parse_args()
      
# Things to loop over
channels = args.channel.split(',')
output = args.output

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

def CreateBatchJob(name,cmssw_base,cmd_list):
  if os.path.exists(job_file): os.system('rm %(name)s' % vars())
  os.system('echo "#!/bin/bash" >> %(name)s' % vars())
  os.system('echo "cd %(cmssw_base)s/src/UserCode/ICHiggsTauTau/Analysis/4tau" >> %(name)s' % vars())
  os.system('echo "source /vols/grid/cms/setup.sh" >> %(name)s' % vars())
  os.system('echo "export SCRAM_ARCH=slc6_amd64_gcc481" >> %(name)s' % vars())
  os.system('echo "eval \'scramv1 runtime -sh\'" >> %(name)s' % vars())
  os.system('echo "source %(cmssw_base)s/src/UserCode/ICHiggsTauTau/Analysis/4tau/scripts/setup_libs.sh" >> %(name)s' % vars())
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

all_ch_variables = [
                    GetBinning('mt_tot',0,600,60,round=1),
#                    GetBinning('st',0,600,100,round=1),
                    GetBinning('pt_1',0,250,25,round=1),
                    GetBinning('pt_2',0,250,25,round=1),
                    GetBinning('pt_3',0,250,25,round=1),
                    GetBinning('pt_4',0,250,25,round=1),
#                    GetBinning('mt_1',0,250,50,round=1),
#                    GetBinning('mt_2',0,200,40,round=1),
#                    GetBinning('mt_3',0,150,30,round=1),
#                    GetBinning('mt_4',0,150,30,round=1),
#                    GetBinning('mt_lep_12',0,400,100,round=1),
#                    GetBinning('mt_lep_13',0,400,100,round=1),
#                    GetBinning('mt_lep_14',0,400,100,round=1),
#                    GetBinning('mt_lep_23',0,400,100,round=1),
#                    GetBinning('mt_lep_24',0,400,100,round=1),
#                    GetBinning('mt_lep_34',0,400,100,round=1),
#                    GetBinning('mvis_12',0,400,100,round=1),
#                    GetBinning('mvis_13',0,400,100,round=1),
#                    GetBinning('mvis_14',0,400,100,round=1),
#                    GetBinning('mvis_23',0,400,100,round=1),
#                    GetBinning('mvis_24',0,400,100,round=1),
#                    GetBinning('mvis_34',0,400,100,round=1),
                    GetBinning('mvis_1234',80,600,100,round=1),
#                    GetBinning('mvis_min_sum_dR_1',0,500,100,round=1),
#                    GetBinning('mvis_min_sum_dR_2',0,300,60,round=1),
#                    GetBinning('mvis_min_sum_dphi_1',0,500,100,round=1),
#                    GetBinning('mvis_min_sum_dphi_2',0,300,60,round=1),
#                    GetBinning('mvis_min_dR_1',0,500,100,round=1),
#                    GetBinning('mvis_min_dR_2',0,300,60,round=1),
#                    GetBinning('mvis_min_dphi_1',0,500,100,round=1),
#                    GetBinning('mvis_min_dphi_2',0,300,60,round=1),
#                    GetBinning('pt_min_sum_dR_1',0,300,60,round=1),
#                    GetBinning('pt_min_sum_dR_2',0,300,60,round=1),
#                    GetBinning('pt_min_sum_dphi_1',0,300,60,round=1),
#                    GetBinning('pt_min_sum_dphi_2',0,300,60,round=1),
#                    GetBinning('pt_min_dR_1',0,300,60,round=1),
#                    GetBinning('pt_min_dR_2',0,300,60,round=1),
#                    GetBinning('pt_min_dphi_1',0,300,60,round=1),
#                    GetBinning('pt_min_dphi_2',0,300,60,round=1),
#                    GetBinning('pt_tt_12',0,300,60,round=1),
#                    GetBinning('pt_tt_13',0,300,60,round=1),
#                    GetBinning('pt_tt_14',0,300,60,round=1),
#                    GetBinning('pt_tt_23',0,300,60,round=1),
#                    GetBinning('pt_tt_24',0,300,60,round=1),
#                    GetBinning('pt_tt_34',0,300,60,round=1),
#                    GetBinning('pt_tttt',0,400,80,round=1),
#                    GetBinning('met',0,250,50,round=1),
#                    "n_jets[0,1,2,3,4,5,6,7,8,9,10]",
#                    "n_bjets[0,1,2,3,4]",
#                    GetBinning('eta_1',-4.0,4.0,40,round=0.05),
#                    GetBinning('eta_2',-4.0,4.0,40,round=0.05),
#                    GetBinning('eta_3',-4.0,4.0,40,round=0.05),
#                    GetBinning('eta_4',-4.0,4.0,40,round=0.05),
#                    GetBinning('dR_12',0,5.0,50,round=0.1),
#                    GetBinning('dR_13',0,5.0,50,round=0.1),
#                    GetBinning('dR_14',0,5.0,50,round=0.1),
#                    GetBinning('dR_23',0,5.0,50,round=0.1),
#                    GetBinning('dR_24',0,5.0,50,round=0.1),
#                    GetBinning('dR_34',0,5.0,50,round=0.1),
#                    GetBinning('dphi_12',-3.2,3.2,40,round=0.05),
#                    GetBinning('dphi_13',-3.2,3.2,40,round=0.05),
#                    GetBinning('dphi_14',-3.2,3.2,40,round=0.05),
#                    GetBinning('dphi_23',-3.2,3.2,40,round=0.05),
#                    GetBinning('dphi_24',-3.2,3.2,40,round=0.05),
#                    GetBinning('dphi_34',-3.2,3.2,40,round=0.05), 
#                    "tau_decay_mode_2[0,1,2,3,4,5,6,7,8,9,10,11,12]",
#                    "tau_decay_mode_3[0,1,2,3,4,5,6,7,8,9,10,11,12]",
#                    "tau_decay_mode_4[0,1,2,3,4,5,6,7,8,9,10,11,12]",
                    ]

ch_dep_var = {"mttt":[
                      ],
              "ettt":[
                      ],
              "mmtt":[
                      ],
              "eett":[
                      ],
              "tttt":[
                      ],
              "emtt":[
                      ],
              "ttt" :[
#                      GetBinning('mvis_os',0,400,100,round=1),
#                      GetBinning('pt_tt_os',0,300,60,round=1),
                      ],
	      "mmmm":[
	              ],
              }

categories = {
              "ttt" : ["inclusive"],
              "tttt": ["inclusive"],
              "ettt": ["nobtag"],
              "mttt": ["nobtag"],
              "emtt": ["z_control_nobtag","2l2t_sig_nobtag"],
              "eett": ["z_control_nobtag","2l2t_sig_nobtag"],
              "mmtt": ["z_control_nobtag","2l2t_sig_nobtag"],
	      "mmmm": ["inclusive"],
              }


add_options = {              
               "ttt" : [
                        ["signal","--blind --x_blind_min=-999 --x_blind_max=999 --plot_signal=ZstarTophi200A60To4Tau,ZstarTophi300A60To4Tau"],
                        ["ff_full","--no_signal --charges_non_zero --under_legend='FF_{total}' --rebin_with_data"],
                        ["ff_1","--no_signal --ff_from=1 --no_sig_sel --under_legend='FF_{1}' --rebin_with_data"],
                        ["ff_2","--no_signal --ff_from=2 --no_sig_sel --under_legend='FF_{2}' --rebin_with_data"],
                        ["ff_3","--no_signal --ff_from=3 --no_sig_sel --under_legend='FF_{3}' --rebin_with_data"],
                        ["ff_12","--no_signal --ff_from=12 --no_sig_sel --under_legend='FF_{1} x FF_{2}' --rebin_with_data"],
                        ["ff_13","--no_signal --ff_from=13 --no_sig_sel --under_legend='FF_{1} x FF_{3}' --rebin_with_data"],
                        ["ff_23","--no_signal --ff_from=23 --no_sig_sel --under_legend='FF_{2} x FF_{3}' --rebin_with_data"],
                        ["ff_123","--no_signal --charges_non_zero --ff_from=123 --under_legend='FF_{1} x FF_{2} x FF_{3}' --rebin_with_data"],
                        ],
               "tttt": [
                        ["signal","--blind --x_blind_min=-999 --x_blind_max=999 --plot_signal=ZstarTophi200A60To4Tau,ZstarTophi300A60To4Tau"],
                        ["ff_full","--no_signal --charges_non_zero --under_legend='FF_{total}' --rebin_with_data"],
                        ["ff_1","--no_signal --ff_from=1 --no_sig_sel --under_legend='FF_{1}' --rebin_with_data"],
                        ["ff_2","--no_signal --ff_from=2 --no_sig_sel --under_legend='FF_{2}' --rebin_with_data"],
                        ["ff_3","--no_signal --ff_from=3 --no_sig_sel --under_legend='FF_{3}' --rebin_with_data"],
                        ["ff_4","--no_signal --ff_from=4 --no_sig_sel --under_legend='FF_{4}' --rebin_with_data"],
                        ["ff_12","--no_signal --ff_from=12 --no_sig_sel --under_legend='FF_{1} x FF_{2}' --rebin_with_data"],
                        ["ff_13","--no_signal --ff_from=13 --no_sig_sel --under_legend='FF_{1} x FF_{3}' --rebin_with_data"],
                        ["ff_14","--no_signal --ff_from=14 --no_sig_sel --under_legend='FF_{1} x FF_{4}' --rebin_with_data"],
                        ["ff_23","--no_signal --ff_from=23 --no_sig_sel --under_legend='FF_{2} x FF_{3}' --rebin_with_data"],
                        ["ff_24","--no_signal --ff_from=24 --no_sig_sel --under_legend='FF_{2} x FF_{4}' --rebin_with_data"],
                        ["ff_34","--no_signal --ff_from=34 --no_sig_sel --under_legend='FF_{3} x FF_{4}' --rebin_with_data"],
                        ["ff_123","--no_signal --ff_from=123 --no_sig_sel --under_legend='FF_{1} x FF_{2} x FF_{3}' --rebin_with_data"],
                        ["ff_124","--no_signal --ff_from=124 --no_sig_sel --under_legend='FF_{1} x FF_{2} x FF_{4}' --rebin_with_data"],
                        ["ff_134","--no_signal --ff_from=134 --no_sig_sel --under_legend='FF_{1} x FF_{3} x FF_{4}' --rebin_with_data"],
                        ["ff_234","--no_signal --ff_from=234 --no_sig_sel --under_legend='FF_{2} x FF_{3} x FF_{4}' --rebin_with_data"],
                        ["ff_1234","--no_signal --charges_non_zero --ff_from=1234 --under_legend='FF_{1} x FF_{2} x FF_{3} x FF_{4}' --rebin_with_data"],
                        ],
               "ettt": [
                        ["signal","--blind --x_blind_min=-999 --x_blind_max=999 --plot_signal=ZstarTophi200A60To4Tau,ZstarTophi300A60To4Tau"],
                        ["ff_full","--no_signal --charges_non_zero --under_legend='FF_{total}' --rebin_with_data"],
                        ["ff_2","--no_signal --ff_from=2 --no_sig_sel --under_legend='FF_{1}' --rebin_with_data"],
                        ["ff_3","--no_signal --ff_from=3 --no_sig_sel --under_legend='FF_{2}' --rebin_with_data"],
                        ["ff_4","--no_signal --ff_from=4 --no_sig_sel --under_legend='FF_{3}' --rebin_with_data"],
                        ["ff_23","--no_signal --ff_from=23 --no_sig_sel --under_legend='FF_{1} x FF_{2}' --rebin_with_data"],
                        ["ff_24","--no_signal --ff_from=24 --no_sig_sel --under_legend='FF_{1} x FF_{3}' --rebin_with_data"],
                        ["ff_34","--no_signal --ff_from=34 --no_sig_sel --under_legend='FF_{2} x FF_{3}' --rebin_with_data"],
                        ["ff_234","--no_signal --charges_non_zero --ff_from=123 --under_legend='FF_{1} x FF_{2} x FF_{3}' --rebin_with_data"],
                        ],
               "mttt": [
                        ["signal","--blind --x_blind_min=-999 --x_blind_max=999 --plot_signal=ZstarTophi200A60To4Tau,ZstarTophi300A60To4Tau"],
                        ["ff_full","--no_signal --charges_non_zero --under_legend='FF_{total}' --rebin_with_data"],
                        ["ff_2","--no_signal --ff_from=2 --no_sig_sel --under_legend='FF_{1}' --rebin_with_data"],
                        ["ff_3","--no_signal --ff_from=3 --no_sig_sel --under_legend='FF_{2}' --rebin_with_data"],
                        ["ff_4","--no_signal --ff_from=4 --no_sig_sel --under_legend='FF_{3}' --rebin_with_data"],
                        ["ff_23","--no_signal --ff_from=23 --no_sig_sel --under_legend='FF_{1} x FF_{2}' --rebin_with_data"],
                        ["ff_24","--no_signal --ff_from=24 --no_sig_sel --under_legend='FF_{1} x FF_{3}' --rebin_with_data"],
                        ["ff_34","--no_signal --ff_from=34 --no_sig_sel --under_legend='FF_{2} x FF_{3}' --rebin_with_data"],
                        ["ff_234","--no_signal --charges_non_zero --ff_from=123 --under_legend='FF_{1} x FF_{2} x FF_{3}' --rebin_with_data"],
                        ],
               "emtt": [
                        ["signal","--blind --x_blind_min=-999 --x_blind_max=999 --plot_signal=ZstarTophi200A60To4Tau,ZstarTophi300A60To4Tau"],
                        ["ff_full","--no_signal --charges_non_zero --under_legend='FF_{total}'  --rebin_with_data"],
                        ["ff_3","--no_signal --ff_from=3 --no_sig_sel --under_legend='FF_{1}' --rebin_with_data"],
                        ["ff_4","--no_signal --ff_from=4 --no_sig_sel --under_legend='FF_{2}' --rebin_with_data"],
                        ["ff_34","--no_signal --charges_non_zero --ff_from=34 --under_legend='FF_{1} x FF_{2}' --rebin_with_data"],
                        ],
               "eett": [
                        ["signal","--plot_signal=ZstarTophi200A60To4Tau,ZstarTophi300A60To4Tau"],
                        ["ff_full","--no_signal --charges_non_zero --under_legend='FF_{total}' --rebin_with_data"],
                        ["ff_3","--no_signal --ff_from=3 --no_sig_sel --under_legend='FF_{1}' --rebin_with_data"],
                        ["ff_4","--no_signal --ff_from=4 --no_sig_sel --under_legend='FF_{2}' --rebin_with_data"],
                        ["ff_34","--no_signal --charges_non_zero --ff_from=34 --under_legend='FF_{1} x FF_{2}' --rebin_with_data"],
                        ],
               "mmtt": [
                        ["signal","--plot_signal=ZstarTophi200A60To4Tau,ZstarTophi300A60To4Tau"],
                        ["ff_full","--no_signal --charges_non_zero --under_legend='FF_{total}' --rebin_with_data"],
                        ["ff_3","--no_signal --ff_from=3 --no_sig_sel --under_legend='FF_{1}' --rebin_with_data"],
                        ["ff_4","--no_signal --ff_from=4 --no_sig_sel --under_legend='FF_{2}' --rebin_with_data"],
                        ["ff_34","--no_signal --charges_non_zero --ff_from=34 --under_legend='FF_{1} x FF_{2}' --rebin_with_data"],
                        ],                    
	       "mmmm":[
		       ["signal",""],
		      ],
               }

### Systematics ###
common_shape_systematics = [
#        '--syst_tau_id', # Tau ID Efficiency
#        '--syst_doubletau_trg', # Double Tau Trigger Effieciency
#        '--syst_tau_scale_group', #Tau Energy Scale
#        '--syst_jet_res', # Jet Energy Resolution
#        '--syst_met_unclustered', # MET Unclustered Energy Uncertainty
#       '--syst_met_scale', # MET Recoil Scale Correction Uncertainty
#       '--syst_met_res', # MET Recoil Resolution Correction Uncertainty
        '--syst_jet_scale_group', # Jet Energy Scale Grouped
#        '--syst_electron_scale', # Electron Energy Scale
#       '--syst_efake_scale_0pi', # l to tau h fake energy scale
#       '--syst_efake_scale_1pi', # l to tau h fake energy scale
#       '--syst_muon_scale', # Muon Energy Scale
#       '--syst_mufake_scale_0pi', # l to tau h fake energy scale
#       '--syst_mufake_scale_1pi', # l to tau h fake energy scale
#        '--syst_electron_id', # Electron ID
#        '--syst_muon_id', # Muon ID
#        '--syst_electron_trg', # SingleElectron trigger
#        '--syst_muon_trg', # SingleMuon trigger
#        '--syst_k_factor', # ZZTo4L k factors
#        '--do_ff_systs', # Fake factor uncertainties
#        '--syst_prefire', # Prefiring
#        '--syst_signal_theory', # signal theory uncertainty
	]

# Set up output directories

cmssw_base = os.getcwd()

if not os.path.isdir('%(cmssw_base)s/%(output)s' % vars()):
  os.system("mkdir %(cmssw_base)s/%(output)s" % vars())

if not os.path.isdir('%(cmssw_base)s/%(output)s/jobs' % vars()):
  os.system("mkdir %(cmssw_base)s/%(output)s/jobs" % vars())

for channel in channels:
  if not os.path.isdir('%(cmssw_base)s/%(output)s/%(channel)s' % vars()):
    os.system("mkdir %(cmssw_base)s/%(output)s/%(channel)s" % vars())


# Job loop
for channel in channels:
  systs = copy.deepcopy(common_shape_systematics)
 # if "e" not in channel: 
 #   systs.remove("--syst_electron_scale")
 #   systs.remove("--syst_electron_id")
 # elif "m" not in channel:
 #   systs.remove("--syst_muon_id")

  if channel == "emtt": systs.remove("--syst_tau_scale_group") # temporary
  
  for name, option in add_options[channel]:
    for cat in categories[channel]:
      variables = all_ch_variables+ch_dep_var[channel]
      for var in variables:
        if '[' in var: var_string = var.split('[')[0]
        elif '(' in var: var_string = var.split('(')[0]
        if args.only_var != "" and args.only_var != var_string: continue
        if args.only_option != "" and args.only_option != name: continue
        if "ff" in name and "ff_full" not in name and cat != "inclusive": continue
        if var_string[-1] == "4" and channel == "ttt": continue
        output_folder = '%(cmssw_base)s/%(output)s/%(channel)s' % vars()
        combined_options = ""
        if "tau_decay_mode" not in var: combined_options = " --combined_options=\\\"--auto_rebinning --rebin_with_data --bin_uncert_fraction=0.25\\\""
        add_cond = ""
        if not (args.no_syst or "ff" in name):
           print "Adding Systematics"
           for syst in systs:
              add_cond += (syst + " ")
        if "ff" in name:
          add_cond += "--do_ff_systs"

        if channel in ["mmtt","eett"] and cat == "2l2t_sig_nobtag": option += " --blind --x_blind_min=-999 --x_blind_max=999"
        run_cmd = "python %(cmssw_base)s/scripts/combined_year_4tauPlot.py --outputfolder=%(output_folder)s --options=\\\"--folder=/vols/cms/gu18/Offline/output/4tau/2301 %(option)s --method=1 --var=\'%(var)s\' --vsjets=loose --ratio_range=0,3 %(add_cond)s --add_stat_to_syst --add_wt=\'(1/wt_ggZZ_k_factor)*(1/wt_qqZZ_k_factor)\' \\\" %(combined_options)s --channel=%(channel)s --cat=%(cat)s --run_datacards --extra_name=%(var_string)s_%(name)s" % vars()
        job_file = "%(cmssw_base)s/%(output)s/jobs/%(var_string)s_%(channel)s_%(cat)s_%(name)s.sh" % vars()
        CreateBatchJob(job_file,os.getcwd().replace('src/UserCode/ICHiggsTauTau/Analysis/4tau',''),[run_cmd])
        SubmitBatchJob(job_file,time=180,memory=24,cores=1)
           
    

