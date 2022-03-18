from xgboost import XGBClassifier
from UserCode.ICHiggsTauTau.ff_ml_tools import *
from hep_ml.reweight import GBReweighter
import numpy
import root_numpy
import pandas
from hep_ml import reweight
from matplotlib import pyplot as plt


from hep_ml.metrics_utils import ks_2samp_weighted

columns=['pt_1','pt_2','met','mt_tot','n_jets','m_vis','tau_decay_mode_2','dR','eta_2']

def draw_distributions(original, target, new_original_weights):
  plt.figure(figsize=[15, 12])
  for id, column in enumerate(columns, 1):
    xlim = numpy.percentile(numpy.hstack([target[column]]), [0.01, 99.99])
    plt.subplot(3, 3, id)
    plt.hist(original[column], weights=new_original_weights, range=xlim, bins=50, alpha=0.8, color="red", label="original")
    plt.hist(target[column], range=xlim, bins=50, alpha=0.8, color="blue", label="target")
    plt.title(column)
    plt.legend()
    print 'KS over ', column, ' = ', ks_2samp_weighted(original[column], target[column], weights1=new_original_weights, weights2=numpy.ones(len(target), dtype=float))
  plt.show()

baseline_dict = {}

years = ["2018"]
channels = ["et"]

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

  baseline_dict[year]["mt"] = '(iso_1<0.15 && deepTauVsJets_vvvloose_2>0.5 && deepTauVsEle_vvloose_2>0.5 && deepTauVsMu_tight_2>0.5 && leptonveto==0 && pt_2>30 && ((trg_mutaucross==1&&pt_2>%(t_lowpt_mt)s&&pt_2<%(t_highpt)s&&fabs(eta_2)<2.1&&pt_1<%(m_lowpt)s)||(trg_singlemuon==1&&pt_1>=%(m_lowpt)s)||(trg_singletau_2==1&&pt_2>=%(t_highpt)s&&fabs(eta_2)<2.1)))' % vars()
  baseline_dict[year]["et"] = '(iso_1<0.15 && deepTauVsJets_vvvloose_2>0.5 && deepTauVsEle_tight_2>0.5 && deepTauVsMu_vloose_2>0.5 && pt_2>30 && ((trg_etaucross==1&&pt_2>%(t_lowpt_et)s&&pt_2<%(t_highpt)s&&fabs(eta_2)<2.1&&pt_1<%(e_lowpt)s)||(trg_singleelectron==1&&pt_1>=%(e_lowpt)s)||(trg_singletau_2==1&&pt_2>=%(t_highpt)s&&fabs(eta_2)<2.1)))' % vars()



X_vars = [
          #"rho",
          "pt_1",
          "pt_2",
          "jet_pt_2/pt_2",
          "n_jets",
          "n_deepbjets",
          "tau_decay_mode_2",
          #"tau_decay_mode_2==0",
          #"tau_decay_mode_2==1",
          #"tau_decay_mode_2==10",
          #"tau_decay_mode_2==11",
          #"tau_decay_mode_2",
          "eta_2",
          #"newmet*cos(newmet_dphi_2)",
          #"trg_etaucross",
          #"trg_mutaucross",
          "m_vis",
          "mt_tot",
          "dR",
          "dphi",
          "eta_1",
          "eta_2",
          "wt",
          "met"
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
for year in years:
  for channel in channels:
    #print ">> Adding Channel:{}, Year:{}".format(channel,year)
    loc = "/vols/cms/dw515/Offline/output/MSSM/mssm_%(year)s_v2/" % vars()
    df_temp = setup_dataframe(loc,root_files,channel,year,X_vars+[y_var],baseline_dict[year][channel])
    if "2016" in years and len(years)>1:  df_temp.loc[:,"2016"] = 1 if year == "2016" else 0
    if "2017" in years and len(years)>1:  df_temp.loc[:,"2017"] = 1 if year == "2017" else 0
    if "2018" in years and len(years)>1:  df_temp.loc[:,"2018"] = 1 if year == "2018" else 0
    if "mt" in channels and len(channels)>1:  df_temp.loc[:,"mt"] = 1 if channel == "mt" else 0
    if "et" in channels and len(channels)>1:  df_temp.loc[:,"et"] = 1 if channel == "et" else 0
    df_list.append(df_temp)
df = pd.concat(df_list, axis=0,ignore_index=True) 

#df = setup_dataframe(loc,root_files,channel,year,X_vars+[y_var],selection)
X, y = df.drop([y_var],axis=1),df.loc[:,y_var]

#print ">> Splitting test and train data"
X_train, X_test, y_train, y_test = train_test_split(X, y, test_size=0.5, random_state=123)
#X_train, y_train = equal_y_events(X_train,y_train,y_var,y_vals=[0,1])

train = pd.concat([X_train, y_train], axis=1)
original_train = train[(train[y_var]==0)].drop([y_var],axis=1).drop(["wt"],axis=1) 
original_weights_train = train[(train[y_var]==0)].loc[:,"wt"]

target_train = train[(train[y_var]==1)].drop([y_var],axis=1).drop(["wt"],axis=1)

test = pd.concat([X_test, y_test], axis=1)
original_test = test[(test[y_var]==0)].drop([y_var],axis=1).drop(["wt"],axis=1)
target_test = test[(test[y_var]==1)].drop([y_var],axis=1).drop(["wt"],axis=1)

#original_weights_train = len(target_train)*numpy.ones(len(original_train))/len(original_train)

print len(original_train), len(target_train)

#print ">> Example of training dataset"
#print pd.concat([X_train, y_train], axis=1).head() 
dtrain = xgb.DMatrix(X_train, label=y_train)
dtest = xgb.DMatrix(X_test, label=y_test)
	
xgb_params = {
    "objective": "multi:softprob",
    "eval_metric": "auc",
    "max_depth": 5,
    "learning_rate": 0.08,
    "silent": 1,
    "n_estimators": 100,
    "subsample": 0.5,
    "n_jobs":10,
    "num_class":2,
    "colsample_bytree":0.8,
    "gamma":1
}

num_round = 500

#print ">> Performing training"
#xgb_ff = xgb.train(xgb_params, dtrain, num_round)
#print ">> Finished training"
#
#print ">> Feature importance:"
#print "{:<40} {:<15}".format('Feature','Importance')
#for k, v in xgb_ff.get_score(importance_type='gain').iteritems():
#    print "{:<40} {:<15}".format(k, v)
#
##pickle.dump(xgb_ff, open("ff_ml.dat", "wb"))
#
#print ">> Applying model to test"
#y_pred = xgb_ff.predict(dtest)
#ff = np.divide(y_pred[:,1],y_pred[:,0]) 
#
#out_df = X_test
#
#print ">> Adding new fake factors"
#out_df.loc[:,y_var] = y_test
#out_df.loc[:,"wt_ff_ml"] = ff
#
#
#print ">> Example of output"
#print out_df

draw_distributions(original_train, target_train, original_weights_train)

reweighter = reweight.GBReweighter(n_estimators=200, learning_rate=0.1, max_depth=5, min_samples_leaf=1000, 
                                   gb_args={'subsample': 0.5})
reweighter.fit(original_train, target_train)

gb_weights_test = reweighter.predict_weights(original_test)
#original_test.loc[:,"wt_ff_ml"] = len(target_train)*gb_weights_test/len(original_train)
original_test.loc[:,"wt_ff_ml"] = gb_weights_test
# validate reweighting rule on the test part comparing 1d projections
draw_distributions(original_test, target_test, original_test.loc[:,"wt_ff_ml"].multiply())

print original_test
#gb = GBReweighter(learning_rate=0.08,max_depth=5,n_estimators=100)
#gb.fit(X_train_fail, X_train_med)
#output = gb.predict_weights(X_test_fail)
#X_test_fail.loc[:,"wt_ff_ml"] = output

#print X_test_fail

# Add variables for plotting 
#vars_2 = [
#          "wt_ff_mssm_wjets_1",
#          "mt_tot",
#          "wt",
#          "wt_tau_trg_mssm",
#          "wt_tau_id_mssm",
#          "os",
#          "met",
#          #"m_vis",
#          "mt_tot"
#          ]
#
#df_list_2 = []
#for year in years:
#  for channel in channels:
#    loc = "/vols/cms/dw515/Offline/output/MSSM/mssm_%(year)s_v2/" % vars()
#    df_temp = setup_dataframe(loc,root_files,channel,year,vars_2,baseline_dict[year][channel])
#    df_list_2.append(df_temp)
#df_2 = pd.concat(df_list_2, axis=0,ignore_index=True)
#out_df = pd.concat([out_df, df_2.reindex(out_df.index)], axis=1,sort=True)
#
#for year in ["2016","2017","2018"]:
#  for channel in ["mt","et"]:
#    to_root(out_df[((out_df[year]==1) & (out_df[channel]==1))], "WJets_ff_ml_{}_{}.root".format(channel,year), key='ntuple')
#
#os.system("python scripts/ff_ml_closure.py ")
