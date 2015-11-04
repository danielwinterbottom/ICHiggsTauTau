import FWCore.ParameterSet.Config as cms
process = cms.Process("MAIN")
import sys

################################################################
# Read Options
################################################################
import FWCore.ParameterSet.VarParsing as parser
opts = parser.VarParsing ('analysis')
#opts.register('file', 'file:/afs/cern.ch/work/a/adewit/private/CMSSW_7_4_4/src/UserCode/ICHiggsTauTau/test/testinput.root', parser.VarParsing.multiplicity.singleton,
#opts.register('file', 'file:/afs/cern.ch/work/a/adewit/private/CMSSW_7_4_5/src/UserCode/ICHiggsTauTau/test/TauDataTest.root', parser.VarParsing.multiplicity.singleton,
opts.register('file',
#'root://xrootd.unl.edu//store/data/Run2015B/SingleElectron/MINIAOD/PromptReco-v1/000/251/163/00000/9C435096-9F26-E511-A1D7-02163E012AB6.root',
'root://xrootd.unl.edu//store/data/Run2015B/SingleElectron/MINIAOD/PromptReco-v1/000/251/164/00000/4633CC68-A326-E511-95D0-02163E0124EA.root', parser.VarParsing.multiplicity.singleton,
#opts.register('file','root://xrootd.unl.edu//store/mc/RunIISpring15DR74/DYJetsToLL_M-50_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8/MINIAODSIM/Asympt25ns_MCRUN2_74_V9-v3/10000/009D49A5-7314-E511-84EF-0025905A605E.root',parser.VarParsing.multiplicity.singleton,
#opts.register('file', 'file:miniaod_gg.root', parser.VarParsing.multiplicity.singleton,
#opts.register('file', 'root://xrootd.unl.edu//store/mc/Phys14DR/GluGluToHToTauTau_M-125_13TeV-powheg-pythia6/MINIAODSIM/PU20bx25_tsg_PHYS14_25_V1-v1/00000/2405749F-8B6F-E411-88EE-848F69FD2910.root', parser.VarParsing.multiplicity.singleton,
#opts.register('file', 'root://xrootd.unl.edu//store/mc/Phys14DR/VBF_HToTauTau_M-125_13TeV-powheg-pythia6/MINIAODSIM/PU40bx25_PHYS14_25_V1-v1/00000/36224FE2-0571-E411-9664-00266CFAE30C.root', parser.VarParsing.multiplicity.singleton,
    parser.VarParsing.varType.string, "input file")
#opts.register('globalTag', 'MCRUN2_74_V9', parser.VarParsing.multiplicity.singleton,
opts.register('globalTag', '74X_dataRun2_Prompt_v0', parser.VarParsing.multiplicity.singleton,
    parser.VarParsing.varType.string, "global tag")
opts.register('isData', 1, parser.VarParsing.multiplicity.singleton,
    parser.VarParsing.varType.int, "Process as data?")
opts.register('release', '74XMINIAOD', parser.VarParsing.multiplicity.singleton,
#opts.register('release', '74X', parser.VarParsing.multiplicity.singleton,
    parser.VarParsing.varType.string, "Release label")
opts.register('isNLO', 0, parser.VarParsing.multiplicity.singleton,
    parser.VarParsing.varType.int, "Is this an NLO sample?")


opts.parseArguments()
infile      = opts.file
if not infile: infile = "file:/tmp/file.root"
isData      = opts.isData
tag         = opts.globalTag
release     = opts.release
if not isData:
  isNLO     = opts.isNLO
else:
  isNLO     = 0
#isEmbedded  = opts.isEmbedded
#isTandP     = opts.isTandP
#isZStudy    = opts.isZStudy
#isPhys14    = opts.isPhys14

if not release in ["74X", "74XMINIAOD"]:
  print 'Release not recognised, exiting!'
  sys.exit(1)
print 'release     : '+release
print 'isData      : '+str(isData)
print 'globalTag   : '+str(tag)
print 'isNLO       : '+str(isNLO)

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
  input = cms.untracked.int32(1000)
)

process.MessageLogger.cerr.FwkReport.reportEvery = 50

process.options   = cms.untracked.PSet(
  wantSummary = cms.untracked.bool(True)
)


################################################################
# Input files and global tags
################################################################
process.source = cms.Source("PoolSource", fileNames = cms.untracked.vstring(
#'root://xrootd.unl.edu//store/data/Run2015B/SingleElectron/MINIAOD/PromptReco-v1/000/251/161/00000/7019DC27-9C26-E511-84FF-02163E011CC2.root',
#'root://xrootd.unl.edu//store/data/Run2015B/SingleElectron/MINIAOD/PromptReco-v1/000/251/162/00000/9CC606D8-4127-E511-8F35-02163E013830.root',
#'root://xrootd.unl.edu//store/data/Run2015B/SingleElectron/MINIAOD/PromptReco-v1/000/251/163/00000/9C435096-9F26-E511-A1D7-02163E012AB6.root',
#'root://xrootd.unl.edu//store/data/Run2015B/SingleElectron/MINIAOD/PromptReco-v1/000/251/164/00000/4633CC68-A326-E511-95D0-02163E0124EA.root'
'root://xrootd.unl.edu//store/data/Run2015B/Tau/MINIAOD/PromptReco-v1/000/251/642/00000/EC1989CD-EB2A-E511-8F15-02163E0146A4.root',

#infile
))
process.GlobalTag.globaltag = cms.string(tag)

# 74X MC: file=root://xrootd.unl.edu//store/mc/Phys14DR/VBF_HToTauTau_M-125_13TeV-powheg-pythia6/AODSIM/PU40bx25_PHYS14_25_V1-v1/00000/00E63918-3A70-E411-A246-7845C4FC35F3.root globalTag=START72_V1::All 
#infile='root://xrootd.unl.edu//store/mc/Phys14DR/VBF_HToTauTau_M-125_13TeV-powheg-pythia6/MINIAODSIM/PU40bx25_PHYS14_25_V1-v1/00000/36224FE2-0571-E411-9664-00266CFAE30C.root globalTag=START72_V1::All'

import UserCode.ICHiggsTauTau.default_producers_cfi as producers
import UserCode.ICHiggsTauTau.default_selectors_cfi as selectors

process.load("PhysicsTools.PatAlgos.patSequences_cff")
################################################################
# Re-do PFTau reconstruction
################################################################
# process.load("RecoTauTag/Configuration/RecoPFTauTag_cff")

process.ic74XSequence = cms.Sequence()
process.icMiniAODSequence = cms.Sequence()

# if release in ['74X']:
#   process.ic74XSequence += process.PFTau

# if release in ['70XMINIAOD', '74XMINIAOD']:
#   process.load('PhysicsTools.PatAlgos.slimming.unpackedTracksAndVertices_cfi')
#   process.icMiniAODSequence += process.unpackedTracksAndVertices
#switchOnTrigger(process, outputModule="")


################################################################
# Need to create kt6PFJets in 42X for L1FastJet correction
################################################################
if release in ['74X', '74XMINIAOD']:
  from RecoJets.JetProducers.kt4PFJets_cfi import kt4PFJets
  process.kt6PFJets = kt4PFJets.clone(
    rParam = cms.double(0.6),
    doAreaFastjet = cms.bool(True),
    doRhoFastjet = cms.bool(True)
  )
  process.ic74XSequence += process.kt6PFJets


################################################################
# Object Selection
################################################################
#process.selectedVertices = cms.EDFilter("VertexRefSelector",
#  src = cms.InputTag("offlinePrimaryVertices"),
#  cut = cms.string("ndof >= 4 & abs(z) <= 24 & abs(position.Rho) <= 2")
#)

#process.selectedPFCandidates = cms.EDFilter("PFCandidateRefSelector",
#  src = cms.InputTag("particleFlow"),
#  cut = cms.string("pt > 5.0")
#)

process.selectedElectrons = cms.EDFilter("GsfElectronRefSelector",
  src = cms.InputTag("gedGsfElectrons"),
  cut = cms.string("pt > 9.5 & abs(eta) < 2.6")
)

process.selectedMuons = cms.EDFilter("MuonRefSelector",
  src = cms.InputTag("muons"),
  cut = cms.string("pt > 3.0 & abs(eta) < 2.6")
)

#process.selectedPFMuons = cms.EDFilter("GenericPFCandidateSelector",
#  src = cms.InputTag("particleFlow"),
#  cut = cms.string("pt > 3.0 & abs(eta) < 2.6 & abs(pdgId) == 13")
#)

process.selectedTaus = cms.EDFilter("PFTauRefSelector",
  src = cms.InputTag("hpsPFTauProducer"),
  cut = cms.string("pt > 10.0 & abs(eta) < 3.0")
)

if release in ['74XMINIAOD']:
#  process.selectedVertices.src = cms.InputTag("offlineSlimmedPrimaryVertices")
  process.selectedElectrons = cms.EDFilter("PATElectronRefSelector",
      src = cms.InputTag("slimmedElectrons"),
      cut = cms.string("pt > 9.5 & abs(eta) < 2.6")
      )
  process.selectedMuons = cms.EDFilter("PATMuonRefSelector",
      src = cms.InputTag("slimmedMuons"),
      cut = cms.string("pt > 3 & abs(eta) < 2.6")
      )
  process.selectedTaus = cms.EDFilter("PATTauRefSelector",
      src = cms.InputTag("slimmedTaus"),
      cut = cms.string('pt > 18.0 & abs(eta) < 2.6 & tauID("decayModeFindingNewDMs") > 0.5')
      )

process.icSelectionSequence = cms.Sequence(
#  process.selectedVertices+
#  process.selectedPFCandidates+
  process.selectedElectrons+
#  process.selectedMuons+
  process.selectedMuons+
  process.selectedTaus
)

################################################################
# PF sequence for lepton isolation
################################################################
process.load("CommonTools.ParticleFlow.pfParticleSelection_cff")
if release in ['74X']:
  process.pfPileUp.PFCandidates = cms.InputTag("particleFlowPtrs")
  process.pfPileUpIso.PFCandidates = cms.InputTag("particleFlowPtrs")
  process.pfNoPileUp.bottomCollection = cms.InputTag("particleFlowPtrs")
  process.pfNoPileUpIso.bottomCollection = cms.InputTag("particleFlowPtrs")


if release in ['74XMINIAOD']:
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

vtxLabel = cms.InputTag("offlinePrimaryVertices")
if release in ['74XMINIAOD']:
  vtxLabel = cms.InputTag("offlineSlimmedPrimaryVertices")

################################################################
# Vertices
################################################################
process.icVertexProducer = producers.icVertexProducer.clone(
  branch  = cms.string("vertices"),
  input = vtxLabel,
  firstVertexOnly = cms.bool(True)
)


process.icVertexSequence = cms.Sequence(
  process.icVertexProducer
)

################################################################
# PFCandidates
################################################################
#process.icPFProducer = cms.EDProducer('ICPFProducer',
#  branch  = cms.string("pfCandidates"),
#  input   = cms.InputTag("selectedPFCandidates"),
#  requestTracks       = cms.bool(True),
#  requestGsfTracks    = cms.bool(True)
#)

#process.icPFSequence = cms.Sequence()
# process.icPFSequence += process.icPFProducer


################################################################
# Tracks
################################################################
#process.selectedTracks = cms.EDFilter("TrackRefSelector",
#  src = cms.InputTag("generalTracks"),
#  cut = cms.string("pt > 0.5")
#)

# We write (for phys 14 studies):
# - all tracks with pT > 5 GeV
# - tracks referenced by the PF candidates we store
# - tracks referenced by the taus we store
# - all tracks with DR < 0.5 pf the selected PF taus with pT > 0.5 GeV
#process.icMergedTracks = cms.EDProducer('ICTrackMerger',
#  merge = cms.VInputTag(
#    cms.InputTag("selectedTracks"),
#    cms.InputTag("icTauProducer", "requestedTracks")
    # cms.InputTag("icPFProducer", "requestedTracks"),
#  )
#)

#process.icTrackProducer = producers.icTrackProducer.clone(
#  branch = cms.string("tracks"),
#  input  = cms.InputTag("icMergedTracks")
#)

#process.icGsfTrackProducer = producers.icTrackProducer.clone(
#  branch = cms.string("gsfTracks"),
#  input  = cms.InputTag("icPFProducer", "requestedGsfTracks")
#)

#process.icTrackSequence = cms.Sequence()
#process.icTrackSequence += cms.Sequence(
#   process.selectedTracks+
#   process.icMergedTracks+
#   process.icTrackProducer+
#   process.icGsfTrackProducer
#)

################################################################
# Electrons
################################################################
electronLabel = cms.InputTag("gedGsfElectrons")
if release in ['74XMINIAOD']:
  electronLabel = cms.InputTag("slimmedElectrons")

process.icElectronSequence = cms.Sequence()

#ICElectronConversionCalculator NOT final, but at least have a running version for now
process.icElectronConversionCalculator = cms.EDProducer('ICElectronConversionCalculator',
    input       = electronLabel, 
    beamspot    = cms.InputTag("offlineBeamSpot"),
    conversions = cms.InputTag("allConversions")
)

if release in ['70XMINIAOD', '74XMINIAOD']:
  process.icElectronConversionCalculator = cms.EDProducer('ICElectronConversionCalculator',
      input       = electronLabel,
      beamspot    = cms.InputTag("offlineBeamSpot"),
      conversions = cms.InputTag("reducedEgamma:reducedConversions")
  )

#  process.icElectronConversionCalculator = cms.EDProducer('ICElectronConversionFromPatCalculator',
#      input       = electronLabel
#  )
#if release in ['74X']:
process.load("RecoEgamma.ElectronIdentification.ElectronMVAValueMapProducer_cfi")
process.electronMVAValueMapProducer.src = electronLabel
process.electronMVAValueMapProducer.srcMiniAOD = electronLabel
#if release in ['74XMINIAOD']:
#New electron ID MVA producer to run without tracks
#  process.mvaNonTrigV025nsPHYS14 = cms.EDProducer("ICElectronIDMVAProducerMiniAOD",
#      verbose=cms.untracked.bool(False),
#      vertexTag=cms.InputTag('offlineSlimmedPrimaryVertices'),
#      electronTag=electronLabel,
#      reducedEBRecHitCollection=cms.InputTag('reducedEgamma','reducedEBRecHits',"PAT"),
#      reducedEERecHitCollection=cms.InputTag('reducedEgamma','reducedEERecHits',"PAT"),
#      method=cms.string("BDTSimpleCat"),
#      mvaWeightFile=cms.vstring(
#                                "EgammaAnalysis/ElectronTools/data/PHYS14/EIDmva_EB1_5_oldscenario2phys14_BDT.weights.xml",
#                                "EgammaAnalysis/ElectronTools/data/PHYS14/EIDmva_EB2_5_oldscenario2phys14_BDT.weights.xml",
#                                "EgammaAnalysis/ElectronTools/data/PHYS14/EIDmva_EE_5_oldscenario2phys14_BDT.weights.xml",
#                                "EgammaAnalysis/ElectronTools/data/PHYS14/EIDmva_EB1_10_oldscenario2phys14_BDT.weights.xml",
#                                "EgammaAnalysis/ElectronTools/data/PHYS14/EIDmva_EB2_10_oldscenario2phys14_BDT.weights.xml",
#                                "EgammaAnalysis/ElectronTools/data/PHYS14/EIDmva_EE_10_oldscenario2phys14_BDT.weights.xml",
#                                 ),
#     trig=cms.bool(False),
#     )

#Include electron cut-based IDs
#from PhysicsTools.SelectorUtils.tools.vid_id_tools import *
#process.load("RecoEgamma.ElectronIdentification.egmGsfElectronIDs_cfi")
#process.egmGsfElectronIDs.physicsObjectSrc = cms.InputTag('selectedElectrons')

#from PhysicsTools.SelectorUtils.centralIDRegistry import central_id_registry
#process.egmGsfElectronIDSequence = cms.Sequence(process.egmGsfElectronIDs)

#my_id_modules = ['RecoEgamma.ElectronIdentification.Identification.cutBasedElectronID_PHYS14_PU20bx25_V1_cff']
#if release in ['74XMINIAOD']:
#  my_id_modules = ['RecoEgamma.ElectronIdentification.Identification.cutBasedElectronID_PHYS14_PU20bx25_V1_miniAOD_cff']
#
#for idmod in my_id_modules:
#  setupAllVIDIdsInModule(process,idmod,setupVIDElectronSelection)
#
process.icElectronSequence+=cms.Sequence(
#   process.egmGsfElectronIDSequence+
   process.electronMVAValueMapProducer
   )

#Electron PF iso sequence:
process.load("CommonTools.ParticleFlow.Isolation.pfElectronIsolation_cff")
#  process.elPFIsoValueCharged03PFIdPFIso    = process.icElectronCharged.clone()
#process.elPFIsoValueCharged03PFIdPFIso    = process.elPFIsoValueCharged03PFId.clone()
process.elPFIsoValueChargedAll03PFIdPFIso = process.elPFIsoValueChargedAll03PFId.clone()
#process.elPFIsoValueGamma03PFIdPFIso      = process.elPFIsoValueGamma03PFId.clone()
#process.elPFIsoValueNeutral03PFIdPFIso    = process.elPFIsoValueNeutral03PFId.clone()
#process.elPFIsoValuePU03PFIdPFIso         = process.elPFIsoValuePU03PFId.clone()
#  process.elPFIsoDepositCharged.src     = electronLabel
process.elPFIsoDepositChargedAll.src  = electronLabel
#  process.elPFIsoDepositNeutral.src     = electronLabel
#  process.elPFIsoDepositGamma.src       = electronLabel
#  process.elPFIsoDepositPU.src          = electronLabel
process.elPFIsoValueCharged03PFIdPFIso = cms.EDProducer('ICRecoElectronIsolation',
  input        = electronLabel,
  deltaR       = cms.double(0.3),
  iso_type = cms.string("charged_iso") 
)    
process.elPFIsoValueGamma03PFIdPFIso = cms.EDProducer('ICRecoElectronIsolation',
  input        = electronLabel,
  deltaR       = cms.double(0.3),
  iso_type = cms.string("photon_iso") 
)    
process.elPFIsoValueNeutral03PFIdPFIso = cms.EDProducer('ICRecoElectronIsolation',
  input        = electronLabel,
  deltaR       = cms.double(0.3),
  iso_type = cms.string("neutral_iso") 
)    
process.elPFIsoValuePU03PFIdPFIso = cms.EDProducer('ICRecoElectronIsolation',
  input        = electronLabel,
  deltaR       = cms.double(0.3),
  iso_type = cms.string("pu_iso") 
)    

process.electronPFIsolationValuesSequence = cms.Sequence(
      process.elPFIsoValueCharged03PFIdPFIso+
      process.elPFIsoValueChargedAll03PFIdPFIso+
      process.elPFIsoValueGamma03PFIdPFIso+
      process.elPFIsoValueNeutral03PFIdPFIso+
      process.elPFIsoValuePU03PFIdPFIso
      )

if release in ['74XMINIAOD']:
  process.elPFIsoDepositGamma.ExtractorPSet.ComponentName = cms.string("CandViewExtractor")
process.icElectronSequence += cms.Sequence(
#      process.electronPFIsolationDepositsSequence+
    process.elPFIsoDepositChargedAll+
    process.electronPFIsolationValuesSequence
    )
#Htt electron vetoes:
#process.elPFIsoValueGamma03PFIdPFIso.deposits[0].vetos = (
#    cms.vstring('EcalEndcaps:ConeVeto(0.08)','EcalBarrel:ConeVeto(0.08)'))
#process.elPFIsoValueNeutral03PFIdPFIso.deposits[0].vetos = (
#    cms.vstring())
#process.elPFIsoValuePU03PFIdPFIso.deposits[0].vetos = (
#    cms.vstring())
#process.elPFIsoValueCharged03PFIdPFIso.deposits[0].vetos = (
#    cms.vstring('EcalEndcaps:ConeVeto(0.015)'))
process.elPFIsoValueChargedAll03PFIdPFIso.deposits[0].vetos = (
    cms.vstring('EcalEndcaps:ConeVeto(0.015)','EcalBarrel:ConeVeto(0.01)'))

if release in ['74XMINIAOD']:
  process.elPFIsoValueCharged04PFIdPFIso = cms.EDProducer('ICElectronIsolation',
    input        = electronLabel,
    deltaR       = cms.double(0.4),
    iso_type = cms.string("charged_iso") 
  )    
  process.elPFIsoValueGamma04PFIdPFIso = cms.EDProducer('ICElectronIsolation',
    input        = electronLabel,
    deltaR       = cms.double(0.4),
    iso_type = cms.string("photon_iso") 
  )    
  process.elPFIsoValueNeutral04PFIdPFIso = cms.EDProducer('ICElectronIsolation',
    input        = electronLabel,
    deltaR       = cms.double(0.4),
    iso_type = cms.string("neutral_iso") 
  )    
  process.elPFIsoValuePU04PFIdPFIso = cms.EDProducer('ICElectronIsolation',
    input        = electronLabel,
    deltaR       = cms.double(0.4),
    iso_type = cms.string("pu_iso") 
  )    


  process.elPFIsoValueChargedAll04PFIdPFIso = process.elPFIsoValueChargedAll04PFId.clone()
  process.electronPFIsolationValuesSequence +=cms.Sequence(
    process.elPFIsoValueCharged04PFIdPFIso+
    process.elPFIsoValueGamma04PFIdPFIso+
    process.elPFIsoValuePU04PFIdPFIso+
    process.elPFIsoValueNeutral04PFIdPFIso+
    process.elPFIsoValueChargedAll04PFIdPFIso
  )




process.icElectronProducer = producers.icElectronProducer.clone(
  branch                    = cms.string("electrons"),
  input                     = cms.InputTag("selectedElectrons"),
  includeConversionMatches  = cms.bool(True),
  inputConversionMatches    = cms.InputTag("icElectronConversionCalculator"),
  includeVertexIP           = cms.bool(True),
  inputVertices             = vtxLabel,
  includeBeamspotIP         = cms.bool(True),
  inputBeamspot             = cms.InputTag("offlineBeamSpot"),
  includeFloats = cms.PSet(
     mvaNonTrigSpring15    = cms.InputTag("electronMVAValueMapProducer:ElectronMVAEstimatorRun2Phys14NonTrigValues")
  ),
  includePFIso03           = cms.bool(True),
  includePFIso04           = cms.bool(True)
)

#if release in ['74XMINIAOD']:
#  process.icElectronProducer.includeFloats = cms.PSet(
#     mvaNonTrigV025nsPHYS14    = cms.InputTag("mvaNonTrigV025nsPHYS14")
#  )

if release in ['74X']:
  process.icElectronProducer.includePFIso03 = cms.bool(False)

process.icElectronSequence += cms.Sequence(
  process.icElectronConversionCalculator+
  process.icElectronProducer
)


################################################################
# Muons
################################################################
process.icMuonSequence = cms.Sequence()
if release in ['74X']: muons = cms.InputTag("muons")
if release in ['74XMINIAOD']: muons = cms.InputTag("slimmedMuons")

process.load("CommonTools.ParticleFlow.Isolation.pfMuonIsolation_cff")
#process.muPFIsoDepositCharged.src     = muons
process.muPFIsoDepositChargedAll.src  = muons #All charged isolation is not stored in reco/pat
#process.muPFIsoDepositNeutral.src     = muons
#process.muPFIsoDepositGamma.src       = muons
#process.muPFIsoDepositPU.src          = muons
#process.icMuonSequence += cms.Sequence(
#    process.muonPFIsolationDepositsSequence+
#    process.muonPFIsolationValuesSequence
#    )

#process.muPFIsoValueCharged03PFIso = process.muPFIsoValueCharged03.clone()
process.muPFIsoValueChargedAll03PFIso = process.muPFIsoValueChargedAll03.clone()
#process.muPFIsoValueGamma03PFIso = process.muPFIsoValueGamma03.clone()
#process.muPFIsoValueNeutral03PFIso = process.muPFIsoValueNeutral03.clone()
#process.muPFIsoValuePU03PFIso = process.muPFIsoValuePU03.clone()
process.muPFIsoValueCharged03PFIso = cms.EDProducer('ICMuonIsolation',
  input        = muons,
  deltaR       = cms.double(0.3),
  iso_type = cms.string("charged_iso") 
)    
process.muPFIsoValueGamma03PFIso = cms.EDProducer('ICMuonIsolation',
  input        = muons,
  deltaR       = cms.double(0.3),
  iso_type = cms.string("photon_iso") 
)    
process.muPFIsoValueNeutral03PFIso = cms.EDProducer('ICMuonIsolation',
  input        = muons,
  deltaR       = cms.double(0.3),
  iso_type = cms.string("neutral_iso") 
)    
process.muPFIsoValuePU03PFIso = cms.EDProducer('ICMuonIsolation',
  input        = muons,
  deltaR       = cms.double(0.3),
  iso_type = cms.string("pu_iso") 
)    

process.muonPFIsolationValuesSequence = cms.Sequence(
   process.muPFIsoValueCharged03PFIso+
   process.muPFIsoValueChargedAll03PFIso+
   process.muPFIsoValueGamma03PFIso+
   process.muPFIsoValueNeutral03PFIso+
   process.muPFIsoValuePU03PFIso
   )





process.muPFIsoValueCharged04PFIso = cms.EDProducer('ICMuonIsolation',
  input        = muons,
  deltaR       = cms.double(0.4),
  iso_type = cms.string("charged_iso") 
)    
process.muPFIsoValueGamma04PFIso = cms.EDProducer('ICMuonIsolation',
  input        = muons,
  deltaR       = cms.double(0.4),
  iso_type = cms.string("photon_iso") 
)    
process.muPFIsoValueNeutral04PFIso = cms.EDProducer('ICMuonIsolation',
  input        = muons,
  deltaR       = cms.double(0.4),
  iso_type = cms.string("neutral_iso") 
)    
process.muPFIsoValuePU04PFIso = cms.EDProducer('ICMuonIsolation',
  input        = muons,
  deltaR       = cms.double(0.4),
  iso_type = cms.string("pu_iso") 
)    
process.muPFIsoValueChargedAll04PFIso = process.muPFIsoValueChargedAll04.clone()
process.muonPFIsolationValuesSequence +=cms.Sequence(
  process.muPFIsoValueCharged04PFIso+
  process.muPFIsoValueGamma04PFIso+
  process.muPFIsoValuePU04PFIso+
  process.muPFIsoValueNeutral04PFIso+
  process.muPFIsoValueChargedAll04PFIso
)

process.icMuonSequence += cms.Sequence(
    process.muPFIsoDepositChargedAll+
    process.muonPFIsolationValuesSequence
    )



process.icMuonProducer = producers.icMuonProducer.clone(
  branch                    = cms.string("muons"),
  input                     = cms.InputTag("selectedMuons"),
  isPF                      = cms.bool(False),
  includeVertexIP           = cms.bool(True),
  inputVertices             = vtxLabel,
  includeBeamspotIP         = cms.bool(True),
  inputBeamspot             = cms.InputTag("offlineBeamSpot"),
  includeFloats = cms.PSet(
  ),
  requestTracks           = cms.bool(True),
  includePFIso03           = cms.bool(True),
  includePFIso04           = cms.bool(True)
)
if release in ['74XMINIAOD']: process.icMuonProducer.isPF = cms.bool(False)

process.icMuonSequence += cms.Sequence(
  process.icMuonProducer
)

################################################################
# Taus
################################################################
import UserCode.ICHiggsTauTau.tau_discriminators_cfi as tauIDs

process.icTauProducer = producers.icTauProducer.clone(
  input                   = cms.InputTag("selectedTaus"),
  inputVertices           = vtxLabel,
  includeVertexIP         = cms.bool(True),
  requestTracks           = cms.bool(True),
  tauIDs = tauIDs.fullNewIds
)

if release in ['74XMINIAOD']:
  process.icTauProducer = cms.EDProducer("ICPFTauFromPatProducer",
    branch                  = cms.string("taus"),
    input                   = cms.InputTag("selectedTaus"),
    inputVertices           = vtxLabel,
    includeVertexIP         = cms.bool(True),
    requestTracks           = cms.bool(False),
    tauIDs = cms.PSet()
  )

process.icTauSequence = cms.Sequence(
  process.icTauProducer
)

# ################################################################
# # Jets
# ################################################################
from RecoJets.JetProducers.ak4PFJets_cfi import ak4PFJets
if release in ['74XMINIAOD']:
  #rebuild ak4 chs jets as in  https://twiki.cern.ch/twiki/bin/view/CMSPublic/WorkBookMiniAOD#Jets
  process.load('PhysicsTools.PatAlgos.slimming.unpackedTracksAndVertices_cfi')
  process.pfchs=cms.EDFilter("CandPtrSelector",src=cms.InputTag("packedPFCandidates"),cut=cms.string("fromPV"))
  process.ak4PFJetsCHS = ak4PFJets.clone(src='pfchs',doAreaFastjet=True)

  #Also make non-chs jets:
  process.ak4PFJets = ak4PFJets.clone(src='packedPFCandidates',doAreaFastjet=True)
   
  process.selectedSlimmedJetsAK4 = cms.EDFilter("PATJetRefSelector",
      src = cms.InputTag("slimmedJets"),
      cut = cms.string("pt > 15")
      )

if release in ['74X']:
  process.load("CommonTools.ParticleFlow.pfNoPileUpJME_cff")
  process.ak4PFJetsCHS = ak4PFJets.clone(src = cms.InputTag("pfNoPileUpJME"),doAreaFastjet=True)
  process.ak4PFJets = ak4PFJets.clone(doAreaFastjet=True)


if release in ['74XMINIAOD']:
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

process.pfJetPartonMatches = cms.EDProducer("JetPartonMatcher",
     jets = cms.InputTag("ak4PFJetsCHS"),
     coneSizeToAssociate = cms.double(0.3),
     partons = cms.InputTag("jetPartons")
)

process.pfJetFlavourAssociation = cms.EDProducer("JetFlavourIdentifier",
     srcByReference = cms.InputTag("pfJetPartonMatches"),
     physicsDefinition = cms.bool(False)
)

process.icPFJetFlavourCalculator = cms.EDProducer('ICJetFlavourCalculator',
     input       = cms.InputTag("ak4PFJetsCHS"),
     flavourMap  = cms.InputTag("pfJetFlavourAssociation")
)


if release in ['74XMINIAOD']:
   process.jetPartons.src = cms.InputTag("prunedGenParticles")



 # Jet energy corrections
 # ----------------------
process.ak4PFL1FastjetCHS = cms.ESProducer("L1FastjetCorrectionESProducer",
#    srcRho = cms.InputTag("kt6PFJets", "rho"),
    srcRho = cms.InputTag("fixedGridRhoFastjetAll"),
    algorithm = cms.string('AK4PFchs'),
    level = cms.string('L1FastJet')
)
process.ak4PFL2RelativeCHS = cms.ESProducer("LXXXCorrectionESProducer",
    algorithm = cms.string('AK4PFchs'),
    level = cms.string('L2Relative')
)
process.ak4PFL3AbsoluteCHS = cms.ESProducer("LXXXCorrectionESProducer",
    algorithm = cms.string('AK4PFchs'),
    level = cms.string('L3Absolute')
)
process.ak4PFResidualCHS = cms.ESProducer("LXXXCorrectionESProducer",
    algorithm = cms.string('AK4PFchs'),
    level = cms.string('L2L3Residual')
)

#Corrections applied to miniaod slimmedJets
pfJECS = cms.PSet(
  L1FastJet  = cms.string("ak4PFL1FastjetCHS"),
  L2Relative = cms.string("ak4PFL2RelativeCHS"),
  L3Absolute = cms.string("ak4PFL3AbsoluteCHS")
)
#if isData: pfJECS.append(
#  L2L3Residual = cms.string("ak4PFResidualCHS")
#)

 # b-tagging
 # ---------
process.load("RecoJets.JetAssociationProducers.ak4JTA_cff")
from RecoJets.JetAssociationProducers.ak4JTA_cff import ak4JetTracksAssociatorAtVertex
process.load("RecoBTag.Configuration.RecoBTag_cff")
import RecoBTag.Configuration.RecoBTag_cff as btag
process.jetTracksAssociatorAtVertexAK4PFCHS = ak4JetTracksAssociatorAtVertex.clone(
  jets = cms.InputTag("ak4PFJetsCHS")
)

if release in ['74XMINIAOD']:
  process.jetTracksAssociatorAtVertexAK4PFCHS.tracks = cms.InputTag("unpackedTracksAndVertices")
  process.jetTracksAssociatorAtVertexAK4PFCHS.pvSrc = cms.InputTag("unpackedTracksAndVertices")

#if isEmbedded:
#  process.jetTracksAssociatorAtVertexAK5PF.tracks = cms.InputTag("tmfTracks")

process.impactParameterTagInfosAK4PFCHS = btag.pfImpactParameterTagInfos.clone(
  #jetTracks = cms.InputTag('jetTracksAssociatorAtVertexAK4PFCHS')
)
if release in ['74XMINIAOD']:
  process.impactParameterTagInfosAK4PFCHS.computeGhostTrack=cms.bool(False)
  process.impactParameterTagInfosAK4PFCHS.candidates=cms.InputTag("packedPFCandidates")
  #process.impactParameterTagInfosAK4PFCHS.primaryVertex = cms.InputTag("offlineSlimmedPrimaryVertices")
  process.impactParameterTagInfosAK4PFCHS.primaryVertex = cms.InputTag("unpackedTracksAndVertices")

process.secondaryVertexTagInfosAK4PFCHS = btag.pfSecondaryVertexTagInfos.clone(
  trackIPTagInfos = cms.InputTag('impactParameterTagInfosAK4PFCHS')
)
##Btag discriminators need to be properly updated 
process.simpleSecondaryVertexHighEffBJetTagsAK4PFCHS = btag.pfSimpleSecondaryVertexHighEffBJetTags.clone (
  tagInfos = cms.VInputTag('secondaryVertexTagInfosAK4PFCHS')
)
process.simpleSecondaryVertexHighPurBJetTagsAK4PFCHS = btag.pfSimpleSecondaryVertexHighPurBJetTags.clone (
  tagInfos = cms.VInputTag('secondaryVertexTagInfosAK4PFCHS')
)
process.combinedSecondaryVertexBJetTagsAK4PFCHS = btag.pfCombinedSecondaryVertexBJetTags.clone (
  tagInfos = cms.VInputTag('impactParameterTagInfosAK4PFCHS', 'secondaryVertexTagInfosAK4PFCHS')
)

process.btaggingSequenceAK4PF = cms.Sequence(
  process.jetTracksAssociatorAtVertexAK4PFCHS
  +process.impactParameterTagInfosAK4PFCHS
  +process.secondaryVertexTagInfosAK4PFCHS
  +process.simpleSecondaryVertexHighEffBJetTagsAK4PFCHS
  +process.simpleSecondaryVertexHighPurBJetTagsAK4PFCHS
  +process.combinedSecondaryVertexBJetTagsAK4PFCHS
 )

 # Pileup ID
 # ---------
 # Recalculated puJetId isn't the same as miniaod stored - should investigate 
stdalgos = cms.VPSet()
from RecoJets.JetProducers.PileupJetIDParams_cfi import *
stdalgos = cms.VPSet(full_5x_chs,cutbased)

process.puJetMva = cms.EDProducer('PileupJetIdProducer',
    produceJetIds = cms.bool(True),
    jetids = cms.InputTag(""),
    runMvas = cms.bool(True),
    #jets = cms.InputTag("slimmedJets"),
    jets = cms.InputTag("ak4PFJetsCHS"),
    vertexes = cms.InputTag("offlinePrimaryVertices"),
#    vertexes = cms.InputTag("unpackedTracksAndVertices"),
    algos = cms.VPSet(stdalgos),
#    rho     = cms.InputTag("kt6PFJets", "rho"),
    rho     = cms.InputTag("fixedGridRhoFastjetAll"),
    jec     = cms.string("AK4PFchs"),
    applyJec = cms.bool(True),
    inputIsCorrected = cms.bool(False),
    residualsFromTxt = cms.bool(False),
    residualsTxt     = cms.FileInPath("RecoJets/JetProducers/data/dummy.txt"),
)

if release in ['74XMINIAOD']:
  process.puJetMva.vertexes = cms.InputTag("offlineSlimmedPrimaryVertices")

if release in ['70X', '70XMINIAOD', '74X', '74XMINIAOD']:
  process.puJetMva.residualsTxt = cms.FileInPath("RecoJets/JetProducers/BuildFile.xml")



 # Producer
 # --------
process.icPFJetProducer = producers.icPFJetProducer.clone(
    branch                    = cms.string("ak4PFJetsCHS"),
    input                     = cms.InputTag("ak4PFJetsCHS"),
    srcConfig = cms.PSet(
      includeJetFlavour         = cms.bool(True),
      inputJetFlavour           = cms.InputTag("icPFJetFlavourCalculator"),
      applyJECs                 = cms.bool(True),
      includeJECs               = cms.bool(True),
      JECs                      = pfJECS,
      applyCutAfterJECs         = cms.bool(True),
      cutAfterJECs              = cms.string("pt > 15.0"),
      inputSVInfo               = cms.InputTag(""),
      requestSVInfo             = cms.bool(False),
      BTagDiscriminators        = cms.PSet(
        simpleSecondaryVertexHighEffBJetTags = cms.InputTag("simpleSecondaryVertexHighEffBJetTagsAK4PFCHS"),
        simpleSecondaryVertexHighPurBJetTags = cms.InputTag("simpleSecondaryVertexHighPurBJetTagsAK4PFCHS"),
        combinedSecondaryVertexBJetTags      = cms.InputTag("combinedSecondaryVertexBJetTagsAK4PFCHS"),
        #include this discriminator again under a different name to be compatible with miniAOD naming conventions
        combinedInclusiveSecondaryVertexV2BJetTags      = cms.InputTag("combinedSecondaryVertexBJetTagsAK4PFCHS")
      )
    ),
    destConfig = cms.PSet(
      includePileupID       = cms.bool(True), #rerunning the pu MVA on the jet collection created in miniAOD is possible in newer CMSSW versions but not yet in 72
      inputPileupID         = cms.InputTag("puJetMva", "fullDiscriminant"),
      includeTrackBasedVars = cms.bool(False),
      inputTracks           = cms.InputTag("generalTracks"),
      inputVertices         = vtxLabel,
      requestTracks         = cms.bool(False)
    )
)

if isData:
  process.icPFJetProducer.srcConfig.includeJetFlavour = cms.bool(False)


if release in ['74XMINIAOD']:
  process.icPFJetProducer.branch=cms.string("ak4PFJetsCHSReclustered")
#  process.icPFJetProducer.destConfig.includePileupID=cms.bool(False)
#  process.icPFJetProducer.destConfig.inputVertices=cms.InputTag("unpackedTracksAndVertices")

# if isPhys14:
#process.selectedPFJets = cms.EDFilter("PFJetRefSelector",
#    src = cms.InputTag("ak4PFJetsCHS"),
#    cut = cms.string("pt > 15")
#    )



  process.icPFJetProducerFromPat = producers.icPFJetFromPatProducer.clone(
      branch                    = cms.string("ak4PFJetsCHS"),
      input                     = cms.InputTag("selectedSlimmedJetsAK4"),
      srcConfig = cms.PSet(
        isSlimmed               = cms.bool(True),
        includeJetFlavour       = cms.bool(True),
        includeJECs             = cms.bool(True),
        inputSVInfo             = cms.InputTag(""),
        requestSVInfo           = cms.bool(False)
      ),
     destConfig = cms.PSet(
       includePileupID         = cms.bool(True),
       inputPileupID           = cms.InputTag("puJetMva", "fullDiscriminant"),
       includeTrackBasedVars   = cms.bool(False),
       inputTracks             = cms.InputTag("unpackedTracksAndVertices"),
       inputVertices           = cms.InputTag("unpackedTracksAndVertices"),
       requestTracks           = cms.bool(False)
      )
  )

process.icPFJetSequence = cms.Sequence()


# # 74X: error loading mva file
# if release in ['42X', '53X', '70X']:
#   process.icPFJetSequence += cms.Sequence(
#     process.puJetMva
#   )
if release in ['74XMINIAOD']:
  process.icPFJetSequence += cms.Sequence(
     process.pfchs+
     process.selectedSlimmedJetsAK4+
     process.unpackedTracksAndVertices+
     process.icPFJetProducerFromPat
     )
if release in ['74X']:
  process.icPFJetSequence += cms.Sequence(
    process.pfNoPileUpJMESequence
    )
if release in ['74X']:#, '74XMINIAOD']:
  process.icPFJetProducer.srcConfig.BTagDiscriminators = cms.PSet()
  process.icPFJetSequence += cms.Sequence(
      process.ak4PFJetsCHS+
      process.puJetMva+ 
      process.jetPartons+
      process.pfJetPartonMatches+
      process.pfJetFlavourAssociation+
      process.icPFJetFlavourCalculator+
      process.btaggingSequenceAK4PF+
      process.icPFJetProducer #Not from slimmed jets!
      )

#jif isData:
#  process.icPFJetSequence.remove(process.jetPartons) 
#  process.icPFJetSequence.remove(process.pfJetPartonMatches)
#  process.icPFJetSequence.remove(process.pfJetFlavourAssociation) 
#  process.icPFJetSequence.remove(process.icPFJetFlavourCalculator)
#if release in ['74XMINIAOD']:
#  process.icPFJetSequence.remove(process.puJetMva) #This works for jets built from PackedCandidates in CMSSW74X but not yet in 72


# ################################################################
# # MVA MET and PF MET
# ################################################################
process.load('JetMETCorrections.Configuration.JetCorrectionProducers_cff')
process.load('RecoMET.METPUSubtraction.mvaPFMET_cff')
process.load("RecoJets.JetProducers.ak4PFJets_cfi")

from RecoMET.METProducers.PFMET_cfi import pfMet

process.pfMetRe = pfMet.clone(src = "particleFlow")

if release in ['74XMINIAOD']:
  process.pfMetRe = pfMet.clone(src = "packedPFCandidates")
  process.pfMetRe.calculateSignificance = False # this can't be easily implemented on packed PF candidates at the moment

if release in ['74X']:
  process.icPfMetProducer = producers.icMetProducer.clone(
                            input = cms.InputTag("pfMetRe"),
                            branch = cms.string("pfMet"),
                            includeCustomID = cms.bool(False),
                            inputCustomID = cms.InputTag("")
                            )

if release in ['74XMINIAOD']:
  process.icPfMetProducer = producers.icMetFromPatProducer.clone(
                           branch = cms.string("pfMet"),
                           getUncorrectedMet=cms.bool(False)
                           )
  #process.icPfMetUncorrProducer = producers.icMetFromPatProducer.clone(
  #                         branch = cms.string("pfMetUncorr"),
  #                         getUncorrectedMet=cms.bool(True)
  #                         )


process.icPfMetSequence = cms.Sequence(
  process.pfMetRe+
  process.icPfMetProducer#+
  #process.icPfMetUncorrProducer
)

if release in ['74XMINIAOD']:
  process.icPfMetSequence.remove(process.pfMetRe)

from ICAnalysis.MVAMETPairProducer.mvaPFMET_cff_leptons_72X import mvaMetPairs

if release in ['74XMINIAOD']:
  process.ak4PFJets.src = cms.InputTag("packedPFCandidates")

process.ak4PFJets.doAreaFastjet = cms.bool(True)

from JetMETCorrections.Configuration.DefaultJEC_cff import ak4PFJetsL1FastL2L3
from JetMETCorrections.Configuration.DefaultJEC_cff import ak4PFJetsL1FastL2L3

process.mvaMetPairsTT = mvaMetPairs.clone(
 srcLeg1 = cms.InputTag('slimmedTaus'),
 srcLeg2 = cms.InputTag('slimmedTaus'),
 leg1Pt = cms.double(40.0),
 leg1Eta = cms.double(2.3),
 leg2Pt = cms.double(40.0),
 leg2Eta = cms.double(2.3),
 minDeltaR = cms.double(0.49)
# srcPFCandidates = cms.InputTag('packedPFCandidates'),
# srcVertices = cms.InputTag("offlineSlimmedPrimaryVertices") 
)


process.mvaMetPairsMT = mvaMetPairs.clone(
 srcLeg1 = cms.InputTag('slimmedMuons'),
 srcLeg2 = cms.InputTag('slimmedTaus'),
 leg1Pt = cms.double(7.0),
 leg1Eta = cms.double(2.6),
 leg2Pt = cms.double(18.0),
 leg2Eta = cms.double(2.6),
 minDeltaR = cms.double(0.49)
# srcPFCandidates = cms.InputTag('packedPFCandidates'),
# srcVertices = cms.InputTag("offlineSlimmedPrimaryVertices") 
)
process.mvaMetPairsET = mvaMetPairs.clone(
  srcLeg1 = cms.InputTag('slimmedElectrons'),
  srcLeg2 = cms.InputTag('slimmedTaus'),
  leg1Pt = cms.double(10.0),
  leg1Eta = cms.double(2.6),
  leg2Pt = cms.double(18.0),
  leg2Eta = cms.double(2.6),
  minDeltaR = cms.double(0.49)
#  srcPFCandidates = cms.InputTag('packedPFCandidates'),
#  srcVertices = cms.InputTag('offlineSlimmedPrimaryVertices')
  )
process.mvaMetPairsEM = mvaMetPairs.clone(
  srcLeg1 = cms.InputTag('slimmedElectrons'),
  srcLeg2 = cms.InputTag('slimmedMuons'),
  leg1Pt = cms.double(9.5),
  leg1Eta = cms.double(2.6),
  leg2Pt = cms.double(9),
  leg2Eta = cms.double(2.6),
  minDeltaR = cms.double(0.29)
#  srcPFCandidates = cms.InputTag('packedPFCandidates'),
#  srcVertices = cms.InputTag('offlineSlimmedPrimaryVertices')
  )
process.puJetIdForPFMVAMEt.rho = cms.InputTag("fixedGridRhoFastjetAll")

if release in ['74XMINIAOD']:
  process.pfMVAMEt.srcPFCandidates = cms.InputTag("packedPFCandidates")
  process.pfMVAMEt.srcVertices = cms.InputTag("offlineSlimmedPrimaryVertices")
#process.pfMVAMEt.srcLeptons = cms.VInputTag("selectedElectrons","selectedMuons","selectedTaus")
#  process.pfMVAMEt.srcLeptons = cms.VInputTag("slimmedElectrons","slimmedMuons","slimmedTaus")
  process.puJetIdForPFMVAMEt.jec = cms.string("AK4PF")
  process.puJetIdForPFMVAMEt.vertexes = cms.InputTag("offlineSlimmedPrimaryVertices")
  process.mvaMetPairsEM.srcPFCandidates = cms.InputTag('packedPFCandidates')
  process.mvaMetPairsEM.srcVertices = cms.InputTag('offlineSlimmedPrimaryVertices')
  process.mvaMetPairsEM.srcRho = cms.InputTag('fixedGridRhoFastjetAll')
  process.mvaMetPairsET.srcPFCandidates = cms.InputTag('packedPFCandidates')
  process.mvaMetPairsET.srcVertices = cms.InputTag('offlineSlimmedPrimaryVertices')
  process.mvaMetPairsET.srcRho = cms.InputTag('fixedGridRhoFastjetAll')
  process.mvaMetPairsMT.srcPFCandidates = cms.InputTag('packedPFCandidates')
  process.mvaMetPairsMT.srcVertices = cms.InputTag('offlineSlimmedPrimaryVertices')
  process.mvaMetPairsMT.srcRho = cms.InputTag('fixedGridRhoFastjetAll')
  process.mvaMetPairsTT.srcPFCandidates = cms.InputTag('packedPFCandidates')
  process.mvaMetPairsTT.srcVertices = cms.InputTag('offlineSlimmedPrimaryVertices')
  process.mvaMetPairsTT.srcRho = cms.InputTag('fixedGridRhoFastjetAll')



if release in ['74X']:
#  process.pfMVAMEt.srcLeptons = cms.VInputTag("selectedElectrons","selectedMuons","selectedTaus")
  process.puJetIdForPFMVAMEt.jec = cms.string("AK4PF")
  process.mvaMetPairsEM.srcLeg1 = cms.InputTag("selectedElectrons")
  process.mvaMetPairsEM.srcLeg2 = cms.InputTag("selectedMuons")
  process.mvaMetPairsET.srcLeg1 = cms.InputTag("selectedElectrons")
  process.mvaMetPairsET.srcLeg2 = cms.InputTag("selectedTaus")
  process.mvaMetPairsMT.srcLeg1 = cms.InputTag("selectedMuons")
  process.mvaMetPairsMT.srcLeg2 = cms.InputTag("selectedTaus")
  process.mvaMetPairsTT.srcLeg1 = cms.InputTag("selectedTaus")
  process.mvaMetPairsTT.srcLeg2 = cms.InputTag("selectedTaus")

process.icMvaMetConcatenate = cms.EDProducer("ICPFMETConcatenate",
   concatenate = cms.VInputTag(
     cms.InputTag("mvaMetPairsTT"),
     cms.InputTag("mvaMetPairsMT"),
     cms.InputTag("mvaMetPairsET"),
     cms.InputTag("mvaMetPairsEM")
   )
)
process.icMvaMetIDConcatenate = cms.EDProducer("ICIDConcatenate",
   concatenate = cms.VInputTag(
     cms.InputTag("mvaMetPairsTT", "MVAMetId"),
     cms.InputTag("mvaMetPairsMT", "MVAMetId"),
     cms.InputTag("mvaMetPairsET", "MVAMetId"),
     cms.InputTag("mvaMetPairsEM", "MVAMetId")
   )
)
process.icMvaMetPairsProducer = producers.icMetProducer.clone(
   branch  = cms.string("pfMVAMetVector"),
   input   = cms.InputTag("icMvaMetConcatenate"),
   includeCustomID = cms.bool(True),
   inputCustomID = cms.InputTag("icMvaMetIDConcatenate")
)

if isData:
  process.calibratedAK4PFJetsForPFMVAMEt.correctors=cms.vstring("ak4PFL1FastL2L3Residual")





process.icMvaMetSequence = cms.Sequence(
   process.ak4PFJets+
   process.calibratedAK4PFJetsForPFMVAMEt+
   process.puJetIdForPFMVAMEt+
   process.mvaMetPairsTT+
   process.mvaMetPairsMT+
   process.mvaMetPairsET+
   process.mvaMetPairsEM+
   process.icMvaMetConcatenate+
   process.icMvaMetIDConcatenate+
   process.icMvaMetPairsProducer+ 
   process.pfMVAMEt
   )

process.icPfMVAMetProducer = cms.EDProducer('ICPFMetProducer',
  input = cms.InputTag("pfMVAMEt"),
  branch = cms.string("pfMVAMet"),
  includeCustomID = cms.bool(False),
  includeExternalMetsig = cms.bool(False),
  includeExternalMetsigMethod2 = cms.bool(False),
  metsig = cms.PSet(
    metsig = cms.InputTag("METSignificance","METSignificance"),
    metsigcov00 = cms.InputTag("METSignificance", "CovarianceMatrix00"),
    metsigcov01 = cms.InputTag("METSignificance", "CovarianceMatrix01"),
    metsigcov10 = cms.InputTag("METSignificance", "CovarianceMatrix10"),
    metsigcov11 = cms.InputTag("METSignificance", "CovarianceMatrix11")
    ),
  metsig_method2 = cms.PSet(
    metsig      = cms.InputTag("METSignificance","METSignificance"),
    metsigcov = cms.InputTag("METSignificance","METCovariance")
    ),
  inputCustomID = cms.InputTag("MVAMetID")
  )


process.icMvaMetSequence += cms.Sequence(
  process.icPfMVAMetProducer
  )

################################################################
# Simulation only: GenParticles, GenJets, PileupInfo
################################################################
process.icGenSequence = cms.Sequence()

process.load("SimGeneral.HepPDTESSource.pythiapdt_cfi")
process.prunedGenParticles = cms.EDProducer("GenParticlePruner",
  src = cms.InputTag("genParticles", "", "HLT"),
  select = cms.vstring(
    "drop  *",
    "keep status == 3 || status == 22 || status == 23",  # all status 3
    "keep abs(pdgId) == 11 || abs(pdgId) == 13 || abs(pdgId) == 15",  # all charged leptons
    "keep abs(pdgId) == 12 || abs(pdgId) == 14 || abs(pdgId) == 16",  # all neutrinos
    "keep++ abs(pdgId) == 15",  # keep full tau decay chain
    "+keep abs(pdgId) == 15 || abs(pdgId) == 11 || abs(pdgId) ==13", #also keep first charged lepton  parent
    "keep (4 <= abs(pdgId) <= 5)", # keep heavy flavour quarks
    "keep (21 <= abs(pdgId) <= 37)",# keep bosons
    "keep (400 <= abs(pdgId) < 600) || (4000 <= abs(pdgId) < 6000)", # keep b and c hadrons
    "keep abs(pdgId) = 10411 || abs(pdgId) = 10421 || abs(pdgId) = 10413 || abs(pdgId) = 10423 || abs(pdgId) = 20413 || abs(pdgId) = 20423 || abs(pdgId) = 10431 || abs(pdgId) = 10433 || abs(pdgId) = 20433", # additional c hadrons for jet fragmentation studies
    "keep abs(pdgId) = 10511 || abs(pdgId) = 10521 || abs(pdgId) = 10513 || abs(pdgId) = 10523 || abs(pdgId) = 20513 || abs(pdgId) = 20523 || abs(pdgId) = 10531 || abs(pdgId) = 10533 || abs(pdgId) = 20533 || abs(pdgId) = 10541 || abs(pdgId) = 10543 || abs(pdgId) = 20543" # additional b hadrons for jet fragmentation studies
  )
)

#process.prunedGenParticlesTaus = cms.EDProducer("GenParticlePruner",
#  src = cms.InputTag("genParticles", "", "HLT"),
#  select = cms.vstring(
#    "drop  *",
#    "keep++ abs(pdgId) == 15",  # keep full tau decay chain
#  )
#)

if release in ['74XMINIAOD']:
#  process.prunedGenParticlesTaus.src = cms.InputTag("prunedGenParticles","","PAT")
  process.prunedGenParticles.src = cms.InputTag("prunedGenParticles", "", "PAT")


process.icGenParticleProducer = producers.icGenParticleProducer.clone(
  input   = cms.InputTag("prunedGenParticles"),
  includeMothers = cms.bool(True),
  includeDaughters = cms.bool(True)
)

#process.icGenParticleTauProducer = producers.icGenParticleProducer.clone(
#  input   = cms.InputTag("prunedGenParticlesTaus"),
#  branch = cms.string("genParticlesTaus"),
#  includeMothers = cms.bool(True),
#  includeDaughters = cms.bool(True)
#)


process.load("RecoJets.Configuration.GenJetParticles_cff")
process.genParticlesForJets.ignoreParticleIDs = cms.vuint32(
  1000022, 2000012, 2000014,
  2000016, 1000039, 5000039,
  4000012, 9900012, 9900014,
  9900016, 39, 12, 14, 16
)
if release in ['74XMINIAOD']:
  # But of course this won't work because genParticlesForJets(InputGenJetsParticleSelector)
  # requires a vector<GenParticle> input. There's no alternative filter for the PackedGenParticle
  # type at the moment. Probably we could make our own generic cut-string selector, but
  # not in this package
  process.genParticlesForJets.src = cms.InputTag("packedGenParticles")

process.load("RecoJets.JetProducers.ak4GenJets_cfi")
process.ak4GenJetsNoNuBSM  =  process.ak4GenJets.clone()

if release in ['74XMINIAOD']:
  process.ak4GenJetsNoNuBSM.src=cms.InputTag("packedGenParticles") #This still contains nus in 72, should be fixed in 74

process.selectedGenJets = cms.EDFilter("GenJetRefSelector",
  src = cms.InputTag("ak4GenJetsNoNuBSM"),
  cut = cms.string("pt > 10.0")
)

process.icGenJetProducer = producers.icGenJetProducer.clone(
  branch  = cms.string("genJets"),
  input   = cms.InputTag("selectedGenJets"),
  inputGenParticles = cms.InputTag("genParticles"),
  requestGenParticles = cms.bool(False)
)

if release in ['74XMINIAOD']:
  process.icGenJetProducer.branch = cms.string("genJetsReclustered")
  process.icGenJetProducer.inputGenParticles = cms.InputTag("prunedGenParticles")
  process.icGenJetProducer.isSlimmed  = cms.bool(True)
  process.icGenJetProducerFromSlimmed = producers.icGenJetProducer.clone(
    branch = cms.string("genJets"),
    input = cms.InputTag("slimmedGenJets"),
    inputGenParticles=cms.InputTag("genParticles"),
    requestGenParticles = cms.bool(False),
    isSlimmed = cms.bool(True)
  ) 

process.icPileupInfoProducer = producers.icPileupInfoProducer.clone()

if not isData:
  process.icGenSequence += (
    process.prunedGenParticles+
#    process.prunedGenParticlesTaus+
#    process.icGenParticleTauProducer+
    process.icGenParticleProducer
  )
  if release in ['74XMINIAOD']:
    process.icGenSequence += (
      process.ak4GenJetsNoNuBSM+
      process.selectedGenJets+
      process.icGenJetProducer+
      process.icGenJetProducerFromSlimmed+
      process.icPileupInfoProducer
    )
  if release in [ '74X']:
    process.icGenSequence += (
      process.genParticlesForJets+
      process.ak4GenJetsNoNuBSM+
      process.selectedGenJets+
      process.icGenJetProducer+
      process.icPileupInfoProducer
    )

#process.load("RecoJets.JetProducers.ak4GenJets_cfi")
#process.ak4GenJetsNoNuBSM  =  process.ak4GenJets.clone()
#process.selectedGenJetsAK4 = cms.EDFilter("GenJetRefSelector",
#  src = cms.InputTag("ak4GenJetsNoNuBSM"),
#  cut = cms.string("pt > 10.0")
#)
#process.icGenJetProducerAK4 = producers.icGenJetProducer.clone(
#  branch  = cms.string("ak4GenJets"),
#  input   = cms.InputTag("selectedGenJetsAK4"),
#  inputGenParticles = cms.InputTag("genParticles"),
#  requestGenParticles = cms.bool(False)
#)
#process.icGenSequence += (
#    process.ak4GenJetsNoNuBSM+
#    process.selectedGenJetsAK4+
#    process.icGenJetProducerAK4
#)



# ################################################################
# # Trigger
# ################################################################
from PhysicsTools.PatAlgos.tools.trigTools import *
process.icTriggerSequence = cms.Sequence()
# if release in ['42X', '53X']:
#   process.patTriggerSequence = cms.Sequence()
#   switchOnTrigger(process, sequence = 'patTriggerSequence', outputModule = '')
#   process.patTriggerPath = cms.Path(process.patTriggerSequence)
if release in ['70X', '74X']:
  process.patTriggerPath = cms.Path()
  switchOnTrigger(process, path = 'patTriggerPath', outputModule = '')

  process.icTriggerPathProducer = producers.icTriggerPathProducer.clone(
   branch = cms.string("triggerPaths"),
   input  = cms.InputTag("patTriggerEvent")
  )


  if isData:
    process.icTriggerSequence += cms.Sequence(
      process.patTrigger+
      process.patTriggerEvent+
     process.icTriggerPathProducer
    )


if release in ['70XMINIAOD', '74XMINIAOD']:
  process.patTriggerPath = cms.Path()
  switchOnTrigger(process, path = 'patTriggerPath',  outputModule = '')


process.icTriggerObjectSequence = cms.Sequence()
if isData:
  process.icTriggerPathProducer = producers.icTriggerPathProducer.clone(
   branch = cms.string("triggerPaths"),
   input  = cms.InputTag("TriggerResults","","HLT"),
   inputIsStandAlone = cms.bool(True),
   inputPrescales = cms.InputTag("patTrigger")
  )

  process.icTriggerSequence += cms.Sequence(
   #process.patTrigger+
   #process.patTriggerEvent+
   process.icTriggerPathProducer
)


if not isData :
  process.icEle12Mu23ObjectProducer = producers.icTriggerObjectProducer.clone(
      input   = cms.InputTag("patTriggerEvent"),
      branch = cms.string("triggerObjectsEle12Mu23"),
      hltPath = cms.string("HLT_Mu23_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL_v1"),
      inputIsStandAlone = cms.bool(False),
      storeOnlyIfFired = cms.bool(False)
      )
  
  process.icEle23Mu8ObjectProducer = producers.icTriggerObjectProducer.clone(
      input   = cms.InputTag("patTriggerEvent"),
      branch = cms.string("triggerObjectsEle23Mu8"),
      hltPath = cms.string("HLT_Mu8_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL_v1"),
      inputIsStandAlone = cms.bool(False),
      storeOnlyIfFired = cms.bool(False)
      )
  
  
  process.icEle22LooseTau20ObjectProducer = producers.icTriggerObjectProducer.clone(
      input   = cms.InputTag("patTriggerEvent"),
      branch = cms.string("triggerObjectsEle22LooseTau20"),
      hltPath = cms.string("HLT_Ele22_eta2p1_WP75_Gsf_LooseIsoPFTau20_v1"),
      inputIsStandAlone = cms.bool(False),
      storeOnlyIfFired = cms.bool(False)
      )
  
  process.icEle27GsfObjectProducer = producers.icTriggerObjectProducer.clone(
      input   = cms.InputTag("patTriggerEvent"),
      branch = cms.string("triggerObjectsEle27Gsf"),
      hltPath = cms.string("HLT_Ele27_eta2p1_WP75_Gsf_v1"),
      inputIsStandAlone = cms.bool(False),
      storeOnlyIfFired = cms.bool(False)
      )
  
  process.icEle32GsfObjectProducer = producers.icTriggerObjectProducer.clone(
      input   = cms.InputTag("patTriggerEvent"),
      branch = cms.string("triggerObjectsEle32Gsf"),
      hltPath = cms.string("HLT_Ele32_eta2p1_WP75_Gsf_v1"),
      inputIsStandAlone = cms.bool(False),
      storeOnlyIfFired = cms.bool(False)
      )

  process.icEle32LooseGsfObjectProducer = producers.icTriggerObjectProducer.clone(
      input   = cms.InputTag("patTriggerEvent"),
      branch = cms.string("triggerObjectsEle32GsfLoose"),
      hltPath = cms.string("HLT_Ele32_eta2p1_WPLoose_Gsf_v"),
      inputIsStandAlone = cms.bool(False),
      storeOnlyIfFired = cms.bool(False)
      )

  
  
  process.icEle22GsfObjectProducer = producers.icTriggerObjectProducer.clone(
      input   = cms.InputTag("patTriggerEvent"),
      branch = cms.string("triggerObjectsEle22Gsf"),
      hltPath = cms.string("HLT_Ele22_eta2p1_WP75_Gsf_v1"),
      inputIsStandAlone = cms.bool(False),
      storeOnlyIfFired = cms.bool(False)
      )
  
  
  process.icIsoMu17LooseTau20ObjectProducer = producers.icTriggerObjectProducer.clone(
      input   = cms.InputTag("patTriggerEvent"),
      branch = cms.string("triggerObjectsIsoMu17LooseTau20"),
      hltPath = cms.string("HLT_IsoMu17_eta2p1_LooseIsoPFTau20_v1"),
      inputIsStandAlone = cms.bool(False),
      storeOnlyIfFired = cms.bool(False)
      )
  
  
  process.icIsoMu16CaloMetObjectProducer = producers.icTriggerObjectProducer.clone(
      input   = cms.InputTag("patTriggerEvent"),
      branch = cms.string("triggerObjectsIsoMu16CaloMet30"),
      hltPath = cms.string("HLT_IsoMu16_eta2p1_CaloMET30_v1"),
      inputIsStandAlone = cms.bool(False),
      storeOnlyIfFired = cms.bool(False)
      )
  
  
  
  process.icIsoMu17IterTrkObjectProducer = producers.icTriggerObjectProducer.clone(
      input   = cms.InputTag("patTriggerEvent"),
      branch = cms.string("triggerObjectsIsoMu17IterTrk"),
      hltPath = cms.string("HLT_IsoMu17_eta2p1_v1"), 
      inputIsStandAlone = cms.bool(False),
      storeOnlyIfFired = cms.bool(False)
      )
  
  
  process.icIsoMu24IterTrkObjectProducer = producers.icTriggerObjectProducer.clone(
      input   = cms.InputTag("patTriggerEvent"),
      branch = cms.string("triggerObjectsIsoMu24"),
      hltPath = cms.string("HLT_IsoMu24_eta2p1_v1"),
      inputIsStandAlone = cms.bool(False),
      storeOnlyIfFired = cms.bool(False)
      )
  
  process.icIsoMu27IterTrkObjectProducer = producers.icTriggerObjectProducer.clone(
      input   = cms.InputTag("patTriggerEvent"),
      branch = cms.string("triggerObjectsIsoMu27"),
      hltPath = cms.string("HLT_IsoMu27_v1"), 
      inputIsStandAlone = cms.bool(False),
      storeOnlyIfFired = cms.bool(False)
      )
  
  
  process.icDoubleMediumTau40ObjectProducer = producers.icTriggerObjectProducer.clone(
      input   = cms.InputTag("patTriggerEvent"),
      branch = cms.string("triggerObjectsDoubleMediumTau40"),
      hltPath = cms.string("HLT_DoubleMediumIsoPFTau40_Trk1_eta2p1_Reg_v1"),
      inputIsStandAlone = cms.bool(False),
      storeOnlyIfFired = cms.bool(False)
      )
  
  process.icLooseTau50Met120ObjectProducer = producers.icTriggerObjectProducer.clone(
      input   = cms.InputTag("patTriggerEvent"),
      branch = cms.string("triggerObjectsLooseTau50Met120"),
      hltPath = cms.string("HLT_LooseIsoPFTau50_Trk30_eta2p1_MET120_v1"),
      inputIsStandAlone = cms.bool(False),
      storeOnlyIfFired = cms.bool(False)
      )
  
  process.icLooseTau50Met80ObjectProducer = producers.icTriggerObjectProducer.clone(
      input   = cms.InputTag("patTriggerEvent"),
      branch = cms.string("triggerObjectsLooseTau50Met80"),
      hltPath = cms.string("HLT_LooseIsoPFTau50_Trk30_eta2p1_MET80_v1"),
      inputIsStandAlone = cms.bool(False),
      storeOnlyIfFired = cms.bool(False)
      )
  
  
  process.icTriggerObjectSequence += cms.Sequence(
      process.icEle12Mu23ObjectProducer +
      process.icEle23Mu8ObjectProducer +
      process.icEle22LooseTau20ObjectProducer +
      process.icEle22GsfObjectProducer +
      process.icEle27GsfObjectProducer + 
      process.icEle32GsfObjectProducer +
#      process.icEle32GsfLooseObjectProducer +
      process.icIsoMu16CaloMetObjectProducer + 
      process.icIsoMu17LooseTau20ObjectProducer +
      process.icIsoMu24IterTrkObjectProducer +
      process.icIsoMu27IterTrkObjectProducer +
      process.icIsoMu17IterTrkObjectProducer +
      process.icDoubleMediumTau40ObjectProducer +
      process.icLooseTau50Met120ObjectProducer +
      process.icLooseTau50Met80ObjectProducer
      )

if isData:
  process.icDoubleMediumTau40ObjectProducer = producers.icTriggerObjectProducer.clone(
      input   = cms.InputTag("patTriggerEvent"),
      branch = cms.string("triggerObjectsDoubleMediumTau40"),
      hltPath = cms.string("HLT_DoubleMediumIsoPFTau40_Trk1_eta2p1_Reg_v"),
      inputIsStandAlone = cms.bool(False),
      storeOnlyIfFired = cms.bool(False)
      )

  process.icIsoMu17LooseTau20ObjectProducer = producers.icTriggerObjectProducer.clone(
      input   = cms.InputTag("patTriggerEvent"),
      branch = cms.string("triggerObjectsIsoMu17LooseTau20"),
      hltPath = cms.string("HLT_IsoMu17_eta2p1_LooseIsoPFTau20_v"),
      inputIsStandAlone = cms.bool(False),
      storeOnlyIfFired = cms.bool(False)
      )

  process.icIsoMu24IterTrkObjectProducer = producers.icTriggerObjectProducer.clone(
      input   = cms.InputTag("patTriggerEvent"),
      branch = cms.string("triggerObjectsIsoMu24"),
      hltPath = cms.string("HLT_IsoMu24_eta2p1_v"),
      inputIsStandAlone = cms.bool(False),
      storeOnlyIfFired = cms.bool(False)
      )

  process.icIsoMu27IterTrkObjectProducer = producers.icTriggerObjectProducer.clone(
      input   = cms.InputTag("patTriggerEvent"),
      branch = cms.string("triggerObjectsIsoMu27"),
      hltPath = cms.string("HLT_IsoMu27_v"), 
      inputIsStandAlone = cms.bool(False),
      storeOnlyIfFired = cms.bool(False)
      )


  process.icEle22LooseTau20ObjectProducer = producers.icTriggerObjectProducer.clone(
      input   = cms.InputTag("patTriggerEvent"),
      branch = cms.string("triggerObjectsEle22LooseTau20"),
      hltPath = cms.string("HLT_Ele22_eta2p1_WPLoose_Gsf_LooseIsoPFTau20_v"),
      inputIsStandAlone = cms.bool(False),
      storeOnlyIfFired = cms.bool(False)
      )
  
  process.icEle27GsfObjectProducer = producers.icTriggerObjectProducer.clone(
      input   = cms.InputTag("patTriggerEvent"),
      branch = cms.string("triggerObjectsEle27Gsf"),
      hltPath = cms.string("HLT_Ele27_eta2p1_WPLoose_Gsf_v"),
      inputIsStandAlone = cms.bool(False),
      storeOnlyIfFired = cms.bool(False)
      )
  
  process.icEle32GsfObjectProducer = producers.icTriggerObjectProducer.clone(
      input   = cms.InputTag("patTriggerEvent"),
      branch = cms.string("triggerObjectsEle32Gsf"),
      hltPath = cms.string("HLT_Ele32_eta2p1_WPTight_Gsf_v"),
      inputIsStandAlone = cms.bool(False),
      storeOnlyIfFired = cms.bool(False)
      )
  process.icEle12Mu23ObjectProducer = producers.icTriggerObjectProducer.clone(
      input   = cms.InputTag("patTriggerEvent"),
      branch = cms.string("triggerObjectsEle12Mu23"),
      hltPath = cms.string("HLT_Mu23_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL_v"),
      inputIsStandAlone = cms.bool(False),
      storeOnlyIfFired = cms.bool(False)
      )
  
  process.icEle23Mu8ObjectProducer = producers.icTriggerObjectProducer.clone(
      input   = cms.InputTag("patTriggerEvent"),
      branch = cms.string("triggerObjectsEle23Mu8"),
      hltPath = cms.string("HLT_Mu8_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL_v"),
      inputIsStandAlone = cms.bool(False),
      storeOnlyIfFired = cms.bool(False)
      )


  process.icTriggerObjectSequence += cms.Sequence(
    process.icDoubleMediumTau40ObjectProducer +
    process.icIsoMu17LooseTau20ObjectProducer +
    process.icEle22LooseTau20ObjectProducer +
    process.icEle27GsfObjectProducer +
    process.icEle32GsfObjectProducer +
    process.icIsoMu24IterTrkObjectProducer +
    process.icIsoMu27IterTrkObjectProducer +
    process.icEle12Mu23ObjectProducer +
    process.icEle23Mu8ObjectProducer 

    )


  
if release in ['74XMINIAOD']:
  for name in process.icTriggerObjectSequence.moduleNames():
    mod = getattr(process, name)
    mod.inputIsStandAlone = cms.bool(True)
    mod.input = cms.InputTag("selectedPatTrigger")

################################################################
# EventInfo
################################################################
process.icEventInfoProducer = producers.icEventInfoProducer.clone(
  includeJetRho       = cms.bool(True),
  isNlo               = cms.bool(False),
  lheProducer         = cms.InputTag("externalLHEProducer"),
  inputJetRho         = cms.InputTag("fixedGridRhoFastjetAll"),
  includeLeptonRho    = cms.bool(False),
  inputLeptonRho      = cms.InputTag("fixedGridRhoFastjetAll"),
  includeVertexCount  = cms.bool(True),
  inputVertices       = vtxLabel,
  includeCSCFilter    = cms.bool(False),
  inputCSCFilter      = cms.InputTag("BeamHaloSummary"),
)

if isNLO:
  process.icEventInfoProducer.isNlo = cms.bool(True)
  

process.icEventInfoSequence = cms.Sequence(
  process.icEventInfoProducer
)


################################################################
# Event
################################################################
process.icEventProducer = producers.icEventProducer.clone()



process.p = cms.Path(
  process.ic74XSequence+
  process.icMiniAODSequence+
  process.icSelectionSequence+
  process.pfParticleSelectionSequence+
  process.icVertexSequence+
#  process.icPFSequence+
  process.icElectronSequence+
  process.icMuonSequence+
  process.icTauSequence+
  process.icTauProducer+
  #process.icL1ExtraMETProducer+
 # process.icTrackSequence+
  process.icPfMetSequence+
  process.icMvaMetSequence+
  process.icGenSequence+
  process.icPFJetSequence+
  process.icTriggerSequence+
  process.icTriggerObjectSequence+
  process.icEventInfoSequence+
  #process.patDefaultSequence+
  process.icEventProducer
)

# process.schedule = cms.Schedule(process.patTriggerPath, process.p)
process.schedule = cms.Schedule(process.p)

#print process.dumpPython()
