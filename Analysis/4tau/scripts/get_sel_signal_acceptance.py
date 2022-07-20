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

charge_sel = "(q_1+q_2+q_3+q_4)==0"

baseline_sel= {
               "tttt":"({sel_1} && {sel_2} && {sel_3} && {sel_4})".format(sel_1=t_sel.replace("X","1"),sel_2=t_sel.replace("X","2"),sel_3=t_sel.replace("X","3"),sel_4=t_sel.replace("X","4")),
               "mttt":"({sel_1} && {sel_2} && {sel_3} && {sel_4})".format(sel_1=m_sel.replace("X","1"),sel_2=t_sel.replace("X","2"),sel_3=t_sel.replace("X","3"),sel_4=t_sel.replace("X","4")),
               "ettt":"({sel_1} && {sel_2} && {sel_3} && {sel_4})".format(sel_1=e_sel.replace("X","1"),sel_2=t_sel.replace("X","2"),sel_3=t_sel.replace("X","3"),sel_4=t_sel.replace("X","4")),
               "emtt":"({sel_1} && {sel_2} && {sel_3} && {sel_4})".format(sel_1=e_sel.replace("X","1"),sel_2=m_sel.replace("X","2"),sel_3=t_sel.replace("X","3"),sel_4=t_sel.replace("X","4")),
               "eett":"({sel_1} && {sel_2} && {sel_3} && {sel_4})".format(sel_1=e_sel.replace("X","1"),sel_2=e_sel.replace("X","2"),sel_3=t_sel.replace("X","3"),sel_4=t_sel.replace("X","4")),
               "mmtt":"({sel_1} && {sel_2} && {sel_3} && {sel_4})".format(sel_1=m_sel.replace("X","1"),sel_2=m_sel.replace("X","2"),sel_3=t_sel.replace("X","3"),sel_4=t_sel.replace("X","4")),
               }

files = [
#         "phi100A60To4Tau",
#         "phi200A60To4Tau",
#         "phi300A60To4Tau",
         "phi100A100To4Tau",
#         "phi200A100To4Tau",
#         "phi300A100To4Tau",
#         "phi100A150To4Tau",
#         "phi200A150To4Tau",
#         "phi300A150To4Tau",
         ]

s = "(pdgid_mother_1 == 25 || pdgid_mother_1 == 35 || pdgid_mother_1 == 36) && (pdgid_mother_2 == 25 || pdgid_mother_2 == 35 || pdgid_mother_2 == 36) && (pdgid_mother_3 == 25 || pdgid_mother_3 == 35 || pdgid_mother_3 == 36) && (pdgid_mother_4 == 25 || pdgid_mother_4 == 35 || pdgid_mother_4 == 36)"

name = "dphi"

s1 = "(((pdgid_mother_1 == pdgid_mother_2) && ((p_min_%(name)s_1 == 1 && p_min_%(name)s_2 == 2) || (p_min_%(name)s_1 == 3 && p_min_%(name)s_2 == 4))) || ((pdgid_mother_1 == pdgid_mother_3) && ((p_min_%(name)s_1 == 1 && p_min_%(name)s_2 == 3) || (p_min_%(name)s_1 == 2 && p_min_%(name)s_2 == 4))) || ((pdgid_mother_1 == pdgid_mother_4) && ((p_min_%(name)s_1 == 1 && p_min_%(name)s_2 == 4) || (p_min_%(name)s_1 == 2 && p_min_%(name)s_2 == 3))))" % vars()

output_scores = OrderedDict()

for f in files:
  output_scores[f] = OrderedDict()
  for k, v in baseline_sel.iteritems():
    file_ext = '_%(k)s_2018.root' % vars()
    fr = ROOT.TFile('%(input_folder)s/%(f)s%(file_ext)s' % vars())
    tr = fr.Get('ntuple')
    output_scores[f][k] = float(tr.GetEntries("(("+s+")&&("+s1+")&&("+baseline_sel[k]+")&&("+charge_sel+"))"))

print output_scores
