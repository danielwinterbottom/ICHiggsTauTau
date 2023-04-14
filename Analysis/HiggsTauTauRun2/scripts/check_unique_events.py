import ROOT
import argparse
from sys import getsizeof

parser = argparse.ArgumentParser()
parser.add_argument('--file', '-f', help= 'File to check for duplicated events', default='/vols/cms/dw515/outputs/MSSM/UL_Mar31_2018/DYJetsToLL-LO_zmm_2018.root')
args = parser.parse_args()
filename = args.file

file = ROOT.TFile(filename)

tree = file.Get("effective")

unique_sets = set()
duplicated_sets = set()

print 'Checking file: %(filename)s' % vars()

count=0
N=tree.GetEntries()
for i in range(0,tree.GetEntries()):
    tree.GetEntry(i)

    values = (tree.run, tree.lumi, tree.event)
    event = tree.GetLeaf('event').GetValue()
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
