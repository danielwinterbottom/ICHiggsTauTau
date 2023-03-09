#!/usr/bin/env python

# importing packages
import sys
from optparse import OptionParser
import os
import numpy as np

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
parser.add_option('--channel',help= 'Name of input channels', default='mt,et,tt')
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
         ['n_jets', BINS(0,8,1,1)],
         ['eta_1',BINS(-2.1,2.1,0.1,0.1)],
         ['eta_2',BINS(-2.1,2.1,0.1,0.1)],
         ['tau_decay_mode_2',BINS(0,12,1,1)],
         ['tau_decay_mode_1',BINS(0,12,1,1)],
         ['pt_2', BINS(0,200,5,1)],
         ['pt_1', BINS(0,200,5,1)],
         ['met', BINS(0,200,10,1)],
         ['pt_vis', BINS(0,300,10,1)],
         ['pt_tt', BINS(0,300,10,1)],
         ['dR', BINS(0,7,0.03,0.03)],
         ]        
var_mt = [
         ['m_vis', BINS(0,300,10,1)],
         ['n_jets', BINS(0,8,1,1)],
         ['eta_1',BINS(-2.1,2.1,0.1,0.1)],
         ['eta_2',BINS(-2.1,2.1,0.1,0.1)],
         ['tau_decay_mode_2',BINS(0,12,1,1)],
         ['pt_2', BINS(0,200,5,1)],
         ['pt_1', BINS(0,200,5,1)],
         ['tau_decay_mode_2,pt_2', '[0,1,10,11],[20,25,30,35,40,50,60,80,100,200]'],
         ['met', BINS(0,200,10,1)],
         ['pt_vis', BINS(0,300,10,1)],
         ['pt_tt', BINS(0,300,10,1)],
         ['dR', BINS(0,7,0.03,0.03)],
         ['mt_1', BINS(0,150,5,1)],
         ]        
var_et = [
         ['m_vis', BINS(0,300,10,1)],
         ['n_jets', BINS(0,8,1,1)],
         ['eta_1',BINS(-2.1,2.1,0.1,0.1)],
         ['eta_2',BINS(-2.1,2.1,0.1,0.1)],
         ['tau_decay_mode_2',BINS(0,12,1,1)],
         ['pt_2', BINS(0,200,5,1)],
         ['pt_1', BINS(0,200,5,1)],
         ['tau_decay_mode_2,pt_2', '[0,1,10,11],[20,25,30,35,40,50,60,80,100,200]'],
         ['met', BINS(0,200,10,1)],
         ['pt_vis', BINS(0,300,10,1)],
         ['pt_tt', BINS(0,300,10,1)],
         ['dR', BINS(0,7,0.03,0.03)],
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

    # pT-dependent binned SF

   # if year == '2016_preVFP':
   #   #pT-binned SFs for era 2016_preVFP:
   #   sf = '((gen_match_2!=5) + (gen_match_2==5)*((pt_2>=20&&pt_2<25)*(1.043)+(pt_2>=25&&pt_2<30)*(1.042)+(pt_2>=30&&pt_2<35)*(1.043)+(pt_2>=35&&pt_2<40)*(1.021)+(pt_2>=40&&pt_2<50)*(1.019)+(pt_2>=50&&pt_2<60)*(1.028)+(pt_2>=60&&pt_2<80)*(0.939)+(pt_2>=80&&pt_2<100)*(0.920)+(pt_2>=100)*(0.929)))'
   # if year == '2016_postVFP':
   #   #pT-binned SFs for era 2016_postVFP:
   #   sf = '((gen_match_2!=5) + (gen_match_2==5)*((pt_2>=20&&pt_2<25)*(0.813)+(pt_2>=25&&pt_2<30)*(0.823)+(pt_2>=30&&pt_2<35)*(0.869)+(pt_2>=35&&pt_2<40)*(0.909)+(pt_2>=40&&pt_2<50)*(0.907)+(pt_2>=50&&pt_2<60)*(0.870)+(pt_2>=60&&pt_2<80)*(0.975)+(pt_2>=80&&pt_2<100)*(0.918)+(pt_2>=100)*(0.916)))'
   # if year == '2017':
   #   #pT-binned SFs for era 2017:
   #   sf = '((gen_match_2!=5) + (gen_match_2==5)*((pt_2>=20&&pt_2<25)*(0.773)+(pt_2>=25&&pt_2<30)*(0.831)+(pt_2>=30&&pt_2<35)*(0.854)+(pt_2>=35&&pt_2<40)*(0.884)+(pt_2>=40&&pt_2<50)*(0.902)+(pt_2>=50&&pt_2<60)*(0.905)+(pt_2>=60&&pt_2<80)*(0.951)+(pt_2>=80&&pt_2<100)*(0.923)+(pt_2>=100)*(0.947)))'
   # if year == '2018':
   #   #pT-binned SFs for era 2018:
   #   sf = '((gen_match_2!=5) + (gen_match_2==5)*((pt_2>=20&&pt_2<25)*(0.783)+(pt_2>=25&&pt_2<30)*(0.824)+(pt_2>=30&&pt_2<35)*(0.859)+(pt_2>=35&&pt_2<40)*(0.871)+(pt_2>=40&&pt_2<50)*(0.924)+(pt_2>=50&&pt_2<60)*(0.890)+(pt_2>=60&&pt_2<80)*(0.926)+(pt_2>=80&&pt_2<100)*(0.957)+(pt_2>=100)*(0.968)))'

    # DM-binned pT-dependent SF
    #DM-binned SFs for era 2016_preVFP:
    sf = '((gen_match_2!=5) + (gen_match_2==5)*((tau_decay_mode_2==0)*((1.03397+0.000407969*min(pt_2,125.)))+(tau_decay_mode_2==1)*((1.15107+-0.00211504*min(pt_2,125.)))+(tau_decay_mode_2==10)*((1.03783+-0.0011254*min(pt_2,125.)))+(tau_decay_mode_2==11)*((0.975761+-0.00305113*min(pt_2,125.)))))'
    #DM-binned SFs for era 2016_postVFP:
    sf = '((gen_match_2!=5) + (gen_match_2==5)*((tau_decay_mode_2==0)*((0.978417+5.72368e-05*min(pt_2,125.)))+(tau_decay_mode_2==1)*(1.04437*TMath::Erf((min(pt_2,125.)--10.8845)/46.7379))+(tau_decay_mode_2==10)*((0.927759+-0.000719045*min(pt_2,125.)))+(tau_decay_mode_2==11)*(0.766739*TMath::Erf((min(pt_2,125.)-16.5928)/11.2111))))'
    #DM-binned SFs for era 2017:
    sf = '((gen_match_2!=5) + (gen_match_2==5)*((tau_decay_mode_2==0)*((1.04266+-0.00158397*min(pt_2,125.)))+(tau_decay_mode_2==1)*(1.01061*TMath::Erf((min(pt_2,125.)--13.518)/48.5772))+(tau_decay_mode_2==10)*(0.934699*TMath::Erf((min(pt_2,125.)-6.19958)/14.8044))+(tau_decay_mode_2==11)*(0.939418*TMath::Erf((min(pt_2,125.)--2.64469)/40.9824))))'
    #DM-binned SFs for era 2018:
    sf = '((gen_match_2!=5) + (gen_match_2==5)*((tau_decay_mode_2==0)*((1.00465+-8.36161e-05*min(pt_2,125.)))+(tau_decay_mode_2==1)*(0.93569*TMath::Erf((min(pt_2,125.)-1.11787)/28.7657))+(tau_decay_mode_2==10)*(0.932986*TMath::Erf((min(pt_2,125.)--13.0774)/36.7303))+(tau_decay_mode_2==11)*((0.72108+0.00110556*min(pt_2,125.)))))'

    if 'pt_2' in sf:
      sf+='*%s' % sf.replace('_2','_1')

#older prefit numbers

# binned SF inclusive by decay mode
#    if year == '2016_preVFP':
#      sf = '((gen_match_1!=5) + (gen_match_1==5)*((pt_1>=20.0&&pt_1<25.0)*1.040+(pt_1>=25.0&&pt_1<30.0)*1.053+(pt_1>=30.0&&pt_1<35.0)*1.044+(pt_1>=35.0&&pt_1<40.0)*1.016+(pt_1>=40.0&&pt_1<50.0)*1.014+(pt_1>=50.0&&pt_1<60.0)*1.091+(pt_1>=60.0&&pt_1<80.0)*0.955+(pt_1>=80.0&&pt_1<100.0)*0.957+(pt_1>=100.0)*0.947))*((gen_match_2!=5) + (gen_match_2==5)*((pt_2>=20.0&&pt_2<25.0)*1.040+(pt_2>=25.0&&pt_2<30.0)*1.053+(pt_2>=30.0&&pt_2<35.0)*1.044+(pt_2>=35.0&&pt_2<40.0)*1.016+(pt_2>=40.0&&pt_2<50.0)*1.014+(pt_2>=50.0&&pt_2<60.0)*1.091+(pt_2>=60.0&&pt_2<80.0)*0.955+(pt_2>=80.0&&pt_2<100.0)*0.957+(pt_2>=100.0)*0.947))'
#    if year == '2016_postVFP':
#      sf = '((gen_match_1!=5) + (gen_match_1==5)*((pt_1>=20.0&&pt_1<25.0)*0.819+(pt_1>=25.0&&pt_1<30.0)*0.849+(pt_1>=30.0&&pt_1<35.0)*0.872+(pt_1>=35.0&&pt_1<40.0)*0.902+(pt_1>=40.0&&pt_1<50.0)*0.901+(pt_1>=50.0&&pt_1<60.0)*0.907+(pt_1>=60.0&&pt_1<80.0)*1.041+(pt_1>=80.0&&pt_1<100.0)*0.984+(pt_1>=100.0)*0.912))*((gen_match_2!=5) + (gen_match_2==5)*((pt_2>=20.0&&pt_2<25.0)*0.819+(pt_2>=25.0&&pt_2<30.0)*0.849+(pt_2>=30.0&&pt_2<35.0)*0.872+(pt_2>=35.0&&pt_2<40.0)*0.902+(pt_2>=40.0&&pt_2<50.0)*0.901+(pt_2>=50.0&&pt_2<60.0)*0.907+(pt_2>=60.0&&pt_2<80.0)*1.041+(pt_2>=80.0&&pt_2<100.0)*0.984+(pt_2>=100.0)*0.912))'
#    if year == '2017':
#      sf = '((gen_match_1!=5) + (gen_match_1==5)*((pt_1>=20.0&&pt_1<25.0)*0.780+(pt_1>=25.0&&pt_1<30.0)*0.828+(pt_1>=30.0&&pt_1<35.0)*0.848+(pt_1>=35.0&&pt_1<40.0)*0.868+(pt_1>=40.0&&pt_1<50.0)*0.889+(pt_1>=50.0&&pt_1<60.0)*0.918+(pt_1>=60.0&&pt_1<80.0)*0.989+(pt_1>=80.0&&pt_1<100.0)*0.982+(pt_1>=100.0)*1.020))*((gen_match_2!=5) + (gen_match_2==5)*((pt_2>=20.0&&pt_2<25.0)*0.780+(pt_2>=25.0&&pt_2<30.0)*0.828+(pt_2>=30.0&&pt_2<35.0)*0.848+(pt_2>=35.0&&pt_2<40.0)*0.868+(pt_2>=40.0&&pt_2<50.0)*0.889+(pt_2>=50.0&&pt_2<60.0)*0.918+(pt_2>=60.0&&pt_2<80.0)*0.989+(pt_2>=80.0&&pt_2<100.0)*0.982+(pt_2>=100.0)*1.020))'
#    if year == '2018':
#      sf = '((gen_match_1!=5) + (gen_match_1==5)*((pt_1>=20.0&&pt_1<25.0)*0.795+(pt_1>=25.0&&pt_1<30.0)*0.831+(pt_1>=30.0&&pt_1<35.0)*0.863+(pt_1>=35.0&&pt_1<40.0)*0.890+(pt_1>=40.0&&pt_1<50.0)*0.946+(pt_1>=50.0&&pt_1<60.0)*0.937+(pt_1>=60.0&&pt_1<80.0)*0.940+(pt_1>=80.0&&pt_1<100.0)*0.984+(pt_1>=100.0)*0.972))*((gen_match_2!=5) + (gen_match_2==5)*((pt_2>=20.0&&pt_2<25.0)*0.795+(pt_2>=25.0&&pt_2<30.0)*0.831+(pt_2>=30.0&&pt_2<35.0)*0.863+(pt_2>=35.0&&pt_2<40.0)*0.890+(pt_2>=40.0&&pt_2<50.0)*0.946+(pt_2>=50.0&&pt_2<60.0)*0.937+(pt_2>=60.0&&pt_2<80.0)*0.940+(pt_2>=80.0&&pt_2<100.0)*0.984+(pt_2>=100.0)*0.972))'

# SF binned in dm and fitted vs pT
    #if year == '2016_preVFP':
    #  sf = '((gen_match_1!=5) + (gen_match_1==5)*((tau_decay_mode_1==0)*(1.06176*TMath::Erf((TMath::Max(TMath::Min(pt_1,150.),25.)+32.2088)/39.1701))+(tau_decay_mode_1==1)*(0.473167*TMath::Landau(TMath::Max(TMath::Min(pt_1,150.),25.),25.8034,3.26054)+1.02112)+(tau_decay_mode_1==10)*(1.40138*TMath::Landau(TMath::Max(TMath::Min(pt_1,150.),25.),37.788,18.5902)+0.742156)+(tau_decay_mode_1==11)*(26.5296*TMath::Landau(TMath::Max(TMath::Min(pt_1,150.),25.),-61.8318,6.68426)+0.754471)))*((gen_match_2!=5) + (gen_match_2==5)*((tau_decay_mode_2==0)*(1.06176*TMath::Erf((TMath::Max(TMath::Min(pt_2,150.),25.)+32.2088)/39.1701))+(tau_decay_mode_2==1)*(0.473167*TMath::Landau(TMath::Max(TMath::Min(pt_2,150.),25.),25.8034,3.26054)+1.02112)+(tau_decay_mode_2==10)*(1.40138*TMath::Landau(TMath::Max(TMath::Min(pt_2,150.),25.),37.788,18.5902)+0.742156)+(tau_decay_mode_2==11)*(26.5296*TMath::Landau(TMath::Max(TMath::Min(pt_2,150.),25.),-61.8318,6.68426)+0.754471)))'
    #if year == '2016_postVFP':
    #  sf = '((gen_match_1!=5) + (gen_match_1==5)*((tau_decay_mode_1==0)*(1.01745*TMath::Erf((TMath::Max(TMath::Min(pt_1,150.),25.)-18.964)/1.95451))+(tau_decay_mode_1==1)*(1.01584*TMath::Erf((TMath::Max(TMath::Min(pt_1,150.),25.)+23.2708)/61.027))+(tau_decay_mode_1==10)*(0.889975*TMath::Erf((TMath::Max(TMath::Min(pt_1,150.),25.)+43.1388)/16.5746))+(tau_decay_mode_1==11)*(0.806998*TMath::Erf((TMath::Max(TMath::Min(pt_1,150.),25.)+5.28462)/25.1591))))*((gen_match_2!=5) + (gen_match_2==5)*((tau_decay_mode_2==0)*(1.01745*TMath::Erf((TMath::Max(TMath::Min(pt_2,150.),25.)-18.964)/1.95451))+(tau_decay_mode_2==1)*(1.01584*TMath::Erf((TMath::Max(TMath::Min(pt_2,150.),25.)+23.2708)/61.027))+(tau_decay_mode_2==10)*(0.889975*TMath::Erf((TMath::Max(TMath::Min(pt_2,150.),25.)+43.1388)/16.5746))+(tau_decay_mode_2==11)*(0.806998*TMath::Erf((TMath::Max(TMath::Min(pt_2,150.),25.)+5.28462)/25.1591))))'
    #if year == '2017':
    #  sf = '((gen_match_1!=5) + (gen_match_1==5)*((tau_decay_mode_1==0)*(0.983337*TMath::Erf((TMath::Max(TMath::Min(pt_1,150.),25.)+28.5993)/13.0616))+(tau_decay_mode_1==1)*(1.01791*TMath::Erf((TMath::Max(TMath::Min(pt_1,150.),25.)+18.196)/59.647))+(tau_decay_mode_1==10)*(0.978292*TMath::Erf((TMath::Max(TMath::Min(pt_1,150.),25.)+64.4343)/83.7118))+(tau_decay_mode_1==11)*(1.15357*TMath::Erf((TMath::Max(TMath::Min(pt_1,150.),25.)+30.4798)/100.967))))*((gen_match_2!=5) + (gen_match_2==5)*((tau_decay_mode_2==0)*(0.983337*TMath::Erf((TMath::Max(TMath::Min(pt_2,150.),25.)+28.5993)/13.0616))+(tau_decay_mode_2==1)*(1.01791*TMath::Erf((TMath::Max(TMath::Min(pt_2,150.),25.)+18.196)/59.647))+(tau_decay_mode_2==10)*(0.978292*TMath::Erf((TMath::Max(TMath::Min(pt_2,150.),25.)+64.4343)/83.7118))+(tau_decay_mode_2==11)*(1.15357*TMath::Erf((TMath::Max(TMath::Min(pt_2,150.),25.)+30.4798)/100.967))))'

    #if year == '2018':
    #  sf = '((gen_match_1!=5) + (gen_match_1==5)*((tau_decay_mode_1==0)*(1.06083*TMath::Erf((TMath::Max(TMath::Min(pt_1,150.),25.)+130.861)/106.183))+(tau_decay_mode_1==1)*(0.964479*TMath::Erf((TMath::Max(TMath::Min(pt_1,150.),25.)+4.64593)/37.2886))+(tau_decay_mode_1==10)*(0.957509*TMath::Erf((TMath::Max(TMath::Min(pt_1,150.),25.)+15.5771)/38.3974))+(tau_decay_mode_1==11)*(21.6076*TMath::Erf((TMath::Max(TMath::Min(pt_1,150.),25.)+385.456)/13633.2))))*((gen_match_2!=5) + (gen_match_2==5)*((tau_decay_mode_2==0)*(1.06083*TMath::Erf((TMath::Max(TMath::Min(pt_2,150.),25.)+130.861)/106.183))+(tau_decay_mode_2==1)*(0.964479*TMath::Erf((TMath::Max(TMath::Min(pt_2,150.),25.)+4.64593)/37.2886))+(tau_decay_mode_2==10)*(0.957509*TMath::Erf((TMath::Max(TMath::Min(pt_2,150.),25.)+15.5771)/38.3974))+(tau_decay_mode_2==11)*(21.6076*TMath::Erf((TMath::Max(TMath::Min(pt_2,150.),25.)+385.456)/13633.2))))'



    if ch=='tt': 
      add_cond = '--add_wt=\'wt_prefire*%(sf)s/wt_tau_id_dm\'' % vars()
    else: add_cond = '--add_wt=\'wt_prefire*%(sf)s/wt_tau_id_pt\'' % vars()

    #uncomment if you want to apply the old corrections
    #if ch=='tt':
    #  add_cond = '--add_wt=\'wt_prefire*%(sf)s\'' % vars()
    #else: add_cond = '--add_wt=\'wt_prefire*%(sf)s\'' % vars()
 
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
           #run_cmd = 'python %(cmssw_base)s/src/UserCode/ICHiggsTauTau/Analysis/HiggsTauTauRun2/scripts/HiggsTauTauPlot.py --cfg=%(CFG)s --channel=%(ch)s --sel=\'(mt_1<30) &&(m_vis>40) && (m_vis<80) && (pt_1>%(pt1_cut)s)\' --method=%(method)s --cat=%(cat)s --outputfolder=%(output_folder)s/%(year)s/%(ch)s --ggh_masses_powheg='' --bbh_masses_powheg='' --var="%(var_used)s%(bin_used)s" %(add_cond)s' % vars()
           if ch == 'tt': 
             run_cmd = 'python %(cmssw_base)s/src/UserCode/ICHiggsTauTau/Analysis/HiggsTauTauRun2/scripts/HiggsTauTauPlot.py --cfg=%(CFG)s --channel=%(ch)s --sel=\'(m_vis>40&&m_vis<80)\' --method=%(method)s --cat=%(cat)s --outputfolder=%(output_folder)s/%(year)s/%(ch)s --ggh_masses_powheg='' --bbh_masses_powheg='' --var=\"%(var_used)s%(bin_used)s\" %(add_cond)s --ratio_range="0.5,1.5"' % vars()
             run_cmd_2 = 'python %(cmssw_base)s/src/UserCode/ICHiggsTauTau/Analysis/HiggsTauTauRun2/scripts/HiggsTauTauPlot.py --cfg=%(CFG)s --channel=%(ch)s --sel=\'(m_vis>40&&m_vis<80)\' --method=%(method)s --cat=%(cat)s --outputfolder=%(output_folder)s/%(year)s/%(ch)s --ggh_masses_powheg='' --bbh_masses_powheg='' --var=\"%(var_used)s%(bin_used)s\" %(add_cond)s --ratio_range="0.5,1.5" --datacard="ss" --do_ss' % vars()
           elif ch == 'mt':
             run_cmd = 'python %(cmssw_base)s/src/UserCode/ICHiggsTauTau/Analysis/HiggsTauTauRun2/scripts/HiggsTauTauPlot.py --cfg=%(CFG)s --channel=%(ch)s --sel=\'(mt_1<30)\' --set_alias=\'%(cat)s:({%(cat)s}&&m_vis>40&&m_vis<80&&pt_1>25&&trg_singlemuon)\' --method=%(method)s --cat=%(cat)s --outputfolder=%(output_folder)s/%(year)s/%(ch)s --ggh_masses_powheg='' --bbh_masses_powheg='' --var=\"%(var_used)s%(bin_used)s\" %(add_cond)s --ratio_range="0.5,1.5" ' % vars()
             run_cmd_2 = 'python %(cmssw_base)s/src/UserCode/ICHiggsTauTau/Analysis/HiggsTauTauRun2/scripts/HiggsTauTauPlot.py --cfg=%(CFG)s --channel=%(ch)s --sel=\'(mt_1<60)\' --set_alias=\'%(cat)s:({%(cat)s}&&m_vis>40&&m_vis<80&&pt_1>25&&trg_singlemuon)\' --method=%(method)s --cat=%(cat)s --outputfolder=%(output_folder)s/%(year)s/%(ch)s --ggh_masses_powheg='' --bbh_masses_powheg='' --var=\"%(var_used)s%(bin_used)s\" %(add_cond)s --ratio_range="0.5,1.5" --datacard="%(cat)s_mTLt60" ' % vars()
           elif ch == 'et': 

             run_cmd = 'python %(cmssw_base)s/src/UserCode/ICHiggsTauTau/Analysis/HiggsTauTauRun2/scripts/HiggsTauTauPlot.py --cfg=%(CFG)s --channel=%(ch)s --sel=\'(mt_1<30)\' --set_alias=\'%(cat)s:({%(cat)s}&&m_vis>40&&m_vis<80&&pt_1>25&&trg_singleelectron)\' --method=%(method)s --cat=%(cat)s --outputfolder=%(output_folder)s/%(year)s/%(ch)s --ggh_masses_powheg='' --bbh_masses_powheg='' --var=\"%(var_used)s%(bin_used)s\" %(add_cond)s --ratio_range="0.5,1.5" ' % vars()
             run_cmd_2 = 'python %(cmssw_base)s/src/UserCode/ICHiggsTauTau/Analysis/HiggsTauTauRun2/scripts/HiggsTauTauPlot.py --cfg=%(CFG)s --channel=%(ch)s --sel=\'(mt_1<60)\' --set_alias=\'%(cat)s:({%(cat)s}&&m_vis>40&&m_vis<80&&pt_1>25&&trg_singleelectron)\' --method=%(method)s --cat=%(cat)s --outputfolder=%(output_folder)s/%(year)s/%(ch)s --ggh_masses_powheg='' --bbh_masses_powheg='' --var=\"%(var_used)s%(bin_used)s\" %(add_cond)s --ratio_range="0.5,1.5" --datacard="%(cat)s_mTLt60" ' % vars()

           commands = [run_cmd, run_cmd_2]
           if ',' in var_used: var_used=var_used.replace(',','_vs_')

           for i in range(0,len(commands)):
             if commands[i] is '': continue
             job_file = '%(output_folder)s/jobs/%(var_used)s_%(cat)s_%(ch)s_%(year)s_%(i)i.sh' % vars()
             CreateBatchJob(job_file,cmssw_base,[commands[i]])
             if 'm_vis' in var_used: 
               os.system('sed -i \'s/m_vis>40&&m_vis<80/1/g\' %(job_file)s ' % vars())
             if 'mt_1' in var_used:
               os.system('sed -i \'s/mt_1<30/1/g\' %(job_file)s ' % vars())
             SubmitBatchJob(job_file,time=180,memory=24,cores=1)
#
