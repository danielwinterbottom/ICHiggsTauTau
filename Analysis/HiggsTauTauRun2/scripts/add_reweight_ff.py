import uproot
import pandas
import pickle
from UserCode.ICHiggsTauTau.ff_ml_tools import SelectColumns,to_root

loc = "/vols/cms/gu18/Offline/output/MSSM/mssm_2017_v10"
newloc = "./"
file_name = "SingleMuonD"

columns = ["pt_1","pt_2","jet_pt_2","jet_pt_2/pt_2","n_jets","n_deepbjets","tau_decay_mode_2","trg_etaucross","dR","dphi","pt_1*cosh(eta_1)","pt_2*cosh(eta_2)","met","rho"]
#columns = ["trg_mutaucross","dphi","mva_dm_2","pt_2","pt_1","met","rho","n_jets","jet_pt_2","dR","n_deepbjets","pt_2*cosh(eta_2)","pt_1*cosh(eta_1)","jet_pt_2/pt_2"] 
weights = ["wt","wt_tau_trg_mssm","wt_tau_id_mssm"]

channel = "mt"
year = "2017"
file_name = "{}_{}_{}.root".format(file_name,channel,year)


norm = {"et": {"2016": {"qcd":0.17, "wjets":0.17, "ttbar":0.17}, "2017":{"qcd":0.131450286969, "wjets":0.254104017874, "ttbar":0.130039812167}, "2018":{"qcd":0.17, "wjets":0.17, "ttbar":0.17}},
        "mt": {"2016":{"qcd":0.17, "wjets":0.17, "ttbar":0.17}, "2017":{"qcd":0.17, "wjets":0.17, "ttbar":0.17}, "2018":{"qcd":0.17, "wjets":0.17, "ttbar":0.17}}}

tree = uproot.open(loc+'/'+file_name)["ntuple"]
j = 0
for small_tree in tree.iterate(entrysteps=10000):
  print j
  #df = small_tree.pandas.df(tree.keys())
  df = pandas.DataFrame.from_dict(small_tree)
  
  for i in weights:
    if i == weights[0]:
      total_weights = df.loc[:,i]
    else:
      total_weights = total_weights.multiply(df.loc[:,i])
  
  new_df = SelectColumns(df,columns)
  
  
  wjets_reweighter = pickle.load(open("wjets_mc_reweighted_ff_{}_{}.sav".format(channel,year), 'rb'))
  wjets_out = wjets_reweighter.predict_weights(new_df,total_weights)
  df.loc[:,"wt_ff_reweight_wjets_1"] = wjets_out*norm[channel][year]["wjets"]
  
  #print df.loc[:,["wt_ff_reweight_wjets_1","wt_ff_mssm_wjets_1","jet_pt_2","tau_decay_mode_2","mva_dm_2"]]
  
  #qcd_reweighter = pickle.load(open("qcd_reweighted_ff_{}_{}.sav".format(channel,year), 'rb'))
  #qcd_out = qcd_reweighter.predict_weights(new_df,weights)
  #df.loc[:,"wt_ff_reweight_qcd_1"] = qcd_out*norm[channel][year]["qcd"]
  
  #ttbar_reweighter = pickle.load(open("ttbar_reweighted_ff_{}_{}.sav".format(channel,year), 'rb'))
  #ttbar_out = ttbar_reweighter.predict_weights(new_df,weights)
  #df.loc[:,"wt_ff_reweight_ttbar_1"] = ttbar_out*norm[channel][year]["qcd"]
  
  to_root(df, file_name.replace(".root","_"+str(j)+".root"), key='ntuple')
  
  del df, new_df, wjets_reweighter, total_weights, small_tree
  j += 1
