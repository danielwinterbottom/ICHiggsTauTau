import ROOT
import numpy as np
from array import array
import argparse
import datetime
import shutil

import UserCode.ICHiggsTauTau.plotting as plotting

ROOT.gROOT.SetBatch(True)
plotting.ModTDRStyle()
ROOT.TH1.SetDefaultSumw2(True)

def parse_arguments():
    parser = argparse.ArgumentParser()
    parser.add_argument("--channel", action="store", default="tt",
            help="Decide which channel to use")

    return parser.parse_args()

def main(args):

    file_ = []
    file_sm = ROOT.TFile.Open("/vols/cms/akd116/Offline/output/SM/2018/Nov19_gen/VBFHToTauTau_M-125_{}_2017.root".format(args.channel), "READ")
    file_ps = ROOT.TFile.Open("/vols/cms/akd116/Offline/output/SM/2018/Nov19_gen/VBFHToPseudoscalarTauTau_GEN_2017_{}_0.root".format(args.channel), "READ")
    file_mm = ROOT.TFile.Open("/vols/cms/akd116/Offline/output/SM/2018/Nov19_gen/VBFHToMaxmixTauTau_GEN_2017_{}_0.root".format(args.channel), "READ")
    file_dy = ROOT.TFile.Open("/vols/cms/akd116/Offline/output/SM/2018/Nov19_gen/DYJetsToLL-LO_{}_2017.root".format(args.channel), "READ")
    file_.extend([file_sm,file_ps,file_mm,file_dy])
    tree_sm1 = file_[0].Get("gen_ntuple")
    tree_ps1 = file_[1].Get("gen_ntuple")
    tree_mm1 = file_[2].Get("gen_ntuple")
    tree_dy = file_[3].Get("gen_ntuple")

    hists = []
    h1 = ROOT.TH1D("h1","h1",5,0,6.3)
    h2 = ROOT.TH1D("h2","h2",5,0,6.3)
    h3 = ROOT.TH1D("h3","h3",5,0,6.3)
    hists.extend([h1,h2,h3])

    sel = ROOT.TCut("cp_channel==-1 && aco_angle_2<6.3 && cp_sign_2<0 && pt_1>20 && pt_2>20")

    hist_sm1 = tree_sm1.Draw("aco_angle_2>>h1", sel)
    hist_ps1 = tree_ps1.Draw("aco_angle_2>>h2", sel)
    hist_dy =   tree_dy.Draw("aco_angle_2>>h3", sel)

    legends = [
            "VBFHToTauTau SM",
            "VBFHToTauTau PS",
            "DY",
            ]

    plotting.CompareHists(
            hists,
            legends,
            ratio=False,
            norm_hists=True,
            x_title="#phi_{#rho#rho}",
            y_title="a.u.",
            plot_name="VBF_gen_CPneg",
            custom_y_range=True,
            y_axis_min=0.05,
            y_axis_max=0.35,
            )

if __name__ == "__main__":
    args = parse_arguments()
    main(args)
