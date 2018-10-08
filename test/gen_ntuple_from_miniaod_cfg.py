import FWCore.ParameterSet.Config as cms

process = cms.Process("MAIN")

################################################################
# Standard setup
################################################################
process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.load("Configuration.Geometry.GeometryRecoDB_cff")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_condDBv2_cff")
process.load("Configuration.StandardSequences.MagneticField_cff")


process.TFileService = cms.Service("TFileService",
        fileName = cms.string("EventTree.root"),
            closeFileFast = cms.untracked.bool(True)
            )


process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(100)
)

process.MessageLogger.cerr.FwkReport.reportEvery = 50

# Input source
process.source = cms.Source("PoolSource",
        fileNames = cms.untracked.vstring('/store/mc/RunIIFall15MiniAODv2/GluGluToHToTauTau_M125_13TeV_amcatnloFXFX_pythia8/MINIAODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/60000/001268DD-CAEB-E511-AC7F-842B2B5C2299.root'),
    secondaryFileNames = cms.untracked.vstring()
)

process.options = cms.untracked.PSet(
    wantSummary = cms.untracked.bool(False)
)


# Other statements
from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, '80X_mcRun2_asymptotic_2016_TrancheIV_v8', '')

import UserCode.ICHiggsTauTau.default_producers_cfi as producers

process.icGenParticleProducer = producers.icGenParticleProducer.clone(
    input               = cms.InputTag('prunedGenParticles', '', 'PAT'),
    includeMothers      = cms.bool(True),
    includeDaughters    = cms.bool(True),
    includeStatusFlags  = cms.bool(True)
    )

from PhysicsTools.JetMCAlgos.HadronAndPartonSelector_cfi import selectedHadronsAndPartons
process.selectedHadronsAndPartons = selectedHadronsAndPartons.clone(
        particles = cms.InputTag('prunedGenParticles', '', 'PAT') 
        )

from PhysicsTools.JetMCAlgos.AK4PFJetsMCFlavourInfos_cfi import ak4JetFlavourInfos
process.genJetFlavourInfos = ak4JetFlavourInfos.clone(
        jets = cms.InputTag("slimmedGenJets") 
        )

process.MessageLogger.categories += cms.vstring('JetPtMismatch', 'MissingJetConstituent', 'JetPtMismatchAtLowPt')
process.MessageLogger.cerr.JetPtMismatch = cms.untracked.PSet(limit = cms.untracked.int32(0))
process.MessageLogger.cerr.MissingJetConstituent = cms.untracked.PSet(limit = cms.untracked.int32(0))
process.MessageLogger.cerr.JetPtMismatchAtLowPt = cms.untracked.PSet(limit = cms.untracked.int32(0))

process.selectedGenJets = cms.EDFilter("GenJetRefSelector",
    src = cms.InputTag("slimmedGenJets"),
    cut = cms.string("pt > 10.0")
    )

process.icGenJetFlavourCalculator = cms.EDProducer('ICJetFlavourCalculator',
        input       = cms.InputTag("slimmedGenJets"),
        flavourMap  = cms.InputTag("genJetFlavourInfos")
        )

process.icGenJetProducer = producers.icGenJetProducer.clone(
    branch              = cms.string("genJets"),
    input               = cms.InputTag("selectedGenJets"),
    inputGenParticles   = cms.InputTag("genParticles"),
    requestGenParticles = cms.bool(False),
    includeFlavourInfo  = cms.bool(True),
    inputFlavourInfo    = cms.InputTag("icGenJetFlavourCalculator"),
    isSlimmed           = cms.bool(True)
    )

process.icEventInfoProducer = producers.icEventInfoProducer.clone(
    lheProducer         = cms.InputTag("source"),
    includeLHEWeights   = cms.bool(False)
    )

process.icEventProducer = producers.icEventProducer.clone()

process.p = cms.Path(
   process.selectedHadronsAndPartons+
   process.genJetFlavourInfos+
   process.icGenJetFlavourCalculator+
   process.icGenParticleProducer+
   process.selectedGenJets+
   process.icGenJetProducer+
   process.icEventInfoProducer+
   process.icEventProducer
)

# Schedule definition
process.schedule = cms.Schedule(process.p)

