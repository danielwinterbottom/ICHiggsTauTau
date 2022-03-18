#!/usr/bin/env python

# python scripts/chi_control_plots.py --channel='mt,et,tt' --year='2016,2017,2018' --output='chi_plots' --categories='high_m_vis_n_jets_0,med_m_vis_n_jets_0,low_m_vis_n_jets_0'

import os
import argparse
import math as math

parser = argparse.ArgumentParser()
parser.add_argument('--channel',help= 'Name of input channels', default='mt,et,tt')
parser.add_argument('--year', help= 'Name of input years', default='2016,2017,2018')
parser.add_argument('--categories', help= 'Name of input methods', default='inclusive')
parser.add_argument('--method', help= 'Name of input methods', default='17')
parser.add_argument('--output', help= 'Name of output folder to create', default='mssm_control_plots')
parser.add_argument('--add_systs', help= 'Add systematic', action='store_true', default=False)
parser.add_argument('--dry_run', help= 'Only create jobs, does not run them', action='store_true', default=False)
args = parser.parse_args()
      
# Things to loop over
channels = args.channel.split(',')
methods = args.method.split(',')
years = args.year.split(',')
output = args.output
categories = args.categories.split(',')

config_files = {'2016':'scripts/plot_vlq_2016.cfg',
                '2017':'scripts/plot_vlq_2017.cfg',
                '2018':'scripts/plot_vlq_2018.cfg'
               }


add_options = '--embedding --ratio --norm_bins --ratio_range=\'0.3,1.7\''
 
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

var_string = "chi"

bin_dict = {
            "et":{
                  "low_m_vis_n_jets_0":{"2016":"[1,3,5,7,9,11,13,15,17,19,21]","2017":"[1,3,5,7,9,11,13,15,17,19,21]","2018":"[1,3,5,7,9,11,13,15,17,19,21]"},
                  "med_m_vis_n_jets_0":{"2016":"[1,3,5,7,9,11,17,21]","2017":"[1,3,5,7,9,11,13,17,21]","2018":"[1,3,5,7,9,11,13,15,17,19,21]"},
                  "high_m_vis_n_jets_0":{"2016":"[1,3,7,21]","2017":"[1,5,9,21]","2018":"[1,3,7,11,21]"},
                 },
            "mt":{
                  "low_m_vis_n_jets_0":{"2016":"[1,3,5,7,9,11,13,15,17,19,21]","2017":"[1,3,5,7,9,11,13,15,17,19,21]","2018":"[1,3,5,7,9,11,13,15,17,19,21]"},
                  "med_m_vis_n_jets_0":{"2016":"[1,3,5,7,9,11,13,17,21]","2017":"[1,3,5,7,9,11,13,15,17,21]","2018":"[1,3,5,7,9,11,13,15,17,21]"},
                  "high_m_vis_n_jets_0":{"2016":"[1,3,21]","2017":"[1,3,7,21]","2018":"[1,3,11,21]"},
                 },
            "tt":
                 {
                  "low_m_vis_n_jets_0":{"2016":"[1,3,5,7,9,11,13,15,17,19,21]","2017":"[1,3,5,7,9,11,13,15,17,19,21]","2018":"[1,3,5,7,9,11,13,15,17,19,21]"},
                  "med_m_vis_n_jets_0":{"2016":"[1,3,5,7,9,11,13,15,17,19,21]","2017":"[1,3,5,7,9,11,13,15,17,19,21]","2018":"[1,3,5,7,9,11,13,15,17,19,21]"},
                  "high_m_vis_n_jets_0":{"2016":"[1,3,5,7,11,21]","2017":"[1,3,5,9,13,21]","2018":"[1,3,5,7,9,13,21]"},
                 }
           }


# Job loop

for year in years:
  for channel in channels:
    for method in methods:
      for cat in categories:

        var = var_string+bin_dict[channel][cat][year]
      
        cfg = config_files[year]
        run_cmd = 'python %(cmssw_base)s/scripts/HiggsTauTauPlot.py --cfg=\'%(cfg)s\' --channel=%(channel)s --method=%(method)s --var=\'%(var)s\' %(add_options)s --folder=/vols/cms/gu18/Offline/output/MSSM/vlq_%(year)s_bkg_data --embed_folder=\'\' --signal_folder=\'\' --log_y --cat=%(cat)s' % vars()
        
        run_list = []
        if channel in ["et","mt"]: 
           run_list.append(run_cmd)
        elif channel in ["tt"]: 
           run_list.append(run_cmd)
        
        for cmd in run_list:
         
          add_name = ''
          output_folder = '%(cmssw_base)s/%(output)s/%(channel)s/%(year)s' % vars()
          if channel in ["mt"]:
            if args.add_systs: 
              add_syst = '--do_custom_uncerts --add_stat_to_syst --syst_tau_trg_diff="trg_syst_*" --syst_tau_id_diff="id_syst_*"  --do_ff_systs --syst_tquark="syst_ttbar_pt"  --syst_embedding_tt="syst_embed_tt"  --syst_tau_scale_grouped="CMS_scale_t_*group_%(year)s" --syst_res_j="CMS_res_j_%(year)s" --syst_scale_met_unclustered="CMS_scale_met_unclustered_%(year)s" --syst_scale_met="CMS_htt_boson_scale_met_%(year)s" --syst_res_met="CMS_htt_boson_reso_met_%(year)s" --syst_scale_j_regrouped="CMS_scale_j_*group" --syst_mufake_0pi_scale="CMS_ZLShape_mt_1prong_%(year)s" --syst_mufake_1pi_scale="CMS_ZLShape_mt_1prong1pizero_%(year)s" --syst_lep_trg_diff="CMS_eff_*_%(year)s"' % vars()
            else:
              add_syst = ''
            cmd += " --sel=\'mt_1<70\'"
          elif channel in ["et"]:
            if args.add_systs: 
              add_syst = '--do_custom_uncerts --add_stat_to_syst --syst_tau_trg_diff="trg_syst_*" --syst_tau_id_diff="id_syst_*"  --do_ff_systs --syst_tquark="syst_ttbar_pt"  --syst_embedding_tt="syst_embed_tt"  --syst_tau_scale_grouped="CMS_scale_t_*group_%(year)s" --syst_res_j="CMS_res_j_%(year)s" --syst_scale_met_unclustered="CMS_scale_met_unclustered_%(year)s" --syst_scale_met="CMS_htt_boson_scale_met_%(year)s" --syst_res_met="CMS_htt_boson_reso_met_%(year)s" --syst_scale_j_regrouped="CMS_scale_j_*group" --syst_efake_0pi_scale="CMS_ZLShape_et_1prong_%(year)s" --syst_efake_1pi_scale="CMS_ZLShape_et_1prong1pizero_%(year)s" --syst_e_scale="CMS_scale_e" --syst_lep_trg_diff="CMS_eff_*_%(year)s"' % vars()
            else:
              add_syst = ''
            cmd += " --sel=\'mt_1<70\'"
  
          elif channel in ["tt"]:
            if args.add_systs:
              add_syst = '--do_custom_uncerts --add_stat_to_syst --syst_tau_trg_diff="trg_syst_*" --syst_tau_id_diff="id_syst_*"  --do_ff_systs --syst_tquark="syst_ttbar_pt"  --syst_embedding_tt="syst_embed_tt"  --syst_tau_scale_grouped="CMS_scale_t_*group_%(year)s" --syst_res_j="CMS_res_j_%(year)s" --syst_scale_met_unclustered="CMS_scale_met_unclustered_%(year)s" --syst_scale_met="CMS_htt_boson_scale_met_%(year)s" --syst_res_met="CMS_htt_boson_reso_met_%(year)s" --syst_scale_j_regrouped="CMS_scale_j_*group"' % vars()
            else:
              add_syst = ''
  
          if year == "2016":
            wt = "--add_wt=\'wt_tau_trg_mssm*wt_tau_id_mssm*wt_prefire\'"
            if args.add_systs:
              add_syst += ' --syst_zwt="CMS_htt_dyShape_2016" --syst_prefire="CMS_prefiring"'
          elif year == "2017":
            wt = "--add_wt=\'wt_tau_trg_mssm*wt_tau_id_mssm*wt_prefire\'"
            if args.add_systs:
              add_syst += ' --syst_prefire="CMS_prefiring" --syst_zwt="CMS_htt_dyShape"'
          elif year == "2018":
            wt = "--add_wt=\'wt_tau_trg_mssm*wt_tau_id_mssm\'"
            if args.add_systs:
              add_syst += ' --syst_zwt="CMS_htt_dyShape"'
   
  
          extra_name = '%(var_string)s%(add_name)s_%(cat)s' % vars()
          cmd += ' --extra_name=\'%(extra_name)s\' --outputfolder=%(output_folder)s %(add_syst)s %(wt)s' % vars()
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
  
          if not args.dry_run:
            if not args.add_systs or not ((year in ["2017","2018"]) and (channel in ['mt','et'])):
              os.system('qsub -e %(error_file)s -o %(output_file)s -V -q hep.q -l h_rt=0:180:0 -cwd %(job_file)s' % vars())
            else:
              os.system('qsub -e %(error_file)s -o %(output_file)s -V -q hep.q -l h_rt=10:0:0 -cwd %(job_file)s' % vars())

           
    


