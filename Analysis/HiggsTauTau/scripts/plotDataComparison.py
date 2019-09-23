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
    parser.add_argument("--channel", action="store", default="zmm",
            help="Decide which channel to use")
    parser.add_argument("--var", action="store", default="sjdphi",
            help="Decide which variable to compare")
    parser.add_argument("--run", action="store", default="B",
            help="Decide which run to compare")

    return parser.parse_args()

def main(args):

    # file_ = []
    # file_1 = ROOT.TFile.Open("/vols/cms/akd116/Offline/output/SM/2018/Nov16_allJets/SingleMuon{}_{}_2017.root".format(args.run,args.channel), "READ")
    # file_1 = ROOT.TFile.Open("/vols/cms/akd116/Offline/output/SM/2019/Jul14_2017_allJets/SingleMuon{}_{}_2017.root".format(args.run,args.channel), "READ")
    # file_1 = ROOT.TFile.Open("/vols/cms/akd116/Offline/output/SM/2019/Jul14_2017_ptNEMFcut/SingleMuon{}_{}_2017.root".format(args.run,args.channel), "READ")
    # # file_1 = ROOT.TFile.Open("/vols/cms/akd116/Offline/output/SM/2018/Nov20_PUJID/SingleMuon{}_{}_2017.root".format(args.run,args.channel), "READ")
    # file_2 = ROOT.TFile.Open("/vols/cms/dw515/Offline/output/SM/EmbedZMM2016/SingleMuon{}_{}_2016.root".format(args.run,args.channel), "READ")
    # file_.extend([file_1,file_2])
    # tree_1 = file_[0].Get("ntuple")
    # tree_2 = file_[1].Get("ntuple")
    # tree_1 = file_1.Get("ntuple")

    chain1 = ROOT.TChain("ntuple")
    chain2 = ROOT.TChain("ntuple")
    for run_ in ["B","C","D","E","F"]:
        # chain1.AddFile("/vols/cms/akd116/Offline/output/SM/2018/Nov16_allJets/SingleMuon{}_{}_2017.root".format(run_,args.channel))
        # chain1.AddFile("/vols/cms/akd116/Offline/output/SM/2018/Nov20_PUJID/SingleMuon{}_{}_2017.root".format(run_,args.channel))
        # chain1 = ROOT.TFile.Open("/vols/cms/akd116/Offline/output/SM/2019/Jul14_2017_allJets/SingleMuon{}_{}_2017.root".format(args.run,args.channel), "READ")

        chain1.AddFile("/vols/cms/akd116/Offline/output/SM/2019/Jul14_2017_ptNEMFcut/SingleMuon{}_{}_2017.root".format(args.run,args.channel))
        # chain1.AddFile("/vols/cms/akd116/Offline/output/SM/2019/Jul14_2017_allJets/SingleMuon{}_{}_2017.root".format(args.run,args.channel))
    for run_ in ["B","C","D","E","F","G","Hv2","Hv3"]:
        chain2.AddFile("/vols/cms/dw515/Offline/output/SM/EmbedZMM2016/SingleMuon{}_{}_2016.root".format(run_,args.channel))

    hists = []
    h1 = ROOT.TH1D("h1","h1",40,-3.2,3.2)
    h2 = ROOT.TH1D("h2","h2",40,-3.2,3.2)
    # h1 = ROOT.TH1D("h1","h1",17,30,200)
    # h2 = ROOT.TH1D("h2","h2",17,30,200)
    hists.extend([h1,h2])

    baseline = ROOT.TCut("pt_1>25 && iso_1<0.15 && iso_2<0.15 && trg_singlemuon")

    sel1 = ROOT.TCut(
            "fabs(jeta_1)>2.65 && fabs(jeta_1)<3.139 && m_vis>70 && m_vis<110 && n_jets==2 && pt_1>30 && pt_tt<50 "
            )
    # sel2 = ROOT.TCut(
    #         "fabs(jeta_2)>2.65 && fabs(jeta_2)<3.139 && m_vis>70 && m_vis<110 && n_jets==2 && pt_1>30 && pt_tt<50 "
    #         )
    # sel2 = ROOT.TCut(
    #         "fabs(jeta_1)>2.65 && fabs(jeta_1)<3.139 && m_vis>70 && m_vis<110 && n_jets==2 "
    #         )
    # sel1 = ROOT.TCut(
    #         "fabs(jeta_1)>2.65 && fabs(jeta_1)<3.139 && m_vis>70 && m_vis<110 && n_jets==1 && fabs(dphi_jtt)>2.5 && (jpt_1/pt_vis)>0.5 && (jpt_1/pt_vis)<1.5"
    #         )
    # sel2 = ROOT.TCut(
    #         "fabs(jeta_1)>2.65 && fabs(jeta_1)<3.139 && m_vis>70 && m_vis<110 && n_jets==1 && fabs(dphi_jtt)<1.5 && (jpt_1/pt_vis)>0.5 && (jpt_1/pt_vis)<1.5"
    #         )

    # hist_1 = tree_1.Draw("{}>>h1".format(args.var), baseline+sel1)
    # hist_2 = tree_1.Draw("{}>>h2".format(args.var), baseline+sel2)

    chain1.Draw("{}>>h1".format(args.var), baseline+sel1)
    chain2.Draw("{}>>h2".format(args.var), baseline+sel1)

    # h1_c = h1.Clone()
    # h2_c = h2.Clone()
    # h2_c.Scale((np.pi - 2.5)/1.5)
    # h1_c.Add(h2_c,-1)

    # print h2.Integral(0,5)/h2.Integral()
    # print h1_c.Integral(0,5)/h1_c.Integral()

    # hists.append(h1_c)

    legends = [
            # "Genuine jet enriched",
            # "PU/noise jet enriched",
            # "Subtracted PU",
            # "2017 jets pT > 50 GeV",
            # "2017 PU Jet ID RunE-F",
            # "2017 PU Jet ID RunE-F",
            # "2017 all jets RunE-F",
            # "2017 all jets",
            "2017 jet mitigation",
            "2016 all jets",
            ]

    plotting.CompareHists(
            hists,
            legends,
            ratio=True,
            ratio_range="0.3,1.7",
            norm_hists=True,
            norm_bins=False,
            # title="Run{}".format(args.run),
            x_title="#Delta#phi_{jj}",
            # x_title="P_{T}^{j_{1}} (GeV)",
            y_title="Normalised events",
            plot_name="compare1617_ptNEMFcut_Zless50_{}".format(args.var),
            # plot_name="genuine_pu_jets_2017zmm_{}".format(args.var),
            extra_pad=0.5,
            # custom_y_range=True,
            # y_axis_min=0,
            # y_axis_max=2,
            )

if __name__ == "__main__":
    args = parse_arguments()
    main(args)
