#!/usr/bin/env python

import os

      
# Things to loop over
channels = ['et', 'mt','tt']
methods = ['17']
years = ['2016','2017']
variables = ['mt_tot[20,30,40,50,60,70,80,90,100,110,125,140,160,175,200,225,250,280,320,350,400,450,500,560,630,710,800,890,1000]',
             'm_vis[20,30,40,50,60,70,80,90,100,110,125,140,160,175,200,225,250,280,320,350,400,450,500,560,630,710,800,890,1000]',
             'met[0,10,20,30,40,50,60,70,80,90,100,120,140,200,400]',
             'n_jets[0,1,>=2]',
             'n_prebjets[0,1,>=2]', 
             'n_deepbjets[0,>=1]',
             'pt_1[20,25,30,35,40,45,50,55,60,65,70,80,90,100,120,140,200,400]',
             'pt_2[20,25,30,35,40,45,50,55,60,65,70,80,90,100,120,140,200,400]',
             'mt_1[0,20,40,60,80,100,120,140,160,180,200,220,240,260,280,300]',
             'mt_2[0,20,40,60,80,100,120,140,160,180,200,220,240,260,280,300]',
             'mt_lep[0,20,40,60,80,100,120,140,160,180,200,220,240,260,280,300]',
             'iso_1[0.0,0.005,0.01,0.015,0.02,0.025,0.03,0.035,0.04,0.045,0.05,0.055,0.06,0.065,0.07,0.075,0.08,0.085,0.09,0.095,0.1,0.105,0.11,0.115,0.12,0.125,0.13,0.135,0.14,0.145,0.15]',
             'eta_1[-3,-2.8,-2.6,-2.4,-2.2,-2.0,-1.8,-1.6,-1.4,-1.2,-1.0,-0.8,-0.6,-0.4,-0.2,0.0,0.2,0.4,0.6,0.8,1.0,1.2,1.4,1.6,1.8,2.0,2.2,2.4,2.6,2.8,3.0]',
             'eta_2[-3,-2.8,-2.6,-2.4,-2.2,-2.0,-1.8,-1.6,-1.4,-1.2,-1.0,-0.8,-0.6,-0.4,-0.2,0.0,0.2,0.4,0.6,0.8,1.0,1.2,1.4,1.6,1.8,2.0,2.2,2.4,2.6,2.8,3.0]',
             'met_dphi_1(20,0,3.1415927)',
             'met_dphi_2(20,0,3.1415927)',
             'dphi(20,0,3.1415927)'
            ]


config_files = {'2016':'scripts/plot_mssm_2016.cfg',
                '2017':'scripts/plot_mssm_2017.cfg',
                '2018':'scripts/plot_mssm_2018.cfg'
               }


add_options = '--embedding --add_wt=\'wt_tau_trg_mssm*wt_tau_id_mssm*wt_prefire\' --ggh_masses=\'\' --bbh_nlo_masses=\'\' --ratio --norm_bins --ratio_range=\'0.8,1.2\''
 
cmssw_base = os.getcwd()


for year in years:
  for channel in channels:
    for method in methods:
      for var in variables:
        if '[' in var: var_string = var.split('[')[0]
        elif '(' in var: var_string = var.split('(')[0]
   
        cfg = config_files[year]
        run_cmd = 'python %(cmssw_base)s/scripts/HiggsTauTauPlot.py --cfg=\'%(cfg)s\' --channel=%(channel)s --method=%(method)s --var=\'%(var)s\' %(add_options)s' % vars()
        run_cmd_w_closure = 'python %(cmssw_base)s/scripts/HiggsTauTauPlotW_2.py --cfg=\'%(cfg)s\' --channel=%(channel)s --method=%(method)s --var=\'%(var)s\' %(add_options)s --sel=\'mt_1>70 && n_deepbjets==0\'' % vars()
        if channel in ["et","mt"]:
          run_cmd_qcd_closure = 'python %(cmssw_base)s/scripts/HiggsTauTauPlotQCD_2.py --cfg=\'%(cfg)s\' --channel=%(channel)s --method=%(method)s --var=\'%(var)s\' %(add_options)s --do_ss --sel=\'mt_1<50 && iso_1>0.05\'' % vars()
        elif channel in ["tt"]:
          run_cmd_qcd_closure = 'python %(cmssw_base)s/scripts/HiggsTauTauPlot.py --cfg=\'%(cfg)s\' --channel=%(channel)s --method=%(method)s --var=\'%(var)s\' %(add_options)s --do_ss' % vars()

        if channel in ["et","mt"]: run_list = [run_cmd,run_cmd_w_closure,run_cmd_qcd_closure]
        elif channel in ["tt"]: run_list = [run_cmd,run_cmd_qcd_closure]
        
        for cmd in run_list:
         
          if "W_2" in cmd: 
            add_name = '_w_dr'
            output_folder = 'mssm_control_plots/closure_plots/%(channel)s/%(year)s' % vars()
            add_syst = ''
          elif "do_ss" in cmd: 
            add_name = '_qcd_dr' 
            output_folder = 'mssm_control_plots/closure_plots/%(channel)s/%(year)s' % vars()
            add_syst = ''
          else:
            add_name = ''
            output_folder = 'mssm_control_plots/control_plots/%(channel)s/%(year)s' % vars()
            if channel in ["mt","et"]:
              add_syst = '--do_custom_uncerts --add_stat_to_syst --syst_tau_trg_diff="trg_syst_*" --syst_tau_id_diff="id_syst_*"  --do_ff_systs --syst_tquark="syst_ttbar_pt"  --syst_embedding_tt="syst_embed_tt"'
              cmd += " --sel=\'mt_1<70\'"
            elif channel in ["tt"]:
              add_syst = '--do_custom_uncerts --add_stat_to_syst --syst_tau_trg_diff="trg_syst_*" --syst_tau_id_diff="id_syst_*"  --do_ff_systs --syst_tquark="syst_ttbar_pt"  --syst_embedding_tt="syst_embed_tt"'

          extra_name = '%(var_string)s%(add_name)s' % vars()
          cmd += ' --extra_name=\'%(extra_name)s\' --outputfolder=%(output_folder)s %(add_syst)s' % vars()
          if var_string in ["mt_tot","m_vis","pt_1","pt_2"]: cmd += ' --log_x'
          job_file = 'jobs/mssm_control_plot_%(year)s_%(channel)s_%(extra_name)s.sh' % vars()
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

          #os.system('qsub -e %(error_file)s -o %(output_file)s -V -q hep.q -l h_rt=0:180:0 -cwd %(job_file)s' % vars())
           
    


