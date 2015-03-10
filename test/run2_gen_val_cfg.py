import FWCore.ParameterSet.Config as cms
process = cms.Process("MAIN")
import sys

################################################################
# Read Options
################################################################
import FWCore.ParameterSet.VarParsing as parser
opts = parser.VarParsing ('analysis')
opts.register('file', 0, parser.VarParsing.multiplicity.singleton,
    parser.VarParsing.varType.string, "input file")
opts.register('globalTag', 0, parser.VarParsing.multiplicity.singleton,
    parser.VarParsing.varType.string, "global tag")
opts.parseArguments()
infile      = opts.file
if not infile: infile = "file:/tmp/file.root"
tag         = opts.globalTag

print 'globalTag   : '+tag

################################################################
# Standard setup
################################################################
process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.load("Configuration.Geometry.GeometryIdeal_cff")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.load("Configuration.StandardSequences.MagneticField_cff")

process.TFileService = cms.Service("TFileService",
  fileName = cms.string("EventTree.root"),
  closeFileFast = cms.untracked.bool(True)
)

################################################################
# Message Logging, summary, and number of events
################################################################
process.maxEvents = cms.untracked.PSet(
  input = cms.untracked.int32(-1)
)

process.MessageLogger.cerr.FwkReport.reportEvery = 50

process.options   = cms.untracked.PSet(
  wantSummary = cms.untracked.bool(True)
)

################################################################
# Input files and global tags
################################################################
process.source = cms.Source("PoolSource", fileNames = cms.untracked.vstring(infile))
process.GlobalTag.globaltag = cms.string(tag)

# file=root://eoscms//eos/cms/store/cmst3/user/ytakahas/CMG/Mc_Validation/HIG-RunIIWinter15GS-00001.root globalTag=MCRUN2_71_V1::All
import UserCode.ICHiggsTauTau.default_producers_cfi as producers
import UserCode.ICHiggsTauTau.default_selectors_cfi as selectors

################################################################
# Simulation only: GenParticles, GenJets, PileupInfo
################################################################
process.icGenSequence = cms.Sequence()

process.icGenParticleProducer = producers.icGenParticleProducer.clone(
  input   = cms.InputTag("genParticles"),
  includeMothers = cms.bool(True),
  includeDaughters = cms.bool(True)
)

process.load("RecoJets.Configuration.GenJetParticles_cff")
process.genParticlesForJets.ignoreParticleIDs = cms.vuint32(
  1000022, 2000012, 2000014,
  2000016, 1000039, 5000039,
  4000012, 9900012, 9900014,
  9900016, 39, 12, 14, 16
)
process.load("RecoJets.JetProducers.ak5GenJets_cfi")
process.ak5GenJetsNoNuBSM  =  process.ak5GenJets.clone()

process.selectedGenJets = cms.EDFilter("GenJetRefSelector",
  src = cms.InputTag("ak5GenJetsNoNuBSM"),
  cut = cms.string("pt > 10.0")
)

process.icGenJetProducer = producers.icGenJetProducer.clone(
  branch  = cms.string("genJets"),
  input   = cms.InputTag("selectedGenJets"),
  inputGenParticles = cms.InputTag("genParticles"),
  requestGenParticles = cms.bool(False)
)


process.icGenSequence += (
    process.icGenParticleProducer+
    process.genParticlesForJets+
    process.ak5GenJetsNoNuBSM+
    process.selectedGenJets+
    process.icGenJetProducer
    )

################################################################
# EventInfo
################################################################
process.icEventInfoProducer = producers.icEventInfoProducer.clone(
  includeJetRho       = cms.bool(False),
  inputJetRho         = cms.InputTag("kt6PFJets", "rho"),
  includeLeptonRho    = cms.bool(False),
  inputLeptonRho      = cms.InputTag("kt6PFJets", "rho"),
  includeVertexCount  = cms.bool(False),
  inputVertices       = cms.InputTag("selectedVertices"),
  includeCSCFilter    = cms.bool(False),
  inputCSCFilter      = cms.InputTag("BeamHaloSummary"),
)

process.icEventInfoSequence = cms.Sequence(
  process.icEventInfoProducer
)


################################################################
# Event
################################################################
process.icEventProducer = producers.icEventProducer.clone()

"""
process.load("SimGeneral.HepPDTESSource.pythiapdt_cfi")
process.printTree = cms.EDAnalyzer("ParticleListDrawer",
    maxEventsToPrint = cms.untracked.int32(1),
    printVertex = cms.untracked.bool(False),
    src = cms.InputTag("genParticles")
    )
"""

process.p = cms.Path(
  #process.printTree+
  process.icGenSequence+
  process.icEventInfoSequence+
  process.icEventProducer
)

process.schedule = cms.Schedule(process.p)

#print process.dumpPython()
