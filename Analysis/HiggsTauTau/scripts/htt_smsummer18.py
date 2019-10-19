#!/usr/bin/env python

import sys
from optparse import OptionParser
import os
import math
import json

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
+JobFlavour     = "espresso"
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

(options, args) = parser.parse_args()
if options.wrapper: JOBWRAPPER=options.wrapper
if options.submit:  JOBSUBMIT=options.submit
if options.condor: JOBWRAPPER = "./scripts/generate_condor_job.sh"

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

CONFIG='scripts/config2018cpdecay.json'
if options.config != '': CONFIG = options.config

n_channels=1
with open(CONFIG,"r") as input:
    with open ("config_for_python_channels.json","w") as output:
        for line in input:
            if not '//' in line:
                output.write(line)
        output.close()
    input.close()

with open("config_for_python_channels.json") as config_file:
    cfg = json.load(config_file)
    n_channels=len(cfg["job"]["channels"])

scale = int(math.ceil(float(n_scales*n_channels)/100))
if scale < 1: scale = 1

total = float(len(flatjsonlistdysig))
flatjsons = []
# this makes sure the JES's are submitted as seperate jobs (solves memory issues)
for i in flatjsonlistdysig:
    if 'scale_j' in i and 'hf' not in i and 'cent' not in i and 'full' not in i and 'relbal' not in i:
        flatjsons.append('job:sequences:all:'+i)
        flatjsonlistdysig.remove(i)
        scale = int(math.ceil(float((n_scales-2)*n_channels)/100))
        if scale < 1: scale = 1
# split into seperate jobs if number of scales is over a value
for i in range(0,scale):
    first = i*int(math.ceil(total/scale))
    last = (i+1)*int(math.ceil(total/scale))
    temp=''.join(flatjsonlistdysig[first:last])
    if temp == '': continue
    temp='job:sequences:all:'+temp
    flatjsons.append(temp)

FILELIST='filelists/Oct07_MC_102X'

signal_mc = [ ]
signal_vh = [ ]
signal_mc_ww = [ ]

if os.path.isfile("./jobs/files_per_sample.txt"):
    os.system("mv ./jobs/files_per_sample.txt ./jobs/files_per_sample-%(BACKUPNAME)s.txt"%vars())

file_persamp = open("./jobs/files_per_sample.txt", "w")


if options.proc_sm or options.proc_all:
    signal_mc += [

      'VBFHToTauTau_M-126-nospinner',
      'VBFHToTauTau_M-125-ext1',
      'VBFHToTauTau_M-125-MM-filter',
      'VBFHToTauTau_M-125-nospinner-filter',
      'VBFHToTauTau_M-125-PS-filter',
      'VBFHToTauTau_M-125-SM-filter',
      'GluGluToHToTauTau_M-125-nospinner',
      'GluGluHToTauTau_M-125',
      'GluGluToHToTauTau_M-125-nospinner-filter',
      'GluGluToHToTauTau_M125_amcatnloFXFX',
      'GluGluToHToTauTauPlusTwoJets_M125_amcatnloFXFX',
      'GluGluToMaxmixHToTauTau_M125_amcatnloFXFX',
      'GluGluToMaxmixHToTauTauPlusTwoJets_M125_amcatnloFXFX',
      'GluGluToPseudoscalarHToTauTau_M125_amcatnloFXFX',
      'GluGluToPseudoscalarHToTauTauPlusTwoJets_M125_amcatnloFXFX',
      'WplusHToTauTau_M-125', 
      'WminusHToTauTau_M-125', 
      'ZHToTauTau_M-125'

        ]
   # signal_mc += [
   #     'GluGluToHToTauTau_M125_amcatnloFXFX-UEUp',
   #     'GluGluToHToTauTau_M125_amcatnloFXFX-UEDown',
   #     'GluGluToHToTauTau_M125_amcatnloFXFX-PSUp',
   #     'GluGluToHToTauTau_M125_amcatnloFXFX-PSDown'
   # ]


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
#    data_eras = ['A','B','C','D']
    data_eras=['D']
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

    DATAFILELIST="./filelists/Oct07_Data_102X"

    if options.calc_lumi:
        for sa in data_samples:
            JOB='%s_2018' % (sa)
            JSONPATCH= (r"'{\"job\":{\"filelist\":\"%(DATAFILELIST)s_%(sa)s.dat\",\"file_prefix\":\"root://gfe02.grid.hep.ph.ic.ac.uk:1097//store/user/dwinterb/Aug22_Data_102X_2018/\",\"sequences\":{\"em\":[],\"et\":[],\"mt\":[],\"tt\":[]}}, \"sequence\":{\"output_name\":\"%(JOB)s\",\"is_data\":true,\"lumi_mask_only\":true}}' "%vars());
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
            DATAFILELIST="./filelists/Oct07_Data_102X"
            JOB='%s_2018' % (sa)
            JSONPATCH= (r"'{\"job\":{\"filelist\":\"%(DATAFILELIST)s_%(sa)s.dat\",\"file_prefix\":\"root://gfe02.grid.hep.ph.ic.ac.uk:1097//store/user/dwinterb/Oct07_Data_102X_2018/\",\"sequences\":{\"em\":[],\"et\":[],\"mt\":[],\"tt\":[],\"zmm\":[],\"zee\":[]}}, \"sequence\":{\"output_name\":\"%(JOB)s\",\"is_data\":true}}' "%vars());
            nfiles = sum(1 for line in open('%(DATAFILELIST)s_%(sa)s.dat' % vars()))
            nperjob = 40
            # if "TauC" in sa: nperjob = 42
            # elif "TauD" in sa: nperjob = 45
            
            for i in range (0,int(math.ceil(float(nfiles)/float(nperjob)))) :  
                os.system('%(JOBWRAPPER)s "./bin/HTT --cfg=%(CONFIG)s --json=%(JSONPATCH)s --offset=%(i)d --nlines=%(nperjob)d &> jobs/%(JOB)s-%(i)d.log" jobs/%(JOB)s-%(i)s.sh' %vars())
                if not parajobs:
                    os.system('%(JOBSUBMIT)s jobs/%(JOB)s-%(i)d.sh' % vars())
            if parajobs:
                os.system('%(JOBWRAPPER)s ./jobs/%(JOB)s-\$\(\(SGE_TASK_ID-1\)\).sh  jobs/parajob_%(JOB)s.sh' %vars())
                PARAJOBSUBMIT = getParaJobSubmit(int(math.ceil(float(nfiles)/float(nperjob))))
                os.system('%(PARAJOBSUBMIT)s jobs/parajob_%(JOB)s.sh' % vars())
            file_persamp.write("%s %d\n" %(JOB, int(math.ceil(float(nfiles)/float(nperjob)))))

if options.proc_embed or options.proc_all:

    embed_samples = []
    data_eras = ['A','B','C','D']
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

    EMBEDFILELIST="./filelists/Oct07_MC_102X"

    for sa in embed_samples:
        job_num=0
        JOB='%s_2018' % (sa)
        JSONPATCH= (r"'{\"job\":{\"filelist\":\"%(EMBEDFILELIST)s_%(sa)s.dat\",\"file_prefix\":\"root://gfe02.grid.hep.ph.ic.ac.uk:1097//store/user/dwinterb/Oct07_MC_102X_2018/\",\"sequences\":{\"em\":[],\"et\":[],\"mt\":[],\"tt\":[],\"zmm\":[],\"zee\":[]}}, \"sequence\":{\"output_name\":\"%(JOB)s\",\"is_embedded\":true}}' "%vars());
        for FLATJSONPATCH in flatjsons:
            nperjob = 10
            FLATJSONPATCH = FLATJSONPATCH.replace('^scale_j_hi^scale_j_lo','').replace('^scale_j_hf_hi^scale_j_hf_lo','').replace('^scale_j_cent_hi^scale_j_cent_lo','').replace('^scale_j_full_hi^scale_j_full_lo','').replace('^scale_j_relbal_hi^scale_j_relbal_lo','').replace('^scale_j_relsamp_hi^scale_j_relsamp_lo','')

            FLATJSONPATCH = FLATJSONPATCH.replace('^scale_efake_0pi_hi^scale_efake_0pi_lo','').replace('^scale_efake_1pi_hi^scale_efake_1pi_lo','').replace('^scale_mufake_0pi_hi^scale_mufake_0pi_lo','').replace('^scale_mufake_1pi_hi^scale_mufake_1pi_lo','').replace('^met_cl_hi^met_cl_lo','').replace('^met_uncl_hi^met_uncl_lo','').replace('scale_met_hi^scale_met_lo','').replace('res_met_hi^res_met_lo','').replace('scale_met_njets0_hi^scale_met_njets0_lo','').replace('res_met_njets0_hi^res_met_njets0_lo','').replace('scale_met_njets1_hi^scale_met_njets1_lo','').replace('res_met_njets1_hi^res_met_njets1_lo','').replace('scale_met_njets2_hi^scale_met_njets2_lo','').replace('res_met_njets2_hi^res_met_njets2_lo','')
            if 'TauTau' in  sa: FLATJSONPATCH = FLATJSONPATCH.replace('^scale_e_hi^scale_e_lo','').replace('^scale_mu_hi^scale_mu_lo','').replace('^scale_t_hi^scale_t_lo','')
            if 'ElMu' in  sa: FLATJSONPATCH = FLATJSONPATCH.replace('^scale_e_hi^scale_e_lo','').replace('^scale_t_0pi_hi^scale_t_0pi_lo','').replace('^scale_t_1pi_hi^scale_t_1pi_lo','').replace('^scale_t_3prong_hi^scale_t_3prong_lo','')
            if 'MuTau' in  sa: FLATJSONPATCH = FLATJSONPATCH.replace('^scale_e_hi^scale_e_lo','').replace('^scale_t_hi^scale_t_lo','')
            if 'ElTau' in  sa: FLATJSONPATCH = FLATJSONPATCH.replace('^scale_mu_hi^scale_mu_lo','').replace('^scale_t_hi^scale_t_lo','')
            nperjob = int(math.ceil(float(nperjob)/max(1.,float(n_scales-8)*float(n_channels)/10.)))
            nfiles = sum(1 for line in open('%(EMBEDFILELIST)s_%(sa)s.dat' % vars()))
            for i in range (0,int(math.ceil(float(nfiles)/float(nperjob)))) :
                os.system('%(JOBWRAPPER)s "./bin/HTT --cfg=%(CONFIG)s --json=%(JSONPATCH)s --flatjson=%(FLATJSONPATCH)s --offset=%(i)d --nlines=%(nperjob)d &> jobs/%(JOB)s-%(job_num)d.log" jobs/%(JOB)s-%(job_num)s.sh' %vars())
                if not parajobs: os.system('%(JOBSUBMIT)s jobs/%(JOB)s-%(job_num)d.sh' % vars())
                job_num+=1
            file_persamp.write("%s %d\n" %(JOB, int(math.ceil(float(nfiles)/float(nperjob)))))
            if parajobs:
                os.system('%(JOBWRAPPER)s ./jobs/%(JOB)s-\$\(\(SGE_TASK_ID-1\)\).sh  jobs/parajob_%(JOB)s.sh' %vars())
                PARAJOBSUBMIT = getParaJobSubmit(job_num)
                os.system('%(PARAJOBSUBMIT)s jobs/parajob_%(JOB)s.sh' % vars())


if options.proc_bkg or options.proc_all:
    central_samples = [
        # 'DYJetsToLL-2017',
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
        # 'WGToLNuG_01J_5f-ext1',
        'WJetsToLNu-LO',
        'WWTo1L1Nu2Q',
        'WWTo2L2Nu',
        # # 'WWTo4Q',
        # 'WWToLNuQQ',
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
        JSONPATCH= (r"'{\"job\":{\"filelist\":\"%(FILELIST)s_%(sa)s.dat\", \"file_prefix\":\"root://gfe02.grid.hep.ph.ic.ac.uk:1097//store/user/dwinterb/Oct07_MC_102X_2018/\"}, \"sequence\":{\"output_name\":\"%(JOB)s\"}}' "%vars());

        job_num=0
        for FLATJSONPATCH in flatjsons:
            nperjob = 20
            if 'scale' in FLATJSONPATCH:
                nperjob = 15
            if 'T-t' in sa:
                nperjob = 10
            if 'TT' in sa:
                nperjob = 20
                if 'scale' in FLATJSONPATCH:
                    nperjob = 15
            if 'QCD' in sa:
                nperjob = 15
            if 'ZZTo4L-ext' in sa or 'TTTo2L2Nu' in sa: nperjob=10
            if 'DY' not in sa and 'EWKZ' not in sa:
                FLATJSONPATCH = FLATJSONPATCH.replace('^scale_efake_0pi_hi^scale_efake_0pi_lo','').replace('^scale_efake_1pi_hi^scale_efake_1pi_lo','').replace('^scale_mufake_0pi_hi^scale_mufake_0pi_lo','').replace('^scale_mufake_1pi_hi^scale_mufake_1pi_lo','')
            if 'DY' not in sa and 'JetsToLNu' not in sa and 'WG' not in sa and 'EWKZ' not in sa and 'EWKW' not in sa:
                FLATJSONPATCH = FLATJSONPATCH.replace('scale_met_hi^scale_met_lo','').replace('res_met_hi^res_met_lo','').replace('scale_met_njets0_hi^scale_met_njets0_lo','').replace('res_met_njets0_hi^res_met_njets0_lo','').replace('scale_met_njets1_hi^scale_met_njets1_lo','').replace('res_met_njets1_hi^res_met_njets1_lo','').replace('scale_met_njets2_hi^scale_met_njets2_lo','').replace('res_met_njets2_hi^res_met_njets2_lo','')
            else:
                FLATJSONPATCH = FLATJSONPATCH.replace('^met_uncl_hi^met_uncl_lo','')
            n_scales = FLATJSONPATCH.count('_lo') + FLATJSONPATCH.count('default')
            nperjob = int(math.ceil(float(nperjob)/max(1.,float(n_scales)*float(n_channels)/10.)))
            nfiles = sum(1 for line in open('%(FILELIST)s_%(sa)s.dat' % vars()))
            for i in range (0,int(math.ceil(float(nfiles)/float(nperjob)))) :
                os.system('%(JOBWRAPPER)s "./bin/HTT --cfg=%(CONFIG)s --json=%(JSONPATCH)s --flatjson=%(FLATJSONPATCH)s --offset=%(i)d --nlines=%(nperjob)d &> jobs/%(JOB)s-%(job_num)d.log" jobs/%(JOB)s-%(job_num)s.sh' %vars())
                if not parajobs: os.system('%(JOBSUBMIT)s jobs/%(JOB)s-%(job_num)d.sh' % vars())
                job_num+=1
            file_persamp.write("%s %d\n" %(JOB, int(math.ceil(float(nfiles)/float(nperjob)))))
        if parajobs:
            os.system('%(JOBWRAPPER)s ./jobs/%(JOB)s-\$\(\(SGE_TASK_ID-1\)\).sh  jobs/parajob_%(JOB)s.sh' %vars())
            PARAJOBSUBMIT = getParaJobSubmit(job_num)
            os.system('%(PARAJOBSUBMIT)s jobs/parajob_%(JOB)s.sh' % vars())

if options.mg_signal or options.proc_sm:
    SIG_FILELIST = FILELIST
    for sa in signal_mc:
        user='dwinterb'
        SIG_FILELIST = FILELIST
        SIG_DIR = 'Oct07_MC_102X_2018'
        JOB='%s_2018' % (sa)
        JSONPATCH= (r"'{\"job\":{\"filelist\":\"%(SIG_FILELIST)s_%(sa)s.dat\",\"file_prefix\":\"root://gfe02.grid.hep.ph.ic.ac.uk:1097//store/user/%(user)s/%(SIG_DIR)s/\"}, \"sequence\":{\"output_name\":\"%(JOB)s\"}}' "%vars());
        if "HToTauTau" in sa and "amcatnloFXFX" in sa:
            JSONPATCH= (r"'{\"job\":{\"filelist\":\"%(SIG_FILELIST)s_%(sa)s.dat\",\"file_prefix\":\"root://gfe02.grid.hep.ph.ic.ac.uk:1097//store/user/%(user)s/%(SIG_DIR)s/\"}, \"sequence\":{\"output_name\":\"%(JOB)s\"}}' "%vars());

        job_num=0
        for FLATJSONPATCH in flatjsons:
            FLATJSONPATCH = FLATJSONPATCH.replace('^scale_efake_0pi_hi^scale_efake_0pi_lo','').replace('^scale_efake_1pi_hi^scale_efake_1pi_lo','').replace('^scale_mufake_0pi_hi^scale_mufake_0pi_lo','').replace('^scale_mufake_1pi_hi^scale_mufake_1pi_lo','')
            FLATJSONPATCH = FLATJSONPATCH.replace('^met_uncl_hi^met_uncl_lo','')
            if os.path.exists('%(SIG_FILELIST)s_%(sa)s.dat' %vars()):
                nfiles = sum(1 for line in open('%(SIG_FILELIST)s_%(sa)s.dat' % vars()))
                nperjob = 20
                if ('MG' in sa or 'Maxmix' in sa or 'Pseudoscalar' in sa) and 'GEN' not in sa: nperjob = 10
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

