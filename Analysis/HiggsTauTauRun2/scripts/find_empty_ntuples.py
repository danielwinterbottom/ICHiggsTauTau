import ROOT
import argparse
import os

parser = argparse.ArgumentParser()
parser.add_argument('--folder',help= 'Folder to check', default='./')
parser.add_argument('--year',help= 'Year to resubmit', default='2017')
args = parser.parse_args()

folder = args.folder
year = args.year

empty_files = []
non_empty_files = []
for filename in os.listdir(folder):
  if filename.endswith(".root") and "_tt_" in filename:
      print filename
      file_loc = os.path.join(folder, filename)
      f = ROOT.TFile(file_loc)
      t = f.Get('ntuple')
      num_und= filename.count('_')
      name = ''
      for i in range(0,num_und-1): name += filename.split('_')[i]
      if t.GetEntries() == 0 and name not in empty_files:
        empty_files.append(name)
      elif t.GetEntries() > 0 and name not in empty_files and name not in non_empty_files:
        non_empty_files.append(name)

print "***************************"
print "Empty Files"
print "***************************"

for i in empty_files: print i

print "***************************"
print "Filled Files"
print "***************************"

for i in non_empty_files: print i

