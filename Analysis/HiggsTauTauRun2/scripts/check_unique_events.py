import argparse
import ROOT
import uproot
import pandas as pd

parser = argparse.ArgumentParser()
parser.add_argument('--file', '-f', help= 'File to check for duplicated events', default='/vols/cms/dw515/outputs/MSSM/UL_Mar31_2018_effective_v2/DYJetsToLL-LO_mt_2018.root')
args = parser.parse_args()
filename = args.file

file = ROOT.TFile(filename)

#tree_name = 'effective'
tree_name = 'ntuple'
#tree_name = 'TauCheck'

tree = file.Get(tree_name)

unique_sets = set()
duplicated_sets = set()

print 'Checking file: %(filename)s' % vars()

N=tree.GetEntries()

#first check looks if there are cases where the next even along is a duplicate (most likely to happen give the bug that caused the duplicates) 
print 'Checking neighbouring events:' 
foundDuplicates=False
i=0
while i<N-1:
  tree.GetEntry(i)
  values_1 = (tree.run, tree.lumi, tree.event)
  tree.GetEntry(i+1)
  values_2 = (tree.run, tree.lumi, tree.event)
  i+=2
  if values_1==values_2:
    print i-2, values_1, values_2 
    foundDuplicates=True 
    break

if foundDuplicates: print 'Found duplicated events!'
else:
  print 'No duplicates found'

file.Close()

# the next check looks for duplicates anywhere in the tree (can have issues with memory for large samples)
print 'Checking all events:' 
file = uproot.open(filename)
tree = file[tree_name]

branches = ["event", "lumi", "run"]

df = tree.pandas.df(branches)

duplicated_events = df.duplicated()

if duplicated_events.any():
  print 'Found duplicated events!'
  print df[duplicated_events]
else:
  print 'No duplicates found'

print 'finished'

