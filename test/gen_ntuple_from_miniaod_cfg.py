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
        fileNames = cms.untracked.vstring('/store/mc/RunIISummer16MiniAODv2/SUSYGluGluToBBHToTauTau_M-3200_TuneCUETP8M1_13TeV-pythia8/MINIAODSIM/PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/70000/BCDBCE42-3BC9-E611-B377-00259073E4C4.root'),
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

