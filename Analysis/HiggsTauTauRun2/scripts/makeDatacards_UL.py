#!/usr/bin/env python

# importing packages
import sys
from optparse import OptionParser
import os
import numpy as np

config_files = {'2016_preVFP':'scripts/plot_UL_2016_preVFP.cfg',
		        '2016_postVFP':'scripts/plot_UL_2016_postVFP.cfg',
                '2017':'scripts/plot_UL_2017.cfg',
                '2018':'scripts/plot_UL_2018.cfg'}
param_files = {'2016_preVFP':'scripts/params_UL_2016_preVFP.json',
	           '2016_postVFP':'scripts/params_UL_2016_postVFP.json',
               '2017':'scripts/params_UL_2017.json',
               '2018':'scripts/params_UL_2018.json'}
               
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
parser.add_option('--channel',help= 'Name of input channels', default='et,mt,tt')
parser.add_option("--years", dest="years", type='string', default='2016_preVFP,2016_postVFP,2017,2018',help="Year input")
parser.add_option('--output_folder', help= 'Name of output folder to create', default='plots')
parser.add_option("--batch", dest="batch", action='store_true', default=False, help="Submit on batch.")
(options, args) = parser.parse_args()

# initialising variables
output_folder = options.output_folder
channels = options.channel.split(',')
years = options.years.split(',')

print 'Processing channels:      %(channels)s' % vars()
print 'Processing years:         %(years)s' % vars()

# cmssw directory
cmssw_base = os.getcwd().replace('src/UserCode/ICHiggsTauTau/Analysis/HiggsTauTauRun2','')

# check whether path is in an exitisting directory
if not os.path.isdir('%(output_folder)s' % vars()):
  os.system("mkdir %(output_folder)s" % vars())
if not os.path.isdir('%(output_folder)s/jobs' % vars()):
  os.system("mkdir %(output_folder)s/jobs" % vars())
  
categories_et = ["inclusive"]
categories_mt = ["inclusive"]
categories_tt = ["inclusive"]
categories_zmm = ["inclusive"]
categories_zee = ["inclusive"]
cat_schemes = {'et' : categories_et,
               'mt' : categories_mt,
               'tt' : categories_tt,
	       'zmm': categories_zmm,
	       'zee': categories_zee}

BINS = np.arange(0,251,10)
BINS_str = ','.join(str(x) for x in BINS)
BINS = []
BINS.append(BINS_str)

BINS_eta = np.arange(-2.1,2.3,0.2)
BINS_eta_str = ','.join(str(x) for x in BINS_eta)
BINS_eta = []
BINS_eta.append(BINS_eta_str)

BINS_pt = np.arange(0,151,10)
BINS_pt_str = ','.join(str(x) for x in BINS_pt)
BINS_pt = []
BINS_pt.append(BINS_pt_str)


var_tt = [
         ['m_vis', BINS],
         ['eta_2',BINS_eta],
         ['tau_decay_mode_2','[0,1,2,3,4,5,6,7,8,9,10,11,12]'],
         ['pt_2', BINS_pt],
         ['eta_1',BINS_eta],
         ['tau_decay_mode_1','[0,1,2,3,4,5,6,7,8,9,10,11,12]'],
         ['pt_1', BINS_pt]
         ]        
var_mt = [
         ['m_vis', BINS],
         ['eta_2',BINS_eta],
         ['tau_decay_mode_2','[0,1,2,3,4,5,6,7,8,9,10,11,12]'],
         ['pt_2', BINS_pt]
         ]        
var_et = [
         ['m_vis', BINS],
         ['eta_2',BINS_eta],
         ['tau_decay_mode_2','[0,1,2,3,4,5,6,7,8,9,10,11,12]'],
         ['pt_2', BINS_pt],
         ]            

var_schemes = {'et' : var_et,
               'mt' : var_mt,
               'tt' : var_tt}
tau_id = "pt_binned"
print(tau_id)
for year in years:
  if not os.path.isdir('%(output_folder)s/%(year)s' % vars()):
    os.system("mkdir %(output_folder)s/%(year)s" % vars())
  CFG = config_files[year]
  for ch in channels:
    if not os.path.isdir('%(output_folder)s/%(year)s/%(ch)s' % vars()):
      os.system("mkdir %(output_folder)s/%(year)s/%(ch)s" % vars())
    if tau_id == "dm_binned":
      if ch == "tt":
         add_cond = '--add_wt=\'wt_prefire*(1/wt_tau_id_dm)*wt_tau_id_pt\''
      if ch == "mt":
         add_cond = '--add_wt=\'wt_prefire\''
      if ch == "et":
         add_cond = '--add_wt=\'wt_prefire\''
    elif tau_id == "dm_binned":
      if ch == "tt":
         add_cond = '--add_wt=\'wt_prefire\''
      if ch == "mt":
         add_cond = '--add_wt=\'wt_prefire*(1/wt_tau_id_pt)*wt_tau_id_dm\''
      if ch == "et":
         add_cond = '--add_wt=\'wt_prefire*(1/wt_tau_id_pt)*wt_tau_id_dm\''
    else:
      add_cond = '--add_wt=\'wt_prefire\''
      #add_cond = '--add_wt=\'wt_tau_trg_mssm*wt_tau_id_mssm*wt_prefire*(1/wt_dysoup)\''
      #add_cond = '--add_wt =\'1/wt_dysoup\''
    method='8'
    categories = cat_schemes[ch]
    variables = var_schemes[ch]
    for cat in categories:
      for item in variables:
        var_used = item[0]
        bin_used = item[1]
        #run_cmd = 'python %(cmssw_base)s/src/UserCode/ICHiggsTauTau/Analysis/HiggsTauTauRun2/scripts/HiggsTauTauPlot.py --cfg=%(CFG)s --ff_ss_closure  --channel=%(ch)s --method=%(method)s --cat=%(cat)s --outputfolder=%(output_folder)s/%(year)s/%(ch)s --ggh_masses_powheg='' --bbh_masses_powheg=''  --var="%(var_used)s%(bin_used)s"  %(add_cond)s' % vars()
        run_cmd = 'python %(cmssw_base)s/src/UserCode/ICHiggsTauTau/Analysis/HiggsTauTauRun2/scripts/HiggsTauTauPlot.py --cfg=%(CFG)s --channel=%(ch)s --method=%(method)s --cat=%(cat)s --outputfolder=%(output_folder)s/%(year)s/%(ch)s --ggh_masses_powheg='' --bbh_masses_powheg=''  --var="%(var_used)s%(bin_used)s"  %(add_cond)s' % vars()
        commands = [run_cmd]
        job_file = '%(output_folder)s/jobs/%(var_used)s_%(cat)s_test_%(ch)s_%(year)s.sh' % vars()
        CreateBatchJob(job_file,cmssw_base,[run_cmd])
        SubmitBatchJob(job_file,time=180,memory=24,cores=1)
