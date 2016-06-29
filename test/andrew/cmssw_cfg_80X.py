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

process.MessageLogger.cerr.FwkReport.reportEvery = 500

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
    branch                  = cms.string("triggerPaths"),
    input                   = cms.InputTag("TriggerResults", "", 'HLT' if isData else 'HLT2'),
    includeAcceptedOnly     = cms.bool(False),
    saveStrings             = cms.bool(False),
    splitVersion            = cms.bool(True),
    inputIsStandAlone       = cms.bool(True),
    inputPrescales          = cms.InputTag("patTrigger", "", 'RECO' if isData else 'PAT')
    )

process.icEventInfoProducer = producers.icEventInfoProducer.clone(
    includeJetRho       = cms.bool(False),
    includeHT           = cms.bool(False),
    lheProducer         = cms.InputTag("externalLHEProducer"),
    inputJetRho         = cms.InputTag("fixedGridRhoFastjetAll"),
    includeLeptonRho    = cms.bool(False),
    inputLeptonRho      = cms.InputTag("fixedGridRhoFastjetAll"),
    includeVertexCount  = cms.bool(True),
    inputVertices       = cms.InputTag("offlineSlimmedPrimaryVertices"),
    includeCSCFilter    = cms.bool(False),
    inputCSCFilter      = cms.InputTag("BeamHaloSummary")
    )

process.icTriggerObjectSequence = cms.Sequence()

paths_2016_MC = [
    'HLT_IsoMu18_v',
    'HLT_IsoMu20_v',
    'HLT_IsoMu22_v',
    'HLT_IsoMu22_eta2p1_v',
    'HLT_IsoMu24_v',
    'HLT_IsoMu27_v',
    'HLT_IsoTkMu18_v',
    'HLT_IsoTkMu20_v',
    'HLT_IsoTkMu22_eta2p1_v',
    'HLT_IsoTkMu22_v',
    'HLT_IsoTkMu24_v',
    'HLT_IsoTkMu27_v',
    'HLT_IsoMu17_eta2p1_LooseIsoPFTau20_SingleL1_v',
    'HLT_IsoMu17_eta2p1_LooseIsoPFTau20_v',
    'HLT_IsoMu19_eta2p1_LooseIsoPFTau20_SingleL1_v',
    'HLT_IsoMu19_eta2p1_LooseIsoPFTau20_v',
    'HLT_IsoMu21_eta2p1_LooseIsoPFTau20_SingleL1_v',
    'HLT_Ele23_WPLoose_Gsf_v',
    'HLT_Ele24_eta2p1_WPLoose_Gsf_v',
    'HLT_Ele25_WPTight_Gsf_v',
    'HLT_Ele25_eta2p1_WPLoose_Gsf_v',
    'HLT_Ele25_eta2p1_WPTight_Gsf_v',
    'HLT_Ele27_WPLoose_Gsf_v',
    'HLT_Ele27_WPTight_Gsf_v',
    'HLT_Ele27_eta2p1_WPLoose_Gsf_v',
    'HLT_Ele27_eta2p1_WPTight_Gsf_v',
    'HLT_Ele32_eta2p1_WPTight_Gsf_v',
    'HLT_Ele22_eta2p1_WPLoose_Gsf_LooseIsoPFTau20_SingleL1_v',
    'HLT_Ele24_eta2p1_WPLoose_Gsf_LooseIsoPFTau20_SingleL1_v',
    'HLT_Ele24_eta2p1_WPLoose_Gsf_LooseIsoPFTau20_v',
    'HLT_Ele27_eta2p1_WPLoose_Gsf_LooseIsoPFTau20_SingleL1_v',
    'HLT_Ele32_eta2p1_WPLoose_Gsf_LooseIsoPFTau20_SingleL1_v',
    'HLT_DoubleMediumIsoPFTau32_Trk1_eta2p1_Reg_v',
    'HLT_DoubleMediumIsoPFTau35_Trk1_eta2p1_Reg_v',
    'HLT_DoubleMediumIsoPFTau40_Trk1_eta2p1_Reg_v',
    'HLT_Mu8_TrkIsoVVL_Ele17_CaloIdL_TrackIdL_IsoVL_v',
    'HLT_Mu8_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL_v',
    'HLT_Mu17_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL_v',
    'HLT_Mu23_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL_v',
    'HLT_Mu23_TrkIsoVVL_Ele8_CaloIdL_TrackIdL_IsoVL_v'
]

for path in paths_2016_MC:
    shortname = path[4:-2]  # drop the HLT_ and _v parts
    setattr(process, 'ic_%s_ObjectProducer' % shortname, producers.icTriggerObjectProducer.clone(
        input               = cms.InputTag("selectedPatTrigger"),
        inputTriggerResults = cms.InputTag("TriggerResults", "", "HLT2"),
        branch              = cms.string("triggerObjects_%s" % shortname),
        hltPath             = cms.string(path),
        inputIsStandAlone   = cms.bool(True),
        storeOnlyIfFired    = cms.bool(False)

    ))
    process.icTriggerObjectSequence += cms.Sequence(getattr(process, 'ic_%s_ObjectProducer' % shortname))


process.icEventProducer = producers.icEventProducer.clone()
process.icHashTreeProducer = cms.EDProducer('ICHashTreeProducer')

process.p = cms.Path(
    process.icTriggerPathProducer+
    process.icTriggerObjectSequence+
    process.icEventInfoProducer+
    process.icEventProducer+
    process.icHashTreeProducer
)

process.schedule = cms.Schedule(process.p)
