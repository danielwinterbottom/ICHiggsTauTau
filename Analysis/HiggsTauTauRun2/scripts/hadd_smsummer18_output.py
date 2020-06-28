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
parser.add_option("--sample_list", dest = "samplelist", default="./jobs/files_per_sample.txt",
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
#JOBSUBMIT       = './scripts/submit_ic_batch_job.sh "hep.q -l h_rt=9:0:0"'


sample_list = [
    #'DYJetsToLL-2017',
    'SingleMuonA',
    'SingleMuonB',
    'SingleMuonC',
    'SingleMuonD',
    'TauA',
    'TauB',
    'TauC',
    'TauD',
    'EGammaA',
    'EGammaB',
    'EGammaC',
    'EGammaD',
    'EGammaD_ReReco',
    'MuonEGA',
    'MuonEGB',
    'MuonEGC',
    'MuonEGD',
    'DYJetsToLL',
    'DY1JetsToLL-LO',
    'DY2JetsToLL-LO',
    'DY3JetsToLL-LO',
    'DY4JetsToLL-LO',
    'DYJetsToLL-LO',
    'DYJetsToLL_M-10-50-LO',
    'EWKWMinus2Jets',
    'EWKWPlus2Jets',
    'EWKZ2Jets',#new
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
    'WWTo1L1Nu2Q',
    'WWTo2L2Nu',#new
    'WWTo4Q',#new
    'WWToLNuQQ',#new
    'WZTo1L1Nu2Q',
    'WZTo1L3Nu',
    'WZTo2L2Q',
    'WZTo3LNu',#new
    'WZTo3LNu-ext1',
    'ZZTo2L2Nu-ext1',
    'ZZTo2L2Nu-ext2',
    'ZZTo2L2Q',
    'ZZTo4L',
    'ZZTo4L-ext',
    'EmbeddingElElB',
    'EmbeddingElElC',
    'EmbeddingElElD',
    'EmbeddingElElA',
    'EmbeddingElMuB',
    'EmbeddingElMuC',
    'EmbeddingElMuD',
    'EmbeddingElMuA',
    'EmbeddingElTauB',
    'EmbeddingElTauC',
    'EmbeddingElTauD',
    'EmbeddingElTauA',
    'EmbeddingMuMuB',
    'EmbeddingMuMuC',
    'EmbeddingMuMuD',
    'EmbeddingMuMuA',
    'EmbeddingMuTauB',
    'EmbeddingMuTauC',
    'EmbeddingMuTauD',
    'EmbeddingMuTauA',
    'EmbeddingTauTauB',
    'EmbeddingTauTauC',
    'EmbeddingTauTauD',
    'EmbeddingTauTauA',

    'GluGluHToTauTauUncorrelatedDecay',
    'GluGluHToTauTauUncorrelatedDecay_Filtered',
    'GluGluHToTauTau_M-125',
    'GluGluHToWWTo2L2Nu_M-125',
    'GluGluToHToTauTauPlusTwoJets_M125_amcatnloFXFX',
    'GluGluToHToTauTau_M-125-nospinner-filter',
    'GluGluToHToTauTau_M-125-nospinner',
    'GluGluToHToTauTau_M125_amcatnloFXFX',
    'GluGluToMaxmixHToTauTauPlusTwoJets_M125_amcatnloFXFX',
    'GluGluToMaxmixHToTauTau_M125_amcatnloFXFX',
    'GluGluToPseudoscalarHToTauTauPlusTwoJets_M125_amcatnloFXFX',
    'GluGluToPseudoscalarHToTauTau_M125_amcatnloFXFX',
    'JJH0MToTauTauPlusOneJets',
    'JJH0MToTauTauPlusOneJets_Filtered',
    'JJH0MToTauTauPlusTwoJets',
    'JJH0MToTauTauPlusTwoJets_Filtered',
    'JJH0MToTauTauPlusZeroJets',
    'JJH0MToTauTauPlusZeroJets_Filtered',
    'JJH0Mf05ph0ToTauTauPlusOneJets',
    'JJH0Mf05ph0ToTauTauPlusOneJets_Filtered',
    'JJH0Mf05ph0ToTauTauPlusTwoJets',
    'JJH0Mf05ph0ToTauTauPlusTwoJets_Filtered',
    'JJH0Mf05ph0ToTauTauPlusZeroJets',
    'JJH0Mf05ph0ToTauTauPlusZeroJets_Filtered',
    'JJH0PMToTauTauPlusOneJets',
    'JJH0PMToTauTauPlusOneJets_Filtered',
    'JJH0PMToTauTauPlusTwoJets',
    'JJH0PMToTauTauPlusTwoJets_Filtered',
    'JJH0PMToTauTauPlusZeroJets',
    'JJH0PMToTauTauPlusZeroJets_Filtered',
    'JJHiggs0MToTauTau',
    'JJHiggs0Mf05ph0ToTauTau',
    'JJHiggs0PMToTauTau',
    'VBFHToTauTauUncorrelatedDecay',
    'VBFHToTauTauUncorrelatedDecay_Filtered',
    'VBFHToTauTau_M-125-MM-filter',
    'VBFHToTauTau_M-125-PS-filter',
    'VBFHToTauTau_M-125-SM-filter',
    'VBFHToTauTau_M-125-ext1',
    'VBFHToTauTau_M-125-nospinner-filter',
    'VBFHToTauTau_M-126-nospinner',
    'VBFHToWWTo2L2Nu_M-125',
    'VBFHiggs0L1ToTauTau',
    'VBFHiggs0L1ZgToTauTau',
    'VBFHiggs0L1Zgf05ph0ToTauTau',
    'VBFHiggs0L1f05ph0ToTauTau',
    'VBFHiggs0MToTauTau',
    'VBFHiggs0Mf05ph0ToTauTau',
    'VBFHiggs0PHToTauTau',
    'VBFHiggs0PHf05ph0ToTauTau',
    'VBFHiggs0PMToTauTau',
    'WHiggs0L1ToTauTau',
    'WHiggs0L1f05ph0ToTauTau',
    'WHiggs0MToTauTau',
    'WHiggs0Mf05ph0ToTauTau',
    'WHiggs0PHToTauTau',
    'WHiggs0PHf05ph0ToTauTau',
    'WHiggs0PMToTauTau',
    'WminusHToTauTauUncorrelatedDecay',
    'WminusHToTauTauUncorrelatedDecay_Filtered',
    'WminusHToTauTau_M-125',
    'WplusHToTauTauUncorrelatedDecay',
    'WplusHToTauTauUncorrelatedDecay_Filtered',
    'WplusHToTauTau_M-125',
    'ZHToTauTauUncorrelatedDecay_Filtered',
    'ZHToTauTau_M-125',
    'ZHiggs0L1ToTauTau',
    'ZHiggs0L1ZgToTauTau',
    'ZHiggs0L1Zgf05ph0ToTauTau',
    'ZHiggs0L1f05ph0ToTauTau',
    'ZHiggs0MToTauTau',
    'ZHiggs0Mf05ph0ToTauTau',
    'ZHiggs0PHToTauTau',
    'ZHiggs0PHf05ph0ToTauTau',
    'ZHiggs0PMToTauTau',
    'ttHiggs0MToTauTau',
    'ttHiggs0Mf05ph0ToTauTau',
    'ttHiggs0PMToTauTau',

	]


channel = ['em','et','mt','tt','zee','zmm','wmnu','tpzee','tpzmm','tpmt','tpem']
#channel = ['mt']
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
  files=fnmatch.filter(infiles,'%(samp)s_2018_%(chan)s_*'%vars())
  nums = [int(x.split('_')[-1].replace('.root','')) for x in files]
  nums.sort()
  res = [ele for ele in range(max(nums)+1) if ele not in nums]

  if len(res) !=0:
    print "Some files are missing for sample %(samp)s_2018_%(chan)s! in %(d)s:"%vars()
    for x in res: print '%(samp)s_2018_%(chan)s_%(x)i.root' % vars()
    return False
  else:
    return True

for ind in range(0,len(lines)):
  nfiles[lines[ind].split()[0]]=int(lines[ind].split()[1])
for sa in sample_list:
  for ch in channel:
    remove=True
    to_remove=[]
    hadd_dirs=[]
    command=''
    if batch:
      JOB='jobs/hadd_%s_%s.sh' % (sa, ch)
      os.system('%(JOBWRAPPER)s "" %(JOB)s' %vars())
    for jsdir in subdirs:
      sdir = jsdir[0]
      infiles=jsdir[1]
      if os.path.isfile('%(outputf)s/%(sdir)s/%(sa)s_2018_%(ch)s_0.root'%vars()):
        if "%(sa)s_2018"%vars() in nfiles or ignore==True:
          no_missing_files = FindMissingFiles(outputf, sdir, sa, ch,infiles) 
          if no_missing_files and (ignore ==True or len(fnmatch.filter(infiles,'%(sa)s_2018_%(ch)s_*'%vars())) == nfiles["%(sa)s_2018"%vars()]):
            if not batch:  
              print "Hadding in subdir %(sdir)s"%vars()
              print "Hadding %(sa)s_%(ch)s in %(sdir)s"%vars()
              os.system('hadd -f %(outputf)s/%(sdir)s/%(sa)s_%(ch)s_2018.root %(outputf)s/%(sdir)s/%(sa)s_2018_%(ch)s_* &> ./haddout.txt'% vars()) 
              os.system("sed -i '/Warning in <TInterpreter::ReadRootmapFile>/d' ./haddout.txt")
              filetext = open("./haddout.txt").read()
              if 'Warning' in filetext or 'Error' in filetext:
                print "Hadd had a problem:"
                print filetext 
                remove=False
              else:
                to_remove.append('rm %(outputf)s/%(sdir)s/%(sa)s_2018_%(ch)s_*' %vars())
                os.system('rm %(outputf)s/%(sdir)s/%(sa)s_2018_%(ch)s_*' %vars())
            else:
              haddout='haddout_%s_%s_%s.txt' % (sa,ch,sdir)  
              hadd_dirs.append((haddout, 'rm %(outputf)s/%(sdir)s/%(sa)s_2018_%(ch)s_*' %vars())) 
              #command+="echo \"Hadding %(sa)s_%(ch)s in %(sdir)s\"\necho \"Hadding %(sa)s_%(ch)s\"\nhadd -f %(outputf)s/%(sdir)s/%(sa)s_%(ch)s_2018.root %(outputf)s/%(sdir)s/%(sa)s_2018_%(ch)s_* &> ./%(haddout)s\nsed -i '/Warning in <TInterpreter::ReadRootmapFile>/d' ./%(haddout)s\n" % vars() # to be used if you want to only delete all files at the end of the hadding 
              command+="echo \"Hadding %(sa)s_%(ch)s in %(sdir)s\"\necho \"Hadding %(sa)s_%(ch)s\"\nhadd -f %(outputf)s/%(sdir)s/%(sa)s_%(ch)s_2018.root %(outputf)s/%(sdir)s/%(sa)s_2018_%(ch)s_* &> ./%(haddout)s\nsed -i '/Warning in <TInterpreter::ReadRootmapFile>/d' ./%(haddout)s\nif [ \"$(cat %(haddout)s | grep -e Warning -e Error)\" != \"\" ]; then echo \"Hadd had a problem:\"\ncat %(haddout)s ;\nelse rm %(outputf)s/%(sdir)s/%(sa)s_2018_%(ch)s_*; fi\n" % vars() 
          else :
            print "Incorrect number of files for sample %(sa)s_2018_%(ch)s! in %(sdir)s"%vars()
            remove=False

    if batch and command:
      with open(JOB, "a") as file: 
        file.write("\n%s" % command)
        file.write('\necho End of job &> jobs/hadd_%(sa)s_%(ch)s.log')
      os.system('%(JOBSUBMIT)s %(JOB)s' % vars())

   # if batch and command:  # to be used if you want to only delete all files at the end of the hadding
   #   with open(JOB, "a") as file: 
   #     file.write("\n%s" % command)
   #     rm_command = 'y=1\n' % vars()
   #     for i in hadd_dirs:
   #       hadd_file  = i[0]
   #       rm_command+='if [ \"$(cat %(hadd_file)s | grep -e Warning -e Error)\"  != \"\" ]; then y=0; fi\n' % vars()
   #     rm_command+='if [ $y == 1 ]; then\n'
   #     for i in hadd_dirs:
   #       input_file = i[1]
   #       rm_command+=input_file+'\n'
   #     rm_command+='fi'
   #     if remove: file.write("\n%s" % rm_command)
   #     file.write('\necho End of job &> jobs/hadd_%(sa)s_%(ch)s.log')
   #   os.system('%(JOBSUBMIT)s %(JOB)s' % vars())

  #if not batch and remove:
  #  # if all channels and systematics were hadded sucsessfully then remove the input files
  #  for x in to_remove:
  #    os.system(x)
