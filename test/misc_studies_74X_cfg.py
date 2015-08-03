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
opts.register('doTauDebugVerbose', 0, parser.VarParsing.multiplicity.singleton,
    parser.VarParsing.varType.int, "Do the tau conversion debugging")

opts.parseArguments()
infile      = opts.file
tag         = opts.globalTag
isData      = bool(opts.isData)
release     = opts.release
doGenWeights = bool(opts.doGenWeights)
doTauDebug   = bool(opts.doTauDebug)
doTauDebugVerbose   = bool(opts.doTauDebugVerbose)

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
process.options   = cms.untracked.PSet(wantSummary = cms.untracked.bool(False))
process.MessageLogger.cerr.FwkReport.reportEvery = 50

################################################################
# Input files and global tags
################################################################
process.source = cms.Source("PoolSource", fileNames = cms.untracked.vstring(infile))
process.GlobalTag.globaltag = cms.string(tag)

import UserCode.ICHiggsTauTau.default_producers_cfi as producers
import UserCode.ICHiggsTauTau.default_selectors_cfi as selectors

inputs = {
    '74X' : {
      'vertices'    : cms.InputTag('offlinePrimaryVertices'),
      'electrons'   : cms.InputTag('gedGsfElectrons'),
      'muons'       : cms.InputTag('muons')
      },
    '74XMINIAOD' : {
      'vertices'    : cms.InputTag('offlineSlimmedPrimaryVertices'),
      'electrons'   : cms.InputTag('slimmedElectrons'),
      'muons'       : cms.InputTag('slimmedMuons')
      }
    }

################################################################
# Vertices & Conversions
################################################################

process.icVertexProducer = producers.icVertexProducer.clone(
  branch  = cms.string("vertices"),
  input = inputs[release]['vertices'],
  firstVertexOnly = cms.bool(True)
)

process.icConversionProducer = cms.EDProducer('ICConversionProducer',
  branch = cms.string("conversions"),
  tracksBranch = cms.string("conversionTracks"),
  input  = cms.InputTag("allConversions"),
  saveTracks = cms.bool(True)
)

process.icPFConversionProducer = cms.EDProducer('ICConversionProducer',
  branch = cms.string("pfConversions"),
  tracksBranch = cms.string("pfConversionTracks"),
  input  = cms.InputTag("particleFlowEGamma"),
  saveTracks = cms.bool(True)
)

process.icVertexSequence = cms.Sequence(
  process.icVertexProducer
)

# Can only save the conversions in regular AOD
if release in ['74X'] and doTauDebug:
  process.icVertexSequence += cms.Sequence(
      process.icConversionProducer+
      process.icPFConversionProducer
    )

################################################################
# PFCandidates
################################################################
process.selectedPFCandidates = cms.EDFilter("PFCandidateRefSelector",
  src = cms.InputTag("particleFlow"),
  cut = cms.string("pt > 0.0")
)

process.genTauChain = cms.EDProducer("GenParticlePruner",
    src = cms.InputTag("genParticles"),
    select = cms.vstring(
        "drop  *", 
        "keep++ abs(pdgId) == 15"
    )
)

process.requestPFCandsByDRToGenTaus = cms.EDProducer('RequestPFCandidatesByDeltaR',
  src = cms.InputTag("particleFlow"),
  reference = cms.InputTag("genTauChain"),
  deltaR = cms.double(0.5)
)

process.requestPFCandsByDRToTaus = cms.EDProducer('RequestPFCandidatesByDeltaR',
  src = cms.InputTag("particleFlow"),
  reference = cms.InputTag("selectedTaus"),
  deltaR = cms.double(1.0)
)

process.icMergedPFCands = cms.EDProducer('ICPFCandidateMerger',
  merge = cms.VInputTag(
    cms.InputTag("requestPFCandsByDRToGenTaus"),
    cms.InputTag("requestPFCandsByDRToTaus")
  )
)

process.icPFProducer = producers.icPFProducer.clone(
  branch  = cms.string("pfCandidates"),
  input   = cms.InputTag('icMergedPFCands' if (not isData) else 'selectedPFCandidates'),
  requestTracks       = cms.bool(True),
  requestGsfTracks    = cms.bool(True)
)

process.icPFSequence = cms.Sequence()

if doTauDebug:
  process.icPFSequence += cms.Sequence(
      process.selectedPFCandidates+
      process.genTauChain+
      process.requestPFCandsByDRToGenTaus+
      process.requestPFCandsByDRToTaus+
      process.icMergedPFCands+
      process.icPFProducer
  )


################################################################
# Tracks
################################################################
process.selectedTracks = cms.EDFilter("TrackRefSelector",
  src = cms.InputTag("generalTracks"),
  cut = cms.string("pt > 0.5")
)

process.requestTracksByDeltaRToGenTaus = cms.EDProducer("RequestTracksByDeltaR",
  src = cms.InputTag("selectedTracks"),
  reference = cms.InputTag("genTauChain"),
  deltaR = cms.double(0.5)
  )

process.icMergedTracks = cms.EDProducer('ICTrackMerger',
  merge = cms.VInputTag(
    cms.InputTag("requestTracksByDeltaRToGenTaus"),
    cms.InputTag("icTauProducer", "requestedTracks")
  )
)

process.icTrackProducer = producers.icTrackProducer.clone(
  branch = cms.string("tracks"),
  input  = cms.InputTag("icMergedTracks")
)

process.icGsfTrackProducer = producers.icTrackProducer.clone(
  branch = cms.string("gsfTracks"),
  input  = cms.InputTag("icPFProducer", "requestedGsfTracks")
)

process.icTrackSequence = cms.Sequence()
if release in ['74X'] and doTauDebug:
  process.icTrackSequence += cms.Sequence(
    process.selectedTracks+
    process.requestTracksByDeltaRToGenTaus+
    process.icMergedTracks+
    process.icTrackProducer+
    process.icGsfTrackProducer
  )

################################################################
# Electrons
################################################################
process.icElectronSequence = cms.Sequence()

if release in ['74X']:
    process.selectedElectrons = cms.EDFilter("GsfElectronRefSelector",
        src = inputs[release]['electrons'],
        cut = cms.string("pt > 9.5 & abs(eta) < 2.6")
    )
    process.icElectronConversionCalculator = cms.EDProducer('ICElectronConversionCalculator',
        input       = inputs[release]['electrons'],
        beamspot    = cms.InputTag("offlineBeamSpot"),
        conversions = cms.InputTag("allConversions")
    )

elif release in ['74XMINIAOD']:
    process.selectedElectrons = cms.EDFilter("PATElectronRefSelector",
        src = inputs[release]['electrons'],
        cut = cms.string("pt > 9.5 & abs(eta) < 2.6")
    )
    process.icElectronConversionCalculator = cms.EDProducer('ICElectronConversionFromPatCalculator',
        input       = inputs[release]['electrons']
    )

# Extract the DR < 0.3 isolation from the electrons themselves, a la HTT sync 
process.elPFIsoValueCharged03PFIdPFIso = cms.EDProducer('ICRecoElectronIsolation',
  input        = inputs[release]['electrons'],
  deltaR       = cms.double(0.3),
  iso_type = cms.string("charged_iso") 
)
process.elPFIsoValueGamma03PFIdPFIso = cms.EDProducer('ICRecoElectronIsolation',
  input        = inputs[release]['electrons'],
  deltaR       = cms.double(0.3),
  iso_type = cms.string("photon_iso") 
)
process.elPFIsoValueNeutral03PFIdPFIso = cms.EDProducer('ICRecoElectronIsolation',
  input        = inputs[release]['electrons'],
  deltaR       = cms.double(0.3),
  iso_type = cms.string("neutral_iso") 
)
process.elPFIsoValuePU03PFIdPFIso = cms.EDProducer('ICRecoElectronIsolation',
  input        = inputs[release]['electrons'],
  deltaR       = cms.double(0.3),
  iso_type = cms.string("pu_iso") 
)

process.icElectronProducer = producers.icElectronProducer.clone(
  branch                    = cms.string("electrons"),
  input                     = cms.InputTag("selectedElectrons"),
  includeConversionMatches  = cms.bool(True),
  inputConversionMatches    = cms.InputTag("icElectronConversionCalculator"),
  includeVertexIP           = cms.bool(True),
  inputVertices             = inputs[release]['vertices'],
  includeBeamspotIP         = cms.bool(False),
  inputBeamspot             = cms.InputTag("offlineBeamSpot"),
  includeFloats             = cms.PSet(),
  includePFIso03            = cms.bool(True),
  pfIso03 = cms.PSet(
    chargedAll  = cms.InputTag("elPFIsoValueCharged03PFIdPFIso"),
    charged     = cms.InputTag("elPFIsoValueCharged03PFIdPFIso"),
    neutral     = cms.InputTag("elPFIsoValueNeutral03PFIdPFIso"),
    gamma       = cms.InputTag("elPFIsoValueGamma03PFIdPFIso"),
    pu          = cms.InputTag("elPFIsoValuePU03PFIdPFIso")
  )
)

process.icElectronSequence += cms.Sequence(
  process.selectedElectrons+
  process.icElectronConversionCalculator+
  process.elPFIsoValueCharged03PFIdPFIso+
  process.elPFIsoValueGamma03PFIdPFIso+
  process.elPFIsoValueNeutral03PFIdPFIso+
  process.elPFIsoValuePU03PFIdPFIso+
  process.icElectronProducer
)


################################################################
# Muons
################################################################
if release in ['74X']:
    process.selectedMuons = cms.EDFilter("MuonRefSelector",
        src = inputs[release]['muons'],
        cut = cms.string("pt > 10 & abs(eta) < 2.6")
      )
elif release in ['74XMINIAOD']:
    process.selectedMuons = cms.EDFilter("PATMuonRefSelector",
        src = inputs[release]['muons'],
        cut = cms.string("pt > 10 & abs(eta) < 2.6")
      )

process.muPFIsoValueCharged03PFIso = cms.EDProducer('ICMuonIsolation',
  input        = inputs[release]['muons'],
  deltaR       = cms.double(0.3),
  iso_type = cms.string("charged_iso") 
)    
process.muPFIsoValueGamma03PFIso = cms.EDProducer('ICMuonIsolation',
  input        = inputs[release]['muons'],
  deltaR       = cms.double(0.3),
  iso_type = cms.string("photon_iso") 
)    
process.muPFIsoValueNeutral03PFIso = cms.EDProducer('ICMuonIsolation',
  input        = inputs[release]['muons'],
  deltaR       = cms.double(0.3),
  iso_type = cms.string("neutral_iso") 
)    
process.muPFIsoValuePU03PFIso = cms.EDProducer('ICMuonIsolation',
  input        = inputs[release]['muons'],
  deltaR       = cms.double(0.3),
  iso_type = cms.string("pu_iso") 
)   

process.icMuonProducer = producers.icMuonProducer.clone(
    branch                    = cms.string("muons"),
    input                     = cms.InputTag("selectedMuons"),
    isPF                      = cms.bool(False),
    includeVertexIP           = cms.bool(True),
    inputVertices             = inputs[release]['vertices'],
    includeBeamspotIP         = cms.bool(False),
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

process.icMuonSequence += cms.Sequence(
    process.selectedMuons+
    process.muPFIsoValueCharged03PFIso+
    process.muPFIsoValueGamma03PFIso+
    process.muPFIsoValueNeutral03PFIso+
    process.muPFIsoValuePU03PFIso+
    process.icMuonProducer
    )

################################################################
# Taus
################################################################
import UserCode.ICHiggsTauTau.tau_discriminators_cfi as tauIDs

if release in ['74X']:
  # Note that these taus are taken before DM, there's no such
  # choice in MiniAOD
  process.selectedTaus = cms.EDFilter("PFTauRefSelector",
      src = cms.InputTag("hpsPFTauProducer"),
      cut = cms.string("pt > 18.0 & abs(eta) < 2.6")
  )
  process.icTauProducer = producers.icTauProducer.clone(
      branch                  = cms.string("taus"),
      input                   = cms.InputTag("selectedTaus"),
      inputVertices           = inputs[release]['vertices'],
      includeVertexIP         = cms.bool(True),
      requestTracks           = cms.bool(True),
      tauIDs = tauIDs.fullNewIds
  )

if release in ['74XMINIAOD']:
  process.selectedTaus = cms.EDFilter("PATTauRefSelector",
      src = cms.InputTag("slimmedTaus"),
      cut = cms.string('pt > 18.0 & abs(eta) < 2.6')
  )
  process.icTauProducer = cms.EDProducer("ICPFTauFromPatProducer",
      branch                  = cms.string("taus"),
      input                   = cms.InputTag("selectedTaus"),
      inputVertices           = vtxLabel,
      includeVertexIP         = cms.bool(True),
      requestTracks           = cms.bool(False),
      tauIDs = cms.PSet()
  )

process.icTauSequence = cms.Sequence(
  process.selectedTaus+
  process.icTauProducer
)

if doTauDebugVerbose:
  process.tauDebug = cms.EDProducer('TauDebug',
      input = cms.InputTag("particleFlow")
    )
  process.icTauSequence += process.tauDebug


################################################################
# Jets
################################################################
process.icJetSequence = cms.Sequence()

if release in ['74XMINIAOD']:
  process.selectedJets = cms.EDFilter("PATJetRefSelector",
      src = cms.InputTag("slimmedJets"),
      cut = cms.string("pt > 15")
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
  process.icJetSequence += cms.Sequence(
    process.selectedJets+
    process.icPFJetProducer
  )
elif release in ['74X']:
   # Jet energy corrections
   # ----------------------
  # process.ak4PFL1FastjetCHS = cms.ESProducer("L1FastjetCorrectionESProducer",
  #     srcRho = cms.InputTag("fixedGridRhoFastjetAll"),
  #     algorithm = cms.string('AK4PFchs'),
  #     level = cms.string('L1FastJet')
  # )
  # process.ak4PFL2RelativeCHS = cms.ESProducer("LXXXCorrectionESProducer",
  #     algorithm = cms.string('AK4PFchs'),
  #     level = cms.string('L2Relative')
  # )
  # process.ak4PFL3AbsoluteCHS = cms.ESProducer("LXXXCorrectionESProducer",
  #     algorithm = cms.string('AK4PFchs'),
  #     level = cms.string('L3Absolute')
  # )
  # pfJECS = cms.PSet(
  #   L1FastJet  = cms.string("ak4PFL1FastjetCHS"),
  #   L2Relative = cms.string("ak4PFL2RelativeCHS"),
  #   L3Absolute = cms.string("ak4PFL3AbsoluteCHS")
  # )
  process.selectedPFJets = cms.EDFilter("PFJetRefSelector",
     src = cms.InputTag("ak4PFJetsCHS"),
     cut = cms.string("pt > 15")
     )
  process.icPFJetProducer = producers.icPFJetProducer.clone(
      branch                    = cms.string("ak4PFJetsCHS"),
      input                     = cms.InputTag("selectedPFJets"),
      srcConfig = cms.PSet(
        includeJetFlavour         = cms.bool(False),
        inputJetFlavour           = cms.InputTag("icPFJetFlavourCalculator"),
        applyJECs                 = cms.bool(False),
        includeJECs               = cms.bool(False),
        JECs                      = cms.PSet(),
        applyCutAfterJECs         = cms.bool(False),
        cutAfterJECs              = cms.string("pt > 15.0"),
        inputSVInfo               = cms.InputTag(""),
        requestSVInfo             = cms.bool(False),
        BTagDiscriminators        = cms.PSet()
      ),
      destConfig = cms.PSet(
        includePileupID       = cms.bool(False), #rerunning the pu MVA on the jet collection created in miniAOD is possible in newer CMSSW versions but not yet in 72
        inputPileupID         = cms.InputTag("puJetMva", "fullDiscriminant"),
        includeTrackBasedVars = cms.bool(False),
        inputTracks           = cms.InputTag("generalTracks"),
        inputVertices         = inputs[release]['vertices'],
        requestTracks         = cms.bool(False)
      )
  )
  process.icJetSequence += cms.Sequence(
    process.selectedPFJets+
    process.icPFJetProducer
  )


process.icMCSequence = cms.Sequence()
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


if not isData:
  if release in ['74X']: process.icMCSequence += process.prunedGenParticles
  process.icMCSequence += (
    process.icGenParticleProducer
    )

################################################################
# PileupInfo
################################################################
process.icPileupInfoProducer = producers.icPileupInfoProducer.clone()
process.icMCSequence += process.icPileupInfoProducer

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
  process.icVertexSequence+
  process.icElectronSequence+
  process.icMuonSequence+
  process.icTauSequence+
  process.icJetSequence+
  process.icPFSequence+
  process.icTrackSequence+
  process.icMCSequence+
  process.icEventInfoSequence+
  process.icEventProducer
)

process.schedule = cms.Schedule(process.p)

#print process.dumpPython()
