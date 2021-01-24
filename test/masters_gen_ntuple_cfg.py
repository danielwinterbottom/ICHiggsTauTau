import FWCore.ParameterSet.Config as cms
process = cms.Process("MAIN")
import sys

################################################################
# Read Options
################################################################
import FWCore.ParameterSet.VarParsing as parser
opts = parser.VarParsing ('analysis')

opts.register('file',
'root://xrootd.unl.edu//store/mc/RunIIAutumn18MiniAOD/GluGluHToTauTauUncorrelatedDecay_Filtered_M125_TuneCP5_13TeV-powheg-pythia8/MINIAODSIM/102X_upgrade2018_realistic_v15-v1/230000/3A2A869C-1E71-1442-B982-9AAA27A50EE8.root',
parser.VarParsing.multiplicity.singleton,
parser.VarParsing.varType.string, "input file")
opts.register('globalTag', '102X_upgrade2018_realistic_v20', parser.VarParsing.multiplicity.singleton,
    parser.VarParsing.varType.string, "global tag")
opts.register('LHEWeights', False, parser.VarParsing.multiplicity.singleton,
    parser.VarParsing.varType.bool, "Produce LHE weights for sample")
opts.register('LHETag', 'externalLHEProducer', parser.VarParsing.multiplicity.singleton,
    parser.VarParsing.varType.string, "Input tag for LHE weights")
opts.register('tauSpinner', True, parser.VarParsing.multiplicity.singleton,
    parser.VarParsing.varType.bool, "Compute weights using tauspinner")

opts.parseArguments()
infile      = opts.file
if not infile: infile = "file:/tmp/file.root"
tag         = opts.globalTag
doLHEWeights = opts.LHEWeights

print 'globalTag   : '+str(tag)


################################################################
# Standard setup
################################################################
process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.load("Configuration.Geometry.GeometryRecoDB_cff")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_condDBv2_cff")
process.load("Configuration.StandardSequences.MagneticField_cff")

process.TFileService = cms.Service("TFileService",
    fileName = cms.string("EventTree22.root"),
    closeFileFast = cms.untracked.bool(True)
)

################################################################
# Message Logging, summary, and number of events
################################################################
process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(10000)
)

process.MessageLogger.cerr.FwkReport.reportEvery = 1


################################################################
# Input files and global tags
################################################################
process.load("CondCore.CondDB.CondDB_cfi")
from CondCore.CondDB.CondDB_cfi import *

process.source = cms.Source("PoolSource", fileNames = cms.untracked.vstring(infile))
process.GlobalTag.globaltag = cms.string(tag)

process.options   = cms.untracked.PSet(
    FailPath=cms.untracked.vstring("FileReadError"),
    wantSummary = cms.untracked.bool(True),
    # numberOfThreads = cms.untracked.uint32(4)
)

import UserCode.ICHiggsTauTau.default_producers_cfi as producers
import UserCode.ICHiggsTauTau.default_selectors_cfi as selectors

process.load("PhysicsTools.PatAlgos.patSequences_cff")


################################################################
# Simulation only: Vertices
################################################################

process.icGenVertexProducer = producers.icGenVertexProducer.clone(
    input = cms.InputTag("prunedGenParticles")
)


process.icVertexSequence = cms.Sequence(
  process.icGenVertexProducer
)


################################################################
# Simulation only: GenParticles, GenJets, PileupInfo
################################################################

process.icGenSequence = cms.Sequence()

process.load("SimGeneral.HepPDTESSource.pythiapdt_cfi")

process.icGenParticleProducer = producers.icGenParticleProducer.clone(
    input   = cms.InputTag("prunedGenParticles","","PAT"),
    includeMothers = cms.bool(True),
    includeDaughters = cms.bool(True),
    includeStatusFlags = cms.bool(True)
)

process.icGenParticleProducerFromLHEParticles = producers.icGenParticleFromLHEParticlesProducer.clone()


process.load("RecoJets.Configuration.GenJetParticles_cff")
process.genParticlesForJets.ignoreParticleIDs = cms.vuint32(
    1000022, 2000012, 2000014,
    2000016, 1000039, 5000039,
    4000012, 9900012, 9900014,
    9900016, 39, 12, 14, 16
)
process.genParticlesForJets.src = cms.InputTag("packedGenParticles")
process.load("RecoJets.JetProducers.ak4GenJets_cfi")
process.ak4GenJetsNoNuBSM  =  process.ak4GenJets.clone()
process.ak4GenJetsNoNuBSM.src=cms.InputTag("packedGenParticles")

process.selectedGenJets = cms.EDFilter("GenJetRefSelector",
    src = cms.InputTag("ak4GenJetsNoNuBSM"),
    cut = cms.string("pt > 10.0")
)

process.icGenJetProducer = producers.icGenJetProducer.clone(
    branch  = cms.string("genJetsReclustered"),
    input   = cms.InputTag("selectedGenJets"),
    inputGenParticles = cms.InputTag("prunedGenParticles","","PAT"),
    requestGenParticles = cms.bool(False),
    isSlimmed  = cms.bool(True)
)

process.icGenJetProducerFromSlimmed = producers.icGenJetProducer.clone(
    branch = cms.string("genJets"),
    input = cms.InputTag("slimmedGenJets"),
    inputGenParticles=cms.InputTag("genParticles"),
    requestGenParticles = cms.bool(False),
    isSlimmed = cms.bool(True)
)

process.icPileupInfoProducer = producers.icPileupInfoProducer.clone()
process.icPileupInfoProducer.input=cms.InputTag("slimmedAddPileupInfo")


process.icGenSequence += (
    process.icGenParticleProducer+
    process.ak4GenJetsNoNuBSM+
    process.selectedGenJets+
    process.icGenJetProducer+
    process.icGenJetProducerFromSlimmed
)
process.icGenSequence += (process.icPileupInfoProducer)
process.icGenSequence += (
    process.icGenParticleProducerFromLHEParticles
)

################################################################
# EventInfo
################################################################

if opts.LHETag: lheTag = opts.LHETag
else: lheTag = 'externalLHEProducer'

data_type = "PAT"

process.icEventInfoProducer = producers.icEventInfoProducer.clone(
    includeJetRho       = cms.bool(True),
    includeLHEWeights   = cms.bool(doLHEWeights),
    includeGenWeights   = cms.bool(doLHEWeights),
    includeHT           = cms.bool(False),
    lheProducer         = cms.InputTag(lheTag),
)

process.icEventInfoSequence = cms.Sequence(
    process.icEventInfoProducer
)

################################################################
# TauSpinner
################################################################

process.icTauSpinnerProducer = cms.EDProducer("ICTauSpinnerProducer",
    branch                  = cms.string("tauspinner"),
    input                   = cms.InputTag("prunedGenParticles"),
    theta                   = cms.string("0,0.25,0.5")
)

if opts.tauSpinner:
    print('\nUsing Tauspinner\n')
    process.icTauSpinnerSequence = cms.Sequence(
        process.icTauSpinnerProducer
    )
else: process.icTauSpinnerSequence = cms.Sequence()



################################################################
# Event
################################################################
process.icEventProducer = producers.icEventProducer.clone()


process.p = cms.Path(
    process.icVertexSequence+
    process.icGenSequence+
    process.icTauSpinnerSequence+
    process.icEventInfoSequence+
    process.icEventProducer
)


process.schedule = cms.Schedule(process.p)

