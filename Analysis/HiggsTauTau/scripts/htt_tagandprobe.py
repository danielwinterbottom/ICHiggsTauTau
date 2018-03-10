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


#CHANNELS_2015 = ['et', 'mt', 'em', 'tt']

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
                  help="Process data samples (including embedded)")
parser.add_option("--embed", dest="proc_embed", action='store_true', default=False,
                  help="Process embedded sampes")
parser.add_option("--bkg", dest="proc_bkg", action='store_true', default=False,
                  help="Process background mc samples")
parser.add_option("--no_json", dest="no_json", action='store_true', default=False,
                  help="Do not read the channels to process from the json to decide which datasets to run on")
parser.add_option("--list_backup", dest="slbackupname", type='string', default='prevlist',
                  help="Name you want to give to the previous files_per_samples file, in case you're resubmitting a subset of jobs")
parser.add_option("--scales", dest="scales", type='string', default='default',
                  help="List of systematic shifts to process")
parser.add_option("--parajobs", dest="parajobs", action='store_true', default=False,
                  help="Submit jobs parametrically")
parser.add_option("--config", dest="config", type='string', default='',
                  help="Config file")


(options, args) = parser.parse_args()
if options.wrapper: JOBWRAPPER=options.wrapper
if options.submit:  JOBSUBMIT=options.submit

def getParaJobSubmit(N):
  if not options.submit: return 'true'
  sub_opts=JOBSUBMIT.split(' ',1)[1]
  if '\"' in sub_opts: sub_opts = sub_opts.replace('\"','')
  sub_opts = '%s \"%s -t 1-%i:1\"' %(JOBSUBMIT.split(' ',1)[0], sub_opts, N)
  return sub_opts

BACKUPNAME = options.slbackupname

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
 
CONFIG='scripts/configtagandprobe.json'
 
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
  if 'scale_j' in i:
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
  
FILELIST='filelists/Trigger_MC_80X'

signal_mc = [ ]
signal_vh = [ ] 
signal_mc_ww = [ ]

if os.path.isfile("./jobs/files_per_sample.txt"):
  os.system("mv ./jobs/files_per_sample.txt ./jobs/files_per_sample-%(BACKUPNAME)s.txt"%vars())

file_persamp = open("./jobs/files_per_sample.txt", "w")


if options.proc_data  or options.proc_embed:
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

if options.proc_data:  

  data_samples = []
  data_eras = ['B','C','D','E','F','G','H']
  for chn in channels:
    for era in data_eras: 
         if 'mt' in chn or 'zmm' or 'tpzmm' in chn or 'tpmt' in chn:
           if not era == 'H':  
               data_samples+=[   
                'SingleMuon'+era]
           else:
               data_samples+=[   
                'SingleMuon'+era+'v2']
               data_samples+=[   
                'SingleMuon'+era+'v3']
         if 'et' in chn or 'zee' in chn or 'tpzee' in chn:
           if not era == 'H':  
               data_samples+=[   
                'SingleElectron'+era]
           else:
               data_samples+=[   
                'SingleElectron'+era+'v2']
               data_samples+=[   
                'SingleElectron'+era+'v3']

  DATAFILELIST="./filelists/Trigger_Data_80X"


  for sa in data_samples:
    JOB='%s_2016' % (sa)
    JSONPATCH= (r"'{\"job\":{\"filelist\":\"%(DATAFILELIST)s_%(sa)s.dat\",\"file_prefix\":\"root://gfe02.grid.hep.ph.ic.ac.uk:1097//store/user/dwinterb/Trigger_80X/\",\"sequences\":{\"em\":[],\"et\":[],\"mt\":[],\"tt\":[],\"zmm\":[],\"zee\":[]}}, \"sequence\":{\"output_name\":\"%(JOB)s\",\"is_data\":true}}' "%vars());
    nfiles = sum(1 for line in open('%(DATAFILELIST)s_%(sa)s.dat' % vars()))
    nperjob = 40

    for i in range (0,int(math.ceil(float(nfiles)/float(nperjob)))) :
      os.system('%(JOBWRAPPER)s "./bin/HTT --cfg=%(CONFIG)s --json=%(JSONPATCH)s --offset=%(i)d --nlines=%(nperjob)d &> jobs/%(JOB)s-%(i)d.log" jobs/%(JOB)s-%(i)s.sh' %vars())
      if not parajobs: os.system('%(JOBSUBMIT)s jobs/%(JOB)s-%(i)d.sh' % vars())
    if parajobs: 
      os.system('%(JOBWRAPPER)s ./jobs/%(JOB)s-\$\(\(SGE_TASK_ID-1\)\).sh  jobs/parajob_%(JOB)s.sh' %vars())
      PARAJOBSUBMIT = getParaJobSubmit(int(math.ceil(float(nfiles)/float(nperjob))))
      os.system('%(PARAJOBSUBMIT)s jobs/parajob_%(JOB)s.sh' % vars())  
    file_persamp.write("%s %d\n" %(JOB, int(math.ceil(float(nfiles)/float(nperjob)))))
      
if options.proc_embed:        
        
  embed_samples = []
  data_eras = ['B','C','D','E','F','G','H']
  for chn in channels:
    for era in data_eras:
      if 'mt' or 'tpmt' in chn:
        embed_samples+=['EmbeddingMuTau'+era]
      if 'tpzee' in chn:
        embed_samples+=['EmbeddingElEl'+era]
      if 'tpzmm' in chn:
        embed_samples+=['EmbeddingMuMu'+era]  

        


  EMBEDFILELIST="./filelists/Trigger_MC_80X"
  
  for sa in embed_samples:
    JOB='%s_2016' % (sa)
    JSONPATCH= (r"'{\"job\":{\"filelist\":\"%(EMBEDFILELIST)s_%(sa)s.dat\",\"file_prefix\":\"root://gfe02.grid.hep.ph.ic.ac.uk:1097//store/user/dwinterb/Trigger_80X/\",\"sequences\":{\"em\":[],\"et\":[],\"mt\":[],\"tt\":[],\"zmm\":[],\"zee\":[]}}, \"sequence\":{\"output_name\":\"%(JOB)s\",\"is_embedded\":true}}' "%vars());
    nfiles = sum(1 for line in open('%(EMBEDFILELIST)s_%(sa)s.dat' % vars()))
    nperjob = 40
    
    for i in range (0,int(math.ceil(float(nfiles)/float(nperjob)))) :  
      os.system('%(JOBWRAPPER)s "./bin/HTT --cfg=%(CONFIG)s --json=%(JSONPATCH)s --offset=%(i)d --nlines=%(nperjob)d &> jobs/%(JOB)s-%(i)d.log" jobs/%(JOB)s-%(i)s.sh' %vars())
      if not parajobs: os.system('%(JOBSUBMIT)s jobs/%(JOB)s-%(i)d.sh' % vars())
    if parajobs: 
      os.system('%(JOBWRAPPER)s ./jobs/%(JOB)s-\$\(\(SGE_TASK_ID-1\)\).sh  jobs/parajob_%(JOB)s.sh' %vars())
      PARAJOBSUBMIT = getParaJobSubmit(int(math.ceil(float(nfiles)/float(nperjob))))
      os.system('%(PARAJOBSUBMIT)s jobs/parajob_%(JOB)s.sh' % vars())  
    file_persamp.write("%s %d\n" %(JOB, int(math.ceil(float(nfiles)/float(nperjob)))))


if options.proc_bkg:
    central_samples = [
    'DYJetsToLL-LO-ext1',
    'DYJetsToLL-LO-ext2',
     ]
  

    for sa in central_samples:
      JOB='%s_2016' % (sa)
      JSONPATCH= (r"'{\"job\":{\"filelist\":\"%(FILELIST)s_%(sa)s.dat\"},\"file_prefix\":\"root://gfe02.grid.hep.ph.ic.ac.uk:1097//store/user/dwinterb/Trigger_80X/\", \"sequence\":{\"output_name\":\"%(JOB)s\"}}' "%vars());
      job_num=0
      for FLATJSONPATCH in flatjsons: 
        nperjob = 20
        if 'scale' in FLATJSONPATCH:
          nperjob = 15
        if 'DY' not in sa:
          FLATJSONPATCH = FLATJSONPATCH.replace('^scale_efake_0pi_hi^scale_efake_0pi_lo','').replace('^scale_efake_1pi_hi^scale_efake_1pi_lo','').replace('^scale_mufake_0pi_hi^scale_mufake_0pi_lo','').replace('^scale_mufake_1pi_hi^scale_mufake_1pi_lo','')
        if ('DY' in sa or 'EWKZ' in sa) and 'scale_j' not in FLATJSONPATCH: nperjob = int(math.ceil(float(nperjob)/max(1.,float(n_scales)*float(n_channels)/50.)))
        elif 'scale_j' not in FLATJSONPATCH: nperjob = int(math.ceil(float(nperjob)/max(1.,float(n_scales-8)*float(n_channels)/10.)))
        else: nperjob = int(math.ceil(float(nperjob)/max(1.,float(n_channels)/3.)))
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



