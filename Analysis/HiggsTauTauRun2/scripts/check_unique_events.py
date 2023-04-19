import argparse

import uproot
import pandas as pd

parser = argparse.ArgumentParser()
parser.add_argument('--file', '-f', help= 'File to check for duplicated events', default='/vols/cms/dw515/outputs/MSSM/UL_Mar31_2018_effective_v2/DYJetsToLL-LO_mt_2018.root')
args = parser.parse_args()
filename = args.file

file = uproot.open(filename)
tree = file["effective"]

print 'Checking file: %(filename)s' % vars()

branches = ["event", "lumi", "run"]

df = tree.pandas.df(branches)

duplicated_events = df.duplicated()

if duplicated_events.any():
  print 'Found duplicated events!'
  print df[duplicated_events]
else:
  print 'No duplicates found'

print 'finished'

