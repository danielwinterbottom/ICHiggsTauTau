
import ROOT
import uproot # can also use root_pandas or root_numpy
import numpy as np
import pandas as pd
from sklearn.model_selection import train_test_split
from sklearn.metrics import classification_report, roc_curve, roc_auc_score, auc
import pickle
import xgboost as xgb
import matplotlib.pyplot as plt
#%matplotlib inline
import time
start_time = time.time()


tree1=uproot.open("/vols/cms/mhh18/Offline/output/SM/Validation_July4_LeadStripSignalThenIsoCone_2017/trainNtuple_Signal/trainNtuple_tt_2017_LeadStripSignalThenIsoCone.root")["train_ntuple"]

tree2=uproot.open("/vols/cms/mhh18/Offline/output/SM/Validation_July4_LeadStripSignalThenIsoCone_2017/trainNtuple_Signal/trainNtuple_mt_2017_LeadStripSignalThenIsoCone.root")["train_ntuple"]


print("working")

df_1 = tree1.pandas.df(["wt", # always require the event weights
                        "event",
                        "wt_cp_sm",
                        "gen_match_1", # gen match info (==5 for real tau_h)
                        "tau_decay_mode_1", # reco tau decay mode
                        "tauFlag1", # gen tau decay mode
                       "E_1","eta_1","pt_1", # tau variables
                       "Egamma1_1","Egamma2_1","Egamma3_1","Egamma4_1", # e/gamma candidate energy
                       "Epi0_1","Epi_1", # pi0 and p energies
                       "rho_dEta_1","rho_dphi_1", 
                       "gammas_dEta_1","gammas_dphi_1",                                               
                       "Mrho_1", # mass of rho (if DM=1, otherwise visible mass of tau)
                       "Mpi0_1",
                       "DeltaR2WRTtau_1",
                       "Mpi0_TwoHighGammas_1",                                         
                       "Mrho_OneHighGammas_1",
                       "Mrho_TwoHighGammas_1",                       
                       "Mrho_subleadingGamma_1",
                       "strip_pt_1",

                      ])

# df_2 will be for the subleading tau ("*_2") in mutau input
df_2 = tree1.pandas.df(["wt", # always require the event weights
                         "event",
                        "wt_cp_sm",
                        "gen_match_2", # gen match info (==5 for real tau_h)
                        "tau_decay_mode_2", # reco tau decay mode
                        "tauFlag2", # gen tau decay mode
                       "E_2","eta_2","pt_2", # tau variables
                       "Egamma1_2","Egamma2_2","Egamma3_2","Egamma4_2", # e/gamma candidate energy
                       "Epi0_2","Epi_2", # pi0 and p energies
                       "rho_dEta_2","rho_dphi_2", 
                       "gammas_dEta_2","gammas_dphi_2",                                               
                       "Mrho_2", # mass of rho (if DM=1, otherwise visible mass of tau)
                       "Mpi0_2",
                       "DeltaR2WRTtau_2",
                       "Mpi0_TwoHighGammas_2",                                         
                       "Mrho_OneHighGammas_2",
                       "Mrho_TwoHighGammas_2",                       
                       "Mrho_subleadingGamma_2",
                       "strip_pt_2",

                       ])

# df_3 will be for the tau ("*_2") in mutau input
df_3 = tree2.pandas.df(["wt", # always require the event weights
                        "event",
                        "wt_cp_sm",
                        "gen_match_2", # gen match info (==5 for real tau_h)
                        "tau_decay_mode_2", # reco tau decay mode
                        "tauFlag2", # gen tau decay mode
                       "E_2","eta_2","pt_2", # tau variables
                       "Egamma1_2","Egamma2_2","Egamma3_2","Egamma4_2", # e/gamma candidate energy
                       "Epi0_2","Epi_2", # pi0 and p energies
                       "rho_dEta_2","rho_dphi_2", 
                       "gammas_dEta_2","gammas_dphi_2",                                               
                       "Mrho_2", # mass of rho (if DM=1, otherwise visible mass of tau)
                       "Mpi0_2",
                       "DeltaR2WRTtau_2",
                       "Mpi0_TwoHighGammas_2",                                         
                       "Mrho_OneHighGammas_2",
                       "Mrho_TwoHighGammas_2",                       
                       "Mrho_subleadingGamma_2",
                       "strip_pt_2",

                      ])





df_1 = df_1[
        #(df_1["mva_olddm_vloose_1"] > 0.5)
    #& (df_1["antiele_1"] == True)
    #& (df_1["antimu_1"] == True)
    (df_1["tau_decay_mode_1"] <2)
    &(df_1["gen_match_1"] == 5)
]


df_2 = df_2[
        #(df_2["mva_olddm_vloose_2"] > 0.5)
    #& (df_2["antiele_2"] == True)
    #& (df_2["antimu_2"] == True)
    (df_2["tau_decay_mode_2"] <2)
    &(df_2["gen_match_2"] == 5)
]




df_3 = df_3[
        #(df_2["mva_olddm_vloose_2"] > 0.5)
    #& (df_2["antiele_2"] == True)
    #& (df_2["antimu_2"] == True)
    (df_3["tau_decay_mode_2"] <2)
    &(df_3["gen_match_2"] == 5)
]


# define some new variables

# for df_1
df_1.loc[:,"Egamma1_tau"] = df_1["Egamma1_1"] / df_1["E_1"]
df_1.loc[:,"Egamma2_tau"] = df_1["Egamma2_1"] / df_1["E_1"]
#df_1.loc[:,"Egamma3_tau"] = df_1["Egamma3_1"] / df_1["E_1"]
df_1.loc[:,"Epi_tau"]     = df_1["Epi_1"] / df_1["E_1"]
df_1.loc[:,"rho_dEta_tau"] = df_1["rho_dEta_1"] * df_1["E_1"]
df_1.loc[:,"rho_dphi_tau"] = df_1["rho_dphi_1"] * df_1["E_1"]
df_1.loc[:,"gammas_dEta_tau"] = df_1["gammas_dEta_1"] * df_1["E_1"]
df_1.loc[:,"gammas_dR_tau"]     = np.sqrt(df_1["gammas_dEta_1"]*df_1["gammas_dEta_1"] + df_1["gammas_dphi_1"]*df_1["gammas_dphi_1"])* df_1["E_1"]
df_1.loc[:,"DeltaR2WRTtau_tau"] = df_1["DeltaR2WRTtau_1"] * df_1["E_1"]* df_1["E_1"]


# now the same for df_2
df_2.loc[:,"Egamma1_tau"] = df_2["Egamma1_2"] / df_2["E_2"]
df_2.loc[:,"Egamma2_tau"] = df_2["Egamma2_2"] / df_2["E_2"]
#df_2.loc[:,"Egamma3_tau"] = df_2["Egamma3_2"] / df_2["E_2"]
df_2.loc[:,"Epi_tau"]     = df_2["Epi_2"] / df_2["E_2"]
df_2.loc[:,"rho_dEta_tau"] = df_2["rho_dEta_2"] * df_2["E_2"]
df_2.loc[:,"rho_dphi_tau"] = df_2["rho_dphi_2"] * df_2["E_2"]
df_2.loc[:,"gammas_dEta_tau"] = df_2["gammas_dEta_2"] * df_2["E_2"]
df_2.loc[:,"gammas_dR_tau"]     = np.sqrt(df_2["gammas_dEta_2"]*df_2["gammas_dEta_2"] + df_2["gammas_dphi_2"]*df_2["gammas_dphi_2"])* df_2["E_2"]
df_2.loc[:,"DeltaR2WRTtau_tau"] = df_2["DeltaR2WRTtau_2"] * df_2["E_2"]* df_2["E_2"]



# now the same for df_3
df_3.loc[:,"Egamma1_tau"] = df_3["Egamma1_2"] / df_3["E_2"]
df_3.loc[:,"Egamma2_tau"] = df_3["Egamma2_2"] / df_3["E_2"]
#df_3.loc[:,"Egamma3_tau"] = df_3["Egamma3_2"] / df_3["E_2"]
df_3.loc[:,"Epi_tau"]     = df_3["Epi_2"] / df_3["E_2"]
df_3.loc[:,"rho_dEta_tau"] = df_3["rho_dEta_2"] * df_3["E_2"]
df_3.loc[:,"rho_dphi_tau"] = df_3["rho_dphi_2"] * df_3["E_2"]
df_3.loc[:,"gammas_dEta_tau"] = df_3["gammas_dEta_2"] * df_3["E_2"]
df_3.loc[:,"gammas_dR_tau"]     = np.sqrt(df_3["gammas_dEta_2"]*df_3["gammas_dEta_2"] + df_3["gammas_dphi_2"]*df_3["gammas_dphi_2"])* df_3["E_2"]
df_3.loc[:,"DeltaR2WRTtau_tau"] = df_3["DeltaR2WRTtau_2"] * df_3["E_2"]* df_3["E_2"]


for key, values in df_1.iteritems():
    if "_1" in key:
        print(key)
        df_1.loc[:,key[:-2]] = df_1[key]
        df_1 = df_1.drop(key, axis=1).reset_index(drop=True)

df_1.loc[:,"tauFlag"] = df_1["tauFlag1"]
df_1 = df_1.drop("tauFlag1", axis=1).reset_index(drop=True)

for key, values in df_2.iteritems():
    if "_2" in key:
        print(key)
        df_2.loc[:,key[:-2]] = df_2[key]
        df_2 = df_2.drop(key, axis=1).reset_index(drop=True)
        
df_2.loc[:,"tauFlag"] = df_2["tauFlag2"]
df_2 = df_2.drop("tauFlag2", axis=1).reset_index(drop=True)


for key, values in df_3.iteritems():
    if "_2" in key:
        print(key)
        df_3.loc[:,key[:-2]] = df_3[key]
        df_3 = df_3.drop(key, axis=1).reset_index(drop=True)
        
df_3.loc[:,"tauFlag"] = df_3["tauFlag2"]
df_3 = df_3.drop("tauFlag2", axis=1).reset_index(drop=True)

comb_df = pd.concat([df_1,df_2], ignore_index=True)
comb_df = pd.concat([comb_df,df_3], ignore_index=True)

#--------------------
df_rho = comb_df[
    (comb_df["tauFlag"] == 1) # this is a genuine rho
]


df_a1 = comb_df[
    (comb_df["tauFlag"] == 2)
]

df_pi = comb_df[
    (comb_df["tauFlag"]==0)
]

df_other = comb_df[
    (comb_df["tauFlag"]==-1)
]



# prepare the target labels
y_rho = pd.DataFrame(np.zeros(df_rho.shape[0]))
y_a1 = pd.DataFrame(np.zeros(df_a1.shape[0]))
y_pi = pd.DataFrame(np.zeros(df_pi.shape[0]))
y_other = pd.DataFrame(np.zeros(df_other.shape[0]))

y_other+=0
y_rho+=1
y_pi+=2
y_a1+=3

frames = [df_other, df_rho, df_pi, df_a1]

X = pd.concat(frames)
w = X["wt_cp_sm"]

y_frames = [y_other, y_rho, y_pi, y_a1 ]

y = pd.concat(y_frames).reset_index(drop=True)
y.columns = ["class"]



# drop any other variables that aren't required in training
X = X.drop(["wt","gen_match","tauFlag","E","Egamma1","Egamma2","Egamma3","Egamma4","gammas_dphi"], axis=1).reset_index(drop=True)

print X.shape




X_odd = X[X["event"]==0]
X_even = X[X["event"]==1]

X_odd_indices = X_odd.index.tolist()
X_even_indices = X_even.index.tolist()

y_odd = y.drop(X_even_indices)
y_even = y.drop(X_odd_indices)

w_odd = X_odd["wt_cp_sm"]
w_even = X_even["wt_cp_sm"]

X_odd = X_odd.drop(["event","wt_cp_sm"], axis=1).reset_index(drop=True)
X_even = X_even.drop(["event","wt_cp_sm"], axis=1).reset_index(drop=True)

print 'variables used in training: '
print X_odd.columns.values


xgb_params = {
    "objective": "multi:softprob",
    "max_depth": 5,
    #"learning_rate": 0.05,
    "learning_rate": 0.04,
    "silent": 1,
    "n_estimators": 2000,
    "subsample": 0.9,
    "seed": 123451,
    "n_jobs":10,
}



xgb_clf_even = xgb.XGBClassifier(**xgb_params)
xgb_clf_even.fit(
    X_even,
    y_even,
    sample_weight=w_even,
    early_stopping_rounds=50,
    eval_set=[(X_even, y_even, w_even), (X_odd, y_odd, w_odd)],
    eval_metric = "mlogloss",
    verbose=True,
)

xgb_clf_even.get_booster().save_model('mvadm_inclusive_2fold_applytoodd.model')


with open ("mvadm_inclusive_2fold_applytoodd.pkl",'w') as f:
    pickle.dump(xgb_clf_even,f)
print 'even-model saved!'

proba_even=xgb_clf_even.predict_proba(X_odd)

with open ("proba_even.pkl",'w') as f:
    pickle.dump(proba_even,f)

with open ("w_odd.pkl",'w') as f:
    pickle.dump(w_odd,f)

with open ("X_odd.pkl",'w') as f:
    pickle.dump(X_odd,f)

with open ("y_odd.pkl",'w') as f:
    pickle.dump(y_odd,f)

predict_even=xgb_clf_even.predict(X_odd)

with open ("predict_even.pkl",'w') as f:
    pickle.dump(predict_even,f)



elapsed_time = time.time() - start_time
print "elapsed time=", elapsed_time




