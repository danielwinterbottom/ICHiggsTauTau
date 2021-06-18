#!/usr/bin/env python

import ROOT
ROOT.PyConfig.IgnoreCommandLineOptions = True  # disable ROOT internal argument parser

import logging
logger = logging.getLogger("annotate_file_inc.py")
logger.setLevel(logging.DEBUG)
formatter = logging.Formatter("%(name)s - %(levelname)s - %(message)s")
handler = logging.StreamHandler()
handler.setFormatter(formatter)
logger.addHandler(handler)

import numpy as np
import yaml
import os
import pickle
from array import array
import argparse
from sklearn.preprocessing import StandardScaler

def parse_arguments():
    parser = argparse.ArgumentParser(
        description="Apply XGB model on ROOT file")
    parser.add_argument(
        "--config-training",
        default="mt_xgb_training_config.yaml",
        help="Path to training config file")
    parser.add_argument(
        "--dir-prefix",
        type=str,
        default="ntuple",
        help="Prefix of directories in ROOT file to be annotated.")
    parser.add_argument(
        "input", help="Path to input file, where response will be added.")
    parser.add_argument(
        "tag", help="Tag to be used as prefix of the annotation.")
    parser.add_argument(
        "--models",
        type=str,
        nargs="+",
        default=["multi_fold1_cpsm_mt_JHU_xgb.pkl", "multi_fold0_cpsm_mt_JHU_xgb.pkl"],
        help=
        "Keras models to be used for the annotation. Note that these have to be booked in the reversed order [fold1*, fold0*], so that the training is independent from the application."
    )
    parser.add_argument(
        "--preprocessing",
        type=str,
        nargs="+",
        default=[
            "tt_training_9May_tests/tt_fold1_scaler.pkl",
            "tt_training_9May_tests/tt_fold0_scaler.pkl"
        ],
        help=
        "Data preprocessing to be used. Note that these have to be booked in the reversed order [fold1*, fold0*], so that the preprocessing is independent for the folds."
    )
    #parser.add_argument(
    #    "--nsplit",
    #    type=int,
    #    default=0,
    #    help=
    #    "When splitting into multiple jobs determins the split to be run"
    #)
    parser.add_argument(
        "--output-folder",type=str, default="",
        help="If specified, chose where to save outputs with BDT scores"
    )
    parser.add_argument(
        "--tree", default="ntuple", help="Name of trees in the directories.")
    parser.add_argument(
        "--training", default="JHU", help="Name of training to use.")
    parser.add_argument(
        "--mjj", default="high", help="mjj training to use.")
    parser.add_argument(
        "--channel", default="mt", help="Name of channel to annotate.")
    parser.add_argument(
        "--model_folder", default="mt_training_10May_mjj_jdeta_dijetpt/", help="Folder name where trained model is.")
    parser.add_argument(
        "--path", default="./", help="directory with the trees to have BDT scores added to.")
    parser.add_argument(
        "--era", default="", help="Year to use.")

    return parser.parse_args()


def parse_config(filename):
    return yaml.load(open(filename, "r"))


def load_files(filelist):

    with open(filelist) as f:
        file_names = f.read().splitlines()
        # file_names = [os.path.splitext(os.path.basename(file))[0] for file in files]

    return file_names


def main(args, config, file_names):

    path = args.path

    # Sanity checks
    for sample_ in file_names:
        sample = sample_.split()[0]
        if len(sample_.split())>1: 
            nsplit = int(sample_.split()[1])
        else: 
            nsplit = 0
        print(sample, nsplit)
        # if not os.path.exists("{}/{}_{}_{}.root".format(path, sample, args.channel, args.era)):
        if not os.path.exists("{}/{}".format(path, sample)):
            logger.fatal("Input file %s does not exist.", sample)
            raise Exception

        logger.debug("Following mapping of classes to class numbers is used.")
        for i, class_ in enumerate(config["classes"]):
            logger.debug("%s : %s", i, class_)

        # Load Keras models and preprocessing

        if args.era != "":
            with open('{}/multi_fold1_sm_{}_{}_{}_xgb.pkl'
                    .format(args.model_folder, args.channel, args.training, args.era), 'r') as f:
                xgb_clf_fold1 = pickle.load(f)
            with open('{}/multi_fold0_sm_{}_{}_{}_xgb.pkl'
                    .format(args.model_folder, args.channel, args.training, args.era), 'r') as f:
                xgb_clf_fold0 = pickle.load(f)
        elif args.training == "madgraph" or args.training == "powheg":
            with open('{}/multi_fold1_sm_{}_{}_{}_xgb.pkl'
                    .format(args.model_folder, args.channel, args.training, args.mjj), 'r') as f:
                xgb_clf_fold1 = pickle.load(f)
            with open('{}/multi_fold0_sm_{}_{}_{}_xgb.pkl'
                    .format(args.model_folder, args.channel, args.training, args.mjj), 'r') as f:
                xgb_clf_fold0 = pickle.load(f)
        else:
            with open('{}/multi_fold1_sm_{}_{}_xgb.pkl'
                    .format(args.model_folder, args.channel, args.training), 'r') as f:
                xgb_clf_fold1 = pickle.load(f)
            with open('{}/multi_fold0_sm_{}_{}_xgb.pkl'
                    .format(args.model_folder, args.channel, args.training), 'r') as f:
                xgb_clf_fold0 = pickle.load(f)
        classifier = [xgb_clf_fold1, xgb_clf_fold0]

        # with open('{}_{}_scaler.pkl'
        #         .format(args.channel,args.mjj), 'r') as f:
        #     preprocessing = pickle.load(f)
        # preprocessing = StandardScaler()
        # preprocessing = [pickle.load(open(x, "rb")) for x in args.preprocessing]

        # Open input file
        file_ = ROOT.TFile("{}/{}".format(path, sample))
        #fileout_ = ROOT.TFile("{}/{}".format(path, sample), "APPEND")
        if file_ == None:
            logger.fatal("File %s is not existent.", sample)
            raise Exception

        tree = file_.Get(args.tree)
        # if tree == None:
        #     logger.fatal("Failed to find tree %s in directory %s.",
        #                  args.tree, name)
        #     raise Exception

        # Book branches for annotation
        values = []
        for variable in config["variables"]:
            if variable in ["dijetpt","eta_h","IC_binary_test_4_score","IC_binary_test_4_index","bpt_1",]:
                values.append(array("f", [-9999]))
            if variable in ["eta_1","eta_2","jdeta","jpt_1","jpt_2","svfit_mass","m_sv","m_vis","met","jeta_1","jeta_2","mt_tot","mt_sv",
                    "met_dphi_1","met_dphi_2","mjj","mt_1","mt_2","mt_lep","pt_1","pt_2","pt_h","pt_tt","pt_vis","pzeta","dR"]:
                values.append(array("d", [-9999]))
            if variable in ["n_jets","n_bjets","opp_sides"]:
                values.append(array("I", [0]))
            if variable not in ["zfeld","centrality","mjj_jdeta","dijetpt_pth","dijetpt_jpt1","dijetpt_pth_over_pt1",
                    "msv_mvis","msvsq_mvis","msv_sq","log_metsq_jeta2","met_jeta2","oppsides_centrality","pthsq_ptvis","msv_rec","dR_custom","rms_pt","rms_jpt","rec_sqrt_msv"]:
                tree.SetBranchAddress(variable, values[-1])
            # else:
            #     tree.SetBranchAddress("eta_h", values[-1])


########################################

        response_max_score = array("f", [-9999])
        response_max_index = array("f", [-9999])

        if tree.GetListOfBranches().FindObject("{}_max_score".format(args.tag)):
           branch_max_score = tree.GetBranch("{}_max_score".format(args.tag))
           tree.SetBranchAddress("{}_max_score".format(args.tag),response_max_score)
        else:
          branch_max_score = tree.Branch("{}_max_score".format(
              args.tag), response_max_score, "{}_max_score/F".format(
                  args.tag))

        if tree.GetListOfBranches().FindObject("{}_max_index".format(args.tag)):
           branch_max_index = tree.GetBranch("{}_max_index".format(args.tag))
           tree.SetBranchAddress("{}_max_index".format(args.tag),response_max_index)
        else: 
          branch_max_index = tree.Branch("{}_max_index".format(
              args.tag), response_max_index, "{}_max_index/F".format(
                  args.tag))

###############################
        
        if args.output_folder != "":
            fileout_ = ROOT.TFile(
                "{}/{}".format(args.output_folder, sample.replace('.root','_{}.root'.format(nsplit))), 
                "RECREATE"
            )
        else:
            fileout_ = ROOT.TFile(
                "{}/{}".format(path, sample.replace('.root','_{}.root'.format(nsplit))), 
                "RECREATE"
            )
        newtree=tree.CloneTree(0)

        # Run the event loop

        perjobs=300000
        mini=nsplit*300000
        maxi=(nsplit+1)*300000
        entries=tree.GetEntries()
        if maxi > entries: maxi=entries


        for i_event in range(mini,maxi):

            tree.GetEntry(i_event)

            # Get event number and compute response
            event = int(getattr(tree, "event"))
            if tree.GetListOfBranches().FindObject("svfit_mass"):
              m_sv = float(getattr(tree, "svfit_mass"))
            else: m_sv=-1

            #print m_sv

            if m_sv > 0:

                values_stacked = np.hstack(values).reshape(1, len(values))

                # only define dijet features in case when there are two jets
                # with pT > 30 GeV
                # hacky but works for now
                # indices
                # mjj: 4, jdeta: 0, jpt_1: 1, n_jets: 5
                # fix dijet variables:
                if values_stacked[0][5] < 2:
                    values_stacked[0][4] = -9999.
                    values_stacked[0][0] = -9999.
                if values_stacked[0][5] < 1:
                    values_stacked[0][1] = -9999.
                response = classifier[event % 2].predict_proba(values_stacked,
                        ntree_limit=classifier[event % 2].best_iteration+1)
                response = np.squeeze(response)

                # Find max score and index
                response_max_score[0] = -9999.0
                for i, r in enumerate(response):
                    if r > response_max_score[0]:
                        response_max_score[0] = r
                        response_max_index[0] = i
                if i_event % 10000 == 0:
                    logger.debug('Currently on event {}'.format(i_event))

                #print response_max_score[0]

                # Fill branches
                newtree.Fill()
                #branch_max_score.Fill()
                #branch_max_index.Fill()

            else:
                response_max_score[0] = -9999.0
                response_max_index[0] = -9999.0

                # Fill branches
                newtree.Fill()
                #branch_max_score.Fill()
                #branch_max_index.Fill()

        logger.debug("Finished looping over events")

        # Write everything to file
        fileout_.cd()
        newtree.Show(1)
        newtree.Write("ntuple",ROOT.TObject.kWriteDelete)
        fileout_.Close()
        file_.Close()

        logger.debug("Closed file")

if __name__ == "__main__":
    args = parse_arguments()
    config = parse_config(args.config_training)
    file_names = load_files(args.input)
    main(args, config, file_names)
