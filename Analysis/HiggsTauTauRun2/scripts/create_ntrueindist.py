import sys
import os
import glob
import UserCode.ICHiggsTauTau.plotting as plotting

import ROOT
ROOT.gROOT.SetBatch(True)

def load_files(filelist):
    with open(filelist) as f:
         files = f.read().splitlines()
         file_names = [os.path.splitext(os.path.basename(file))[0] for file in files]
    return file_names

year = "2022"
files = ["DYJetsToLL-LO_postEE_postEE"]
channels = ["tt"]

for file_ in files:
    for path in glob.glob(sys.argv[1]):
        print(path)
        hist = ROOT.TH1D("pileup", ";PileUp_nTrueInt", 100, 0., 100.) # for 2017UL
        chain = ROOT.TChain("gen_ntuple")
        for channel in channels:
            chain.AddFile("{}/{}_{}_{}.root".format(path,file_,channel,year))

        chain.Draw("n_pu>>pileup","wt")
    
        if not os.path.exists(os.path.join(path, "pileupDistribution")):
            os.makedirs(os.path.join(path, "pileupDistribution"))

        # if year == "2016" and split == "pre":  new_file = ROOT.TFile.Open(os.path.join(path, "pileupDistribution/pileup_{}_preEE_{}.root".format(year,file_)), "RECREATE")
        # elif year == "2016" and split == "post": new_file = ROOT.TFile.Open(os.path.join(path, "pileupDistribution/pileup_{}_postEE_{}.root".format(year,file_)), "RECREATE")
        # else: new_file = ROOT.TFile.Open(os.path.join(path, "pileupDistribution/pileup_{}_{}.root".format(year,file_)), "RECREATE")
        new_file = ROOT.TFile.Open(os.path.join(path, "pileupDistribution/pileup_{}_{}.root".format(year,file_)), "RECREATE")


        hist.SetDirectory(new_file)
        new_file.Write()
        new_file.Close()
        del hist
        del new_file
        chain.Reset()
