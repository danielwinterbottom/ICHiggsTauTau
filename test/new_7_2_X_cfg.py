import FWCore.ParameterSet.Config as cms
process = cms.Process("MAIN")
import sys

process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.load("Configuration.Geometry.GeometryIdeal_cff")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.load("Configuration.StandardSequences.MagneticField_cff")

process.MessageLogger.cerr.FwkReport.reportEvery = 50
process.source = cms.Source("PoolSource",
  #fileNames = cms.untracked.vstring('file:/Volumes/HDD/DYJetsToLL.root')
  fileNames = cms.untracked.vstring('root://cms-xrd-global.cern.ch//store/mc/Spring14dr/GluGluToHToTauTau_M-125_13TeV-powheg-pythia6/AODSIM/PU_S14_POSTLS170_V6-v1/00000/008D295A-49ED-E311-9F6D-7845C4F92ECD.root')
)
process.GlobalTag.globaltag = cms.string('PLS170_V7AN1::All')

process.options   = cms.untracked.PSet( wantSummary = cms.untracked.bool(True) )
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(500) )

##############################################################################
## Selection
##############################################################################
process.selectedVertices = cms.EDFilter("VertexSelector",
  src = cms.InputTag("offlinePrimaryVertices"),
  cut = cms.string("ndof >= 4 & abs(z) <= 24 & abs(position.Rho) <= 2")
)

process.selectedElectrons = cms.EDFilter("GsfElectronSelector",
  src = cms.InputTag("gedGsfElectrons"),
  cut = cms.string("pt > 20 & abs(eta) < 2.5")
)

process.selectedMuons = cms.EDFilter("MuonSelector",
  src = cms.InputTag("muons"),
  cut = cms.string("pt > 20 & abs(eta) < 2.1")
)

process.selectedPFMuons = cms.EDFilter("GenericPFCandidateSelector",
  src = cms.InputTag("particleFlow"),
  cut = cms.string("pt > 20 & abs(eta) < 2.1 & abs(pdgId) == 13")
)

process.selectedCaloJets = cms.EDFilter("CaloJetRefSelector",
  src = cms.InputTag("ak5CaloJets"),
  cut = cms.string("pt > 30 & abs(eta) < 4.7")
)

process.selectedJPTJets = cms.EDFilter("JPTJetRefSelector",
  src = cms.InputTag("JetPlusTrackZSPCorJetAntiKt5"),
  cut = cms.string("pt > 30 & abs(eta) < 4.7")
)


process.selectedPFJets = cms.EDFilter("PFJetRefSelector",
  src = cms.InputTag("ak5PFJets"),
  cut = cms.string("pt > 30 & abs(eta) < 4.7")
)

##############################################################################
## PF Lepton Isolation
##############################################################################
#from CommonTools.ParticleFlow.Tools.pfIsolation import setupPFElectronIso, setupPFMuonIso
#process.eleIsoSequence = setupPFElectronIso(process, 'selectedElectrons')
#process.muoIsoSequence = setupPFMuonIso(process, 'selectedPFMuons')

##############################################################################
## Jet Parton Flavour
##############################################################################
# This is the default Run1 configuration "Algorithmic" used by b-tag POG.
# Also what is done by PAT. New recipe and details for Run 2:
# https://twiki.cern.ch/twiki/bin/view/CMSPublic/SWGuideBTagMCTools#Legacy_jet_flavour_definition
# See PAT implementation here:
# https://github.com/cms-sw/cmssw/blob/CMSSW_5_3_X/PhysicsTools/PatAlgos/plugins/PATJetProducer.cc#L296-L298
process.jetPartons = cms.EDProducer("PartonSelector",
    src = cms.InputTag("genParticles"),
    withLeptons = cms.bool(False)
)

process.caloJetPartonMatches = cms.EDProducer("JetPartonMatcher",
    jets = cms.InputTag("ak5CaloJets"),
    coneSizeToAssociate = cms.double(0.3),
    partons = cms.InputTag("jetPartons")
)
process.caloJetFlavourAssociation = cms.EDProducer("JetFlavourIdentifier",
    srcByReference = cms.InputTag("caloJetPartonMatches"),
    physicsDefinition = cms.bool(False)
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
##############################################################################
## Jet Extras
##############################################################################
process.icCaloJetFlavourCalculator = cms.EDProducer('ICJetFlavourCalculator',
    input       = cms.InputTag("ak5CaloJets"),
    flavourMap  = cms.InputTag("caloJetFlavourAssociation")
)

process.icPFJetFlavourCalculator = cms.EDProducer('ICJetFlavourCalculator',
    input       = cms.InputTag("ak5PFJets"),
    flavourMap  = cms.InputTag("pfJetFlavourAssociation")
)


#process.ak5CaloL1Fastjet = cms.ESProducer("L1FastjetCorrectionESProducer",
#    srcRho = cms.InputTag("kt6CaloJets","rho"),
#    algorithm = cms.string('AK5Calo'),
#    level = cms.string('L1FastJet')
#)
#process.ak5CaloL2Relative = cms.ESProducer("LXXXCorrectionESProducer",
#    algorithm = cms.string('AK5Calo'),
#    level = cms.string('L2Relative')
#)
#process.ak5CaloL3Absolute = cms.ESProducer("LXXXCorrectionESProducer",
#    algorithm = cms.string('AK5Calo'),
#    level = cms.string('L3Absolute')
#)
#
#process.ak5PFL1Fastjet = cms.ESProducer("L1FastjetCorrectionESProducer",
#    srcRho = cms.InputTag("kt6PFJets","rho"),
#    algorithm = cms.string('AK5PF'),
#    level = cms.string('L1FastJet')
#)
#process.ak5PFL2Relative = cms.ESProducer("LXXXCorrectionESProducer",
#    algorithm = cms.string('AK5PF'),
#    level = cms.string('L2Relative')
#)
#process.ak5PFL3Absolute = cms.ESProducer("LXXXCorrectionESProducer",
#    algorithm = cms.string('AK5PF'),
#    level = cms.string('L3Absolute')
#)

##############################################################################
## Jet B-tagging
##############################################################################
process.load("RecoJets.JetAssociationProducers.ak5JTA_cff")
from RecoJets.JetAssociationProducers.ak5JTA_cff import ak5JetTracksAssociatorAtVertex
process.load("RecoBTag.Configuration.RecoBTag_cff")
import RecoBTag.Configuration.RecoBTag_cff as btag
process.ak5JetTracksAssociatorAtVertexAK5PF = ak5JetTracksAssociatorAtVertex.clone(
  jets = cms.InputTag("ak5PFJets")
  )
process.impactParameterTagInfosAK5PF = btag.impactParameterTagInfos.clone(
  jetTracks = cms.InputTag('ak5JetTracksAssociatorAtVertexAK5PF')
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
  tagInfos = cms.VInputTag('impactParameterTagInfosAK5PF','secondaryVertexTagInfosAK5PF')
  )
process.btaggingSequenceAK5PF = cms.Sequence(
  process.ak5JetTracksAssociatorAtVertexAK5PF
  +process.impactParameterTagInfosAK5PF
  +process.secondaryVertexTagInfosAK5PF
  +process.simpleSecondaryVertexHighEffBJetTagsAK5PF
  +process.simpleSecondaryVertexHighPurBJetTagsAK5PF
  +process.combinedSecondaryVertexBJetTagsAK5PF
  )

##############################################################################
## Electron Extras
##############################################################################
#process.icElectronR9Calculator = cms.EDProducer('ICElectronR9Calculator',
#    input = cms.InputTag("selectedElectrons")
#)

#process.icElectronHcalDepthCalculator = cms.EDProducer('ICElectronHcalDepthCalculator',
#    input = cms.InputTag("selectedElectrons")
#)

process.icElectronConversionCalculator = cms.EDProducer('ICElectronConversionCalculator',
    input       = cms.InputTag("selectedElectrons"),
    beamspot    = cms.InputTag("offlineBeamSpot"),
    conversions = cms.InputTag("allConversions")
)

#process.icHttElecIsoCheck = cms.EDProducer('ICHttElecIsoCheck',
#    input         = cms.InputTag("selectedElectrons"),
#    pfChargedAll  = cms.InputTag("pfAllChargedParticles")
#)

process.icHttMuonOverlapCheck = cms.EDProducer('ICHttMuonOverlapCheck',
    input = cms.InputTag("selectedElectrons"),
    muons = cms.InputTag("muons")
)

##############################################################################
# Electron Module
##############################################################################
process.icElectronProducer = cms.EDProducer('ICElectronProducer',
    branch                    = cms.string("electrons"),
    input                     = cms.InputTag("selectedElectrons"),
    includeR9                 = cms.bool(False),
    inputR9                   = cms.InputTag("icElectronR9Calculator"),
    includeHcalSum            = cms.bool(False),
    inputHcalSum              = cms.InputTag("icElectronHcalDepthCalculator"),
    includeConversionMatches  = cms.bool(True),
    inputConversionMatches    = cms.InputTag("icElectronConversionCalculator"),
    includeVertexIP           = cms.bool(True),
    inputVertices             = cms.InputTag("selectedVertices"),
    includeBeamspotIP         = cms.bool(True),
    inputBeamspot             = cms.InputTag("offlineBeamSpot"),
    includeFloats = cms.PSet(
      # mvaTrigV0       = cms.InputTag("mvaTrigV0"),
      # mvaNonTrigV0    = cms.InputTag("mvaNonTrigV0"),
      # trackInIsoSum   = cms.InputTag("icHttElecIsoCheck"),
      matchedRecoMuon = cms.InputTag("icHttMuonOverlapCheck")
    ),
    includePFIso03           = cms.bool(False),
    pfIso03 = cms.PSet(
      chargedAll  = cms.InputTag("elPFIsoValueChargedAll03PFIdPFIso"),
      charged     = cms.InputTag("elPFIsoValueCharged03PFIdPFIso"),
      neutral     = cms.InputTag("elPFIsoValueNeutral03PFIdPFIso"),
      gamma       = cms.InputTag("elPFIsoValueGamma03PFIdPFIso"),
      pu          = cms.InputTag("elPFIsoValuePU03PFIdPFIso")
    ),
    includePFIso04           = cms.bool(False),
    pfIso04 = cms.PSet(
      chargedAll  = cms.InputTag("elPFIsoValueChargedAll04PFIdPFIso"),
      charged     = cms.InputTag("elPFIsoValueCharged04PFIdPFIso"),
      neutral     = cms.InputTag("elPFIsoValueNeutral04PFIdPFIso"),
      gamma       = cms.InputTag("elPFIsoValueGamma04PFIdPFIso"),
      pu          = cms.InputTag("elPFIsoValuePU04PFIdPFIso")
    )
)

##############################################################################
# Reco Muon Module
##############################################################################
process.icMuonProducer = cms.EDProducer('ICMuonProducer',
    branch                    = cms.string("muons"),
    input                     = cms.InputTag("selectedMuons"),
    isPF                      = cms.bool(False),
    includeVertexIP           = cms.bool(True),
    inputVertices             = cms.InputTag("selectedVertices"),
    includeBeamspotIP         = cms.bool(True),
    inputBeamspot             = cms.InputTag("offlineBeamSpot"),
    includeFloats = cms.PSet(
    ),
    includePFIso03           = cms.bool(False),
    pfIso03 = cms.PSet(
      chargedAll  = cms.InputTag(""),
      charged     = cms.InputTag(""),
      neutral     = cms.InputTag(""),
      gamma       = cms.InputTag(""),
      pu          = cms.InputTag("")
    ),
    includePFIso04           = cms.bool(False),
    pfIso04 = cms.PSet(
      chargedAll  = cms.InputTag(""),
      charged     = cms.InputTag(""),
      neutral     = cms.InputTag(""),
      gamma       = cms.InputTag(""),
      pu          = cms.InputTag("")
    )
)

##############################################################################
# PF Muon Module
##############################################################################
process.icPFMuonProducer = cms.EDProducer('ICMuonProducer',
    branch                    = cms.string("pfMuons"),
    input                     = cms.InputTag("selectedPFMuons"),
    isPF                      = cms.bool(True),
    includeVertexIP           = cms.bool(True),
    inputVertices             = cms.InputTag("selectedVertices"),
    includeBeamspotIP         = cms.bool(True),
    inputBeamspot             = cms.InputTag("offlineBeamSpot"),
    includeFloats = cms.PSet(
    ),
    includePFIso03           = cms.bool(False),
    pfIso03 = cms.PSet(
      chargedAll  = cms.InputTag("muPFIsoValueChargedAll03PFIso"),
      charged     = cms.InputTag("muPFIsoValueCharged03PFIso"),
      neutral     = cms.InputTag("muPFIsoValueNeutral03PFIso"),
      gamma       = cms.InputTag("muPFIsoValueGamma03PFIso"),
      pu          = cms.InputTag("muPFIsoValuePU03PFIso")
    ),
    includePFIso04           = cms.bool(False),
    pfIso04 = cms.PSet(
      chargedAll  = cms.InputTag("muPFIsoValueChargedAll04PFIso"),
      charged     = cms.InputTag("muPFIsoValueCharged04PFIso"),
      neutral     = cms.InputTag("muPFIsoValueNeutral04PFIso"),
      gamma       = cms.InputTag("muPFIsoValueGamma04PFIso"),
      pu          = cms.InputTag("muPFIsoValuePU04PFIso")
    )
)

##############################################################################
# CaloJet Module
##############################################################################
process.icCaloJetProducer = cms.EDProducer('ICCaloJetProducer',
    branch                    = cms.string("caloJets"),
    input                     = cms.InputTag("selectedCaloJets"),
    includeJetFlavour         = cms.bool(True),
    inputJetFlavour           = cms.InputTag("icCaloJetFlavourCalculator"),
    applyJECs                 = cms.bool(False),
    includeJECs               = cms.bool(False),
    JECs                      = cms.PSet(
      #L1FastJet  = cms.string("ak5CaloL1Fastjet"),
      #L2Relative = cms.string("ak5CaloL2Relative"),
      #L3Absolute = cms.string("ak5CaloL3Absolute")
    ),
    applyCutAfterJECs         = cms.bool(False),
    cutAfterJECs              = cms.string("pt > 30"),
    inputSVInfo               = cms.InputTag(""),
    requestSVInfo             = cms.bool(False),
    BTagDiscriminators        = cms.PSet(),
    specificConfig = cms.PSet(
      includeJetID       = cms.bool(False),
      inputJetID         = cms.InputTag("ak5JetID"),
      includeTowerCounts = cms.bool(False)
    )
)

##############################################################################
# PF Jet Module
##############################################################################
process.icPFJetProducer = cms.EDProducer('ICPFJetProducer',
    branch                    = cms.string("pfJets"),
    input                     = cms.InputTag("selectedPFJets"),
    includeJetFlavour         = cms.bool(True),
    inputJetFlavour           = cms.InputTag("icPFJetFlavourCalculator"),
    applyJECs                 = cms.bool(False),
    includeJECs               = cms.bool(False),
    JECs                      = cms.PSet(
      #L1FastJet  = cms.string("ak5PFL1Fastjet"),
      #L2Relative = cms.string("ak5PFL2Relative"),
      #L3Absolute = cms.string("ak5PFL3Absolute")
    ),
    applyCutAfterJECs         = cms.bool(False),
    cutAfterJECs              = cms.string("pt > 30"),
    inputSVInfo               = cms.InputTag("secondaryVertexTagInfosAK5PF"),
    requestSVInfo             = cms.bool(True),
    BTagDiscriminators        = cms.PSet(
      simpleSecondaryVertexHighEff = cms.InputTag("simpleSecondaryVertexHighEffBJetTagsAK5PF"),
      simpleSecondaryVertexHighPur = cms.InputTag("simpleSecondaryVertexHighPurBJetTagsAK5PF"),
      combinedSecondaryVertex      = cms.InputTag("combinedSecondaryVertexBJetTagsAK5PF")
    ),
    specificConfig = cms.PSet(
      includePileupID    = cms.bool(False),
      inputPileupID      = cms.InputTag(""),
      includeTrackBasedVars = cms.bool(True),
      inputTracks           = cms.InputTag("generalTracks"),
      inputVertices         = cms.InputTag("offlinePrimaryVertices"),
      requestTracks         = cms.bool(True)
    )
)

##############################################################################
# JPT Jet Module
##############################################################################
process.icJPTJetProducer = cms.EDProducer('ICJPTJetProducer',
    branch                    = cms.string("jptJets"),
    input                     = cms.InputTag("selectedJPTJets"),
    includeJetFlavour         = cms.bool(False),
    inputJetFlavour           = cms.InputTag(""),
    applyJECs                 = cms.bool(False),
    includeJECs               = cms.bool(False),
    JECs                      = cms.PSet(
    ),
    applyCutAfterJECs         = cms.bool(False),
    cutAfterJECs              = cms.string(""),
    inputSVInfo               = cms.InputTag(""),
    requestSVInfo             = cms.bool(False),
    BTagDiscriminators        = cms.PSet(
    ),
    specificConfig = cms.PSet(
      includeTrackBasedVars = cms.bool(True),
      inputTracks           = cms.InputTag("generalTracks"),
      inputVertices         = cms.InputTag("offlinePrimaryVertices"),
      requestTracks         = cms.bool(True),
      includeJetID          = cms.bool(False),
      inputJetID            = cms.InputTag("ak5JetID"),
      includeTowerCounts = cms.bool(False)
    )
)



##############################################################################
# Vertex Module
##############################################################################
process.icVertexProducer = cms.EDProducer('ICVertexProducer',
  branch  = cms.string("vertices"),
  input   = cms.InputTag("offlinePrimaryVertices"),
  firstVertexOnly = cms.bool(True),
  trackPtThreshold = cms.double(0.0),
  requestTracks = cms.bool(True)
)

##############################################################################
# Secondary Vertex Module
##############################################################################
process.icSecondaryVertexProducer = cms.EDProducer('ICSecondaryVertexProducer',
  branch  = cms.string("secondaryVertices"),
  input   = cms.InputTag("icPFJetProducer", "requestedSVInfo"),
  trackPtThreshold = cms.double(0.0),
  requestTracks = cms.bool(True)
)

##############################################################################
# Track Module
##############################################################################
process.icMergedTracks = cms.EDProducer('ICTrackMerger',
  merge = cms.VInputTag(
    cms.InputTag("icPFJetProducer", "requestedTracks"),
    cms.InputTag("icVertexProducer", "requestedTracks"),
    cms.InputTag("icSecondaryVertexProducer", "requestedTracks"),
    cms.InputTag("icJPTJetProducer", "requestedTracks")
  )
)

process.icTrackProducer = cms.EDProducer('ICLightTrackProducer',
  branch  = cms.string("tracks"),
  input   = cms.InputTag("icMergedTracks")
)


##############################################################################
# PileupInfo Module
##############################################################################
process.icPileupInfoProducer = cms.EDProducer('ICPileupInfoProducer',
  branch  = cms.string("pileupInfo"),
  input   = cms.InputTag("addPileupInfo")
)



##############################################################################
# GenParticle Module
##############################################################################
process.load("SimGeneral.HepPDTESSource.pythiapdt_cfi")
process.prunedGenParticles = cms.EDProducer(
    "GenParticlePruner",
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

process.icGenParticleProducer = cms.EDProducer('ICGenParticleProducer',
  branch  = cms.string("genParticles"),
  input   = cms.InputTag("prunedGenParticles"),
  includeMothers = cms.bool(True),
  includeDaughters = cms.bool(True)
)

##############################################################################
# GenJet Module
##############################################################################
process.icGenJetProducer = cms.EDProducer('ICGenJetProducer',
  branch  = cms.string("genJets"),
  input   = cms.InputTag("ak5GenJets"),
  inputGenParticles = cms.InputTag("genParticles"),
  requestGenParticles = cms.bool(True)
)

process.icGenParticlesFromJetsProducer = cms.EDProducer('ICGenParticleProducer',
  branch  = cms.string("genParticlesFromJets"),
  input   = cms.InputTag("icGenJetProducer", "requestedGenParticles"),
  includeMothers = cms.bool(False),
  includeDaughters = cms.bool(False)
)

##############################################################################
# Supercluster Module
##############################################################################
process.icSuperClusterProducer = cms.EDProducer('ICSuperClusterProducer',
  branch  = cms.string("superClusters"),
  inputBarrel   = cms.InputTag("correctedHybridSuperClusters"),
  inputEndcap   = cms.InputTag("correctedMulti5x5SuperClustersWithPreshower")
)

##############################################################################
# L1Extra Modules
##############################################################################
process.icL1EmIsolatedProducer = cms.EDProducer('ICCandidateProducer',
  branch  = cms.string("l1EmIsolated"),
  input   = cms.InputTag("l1extraParticles", "Isolated", "RECO")
)

process.icL1MHTProducer = cms.EDProducer('ICL1EtMissProducer',
  branch  = cms.string("l1MHT"),
  input   = cms.InputTag("l1extraParticles", "MHT", "RECO")
)


##############################################################################
# TriggerPath Modules
##############################################################################
from PhysicsTools.PatAlgos.tools.trigTools import *
process.p = cms.Path()
switchOnTrigger(process, path = 'p', outputModule = '')

process.icTriggerPathProducer = cms.EDProducer('ICTriggerPathProducer',
  branch  = cms.string("triggerPaths"),
  input   = cms.InputTag("patTriggerEvent"),
  includeAcceptedOnly = cms.bool(True),
  saveStrings = cms.bool(False),
  splitVersion = cms.bool(True)
)

##############################################################################
# MET Modules
##############################################################################
process.icPfMetProducer = cms.EDProducer('ICMetProducer',
  branch  = cms.string("pfMet"),
  input   = cms.InputTag("pfMet"),
  includeCustomID = cms.bool(False),
  inputCustomID = cms.InputTag("")
)

process.icGenMetProducer = cms.EDProducer('ICSingleMetProducer',
  branch  = cms.string("genMet"),
  input   = cms.InputTag("genMetTrue")
)


##############################################################################
# TriggerObject Modules
##############################################################################
process.icIsoMu24ObjectProducer = cms.EDProducer('ICTriggerObjectProducer',
  branch = cms.string("triggerObjectsIsoMu24"),
  input   = cms.InputTag("patTriggerEvent"),
  hltPath = cms.string("HLT_IsoMu24_eta2p1_v"),
  storeOnlyIfFired = cms.bool(False)
)

##############################################################################
# Photon Modules
##############################################################################
process.icPhotonHadTowerOverEmCalculator = cms.EDProducer('ICPhotonHadTowerOverEmCalculator',
    input = cms.InputTag("photons")
)

process.icPhotonElectronVetoCalculator = cms.EDProducer('ICPhotonElectronVetoCalculator',
    input       = cms.InputTag("photons"),
    electrons   = cms.InputTag("gedGsfElectrons"),
    beamspot    = cms.InputTag("offlineBeamSpot"),
    conversions = cms.InputTag("allConversions")
)

process.icPhotonProducer = cms.EDProducer('ICPhotonProducer',
    branch                  = cms.string("photons"),
    input                   = cms.InputTag("photons"),
    includeElectronVeto     = cms.bool(True),
    inputElectronVeto       = cms.InputTag("icPhotonElectronVetoCalculator"),
    includeHadTowOverEm     = cms.bool(True),
    inputHadTowOverEm       = cms.InputTag("icPhotonHadTowerOverEmCalculator"),
    includePFIso            = cms.bool(False),
    pfIso = cms.PSet(
      charged     = cms.InputTag(""),
      neutral     = cms.InputTag(""),
      gamma       = cms.InputTag("")
    )
)

##############################################################################
# Tau Module
##############################################################################
# process.selectedTaus = cms.EDFilter("PFTauRefSelector",
#   src = cms.InputTag("fixedConePFTauProducer"),
#   discriminators = cms.VPSet(
#     cms.PSet( discriminator=cms.InputTag("fixedConePFTauDiscriminationByIsolation"),selectionCut=cms.double(0.5))
#   ),
#   cut = cms.string("pt > 0"),
# )
process.selectedPFTaus = cms.EDFilter("PFTauRefSelector",
  src = cms.InputTag("hpsPFTauProducer"),
  cut = cms.string("pt > 18")
)

process.icTauProducer = cms.EDProducer("ICPFTauProducer",
  branch                  = cms.string("taus"),
  input                   = cms.InputTag("selectedPFTaus"),
  inputVertices           = cms.InputTag("offlinePrimaryVertices"),
  includeVertexIP         = cms.bool(True),
  requestTracks           = cms.bool(True),
  tauIDs = cms.PSet(
    decayModeFinding = cms.InputTag("hpsPFTauDiscriminationByDecayModeFinding")
  )
)


##############################################################################
# EventInfo Module
##############################################################################
process.icEventInfoProducer = cms.EDProducer('ICEventInfoProducer',
  branch              = cms.string("eventInfo"),
  includeJetRho       = cms.bool(False),
  inputJetRho         = cms.InputTag("kt6PFJets", "rho"),
  includeLeptonRho    = cms.bool(False),
  inputLeptonRho      = cms.InputTag("kt6PFJets", "rho"),
  includeVertexCount  = cms.bool(True),
  inputVertices       = cms.InputTag("offlinePrimaryVertices"),
  includeCSCFilter    = cms.bool(True),
  inputCSCFilter      = cms.InputTag("BeamHaloSummary"),
  filters             = cms.PSet(),
  weights             = cms.PSet(
    # embed_weight = cms.InputTag("generator", "weight", "EmbeddedRECO")
  ),
  genFilterWeights    = cms.PSet(
    # embed_weight = cms.InputTag("generator", "minVisPtFilter", "EmbeddedRECO")
  )
)


process.icEventProducer = cms.EDProducer('ICEventProducer')

process.p += (
  process.btaggingSequenceAK5PF+
  process.selectedVertices+
  process.selectedElectrons+
  process.selectedMuons+
  process.selectedCaloJets+
  process.selectedPFJets+
  process.selectedJPTJets+
  process.selectedPFMuons+
  #process.pfParticleSelectionSequence+
  #process.eleIsoSequence+
  #process.muoIsoSequence+
  #process.icElectronR9Calculator+
  #process.icElectronHcalDepthCalculator+
  process.icElectronConversionCalculator+
  #process.icHttElecIsoCheck+
  process.icHttMuonOverlapCheck+
  process.jetPartons+
  process.caloJetPartonMatches+
  process.caloJetFlavourAssociation+
  process.icCaloJetFlavourCalculator+
  process.pfJetPartonMatches+
  process.pfJetFlavourAssociation+
  process.icPFJetFlavourCalculator+
  process.icElectronProducer+
  process.icMuonProducer+
  process.icPFMuonProducer+
  process.icCaloJetProducer+
  process.icPFJetProducer+
  process.icJPTJetProducer+
  process.icVertexProducer+
  process.icSecondaryVertexProducer+
  process.icMergedTracks+
  process.icTrackProducer+
  process.icPileupInfoProducer+
  process.prunedGenParticles+
  process.icGenParticleProducer+
  process.icGenJetProducer+
  process.icGenParticlesFromJetsProducer+
  process.icSuperClusterProducer+
  process.icL1EmIsolatedProducer+
  process.icL1MHTProducer+
  #process.patTriggerSequence+
  process.icTriggerPathProducer+
  process.icPfMetProducer+
  process.icGenMetProducer+
  process.icIsoMu24ObjectProducer+
  process.icPhotonHadTowerOverEmCalculator+
  process.icPhotonElectronVetoCalculator+
  process.icPhotonProducer+
  process.selectedPFTaus+
  process.icTauProducer+
  process.icEventInfoProducer+
  process.icEventProducer
  )

#print process.dumpPython()
