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
if not release in ["72X", "72XMINIAOD"]:
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
if release in ['70X']:
  process.load("Configuration.Geometry.GeometryIdeal_cff")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.load("Configuration.StandardSequences.MagneticField_cff")

process.MessageLogger.cerr.FwkReport.reportEvery = 1
process.source = cms.Source("PoolSource", fileNames = cms.untracked.vstring(infile))
# 53X: START53_V22::All (MC)
# 70X: PLS170_V7AN1::All (MC)
# or START70_V6
process.GlobalTag.globaltag = cms.string(tag)
process.options   = cms.untracked.PSet( wantSummary = cms.untracked.bool(True) )
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(500) )

process.TFileService = cms.Service("TFileService",
  fileName = cms.string("EventTree.root"),
  closeFileFast = cms.untracked.bool(True)
)

process.ic70XExtraSequence = cms.Sequence()
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
process.icCandidateProducer = producers.icCandidateProducer.clone(
  branch  = cms.string("pfCandidates"),
  input   = cms.InputTag("packedPFCandidates")
)

##############################################################################
# Vertex Module
##############################################################################
process.selectedVertices = cms.EDFilter("VertexRefSelector",
  src = cms.InputTag("offlineSlimmedPrimaryVertices"),
  cut = cms.string("ndof >= 4 & abs(z) <= 24 & abs(position.Rho) <= 2")
)

process.icVertexProducer = producers.icVertexProducer.clone(
  branch = cms.string("vertices"),
  input  = cms.InputTag("selectedVertices")
)

##############################################################################
# PF Isolation for MiniAOD 
##############################################################################
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

process.pfIsoMiniAODSequence = cms.Sequence(
    process.pfPileUp+
    process.pfNoPileUp+
    process.pfAllNeutralHadrons+
    process.pfAllChargedHadrons+
    process.pfAllPhotons+
    process.pfAllChargedParticles+
    process.pfPileUpAllChargedParticles+
    process.pfAllNeutralHadronsAndPhotons
    )

##############################################################################
# Electron Module
##############################################################################
process.load("CommonTools.ParticleFlow.Isolation.pfElectronIsolation_cff")
process.electronPFIsolationValuesSequence = cms.Sequence(
    process.elPFIsoValueCharged04PFId+
    process.elPFIsoValueChargedAll04PFId+
    process.elPFIsoValueGamma04PFId+
    process.elPFIsoValueNeutral04PFId+
    process.elPFIsoValuePU04PFId
    )

process.elPFIsoDepositCharged.src     = cms.InputTag("slimmedElectrons")
process.elPFIsoDepositChargedAll.src  = cms.InputTag("slimmedElectrons")
process.elPFIsoDepositNeutral.src     = cms.InputTag("slimmedElectrons")
process.elPFIsoDepositGamma.src       = cms.InputTag("slimmedElectrons")
process.elPFIsoDepositPU.src          = cms.InputTag("slimmedElectrons")
# Use standard HTT veto definitions for now
process.elPFIsoValueGamma04PFId.deposits[0].vetos      = cms.vstring('EcalEndcaps:ConeVeto(0.08)','EcalBarrel:ConeVeto(0.08)')
process.elPFIsoValueNeutral04PFId.deposits[0].vetos    = cms.vstring()
process.elPFIsoValuePU04PFId.deposits[0].vetos         = cms.vstring()
process.elPFIsoValueCharged04PFId.deposits[0].vetos    = cms.vstring('EcalEndcaps:ConeVeto(0.015)')
process.elPFIsoValueChargedAll04PFId.deposits[0].vetos = cms.vstring('EcalEndcaps:ConeVeto(0.015)','EcalBarrel:ConeVeto(0.01)')
# Can't run the special IsoDepositProducer here:
# https://github.com/cms-sw/cmssw/blob/CMSSW_7_0_X/RecoParticleFlow/PFProducer/python/electronPFIsolationDeposits_cff.py
# because we have these PackedCandidates as input, and this expected PFCandidates
process.elPFIsoDepositGamma.ExtractorPSet.ComponentName = cms.string("CandViewExtractor")

process.selectedElectrons = cms.EDFilter("PATElectronRefSelector",
    src = cms.InputTag("slimmedElectrons"),
    cut = cms.string("pt > 15 & abs(eta) < 2.5")
    )

process.icElectronProducer = producers.icElectronProducer.clone(
    branch                    = cms.string("electrons"),
    input                     = cms.InputTag("selectedElectrons"),
    includeVertexIP           = cms.bool(True),
    inputVertices             = cms.InputTag("selectedVertices"),
    includePFIso04           = cms.bool(True),
    pfIso04 = cms.PSet(
      chargedAll  = cms.InputTag("elPFIsoValueChargedAll04PFId"),
      charged     = cms.InputTag("elPFIsoValueCharged04PFId"),
      neutral     = cms.InputTag("elPFIsoValueNeutral04PFId"),
      gamma       = cms.InputTag("elPFIsoValueGamma04PFId"),
      pu          = cms.InputTag("elPFIsoValuePU04PFId")
      )
    )

##############################################################################
# Reco Muon Module
##############################################################################
process.load("CommonTools.ParticleFlow.Isolation.pfMuonIsolation_cff")
process.muonPFIsolationValuesSequence = cms.Sequence(
    process.muPFIsoValueCharged04+
    process.muPFIsoValueChargedAll04+
    process.muPFIsoValueGamma04+
    process.muPFIsoValueNeutral04+
    process.muPFIsoValuePU04
    )

process.muPFIsoDepositCharged.src     = cms.InputTag("slimmedMuons")
process.muPFIsoDepositChargedAll.src  = cms.InputTag("slimmedMuons")
process.muPFIsoDepositNeutral.src     = cms.InputTag("slimmedMuons")
process.muPFIsoDepositGamma.src       = cms.InputTag("slimmedMuons")
process.muPFIsoDepositPU.src          = cms.InputTag("slimmedMuons")

process.selectedMuons = cms.EDFilter("PATMuonRefSelector",
    src = cms.InputTag("slimmedMuons"),
    cut = cms.string("pt > 15 & abs(eta) < 2.5")
    )

process.icMuonProducer = producers.icMuonProducer.clone(
    branch                    = cms.string("muons"),
    input                     = cms.InputTag("slimmedMuons"),
    includeVertexIP           = cms.bool(True),
    inputVertices             = cms.InputTag("selectedVertices"),
    includePFIso04           = cms.bool(True),
    pfIso04 = cms.PSet(
      chargedAll  = cms.InputTag("muPFIsoValueChargedAll04"),
      charged     = cms.InputTag("muPFIsoValueCharged04"),
      neutral     = cms.InputTag("muPFIsoValueNeutral04"),
      gamma       = cms.InputTag("muPFIsoValueGamma04"),
      pu          = cms.InputTag("muPFIsoValuePU04")
      )
)

##############################################################################
# Tau Module
##############################################################################
process.icTauProducer = cms.EDProducer("ICPFTauFromPatProducer",
    branch                  = cms.string("taus"),
    input                   = cms.InputTag("slimmedTaus"),
    inputVertices           = cms.InputTag("selectedVertices"),
    includeVertexIP         = cms.bool(True),
    requestTracks           = cms.bool(False),
    tauIDs = cms.PSet(
      )
    )

##############################################################################
# PF Jet Module
##############################################################################
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

process.load('PhysicsTools.PatAlgos.slimming.unpackedTracksAndVertices_cfi')

# Parton flavour
# --------------
process.jetPartons = cms.EDProducer("PartonSelector",
    src = cms.InputTag("prunedGenParticles"),
    withLeptons = cms.bool(False)
)

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
  jets = cms.InputTag("ak5PFJets"),
  tracks = cms.InputTag("unpackedTracksAndVertices"),
  pvSrc = cms.InputTag("unpackedTracksAndVertices")
)

process.impactParameterTagInfosAK5PF = btag.impactParameterTagInfos.clone(
  jetTracks = cms.InputTag('jetTracksAssociatorAtVertexAK5PF'),
  primaryVertex = cms.InputTag("unpackedTracksAndVertices")
)

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

# Producer
# --------
process.icPFJetProducer = producers.icPFJetProducer.clone(
    branch                    = cms.string("pfJets"),
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
      includePileupID       = cms.bool(False),
      inputPileupID         = cms.InputTag("puJetMva", "fullDiscriminant"),
      includeTrackBasedVars = cms.bool(False),
      inputTracks           = cms.InputTag("unpackedTracksAndVertices"),
      inputVertices         = cms.InputTag("unpackedTracksAndVertices"),
      requestTracks         = cms.bool(False)
    )
)

process.icPFJetSequence = cms.Sequence(
  process.ak5PFJets+
  process.kt6PFJets+
  process.unpackedTracksAndVertices+
  process.jetPartons+
  process.pfJetPartonMatches+
  process.pfJetFlavourAssociation+
  process.icPFJetFlavourCalculator+
  process.btaggingSequenceAK5PF+
  process.icPFJetProducer
)

##############################################################################
# MET Modules
##############################################################################
process.icPfMetProducer = producers.icMetProducer.clone(
  branch  = cms.string("pfMetType1"),
  input   = cms.InputTag("slimmedMETs"),
)

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
process.icGenParticleProducer = producers.icGenParticleProducer.clone(
  branch = cms.string("genParticles"),
  input  = cms.InputTag("prunedGenParticles")
)

##############################################################################
# GenJet Module
##############################################################################
#process.selectedGenJets = cms.EDFilter("GenJetRefSelector",
#  src = cms.InputTag("ak5GenJets"),
#  cut = cms.string("pt > 15")
#)

process.icGenJetProducer = producers.icGenJetProducer.clone(
  branch  = cms.string("genJets"),
  input   = cms.InputTag("slimmedGenJets")
)

##############################################################################
# GenMet Module
##############################################################################
#process.icGenMetProducer = producers.icSingleMetProducer.clone(
#  branch = cms.string("genMet"),
#  input  = cms.InputTag("genMetTrue")
#)

process.icMCSequence = cms.Sequence()
if not isData:
  process.icMCSequence += (
    process.icPileupInfoProducer+
    #process.selectedGenParticles+
    process.icGenParticleProducer+
    #process.selectedGenJets+
    process.icGenJetProducer
    #process.icGenMetProducer
  )

##############################################################################
# TriggerPath Modules
#####################e#########################################################
from PhysicsTools.PatAlgos.tools.trigTools import *
process.patTriggerPath = cms.Path()
if release in ['72X']:
 switchOnTrigger(process, path = 'patTriggerPath', outputModule = '')
#
process.icTriggerPathProducer = producers.icTriggerPathProducer.clone(
 branch = cms.string("triggerPaths"),
 input  = cms.InputTag("patTriggerEvent")
)

##############################################################################
# TriggerObject Module
##############################################################################

process.icIsoMu17LooseTau20ObjectProducer = producers.icTriggerObjectProducer.clone(
 branch = cms.string("triggerObjectsIsoMu17LooseTau20"),
 input   = cms.InputTag("patTriggerEvent"),
 hltPath = cms.string("HLT_IsoMu17_eta2p1_LooseIsoPFTau20_v"),
 inputIsStandAlone = cms.bool(False),
 storeOnlyIfFired = cms.bool(True)
)

process.icTriggerObjectSequence = cms.Sequence(
  process.icIsoMu17LooseTau20ObjectProducer
)

if release in ['72XMINIAOD']:
    process.icTriggerPathProducer.inputIsStandAlone = cms.bool(True)
    process.icTriggerPathProducer.input = cms.InputTag("TriggerResults", "", "HLT")
    for name in process.icTriggerObjectSequence.moduleNames():
        mod = getattr(process, name)
        mod.inputIsStandAlone = cms.bool(True)
        mod.input = cms.InputTag("selectedPatTrigger", "", "PAT")


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
  process.icTriggerPathProducer+
  process.icTriggerObjectSequence+
  process.icEventProducer
  )

process.schedule = cms.Schedule(process.patTriggerPath, process.p)
#process.schedule = cms.Schedule(process.p)

#print process.dumpPython()
