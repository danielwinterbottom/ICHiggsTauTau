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
opts.register('isData', 0, parser.VarParsing.multiplicity.singleton,
    parser.VarParsing.varType.int, "Process as data?")
opts.register('isEmbedded', 0, parser.VarParsing.multiplicity.singleton,
    parser.VarParsing.varType.int, "Process as embedded?")
opts.register('release', '', parser.VarParsing.multiplicity.singleton,
    parser.VarParsing.varType.string, "Release label")
opts.register('isTandP', 0, parser.VarParsing.multiplicity.singleton,
    parser.VarParsing.varType.int, "Tag and probe ntuples?")
opts.register('isZStudy', 0, parser.VarParsing.multiplicity.singleton,
    parser.VarParsing.varType.int, "Process for Z->ee or Z->mumu?")
opts.register('isPhys14', 0, parser.VarParsing.multiplicity.singleton,
    parser.VarParsing.varType.int, "Process for Phys14 study?")
opts.parseArguments()
infile      = opts.file
if not infile: infile = "file:/tmp/file.root"
tag         = opts.globalTag
isData      = opts.isData
release     = opts.release
isEmbedded  = opts.isEmbedded
isTandP     = opts.isTandP
isZStudy    = opts.isZStudy
isPhys14    = opts.isPhys14

if not release in ["42X", "53X", "70X", "70XMINIAOD", "72X", "72XMINIAOD"]:
  print 'Release not recognised, exiting!'
  sys.exit(1)
print 'release     : '+release
print 'isData      : '+str(isData)
print 'isEmbedded  : '+str(isEmbedded)
print 'isTandP     : '+str(isTandP)
print 'isZStudy    : '+str(isZStudy)
print 'isPhys14    : '+str(isPhys14)
print 'globalTag   : '+tag

################################################################
# Standard setup
################################################################
process.load("FWCore.MessageLogger.MessageLogger_cfi")
if release in ['42X']:
  process.load("Configuration.StandardSequences.Geometry_cff")
if release in ['53X', "70X", "70XMINIAOD", "72X", "72XMINIAOD"]:
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
  input = cms.untracked.int32(1000)
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

# 42X Data: root://eoscms//eos/cms/store/user/agilbert/samples/TauPlusX-2011A-Run166512-42X.root, GR_R_42_V25::All
# 42X MC: root://eoscms//eos/cms/store/user/agilbert/samples/DYJetsToLL-Fall11-42X.root, START42_V17::All
# 53X Data: root://eoscms//eos/cms/store/user/agilbert/samples/TauPlusX-2012D.root, FT_53_V21_AN4::All
# 53X MC: root://eoscms//eos/cms/store/user/agilbert/samples/DYJetsToLL-Summer12-53X-Sample.root, START53_V22::All
# 70X MC: file=root://eoscms//eos/cms/store/user/agilbert/samples/GluGluToHToTauTau_M-125_13TeV-powheg-pythia6-PU_S14_POSTLS170_V6-v1.root globalTag=PLS170_V7AN1::All
# 70XMINIAOD MC: file=root://eoscms//eos/cms/store/user/agilbert/samples/VBF_HToTauTau_M-125_13TeV-powheg-pythia6-PU20bx25_POSTLS170_V5-v2-MINIAODSIM.root globalTag=PLS170_V7AN1::All
# 72X MC: file=root://xrootd.unl.edu//store/mc/Phys14DR/VBF_HToTauTau_M-125_13TeV-powheg-pythia6/AODSIM/PU40bx25_PHYS14_25_V1-v1/00000/00E63918-3A70-E411-A246-7845C4FC35F3.root globalTag=START72_V1::All 
# 72XMINIAOD MC: file=root://xrootd.unl.edu//store/mc/Phys14DR/VBF_HToTauTau_M-125_13TeV-powheg-pythia6/MINIAODSIM/PU40bx25_PHYS14_25_V1-v1/00000/36224FE2-0571-E411-9664-00266CFAE30C.root globalTag=START72_V1::All

import UserCode.ICHiggsTauTau.default_producers_cfi as producers
import UserCode.ICHiggsTauTau.default_selectors_cfi as selectors

################################################################
# Re-do PFTau reconstruction
################################################################
process.load("RecoTauTag/Configuration/RecoPFTauTag_cff")


process.ic42XSequence = cms.Sequence()
process.ic53XSequence = cms.Sequence()
process.ic70XSequence = cms.Sequence()
process.ic72XSequence = cms.Sequence()
process.icMiniAODSequence = cms.Sequence()

if release in ['42X']:
  process.ic42XSequence += process.recoTauClassicHPSSequence

if release in ['53X']:
  process.ic53XSequence += process.recoTauClassicHPSSequence

if release in ['70X']:
  process.ic70XSequence += process.PFTau

if release in ['72X']:
  process.ic72XSequence += process.PFTau

if release in ['70XMINIAOD', '72XMINIAOD']:
  process.load('PhysicsTools.PatAlgos.slimming.unpackedTracksAndVertices_cfi')
  process.icMiniAODSequence += process.unpackedTracksAndVertices

################################################################
# Need to create kt6PFJets in 42X for L1FastJet correction
################################################################
if release in ['42X', '70X', '70XMINIAOD', '72X', '72XMINIAOD']:
  from RecoJets.JetProducers.kt4PFJets_cfi import kt4PFJets
  process.kt6PFJets = kt4PFJets.clone(
    rParam = cms.double(0.6),
    doAreaFastjet = cms.bool(True),
    doRhoFastjet = cms.bool(True)
  )
  if release in ['42X']: process.ic42XSequence += process.kt6PFJets
  if release in ['70X', '70XMINIAOD', '72X', '72XMINIAOD']:
    process.ic70XSequence += process.kt6PFJets

if release in ['42X']:
  process.load("RecoJets.JetProducers.ak5PFJets_cfi")
  process.ak5PFJets.doAreaFastjet = cms.bool(True)
  process.ic42XSequence += process.ak5PFJets

################################################################
# Object Selection
################################################################
process.selectedVertices = cms.EDFilter("VertexRefSelector",
  src = cms.InputTag("offlinePrimaryVertices"),
  cut = cms.string("ndof >= 4 & abs(z) <= 24 & abs(position.Rho) <= 2")
)

process.selectedPFCandidates = cms.EDFilter("PFCandidateRefSelector",
  src = cms.InputTag("particleFlow"),
  cut = cms.string("pt > 0.0")
)

process.selectedElectrons = cms.EDFilter("GsfElectronRefSelector",
  src = cms.InputTag("gsfElectrons"),
  cut = cms.string("pt > 9.5 & abs(eta) < 2.6")
)
if release in ['70X', '72X']:
  process.selectedElectrons.src = cms.InputTag("gedGsfElectrons")

process.selectedPFMuons = cms.EDFilter("GenericPFCandidateSelector",
  src = cms.InputTag("particleFlow"),
  cut = cms.string("pt > 3.0 & abs(eta) < 2.6 & abs(pdgId) == 13")
)

process.selectedPFTaus = cms.EDFilter("PFTauRefSelector",
  src = cms.InputTag("hpsPFTauProducer"),
  cut = cms.string("pt > 18.0 & abs(eta) < 2.6 & decayMode > -0.5")
)
if isPhys14:
  process.selectedPFTaus.cut = cms.string("pt > 10.0 & abs(eta) < 3.0")

if release in ['70X', '72X'] and not isPhys14:
  process.selectedPFTausPre = cms.EDFilter("PFTauRefSelector",
    src = cms.InputTag("hpsPFTauProducer"),
    cut = cms.string("pt > 18.0 & abs(eta) < 2.6")
  )
  process.selectedPFTaus = cms.EDFilter("RecoTauDiscriminatorRefSelector",
    src = cms.InputTag("selectedPFTausPre"),
    discriminator = cms.InputTag("hpsPFTauDiscriminationByDecayModeFinding"),
    cut = cms.double(0.5),
    filter = cms.bool(False)
  )

if release in ['70XMINIAOD', '72XMINIAOD']:
  process.selectedVertices.src = cms.InputTag("offlineSlimmedPrimaryVertices")
  process.selectedElectrons = cms.EDFilter("PATElectronRefSelector",
      src = cms.InputTag("slimmedElectrons"),
      cut = cms.string("pt > 9.5 & abs(eta) < 2.6")
      )
  process.selectedPFMuons = cms.EDFilter("PATMuonRefSelector",
      src = cms.InputTag("slimmedMuons"),
      cut = cms.string("pt > 3 & abs(eta) < 2.6")
      )
  process.selectedPFTaus = cms.EDFilter("PATTauRefSelector",
      src = cms.InputTag("slimmedTaus"),
      cut = cms.string('pt > 18.0 & abs(eta) < 2.6 & tauID("decayModeFinding") > 0.5')
      )

process.icSelectionSequence = cms.Sequence(
  process.selectedVertices+
  process.selectedPFCandidates+
  process.selectedElectrons+
  process.selectedPFMuons
)

if release in ['70X', '72X'] and not isPhys14:
  process.icSelectionSequence += process.selectedPFTausPre

process.icSelectionSequence += process.selectedPFTaus


################################################################
# PF sequence for lepton isolation
################################################################
process.load("CommonTools.ParticleFlow.pfParticleSelection_cff")
if release in ['70X', '72X']:
  process.pfPileUp.PFCandidates = cms.InputTag("particleFlowPtrs")
  process.pfPileUpIso.PFCandidates = cms.InputTag("particleFlowPtrs")
  process.pfNoPileUp.bottomCollection = cms.InputTag("particleFlowPtrs")
  process.pfNoPileUpIso.bottomCollection = cms.InputTag("particleFlowPtrs")

if release in ['70XMINIAOD', '72XMINIAOD']:
  process.pfPileUp = cms.EDFilter("CandPtrSelector",
      src = cms.InputTag("packedPFCandidates"),
      cut = cms.string("fromPV <= 1")
      )
  process.pfNoPileUp = cms.EDFilter("CandPtrSelector",
      src = cms.InputTag("packedPFCandidates"),
      cut = cms.string("fromPV > 1")
      )
  process.pfAllNeutralHadrons = cms.EDFilter("CandPtrSelector",
      src = cms.InputTag("pfNoPileUp"),
      cut = cms.string("abs(pdgId) = 111 | abs(pdgId) = 130 | " \
          "abs(pdgId) = 310 | abs(pdgId) = 2112")
      )
  process.pfAllChargedHadrons= cms.EDFilter("CandPtrSelector",
      src = cms.InputTag("pfNoPileUp"),
      cut = cms.string("abs(pdgId) = 211 | abs(pdgId) = 321 | " \
          "abs(pdgId) = 999211 | abs(pdgId) = 2212")
      )
  process.pfAllPhotons= cms.EDFilter("CandPtrSelector",
      src = cms.InputTag("pfNoPileUp"),
      cut = cms.string("abs(pdgId) = 22")
      )
  process.pfAllChargedParticles= cms.EDFilter("CandPtrSelector",
      src = cms.InputTag("pfNoPileUp"),
      cut = cms.string("abs(pdgId) = 211 | abs(pdgId) = 321 | " \
          "abs(pdgId) = 999211 | abs(pdgId) = 2212 | " \
          "abs(pdgId) = 11 | abs(pdgId) = 13")
      )
  process.pfPileUpAllChargedParticles= cms.EDFilter("CandPtrSelector",
      src = cms.InputTag("pfPileUp"),
      cut = cms.string("abs(pdgId) = 211 | abs(pdgId) = 321 | " \
          "abs(pdgId) = 999211 | abs(pdgId) = 2212 | " \
          "abs(pdgId) = 11 | abs(pdgId) = 13")
      )
  process.pfAllNeutralHadronsAndPhotons = cms.EDFilter("CandPtrSelector",
      src = cms.InputTag("pfNoPileUp"),
      cut = cms.string("abs(pdgId) = 111 | abs(pdgId) = 130 | " \
          "abs(pdgId) = 310 | abs(pdgId) = 2112 | abs(pdgId) = 22")
      )
  process.pfParticleSelectionSequence = cms.Sequence(
      process.pfPileUp+
      process.pfNoPileUp+
      process.pfAllNeutralHadrons+
      process.pfAllChargedHadrons+
      process.pfAllPhotons+
      process.pfAllChargedParticles+
      process.pfPileUpAllChargedParticles+
      process.pfAllNeutralHadronsAndPhotons
      )


################################################################
# Vertices
################################################################
process.icVertexProducer = producers.icVertexProducer.clone(
  branch  = cms.string("vertices"),
  input = cms.InputTag("selectedVertices"),
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
  process.icVertexProducer+
  process.icConversionProducer+
  process.icPFConversionProducer
)

################################################################
# PFCandidates
################################################################
process.icPFProducer = cms.EDProducer('ICPFProducer',
  branch  = cms.string("pfCandidates"),
  input   = cms.InputTag("selectedPFCandidates"),
  requestTracks       = cms.bool(True),
  requestGsfTracks    = cms.bool(True),
  inputUnpackedTracks = cms.InputTag("")
)

process.icPFSequence = cms.Sequence()

if isPhys14:
  process.icPFSequence += process.icPFProducer


################################################################
# Tracks
################################################################
process.selectedTracks = cms.EDFilter("TrackRefSelector",
  src = cms.InputTag("generalTracks"),
  cut = cms.string("pt > 0.5")
)

process.selectedTauTracks = cms.EDFilter("TrackRefSelector",
  src = cms.InputTag("generalTracks"),
  cut = cms.string("pt > 0.5")
)

process.requestTracksByDeltaRToTaus = cms.EDProducer("RequestTracksByDeltaR",
  src = cms.InputTag("selectedTauTracks"),
  reference = cms.InputTag("selectedPFTaus"),
  deltaR = cms.double(0.5)
  )

# We write (for phys 14 studies):
# - all tracks with pT > 5 GeV
# - tracks referenced by the PF candidates we store
# - tracks referenced by the taus we store
# - all tracks with DR < 0.5 pf the selected PF taus with pT > 0.5 GeV
process.icMergedTracks = cms.EDProducer('ICTrackMerger',
  merge = cms.VInputTag(
    cms.InputTag("selectedTracks"),
    cms.InputTag("icPFProducer", "requestedTracks"),
    cms.InputTag("icTauProducer", "requestedTracks"),
    cms.InputTag("requestTracksByDeltaRToTaus")
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
if isPhys14:
  process.icTrackSequence += cms.Sequence(
    process.selectedTracks+
    process.selectedTauTracks+
    process.requestTracksByDeltaRToTaus+
    process.icMergedTracks+
    process.icTrackProducer+
    process.icGsfTrackProducer
  )

################################################################
# Electrons
################################################################
electronLabel = cms.InputTag("gsfElectrons")
if release in ['70X', '72X']: electronLabel = cms.InputTag("gedGsfElectrons")
if release in ['70XMINIAOD', '72XMINIAOD']: electronLabel = cms.InputTag("slimmedElectrons")

process.icElectronSequence = cms.Sequence()

process.icElectronConversionCalculator = cms.EDProducer('ICElectronConversionCalculator',
    input       = electronLabel,
    beamspot    = cms.InputTag("offlineBeamSpot"),
    conversions = cms.InputTag("allConversions")
)
if release in ['70XMINIAOD', '72XMINIAOD']:
  process.icElectronConversionCalculator = cms.EDProducer('ICElectronConversionFromPatCalculator',
      input       = electronLabel
  )



process.icHttElecIsoCheck = cms.EDProducer('ICHttElecIsoCheck',
    input         = electronLabel,
    pfChargedAll  = cms.InputTag("pfAllChargedParticles")
)

process.icHttMuonOverlapCheck = cms.EDProducer('ICHttMuonOverlapCheck',
    input = electronLabel,
    muons = cms.InputTag("muons")
)

process.load("EgammaAnalysis.ElectronTools.electronIdMVAProducer_cfi")
#if release in ['70XMINIAOD', ['72XMINIAOD']:
#  process.mvaTrigV0.vertexTag = cms.InputTag("offlineSlimmedPrimaryVertices")
#  process.mvaTrigV0.reducedEBRecHitCollection = cms.InputTag("reducedEgamma", "reducedEBRecHits", "PAT")
#  process.mvaTrigV0.reducedEERecHitCollection = cms.InputTag("reducedEgamma", "reducedEERecHits", "PAT")
#  process.mvaNonTrigV0.vertexTag = cms.InputTag("offlineSlimmedPrimaryVertices")
#  process.mvaNonTrigV0.reducedEBRecHitCollection = cms.InputTag("reducedEgamma", "reducedEBRecHits", "PAT")
#  process.mvaNonTrigV0.reducedEERecHitCollection = cms.InputTag("reducedEgamma", "reducedEERecHits", "PAT")

# 72X: error in getting calo conditions!
if release in ['42X', '53X', '70X']:
  process.icElectronSequence += cms.Sequence(
    process.mvaTrigV0+
    process.mvaNonTrigV0
  )
  process.mvaTrigV0.electronTag = electronLabel
  process.mvaNonTrigV0.electronTag = electronLabel

if release in ['42X', '53X']:
  from CommonTools.ParticleFlow.Tools.pfIsolation import setupPFElectronIso
  process.eleIsoSequence = setupPFElectronIso(process, 'gsfElectrons')
  process.eleIsoSequence.remove(process.elPFIsoValueCharged03NoPFIdPFIso)
  process.eleIsoSequence.remove(process.elPFIsoValueChargedAll03NoPFIdPFIso)
  process.eleIsoSequence.remove(process.elPFIsoValueGamma03NoPFIdPFIso)
  process.eleIsoSequence.remove(process.elPFIsoValueNeutral03NoPFIdPFIso)
  process.eleIsoSequence.remove(process.elPFIsoValuePU03NoPFIdPFIso)
  process.eleIsoSequence.remove(process.elPFIsoValueCharged04NoPFIdPFIso)
  process.eleIsoSequence.remove(process.elPFIsoValueChargedAll04NoPFIdPFIso)
  process.eleIsoSequence.remove(process.elPFIsoValueGamma04NoPFIdPFIso)
  process.eleIsoSequence.remove(process.elPFIsoValueNeutral04NoPFIdPFIso)
  process.eleIsoSequence.remove(process.elPFIsoValuePU04NoPFIdPFIso)
  process.icElectronSequence += cms.Sequence(
      process.eleIsoSequence
      )

if release in ['70X', '70XMINIAOD', '72X', '72XMINIAOD']:
  process.load("CommonTools.ParticleFlow.Isolation.pfElectronIsolation_cff")
  process.elPFIsoValueCharged04PFIdPFIso    = process.elPFIsoValueCharged04PFId.clone()
  process.elPFIsoValueChargedAll04PFIdPFIso = process.elPFIsoValueChargedAll04PFId.clone()
  process.elPFIsoValueGamma04PFIdPFIso      = process.elPFIsoValueGamma04PFId.clone()
  process.elPFIsoValueNeutral04PFIdPFIso    = process.elPFIsoValueNeutral04PFId.clone()
  process.elPFIsoValuePU04PFIdPFIso         = process.elPFIsoValuePU04PFId.clone()
  process.electronPFIsolationValuesSequence = cms.Sequence(
      process.elPFIsoValueCharged04PFIdPFIso+
      process.elPFIsoValueChargedAll04PFIdPFIso+
      process.elPFIsoValueGamma04PFIdPFIso+
      process.elPFIsoValueNeutral04PFIdPFIso+
      process.elPFIsoValuePU04PFIdPFIso
      )
  process.elPFIsoDepositCharged.src     = electronLabel
  process.elPFIsoDepositChargedAll.src  = electronLabel
  process.elPFIsoDepositNeutral.src     = electronLabel
  process.elPFIsoDepositGamma.src       = electronLabel
  process.elPFIsoDepositPU.src          = electronLabel
  if release in ['70XMINIAOD', '72XMINIAOD']:
    process.elPFIsoDepositGamma.ExtractorPSet.ComponentName = cms.string("CandViewExtractor")
  process.icElectronSequence += cms.Sequence(
      process.electronPFIsolationDepositsSequence+
      process.electronPFIsolationValuesSequence
      )

process.elPFIsoValueGamma04PFIdPFIso.deposits[0].vetos = (
    cms.vstring('EcalEndcaps:ConeVeto(0.08)','EcalBarrel:ConeVeto(0.08)'))
process.elPFIsoValueNeutral04PFIdPFIso.deposits[0].vetos = (
    cms.vstring())
process.elPFIsoValuePU04PFIdPFIso.deposits[0].vetos = (
    cms.vstring())
process.elPFIsoValueCharged04PFIdPFIso.deposits[0].vetos = (
    cms.vstring('EcalEndcaps:ConeVeto(0.015)'))
process.elPFIsoValueChargedAll04PFIdPFIso.deposits[0].vetos = (
    cms.vstring('EcalEndcaps:ConeVeto(0.015)','EcalBarrel:ConeVeto(0.01)'))

process.icElectronProducer = producers.icElectronProducer.clone(
  branch                    = cms.string("electrons"),
  input                     = cms.InputTag("selectedElectrons"),
  includeConversionMatches  = cms.bool(True),
  inputConversionMatches    = cms.InputTag("icElectronConversionCalculator"),
  includeVertexIP           = cms.bool(True),
  inputVertices             = cms.InputTag("selectedVertices"),
  includeBeamspotIP         = cms.bool(True),
  inputBeamspot             = cms.InputTag("offlineBeamSpot"),
  includeFloats = cms.PSet(
    mvaTrigV0       = cms.InputTag("mvaTrigV0"),
    mvaNonTrigV0    = cms.InputTag("mvaNonTrigV0"),
    trackInIsoSum   = cms.InputTag("icHttElecIsoCheck"),
    matchedRecoMuon = cms.InputTag("icHttMuonOverlapCheck")
  ),
  includePFIso04           = cms.bool(True)
)

if release in ['70XMINIAOD', '72XMINIAOD']:
  process.icElectronProducer.includeFloats = cms.PSet(
    #trackInIsoSum   = cms.InputTag("icHttElecIsoCheck"),
    #matchedRecoMuon = cms.InputTag("icHttMuonOverlapCheck")
    )
if release in ['72X']:
  process.icElectronProducer.includeFloats = cms.PSet(
    trackInIsoSum   = cms.InputTag("icHttElecIsoCheck"),
    matchedRecoMuon = cms.InputTag("icHttMuonOverlapCheck")
  )


if release in ['42X', '53X', '70X', '72X']:
  process.icElectronSequence += cms.Sequence(
    process.icHttElecIsoCheck+
    process.icHttMuonOverlapCheck
  )

process.icElectronSequence += cms.Sequence(
  process.icElectronConversionCalculator+
  process.icElectronProducer
)

################################################################
# Muons
################################################################
process.icMuonSequence = cms.Sequence()
if release in ['42X', '53X']:
  from CommonTools.ParticleFlow.Tools.pfIsolation import setupPFMuonIso
  process.muIsoSequence = setupPFMuonIso(process, 'selectedPFMuons')
  process.icMuonSequence += process.muIsoSequence

if release in ['70X', '70XMINIAOD', '72X', '72XMINIAOD']:
  process.load("CommonTools.ParticleFlow.Isolation.pfMuonIsolation_cff")
  process.muPFIsoValueCharged04PFIso    = process.muPFIsoValueCharged04.clone()
  process.muPFIsoValueChargedAll04PFIso = process.muPFIsoValueChargedAll04.clone()
  process.muPFIsoValueGamma04PFIso      = process.muPFIsoValueGamma04.clone()
  process.muPFIsoValueNeutral04PFIso    = process.muPFIsoValueNeutral04.clone()
  process.muPFIsoValuePU04PFIso         = process.muPFIsoValuePU04.clone()
  process.muonPFIsolationValuesSequence = cms.Sequence(
      process.muPFIsoValueCharged04PFIso+
      process.muPFIsoValueChargedAll04PFIso+
      process.muPFIsoValueGamma04PFIso+
      process.muPFIsoValueNeutral04PFIso+
      process.muPFIsoValuePU04PFIso
      )
  if release in ['70X', '72X']: muons = cms.InputTag("selectedPFMuons")
  if release in ['70XMINIAOD', '72XMINIAOD']: muons = cms.InputTag("slimmedMuons")
  process.muPFIsoDepositCharged.src     = muons
  process.muPFIsoDepositChargedAll.src  = muons
  process.muPFIsoDepositNeutral.src     = muons
  process.muPFIsoDepositGamma.src       = muons
  process.muPFIsoDepositPU.src          = muons
  process.icMuonSequence += cms.Sequence(
      process.muonPFIsolationDepositsSequence+
      process.muonPFIsolationValuesSequence
      )


process.icMuonProducer = producers.icMuonProducer.clone(
  branch                    = cms.string("muonsPFlow"),
  input                     = cms.InputTag("selectedPFMuons"),
  isPF                      = cms.bool(True),
  includeVertexIP           = cms.bool(True),
  inputVertices             = cms.InputTag("selectedVertices"),
  includeBeamspotIP         = cms.bool(True),
  inputBeamspot             = cms.InputTag("offlineBeamSpot"),
  includeFloats = cms.PSet(
  ),
  includePFIso04           = cms.bool(True)
)
if release in ['70XMINIAOD', '72XMINIAOD']: process.icMuonProducer.isPF = cms.bool(False)

process.icMuonSequence += cms.Sequence(
  process.icMuonProducer
)

################################################################
# Taus
################################################################
import UserCode.ICHiggsTauTau.tau_discriminators_cfi as tauIDs

process.icTauProducer = producers.icTauProducer.clone(
  input                   = cms.InputTag("selectedPFTaus"),
  inputVertices           = cms.InputTag("selectedVertices"),
  includeVertexIP         = cms.bool(True),
  requestTracks           = cms.bool(False),
  tauIDs = tauIDs.minimalHttIds
)

## In the Phys14 studies we want to keep a record of the "signal"
## tracks used to build the tau, so that we can approximate the
## charged isolation ourselves
if isPhys14: process.icTauProducer.requestTracks = cms.bool(True)


if release in ['53X']: process.icTauProducer.tauIDs = tauIDs.fullNewIds
if release in ['70X', '72X']: process.icTauProducer.tauIDs = tauIDs.fullNewIds

if release in ['70XMINIAOD', '72XMINIAOD']:
  process.icTauProducer = cms.EDProducer("ICPFTauFromPatProducer",
    branch                  = cms.string("taus"),
    input                   = cms.InputTag("selectedPFTaus"),
    inputVertices           = cms.InputTag("selectedVertices"),
    includeVertexIP         = cms.bool(True),
    requestTracks           = cms.bool(False),
    tauIDs = cms.PSet()
  )

process.icTauSequence = cms.Sequence(
  process.icTauProducer
)

################################################################
# Jets
################################################################

if release in ['70XMINIAOD', '72XMINIAOD']:
  from RecoJets.JetProducers.ak5PFJets_cfi import ak5PFJets
  process.ak5PFJets = ak5PFJets.clone(
      src = 'packedPFCandidates',
      doAreaFastjet = True
      )
  from RecoJets.JetProducers.kt4PFJets_cfi import kt4PFJets
  process.kt6PFJets = kt4PFJets.clone(
      src = 'packedPFCandidates',
      rParam = cms.double(0.6),
      doAreaFastjet = cms.bool(True),
      doRhoFastjet = cms.bool(True)
      )

# Parton flavour
# --------------
process.jetPartons = cms.EDProducer("PartonSelector",
    src = cms.InputTag("genParticles"),
    withLeptons = cms.bool(False)
)
if release in ['70XMINIAOD', '72XMINIAOD']:
  process.jetPartons.src = cms.InputTag("prunedGenParticles")

process.pfJetPartonMatches = cms.EDProducer("JetPartonMatcher",
    jets = cms.InputTag("ak5PFJets"),
    coneSizeToAssociate = cms.double(0.3),
    partons = cms.InputTag("jetPartons")
)
process.pfJetFlavourAssociation = cms.EDProducer("JetFlavourIdentifier",
    srcByReference = cms.InputTag("pfJetPartonMatches"),
    physicsDefinition = cms.bool(False)
)

process.icPFJetFlavourCalculator = cms.EDProducer('ICJetFlavourCalculator',
    input       = cms.InputTag("ak5PFJets"),
    flavourMap  = cms.InputTag("pfJetFlavourAssociation")
)

# Jet energy corrections
# ----------------------
process.ak5PFL1Fastjet = cms.ESProducer("L1FastjetCorrectionESProducer",
    srcRho = cms.InputTag("kt6PFJets", "rho"),
    algorithm = cms.string('AK5PF'),
    level = cms.string('L1FastJet')
)
process.ak5PFL2Relative = cms.ESProducer("LXXXCorrectionESProducer",
    algorithm = cms.string('AK5PF'),
    level = cms.string('L2Relative')
)
process.ak5PFL3Absolute = cms.ESProducer("LXXXCorrectionESProducer",
    algorithm = cms.string('AK5PF'),
    level = cms.string('L3Absolute')
)
process.ak5PFResidual = cms.ESProducer("LXXXCorrectionESProducer",
    algorithm = cms.string('AK5PF'),
    level = cms.string('L2L3Residual')
)

pfJECS = cms.PSet(
  L1FastJet  = cms.string("ak5PFL1Fastjet"),
  L2Relative = cms.string("ak5PFL2Relative"),
  L3Absolute = cms.string("ak5PFL3Absolute")
)
if isData: pfJECS.append(
  L2L3Residual = cms.string("ak5PFResidual")
)

# b-tagging
# ---------
process.load("RecoJets.JetAssociationProducers.ak5JTA_cff")
from RecoJets.JetAssociationProducers.ak5JTA_cff import ak5JetTracksAssociatorAtVertex
process.load("RecoBTag.Configuration.RecoBTag_cff")
import RecoBTag.Configuration.RecoBTag_cff as btag
process.jetTracksAssociatorAtVertexAK5PF = ak5JetTracksAssociatorAtVertex.clone(
  jets = cms.InputTag("ak5PFJets")
)

if release in ['70XMINIAOD', '72XMINIAOD']:
  process.jetTracksAssociatorAtVertexAK5PF.tracks = cms.InputTag("unpackedTracksAndVertices")
  process.jetTracksAssociatorAtVertexAK5PF.pvSrc = cms.InputTag("unpackedTracksAndVertices")

if isEmbedded:
  process.jetTracksAssociatorAtVertexAK5PF.tracks = cms.InputTag("tmfTracks")

process.impactParameterTagInfosAK5PF = btag.impactParameterTagInfos.clone(
  jetTracks = cms.InputTag('jetTracksAssociatorAtVertexAK5PF')
)
if release in ['70XMINIAOD', '72XMINIAOD']:
  process.impactParameterTagInfosAK5PF.primaryVertex = cms.InputTag("unpackedTracksAndVertices")

process.secondaryVertexTagInfosAK5PF = btag.secondaryVertexTagInfos.clone(
  trackIPTagInfos = cms.InputTag('impactParameterTagInfosAK5PF')
)
process.simpleSecondaryVertexHighEffBJetTagsAK5PF = btag.simpleSecondaryVertexHighEffBJetTags.clone (
  tagInfos = cms.VInputTag('secondaryVertexTagInfosAK5PF')
)
process.simpleSecondaryVertexHighPurBJetTagsAK5PF = btag.simpleSecondaryVertexHighPurBJetTags.clone (
  tagInfos = cms.VInputTag('secondaryVertexTagInfosAK5PF')
)
process.combinedSecondaryVertexBJetTagsAK5PF = btag.combinedSecondaryVertexBJetTags.clone (
  tagInfos = cms.VInputTag('impactParameterTagInfosAK5PF', 'secondaryVertexTagInfosAK5PF')
)

process.btaggingSequenceAK5PF = cms.Sequence(
  process.jetTracksAssociatorAtVertexAK5PF
  +process.impactParameterTagInfosAK5PF
  +process.secondaryVertexTagInfosAK5PF
  +process.simpleSecondaryVertexHighEffBJetTagsAK5PF
  +process.simpleSecondaryVertexHighPurBJetTagsAK5PF
  +process.combinedSecondaryVertexBJetTagsAK5PF
 )

# Pileup ID
# ---------
stdalgos = cms.VPSet()
if release in ['70X', '70XMINIAOD', '72X', '72XMINIAOD']:
  from RecoJets.JetProducers.PileupJetIDCutParams_cfi import full_5x_wp
  full_53x = cms.PSet(
      impactParTkThreshold = cms.double(1.) ,
      cutBased = cms.bool(False),
      tmvaWeights = cms.string("RecoJets/JetProducers/data/TMVAClassificationCategory_JetID_53X_Dec2012.weights.xml.gz"),
      tmvaMethod  = cms.string("JetIDMVAHighPt"),
      version = cms.int32(-1),
      tmvaVariables = cms.vstring(
        "nvtx"     ,
        "dZ"       ,
        "beta"     ,
        "betaStar" ,
        "nCharged" ,
        "nNeutrals",
        "dR2Mean"  ,
        "ptD"      ,
        "frac01"   ,
        "frac02"   ,
        "frac03"   ,
        "frac04"   ,
        "frac05"   ,
        ),
      tmvaSpectators = cms.vstring(
        "jetPt",
        "jetEta",
        "jetPhi"
        ),
      JetIdParams = full_5x_wp,
      label = cms.string("full")
      )
  stdalgos = cms.VPSet(full_53x)
if release in ['53X']:
  from RecoJets.JetProducers.PileupJetID_cfi import *
  stdalgos = cms.VPSet(full_53x,cutbased,PhilV1)
if release in ['42X']:
  from RecoJets.JetProducers.PileupJetID_cfi import *
  stdalgos = cms.VPSet(full,cutbased,PhilV1)

process.puJetMva = cms.EDProducer('PileupJetIdProducer',
    produceJetIds = cms.bool(True),
    jetids = cms.InputTag(""),
    runMvas = cms.bool(True),
    jets = cms.InputTag("ak5PFJets"),
    vertexes = cms.InputTag("offlinePrimaryVertices"),
    algos = cms.VPSet(stdalgos),
    rho     = cms.InputTag("kt6PFJets", "rho"),
    jec     = cms.string("AK5PF"),
    applyJec = cms.bool(True),
    inputIsCorrected = cms.bool(False),
    residualsFromTxt = cms.bool(False),
    residualsTxt     = cms.FileInPath("RecoJets/JetProducers/data/dummy.txt"),
)

if release in ['70X', '70XMINIAOD', '72X', '72XMINIAOD']:
  process.puJetMva.residualsTxt = cms.FileInPath("RecoJets/JetProducers/BuildFile.xml")

# Producer
# --------
process.icPFJetProducer = producers.icPFJetProducer.clone(
    branch                    = cms.string("pfJetsPFlow"),
    input                     = cms.InputTag("ak5PFJets"),
    srcConfig = cms.PSet(
      includeJetFlavour         = cms.bool(True),
      inputJetFlavour           = cms.InputTag("icPFJetFlavourCalculator"),
      applyJECs                 = cms.bool(True),
      includeJECs               = cms.bool(False),
      JECs                      = pfJECS,
      applyCutAfterJECs         = cms.bool(True),
      cutAfterJECs              = cms.string("pt > 15.0"),
      inputSVInfo               = cms.InputTag(""),
      requestSVInfo             = cms.bool(False),
      BTagDiscriminators        = cms.PSet(
        simpleSecondaryVertexHighEffBJetTags = cms.InputTag("simpleSecondaryVertexHighEffBJetTagsAK5PF"),
        simpleSecondaryVertexHighPurBJetTags = cms.InputTag("simpleSecondaryVertexHighPurBJetTagsAK5PF"),
        combinedSecondaryVertexBJetTags      = cms.InputTag("combinedSecondaryVertexBJetTagsAK5PF")
      )
    ),
    destConfig = cms.PSet(
      includePileupID       = cms.bool(True),
      inputPileupID         = cms.InputTag("puJetMva", "fullDiscriminant"),
      includeTrackBasedVars = cms.bool(True),
      inputTracks           = cms.InputTag("generalTracks"),
      inputVertices         = cms.InputTag("offlinePrimaryVertices"),
      requestTracks         = cms.bool(False)
    )
)

if isPhys14:
  process.selectedPFJets = cms.EDFilter("PFJetRefSelector",
      src = cms.InputTag("ak5PFJets"),
      cut = cms.string("pt > 15")
      )
  process.icPFJetProducer.input = cms.InputTag("selectedPFJets")
  process.icPFJetProducer.srcConfig.applyJECs = cms.bool(False)
  process.icPFJetProducer.srcConfig.applyCutAfterJECs = cms.bool(False)

  process.selectedPFJetsAK4 = cms.EDFilter("PFJetRefSelector",
      src = cms.InputTag("ak4PFJets"),
      cut = cms.string("pt > 15")
      )
  process.icPFJetProducerAK4 = producers.icPFJetProducer.clone(
      branch                    = cms.string("ak4PFJets"),
      input                     = cms.InputTag("selectedPFJetsAK4")
  )



if release in ['70XMINIAOD', '72XMINIAOD']:
  process.icPFJetProducer.destConfig.includePileupID = cms.bool(False)
  process.icPFJetProducer.destConfig.includeTrackBasedVars = cms.bool(False)

if release in ['72X']:
  process.icPFJetProducer.destConfig.includePileupID = cms.bool(False)

process.icPFJetSequence = cms.Sequence()

if release in ['70XMINIAOD', '72XMINIAOD']:
  process.icPFJetSequence += cms.Sequence(
    process.ak5PFJets
  )

# 72X: error loading mva file
if release in ['42X', '53X', '70X']:
  process.icPFJetSequence += cms.Sequence(
    process.puJetMva
  )

if release in ['72X', '72XMINIAOD']:
  process.icPFJetProducer.srcConfig.BTagDiscriminators = cms.PSet()
  if isPhys14:
    process.icPFJetSequence += cms.Sequence(
        process.selectedPFJets+
        process.selectedPFJetsAK4+
        process.icPFJetProducerAK4
        )
  process.icPFJetSequence += cms.Sequence(
      process.jetPartons+
      process.pfJetPartonMatches+
      process.pfJetFlavourAssociation+
      process.icPFJetFlavourCalculator+
      process.icPFJetProducer
      )
else:
  if isPhys14:
    process.icPFJetSequence += cms.Sequence(
        process.selectedPFJets
        )
  process.icPFJetSequence += cms.Sequence(
      process.jetPartons+
      process.pfJetPartonMatches+
      process.pfJetFlavourAssociation+
      process.icPFJetFlavourCalculator+
      process.btaggingSequenceAK5PF+
      process.icPFJetProducer
      )



################################################################
# MVA MET and PF MET
################################################################
process.icMvaMetSequence = cms.Sequence()

if release in ['53X']:
  process.load("ICAnalysis.MVAMETPairProducer.mvaPFMET_cff_leptons_53X_Dec2012")
if release in ['42X']:
  process.load("ICAnalysis.MVAMETPairProducer.mvaPFMET_cff_leptons_42X_Dec2012")
  process.pfMEtMVA.srcLeptons = cms.VInputTag("isomuons", "isoelectrons", "isotaus")
  process.pfMEtMVA.useOld42  = cms.bool(False)

if release in ['42X', '53X']:
  if isData:
    process.calibratedAK5PFJetsForPFMEtMVA.correctors = cms.vstring("ak5PFL1FastL2L3Residual")
  else:
    process.calibratedAK5PFJetsForPFMEtMVA.correctors = cms.vstring("ak5PFL1FastL2L3")
  ## don't run the standard MVA MET (crases endJob in 5_3_15, prob. new tau ID)
  process.pfMEtMVAsequence = cms.Sequence(
    process.calibratedAK5PFJetsForPFMEtMVA
  )
  process.mvaMetPairsMT = process.mvaMetPairs.clone(
    srcLeg1   = cms.InputTag('selectedPFMuons'),
    srcLeg2   = cms.InputTag('selectedPFTaus'),
    leg1Pt    = cms.double(7.0),
    leg1Eta   = cms.double(2.6),
    leg2Pt    = cms.double(18.0),
    leg2Eta   = cms.double(2.6),
    minDeltaR = cms.double(0.49)
  )
  process.mvaMetPairsET = process.mvaMetPairs.clone(
    srcLeg1   = cms.InputTag('selectedElectrons'),
    srcLeg2   = cms.InputTag('selectedPFTaus'),
    leg1Pt    = cms.double(10.0),
    leg1Eta   = cms.double(2.6),
    leg2Pt    = cms.double(18.0),
    leg2Eta   = cms.double(2.6),
    minDeltaR = cms.double(0.49)
  )
  process.mvaMetPairsEM = process.mvaMetPairs.clone(
    srcLeg1   = cms.InputTag('selectedElectrons'),
    srcLeg2   = cms.InputTag('selectedPFMuons'),
    leg1Pt    = cms.double(9.5),
    leg1Eta   = cms.double(2.6),
    leg2Pt    = cms.double(9.5),
    leg2Eta   = cms.double(2.6),
    minDeltaR = cms.double(0.29)
  )
  process.pfMEtMVAsequence += process.mvaMetPairsMT
  process.pfMEtMVAsequence += process.mvaMetPairsET
  process.pfMEtMVAsequence += process.mvaMetPairsEM
  process.icMvaMetConcatenate = cms.EDProducer("ICPFMETConcatenate",
    concatenate = cms.VInputTag(
      cms.InputTag("mvaMetPairsMT"),
      cms.InputTag("mvaMetPairsET"),
      cms.InputTag("mvaMetPairsEM")
    )
  )
  process.icMvaMetIDConcatenate = cms.EDProducer("ICIDConcatenate",
    concatenate = cms.VInputTag(
      cms.InputTag("mvaMetPairsMT", "MVAMetId"),
      cms.InputTag("mvaMetPairsET", "MVAMetId"),
      cms.InputTag("mvaMetPairsEM", "MVAMetId")
    )
  )
  process.icMvaMetProducer = producers.icMetProducer.clone(
    branch  = cms.string("pfMVAMetVector"),
    input   = cms.InputTag("icMvaMetConcatenate"),
    includeCustomID = cms.bool(True),
    inputCustomID = cms.InputTag("icMvaMetIDConcatenate")
  )
  process.icMvaMetSequence += cms.Sequence(
    process.pfMEtMVAsequence+
    process.icMvaMetConcatenate+
    process.icMvaMetIDConcatenate+
    process.icMvaMetProducer
  )

process.icPfMetProducer = producers.icSingleMetProducer.clone(
  branch  = cms.string("pfMet"),
  input   = cms.InputTag("pfMet")
)

process.icPfMetType1Producer = producers.icSingleMetProducer.clone(
  branch  = cms.string("pfMetType1"),
  input   = cms.InputTag("pfMetType1")
)

if release in ['42X']:
  #process.load("JetMETCorrections.Type1MET.correctedMet_cff")
  #process.pfMetType1 = process.pfMetT1.clone()
  process.icMvaMetSequence += cms.Sequence(
    process.icPfMetProducer
  )

if release in ['53X']:
  process.load("JetMETCorrections.Type1MET.pfMETCorrections_cff")
  process.pfJetMETcorr.jetCorrLabel = cms.string("ak5PFL1FastL2L3Residual") if isData else cms.string("ak5PFL1FastL2L3")
  process.pfMetType1 = process.pfType1CorrectedMet.clone()
  process.icMvaMetSequence += cms.Sequence(
    process.pfJetMETcorr+
    process.pfMetType1+
    process.icPfMetType1Producer+
    process.icPfMetProducer
  )

# 72X: ak5 JES not defined! Guess we have to switch to ak4 now...
if release in ['70X']:
  process.load("JetMETCorrections.Type1MET.correctionTermsPfMetType1Type2_cff")
  process.load("JetMETCorrections.Type1MET.correctedMet_cff")
  process.corrPfMetType1.jetCorrLabel = cms.string("ak5PFL1FastL2L3Residual") if isData else cms.string("ak5PFL1FastL2L3")
  process.pfMetType1 = process.pfMetT1.clone()
  process.icMvaMetSequence += cms.Sequence(
    process.corrPfMetType1+
    process.pfMetType1+
    process.icPfMetType1Producer+
    process.icPfMetProducer
  )

if release in ['70XMINIAOD', '72XMINIAOD']:
  process.icPfMetType1Producer = producers.icSingleMetProducer.clone(
    branch  = cms.string("pfMetType1"),
    input   = cms.InputTag("slimmedMETs"),
  )
  process.icMvaMetSequence += cms.Sequence(
    process.icPfMetType1Producer
  )


################################################################
# Simulation only: GenParticles, GenJets, PileupInfo
################################################################
process.icGenSequence = cms.Sequence()

process.load("SimGeneral.HepPDTESSource.pythiapdt_cfi")
process.prunedGenParticles = cms.EDProducer("ICGenParticlePruner53X",
  src = cms.InputTag("genParticles", "", "SIM"),
  select = cms.vstring(
    "drop  *",
    "keep status == 3 || status == 22 || status == 23",  # all status 3
    "keep abs(pdgId) == 11 || abs(pdgId) == 13 || abs(pdgId) == 15",  # all charged leptons
    "keep abs(pdgId) == 12 || abs(pdgId) == 14 || abs(pdgId) == 16",  # all neutrinos
    "keep++ abs(pdgId) == 15",  # keep full tau decay chain
    "keep (4 <= abs(pdgId) <= 5)", # keep heavy flavour quarks
    "keep (400 <= abs(pdgId) < 600) || (4000 <= abs(pdgId) < 6000)", # keep b and c hadrons
    "keep abs(pdgId) = 10411 || abs(pdgId) = 10421 || abs(pdgId) = 10413 || abs(pdgId) = 10423 || abs(pdgId) = 20413 || abs(pdgId) = 20423 || abs(pdgId) = 10431 || abs(pdgId) = 10433 || abs(pdgId) = 20433", # additional c hadrons for jet fragmentation studies
    "keep abs(pdgId) = 10511 || abs(pdgId) = 10521 || abs(pdgId) = 10513 || abs(pdgId) = 10523 || abs(pdgId) = 20513 || abs(pdgId) = 20523 || abs(pdgId) = 10531 || abs(pdgId) = 10533 || abs(pdgId) = 20533 || abs(pdgId) = 10541 || abs(pdgId) = 10543 || abs(pdgId) = 20543" # additional b hadrons for jet fragmentation studies
  )
)
if release in ['42X', '72X']:
  process.prunedGenParticles.src = cms.InputTag("genParticles","","HLT")
if release in ['70XMINIAOD', '72XMINIAOD']:
  process.prunedGenParticles.src = cms.InputTag("prunedGenParticles", "", "PAT")

process.icGenParticleProducer = producers.icGenParticleProducer.clone(
  input   = cms.InputTag("prunedGenParticles"),
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
if release in ['70XMINIAOD', '72XMINIAOD']:
  # But of course this won't work because genParticlesForJets(InputGenJetsParticleSelector)
  # requires a vector<GenParticle> input. There's no alternative filter for the PackedGenParticle
  # type at the moment. Probably we could make our own generic cut-string selector, but
  # not in this package
  process.genParticlesForJets.src = cms.InputTag("packedGenParticles")

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

process.icPileupInfoProducer = producers.icPileupInfoProducer.clone()

if not isData:
  process.icGenSequence += (
    process.prunedGenParticles+
    process.icGenParticleProducer+
    process.icPileupInfoProducer
  )
  if release in ['42X', '53X', '70X', '72X']:
    process.icGenSequence += (
      process.genParticlesForJets+
      process.ak5GenJetsNoNuBSM+
      process.selectedGenJets+
      process.icGenJetProducer
    )

if release in ['72X'] and isPhys14:
  process.load("RecoJets.JetProducers.ak4GenJets_cfi")
  process.ak4GenJetsNoNuBSM  =  process.ak4GenJets.clone()
  process.selectedGenJetsAK4 = cms.EDFilter("GenJetRefSelector",
    src = cms.InputTag("ak4GenJetsNoNuBSM"),
    cut = cms.string("pt > 10.0")
  )
  process.icGenJetProducerAK4 = producers.icGenJetProducer.clone(
    branch  = cms.string("ak4GenJets"),
    input   = cms.InputTag("selectedGenJetsAK4"),
    inputGenParticles = cms.InputTag("genParticles"),
    requestGenParticles = cms.bool(False)
  )
  process.icGenSequence += (
      process.ak4GenJetsNoNuBSM+
      process.selectedGenJetsAK4+
      process.icGenJetProducerAK4
  )

################################################################
# Embedding
################################################################
process.icEmbeddingSequence = cms.Sequence()

process.icEmbeddedGenParticleProducer = producers.icGenParticleProducer.clone(
  branch  = cms.string("genParticlesEmbedded"),
  input   = cms.InputTag("genParticles", "", "EmbeddedRECO"),
  includeMothers = cms.bool(True),
  includeDaughters = cms.bool(True)
)

if isEmbedded:
  process.icEmbeddingSequence += process.icEmbeddedGenParticleProducer

################################################################
# Trigger
################################################################
from PhysicsTools.PatAlgos.tools.trigTools import *

if release in ['42X', '53X']:
  process.patTriggerSequence = cms.Sequence()
  switchOnTrigger(process, sequence = 'patTriggerSequence', outputModule = '')
  process.patTriggerPath = cms.Path(process.patTriggerSequence)
if release in ['70X', '72X']:
  process.patTriggerPath = cms.Path()
  switchOnTrigger(process, path = 'patTriggerPath', outputModule = '')
if release in ['70XMINIAOD', '72XMINIAOD']:
  process.patTriggerPath = cms.Path()

process.icTriggerPathProducer = producers.icTriggerPathProducer.clone(
  input   = cms.InputTag("patTriggerEvent"),
  includeAcceptedOnly = cms.bool(True),
  saveStrings = cms.bool(True),
  splitVersion = cms.bool(False)
)
process.icTriggerSequence = cms.Sequence()
if isData:
  process.icTriggerSequence += cms.Sequence(
    process.icTriggerPathProducer
  )

notTp = not isTandP
# Define 2011 et,mt,em physics triggers for data
if (release == '42X' and isData):
  process.icEle15LooseTau15ObjectProducer = producers.icTriggerObjectProducer.clone(
      branch = cms.string("triggerObjectsEle15LooseTau15"),
      hltPath = cms.string("HLT_Ele15_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_LooseIsoPFTau15_v"),
      storeOnlyIfFired = cms.bool(notTp)
      )
  process.icEle15TightTau20ObjectProducer = producers.icTriggerObjectProducer.clone(
      branch = cms.string("triggerObjectsEle15TightTau20"),
      hltPath = cms.string("HLT_Ele15_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_TightIsoPFTau20_v"),
      storeOnlyIfFired = cms.bool(notTp)
      )
  process.icEle15LooseTau20ObjectProducer = producers.icTriggerObjectProducer.clone(
      branch = cms.string("triggerObjectsEle15LooseTau20"),
      hltPath = cms.string("HLT_Ele15_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_LooseIsoPFTau20_v"),
      storeOnlyIfFired = cms.bool(notTp)
      )
  process.icEle18MediumTau20ObjectProducer = producers.icTriggerObjectProducer.clone(
      branch = cms.string("triggerObjectsEle18MediumTau20"),
      hltPath = cms.string("HLT_Ele18_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_MediumIsoPFTau20_v"),
      storeOnlyIfFired = cms.bool(notTp)
      )
  process.icEle20MediumTau20ObjectProducer = producers.icTriggerObjectProducer.clone(
      branch = cms.string("triggerObjectsEle20MediumTau20"),
      hltPath = cms.string("HLT_Ele20_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_MediumIsoPFTau20_v"),
      storeOnlyIfFired = cms.bool(notTp)
      )
  process.icIsoMu12LooseTau10ObjectProducer = producers.icTriggerObjectProducer.clone(
      branch = cms.string("triggerObjectsIsoMu12LooseTau10"),
      hltPath = cms.string("HLT_IsoMu12_LooseIsoPFTau10_v"),
      storeOnlyIfFired = cms.bool(notTp)
      )
  process.icIsoMu15LooseTau15ObjectProducer = producers.icTriggerObjectProducer.clone(
      branch = cms.string("triggerObjectsIsoMu15LooseTau15"),
      hltPath = cms.string("HLT_IsoMu15_LooseIsoPFTau15_v"),
      storeOnlyIfFired = cms.bool(notTp)
      )
  process.icIsoMu15LooseTau20ObjectProducer = producers.icTriggerObjectProducer.clone(
      branch = cms.string("triggerObjectsIsoMu15LooseTau20"),
      hltPath = cms.string("HLT_IsoMu15_eta2p1_LooseIsoPFTau20_v"),
      storeOnlyIfFired = cms.bool(notTp)
      )
  process.icMu8Ele17ObjectProducer = producers.icTriggerObjectProducer.clone(
      branch = cms.string("triggerObjectsMu8Ele17"),
      hltPath = cms.string("HLT_Mu8_Ele17_CaloIdT_CaloIsoVL_v"),
      storeOnlyIfFired = cms.bool(notTp)
      )
  process.icMu17Ele8ObjectProducer = producers.icTriggerObjectProducer.clone(
      branch = cms.string("triggerObjectsMu17Ele8"),
      hltPath = cms.string("HLT_Mu17_Ele8_CaloIdT_CaloIsoVL_v"),
      storeOnlyIfFired = cms.bool(notTp)
      )
  process.icMu8Ele17IdLObjectProducer = producers.icTriggerObjectProducer.clone(
      branch = cms.string("triggerObjectsMu8Ele17IdL"),
      hltPath = cms.string("HLT_Mu8_Ele17_CaloIdL_v"),
      storeOnlyIfFired = cms.bool(notTp)
      )
  process.icMu17Ele8IdLObjectProducer = producers.icTriggerObjectProducer.clone(
      branch = cms.string("triggerObjectsMu17Ele8IdL"),
      hltPath = cms.string("HLT_Mu17_Ele8_CaloIdL_v"),
      storeOnlyIfFired = cms.bool(notTp)
      )
  process.icTriggerSequence += (
    process.icEle15LooseTau15ObjectProducer
    +process.icEle15TightTau20ObjectProducer
    +process.icEle15LooseTau20ObjectProducer
    +process.icEle18MediumTau20ObjectProducer
    +process.icEle20MediumTau20ObjectProducer
    +process.icIsoMu12LooseTau10ObjectProducer
    +process.icIsoMu15LooseTau15ObjectProducer
    +process.icIsoMu15LooseTau20ObjectProducer
    +process.icMu8Ele17IdLObjectProducer
    +process.icMu17Ele8IdLObjectProducer
    +process.icMu8Ele17ObjectProducer
    +process.icMu17Ele8ObjectProducer
    )

# Define 2011 et,mt,em physics triggers for mc
if (release == '42X' and not isData):
  process.icIsoMu15LooseTau15ObjectProducer = producers.icTriggerObjectProducer.clone(
      branch = cms.string("triggerObjectsIsoMu15LooseTau15"),
      hltPath = cms.string("HLT_IsoMu15_LooseIsoPFTau15_v"),
      storeOnlyIfFired = cms.bool(notTp)
      )
  process.icEle18MediumTau20ObjectProducer = producers.icTriggerObjectProducer.clone(
      branch = cms.string("triggerObjectsEle18MediumTau20"),
      hltPath = cms.string("HLT_Ele18_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_MediumIsoPFTau20_v"),
      storeOnlyIfFired = cms.bool(notTp)
      )
  process.icMu8Ele17ObjectProducer = producers.icTriggerObjectProducer.clone(
      branch = cms.string("triggerObjectsMu8Ele17"),
      hltPath = cms.string("HLT_Mu8_Ele17_CaloIdT_CaloIsoVL_v"),
      storeOnlyIfFired = cms.bool(notTp)
      )
  process.icMu17Ele8ObjectProducer = producers.icTriggerObjectProducer.clone(
      branch = cms.string("triggerObjectsMu17Ele8"),
      hltPath = cms.string("HLT_Mu17_Ele8_CaloIdT_CaloIsoVL_v"),
      storeOnlyIfFired = cms.bool(notTp)
      )
  process.icTriggerSequence += (
    process.icIsoMu15LooseTau15ObjectProducer
    +process.icEle18MediumTau20ObjectProducer
    +process.icMu8Ele17ObjectProducer
    +process.icMu17Ele8ObjectProducer
    )

# Define 2012 et,mt,em physics triggers for data
if (release == '53X' and isData):
  process.icEle20RhoLooseTau20ObjectProducer = producers.icTriggerObjectProducer.clone(
      branch = cms.string("triggerObjectsEle20RhoLooseTau20"),
      hltPath = cms.string("HLT_Ele20_CaloIdVT_CaloIsoRhoT_TrkIdT_TrkIsoT_LooseIsoPFTau20_v"),
      storeOnlyIfFired = cms.bool(notTp)
      )
  process.icEle22WP90RhoLooseTau20ObjectProducer = producers.icTriggerObjectProducer.clone(
      branch = cms.string("triggerObjectsEle22WP90RhoLooseTau20"),
      hltPath = cms.string("HLT_Ele22_eta2p1_WP90Rho_LooseIsoPFTau20_v"),
      storeOnlyIfFired = cms.bool(notTp)
      )
  process.icIsoMu18LooseTau20ObjectProducer = producers.icTriggerObjectProducer.clone(
      branch = cms.string("triggerObjectsIsoMu18LooseTau20"),
      hltPath = cms.string("HLT_IsoMu18_eta2p1_LooseIsoPFTau20_v"),
      storeOnlyIfFired = cms.bool(notTp)
      )
  process.icIsoMu17LooseTau20ObjectProducer = producers.icTriggerObjectProducer.clone(
      branch = cms.string("triggerObjectsIsoMu17LooseTau20"),
      hltPath = cms.string("HLT_IsoMu17_eta2p1_LooseIsoPFTau20_v"),
      storeOnlyIfFired = cms.bool(notTp)
      )
  process.icMu8Ele17ObjectProducer = producers.icTriggerObjectProducer.clone(
      branch = cms.string("triggerObjectsMu8Ele17"),
      hltPath = cms.string("HLT_Mu8_Ele17_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL_v"),
      storeOnlyIfFired = cms.bool(notTp)
      )
  process.icMu17Ele8ObjectProducer = producers.icTriggerObjectProducer.clone(
      branch = cms.string("triggerObjectsMu17Ele8"),
      hltPath = cms.string("HLT_Mu17_Ele8_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL_v"),
      storeOnlyIfFired = cms.bool(notTp)
      )
  process.icTriggerSequence += (
    process.icEle20RhoLooseTau20ObjectProducer
    +process.icEle22WP90RhoLooseTau20ObjectProducer
    +process.icIsoMu18LooseTau20ObjectProducer
    +process.icIsoMu17LooseTau20ObjectProducer
    +process.icMu8Ele17ObjectProducer
    +process.icMu17Ele8ObjectProducer
    )

# Define 2012 et,mt,em physics triggers for mc
if (release in ['53X', '70X', '72X'] and not isData):
  process.icEle22WP90RhoLooseTau20ObjectProducer = producers.icTriggerObjectProducer.clone(
      branch = cms.string("triggerObjectsEle22WP90RhoLooseTau20"),
      hltPath = cms.string("HLT_Ele22_eta2p1_WP90Rho_LooseIsoPFTau20_v"),
      storeOnlyIfFired = cms.bool(notTp)
      )
  process.icIsoMu17LooseTau20ObjectProducer = producers.icTriggerObjectProducer.clone(
      branch = cms.string("triggerObjectsIsoMu17LooseTau20"),
      hltPath = cms.string("HLT_IsoMu17_eta2p1_LooseIsoPFTau20_v"),
      storeOnlyIfFired = cms.bool(notTp)
      )
  process.icMu8Ele17ObjectProducer = producers.icTriggerObjectProducer.clone(
      branch = cms.string("triggerObjectsMu8Ele17"),
      hltPath = cms.string("HLT_Mu8_Ele17_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL_v"),
      storeOnlyIfFired = cms.bool(notTp)
      )
  process.icMu17Ele8ObjectProducer = producers.icTriggerObjectProducer.clone(
      branch = cms.string("triggerObjectsMu17Ele8"),
      hltPath = cms.string("HLT_Mu17_Ele8_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL_v"),
      storeOnlyIfFired = cms.bool(notTp)
      )
  process.icMu8ObjectProducer = producers.icTriggerObjectProducer.clone(
      branch = cms.string("triggerObjectsMu8"),
      hltPath = cms.string("HLT_Mu8_v"),
      storeOnlyIfFired = cms.bool(notTp)
      )
  process.icEle8ObjectProducer = producers.icTriggerObjectProducer.clone(
      branch = cms.string("triggerObjectsEle8"),
      hltPath = cms.string("HLT_Ele8_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL_v"),
      storeOnlyIfFired = cms.bool(notTp)
      )
  process.icIsoMu24ObjectProducer = producers.icTriggerObjectProducer.clone(
      branch = cms.string("triggerObjectsIsoMu24"),
      hltPath = cms.string("HLT_IsoMu24_eta2p1_v"),
      storeOnlyIfFired = cms.bool(True)
      )
  process.icTriggerSequence += (
    process.icEle22WP90RhoLooseTau20ObjectProducer
    +process.icIsoMu17LooseTau20ObjectProducer
    +process.icMu8Ele17ObjectProducer
    +process.icMu17Ele8ObjectProducer
    +process.icMu8ObjectProducer
    +process.icEle8ObjectProducer
    +process.icIsoMu24ObjectProducer
    )

# Define 2011 t+p triggers for data
if (release == '42X' and isData and isTandP):
  process.icIsoMu24ObjectProducer = producers.icTriggerObjectProducer.clone(
      branch = cms.string("triggerObjectsIsoMu24"),
      hltPath = cms.string("HLT_IsoMu24_v"),
      storeOnlyIfFired = cms.bool(True)
      )
  process.icEle17Ele8ObjectProducer = producers.icTriggerObjectProducer.clone(
      branch = cms.string("triggerObjectsEle17Ele8"),
      hltPath = cms.string("HLT_Ele17_CaloIdVT_CaloIsoVT_TrkIdT_TrkIsoVT_Ele8_Mass30_v"),
      storeOnlyIfFired = cms.bool(True)
      )
  process.icEle17SC8ObjectProducer = producers.icTriggerObjectProducer.clone(
      branch = cms.string("triggerObjectsEle17SC8"),
      hltPath = cms.string("HLT_Ele17_CaloIdVT_CaloIsoVT_TrkIdT_TrkIsoVT_SC8_Mass30_v"),
      storeOnlyIfFired = cms.bool(True)
      )
  process.icTriggerSequence += (
    process.icIsoMu24ObjectProducer
    +process.icEle17Ele8ObjectProducer
    +process.icEle17SC8ObjectProducer
    )

# Define 2012 t+p triggers for data
if (release == '53X' and isData and isTandP):
  process.icIsoMu24ObjectProducer = producers.icTriggerObjectProducer.clone(
      branch = cms.string("triggerObjectsIsoMu24"),
      hltPath = cms.string("HLT_IsoMu24_eta2p1_v"),
      storeOnlyIfFired = cms.bool(True)
      )
  process.icMu17TkMu8ObjectProducer = producers.icTriggerObjectProducer.clone(
      branch = cms.string("triggerObjectsMu17TkMu8"),
      hltPath = cms.string("HLT_Mu17_TkMu8_v"),
      storeOnlyIfFired = cms.bool(True)
      )
  process.icEle27ObjectProducer = producers.icTriggerObjectProducer.clone(
      branch = cms.string("triggerObjectsEle27"),
      hltPath = cms.string("HLT_Ele27_WP80_v"),
      storeOnlyIfFired = cms.bool(True)
      )
  process.icEle17Ele8Mass50ObjectProducer = producers.icTriggerObjectProducer.clone(
      branch = cms.string("triggerObjectsEle17Ele8Mass50"),
      hltPath = cms.string("HLT_Ele17_CaloIdVT_CaloIsoVT_TrkIdT_TrkIsoVT_Ele8_Mass50_v"),
      storeOnlyIfFired = cms.bool(True)
      )
  process.icEle20SC4Mass50ObjectProducer = producers.icTriggerObjectProducer.clone(
      branch = cms.string("triggerObjectsEle20SC4Mass50"),
      hltPath = cms.string("HLT_Ele20_CaloIdVT_CaloIsoVT_TrkIdT_TrkIsoVT_SC4_Mass50_v"),
      storeOnlyIfFired = cms.bool(True)
      )
  process.icTriggerSequence += (
    process.icIsoMu24ObjectProducer
    +process.icMu17TkMu8ObjectProducer
    +process.icEle27ObjectProducer
    +process.icEle17Ele8Mass50ObjectProducer
    +process.icEle20SC4Mass50ObjectProducer
    )

# Define 2012 mm,ee physics triggers for data
if (release == '53X' and isData and isZStudy):
  process.icEle17Ele8ObjectProducer = producers.icTriggerObjectProducer.clone(
      branch = cms.string("triggerObjectsEle17Ele8"),
      hltPath = cms.string("HLT_Ele17_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL_Ele8_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL_v"),
      storeOnlyIfFired = cms.bool(notTp)
      )
  process.icMu17Mu8ObjectProducer = producers.icTriggerObjectProducer.clone(
      branch = cms.string("triggerObjectsMu17Mu8"),
      hltPath = cms.string("HLT_Mu17_Mu8_v"),
      storeOnlyIfFired = cms.bool(notTp)
      )
  process.icTriggerSequence += (
    process.icEle17Ele8ObjectProducer
    +process.icMu17Mu8ObjectProducer
    )

# Define 2012 mm,ee physics triggers for mc
if (release == '53X' and (not isData) and isZStudy):
  process.icEle17Ele8ObjectProducer = producers.icTriggerObjectProducer.clone(
      branch = cms.string("triggerObjectsEle17Ele8"),
      hltPath = cms.string("HLT_Ele17_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL_Ele8_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL_v"),
      storeOnlyIfFired = cms.bool(notTp)
      )
  process.icMu17Mu8ObjectProducer = producers.icTriggerObjectProducer.clone(
      branch = cms.string("triggerObjectsMu17Mu8"),
      hltPath = cms.string("HLT_Mu17_Mu8_v"),
      storeOnlyIfFired = cms.bool(notTp)
      )
  process.icTriggerSequence += (
    process.icEle17Ele8ObjectProducer
    +process.icMu17Mu8ObjectProducer
    )


################################################################
# EventInfo
################################################################
process.icEventInfoProducer = producers.icEventInfoProducer.clone(
  includeJetRho       = cms.bool(True),
  inputJetRho         = cms.InputTag("kt6PFJets", "rho"),
  includeLeptonRho    = cms.bool(False),
  inputLeptonRho      = cms.InputTag("kt6PFJets", "rho"),
  includeVertexCount  = cms.bool(True),
  inputVertices       = cms.InputTag("selectedVertices"),
  includeCSCFilter    = cms.bool(False),
  inputCSCFilter      = cms.InputTag("BeamHaloSummary"),
)

if isEmbedded and release == '53X':
  process.icEventInfoProducer.genFilterWeights.append(
    embed_weight = cms.InputTag("generator", "minVisPtFilter", "EmbeddedRECO")
  )
if isEmbedded and release == '42X':
  process.icEventInfoProducer.weights.append(
    embed_weight = cms.InputTag("generator", "weight", "EmbeddedRECO")
)

process.icEventInfoSequence = cms.Sequence(
  process.icEventInfoProducer
)


################################################################
# Event
################################################################
process.icEventProducer = producers.icEventProducer.clone()



process.p = cms.Path(
  process.ic42XSequence+
  process.ic53XSequence+
  process.ic70XSequence+
  process.ic72XSequence+
  process.icMiniAODSequence+
  process.icSelectionSequence+
  process.pfParticleSelectionSequence+
  process.icVertexSequence+
  process.icPFSequence+
  process.icElectronSequence+
  process.icMuonSequence+
  process.icTauProducer+
  process.icTrackSequence+
  process.icMvaMetSequence+
  process.icPFJetSequence+
  process.icGenSequence+
  process.icTriggerSequence+
  process.icEmbeddingSequence+
  process.icEventInfoSequence+
  process.icEventProducer
)

process.schedule = cms.Schedule(process.patTriggerPath, process.p)

#print process.dumpPython()
