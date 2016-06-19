import FWCore.ParameterSet.Config as cms
process = cms.Process("MAIN")
import sys

################################################################
# Read Options
################################################################
import FWCore.ParameterSet.VarParsing as parser
opts = parser.VarParsing('analysis')
opts.register('file', '',
    parser.VarParsing.multiplicity.singleton,
    parser.VarParsing.varType.string, 'input file')
opts.register('globalTag', '80X_dataRun2_Prompt_v8', parser.VarParsing.multiplicity.singleton,
    parser.VarParsing.varType.string, 'global tag')
opts.register('isData', 1, parser.VarParsing.multiplicity.singleton,
    parser.VarParsing.varType.int, 'Process as data')
opts.register('release', '80XMINIAOD', parser.VarParsing.multiplicity.singleton,
    parser.VarParsing.varType.string, 'Release label')

opts.parseArguments()
infile = opts.file
isData = opts.isData
tag  = opts.globalTag
release = opts.release

if not release in ['80XMINIAOD']:
  print 'Release not recognised, exiting!'
  sys.exit(1)
print 'release     : '+release
print 'isData      : '+str(isData)
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
  input = cms.untracked.int32(3000)
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

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(infile)
    )

process.GlobalTag.globaltag = cms.string(tag)

import UserCode.ICHiggsTauTau.default_producers_cfi as producers
import UserCode.ICHiggsTauTau.default_selectors_cfi as selectors

process.load("PhysicsTools.PatAlgos.patSequences_cff")

################################################################
# Event
################################################################

process.icTriggerPathProducer = cms.EDProducer('ICTriggerPathProducer',
    branch = cms.string("triggerPaths"),
    input  = cms.InputTag("TriggerResults", "", "HLT"),
    includeAcceptedOnly = cms.bool(False),
    saveStrings = cms.bool(False),
    splitVersion = cms.bool(True),
    inputIsStandAlone = cms.bool(True),
    inputPrescales = cms.InputTag("patTrigger", "", "RECO")
    )

process.icEventProducer = producers.icEventProducer.clone()
process.icHashTreeProducer = cms.EDProducer('ICHashTreeProducer')

process.p = cms.Path(
    process.icTriggerPathProducer+
    process.icEventProducer+
    process.icHashTreeProducer
)

process.schedule = cms.Schedule(process.p)
