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
                     'mjj[0,50,100,150,200,250,300,350,400,450,500,550,600,700,750,800,850,900,950,1000]',
                    'jeta_1[-5.,-4.5,-4.,-3.5,-3.,-2.5,-2.,-1.5,-1.,-0.5,0.,0.5, 1.,1.5,2.,2.5,3.,3.5,4.,4.5,5.]',
#                  #  'pt_1/(pt_1+pt_2+met+jpt_1)[0.,0.1,0.2,0.3,0.4,0.5,0.6,0.7,0.8,0.9,1.0]',
#                  #  'pt_2/(pt_1+pt_2+met+jpt_1)[0.,0.1,0.2,0.3,0.4,0.5,0.6,0.7,0.8,0.9,1.0]',
#                  #  'met/(pt_1+pt_2+met+jpt_1)[0.,0.1,0.2,0.3,0.4,0.5,0.6,0.7,0.8,0.9,1.0]',
#                  #  'jpt_1/(pt_1+pt_2+met+jpt_1)[0.,0.1,0.2,0.3,0.4,0.5,0.6,0.7,0.8,0.9,1.0]',
##                 #   #GetBinning('mt_tot',20,3200,100,log=True,round=1),
##                 #   'mt_tot[0,50,60,70,80,90,100,110,120,130,140,150,160,170,180,190,200,225,250,275,300,325,350,400,450,500,600,700,800,900,1100,1300,1500,5000',
##                 #   #GetBinning('pt_1+pt_2+met+jpt_1',20,3200,100,log=True,round=1),
#                    'pt_1+pt_2+met+jpt_1[100,150,200,250,300,350,400,450,500,550,600,650,700,750,800,850,900,950,1000,1100,1200,1300,1400,1500,3000]',
##                 #  # #GetBinning('m_vis',20,3200,100,log=True,round=1),
##                 #  # ##GetBinning('svfit_mass',20,3200,50,log=True,round=1,final_bin_start=1000),
##                 #  # #GetBinning('met',10,800,100,log=True,round=1),
##                 #  # ##'n_jets[0,1,>=2]',
##                 #  # ##'n_deepbjets[0,>=1]',
##                 #  # #'n_jets[0,1,2,3,4,5]',
##                 #  # #'n_deepbjets[0,1,2,3]',
##                 #  # #GetBinning('mt_1',0,500,100,log=False,round=1),
##                 #  # #GetBinning('mt_2',0,500,100,log=False,round=1),
##                 #  # #GetBinning('mt_lep',0,500,100,log=False,round=1),
##                 #  # ##GetBinning('eta_1',-2.2,2.2,100,log=False,round=0.05),
##                 #  # ##GetBinning('eta_2',-2.2,2.2,100,log=False,round=0.05),
##                 #  # #GetBinning('met_dphi_1',0,3.2,100,log=False,round=0.05),
##                 #  # #GetBinning('met_dphi_2',0,3.2,100,log=False,round=0.05),
##                 #  # #GetBinning('dphi',0,3.2,100,log=False,round=0.05),
##                 #  # #GetBinning('jpt_1',30,600,100,log=True,round=1),
##                 #  # #GetBinning('jpt_2',30,600,100,log=True,round=1),
##                 #  # #GetBinning('pt_vis',30,600,100,log=True,round=1),
##                 #  # #GetBinning('pt_tt',30,600,100,log=True,round=1),
##                 #  # #GetBinning('mjj',0,500,100,log=False,round=1),
##                 #  # #GetBinning('dijetpt',30,600,100,log=True,round=1),
##                 #  # #GetBinning('jdeta',0,6.0,100,log=False,round=0.05),
                    'fabs(dphi)[0.0,0.31416,0.62832,0.94248,1.25664,1.5708,1.88496,2.19912,2.51328,2.82744,3.1416]',
##                 #   'n_deepbjets[0,1,2,3,4,5]',
                    'n_jets[0,1,2,3,4,5,6,7]',
                    'jpt_1[10,70,130,190,250,310,370,430,490,550,610,670,730,790,850,910]', 
                    'jpt_2[10,70,130,190,250,310,370,430,490,550,610,670,730,790,850,910]', 
                    'met[0,60,120,180,240,300,360,420,480,540,600,660,780,900,1400]', 
                    'pt_1[0,60,120,180,240,300,360,420,480,540,600,900,1400]', 
                    'pt_2[0,60,120,180,240,300,360,420,480,540,600,900,1400]',
#                    'ip_sig_1[0,0.5,1.0,1.5,2.0,3,4,5,10]' ,
##                 #   #'ip_sig_2[0,0.5,1.0,1.5,2.0,3,4,5,10]' ,
#                    'ip_mag_1[0.001,0.002,0.003,0.004,0.005,0.006,0.008,0.010,0.014,0.020]',
##                 #   #'ip_mag_2[0.001,0.002,0.003,0.004,0.005,0.006,0.008,0.010,0.014,0.020]',
                    ]

ch_dep_var = {"mt":[
#                  #  #GetBinning('pt_1',20,800,100,log=True,round=1),
#                  #  #GetBinning('pt_2',30,800,100,log=True,round=1),
#                  #  #GetBinning('iso_1',0,0.15,100,log=False,round=0.001),
#                  #  #'tau_decay_mode_2[==0,==1,==10,==11]',
#                  #  #'tau_decay_mode_2[0,1,10,11]',
                    'ip_sig_1[0,0.5,1.0,1.5,2.0,3,4,5,10]' ,
                    'ip_mag_1[0.001,0.002,0.003,0.004,0.005,0.006,0.008,0.010,0.014,0.020]',
                    'mt_tot[0.0,50.0,60.0,70.0,80.0,90.0,100.0,110.0,120.0,130.0,140.0,150.0,160.0,170.0,180.0,190.0,200.0,225.0,250.0,275.0,300.0,325.0,350.0,400.0,450.0,500.0,600.0,700.0,800.0,900.0,1100.0,5000.0]',
                    'pt_1+pt_2+met+jpt_1[0.0,130.0,140.0,150.0,160.0,170.0,180.0,190.0,200.0,225.0,250.0,275.0,300.0,325.0,350.0,400.0,450.0,500.0,600.0,700.0,800.0,900.0,1100.0,1300.0,1500.0,1700.0,5000.0]',
                    ],
              "et":[
#                  #  #GetBinning('pt_1',20,800,40,log=True,round=1),
#                  #  #GetBinning('pt_2',30,800,40,log=True,round=1),
#                  #  #GetBinning('iso_1',0,0.15,40,log=False,round=0.001),
#                  #  #'tau_decay_mode_2[==0,==1,==10,==11]',
#                  #  #'tau_decay_mode_2[0,1,10,11]',
                    'ip_sig_1[0,0.5,1.0,1.5,2.0,3,4,5,10]' ,
                    'ip_mag_1[0.001,0.002,0.003,0.004,0.005,0.006,0.008,0.010,0.014,0.020]',
                    'mt_tot[0.0,50.0,60.0,70.0,80.0,90.0,100.0,110.0,120.0,130.0,140.0,150.0,160.0,170.0,180.0,190.0,200.0,225.0,250.0,275.0,300.0,325.0,350.0,400.0,450.0,500.0,600.0,700.0,800.0,900.0,1100.0,5000.0]',
                    'pt_1+pt_2+met+jpt_1[0.0,130.0,140.0,150.0,160.0,170.0,180.0,190.0,200.0,225.0,250.0,275.0,300.0,325.0,350.0,400.0,450.0,500.0,600.0,700.0,800.0,900.0,1100.0,1300.0,1500.0,1700.0,5000.0]',
                    ],
              "tt":[
#                  #  #GetBinning('pt_1',40,800,40,log=True,round=1),
#                  #  #GetBinning('pt_2',40,800,40,log=True,round=1),
#                  #  #'tau_decay_mode_1[==0,==1,==10,==11]',
#                  #  #'tau_decay_mode_2[==0,==1,==10,==11]',
#                  #  #'tau_decay_mode_1[0,1,10,11]',
#                  #  #'tau_decay_mode_2[0,1,10,11]',
                    'mt_tot[0.0,50.0,60.0,70.0,80.0,90.0,100.0,110.0,120.0,130.0,140.0,150.0,160.0,170.0,180.0,190.0,200.0,225.0,250.0,275.0,300.0,325.0,350.0,400.0,450.0,500.0,600.0,700.0,800.0,900.0,1100.0,1300.0,5000.0]',
                    'pt_1+pt_2+met+jpt_1[0.0,140.0,150.0,160.0,170.0,180.0,190.0,200.0,225.0,250.0,275.0,300.0,325.0,350.0,400.0,450.0,500.0,600.0,700.0,800.0,900.0,1100.0,1300.0,1700.0,5000.0]',
                    ],
               "em":[
#                    GetBinning('pt_1',20,600,40,log=True,round=1),
#                    GetBinning('pt_2',30,600,40,log=True,round=1),
#                    GetBinning('iso_1',0,0.15,40,log=False,round=0.001),
#                    GetBinning('iso_2',0,0.15,40,log=False,round=0.001),
                    ]
              }


config_files = {'2016':'scripts/plot_vlq_2016.cfg',
                '2017':'scripts/plot_vlq_2017.cfg',
                '2018':'scripts/plot_vlq_2018.cfg'
               }

categories = {
              "mt":[
                  #  #"Nbtag0_MTLt40",
                  #  #"Nbtag0_MT40To70",
                  #  #"NbtagGt1_MTLt40",
                  #  #"NbtagGt1_MT40To70",
                  #  "Nbtag0_MTLt70",
                  #  "NbtagGt1_MTLt70",
                    "MTLt70",
                  #  "inclusive",
                    "Nbtag0_NjetsGt1_MTLt70",
                    "Nbtag0_Njets0_MTLt70",
                    "Nbtag0_NjetsGt1_MTLt70_STMETGt800",
                    "Nbtag0_NjetsGt1_MTLt70_STMETGt1000",
                  #  "Nbtag0_NjetsGt1_MTLt70_STMET400to800",
                    "Nbtag0_NjetsGt1",
                    "Nbtag0_Njets0",
                    "Nbtag0_NjetsGt1_STMETGt800",
                    "Nbtag0_NjetsGt1_STMETGt1000",
                    "Nbtag0_NjetsGt1_STPFMETGt800",
                    "NLooseCSVbtag0_NjetsGt1_STMETGt800",
                    "NLooseCSVbtag0_NjetsGt1_STPFMETGt800",
                  #  "Nbtag0_NjetsGt1_STMET400to800",
                    ],
              "et":[                    
                    #"Nbtag0_MTLt40",
                    #"Nbtag0_MT40To70",
                    #"NbtagGt1_MTLt40",
                    #"NbtagGt1_MT40To70",
                    #"Nbtag0_MTLt70",
                    #"NbtagGt1_MTLt70",
                    "MTLt70",
                    #"inclusive",
                    "Nbtag0_NjetsGt1_MTLt70",
                    "Nbtag0_Njets0_MTLt70",
                    "Nbtag0_NjetsGt1_MTLt70_STMETGt800",
                    "Nbtag0_NjetsGt1_MTLt70_STMETGt1000",
                    #"Nbtag0_NjetsGt1_MTLt70_STMET400to800",
                    "Nbtag0_NjetsGt1",
                    "Nbtag0_Njets0",
                    "Nbtag0_NjetsGt1_STMETGt800",
                    "Nbtag0_NjetsGt1_STMETGt1000",
                    "Nbtag0_NjetsGt1_STPFMETGt800",
                    #"Nbtag0_NjetsGt1_STMET400to800",
                    "NLooseCSVbtag0_NjetsGt1_STMETGt800",
                    "NLooseCSVbtag0_NjetsGt1_STPFMETGt800",
                    ],
              "tt":[
                    "inclusive",
                    #"Nbtag0",
                    #"NbtagGt1",
                    "Nbtag0_NjetsGt1",
                    "Nbtag0_Njets0",
                    "Nbtag0_NjetsGt1_STMETGt800",
                    "Nbtag0_NjetsGt1_STPFMETGt800",
                    "Nbtag0_NjetsGt1_STMETGt1000",
                    "NLooseCSVbtag0_NjetsGt1_STMETGt800",
                    "NLooseCSVbtag0_NjetsGt1_STPFMETGt800",
                    #"Nbtag0_NjetsGt1_STMET400to800",
                    ],
              "em":[
                    #"NbtagGt1_DZetaLtm35",
                    #"Nbtag0_DZetaGt30",
                    #"Nbtag0_DZetam10To30",
                    #"Nbtag0_DZetam35Tom10",
                    #"NbtagGt1_DZetaGt30",
                    #"NbtagGt1_DZetam10To30",
                    #"NbtagGt1_DZetam35Tom10",
                    #"Nbtag0_DZetaGtm35",
                    #"NbtagGt1_DZetaGtm35",
                    #"DZetaGtm35",
                    ]
              }

methods = {
           "mt":17,
           "et":17,
           "tt":17,
           "em":19,
           }


#add_options = '--ratio_range=\'auto\' --gU=3.9  --plot_signals=\'VLQ_betaRd33_0_matched_M_2000,VLQ_betaRd33_0_offdiag0_matched_M_2000\' --auto_rebinning --bin_uncert_frac=0.2'
add_options = '--ratio_range=\'auto\' --gU=3.9  --plot_signals=\'VLQ_betaRd33_0_matched_M_2000,VLQ_betaRd33_0_offdiag0_matched_M_2000\' '
 
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
        run_cmd = 'python %(cmssw_base)s/src/UserCode/ICHiggsTauTau/Analysis/HiggsTauTauRun2/scripts/HiggsTauTauPlot.py --cfg=\'%(cfg)s\' --channel=%(channel)s --method=%(method)s --var=\'%(var)s\' --cat=%(cat)s %(add_options)s --extra_pad 0.5 ' % vars()
        print run_cmd
        #run_list = []
        run_list = [run_cmd]
        if channel in ["mt","et"]:
          run_list.append(run_cmd + " --w_ff_closure")
        if channel in ["mt","et","tt"]:
          run_list.append(run_cmd + " --qcd_ff_closure")


        for i,cmd in enumerate(run_list):

          if var_string in ["mt_tot","m_vis","pt_1+pt_2+met+jpt_1","met"]: cmd += " --log_y --log_x"

          add_syst = ''
          output_folder = '%(cmssw_base)s/src/UserCode/ICHiggsTauTau/Analysis/HiggsTauTauRun2/%(output)s/%(channel)s/%(year)s' % vars()
          if args.add_systs:

            common_shape_systematics = [
                ' --syst_tau_id_diff="CMS_eff_t_*_%(year)s"' % vars(), # Tau ID efficiency
                ' --syst_tau_trg_diff="CMS_eff_*_%(year)s"' % vars(), # Tau Trigger efficiency
                ' --syst_tquark="CMS_htt_ttbarShape"', # Top pT re-weighting
                ' --syst_embedding_tt="CMS_htt_emb_ttbar_%(year)s"' % vars(), # ttbar contamination in embedding
                ' --syst_tau_scale_grouped="CMS_scale_t_*group_%(year)s"' % vars(), # Tau energy scale
                ' --syst_res_j="CMS_res_j_%(year)s"' % vars(), # Jet energy resolution
                ' --syst_scale_met_unclustered="CMS_scale_met_unclustered_%(year)s"' % vars(), # MET unclustered energy uncertainty
                ' --syst_scale_met="CMS_htt_boson_scale_met_%(year)s"' % vars(), # MET recoil correction uncertainty
                ' --syst_res_met="CMS_htt_boson_res_met_%(year)s"' % vars(),  # MET recoil correction uncertainty
                ' --syst_scale_j_regrouped="CMS_scale_j_*group"', # Jet energy scale (grouped)
                ' --do_ff_systs',
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
              ' --syst_efake_0pi_scale="CMS_ZLShape_et_1prong_%(year)s"' % vars(), # l to tau h fake energy scale
              ' --syst_efake_1pi_scale="CMS_ZLShape_et_1prong1pizero_%(year)s"' % vars(), # l to tau h fake energy scale
              ' --syst_e_scale="CMS_scale_e"', # Election energy scale
            ]

            mt_shape_systematics = [
              ' --syst_lep_trg_diff="CMS_eff_*_%(year)s"' % vars(), # Lepton trigger efficiency 
              ' --syst_mufake_0pi_scale="CMS_ZLShape_mt_1prong_%(year)s"' % vars(), # l to tau h fake energy scale
              ' --syst_mufake_1pi_scale="CMS_ZLShape_mt_1prong1pizero_%(year)s"' % vars(), # l to tau h fake energy scale
            ]
            add_syst=''
            for syst in common_shape_systematics: add_syst+=' %s ' % syst

            if channel == "mt":
              for syst in mt_shape_systematics: add_syst+=' %s ' % syst

            if channel == "et":
              for syst in et_shape_systematics: add_syst+=' %s ' % syst

            print add_syst
           

           # if channel == "mt":
           #   add_syst = '--do_custom_uncerts --add_stat_to_syst --syst_tau_trg_diff="trg_syst_*" --syst_tau_id_diff="id_syst_*"  --do_ff_systs --syst_tquark="syst_ttbar_pt"  --syst_embedding_tt="syst_embed_tt"  --syst_tau_scale_grouped="CMS_scale_t_*group_%(year)s" --syst_res_j="CMS_res_j_%(year)s" --syst_scale_met_unclustered="CMS_scale_met_unclustered_%(year)s" --syst_scale_met="CMS_htt_boson_scale_met_%(year)s" --syst_res_met="CMS_htt_boson_reso_met_%(year)s" --syst_scale_j_regrouped="CMS_scale_j_*group" --syst_mufake_0pi_scale="CMS_ZLShape_mt_1prong_%(year)s" --syst_mufake_1pi_scale="CMS_ZLShape_mt_1prong1pizero_%(year)s" --syst_lep_trg_diff="CMS_eff_*_%(year)s"' % vars()
           # elif channel in ["et"]:
           #   add_syst = '--do_custom_uncerts --add_stat_to_syst --syst_tau_trg_diff="trg_syst_*" --syst_tau_id_diff="id_syst_*"  --do_ff_systs --syst_tquark="syst_ttbar_pt"  --syst_embedding_tt="syst_embed_tt"  --syst_tau_scale_grouped="CMS_scale_t_*group_%(year)s" --syst_res_j="CMS_res_j_%(year)s" --syst_scale_met_unclustered="CMS_scale_met_unclustered_%(year)s" --syst_scale_met="CMS_htt_boson_scale_met_%(year)s" --syst_res_met="CMS_htt_boson_reso_met_%(year)s" --syst_scale_j_regrouped="CMS_scale_j_*group" --syst_efake_0pi_scale="CMS_ZLShape_et_1prong_%(year)s" --syst_efake_1pi_scale="CMS_ZLShape_et_1prong1pizero_%(year)s" --syst_e_scale="CMS_scale_e" --syst_lep_trg_diff="CMS_eff_*_%(year)s"' % vars()
           # elif channel in ["tt"]:
           #   add_syst = '--do_custom_uncerts --add_stat_to_syst --syst_tau_trg_diff="trg_syst_*" --syst_tau_id_diff="id_syst_*"  --do_ff_systs --syst_tquark="syst_ttbar_pt"  --syst_embedding_tt="syst_embed_tt"  --syst_tau_scale_grouped="CMS_scale_t_*group_%(year)s" --syst_res_j="CMS_res_j_%(year)s" --syst_scale_met_unclustered="CMS_scale_met_unclustered_%(year)s" --syst_scale_met="CMS_htt_boson_scale_met_%(year)s" --syst_res_met="CMS_htt_boson_reso_met_%(year)s" --syst_scale_j_regrouped="CMS_scale_j_*group"' % vars()
           # if year == "2016":
           #   add_syst += ' --syst_zwt="CMS_htt_dyShape_2016" --syst_prefire="CMS_prefiring"'
           # elif year == "2017":
           #   add_syst += ' --syst_prefire="CMS_prefiring" --syst_zwt="CMS_htt_dyShape"'
           # elif year == "2018":
           #   add_syst += ' --syst_zwt="CMS_htt_dyShape"'
  
          if "MT" in cat and "w_ff_closure" in cmd: continue 
          
          extra_name = '%(var_string)s' % vars()
          extra_name = extra_name.replace('(','')
          extra_name = extra_name.replace(')','').replace('/','_over_')
          if "w_ff_closure" in cmd: extra_name += "_w_ff_closure"
          if "qcd_ff_closure" in cmd: extra_name += "_qcd_ff_closure"

          cmd += ' --extra_name=\'%(extra_name)s\' --outputfolder=%(output_folder)s %(add_syst)s' % vars()
          if 'NLooseCSVbtag' in cat: cmd +=' --add_wt=\'wt_tau_trg_mssm*wt_tau_id_mssm*wt_prefire*wt_btag\''
          #if var_string in ["mt_tot","m_vis","pt_1","pt_2","met","svfit_mass","jpt_1","jpt_2","pt_vis","pt_tt","dijetpt"]: cmd += ' --log_x'
          if var_string in ["mt_tot","m_vis","pt_1","pt_2","met","svfit_mass","pt_vis","pt_tt","dijetpt"]: cmd += ' --log_x'
          job_file = '%(cmssw_base)s/src/UserCode/ICHiggsTauTau/Analysis/HiggsTauTauRun2/%(output)s/jobs/vlq_plot_%(cat)s_%(year)s_%(channel)s_%(extra_name)s.sh' % vars()
          print job_file
          CreateBatchJob(job_file,cmssw_base,[cmd])
          if not args.dry_run: SubmitBatchJob(job_file,time=180,memory=24,cores=1)

           
    


