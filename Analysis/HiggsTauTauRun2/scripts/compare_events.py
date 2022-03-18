import ROOT

file_1 = "/vols/cms/gu18/Offline/output/MSSM/vlq_2018_matched_v2/VectorLQToTauTau_betaRd33_0_mU1000_gU1_matched_tt_2018.root"
ntuple_name_1 = "ntuple"

file_2 = "/vols/cms/gu18/Offline/output/MSSM/vlq_2018_matched_gen/VectorLQToTauTau_betaRd33_0_mU1000_gU1_matched_tt_2018.root"
ntuple_name_2 = "gen_ntuple"

file_1 = "/vols/cms/dw515/Offline/output/MSSM/mssm_2018_v2/GluGluHToTauTau_M-125_tt_2018.root"


f_1 = ROOT.TFile(file_1)
t_1 = f_1.Get(ntuple_name_1)

l = []
for entry in t_1:
  l.append(entry.ngenjets)

print l.count(0)
print l.count(1)
print l.count(2)
print l.count(3)
print l.count(4)
print l.count(5)



#f_2 = ROOT.TFile(file_2)
#t_2 = f_2.Get(ntuple_name_2)
#
#if t_1.GetEntries() >= t_2.GetEntries():
#  t_i = t_2
#  t_f = t_1
#else:
#  t_i = t_1
#  t_f = t_2
#
#event_dict = {}
#
#
#for entry in t_i:
#  event_dict[entry.event] = [entry.ngenjets]
#  
#
#for entry in t_f:
#  if entry.event in event_dict.keys():
#    event_dict[entry.event].append(entry.n_jets)
#
#for entry in t_i:
#  if len(event_dict[entry.event]) == 1:
#     del event_dict[entry.event]
#
#l_1 = []
#l_2 = []
#for key, val in event_dict.items():
#  l_1.append(val[0]) 
#  l_2.append(val[1])
#
#print l_1.count(0),l_2.count(0)
#print l_1.count(1),l_2.count(1)
#print l_1.count(2),l_2.count(2)
#print l_1.count(3),l_2.count(3)
#print l_1.count(4),l_2.count(4)

