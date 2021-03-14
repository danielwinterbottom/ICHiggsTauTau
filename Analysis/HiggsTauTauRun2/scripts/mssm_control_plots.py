#!/usr/bin/env python

# python scripts/mssm_control_plots.py --channel='mt,et,tt' --year='2016,2017,2018' --output='control_plots_mssm'
# python scripts/mssm_control_plots.py --channel='mt,et,tt' --year='2018' --output='control_plots_mssm_new' --control_plots=True --ff_closures=True --add_systs=True --dry_run=True

import os
import argparse
import math as math

parser = argparse.ArgumentParser()
parser.add_argument('--channel',help= 'Name of input channels', default='mt,et,tt')
parser.add_argument('--year', help= 'Name of input years', default='2016,2017,2018')
parser.add_argument('--method', help= 'Name of input methods', default='17')
parser.add_argument('--output', help= 'Name of output folder to create', default='mssm_control_plots')
parser.add_argument('--ff_closures', help= 'Draw fake factor DR closure plots', default='False')
parser.add_argument('--control_plots', help= 'Draw control plots (Set as false if you only want ff closures)', default='True')
parser.add_argument('--add_systs', help= 'Add systematic', default='False')
parser.add_argument('--dry_run', help= 'Only create jobs, does not run them', default='False')
args = parser.parse_args()
      
# Things to loop over
channels = args.channel.split(',')
methods = args.method.split(',')
years = args.year.split(',')
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

all_ch_variables = [
                    GetBinning('mt_tot',20,3200,50,log=True,round=1,final_bin_start=1000),
                    GetBinning('m_vis',20,3200,50,log=True,round=1,final_bin_start=1000),
                    GetBinning('met',10,600,40,log=True,round=1,final_bin_start=None),
                    'n_jets[0,1,>=2]',
                    'n_prebjets[0,1,>=2]', 
                    'n_deepbjets[0,>=1]',
                    GetBinning('mt_1',0,400,40,log=False,round=1,final_bin_start=None),
                    GetBinning('mt_2',0,400,40,log=False,round=1,final_bin_start=None),
                    GetBinning('mt_lep',0,400,40,log=False,round=1,final_bin_start=None),
                    #GetBinning('eta_1',-2.2,2.2,40,log=False,round=0.05,final_bin_start=None),
                    #GetBinning('eta_2',-2.2,2.2,40,log=False,round=0.05,final_bin_start=None),
                    #GetBinning('met_dphi_1',0,3.2,40,log=False,round=0.05,final_bin_start=None),
                    #GetBinning('met_dphi_2',0,3.2,40,log=False,round=0.05,final_bin_start=None),
                    #GetBinning('dphi',0,3.2,40,log=False,round=0.05,final_bin_start=None),
                    ]


#all_ch_variables = [
#             'mt_tot[20,30,40,50,60,70,80,90,100,110,125,140,160,175,200,225,250,280,320,350,400,450,500,560,630,710,800,890,1000,4000]',
#             'm_vis[20,30,40,50,60,70,80,90,100,110,125,140,160,175,200,225,250,280,320,350,400,450,500,560,630,710,800,890,1000,4000]',
#             'met[0,10,20,30,40,50,60,70,80,90,100,120,140,200,400]',
#             'n_jets[0,1,>=2]',
#             'n_prebjets[0,1,>=2]', 
#             'n_deepbjets[0,>=1]',
#             'mt_1[0,20,40,60,80,100,120,140,160,180,200,220,240,260,280,300]',
#             'mt_2[0,20,40,60,80,100,120,140,160,180,200,220,240,260,280,300]',
#             'mt_lep[0,20,40,60,80,100,120,140,160,180,200,220,240,260,280,300]',
#             'eta_1[-2.2,-2.0,-1.8,-1.6,-1.4,-1.2,-1.0,-0.8,-0.6,-0.4,-0.2,0.0,0.2,0.4,0.6,0.8,1.0,1.2,1.4,1.6,1.8,2.0,2.2]',
#             'eta_2[-2.2,-2.0,-1.8,-1.6,-1.4,-1.2,-1.0,-0.8,-0.6,-0.4,-0.2,0.0,0.2,0.4,0.6,0.8,1.0,1.2,1.4,1.6,1.8,2.0,2.2]',
#             'met_dphi_1[0.0,0.2,0.4,0.6,0.8,1.0,1.2,1.4,1.6,1.8,2.0,2.2,2.4,2.6,2.8,3,3.2]',
#             'met_dphi_2[0.0,0.2,0.4,0.6,0.8,1.0,1.2,1.4,1.6,1.8,2.0,2.2,2.4,2.6,2.8,3,3.2]',
#             'dphi[0.0,0.2,0.4,0.6,0.8,1.0,1.2,1.4,1.6,1.8,2.0,2.2,2.4,2.6,2.8,3,3.2]'
#            ]


ch_dep_var = {"mt":[
                    GetBinning('pt_1',20,600,40,log=True,round=1,final_bin_start=None),
                    GetBinning('pt_2',30,600,40,log=True,round=1,final_bin_start=None),
                    GetBinning('iso_1',0,0.15,40,log=False,round=0.001,final_bin_start=None),
                    ],
              "et":[
                    GetBinning('pt_1',20,600,40,log=True,round=1,final_bin_start=None),
                    GetBinning('pt_2',30,600,40,log=True,round=1,final_bin_start=None),
                    GetBinning('iso_1',0,0.15,40,log=False,round=0.001,final_bin_start=None),
                    ],
              "tt":[
                    GetBinning('pt_1',40,600,40,log=True,round=1,final_bin_start=None),
                    GetBinning('pt_2',40,600,40,log=True,round=1,final_bin_start=None),
                    ]
              }


#ch_dep_var = {"mt":[
#                    'pt_1[20,25,30,35,40,45,50,55,60,65,70,80,90,100,120,140,200,400]',
#                    'pt_2[30,35,40,45,50,55,60,65,70,80,90,100,120,140,200,400]',
#                    'iso_1[0.0,0.005,0.01,0.015,0.02,0.025,0.03,0.035,0.04,0.045,0.05,0.055,0.06,0.065,0.07,0.075,0.08,0.085,0.09,0.095,0.1,0.105,0.11,0.115,0.12,0.125,0.13,0.135,0.14,0.145,0.15]'
#                    ],
#              "et":[
#                    'pt_1[20,25,30,35,40,45,50,55,60,65,70,80,90,100,120,140,200,400]',
#                    'pt_2[30,35,40,45,50,55,60,65,70,80,90,100,120,140,200,400]',
#                    'iso_1[0.0,0.005,0.01,0.015,0.02,0.025,0.03,0.035,0.04,0.045,0.05,0.055,0.06,0.065,0.07,0.075,0.08,0.085,0.09,0.095,0.1,0.105,0.11,0.115,0.12,0.125,0.13,0.135,0.14,0.145,0.15]'
#                    ],
#              "tt":[
#                    'pt_1[40,45,50,55,60,65,70,80,90,100,120,140,200,400]',
#                    'pt_2[40,45,50,55,60,65,70,80,90,100,120,140,200,400]'
#                    ]
#              }

config_files = {'2016':'scripts/plot_mssm_2016.cfg',
                '2017':'scripts/plot_mssm_2017.cfg',
                '2018':'scripts/plot_mssm_2018.cfg'
               }


add_options = '--embedding --ggh_masses=\'\' --bbh_nlo_masses=\'\' --ratio --norm_bins --ratio_range=\'0.6,1.4\''
 
# Set up output directories

cmssw_base = os.getcwd()

if not os.path.isdir('%(cmssw_base)s/%(output)s' % vars()):
  os.system("mkdir %(cmssw_base)s/%(output)s" % vars())

if not os.path.isdir('%(cmssw_base)s/%(output)s/jobs' % vars()):
  os.system("mkdir %(cmssw_base)s/%(output)s/jobs" % vars())

for channel in channels:
  if not os.path.isdir('%(cmssw_base)s/%(output)s/%(channel)s' % vars()):
    os.system("mkdir %(cmssw_base)s/%(output)s/%(channel)s" % vars())
  for year in years:
    if not os.path.isdir('%(cmssw_base)s/%(output)s/%(channel)s/%(year)s' % vars()):
      os.system("mkdir %(cmssw_base)s/%(output)s/%(channel)s/%(year)s" % vars())

if eval(args.ff_closures):
  if not os.path.isdir('%(cmssw_base)s/%(output)s/closure_plots' % vars()):
    os.system("mkdir %(cmssw_base)s/%(output)s/closure_plots" % vars())

  for channel in channels:
    if not os.path.isdir('%(cmssw_base)s/%(output)s/closure_plots/%(channel)s' % vars()):
      os.system("mkdir %(cmssw_base)s/%(output)s/closure_plots/%(channel)s" % vars())
    for year in years:
      if not os.path.isdir('%(cmssw_base)s/%(output)s/closure_plots/%(channel)s/%(year)s' % vars()):
        os.system("mkdir %(cmssw_base)s/%(output)s/closure_plots/%(channel)s/%(year)s" % vars())


# Job loop

for year in years:
  for channel in channels:
    for method in methods:
      variables = all_ch_variables + ch_dep_var[channel]
      for var in variables:
        if '[' in var: var_string = var.split('[')[0]
        elif '(' in var: var_string = var.split('(')[0]
   
        cfg = config_files[year]
        run_cmd = 'python %(cmssw_base)s/scripts/HiggsTauTauPlot.py --cfg=\'%(cfg)s\' --channel=%(channel)s --method=%(method)s --var=\'%(var)s\' %(add_options)s' % vars()

        if eval(args.add_systs):
          run_cmd_w_closure = 'python %(cmssw_base)s/scripts/HiggsTauTauPlot.py --cfg=\'%(cfg)s\' --channel=%(channel)s --method=%(method)s --var=\'%(var)s\' %(add_options)s --w_ff_closure --do_custom_uncerts --add_stat_to_syst --do_ff_syst' % vars()
          run_cmd_qcd_closure = 'python %(cmssw_base)s/scripts/HiggsTauTauPlot.py --cfg=\'%(cfg)s\' --channel=%(channel)s --method=%(method)s --var=\'%(var)s\' %(add_options)s --qcd_ff_closure --do_custom_uncerts --add_stat_to_syst --do_ff_syst' % vars()
        else:
          run_cmd_w_closure = 'python %(cmssw_base)s/scripts/HiggsTauTauPlot.py --cfg=\'%(cfg)s\' --channel=%(channel)s --method=%(method)s --var=\'%(var)s\' %(add_options)s --w_ff_closure' % vars()
          run_cmd_qcd_closure = 'python %(cmssw_base)s/scripts/HiggsTauTauPlot.py --cfg=\'%(cfg)s\' --channel=%(channel)s --method=%(method)s --var=\'%(var)s\' %(add_options)s --qcd_ff_closure' % vars()

        run_list = []
        if channel in ["et","mt"]: 
           if eval(args.ff_closures):
             run_list.append(run_cmd_w_closure)
             run_list.append(run_cmd_qcd_closure)
           if eval(args.control_plots):
             run_list.append(run_cmd)
        elif channel in ["tt"]: 
           if eval(args.ff_closures):
             run_list.append(run_cmd_qcd_closure)
           if eval(args.control_plots):
             run_list.append(run_cmd)
        
        for cmd in run_list:
         
          if "w_ff_closure" in cmd: 
            add_name = '_w_dr'
            output_folder = '%(cmssw_base)s/%(output)s/closure_plots/%(channel)s/%(year)s' % vars()
            add_syst = ''
          elif "qcd_ff_closure" in cmd: 
            add_name = '_qcd_dr' 
            output_folder = '%(cmssw_base)s/%(output)s/closure_plots/%(channel)s/%(year)s' % vars()
            add_syst = ''
          else:
            add_name = ''
            output_folder = '%(cmssw_base)s/%(output)s/%(channel)s/%(year)s' % vars()
            if channel in ["mt"]:
              if eval(args.add_systs): 
                add_syst = '--do_custom_uncerts --add_stat_to_syst --syst_tau_trg_diff="trg_syst_*" --syst_tau_id_diff="id_syst_*"  --do_ff_systs --syst_tquark="syst_ttbar_pt"  --syst_embedding_tt="syst_embed_tt"  --syst_tau_scale_grouped="CMS_scale_t_*group_%(year)s" --syst_res_j="CMS_res_j_%(year)s" --syst_scale_met_unclustered="CMS_scale_met_unclustered_%(year)s" --syst_scale_met="CMS_htt_boson_scale_met_%(year)s" --syst_res_met="CMS_htt_boson_reso_met_%(year)s" --syst_scale_j_regrouped="CMS_scale_j_*group" --syst_mufake_0pi_scale="CMS_ZLShape_mt_1prong_%(year)s" --syst_mufake_1pi_scale="CMS_ZLShape_mt_1prong1pizero_%(year)s" --syst_lep_trg_diff="CMS_eff_*_%(year)s"' % vars()
              else:
                add_syst = ''
              cmd += " --sel=\'mt_1<70\'"
            elif channel in ["et"]:
              if eval(args.add_systs): 
                add_syst = '--do_custom_uncerts --add_stat_to_syst --syst_tau_trg_diff="trg_syst_*" --syst_tau_id_diff="id_syst_*"  --do_ff_systs --syst_tquark="syst_ttbar_pt"  --syst_embedding_tt="syst_embed_tt"  --syst_tau_scale_grouped="CMS_scale_t_*group_%(year)s" --syst_res_j="CMS_res_j_%(year)s" --syst_scale_met_unclustered="CMS_scale_met_unclustered_%(year)s" --syst_scale_met="CMS_htt_boson_scale_met_%(year)s" --syst_res_met="CMS_htt_boson_reso_met_%(year)s" --syst_scale_j_regrouped="CMS_scale_j_*group" --syst_efake_0pi_scale="CMS_ZLShape_et_1prong_%(year)s" --syst_efake_1pi_scale="CMS_ZLShape_et_1prong1pizero_%(year)s" --syst_e_scale="CMS_scale_e" --syst_lep_trg_diff="CMS_eff_*_%(year)s"' % vars()
              else:
                add_syst = ''
              cmd += " --sel=\'mt_1<70\'"

            elif channel in ["tt"]:
              if eval(args.add_systs):
                add_syst = '--do_custom_uncerts --add_stat_to_syst --syst_tau_trg_diff="trg_syst_*" --syst_tau_id_diff="id_syst_*"  --do_ff_systs --syst_tquark="syst_ttbar_pt"  --syst_embedding_tt="syst_embed_tt"  --syst_tau_scale_grouped="CMS_scale_t_*group_%(year)s" --syst_res_j="CMS_res_j_%(year)s" --syst_scale_met_unclustered="CMS_scale_met_unclustered_%(year)s" --syst_scale_met="CMS_htt_boson_scale_met_%(year)s" --syst_res_met="CMS_htt_boson_reso_met_%(year)s" --syst_scale_j_regrouped="CMS_scale_j_*group"' % vars()
              else:
                add_syst = ''

          if year == "2016":
            wt = "--add_wt=\'wt_tau_trg_mssm*wt_tau_id_mssm*wt_prefire\'"
            if eval(args.add_systs):
              add_syst += ' --syst_zwt="CMS_htt_dyShape_2016" --syst_prefire="CMS_prefiring"'
          elif year == "2017":
            wt = "--add_wt=\'wt_tau_trg_mssm*wt_tau_id_mssm*wt_prefire\'"
            if eval(args.add_systs):
              add_syst += ' --syst_prefire="CMS_prefiring" --syst_zwt="CMS_htt_dyShape"'
          elif year == "2018":
            wt = "--add_wt=\'wt_tau_trg_mssm*wt_tau_id_mssm\'"
            if eval(args.add_systs):
              add_syst += ' --syst_zwt="CMS_htt_dyShape"'
 

          extra_name = '%(var_string)s%(add_name)s' % vars()
          cmd += ' --extra_name=\'%(extra_name)s\' --outputfolder=%(output_folder)s %(add_syst)s %(wt)s' % vars()
          if var_string in ["mt_tot","m_vis","pt_1","pt_2","met"]: cmd += ' --log_x'
          job_file = '%(cmssw_base)s/%(output)s/jobs/mssm_control_plot_%(year)s_%(channel)s_%(extra_name)s.sh' % vars()
          if os.path.exists(job_file): os.system('rm %(job_file)s' % vars())
          os.system('echo "#!/bin/bash" >> %(job_file)s' % vars())
          os.system('echo "cd %(cmssw_base)s" >> %(job_file)s' % vars())
          os.system('echo "source /vols/grid/cms/setup.sh" >> %(job_file)s' % vars()) 
          os.system('echo "export SCRAM_ARCH=slc6_amd64_gcc481" >> %(job_file)s' % vars())
          os.system('echo "eval \'scramv1 runtime -sh\'" >> %(job_file)s' % vars())
          os.system('echo "source %(cmssw_base)s/scripts/setup_libs.sh" >> %(job_file)s' % vars())
          os.system('echo "ulimit -c 0" >> %(job_file)s' % vars())
          os.system('echo "%(cmd)s" >> %(job_file)s' % vars())
          os.system('echo "rm %(output_folder)s/*.png" >> %(job_file)s' % vars())
          os.system('chmod +x %(job_file)s' % vars())

          print "Created job:",job_file
          error_file = job_file.replace('.sh','_error.log')
          output_file = job_file.replace('.sh','_output.log')

          if os.path.exists(error_file): os.system('rm %(error_file)s' % vars())
          if os.path.exists(output_file): os.system('rm %(output_file)s' % vars())

          if not eval(args.dry_run):
            if 'w_dr' in job_file or 'qcd_dr' in job_file or not eval(args.add_systs) or not ((year in ["2017","2018"]) and (channel in ['mt','et'])):
              os.system('qsub -e %(error_file)s -o %(output_file)s -V -q hep.q -l h_rt=0:180:0 -cwd %(job_file)s' % vars())
            else:
              os.system('qsub -e %(error_file)s -o %(output_file)s -V -q hep.q -l h_rt=10:0:0 -cwd %(job_file)s' % vars())

           
    


