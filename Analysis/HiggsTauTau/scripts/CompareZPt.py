import ROOT
import os
import glob
import json
from UserCode.ICHiggsTauTau.analysis import *
from UserCode.ICHiggsTauTau.uncertainties import ufloat
from optparse import OptionParser
import argparse
import ConfigParser
import UserCode.ICHiggsTauTau.plotting as plotting
from collections import OrderedDict
import copy

file_prefix="/vols/cms/dw515/Offline/output/MSSM/gen_studies/"
file1 = ROOT.TFile(file_prefix+"test.root")

nlo        = file1.Get("nlo")     
nlo_b      = file1.Get("nlo_b")   
lo         = file1.Get("lo")      
lo_b       = file1.Get("lo_b")    
lo_wtzpt   = file1.Get("lo_wtzpt")
lo_b_wtzpt = file1.Get("lo_b_wtzpt")

hists = [nlo, lo, lo_wtzpt]

plotting.CompareHists(hists,
                      ["NLO", "LO", "LO + ZpT re-weighting"],
                      "inclusive",
                      True,
                      False,
                      False,
                      "0.7,1.3",
                      False,
                      4000,
                      0,
                      False,
                      4000,
                      0,
                      "Z p_{T} [GeV]",
                      "Normalized Entries",
                      0,
                      True,
                      "Zpt_lo_nlo_comp_inclusive")
    

hists = [nlo_b, lo_b, lo_b_wtzpt]

plotting.CompareHists(hists,
                      ["NLO", "LO", "LO + ZpT re-weighting"],
                      "N_{bquarks}>0",
                      True,
                      False,
                      False,
                      "0.7,1.3",
                      False,
                      4000,
                      0,
                      False,
                      4000,
                      0,
                      "Z p_{T} [GeV]",
                      "Normalized Entries",
                      0,
                      True,
                      "Zpt_lo_nlo_comp_bquarks")


 

