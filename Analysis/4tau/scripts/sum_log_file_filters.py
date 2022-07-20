import glob
from collections import OrderedDict

filename = "DoubleMuon"

store = OrderedDict()
for f in glob.glob("jobs/"+filename+"*.log"):
  save_output = False
  fo = open(f, "r")
  for x in fo:
    if "Post-analysis" in x:
      save_output = True
      continue
    if "Finished Processing" in x:
      save_output = False
      continue
    if save_output and "------------" not in x:
      split_x = x.split() 
      if split_x[0] in store.keys():
        store[split_x[0]] += int(split_x[1])
      else:
        store[split_x[0]] = int(split_x[1])


for k, v in store.iteritems():
  print k

print " "
for k, v in store.iteritems():
  print v
