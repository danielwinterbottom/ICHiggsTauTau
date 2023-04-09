#!/usr/bin/env python

# importing packages
import sys
from optparse import OptionParser
import os
import numpy as np
import json

config_files = {'2016_preVFP':'scripts/plot_UL_2016_preVFP.cfg',
		        '2016_postVFP':'scripts/plot_UL_2016_postVFP.cfg',
		        '2016preUL':'scripts/plot_mssm_2016.cfg',
                '2017preUL':'scripts/plot_mssm_2017.cfg',
                '2017':'scripts/plot_UL_2017.cfg',
                '2018':'scripts/plot_UL_2018.cfg',
                '2018preUL':'scripts/plot_mssm_2018.cfg'}
               
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

def BINS(start,end,interval,offset):
  bins = np.arange(start,end+offset,interval)
  bins_string = ','.join(str(x) for x in bins)
  bins  = []
  bins.append(bins_string)
  return bins


parser = OptionParser()
parser.add_option('--channel',help= 'Name of input channels', default='mt,tt,et')
parser.add_option("--years", dest="years", type='string', default='2016_preVFP,2016_postVFP,2017,2018',help="Year input")
parser.add_option('--output_folder', help= 'Name of output folder to create', default='plots')
parser.add_option("--batch", dest="batch", action='store_true', default=False, help="Submit on batch.")
parser.add_option('--wp', help= 'Name of VsJet WP to use', default='medium')
parser.add_option('--sf_option', help= 'Select which SFs you want to apply', default='none')
(options, args) = parser.parse_args()

# initialising variables
output_folder = options.output_folder
channels = options.channel.split(',')
years = options.years.split(',')
wp=options.wp

print 'Processing channels:      %(channels)s' % vars()
print 'Processing years:         %(years)s' % vars()

# cmssw directory
cmssw_base = os.getcwd().replace('src/UserCode/ICHiggsTauTau/Analysis/HiggsTauTauRun2','')

# check whether path is in an exitisting directory
if not os.path.isdir('%(output_folder)s' % vars()):
  os.system("mkdir %(output_folder)s" % vars())
if not os.path.isdir('%(output_folder)s/jobs' % vars()):
  os.system("mkdir %(output_folder)s/jobs" % vars())
  
categories_et = ["inclusive",'dm0','dm1','dm10','dm11']
categories_mt = ["inclusive",'dm0','dm1','dm10','dm11']
categories_tt = ["inclusive"]
categories_zmm = ["inclusive"]
categories_zee = ["inclusive"]
cat_schemes = {'et' : categories_et,
               'mt' : categories_mt,
               'tt' : categories_tt,
	       'zmm': categories_zmm,
	       'zee': categories_zee}

var_tt = [
         ['m_vis', BINS(0,300,10,1)],
         #['n_jets', BINS(0,8,1,1)],
         #['eta_1',BINS(-2.1,2.1,0.1,0.1)],
         #['eta_2',BINS(-2.1,2.1,0.1,0.1)],
         ['tau_decay_mode_2',BINS(0,12,1,1)],
         ['tau_decay_mode_1',BINS(0,12,1,1)],
         ['pt_2', BINS(0,200,5,1)],
         ['pt_1', BINS(0,200,5,1)],
         ['iso_1', BINS(0.8,1,0.01,0.01)],
         ['iso_2', BINS(0.8,1,0.01,0.01)],
         #['met', BINS(0,200,10,1)],
         #['pt_vis', BINS(0,300,10,1)],
         #['pt_tt', BINS(0,300,10,1)],
         #['dR', BINS(0,7,0.03,0.03)],
         #['mt_1', BINS(0,150,5,1)],
         #['met_dphi_1', BINS(0,3.2,0.03,0.03)],
         #['met_dphi_2', BINS(0,3.2,0.03,0.03)],
         ]        
var_mt = [
         ['m_vis', BINS(0,300,10,1)],
         ['iso_2', BINS(0.8,1,0.01,0.01)],
         #['n_jets', BINS(0,8,1,1)],
         #['eta_1',BINS(-2.1,2.1,0.1,0.1)],
         #['eta_2',BINS(-2.1,2.1,0.1,0.1)],
         ['tau_decay_mode_2',BINS(0,12,1,1)],
         ['pt_2', BINS(0,200,5,1)],
         #['pt_1', BINS(0,200,5,1)],
         #['tau_decay_mode_2,pt_2', '[0,1,10,11],[20,25,30,35,40,50,60,80,100,200]'],
         #['met', BINS(0,200,10,1)],
         #['pt_vis', BINS(0,300,10,1)],
         #['pt_tt', BINS(0,300,10,1)],
         #['dR', BINS(0,7,0.03,0.03)],
         ['mt_1', BINS(0,150,5,1)],
         #['met_dphi_1', BINS(0,3.2,0.03,0.03)],
         #['met_dphi_2', BINS(0,3.2,0.03,0.03)],
         ]        
var_et = [
         ['m_vis', BINS(0,300,10,1)],
         #['n_jets', BINS(0,8,1,1)],
         ['iso_2', BINS(0.8,1,0.01,0.01)],
         #['eta_1',BINS(-2.1,2.1,0.1,0.1)],
         #['eta_2',BINS(-2.1,2.1,0.1,0.1)],
         ['tau_decay_mode_2',BINS(0,12,1,1)],
         ['pt_2', BINS(0,200,5,1)],
         #['pt_1', BINS(0,200,5,1)],
         #['tau_decay_mode_2,pt_2', '[0,1,10,11],[20,25,30,35,40,50,60,80,100,200]'],
         #['met', BINS(0,200,10,1)],
         #['pt_vis', BINS(0,300,10,1)],
         #['pt_tt', BINS(0,300,10,1)],
         #['dR', BINS(0,7,0.03,0.03)],
         ['mt_1', BINS(0,150,5,1)],
         ]

var_zmm = [
          ['pt_tt', '[0,10,20,30,40,60,80,100,120,160,200,280,320,400,600]'],
          ['m_vis', BINS(0,300,10,1)],
          ]

var_schemes = {'et' : var_et,
               'mt' : var_mt,
               'tt' : var_tt,
	           'zmm': var_zmm}

for year in years:
  if not os.path.isdir('%(output_folder)s/%(year)s' % vars()):
    os.system("mkdir %(output_folder)s/%(year)s" % vars())
  CFG = config_files[year]
  for ch in channels:
    if not os.path.isdir('%(output_folder)s/%(year)s/%(ch)s' % vars()):
      os.system("mkdir %(output_folder)s/%(year)s/%(ch)s" % vars())
    if ch=='tt': method='8'
    else: method='12'

    sf='1.'

    if 'ptbinned' in options.sf_option or 'dmbinned' in options.sf_option:
      if 'dmbinned' in options.sf_option: json_name = '%(cmssw_base)s/src/UserCode/ICHiggsTauTau/Analysis/HiggsTauTauRun2/input/tau_SF_dm_binned.json' % vars()
      else: json_name= '%(cmssw_base)s/src/UserCode/ICHiggsTauTau/Analysis/HiggsTauTauRun2/input/tau_SF_pt_binned.json' % vars()
      with open(json_name) as json_file:
        sf_data = json.load(json_file) 
      if ch == 'et':
        sf=sf_data['%(wp)svsjet_tightvsele' % vars()][year]
      else:
        sf=sf_data['%(wp)svsjet_vvloosevsele' % vars()][year]


    if 'pt_2' in sf and ch =='tt':
      sf+='*%s' % sf.replace('_2','_1')

    trg_sf='1'

    if ch=='tt' and 'trg' in options.sf_option:
      # to use custom version of the tau trigger SFs
      if year == '2016_preVFP':
        # tau trigger SFs for 2016_preVFP:
         trg_sf='((gen_match_2!=5) + (gen_match_2==5)*((tau_decay_mode_2==0)*(1.01589*TMath::Erf((pt_2-1.84933)/29.3807))+(tau_decay_mode_2==1)*((0.983576+0.00054619*pt_2))+(tau_decay_mode_2==10)*(0.980229*TMath::Erf((pt_2-28.8726)/13.9263))+(tau_decay_mode_2==11)*((1.13688+-0.00146359*pt_2))))/trigweight_2'
      if year == '2016_postVFP':
        # tau trigger SFs for 2016_postVFP:
        trg_sf='((gen_match_2!=5) + (gen_match_2==5)*((tau_decay_mode_2==0)*(0.96416*TMath::Erf((pt_2-11.8269)/24.4132))+(tau_decay_mode_2==1)*(1.00473*TMath::Erf((pt_2--38.7059)/61.3979))+(tau_decay_mode_2==10)*((0.913341+0.000308981*pt_2))+(tau_decay_mode_2==11)*(0.956707*TMath::Erf((pt_2-222.81)/-28.0787))))/trigweight_2' 
      if year == '2017':
        # tau trigger SFs for 2017:
        trg_sf='((gen_match_2!=5) + (gen_match_2==5)*((tau_decay_mode_2==0)*(0.984864*TMath::Erf((pt_2-34.2027)/6.80412))+(tau_decay_mode_2==1)*(0.966496*TMath::Erf((pt_2-22.2322)/11.6171))+(tau_decay_mode_2==10)*(0.99587*TMath::Erf((pt_2-4.56747)/33.2198))+(tau_decay_mode_2==11)*(0.989166*TMath::Erf((pt_2-38.4152)/3.03636))))/trigweight_2' 
      if year == '2018':
        # tau trigger SFs for 2018:
        trg_sf='((gen_match_2!=5) + (gen_match_2==5)*((tau_decay_mode_2==0)*(0.963764*TMath::Erf((pt_2-20.5994)/20.7016))+(tau_decay_mode_2==1)*(0.995965*TMath::Erf((pt_2-13.7808)/25.171))+(tau_decay_mode_2==10)*(1.05842*TMath::Erf((pt_2-19.6424)/24.2147))+(tau_decay_mode_2==11)*(0.998749*TMath::Erf((pt_2-20.4187)/18.4246))))/trigweight_2'

    if 'pt_2' in trg_sf:
      trg_sf+='*%s' % trg_sf.replace('_2','_1')

    if options.sf_option != 'none':
      if ch=='tt': 
        add_cond = '--add_wt=\'wt_prefire*%(sf)s/wt_tau_id_dm*%(trg_sf)s\'' % vars()
      else: add_cond = '--add_wt=\'wt_prefire*%(sf)s/wt_tau_id_pt*%(trg_sf)s\'' % vars()
    else:
      #if you want to apply the old corrections
      if ch=='tt':
        add_cond = '--add_wt=\'wt_prefire*%(sf)s\'' % vars()
      else: add_cond = '--add_wt=\'wt_prefire*%(sf)s\'' % vars()

    add_cond+=' --wp=%(wp)s ' % vars()
 
    if year == "2016_preVFP" or year == "2016_postVFP":
      pt1_cut = 23
    else:
      pt1_cut = 25   

    categories = cat_schemes[ch]
    variables = var_schemes[ch]
    for cat in categories:
      for item in variables:
           var_used = item[0]
           bin_used = item[1]
           run_cmd_3=''
           if ch == 'tt': 
             run_cmd = 'python %(cmssw_base)s/src/UserCode/ICHiggsTauTau/Analysis/HiggsTauTauRun2/scripts/HiggsTauTauPlot.py --cfg=%(CFG)s --channel=%(ch)s --sel=\'(m_vis>40&&m_vis<80)\' --method=%(method)s --cat=%(cat)s --outputfolder=%(output_folder)s/%(year)s/%(ch)s --ggh_masses_powheg='' --bbh_masses_powheg='' --var=\"%(var_used)s%(bin_used)s\" %(add_cond)s --ratio_range="0.5,1.5"' % vars()
           elif ch == 'mt':
             run_cmd = 'python %(cmssw_base)s/src/UserCode/ICHiggsTauTau/Analysis/HiggsTauTauRun2/scripts/HiggsTauTauPlot.py --cfg=%(CFG)s --channel=%(ch)s --sel=\'(mt_1<40)\' --set_alias=\'%(cat)s:({%(cat)s}&&m_vis>40&&m_vis<80&&pt_1>25&&trg_singlemuon&&fabs(eta_1)<2.1)\' --method=%(method)s --cat=%(cat)s --outputfolder=%(output_folder)s/%(year)s/%(ch)s --ggh_masses_powheg='' --bbh_masses_powheg='' --var=\"%(var_used)s%(bin_used)s\" %(add_cond)s --ratio_range="0.5,1.5" --datacard="%(cat)s_mTLt40" ' % vars()
             run_cmd_2 = 'python %(cmssw_base)s/src/UserCode/ICHiggsTauTau/Analysis/HiggsTauTauRun2/scripts/HiggsTauTauPlot.py --cfg=%(CFG)s --channel=%(ch)s --sel=\'(mt_1<65)\' --set_alias=\'%(cat)s:({%(cat)s}&&m_vis>40&&m_vis<80&&pt_1>25&&trg_singlemuon&&fabs(eta_1)<2.1)\' --method=%(method)s --cat=%(cat)s --outputfolder=%(output_folder)s/%(year)s/%(ch)s --ggh_masses_powheg='' --bbh_masses_powheg='' --var=\"%(var_used)s%(bin_used)s\" %(add_cond)s --ratio_range="0.5,1.5" --datacard="%(cat)s_mTLt65" ' % vars()
           elif ch == 'et': 
             run_cmd = 'python %(cmssw_base)s/src/UserCode/ICHiggsTauTau/Analysis/HiggsTauTauRun2/scripts/HiggsTauTauPlot.py --cfg=%(CFG)s --channel=%(ch)s --sel=\'(mt_1<40)\' --set_alias=\'%(cat)s:({%(cat)s}&&m_vis>40&&m_vis<80&&pt_1>25&&trg_singleelectron)\' --method=%(method)s --cat=%(cat)s --outputfolder=%(output_folder)s/%(year)s/%(ch)s --ggh_masses_powheg='' --bbh_masses_powheg='' --var=\"%(var_used)s%(bin_used)s\" %(add_cond)s --ratio_range="0.5,1.5" --datacard="%(cat)s_mTLt40" ' % vars()
             run_cmd_2 = 'python %(cmssw_base)s/src/UserCode/ICHiggsTauTau/Analysis/HiggsTauTauRun2/scripts/HiggsTauTauPlot.py --cfg=%(CFG)s --channel=%(ch)s --sel=\'(mt_1<65)\' --set_alias=\'%(cat)s:({%(cat)s}&&m_vis>40&&m_vis<80&&pt_1>25&&trg_singleelectron)\' --method=%(method)s --cat=%(cat)s --outputfolder=%(output_folder)s/%(year)s/%(ch)s --ggh_masses_powheg='' --bbh_masses_powheg='' --var=\"%(var_used)s%(bin_used)s\" %(add_cond)s --ratio_range="0.5,1.5" --datacard="%(cat)s_mTLt65" ' % vars()

           commands = [run_cmd, run_cmd_2]
           if ',' in var_used: var_used=var_used.replace(',','_vs_')

           for i in range(0,len(commands)):
             if commands[i] is '': continue
             job_file = '%(output_folder)s/jobs/%(var_used)s_%(cat)s_%(ch)s_%(year)s_%(i)i.sh' % vars()
             CreateBatchJob(job_file,cmssw_base,[commands[i]])
             if 'm_vis' in var_used: 
               os.system('sed -i \'s/m_vis>40&&m_vis<80/1/g\' %(job_file)s ' % vars())
             if 'mt_1' in var_used:
               os.system('sed -i \'s/mt_1<65/1/g\' %(job_file)s ' % vars())
             SubmitBatchJob(job_file,time=180,memory=24,cores=1)

