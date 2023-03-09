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
parser.add_option("--sample_list", dest = "samplelist", default="./jobs/files_per_sample_2017.txt",
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
JOBSUBMIT       = './scripts/submit_ic_batch_job.sh "hep.q -l h_rt=0:180:0"'

sample_list = [
         # Data
         # Single Muon
        'SingleMuonB',
        'SingleMuonC',
        'SingleMuonD',
        'SingleMuonE',
        'SingleMuonF',
         # Single Electron
        'SingleElectronB',
        'SingleElectronC',
        'SingleElectronD',
        'SingleElectronE',
        'SingleElectronF',
         # Tau
        'TauB',
        'TauC',
        'TauD',
        'TauE',
        'TauF',
         # MuonEG
        'MuonEGB',
        'MuonEGC',
        'MuonEGD',
        'MuonEGE',
        'MuonEGF',
         # Double Muon
#        'DoubleMuonB',
#        'DoubleMuonC',
#        'DoubleMuonD',
#        'DoubleMuonE',
#        'DoubleMuonF',

         # MC
  	 # Drell-Yan LO
	'DY1JetsToLL-LO',
        'DY2JetsToLL-LO',
        'DY3JetsToLL-LO',
        'DY4JetsToLL-LO',
        'DYJetsToLL-LO',
	 # Low mass Drell Yan LO
        'DYJetsToLL_M-10to50-LO',
	'DY1JetsToLL_M-10to50-LO',
	'DY2JetsToLL_M-10to50-LO',
	'DY3JetsToLL_M-10to50-LO',
	'DY4JetsToLL_M-10to50-LO',
	 # Drell-Yan NLO
	'DYJetsToLL_0J-NLO',
	'DYJetsToLL_1J-NLO',
	'DYJetsToLL_2J-NLO',
	'DYJetsToLL-NLO',
	 # Electroweak W and Z
	'EWKWMinus2Jets_WToLNu',
	'EWKWPlus2Jets_WToLNu',
	'EWKZ2Jets_ZToLL',
	 # W + Jets L0
	'WJetsToLNu-LO',
	'W1JetsToLNu-LO',
	'W2JetsToLNu-LO',
	'W3JetsToLNu-LO',
	'W4JetsToLNu-LO',
	 # W + Jets NLO
	#'WJetsToLNu_0J-NLO',
	#'WJetsToLNu_1J-NLO',
	#'WJetsToLNu_2J-NLO',
	 # ttbar
	'TTTo2L2Nu',
	'TTToHadronic',
	'TTToSemiLeptonic',
	 # Split diboson (Missing Files: WZTo1L3Nu, WZTo2L2Q,WZTo1L1Nu2Q,WWTo1L1Nu2Q)
	'WZTo3LNu',
	'WWTo2L2Nu',
	'ZZTo2L2Nu',
	'ZZTo4L',
        'WZTo1L3Nu',
	'WZTo2Q2L',
	'WZTo1L1Nu2Q',
	'WWTo1L1Nu2Q',
	 # Inclusive
	'WW',
	'WZ',
	'ZZ',
         # Triboson
        'WWZ',
        'WWZ-ext1',
        'WZZ',
        'WZZ-ext1',
        'WWW',
        'WWW-ext1',
        'ZZZ',
        'ZZZ-ext1',
         # Other backgrounds
        'WGToLNuG',
        'Tbar-t',
        'Tbar-tW',
        'T-t',
        'T-tW',
         # SM Higgs
        'GluGluHToTauTau_M125',
        'VBFHToTauTau_M125',
        'WminusHToTauTau_M125',
        'WplusHToTauTau_M125',
        'ZHToTauTau_M125',
        'ttHToTauTau_M125',

	]


sample_list = list(set(sample_list))

out=''
for s in sorted(sample_list): out+='    - %s\n' %s

print out

channel = ['em','et','mt','tt','zee','zmm','wmnu','tpzee','tpzmm','tpmt','tpem']
#channel = ['zee','et','em','mt','tt','zmm']
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
  files=fnmatch.filter(infiles,'%(samp)s_2017_%(chan)s_*'%vars())
  nums = [int(x.split('_')[-1].replace('.root','')) for x in files]
  nums.sort()
  res = [ele for ele in range(max(nums)+1) if ele not in nums]

  if len(res) !=0:
    print "Some files are missing for sample %(samp)s_2017_%(chan)s! in %(d)s:"%vars()
    for x in res: print '%(samp)s_2017_%(chan)s_%(x)i.root' % vars()
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
    JOB='jobs/hadd_%s_2017.sh' % sa
    os.system('%(JOBWRAPPER)s "" %(JOB)s' %vars())
  for ch in channel:
    for jsdir in subdirs:
      sdir = jsdir[0]
      infiles=jsdir[1]
      if os.path.isfile('%(outputf)s/%(sdir)s/%(sa)s_2017_%(ch)s_0.root'%vars()):
        if "%(sa)s_2017"%vars() in nfiles or ignore==True:
#          files=glob.glob('%(outputf)s/%(sdir)s/%(sa)s_2017_%(ch)s_*.root'%vars())
          no_missing_files = FindMissingFiles(outputf, sdir, sa, ch,infiles) 
          if no_missing_files and (ignore ==True or len(fnmatch.filter(infiles,'%(sa)s_2017_%(ch)s_*'%vars())) == nfiles["%(sa)s_2017"%vars()]):
            if not batch:  
              print "Hadding in subdir %(sdir)s"%vars()
              print "Hadding %(sa)s_%(ch)s in %(sdir)s"%vars()
              os.system('hadd -f %(outputf)s/%(sdir)s/%(sa)s_%(ch)s_2017.root %(outputf)s/%(sdir)s/%(sa)s_2017_%(ch)s_* &> ./haddout.txt'% vars()) 
              os.system("sed -i '/Warning in <TInterpreter::ReadRootmapFile>/d' ./haddout.txt")
              filetext = open("./haddout.txt").read()
              if 'Warning' in filetext or 'Error' in filetext:
                print "Hadd had a problem:"
                print filetext 
                remove=False
              else :
                to_remove.append('rm %(outputf)s/%(sdir)s/%(sa)s_2017_%(ch)s_*' %vars())
            else:
              haddout='haddout_%s_%s_%s.txt' % (sa,ch,sdir)  
              hadd_dirs.append((haddout, 'rm %(outputf)s/%(sdir)s/%(sa)s_2017_%(ch)s_*' %vars())) 
              command+="echo \"Hadding %(sa)s_%(ch)s in %(sdir)s\"\necho \"Hadding %(sa)s_%(ch)s\"\nhadd -f %(outputf)s/%(sdir)s/%(sa)s_%(ch)s_2017.root %(outputf)s/%(sdir)s/%(sa)s_2017_%(ch)s_* &> ./%(haddout)s\nsed -i '/Warning in <TInterpreter::ReadRootmapFile>/d' ./%(haddout)s\n" % vars()     
          else :
            print "Incorrect number of files for sample %(sa)s_2017_%(ch)s! in %(sdir)s"%vars()
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
