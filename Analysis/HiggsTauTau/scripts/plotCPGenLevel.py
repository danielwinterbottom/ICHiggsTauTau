import ROOT
import numpy as np
from array import array
import argparse
import datetime
import shutil

import UserCode.ICHiggsTauTau.plotting as plotting

ROOT.gROOT.SetBatch(True)
plotting.ModTDRStyle()

def parse_arguments():
    parser = argparse.ArgumentParser()
    parser.add_argument("--channel", action="store", default="tt",
            help="Decide which channel to use")
    parser.add_argument("--sign", action="store", default="neg",
            help="Decide which cp sign to use")
    parser.add_argument("--smearE", action="store_true", default=False,
            help="Want to smear E?")
    parser.add_argument("--smearEta", action="store_true", default=False,
            help="Want to smear Eta?")
    parser.add_argument("--smearPhi", action="store_true", default=False,
            help="Want to smear Phi?")
    parser.add_argument("--doPi0", action="store_true", default=False,
            help="Want to smear with Pi0 samplings?")
    parser.add_argument("--smearAll", action="store_true", default=False,
            help="Want to smear all for both pi0 and pi+-?")
    parser.add_argument("--checkSpinner", action="store_true", default=False,
            help="Check spinner?")

    return parser.parse_args()

def main(args):

    # file1_ = ROOT.TFile.Open("/vols/cms/akd116/Offline/output/SM/2019/Jan29_2016/GluGluToHToTauTau_M-125-nospinner_{}_2016.root".format(args.channel), "READ")
    file1_ = ROOT.TFile.Open("/vols/cms/dw515/Offline/output/SM/Jan22/VBFHToTauTau_M-125-nospinner_{}_2016.root".format(args.channel), "READ")

    # file1_ = ROOT.TFile.Open("/vols/cms/akd116/Offline/output/SM/2019/Jan28/GluGluHToTauTau_M-125_{}_2017.root".format(args.channel), "READ")
    # file2_ = ROOT.TFile.Open("/vols/cms/akd116/Offline/output/SM/2019/Jan29/GluGluToHToTauTau_M125_nospinner-2017_{}_2017.root".format(args.channel), "READ")

    # tree_ = file_.Get("gen_ntuple")
    tree1_ = file1_.Get("ntuple")
    # tree2_ = file2_.Get("gen_ntuple")

    if args.sign == "pos":
        sel_  = ROOT.TCut("wt*(pt_1>40 && pt_2>40)*(cp_channel==3)*(cp_sign>0)")
        sel_sm  = ROOT.TCut("wt*wt_cp_sm*(pt_1>40 && pt_2>40)*(cp_channel==3)*(cp_sign>0)")
        sel_ps  = ROOT.TCut("wt*wt_cp_ps*(pt_1>40 && pt_2>40)*(cp_channel==3)*(cp_sign>0)")
        sel_mm  = ROOT.TCut("wt*wt_cp_mm*(pt_1>40 && pt_2>40)*(cp_channel==3)*(cp_sign>0)")
    else:
        sel_  = ROOT.TCut("wt*(pt_1>40 && pt_2>40)*(cp_channel==3)*(cp_sign<0)")
        sel_sm  = ROOT.TCut("wt*wt_cp_sm*(pt_1>40 && pt_2>40)*(cp_channel==3)")
        sel_ps  = ROOT.TCut("wt*wt_cp_ps*(pt_1>40 && pt_2>40)*(cp_channel==3)")
        sel_mm  = ROOT.TCut("wt*wt_cp_mm*(pt_1>40 && pt_2>40)*(cp_channel==3)*(cp_sign<0)")

    hists = []
    legends = []
    h1 = ROOT.TH1D("h1","h1",20,0,6.3)
    tree1_.Draw("aco_angle_mod>>h1", sel_sm)
    hists.append(h1)
    legends.append("qqH SM")

    h2 = ROOT.TH1D("h2","h2",20,0,6.3)
    tree1_.Draw("aco_angle_mod>>h2", sel_ps)
    hists.append(h2)
    legends.append("qqH PS")
    
    plotname = "VBF_reco_CP{}_tauspinner".format(args.sign)
    # if args.checkSpinner:
    #     plotname = "ggH_gen_CP{}_tauspinner_check".format(args.sign)
    #     h2 = ROOT.TH1D("h2","h2",10,0,6.3)
    #     tree1_.Draw("aco_angle_1>>h2", sel_sm)
    #     hists.append(h2)
    #     legends.append("ggH SM tauspinner")

    # if args.doPi0:
    #     plotname = "ggH_gen_CP{}_tauspinner_smearingPi0".format(args.sign)
    #     if args.smearE:
    #         h2 = ROOT.TH1D("h2","h2",10,0,6.3)
    #         tree_.Draw("aco_angle_1_Pi0ESmeared>>h2", sel_sm)
    #         hists.append(h2)
    #         legends.append("ggH SM E(strip) smeared")
            
    #     if args.smearPhi:
    #         h3 = ROOT.TH1D("h3","h3",10,0,6.3)
    #         tree_.Draw("aco_angle_1_Pi0PhiSmeared>>h3", sel_sm)
    #         hists.append(h3)
    #         legends.append("ggH SM #phi(strip) smeared")

    #     if args.smearEta:
    #         h4 = ROOT.TH1D("h4","h4",10,0,6.3)
    #         tree_.Draw("aco_angle_1_Pi0EtaSmeared>>h4", sel_sm)
    #         hists.append(h4)
    #         legends.append("ggH SM #eta(strip) smeared")

    #     h5 = ROOT.TH1D("h5","h5",10,0,6.3)
    #     tree_.Draw("aco_angle_1>>h5", sel_ps)
    #     hists.append(h5)
    #     legends.append("ggH PS")
    #     if args.smearE:
    #         h6 = ROOT.TH1D("h6","h6",10,0,6.3)
    #         tree_.Draw("aco_angle_1_Pi0ESmeared>>h6", sel_ps)
    #         hists.append(h6)
    #         legends.append("ggH PS E(strip) smeared")
    #     if args.smearPhi:
    #         h7 = ROOT.TH1D("h7","h7",10,0,6.3)
    #         tree_.Draw("aco_angle_1_Pi0PhiSmeared>>h7", sel_ps)
    #         hists.append(h7)
    #         legends.append("ggH PS #phi(strip) smeared")
    #     if args.smearEta:
    #         h8 = ROOT.TH1D("h8","h8",10,0,6.3)
    #         tree_.Draw("aco_angle_1_Pi0EtaSmeared>>h8", sel_ps)
    #         hists.append(h8)
    #         legends.append("ggH PS #eta(strip) smeared")
    # elif not args.smearAll and not args.checkSpinner:
    #     plotname = "ggH_gen_CP{}_tauspinner_smearingPi".format(args.sign)
    #     if args.smearE:
    #         h2 = ROOT.TH1D("h2","h2",10,0,6.3)
    #         tree_.Draw("aco_angle_1_PiESmeared>>h2", sel_sm)
    #         hists.append(h2)
    #         legends.append("ggH SM E(CH) smeared")
            
    #     if args.smearPhi:
    #         h3 = ROOT.TH1D("h3","h3",10,0,6.3)
    #         tree_.Draw("aco_angle_1_PiPhiSmeared>>h3", sel_sm)
    #         hists.append(h3)
    #         legends.append("ggH SM #phi(CH) smeared")

    #     if args.smearEta:
    #         h4 = ROOT.TH1D("h4","h4",10,0,6.3)
    #         tree_.Draw("aco_angle_1_PiEtaSmeared>>h4", sel_sm)
    #         hists.append(h4)
    #         legends.append("ggH SM #eta(CH) smeared")

    #     h5 = ROOT.TH1D("h5","h5",10,0,6.3)
    #     tree_.Draw("aco_angle_1>>h5", sel_ps)
    #     hists.append(h5)
    #     legends.append("ggH PS")
    #     if args.smearE:
    #         h6 = ROOT.TH1D("h6","h6",10,0,6.3)
    #         tree_.Draw("aco_angle_1_PiESmeared>>h6", sel_ps)
    #         hists.append(h6)
    #         legends.append("ggH PS E(CH) smeared")
    #     if args.smearPhi:
    #         h7 = ROOT.TH1D("h7","h7",10,0,6.3)
    #         tree_.Draw("aco_angle_1_PiPhiSmeared>>h7", sel_ps)
    #         hists.append(h7)
    #         legends.append("ggH PS #phi(CH) smeared")
    #     if args.smearEta:
    #         h8 = ROOT.TH1D("h8","h8",10,0,6.3)
    #         tree_.Draw("aco_angle_1_PiEtaSmeared>>h8", sel_ps)
    #         hists.append(h8)
    #         legends.append("ggH PS #eta(CH) smeared")
    # elif args.smearAll and not args.checkSpinner:
    #     plotname = "ggH_gen_CP{}_tauspinner_smearingFull".format(args.sign)

    #     h2 = ROOT.TH1D("h2","h2",10,0,6.3)
    #     tree1_.Draw("aco_angle_1_FullSmeared>>h2", sel_sm)
    #     hists.append(h2)
    #     legends.append("ggH SM smeared")

    #     h5 = ROOT.TH1D("h5","h5",10,0,6.3)
    #     tree1_.Draw("aco_angle_1>>h5", sel_ps)
    #     hists.append(h5)
    #     legends.append("ggH PS")

    #     h6 = ROOT.TH1D("h6","h6",10,0,6.3)
    #     tree1_.Draw("aco_angle_1_FullSmeared>>h6", sel_ps)
    #     hists.append(h6)
    #     legends.append("ggH PS smeared")


    # hists.extend([h1,h2,h3,h4,h5,h6,h7,h8])


    # legends = [
    #         "ggH SM",
    #         "ggH SM E(strip) smeared",
    #         "ggH SM Phi(strip) smeared",
    #         "ggH SM Eta(strip) smeared",
    #         "ggH PS",
    #         "ggH PS E(strip) smeared",
    #         "ggH PS Phi(strip) smeared",
    #         "ggH PS Eta(strip) smeared",
    #         ]

    plotting.CompareHists(
            hists,
            legends,
            ratio=False,
            extra_pad=0.3,
            norm_hists=True,
            norm_bins=False,
            x_title="#phi_{#rho#rho}",
            y_title="a.u.",
            plot_name=plotname,
            # plot_name="VBFH_gen_CP{}".format(args.sign),
            # custom_y_range=True,
            # y_axis_min=0.,
            # y_axis_max=0.25,
            )

if __name__ == "__main__":
    args = parse_arguments()
    main(args)
