import ROOT
from collections import OrderedDict
from prettytable import PrettyTable

#input_folder = "/vols/cms/gu18/Offline/output/4tau/2018_2006_trig_v2"
input_folder = "/vols/cms/gu18/Offline/output/4tau/2018_1307_sigmass"


VsJets_wp = "vvvloose"
VsEle_wp = "vloose"
VsMu_wp = "vloose"
lepton_iso = "0.15"

t_sel = "deepTauVsJets_%(VsJets_wp)s_X>0.5 && deepTauVsMu_%(VsMu_wp)s_X>0.5 && deepTauVsEle_%(VsEle_wp)s_X>0.5" % vars()
e_sel = "iso_X<%(lepton_iso)s" % vars()
m_sel = "iso_X<%(lepton_iso)s" % vars()

dR_cut = 0.1

charge_sel = "((q_1+q_2+q_3+q_4)==0)"

baseline_sel= {
               "tttt":"({sel_1} && {sel_2} && {sel_3} && {sel_4})".format(sel_1=t_sel.replace("X","1"),sel_2=t_sel.replace("X","2"),sel_3=t_sel.replace("X","3"),sel_4=t_sel.replace("X","4")),
               "mttt":"({sel_1} && {sel_2} && {sel_3} && {sel_4})".format(sel_1=m_sel.replace("X","1"),sel_2=t_sel.replace("X","2"),sel_3=t_sel.replace("X","3"),sel_4=t_sel.replace("X","4")),
               "ettt":"({sel_1} && {sel_2} && {sel_3} && {sel_4})".format(sel_1=e_sel.replace("X","1"),sel_2=t_sel.replace("X","2"),sel_3=t_sel.replace("X","3"),sel_4=t_sel.replace("X","4")),
               "emtt":"({sel_1} && {sel_2} && {sel_3} && {sel_4})".format(sel_1=e_sel.replace("X","1"),sel_2=m_sel.replace("X","2"),sel_3=t_sel.replace("X","3"),sel_4=t_sel.replace("X","4")),
               "eett":"({sel_1} && {sel_2} && {sel_3} && {sel_4})".format(sel_1=e_sel.replace("X","1"),sel_2=e_sel.replace("X","2"),sel_3=t_sel.replace("X","3"),sel_4=t_sel.replace("X","4")),
               "mmtt":"({sel_1} && {sel_2} && {sel_3} && {sel_4})".format(sel_1=m_sel.replace("X","1"),sel_2=m_sel.replace("X","2"),sel_3=t_sel.replace("X","3"),sel_4=t_sel.replace("X","4")),
               }

files = [
         "phi100A60To4Tau",
         "phi200A60To4Tau",
         "phi300A60To4Tau",
         "phi100A100To4Tau",
         "phi200A100To4Tau",
         "phi300A100To4Tau",
         "phi100A150To4Tau",
         "phi200A150To4Tau",
         "phi300A150To4Tau",
         ]

#files = ["TTToHadronic"]

#files = ["DYJetsToLL-LO"]

files = [
         "phi100A100To4Tau",
         "phi200A100To4Tau",
         "phi300A100To4Tau",
#         "TTToHadronic",
         "DYJetsToLL-LO",
         ]


conversions = {
               "||": " or ", 
               "trg_doubletau_12": "$\\tau_{h}^{1}\\tau_{h}^{2}$",
               "trg_doubletau_13": "$\\tau_{h}^{1}\\tau_{h}^{3}$",
               "trg_doubletau_14": "$\\tau_{h}^{1}\\tau_{h}^{4}$",
               "trg_doubletau_23": "$\\tau_{h}^{2}\\tau_{h}^{3}$",
               "trg_doubletau_24": "$\\tau_{h}^{2}\\tau_{h}^{4}$",
               "trg_doubletau_34": "$\\tau_{h}^{3}\\tau_{h}^{4}$",
               "trg_singlemuon_1": "$\\mu^{1}$",
               "trg_singlemuon_2": "$\\mu^{2}$",
               "trg_singleelectron_1": "$e^{1}$",
               "trg_singleelectron_2": "$e^{2}$",
               "trg_mutaucross_12": "$\\mu^{1}\\tau_{h}^{2}$",
               "trg_mutaucross_13": "$\\mu^{1}\\tau_{h}^{3}$",
               "trg_mutaucross_14": "$\\mu^{1}\\tau_{h}^{4}$",
               "trg_mutaucross_23": "$\\mu^{2}\\tau_{h}^{3}$",
               "trg_mutaucross_24": "$\\mu^{2}\\tau_{h}^{4}$",
               "trg_etaucross_12": "$e^{1}\\tau_{h}^{2}$",
               "trg_etaucross_13": "$e^{1}\\tau_{h}^{3}$",
               "trg_etaucross_14": "$e^{1}\\tau_{h}^{4}$",
               "trg_etaucross_23": "$e^{2}\\tau_{h}^{3}$",
               "trg_etaucross_24": "$e^{2}\\tau_{h}^{4}$",
               "trg_emucross_12": "$e^{1}\\mu^{1}$",
               "tttt": "$\\tau_{h}\\tau_{h}\\tau_{h}\\tau_{h}$",
               "mttt": "$\\mu\\tau_{h}\\tau_{h}\\tau_{h}$",
               "ettt": "$e\\tau_{h}\\tau_{h}\\tau_{h}$",
               "mmtt": "$\\mu\\mu\\tau_{h}\\tau_{h}$",
               "eett": "$ee\\tau_{h}\\tau_{h}$",
               "emtt": "$e\\mu\\tau_{h}\\tau_{h}$",
               }

test_dict = OrderedDict()

test_dict["tttt"] = [
                     "trg_doubletau_12",
                     "trg_doubletau_12 || trg_doubletau_13",
                     "trg_doubletau_12 || trg_doubletau_13 || trg_doubletau_14",
                     "trg_doubletau_12 || trg_doubletau_13 || trg_doubletau_14 || trg_doubletau_23",
                     "trg_doubletau_12 || trg_doubletau_13 || trg_doubletau_14 || trg_doubletau_23 || trg_doubletau_24",
                     "trg_doubletau_12 || trg_doubletau_13 || trg_doubletau_14 || trg_doubletau_23 || trg_doubletau_24 || trg_doubletau_34",
                     ]
test_dict["mttt"] = [
                     "trg_singlemuon_1",
                     "trg_singlemuon_1 || trg_doubletau_23",
                     "trg_singlemuon_1 || trg_doubletau_23 || trg_doubletau_24",
                     "trg_singlemuon_1 || trg_doubletau_23 || trg_doubletau_24 || trg_doubletau_34",
                     "trg_singlemuon_1 || trg_doubletau_23 || trg_doubletau_24 || trg_doubletau_34 || trg_mutaucross_12 || trg_mutaucross_13 || trg_mutaucross_14",
                     "trg_doubletau_23 || trg_doubletau_24 || trg_doubletau_34",
                     "trg_mutaucross_12 || trg_mutaucross_13 || trg_mutaucross_14",
                     "trg_mutaucross_12 || trg_mutaucross_13 || trg_mutaucross_14 || trg_doubletau_23 || trg_doubletau_24 || trg_doubletau_34",
                     ] 
test_dict["ettt"] = [
                     "trg_singleelectron_1",
                     "trg_singleelectron_1 || trg_doubletau_23",
                     "trg_singleelectron_1 || trg_doubletau_23 || trg_doubletau_24",
                     "trg_singleelectron_1 || trg_doubletau_23 || trg_doubletau_24 || trg_doubletau_34",
                     "trg_singleelectron_1 || trg_doubletau_23 || trg_doubletau_24 || trg_doubletau_34 || trg_etaucross_12 || trg_etaucross_13 || trg_etaucross_14",
                     "((trg_singleelectron_1 || trg_doubletau_23 || trg_doubletau_24 || trg_doubletau_34) && pt_1>18) || ((trg_etaucross_12 || trg_etaucross_13 || trg_etaucross_14) && pt_1<18)",
                     "trg_doubletau_23 || trg_doubletau_24 || trg_doubletau_34",
                     "trg_etaucross_12 || trg_etaucross_13 || trg_etaucross_14",
                     "trg_etaucross_12 || trg_etaucross_13 || trg_etaucross_14 || trg_doubletau_23 || trg_doubletau_24 || trg_doubletau_34",
                     ]
test_dict["emtt"] = [
                     "trg_singleelectron_1 || trg_singlemuon_2",
                     "trg_singleelectron_1 || trg_singlemuon_2 || trg_doubletau_34",
                     "trg_singleelectron_1 || trg_singlemuon_2 || trg_doubletau_34 || trg_emucross_12",
                     "trg_emucross_12",
                     "trg_emucross_12 || trg_doubletau_34",
                     "trg_doubletau_34",
                     ]
test_dict["eett"] = [
                     "trg_singleelectron_1",
                     "trg_singleelectron_1 || trg_singleelectron_2",
                     "trg_singleelectron_1 || trg_singleelectron_2 || trg_doubletau_34",
                     "trg_singleelectron_1 || trg_singleelectron_2 || trg_doubletau_34 || trg_etaucross_13 || trg_etaucross_14 || trg_etaucross_23 || trg_etaucross_24",
                     "trg_doubletau_34",
                     ]
test_dict["mmtt"] = [
                     "trg_singlemuon_1",
                     "trg_singlemuon_1 || trg_singlemuon_2",
                     "trg_singlemuon_1 || trg_singlemuon_2 || trg_doubletau_34",
                     "trg_singlemuon_1 || trg_singlemuon_2 || trg_doubletau_34 || trg_mutaucross_13 || trg_mutaucross_14 || trg_mutaucross_23 || trg_mutaucross_24",
                     "trg_doubletau_34",
                     ]


output_scores = OrderedDict()

for f in files:
  output_scores[f] = OrderedDict()
  for k, v in test_dict.iteritems():
    file_ext = '_%(k)s_2018.root' % vars()
    fr = ROOT.TFile('%(input_folder)s/%(f)s%(file_ext)s' % vars())
    tr = fr.Get('ntuple')
    output_scores[f][k] = OrderedDict()
    for s in v:
      output_scores[f][k][s] = round(float(tr.GetEntries("(("+s+")&&("+baseline_sel[k]+")&&("+charge_sel+"))"))/float(tr.GetEntries("(("+baseline_sel[k]+")&&("+charge_sel+"))")),2)

 
t = PrettyTable(["Channel","Selection"]+files) 
for k, v in test_dict.iteritems():
  data = []
  for i,s in enumerate(v):
    if i == 0:
      data.append([k,s.replace(" ","")])
    else:
      data.append(["",s.replace(" ","")])
    for f in files:
      data[i].append(output_scores[f][k][s])
    t.add_row(data[i])
    if i == len(v)-1:
      t.add_row([""]*(len(files)+2))

  #for ki in data:
  #  if ki[0] != "":
  #    print "\\hline"

  #  channel_name = ki[0]
  #  for kk, kv in conversions.items():
  #    channel_name = channel_name.replace(kk,kv)

  #  trigger_name = ki[1]
  #  for kk, kv in conversions.items():
  #    trigger_name = trigger_name.replace(kk,kv)
  #  print "\multicolumn{1}{|l|}{"+channel_name+"} & \multicolumn{2}{l||}{"+trigger_name+"} & \multicolumn{1}{l|}{"+str(ki[2])+"} & \multicolumn{1}{l|}{"+str(ki[3])+"} & \multicolumn{1}{l|}{"+str(ki[4])+"} & \multicolumn{1}{l|}{"+str(ki[5])+"} & \multicolumn{1}{l|}{"+str(ki[6])+"} & \multicolumn{1}{l|}{"+str(ki[7])+"} & \multicolumn{1}{l|}{"+str(ki[8])+"} & \multicolumn{1}{l|}{"+str(ki[9])+"} & \multicolumn{1}{l|}{"+str(ki[10])+"} \\\\"

     
t.align["Channel"] = "l"
t.align["Selection"] = "l"

print t

