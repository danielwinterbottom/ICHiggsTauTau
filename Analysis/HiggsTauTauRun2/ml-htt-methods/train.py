# Usage:
#     python train.py --analysis cpsm --sig_sample madgraph --mjj_training low --mode xgb_multi --channel tt --kfold --fold 0
# cp decays inc training
#     python train.py --analysis cpsm --sig_sample powheg --mode xgb_multi --channel tt --kfold --fold 0 --inc --era 2017 

# rho ID
# python train.py --analysis cpsm --mode rho_ttsplit --channel tt --kfold --fold 0

# noise jet ID
# python train.py --analysis cpsm --mode noisejet_ttsplit --channel tt --kfold --fold 0

import random
import uproot
import xgboost as xgb
import pandas as pd
import numpy as np
import pickle
import argparse
from scipy import interp
import json
from pandas.core.groupby import GroupBy
# import seaborn as sns

from sklearn.utils import class_weight
from sklearn.metrics import classification_report
from sklearn.metrics import roc_auc_score
from sklearn.metrics import roc_curve
from sklearn.metrics import auc
from sklearn.metrics import recall_score
from sklearn.metrics import precision_score
from sklearn.model_selection import train_test_split
from sklearn.model_selection import StratifiedKFold
from pandas.plotting import scatter_matrix
from sklearn.metrics import confusion_matrix
from sklearn.preprocessing import StandardScaler

# custom modules
import plot_functions as pf
import fit_functions as ff


def parse_arguments():
    parser = argparse.ArgumentParser()

    parser.add_argument('--mode', action='store', default='sklearn_ttsplit',
        help='training procedure (default train_test_split)')
    parser.add_argument('--channel', action='store', default='mt',
        help='channels to train on')
    parser.add_argument('--sig_sample', action='store', default='powheg',
        help='''ggh signal sample to run on (default powheg)\n
        choose powheg for n_jets < 2 | (n_jets >= 2 & mjj < 300)\n
        choose JHU for n_jets >=2 & mjj > 300\n
        choose madgraph for new sig samples''')
    parser.add_argument('--kfold', action='store_true', default=False,
        dest='kfold', help='use kfold (default False)')
    parser.add_argument('--fold', action='store', default=0,
        help='which fold to train (default 0)')
    parser.add_argument('--cv', action='store_true', default=False,
        dest='cv', help='use cv (default False)')
    parser.add_argument('--analysis', action='store', default='cpsm',
        dest='analysis', help='what analysis to make dataset for (default cpsm)')
    parser.add_argument('--mjj_training', action='store', default='low',
        dest='mjj_training', help='Do training for high Mjj or low Mjj events?')
    parser.add_argument('--inc', action='store_true', default=False,
        dest='inc', help='Do inclusive training?')
    parser.add_argument('--era', action='store', default='2016',
        dest='era', help='Which year?')
    parser.add_argument('--splitByDM', action='store', default=None,
        dest='splitByDM', help='Split by DM?')

    return parser.parse_args()



def main(opt):

    if opt.mode == 'sklearn_ttsplit':
        train_data = pd.read_hdf('data/dataset_{}_{}_{}.hdf5'
                .format(opt.analysis, opt.channel, opt.sig_sample))

        ff.fit_ttsplit(train_data, opt.channel, opt.sig_sample)

    if opt.mode == 'ttsplit':
        train_data = pd.read_hdf('data/binary_dataset_fold{}_{}_{}.hdf5'
                .format(opt.fold, opt.analysis, opt.channel))

        ff.fit_ttsplit(train_data, opt.channel, opt.fold)

    if opt.mode == 'rho_ttsplit':
        train_data = pd.read_hdf('RhoID/binary_dataset_fold{}_{}_{}.hdf5'
                .format(opt.fold, opt.analysis, opt.channel))

        ff.fit_rhottsplit(train_data, opt.channel, opt.fold)

    if opt.mode == 'noisejet_ttsplit':
        train_data = pd.read_hdf('noisejetID/binary_dataset_fold{}_{}_{}.hdf5'
                .format(opt.fold, opt.analysis, opt.channel))

        ff.fit_noisejets_ttsplit(train_data, opt.channel, opt.fold)

    if opt.mode == 'sklearn_sssplit':

        ff.fit_sssplit(train_data, 4, opt.channel, opt.sig_sample)

    if opt.mode == 'gbc_ttsplit':

        ff.fit_gbc_ttsplit(train_data, opt.channel, opt.sig_sample)

    if opt.mode == 'sklearnNN_multi':

        train_fold = pd.read_hdf('data_Aug14Danny/dataset_fold{}_{}_{}_{}_{}.hdf5'
                .format(opt.fold, opt.analysis, opt.channel, opt.sig_sample,opt.mjj_training))
        ff.fit_sklearnNN(train_fold, opt.channel, opt.fold, opt.analysis, opt.sig_sample, opt.mjj_training)

    if opt.mode == 'keras_multi':

        if not opt.inc:
            train_fold = pd.read_hdf('data_Aug14Danny/dataset_fold{}_{}_{}_{}_{}.hdf5'
                    .format(opt.fold, opt.analysis, opt.channel, opt.sig_sample,opt.mjj_training))
            ff.fit_keras(train_fold, opt.channel, opt.fold, opt.analysis, opt.sig_sample, opt.mjj_training)
        elif opt.inc:
            train_fold = pd.read_hdf('data_CPdecay2016/dataset_fold{}_{}_{}_{}.hdf5' # Feb26
                    .format(opt.fold, opt.analysis, opt.channel, opt.era))
            ff.fit_keras_inc(train_fold, opt.channel, opt.fold, opt.analysis, opt.sig_sample)

    if opt.mode == 'tf':

        train_fold = pd.read_hdf('data_Aug14Danny/dataset_fold{}_{}_{}_{}_{}.hdf5'
                .format(opt.fold, opt.analysis, opt.channel, opt.sig_sample,opt.mjj_training))
        ff.fit_tf(train_fold, opt.channel, opt.fold, opt.analysis, opt.sig_sample, opt.mjj_training)

    if opt.mode == 'pytorch':

        train_fold = pd.read_hdf('data_Aug14Danny/dataset_fold{}_{}_{}_{}_{}.hdf5'
                .format(opt.fold, opt.analysis, opt.channel, opt.sig_sample,opt.mjj_training))
        ff.fit_pytorch(train_fold, opt.channel, opt.fold, opt.analysis, opt.sig_sample, opt.mjj_training)

    if opt.mode == 'xgb_multi':

        if not opt.kfold:
            ff.fit_multiclass_ttsplit(train_data, opt.analysis, opt.channel, opt.sig_sample)

        elif opt.inc:
            # if opt.era == "2018":
            #     # train_fold = pd.read_hdf('data_tauspinner_2018/dataset_fold{}_{}_{}_{}.hdf5'
            #     #         .format(opt.fold, opt.analysis, opt.channel, opt.era))
            #     # train_fold = pd.read_hdf('data_tauspinner_12Mar2020_2018_NopT2dijetpT/dataset_fold{}_{}_{}_{}.hdf5'
            #     train_fold = pd.read_hdf('data_tauspinner_08Apr2020_2018/dataset_fold{}_{}_{}_{}.hdf5'
            #             .format(opt.fold, opt.analysis, opt.channel, opt.era))
            # if opt.era == "2017":
            #     # train_fold = pd.read_hdf('data_tauspinner_2017/dataset_fold{}_{}_{}_{}.hdf5'
            #     #         .format(opt.fold, opt.analysis, opt.channel, opt.era))
            #     # train_fold = pd.read_hdf('data_tauspinner_12Mar2020_2017/dataset_fold{}_{}_{}_{}.hdf5'
            #     train_fold = pd.read_hdf('data_tauspinner_08Apr2020_2017/dataset_fold{}_{}_{}_{}.hdf5'
            #             .format(opt.fold, opt.analysis, opt.channel, opt.era))
            # if opt.era == "2016":
            #     # previous Feb13 model
            #     # train_fold = pd.read_hdf('data_Dec05/dataset_fold{}_{}_{}_{}.hdf5'
            #     #         .format(opt.fold, opt.analysis, opt.channel, opt.era))

            #     # PS tauspinner
            #     # train_fold = pd.read_hdf('data_CPdecay2016_trainOnPS/dataset_fold{}_{}_{}_{}.hdf5'
            #     #         .format(opt.fold, opt.analysis, opt.channel, opt.era))

            #     # SM tauspinner
            #     # train_fold = pd.read_hdf('data_CPdecay2016_trainOnSM/dataset_fold{}_{}_{}_{}.hdf5'
            #     # train_fold = pd.read_hdf('data_CPdecay2016_SM_splitByDM/dataset_fold{}_{}_{}_{}.hdf5'
            #     #         .format(opt.fold, opt.analysis, opt.channel, opt.era, opt.splitByDM))
            #     # train_fold = pd.read_hdf('data_tauspinner_12Mar2020_2016/dataset_fold{}_{}_{}_{}.hdf5'
            #     train_fold = pd.read_hdf('data_tauspinner_08Apr2020_2018/dataset_fold{}_{}_{}_{}.hdf5'
            #             .format(opt.fold, opt.analysis, opt.channel, opt.era))

                # to use SM analysis like variables
                # train_fold = pd.read_hdf('data_CPdecay2016/dataset_fold{}_{}_{}_{}.hdf5' # Feb26
                #         .format(opt.fold, opt.analysis, opt.channel, opt.era))

            # train_fold = pd.read_hdf('data_tauspinner_11May2020_{}/dataset_fold{}_{}_{}_{}.hdf5'
            train_fold = pd.read_hdf('data_tauspinner_01Jun2020_{}/dataset_fold{}_{}_{}_{}.hdf5'
                    .format(opt.era, opt.fold, opt.analysis, opt.channel, opt.era))
            print('train_fold used: dataset_fold{}_{}_{}_{}.hdf5'.format(opt.fold, opt.analysis, opt.channel, opt.era))
            print(train_fold.shape)
            ff.fit_multiclass_kfold_inc(train_fold, opt.fold, opt.analysis, opt.channel, opt.sig_sample, opt.era)
        else:
            train_fold = pd.read_hdf('data_Dec05/dataset_fold{}_{}_{}_{}_{}.hdf5'
                    .format(opt.fold, opt.analysis, opt.channel, opt.sig_sample, opt.mjj_training))
            print('train_fold used:  data_Dec05/dataset_fold{}_{}_{}_{}_{}.hdf5'.format(opt.fold, opt.analysis, opt.channel, opt.sig_sample, opt.mjj_training))
            print(train_fold.shape)

            if not opt.cv:
                ff.fit_multiclass_kfold(train_fold, opt.fold, opt.analysis, opt.channel, opt.sig_sample, opt.mjj_training)
            else:
                ff.fit_multiclass_cvkfold(train_fold, opt.fold, opt.analysis, opt.channel, opt.sig_sample)

        if opt.fold not in [0,1]:
            assert ValueError('Fold{} not found'.format(opt.fold))



if __name__ == "__main__":
    opt = parse_arguments()
    main(opt)

