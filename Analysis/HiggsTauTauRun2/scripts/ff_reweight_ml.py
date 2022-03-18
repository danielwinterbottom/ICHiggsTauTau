import argparse
import json
import root_numpy
import pandas
import numpy
import pickle
#from hep_ml.reweight import GBReweighter
#from hep_ml import reweight
from UserCode.ICHiggsTauTau import reweight
from UserCode.ICHiggsTauTau.reweight import GBReweighter
from UserCode.ICHiggsTauTau.ff_ml_tools import *
from hep_ml.metrics_utils import ks_2samp_weighted

parser = argparse.ArgumentParser()
parser.add_argument('--channel',help= 'Input channel to determine fake factor weights for', default='mt')
parser.add_argument('--year',help= 'Input year to determine fake factor weights for', default='2017')
parser.add_argument("--do_W", action='store_true',help="Get W + jets fake factors. If no do_{W,QCD,ttbar,W_mc} not set will do all.")
parser.add_argument("--do_QCD", action='store_true',help="Get qcd fake factors. If no do_{W,QCD,ttbar,W_mc} not set will do all.")
parser.add_argument("--do_ttbar", action='store_true',help="Get ttbar fake factors. If no do_{W,QCD,ttbar,W_mc} not set will do all.")
parser.add_argument("--do_W_mc", action='store_true',help="Get ttbar fake factors. If no do_{W,QCD,ttbar,W_mc} not set will do all.")
parser.add_argument("--batch", action='store_true',help="Batch run fake factors")
args = parser.parse_args()


if not args.batch:
  # get files and luminoscity for relevant year
  if args.year == '2018':
    lumi = 58826.8469
    params_file = 'scripts/params_mssm_2018.json'
    input_folder = '/vols/cms/dw515/Offline/output/MSSM/mssm_2018_v2/'
    if args.channel == "mt": data_files = ['SingleMuonA','SingleMuonB','SingleMuonC','SingleMuonD','TauA','TauB','TauC','TauD']
    elif args.channel == "et": data_files = ['EGammaA','EGammaB','EGammaC','EGammaD','TauA','TauB','TauC','TauD']
    elif args.channel == "tt": data_files = ['TauA','TauB','TauC','TauD']
    ttbar_files = ['TTTo2L2Nu','TTToHadronic','TTToSemiLeptonic']
    wjets_files = ['W1JetsToLNu-LO','W2JetsToLNu-LO','W3JetsToLNu-LO','W4JetsToLNu-LO','WJetsToLNu-LO','EWKWMinus2Jets','EWKWPlus2Jets']
    other_files = ['EWKZ2Jets','T-tW-ext1','T-t','DY1JetsToLL-LO','DY2JetsToLL-LO','DY3JetsToLL-LO','DY4JetsToLL-LO','DYJetsToLL-LO','DYJetsToLL_M-10-50-LO','Tbar-tW-ext1',
                   'Tbar-t','WWToLNuQQ','WWTo2L2Nu','WZTo1L3Nu','WZTo2L2Q','WZTo3LNu-ext1','WZTo3LNu','ZZTo2L2Nu-ext1','ZZTo2L2Nu-ext2','ZZTo2L2Q','ZZTo4L-ext','ZZTo4L']
  elif args.year == "2017":
    lumi = 41530.
    params_file = 'scripts/params_mssm_2017.json'
    input_folder = '/vols/cms/dw515/Offline/output/MSSM/mssm_2017_v2/'
    if args.channel == "mt": data_files = ['SingleMuonB','SingleMuonC','SingleMuonD','SingleMuonE','SingleMuonF','TauB','TauC','TauD','TauE','TauF']
    elif args.channel == "et": data_files = ['SingleElectronB','SingleElectronC','SingleElectronD','SingleElectronE','SingleElectronF','TauB','TauC','TauD','TauE','TauF']
    elif args.channel == "tt": data_files = ['TauB','TauC','TauD','TauE','TauF']
    ttbar_files = ['TTTo2L2Nu', 'TTToHadronic', 'TTToSemiLeptonic'] 
    wjets_files = ['WJetsToLNu-LO','WJetsToLNu-LO-ext','W1JetsToLNu-LO','W2JetsToLNu-LO','W3JetsToLNu-LO','W4JetsToLNu-LO','EWKWMinus2Jets','EWKWPlus2Jets']
    other_files = ['DYJetsToLL-LO','DYJetsToLL-LO-ext1','DY1JetsToLL-LO','DY1JetsToLL-LO-ext','DY2JetsToLL-LO','DY2JetsToLL-LO-ext','DY3JetsToLL-LO','DY3JetsToLL-LO-ext','DY4JetsToLL-LO',
                   'DYJetsToLL_M-10-50-LO','DYJetsToLL_M-10-50-LO-ext1','T-tW', 'Tbar-tW','Tbar-t','T-t','WWToLNuQQ','WZTo2L2Q','WZTo1L1Nu2Q','WZTo1L3Nu','WZTo3LNu','ZZTo2L2Nu','WWTo2L2Nu',
                   'ZZTo2L2Q','ZZTo4L-ext','ZZTo4L','EWKZ2Jets']
  elif args.year == "2016":
    lumi = 35920.
    params_file = 'scripts/params_mssm_2016.json'
    input_folder = '/vols/cms/dw515/Offline/output/MSSM/mssm_2016_v2/'
    if args.channel == "mt": data_files = ['SingleMuonB','SingleMuonC','SingleMuonD','SingleMuonE','SingleMuonF','SingleMuonG','SingleMuonH','TauB','TauC','TauD','TauE','TauF','TauG','TauH']
    elif args.channel == "et": data_files = ['SingleElectronB','SingleElectronC','SingleElectronD','SingleElectronE','SingleElectronF','SingleElectronG','SingleElectronH','TauB','TauC','TauD',
                                        'TauE','TauF','TauG','TauH']
    elif args.channel == "tt": data_files = ['TauB','TauC','TauD','TauE','TauF','TauG','TauH']
    ttbar_files = ['TT']
    wjets_files = ['WJetsToLNu-LO', 'WJetsToLNu-LO-ext','W1JetsToLNu-LO','W2JetsToLNu-LO','W2JetsToLNu-LO-ext','W3JetsToLNu-LO','W3JetsToLNu-LO-ext','W4JetsToLNu-LO','W4JetsToLNu-LO-ext1',
                   'W4JetsToLNu-LO-ext2', 'EWKWMinus2Jets_WToLNu','EWKWMinus2Jets_WToLNu-ext1','EWKWMinus2Jets_WToLNu-ext2','EWKWPlus2Jets_WToLNu','EWKWPlus2Jets_WToLNu-ext1','EWKWPlus2Jets_WToLNu-ext2']
    other_files = ['DYJetsToLL-LO-ext1','DYJetsToLL-LO-ext2','DY1JetsToLL-LO','DY2JetsToLL-LO','DY3JetsToLL-LO','DY4JetsToLL-LO','DYJetsToLL_M-10-50-LO', 'T-tW', 'Tbar-tW','Tbar-t','T-t','WWTo1L1Nu2Q',
                   'WZJToLLLNu','VVTo2L2Nu','VVTo2L2Nu-ext1','ZZTo2L2Q','ZZTo4L-amcat','WZTo2L2Q','WZTo1L3Nu','WZTo1L1Nu2Q','EWKZ2Jets_ZToLL','EWKZ2Jets_ZToLL-ext1','EWKZ2Jets_ZToLL-ext2']
    
  # read params from json
  with open(params_file) as jsonfile:
    params = json.load(jsonfile)
  
  # get selection for relevant year and channel with alternative working points
  if args.year == "2016": m_lowpt,e_lowpt,t_highpt,t_lowpt_mt,t_lowpt_et = 23,26,120,25,25
  elif args.year == "2017": m_lowpt,e_lowpt,t_highpt,t_lowpt_mt,t_lowpt_et = 25,28,180,32,35
  elif args.year == "2018": m_lowpt,e_lowpt,t_highpt,t_lowpt_mt,t_lowpt_et = 25,33,180,32,35
  if args.channel == "mt":
    baseline = '(iso_1<0.15 && deepTauVsJets_vvvloose_2>0.5 && deepTauVsEle_vvloose_2>0.5 && deepTauVsMu_tight_2>0.5 && leptonveto==0 && pt_2>30 && ((trg_mutaucross==1&&pt_2>%(t_lowpt_mt)s&&pt_2<%(t_highpt)s&&fabs(eta_2)<2.1&&pt_1<%(m_lowpt)s)||(trg_singlemuon==1&&pt_1>=%(m_lowpt)s)||(trg_singletau_2==1&&pt_2>=%(t_highpt)s&&fabs(eta_2)<2.1)))' % vars()
  elif args.channel == "et":
    baseline = '(iso_1<0.15 && deepTauVsJets_vvvloose_2>0.5 && deepTauVsEle_tight_2>0.5 && deepTauVsMu_vloose_2>0.5 && pt_2>30 && ((trg_etaucross==1&&pt_2>%(t_lowpt_et)s&&pt_2<%(t_highpt)s&&fabs(eta_2)<2.1&&pt_1<%(e_lowpt)s)||(trg_singleelectron==1&&pt_1>=%(e_lowpt)s)||(trg_singletau_2==1&&pt_2>=%(t_highpt)s&&fabs(eta_2)<2.1)))' % vars()
  elif args.channel == "tt":
    baseline = '(deepTauVsJets_vvvloose_1>0.5 && deepTauVsJets_vvvloose_2>0.5 && leptonveto==0 && (trg_doubletau==1 || (pt_1>%(t_highpt)s && trg_singletau_1==1) || (pt_2>%(t_highpt)s && trg_singletau_2==1)) && deepTauVsEle_vvloose_1==1 && deepTauVsEle_vvloose_2==1 && deepTauVsMu_vloose_1==1 && deepTauVsMu_vloose_2==1)' % vars()
  
  # variables and weights needed for training
  if args.channel == "mt":
    X_vars = ["pt_1","pt_2","jet_pt_2","jet_pt_2/pt_2","n_jets","n_deepbjets","mva_dm_2","trg_mutaucross","dR","dphi","pt_1*cosh(eta_1)","pt_2*cosh(eta_2)","met","rho"]
    scoring_vars = ['mt_tot','pt_1','pt_2','m_vis','met','eta_2','n_jets','n_deepbjets']
  elif args.channel == "et":
    X_vars = ["pt_1","pt_2","jet_pt_2","jet_pt_2/pt_2","n_jets","n_deepbjets","tau_decay_mode_2","trg_etaucross","dR","dphi","pt_1*cosh(eta_1)","pt_2*cosh(eta_2)","met","rho"]
    scoring_vars = ['mt_tot','pt_1','pt_2','m_vis','met','eta_2','n_jets','n_deepbjets']
  elif args.channel == "tt":
    X_vars = ["pt_1","pt_2","jet_pt_1/pt_1","jet_pt_2/pt_2","jet_pt_1","jet_pt_2","n_jets","n_deepbjets","tau_decay_mode_1","tau_decay_mode_2","dR","dphi","pt_1*cosh(eta_1)","pt_2*cosh(eta_2)","met","rho"]
    scoring_vars = ['mt_tot','pt_1','pt_2','m_vis','met','eta_1','eta_2','n_jets','n_deepbjets']
  
  weights = ["wt","wt_tau_trg_mssm","wt_tau_id_mssm"]
  y_var = "deepTauVsJets_medium_X"
  selection_vars = ["os","mt_1","gen_match_2"] # additional variables need for selection later
  other_vars = ["wt_ff_mssm_1","wt_ff_mssm_wjets_1","wt_ff_mssm_qcd_1","wt_ff_mssm_ttbar_1"]
  
  # read root files into dataframes, performing initial cuts
  if (args.do_W or args.do_QCD) or not (args.do_ttbar or args.do_W_mc or args.do_W or args.do_QCD):
    print ">> Converting root files into dataframes for data"
    data_df_concat = GetDataframe(baseline,lumi,params,input_folder,data_files,args.channel,args.year,X_vars,weights,y_var,selection_vars,scoring_vars,other_vars,data=True)
  
  if (args.do_W_mc) or not (args.do_ttbar or args.do_W_mc or args.do_W or args.do_QCD):
    print ">> Converting root files into dataframes for W + jets MC"
    wjets_df_concat = GetDataframe(baseline,lumi,params,input_folder,wjets_files,args.channel,args.year,X_vars,weights,y_var,selection_vars,scoring_vars,other_vars)
  
  if (args.do_ttbar) or not (args.do_ttbar or args.do_W_mc or args.do_W or args.do_QCD):
    print ">> Converting root files into dataframes for ttbar MC"
    ttbar_df_concat = GetDataframe(baseline,lumi,params,input_folder,ttbar_files,args.channel,args.year,X_vars,weights,y_var,selection_vars,scoring_vars,other_vars)
  
  #if (args.do_W or args.do_QCD) or not (args.do_ttbar or args.do_W_mc or args.do_W or args.do_QCD):
  #  print ">> Converting root files into dataframes for other MC"
  #  other_df_concat = GetDataframe(baseline,lumi,params,input_folder,other_files,args.channel,args.year,X_vars,weights,y_var,selection_vars,scoring_vars,other_vars)
  
  print ">> All root files converted to dataframes"
  
  # perform training and test in each determination region
  if args.channel in ["mt","et"]:
    # get W + jets fake factors
    if args.do_W or not (args.do_ttbar or args.do_W_mc or args.do_W or args.do_QCD):
      print ">> Combining and selecting events for W + jets fake factor determination"
  
      # Add selected data and subtracted other backgrounds with negative weights
      wjets_data = CutAndScale(data_df_concat,"(mt_1>70 && os==1)",1)
      #wjets_sub_mc = CutAndScale([ttbar_df_concat,other_df_concat],"(mt_1>70 && os==1)",-1)
      #wjets_sub_qcd_data = CutAndScale(data_df_concat,"(mt_1>70 && os==0)",-1.1)
      #wjets_sub_qcd_sub_mc = CutAndScale([wjets_df_concat,ttbar_df_concat,other_df_concat],"(mt_1>70 && os==0)",1.1)
      #wjets_data = pd.concat([wjets_data,wjets_sub_mc,wjets_sub_qcd_data,wjets_sub_qcd_sub_mc],ignore_index=True, sort=False)
  
      best_score = {'combined':1,'KS':[],'max_depth':1,'learning_rate':1,'n_estimators':1}
      #for md in [4,5,6]:
      #  for lr in [0.1,0.11,0.12]:
      #    for n_est in [30,40,50]:
      for md in [6]:
        for lr in [0.12]:
          for n_est in [50]:

            print "learning_rate={} max_depth={} n_estimators={}".format(lr,md,n_est)
  
            # split dataframe up into target and original, train and test, training var, weights and other vars
            dfs = SetUpDataframeDict(wjets_data,X_vars,y_var.replace("X","2"),selection_vars,scoring_vars,other_vars)
  
            # fit model
            wjets_reweighter = reweight.GBReweighter(n_estimators=n_est, learning_rate=lr, max_depth=md, min_samples_leaf=1000, gb_args={'subsample': 0.4})
            wjets_reweighter.fit(dfs["original_train"], dfs["target_train"], original_weight=dfs["original_weights_train"] ,target_weight=dfs["target_weights_train"])
  
            # test model
            score = ScoreModel(dfs,wjets_reweighter,X_vars,scoring_vars,other_vars,silent=True)
            if score[0] < best_score['combined']:
              print "learning_rate={} max_depth={} n_estimators={}: {}".format(lr,md,n_est,score)
              best_score['combined'] = score[0]
              best_score['KS'] = score[1]
              best_score['max_depth'] = md
              best_score['learning_rate'] = lr
              best_score['n_estimators'] = n_est
              # save model
              filename = 'wjets_reweighted_ff_{}_{}.sav'.format(args.channel,args.year)
              pickle.dump(wjets_reweighter, open(filename, 'wb'))
      print "Best Score"
      print best_score

      dfs_mc = SetUpDataframeDict(wjets_df_concat,X_vars,y_var.replace("X","2"),selection_vars,scoring_vars,other_vars)
      norm = GetNormalisation(dfs_mc,wjets_reweighter)
      gb_weights_test = norm*wjets_reweighter.predict_weights(dfs_mc["original_test"],dfs_mc["original_weights_test"],merge_weights=False)
      print "Normalisation: ",norm
  
    # get ttbar fake factors
    if args.do_ttbar or not (args.do_ttbar or args.do_W_mc or args.do_W or args.do_QCD):
      print ">> Combining and selecting events for ttbar fake factor determination"
  
      # Add selected data and subtracted other backgrounds with negative weights
      ttbar_data = CutAndScale(ttbar_df_concat,"(mt_1<70 && os==1 && gen_match_2==6)",1)
  
      best_score = {'combined':1,'KS':[],'max_depth':1,'learning_rate':1,'n_estimators':1}
      #for md in [4,5,6]:
      #  for lr in [0.1,0.11,0.12]:
      #    for n_est in [30,40,50]:
      for md in [6]:
        for lr in [0.12]:
          for n_est in [50]:
            print "learning_rate={} max_depth={} n_estimators={}".format(lr,md,n_est)
  
            # split dataframe up into target and original, train and test, training var, weights and other vars
            dfs = SetUpDataframeDict(ttbar_data,X_vars,y_var.replace("X","2"),selection_vars,scoring_vars,other_vars)
  
            # fit model
            ttbar_reweighter = reweight.GBReweighter(n_estimators=n_est, learning_rate=lr, max_depth=md, min_samples_leaf=1000, gb_args={'subsample': 0.4})
            ttbar_reweighter.fit(dfs["original_train"], dfs["target_train"], original_weight=dfs["original_weights_train"] ,target_weight=dfs["target_weights_train"])
  
            # test model
            score = ScoreModel(dfs,ttbar_reweighter,X_vars,scoring_vars,other_vars,silent=True)
            if score[0] < best_score['combined']:
              print "learning_rate={} max_depth={} n_estimators={}: {}".format(lr,md,n_est,score)
              best_score['combined'] = score[0]
              best_score['KS'] = score[1]
              best_score['max_depth'] = md
              best_score['learning_rate'] = lr
              best_score['n_estimators'] = n_est
              # save model
              filename = 'ttbar_reweighted_ff_{}_{}.sav'.format(args.channel,args.year)
              pickle.dump(ttbar_reweighter, open(filename, 'wb'))
  
      print "Best Score"
      print best_score
  
    # get wjets mc fake factors
    if (args.do_W_mc) or not (args.do_ttbar or args.do_W_mc or args.do_W or args.do_QCD):
      print ">> Combining and selecting events for W + jets MC fake factor determination"
  
      # Add selected data and subtracted other backgrounds with negative weights
      wjets_mc_data = CutAndScale(wjets_df_concat,"(mt_1>70 && os==1 && gen_match_2==6)",1)
  
      best_score = {'combined':1,'KS':[],'max_depth':1,'learning_rate':1,'n_estimators':1}
      #for md in [4,5,6]:
      #  for lr in [0.1,0.11,0.12]:
      #    for n_est in [30,40,50]:
      for md in [6]:
        for lr in [0.12]:
          for n_est in [50]:
            print "learning_rate={} max_depth={} n_estimators={}".format(lr,md,n_est)
  
            # split dataframe up into target and original, train and test, training var, weights and other vars
            dfs = SetUpDataframeDict(wjets_mc_data,X_vars,y_var.replace("X","2"),selection_vars,scoring_vars,other_vars)
       
            # fit model
            wjets_mc_reweighter = reweight.GBReweighter(n_estimators=n_est, learning_rate=lr, max_depth=md, min_samples_leaf=1000, gb_args={'subsample': 0.4})
            wjets_mc_reweighter.fit(dfs["original_train"], dfs["target_train"], original_weight=dfs["original_weights_train"] ,target_weight=dfs["target_weights_train"])
     
            # test model
            score = ScoreModel(dfs,wjets_mc_reweighter,X_vars,scoring_vars,other_vars,silent=True)
            if score[0] < best_score['combined']:
              print "learning_rate={} max_depth={} n_estimators={}: {}".format(lr,md,n_est,score)
              best_score['combined'] = score[0]
              best_score['KS'] = score[1]
              best_score['max_depth'] = md
              best_score['learning_rate'] = lr
              best_score['n_estimators'] = n_est
              # save model
              filename = 'wjets_mc_reweighted_ff_{}_{}.sav'.format(args.channel,args.year)
              pickle.dump(wjets_mc_reweighter, open(filename, 'wb'))
  
      print "Best Score"
      print best_score
      dfs_mc = SetUpDataframeDict(wjets_df_concat,X_vars,y_var.replace("X","2"),selection_vars,scoring_vars,other_vars)
      norm = GetNormalisation(dfs_mc,wjets_mc_reweighter)
      gb_weights_test = norm*wjets_mc_reweighter.predict_weights(dfs_mc["original_test"],dfs_mc["original_weights_test"],merge_weights=False)
      print "Normalisation: ",norm
      print gb_weights_test[:100]
      print gb_weights_test.mean()

      wjets_mc_reweighter = pickle.load(open("wjets_mc_reweighted_ff_{}_{}.sav".format(args.channel,args.year), 'rb'))
      print dfs_mc["original_test"]
      gb_weights_test = norm*wjets_mc_reweighter.predict_weights(dfs_mc["original_test"],dfs_mc["original_weights_test"],merge_weights=False)
      print "Normalisation: ",norm
      print gb_weights_test[:100]
      print gb_weights_test.mean()

  
  # get QCD fake factors
  if args.do_QCD or not (args.do_ttbar or args.do_W_mc or args.do_W or args.do_QCD):
    print ">> Combining and selecting events for QCD fake factor determination"
  
    if args.channel in ["mt","et"]:
  
      # Add selected data and subtracted other backgrounds with negative weights
      qcd_data = CutAndScale(data_df_concat,"(mt_1<50 && os==0)",1)
      #qcd_sub_mc = CutAndScale([wjets_df_concat,ttbar_df_concat,other_df_concat],"(mt_1<50 && os==0)",-1)
      #qcd_data = pd.concat([qcd_data,qcd_sub_mc],ignore_index=True, sort=False)
  
      best_score = {'combined':1,'KS':[],'max_depth':1,'learning_rate':1,'n_estimators':1}
      #for md in [4,5,6]:
      #  for lr in [0.1,0.11,0.12]:
      #    for n_est in [30,40,50]:
      for md in [6]:
        for lr in [0.12]:
          for n_est in [50]:

            print "learning_rate={} max_depth={} n_estimators={}".format(lr,md,n_est)
  
            # split dataframe up into target and original, train and test, training var, weights and other vars
            dfs = SetUpDataframeDict(qcd_data,X_vars,y_var.replace("X","2"),selection_vars,scoring_vars,other_vars)
  
            # fit model
            qcd_reweighter = reweight.GBReweighter(n_estimators=n_est, learning_rate=lr, max_depth=md, min_samples_leaf=1000, gb_args={'subsample': 0.4})
            qcd_reweighter.fit(dfs["original_train"], dfs["target_train"], original_weight=dfs["original_weights_train"] ,target_weight=dfs["target_weights_train"])
  
            # test model
            score = ScoreModel(dfs,qcd_reweighter,X_vars,scoring_vars,other_vars,silent=True)
            if score[0] < best_score['combined']:
              print "learning_rate={} max_depth={} n_estimators={}: {}".format(lr,md,n_est,score)
              best_score['combined'] = score[0]
              best_score['KS'] = score[1]
              best_score['max_depth'] = md
              best_score['learning_rate'] = lr
              best_score['n_estimators'] = n_est
              # save model
              filename = 'qcd_reweighted_ff_{}_{}.sav'.format(args.channel,args.year)
              pickle.dump(qcd_reweighter, open(filename, 'wb'))
      print "Best Score"
      print best_score
  
    elif args.channel == "tt":
  
      # Add selected data and subtracted other backgrounds with negative weights
      qcd_data = CutAndScale(data_df_concat,"(os==0)",1)
      #qcd_sub_mc = CutAndScale([wjets_df_concat,ttbar_df_concat,other_df_concat],"(os==0)",-1)
      #qcd_data = pd.concat([qcd_data,qcd_sub_mc],ignore_index=True, sort=False)
  
      # getting leading tau fake factors
      qcd_data_lead = qcd_data[qcd_data.loc[:,y_var.replace("X","2")] == 1]
  
      best_score = {'combined':1,'KS':[],'max_depth':1,'learning_rate':1,'n_estimators':1}
      for md in [4,5,6]:
        for lr in [0.1,0.11,0.12]:
          for n_est in [30,40,50]:
            print "learning_rate={} max_depth={} n_estimators={}".format(lr,md,n_est)
  
            # split dataframe up into target and original, train and test, training var, weights and other vars
            dfs = SetUpDataframeDict(qcd_data_lead,X_vars,y_var.replace("X","1"),selection_vars,scoring_vars,other_vars)
  
            # fit model
            qcd_lead_reweighter = reweight.GBReweighter(n_estimators=n_est, learning_rate=lr, max_depth=md, min_samples_leaf=1000, gb_args={'subsample': 0.4})
            qcd_lead_reweighter.fit(dfs["original_train"], dfs["target_train"], original_weight=dfs["original_weights_train"] ,target_weight=dfs["target_weights_train"])
  
            # test model
            score = ScoreModel(dfs,qcd_lead_reweighter,X_vars,scoring_vars,other_vars,silent=True)
            if score[0] < best_score['combined']:
              print "learning_rate={} max_depth={} n_estimators={}: {}".format(lr,md,n_est,score)
              best_score['combined'] = score[0]
              best_score['KS'] = score[1]
              best_score['max_depth'] = md
              best_score['learning_rate'] = lr
              best_score['n_estimators'] = n_est
              # save model
              filename = 'qcd_lead_reweighted_ff_{}_{}.sav'.format(args.channel,args.year)
              pickle.dump(qcd_lead_reweighter, open(filename, 'wb'))
      print "Best Score"
      print best_score
  
  
      # getting subleading tau fake factors
      qcd_data_sublead = qcd_data[qcd_data.loc[:,y_var.replace("X","1")] == 1]
      best_score = {'combined':1,'KS':[],'max_depth':1,'learning_rate':1,'n_estimators':1}
      for md in [4,5,6]:
        for lr in [0.1,0.11,0.12]:
          for n_est in [30,40,50]:
            print "learning_rate={} max_depth={} n_estimators={}".format(lr,md,n_est)
  
            # split dataframe up into target and original, train and test, training var, weights and other vars
            dfs = SetUpDataframeDict(qcd_data_sublead,X_vars,y_var.replace("X","2"),selection_vars,scoring_vars,other_vars)
  
            # fit model
            qcd_sublead_reweighter = reweight.GBReweighter(n_estimators=n_est, learning_rate=lr, max_depth=md, min_samples_leaf=1000, gb_args={'subsample': 0.4})
            qcd_sublead_reweighter.fit(dfs["original_train"], dfs["target_train"], original_weight=dfs["original_weights_train"] ,target_weight=dfs["target_weights_train"])
  
            # test model
            score = ScoreModel(dfs,qcd_sublead_reweighter,X_vars,scoring_vars,other_vars,silent=True)
            if score[0] < best_score['combined']:
              print "learning_rate={} max_depth={} n_estimators={}: {}".format(lr,md,n_est,score)
              best_score['combined'] = score[0]
              best_score['KS'] = score[1]
              best_score['max_depth'] = md
              best_score['learning_rate'] = lr
              best_score['n_estimators'] = n_est
              # save model
              filename = 'qcd_sublead_reweighted_ff_{}_{}.sav'.format(args.channel,args.year)
              pickle.dump(qcd_sublead_reweighter, open(filename, 'wb'))
      print "Best Score"
      print best_score
else:
  cmssw_base = os.getcwd().replace('src/UserCode/ICHiggsTauTau/Analysis/HiggsTauTauRun2','')
  cmd = "python scripts/ff_reweight_ml.py --channel={} --year={}".format(args.channel,args.year)
  if args.do_W: cmd += ' --do_W'
  if args.do_QCD: cmd += ' --do_QCD'
  if args.do_W_mc: cmd += ' --do_W_mc'
  if args.do_ttbar: cmd += ' --do_ttbar'
  name = "ff_reweight_{}_{}.sh".format(args.channel,args.year)
  CreateBatchJob(name,cmssw_base,[cmd])
  SubmitBatchJob(name,time=180,memory=24,cores=1)
  





