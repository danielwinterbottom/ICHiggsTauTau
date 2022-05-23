#!/usr/bin/env python

# -- Example Commands & Comments ---

#python scripts/HiggsTauTauPlot.py --folder=/vols/cms/ks1021/output/test_2018 --embed_folder='' --channel=tt --method=17 --var="n_jets[1,2,3,4,50]" --ggh_masses_powheg='' --bbh_masses_powheg='' --embedding --cfg="scripts/plot_mssm_2018.cfg" --outputfolder=/vols/cms/ks1021/output/test_2018

#python scripts/makeDatacards_test.py --years='2016,2017,2018' --channels='tt,mt,et' --folder='~/vols/cms/ks1021/output/test_2018" --output_folder='~/vols/cms/ks1021/output/test_2018' --batch


# python scripts/makeDatacards_mssm_combined.py --years='2016,2017,2018' --channels='tt,mt,et' --batch

# python scripts/makeDatacards_mssm_combined.py --years='2016,2017,2018' --channels='tt,mt,et' --output_folder='mssm_dc' --batch

# python scripts/makeDatacards_mssm_combined.py --years='2016,2017,2018' --channels='tt,mt,et' --output_folder='mssm_dc_v4' --no_syst --batch

# after running all jobs hadd them using this commnd inside the output folder:
#for ch in tt et mt; do for year in 2016 2017 2018; do eval "hadd -f ${year}/${ch}/htt_all.inputs-mssm-vs-sm-Run${year}-mt_tot_puppi.root ${year}/${ch}/*.root"; done; done

# --- Main Body ---

# importing packages
import sys
from optparse import OptionParser
import os

CHANNELS = ['et','mt','tt','zmm','zee']
config_files = {'2016':'scripts/plot_mssm_2016.cfg',
                '2017':'scripts/plot_mssm_2017.cfg',
                '2018':'scripts/plot_mssm_2018.cfg'}
param_files = {'2016':'scripts/params_mssm_2016.json',
               '2017':'scripts/params_mssm_2017.json',
               '2018':'scripts/params_mssm_2018.json'}

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
# parsing command-line options
parser.add_option("--folder", dest = "folder",type='string',help = "Name of input folder")
parser.add_option("--output_folder", dest="output_folder", type='string', default='mssm_datacards',
                  help="Output folder where plots/datacards will be saved to.")
parser.add_option("-c", "--channels", dest="channels", type='string', action='callback',callback=split_callback,
                  help="A comma separated list of channels to process.  Supported channels: %(CHANNELS)s" % vars())
parser.add_option("--batch", dest="batch", action='store_true', default=False,
                  help="Submit on batch.")
parser.add_option("--years", dest="years", type='string', default='2016,2017,2018',
                  help="Year input")

(options, args) = parser.parse_args()

# initialising variables
input_folder = options.folder
output_folder = options.output_folder
channels = options.channels
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

#categories_et = ["MTLt70"]
categories_et = ["inclusive"]

#categories_mt = ["MTLt70"]
categories_mt = ["inclusive"]

categories_tt = ["inclusive"]

categories_zmm = ["inclusive"]

categories_zee = ["inclusive"]

cat_schemes = {'et' : categories_et,
               'mt' : categories_mt,
               'tt' : categories_tt,
	       'zmm': categories_zmm,
	       'zee': categories_zee}
for year in years:

  if not os.path.isdir('%(output_folder)s/%(year)s' % vars()):
    os.system("mkdir %(output_folder)s/%(year)s" % vars())

  # initialise list of paths of config files
  CFG = config_files[year]

 ########## Set up schemes and options ############
  BINS = '[0,20,40,60,80,100,120,140,160,180,200,220,240,260,280,300,340,380,420,460,500]'
  BINSlt200 = '[0,20,40,60,80,100,120,140,160,180,200]'
  BINSlt400 = '[0,20,40,60,80,100,120,140,160,180,200,220,240,260,280,300,320,340,360,380,400]'
  BINSlt500 = '[0,20,40,60,80,100,120,140,160,180,200,220,240,260,280,300,340,380,420,460,500]'

  var = [['mt_tot', BINS],
        ['m_vis', BINS]]
        # ['met',BINSlt200],
        # ['mt_1',BINSlt400],
        # ['mt_2',BINSlt200],
        # ['pt_1',BINSlt400],
        # ['pt_2',BINSlt200],
        # ['pt_tt',BINSlt500]]


  for ch in channels:

    if not os.path.isdir('%(output_folder)s/%(year)s/%(ch)s' % vars()):
      os.system("mkdir %(output_folder)s/%(year)s/%(ch)s" % vars())

    add_cond = '--add_wt=\'wt_tau_trg_mssm*wt_tau_id_mssm*wt_prefire*(1/puweight)\''
    method='8'
    categories = cat_schemes[ch]
    for cat in categories:
	
 	for item in var:
            var_used = item[0]
            bin_used = item[1]

            run_cmd = 'python %(cmssw_base)s/src/UserCode/ICHiggsTauTau/Analysis/HiggsTauTauRun2/scripts/HiggsTauTauPlot.py --cfg=%(CFG)s --sel=\'m_vis>50\' --channel=%(ch)s --method=%(method)s --cat=%(cat)s --outputfolder=%(output_folder)s/%(year)s/%(ch)s --ggh_masses_powheg='' --bbh_masses_powheg='' --var="%(var_used)s%(bin_used)s"  %(add_cond)s' % vars()


            commands = [run_cmd]
            #if run_cmd_alt1 != '': commands.append((run_cmd_alt1, rename_cmd_alt1))
            #if run_cmd_alt2 != '': commands.append((run_cmd_alt2, rename_cmd_alt2))
            #if run_cmd_alt3 != '': commands.append((run_cmd_alt3, rename_cmd_alt3)) 

            job_file = '%(output_folder)s/jobs/%(var_used)s_%(cat)s_test_%(ch)s_%(year)s.sh' % vars()
	    CreateBatchJob(job_file,cmssw_base,[run_cmd])
            SubmitBatchJob(job_file,time=180,memory=24,cores=1)





