import FWCore.ParameterSet.Config as cms
process = cms.Process("MAIN")
import sys

################################################################
# Read Options
################################################################
import FWCore.ParameterSet.VarParsing as parser
opts = parser.VarParsing ('analysis')
#opts.register('file', 'file:InputAOD_gg.root', parser.VarParsing.multiplicity.singleton,
opts.register('file', 'file:miniaod_gg.root', parser.VarParsing.multiplicity.singleton,
#opts.register('file', 'root://xrootd.unl.edu//store/mc/Phys14DR/GluGluToHToTauTau_M-125_13TeV-powheg-pythia6/MINIAODSIM/PU20bx25_tsg_PHYS14_25_V1-v1/00000/2405749F-8B6F-E411-88EE-848F69FD2910.root', parser.VarParsing.multiplicity.singleton,
#opts.register('file', 'root://xrootd.unl.edu//store/mc/Phys14DR/VBF_HToTauTau_M-125_13TeV-powheg-pythia6/MINIAODSIM/PU40bx25_PHYS14_25_V1-v1/00000/36224FE2-0571-E411-9664-00266CFAE30C.root', parser.VarParsing.multiplicity.singleton,
    parser.VarParsing.varType.string, "input file")
opts.register('globalTag', 'PHYS14_25_V1::All', parser.VarParsing.multiplicity.singleton,
#opts.register('globalTag', 'PLS170_V7AN1::All', parser.VarParsing.multiplicity.singleton,
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


################################################################
# Object Selection
################################################################
process.selectedVertices = cms.EDFilter("VertexRefSelector",
  src = cms.InputTag("offlinePrimaryVertices"),
  cut = cms.string("ndof >= 4 & abs(z) <= 24 & abs(position.Rho) <= 2")
)

#process.selectedPFCandidates = cms.EDFilter("PFCandidateRefSelector",
#  src = cms.InputTag("particleFlow"),
#  cut = cms.string("pt > 5.0")
#)

#process.selectedElectrons = cms.EDFilter("GsfElectronRefSelector",
#  src = cms.InputTag("gedGsfElectrons"),
#  cut = cms.string("pt > 9.5 & abs(eta) < 2.6")
#)

#process.selectedMuons = cms.EDFilter("MuonRefSelector",
#  src = cms.InputTag("muons"),
#  cut = cms.string("pt > 3.0 & abs(eta) < 2.6")
#)

#process.selectedPFMuons = cms.EDFilter("GenericPFCandidateSelector",
#  src = cms.InputTag("particleFlow"),
#  cut = cms.string("pt > 3.0 & abs(eta) < 2.6 & abs(pdgId) == 13")
#)

#process.selectedPFTaus = cms.EDFilter("PFTauRefSelector",
#  src = cms.InputTag("hpsPFTauProducer"),
#  cut = cms.string("pt > 10.0 & abs(eta) < 3.0")
#)

if release in ['72XMINIAOD']:
  process.selectedVertices.src = cms.InputTag("offlineSlimmedPrimaryVertices")
  process.selectedElectrons = cms.EDFilter("PATElectronSelector",
      src = cms.InputTag("slimmedElectrons"),
      cut = cms.string("pt > 9.5 & abs(eta) < 2.6")
      )
  process.selectedMuons = cms.EDFilter("PATMuonSelector",
      src = cms.InputTag("slimmedMuons"),
      cut = cms.string("pt > 3 & abs(eta) < 2.6")
      )
  process.selectedTaus = cms.EDFilter("PATTauRefSelector",
      src = cms.InputTag("slimmedTaus"),
      cut = cms.string('pt > 18.0 & abs(eta) < 2.6 & tauID("decayModeFinding") > 0.5')
      )

process.icSelectionSequence = cms.Sequence(
  process.selectedVertices
#  process.selectedPFCandidates
#  process.selectedElectrons+
#  process.selectedMuons+
#  process.selectedMuons+
#  process.selectedTaus
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
#if release in ['72XMINIAOD']: electronLabel = cms.InputTag("slimmedElectrons")
if release in ['72XMINIAOD']: electronLabel = cms.InputTag("selectedElectrons")

process.icElectronSequence = cms.Sequence()

#ICElectronConversionCalculator needs to come back in, but modifications needed
#process.icElectronConversionCalculator = cms.EDProducer('ICElectronConversionCalculator',
#    input       = electronLabel,
#    beamspot    = cms.InputTag("offlineBeamSpot"),
#    conversions = cms.InputTag("allConversions")
#)
#if release in ['70XMINIAOD', '72XMINIAOD']:
#  process.icElectronConversionCalculator = cms.EDProducer('ICElectronConversionFromPatCalculator',
#      input       = electronLabel
#  )

#process.load("EgammaAnalysis.ElectronTools.electronIdMVAProducer_CSA14_cfi")
#New electron ID MVA producer to run without tracks
#process.mvaNonTrigphys14 = cms.EDProducer("ICElectronIDMVAProducerMiniAOD",
#    verbose=cms.untracked.bool(False),
#    vertexTag=cms.InputTag('offlineSlimmedPrimaryVertices'),
#    electronTag=cms.InputTag('selectedElectrons'),
#    reducedEBRecHitCollection=cms.InputTag('reducedEgamma','reducedEBRecHits',"PAT"),
#    reducedEERecHitCollection=cms.InputTag('reducedEgamma','reducedEERecHits',"PAT"),
#    method=cms.string("BDTSimpleCat"),
#    mvaWeightFile=cms.vstring(
#                              "EgammaAnalysis/ElectronTools/data/PHYS14/EIDmva_EB1_5_oldscenario2phys14_BDT.weights.xml",
#                              "EgammaAnalysis/ElectronTools/data/PHYS14/EIDmva_EB2_5_oldscenario2phys14_BDT.weights.xml",
#                              "EgammaAnalysis/ElectronTools/data/PHYS14/EIDmva_EE_5_oldscenario2phys14_BDT.weights.xml",
#                              "EgammaAnalysis/ElectronTools/data/PHYS14/EIDmva_EB1_10_oldscenario2phys14_BDT.weights.xml",
#                              "EgammaAnalysis/ElectronTools/data/PHYS14/EIDmva_EB2_10_oldscenario2phys14_BDT.weights.xml",
#                              "EgammaAnalysis/ElectronTools/data/PHYS14/EIDmva_EE_10_oldscenario2phys14_BDT.weights.xml",
#                               ),
#   trig=cms.bool(False),
#   )

#Include electron cut-based IDs
#from PhysicsTools.SelectorUtils.tools.vid_id_tools import *
#process.load("RecoEgamma.ElectronIdentification.egmGsfElectronIDs_cfi")
#process.egmGsfElectronIDs.physicsObjectSrc = cms.InputTag('selectedElectrons')

#from PhysicsTools.SelectorUtils.centralIDRegistry import central_id_registry
#process.egmGsfElectronIDSequence = cms.Sequence(process.egmGsfElectronIDs)

#my_id_modules = ['RecoEgamma.ElectronIdentification.Identification.cutBasedElectronID_PHYS14_PU20bx25_V1_miniAOD_cff']
#for idmod in my_id_modules:
#  setupAllVIDIdsInModule(process,idmod,setupVIDElectronSelection)

#process.icElectronSequence+=cms.Sequence(
#   process.egmGsfElectronIDSequence+
#   process.mvaNonTrigphys14
#   )

#Electron PF iso sequence
#if release in ['72XMINIAOD']:
#  process.load("CommonTools.ParticleFlow.Isolation.pfElectronIsolation_cff")
#  process.elPFIsoValueCharged03PFIdPFIso    = process.elPFIsoValueCharged03PFId.clone()
#  process.elPFIsoValueChargedAll03PFIdPFIso = process.elPFIsoValueChargedAll03PFId.clone()
#  process.elPFIsoValueGamma03PFIdPFIso      = process.elPFIsoValueGamma03PFId.clone()
#  process.elPFIsoValueNeutral03PFIdPFIso    = process.elPFIsoValueNeutral03PFId.clone()
#  process.elPFIsoValuePU03PFIdPFIso         = process.elPFIsoValuePU03PFId.clone()
#  process.electronPFIsolationValuesSequence = cms.Sequence(
#      process.elPFIsoValueCharged03PFIdPFIso+
#      process.elPFIsoValueChargedAll03PFIdPFIso+
#      process.elPFIsoValueGamma03PFIdPFIso+
#      process.elPFIsoValueNeutral03PFIdPFIso+
#      process.elPFIsoValuePU03PFIdPFIso
#      )
#  process.elPFIsoDepositCharged.src     = electronLabel
#  process.elPFIsoDepositChargedAll.src  = electronLabel
#  process.elPFIsoDepositNeutral.src     = electronLabel
#  process.elPFIsoDepositGamma.src       = electronLabel
#  process.elPFIsoDepositPU.src          = electronLabel
#  if release in ['70XMINIAOD', '72XMINIAOD']:
#    process.elPFIsoDepositGamma.ExtractorPSet.ComponentName = cms.string("CandViewExtractor")
#  process.icElectronSequence += cms.Sequence(
#      process.electronPFIsolationDepositsSequence+
#      process.electronPFIsolationValuesSequence
#      )
#Leave these vetos out for now
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

#process.icElectronProducer = producers.icElectronProducer.clone(
#  branch                    = cms.string("electrons"),
#  input                     = cms.InputTag("selectedElectrons"),
#  includeConversionMatches  = cms.bool(False),
#  inputConversionMatches    = cms.InputTag("icElectronConversionCalculator"),
#  includeVertexIP           = cms.bool(True),
#  inputVertices             = cms.InputTag("selectedVertices"),
#  includeBeamspotIP         = cms.bool(True),
#  inputBeamspot             = cms.InputTag("offlineBeamSpot"),
#  includeFloats = cms.PSet(
#     cutBasedVeto        = cms.InputTag("egmGsfElectronIDs:cutBasedElectronID-PHYS14-PU20bx25-V1-miniAOD-standalone-veto"),
#     mvaNonTrigphys14    = cms.InputTag("mvaNonTrigphys14")
#    # trackInIsoSum   = cms.InputTag("icHttElecIsoCheck"),
#    # matchedRecoMuon = cms.InputTag("icHttMuonOverlapCheck")
#  ),
#  includePFIso03           = cms.bool(True)
#)

#process.icElectronSequence += cms.Sequence(
#  process.icElectronConversionCalculator+
#  process.icElectronProducer
#)

################################################################
# Muons
################################################################
process.icMuonSequence = cms.Sequence()

#process.muonIdMedium = cms.EDFilter("ICMuonIDProducer",
#    verbose=cms.untracked.bool(False),
#    muonTag=cms.InputTag('selectedMuons')
#   )

#process.icMuonSequence+=cms.Sequence(
#   process.muonIdMedium
#   )


process.load("CommonTools.ParticleFlow.Isolation.pfMuonIsolation_cff")
process.muPFIsoValueCharged03PFIso    = process.muPFIsoValueCharged03.clone()
process.muPFIsoValueChargedAll03PFIso = process.muPFIsoValueChargedAll03.clone()
process.muPFIsoValueGamma03PFIso      = process.muPFIsoValueGamma03.clone()
process.muPFIsoValueNeutral03PFIso    = process.muPFIsoValueNeutral03.clone()
process.muPFIsoValuePU03PFIso         = process.muPFIsoValuePU03.clone()
process.muonPFIsolationValuesSequence = cms.Sequence(
    process.muPFIsoValueCharged03PFIso+
    process.muPFIsoValueChargedAll03PFIso+
    process.muPFIsoValueGamma03PFIso+
    process.muPFIsoValueNeutral03PFIso+
    process.muPFIsoValuePU03PFIso
    )
if release in ['72X']: muons = cms.InputTag("selectedMuons")
if release in ['72XMINIAOD']: muons = cms.InputTag("selectedMuons")
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
  branch                    = cms.string("muons"),
  input                     = cms.InputTag("selectedMuons"),
  isPF                      = cms.bool(False),
  includeVertexIP           = cms.bool(True),
  inputVertices             = cms.InputTag("selectedVertices"),
  includeBeamspotIP         = cms.bool(True),
  inputBeamspot             = cms.InputTag("offlineBeamSpot"),
  includeFloats = cms.PSet(
     muonIdMedium    = cms.InputTag("muonIdMedium")
  ),
  requestTracks           = cms.bool(True),
  includePFIso03           = cms.bool(True)
)
if release in ['72XMINIAOD']: process.icMuonProducer.isPF = cms.bool(False)

#process.icMuonSequence += cms.Sequence(
#  process.icMuonProducer
#)

################################################################
# Taus
################################################################
import UserCode.ICHiggsTauTau.tau_discriminators_cfi as tauIDs

process.icTauProducer = producers.icTauProducer.clone(
  input                   = cms.InputTag("selectedTaus"),
  inputVertices           = cms.InputTag("selectedVertices"),
  includeVertexIP         = cms.bool(True),
  requestTracks           = cms.bool(True),
  tauIDs = tauIDs.fullNewIds
)

if release in ['72XMINIAOD']:
  process.icTauProducer = cms.EDProducer("ICPFTauFromPatProducer",
    branch                  = cms.string("taus"),
    input                   = cms.InputTag("selectedTaus"),
    inputVertices           = cms.InputTag("selectedVertices"),
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
if release in ['72XMINIAOD']:
  #rebuild ak4 jets as in  https://twiki.cern.ch/twiki/bin/view/CMSPublic/WorkBookMiniAOD#Jets
  process.load('PhysicsTools.PatAlgos.slimming.unpackedTracksAndVertices_cfi')
  process.pfchs=cms.EDFilter("CandPtrSelector",src=cms.InputTag("packedPFCandidates"),cut=cms.string("fromPV"))
  process.ak4PFJetsCHS = ak4PFJets.clone(src='pfchs',doAreaFastjet=True)
#  process.ak4JetTracksAssociatorAtVertexPF.jets = cms.InputTag("ak4PFJetsCHS")
#  process.ak4JetTracksAssociatorAtVertexPF.tracks = cms.InputTag("unpackedTracksAndVertices")


#  from PhysicsTools.PatAlgos.tools.jetTools import addJetCollection
#  addJetCollection(
#    process,
#    postfix = "",
#    labelName='ak4PFJetsCHS',
#    jetSource = cms.InputTag('ak4PFCHS'),
#    trackSource = cms.InputTag('unpackedTracksAndVertices'),
#    pvSource = cms.InputTag('unpackedTracksAndVertices'),
#    jetCorrections=('AK4PFchs',cms.vstring(['L1FastJet','L2Relative','L3Absolute']),'Type-2'),
#    btagDiscriminators=['combinedSecondaryVertexBJetTags'],
#    algo='AK', rParam=0.4
#  )

 
  process.selectedSlimmedJetsAK4 = cms.EDFilter("PATJetRefSelector",
      src = cms.InputTag("slimmedJets"),
      cut = cms.string("pt > 15")
      )

if release in ['72X']:
#  process.pfchs = cms.EDFilter("CandPtrSelector",src=cms.InputTag("PFCandidates"),cut=cms.string("fromPV"))
  process.ak4PFJetsCHS = ak4PFJets.clone(src = cms.InputTag("pfNoPileUp"))



if release in ['72XMINIAOD']:

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
if release in ['72XMINIAOD']:
   process.jetPartons.src = cms.InputTag("prunedGenParticles")

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



 # Jet energy corrections
 # ----------------------
process.ak4PFL1Fastjet = cms.ESProducer("L1FastjetCorrectionESProducer",
    srcRho = cms.InputTag("kt6PFJets", "rho"),
    algorithm = cms.string('AK4PFchs'),
    level = cms.string('L1FastJet')
)
process.ak4PFL2Relative = cms.ESProducer("LXXXCorrectionESProducer",
    algorithm = cms.string('AK4PFchs'),
    level = cms.string('L2Relative')
)
process.ak4PFL3Absolute = cms.ESProducer("LXXXCorrectionESProducer",
    algorithm = cms.string('AK4PFchs'),
    level = cms.string('L3Absolute')
)
process.ak4PFResidual = cms.ESProducer("LXXXCorrectionESProducer",
    algorithm = cms.string('AK4PFchs'),
    level = cms.string('L2L3Residual')
)

#Corrections applied to miniaod slimmedJets
pfJECS = cms.PSet(
  L1FastJet  = cms.string("ak4PFL1Fastjet"),
  L2Relative = cms.string("ak4PFL2Relative"),
  L3Absolute = cms.string("ak4PFL3Absolute")
)
if isData: pfJECS.append(
  L2L3Residual = cms.string("ak4PFResidual")
)

 # b-tagging
 # ---------
process.load("RecoJets.JetAssociationProducers.ak4JTA_cff")
from RecoJets.JetAssociationProducers.ak4JTA_cff import ak4JetTracksAssociatorAtVertex
process.load("RecoBTag.Configuration.RecoBTag_cff")
import RecoBTag.Configuration.RecoBTag_cff as btag
process.jetTracksAssociatorAtVertexAK4PF = ak4JetTracksAssociatorAtVertex.clone(
  jets = cms.InputTag("ak4PFJetsCHS")
)

if release in ['70XMINIAOD', '72XMINIAOD']:
  process.jetTracksAssociatorAtVertexAK4PF.tracks = cms.InputTag("unpackedTracksAndVertices")
  process.jetTracksAssociatorAtVertexAK4PF.pvSrc = cms.InputTag("unpackedTracksAndVertices")

#if isEmbedded:
#  process.jetTracksAssociatorAtVertexAK5PF.tracks = cms.InputTag("tmfTracks")

process.impactParameterTagInfosAK4PF = btag.impactParameterTagInfos.clone(
  jetTracks = cms.InputTag('jetTracksAssociatorAtVertexAK4PF')
)
if release in ['70XMINIAOD', '72XMINIAOD']:
  process.impactParameterTagInfosAK4PF.primaryVertex = cms.InputTag("unpackedTracksAndVertices")

process.secondaryVertexTagInfosAK4PF = btag.secondaryVertexTagInfos.clone(
  trackIPTagInfos = cms.InputTag('impactParameterTagInfosAK4PF')
)
process.simpleSecondaryVertexHighEffBJetTagsAK4PF = btag.simpleSecondaryVertexHighEffBJetTags.clone (
  tagInfos = cms.VInputTag('secondaryVertexTagInfosAK4PF')
)
process.simpleSecondaryVertexHighPurBJetTagsAK4PF = btag.simpleSecondaryVertexHighPurBJetTags.clone (
  tagInfos = cms.VInputTag('secondaryVertexTagInfosAK4PF')
)
process.combinedSecondaryVertexBJetTagsAK4PF = btag.combinedSecondaryVertexBJetTags.clone (
  tagInfos = cms.VInputTag('impactParameterTagInfosAK4PF', 'secondaryVertexTagInfosAK4PF')
)

process.btaggingSequenceAK4PF = cms.Sequence(
  process.jetTracksAssociatorAtVertexAK4PF
  +process.impactParameterTagInfosAK4PF
  +process.secondaryVertexTagInfosAK4PF
  +process.simpleSecondaryVertexHighEffBJetTagsAK4PF
  +process.simpleSecondaryVertexHighPurBJetTagsAK4PF
  +process.combinedSecondaryVertexBJetTagsAK4PF
 )

 # Pileup ID
 # ---------
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
    rho     = cms.InputTag("kt6PFJets", "rho"),
#    rho     = cms.InputTag("fixedGridRhoFastjetAll"),
    jec     = cms.string("AK4PFchs"),
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
    input                     = cms.InputTag("ak4PFJetsCHS"),
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
        simpleSecondaryVertexHighEffBJetTags = cms.InputTag("simpleSecondaryVertexHighEffBJetTagsAK4PF"),
        simpleSecondaryVertexHighPurBJetTags = cms.InputTag("simpleSecondaryVertexHighPurBJetTagsAK4PF"),
        combinedSecondaryVertexBJetTags      = cms.InputTag("combinedSecondaryVertexBJetTagsAK4PF")
      )
    ),
    destConfig = cms.PSet(
      includePileupID       = cms.bool(True), #rerunning the pu MVA on the jet collection created in miniAOD is possible in newer CMSSW versions but not yet in 72
      inputPileupID         = cms.InputTag("puJetMva", "fullDiscriminant"),
      includeTrackBasedVars = cms.bool(False),
      inputTracks           = cms.InputTag("generalTracks"),
      inputVertices         = cms.InputTag("selectedVertices"),
      requestTracks         = cms.bool(False)
    )
)


if release in ['72XMINIAOD']:
  process.icPFJetProducer.destConfig.includePileupID=cms.bool(False)
  process.icPFJetProducer.destConfig.inputVertices=cms.InputTag("unpackedTracksAndVertices")

# if isPhys14:
#process.selectedPFJets = cms.EDFilter("PFJetRefSelector",
#    src = cms.InputTag("ak4PFJetsCHS"),
#    cut = cms.string("pt > 15")
#    )



  process.icPFJetProducerFromPat = producers.icPFJetFromPatProducer.clone(
      branch                    = cms.string("ak4SlimmedJets"),
      input                     = cms.InputTag("selectedSlimmedJetsAK4"),
      srcConfig = cms.PSet(
        isSlimmed               = cms.bool(True),
        includeJetFlavour       = cms.bool(True),
        includeJECs             = cms.bool(False),
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


# # 72X: error loading mva file
# if release in ['42X', '53X', '70X']:
#   process.icPFJetSequence += cms.Sequence(
#     process.puJetMva
#   )
if release in ['72XMINIAOD']:
  process.icPFJetSequence += cms.Sequence(
     process.pfchs+
     process.selectedSlimmedJetsAK4+
     process.unpackedTracksAndVertices+
     process.icPFJetProducerFromPat
     )
if release in ['72X', '72XMINIAOD']:
 # process.icPFJetProducer.srcConfig.BTagDiscriminators = cms.PSet()
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

if release in ['72XMINIAOD']:
  process.icPFJetSequence.remove(process.puJetMva) #This works for jets built from PackedCandidates in CMSSW74X but not yet in 72

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

process.icEventInfoSequence = cms.Sequence(
  process.icEventInfoProducer
)


################################################################
# Event
################################################################
process.icEventProducer = producers.icEventProducer.clone()


process.p = cms.Path(
  process.ic72XSequence+
  process.icMiniAODSequence+
  process.icSelectionSequence+
  process.pfParticleSelectionSequence+
  process.icVertexSequence+
  process.icPFJetSequence+
  #process.icGenSequence+
  #process.icEventInfoSequence+
  process.icEventProducer
)

# process.schedule = cms.Schedule(process.patTriggerPath, process.p)
process.schedule = cms.Schedule(process.p)

#print process.dumpPython()
