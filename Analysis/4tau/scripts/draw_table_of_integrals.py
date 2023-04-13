import ROOT
from collections import OrderedDict
import argparse

parser = argparse.ArgumentParser()
parser.add_argument('--folder', help= 'Name of folder datacards are in', default='')
parser.add_argument('--cnz', help= 'Run charges non zero', action='store_true')
args = parser.parse_args()


if args.cnz:
  folder = args.folder+"/CHANNEL/mt_tot_ff_full_CAT_all.root"
  show_data = True
  unblind = []
  show_other = []
  replace_show_other = []
else:
  folder = args.folder+"/CHANNEL/mt_tot_signal_CAT_all.root"
  show_data = False
  unblind = ["mmtt_z_control_nobtag","eett_z_control_nobtag"]
  show_other = ["A160phi300"]
  replace_show_other = ["$(m_{A},m_{\\phi}) = (160,300)$ GeV"]

files = OrderedDict()

files['$\\tau_{h}\\tau_{h}\\tau_{h}\\tau_{h}$'] = 'tttt_inclusive'
files['$\\tau_{h}\\tau_{h}\\tau_{h}$'] = 'ttt_inclusive'
files['$e\\tau_{h}\\tau_{h}\\tau_{h}$'] = 'ettt_nobtag'
files['$\\mu\\tau_{h}\\tau_{h}\\tau_{h}$'] = 'mttt_nobtag'
files['$ee\\tau_{h}\\tau_{h}$ Opposite Sign Leptons'] = 'eett_z_control_nobtag'
files['$ee\\tau_{h}\\tau_{h}$ Same Sign Leptons'] = 'eett_2l2t_sig_nobtag'
files['$\\mu\\mu\\tau_{h}\\tau_{h}$ Opposite Sign Leptons'] = 'mmtt_z_control_nobtag'
files['$\\mu\\mu\\tau_{h}\\tau_{h}$ Same Sign Leptons'] = 'mmtt_2l2t_sig_nobtag'
files['$e\\mu\\tau_{h}\\tau_{h}$ Opposite Sign Leptons'] = 'emtt_z_control_nobtag'
files['$e\\mu\\tau_{h}\\tau_{h}$ Same Sign Leptons'] = 'emtt_2l2t_sig_nobtag'


def GetIntegralOfVariation(hist):
  val = 0.0
  for i in range(0,hist.GetNbinsX()+1):
    val += (hist.GetBinError(i) ** 2)
  val = val ** 0.5
  return hist.Integral() - val,  hist.Integral() + val

 

print "\\begin{table}[]"
print "\\centering"
table_align = "|l||c|c|"
if show_data or unblind != []:
  table_align += "c|"
for i in show_other:
  table_align += "c|"

print "\\begin{tabular}{"+table_align+"}"
print "\\hline"
header = "  Category & $N_{\\text{bins}}$ & Total Background"
empty = " & &"
if show_data or unblind != []:
  header += " & Data Observed"
  empty += " &"
for i in replace_show_other:
  header += " & " + i
  empty += " &"

empty += " \\\\"
header += " \\\\ \\hline \\hline" 
print header


for k, v in files.iteritems():
  name = folder.replace("CAT",v).replace("CHANNEL",v.split("_")[0])
  f = ROOT.TFile(name)
  tb = f.Get(v+"/total_bkg")
  tbi = tb.Integral()

  tbstatdown, tbstatup = GetIntegralOfVariation(tb)  

  stat = tbi-tbstatdown

  tbup = f.Get(v+"/total_bkg_custom_uncerts_up")
  tbupi = tbup.Integral()
  tbdown = f.Get(v+"/total_bkg_custom_uncerts_down")
  tbdowni = tbdown.Integral()
  if show_data or v in unblind:
    do = f.Get(v+"/data_obs")
    doi = str(int(do.Integral()))
  else:
    doi = "-"
  other = []
  otheri = []
  for ind, i in enumerate(show_other):
    other.append(f.Get(v+"/"+i))
    otheri.append(other[ind].Integral())
 
  #debug
  #print "File:", name
  #print "Nom:", tbi
  #print "Nom+stat:", tbstatup
  #print "Nom-stat:", tbstatdown
  #print "TotalUp:", tbupi
  #print "TotalDown:", tbdowni
  
  print empty
  row = " " + k
  row += " & "
  row += str(tb.GetNbinsX())
  row += " & "
  row += "$" + str(round(tbi,1))  + "$ $\\pm$ $" + str(round(stat,1)) + "$ (stat) "  + "$^{+" + str(round(tbupi-tbi,1)) +  "}_{-" + str(round(tbi-tbdowni,1)) + "}$ (syst)"
  if show_data or unblind != []:
    row += " & "
    row += "$" + doi + "$"
  for ind, i in enumerate(show_other):
    row += " & "
    row += "$" + str(round(otheri[ind],1)) + "$"

  row += "\\\\"
  print row

print empty
print "\\hline"
print "\\end{tabular}"
print "\\end{table}"


