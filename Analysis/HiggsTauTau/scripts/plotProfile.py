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
    parser.add_argument("--training", action="store", default="Sep20_4",
            help="Decide which training to use")
    parser.add_argument("--channel", action="store", default="tt",
            help="Decide which channel to use")
    parser.add_argument("--cat", action="store", default="ggh",
            help="Decide which category to use")

    return parser.parse_args()

def main(args):

    if args.channel == "tt":
        cats = ["ggh","qqh","zttEmbed","jetFakes"]
    else:
        cats = []
    
    reduced_cats = [x for x in cats if args.cat != x]

    datacards = []
    datacard = ROOT.TFile.Open("./output/datacard_mjj_vs_IC_highMjj_{}_max_score_{}_highMjj_{}_2016.root".format(args.training,args.cat,args.channel), "READ")

    datacards.append(datacard)

    for cat in reduced_cats:
        datacard = ROOT.TFile.Open("./output/datacard_mjj_vs_IC_highMjj_{}_max_score_{}_highMjj_{}_2016.root".format(args.training,cat,args.channel), "READ")
        datacards.append(datacard)

    # datacard1 = ROOT.TFile.Open("./output/datacard_mjj_vs_IC_highMjj_{}_max_score_qqh_highMjj_{}_2016.root".format(args.training,args.channel), "READ")
    # datacard2 = ROOT.TFile.Open("./output/datacard_mjj_vs_IC_highMjj_{}_max_score_zttEmbed_highMjj_{}_2016.root".format(args.training,args.channel), "READ")
    # datacard3 = ROOT.TFile.Open("./output/datacard_mjj_vs_IC_highMjj_{}_max_score_jetFakes_highMjj_{}_2016.root".format(args.training,args.channel), "READ")
    # if args.channel in ["mt","et","em"]:
    #     datacard4 = ROOT.TFile.Open("./output/datacard_mjj_vs_IC_highMjj_{}_max_score_tt_highMjj_{}_2016.root".format(args.training,args.channel), "READ")

    
    # directory = ROOT.gFile.GetDirectory("{}_{}_highMjj".format(args.channel,args.cat))
    # directory = datacard.GetDirectory("{}_{}_highMjj".format(args.channel,args.cat))
    # directory1 = datacard1.GetDirectory("{}_qqh_highMjj".format(args.channel))
    # directory2 = datacard1.GetDirectory("{}_zttEmbed_highMjj".format(args.channel))
    # directory3 = datacard1.GetDirectory("{}_jetFakes_highMjj".format(args.channel))
    
    hists = []
    hist = datacards[0].Get("{}_{}_highMjj/ggHsm_htt125".format(args.channel,args.cat))
    hists.append(hist)

    for i, cat in enumerate(reduced_cats):
        hist = datacards[i+1].Get("{}_{}_highMjj/ggHsm_htt125".format(args.channel,cat))
        hists.append(hist)

    # hist_ggh =   datacard.Get("{}_{}_highMjj/ggHsm_htt125".format(args.channel,args.cat))
    # hist_ggh1 = datacard1.Get("{}_qqh_highMjj/ggHsm_htt125".format(args.channel))
    # hist_ggh2 = datacard2.Get("{}_zttEmbed_highMjj/ggHsm_htt125".format(args.channel))
    # hist_ggh3 = datacard3.Get("{}_jetFakes_highMjj/ggHsm_htt125".format(args.channel))
    # if args.channel in ["mt","et","em"]:
    #     hist_ggh4 = datacard4.Get("{}_tt_highMjj/ggHsm_htt125".format(args.channel))
    

    mean_mjj_num = 0
    mean_mjj_denom = 0
    for hist in hists:
        mean_mjj_num += hist.Integral()*hist.ProjectionY("",-1,-1).GetMean()
        mean_mjj_denom += hist.Integral()
    mean_mjj = mean_mjj_num/mean_mjj_denom

    # mean_mjj = (hist_ggh.Integral()*hist_ggh.ProjectionY("",-1,-1).GetMean() + 
    #         hist_ggh1.Integral()*hist_ggh1.ProjectionY("",-1,-1).GetMean() + 
    #         hist_ggh2.Integral()*hist_ggh2.ProjectionY("",-1,-1).GetMean() + 
    #         hist_ggh3.Integral()*hist_ggh3.ProjectionY("",-1,-1).GetMean())/(hist_ggh.Integral()+hist_ggh1.Integral()+hist_ggh2.Integral()+hist_ggh3.Integral())
    # if args.channel in ["mt","et","em"]:
    #     mean_mjj = (hist_ggh.Integral()*hist_ggh.ProjectionY("",-1,-1).GetMean() + 
    #             hist_ggh1.Integral()*hist_ggh1.ProjectionY("",-1,-1).GetMean() + 
    #             hist_ggh2.Integral()*hist_ggh2.ProjectionY("",-1,-1).GetMean() + 
    #             hist_ggh3.Integral()*hist_ggh3.ProjectionY("",-1,-1).GetMean() +
    #             hist_ggh4.Integral()*hist_ggh4.ProjectionY("",-1,-1).GetMean())/(hist_ggh.Integral()+hist_ggh1.Integral()+hist_ggh2.Integral()+hist_ggh3.Integral()+hist_ggh4.Integral())

    profile = hists[0].ProfileX()

    # profile = hist_ggh.ProfileX()
    
    c1 = ROOT.TCanvas()
    c1.cd()
    
    print "mean mjj across all cats ", mean_mjj
    profile.Draw()
    line = ROOT.TLine(0,mean_mjj,1,mean_mjj)
    line.SetLineStyle(2)
    line.Draw()
    
    profile.GetYaxis().SetTitle("m_{jj} (GeV)")
    profile.GetYaxis().SetRangeUser(300.,1000.)
    if args.cat == "qqh":
        profile.GetYaxis().SetRangeUser(300.,1500.)
    profile.GetXaxis().SetTitle("MVA score bin")
    
    c1.SaveAs("mjj_vs_mva_profile_{}_{}_{}.pdf".format(args.training,args.channel,args.cat))

if __name__ == "__main__":
    args = parse_arguments()
    main(args)
