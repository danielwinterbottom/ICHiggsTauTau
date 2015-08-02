import FWCore.ParameterSet.Config as cms
process = cms.Process("MAIN")
import sys

################################################################
# Read Options
################################################################
import FWCore.ParameterSet.VarParsing as parser
opts = parser.VarParsing ('analysis')
opts.register('file', 'file:/tmp/file.root', parser.VarParsing.multiplicity.singleton,
    parser.VarParsing.varType.string, "input file")
opts.register('globalTag', 'PHYS14_25_V1::All', parser.VarParsing.multiplicity.singleton,
    parser.VarParsing.varType.string, "global tag")
opts.register('isData', 0, parser.VarParsing.multiplicity.singleton,
    parser.VarParsing.varType.int, "Process as data?")
opts.register('release', '74XMINIAOD', parser.VarParsing.multiplicity.singleton,
    parser.VarParsing.varType.string, "Release label")
opts.register('doGenWeights', 0, parser.VarParsing.multiplicity.singleton,
    parser.VarParsing.varType.int, "Store info for generator weights study")
opts.register('doTauDebug', 0, parser.VarParsing.multiplicity.singleton,
    parser.VarParsing.varType.int, "Do the tau conversion debugging")


opts.parseArguments()
infile      = opts.file
tag         = opts.globalTag
isData      = bool(opts.isData)
release     = opts.release
doGenWeights = bool(opts.doGenWeights)
doTauDebug   = bool(opts.doTauDebug)

if not release in ["74X", "74XMINIAOD"]:
  print 'Release not recognised, exiting!'
  sys.exit(1)
print 'release     : '+release
print 'isData      : '+str(isData)
print 'globalTag   : '+str(tag)

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
process.maxEvents = cms.untracked.PSet(input = cms.untracked.int32(500))
process.options   = cms.untracked.PSet(wantSummary = cms.untracked.bool(True))
process.MessageLogger.cerr.FwkReport.reportEvery = 50

################################################################
# Input files and global tags
################################################################
process.source = cms.Source("PoolSource", fileNames = cms.untracked.vstring(infile))
process.GlobalTag.globaltag = cms.string(tag)

import UserCode.ICHiggsTauTau.default_producers_cfi as producers
import UserCode.ICHiggsTauTau.default_selectors_cfi as selectors

################################################################
# Vertices
################################################################
inputs = {
    '74X' : {
      'vertices' : cms.InputTag('offlinePrimaryVertices'),
      },
    '74XMINIAOD' : {
      'vertices' : cms.InputTag('offlineSlimmedPrimaryVertices'),
      }
    }

process.icVertexProducer = producers.icVertexProducer.clone(
  branch  = cms.string("vertices"),
  input = inputs[release]['vertices'],
  firstVertexOnly = cms.bool(True)
)

################################################################
# PFCandidates
################################################################
process.selectedPFCandidates = cms.EDFilter("PFCandidateRefSelector",
  src = cms.InputTag("particleFlow"),
  cut = cms.string("pt > 0.0")
)

process.icPFProducer = producers.icPFProducer.clone(
  branch  = cms.string("pfCandidates"),
  input   = cms.InputTag("selectedPFCandidates"),
  requestTracks       = cms.bool(True),
  requestGsfTracks    = cms.bool(True)
)

process.icPFSequence = cms.Sequence()

if doTauDebug:
  process.icPFSequence += cms.Sequence(
      process.selectedPFCandidates+
      process.icPFProducer
      )

################################################################
# Muons
################################################################
process.selectedMuons = cms.EDFilter("PATMuonRefSelector",
    src = cms.InputTag("slimmedMuons"),
    cut = cms.string("pt > 10 & abs(eta) < 2.6")
  )

process.muPFIsoValueCharged03PFIso = cms.EDProducer('ICMuonIsolation',
  input        = cms.InputTag("slimmedMuons"),
  deltaR       = cms.double(0.3),
  iso_type = cms.string("charged_iso") 
)    
process.muPFIsoValueGamma03PFIso = cms.EDProducer('ICMuonIsolation',
  input        = cms.InputTag("slimmedMuons"),
  deltaR       = cms.double(0.3),
  iso_type = cms.string("photon_iso") 
)    
process.muPFIsoValueNeutral03PFIso = cms.EDProducer('ICMuonIsolation',
  input        = cms.InputTag("slimmedMuons"),
  deltaR       = cms.double(0.3),
  iso_type = cms.string("neutral_iso") 
)    
process.muPFIsoValuePU03PFIso = cms.EDProducer('ICMuonIsolation',
  input        = cms.InputTag("slimmedMuons"),
  deltaR       = cms.double(0.3),
  iso_type = cms.string("pu_iso") 
)   

process.icMuonProducer = producers.icMuonProducer.clone(
    branch                    = cms.string("muons"),
    input                     = cms.InputTag("selectedMuons"),
    isPF                      = cms.bool(False),
    includeVertexIP           = cms.bool(True),
    inputVertices             = cms.InputTag("offlineSlimmedPrimaryVertices"),
    includeBeamspotIP         = cms.bool(True),
    inputBeamspot             = cms.InputTag("offlineBeamSpot"),
    includeFloats             = cms.PSet(),
    includePFIso03            = cms.bool(True),
    pfIso03 = cms.PSet(
      chargedAll  = cms.InputTag("muPFIsoValueCharged03PFIso"),
      charged     = cms.InputTag("muPFIsoValueCharged03PFIso"),
      neutral     = cms.InputTag("muPFIsoValueNeutral03PFIso"),
      gamma       = cms.InputTag("muPFIsoValueGamma03PFIso"),
      pu          = cms.InputTag("muPFIsoValuePU03PFIso")
    )
  )

process.icMuonSequence = cms.Sequence()

if release in ['74XMINIAOD']:
  process.icMuonSequence += cms.Sequence(
      process.selectedMuons+
      process.muPFIsoValueCharged03PFIso+
      process.muPFIsoValueGamma03PFIso+
      process.muPFIsoValueNeutral03PFIso+
      process.muPFIsoValuePU03PFIso+
      process.icMuonProducer
      )


################################################################
# Jets
################################################################
process.selectedJets = cms.EDFilter("PATJetRefSelector",
    src = cms.InputTag("slimmedJets"),
    cut = cms.string("pt > 20")
  )


process.icPFJetProducer = producers.icPFJetFromPatProducer.clone(
    branch                    = cms.string("ak4PFJetsCHS"),
    input                     = cms.InputTag("selectedJets"),
    srcConfig = cms.PSet(
      isSlimmed               = cms.bool(True),
      includeJetFlavour       = cms.bool(True),
      includeJECs             = cms.bool(True),
      inputSVInfo             = cms.InputTag(""),
      requestSVInfo           = cms.bool(False)
    ),
   destConfig = cms.PSet(
     includePileupID         = cms.bool(False),
     inputPileupID           = cms.InputTag("puJetMva", "fullDiscriminant"),
     includeTrackBasedVars   = cms.bool(False),
     inputTracks             = cms.InputTag("unpackedTracksAndVertices"),
     inputVertices           = cms.InputTag("unpackedTracksAndVertices"),
     requestTracks           = cms.bool(False)
    )
)

process.icJetSequence = cms.Sequence()

if release in ['74XMINIAOD']:
  process.icJetSequence += cms.Sequence(
      process.selectedJets+
      process.icPFJetProducer
      )

################################################################
# GenParticles
################################################################
if release in ['74X']:
  from PhysicsTools.PatAlgos.slimming.prunedGenParticles_cfi import prunedGenParticles
  process.prunedGenParticles = prunedGenParticles

process.icGenParticleProducer = producers.icGenParticleProducer.clone(
  input             = cms.InputTag("prunedGenParticles"),
  includeMothers    = cms.bool(True),
  includeDaughters  = cms.bool(True)
)

process.icMCSequence = cms.Sequence()

if not isData:
  if release in ['74X']: process.icMCSequence += process.prunedGenParticles
  process.icMCSequence += (
    process.icGenParticleProducer
    )


################################################################
# EventInfo
################################################################
process.icEventInfoProducer = producers.icEventInfoProducer.clone(
  includeJetRho       = cms.bool(True),
  inputJetRho         = cms.InputTag("fixedGridRhoFastjetAll"),
  includeLeptonRho    = cms.bool(False),
  inputLeptonRho      = cms.InputTag("fixedGridRhoFastjetAll"),
  includeVertexCount  = cms.bool(True),
  inputVertices       = inputs[release]['vertices'],
  includeCSCFilter    = cms.bool(False),
  inputCSCFilter      = cms.InputTag("BeamHaloSummary"),
  isNlo               = cms.bool((not isData) and doGenWeights),
  lheProducer         = cms.InputTag("externalLHEProducer"),
  includeLHEWeights   = cms.bool((not isData) and doGenWeights)
)

process.icEventInfoSequence = cms.Sequence(
  process.icEventInfoProducer
)

################################################################
# Event
################################################################
process.icEventProducer = producers.icEventProducer.clone()

################################################################
# Sequence
################################################################
process.p = cms.Path(
  process.icVertexProducer+
  process.icPFSequence+
  process.icMuonSequence+
  process.icJetSequence+
  process.icMCSequence+
  process.icEventInfoSequence+
  process.icEventProducer
)

process.schedule = cms.Schedule(process.p)

#print process.dumpPython()
