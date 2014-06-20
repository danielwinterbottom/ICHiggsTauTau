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
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(50) )

process.bestMuons = cms.EDFilter("MuonRefSelector",
  src = cms.InputTag("muons"),
  cut = cms.string("pt > 20 & abs(eta) < 2")
  )

process.mergeMuons = cms.EDProducer("ICMuonMerger",
  merge = cms.VInputTag("bestMuons", "muons")
  )

process.testModule = cms.EDProducer("TestModule")


process.TFileService = cms.Service("TFileService",
  fileName = cms.string("EventTree.root"),
  closeFileFast = cms.untracked.bool(True)
)

from PhysicsTools.SelectorUtils.pvSelector_cfi import pvSelector
process.goodOfflinePrimaryVertices = cms.EDFilter(
    "PrimaryVertexObjectFilter",
    filterParams = pvSelector.clone( minNdof = cms.double(4.0), maxZ = cms.double(24.0) ),
    src=cms.InputTag('offlinePrimaryVertices')
    )

process.selectedElectrons = cms.EDFilter("GsfElectronSelector",
  src = cms.InputTag("gsfElectrons"),
  cut = cms.string("pt > 20 & abs(eta) < 2.5")
)

process.selectedVertices = cms.EDFilter("VertexSelector",
  src = cms.InputTag("offlinePrimaryVertices"),
  cut = cms.string("ndof >= 4 & abs(z) <= 24 & abs(position.Rho) <= 2")
)

from CommonTools.ParticleFlow.Tools.pfIsolation import setupPFElectronIso, setupPFMuonIso
process.eleIsoSequence = setupPFElectronIso(process, 'selectedElectrons')

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

process.icElectronProducer = cms.EDProducer('ICElectronProducer',
    branch            = cms.string("electrons"),
    input             = cms.InputTag("selectedElectrons"),
    includeR9         = cms.InputTag("icElectronR9Calculator"),
    includeHcalSum    = cms.InputTag("icElectronHcalDepthCalculator"),
    includeVertexIP   = cms.InputTag("selectedVertices"),
    includeBeamspotIP = cms.InputTag("offlineBeamSpot"),
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

process.icEventProducer = cms.EDProducer('ICEventProducer')


process.p = cms.Path(
  # process.bestMuons+
  # process.mergeMuons+
  # process.testModule
  # process.goodOfflinePrimaryVertices+
  process.selectedVertices+
  process.selectedElectrons+
  process.pfParticleSelectionSequence+
  process.eleIsoSequence+
  process.icElectronR9Calculator+
  process.icElectronHcalDepthCalculator+
  process.icElectronConversionCalculator+
  process.icHttElecIsoCheck+
  process.icHttMuonOverlapCheck+
  process.icElectronProducer+
  process.icEventProducer
  )

#print process.dumpPython()