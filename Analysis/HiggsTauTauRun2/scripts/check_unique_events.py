import ROOT
import argparse
from sys import getsizeof

parser = argparse.ArgumentParser()
parser.add_argument('--file', '-f', help= 'File to check for duplicated events', default='/vols/cms/dw515/outputs/MSSM/UL_Mar31_2018_effective_v2/DYJetsToLL-LO_mt_2018.root')
args = parser.parse_args()
filename = args.file

file = ROOT.TFile(filename)

tree = file.Get("effective")

unique_sets = set()
duplicated_sets = set()

print 'Checking file: %(filename)s' % vars()

count=0
N=tree.GetEntries()

#first check looks if there are cases where the next even along is a duplicate (most likely to happen give the bug that caused the duplicates) 
print 'Checking neighbouring events:' 
foundDuplicates=False
for i in range(0,tree.GetEntries(),2):
  tree.GetEntry(i)
  values_1 = (tree.run, tree.lumi, tree.event)
  tree.GetEntry(i+1)
  values_2 = (tree.run, tree.lumi, tree.event)

  if values_1==values_2: 
    foundDuplicates=True 
    break

if foundDuplicates: print 'Found duplicated events!'
else:
  print 'No duplicates found'

# the next check looks for duplicates anywhere in the tree (can have issues with memory for large samples)
print 'Checking all events:' 
for i in range(0,tree.GetEntries()):
    tree.GetEntry(i)

    values = (tree.run, tree.lumi, tree.event)
    if values in unique_sets:
        count+=1
        duplicated_sets.add(values)
        #print("Found duplicate set:", i, values, event)
    else:
        unique_sets.add(values)

    if (i % 100000==0): print 'Processing event %i of %i. Length of unique_sets and duplicated_sets = %i and %i. Size of unique_sets and duplicated_sets in bytes = %i and %i ' % ( i, N, len(unique_sets), len(duplicated_sets), getsizeof(unique_sets), getsizeof(duplicated_sets))

if count>0:
  print 'Found %(count)i duplicated events!' % vars()
  print duplicated_sets
else:
  print 'No duplicates found'

print 'finished'

#COUNT=1; for x in /vols/cms/dw515/outputs/MSSM/UL_Mar31_2018/*.root; do echo if [ \$\{SGE_TASK_ID\} == ${COUNT} ]\; then python -u check_unique_events.py -f ${x} \&\> duplicate_check_${COUNT}.out\; fi; ((COUNT++)); done >> duplicates_check_para_job.sh
