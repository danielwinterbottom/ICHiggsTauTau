from xgboost import XGBClassifier
from UserCode.ICHiggsTauTau.ff_ml_tools import *
from hep_ml.reweight import GBReweighter
import numpy
import root_numpy
import pandas
from hep_ml import reweight
from matplotlib import pyplot as plt


from hep_ml.metrics_utils import ks_2samp_weighted

columns=['pt_1','pt_2','met','mt_tot','n_jets','m_vis']

def draw_distributions(original, target, original_weights, target_weights, columns=['mt_tot'], new_original_weights=None):
  plt.figure(figsize=[15, 12])
  for id, column in enumerate(columns, 1):
    xlim = numpy.percentile(numpy.hstack([target[column]]), [0.01, 99.99])
    plt.subplot(3, 3, id)
    if new_original_weights != None:
      plt.hist(original[column], weights=new_original_weights.multiply(original_weights), range=xlim, bins=50, alpha=0.8, color="red", label="original")
    else:
      plt.hist(original[column], weights=original_weights, range=xlim, bins=50, alpha=0.8, color="red", label="original")
    plt.hist(target[column], weights=target_weights, range=xlim, bins=50, alpha=0.8, color="blue", label="target")
    plt.title(column)
    plt.legend()
  plt.show()

def KS_test(original, target, original_weights, target_weights, columns=['mt_tot'], new_original_weights=None):
  ks_dict = {}
  ks_total = 0
  for id, column in enumerate(columns, 1):
    if new_original_weights != None:
      ks_dict[column] = ks_2samp_weighted(original[column], target[column], weights1=new_original_weights.multiply(original_weights), weights2=target_weights)
    else:
      ks_dict[column] = ks_2samp_weighted(original[column], target[column], weights1=original_weights, weights2=target_weights)
    ks_total += ks_dict[column]
  return ks_total, ks_dict


baseline_dict = {}

year = "2018"
channel = "mt"

for year in ["2016","2017","2018"]:
  baseline_dict[year] = {}
  if year == "2016":
    m_lowpt=23
    e_lowpt=26
    t_highpt=120
    t_lowpt_mt=25
    t_lowpt_et=25
  elif year == "2017":
    m_lowpt=25
    e_lowpt=28
    t_highpt=180
    t_lowpt_mt=32
    t_lowpt_et=35
  elif year == "2018":
    m_lowpt=25
    e_lowpt=33
    t_highpt=180
    t_lowpt_mt=32
    t_lowpt_et=35

  baseline_dict[year]["mt"] = '(gen_match_2==6 && os==1 && iso_1<0.15 && deepTauVsJets_vvvloose_2>0.5 && deepTauVsEle_vvloose_2>0.5 && deepTauVsMu_tight_2>0.5 && leptonveto==0 && pt_2>30 && ((trg_mutaucross==1&&pt_2>%(t_lowpt_mt)s&&pt_2<%(t_highpt)s&&fabs(eta_2)<2.1&&pt_1<%(m_lowpt)s)||(trg_singlemuon==1&&pt_1>=%(m_lowpt)s)||(trg_singletau_2==1&&pt_2>=%(t_highpt)s&&fabs(eta_2)<2.1)))' % vars()
  baseline_dict[year]["et"] = '(gen_match_2==6 && os==1 && iso_1<0.15 && deepTauVsJets_vvvloose_2>0.5 && deepTauVsEle_tight_2>0.5 && deepTauVsMu_vloose_2>0.5 && pt_2>30 && ((trg_etaucross==1&&pt_2>%(t_lowpt_et)s&&pt_2<%(t_highpt)s&&fabs(eta_2)<2.1&&pt_1<%(e_lowpt)s)||(trg_singleelectron==1&&pt_1>=%(e_lowpt)s)||(trg_singletau_2==1&&pt_2>=%(t_highpt)s&&fabs(eta_2)<2.1)))' % vars()



X_vars = [
          "rho",
          "pt_1",
          "pt_2",
          "jet_pt_2/pt_2",
          "n_jets",
          "n_deepbjets",
          "tau_decay_mode_2",
          "newmet*cos(newmet_dphi_2)",
          "trg_etaucross",
          "trg_mutaucross",
          "dR",
          "dphi",
          "eta_1",
          "eta_2",
          "wt",
          "wt_tau_trg_mssm",
          "wt_tau_id_mssm",
          "met",
          "mt_tot",
          "m_vis",
          #"iso_1",
          ]

y_var = "deepTauVsJets_medium_2"

root_files = [
              'W1JetsToLNu-LO',
              'W2JetsToLNu-LO',
              'W3JetsToLNu-LO',
              'W4JetsToLNu-LO',
              'WJetsToLNu-LO',
              ]


# Set up dataframe
df_list = []
loc = "/vols/cms/dw515/Offline/output/MSSM/mssm_%(year)s_v2/" % vars()
df = setup_dataframe(loc,root_files,channel,year,X_vars+[y_var],baseline_dict[year][channel])

X, y = df.drop([y_var],axis=1),df.loc[:,y_var]
X_train, X_test, y_train, y_test = train_test_split(X, y, test_size=0.5, random_state=123)

### TRAINING DATASET ###
train = pd.concat([X_train, y_train], axis=1)

# Get event weights
original_weights_train = train[(train[y_var]==0)].loc[:,"wt"].multiply(train[(train[y_var]==0)].loc[:,"wt_tau_trg_mssm"]).multiply(train[(train[y_var]==0)].loc[:,"wt_tau_id_mssm"])
target_weights_train = train[(train[y_var]==1)].loc[:,"wt"].multiply(train[(train[y_var]==1)].loc[:,"wt_tau_trg_mssm"]).multiply(train[(train[y_var]==1)].loc[:,"wt_tau_id_mssm"])

# Normalise event weights
original_weights_train_norm =  original_weights_train/original_weights_train.sum()
target_weights_train_norm = target_weights_train/target_weights_train.sum()

# Organise dataframe
original_train = train[(train[y_var]==0)].drop([y_var],axis=1).drop(["wt"],axis=1).drop(["wt_tau_trg_mssm"],axis=1).drop(["wt_tau_id_mssm"],axis=1)
target_train = train[(train[y_var]==1)].drop([y_var],axis=1).drop(["wt"],axis=1).drop(["wt_tau_trg_mssm"],axis=1).drop(["wt_tau_id_mssm"],axis=1)

### TESTING DATASET ##
test = pd.concat([X_test, y_test], axis=1)

# Get event weights
original_weights_test = test[(test[y_var]==0)].loc[:,"wt"].multiply(test[(test[y_var]==0)].loc[:,"wt_tau_trg_mssm"]).multiply(test[(test[y_var]==0)].loc[:,"wt_tau_id_mssm"])
target_weights_test = test[(test[y_var]==1)].loc[:,"wt"].multiply(test[(test[y_var]==1)].loc[:,"wt_tau_trg_mssm"]).multiply(test[(test[y_var]==1)].loc[:,"wt_tau_id_mssm"])

# Normalise event weights
original_weights_test_norm = original_weights_test/original_weights_test.sum()
target_weights_test_norm = target_weights_test/target_weights_test.sum()

# Organise dataframe
original_test = test[(test[y_var]==0)].drop([y_var],axis=1).drop(["wt"],axis=1).drop(["wt_tau_trg_mssm"],axis=1).drop(["wt_tau_id_mssm"],axis=1)
target_test = test[(test[y_var]==1)].drop([y_var],axis=1).drop(["wt"],axis=1).drop(["wt_tau_trg_mssm"],axis=1).drop(["wt_tau_id_mssm"],axis=1)

### FIT MODEL ###
reweighter = reweight.GBReweighter(n_estimators=35, learning_rate=0.1, max_depth=15, min_samples_leaf=1100, gb_args={'subsample': 0.4})
reweighter.fit(original_train, target_train, original_weight=original_weights_train_norm ,target_weight=target_weights_train_norm)

gb_weights_test = reweighter.predict_weights(original_test)
original_test.loc[:,"wt_ff_ml"] = gb_weights_test
original_test.loc[:,"wt_ff_ml"] = original_test.loc[:,"wt_ff_ml"]*(target_weights_test_norm.sum()/original_weights_test_norm.multiply(original_test.loc[:,"wt_ff_ml"]).sum())
original_test.loc[:,"wt_ff_ml"] = original_test.loc[:,"wt_ff_ml"]*(target_weights_test.sum()/original_weights_test.sum())

### ADD VARIABLES AFTER TRAINING ###
#add_var = [
#          "wt_ff_mssm_wjets_1",
#          "wt",
#          "wt_tau_trg_mssm",
#          "wt_tau_id_mssm",
#          ]
#df_2 = setup_dataframe(loc,root_files,channel,year,add_var,baseline_dict[year][channel])

#original_test = pd.concat([original_test, df_2.reindex(original_test.index)], axis=1,sort=True)

print "-------------------------------------------------------"
print "ML FF"
print "-------------------------------------------------------"
#draw_distributions(original_test, target_test, original_weights_test_norm.multiply(original_test.loc[:,"wt_ff_ml"]),target_weights_test_norm)
print KS_test(original_test, target_test, original_weights_test_norm.multiply(original_test.loc[:,"wt_ff_ml"]),target_weights_test_norm)
draw_distributions(original_test, target_test, original_weights_test.multiply(original_test.loc[:,"wt_ff_ml"]),target_weights_test)

#print "-------------------------------------------------------"
#print "MSSM FF"
#print "-------------------------------------------------------"
#print KS_test(original_test, target_test, original_weights_test_norm.multiply(original_test.loc[:,"wt_ff_mssm_wjets_1"]),target_weights_test_norm)
#draw_distributions(original_test, target_test, original_weights_test.multiply(original_test.loc[:,"wt_ff_mssm_wjets_1"]),target_weights_test)

#target_test = pd.concat([target_test, df_2.reindex(target_test.index)], axis=1,sort=True)

#original_test.loc[:,"deepTauVsJets_medium_2"] = 0
#target_test.loc[:,"deepTauVsJets_medium_2"] = 1
#target_test.loc[:,"wt_ff_ml"] = 0

#out_df = pd.concat([original_test,target_test],axis=0,sort=True)
#print out_df
#to_root(out_df, "WJets_ff_ml_{}_{}.root".format(channel,year), key='ntuple')
