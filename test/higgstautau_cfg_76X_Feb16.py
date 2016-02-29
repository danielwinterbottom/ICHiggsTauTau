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
#'root://xrootd.unl.edu//store/mc/RunIISpring15MiniAODv2/SUSYGluGluToHToTauTau_M-160_TuneCUETP8M1_13TeV-pythia8/MINIAODSIM/74X_mcRun2_asymptotic_v2-v1/40000/10563B6E-D871-E511-9513-B499BAABD280.root',parser.VarParsing.multiplicity.singleton,
#'root://xrootd.unl.edu//store/mc/RunIISpring15MiniAODv2/SUSYGluGluToHToTauTau_M-1000_TuneCUETP8M1_13TeV-pythia8/MINIAODSIM/74X_mcRun2_asymptotic_v2-v1/50000/9EF16FCE-E771-E511-AAB0-008CFA1979EC.root',parser.VarParsing.multiplicity.singleton,
#'root://xrootd.unl.edu//store/data/Run2015B/SingleElectron/MINIAOD/PromptReco-v1/000/251/163/00000/9C435096-9F26-E511-A1D7-02163E012AB6.root',parser.VarParsing.multiplicity.singleton,
#'root://xrootd.unl.edu//store/data/Run2015D/MuonEG/MINIAOD/PromptReco-v3/000/256/630/00000/24F810E0-335F-E511-94F4-02163E011C61.root', parser.VarParsing.multiplicity.singleton,
#'root://xrootd.unl.edu//store/mc/RunIISpring15MiniAODv2/TTJets_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8/MINIAODSIM/74X_mcRun2_asymptotic_v2-v1/00000/0014DC94-DC5C-E511-82FB-7845C4FC39F5.root', parser.VarParsing.multiplicity.singleton,
#'root://xrootd.unl.edu//store/mc/RunIISpring15DR74/SUSYGluGluToHToTauTau_M-160_TuneCUETP8M1_13TeV-pythia8/AODSIM/Asympt25ns_MCRUN2_74_V9-v1/10000/02D2D410-2A03-E511-8F6C-0025905A60A8.root', parser.VarParsing.multiplicity.singleton,
#'root://xrootd.unl.edu//store/data/Run2015D/Tau/MINIAOD/16Dec2015-v1/00000/006DFE2F-B2B6-E511-A7B6-3417EBE65E39.root',parser.VarParsing.multiplicity.singleton,
#'root://xrootd.unl.edu//store/mc/RunIISpring15DR74/SUSYGluGluToHToTauTau_M-160_TuneCUETP8M1_13TeV-pythia8/MINIAODSIM/Asympt25ns_MCRUN2_74_V9-v1/10000/2A3929AE-5303-E511-9EFE-0025905A48C0.root', parser.VarParsing.multiplicity.singleton,
#opts.register('file',
'root://xrootd.unl.edu//store/mc/RunIIFall15MiniAODv2/WZJets_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8/MINIAODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/00000/00D30F00-1EB8-E511-A3A0-FA163E2815B2.root',parser.VarParsing.multiplicity.singleton,
#'root://xrootd.unl.edu//store/mc/RunIIFall15MiniAODv2/DY2JetsToLL_M-50_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/MINIAODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/10000/044F8A3A-43B8-E511-8F98-0025904CF75A.root',parser.VarParsing.multiplicity.singleton,
#'root://xrootd.unl.edu//store/mc/RunIIFall15MiniAODv2/GluGluHToTauTau_M125_13TeV_powheg_pythia8/MINIAODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/20000/4AAC498F-8BB8-E511-A9E0-FA163E84A67A.root',parser.VarParsing.multiplicity.singleton,
#'root://xrootd.unl.edu//store/data/Run2015D/SingleElectron/MINIAOD/16Dec2015-v1/20000/00050EF1-F9A6-E511-86B2-0025905A48D0.root',parser.VarParsing.multiplicity.singleton,
#'root://xrootd.unl.edu//store/mc/RunIIFall15MiniAODv2/SUSYGluGluToHToTauTau_M-160_TuneCUETP8M1_13TeV-pythia8/MINIAODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/50000/12184969-3DB8-E511-879B-001E67504A65.root',parser.VarParsing.multiplicity.singleton,
#'root://xrootd.unl.edu//store/mc/RunIISpring15MiniAODv2/TT_TuneCUETP8M1_13TeV-powheg-pythia8/MINIAODSIM/74X_mcRun2_asymptotic_v2_ext3-v1/10000/0041D4C0-D86E-E511-8D6B-001E67A3E8F9.root',parser.VarParsing.multiplicity.singleton,
#'root://xrootd.unl.edu//store/data/Run2015C/SingleElectron/MINIAOD/PromptReco-v1/000/254/317/00000/C4F3838C-8345-E511-9AA9-02163E011FE4.root', parser.VarParsing.multiplicity.singleton,
#'root://xrootd.unl.edu//store/data/Run2015B/SingleElectron/MINIAOD/PromptReco-v1/000/251/164/00000/4633CC68-A326-E511-95D0-02163E0124EA.root', parser.VarParsing.multiplicity.singleton,
#opts.register('file','root://xrootd.unl.edu//store/mc/RunIISpring15DR74/DYJetsToLL_M-50_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8/MINIAODSIM/Asympt25ns_MCRUN2_74_V9-v3/10000/009D49A5-7314-E511-84EF-0025905A605E.root',parser.VarParsing.multiplicity.singleton,
#opts.register('file', 'file:miniaod_gg.root', parser.VarParsing.multiplicity.singleton,
#opts.register('file', 'root://xrootd.unl.edu//store/mc/Phys14DR/GluGluToHToTauTau_M-125_13TeV-powheg-pythia6/MINIAODSIM/PU20bx25_tsg_PHYS14_25_V1-v1/00000/2405749F-8B6F-E411-88EE-848F69FD2910.root', parser.VarParsing.multiplicity.singleton,
#opts.register('file', 'root://xrootd.unl.edu//store/mc/Phys14DR/VBF_HToTauTau_M-125_13TeV-powheg-pythia6/MINIAODSIM/PU40bx25_PHYS14_25_V1-v1/00000/36224FE2-0571-E411-9664-00266CFAE30C.root', parser.VarParsing.multiplicity.singleton,
    parser.VarParsing.varType.string, "input file")
#opts.register('globalTag', '76X_dataRun2_v15', parser.VarParsing.multiplicity.singleton,
opts.register('globalTag', '76X_mcRun2_asymptotic_v12', parser.VarParsing.multiplicity.singleton,
#opts.register('globalTag', '74X_mcRun2_asymptotic_v2', parser.VarParsing.multiplicity.singleton,
    parser.VarParsing.varType.string, "global tag")
opts.register('isData', 0, parser.VarParsing.multiplicity.singleton,
    parser.VarParsing.varType.int, "Process as data?")
#opts.register('release', '7412MINIAOD', parser.VarParsing.multiplicity.singleton,
opts.register('release', '76XMINIAOD', parser.VarParsing.multiplicity.singleton,
    parser.VarParsing.varType.string, "Release label")
opts.register('doHT', 0, parser.VarParsing.multiplicity.singleton,
    parser.VarParsing.varType.int, "Store HT and number of outgoing partons?")

opts.parseArguments()
infile      = opts.file
if not infile: infile = "file:/tmp/file.root"
isData      = opts.isData
tag         = opts.globalTag
release     = opts.release
if not isData:
  doHT     = opts.doHT
else:
  doHT     = 0
#isEmbedded  = opts.isEmbedded
#isTandP     = opts.isTandP
#isZStudy    = opts.isZStudy
#isPhys14    = opts.isPhys14

if not release in ["76X", "76XMINIAOD"]:
  print 'Release not recognised, exiting!'
  sys.exit(1)
print 'release     : '+release
print 'isData      : '+str(isData)
print 'globalTag   : '+str(tag)
print 'doHT        : '+str(doHT)

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
  input = cms.untracked.int32(100)
)

process.MessageLogger.cerr.FwkReport.reportEvery = 50

process.options   = cms.untracked.PSet(
  wantSummary = cms.untracked.bool(True)
)
process.options.allowUnschduled = cms.untracked.bool(True)


################################################################
# Input files and global tags
################################################################
process.load("CondCore.DBCommon.CondDBCommon_cfi")
from CondCore.DBCommon.CondDBSetup_cfi import *
if not isData:
  process.jec = cms.ESSource("PoolDBESSource",
    DBParameters = cms.PSet(
     messageLevel = cms.untracked.int32(0)
    ),
    timetype = cms.string('runnumber'),
    toGet = cms.VPSet(
    cms.PSet(
      record = cms.string('JetCorrectionsRecord'),
      tag = cms.string('JetCorrectorParametersCollection_Fall15_25nsV2_MC_AK4PF'),
      label = cms.untracked.string('AK4PF')
      ),
    cms.PSet(
      record = cms.string('JetCorrectionsRecord'),
      tag = cms.string('JetCorrectorParametersCollection_Fall15_25nsV2_MC_AK4PFchs'),
      label = cms.untracked.string('AK4PFchs')
      ),
    ),
    connect = cms.string('sqlite:Fall15_25nsV2_MC.db')
  )

  process.es_prefer_jec = cms.ESPrefer('PoolDBESSource','jec')

else:
  process.jec = cms.ESSource("PoolDBESSource",
    DBParameters = cms.PSet(
     messageLevel = cms.untracked.int32(0)
    ),
    timetype = cms.string('runnumber'),
    toGet = cms.VPSet(
    cms.PSet(
      record = cms.string('JetCorrectionsRecord'),
      tag = cms.string('JetCorrectorParametersCollection_Fall15_25nsV2_DATA_AK4PF'),
      label = cms.untracked.string('AK4PF')
      ),
    cms.PSet(
      record = cms.string('JetCorrectionsRecord'),
      tag = cms.string('JetCorrectorParametersCollection_Fall15_25nsV2_DATA_AK4PFchs'),
      label = cms.untracked.string('AK4PFchs')
      ),
    ),
    connect = cms.string('sqlite:Fall15_25nsV2_DATA.db')
  )

  process.es_prefer_jec = cms.ESPrefer('PoolDBESSource','jec')

process.source = cms.Source("PoolSource", fileNames = cms.untracked.vstring(
#process.source = cms.Source("PoolSource", fileNames = cms.untracked.vstring(
#'root://xrootd.unl.edu//store/mc/RunIISpring15MiniAODv2/QCD_HT200to300_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/MINIAODSIM/74X_mcRun2_asymptotic_v2-v1/30000/D61F7B94-676F-E511-9970-00221981B434.root',
#'root://xrootd.unl.edu//store/mc/RunIISpring15MiniAODv2/QCD_HT200to300_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/MINIAODSIM/74X_mcRun2_asymptotic_v2-v1/30000/103BC4D8-666F-E511-AAC3-00221981AF26.root'
#'root://xrootd.unl.edu//store/mc/RunIISpring15DR74/TTJets_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8/AODSIM/Asympt25ns_MCRUN2_74_V9-v1/00000/00B2F04A-E301-E511-8CF3-0025905A6090.root',
# 'root://xrootd.unl.edu//store/mc/RunIISpring15DR74/SUSYGluGluToHToTauTau_M-160_TuneCUETP8M1_13TeV-pythia8/AODSIM/Asympt25ns_MCRUN2_74_V9-v1/10000/02D2D410-2A03-E511-8F6C-0025905A60A8.root',
# 'root://xrootd.unl.edu//store/mc/RunIISpring15DR74/SUSYGluGluToHToTauTau_M-160_TuneCUETP8M1_13TeV-pythia8/MINIAODSIM/Asympt25ns_MCRUN2_74_V9-v1/10000/2A3929AE-5303-E511-9EFE-0025905A48C0.root',
#'root://xrootd.unl.edu//store/data/Run2015B/SingleElectron/MINIAOD/PromptReco-v1/000/251/161/00000/7019DC27-9C26-E511-84FF-02163E011CC2.root',
#'root://xrootd.unl.edu//store/data/Run2015B/SingleElectron/MINIAOD/PromptReco-v1/000/251/162/00000/9CC606D8-4127-E511-8F35-02163E013830.root',
#'root://xrootd.unl.edu//store/data/Run2015B/SingleElectron/MINIAOD/PromptReco-v1/000/251/163/00000/9C435096-9F26-E511-A1D7-02163E012AB6.root',
#'root://xrootd.unl.edu//store/data/Run2015B/SingleElectron/MINIAOD/PromptReco-v1/000/251/164/00000/4633CC68-A326-E511-95D0-02163E0124EA.root'
#'root://xrootd.unl.edu//store/data/Run2015B/Tau/MINIAOD/PromptReco-v1/000/251/642/00000/EC1989CD-EB2A-E511-8F15-02163E0146A4.root',

#),
#lumisToProcess = cms.untracked.VLuminosityBlockRange('1:100317','1:98527'), 
#)

#'root://xrootd.unl.edu//store/data/Run2015B/SingleElectron/MINIAOD/PromptReco-v1/000/251/161/00000/7019DC27-9C26-E511-84FF-02163E011CC2.root',
#'root://xrootd.unl.edu//store/data/Run2015B/SingleElectron/MINIAOD/PromptReco-v1/000/251/162/00000/9CC606D8-4127-E511-8F35-02163E013830.root',
#'root://xrootd.unl.edu//store/data/Run2015B/SingleElectron/MINIAOD/PromptReco-v1/000/251/163/00000/9C435096-9F26-E511-A1D7-02163E012AB6.root',
#'root://xrootd.unl.edu//store/data/Run2015B/SingleElectron/MINIAOD/PromptReco-v1/000/251/164/00000/4633CC68-A326-E511-95D0-02163E0124EA.root'
#'root://xrootd.unl.edu//store/data/Run2015B/Tau/MINIAOD/PromptReco-v1/000/251/642/00000/EC1989CD-EB2A-E511-8F15-02163E0146A4.root',

infile
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
process.load("RecoTauTag/Configuration/RecoPFTauTag_cff")
#switchToPFTauHPS(process)

process.ic74XSequence = cms.Sequence()
process.icMiniAODSequence = cms.Sequence()

#if release in ['74X']:
#  process.ic74XSequence += process.PFTau
#  process.ic74XSequence += process.produceHPSPFTaus

# if release in ['70XMINIAOD', '74XMINIAOD','7412MINIAOD']:
#   process.load('PhysicsTools.PatAlgos.slimming.unpackedTracksAndVertices_cfi')
#   process.icMiniAODSequence += process.unpackedTracksAndVertices
#switchOnTrigger(process, outputModule="")


################################################################
# Need to create kt6PFJets in 42X for L1FastJet correction
################################################################
#if release in ['74X', '74XMINIAOD','7412MINIAOD']:
#  from RecoJets.JetProducers.kt4PFJets_cfi import kt4PFJets
#  process.kt6PFJets = kt4PFJets.clone(
#    rParam = cms.double(0.6),
#    doAreaFastjet = cms.bool(True),
#    doRhoFastjet = cms.bool(True)
#  )
#  process.ic74XSequence += process.kt6PFJets


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

if release in ['76XMINIAOD']:
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


process.icSelectionSequence = cms.Sequence()

if release in ['76X']:
  process.icSelectionSequence += cms.Sequence(
    process.PFTau+
    process.produceHPSPFTaus
 )


process.icSelectionSequence += cms.Sequence(
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
if release in ['76X']:
  process.pfPileUp.PFCandidates = cms.InputTag("particleFlowPtrs")
  process.pfPileUpIso.PFCandidates = cms.InputTag("particleFlowPtrs")
  process.pfNoPileUp.bottomCollection = cms.InputTag("particleFlowPtrs")
  process.pfNoPileUpIso.bottomCollection = cms.InputTag("particleFlowPtrs")


if release in ['76XMINIAOD']:
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
if release in ['76XMINIAOD']:
  vtxLabel = cms.InputTag("offlineSlimmedPrimaryVertices")

################################################################
# Vertices
################################################################
process.icVertexProducer = producers.icVertexProducer.clone(
  branch  = cms.string("vertices"),
  input = vtxLabel,
  firstVertexOnly = cms.bool(True)
)

process.icGenVertexProducer = producers.icGenVertexProducer.clone(
  input = cms.InputTag("prunedGenParticles")
)


process.icVertexSequence = cms.Sequence(
  process.icVertexProducer+
  process.icGenVertexProducer
)

if isData :
  process.icVertexSequence.remove(process.icGenVertexProducer)

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
if release in ['76XMINIAOD']:
  electronLabel = cms.InputTag("slimmedElectrons")

process.icElectronSequence = cms.Sequence()

#ICElectronConversionCalculator NOT final, but at least have a running version for now
process.icElectronConversionCalculator = cms.EDProducer('ICElectronConversionCalculator',
    input       = electronLabel, 
    beamspot    = cms.InputTag("offlineBeamSpot"),
    conversions = cms.InputTag("allConversions")
)

if release in ['76XMINIAOD']:
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
#if release in ['74XMINIAOD','7412MINIAOD']:
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
#if release in ['74XMINIAOD','7412MINIAOD']:
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

if release in ['76XMINIAOD']:
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

if release in ['76XMINIAOD']:
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

  process.elEcalPFClusterIso = cms.EDProducer('ICElectronIsolation',
    input        = electronLabel,
    deltaR       = cms.double(0.3), 
    iso_type     = cms.string("ecal_pf_cluster_iso")
  )

  process.elHcalPFClusterIso = cms.EDProducer('ICElectronIsolation',
    input        = electronLabel,
    deltaR       = cms.double(0.3), 
    iso_type     = cms.string("hcal_pf_cluster_iso")
  )

if release in ['76X']:#Need to recalculate this as 04 isolation is stored in pat not reco electrons
  process.elPFIsoValueCharged04PFIdPFIso    = process.elPFIsoValueCharged04PFId.clone()
  process.elPFIsoValueGamma04PFIdPFIso      = process.elPFIsoValueGamma04PFId.clone()
  process.elPFIsoValueNeutral04PFIdPFIso    = process.elPFIsoValueNeutral04PFId.clone()
  process.elPFIsoValuePU04PFIdPFIso         = process.elPFIsoValuePU04PFId.clone()
  process.elPFIsoDepositCharged.src     = electronLabel
  process.elPFIsoDepositNeutral.src     = electronLabel
  process.elPFIsoDepositGamma.src       = electronLabel
  process.elPFIsoDepositPU.src          = electronLabel
  process.elPFIsoValueGamma04PFIdPFIso.deposits[0].vetos = (
      cms.vstring('EcalEndcaps:ConeVeto(0.08)','EcalBarrel:ConeVeto(0.08)'))
  process.elPFIsoValueNeutral04PFIdPFIso.deposits[0].vetos = (
      cms.vstring())
  process.elPFIsoValuePU04PFIdPFIso.deposits[0].vetos = (
      cms.vstring())
  process.elPFIsoValueCharged04PFIdPFIso.deposits[0].vetos = (
      cms.vstring('EcalEndcaps:ConeVeto(0.015)'))
  process.electronPFIsolationValuesSequence += cms.Sequence(
    process.elPFIsoDepositCharged+
    process.elPFIsoDepositNeutral+
    process.elPFIsoDepositGamma+
    process.elPFIsoDepositPU
  )


process.elPFIsoValueChargedAll04PFIdPFIso = process.elPFIsoValueChargedAll04PFId.clone()
process.elPFIsoValueChargedAll04PFIdPFIso.deposits[0].vetos = (
  cms.vstring('EcalEndcaps:ConeVeto(0.015)','EcalBarrel:ConeVeto(0.01)'))

process.electronPFIsolationValuesSequence +=cms.Sequence(
  process.elPFIsoValueCharged04PFIdPFIso+
  process.elPFIsoValueGamma04PFIdPFIso+
  process.elPFIsoValuePU04PFIdPFIso+
  process.elPFIsoValueNeutral04PFIdPFIso+
  process.elPFIsoValueChargedAll04PFIdPFIso+
  process.elEcalPFClusterIso+
  process.elHcalPFClusterIso
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
     mvaNonTrigSpring15    = cms.InputTag("electronMVAValueMapProducer:ElectronMVAEstimatorRun2Spring15NonTrig25nsV1Values"),
     mvaTrigSpring15       = cms.InputTag("electronMVAValueMapProducer:ElectronMVAEstimatorRun2Spring15Trig25nsV1Values")
  ),
  includeClusterIso        = cms.bool(True),
  includePFIso03           = cms.bool(True),
  includePFIso04           = cms.bool(True)
)


if release in ['76X']:
  process.icElectronProducer.includeClusterIso = cms.bool(False)

process.icElectronSequence += cms.Sequence(
  process.icElectronConversionCalculator+
  process.icElectronProducer
)


################################################################
# Muons
################################################################
process.icMuonSequence = cms.Sequence()
if release in ['76X']: muons = cms.InputTag("muons")
if release in ['76XMINIAOD']: muons = cms.InputTag("slimmedMuons")

process.load("CommonTools.ParticleFlow.Isolation.pfMuonIsolation_cff")
process.load("CommonTools.ParticleFlow.deltaBetaWeights_cff")
#process.muPFIsoDepositCharged.src     = muons
process.muPFIsoDepositChargedAll.src  = muons #All charged isolation is not stored in reco/pat
process.muPFIsoDepositNeutral.src     = muons
process.muPFIsoDepositNeutral.ExtractorPSet.inputCandView = cms.InputTag("pfWeightedNeutralHadrons")
process.muPFIsoDepositGamma.src       = muons
process.muPFIsoDepositGamma.ExtractorPSet.inputCandView = cms.InputTag("pfWeightedPhotons")
#process.muPFIsoDepositNeutral.src     = muons
#process.muPFIsoDepositGamma.src       = muons
#process.muPFIsoDepositPU.src          = muons
#process.icMuonSequence += cms.Sequence(
#    process.muonPFIsolationDepositsSequence+
#    process.muonPFIsolationValuesSequence
#    )

#process.muPFIsoValueCharged03PFIso = process.muPFIsoValueCharged03.clone()
process.muPFIsoValueChargedAll03PFIso = process.muPFIsoValueChargedAll03.clone()
process.muPFIsoValueNeutral04PFWeights = process.muPFIsoValueNeutral04.clone()
process.muPFIsoValueGamma04PFWeights = process.muPFIsoValueGamma04.clone()
process.muPFIsoValueNeutral03PFWeights = process.muPFIsoValueNeutral03.clone()
process.muPFIsoValueGamma03PFWeights = process.muPFIsoValueGamma03.clone()
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
   process.muPFIsoValuePU03PFIso+
   process.muPFIsoValueNeutral04PFWeights+
   process.muPFIsoValueGamma04PFWeights+
   process.muPFIsoValueNeutral03PFWeights+
   process.muPFIsoValueGamma03PFWeights
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
    process.pfDeltaBetaWeightingSequence+
    process.muPFIsoDepositChargedAll+
    process.muPFIsoDepositNeutral+
    process.muPFIsoDepositGamma+
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
  includeDoubles = cms.PSet(
   neutral_pfweighted_iso_03 = cms.InputTag("muPFIsoValueNeutral03PFWeights"),
   neutral_pfweighted_iso_04 = cms.InputTag("muPFIsoValueNeutral04PFWeights"),
   gamma_pfweighted_iso_03 = cms.InputTag("muPFIsoValueGamma03PFWeights"),
   gamma_pfweighted_iso_04 = cms.InputTag("muPFIsoValueGamma04PFWeights")
  ),
  requestTracks           = cms.bool(True),
  includePFIso03           = cms.bool(True),
  includePFIso04           = cms.bool(True),
)
if release in ['76XMINIAOD']: process.icMuonProducer.isPF = cms.bool(False)

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
  tauIDs = tauIDs.dynamicStripIds
)

if release in ['76XMINIAOD']:
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

################################################################
# L1 Taus
################################################################

process.icL1ExtraTauProducer = cms.EDProducer("ICCandidateProducer",
  branch                     = cms.string("l1isoTaus"),
  input                      = cms.InputTag("l1extraParticles","IsoTau","RECO")
)

if not isData:
  process.icTauSequence+= cms.Sequence(
    process.icL1ExtraTauProducer
  )

# ################################################################
# # Jets
# ################################################################
from RecoJets.JetProducers.ak4PFJets_cfi import ak4PFJets
from PhysicsTools.PatAlgos.producersLayer1.jetUpdater_cff import patJetCorrFactorsUpdated
from PhysicsTools.PatAlgos.producersLayer1.jetUpdater_cff import patJetsUpdated

if release in ['76XMINIAOD']:
  #rebuild ak4 chs jets as in  https://twiki.cern.ch/twiki/bin/view/CMSPublic/WorkBookMiniAOD#Jets
  process.load('PhysicsTools.PatAlgos.slimming.unpackedTracksAndVertices_cfi')
  process.pfchs=cms.EDFilter("CandPtrSelector",src=cms.InputTag("packedPFCandidates"),cut=cms.string("fromPV"))
  process.ak4PFJetsCHS = ak4PFJets.clone(src='pfchs',doAreaFastjet=True)

  #Also make non-chs jets:
  process.ak4PFJets = ak4PFJets.clone(src='packedPFCandidates',doAreaFastjet=True)
   
  #Reapply JECs on miniAOD jets:
  process.patJetCorrFactorsReapplyJEC = patJetCorrFactorsUpdated.clone(
    src = cms.InputTag("slimmedJets"),
    levels = ['L1FastJet','L2Relative','L3Absolute'],
    payload = 'AK4PFchs')

  process.patJetsReapplyJEC = patJetsUpdated.clone(
    jetSource = cms.InputTag("slimmedJets"),
    jetCorrFactorsSource = cms.VInputTag(cms.InputTag("patJetCorrFactorsReapplyJEC"))
    )
 
  process.selectedSlimmedJetsAK4 = cms.EDFilter("PATJetRefSelector",
      src = cms.InputTag("patJetsReapplyJEC"),
      cut = cms.string("pt > 15")
      )

if release in ['74X']:
  process.load("CommonTools.ParticleFlow.pfNoPileUpJME_cff")
  process.ak4PFJetsCHS = ak4PFJets.clone(src = cms.InputTag("pfNoPileUpJME"),doAreaFastjet=True)
  process.ak4PFJets = ak4PFJets.clone(doAreaFastjet=True)


#if release in ['76XMINIAOD']:
#   process.kt6PFJets = kt4PFJets.clone(
#       src = 'packedPFCandidates',
#       rParam = cms.double(0.6),
#       doAreaFastjet = cms.bool(True),
#       doRhoFastjet = cms.bool(True)
#       )

 # Parton flavour
 # --------------
#process.jetPartons = cms.EDProducer("PartonSelector",
#     src = cms.InputTag("genParticles"),
#     withLeptons = cms.bool(False)
#)

#process.pfJetPartonMatches = cms.EDProducer("JetPartonMatcher",
#     jets = cms.InputTag("ak4PFJetsCHS"),
#     coneSizeToAssociate = cms.double(0.3),
#     partons = cms.InputTag("jetPartons")
#)

process.jetPartons = cms.EDProducer('HadronAndPartonSelector',
  src = cms.InputTag("generator"),
  particles = cms.InputTag("genParticles","","HLT"),
  partonMode = cms.string("Auto")
)

process.pfJetFlavourAssociation = cms.EDProducer("JetFlavourClustering",
     jets = cms.InputTag("ak4PFJetsCHS"),
     bHadrons = cms.InputTag("jetPartons","bHadrons"),
     cHadrons = cms.InputTag("jetPartons","cHadrons"),
     partons = cms.InputTag("jetPartons","algorithmicPartons"),
     leptons = cms.InputTag("jetPartons","leptons"),
     jetAlgorithm = cms.string("AntiKt"),
     rParam = cms.double(0.4),
     ghostRescaling = cms.double(1e-18),
     hadronFlavourHasPriority = cms.bool(False)
)


process.icPFJetFlavourCalculator = cms.EDProducer('ICJetFlavourCalculator',
     input       = cms.InputTag("ak4PFJetsCHS"),
     flavourMap  = cms.InputTag("pfJetFlavourAssociation")
)


if release in ['76XMINIAOD']:
   process.jetPartons.particles = cms.InputTag("prunedGenParticles","","PAT")



 # Jet energy corrections
 # ----------------------
process.ak4PFL1FastjetCHS = cms.EDProducer("L1FastjetCorrectorProducer",
#    srcRho = cms.InputTag("kt6PFJets", "rho"),
    srcRho = cms.InputTag("fixedGridRhoFastjetAll"),
    algorithm = cms.string('AK4PFchs'),
    level = cms.string('L1FastJet')
)
process.ak4PFL2RelativeCHS = cms.EDProducer("LXXXCorrectorProducer",
    algorithm = cms.string('AK4PFchs'),
    level = cms.string('L2Relative')
)
process.ak4PFL3AbsoluteCHS = cms.EDProducer("LXXXCorrectorProducer",
    algorithm = cms.string('AK4PFchs'),
    level = cms.string('L3Absolute')
)
process.ak4PFResidualCHS = cms.EDProducer("LXXXCorrectorProducer",
    algorithm = cms.string('AK4PFchs'),
    level = cms.string('L2L3Residual')
)

#Corrections applied to miniaod slimmedJets
pfJECS = cms.PSet(
  L1FastJet  = cms.string("ak4PFL1FastjetCHS"),
  L2Relative = cms.string("ak4PFL2RelativeCHS"),
  L3Absolute = cms.string("ak4PFL3AbsoluteCHS")
)
if isData: pfJECS = cms.PSet(
  L1FastJet  = cms.string("ak4PFL1FastjetCHS"),
  L2Relative = cms.string("ak4PFL2RelativeCHS"),
  L3Absolute = cms.string("ak4PFL3AbsoluteCHS"),
  L2L3Residual = cms.string("ak4PFResidualCHS")
)

 # b-tagging
 # ---------
process.load("RecoJets.JetAssociationProducers.ak4JTA_cff")
from RecoJets.JetAssociationProducers.ak4JTA_cff import ak4JetTracksAssociatorAtVertex
process.load("RecoBTag.Configuration.RecoBTag_cff")
import RecoBTag.Configuration.RecoBTag_cff as btag
process.jetTracksAssociatorAtVertexAK4PFCHS = ak4JetTracksAssociatorAtVertex.clone(
  jets = cms.InputTag("ak4PFJetsCHS")
)

if release in ['76XMINIAOD']:
  process.jetTracksAssociatorAtVertexAK4PFCHS.tracks = cms.InputTag("unpackedTracksAndVertices")
  process.jetTracksAssociatorAtVertexAK4PFCHS.pvSrc = cms.InputTag("unpackedTracksAndVertices")

#if isEmbedded:
#  process.jetTracksAssociatorAtVertexAK5PF.tracks = cms.InputTag("tmfTracks")

process.impactParameterTagInfosAK4PFCHS = btag.pfImpactParameterTagInfos.clone(
  #jetTracks = cms.InputTag('jetTracksAssociatorAtVertexAK4PFCHS')
)
if release in ['76XMINIAOD']:
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

#if release in ['74X',]:
process.btaggingSequenceAK4PF += cms.Sequence(
  process.pfImpactParameterTagInfos+
  process.pfSecondaryVertexTagInfos+
  process.pfCombinedSecondaryVertexV2BJetTags
)

if release in ['76XMINIAOD']:
  process.pfImpactParameterTagInfos.primaryVertex = cms.InputTag("offlineSlimmedPrimaryVertices")
  process.pfImpactParameterTagInfos.candidates = cms.InputTag("packedPFCandidates")

 # Pileup ID
 # ---------
 # Recalculated puJetId isn't the same as miniaod stored - should investigate 
#stdalgos = cms.VPSet()
#from RecoJets.JetProducers.PileupJetIDParams_cfi import *
#stdalgos = cms.VPSet(full_5x_chs,cutbased)
process.load('RecoJets.JetProducers.PileupJetID_cfi')

if release in ['76XMINIAOD']:
  process.pileupJetIdCalculator.vertexes = cms.InputTag("offlineSlimmedPrimaryVertices")
process.pileupJetIdCalculator.jets = cms.InputTag("ak4PFJetsCHS")
process.pileupJetIdCalculator.rho = cms.InputTag("fixedGridRhoFastjetAll")
process.pileupJetIdEvaluator.jets = cms.InputTag("ak4PFJetsCHS")
process.pileupJetIdEvaluator.rho = cms.InputTag("fixedGridRhoFastjetAll")

#process.puJetMvaRe = cms.EDProducer('PileupJetIdProducer',
#    produceJetIds = cms.bool(True),
#    jetids = cms.InputTag(""),
#    runMvas = cms.bool(True),
    #jets = cms.InputTag("slimmedJets"),
#    jets = cms.InputTag("ak4PFJetsCHS"),
#    vertexes = cms.InputTag("offlinePrimaryVertices"),
#    vertexes = cms.InputTag("unpackedTracksAndVertices"),
#    algos = cms.VPSet(stdalgos),
#    rho     = cms.InputTag("kt6PFJets", "rho"),
#    rho     = cms.InputTag("fixedGridRhoFastjetAll"),
#    jec     = cms.string("AK4PFchs"),
#    applyJec = cms.bool(True),
#    inputIsCorrected = cms.bool(False),
#    residualsFromTxt = cms.bool(False),
#    residualsTxt     = cms.FileInPath("RecoJets/JetProducers/data/dummy.txt"),
#)

#if release in ['76XMINIAOD']:
#  process.puJetMvaRe.vertexes = cms.InputTag("offlineSlimmedPrimaryVertices")

#if release in ['76X', '76XMINIAOD']:
#  process.puJetMvaRe.residualsTxt = cms.FileInPath("RecoJets/JetProducers/BuildFile.xml")



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
      inputPileupID         = cms.InputTag("pileupJetIdEvaluator", "fullDiscriminant"),
      includeTrackBasedVars = cms.bool(False),
      inputTracks           = cms.InputTag("generalTracks"),
      inputVertices         = vtxLabel,
      requestTracks         = cms.bool(False)
    )
)

if isData:
  process.icPFJetProducer.srcConfig.includeJetFlavour = cms.bool(False)


if release in ['76XMINIAOD']:
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
if release in ['76XMINIAOD']:
  process.icPFJetSequence += cms.Sequence(
     process.pfchs+
     process.patJetCorrFactorsReapplyJEC+
     process.patJetsReapplyJEC+
     process.selectedSlimmedJetsAK4+
     process.unpackedTracksAndVertices+
     process.icPFJetProducerFromPat
     )
if release in ['76X']:
  process.icPFJetSequence += cms.Sequence(
    process.pfNoPileUpJMESequence
    )
if release in ['76X', '76XMINIAOD']:
  process.icPFJetProducer.srcConfig.BTagDiscriminators = cms.PSet(
  pfCombinedSecondaryVertexV2BJetTags = cms.InputTag("pfCombinedSecondaryVertexV2BJetTags")
  )
  process.icPFJetSequence += cms.Sequence(
    process.ak4PFL1FastjetCHS+
    process.ak4PFL2RelativeCHS+
    process.ak4PFL3AbsoluteCHS+
    process.ak4PFResidualCHS+
    process.ak4PFJetsCHS+
    process.pileupJetIdCalculator+
    process.pileupJetIdEvaluator+ 
    process.jetPartons+
#      process.pfJetPartonMatches+
    process.pfJetFlavourAssociation+
    process.icPFJetFlavourCalculator+
    process.btaggingSequenceAK4PF+
    process.icPFJetProducer #Not from slimmed jets!
    )

  if not isData:
    process.icPFJetSequence.remove(process.ak4PFResidualCHS)

  if isData:
    process.icPFJetSequence.remove(process.jetPartons) 
    process.icPFJetSequence.remove(process.pfJetFlavourAssociation) 
    process.icPFJetSequence.remove(process.icPFJetFlavourCalculator)
#if release in ['74XMINIAOD','7412MINIAOD']:
#  process.icPFJetSequence.remove(process.puJetMva) #This works for jets built from PackedCandidates in CMSSW74X but not yet in 72


# ################################################################
# # MVA MET and PF MET
# ################################################################
process.load('JetMETCorrections.Configuration.JetCorrectors_cff')
#process.load('RecoMET.METPUSubtraction.mvaPFMET_cff')
process.load("RecoJets.JetProducers.ak4PFJets_cfi")

from RecoMET.METProducers.PFMET_cfi import pfMet

process.pfMetRe = pfMet.clone(src = "particleFlow")

if release in ['76XMINIAOD']:
  process.pfMetRe = pfMet.clone(src = "packedPFCandidates")
  process.pfMetRe.calculateSignificance = False # this can't be easily implemented on packed PF candidates at the moment

if release in ['76X']:
  process.icPfMetProducer = producers.icMetProducer.clone(
                            input = cms.InputTag("pfMetRe"),
                            branch = cms.string("pfMet"),
                            includeCustomID = cms.bool(False),
                            inputCustomID = cms.InputTag("")
                            )

if release in ['76XMINIAOD']:
  process.icPfMetProducer = producers.icMetFromPatProducer.clone(
                           branch = cms.string("pfMet"),
                           getUncorrectedMet=cms.bool(False)
                           )
  process.icPuppiMetProducer = producers.icMetFromPatProducer.clone(
                           input=cms.InputTag("slimmedMETsPuppi"),
                           branch = cms.string("puppiMet"),
                           getUncorrectedMet=cms.bool(False)
                           )


process.icPfMetSequence = cms.Sequence(
  process.pfMetRe+
  process.icPfMetProducer
)

if release in ['76XMINIAOD']:
  process.icPfMetSequence.remove(process.pfMetRe)
  process.icPfMetSequence+=cms.Sequence(process.icPuppiMetProducer)

from RecoMET.METPUSubtraction.MVAMETConfiguration_cff import runMVAMET
runMVAMET(process, jetCollectionPF='patJetsReapplyJEC')


process.icPfMVAMetProducer = cms.EDProducer('ICPFMetFromPatProducer',
  input = cms.InputTag("MVAMET","MVAMET"),
  branch = cms.string("pfMVAMetVector"),
  includeCustomID = cms.bool(False),
  includeUserCandID = cms.bool(True),
  includeExternalMetsig = cms.bool(False),
  includeMetUncertainties = cms.bool(False),
  metuncertainties = cms.vstring(),
  metcorrections = cms.vstring(),
  includeExternalMetsigMethod2 = cms.bool(False),
  includeMetCorrections = cms.bool(False),
  doGenMet = cms.bool(False),
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
  inputCustomID = cms.InputTag("")
  )

process.icMvaMetSequence = cms.Sequence(
  process.tauDecayProducts+
  process.egmGsfElectronIDs+
  process.electronMVAValueMapProducer+
  process.electronRegressionValueMapProducer+
  process.photonIDValueMapProducer+
  process.photonMVAValueMapProducer+
  process.slimmedElectronsTight+
  process.slimmedMuonsTight+
  process.slimmedTausLoose+
  process.slimmedTausLooseCleaned+
  process.patJetsReapplyJECCleaned+
  process.pfNeutrals+
  process.neutralInJets+
  process.pfChargedPV+
  process.pfChs+
  process.pfChargedPU+
  process.pfMETCands+
  process.pfTrackMETCands+
  process.pfNoPUMETCands+
  process.pfPUMETCands+
  process.pfChargedPUMETCands+
  process.pfNeutralPUMETCands+
  process.pfNeutralPVMETCands+
  process.pfNeutralUnclusteredMETCands+
  process.pfPUCorrectedMETCands+
  process.ak4PFCHSL1FastjetCorrector+
  process.ak4PFCHSL2RelativeCorrector+
  process.ak4PFCHSL3AbsoluteCorrector+
  process.ak4PFCHSL1FastL2L3Corrector+
  process.ak4PFCHSResidualCorrector+
  process.ak4PFCHSL1FastL2L3ResidualCorrector+
  process.ak4JetsForpfMET+
  process.ak4JetsForpfTrackMET+
  process.ak4JetsForpfPUMET+
  process.ak4JetsForpfChargedPUMET+
  process.ak4JetsForpfNeutralPUMET+
  process.ak4JetsForpfNeutralPVMET+
  process.ak4JetsForpfNeutralUnclusteredMET+
  process.ak4JetsForpfPUCorrectedMET+
  process.ak4JetsForpfNoPUMET+
  process.corrpfMET+
  process.corrpfTrackMET+
  process.corrpfPUMET+
  process.corrpfChargedPUMET+
  process.corrpfNeutralPVMET+
  process.corrpfNeutralUnclusteredMET+
  process.corrpfNeutralPUMET+
  process.corrpfPUCorrectedMET+
  process.corrpfNoPUMET+
  process.pfMET+
  process.pfMETT1+
  process.patpfMET+
  process.pfTrackMET+
  process.patpfTrackMET+
  process.pfTrackMETT1+
  process.patpfTrackMETT1+
  process.pfPUCorrectedMET+
  process.pfPUMET+
  process.pfChargedPUMET+
  process.pfNeutralPUMET+
  process.patpfPUMET+
  process.patpfChargedPUMET+
  process.patpfNeutralPUMET+
  process.pfNeutralPVMET+
  process.pfNeutralUnclusteredMET+
  process.patpfNeutralPVMET+
  process.patpfNeutralUnclusteredMET+
  process.patpfPUCorrectedMET+
  process.pfPUMETT1+
  process.patpfPUMETT1+
  process.pfChargedPUMETT1+
  process.pfNeutralPUMETT1+
  process.patpfChargedPUMETT1+
  process.patpfNeutralPUMETT1+
  process.pfPUCorrectedMETT1+
  process.patpfPUCorrectedMETT1+
  process.pfNoPUMET+
  process.patpfNoPUMET+
  process.pfNoPUMETT1+
  process.patpfNoPUMETT1+
  process.patpfMETT1+
  process.tauPFMET+
  process.tauMET+
  process.tausSignificance+
  process.MVAMET+
  process.icPfMVAMetProducer
  )

if not isData:
  process.icMvaMetSequence.remove(process.ak4PFCHSResidualCorrector)
  process.icMvaMetSequence.remove(process.ak4PFCHSL1FastL2L3ResidualCorrector)

################################################################
# Simulation only: GenParticles, GenJets, PileupInfo
################################################################
process.icGenSequence = cms.Sequence()

#if release in ['74X']:
process.load("SimGeneral.HepPDTESSource.pythiapdt_cfi")
process.prunedGenParticles = cms.EDProducer("GenParticlePruner",
    src = cms.InputTag("genParticles","","HLT"),
    select = cms.vstring(
        "drop  *", # this is the default
        "++keep abs(pdgId) == 11 || abs(pdgId) == 13 || abs(pdgId) == 15", # keep leptons, with history
        "keep abs(pdgId) == 12 || abs(pdgId) == 14 || abs(pdgId) == 16",   # keep neutrinos
        "drop   status == 2",                                              # drop the shower part of the history
        "+keep pdgId == 22 && status == 1 && (pt > 10 || isPromptFinalState())", # keep gamma above 10 GeV (or all prompt) and its first parent
        "+keep abs(pdgId) == 11 && status == 1 && (pt > 3 || isPromptFinalState())", # keep first parent of electrons above 3 GeV (or prompt)
        "keep++ abs(pdgId) == 15",                                         # but keep keep taus with decays
	"drop  status > 30 && status < 70 ", 				   #remove pythia8 garbage
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
 

#process.prunedGenParticlesTaus = cms.EDProducer("ICGenParticlePruner53X",
#  src = cms.InputTag("genParticles", "", "HLT"),
#  select = cms.vstring(
#    "drop  *",
#    "keep++ abs(pdgId) == 15",  # keep full tau decay chain
#  )
#)

#if release in ['74XMINIAOD','7412MINIAOD']:
#  process.prunedGenParticlesTaus.src = cms.InputTag("prunedGenParticles","","PAT")
#  process.prunedGenParticles.src = cms.InputTag("prunedGenParticles", "", "PAT")


process.icGenParticleProducer = producers.icGenParticleProducer.clone(
  input   = cms.InputTag("prunedGenParticles"),
  includeMothers = cms.bool(True),
  includeDaughters = cms.bool(True),
  includeStatusFlags = cms.bool(True)
)

if release in ['76XMINIAOD']:
  process.icGenParticleProducer.input=cms.InputTag("prunedGenParticles","","PAT") #Store ALL pruned gen particles


process.load("RecoJets.Configuration.GenJetParticles_cff")
process.genParticlesForJets.ignoreParticleIDs = cms.vuint32(
  1000022, 2000012, 2000014,
  2000016, 1000039, 5000039,
  4000012, 9900012, 9900014,
  9900016, 39, 12, 14, 16
)
if release in ['76XMINIAOD']:
  # But of course this won't work because genParticlesForJets(InputGenJetsParticleSelector)
  # requires a vector<GenParticle> input. There's no alternative filter for the PackedGenParticle
  # type at the moment. Probably we could make our own generic cut-string selector, but
  # not in this package
  process.genParticlesForJets.src = cms.InputTag("packedGenParticles")

process.load("RecoJets.JetProducers.ak4GenJets_cfi")
process.ak4GenJetsNoNuBSM  =  process.ak4GenJets.clone()

if release in ['76XMINIAOD']:
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

if release in ['76XMINIAOD']:
  process.icGenJetProducer.branch = cms.string("genJetsReclustered")
  process.icGenJetProducer.inputGenParticles = cms.InputTag("prunedGenParticles","","PAT")
  process.icGenJetProducer.isSlimmed  = cms.bool(True)
  process.icGenJetProducerFromSlimmed = producers.icGenJetProducer.clone(
    branch = cms.string("genJets"),
    input = cms.InputTag("slimmedGenJets"),
    inputGenParticles=cms.InputTag("genParticles"),
    requestGenParticles = cms.bool(False),
    isSlimmed = cms.bool(True)
  ) 

process.icPileupInfoProducer = producers.icPileupInfoProducer.clone()
if release in ['76XMINIAOD']:
  process.icPileupInfoProducer.input=cms.InputTag("slimmedAddPileupInfo")


if not isData:
  process.icGenSequence += (
    process.prunedGenParticles+
    process.icGenParticleProducer
  )
  if release in ['76XMINIAOD']:
    process.icGenSequence.remove(process.prunedGenParticles)
    process.icGenSequence += (
      process.ak4GenJetsNoNuBSM+
      process.selectedGenJets+
      process.icGenJetProducer+
      process.icGenJetProducerFromSlimmed+
      process.icPileupInfoProducer
    )
  if release in [ '76X']:
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
process.icTriggerObjectSequence = cms.Sequence()
# if release in ['42X', '53X']:
#   process.patTriggerSequence = cms.Sequence()
#   switchOnTrigger(process, sequence = 'patTriggerSequence', outputModule = '')
#   process.patTriggerPath = cms.Path(process.patTriggerSequence)
#if release in ['70X', '74X']:
#  process.patTriggerPath = cms.Path()
#  switchOnTrigger(process, path = 'patTriggerPath', outputModule = '')

#  process.icTriggerPathProducer = producers.icTriggerPathProducer.clone(
#   branch = cms.string("triggerPaths"),
#   input  = cms.InputTag("patTriggerEvent")
#  )




process.patTriggerPath = cms.Path()
if release in ['76XMINIAOD']:
  switchOnTrigger(process, path = 'patTriggerPath',  outputModule = '')
if release in ['74X']:
  switchOnTrigger(process, outputModule = '')


if release in ['76X']:
  process.icTriggerPathProducer = producers.icTriggerPathProducer.clone(
    branch = cms.string("triggerPaths"),
    input = cms.InputTag("patTriggerEvent")
  )

#  if isData:
  process.icTriggerSequence += cms.Sequence(
    process.patTrigger+
    process.patTriggerEvent+
   process.icTriggerPathProducer
  )

if release in ['76XMINIAOD']:
  process.icTriggerPathProducer = producers.icTriggerPathProducer.clone(
   branch = cms.string("triggerPaths"),
   input  = cms.InputTag("TriggerResults","","HLT"),
   inputIsStandAlone = cms.bool(True),
   inputPrescales = cms.InputTag("patTrigger")
  )

  if isData:
    process.icTriggerSequence += cms.Sequence(
     #process.patTrigger+
     #process.patTriggerEvent+
     process.icTriggerPathProducer
    )


process.icEle12Mu23ObjectProducer = producers.icTriggerObjectProducer.clone(
      input   = cms.InputTag("patTriggerEvent"),
      branch = cms.string("triggerObjectsEle12Mu23"),
      hltPath = cms.string("HLT_Mu23_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL_v"),
      inputIsStandAlone = cms.bool(False),
      storeOnlyIfFired = cms.bool(False)
      )

process.icEle12Mu17ObjectProducer = producers.icTriggerObjectProducer.clone(
      input   = cms.InputTag("patTriggerEvent"),
      branch = cms.string("triggerObjectsEle12Mu17"),
      hltPath = cms.string("HLT_Mu17_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL_v"),
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
  
process.icEle17Mu8ObjectProducer = producers.icTriggerObjectProducer.clone(
      input   = cms.InputTag("patTriggerEvent"),
      branch = cms.string("triggerObjectsEle17Mu8"),
      hltPath = cms.string("HLT_Mu8_TrkIsoVVL_Ele17_CaloIdL_TrackIdL_IsoVL_v"),
      inputIsStandAlone = cms.bool(False),
      storeOnlyIfFired = cms.bool(False)
      )

  
process.icEle22LooseTau20ObjectProducer = producers.icTriggerObjectProducer.clone(
      input   = cms.InputTag("patTriggerEvent"),
      branch = cms.string("triggerObjectsEle22LooseTau20"),
      hltPath = cms.string("HLT_Ele22_eta2p1_WPLoose_Gsf_LooseIsoPFTau20_v"),
  #    hltPath = cms.string("HLT_Ele22_eta2p1_WP75_Gsf_LooseIsoPFTau20_v"),
      inputIsStandAlone = cms.bool(False),
      storeOnlyIfFired = cms.bool(False)
      )


  
process.icEle27GsfObjectProducer = producers.icTriggerObjectProducer.clone(
      input   = cms.InputTag("patTriggerEvent"),
      branch = cms.string("triggerObjectsEle27Gsf"),
  #    hltPath = cms.string("HLT_Ele27_eta2p1_WP75_Gsf_v"),
      hltPath = cms.string("HLT_Ele27_eta2p1_WPLoose_Gsf_v"),
      inputIsStandAlone = cms.bool(False),
      storeOnlyIfFired = cms.bool(False)
      )
  
#process.icEle32GsfObjectProducer = producers.icTriggerObjectProducer.clone(
#      input   = cms.InputTag("patTriggerEvent"),
#      branch = cms.string("triggerObjectsEle32Gsf"),
#      hltPath = cms.string("HLT_Ele32_eta2p1_WP75_Gsf_v"),
#      inputIsStandAlone = cms.bool(False),
#      storeOnlyIfFired = cms.bool(False)
#      )

process.icEle32TightGsfObjectProducer = producers.icTriggerObjectProducer.clone(
      input   = cms.InputTag("patTriggerEvent"),
      branch = cms.string("triggerObjectsEle32GsfTight"),
      hltPath = cms.string("HLT_Ele32_eta2p1_WPTight_Gsf_v"),
      inputIsStandAlone = cms.bool(False),
      storeOnlyIfFired = cms.bool(False)
      )

  
  
process.icEle22GsfObjectProducer = producers.icTriggerObjectProducer.clone(
      input   = cms.InputTag("patTriggerEvent"),
      branch = cms.string("triggerObjectsEle22Gsf"),
      hltPath = cms.string("HLT_Ele22_eta2p1_WPLoose_Gsf_v"),
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

  
process.icIsoMu17IterTrkObjectProducer = producers.icTriggerObjectProducer.clone(
      input   = cms.InputTag("patTriggerEvent"),
      branch = cms.string("triggerObjectsIsoMu17"),
      hltPath = cms.string("HLT_IsoMu17_eta2p1_v"), 
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
      hltPath = cms.string("HLT_IsoTkMu27_v"), 
      inputIsStandAlone = cms.bool(False),
      storeOnlyIfFired = cms.bool(False)
      )
  
  
process.icIsoMu22ObjectProducer = producers.icTriggerObjectProducer.clone(
    input   = cms.InputTag("patTriggerEvent"),
    branch = cms.string("triggerObjectsIsoMu22"),
    hltPath = cms.string("HLT_IsoMu22_v"),
    inputIsStandAlone = cms.bool(False),
    storeOnlyIfFired = cms.bool(False)
    )
process.icIsoMu18ObjectProducer = producers.icTriggerObjectProducer.clone(
    input = cms.InputTag("patTriggerEvent"),
    branch = cms.string("triggerObjectsIsoMu18"),
    hltPath = cms.string("HLT_IsoMu18_v"),
    inputIsStandAlone = cms.bool(False),
    storeOnlyIfFired = cms.bool(False)
    )


process.icEle23ObjectProducer = producers.icTriggerObjectProducer.clone(
    input = cms.InputTag("patTriggerEvent"),
    branch = cms.string("triggerObjectsEle23"),
    hltPath = cms.string("HLT_Ele23_WPLoose_Gsf_v"),
    inputIsStandAlone = cms.bool(False),
    storeOnlyIfFired = cms.bool(False)
    )


process.icDoubleMediumTau35ObjectProducer = producers.icTriggerObjectProducer.clone(
   input = cms.InputTag("patTriggerEvent"),
   branch = cms.string("triggerObjectsDoubleMediumTau35"),
   hltPath = cms.string("HLT_DoubleMediumIsoPFTau35_Trk1_eta2p1_Reg_v"),
   inputIsStandAlone = cms.bool(False),
   storeOnlyIfFired = cms.bool(False)
   )

if isData :
  process.icDoubleMediumTau35ObjectProducer.branch = cms.string("triggerObjectsDiTau35")

process.icDoubleMediumTau40ObjectProducer = producers.icTriggerObjectProducer.clone(
      input   = cms.InputTag("patTriggerEvent"),
      branch = cms.string("triggerObjectsDoubleMediumTau40"),
      hltPath = cms.string("HLT_DoubleMediumIsoPFTau40_Trk1_eta2p1_Reg_v"),
      inputIsStandAlone = cms.bool(False),
      storeOnlyIfFired = cms.bool(False)
      )
  
process.icLooseTau50Met120ObjectProducer = producers.icTriggerObjectProducer.clone(
      input   = cms.InputTag("patTriggerEvent"),
      branch = cms.string("triggerObjectsLooseTau50Met120"),
      hltPath = cms.string("HLT_LooseIsoPFTau50_Trk30_eta2p1_MET120_v"),
      inputIsStandAlone = cms.bool(False),
      storeOnlyIfFired = cms.bool(False)
      )
  
process.icLooseTau50Met80ObjectProducer = producers.icTriggerObjectProducer.clone(
      input   = cms.InputTag("patTriggerEvent"),
      branch = cms.string("triggerObjectsLooseTau50Met80"),
      hltPath = cms.string("HLT_LooseIsoPFTau50_Trk30_eta2p1_MET80_v"),
      inputIsStandAlone = cms.bool(False),
      storeOnlyIfFired = cms.bool(False)
      )
  
  
process.icTriggerObjectSequence += cms.Sequence(
      process.icEle12Mu23ObjectProducer +
      process.icEle23Mu8ObjectProducer +
      process.icEle12Mu17ObjectProducer +
      process.icEle17Mu8ObjectProducer +
      process.icEle22LooseTau20ObjectProducer +
      process.icEle22GsfObjectProducer +
      process.icEle23ObjectProducer +
      process.icEle27GsfObjectProducer + 
      process.icEle32TightGsfObjectProducer +
      process.icIsoMu17LooseTau20ObjectProducer +
      process.icIsoMu22ObjectProducer+
      process.icIsoMu18ObjectProducer+
#      process.icIsoMu24IterTrkObjectProducer +
#      process.icIsoMu27IterTrkObjectProducer +
      process.icIsoMu17IterTrkObjectProducer +
      process.icDoubleMediumTau40ObjectProducer +
      process.icDoubleMediumTau35ObjectProducer #+
#      process.icLooseTau50Met120ObjectProducer +
#      process.icLooseTau50Met80ObjectProducer
      )

#if isData:
#  process.icTriggerObjectSequence.remove(process.icEle22GsfObjectProducer)
#  process.icTriggerObjectSequence.remove(process.icIsoMu17IterTrkObjectProducer)
#  process.icTriggerObjectSequence.remove(process.icLooseTau50Met80ObjectProducer)
#  process.icTriggerObjectSequence.remove(process.icLooseTau50Met120ObjectProducer)
#  process.icIsoMu24IterTrkObjectProducer.hltPath=cms.string("HLT_IsoTkMu24_eta2p1_v")
#  process.icTriggerObjectSequence.remove(process.icIsoMu24IterTrkObjectProducer)
#  process.icIsoMu27IterTrkObjectProducer.hltPath=cms.string("HLT_IsoTkMu27_v")
#  process.icIsoMu22ObjectProducer = producers.icTriggerObjectProducer.clone(
#      input   = cms.InputTag("patTriggerEvent"),
#      branch = cms.string("triggerObjectsIsoMu22"),
#      hltPath = cms.string("HLT_IsoMu22_v"),
#      inputIsStandAlone = cms.bool(False),
#      storeOnlyIfFired = cms.bool(False)
#      )
#  process.icIsoMu18ObjectProducer = producers.icTriggerObjectProducer.clone(
#      input = cms.InputTag("patTriggerEvent"),
#      branch = cms.string("triggerObjectsIsoMu18"),
#      hltPath = cms.string("HLT_IsoMu18_v"),
#      inputIsStandAlone = cms.bool(False),
#      storeOnlyIfFired = cms.bool(False)
#      )

#  process.icEle22ObjectProducer = producers.icTriggerObjectProducer.clone(
#      input = cms.InputTag("patTriggerEvent"),
#      branch = cms.string("triggerObjectsEle22"),
#      hltPath = cms.string("HLT_Ele22_eta2p1_WPLoose_Gsf_v"),
#      inputIsStandAlone = cms.bool(False),
#      storeOnlyIfFired = cms.bool(False)
#      )

#  process.icEle23ObjectProducer = producers.icTriggerObjectProducer.clone(
#      input = cms.InputTag("patTriggerEvent"),
#      branch = cms.string("triggerObjectsEle23"),
#      hltPath = cms.string("HLT_Ele23_WPLoose_Gsf_v"),
#      inputIsStandAlone = cms.bool(False),
#      storeOnlyIfFired = cms.bool(False)
#      )
#
#
#  process.icTau35ObjectProducer = producers.icTriggerObjectProducer.clone(
#     input = cms.InputTag("patTriggerEvent"),
#     branch = cms.string("triggerObjectsDiTau35"),
#     hltPath = cms.string("HLT_DoubleMediumIsoPFTau35_Trk1_eta2p1_Reg_v"),
#     inputIsStandAlone = cms.bool(False),
#     storeOnlyIfFired = cms.bool(False)
#     )

#  process.icTriggerObjectSequence += cms.Sequence(
#    process.icIsoMu22ObjectProducer+
#    process.icIsoMu18ObjectProducer+
##    process.icEle22ObjectProducer+
#    process.icEle23ObjectProducer+
#    process.icTau35ObjectProducer+
#    process.icTau35ObjectProducer
#  )


#  process.icDoubleMediumTau40ObjectProducer = producers.icTriggerObjectProducer.clone(
#      input   = cms.InputTag("patTriggerEvent"),
#      branch = cms.string("triggerObjectsDoubleMediumTau40"),
#      hltPath = cms.string("HLT_DoubleMediumIsoPFTau40_Trk1_eta2p1_Reg_v"),
#      inputIsStandAlone = cms.bool(False),
#      storeOnlyIfFired = cms.bool(False)
#      )
#
#  process.icIsoMu17LooseTau20ObjectProducer = producers.icTriggerObjectProducer.clone(
#      input   = cms.InputTag("patTriggerEvent"),
#      branch = cms.string("triggerObjectsIsoMu17LooseTau20"),
#      hltPath = cms.string("HLT_IsoMu17_eta2p1_LooseIsoPFTau20_v"),
#      inputIsStandAlone = cms.bool(False),
#      storeOnlyIfFired = cms.bool(False)
#      )
#
#  process.icIsoMu24IterTrkObjectProducer = producers.icTriggerObjectProducer.clone(
#      input   = cms.InputTag("patTriggerEvent"),
#      branch = cms.string("triggerObjectsIsoMu24"),
#      hltPath = cms.string("HLT_IsoMu24_eta2p1_v"),
#      inputIsStandAlone = cms.bool(False),
#      storeOnlyIfFired = cms.bool(False)
#      )
#
#  process.icIsoMu27IterTrkObjectProducer = producers.icTriggerObjectProducer.clone(
#      input   = cms.InputTag("patTriggerEvent"),
#      branch = cms.string("triggerObjectsIsoMu27"),
#      hltPath = cms.string("HLT_IsoMu27_v"), 
#      inputIsStandAlone = cms.bool(False),
#      storeOnlyIfFired = cms.bool(False)
#      )
#
#
#  process.icEle22LooseTau20ObjectProducer.hltPath = cms.string("HLT_Ele22_eta2p1_WPLoose_Gsf_LooseIsoPFTau20_v")
#  process.icEle27GsfObjectProducer.hltPath = cms.string("HLT_Ele27_eta2p1_WPLoose_Gsf_v") 
#  process.icEle32GsfObjectProducer.hltPath = cms.string("HLT_Ele32_eta2p1_WPTight_Gsf_v")
 
#  process.icEle12Mu23ObjectProducer = producers.icTriggerObjectProducer.clone(
#      input   = cms.InputTag("patTriggerEvent"),
#      branch = cms.string("triggerObjectsEle12Mu23"),
#      hltPath = cms.string("HLT_Mu23_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL_v"),
#      inputIsStandAlone = cms.bool(False),
#      storeOnlyIfFired = cms.bool(False)
#      )
#  
#  process.icEle23Mu8ObjectProducer = producers.icTriggerObjectProducer.clone(
#      input   = cms.InputTag("patTriggerEvent"),
#      branch = cms.string("triggerObjectsEle23Mu8"),
#      hltPath = cms.string("HLT_Mu8_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL_v"),
#      inputIsStandAlone = cms.bool(False),
#      storeOnlyIfFired = cms.bool(False)
#      )
#
#
#  process.icTriggerObjectSequence += cms.Sequence(
#    process.icDoubleMediumTau40ObjectProducer +
#    process.icIsoMu17LooseTau20ObjectProducer +
#    process.icEle22LooseTau20ObjectProducer +
#    process.icEle27GsfObjectProducer +
#    process.icEle32GsfObjectProducer +
#    process.icIsoMu24IterTrkObjectProducer +
#    process.icIsoMu27IterTrkObjectProducer +
#    process.icEle12Mu23ObjectProducer +
#    process.icEle23Mu8ObjectProducer 
#
#    )


  
if release in ['76XMINIAOD']:
  for name in process.icTriggerObjectSequence.moduleNames():
    mod = getattr(process, name)
    mod.inputIsStandAlone = cms.bool(True)
    mod.input = cms.InputTag("selectedPatTrigger")

################################################################
# EventInfo
################################################################
process.icEventInfoProducer = producers.icEventInfoProducer.clone(
  includeJetRho       = cms.bool(True),
  includeHT           = cms.bool(False),
  lheProducer         = cms.InputTag("externalLHEProducer"),
  inputJetRho         = cms.InputTag("fixedGridRhoFastjetAll"),
  includeLeptonRho    = cms.bool(False),
  inputLeptonRho      = cms.InputTag("fixedGridRhoFastjetAll"),
  includeVertexCount  = cms.bool(True),
  inputVertices       = vtxLabel,
  includeCSCFilter    = cms.bool(False),
  inputCSCFilter      = cms.InputTag("BeamHaloSummary"),
)

if doHT:
  process.icEventInfoProducer.includeHT = cms.bool(True)


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
# process.icPFSequence+
  process.icElectronSequence+
  process.icMuonSequence+
  process.icTauSequence+
  process.icTauProducer+
#  process.icL1ExtraTauProducer+
  #process.icL1ExtraMETProducer+
 # process.icTrackSequence+
  process.icPfMetSequence+
  process.icGenSequence+
  process.icPFJetSequence+
  process.icMvaMetSequence+
  process.icTriggerSequence+
  process.icTriggerObjectSequence+
  process.icEventInfoSequence+
  #process.patDefaultSequence+
  process.icEventProducer
)


# process.schedule = cms.Schedule(process.patTriggerPath, process.p)
process.schedule = cms.Schedule(process.p)

