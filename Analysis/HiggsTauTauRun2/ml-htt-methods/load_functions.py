import uproot
import os
import pandas as pd
import numpy as np

def load_mc_ntuple(
    data, tree, branch, mjj_training, channel, cut_feats, apply_cuts,
    split_by_sample, signal, embedding, ff, dijet_feats, singlejet_feats,
):

    try:
        iterator = uproot.iterate(data, tree, branches=branch+cut_feats)
    except IOError:
        print 'Tree/Branches not found'

    df = []
    try:
        for block in iterator:
            df_b = pd.DataFrame(block)

            if apply_cuts:
                if channel == 'tt':
                    df_b = df_b.query(
                            "pt_1 > 40 "
                            "and deepTauVsJets_medium_1  > 0.5 "
                            "and deepTauVsJets_medium_2  > 0.5 "
                            "and deepTauVsEle_vvvloose_1 > 0.5 "
                            "and deepTauVsMu_vloose_1    > 0.5 "
                            "and deepTauVsEle_vvvloose_2 > 0.5 "
                            "and deepTauVsMu_vloose_2    > 0.5 "
                            "and leptonveto == False "
                            "and trg_doubletau == True "
                            "and mva_dm_1 != -1 "
                            "and mva_dm_2 != -1"
                            )

                elif channel == 'mt':
                    df_b = df_b[
                            (df_b['iso_1'] < 0.15)
                            & (df_b['mt_1'] < 50)
                            & (df_b['deepTauVsJets_medium_2'] > 0.5)
                            & (df_b['deepTauVsEle_vvvloose_2'] > 0.5 )
                            & (df_b['deepTauVsMu_tight_2']     > 0.5 )
                            & (df_b['leptonveto'] == False)
                            & (df_b['pt_2'] > 20)
                            & (df_b['n_bjets'] == 0) # bjet veto to remove ttbar
                            # & ((df_b['trg_singlemuon']*df_b['pt_1'] > 23)
                            #     | (df_b['trg_mutaucross']*df_b['pt_1'] < 23))
                            & ((df_b['trg_singlemuon']*df_b['pt_1'] > 25)
                                | (df_b['trg_mutaucross']*df_b['pt_2'] > 29))
                            ]

                elif channel == 'et':
                    df_b = df_b[
                            (df_b['iso_1'] < 0.1)
                            & (df_b['mt_1'] < 40)
                            & (df_b['mva_olddm_tight_2'] > 0.5)
                            & (df_b['antiele_2'] == True)
                            & (df_b['antimu_2'] == True)
                            & (df_b['leptonveto'] == False)
                            & (df_b['pt_2'] > 20)
                            & (df_b['trg_singleelectron'] == True)
                            ]

                elif channel == 'em':
                    df_b = df_b[
                            (df_b['iso_1'] < 0.15)
                            & (df_b['iso_2'] < 0.2)
                            & (df_b['pzeta'] > -50)
                            & (df_b['leptonveto'] == False)
                            & (df_b['trg_muonelectron'] == True)
                            ]
                else:
                    assert ValueError('Channel not in ["tt", "mt", "et", "em"]')

                # if not embedding and not signal:
                #     if channel == 'tt': 
                #         df_b = df_b[
                #                 ~((df_b['gen_match_1'] == 5) 
                #                     & (df_b['gen_match_2'] == 5))]

                #     elif channel == 'mt': 
                #         df_b = df_b[
                #                 ~((df_b['gen_match_1'] == 4) 
                #                     & (df_b['gen_match_2'] == 5))]
                        
                #     elif channel == 'et': 
                #         df_b = df_b[
                #                 ~((df_b['gen_match_1'] == 3) 
                #                     & (df_b['gen_match_2'] == 5))]

                #     elif channel == 'em': 
                #         df_b = df_b[
                #                 ~((df_b['gen_match_1'] == 3) 
                #                     & (df_b['gen_match_2'] == 4))]

                if ff and not signal:
                    if channel == 'tt': 
                        df_b = df_b[
                                ~((df_b['gen_match_1'] == 6)
                                    | (df_b['gen_match_2'] == 6))]

                    elif channel in ['et','mt']:
                        df_b = df_b[df_b['gen_match_2'] < 6]

                ## TO SELECT THE SIGNAL SAMPLE ACCORDING TO
                ## CUTS APPLIED RELATING TO n_jets AND mjj
                if not split_by_sample:
                    df_b = df_b
                else:
                    if mjj_training == 'low':
                            df_b = df_b[
                                    (df_b['n_jets'] < 2)
                                    | ((df_b['n_jets'] >= 2)
                                        & (df_b['mjj'] < 300))
                                    ]
                    elif mjj_training == 'high':
                            df_b = df_b[
                                    ((df_b['n_jets'] >= 2)
                                    & (df_b['mjj'] > 300))
                                    ]
                    else:
                        assert ValueError('Mjj training not in ["low", "high"]')

                df_b = df_b[(df_b['svfit_mass'] > 0)] ## SOME svfit_mass ARE MISSING

                # only define dijet features in case when there are two jets
                # with pT > 30 GeV, otherwise taking pT > 20 GeV jets as well
                for dijet_feature in dijet_feats:
                    df_b.loc[df_b["n_jets"] < 2, dijet_feature] = -9999
                for singlejet_feature in singlejet_feats:
                    print(singlejet_feature)
                    print(df_b.loc[df_b["n_jets"] < 1, singlejet_feature])
                    df_b.loc[df_b["n_jets"] < 1, singlejet_feature] = -9999
                print(df_b.loc[df_b["n_jets"] < 1, singlejet_feature])

            df_b = df_b.drop(cut_feats, axis=1)
            df.append(df_b)

        df = pd.concat(df, ignore_index=True)
    except IndexError:
        print 'zero events in ntuple'

    return df

## NEED TO CHANGE THE CUTS HERE BUT ONLY IF USING QCD FROM SS DATA
## FOR FF WE USE FUNCTION BELOW
def load_data_ntuple(data, tree, branch, mjj_training, channel, cut_feats, apply_cuts, split_by_sample):
    ## THIS FUNCTION IS FOR READING IN SAME SIGN DATA (FOR mt, et, em CHANNELS)
    ## OR ANTIISOLATED (FOR tt CHANNEL) FOR THE QCD ESTIMATION

    try:
        iterator = uproot.iterate(data, tree, branches=branch+cut_feats)
    except IOError:
        print 'Tree/Branches not found'

    df = []
    for block in iterator:
        df_b = pd.DataFrame(block)

        if apply_cuts:
            if channel == 'tt':
                df_b = df_b[
                        (df_b['pt_1'] > 40)
                        & ((df_b['mva_olddm_loose_1'] > 0.5)
                            & (df_b['mva_olddm_tight_1'] < 0.5)
                            & (df_b['mva_olddm_medium_2'] > 0.5))
                        | ((df_b['mva_olddm_loose_2'] > 0.5)
                            & (df_b['mva_olddm_tight_2'] < 0.5)
                            & (df_b['mva_olddm_medium_1'] > 0.5))
                        & (df_b['antiele_1'] == True)
                        & (df_b['antimu_1'] == True)
                        & (df_b['antiele_2'] == True)
                        & (df_b['antimu_2'] == True)
                        & (df_b['leptonveto'] == False)
                        & (df_b['trg_doubletau'] == True)
                        ]

            elif channel == 'mt':
                df_b = df_b[
                        (df_b['iso_1'] < 0.15)
                        & (df_b['mt_1'] < 40)
                        & (df_b['mva_olddm_tight_2'] > 0.5)
                        & (df_b['antiele_2'] == True)
                        & (df_b['antimu_2'] == True)
                        & (df_b['leptonveto'] == False)
                        & (df_b['pt_2'] > 20)
                        & ((df_b['trg_singlemuon']*df_b['pt_1'] > 23)
                            | (df_b['trg_mutaucross']*df_b['pt_1'] < 23))
                        & (df_b['os'] == False)
                        ]

            elif channel == 'et':
                df_b = df_b[
                        (df_b['iso_1'] < 0.1)
                        & (df_b['mt_1'] < 40)
                        & (df_b['mva_olddm_tight_2'] > 0.5)
                        & (df_b['antiele_2'] == True)
                        & (df_b['antimu_2'] == True)
                        & (df_b['leptonveto'] == False)
                        & (df_b['pt_2'] > 20)
                        & (df_b['trg_singleelectron'] == True)
                        & (df_b['os'] == False)
                        ]

            elif channel == 'em':
                df_b = df_b[
                        (df_b['iso_1'] < 0.15)
                        & (df_b['iso_2'] < 0.2)
                        & (df_b['pzeta'] > -50)
                        & (df_b['leptonveto'] == False)
                        & (df_b['trg_muonelectron'] == True)
                        & (df_b['os'] == False)
                        ]
            else:
                assert ValueError('Channel not in ["tt", "mt", "et", "em"]')

            ## TO SELECT THE SIGNAL SAMPLE ACCORDING TO
            ## CUTS APPLIED RELATING TO n_jets AND mjj
            if not split_by_sample:
                df_b = df_b
            else:
                if mjj_training == 'low':
                        df_b = df_b[
                                (df_b['n_jets'] < 2)
                                | ((df_b['n_jets'] >= 2)
                                    & (df_b['mjj'] < 300))
                                ]
                elif mjj_training == 'high':
                        df_b = df_b[
                                ((df_b['n_jets'] >= 2)
                                & (df_b['mjj'] > 300))
                                ]
                else:
                    assert ValueError('Mjj training not in ["low", "high"]')

            df_b = df_b[(df_b['svfit_mass'] > 0)] ## SOME svfit_mass ARE MISSING

        df_b = df_b.drop(cut_feats, axis=1)
        df.append(df_b)


    df = pd.concat(df, ignore_index=True)

    return df

def load_ff_ntuple(
    data, tree, branch, mjj_training, channel, cut_feats, 
    apply_cuts, split_by_sample, dijet_feats, singlejet_feats,
):

    try:
        iterator = uproot.iterate(data, tree, branches=branch+cut_feats)
    except IOError:
        print 'Tree/Branches not found'

    df = []
    for block in iterator:
        df_b = pd.DataFrame(block)

        if apply_cuts:
            if channel == 'tt':
                df_b_tt = []
                # df_b_1 = df_b[
                #         (df_b['pt_1'] > 40)
                #         & ((df_b['mva_olddm_tight_1'] < 0.5)
                #             & (df_b['mva_olddm_vloose_1'] > 0.5)
                #             & (df_b['mva_olddm_tight_2'] > 0.5))
                #         & (df_b['antiele_1'] == True)
                #         & (df_b['antimu_1'] == True)
                #         & (df_b['antiele_2'] == True)
                #         & (df_b['antimu_2'] == True)
                #         & (df_b['leptonveto'] == False)
                #         & (df_b['trg_doubletau'] == True)
                #         ]
                # df_b_1["wt"] = df_b_1["wt_ff_1"]
                # df_b_tt.append(df_b_1)
                # df_b_2 = df_b[
                #         (df_b['pt_1'] > 40)
                #         & ((df_b['mva_olddm_tight_2'] < 0.5)
                #             & (df_b['mva_olddm_vloose_2'] > 0.5)
                #             & (df_b['mva_olddm_tight_1'] > 0.5))
                #         & (df_b['antiele_1'] == True)
                #         & (df_b['antimu_1'] == True)
                #         & (df_b['antiele_2'] == True)
                #         & (df_b['antimu_2'] == True)
                #         & (df_b['leptonveto'] == False)
                #         & (df_b['trg_doubletau'] == True)
                #         ]
                # df_b_2["wt"] = df_b_2["wt_ff_2"]
                # df_b_tt.append(df_b_2)

                # new ffs with deeptau
                df_b = df_b.query(
                        "pt_1 > 40 "
                        "and deepTauVsJets_medium_1 < 0.5 "
                        "and deepTauVsJets_vvvloose_1 > 0.5 "
                        "and deepTauVsJets_medium_2 > 0.5 "
                        "and deepTauVsEle_vvvloose_1 > 0.5 "
                        "and deepTauVsMu_vloose_1    > 0.5 "
                        "and deepTauVsEle_vvvloose_2 > 0.5 "
                        "and deepTauVsMu_vloose_2    > 0.5 "
                        "and leptonveto == False "
                        "and trg_doubletau == True "
                        "and mva_dm_1 != -1 "
                        "and mva_dm_2 != -1"
                        )

                df_b["wt"] = df_b["wt_ff_1"]

            elif channel == 'mt':
                df_b = df_b[
                        (df_b['iso_1'] < 0.15)
                        & (df_b['mt_1'] < 50)
                        & (df_b['deepTauVsJets_medium_2'] > 0.5)
                        & (df_b['deepTauVsEle_vvvloose_2'] > 0.5 )
                        & (df_b['deepTauVsMu_tight_2']     > 0.5 )
                        & (df_b['leptonveto'] == False)
                        & (df_b['pt_2'] > 20)
                        # & ((df_b['trg_singlemuon']*df_b['pt_1'] > 23)
                        #     | (df_b['trg_mutaucross']*df_b['pt_1'] < 23))
                        & ((df_b['trg_singlemuon']*df_b['pt_1'] > 25)
                            | (df_b['trg_mutaucross']*df_b['pt_2'] > 29))
                        & (df_b['n_bjets'] == 0) # bjet veto to remove ttbar
                        ]
                df_b["wt"] = df_b["wt_ff_1"]

            elif channel == 'et':
                df_b = df_b[
                        (df_b['iso_1'] < 0.1)
                        & (df_b['mt_1'] < 40)
                        & (df_b['mva_olddm_tight_2'] < 0.5)
                        & (df_b['mva_olddm_vloose_2'] > 0.5)
                        & (df_b['antiele_2'] == True)
                        & (df_b['antimu_2'] == True)
                        & (df_b['leptonveto'] == False)
                        & (df_b['pt_2'] > 20)
                        & (df_b['trg_singleelectron'] == True)
                        ]
                df_b["wt"] = df_b["wt_ff_1"]

            else:
                assert ValueError('Channel not in ["tt", "mt", "et"]')

            ## TO SELECT THE SIGNAL SAMPLE ACCORDING TO
            ## CUTS APPLIED RELATING TO n_jets AND mjj
            if not split_by_sample:
                df_b = df_b
            else:
                if mjj_training == 'low':
                        df_b = df_b[
                                (df_b['n_jets'] < 2)
                                | ((df_b['n_jets'] >= 2)
                                    & (df_b['mjj'] < 300))
                                ]
                elif mjj_training == 'high':
                        df_b = df_b[
                                ((df_b['n_jets'] >= 2)
                                & (df_b['mjj'] > 300))
                                ]
                else:
                    assert ValueError('Mjj training not in ["low", "high"]')

            df_b = df_b[(df_b['svfit_mass'] > 0)] ## SOME svfit_mass ARE MISSING

            # only define dijet features in case when there are two jets
            # with pT > 30 GeV, otherwise taking pT > 20 GeV jets as well
            for dijet_feature in dijet_feats:
                df_b.loc[df_b["n_jets"] < 2, dijet_feature] = -9999
            for singlejet_feature in singlejet_feats:
                df_b.loc[df_b["n_jets"] < 1, singlejet_feature] = -9999

        df_b = df_b.drop(cut_feats, axis=1)
        df.append(df_b)


    df = pd.concat(df, ignore_index=True)

    return df

def load_rhoID_ntuple(data, tree, branch, channel, cut_feats, apply_cuts):
    # LOAD MC NTUPLES AND APPLY BASELINE CUTS BY CHANNEL
    # need to do something for when df too large..


    try:
        iterator = uproot.iterate(data, tree, branches=branch+cut_feats)
    except IOError:
        print 'Tree/Branches not found'

    df1 = []
    df2 = []
    try:
        for block in iterator:
            df_b1 = pd.DataFrame(block)
            df_b2 = pd.DataFrame(block)

            if apply_cuts:
                if channel == 'tt':
                    df_b1 = df_b1[
                            (df_b1['pt_1'] > 40)
                            & (df_b1['mva_olddm_tight_1'] > 0.5)
                            & (df_b1['antiele_1'] == True)
                            & (df_b1['antimu_1'] == True)
                            & (df_b1['leptonveto'] == False)
                            & (df_b1['trg_doubletau'] == True)
                            & (df_b1['tau_decay_mode_1'] == 1)
                            ]
                    df_b2 = df_b2[
                            (df_b2['pt_1'] > 40)
                            & (df_b2['mva_olddm_tight_2'] > 0.5)
                            & (df_b2['antiele_2'] == True)
                            & (df_b2['antimu_2'] == True)
                            & (df_b2['leptonveto'] == False)
                            & (df_b2['trg_doubletau'] == True)
                            & (df_b2['tau_decay_mode_2'] == 1)
                            ]

                else:
                    assert ValueError('Channel not "tt"')

            df_b1 = df_b1.drop(cut_feats, axis=1)
            df1.append(df_b1)
            df_b2 = df_b2.drop(cut_feats, axis=1)
            df2.append(df_b2)

    except IndexError:
        print 'zero events in ntuple'

    df1 = pd.concat(df1, ignore_index=True)
    df2 = pd.concat(df2, ignore_index=True)
    return df1, df2

def load_noisejets_ntuple(data, tree, branch, channel, cut_feats, apply_cuts):
    # LOAD MC NTUPLES AND APPLY BASELINE CUTS BY CHANNEL
    # need to do something for when df too large..


    try:
        iterator = uproot.iterate(data, tree, branches=branch+cut_feats)
    except IOError:
        print 'Tree/Branches not found'

    df1 = []
    try:
        for block in iterator:
            df_b1 = pd.DataFrame(block)
            df_b2 = pd.DataFrame(block)

            if apply_cuts:
                if channel == 'zmm': # only train on zmm
                    df_b1 = df_b1[
                            (df_b1['pt_1'] > 25)
                            & (df_b1['iso_1'] < 0.15)
                            & (df_b1['iso_2'] < 0.15)
                            & (df_b1['trg_singlemuon'] == True)
                            & ((df_b1["jpt_1"]/df_b1["pt_vis"]) > 0.5)
                            & ((df_b1["jpt_1"]/df_b1["pt_vis"]) < 1.5)
                            & ((df_b1["jpt_1"]) < 100)
                            & (np.abs(df_b1["jeta_1"]) > 2.65)
                            & (np.abs(df_b1["jeta_1"]) < 3.139)
                            # & (df_b1["n_jets"] == 1)
                            ]

                else:
                    assert ValueError('Channel not "zmm"')

            df_b1 = df_b1.drop(cut_feats, axis=1)
            df1.append(df_b1)

    except IndexError:
        print 'zero events in ntuple'

    df1 = pd.concat(df1, ignore_index=True)
    return df1

def load_files(filelist):

    with open(filelist) as f:
        files = f.read().splitlines()
        file_names = [os.path.splitext(os.path.basename(file))[0] for file in files]

    return file_names

