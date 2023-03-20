import ROOT
import json

sig_samples=[]
grid_phi = ["100","110","125","140","160","180","200","250","300"]
grid_A = ["60","70","80","90","100","125","140","160"]
for mp in grid_phi:
  for mA in grid_A:
       sig_samples.append("ZstarTophi{}A{}To4Tau".format(mp,mA))

folder = "/vols/cms/gu18/Offline/output/4tau/1502_sig_eff/"
file_ext = "_mmtt_2018.root"

# total
dump_dict = {}
for k in sig_samples:
  dump_dict[k] = {}

# mur muf
try_weights = {"qcd_scale": [
                   "wt_mur1_muf1",
                   "wt_mur1_muf2",
                   "wt_mur1_muf0p5",
                   "wt_mur2_muf1",
                   "wt_mur2_muf2",
                   #"wt_mur2_muf0p5",
                   "wt_mur0p5_muf1",
                   #"wt_mur0p5_muf2",
                   "wt_mur0p5_muf0p5",
                   ], 
                "alpha_s": [
                   "wt_alphas_up",
                   "wt_alphas_down",
                   ],      
                "pdf": [
                   "wt_pdf",
                   ],
               }

for name in sig_samples:
  f = ROOT.TFile(folder+name+file_ext)
  t = f.effective
  sum_dict = {}
  for k,v in try_weights.items(): 
    sum_dict[k] = []
    for i in v: sum_dict[k].append(0.0)

  for e in t:
    for k, v in try_weights.items():
      for ind, i in enumerate(v): 
        sum_dict[k][ind] += getattr(e ,i)

 
  for k,v in try_weights.items(): 
    dump_dict[name][k] = {}
    if len(v) != 1:
      dump_dict[name][k]["Up"] = max(sum_dict[k])/t.GetEntries()
      dump_dict[name][k]["Down"] = min(sum_dict[k])/t.GetEntries()
    else:
      dump_dict[name][k]["Up"] = sum_dict[k][0]/t.GetEntries()
      dump_dict[name][k]["Down"] = 1.0/dump_dict[name][k]["Up"]

  

with open("4tau_xs_uncerts.json", 'w') as outfile:
  json.dump(dump_dict, outfile, indent=2)
