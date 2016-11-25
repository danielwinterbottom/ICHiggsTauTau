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
opts.register('release', '', parser.VarParsing.multiplicity.singleton,
    parser.VarParsing.varType.string, "Release label")

opts.parseArguments()
infile      = opts.file
if not infile: infile = "file:/tmp/file.root"
tag         = opts.globalTag
isData      = opts.isData
release     = opts.release

if not release in ['74X']:
  print 'Release not recognised, exiting!'
  sys.exit(1)
print 'release     : '+release
print 'isData      : '+str(isData)
print 'globalTag   : '+tag

################################################################
# Standard setup
################################################################
process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.load('Configuration.StandardSequences.GeometryRecoDB_cff')
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_condDBv2_cff")
# process.load("Configuration.StandardSequences.MagneticField_cff")

process.TFileService = cms.Service("TFileService",
  fileName = cms.string("EventTree.root"),
  closeFileFast = cms.untracked.bool(True)
)

################################################################
# Message Logging, summary, and number of events
################################################################
process.maxEvents = cms.untracked.PSet(
  input = cms.untracked.int32(500)
)

process.MessageLogger.cerr.FwkReport.reportEvery = 1

process.options   = cms.untracked.PSet(
  wantSummary = cms.untracked.bool(True)
)

################################################################
# Input files and global tags
################################################################
process.source = cms.Source("PoolSource", fileNames = cms.untracked.vstring(infile))
from Configuration.AlCa.GlobalTag_condDBv2 import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, tag, '')

# 72XMINIAOD MC: file=root://xrootd.unl.edu//store/mc/Phys14DR/VBF_HToTauTau_M-125_13TeV-powheg-pythia6/MINIAODSIM/PU40bx25_PHYS14_25_V1-v1/00000/36224FE2-0571-E411-9664-00266CFAE30C.root globalTag=START72_V1::All
# 74X: globalTag=MCRUN2_74_V9A file=/store/mc/RunIISpring15DR74/DYJetsToLL_M-50_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8/GEN-SIM-RECO/AsymptNoPURawReco_MCRUN2_74_V9A-v4/10000/000BE505-D818-E511-8A12-3417EBE5280A.root
# from RAW: /store/mc/RunIISpring15DR74/DYJetsToLL_M-50_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8/GEN-SIM-RAW/AsymptNoPURawReco_MCRUN2_74_V9A-v4/10000/0012730E-CC18-E511-8E17-00266CFFBE5C.root
import UserCode.ICHiggsTauTau.default_producers_cfi as producers
import UserCode.ICHiggsTauTau.default_selectors_cfi as selectors

################################################################
# Re-do PFTau reconstruction
################################################################
process.load('Configuration.StandardSequences.Reconstruction_cff')
process.load("RecoTauTag/Configuration/RecoPFTauTag_cff")

process.extraPreSequence = cms.Sequence()

if release in ['74X']:
  process.load('Configuration.StandardSequences.Services_cff')
  # process.load('SimGeneral.HepPDTESSource.pythiapdt_cfi')
  process.load('Configuration.EventContent.EventContent_cff')
  process.load('Configuration.StandardSequences.MagneticField_38T_PostLS1_cff')
  process.load('SimGeneral.MixingModule.mixNoPU_cfi')
  process.load('Configuration.StandardSequences.RawToDigi_cff')
  process.load('Configuration.StandardSequences.L1Reco_cff')
  process.load('Configuration.StandardSequences.Reconstruction_cff')
  process.load('CommonTools.ParticleFlow.EITopPAG_cff')
  process.extraPreSequence += process.RawToDigi
  process.extraPreSequence += process.L1Reco
  process.extraPreSequence += process.reconstruction
  process.extraPreSequence += process.EIsequence
  from SLHCUpgradeSimulations.Configuration.postLS1Customs import customisePostLS1_50ns 
  process = customisePostLS1_50ns(process)

  # process.extraPreSequence += process.PFTau
  # process.extraPreSequence += process.globalreco
  # process.extraPreSequence += process.highlevelreco


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
  src = cms.InputTag("gedGsfElectrons"),
  cut = cms.string("pt > 9.5 & abs(eta) < 2.6")
)

process.selectedPFMuons = cms.EDFilter("GenericPFCandidateSelector",
  src = cms.InputTag("particleFlow"),
  cut = cms.string("pt > 3.0 & abs(eta) < 2.6 & abs(pdgId) == 13")
)

process.selectedPFTaus = cms.EDFilter("PFTauRefSelector",
  src = cms.InputTag("hpsPFTauProducer"),
  cut = cms.string("pt > 10.0 & abs(eta) < 3.0")
)


process.icSelectionSequence = cms.Sequence(
  process.selectedVertices+
  process.selectedPFCandidates+
  process.selectedElectrons+
  process.selectedPFMuons+
  process.selectedPFTaus
)

# ################################################################
# # PF sequence for lepton isolation
# ################################################################
process.elPFIsoValueCharged03PFIdPFIso = cms.EDProducer('ICRecoElectronIsolation',
  input        = cms.InputTag("gedGsfElectrons"),
  deltaR       = cms.double(0.3),
  iso_type = cms.string("charged_iso") 
)    
process.elPFIsoValueGamma03PFIdPFIso = cms.EDProducer('ICRecoElectronIsolation',
  input        = cms.InputTag("gedGsfElectrons"),
  deltaR       = cms.double(0.3),
  iso_type = cms.string("photon_iso") 
)    
process.elPFIsoValueNeutral03PFIdPFIso = cms.EDProducer('ICRecoElectronIsolation',
  input        = cms.InputTag("gedGsfElectrons"),
  deltaR       = cms.double(0.3),
  iso_type = cms.string("neutral_iso") 
)    
process.elPFIsoValuePU03PFIdPFIso = cms.EDProducer('ICRecoElectronIsolation',
  input        = cms.InputTag("gedGsfElectrons"),
  deltaR       = cms.double(0.3),
  iso_type = cms.string("pu_iso") 
)    

process.electronPFIsolationValuesSequence = cms.Sequence(
  process.elPFIsoValueCharged03PFIdPFIso+
  process.elPFIsoValueGamma03PFIdPFIso+
  process.elPFIsoValueNeutral03PFIdPFIso+
  process.elPFIsoValuePU03PFIdPFIso
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

process.icPFSequence = cms.Sequence(process.icPFProducer)


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

process.icTrackSequence = cms.Sequence(
  process.selectedTracks+
  process.selectedTauTracks+
  process.requestTracksByDeltaRToTaus+
  process.icMergedTracks+
  process.icTrackProducer+
  process.icGsfTrackProducer
)

# ################################################################
# # Electrons
# ################################################################
# electronLabel = cms.InputTag("gsfElectrons")
# if release in ['70X', '72X']: electronLabel = cms.InputTag("gedGsfElectrons")
# if release in ['70XMINIAOD', '72XMINIAOD']: electronLabel = cms.InputTag("slimmedElectrons")

# process.icElectronSequence = cms.Sequence()

# process.icElectronConversionCalculator = cms.EDProducer('ICElectronConversionCalculator',
#     input       = electronLabel,
#     beamspot    = cms.InputTag("offlineBeamSpot"),
#     conversions = cms.InputTag("allConversions")
# )
# if release in ['70XMINIAOD', '72XMINIAOD']:
#   process.icElectronConversionCalculator = cms.EDProducer('ICElectronConversionFromPatCalculator',
#       input       = electronLabel
#   )



# process.icHttElecIsoCheck = cms.EDProducer('ICHttElecIsoCheck',
#     input         = electronLabel,
#     pfChargedAll  = cms.InputTag("pfAllChargedParticles")
# )

# process.icHttMuonOverlapCheck = cms.EDProducer('ICHttMuonOverlapCheck',
#     input = electronLabel,
#     muons = cms.InputTag("muons")
# )

# process.load("EgammaAnalysis.ElectronTools.electronIdMVAProducer_cfi")
# #if release in ['70XMINIAOD', ['72XMINIAOD']:
# #  process.mvaTrigV0.vertexTag = cms.InputTag("offlineSlimmedPrimaryVertices")
# #  process.mvaTrigV0.reducedEBRecHitCollection = cms.InputTag("reducedEgamma", "reducedEBRecHits", "PAT")
# #  process.mvaTrigV0.reducedEERecHitCollection = cms.InputTag("reducedEgamma", "reducedEERecHits", "PAT")
# #  process.mvaNonTrigV0.vertexTag = cms.InputTag("offlineSlimmedPrimaryVertices")
# #  process.mvaNonTrigV0.reducedEBRecHitCollection = cms.InputTag("reducedEgamma", "reducedEBRecHits", "PAT")
# #  process.mvaNonTrigV0.reducedEERecHitCollection = cms.InputTag("reducedEgamma", "reducedEERecHits", "PAT")

# # 72X: error in getting calo conditions!
# if release in ['42X', '53X', '70X']:
#   process.icElectronSequence += cms.Sequence(
#     process.mvaTrigV0+
#     process.mvaNonTrigV0
#   )
#   process.mvaTrigV0.electronTag = electronLabel
#   process.mvaNonTrigV0.electronTag = electronLabel

# if release in ['42X', '53X']:
#   from CommonTools.ParticleFlow.Tools.pfIsolation import setupPFElectronIso
#   process.eleIsoSequence = setupPFElectronIso(process, 'gsfElectrons')
#   process.eleIsoSequence.remove(process.elPFIsoValueCharged03NoPFIdPFIso)
#   process.eleIsoSequence.remove(process.elPFIsoValueChargedAll03NoPFIdPFIso)
#   process.eleIsoSequence.remove(process.elPFIsoValueGamma03NoPFIdPFIso)
#   process.eleIsoSequence.remove(process.elPFIsoValueNeutral03NoPFIdPFIso)
#   process.eleIsoSequence.remove(process.elPFIsoValuePU03NoPFIdPFIso)
#   process.eleIsoSequence.remove(process.elPFIsoValueCharged04NoPFIdPFIso)
#   process.eleIsoSequence.remove(process.elPFIsoValueChargedAll04NoPFIdPFIso)
#   process.eleIsoSequence.remove(process.elPFIsoValueGamma04NoPFIdPFIso)
#   process.eleIsoSequence.remove(process.elPFIsoValueNeutral04NoPFIdPFIso)
#   process.eleIsoSequence.remove(process.elPFIsoValuePU04NoPFIdPFIso)
#   process.icElectronSequence += cms.Sequence(
#       process.eleIsoSequence
#       )

# if release in ['70X', '70XMINIAOD', '72X', '72XMINIAOD']:
#   process.load("CommonTools.ParticleFlow.Isolation.pfElectronIsolation_cff")
#   process.elPFIsoValueCharged04PFIdPFIso    = process.elPFIsoValueCharged04PFId.clone()
#   process.elPFIsoValueChargedAll04PFIdPFIso = process.elPFIsoValueChargedAll04PFId.clone()
#   process.elPFIsoValueGamma04PFIdPFIso      = process.elPFIsoValueGamma04PFId.clone()
#   process.elPFIsoValueNeutral04PFIdPFIso    = process.elPFIsoValueNeutral04PFId.clone()
#   process.elPFIsoValuePU04PFIdPFIso         = process.elPFIsoValuePU04PFId.clone()
#   process.electronPFIsolationValuesSequence = cms.Sequence(
#       process.elPFIsoValueCharged04PFIdPFIso+
#       process.elPFIsoValueChargedAll04PFIdPFIso+
#       process.elPFIsoValueGamma04PFIdPFIso+
#       process.elPFIsoValueNeutral04PFIdPFIso+
#       process.elPFIsoValuePU04PFIdPFIso
#       )
#   process.elPFIsoDepositCharged.src     = electronLabel
#   process.elPFIsoDepositChargedAll.src  = electronLabel
#   process.elPFIsoDepositNeutral.src     = electronLabel
#   process.elPFIsoDepositGamma.src       = electronLabel
#   process.elPFIsoDepositPU.src          = electronLabel
#   if release in ['70XMINIAOD', '72XMINIAOD']:
#     process.elPFIsoDepositGamma.ExtractorPSet.ComponentName = cms.string("CandViewExtractor")
#   process.icElectronSequence += cms.Sequence(
#       process.electronPFIsolationDepositsSequence+
#       process.electronPFIsolationValuesSequence
#       )

# process.elPFIsoValueGamma04PFIdPFIso.deposits[0].vetos = (
#     cms.vstring('EcalEndcaps:ConeVeto(0.08)','EcalBarrel:ConeVeto(0.08)'))
# process.elPFIsoValueNeutral04PFIdPFIso.deposits[0].vetos = (
#     cms.vstring())
# process.elPFIsoValuePU04PFIdPFIso.deposits[0].vetos = (
#     cms.vstring())
# process.elPFIsoValueCharged04PFIdPFIso.deposits[0].vetos = (
#     cms.vstring('EcalEndcaps:ConeVeto(0.015)'))
# process.elPFIsoValueChargedAll04PFIdPFIso.deposits[0].vetos = (
#     cms.vstring('EcalEndcaps:ConeVeto(0.015)','EcalBarrel:ConeVeto(0.01)'))

# process.icElectronProducer = producers.icElectronProducer.clone(
#   branch                    = cms.string("electrons"),
#   input                     = cms.InputTag("selectedElectrons"),
#   includeConversionMatches  = cms.bool(True),
#   inputConversionMatches    = cms.InputTag("icElectronConversionCalculator"),
#   includeVertexIP           = cms.bool(True),
#   inputVertices             = cms.InputTag("selectedVertices"),
#   includeBeamspotIP         = cms.bool(True),
#   inputBeamspot             = cms.InputTag("offlineBeamSpot"),
#   includeFloats = cms.PSet(
#     mvaTrigV0       = cms.InputTag("mvaTrigV0"),
#     mvaNonTrigV0    = cms.InputTag("mvaNonTrigV0"),
#     trackInIsoSum   = cms.InputTag("icHttElecIsoCheck"),
#     matchedRecoMuon = cms.InputTag("icHttMuonOverlapCheck")
#   ),
#   includePFIso04           = cms.bool(True)
# )

# if release in ['70XMINIAOD', '72XMINIAOD']:
#   process.icElectronProducer.includeFloats = cms.PSet(
#     #trackInIsoSum   = cms.InputTag("icHttElecIsoCheck"),
#     #matchedRecoMuon = cms.InputTag("icHttMuonOverlapCheck")
#     )
# if release in ['72X']:
#   process.icElectronProducer.includeFloats = cms.PSet(
#     trackInIsoSum   = cms.InputTag("icHttElecIsoCheck"),
#     matchedRecoMuon = cms.InputTag("icHttMuonOverlapCheck")
#   )


# if release in ['42X', '53X', '70X', '72X']:
#   process.icElectronSequence += cms.Sequence(
#     process.icHttElecIsoCheck+
#     process.icHttMuonOverlapCheck
#   )

# process.icElectronSequence += cms.Sequence(
#   process.icElectronConversionCalculator+
#   process.icElectronProducer
# )

# ################################################################
# # Muons
# ################################################################
# process.icMuonSequence = cms.Sequence()
# if release in ['42X', '53X']:
#   from CommonTools.ParticleFlow.Tools.pfIsolation import setupPFMuonIso
#   process.muIsoSequence = setupPFMuonIso(process, 'selectedPFMuons')
#   process.icMuonSequence += process.muIsoSequence

# if release in ['70X', '70XMINIAOD', '72X', '72XMINIAOD']:
#   process.load("CommonTools.ParticleFlow.Isolation.pfMuonIsolation_cff")
#   process.muPFIsoValueCharged04PFIso    = process.muPFIsoValueCharged04.clone()
#   process.muPFIsoValueChargedAll04PFIso = process.muPFIsoValueChargedAll04.clone()
#   process.muPFIsoValueGamma04PFIso      = process.muPFIsoValueGamma04.clone()
#   process.muPFIsoValueNeutral04PFIso    = process.muPFIsoValueNeutral04.clone()
#   process.muPFIsoValuePU04PFIso         = process.muPFIsoValuePU04.clone()
#   process.muonPFIsolationValuesSequence = cms.Sequence(
#       process.muPFIsoValueCharged04PFIso+
#       process.muPFIsoValueChargedAll04PFIso+
#       process.muPFIsoValueGamma04PFIso+
#       process.muPFIsoValueNeutral04PFIso+
#       process.muPFIsoValuePU04PFIso
#       )
#   if release in ['70X', '72X']: muons = cms.InputTag("selectedPFMuons")
#   if release in ['70XMINIAOD', '72XMINIAOD']: muons = cms.InputTag("slimmedMuons")
#   process.muPFIsoDepositCharged.src     = muons
#   process.muPFIsoDepositChargedAll.src  = muons
#   process.muPFIsoDepositNeutral.src     = muons
#   process.muPFIsoDepositGamma.src       = muons
#   process.muPFIsoDepositPU.src          = muons
#   process.icMuonSequence += cms.Sequence(
#       process.muonPFIsolationDepositsSequence+
#       process.muonPFIsolationValuesSequence
#       )


# process.icMuonProducer = producers.icMuonProducer.clone(
#   branch                    = cms.string("muonsPFlow"),
#   input                     = cms.InputTag("selectedPFMuons"),
#   isPF                      = cms.bool(True),
#   includeVertexIP           = cms.bool(True),
#   inputVertices             = cms.InputTag("selectedVertices"),
#   includeBeamspotIP         = cms.bool(True),
#   inputBeamspot             = cms.InputTag("offlineBeamSpot"),
#   includeFloats = cms.PSet(
#   ),
#   includePFIso04           = cms.bool(True)
# )
# if release in ['70XMINIAOD', '72XMINIAOD']: process.icMuonProducer.isPF = cms.bool(False)

# process.icMuonSequence += cms.Sequence(
#   process.icMuonProducer
# )

################################################################
# Taus
################################################################
import UserCode.ICHiggsTauTau.tau_discriminators_cfi as tauIDs

process.icTauProducer = producers.icTauProducer.clone(
  input                   = cms.InputTag("selectedPFTaus"),
  inputVertices           = cms.InputTag("offlinePrimaryVertices"),
  includeVertexIP         = cms.bool(True),
  requestTracks           = cms.bool(True),
  tauIDs = tauIDs.dynamicStripIds
)

process.icTauSequence = cms.Sequence(
  process.icTauProducer
)

# ################################################################
# # Jets
# ################################################################


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
# if isData: pfJECS.append(
#   L2L3Residual = cms.string("ak4PFResidualCHS")
# )


# Producer
# --------
process.icPFJetProducer = producers.icPFJetProducer.clone(
    branch                    = cms.string("ak4PFJetsCHS"),
    input                     = cms.InputTag("ak4PFJetsCHS"),
    srcConfig = cms.PSet(
      includeJetFlavour         = cms.bool(False),
      inputJetFlavour           = cms.InputTag("icPFJetFlavourCalculator"),
      applyJECs                 = cms.bool(True),
      includeJECs               = cms.bool(True),
      JECs                      = pfJECS,
      applyCutAfterJECs         = cms.bool(True),
      cutAfterJECs              = cms.string("pt > 15.0"),
      inputSVInfo               = cms.InputTag(""),
      requestSVInfo             = cms.bool(False),
      BTagDiscriminators        = cms.PSet()
    ),
    destConfig = cms.PSet(
      includePileupID       = cms.bool(False),
      inputPileupID         = cms.InputTag("puJetMva", "fullDiscriminant"),
      includeTrackBasedVars = cms.bool(False),
      inputTracks           = cms.InputTag("generalTracks"),
      inputVertices         = cms.InputTag("offlinePrimaryVertices"),
      requestTracks         = cms.bool(False)
    )
)


process.icPFJetSequence = cms.Sequence()

process.icPFJetSequence += cms.Sequence(
  process.icPFJetProducer
)

################################################################
# Simulation only: GenParticles, GenJets, PileupInfo
################################################################
process.icGenSequence = cms.Sequence()

process.load("SimGeneral.HepPDTESSource.pythiapdt_cfi")
process.prunedGenParticles = cms.EDProducer("GenParticlePruner",
    src = cms.InputTag("genParticles"),
    select = cms.vstring(
      "drop  *", # this is the default
      "++keep abs(pdgId) == 11 || abs(pdgId) == 13 || abs(pdgId) == 15", # keep leptons, with history
      "keep abs(pdgId) == 12 || abs(pdgId) == 14 || abs(pdgId) == 16",   # keep neutrinos
      "drop   status == 2",                                              # drop the shower part of the history
      "+keep pdgId == 22 && status == 1 && (pt > 10 || isPromptFinalState())", # keep gamma above 10 GeV (or all prompt) and its first parent
      "+keep abs(pdgId) == 11 && status == 1 && (pt > 3 || isPromptFinalState())", # keep first parent of electrons above 3 GeV (or prompt)
      "keep++ abs(pdgId) == 15",                                         # but keep keep taus with decays
      "drop  status > 30 && status < 70 ",           #remove pythia8 garbage
      "drop  pdgId == 21 && pt < 5",                                    #remove pythia8 garbage
      "drop   status == 2 && abs(pdgId) == 21",                          # but remove again gluons in the inheritance chain
      "keep abs(pdgId) == 23 || abs(pdgId) == 24 || abs(pdgId) == 25 || abs(pdgId) == 6 || abs(pdgId) == 37 ",   # keep VIP(articles)s
      "keep abs(pdgId) == 310 && abs(eta) < 2.5 && pt > 1 ",                                                     # keep K0
      # keep heavy flavour quarks for parton-based jet flavour
      "keep (4 <= abs(pdgId) <= 5) & (status = 2 || status = 11 || status = 71 || status = 72)",
      # keep light-flavour quarks and gluons for parton-based jet flavour
      "keep (1 <= abs(pdgId) <= 3 || pdgId = 21) & (status = 2 || status = 11 || status = 71 || status = 72) && pt>5", 
      # keep b and c hadrons for hadron-based jet flavour
      "keep (400 < abs(pdgId) < 600) || (4000 < abs(pdgId) < 6000)",
      # additional c hadrons for jet fragmentation studies
      "keep abs(pdgId) = 10411 || abs(pdgId) = 10421 || abs(pdgId) = 10413 || abs(pdgId) = 10423 || abs(pdgId) = 20413 || abs(pdgId) = 20423 || abs(pdgId) = 10431 || abs(pdgId) = 10433 || abs(pdgId) = 20433", 
      # additional b hadrons for jet fragmentation studies
      "keep abs(pdgId) = 10511 || abs(pdgId) = 10521 || abs(pdgId) = 10513 || abs(pdgId) = 10523 || abs(pdgId) = 20513 || abs(pdgId) = 20523 || abs(pdgId) = 10531 || abs(pdgId) = 10533 || abs(pdgId) = 20533 || abs(pdgId) = 10541 || abs(pdgId) = 10543 || abs(pdgId) = 20543", 
      #keep SUSY particles
      "keep (1000001 <= abs(pdgId) <= 1000039 ) || ( 2000001 <= abs(pdgId) <= 2000015)",
      # keep protons
      "keep pdgId = 2212",
      "keep status == 3 || ( 21 <= status <= 29) || ( 11 <= status <= 19)",  #keep event summary (status=3 for pythia6, 21 <= status <= 29 for pythia8)
      "keep isHardProcess() || fromHardProcessFinalState() || fromHardProcessDecayed() || fromHardProcessBeforeFSR() || (statusFlags().fromHardProcess() && statusFlags().isLastCopy())",  #keep event summary based on status flags
    )
)

process.icGenParticleProducer = producers.icGenParticleProducer.clone(
  input   = cms.InputTag("prunedGenParticles"),
  includeMothers = cms.bool(True),
  includeDaughters = cms.bool(True)
)

process.selectedGenJets = cms.EDFilter("GenJetRefSelector",
  src = cms.InputTag("ak4GenJetsNoNu"),
  cut = cms.string("pt > 10.0")
)

process.icGenJetProducer = producers.icGenJetProducer.clone(
  branch  = cms.string("genJets"),
  input   = cms.InputTag("selectedGenJets"),
  inputGenParticles = cms.InputTag("genParticles"),
  requestGenParticles = cms.bool(False)
)

process.icPileupInfoProducer = producers.icPileupInfoProducer.clone()

process.icGenSequence += (
  process.prunedGenParticles+
  process.icGenParticleProducer+
  process.icPileupInfoProducer+
  process.selectedGenJets+
  process.icGenJetProducer
)

################################################################
# EventInfo
################################################################
process.icEventInfoProducer = producers.icEventInfoProducer.clone(
  includeJetRho       = cms.bool(False),
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


process.tauParticles = cms.EDProducer("GenParticlePruner",
    src = cms.InputTag("genParticles"),
    select = cms.vstring(
      "drop *", # this is the default
      "keep abs(pdgId) == 15"  #keep event summary (status=3 for pythia6, 21 <= status <= 29 for pythia8)
    )
)

process.zTauTauFilter = cms.EDFilter("CandViewCountFilter",
    src = cms.InputTag("tauParticles"),
    minNumber = cms.uint32(2),
  )


process.p = cms.Path(
  process.tauParticles+
  process.zTauTauFilter+
  process.extraPreSequence+
  process.icSelectionSequence+
  process.electronPFIsolationValuesSequence+
  process.icVertexSequence+
  process.icPFSequence+
  # process.icElectronSequence+
  # process.icMuonSequence+
  process.icTauProducer+
  process.icTrackSequence+
  process.icPFJetSequence+
  process.icGenSequence+
  process.icEventInfoSequence+
  process.icEventProducer
)

# process.schedule = cms.Schedule(process.p)
process.schedule = cms.Schedule(process.p)

#print process.dumpPython()
