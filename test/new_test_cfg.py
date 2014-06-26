import FWCore.ParameterSet.Config as cms
process = cms.Process("MAIN")
import sys

process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.load("Configuration.Geometry.GeometryIdeal_cff")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.load("Configuration.StandardSequences.MagneticField_cff")

process.MessageLogger.cerr.FwkReport.reportEvery = 50
process.source = cms.Source("PoolSource",
  fileNames = cms.untracked.vstring('file:/Volumes/HDD/DYJetsToLL.root')
)
process.GlobalTag.globaltag = cms.string('START53_V22::All')

process.options   = cms.untracked.PSet( wantSummary = cms.untracked.bool(True) )
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(500) )

# process.TFileService = cms.Service("TFileService",
#   fileName = cms.string("EventTree.root"),
#   closeFileFast = cms.untracked.bool(True)
# )

##############################################################################
## Selection
##############################################################################
process.selectedVertices = cms.EDFilter("VertexSelector",
  src = cms.InputTag("offlinePrimaryVertices"),
  cut = cms.string("ndof >= 4 & abs(z) <= 24 & abs(position.Rho) <= 2")
)

process.selectedElectrons = cms.EDFilter("GsfElectronSelector",
  src = cms.InputTag("gsfElectrons"),
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

##############################################################################
## PF Lepton Isolation
##############################################################################
from CommonTools.ParticleFlow.Tools.pfIsolation import setupPFElectronIso, setupPFMuonIso
process.eleIsoSequence = setupPFElectronIso(process, 'selectedElectrons')
process.muoIsoSequence = setupPFMuonIso(process, 'selectedPFMuons')

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
process.jetPartonMatches = cms.EDProducer("JetPartonMatcher",
    jets = cms.InputTag("ak5CaloJets"),
    coneSizeToAssociate = cms.double(0.3),
    partons = cms.InputTag("jetPartons")
)
process.jetFlavourAssociation = cms.EDProducer("JetFlavourIdentifier",
    srcByReference = cms.InputTag("jetPartonMatches"),
    physicsDefinition = cms.bool(False)
)
##############################################################################
## Jet Extras
##############################################################################
process.icJetFlavourCalculator = cms.EDProducer('ICJetFlavourCalculator',
    input       = cms.InputTag("ak5CaloJets"),
    flavourMap  = cms.InputTag("jetFlavourAssociation")
)

##############################################################################
## Electron Extras
##############################################################################
process.icElectronR9Calculator = cms.EDProducer('ICElectronR9Calculator',
    input = cms.InputTag("selectedElectrons")
)

process.icElectronHcalDepthCalculator = cms.EDProducer('ICElectronHcalDepthCalculator',
    input = cms.InputTag("selectedElectrons")
)

process.icElectronConversionCalculator = cms.EDProducer('ICElectronConversionCalculator',
    input       = cms.InputTag("selectedElectrons"),
    beamspot    = cms.InputTag("offlineBeamSpot"),
    conversions = cms.InputTag("allConversions")
)

process.icHttElecIsoCheck = cms.EDProducer('ICHttElecIsoCheck',
    input         = cms.InputTag("selectedElectrons"),
    pfChargedAll  = cms.InputTag("pfAllChargedParticles")
)

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
    includeR9                 = cms.InputTag("icElectronR9Calculator"),
    includeHcalSum            = cms.InputTag("icElectronHcalDepthCalculator"),
    includeConversionMatches  = cms.InputTag("icElectronConversionCalculator"),
    includeVertexIP           = cms.InputTag("selectedVertices"),
    includeBeamspotIP         = cms.InputTag("offlineBeamSpot"),
    includeFloats = cms.PSet(
      # mvaTrigV0       = cms.InputTag("mvaTrigV0"),
      # mvaNonTrigV0    = cms.InputTag("mvaNonTrigV0"),
      trackInIsoSum   = cms.InputTag("icHttElecIsoCheck"),
      matchedRecoMuon = cms.InputTag("icHttMuonOverlapCheck")
    ),
    pfIso03 = cms.PSet(
      chargedAll  = cms.InputTag("elPFIsoValueChargedAll03PFIdPFIso"),
      charged     = cms.InputTag("elPFIsoValueCharged03PFIdPFIso"),
      neutral     = cms.InputTag("elPFIsoValueNeutral03PFIdPFIso"),
      gamma       = cms.InputTag("elPFIsoValueGamma03PFIdPFIso"),
      pu          = cms.InputTag("elPFIsoValuePU03PFIdPFIso")
    ),
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
    includeVertexIP           = cms.InputTag("selectedVertices"),
    includeBeamspotIP         = cms.InputTag("offlineBeamSpot"),
    includeFloats = cms.PSet(
    )
)

##############################################################################
# PF Muon Module
##############################################################################
process.icPFMuonProducer = cms.EDProducer('ICMuonProducer',
    branch                    = cms.string("pfMuons"),
    input                     = cms.InputTag("selectedPFMuons"),
    isPF                      = cms.bool(True),
    includeVertexIP           = cms.InputTag("selectedVertices"),
    includeBeamspotIP         = cms.InputTag("offlineBeamSpot"),
    includeFloats = cms.PSet(
    ),
    pfIso03 = cms.PSet(
      chargedAll  = cms.InputTag("muPFIsoValueChargedAll03PFIso"),
      charged     = cms.InputTag("muPFIsoValueCharged03PFIso"),
      neutral     = cms.InputTag("muPFIsoValueNeutral03PFIso"),
      gamma       = cms.InputTag("muPFIsoValueGamma03PFIso"),
      pu          = cms.InputTag("muPFIsoValuePU03PFIso")
    ),
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
process.icCaloJetProducer = cms.EDProducer('ICNewCaloJetProducer',
    branch                    = cms.string("caloJets"),
    input                     = cms.InputTag("selectedCaloJets"),
    includeJetFlavour         = cms.InputTag("icJetFlavourCalculator"),
    specificConfig = cms.PSet(
      includeJetID = cms.InputTag("ak5JetID")
    )
)

process.icEventProducer = cms.EDProducer('ICEventProducer')


process.p = cms.Path(
  process.selectedVertices+
  process.selectedElectrons+
  process.selectedMuons+
  process.selectedCaloJets+
  process.selectedPFMuons+
  process.pfParticleSelectionSequence+
  process.eleIsoSequence+
  process.muoIsoSequence+
  process.icElectronR9Calculator+
  process.icElectronHcalDepthCalculator+
  process.icElectronConversionCalculator+
  process.icHttElecIsoCheck+
  process.icHttMuonOverlapCheck+
  process.jetPartons+
  process.jetPartonMatches+
  process.jetFlavourAssociation+
  process.icJetFlavourCalculator+
  process.icElectronProducer+
  process.icMuonProducer+
  process.icPFMuonProducer+
  process.icCaloJetProducer+
  process.icEventProducer
  )

#print process.dumpPython()