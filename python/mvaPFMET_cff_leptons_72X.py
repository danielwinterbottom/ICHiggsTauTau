import FWCore.ParameterSet.Config as cms

from JetMETCorrections.Configuration.JetCorrectionServicesAllAlgos_cff  import *
from JetMETCorrections.Configuration.DefaultJEC_cff             import *
from RecoMET.METPUSubtraction.mvaPFMET_cff    import *
#del hpsPFTauDiscriminationByDecayModeFinding
from RecoJets.JetProducers.PileupJetIDParams_cfi                import JetIdParams

mvaMetPairs = cms.EDProducer("MVAMETPairProducer",
    srcCorrJets = cms.InputTag('calibratedAK4PFJetsForPFMVAMEt'),
    srcUncorrJets = cms.InputTag('ak4PFJets'),
    srcPFCandidates = cms.InputTag('particleFlow'),
    srcVertices = cms.InputTag('offlinePrimaryVertices'),
    srcLeg1 = cms.InputTag('dummyCollection'),
    srcLeg2 = cms.InputTag('dummyCollection'),
    leg1Pt = cms.double(10.0),
    leg1Eta = cms.double(2.6),
    leg2Pt = cms.double(10.0),
    leg2Eta = cms.double(2.6),
    minDeltaR = cms.double(0.3),
#    srcRho = cms.InputTag('kt6PFJets','rho'),
    srcRho = cms.InputTag("fixedGridRhoFastjetAll"),
    globalThreshold = cms.double(-1.),#pfMet.globalThreshold,
    minCorrJetPt = cms.double(-1.),
    inputFileNames = cms.PSet(
        U     = cms.FileInPath('RecoMET/METPUSubtraction/data/gbrmet_7_2_X_MINIAOD_BX25PU20_Mar2015.root'),
        DPhi  = cms.FileInPath('RecoMET/METPUSubtraction/data/gbrphi_7_2_X_MINIAOD_BX25PU20_Mar2015.root'),
        CovU1 = cms.FileInPath('RecoMET/METPUSubtraction/data/gbru1cov_7_2_X_MINIAOD_BX25PU20_Mar2015.root'),
        CovU2 = cms.FileInPath('RecoMET/METPUSubtraction/data/gbru2cov_7_2_X_MINIAOD_BX25PU20_Mar2015.root')
    ),
    inputRecords = cms.PSet(
        U     = cms.string("RecoilCor"),
        DPhi  = cms.string("PhiCor"), 
        CovU1 = cms.string("CovU1"),
        CovU2 = cms.string("CovU2")
    ),
    loadMVAfromDB = cms.bool(False),
    corrector = cms.string("ak4PFL1Fastjet"),
    useType1  = cms.bool(False),
    useOld42  = cms.bool(False),
    dZcut = cms.double(0.1),
    impactParTkThreshold = cms.double(0.),
    tmvaWeights = cms.string("RecoJets/JetProducers/data/TMVAClassificationCategory_JetID_MET_53X_Dec2012.weights.xml.gz"),
    tmvaMethod = cms.string("JetID"),
    version = cms.int32(-1),
    cutBased = cms.bool(False),
    tmvaVariables = cms.vstring(
        "nvtx",
        "jetPt",
        "jetEta",
        "jetPhi",
        "dZ",
        "beta",
        "betaStar",
        "nCharged",
        "nNeutrals",
        "dR2Mean",
        "ptD",
        "frac01",
        "frac02",
        "frac03",
        "frac04",
        "frac05",
    ),
    tmvaSpectators = cms.vstring(),
    JetIdParams = JetIdParams,
    label = cms.string("full"),
    verbosity = cms.int32(0)
)

