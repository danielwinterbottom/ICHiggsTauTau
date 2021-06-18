# Usage:
#     python make_dataset.py -c --analysis cpsm --sig_sample powheg --mjj_training low --split --mode xgb_multi --channel tt --kfold
# with embedding and FF
#     python make_dataset.py -c --analysis cpsm --sig_sample powheg --mjj_training low --split --mode xgb_multi --channel tt --kfold --embedding --ff
# with new sig samples
#     python make_dataset.py -c --analysis cpsm --sig_sample madgraph --mjj_training low --split --mode xgb_multi --channel tt --kfold --embedding --ff
# cp decays inc training datasets
# python make_dataset.py -c --analysis cpsm --sig_sample powheg --mode xgb_multi --channel tt --kfold --embedding --ff --era 2017 --inc 

import random
import uproot
import xgboost as xgb
import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
import pickle
import argparse
from scipy import interp
import json
from pandas.core.groupby import GroupBy
# import seaborn as sns
import yaml

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
from sklearn.preprocessing import MinMaxScaler

# custom modules
import plot_functions as pf
import load_functions as lf
import fit_functions as ff

def parse_arguments():
    parser = argparse.ArgumentParser()

    parser.add_argument('-c', action='store_true', default=False,
        dest='apply_selection', help='apply the preselection (default False)')
    parser.add_argument('--mode', action='store', default='sklearn_ttsplit',
        help='training procedure (default train_test_split)')
    parser.add_argument('--channel', action='store', default='mt',
        help='channels to train on')
    parser.add_argument('--sig_sample', action='store', default='tauspinner',
        help='''ggh signal sample to run on (default powheg)\n
        choose powheg for n_jets < 2 | (n_jets >= 2 & mjj < 300)\n
        choose JHU for n_jets >=2 & mjj > 300\n
        choose madgraph for new sig samples''')
    parser.add_argument('--kfold', action='store_true', default=False,
        dest='kfold', help='apply kfold on dataset (default False)')
    parser.add_argument('--split', action='store_true', default=False,
        dest='split', help='split by powheg/JHU sample (default False)')
    parser.add_argument('--analysis', action='store', default='cpsm',
        dest='analysis', help='what analysis to make dataset for (default cpsm)')
    parser.add_argument('--use_jet_variables', action='store_true', default=False,
        dest='use_jet_variables', help='whether to use jet variables or not')
    parser.add_argument('--embedding', action='store_true', default=True,
        dest='embedding', help='Use embedded samples?')
    parser.add_argument('--ff', action='store_true', default=True,
        dest='ff', help='Use FF method?')
    parser.add_argument('--mjj_training', action='store', default='low',
        dest='mjj_training', help='Do training for high Mjj or low Mjj events?')
    parser.add_argument('--era', action='store', default='2016',
        dest='era', help='Which year to use?')
    parser.add_argument('--inc', action='store_true', default=False,
        dest='inc', help='Train inclusively (mjj doesnt matter)?')


    return parser.parse_args()

def main(opt):
    ## To create dataset for chosen channel & sig_sample (i.e. boosted or dijet cat)

    print '\nCreate dataset for {} channel with {} sig samples for {} mjj training\n'.format(opt.channel, opt.sig_sample, opt.mjj_training)
    if opt.sig_sample == "tauspinner":
        sig_files = lf.load_files('./filelist/sig_{}_files.dat'.format(opt.sig_sample))
    elif opt.sig_sample != "madgraph":
        sig_files = lf.load_files('./filelist/sig_{}_files_{}.dat'.format(opt.sig_sample, opt.era))
    else:
        sig_files = lf.load_files('./filelist/sig_{}_{}_files.dat'.format(opt.sig_sample, opt.mjj_training))

    bkg_files = lf.load_files('./filelist/slimmed_bkgs_files_{}.dat'.format(opt.era))
    data_files = lf.load_files('./filelist/{}_data_files_{}.dat'.format(opt.channel, opt.era))
    if opt.embedding:
        embed_files = lf.load_files("./filelist/embed_{}_files_{}.dat".format(opt.channel, opt.era))

    # this file contains information about the xsections, lumi and event numbers
    if opt.era == "2016":
        params_file = json.load(open('params_leg2016.json'))
    elif opt.era == "2017":
        params_file = json.load(open('params_2017.json'))
    elif opt.era == "2018":
        params_file = json.load(open('params_2018.json'))
    lumi = params_file['MuonEG']['lumi']

    # cut_features will only be used for preselection
    # and then dropped again
    if opt.channel == 'tt':
        cut_features = [
                'deepTauVsJets_medium_1','deepTauVsJets_medium_2',
                'deepTauVsJets_vvvloose_1','deepTauVsJets_vvvloose_2',
                'deepTauVsEle_vvvloose_1','deepTauVsEle_vvvloose_2',
                'deepTauVsMu_vloose_1','deepTauVsMu_vloose_2',
                'leptonveto', 'trg_doubletau',
                ]
        if opt.ff:
            cut_features.extend([
                # 'mva_olddm_vloose_1', 'mva_olddm_vloose_2',
                'wt_ff_1',
                # 'wt_ff_2','wt_ff_dmbins_1',
                'mva_dm_1','mva_dm_2',
                ])


    elif opt.channel == 'mt':
        cut_features = [
                'iso_1',
                'deepTauVsJets_medium_1','deepTauVsJets_medium_2',
                'deepTauVsJets_vvvloose_1','deepTauVsJets_vvvloose_2',
                'deepTauVsEle_vvvloose_2',
                'deepTauVsMu_vloose_2',
                'deepTauVsMu_tight_2',
                'leptonveto',
                'trg_singlemuon', 'trg_mutaucross',
                'os',
                'n_bjets',
                ]
        if opt.ff:
            cut_features.extend([
                'wt_ff_1','mva_olddm_vloose_2','wt_ff_dmbins_1',
                ])

    elif opt.channel == 'et':
        cut_features = [
                'iso_1',
                'mva_olddm_tight_2',
                'antiele_2', 'antimu_2',
                'leptonveto',
                'trg_singleelectron',
                'os',
                ]
        if opt.ff:
            cut_features.extend([
                'wt_ff_1','mva_olddm_vloose_2',
                ])

    elif opt.channel == 'em':
        cut_features = [
                'iso_1',
                'iso_2',
                'leptonveto',
                'trg_muonelectron',
                'os',
                ]
    # if opt.analysis == 'cpsm' and not opt.use_jet_variables:
    #     cut_features.append('mjj')

    # features to train on
    # apart from 'wt' - this is used for weights
    # still need to multipy 'wt' by the scaling factor
    # coming from the xsection

    if opt.mode in ['keras_multi', 'xgb_multi']:
        if opt.analysis == 'cpsm':
            features = [
                'pt_1', 'pt_2',
                'mt_1', 'mt_2', 'mt_lep',
                'm_vis', 'm_sv', 'eta_h',
                'pt_tt',
                'met',
                'n_jets', 'n_bjets',
                'pt_vis',
                'mjj','jdeta',
                'jpt_1','jpt_2',
                'dijetpt',
                'mt_sv','mt_tot',
                'dR',
                # just for dm splitting (HPS based)
                'tau_decay_mode_1',
                'tau_decay_mode_2',
                ]
        # if opt.analysis == 'cpsm':
        #     features = [
        #         'pt_1', 'pt_2', 'eta_1', 'eta_2',
        #         'mt_1', 'mt_2', 'mt_lep',
        #         'm_vis', 'm_sv',#'pt_tt',# 'eta_tt',
        #         'eta_h','pt_tt',
        #         'met','met_dphi_1', 'met_dphi_2',
        #         'n_jets', 'n_bjets',
        #         'pt_vis',
        #         'mjj','jdeta',
        #         'jpt_1','jpt_2',
        #         'dijetpt',
        #         'jeta_1','jeta_2',
        #         'mt_sv','mt_tot',
        #         'dR',
        #         #'IC_binary_test_4_score','IC_binary_test_4_index'
        #         ]
            # if opt.channel == "tt":
            #     features = [
            #         'pt_1', 'pt_2',
            #         'mt_1', 'mt_lep',
            #         'm_vis', 'm_sv',#'pt_tt',# 'eta_tt',
            #         'eta_h','pt_h',
            #         'met',# 'met_dphi_1', 'met_dphi_2',
            #         'n_jets', 'n_bjets',
            #         'pt_vis',
            #         'mjj','jdeta',
            #         'jpt_1','jpt_2',
            #         'dijetpt',
            #         'jeta_1','jeta_2',
            #         'opp_sides',
            #         'mt_sv','mt_tot',
            #         #'IC_binary_test_4_score','IC_binary_test_4_index'
            #         ]
        if opt.channel == 'em':
            features.extend(['pzeta','wt_em_qcd'])
        if opt.use_jet_variables:
            features.extend([
                'mjj','jdeta', # use jet variables now
                'jpt_1','jpt_2',
                'dijetpt',
                ])

        if opt.analysis == 'sm':
            if opt.channel == 'tt': # latest SM vars
                features = [
                    # 'm_sv',
                    'svfit_mass',
                    'pt_1',
                    #'pt_2',
                    'jpt_1',
                    'met',
                    'n_jets','pt_vis','pt_tt',
                    'mjj','jdeta', # add to dijet_features
                    'm_vis',
                    #'dijetpt',
                    # add dR variable to test
                    #'dR',
                ]

                dijet_features  = ['mjj','jdeta']
                singlejet_features = ['jpt_1']

            if opt.channel == 'mt':
                features = [
                        'm_sv','pt_1','pt_2',
                        'jpt_1',
                        'met',
                        'n_jets','pt_vis','pt_tt',
                        'mjj','jdeta','m_vis',
                        'dijetpt',
                        'mt_1',

                        # 'm_sv','pt_1','pt_2','eta_2',
                        # 'jpt_2','jphi_2','jm_1','jm_2',
                        # 'bpt_1','bpt_2','beta_1','beta_2',
                        # 'dijetpt','met','bphi_1','bphi_2',
                        # 'bcsv_1','bcsv_2','n_jets','n_bjets',
                        # 'mt_1','mt_2','pt_vis','pt_tt',
                        # 'mjj','m_vis',
                        ]
            if opt.channel == 'et':
                features = [
                        'm_sv','pt_1','pt_2','jpt_1',
                        'jcsv_2','jm_1','bpt_1','bpt_2',
                        'beta_1','beta_2','bphi_1','bphi_2',
                        'met','bcsv_1','bcsv_2','n_jets',
                        'n_bjets','mt_1','mt_2','pt_vis',
                        'pt_tt','mjj','m_vis','dijetpt',
                        ]
            if opt.channel == 'em':
                features = [
                        'm_sv','pt_1','pt_2','jpt_1',
                        'jcsv_2','jm_1','bpt_1','bpt_2',
                        'beta_1','beta_2','bphi_1','bphi_2',
                        'met','bcsv_1','bcsv_2','n_jets',
                        'n_bjets','mt_1','mt_2','pt_vis',
                        'pt_tt','mjj','m_vis','dijetpt',
                        'pzeta',
                        ]

#                     'pt_1', 'pt_2', 'eta_1', 'eta_2', 'dphi',
#                     'mt_1', 'mt_2', 'mt_lep',
#                     'm_vis', 'm_sv', 'pt_tt',
#                     'met', 'met_dphi_1', 'met_dphi_2',
#                     'n_jets', 'n_bjets',
#                     'pt_vis',
#                     'phi_1', 'phi_2',
#                     # 'wt', # for training/validation weights
#                     # 'gen_match_1', 'gen_match_2', # for splitting DY into separate processes
#                     # 'event',
#                     # add more features similar to KIT take tt for now
#                     'mjj', 'jdeta',
#                     'jpt_1', 'jeta_1', 'jphi_1',
#                     'jphi_2',
#                     'jdphi',
#                     ]
    else:
        features = [
                'pt_1', 'pt_2', 'eta_1', 'eta_2', 'dphi',
                'mt_1', 'mt_2', 'mt_lep',
                'm_vis', 'm_sv', 'pt_tt', 'eta_tt',
                'met', 'met_dphi_1', 'met_dphi_2',
                'n_jets', 'n_bjets',
                'pt_vis',
                # 'wt',
                ]

    # auxiliary features for preselection
    features.extend((
            'wt', # weights
            'wt_cp_sm', # cp weight
            'wt_cp_ps', # cp weight
            'wt_ph_nnlops', # nnlops weight
            'gen_match_1','gen_match_2', # split DY
            'event', # kfolding
            ))


    if opt.era in ["2016","2017","2018"]:
        class_dict = {}
        with open("{}_training_samples_{}.yaml".format(opt.era,opt.channel), "r") as f:
            try:
                full_class_dict = yaml.safe_load(f)
                for key, values in full_class_dict.iteritems():
                    print key
                    print values
                    class_dict[key] = values
            except yaml.YAMLError as exc:
                print exc

    # directory of the files (usually /vols/cms/)
    if opt.era == "2016":
        # path = '/vols/cms/akd116/Offline/output/SM/2018/Mar19'
        # path = '/vols/cms/akd116/Offline/output/SM/2018/Apr24_1'
        # path = '/vols/cms/akd116/Offline/output/SM/2018/May17_2'
        # path = '/vols/cms/akd116/Offline/output/SM/2018/Jun22_2016_Danny'
        # path = '/vols/cms/akd116/Offline/output/SM/2018/Aug14_2016_Danny_v3'
        # path = '/vols/cms/akd116/Offline/output/SM/2018/Feb12_2016'
        # path = '/vols/cms/dw515/Offline/output/SM/Dec05_2016/'
        # path = '/vols/cms/akd116/Offline/output/SM/2019/Feb26_2016/'
        # path = '/vols/cms/akd116/Offline/output/SM/2019/CPdecay_Apr26_2/'
        # path = '/vols/cms/akd116/Offline/output/SM/2019/Jun07_2016/'
        # path = '/vols/cms/dw515/Offline/output/SM/Jan24_2016_ttonly/'
        # path = '/vols/cms/dw515/Offline/output/SM/Mar27_2016/'
        # path = '/vols/cms/dw515/Offline/output/SM/May04_2016/'
        path = '/vols/cms/dw515/Offline/output/SM/May30_2016/'
    elif opt.era == "2017":
        # path = '/vols/cms/akd116/Offline/output/SM/2019/Nov21_2017_v2/'
        # path = '/vols/cms/dw515/Offline/output/SM/Jan24_2017_ttonly/'
        # path = '/vols/cms/dw515/Offline/output/SM/Mar27_2017/'
        # path = '/vols/cms/dw515/Offline/output/SM/May04_2017/'
        path = '/vols/cms/dw515/Offline/output/SM/May30_2017/'
    elif opt.era == "2018":
        # path = '/vols/cms/dw515/Offline/output/SM/Jan24_2018_ttonly/'
        # path = '/vols/cms/dw515/Offline/output/SM/Mar27_2018/'
        # path = '/vols/cms/dw515/Offline/output/SM/May04_2018/'
        path = '/vols/cms/dw515/Offline/output/SM/May30_2018/'

    ggh = []
    for sig in sig_files:
        print sig
        print('{}/{}_{}_{}.root'.format(path, sig, opt.channel, opt.era))
        sig_tmp = lf.load_mc_ntuple(
                '{}/{}_{}_{}.root'.format(path, sig, opt.channel, opt.era),
                'ntuple',
                features,
                opt.mjj_training,
                opt.channel,
                cut_features,
                apply_cuts=opt.apply_selection,
                split_by_sample=opt.split,
                signal=True,
                embedding=False,
                ff=opt.ff,
                dijet_feats=dijet_features,
                singlejet_feats=singlejet_features,
                )
        ## need to multiply event weight by
        ## (XS * Lumi) / #events
        if sig in [
                "GluGluToHToTauTauPlusTwoJets_M125_amcatnloFXFX",
                "GluGluToPseudoscalarHToTauTauPlusTwoJets_M125_amcatnloFXFX",
                "GluGluToMaxmixHToTauTauPlusTwoJets_M125_amcatnloFXFX",
                "GluGluToMaxmixHToTauTau_M125_amcatnloFXFX",
                "GluGluToHToTauTau_M125_amcatnloFXFX",
                "GluGluToPseudoscalarHToTauTau_M125_amcatnloFXFX",
                ]:
            xs_tmp = params_file[sig]['xs']
            events_tmp_1 = params_file["GluGluToHToTauTauPlusTwoJets_M125_amcatnloFXFX"]['evt']
            events_tmp_2 = params_file["GluGluToPseudoscalarHToTauTauPlusTwoJets_M125_amcatnloFXFX"]['evt']
            events_tmp_3 = params_file["GluGluToMaxmixHToTauTauPlusTwoJets_M125_amcatnloFXFX"]['evt']
            # if only training with H+2j samples use XS = 0.5
            # event numbers SM 4808923.0, PS = 4723705.0, MIX = 4788426.0
            if opt.mjj_training == "high":
                if sig in [
                        "GluGluToHToTauTauPlusTwoJets_M125_amcatnloFXFX",
                        "GluGluToPseudoscalarHToTauTauPlusTwoJets_M125_amcatnloFXFX",
                        "GluGluToMaxmixHToTauTauPlusTwoJets_M125_amcatnloFXFX",
                        ]:
                    sig_tmp['wt_xs'] = sig_tmp['wt'] * (0.5 * lumi)/((4723705.+4723705.+4788426.))

                # if sig == "GluGluToHToTauTauPlusTwoJets_M125_amcatnloFXFX":
                #     sig_tmp['wt_xs'] = sig_tmp['wt'] * (0.5 * lumi)/4808923. # when taking abs of neg wts this may not be the right evt number (factor 3 smaller)
                # elif sig == "GluGluToPseudoscalarHToTauTauPlusTwoJets_M125_amcatnloFXFX":
                #     sig_tmp['wt_xs'] = sig_tmp['wt'] * (0.5 * lumi)/4723705.
                # elif sig == "GluGluToMaxmixHToTauTauPlusTwoJets_M125_amcatnloFXFX":
                #     sig_tmp['wt_xs'] = sig_tmp['wt'] * (0.5 * lumi)/4788426.
            else:
                sig_tmp['wt_xs'] = sig_tmp['wt'] * (xs_tmp * lumi)/(events_tmp_1 + events_tmp_2 + events_tmp_3)
            # sig_tmp['wt_xs'] = scaler.fit_transform(sig_tmp['wt_xs'].values.reshape(-1,1))
            if opt.mjj_training == "high":
                if sig == "GluGluToHToTauTauPlusTwoJets_M125_amcatnloFXFX":
                    ggh_stitch_wt = (3089015. * 0.279662) / ((3089015. * 0.279662) + 14254055.)
                    # print ggh_stitch_wt
                    sig_tmp['wt_xs'] *= 1./ggh_stitch_wt
                elif sig == "GluGluToPseudoscalarHToTauTauPlusTwoJets_M125_amcatnloFXFX":
                    ggh_stitch_wt = (2982585. * 0.274486) / ((2982585. * 0.274486) + 14192951.)
                    # print ggh_stitch_wt
                    sig_tmp['wt_xs'] *= 1./ggh_stitch_wt
                elif sig == "GluGluToMaxmixHToTauTauPlusTwoJets_M125_amcatnloFXFX":
                    ggh_stitch_wt = (3100706. * 0.282423) / ((3100706. * 0.282423) + 14302986.)
                    # print ggh_stitch_wt
                    sig_tmp['wt_xs'] *= 1./ggh_stitch_wt
        elif sig in [
                "GluGluToHToTauTau_M-125-nospinner-filter",
                "VBFHToTauTau_M-125-nospinner-filter",
                "VBFHToTauTau_M-125-nospinner-filter-ext",
                "GluGluHToTauTauUncorrelatedDecay_Filtered",
                "VBFHToTauTauUncorrelatedDecay_Filtered",
                "WminusHToTauTauUncorrelatedDecay_Filtered",
                "WplusHToTauTauUncorrelatedDecay_Filtered",
                "ZHToTauTauUncorrelatedDecay_Filtered",
                ]:
            xs_tmp = params_file[sig]['xs']
            events_tmp = params_file[sig]['evt']
            filter_eff_tmp = params_file[sig]['eff']
            sig_tmp['wt_xs'] = sig_tmp["wt_cp_sm"] * \
                    sig_tmp['wt'] * (xs_tmp * lumi)/events_tmp * filter_eff_tmp
            if "GluGlu" in sig:
                print("Multiplying by ggH NNLOPS weight")
                print(sig_tmp["wt_ph_nnlops"])
                sig_tmp["wt_xs"] *= sig_tmp["wt_ph_nnlops"]
        else:
            xs_tmp = params_file[sig]['xs']
            events_tmp = params_file[sig]['evt']
            sig_tmp['wt_xs'] = sig_tmp['wt'] * (xs_tmp * lumi)/events_tmp

        sig_tmp['process'] = sig

        if opt.mode in ['keras_multi', 'xgb_multi']:
            for key, value in class_dict.iteritems():
                if sig in value:
                    print(key)
                    sig_tmp['multi_class'] = key
            # for key, value in class_weight_dict.iteritems():
            #     if sig_tmp['multi_class'].iloc[0] == key:
            #         sig_tmp['wt'] = value * sig_tmp['wt'] * (xs_tmp * lumi)/events_tmp
        # else:
            # sig_tmp['wt'] = sig_tmp['wt'] * (xs_tmp * lumi)/events_tmp

        ggh.append(sig_tmp)


    ggh = pd.concat(ggh, ignore_index=True)
    print ggh.shape


    bkgs_tmp = []
    for bkg in bkg_files:
        print bkg
        isSignal = False
        if bkg in ["VBFHToTauTau_M-125","WminusHToTauTau_M-125","WplusHToTauTau_M-125","ZHToTauTau_M-125"]:
            isSignal = True
        bkg_tmp = lf.load_mc_ntuple(
                '{}/{}_{}_{}.root'.format(path, bkg, opt.channel, opt.era),
                'ntuple',
                features,
                opt.mjj_training,
                opt.channel,
                cut_features,
                apply_cuts=opt.apply_selection,
                split_by_sample=opt.split,
                signal=isSignal,
                embedding=False,
                ff=opt.ff,
                dijet_feats=dijet_features,
                singlejet_feats=singlejet_features,
                )
        if opt.mode in ['keras_multi', 'xgb_multi'] and bkg in [
            'DYJetsToLL_M-10-50-LO','DY1JetsToLL-LO','DY2JetsToLL-LO',
            'DY3JetsToLL-LO','DY4JetsToLL-LO','DYJetsToLL-LO-ext1',
            'DYJetsToLL-LO-ext2',
            # 2017
            'DYJetsToLL-LO',
            'DYJetsToLL-LO-ext1',
            'DY1JetsToLL-LO',
            'DY1JetsToLL-LO-ext',
            'DY2JetsToLL-LO',
            'DY2JetsToLL-LO-ext',
            'DY3JetsToLL-LO',
            'DY3JetsToLL-LO-ext',
            'DY4JetsToLL-LO',
            'DYJetsToLL_M-10-50-LO',
            'DYJetsToLL_M-10-50-LO-ext1',
            ]:

            ztt_tmp = pd.DataFrame()
            zl_tmp = pd.DataFrame()
            zj_tmp = pd.DataFrame()
            zll_tmp = pd.DataFrame()

        if opt.channel in ['et','mt'] and opt.mode in ['keras_multi', 'xgb_multi'] and bkg in ['TT','TTTo2L2Nu','TTToHadronic','TTToSemiLeptonic']:

            ttt_tmp = pd.DataFrame()
            ttj_tmp = pd.DataFrame()

        if opt.channel in ['et','mt'] and opt.mode in ['keras_multi', 'xgb_multi'] and bkg in ['VVTo2L2Nu-ext1','VVTo2L2Nu','ZZTo2L2Nu','WWTo2L2Nu',]:

            vvt_tmp = pd.DataFrame()
            vvj_tmp = pd.DataFrame()

        ## need to multiply event weight by
        ## (XS * Lumi) / #events
        xs_tmp = params_file[bkg]['xs']
        events_tmp = params_file[bkg]['evt']
        if len(bkg_tmp) >= 1:
            bkg_tmp['process'] = bkg
            bkg_tmp['wt_xs'] = bkg_tmp['wt'] * (xs_tmp * lumi)/events_tmp

            if opt.mode in ['keras_multi', 'xgb_multi']:
                for key, value in class_dict.iteritems():
                    if bkg in value:
                        bkg_tmp['multi_class'] = key
                # for key, value in class_weight_dict.iteritems():
                #     if bkg_tmp['multi_class'].iloc[0] == key:
                #         bkg_tmp['wt'] = value * bkg_tmp['wt'] * (xs_tmp * lumi)/events_tmp
            # else:
            #     bkg_tmp['wt'] = bkg_tmp['wt'] * (xs_tmp * lumi)/events_tmp

            ## need to genmatch for splitting DY into ZTT and ZLL
            if opt.mode in ['keras_multi', 'xgb_multi'] and bkg in [
                'DYJetsToLL_M-10-50-LO','DY1JetsToLL-LO','DY2JetsToLL-LO',
                'DY3JetsToLL-LO','DY4JetsToLL-LO','DYJetsToLL-LO-ext1',
                'DYJetsToLL-LO-ext2',
                # 2017
                'DYJetsToLL-LO',
                'DYJetsToLL-LO-ext1',
                'DY1JetsToLL-LO',
                'DY1JetsToLL-LO-ext',
                'DY2JetsToLL-LO',
                'DY2JetsToLL-LO-ext',
                'DY3JetsToLL-LO',
                'DY3JetsToLL-LO-ext',
                'DY4JetsToLL-LO',
                'DYJetsToLL_M-10-50-LO',
                'DYJetsToLL_M-10-50-LO-ext1',
                ]:

                if opt.channel == 'tt':
                    ztt_tmp = bkg_tmp[(bkg_tmp['gen_match_1'] == 5) & (bkg_tmp['gen_match_2'] == 5)].copy(deep=True)
                    ztt_tmp.reset_index(drop=True)
                    ztt_tmp['multi_class'] = 'ztt'

                    zl_tmp = bkg_tmp[
                        ((bkg_tmp['gen_match_1'] != 6) & (bkg_tmp['gen_match_2'] != 6))
                        & ((bkg_tmp['gen_match_1'] != 5) & (bkg_tmp['gen_match_2'] != 5))
                        ]
                    zl_tmp.reset_index(drop=True)
                    zl_tmp['multi_class'] = 'misc' ## zl -- > misc for tt training

                    if not opt.ff:
                        zj_tmp = bkg_tmp[(bkg_tmp['gen_match_1'] == 6) | (bkg_tmp['gen_match_2'] == 6)]
                        zj_tmp.reset_index(drop=True)
                        zj_tmp['multi_class'] = 'misc' ## zj --> misc

                if opt.channel in ['mt', 'et']:
                    ztt_tmp = bkg_tmp[(bkg_tmp['gen_match_2'] == 5)] # and gen_match_1==4 for mt, gen_match_1==3 for et
                    ztt_tmp.reset_index(drop=True)
                    ztt_tmp['multi_class'] = 'ztt'

                    zl_tmp = bkg_tmp[(bkg_tmp['gen_match_2'] != 6) & (bkg_tmp['gen_match_2'] != 5)]
                    zl_tmp.reset_index(drop=True)
                    if opt.mjj_training == 'low':
                        zl_tmp['multi_class'] = 'zll' ## zl --> zll
                    else:
                        zl_tmp['multi_class'] = 'misc' ## zl --> misc

                    if not opt.ff:
                        zj_tmp = bkg_tmp[(bkg_tmp['gen_match_2'] == 6)]
                        zj_tmp.reset_index(drop=True)
                        zj_tmp['multi_class'] = 'fake' ## zj --> fake


                if opt.channel == 'em':
                    ztt_tmp = bkg_tmp[(bkg_tmp['gen_match_1'] > 2) & (bkg_tmp['gen_match_2'] > 3)]
                    ztt_tmp.reset_index(drop=True)
                    ztt_tmp['multi_class'] = 'ztt'


                    zll_tmp = bkg_tmp[(bkg_tmp['gen_match_1'] < 3) | (bkg_tmp['gen_match_2'] < 4)]
                    zll_tmp.reset_index(drop=True)
                    zll_tmp['multi_class'] = 'misc'


                if not opt.ff:
                    for zsplit in [ztt_tmp, zl_tmp, zj_tmp, zll_tmp]:
                        bkgs_tmp.append(zsplit)
                else:
                    for zsplit in [ztt_tmp, zl_tmp, zll_tmp]:
                        bkgs_tmp.append(zsplit)

            elif opt.channel in ['et','mt'] and opt.mode in ['keras_multi', 'xgb_multi'] and bkg in ['TT','TTTo2L2Nu','TTToHadronic','TTToSemiLeptonic',]:
                ttt_tmp = bkg_tmp[(bkg_tmp['gen_match_2'] < 6)]
                ttt_tmp.reset_index(drop=True)
                ttt_tmp['multi_class'] = 'tt'

                if not opt.ff:
                    ttj_tmp = bkg_tmp[(bkg_tmp['gen_match_2'] != 5)]
                    ttj_tmp.reset_index(drop=True)
                    ttj_tmp['multi_class'] = 'fake'

                    for fake_split in [ttt_tmp, ttj_tmp,]:
                        bkgs_tmp.append(fake_split)
                else:
                    for fake_split in [ttt_tmp]:
                        bkgs_tmp.append(fake_split)


            elif opt.channel in ['et','mt'] and opt.mode in ['keras_multi', 'xgb_multi'] and bkg in ['VVTo2L2Nu-ext1','VVTo2L2Nu','ZZTo2L2Nu','WWTo2L2Nu',]:
                vvt_tmp = bkg_tmp[(bkg_tmp['gen_match_2'] < 6)]
                vvt_tmp.reset_index(drop=True)
                vvt_tmp['multi_class'] = 'misc'

                if not opt.ff:
                    vvj_tmp = bkg_tmp[(bkg_tmp['gen_match_2'] != 5)]
                    vvj_tmp.reset_index(drop=True)
                    vvj_tmp['multi_class'] = 'fake'

                    for fake_split in [vvt_tmp, vvj_tmp]:
                        bkgs_tmp.append(fake_split)
                else:
                    for fake_split in [vvt_tmp]:
                        bkgs_tmp.append(fake_split)

            else:
                bkgs_tmp.append(bkg_tmp)


    bkgs = pd.concat(bkgs_tmp, ignore_index=True)

    print bkgs.shape
    
    # add embedding stuff
    if opt.embedding:
        embeds_tmp = []
        for embed in embed_files:
            print embed
            embed_tmp = lf.load_mc_ntuple(
                    '{}/{}_{}_{}.root'.format(path, embed, opt.channel, opt.era),
                    'ntuple',
                    features,
                    opt.mjj_training,
                    opt.channel,
                    cut_features,
                    apply_cuts=opt.apply_selection,
                    split_by_sample=opt.split,
                    signal=False,
                    embedding=opt.embedding,
                    ff=opt.ff,
                    dijet_feats=dijet_features,
                    singlejet_feats=singlejet_features,
                    )

            ztt_embed_tmp = pd.DataFrame()

            embed_tmp['process'] = embed
            embed_tmp['wt_xs'] = embed_tmp['wt']
            embed_tmp = embed_tmp[(embed_tmp["wt_xs"] < 2)]

            if opt.mode in ['keras_multi', 'xgb_multi']:
                for key, value in class_dict.iteritems():
                    if embed in value:
                        embed_tmp['multi_class'] = key

            if opt.channel == 'tt':
                ztt_embed_tmp = embed_tmp[(embed_tmp['gen_match_1'] == 5) & (embed_tmp['gen_match_2'] == 5)]
                ztt_embed_tmp.reset_index(drop=True)
                ztt_embed_tmp['multi_class'] = 'ztt_embed'

            if opt.channel in ['mt', 'et']:
                ztt_embed_tmp = embed_tmp[(embed_tmp['gen_match_2'] == 5)] # and gen_match_1==4 for mt, gen_match_1==3 for et
                ztt_embed_tmp.reset_index(drop=True)
                ztt_embed_tmp['multi_class'] = 'ztt_embed'

            if opt.channel == 'em':
                ztt_embed_tmp = embed_tmp[(embed_tmp['gen_match_1'] > 2) & (embed_tmp['gen_match_2'] > 3)]
                ztt_embed_tmp.reset_index(drop=True)
                ztt_embed_tmp['multi_class'] = 'ztt_embed'

            embeds_tmp.append(ztt_embed_tmp)

        embeds = pd.concat(embeds_tmp, ignore_index=True)

        print embeds.shape
        ### 

    ## add FF stuff
    if opt.ff:
        ff_tmp = []
        for data in data_files:
            print data
            data_tmp = lf.load_ff_ntuple(
                    '{}/{}_{}_{}.root'.format(path, data, opt.channel, opt.era),
                    'ntuple',
                    features,
                    opt.mjj_training,
                    opt.channel,
                    cut_features,
                    apply_cuts=opt.apply_selection,
                    split_by_sample=opt.split,
                    dijet_feats=dijet_features,
                    singlejet_feats=singlejet_features,
                    )

            data_tmp['process'] = data
            data_tmp['wt_xs'] = data_tmp['wt']
            data_tmp['multi_class'] = "jetFakes"
            ff_tmp.append(data_tmp)
        ff = pd.concat(ff_tmp, ignore_index=True)

        print ff.shape
        ###

    if not opt.ff:
        qcd_tmp = []
        for data in data_files:
            print data
            data_tmp = lf.load_data_ntuple(
                    '{}/{}_{}_{}.root'.format(path, data, opt.channel, opt.era),
                    'ntuple',
                    features,
                    opt.mjj_training,
                    opt.channel,
                    cut_features,
                    apply_cuts=opt.apply_selection,
                    split_by_sample=opt.split
                    )

            data_tmp['process'] = data
            data_tmp['wt_xs'] = data_tmp['wt']
            if opt.channel == "em":
                data_tmp['wt_xs'] *= data_tmp["wt_em_qcd"]
            if opt.mode in ['keras_multi', 'xgb_multi']:
                for key, value in class_dict.iteritems():
                    if data in value:
                        data_tmp['multi_class'] = key
                # for key, value in class_weight_dict.iteritems():
                #     if data_tmp['multi_class'].iloc[0] == key:
                #         data_tmp['wt'] = value * data_tmp['wt']
            qcd_tmp.append(data_tmp)
        qcd = pd.concat(qcd_tmp, ignore_index=True)


    # full background DataFrame
    if not opt.embedding and not opt.ff:
        bkgs = pd.concat([bkgs, qcd], ignore_index=True)
    # with embedding
    if opt.embedding and not opt.ff:
        bkgs = pd.concat([bkgs, embeds, qcd], ignore_index=True)
        bkgs = bkgs[bkgs["multi_class"] != "ztt"]
    # with embedding and FF, still need to get rid of W process things
    if opt.embedding and opt.ff:
        bkgs = pd.concat([bkgs, embeds, ff], ignore_index=True)
        bkgs = bkgs[~bkgs["process"].str.contains("JetsToLNu")]
        bkgs = bkgs[bkgs["multi_class"] != "ztt"]
        if opt.mjj_training == "high" and opt.channel == "tt":
            bkgs = bkgs[bkgs["multi_class"] != "misc"]

    print bkgs.shape



    # pf.plot_roc_cutbased(ggh, bkgs, 'm_sv', opt.channel, opt.sig_sample)

    # pf.plot_correlation_matrix(
    #         bkgs.drop(['wt'], axis=1),
    #         'bkgs_{}_{}_correlation_matrix.pdf'.format(opt.channel, opt.sig_sample))



    ## for ggh vs rest discrimation
    if opt.mode not in ['keras_multi','xgb_multi']:
        y_sig = pd.DataFrame(np.ones(ggh.shape[0]))
        y_bkgs = pd.DataFrame(np.zeros(bkgs.shape[0]))
        y = pd.concat([y_sig, y_bkgs])
        y.columns = ['class']


    X = pd.concat([ggh, bkgs])
    print X.wt_xs
    print X.wt_xs.values.reshape(-1,1)
    # X['wt_xs'] = scaler.fit_transform(X['wt_xs'].values.reshape(-1,1))
    # print X.wt_xs
    # print X
    # X['deta'] = np.abs(X['eta_1'] - X['eta_2'])
    # X['class'] = y.values

    # # make zeppenfeld variable
    # X["zfeld"] = np.fabs(X.eta_h - (X.jeta_1 + X.jeta_2)/2.)
    # # print X["zfeld"]
    # # make centrality variable
    # X["centrality"] = np.exp(-4*(X.zfeld/np.fabs(X.jdeta))**2)
    # # make new variable combinatinos
    # X["mjj_jdeta"] = X.mjj * X.jdeta
    # X["dijetpt_pth"] = X.dijetpt * X.pt_h
    # X["dijetpt_jpt1"] = X.dijetpt * X.jpt_1
    # # X["exp_dijetpt_jpt1"] = np.exp(-30000*(X.dijetpt/X.jpt_1))


    # pf.plot_signal_background(
    #         X[X.multi_class == "ggh"], X[X.multi_class == "qqh"], 'dijetpt_pth',
    #         opt.channel, opt.sig_sample,
    #         bins=100
    #         )
    # pf.plot_signal_background(
    #         X[X.multi_class == "ggh"], X[X.multi_class == "qqh"], 'exp_dijetpt_jpt1',
    #         opt.channel, opt.sig_sample,
    #         bins=100
    #         )
    # pf.plot_signal_background(
    #         X[X.multi_class == "ggh"], X[X.multi_class != "ggh"], 'centrality',
    #         opt.channel, opt.sig_sample,
    #         bins=100
    #         )

    scaler = StandardScaler()
    if opt.channel != "em":
        scaled_fit = scaler.fit(X.drop([
            'wt','wt_xs', 'process', 'multi_class','event',
            'gen_match_1', 'gen_match_2',
            ], axis=1).as_matrix())
    else:
        scaled_fit = scaler.fit(X.drop([
            'wt','wt_xs', 'process', 'multi_class','event',
            'gen_match_1', 'gen_match_2','wt_em_qcd',
            ], axis=1).as_matrix())
    with open('{}_{}_scaler.pkl'.format(opt.channel,opt.mjj_training), 'w') as f:
        pickle.dump(scaled_fit, f)


    # pf.plot_correlation_matrix(X, 'correlation_matrix.pdf')


    # randomise the order of events
    # print X[(X.multi_class == 'ggh')]

    X = X.sample(
            frac=1,
            random_state=123456,
            ).reset_index(drop=True)

    # print X[(X.multi_class == 'ggh')]


    ## divide datasets
    if opt.kfold:

        # get even event numbers
        X_fold0 = X[(X['event'] % 2 == 0)]#.drop(['event'], axis=1)

        # get odd event numbers
        X_fold1 = X[(X['event'] % 2 == 1)]#.drop(['event'], axis=1)

        # X_fold1.to_hdf('data_tauspinner_12Mar2020_{}_NopT2dijetpT/dataset_fold1_{}_{}_{}.hdf5' # odd event numbers
        # X_fold1.to_hdf('data_tauspinner_08Apr2020_{}/dataset_fold1_{}_{}_{}.hdf5' # odd event numbers
        # X_fold1.to_hdf('data_tauspinner_11May2020_{}/dataset_fold1_{}_{}_{}.hdf5' # odd event numbers
        # X_fold1.to_hdf('data_tauspinner_PUJIDCheck_{}/dataset_fold1_{}_{}_{}.hdf5' # odd event numbers
        X_fold1.to_hdf('data_tauspinner_01Jun2020_{}/dataset_fold1_{}_{}_{}.hdf5' # odd event numbers
            .format(opt.era, opt.analysis, opt.channel, opt.era),
            key='X_fold1',
            mode='w')
        # X_fold0.to_hdf('data_tauspinner_12Mar2020_{}_NopT2dijetpT/dataset_fold0_{}_{}_{}.hdf5' # even event numbers
        # X_fold0.to_hdf('data_tauspinner_08Apr2020_{}/dataset_fold0_{}_{}_{}.hdf5' # even event numbers
        # X_fold0.to_hdf('data_tauspinner_11May2020_{}/dataset_fold0_{}_{}_{}.hdf5' # even event numbers
        # X_fold0.to_hdf('data_tauspinner_PUJIDCheck_{}/dataset_fold0_{}_{}_{}.hdf5' # even event numbers
        X_fold0.to_hdf('data_tauspinner_01Jun2020_{}/dataset_fold0_{}_{}_{}.hdf5' # even event numbers
            .format(opt.era, opt.analysis, opt.channel, opt.era),
            key='X_fold0',
            mode='w')

    else:
        if opt.apply_selection:
            X.to_hdf('data/dataset_{}_{}_{}.hdf5'
                .format(opt.analysis, opt.channel, opt.sig_sample),
                key='X',
                mode='w')
        else:
            X.to_hdf('data/dataset_full_{}_{}.hdf5'
                .format(opt.analysis, opt.channel),
                key='X',
                mode='w')

if __name__ == "__main__":
    opt = parse_arguments()
    main(opt)
