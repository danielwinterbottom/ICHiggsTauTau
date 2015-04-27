import FWCore.ParameterSet.Config as cms
process = cms.Process("MAIN")
import sys

################################################################
# Read Options
################################################################
import FWCore.ParameterSet.VarParsing as parser
opts = parser.VarParsing ('analysis')
opts.register('file', 'file:miniaod_gg.root', parser.VarParsing.multiplicity.singleton,
#opts.register('file', 'file:InputAOD_gg.root', parser.VarParsing.multiplicity.singleton,
#opts.register('file', 'root://xrootd.unl.edu//store/mc/Phys14DR/GluGluToHToTauTau_M-125_13TeV-powheg-pythia6/MINIAODSIM/PU20bx25_tsg_PHYS14_25_V1-v1/00000/2405749F-8B6F-E411-88EE-848F69FD2910.root', parser.VarParsing.multiplicity.singleton,
#opts.register('file', 'root://xrootd.unl.edu//store/mc/Phys14DR/VBF_HToTauTau_M-125_13TeV-powheg-pythia6/MINIAODSIM/PU40bx25_PHYS14_25_V1-v1/00000/36224FE2-0571-E411-9664-00266CFAE30C.root', parser.VarParsing.multiplicity.singleton,
    parser.VarParsing.varType.string, "input file")
opts.register('globalTag', 'PHYS14_25_V2::All', parser.VarParsing.multiplicity.singleton,
    parser.VarParsing.varType.string, "global tag")
opts.register('isData', 0, parser.VarParsing.multiplicity.singleton,
    parser.VarParsing.varType.int, "Process as data?")
opts.register('release', '72XMINIAOD', parser.VarParsing.multiplicity.singleton,
    parser.VarParsing.varType.string, "Release label")

opts.parseArguments()
infile      = opts.file
if not infile: infile = "file:/tmp/file.root"
tag         = opts.globalTag
isData      = opts.isData
release     = opts.release
#isEmbedded  = opts.isEmbedded
#isTandP     = opts.isTandP
#isZStudy    = opts.isZStudy
#isPhys14    = opts.isPhys14

if not release in ["72X", "72XMINIAOD"]:
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
process.maxEvents = cms.untracked.PSet(
  input = cms.untracked.int32(100)
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

# 72X MC: file=root://xrootd.unl.edu//store/mc/Phys14DR/VBF_HToTauTau_M-125_13TeV-powheg-pythia6/AODSIM/PU40bx25_PHYS14_25_V1-v1/00000/00E63918-3A70-E411-A246-7845C4FC35F3.root globalTag=START72_V1::All 
#infile='root://xrootd.unl.edu//store/mc/Phys14DR/VBF_HToTauTau_M-125_13TeV-powheg-pythia6/MINIAODSIM/PU40bx25_PHYS14_25_V1-v1/00000/36224FE2-0571-E411-9664-00266CFAE30C.root globalTag=START72_V1::All'

import UserCode.ICHiggsTauTau.default_producers_cfi as producers
import UserCode.ICHiggsTauTau.default_selectors_cfi as selectors

################################################################
# Re-do PFTau reconstruction
################################################################
# process.load("RecoTauTag/Configuration/RecoPFTauTag_cff")

process.ic72XSequence = cms.Sequence()
process.icMiniAODSequence = cms.Sequence()

# if release in ['72X']:
#   process.ic72XSequence += process.PFTau

# if release in ['70XMINIAOD', '72XMINIAOD']:
#   process.load('PhysicsTools.PatAlgos.slimming.unpackedTracksAndVertices_cfi')
#   process.icMiniAODSequence += process.unpackedTracksAndVertices
#switchOnTrigger(process, outputModule="")


################################################################
# Need to create kt6PFJets in 42X for L1FastJet correction
################################################################
if release in ['72X', '72XMINIAOD']:
  from RecoJets.JetProducers.kt4PFJets_cfi import kt4PFJets
  process.kt6PFJets = kt4PFJets.clone(
    rParam = cms.double(0.6),
    doAreaFastjet = cms.bool(True),
    doRhoFastjet = cms.bool(True)
  )
  process.ic72XSequence += process.kt6PFJets

from PhysicsTools.SelectorUtils.pvSelector_cfi import pvSelector
process.goodOfflinePrimaryVertices = cms.EDFilter(
    "PrimaryVertexObjectFilter",
    filterParams = pvSelector.clone( minNdof = cms.double(4.0), maxZ = cms.double(24.0) ),
    src=cms.InputTag('offlinePrimaryVertices')
    )


process.load("PhysicsTools.PatAlgos.patSequences_cff")
from PhysicsTools.PatAlgos.tools.coreTools import *
from PhysicsTools.PatAlgos.tools.pfTools import *
from PhysicsTools.PatAlgos.tools.tauTools import *
from PhysicsTools.PatAlgos.tools.jetTools import *
from PhysicsTools.PatAlgos.tools.trigTools import *
from PhysicsTools.PatAlgos.tools.metTools import *


################################################################
# Object Selection
################################################################
process.selectedVertices = cms.EDFilter("VertexRefSelector",
  src = cms.InputTag("offlinePrimaryVertices"),
  cut = cms.string("ndof >= 4 & abs(z) <= 24 & abs(position.Rho) <= 2")
)


process.selectedElectrons = cms.EDFilter("GsfElectronSelector",
  src = cms.InputTag("gedGsfElectrons"),
  cut = cms.string("pt > 9.5 & abs(eta) < 2.6")
)


if release in ['72XMINIAOD']:
  process.selectedVertices.src = cms.InputTag("offlineSlimmedPrimaryVertices")
  process.selectedElectrons = cms.EDFilter("PATElectronSelector",
      src = cms.InputTag("slimmedElectrons"),
      cut = cms.string("pt > 9.5 & abs(eta) < 2.6")
      )

process.icSelectionSequence = cms.Sequence(
  process.selectedVertices+
  process.selectedElectrons
)

################################################################
# PF sequence for lepton isolation
################################################################
process.load("CommonTools.ParticleFlow.pfParticleSelection_cff")
if release in ['72X']:
  process.pfPileUp.PFCandidates = cms.InputTag("particleFlowPtrs")
  process.pfPileUpIso.PFCandidates = cms.InputTag("particleFlowPtrs")
  process.pfNoPileUp.bottomCollection = cms.InputTag("particleFlowPtrs")
  process.pfNoPileUpIso.bottomCollection = cms.InputTag("particleFlowPtrs")

if release in ['72XMINIAOD']:
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

process.icVertexSequence = cms.Sequence(
  process.icVertexProducer
)

################################################################
# PFCandidates
################################################################

################################################################
# Tracks
################################################################
################################################################
# Electrons
################################################################
#electronLabel = cms.InputTag("gedGsfElectrons")
#if release in ['72XMINIAOD']: electronLabel = cms.InputTag("selectedElectrons")
electronLabel = cms.InputTag("selectedElectrons")

process.icElectronSequence = cms.Sequence()

#ICElectronConversionCalculator NOT final, but at least have a running version for now
process.icElectronConversionCalculator = cms.EDProducer('ICElectronConversionCalculator',
    input       = electronLabel,
    beamspot    = cms.InputTag("offlineBeamSpot"),
    conversions = cms.InputTag("allConversions")
)
if release in ['70XMINIAOD', '72XMINIAOD']:
  process.icElectronConversionCalculator = cms.EDProducer('ICElectronConversionFromPatCalculator',
      input       = electronLabel
  )

#Recalculation of electron conversion veto (alternative to using the value stored in the 
#slimmedElectrons collection, commented out for now)
#  process.icElectronConversionCalculator = cms.EDProducer('ICElectronConversionCalculator',
#      input       = electronLabel,
#      beamspot    = cms.InputTag("offlineBeamSpot"),
#      conversions = cms.InputTag("reducedEgamma:reducedConversions")
#  )

if release in ['72X']:
  process.load("EgammaAnalysis.ElectronTools.electronIdMVAProducer_CSA14_cfi")
  process.mvaNonTrigV025nsPHYS14.electronTag = cms.InputTag("selectedElectrons")
if release in ['72XMINIAOD']:
#New electron ID MVA producer to run without tracks
  process.mvaNonTrigV025nsPHYS14 = cms.EDProducer("ICElectronIDMVAProducerMiniAOD",
      verbose=cms.untracked.bool(False),
      vertexTag=cms.InputTag('offlineSlimmedPrimaryVertices'),
      electronTag=cms.InputTag('selectedElectrons'),
      reducedEBRecHitCollection=cms.InputTag('reducedEgamma','reducedEBRecHits',"PAT"),
      reducedEERecHitCollection=cms.InputTag('reducedEgamma','reducedEERecHits',"PAT"),
      method=cms.string("BDTSimpleCat"),
      mvaWeightFile=cms.vstring(
                                "EgammaAnalysis/ElectronTools/data/PHYS14/EIDmva_EB1_5_oldscenario2phys14_BDT.weights.xml",
                                "EgammaAnalysis/ElectronTools/data/PHYS14/EIDmva_EB2_5_oldscenario2phys14_BDT.weights.xml",
                                "EgammaAnalysis/ElectronTools/data/PHYS14/EIDmva_EE_5_oldscenario2phys14_BDT.weights.xml",
                                "EgammaAnalysis/ElectronTools/data/PHYS14/EIDmva_EB1_10_oldscenario2phys14_BDT.weights.xml",
                                "EgammaAnalysis/ElectronTools/data/PHYS14/EIDmva_EB2_10_oldscenario2phys14_BDT.weights.xml",
                                "EgammaAnalysis/ElectronTools/data/PHYS14/EIDmva_EE_10_oldscenario2phys14_BDT.weights.xml",
                                 ),
     trig=cms.bool(False),
     )

#Include electron cut-based IDs
#Commented out as it's not stricly necessary 
#from PhysicsTools.SelectorUtils.tools.vid_id_tools import *
#process.load("RecoEgamma.ElectronIdentification.egmGsfElectronIDs_cfi")
#process.egmGsfElectronIDs.physicsObjectSrc = cms.InputTag('selectedElectrons')

#from PhysicsTools.SelectorUtils.centralIDRegistry import central_id_registry
#process.egmGsfElectronIDSequence = cms.Sequence(process.egmGsfElectronIDs)

#my_id_modules = ['RecoEgamma.ElectronIdentification.Identification.cutBasedElectronID_PHYS14_PU20bx25_V1_cff']
#if release in ['72XMINIAOD']:
#  my_id_modules = ['RecoEgamma.ElectronIdentification.Identification.cutBasedElectronID_PHYS14_PU20bx25_V1_miniAOD_cff']

#for idmod in my_id_modules:
#  setupAllVIDIdsInModule(process,idmod,setupVIDElectronSelection)

process.icElectronSequence+=cms.Sequence(
#   process.egmGsfElectronIDSequence+
   process.mvaNonTrigV025nsPHYS14
   )

#Electron PF iso sequence:
if release in ['72X', '72XMINIAOD']:
  process.load("CommonTools.ParticleFlow.Isolation.pfElectronIsolation_cff")
  process.elPFIsoValueCharged03PFIdPFIso    = process.elPFIsoValueCharged03PFId.clone()
  process.elPFIsoValueChargedAll03PFIdPFIso = process.elPFIsoValueChargedAll03PFId.clone()
  process.elPFIsoValueGamma03PFIdPFIso      = process.elPFIsoValueGamma03PFId.clone()
  process.elPFIsoValueNeutral03PFIdPFIso    = process.elPFIsoValueNeutral03PFId.clone()
  process.elPFIsoValuePU03PFIdPFIso         = process.elPFIsoValuePU03PFId.clone()
  process.electronPFIsolationValuesSequence = cms.Sequence(
      process.elPFIsoValueCharged03PFIdPFIso+
      process.elPFIsoValueChargedAll03PFIdPFIso+
      process.elPFIsoValueGamma03PFIdPFIso+
      process.elPFIsoValueNeutral03PFIdPFIso+
      process.elPFIsoValuePU03PFIdPFIso
      )
  process.elPFIsoDepositCharged.src     = electronLabel
  process.elPFIsoDepositChargedAll.src  = electronLabel
  process.elPFIsoDepositNeutral.src     = electronLabel
  process.elPFIsoDepositGamma.src       = electronLabel
  process.elPFIsoDepositPU.src          = electronLabel
  if release in ['72XMINIAOD']:
    process.elPFIsoDepositGamma.ExtractorPSet.ComponentName = cms.string("CandViewExtractor")
  process.icElectronSequence += cms.Sequence(
      process.electronPFIsolationDepositsSequence+
      process.electronPFIsolationValuesSequence
      )
#Leave these out for now:
#process.elPFIsoValueGamma04PFIdPFIso.deposits[0].vetos = (
#    cms.vstring('EcalEndcaps:ConeVeto(0.08)','EcalBarrel:ConeVeto(0.08)'))
#process.elPFIsoValueNeutral04PFIdPFIso.deposits[0].vetos = (
#    cms.vstring())
#process.elPFIsoValuePU04PFIdPFIso.deposits[0].vetos = (
#    cms.vstring())
#process.elPFIsoValueCharged04PFIdPFIso.deposits[0].vetos = (
#    cms.vstring('EcalEndcaps:ConeVeto(0.015)'))
#process.elPFIsoValueChargedAll04PFIdPFIso.deposits[0].vetos = (
#    cms.vstring('EcalEndcaps:ConeVeto(0.015)','EcalBarrel:ConeVeto(0.01)'))

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
#     cutBasedVeto        = cms.InputTag("egmGsfElectronIDs:cutBasedElectronID-PHYS14-PU20bx25-V1-standalone-veto"),
#     cutBasedLoose        = cms.InputTag("egmGsfElectronIDs:cutBasedElectronID-PHYS14-PU20bx25-V1-standalone-loose"),
#     cutBasedMedium        = cms.InputTag("egmGsfElectronIDs:cutBasedElectronID-PHYS14-PU20bx25-V1-standalone-medium"),
#     cutBasedTight        = cms.InputTag("egmGsfElectronIDs:cutBasedElectronID-PHYS14-PU20bx25-V1-standalone-tight"),
     mvaNonTrigV025nsPHYS14    = cms.InputTag("mvaNonTrigV025nsPHYS14")
  ),
  includePFIso03           = cms.bool(True)
)

if release in ['72XMINIAOD']:
  process.icElectronProducer.includeFloats = cms.PSet(
#     cutBasedVeto        = cms.InputTag("egmGsfElectronIDs:cutBasedElectronID-PHYS14-PU20bx25-V1-miniAOD-standalone-veto"),
#     cutBasedLoose        = cms.InputTag("egmGsfElectronIDs:cutBasedElectronID-PHYS14-PU20bx25-V1-miniAOD-standalone-loose"),
#     cutBasedMedium        = cms.InputTag("egmGsfElectronIDs:cutBasedElectronID-PHYS14-PU20bx25-V1-miniAOD-standalone-medium"),
#     cutBasedTight        = cms.InputTag("egmGsfElectronIDs:cutBasedElectronID-PHYS14-PU20bx25-V1-miniAOD-standalone-tight"),
     mvaNonTrigV025nsPHYS14    = cms.InputTag("mvaNonTrigV025nsPHYS14")
  )

process.icElectronSequence += cms.Sequence(
  process.icElectronConversionCalculator+
  process.icElectronProducer
)

################################################################
# Muons
################################################################
################################################################
# Taus
################################################################
# ################################################################
# # Jets
# ################################################################

# ################################################################
# # MVA MET and PF MET
# ################################################################

################################################################
# Simulation only: GenParticles, GenJets, PileupInfo
################################################################

# ################################################################
# # Trigger
# ################################################################
################################################################
# EventInfo
################################################################

################################################################
# Event
################################################################
process.icEventProducer = producers.icEventProducer.clone()


process.p = cms.Path(
#  process.ic72XSequence+
#  process.icMiniAODSequence+
  process.icSelectionSequence+
  process.pfParticleSelectionSequence+
  process.icVertexSequence+
  process.icElectronSequence+
  process.icEventProducer
)

# process.schedule = cms.Schedule(process.patTriggerPath, process.p)
process.schedule = cms.Schedule(process.p)

#print process.dumpPython()
