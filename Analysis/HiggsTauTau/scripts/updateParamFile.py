import ROOT
import os
import sys
import json
import pprint
# import glob

args = sys.argv[1:]

with open(args[0]) as jsonfile:
    js = json.load(jsonfile)


pprint.pprint(js)

for file in args[2:]:
    f = ROOT.TFile(file)
    t = f.Get('effective')
    if t == None:
        continue
    name = os.path.splitext(os.path.basename(file))[0]
    e_all = t.GetEntries()
    e_p = t.GetEntries('wt>0')
    e_m = t.GetEntries('wt<0')
    e_eff = e_p - e_m
    print '%-40s %15i %15i %15i %15i' % (name, e_all, e_p, e_m, e_eff)
    if name in js:
        js[name]['evt'] = e_p - e_m
        js[name]['evt_p'] = e_p
        js[name]['evt_m'] = e_m


jsondata = json.dumps(js, sort_keys=True, indent=2, separators=(',', ': '))

with open(args[1], 'w') as out_file:
    out_file.write(jsondata)
