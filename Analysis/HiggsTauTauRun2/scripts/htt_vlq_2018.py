#!/usr/bin/env python

# python scripts/htt_vlq_2018.py --bkg --data --embed --sm --vlq_matched --vlq_interference --jetmetuncerts --scales="default,scale_t_0pi,scale_t_1pi,scale_t_3prong,scale_t_3prong1pi0,scale_efake_0pi,scale_efake_1pi,scale_mufake_0pi,scale_mufake_1pi,scale_e,scale_bfake,scale_b" --submit='./scripts/submit_ic_batch_job.sh "hep.q -l h_rt=0:180:0 -l h_vmem=24G"' --parajobs


import sys
from optparse import OptionParser
import os
import math
import json

os.system("voms-proxy-init --voms cms --valid 96:00 --out ~/cms.proxy")

JOBWRAPPER      = './scripts/generate_job.sh'
JOBSUBMIT       = 'true'
if "JOBWRAPPER" in os.environ:      JOBWRAPPER      = os.environ["JOBWRAPPER"]
if "JOBSUBMIT"  in os.environ:      JOBSUBMIT       = os.environ["JOBSUBMIT"]
print "Using job-wrapper:    " + JOBWRAPPER
print "Using job-submission: " + JOBSUBMIT

CONDOR_TEMPLATE = """executable = ./jobs/%(EXE)s
Proxy_path =/afs/cern.ch/user/a/adow/private/x509up
arguments = $(ProcId) $(Proxy_path)
output                = ./jobs/%(TASK)s.$(ClusterId).$(ProcId).out
error                 = ./jobs/%(TASK)s.$(ClusterId).$(ProcId).err
log                   = ./jobs/%(TASK)s.$(ClusterId).log
requirements = (OpSysAndVer =?= "SLCern6")
+JobFlavour     = "longlunch"
queue
"""

def split_callback(option, opt, value, parser):
    setattr(parser.values, option.dest, value.split(','))

parser = OptionParser()
parser.add_option("--wrapper", dest="wrapper",
                  help="Specify the job-wrapper script. The current wrapper is '%(JOBWRAPPER)s'."
                  " Using the --wrapper option overrides both the default and the environment variable. " % vars())

parser.add_option("--submit", dest="submit",
                  help="Specify the job-submission method. The current method is '%(JOBSUBMIT)s'"
                  " Using the --submit option overrides both the default and the environment variable. " % vars())

parser.add_option("--data", dest="proc_data", action='store_true', default=False,
                  help="Process data samples")
parser.add_option("--embed", dest="proc_embed", action='store_true', default=False,
                  help="Process embedded sampes")
parser.add_option("--calc_lumi", dest="calc_lumi", action='store_true', default=False,
                  help="Run on data and only write out lumi mask jsons")

parser.add_option("--bkg", dest="proc_bkg", action='store_true', default=False,
                  help="Process background mc samples")

parser.add_option("--sm", dest="proc_sm", action='store_true', default=False,
                  help="Process signal SM mc samples")

parser.add_option("--vlq_old", dest="proc_vlq_old", action='store_true', default=False,
                  help="Process signal VLQ old mc samples")

parser.add_option("--vlq_exo", dest="proc_vlq_exo", action='store_true', default=False,
                  help="Process exo signal samples")

parser.add_option("--vlq_matched", dest="proc_vlq_matched", action='store_true', default=False,
                  help="Process signal VLQ matched mc samples")

parser.add_option("--vlq_interference", dest="proc_vlq_interference", action='store_true', default=False,
                  help="Process signal VLQ interference mc samples")

parser.add_option("--all", dest="proc_all", action='store_true', default=False,
                  help="Process all samples")

parser.add_option("--short_signal", dest="short_signal", action='store_true', default=False,
                  help="Only process the 125/160 signal samples")
parser.add_option("--cp_signal", dest="cp_signal", action='store_true', default=False,
                  help="Process the samples needed for CP studies")
parser.add_option("--mg_signal", dest="mg_signal", action='store_true', default=False,
                  help="Process the MG signal samples")
parser.add_option("--no_json", dest="no_json", action='store_true', default=False,
                  help="Do not read the channels to process from the json to decide which datasets to run on")

parser.add_option("--scales", dest="scales", type='string', default='default',
                  help="List of systematic shifts to process")

parser.add_option("--parajobs", dest="parajobs", action='store_true', default=False,
                  help="Submit jobs parametrically")
parser.add_option("--config", dest="config", type='string', default='',
                  help="Config file")
parser.add_option("--list_backup", dest="slbackupname", type='string', default='prevlist',
                  help="Name you want to give to the previous files_per_samples file, in case you're resubmitting a subset of jobs")
parser.add_option("--condor", action='store_true', default=False,
                  help="Submit jobs to condor (for lxplus)")
parser.add_option("--jetmetuncerts", dest="jetmetuncerts", action='store_true', default=False,
                  help="Do JES, JER, and MET uncertainties")


(options, args) = parser.parse_args()
if options.wrapper: JOBWRAPPER=options.wrapper
if options.submit:  JOBSUBMIT=options.submit
if options.condor: JOBWRAPPER = "./scripts/generate_condor_job.sh"

jetuncert_string=''
if options.jetmetuncerts:
  jetuncert_string='\\"do_jetmet_uncerts\\":true'
else:
  jetuncert_string='\\"do_jetmet_uncerts\\":false'

def getParaJobSubmit(N):
    if not options.submit: return 'true'
    sub_opts=JOBSUBMIT.split(' ',1)[1]
    if '\"' in sub_opts: sub_opts = sub_opts.replace('\"','')
    sub_opts = '%s \"%s -t 1-%i:1\"' %(JOBSUBMIT.split(' ',1)[0], sub_opts, N)
    return sub_opts

BACKUPNAME = options.slbackupname

#channels = options.channels
scales = options.scales
no_json = options.no_json
parajobs = options.parajobs

scale_list = scales.split(',')
flatjsonlist = []
flatjsonlistdysig = []
flatjsonlist.append("job:sequences:all:")
n_scales=0
for scale in scale_list:
    n_scales+=1
    if scale == "default":
        flatjsonlist.append("^%(scale)s"%vars())
        flatjsonlistdysig.append("^%(scale)s"%vars())
    else:
        n_scales+=1
        flatjsonlist.append("^%(scale)s_hi^%(scale)s_lo"%vars())
        flatjsonlistdysig.append("^%(scale)s_hi^%(scale)s_lo"%vars())

CONFIG='scripts/config_vlq_2018.json'
if options.config != '': CONFIG = options.config

n_channels=1
output_folder = ""
svfit_folder = ""
svfit_mode = 0
with open(CONFIG,"r") as input:
    with open ("config_for_python_channels.json","w") as output:
        for line in input:
            if not '//' in line:
                output.write(line)
        output.close()
    input.close()

n_channels=4
with open("config_for_python_channels.json") as config_file:
    cfg = json.load(config_file)
    n_channels=len(cfg["job"]["channels"])
    output_folder = cfg["sequence"]["output_folder"]
    svfit_mode = cfg["sequence"]["new_svfit_mode"]
    svfit_folder = cfg["sequence"]["svfit_folder"]

# makes sure output folder(s) (and svfit folder(s) if needed) is always created
os.system("bash scripts/make_output_folder.sh {}".format(output_folder))
if svfit_mode == 1:
    os.system("bash scripts/make_output_folder.sh {}".format(svfit_folder))


scale = int(math.ceil(float(n_scales*n_channels)/30))
#scale = int(math.ceil(float(n_scales*n_channels)/8)) # change back later!
if scale < 1: scale = 1

total = float(len(flatjsonlistdysig))
flatjsons = []
# this makes sure the JES's are submitted as seperate jobs (solves memory issues)
#for i in flatjsonlistdysig:
#  if 'scale_j' in i and 'hf' not in i and 'cent' not in i and 'full' not in i and 'relbal' not in i:
#    flatjsons.append('job:sequences:all:'+i)
#    flatjsonlistdysig.remove(i)
#    scale = int(math.ceil(float((n_scales-2)*n_channels)/50))
#    if scale < 1: scale = 1
if options.jetmetuncerts:
# when we do the jet met uncertainties we do not want to run additional systematics in the same job 
  for i in flatjsonlistdysig:
    if 'default' in i:
      flatjsons.append('job:sequences:all:'+i)
      flatjsonlistdysig.remove(i)
# split into seperate jobs if number of scales is over a value
for i in range(0,scale):
   first = i*int(math.ceil(total/scale))
   last = (i+1)*int(math.ceil(total/scale))
   temp=''.join(flatjsonlistdysig[first:last])
   if temp == '': continue
   temp='job:sequences:all:'+temp
   flatjsons.append(temp)


FILELIST='filelists/Sep18_2018_MC_102X'

signal_mc = [ ]
signal_vh = [ ]
signal_mc_ww = [ ]

if os.path.isfile("./jobs/files_per_sample_2018.txt"):
    os.system("mv ./jobs/files_per_sample_2018.txt ./jobs/files_per_sample_2018-%(BACKUPNAME)s.txt"%vars())

file_persamp = open("./jobs/files_per_sample_2018.txt", "w")


if options.proc_sm or options.proc_all:
    signal_mc += [
      'GluGluHToWWTo2L2Nu_M-125',
      'VBFHToWWTo2L2Nu_M-125',
      'GluGluZH_HToWW',
      'HZJ_HToWW',
      'HWminusJ_HToWW',
      'HWplusJ_HToWW',
      'GluGluHToTauTau_M-125',
      'VBFHToTauTau_M-125-ext1',
      'WplusHToTauTau_M-125',
      'WminusHToTauTau_M-125',
      'ZHToTauTau_M-125',
      'ttHToTauTau',
    ]

if options.proc_vlq_old or options.proc_all:
  signal_mc += [
    'VectorLQToTauTau_betaRd33_0_mU2_gU1','VectorLQToTauTau_betaRd33_0_mU2_gU2','VectorLQToTauTau_betaRd33_0_mU2_gU3',
    'VectorLQToTauTau_betaRd33_0_mU3_gU1','VectorLQToTauTau_betaRd33_0_mU3_gU2','VectorLQToTauTau_betaRd33_0_mU3_gU3',
    'VectorLQToTauTau_betaRd33_0_mU4_gU1','VectorLQToTauTau_betaRd33_0_mU4_gU2','VectorLQToTauTau_betaRd33_0_mU4_gU3',
    'VectorLQToTauTau_betaRd33_minus1_mU2_gU1','VectorLQToTauTau_betaRd33_minus1_mU2_gU2','VectorLQToTauTau_betaRd33_minus1_mU2_gU3',
    'VectorLQToTauTau_betaRd33_minus1_mU3_gU1','VectorLQToTauTau_betaRd33_minus1_mU3_gU2','VectorLQToTauTau_betaRd33_minus1_mU3_gU3',
    'VectorLQToTauTau_betaRd33_minus1_mU4_gU1','VectorLQToTauTau_betaRd33_minus1_mU4_gU2','VectorLQToTauTau_betaRd33_minus1_mU4_gU3',
  ]

if options.proc_vlq_matched or options.proc_all:
  signal_mc += [
    'VectorLQToTauTau_betaRd33_0_mU500_gU1_matched',
    'VectorLQToTauTau_betaRd33_0_mU1000_gU1_matched',
    'VectorLQToTauTau_betaRd33_0_mU2000_gU1_matched',
    'VectorLQToTauTau_betaRd33_0_mU3000_gU1_matched',
    'VectorLQToTauTau_betaRd33_0_mU4000_gU1_matched',
    'VectorLQToTauTau_betaRd33_0_mU5000_gU1_matched',
    'VectorLQToTauTau_betaRd33_minus1_mU500_gU1_matched',
    'VectorLQToTauTau_betaRd33_minus1_mU1000_gU1_matched',
    'VectorLQToTauTau_betaRd33_minus1_mU2000_gU1_matched',
    'VectorLQToTauTau_betaRd33_minus1_mU3000_gU1_matched',
    'VectorLQToTauTau_betaRd33_minus1_mU4000_gU1_matched',
    'VectorLQToTauTau_betaRd33_minus1_mU5000_gU1_matched',
    'VectorLQToTauTau_betaRd33_0_mU1000_gU1',
    'VectorLQToTauTau_betaRd33_0_mU1000_gU1_matched_xqcut_up',
    'VectorLQToTauTau_betaRd33_0_mU1000_gU1_matched_xqcut_down',
  ]

if options.proc_vlq_interference or options.proc_all:
  signal_mc += [
    'VectorLQToTauTau_betaRd33_0_lowM_mU500_gU1_matched_interference',
    'VectorLQToTauTau_betaRd33_0_lowM_mU1000_gU1_matched_interference',
    'VectorLQToTauTau_betaRd33_0_lowM_mU2000_gU1_matched_interference',
    'VectorLQToTauTau_betaRd33_0_lowM_mU3000_gU1_matched_interference',
    'VectorLQToTauTau_betaRd33_0_lowM_mU4000_gU1_matched_interference',
    'VectorLQToTauTau_betaRd33_0_lowM_mU5000_gU1_matched_interference',
    'VectorLQToTauTau_betaRd33_minus1_lowM_mU500_gU1_matched_interference',
    'VectorLQToTauTau_betaRd33_minus1_lowM_mU1000_gU1_matched_interference',
    'VectorLQToTauTau_betaRd33_minus1_lowM_mU2000_gU1_matched_interference',
    'VectorLQToTauTau_betaRd33_minus1_lowM_mU3000_gU1_matched_interference',
    'VectorLQToTauTau_betaRd33_minus1_lowM_mU4000_gU1_matched_interference',
    'VectorLQToTauTau_betaRd33_minus1_lowM_mU5000_gU1_matched_interference',
    'VectorLQToTauTau_betaRd33_0_highM_mU500_gU1_matched_interference',
    'VectorLQToTauTau_betaRd33_0_highM_mU1000_gU1_matched_interference',
    'VectorLQToTauTau_betaRd33_0_highM_mU2000_gU1_matched_interference',
    'VectorLQToTauTau_betaRd33_0_highM_mU3000_gU1_matched_interference',
    'VectorLQToTauTau_betaRd33_0_highM_mU4000_gU1_matched_interference',
    'VectorLQToTauTau_betaRd33_0_highM_mU5000_gU1_matched_interference',
    'VectorLQToTauTau_betaRd33_minus1_highM_mU500_gU1_matched_interference',
    'VectorLQToTauTau_betaRd33_minus1_highM_mU1000_gU1_matched_interference',
    'VectorLQToTauTau_betaRd33_minus1_highM_mU2000_gU1_matched_interference',
    'VectorLQToTauTau_betaRd33_minus1_highM_mU3000_gU1_matched_interference',
    'VectorLQToTauTau_betaRd33_minus1_highM_mU4000_gU1_matched_interference',
    'VectorLQToTauTau_betaRd33_minus1_highM_mU5000_gU1_matched_interference',
    'VectorLQToTauTau_betaRd33_0_lowM_mU1000_gU1_interference',
    'VectorLQToTauTau_betaRd33_0_lowM_mU1000_gU1_matched_xqcut_up_interference',
    'VectorLQToTauTau_betaRd33_0_lowM_mU1000_gU1_matched_xqcut_down_interference',
    'VectorLQToTauTau_betaRd33_0_highM_mU1000_gU1_interference',
    'VectorLQToTauTau_betaRd33_0_highM_mU1000_gU1_matched_xqcut_up_interference',
    'VectorLQToTauTau_betaRd33_0_highM_mU1000_gU1_matched_xqcut_down_interference',
  ]


if options.proc_vlq_exo or options.proc_all:
  signal_mc += [
    #'exo_vlq_sample',
    'exo_slq_sample'
  ]

if options.proc_data or options.proc_all or options.calc_lumi or options.proc_embed:
    if not no_json:
        with open(CONFIG,"r") as input:
            with open ("config_for_python.json","w") as output:
                for line in input:
                    if not '//' in line:
                        output.write(line)
                output.close()
            input.close()

        with open("config_for_python.json") as config_file:
            cfg = json.load(config_file)

        channels=cfg["job"]["channels"]
    else:
        channels=['mt','et','tt','em','zmm','zee']

if options.proc_data or options.proc_all or options.calc_lumi:

    data_samples = []
    data_eras = ['A','B','C','D']
    for chn in channels:
        for era in data_eras:
            if 'mt' in chn or 'zmm' in chn:
                data_samples+=['SingleMuon'+era]
            if 'et' in chn or 'zee' in chn:
                data_samples+=['EGamma'+era]
            if 'em' in chn:
                data_samples+=['MuonEG'+era]
            if 'tt' in chn:
                data_samples+=['Tau'+era]
    if ('mt' in channels or 'et' in channels) and 'tt' not in channels:
      for era in data_eras:
        data_samples+=['Tau'+era]


    DATAFILELIST="./filelists/Sep18_2018_Data_102X"

    if options.calc_lumi:
        for sa in data_samples:
            JOB='%s_2018' % (sa)
            JSONPATCH= (r"'{\"job\":{\"filelist\":\"%(DATAFILELIST)s_%(sa)s.dat\",\"file_prefix\":\"root://gfe02.grid.hep.ph.ic.ac.uk:1097//store/user/dwinterb/Aug22_Data_102X/\",\"sequences\":{\"em\":[],\"et\":[],\"mt\":[],\"tt\":[]}}, \"sequence\":{\"output_name\":\"%(JOB)s\",\"is_data\":true,\"lumi_mask_only\":true}}' "%vars());
            nfiles = sum(1 for line in open('%(DATAFILELIST)s_%(sa)s.dat' % vars()))
            nperjob = 500 
            if "TauC" in sa: nperjob = 252
            elif "TauD" in sa: nperjob = 271
            for i in range (0,int(math.ceil(float(nfiles)/float(nperjob)))):
                os.system('%(JOBWRAPPER)s "./bin/HTT --cfg=%(CONFIG)s --json=%(JSONPATCH)s --offset=%(i)d --nlines=%(nperjob)d &> jobs/%(JOB)s-%(i)d.log" jobs/%(JOB)s-%(i)s.sh' %vars())
                if not parajobs:
                    os.system('%(JOBSUBMIT)s jobs/%(JOB)s-%(i)d.sh' % vars())
            if parajobs:
                os.system('%(JOBWRAPPER)s ./jobs/%(JOB)s-\$\(\(SGE_TASK_ID-1\)\).sh  jobs/parajob_%(JOB)s.sh' %vars())
                PARAJOBSUBMIT = getParaJobSubmit(int(math.ceil(float(nfiles)/float(nperjob))))
                os.system('%(PARAJOBSUBMIT)s jobs/parajob_%(JOB)s.sh' % vars())
            file_persamp.write("%s %d\n" %(JOB, int(math.ceil(float(nfiles)/float(nperjob)))))

    else:
        for sa in data_samples:
            DATAFILELIST="./filelists/Sep18_2018_Data_102X"
            JOB='%s_2018' % (sa)
            JSONPATCH= (r"'{\"job\":{\"filelist\":\"%(DATAFILELIST)s_%(sa)s.dat\",\"file_prefix\":\"root://gfe02.grid.hep.ph.ic.ac.uk:1097//store/user/guttley/Sep18_Data_102X_2018/\",\"sequences\":{\"em\":[],\"et\":[],\"mt\":[],\"tt\":[],\"zmm\":[],\"zee\":[]}}, \"sequence\":{\"output_name\":\"%(JOB)s\",\"is_data\":true}}' "%vars());
            nfiles = sum(1 for line in open('%(DATAFILELIST)s_%(sa)s.dat' % vars()))
            nperjob = 30
      
            if "EGammaD" in sa: nperjob = 50
            # elif "TauD" in sa: nperjob = 45
            
            for i in range (0,int(math.ceil(float(nfiles)/float(nperjob)))) :  
                os.system('%(JOBWRAPPER)s "./bin/HTT --cfg=%(CONFIG)s --json=%(JSONPATCH)s --offset=%(i)d --nlines=%(nperjob)d &> jobs/%(JOB)s-%(i)d.log" jobs/%(JOB)s-%(i)s.sh' %vars())
                if not parajobs and not options.condor:
                    os.system('%(JOBSUBMIT)s jobs/%(JOB)s-%(i)d.sh' % vars())
                elif not parajobs and options.condor:
                    outscriptname = '{}-{}.sh'.format(JOB, i)
                    subfilename = '{}_{}.sub'.format(JOB, i)
                    subfile = open("jobs/{}".format(subfilename), "w")
                    condor_settings = CONDOR_TEMPLATE % { 
                      'EXE': outscriptname,
                      'TASK': "{}-{}".format(JOB, i)
                    }
                    subfile.write(condor_settings)
                    subfile.close()
                    os.system('condor_submit jobs/{}'.format(subfilename))

            if parajobs:
                os.system('%(JOBWRAPPER)s ./jobs/%(JOB)s-\$\(\(SGE_TASK_ID-1\)\).sh  jobs/parajob_%(JOB)s.sh' %vars())
                PARAJOBSUBMIT = getParaJobSubmit(int(math.ceil(float(nfiles)/float(nperjob))))
                os.system('%(PARAJOBSUBMIT)s jobs/parajob_%(JOB)s.sh' % vars())
            file_persamp.write("%s %d\n" %(JOB, int(math.ceil(float(nfiles)/float(nperjob)))))

if options.proc_embed or options.proc_all:

    embed_samples = []
    data_eras = ['A','B','C','D']
    #data_eras = ['D']
    for chn in channels:
        for era in data_eras:
            if 'em' in chn:
                embed_samples+=['EmbeddingElMu'+era]
            if 'et' in chn:
                embed_samples+=['EmbeddingElTau'+era]
            if 'mt' in chn:
                embed_samples+=['EmbeddingMuTau'+era]
            if 'tt' in chn:
                embed_samples+=['EmbeddingTauTau'+era]
            if 'zmm' in chn:
                embed_samples+=['EmbeddingMuMu'+era]
            if 'zee' in chn:
                embed_samples+=['EmbeddingElEl'+era]

    EMBEDFILELIST="./filelists/Sep18_2018_MC_102X"

    for sa in embed_samples:
        job_num=0
        JOB='%s_2018' % (sa)
        JSONPATCH= (r"'{\"job\":{\"filelist\":\"%(EMBEDFILELIST)s_%(sa)s.dat\",\"file_prefix\":\"root://gfe02.grid.hep.ph.ic.ac.uk:1097//store/user/guttley/Sep18_MC_102X_2018/\",\"sequences\":{\"em\":[],\"et\":[],\"mt\":[],\"tt\":[],\"zmm\":[],\"zee\":[]}}, \"sequence\":{\"output_name\":\"%(JOB)s\",\"is_embedded\":true,%(jetuncert_string)s}}' "%vars());
        for FLATJSONPATCH in flatjsons:
            nperjob = 20
            #if 'ElTau' in sa: nperjob = 10
            if 'ElTauD' in sa: nperjob = 100
            if 'MuMu' in sa and 'MuMuD' not in sa: nperjob = 10
            #print FLATJSONPATCH
            FLATJSONPATCH = FLATJSONPATCH.replace('^scale_j_hi^scale_j_lo','').replace('^scale_j_hf_hi^scale_j_hf_lo','').replace('^scale_j_cent_hi^scale_j_cent_lo','').replace('^scale_j_full_hi^scale_j_full_lo','').replace('^scale_j_relbal_hi^scale_j_relbal_lo','').replace('^scale_j_relsamp_hi^scale_j_relsamp_lo','').replace('^scale_j_relbal_hi^scale_j_relbal_lo','').replace('^scale_j_abs_hi^scale_j_abs_lo','').replace('^scale_j_abs_year_hi^scale_j_abs_year_lo','').replace('^scale_j_flav_hi^scale_j_flav_lo','').replace('^scale_j_bbec1_hi^scale_j_bbec1_lo','').replace('^scale_j_bbec1_year_hi^scale_j_bbec1_year_lo','').replace('^scale_j_ec2_hi^scale_j_ec2_lo','').replace('^scale_j_ec2_year_hi^scale_j_ec2_year_lo','').replace('^scale_j_hf_hi^scale_j_hf_lo','').replace('^scale_j_hf_year_hi^scale_j_hf_year_lo','').replace('^scale_j_relsamp_year_hi^scale_j_relsamp_year_lo','').replace('^res_j_hi^res_j_lo','')
   
            FLATJSONPATCH = FLATJSONPATCH.replace('^scale_efake_0pi_hi^scale_efake_0pi_lo','').replace('^scale_efake_1pi_hi^scale_efake_1pi_lo','').replace('^scale_mufake_0pi_hi^scale_mufake_0pi_lo','').replace('^scale_mufake_1pi_hi^scale_mufake_1pi_lo','').replace('^met_cl_hi^met_cl_lo','').replace('^met_uncl_hi^met_uncl_lo','').replace('^scale_met_hi^scale_met_lo','').replace('^res_met_hi^res_met_lo','').replace('^scale_met_njets0_hi^scale_met_njets0_lo','').replace('^res_met_njets0_hi^res_met_njets0_lo','').replace('^scale_met_njets1_hi^scale_met_njets1_lo','').replace('^res_met_njets1_hi^res_met_njets1_lo','').replace('^scale_met_njets2_hi^scale_met_njets2_lo','').replace('^res_met_njets2_hi^res_met_njets2_lo','')
            if 'TauTau' in  sa: FLATJSONPATCH = FLATJSONPATCH.replace('^scale_e_hi^scale_e_lo','').replace('^scale_mu_hi^scale_mu_lo','').replace('^scale_t_hi^scale_t_lo','')
            if 'ElMu' in  sa: FLATJSONPATCH = FLATJSONPATCH.replace('^scale_t_0pi_hi^scale_t_0pi_lo','').replace('^scale_t_1pi_hi^scale_t_1pi_lo','').replace('^scale_t_3prong_hi^scale_t_3prong_lo','').replace('^scale_t_3prong1pi0_hi^scale_t_3prong1pi0_lo','')
            if 'MuTau' in  sa: FLATJSONPATCH = FLATJSONPATCH.replace('^scale_e_hi^scale_e_lo','').replace('^scale_t_hi^scale_t_lo','')
            if 'ElTau' in  sa: FLATJSONPATCH = FLATJSONPATCH.replace('^scale_mu_hi^scale_mu_lo','').replace('^scale_t_hi^scale_t_lo','')
            if FLATJSONPATCH == 'job:sequences:all:^^' or FLATJSONPATCH == 'job:sequences:all:': continue

            n_scales = FLATJSONPATCH.count('_lo')*2 + FLATJSONPATCH.count('default')

            if options.jetmetuncerts and 'default' in FLATJSONPATCH: n_scales +=2

            if n_scales*n_channels>=24: nperjob = 10
            if n_scales*n_channels>=48: nperjob=5
            if 'MuTauD' in sa or 'TauTauD' in sa:
              nperjob = 300
              if n_scales*n_channels>=28: nperjob = 150
              if n_scales*n_channels>=56: nperjob=75
            if 'MuTau' in sa: nperjob = int(math.ceil(float(nperjob)/10))  
            if 'ElTau' in sa and 'ElTauD' not in sa: nperjob = int(math.ceil(float(nperjob)/5))  
#            nperjob = int(math.ceil(float(nperjob)/max(1.,float(n_scales-8)*float(n_channels)/10.)))

            nfiles = sum(1 for line in open('%(EMBEDFILELIST)s_%(sa)s.dat' % vars()))
            for i in range (0,int(math.ceil(float(nfiles)/float(nperjob)))) :
                os.system('%(JOBWRAPPER)s "./bin/HTT --cfg=%(CONFIG)s --json=%(JSONPATCH)s --flatjson=%(FLATJSONPATCH)s --offset=%(i)d --nlines=%(nperjob)d &> jobs/%(JOB)s-%(job_num)d.log" jobs/%(JOB)s-%(job_num)s.sh' %vars())
                if not parajobs and not options.condor:
                    os.system('%(JOBSUBMIT)s jobs/%(JOB)s-%(job_num)d.sh' % vars())
                elif not parajobs and options.condor:
                    outscriptname = '{}-{}.sh'.format(JOB, job_num)
                    subfilename = '{}_{}.sub'.format(JOB, job_num)
                    subfile = open("jobs/{}".format(subfilename), "w")
                    condor_settings = CONDOR_TEMPLATE % { 
                      'EXE': outscriptname,
                      'TASK': "{}-{}".format(JOB, job_num)
                    }
                    subfile.write(condor_settings)
                    subfile.close()
                    os.system('condor_submit jobs/{}'.format(subfilename))

                job_num+=1
            file_persamp.write("%s %d\n" %(JOB, int(math.ceil(float(nfiles)/float(nperjob)))))
        if parajobs:
            os.system('%(JOBWRAPPER)s ./jobs/%(JOB)s-\$\(\(SGE_TASK_ID-1\)\).sh  jobs/parajob_%(JOB)s.sh' %vars())
            PARAJOBSUBMIT = getParaJobSubmit(job_num)
            os.system('%(PARAJOBSUBMIT)s jobs/parajob_%(JOB)s.sh' % vars())


if options.proc_bkg or options.proc_all:
    central_samples = [
         'DYJetsToLL',
         'DY1JetsToLL-LO',
         'DY2JetsToLL-LO',
         'DY3JetsToLL-LO',
         'DY4JetsToLL-LO',
         'DYJetsToLL-LO',
         'DYJetsToLL_M-10-50-LO',
         'EWKWMinus2Jets',
         'EWKWPlus2Jets',
         'EWKZ2Jets',
         'T-t',
         'T-tW-ext1',
         'TTTo2L2Nu',
         'TTToHadronic',
         'TTToSemiLeptonic',
         'Tbar-t',
         'Tbar-tW-ext1',
         'W1JetsToLNu-LO',
         'W2JetsToLNu-LO',
         'W3JetsToLNu-LO',
         'W4JetsToLNu-LO',
         'WGToLNuG',
         'WJetsToLNu-LO',
         'WWTo2L2Nu',
         'WWToLNuQQ',
         'WZTo1L3Nu',
         'WZTo2L2Q',
         'WZTo3LNu',
         'WZTo3LNu-ext1',
         'ZZTo2L2Nu-ext1',
         'ZZTo2L2Nu-ext2',
         'ZZTo2L2Q',
         'ZZTo4L',
         'ZZTo4L-ext',
    ]


    for sa in central_samples:
        JOB='%s_2018' % (sa)
        JSONPATCH= (r"'{\"job\":{\"filelist\":\"%(FILELIST)s_%(sa)s.dat\", \"file_prefix\":\"root://gfe02.grid.hep.ph.ic.ac.uk:1097//store/user/guttley/Sep18_MC_102X_2018/\"}, \"sequence\":{\"output_name\":\"%(JOB)s\",%(jetuncert_string)s}}' "%vars());

        job_num=0
        for FLATJSONPATCH in flatjsons:
            #nperjob = 40
            nperjob=20
            if 'DY' not in sa and 'EWKZ' not in sa:
                FLATJSONPATCH = FLATJSONPATCH.replace('^scale_efake_0pi_hi^scale_efake_0pi_lo','').replace('^scale_efake_1pi_hi^scale_efake_1pi_lo','').replace('^scale_mufake_0pi_hi^scale_mufake_0pi_lo','').replace('^scale_mufake_1pi_hi^scale_mufake_1pi_lo','')
            if 'DY' not in sa and 'JetsToLNu' not in sa and 'WG' not in sa and 'EWKZ' not in sa and 'EWKW' not in sa:
                FLATJSONPATCH = FLATJSONPATCH.replace('^scale_met_hi^scale_met_lo','').replace('^res_met_hi^res_met_lo','').replace('^scale_met_njets0_hi^scale_met_njets0_lo','').replace('^res_met_njets0_hi^res_met_njets0_lo','').replace('^scale_met_njets1_hi^scale_met_njets1_lo','').replace('^res_met_njets1_hi^res_met_njets1_lo','').replace('^scale_met_njets2_hi^scale_met_njets2_lo','').replace('^res_met_njets2_hi^res_met_njets2_lo','')
            #else:
            #    FLATJSONPATCH = FLATJSONPATCH.replace('^met_uncl_hi^met_uncl_lo','')
            if FLATJSONPATCH == 'job:sequences:all:^^' or FLATJSONPATCH == 'job:sequences:all:': continue
            n_scales = FLATJSONPATCH.count('_lo')*2 + FLATJSONPATCH.count('default')
            if n_scales*n_channels>=24: nperjob = 10
            if n_scales*n_channels>=48: nperjob=5

            if options.jetmetuncerts and 'default' in FLATJSONPATCH: nperjob = int(math.ceil(float(nperjob)/2))

            #if 'TTTo' in sa: nperjob = int(math.ceil(float(nperjob)/2)) 
            if 'TTTo' in sa: nperjob = int(math.ceil(float(nperjob)/2)) 
            #nperjob = int(math.ceil(float(nperjob)/max(1.,float(n_scales)*float(n_channels)/10.)))
            nfiles = sum(1 for line in open('%(FILELIST)s_%(sa)s.dat' % vars()))
            for i in range (0,int(math.ceil(float(nfiles)/float(nperjob)))) :
                os.system('%(JOBWRAPPER)s "./bin/HTT --cfg=%(CONFIG)s --json=%(JSONPATCH)s --flatjson=%(FLATJSONPATCH)s --offset=%(i)d --nlines=%(nperjob)d &> jobs/%(JOB)s-%(job_num)d.log" jobs/%(JOB)s-%(job_num)s.sh' %vars())
                if not parajobs and not options.condor:
                    os.system('%(JOBSUBMIT)s jobs/%(JOB)s-%(job_num)d.sh' % vars())
                elif not parajobs and options.condor:
                    outscriptname = '{}-{}.sh'.format(JOB, job_num)
                    subfilename = '{}_{}.sub'.format(JOB, job_num)
                    subfile = open("jobs/{}".format(subfilename), "w")
                    condor_settings = CONDOR_TEMPLATE % {
                      'EXE': outscriptname,
                      'TASK': "{}-{}".format(JOB, job_num)
                    }
                    subfile.write(condor_settings)
                    subfile.close()
                    os.system('condor_submit jobs/{}'.format(subfilename))

                job_num+=1
            file_persamp.write("%s %d\n" %(JOB, int(math.ceil(float(nfiles)/float(nperjob)))))
        if parajobs:
            os.system('%(JOBWRAPPER)s ./jobs/%(JOB)s-\$\(\(SGE_TASK_ID-1\)\).sh  jobs/parajob_%(JOB)s.sh' %vars())
            PARAJOBSUBMIT = getParaJobSubmit(job_num)
            os.system('%(PARAJOBSUBMIT)s jobs/parajob_%(JOB)s.sh' % vars())

if options.mg_signal or options.proc_sm or options.proc_vlq_old or options.proc_vlq_matched or options.proc_vlq_interference or options.proc_vlq_exo or options.proc_all:
    SIG_FILELIST = FILELIST
    for sa in signal_mc:
        SIG_FILELIST = FILELIST
        SIG_DIR = 'Sep18_MC_102X_2018'

        if 'VectorLQ' in sa and 'interference' in sa:
          SIG_DIR = 'Aug25_MC_102X_2018'
          SIG_FILELIST ="./filelists/Aug25_2018_MC_102X"
          user='guttley'
        elif 'VectorLQ' in sa and 'matched' in sa or sa.endswith("mU1000_gU1"):
          SIG_DIR = 'Aug17_MC_102X_2018'
          SIG_FILELIST ="./filelists/Aug17_2018_MC_102X"
          user='guttley'
        elif 'VectorLQ' in sa:
          SIG_DIR = 'Jul02_MC_102X_2018'
          SIG_FILELIST ="./filelists/Jul02_2018_MC_102X"                  
          user='guttley'
        elif 'exo_vlq' in sa:
          SIG_DIR = 'exo_vlq_sample' 
          SIG_FILELIST ="./filelists/" 
          user = 'guttley'
        elif 'exo_slq' in sa:
          SIG_DIR = 'scalarLQ_ntuple'
          SIG_FILELIST ="./filelists/"
          user = 'guttley'
        else:
          SIG_DIR = 'Sep18_MC_102X_2018'
          SIG_FILELIST = FILELIST
          user='guttley'

        JOB='%s_2018' % (sa)
        JSONPATCH= (r"'{\"job\":{\"filelist\":\"%(SIG_FILELIST)s_%(sa)s.dat\",\"file_prefix\":\"root://gfe02.grid.hep.ph.ic.ac.uk:1097//store/user/%(user)s/%(SIG_DIR)s/\"}, \"sequence\":{\"output_name\":\"%(JOB)s\",%(jetuncert_string)s}}' "%vars());
        if ("HToTauTau" in sa and "amcatnloFXFX" in sa) or 'nospinner' in sa:
            JSONPATCH= (r"'{\"job\":{\"filelist\":\"%(SIG_FILELIST)s_%(sa)s.dat\",\"file_prefix\":\"root://gfe02.grid.hep.ph.ic.ac.uk:1097//store/user/%(user)s/%(SIG_DIR)s/\"}, \"sequence\":{\"output_name\":\"%(JOB)s\"}}' "%vars());
            # use input/pileup/2018/pileup_2018_DYJetsToLL-2017.root in this case
            # for the 2017 MG ggH signal samples
            JSONPATCH= (r"'{\"job\":{\"filelist\":\"%(SIG_FILELIST)s_%(sa)s.dat\",\"file_prefix\":\"root://gfe02.grid.hep.ph.ic.ac.uk:1097//store/user/%(user)s/%(SIG_DIR)s/\"}, \"sequence\":{\"output_name\":\"%(JOB)s\",\"mc_pu_file\":\"input/pileup/2018/pileup_2018_DYJetsToLL-LO.root\",\"trg_in_mc\":false}}' "%vars());

        job_num=0
        for FLATJSONPATCH in flatjsons:
            FLATJSONPATCH = FLATJSONPATCH.replace('^scale_efake_0pi_hi^scale_efake_0pi_lo','').replace('^scale_efake_1pi_hi^scale_efake_1pi_lo','').replace('^scale_mufake_0pi_hi^scale_mufake_0pi_lo','').replace('^scale_mufake_1pi_hi^scale_mufake_1pi_lo','')
            #FLATJSONPATCH = FLATJSONPATCH.replace('^met_uncl_hi^met_uncl_lo','')
            print sa
            if FLATJSONPATCH == 'job:sequences:all:^^' or FLATJSONPATCH == 'job:sequences:all:': continue
            if os.path.exists('%(SIG_FILELIST)s_%(sa)s.dat' %vars()):
                nfiles = sum(1 for line in open('%(SIG_FILELIST)s_%(sa)s.dat' % vars()))
                nperjob = 20
                n_scales = FLATJSONPATCH.count('_lo')*2 + FLATJSONPATCH.count('default')
                if n_scales*n_channels>=24: nperjob = 10
                if n_scales*n_channels>=48: nperjob=5

                if ('JJH' in sa and 'ToTauTau' in sa) or 'Filtered' in sa: 
                  nperjob = int(math.ceil(float(nperjob)/5)) 
                if "exo" in JOB: nperjob=80
                if "VectorLQ" in JOB: nperjob=50
                if options.jetmetuncerts and 'default' in FLATJSONPATCH: nperjob = int(math.ceil(float(nperjob)/2))
 
                #if ('MG' in sa or 'Maxmix' in sa or 'Pseudoscalar' in sa) and 'GEN' not in sa: nperjob = 10
                for i in range (0,int(math.ceil(float(nfiles)/float(nperjob)))) :
                    os.system('%(JOBWRAPPER)s "./bin/HTT --cfg=%(CONFIG)s --json=%(JSONPATCH)s --flatjson=%(FLATJSONPATCH)s --offset=%(i)d --nlines=%(nperjob)d &> jobs/%(JOB)s-%(job_num)d.log" jobs/%(JOB)s-%(job_num)s.sh' %vars())
                    if not parajobs and not options.condor:
                        os.system('%(JOBSUBMIT)s jobs/%(JOB)s-%(job_num)d.sh' % vars())
                    elif not parajobs and options.condor:
                        outscriptname = '{}-{}.sh'.format(JOB, job_num)
                        subfilename = '{}_{}.sub'.format(JOB, job_num)
                        subfile = open("jobs/{}".format(subfilename), "w")
                        condor_settings = CONDOR_TEMPLATE % {
                          'EXE': outscriptname,
                          'TASK': "{}-{}".format(JOB, job_num)
                        }
                        subfile.write(condor_settings)
                        subfile.close()
                        os.system('condor_submit jobs/{}'.format(subfilename))
                        # print('condor_submit jobs/{}'.format(subfilename))
                    job_num+=1
                file_persamp.write("%s %d\n" %(JOB, int(math.ceil(float(nfiles)/float(nperjob)))))
        if parajobs:
            os.system('%(JOBWRAPPER)s ./jobs/%(JOB)s-\$\(\(SGE_TASK_ID-1\)\).sh  jobs/parajob_%(JOB)s.sh' %vars())
            PARAJOBSUBMIT = getParaJobSubmit(job_num)
            os.system('%(PARAJOBSUBMIT)s jobs/parajob_%(JOB)s.sh' % vars())

