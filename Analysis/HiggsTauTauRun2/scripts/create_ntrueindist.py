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

# files = load_files("scripts/dy_2017for18.txt")
# files = load_files("new_mc2017puprofiles.txt")
# files = load_files("new_mc2017puprofiles.txt")
files = load_files("scripts/Apr22_2016preVFP_mcPU.txt")
#files = load_files("scripts/Apr22_2016postVFP_mcPU.txt")
#files = load_files("scripts/Apr22_2017_mcPU.txt")
#files = load_files("scripts/Apr22_2018_mcPU.txt")




channels = ["mt"]
for file_ in files:
    for path in glob.glob(sys.argv[1]):
        print path
        # hist = ROOT.TH1D("pileup_{}".format(file_), ";PileUp_nTrueInt", 200, 0., 200.)
        hist = ROOT.TH1D("pileup", ";PileUp_nTrueInt", 99, 0., 99.) # for 2017UL
        # hist = ROOT.TH1D("pileup", ";PileUp_nTrueInt", 100, 0., 100.) # for 2018
        chain = ROOT.TChain("gen_ntuple")
        for channel in channels:
            chain.AddFile("{}/{}_{}_2016.root".format(path,file_,channel))
            #chain.AddFile("{}/{}_{}_2017.root".format(path,file_,channel))
           # chain.AddFile("{}/{}_{}_2018.root".format(path,file_,channel))
        # chainW = ROOT.TChain("genweights")
        # chainW.Add(os.path.join(path, "", "*.root"))
        # chain.AddFriend("genweights")
    
        # chain.Draw("n_pu>>pileup_{}".format(file_),"wt")
        chain.Draw("n_pu>>pileup","wt")
    
        if not os.path.exists(os.path.join(path, "pileupDistribution")):
            os.makedirs(os.path.join(path, "pileupDistribution"))
        new_file = ROOT.TFile.Open(os.path.join(path, "pileupDistribution/pileup_2017_{}.root".format(file_)), "RECREATE")
        # new_file = ROOT.TFile.Open(os.path.join(path, "pileupDistribution/pileup_2018_{}.root".format(file_)), "RECREATE")
    
        # if not os.path.exists(os.path.join(path, "pileupDistribution_forHTT")):
        #     os.makedirs(os.path.join(path, "pileupDistribution_forHTT"))
        # new_file = ROOT.TFile.Open(os.path.join(path, "pileupDistribution_forHTT/pileup_2017_{}.root".format(file_)), "RECREATE")
        hist.SetDirectory(new_file)
        new_file.Write()
        new_file.Close()
        del hist
        del new_file
        chain.Reset()
