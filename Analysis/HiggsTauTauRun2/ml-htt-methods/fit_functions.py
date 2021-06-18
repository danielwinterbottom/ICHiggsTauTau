import xgboost as xgb
import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
import pickle
import plot_functions as pf
from scipy import interp
# from root_numpy import array2root
import json
import operator
import gc
# from eli5 import explain_prediction_xgboost

from keras.models import Sequential
from keras.initializers import RandomNormal
from keras.layers import Dense
from keras.layers import Activation
from keras.layers import *
from keras.optimizers import Nadam
from keras.optimizers import adam
from keras.regularizers import l2
from keras.callbacks import EarlyStopping
from keras.utils import np_utils


from sklearn.model_selection import KFold
from sklearn.utils import class_weight
from sklearn.metrics import classification_report
from sklearn.metrics import roc_auc_score
from sklearn.metrics import roc_curve
from sklearn.metrics import auc
from sklearn.metrics import recall_score
from sklearn.metrics import precision_score
from sklearn.model_selection import train_test_split
from sklearn.model_selection import RandomizedSearchCV
from sklearn.model_selection import StratifiedKFold
from sklearn.model_selection import StratifiedShuffleSplit
from sklearn.model_selection import cross_val_score
from sklearn.preprocessing import StandardScaler
from sklearn.preprocessing import MinMaxScaler
from sklearn.preprocessing import LabelEncoder
from pandas.plotting import scatter_matrix
from sklearn.metrics import confusion_matrix
from sklearn.ensemble import GradientBoostingClassifier
from sklearn.metrics import mean_squared_error
from sklearn.metrics import f1_score
from sklearn.metrics import fbeta_score
from sklearn.feature_selection import mutual_info_classif
from sklearn.feature_selection import SelectFromModel
from sklearn.neural_network import MLPClassifier

from bayes_opt import BayesianOptimization

def fit_ttsplit(X, channel, fold):

    X["zfeld"] = np.fabs(X.eta_h - (X.jeta_1 + X.jeta_2)/2.)
    X["centrality"] = np.exp(-4*(X.zfeld/np.fabs(X.jdeta))**2)
    X["logPt1"] = np.log(X.pt_1)
    X["logPt2"] = np.log(X.pt_2)

    X_train,X_test, y_train,y_test,w_train,w_test  = train_test_split(
            X,
            X['class'],
            X['wt_xs'],
            test_size=0.33,
            random_state=123456,
            stratify=X['class'].as_matrix(),
            )
    print(X.shape)
    print(X_train[(X_train['class'] == 1)].shape)
    print(X_test[(X_test['class'] == 1)].shape)

    sum_w = X_train['wt_xs'].sum()
    sum_w_cat = X_train.groupby('multi_class')['wt_xs'].sum()
    class_weights = sum_w / sum_w_cat

    class_weight_dict = dict(class_weights)

    print(class_weight_dict)

    # multiply w_train by class_weight now
    for i in w_train.index:
        for key, value in class_weight_dict.items():
            if y_train[i] == key:
                    w_train.at[i] *= value
                    # if key == 'ggh':
                    #     w_train.at[i] *= value * 1.

    X_train = X_train.drop([
        'event','wt','wt_xs','multi_class','process','class',
        'jeta_1','jeta_2','eta_h','zfeld',
        'pt_1','pt_2',
        ], axis=1).reset_index(drop=True)

    X_test = X_test.drop([
        'event','wt','wt_xs','multi_class','process','class',
        'jeta_1','jeta_2','eta_h','zfeld',
        'pt_1','pt_2',
        ], axis=1).reset_index(drop=True)

    orig_columns = X_train.columns
    X_train.columns = ["f{}".format(x) for x in np.arange(X_train.shape[1])]
    X_test.columns = ["f{}".format(x) for x in np.arange(X_train.shape[1])]
    print(orig_columns)
    print(X_train.columns)

    params = {
            'objective':'binary:logistic',
            'max_depth':4,
            # 'min_child_weight':0,
            'learning_rate':0.01,
            'silent':1,
            'n_estimators':10000,
            'gamma':0.1,
            'subsample':0.9,
            # 'max_delta_step':1,
            'nthread':-1,
            'seed':123456
            }

    xgb_clf = xgb.XGBClassifier(**params)

    xgb_clf.fit(
            X_train,
            y_train,
            sample_weight = w_train,
            early_stopping_rounds=50,
            eval_set=[(X_train, y_train,w_train), (X_test, y_test,w_test)],
            eval_metric = ['auc','logloss'],
            verbose=True
            )

    # evals_result = xgb_clf.evals_result()

    y_predict = xgb_clf.predict(X_test)
    print(y_predict)

    print(classification_report(
            y_test,
            y_predict,
            target_names=["ggh", "qqh"],
            sample_weight=w_test
            ))


    y_pred = xgb_clf.predict_proba(X_test)

    print(y_pred)
    # proba_predict_train = xgb_clf.predict_proba(X_train)[:,1]
    # proba_predict_test = xgb_clf.predict_proba(X_test)[:,1]

    ## 15% of highest probablilty output

    # Make predictions for s and b

    ## SAVE FOR SKIP

    # with open('fpr.pkl', 'w') as f:
    #     pickle.dump(fpr, f)
    # with open('tpr.pkl', 'w') as f:
    #     pickle.dump(tpr, f)
    # with open('auc.pkl', 'w') as f:
    #     pickle.dump(auc, f)
    # with open('X_train.pkl', 'w') as f:
    #     pickle.dump(X_train, f)
    # with open('y_train.pkl', 'w') as f:
    #     pickle.dump(y_train, f)
    # with open('X_test.pkl', 'w') as f:
    #     pickle.dump(X_test, f)
    # with open('y_test.pkl', 'w') as f:
    #     pickle.dump(y_test, f)
    # with open('w_test.pkl', 'w') as f:
    #     pickle.dump(w_test, f)
    # with open('w_train.pkl', 'w') as f:
    #     pickle.dump(w_train, f)
    with open('binary_{}_fold{}_xgb.pkl'.format(channel,fold), 'w') as f:
        pickle.dump(xgb_clf, f)

    print(xgb_clf.feature_importances_)
    
    auc = roc_auc_score(y_test, y_pred[:,1])
    print(auc)
    fpr, tpr, _ = roc_curve(y_test, y_pred[:,1])

    pf.plot_roc_curve(
            fpr, tpr, auc,
            '{}_fold{}_roc.pdf'.format(channel, fold))

    # Define these so that I can use plot_output()
    xg_train = xgb.DMatrix(
            X_train,
            label=y_train,
            # missing=-9999,
            weight=w_train
            )
    xg_test = xgb.DMatrix(
            X_test,
            label=y_test,
            # missing=-9999,
            weight=w_test
            )

    pf.plot_features(
            xgb_clf,#.booster(),
            'weight',
            'binary_{}_fold{}_features_weight.pdf'.format(channel,fold))

    pf.plot_features(
            xgb_clf,#.booster(),
            'gain',
            'binary_{}_fold{}_features_gain.pdf'.format(channel,fold))

    pf.plot_output(
            xgb_clf,#.booster(),
            xg_train, xg_test,
            y_train, y_test,
            'binary_{}_fold{}_output.pdf'.format(channel,fold))


    y_prediction = xgb_clf.predict(X_test)

    pf.plot_confusion_matrix(
            y_test, y_prediction, w_test,
            classes=["qqh", "ggh"],
            figname='binary_{}_fold{}_non-normalised_weights_cm.pdf'.format(channel,fold))

    pf.plot_confusion_matrix(
            y_test, y_prediction, w_test,
            classes=["qqh", "ggh"],
            figname='binary_{}_fold{}_normalised_weights_cm.pdf'.format(channel,fold),
            normalise_by_row=True)

    return None

def fit_rhottsplit(X, channel, fold):

    # X["Egamma1_tau1"] = X.Egamma1_1 / X.E_1
    # X["Egamma2_tau1"] = X.Egamma2_1 / X.E_1
    # X["Egamma3_tau1"] = X.Egamma3_1 / X.E_1
    # X["Egamma4_tau1"] = X.Egamma4_1 / X.E_1

    # X["Egamma1_tau2"] = X.Egamma1_2 / X.E_2
    # X["Egamma2_tau2"] = X.Egamma2_2 / X.E_2
    # X["Egamma3_tau2"] = X.Egamma3_2 / X.E_2
    # X["Egamma4_tau2"] = X.Egamma4_2 / X.E_2

    # X["Egamma1_pi01"] = X.Egamma1_1 / X.Epi0_1
    # X["Egamma2_pi01"] = X.Egamma2_1 / X.Epi0_1
    # X["Egamma3_pi01"] = X.Egamma3_1 / X.Epi0_1
    # X["Egamma4_pi01"] = X.Egamma4_1 / X.Epi0_1

    # X["Egamma1_pi02"] = X.Egamma1_2 / X.Epi0_2
    # X["Egamma2_pi02"] = X.Egamma2_2 / X.Epi0_2
    # X["Egamma3_pi02"] = X.Egamma3_2 / X.Epi0_2
    # X["Egamma4_pi02"] = X.Egamma4_2 / X.Epi0_2

    # X["Epi_tau_1"] = X.Epi_1 / X.E_1
    # X["Epi_tau_2"] = X.Epi_2 / X.E_2
    print((X.Mrho))

    X_train,X_test, y_train,y_test,w_train,w_test  = train_test_split(
            X,
            X['class'],
            X['wt_xs'],
            test_size=0.33,
            random_state=123456,
            stratify=X['class'].as_matrix(),
            )
    print(X.shape)
    print(X_train[(X_train['class'] == 1)].shape)
    print(X_test[(X_test['class'] == 1)].shape)

    sum_w = X_train['wt_xs'].sum()
    sum_w_cat = X_train.groupby('class')['wt_xs'].sum()
    class_weights = sum_w / sum_w_cat

    class_weight_dict = dict(class_weights)

    print(class_weight_dict)

    # multiply w_train by class_weight now
    # for i in w_train.index:
    #     for key, value in class_weight_dict.iteritems():
    #         if y_train[i] == key:
    #                 w_train.at[i] *= value
                    # if key == 'ggh':
                    #     w_train.at[i] *= value * 1.

    X_train = X_train.drop([
        'event','wt','wt_xs','multi_class','process','class',
        'tauFlag1','tauFlag2',
        'Egamma1','Egamma2','Egamma3','Egamma4',
        ], axis=1).reset_index(drop=True)

    X_test = X_test.drop([
        'event','wt','wt_xs','multi_class','process','class',
        'tauFlag1','tauFlag2',
        'Egamma1','Egamma2','Egamma3','Egamma4',
        ], axis=1).reset_index(drop=True)

    orig_columns = X_train.columns
    X_train.columns = ["f{}".format(x) for x in np.arange(X_train.shape[1])]
    X_test.columns = ["f{}".format(x) for x in np.arange(X_train.shape[1])]
    print(orig_columns)
    print(X_train.columns)

    params = {
            'objective':'binary:logistic',
            'max_depth':4,
            # 'min_child_weight':0,
            'learning_rate':0.01,
            'silent':1,
            'n_estimators':10000,
            'gamma':0.1,
            'subsample':0.9,
            # 'max_delta_step':1,
            'nthread':-1,
            'seed':123456
            }

    xgb_clf = xgb.XGBClassifier(**params)

    xgb_clf.fit(
            X_train,
            y_train,
            sample_weight = w_train,
            early_stopping_rounds=50,
            eval_set=[(X_train, y_train,w_train), (X_test, y_test,w_test)],
            eval_metric = ['auc','logloss'],
            verbose=True
            )

    # evals_result = xgb_clf.evals_result()

    y_predict = xgb_clf.predict(X_test)
    print(y_predict)

    print(classification_report(
            y_test,
            y_predict,
            target_names=["ggh_rho", "ggh_bkg"],
            sample_weight=w_test
            ))


    y_pred = xgb_clf.predict_proba(X_test)

    # proba_predict_train = xgb_clf.predict_proba(X_train)[:,1]
    # proba_predict_test = xgb_clf.predict_proba(X_test)[:,1]

    ## 15% of highest probablilty output

    # Make predictions for s and b

    ## SAVE FOR SKIP

    # with open('fpr.pkl', 'w') as f:
    #     pickle.dump(fpr, f)
    # with open('tpr.pkl', 'w') as f:
    #     pickle.dump(tpr, f)
    # with open('auc.pkl', 'w') as f:
    #     pickle.dump(auc, f)
    # with open('X_train.pkl', 'w') as f:
    #     pickle.dump(X_train, f)
    # with open('y_train.pkl', 'w') as f:
    #     pickle.dump(y_train, f)
    # with open('X_test.pkl', 'w') as f:
    #     pickle.dump(X_test, f)
    # with open('y_test.pkl', 'w') as f:
    #     pickle.dump(y_test, f)
    # with open('w_test.pkl', 'w') as f:
    #     pickle.dump(w_test, f)
    # with open('w_train.pkl', 'w') as f:
    #     pickle.dump(w_train, f)
    with open('RhoID/binary_{}_fold{}_xgb.pkl'.format(channel,fold), 'w') as f:
        pickle.dump(xgb_clf, f)

    auc = roc_auc_score(y_test, y_pred[:,1])
    print(auc)
    fpr, tpr, _ = roc_curve(y_test, y_pred[:,1])

    pf.plot_roc_curve(
            fpr, tpr, auc,
            'RhoID/{}_fold{}_roc.pdf'.format(channel, fold))

    # Define these so that I can use plot_output()
    xg_train = xgb.DMatrix(
            X_train,
            label=y_train,
            # missing=-9999,
            weight=w_train
            )
    xg_test = xgb.DMatrix(
            X_test,
            label=y_test,
            # missing=-9999,
            weight=w_test
            )

    pf.plot_features(
            xgb_clf,#.booster(),
            'weight',
            'RhoID/binary_{}_fold{}_features_weight.pdf'.format(channel,fold))

    pf.plot_features(
            xgb_clf,#.booster(),
            'gain',
            'RhoID/binary_{}_fold{}_features_gain.pdf'.format(channel,fold))

    pf.plot_output(
            xgb_clf,#.booster(),
            xg_train, xg_test,
            y_train, y_test,
            'RhoID/binary_{}_fold{}_output.pdf'.format(channel,fold))


    y_prediction = xgb_clf.predict(X_test)

    pf.plot_confusion_matrix(
            y_test, y_prediction, w_test,
            classes=["ggh_bkg", "ggh_rho"],
            figname='RhoID/binary_{}_fold{}_non-normalised_weights_cm.pdf'.format(channel,fold))

    pf.plot_confusion_matrix(
            y_test, y_prediction, w_test,
            classes=["ggh_bkg", "ggh_rho"],
            figname='RhoID/binary_{}_fold{}_normalised_weights_cm.pdf'.format(channel,fold),
            normalise_by_row=True)

    return None


def fit_noisejets_ttsplit(X, channel, fold):

    X_train,X_test, y_train,y_test,w_train,w_test  = train_test_split(
            X,
            X['class'],
            X['wt'],
            test_size=0.2,
            random_state=123456,
            stratify=X['class'].as_matrix(),
            )
    print(X.shape)

    sum_w = X_train['wt'].sum()
    sum_w_cat = X_train.groupby('class')['wt'].sum()
    class_weights = sum_w / sum_w_cat

    class_weight_dict = dict(class_weights)

    print(class_weight_dict)

    # multiply w_train by class_weight now
    # print(X_train["multi_class"])
    # print(w_train)
    # for i in w_train.index:
    #     for key, value in class_weight_dict.iteritems():
    #         if y_train[i] == key:
    #                 w_train.at[i] *= value
    # print(X_train["multi_class"])
    # print(w_train)

    X_train = X_train.drop([
        'event','wt','class',#'multi_class',
        'dphi_jtt',
        'jphi_1','jpt_1'

        ], axis=1).reset_index(drop=True)

    X_test = X_test.drop([
        'event','wt','class',#'multi_class',
        'dphi_jtt',
        'jphi_1','jpt_1'

        ], axis=1).reset_index(drop=True)

    # orig_columns = X_train.columns
    # X_train.columns = ["f{}".format(x) for x in np.arange(X_train.shape[1])]
    # X_test.columns = ["f{}".format(x) for x in np.arange(X_train.shape[1])]
    # print(orig_columns)
    print(X_train.columns)


    params = {
            'objective':'binary:logistic',
            'max_depth':4,
            'learning_rate':0.01,
            'silent':1,
            'n_estimators':10000,
            # 'subsample':0.9,
            # 'max_delta_step':1,
            'nthread':-1,
            'seed':123456
            }

    xgb_clf = xgb.XGBClassifier(**params)

    xgb_clf.fit(
            X_train,
            y_train,
            # w_train,
            early_stopping_rounds=20,
            # eval_set=[(X_train, y_train, w_train), (X_test, y_test, w_test)],
            eval_set=[(X_train, y_train), (X_test, y_test)],
            eval_metric = 'auc',
            verbose=True
            )

    # evals_result = xgb_clf.evals_result()

    y_predict = xgb_clf.predict(X_test)
    print(y_predict)
    print('true label: {},{},{}'.format(y_test.values[0],y_test.values[1],y_test.values[2]))
    print('predicted label: {},{},{}'.format(y_predict[0],y_predict[1],y_predict[2]))

    print(classification_report(
            y_test,
            y_predict,
            target_names=["data_genuine", "data_noise"],
            ))


    y_pred = xgb_clf.predict_proba(X_test)

    # proba_predict_train = xgb_clf.predict_proba(X_train)[:,1]
    # proba_predict_test = xgb_clf.predict_proba(X_test)[:,1]

    ## SAVE FOR SKIP

    with open('noisejetID/binary_{}_fold{}_xgb.pkl'.format(channel,fold), 'w') as f:
        pickle.dump(xgb_clf, f)

    auc = roc_auc_score(y_test, y_pred[:,1])
    print(auc)
    fpr, tpr, _ = roc_curve(y_test, y_pred[:,1])

    pf.plot_roc_curve(
            fpr, tpr, auc,
            'noisejetID/{}_fold{}_roc.pdf'.format(channel, fold))

    xgb_clf.save_model("noisejetID/binary_{}_fold{}_xgb.model".format(channel,fold))

    y_prediction = xgb_clf.predict(X_test)

    pf.plot_confusion_matrix(
            y_test, y_prediction, w_test,
            classes=["data_noise", "data_genuine"],
            figname='noisejetID/binary_{}_fold{}_non-normalised_weights_cm.pdf'.format(channel,fold))

    pf.plot_confusion_matrix(
            y_test, y_prediction, w_test,
            classes=["data_noise", "data_genuine"],
            figname='noisejetID/binary_{}_fold{}_normalised_weights_cm.pdf'.format(channel,fold),
            normalise_by_row=True)

    # Define these so that I can use plot_output()
    xg_train = xgb.DMatrix(
            X_train.values,
            label=y_train.values,
            # missing=-9999,
            weight=w_train.values
            )
    xg_test = xgb.DMatrix(
            X_test.values,
            label=y_test.values,
            # missing=-9999,
            weight=w_test.values
            )
    print("bla bla")

    pf.plot_features(
            xgb_clf,#.booster(),
            'weight',
            'noisejetID/binary_{}_fold{}_features_weight.pdf'.format(channel,fold))

    pf.plot_features(
            xgb_clf,#.booster(),
            'gain',
            'noisejetID/binary_{}_fold{}_features_gain.pdf'.format(channel,fold))

    pf.plot_output(
            xgb_clf,#.booster(),
            xg_train, xg_test,
            y_train.values, y_test.values,
            'noisejetID/binary_{}_fold{}_output.pdf'.format(channel,fold))


    return None

def fit_sssplit(X, folds, channel, sig_sample):
    ## STRATIFIED SHUFFLE K FOLD

    sss = StratifiedShuffleSplit(n_splits=folds, test_size=0.3, random_state=123456)

    X = X.sample(frac=1).reset_index(drop=True)
    y = X['class']

    tprs = []
    aucs = []
    mean_fpr = np.linspace(0, 1, 100)

    for i, (train_index, test_index) in enumerate(sss.split(X, y)):
        print('Fold {}/{}'.format(i+1, folds))
        X_train, X_test = X.loc[train_index,:], X.loc[test_index,:]
        y_train, y_test = y[train_index], y[test_index]
        w_train, w_test = X_train['wt'], X_test['wt']

        X_train = X_train.drop(['wt', 'class'], axis=1).reset_index(drop=True)
        X_test = X_test.drop(['wt', 'class'], axis=1).reset_index(drop=True)

        sum_wpos = np.sum(w_train[y_train == 1])
        sum_wneg = np.sum(w_train[y_train == 0])
        ratio = sum_wneg / sum_wpos

        params = {
                'objective':'binary:logistic',
                'max_depth':3,
                'min_child_weight':10,
                'learning_rate':0.01,
                'silent':1,
                'scale_pos_weight':ratio,
                'n_estimators':2000,
                # 'gamma':0.1,
                'subsample':0.9,
                'colsample_bytree':0.9,
                # 'max_delta_step':1,
                'nthread':-1,
                'seed':123456
                }

        xgb_clf = xgb.XGBClassifier(**params)

        xgb_clf.fit(
                X_train,
                y_train,
                sample_weight = w_train,
                early_stopping_rounds=50,
                eval_set=[(X_train, y_train), (X_test, y_test)],
                eval_metric = ['mae', 'auc'],
                verbose=True
                )


        probas_ = xgb_clf.predict_proba(X_test)

        fpr, tpr, _ = roc_curve(y_test.ravel(), probas_[:,1])
        tprs.append(interp(mean_fpr, fpr, tpr))
        tprs[-1][0] = 0.0
        roc_auc = auc(fpr, tpr)
        aucs.append(roc_auc)
        fig, ax = plt.subplots()
        ax.plot(fpr, tpr, lw=1, alpha=0.3)
        #, label='ROC fold {0} (AUC = {1:.2f})'.format(i, roc_auc))

        i += 1

        ax.plot([0,1], [0,1], 'k--')

        mean_tpr = np.mean(tprs, axis=0)
        mean_tpr[-1] = 1.0
        mean_auc = auc(mean_fpr, mean_tpr)
        std_auc = np.std(aucs)

        ax.plot(
                mean_fpr,
                mean_tpr,
                'b',
                label=r'Mean ROC (AUC = {:.2f} $\pm$ {:.2f})'.format(mean_auc, std_auc))

        std_tpr = np.std(tprs, axis=0)
        tprs_upper = np.minimum(mean_tpr + std_tpr, 1)
        tprs_lower = np.maximum(mean_tpr - std_tpr, 0)
        ax.fill_between(
                mean_fpr,
                tprs_lower,
                tprs_upper,
                color='grey',
                alpha=.2,
                label=r'$\pm$ 1 std deviation'
                )

        ax.set_xlabel('False Positive Rate')
        ax.set_ylabel('True Positive Rate')
        ax.grid()
        ax.legend(loc='lower right')
        fig.savefig('{}fold_roc_{}_{}.pdf'.format(folds, channel, sig_sample))

    return None


def fit_gbc_ttsplit(X, channel, sig_sample):

    X = X.sample(frac=1).reset_index(drop=True)

    X_train,X_test, y_train,y_test,w_train,w_test  = train_test_split(
            X,
            X['class'],
            X['wt'],
            test_size=0.30,
            random_state=123456,
            )

    ## SOME TESTS WITH WEIGHTS
    # w_train *= (sum(w) / sum(w_train))
    # w_test *= (sum(w) / sum(w_test))

    sum_wpos = np.sum(w_train[y_train == 1])
    sum_wneg = np.sum(w_train[y_train == 0])
    ratio = sum_wneg / sum_wpos

    X_train = X_train.drop(['wt', 'class', 'eta_1', 'eta_2'], axis=1).reset_index(drop=True)
    X_test = X_test.drop(['wt', 'class', 'eta_1', 'eta_2'], axis=1).reset_index(drop=True)

    # if channel == 'tt':

    if sig_sample in ['powheg', 'JHU']:
        params = {
                'loss':'deviance',
                'max_depth':3,
                # 'min_child_weight':1,
                'learning_rate':0.1,
                'verbose':1,
                # 'scale_pos_weight':ratio,
                # 'min_samples_leaf':600,
                'n_estimators':100,
                'subsample':0.7,
                # 'colsample_bytree':0.8,
                # 'max_delta_step':1,
                'random_state':123456
                }


    # if sig_sample == 'JHU':
    #     params = {
    #             'objective':'binary:logistic',
    #             'max_depth':9,
    #             'min_child_weight':1,
    #             'learning_rate':0.01,
    #             'silent':1,
    #             'scale_pos_weight':ratio,
    #             'n_estimators':2000,
    #             'gamma':2.0,
    #             'subsample':0.9,
    #             'colsample_bytree':0.9,
    #             # 'max_delta_step':1,
    #             'nthread':-1,
    #             'seed':123456
    #             }

    gbc_clf = GradientBoostingClassifier(**params)

    gbc_clf.fit(
            X_train,
            y_train,
            sample_weight = w_train,
            )

    # evals_result = gbc_clf.evals_result()

    y_predict = gbc_clf.predict(X_test)
    print(y_predict)

    print(classification_report(
            y_test,
            y_predict,
            target_names=["background", "signal"],
            sample_weight=w_test
            ))


    decisions = gbc_clf.decision_function(X_test)

    # proba_predict_train = gbc_clf.predict_proba(X_train)[:,1]
    # proba_predict_test = gbc_clf.predict_proba(X_test)[:,1]

    ## 15% of highest probablilty output

    # Make predictions for s and b

    fpr, tpr, _ = roc_curve(y_test, decisions)
    roc_auc = auc(fpr,tpr)

    print(roc_auc)

#     pf.plot_roc_curve(
#             fpr, tpr, roc_auc,
#             'gbc_{}_{}_roc.pdf'.format(channel, sig_sample))

#     pf.compare_train_test(gbc_clf, X_train, y_train, X_test, y_test, 'gbc_{}_{}_output.pdf'.format(channel, sig_sample), bins=30)

    # Define these so that I can use plot_output()
    # xg_train = gbc.DMatrix(
    #         X_train,
    #         label=y_train,
    #         # missing=-9999,
    #         weight=w_train
    #         )
    # xg_test = gbc.DMatrix(
    #         X_test,
    #         label=y_test,
    #         # missing=-9999,
    #         weight=w_test
    #         )

    # pf.plot_output(
    #         gbc_clf.booster(),
    #         xg_train, xg_test,
    #         y_train, y_test,
    #         '{}_{}_output.pdf'.format(channel, sig_sample))

    # pf.plot_features(
    #         gbc_clf.booster(),
    #         'weight',
    #         '{}_{}_features_weight.pdf'.format(channel, sig_sample))

    # pf.plot_features(
    #         gbc_clf.booster(),
    #         'gain',
    #         '{}_{}_features_gain.pdf'.format(channel, sig_sample))


    # y_prediction = gbc_clf.predict(X_test)

    # pf.plot_confusion_matrix(
    #         y_test, y_prediction, w_test,
    #         classes=['background', 'signal'],
    #         figname='{}_{}_non-normalised_weights_cm.pdf'.format(channel, sig_sample),
    #         normalise=False)

    # pf.plot_confusion_matrix(
    #         y_test, y_prediction, w_test,
    #         classes=['background', 'signal'],
    #         figname='{}_{}_normalised_weights_cm.pdf'.format(channel, sig_sample),
    #         normalise=True)

    # ## SAVE FOR SKIP

    # # with open('fpr.pkl', 'w') as f:
    # #     pickle.dump(fpr, f)
    # # with open('tpr.pkl', 'w') as f:
    # #     pickle.dump(tpr, f)
    # # with open('auc.pkl', 'w') as f:
    # #     pickle.dump(auc, f)
    # # with open('X_train.pkl', 'w') as f:
    # #     pickle.dump(X_train, f)
    # # with open('y_train.pkl', 'w') as f:
    # #     pickle.dump(y_train, f)
    # # with open('X_test.pkl', 'w') as f:
    # #     pickle.dump(X_test, f)
    # # with open('y_test.pkl', 'w') as f:
    # #     pickle.dump(y_test, f)
    # # with open('w_test.pkl', 'w') as f:
    # #     pickle.dump(w_test, f)
    # # with open('w_train.pkl', 'w') as f:
    # #     pickle.dump(w_train, f)
    with open('skl_{}_{}_gbc.pkl'.format(channel, sig_sample), 'w') as f:
        pickle.dump(gbc_clf, f)

    return None

def custom_mean_squared_error(y_predicted, y_true):
    labels = y_true.get_label()
    assert len(y_predicted) == len(labels)
    preds = []
    for ls in y_predicted:
        preds.append(max([(v,i) for i,v in enumerate(ls)]))

    np_preds = np.array(preds)
    pred_labels = np_preds[:,1]

    error = np.subtract(pred_labels, labels)

    return 'custom_mean_squared_error', np.mean(np.square(error))

def custom_exponential_loss(y_predicted, y_true):
    labels = y_true.get_label()
    assert len(y_predicted) == len(labels)
    preds = []
    for ls in y_predicted:
        preds.append(max([(v,i) for i,v in enumerate(ls)]))

    np_preds = np.array(preds)
    pred_labels = np_preds[:,1]

    factor = labels * pred_labels

    return 'custom_exponential_loss', - np.exp((1./len(labels)) * np.mean(factor))

def custom_f1_score(y_predicted, y_true):
    labels = y_true.get_label()
    assert len(y_predicted) == len(labels)
    preds = []
    for ls in y_predicted:
        preds.append(max([(v,i) for i,v in enumerate(ls)]))

    # labels_ggh = [x for ind,x in enumerate(labels) if x ==0]
    # ind_labels_ggh = [ind for ind,x in enumerate(labels) if x ==0]
    np_preds = np.array(preds)
    # np_preds = np_preds[ind_labels_ggh]
    pred_labels = np_preds[:,1]
    # print "labels",labels_ggh
    # print "pred_labels",pred_labels

    # f1 = f1_score(labels_ggh,pred_labels,average='micro')
    f1 = f1_score(labels,pred_labels,average='weighted')

    return 'custom_f1_score', 1./f1

def custom_fbeta_score(y_predicted, y_true):
    labels = y_true.get_label()
    assert len(y_predicted) == len(labels)
    preds = []
    for ls in y_predicted:
        preds.append(max([(v,i) for i,v in enumerate(ls)]))

    np_preds = np.array(preds)
    pred_labels = np_preds[:,1]

    fbeta = fbeta_score(labels,pred_labels,beta=5,average='weighted')

    return 'custom_fbeta_score', 1./fbeta

def fit_multiclass_ttsplit(X, analysis, channel, sig_sample):

    # use 'wt_xs' as event weights
    # but calculate class weights for training
    # later using 'wt'

    # actually using scaled weights straight
    # because of better performance
    X_train,X_test, y_train,y_test,w_train,w_test  = train_test_split(
        X,
        X['multi_class'],
        X['wt_xs'],
        test_size=0.5,
        random_state=123456,
        )

    ## FINISH THIS FOR CLASS WEIGHTS CALC
    # class_weights = compute_class_weights(X_train)
    # print class_weights
    # sum_w = X_train['wt'].sum()
    # print sum_w

    # data_gb = X_train.groupby('multi_class')
    # dict_data_gb = {x: data_gb.get_group(x) for x in data_gb.groups}
    # print dict_data_gb

    # class_weights = []
    # # calculate sum of event weights per category
    # sum_w_cat = []
    # for cat in X_train['multi_class']:
    #     if X_train['multi_class'] == cat:
    #         sum_w_cat.append(X_train['wt'])
    #         print 'individual', sum_w_cat
    #     print 'full cat', sum_w_cat

        # try:
        #     print 'category {}'.format(cat)
        #     weights = sum_w / sum_w_cat
        #     print weights
        #     class_weights.append(weights)
        # except ZeroDivisionError:
        #     'Cannot divide by zero'

    # print class_weights

    sum_w = X_train['wt_xs'].sum()
    # print 'sum_w', sum_w
    sum_w_cat = X_train.groupby('multi_class')['wt_xs'].sum()
    # print 'sum_w_cat', sum_w_cat
    class_weights = sum_w / sum_w_cat

    class_weight_dict = dict(class_weights)

    print(class_weight_dict)

    # multiply w_train by class_weight now

    for i in w_train.index:
        for key, value in class_weight_dict.items():
        # print 'before: ',index, row
            if y_train[i] == key:
                if key == 'ggh':
                    w_train.at[i] *= value
                else:
                    w_train.at[i] *= value
                # print 'after dividing by class_weight: ',index, row




    ## use one-hot encoding
    # encode class values as integers
    encoder_train = LabelEncoder()
    encoder_test = LabelEncoder()
    encoder_train.fit(y_train)

    y_train = encoder_train.transform(y_train)

    encoder_test.fit(y_test)
    y_test = encoder_test.transform(y_test)



    # test_class_weight = class_weight.compute_class_weight(
    #     'balanced', np.unique(encoded_Y), encoded_Y
    #     )
    # print test_class_weight

    # print 'original Y: ', X_train['multi_class'].head()
    # print 'one-hot y: ', y_train


    X_train = X_train.drop([
        'wt', 'wt_xs', 'process', 'multi_class', 'class', 'event',
        'gen_match_1', 'gen_match_2'
        ], axis=1).reset_index(drop=True)

    X_test = X_test.drop([
        'wt', 'wt_xs', 'process', 'multi_class', 'class', 'event',
        'gen_match_1', 'gen_match_2'
        ], axis=1).reset_index(drop=True)

    print(X_train.shape)
    print(X_test.shape)


    ## standard scaler
    # columns = X_train.columns
    # scaler = StandardScaler()
    # np_scaled_train = scaler.fit_transform(X_train.as_matrix())
    # del X_train
    # X_train = pd.DataFrame(np_scaled_train)
    # X_train.columns = columns

    # np_scaled_test = scaler.fit_transform(X_test.as_matrix())
    # del X_test
    # X_test = pd.DataFrame(np_scaled_test)
    # X_test.columns = columns

    ## SOME TESTS WITH WEIGHTS
    # w_train *= (sum(w) / sum(w_train))
    # w_test *= (sum(w) / sum(w_test))



    # sum_wpos = np.sum(w_train[y_train == 1])
    # sum_wneg = np.sum(w_train[y_train != 1])
    # ratio = sum_wneg / sum_wpos

    # X_train = X_train.drop(['wt', 'class', 'eta_1', 'eta_2'], axis=1).reset_index(drop=True)
    # X_test = X_test.drop(['wt', 'class', 'eta_1', 'eta_2'], axis=1).reset_index(drop=True)

    # if channel == 'tt':

    # if sig_sample == 'powheg':
    #     params = {
    #             'objective':'multi:softprob',
    #             'max_depth':3,
    #             'min_child_weight':1,
    #             'learning_rate':0.01,
    #             'silent':1,
    #             # 'scale_pos_weight':ratio,
    #             'n_estimators':2000,
    #             'gamma':1.0,
    #             'subsample':0.7,
    #             'colsample_bytree':0.8,
    #             'max_delta_step':1,
    #             'nthread':-1,
    #             'seed':123456
    #             }


    if sig_sample in ['powheg']:
        if channel in ['tt','mt','et','em']:
            params = {
                    'objective':'multi:softprob',
                    'max_depth':8,
                    # 'min_child_weight':1,
                    'learning_rate':0.005,
                    'silent':1,
                    # 'scale_pos_weight':ratio,
                    'n_estimators':500,
                    'gamma':0,
                    'subsample':0.8,
                    'colsample_bytree':0.8,
                    # 'max_delta_step':3,
                    'nthread':-1,
                    'missing':-9999,
                    'seed':123456
                    }
    if sig_sample in ['JHU']:
        if channel in ['tt','mt','et','em']:
            params = {
                    'objective':'multi:softprob',
                    'max_depth':5,
                    # 'min_child_weight':1,
                    'learning_rate':0.025,
                    'silent':1,
                    # 'scale_pos_weight':1,
                    'n_estimators':300,
                    'gamma':0,
                    'subsample':0.8,
                    'colsample_bytree':0.8,
                    # 'max_delta_step':5,
                    'nthread':-1,
                    'missing':-9999,
                    'seed':123456
                    }

        # if channel in ['mt']:
        #     params = {
        #             'objective':'multi:softprob',
        #             'max_depth':8,
        #             # 'min_child_weight':1,
        #             'learning_rate':0.025,
        #             'silent':1,
        #             # 'scale_pos_weight':ratio,
        #             'n_estimators':100,
        #             # 'gamma':2.0,
        #             'subsample':0.9,
        #             'colsample_bytree':0.9,
        #             # 'max_delta_step':1,
        #             'nthread':-1,
        #             'seed':123456
        #             }

        # if channel in ['et']:
        #     params = {
        #             'objective':'multi:softprob',
        #             'max_depth':7,
        #             'min_child_weight':1,
        #             'learning_rate':0.025,
        #             'silent':1,
        #             # 'scale_pos_weight':ratio,
        #             'n_estimators':100,
        #             'gamma':2.0,
        #             'subsample':0.9,
        #             'colsample_bytree':0.9,
        #             # 'max_delta_step':1,
        #             'nthread':-1,
        #             'seed':123456
        #             }

        # if channel == 'em':
        #     params = {
        #             'objective':'multi:softprob',
        #             'max_depth':8,
        #             'min_child_weight':1,
        #             'learning_rate':0.025,
        #             'silent':1,
        #             # 'scale_pos_weight':ratio,
        #             'n_estimators':100,
        #             'gamma':2.0,
        #             'subsample':0.9,
        #             'colsample_bytree':0.9,
        #             'max_delta_step':1,
        #             'nthread':-1,
        #             'seed':123456
        #             }

    xgb_clf = xgb.XGBClassifier(**params)


    xgb_clf.fit(
            X_train,
            y_train,
            sample_weight = w_train,
            early_stopping_rounds=100,
            eval_set=[(X_train, y_train, w_train), (X_test, y_test, w_test)],
            eval_metric = ['merror'],
            verbose=True
            )

    # evals_result = xgb_clf.evals_result()

    y_predict = xgb_clf.predict(X_test)
    print('true label: {},{},{}'.format(y_test[0],y_test[1],y_test[2]))
    print('predicted label: {},{},{}'.format(y_predict[0],y_predict[1],y_predict[2]))

    print('\n Mean Square Error: {}'.format(mean_squared_error(y_test,y_predict)))

    print(classification_report(
            y_test,
            y_predict,
            # target_names=["background", "signal"],
            target_names=list(encoder_test.classes_),
            sample_weight=w_test
            ))


    y_pred = xgb_clf.predict_proba(X_test)
    print('highest proba: {},{},{}'.format(max(y_pred[0]),max(y_pred[1]),max(y_pred[2])))


    with open('multi_{}_{}_{}_xgb.pkl'.format(analysis, channel, sig_sample), 'w') as f:
        pickle.dump(xgb_clf, f)

    # Define these so that I can use plot_output()
    xg_train = xgb.DMatrix(
            X_train,
            label=y_train,
            # missing=-9999,
            weight=w_train
            )
    xg_test = xgb.DMatrix(
            X_test,
            label=y_test,
            # missing=-9999,
            weight=w_test
            )

    # pf.plot_output(
    #         xgb_clf.booster(),
    #         xg_train, xg_test,
    #         y_train, y_test,
    #         'multi_{}_{}_output.pdf'.format(channel, sig_sample))

    pf.plot_features(
            xgb_clf.booster(),
            'weight',
            'multi_{}_{}_{}_features_weight.pdf'.format(analysis, channel, sig_sample))

    pf.plot_features(
            xgb_clf.booster(),
            'gain',
            'multi_{}_{}_{}_features_gain.pdf'.format(analysis, channel, sig_sample))


    y_prediction = xgb_clf.predict(X_test)

    pf.plot_confusion_matrix(
            y_test, y_prediction, w_test,
            # classes=['background', 'signal'],
            classes=list(encoder_test.classes_),
            figname='multi_{}_{}_{}_non-normalised_weights_cm.pdf'.format(analysis, channel, sig_sample),
            normalise=False)

    pf.plot_confusion_matrix(
            y_test, y_prediction, w_test,
            classes=list(encoder_test.classes_),
            figname='multi_{}_{}_{}_normalised_weights_cm.pdf'.format(analysis, channel, sig_sample),
            normalise=True)

    return None


def fit_multiclass_kfold(X, fold, analysis, channel, sig_sample, mjj_training):

    ## START EDITING THIS FOR ODD/EVEN SPLIT
    print('Training XGBoost model fold{}'.format(fold))
    print(X.columns)
    print(X[X.multi_class == "ggh"].wt_xs)
    
    if mjj_training == "high":
        X = X[X["multi_class"] != "misc"]
        if channel == "em":
            X = X[X["multi_class"] != "qcd"]

        # merge ggh and qqh
        # X.multi_class.replace("qqh","ggh",inplace=True)

        # drop ggh entirely and train for qqh
        X = X[X["multi_class"] != "ggh"]


    # for x in X.columns:
    #     if x in ["pt_h"]:
    #         X["exp_{}".format(str(x))] = np.exp(X[str(x)])
    #         X["log_{}".format(str(x))] = np.log(X[str(x)])
    #         X["{}_sq".format(str(x))] = X[str(x)]**2
    #         X["{}_cb".format(str(x))] = X[str(x)]**3
    #         X["{}_tanh".format(str(x))] = np.tanh(X[str(x)])

        # make new variable combinatinos
        # X["mjj_jdeta"] = X.mjj * X.jdeta
        # X["dijetpt_pth"] = X.dijetpt * X.pt_h
        # X["dijetpt_jpt1"] = X.dijetpt * X.jpt_1
        # X["exp_dijetpt_jpt1"] = np.exp(-30000*(X.dijetpt/X.jpt_1))
    
        # X["dijetpt_pth_over_pt1"] = X.dijetpt_pth/X.pt_1
        # X["msv_mvis"] = X.m_sv / X.m_vis
        # X["msvsq_mvis"] = X.m_sv**2 / X.m_vis
        # X["msv_sq"] = np.log(X.m_vis/X.m_sv**2)
        # X["log_metsq_jeta2"] = np.fabs(np.log(X.met**2 * np.fabs(X.jeta_2)))
        # X["met_jeta2"] = X.met * np.fabs(X.jeta_2)
        # X["oppsides_centrality"] = X.opp_sides * X.centrality
        # X["pthsq_ptvis"] = X.pt_h**2 / X.pt_vis
        X["dphi_custom"] = np.arccos(1-X.mt_lep**2/(2.*X.pt_1*X.pt_2))
        X["dR_custom"] = np.sqrt((X.eta_1-X.eta_2)**2 + (X.dphi_custom)**2)
        # X["msv_rec"] = 1. / X.m_sv
        # X["rms_pt"] = np.sqrt(0.5 * (X.pt_1**2 + X.pt_2**2))
        # X["rms_jpt"] = np.sqrt(0.5 * (X.jpt_1**2 + X.jpt_2**2))

        # X["centrality_l1"] = np.exp(-4*np.fabs(X.eta_1-(X.jeta_1 + X.jeta_2)/2.)/X.jdeta**2)
        # X["centrality_l2"] = np.exp(-4*np.fabs(X.eta_2-(X.jeta_1 + X.jeta_2)/2.)/X.jdeta**2)
        # X["centrality_l"] = X.centrality_l1 + X.centrality_l2

        # X["rec_sqrt_msv"] = np.sqrt(1./X.m_sv)

        # for class_ in ["jetFakes","ztt_embed","qqh"]:
        #     pf.plot_signal_background(
        #             X[X.multi_class == "ggh"], X[X.multi_class == class_], 'centrality',
        #             channel, sig_sample,
        #             bins=100
        #             )
    
        # make zeppenfeld variable
        X["zfeld"] = np.fabs(X.eta_h - (X.jeta_1 + X.jeta_2)/2.)
        # print X["zfeld"]
        # make centrality variable
        X["centrality"] = np.exp(-4*(X.zfeld/np.fabs(X.jdeta))**2)

    if mjj_training == "low":
        X = X[X["multi_class"] != "misc"]

        X["dphi_custom"] = np.arccos(1-X.mt_lep**2/(2.*X.pt_1*X.pt_2))
        X["dR_custom"] = np.sqrt((X.eta_1-X.eta_2)**2 + (X.dphi_custom)**2)

    X_train,X_test, y_train,y_test,w_train,w_test  = train_test_split(
        X,
        X['multi_class'],
        X['wt_xs'],
        test_size=0.25,
        random_state=123456,
        stratify=X['multi_class'].as_matrix(),
        )

    print(X_train[(X_train.multi_class == 'ggh')].shape)
    del X
    gc.collect()


    # if want to plot any variables
    # pf.plot_signal_background(X[X["multi_class"] == "ggh"], X[X["multi_class"] == "qqh"], "mjj",channel,sig_sample)

    sum_w = X_train['wt_xs'].sum()
    # print 'sum_w', sum_w
    sum_w_cat = X_train.groupby('multi_class')['wt_xs'].sum()
    # print 'sum_w_cat', sum_w_cat
    class_weights = sum_w / sum_w_cat

    class_weight_dict = dict(class_weights)

    print(class_weight_dict)

    # multiply w_train by class_weight now
    # add mjj dependent weight for ggH

    for i in w_train.index:
        for key, value in class_weight_dict.items():
            if y_train[i] == key:
                if key == "ggh" and mjj_training == "high":
                    # print 'before: ',w_train.at[i]
                    w_train.at[i] *= value * 1.0
                    # print 'after multiplying by class_weight: ',w_train.at[i]
                    # wt_mjj = X_train['mjj'].at[i] * 0.003104 - 0.009583 if X_train['mjj'].at[i] > 300 else 1.0 #from ROC until 1500 GeV
                    # wt_mjj = X_train['mjj'].at[i] * 0.003104 - 0.009583 if X_train['mjj'].at[i] > 500 and X_train['mjj'].at[i] < 1500 else 1.0 #from ROC (slightly higher) until 1500 GeV
                    # wt_mjj = np.sqrt(X_train['mjj'].at[i]) * 0.1368 - 1.3694 # sqrt function
                    # wt_mjj = 1.5 if X_train['mjj'].at[i] > 300 and X_train['mjj'].at[i] < 600 else 1.0  # step function
                    # wt_mjj = ((X_train['mjj'].at[i])**2 * 0.000017 - (X_train['mjj'].at[i] * 0.0017)) #second order poly
                    # w_train.at[i] *= wt_mjj
                # elif key == 'qqh' and mjj_training == "high":
                #     w_train.at[i] *= value*1.5
                # elif key == 'ztt_embed' and mjj_training == "high":
                #     w_train.at[i] *= value*0.5
                # elif channel == 'em' and key == 'qcd':
                #     w_train.at[i] *= value*2.0
                else:
                    w_train.at[i] *= value

    # print w_train
    # minMax = MinMaxScaler()
    # w_train = minMax.fit_transform(w_train)
    # print w_train


    ## use one-hot encoding
    # encode class values as integers
    encoder_train = LabelEncoder()
    encoder_test = LabelEncoder()
    encoder_train.fit(y_train)

    y_train = encoder_train.transform(y_train)

    encoder_test.fit(y_test)
    y_test = encoder_test.transform(y_test)



    # test_class_weight = class_weight.compute_class_weight(
    #     'balanced', np.unique(encoded_Y), encoded_Y
    #     )
    # print test_class_weight

    # print 'original Y: ', X_train['multi_class'].head()
    # print 'one-hot y: ', y_train

    print(X_train.head(5))

    X_train = X_train.drop([
        'wt','wt_xs', 'process', 'multi_class','event',
        'gen_match_1', 'gen_match_2',#'eta_tt',
        # 'dphi_custom',
        # 'dR','opp_sides','mjj','pt_h',
        # 'met_dphi_1','met_dphi_2',
        # 'zfeld',
        #'jeta_1','jeta_2',#'zfeld',
        # 'jpt_1','jpt_2','dijetpt',
        ], axis=1).reset_index(drop=True)

    X_test = X_test.drop([
        'wt','wt_xs', 'process', 'multi_class','event',
        'gen_match_1', 'gen_match_2',#'eta_tt',
        # 'dphi_custom',
        # 'dR','opp_sides','mjj','pt_h',
        # 'met_dphi_1','met_dphi_2',
        # 'zfeld',
        # 'jeta_1','jeta_2',#'zfeld',
        # 'jpt_1','jpt_2','dijetpt',
        ], axis=1).reset_index(drop=True)
    if channel == "em":
        X_train = X_train.drop(["wt_em_qcd"], axis=1).reset_index(drop=True)
        X_test = X_test.drop(["wt_em_qcd"], axis=1).reset_index(drop=True)
    if mjj_training == "high":
        X_train = X_train.drop(["dphi_custom"], axis=1).reset_index(drop=True)
        X_test = X_test.drop(["dphi_custom"], axis=1).reset_index(drop=True)
    if mjj_training == "low":
        X_train = X_train.drop(["dphi_custom"], axis=1).reset_index(drop=True)
        X_test = X_test.drop(["dphi_custom"], axis=1).reset_index(drop=True)
    # else:
    #     X_train = X_train.drop(["zfeld","centrality"], axis=1).reset_index(drop=True)
    #     X_test = X_test.drop(["zfeld","centrality"], axis=1).reset_index(drop=True)

    
    # pf.plot_correlation_matrix(X_train, 'correlation_matrix.pdf')

    # MI = mutual_info_classif(X_train,y_train)
    # print MI


    ## standard scaler
    # scaler = StandardScaler()

    # np_scaled_fit = scaler.fit(X_train.as_matrix())
    # with open('{}_fold{}_scaler.pkl'.format(channel, fold), 'w') as f:
    #     pickle.dump(scaler, f)
    
    # uncomment here if want to use scaler
    ## load scaler from make_dataset
    # with open('{}_{}_scaler.pkl'.format(channel,mjj_training), 'r') as f:
    #     scaler = pickle.load(f)
    # print X_train.head()
    # np_scaled_train = scaler.transform(X_train.as_matrix())
    # X_scaled_train = pd.DataFrame(np_scaled_train)
    # X_scaled_train.columns = X_train.columns

    # del X_train

    # X_train = X_scaled_train
    # print X_train.head()

    # del X_scaled_train

    # np_scaled_test = scaler.transform(X_test.as_matrix())
    # X_scaled_test = pd.DataFrame(np_scaled_test)
    # X_scaled_test.columns = X_test.columns

    # del X_test

    # X_test = X_scaled_test

    # del X_scaled_test


    # X_train = X_train.drop([
    #     'zfeld','jeta_1','jeta_2'
    #     ], axis=1).reset_index(drop=True)

    # X_test = X_test.drop([
    #     'zfeld','jeta_1','jeta_2'
    #     ], axis=1).reset_index(drop=True)

    # to use names "f0" etcs
    print(X_train.columns)
    orig_columns = X_train.columns
    X_train.columns = ["f{}".format(x) for x in np.arange(X_train.shape[1])]
    X_test.columns = ["f{}".format(x) for x in np.arange(X_train.shape[1])]
    print(X_train.columns)


    ## SOME TESTS WITH WEIGHTS
    # w_train *= (sum(w) / sum(w_train))
    # w_test *= (sum(w) / sum(w_test))



    # sum_wpos = np.sum(w_train[y_train == 1])
    # sum_wneg = np.sum(w_train[y_train != 1])
    # ratio = sum_wneg / sum_wpos

    # X_train = X_train.drop(['wt', 'class', 'eta_1', 'eta_2'], axis=1).reset_index(drop=True)
    # X_test = X_test.drop(['wt', 'class', 'eta_1', 'eta_2'], axis=1).reset_index(drop=True)

    # if channel == 'tt':

    # if sig_sample == 'powheg':
    #     params = {
    #             'objective':'multi:softprob',
    #             'max_depth':3,
    #             'min_child_weight':1,
    #             'learning_rate':0.01,
    #             'silent':1,
    #             # 'scale_pos_weight':ratio,
    #             'n_estimators':2000,
    #             'gamma':1.0,
    #             'subsample':0.7,
    #             'colsample_bytree':0.8,
    #             'max_delta_step':1,
    #             'nthread':-1,
    #             'seed':123456
    #             }
    if mjj_training in ['low']:
        if analysis == 'sm':
            if channel in ['tt','mt','et','em']:
                params = {
                        'objective':'multi:softprob',
                        'max_depth':8,
                        # 'min_child_weight':1,
                        'learning_rate':0.05,
                        'silent':1,
                        # 'scale_pos_weight':ratio,
                        'n_estimators':500,
                        'gamma':0,
                        'subsample':0.8,
                        'colsample_bytree':0.8,
                        # 'max_delta_step':3,
                        'nthread':-1,
                        # 'missing':-9999,
                        'seed':123456
                        }
        if analysis == 'cpsm':
            if channel in ['mt','et']:
                params = {
                        'objective':'multi:softprob',
                        'max_depth':7,
                        # 'min_child_weight':1,
                        'learning_rate':0.025,
                        'silent':1,
                        # 'scale_pos_weight':ratio,
                        'n_estimators':3000,
                        'gamma':5,
                        'subsample':0.9,
                        'colsample_bytree':0.6,
                        # 'max_delta_step':3,
                        'nthread':-1,
                        # 'missing':-9999,
                        'seed':123456
                        }
            if channel in ['tt']:
                params = {
                        'objective':'multi:softprob',
                        'max_depth':7,
                        # 'min_child_weight':1,
                        'learning_rate':0.025,
                        'silent':1,
                        # 'scale_pos_weight':ratio,
                        'n_estimators':200,
                        'gamma':5,
                        'subsample':0.9,
                        'colsample_bytree':0.6,
                        # 'max_delta_step':3,
                        'nthread':-1,
                        # 'missing':-9999,
                        'seed':123456
                        }
            if channel in ['em']:
                params = {
                        'objective':'multi:softprob',
                        'max_depth':7,
                        # 'min_child_weight':1,
                        'learning_rate':0.025,
                        'silent':1,
                        # 'scale_pos_weight':ratio,
                        'n_estimators':1000,
                        'gamma':5,
                        'subsample':0.9,
                        'colsample_bytree':0.6,
                        # 'max_delta_step':3,
                        'nthread':-1,
                        # 'missing':-9999,
                        'seed':123456
                        }
    if mjj_training in ['high']:
        if channel in ['tt']:
            params = {
                    'objective':'multi:softprob',
                    'max_depth':5,
                    'min_child_weight':0,
                    'learning_rate':0.025,
                    'silent':1,
                    # 'scale_pos_weight':1,
                    'n_estimators':600,
                    # 'gamma':5,
                    'subsample':0.9,
                    # 'colsample_bytree':0.9,
                    # 'max_delta_step':5,
                    'nthread':-1,
                    # 'missing':-100.0,
                    'seed':123456
                    }
        if channel in ['mt','et']:
            params = {
                    'objective':'multi:softprob',
                    'max_depth':4,
                    # 'min_child_weight':1,
                    'learning_rate':0.025,
                    'silent':1,
                    # 'scale_pos_weight':1,
                    'n_estimators':1500,
                    'gamma':5,
                    'subsample':0.9,
                    'colsample_bytree':0.6,
                    # 'max_delta_step':5,
                    'nthread':-1,
                    # 'missing':-100.0,
                    'seed':123456
                    }
        if channel in ['em']:
            params = {
                    'objective':'multi:softprob',
                    'max_depth':4,
                    # 'min_child_weight':1,
                    'learning_rate':0.025,
                    'silent':1,
                    # 'scale_pos_weight':1,
                    'n_estimators':1500,
                    'gamma':5,
                    'subsample':0.9,
                    'colsample_bytree':0.6,
                    # 'max_delta_step':5,
                    'nthread':-1,
                    # 'missing':-100.0,
                    'seed':123456
                    }
    if sig_sample == "madgraph":
        if mjj_training in ['high','high_tight']:
            if channel in ['tt']:
                params = {
                        'objective':'multi:softprob',
                        'max_depth':5,
                        # 'min_child_weight':5,
                        'learning_rate':0.025,
                        'silent':1,
                        # 'scale_pos_weight':1000,
                        'n_estimators':10000,
                        'gamma':0.1,
                        'reg_lambda':0.3,
                        # 'reg_alpha':0.1,
                        'subsample':0.8,
                        # 'colsample_bytree':0.9,
                        # 'max_delta_step':5,
                        'nthread':-1,
                        # 'missing':-100.0,
                        'seed':123456
                        }
            if channel in ['mt','et']:
                params = {
                        'objective':'multi:softprob',
                        'max_depth':6,
                        # 'min_child_weight':1,
                        'learning_rate':0.025,
                        'silent':1,
                        # 'scale_pos_weight':1,
                        'n_estimators':10000,
                        'gamma':0.1,
                        'reg_lambda':0.3,
                        # 'reg_alpha':0.1,
                        'subsample':0.8,
                        # 'colsample_bytree':0.9,
                        # 'max_delta_step':5,
                        'nthread':-1,
                        # 'missing':-100.0,
                        'seed':123456
                        }
            if channel in ['em']:
                params = {
                        'objective':'multi:softprob',
                        'max_depth':6,
                        # 'min_child_weight':5,
                        'learning_rate':0.025,
                        'silent':1,
                        # 'scale_pos_weight':1,
                        'n_estimators':10000,
                        'gamma':0.1,
                        'reg_lambda':0.3,
                        # 'reg_alpha':0.1,
                        'subsample':0.8,
                        # 'colsample_bytree':0.9,
                        # 'max_delta_step':5,
                        'nthread':-1,
                        # 'missing':-100.0,
                        'seed':123456
                        }
        elif mjj_training in ['low']:
            if channel in ['tt']:
                params = {
                        'objective':'multi:softprob',
                        'max_depth':6,
                        'min_child_weight':1,
                        'learning_rate':0.01,
                        'silent':1,
                        # 'scale_pos_weight':1,
                        'n_estimators':10000,
                        'gamma':2,
                        'subsample':0.9,
                        'colsample_bytree':0.6,
                        # 'max_delta_step':5,
                        'nthread':-1,
                        # 'missing':-100.0,
                        'seed':123456
                        }
            if channel in ['mt','et']:
                params = {
                        'objective':'multi:softprob',
                        'max_depth':6,
                        'min_child_weight':1,
                        'learning_rate':0.05,
                        'silent':1,
                        # 'scale_pos_weight':1,
                        'n_estimators':10000,
                        'gamma':0.1,
                        'reg_lambda':0.3,
                        'subsample':0.8,
                        # 'colsample_bytree':0.6,
                        # 'max_delta_step':5,
                        'nthread':-1,
                        # 'missing':-100.0,
                        'seed':123456
                        }
            if channel in ['em']:
                params = {
                        'objective':'multi:softprob',
                        'max_depth':6,
                        'min_child_weight':1,
                        'learning_rate':0.05,
                        'silent':1,
                        # 'scale_pos_weight':1,
                        'n_estimators':10000,
                        'gamma':0.1,
                        'reg_lambda':0.3,
                        'subsample':0.8,
                        # 'colsample_bytree':0.6,
                        # 'max_delta_step':5,
                        'nthread':-1,
                        # 'missing':-100.0,
                        'seed':123456
                        }
        # if channel in ['mt','et','em']:
        #     params = {
        #             'objective':'multi:softprob',
        #             'max_depth':5,
        #             # 'min_child_weight':1,
        #             'learning_rate':0.025,
        #             'silent':1,
        #             # 'scale_pos_weight':1,
        #             'n_estimators':3000,
        #             # 'gamma':10,
        #             'subsample':0.9,
        #             # 'colsample_bytree':0.5,
        #             # 'max_delta_step':5,
        #             'nthread':-1,
        #             # 'missing':-9999,
        #             'seed':123456
        #             }
        # if channel in ['et']:
        #     params = {
        #             'objective':'multi:softprob',
        #             'max_depth':4,
        #             # 'min_child_weight':1,
        #             'learning_rate':0.1,
        #             'silent':1,
        #             # 'scale_pos_weight':1,
        #             'n_estimators':10000,
        #             # 'gamma':10,
        #             'subsample':0.9,
        #             # 'colsample_bytree':0.5,
        #             # 'max_delta_step':5,
        #             'nthread':-1,
        #             # 'missing':-9999,
        #             'seed':123456
        #             }
        # if channel in ['em']:
        #     params = {
        #             'objective':'multi:softprob',
        #             'max_depth':5,
        #             # 'min_child_weight':1,
        #             'learning_rate':0.005,
        #             'silent':1,
        #             # 'scale_pos_weight':1,
        #             'n_estimators':3500,
        #             # 'gamma':10,
        #             'subsample':0.9,
        #             # 'colsample_bytree':0.5,
        #             # 'max_delta_step':5,
        #             'nthread':-1,
        #             # 'missing':-9999,
        #             'seed':123456
        #             }


    xgb_clf = xgb.XGBClassifier(**params)

    # select features using threshold
    # selection = SelectFromModel(xgb_clf)

    if mjj_training in ['high','high_tight']:
        if channel in ['tt','mt','et','em']:
            xgb_clf.fit(
                    X_train,
                    y_train,
                    sample_weight = w_train,
                    early_stopping_rounds=50,
                    eval_set=[(X_train, y_train, w_train), (X_test, y_test, w_test)],
                    eval_metric = ['merror','mlogloss'],
                    # eval_metric = custom_mean_squared_error,
                    # eval_metric = custom_f1_score,
                    verbose=True
                    )
            # selection.fit(
            #         X_train,
            #         y_train,
            #         sample_weight = w_train,
            #         early_stopping_rounds=50,
            #         eval_set=[(X_train, y_train, w_train), (X_test, y_test, w_test)],
            #         eval_metric = ['merror','mlogloss'],
            #         # eval_metric = custom_f1_score,
            #         verbose=True
            #         )
    if mjj_training in ['low']:
        if channel in ['tt','mt','et','em']:
            xgb_clf.fit(
                    X_train,
                    y_train,
                    sample_weight = w_train,
                    early_stopping_rounds=50,
                    eval_set=[(X_train, y_train, w_train), (X_test, y_test, w_test)],
                    eval_metric = ['merror','mlogloss'],
                    verbose=True
                    )
        # if channel in ['em']:
        #     xgb_clf.fit(
        #             X_train,
        #             y_train,
        #             sample_weight = w_train,
        #             # early_stopping_rounds=50,
        #             eval_set=[(X_train, y_train, w_train), (X_test, y_test, w_test)],
        #             eval_metric = custom_mean_squared_error,
        #             verbose=True
        #             )
    # if sig_sample in ['JHU']:
    #     if channel in ['tt','mt','et','em']:
    #         xgb_clf.fit(
    #                 X_train,
    #                 y_train,
    #                 sample_weight = w_train,
    #                 early_stopping_rounds=20,
    #                 eval_set=[(X_train, y_train, w_train), (X_test, y_test, w_test)],
    #                 eval_metric = ['mlogloss'],
    #                 verbose=True
    #                 )

    # evals_result = xgb_clf.evals_result()
    # print selection.get_support()

    # print "best iteration: ",xgb_clf.best_iteration

    # eli5 explanation
    # print explain_prediction_xgboost(xgb_clf.get_booster(),X_test.iloc[0])

    # xgb_bo = BayesianOptimization(xgb_clf, {
    #     "max_depth": (2,8),
    #     "gamma": (0.001, 3.0),
    #     "min_child_weight": (0, 20),
    #     "max_delta_step": (0, 5),
    #     "subsample": (0.4, 1.0),
    #     "colsample_bytree": (0.4, 1.0),
    #     "reg_lambda": (0.001, 1.0),
    #     })
    # print(("-"*100))
    # xgb_bo.maximize(init_points=2, n_iter=5)
    # print(('Maximum XGBOOST value: %f' % XGB_BO.res['max']['max_val']))
    # print(('Best XGBOOST parameters: ', XGB_BO.res['max']['max_params']))

    # y_predict = selection.predict(X_test)
    y_predict = xgb_clf.predict(X_test)
    print('true label: {},{},{},{},{},{}'.format(y_test[0],y_test[1],y_test[2],y_test[3],y_test[4],y_test[5]))
    print('predicted label: {},{},{},{},{},{}'.format(y_predict[0],y_predict[1],y_predict[2],y_predict[3],y_predict[4],y_predict[5]))

    print('\n Mean Square Error: {}'.format(mean_squared_error(y_test,y_predict)))

    print(classification_report(
            y_test,
            y_predict,
            # target_names=["background", "signal"],
            target_names=list(encoder_test.classes_),
            sample_weight=w_test
            ))


    y_pred = xgb_clf.predict_proba(X_test)
    print('all probs: {} \n {} \n {}'.format(y_pred[0],y_pred[1],y_pred[2],y_pred[3],y_pred[4],y_pred[5]))
    print('highest proba: {},{},{}'.format(max(y_pred[0]),max(y_pred[1]),max(y_pred[2])))


    print(xgb_clf)
    with open('multi_fold{}_{}_{}_{}_{}_xgb.pkl'.format(fold, analysis, channel, sig_sample, mjj_training), 'w') as f:
        pickle.dump(xgb_clf, f)

    # Define these so that I can use plot_output()
    xg_train = xgb.DMatrix(
            X_train,
            label=y_train,
            # missing=-100.0,
            weight=w_train
            )
    xg_test = xgb.DMatrix(
            X_test,
            label=y_test,
            # missing=-100.0,
            weight=w_test
            )

    ## Plotting things

    pf.plot_learning_curve(xgb_clf,
            "mlogloss",
            "multi_fold{}_{}_{}_{}_{}_learning_curve_logloss.pdf".format(fold, analysis, channel, sig_sample, mjj_training))
    pf.plot_learning_curve(xgb_clf,
            "merror",
            "multi_fold{}_{}_{}_{}_{}_learning_curve_error.pdf".format(fold, analysis, channel, sig_sample, mjj_training))

    # pf.plot_output(
    #         xgb_clf.booster(),
    #         xg_train, xg_test,
    #         y_train, y_test,
    #         'multi_{}_{}_output.pdf'.format(channel, sig_sample))

    pf.plot_features(
            xgb_clf,
            'weight',
            'multi_fold{}_{}_{}_{}_{}_features_weight.pdf'.format(fold, analysis, channel, sig_sample, mjj_training))

    pf.plot_features(
            xgb_clf,
            'gain',
            'multi_fold{}_{}_{}_{}_{}_features_gain.pdf'.format(fold, analysis, channel, sig_sample, mjj_training))


    y_prediction = xgb_clf.predict(X_test)

    pf.plot_confusion_matrix(
            y_test, y_prediction, w_test,
            # classes=['background', 'signal'],
            classes=list(encoder_test.classes_),
            figname='multi_fold{}_{}_{}_{}_{}_non-normalised_weights_cm.pdf'.format(fold, analysis, channel, sig_sample, mjj_training))

    pf.plot_confusion_matrix(
            y_test, y_prediction, w_test,
            classes=list(encoder_test.classes_),
            figname='multi_fold{}_{}_{}_{}_{}_normalised_efficiency_weights_cm.pdf'.format(fold, analysis, channel, sig_sample, mjj_training),
            normalise_by_col=True)
    pf.plot_confusion_matrix(
            y_test, y_prediction, w_test,
            classes=list(encoder_test.classes_),
            figname='multi_fold{}_{}_{}_{}_{}_normalised_purity_weights_cm.pdf'.format(fold, analysis, channel, sig_sample, mjj_training),
            normalise_by_row=True)

    return None



#### NEW FUNCTION FOR INCLUSIVE TRAINING (CP IN DECAYS)
def fit_multiclass_kfold_inc(X, fold, analysis, channel, sig_sample, era, splitByDM=None):

    ## START EDITING THIS FOR ODD/EVEN SPLIT
    print('Training XGBoost model fold{}'.format(fold))
    print(X.columns)
    X.dropna(inplace=True)

    # X = X[X["multi_class"] != "misc"]
    if channel == "em":
        X = X[X["multi_class"] != "qcd"]
    if channel == "mt":
        X = X[X["multi_class"] != "zll"]

    X = X[X["multi_class"] != "misc"]
    X["multi_class"].replace("qqh","ggh",inplace=True)
    X["multi_class"].replace("vh","ggh",inplace=True)
    X["multi_class"].replace("ggh","higgs",inplace=True)

    # split by DM here (HPS for now)
    if splitByDM is not None:
        if splitByDM == 1:
            X.eval("tau_decay_mode_1==1 and tau_decay_mode_2==1", inplace=True)
        if splitByDM == 2:
            X.eval("(tau_decay_mode_1==1 and tau_decay_mode_2==10) or (tau_decay_mode_1==10 and tau_decay_mode_2==1)", inplace=True)
    # X = X.drop(["tau_decay_mode_1", "tau_decay_mode_2"], axis=1).reset_index(drop=True)

    X_train,X_test, y_train,y_test,w_train,w_test  = train_test_split(
        X,
        X['multi_class'],
        X['wt_xs'],
        test_size=0.25,
        random_state=123456,
        stratify=X['multi_class'].as_matrix(),
        )

    print(X_train[(X_train.multi_class == 'higgs')].shape)
    del X
    gc.collect()

    # if want to plot any variables
    # pf.plot_signal_background(X[X["multi_class"] == "ggh"], X[X["multi_class"] == "qqh"], "mjj",channel,sig_sample)

    sum_w = X_train['wt_xs'].sum()
    # print 'sum_w', sum_w
    sum_w_cat = X_train.groupby('multi_class')['wt_xs'].sum()
    # print 'sum_w_cat', sum_w_cat
    class_weights = sum_w / sum_w_cat

    class_weight_dict = dict(class_weights)

    print(class_weight_dict)


    # multiply w_train by class_weight now
    # add mjj dependent weight for ggH

    for i in w_train.index:
        for key, value in class_weight_dict.items():
            if y_train[i] == key:
                w_train.at[i] *= value

    sum_w_cat_after = X_train.groupby('multi_class')['wt_xs'].sum()
    print(sum_w_cat_after)

    # if want to replace here to check effect of reweighting by class here
    # X_train["multi_class"].replace("qqh","ggh",inplace=True)
    # X_test["multi_class"].replace("qqh","ggh",inplace=True)
    # y_train = np.where(y_train=="qqh", "ggh", y_train)
    # y_test =  np.where(y_test=="qqh", "ggh", y_test)

    ## use one-hot encoding
    # encode class values as integers
    encoder_train = LabelEncoder()
    encoder_test = LabelEncoder()
    encoder_train.fit(y_train)

    y_train = encoder_train.transform(y_train)

    encoder_test.fit(y_test)
    y_test = encoder_test.transform(y_test)

    print(X_train.head(5))

    dropVars = ["wt","wt_xs", "process", "multi_class","event","gen_match_1", "gen_match_2",]
    if sig_sample in ["tauspinner","powheg"]:
        dropVars.append("wt_cp_sm")
        dropVars.append("wt_cp_ps")
        dropVars.append("wt_ph_nnlops")
    if channel == "em":
        dropVars.append("wt_em_qcd")

    X_train = X_train.drop(dropVars, axis=1).reset_index(drop=True)
    X_test = X_test.drop(dropVars, axis=1).reset_index(drop=True)

    # pf.plot_correlation_matrix(X_train, 'correlation_matrix_fold{}_{}.pdf'.format(fold, era))

    # to use names "f0" etcs
    print(X_train.columns)
    orig_columns = X_train.columns
    X_train.columns = ["f{}".format(x) for x in np.arange(X_train.shape[1])]
    X_test.columns = ["f{}".format(x) for x in np.arange(X_train.shape[1])]
    print(X_train.columns)

    with open('xtest_{}_{}_{}.pkl'.format(era, channel, fold), 'w') as f:
        pickle.dump(X_test, f)
    with open('ytest_{}_{}_{}.pkl'.format(era, channel, fold), 'w') as f:
        pickle.dump(y_test, f)

    if channel in ['tt']:
        params_optimised = {
            'gamma': 0.6116681199107202, 'booster': 'gbtree',
            'max_depth': 4, 'learning_rate': 0.06427997105779841,
            'objective': 'multi:softprob', 
            'reg_lambda': 1.5550976281683702, 'reg_alpha': 0.12309703586128405,
            'subsample': 0.7563612725885316,
            'colsample_bytree': 0.8412848980537144,
            'min_child_weight': 258.320681582356, 'max_delta_step': 2,
            'num_class': 3, 'seed': 123456, 'nthread': 1, 'silent': 1,
            'n_estimators':10000,
        }
        params = { # not used anymore
                'objective':'multi:softprob',
                'max_depth':4,
                'min_child_weight':1,
                # 'learning_rate':0.05,
                'learning_rate':0.1,
                'silent':1,
                # 'scale_pos_weight':1,
                'n_estimators':10000,
                'gamma':2,
                'subsample':0.9,
                'colsample_bytree':0.6,
                # 'max_delta_step':5,
                'nthread':1,
                'seed':123456
                }
    if channel in ['mt','et']:
        params = {
                'objective':'multi:softprob',
                'max_depth':4,
                'min_child_weight':1,
                'learning_rate':0.1,
                'silent':1,
                # 'scale_pos_weight':1,
                'n_estimators':10000,
                # 'gamma':0.1,
                # 'reg_lambda':0.3,
                'gamma':2,
                'subsample':0.9,
                'colsample_bytree':0.6,
                # 'max_delta_step':5,
                'nthread':1,
                'seed':123456
                }

    xgb_clf = xgb.XGBClassifier(**params_optimised)
    # xgb_clf = xgb.XGBClassifier(**params)
    print(xgb_clf)

    if channel in ['tt','mt','et','em']:
        xgb_clf.fit(
                X_train,
                y_train,
                sample_weight = w_train,
                early_stopping_rounds=20,
                eval_set=[(X_train, y_train, w_train), (X_test, y_test, w_test)],
                eval_metric = ['merror','mlogloss'],
                # eval_metric = custom_mean_squared_error,
                # eval_metric = custom_f1_score,
                verbose=True
                )

    y_predict = xgb_clf.predict(X_test)
    print('true label: {},{},{},{},{},{}'.format(y_test[0],y_test[1],y_test[2],y_test[3],y_test[4],y_test[5]))
    print('predicted label: {},{},{},{},{},{}'.format(y_predict[0],y_predict[1],y_predict[2],y_predict[3],y_predict[4],y_predict[5]))

    print('\n Mean Square Error: {}'.format(mean_squared_error(y_test,y_predict)))

    print(classification_report(
            y_test,
            y_predict,
            target_names=list(encoder_test.classes_),
            sample_weight=w_test
            ))


    y_pred = xgb_clf.predict_proba(X_test)
    print('all probs: {} \n {} \n {}'.format(y_pred[0],y_pred[1],y_pred[2],y_pred[3],y_pred[4],y_pred[5]))
    print('highest proba: {},{},{}'.format(max(y_pred[0]),max(y_pred[1]),max(y_pred[2])))


    print(xgb_clf)
    with open('multi_fold{}_{}_{}_{}_{}_xgb.pkl'.format(fold, analysis, channel, sig_sample, era), 'w') as f:
        pickle.dump(xgb_clf, f)

    # Define these so that I can use plot_output()
    xg_train = xgb.DMatrix(
            X_train,
            label=y_train,
            weight=w_train
            )
    xg_test = xgb.DMatrix(
            X_test,
            label=y_test,
            weight=w_test
            )

    ## Plotting things

    pf.plot_learning_curve(xgb_clf,
            "mlogloss",
            "multi_fold{}_{}_{}_{}_{}_learning_curve_logloss.pdf".format(fold, analysis, channel, sig_sample, era))
    pf.plot_learning_curve(xgb_clf,
            "merror",
            "multi_fold{}_{}_{}_{}_{}_learning_curve_error.pdf".format(fold, analysis, channel, sig_sample, era))

    pf.plot_features(
            xgb_clf,
            'weight',
            'multi_fold{}_{}_{}_{}_{}_features_weight.pdf'.format(fold, analysis, channel, sig_sample, era))

    pf.plot_features(
            xgb_clf,
            'gain',
            'multi_fold{}_{}_{}_{}_{}_features_gain.pdf'.format(fold, analysis, channel, sig_sample, era))


    y_prediction = xgb_clf.predict(X_test)

    pf.plot_confusion_matrix(
            y_test, y_prediction, w_test,
            classes=list(encoder_test.classes_),
            figname='multi_fold{}_{}_{}_{}_{}_non-normalised_weights_cm.pdf'.format(fold, analysis, channel, sig_sample, era))

    pf.plot_confusion_matrix(
            y_test, y_prediction, w_test,
            classes=list(encoder_test.classes_),
            figname='multi_fold{}_{}_{}_{}_{}_normalised_efficiency_weights_cm.pdf'.format(fold, analysis, channel, sig_sample, era),
            normalise_by_col=True)
    pf.plot_confusion_matrix(
            y_test, y_prediction, w_test,
            classes=list(encoder_test.classes_),
            figname='multi_fold{}_{}_{}_{}_{}_normalised_purity_weights_cm.pdf'.format(fold, analysis, channel, sig_sample, era),
            normalise_by_row=True)

    return None

######## TESTING CV
def fit_multiclass_cvkfold(X, fold, analysis, channel, sig_sample):

    ## START EDITING THIS FOR ODD/EVEN SPLIT
    print('Training XGBoost model fold{}'.format(fold))


    numFolds = 4
    folds = StratifiedKFold(n_splits=numFolds, shuffle=True, random_state=123456)

    estimators = []
    results = np.zeros(X.shape[0])
    score = 0.0

    X = X.reset_index(drop=True)

    for train_index, test_index in folds.split(X,X['multi_class']):
        print(train_index)
        X_train, X_test = X.iloc[train_index], X.iloc[test_index]
        y_train, y_test = X['multi_class'][train_index], X['multi_class'][test_index]
        w_train, w_test = X['wt_xs'][train_index], X['wt_xs'][test_index]

        print(X_train[(X_train.multi_class == 'ggh')].shape)

        sum_w = X_train['wt_xs'].sum()
        # print 'sum_w', sum_w
        sum_w_cat = X_train.groupby('multi_class')['wt_xs'].sum()
        # print 'sum_w_cat', sum_w_cat
        class_weights = sum_w / sum_w_cat

        class_weight_dict = dict(class_weights)

        print(class_weight_dict)

        # multiply w_train by class_weight now

        for i in w_train.index:
            for key, value in class_weight_dict.items():
            # print 'before: ',index, row
                if y_train[i] == key:
                    # if key == 'ggh':
                    #     w_train.at[i] *= value
                    # else:
                        w_train.at[i] *= value
                    # print 'after dividing by class_weight: ',index, row

        ## use one-hot encoding
        # encode class values as integers
        encoder_train = LabelEncoder()
        encoder_test = LabelEncoder()
        encoder_train.fit(y_train)

        y_train = encoder_train.transform(y_train)

        encoder_test.fit(y_test)
        y_test = encoder_test.transform(y_test)


        X_train = X_train.drop([
            'wt','wt_xs', 'process', 'multi_class','event',
            'gen_match_1', 'gen_match_2','eta_tt',
            # 'jpt_1','jpt_2','dijetpt',
            ], axis=1).reset_index(drop=True)

        X_test = X_test.drop([
            'wt','wt_xs', 'process', 'multi_class','event',
            'gen_match_1', 'gen_match_2','eta_tt',
            # 'jpt_1','jpt_2','dijetpt',
            ], axis=1).reset_index(drop=True)

        # to use names "f0" etcs
        print(X_train.columns)
        orig_columns = X_train.columns
        X_train.columns = ["f{}".format(x) for x in np.arange(X_train.shape[1])]
        X_test.columns = ["f{}".format(x) for x in np.arange(X_train.shape[1])]
        print(X_train.columns)

        ## standard scaler
        # scaler = StandardScaler()
        # np_scaled_fit = scaler.fit(X_train.as_matrix())
        # with open('{}_fold{}_scaler.pkl'.format(channel, fold), 'w') as f:
        #     pickle.dump(scaler, f)
        # np_scaled_train = scaler.transform(X_train.as_matrix())
        # X_scaled_train = pd.DataFrame(np_scaled_train)
        # X_scaled_train.columns = X_train.columns

        # del X_train

        # X_train = X_scaled_train

        # del X_scaled_train

        # np_scaled_test = scaler.transform(X_test.as_matrix())
        # X_scaled_test = pd.DataFrame(np_scaled_test)
        # X_scaled_test.columns = X_test.columns

        # del X_test

        # X_test = X_scaled_test

        # del X_scaled_test


        ## SOME TESTS WITH WEIGHTS
        # w_train *= (sum(w) / sum(w_train))
        # w_test *= (sum(w) / sum(w_test))



        # sum_wpos = np.sum(w_train[y_train == 1])
        # sum_wneg = np.sum(w_train[y_train != 1])
        # ratio = sum_wneg / sum_wpos

        # X_train = X_train.drop(['wt', 'class', 'eta_1', 'eta_2'], axis=1).reset_index(drop=True)
        # X_test = X_test.drop(['wt', 'class', 'eta_1', 'eta_2'], axis=1).reset_index(drop=True)

        # if channel == 'tt':

        # if sig_sample == 'powheg':
        #     params = {
        #             'objective':'multi:softprob',
        #             'max_depth':3,
        #             'min_child_weight':1,
        #             'learning_rate':0.01,
        #             'silent':1,
        #             # 'scale_pos_weight':ratio,
        #             'n_estimators':2000,
        #             'gamma':1.0,
        #             'subsample':0.7,
        #             'colsample_bytree':0.8,
        #             'max_delta_step':1,
        #             'nthread':-1,
        #             'seed':123456
        #             }
        if sig_sample in ['powheg']:
            if analysis == 'sm':
                if channel in ['tt','mt','et','em']:
                    params = {
                            'objective':'multi:softprob',
                            'max_depth':8,
                            # 'min_child_weight':1,
                            'learning_rate':0.05,
                            'silent':1,
                            # 'scale_pos_weight':ratio,
                            'n_estimators':500,
                            'gamma':0,
                            'subsample':0.8,
                            'colsample_bytree':0.8,
                            # 'max_delta_step':3,
                            'nthread':-1,
                            # 'missing':-9999,
                            'seed':123456
                            }
            if analysis == 'cpsm':
                if channel in ['tt','mt','et']:
                    params = {
                            'objective':'multi:softprob',
                            'max_depth':7,
                            # 'min_child_weight':1,
                            'learning_rate':0.05,
                            'silent':1,
                            # 'scale_pos_weight':ratio,
                            'n_estimators':300,
                            # 'gamma':0,
                            'subsample':0.9,
                            # 'colsample_bytree':0.5,
                            # 'max_delta_step':3,
                            'nthread':-1,
                            # 'missing':-9999,
                            'seed':123456
                            }
                if channel in ['em']:
                    params = {
                            'objective':'multi:softprob',
                            'max_depth':7,
                            # 'min_child_weight':1,
                            'learning_rate':0.025,
                            'silent':1,
                            # 'scale_pos_weight':ratio,
                            'n_estimators':150,
                            # 'gamma':0,
                            'subsample':0.9,
                            # 'colsample_bytree':0.5,
                            # 'max_delta_step':3,
                            'nthread':-1,
                            # 'missing':-9999,
                            'seed':123456
                            }
        if sig_sample in ['JHU']:
            if channel in ['tt','mt','et','em']:
                params = {
                        'objective':'multi:softprob',
                        'max_depth':5,
                        # 'min_child_weight':1,
                        'learning_rate':0.025,
                        'silent':1,
                        # 'scale_pos_weight':1,
                        'n_estimators':3000,
                        'gamma':5,
                        'subsample':0.9,
                        'colsample_bylevel':0.6,
                        # 'max_delta_step':5,
                        'nthread':-1,
                        # 'missing':-100.0,
                        'seed':123456
                        }
            # if channel in ['mt','et','em']:
            #     params = {
            #             'objective':'multi:softprob',
            #             'max_depth':5,
            #             # 'min_child_weight':1,
            #             'learning_rate':0.025,
            #             'silent':1,
            #             # 'scale_pos_weight':1,
            #             'n_estimators':3000,
            #             # 'gamma':10,
            #             'subsample':0.9,
            #             # 'colsample_bytree':0.5,
            #             # 'max_delta_step':5,
            #             'nthread':-1,
            #             # 'missing':-9999,
            #             'seed':123456
                        # }
            # if channel in ['et']:
            #     params = {
            #             'objective':'multi:softprob',
            #             'max_depth':4,
            #             # 'min_child_weight':1,
            #             'learning_rate':0.1,
            #             'silent':1,
            #             # 'scale_pos_weight':1,
            #             'n_estimators':10000,
            #             # 'gamma':10,
            #             'subsample':0.9,
            #             # 'colsample_bytree':0.5,
            #             # 'max_delta_step':5,
            #             'nthread':-1,
            #             # 'missing':-9999,
            #             'seed':123456
            #             }
            # if channel in ['em']:
            #     params = {
            #             'objective':'multi:softprob',
            #             'max_depth':5,
            #             # 'min_child_weight':1,
            #             'learning_rate':0.005,
            #             'silent':1,
            #             # 'scale_pos_weight':1,
            #             'n_estimators':3500,
            #             # 'gamma':10,
            #             'subsample':0.9,
            #             # 'colsample_bytree':0.5,
            #             # 'max_delta_step':5,
            #             'nthread':-1,
            #             # 'missing':-9999,
            #             'seed':123456
            #             }


        print(params)
        xgb_clf = xgb.XGBClassifier(**params)


        if sig_sample in ['JHU']:
            if channel in ['tt','mt','et','em']:
                xgb_clf.fit(
                        X_train,
                        y_train,
                        sample_weight = w_train,
                        early_stopping_rounds=50,
                        eval_set=[(X_train, y_train, w_train), (X_test, y_test, w_test)],
                        eval_metric = 'mlogloss',
                        verbose=True
                        )
        if sig_sample in ['powheg']:
            if channel in ['tt','mt','et']:
                xgb_clf.fit(
                        X_train,
                        y_train,
                        sample_weight = w_train,
                        early_stopping_rounds=20,
                        eval_set=[(X_train, y_train, w_train), (X_test, y_test, w_test)],
                        eval_metric = custom_mean_squared_error,
                        verbose=True
                        )
            if channel in ['em']:
                xgb_clf.fit(
                        X_train,
                        y_train,
                        sample_weight = w_train,
                        early_stopping_rounds=30,
                        eval_set=[(X_train, y_train, w_train), (X_test, y_test, w_test)],
                        eval_metric = custom_mean_squared_error,
                        verbose=True
                        )
        # if sig_sample in ['JHU']:
        #     if channel in ['tt','mt','et','em']:
        #         xgb_clf.fit(
        #                 X_train,
        #                 y_train,
        #                 sample_weight = w_train,
        #                 early_stopping_rounds=20,
        #                 eval_set=[(X_train, y_train, w_train), (X_test, y_test, w_test)],
        #                 eval_metric = ['mlogloss'],
        #                 verbose=True
        #                 )

        # evals_result = xgb_clf.evals_result()

        y_predict = xgb_clf.predict(X_test)
        print('true label: {},{},{}'.format(y_test[0],y_test[1],y_test[2]))
        print('predicted label: {},{},{}'.format(y_predict[0],y_predict[1],y_predict[2]))

        print('\n Mean Square Error: {}'.format(mean_squared_error(y_test,y_predict)))

        print(classification_report(
                y_test,
                y_predict,
                # target_names=["background", "signal"],
                target_names=list(encoder_test.classes_),
                sample_weight=w_test
                ))


        y_pred = xgb_clf.predict_proba(X_test)
        print('all probs: {} \n {} \n {}'.format(y_pred[0],y_pred[1],y_pred[2]))
        print('highest proba: {},{},{}'.format(max(y_pred[0]),max(y_pred[1]),max(y_pred[2])))


        # with open('multi_fold{}_{}_{}_{}_xgb.pkl'.format(fold, analysis, channel, sig_sample), 'w') as f:
        #     pickle.dump(xgb_clf, f)

        # Define these so that I can use plot_output()
        # xg_train = xgb.DMatrix(
        #         X_train,
        #         label=y_train,
        #         # missing=-100.0,
        #         weight=w_train
        #         )
        # xg_test = xgb.DMatrix(
        #         X_test,
        #         label=y_test,
        #         # missing=-100.0,
        #         weight=w_test
        #         )

        # pf.plot_output(
        #         xgb_clf.booster(),
        #         xg_train, xg_test,
        #         y_train, y_test,
        #         'multi_{}_{}_output.pdf'.format(channel, sig_sample))

        # pf.plot_features(
        #         xgb_clf.booster(),
        #         'weight',
        #         'multi_fold{}_{}_{}_{}_features_weight.pdf'.format(fold, analysis, channel, sig_sample))

        # pf.plot_features(
        #         xgb_clf.booster(),
        #         'gain',
        #         'multi_fold{}_{}_{}_{}_features_gain.pdf'.format(fold, analysis, channel, sig_sample))


        # y_prediction = xgb_clf.predict(X_test)

        # pf.plot_confusion_matrix(
        #         y_test, y_prediction, w_test,
        #         # classes=['background', 'signal'],
        #         classes=list(encoder_test.classes_),
        #         figname='multi_fold{}_{}_{}_{}_non-normalised_weights_cm.pdf'.format(fold, analysis, channel, sig_sample))

        # pf.plot_confusion_matrix(
        #         y_test, y_prediction, w_test,
        #         classes=list(encoder_test.classes_),
        #         figname='multi_fold{}_{}_{}_{}_normalised_efficiency_weights_cm.pdf'.format(fold, analysis, channel, sig_sample),
        #         normalise_by_col=True)
        # pf.plot_confusion_matrix(
        #         y_test, y_prediction, w_test,
        #         classes=list(encoder_test.classes_),
        #         figname='multi_fold{}_{}_{}_{}_normalised_purity_weights_cm.pdf'.format(fold, analysis, channel, sig_sample),
        #         normalise_by_row=True)


        estimators.append(xgb_clf.best_iteration)
        print(estimators)
        results[test_index] = xgb_clf.predict(X_test)
        score += f1_score(y_test, results[test_index],average='micro',sample_weight=w_test)
    score /= numFolds
    print(score)

    return None


########
def fit_sklearnNN(X, channel, fold, analysis, sig_sample, mjj_training):
    ### TEST A KERAS MODEL


    ## START EDITING THIS FOR ODD/EVEN SPLIT
    print('Training keras model fold{}'.format(fold))

    if mjj_training == "high":
        X = X[X["multi_class"] != "misc"]
        if channel == "em":
            X = X[X["multi_class"] != "qcd"]

        X.multi_class.replace("qqh","ggh",inplace=True)

    X_train,X_test, y_train,y_test,w_train,w_test  = train_test_split(
        X,
        X['multi_class'],
        X['wt_xs'],
        test_size=0.25,
        random_state=123456,
        stratify=X['multi_class'].as_matrix(),
        )
    
    sum_w = X_train['wt_xs'].sum()
    # print 'sum_w', sum_w
    sum_w_cat = X_train.groupby('multi_class')['wt_xs'].sum()
    # print 'sum_w_cat', sum_w_cat
    class_weights = sum_w / sum_w_cat

    class_weight_dict = dict(class_weights)

    print(class_weight_dict)

    # multiply w_train by class_weight now

    for i in w_train.index:
        for key, value in class_weight_dict.items():
            if y_train[i] == key:
                # if key == "ggh" and mjj_training == "high":
                    # w_train.at[i] *= value * 1.5/3.
                    # print 'after multiplying by class_weight: ',w_train.at[i]
                    # wt_mjj = X_train['mjj'].at[i] * 0.003104 - 0.009583 if X_train['mjj'].at[i] > 300 else 1.0 #from ROC until 1500 GeV
                    # w_train.at[i] *= wt_mjj
                # else:
                w_train.at[i] *= value


    ## use one-hot encoding
    # encode class values as integers
    encoder = LabelEncoder()
    encoder.fit(y_train)
    encoded_y_train = encoder.transform(y_train)
    # convert integers to dummy variables (i.e. one hot encoded)
    y_train = np_utils.to_categorical(encoded_y_train, num_classes=3)
    encoder.fit(y_test)
    encoded_y_test = encoder.transform(y_test)
    # convert integers to dummy variables (i.e. one hot encoded)
    y_test = np_utils.to_categorical(encoded_y_test, num_classes=3)
    
    print('original Y: ', X_train['multi_class'].head())
    print('one-hot y: ', y_train[0])


    X_train = X_train.drop([
        'wt','wt_xs', 'process', 'multi_class','event',
        'gen_match_1', 'gen_match_2','opp_sides',
        ], axis=1).reset_index(drop=True)

    X_test = X_test.drop([
        'wt','wt_xs', 'process', 'multi_class','event',
        'gen_match_1', 'gen_match_2','opp_sides',
        ], axis=1).reset_index(drop=True)
    if channel == "em":
        X_train = X_train.drop(["wt_em_qcd"], axis=1).reset_index(drop=True)
        X_test = X_test.drop(["wt_em_qcd"], axis=1).reset_index(drop=True)

    # to use names "f0" etcs
    print(X_train.columns)
    orig_columns = X_train.columns
    X_train.columns = ["f{}".format(x) for x in np.arange(X_train.shape[1])]
    X_test.columns = ["f{}".format(x) for x in np.arange(X_train.shape[1])]
    print(X_train.columns)

    ## standard scaler
    columns = X_train.columns
    scaler = StandardScaler()
    X_train['wt'] = w_train.reset_index(drop=True)
    np_scaled_train = scaler.fit_transform(X_train.as_matrix())
    # with open('{}_{}_scaler.pkl'.format(channel, mjj_training), 'w') as f:
    #     pickle.dump(scaler, f)

    scaled_train = np_scaled_train
    # scaled_train = pd.DataFrame(np_scaled_train)
    # scaled_train.columns = columns

    X_test['wt'] = w_test.reset_index(drop=True)
    np_scaled_test = scaler.transform(X_test.as_matrix())
    scaled_test = np_scaled_test
    # scaled_test = pd.DataFrame(np_scaled_test)
    # scaled_test.columns = columns
    scaled_train = X_train.drop(["wt"], axis=1).reset_index(drop=True)
    scaled_test = X_test.drop(["wt"], axis=1).reset_index(drop=True)


    clf = MLPClassifier(solver='adam', alpha=1e-5, 
            hidden_layer_sizes=(4,), random_state=123456,
            verbose=True, nesterovs_momentum=True)
    clf.fit(scaled_train,
            y_train,
            )

    print(clf.score(scaled_test,y_test,w_test))
    print(clf.predict(scaled_test))

    # with open('keras_model_fold{}_{}_{}_{}_{}_xgb.pkl'.format(fold, analysis, channel, sig_sample, mjj_training), 'w') as f:
    #     pickle.dump(model,f)
    # model.save('keras_model_weights_{}_{}.h5'.format(channel, sig_sample))

    return None

###



def fit_keras(X, channel, fold, analysis, sig_sample, mjj_training):
    ### TEST A KERAS MODEL


    ## START EDITING THIS FOR ODD/EVEN SPLIT
    print('Training keras model fold{}'.format(fold))

    if mjj_training == "high":
        X = X[X["multi_class"] != "misc"]
        if channel == "em":
            X = X[X["multi_class"] != "qcd"]

        X.multi_class.replace("qqh","ggh",inplace=True)


    X["zfeld"] = np.fabs(X.eta_h - (X.jeta_1 + X.jeta_2)/2.)
    # # print X["zfeld"]
    # # make centrality variable
    X["centrality"] = np.exp(-4*(X.zfeld/np.fabs(X.jdeta))**2)

    X["dphi_custom"] = np.arccos(1-X.mt_lep**2/(2.*X.pt_1*X.pt_2))
    X["dR_custom"] = np.sqrt((X.eta_1-X.eta_2)**2 + (X.dphi_custom)**2)

    X_train,X_test, y_train,y_test,w_train,w_test  = train_test_split(
        X,
        X['multi_class'],
        X['wt_xs'],
        test_size=0.25,
        random_state=123456,
        stratify=X['multi_class'].values,
        )
    
    sum_w = X_train['wt_xs'].sum()
    # print 'sum_w', sum_w
    sum_w_cat = X_train.groupby('multi_class')['wt_xs'].sum()
    # print 'sum_w_cat', sum_w_cat
    class_weights = sum_w / sum_w_cat

    class_weight_dict = dict(class_weights)

    print(class_weight_dict)

    # multiply w_train by class_weight now

    for i in w_train.index:
        for key, value in class_weight_dict.items():
            if y_train[i] == key:
                # if key == "ggh" and mjj_training == "high":
                    # w_train.at[i] *= value * 1.5/3.
                    # print 'after multiplying by class_weight: ',w_train.at[i]
                    # wt_mjj = X_train['mjj'].at[i] * 0.003104 - 0.009583 if X_train['mjj'].at[i] > 300 else 1.0 #from ROC until 1500 GeV
                    # w_train.at[i] *= wt_mjj
                # else:
                w_train.at[i] *= value


    ## use one-hot encoding
    # encode class values as integers
    encoder = LabelEncoder()
    encoder.fit(y_train)
    encoded_y_train = encoder.transform(y_train)
    # convert integers to dummy variables (i.e. one hot encoded)
    y_train = np_utils.to_categorical(encoded_y_train, num_classes=3)
    encoder.fit(y_test)
    encoded_y_test = encoder.transform(y_test)
    # convert integers to dummy variables (i.e. one hot encoded)
    y_test = np_utils.to_categorical(encoded_y_test, num_classes=3)
    
    print('original Y: ', X_train['multi_class'].head())
    print('one-hot y: ', y_train[0])


    X_train = X_train.drop([
        'wt','wt_xs', 'process', 'multi_class','event',
        'gen_match_1', 'gen_match_2','opp_sides','zfeld'
        ], axis=1).reset_index(drop=True)

    X_test = X_test.drop([
        'wt','wt_xs', 'process', 'multi_class','event',
        'gen_match_1', 'gen_match_2','opp_sides','zfeld'
        ], axis=1).reset_index(drop=True)
    if channel == "em":
        X_train = X_train.drop(["wt_em_qcd"], axis=1).reset_index(drop=True)
        X_test = X_test.drop(["wt_em_qcd"], axis=1).reset_index(drop=True)

    # to use names "f0" etcs
    print(X_train.columns)
    orig_columns = X_train.columns
    X_train.columns = ["f{}".format(x) for x in np.arange(X_train.shape[1])]
    X_test.columns = ["f{}".format(x) for x in np.arange(X_train.shape[1])]
    print(X_train.columns)

    ## standard scaler
    columns = X_train.columns
    scaler = StandardScaler()
    np_scaled_train = scaler.fit_transform(X_train.values)
    with open('{}_{}_scaler.pkl'.format(channel, mjj_training), 'w') as f:
        pickle.dump(scaler, f)

    scaled_train = np_scaled_train
    # scaled_train = pd.DataFrame(np_scaled_train)
    # scaled_train.columns = columns

    np_scaled_test = scaler.transform(X_test.values)
    scaled_test = np_scaled_test
    # scaled_test = pd.DataFrame(np_scaled_test)
    # scaled_test.columns = columns
    # X_train = X_train.drop(["wt"], axis=1).reset_index(drop=True)
    # X_test = X_test.drop(["wt"], axis=1).reset_index(drop=True)

    min_maxscaler = MinMaxScaler()
    print(w_train)
    scaled_w_train = min_maxscaler.fit_transform(w_train.values.reshape(-1,1))
    print(scaled_w_train)
    scaled_w_test = min_maxscaler.transform(w_test.values.reshape(-1,1))

    print((scaled_w_train.mean()))
    print((scaled_w_train.mean()))

    ## how many features
    num_inputs = scaled_train.shape[1]
    ## how many classes
    num_outputs = 3
    
    model = Sequential()
    model.add(
        Dense(
            200,
            kernel_initializer='glorot_normal',
            activation='tanh',
            kernel_regularizer=l2(1e-4),
            input_dim=num_inputs
            )
        )
    model.add(
        Dense(
            200,
            kernel_initializer='glorot_normal',
            activation='tanh',
            kernel_regularizer=l2(1e-4),
            )
        )
    model.add(
        Dense(
            200,
            init='glorot_normal',
            activation='tanh',
            W_regularizer=l2(1e-4),
            )
        )
    model.add(
        Dense(
            num_outputs,
            kernel_initializer=RandomNormal(),
            activation='softmax'
            )
        )
    model.compile(
        loss='categorical_crossentropy',
        optimizer=Nadam(),
        metrics=['mse']
        )


    ## add early stopping
    callbacks = []
    callbacks.append(
        EarlyStopping(patience=40)
        )

    model.summary()
    model.fit(
        # X_train,
        scaled_train,
        y_train,
        # class_weight=test_class_weight,
        sample_weight=scaled_w_train.squeeze(),
        # validation_data=(X_test,y_test,w_test),
        validation_data=(scaled_test,y_test,scaled_w_test.squeeze()),
        batch_size=1000,
        epochs=10000,
        shuffle=True,
        callbacks=callbacks
        )

    # with open('keras_model_fold{}_{}_{}_{}_{}_xgb.pkl'.format(fold, analysis, channel, sig_sample, mjj_training), 'w') as f:
    #     pickle.dump(model,f)
    model.save('keras_model_fold{}_{}_{}_{}_{}.h5'
            .format(fold, analysis, channel, sig_sample, mjj_training))

    return None

def fit_keras_inc(X, channel, fold, analysis, sig_sample):
    ### TEST A KERAS MODEL

    ## START EDITING THIS FOR ODD/EVEN SPLIT
    print('Training keras model fold{}'.format(fold))

    # sum_w = X_train['wt_xs'].sum()
    # sum_w_cat = X.groupby('multi_class')['wt_xs'].sum()
    # class_weights = sum_w / sum_w_cat

    X = X[X["multi_class"] != "misc"] # don't use misc
    # X.multi_class.replace("qqh","ggh",inplace=True)

    # X["zfeld"] = np.fabs(X.eta_h - (X.jeta_1 + X.jeta_2)/2.)
    # # print X["zfeld"]
    # # make centrality variable
    # X["centrality"] = np.exp(-4*(X.zfeld/np.fabs(X.jdeta))**2)

    # X["dphi_custom"] = np.arccos(1-X.mt_lep**2/(2.*X.pt_1*X.pt_2))
    # X["dR_custom"] = np.sqrt((X.eta_1-X.eta_2)**2 + (X.dphi_custom)**2)

    X.replace(-999.,-10, inplace=True)
    X.replace(-9999.,-10, inplace=True)

    # split
    X_train,X_test, y_train,y_test,w_train,w_test  = train_test_split(
        X,
        X['multi_class'],
        X['wt_xs'],
        test_size=0.25,
        random_state=123456,
        stratify=X['multi_class'].values,
        )
    print(X.head())
    print(X_train.head())
    print(w_train)

    sum_w = X_train['wt_xs'].sum()
    # print 'sum_w', sum_w
    sum_w_cat = X_train.groupby('multi_class')['wt_xs'].sum()
    # print 'sum_w_cat', sum_w_cat
    class_weights = sum_w / sum_w_cat

    class_weight_dict = dict(class_weights)

    print(class_weight_dict)

    # multiply w_train by class_weight now
    for i in w_train.index:
        for key, value in class_weight_dict.items():
            if y_train[i] == key:
                w_train.at[i] *= value

    # replace now to get the weights right still for the individual ones
    # X_train["multi_class"].replace("qqh","ggh",inplace=True)
    # X_test["multi_class"].replace("qqh","ggh",inplace=True)
    # y_train.replace("qqh","ggh",inplace=True)
    # y_test.replace("qqh","ggh",inplace=True)
    min_maxscaler = MinMaxScaler()
    fit_minmax = min_maxscaler.fit(X["wt_xs"].values.reshape(-1,1))


    # Fit the min max scaler on training weights
    scaled_w_train = min_maxscaler.transform(w_train.values.reshape(-1,1))

    ## use one-hot encoding
    # encode class values as integers
    encoder = LabelEncoder()
    encoder.fit(y_train)
    encoded_y_train = encoder.transform(y_train)
    # convert integers to dummy variables (i.e. one hot encoded)
    y_train = np_utils.to_categorical(encoded_y_train, num_classes=len(X_train["multi_class"].unique()))
    # encoder.fit(y_test)
    encoder.classes_
    encoded_y_test = encoder.transform(y_test)
    # convert integers to dummy variables (i.e. one hot encoded)
    y_test = np_utils.to_categorical(encoded_y_test, num_classes=len(X_train["multi_class"].unique()))
    
    print('original Y: ', X_train['multi_class'].head())
    print('one-hot y: ', y_train[0])
    print('one-hot y: ', y_train[1])
    print('one-hot y: ', y_train[2])

    print('original Y: ', X_test['multi_class'].head())
    print('one-hot y: ', y_test[0])
    print('one-hot y: ', y_test[1])
    print('one-hot y: ', y_test[2])

    dropVars = ["wt","wt_xs", "process", "multi_class","event","gen_match_1", "gen_match_2",]
    if sig_sample == "tauspinner":
        dropVars.append("wt_cp_sm")
    if channel == "em":
        dropVars.append("wt_em_qcd")

    X_train = X_train.drop(dropVars, axis=1).reset_index(drop=True)
    X_test = X_test.drop(dropVars, axis=1).reset_index(drop=True)

    # to use names "f0" etcs
    print(X_train.columns)
    orig_columns = X_train.columns
    X_train.columns = ["f{}".format(x) for x in np.arange(X_train.shape[1])]
    X_test.columns = ["f{}".format(x) for x in np.arange(X_train.shape[1])]
    print(X_train.columns)

    ## standard scaler
    columns = X_train.columns
    scaler = StandardScaler()
    np_scaled_train = scaler.fit_transform(X_train.values)
    with open('{}_scaler.pkl'.format(channel), 'w') as f:
        pickle.dump(scaler, f)

    print(X_train)
    scaled_train = np_scaled_train
    # scaled_train = pd.DataFrame(np_scaled_train).reset_index(drop=True)
    # scaled_train.columns = columns
    print(scaled_train)

    np_scaled_test = scaler.transform(X_test.values)
    scaled_test = np_scaled_test
    print(X_test)
    # scaled_test = pd.DataFrame(np_scaled_test).reset_index(drop=True)
    # scaled_test.columns = columns
    print(scaled_test)

    ## how many features
    num_inputs = scaled_train.shape[1]
    ## how many classes
    num_outputs = y_train.shape[1]

    model = Sequential()

    for i, nodes in enumerate([200] * 2):
        if i == 0:
            model.add(Dense(nodes, kernel_regularizer=l2(1e-5), input_dim=num_inputs))
        else:
            model.add(Dense(nodes, kernel_regularizer=l2(1e-5)))
        model.add(Activation("tanh"))
        model.add(Dropout(0.3))

    model.add(Dense(num_outputs, kernel_regularizer=l2(1e-5)))
    model.add(Activation("softmax"))

    model.compile(
        loss="categorical_crossentropy", 
        optimizer=adam(lr=1e-4),
        metrics=["accuracy"]
        )

    w_train = w_train.reset_index(drop=True)
    w_test  = w_test.reset_index(drop=True)

    ## add early stopping
    callbacks = []
    callbacks.append(
        EarlyStopping(patience=50)
        )

    model.summary()
    model.fit(
        # X_train,
        scaled_train,
        y_train,
        # class_weight=test_class_weight,
        # sample_weight=scaled_w_train.squeeze(),
        sample_weight=w_train,
        # validation_data=(X_test,y_test,w_test),
        validation_data=(scaled_test,y_test,w_test),
        # validation_data=(scaled_test,y_test,w_test),
        batch_size=1000,
        epochs=100000,
        shuffle=True,
        callbacks=callbacks
        )

    model.save('keras_model_fold{}_{}_{}_{}.h5'
            .format(fold, analysis, channel, sig_sample))

    ## Plotting things
    y_prediction = model.predict_classes(X_test.values)
    y_test = np.argmax(y_test, axis=1)

    pf.plot_confusion_matrix(
            y_test, y_prediction, w_test.squeeze(),
            classes=list(encoder.classes_),
            figname='multi_fold{}_{}_{}_{}_non-normalised_weights_cm.pdf'.format(fold, analysis, channel, sig_sample))
    pf.plot_confusion_matrix(
            y_test, y_prediction, w_test.squeeze(),
            classes=list(encoder.classes_),
            figname='multi_fold{}_{}_{}_{}_normalised_efficiency_weights_cm.pdf'.format(fold, analysis, channel, sig_sample),
            normalise_by_col=True)
    pf.plot_confusion_matrix(
            y_test, y_prediction, w_test.squeeze(),
            classes=list(encoder.classes_),
            figname='multi_fold{}_{}_{}_{}_normalised_purity_weights_cm.pdf'.format(fold, analysis, channel, sig_sample),
            normalise_by_row=True)

    return None


def fit_tf(X, channel, fold, analysis, sig_sample, mjj_training):
    ### TEST A KERAS MODEL


    ## START EDITING THIS FOR ODD/EVEN SPLIT
    print('Training keras model fold{}'.format(fold))

    if mjj_training == "high":
        X = X[X["multi_class"] != "misc"]

    X_train,X_test, y_train,y_test,w_train,w_test  = train_test_split(
        X,
        X['multi_class'],
        X['wt_xs'],
        test_size=0.25,
        random_state=123456,
        stratify=X['multi_class'].as_matrix(),
        )
    
    sum_w = X_train['wt_xs'].sum()
    # print 'sum_w', sum_w
    sum_w_cat = X_train.groupby('multi_class')['wt_xs'].sum()
    # print 'sum_w_cat', sum_w_cat
    class_weights = sum_w / sum_w_cat

    class_weight_dict = dict(class_weights)

    print(class_weight_dict)

    # multiply w_train by class_weight now

    for i in w_train.index:
        for key, value in class_weight_dict.items():
            if y_train[i] == key:
                if key == "ggh" and mjj_training == "high":
                    # print 'before: ',w_train.at[i]
                    w_train.at[i] *= value * 1.5/3.
                    # print 'after multiplying by class_weight: ',w_train.at[i]
                    wt_mjj = X_train['mjj'].at[i] * 0.003104 - 0.009583 if X_train['mjj'].at[i] > 300 else 1.0 #from ROC until 1500 GeV
                    # wt_mjj = X_train['mjj'].at[i] * 0.01 if X_train['mjj'].at[i] > 500 and X_train['mjj'].at[i] < 1500 else 1.0 #from ROC (slightly higher) until 1500 GeV
                    # wt_mjj = np.sqrt(X_train['mjj'].at[i]) * 0.1368 - 1.3694 # sqrt function
                    # wt_mjj = 1.5 if X_train['mjj'].at[i] > 300 and X_train['mjj'].at[i] < 600 else 1.0  # step function
                    # wt_mjj = ((X_train['mjj'].at[i])**2 * 0.000017 - (X_train['mjj'].at[i] * 0.0017)) #second order poly
                    w_train.at[i] *= wt_mjj
                # elif key == 'qqh' and mjj_training == "high":
                #     w_train.at[i] *= value*0.5
                # elif key == 'ztt_embed' and mjj_training == "high":
                #     w_train.at[i] *= value*0.5
                # elif channel == 'em' and key == 'qcd':
                #     w_train.at[i] *= value*2.0
                else:
                    w_train.at[i] *= value


    ## use one-hot encoding
    # encode class values as integers
    encoder = LabelEncoder()
    encoder.fit(y_train)
    encoded_y_train = encoder.transform(y_train)
    # convert integers to dummy variables (i.e. one hot encoded)
    y_train = np_utils.to_categorical(encoded_y_train, num_classes=4)
    encoder.fit(y_test)
    encoded_y_test = encoder.transform(y_test)
    # convert integers to dummy variables (i.e. one hot encoded)
    y_test = np_utils.to_categorical(encoded_y_test, num_classes=4)
    
    # print w_train
    # minMax = MinMaxScaler()
    # w_train = minMax.fit_transform(w_train)
    # print w_train
    # w_test = minMax.fit_transform(w_test)

    print('original Y: ', X_train['multi_class'].head())
    print('one-hot y: ', y_train[0])


    X_train = X_train.drop([
        'wt','wt_xs', 'process', 'multi_class','event',
        'gen_match_1', 'gen_match_2',
        'mjj_jdeta','dijetpt_pth','dijetpt_jpt1'
        ], axis=1).reset_index(drop=True)

    X_test = X_test.drop([
        'wt','wt_xs', 'process', 'multi_class','event',
        'gen_match_1', 'gen_match_2',
        'mjj_jdeta','dijetpt_pth','dijetpt_jpt1'
        ], axis=1).reset_index(drop=True)
    if channel == "em":
        X_train = X_train.drop(["wt_em_qcd"], axis=1).reset_index(drop=True)
        X_test = X_test.drop(["wt_em_qcd"], axis=1).reset_index(drop=True)

    # to use names "f0" etcs
    print(X_train.columns)
    orig_columns = X_train.columns
    X_train.columns = ["f{}".format(x) for x in np.arange(X_train.shape[1])]
    X_test.columns = ["f{}".format(x) for x in np.arange(X_train.shape[1])]
    print(X_train.columns)

    ## standard scaler
    columns = X_train.columns
    scaler = StandardScaler()
    # X_train['wt'] = w_train
    print(X_train.head(5))
    np_scaled_train = scaler.fit_transform(X_train.as_matrix())
    # with open('{}_{}_scaler.pkl'.format(channel, mjj_training), 'w') as f:
    #     pickle.dump(scaler, f)

    scaled_train = np_scaled_train
    # scaled_train = pd.DataFrame(np_scaled_train)
    # scaled_train.columns = columns

    # X_test['wt'] = w_test
    np_scaled_test = scaler.fit_transform(X_test.as_matrix())
    scaled_test = np_scaled_test
    # scaled_test = pd.DataFrame(np_scaled_test)
    # scaled_test.columns = columns
    # X_train = X_train.drop(["wt"], axis=1).reset_index(drop=True)
    # X_test = X_test.drop(["wt"], axis=1).reset_index(drop=True)


    ## how many features
    num_inputs = scaled_train.shape[1]
    ## how many classes
    num_outputs = 4

    import tensorflow as tf

    # Parameters
    learning_rate = 0.1
    num_steps = 500
    batch_size = 128
    display_step = 100
    
    # Network Parameters
    n_hidden_1 = 256
    n_hidden_2 = 256 
    num_input = scaled_train.shape[1] 
    num_classes = 4 
    
    # tf Graph input
    X = tf.placeholder("float", [None, num_input])
    Y = tf.placeholder("float", [None, num_classes])
    
    # Store layers weight & bias
    weights = {
        'h1': tf.Variable(tf.random_normal([num_input, n_hidden_1])),
        'h2': tf.Variable(tf.random_normal([n_hidden_1, n_hidden_2])),
        'out': tf.Variable(tf.random_normal([n_hidden_2, num_classes]))
    }
    biases = {
        'b1': tf.Variable(tf.random_normal([n_hidden_1])),
        'b2': tf.Variable(tf.random_normal([n_hidden_2])),
        'out': tf.Variable(tf.random_normal([num_classes]))
    }
    
    
    # Create model
    def neural_net(x):
        # Hidden fully connected layer with 256 neurons
        layer_1 = tf.add(tf.matmul(x, weights['h1']), biases['b1'])
        # Hidden fully connected layer with 256 neurons
        layer_2 = tf.add(tf.matmul(layer_1, weights['h2']), biases['b2'])
        # Output fully connected layer with a neuron for each class
        out_layer = tf.matmul(layer_2, weights['out']) + biases['out']
        return out_layer
    
    # Construct model
    logits = neural_net(X)
    prediction = tf.nn.softmax(logits)
    
    # Define loss and optimizer
    loss_op = tf.reduce_mean(tf.nn.softmax_cross_entropy_with_logits(
        logits=logits, labels=Y))
    optimizer = tf.train.AdamOptimizer(learning_rate=learning_rate)
    train_op = optimizer.minimize(loss_op)
    
    # Evaluate model
    correct_pred = tf.equal(tf.argmax(prediction, 1), tf.argmax(Y, 1))
    accuracy = tf.reduce_mean(tf.cast(correct_pred, tf.float32))
    
    # Initialize the variables (i.e. assign their default value)
    init = tf.global_variables_initializer()
    
    # Start training
    with tf.Session() as sess:
    
        # Run the initializer
        sess.run(init)
    
        for step in range(1, num_steps+1):
            batch_x, batch_y = scaled_train.next_batch(batch_size)
            # Run optimization op (backprop)
            sess.run(train_op, feed_dict={X: batch_x, Y: batch_y})
            if step % display_step == 0 or step == 1:
                # Calculate batch loss and accuracy
                loss, acc = sess.run([loss_op, accuracy], feed_dict={X: batch_x,
                                                                     Y: batch_y})
                print(("Step " + str(step) + ", Minibatch Loss= " + \
                      "{:.4f}".format(loss) + ", Training Accuracy= " + \
                      "{:.3f}".format(acc)))
    
        print("Optimization Finished!")
    
        # Calculate accuracy for test 
        print(("Testing Accuracy:", \
            sess.run(accuracy, feed_dict={X: scaled_test,
                                          Y: y_test})))
    
    # with open('keras_model_fold{}_{}_{}_{}_{}_xgb.pkl'.format(fold, analysis, channel, sig_sample, mjj_training), 'w') as f:
    #     pickle.dump(model,f)
    # model.save_weights('keras_model_weights_{}_{}.h5'.format(channel, sig_sample))

    return None

# def fit_pytorch(X, fold, analysis, channel, sig_sample, mjj_training):
#     import torch 
#     import torch.nn as nn

#     ## START EDITING THIS FOR ODD/EVEN SPLIT
#     print 'Training XGBoost model fold{}'.format(fold)
    
#     if mjj_training == "high":
#         X = X[X["multi_class"] != "jetFakes"]
#         X = X[X["multi_class"] != "ztt_embed"]
#         print X.head()

#     # for x in X.columns:
#     #     if x in ["pt_h"]:
#     #         X["exp_{}".format(str(x))] = np.exp(X[str(x)])
#     #         X["log_{}".format(str(x))] = np.log(X[str(x)])
#     #         X["{}_sq".format(str(x))] = X[str(x)]**2
#     #         X["{}_cb".format(str(x))] = X[str(x)]**3
#     #         X["{}_tanh".format(str(x))] = np.tanh(X[str(x)])

#         # make new variable combinatinos
#         X["dphi_custom"] = np.arccos(1-X.mt_lep**2/(2.*X.pt_1*X.pt_2))
#         X["dR_custom"] = np.sqrt((X.eta_1-X.eta_2)**2 + (X.dphi_custom)**2)
#         X["rms_pt"] = np.sqrt(0.5 * (X.pt_1**2 + X.pt_2**2))
#         X["rms_jpt"] = np.sqrt(0.5 * (X.jpt_1**2 + X.jpt_2**2))

    
#         # make zeppenfeld variable
#         X["zfeld"] = np.fabs(X.eta_h - (X.jeta_1 + X.jeta_2)/2.)
#         # print X["zfeld"]
#         # make centrality variable
#         X["centrality"] = np.exp(-4*(X.zfeld/np.fabs(X.jdeta))**2)

#     X_train,X_test, y_train,y_test,w_train,w_test  = train_test_split(
#         X,
#         X['multi_class'],
#         X['wt_xs'],
#         test_size=0.25,
#         random_state=123456,
#         stratify=X['multi_class'].as_matrix(),
#         )

#     print X_train[(X_train.multi_class == 'ggh')].shape
#     del X
#     gc.collect()


#     # if want to plot any variables
#     # pf.plot_signal_background(X[X["multi_class"] == "ggh"], X[X["multi_class"] == "qqh"], "mjj",channel,sig_sample)

#     sum_w = X_train['wt_xs'].sum()
#     # print 'sum_w', sum_w
#     sum_w_cat = X_train.groupby('multi_class')['wt_xs'].sum()
#     # print 'sum_w_cat', sum_w_cat
#     class_weights = sum_w / sum_w_cat

#     class_weight_dict = dict(class_weights)

#     print class_weight_dict

#     # multiply w_train by class_weight now
#     # add mjj dependent weight for ggH

#     for i in w_train.index:
#         for key, value in class_weight_dict.iteritems():
#             if y_train[i] == key:
#                 w_train.at[i] *= value

#     # ## use one-hot encoding
#     # # encode class values as integers
#     # encoder_train = LabelEncoder()
#     # encoder_test = LabelEncoder()
#     # encoder_train.fit(y_train)

#     # y_train = encoder_train.transform(y_train)

#     # encoder_test.fit(y_test)
#     # y_test = encoder_test.transform(y_test)

#     ## use one-hot encoding
#     # encode class values as integers
#     encoder = LabelEncoder()
#     encoder.fit(y_train)
#     encoded_y_train = encoder.transform(y_train)
#     # convert integers to dummy variables (i.e. one hot encoded)
#     y_train = np_utils.to_categorical(encoded_y_train, num_classes=4)
#     encoder.fit(y_test)
#     encoded_y_test = encoder.transform(y_test)
#     # convert integers to dummy variables (i.e. one hot encoded)
#     y_test = np_utils.to_categorical(encoded_y_test, num_classes=4)


#     # test_class_weight = class_weight.compute_class_weight(
#     #     'balanced', np.unique(encoded_Y), encoded_Y
#     #     )
#     # print test_class_weight

#     # print 'original Y: ', X_train['multi_class'].head()
#     # print 'one-hot y: ', y_train

#     print X_train.head(5)

#     X_train = X_train.drop([
#         'wt','wt_xs', 'process', 'multi_class','event',
#         'gen_match_1', 'gen_match_2',#'eta_tt',
#         ], axis=1).reset_index(drop=True)

#     X_test = X_test.drop([
#         'wt','wt_xs', 'process', 'multi_class','event',
#         'gen_match_1', 'gen_match_2',#'eta_tt',
#         ], axis=1).reset_index(drop=True)
#     if channel == "em":
#         X_train = X_train.drop(["wt_em_qcd"], axis=1).reset_index(drop=True)
#         X_test = X_test.drop(["wt_em_qcd"], axis=1).reset_index(drop=True)
#     if mjj_training == "high":
#         X_train = X_train.drop(["dphi_custom","dR","opp_sides"], axis=1).reset_index(drop=True)
#         X_test = X_test.drop(["dphi_custom","dR","opp_sides"], axis=1).reset_index(drop=True)

#     # to use names "f0" etcs
#     print X_train.columns
#     orig_columns = X_train.columns
#     X_train.columns = ["f{}".format(x) for x in np.arange(X_train.shape[1])]
#     X_test.columns = ["f{}".format(x) for x in np.arange(X_train.shape[1])]
#     print X_train.columns

#     ## standard scaler
#     columns = X_train.columns
#     scaler = StandardScaler()
#     # X_train['wt'] = w_train
#     print X_train.head(5)
#     np_scaled_train = scaler.fit_transform(X_train.as_matrix())
#     # with open('{}_{}_scaler.pkl'.format(channel, mjj_training), 'w') as f:
#     #     pickle.dump(scaler, f)

#     scaled_train = np_scaled_train
#     scaled_train = pd.DataFrame(np_scaled_train)
#     scaled_train.columns = columns

#     # X_test['wt'] = w_test
#     np_scaled_test = scaler.fit_transform(X_test.as_matrix())
#     scaled_test = np_scaled_test
#     scaled_test = pd.DataFrame(np_scaled_test)
#     scaled_test.columns = columns
#     # X_train = X_train.drop(["wt"], axis=1).reset_index(drop=True)
#     # X_test = X_test.drop(["wt"], axis=1).reset_index(drop=True)


#     # X_train = X_train.drop([
#     #     'zfeld','jeta_1','jeta_2'
#     #     ], axis=1).reset_index(drop=True)

#     # X_test = X_test.drop([
#     #     'zfeld','jeta_1','jeta_2'
#     #     ], axis=1).reset_index(drop=True)


#     num_input = scaled_train.shape[1] 
#     # Defining input size, hidden layer size, output size and batch size
#     # respectively
#     n_in, n_h, n_out, batch_size = num_input, 2, 2, 64

#     # Create a model
#     model = nn.Sequential(
#                 nn.Linear(n_in, n_h),
#                 nn.ReLU(),
#                 nn.Linear(n_h, n_out),
#                 nn.Sigmoid()
#                 )

#     # Construct the loss function
#     criterion = torch.nn.MSELoss()

#     # Construct the optimizer (Stochastic Gradient Descent in this case)
#     optimizer = torch.optim.SGD(model.parameters(), lr=0.01)

#     # Gradient Descent
#     for epoch in range(50):
#         # Forward pass: Compute predicted y by passing x to the model
#         y_pred = model(scaled_train)
    
#         # Compute and print loss
#         loss = criterion(y_pred, y_train)
#         print('epoch: ', epoch,' loss: ', loss.item())
    
#         # Zero gradients, perform a backward pass, and update the weights.
#         optimizer.zero_grad()
        
#         # perform a backward pass (backpropagation)
#         loss.backward()
        
#         # Update the parameters
#         optimizer.step()

#     return None

def write_score(data, model, channel, doSystematics):

    path = '/vols/cms/akd116/Offline/output/SM/2018/Mar18' # path of nominal ntuples

    # for full systematics need this:
    systematics = [
            'TSCALE_UP', 'TSCALE_DOWN', 'TSCALE0PI_UP', 'TSCALE0PI_DOWN', 'TSCALE1PI_UP',
            'TSCALE1PI_DOWN', 'TSCALE3PRONG_UP', 'TSCALE3PRONG_DOWN' , 'JES_UP', 'JES_DOWN',
            'EFAKE0PI_DOWN', 'EFAKE0PI_UP', 'EFAKE1PI_DOWN', 'EFAKE1PI_UP', 'MUFAKE0PI_DOWN' ,
            'MUFAKE0PI_UP', 'MUFAKE1PI_DOWN', 'MUFAKE1PI_UP', 'METUNCL_UP', 'METUNCL_DOWN',
            'METCL_UP', 'METCL_DOWN',

            # 'TSCALE_UP_1', 'TSCALE_UP_2', 'TSCALE_DOWN_2', 'TSCALE_UP_3', 'TSCALE_DOWN_3',
            # 'TSCALE_UP_0.5', 'TSCALE_DOWN_0.5', 'TSCALE_UP_1.5', 'TSCALE_DOWN_1.5', 'TSCALE_UP_2.5',
            # 'TSCALE_DOWN_2.5', 'BTAG_UP', 'BTAG_DOWN', 'BFAKE_UP', 'BFAKE_DOWN',
            # 'HF_UP', 'HF_DOWN', 'HFSTATS1_UP', 'HFSTATS1_DOWN', 'HFSTATS2_UP',
            # 'HFSTATS2_DOWN', 'CFERR1_UP', 'CFERR1_DOWN', 'CFERR2_UP', 'CFERR2_DOWN',
            # 'LF_UP', 'LF_DOWN', 'LFSTATS1_UP', 'LFSTATS1_DOWN', 'LFSTATS2_UP',
            # 'LFSTATS2_DOWN', 'MET_SCALE_UP', 'MET_SCALE_DOWN', 'MET_RES_UP', 'MET_RES_DOWN',
            ]


    if len(data) > 0:
        gb = data.groupby('process')
        df_dict = {x: gb.get_group(x) for x in gb.groups}

    score = []
    for key, value in df_dict.items():
        print('Writing into {}_{}_2016.root'.format(key, channel))
        value = value.drop(['process'], axis=1)
        if len(data) > 0:
            score = model.predict_proba(value)[:,1]
        else:
            score = np.array(0.0)

        score.dtype = [('mva_score', np.float32)]
        array2root(
            score,
            '{}/{}_{}_2016.root'.format(path, key, channel),
            'ntuple',
            mode = 'update'
            )


        if doSystematics:
            for systematic in systematics:
                print('Writing into {}/{}_{}_2016.root'.format(systematic, key, channel))

                array2root(
                    score,
                    '{}/{}/{}_{}_2016.root'.format(path, systematic, key, channel),
                    'ntuple',
                    mode = 'update'
                    )

    return None




def write_score_multi(data, model, analysis, channel, sig_sample, doSystematics, name):
    ## START EDITING THIS

    path = '/vols/cms/akd116/Offline/output/SM/2018/Mar19' # nominal ntuples

    # for full systematics need this:
    systematics = [
            'TSCALE_UP', 'TSCALE_DOWN', 'TSCALE0PI_UP', 'TSCALE0PI_DOWN', 'TSCALE1PI_UP',
            'TSCALE1PI_DOWN', 'TSCALE3PRONG_UP', 'TSCALE3PRONG_DOWN' , 'JES_UP', 'JES_DOWN',
            'EFAKE0PI_DOWN', 'EFAKE0PI_UP', 'EFAKE1PI_DOWN', 'EFAKE1PI_UP', 'MUFAKE0PI_DOWN' ,
            'MUFAKE0PI_UP', 'MUFAKE1PI_DOWN', 'MUFAKE1PI_UP', 'METUNCL_UP', 'METUNCL_DOWN',
            'METCL_UP', 'METCL_DOWN',

            # 'TSCALE_UP_1', 'TSCALE_UP_2', 'TSCALE_DOWN_2', 'TSCALE_UP_3', 'TSCALE_DOWN_3',
            # 'TSCALE_UP_0.5', 'TSCALE_DOWN_0.5', 'TSCALE_UP_1.5', 'TSCALE_DOWN_1.5', 'TSCALE_UP_2.5',
            # 'TSCALE_DOWN_2.5', 'BTAG_UP', 'BTAG_DOWN', 'BFAKE_UP', 'BFAKE_DOWN',
            # 'HF_UP', 'HF_DOWN', 'HFSTATS1_UP', 'HFSTATS1_DOWN', 'HFSTATS2_UP',
            # 'HFSTATS2_DOWN', 'CFERR1_UP', 'CFERR1_DOWN', 'CFERR2_UP', 'CFERR2_DOWN',
            # 'LF_UP', 'LF_DOWN', 'LFSTATS1_UP', 'LFSTATS1_DOWN', 'LFSTATS2_UP',
            # 'LFSTATS2_DOWN', 'MET_SCALE_UP', 'MET_SCALE_DOWN', 'MET_RES_UP', 'MET_RES_DOWN',
            ]


    if len(data) > 0:
        gb = data.groupby('process')
        df_dict = {x: gb.get_group(x) for x in gb.groups}

    score = []
    for key, value in df_dict.items():
        print('Writing into {}_{}_2016.root'.format(key, channel))
        value = value.drop(['process'], axis=1)
        if len(data) > 0:
            # assign event to max score class
            # print model.predict_proba(value)
            # print model.predict(value)

            for index, ls in enumerate(model.predict_proba(value)):
                # print index
                # print ls
                score.append(max(ls))
                # print score

            np_score = np.array(score)
            cat = np.array(model.predict(value))

        else:
            np_score = np.array(0.0)
            cat = ''

        if sig_sample == 'powheg':
            np_score.dtype = [('mva_score_{}_{}_powheg'.format(analysis, name), np.float32)]
            cat.dtype = [('mva_cat_{}_{}_powheg'.format(analysis, name), np.int)]
        elif sig_sample == 'JHU':
            np_score.dtype = [('mva_score_{}_{}_JHU'.format(analysis, name), np.float32)]
            cat.dtype = [('mva_cat_{}_{}_JHU'.format(analysis, name), np.int)]

        array2root(
            np_score,
            '{}/{}_{}_2016.root'.format(path, key, channel),
            'ntuple',
            mode = 'update'
            )
        array2root(
            cat,
            '{}/{}_{}_2016.root'.format(path, key, channel),
            'ntuple',
            mode = 'update'
            )


        if doSystematics:
            for systematic in systematics:
                print('Writing into {}/{}_{}_2016.root'.format(systematic, key, channel))

                array2root(
                    np_score,
                    '{}/{}/{}_{}_2016.root'.format(path, systematic, key, channel),
                    'ntuple',
                    mode = 'update'
                    )
                array2root(
                    cat,
                    '{}/{}/{}_{}_2016.root'.format(path, systematic, key, channel),
                    'ntuple',
                    mode = 'update'
                    )


    return None


def write_score_multi_folds(data, model, analysis, channel, sig_sample, fold, name):
    ## START EDITING THIS

    path = '/vols/cms/akd116/Offline/output/SM/2018/Apr23' # nominal ntuples

    if len(data) > 0:
        gb = data.groupby('process')
        df_dict = {x: gb.get_group(x) for x in gb.groups}

    score = []
    for key, value in df_dict.items():
        print('Writing into {}_{}_2016.root'.format(key, channel))
        value = value.drop(['process'], axis=1)
        if len(data) > 0:
            # assign event to max score class
            # print model.predict_proba(value)
            # print model.predict(value)

            for index, ls in enumerate(model.predict_proba(value)):
                # print index
                # print ls
                score.append(max(ls))
                # print score

            np_score = np.array(score)
            cat = np.array(model.predict(value))

        else:
            np_score = np.array(0.0)
            cat = ''

        if sig_sample == 'powheg':
            np_score.dtype = [('mva_score_{}_{}_{}_powheg'.format(fold, analysis, name), np.float32)]
            cat.dtype = [('mva_cat_{}_{}_{}_powheg'.format(fold, analysis, name), np.int)]
        elif sig_sample == 'JHU':
            np_score.dtype = [('mva_score_{}_{}_{}_JHU'.format(fold, analysis, name), np.float32)]
            cat.dtype = [('mva_cat_{}_{}_{}_JHU'.format(fold, analysis, name), np.int)]

        array2root(
            np_score,
            '{}/{}_{}_2016.root'.format(path, key, channel),
            'ntuple',
            mode = 'update'
            )
        array2root(
            cat,
            '{}/{}_{}_2016.root'.format(path, key, channel),
            'ntuple',
            mode = 'update'
            )

    return None


def write_score_multi_syst(data, model, analysis, channel, sig_sample, fold, doSystematics, name):
    ## START EDITING THIS

    path = '/vols/cms/akd116/Offline/output/SM/2018/Apr23' # nominal ntuples

    # for full systematics need this:
    systematics = [
            'TSCALE_UP', 'TSCALE_DOWN', 'TSCALE0PI_UP', 'TSCALE0PI_DOWN', 'TSCALE1PI_UP',
            'TSCALE1PI_DOWN', 'TSCALE3PRONG_UP', 'TSCALE3PRONG_DOWN' , 'JES_UP', 'JES_DOWN',
            'EFAKE0PI_DOWN', 'EFAKE0PI_UP', 'EFAKE1PI_DOWN', 'EFAKE1PI_UP', 'MUFAKE0PI_DOWN' ,
            'MUFAKE0PI_UP', 'MUFAKE1PI_DOWN', 'MUFAKE1PI_UP', 'METUNCL_UP', 'METUNCL_DOWN',
            'METCL_UP', 'METCL_DOWN',
            ]


    if len(data) > 0:
        gb = data.groupby('process')
        df_dict = {x: gb.get_group(x) for x in gb.groups}

    score = []
    for key, value in df_dict.items():
        print('Writing into {}_{}_2016.root'.format(key, channel))
        value = value.drop(['process'], axis=1)
        if len(data) > 0:
            # assign event to max score class
            # print model.predict_proba(value)
            # print model.predict(value)

            for index, ls in enumerate(model.predict_proba(value)):
                # print index
                # print ls
                score.append(max(ls))
                # print score

            np_score = np.array(score)
            cat = np.array(model.predict(value))

        else:
            np_score = np.array(0.0)
            cat = ''

        if sig_sample == 'powheg':
            np_score.dtype = [('mva_score_{}_{}_{}_powheg'.format(fold, analysis, name), np.float32)]
            cat.dtype = [('mva_cat_{}_{}_{}_powheg'.format(fold, analysis, name), np.int)]
        elif sig_sample == 'JHU':
            np_score.dtype = [('mva_score_{}_{}_{}_JHU'.format(fold, analysis, name), np.float32)]
            cat.dtype = [('mva_cat_{}_{}_{}_JHU'.format(fold, analysis, name), np.int)]

        array2root(
            np_score,
            '{}/{}_{}_2016.root'.format(path, key, channel),
            'ntuple',
            mode = 'update'
            )
        array2root(
            cat,
            '{}/{}_{}_2016.root'.format(path, key, channel),
            'ntuple',
            mode = 'update'
            )


        if doSystematics:
            for systematic in systematics:
                print('Writing into {}/{}_{}_2016.root'.format(systematic, key, channel))

                array2root(
                    np_score,
                    '{}/{}/{}_{}_2016.root'.format(path, systematic, key, channel),
                    'ntuple',
                    mode = 'update'
                    )
                array2root(
                    cat,
                    '{}/{}/{}_{}_2016.root'.format(path, systematic, key, channel),
                    'ntuple',
                    mode = 'update'
                    )


    return None

def compute_class_weights(df):#, channel, sig_sample):
    # calculate sum of all event weights per category
    print(df['wt'])
    sum_w = df['wt'].sum()
    print(sum_w)

    class_weights = []
    # calculate sum of event weights per category
    for cat in df['multi_class']:
        sum_w_cat = df['wt'].sum()
        try:
            weights = sum_w / sum_w_cat
            return class_weights.append(weights)
        except ZeroDivisionError:
            'Cannot divide by zero'


