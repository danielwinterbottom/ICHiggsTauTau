import FWCore.ParameterSet.Config as cms
process = cms.Process("MAIN")
import sys

################################################################
# Read Options
################################################################
import FWCore.ParameterSet.VarParsing as parser
opts = parser.VarParsing ('analysis')
opts.register('file', 'root://xrootd.unl.edu//store/group/cmst3/group/htautau/gen-prod-260917/OldAcc_1_M-700_TuneCUETP8M1_13TeV-amcatnlo-pythia8/gen-prod-260917/170926_134002/0000/gen_1.root', parser.VarParsing.multiplicity.singleton,               

parser.VarParsing.varType.string, "input file")
opts.register('globalTag', '80X_mcRun2_asymptotic_2016_TrancheIV_v7', parser.VarParsing.multiplicity.singleton,
    parser.VarParsing.varType.string, "global tag")
opts.register('LHEWeights', False, parser.VarParsing.multiplicity.singleton,
    parser.VarParsing.varType.bool, "Produce LHE weights for sample")
opts.register('LHETag', 'externalLHEProducer', parser.VarParsing.multiplicity.singleton,
    parser.VarParsing.varType.string, "Input tag for LHE weights")

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
  fileName = cms.string("EventTree.root"),
  closeFileFast = cms.untracked.bool(True)
)

################################################################
# Message Logging, summary, and number of events
################################################################
process.maxEvents = cms.untracked.PSet(
  input = cms.untracked.int32(100)
)

process.MessageLogger.cerr.FwkReport.reportEvery = 50

process.options   = cms.untracked.PSet(
  wantSummary = cms.untracked.bool(True)
)

################################################################
# Input files and global tags
################################################################
process.load("CondCore.CondDB.CondDB_cfi")
from CondCore.CondDB.CondDB_cfi import *

process.source = cms.Source("PoolSource", fileNames = cms.untracked.vstring(infile))
process.GlobalTag.globaltag = cms.string(tag)

process.options   = cms.untracked.PSet(
  FailPath=cms.untracked.vstring("FileReadError")
)

import UserCode.ICHiggsTauTau.default_producers_cfi as producers
import UserCode.ICHiggsTauTau.default_selectors_cfi as selectors

process.load("PhysicsTools.PatAlgos.patSequences_cff")


process.icGenVertexProducer = producers.icGenVertexProducer.clone(
  input = cms.InputTag("genParticles")
)

process.icVertexSequence = cms.Sequence(
  process.icGenVertexProducer
)

################################################################
# Simulation only: GenParticles, GenJets, PileupInfo
################################################################
process.icGenSequence = cms.Sequence()
 
process.icGenParticleProducer = producers.icGenParticleProducer.clone(
  input   = cms.InputTag("genParticles"),
  includeMothers = cms.bool(True),
  includeDaughters = cms.bool(True),
  includeStatusFlags = cms.bool(True)
)

process.selectedGenJets = cms.EDFilter("GenJetRefSelector",
  src = cms.InputTag("ak4GenJets"),
  cut = cms.string("pt > 10.0")
)

process.icGenJetProducer = producers.icGenJetProducer.clone(
  branch  = cms.string("genJets"),
  input   = cms.InputTag("selectedGenJets"),
)

process.icGenSequence += (
  process.icGenParticleProducer+
  process.selectedGenJets+
  process.icGenJetProducer
)

#################################################################
## EventInfo
#################################################################
if opts.LHETag: lheTag = opts.LHETag
else: lheTag = 'externalLHEProducer'

process.icEventInfoProducer = producers.icEventInfoProducer.clone(
  includeLHEWeights   = cms.bool(doLHEWeights),
  lheProducer         = cms.InputTag(lheTag),
)

process.icEventInfoSequence = cms.Sequence(
  process.icEventInfoProducer
)
  
################################################################
# Event
################################################################
process.icEventProducer = producers.icEventProducer.clone()

process.p = cms.Path(
  process.icVertexSequence+
  process.icGenSequence+
  process.icEventInfoSequence+
  process.icEventProducer
)

process.schedule = cms.Schedule(process.p)

