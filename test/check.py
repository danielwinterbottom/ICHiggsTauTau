# import ROOT in batch mode
import sys
oldargv = sys.argv[:]
sys.argv = [ '-b-' ]
import ROOT
ROOT.gROOT.SetBatch(True)
sys.argv = oldargv

ROOT.gSystem.Load("libFWCoreFWLite.so");
ROOT.gSystem.Load("libDataFormatsFWLite.so");
ROOT.FWLiteEnabler.enable()

from DataFormats.FWLite import Handle, Events

# pfHandle  = Handle ('std::vector<pat::PackedCandidate>')
# pfLabel = ("packedPFCandidates","","PAT")

tauHandle  = Handle ('std::vector<pat::Tau>')
tauLabel = ("slimmedTaus","","PAT")

events = Events("root://xrootd.unl.edu//store/mc/RunIIFall17MiniAODv2/VBFHToTauTau_M125_13TeV_powheg_pythia8/MINIAODSIM/PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/00000/2EE992B1-F942-E811-8F11-0CC47A4C8E8A.root")


for iev,event in enumerate(events):
    print "event: {}".format(event.eventAuxiliary().event())
    try:
        # event.getByLabel(pfLabel, pfHandle)
        event.getByLabel(tauLabel, tauHandle)

        # pfCands = pfHandle.product()
        # pfCands = filter(lambda x: x.pt()>10., pfCands)

        tauCands = tauHandle.product()
        tauCands = filter(lambda x: x.pt()>5., tauCands)

        # for iJet, jet in enumerate(pfCands):
        #     print dir(jet)

        for iTau, tau in enumerate(tauCands):
            for iCand, cand in enumerate(tau.signalChargedHadrCands()):
                print cand.pt()

    except RuntimeError:
        continue
