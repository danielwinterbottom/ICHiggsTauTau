#!/usr/bin/env python

import sys
import os
from optparse import OptionParser
import math
import fnmatch
import glob

parser = OptionParser()

parser.add_option("--folder", dest = "folder",
                  help="Specify folder that contains the output to be hadded")
parser.add_option("--ignore_nfiles", dest= "ignore", default=False, action='store_true',
                  help="Ignore number of files per sample")
parser.add_option("--sample_list", dest = "samplelist", default="./jobs/files_per_sample_2022_preEE.txt",
                  help="list of files per sample you want to use for hadding")
parser.add_option("--batch", dest= "batch", default=False, action='store_true',
                  help="Submit as batch jobs")

def list_paths(path):
    directories = []
    for item in os.listdir(path):
      if os.path.isdir(os.path.join(path, item)):
        directories.append(item)
    return directories

(options,args) = parser.parse_args()

if not options.folder:
  parser.error('No folder specified')

outputf = options.folder
samplelist = options.samplelist
ignore = options.ignore
batch = options.batch

JOBWRAPPER      = './scripts/generate_job.sh'
JOBSUBMIT       = './scripts/submit_ic_batch_job.sh "hep.q -l h_rt=3:0:0"'
#JOBSUBMIT       = './scripts/submit_ic_batch_job.sh "hep.q -l h_rt=9:0:0"'


sample_list = [

          # Tau
         'TauA_preEE',
         'TauB_preEE',
         'TauB_rereco_preEE',
         'TauC_preEE',
         'TauC_rereco_preEE',
         'TauD_preEE',
         'TauD_rereco_preEE',
         'TauE_postEE',
         'TauE_rereco_postEE',
         'TauF_postEE',
         'TauG_postEE',
         
         #Muon
         
         'SingleMuonA_rereco_preEE',
         'SingleMuonB_rereco_preEE',
         'SingleMuonC_rereco_preEE',
         'SingleMuonA_preEE',
         'SingleMuonB_preEE',
         'SingleMuonC_preEE',
         'MuonD_rereco_preEE',
         'MuonD_preEE',
         'MuonC_preEE',
         'MuonC_rereco_preEE',
         
         
         #MC postEE
         
         'DYJetsToLL-LO_postEE_postEE',
         'GluGluHToTauTau_M125_postEE_v2_postEE',
         'GluGluHToTauTau_M125_postEE_v3_postEE',
         'TTTo2L2Nu_postEE',
         'VBFHToTauTau_M125_Poisson60KeepRAW_postEE',
         'VBFHToTauTau_M125_v2_Poisson70KeepRAW_postEE',
         'WW_postEE_postEE',
         'WZ_postEE_postEE',
         'ZZ_postEE_postEE',
         
         #MC preEE
         
         'DYJetsToLL-LO_summer_preEE',
         'GluGluHToTauTau_M125_preEE',
         'TTTo2L2Nu_preEE',
         'VBFHToTauTau_M125_preEE',
         'WW_preEE',
         'WZ_preEE',
         'ZZ_preEE',
         'ZZZ_preEE'
	]


sample_list = list(set(sample_list))

out=''
for s in sorted(sample_list): out+='    - %s\n' %s

print out

channel = ['tpzee','tpzmm','tt','mt','et','em','zmm','zee']
with open("%(samplelist)s"%vars(),"r") as inf:
  lines = inf.readlines()

subdirs=['']
subdirs+=list_paths(outputf)

new_subdirs=[]
for d in subdirs:
  infi=os.listdir('%(outputf)s/%(d)s' % vars())
  if infi: new_subdirs.append((d,infi))
subdirs=new_subdirs

print subdirs


nfiles={}


def FindMissingFiles(outf, d, samp, chan, infiles):
  files=fnmatch.filter(infiles,'%(samp)s_2022_%(chan)s_*'%vars())
  nums = [int(x.split('_')[-1].replace('.root','')) for x in files]
  nums.sort()
  res = [ele for ele in range(max(nums)+1) if ele not in nums]

  if len(res) !=0:
    print "Some files are missing for sample %(samp)s_2022_%(chan)s! in %(d)s:"%vars()
    for x in res: print '%(samp)s_2022_%(chan)s_%(x)i.root' % vars()
    return False
  else:
    return True

for ind in range(0,len(lines)):
  nfiles[lines[ind].split()[0]]=int(lines[ind].split()[1])
for sa in sample_list:
  remove=True
  to_remove=[]
  hadd_dirs=[]
  command=''
  if batch:
    JOB='jobs/hadd_%s_2022.sh' % sa
    os.system('%(JOBWRAPPER)s "" %(JOB)s' %vars())
  for ch in channel:
    for jsdir in subdirs:
      sdir = jsdir[0]
      infiles=jsdir[1]
      if os.path.isfile('%(outputf)s/%(sdir)s/%(sa)s_2022_%(ch)s_0.root'%vars()):
        if "%(sa)s_2022"%vars() in nfiles or ignore==True:
#          files=glob.glob('%(outputf)s/%(sdir)s/%(sa)s_2022_%(ch)s_*.root'%vars())
          no_missing_files = FindMissingFiles(outputf, sdir, sa, ch,infiles) 
          if no_missing_files and (ignore ==True or len(fnmatch.filter(infiles,'%(sa)s_2022_%(ch)s_*'%vars())) == nfiles["%(sa)s_2022"%vars()]):
            if not batch:  
              print "Hadding in subdir %(sdir)s"%vars()
              print "Hadding %(sa)s_%(ch)s in %(sdir)s"%vars()
              os.system('hadd -f %(outputf)s/%(sdir)s/%(sa)s_%(ch)s_2022.root %(outputf)s/%(sdir)s/%(sa)s_2022_%(ch)s_* &> ./haddout.txt'% vars()) 
              os.system("sed -i '/Warning in <TInterpreter::ReadRootmapFile>/d' ./haddout.txt")
              filetext = open("./haddout.txt").read()
              if 'Warning' in filetext or 'Error' in filetext:
                print "Hadd had a problem:"
                print filetext 
                remove=False
              else :
                to_remove.append('rm %(outputf)s/%(sdir)s/%(sa)s_2022_%(ch)s_*' %vars())
            else:
              haddout='haddout_%s_%s_%s.txt' % (sa,ch,sdir)  
              hadd_dirs.append((haddout, 'rm %(outputf)s/%(sdir)s/%(sa)s_2022_%(ch)s_*' %vars())) 
              command+="echo \"Hadding %(sa)s_%(ch)s in %(sdir)s\"\necho \"Hadding %(sa)s_%(ch)s\"\nhadd -f %(outputf)s/%(sdir)s/%(sa)s_%(ch)s_2022.root %(outputf)s/%(sdir)s/%(sa)s_2022_%(ch)s_* &> ./%(haddout)s\nsed -i '/Warning in <TInterpreter::ReadRootmapFile>/d' ./%(haddout)s\n" % vars()     
          else :
            print "Incorrect number of files for sample %(sa)s_2022_%(ch)s! in %(sdir)s"%vars()
            remove=False

  if not batch and remove:
    # if all channels and systematics were hadded sucsessfully then remove the input files
    for x in to_remove: 
      os.system(x)

  if batch and command:
    with open(JOB, "a") as file: 
      file.write("\n%s" % command)
      rm_command = 'y=1\n' % vars()
      for i in hadd_dirs:
        hadd_file  = i[0]
        rm_command+='if [ \"$(cat %(hadd_file)s | grep -e Warning -e Error)\"  != \"\" ]; then y=0; fi\n' % vars()
      rm_command+='if [ $y == 1 ]; then\n'
      for i in hadd_dirs:
        input_file = i[1]
        rm_command+=input_file+'\n'
      rm_command+='fi'
      if remove: file.write("\n%s" % rm_command)
      file.write('\nEnd of job')
    os.system('%(JOBSUBMIT)s %(JOB)s' % vars())
