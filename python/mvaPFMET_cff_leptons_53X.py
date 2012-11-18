
import FWCore.ParameterSet.Config as cms

#from RecoMET.METProducers.PFMET_cfi import pfMet
from JetMETCorrections.Configuration.JetCorrectionServices_cff import *
from JetMETCorrections.Configuration.DefaultJEC_cff  import *
#from JetMETCorrections.METPUSubtraction.mvaPFMET_leptons           import *
from JetMETCorrections.METPUSubtraction.mvaPFMET_leptons_cff       import *
from RecoJets.JetProducers.PileupJetIDParams_cfi                        import JetIdParams

mvaMetPairs = cms.EDProducer("MVAMETPairProducer",
    srcCorrJets = cms.InputTag('calibratedAK5PFJetsForPFMEtMVA'),
    srcUncorrJets = cms.InputTag('ak5PFJets'),
    srcPFCandidates = cms.InputTag('particleFlow'),
    srcVertices = cms.InputTag('offlinePrimaryVertices'),
    srcLeg1 = cms.InputTag('dummyCollection'),
    srcLeg2 = cms.InputTag('dummyCollection'),
    leg1Pt = cms.double(10.0),
    leg1Eta = cms.double(2.6),
    leg2Pt = cms.double(10.0),
    leg2Eta = cms.double(2.6),
    minDeltaR = cms.double(0.3),
    srcRho = cms.InputTag('kt6PFJets','rho'),
    globalThreshold = cms.double(-1.),#pfMet.globalThreshold,
    minCorrJetPt = cms.double(-1.),
    inputFileNames = cms.PSet(
        U     = cms.FileInPath('JetMETCorrections/METPUSubtraction/data/gbrmet_53.root'),
        DPhi  = cms.FileInPath('JetMETCorrections/METPUSubtraction/data/gbrmetphi_53.root'),
        CovU1 = cms.FileInPath('JetMETCorrections/METPUSubtraction/data/gbru1cov_53.root'),
        CovU2 = cms.FileInPath('JetMETCorrections/METPUSubtraction/data/gbru2cov_53.root')
    ),
    corrector = cms.string("ak5PFL1Fastjet"),
    useType1  = cms.bool(False),
    dZcut = cms.double(0.1),
    impactParTkThreshold = cms.double(0.),
    tmvaWeights = cms.string("RecoJets/JetProducers/data/mva_JetID_v1.weights.xml"),
    tmvaMethod = cms.string("JetID"),
    version = cms.int32(-1),
    cutBased = cms.bool(False),                      
    tmvaVariables = cms.vstring(
        "nvtx",
        "jetPt",
        "jetEta",
        "jetPhi",
        "dZ",
        "d0",
        "beta",
        "betaStar",
        "nCharged",
        "nNeutrals",
        "dRMean",
        "frac01",
        "frac02",
        "frac03",
        "frac04",
        "frac05",
    ),
    tmvaSpectators = cms.vstring(),
    JetIdParams = JetIdParams,
    verbosity = cms.int32(0)
)

pfMEtAllPairsMVAsequence  = cms.Sequence(
    (isomuonseq+isotauseq+isoelectronseq)*
    calibratedAK5PFJetsForPFMEtMVA*
    pfMEtMVA
)


