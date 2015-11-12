import FWCore.ParameterSet.Config as cms
process = cms.Process("MAIN")
import sys

################################################################
# Read Options
################################################################
import FWCore.ParameterSet.VarParsing as parser
opts = parser.VarParsing ('analysis')
opts.register('isData', 0, parser.VarParsing.multiplicity.singleton,
    parser.VarParsing.varType.int, "Process as data?")
opts.register('file', 0, parser.VarParsing.multiplicity.singleton,
    parser.VarParsing.varType.string, "input file")
opts.register('globalTag', 0, parser.VarParsing.multiplicity.singleton,
    parser.VarParsing.varType.string, "global tag")
opts.register('release', '', parser.VarParsing.multiplicity.singleton,
    parser.VarParsing.varType.string, "Release label")
opts.parseArguments()
infile      = opts.file
tag         = opts.globalTag
isData      = opts.isData
release     = opts.release
if not release in ["42X", "53X", "70X", "72X"]:
    print 'Release not recognised, exiting!'
    sys.exit(1)
print 'release     : '+release
print 'isData      : '+str(isData)
print 'globalTag   : '+tag
print 'file        : '+infile

################################################################
# Standard setup
################################################################
process.load("FWCore.MessageLogger.MessageLogger_cfi")
if release in ['42X']:
  process.load("Configuration.StandardSequences.Geometry_cff")
if release in ['53X', '70X', '72X']:
  process.load("Configuration.Geometry.GeometryIdeal_cff")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.load("Configuration.StandardSequences.MagneticField_cff")

process.MessageLogger.cerr.FwkReport.reportEvery = 50
process.source = cms.Source("PoolSource", fileNames = cms.untracked.vstring(infile))
# 53X: START53_V22::All (MC)
# 70X: PLS170_V7AN1::All (MC)
# 72X: globalTag=START72_V1::All file=root://xrootd.unl.edu//store/mc/Phys14DR/VBF_HToTauTau_M-125_13TeV-powheg-pythia6/AODSIM/PU40bx25_PHYS14_25_V1-v1/00000/00E63918-3A70-E411-A246-7845C4FC35F3.root
process.GlobalTag.globaltag = cms.string(tag)
process.options   = cms.untracked.PSet( wantSummary = cms.untracked.bool(True) )
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(500) )

process.TFileService = cms.Service("TFileService",
  fileName = cms.string("EventTree.root"),
  closeFileFast = cms.untracked.bool(True)
)

process.ic42XExtraSequence = cms.Sequence()
process.ic53XExtraSequence = cms.Sequence()
process.ic70XExtraSequence = cms.Sequence()
process.ic72XExtraSequence = cms.Sequence()
# ##############################################################################
# ## Jet Parton Flavour
# ##############################################################################
# # This is the default Run1 configuration "Algorithmic" used by b-tag POG.
# # Also what is done by PAT. New recipe and details for Run 2:
# # https://twiki.cern.ch/twiki/bin/view/CMSPublic/SWGuideBTagMCTools#Legacy_jet_flavour_definition
# # See PAT implementation here:
# # https://github.com/cms-sw/cmssw/blob/CMSSW_5_3_X/PhysicsTools/PatAlgos/plugins/PATJetProducer.cc#L296-L298

import UserCode.ICHiggsTauTau.default_producers_cfi as producers

##############################################################################
# Candidate Module
##############################################################################
process.selectedPFCandidates = cms.EDFilter("GenericPFCandidateSelector",
  src = cms.InputTag("particleFlow"),
  cut = cms.string("pt > 15")
)

process.icCandidateProducer = producers.icCandidateProducer.clone(
  branch  = cms.string("pfCandidates"),
  input   = cms.InputTag("selectedPFCandidates")
)

##############################################################################
# Electron Module
##############################################################################
process.selectedElectrons = cms.EDFilter("GsfElectronRefSelector",
  src = cms.InputTag("gsfElectrons"),
  cut = cms.string("pt > 10 & abs(eta) < 2.5")
)

if release in ['70X', '72X']:
  process.selectedElectrons.src = cms.InputTag("gedGsfElectrons")

process.icElectronProducer = producers.icElectronProducer.clone(
    branch                    = cms.string("electrons"),
    input                     = cms.InputTag("selectedElectrons")
)

##############################################################################
# Reco Muon Module
##############################################################################
process.selectedMuons = cms.EDFilter("MuonRefSelector",
  src = cms.InputTag("muons"),
  cut = cms.string("pt > 10 & abs(eta) < 2.4")
)

process.icMuonProducer = producers.icMuonProducer.clone(
    branch                    = cms.string("muons"),
    input                     = cms.InputTag("selectedMuons")
)

##############################################################################
# PF Muon Module
##############################################################################
process.selectedPFMuons = cms.EDFilter("GenericPFCandidateSelector",
  src = cms.InputTag("particleFlow"),
  cut = cms.string("pt > 10 & abs(eta) < 2.4 & abs(pdgId) == 13")
)

process.icPFMuonProducer = producers.icMuonProducer.clone(
  branch = cms.string("pfMuons"),
  input  = cms.InputTag("selectedPFMuons"),
  isPF   = cms.bool(True)
)

##############################################################################
# Tau Module
##############################################################################
process.selectedPFTaus = cms.EDFilter("PFTauRefSelector",
  src = cms.InputTag("hpsPFTauProducer"),
  cut = cms.string("pt > 18 & decayMode > -0.5")
)

process.icTauProducer = producers.icTauProducer.clone(
  branch = cms.string("taus"),
  input  = cms.InputTag("selectedPFTaus")
)

##############################################################################
# Photon Modules
##############################################################################
process.selectedPhotons = cms.EDFilter("PhotonRefSelector",
  src = cms.InputTag("photons"),
  cut = cms.string("pt > 10 & abs(eta) < 2.5")
)

# process.icPhotonHadTowerOverEmCalculator = cms.EDProducer('ICPhotonHadTowerOverEmCalculator',
#     input = cms.InputTag("photons")
# )

# process.icPhotonElectronVetoCalculator = cms.EDProducer('ICPhotonElectronVetoCalculator',
#     input       = cms.InputTag("photons"),
#     electrons   = cms.InputTag("gsfElectrons"),
#     beamspot    = cms.InputTag("offlineBeamSpot"),
#     conversions = cms.InputTag("allConversions")
# )

process.icPhotonProducer = producers.icPhotonProducer.clone(
  branch = cms.string("selectedPhotons"),
  input  = cms.InputTag("photons")
)

##############################################################################
# CaloJet Module
##############################################################################
process.selectedCaloJets = cms.EDFilter("CaloJetRefSelector",
  src = cms.InputTag("ak5CaloJets"),
  cut = cms.string("pt > 20 & abs(eta) < 5.0")
)

process.icCaloJetProducer = producers.icCaloJetProducer.clone(
  branch = cms.string("caloJets"),
  input  = cms.InputTag("selectedCaloJets")
)

##############################################################################
# JPT Jet Module
##############################################################################
process.selectedJPTJets = cms.EDFilter("JPTJetRefSelector",
  src = cms.InputTag("JetPlusTrackZSPCorJetAntiKt5"),
  cut = cms.string("pt > 20 & abs(eta) < 5.0")
)

process.icJPTJetProducer = producers.icJPTJetProducer.clone(
  branch = cms.string("jptJets"),
  input  = cms.InputTag("selectedJPTJets")
)

##############################################################################
# PF Jet Module
##############################################################################
# re-run ak5PFJets because the jetArea() not available in 42X inputs
if release in ['42X']:
  process.load("RecoJets.JetProducers.ak5PFJets_cfi")
  process.ak5PFJets.doAreaFastjet = cms.bool(True)
  process.ic42XExtraSequence += process.ak5PFJets

process.selectedPFJets = cms.EDFilter("PFJetRefSelector",
  src = cms.InputTag("ak5PFJets"),
  cut = cms.string("pt > 20 & abs(eta) < 5.0")
)

process.icPFJetProducer = producers.icPFJetProducer.clone(
  branch = cms.string("pfJets"),
  input  = cms.InputTag("selectedPFJets")
)

##############################################################################
# MET Modules
##############################################################################
process.icPfMetProducer = producers.icMetProducer.clone(
  branch  = cms.string("pfMet"),
  input   = cms.InputTag("pfMet"),
)

##############################################################################
# Track Module
##############################################################################
process.selectedTracks = cms.EDFilter("TrackRefSelector",
  src = cms.InputTag("generalTracks"),
  cut = cms.string("pt > 5")
)

process.icTrackProducer = producers.icTrackProducer.clone(
  branch = cms.string("tracks"),
  input  = cms.InputTag("selectedTracks")
)

##############################################################################
# Vertex Module
##############################################################################
process.selectedVertices = cms.EDFilter("VertexRefSelector",
  src = cms.InputTag("offlinePrimaryVertices"),
  cut = cms.string("ndof >= 4 & abs(z) <= 24 & abs(position.Rho) <= 2")
)

process.icVertexProducer = producers.icVertexProducer.clone(
  branch = cms.string("vertices"),
  input  = cms.InputTag("offlinePrimaryVertices")
)

##############################################################################
# Supercluster Module
##############################################################################
process.selectedSuperClustersBarrel = cms.EDFilter("SuperClusterRefSelector",
  src = cms.InputTag("correctedHybridSuperClusters"),
  cut = cms.string("energy > 20")
)

process.selectedSuperClustersEndcap = cms.EDFilter("SuperClusterRefSelector",
  src = cms.InputTag("correctedMulti5x5SuperClustersWithPreshower"),
  cut = cms.string("energy > 20")
)

process.icSuperClusterProducer = producers.icSuperClusterProducer.clone(
  branch      = cms.string("superClusters"),
  inputBarrel = cms.InputTag("selectedSuperClustersBarrel"),
  inputEndcap = cms.InputTag("selectedSuperClustersEndcap")
)

# ##############################################################################
# # Secondary Vertex Module
# ##############################################################################
# process.icSecondaryVertexProducer = cms.EDProducer('ICSecondaryVertexProducer',
#   branch  = cms.string("secondaryVertices"),
#   input   = cms.InputTag("icPFJetProducer", "requestedSVInfo"),
#   trackPtThreshold = cms.double(0.0),
#   requestTracks = cms.bool(True)
# )

##############################################################################
# PileupInfo Module
##############################################################################
process.icPileupInfoProducer = producers.icPileupInfoProducer.clone(
  branch = cms.string("pileupInfo"),
  input  = cms.InputTag("addPileupInfo")
)

##############################################################################
# GenParticle Module
##############################################################################
process.load("SimGeneral.HepPDTESSource.pythiapdt_cfi")
process.selectedGenParticles = cms.EDProducer("ICGenParticlePruner53X",
  src = cms.InputTag("genParticles"),
  select = cms.vstring(
    "drop  *",
    "keep status == 3 || status == 22 || status == 23",  # all status 3
    "keep abs(pdgId) == 11 || abs(pdgId) == 13 || abs(pdgId) == 15",  # all electrons and muons
    "keep abs(pdgId) == 12 || abs(pdgId) == 14 || abs(pdgId) == 16",  # keep neutrinos
    "keep++ abs(pdgId) == 15",                                        # but keep keep taus with decays
    "keep (4 <= abs(pdgId) <= 5)", # keep heavy flavour quarks for parton-based jet flavour
    "keep (400 <= abs(pdgId) < 600) || (4000 <= abs(pdgId) < 6000)", # keep b and c hadrons for hadron-based jet flavour
    "keep abs(pdgId) = 10411 || abs(pdgId) = 10421 || abs(pdgId) = 10413 || abs(pdgId) = 10423 || abs(pdgId) = 20413 || abs(pdgId) = 20423 || abs(pdgId) = 10431 || abs(pdgId) = 10433 || abs(pdgId) = 20433", # additional c hadrons for jet fragmentation studies
    "keep abs(pdgId) = 10511 || abs(pdgId) = 10521 || abs(pdgId) = 10513 || abs(pdgId) = 10523 || abs(pdgId) = 20513 || abs(pdgId) = 20523 || abs(pdgId) = 10531 || abs(pdgId) = 10533 || abs(pdgId) = 20533 || abs(pdgId) = 10541 || abs(pdgId) = 10543 || abs(pdgId) = 20543" # additional b hadrons for jet fragmentation studies
  )
)

process.icGenParticleProducer = producers.icGenParticleProducer.clone(
  branch = cms.string("genParticles"),
  input  = cms.InputTag("selectedGenParticles")
)

##############################################################################
# GenJet Module
##############################################################################
process.selectedGenJets = cms.EDFilter("GenJetRefSelector",
  src = cms.InputTag("ak5GenJets"),
  cut = cms.string("pt > 15")
)

process.icGenJetProducer = producers.icGenJetProducer.clone(
  branch  = cms.string("genJets"),
  input   = cms.InputTag("selectedGenJets")
)

##############################################################################
# GenMet Module
##############################################################################
process.icGenMetProducer = producers.icSingleMetProducer.clone(
  branch = cms.string("genMet"),
  input  = cms.InputTag("genMetTrue")
)

process.icMCSequence = cms.Sequence()
if not isData:
  process.icMCSequence += (
    process.icPileupInfoProducer+
    process.selectedGenParticles+
    process.icGenParticleProducer+
    process.selectedGenJets+
    process.icGenJetProducer+
    process.icGenMetProducer
  )

##############################################################################
# TriggerPath Modules
##############################################################################
from PhysicsTools.PatAlgos.tools.trigTools import *
if release in ['42X', '53X']:
  process.patTriggerSequence = cms.Sequence()
  switchOnTrigger(process, sequence = 'patTriggerSequence', outputModule = '')
  process.patTriggerPath = cms.Path(process.patTriggerSequence)
if release in ['70X', '72X']:
  process.patTriggerPath = cms.Path()
  switchOnTrigger(process, path = 'patTriggerPath', outputModule = '')

process.icTriggerPathProducer = producers.icTriggerPathProducer.clone(
  branch = cms.string("triggerPaths"),
  input  = cms.InputTag("patTriggerEvent")
)

##############################################################################
# TriggerObject Module
##############################################################################
process.icIsoMu24ObjectProducer = producers.icTriggerObjectProducer.clone(
  branch = cms.string("triggerObjectsIsoMu24"),
  input   = cms.InputTag("patTriggerEvent"),
  hltPath = cms.string("HLT_IsoMu24_eta2p1_v")
)

##############################################################################
# EventInfo Module
##############################################################################
process.icEventInfoProducer = producers.icEventInfoProducer.clone(
  branch = cms.string("eventInfo")
)

##############################################################################
# Event Module
##############################################################################
process.icEventProducer = cms.EDProducer('ICEventProducer')

process.p = cms.Path(
  process.ic42XExtraSequence+
  process.ic53XExtraSequence+
  process.ic70XExtraSequence+
  process.ic72XExtraSequence+
  process.selectedPFCandidates+
  process.icCandidateProducer+
  process.selectedElectrons+
  process.icElectronProducer+
  process.selectedMuons+
  process.icMuonProducer+
  process.selectedPFMuons+
  process.icPFMuonProducer+
  process.selectedPFTaus+
  process.icTauProducer+
  process.selectedPhotons+
  process.icPhotonProducer+
  process.selectedCaloJets+
  process.icCaloJetProducer+
  process.selectedJPTJets+
  process.icJPTJetProducer+
  process.selectedPFJets+
  process.icPFJetProducer+
  process.icPfMetProducer+
  process.selectedTracks+
  process.icTrackProducer+
  process.selectedVertices+
  process.icVertexProducer+
  process.selectedSuperClustersBarrel+
  process.selectedSuperClustersEndcap+
  process.icSuperClusterProducer+
  process.icMCSequence+
  process.icTriggerPathProducer+
  process.icIsoMu24ObjectProducer+
  process.icEventInfoProducer+
  process.icEventProducer
  )

# JPT jets no longer in AOD from 7_2_X
if release in ['72X']:
  process.p.remove(process.selectedJPTJets)
  process.p.remove(process.icJPTJetProducer)

process.schedule = cms.Schedule(process.patTriggerPath, process.p)

#print process.dumpPython()
